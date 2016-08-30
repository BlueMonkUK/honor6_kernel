


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "NasEmmTauSerInclude.h"
#include "EmmTcInterface.h"
/* lihong00150010 emergency tau&service begin */
#include "NasEmmAttDetInclude.h"
/* lihong00150010 emergency tau&service ends */

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASEMMSERVICESERREQ_C
/*lint +e767*/



/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* lihong00150010 emergency tau&service begin */

VOS_UINT32 NAS_EMM_MsRegSsLimitedSrvMsgRabmReestReq
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    static VOS_UINT32                   ulMsgCnt    = NAS_EMM_NULL;
    EMM_ERABM_REEST_REQ_STRU           *pstReestReq = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsRegSsLimitedSrvMsgRabmReestReq is entered.");

    pstReestReq = (EMM_ERABM_REEST_REQ_STRU *)pMsgStru;

    /* ������ǽ���ҵ������ */
    if (VOS_TRUE != pstReestReq->ulIsEmcType)
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_MsRegSsLimitedSrvMsgRabmReestReq:Not EMC!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*�����������̬����ӡ������Ϣ*/
    if (NAS_EMM_CONN_IDLE != NAS_EMM_GetConnState())
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_MsRegSsLimitedSrvMsgRabmReestReq: CONN.");
        ulMsgCnt++;

        /* ������,��ֹEMM��RABMά����RRC��·״̬��һ��,����UE��ʱ���޷�������*/
        /* ����2���յ�ERABM�Ľ�������ʱ,��RRC��·״̬����ΪIDLE̬*/
        if (NAS_EMM_DISCARD_ERABM_RESET_REQ_MAX_CNT > ulMsgCnt)
        {
            return NAS_LMM_MSG_HANDLED;
        }

        /*��������״̬*/
        NAS_EMM_MrrcChangeRrcStatusToIdle();
    }
    ulMsgCnt = NAS_EMM_NULL;

    NAS_EMM_SER_RcvRabmReestReq(pstReestReq->ulIsEmcType);

    return NAS_LMM_MSG_HANDLED;
}
/* lihong00150010 emergency tau&service end */

VOS_UINT32 NAS_EMM_MsRegSsNormalMsgRabmReestReq(VOS_UINT32  ulMsgId,
                                                   VOS_VOID   *pMsgStru
                               )
{
    VOS_UINT32                          ulRslt      = NAS_EMM_FAIL;
    static VOS_UINT32                   ulMsgCnt    = NAS_EMM_NULL;
    EMM_ERABM_REEST_REQ_STRU           *pstReestReq = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO( "Nas_Emm_MsRegSsNormalMsgRabmReestReq is entered.");

    /*��鵱ǰ״̬������ָ��*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_REG,EMM_SS_REG_NORMAL_SERVICE,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_TAUSER_CHKFSMStateMsgp ERROR !!");
         return NAS_LMM_MSG_HANDLED;
    }

    /*�����������̬����ӡ������Ϣ*/
    if (NAS_EMM_CONN_IDLE != NAS_EMM_GetConnState())
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_MsRegSsNormalMsgRabmReestReq: CONN.");
        ulMsgCnt++;

        /* ������,��ֹEMM��RABMά����RRC��·״̬��һ��,����UE��ʱ���޷�������*/
        /* ����2���յ�ERABM�Ľ�������ʱ,��RRC��·״̬����ΪIDLE̬*/
        if (NAS_EMM_DISCARD_ERABM_RESET_REQ_MAX_CNT > ulMsgCnt)
        {
            return NAS_LMM_MSG_HANDLED;
        }

        /*��������״̬*/
        NAS_EMM_MrrcChangeRrcStatusToIdle();
    }
    ulMsgCnt = NAS_EMM_NULL;

    /* ���CSFB��ʱ��ʱ�������У����ȿ���CSFB��PS����ҵ�����ȼ��ŵ� ��
       (��: ��ʹ���ڷ���SER��Ҳ����ǰ�����ͷŹ����ж�ֱ��ʧ��)��
       �����������˴�����RABM�Ľ�������*/
    if((NAS_LMM_TIMER_RUNNING == NAS_LMM_IsPtlTimerRunning(TI_NAS_EMM_PTL_CSFB_DELAY))
        &&(VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
        &&(NAS_EMM_CONN_RELEASING == NAS_EMM_GetConnState()))
    {
        NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsRegSsNormalMsgRabmReestReq: Msg discard, CSFB delay timer is running.");
        return NAS_LMM_MSG_HANDLED;
    }

	/* lihong00150010 emergency tau&service begin */
    pstReestReq = (EMM_ERABM_REEST_REQ_STRU *)pMsgStru;
    NAS_EMM_SER_RcvRabmReestReq(pstReestReq->ulIsEmcType);
	/* lihong00150010 emergency tau&service end */
    return NAS_LMM_MSG_HANDLED;

}


VOS_UINT32  NAS_EMM_MsRegSsRegAttemptUpdateMmMsgRabmReestReq
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    VOS_UINT32                          ulRslt      = NAS_EMM_FAIL;
    static VOS_UINT32                   ulMsgCnt    = NAS_EMM_NULL;
    EMM_ERABM_REEST_REQ_STRU           *pstReestReq = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsRegSsRegAttemptUpdateMmMsgRabmReestReq is entered.");

    /*��鵱ǰ״̬������ָ��*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_REG,
                                        EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM,
                                        pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_TAUSER_CHKFSMStateMsgp ERROR !!");
         return NAS_LMM_MSG_HANDLED;
    }

    /*�����������̬����ӡ������Ϣ*/
    if (NAS_EMM_CONN_IDLE != NAS_EMM_GetConnState())
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_MsRegSsRegAttemptUpdateMmMsgRabmReestReq: CONN.");
        ulMsgCnt++;

        /* ������,��ֹEMM��RABMά����RRC��·״̬��һ��,����UE��ʱ���޷�������*/
        /* ����2���յ�ERABM�Ľ�������ʱ,��RRC��·״̬����ΪIDLE̬*/
        if (NAS_EMM_DISCARD_ERABM_RESET_REQ_MAX_CNT > ulMsgCnt)
        {
            return NAS_LMM_MSG_HANDLED;
        }

        /*��������״̬*/
        NAS_EMM_MrrcChangeRrcStatusToIdle();
    }
    ulMsgCnt = NAS_EMM_NULL;

    /* ��¼UPDATE_MM��ʶ */
    /*NAS_LMM_SetEmmInfoUpdateMmFlag(NAS_EMM_UPDATE_MM_FLAG_VALID);*/
	/* lihong00150010 emergency tau&service begin */
    pstReestReq = (EMM_ERABM_REEST_REQ_STRU *)pMsgStru;
    NAS_EMM_SER_RcvRabmReestReq(pstReestReq->ulIsEmcType);
	/* lihong00150010 emergency tau&service end */

    return NAS_LMM_MSG_HANDLED;
}

/* lihong00150010 emergency tau&service begin */
/*****************************************************************************
 Function Name   : NAS_EMM_MsRegSsLimitedSrvMsgRabmRelReq
 Description     : Reg.Limit_Service״̬���յ�RABM�����쳣���ͷ�����
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2012-12-29  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_MsRegSsLimitedSrvMsgRabmRelReq
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    NAS_EMM_SER_LOG_INFO("NAS_EMM_MsRegSsLimitedSrvMsgRabmRelReq entered.");
    (VOS_VOID)ulMsgId;
    (VOS_VOID)pMsgStru;

    /*��MRRC����NAS_EMM_MRRC_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);

    return NAS_LMM_MSG_HANDLED;
}
/* lihong00150010 emergency tau&service end */

/*****************************************************************************
 Function Name   : NAS_EMM_MsRegSsNormalMsgRabmRelReq
 Description     : Reg.Normal_Service״̬���յ�RABM�����쳣���ͷ�����
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.sunbing      2010-12-29  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_MsRegSsNormalMsgRabmRelReq( VOS_UINT32  ulMsgId,
                                                   VOS_VOID   *pMsgStru )
{
    NAS_EMM_SER_LOG_INFO("NAS_EMM_MsRegSsNormalMsgRabmRelReq entered.");
    (VOS_VOID)ulMsgId;
    (VOS_VOID)pMsgStru;

    /*��MRRC����NAS_EMM_MRRC_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);

    return NAS_LMM_MSG_HANDLED;

}

/*****************************************************************************
 Function Name   : NAS_EMM_MsRegSsRegAttemptUpdateMmMsgRabmRelReq
 Description     : ������REG+ATTEMPT_TO_UPDATE_MM״̬���յ�RABM���ͷ���·����
 Input           : ulMsgId-------------------------��ϢID
                   pMsgStru------------------------��Ϣָ��
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2011-11-17  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_MsRegSsRegAttemptUpdateMmMsgRabmRelReq
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    NAS_EMM_SER_LOG_INFO("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgRabmRelReq entered.");

    (VOS_VOID)ulMsgId;
    (VOS_VOID)pMsgStru;

    /*��MRRC����NAS_EMM_MRRC_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);

    return NAS_LMM_MSG_HANDLED;
}

/*****************************************************************************
 Function Name   : NAS_EMM_MsRegInitSsWaitCnAttachCnfMsgTcDataReq
 Description     : ��ס״̬ΪEMM_RS_REG_INIT����״̬ΪEMM_SS_ATTACH_WAIT_CN_ATTACH_CNF
                   ���յ�ID_EMM_ETC_DATA_REQ��Ϣ
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2009-10-16  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_MsRegInitSsWaitCnAttachCnfMsgTcDataReq
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    VOS_UINT32                          ulRslt;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO("NAS_EMM_MsRegInitSsWaitCnAttachCnfMsgTcDataReq is entered.");

    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_REG_INIT,EMM_SS_ATTACH_WAIT_CN_ATTACH_CNF,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        /* ��ӡ�쳣 */
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_MsRegInitSsWaitCnAttachCnfMsgTcDataReq ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    NAS_EMM_SER_SendMrrcDataReq_Tcdata((EMM_ETC_DATA_REQ_STRU *)pMsgStru);

    return NAS_LMM_MSG_HANDLED;
}


/*****************************************************************************
 Function Name   : NAS_EMM_MsRegSsNormalMsgTcDataReq
 Description     : ��ס״̬ΪEMM_RS_REG����״̬ΪEMM_SS_NORMAL_SERVICE
                   ���յ�ID_EMM_ETC_DATA_REQ��Ϣ
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2009-10-16  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_MsRegSsNormalMsgTcDataReq
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    VOS_UINT32                      ulRslt              = NAS_EMM_FAIL;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO("NAS_EMM_MsRegSsNormalMsgTcDataReq is entered.");

    /* ��������ָ��������, ״̬ƥ����,����ƥ��,�˳�*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_REG,EMM_SS_REG_NORMAL_SERVICE,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_TAUSER_CHKFSMStateMsgp ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*CONNģʽ�£�ת��TC��Ϣ��IDLEģʽ�£���ӡ������Ϣ*/
    if((NAS_EMM_CONN_SIG == NAS_EMM_GetConnState()) ||
        (NAS_EMM_CONN_DATA == NAS_EMM_GetConnState()))
    {
        NAS_EMM_SER_SendMrrcDataReq_Tcdata((EMM_ETC_DATA_REQ_STRU *)pMsgStru);
    }
    else
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_MsRegSsNormalMsgTcDataReq:Warning:RRC connection is not Exist!");
    }

    return NAS_LMM_MSG_HANDLED;

}


VOS_UINT32  NAS_EMM_MsRegSsRegAttemptUpdateMmMsgTcDataReq
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    VOS_UINT32                      ulRslt              = NAS_EMM_FAIL;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgTcDataReq is entered.");

    /* ��������ָ��������, ״̬ƥ����,����ƥ��,�˳�*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_REG,
                                        EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM,
                                        pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_TAUSER_CHKFSMStateMsgp ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*CONNģʽ�£�ת��TC��Ϣ��IDLEģʽ�£���ӡ������Ϣ*/
    if((NAS_EMM_CONN_SIG == NAS_EMM_GetConnState()) ||
        (NAS_EMM_CONN_DATA == NAS_EMM_GetConnState()))
    {
        NAS_EMM_SER_SendMrrcDataReq_Tcdata((EMM_ETC_DATA_REQ_STRU *)pMsgStru);
    }
    else
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_MsRegSsRegAttemptUpdateMmMsgTcDataReq:Warning:RRC connection is not Exist!");
    }

    return NAS_LMM_MSG_HANDLED;
}


/*******************************************************************************
  Module   :
  Function : NAS_EMM_MsRegInitSsWaitCnAttachCnfMsgEsmDataReq
  Input    :
  Output   :
  NOTE     :
  Return   :
  History  :
    1.  lihong00150010  2009-09-25  �¹�����
*******************************************************************************/
VOS_UINT32 NAS_EMM_MsRegInitSsWaitCnAttachCnfMsgEsmDataReq
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    VOS_UINT32                          ulRslt;
    EMM_ESM_DATA_REQ_STRU              *pstEsmDataReq = (EMM_ESM_DATA_REQ_STRU*)pMsgStru;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO("NAS_EMM_MsRegInitSsWaitCnAttachCnfMsgEsmDataReq is entered.");

    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_REG_INIT,EMM_SS_ATTACH_WAIT_CN_ATTACH_CNF,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        /* ��ӡ�쳣 */
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_MsRegInitSsWaitCnAttachCnfMsgEsmDataReq ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    NAS_EMM_SER_SendMrrcDataReq_ESMdata(&pstEsmDataReq->stEsmMsg);

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32 NAS_EMM_MsRegSsNormalMsgEsmDataReq(VOS_UINT32  ulMsgId,
                                                   VOS_VOID   *pMsgStru
                               )
{
    VOS_UINT32                          ulRslt          = NAS_EMM_FAIL;
    EMM_ESM_DATA_REQ_STRU              *pstEsmDataReq   = (EMM_ESM_DATA_REQ_STRU*)pMsgStru;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO("NAS_EMM_MsRegSsNormalMsgEsmDataReq entered.");

    /* ��������ָ��������, ״̬ƥ����,����ƥ��,�˳�*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_REG,EMM_SS_REG_NORMAL_SERVICE,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_TAUSER_CHKFSMStateMsgp ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    if (VOS_TRUE == pstEsmDataReq->ulIsEmcType)
    {
        NAS_LMM_SetEmmInfoIsEmerPndEsting(VOS_TRUE);
    }

    /*CONN̬��ת��ESM��Ϣ*/
    if((NAS_EMM_CONN_SIG == NAS_EMM_GetConnState()) ||
        (NAS_EMM_CONN_DATA == NAS_EMM_GetConnState()))
    {
        NAS_EMM_SER_SendMrrcDataReq_ESMdata(&pstEsmDataReq->stEsmMsg);
        return NAS_LMM_MSG_HANDLED;
    }

    /* ���CSFB��ʱ��ʱ�������У����ȿ���CSFB��PS����ҵ�����ȼ��ŵ� ��
       (��: ��ʹ���ڷ���SER��Ҳ����ǰ�����ͷŹ����ж�ֱ��ʧ��)��
       �����������˴�����ESM�Ľ�������*/
    if((NAS_LMM_TIMER_RUNNING == NAS_LMM_IsPtlTimerRunning(TI_NAS_EMM_PTL_CSFB_DELAY))
        &&(VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
        &&(NAS_EMM_CONN_RELEASING == NAS_EMM_GetConnState()))
    {
        NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsRegSsNormalMsgEsmDataReq: Msg discard, CSFB delay timer is running.");
        return NAS_LMM_MSG_HANDLED;
    }

    NAS_EMM_SER_RcvEsmDataReq(pMsgStru);

    return NAS_LMM_MSG_HANDLED;

}

/*****************************************************************************
 Function Name   : NAS_EMM_MsRegSsNormalMsgSmsEstReq
 Description     : ��������״̬�´���SMS��������
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.sunbing 49683      2011-11-3  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_MsRegSsNormalMsgSmsEstReq
(
    VOS_UINT32  ulMsgId,
    VOS_VOID   *pMsgStru
)
{
    VOS_UINT32                          ulRslt          = NAS_EMM_FAIL;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO("NAS_EMM_MsRegSsNormalMsgSmsEstReq entered.");

    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_REG,EMM_SS_REG_NORMAL_SERVICE,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_TAUSER_CHKFSMStateMsgp ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*��Ϊ���������Ѿ�����Ԥ�����������������˵��CS���Ѿ�ע�ᣬ
      ��������ڿ���̬������ֱ�ӻظ������ɹ�*/
    if (NAS_EMM_NO == NAS_LMM_IsRrcConnectStatusIdle())
    {
        NAS_LMM_SndLmmSmsEstCnf();

        NAS_LMM_SetConnectionClientId(NAS_LMM_CONNECTION_CLIENT_ID_SMS);
        return NAS_LMM_MSG_HANDLED;
    }

    /*����SER����ԭ��Ϊ NAS_EMM_SER_START_CAUSE_SMS_EST_REQ*/
    NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_SMS_EST_REQ);

    /*Inform RABM that SER init*/
    NAS_EMM_SER_SendRabmReestInd(EMM_ERABM_REEST_STATE_INITIATE);

    /*������ʱ��3417*/
    NAS_LMM_StartStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417);

    /*ת��EMM״̬��MS_SER_INIT+SS_SER_WAIT_CN_CNF*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_SER_INIT, EMM_SS_SER_WAIT_CN_SER_CNF, TI_NAS_EMM_STATE_SERVICE_T3417);

    /*��ϲ�����MRRC_DATA_REQ(SERVICE_REQ)*/
    NAS_EMM_SER_SendMrrcDataReq_ServiceReq();

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32  NAS_EMM_MsRegSsRegAttemptUpdateMmMsgEsmDataReq
(
    VOS_UINT32  ulMsgId,
    VOS_VOID   *pMsgStru
)
{
    VOS_UINT32                          ulRslt          = NAS_EMM_FAIL;
    EMM_ESM_DATA_REQ_STRU              *pstEsmDataReq   = (EMM_ESM_DATA_REQ_STRU*)pMsgStru;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgEsmDataReq entered.");

    /* ��������ָ��������, ״̬ƥ����,����ƥ��,�˳�*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_REG,
                                        EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM,
                                        pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_TAUSER_CHKFSMStateMsgp ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    if (VOS_TRUE == pstEsmDataReq->ulIsEmcType)
    {
        NAS_LMM_SetEmmInfoIsEmerPndEsting(VOS_TRUE);
    }

    /*CONN̬��ת��ESM��Ϣ*/
    if((NAS_EMM_CONN_SIG == NAS_EMM_GetConnState()) ||
        (NAS_EMM_CONN_DATA == NAS_EMM_GetConnState()))
    {
        NAS_EMM_SER_SendMrrcDataReq_ESMdata(&pstEsmDataReq->stEsmMsg);
        return NAS_LMM_MSG_HANDLED;
    }
    /* ��¼UPDATE_MM��ʶ */
    /*NAS_LMM_SetEmmInfoUpdateMmFlag(NAS_EMM_UPDATE_MM_FLAG_VALID);*/
    NAS_EMM_SER_RcvEsmDataReq(pMsgStru);

    return NAS_LMM_MSG_HANDLED;
}



VOS_UINT32 NAS_EMM_MsTauInitMsgRabmReestReq(VOS_UINT32  ulMsgId,
                                                   VOS_VOID   *pMsgStru
                                )
{
    VOS_UINT32                      ulRslt                = NAS_EMM_FAIL;

    (VOS_VOID)ulMsgId;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "Nas_Emm_MsTauInitMsgRabmReestReq is entered.");

    /* ��������ָ��������, ״̬ƥ����,����ƥ��,�˳�*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_TAU_INIT,EMM_SS_TAU_WAIT_CN_TAU_CNF,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        /* ��ӡ�쳣 */
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_TAUSER_CHKFSMStateMsgp ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    return NAS_LMM_MSG_DISCARD;
    }
VOS_UINT32 NAS_EMM_MsTauInitMsgRrcPagingInd(VOS_UINT32  ulMsgId,
                                                   VOS_VOID   *pMsgStru
                                )
{
    VOS_UINT32                      ulRslt              = NAS_EMM_FAIL;

    (VOS_VOID)ulMsgId;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "Nas_Emm_MsTauInitMsgRrcPagingInd is entered.");

    /* ��������ָ��������, ״̬ƥ����,����ƥ��,�˳�*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_TAU_INIT,EMM_SS_TAU_WAIT_CN_TAU_CNF,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        /* ��ӡ�쳣 */
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_TAUSER_CHKFSMStateMsgp ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    return NAS_LMM_STORE_HIGH_PRIO_MSG;
}
VOS_UINT32 NAS_EMM_MsSerInitMsgEsmdataReq
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    VOS_UINT32                          ulRslt             = NAS_EMM_FAIL;
    EMM_ESM_DATA_REQ_STRU              *pstsmdatareq       = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsSerInitMsgEsmdataReq is entered.");

    /* ��������ָ��������, ״̬ƥ����,����ƥ��,�˳�*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_SER_INIT ,EMM_SS_SER_WAIT_CN_SER_CNF,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        /* ��ӡ�쳣 */
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_TAUSER_CHKFSMStateMsgp ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*SERģ�����л���ESM DATA��Ϣ*/
    pstsmdatareq = (EMM_ESM_DATA_REQ_STRU        *)pMsgStru;

    if (VOS_TRUE == pstsmdatareq->ulIsEmcType)
    {
        NAS_LMM_SetEmmInfoIsEmerPndEsting(VOS_TRUE);
		/* lihong00150010 emergency tau&service begin */
        /* ����������͵�ESM��Ϣ */
        NAS_EMM_SaveEmcEsmMsg(pMsgStru);
		/* lihong00150010 emergency tau&service end */
    }
    NAS_EMM_SER_SaveEsmMsg(pstsmdatareq);

    return  NAS_LMM_MSG_HANDLED;
}
VOS_UINT32 NAS_EMM_MsTauInitMsgEsmdataReq
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    VOS_UINT32                          ulRslt             = NAS_EMM_FAIL;
    EMM_ESM_DATA_REQ_STRU              *pstsmdatareq       = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "Nas_Emm_MsTauInitMsgEsmdataReq is entered.");

    /* ��������ָ��������, ״̬ƥ����,����ƥ��,�˳�*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_TAU_INIT,EMM_SS_TAU_WAIT_CN_TAU_CNF,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        /* ��ӡ�쳣 */
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_TAUSER_CHKFSMStateMsgp ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    pstsmdatareq = (EMM_ESM_DATA_REQ_STRU *)pMsgStru;

    if (VOS_TRUE == pstsmdatareq->ulIsEmcType)
    {
        NAS_LMM_SetEmmInfoIsEmerPndEsting(VOS_TRUE);
    }

    /* ����ǰTAU�Ǵ����SERVICE��TAU����,
       �򽫴��յ���ESM��Ϣ���� */
    if (NAS_EMM_COLLISION_SERVICE == NAS_EMM_TAU_GetEmmCollisionCtrl())
    {
        /*SERģ�����л���ESM DATA��Ϣ*/
        NAS_EMM_SER_SaveEsmMsg(pstsmdatareq);
        /* lihong00150010 emergency tau&service begin */
        if (VOS_TRUE == pstsmdatareq->ulIsEmcType)
        {
            /* ����������͵�ESM��Ϣ */
            NAS_EMM_SaveEmcEsmMsg(pMsgStru);
        }
        /* lihong00150010 emergency tau&service end */
        return  NAS_LMM_MSG_HANDLED;
    }

    /* ͸��SM��Ϣ */
    NAS_EMM_SER_SendMrrcDataReq_ESMdata(&pstsmdatareq->stEsmMsg);

    return  NAS_LMM_MSG_HANDLED;
}

/*****************************************************************************
 Function Name   : NAS_EMM_MsTauInitMsgRabmRelReq
 Description     : TauInit.WtTauCnf״̬���յ������쳣����ֹ��ǰTAU���̣�����ͬ
                   �ײ��쳣
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.sunbing           2010-12-29  Draft Enact
    2.lihong00150010    2012-12-14  Modify:Emergency

*****************************************************************************/
VOS_UINT32  NAS_EMM_MsTauInitMsgRabmRelReq(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsTauInitMsgRabmRelReq is entered.");
    (VOS_VOID)ulMsgId;
    (VOS_VOID)pMsgStru;

    /*��ֹ��ǰTAU����*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_TAU_T3430);

    NAS_EMM_TAU_GetEmmTAUAttemptCnt()++;

    NAS_EMM_TAU_ProcAbnormal();

    /* lihong00150010 emergency tau&service begin */
    if (NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_MsTauInitMsgRabmRelReq:CAUSE_ESM_EMC_PDN_REQ");

        NAS_EMM_EmcPndReqTauAbnormalCommProc(   NAS_EMM_MmcSendTauActionResultIndFailWithPara,
                                                (VOS_VOID*)NAS_EMM_NULL_PTR,
                                                EMM_SS_DEREG_NORMAL_SERVICE);
    }
    else
    {
        NAS_EMM_TAU_RelIndCollisionProc(NAS_EMM_MmcSendTauActionResultIndFailWithPara,
                        (VOS_VOID*)NAS_EMM_NULL_PTR);
    }
	/* lihong00150010 emergency tau&service end */
    #if 0
    /* ���������3402,���TAU���� */
    if(NAS_LMM_TIMER_RUNNING == NAS_LMM_IsPtlTimerRunning(TI_NAS_EMM_PTL_T3402))
    {
        NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);
    }
    #endif
    /*��MRRC����NAS_EMM_MRRC_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);



    return  NAS_LMM_MSG_HANDLED;
}
VOS_VOID    NAS_EMM_SER_RcvRabmReestReq
(
    VOS_UINT32                          ulIsEmcType
)
{
    NAS_EMM_SER_LOG_INFO( "Nas_Emm_Ser_RcvRabmReestReq is entered.");
	/* lihong00150010 emergency tau&service begin */
    if (VOS_TRUE == ulIsEmcType)
    {
        NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_RABM_REEST_EMC);
    }
    else
    {
        NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_RABM_REEST);
    }
	/* lihong00150010 emergency tau&service end */
    /*Inform RABM that SER init*/
    NAS_EMM_SER_SendRabmReestInd(EMM_ERABM_REEST_STATE_INITIATE);

    /*������ʱ��3417*/
    NAS_LMM_StartStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417);

    /*ת��EMM״̬��MS_SER_INIT+SS_SER_WAIT_CN_CNF*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_SER_INIT, EMM_SS_SER_WAIT_CN_SER_CNF, TI_NAS_EMM_STATE_SERVICE_T3417);

    /*��ϲ�����MRRC_DATA_REQ(SERVICE_REQ)*/
    NAS_EMM_SER_SendMrrcDataReq_ServiceReq();
    return;
}


VOS_VOID    NAS_EMM_SER_RcvRrcStmsiPagingInd(VOS_VOID)
{

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_RcvRrcStmsiPagingInd is entered.");

    /*����SER����ԭ��Ϊ NAS_EMM_SER_START_CAUSE_RRC_PAGING*/
    NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_RRC_PAGING);

    /*Inform RABM that SER init*/
    NAS_EMM_SER_SendRabmReestInd(EMM_ERABM_REEST_STATE_INITIATE);

    /*������ʱ��3417*/
    NAS_LMM_StartStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417);

    /*ת��EMM״̬��MS_SER_INIT+SS_SER_WAIT_CN_CNF*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_SER_INIT, EMM_SS_SER_WAIT_CN_SER_CNF, TI_NAS_EMM_STATE_SERVICE_T3417);

    /*��ϲ�����MRRC_DATA_REQ(SERVICE_REQ)*/
    NAS_EMM_SER_SendMrrcDataReq_ServiceReq();
    return;

}
VOS_UINT32 NAS_EMM_SER_VerifyMtCsfb( VOS_VOID )
{
    NAS_LMM_CS_SERVICE_ENUM_UINT32      ulCsService = NAS_LMM_CS_SERVICE_BUTT;

    /* �ж�ע�����Ƿ�ΪCS+PS */
    if (NAS_LMM_REG_DOMAIN_CS_PS != NAS_LMM_GetEmmInfoRegDomain())
    {
        NAS_EMM_SER_LOG_NORM( "NAS_EMM_SER_VerifyMtCsfb:cs is not registered!");
        return NAS_EMM_FAIL;
    }

    /* �ж�UE�Ƿ�֧��CSFB,���CS SERVICEδʹ�ܣ���Ĭ��֧��CSFB */
    ulCsService = NAS_EMM_GetCsService();
    if ((NAS_LMM_CS_SERVICE_CSFB_SMS != ulCsService)
        && (NAS_LMM_CS_SERVICE_BUTT != ulCsService))
    {
        NAS_EMM_SER_LOG_NORM( "NAS_EMM_SER_VerifyMtCsfb:ue is not support csfb!");
        return NAS_EMM_FAIL;
    }

    /* �ж������Ƿ�Я����SMS ONLY */
    /*leili modify for isr begin*/
    if (NAS_LMM_ADDITIONAL_UPDATE_SMS_ONLY == NAS_EMM_GetAddUpdateRslt())
    {
        NAS_EMM_SER_LOG_NORM( "NAS_EMM_SER_VerifyMtCsfb:additional update result sms only");
        return NAS_EMM_FAIL;
    }
    #if 0
    pstPubInfo = NAS_LMM_GetEmmInfoAddr();
    if ((NAS_EMM_BIT_SLCT == pstPubInfo->bitOpAddUpRslt)
        && (NAS_LMM_ADDITIONAL_UPDATE_SMS_ONLY == pstPubInfo->ulAdditionUpRslt))
    {
        NAS_EMM_SER_LOG_NORM( "NAS_EMM_SER_VerifyMtCsfb:additional update result sms only");
        return NAS_EMM_FAIL;
    }
    #endif
    /*leili modify for isr end*/

    /* �ж��Ƿ���L��ģ */
    if(NAS_EMM_SUCC != NAS_EMM_CheckMutiModeSupport())
    {
        NAS_EMM_SER_LOG_NORM( "NAS_EMM_SER_VerifyMtCsfb:lte only");
        return NAS_EMM_FAIL;
    }

    return NAS_EMM_SUCC;
}


VOS_UINT32 NAS_EMM_SER_VerifyCsfb(MM_LMM_CSFB_SERVICE_TYPE_ENUM_UINT32  enCsfbSrvType)
{

    NAS_LMM_CS_SERVICE_ENUM_UINT32      ulCsService = NAS_LMM_CS_SERVICE_BUTT;

    /* �ж�ע�����Ƿ�ΪCS+PS */
    if (NAS_LMM_REG_DOMAIN_CS_PS != NAS_LMM_GetEmmInfoRegDomain())
    {
       NAS_EMM_SER_LOG_NORM( "NAS_EMM_SER_VerifyCsfb:cs is not registered!");
       return NAS_EMM_FAIL;
    }

    /* �ж�UE�Ƿ�֧��CSFB,���CS SERVICEδʹ�ܣ���Ĭ��֧��CSFB */
    ulCsService = NAS_EMM_GetCsService();
    if ((NAS_LMM_CS_SERVICE_CSFB_SMS != ulCsService)
       && (NAS_LMM_CS_SERVICE_BUTT != ulCsService))
    {
       NAS_EMM_SER_LOG_NORM( "NAS_EMM_SER_VerifyCsfb:ue is not support csfb!");
       return NAS_EMM_FAIL;
    }

    /* ����MO���͵ģ�������̬�������ڽ������͵ģ���Ԥ���������Ѿ��жϣ�
    Ӧ�ò�����֣��ڵ��÷�֧��ע�� */
    /* �ж������Ƿ�Я����SMS ONLY */
    if ((NAS_LMM_ADDITIONAL_UPDATE_SMS_ONLY == NAS_EMM_GetAddUpdateRslt())
        && (MM_LMM_CSFB_SERVICE_MT_NORMAL == enCsfbSrvType))
    {
       NAS_EMM_SER_LOG_NORM( "NAS_EMM_SER_VerifyCsfb:additional update result sms only");
       return NAS_EMM_FAIL;
    }

    /* �ж��Ƿ���L��ģ */
    if(NAS_EMM_SUCC != NAS_EMM_CheckMutiModeSupport())
    {
       NAS_EMM_SER_LOG_NORM( "NAS_EMM_SER_VerifyCsfb:lte only");
       return NAS_EMM_FAIL;
    }
    return NAS_EMM_SUCC;
}



VOS_VOID    NAS_EMM_SER_RcvRrcCsPagingInd
(
    LRRC_LNAS_PAGING_UE_ID_ENUM_UINT32 enPagingUeId
)
{

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_RcvRrcCsPagingInd is entered.");

    /* ���MT CSFB�����Ƿ��ܹ����� */
    if (NAS_EMM_FAIL == NAS_EMM_SER_VerifyCsfb(MM_LMM_CSFB_SERVICE_MT_NORMAL))
    {
        NAS_EMM_SER_LOG_NORM( "NAS_EMM_SER_RcvRrcCsPagingInd:cannot csfb!");
        return ;
    }

    /* ��MMģ�鷢��MM_MM_CSFB_SERVICE_PAGING_IND��Ϣ */
    NAS_EMM_MmSendCsfbSerPaingInd(  NAS_EMM_MT_CSFB_TYPE_CS_PAGING,
                                    VOS_NULL_PTR,
                                    enPagingUeId);
    return;
}


VOS_VOID    NAS_EMM_SER_RcvEsmDataReq(VOS_VOID   *pMsgStru)
{
    EMM_ESM_DATA_REQ_STRU        *pstsmdatareq = (EMM_ESM_DATA_REQ_STRU*)pMsgStru;

    NAS_EMM_SER_LOG_INFO( "Nas_Emm_Ser_RcvEsmDataReq is entered.");
	/* lihong00150010 emergency tau&service begin */
    /* ����SERVICE����ԭ��ֵ */
    if (VOS_TRUE == pstsmdatareq->ulIsEmcType)
    {
        NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC);

        /* ����������͵�ESM��Ϣ */
        NAS_EMM_SaveEmcEsmMsg(          pMsgStru);
    }
    else
    {
        NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ);
    }
	/* lihong00150010 emergency tau&service end */
    /*Inform RABM that SER init*/
    NAS_EMM_SER_SendRabmReestInd(EMM_ERABM_REEST_STATE_INITIATE);

    /*SERģ�����л���ESM DATA��Ϣ*/
    pstsmdatareq = (EMM_ESM_DATA_REQ_STRU        *)pMsgStru;
    NAS_EMM_SER_SaveEsmMsg(pstsmdatareq);

    /*������ʱ��3417*/
    NAS_LMM_StartStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417);

    /*ת��EMM״̬��MS_SER_INIT+SS_SER_WAIT_CN_CNF*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_SER_INIT, EMM_SS_SER_WAIT_CN_SER_CNF, TI_NAS_EMM_STATE_SERVICE_T3417);

    /*��ϲ�����MRRC_DATA_REQ(SERVICE_REQ)*/
    NAS_EMM_SER_SendMrrcDataReq_ServiceReq();
    return;
}
VOS_VOID  NAS_EMM_SER_UplinkPending( VOS_VOID )
{
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_UplinkPending is entered.");

    /*����SER����ԭ��Ϊ NAS_ESM_SER_START_CAUSE_UPLINK_PENDING*/
    NAS_EMM_SER_SaveEmmSERStartCause(NAS_ESM_SER_START_CAUSE_UPLINK_PENDING);

    /*Inform RABM that SER init*/
    NAS_EMM_SER_SendRabmReestInd(EMM_ERABM_REEST_STATE_INITIATE);

    /*������ʱ��3417*/
    NAS_LMM_StartStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417);

    /*ת��EMM״̬��MS_SER_INIT+SS_SER_WAIT_CN_CNF*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_SER_INIT, EMM_SS_SER_WAIT_CN_SER_CNF, TI_NAS_EMM_STATE_SERVICE_T3417);

     /*��ϲ�����MRRC_DATA_REQ(SERVICE_REQ)*/
    NAS_EMM_SER_SendMrrcDataReq_ServiceReq();
    return;
}
VOS_VOID  NAS_EMM_SER_SmsEstReq( VOS_VOID )
{
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SmsEstReq is entered.");

    /*����SER����ԭ��Ϊ NAS_EMM_SER_START_CAUSE_SMS_EST_REQ*/
    NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_SMS_EST_REQ);

    /*Inform RABM that SER init*/
    NAS_EMM_SER_SendRabmReestInd(EMM_ERABM_REEST_STATE_INITIATE);

    /*��ϲ�����MRRC_DATA_REQ(SERVICE_REQ)*/
    NAS_EMM_SER_SendMrrcDataReq_ServiceReq();

    /*������ʱ��3417*/
    NAS_LMM_StartStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417);

    /*ת��EMM״̬��MS_SER_INIT+SS_SER_WAIT_CN_CNF*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_SER_INIT, EMM_SS_SER_WAIT_CN_SER_CNF, TI_NAS_EMM_STATE_SERVICE_T3417);

    return;
}
/*lint -e960*/
/*lint -e961*/
VOS_BOOL NAS_EMM_SER_IsSameEsmMsgInBuf
(
    const EMM_ESM_DATA_REQ_STRU               *pMsgStru
)
{
    VOS_UINT32                          i       = 0;
    EMM_ESM_DATA_REQ_STRU              *pEsmMsg = NAS_LMM_NULL_PTR;

    /* �����Ϣ���Ⱥ�������ͬ������Ϊ���ظ���Ϣ */
    for (i = 0; i < g_stEmmEsmMsgBuf.ulEsmMsgCnt; i++)
    {
        if (NAS_LMM_NULL_PTR != g_stEmmEsmMsgBuf.apucEsmMsgBuf[i])
        {
            pEsmMsg = (EMM_ESM_DATA_REQ_STRU *)g_stEmmEsmMsgBuf.apucEsmMsgBuf[i];

            /* lihong00150010 emergency tau&service begin */
            if ((pMsgStru->stEsmMsg.ulEsmMsgSize == pEsmMsg->stEsmMsg.ulEsmMsgSize)
             && (0 == NAS_LMM_MEM_CMP(pMsgStru->stEsmMsg.aucEsmMsg,
                                     pEsmMsg->stEsmMsg.aucEsmMsg,
                                     pEsmMsg->stEsmMsg.ulEsmMsgSize))
             && (pMsgStru->ulOpId == pEsmMsg->ulOpId)
             && (pMsgStru->ulIsEmcType == pEsmMsg->ulIsEmcType)
               )
            {
                return VOS_TRUE;
            }
            /* lihong00150010 emergency tau&service end */
        }
        else
        {
            NAS_EMM_SER_LOG_WARN( "NAS_EMM_SER_IsSameEsmMsgInBuf: Null buffer item.");
        }
    }

    return VOS_FALSE;
}

/* lihong00150010 emergency tau&service begin */

VOS_UINT32 NAS_EMM_SER_FindEsmMsg
(
    VOS_UINT32                          ulOpid
)
{
    VOS_UINT32                          ulIndex     = NAS_EMM_NULL;
    EMM_ESM_DATA_REQ_STRU              *pstEsmMsg   = NAS_EMM_NULL_PTR;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_FindEsmMsg is entered.");

    for (ulIndex = NAS_EMM_NULL; ulIndex < g_stEmmEsmMsgBuf.ulEsmMsgCnt; ulIndex++)
    {
        pstEsmMsg = (EMM_ESM_DATA_REQ_STRU *)g_stEmmEsmMsgBuf.apucEsmMsgBuf[ulIndex];
        if (ulOpid == pstEsmMsg->ulOpId)
        {
            return ulIndex;
        }
    }

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_FindEsmMsg failed!");

    return NAS_EMM_SER_MAX_ESM_BUFF_MSG_NUM;
}
VOS_VOID NAS_EMM_SER_DeleteEsmMsg
(
    VOS_UINT32                          ulOpid
)
{
    VOS_UINT32                          ulIndex = NAS_EMM_NULL;
    VOS_UINT32                          ulRslt  = NAS_EMM_NULL;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_DeleteEsmMsg is entered.");

    ulIndex = NAS_EMM_SER_FindEsmMsg(ulOpid);
    if (ulIndex >= g_stEmmEsmMsgBuf.ulEsmMsgCnt)
    {
        return ;
    }

    ulRslt = NAS_COMM_FreeBuffItem(NAS_COMM_BUFF_TYPE_EMM, g_stEmmEsmMsgBuf.apucEsmMsgBuf[ulIndex]);

    if (NAS_COMM_BUFF_SUCCESS != ulRslt)
    {
       NAS_EMM_SER_LOG_WARN("NAS_EMM_SER_DeleteEsmMsg, Memory Free is not succ");
    }

    g_stEmmEsmMsgBuf.apucEsmMsgBuf[ulIndex] = NAS_LMM_NULL_PTR;

    for (; ulIndex < (g_stEmmEsmMsgBuf.ulEsmMsgCnt - 1); ulIndex++)
    {
        g_stEmmEsmMsgBuf.apucEsmMsgBuf[ulIndex] = g_stEmmEsmMsgBuf.apucEsmMsgBuf[ulIndex+1];
    }

    g_stEmmEsmMsgBuf.apucEsmMsgBuf[g_stEmmEsmMsgBuf.ulEsmMsgCnt - 1] = NAS_LMM_NULL_PTR;

    g_stEmmEsmMsgBuf.ulEsmMsgCnt--;
}
/* lihong00150010 emergency tau&service end */


VOS_VOID NAS_EMM_SER_SaveEsmMsg(const EMM_ESM_DATA_REQ_STRU  *pMsgStru)
{
    VOS_VOID                            *pMsgBuf   = NAS_LMM_NULL_PTR;
    VOS_UINT32                           ulBufSize = 0;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_Ser_SaveEsmMsg is entered.");

    /* �����ظ���SM��Ϣ���������*/
    if ((VOS_FALSE == NAS_EMM_SER_IsSameEsmMsgInBuf(pMsgStru))
     && (NAS_EMM_SER_MAX_ESM_BUFF_MSG_NUM > g_stEmmEsmMsgBuf.ulEsmMsgCnt))
    {
        ulBufSize = pMsgStru->stEsmMsg.ulEsmMsgSize +
                    sizeof(pMsgStru->stEsmMsg.ulEsmMsgSize) +
                    sizeof(pMsgStru->ulOpId) +
                    sizeof(pMsgStru->ulIsEmcType) +
                    EMM_LEN_VOS_MSG_HEADER +
                    EMM_LEN_MSG_ID;

        /* ����ռ� */
        pMsgBuf = NAS_COMM_AllocBuffItem(NAS_COMM_BUFF_TYPE_EMM, ulBufSize);

        if (NAS_LMM_NULL_PTR != pMsgBuf)
        {
            NAS_LMM_MEM_CPY(pMsgBuf, pMsgStru, ulBufSize);

            g_stEmmEsmMsgBuf.apucEsmMsgBuf[g_stEmmEsmMsgBuf.ulEsmMsgCnt] = pMsgBuf;
            g_stEmmEsmMsgBuf.ulEsmMsgCnt++;

        }
        else
        {
            NAS_EMM_SER_LOG_INFO( "NAS_EMM_Ser_SaveEsmMsg: NAS_AllocBuffItem return null pointer.");
        }
    }
    else
    {
        NAS_EMM_SER_LOG1_INFO( "NAS_EMM_Ser_SaveEsmMsg: ESM Msg Not Buffered, Buffered msg number is:",
                               g_stEmmEsmMsgBuf.ulEsmMsgCnt);
    }

    NAS_EMM_SER_LOG1_INFO( "NAS_EMM_Ser_SaveEsmMsg: Buffered msg number is:",
                           g_stEmmEsmMsgBuf.ulEsmMsgCnt);

    return;
}

#if 0
VOS_VOID  NAS_EMM_SER_GetEsmMsg( EMM_ESM_DATA_REQ_STRU *pstData )
{
    EMM_ESM_DATA_REQ_STRU               *pstEsmData = NAS_LMM_NULL_PTR;
    VOS_UINT32                          ulLen = NAS_LMM_NULL;

    if (NAS_LMM_NULL == g_stEmmEsmMsgBuf.ulEsmMsgCnt)
    {
        NAS_EMM_SER_LOG_NORM( "NAS_EMM_SER_GetEsmMsg: ESM msg is not exist.");
        return;
    }

    pstEsmData = (EMM_ESM_DATA_REQ_STRU *)g_stEmmEsmMsgBuf.apucEsmMsgBuf[g_stEmmEsmMsgBuf.ulEsmMsgCnt-1];

    ulLen   = pstEsmData->stEsmMsg.ulEsmMsgSize + sizeof(pstEsmData->stEsmMsg.ulEsmMsgSize)
                + EMM_LEN_VOS_MSG_HEADER + EMM_LEN_MSG_ID;

    NAS_LMM_MEM_CPY(pstData,pstEsmData,ulLen);


    return;
}
#endif



VOS_UINT32 NAS_EMM_EmmMsRegInitSsWaitRrcDataCnfMsgEsmDataReq
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    EMM_ESM_DATA_REQ_STRU              *pstEsmDataReq = (EMM_ESM_DATA_REQ_STRU*)pMsgStru;

    (VOS_VOID)(ulMsgId);
    NAS_EMM_SER_LOG_INFO("NAS_EMM_EmmMsRegInitSsWaitRrcDataCnfMsgEsmDataReq is entered.");
    NAS_EMM_SER_SendMrrcDataReq_ESMdata(&pstEsmDataReq->stEsmMsg);
    return NAS_LMM_MSG_HANDLED;
}




VOS_VOID  NAS_EMM_MsTauSerSsWaitCnCnfEmergencyCsfbProc(VOS_VOID)
{
    MMC_LMM_TAU_RSLT_ENUM_UINT32        ulTauRslt = MMC_LMM_TAU_RSLT_BUTT;

    /* TAU������, ���������Զ������Դ */
    if (EMM_MS_TAU_INIT == NAS_LMM_GetEmmCurFsmMS())
    {
        /* ��MMC����LMM_MMC_TAU_RESULT_IND��Ϣ */
        ulTauRslt = MMC_LMM_TAU_RSLT_FAILURE;
        NAS_EMM_MmcSendTauActionResultIndOthertype((VOS_VOID*)&ulTauRslt);

        NAS_EMM_TAU_AbnormalOver();
    }
    else  /* SER������,��ֹSER */
    {
        NAS_EMM_SER_AbnormalOver();
    }

    /* ת��REG.PLMN-SEARCH��MMC���� */
    NAS_EMM_AdStateConvert(EMM_MS_REG,
                           EMM_SS_REG_PLMN_SEARCH,
                           TI_NAS_EMM_STATE_NO_TIMER);

    /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
    NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);

    /* �����������̬��ѹջ�ͷŴ��� */
    NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);

    return;

}


VOS_VOID  NAS_EMM_MsAnySsWaitCnDetachCnfEmergencyCsfbProc(VOS_VOID)
{
    NAS_LMM_StopStateTimer(TI_NAS_EMM_T3421);

    /* REG. EMM_SS_REG_IMSI_DETACH_WATI_CN_DETACH_CNF ̬*/
    if (EMM_MS_REG == NAS_LMM_GetEmmCurFsmMS())
    {
        NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_PS);

        /* ��MMC���ͱ���LMM_MMC_DETACH_IND��Ϣ */
        NAS_EMM_SendDetRslt(MMC_LMM_DETACH_RSLT_SUCCESS);
        NAS_EMM_AdStateConvert(EMM_MS_REG,
                               EMM_SS_REG_NORMAL_SERVICE,
                               TI_NAS_EMM_STATE_NO_TIMER);

        NAS_LMM_ImsiDetachReleaseResource();
    }
    else /* DEREG_INIT. EMM_SS_DETACH_WAIT_CN_DETACH_CNF ̬*/
    {
        /*��MMC���ͱ���LMM_MMC_DETACH_IND��Ϣ*/
        NAS_EMM_SendDetRslt(MMC_LMM_DETACH_RSLT_SUCCESS);
        NAS_EMM_AdStateConvert(EMM_MS_DEREG,
                               EMM_SS_DEREG_NORMAL_SERVICE,
                               TI_NAS_EMM_STATE_NO_TIMER);

        /* ֪ͨESM�����Դ */
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }

    /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
    NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);

    /* �����������̬��ѹջ�ͷŴ��� */
    NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);

    return;

}
VOS_VOID  NAS_EMM_MsRegInitSsAnyStateEmergencyCsfbProc(VOS_VOID)
{
    /* ��MMC�ϱ�ATTACH���Ϊʧ�� */
    NAS_EMM_AppSendAttOtherType(MMC_LMM_ATT_RSLT_FAILURE);

    /* ATTACHͣ��ʱ�� + �����Դ */
    NAS_EMM_Attach_SuspendInitClearResourse();

    /* �޸�״̬��������״̬DEREG��״̬DEREG_PLMN_SEARCH, ��ʱ����״̬���ϱ��ı�*/
    NAS_EMM_AdStateConvert(EMM_MS_DEREG,
                           EMM_SS_DEREG_PLMN_SEARCH ,
                           TI_NAS_EMM_STATE_NO_TIMER);

    /* ֪ͨESM�����Դ */
    NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);

    /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
    NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);

    /* �����������̬��ѹջ�ͷŴ��� */
    NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);

    return;

}


VOS_UINT32  NAS_EMM_UnableDirectlyStartMoEmergencyCsfbProc(VOS_VOID)
{
    switch(NAS_EMM_CUR_MAIN_STAT)
    {
        case    EMM_MS_REG_INIT:
            NAS_EMM_MsRegInitSsAnyStateEmergencyCsfbProc();
            break;

        case    EMM_MS_TAU_INIT:
        case    EMM_MS_SER_INIT:
            NAS_EMM_MsTauSerSsWaitCnCnfEmergencyCsfbProc();
            break;

        case    EMM_MS_RRC_CONN_EST_INIT:
        case    EMM_MS_RRC_CONN_REL_INIT:
            NAS_EMM_SER_LOG_NORM( "NAS_EMM_CannotDirectlyStartMoEmergencyCsfbProc:High priority storage!");
            return NAS_LMM_STORE_HIGH_PRIO_MSG;

        case    EMM_MS_AUTH_INIT:
        case    EMM_MS_RESUME:
            NAS_EMM_SER_LOG_NORM( "NAS_EMM_CannotDirectlyStartMoEmergencyCsfbProc:Low priority storage!");
            return NAS_LMM_STORE_LOW_PRIO_MSG;

        case    EMM_MS_REG:
        case    EMM_MS_DEREG_INIT:
            if ((EMM_SS_REG_IMSI_DETACH_WATI_CN_DETACH_CNF == NAS_LMM_GetEmmCurFsmSS())
                || (EMM_SS_DETACH_WAIT_CN_DETACH_CNF == NAS_LMM_GetEmmCurFsmSS()))
            {
                NAS_EMM_MsAnySsWaitCnDetachCnfEmergencyCsfbProc();
            }
            else
            {
                /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
                NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);
                NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
            }
            break;

        default:

            /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
            NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);
            NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
            break;
    }

    return NAS_LMM_MSG_HANDLED;

}



VOS_UINT32  NAS_EMM_MsRegPreProcMmNormalCsfbNotifyMsg
(
    MM_LMM_CSFB_SERVICE_TYPE_ENUM_UINT32  enCsfbSrvTyp
)
{
    NAS_EMM_SER_LOG_NORM( "NAS_EMM_MsRegPreProcMmNormalCsfbNotifyMsg:enter!");

    if ((EMM_SS_REG_NORMAL_SERVICE == NAS_EMM_CUR_SUB_STAT)
        || (EMM_SS_REG_WAIT_ACCESS_GRANT_IND == NAS_EMM_CUR_SUB_STAT))
    {
        NAS_EMM_SER_LOG_NORM( "NAS_EMM_MsRegPreProcMmNormalCsfbNotifyMsg:REG+NORMAL!");
        return NAS_EMM_FAIL;
    }
    /* ���⼸��״̬�£������MO���͵ģ���T3442û�������У��Ҳ���SMS ONLY,��ѡ����GUģ */
    else if ((EMM_SS_REG_ATTEMPTING_TO_UPDATE == NAS_EMM_CUR_SUB_STAT)
                || (EMM_SS_REG_LIMITED_SERVICE == NAS_EMM_CUR_SUB_STAT)
                || (EMM_SS_REG_NO_CELL_AVAILABLE == NAS_EMM_CUR_SUB_STAT))
    {
        if (MM_LMM_CSFB_SERVICE_MO_NORMAL == enCsfbSrvTyp)
        {
            NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);
            return NAS_EMM_SUCC;
        }
        NAS_EMM_MmSendCsfbSerEndInd(MMC_LMM_SERVICE_RSLT_FAILURE);
        return NAS_EMM_SUCC;
    }
    else
    {
        NAS_EMM_PUBU_LOG1_ERR("NAS_EMM_RcvMmNormalCsfbNotifyMsgProc: Sub State is err!",NAS_EMM_CUR_SUB_STAT);

        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_FAILURE);

        return NAS_EMM_SUCC;
    }
}
VOS_UINT32  NAS_EMM_RcvMmNormalCsfbNotifyMsgProc
(
    MM_LMM_CSFB_SERVICE_TYPE_ENUM_UINT32  enCsfbSrvTyp
)
{
    /* ���CSFB ABORT��ʶ */
    NAS_EMM_SER_SaveEmmSerCsfbAbortFlag(NAS_EMM_CSFB_ABORT_FLAG_INVALID);

    /* ���CSFB�����Ƿ��ܹ����� */
    if (NAS_EMM_FAIL == NAS_EMM_SER_VerifyCsfb(enCsfbSrvTyp))
    {
        NAS_EMM_SER_LOG_NORM( "NAS_EMM_RcvMmNormalCsfbNotifyMsgProc:cannot csfb!");
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_FAILURE);
        return NAS_LMM_MSG_HANDLED;
    }

    if ((MM_LMM_CSFB_SERVICE_MO_NORMAL == enCsfbSrvTyp)
        && ((NAS_LMM_TIMER_RUNNING == NAS_LMM_IsStaTimerRunning(TI_NAS_EMM_STATE_SERVICE_T3442))
            || (NAS_LMM_ADDITIONAL_UPDATE_SMS_ONLY == NAS_EMM_GetAddUpdateRslt())))
    {
        NAS_EMM_SER_LOG_NORM( "NAS_EMM_RcvMmNormalCsfbNotifyMsgProc:SMS ONLY and mo csfb!");
        NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);
        return NAS_LMM_MSG_HANDLED;
    }

    switch(NAS_EMM_CUR_MAIN_STAT)
    {
        /*ѹջ�м�״̬����Ҫ�����ȼ����棬��pop��״̬֮���ٴ���*/
        case    EMM_MS_RRC_CONN_EST_INIT:
        case    EMM_MS_RRC_CONN_REL_INIT:
            NAS_EMM_SER_LOG_NORM( "NAS_EMM_RcvMmNormalCsfbNotifyMsgProc:High priority storage!");
            return NAS_LMM_STORE_HIGH_PRIO_MSG;

        /*TAU��SR�����У���Ȩ�����У��ָ������У������ȼ����棬��������̬����*/
        case    EMM_MS_TAU_INIT:
        case    EMM_MS_AUTH_INIT:
        case    EMM_MS_RESUME:
            NAS_EMM_SER_LOG_NORM( "NAS_EMM_RcvMmNormalCsfbNotifyMsgProc:Low priority storage!");
            return NAS_LMM_STORE_LOW_PRIO_MSG;

        case    EMM_MS_SER_INIT:

            /* �жϵ�ǰ��MT CSFB������ESR�����У��������ٴδ���MT CSFB�������´���ESR����ʱ��T3417EXT��������
               ����յ���MO���͵�CSFB��ֱ�Ӷ���*/
            if ((NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause()))
            {
                if (MM_LMM_CSFB_SERVICE_MT_NORMAL == enCsfbSrvTyp)
                {
                    /*ֹͣ��ʱ��T3417ext*/
                    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

                    /*����SER����ԭ��ֵ*/
                    NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ);

                    /* ����UE����CSFB */
                    NAS_EMM_SER_SaveEmmSerCsfbRsp(NAS_EMM_CSFB_RSP_ACCEPTED_BY_UE);

                    /*��ϲ�����MRRC_DATA_REQ(SERVICE_REQ)*/
                    NAS_EMM_SER_SendMrrcDataReq_ExtendedServiceReq();

                    /*������ʱ��T3417ext*/
                    NAS_LMM_StartStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417_EXT);
                }
                return NAS_LMM_MSG_HANDLED;
            }
            else
            {
                return NAS_LMM_STORE_LOW_PRIO_MSG;
            }

        /*ֻ��REG+NORMAL_SERVICE̬��REG+WAIT_ACCESS_GRANT_IND̬�ܷ���CSFB*/
        case    EMM_MS_REG:
            if (NAS_EMM_FAIL == NAS_EMM_MsRegPreProcMmNormalCsfbNotifyMsg(enCsfbSrvTyp))
            {
                return NAS_LMM_MSG_DISCARD;
            }
            else
            {
                return NAS_LMM_MSG_HANDLED;
            }

        default:/*����״̬Ϊ�����״̬�����Ӹ澯��ӡ*/
            NAS_EMM_PUBU_LOG1_ERR("NAS_EMM_RcvMmNormalCsfbNotifyMsgProc: Main State is err!",NAS_EMM_CUR_MAIN_STAT);

            /*Ϊ�ݴ����Ӷ�MM�Ļظ�*/
            NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_FAILURE);

            return NAS_LMM_MSG_HANDLED;
    }
}
/*lint +e961*/
/*lint +e960*/

VOS_UINT32  NAS_EMM_RcvMmMoEmergencyCsfbNotifyMsgProc(VOS_VOID)
{
    /* ���CSFB ABORT��ʶ */
    NAS_EMM_SER_SaveEmmSerCsfbAbortFlag(NAS_EMM_CSFB_ABORT_FLAG_INVALID);

    /* L��ģ���ܷ������CSFB */
    if (NAS_EMM_SUCC != NAS_EMM_CheckMutiModeSupport())
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_FAILURE);
        return NAS_LMM_MSG_HANDLED;
    }

    /* ����SERVICE����ԭ��Ϊ����CSFB, ���ڸ�MMC�ϱ�SERVICE_RESULT_IND */
    NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_MO_EMERGENCY_CSFB_REQ);

    /* ���ܿ���ֱ�ӷ������CSFB����, ���ݵ�ǰ����״̬���в�ͬ����*/
    if (NAS_EMM_SUCC == NAS_EMM_SER_VerifyCsfb(MM_LMM_CSFB_SERVICE_MO_EMERGENCY))
    {
        switch(NAS_EMM_CUR_MAIN_STAT)
        {
            case    EMM_MS_RRC_CONN_EST_INIT:
            case    EMM_MS_RRC_CONN_REL_INIT:
                NAS_EMM_SER_LOG_NORM( "NAS_EMM_RcvMmMoEmergencyCsfbNotifyMsgProc:High priority storage!");
                return NAS_LMM_STORE_HIGH_PRIO_MSG;

            case    EMM_MS_TAU_INIT:
            case    EMM_MS_SER_INIT:
            case    EMM_MS_AUTH_INIT:
            case    EMM_MS_RESUME:
                NAS_EMM_SER_LOG_NORM( "NAS_EMM_RcvMmMoEmergencyCsfbNotifyMsgProc:Low priority storage!");
                return NAS_LMM_STORE_LOW_PRIO_MSG;

            /*ֻ��REG+NORMAL_SERVICE̬����ֱ�ӷ������CSFB*/
            case    EMM_MS_REG:
                if (EMM_SS_REG_NORMAL_SERVICE == NAS_EMM_CUR_SUB_STAT)
                {
                    return NAS_LMM_MSG_DISCARD;
                }
                break;

            default:
                break;

        }
    }

    /* ��Ȼ����ֱ�ӷ������״̬����������������Ϊ���ܷ��� */
    return NAS_EMM_UnableDirectlyStartMoEmergencyCsfbProc();

}

VOS_UINT32  NAS_EMM_PreProcMsgMmCsfbSerStartNotify( MsgBlock * pMsg )
{
    MM_LMM_CSFB_SERVICE_START_NOTIFY_STRU *pstCsfbSerStartNotify = VOS_NULL_PTR;

    pstCsfbSerStartNotify = (MM_LMM_CSFB_SERVICE_START_NOTIFY_STRU*)pMsg;

    /* ����CSFB�Ĵ��� */
    if (MM_LMM_CSFB_SERVICE_MO_EMERGENCY == pstCsfbSerStartNotify->enCsfbSrvType)
    {
        return NAS_EMM_RcvMmMoEmergencyCsfbNotifyMsgProc();
    }
    else  /* MO��MT��NORMAL CSFB�Ĵ��� */
    {
        return NAS_EMM_RcvMmNormalCsfbNotifyMsgProc(pstCsfbSerStartNotify->enCsfbSrvType);
    }
}


VOS_UINT32  NAS_EMM_MsRegSsNormalMsgMmCsfbSerStartNotify
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    VOS_UINT32                             ulRslt                   = NAS_EMM_FAIL;
    MM_LMM_CSFB_SERVICE_START_NOTIFY_STRU *pstCsfbSerStartNotify    = VOS_NULL_PTR;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO("NAS_EMM_MsRegSsNormalMsgMmCsfbSerStartNotify entered.");

    pstCsfbSerStartNotify = (MM_LMM_CSFB_SERVICE_START_NOTIFY_STRU *)pMsgStru;

    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_REG,EMM_SS_REG_NORMAL_SERVICE,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_MsRegSsNormalMsgMmCsfbSerStartNotify ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*����SER����ԭ��ֵ*/
    if (MM_LMM_CSFB_SERVICE_MO_NORMAL == pstCsfbSerStartNotify->enCsfbSrvType)
    {
        NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_MO_CSFB_REQ);
    }
    else if (MM_LMM_CSFB_SERVICE_MO_EMERGENCY == pstCsfbSerStartNotify->enCsfbSrvType)
    {
        NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_MO_EMERGENCY_CSFB_REQ);
    }
    else
    {
        NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ);
    }

    /* ��������ͷŹ����У���ײ㴦��δפ��״̬������CSFB��ʱ��ʱ�������յ�ϵͳ��Ϣʱ�ٿ��Ƿ��� */
    if(NAS_EMM_CONN_RELEASING == NAS_EMM_GetConnState())
    {
       NAS_LMM_StartPtlTimer(TI_NAS_EMM_PTL_CSFB_DELAY);
       return NAS_LMM_MSG_HANDLED;
    }

    /* ����UE����CSFB */
    NAS_EMM_SER_SaveEmmSerCsfbRsp(NAS_EMM_CSFB_RSP_ACCEPTED_BY_UE);

    /*������ʱ��3417*/
    NAS_LMM_StartStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

    /*ת��EMM״̬��MS_SER_INIT+SS_SER_WAIT_CN_CNF*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_SER_INIT, EMM_SS_SER_WAIT_CN_SER_CNF, TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

    /*��ϲ�����MRRC_DATA_REQ(SERVICE_REQ)*/
    NAS_EMM_SER_SendMrrcDataReq_ExtendedServiceReq();
    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32  NAS_EMM_MsRegSsWaitAccessGrantIndMsgMmCsfbSerStartNotify
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    VOS_UINT32                             ulRslt                   = NAS_EMM_FAIL;
    MM_LMM_CSFB_SERVICE_START_NOTIFY_STRU *pstCsfbSerStartNotify    = VOS_NULL_PTR;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO("NAS_EMM_MsRegSsWaitAccessGrantIndMsgMmCsfbSerStartNotify entered.");

    pstCsfbSerStartNotify = (MM_LMM_CSFB_SERVICE_START_NOTIFY_STRU *)pMsgStru;

    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_REG,EMM_SS_REG_WAIT_ACCESS_GRANT_IND,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_MsRegSsWaitAccessGrantIndMsgMmCsfbSerStartNotify ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* MO CSFB���ܷ��� */
    if (MM_LMM_CSFB_SERVICE_MO_NORMAL == pstCsfbSerStartNotify->enCsfbSrvType)
    {
        NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);
        return NAS_LMM_MSG_HANDLED;
    }

    /* ��״̬�������յ�����CSFB,Ԥ�������ѹ�� */


    /* ����SER����ԭ��ΪMT CSFB */
    NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ);

    /* ����UE����CSFB */
    NAS_EMM_SER_SaveEmmSerCsfbRsp(NAS_EMM_CSFB_RSP_ACCEPTED_BY_UE);

    /*������ʱ��3417*/
    NAS_LMM_StartStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

    /*ת��EMM״̬��MS_SER_INIT+SS_SER_WAIT_CN_CNF*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_SER_INIT, EMM_SS_SER_WAIT_CN_SER_CNF, TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

    /*��ϲ�����MRRC_DATA_REQ(SERVICE_REQ)*/
    NAS_EMM_SER_SendMrrcDataReq_ExtendedServiceReq();
    return NAS_LMM_MSG_HANDLED;
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif



