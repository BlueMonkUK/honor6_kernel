/******************************************************************************

   Copyright(C)2013,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : ImsaProcApsMsg.c
  Description     : ��C�ļ�ʵ��APS��Ϣ�����APS��Ϣ����
  History           :
     1.sunbing 49683      2013-06-19  Draft Enact

******************************************************************************/

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "ImsaConnManagement.h"
#include "ImsaProcApsMsg.h"
#include "TafApsApi.h"
#include "ImsaPublic.h"
#include "ImsaIntraInterface.h"
#include "ImsaProcAtMsg.h"
#include "ImsaCallManagement.h"
#include "ImsaServiceManagement.h"
#include "ImsaRegManagement.h"
/*lint -e767*/
#define    THIS_FILE_ID  PS_FILE_ID_IMSAPROCAPSMSG_C
/*lint +e767*/

/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (FEATURE_ON == FEATURE_IMS)
/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/
const IMSA_CONN_PS_EVT_FUNC_TBL_STRU    g_astImsaConnPsEvtFuncTbl[] =
{
    /* PS CALL */
    {ID_EVT_TAF_PS_CALL_PDP_ACTIVATE_CNF,
        IMSA_CONN_ProcTafPsEvtPdpActivateCnf},
    {ID_EVT_TAF_PS_CALL_PDP_ACTIVATE_REJ,
        IMSA_CONN_ProcTafPsEvtPdpActivateRej},
    {ID_EVT_TAF_PS_CALL_PDP_ACTIVATE_IND,
        IMSA_CONN_ProcTafPsEvtPdpActivateInd},
    {ID_EVT_TAF_PS_CALL_PDP_MODIFY_IND,
        IMSA_CONN_ProcTafPsEvtPdpModifyInd},
    {ID_EVT_TAF_PS_CALL_PDP_DEACTIVATE_CNF,
        IMSA_CONN_ProcTafPsEvtPdpDeactivateCnf},
    {ID_EVT_TAF_PS_CALL_PDP_DEACTIVATE_IND,
        IMSA_CONN_ProcTafPsEvtPdpDeactivateInd},
    {ID_EVT_TAF_PS_CALL_PDP_IPV6_INFO_IND,
        IMSA_CONN_ProcTafPsEvtPdpIpv6InfoInd},
    {ID_EVT_TAF_PS_CALL_ORIG_CNF,
        IMSA_CONN_ProcTafPsEvtCallOrigCnf},
    {ID_EVT_TAF_PS_CALL_END_CNF,
        IMSA_CONN_ProcTafPsEvtCallEndCnf},
    {ID_EVT_TAF_PS_SRVCC_CANCEL_NOTIFY_IND,
        IMSA_CONN_ProcTafPsEvtSrvccCancelInd},
};

const VOS_UINT32 g_ulImsaConnPsEvtFuncTblSize  = sizeof(g_astImsaConnPsEvtFuncTbl) / sizeof(g_astImsaConnPsEvtFuncTbl[0]);


extern VOS_VOID IMSA_CONN_WaitForIpv6InfoProc
(
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType
);
/*
extern VOS_UINT32 IMSA_CONN_SipSignalPdpActOrig
(
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType,
    VOS_UINT8                           ucCid,
    IMSA_IP_TYPE_ENUM_UINT8             enIpType
);
*/
extern VOS_VOID  IMSA_CONN_ProcActCnfBack
(
    const TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU    *pstPdpActivateCnf
);
extern VOS_VOID  IMSA_CONN_ProcIpv4ActCnf
(
    IMSA_CONN_TYPE_ENUM_UINT32                  enConnType,
    const TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU    *pstPdpActivateCnf
);
extern VOS_VOID  IMSA_CONN_ProcIpv6ActCnf
(
    IMSA_CONN_TYPE_ENUM_UINT32                  enConnType,
    const TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU    *pstPdpActivateCnf
);
extern VOS_VOID  IMSA_CONN_ProcIpv4v6ActCnf
(
    IMSA_CONN_TYPE_ENUM_UINT32                  enConnType
);
extern VOS_VOID IMSA_CONN_ProcActRejCurReqIpv4v6
(
    IMSA_CONN_TYPE_ENUM_UINT32                  enConnType,
    const TAF_PS_CALL_PDP_ACTIVATE_REJ_STRU    *pstPdpActivateRej
);
extern VOS_VOID IMSA_CONN_ProcActRejCurReqIpv4
(
    IMSA_CONN_TYPE_ENUM_UINT32                  enConnType
);
extern VOS_VOID IMSA_CONN_ProcActRejCurReqIpv6
(
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType
);

extern VOS_UINT32 IMSA_CONN_MatchIPV4V6FallBackCause
(
    TAF_PS_CAUSE_ENUM_UINT32                    enCause
);

/*****************************************************************************
  3 Function
*****************************************************************************/
/*lint -e960*/
/*lint -e961*/

VOS_VOID IMSA_CONN_ProcTafPsEvt
(
    TAF_PS_EVT_STRU                    *pstEvt
)
{
    VOS_UINT32                          i           = IMSA_NULL;
    VOS_UINT32                          ulResult    = IMSA_NULL;
    IMSA_CONN_PS_EVT_FUNC               pPsEvtFunc  = VOS_NULL_PTR;


    /* ���¼�������в��Ҵ����� */
    for ( i = 0; i < g_ulImsaConnPsEvtFuncTblSize; i++ )
    {
        if ( pstEvt->ulEvtId == g_astImsaConnPsEvtFuncTbl[i].ulEvtId )
        {
            /* �¼�IDƥ�� */
            pPsEvtFunc = g_astImsaConnPsEvtFuncTbl[i].pPsEvtFunc;
            break;
        }
    }

    /* ������������������ */
    if ( VOS_NULL_PTR != pPsEvtFunc )
    {
        ulResult = pPsEvtFunc(pstEvt->aucContent);
    }
    else
    {
        IMSA_ERR_LOG1("IMSA_CONN_ProcTafPsEvt: Unexpected event received! <EvtId>",
            pstEvt->ulEvtId);
        ulResult    = VOS_ERR;
    }

    if ( VOS_OK != ulResult )
    {
        IMSA_ERR_LOG1("IMSA_CONN_ProcTafPsEvt: Handle this event failed! <EvtId>",
            pstEvt->ulEvtId);
    }

    return;
}
VOS_VOID IMSA_CONN_WaitForIpv6InfoProc
(
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType
)
{
    IMSA_NORMAL_CONN_STRU              *pstNormalConn   = VOS_NULL_PTR;
    IMSA_EMC_CONN_STRU                 *pstEmcConn      = VOS_NULL_PTR;
    IMSA_IP_ADDRESS_STRU               *pstPdpIpAddr    = VOS_NULL_PTR;
    VOS_UINT32                          i               = IMSA_NULL;
    VOS_UINT8                           aucTemp[IMSA_IPV6_PREFIX_LEN] = {0};

    /* ��ȡPRIM PDP�����ĸ�ʽ */
    if (IMSA_CONN_TYPE_NORMAL == enConnType)
    {
        pstNormalConn                   = IMSA_CONN_GetNormalConnAddr();
        for (i = 0; i < pstNormalConn->ulSipSignalPdpNum; i++)
        {
            pstPdpIpAddr = &pstNormalConn->astSipSignalPdpArray[i].stPdpAddr;
            if (((IMSA_IP_TYPE_IPV6 == pstPdpIpAddr->enIpType)
                    || (IMSA_IP_TYPE_IPV4V6 == pstPdpIpAddr->enIpType))
                && (0 == IMSA_MEM_CMP(aucTemp, pstPdpIpAddr->aucIpV6Addr, IMSA_IPV6_PREFIX_LEN)))
            {
                /* �����ȴ�IPV6������ʱ�����ȴ�APS��ID_EVT_TAF_PS_CALL_PDP_IPV6_INFO_IND��Ϣ��
                   ���޸�����״̬ */
                IMSA_CONN_StartTimer(IMSA_CONN_TYPE_NORMAL, TI_IMSA_WAIT_IPV6_INFO);

                return ;
            }
        }
    }
    else
    {
        pstEmcConn                      = IMSA_CONN_GetEmcConnAddr();
        pstPdpIpAddr = &pstEmcConn->stSipSignalPdp.stPdpAddr;
        if (((IMSA_IP_TYPE_IPV6 == pstPdpIpAddr->enIpType)
                || (IMSA_IP_TYPE_IPV4V6 == pstPdpIpAddr->enIpType))
            && (0 == IMSA_MEM_CMP(aucTemp, pstPdpIpAddr->aucIpV6Addr, IMSA_IPV6_PREFIX_LEN)))
        {
            /* �����ȴ�IPV6������ʱ�����ȴ�APS��ID_EVT_TAF_PS_CALL_PDP_IPV6_INFO_IND��Ϣ��
               ���޸�����״̬ */
            IMSA_CONN_StartTimer(IMSA_CONN_TYPE_EMC, TI_IMSA_WAIT_IPV6_INFO);

            return ;
        }
    }

    IMSA_CONN_SetupConnSucc(enConnType);
}


VOS_UINT32 IMSA_CONN_SipSignalPdpActOrig
(
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType,
    VOS_UINT8                           ucCid,
    IMSA_IP_TYPE_ENUM_UINT8             enIpType
)
{
    TAF_PS_DIAL_PARA_STRU              *pstDialParaInfo     = VOS_NULL_PTR;
    /* TAF_PDP_PRIM_CONTEXT_EXT_STRU       stPdpPrimContextExt = {0}; */
    IMSA_NORMAL_CONN_STRU              *pstNormalConn       = VOS_NULL_PTR;
    IMSA_EMC_CONN_STRU                 *pstEmcConn          = VOS_NULL_PTR;
    VOS_UINT8                           ucOpid              = IMSA_NULL;

    pstNormalConn   = IMSA_CONN_GetNormalConnAddr();
    pstEmcConn      = IMSA_CONN_GetEmcConnAddr();

    IMSA_INFO_LOG1("IMSA_CONN_SipSignalPdpActOrig: enConnType:", enConnType);
    IMSA_INFO_LOG1("IMSA_CONN_SipSignalPdpActOrig: ucCid:", ucCid);
    IMSA_INFO_LOG1("IMSA_CONN_SipSignalPdpActOrig: enIpType:", enIpType);

    /* ��ȡPRIM PDP�����ĸ�ʽ */
    /*
    if (IMSA_CONN_TYPE_NORMAL == enConnType)
    {
        IMSA_CONN_GetPrimPdpCntFromSelSdfPara(&pstNormalConn->stSelSdfPara, &stPdpPrimContextExt);
    }
    else
    {
        IMSA_CONN_GetPrimPdpCntFromSelSdfPara(&pstEmcConn->stSelSdfPara, &stPdpPrimContextExt);
    }

    stPdpPrimContextExt.enPdpType   = enIpType;
    stPdpPrimContextExt.ucCid       = ucCid;

    IMSA_CONN_AssignOpid(enConnType, &ucOpid);

    if (VOS_ERR == TAF_PS_SetPrimPdpContextInfo(PS_PID_IMSA, IMSA_CLIENT_ID,
                                                ucOpid, &stPdpPrimContextExt))
    {
        IMSA_ERR_LOG("IMSA_CONN_SipSignalPdpActOrig:TAF_PS_SetPrimPdpContextInfo failed!");

        return IMSA_FAIL;
    }
    */

    /* ��ȡ���Ų��� */
    pstDialParaInfo = IMSA_MEM_ALLOC(sizeof(TAF_PS_DIAL_PARA_STRU));
    if (VOS_NULL_PTR == pstDialParaInfo)
    {
        /*��ӡ���Ϸ���Ϣ*/
        IMSA_ERR_LOG("IMSA_CONN_SipSignalPdpActOrig:TAF_PS_DIAL_PARA_STRU ERROR: Mem alloc fail!");

        return IMSA_FAIL;
    }

    if (IMSA_CONN_TYPE_NORMAL == enConnType)
    {
        IMSA_CONN_GetImsDailParaFromSelSdfPara(&pstNormalConn->stSelSdfPara, pstDialParaInfo);

        pstNormalConn->stSelSdfPara.ucCid = ucCid;
        pstNormalConn->stSelSdfPara.enPdnType = enIpType;
    }
    else
    {
        IMSA_CONN_GetImsDailParaFromSelSdfPara(&pstEmcConn->stSelSdfPara, pstDialParaInfo);

        pstEmcConn->stSelSdfPara.ucCid = ucCid;
        pstEmcConn->stSelSdfPara.enPdnType = enIpType;
    }

    pstDialParaInfo->ucCid        = ucCid;
    pstDialParaInfo->enPdpType    = enIpType;


    IMSA_CONN_AssignOpid(enConnType, &ucOpid);
    /* IMSA_CONN_SetNormalConnOpid(ucOpid); */
    IMSA_CONN_SetOpid(enConnType, ucOpid);

    if (VOS_OK == TAF_PS_CallOrig(PS_PID_IMSA,IMSA_CLIENT_ID, ucOpid, pstDialParaInfo))
    {
        /* ����IMS���Ŷ�ʱ�� */
        IMSA_CONN_StartTimer(enConnType, TI_IMSA_SIP_SIGAL_PDP_ORIG);

        /* �޸ĵ�ǰ���������PDN���� */
        IMSA_CONN_SaveCurReqPdnType(enConnType, enIpType);

        IMSA_MEM_FREE(pstDialParaInfo);

        return IMSA_SUCC;
    }

    IMSA_MEM_FREE(pstDialParaInfo);
    return IMSA_FAIL;
}
VOS_VOID  IMSA_CONN_ProcActCnfBack
(
    const TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU    *pstPdpActivateCnf
)
{
    VOS_UINT8                           ucCid   = IMSA_NULL;
    VOS_UINT32                          ulRet   = IMSA_FAIL;
    VOS_UINT8                           ucOpid  = IMSA_NULL;

    IMSA_INFO_LOG("IMSA_CONN_ProcActCnfBack is entered!");

    if ((VOS_TRUE == pstPdpActivateCnf->bitOpCause)
        && (TAF_PS_CAUSE_SM_NW_SINGLE_ADDR_BEARERS_ONLY_ALLOWED != pstPdpActivateCnf->enCause))
    {
        IMSA_INFO_LOG("IMSA_CONN_ProcActCnfBack:Not Back!");

        /* ���ᷢ����һ��SIP������صĽ��� */

        /* ���IPV6ȫ�ֵ�ַ�ѻ�ã��ظ����ӽ����ɹ������������ȴ�IPV6������ʱ�� */
        IMSA_CONN_WaitForIpv6InfoProc(IMSA_CONN_TYPE_NORMAL);
        return ;
    }

    if (TAF_PDP_IPV4 == pstPdpActivateCnf->stPdpAddr.enPdpType)
    {
        if (IMSA_PDP_STATE_ACTIVE == IMSA_CONN_GetSipSignalPdpState(IMSA_CONN_TYPE_NORMAL,
                                                                    TAF_PDP_IPV6))
        {
            IMSA_INFO_LOG("IMSA_CONN_ProcActCnfBack:IPV6 is already active!");

            /* ���IPV6ȫ�ֵ�ַ�ѻ�ã��ظ����ӽ����ɹ������������ȴ�IPV6������ʱ�� */
            IMSA_CONN_WaitForIpv6InfoProc(IMSA_CONN_TYPE_NORMAL);
            return ;
        }

        /* ����һ��δ�����CID���м��� */
        IMSA_CONN_AssignOpid(IMSA_CONN_TYPE_NORMAL, &ucOpid);
        ulRet = TAF_PS_GetUnusedCid(PS_PID_IMSA, IMSA_CLIENT_ID, ucOpid, &ucCid);
        if (VOS_OK != ulRet)
        {
            IMSA_WARN_LOG("IMSA_CONN_ProcActCnfBack:IPV4,get cid failed");

            IMSA_CONN_SetupConnSucc(IMSA_CONN_TYPE_NORMAL);
            return ;
        }

        ulRet = IMSA_CONN_SipSignalPdpActOrig(IMSA_CONN_TYPE_NORMAL, ucCid, TAF_PDP_IPV6);
        if (IMSA_SUCC != ulRet)
        {
            IMSA_WARN_LOG("IMSA_CONN_ProcActCnfBack:IPV4,IMSA_CONN_SipSignalPdpActOrig failed");

            IMSA_CONN_SetupConnSucc(IMSA_CONN_TYPE_NORMAL);
            return ;
        }
        return ;
    }
    else if (TAF_PDP_IPV6 == pstPdpActivateCnf->stPdpAddr.enPdpType)
    {
        if (IMSA_PDP_STATE_ACTIVE == IMSA_CONN_GetSipSignalPdpState(IMSA_CONN_TYPE_NORMAL,
                                                                    TAF_PDP_IPV4))
        {
            IMSA_INFO_LOG("IMSA_CONN_ProcActCnfBack:IPV4 is already active!");

            /* ���IPV6ȫ�ֵ�ַ�ѻ�ã��ظ����ӽ����ɹ������������ȴ�IPV6������ʱ�� */
            IMSA_CONN_WaitForIpv6InfoProc(IMSA_CONN_TYPE_NORMAL);\
            return ;
        }

        /* ����һ��δ�����CID���м��� */
        IMSA_CONN_AssignOpid(IMSA_CONN_TYPE_NORMAL, &ucOpid);
        ulRet = TAF_PS_GetUnusedCid(PS_PID_IMSA, IMSA_CLIENT_ID, ucOpid, &ucCid);
        if (VOS_OK != ulRet)
        {
            IMSA_WARN_LOG("IMSA_CONN_ProcActCnfBack:IPV6,get cid failed");

            /* ���IPV6ȫ�ֵ�ַ�ѻ�ã��ظ����ӽ����ɹ������������ȴ�IPV6������ʱ�� */
            IMSA_CONN_WaitForIpv6InfoProc(IMSA_CONN_TYPE_NORMAL);
            return ;
        }

        ulRet = IMSA_CONN_SipSignalPdpActOrig(IMSA_CONN_TYPE_NORMAL, ucCid, TAF_PDP_IPV4);
        if (IMSA_SUCC != ulRet)
        {
            IMSA_WARN_LOG("IMSA_CONN_ProcActCnfBack:IPV6,IMSA_CONN_SipSignalPdpActOrig failed");

            /* ���IPV6ȫ�ֵ�ַ�ѻ�ã��ظ����ӽ����ɹ������������ȴ�IPV6������ʱ�� */
            IMSA_CONN_WaitForIpv6InfoProc(IMSA_CONN_TYPE_NORMAL);
            return ;
        }
        return ;
    }
    else
    {
        ;
    }

    return;
}


VOS_VOID  IMSA_CONN_ProcIpv4ActCnf
(
    IMSA_CONN_TYPE_ENUM_UINT32                  enConnType,
    const TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU    *pstPdpActivateCnf
)
{
    IMSA_NORMAL_CONN_STRU              *pstNormalConn   = VOS_NULL_PTR;
    VOS_UINT8                           ucCid           = IMSA_NULL;
    VOS_UINT32                          ulRet           = IMSA_FAIL;
    VOS_UINT8                           ucOpid          = IMSA_NULL;

    if (IMSA_CONN_TYPE_EMC == enConnType)
    {
        IMSA_INFO_LOG("IMSA_CONN_ProcIpv4ActCnf:EMC!");

        /* ����3GPPЭ��Ҫ��ͬһʱ��ֻ�ܴ���һ���������أ���˲����ٷ�����һ������
           ���ؽ��� */

        IMSA_CONN_SetupConnSucc(IMSA_CONN_TYPE_EMC);

        return ;
    }

    pstNormalConn   = IMSA_CONN_GetNormalConnAddr();

    /* �����IPV4V6����Ҫ������һ��PDP */
    if (IMSA_IP_TYPE_IPV4V6 != pstNormalConn->enFirstReqPdnType)
    {
        IMSA_INFO_LOG("IMSA_CONN_ProcIpv4ActCnf:Not ipv4v6");

        IMSA_CONN_SetupConnSucc(IMSA_CONN_TYPE_NORMAL);

        return ;
    }

    switch (pstNormalConn->enCurReqPdnType)
    {
        /* �������������PDP���ˣ���ǰ��PDP����ܾ��󣬷ֱ���IPV4��
           IPV6��PDP���*/
        case IMSA_IP_TYPE_IPV4:
            if (IMSA_PDP_STATE_ACTIVE == IMSA_CONN_GetSipSignalPdpState(IMSA_CONN_TYPE_NORMAL,
                                                                        IMSA_IP_TYPE_IPV6))
            {
                IMSA_INFO_LOG("IMSA_CONN_ProcIpv4ActCnf:ipv6 already active");

                /* ���IPV6ȫ�ֵ�ַ�ѻ�ã��ظ����ӽ����ɹ������������ȴ�IPV6������ʱ�� */
                IMSA_CONN_WaitForIpv6InfoProc(IMSA_CONN_TYPE_NORMAL);
                return ;
            }

            /* ����һ��δ�����CID���м��� */
            IMSA_CONN_AssignOpid(enConnType, &ucOpid);
            ulRet = TAF_PS_GetUnusedCid(PS_PID_IMSA, IMSA_CLIENT_ID, ucOpid, &ucCid);

            if (VOS_OK != ulRet)
            {
                IMSA_WARN_LOG("IMSA_CONN_ProcIpv4ActCnf:get cid failed!");

                IMSA_CONN_SetupConnSucc(IMSA_CONN_TYPE_NORMAL);
                return ;
            }

            ulRet = IMSA_CONN_SipSignalPdpActOrig(IMSA_CONN_TYPE_NORMAL, ucCid, TAF_PDP_IPV6);
            if (IMSA_SUCC != ulRet)
            {
                IMSA_WARN_LOG("IMSA_CONN_ProcIpv4ActCnf:IMSA_CONN_SipSignalPdpActOrig failed!");

                IMSA_CONN_SetupConnSucc(IMSA_CONN_TYPE_NORMAL);
                return ;
            }

            break;

        /* ������PDP������˹���ʵ�� */
        case TAF_PDP_IPV4V6:
            IMSA_CONN_ProcActCnfBack(pstPdpActivateCnf);
            break;

        default:
            IMSA_WARN_LOG("IMSA_CONN_ProcIpv4ActCnf:current requst Pdn Type is error!");
            break;
    }

    return;
}
VOS_VOID  IMSA_CONN_ProcIpv6ActCnf
(
    IMSA_CONN_TYPE_ENUM_UINT32                  enConnType,
    const TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU    *pstPdpActivateCnf
)
{
    IMSA_NORMAL_CONN_STRU              *pstNormalConn   = VOS_NULL_PTR;

    if (IMSA_CONN_TYPE_EMC == enConnType)
    {
        /* ����3GPPЭ��Ҫ��ͬһʱ��ֻ�ܴ���һ���������أ���˲����ٷ�����һ������
           ���ؽ��� */

        /* ���IPV6ȫ�ֵ�ַ�ѻ�ã��ظ����ӽ����ɹ������������ȴ�IPV6������ʱ�� */
        IMSA_CONN_WaitForIpv6InfoProc(IMSA_CONN_TYPE_EMC);

        return ;
    }

    pstNormalConn   = IMSA_CONN_GetNormalConnAddr();

    /* �����IPV4V6����Ҫ������һ��PDP */
    if (IMSA_IP_TYPE_IPV4V6 != pstNormalConn->enFirstReqPdnType)
    {
        IMSA_INFO_LOG("IMSA_CONN_ProcIpv6ActCnf:Not ipv4v6");

        /* ���IPV6ȫ�ֵ�ַ�ѻ�ã��ظ����ӽ����ɹ������������ȴ�IPV6������ʱ�� */
        IMSA_CONN_WaitForIpv6InfoProc(IMSA_CONN_TYPE_NORMAL);

        return ;
    }

    /* �����IPV4V6����Ҫ������һ��PDP */
    switch (pstNormalConn->enCurReqPdnType)
    {
        case TAF_PDP_IPV6:
            /* �������������PDP���ˣ���ǰ��PDP����ܾ��󣬷ֱ���IPV4��
               IPV6��PDP����, IPV6������ٷ���IPV4��PDP���� */

            /* ���IPV6ȫ�ֵ�ַ�ѻ�ã��ظ����ӽ����ɹ������������ȴ�IPV6������ʱ�� */
            IMSA_CONN_WaitForIpv6InfoProc(IMSA_CONN_TYPE_NORMAL);
            break;

        /* ������PDP������˹���ʵ�� */
        case TAF_PDP_IPV4V6:

           IMSA_CONN_ProcActCnfBack(pstPdpActivateCnf);
           break;

        default:
           IMSA_WARN_LOG("IMSA_CONN_ProcIpv4ActCnf:current requst Pdn Type is error!");
           break;
    }

    return;
}


VOS_VOID  IMSA_CONN_ProcIpv4v6ActCnf
(
    IMSA_CONN_TYPE_ENUM_UINT32                  enConnType
)
{
    /* ���IPV6ȫ�ֵ�ַ�ѻ�ã��ظ����ӽ����ɹ������������ȴ�IPV6������ʱ�� */
    IMSA_CONN_WaitForIpv6InfoProc(enConnType);
}
VOS_UINT32 IMSA_CONN_MatchIPV4V6FallBackCause
(
    TAF_PS_CAUSE_ENUM_UINT32                    enCause
)
{
    IMSA_CONN_MANAGER_STRU              *pstConnManager;
    VOS_UINT32                          ulCnt               = IMSA_NULL;

    IMSA_INFO_LOG("IMSA_CONN_MatchIPV4V6FallBackCause is entered!");

    if (TAF_PS_CAUSE_SM_NW_UNKNOWN_PDP_ADDR_OR_TYPE == enCause)
    {
        IMSA_INFO_LOG("IMSA_CONN_MatchIPV4V6FallBackCause: cause match #28!");
        return IMSA_SUCC;
    }

    pstConnManager = IMSA_CONN_GetConnManagerAddr();
    for (ulCnt = 0; ulCnt < pstConnManager->stIpv6FallBackExtCause.ulCauseNum; ulCnt++)
    {
        if (pstConnManager->stIpv6FallBackExtCause.aenPsCause[ulCnt] == enCause)
        {
            IMSA_INFO_LOG("IMSA_CONN_MatchIPV4V6FallBackCause: cause match succ!");
            return IMSA_SUCC;
        }
    }

    return IMSA_FAIL;
}


VOS_VOID IMSA_CONN_ProcActRejCurReqIpv4v6
(
    IMSA_CONN_TYPE_ENUM_UINT32                  enConnType,
    const TAF_PS_CALL_PDP_ACTIVATE_REJ_STRU    *pstPdpActivateRej
)
{
    VOS_UINT32                          ulResult            = IMSA_FAIL;
    VOS_UINT8                           ucCid               = IMSA_NULL;

    /* �����������Ҫ�ֱ���IPv4��IPv6��PDP���� */
    if (IMSA_SUCC == IMSA_CONN_MatchIPV4V6FallBackCause(pstPdpActivateRej->enCause))
    {
        IMSA_INFO_LOG("IMSA_CONN_ProcActRejCurReqIpv4v6: match fallback cause succ!");

        IMSA_CONN_GetConnSelectedCid(enConnType, ucCid);
        ulResult = IMSA_CONN_SipSignalPdpActOrig(enConnType, ucCid, IMSA_IP_TYPE_IPV4);
        if (IMSA_SUCC != ulResult)
        {
            IMSA_WARN_LOG("IMSA_CONN_ProcActRejCurReqIpv4v6:unknown pdp addr or type,IMSA_CONN_SipSignalPdpActOrig failed!");

            IMSA_CONN_SetupConnFail(enConnType, IMSA_CONN_RESULT_FAIL_CN_REJ);

            return ;
        }

        return ;
    }

    if (IMSA_IP_TYPE_IPV4V6 == pstPdpActivateRej->enPdpType)
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcActRejCurReqIpv4v6:ipv4v6");

        IMSA_CONN_SetupConnFail(enConnType, IMSA_CONN_RESULT_FAIL_CN_REJ);
        return ;
    }

    if ((IMSA_OP_TRUE == pstPdpActivateRej->bitOpCauseEx)
        && (TAF_PS_CAUSE_SM_NW_SINGLE_ADDR_BEARERS_ONLY_ALLOWED != pstPdpActivateRej->enCauseEx))
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcActRejCurReqIpv4v6:Causeex is not sigle addr bearers only allowed!");

        IMSA_CONN_SetupConnFail(enConnType, IMSA_CONN_RESULT_FAIL_CN_REJ);
        return ;
    }

    if (IMSA_IP_TYPE_IPV4 == pstPdpActivateRej->enPdpType)
    {
        IMSA_CONN_GetConnSelectedCid(enConnType, ucCid);
        ulResult = IMSA_CONN_SipSignalPdpActOrig(enConnType, ucCid, IMSA_IP_TYPE_IPV6);
        if (IMSA_SUCC != ulResult)
        {
            IMSA_WARN_LOG("IMSA_CONN_ProcActRejCurReqIpv4v6:qos not accepted,ipv4,IMSA_CONN_SipSignalPdpActOrig failed!");

            IMSA_CONN_SetupConnFail(enConnType, IMSA_CONN_RESULT_FAIL_CN_REJ);
            return ;
        }

        return ;
    }
    else
    {
        IMSA_CONN_GetConnSelectedCid(enConnType, ucCid);
        ulResult = IMSA_CONN_SipSignalPdpActOrig(enConnType, ucCid, IMSA_IP_TYPE_IPV4);
        if (IMSA_SUCC != ulResult)
        {
            IMSA_WARN_LOG("IMSA_CONN_ProcActRejCurReqIpv4v6:qos not accepted,ipv6,IMSA_CONN_SipSignalPdpActOrig failed!");

            IMSA_CONN_SetupConnFail(enConnType, IMSA_CONN_RESULT_FAIL_CN_REJ);
            return ;
        }

        /* ����IPV6�Ѿ�ȷ������ɹ���Ϊ�˷�ֹ�����ٳ���IPV6���ͣ���
           first requst pdn type����ΪIPV4 */
        IMSA_CONN_SaveFirstReqPdnType(enConnType, IMSA_IP_TYPE_IPV4);

        return ;
    }
}


VOS_VOID IMSA_CONN_ProcActRejCurReqIpv4
(
    IMSA_CONN_TYPE_ENUM_UINT32                  enConnType
)
{
    VOS_UINT32                          ulResult            = IMSA_FAIL;
    VOS_UINT8                           ucCid               = IMSA_NULL;

    /* �û�����IPv4v6���͵�PDP����, ���ұ�����ܾ�, ԭ��Ϊ28, Э��ջ��Ҫ
       �ֱ���IPv4/IPv6���͵�PDP����, Э��ջ���ȷ���IPv4, �ٷ���IPv6,
       ���IPV4���͵�PDP�����ٴα�����ܾ�, Э��ջ����Ҫ����IPV6���͵�
       PDP����Ϊ�˷�ֹPDP����Ƕ��, ���IPv6���͵�PDP����ʧ��, �����ٳ���
       IPv4���͵�PDP���� */
    if (IMSA_PDP_STATE_ACTIVE == IMSA_CONN_GetSipSignalPdpState(enConnType, IMSA_IP_TYPE_IPV6))
    {
        IMSA_INFO_LOG("IMSA_CONN_ProcActRejCurReqIpv4:ipv6 already active");

        /* ���IPV6ȫ�ֵ�ַ�ѻ�ã��ظ����ӽ����ɹ������������ȴ�IPV6������ʱ�� */
        IMSA_CONN_WaitForIpv6InfoProc(enConnType);
        return ;
    }

    IMSA_CONN_GetConnSelectedCid(enConnType, ucCid);
    ulResult = IMSA_CONN_SipSignalPdpActOrig(enConnType, ucCid, IMSA_IP_TYPE_IPV6);
    if (IMSA_SUCC != ulResult)
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcActRejCurReqIpv4:IMSA_CONN_SipSignalPdpActOrig failed!");

        IMSA_CONN_SetupConnFail(enConnType, IMSA_CONN_RESULT_FAIL_CN_REJ);
        return ;
    }
}
VOS_VOID IMSA_CONN_ProcActRejCurReqIpv6
(
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType
)
{
    if (IMSA_PDP_STATE_ACTIVE == IMSA_CONN_GetSipSignalPdpState(enConnType, IMSA_IP_TYPE_IPV4))
    {
        IMSA_INFO_LOG("IMSA_CONN_ProcActRejCurReqIpv6:ipv4 already active");

        IMSA_CONN_SetupConnSucc(enConnType);
        return ;
    }
    else
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcActRejCurReqIpv6:ipv4 not active,ipv6 rej!");

        IMSA_CONN_SetupConnFail(enConnType, IMSA_CONN_RESULT_FAIL_CN_REJ);
        return ;
    }
}


VOS_UINT32 IMSA_CONN_ProcTafPsEvtPdpActivateCnf
(
    VOS_VOID                           *pEvtInfo
)
{
    VOS_UINT32                          ulResult            = IMSA_FAIL;
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstPdpActivateCnf   = VOS_NULL_PTR;
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType          = IMSA_CONN_TYPE_BUTT;
    IMSA_NORMAL_CONN_STRU              *pstNormalConn       = VOS_NULL_PTR;
    IMSA_EMC_CONN_STRU                 *pstEmcConn          = VOS_NULL_PTR;
    IMSA_PDP_CNTXT_INFO_STRU           *pstPdpInfo          = VOS_NULL_PTR;

    /* ��ʼ�� */
    pstPdpActivateCnf  = (TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU*)pEvtInfo;

    IMSA_INFO_LOG1("IMSA_CONN_ProcTafPsEvtPdpActivateCnf is entered! opid:", pstPdpActivateCnf->stCtrl.ucOpId);

    pstNormalConn                   = IMSA_CONN_GetNormalConnAddr();
    pstEmcConn                      = IMSA_CONN_GetEmcConnAddr();

    /* ����OPID���������������ʵ������ */
    ulResult = IMSA_CONN_GetConnTypeByOpid( pstPdpActivateCnf->stCtrl.ucOpId,
                                            &enConnType);

    /* �������ʧ�ܣ���ֱ���˳� */
    if (IMSA_FAIL == ulResult)
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsCallEvtPdpActivateCnf:Get Opid Failed!");
        return IMSA_FAIL;
    }

    /* ����CONNING״̬����ֱ���˳� */
    if (IMSA_FALSE == IMSA_CONN_IsConnStatusEqual(enConnType, IMSA_CONN_STATUS_CONNING))
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsCallEvtPdpActivateCnf:CONNING state!");
        return IMSA_FAIL;
    }

    /* �������ȱʡ���أ���ֱ�Ӷ�������ʱ֧��ȱʡ�������͵�SIP������� */
    if (IMSA_OP_TRUE == pstPdpActivateCnf->bitOpLinkdRabId)
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsCallEvtPdpActivateCnf:Illegal Bearer Type!");
        return IMSA_FAIL;
    }

    /* �洢SIP���������Ϣ������P-CSCF��DNS��IP��ַ��Ϣ�� */
    if (IMSA_CONN_TYPE_NORMAL == enConnType)
    {
        if (IMSA_CONN_MAX_NORMAL_SIP_SIGNAL_PDP_NUM == pstNormalConn->ulSipSignalPdpNum)
        {
            IMSA_WARN_LOG("IMSA_CONN_ProcTafPsCallEvtPdpActivateCnf:signal pdp is already max num!");
            return IMSA_FAIL;
        }

        pstPdpInfo                      = &pstNormalConn->astSipSignalPdpArray[pstNormalConn->ulSipSignalPdpNum];

        IMSA_CONN_SaveSipSignalPdpInfo( pstPdpInfo, pstPdpActivateCnf);

        pstNormalConn->ulSipSignalPdpNum++;
    }
    else
    {
        pstPdpInfo                      = &pstEmcConn->stSipSignalPdp;

        IMSA_CONN_SaveSipSignalPdpInfo( pstPdpInfo, pstPdpActivateCnf);
    }

    /* ��REGģ�����õ�ַ�� */
    IMSA_CONN_ConfigPdpInfo2Reg(enConnType, pstPdpInfo);

    /* ����������IP��ַ��DNS��ַ */
    /* IMSA_CONN_ConfigPdpInfo2Bsp(pstPdpInfo); */

    /* ֹͣIMS���Ŷ�ʱ�� */
    IMSA_CONN_StopTimer(enConnType, TI_IMSA_SIP_SIGAL_PDP_ORIG);

    /* ��CDS�������й��˳��� */
    IMSA_CONN_SndCdsSetImsBearerReq();

    /* ����PDP���ͷֱ���*/
    switch ( pstPdpActivateCnf->stPdpAddr.enPdpType )
    {
        case TAF_PDP_IPV4:
            IMSA_CONN_ProcIpv4ActCnf(enConnType, pstPdpActivateCnf);
            break;

        case TAF_PDP_IPV6:
            IMSA_CONN_ProcIpv6ActCnf(enConnType, pstPdpActivateCnf);
            break;

        case TAF_PDP_IPV4V6:
            IMSA_CONN_ProcIpv4v6ActCnf(enConnType);
            break;

        default:
            IMSA_WARN_LOG("IMSA_CONN_ProcTafPsCallEvtPdpActivateCnf:pdp type invaild!");
            break;
    }

    return IMSA_SUCC;
}



VOS_UINT32 IMSA_CONN_ProcTafPsEvtPdpActivateRej
(
    VOS_VOID                           *pEvtInfo
)
{
    VOS_UINT32                          ulResult            = IMSA_FAIL;
    TAF_PS_CALL_PDP_ACTIVATE_REJ_STRU  *pstPdpActivateRej   = VOS_NULL_PTR;
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType          = IMSA_CONN_TYPE_BUTT;
    IMSA_IP_TYPE_ENUM_UINT8             enFirstReqPdnType   = IMSA_IP_TYPE_BUTT;
    IMSA_IP_TYPE_ENUM_UINT8             enCurReqPdnType     = IMSA_IP_TYPE_BUTT;

    /* ��ʼ�� */
    pstPdpActivateRej  = (TAF_PS_CALL_PDP_ACTIVATE_REJ_STRU*)pEvtInfo;

    IMSA_INFO_LOG1("IMSA_CONN_ProcTafPsEvtPdpActivateRej is entered! opid:", pstPdpActivateRej->stCtrl.ucOpId);

    /* ����OPID���������������ʵ������ */
    ulResult = IMSA_CONN_GetConnTypeByOpid( pstPdpActivateRej->stCtrl.ucOpId,
                                            &enConnType);

    /* �������ʧ�ܣ���ֱ���˳� */
    if (IMSA_FAIL == ulResult)
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtPdpActivateRej:Get Conn Type Failed!");
        return IMSA_FAIL;
    }

    /* ����CONNING״̬����ֱ���˳� */
    if (IMSA_FALSE == IMSA_CONN_IsConnStatusEqual(enConnType, IMSA_CONN_STATUS_CONNING))
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtPdpActivateRej:NOT CONNING state!");
        return IMSA_FAIL;
    }

    /* ֹͣIMS���Ŷ�ʱ�� */
    IMSA_CONN_StopTimer(enConnType, TI_IMSA_SIP_SIGAL_PDP_ORIG);

    IMSA_CONN_GetConnFirstReqPdnType(enConnType, enFirstReqPdnType);
    IMSA_CONN_GetConnCurReqPdnType(enConnType, enCurReqPdnType);


    IMSA_INFO_LOG1("enFirstReqPdnType:", enFirstReqPdnType);

    /* �����IPV4 ONLY��IPV6 ONLYֱ���ϱ�����ʧ��. */
    if (IMSA_IP_TYPE_IPV4V6 != enFirstReqPdnType)
    {
        if (IMSA_TRUE == IMSA_CONN_HasActiveSipSignalPdp(enConnType))
        {
            IMSA_CONN_SndConnSetupInd(  IMSA_CONN_RESULT_SUCCESS,
                                        enConnType,
                                        IMSA_CONN_SIP_PDP_TYPE_SIGNAL);
            return IMSA_SUCC;
        }
        IMSA_CONN_SetupConnFail(enConnType, IMSA_CONN_RESULT_FAIL_CN_REJ);

        return IMSA_SUCC;
    }

    /* IPV4V6����Ҫ�Ӿ���ʵ�ֱַ���IPv4��IPv6���͵�PDP���� */
    if (IMSA_IP_TYPE_IPV4V6 == enCurReqPdnType)
    {
        IMSA_CONN_ProcActRejCurReqIpv4v6(enConnType, pstPdpActivateRej);
    }
    else if (IMSA_IP_TYPE_IPV4 == enCurReqPdnType)
    {
        IMSA_CONN_ProcActRejCurReqIpv4(enConnType);
    }
    else
    {
        IMSA_CONN_ProcActRejCurReqIpv6(enConnType);
    }

    return IMSA_SUCC;
}
VOS_UINT32 IMSA_CONN_ProcTafPsEvtPdpActivateInd
(
    VOS_VOID                           *pEvtInfo
)
{
    TAF_PS_CALL_PDP_ACTIVATE_IND_STRU  *pstPdpActivateInd   = VOS_NULL_PTR;
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType          = IMSA_CONN_TYPE_BUTT;
    IMSA_NORMAL_CONN_STRU              *pstNormalConn       = VOS_NULL_PTR;
    IMSA_EMC_CONN_STRU                 *pstEmcConn          = VOS_NULL_PTR;
    IMSA_CONN_SIP_PDP_TYPE_ENUM_UINT32  enSipPdpType        = IMSA_CONN_SIP_PDP_TYPE_BUTT;
    IMSA_PDP_CNTXT_INFO_STRU           *pstPdpContext       = VOS_NULL_PTR;
    VOS_UINT32                          ulCurSipMediaPdpNum = IMSA_NULL;

    IMSA_INFO_LOG("IMSA_CONN_ProcTafPsEvtPdpActivateInd is entered!");

    /* ��ʼ�� */
    pstPdpActivateInd  = (TAF_PS_CALL_PDP_ACTIVATE_IND_STRU*)pEvtInfo;

    /* �������ר�г��أ���ֱ���˳� */
    if (IMSA_OP_TRUE != pstPdpActivateInd->bitOpLinkdRabId)
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtPdpActivateInd:not dedicated!");
        return IMSA_FAIL;
    }

    /* ��ȡ���������� */
    if (IMSA_SUCC       != IMSA_CONN_GetPdpContextByPdpId(  IMSA_CONN_TYPE_NORMAL,
                                                            pstPdpActivateInd->ucLinkdRabId,
                                                            &enSipPdpType,
                                                            &pstPdpContext))
    {
        if (IMSA_SUCC   != IMSA_CONN_GetPdpContextByPdpId(  IMSA_CONN_TYPE_EMC,
                                                            pstPdpActivateInd->ucLinkdRabId,
                                                            &enSipPdpType,
                                                            &pstPdpContext))
        {
            IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtPdpActivateInd:Get LinkRabId context failed!");
            return IMSA_FAIL;
        }
        else
        {
            enConnType          = IMSA_CONN_TYPE_EMC;
            pstEmcConn          = IMSA_CONN_GetEmcConnAddr();
            ulCurSipMediaPdpNum = pstEmcConn->ulSipMediaPdpNum;
        }
    }
    else
    {
        enConnType          = IMSA_CONN_TYPE_NORMAL;
        pstNormalConn       = IMSA_CONN_GetNormalConnAddr();
        ulCurSipMediaPdpNum = pstNormalConn->ulSipMediaPdpNum;
    }

    if (IMSA_CONN_SIP_PDP_TYPE_SIGNAL != enSipPdpType)
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtPdpActivateInd:linked pdp is not signal pdp!");
        return IMSA_FAIL;
    }

    /* ����CONN״̬����ֱ���˳� */
    if (IMSA_FALSE == IMSA_CONN_IsConnStatusEqual(  enConnType,
                                                    IMSA_CONN_STATUS_CONN))
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtPdpActivateInd:Not CONN state!");
        return IMSA_FAIL;
    }

    if (IMSA_CONN_MAX_NORMAL_SIP_MEDIA_PDP_NUM <= ulCurSipMediaPdpNum)
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtPdpActivateInd:media pdp num is already 2!");
        return IMSA_FAIL;
    }

    /* �洢SIPý�������Ϣ */
    IMSA_CONN_SaveSipMediaPdpInfo(enConnType, pstPdpActivateInd);

    /* ֪ͨSERVICEģ��ý����ؽ����ɹ� */
    IMSA_CONN_SndConnSetupInd(  IMSA_CONN_RESULT_SUCCESS,
                                enConnType,
                                IMSA_CONN_SIP_PDP_TYPE_MEDIA);

    /* ��CDS�������й��˳��� */
    IMSA_CONN_SndCdsSetImsBearerReq();

    return IMSA_SUCC;
}
VOS_UINT32 IMSA_CONN_ProcTafPsEvtPdpModifyInd
(
    VOS_VOID                           *pEvtInfo
)
{
    TAF_PS_CALL_PDP_MODIFY_IND_STRU    *pstPdpModifyInd     = VOS_NULL_PTR;
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType          = IMSA_CONN_TYPE_BUTT;
    IMSA_CONN_SIP_PDP_TYPE_ENUM_UINT32  enSipPdpType        = IMSA_CONN_SIP_PDP_TYPE_BUTT;
    IMSA_PDP_CNTXT_INFO_STRU           *pstPdpContext       = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CONN_ProcTafPsEvtPdpModifyInd is entered!");

    /* ��ʼ�� */
    pstPdpModifyInd     = (TAF_PS_CALL_PDP_MODIFY_IND_STRU*)pEvtInfo;

    /* ��ȡ���������� */
    if (IMSA_SUCC       != IMSA_CONN_GetPdpContextByPdpId(  IMSA_CONN_TYPE_NORMAL,
                                                            pstPdpModifyInd->ucRabId,
                                                            &enSipPdpType,
                                                            &pstPdpContext))
    {
        if (IMSA_SUCC   != IMSA_CONN_GetPdpContextByPdpId(  IMSA_CONN_TYPE_EMC,
                                                            pstPdpModifyInd->ucRabId,
                                                            &enSipPdpType,
                                                            &pstPdpContext))
        {
            IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtPdpModifyInd:Get pdp context failed!");
            return IMSA_FAIL;
        }
        else
        {
            enConnType  = IMSA_CONN_TYPE_EMC;
        }
    }
    else
    {
        enConnType      = IMSA_CONN_TYPE_NORMAL;
    }

    IMSA_INFO_LOG1("IMSA_CONN_ProcTafPsEvtPdpModifyInd: Conn type :", enConnType);

    /* �����IDLE״̬����ֱ���˳� */
    if (IMSA_TRUE       == IMSA_CONN_IsConnStatusEqual(enConnType, IMSA_CONN_STATUS_IDLE))
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtPdpModifyInd:IDLE state!");
        return IMSA_FAIL;
    }

    /* �޸�SIP������Ϣ */
    IMSA_CONN_ModifySipPdpInfo(enSipPdpType, pstPdpContext, pstPdpModifyInd);

    /* �����ý����أ���֪ͨSERVICEý������޸� */
    if (IMSA_CONN_SIP_PDP_TYPE_MEDIA == enSipPdpType)
    {
        IMSA_CONN_SndConnMediaPdpModifyInd(enConnType);
    }

    return IMSA_SUCC;
}
VOS_UINT32 IMSA_CONN_ProcTafPsEvtPdpDeactivateCnf
(
    VOS_VOID                           *pEvtInfo
)
{
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU    *pstPdpDeactivateCnf = VOS_NULL_PTR;
    VOS_UINT32                              ulResult            = IMSA_FAIL;
    IMSA_CONN_TYPE_ENUM_UINT32              enConnType          = IMSA_CONN_TYPE_BUTT;
    IMSA_CONN_SIP_PDP_TYPE_ENUM_UINT32      enSipPdpType        = IMSA_CONN_SIP_PDP_TYPE_BUTT;
    IMSA_PDP_CNTXT_INFO_STRU               *pstPdpContext       = VOS_NULL_PTR;
    VOS_UINT32                              ulRegParaInvalidFlag= IMSA_NULL;

    pstPdpDeactivateCnf  = (TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU*)pEvtInfo;

    IMSA_INFO_LOG1("IMSA_CONN_ProcTafPsEvtPdpDeactivateCnf is entered! opid:", pstPdpDeactivateCnf->stCtrl.ucOpId);

    /* ����OPID���������������ʵ������ */
    ulResult = IMSA_CONN_GetConnTypeByOpid( pstPdpDeactivateCnf->stCtrl.ucOpId,
                                            &enConnType);

    /* �������ʧ�ܣ���ֱ���˳� */
    if (IMSA_FAIL == ulResult)
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtPdpDeactivateCnf:Get Opid Failed!");
        return IMSA_FAIL;
    }

    /* ����RELEASING״̬����ֱ���˳� */
    if (IMSA_FALSE == IMSA_CONN_IsConnStatusEqual(enConnType, IMSA_CONN_STATUS_RELEASING))
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtPdpDeactivateCnf:Not releasing state!");
        return IMSA_FAIL;
    }

    /* CID�����ڣ���ֱ���˳� */
    if (IMSA_SUCC != IMSA_CONN_GetPdpContextByCid(  enConnType,
                                                    pstPdpDeactivateCnf->ucCid,
                                                    &enSipPdpType,
                                                    &pstPdpContext))
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtPdpDeactivateCnf:cid not exist!");
        return IMSA_SUCC;
    }

    /* �������������أ���ֱ���˳� */
    if (IMSA_CONN_SIP_PDP_TYPE_SIGNAL != enSipPdpType)
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtPdpDeactivateCnf:Not signal pdp!");
        return IMSA_FAIL;
    }

    /* ֹͣIMS���Ŷ�ʱ�� */
    IMSA_CONN_StopTimer(enConnType, TI_IMSA_SIP_SIGAL_PDP_END);

    /* ɾ�����������Ϣ */
    IMSA_CONN_DeletePdpInfo(enConnType, pstPdpDeactivateCnf->ucCid, &ulRegParaInvalidFlag);

    if (IMSA_TRUE == IMSA_CONN_HasActiveSipSignalPdp(enConnType))
    {
        /* ����APS�ͷ����� */
        IMSA_CONN_RequestApsRelConn(enConnType);
    }
    else
    {
        IMSA_CONN_ClearConnResource(enConnType);

        /* ֪ͨSERVICEģ�������ͷ� */
        IMSA_CONN_SndConnRelInd(enConnType, IMSA_CONN_SIP_PDP_TYPE_SIGNAL);
    }

    return IMSA_SUCC;
}
VOS_UINT32 IMSA_CONN_ProcTafPsEvtPdpDeactivateInd
(
    VOS_VOID                           *pEvtInfo
)
{
    TAF_PS_CALL_PDP_DEACTIVATE_IND_STRU    *pstPdpDeactivateInd = VOS_NULL_PTR;
    IMSA_CONN_TYPE_ENUM_UINT32              enConnType          = IMSA_CONN_TYPE_BUTT;
    IMSA_CONN_SIP_PDP_TYPE_ENUM_UINT32      enSipPdpType        = IMSA_CONN_SIP_PDP_TYPE_BUTT;
    IMSA_PDP_CNTXT_INFO_STRU               *pstPdpContext       = VOS_NULL_PTR;
    VOS_UINT32                              ulRegParaValidFlag  = IMSA_NULL;

    IMSA_INFO_LOG("IMSA_CONN_ProcTafPsEvtPdpDeactivateInd is entered!");

    /* ��ʼ�� */
    pstPdpDeactivateInd = (TAF_PS_CALL_PDP_DEACTIVATE_IND_STRU*)pEvtInfo;

    /* ��ȡ���������� */
    if (IMSA_SUCC       != IMSA_CONN_GetPdpContextByCid(    IMSA_CONN_TYPE_NORMAL,
                                                            pstPdpDeactivateInd->ucCid,
                                                            &enSipPdpType,
                                                            &pstPdpContext))
    {
        if (IMSA_SUCC   != IMSA_CONN_GetPdpContextByCid(    IMSA_CONN_TYPE_EMC,
                                                            pstPdpDeactivateInd->ucCid,
                                                            &enSipPdpType,
                                                            &pstPdpContext))
        {
            IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtPdpDeactivateInd:Get pdp context failed!");
            return IMSA_FAIL;
        }
        else
        {
            enConnType  = IMSA_CONN_TYPE_EMC;
        }
    }
    else
    {
        enConnType      = IMSA_CONN_TYPE_NORMAL;
    }
     /* ɾ��������Ϣǰ�������ͷų��ص����ͺ�CID */
    IMSA_RegSaveRegedPara((IMSA_REG_TYPE_ENUM_UINT8)enConnType,
                            pstPdpDeactivateInd->ucCid,
                            pstPdpDeactivateInd->enPdpType);

    /* ɾ��������Ϣ */
    IMSA_CONN_DeletePdpInfo(enConnType, pstPdpDeactivateInd->ucCid, &ulRegParaValidFlag);

    if (IMSA_CONN_TYPE_EMC == enConnType)
    {
        if (IMSA_CONN_SIP_PDP_TYPE_MEDIA == enSipPdpType)
        {
            IMSA_INFO_LOG("IMSA_CONN_ProcTafPsEvtPdpDeactivateInd:EMC media");

            IMSA_CONN_SndConnRelInd(enConnType, IMSA_CONN_SIP_PDP_TYPE_MEDIA);

            return IMSA_SUCC;
        }

        IMSA_INFO_LOG("IMSA_CONN_ProcTafPsEvtPdpDeactivateInd:EMC sig");

        /* ֹͣIMS���Ŷ�ʱ�� */
        IMSA_CONN_StopTimer(enConnType, TI_IMSA_SIP_SIGAL_PDP_END);

        /* ���������Դ */
        /* IMSA_CONN_ClearConnResource(enConnType); */
        IMSA_CONN_SetConnStatus(IMSA_CONN_TYPE_EMC, IMSA_CONN_STATUS_IDLE);

        /* ֪ͨSERVICEģ�������ͷ� */
        IMSA_CONN_SndConnRelInd(enConnType, IMSA_CONN_SIP_PDP_TYPE_SIGNAL);

        return IMSA_SUCC;
    }

    if (IMSA_CONN_SIP_PDP_TYPE_MEDIA == enSipPdpType)
    {
        IMSA_INFO_LOG("IMSA_CONN_ProcTafPsEvtPdpDeactivateInd:MEDIA");

        IMSA_CONN_SndConnRelInd(enConnType, IMSA_CONN_SIP_PDP_TYPE_MEDIA);

        return IMSA_SUCC;
    }

    if (IMSA_CONN_STATUS_CONNING == IMSA_CONN_GetNormalConnStatus())
    {
        IMSA_INFO_LOG("IMSA_CONN_ProcTafPsEvtPdpDeactivateInd:normal,conning");

        return IMSA_SUCC;
    }

    if (IMSA_CONN_STATUS_RELEASING == IMSA_CONN_GetNormalConnStatus())
    {
        IMSA_INFO_LOG("IMSA_CONN_ProcTafPsEvtPdpDeactivateInd:normal,releasing");

        if (IMSA_TRUE != IMSA_CONN_HasActiveSipSignalPdp(enConnType))
        {
            /* ֹͣIMS���Ŷ�ʱ�� */
            IMSA_CONN_StopTimer(enConnType, TI_IMSA_SIP_SIGAL_PDP_END);

            /* ���������Դ */
            IMSA_CONN_ClearConnResource(enConnType);

            /* ֪ͨSERVICEģ�������ͷ� */
            IMSA_CONN_SndConnRelInd(enConnType, IMSA_CONN_SIP_PDP_TYPE_SIGNAL);
        }

        return IMSA_SUCC;
    }

    if (IMSA_TRUE != IMSA_CONN_HasActiveSipSignalPdp(enConnType))
    {
        IMSA_INFO_LOG("IMSA_CONN_ProcTafPsEvtPdpDeactivateInd:normal,conn,no active pdp!");

        /* ���������Դ */
        /* IMSA_CONN_ClearConnResource(enConnType); */
        IMSA_CONN_SetConnStatus(IMSA_CONN_TYPE_NORMAL, IMSA_CONN_STATUS_IDLE);

        /* ֪ͨSERVICEģ�������ͷ� */
        IMSA_CONN_SndConnRelInd(enConnType, IMSA_CONN_SIP_PDP_TYPE_SIGNAL);

        return IMSA_SUCC;
    }

    /* ���ע�����ʧЧ���һ�������������������ʱ��֪ͨSERVICEģ��ע�����ʧЧ */
    if (IMSA_CONN_REG_PARA_INVALID == ulRegParaValidFlag)
    {
        IMSA_INFO_LOG("IMSA_CONN_ProcTafPsEvtPdpDeactivateInd:normal,conn,active pdp,reg para invalid!");

        IMSA_CONN_SndConnRegParaInvalid();

        return IMSA_SUCC;
    }

    return IMSA_SUCC;
}

/* lihong00150010 ims begin 2013-01-08 ��ֹ�����ز�·�ɹ��� */

VOS_UINT32 IMSA_CONN_IsIpv6PrefixChanged
(
    const IMSA_PDP_CNTXT_INFO_STRU             *pstPdpInfo,
    const TAF_PS_IPV6_INFO_IND_STRU            *pstIpv6InfoInd
)
{
    if (0 == IMSA_MEM_CMP(  pstPdpInfo->stPdpAddr.aucIpV6Addr,
                            pstIpv6InfoInd->stIpv6RaInfo.astPrefixList[0].aucPrefix,
                            IMSA_IPV6_PREFIX_LEN))
    {
        return IMSA_FALSE;
    }

    return IMSA_TRUE;
}
/* lihong00150010 ims end 2013-01-08 ��ֹ�����ز�·�ɹ��� */

VOS_UINT32 IMSA_CONN_ProcTafPsEvtPdpIpv6InfoInd
(
    VOS_VOID                           *pEvtInfo
)
{
    TAF_PS_IPV6_INFO_IND_STRU          *pstPsIpv6InfoInd    = VOS_NULL_PTR;
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType          = IMSA_CONN_TYPE_BUTT;
    IMSA_CONN_SIP_PDP_TYPE_ENUM_UINT32  enSipPdpType        = IMSA_CONN_SIP_PDP_TYPE_BUTT;
    IMSA_PDP_CNTXT_INFO_STRU           *pstPdpContext       = VOS_NULL_PTR;
    VOS_CHAR                            acUeAddr[IMSA_IPV6_ADDR_STRING_LEN + 1] = {0};

    IMSA_INFO_LOG("IMSA_CONN_ProcTafPsEvtPdpIpv6InfoInd is entered!");

    /* ��ʼ�� */
    pstPsIpv6InfoInd     = (TAF_PS_IPV6_INFO_IND_STRU*)pEvtInfo;

    /* ���û��Я��ǰ׺��Ϣ����ֱ���˳� */
    if (0 == pstPsIpv6InfoInd->stIpv6RaInfo.ulPrefixNum)
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtPdpIpv6InfoInd:No prefix!");
        return IMSA_FAIL;
    }

    /* ��ȡ���������� */
    if (IMSA_SUCC       != IMSA_CONN_GetPdpContextByPdpId(  IMSA_CONN_TYPE_NORMAL,
                                                            pstPsIpv6InfoInd->ucRabId,
                                                            &enSipPdpType,
                                                            &pstPdpContext))
    {
        if (IMSA_SUCC   != IMSA_CONN_GetPdpContextByPdpId(  IMSA_CONN_TYPE_EMC,
                                                            pstPsIpv6InfoInd->ucRabId,
                                                            &enSipPdpType,
                                                            &pstPdpContext))
        {
            IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtPdpIpv6InfoInd:Get pdp context failed!");
            return IMSA_FAIL;
        }
        else
        {
            enConnType  = IMSA_CONN_TYPE_EMC;
        }
    }
    else
    {
        enConnType      = IMSA_CONN_TYPE_NORMAL;
    }

    if (IMSA_CONN_SIP_PDP_TYPE_MEDIA == enSipPdpType)
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtPdpIpv6InfoInd:not signal pdp type!");
        return IMSA_FAIL;
    }

    /* �����CONN״̬�������PDP������ */
    if (IMSA_TRUE == IMSA_CONN_IsConnStatusEqual(enConnType, IMSA_CONN_STATUS_CONN))
    {
        IMSA_INFO_LOG("IMSA_CONN_ProcTafPsEvtPdpModifyInd:CONN state!");

        /* �洢IPV6��Ϣ */
        /* lihong00150010 ims begin 2013-01-08 ��ֹ�����ز�·�ɹ��� */
        if (IMSA_TRUE == IMSA_CONN_IsIpv6PrefixChanged(pstPdpContext, pstPsIpv6InfoInd))
        {
            IMSA_CONN_SaveIpv6Info(pstPdpContext, pstPsIpv6InfoInd);

            /* ��REGģ������IPV6��ַ */
            IMSA_CONN_ConvertIpAddress2String(  IMSA_IP_TYPE_IPV6,
                                                pstPdpContext->stPdpAddr.aucIpV6Addr,
                                                acUeAddr);

            (VOS_VOID)IMSA_RegAddrPairMgrAddUeAddr( (IMSA_REG_TYPE_ENUM_UINT8)enConnType,
                                                    IMSA_IP_TYPE_IPV6,
                                                    acUeAddr);

            /* ����������IPv6��ַ��IPv6 DNS��ַ */
            /* IMSA_CONN_ConfigPdpIPv6Info2Bsp(pstPdpContext); */
        }
        /* lihong00150010 ims end 2013-01-08 ��ֹ�����ز�·�ɹ��� */

        return IMSA_SUCC;
    }

    /* �����CONNING״̬�������PDP�����ģ�֪ͨSERVICE���ӽ����ɹ� */
    if (IMSA_TRUE == IMSA_CONN_IsConnStatusEqual(enConnType, IMSA_CONN_STATUS_CONNING))
    {
        IMSA_INFO_LOG("IMSA_CONN_ProcTafPsEvtPdpModifyInd:CONNING state!");

        /* �洢IPV6��Ϣ */
        /* lihong00150010 ims begin 2013-01-08 ��ֹ�����ز�·�ɹ��� */
        if (IMSA_TRUE == IMSA_CONN_IsIpv6PrefixChanged(pstPdpContext, pstPsIpv6InfoInd))
        {
            IMSA_CONN_SaveIpv6Info(pstPdpContext, pstPsIpv6InfoInd);

            /* ��REGģ������IPV6��ַ */
            IMSA_CONN_ConvertIpAddress2String(  IMSA_IP_TYPE_IPV6,
                                                pstPdpContext->stPdpAddr.aucIpV6Addr,
                                                acUeAddr);

            (VOS_VOID)IMSA_RegAddrPairMgrAddUeAddr( (IMSA_REG_TYPE_ENUM_UINT8)enConnType,
                                                    IMSA_IP_TYPE_IPV6,
                                                    acUeAddr);

            /* ����������IPv6��ַ��IPv6 DNS��ַ */
            /* IMSA_CONN_ConfigPdpIPv6Info2Bsp(pstPdpContext); */
        }
        /* lihong00150010 ims end 2013-01-08 ��ֹ�����ز�·�ɹ��� */

        /* �����ǰ��������IPV4��SIP������ؽ�������ȴ�IPV4������ؽ�����ɺ�
           ��֪ͨSERVICEģ�����ӽ����ɹ� */
        if (IMSA_TRUE == IMSA_CONN_IsEqualToSelectedCid(enConnType, pstPsIpv6InfoInd->ucCid))
        {
            /* ֪ͨSERIVCE���ӽ����ɹ� */
            IMSA_CONN_SetupConnSucc(enConnType);
        }

        /* ֹͣ�ȴ�IPV6������ʱ�� */
        IMSA_CONN_StopTimer(enConnType, TI_IMSA_WAIT_IPV6_INFO);

        return IMSA_SUCC;
    }

    return IMSA_FAIL;
}


VOS_UINT32 IMSA_CONN_ProcTafPsEvtCallOrigCnf
(
    VOS_VOID                           *pEvtInfo
)
{
    TAF_PS_CALL_ORIG_CNF_STRU          *pstCallOrigCnf  = VOS_NULL_PTR;
    VOS_UINT32                          ulResult        = IMSA_FAIL;
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType      = IMSA_CONN_TYPE_BUTT;

    pstCallOrigCnf  = (TAF_PS_CALL_ORIG_CNF_STRU*)pEvtInfo;

    IMSA_INFO_LOG1("IMSA_CONN_ProcTafPsEvtCallOrigCnf is entered! opid:", pstCallOrigCnf->stCtrl.ucOpId);

    /* ����OPID���������������ʵ������ */
    ulResult = IMSA_CONN_GetConnTypeByOpid( pstCallOrigCnf->stCtrl.ucOpId,
                                            &enConnType);

    /* �������ʧ�ܣ���ֱ���˳� */
    if (IMSA_FAIL == ulResult)
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtCallOrigCnf:Get Opid Failed!");
        return IMSA_FAIL;
    }

    /* ����CONNING״̬����ֱ���˳� */
    if (IMSA_FALSE == IMSA_CONN_IsConnStatusEqual(enConnType, IMSA_CONN_STATUS_CONNING))
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtCallOrigCnf:Not CONNING state!");
        return IMSA_FAIL;
    }

    /* APS���������ȷ��ֱ���˳� */
    if (TAF_PS_CAUSE_SUCCESS == pstCallOrigCnf->enCause)
    {
        return IMSA_SUCC;
    }

    /* ֹͣIMS���Ŷ�ʱ�� */
    IMSA_CONN_StopTimer(enConnType, TI_IMSA_SIP_SIGAL_PDP_ORIG);

    if (IMSA_TRUE == IMSA_CONN_HasActiveSipSignalPdp(enConnType))
    {
        /* ���IPV6ȫ�ֵ�ַ�ѻ�ã��ظ����ӽ����ɹ������������ȴ�IPV6������ʱ�� */
        IMSA_CONN_WaitForIpv6InfoProc(enConnType);

        return IMSA_SUCC;
    }

    /* ��SERVICE�ظ�IMSA_CONN_SETUP_IND��Ϣ�������TAF_PS_CAUSE_PDP_ACTIVATE_LIMIT��
           ����ֵ��ΪFAIL_PDP_ACTIVATE_LIMIT��������ΪFAIL_PARA_ERR */
    if (TAF_PS_CAUSE_PDP_ACTIVATE_LIMIT == pstCallOrigCnf->enCause)
    {
        IMSA_CONN_SetupConnFail(enConnType, IMSA_CONN_RESULT_FAIL_PDP_ACTIVATE_LIMIT);
    }
    else if (TAF_PS_CAUSE_OPERATION_CONFLICT == pstCallOrigCnf->enCause)
    {
        IMSA_CONN_SetupConnFail(enConnType, IMSA_CONN_RESULT_FAIL_SAME_APN_OPERATING);
    }
    else
    {
        IMSA_CONN_SetupConnFail(enConnType, IMSA_CONN_RESULT_FAIL_PARA_ERR);
    }

    return IMSA_SUCC;
}



VOS_UINT32 IMSA_CONN_ProcTafPsEvtCallEndCnf
(
    VOS_VOID                           *pEvtInfo
)
{
    TAF_PS_CALL_END_CNF_STRU           *pstCallEndCnf           = VOS_NULL_PTR;
    VOS_UINT32                          ulResult                = IMSA_FAIL;
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType              = IMSA_CONN_TYPE_BUTT;
    IMSA_CONN_SIP_PDP_TYPE_ENUM_UINT32  enSipPdpType            = IMSA_CONN_SIP_PDP_TYPE_BUTT;
    IMSA_PDP_CNTXT_INFO_STRU           *pstPdpContext           = VOS_NULL_PTR;
    VOS_UINT32                          ulRegParaInvalidFlag    = IMSA_NULL;

    pstCallEndCnf  = (TAF_PS_CALL_END_CNF_STRU*)pEvtInfo;

    IMSA_INFO_LOG1("IMSA_CONN_ProcTafPsEvtCallEndCnf is entered! opid:",pstCallEndCnf->stCtrl.ucOpId);

    /* ����OPID���������������ʵ������ */
    ulResult = IMSA_CONN_GetConnTypeByOpid( pstCallEndCnf->stCtrl.ucOpId,
                                            &enConnType);

    /* �������ʧ�ܣ���ֱ���˳� */
    if (IMSA_FAIL == ulResult)
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtCallEndCnf:Get Opid Failed!");
        return IMSA_FAIL;
    }

    /* ����RELEASING״̬����ֱ���˳� */
    if (IMSA_FALSE == IMSA_CONN_IsConnStatusEqual(enConnType, IMSA_CONN_STATUS_RELEASING))
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtCallEndCnf:Not releasing state!");
        return IMSA_FAIL;
    }

    /* APS���������ȷ��ֱ���˳� */
    if (TAF_PS_CAUSE_SUCCESS == pstCallEndCnf->enCause)
    {
        return IMSA_SUCC;
    }

    /* CID�����ڣ���ֱ���˳� */
    if (IMSA_SUCC != IMSA_CONN_GetPdpContextByCid(  enConnType,
                                                    pstCallEndCnf->ucCid,
                                                    &enSipPdpType,
                                                    &pstPdpContext))
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTafPsEvtCallEndCnf:cid not exist!");
        return IMSA_SUCC;
    }

    /* ֹͣIMS���Ŷ�ʱ�� */
    IMSA_CONN_StopTimer(enConnType, TI_IMSA_SIP_SIGAL_PDP_END);

    /* ɾ�����������Ϣ */
    IMSA_CONN_DeletePdpInfo(enConnType, pstCallEndCnf->ucCid, &ulRegParaInvalidFlag);

    if (IMSA_TRUE == IMSA_CONN_HasActiveSipSignalPdp(enConnType))
    {
        /* ����APS�ͷ����� */
        IMSA_CONN_RequestApsRelConn(enConnType);
    }

    return IMSA_SUCC;
}

/*****************************************************************************
 Function Name  : IMSA_CONN_ProcTafPsEvtSrvccCancelInd
 Description    : IMSA CONNģ�鴦��SRVCC CANCEL�¼�
 Input          : pEvtInfo--------------�¼���Ϣָ��
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-10-14  Draft Enact
*****************************************************************************/
VOS_UINT32 IMSA_CONN_ProcTafPsEvtSrvccCancelInd
(
    VOS_VOID                           *pEvtInfo
)
{
    IMSA_INFO_LOG("IMSA_CONN_ProcTafPsEvtSrvccCancelInd is entered!");

    (VOS_VOID)pEvtInfo;

    /*����ȴ���ϵͳָʾ��ʾ*/
    IMSA_CallSetSrvccFlag(IMSA_FALSE);

    /* ��������ϱ�ALL RELEASED�¼���ʶ */
    IMSA_CallSetNotReportAllReleasedFlag(IMSA_FALSE);

    /*�յ�����Ϣʱ���ն˵���ϵͳ���̻�û������������֪ͨIMS��AT�ϱ�������Ҫ�����⴦��*/

    /*�յ�ESM/SM Notification ind�¼�����Ҫ֪ͨIMSЭ��ջ����re-invite����*/
    (VOS_VOID)IMSA_CallSendImsMsgSrvcc(IMSA_IMS_INPUT_CALL_REASON_SRVCC_CANCELED);

    /*�ϱ�HOʧ��*/
    IMSA_SndMsgAtCirephInd(AT_IMSA_SRVCC_HANDOVER_FAILURE);

    return IMSA_SUCC;
}
VOS_VOID IMSA_CONN_ProcTimerMsgSipSignalPdpOrigExp
(
    const VOS_VOID                     *pMsg
)
{
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType = IMSA_CONN_TYPE_BUTT;

    IMSA_INFO_LOG("IMSA_CONN_ProcTimerMsgSipSignalPdpOrigExp is entered!");

    enConnType = PS_GET_REL_TIMER_PARA(pMsg);

    if (enConnType >= IMSA_CONN_TYPE_BUTT)
    {
        IMSA_ERR_LOG("IMSA_CONN_ProcTimerMsgSipSignalPdpOrigExp:Illegal Para!");
        return ;
    }

    /* ����CONNING״̬����ֱ���˳� */
    if (IMSA_FALSE == IMSA_CONN_IsConnStatusEqual(enConnType, IMSA_CONN_STATUS_CONNING))
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTimerMsgSipSignalPdpEndExp:Not CONNING state!");
        return ;
    }

    if (IMSA_CONN_TYPE_EMC == enConnType)
    {
        IMSA_INFO_LOG("IMSA_CONN_ProcTimerMsgSipSignalPdpOrigExp:EMC,conning!");

        IMSA_CONN_SetupConnFail(enConnType, IMSA_CONN_RESULT_FAIL_TIMER_EXP);
        return ;
    }

    IMSA_INFO_LOG("IMSA_CONN_ProcTimerMsgSipSignalPdpOrigExp:normal,conning!");

    if (IMSA_TRUE == IMSA_CONN_HasActiveSipSignalPdp(enConnType))
    {
        /* ���IPV6ȫ�ֵ�ַ�ѻ�ã��ظ����ӽ����ɹ������������ȴ�IPV6������ʱ�� */
        IMSA_CONN_WaitForIpv6InfoProc(enConnType);
    }
    else
    {
        IMSA_CONN_SetupConnFail(enConnType, IMSA_CONN_RESULT_FAIL_TIMER_EXP);
    }

    return ;
}


VOS_VOID IMSA_CONN_ProcTimerMsgSipSignalPdpEndExp
(
    const VOS_VOID                     *pMsg
)
{
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType              = IMSA_CONN_TYPE_BUTT;
    VOS_UINT32                          ulRegParaInvalidFlag    = IMSA_NULL;
    IMSA_NORMAL_CONN_STRU              *pstNormalConn           = VOS_NULL_PTR;
    IMSA_EMC_CONN_STRU                 *pstEmcConn              = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CONN_ProcTimerMsgSipSignalPdpEndExp is entered!");

    enConnType = PS_GET_REL_TIMER_PARA(pMsg);

    if (enConnType >= IMSA_CONN_TYPE_BUTT)
    {
        IMSA_ERR_LOG("IMSA_CONN_ProcTimerMsgSipSignalPdpEndExp:Illegal Para!");

        return ;
    }

    /* ����RELEASING״̬����ֱ���˳� */
    if (IMSA_FALSE == IMSA_CONN_IsConnStatusEqual(enConnType, IMSA_CONN_STATUS_RELEASING))
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTimerMsgSipSignalPdpEndExp:Not releasing state!");
        return ;
    }

    if (IMSA_CONN_TYPE_NORMAL == enConnType)
    {
        pstNormalConn   = IMSA_CONN_GetNormalConnAddr();

        /* ɾ�����������Ϣ */
        IMSA_CONN_DeletePdpInfo(    enConnType,
                                    pstNormalConn->astSipSignalPdpArray[0].ucCid,
                                    &ulRegParaInvalidFlag);
    }
    else
    {
        pstEmcConn      = IMSA_CONN_GetEmcConnAddr();

        /* ɾ�����������Ϣ */
        IMSA_CONN_DeletePdpInfo(    enConnType,
                                    pstEmcConn->stSipSignalPdp.ucCid,
                                    &ulRegParaInvalidFlag);
    }

    if (IMSA_TRUE == IMSA_CONN_HasActiveSipSignalPdp(enConnType))
    {
        /* ����APS�ͷ����� */
        IMSA_CONN_RequestApsRelConn(enConnType);
    }
    else
    {
        IMSA_CONN_ClearConnResource(enConnType);

        /* ֪ͨSERVICEģ�������ͷ� */
        IMSA_CONN_SndConnRelInd(enConnType, IMSA_CONN_SIP_PDP_TYPE_SIGNAL);
    }

    return ;
}
VOS_VOID IMSA_CONN_ProcTimerMsgWaitIpv6InfoExp
(
    const VOS_VOID                     *pMsg
)
{
    IMSA_CONN_TYPE_ENUM_UINT32          enConnType              = IMSA_CONN_TYPE_BUTT;

    IMSA_INFO_LOG("IMSA_CONN_ProcTimerMsgWaitIpv6InfoExp is entered!");

    enConnType = PS_GET_REL_TIMER_PARA(pMsg);

    if (enConnType >= IMSA_CONN_TYPE_BUTT)
    {
        IMSA_ERR_LOG("IMSA_CONN_ProcTimerMsgWaitIpv6InfoExp:Illegal Para!");

        return ;
    }

    /* ����CONNING״̬����ֱ���˳� */
    if (IMSA_FALSE == IMSA_CONN_IsConnStatusEqual(enConnType, IMSA_CONN_STATUS_CONNING))
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTimerMsgSipSignalPdpEndExp:Not CONNING state!");
        return ;
    }

    if ((IMSA_PDP_STATE_ACTIVE == IMSA_CONN_GetSipSignalPdpState(enConnType, IMSA_IP_TYPE_IPV4))
        || (IMSA_PDP_STATE_ACTIVE == IMSA_CONN_GetSipSignalPdpState(enConnType, IMSA_IP_TYPE_IPV4V6)))
    {
        IMSA_WARN_LOG("IMSA_CONN_ProcTimerMsgSipSignalPdpEndExp:IPV4 OR IPV4V6 PDP!");

        IMSA_CONN_SetupConnSucc(enConnType);
    }

    return ;
}
/*lint +e961*/
/*lint +e960*/
#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
/* end of ImsaProcApsMsg.c*/



