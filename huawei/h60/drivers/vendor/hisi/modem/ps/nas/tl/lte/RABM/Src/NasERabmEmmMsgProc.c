

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include  "NasERabmEmmMsgProc.h"
#include  "NasERabmRrcMsgProc.h"
#include  "NasERabmIpFilter.h"
#include  "NasERabmETcMsgProc.h"
#include  "LUPAppItf.h"
/*#include  "R_ITF_FlowCtrl.h"*/
#include  "NasERabmCdsMsgProc.h"
#include  "CdsInterface.h"



/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASRABMEMMMSGPROC_C
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


/*****************************************************************************
  3 Function
*****************************************************************************/


VOS_VOID NAS_ERABM_EmmMsgDistr( VOS_VOID *pRcvMsg )
{
    PS_MSG_HEADER_STRU         *pEmmMsg = VOS_NULL_PTR;

    pEmmMsg = (PS_MSG_HEADER_STRU*)pRcvMsg;

    /*��ӡ����ú���*/
    NAS_ERABM_INFO_LOG("NAS_ERABM_EmmMsgDistr is entered.");

    /*������Ϣ����������Ӧ����Ϣ������*/
    switch (pEmmMsg->ulMsgName)
    {
        /*����յ�����EMM_ERABM_REEST_IND��Ϣ������NAS_ERABM_RcvRabmEmmReest_Ind����*/
        case ID_EMM_ERABM_REEST_IND:
            NAS_ERABM_RcvRabmEmmReestInd((EMM_ERABM_REEST_IND_STRU*)pRcvMsg);
            break;

        /*����յ�����EMM_ERABM_REL_IND��Ϣ������NAS_ERABM_RcvRabmEmmRelInd����*/
        case ID_EMM_ERABM_REL_IND:
            NAS_ERABM_RcvRabmEmmRelInd();
            break;

        case ID_EMM_ERABM_RRC_CON_REL_IND:
            NAS_ERABM_RcvRabmEmmRrcConRelInd();
            break;

        case ID_EMM_ERABM_SUSPEND_IND:
            NAS_ERABM_RcvRabmEmmSuspendInd( (EMM_ERABM_SUSPEND_IND_STRU *) pRcvMsg );
            break;

        case ID_EMM_ERABM_RESUME_IND:
            NAS_ERABM_RcvRabmEmmResumeInd( (EMM_ERABM_RESUME_IND_STRU *) pRcvMsg );
            break;

        default:
            NAS_ERABM_WARN_LOG("NAS_ERABM_EmmMsgDistr:WARNING:EMM->RABM Message name non-existent!");
            break;
    }
}


/*lint -e960*/
/*lint -e961*/
VOS_VOID  NAS_ERABM_RcvRabmEmmReestInd(const EMM_ERABM_REEST_IND_STRU *pRcvMsg )
{
    if (NAS_ERABM_L_MODE_STATUS_NORMAL != NAS_ERABM_GetLModeStatus())
    {
        NAS_ERABM_WARN_LOG("NAS_ERABM_RcvRabmEmmReestInd:Rabm is already suspended!");
        return ;
    }

    switch(pRcvMsg->enReEstState)
    {
        /*case EMM_ERABM_REEST_STATE_SUCC:*/
        case EMM_ERABM_REEST_STATE_FAIL:

            /*��ӡ����ú���*/
            NAS_ERABM_LOG1("NAS_ERABM_RcvRabmEmmReestInd: receive Emm Reset result: .",
                          pRcvMsg->enReEstState);
            /*SERVICE���̽���*,ֹͣ��ʱ��*/
            NAS_ERABM_TimerStop(NAS_ERABM_WAIT_RB_REEST_TIMER);
            NAS_ERABM_SetEmmSrState(NAS_ERABM_SERVICE_STATE_TERMIN);
            /* ���������ģʽ�� */
            if (NAS_ERABM_MODE_TYPE_NORMAL == NAS_ERABM_GetMode())
            {
                /* ֪ͨL2�ͷŻ�������ݰ� */
                /*APPITF_FreeAllofRabmBuf();*/

                /* ֪ͨCDS�ͷŻ�������ݰ� */
                NAS_ERABM_SndErabmCdsFreeBuffDataInd();

                /* �����������������־*/
                NAS_ERABM_SetUpDataPending(EMM_ERABM_NO_UP_DATA_PENDING);
            }
            else if (NAS_ERABM_TIMER_STATE_STOPED == NAS_ERABM_IsTimerRunning(NAS_ERABM_WAIT_TC_FREE_BUFFER_TIMER))
            /*����ģʽ�²������棬����ԭ��������Pending״̬*/
            {
                NAS_ERABM_INFO_LOG("NAS_ERABM_RcvRabmEmmReestInd: NAS_ERABM_WAIT_TC_FREE_BUFFER_TIMER start.");
                /* lihong00150010 emergency tau&service begin */
                NAS_ERABM_TimerStart(   NAS_ERABM_WAIT_TC_FREE_BUFFER_LENGTH,
                                        NAS_ERABM_WAIT_TC_FREE_BUFFER_TIMER,
                                        NAS_ERABM_WAIT_TC_FREE_BUFFER_TIMER);
            }   /* lihong00150010 emergency tau&service end */
            else
            {
                NAS_ERABM_INFO_LOG("NAS_ERABM_RcvRabmEmmReestInd: NAS_ERABM_WAIT_TC_FREE_BUFFER_TIMER is running.");
            }
            break;

        case EMM_ERABM_REEST_STATE_INITIATE:

            /*��ӡ����ú���*/
            NAS_ERABM_INFO_LOG("NAS_ERABM_RcvRabmEmmReestInd: receive EMM_ERABM_REEST_STATE_INITIATE .");
            /*SERVICE��������,������ʱ��*/
            if (NAS_ERABM_SERVICE_STATE_TERMIN == NAS_ERABM_GetEmmSrState())
            {/* lihong00150010 emergency tau&service begin */
                NAS_ERABM_TimerStart(   NAS_ERABM_WAIT_RB_REEST_LENGTH,
                                        NAS_ERABM_WAIT_RB_REEST_TIMER,
                                        VOS_FALSE);
                NAS_ERABM_SetEmmSrState(NAS_ERABM_SERVICE_STATE_INIT);
            }/* lihong00150010 emergency tau&service end */
            break;

        default:
            NAS_ERABM_WARN_LOG("NAS_ERABM_RcvRabmEmmReestInd:WARNNING: EstState illegal!");
            break;
    }
}
VOS_VOID  NAS_ERABM_RcvRabmEmmRelInd( VOS_VOID )
{
    VOS_UINT32 ulLoop = NAS_ERABM_NULL;

    /*��ӡ����ú���*/
    NAS_ERABM_INFO_LOG("NAS_ERABM_RcvRabmEmmRelInd is entered.");

    /* ֪ͨCDS RAB RELEASE */
    for (ulLoop = NAS_ERABM_MIN_EPSB_ID; ulLoop <= NAS_ERABM_MAX_EPSB_ID; ulLoop++)
    {
        if (NAS_ERABM_EPSB_ACTIVE == NAS_ERABM_GetEpsbStateInfo(ulLoop))
        {
            /* ��CDS����CDS_ERABM_RAB_RELEASE_IND */
            NAS_ERABM_SndErabmCdsRabReleaseInd(ulLoop);
        }
    }

    /*�ͷ�������Դ,�ص�������ʼ�����״̬*/
    NAS_ERABM_ClearRabmResource();

    /*����TFT�ͳ�����Ϣ����IP Filter��Ӳ��������*/
    if(PS_FAIL == NAS_ERABM_IpfConfigUlFilter())
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_EsmMsgDistr: IPF Update IP Filter Failed.");
    }

    /*����TFT�ͳ�����Ϣ����IP Filter��CDS*/
    if(PS_FAIL == NAS_ERABM_CdsConfigUlFilter())
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_EsmMsgDistr: CDS Update IP Filter Failed.");
    }
    #if 0
    /* ��������TFT��Ϣ */
    if(PS_FAIL == NAS_ERABM_IpfConfigDlFilter())
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_RcvRabmEmmResumeInd: Update DL IP Filter Failed.");
    }
    #endif
}
VOS_VOID  NAS_ERABM_RcvRabmEmmRrcConRelInd( VOS_VOID )
{
    VOS_UINT32                          ulEpsbId = NAS_ERABM_NULL;

    /*��ӡ����ú���*/
    NAS_ERABM_INFO_LOG("NAS_ERABM_RcvRabmEmmRrcConRelInd is entered.");

    NAS_ERABM_TimerStop( NAS_ERABM_WAIT_RB_RESUME_TIMER);

    NAS_ERABM_TimerStop( NAS_ERABM_WAIT_RB_REEST_TIMER);

    NAS_ERABM_SetEmmSrState(NAS_ERABM_SERVICE_STATE_TERMIN);

    /*ֹͣ�����ȴ���ʱ��*/
    NAS_ERABM_TimerStop(NAS_ERABM_WAIT_EPSB_ACT_TIMER);

    /*����ȴ����ؽ�����ʶ */
    NAS_ERABM_SetWaitEpsBActSign(NAS_ERABM_NOT_WAIT_EPSB_ACT_MSG);


    /* ���������ģʽ�� */
    if (NAS_ERABM_MODE_TYPE_NORMAL == NAS_ERABM_GetMode())
    {
        /* ֪ͨL2�ͷŻ�������ݰ� */
        /*APPITF_FreeAllofRabmBuf();*/

        /* ��ϵͳ�л��ɹ���RRC���ϱ���·�ͷ���DRB�����˳�������֪ͨCDS�建�� */
        if (NAS_ERABM_L_MODE_STATUS_NORMAL == NAS_ERABM_GetLModeStatus())
        {
            /* (1)�����ǰ������Pending ��ʶ���������ݻ���,���CDS���ͷŻ��� */
            /* (2)�����Ѽ���DRB,���CDS���ͷŻ��� */
            if((EMM_ERABM_UP_DATA_PENDING == NAS_ERABM_GetUpDataPending())
               || ((NAS_ERABM_FAILURE == NAS_ERABM_IsAllActtiveBearerWithoutDrb())
                    && (NAS_ERABM_NULL != NAS_ERABM_GetActiveEpsBearerNum())))
            {
                /* ֪ͨCDS�ͷŻ�������ݰ� */
                NAS_ERABM_SndErabmCdsFreeBuffDataInd();

                /* �����������������־ */
                NAS_ERABM_SetUpDataPending(EMM_ERABM_NO_UP_DATA_PENDING);
            }
        }

    }
    else  /*����ģʽ�²������棬����ԭ��������Pending״̬*/
    {
        NAS_ERABM_INFO_LOG("NAS_ERABM_RcvRabmEmmRrcConRelInd: Do not release buffer .");
    }

    /* �ͷ�����RB */
    for (ulEpsbId = NAS_ERABM_MIN_EPSB_ID; ulEpsbId<= NAS_ERABM_MAX_EPSB_ID; ulEpsbId++)
    {
        NAS_ERABM_RcvRbRelease(ulEpsbId);
    }

    return;
}
VOS_VOID NAS_ERABM_SndRabmEmmReestReq
(
    VOS_UINT32                          ulIsEmcType
)
{
    EMM_ERABM_REEST_REQ_STRU    *pstReestReq = VOS_NULL_PTR;

    /*����ռ�ͼ���Ƿ����ɹ�*/
    pstReestReq = (VOS_VOID*)NAS_ERABM_ALLOC_MSG(
                                        sizeof(EMM_ERABM_REEST_REQ_STRU));
    if ( VOS_NULL_PTR == pstReestReq )
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_SndRabmEmmReestReq:ERROR:Alloc msg fail!" );
        return;
    }

    /*���*/
    NAS_ERABM_MEM_SET(NAS_ERABM_GET_MSG_ENTITY(pstReestReq), NAS_ERABM_NULL,\
                     NAS_ERABM_GET_MSG_LENGTH(pstReestReq));

    /*��д��Ϣͷ*/
    NAS_ERABM_WRITE_EMM_MSG_HEAD(pstReestReq, ID_EMM_ERABM_REEST_REQ);
	/* lihong00150010 emergency tau&service begin */
    pstReestReq->ulIsEmcType = ulIsEmcType;
	/* lihong00150010 emergency tau&service end */
    /* ������Ϣ���ͺ��� */
    NAS_ERABM_SND_MSG(pstReestReq);

}


VOS_VOID NAS_ERABM_SndRabmEmmDrbSetupInd( VOS_VOID )
{
    EMM_ERABM_DRB_SETUP_IND_STRU    *pstDrbSetInd = VOS_NULL_PTR;

    /*����ռ�ͼ���Ƿ����ɹ�*/
    pstDrbSetInd = (VOS_VOID*)NAS_ERABM_ALLOC_MSG(
                                        sizeof(EMM_ERABM_DRB_SETUP_IND_STRU));
    if ( VOS_NULL_PTR == pstDrbSetInd )
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_SndRabmEmmDrbSetupInd:ERROR:Alloc msg fail!" );
        return;
    }

    /*���*/
    NAS_ERABM_MEM_SET(NAS_ERABM_GET_MSG_ENTITY(pstDrbSetInd), NAS_ERABM_NULL,\
                     NAS_ERABM_GET_MSG_LENGTH(pstDrbSetInd));

    /*��д��Ϣͷ*/
    NAS_ERABM_WRITE_EMM_MSG_HEAD(pstDrbSetInd, ID_EMM_ERABM_DRB_SETUP_IND);

    /* ������Ϣ���ͺ���*/
    NAS_ERABM_SND_MSG(pstDrbSetInd);

}


VOS_VOID NAS_ERABM_SndRabmEmmRelReq( VOS_VOID)
{
    EMM_ERABM_REL_REQ_STRU    *pstRelReq  = VOS_NULL_PTR;

    /*����ռ�ͼ���Ƿ����ɹ�*/
    pstRelReq = (VOS_VOID*)NAS_ERABM_ALLOC_MSG(sizeof(EMM_ERABM_REL_REQ_STRU));
    if ( VOS_NULL_PTR == pstRelReq )
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_SndRabmEmmRelReq:ERROR:Alloc msg fail!" );
        return;
    }

    NAS_ERABM_MEM_SET(NAS_ERABM_GET_MSG_ENTITY(pstRelReq), NAS_ERABM_NULL,\
                     NAS_ERABM_GET_MSG_LENGTH(pstRelReq));

    /*��д��Ϣͷ*/
    NAS_ERABM_WRITE_EMM_MSG_HEAD(pstRelReq, ID_EMM_ERABM_REL_REQ);

    /* ������Ϣ���ͺ���*/
    NAS_ERABM_SND_MSG(pstRelReq);

}

/*****************************************************************************
 Function Name   : NAS_ERABM_SndRabmEmmSuspendRsp
 Description     : ERABMģ��ظ�ID_EMM_ERABM_SUSPEND_RSP��Ϣ
 Input           : EMM_ERABM_RSLT_TYPE_ENUM_UINT32       enRslt
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2011-05-03  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ERABM_SndRabmEmmSuspendRsp
(
    EMM_ERABM_RSLT_TYPE_ENUM_UINT32      enRslt
)
{
    EMM_ERABM_SUSPEND_RSP_STRU          *pstRabmEmmSuspendRsp  = VOS_NULL_PTR;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstRabmEmmSuspendRsp = (VOS_VOID*)NAS_ERABM_ALLOC_MSG(sizeof(EMM_ERABM_SUSPEND_RSP_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pstRabmEmmSuspendRsp)
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ERABM_ERR_LOG("NAS_ERABM_SndRabmEmmSuspendRsp:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    NAS_ERABM_MEM_SET( NAS_ERABM_GET_MSG_ENTITY(pstRabmEmmSuspendRsp), 0, NAS_ERABM_GET_MSG_LENGTH(pstRabmEmmSuspendRsp));

    /*��д��Ϣͷ*/
    NAS_ERABM_WRITE_EMM_MSG_HEAD(pstRabmEmmSuspendRsp, ID_EMM_ERABM_SUSPEND_RSP);

    /*��д��Ӧ���*/
    pstRabmEmmSuspendRsp->enRslt= enRslt;

    /*������Ϣ���ͺ��� */
    NAS_ERABM_SND_MSG(pstRabmEmmSuspendRsp);
}

/*****************************************************************************
 Function Name   : NAS_ERABM_SndRabmEmmResumeRsp
 Description     : ESMģ��ظ�ID_EMM_ERABM_RESUME_RSP��Ϣ
 Input           : EMM_ERABM_RSLT_TYPE_ENUM_UINT32       enRslt
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2011-05-03  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ERABM_SndRabmEmmResumeRsp
(
    EMM_ERABM_RSLT_TYPE_ENUM_UINT32      enRslt
)
{
    EMM_ERABM_RESUME_RSP_STRU            *pstRabmEmmResumeRsp  = VOS_NULL_PTR;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstRabmEmmResumeRsp = (VOS_VOID*)NAS_ERABM_ALLOC_MSG(sizeof(EMM_ERABM_RESUME_RSP_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pstRabmEmmResumeRsp)
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ERABM_ERR_LOG("NAS_ERABM_SndRabmEmmResumeRsp:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    NAS_ERABM_MEM_SET( NAS_ERABM_GET_MSG_ENTITY(pstRabmEmmResumeRsp), 0, NAS_ERABM_GET_MSG_LENGTH(pstRabmEmmResumeRsp));

    /*��д��Ϣͷ*/
    NAS_ERABM_WRITE_EMM_MSG_HEAD(pstRabmEmmResumeRsp, ID_EMM_ERABM_RESUME_RSP);

    /*��д��Ӧ���*/
    pstRabmEmmResumeRsp->enRslt= enRslt;

    /*������Ϣ���ͺ��� */
    NAS_ERABM_SND_MSG(pstRabmEmmResumeRsp);
}
EMM_ERABM_UP_DATA_PENDING_ENUM_UINT32 NAS_ERABM_IsDataPending( VOS_VOID )
{
    /* ����CDS��¼�Ƿ������л������ݣ��Լ�RABM��¼�Ƿ�����������PENDING��ͬ���� */
    return (CDS_IsPsDataAvail()||NAS_ERABM_GetUpDataPending());
}

/*****************************************************************************
 Function Name   : NAS_ERABM_RcvRabmEmmSuspendInd
 Description     : ERABMģ���յ�ID_EMM_ERABM_SUSPEND_IND������
 Input           : EMM_ERABM_SUSPEND_IND_STRU *pRcvMsg
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010            2011-05-03      Draft Enact
*****************************************************************************/
VOS_VOID  NAS_ERABM_RcvRabmEmmSuspendInd(const EMM_ERABM_SUSPEND_IND_STRU *pRcvMsg )
{
    VOS_UINT32                          ulEpsbId        = NAS_ERABM_NULL;
    VOS_UINT8                           ucTimerIndex    = NAS_ERABM_NULL;

    /* ����״̬Ϊ����̬ */
    NAS_ERABM_SetLModeStatus(NAS_ERABM_L_MODE_STATUS_SUSPENDED);

    /* ֹͣ��ʱ�� */
    for (ucTimerIndex = 0; ucTimerIndex < NAS_NAS_ERABM_TIMER_NAME_BUTT; ucTimerIndex++)
    {
        NAS_ERABM_TimerStop(ucTimerIndex);
    }
    /* ������л���CCO���͵Ĺ�����ɾ��DRB��Ϣ��ֻ��DRB��״̬��ΪNAS_ERABM_RB_SUSPENDED��
       ��ΪL-GU�л�����CCOʧ�ܻ��˳ɹ�ʱ��RRC�����ϱ�LRRC_LRABM_RAB_IND��Ϣ����
       ֻ�ϱ�LRRC_LRABM_STATUS_IND��Ϣ�ָ��������������͵Ĺ�����ɾ��DRB��Ϣ*/
    if ((EMM_ERABM_SYS_CHNG_TYPE_HO == pRcvMsg->enSysChngType)
         || (EMM_ERABM_SYS_CHNG_TYPE_CCO == pRcvMsg->enSysChngType))
    {
        /* ��DRB״̬��ΪNAS_ERABM_RB_SUSPENDED */
        for (ulEpsbId = NAS_ERABM_MIN_EPSB_ID; ulEpsbId<= NAS_ERABM_MAX_EPSB_ID; ulEpsbId++)
        {
            NAS_ERABM_SetRbStateInfo(ulEpsbId, NAS_ERABM_RB_SUSPENDED);
        }
    }

    #if 0
    /* �����û���ģʽΪNULL */
    if (PS_SUCC != UP_SetRanMode(RAN_MODE_NULL))
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_RcvRabmEmmSuspendInd: Update Ran Mode Failed.");
    }
    #endif

    /*����ȴ�EPS���ؼ����ʶ*/
    NAS_ERABM_SetWaitEpsBActSign(NAS_ERABM_NOT_WAIT_EPSB_ACT_MSG);

    /*SERVICE����ֹͣ*/
    NAS_ERABM_SetEmmSrState(NAS_ERABM_SERVICE_STATE_TERMIN);

    /* ֪ͨL2�ͷŻ�������ݰ� */
    /*APPITF_FreeAllofRabmBuf();*/

    /* �����������������־*/
    NAS_ERABM_SetUpDataPending(EMM_ERABM_NO_UP_DATA_PENDING);

    /* ֪ͨETC���� */
    NAS_ERABM_SndRabmTcSuspendInd();

    /* �ظ�EMM����ɹ� */
    NAS_ERABM_SndRabmEmmSuspendRsp(EMM_ERABM_RSLT_TYPE_SUCC);
}

/*****************************************************************************
 Function Name   : NAS_ERABM_RcvRabmEmmResumeInd
 Description     : SMģ���յ�ID_EMM_ERABM_RESUME_IND������
 Input           : EMM_ERABM_RESUME_IND_STRU *pRcvMsg
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010            2011-05-03      Draft Enact
*****************************************************************************/
VOS_VOID  NAS_ERABM_RcvRabmEmmResumeInd(const EMM_ERABM_RESUME_IND_STRU *pRcvMsg )
{
    (VOS_VOID)pRcvMsg;

    /* ����״̬Ϊ����̬ */
    NAS_ERABM_SetLModeStatus(NAS_ERABM_L_MODE_STATUS_NORMAL);

    #if 0
    /* �����û���ģʽΪNULL */
    if (PS_SUCC != UP_SetRanMode(RAN_MODE_LTE))
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_RcvRabmEmmResumeInd: Update Ran Mode Failed.");
    }

    /*����LTEģ���ز���*/
    R_ITF_SetLFlowLev();
    #endif

    /*����TFT�ͳ�����Ϣ����IP Filter��Ӳ��������*/
    if(PS_FAIL == NAS_ERABM_IpfConfigUlFilter())
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_EsmMsgDistr: IPF Update IP Filter Failed.");
    }

    /*����TFT�ͳ�����Ϣ����IP Filter��CDS*/
    if(PS_FAIL == NAS_ERABM_CdsConfigUlFilter())
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_EsmMsgDistr: CDS Update IP Filter Failed.");
    }
    #if 0
    /* ��������TFT��Ϣ */
    if(PS_FAIL == NAS_ERABM_IpfConfigDlFilter())
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_RcvRabmEmmResumeInd: Update DL IP Filter Failed.");
    }
    #endif

    /* ֪ͨETC��� */
    NAS_ERABM_SndRabmTcResumeInd();

    /* �ظ�EMM��ҳɹ� */
    NAS_ERABM_SndRabmEmmResumeRsp(EMM_ERABM_RSLT_TYPE_SUCC);
}
/*lint +e961*/
/*lint +e960*/

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

