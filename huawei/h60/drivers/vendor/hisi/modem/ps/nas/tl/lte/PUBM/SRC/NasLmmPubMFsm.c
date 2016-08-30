


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include    "NasLmmPubMInclude.h"
#include    "NasLmmPubMPrint.h"
#include    "NasLmmPubMTimer.h"
#include    "Msp_diag_cmd_id.h"
#include    "NasEmmSmsInterface.h"
#include    "NasLmmCore.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASMMPUBMFSM_C
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
NAS_LMM_MAIN_CONTEXT_STRU                g_stMmMainContext;
NAS_LMM_FSM_DESC_STRU                    g_astNasMmFsmDesc[NAS_LMM_PARALLEL_FSM_BUTT];
VOS_UINT32                               g_ulMmcSerRegFlag;

/*****************************************************************************
  3 Function
*****************************************************************************/



/*lint -e960*/
/*lint -e961*/
VOS_UINT32 NAS_LMM_FsmRegisterFsm(       NAS_LMM_PARALLEL_FSM_ENUM_UINT16 enParaFsmId,
                                        VOS_UINT32                      ulSize,
                                        NAS_LMM_STA_STRU                *pStaTable,
                                        NAS_LMM_EXCEP_FUN                pfExceptHandle )
{
    NAS_LMM_FSM_DESC_STRU               *pFsmDesc;

    /* �����ڲ��� */
    if ( ( NAS_LMM_PARALLEL_FSM_BUTT <= enParaFsmId ) || ( NAS_LMM_NULL_PTR == pStaTable ) )
    {
        NAS_LMM_PUBM_LOG2_ERR("NAS_LMM_FsmRegisterFsm, The input is illegal! enParaFsmId, pStaTable",
                            enParaFsmId,
                            pStaTable);
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /* ָ��ָ���Ĳ���״̬����״̬�������� */
    pFsmDesc                            = &g_astNasMmFsmDesc[enParaFsmId];


    /* ��״̬���������ṹ���и�ֵ */
    pFsmDesc->ulSize                    = ulSize;
    pFsmDesc->pStaTable                 = pStaTable;
    pFsmDesc->pfExceptHandle            = pfExceptHandle;
    pFsmDesc->aulMmParaFsmId            = enParaFsmId;


    /* ��״̬ת�Ʊ��������, UE 1.0 ����������*/
    /*
    NAS_LMM_FSM_StaSort(pStaTable, ulSize);
    */
    return NAS_LMM_SUCC;

}




VOS_UINT32 NAS_LMM_FsmProcessEvent(      NAS_LMM_PARALLEL_FSM_ENUM_UINT16 enParalFsmId,
                                        VOS_UINT32                      ulEventType,
                                        VOS_UINT32                      ulMsgID,
                                        VOS_UINT8                      *pRcvMsg )
{
    NAS_LMM_ACTION_FUN                   pActFun;
    NAS_LMM_FSM_DESC_STRU               *pFsmDesc;
    VOS_UINT32                          ulCurState;
    NAS_LMM_FSM_STATE_STRU              *pstFsmCtx  = NAS_LMM_NULL_PTR;




    /* �ҵ�ĳ����״̬�����������ַ */
    pFsmDesc = &(g_astNasMmFsmDesc[enParalFsmId]);

    /* ��ȡ״̬����ַ,����ȡ״̬ */
    pstFsmCtx = NAS_LMM_GetCurFsmAddr(enParalFsmId);

    /* ����ĳ����״̬���ĵ�ǰ״̬ */
    ulCurState = NAS_LMM_PUB_COMP_EMMSTATE(pstFsmCtx->enMainState,
                                          pstFsmCtx->enSubState);


    /* ����״̬ת�Ʊ��е���Ӧ�Ķ��������� */
    pActFun = NAS_LMM_FsmFindAct(pFsmDesc, ulCurState, ulEventType);
    if (NAS_LMM_NULL_PTR != pActFun)
    {
        return (*pActFun) (ulMsgID, pRcvMsg);
    }
    /* ����Ҳ�����������������ʾ�쳣��������ʾ����Ϣû�б���ǰ״̬������
        ����������£���Ҫ���ø�״̬�����쳣��������������Ϣ�ܹ����쳣�������д��� */
    else
    {
        /* �쳣�������� */
        if (NAS_LMM_NULL_PTR != pFsmDesc->pfExceptHandle)
        {

            return (*pFsmDesc->pfExceptHandle) (ulEventType, pRcvMsg);
        }
        /* ���û���쳣����������ʾ����Ϣû�б���״̬������ */
        else
        {
            return NAS_LMM_MSG_DISCARD;
        }
    }
}




/*****************************************************************************
 Function Name  : NAS_LMM_FsmFindAct
 Discription    : ����״̬���¼�������������״̬ת�Ʊ��п��ٲ����¼�������
 Input          : ״̬�������� ״̬ �¼�����
 Output         : None
 Return         : �¼�������
 History:
      1.  Wang Yangcai 49178  2008-02-19 ��ֲ����

*****************************************************************************/
NAS_LMM_ACTION_FUN   NAS_LMM_FsmFindAct(  const NAS_LMM_FSM_DESC_STRU   *pFsmDesc,
                                        VOS_UINT32              ulState,
                                        VOS_UINT32              ulEventType)
{
    VOS_UINT32                          ulActTblLoc;
    VOS_UINT32                          ulMmStaTblLoc;
    NAS_LMM_ACT_STRU                    *pMmActStru;

    (VOS_VOID)(ulState);
    /* ����ĳ����״̬���ĵ�ǰ״̬(��״̬����״̬�����״̬),�ڸ�״̬����״̬
    ת�Ʊ��в���״̬�¼������, ���Ϊ״̬�¼��������״̬ת�Ʊ��е����� */
    NAS_LMM_FsmGetEvtTab(pFsmDesc, &ulMmStaTblLoc);
    if ( NAS_LMM_SEA_TABLE_ERR_LOC == ulMmStaTblLoc)
    {   /* ��û���ҵ��¼������, �����쳣������ */

        /* Ŀǰ���쳣������Ǵ�ӡ */
        NAS_LMM_PUBM_LOG_ERR(            "NAS_LMM_FsmFindAct: In ulMmStaTblLoc not fine correct StaTable.");
        return  VOS_NULL_PTR;
    }
    else
    {
        /*
        NAS_LMM_PUBM_LOG2_INFO(          "NAS_LMM_FsmFindAct: aulMmParaFsmId = , ulMmStaTblLoc =  ",
                                        pFsmDesc->aulMmParaFsmId,
                                        ulMmStaTblLoc);
        */
    }



    /* �ҵ�ĳ����״̬����״̬�¼������ */
    pMmActStru = pFsmDesc->pStaTable[ulMmStaTblLoc].pActTable;


    /* �����յ����¼�����, ��״̬�¼�������в����¼����͵Ĵ�����  */
    NAS_LMM_FsmGetEvtStatFun(            pFsmDesc,
                                        ulMmStaTblLoc,
                                        ulEventType,
                                        &ulActTblLoc);
    if ( NAS_LMM_SEA_TABLE_ERR_LOC       == ulActTblLoc)
    {   /* ��û���ҵ��¼�����, �����쳣������ */

        /* Ŀǰ���쳣������Ǵ�ӡ */
        return  VOS_NULL_PTR;
    }
    else
    {
        /*
        NAS_LMM_PUBM_LOG2_INFO(          "NAS_LMM_FsmFindAct: aulMmParaFsmId = , ulActTblLoc =  ",
                                        pFsmDesc->aulMmParaFsmId,
                                        ulActTblLoc);
       */
    }

    /* ʹ���ҵ��ĺ��������յ����¼� */
    return  pMmActStru[ulActTblLoc].pfActionFun;
}


/*****************************************************************************
 Function Name  : NAS_LMM_FsmGetEvtTab
 Discription    : ���ݵ�ǰ״̬����״̬ת�Ʊ�g_astEmmStaTbl�в����¼������
                  ��������¼��������g_astEmmStaTbl�е���������ֵ
 Input          : None
 Output         : pActTable
 Return         : None
 History:
      1.  ��³�� 41410  yyyy-mm-dd  Draft Enact
*****************************************************************************/
VOS_VOID    NAS_LMM_FsmGetEvtTab(       const NAS_LMM_FSM_DESC_STRU   *pFsmDesc,
                                        VOS_UINT32             *pulStaTblLoc)
{

    NAS_LMM_STA_STRU*                    pStaTable;                /* ״̬ת�Ʊ�Ļ���ַ     */
    VOS_UINT32                          ulMmStaTableSize;         /* ״̬ת�Ʊ�Ĵ�С       */
    VOS_UINT32                          ulCurState;
    NAS_LMM_FSM_STATE_STRU              *pstFsmCtx  = NAS_LMM_NULL_PTR;


    VOS_UINT32                          i;
    VOS_UINT32                          ulMaxSeaCycle;


    pStaTable                           = pFsmDesc->pStaTable;
    ulMmStaTableSize                    = pFsmDesc->ulSize;



    /* ��ȡ״̬����ַ,����ȡ״̬ */
    pstFsmCtx = NAS_LMM_GetCurFsmAddr(pFsmDesc->aulMmParaFsmId);

    /* ����ĳ����״̬���ĵ�ǰ״̬ */
    ulCurState = NAS_LMM_PUB_COMP_EMMSTATE(pstFsmCtx->enMainState, pstFsmCtx->enSubState);


    /* ��ȡ��ĳ����״̬����״̬ת�Ʊ��е�������ѭ������ */
    ulMaxSeaCycle = ulMmStaTableSize;

    /* ����ĳ����״̬����״̬ת�Ʊ��в��Ҷ�Ӧ��״̬����� */
    for(i = 0; i < ulMaxSeaCycle; i++)
    {
        /* ��״̬��ͬ,���ҵ���,�������������ǰ�ڱ��е�����,�˳�ѭ�� */
        if((pStaTable + i)->ulState     == ulCurState)
        {
            *pulStaTblLoc               = i;
            break;
        }
    }

    /* ��û���ҵ�ƥ���״̬, ���������������ֵ */
    if(i >= ulMaxSeaCycle)
    {
        *pulStaTblLoc                   = NAS_LMM_SEA_TABLE_ERR_LOC;
        NAS_LMM_PUBM_LOG2_ERR(           "NAS_LMM_GetEvtTab: Not find correct STATE. aulMmParaFsmId = , ulCurState =",
                                        pFsmDesc->aulMmParaFsmId,
                                        ulCurState);
    }

    return;
}


/*****************************************************************************
 Function Name  : NAS_LMM_FsmGetEvtStatFun
 Discription    : ��ĳ״̬���¼�������¼��������,���Ҵ���������Ϣ�ĺ���,�����
 Input          : ulEmmStaTblLoc,
                  pRcvMsg
 Output         : pulEmmActTblLoc
 Return         : None
 History:
      1.  ��³�� 41410  yyyy-mm-dd  Draft Enact
*****************************************************************************/
VOS_VOID    NAS_LMM_FsmGetEvtStatFun(    const NAS_LMM_FSM_DESC_STRU   *pFsmDesc,
                                        VOS_UINT32              ulFsmStaTblLoc,
                                        VOS_UINT32              ulEventType,
                                        VOS_UINT32             *pulEmmActTblLoc)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulMaxSeaCycle;
    NAS_LMM_ACT_STRU                    *pMmActStru;


    /* ��״̬ת�Ʊ����ҵ�ĳ״̬���¼������ */
    pMmActStru = pFsmDesc->pStaTable[ulFsmStaTblLoc].pActTable;

    /* ��g_astEmmStaTbl[ulEmmStaTblLoc].pActTable�е�������ѭ������ *//*HLF*/
    ulMaxSeaCycle = pFsmDesc->pStaTable[ulFsmStaTblLoc].ulSize;

    /* ��g_astEmmStaTbl[ulEmmStaTblLoc].pActTable�в��Ҷ�Ӧ�¼����͵����� */
    for(i = 0; i < ulMaxSeaCycle; i++)
    {
        /* ���¼�������ͬ,���ҵ���,�������������ǰ�ڱ��е�����,�˳�ѭ�� */
        if(ulEventType == (pMmActStru + i)->ulEventType)
        {
            *pulEmmActTblLoc            = i;
            break;
        }
    }

    /* ��û���ҵ�ƥ���״̬, ���������������ֵ */
    if(i >= ulMaxSeaCycle)
    {
        *pulEmmActTblLoc                = NAS_LMM_SEA_TABLE_ERR_LOC;
      /*  NAS_LMM_PUBM_LOG2_ERR("NAS_LMM_FsmGetEvtStatFun: Not find correct FUNC. aulMmParaFsmId = , EVENT TYPE= ",
                                        pFsmDesc->aulMmParaFsmId,
                                        ulEventType);
      */
    }

    return;
}

/* lihong00150010 emergency tau&service begin */

VOS_VOID  NAS_LMM_EnterDeregStateModify
(
    NAS_LMM_FSM_STATE_STRU              *pstDestState
)
{
    NAS_MM_NETWORK_ID_STRU             *pstPresentNwInfo = NAS_EMM_NULL_PTR;

    /* ����޿����߿���Ч�������DEREG+NO_IMSI̬ */
    if ((NAS_LMM_SIM_STATUS_UNAVAILABLE == NAS_LMM_GetSimState())
        || (NAS_LMM_SIM_INVALID == NAS_LMM_GetPsSimValidity()))
    {
        pstDestState->enSubState = EMM_SS_DEREG_NO_IMSI;
        return ;
    }

    /* ���Ŀ��״̬ΪDEREG+NORMAL_SERVICE̬���ҵ�ǰС������������ANYCELLС����
       �����DEREREG+LIMIT_SERVICE */
    pstPresentNwInfo = (VOS_VOID *)NAS_EMM_GetEmmGlobleAddr(NAS_LMM_GLOBLE_PRESENT_NET_ID);
    if ((EMM_SS_DEREG_NORMAL_SERVICE == pstDestState->enSubState)
        && ((EMMC_EMM_NO_FORBIDDEN != pstPresentNwInfo->ulForbiddenInfo)
            || (EMMC_EMM_CELL_STATUS_ANYCELL == pstPresentNwInfo->ulCellStatus)))
    {
        pstDestState->enSubState = EMM_SS_DEREG_LIMITED_SERVICE;
        return ;
    }

    return ;
}
/* lihong00150010 emergency tau&service end */

VOS_VOID  NAS_LMM_StaTransProc( NAS_LMM_FSM_STATE_STRU  stDestState )
{

    NAS_LMM_FSM_STATE_STRU *             pstCurState;

    NAS_LMM_PUBM_LOG_INFO("NAS_LMM_StaTransProc enter!");

    /* ��μ�� */
    if ( NAS_LMM_PARALLEL_FSM_BUTT <= stDestState.enFsmId )
    {
        NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_StaTransProc, The input is illegal! enParaFsmId",
                             stDestState.enFsmId);
        return;
    }

    /* ��ȡĳ����״̬����ǰ״̬�ĵ�ַ */
    pstCurState                         = NAS_LMM_GetCurFsmAddr(stDestState.enFsmId);

    /* �����ǰ״̬���ת��Ϊ��״̬��ͬ,�򲻽����޸� */
    if( (pstCurState->enMainState       == stDestState.enMainState) &&
        (pstCurState->enSubState        == stDestState.enSubState)  &&
        (pstCurState->enStaTId          == stDestState.enStaTId))
    {
        NAS_LMM_PUBM_LOG_WARN("NAS_LMM_StaTransProc: EMM STATE REMAINS. ");
        return;
    }

    /*ִ��״̬Ǩ������Ĵ���*/
    NAS_LMM_ActionBeforeTransToDestSta(stDestState);
	/* lihong00150010 emergency tau&service begin */
    if (EMM_MS_DEREG == stDestState.enMainState)
    {
        NAS_LMM_EnterDeregStateModify(&stDestState);
    }
	/* lihong00150010 emergency tau&service end */
    /*״̬��Ǩ��*/
    NAS_LMM_FsmTranState(stDestState);

    return;
}


/*****************************************************************************
 Function Name  :NAS_LMM_FsmTranState
 Discription    :
 Input          :stDestState:Ŀ��״̬��UE 1.0��д����4����Ա:
                                            enFsmId
                                            enMainState
                                            enSubState;
                                            enStaTId;
 Output         : None
 Return         : None
 History:
      1.  Name+ID  yyyy-mm-dd  Draft Enact
*****************************************************************************/
VOS_VOID  NAS_LMM_FsmTranState(          NAS_LMM_FSM_STATE_STRU  stDestState)
{
    NAS_LMM_FSM_STATE_STRU *             pstCurState;

    /* ��ȡĳ����״̬����ǰ״̬�ĵ�ַ */
    pstCurState                         = NAS_LMM_GetCurFsmAddr(stDestState.enFsmId);

    /* ��ӡ��ǰ״̬�Ͷ�ʱ�� */
    NAS_LMM_PUBM_LOG_NORM("======================================================================================");
    if(NAS_LMM_PARALLEL_FSM_EMM          == stDestState.enFsmId)
    {
        NAS_LMM_PUBM_LOG_NORM("======NAS_EMM Change Statate To:======");
    }
    #if 0
    else if(NAS_LMM_PARALLEL_FSM_MMC     == stDestState.enFsmId)
    {
        NAS_LMM_PUBM_LOG_NORM("======NAS_LMMC Change Statate To:======");
    }
    #endif
    else
    {
        NAS_LMM_PUBM_LOG_NORM("======NAS_SECU Change Statate To:======");
    }

    /* MM��ĳ����״̬����״̬ת�� */
    pstCurState->enMainState            = stDestState.enMainState;
    pstCurState->enSubState             = stDestState.enSubState;
    pstCurState->enStaTId               = stDestState.enStaTId;

    /* ��ӡת�����״̬�Ͷ�ʱ�� */
    NAS_LMM_PrintFsmState(pstCurState->enFsmId);

    NAS_LMM_PUBM_LOG_NORM("======================================================================================");
    return;
}

/*****************************************************************************
 Function Name   : NAS_EMM_ProcRrcOriResumeMsgMmcSuspendReq
 Description     : RESUME+RRCORI_WAIT_SYS_INFO״̬,���Ҫת��Dereg״̬ʱ������
                                 �Ƿ�Ӧ�ø��°�ȫ�����Ľ����ж�
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.FTY����      2012-04-26  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_EMM_ProcRrcOriResumeDestStateDeregWriteEpsSecuContext(VOS_VOID)
{
    NAS_EMM_PUBU_LOG1_INFO(" NAS_EMM_ProcRrcOriResumeDestStateDeregWriteEpsSecuContext:NAS_EMM_GetResumeType()",
                            NAS_EMM_GetResumeType());

    /*RESUME+RRCORI_WAIT_SYS_INFO��RESUME+RRCRSM_WAIT_OTHER_RESUME״̬���յ�MMC������Ϣ�Ĵ���*/
    switch (NAS_EMM_GetResumeType())
    {
        case NAS_LMM_SYS_CHNG_TYPE_REVERSE_RSL:
        case NAS_LMM_SYS_CHNG_TYPE_REVERSE_HO:
        case NAS_LMM_SYS_CHNG_TYPE_REVERSE_CCO:
        case NAS_LMM_SYS_CHNG_TYPE_REVERSE_REDIR:
            if(EMM_MS_DEREG != NAS_EMM_GetMsBefSuspend())
            {
                NAS_LMM_WriteEpsSecuContext(NAS_NV_ITEM_UPDATE);
            }
            break;

        default:
            if(EMM_MS_DEREG != NAS_EMM_GetMsBefResume())
            {
                NAS_LMM_WriteEpsSecuContext(NAS_NV_ITEM_UPDATE);
            }
            break;
    }
}
VOS_VOID  NAS_LMM_ClearEmmPara( VOS_VOID )
{
    /*���� EPS update statusΪEU3*/
    NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_NOT_UPDATED_EU2;

    /*ɾ��GUTI*/
    NAS_EMM_ClearGuti();

    /*ɾ��LVR TAI*/
    NAS_EMM_ClearLVRTai();

    /*ɾ��KSIasme*/
    NAS_EMM_ClearAllSecuCntxt();

    /* ɾ��TINֵ */
    NAS_EMM_SetTinType(MMC_LMM_TIN_INVALID);
}
VOS_VOID  NAS_LMM_ActionBeforeTransToDestSta( NAS_LMM_FSM_STATE_STRU  stDestState )
{

    NAS_LMM_FSM_STATE_STRU *             pstCurState;


    NAS_LMM_PUBM_LOG_INFO("NAS_LMM_ActionBeforeTransToDestSta enter!");

    /*�ж������ϱ���APP��״̬�Ƿ�ı䣬����ı�������ϱ�APP_MM_REG_STAT_IND��Ϣ*/
    NAS_LMM_AppStateChange(stDestState);

    /* Lģ�����������ϱ� */

    /*���ڷ���״̬���ϱ�:��ʹ�ڹ���ҲҪ��ʱά������ӳ��ķ���״̬,ע��״̬
      ���ϱ�״̬ǰ��ȥ�жϵ�ǰ�Ƿ�Ϊ��ģ*/
    /*if ( (EMM_MS_NULL == stDestState.enMainState)
       ||( (NAS_LMM_CUR_LTE_ACTIVE == NAS_EMM_GetCurLteState())
         &&(EMM_MS_NULL != stDestState.enMainState)))
    {*/
    /*�ж������ϱ���MMC�ķ���״̬�Ƿ�ı䣬����ı�������ϱ�ID_LMM_MMC_SERVICE_STATUS_IND��Ϣ*/
    NAS_LMM_MmcServiceStateChange(stDestState);

    /*�ж������ϱ���MMC��ע��״̬�Ƿ�ı䣬����ı�������ϱ�ID_LMM_MMC_REGISTER_STATUS_IND��Ϣ*/
    NAS_LMM_MmcRegisterStateChange(stDestState);
    /*}*/

    /*���Ŀ��״̬��Dereg:
      ɾ�� new mapped security context
      ɾ�� new native security context
      ɾ�� current mapped security context
      �������non-current native secu cntxt���仹ԭΪcurrent native secu cntxt
      ���EMM����ά���ĳ���������*/
    if(EMM_MS_DEREG == stDestState.enMainState)
    {
        /* ����DEREG̬�����UE���������仯��ʶ */
        NAS_EMM_ClearUeRadioAccCapChgFlag();

        /*leili modify for isr begin*/
        /*ֹͣ��ʱ��*/
        NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_T3412);

        /*֪ͨGUģT3412ֹͣ*/
        NAS_EMM_SendTimerStateNotify(TI_NAS_EMM_PTL_T3412, LMM_MMC_TIMER_STOP);

        NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_T3423);

        /*֪ͨGUģT3423ֹͣ*/
        NAS_EMM_SendTimerStateNotify(TI_NAS_EMM_PTL_T3423, LMM_MMC_TIMER_STOP);

        /*leili modify for isr end*/
        /*��ȫ������*/
        NAS_EMM_SecuDeregClrSecuCntxt();


        /*leili modify for isr begin*/
        /*���EMMά���ĳ���������*/
        NAS_EMM_PubmInfoBearerContextInit();
#if 0
        NAS_LMM_MEM_SET(                NAS_EMM_GetEpsBearerContextAddr(),
                                        0,
                                        sizeof(NAS_LMM_EPS_BEARER_CONTEXT_STRU));
#endif

        if (NAS_LMM_REG_STATUS_EMC_REGED == NAS_LMM_GetEmmInfoRegStatus())
        {
            NAS_LMM_ClearEmmPara();
        }

        NAS_LMM_SetEmmInfoRegStatus(NAS_LMM_REG_STATUS_DEREG);


        /*leili modify for isr end*/
        NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_NULL);

        /* ���GU�����LAU��������RAU��ʶ����Ϊ����DEREG̬�������ATTACH���̣�������TAU���� */
        NAS_LMM_SetEmmInfoLauOrComRauFlag(NAS_EMM_LAU_OR_COMBINED_RAU_NOT_HAPPENED);
        NAS_LMM_SetEmmInfoSrvccFlag(NAS_EMM_SRVCC_NOT_HAPPENED);
        /* �����һ��TAU��ʶ����Ϊ����DEREG̬�������ATTACH���̣�������TAU���� */
        NAS_LMM_SetEmmInfoFirstTauFlag(NAS_EMM_NOT_FIRST_TAU);

        /* �������DETACH��TAU��ϱ�ʶ */
        NAS_EMM_TAU_SetEmmCombinedDetachBlockFlag(NAS_EMM_COM_DET_BLO_NO);
        /* ���UPDATE_MM��ʶ */
        /*NAS_LMM_SetEmmInfoUpdateMmFlag(NAS_EMM_UPDATE_MM_FLAG_INVALID);*/

        /*���T3412��ʱ��־��DrxNetCapability��־*/
        NAS_LMM_SetEmmInfoT3412ExpCtrl(    NAS_EMM_T3412_EXP_NO);
        NAS_LMM_SetEmmInfoDrxNetCapChange(  NAS_EMM_NO);
        /*���RRCɾ��ԭ���ʶ*/
        NAS_LMM_SetEmmInfoTriggerTauRrcRel(NAS_EMM_TRIGGER_TAU_RRC_REL_NO);
        /*��� EPS ���������ı����־*/
        NAS_EMM_SetEpsContextStatusChange(NAS_EMM_EPS_BEARER_STATUS_NOCHANGE);

        NAS_LMM_SetEmmInfoTriggerTauSysChange(NAS_EMM_NO);
        NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3442);

        NAS_EMM_SER_SaveEmmSerCsfbAbortFlag(NAS_EMM_CSFB_ABORT_FLAG_INVALID);

        NAS_LMM_SetEmmInfoPsState(GMM_LMM_GPRS_NOT_SUSPENSION);

        NAS_EMM_ClrAllUlDataReqBufferMsgExceptMtDetachAcc();

        return ;
    }

    /*��Ŀ��״̬��EMM_MS_REG�����ҵ�ǰ״̬��EMM_MS_DEREG��
    ɾ��������NV�б���İ�ȫ������*/
    /* 33401 CR457������UE������ȥע��̬תȥע��̬ʱ����Ҫ��SIM������NV�еİ�ȫ
    ����������Ϊ��Ч*/
    pstCurState = NAS_LMM_GetEmmCurFsmAddr() ;
    if((EMM_MS_REG == stDestState.enMainState)
        &&(EMM_MS_DEREG == pstCurState->enMainState))
    {
        NAS_LMM_WriteEpsSecuContext(NAS_NV_ITEM_DELETE);
    }

    #if 0
   /* ����Ҫת��REG+NORMAL_SERVICE̬ʱ�����UPDATE_MM��ʶ��Ч������Ҫ�����ڲ�
       TAU��Ϣ������ΪNAS_LMM_INTRA_TAU_TYPE_UPDATE_MM����Ԥ�������������͵�
       �ڲ���Ϣ����״̬ת��REG+ATTEMP_TO_UPDATE_MM״̬ */
    if ((EMM_MS_REG == stDestState.enMainState)
         && (EMM_SS_REG_NORMAL_SERVICE == stDestState.enSubState)
         && (NAS_EMM_YES == NAS_EMM_TAU_CanTriggerComTauWithIMSI()))
    {
        NAS_EMM_SendIntraTauReq(    ID_APP_MM_SET_DRX_REQ,
                                    NAS_LMM_INTRA_TAU_TYPE_UPDATE_MM);
    }
    #endif
    return;
}



VOS_UINT32  NAS_LMM_MsgProcessInFsm(     NAS_LMM_PARALLEL_FSM_ENUM_UINT16  enParalFsmId,
                                        MsgBlock                        *pMsg,
                                        VOS_UINT32                       ulMsgId,
                                        VOS_UINT32                       ulEventType )
{
    VOS_UINT32                          ulProcessFlag; /* ��Ǹ���Ϣ�Ƿ��� */

    /* ״̬����ַԽ�� */
    if ( enParalFsmId >= NAS_LMM_PARALLEL_FSM_BUTT )
    {
        /* ��ӡ������Ϣ */
        NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_MsgProcessInFsm, FsmId Error, enParalFsm",
                                        enParalFsmId);

        return  NAS_LMM_MSG_DISCARD;
    }

    /* ״̬������Ϣ�Ĵ��� */
    ulProcessFlag = NAS_LMM_FsmProcessEvent(     enParalFsmId,
                                                ulEventType,
                                                ulMsgId,
                                                (VOS_UINT8 *)pMsg );
    return ulProcessFlag;
}



NAS_LMM_COMM_ACTION_FUN  NAS_LMM_CommFindFun(
                                        const NAS_LMM_COMM_ACT_STRU   *pastMap,
                                        VOS_UINT32              ulMapNum,
                                        VOS_UINT32              ulMsgId,
                                        VOS_UINT32              ulSenderPid)
{
    VOS_UINT32                          i;
    NAS_LMM_COMM_ACTION_FUN              pActFun = NAS_LMM_NULL_PTR;

    #if 0
    /* ��Map�в��Ҷ�Ӧ�� TYPE*/
    for(i = 0; i < ulMapNum; i++)
    {
        /* ��������ͬ,���ҵ���,���ص�ǰ�Ķ����������˳�ѭ�� */
        if(ulMsgId == pastMap[i].ulMsgId)
        {
            pActFun            = pastMap[i].pfFun;
            break;
        }
    }
    #endif
    /* ��Map�в��Ҷ�Ӧ�� TYPE*/
    for(i = 0; i < ulMapNum; i++)
    {
        /* ��������ͬ,���ҵ���,���ص�ǰ�Ķ����������˳�ѭ�� */
        if((ulMsgId == pastMap[i].ulMsgId)
            && (ulSenderPid == pastMap[i].ulSenderPid))
        {
            pActFun            = pastMap[i].pfFun;
            break;
        }
    }

#if 0
    if (NAS_LMM_NULL_PTR == pActFun)
    {
        /* ����Ҳ�������������ʾ��ǰû�иô����� */
        NAS_LMM_PUBM_LOG_INFO("NAS_LMM_CommFindFun: Not find action functin");
    }
#endif

    return  pActFun;
}

/*****************************************************************************
 Function Name   : NAS_LMM_CommFindFun
 Description     : ��pMapָ��Ľṹ�в���ulMsgId��Ӧ�Ĳ�������;
                   pMapָ��Ľṹ�й���ulMapNum��;
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.wangchen    2012-05-31
*****************************************************************************/
NAS_EMMC_COMM_ACTION_FUN  NAS_EMMC_CommFindFun(
                                        const NAS_EMMC_COMM_ACT_STRU   *pastMap,
                                        VOS_UINT32              ulMapNum,
                                        VOS_UINT32              ulMsgId,
                                        VOS_UINT32              ulSenderPid)
{
    VOS_UINT32                          i;
    NAS_EMMC_COMM_ACTION_FUN              pActFun = NAS_LMM_NULL_PTR;

    /* ��Map�в��Ҷ�Ӧ�� TYPE*/
    for(i = 0; i < ulMapNum; i++)
    {
        /* ��������ͬ,���ҵ���,���ص�ǰ�Ķ����������˳�ѭ�� */
        if((ulMsgId == pastMap[i].ulMsgId)
            && (ulSenderPid == pastMap[i].ulSenderPid))
        {
            pActFun            = pastMap[i].pfFun;
            break;
        }
    }

#if 0
    if (NAS_LMM_NULL_PTR == pActFun)
    {
        /* ����Ҳ�������������ʾ��ǰû�иô����� */
        NAS_LMM_PUBM_LOG_INFO("NAS_LMM_CommFindFun: Not find action functin");
    }
#endif

    return  pActFun;
}
VOS_UINT32  NAS_LMM_GetServiceDomain(NAS_LMM_MAIN_STATE_ENUM_UINT16  enMainState)
{
    VOS_UINT32                           ulSrvDomain = APP_MM_SERVICE_DOMAIN_NULL;

    switch( enMainState )
    {
        case      EMM_MS_REG:
        case      EMM_MS_TAU_INIT:
        case      EMM_MS_SER_INIT:
                /* ��ע��̬�£�����PS */
                ulSrvDomain = APP_MM_SERVICE_DOMAIN_PS;
                break;

        default:
                /*��ע��̬�·���NULL*/
                ulSrvDomain = APP_MM_SERVICE_DOMAIN_NULL;
                break;
    }

    return ulSrvDomain;
}




VOS_VOID  NAS_LMM_AppStateChange( NAS_EMM_FSM_STATE_STRU  stEmmDestState)
{
    NAS_APP_REG_STAT_ENUM_UINT32       ulNasAppDstState;

    /* ��μ�� */
    if ( NAS_LMM_PARALLEL_FSM_EMM != stEmmDestState.enFsmId )
    {
       /* NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_AppStateChange, The input is illegal! enParaFsmId",
                             stEmmDestState.enFsmId);*/
        return;
    }

    ulNasAppDstState = NAS_LMM_GetNasAppRegState();

    /* ����EMM��Ŀ��״̬������Ŀ��APPע��״̬ */
    NAS_LMM_StateMap(&stEmmDestState, &ulNasAppDstState);

    NAS_LMM_PUBM_LOG1_NORM("NAS_LMM_AppStateChange NAS_LMM_GetNasAppRegState()=",NAS_LMM_GetNasAppRegState());

    NAS_LMM_PUBM_LOG1_NORM("NAS_LMM_AppStateChange ulNasAppDstState=",ulNasAppDstState);



    /* ��APPע��״̬�����仯�����޸ĸ�״̬�����ϱ���APP */
    if(NAS_LMM_GetNasAppRegState() != ulNasAppDstState)
    {
        /*��AT����APP_MM_STAT_IND��Ϣ*/
        NAS_EMM_AppRegStatInd(ulNasAppDstState,stEmmDestState.enMainState);

        NAS_LMM_SetNasAppRegState(ulNasAppDstState);

    }
    return;
}

/*****************************************************************************
 Function Name   : NAS_LMM_IsRegisteredInHplmn
 Description     : �ж��Ƿ�ע����HPLMN��
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2012-07-10  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_LMM_IsRegisteredInHplmn()
{
    NAS_MM_PLMN_LIST_STRU               stEHplmnList    = {0};
    VOS_UINT32                          ulRslt          = MMC_LMM_FAIL;
    MMC_LMM_PUB_INFO_STRU               stPubInfo;
    NAS_EMM_PLMN_ID_STRU               *pstCurentPlmn   = VOS_NULL_PTR;

    pstCurentPlmn = NAS_LMM_GetEmmInfoPresentPlmnAddr();

    NAS_LMM_MEM_SET(&stPubInfo,0,sizeof(MMC_LMM_PUB_INFO_STRU));
    /*PC REPLAY MODIFY BY LEILI BEGIN*/
    /* ���EHPLMN��ȡ�ɹ����Ҹ�����Ϊ0�����жϵ�ǰPLMN�Ƿ���EHPLMN�б��У����
       ���ڣ�����ע����HPLMN�ϣ��������ע�ᵽRPLMN�� */
    ulRslt = NAS_EMM_GetGulPubInfo(MMC_LMM_EHPLMN, &stPubInfo);
    /*PC REPLAY MODIFY BY LEILI END*/
    if ((MMC_LMM_SUCC == ulRslt) && (stPubInfo.u.stEHplmnList.ulPlmnNum > 0))
    {
        NAS_LMM_PUBM_LOG1_NORM("NAS_LMM_StateMap HPLMN",stEHplmnList.ulPlmnNum);

        NAS_LMM_MEM_CPY (   &stEHplmnList,
                            &stPubInfo.u.stEHplmnList,
                            sizeof(MMC_LMM_PLMN_LIST_STRU));

        if (NAS_LMM_MATCH_SUCCESS == NAS_LMM_PlmnIDMatchHplmnList(pstCurentPlmn, &stEHplmnList))
        {
            return NAS_EMM_YES;
        }

        return NAS_EMM_NO;
    }

    /* ���EHPLMN��Ч�����жϵ�ǰPLMN��HPLMN�Ƿ���ͬ�������ͬ������ע����HPLMN
       �ϣ��������ע�ᵽRPLMN�� */
    /*PC REPLAY MODIFY BY LEILI BEGIN*/
    ulRslt = NAS_EMM_GetGulPubInfo(MMC_LMM_HPLMN, &stPubInfo);
    if (MMC_LMM_SUCC != ulRslt)
    {
        NAS_LMM_PUBM_LOG_NORM("NAS_LMM_IsRegisteredInHplmn:Hplmn cannot be got!");
        return NAS_EMM_NO;
    }
    /*PC REPLAY MODIFY BY LEILI END*/

    if (NAS_LMM_MATCH_SUCCESS == NAS_LMM_HplmnMatch((NAS_MM_PLMN_ID_STRU *)(&stPubInfo.u.stHplmn), pstCurentPlmn))
    {
     	return NAS_EMM_YES;
    }

    return NAS_EMM_NO;
}


VOS_VOID  NAS_LMM_StateMap(const NAS_EMM_FSM_STATE_STRU *pstEmmDestState,
                        NAS_APP_REG_STAT_ENUM_UINT32   *pulNasAppDstState)

{
    switch(pstEmmDestState->enMainState)
    {
        case    EMM_MS_NULL:
        case    EMM_MS_DEREG_INIT:
                *pulNasAppDstState = NAS_APP_RS_NOT_REG_AND_NOT_SRCH_OPERATOR;
                break;

        case    EMM_MS_DEREG:
                if ((EMM_SS_DEREG_NO_IMSI == pstEmmDestState->enSubState))
                {
                    *pulNasAppDstState = NAS_APP_RS_NOT_REG_AND_NOT_SRCH_OPERATOR;
                }
                else
                {
                    *pulNasAppDstState = NAS_APP_RS_NOT_REG_AND_SRCH_OPERATOR;
                }
                break;

        case    EMM_MS_REG_INIT:
                *pulNasAppDstState = NAS_APP_RS_NOT_REG_AND_SRCH_OPERATOR;
                break;

        case    EMM_MS_REG:
                if (NAS_EMM_YES == NAS_LMM_IsRegisteredInHplmn())
                {
                    NAS_LMM_PUBM_LOG_NORM("NAS_LMM_StateMap HPLMN");
                    *pulNasAppDstState = NAS_APP_RS_REG_HPLMN;
                }
                else
                {
                    *pulNasAppDstState = NAS_APP_RS_REG_ROAMING;
                }
                break;

        default:
                break;

    }

    return;
}




VOS_VOID  NAS_LMM_RegStatOsa( const NAS_APP_REG_STAT_ENUM_UINT32   *pulNasAppDstState,
                               APP_REG_STAT_ENUM_UINT32       *pulAppRegStat)
{
    NAS_LMM_PUBM_LOG_INFO("NAS_LMM_RegStatOsa is entered");

    switch(*pulNasAppDstState)
    {
    case NAS_APP_RS_NOT_REG_AND_NOT_SRCH_OPERATOR:
        *pulAppRegStat                    = APP_REG_STAT_NOT_REG_AND_NOT_SRCH_OPERATOR;
        break;


    case NAS_APP_RS_NOT_REG_AND_SRCH_OPERATOR:
        *pulAppRegStat                    = APP_REG_STAT_NOT_REG_AND_SRCH_OPERATOR;
        break;

    case NAS_APP_RS_REG_HPLMN:
        *pulAppRegStat                    = APP_REG_STAT_REG_HPLMN;
        break;

    case NAS_APP_RS_REG_ROAMING:
        *pulAppRegStat                    = APP_REG_STAT_REG_ROAMING;
        break;

    case NAS_APP_RS_REG_DENIED:
        *pulAppRegStat                    = APP_REG_STAT_REG_DENIED;
        break;

    case NAS_APP_RS_UNKNOWN:
        *pulAppRegStat                    = APP_REG_STAT_UNKNOWN;
        break;
    default:

        break;

    }
    return;
}



VOS_VOID  NAS_EMM_AppRegStatInd( NAS_APP_REG_STAT_ENUM_UINT32   ulNasAppState,
                                           NAS_LMM_MAIN_STATE_ENUM_UINT16  enMainState)
{

    APP_MM_REG_STAT_IND_STRU            *pEmmAppRegStatMsg;
    NAS_MM_NETWORK_ID_STRU              *pstNetIdInfo;
    APP_REG_STAT_ENUM_UINT32            ulAppRegStat = APP_REG_STAT_NOT_REG_AND_NOT_SRCH_OPERATOR;

    NAS_LMM_PUBM_LOG_INFO("NAS_EMM_AppRegStatInd is entered");

    pstNetIdInfo                        = NAS_LMM_GetEmmInfoNetInfoPresentNetAddr();

    /*������Ϣ�ڴ�*/
    pEmmAppRegStatMsg = (VOS_VOID *)NAS_LMM_ALLOC_MSG(sizeof(APP_MM_REG_STAT_IND_STRU));

    /*�ж�����������ʧ�ܴ�ӡ�����˳�*/
    if (NAS_EMM_NULL_PTR == pEmmAppRegStatMsg)
    {
        /*��ӡ����*/
        NAS_EMM_ATTACH_LOG_ERR("NAS_EMM_AppRegStatInd: MSG ALLOC ERR!");
        return;

    }

    NAS_LMM_MEM_SET(pEmmAppRegStatMsg,0,sizeof(APP_MM_REG_STAT_IND_STRU));

    /*����APP_MM_REG_STAT_IND��Ϣ*/
     /* �����Ϣͷ */
    NAS_LMM_COMP_APP_DT_MSG_HEADER( pEmmAppRegStatMsg,
                                 NAS_LMM_MSG_LENGTH_NO_HEADER(APP_MM_REG_STAT_IND_STRU));
    /*�����ϢID*/
    pEmmAppRegStatMsg->ulMsgId          = ID_APP_MM_REG_STAT_IND;

    /*�����Ϣ����*/
    pEmmAppRegStatMsg->ulOpId           = DIAG_CMD_MM_REG_STAT_IND;
    /*pEmmAppRegStatMsg->ulRslt           = APP_SUCCESS;*/
    pEmmAppRegStatMsg->enAccessMode     = NAS_LMM_GetEmmInfoRaMod();

    /*��÷�������Ϣ*/
    pEmmAppRegStatMsg->enSrvDomain      = NAS_LMM_GetServiceDomain(enMainState);

    NAS_LMM_RegStatOsa(&ulNasAppState,   &ulAppRegStat);

    pEmmAppRegStatMsg->enRegStat        = ulAppRegStat;

    NAS_LMM_MEM_CPY(                     &pEmmAppRegStatMsg->stPlmnId,
                                        &pstNetIdInfo->stPlmnId,
                                        sizeof(NAS_MM_PLMN_ID_STRU));

    NAS_LMM_MEM_CPY(                     &pEmmAppRegStatMsg->stTac,
                                        &pstNetIdInfo->stTac,
                                        sizeof(NAS_MM_TAC_STRU));

    pEmmAppRegStatMsg->ulCellId         = pstNetIdInfo->ulCellId;

    /*��APP����APP_MM_REG_STAT_IND��Ϣ*/
    NAS_LMM_SEND_MSG(pEmmAppRegStatMsg);

    return;
}


MMC_LMM_SERVICE_STATUS_ENUM_UINT32  NAS_LMM_DeregServiceStateMap(
                                        const NAS_EMM_FSM_STATE_STRU        *pstEmmDestState)
{
    MMC_LMM_SERVICE_STATUS_ENUM_UINT32  ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_TYPE_BUTT;

    switch(pstEmmDestState->enSubState)
    {
        case EMM_SS_DEREG_NORMAL_SERVICE:
        case EMM_SS_DEREG_NO_CELL_AVAILABLE:
            ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_NO_SERVICE;
            break;

        case EMM_SS_DEREG_ATTEMPTING_TO_ATTACH:
        case EMM_SS_DEREG_ATTACH_NEEDED:
        case EMM_SS_DEREG_NO_IMSI:
        case EMM_SS_DEREG_LIMITED_SERVICE:
            ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_LIMIT_SERVICE;
            break;

        default:
            break;
    }
    /* ע��rej12�ı�����뿪DEREG LIMIT SERVICE״̬ʱҪ��0 */
    if ((EMM_SS_DEREG_LIMITED_SERVICE == pstEmmDestState->enSubState)
       &&(NAS_LMM_REJ_CAUSE_IS_12 == NAS_LMM_GetNasRejCause12Flag()))
    {
        ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_LIMIT_SERVICE_REGION;
        NAS_LMM_SetNasRejCause12Flag(NAS_LMM_REJ_CAUSE_NOT_12);

    }
    return ulNasMmcDstServiceState;
}
MMC_LMM_SERVICE_STATUS_ENUM_UINT32  NAS_LMM_RegServiceStateMap(
                                        const NAS_EMM_FSM_STATE_STRU        *pstEmmDestState)
{
    MMC_LMM_SERVICE_STATUS_ENUM_UINT32  ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_TYPE_BUTT;

    switch(pstEmmDestState->enSubState)
    {
        case EMM_SS_REG_NO_CELL_AVAILABLE:

            ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_NO_SERVICE;
            break;

        case EMM_SS_REG_ATTEMPTING_TO_UPDATE:
            if(NAS_EMM_TAU_ATTEMPT_CNT_MAX <= NAS_EMM_TAU_GetEmmTAUAttemptCnt())
            {
                ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_LIMIT_SERVICE;
            }
            break;

        case EMM_SS_REG_LIMITED_SERVICE:
            ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_LIMIT_SERVICE;
            break;

        case EMM_SS_REG_WAIT_ACCESS_GRANT_IND:
            if (NAS_EMM_BAR_PROCEDURE_TAU == NAS_EMM_GetBarProcedure())
            {
                ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_LIMIT_SERVICE;
            }
            break;

        case EMM_SS_REG_NORMAL_SERVICE:
        case EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM:
        case EMM_SS_REG_IMSI_DETACH_WATI_CN_DETACH_CNF:
            ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_NORMAL_SERVICE;
            break;

        default:
            break;
    }

    return ulNasMmcDstServiceState;
}
VOS_VOID  NAS_EMM_SendMmcServiceStatInd( VOS_UINT32 ulCsServiceStateChangeFlag,
                                                      VOS_UINT32 ulPsServiceStateChangeFlag)

{
    LMM_MMC_SERVICE_STATUS_IND_STRU    *pEmmMmcSerStatMsg;

    NAS_LMM_PUBM_LOG_INFO("NAS_EMM_SendMmcServiceStatInd is entered");

    if(NAS_LMM_MMC_SER_REG_STAT_OPEN_FLAG == NAS_Emm_Get_Mmc_Srv_Reg_Flag())
    {
        /*������Ϣ�ڴ�*/
        pEmmMmcSerStatMsg = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_SERVICE_STATUS_IND_STRU));

        /*�ж�����������ʧ�ܴ�ӡ�����˳�*/
        if (NAS_EMM_NULL_PTR == pEmmMmcSerStatMsg)
        {
            /*��ӡ����*/
            NAS_LMM_PUBM_LOG_ERR("NAS_EMM_SendMmcServiceStatInd: MSG ALLOC ERR!");
            return;

        }

        NAS_LMM_MEM_SET(pEmmMmcSerStatMsg,0,sizeof(LMM_MMC_SERVICE_STATUS_IND_STRU));

        /*����LMM_MMC_SERVICE_STATUS_IND��Ϣ*/
        /* �����Ϣͷ */
        EMM_PUBU_COMP_MMC_MSG_HEADER( pEmmMmcSerStatMsg,
                                     NAS_LMM_MSG_LENGTH_NO_HEADER(LMM_MMC_SERVICE_STATUS_IND_STRU));

        /*�����ϢID*/
        pEmmMmcSerStatMsg->ulMsgId          = ID_LMM_MMC_SERVICE_STATUS_IND;

        /*�����Ϣ����*/
        pEmmMmcSerStatMsg->ulOpId           = NAS_EMM_OPID_MMC;

        if((NAS_EMM_YES == ulCsServiceStateChangeFlag) && (MMC_LMM_SERVICE_STATUS_TYPE_BUTT != NAS_LMM_GetNasMmcCSServiceState()))
        {
            pEmmMmcSerStatMsg->bitOpCsStatus = NAS_EMM_BIT_SLCT;
            pEmmMmcSerStatMsg->ulCsServiceStatus = NAS_LMM_GetNasMmcCSServiceState();
        }

        if(NAS_EMM_YES == ulPsServiceStateChangeFlag)
        {
            pEmmMmcSerStatMsg->bitOpPsStatus = NAS_EMM_BIT_SLCT;
            pEmmMmcSerStatMsg->ulServiceStatus  = NAS_LMM_GetNasMmcServiceState();
        }

        /*pEmmMmcSerStatMsg->ulServiceStatus  = ulNasMmcSerivceState;*/

        /*��MMC����LMM_MMC_SERVICE_STATUS_IND��Ϣ*/
        NAS_LMM_SendLmmMmcMsg(pEmmMmcSerStatMsg);
    }
    return;
}
MMC_LMM_SERVICE_STATUS_ENUM_UINT32  NAS_LMM_RegCSServiceStateMap(
                                        const NAS_EMM_FSM_STATE_STRU        *pstEmmDestState)
{
    MMC_LMM_SERVICE_STATUS_ENUM_UINT32  ulNasMmcDstServiceState = NAS_LMM_GetNasMmcCSServiceState();

    /*ע����ΪPS,CS����״̬��ΪNO SERVICE*/
    if(NAS_LMM_REG_DOMAIN_PS == NAS_LMM_GetEmmInfoRegDomain())
    {
        NAS_LMM_PUBM_LOG1_NORM("NAS_LMM_RegCSServiceStateMap:NAS_LMM_GetEmmInfoRegDomain",NAS_LMM_GetEmmInfoRegDomain());
        ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_NO_SERVICE;

        return ulNasMmcDstServiceState;
    }

    switch(pstEmmDestState->enSubState)
    {
        case EMM_SS_REG_NO_CELL_AVAILABLE:
        /* EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM������ΪCS_PSע���� ������   */
        /*case EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM:*/
            ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_NO_SERVICE;
            break;

        case EMM_SS_REG_ATTEMPTING_TO_UPDATE:
            if(NAS_EMM_TAU_ATTEMPT_CNT_MAX <= NAS_EMM_TAU_GetEmmTAUAttemptCnt())
            {
                ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_LIMIT_SERVICE;
            }
            break;

        case EMM_SS_REG_LIMITED_SERVICE:
            ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_LIMIT_SERVICE;
            break;
        case EMM_SS_REG_WAIT_ACCESS_GRANT_IND:
            /* tau�����н�����barʱ�ϱ����Ʒ��� */
            if (NAS_EMM_BAR_PROCEDURE_TAU == NAS_EMM_GetBarProcedure())
            {
                ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_LIMIT_SERVICE;
            }
            break;
        case EMM_SS_REG_NORMAL_SERVICE:
            ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_NORMAL_SERVICE;
            break;

        default:
            break;

    }

    return ulNasMmcDstServiceState;
}


/*****************************************************************************
 Function Name   : NAS_LMM_UeModeCsPsCSServiceStateMap
 Description     : UEģʽΪCS/PS,�ѵ�ǰEMM״̬ӳ��ΪEMM�ڲ������ϱ���MMC��CS�����״̬
 Input           : ��ǰEMM״̬
 Output          : �ڲ������ϱ���MMC�ķ���״̬
 Return          : VOS_VOID

 History         :
    1.   FTY       2012-01-11  Draft Enact

*****************************************************************************/
MMC_LMM_SERVICE_STATUS_ENUM_UINT32  NAS_LMM_CsPsModeCSServiceStateMap(
                                        const NAS_EMM_FSM_STATE_STRU        *pstEmmDestState)

{
    MMC_LMM_SERVICE_STATUS_ENUM_UINT32 ulNasMmcDstServiceState = NAS_LMM_GetNasMmcCSServiceState();

    switch(pstEmmDestState->enMainState)
    {
        case EMM_MS_NULL:
        case EMM_MS_DEREG:
            ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_NO_SERVICE;
            break;

        case EMM_MS_REG:
            ulNasMmcDstServiceState = NAS_LMM_RegCSServiceStateMap(pstEmmDestState);
            break;

        /*����״̬��������*/
        default:
                break;

    }

    return ulNasMmcDstServiceState;
}
VOS_VOID NAS_LMM_CovertMmlRegStatusToLmmRegStatus
(
    NAS_MML_REG_STATUS_ENUM_UINT8 enMmlRegStatus,
    MMC_LMM_REGISTER_STATUS_ENUM_UINT32 *penLmmRegState
)
{
    switch(enMmlRegStatus)
    {
        case  NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH:
              *penLmmRegState = MMC_LMM_REGISTER_STATUS_NOT_REG_NOT_SRCHING;
              break;

        case  NAS_MML_REG_REGISTERED_HOME_NETWORK:
              *penLmmRegState = MMC_LMM_REGISTER_STATUS_REG_HPLMN;
              break;

        case  NAS_MML_REG_NOT_REGISTERED_SEARCHING:
              *penLmmRegState = MMC_LMM_REGISTER_STATUS_NOT_REG_SRCHING;
              break;

        case  NAS_MML_REG_REGISTERED_DENIED:
              *penLmmRegState = MMC_LMM_REGISTER_STATUS_REG_DENINED;
              break;

        case  NAS_MML_REG_UNKNOWN:
              *penLmmRegState = MMC_LMM_REGISTER_STATUS_UNKNOWN;
              break;

        case  NAS_MML_REG_REGISTERED_ROAM:
              *penLmmRegState = MMC_LMM_REGISTER_STATUS_REG_ROAMING;
              break;

        default:
              NAS_LMM_PUBM_LOG_ERR("NAS_LMM_CovertMmlRegStatusToLmmRegStatus: enMmlRegStatus ERR!");
              break;

    }
    return;
}





VOS_VOID  NAS_EMM_SendMmcRegisterStatInd(NAS_EMM_REG_CHANGE_TYPE_ENUM_UINT32 ulChangeType)
{
    NAS_MM_NETWORK_ID_STRU             *pstNetIdInfo;
    LMM_MMC_REGISTER_STATUS_IND_STRU   *pEmmMmcRegStatMsg;
    MMC_LMM_REGISTER_STATUS_ENUM_UINT32 ulNasMmcRegState;


    pstNetIdInfo                        = NAS_LMM_GetEmmInfoNetInfoPresentNetAddr();
    ulNasMmcRegState                    = NAS_LMM_GetNasMmcRegisterState();

    NAS_LMM_PUBM_LOG_INFO("NAS_EMM_SendMmcServiceStatInd is entered");

    if(NAS_LMM_MMC_SER_REG_STAT_OPEN_FLAG == NAS_Emm_Get_Mmc_Srv_Reg_Flag())
    {
        /*������Ϣ�ڴ�*/
        pEmmMmcRegStatMsg = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_REGISTER_STATUS_IND_STRU));

        /*�ж�����������ʧ�ܴ�ӡ�����˳�*/
        if (NAS_EMM_NULL_PTR == pEmmMmcRegStatMsg)
        {
            /*��ӡ����*/
            NAS_LMM_PUBM_LOG_ERR("NAS_EMM_SendMmcRegisterStatInd: MSG ALLOC ERR!");
            return;

        }

        NAS_LMM_MEM_SET(pEmmMmcRegStatMsg,0,sizeof(LMM_MMC_REGISTER_STATUS_IND_STRU));

        /*����LMM_MMC_RESISTER_STATUS_IND��Ϣ*/
        /* �����Ϣͷ */
        EMM_PUBU_COMP_MMC_MSG_HEADER( pEmmMmcRegStatMsg,
                                     NAS_LMM_MSG_LENGTH_NO_HEADER(LMM_MMC_REGISTER_STATUS_IND_STRU));

        /*�����ϢID*/
        pEmmMmcRegStatMsg->ulMsgId          = ID_LMM_MMC_REGISTER_STATUS_IND;

        /*�����Ϣ����*/
        pEmmMmcRegStatMsg->ulOpId           = NAS_EMM_OPID_MMC;


        if (NAS_EMM_REG_CHANGE_TYPE_REG_STATE == ulChangeType)
        {
            pEmmMmcRegStatMsg->bitOpLocationInfo = NAS_EMM_BIT_NO_SLCT;

            pEmmMmcRegStatMsg->bitOpRegisterStatus = NAS_EMM_BIT_SLCT;

            pEmmMmcRegStatMsg->ulRegisterStatus = ulNasMmcRegState;
        }
        else
        {
            pEmmMmcRegStatMsg->bitOpLocationInfo = NAS_EMM_BIT_SLCT;

            pEmmMmcRegStatMsg->bitOpRegisterStatus = NAS_EMM_BIT_NO_SLCT;

            pEmmMmcRegStatMsg->ulCellId = pstNetIdInfo->ulCellId;

            NAS_LMM_MEM_CPY(                    &pEmmMmcRegStatMsg->stTac,
                                            &pstNetIdInfo->stTac,
                                            sizeof(NAS_MM_TAC_STRU));
        }

        /*��MMC����LMM_MMC_REGISTER_STATUS_IND��Ϣ*/
        NAS_LMM_SendLmmMmcMsg(pEmmMmcRegStatMsg);

    }
    return;

}


MMC_LMM_SERVICE_STATUS_ENUM_UINT32  NAS_LMM_ServiceStateMap(
                                        const NAS_EMM_FSM_STATE_STRU        *pstEmmDestState)

{
    MMC_LMM_SERVICE_STATUS_ENUM_UINT32 ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_TYPE_BUTT;

    switch(pstEmmDestState->enMainState)
    {
        case EMM_MS_NULL:
            ulNasMmcDstServiceState = MMC_LMM_SERVICE_STATUS_NO_SERVICE;
            break;

        case EMM_MS_DEREG:
            ulNasMmcDstServiceState = NAS_LMM_DeregServiceStateMap(pstEmmDestState);
            break;

        case EMM_MS_REG:
            ulNasMmcDstServiceState = NAS_LMM_RegServiceStateMap(pstEmmDestState);
            break;

        /*����״̬��������*/
        default:
                break;

    }

    return ulNasMmcDstServiceState;
}

/*****************************************************************************
 Function Name   : NAS_LMM_CSServiceStateMap
 Description     : �ѵ�ǰEMM״̬ӳ��ΪEMM�ڲ������ϱ���MMC��CS�����״̬
 Input           : ��ǰEMM״̬
 Output          : �ڲ������ϱ���MMC�ķ���״̬
 Return          : VOS_VOID

 History         :
    1.    FTY      2012-01-11  Draft Enact

*****************************************************************************/
MMC_LMM_SERVICE_STATUS_ENUM_UINT32  NAS_LMM_CSServiceStateMap(
                                        const NAS_EMM_FSM_STATE_STRU        *pstEmmDestState)

{
      MMC_LMM_SERVICE_STATUS_ENUM_UINT32 ulNasMmcDstCSServiceState = MMC_LMM_SERVICE_STATUS_TYPE_BUTT;
      if(NAS_EMM_YES == NAS_EMM_IsCsPsUeMode())
      {
           ulNasMmcDstCSServiceState = NAS_LMM_CsPsModeCSServiceStateMap(pstEmmDestState);
      }

      return ulNasMmcDstCSServiceState;
}

/*****************************************************************************
 Function Name   : NAS_LMM_IsMmcPSServiceStateChange
 Description     :
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.    FTY      2012-01-13 Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_LMM_IsMmcPSServiceStateChange( const NAS_EMM_FSM_STATE_STRU  *pstEmmDestState,
                                                              MMC_LMM_SERVICE_STATUS_ENUM_UINT32   *pulNasMmcDstServiceState)
{
    MMC_LMM_SERVICE_STATUS_ENUM_UINT32       ulNasMmcDstServiceState;
    MMC_LMM_SERVICE_STATUS_ENUM_UINT32       ulNasMmcOriServiceState;


    /* ����EMM��Ŀ��״̬������Ŀ��MMCע��״̬ */
    ulNasMmcDstServiceState = NAS_LMM_ServiceStateMap(pstEmmDestState);
    ulNasMmcOriServiceState = NAS_LMM_GetNasMmcServiceState();

    NAS_LMM_PUBM_LOG1_NORM("NAS_LMM_IsMmcPSServiceStateChange NAS_LMM_GetNasMmcServiceState()=",ulNasMmcOriServiceState);
    NAS_LMM_PUBM_LOG1_NORM("NAS_LMM_IsMmcPSServiceStateChange ulNasMmcDstState=",ulNasMmcDstServiceState);

    if((ulNasMmcOriServiceState != ulNasMmcDstServiceState)
      &&(MMC_LMM_SERVICE_STATUS_TYPE_BUTT != ulNasMmcDstServiceState))
    {
        *pulNasMmcDstServiceState = ulNasMmcDstServiceState;
        return NAS_EMM_YES;
    }

    return NAS_EMM_NO;

}

/*****************************************************************************
 Function Name   : NAS_LMM_IsMmcCSServiceStateChange
 Description     :
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.    FTY      2012-01-13 Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_LMM_IsMmcCSServiceStateChange( const NAS_EMM_FSM_STATE_STRU  *pstEmmDestState,
                                               MMC_LMM_SERVICE_STATUS_ENUM_UINT32   *pulNasMmcDstServiceState)
{
    MMC_LMM_SERVICE_STATUS_ENUM_UINT32       ulNasMmcCSDstServiceState;
    MMC_LMM_SERVICE_STATUS_ENUM_UINT32       ulNasMmcCSOriServiceState;


    /* ����EMM��Ŀ��״̬������Ŀ��MMCע��״̬ */
    ulNasMmcCSDstServiceState = NAS_LMM_CSServiceStateMap(pstEmmDestState);
    ulNasMmcCSOriServiceState = NAS_LMM_GetNasMmcCSServiceState();

    NAS_LMM_PUBM_LOG1_NORM("NAS_LMM_IsMmcCSServiceStateChange NAS_LMM_GetNasMmcCSServiceState()=",ulNasMmcCSOriServiceState);
    NAS_LMM_PUBM_LOG1_NORM("NAS_LMM_IsMmcCSServiceStateChange ulNasMmcDstState=",ulNasMmcCSDstServiceState);

    if(ulNasMmcCSOriServiceState != ulNasMmcCSDstServiceState)
    {
        *pulNasMmcDstServiceState = ulNasMmcCSDstServiceState;
        return NAS_EMM_YES;
    }

    return NAS_EMM_NO;

}
VOS_VOID  NAS_LMM_MmcServiceStateChange( NAS_EMM_FSM_STATE_STRU  stEmmDestState)
{
    MMC_LMM_SERVICE_STATUS_ENUM_UINT32       ulNasMmcDstPSServiceState  = MMC_LMM_SERVICE_STATUS_TYPE_BUTT;
    MMC_LMM_SERVICE_STATUS_ENUM_UINT32       ulNasMmcDstCSServiceState  = MMC_LMM_SERVICE_STATUS_TYPE_BUTT;
    VOS_UINT32                               ulPsServiceStateChangeFlag = NAS_EMM_NO;
    VOS_UINT32                               ulCsServiceStateChangeFlag = NAS_EMM_NO;;

    /* ��μ�� */
    if ( NAS_LMM_PARALLEL_FSM_EMM != stEmmDestState.enFsmId )
    {
       /* NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_AppStateChange, The input is illegal! enParaFsmId",
                             stEmmDestState.enFsmId);*/
        return;
    }

    /*���CS��״̬�Ƿ�ı�*/
    ulCsServiceStateChangeFlag = NAS_LMM_IsMmcCSServiceStateChange(&stEmmDestState,&ulNasMmcDstCSServiceState);
    if(NAS_EMM_YES == ulCsServiceStateChangeFlag)
    {
        NAS_LMM_SetNasMmcCSServiceState(ulNasMmcDstCSServiceState);
    }

    /*���PS��״̬�Ƿ�ı�*/
    ulPsServiceStateChangeFlag = NAS_LMM_IsMmcPSServiceStateChange(&stEmmDestState,&ulNasMmcDstPSServiceState);
    if(NAS_EMM_YES == ulPsServiceStateChangeFlag)
    {
        NAS_LMM_SetNasMmcServiceState(ulNasMmcDstPSServiceState);
    }

    /*ֻ�з���״̬�ı䣬����CS�ı�ʱ��״̬��Чʱ�ŷ���*/
    if((NAS_EMM_NO != ulPsServiceStateChangeFlag)
      || ((NAS_EMM_NO != ulCsServiceStateChangeFlag)
         &&(MMC_LMM_SERVICE_STATUS_TYPE_BUTT != ulNasMmcDstCSServiceState)))
    {
        /*���LģΪ��ģ,����Ҫ����*/
        if ( (EMM_MS_NULL == stEmmDestState.enMainState)
           ||( (NAS_LMM_CUR_LTE_ACTIVE == NAS_EMM_GetCurLteState())
             &&(EMM_MS_NULL != stEmmDestState.enMainState)))
        {
           NAS_EMM_SendMmcServiceStatInd(ulCsServiceStateChangeFlag, ulPsServiceStateChangeFlag);
        }
    }


    return;
}

MMC_LMM_REGISTER_STATUS_ENUM_UINT32  NAS_LMM_RegRegisterStateMap(
                                        const NAS_EMM_FSM_STATE_STRU        *pstEmmDestState)
{
    MMC_LMM_REGISTER_STATUS_ENUM_UINT32 ulNasMmcDstRegisterState = MMC_LMM_REGISTER_STATUS_BUTT;

    switch(pstEmmDestState->enSubState)
    {
        case EMM_SS_REG_WAIT_ACCESS_GRANT_IND:
            ulNasMmcDstRegisterState = MMC_LMM_REGISTER_STATUS_NOT_REG_NOT_SRCHING;
            break;

        case EMM_SS_REG_LIMITED_SERVICE:
        case EMM_SS_REG_PLMN_SEARCH:
        case EMM_SS_REG_NO_CELL_AVAILABLE:
            ulNasMmcDstRegisterState = MMC_LMM_REGISTER_STATUS_NOT_REG_SRCHING;
            break;

        case EMM_SS_REG_NORMAL_SERVICE:
        case EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM:
        case EMM_SS_REG_IMSI_DETACH_WATI_CN_DETACH_CNF:
            if(NAS_EMM_YES == NAS_LMM_IsRegisteredInHplmn())
            {
                ulNasMmcDstRegisterState = MMC_LMM_REGISTER_STATUS_REG_HPLMN;
            }
            else
            {
                ulNasMmcDstRegisterState = MMC_LMM_REGISTER_STATUS_REG_ROAMING;
            }
            break;

        case EMM_SS_REG_ATTEMPTING_TO_UPDATE:
            if(NAS_EMM_TAU_ATTEMPT_CNT_MAX <= NAS_EMM_TAU_GetEmmTAUAttemptCnt())
            {
                ulNasMmcDstRegisterState = MMC_LMM_REGISTER_STATUS_NOT_REG_SRCHING;
            }
            break;

        default:
            break;
    }
    return ulNasMmcDstRegisterState;
}


MMC_LMM_REGISTER_STATUS_ENUM_UINT32  NAS_LMM_DeregRegisterStateMap(
                                        const NAS_EMM_FSM_STATE_STRU         *pstEmmDestState)
{
    MMC_LMM_REGISTER_STATUS_ENUM_UINT32 ulNasMmcDstRegisterState = MMC_LMM_REGISTER_STATUS_BUTT;

    switch(pstEmmDestState->enSubState)
    {
        case EMM_SS_DEREG_NORMAL_SERVICE:
        case EMM_SS_DEREG_ATTACH_NEEDED:
            ulNasMmcDstRegisterState = MMC_LMM_REGISTER_STATUS_NOT_REG_NOT_SRCHING;
            break;

        case EMM_SS_DEREG_LIMITED_SERVICE:
        case EMM_SS_DEREG_ATTEMPTING_TO_ATTACH:
        case EMM_SS_DEREG_NO_CELL_AVAILABLE:
        case EMM_SS_DEREG_PLMN_SEARCH:
            if (VOS_FALSE == NAS_MML_GetPsAttachAllowFlg())
            {
                ulNasMmcDstRegisterState = MMC_LMM_REGISTER_STATUS_NOT_REG_NOT_SRCHING;
            }
            else
            {
                ulNasMmcDstRegisterState = MMC_LMM_REGISTER_STATUS_NOT_REG_SRCHING;
            }
            break;

        case EMM_SS_DEREG_NO_IMSI:
            ulNasMmcDstRegisterState = MMC_LMM_REGISTER_STATUS_REG_DENINED;
            break;

        default:
            break;
    }
    return ulNasMmcDstRegisterState;
}


MMC_LMM_REGISTER_STATUS_ENUM_UINT32  NAS_LMM_RegisterStateMap(
                                        const NAS_EMM_FSM_STATE_STRU         *pstEmmDestState)
{
    MMC_LMM_REGISTER_STATUS_ENUM_UINT32 ulNasMmcDstRegisterState = MMC_LMM_REGISTER_STATUS_BUTT;

    switch(pstEmmDestState->enMainState)
    {

        case EMM_MS_NULL:
        case EMM_MS_DEREG_INIT:
            ulNasMmcDstRegisterState = MMC_LMM_REGISTER_STATUS_NOT_REG_NOT_SRCHING;
            break;

        case EMM_MS_REG_INIT:
            ulNasMmcDstRegisterState = MMC_LMM_REGISTER_STATUS_NOT_REG_SRCHING;
            break;

        case EMM_MS_SER_INIT:
            if(NAS_EMM_YES == NAS_LMM_IsRegisteredInHplmn())
            {
                ulNasMmcDstRegisterState = MMC_LMM_REGISTER_STATUS_REG_HPLMN;
            }
            else
            {
                ulNasMmcDstRegisterState = MMC_LMM_REGISTER_STATUS_REG_ROAMING;
            }
            break;

        case EMM_MS_DEREG:
            ulNasMmcDstRegisterState = NAS_LMM_DeregRegisterStateMap(pstEmmDestState);
            break;

        case EMM_MS_REG:
            ulNasMmcDstRegisterState = NAS_LMM_RegRegisterStateMap(pstEmmDestState);
            break;

        /*����״̬��������*/
        default:
            break;
    }
    return ulNasMmcDstRegisterState;
}
VOS_VOID  NAS_LMM_MmcRegisterStateChange( NAS_EMM_FSM_STATE_STRU  stEmmDestState)
{
    MMC_LMM_REGISTER_STATUS_ENUM_UINT32       ulNasMmcDstRegState;

    /* ��μ�� */
    if ( NAS_LMM_PARALLEL_FSM_EMM != stEmmDestState.enFsmId )
    {
       /* NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_AppStateChange, The input is illegal! enParaFsmId",
                             stEmmDestState.enFsmId);*/
        return;
    }

    ulNasMmcDstRegState = NAS_LMM_GetNasMmcRegisterState();

    /* ����EMM��Ŀ��״̬������Ŀ��MMCע��״̬ */
    ulNasMmcDstRegState = NAS_LMM_RegisterStateMap(&stEmmDestState);

    NAS_LMM_PUBM_LOG1_NORM("NAS_LMM_MmcRegisterStateChange NAS_LMM_GetNasMmcRegState()=",NAS_LMM_GetNasMmcRegisterState());

    NAS_LMM_PUBM_LOG1_NORM("NAS_LMM_MmcRegisterStateChange ulNasMmcDstState=",ulNasMmcDstRegState);

    /* ���ϱ���MMCע��״̬�����仯�����޸ĸ�״̬�����ϱ���MMC */
    if ((NAS_LMM_GetNasMmcRegisterState() != ulNasMmcDstRegState)
       &&(MMC_LMM_REGISTER_STATUS_BUTT != ulNasMmcDstRegState))
    {
        NAS_LMM_SetNasMmcRegisterState(ulNasMmcDstRegState);

        /*�ڷ���֮ǰ�жϣ�Lģ�����������ϱ� */
        /*add by FTY in 2012-01-11 start:*/
        /*û�м�������������ı�Ҫ���ر���ע��״̬�����ϱ�*/

        if (NAS_LMM_CUR_LTE_ACTIVE == NAS_EMM_GetCurLteState())
        {
            /*��MMC����ID_LMM_MMC_SERVICE_STATUS_IND��Ϣ*/
            NAS_EMM_SendMmcRegisterStatInd(NAS_EMM_REG_CHANGE_TYPE_REG_STATE);
        }
    }
}
/*****************************************************************************/
/*lint +e961*/
/*lint +e960*/
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
/* end of ��ϵͳ+ģ��+�ļ���.c */
