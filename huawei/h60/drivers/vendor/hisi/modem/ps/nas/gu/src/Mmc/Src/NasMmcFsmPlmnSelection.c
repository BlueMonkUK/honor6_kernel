

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "om.h"

#include "NasMmcFsmPlmnSelectionTbl.h"
#include "NasMmcCtx.h"
#include "NasMmcFsmPlmnSelection.h"
#include "NasMmcSndGuAs.h"
#include "NasMmcSndLmm.h"
#include "NasMmcSndGmm.h"
#include "NasMmcSndMm.h"
#include "NasMmcProcRegRslt.h"
#include "NasMmcPlmnSelectionStrategy.h"
#include "NasMmcTimerMgmt.h"
#include "NasComm.h"
#include "NasMmlLib.h"
#include "NasMmlCtx.h"
#include "NasMmlMsgProc.h"
#include "NasMmcSndMma.h"
#include "NasMmcProcUsim.h"
#include "NasMmcProcSuspend.h"
#include "NasMmcSndOm.h"
#include "NasMmcProcNvim.h"
/* ɾ��ExtAppMmcInterface.h*/
#include "MmcMmInterface.h"
#include "MmcGmmInterface.H"
#include "NasMmcComFunc.h"

#include "MmaMmcInterface.h"
#include "NasUtranCtrlInterface.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_NAS_MMC_FSM_PLMN_SELECTION_C



VOS_UINT32 NAS_MMC_RcvTafPlmnSearchReq_PlmnSelection_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_WITH_RAT_STRU                              stDestPlmn;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;


	/* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    pstPlmnSelectionListInfo = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    /* �ϱ�AT������ʼ */
    NAS_MMC_SndMmaPlmnSelectStartInd();

    /* ���������Ϣ */
    NAS_MMC_SaveCurEntryMsg(ulEventType, pstMsg);

    /* ���ݳ�����ʼ�������б���Ϣ */
    NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_SWITCH_ON,
                                  VOS_NULL_PTR,
                                  pstPlmnSelectionListInfo);

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);

    /* ��ȡ��Ҫ���������缰����뼼�� */
    if (VOS_TRUE == NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
    {

        /* ѡ��״̬������ѡ���������NO RF��� */
        NAS_MMC_ClearNoRfInfo();
        
        /* ���ݻ�ȡ�Ľ��뼼�����õ�ǰ���뼼�� */
        NAS_MML_SetCurrNetRatType(stDestPlmn.enRat);

        /* ��MM��GMM��������ָʾ */
        NAS_MMC_SndGmmPlmnSchInit();
        NAS_MMC_SndMmPlmnSchInit();

        NAS_MMC_SndMmaCampOnInd(VOS_FALSE);

        NAS_MMC_UpdateRegStateSpecPlmnSearch();

        /* �������LMM�����������󣬸��ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);

#if   (FEATURE_ON == FEATURE_LTE)
        /* ��������ʱ�����ݿ����͸���Lte�������������SIM������ҪDisable Lte */
        NAS_MMC_DisableLteForSimReason_PlmnSelection();
#endif

    }
    else
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTafPlmnSearchReq_PlmnSelection_Init: Get Next Search Plmn fail at switch on.");

        /* ���ϲ�״̬��ָʾѡ��ʧ�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvTafPlmnSpecialReq_PlmnSelection_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_WITH_RAT_STRU                              stDestPlmn;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enNetRatType;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;

    MMA_MMC_PLMN_SPECIAL_REQ_STRU                          *pstUserSelReqMsg  = VOS_NULL_PTR;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;
#endif

    /* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    /* �ϱ�AT������ʼ */
    NAS_MMC_SndMmaPlmnSelectStartInd();

    pstUserSelReqMsg    = (MMA_MMC_PLMN_SPECIAL_REQ_STRU*)pstMsg;

    pstPlmnSelectionListInfo = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

#if (FEATURE_ON == FEATURE_LTE)
    enLteCapabilityStatus = NAS_MML_GetLteCapabilityStatus();
#endif

    /* ���������Ϣ */
    NAS_MMC_SaveCurEntryMsg(ulEventType, pstMsg);

    /* �����ֶ�������Ϣ */
    NAS_MMC_UpdateUserSpecPlmnSearchInfo_PlmnSelection((NAS_MML_PLMN_ID_STRU*)(&pstUserSelReqMsg->stPlmnId),
                                                       pstUserSelReqMsg->enAccessMode);

    /* ���ݳ�����ʼ�������б���Ϣ */
    NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_USER_SPEC_PLMN_SEARCH,
                                  VOS_NULL_PTR,
                                  pstPlmnSelectionListInfo);


    /* ѡ��״̬������ѡ���������NO RF��� */
    NAS_MMC_ClearNoRfInfo();

    if ( VOS_TRUE == NAS_MMC_IsNormalServiceStatus() )
    {
        /* SYSCFG���ú���Ҫ����������Ϊ���Ʒ���״̬���ϱ� */
        NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);
        NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_NOT_REG_PLMN_SEARCHING, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

        NAS_MMC_UpdateRegStateSpecPlmnSearch();
    }


    /* ��ǰδפ��,��������ʱ�������������,����״̬���˳����anycell���� */
    if (NAS_MMC_AS_CELL_NOT_CAMP_ON  == NAS_MMC_GetAsCellCampOn())
    {
        NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);
    }

    /* ��ȡ��ǰ���뼼�� */
    enNetRatType = NAS_MML_GetCurrNetRatType();

#if (FEATURE_ON == FEATURE_LTE)
    if ( VOS_TRUE == NAS_MMC_IsNeedEnableLte_PlmnSelection(NAS_MMC_PLMN_SEARCH_SCENE_USER_SPEC_PLMN_SEARCH) )
    {
        if ((VOS_FALSE == NAS_MML_GetDisableLteRoamFlg())
         && (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLteCapabilityStatus))
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

    /* �������Ӵ��� */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        /* ���ݵ�ǰ��ͬ�Ľ��뼼��,�����ͷ�����,Ǩ�Ƶ���ͬ�ĵȴ������ͷŵ�L2״̬��������������ʱ�� */
        NAS_MMC_SndRelReq_PlmnSelection(enNetRatType);

        return VOS_TRUE;
    }

    /* ��ȡ�¸�����ʧ�ܣ����˳�״̬�� */
    if (VOS_TRUE != NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
    {
        /* ��ӡ�쳣 */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTafPlmnSpecialReq_PlmnSelection_Init: Get Next Search Plmn fail at user spec search.");

        /* ��MMA��ָ������ʧ�� */
        NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_FAIL);


        /*���������ϢΪ��ͨ���������󣬺������µ��û�����ʱ�����Դ�ϸ�״̬��*/
        NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection();

        /* ���ϲ�״̬��ָʾѡ��ʧ�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

        /* �˳����״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ��MM��GMM��������ָʾ */
    NAS_MMC_SndGmmPlmnSchInit();
    NAS_MMC_SndMmPlmnSchInit();

    NAS_MMC_SndMmaCampOnInd(VOS_FALSE);


    /* �ж���Ҫ����������뼼���뵱ǰ���뼼���Ƿ���ͬ,���ߵ�ǰ���뼼��ΪButt,
       ��Ӧ�ĳ�����ѡ��״̬����������״̬���յ�Abort��Ϣ,�����ظ�SUSPNED CNF
       ��Ὣ��ǰ����ģʽ����ΪButt���˳�,��ʾ�������κν��뼼��ֱ�ӷ������� */
    if ((enNetRatType == stDestPlmn.enRat)
     || (NAS_MML_NET_RAT_TYPE_BUTT == enNetRatType))
    {
        /* ���ݻ�ȡ�Ľ��뼼�����õ�ǰ���뼼�� */
        NAS_MML_SetCurrNetRatType(stDestPlmn.enRat);

        /* �������LMM�����������󣬸��ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
    }
    else
    {
        /* �������LMM���͹������󣬸��ݵ�ǰ��ͬ�Ľ��뼼����Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSuspendReq_PlmnSelection(enNetRatType);
    }

    NAS_MMC_UpdateRegStateSpecPlmnSearch();


    return VOS_TRUE;
}
VOS_UINT32  NAS_MMC_RcvMmcMmcInterPlmnSearchReq_PlmnSelection_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_INTER_PLMN_SEARCH_REQ_STRU                     *pstPlmnSearchReq = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stDestPlmn;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enNetRatType;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;

    VOS_UINT32                                              ulIsNormalService;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enCampPlmnNetRat;
    NAS_MML_LAI_STRU                                       *pstLai;
    VOS_UINT32                                              ucSpecPlmnSearchFlg;
    VOS_UINT32                                              ulSingleDomainPlmnListFlg;


    VOS_UINT8                                               ucCsAttachAllowFlg;
    VOS_UINT8                                               ucPsAttachAllowFlg;


#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo           = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stCurPlmn;

    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;

    VOS_UINT32                                              ulIsCsPsMode1NeedPeriodSearchGU;

    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene;
    VOS_UINT8                                               ucEmcFlg;
    NAS_MMC_TIMER_STATUS_ENUM_U8                            enTimerStatus;

    /* ��ȡ��ǰפ����������Ϣ */
    pstCurCampInfo     = NAS_MML_GetCurrCampPlmnInfo();
    stCurPlmn.stPlmnId = pstCurCampInfo->stLai.stPlmnId;
    stCurPlmn.enRat    = pstCurCampInfo->enNetRatType;
    enLteCapabilityStatus = NAS_MML_GetLteCapabilityStatus();
#endif

    ucCsAttachAllowFlg = NAS_MML_GetCsAttachAllowFlg();
    ucPsAttachAllowFlg = NAS_MML_GetPsAttachAllowFlg();

    /* �µ�����:����ע��ʧ�ܷ����������Դ�ʱ,���ע��ʧ��ԭ��ֵ��#17,��Ҫ�������� */
    ulSingleDomainPlmnListFlg = NAS_MMC_IsSingleDomainRegFailNeedPlmnSrch(MMA_MMC_SRVDOMAIN_CS);
    ulSingleDomainPlmnListFlg |= NAS_MMC_IsSingleDomainRegFailNeedPlmnSrch(MMA_MMC_SRVDOMAIN_PS);

    /* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    /* �ϱ�AT������ʼ */
    NAS_MMC_SndMmaPlmnSelectStartInd();

    ucSpecPlmnSearchFlg = NAS_MMC_GetSpecPlmnSearchState();
    pstLai              = NAS_MML_GetCurrCampLai();
    enCampPlmnNetRat    = pstLai->enCampPlmnNetRat;

    pstPlmnSelectionListInfo = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    /* �ڲ�����������  */
    pstPlmnSearchReq = (NAS_MMC_INTER_PLMN_SEARCH_REQ_STRU*)pstMsg;

    /* ���������Ϣ */
    NAS_MMC_SaveCurEntryMsg(ulEventType, pstMsg);

    /* ���ݳ�����ʼ�������б���Ϣ */
    NAS_MMC_InitPlmnSelectionList(pstPlmnSearchReq->enPlmnSearchScene,
                                  &(pstPlmnSearchReq->astInterPlmnSearchInfo[0]),
                                  pstPlmnSelectionListInfo);


    /* ѡ��״̬������ѡ���������NO RF��� */
    NAS_MMC_ClearNoRfInfo();

    /* ��������������ͳһ������Ϣ��Я����������Ϣ���������б� */
    NAS_MMC_UpdatePlmnSearchInfo_PlmnSelection(pstPlmnSearchReq, pstPlmnSelectionListInfo);
#if (FEATURE_ON == FEATURE_LTE)
    /* ����CSFB��EMC���߻����EMC������ENABLE LTE��ʱ�������ڼ�,��ENABLE LTE */
    ucEmcFlg      = NAS_MML_GetCsEmergencyServiceFlg();
    enTimerStatus = NAS_MMC_GetTimerStatus(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER);
    if (( (MMC_LMM_DISABLE_LTE_REASON_EMERGENCY_CALL == NAS_MML_GetDisableLteReason())
       || (VOS_TRUE == ucEmcFlg))
     && (NAS_MMC_TIMER_STATUS_RUNING == enTimerStatus))
    {
        enPlmnSearchScene = NAS_MMC_PLMN_SEARCH_SCENE_DISABLE_LTE;
    }
    else
    {
        enPlmnSearchScene = pstPlmnSearchReq->enPlmnSearchScene;
    }
    if ( VOS_TRUE == NAS_MMC_IsNeedEnableLte_PlmnSelection(enPlmnSearchScene) )
    {
        if ((VOS_FALSE == NAS_MML_GetDisableLteRoamFlg())
         && (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLteCapabilityStatus))
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

    /*�жϵ�ǰ�Ƿ��Ѿ�פ����L�����ṩ��������
      ��Ӧ�ĳ�����CS/PS mode 1�ػ�LTE����������GU����*/
    ulIsCsPsMode1NeedPeriodSearchGU = NAS_MMC_IsCsPsMode1NeedPeriodSearchGU();

    if ((VOS_TRUE == NAS_MMC_IsCampLteNormalService())
     && (VOS_TRUE == ulIsCsPsMode1NeedPeriodSearchGU))
    {
        /* ���浱ǰ���絽״̬�������� */
        NAS_MMC_SetCsPsMode1ReCampLtePlmn_PlmnSelection(NAS_MML_GetCurrCampPlmnId());

        /* ����ѡ���б��е�ǰ����״̬Ϊ��ע�� */
        NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&stCurPlmn,
                                                       NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,
                                                       pstPlmnSelectionListInfo);
    }
#endif

    /* list����״̬���˳���������������NAS_MMC_PLMN_SEARCH_SCENE_USER_LIST��NAS_MMC_PLMN_SEARCH_SCENE_DISABLE_LTE*/
    if ((NAS_MMC_PLMN_SEARCH_SCENE_USER_LIST == pstPlmnSearchReq->enPlmnSearchScene)
     || (NAS_MMC_PLMN_SEARCH_SCENE_DISABLE_LTE == pstPlmnSearchReq->enPlmnSearchScene))
    {
        /* ���cs��ps�������Ʒ���list����������е�ǰ���磬�����ظ�������ǰ����*/
        ulIsNormalService = NAS_MMC_IsNormalServiceStatus();

        if (((VOS_FALSE == ulIsNormalService)
          || (VOS_TRUE  == ulSingleDomainPlmnListFlg))
         && (NAS_MMC_SPEC_PLMN_SEARCH_STOP == ucSpecPlmnSearchFlg))
        {
            /* svlte modem0 psǨ�Ƶ�modem1��g������פ����available��ʱ����ʱ��g���ڲ��б��Ѵ�����l�������g��ǰפ��
               ���磬�û��б��ѳ�����������ɾ����ǰפ��g�����磬��Ϊ�������ָ����l������ʧ�ܣ���Ҫ���»ص�g */
            if (!((VOS_TRUE == NAS_MML_GetSvlteSupportFlag())
              && (VOS_FALSE == ucCsAttachAllowFlg)
              && (VOS_FALSE == ucPsAttachAllowFlg)))
            {
                /* ɾ����ǰע�ᱻ�ܵ����� */
                NAS_MMC_DelSpecPlmnWithRatInPlmnList(NAS_MML_GetCurrCampPlmnId(),
                                                    enCampPlmnNetRat,
                                                    pstPlmnSelectionListInfo);
            }

        }

    }

    /* ��ǰδפ��,��������ʱ�������������,����״̬���˳����anycell���� */
    if (NAS_MMC_AS_CELL_NOT_CAMP_ON  == NAS_MMC_GetAsCellCampOn())
    {
        NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);
    }
    /* ��ȡ�¸�����ʧ�ܣ����˳�״̬�� */
    if (VOS_TRUE != NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
    {
        /* ��ӡ�쳣 */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvMmcMmcInterPlmnSearchReq_PlmnSelection_Init: Get Next Search Plmn fail at inter search.");

        if ( (VOS_TRUE == NAS_MMC_IsSorTriggerAdditionalLau())
          && (NAS_MMC_PLMN_SEARCH_SCENE_USER_LIST == pstPlmnSearchReq->enPlmnSearchScene) )
        {
            /* ���additional lau��Ϣ */
            NAS_MML_InitSorAdditionalLauCtx();
            
            /* LIST�ѽ�����֪ͨMM���ʹ���SOR��LAU */
            NAS_MMC_SndMmLauReq(MMC_MM_LAU_REQ_REASON_SOR);           
        }
        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();

        return VOS_TRUE;
    }

    /* ��ȡ��ǰ���뼼�� */
    enNetRatType = NAS_MML_GetCurrNetRatType();

    /*�������Ӵ���*/
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        /* ���ݵ�ǰ��ͬ�Ľ��뼼��,�����ͷ�����,Ǩ�Ƶ���ͬ�ĵȴ������ͷŵ�L2״̬��������������ʱ�� */
        NAS_MMC_SndRelReq_PlmnSelection(enNetRatType);

        return VOS_TRUE;
    }

    /* ��MM��GMM��������ָʾ */
    NAS_MMC_SndGmmPlmnSchInit();
    NAS_MMC_SndMmPlmnSchInit();

    NAS_MMC_SndMmaCampOnInd(VOS_FALSE);

    /* �ж���Ҫ����������뼼���뵱ǰ���뼼���Ƿ���ͬ,���ߵ�ǰ���뼼��ΪButt,
       ��Ӧ�ĳ�����ѡ��״̬����������״̬���յ�Abort��Ϣ,�����ظ�SUSPNED CNF
       ��Ὣ��ǰ����ģʽ����ΪButt���˳�,��ʾ�������κν��뼼��ֱ�ӷ������� */
    if ((enNetRatType == stDestPlmn.enRat)
     || (NAS_MML_NET_RAT_TYPE_BUTT == enNetRatType))
    {
        /* ���ݻ�ȡ�Ľ��뼼�����õ�ǰ���뼼�� */
        NAS_MML_SetCurrNetRatType(stDestPlmn.enRat);

        /* �������LMM�����������󣬸��ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
    }
    /*��Ҫ����ǰģʽ���л����ȴ�����״̬������������ʱ��*/
    else
    {
        /* �������LMM���͹������󣬸��ݵ�ǰ��ͬ�Ľ��뼼����Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSuspendReq_PlmnSelection(enNetRatType);
    }

    NAS_MMC_UpdateRegStateSpecPlmnSearch();

    return VOS_TRUE;

}


VOS_UINT32 NAS_MMC_RcvMmaAcqReq_PlmnSelection_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enNetRatType;
    NAS_MML_PLMN_WITH_RAT_STRU                              stDestPlmn;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    MMA_MMC_ACQ_REQ_STRU                                   *pstAcqReq = VOS_NULL_PTR;

    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    pstPlmnSelectionListInfo = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    /* �ϱ�AT������ʼ */
    NAS_MMC_SndMmaPlmnSelectStartInd();

    /* ���������Ϣ */
    NAS_MMC_SaveCurEntryMsg(ulEventType, pstMsg);

    pstAcqReq = (MMA_MMC_ACQ_REQ_STRU*)pstMsg;

    /* ���ݻ�ȡԭ����������б��ʼ�� */
    if (MMA_MMC_ACQ_REASON_POWER_UP == pstAcqReq->enAcqReason)
    {
        NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_SWITCH_ON,
                                      VOS_NULL_PTR,
                                      pstPlmnSelectionListInfo);
    }
    else if (MMA_MMC_ACQ_REASON_OOS == pstAcqReq->enAcqReason)
    {
        NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED,
                                      VOS_NULL_PTR,
                                      pstPlmnSelectionListInfo);
    }
    else
    {
        NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_PLMN_SEARCH,
                                      VOS_NULL_PTR,
                                      pstPlmnSelectionListInfo);
    }

    if (VOS_TRUE != NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
    {
        /* ��ӡ�쳣 */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvMmaAcqReq_PlmnSelection_Init: Get Next Search Plmn fail at inter search.");

        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();

        return VOS_TRUE;
    }

    /* ��ȡ��ǰ���뼼�� */
    enNetRatType = NAS_MML_GetCurrNetRatType();

    /*�������Ӵ���*/
    if ((VOS_TRUE == NAS_MML_IsRrcConnExist())
     && (NAS_MML_NET_RAT_TYPE_BUTT != enNetRatType))
    {
        /* ���ݵ�ǰ��ͬ�Ľ��뼼��,�����ͷ�����,Ǩ�Ƶ���ͬ�ĵȴ������ͷŵ�L2״̬��������������ʱ�� */
        NAS_MMC_SndRelReq_PlmnSelection(enNetRatType);

        return VOS_TRUE;
    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);

    /* ��MM��GMM��������ָʾ */
    NAS_MMC_SndGmmPlmnSchInit();
    NAS_MMC_SndMmPlmnSchInit();

    NAS_MMC_SndMmaCampOnInd(VOS_FALSE);

    /* �ж���Ҫ����������뼼���뵱ǰ���뼼���Ƿ���ͬ,���ߵ�ǰ���뼼��ΪButt,
       ��Ӧ�ĳ�����ѡ��״̬����������״̬���յ�Abort��Ϣ,�����ظ�SUSPNED CNF
       ��Ὣ��ǰ����ģʽ����ΪButt���˳�,��ʾ�������κν��뼼��ֱ�ӷ������� */
    if ((enNetRatType == stDestPlmn.enRat)
     || (NAS_MML_NET_RAT_TYPE_BUTT == enNetRatType))
    {
        /* ���ݻ�ȡ�Ľ��뼼�����õ�ǰ���뼼�� */
        NAS_MML_SetCurrNetRatType(stDestPlmn.enRat);

        /* �������LMM�����������󣬸��ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
    }
    /*��Ҫ����ǰģʽ���л����ȴ�����״̬������������ʱ��*/
    else
    {
        /* �������LMM���͹������󣬸��ݵ�ǰ��ͬ�Ľ��뼼����Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSuspendReq_PlmnSelection(enNetRatType);
    }

#if   (FEATURE_ON == FEATURE_LTE)
    /* ���������ȡ����Ҫ�ж�SIM������Disable Lte���� */
    if (MMA_MMC_ACQ_REASON_POWER_UP == pstAcqReq->enAcqReason)
    {
        NAS_MMC_DisableLteForSimReason_PlmnSelection();
    }
#endif

    NAS_MMC_UpdateRegStateSpecPlmnSearch();

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_PlmnSelection_WaitWasPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_PLMN_SEARCH_CNF);
    
    NAS_MMC_SetAbortFlag_PlmnSelection(VOS_TRUE);

    /* ֪ͨASָֹͣ������ */
    NAS_MMC_SndAsPlmnSrchStopReq(WUEPS_PID_WRR);

    /*���ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������ͬ������ʱ��*/
    NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_WAS_PLMN_STOP_CNF);

    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvRrMmRelInd_PlmnSelection_WaitWasPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulResult;
    NAS_MML_PLMN_WITH_RAT_STRU          stDestPlmn;

    /* �����ж�ԭ��ֵ */

	/* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmRelInd_PlmnSelection_WaitWasPlmnSearchCnf: ENTERED");

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_PLMN_SEARCH_CNF);


    /* ��ȡ��һ��Ҫ����������,����ȡʧ�ܣ�����ϲ�״̬���ϱ�����ʧ�ܣ��˳�
       ����ȡ�ɹ������ж���һ��Ҫ�����������Ƿ��뵱ǰפ��������ͬ��
       ����ͬ���ڵ�ǰģʽ�����������������ǰ���� */
    ulResult = NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn);

    /*  ��ȡ�¸�����ʧ�ܣ�����г����������� */
    if (VOS_TRUE != ulResult)
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();

        return VOS_TRUE;
    }


    /*��һ��Ҫ�����������Ƿ��뵱ǰפ�����粻ͬ,��Ҫ����ǰģʽ���л����ȴ�����״̬������������ʱ�� */
    if (stDestPlmn.enRat != NAS_MML_GetCurrNetRatType())
    {
        NAS_MMC_SndSuspendReq_PlmnSelection(NAS_MML_GetCurrNetRatType());
    }
    else
    {
        /* �������LMM�����������󣬸��ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
    }

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_PlmnSelection_WaitGasPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_PLMN_SEARCH_CNF);
    
    NAS_MMC_SetAbortFlag_PlmnSelection(VOS_TRUE);

    /* ֪ͨASָֹͣ������ */
    NAS_MMC_SndAsPlmnSrchStopReq(UEPS_PID_GAS);

    NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_GAS_PLMN_STOP_CNF);

    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_GAS_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvRrMmPlmnSrchCnf_PlmnSelection_WaitWasPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_PLMN_SEARCH_CNF_STRU                              *pstSrchCnfMsg = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stDestPlmn;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    stPlmnSearchInfo;

    pstSrchCnfMsg = (RRMM_PLMN_SEARCH_CNF_STRU *)pstMsg;

	/* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_PLMN_SEARCH_CNF);

    /* ���µ�ǰģʽ�����縲����Ϣ */
    NAS_MMC_UpdateGUCoverageFlg_PlmnSelection(pstSrchCnfMsg);

    if (RRC_PLMN_SEARCH_RLT_SPEC == pstSrchCnfMsg->ulSearchRlt)
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_SUCCESS,
                VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        /* ���ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬������������ʱ�� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_WAS_SYSINFO_IND);

        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_SYS_INFO, TI_NAS_MMC_WAIT_WAS_SYS_INFO_LEN);

        return VOS_TRUE;
    }


    /* �����ǰΪNO RF����ֱ����״̬�� */
    if (RRC_PLMN_SEARCH_RLT_SPEC_NO_RF == pstSrchCnfMsg->ulSearchRlt)
    {
        NAS_MMC_ProcPlmnSearchNoRfFail_PlmnSelection();
        
        return VOS_TRUE;
    }

    /* ���õ�ǰ���뼼��������ȫƵ���� */
    NAS_MMC_SetAllBandSearch_PlmnSelection(NAS_MML_NET_RAT_TYPE_WCDMA, VOS_TRUE);

    /* ��������Ϣ��Я����������Ϣת��Ϊ�ڲ���Ϣ��������Ϣ */
    NAS_MMC_ConvertRrcPlmnList2SearchedPlmnListInfo(NAS_MML_NET_RAT_TYPE_WCDMA,
                                                     &pstSrchCnfMsg->PlmnIdList,
                                                     &stPlmnSearchInfo);

    /* ������Ϣ��Я����������Ϣ���������б� */
    NAS_MMC_UpdatePlmnSearchList_PlmnSelection(&stPlmnSearchInfo);

    /* ����������һ����Ҫ����������ʱ�������������������򣬽�������פ�� */
    if (VOS_TRUE == NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
    {
        if (NAS_MML_NET_RAT_TYPE_WCDMA == stDestPlmn.enRat)
        {
            /* ��W����㷢����������Ǩ�Ƶ���Ӧ��L2״̬��������������ʱ�� */
            NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
        }
        else
        {
            /* ��W����㷢�͹�������Ǩ�Ƶ���Ӧ��L2״̬��������������ʱ�� */
            NAS_MMC_SndSuspendReq_PlmnSelection(NAS_MML_NET_RAT_TYPE_WCDMA);
        }
    }
    else
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();
    }

    return VOS_TRUE;

}
VOS_UINT32 NAS_MMC_RcvRrMmPlmnSrchCnf_PlmnSelection_WaitGasPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_PLMN_SEARCH_CNF_STRU                              *pstSrchCnfMsg = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stDestPlmn;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    stPlmnSearchInfo;

    pstSrchCnfMsg = (RRMM_PLMN_SEARCH_CNF_STRU *)pstMsg;

	/* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_PLMN_SEARCH_CNF);

    /* ���õ�ǰģʽ�Ƿ����ȫƵ�������Ƿ�������縲�� */
    NAS_MMC_UpdateGUCoverageFlg_PlmnSelection(pstSrchCnfMsg);

    if (RRC_PLMN_SEARCH_RLT_SPEC == pstSrchCnfMsg->ulSearchRlt)
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_SUCCESS,
                VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        /* ���ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬������������ʱ�� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_GAS_SYSINFO_IND);

        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_SYS_INFO, TI_NAS_MMC_WAIT_GAS_SYS_INFO_LEN);

        return VOS_TRUE;
    }


    /* �����ǰΪNO RF����ֱ����״̬�� */
    if (RRC_PLMN_SEARCH_RLT_SPEC_NO_RF == pstSrchCnfMsg->ulSearchRlt)
    {
        NAS_MMC_ProcPlmnSearchNoRfFail_PlmnSelection();
        
        return VOS_TRUE;
    }

    /* ���µ�ǰ���뼼��������ȫƵ���� */
    NAS_MMC_SetAllBandSearch_PlmnSelection(NAS_MML_NET_RAT_TYPE_GSM, VOS_TRUE);

    /* ��������Ϣ��Я����������Ϣת��Ϊ�ڲ���Ϣ��������Ϣ */
    NAS_MMC_ConvertRrcPlmnList2SearchedPlmnListInfo(NAS_MML_NET_RAT_TYPE_GSM,
                                                     &pstSrchCnfMsg->PlmnIdList,
                                                     &stPlmnSearchInfo);

    /* ������Ϣ��Я����������Ϣ���������б� */
    NAS_MMC_UpdatePlmnSearchList_PlmnSelection(&stPlmnSearchInfo);

    /* ����������һ����Ҫ����������ʱ�������������������򣬽�������פ�� */
    if (VOS_TRUE == NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
    {
        if (NAS_MML_NET_RAT_TYPE_GSM == stDestPlmn.enRat)
        {
            /* ��G����㷢����������Ǩ�Ƶ���Ӧ��L2״̬��������������ʱ�� */
            NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
        }
        else
        {
            /* ��G����㷢�͹�������Ǩ�Ƶ���Ӧ��L2״̬��������������ʱ�� */
            NAS_MMC_SndSuspendReq_PlmnSelection(NAS_MML_NET_RAT_TYPE_GSM);
        }
    }
    else
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();
    }

    return VOS_TRUE;

}

VOS_UINT32 NAS_MMC_RcvTiWaitWasPlmnSearchCnfExpired_PlmnSelection_WaitWasPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{   
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    stPlmnSearchInfo;


    PS_MEM_SET(&stPlmnSearchInfo, 0x00, sizeof(stPlmnSearchInfo));


    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitWasPlmnSearchCnfExpired_PlmnSelection_WaitWasPlmnSearchCnf: Timer Expired");

    /* ���õ�ǰ�����Ѿ�����ȫƵ���� */
    NAS_MMC_SetAllBandSearch_PlmnSelection(NAS_MML_NET_RAT_TYPE_WCDMA, VOS_TRUE);

    /* ���õ�ǰ�����޸��� */
    NAS_MMC_SetRatCoverage_PlmnSelection(NAS_MML_NET_RAT_TYPE_WCDMA, NAS_MMC_COVERAGE_TYPE_NONE);

    /* ���õ�ǰ���뼼��û���������κ����� */
    stPlmnSearchInfo.ulHighPlmnNum = 0x0;
    stPlmnSearchInfo.ulLowPlmnNum  = 0x0;
    stPlmnSearchInfo.enRatType     = NAS_MML_NET_RAT_TYPE_WCDMA;

    /* ���������б� */
    NAS_MMC_UpdatePlmnSearchList_PlmnSelection(&stPlmnSearchInfo);
    
    /* ֪ͨASָֹͣ������ */
    NAS_MMC_SndAsPlmnSrchStopReq(WUEPS_PID_WRR);

    /*���ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������ͬ������ʱ��*/
    NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_WAS_PLMN_STOP_CNF);

    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}

VOS_UINT32 NAS_MMC_RcvTiWaitGasPlmnSearchCnfExpired_PlmnSelection_WaitGasPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    stPlmnSearchInfo;


    PS_MEM_SET(&stPlmnSearchInfo, 0x00, sizeof(stPlmnSearchInfo));


    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitGasPlmnSearchCnfExpired_PlmnSelection_WaitGasPlmnSearchCnf: Timer Expired");

    /* ���õ�ǰ�����Ѿ�����ȫƵ���� */
    NAS_MMC_SetAllBandSearch_PlmnSelection(NAS_MML_NET_RAT_TYPE_GSM, VOS_TRUE);

    /* ���õ�ǰ�����޸��� */
    NAS_MMC_SetRatCoverage_PlmnSelection(NAS_MML_NET_RAT_TYPE_GSM, NAS_MMC_COVERAGE_TYPE_NONE);

    /* ���õ�ǰ���뼼��û���������κ����� */
    stPlmnSearchInfo.ulHighPlmnNum = 0x0;
    stPlmnSearchInfo.ulLowPlmnNum  = 0x0;
    stPlmnSearchInfo.enRatType     = NAS_MML_NET_RAT_TYPE_GSM;

    /* ������Ϣ��Я����������Ϣ���������б� */
    NAS_MMC_UpdatePlmnSearchList_PlmnSelection(&stPlmnSearchInfo);
    
    NAS_MMC_SndAsPlmnSrchStopReq(UEPS_PID_GAS);

    NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_GAS_PLMN_STOP_CNF);

    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_GAS_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_PlmnSelection_WaitAsSuspendCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��¼��Ҫ��ֹ��ǰ״̬����־ */
    NAS_MMC_SetAbortFlag_PlmnSelection(VOS_TRUE);

    return VOS_TRUE;
}

VOS_UINT32 NAS_MMC_RcvRrMmSuspendCnf_PlmnSelection_WaitWasSuspendCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SUSPEND_CNF_ST                *pstSuspendCnf = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU          stDestPlmn;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurRat;

    pstSuspendCnf = (RRMM_SUSPEND_CNF_ST*)pstMsg;

    /* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    enCurRat      = NAS_MML_GetCurrNetRatType();

    /* ֹͣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_SUSPEND_CNF);

    if (MMC_AS_SUSPEND_FAILURE == pstSuspendCnf->ucResult)
    {
        /* ���ɻָ�����,���õ���ӿ�RESET */

        if ( (NAS_UTRANCTRL_UTRAN_MODE_FDD == NAS_UTRANCTRL_GetCurrUtranMode())
          && (NAS_MML_NET_RAT_TYPE_WCDMA == enCurRat) )
        {
            NAS_MML_SoftReBoot();
        }
        else
        {
            NAS_MML_SoftReBoot();
        }

        return VOS_TRUE;
    }

    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* ���õ�ǰ���뼼��ΪBUTT */
        NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_BUTT);

        /* �ظ�Plmn Selectionִ�н�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ��ȡ��Ҫ���������缰����뼼�� */
    if (VOS_TRUE == NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
    {
        /* ���ݻ�ȡ�Ľ��뼼�����õ�ǰ���뼼�� */
        NAS_MML_SetCurrNetRatType(stDestPlmn.enRat);

        /* �������LMM�����������󣬸��ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
    }
    else
    {
        /* Error�����ӡ */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmSuspendCnf_PlmnSelection_WaitWasSuspendCnf: Get next search plmn fail.");

        NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_BUTT);

        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();
    }

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvRrMmSuspendCnf_PlmnSelection_WaitGasSuspendCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SUSPEND_CNF_ST                *pstSuspendCnf = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU          stDestPlmn;

    pstSuspendCnf = (RRMM_SUSPEND_CNF_ST*)pstMsg;

    /* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    /* ֹͣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_SUSPEND_CNF);

    if (MMC_AS_SUSPEND_FAILURE == pstSuspendCnf->ucResult)
    {
        /* ���ɻָ�����,���õ���ӿ�RESET */

        NAS_MML_SoftReBoot();

        return VOS_TRUE;
    }

    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* ���õ�ǰ���뼼��ΪBUTT */
        NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_BUTT);

        /* �����������,ԭ��Ϊ�˳�״̬�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }


    /* ��ȡ��Ҫ���������缰����뼼�� */
    if (VOS_TRUE == NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
    {
        /* ���ݻ�ȡ�Ľ��뼼�����õ�ǰ���뼼�� */
        NAS_MML_SetCurrNetRatType(stDestPlmn.enRat);

        /* �������LMM�����������󣬸��ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
    }
    else
    {
        /* Error�����ӡ */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmSuspendCnf_PlmnSelection_WaitGasSuspendCnf: Get next search plmn fail.");

        NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_BUTT);

        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();
    }

    return VOS_TRUE;
}

VOS_UINT32 NAS_MMC_RcvTiWaitAsSuspendCnfExpired_PlmnSelection_WaitAsSuspendCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurRat;

    enCurRat      = NAS_MML_GetCurrNetRatType();

    /* ���ɻָ����ϣ���λ */

    if ( (NAS_UTRANCTRL_UTRAN_MODE_FDD == NAS_UTRANCTRL_GetCurrUtranMode())
      && (NAS_MML_NET_RAT_TYPE_WCDMA == enCurRat) )
    {
        NAS_MML_SoftReBoot();
    }
    else if ( NAS_MML_NET_RAT_TYPE_GSM == enCurRat )
    {
        NAS_MML_SoftReBoot();
    }
    else
    {
        NAS_MML_SoftReBoot();
    }

    return VOS_TRUE;
}

VOS_UINT32 NAS_MMC_RcvRrMmPlmnSearchStopCnf_PlmnSelection_WaitWasPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{   
    NAS_MML_PLMN_WITH_RAT_STRU                              stDestPlmn;

    /* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));
    
    NAS_MMC_StopTimer( TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF );

    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����������һ����Ҫ����������ʱ�������������������򣬽�������פ�� */
    if (VOS_TRUE == NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
    {
        if (NAS_MML_NET_RAT_TYPE_WCDMA == stDestPlmn.enRat)
        {
            /* ��W����㷢����������Ǩ�Ƶ���Ӧ��L2״̬��������������ʱ�� */
            NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
        }
        else
        {
            /* ��W����㷢�͹�������Ǩ�Ƶ���Ӧ��L2״̬��������������ʱ�� */
            NAS_MMC_SndSuspendReq_PlmnSelection(NAS_MML_NET_RAT_TYPE_WCDMA);
        }
    }
    else
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();
    }
    
    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvRrMmRelInd_PlmnSelection_WaitWasPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �����ж�ԭ��ֵ */

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmRelInd_PlmnSelection_WaitWasPlmnStopCnf: ENTERED");

    /* ֹͣ������ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_PLMN_STOP_CNF);

    NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}

VOS_UINT32 NAS_MMC_RcvRrMmPlmnSearchStopCnf_PlmnSelection_WaitGasPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_WITH_RAT_STRU                              stDestPlmn;

    /* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));
    
    NAS_MMC_StopTimer( TI_NAS_MMC_WAIT_GAS_PLMN_STOP_CNF );

    /* �д��ʱ���˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����������һ����Ҫ����������ʱ�������������������򣬽�������פ�� */
    if (VOS_TRUE == NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
    {
        if (NAS_MML_NET_RAT_TYPE_GSM == stDestPlmn.enRat)
        {
            /* ��G����㷢����������Ǩ�Ƶ���Ӧ��L2״̬��������������ʱ�� */
            NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
        }
        else
        {
            /* ��G����㷢�͹�������Ǩ�Ƶ���Ӧ��L2״̬��������������ʱ�� */
            NAS_MMC_SndSuspendReq_PlmnSelection(NAS_MML_NET_RAT_TYPE_GSM);
        }
    }
    else
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_SUCCESS,
                VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();
    }


    return VOS_TRUE;
}

VOS_UINT32 NAS_MMC_RcvTiWaitWasStopCnfExpired_PlmnSelection_WaitWasPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{   
    NAS_MML_PLMN_WITH_RAT_STRU                              stDestPlmn;

    /* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));
    
    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitWasStopCnfExpired_PlmnSelection_WaitWasPlmnStopCnf: Timer Expired");

    /* �д��ʱ���˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }
    
    /* ����������һ����Ҫ����������ʱ�������������������򣬽�������פ�� */
    if (VOS_TRUE == NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
    {
        if (NAS_MML_NET_RAT_TYPE_WCDMA == stDestPlmn.enRat)
        {
            /* ��W����㷢����������Ǩ�Ƶ���Ӧ��L2״̬��������������ʱ�� */
            NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
        }
        else
        {
            /* ��W����㷢�͹�������Ǩ�Ƶ���Ӧ��L2״̬��������������ʱ�� */
            NAS_MMC_SndSuspendReq_PlmnSelection(NAS_MML_NET_RAT_TYPE_WCDMA);
        }
    }
    else
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();
    }

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvTiWaitGasStopCnfExpired_PlmnSelection_WaitGasPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{   
    NAS_MML_PLMN_WITH_RAT_STRU                              stDestPlmn;

    /* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));
    
    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitGasStopCnfExpired_PlmnSelection_WaitGasPlmnStopCnf: Timer Expired");

    /* �д��ʱ���˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����������һ����Ҫ����������ʱ�������������������򣬽�������פ�� */
    if (VOS_TRUE == NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
    {
        if (NAS_MML_NET_RAT_TYPE_GSM == stDestPlmn.enRat)
        {
            /* ��G����㷢����������Ǩ�Ƶ���Ӧ��L2״̬��������������ʱ�� */
            NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
        }
        else
        {
            /* ��G����㷢�͹�������Ǩ�Ƶ���Ӧ��L2״̬��������������ʱ�� */
            NAS_MMC_SndSuspendReq_PlmnSelection(NAS_MML_NET_RAT_TYPE_GSM);
        }
    }
    else
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_SUCCESS,
                VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();
    }

    return VOS_TRUE;
}

/************************ ע�Ჿ��*************************************/

VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_PlmnSelection_WaitWasSysInfoInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    NAS_MMC_SetAbortFlag_PlmnSelection(VOS_TRUE);

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_PlmnSelection_WaitGasSysInfoInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    NAS_MMC_SetAbortFlag_PlmnSelection(VOS_TRUE);

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvSysInfoInd_PlmnSelection_WaitWasSysInfoInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo           = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;
    NAS_MMC_FSM_ID_ENUM_UINT32                              enCurrentFsmId;
    VOS_UINT32                                              ulCurrentEventType;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;

    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8                enSpecPlmnSearchState;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;

    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();

#endif

    /* ֹͣ�ȴ�ϵͳ��Ϣ�Ķ�ʱ��  */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_SYS_INFO);

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg,enLCapabilityStatus);
#endif

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* �ظ�Plmn Selectionִ�н�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_WasSysInfo(pstMsg);

    /* פ��С���仯����������ʱ���ϱ�ϵͳ��Ϣ */
    enSpecPlmnSearchState = NAS_MMC_GetSpecPlmnSearchState();

    if ((VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
     || (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING == enSpecPlmnSearchState))
    {
        /* �����ϱ� */
        NAS_MMC_SndMmaSysInfo();

    }

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMMAģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMmaAcInfoChangeInd(MMA_MMC_SRVDOMAIN_CS, pstCurCsRestrictInfo);
    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMmaAcInfoChangeInd(MMA_MMC_SRVDOMAIN_PS, pstCurPsRestrictInfo);
    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_WCDMA);
#endif

    NAS_MMC_SndMmaCampOnInd(VOS_TRUE);

    /* ���µ�ǰ�ȴ�CSPS��ע���� */
    NAS_MMC_ClearAllWaitRegRsltFlag_PlmnSelection();
    NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_CS);
    NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* ����CSPS��AdditionalAction */
    NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_BUTT);
    NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_BUTT);

    /* ����CSPS��RegCause */
    NAS_MMC_SetCsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_BUTT);
    NAS_MMC_SetPsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_BUTT);

    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }

    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();

    if ( VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType) )
    {
        /* ת��ϵͳ��Ϣ֪ͨ �� MM/GMM */
        NAS_MMC_SndMmWasSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenTypeForUserSpecPlmnSrch(),
                                   VOS_TRUE,
                                   pstMsg);

        NAS_MMC_SndGmmWasSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenTypeForUserSpecPlmnSrch(),
                                   VOS_TRUE,
                                   pstMsg);
    }
    else
    {
        /* ת��ϵͳ��Ϣ֪ͨ �� MM/GMM,������ǰϵͳ��Ϣ�� Forbidden ����Я����ȥ */
        NAS_MMC_SndMmWasSysInfoInd(NAS_MML_GetLaiForbType(&(pstCurCampInfo->stLai)),
                                   VOS_FALSE,
                                   pstMsg);

        NAS_MMC_SndGmmWasSysInfoInd(NAS_MML_GetLaiForbType(&(pstCurCampInfo->stLai)),
                                   VOS_FALSE,
                                   pstMsg);        
    }

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmWcdmaSysInfoInd(pstMsg);
#endif

    /* �յ�ϵͳ��Ϣ��Ǩ�Ƶ��ȴ�CS+PS���ע������������ʱ�� */
    NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_CSPS_REG_IND);
    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND, TI_NAS_MMC_WAIT_CSPS_REG_IND_LEN);

    return VOS_TRUE;
}




VOS_UINT32 NAS_MMC_RcvSysInfoInd_PlmnSelection_WaitGasSysInfoInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;
    NAS_MMC_FSM_ID_ENUM_UINT32                              enCurrentFsmId;
    VOS_UINT32                                              ulCurrentEventType;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;

    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8                enSpecPlmnSearchState;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;


    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();

#endif

    /* ����յ�GAS��ϵͳ��Ϣ����ĿǰUtran��Enable��������Disable Utran */
    NAS_MMC_RcvGasSysInfoSetUtranCapabilityStatus();

    /* ֹͣ�ȴ�ϵͳ��Ϣ�Ķ�ʱ��  */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_SYS_INFO);

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg, enLCapabilityStatus);
#endif

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {

        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_GasSysInfo(pstMsg);

    /* פ��С���仯����������ʱ���ϱ�ϵͳ��Ϣ */
    enSpecPlmnSearchState = NAS_MMC_GetSpecPlmnSearchState();

    if ((VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
     || (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING == enSpecPlmnSearchState))
    {
        /* �����ϱ� */
        NAS_MMC_SndMmaSysInfo();
    }

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMMAģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMmaAcInfoChangeInd(MMA_MMC_SRVDOMAIN_CS, pstCurCsRestrictInfo);
    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMmaAcInfoChangeInd(MMA_MMC_SRVDOMAIN_PS, pstCurPsRestrictInfo);
    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_GSM);
#endif

    NAS_MMC_SndMmaCampOnInd(VOS_TRUE);

    /* ���µ�ǰ�ȴ�CSPS��ע���� */
    NAS_MMC_ClearAllWaitRegRsltFlag_PlmnSelection();
    NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_CS);
    NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* ����CSPS��AdditionalAction */
    NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_BUTT);
    NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_BUTT);

    /* ����CSPS��RegCause */
    NAS_MMC_SetCsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_BUTT);
    NAS_MMC_SetPsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_BUTT);

    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();


    /* ���û�ָ������ʱ������ҪЯ��FORB���� */
    if ( VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType) )
    {
        /* ת��ϵͳ��Ϣ֪ͨ �� MM/GMM */
        NAS_MMC_SndMmGsmSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenTypeForUserSpecPlmnSrch(),
                                   VOS_TRUE,
                                   pstMsg);

        NAS_MMC_SndGmmGsmSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenTypeForUserSpecPlmnSrch(),
                                   VOS_TRUE,
                                   pstMsg);
    }
    else
    {
        /* ת��ϵͳ��Ϣ֪ͨ �� MM/GMM,������ǰϵͳ��Ϣ�� Forbidden ����Я����ȥ */
        NAS_MMC_SndMmGsmSysInfoInd(NAS_MML_GetLaiForbType(&(pstCurCampInfo->stLai)),
                                   VOS_FALSE,
                                   pstMsg);

        NAS_MMC_SndGmmGsmSysInfoInd(NAS_MML_GetLaiForbType(&(pstCurCampInfo->stLai)),
                                   VOS_FALSE,
                                   pstMsg);
        
    }

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmGsmSysInfoInd(pstMsg);
#endif

    /* �յ�ϵͳ��Ϣ��Ǩ�Ƶ��ȴ�CS+PS���ע������������ʱ�� */
    NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_CSPS_REG_IND);

    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND, TI_NAS_MMC_WAIT_CSPS_REG_IND_LEN);


    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvTiWaitSysInfoExpired_PlmnSelection_WaitWasSysInfoInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_WITH_RAT_STRU          stDestPlmn;
    VOS_UINT32                          ulResult;

	/* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitSysInfoExpired_PlmnSelection_WaitWasSysInfoInd: Timer Expired");

    /*�����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE  == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

       /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());

    /*��ȡ��һ��Ҫ����������,����ȡʧ�ܣ�����ϲ�״̬���ϱ�����ʧ�ܣ��˳�
      ����ȡ�ɹ������ж���һ��Ҫ�����������Ƿ��뵱ǰפ��������ͬ��
      ����ͬ���ڵ�ǰģʽ�����������������ǰ����
     */
    ulResult = NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn);

    /*  ��ȡ�¸�����ʧ�ܣ�����г����������� */
    if (VOS_TRUE != ulResult)
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();

        return VOS_TRUE;
    }

    /* ��MM��GMM��������ָʾ */
    NAS_MMC_SndGmmPlmnSchInit();
    NAS_MMC_SndMmPlmnSchInit();

    NAS_MMC_SndMmaCampOnInd(VOS_FALSE);

    NAS_MMC_UpdateRegStateSpecPlmnSearch();

    /*��һ��Ҫ�����������Ƿ��뵱ǰפ�����粻ͬ,��Ҫ����ǰģʽ���л����ȴ�����״̬������������ʱ�� */
    if (stDestPlmn.enRat != NAS_MML_GetCurrNetRatType())
    {
        NAS_MMC_SndSuspendReq_PlmnSelection(NAS_MML_GetCurrNetRatType());
    }
    else
    {
        /* �������LMM�����������󣬸��ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
    }

    return VOS_TRUE;
}



VOS_UINT32  NAS_MMC_RcvTiWaitSysInfoExpired_PlmnSelection_WaitGasSysInfoInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_WITH_RAT_STRU          stDestPlmn;
    VOS_UINT32                          ulResult;

	/* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));
    
    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitSysInfoExpired_PlmnSelection_WaitGasSysInfoInd: Timer Expired");

    /*�����ǰ��Ҫ�˳�״̬��*/
    if (VOS_TRUE  == NAS_MMC_GetAbortFlag_PlmnSelection())
    {

        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());

    /*��ȡ��һ��Ҫ����������,����ȡʧ�ܣ�����ϲ�״̬���ϱ�����ʧ�ܣ��˳�
      ����ȡ�ɹ������ж���һ��Ҫ�����������Ƿ��뵱ǰפ��������ͬ��
      ����ͬ���ڵ�ǰģʽ�����������������ǰ����
     */
    ulResult = NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn);

    /*  ��ȡ�¸�����ʧ�ܣ�����г�����������*/
    if (VOS_TRUE != ulResult)
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();

        return VOS_TRUE;
    }

    /* ��MM��GMM��������ָʾ */
    NAS_MMC_SndGmmPlmnSchInit();
    NAS_MMC_SndMmPlmnSchInit();

    NAS_MMC_SndMmaCampOnInd(VOS_FALSE);

    NAS_MMC_UpdateRegStateSpecPlmnSearch();

    /*��һ��Ҫ�����������Ƿ��뵱ǰפ�����粻ͬ,��Ҫ����ǰģʽ���л����ȴ�����״̬������������ʱ��*/
    if (stDestPlmn.enRat != NAS_MML_GetCurrNetRatType())
    {
        NAS_MMC_SndSuspendReq_PlmnSelection(NAS_MML_GetCurrNetRatType());
    }
    else
    {
        /* �������LMM�����������󣬸��ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;
    VOS_UINT8                           ucWaitRegFlag;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;


    /* ������������û�����Ĺػ�����ֱ���˳���ע����״̬�� */
    if (NAS_BuildEventType(WUEPS_PID_MMA, ID_MMA_MMC_POWER_OFF_REQ) == pstAbortMsg->ulEventType)
    {
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);

        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    ucWaitRegFlag = NAS_MMC_GetWaitRegRsltFlag_PlmnSelection();

    /* ��ǰû�����ӿ���ֱ���˳�����Ҫָcs/psע�ᱻ�ܣ���cause17,��Ҫ�����ȴ�ע��������� */
    if (( VOS_FALSE == NAS_MML_IsRrcConnExist())
     || (NAS_MMC_WAIT_REG_RESULT_IND_NULL == ucWaitRegFlag))
    {

        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);

        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����Abort��־ */
    NAS_MMC_SetAbortFlag_PlmnSelection(VOS_TRUE);

    /* �����Ҫ�����˳� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == pstAbortMsg->enAbortFsmType)
    {
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);
        NAS_MMC_SndRelReq_PlmnSelection(NAS_MML_GetCurrNetRatType());
    }

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvGmmPsRegResultInd_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    GMMMMC_PS_REG_RESULT_IND_STRU      *pstPsRegRslt = VOS_NULL_PTR;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsRegCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsRegCause;
    NAS_MMC_FSM_ID_ENUM_UINT32          enCurrentFsmId;
    VOS_UINT32                          ulCurrentEventType;

    enCurrentFsmId      = NAS_MMC_GetCurrFsmId();
    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    ulCurrentEventType  = NAS_MMC_GetCurrFsmEventType();

    pstPsRegRslt        = (GMMMMC_PS_REG_RESULT_IND_STRU*)pstMsg;

    /* ���CCOʧ�ܻ��˹������յ�����ϵͳ��ע����������Ҫ���� */
    if (pstPsRegRslt->enNetType != NAS_MML_GetCurrNetRatType())
    {
        return VOS_TRUE;
    }

    /* ����ȴ�PSע����mask */
    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* ����ע��������Ӧ����: ���÷���״̬����¼Causeֵ��������һ���������ͣ�֪ͨ��Ӧ��� */
    NAS_MMC_ProcPsRegRslt_PlmnSelection(pstPsRegRslt);

    if (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType))
    {
        enCsRegCause = NAS_MMC_GetCsRegCause_PlmnSelection();
        enPsRegCause = NAS_MMC_GetPsRegCause_PlmnSelection();

        if (VOS_TRUE == NAS_MMC_ProcUserSearchRegRslt_PlmnSelection(enCsRegCause,enPsRegCause))
        {
            /* ���������ϢΪ��ͨ���������󣬺������µ��û�����ʱ�����Դ�ϸ�״̬�� */
            NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection();

            /* �û�ָ������ɺ���Ҫ��MMLȫ�ֱ����еĽ�ֹ����LA��Ϣ֪ͨ�����*/
            NAS_MMC_SndRrMmCellSelReq(RRC_NAS_FORBLA_CHANGE);
        }
    }

    if (NAS_MMC_WAIT_REG_RESULT_IND_NULL != NAS_MMC_GetWaitRegRsltFlag_PlmnSelection())
    {
        return VOS_TRUE;
    }

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* ���ϲ�״̬������������� ABORT */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        if (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType))
        {
            NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_FAIL);

#if   (FEATURE_ON == FEATURE_LTE)
            /* ��LMM�����û���������֪ͨ */
            if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
            {
                NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
            }
#endif
        }

        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    NAS_MMC_PerformAdditionalActionRegFinished_PlmnSelection();

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvMmCsRegResultInd_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMMMC_CS_REG_RESULT_IND_STRU       *pstCsRegRslt = VOS_NULL_PTR;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsRegCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsRegCause;
    NAS_MMC_FSM_ID_ENUM_UINT32          enCurrentFsmId;
    VOS_UINT32                          ulCurrentEventType;

    pstCsRegRslt = (MMMMC_CS_REG_RESULT_IND_STRU*)pstMsg;

    /* ���CCOʧ�ܻ��˹������յ�����ϵͳ��ע����������Ҫ���� */
    if (pstCsRegRslt->enNetType != NAS_MML_GetCurrNetRatType())
    {
        return VOS_TRUE;
    }

    /* ����ȴ�CSע����mask */
    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_CS);

    /* ����ע��������Ӧ����: ���÷���״̬����¼Causeֵ��������һ���������ͣ�֪ͨ��Ӧ��� */
    NAS_MMC_ProcCsRegRslt_PlmnSelection(pstCsRegRslt);

    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();
    if (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType))
    {
        enCsRegCause = NAS_MMC_GetCsRegCause_PlmnSelection();
        enPsRegCause = NAS_MMC_GetPsRegCause_PlmnSelection();

        if (VOS_TRUE == NAS_MMC_ProcUserSearchRegRslt_PlmnSelection(enCsRegCause,enPsRegCause))
        {

            /* ��GSM�£����CSע��ɹ���PSδע�ᣬ��ɾ��FORB GPRS��Ϣ */
            NAS_MMC_ProcUserSpecPlmnSrchForbGprsInfo_PlmnSelection();            
                
            /*���������ϢΪ��ͨ���������󣬺������µ��û�����ʱ�����Դ�ϸ�״̬��*/
            NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection();

            /* �û�ָ������ɺ���Ҫ��MMLȫ�ֱ����еĽ�ֹ����LA��Ϣ֪ͨ�����*/
            NAS_MMC_SndRrMmCellSelReq(RRC_NAS_FORBLA_CHANGE);
        }
    }

    if (NAS_MMC_WAIT_REG_RESULT_IND_NULL != NAS_MMC_GetWaitRegRsltFlag_PlmnSelection())
    {
        return VOS_TRUE;
    }

    /*�����ǰ��Ҫ�˳�״̬��*/
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {

        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    NAS_MMC_PerformAdditionalActionRegFinished_PlmnSelection();

    return VOS_TRUE;

}
VOS_UINT32 NAS_MMC_RcvTiWaitCsPsRegRsltIndExpired_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enPsAdditionalAction;
    NAS_MMC_FSM_ID_ENUM_UINT32           enCurrentFsmId;
    VOS_UINT32                           ulCurrentEventType;

    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitCsPsRegRsltIndExpired_PlmnSelection_WaitCsPsRegRsltInd: Timer Expired");

    /* �����ǰ�ȴ�ע��ı�ʶ */
    NAS_MMC_ClearAllWaitRegRsltFlag_PlmnSelection();

    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();
    if (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType))
    {
        NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_FAIL);

        /* ���������ϢΪ��ͨ���������󣬺������µ��û�����ʱ�����Դ�ϸ�״̬�� */
        NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection();

#if   (FEATURE_ON == FEATURE_LTE)
        /* ��LMM�����û���������֪ͨ */
        if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
        {
            NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
        }
#endif

    }

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* ���ϲ�״̬������������� ABORT */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    enCsAdditionalAction = NAS_MMC_GetCsRegAdditionalAction_PlmnSelection();
    enPsAdditionalAction = NAS_MMC_GetPsRegAdditionalAction_PlmnSelection();

    /* �ȴ�CS PS��ע����ʱ���п���һ������ע�������˺����Լ��������е�ҵ��
      (����G�½���CS����к��У�PS���޷�����ע��),����MMCһ�µȲ�������һ�����
      ע��������ʱMMCֱ��ͨ��additional��������������MMC��ʱ���ݵ�ǰ�յ���ע
      �������µ�additionalֵ�������Ƿ�ɹ��˳�����״̬������ʧ���˳�����״̬����
      ��L1״̬����������״̬�����˳���������Ƿ�����������ѡ����ʱ�����Ǽ����ȴ�
      ҵ������Լ�CSPS��ע���� */

    enAdditionalAction = NAS_MMC_GetPrioAddtionalAction(enCsAdditionalAction, enPsAdditionalAction);

    /* ���Ӳ���Ϊפ����ǰ���� */
    if ((NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_ACCESS_BARRED == enAdditionalAction))
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_SUCC);
    }
    else
    {
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);
    }

    /* ֱ���˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvGmmNetworkDetachInd_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsRegCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsRegCause;
    MMCGMM_NETWORK_DETACH_IND_STRU     *pstDetachMsg = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAdditionalAction;
    NAS_MMC_FSM_ID_ENUM_UINT32           enCurrentFsmId;
    VOS_UINT32                           ulCurrentEventType;

    pstDetachMsg = (MMCGMM_NETWORK_DETACH_IND_STRU*)pstMsg;

    /* ���ݵ�ǰ��Detach���ͣ� �����´��� */
    enAdditionalAction = NAS_MMC_ProcGmmNetworkDetachInd(pstDetachMsg);

    /* AdditionalAction��Ч������PS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(enAdditionalAction);
    }

    /* ��¼Ps ��ע���� */
    NAS_MMC_SetPsRegCause_PlmnSelection((NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16)pstDetachMsg->ulDetachCause);


    /* ������ԭ��ֵΪ#15, ����ӽ�ֹ������Ϣ��ѡ��״̬���Ľ�ֹ����LA������ȥ,
       ���û�ָ������ʱ������Ҫ�ѽ�ֹLA��������� */
    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();
    if ( (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId, ulCurrentEventType))
      && (NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL == pstDetachMsg->ulDetachCause) )
    {
        NAS_MML_AddForbRoamLa( NAS_MML_GetCurrCampLai(),
                               NAS_MMC_GetForbRoamLaInfo_PlmnSelection() );
    }

    /* ��Ҫ����ע��ʱ������Ҫִ�к����Ķ��� */
    if (NAS_MMC_GMM_NT_DETACH_REATTACH == pstDetachMsg->ulDetachType)
    {
        return VOS_TRUE;
    }

    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_PS);

#if   (FEATURE_ON == FEATURE_LTE)
    /* ��LMM����detach�����Ϣ */
    NAS_MMC_SndLmmMtDetachIndActionResultReq(pstDetachMsg->ulDetachType,
        (NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16)pstDetachMsg->ulDetachCause);
#endif

    if (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType))
    {
        enCsRegCause = NAS_MMC_GetCsRegCause_PlmnSelection();
        enPsRegCause = NAS_MMC_GetPsRegCause_PlmnSelection();

        if (VOS_TRUE == NAS_MMC_ProcUserSearchRegRslt_PlmnSelection(enCsRegCause,enPsRegCause))
        {
            /* ���������ϢΪ��ͨ���������󣬺������µ��û�����ʱ�����Դ�ϸ�״̬�� */
            NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection();

            /* �û�ָ������ɺ���Ҫ��MMLȫ�ֱ����еĽ�ֹ����LA��Ϣ֪ͨ�����*/
            NAS_MMC_SndRrMmCellSelReq(RRC_NAS_FORBLA_CHANGE);
        }
    }

    if (NAS_MMC_WAIT_REG_RESULT_IND_NULL != NAS_MMC_GetWaitRegRsltFlag_PlmnSelection())
    {
        return VOS_TRUE;
    }

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* �ظ��ϲ�״̬��������� ABORT */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    NAS_MMC_PerformAdditionalActionRegFinished_PlmnSelection();

    return VOS_TRUE;
}

VOS_UINT32 NAS_MMC_RcvGmmServiceRequestResultInd_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    GMMMMC_SERVICE_REQUEST_RESULT_IND_STRU                 *pstServiceRsltInd = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enPsRegCause;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_GU_ACTION_RSLT_INFO_STRU                        stActionRslt;
#endif
    NAS_MMC_FSM_ID_ENUM_UINT32                              enCurrentFsmId;
    VOS_UINT32                                              ulCurrentEventType;

    pstServiceRsltInd = (GMMMMC_SERVICE_REQUEST_RESULT_IND_STRU*)pstMsg;

    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();

    /* ����Service Reqest Result ��������ͬ���� */
    if (GMM_MMC_ACTION_RESULT_SUCCESS == pstServiceRsltInd->enActionResult)
    {
        /* ���·���״̬ */
        NAS_MMC_SetPsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_NULL);

        /* ����PS��AdditionalAction */
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON);

#if (FEATURE_ON == FEATURE_LTE)
        /* ��LMM����Service requestResult�����Ϣ����*/
        stActionRslt.enProcType     = NAS_MML_PROC_SER;
        stActionRslt.enRegRst       = NAS_MML_REG_RESULT_SUCCESS;
        stActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_PS;
        stActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_PS;
        stActionRslt.enCnCause      = NAS_MML_REG_FAIL_CAUSE_NULL;
        stActionRslt.ulAttemptCount = 0;

        NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);

#endif

    }
    else
    {
        enAdditionalAction = NAS_MMC_ProcPsServiceRegFail(pstServiceRsltInd);

        NAS_MMC_SetPsRegCause_PlmnSelection(pstServiceRsltInd->enRegFailCause);


        /* ������ԭ��ֵΪ#15, ����ӽ�ֹ������Ϣ��ѡ��״̬���Ľ�ֹ����LA������ȥ,
           ���û�ָ������ʱ������Ҫ�ѽ�ֹLA��������� */
        if ( (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId, ulCurrentEventType))
          && (NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL == pstServiceRsltInd->enRegFailCause) )
        {
            NAS_MML_AddForbRoamLa( NAS_MML_GetCurrCampLai(),
                                   NAS_MMC_GetForbRoamLaInfo_PlmnSelection() );
        }

        /* AdditionalAction��Ч������PS����һ���Ķ������� */
        if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
        {
            NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(enAdditionalAction);
        }

#if (FEATURE_ON == FEATURE_LTE)
        /* ��LMM����Service requestResult�����Ϣ���� */
        stActionRslt.enProcType     = NAS_MML_PROC_SER;
        stActionRslt.enRegRst       = NAS_MML_REG_RESULT_FAILURE;
        stActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_PS;
        stActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_PS;
        stActionRslt.enCnCause      = pstServiceRsltInd->enRegFailCause;
        stActionRslt.ulAttemptCount = 0;

        NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);
#endif

    }

    if (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType))
    {
        enCsRegCause = NAS_MMC_GetCsRegCause_PlmnSelection();
        enPsRegCause = NAS_MMC_GetPsRegCause_PlmnSelection();

        if (VOS_TRUE == NAS_MMC_ProcUserSearchRegRslt_PlmnSelection(enCsRegCause, enPsRegCause))
        {
            /* ���������ϢΪ��ͨ���������󣬺������µ��û�����ʱ�����Դ�ϸ�״̬�� */
            NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection();

            /* �û�ָ������ɺ���Ҫ��MMLȫ�ֱ����еĽ�ֹ����LA��Ϣ֪ͨ�����*/
            NAS_MMC_SndRrMmCellSelReq(RRC_NAS_FORBLA_CHANGE);
        }
    }

    if (NAS_MMC_WAIT_REG_RESULT_IND_NULL != NAS_MMC_GetWaitRegRsltFlag_PlmnSelection())
    {
        return VOS_TRUE;
    }

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }


    NAS_MMC_PerformAdditionalActionRegFinished_PlmnSelection();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvCmServiceRejectInd_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    MMCMM_CM_SERVICE_REJECT_IND_STRU   *pstRcvMsg = VOS_NULL_PTR;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsRegCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsRegCause;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_GU_ACTION_RSLT_INFO_STRU    stActionRslt;
#endif
    NAS_MMC_FSM_ID_ENUM_UINT32          enCurrentFsmId;
    VOS_UINT32                          ulCurrentEventType;

    pstRcvMsg = (MMCMM_CM_SERVICE_REJECT_IND_STRU*)pstMsg;

#if (FEATURE_ON == FEATURE_LTE)
    stActionRslt.enProcType     = NAS_MML_PROC_CM_SER;
    stActionRslt.enRegRst       = NAS_MML_REG_RESULT_FAILURE;
    stActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_CS;
    stActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_CS;
    stActionRslt.enCnCause      = (NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16)pstRcvMsg->ulCause;
    stActionRslt.ulAttemptCount = 0;

    NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);

#endif

    /*CmService Rejֻ�� #4��#6����ʱ���ᷢ�͸�MMC */
    if (NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME == pstRcvMsg->ulCause)
    {
        /* ���CS��ĵȴ���־ */
        NAS_MMC_ClearSingleDomainWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_CS);

        /* �����µ�enCsAdditioalActionΪAnycell */
        NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL);
    }


    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();
    if (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType))
    {
        enCsRegCause = NAS_MMC_GetCsRegCause_PlmnSelection();
        enPsRegCause = NAS_MMC_GetPsRegCause_PlmnSelection();

        if (VOS_TRUE == NAS_MMC_ProcUserSearchRegRslt_PlmnSelection(enCsRegCause,enPsRegCause))
        {
            /*���������ϢΪ��ͨ���������󣬺������µ��û�����ʱ�����Դ�ϸ�״̬��*/
            NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection();

            /* �û�ָ������ɺ���Ҫ��MMLȫ�ֱ����еĽ�ֹ����LA��Ϣ֪ͨ�����*/
            NAS_MMC_SndRrMmCellSelReq(RRC_NAS_FORBLA_CHANGE);
        }
    }

    if (NAS_MMC_WAIT_REG_RESULT_IND_NULL != NAS_MMC_GetWaitRegRsltFlag_PlmnSelection())
    {
        return VOS_TRUE;
    }


    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    NAS_MMC_PerformAdditionalActionRegFinished_PlmnSelection();

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvWasSuspendInd_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SUSPEND_IND_ST                *pstSuspendMsg = VOS_NULL_PTR;

    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat;

    pstSuspendMsg = (RRMM_SUSPEND_IND_ST*)pstMsg;

    enRat         = NAS_MML_GetCurrNetRatType();

    /* ���ΪOOS���߲�������ȷ, �����������ϵͳ��ѡ */
    if ((MMC_SUSPEND_CAUSE_PLMNSEARCH == pstSuspendMsg->ucSuspendCause)
     || (MMC_SUSPEND_CAUSE_BUTT <= pstSuspendMsg->ucSuspendCause))
    {
        NAS_MMC_SndAsSuspendRsp(RRC_NAS_SUSPEND_FAILURE, WUEPS_PID_WRR);
    }
    else
    {
        NAS_MMC_SetInterSysSuspendRat_PlmnSelection(enRat);
        
        NAS_MMC_LoadInterSysFsm_PlmnSelection(pstSuspendMsg->ucSuspendCause);
    }


    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvGasSuspendInd_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    RRMM_SUSPEND_IND_ST                *pstSuspendMsg = VOS_NULL_PTR;

    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat;

    pstSuspendMsg = (RRMM_SUSPEND_IND_ST*)pstMsg;

    enRat         = NAS_MML_GetCurrNetRatType();

    /* ���ΪOOS���߲�������ȷ, �����������ϵͳ��ѡ */
    if ((MMC_SUSPEND_CAUSE_PLMNSEARCH == pstSuspendMsg->ucSuspendCause)
     || (MMC_SUSPEND_CAUSE_BUTT <= pstSuspendMsg->ucSuspendCause))
    {
        NAS_MMC_SndAsSuspendRsp(RRC_NAS_SUSPEND_FAILURE, UEPS_PID_GAS);
    }
    else
    {
        NAS_MMC_SetInterSysSuspendRat_PlmnSelection(enRat);
        
        NAS_MMC_LoadInterSysFsm_PlmnSelection(pstSuspendMsg->ucSuspendCause);
    }


    return VOS_TRUE;
}

VOS_UINT32 NAS_MMC_RcvSuspendFailCauseArealost_WaitCsPsRegRsltInd(VOS_VOID
)
{
    NAS_MML_PLMN_WITH_RAT_STRU          stDestPlmn;
    VOS_UINT32                          ulResult;

    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo           = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stCurPlmn;

	/* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    /* ֹͣ�ȴ�ע������Ϣ�Ķ�ʱ��  */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    ulResult = NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn);
    
    /* ��ȡ�¸�����ʧ�� */
    if (VOS_TRUE != ulResult)
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);
        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();
        return VOS_TRUE;
    }
    
    /* ��ȡ��ǰפ����������Ϣ */
    pstCurCampInfo     = NAS_MML_GetCurrCampPlmnInfo();
    stCurPlmn.stPlmnId = stDestPlmn.stPlmnId;
    stCurPlmn.enRat    = pstCurCampInfo->enNetRatType;
    
    /* ���µ�ǰ���������״̬Ϊ��ע������ڴ��ָ����ȼ������У��������Ѵ����� */
    NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&stCurPlmn,
                                            NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,
                                            pstPlmnSelectionListInfo);
    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());

    /*��ȡ��һ��Ҫ����������,����ȡʧ�ܣ�����ϲ�״̬���ϱ�����ʧ�ܣ��˳�
      ����ȡ�ɹ������ж���һ��Ҫ�����������Ƿ��뵱ǰפ��������ͬ��
      ����ͬ���ڵ�ǰģʽ�����������������ǰ����
     */
    ulResult = NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn);

    /* ��ȡ�¸�����ʧ�ܣ�����г����������� */
    if (VOS_TRUE != ulResult)
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();

        return VOS_TRUE;
    }

    /* ��MM��GMM��������ָʾ */
    NAS_MMC_SndGmmPlmnSchInit();
    NAS_MMC_SndMmPlmnSchInit();

    NAS_MMC_SndMmaCampOnInd(VOS_FALSE);

    NAS_MMC_UpdateRegStateSpecPlmnSearch();

    /*��һ��Ҫ�����������Ƿ��뵱ǰפ�����粻ͬ,��Ҫ����ǰģʽ���л����ȴ�����״̬������������ʱ��*/
    if (stDestPlmn.enRat != NAS_MML_GetCurrNetRatType())
    {
        NAS_MMC_SndSuspendReq_PlmnSelection(NAS_MML_GetCurrNetRatType());
    }
    /* ����ҪSuspend */
    else
    {
        /* �������LMM�����������󣬸��ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
    }

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvSuspendRsltHoSucc_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CONN_STATUS_INFO_STRU                          *pstConnStatus       = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;
    NAS_MMC_FSM_ID_ENUM_UINT32                              enCurrentFsmId;
    VOS_UINT32                                              ulCurrentEventType; 
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         ucInterSysSuspendRat;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         ucCurrRat; 

    enCurrentFsmId       = NAS_MMC_GetCurrFsmId();
    pstConnStatus        = NAS_MML_GetConnStatus();
    ucInterSysSuspendRat = NAS_MMC_GetInterSysSuspendRat_PlmnSelection();
    ucCurrRat            = NAS_MML_GetCurrNetRatType();


#if (FEATURE_LTE == FEATURE_ON)

    /* Ǩ�Ƶ���ͬ�Ľ��뼼��ʱ����ҪǨ�Ƶ���ͬ�Ĵ���ע����״̬ */
    if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
    {
        /* ֹͣ��ǰ�ȴ�CS/PSע������״̬ */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);

        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_SUCC);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

#endif

    /* ��ע����ʱ�������ǰ��CSҵ������ʱ��������ϵͳ�л�������״̬����L1 */
    if ( (VOS_TRUE  == pstConnStatus->ucCsServiceConnStatusFlg)
      && (ucCurrRat != ucInterSysSuspendRat) )
    {
        /* ֹͣ��ǰ�ȴ�CS/PSע������״̬ */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);
        
        /* ��ǰ�û��������ָ������,��ظ�ע�����ɹ� */
        ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();
        
        if (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType))
        {
            NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_FAIL);

            /* ���������ϢΪ��ͨ���������󣬺������µ��û�����ʱ�����Դ�ϸ�״̬�� */
            NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection();

#if   (FEATURE_ON == FEATURE_LTE)
            /* ��LMM�����û���������֪ͨ */
            if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
            {
                NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
            }
#endif
        }

        enCsAdditionalAction = NAS_MMC_GetCsRegAdditionalAction_PlmnSelection();
        enPsAdditionalAction = NAS_MMC_GetPsRegAdditionalAction_PlmnSelection();

        /* �ȴ�CS PS��ע����ʱ���п���һ������ע�������˺����Լ��������е�ҵ��,
           ������G<->W֮�����ϵͳ���л�(����G�½���CS����к��У�PS���޷�����ע��),
           �绰����������ǰ����绰������״̬������ΪREGISTERED,�����ѡ��״̬��������
           ȥѡ����ǰ���Ե����硣MMC��ʱ���ݵ�ǰ�յ���ע�������µ�additionalֵ������
           �Ƿ�ɹ��˳�����״̬������ʧ���˳�����״̬��,��L1״̬����������״̬�����˳�
           ��������Ƿ�����������ѡ����ʱ�����Ǽ����ȴ�ҵ������Լ�CSPS��ע���� */

        enAdditionalAction = NAS_MMC_GetPrioAddtionalAction(enCsAdditionalAction, enPsAdditionalAction);

        /* ���Ӳ���Ϊפ����ǰ���� */
        if ((NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON  == enAdditionalAction)
         || (NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION  == enAdditionalAction)
         || (NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON == enAdditionalAction)
         || (NAS_MMC_ADDITIONAL_ACTION_ACCESS_BARRED   == enAdditionalAction))
        {
            /* �ظ�״̬���˳��ɹ� */
            NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_SUCC);
        }
        else
        {
            NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);
        }

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE; 
    }

    return VOS_TRUE;
}
VOS_UINT32  NAS_MMC_RcvMmcMmcSuspendRslt_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMCMMC_SUSPEND_RSLT_STRU       *pstSuspendRsltMsg = VOS_NULL_PTR;

    pstSuspendRsltMsg = (NAS_MMCMMC_SUSPEND_RSLT_STRU*)pstMsg;

    
    /* HO�ɹ��Ĵ��� */
    if ( (MMC_SUSPEND_CAUSE_HANDOVER == pstSuspendRsltMsg->enSuspendCause)
      && (NAS_MMC_SUSPEND_SUCCESS    == pstSuspendRsltMsg->enRslt) )
    {
        NAS_MMC_RcvSuspendRsltHoSucc_PlmnSelection_WaitCsPsRegRsltInd(ulEventType, pstMsg);

        return VOS_TRUE; 
    }
    
    /* �����ѡ���Ϊʧ��,ԭ��Ϊ��������,����ע������г����������� */
    if ((NAS_MMC_SUSPEND_FAIL == pstSuspendRsltMsg->enRslt)
     && (NAS_MMC_SUSPEND_FAIL_CAUSE_AREA_LOST == pstSuspendRsltMsg->enFailCause))
    {
        /*�����ǰ��Ҫ�˳�״̬��*/
        if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
        {
            NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);
            NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
            
            /* �ظ�״̬���˳��ɹ� */
            NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

            /* ֱ���˳���ǰ״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            return VOS_TRUE;
        }
        else
        {
            return NAS_MMC_RcvSuspendFailCauseArealost_WaitCsPsRegRsltInd();
        }
    }
    
#if (FEATURE_LTE == FEATURE_ON)

    /* Ǩ�Ƶ���ͬ�Ľ��뼼��ʱ����ҪǨ�Ƶ���ͬ�Ĵ���ע����״̬ */
    if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
    {
        /* ֹͣ��ǰ�ȴ�CS/PSע������״̬ */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);


        /* HO�ɹ��Ĵ����װ��NAS_MMC_RcvSuspendRsltHoSucc_PlmnSelection_WaitCsPsRegRsltInd */

        /* ���µ�ǰ�ȴ�EPS��ע���� */
        NAS_MMC_ClearAllWaitRegRsltFlag_PlmnSelection();
        NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_PS);

#if (FEATURE_ON == FEATURE_IMS)    
        /* ���ӵȴ�IMS�Ƿ���ý��FLG,MMA��֤�յ�PSע�����ϱ�IMS�Ƿ����ָʾ��MMC */    
        NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_IMS);
#endif    

        /* ����CS��ע��Cause��AdditionalAction */
        NAS_MMC_SetCsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_SERVING_CELL_DOMAIN_NOT_SUPPORT);
        NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_BUTT);
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_BUTT);

        /* Ǩ�Ƶ��ȴ�ע������״̬PLMN_SRCH:NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_REG_IND */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_REG_IND);

        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_REG_IND, TI_NAS_MMC_WAIT_EPS_REG_IND_LEN);

        return VOS_TRUE;
    }

#endif

     /* ��Intersys״̬������ϴ���������
     switch (pstSuspendRsltMsg->enRslt)
     {
         case NAS_MMC_SUSPEND_SUCCESS:
         break;

         case NAS_MMC_SUSPEND_FAIL :
         case NAS_MMC_SUSPEND_QUIT :

         break;

         case NAS_MMC_SUSPEND_AREA_LOST :
         break;

         default:

         break;
     }
     */


    return VOS_TRUE;
}

VOS_UINT32 NAS_MMC_RcvAreaLostInd_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_WITH_RAT_STRU          stDestPlmn;
    VOS_UINT32                          ulResult;

    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo           = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stCurPlmn;

    /* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    /* ֹͣ�ȴ�ע������Ϣ�Ķ�ʱ��  */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);


    /* ��ȡ��ǰפ����������Ϣ */
    pstCurCampInfo     = NAS_MML_GetCurrCampPlmnInfo();
    stCurPlmn.stPlmnId = pstCurCampInfo->stLai.stPlmnId;
    stCurPlmn.enRat    = pstCurCampInfo->enNetRatType;

    NAS_MMC_SaveLastCampedPlmnid(NAS_MML_GetCurrCampPlmnInfo()->stLai.stPlmnId.ulMcc,
                           NAS_MML_GetCurrCampPlmnInfo()->stLai.stPlmnId.ulMnc);

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    /* ���µ�ǰ���������״̬Ϊ��ע������ڴ��ָ����ȼ������У��������Ѵ����� */
    NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&stCurPlmn,
                                            NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,
                                            pstPlmnSelectionListInfo);

    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());

    /*�����ǰ��Ҫ�˳�״̬��*/
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /*��ȡ��һ��Ҫ����������,����ȡʧ�ܣ�����ϲ�״̬���ϱ�����ʧ�ܣ��˳�
      ����ȡ�ɹ������ж���һ��Ҫ�����������Ƿ��뵱ǰפ��������ͬ��
      ����ͬ���ڵ�ǰģʽ�����������������ǰ����
     */
    ulResult = NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn);

    /* ��ȡ�¸�����ʧ�ܣ�����г����������� */
    if (VOS_TRUE != ulResult)
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();

        return VOS_TRUE;
    }

    /* ��MM��GMM��������ָʾ */
    NAS_MMC_SndGmmPlmnSchInit();
    NAS_MMC_SndMmPlmnSchInit();

    NAS_MMC_SndMmaCampOnInd(VOS_FALSE);

    NAS_MMC_UpdateRegStateSpecPlmnSearch();

    /*��һ��Ҫ�����������Ƿ��뵱ǰפ�����粻ͬ,��Ҫ����ǰģʽ���л����ȴ�����״̬������������ʱ��*/
    if (stDestPlmn.enRat != NAS_MML_GetCurrNetRatType())
    {
        NAS_MMC_SndSuspendReq_PlmnSelection(NAS_MML_GetCurrNetRatType());
    }
    /* ����ҪSuspend */
    else
    {
        /* �������LMM�����������󣬸��ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
    }

    return VOS_TRUE;
}
VOS_UINT32  NAS_MMC_RcvWasSysInfoInd_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;
    NAS_MMC_FSM_ID_ENUM_UINT32                              enCurrentFsmId;
    VOS_UINT32                                              ulCurrentEventType;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;

    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();
    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg,enLCapabilityStatus);
#endif

    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_WasSysInfo(pstMsg);

    /* פ��С���仯ʱ���ϱ�ϵͳ��Ϣ */
    if (VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
    {
        /* �����ϱ� */
        NAS_MMC_SndMmaSysInfo();

    }

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMMAģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMmaAcInfoChangeInd(MMA_MMC_SRVDOMAIN_CS, pstCurCsRestrictInfo);
    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMmaAcInfoChangeInd(MMA_MMC_SRVDOMAIN_PS, pstCurPsRestrictInfo);
    }

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_WCDMA);
#endif

    NAS_MMC_SndMmaCampOnInd(VOS_TRUE);

    /* ���µ�ǰ�ȴ�CSPS��ע���� */
    NAS_MMC_ClearAllWaitRegRsltFlag_PlmnSelection();
    NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_CS);
    NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* ����CSPS��RegCause */
    NAS_MMC_SetCsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_BUTT);
    NAS_MMC_SetPsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_BUTT);



    /* ת��ϵͳ��Ϣ֪ͨ �� MM/GMM,������ǰϵͳ��Ϣ�� Forbidden ����Я����ȥ */

    if (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType))
    {
        NAS_MMC_SndMmWasSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenTypeForUserSpecPlmnSrch(),
                                   VOS_TRUE,
                                   pstMsg);
        
        NAS_MMC_SndGmmWasSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenTypeForUserSpecPlmnSrch(),
                                   VOS_TRUE,
                                   pstMsg);
    }
    else
    {
        NAS_MMC_SndMmWasSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(),
                                   VOS_FALSE,
                                   pstMsg);
        
        NAS_MMC_SndGmmWasSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(),
                                   VOS_FALSE,
                                   pstMsg);
    }


#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmWcdmaSysInfoInd(pstMsg);
#endif

    return VOS_TRUE;
}
VOS_UINT32  NAS_MMC_RcvGasSysInfoInd_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;
    NAS_MMC_FSM_ID_ENUM_UINT32                              enCurrentFsmId;
    VOS_UINT32                                              ulCurrentEventType;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;

    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();
#endif

    /* ����յ�GAS��ϵͳ��Ϣ����ĿǰUtran��Enable��������Disable Utran */
    NAS_MMC_RcvGasSysInfoSetUtranCapabilityStatus();

    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_GasSysInfo(pstMsg);

    /* פ��С���仯ʱ���ϱ�ϵͳ��Ϣ */
    if (VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
    {
        /* �����ϱ� */
        NAS_MMC_SndMmaSysInfo();

    }

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMMAģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMmaAcInfoChangeInd(MMA_MMC_SRVDOMAIN_CS, pstCurCsRestrictInfo);
    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMmaAcInfoChangeInd(MMA_MMC_SRVDOMAIN_PS, pstCurPsRestrictInfo);
    }

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg, enLCapabilityStatus);
#endif

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_GSM);
#endif

    NAS_MMC_SndMmaCampOnInd(VOS_TRUE);

    /* ���µ�ǰ�ȴ�CSPS��ע���� */
    NAS_MMC_ClearAllWaitRegRsltFlag_PlmnSelection();
    NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_CS);
    NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* ����CSPS��RegCause */
    NAS_MMC_SetCsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_BUTT);
    NAS_MMC_SetPsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_BUTT);


    if (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType))
    {
        NAS_MMC_SndMmGsmSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenTypeForUserSpecPlmnSrch(),
                                   VOS_TRUE,
                                   pstMsg);
        
        NAS_MMC_SndGmmGsmSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenTypeForUserSpecPlmnSrch(),
                                   VOS_TRUE,
                                   pstMsg);        
    }
    else
    {
        NAS_MMC_SndMmGsmSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(),
                                   VOS_FALSE,
                                   pstMsg);
        
        NAS_MMC_SndGmmGsmSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(),
                                   VOS_FALSE,
                                   pstMsg);          
    }

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmGsmSysInfoInd(pstMsg);
#endif

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvTafSysCfgReq_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֱ�ӵ���syscfg���״̬�� */
    NAS_MMC_FSM_InitFsmL2(NAS_MMC_FSM_SYSCFG);

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvTafPlmnSpecialReq_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSrchList   = VOS_NULL_PTR;

    MMA_MMC_PLMN_SPECIAL_REQ_STRU                          *pstUserSelReqMsg  = VOS_NULL_PTR;

    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enPsRegCause;

    pstUserSelReqMsg    = (MMA_MMC_PLMN_SPECIAL_REQ_STRU*)pstMsg;

    /* �����ֶ�������Ϣ */
    NAS_MMC_UpdateUserSpecPlmnSearchInfo_PlmnSelection((NAS_MML_PLMN_ID_STRU*)(&pstUserSelReqMsg->stPlmnId),
                                                       pstUserSelReqMsg->enAccessMode);

    /* ��ȡѡ���б� */
    pstPlmnSrchList = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    /* �����û�ָ�������������³�ʼ��ѡ���б�, Ŀǰֻ�е�ǰע��������ָ������������ͬʱ���Ż��ߵ���һ�� */
    NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_USER_SPEC_PLMN_SEARCH,
                                  VOS_NULL_PTR,
                                  pstPlmnSrchList);

    /* ���û�ָ�����뼼��������(Ҳ�ǵ�ǰ����)����Ϊ������ע������ظ�ע�� */
    NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(NAS_MMC_GetUserSpecPlmnId(),
                                                   NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,
                                                   pstPlmnSrchList);

    enCsRegCause = NAS_MMC_GetCsRegCause_PlmnSelection();
    enPsRegCause = NAS_MMC_GetPsRegCause_PlmnSelection();

    if (VOS_TRUE == NAS_MMC_ProcUserSearchRegRslt_PlmnSelection(enCsRegCause, enPsRegCause))
    {
        return VOS_TRUE;
    }

    /* ���������Ϣ */
    NAS_MMC_SaveCurEntryMsg(ulEventType, pstMsg);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmcSyscfgRsltCnf_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmAbortInd_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMMMC_ABORT_IND_STRU               *pstMmAbortInd   = VOS_NULL_PTR;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_GU_ACTION_RSLT_INFO_STRU    stGuActionRslt;
#endif

    pstMmAbortInd = (MMMMC_ABORT_IND_STRU*)pstMsg;

    /* ���ԭ��ֵ��Ϊ#6���Ҳ�Ϊ#17��ֱ�ӷ��� */
    if ((NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME != (pstMmAbortInd->enAbortCause))
     && (NAS_MML_REG_FAIL_CAUSE_NETWORK_FAILURE != (pstMmAbortInd->enAbortCause)))
    {
        return VOS_TRUE;
    }

    /* ���ԭ��ֵ#6������CS AdditionΪAnycellפ�� */
    if (NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME == pstMmAbortInd->enAbortCause)
    {
        NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL);
    }

#if (FEATURE_ON == FEATURE_LTE)

    /* ��LMM����ID_MMC_LMM_ACTION_RESULT_REQ */
    /* ����ActionRslt��Ϣ�����ΪCN_Rej,������Ϊ����NAS_MML_PROC_MM_ABORT����
       ����ͽ������CS��ԭ��ֵΪAbort����ԭ��ֵ */
    stGuActionRslt.enProcType     = NAS_MML_PROC_MM_ABORT;
    stGuActionRslt.enRegRst       = NAS_MML_REG_RESULT_CN_REJ;
    stGuActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_CS;
    stGuActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_CS;
    stGuActionRslt.enCnCause      = pstMmAbortInd->enAbortCause;
    stGuActionRslt.ulAttemptCount = 0x0;

    NAS_MMC_SndLmmRegActionResultReq(&stGuActionRslt);
#endif

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmaImsVoiceCapInd_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* GU���յ�IMS VOICE�Ƿ����Ŀǰ�ݲ�����
       ������ϵͳ�������յ���IMS VOICE�Ƿ���õĳ������ڿ�������ϵͳ���µ�IMS VOICE
       ������,��ʱ��������disable Lģ��׼ȷ,�ݲ����Ǹó��� */

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;

    /* ������������û�����Ĺػ�����ֱ���˳��������ͷŽ��״̬�� */
    if (NAS_BuildEventType(WUEPS_PID_MMA, ID_MMA_MMC_POWER_OFF_REQ) == pstAbortMsg->ulEventType)
    {
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ��¼ Abort��־ */
    NAS_MMC_SetAbortFlag_PlmnSelection(VOS_TRUE);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvGmmSignalingStatusInd_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    /*  ��������������ӣ�������ȴ�  */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

    if ( VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection() )
    {
        /* ���ϲ�״̬��ָʾѡ��ABORT */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����������Ӳ����ڶ��Ҳ��������ͷ���������и��Ӳ��� */
    if (VOS_FALSE == NAS_MMC_GetRelRequestFlag_PlmnSelection())
    {
        NAS_MMC_PerformAdditionalActionConnRel_PlmnSelection();
    }
    else
    {
        NAS_MMC_SetRelRequestFlag_PlmnSelection(VOS_FALSE);

        NAS_MMC_SearchNextPlmn_PlmnSelection();
    }


    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvGmmNetworkDetachInd_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMCGMM_NETWORK_DETACH_IND_STRU     *pstDetachMsg = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAdditionalAction;

    NAS_MMC_FSM_ID_ENUM_UINT32                              enCurrentFsmId;
    VOS_UINT32                                              ulCurrentEventType;

    pstDetachMsg = (MMCGMM_NETWORK_DETACH_IND_STRU *)pstMsg;

    enAdditionalAction = NAS_MMC_ProcGmmNetworkDetachInd(pstDetachMsg);

    /* AdditionalAction��Ч������PS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(enAdditionalAction);
    }

    /* ��¼Ps ��ע���� */
    NAS_MMC_SetPsRegCause_PlmnSelection((NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16)pstDetachMsg->ulDetachCause);


    /* ������ԭ��ֵΪ#15, ����ӽ�ֹ������Ϣ��ѡ��״̬���Ľ�ֹ����LA������ȥ,
       ���û�ָ������ʱ������Ҫ�ѽ�ֹLA��������� */
    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
   ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();
    if ( (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId, ulCurrentEventType))
      && (NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL == pstDetachMsg->ulDetachCause) )
    {
        NAS_MML_AddForbRoamLa( NAS_MML_GetCurrCampLai(),
                               NAS_MMC_GetForbRoamLaInfo_PlmnSelection() );
    }


#if   (FEATURE_ON == FEATURE_LTE)
    /* ��LMM����detach�����Ϣ */
    NAS_MMC_SndLmmMtDetachIndActionResultReq(pstDetachMsg->ulDetachType,
        (NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16)pstDetachMsg->ulDetachCause);
#endif

    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvGmmServiceRequestResultInd_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    GMMMMC_SERVICE_REQUEST_RESULT_IND_STRU                 *pstServiceRsltInd = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

    NAS_MMC_FSM_ID_ENUM_UINT32                              enCurrentFsmId;
    VOS_UINT32                                              ulCurrentEventType;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_GU_ACTION_RSLT_INFO_STRU                        stActionRslt;
#endif

    pstServiceRsltInd = (GMMMMC_SERVICE_REQUEST_RESULT_IND_STRU*)pstMsg;

    /* ����Service Reqest Result ��������ͬ���� */
    if (GMM_MMC_ACTION_RESULT_SUCCESS == pstServiceRsltInd->enActionResult)
    {
        /* ���·���״̬ */
        enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;
        NAS_MMC_SetPsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_NULL);

#if (FEATURE_ON == FEATURE_LTE)
        /* ��LMM����Service requestResult�����Ϣ���� */
        stActionRslt.enProcType     = NAS_MML_PROC_SER;
        stActionRslt.enRegRst       = NAS_MML_REG_RESULT_SUCCESS;
        stActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_PS;
        stActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_PS;
        stActionRslt.enCnCause      = NAS_MML_REG_FAIL_CAUSE_NULL;
        stActionRslt.ulAttemptCount = 0;

        NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);
#endif


    }
    else
    {
        enAdditionalAction = NAS_MMC_ProcPsServiceRegFail(pstServiceRsltInd);

        NAS_MMC_SetPsRegCause_PlmnSelection(pstServiceRsltInd->enRegFailCause);


        /* ������ԭ��ֵΪ#15, ����ӽ�ֹ������Ϣ��ѡ��״̬���Ľ�ֹ����LA������ȥ,
           ���û�ָ������ʱ������Ҫ�ѽ�ֹLA��������� */
        enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
        if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
        {
            NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
        }
        ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();
        if ( (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId, ulCurrentEventType))
          && (NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL == pstServiceRsltInd->enRegFailCause) )
        {
            NAS_MML_AddForbRoamLa( NAS_MML_GetCurrCampLai(),
                                   NAS_MMC_GetForbRoamLaInfo_PlmnSelection() );
        }

#if (FEATURE_ON == FEATURE_LTE)

        /* ��LMM����Service requestResult�����Ϣ���� */
        stActionRslt.enProcType     = NAS_MML_PROC_SER;
        stActionRslt.enRegRst       = NAS_MML_REG_RESULT_FAILURE;
        stActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_PS;
        stActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_PS;
        stActionRslt.enCnCause      = pstServiceRsltInd->enRegFailCause;
        stActionRslt.ulAttemptCount = 0;

            NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);
#endif

    }

    /* AdditionalAction��Ч������PS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(enAdditionalAction);
    }

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvMmConnInfoInd_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    /*  ��������������ӣ�������ȴ�  */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

    if ( VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection() )
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����������Ӳ����ڶ��Ҳ��������ͷ���������и��Ӳ��� */
    if (VOS_FALSE == NAS_MMC_GetRelRequestFlag_PlmnSelection())
    {
        NAS_MMC_PerformAdditionalActionConnRel_PlmnSelection();
    }
    else
    {
        NAS_MMC_SetRelRequestFlag_PlmnSelection(VOS_FALSE);

        NAS_MMC_SearchNextPlmn_PlmnSelection();
    }


    return VOS_TRUE;
}



VOS_UINT32  NAS_MMC_RcvMmRrRelInd_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    /*  ��������������ӣ�������ȴ�  */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

    if ( VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection() )
    {
        /* ���ϲ�״̬��ָʾѡ��ABORT */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����������Ӳ����ڶ��Ҳ��������ͷ���������и��Ӳ��� */
    if (VOS_FALSE == NAS_MMC_GetRelRequestFlag_PlmnSelection())
    {
        NAS_MMC_PerformAdditionalActionConnRel_PlmnSelection();
    }
    else
    {
        NAS_MMC_SetRelRequestFlag_PlmnSelection(VOS_FALSE);

        NAS_MMC_SearchNextPlmn_PlmnSelection();
    }


    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvRrMmRelInd_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_REL_IND_STRU                  *pstRelIndMsg = VOS_NULL_PTR;

    pstRelIndMsg = ( RRMM_REL_IND_STRU * )pstMsg;


    /* ������Ӵ��ڱ�־ */
    if (RRC_RRC_CONN_STATUS_ABSENT == pstRelIndMsg->ulRrcConnStatus)
    {
        NAS_MML_SetRrcConnStatusFlg(VOS_FALSE);
    }

    /*  ��������������ӣ�������ȴ�  */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

    if ( VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection() )
    {
        /* ���ϲ�״̬��ָʾѡ��ABORT */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����������Ӳ����ڶ��Ҳ��������ͷ���������и��Ӳ��� */
    if (VOS_FALSE == NAS_MMC_GetRelRequestFlag_PlmnSelection())
    {
        NAS_MMC_PerformAdditionalActionConnRel_PlmnSelection();
    }
    else
    {
        NAS_MMC_SetRelRequestFlag_PlmnSelection(VOS_FALSE);

        NAS_MMC_SearchNextPlmn_PlmnSelection();
    }


    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvGmmTbfRelInd_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    /*  ��������������ӣ�������ȴ�  */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    /* ֹͣ�ȴ������ͷŶ�ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

    if ( VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection() )
    {
        /* ���ϲ�״̬��ָʾѡ��ABORT */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����������Ӳ����ڶ��Ҳ��������ͷ���������и��Ӳ��� */
    if (VOS_FALSE == NAS_MMC_GetRelRequestFlag_PlmnSelection())
    {
        NAS_MMC_PerformAdditionalActionConnRel_PlmnSelection();
    }
    else
    {
        NAS_MMC_SetRelRequestFlag_PlmnSelection(VOS_FALSE);

        NAS_MMC_SearchNextPlmn_PlmnSelection();
    }


    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvWasSuspendInd_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    RRMM_SUSPEND_IND_ST                *pstSuspendMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulRelRequestFlg;

    pstSuspendMsg = (RRMM_SUSPEND_IND_ST*)pstMsg;

    ulRelRequestFlg = NAS_MMC_GetRelRequestFlag_PlmnSelection();

    /* ����������ͷŻ����Ѿ����յ�Abort��Ϣ,�����������ϵͳ��ѡ */
    if ((VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
     || (VOS_TRUE == ulRelRequestFlg)
     || (MMC_SUSPEND_CAUSE_PLMNSEARCH == pstSuspendMsg->ucSuspendCause)
     || (MMC_SUSPEND_CAUSE_BUTT <= pstSuspendMsg->ucSuspendCause))
    {
        NAS_MMC_SndAsSuspendRsp(RRC_NAS_SUSPEND_FAILURE, WUEPS_PID_WRR);
    }
    else
    {
        NAS_MMC_LoadInterSysFsm_PlmnSelection(pstSuspendMsg->ucSuspendCause);
    }


    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvGasSuspendInd_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    RRMM_SUSPEND_IND_ST                *pstSuspendMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulRelRequestFlg;

    pstSuspendMsg = (RRMM_SUSPEND_IND_ST*)pstMsg;

    ulRelRequestFlg = NAS_MMC_GetRelRequestFlag_PlmnSelection();

    /* ����������ͷŻ����Ѿ����յ�Abort��Ϣ,�����������ϵͳ��ѡ */
    if ((VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
     || (VOS_TRUE == ulRelRequestFlg)
     || (MMC_SUSPEND_CAUSE_PLMNSEARCH == pstSuspendMsg->ucSuspendCause)
     || (MMC_SUSPEND_CAUSE_BUTT <= pstSuspendMsg->ucSuspendCause))

    {
        NAS_MMC_SndAsSuspendRsp(RRC_NAS_SUSPEND_FAILURE, UEPS_PID_GAS);
    }
    else
    {
        NAS_MMC_LoadInterSysFsm_PlmnSelection(pstSuspendMsg->ucSuspendCause);
    }


    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvMmcSuspendRslt_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
#if (FEATURE_LTE == FEATURE_ON)

    NAS_MMCMMC_SUSPEND_RSLT_STRU       *pstSuspendRsltMsg = VOS_NULL_PTR;

    pstSuspendRsltMsg = (NAS_MMCMMC_SUSPEND_RSLT_STRU*)pstMsg;

     /* Ǩ�Ƶ���ͬ�Ľ��뼼��ʱ����ҪǨ�Ƶ���ͬ�Ĵ���ע����״̬ */

    if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
    {
        /* ֹͣ��ǰ�ȴ�CS/PS�����ͷŵ�״̬ */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

        /* GU handover �� L �ɹ��󣬿��ܺܳ�ʱ���޷��յ�ϵͳ��Ϣ��ת��L1����  */
        if ((NAS_MMC_SUSPEND_SUCCESS == pstSuspendRsltMsg->enRslt)
         && (MMC_SUSPEND_CAUSE_HANDOVER == pstSuspendRsltMsg->enSuspendCause))
        {
            /* �ظ�״̬���˳��ɹ� */
            NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_SUCC);

            /* ֱ���˳���ǰ״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            return VOS_TRUE;
        }

        /* ���µ�ǰ�ȴ�EPS��ע���� */
        NAS_MMC_ClearAllWaitRegRsltFlag_PlmnSelection();
        NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_PS);
        
#if (FEATURE_ON == FEATURE_IMS)    
        /* ���ӵȴ�IMS�Ƿ���ý��FLG,MMA��֤�յ�PSע�����ϱ�IMS�Ƿ����ָʾ��MMC */    
        NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_IMS);
#endif    

        /* ����CS��ע��Cause��AdditionalAction */
        NAS_MMC_SetCsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_SERVING_CELL_DOMAIN_NOT_SUPPORT);
        NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_BUTT);
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_BUTT);

        /* Ǩ�Ƶ��ȴ�ע������״̬PLMN_SRCH:NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_REG_IND */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_REG_IND);

        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_REG_IND, TI_NAS_MMC_WAIT_EPS_REG_IND_LEN);

        return VOS_TRUE;
    }

#endif

    /*  ��������������ӣ�������ȴ�  */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

    /* ����ѡ֮ǰ�Ѿ��ж�,��ʱ�����ܴ���Abort���������ͷ�,����������Ӳ����ڿ��ܷ������� */
    NAS_MMC_PerformAdditionalActionConnRel_PlmnSelection();

   /* ��Intersys״̬������ϴ���������
    switch (pstSuspendRsltMsg->enRslt)
    {
        case NAS_MMC_SUSPEND_SUCCESS:
        break;

        case NAS_MMC_SUSPEND_FAIL :
        case NAS_MMC_SUSPEND_QUIT :

        break;

        case NAS_MMC_SUSPEND_AREA_LOST :
        break;

        default:

        break;
    }
    */




    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvWasSysInfoInd_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU        *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU         stOldCampInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;


    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();

#endif

    /* ��������������ͷ�,������Ҫ������������,ϵͳ��Ϣ������ */
    if (VOS_TRUE == NAS_MMC_GetRelRequestFlag_PlmnSelection())
    {
        return VOS_TRUE;
    }

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_WasSysInfo(pstMsg);

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg,enLCapabilityStatus);
#endif

    /* פ��С���仯ʱ���ϱ�ϵͳ��Ϣ */
    if (VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
    {
        /* �����ϱ� */
        NAS_MMC_SndMmaSysInfo();

    }

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMMAģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMmaAcInfoChangeInd(MMA_MMC_SRVDOMAIN_CS, pstCurCsRestrictInfo);
    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMmaAcInfoChangeInd(MMA_MMC_SRVDOMAIN_PS, pstCurPsRestrictInfo);
    }

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_WCDMA);
#endif

    NAS_MMC_SndMmaCampOnInd(VOS_TRUE);

    /*ת��ϵͳ��Ϣ֪ͨ �� MM/GMM,������ǰϵͳ��Ϣ�� Forbidden ����Я����ȥ */
    NAS_MMC_SndMmWasSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);
    NAS_MMC_SndGmmWasSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmWcdmaSysInfoInd(pstMsg);
#endif

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvGasSysInfoInd_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;


    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();
#endif

    /* ����յ�GAS��ϵͳ��Ϣ����ĿǰUtran��Enable��������Disable Utran */
    NAS_MMC_RcvGasSysInfoSetUtranCapabilityStatus();

    /* ��������������ͷ�,������Ҫ������������,ϵͳ��Ϣ������ */
    if (VOS_TRUE == NAS_MMC_GetRelRequestFlag_PlmnSelection())
    {
        return VOS_TRUE;
    }

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_GasSysInfo(pstMsg);

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_RcvGuSysInfoIndSetLteAbility(pstMsg, enLCapabilityStatus);
#endif

    /* פ��С���仯ʱ���ϱ�ϵͳ��Ϣ */
    if (VOS_TRUE == NAS_MML_IsCampPlmnInfoChanged(&stOldCampInfo, pstCurCampInfo))
    {
        /* �����ϱ� */
        NAS_MMC_SndMmaSysInfo();

    }

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMMAģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMmaAcInfoChangeInd(MMA_MMC_SRVDOMAIN_CS, pstCurCsRestrictInfo);
    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMmaAcInfoChangeInd(MMA_MMC_SRVDOMAIN_PS, pstCurPsRestrictInfo);
    }

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_GSM);
#endif

    NAS_MMC_SndMmaCampOnInd(VOS_TRUE);

    /*ת��ϵͳ��Ϣ֪ͨ �� MM/GMM,������ǰϵͳ��Ϣ�� Forbidden ����Я����ȥ */
    NAS_MMC_SndMmGsmSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);
    NAS_MMC_SndGmmGsmSysInfoInd(NAS_MMC_GetCurrentLaiForbbidenType(), VOS_FALSE, pstMsg);

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_SndLmmGsmSysInfoInd(pstMsg);
#endif

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvTiWaitRrcConnRelExpired_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulCsServiceStatusFlg;
    VOS_UINT32                          ulCsEmergencyServiceFlg;
    NAS_MML_PLMN_WITH_RAT_STRU          stDestPlmn;

    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitRrcConnRelExpired_PlmnSelection_WaitRrcConnRelInd ENTERED");

    /* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));


    if ( VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection() )
    {
        /* ���ϲ�״̬��ָʾѡ��ABORT */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����������Ӵ���, �����������ͷ�����,������������� */
    if (VOS_TRUE == NAS_MMC_GetRelRequestFlag_PlmnSelection())
    {
        NAS_MMC_SetRelRequestFlag_PlmnSelection(VOS_FALSE);

        NAS_MMC_SearchNextPlmn_PlmnSelection();

        return VOS_TRUE;
    }

    /*
    Connrel �����ͷţ�
    �ж��Ƿ���ҵ���У����˳� (�������񣬷��سɹ������޷��񣬷���ʧ��)
    ���򣬻�ȡ��һ������ɹ����������ͷţ��ñ�־λ��ʧ�����˳�
    */
    ulCsServiceStatusFlg    = NAS_MML_GetCsServiceExistFlg();
    ulCsEmergencyServiceFlg = NAS_MML_GetCsEmergencyServiceFlg();
    if ( (VOS_TRUE == ulCsServiceStatusFlg)
      && (VOS_TRUE == ulCsEmergencyServiceFlg))
    {
        /* �ظ�״̬������ʧ�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

        /* �˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }
    else
    {
        /* ��ȡ�¸����磬����ɹ��������ͷţ������˳� */
        if (VOS_TRUE == NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
        {
            /* ���ݵ�ǰ��ͬ�Ľ��뼼��,�����ͷ�����,Ǩ�Ƶ���ͬ�ĵȴ������ͷŵ�L2״̬��������������ʱ�� */
            NAS_MMC_SndRelReq_PlmnSelection(NAS_MML_GetCurrNetRatType());
        }
        else
        {
            NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

            NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();
        }
    }


    return VOS_TRUE;

}


VOS_UINT32 NAS_MMC_RcvTafSysCfgReq_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֱ�ӵ���syscfg���״̬�� */
    NAS_MMC_FSM_InitFsmL2(NAS_MMC_FSM_SYSCFG);

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvTafPlmnSpecialReq_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMA_MMC_PLMN_SPECIAL_REQ_STRU                          *pstUserSelReqMsg  = VOS_NULL_PTR;

    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSrchList   = VOS_NULL_PTR;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enPsCause;

    pstUserSelReqMsg    = (MMA_MMC_PLMN_SPECIAL_REQ_STRU*)pstMsg;


    /* �����ֶ�������Ϣ */
    NAS_MMC_UpdateUserSpecPlmnSearchInfo_PlmnSelection((NAS_MML_PLMN_ID_STRU*)(&pstUserSelReqMsg->stPlmnId),
                                                       pstUserSelReqMsg->enAccessMode);

    /* ��ȡѡ���б� */
    pstPlmnSrchList = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    /* �����û�ָ�������������³�ʼ��ѡ���б� */
    NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_USER_SPEC_PLMN_SEARCH,
                                  VOS_NULL_PTR,
                                  pstPlmnSrchList);

    /* ���û�ָ�����뼼��������(Ҳ�ǵ�ǰ����)����Ϊ������ע������ظ�ע�� */
    NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(NAS_MMC_GetUserSpecPlmnId(),
                                                   NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,
                                                   pstPlmnSrchList);

    enCsCause = NAS_MMC_GetCsRegCause_PlmnSelection();
    enPsCause = NAS_MMC_GetPsRegCause_PlmnSelection();

    /* ���ݱ����CS/PSע�����ж�ָ�����������Ƿ��ѽ��� */
    if (VOS_TRUE == NAS_MMC_ProcUserSearchRegRslt_PlmnSelection(enCsCause, enPsCause))
    {
        return VOS_TRUE;
    }

    /* ��ΪĿǰ״̬������Ϊ�ֶ���������,��Ҫ���������Ϣ , ĿǰӦ���ò���*/
    NAS_MMC_SaveCurEntryMsg(ulEventType, pstMsg);

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvMmcSyscfgRsltCnf_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvMmcSyscfgRsltCnf_PlmnSelection_WaitRrcConnRelInd: ENTERED. ");
    return VOS_TRUE;
}




VOS_UINT32 NAS_MMC_RcvCmServiceRejectInd_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMCMM_CM_SERVICE_REJECT_IND_STRU   *pstRcvMsg = VOS_NULL_PTR;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_GU_ACTION_RSLT_INFO_STRU    stActionRslt;
#endif

    pstRcvMsg = (MMCMM_CM_SERVICE_REJECT_IND_STRU*)pstMsg;

#if (FEATURE_ON == FEATURE_LTE)
    stActionRslt.enProcType     = NAS_MML_PROC_CM_SER;
    stActionRslt.enRegRst       = NAS_MML_REG_RESULT_FAILURE;
    stActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_CS;
    stActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_CS;
    stActionRslt.enCnCause      = (NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16)pstRcvMsg->ulCause;
    stActionRslt.ulAttemptCount = 0;

    NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);

#endif

    /* ��#6�����⴦��ʱ */
    if (NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME == pstRcvMsg->ulCause)
    {
        /* �����µ�enCsAdditioalAction��*/
        NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL);

    }

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvGmmPsRegResultInd_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    GMMMMC_PS_REG_RESULT_IND_STRU      *pstPsRegRslt = VOS_NULL_PTR;

    pstPsRegRslt = (GMMMMC_PS_REG_RESULT_IND_STRU*)pstMsg;

    /* ���CCOʧ�ܻ��˹������յ�����ϵͳ��ע����������Ҫ���� */
    if (pstPsRegRslt->enNetType != NAS_MML_GetCurrNetRatType())
    {
        return VOS_TRUE;
    }

    /* ����ע��������Ӧ����: ���÷���״̬����¼Causeֵ��������һ���������ͣ�֪ͨ��Ӧ��� */
    NAS_MMC_ProcPsRegRslt_PlmnSelection(pstPsRegRslt);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmCsRegResultInd_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMMMC_CS_REG_RESULT_IND_STRU       *pstCsRegRslt;

    pstCsRegRslt = (MMMMC_CS_REG_RESULT_IND_STRU*)pstMsg;

    /* ���CCOʧ�ܻ��˹������յ�����ϵͳ��ע����������Ҫ���� */
    if (pstCsRegRslt->enNetType != NAS_MML_GetCurrNetRatType())
    {
        return VOS_TRUE;
    }

    /* ����ע��������Ӧ����: ���÷���״̬����¼Causeֵ��������һ���������ͣ�֪ͨ��Ӧ��� */
    NAS_MMC_ProcCsRegRslt_PlmnSelection(pstCsRegRslt);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmAbortInd_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMMMC_ABORT_IND_STRU               *pstMmAbortInd   = VOS_NULL_PTR;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_GU_ACTION_RSLT_INFO_STRU    stGuActionRslt;
#endif

    pstMmAbortInd = (MMMMC_ABORT_IND_STRU*)pstMsg;

    /* ���ԭ��ֵ��Ϊ#6���Ҳ�Ϊ#17��ֱ�ӷ��� */
    if ((NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME != pstMmAbortInd->enAbortCause)
     && (NAS_MML_REG_FAIL_CAUSE_NETWORK_FAILURE != pstMmAbortInd->enAbortCause))
    {
        return VOS_TRUE;
    }

    /* ���ԭ��ֵ#6������CS AdditionΪAnycellפ�� */
    if (NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME == pstMmAbortInd->enAbortCause)
    {
        NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL);
    }

#if (FEATURE_ON == FEATURE_LTE)

    /* ��LMM����ID_MMC_LMM_ACTION_RESULT_REQ */
    /* ����ActionRslt��Ϣ�����ΪCN_Rej,������Ϊ����NAS_MML_PROC_MM_ABORT����
       ����ͽ������CS��ԭ��ֵΪAbort����ԭ��ֵ */
    stGuActionRslt.enProcType     = NAS_MML_PROC_MM_ABORT;
    stGuActionRslt.enRegRst       = NAS_MML_REG_RESULT_CN_REJ;
    stGuActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_CS;
    stGuActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_CS;
    stGuActionRslt.enCnCause      = pstMmAbortInd->enAbortCause;
    stGuActionRslt.ulAttemptCount = 0x0;

    NAS_MMC_SndLmmRegActionResultReq(&stGuActionRslt);
#endif

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmaRegReq_PlmnSelection_WaitMmaRegReq(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enNetRatType;
    NAS_MML_PLMN_WITH_RAT_STRU                              stDestPlmn;
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene;
    MMA_MMC_PLMN_ID_STRU                                    stPlmnId;
    NAS_MMC_INTER_PLMN_SEARCH_REQ_STRU                     *pstInterPlmnSrchMsg = VOS_NULL_PTR;
    MMA_MMC_REG_REQ_STRU                                   *pstRegReq = VOS_NULL_PTR;

    stPlmnId.ulMcc     = NAS_MML_INVALID_MCC;
    stPlmnId.ulMnc     = NAS_MML_INVALID_MNC;

    /* ֹͣ������ʱ��TI_NAS_MMC_WAIT_MMA_REG_REQ */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_MMA_REG_REQ);

    /* �����ABORT��־��ֱ���˳� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* ��û�д��ʱ���Żṹ���ڲ�������Ϣ��������������ΪREG_NCELL/REG_HPLMN/...
           �����д��ʱ��NAS_MMC_IsMmaRegReq_PlmnSelection����ΪVOS_TRUE
           ��Ҫ�������֧���reg_cnf */
        if (VOS_TRUE == NAS_MMC_IsNormalServiceStatus())
        {
            NAS_MMC_SndMmaRegCnf(MMC_MMA_REG_RESULT_SUCCESS, NAS_MML_GetCurrCampPlmnId(),
                                 NAS_MML_GetCurrCampArfcn());
        }
        else
        {
            if (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING != NAS_MMC_GetSpecPlmnSearchState())
            {
                NAS_MMC_SndMmaRegCnf(MMC_MMA_REG_RESULT_REG_FAILURE, NAS_MML_GetCurrCampPlmnId(),
                                 NAS_MML_GetCurrCampArfcn());
            }
            else
            {
                NAS_MMC_SndMmaRegCnf(MMC_MMA_REG_RESULT_ACQ_FAILURE, &stPlmnId, 0x0);
            }
        }

        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    pstRegReq = (MMA_MMC_REG_REQ_STRU*)pstMsg;

    /* ����NCELL��Ϣ */
    if (VOS_TRUE == pstRegReq->bitOpCellInfo)
    {
        NAS_MMC_SaveRegReqNCellInfo(&(pstRegReq->stCellInfo));
    }

    enPlmnSearchScene = NAS_MMC_GetPlmnSearchSceneMmaRegReq(pstRegReq);

    /* ��״̬�������Ϣ�滻Ϊ������ڲ�������Ϣ�������ѵ���������ֱ�ӷ���ע�� */
    pstInterPlmnSrchMsg = (NAS_MMC_INTER_PLMN_SEARCH_REQ_STRU*)PS_MEM_ALLOC(WUEPS_PID_MMC, sizeof(NAS_MMC_INTER_PLMN_SEARCH_REQ_STRU));
    if (VOS_NULL_PTR == pstInterPlmnSrchMsg)
    {
        /* �쳣��ӡ */
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvMmaRegReq_PlmnSelection_WaitMmaRegReq Alloc mem fail");

        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    NAS_MMC_BulidInterPlmnSearchReqMsg(enPlmnSearchScene, VOS_NULL_PTR, 0, pstInterPlmnSrchMsg);
    NAS_MMC_SaveCurEntryMsg(NAS_BuildEventType(WUEPS_PID_MMC, MMCMMC_INTER_PLMN_SEARCH_REQ),
                            (struct MsgCB*)pstInterPlmnSrchMsg);
    PS_MEM_FREE(WUEPS_PID_MMC, pstInterPlmnSrchMsg);

    /* ����enPlmnSearchScene����ˢ��ѡ���б�ɾ�����������ȼ�Ҫ������� */
    NAS_MMC_RefreshPlmnSelectionListRcvMmaRegReq_PlmnSelection(enPlmnSearchScene);

    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    /* ��ȡ��ǰ���뼼�� */
    enNetRatType = NAS_MML_GetCurrNetRatType();

    if (VOS_TRUE != NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
    {
        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();

        return VOS_TRUE;
    }

    if (enNetRatType == stDestPlmn.enRat)
    {
        /* �������LMM�����������󣬸��ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
    }
    /*��Ҫ����ǰģʽ���л����ȴ�����״̬������������ʱ��*/
    else
    {
        /* �������LMM���͹������󣬸��ݵ�ǰ��ͬ�Ľ��뼼����Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSuspendReq_PlmnSelection(enNetRatType);
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_PlmnSelection_WaitMmaRegReq(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;

    /* ������������û�����Ĺػ�����ֱ���˳���ע������״̬�� */
    /* �����������ID_MMA_MMC_POWER_SAVE_REQ��Ҳ��Ҫֱ���˳�״̬�� */
    if ( (NAS_BuildEventType(WUEPS_PID_MMA, ID_MMA_MMC_POWER_OFF_REQ) == pstAbortMsg->ulEventType)
      || (NAS_BuildEventType(WUEPS_PID_MMA, ID_MMA_MMC_POWER_SAVE_REQ) == pstAbortMsg->ulEventType) 
      || (NAS_BuildEventType(WUEPS_PID_MMA, ID_MMA_MMC_ACQ_REQ)        == pstAbortMsg->ulEventType) )
    {
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_MMA_REG_REQ);

        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ��¼��ϱ�־ */
    NAS_MMC_SetAbortFlag_PlmnSelection(VOS_TRUE);


    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvTiWaitMmaRegReqExpired_PlmnSelection_WaitMmaRegReq(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitMmaRegReqExpired_PlmnSelection_WaitMmaRegReq ENTERED");

    /* ����abort��־��ظ�abort������ظ�ʧ�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);
    }
    else
    {
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);
    }
    
    /* ֱ���˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}

#if (FEATURE_ON == FEATURE_LTE)


VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_PlmnSelection_WaitLmmPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF);

    /* ֪ͨLMMָֹͣ������ */
    NAS_MMC_SndLmmPlmnSrchStopReq();
    
    NAS_MMC_SetAbortFlag_PlmnSelection(VOS_TRUE);

    /*���ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������ͬ������ʱ��*/
    NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_PLMN_STOP_CNF);

    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmPlmnCnf_PlmnSelection_WaitLmmPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_PLMN_SRCH_CNF_STRU                             *pstLmmSrchCnfMsg = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stDestPlmn;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    stPlmnSearchInfo;

    /* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));
    
    pstLmmSrchCnfMsg = (LMM_MMC_PLMN_SRCH_CNF_STRU*)pstMsg;

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF);

    /* ���õ�ǰģʽ�Ƿ����ȫƵ�������Ƿ�������縲�� */
    NAS_MMC_UpdateLmmCoverageFlg_PlmnSelection(pstLmmSrchCnfMsg);

    if (MMC_LMM_PLMN_SRCH_RLT_SPEC_SUCC == pstLmmSrchCnfMsg->enRlst)
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_SUCCESS,
                VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        /*���ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬������������ʱ�� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_SYSINFO_IND);

        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_SYS_INFO, TI_NAS_MMC_WAIT_LMM_SYS_INFO_LEN);

        return VOS_TRUE;
    }

    /* ���µ�ǰ���뼼��������ȫƵ���� */
    NAS_MMC_SetAllBandSearch_PlmnSelection(NAS_MML_NET_RAT_TYPE_LTE, VOS_TRUE);

    /* ��������Ϣ��Я����������Ϣת��Ϊ�ڲ���Ϣ��������Ϣ */
    NAS_MMC_ConvertLmmPlmnList2SearchedPlmnListInfo(&(pstLmmSrchCnfMsg->stPlmnIdList),
                                                     &stPlmnSearchInfo);

    /* ������Ϣ��Я����������Ϣ���������б� */
    NAS_MMC_UpdatePlmnSearchList_PlmnSelection(&stPlmnSearchInfo);


    /* ����������һ����Ҫ����������ʱ�������������������򣬽�������פ�� */
    if (VOS_TRUE == NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
    {
        if (NAS_MML_NET_RAT_TYPE_LTE == stDestPlmn.enRat)
        {
            /* �������LMM�����������󣬸��ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
            NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
        }
        else
        {
            /* �������LMM���͹������󣬸��ݵ�ǰ��ͬ�Ľ��뼼����Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
            NAS_MMC_SndSuspendReq_PlmnSelection(NAS_MML_NET_RAT_TYPE_LTE);
        }
    }
    else
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();
    }


    return VOS_TRUE;

}


VOS_UINT32 NAS_MMC_RcvLmmAttachCnf_PlmnSelection_WaitLmmPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ǰLTE������״̬��ΪDISABLE ״̬���򲻴������Ϣ */
    if ( NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS != NAS_MML_GetLteCapabilityStatus())
    {
        /* �쳣��ӡ */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmAttachCnf_PlmnSelection_WaitLmmPlmnSearchCnf: unexpect message received");

        return VOS_TRUE;
    }

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF);

    /* ֪ͨLMMָֹͣ������ */
    NAS_MMC_SndLmmPlmnSrchStopReq();

    /*���ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������ͬ������ʱ��*/
    NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_PLMN_STOP_CNF);

    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmDetachCnf_PlmnSelection_WaitLmmPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ǰLTE������״̬��ΪDISABLE ״̬���򲻴������Ϣ */
    if ( NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS != NAS_MML_GetLteCapabilityStatus())
    {
        /* �쳣��ӡ */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmDetachCnf_PlmnSelection_WaitLmmPlmnSearchCnf: unexpect message received");

        return VOS_TRUE;
    }

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF);

    /* ֪ͨLMMָֹͣ������ */
    NAS_MMC_SndLmmPlmnSrchStopReq();

    /*���ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������ͬ������ʱ��*/
    NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_PLMN_STOP_CNF);

    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvTiWaitLmmPlmnSearchCnfExpired_PlmnSelection_WaitLmmPlmnSearchCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{   
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    stPlmnSearchInfo;

    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitLmmPlmnSearchCnfExpired_PlmnSelection_WaitLmmPlmnSearchCnf: Timer Expired");

    /* ���õ�ǰ�����Ѿ�����ȫƵ���� */
    NAS_MMC_SetAllBandSearch_PlmnSelection(NAS_MML_NET_RAT_TYPE_LTE, VOS_TRUE);

    /* ���õ�ǰ�����޸��� */
    NAS_MMC_SetRatCoverage_PlmnSelection(NAS_MML_NET_RAT_TYPE_LTE, NAS_MMC_COVERAGE_TYPE_NONE);

    /* ���õ�ǰ���뼼��û���������κ����� */
    stPlmnSearchInfo.ulHighPlmnNum = 0x0;
    stPlmnSearchInfo.ulLowPlmnNum  = 0x0;
    stPlmnSearchInfo.enRatType     = NAS_MML_NET_RAT_TYPE_LTE;

    /* ������Ϣ��Я����������Ϣ���������б� */
    NAS_MMC_UpdatePlmnSearchList_PlmnSelection(&stPlmnSearchInfo);
    
    /* ֪ͨLMMָֹͣ������ */
    NAS_MMC_SndLmmPlmnSrchStopReq();

    /*���ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������ͬ������ʱ��*/
    NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_PLMN_STOP_CNF);

    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
    
}



VOS_UINT32 NAS_MMC_RcvLmmSuspendCnf_PlmnSelection_WaitLmmSuspendCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SUSPEND_CNF_ST                *pstGuSuspendCnf = VOS_NULL_PTR;
    LMM_MMC_SUSPEND_CNF_STRU           *pstLmmSuspendCnf = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU          stDestPlmn;

    pstGuSuspendCnf = (RRMM_SUSPEND_CNF_ST*)pstMsg;

	/* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    /* Lģʱ�������¸�ʽת�� */
    pstLmmSuspendCnf = (LMM_MMC_SUSPEND_CNF_STRU*)pstMsg;

    pstGuSuspendCnf->ucResult = (VOS_UINT8)(pstLmmSuspendCnf->ulRst);

    /* ֹͣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_SUSPEND_CNF);

    if (MMC_AS_SUSPEND_FAILURE == pstGuSuspendCnf->ucResult)
    {
        /* ���ɻָ�����,���õ���ӿ�RESET */

        NAS_MML_SoftReBoot();

        return VOS_TRUE;
    }


    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* ���õ�ǰ���뼼��ΪBUTT */
        NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_BUTT);

        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* �����������,ԭ��Ϊ�˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }


    /* ��ȡ��Ҫ���������缰����뼼�� */
    if (VOS_TRUE == NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
    {
        /* ���ݻ�ȡ�Ľ��뼼�����õ�ǰ���뼼�� */
        NAS_MML_SetCurrNetRatType(stDestPlmn.enRat);

        /* �������LMM�����������󣬸��ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);

        /* ��֤����L�ص�GUʱPS��ע��״̬ʼ��Ϊ2,�˴�����һ�θ���(����LʱL�����ϱ���ע��״̬��Ϊ2) */
        NAS_MMC_UpdateRegStateSpecPlmnSearch();
    }
    else
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmSuspendCnf_PlmnSelection_WaitLmmSuspendCnf,WARNING:CAN'T FIND THE NEXT PLMN");

        /* ���õ�ǰ���뼼��ΪBUTT */
        NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_BUTT);

        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();
    }

    return VOS_TRUE;
}

VOS_UINT32 NAS_MMC_RcvLmmPlmnSearchStopCnf_PlmnSelection_WaitLmmPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_WITH_RAT_STRU          stDestPlmn;

    /* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    NAS_MMC_StopTimer( TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF );

    /* �д��ʱ���˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);
        
        NAS_MMC_FSM_QuitFsmL2();
        
        return VOS_TRUE;
    }

    /* �޴��ʱ:�����ǰLTE������״̬Ϊdisable LTE���������¸���LTE������
        �����ǰLTE������״̬ΪEnable LTE,�������¸����õ�LTE����
        Enable LTE��Ӧ�ĳ���Ϊ:��LMM���������ʱ�յ�LmmDetachCnf����ҪDisable LTE,Ǩ����LmmStopCnf״̬
        �ڵȴ�LmmStopCnfʱ���յ�LmmAttachCnf������ҪEnable LTE,��ʱ���յ�LmmStopCnf��Ҫ����ȥLTE������ */
    
    /* ����������һ����Ҫ����������ʱ�������������������򣬽�������פ�� */
    if (VOS_TRUE == NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
    {
        if (NAS_MML_NET_RAT_TYPE_LTE == stDestPlmn.enRat)
        {
            /* ��LMM������������Ǩ��״̬��������������ʱ�� */
            NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
        }
        else
        {
            /* �������LMM���͹������󣬸��ݵ�ǰ��ͬ�Ľ��뼼����Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
            NAS_MMC_SndSuspendReq_PlmnSelection(NAS_MML_NET_RAT_TYPE_LTE);
        }
    }
    else
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

            NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();
    }

    return VOS_TRUE;

}



VOS_UINT32 NAS_MMC_RcvTiWaitLmmStopCnfExpired_PlmnSelection_WaitLmmPlmnStopCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_WITH_RAT_STRU                              stDestPlmn;

	/* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitLmmStopCnfExpired_PlmnSelection_WaitLmmPlmnStopCnf: Timer Expired");

    /* �д��ʱ���˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);
        
        NAS_MMC_FSM_QuitFsmL2();
        
        return VOS_TRUE;
    }

    /* �޴��ʱ:�����ǰLTE������״̬Ϊdisable LTE���������¸���LTE������
        �����ǰLTE������״̬ΪEnable LTE,�������¸����õ�LTE����
        Enable LTE��Ӧ�ĳ���Ϊ:��LMM���������ʱ�յ�LmmDetachCnf����ҪDisable LTE,Ǩ����LmmStopCnf״̬
        �ڵȴ�LmmStopCnfʱ���յ�LmmAttachCnf������ҪEnable LTE,��ʱ���յ�LmmStopCnf��Ҫ����ȥLTE������ */
    
    /* ����������һ����Ҫ����������ʱ�������������������򣬽�������פ�� */
    if (VOS_TRUE == NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
    {
        if (NAS_MML_NET_RAT_TYPE_LTE == stDestPlmn.enRat)
        {
            /* ��LMM������������Ǩ��״̬��������������ʱ�� */
            NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
        }
        else
        {
            /* �������LMM���͹������󣬸��ݵ�ǰ��ͬ�Ľ��뼼����Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
            NAS_MMC_SndSuspendReq_PlmnSelection(NAS_MML_NET_RAT_TYPE_LTE);
        }
    }
    else
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

            NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();
    }

    return VOS_TRUE;


}
VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_PlmnSelection_WaitLSysInfoInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    NAS_MMC_SetAbortFlag_PlmnSelection(VOS_TRUE);

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvLSysInfoInd_PlmnSelection_WaitLSysInfoInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;

    MMA_MMC_PLMN_ID_STRU                stPlmnId;
    VOS_UINT32                          ulCurrentEventType;
    LMM_MMC_SYS_INFO_IND_STRU          *pstLmmSysInfoMsg    = VOS_NULL_PTR;

    /* ֹͣ�ȴ�ϵͳ��Ϣ�Ķ�ʱ��  */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_SYS_INFO);

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE  == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    if (VOS_FALSE == NAS_MMC_IsAllowedRegAfterAcqSucc_PlmnSelection())
    {
        /* �������ǻ�ȡ������MMA�ظ���ȡ�ɹ��������ϱ���ȡָʾ */
        pstLmmSysInfoMsg  = (LMM_MMC_SYS_INFO_IND_STRU*)pstMsg;
        NAS_MMC_ConvertLmmPlmnToGUNasFormat(&(pstLmmSysInfoMsg->stLteSysInfo.stSpecPlmnIdList.astSuitPlmnList[0]), &stPlmnId);

        if (NAS_BuildEventType(WUEPS_PID_MMA, ID_MMA_MMC_ACQ_REQ) == ulCurrentEventType)
        {
            NAS_MMC_SndMmaAcqCnf(MMC_MMA_ACQ_RESULT_SUCCESS, &stPlmnId, pstLmmSysInfoMsg->stLteSysInfo.usArfcn);

            NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection();
        }
        else
        {
            NAS_MMC_SndMmaAcqInd(MMC_MMA_ACQ_RESULT_SUCCESS, &stPlmnId, pstLmmSysInfoMsg->stLteSysInfo.usArfcn);
        }

        /* ״̬Ǩ�Ƶ�NAS_MMC_PLMN_SELECTION_STA_WAIT_MMA_REG_REQ */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_MMA_REG_REQ);

        /* ����������ʱ�� */
        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_MMA_REG_REQ, TI_NAS_MMC_WAIT_MMA_REG_REQ_LEN);

        return VOS_TRUE;
    }

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_LmmSysInfo(pstMsg);



    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

    /* ���µ�ǰ�ȴ�EPS��ע���� */
    NAS_MMC_ClearAllWaitRegRsltFlag_PlmnSelection();
    NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_PS);
    
#if (FEATURE_ON == FEATURE_IMS)    
    /* ���ӵȴ�IMS�Ƿ���ý��FLG,MMA��֤�յ�PSע�����ϱ�IMS�Ƿ����ָʾ��MMC */    
    NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_IMS);
#endif    

    /* ����CS��ע��Cause��AdditionalAction */
    NAS_MMC_SetCsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_SERVING_CELL_DOMAIN_NOT_SUPPORT);

    NAS_MMC_SetPsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_BUTT);

    NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_BUTT);
    NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_BUTT);

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_LTE);

    NAS_MMC_SndMmaCampOnInd(VOS_TRUE);

    /* ת��GMMϵͳ��Ϣ,��rabmʹ�� */
    NAS_MMC_SndGmmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);
    NAS_MMC_SndMmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    NAS_MMC_SndLmmLteSysInfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    /* Ǩ�Ƶ��ȴ�ע������״̬PLMN_SRCH:NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_REG_IND */
    NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_REG_IND);

    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_REG_IND, TI_NAS_MMC_WAIT_EPS_REG_IND_LEN);

    return VOS_TRUE;

}


VOS_UINT32 NAS_MMC_RcvTiWaitLSysInfoExpired_PlmnSelection_WaitLSysInfoInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    NAS_MML_PLMN_WITH_RAT_STRU          stDestPlmn;
    VOS_UINT32                          ulResult;

    /* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitLSysInfoExpired_PlmnSelection_WaitLSysInfoInd: Timer Expired");

    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);

    /*�����ǰ��Ҫ�˳�״̬��*/
    if (VOS_TRUE  == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);
    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());

    /*��ȡ��һ��Ҫ����������,����ȡʧ�ܣ�����ϲ�״̬���ϱ�����ʧ�ܣ��˳�
      ����ȡ�ɹ������ж���һ��Ҫ�����������Ƿ��뵱ǰפ��������ͬ��
      ����ͬ���ڵ�ǰģʽ�����������������ǰ����
     */
    ulResult = NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn);

    /*  ��ȡ�¸�����ʧ�ܣ�����г�����������*/
    if (VOS_TRUE != ulResult)
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();
        return VOS_TRUE;
    }

    /* ��MM��GMM��������ָʾ */
    NAS_MMC_SndGmmPlmnSchInit();
    NAS_MMC_SndMmPlmnSchInit();

    NAS_MMC_SndMmaCampOnInd(VOS_FALSE);

    NAS_MMC_UpdateRegStateSpecPlmnSearch();

    /*��һ��Ҫ�����������Ƿ��뵱ǰפ�����粻ͬ,��Ҫ����ǰģʽ���л����ȴ�����״̬������������ʱ��*/
    if (stDestPlmn.enRat != NAS_MML_GetCurrNetRatType())
    {
        NAS_MMC_SndSuspendReq_PlmnSelection(NAS_MML_GetCurrNetRatType());
    }
    /* ����ҪSuspend */
    else
    {
        /* �������LMM�����������󣬸��ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
    }


    return VOS_TRUE;

}





VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_PlmnSelection_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;


    /* ������������û�����Ĺػ�����ֱ���˳���ע����״̬�� */
    if (NAS_BuildEventType(WUEPS_PID_MMA, ID_MMA_MMC_POWER_OFF_REQ) == pstAbortMsg->ulEventType)
    {
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ��ǰû�����ӿ���ֱ���˳� */
    if ( VOS_FALSE == NAS_MML_IsRrcConnExist())
    {
        /* ֹͣ�ȴ�epsע������Ϣ�Ķ�ʱ��  */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����Abort��־ */
    NAS_MMC_SetAbortFlag_PlmnSelection(VOS_TRUE);

    /* �����Ҫ�����˳� */
    if (NAS_MMC_ABORT_FSM_IMMEDIATELY == pstAbortMsg->enAbortFsmType)
    {
        /* ֹͣ�ȴ�epsע������Ϣ�Ķ�ʱ��  */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* ���������ͷ����� */
        NAS_MMC_SndLmmRelReq();

        NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_CONN_REL_IND);
        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND, TI_NAS_MMC_WAIT_EPS_CONN_REL_IND_LEN);
    }

    return VOS_TRUE;

}


VOS_UINT32 NAS_MMC_RcvLmmMmcDetachInd_PlmnSelection_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_DETACH_IND_STRU                                *pstDetachMsg = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;

    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo;
    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;

    pstDetachMsg        = (LMM_MMC_DETACH_IND_STRU*)pstMsg;

    /* ����DETACH��Ϣ��������һ������ */
    NAS_MMC_ProcLmmDetachInd(pstDetachMsg, &enCsAdditionalAction, &enPsAdditionalAction);

    /*����EPS����һ���Ķ�������*/
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enPsAdditionalAction)
    {
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(enPsAdditionalAction);
    }

    /*����CS����һ���Ķ�������*/
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enCsAdditionalAction)
    {
        NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(enCsAdditionalAction);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn_PlmnSelection())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndLmmEquPlmnReq(&stSndEquPlmnInfo);

        NAS_MMC_SndOmEquPlmn();
    }

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvLmmMmcDetachCnf_PlmnSelection_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_DETACH_CNF_STRU                                *pstDetachCnfMsg = VOS_NULL_PTR;

    pstDetachCnfMsg        = (LMM_MMC_DETACH_CNF_STRU*)pstMsg;

    /* ����detach������Ǽ�Ȩ���ܣ�����Ҫ��״̬����Ԥ�����оͿ��Դ��� */
    if ( MMC_LMM_DETACH_RSLT_AUTH_REJ != pstDetachCnfMsg->ulDetachRslt)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmMmcDetachCnf_PlmnSelection_WaitEpsConnRelInd: Unexpected detach result!");

        return VOS_TRUE;
    }

    /*����EPS����һ���Ķ�������*/
    NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL);

    /*����CS����һ���Ķ�������*/
    NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL);

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvLmmMmcStatusInd_PlmnSelection_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_STATUS_IND_STRU            *pstLmmStatusIndMsg = VOS_NULL_PTR;

    pstLmmStatusIndMsg = (LMM_MMC_STATUS_IND_STRU*)pstMsg;

    if ( VOS_FALSE == pstLmmStatusIndMsg->bitOpConnState)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmMmcStatusInd_PlmnSelection_WaitEpsRegRsltInd:bitOpConnState IS VOS_FALSE");
        return VOS_TRUE;
    }

    /* ����LMM������״̬ */
    switch (pstLmmStatusIndMsg->ulConnState)
    {
        case MMC_LMM_CONN_ESTING:
        case MMC_LMM_CONNECTED_SIG:
            NAS_MML_SetEpsSigConnStatusFlg(VOS_TRUE);
            break;

        case MMC_LMM_CONNECTED_DATA:
            NAS_MML_SetEpsServiceConnStatusFlg(VOS_TRUE);
            break;

        case MMC_LMM_CONN_IDLE:
            NAS_MML_SetEpsSigConnStatusFlg(VOS_FALSE);
            NAS_MML_SetEpsServiceConnStatusFlg(VOS_FALSE);
            break;

        default:
            break;
    }

    return VOS_TRUE;

}
VOS_UINT32  NAS_MMC_RcvLmmMmcAttachInd_PlmnSelection_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsRegCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsRegCause;
    LMM_MMC_ATTACH_IND_STRU            *pstLmmAttachIndMsg = VOS_NULL_PTR;
    NAS_MMC_FSM_ID_ENUM_UINT32          enCurrentFsmId;
    VOS_UINT32                          ulCurrentEventType;

    NAS_MML_LTE_UE_OPERATION_MODE_ENUM_UINT8                enUeOperationMode;

    enUeOperationMode   =  NAS_MML_GetLteUeOperationMode();
    pstLmmAttachIndMsg  = (LMM_MMC_ATTACH_IND_STRU*)pstMsg;

    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* LMM������δ�ظ�search cnfʱ�յ��û�detach ps����󣬺���������ע���mmc�ظ�attach result
       ΪMMC_LMM_ATT_RSLT_PS_ATT_NOT_ALLOW���������������MMC_LMM_ATT_TYPE_BUTT��
       MMC��Ҫ���ݵ�ǰue operation mode�������������¸�ֵ */
    if ((MMC_LMM_ATT_TYPE_BUTT == pstLmmAttachIndMsg->ulReqType)
     && (MMC_LMM_ATT_RSLT_PS_ATT_NOT_ALLOW == pstLmmAttachIndMsg->ulAttachRslt))
    {
        pstLmmAttachIndMsg->ulReqType = NAS_MMC_ConvertLmmAttachReqType(enUeOperationMode);
    }

    /* ��GMM��MMת��ע������Ϣ */
    NAS_MMC_SndMmLmmAttachInd(pstLmmAttachIndMsg);
    NAS_MMC_SndGmmLmmAttachInd(pstLmmAttachIndMsg);

    /* ����attach��� */
    NAS_MMC_ProcLmmAttachInd_PlmnSelection(pstLmmAttachIndMsg);

    /* �Ѿ�Disable L����ע��ɹ�,��Ҫ����ǰ���籣����״̬����������,���ں���
       GU����ʧ�����»ظ������ṩPSҵ��*/
    if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == NAS_MML_GetLteCapabilityStatus())
      && (MMC_LMM_ATT_RSLT_SUCCESS == pstLmmAttachIndMsg->ulAttachRslt))
    {
        /* ���浱ǰ������״̬���������� */
        NAS_MMC_SetCsPsMode1ReCampLtePlmn_PlmnSelection(NAS_MML_GetCurrCampPlmnId());
    }

    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();
    if (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType))
    {
        enCsRegCause = NAS_MMC_GetCsRegCause_PlmnSelection();
        enPsRegCause = NAS_MMC_GetPsRegCause_PlmnSelection();

        if (VOS_TRUE == NAS_MMC_ProcUserSearchRegRslt_PlmnSelection(enCsRegCause,enPsRegCause))
        {
            /*���������ϢΪ��ͨ���������󣬺������µ��û�����ʱ�����Դ�ϸ�״̬��*/
            NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection();
        }
    }

    if (NAS_MMC_WAIT_REG_RESULT_IND_NULL != NAS_MMC_GetWaitRegRsltFlag_PlmnSelection())
    {
        return VOS_TRUE;
    }

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {

        if (VOS_TRUE == NAS_MMC_IsDisableLteNeedLocalReleaseEpsConn())
        {
            NAS_MMC_SndLmmRelReq();

            NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_CONN_REL_IND);

            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND, TI_NAS_MMC_WAIT_EPS_CONN_REL_IND_LEN);

            return VOS_TRUE;
        }


        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֹͣ�ȴ�epsע������Ϣ�Ķ�ʱ��  */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    NAS_MMC_PerformAdditionalActionRegFinished_PlmnSelection();

    return VOS_TRUE;

}
VOS_UINT32  NAS_MMC_RcvLmmMmcTauResultInd_PlmnSelection_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsRegCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsRegCause;
    LMM_MMC_TAU_RESULT_IND_STRU        *pstLmmTauIndMsg = VOS_NULL_PTR;
    NAS_MMC_FSM_ID_ENUM_UINT32          enCurrentFsmId;
    VOS_UINT32                          ulCurrentEventType;

    pstLmmTauIndMsg  = (LMM_MMC_TAU_RESULT_IND_STRU*)pstMsg;

    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* ��GMM��MMת��ע������Ϣ */
    NAS_MMC_SndMmLmmTauResultInd(pstLmmTauIndMsg);
    NAS_MMC_SndGmmLmmTauResultInd(pstLmmTauIndMsg);

    /* ����tau��� */
    NAS_MMC_ProcLmmTauResultInd_PlmnSelection(pstLmmTauIndMsg);

    /* �Ѿ�Disable L����ע��ɹ�,��Ҫ����ǰ���籣����״̬����������,���ں���
       GU����ʧ�����»ظ������ṩPSҵ��*/
    if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == NAS_MML_GetLteCapabilityStatus())
      && (MMC_LMM_TAU_RSLT_SUCCESS == pstLmmTauIndMsg->ulTauRst))
    {
        /* ���浱ǰ������״̬���������� */
        NAS_MMC_SetCsPsMode1ReCampLtePlmn_PlmnSelection(NAS_MML_GetCurrCampPlmnId());
    }

    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();
    if (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType))
    {
        enCsRegCause = NAS_MMC_GetCsRegCause_PlmnSelection();
        enPsRegCause = NAS_MMC_GetPsRegCause_PlmnSelection();

        if (VOS_TRUE == NAS_MMC_ProcUserSearchRegRslt_PlmnSelection(enCsRegCause,enPsRegCause))
        {
            /*���������ϢΪ��ͨ���������󣬺������µ��û�����ʱ�����Դ�ϸ�״̬��*/
            NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection();
        }
    }

    if (NAS_MMC_WAIT_REG_RESULT_IND_NULL != NAS_MMC_GetWaitRegRsltFlag_PlmnSelection())
    {
        return VOS_TRUE;
    }

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֹͣ�ȴ�epsע������Ϣ�Ķ�ʱ��  */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    NAS_MMC_PerformAdditionalActionRegFinished_PlmnSelection();

    return VOS_TRUE;

}


VOS_UINT32 NAS_MMC_RcvLmmMmcDetachInd_PlmnSelection_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_DETACH_IND_STRU                                *pstDetachMsg = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;

    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo;
    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;

    pstDetachMsg        = (LMM_MMC_DETACH_IND_STRU*)pstMsg;

    /* ����DETACH��Ϣ��������һ������ */
    NAS_MMC_ProcLmmDetachInd(pstDetachMsg, &enCsAdditionalAction, &enPsAdditionalAction);

    /*����EPS����һ���Ķ�������*/
    if (NAS_MMC_ADDITIONAL_ACTION_BUTT != enPsAdditionalAction)
    {
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(enPsAdditionalAction);
    }

    /*����CS����һ���Ķ�������*/
    if (NAS_MMC_ADDITIONAL_ACTION_BUTT != enCsAdditionalAction)
    {
        NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(enCsAdditionalAction);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn_PlmnSelection())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndLmmEquPlmnReq(&stSndEquPlmnInfo);

        NAS_MMC_SndOmEquPlmn();
    }

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֹͣ�ȴ�epsע������Ϣ�Ķ�ʱ��  */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    NAS_MMC_PerformAdditionalActionRegFinished_PlmnSelection();

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvLmmMmcServiceRsltInd_PlmnSelection_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_SERVICE_RESULT_IND_STRU                        *pstSerRsltMsg = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

    pstSerRsltMsg     = (LMM_MMC_SERVICE_RESULT_IND_STRU*)pstMsg;

    /* ����LMM��service result���������һ�������� */
    enAdditionalAction = NAS_MMC_ProcLmmServiceRsltInd(pstSerRsltMsg);

    /* AdditionalAction��Ч������EPS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(enAdditionalAction);
    }

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֹͣ�ȴ�epsע������Ϣ�Ķ�ʱ��  */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    NAS_MMC_PerformAdditionalActionRegFinished_PlmnSelection();

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvLmmSuspendInd_PlmnSelection_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    LMM_MMC_SUSPEND_IND_STRU           *pstLSuspendMsg = VOS_NULL_PTR;
    MMC_SUSPEND_CAUSE_ENUM_UINT8        enSuspendCause;

    pstLSuspendMsg = (LMM_MMC_SUSPEND_IND_STRU*)pstMsg;

    /* Lģ�£�����ԭ������GU�²�ͬ��ת��һ�¹���ԭ���� */
    NAS_MMC_ConvertLmmSysChngTypeToMmcType(pstLSuspendMsg->ulSysChngType, &enSuspendCause);

    /* ���ΪOOS���߲�������ȷ, �����������ϵͳ��ѡ */
    if ((MMC_SUSPEND_CAUSE_PLMNSEARCH == enSuspendCause)
     || (MMC_SUSPEND_CAUSE_BUTT <= enSuspendCause))
    {
        NAS_MMC_SndLmmSuspendRsp(MMC_LMM_FAIL);
    }
    else
    {
        /* ����SUSPEND��Ϣ�еĲ�ͬ��������Ӧ��״̬�� */
        NAS_MMC_LoadInterSysFsm_PlmnSelection(enSuspendCause);
    }


    return VOS_TRUE;

}
VOS_UINT32 NAS_MMC_RcvMmcMmcSuspendRslt_PlmnSelection_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRatType;

#if (FEATURE_ON == FEATURE_CL_INTERWORK)
    NAS_MMCMMC_SUSPEND_RSLT_STRU       *pstSuspendRslt = VOS_NULL_PTR;
#endif

     /* Ǩ�Ƶ���ͬ�Ľ��뼼��ʱ����ҪǨ�Ƶ���ͬ�Ĵ���ע����״̬ */

     enRatType = NAS_MML_GetCurrNetRatType();

    if ((NAS_MML_NET_RAT_TYPE_WCDMA == enRatType)
     || (NAS_MML_NET_RAT_TYPE_GSM == enRatType))
    {
        /* ֹͣ��ǰ�ȴ�EPSע�����Ķ�ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* ���µ�ǰ�ȴ�CSPS��ע���� */
        NAS_MMC_ClearAllWaitRegRsltFlag_PlmnSelection();
        NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_CS);
        NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_PS);

        /* ����CSPS��AdditionalAction */
        NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_BUTT);
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_BUTT);

        /* ����CSPS��RegCause */
        NAS_MMC_SetCsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_BUTT);
        NAS_MMC_SetPsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_BUTT);


        /* �յ�ϵͳ��Ϣ��Ǩ�Ƶ��ȴ�CS+PS���ע������������ʱ�� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_CSPS_REG_IND);

        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND, TI_NAS_MMC_WAIT_CSPS_REG_IND_LEN);


        return VOS_TRUE;
    }


#if (FEATURE_ON == FEATURE_CL_INTERWORK)
    pstSuspendRslt  = (NAS_MMCMMC_SUSPEND_RSLT_STRU*)pstMsg;

    /* ��ѡ��HRPD��,�˳�ѡ��״̬�� */
    if ((NAS_MML_NET_RAT_TYPE_BUTT == enRatType)
     && (NAS_MMC_SUSPEND_SUCCESS == pstSuspendRslt->enRslt))
    {
        /* ֹͣ��ǰ�ȴ�EPSע�����Ķ�ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* ���ϲ�״̬������������� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_INTER_SYS_HRPD);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }
#endif

    NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvMmcMmcSuspendRslt_PlmnSelection_WaitEpsRegRsltInd:ENTERED");
    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvTiWaitEpsRegRsltIndExpired_PlmnSelection_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_FSM_ID_ENUM_UINT32              enCurrentFsmId;
    VOS_UINT32                              ulCurrentEventType;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8    enAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8    enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8    enPsAdditionalAction;

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitEpsRegRsltIndExpired_PlmnSelection_WaitEpsRegRsltInd: Timer Expired");

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();

    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }

    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();
    if (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType))
    {
        NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_FAIL);

        /*���������ϢΪ��ͨ���������󣬺������µ��û�����ʱ�����Դ�ϸ�״̬��*/
        NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection();

        /* ��LMM�����û���������֪ͨ */
        if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
        {
            NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
        }
    }

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    enCsAdditionalAction = NAS_MMC_GetCsRegAdditionalAction_PlmnSelection();
    enPsAdditionalAction = NAS_MMC_GetPsRegAdditionalAction_PlmnSelection();

    /* �ȴ�CS PS��ע����ʱ���п���һ������ע�������˺����Լ��������е�ҵ��
      (����G�½���CS����к��У�PS���޷�����ע��),����MMCһ�µȲ�������һ�����
      ע��������ʱMMCֱ��ͨ��additional��������������MMC��ʱ���ݵ�ǰ�յ���ע
      �������µ�additionalֵ�������Ƿ�ɹ��˳�����״̬������ʧ���˳�����״̬����
      ��L1״̬����������״̬�����˳���������Ƿ�����������ѡ����ʱ�����Ǽ����ȴ�
      ҵ������Լ�CSPS��ע���� */

    enAdditionalAction = NAS_MMC_GetPrioAddtionalAction(enCsAdditionalAction, enPsAdditionalAction);

    /* ���Ӳ���Ϊפ����ǰ���� */
    if ((NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_ACCESS_BARRED == enAdditionalAction))
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_SUCC);
    }
    else
    {
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);
    }

    /* ֱ���˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;

}
VOS_UINT32 NAS_MMC_RcvLmmAreaLostInd_PlmnSelection_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_WITH_RAT_STRU          stDestPlmn;
    VOS_UINT32                          ulResult;

    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo           = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stCurPlmn;

    /* ����汾Coverity���� */
    PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    /* ��ȡ��ǰפ����������Ϣ */
    pstCurCampInfo     = NAS_MML_GetCurrCampPlmnInfo();
    stCurPlmn.stPlmnId = pstCurCampInfo->stLai.stPlmnId;
    stCurPlmn.enRat    = pstCurCampInfo->enNetRatType;

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    /* ���µ�ǰ���������״̬Ϊ��ע������ڴ��������У��������Ѵ����� */
    NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&stCurPlmn,
                                            NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,
                                            pstPlmnSelectionListInfo);

    /* ֹͣ�ȴ�Epsע������Ϣ�Ķ�ʱ��  */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE  == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ��յȴ�ע������־ */
    NAS_MMC_ClearAllWaitRegRsltFlag_PlmnSelection();

    /* �����NCELL����������Ҫ���������������ˣ��˳�״̬�� */
    if (NAS_MMC_PLMN_SEARCH_SCENE_REG_NCELL == NAS_MMC_GetPlmnSearchScene_PlmnSelection())
    {
        /* ���ϲ�״̬��ָʾѡ��ʧ�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ��ȡ��һ��Ҫ����������,����ȡʧ�ܣ�����ϲ�״̬���ϱ�����ʧ�ܣ��˳�
      ����ȡ�ɹ������ж���һ��Ҫ�����������Ƿ��뵱ǰפ��������ͬ��
      ����ͬ���ڵ�ǰģʽ�����������������ǰ���� */
    ulResult = NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn);

    /*  ��ȡ�¸�����ʧ�ܣ�����г����������� */
    if (VOS_TRUE != ulResult)
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();
        return VOS_TRUE;
    }

    /* ��MM��GMM��������ָʾ */
    NAS_MMC_SndGmmPlmnSchInit();
    NAS_MMC_SndMmPlmnSchInit();

    NAS_MMC_SndMmaCampOnInd(VOS_FALSE);

    NAS_MMC_UpdateRegStateSpecPlmnSearch();

    /* ��һ��Ҫ�����������Ƿ��뵱ǰפ�����粻ͬ,��Ҫ����ǰģʽ���л����ȴ�����״̬������������ʱ�� */
    if (stDestPlmn.enRat != NAS_MML_GetCurrNetRatType())
    {
        NAS_MMC_SndSuspendReq_PlmnSelection(NAS_MML_GetCurrNetRatType());
    }
    else
    {
        /* �������LMM�����������󣬸��ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
    }

    return VOS_TRUE;

}


VOS_UINT32 NAS_MMC_RcvLSysInfoInd_PlmnSelection_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_LmmSysInfo(pstMsg);



    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_LTE);

    NAS_MMC_SndMmaCampOnInd(VOS_TRUE);

    /* ת��GMMϵͳ��Ϣ,��rabmʹ�� */
    NAS_MMC_SndGmmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);
    NAS_MMC_SndMmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    NAS_MMC_SndLmmLteSysInfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    return VOS_TRUE;

}
VOS_UINT32 NAS_MMC_RcvTafPlmnSpecialReq_PlmnSelection_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    MMA_MMC_PLMN_SPECIAL_REQ_STRU                          *pstUserSelReqMsg  = VOS_NULL_PTR;

    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSrchList   = VOS_NULL_PTR;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enPsRegCause;

    pstUserSelReqMsg    = (MMA_MMC_PLMN_SPECIAL_REQ_STRU*)pstMsg;

    /* �����û�ָ��������Ϣ�����µ�ǰ����������Ϊ�ֶ�ģʽ */
    NAS_MMC_UpdateUserSpecPlmnSearchInfo_PlmnSelection((NAS_MML_PLMN_ID_STRU*)(&pstUserSelReqMsg->stPlmnId),
                                                        pstUserSelReqMsg->enAccessMode);

    /* ��ȡѡ���б� */
    pstPlmnSrchList = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    /* �����û�ָ�������������³�ʼ��ѡ���б� */
    NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_USER_SPEC_PLMN_SEARCH,
                                  VOS_NULL_PTR,
                                  pstPlmnSrchList);

    /* ���û�ָ�����뼼��������(Ҳ�ǵ�ǰ����)����Ϊ������ע������ظ�ע�� */
    NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(NAS_MMC_GetUserSpecPlmnId(),
                                                   NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,
                                                   pstPlmnSrchList);

    enCsRegCause = NAS_MMC_GetCsRegCause_PlmnSelection();
    enPsRegCause = NAS_MMC_GetPsRegCause_PlmnSelection();

    if (VOS_TRUE == NAS_MMC_ProcUserSearchRegRslt_PlmnSelection(enCsRegCause, enPsRegCause))
    {
        return VOS_TRUE;
    }

    /* ���������Ϣ */
    NAS_MMC_SaveCurEntryMsg(ulEventType, pstMsg);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvTafSysCfgReq_PlmnSelection_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֱ�ӵ���syscfg���״̬�� */
    NAS_MMC_FSM_InitFsmL2(NAS_MMC_FSM_SYSCFG);
    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvMmcSyscfgRsltCnf_PlmnSelection_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvMmaImsVoiceCapInd_PlmnSelection_WaitEpsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMA_MMC_IMS_VOICE_CAP_IND_STRU     *pstImsVoiceInd = VOS_NULL_PTR;
    VOS_UINT8                           ucWaitRegRsltFlag;
    
    ucWaitRegRsltFlag   = NAS_MMC_GetWaitRegRsltFlag_PlmnSelection();
    pstImsVoiceInd      = (MMA_MMC_IMS_VOICE_CAP_IND_STRU *)pstMsg;
    
    /* ����ڵȴ�PS��ע����������ȴ�PSע���� */
    if (NAS_MMC_WAIT_REG_RESULT_IND_PS == (ucWaitRegRsltFlag & NAS_MMC_WAIT_REG_RESULT_IND_PS))
    {
        return VOS_TRUE;
    }

    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_IMS);        

    /* IMS������ʱ�ж��Ƿ���Ҫdisable LTE */
    if (VOS_FALSE  == pstImsVoiceInd->ucAvail) 
    {
        if (VOS_TRUE == NAS_MMC_IsNeedDisableLte_ImsVoiceNotAvail())
        {
            NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION);

            NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS);

            NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_LTE_VOICE_NOT_AVAILABLE);

            /* �Ѿ�Disable L����ע��ɹ�,��Ҫ����ǰ���籣����״̬����������,���ں���
               GU����ʧ�����»ظ������ṩPSҵ��*/
            NAS_MMC_SetCsPsMode1ReCampLtePlmn_PlmnSelection(NAS_MML_GetCurrCampPlmnId());
        }
    }

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        if (VOS_TRUE == NAS_MMC_IsDisableLteNeedLocalReleaseEpsConn())
        {
            NAS_MMC_SndLmmRelReq();

            NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_CONN_REL_IND);

            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND, TI_NAS_MMC_WAIT_EPS_CONN_REL_IND_LEN);

            return VOS_TRUE;
        }

        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֹͣ�ȴ�epsע������Ϣ�Ķ�ʱ��  */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    NAS_MMC_PerformAdditionalActionRegFinished_PlmnSelection();

    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvMmcAbortFsmMsg_PlmnSelection_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;

    /* ������������û�����Ĺػ�����ֱ���˳��������ͷŽ��״̬�� */
    if (NAS_BuildEventType(WUEPS_PID_MMA, ID_MMA_MMC_POWER_OFF_REQ) == pstAbortMsg->ulEventType)
    {
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND);

        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����Abort��־, �������ͷź����quit */
    NAS_MMC_SetAbortFlag_PlmnSelection(VOS_TRUE);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmcSyscfgRsltCnf_PlmnSelection_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvLmmMmcStatusInd_PlmnSelection_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_STATUS_IND_STRU            *pstLmmStatusIndMsg = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAdditionalAction;

    pstLmmStatusIndMsg  = ( LMM_MMC_STATUS_IND_STRU* )pstMsg;


    /* ����EPS������Ӵ���״̬ */
    if ( (MMC_LMM_CONN_ESTING == pstLmmStatusIndMsg->ulConnState)
      || (MMC_LMM_CONNECTED_SIG == pstLmmStatusIndMsg->ulConnState) )
    {
        /* ���ô���EPS�������� */
        NAS_MML_SetEpsSigConnStatusFlg(VOS_TRUE);
    }
    else if (MMC_LMM_CONNECTED_DATA == pstLmmStatusIndMsg->ulConnState)
    {
        /* ���ô���EPSҵ������ */
        NAS_MML_SetEpsServiceConnStatusFlg(VOS_TRUE);
    }
    else if (MMC_LMM_CONN_IDLE == pstLmmStatusIndMsg->ulConnState)
    {
        /* ���ò�����EPS�������� */
        NAS_MML_SetEpsSigConnStatusFlg(VOS_FALSE);
        NAS_MML_SetEpsServiceConnStatusFlg(VOS_FALSE);
    }
    else
    {
        ;
    }

    /*  ��������������ӣ�������ȴ�  */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND);

    /* �Ƿ���Ҫ Abort״̬�� */
    if ( VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection() )
    {
        /* ���ϲ�״̬��ָʾѡ��ABORT */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����������Ӳ����ڶ��Ҳ��������ͷ���������и��Ӳ��� */
    if (VOS_FALSE == NAS_MMC_GetRelRequestFlag_PlmnSelection())
    {
        NAS_MMC_PerformAdditionalActionConnRel_PlmnSelection();
    }
    else
    {
        NAS_MMC_SetRelRequestFlag_PlmnSelection(VOS_FALSE);

        /* �����ͷ�����ʱ,����additionҲ���ܵ�������,ǰ���յ�ע����δˢ��ѡ���б�,�˴���Ҫ��ˢ��һ�� */
        enAdditionalAction   = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_GetCsRegAdditionalAction_PlmnSelection(), 
                                                              NAS_MMC_GetPsRegAdditionalAction_PlmnSelection());
        
        NAS_MMC_RefreshPlmnSelectionListAfterRegFail_PlmnSelection(enAdditionalAction);

        NAS_MMC_SearchNextPlmn_PlmnSelection();
    }


    return VOS_TRUE;

}


VOS_UINT32 NAS_MMC_RcvTiWaitEpsConnRelIndExpired_PlmnSelection_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitEpsConnRelIndExpired_PlmnSelection_WaitEpsConnRelInd ENTERED");


    if ( VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection() )
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����������Ӳ����ڶ��Ҳ��������ͷ���������и��Ӳ��� */
    if (VOS_FALSE == NAS_MMC_GetRelRequestFlag_PlmnSelection())
    {
        /* ���������ͷ�����MM��GMM�����������ȴ������ͷŶ�ʱ�� */
        NAS_MMC_SetRelRequestFlag_PlmnSelection(VOS_TRUE);

        NAS_MMC_SndLmmRelReq();
        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND, TI_NAS_MMC_WAIT_EPS_CONN_REL_IND_LEN);
    }
    else
    {
        NAS_MMC_SetRelRequestFlag_PlmnSelection(VOS_FALSE);

        NAS_MMC_SearchNextPlmn_PlmnSelection();
    }


    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLSysInfoInd_PlmnSelection_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU        *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU         stOldCampInfo;

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

    /* ��������������ͷ�,������Ҫ������������,ϵͳ��Ϣ������ */
    if (VOS_TRUE == NAS_MMC_GetRelRequestFlag_PlmnSelection())
    {
        return VOS_TRUE;
    }

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_LmmSysInfo(pstMsg);
    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);



    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_LTE);

    NAS_MMC_SndMmaCampOnInd(VOS_TRUE);

    /* ת��GMMϵͳ��Ϣ,��rabmʹ�� */
    NAS_MMC_SndGmmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);
    NAS_MMC_SndMmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    NAS_MMC_SndLmmLteSysInfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    return VOS_TRUE;

}


VOS_UINT32 NAS_MMC_RcvTafSysCfgReq_PlmnSelection_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֱ�ӵ���syscfg���״̬�� */
    NAS_MMC_FSM_InitFsmL2(NAS_MMC_FSM_SYSCFG);

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvLmmSuspendInd_PlmnSelection_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    LMM_MMC_SUSPEND_IND_STRU           *pstLSuspendMsg = VOS_NULL_PTR;
    MMC_SUSPEND_CAUSE_ENUM_UINT8        enSuspendCause;
    VOS_UINT32                          ulRelRequestFlg;

    pstLSuspendMsg = (LMM_MMC_SUSPEND_IND_STRU*)pstMsg;

    ulRelRequestFlg = NAS_MMC_GetRelRequestFlag_PlmnSelection();

    /* Lģ�£�����ԭ������GU�²�ͬ��ת��һ�¹���ԭ���� */
    NAS_MMC_ConvertLmmSysChngTypeToMmcType(pstLSuspendMsg->ulSysChngType, &enSuspendCause);

    /* ����������ͷŻ����Ѿ����յ�Abort��Ϣ,�����������ϵͳ��ѡ */
    if ((VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
     || (VOS_TRUE == ulRelRequestFlg)
     || (MMC_SUSPEND_CAUSE_PLMNSEARCH == enSuspendCause)
     || (MMC_SUSPEND_CAUSE_BUTT <= enSuspendCause))
    {
        NAS_MMC_SndLmmSuspendRsp(MMC_LMM_FAIL);
    }
    else
    {
        /* ����SUSPEND��Ϣ�еĲ�ͬ��������Ӧ��״̬�� */
        NAS_MMC_LoadInterSysFsm_PlmnSelection(enSuspendCause);
    }


    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvMmcSuspendRslt_PlmnSelection_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRatType;

#if (FEATURE_ON == FEATURE_CL_INTERWORK)
    NAS_MMCMMC_SUSPEND_RSLT_STRU       *pstSuspendRslt = VOS_NULL_PTR;
#endif

    /* Ǩ�Ƶ���ͬ�Ľ��뼼��ʱ����ҪǨ�Ƶ���ͬ�Ĵ���ע����״̬ */
    enRatType = NAS_MML_GetCurrNetRatType();

    if ((NAS_MML_NET_RAT_TYPE_WCDMA == enRatType)
     || (NAS_MML_NET_RAT_TYPE_GSM == enRatType))
    {
        /* ֹͣ��ǰ�ȴ�eps�����ͷŶ�ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND);

        /* ���µ�ǰ�ȴ�CSPS��ע���� */
        NAS_MMC_ClearAllWaitRegRsltFlag_PlmnSelection();
        NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_CS);
        NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_PS);

        /* ����CSPS��AdditionalAction */
        NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_BUTT);
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_BUTT);

        /* ����CSPS��RegCause */
        NAS_MMC_SetCsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_BUTT);
        NAS_MMC_SetPsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_BUTT);


        /* �յ�ϵͳ��Ϣ��Ǩ�Ƶ��ȴ�CS+PS���ע������������ʱ�� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_CSPS_REG_IND);

        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND, TI_NAS_MMC_WAIT_CSPS_REG_IND_LEN);


        return VOS_TRUE;
    }

#if (FEATURE_ON == FEATURE_CL_INTERWORK)
    pstSuspendRslt  = (NAS_MMCMMC_SUSPEND_RSLT_STRU*)pstMsg;

    /* ��ѡ��HRPD��,�˳�ѡ��״̬�� */
    if ((NAS_MML_NET_RAT_TYPE_BUTT == enRatType)
     && (NAS_MMC_SUSPEND_SUCCESS == pstSuspendRslt->enRslt))
    {
        /* ֹͣ��ǰ�ȴ�eps�����ͷŶ�ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND);

        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_INTER_SYS_HRPD);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }
#endif

    /*  ��������������ӣ�������ȴ�  */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL);

    if ( VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection() )
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����������Ӳ����ڿ��ܷ������� */
    NAS_MMC_PerformAdditionalActionConnRel_PlmnSelection();


    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvTafPlmnSpecialReq_PlmnSelection_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMA_MMC_PLMN_SPECIAL_REQ_STRU                          *pstUserSelReqMsg  = VOS_NULL_PTR;

    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSrchList;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enPsCause;

    pstUserSelReqMsg    = (MMA_MMC_PLMN_SPECIAL_REQ_STRU*)pstMsg;

    /* �����ֶ�������Ϣ */
    NAS_MMC_UpdateUserSpecPlmnSearchInfo_PlmnSelection((NAS_MML_PLMN_ID_STRU*)(&pstUserSelReqMsg->stPlmnId),
                                                       pstUserSelReqMsg->enAccessMode);

    /* ��ȡѡ���б� */
    pstPlmnSrchList = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    /* �����û�ָ�������������³�ʼ��ѡ���б� */
    NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_USER_SPEC_PLMN_SEARCH,
                                  VOS_NULL_PTR,
                                  pstPlmnSrchList);

    /* ���û�ָ�����뼼��������(Ҳ�ǵ�ǰ����)����Ϊ������ע������ظ�ע�� */
    NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(NAS_MMC_GetUserSpecPlmnId(),
                                                   NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,
                                                   pstPlmnSrchList);

    /* �����û�ָ����PlmnId��RAT�͵�ǰפ����PlmnId��RAT��ͬ��ֱ�ӷ���ָ��������� */
    enCsCause = NAS_MMC_GetCsRegCause_PlmnSelection();
    enPsCause = NAS_MMC_GetPsRegCause_PlmnSelection();

    if (VOS_TRUE == NAS_MMC_ProcUserSearchRegRslt_PlmnSelection(enCsCause, enPsCause))
    {
        return VOS_TRUE;
    }

    /* ���������Ϣ */
    NAS_MMC_SaveCurEntryMsg(ulEventType, pstMsg);

    return VOS_TRUE;

}


VOS_UINT32 NAS_MMC_RcvLmmMmcServiceRsltInd_PlmnSelection_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_SERVICE_RESULT_IND_STRU                        *pstSerRsltMsg = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

    pstSerRsltMsg     = (LMM_MMC_SERVICE_RESULT_IND_STRU*)pstMsg;

    /* ����LMM��service result���������һ�������� */
    enAdditionalAction = NAS_MMC_ProcLmmServiceRsltInd(pstSerRsltMsg);

    /* AdditionalAction��Ч������EPS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(enAdditionalAction);
    }

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֹͣ�ȴ�epsע������Ϣ�Ķ�ʱ��  */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    NAS_MMC_PerformAdditionalActionRegFinished_PlmnSelection();

    return VOS_TRUE;
}




VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_PlmnSelection_WaitAsSuspendCnfCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��¼��Ҫ��ֹ��ǰ״̬����־ */
    NAS_MMC_SetAbortFlag_PlmnSelection(VOS_TRUE);

    return VOS_TRUE;
}

VOS_UINT32 NAS_MMC_RcvRrMmSuspendCnf_PlmnSelection_WaitWasSuspendCnfCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SUSPEND_CNF_ST                *pstSuspendCnf = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU          stReCampLtePlmn;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurRat;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU     stDestPlmnList;

    pstSuspendCnf            = (RRMM_SUSPEND_CNF_ST*)pstMsg;
    stReCampLtePlmn.enRat    = NAS_MML_NET_RAT_TYPE_LTE;

    PS_MEM_CPY(&(stReCampLtePlmn.stPlmnId),
               NAS_MMC_GetCsPsMode1ReCampLtePlmn_PlmnSelection(),
               sizeof(stReCampLtePlmn.stPlmnId));

    enCurRat      = NAS_MML_GetCurrNetRatType();

    /* ֹͣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_WAS_SUSPEND_CNF);

    if (MMC_AS_SUSPEND_FAILURE == pstSuspendCnf->ucResult)
    {
        /* ���ɻָ�����,���õ���ӿ�RESET */
        if ( (NAS_UTRANCTRL_UTRAN_MODE_FDD == NAS_UTRANCTRL_GetCurrUtranMode())
          && (NAS_MML_NET_RAT_TYPE_WCDMA == enCurRat) )
        {
            NAS_MML_SoftReBoot();
        }
        else
        {
            NAS_MML_SoftReBoot();
        }

        return VOS_TRUE;
    }

    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* ���õ�ǰ���뼼��ΪBUTT */
        NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_BUTT);

        /* �ظ�Plmn Selectionִ�н�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ������Ҫ���»�LTE�����磬
       ���ڹ�������п��ܽ��յ��û�Detach PS�������˴���Ҫ���ж������Ƿ���Ч */
    if (VOS_TRUE == NAS_MMC_IsReCampLtePlmnValid_PlmnSelection())
    {
        /* ���õ�ǰ���뼼��ΪLTE */
        NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_LTE);

        /* ��LMM����ָ������ */
        PS_MEM_SET(&stDestPlmnList, 0x0, sizeof(stDestPlmnList));

        stDestPlmnList.ulPlmnNum = 1;
        PS_MEM_CPY(&stDestPlmnList.astPlmnId[0], &stReCampLtePlmn, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

        NAS_MMC_SndLmmSpecPlmnSearchReq(&stDestPlmnList, VOS_FALSE);

        /* ״̬Ǩ��NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_PLMN_SEARCH_CNF_CSPSMODE1_RECAMP_LTE */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_PLMN_SEARCH_CNF_CSPSMODE1_RECAMP_LTE);

        /* ����������ʱ��TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF */
        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF_LEN);
    }
    else
    {
        /* ���õ�ǰ���뼼��ΪBUTT */
        NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_BUTT);

        /* �ظ�Plmn Selectionִ�н�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }

    return VOS_TRUE;
}

VOS_UINT32 NAS_MMC_RcvRrMmSuspendCnf_PlmnSelection_WaitGasSuspendCnfCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_SUSPEND_CNF_ST                *pstSuspendCnf = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU          stReCampLtePlmn;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU     stDestPlmnList;

    pstSuspendCnf            = (RRMM_SUSPEND_CNF_ST*)pstMsg;
    stReCampLtePlmn.enRat    = NAS_MML_NET_RAT_TYPE_LTE;

    PS_MEM_CPY(&(stReCampLtePlmn.stPlmnId),
               NAS_MMC_GetCsPsMode1ReCampLtePlmn_PlmnSelection(),
               sizeof(stReCampLtePlmn.stPlmnId));

    /* ֹͣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_GAS_SUSPEND_CNF);

    if (MMC_AS_SUSPEND_FAILURE == pstSuspendCnf->ucResult)
    {
        /* ���ɻָ�����,���õ���ӿ�RESET */
        NAS_MML_SoftReBoot();

        return VOS_TRUE;
    }

    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* ���õ�ǰ���뼼��ΪBUTT */
        NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_BUTT);

        /* �ظ�Plmn Selectionִ�н�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ������Ҫ���»�LTE�����磬
       ���ڹ�������п��ܽ��յ��û�Detach PS�������˴���Ҫ���ж������Ƿ���Ч */
    if (VOS_TRUE == NAS_MMC_IsReCampLtePlmnValid_PlmnSelection())
    {
        /* ���õ�ǰ���뼼��ΪLTE */
        NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_LTE);

        /* ��LMM����ָ������ */
        PS_MEM_SET(&stDestPlmnList, 0x0, sizeof(stDestPlmnList));

        stDestPlmnList.ulPlmnNum = 1;
        PS_MEM_CPY(&stDestPlmnList.astPlmnId[0], &stReCampLtePlmn, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

        NAS_MMC_SndLmmSpecPlmnSearchReq(&stDestPlmnList, VOS_FALSE);

        /* ״̬Ǩ��NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_PLMN_SEARCH_CNF_CSPSMODE1_RECAMP_LTE */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_PLMN_SEARCH_CNF_CSPSMODE1_RECAMP_LTE);

        /* ����������ʱ��TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF */
        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF_LEN);
    }
    else
    {
        /* ���õ�ǰ���뼼��ΪBUTT */
        NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_BUTT);

        /* �ظ�Plmn Selectionִ�н�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }

    return VOS_TRUE;
}

VOS_UINT32 NAS_MMC_RcvTiWaitAsSuspendCnfExpired_PlmnSelection_WaitAsSuspendCnfCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurRat;

    enCurRat      = NAS_MML_GetCurrNetRatType();

    /* ���ɻָ����ϣ���λ */
    if ( (NAS_UTRANCTRL_UTRAN_MODE_FDD == NAS_UTRANCTRL_GetCurrUtranMode())
      && (NAS_MML_NET_RAT_TYPE_WCDMA == enCurRat) )
    {
        NAS_MML_SoftReBoot();
    }
    else if ( NAS_MML_NET_RAT_TYPE_GSM == enCurRat )
    {
        NAS_MML_SoftReBoot();
    }
    else
    {
        NAS_MML_SoftReBoot();
    }

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_PlmnSelection_WaitLmmPlmnSearchCnfCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF);

    NAS_MMC_SetAbortFlag_PlmnSelection(VOS_TRUE);

    /* ֪ͨLMMָֹͣ������ */
    NAS_MMC_SndLmmPlmnSrchStopReq();

    /*Ǩ�Ƶ��ȴ�ֹͣ����ȷ��״̬������������ʱ��*/
    NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_PLMN_STOP_CNF_CSPSMODE1_RECAMP_LTE);

    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmPlmnCnf_PlmnSelection_WaitLmmPlmnSearchCnfCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_PLMN_SRCH_CNF_STRU                             *pstLmmSrchCnfMsg = VOS_NULL_PTR;

    pstLmmSrchCnfMsg = (LMM_MMC_PLMN_SRCH_CNF_STRU*)pstMsg;

    /* ֹͣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF);

    /* ���õ�ǰģʽ�Ƿ����ȫƵ�������Ƿ�������縲�� */
    NAS_MMC_UpdateLmmCoverageFlg_PlmnSelection(pstLmmSrchCnfMsg);

    if (MMC_LMM_PLMN_SRCH_RLT_SPEC_SUCC == pstLmmSrchCnfMsg->enRlst)
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_SUCCESS,
                VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        /*Ǩ��״̬������������ʱ�� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_SYSINFO_IND_CSPSMODE1_RECAMP_LTE);

        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_SYS_INFO, TI_NAS_MMC_WAIT_LMM_SYS_INFO_LEN);
    }
    else
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        /* ��ע������������CS/PS����״̬��δ�ϱ�����ʱ��Ҫ���� */
        NAS_MMC_SetCsServiceStatus(NAS_MMC_NO_SERVICE);
        NAS_MMC_SetPsServiceStatus(NAS_MMC_NO_SERVICE);

        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NO_SERVICE);


        /* ���µ�ǰ���뼼��������ȫƵ���� */
        NAS_MMC_SetAllBandSearch_PlmnSelection(NAS_MML_NET_RAT_TYPE_LTE, VOS_TRUE);


        /* ���ϲ�״̬��ָʾѡ��ʧ�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

        /* �˳����״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvLmmAttachCnf_PlmnSelection_WaitLmmPlmnSearchCnfCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ǰLTE������״̬��ΪDISABLE״̬���򲻴������Ϣ */
    if (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS != NAS_MML_GetLteCapabilityStatus())
    {
        /* �쳣��ӡ */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmAttachCnf_PlmnSelection_WaitLmmPlmnSearchCnfCsPsMode1ReCampLte: unexpect message received");

        return VOS_TRUE;
    }

    /* ֹͣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF);

    /* ֪ͨLMMָֹͣ������ */
    NAS_MMC_SndLmmPlmnSrchStopReq();

    /*Ǩ��״̬������������ʱ��*/
    NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_PLMN_STOP_CNF_CSPSMODE1_RECAMP_LTE);

    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvLmmDetachCnf_PlmnSelection_WaitLmmPlmnSearchCnfCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ǰLTE������״̬��ΪDISABLE״̬���򲻴������Ϣ */
    if (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS != NAS_MML_GetLteCapabilityStatus())
    {
        /* �쳣��ӡ */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmDetachCnf_PlmnSelection_WaitLmmPlmnSearchCnfCsPsMode1ReCampLte: unexpect message received");

        return VOS_TRUE;
    }

    /* ֹͣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF);

    /* ֪ͨLMMָֹͣ������ */
    NAS_MMC_SndLmmPlmnSrchStopReq();

    /*Ǩ��״̬������������ʱ��*/
    NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_PLMN_STOP_CNF_CSPSMODE1_RECAMP_LTE);

    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF_LEN);

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvTiWaitLmmPlmnSearchCnfExpired_PlmnSelection_WaitLmmPlmnSearchCnfCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitLmmPlmnSearchCnfExpired_PlmnSelection_WaitLmmPlmnSearchCnfCsPsMode1ReCampLte ENTERED");

    NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                    VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

    /* ��ע������������CS/PS����״̬��δ�ϱ�����ʱ��Ҫ���� */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_NO_SERVICE);
    NAS_MMC_SetPsServiceStatus(NAS_MMC_NO_SERVICE);

    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NO_SERVICE);

    /* ���ϲ�״̬��ָʾѡ��ʧ�� */
    NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

    /* �˳����״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvLmmPlmnStopCnf_PlmnSelection_WaitLmmPlmnStopCnfCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֹͣ��ʱ�� */
    NAS_MMC_StopTimer( TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF );

    /* �ж��Ƿ���ڴ�ϱ�־ */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* ���ϲ�״̬��ָʾѡ������� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);
    }
    else
    {
        /* ���ϲ�״̬��ָʾѡ��ʧ�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);
    }

    /* �˳����״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}

VOS_UINT32 NAS_MMC_RcvTiWaitLmmStopCnfExpired_PlmnSelection_WaitLmmPlmnStopCnfCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitLmmStopCnfExpired_PlmnSelection_WaitLmmPlmnStopCnfCsPsMode1ReCampLte: Timer Expired");

    /* �ж��Ƿ���ڴ�ϱ�־ */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* ���ϲ�״̬��ָʾѡ������� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);
    }
    else
    {
        /* ���ϲ�״̬��ָʾѡ��ʧ�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);
    }

    /* �˳����״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}

VOS_UINT32 NAS_MMC_RcvLmmPlmnSearchCnf_PlmnSelection_WaitLmmPlmnStopCnfCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_PLMN_SRCH_CNF_STRU          *pstLmmSrchCnfMsg = VOS_NULL_PTR;

    pstLmmSrchCnfMsg = (LMM_MMC_PLMN_SRCH_CNF_STRU*)pstMsg;

    /* ֹͣ��ʱ�� */
    NAS_MMC_StopTimer( TI_NAS_MMC_WAIT_LMM_PLMN_STOP_CNF );

    /* ���õ�ǰģʽ�Ƿ����ȫƵ�������Ƿ�������縲�� */
    NAS_MMC_UpdateLmmCoverageFlg_PlmnSelection(pstLmmSrchCnfMsg);

    /* �ж��Ƿ���ڴ�ϱ�־ */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* ���ϲ�״̬��ָʾѡ����ֹ */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);
    }
    else
    {
        /* ���ϲ�״̬��ָʾѡ��ʧ�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);
    }

    /* �˳����״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_PlmnSelection_WaitLSysInfoIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ����Abort��־ */
    NAS_MMC_SetAbortFlag_PlmnSelection(VOS_TRUE);

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvLSysInfoInd_PlmnSelection_WaitLSysInfoIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    /* ֹͣ�ȴ�ϵͳ��Ϣ�Ķ�ʱ��  */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_SYS_INFO);

    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE  == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* �ظ�״̬������� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_LmmSysInfo(pstMsg);



    /* ���µ�ǰ����״̬���� */
    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STOP);

    /* ���µ�ǰ�ȴ�EPS��ע���� */
    NAS_MMC_ClearAllWaitRegRsltFlag_PlmnSelection();
    NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_PS);
    
#if (FEATURE_ON == FEATURE_IMS)        
    /* ���ӵȴ�IMS�Ƿ���ý��FLG,MMA��֤�յ�PSע�����ϱ�IMS�Ƿ����ָʾ��MMC */    
    NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_IMS);
#endif    

    /* ����CS/PS��ע��Cause��AdditionalAction */
    NAS_MMC_SetCsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_BUTT);
    NAS_MMC_SetPsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_BUTT);

    NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_BUTT);
    NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_BUTT);

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_LTE);

    NAS_MMC_SndMmaCampOnInd(VOS_TRUE);

    /* ת��GMMϵͳ��Ϣ */
    NAS_MMC_SndGmmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);
    NAS_MMC_SndMmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    NAS_MMC_SndLmmLteSysInfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    /* Ǩ�Ƶ��ȴ�ע������״̬PLMN_SRCH:NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_REG_IND */
    NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_REG_IND_CSPSMODE1_RECAMP_LTE);

    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_REG_IND, TI_NAS_MMC_WAIT_EPS_REG_IND_LEN);

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvTiWaitLSysInfoExpired_PlmnSelection_WaitLSysInfoIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitLSysInfoExpired_PlmnSelection_WaitLSysInfoIndCsPsMode1ReCampLte: Timer Expired");

    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);

    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);
    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());

    /*�����ǰ��Ҫ�˳�״̬��*/
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* �ظ�״̬������� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);
    }
    else
    {
        /* ���ϲ�״̬��ָʾѡ��ʧ�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);
    }

    /* �˳����״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_PlmnSelection_WaitEpsRegRsltIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;

    /* ������������û�����Ĺػ���������ϻ�ǰû�����ӿ���ֱ���˳� */
    if ((VOS_FALSE == NAS_MML_IsRrcConnExist())
     || (NAS_MMC_ABORT_FSM_IMMEDIATELY == pstAbortMsg->enAbortFsmType)
     || (NAS_BuildEventType(WUEPS_PID_MMA, ID_MMA_MMC_POWER_OFF_REQ) == pstAbortMsg->ulEventType))
    {
        /* ֹͣ�ȴ�epsע������Ϣ�Ķ�ʱ��  */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* �ظ�״̬������� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����Abort��־ */
    NAS_MMC_SetAbortFlag_PlmnSelection(VOS_TRUE);

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvLmmMmcStatusInd_PlmnSelection_WaitEpsRegRsltIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_STATUS_IND_STRU            *pstLmmStatusIndMsg = VOS_NULL_PTR;

    pstLmmStatusIndMsg = (LMM_MMC_STATUS_IND_STRU*)pstMsg;

    /* ����LMM������״̬ */
    NAS_MML_UpdateEpsConnectionStatus(pstLmmStatusIndMsg->ulConnState);

    return VOS_TRUE;
}
VOS_UINT32  NAS_MMC_RcvLmmMmcAttachInd_PlmnSelection_WaitEpsRegRsltIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    LMM_MMC_ATTACH_IND_STRU                                *pstLmmAttachIndMsg = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

    pstLmmAttachIndMsg  = (LMM_MMC_ATTACH_IND_STRU*)pstMsg;

    /* ����ȴ�PS��־ */
    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* LMM������δ�ظ�search cnfʱ�յ��û�detach ps����󣬺���������ע���mmc�ظ�attach result
       ΪMMC_LMM_ATT_RSLT_PS_ATT_NOT_ALLOW���������������MMC_LMM_ATT_TYPE_BUTT��
       MMC��Ҫ���ݵ�ǰue operation mode�������������¸�ֵ */
    if ((MMC_LMM_ATT_TYPE_BUTT == pstLmmAttachIndMsg->ulReqType)
     && (MMC_LMM_ATT_RSLT_PS_ATT_NOT_ALLOW == pstLmmAttachIndMsg->ulAttachRslt))
    {
        pstLmmAttachIndMsg->ulReqType = NAS_MMC_ConvertLmmAttachReqType(NAS_MML_GetLteUeOperationMode());
    }

    /* ��GMM��MMת��ע������Ϣ */
    NAS_MMC_SndMmLmmAttachInd(pstLmmAttachIndMsg);
    NAS_MMC_SndGmmLmmAttachInd(pstLmmAttachIndMsg);

    /* ����attach��� */
    NAS_MMC_ProcLmmAttachInd_PlmnSelection(pstLmmAttachIndMsg);

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* ״̬���˳����ΪAbort */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֹͣ�ȴ�epsע������Ϣ�Ķ�ʱ��  */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �������ȼ��Ƚϱ��ó�CS/PS�Ĵ������ȼ�����ת��Ϊ״̬����һ����Ϊ */
    enAdditionalAction   = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_GetCsRegAdditionalAction_PlmnSelection(),
                                                          NAS_MMC_GetPsRegAdditionalAction_PlmnSelection());

    if (NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT == enAdditionalAction)
    {
        return VOS_TRUE;
    }

    if (NAS_MMC_WAIT_REG_RESULT_IND_NULL != NAS_MMC_GetWaitRegRsltFlag_PlmnSelection())
    {
        return VOS_TRUE;
    }

    /* ͣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

    /* ���Ӳ���Ϊפ����ǰ���� */
    if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enAdditionalAction))
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_SUCC);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /*  ��������������ӣ�Ǩ�Ƶ��ȴ������ͷ�״̬  */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_REL_IND_CSPSMODE1_RECAMP_LTE);

        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND, TI_NAS_MMC_WAIT_EPS_CONN_REL_IND_LEN);

        return VOS_TRUE;
    }

    /* ����פ��LTEʱ������״̬�ϱ� */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_CS));
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_PS));

    /* �ظ�״̬���˳�ʧ�� */
    NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

    if (VOS_TRUE == NAS_MML_GetSimPsRegStatus())
    {
        /* ���Disable Lȫ�ֱ�����־ */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }

    /* ֱ���˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvLmmMmcTauResultInd_PlmnSelection_WaitEpsRegRsltIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_TAU_RESULT_IND_STRU                            *pstLmmTauIndMsg = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

    pstLmmTauIndMsg  = (LMM_MMC_TAU_RESULT_IND_STRU*)pstMsg;

    /* ����ȴ�PS��־ */
    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* ��GMM��MMת��TAU�����Ϣ */
    NAS_MMC_SndMmLmmTauResultInd(pstLmmTauIndMsg);
    NAS_MMC_SndGmmLmmTauResultInd(pstLmmTauIndMsg);

    /* ����TAU��� */
    NAS_MMC_ProcLmmTauResultInd_PlmnSelection(pstLmmTauIndMsg);

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* ״̬���˳����ΪAbort */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֹͣ�ȴ�epsע������Ϣ�Ķ�ʱ��  */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �������ȼ��Ƚϱ��ó�CS/PS�Ĵ������ȼ�����ת��Ϊ״̬����һ����Ϊ */
    enAdditionalAction   = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_GetCsRegAdditionalAction_PlmnSelection(),
                                                          NAS_MMC_GetPsRegAdditionalAction_PlmnSelection());

    if (NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT == enAdditionalAction)
    {
        return VOS_TRUE;
    }

    if (NAS_MMC_WAIT_REG_RESULT_IND_NULL != NAS_MMC_GetWaitRegRsltFlag_PlmnSelection())
    {
        return VOS_TRUE;
    }

    /* ͣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

    /* ���Ӳ���Ϊפ����ǰ���� */
    if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enAdditionalAction))
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_SUCC);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /*  ��������������ӣ�Ǩ�Ƶ��ȴ������ͷ�״̬  */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_REL_IND_CSPSMODE1_RECAMP_LTE);

        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND, TI_NAS_MMC_WAIT_EPS_CONN_REL_IND_LEN);

        return VOS_TRUE;
    }

    /* ����פ��LTEʱ������״̬�ϱ� */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_CS));
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_PS));

    /* �ظ�״̬���˳�ʧ�� */
    NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

    if (VOS_TRUE == NAS_MML_GetSimPsRegStatus())
    {
        /* ���Disable Lȫ�ֱ�����־ */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }

    /* ֱ���˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;

}
VOS_UINT32  NAS_MMC_RcvLmmMmcDetachInd_PlmnSelection_WaitEpsRegRsltIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_DETACH_IND_STRU                                *pstDetachMsg = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo;
    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;

    pstDetachMsg        = (LMM_MMC_DETACH_IND_STRU*)pstMsg;

    /* ����DETACH��Ϣ�������һ������ */
    NAS_MMC_ProcLmmDetachInd(pstDetachMsg, &enCsAdditionalAction, &enPsAdditionalAction);

    /*����EPS����һ���Ķ�������*/
    if (NAS_MMC_ADDITIONAL_ACTION_BUTT != enPsAdditionalAction)
    {
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(enPsAdditionalAction);
    }

    /*����CS����һ���Ķ�������*/
    if (NAS_MMC_ADDITIONAL_ACTION_BUTT != enCsAdditionalAction)
    {
        NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(enCsAdditionalAction);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn_PlmnSelection())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndLmmEquPlmnReq(&stSndEquPlmnInfo);

        NAS_MMC_SndOmEquPlmn();
    }

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* ״̬���˳����ΪAbort */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֹͣ�ȴ�epsע������Ϣ�Ķ�ʱ��  */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �������ȼ��Ƚϱ��ó�CS/PS�Ĵ������ȼ�����ת��Ϊ״̬����һ����Ϊ */
    enAdditionalAction   = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_GetCsRegAdditionalAction_PlmnSelection(),
                                                              NAS_MMC_GetPsRegAdditionalAction_PlmnSelection());

    /* ѡ������AdditionalActionΪ��Ҫ�����ȴ�ע���� */
    if (NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT == enAdditionalAction)
    {
        /*���ؼ����ȴ�ע����*/
        return VOS_TRUE;
    }

    /* ͣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

    /* ���Ӳ���Ϊפ����ǰ���� */
    if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enAdditionalAction))
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_SUCC);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /*  ��������������ӣ�Ǩ�Ƶ��ȴ������ͷ�״̬  */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_REL_IND_CSPSMODE1_RECAMP_LTE);

        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND, TI_NAS_MMC_WAIT_EPS_CONN_REL_IND_LEN);
    }
    else
    {
        /* �ظ�״̬���˳�ʧ�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }

    return VOS_TRUE;

}


VOS_UINT32 NAS_MMC_RcvLmmMmcServiceRsltInd_PlmnSelection_WaitEpsRegRsltIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_SERVICE_RESULT_IND_STRU                        *pstSerRsltMsg = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

    pstSerRsltMsg     = (LMM_MMC_SERVICE_RESULT_IND_STRU*)pstMsg;

    /* ��LMM�ķ��������д���������ɺ󷵻���Ҫ���е�ѡ������ */
    enAdditionalAction = NAS_MMC_ProcLmmServiceRsltInd(pstSerRsltMsg);

    /* ѡ������AdditionalAction��Ч������EPS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(enAdditionalAction);
    }

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* ״̬���˳����ΪAbort */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֹͣ�ȴ�epsע������Ϣ�Ķ�ʱ��  */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* �������ȼ��Ƚϱ��ó�CS/PS�Ĵ������ȼ�����ת��Ϊ״̬����һ����Ϊ */
    enAdditionalAction   = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_GetCsRegAdditionalAction_PlmnSelection(),
                                                              NAS_MMC_GetPsRegAdditionalAction_PlmnSelection());

    /* ѡ������AdditionalActionΪ��Ҫ�����ȴ�ע���� */
    if (NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT == enAdditionalAction)
    {
        /*���ؼ����ȴ�ע����*/
        return VOS_TRUE;
    }

    /* ͣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

    /* ���Ӳ���Ϊפ����ǰ���� */
    if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enAdditionalAction))
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_SUCC);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /*  ��������������ӣ�Ǩ�Ƶ��ȴ������ͷ�״̬  */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_REL_IND_CSPSMODE1_RECAMP_LTE);

        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND, TI_NAS_MMC_WAIT_EPS_CONN_REL_IND_LEN);

        return VOS_TRUE;
    }

    /* �ظ�״̬���˳�ʧ�� */
    NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

    /* ֱ���˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmSuspendInd_PlmnSelection_WaitEpsRegRsltIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_SUSPEND_IND_STRU           *pstLSuspendMsg = VOS_NULL_PTR;
    MMC_SUSPEND_CAUSE_ENUM_UINT8        enSuspendCause;

    pstLSuspendMsg = (LMM_MMC_SUSPEND_IND_STRU*)pstMsg;

    /* Lģ�£�����ԭ������GU�²�ͬ��ת��һ�¹���ԭ���� */
    NAS_MMC_ConvertLmmSysChngTypeToMmcType(pstLSuspendMsg->ulSysChngType, &enSuspendCause);

    /* ���ΪOOS���߲�������ȷ, �����������ϵͳ��ѡ */
    if (MMC_SUSPEND_CAUSE_BUTT <= enSuspendCause)
    {
        NAS_MMC_SndLmmSuspendRsp(MMC_LMM_FAIL);
    }
    else
    {
        /* ����SUSPEND��Ϣ�еĲ�ͬ��������Ӧ��״̬�� */
        NAS_MMC_LoadInterSysFsm_PlmnSelection(enSuspendCause);
    }

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvMmcMmcSuspendRslt_PlmnSelection_WaitEpsRegRsltIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRatType;

    enRatType = NAS_MML_GetCurrNetRatType();

    /* �����ǰ��ϵͳ�ı䵽GU,ֱ���˳�״̬�� */
    if ((NAS_MML_NET_RAT_TYPE_WCDMA == enRatType)
     || (NAS_MML_NET_RAT_TYPE_GSM == enRatType))
    {
        /* ״̬���˳����Ϊ�ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_SUCC);

        /* ֹͣ�ȴ�epsע������Ϣ�Ķ�ʱ��  */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }


#if (FEATURE_ON == FEATURE_CL_INTERWORK)
    /* ��ѡ��HRPD��,�˳�ѡ��״̬�� */
    if (NAS_MML_NET_RAT_TYPE_BUTT == enRatType)
    {
        /* ֹͣ�ȴ�epsע������Ϣ�Ķ�ʱ��  */
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

        /* ���ϲ�״̬�����͸����ȼ�����ʧ�ܣ���Ҫ���� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_INTER_SYS_HRPD);

        /* �˳�״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }
#endif

    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvTiWaitEpsRegRsltIndExpired_PlmnSelection_WaitEpsRegRsltIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitEpsRegRsltIndExpired_PlmnSelection_WaitEpsRegRsltIndCsPsMode1ReCampLte: Timer Expired");

    /* ����NAS_MMC_SetPsServiceStatus����EPS����״̬NAS_MMC_LIMITED_SERVICE */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* �ظ�״̬������� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);
    }
    else
    {
        /* ����״̬���˳������Ϣ�����Ϊʧ�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);
    }

    /* ֱ���˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvLmmAreaLostInd_PlmnSelection_WaitEpsRegRsltIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֹͣ�ȴ�Epsע������Ϣ�Ķ�ʱ��  */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

    /* ���פ��״̬ */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
    NAS_MML_InitCampRai(NAS_MML_GetCurrCampPlmnInfo());
    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);

    /* ��ǰ�źŸ��� */
    NAS_MML_InitRssiValue(NAS_MML_GetCampCellInfo());

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_NO_SERVICE);

    NAS_MMC_SetPsServiceStatus(NAS_MMC_NO_SERVICE);

    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NO_SERVICE);

    /* �����ǰ��Ҫ�˳�״̬�� */
    if (VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection())
    {
        /* �ظ�״̬��ѡ������� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);
    }
    else
    {
        /* ����״̬���˳������Ϣ�����Ϊʧ�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);
    }

    /* ֱ���˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLSysInfoInd_PlmnSelection_WaitEpsRegRsltIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                             stOldCampInfo;

    /* �����ϵ�פ��������Ϣ */
    pstCurCampInfo  = NAS_MML_GetCurrCampPlmnInfo();
    PS_MEM_CPY(&stOldCampInfo, pstCurCampInfo, sizeof(NAS_MML_CAMP_PLMN_INFO_STRU));

    /* ����ϵͳ��Ϣ������MML��ȫ������ */
    NAS_MMC_UpdateNetworkInfo_LmmSysInfo(pstMsg);



    /* ״̬����Ϊ��ǰ�Ѿ�פ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

    /* CBS�ϱ�*/
    NAS_MMC_SndCbaMsgNetModInd(NAS_MML_NET_RAT_TYPE_LTE);

    NAS_MMC_SndMmaCampOnInd(VOS_TRUE);

    /* ת��MM/GMMϵͳ��Ϣ */
    NAS_MMC_SndGmmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);
    NAS_MMC_SndMmLteSysinfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    NAS_MMC_SndLmmLteSysInfoInd((LMM_MMC_SYS_INFO_IND_STRU*)pstMsg);

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvTafPlmnSpecialReq_PlmnSelection_WaitEpsRegRsltIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMA_MMC_PLMN_SPECIAL_REQ_STRU                          *pstUserSelReqMsg  = VOS_NULL_PTR;

    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSrchList   = VOS_NULL_PTR;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enPsRegCause;

    pstUserSelReqMsg    = (MMA_MMC_PLMN_SPECIAL_REQ_STRU*)pstMsg;

    /* �����û�ָ��������Ϣ�����µ�ǰ����������Ϊ�ֶ�ģʽ */
    NAS_MMC_UpdateUserSpecPlmnSearchInfo_PlmnSelection((NAS_MML_PLMN_ID_STRU*)(&pstUserSelReqMsg->stPlmnId),
                                                        pstUserSelReqMsg->enAccessMode);

    /* ��ȡѡ���б� */
    pstPlmnSrchList = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    /* �����û�ָ�������������³�ʼ��ѡ���б� */
    NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_USER_SPEC_PLMN_SEARCH,
                                  VOS_NULL_PTR,
                                  pstPlmnSrchList);

    /* ���û�ָ�����뼼��������(Ҳ�ǵ�ǰ����)����Ϊ������ע������ظ�ע�� */
    NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(NAS_MMC_GetUserSpecPlmnId(),
                                                   NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,
                                                   pstPlmnSrchList);

    enCsRegCause = NAS_MMC_GetCsRegCause_PlmnSelection();
    enPsRegCause = NAS_MMC_GetPsRegCause_PlmnSelection();

    if (VOS_TRUE == NAS_MMC_ProcUserSearchRegRslt_PlmnSelection(enCsRegCause, enPsRegCause))
    {
        return VOS_TRUE;
    }

    /* ���������Ϣ */
    NAS_MMC_SaveCurEntryMsg(ulEventType, pstMsg);

    /* ״̬Ǩ�Ƶ�NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_REG_IND */
    NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_REG_IND);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvTafSysCfgReq_PlmnSelection_WaitEpsRegRsltIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֱ�ӵ���syscfg���״̬�� */
    NAS_MMC_FSM_InitFsmL2(NAS_MMC_FSM_SYSCFG);

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvMmcSyscfgRsltCnf_PlmnSelection_WaitEpsRegRsltIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ״̬���в���Ҫ��SYSCFG״̬��������д���ֱ�ӷ��� */
    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvMmaImsVoiceCapInd_PlmnSelection_WaitEpsRegRsltIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMA_MMC_IMS_VOICE_CAP_IND_STRU                         *pstImsVoiceInd = VOS_NULL_PTR;
    VOS_UINT8                                               ucWaitRegRsltFlag;    
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;
    
    ucWaitRegRsltFlag   = NAS_MMC_GetWaitRegRsltFlag_PlmnSelection();
    pstImsVoiceInd      = (MMA_MMC_IMS_VOICE_CAP_IND_STRU *)pstMsg;
    
    /* ����ڵȴ�PS��ע����������ȴ�PSע���� */
    if (NAS_MMC_WAIT_REG_RESULT_IND_PS == (ucWaitRegRsltFlag & NAS_MMC_WAIT_REG_RESULT_IND_PS))
    {
        return VOS_TRUE;
    }

    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_IMS);

    /* IMS������ʱ�ж��Ƿ���Ҫdisable LTE */
    if (VOS_FALSE  == pstImsVoiceInd->ucAvail) 
    {
        if (VOS_TRUE == NAS_MMC_IsNeedDisableLte_ImsVoiceNotAvail())
        {
            NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION);

            NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS);

            NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_LTE_VOICE_NOT_AVAILABLE);
        }
    }
  
    /* ͣ��ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);

    /* �������ȼ��Ƚϱ��ó�CS/PS�Ĵ������ȼ�����ת��Ϊ״̬����һ����Ϊ */
    enAdditionalAction   = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_GetCsRegAdditionalAction_PlmnSelection(),
                                                          NAS_MMC_GetPsRegAdditionalAction_PlmnSelection());

    if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enAdditionalAction))
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_SUCC);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /*  ��������������ӣ�Ǩ�Ƶ��ȴ������ͷ�״̬  */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_REL_IND_CSPSMODE1_RECAMP_LTE);

        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND, TI_NAS_MMC_WAIT_EPS_CONN_REL_IND_LEN);

        return VOS_TRUE;
    }

    /* ����פ��LTEʱ������״̬�ϱ� */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_CS));
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_PS));

    /* �ظ�״̬���˳�ʧ�� */
    NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

    if (VOS_TRUE == NAS_MML_GetSimPsRegStatus())
    {
        /* ���Disable Lȫ�ֱ�����־ */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }

    /* ֱ���˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}
VOS_UINT32  NAS_MMC_RcvMmcAbortFsmMsg_PlmnSelection_WaitEpsRelIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_ABORT_FSM_STRU             *pstAbortMsg = VOS_NULL_PTR;

    pstAbortMsg = (NAS_MMC_ABORT_FSM_STRU*)pstMsg;

    /* ������������û�����Ĺػ�����ֱ���˳��������ͷŽ��״̬�� */
    if (NAS_BuildEventType(WUEPS_PID_MMA, ID_MMA_MMC_POWER_OFF_REQ) == pstAbortMsg->ulEventType)
    {
        NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND);

        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����Abort��־, �������ͷź����quit */
    NAS_MMC_SetAbortFlag_PlmnSelection(VOS_TRUE);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmMmcStatusInd_PlmnSelection_WaitEpsRelIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_STATUS_IND_STRU            *pstLmmStatusIndMsg = VOS_NULL_PTR;

    pstLmmStatusIndMsg = (LMM_MMC_STATUS_IND_STRU*)pstMsg;

    /* ����LMM������״̬ */
    NAS_MML_UpdateEpsConnectionStatus(pstLmmStatusIndMsg->ulConnState);

    /*  ��������������ӣ�������ȴ�  */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND);

    /* �Ƿ���Ҫ Abort״̬�� */
    if ( VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection() )
    {
        /* ���ϲ�״̬��ָʾѡ��ABORT */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����פ��LTEʱ������״̬�ϱ� */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_CS));
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_PS));

    /* �ظ�״̬���˳�ʧ�� */
    NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

    if (VOS_TRUE == NAS_MML_GetSimPsRegStatus())
    {
        /* ���Disable Lȫ�ֱ�����־ */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }

    /* ֱ���˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvTiWaitEpsRelIndExpired_PlmnSelection_WaitEpsRelIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitEpsRelIndExpired_PlmnSelection_WaitEpsRelIndCsPsMode1ReCampLte ENTERED");

    if ( VOS_TRUE == NAS_MMC_GetAbortFlag_PlmnSelection() )
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_ABORTED);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }

    /* ����פ��LTEʱ������״̬�ϱ� */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_CS));
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_PS));

    /* �ظ�״̬���˳�ʧ�� */
    NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

    if (VOS_TRUE == NAS_MML_GetSimPsRegStatus())
    {
        /* ���Disable Lȫ�ֱ�����־ */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }

    /* ֱ���˳���ǰ״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvTafPlmnSpecialReq_PlmnSelection_WaitEpsRelIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMA_MMC_PLMN_SPECIAL_REQ_STRU                          *pstUserSelReqMsg  = VOS_NULL_PTR;

    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSrchList   = VOS_NULL_PTR;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enPsRegCause;

    pstUserSelReqMsg    = (MMA_MMC_PLMN_SPECIAL_REQ_STRU*)pstMsg;

    /* �����û�ָ��������Ϣ�����µ�ǰ����������Ϊ�ֶ�ģʽ */
    NAS_MMC_UpdateUserSpecPlmnSearchInfo_PlmnSelection((NAS_MML_PLMN_ID_STRU*)(&pstUserSelReqMsg->stPlmnId),
                                                        pstUserSelReqMsg->enAccessMode);

    /* ��ȡѡ���б� */
    pstPlmnSrchList = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    /* �����û�ָ�������������³�ʼ��ѡ���б� */
    NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_USER_SPEC_PLMN_SEARCH,
                                  VOS_NULL_PTR,
                                  pstPlmnSrchList);

    /* ���û�ָ�����뼼��������(Ҳ�ǵ�ǰ����)����Ϊ������ע������ظ�ע�� */
    NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(NAS_MMC_GetUserSpecPlmnId(),
                                                   NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,
                                                   pstPlmnSrchList);

    enCsRegCause = NAS_MMC_GetCsRegCause_PlmnSelection();
    enPsRegCause = NAS_MMC_GetPsRegCause_PlmnSelection();

    if (VOS_TRUE == NAS_MMC_ProcUserSearchRegRslt_PlmnSelection(enCsRegCause, enPsRegCause))
    {
        return VOS_TRUE;
    }

    /* ���������Ϣ */
    NAS_MMC_SaveCurEntryMsg(ulEventType, pstMsg);

    /* ״̬Ǩ�Ƶ�NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_REG_IND */
    NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_CONN_REL_IND);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvTafSysCfgReq_PlmnSelection_WaitEpsRelIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֱ�ӵ���syscfg���״̬�� */
    NAS_MMC_FSM_InitFsmL2(NAS_MMC_FSM_SYSCFG);

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvMmcSyscfgRsltCnf_PlmnSelection_WaitEpsRelIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ״̬���в���Ҫ��SYSCFG״̬��������д���ֱ�ӷ��� */
    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvLmmMmcDetachInd_PlmnSelection_WaitEpsRelIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_DETACH_IND_STRU                                *pstDetachMsg = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;

    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo;
    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;

    pstDetachMsg        = (LMM_MMC_DETACH_IND_STRU*)pstMsg;

    /* ����DETACH��Ϣ��������һ������ */
    NAS_MMC_ProcLmmDetachInd(pstDetachMsg, &enCsAdditionalAction, &enPsAdditionalAction);

    /*����EPS����һ���Ķ�������*/
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enPsAdditionalAction)
    {
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(enPsAdditionalAction);
    }

    /*����CS����һ���Ķ�������*/
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enCsAdditionalAction)
    {
        NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(enCsAdditionalAction);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn_PlmnSelection())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndLmmEquPlmnReq(&stSndEquPlmnInfo);

        NAS_MMC_SndOmEquPlmn();
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmMmcDetachCnf_PlmnSelection_WaitEpsRelIndCsPsMode1ReCampLte(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_DETACH_CNF_STRU            *pstDetachCnfMsg = VOS_NULL_PTR;

    pstDetachCnfMsg        = (LMM_MMC_DETACH_CNF_STRU*)pstMsg;

    /* ���detach������Ǽ�Ȩ���ܣ�����Ҫ��״̬����Ԥ�����оͿ��Դ��� */
    if ( MMC_LMM_DETACH_RSLT_AUTH_REJ != pstDetachCnfMsg->ulDetachRslt)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmMmcDetachCnf_PlmnSelection_WaitEpsConnRelInd: Unexpected detach result!");

        return VOS_TRUE;
    }

    /*����EPS����һ���Ķ�������*/
    NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL);

    /*����CS����һ���Ķ�������*/
    NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL);

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvLSysInfoInd_PlmnSelection_WaitMmaRegReq(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnId;
    LMM_MMC_SYS_INFO_IND_STRU                              *pstLmmSysInfoMsg    = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo    = VOS_NULL_PTR;

    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

    /* ����ϵͳ��Ϣ��PLMNID������ѡ���б��и�����״̬Ϊ���� */
    pstLmmSysInfoMsg  = (LMM_MMC_SYS_INFO_IND_STRU*)pstMsg;
    NAS_MMC_ConvertLmmPlmnToGUNasFormat(&(pstLmmSysInfoMsg->stLteSysInfo.stSpecPlmnIdList.astSuitPlmnList[0]), &(stPlmnId.stPlmnId));

    stPlmnId.enRat           = NAS_MML_NET_RAT_TYPE_LTE;
    pstPlmnSelectionListInfo = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();
    NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&stPlmnId, NAS_MMC_NET_STATUS_SEARCHED_EXIST, pstPlmnSelectionListInfo);

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvLmmSuspendInd_PlmnSelection_WaitMmaRegReq(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��״̬�����������ϵͳ��ѡ���ظ�LMM����ʧ�� */
    NAS_MMC_SndLmmSuspendRsp(MMC_LMM_FAIL);

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvLmmAreaLostInd_PlmnSelection_WaitMmaRegReq(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ���ý����פ��״̬Ϊδפ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);

    return VOS_TRUE;
}



#endif  /* (FEATURE_ON == FEATURE_LTE) */


VOS_UINT32 NAS_MMC_GetNextSearchPlmn_PlmnSelection(
    NAS_MML_PLMN_WITH_RAT_STRU         *pstDestPlmn
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList       = VOS_NULL_PTR;
    VOS_UINT32                                              i;
    VOS_UINT32                                              ulUserSpecPlmnSearch;
    NAS_MMC_FSM_ID_ENUM_UINT32                              enCurrentFsmId;
    VOS_UINT32                                              ulCurrentEventType;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;

    NAS_MML_SIM_TYPE_ENUM_UINT8                             enSimType;
#endif


    enCurrentFsmId       = NAS_MMC_GetCurrFsmId();
    if (enCurrentFsmId >= NAS_FSM_BUTT )
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    ulCurrentEventType   = NAS_MMC_GetCurrFsmEventType();

    /* ��ȡ�Ƿ��û�ָ������ */
    ulUserSpecPlmnSearch = NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType);

    /* ��ȡ��ǰ���뼼�����ȼ���Ϣ */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();

    pstPlmnSelectionList = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    /* ��ȡѡ���б���������ǰ���״̬Ϊδ�����������������ڵ����� */
    if (VOS_TRUE == NAS_MMC_GetNextSearchPlmn(pstDestPlmn, pstPrioRatList,
                                              ulUserSpecPlmnSearch, pstPlmnSelectionList))
    {
        NAS_MMC_SetCurrSearchingPlmn_PlmnSelection(pstDestPlmn);

        return VOS_TRUE;
    }

    /* �����ǰΪ�Զ�ѡ�� */
    if (VOS_TRUE == NAS_MMC_IsNeedSearchAvailPlmn_PlmnSelection())
    {
        /* ��SYSCFG���õĽ��뼼�����ȼ��ж��Ƿ���ĳ�����뼼��δ����ȫƵ������
           ����У�����VOS_TRUE,����ý��뼼����ȫF��Ч����,ָʾ���������,
           ���������ʧ��ʱ�����һ��ȫƵ���� */
        for (i = 0; i < pstPrioRatList->ucRatNum; i++)
        {
            /* �����жϣ����RAT����ֹ�Ļ�������Ҫ��RAT */
            if (VOS_TRUE == NAS_MML_IsRatInForbiddenList(pstPrioRatList->aucRatPrio[i]))
            {
                continue;
            }

#if (FEATURE_ON == FEATURE_LTE)
            enSimType = NAS_MML_GetSimType();

            /* ��ȡLTE����״̬ */
            enLCapabilityStatus = NAS_MML_GetLteCapabilityStatus();

            /* ������������,����Ҫ����LTE������:
                1)��ǰ����Ľ��뼼��ΪL�ҵ�ǰPS����Ч
                2)��ǰ����Ľ��뼼��ΪL�ҵ�ǰDISABLE LTE
                3)������ΪSIM��:��Ӧ�ĳ���Ϊ:SIM��ʱ���ó�LTE only����ȥ��Lģ������ʱ����ʱ����Lģ
            */
            if (((VOS_FALSE                             == NAS_MML_GetSimPsRegStatus())
              || ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLCapabilityStatus)
               || (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == enLCapabilityStatus))
              ||(NAS_MML_SIM_TYPE_SIM == enSimType))
             && (NAS_MML_NET_RAT_TYPE_LTE == pstPrioRatList->aucRatPrio[i]))
            {
                continue;
            }
#endif
            /* ���USIM��GSM��ֹ���룬��ѡ��GSM���� */
            if ( (VOS_TRUE == NAS_MML_GetGsmForbidFlg())
              && (NAS_MML_NET_RAT_TYPE_GSM == pstPrioRatList->aucRatPrio[i]) )
            {
                continue;
            }

            /* ���β�������֧�ֵĽ��뼼�� */
            if (VOS_FALSE == NAS_MMC_GetAllBandSearch_PlmnSelection(pstPrioRatList->aucRatPrio[i]))
            {
                pstDestPlmn->stPlmnId.ulMcc = NAS_MML_INVALID_MCC;
                pstDestPlmn->stPlmnId.ulMnc = NAS_MML_INVALID_MNC;
                pstDestPlmn->enRat          = pstPrioRatList->aucRatPrio[i];

                NAS_MMC_SetCurrSearchingPlmn_PlmnSelection(pstDestPlmn);

                return VOS_TRUE;
            }
        }
    }

    return VOS_FALSE;
}


VOS_VOID NAS_MMC_GetNextSearchPlmnFail_PlmnSelection(VOS_VOID)
{
    NAS_MMC_FSM_ID_ENUM_UINT32          enCurrentFsmId;
    VOS_UINT32                          ulCurrentEventType;

    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();

    /* ������û�ָ�����������ϲ�ظ�����ʧ�� */
    if (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType))
    {
        NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_FAIL);

        
        /*���������ϢΪ��ͨ���������󣬺������µ��û�����ʱ�����Դ�ϸ�״̬��*/
         NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection();

#if   (FEATURE_ON == FEATURE_LTE)
        /* ��LMM�����û���������֪ͨ */
        if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
        {

            NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
        }
#endif
    }

#if   (FEATURE_ON == FEATURE_LTE)
    /* �ж��Ƿ���Ҫ����פ��LTE���� */
    if (VOS_TRUE == NAS_MMC_IsReCampLtePlmnValid_PlmnSelection())
    {
        NAS_MMC_ReCampLte_PlmnSelection();
    }
    else
#endif
    {
        /* ���ϲ�״̬��ָʾѡ��ʧ�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

        /* �˳����״̬�� */
        NAS_MMC_FSM_QuitFsmL2();
    }

    return;
}


VOS_VOID NAS_MMC_ProcPlmnSearchNoRfFail_PlmnSelection(VOS_VOID)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList     = VOS_NULL_PTR;
    NAS_MMC_RAT_SEARCH_INFO_STRU                           *pstSearchRatInfo = VOS_NULL_PTR;
    NAS_MMC_FSM_ID_ENUM_UINT32                              enCurrentFsmId;
    VOS_UINT32                                              ulCurrentEventType;

    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
    pstSearchRatInfo   = NAS_MMC_GetRatCoverage_PlmnSelection();
    pstPlmnSelectionList = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();
    
    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    
    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();

    /* ����NO RF���ڱ����Ϣ */
    NAS_MMC_SetCurrRatPlmnSrchNoRfFlg(NAS_MML_GetCurrNetRatType(), VOS_TRUE);

    /* NO RFʧ�ܣ��豣�浱ǰ��ѡ��״̬���б���Ϣ */
    NAS_MMC_SetPlmnSelectionListType(NAS_MMC_BACKUP_PLMN_SELECTION_LIST);
    PS_MEM_CPY(NAS_MMC_GetHighPrioPlmnList(), pstPlmnSelectionList, sizeof(NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU));

    /* ������û�ָ�����������ϲ�ظ�����ʧ�� */
    if (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType))
    {
        NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_FAIL);

        
        /*���������ϢΪ��ͨ���������󣬺������µ��û�����ʱ�����Դ�ϸ�״̬��*/
         NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection();

#if   (FEATURE_ON == FEATURE_LTE)
        /* ��LMM�����û���������֪ͨ */
        if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
        {

            NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
        }
#endif
    }

    /* ���ϲ�״̬��ָʾѡ��ʧ��,���ն�������:����AVAILABLE��ʱ�� */
    NAS_MMC_SndPlmnSelectionRslt(NAS_MMC_PLMN_SELECTION_FAIL, VOS_FALSE, 
                                 pstSearchRatInfo, 0);

    /* �˳����״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return;
}

#if (FEATURE_ON == FEATURE_LTE)
VOS_VOID NAS_MMC_SndLmmSpecPlmnSearchReq_PlmnSelection(
    NAS_MML_PLMN_LIST_WITH_RAT_STRU    *pstDestPlmnList,
    VOS_UINT32                          ulUserSpecFlag
)
{
    NAS_MMC_NCELL_SEARCH_INFO_STRU                         *pstNcellSearchInfo = VOS_NULL_PTR;
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstDestPlmn = VOS_NULL_PTR;

    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;

    PS_MEM_SET(&stSndEquPlmnInfo, 0, sizeof(stSndEquPlmnInfo));

    pstNcellSearchInfo = NAS_MMC_GetNcellSearchInfo();
    enPlmnSearchScene  = NAS_MMC_GetPlmnSearchScene_PlmnSelection();

    pstDestPlmn = &(pstDestPlmnList->astPlmnId[0]);

    /* ncell�������� */
    if ((VOS_TRUE == NAS_MMC_IsInterPlmnSearch_PlmnSelection())
     && ((NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_NCELL_SEARCH == enPlmnSearchScene)
      || (NAS_MMC_PLMN_SEARCH_SCENE_REG_NCELL == enPlmnSearchScene)))
    {
        if (NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_NCELL_SEARCH == enPlmnSearchScene)
        {
            /* ����ncell������Чplmn����Ϊmodem1�ĵ�Чplmn+modem0��ЧEHPLMN+modem0��RPLMN */
            NAS_MMC_BuildNcellPlmnSearchEquPlmnInfo_PlmnSelection(&stSndEquPlmnInfo, pstDestPlmn->enRat);
        }
        else
        {
            stSndEquPlmnInfo.ucEquPlmnNum = NAS_MML_MIN(NAS_MML_MAX_EQUPLMN_NUM, pstNcellSearchInfo->stOtherModemEplmnInfo.ucEquPlmnNum);
            PS_MEM_CPY(stSndEquPlmnInfo.astEquPlmnAddr, pstNcellSearchInfo->stOtherModemEplmnInfo.astEquPlmnAddr,
                       (stSndEquPlmnInfo.ucEquPlmnNum*sizeof(NAS_MML_PLMN_ID_STRU)));
        }

        /* ɾ����ֹ���� */
        NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndLmmNcellSpecPlmnSearchReq(&pstNcellSearchInfo->stLteNcellInfo,
                                     &stSndEquPlmnInfo);

    }
    else
    {
        NAS_MMC_SndLmmSpecPlmnSearchReq(pstDestPlmnList, ulUserSpecFlag);
    }

}

#endif
VOS_VOID NAS_MMC_BuildNcellPlmnSearchEquPlmnInfo_PlmnSelection(
    NAS_MML_EQUPLMN_INFO_STRU          *pstOutEplmnInfo,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRatType
)
{
    MMA_MMC_PLMN_ID_STRU                astEquPlmnAddr[MMA_MMC_MAX_EQUPLMN_NUM];
    NAS_MMC_NCELL_SEARCH_INFO_STRU     *pstNcellSearchInfo = VOS_NULL_PTR;
    VOS_UINT32                          ulIndex;
    VOS_UINT8                           ucSndEplmnNum;
    NAS_MML_PLMN_ID_STRU                stRPlmnId;
    VOS_UINT32                          ulRplmnExistFlg;
    NAS_MML_SIM_EHPLMN_INFO_STRU       *pstSimEHPlmnInfo = VOS_NULL_PTR;

    PS_MEM_SET(astEquPlmnAddr, 0, sizeof(astEquPlmnAddr));
    PS_MEM_SET(&stRPlmnId, 0, sizeof(stRPlmnId));
    ucSndEplmnNum      = 0;
    ulIndex            = 0;
    pstSimEHPlmnInfo   = NAS_MML_GetSimEhplmnList();
    ulRplmnExistFlg    = VOS_FALSE;
    pstNcellSearchInfo = NAS_MMC_GetNcellSearchInfo();

    if (NAS_MML_MAX_EQUPLMN_NUM < pstNcellSearchInfo->stOtherModemEplmnInfo.ucEquPlmnNum)
    {
        pstNcellSearchInfo->stOtherModemEplmnInfo.ucEquPlmnNum  = NAS_MML_MAX_EQUPLMN_NUM;
        
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_BuildNcellPlmnSearchEquPlmnInfo_PlmnSelection:other modem eplmn num incorrect!");
        
    }
    
    /* ��Чplmn��Ϣ�м���modem1�ĵ�Чplmn��Ϣ */
    for (ulIndex = 0; ulIndex < pstNcellSearchInfo->stOtherModemEplmnInfo.ucEquPlmnNum; ulIndex++)
    {
        if (ucSndEplmnNum >= MMA_MMC_MAX_EQUPLMN_NUM)
        {
            pstOutEplmnInfo->ucEquPlmnNum = ucSndEplmnNum;
            PS_MEM_CPY(pstOutEplmnInfo->astEquPlmnAddr, astEquPlmnAddr, sizeof(pstOutEplmnInfo->astEquPlmnAddr));
            return;
        }

        astEquPlmnAddr[ucSndEplmnNum].ulMcc = pstNcellSearchInfo->stOtherModemEplmnInfo.astEquPlmnAddr[ulIndex].ulMcc;
        astEquPlmnAddr[ucSndEplmnNum].ulMnc = pstNcellSearchInfo->stOtherModemEplmnInfo.astEquPlmnAddr[ulIndex].ulMnc;
        ucSndEplmnNum++;
    }

    /* ��Чplmn��Ϣ�м���modem0��RPLMN��Ϣ */
#if (FEATURE_ON == FEATURE_LTE)
    if (NAS_MML_NET_RAT_TYPE_LTE == enRatType)
    {
        ulRplmnExistFlg = NAS_MML_GetLteRPlmn(&stRPlmnId);
    }
    else
#endif
    {
        ulRplmnExistFlg = NAS_MML_GetGURPlmn(&stRPlmnId);
    }

    if ((VOS_TRUE == ulRplmnExistFlg)
     && (ucSndEplmnNum < MMA_MMC_MAX_EQUPLMN_NUM))
    {
        astEquPlmnAddr[ucSndEplmnNum].ulMcc = stRPlmnId.ulMcc;
        astEquPlmnAddr[ucSndEplmnNum].ulMnc = stRPlmnId.ulMnc;
        ucSndEplmnNum++;
    }

    if (NAS_MML_MAX_EHPLMN_NUM < pstSimEHPlmnInfo->ucEhPlmnNum)
    {
        pstSimEHPlmnInfo->ucEhPlmnNum   = NAS_MML_MAX_EHPLMN_NUM;

        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_BuildNcellPlmnSearchEquPlmnInfo_PlmnSelection:other modem ehplmn num incorrect!");
    }

    /* ��Чplmn��Ϣ�м���modem0��EHPLMN��Ϣ */
    for (ulIndex = 0; ulIndex < pstSimEHPlmnInfo->ucEhPlmnNum; ulIndex++)
    {
        if (ucSndEplmnNum >= MMA_MMC_MAX_EQUPLMN_NUM)
        {
            pstOutEplmnInfo->ucEquPlmnNum = ucSndEplmnNum;
            PS_MEM_CPY(pstOutEplmnInfo->astEquPlmnAddr, astEquPlmnAddr, sizeof(pstOutEplmnInfo->astEquPlmnAddr));
            return;
        }

        astEquPlmnAddr[ucSndEplmnNum].ulMcc = pstSimEHPlmnInfo->astEhPlmnInfo[ulIndex].stPlmnId.ulMcc;
        astEquPlmnAddr[ucSndEplmnNum].ulMnc = pstSimEHPlmnInfo->astEhPlmnInfo[ulIndex].stPlmnId.ulMnc;
        ucSndEplmnNum++;
    }

    pstOutEplmnInfo->ucEquPlmnNum = ucSndEplmnNum;
    PS_MEM_CPY(pstOutEplmnInfo->astEquPlmnAddr, astEquPlmnAddr, sizeof(pstOutEplmnInfo->astEquPlmnAddr));

    return;
}
VOS_VOID NAS_MMC_SndAsSpecPlmnSearchReq_PlmnSelection(
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstDestPlmnList,
    NAS_MML_FORBIDPLMN_ROAMING_LAS_INFO_STRU               *pstForbRoamLaInfo
)
{
    NAS_MMC_NCELL_SEARCH_INFO_STRU                         *pstNcellSearchInfo = VOS_NULL_PTR;
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstDestPlmn = VOS_NULL_PTR;

    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;

    PS_MEM_SET(&stSndEquPlmnInfo, 0, sizeof(stSndEquPlmnInfo));

    pstNcellSearchInfo = NAS_MMC_GetNcellSearchInfo();
    enPlmnSearchScene  = NAS_MMC_GetPlmnSearchScene_PlmnSelection();
    pstDestPlmn        = &(pstDestPlmnList->astPlmnId[0]);

    if ((VOS_TRUE == NAS_MMC_IsInterPlmnSearch_PlmnSelection())
     && (NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_NCELL_SEARCH == enPlmnSearchScene))
    {
        /* ����ncell������Чplmn����Ϊmodem1�ĵ�Чplmn+modem0��ЧEHPLMN+modem0��RPLMN */
        NAS_MMC_BuildNcellPlmnSearchEquPlmnInfo_PlmnSelection(&stSndEquPlmnInfo, pstDestPlmn->enRat);

        /* ɾ����ֹ���� */
        NAS_MMC_BuildEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndAsNcellSpecPlmnSearchReq(&pstNcellSearchInfo->stTdsNcellInfo,
                                            &stSndEquPlmnInfo);
    }
    else
    {
        NAS_MMC_SndAsSpecPlmnSearchReq(pstDestPlmnList, pstForbRoamLaInfo);
    }


}
VOS_UINT32 NAS_MMC_IsNeedAddEhplmnWhenSearchRplmn_PlmnSelection(
    NAS_MML_PLMN_WITH_RAT_STRU         *pstDestPlmn
)
{
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene;
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList = VOS_NULL_PTR;
    VOS_UINT32                                              ulEventType;
    VOS_UINT8                                               ucSearchRplmnAndHplmnFlg;

    enPlmnSearchScene                = NAS_MMC_GetPlmnSearchScene_PlmnSelection();
    pstPlmnSelectionList             = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();
    enPlmnType                       = NAS_MMC_GetPlmnTypeInPlmnSelectionList(pstDestPlmn, pstPlmnSelectionList);
    ulEventType                      = NAS_MMC_GetCurrFsmEventType();
    ucSearchRplmnAndHplmnFlg         = NAS_MMC_GetSearchRplmnAndHplmnFlg_PlmnSelection(pstDestPlmn->enRat);

    if ( (VOS_TRUE == NAS_MMC_IsRPlmnFirstSearchWithHPlmn())
      && (NAS_MMC_PLMN_TYPE_RPLMN == enPlmnType)
      && (VOS_FALSE == ucSearchRplmnAndHplmnFlg)
      && ( (ulEventType == NAS_BuildEventType(WUEPS_PID_MMA, ID_MMA_MMC_PLMN_SEARCH_REQ))
        || (NAS_MMC_PLMN_SEARCH_SCENE_AREA_LOST == enPlmnSearchScene) ) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}




VOS_VOID NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(
    NAS_MML_PLMN_WITH_RAT_STRU         *pstDestPlmn
)
{

    VOS_UINT32                                              ulUserSpecPlmnSearch;
    NAS_MMC_FSM_ID_ENUM_UINT32                              enCurrentFsmId;
    VOS_UINT32                                              ulCurrentEventType;
    NAS_MML_FORBIDPLMN_ROAMING_LAS_INFO_STRU                stForbRoamLaInfo;
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU                        *pstForbPlmnInfo = VOS_NULL_PTR;
    VOS_UINT32                                              ulWaitWasPlmnSrchCnfTimerLen;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                         stDestPlmnList;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;

    NAS_MML_RAT_CAPABILITY_STATUS_ENUM_UINT8                enLteForbiddenStatus;

    enLteForbiddenStatus = NAS_MML_GetLteForbiddenStatusFlg();
    
    /* ��ȡLTE����״̬ */
    enLCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();
#endif

    ulWaitWasPlmnSrchCnfTimerLen = NAS_MMC_GetWaitWasPlmnSrchCnfTimerLen();
    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();

    /* ��ȡ�Ƿ��û�ָ������ */
    ulUserSpecPlmnSearch = NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType);
    if ( VOS_TRUE == ulUserSpecPlmnSearch )
    {
        /* ���û�ָ���������ӵ�ǰѡ��״̬���л�ȡFORBIDEN ROAMING LAS��Ϣ */
        PS_MEM_CPY( &stForbRoamLaInfo,
                    NAS_MMC_GetForbRoamLaInfo_PlmnSelection(),
                    sizeof(stForbRoamLaInfo) );
    }
    else
    {
        /* ���û�ָ����������ȫ�ֱ����л�ȡFORBIDEN ROAMING LAS��Ϣ */
        pstForbPlmnInfo                  = NAS_MML_GetForbidPlmnInfo();
        stForbRoamLaInfo.ucForbRoamLaNum = pstForbPlmnInfo->ucForbRoamLaNum;
        PS_MEM_CPY( stForbRoamLaInfo.astForbRomLaList,
                    pstForbPlmnInfo->astForbRomLaList,
                    sizeof(pstForbPlmnInfo->astForbRomLaList) );
    }

    /* ״̬����Ϊ��ǰδפ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);

    NAS_MMC_SetAsAnyCampOn(VOS_FALSE);

#if (FEATURE_ON == FEATURE_LTE)
    /* �����ǰ�Ѿ�֪ͨLTE disable,��ʱ����LTE�Ĺ���������Ҫenable LTE */

    if ((VOS_TRUE == NAS_MML_GetDisableLteRoamFlg())
     && (NAS_MML_NET_RAT_TYPE_LTE == pstDestPlmn->enRat)
     && (NAS_MML_RAT_CAPABILITY_STATUS_DISABLE != enLteForbiddenStatus))
    {
        /* ��ǰLTE�����ָ����û���ΪĬ��ֵ */
        if ((NAS_MML_LTE_CAPABILITY_STATUS_BUTT == enLCapabilityStatus)
         || (NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS == enLCapabilityStatus))
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
        NAS_MML_SetDisableLteRoamFlg(VOS_FALSE);
    }
#endif

    PS_MEM_SET(&stDestPlmnList, 0x0, sizeof(stDestPlmnList));

    stDestPlmnList.ulPlmnNum = 1;
    PS_MEM_CPY(&stDestPlmnList.astPlmnId[0], pstDestPlmn, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    if ( VOS_TRUE == NAS_MMC_IsNeedAddEhplmnWhenSearchRplmn_PlmnSelection(pstDestPlmn) )
    {
        NAS_MMC_AddEHPlmnInDestPlmnList(&stDestPlmnList);

        NAS_MMC_SetSearchRplmnAndHplmnFlg_PlmnSelection(pstDestPlmn->enRat, VOS_TRUE);
    }

    /* ���ݲ�ͬ�Ľ��뼼�������������� */
    switch (pstDestPlmn->enRat)
    {
#if (FEATURE_ON == FEATURE_LTE)
        case NAS_MML_NET_RAT_TYPE_LTE:
            NAS_MMC_SndLmmSpecPlmnSearchReq_PlmnSelection(&stDestPlmnList, ulUserSpecPlmnSearch);

            NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_PLMN_SEARCH_CNF);
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF_LEN);
            break;
#endif

        case NAS_MML_NET_RAT_TYPE_WCDMA:
            NAS_MMC_SndAsSpecPlmnSearchReq_PlmnSelection(&stDestPlmnList, &stForbRoamLaInfo);

            NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_WAS_PLMN_SEARCH_CNF);
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_PLMN_SEARCH_CNF, ulWaitWasPlmnSrchCnfTimerLen);
            break;

        case NAS_MML_NET_RAT_TYPE_GSM:
            NAS_MMC_SndAsSpecPlmnSearchReq(&stDestPlmnList, &stForbRoamLaInfo);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_GAS_PLMN_SEARCH_CNF);
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_PLMN_SEARCH_CNF, TI_NAS_MMC_WAIT_GAS_PLMN_SEARCH_CNF_LEN);
            break;

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection: Rat not support.");
            break;
    }


    return;
}
VOS_VOID NAS_MMC_SndSuspendReq_PlmnSelection(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
)
{
    /* ״̬����Ϊ��ǰδפ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);

    NAS_MMC_SetAsAnyCampOn(VOS_FALSE);

    /* ���ݲ�ͬ�Ľ��뼼�����õȴ������ظ���״̬ */
    switch (enRat)
    {
#if (FEATURE_ON == FEATURE_LTE)
        case NAS_MML_NET_RAT_TYPE_LTE:
            NAS_MMC_SndLmmSuspendReq();
            NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_SUSPEND_CNF);
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_SUSPEND_CNF, TI_NAS_MMC_WAIT_LMM_SUSPEND_CNF_LEN);
            break;
#endif

        case NAS_MML_NET_RAT_TYPE_WCDMA:
            NAS_MMC_SndAsSuspendReq(WUEPS_PID_WRR);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_WAS_SUSPEND_CNF);
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_SUSPEND_CNF, TI_NAS_MMC_WAIT_WAS_SUSPEND_CNF_LEN);
            break;

        case NAS_MML_NET_RAT_TYPE_GSM:
            NAS_MMC_SndAsSuspendReq(UEPS_PID_GAS);
            NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_GAS_SUSPEND_CNF);
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_SUSPEND_CNF, TI_NAS_MMC_WAIT_GAS_SUSPEND_CNF_LEN);
            break;

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SndSuspendReq_PlmnSelection: Rat not support.");
            break;
    }

    return;
}


VOS_VOID NAS_MMC_SndRelReq_PlmnSelection(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
)
{
    /* ���ݲ�ͬ�Ľ��뼼���ֱ��� */
    switch (enRat)
    {
#if (FEATURE_ON == FEATURE_LTE)
        case NAS_MML_NET_RAT_TYPE_LTE:

            /* ��EMM�����ͷ����� */
            NAS_MMC_SndLmmRelReq();
            NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_CONN_REL_IND);
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND, TI_NAS_MMC_WAIT_EPS_CONN_REL_IND_LEN);
            NAS_MMC_SetRelRequestFlag_PlmnSelection(VOS_TRUE);
            break;
#endif

        case NAS_MML_NET_RAT_TYPE_WCDMA:
        case NAS_MML_NET_RAT_TYPE_GSM:

            /* ��MM/GMM�����ͷ����� */
            NAS_MMC_SndMmRelReq();
            NAS_MMC_SndGmmRelReq();

            NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);

            NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_RRC_CONN_REL_IND);
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL, TI_NAS_MMC_WAIT_RRC_CONN_REL_LEN);
            NAS_MMC_SetRelRequestFlag_PlmnSelection(VOS_TRUE);
            break;

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SndRelReq_PlmnSelection: Rat not support.");
            break;
    }

    return;
}
VOS_VOID NAS_MMC_UpdateGUCoverageFlg_PlmnSelection(
    RRMM_PLMN_SEARCH_CNF_STRU          *pstSrchCnfMsg
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat;
    NAS_MMC_COVERAGE_TYPE_ENUM_UINT8    enCoverType;

    if (WUEPS_PID_WRR == pstSrchCnfMsg->MsgHeader.ulSenderPid)
    {
        enRat = NAS_MML_NET_RAT_TYPE_WCDMA;
    }
    else
    {
        enRat = NAS_MML_NET_RAT_TYPE_GSM;
    }

    if (RRC_PLMN_SEARCH_RLT_SPEC_FAIL == pstSrchCnfMsg->ulSearchRlt)
    {
        /* ��RRC�ĸ�������ת��ΪNAS��ʽ�ĸ������� */
        NAS_MMC_ConvertRrcCoverTypeToNasFormat(pstSrchCnfMsg->enCoverageType, &enCoverType);

        /* ����RRC�ĵ�ǰ���뼼��������Ϣ���� */
        NAS_MMC_SetRatCoverage_PlmnSelection(enRat, enCoverType);
    }
    else
    {
        /* ���µ�ǰ���뼼�����ڸ��� */
        if (NAS_MMC_COVERAGE_TYPE_BUTT == NAS_MMC_GetSpecRatCoverageTypeInRatSearchInfoList(
                                          NAS_MMC_GetRatCoverage_PlmnSelection(), enRat))
        {
            NAS_MMC_SetRatCoverage_PlmnSelection(enRat, NAS_MMC_COVERAGE_TYPE_LOW);
        }
    }

    return;
}

#if (FEATURE_ON == FEATURE_LTE)
VOS_VOID NAS_MMC_UpdateLmmCoverageFlg_PlmnSelection(
    LMM_MMC_PLMN_SRCH_CNF_STRU         *pstLmmSrchCnfMsg
)
{
    NAS_MMC_COVERAGE_TYPE_ENUM_UINT8    enCoverType;

    if (MMC_LMM_PLMN_SRCH_RLT_SPEC_FAIL == pstLmmSrchCnfMsg->enRlst)
    {
        /* ��RRC�ĸ�������ת��ΪNAS��ʽ�ĸ������� */
        NAS_MMC_ConvertLmmCoverTypeToNasFormat(pstLmmSrchCnfMsg->enCoverageType, &enCoverType);

        /* ����RRC�ĵ�ǰ���뼼��������Ϣ���� */
        NAS_MMC_SetRatCoverage_PlmnSelection(NAS_MML_NET_RAT_TYPE_LTE, enCoverType);
    }
    else
    {
        /* ���µ�ǰ���뼼�����ڸ��� */
        if (NAS_MMC_COVERAGE_TYPE_BUTT == NAS_MMC_GetSpecRatCoverageTypeInRatSearchInfoList(
                                          NAS_MMC_GetRatCoverage_PlmnSelection(), NAS_MML_NET_RAT_TYPE_LTE))
        {
            NAS_MMC_SetRatCoverage_PlmnSelection(NAS_MML_NET_RAT_TYPE_LTE, NAS_MMC_COVERAGE_TYPE_LOW);
        }
    }

    return;
}
VOS_UINT32 NAS_MMC_IsReCampLtePlmnValid_PlmnSelection(VOS_VOID)
{
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU                       *pstForbidPlmnInfo = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                                   *pstReCampLtePlmn  = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_MODE_ENUM_UINT8                  enPlmnSelectionMode;

    pstReCampLtePlmn          = NAS_MMC_GetCsPsMode1ReCampLtePlmn_PlmnSelection();
    pstForbidPlmnInfo         = NAS_MML_GetForbidPlmnInfo();
    enPlmnSelectionMode       = NAS_MMC_GetPlmnSelectionMode();

    /* ��ǰ��֧��Lʱ,���»�LTE��������Ч */
    if ( VOS_FALSE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE) )
    {
        return VOS_FALSE;
    }
    
    if (VOS_TRUE == NAS_MML_IsRatInForbiddenList(NAS_MML_NET_RAT_TYPE_LTE))
    {
        return VOS_FALSE;
    }

    /* ״̬���������л�LTE������Ч */
    if (VOS_FALSE == NAS_MML_IsPlmnIdValid(pstReCampLtePlmn))
    {
        return VOS_FALSE;
    }

    /* PS����Ч */
    if ( VOS_FALSE == NAS_MML_GetSimPsRegStatus() )
    {
        return VOS_FALSE;
    }

    /* PS��������ע�� */
    if ( VOS_FALSE == NAS_MML_GetPsAttachAllowFlg() )
    {
        return VOS_FALSE;
    }

    /* ��LTE�����ڽ�ֹ�б���,��Ӧ�ĳ����Ǹ�������GUע��ʱ��ԭ��ֵ#11�ܾ�*/
    if (VOS_TRUE == NAS_MML_IsSimPlmnIdInDestBcchPlmnList(pstReCampLtePlmn,
                                          pstForbidPlmnInfo->ucForbPlmnNum,
                                          pstForbidPlmnInfo->astForbPlmnIdList))
    {
        return VOS_FALSE;
    }

    /* ��LTE�����ڽ�ֹGPRS�б���,��Ӧ�ĳ����Ǹ�������GUע��ʱ��ԭ��ֵ#14�ܾ� */
    if (VOS_TRUE == NAS_MML_IsSimPlmnIdInDestBcchPlmnList(pstReCampLtePlmn,
                                          pstForbidPlmnInfo->ucForbGprsPlmnNum,
                                          pstForbidPlmnInfo->astForbGprsPlmnList))
    {
        return VOS_FALSE;
    }

    /* �Զ�ѡ��ģʽ��������Ч */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == enPlmnSelectionMode)
    {
        return VOS_TRUE;
    }

    /* �ֶ�ѡ��ģʽʱ�ػ�LTE�����Ƿ����� */
    if (VOS_TRUE == NAS_MMC_IsPlmnAllowedInManualMode(pstReCampLtePlmn))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_VOID NAS_MMC_ReCampLte_PlmnSelection(VOS_VOID)
{
    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8   enSpecPlmnSearchFlg;
    VOS_UINT32                                  ulEqual;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8             enRat;
    NAS_MML_PLMN_WITH_RAT_STRU                  stReCampLtePlmn;
    NAS_MML_PLMN_LIST_WITH_RAT_STRU     stDestPlmnList;
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32   enLteCapabilityStatus;

    enLteCapabilityStatus               = NAS_MML_GetLteCapabilityStatus();

    stReCampLtePlmn.enRat               = NAS_MML_NET_RAT_TYPE_LTE;
    enSpecPlmnSearchFlg                 = NAS_MMC_GetSpecPlmnSearchState();
    enRat                               = NAS_MML_GetCurrNetRatType();

    PS_MEM_CPY(&(stReCampLtePlmn.stPlmnId),
               NAS_MMC_GetCsPsMode1ReCampLtePlmn_PlmnSelection(),
               sizeof(stReCampLtePlmn.stPlmnId));

    ulEqual = NAS_MML_CompareBcchPlmnwithSimPlmn(&(stReCampLtePlmn.stPlmnId),
                                                  NAS_MML_GetCurrCampPlmnId());

    if (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLteCapabilityStatus)
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

    /* ��ǰ�Ѿ�פ��LTE����,�Ҹ���������Ҫ����פ��������ͬ */
    if ( (NAS_MML_NET_RAT_TYPE_LTE == enRat)
      && (NAS_MMC_SPEC_PLMN_SEARCH_STOP == enSpecPlmnSearchFlg)
      && (VOS_TRUE == ulEqual) )
    {
        /* ����פ��LTEʱ������״̬�ϱ� */
        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_CS));
        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_PS));

        /* ���ϲ�״̬��ָʾѡ��ʧ�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

        /* �˳����״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return;
    }

    /* ״̬����Ϊ��ǰδפ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);


    switch (enRat)
    {
        /* suspend������detach PS�ˣ����»�ȡ��һ������ʧ�� */
        case NAS_MML_NET_RAT_TYPE_BUTT:
        case NAS_MML_NET_RAT_TYPE_LTE:

            /* ��MM��GMM��������ָʾ */
            NAS_MMC_SndGmmPlmnSchInit();
            NAS_MMC_SndMmPlmnSchInit();

            NAS_MMC_SndMmaCampOnInd(VOS_FALSE);

            NAS_MMC_UpdateRegStateSpecPlmnSearch();

            /* ��LMM����ָ������ */
            PS_MEM_SET(&stDestPlmnList, 0x0, sizeof(stDestPlmnList));

            stDestPlmnList.ulPlmnNum = 1;
            PS_MEM_CPY(&stDestPlmnList.astPlmnId[0], &stReCampLtePlmn, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

            NAS_MMC_SndLmmSpecPlmnSearchReq(&stDestPlmnList, VOS_FALSE);

            /* ״̬Ǩ��NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_PLMN_SEARCH_CNF_CSPSMODE1_RECAMP_LTE */
            NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_PLMN_SEARCH_CNF_CSPSMODE1_RECAMP_LTE);

            /* ����������ʱ��TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF */
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF, TI_NAS_MMC_WAIT_LMM_PLMN_SEARCH_CNF_LEN);

            break;

        case NAS_MML_NET_RAT_TYPE_WCDMA:


            /* ��MM��GMM��������ָʾ */
            NAS_MMC_SndGmmPlmnSchInit();
            NAS_MMC_SndMmPlmnSchInit();

            NAS_MMC_SndMmaCampOnInd(VOS_FALSE);

            NAS_MMC_UpdateRegStateSpecPlmnSearch();

            /* ��WRR���͹������� */
            NAS_MMC_SndAsSuspendReq(WUEPS_PID_WRR);

            /* ״̬Ǩ��NAS_MMC_PLMN_SELECTION_STA_WAIT_WAS_SUSPEND_CNF_CSPSMODE1_RECAMP_LTE */
            NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_WAS_SUSPEND_CNF_CSPSMODE1_RECAMP_LTE);

            /* ����������ʱ��TI_NAS_MMC_WAIT_WAS_SUSPEND_CNF */
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_WAS_SUSPEND_CNF, TI_NAS_MMC_WAIT_WAS_SUSPEND_CNF_LEN);

            break;

        case NAS_MML_NET_RAT_TYPE_GSM:


            /* ��MM��GMM��������ָʾ */
            NAS_MMC_SndGmmPlmnSchInit();
            NAS_MMC_SndMmPlmnSchInit();

            NAS_MMC_SndMmaCampOnInd(VOS_FALSE);

            NAS_MMC_UpdateRegStateSpecPlmnSearch();

            /* ��Gas���͹������� */
            NAS_MMC_SndAsSuspendReq(UEPS_PID_GAS);

            /* ״̬Ǩ��NAS_MMC_PLMN_SELECTION_STA_WAIT_GAS_SUSPEND_CNF_CSPSMODE1_RECAMP_LTE */
            NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_GAS_SUSPEND_CNF_CSPSMODE1_RECAMP_LTE);

            /* ����������ʱ��TI_NAS_MMC_WAIT_GAS_SUSPEND_CNF */
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_GAS_SUSPEND_CNF, TI_NAS_MMC_WAIT_GAS_SUSPEND_CNF_LEN);

            break;

        default:

            /* �쳣��ӡ */
            NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_ReCampLte_PlmnSelection,ERROR:Rat Error");

            break;
    }

    return;
}

#endif  /* (FEATURE_ON == FEATURE_LTE) */



VOS_VOID NAS_MMC_UpdatePlmnSearchInfo_PlmnSelection(
    NAS_MMC_INTER_PLMN_SEARCH_REQ_STRU                     *pstPlmnSearchReq,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                          i;

    for (i = 0; i < NAS_MML_MAX_RAT_NUM; i++)
    {
        if (NAS_MML_NET_RAT_TYPE_BUTT == pstPlmnSearchReq->astInterPlmnSearchInfo[i].enRatType)
        {
            continue;
        }

        /* ���õ�ǰ���뼼��������ȫƵ���� */
        NAS_MMC_SetAllBandSearch_PlmnSelection(pstPlmnSearchReq->astInterPlmnSearchInfo[i].enRatType, VOS_TRUE);

        /* ���µ�ǰ���뼼�������縲������ */
        NAS_MMC_SetRatCoverage_PlmnSelection(pstPlmnSearchReq->astInterPlmnSearchInfo[i].enRatType,
                                             pstPlmnSearchReq->astInterPlmnSearchInfo[i].enCoverType);

        /* �Զ����� */
        if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
        {
            /* ���������Ҫ��ʣ�����粹�䵽ѡ���б� */
            NAS_MMC_UpdatePlmnListInPlmnSelectionList(&(pstPlmnSearchReq->astInterPlmnSearchInfo[i]),
                                                      pstPlmnSelectionListInfo,
                                                      VOS_TRUE);

            /*���¶Ը����������������*/
            NAS_MMC_SortAvailPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ������ϲ���Ҫ��ʣ�����粹�䵽ѡ���б� */
            NAS_MMC_UpdatePlmnListInPlmnSelectionList(&(pstPlmnSearchReq->astInterPlmnSearchInfo[i]),
                                                      pstPlmnSelectionListInfo,
                                                      VOS_FALSE);
        }
    }

    /* ��ά�ɲ⣬���ѡ���б���Ϣ */
    NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionListInfo);

    return;
}


VOS_VOID NAS_MMC_UpdatePlmnSearchList_PlmnSelection(
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    *pstInterPlmnSearchInfo
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList = VOS_NULL_PTR;

    pstPlmnSelectionList = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    /* �Զ�����������������ΪREG_HPLMN,Ҳ��ΪREG_PREF_PLMN,�����������������������������Ѿ�����ѡ���б����� */
    if (VOS_TRUE == NAS_MMC_IsNeedSearchAvailPlmn_PlmnSelection())
    {
        /* ���������Ҫ��ʣ�����粹�䵽ѡ���б� */
        NAS_MMC_UpdatePlmnListInPlmnSelectionList(pstInterPlmnSearchInfo,
                                                  pstPlmnSelectionList,
                                                  VOS_TRUE);

        /*���¶Ը����������������*/
        NAS_MMC_SortAvailPlmnInPlmnSelectionList(pstPlmnSelectionList);
    }
    else
    {
        /* ������ϲ���Ҫ��ʣ�����粹�䵽ѡ���б� */
        NAS_MMC_UpdatePlmnListInPlmnSelectionList(pstInterPlmnSearchInfo,
                                                  pstPlmnSelectionList,
                                                  VOS_FALSE);
    }

    /* ��ά�ɲ⣬���ѡ���б���Ϣ */
    NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionList);

    return;
}
VOS_VOID NAS_MMC_RefreshPlmnSelectionListAfterRegFail_PlmnSelection(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAdditionalAction
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList = VOS_NULL_PTR;
    NAS_MMC_RAT_SEARCH_INFO_STRU                           *pstSearchRatInfo     = VOS_NULL_PTR;
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType;
    NAS_MML_PLMN_WITH_RAT_STRU                              stCurrPlmnWithRat;

#if (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                                              ulDisableLteByCsPsModeOne;

    ulDisableLteByCsPsModeOne = NAS_MMC_IsDisableLteNeedLocalReleaseEpsConn();
#endif


    pstPlmnSelectionList = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();
    pstSearchRatInfo     = NAS_MMC_GetRatCoverage_PlmnSelection();

    stCurrPlmnWithRat.stPlmnId = (*NAS_MML_GetCurrCampPlmnId());
    stCurrPlmnWithRat.enRat    = NAS_MML_GetCurrNetRatType();

    /* ��ѡ���б��л�ȡ��ǰ��������� */
    enPlmnType = NAS_MMC_GetPlmnTypeInPlmnSelectionList(&stCurrPlmnWithRat, pstPlmnSelectionList);

    if ((NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION == enAdditionalAction))
    {
#if (FEATURE_ON == FEATURE_LTE)
        if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == NAS_MML_GetLteCapabilityStatus())
         && ( VOS_TRUE == ulDisableLteByCsPsModeOne))
        {
            NAS_MMC_RefreshPlmnSelectionList_DisableLte_CsPsMode1RegSuccess(pstPlmnSelectionList, pstSearchRatInfo);
        }
        /* ��ǰ����������������ΪRplmn����Eplmnʱ����Ҫ��ѡ���б���EPLMN���е��� */
        else if ((NAS_MMC_PLMN_TYPE_RPLMN == enPlmnType)
         || (NAS_MMC_PLMN_TYPE_EPLMN == enPlmnType)
         || (NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN == enPlmnType))
        {
            /* ��Ҫ����PLMN SELECTIONʱ,�Զ�ѡ��ģʽ��������Ӧ����������EHPLMN,
               �ֶ�ѡ��ģʽֻ�������û�ָ������ */
            NAS_MMC_RefreshEPlmnInPlmnSelectionList(pstPlmnSelectionList);
        }
        else
        {
        }
#else
        if ((NAS_MMC_PLMN_TYPE_RPLMN == enPlmnType)
         || (NAS_MMC_PLMN_TYPE_EPLMN == enPlmnType)
         || (NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN == enPlmnType))
        {
            /* ��Ҫ����PLMN SELECTIONʱ,�Զ�ѡ��ģʽ��������Ӧ����������EHPLMN,
               �ֶ�ѡ��ģʽֻ�������û�ָ������ */
            NAS_MMC_RefreshEPlmnInPlmnSelectionList(pstPlmnSelectionList);
        }
#endif
    }
    else if (NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN == enAdditionalAction)
    {
        /* ��Ҫ������ǰ��������λ����ʱ,��Ҫ����ǰ��������δ����ע��Ľ��뼼��
           ����ѡ���б���ǰ��,��ǰ���뼼�������������뼼��ǰ�� */

        NAS_MMC_RefreshCurPlmnInPlmnSelectionList(pstPlmnSelectionList, pstSearchRatInfo, enPlmnType);
    }
    else
    {
    }

    /* ��ά�ɲ⣬���ѡ���б���Ϣ */
    NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionList);

    return;
}
VOS_VOID NAS_MMC_SearchNextPlmn_PlmnSelection(VOS_VOID)
{
    NAS_MML_PLMN_WITH_RAT_STRU          stDestPlmn;

	PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));

    /*  ��ȡ�¸�����ʧ�ܣ�����г����������� */
    if (VOS_TRUE != NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
    {
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PLMN_SELECTION_FAILURE,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);


        NAS_MMC_GetNextSearchPlmnFail_PlmnSelection();

        return;
    }

    /* ��GMM��������ָʾ */
    NAS_MMC_SndGmmPlmnSchInit();

    /* ��MM��������ָʾ */
    NAS_MMC_SndMmPlmnSchInit();

    NAS_MMC_SndMmaCampOnInd(VOS_FALSE);

    NAS_MMC_UpdateRegStateSpecPlmnSearch();

    /* ��Ҫ����ǰģʽ���л����ȴ�����״̬������������ʱ�� */
    if (stDestPlmn.enRat == NAS_MML_GetCurrNetRatType())
    {
        /* �������LMM�����������󣬸��ݲ�ͬ������ģʽ��Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSpecPlmnSearchReq_PlmnSelection(&stDestPlmn);
    }
    else
    {
        /* �������LMM���͹������󣬸��ݵ�ǰ��ͬ�Ľ��뼼����Ǩ�Ƶ���ͬ��L2״̬��������������ʱ�� */
        NAS_MMC_SndSuspendReq_PlmnSelection(NAS_MML_GetCurrNetRatType());
    }

    return;
}




VOS_VOID NAS_MMC_PerformAdditionalActionRegFinished_PlmnSelection()
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCurCampInfo           = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stCurPlmn;

    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstCurrSearchingPlmn = VOS_NULL_PTR;

    enPlmnSearchScene     = NAS_MMC_GetPlmnSearchScene_PlmnSelection();
    pstCurrSearchingPlmn  = NAS_MMC_GetCurrSearchingPlmn_PlmnSelection();

    /* ��ȡ��ǰפ����������Ϣ */
    pstCurCampInfo     = NAS_MML_GetCurrCampPlmnInfo();
    stCurPlmn.stPlmnId = pstCurCampInfo->stLai.stPlmnId;
    stCurPlmn.enRat    = pstCurCampInfo->enNetRatType;

    /* ��ȡ�����ȼ������б� */
    pstPlmnSelectionListInfo = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    /* ���µ�ǰ���������״̬Ϊ��ע������ڴ��ָ����ȼ������У��������Ѵ����� */
    NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&stCurPlmn,
                                            NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,
                                            pstPlmnSelectionListInfo);

    /* ncell��������,ϵͳ��Ϣ�ϱ������粻һ����ncell�ѵ����磬����ˢ�������б�ʱӦ��ˢ��֮ǰ������plmn */
    if ((VOS_TRUE == NAS_MMC_IsInterPlmnSearch_PlmnSelection())
     && (NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_NCELL_SEARCH == enPlmnSearchScene))
    {
        stCurPlmn.stPlmnId = pstCurrSearchingPlmn->stPlmnId;
        stCurPlmn.enRat    = pstCurrSearchingPlmn->enRat;
        NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(&stCurPlmn,
                                            NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,
                                            pstPlmnSelectionListInfo);
    }

    /* �������ȼ��Ƚϱ��ó�CS/PS�Ĵ������ȼ�����ת��Ϊ״̬����һ����Ϊ */
    enCsAdditionalAction = NAS_MMC_GetCsRegAdditionalAction_PlmnSelection();
    enPsAdditionalAction = NAS_MMC_GetPsRegAdditionalAction_PlmnSelection();
    enAdditionalAction   = NAS_MMC_GetPrioAddtionalAction(enCsAdditionalAction, enPsAdditionalAction);

    if (NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT == enAdditionalAction)
    {
        return;
    }

    /* ����L��GU��ע�������������ô˺�������������ʱ����ͣһ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_EPS_REG_IND);
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND);

    /* ���Ӳ���Ϊפ����ǰ���� */
    if ((NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_ACCESS_BARRED == enAdditionalAction))
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_SUCC);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return;
    }

    /* �����NCELL����������Ҫ���������������ˣ��˳�״̬�� */
    if (NAS_MMC_PLMN_SEARCH_SCENE_REG_NCELL == NAS_MMC_GetPlmnSearchScene_PlmnSelection())
    {
        /* ���ϲ�״̬��ָʾѡ��ʧ�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return;
    }

    /* �������Ӵ��ڣ��ҵ�ǰ��Ҫ��������Ǩ�Ƶ��ȴ������ͷŵ�״̬ */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
#if (FEATURE_ON == FEATURE_LTE)
        if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
        {
            if (VOS_FALSE == NAS_MMC_IsDisableLteNeedWaitEpsConnRel_PlmnSelection())
            {
                return;
            }

            NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_CONN_REL_IND);

            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_CONN_REL_IND, TI_NAS_MMC_WAIT_EPS_CONN_REL_IND_LEN);
        }
        else
#endif
        {
            NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_RRC_CONN_REL_IND);

            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_RRC_CONN_REL, TI_NAS_MMC_WAIT_RRC_CONN_REL_LEN);
        }

        return;
    }

    if ((NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL == enAdditionalAction))
    {
        /* ���ϲ�״̬��ָʾѡ��ʧ�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return;
    }

    /* ��enRegAdditionalActionΪ��������������͸����б� */
    NAS_MMC_RefreshPlmnSelectionListAfterRegFail_PlmnSelection(enAdditionalAction);

    NAS_MMC_SearchNextPlmn_PlmnSelection();

    return;
}
VOS_VOID NAS_MMC_PerformAdditionalActionConnRel_PlmnSelection(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enPsAdditionalAction;

    /* �������ȼ��Ƚϱ��ó�CS/PS�Ĵ������ȼ�����ת��Ϊ״̬����һ����Ϊ */
    enCsAdditionalAction = NAS_MMC_GetCsRegAdditionalAction_PlmnSelection();
    enPsAdditionalAction = NAS_MMC_GetPsRegAdditionalAction_PlmnSelection();
    enAdditionalAction   = NAS_MMC_GetPrioAddtionalAction(enCsAdditionalAction, enPsAdditionalAction);

    /* ������Ӳ���Ϊ�����ȴ�,��ʾ�ȴ����ӹ������ֽ��յ���ע����,��ʱ��״̬Ǩ�ص��ȴ�ע���� */
    if (NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT == enAdditionalAction)
    {
#if (FEATURE_ON == FEATURE_LTE)
        if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
        {
            NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_EPS_REG_IND);

            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_EPS_REG_IND, TI_NAS_MMC_WAIT_EPS_REG_IND_LEN);
        }
        else
#endif
        {
            NAS_MMC_FSM_SetCurrState(NAS_MMC_PLMN_SELECTION_STA_WAIT_CSPS_REG_IND);

            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_CSPS_REG_IND, TI_NAS_MMC_WAIT_CSPS_REG_IND_LEN);
        }

        return;
    }

    /* ���Ӳ���Ϊפ����ǰ���� */
    if ((NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_ACCESS_BARRED == enAdditionalAction))
    {
        /* �ظ�״̬���˳��ɹ� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_SUCC);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return;
    }

    if ((NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL == enAdditionalAction)
     || (NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL == enAdditionalAction))
    {
        /* ���ϲ�״̬��ָʾѡ��ʧ�� */
        NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

        /* ֱ���˳���ǰ״̬�� */
        NAS_MMC_FSM_QuitFsmL2();

        return;
    }

    /* ��enRegAdditionalActionΪ��������������͸����б� */
    NAS_MMC_RefreshPlmnSelectionListAfterRegFail_PlmnSelection(enAdditionalAction);

    NAS_MMC_SearchNextPlmn_PlmnSelection();

    return;
}




VOS_UINT32 NAS_MMC_ProcUserSearchCsRegRslt_PlmnSelection(
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsCause
)
{
    VOS_UINT32                          ulRlst;

    ulRlst = VOS_FALSE;

    /* ֻҪ��һ����ע��ɹ�����MMA�����û�ָ�������ɹ� */
    if (NAS_MML_REG_FAIL_CAUSE_NULL == enCsCause)
    {

        /* ���õ�ǰע��״̬Ϊ�ɹ� */
        NAS_MMC_SetUserSpecPlmnRegisterStatus(VOS_TRUE);

        NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_SUCCESS);

#if   (FEATURE_ON == FEATURE_LTE)
        /* ��LMM�����û���������֪ͨ */
        if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
        {
            NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
        }
#endif

        return VOS_TRUE;
    }

    /*
       ע��ʧ��ʱ
       1��CS������ע��ʧ�ܣ�ԭ��ֵ��#2,#3,#6������PSע����֪ͨTAF�������
       2��ĳһ������ע��ʧ��ԭ��ֵ��#11,#12,#13��֪ͨTAF����ʧ��
       3��CS Other Cause�������ȴ�PS��ע����
    */

    switch(enCsCause)
    {
        /* ע��ʧ��ԭ��ֵ��#11,#12,#13��֪ͨTAFָ������ʧ�� */
        case NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW:
        case NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW:
        case NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW:
        case NAS_MML_REG_FAIL_CAUSE_AUTH_REJ:

            NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_FAIL);

#if   (FEATURE_ON == FEATURE_LTE)
            /* ��LMM�����û���������֪ͨ */
            if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
            {
                NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
            }
#endif

            ulRlst = VOS_TRUE;
            break;

        /* ����ԭ��ֵ���ݲ���MMA�����û�ָ����������������ȴ�PS���ע���� */
        default:
            break;
    }
    return ulRlst;
}
VOS_UINT32 NAS_MMC_ProcUserSearchPsRegRslt_PlmnSelection(
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsCause
)
{
    VOS_UINT32                          ulRlst;

    ulRlst = VOS_FALSE;

    /* ֻҪ��һ����ע��ɹ���֪ͨ�û������ɹ�, CAUSE_NULL��ʾע��ɹ� */
    if (NAS_MML_REG_FAIL_CAUSE_NULL == enPsCause)
    {

        /* ���õ�ǰע��״̬Ϊ�ɹ� */
        NAS_MMC_SetUserSpecPlmnRegisterStatus(VOS_TRUE);

        /* �ϱ��ɹ������µ�ǰ��������Ϊ�ֶ����� */
        NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_SUCCESS);

#if   (FEATURE_ON == FEATURE_LTE)
        /* ��LMM�����û���������֪ͨ */
        if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
        {
            NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
        }
#endif

        return VOS_TRUE;
    }

    /*  ע��ʧ��ʱ
        1��PS��ע��ʧ�ܣ�ԭ��ֵ��#3,#6�������޿�״̬��֪ͨTAF����ʧ��
        2��ĳһ������ע��ʧ��ԭ��ֵ��#11,#12,#13��֪ͨTAF����ʧ��
        3��PSע��ʧ��#8��֪ͨTAF����ʧ��
        4��PS����ע��ʧ��#7�������ȴ�CS��ע����
        5��PS����ע��ʧ��#7�������ȴ�CS��ע����
        6��ע������#9,#10�����ȴ�PSע����;���PSע�����ɹ������سɹ������PSע��ʧ�ܣ���������ʧ��
    */

    switch(enPsCause)
    {
        /* ԭ��ֵ#3,#6,#8�Ĵ��� */
        case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_MS:
        case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME:
        case NAS_MML_REG_FAIL_CAUSE_GPRS_SERV_AND_NON_GPRS_SERV_NOT_ALLOW:

            NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_FAIL);

#if   (FEATURE_ON == FEATURE_LTE)
            /* ��LMM�����û���������֪ͨ */
            if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
            {
                NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
            }
#endif

            ulRlst = VOS_TRUE;
            break;

        /* ע��ʧ��ԭ��ֵ��#11,#12,#13��֪ͨTAFָ������ʧ�� */
        case NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW:
        case NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW:
        case NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW:
        case NAS_MML_REG_FAIL_CAUSE_AUTH_REJ:

            NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_FAIL);

#if   (FEATURE_ON == FEATURE_LTE)
            /* ��LMM�����û���������֪ͨ */
            if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
            {
                NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
            }
#endif

            ulRlst = VOS_TRUE;
            break;

        /* PSע��ʧ��ԭ��ֵ#7,#9,#10,#14�����ȴ�CSע���� */
        case NAS_MML_REG_FAIL_CAUSE_MS_ID_NOT_DERIVED:
        case NAS_MML_REG_FAIL_CAUSE_IMPLICIT_DETACHED:
        case NAS_MML_REG_FAIL_CAUSE_GPRS_SERV_NOT_ALLOW:
        case NAS_MML_REG_FAIL_CAUSE_GPRS_SERV_NOT_ALLOW_IN_PLMN:
            break;

        /* ����ԭ��ֵ�Ĵ��� ,�ȴ�CS�Ĵ����� */
        default:
            break;
    }

    return ulRlst;
}


VOS_UINT32 NAS_MMC_ProcUserSearchRegCause15_PlmnSelection(
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsCause,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsCause
)
{
    VOS_UINT32                          ulRlst;

    ulRlst = VOS_FALSE;

    /*  PSע��15��ԭ��ֵʱ
        1��CSע��ʧ��ԭ��ֵ��#11,#12,#13��֪ͨTAF����ʧ��
        2��CSע��ʧ������ԭ��ֵ��������ע��

        CSע��15��ԭ��ֵʱ
        1��PS��ע��ʧ�ܣ�ԭ��ֵ��#3,#6�������޿�״̬��֪ͨTAF����ʧ��
        2��PSע��ʧ��ԭ��ֵ��#11,#12,#13��֪ͨTAF����ʧ��
        3��PSע��ʧ��#8��֪ͨTAF����ʧ��
        4��PSע��ʧ������ԭ��ֵ��������ע��
    */

    switch (enCsCause)
    {
        /* ע��ʧ��ԭ��ֵ��#11,#12,#13��֪ͨTAFָ������ʧ�� */
        case NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW:
        case NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW:
        case NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW:
        case NAS_MML_REG_FAIL_CAUSE_AUTH_REJ:

            NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_FAIL);

#if   (FEATURE_ON == FEATURE_LTE)
            /* ��LMM�����û���������֪ͨ */
            if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
            {
                NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
            }
#endif

            ulRlst = VOS_TRUE;
            break;

        /* ����ԭ��ֵ���ݲ���MMA�����û�ָ����������������´�����ע�� */
        default:
            break;
    }

    switch (enPsCause)
    {
        /* ԭ��ֵ#3,#6,#8�Ĵ��� */
        case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_MS:
        case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME:
        case NAS_MML_REG_FAIL_CAUSE_GPRS_SERV_AND_NON_GPRS_SERV_NOT_ALLOW:

            NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_FAIL);
#if   (FEATURE_ON == FEATURE_LTE)
            /* ��LMM�����û���������֪ͨ */
            if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
            {
                NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
            }
#endif
            ulRlst = VOS_TRUE;
            break;

        /* ע��ʧ��ԭ��ֵ��#11,#12,#13��֪ͨTAFָ������ʧ�� */
        case NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW:
        case NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW:
        case NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW:
        case NAS_MML_REG_FAIL_CAUSE_AUTH_REJ:

            NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_FAIL);

#if   (FEATURE_ON == FEATURE_LTE)
            /* ��LMM�����û���������֪ͨ */
            if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
            {
                NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
            }
#endif

            ulRlst = VOS_TRUE;
            break;

        /* ����ԭ��ֵ���ݲ���MMA�����û�ָ����������������´�����ע�� */
        default:
            break;
    }

    return ulRlst;
}


VOS_UINT32 NAS_MMC_ProcUserSearchRegOtherCause_PlmnSelection(
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsCause,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsCause
)
{

    /* ֻҪ��һ����ע��ɹ���֪ͨ�û������ɹ�, CAUSE_NULL��ʾע��ɹ� */
    if ((NAS_MML_REG_FAIL_CAUSE_NULL == enCsCause)
     || (NAS_MML_REG_FAIL_CAUSE_NULL == enPsCause))
    {

        /* ���õ�ǰע��״̬Ϊ�ɹ� */
        NAS_MMC_SetUserSpecPlmnRegisterStatus(VOS_TRUE);

        /* �ϱ��ɹ������µ�ǰ��������Ϊ�ֶ����� */
        NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_SUCCESS);

#if   (FEATURE_ON == FEATURE_LTE)
        /* ��LMM�����û���������֪ͨ */
        if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
        {
            NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
        }
#endif

        return VOS_TRUE;
    }

    /* �������ΪCS ONLY����PS ONLY,��һ����ǰС����֧��ʱ,��Ҫ�����б�����һ
       �������ظ�ָ��������� */
    if ((NAS_MML_REG_FAIL_CAUSE_MS_CFG_DOMAIN_NOT_SUPPORT == enCsCause)
     && (NAS_MML_REG_FAIL_CAUSE_SERVING_CELL_DOMAIN_NOT_SUPPORT == enPsCause))
    {
        return VOS_FALSE;
    }

    if ((NAS_MML_REG_FAIL_CAUSE_SERVING_CELL_DOMAIN_NOT_SUPPORT == enCsCause)
     && (NAS_MML_REG_FAIL_CAUSE_MS_CFG_DOMAIN_NOT_SUPPORT == enPsCause))
    {
        return VOS_FALSE;
    }

    /* PSע��ʧ��ԭ��ֵ#9,#10�����ȴ�PSע���� */
    if ((NAS_MML_REG_FAIL_CAUSE_MS_ID_NOT_DERIVED == enPsCause)
     || (NAS_MML_REG_FAIL_CAUSE_IMPLICIT_DETACHED == enPsCause))
    {
        return VOS_FALSE;
    }

    NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_FAIL);

#if   (FEATURE_ON == FEATURE_LTE)
    /* ��LMM�����û���������֪ͨ */
    if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
    {
        NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
    }
#endif

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_ProcUserSearchRegRslt_PlmnSelection(
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsCause,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsCause
)
{
    /* CauseֵΪ NAS_MML_REG_FAIL_CAUSE_NULL ��ʾ����ע��ɹ���
       CauseֵΪ NAS_MML_REG_FAIL_CAUSE_BUTT ��ʾ����δ�յ�ע���� */

    /* ��δ��ȡ��ע����,�����ȴ�ע���� */
    if ((NAS_MML_REG_FAIL_CAUSE_BUTT == enPsCause)
     && (NAS_MML_REG_FAIL_CAUSE_BUTT == enCsCause))
    {
        return VOS_FALSE;
    }

    /* ��δ��ȡ��PSע��������ǰ��ȡ��CSע����, ����CS��ע��ԭ��ֵ */
    if ( (NAS_MML_REG_FAIL_CAUSE_BUTT == enPsCause)
      && (NAS_MML_REG_FAIL_CAUSE_BUTT != enCsCause) )
    {
        return NAS_MMC_ProcUserSearchCsRegRslt_PlmnSelection(enCsCause);
    }

    /* ��δ��ȡ��CSע��������ǰ��ȡ��PSע����������PS��ע��ԭ��ֵ */
    if ( (NAS_MML_REG_FAIL_CAUSE_BUTT == enCsCause)
      && (NAS_MML_REG_FAIL_CAUSE_BUTT != enPsCause) )
    {
        return NAS_MMC_ProcUserSearchPsRegRslt_PlmnSelection(enPsCause);
    }

    
    /* �������ע��������ȡ,CS��PSע��15��ԭ��ֵʱ,��ע�������� */
    if ( (NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL       == enCsCause)
      || (NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL       == enPsCause)
      || (NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_ROAM == enCsCause)
      || (NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_ROAM == enPsCause))
    {
        return NAS_MMC_ProcUserSearchRegCause15_PlmnSelection(enCsCause, enPsCause);
    }

    /* �������ע��������ȡ,CS��PSע��15��ԭ��ֵʱ,��ע�������� */
    if ((NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE <= enCsCause)
     || (NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE <= enPsCause))
    {
        return NAS_MMC_ProcUserSearchRegOtherCause_PlmnSelection(enCsCause, enPsCause);
    }

    /* ֻҪ��һ����ע��ɹ�����MMA�����û�ָ�������ɹ� */
    if ((NAS_MML_REG_FAIL_CAUSE_NULL == enCsCause)
     || (NAS_MML_REG_FAIL_CAUSE_NULL == enPsCause))
    {

        /* ���õ�ǰע��״̬Ϊ�ɹ� */
        NAS_MMC_SetUserSpecPlmnRegisterStatus(VOS_TRUE);

        NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_SUCCESS);

#if   (FEATURE_ON == FEATURE_LTE)
        /* ��LMM�����û���������֪ͨ */
        if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
        {
            NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
        }
#endif

        return VOS_TRUE;
    }

    /* �ѻ�ȡ��CS��PS��ע������������ȷԭ��ֵ����other cause��15,�ϱ�ʧ�� */
    NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_FAIL);

#if   (FEATURE_ON == FEATURE_LTE)
    /* ��LMM�����û���������֪ͨ */
    if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
    {
        NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
    }
#endif

    return VOS_TRUE;
}



VOS_VOID NAS_MMC_LoadInterSysFsm_PlmnSelection(
    MMC_SUSPEND_CAUSE_ENUM_UINT8        enSuspendCause
)
{
    switch ( enSuspendCause )
    {
        /* ����HANDOVER״̬�� */
        case MMC_SUSPEND_CAUSE_HANDOVER:
            NAS_MMC_FSM_InitFsmL2(NAS_MMC_FSM_INTER_SYS_HO);
            break;

        /* ���� CCO״̬�� */
        case MMC_SUSPEND_CAUSE_CELLCHANGE:
            NAS_MMC_FSM_InitFsmL2(NAS_MMC_FSM_INTER_SYS_CCO);
            break;

        /* ���� С����ѡ ״̬�� */
        case MMC_SUSPEND_CAUSE_CELLRESELECT:
        case MMC_SUSPEND_CAUSE_REDIRECTION:
            NAS_MMC_FSM_InitFsmL2(NAS_MMC_FSM_INTER_SYS_CELLRESEL);
            break;

        /* ���� ϵͳ������������ ״̬�� */
        case MMC_SUSPEND_CAUSE_PLMNSEARCH:
            NAS_MMC_FSM_InitFsmL2(NAS_MMC_FSM_INTER_SYS_OOS);
            break;

        default:
            NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_LoadInterSysFsm_PlmnSelection():default case");
            break;
    }

}



VOS_VOID NAS_MMC_ProcUserSpecPlmnSrchForbGprsInfo_PlmnSelection( VOS_VOID )
{
    NAS_MML_LAI_STRU                   *pstLai          = VOS_NULL_PTR;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsRegCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsRegCause;

    pstLai              = NAS_MML_GetCurrCampLai();
    enCsRegCause        = NAS_MMC_GetCsRegCause_PlmnSelection();
    enPsRegCause        = NAS_MMC_GetPsRegCause_PlmnSelection();

    /* ��GSM�£����CSע��ɹ���PSδע�ᣬ��ɾ��FORB GPRS��Ϣ */
    if ( (NAS_MML_NET_RAT_TYPE_GSM          == NAS_MML_GetCurrNetRatType())
      && (NAS_MML_REG_FAIL_CAUSE_NULL       == enCsRegCause)
      && (NAS_MML_REG_FAIL_CAUSE_BUTT       == enPsRegCause) )
    {
        NAS_MML_DelForbGprsPlmn(&(pstLai->stPlmnId));
    }

    return;

}
VOS_VOID NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection( VOS_VOID )
{
    MMA_MMC_PLMN_SEARCH_REQ_STRU        stPlmnSrhReg;
    VOS_UINT32                          ulBuildEventType;

    stPlmnSrhReg.MsgHeader.ulSenderCpuId     = VOS_LOCAL_CPUID;
    stPlmnSrhReg.MsgHeader.ulSenderPid       = WUEPS_PID_MMA;
    stPlmnSrhReg.MsgHeader.ulReceiverCpuId   = VOS_LOCAL_CPUID;
    stPlmnSrhReg.MsgHeader.ulReceiverPid     = WUEPS_PID_MMC;
    stPlmnSrhReg.MsgHeader.ulLength          = sizeof(MMA_MMC_PLMN_SEARCH_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    stPlmnSrhReg.MsgHeader.ulMsgName         = ID_MMA_MMC_PLMN_SEARCH_REQ;

    ulBuildEventType  = NAS_BuildEventType(WUEPS_PID_MMA, ID_MMA_MMC_PLMN_SEARCH_REQ);

    /*���õ�ǰ�����ϢΪ��������*/
    NAS_MMC_SaveCurEntryMsg(ulBuildEventType, (struct MsgCB*)&stPlmnSrhReg);

}
VOS_VOID  NAS_MMC_UpdateUserSpecPlmnSearchInfo_PlmnSelection(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    VOS_UINT8                           ucAccessMode
)
{
    NAS_MML_PLMN_WITH_RAT_STRU          stUserSpecPlmn;

    /* �����û�ָ����PlmnID������뼼�� */
    stUserSpecPlmn.stPlmnId.ulMcc = pstPlmnId->ulMcc;
    stUserSpecPlmn.stPlmnId.ulMnc = pstPlmnId->ulMnc;

    stUserSpecPlmn.enRat = ucAccessMode;

    NAS_MMC_SetUserSpecPlmnId(&stUserSpecPlmn);

    /* ����ָ������ǰפ����PlmnID�����뼼�� */
    NAS_MMC_SaveUserReselPlmnInfo();


    /* ��ǰΪָ���ѣ����ʼ��ѡ��״̬���Ľ�ֹ����LA��Ϣ���� */
    PS_MEM_SET( NAS_MMC_GetForbRoamLaInfo_PlmnSelection(),
                0X0, sizeof(NAS_MML_FORBIDPLMN_ROAMING_LAS_INFO_STRU) );

    PS_MEM_SET(NAS_MMC_GetForbPlmnForGrpsInfo_PlmnSelection(),
               0X0, sizeof(NAS_MML_PLMN_ID_STRU));

    /* ���õ�ǰע��״̬δע�� */
    NAS_MMC_SetUserSpecPlmnRegisterStatus(VOS_FALSE);

    /* ��ǰģʽ����MANUAL���򲻸���NV */
    if (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
    {
        return;
    }

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
VOS_VOID NAS_MMC_ProcCsRegRslt_PlmnSelection(
    MMMMC_CS_REG_RESULT_IND_STRU       *pstCsRegRsltInd
)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;
    NAS_MMC_SERVICE_ENUM_UINT8                              enService;

    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;

    NAS_MMC_FSM_ID_ENUM_UINT32                              enCurrentFsmId;
    VOS_UINT32                                              ulCurrentEventType;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_GU_ACTION_RSLT_INFO_STRU                        stActionRslt;
#endif

    enService = NAS_MMC_ConverMmStatusToMmc(NAS_MMC_REG_DOMAIN_CS,
                                            (NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8)pstCsRegRsltInd->ulServiceStatus);

    NAS_MMC_SetCsServiceStatus(enService);

    if (MM_MMC_LU_RESULT_SUCCESS == pstCsRegRsltInd->enLuResult)
    {
        NAS_MMC_SetCsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_NULL);

        enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

        /* ����ע��������ע����Ϣ�� */
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_CS, NAS_MML_REG_FAIL_CAUSE_NULL);

        /* ��Hplmn��Rej Lai��Ϣ����� */
        NAS_MMC_ClearHplmnRejDomainInfo(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_CS);

        /* ע��ɹ�ʱ����Ҫɾ��ForbPlmn,ForbLa,ForbGprs����Ϣ */
        NAS_MMC_DelForbInfo_GuRegRsltSucc(NAS_MMC_REG_DOMAIN_CS);

        NAS_MMC_SetUserSpecPlmnRegisterStatus(VOS_TRUE);

        NAS_MMC_UpdateCsRegStateCsRegSucc();

#if   (FEATURE_ON == FEATURE_LTE)

        /* ����ע��ʱ��֪ͨLMM��ʱLU�Ľ�� */
        if ((VOS_TRUE  == NAS_MML_IsRegFailCauseNotifyLmm(pstCsRegRsltInd->enRegFailCause))
         && (VOS_FALSE == pstCsRegRsltInd->ucIsComBined))
        {
            stActionRslt.enProcType     = NAS_MML_PROC_LAU;
            stActionRslt.enRegRst       = NAS_MML_REG_RESULT_SUCCESS;
            stActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_CS;
            stActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_CS;
            stActionRslt.enCnCause      = NAS_MML_REG_FAIL_CAUSE_NULL;
            stActionRslt.ulAttemptCount = 0;

            NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);
        }
#endif

        NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_CS, VOS_TRUE, pstCsRegRsltInd->enRegFailCause);
    }
    else
    {
        enAdditionalAction = NAS_MMC_ProcCsRegFail(MM_MMC_CS_REG_RESULT_IND, pstCsRegRsltInd);


        /* ������ԭ��ֵΪ#15, ����ӽ�ֹ������Ϣ��ѡ��״̬���Ľ�ֹ����LA������ȥ,
           ���û�ָ������ʱ������Ҫ�ѽ�ֹLA��������� */
        enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
        if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
        {
            NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
        }
        ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();
        if ( (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId, ulCurrentEventType))
          && (NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL == pstCsRegRsltInd->enRegFailCause) )
        {
            NAS_MML_AddForbRoamLa( NAS_MML_GetCurrCampLai(),
                                   NAS_MMC_GetForbRoamLaInfo_PlmnSelection() );
        }

        /* �������Ҫ�ȴ���һ��ע����,�򲻸���ע��ԭ��ֵ,�ȴ�conut�ﵽ�������,
           �Ա����û�ָ�������ظ���� */
        if ((NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT != enAdditionalAction)
         && (NAS_MMC_ADDITIONAL_ACTION_BUTT             != enAdditionalAction))
        {
            NAS_MMC_SetCsRegCause_PlmnSelection(pstCsRegRsltInd->enRegFailCause);
        }

        if ( VOS_TRUE == NAS_MML_IsNetworkRegFailCause(pstCsRegRsltInd->enRegFailCause) )
        {
            /* ֻ�ڸ�������ʵ����������ʱ��Ÿ���ע����Ϣ�� */
            NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_CS, pstCsRegRsltInd->enRegFailCause);
        }

        /* PSע��ɹ�, CSע��ʧ��, ԭ��Ϊ11 12 13 15�����, ��Ҫ����ñ�־.
           ��Ϊ��ʱCS�� RPLMNû�и���Ϊ�û�ָ����PLMN, �˺����ȴ���������������
           RPLMN����EPLMN, ����Ҫ����ñ�־. */

        NAS_MMC_UpdateUserSpecPlmnRegisterStatusWhenRegFail(pstCsRegRsltInd->enRegFailCause);


#if   (FEATURE_ON == FEATURE_LTE)

        /* С�� NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE�ľܾ�ԭ����UE���Է���ע���ԭ��ֵ����Ҫ֪ͨLMM */
        /* ����ע��ʱ��֪ͨLMM��ʱLU�Ľ�� */
        if ((VOS_TRUE  == NAS_MML_IsRegFailCauseNotifyLmm(pstCsRegRsltInd->enRegFailCause))
         && (VOS_FALSE == pstCsRegRsltInd->ucIsComBined))
        {
            /* ��ǰSYSCFG��L����ʱ����Ҫ��LMM֪ͨ */
            stActionRslt.enProcType     = NAS_MML_PROC_LAU;
            stActionRslt.enRegRst       = NAS_MML_REG_RESULT_FAILURE;
            stActionRslt.enReqDomain    = NAS_MMC_REG_DOMAIN_CS;
            stActionRslt.enRstDomain    = NAS_MMC_REG_DOMAIN_CS;
            stActionRslt.enCnCause      = pstCsRegRsltInd->enRegFailCause;
            stActionRslt.ulAttemptCount = pstCsRegRsltInd->ulLuAttemptCnt;

            NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);
        }
#endif
    }


    /* AdditionalAction��Ч������CS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(enAdditionalAction);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn_PlmnSelection())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndGuAsEquPlmnReq(&stSndEquPlmnInfo, NAS_MML_GetCurrNetRatType());
        NAS_MMC_SndOmEquPlmn();

        NAS_MMC_SndMmaEplmnInfoInd(&stSndEquPlmnInfo);
    }

    return;
}


VOS_VOID NAS_MMC_ProcPsRegRslt_PlmnSelection(
    GMMMMC_PS_REG_RESULT_IND_STRU      *pstPsRegRsltInd
)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;
    NAS_MMC_SERVICE_ENUM_UINT8                              enService;

    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;


    NAS_MMC_FSM_ID_ENUM_UINT32                              enCurrentFsmId;
    VOS_UINT32                                              ulCurrentEventType;

    NAS_MML_PLMN_ID_STRU                                   *pstForbPlmnForGprs = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                                   *pstCurrCampOnPlmn = VOS_NULL_PTR;

#if (FEATURE_ON == FEATURE_LTE)

    NAS_MMC_GU_ACTION_RSLT_INFO_STRU                        stActionRslt;
    NAS_MML_PROC_TYPE_ENUM_U32                              enProcType;

    /* ת�����͸�L��proctype */
    enProcType = NAS_MML_PROC_BUTT;

    NAS_MMC_ConverGmmActionTypeToMml(pstPsRegRsltInd->enGmmActionType, &enProcType);


    NAS_MMC_ConvertGmmRegDomainToMmcDomain(pstPsRegRsltInd->enReqDomain, &stActionRslt.enReqDomain);

    NAS_MMC_ConvertGmmRegDomainToMmcDomain(pstPsRegRsltInd->enRsltDomain, &stActionRslt.enRstDomain);



#endif

    enService = NAS_MMC_ConverMmStatusToMmc(NAS_MMC_REG_DOMAIN_PS,
                                            (NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8)pstPsRegRsltInd->ulServiceStatus);

    NAS_MMC_SetPsServiceStatus(enService);

    if (GMM_MMC_ACTION_RESULT_SUCCESS == pstPsRegRsltInd->enActionResult)
    {
        /* ����ע������Ϣ */
        NAS_MMC_SaveRegRsltCtx(GMM_MMC_PS_REG_RESULT_IND, pstPsRegRsltInd);
    
        NAS_MMC_SetPsRegCause_PlmnSelection(NAS_MML_REG_FAIL_CAUSE_NULL);
        enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

        /* ����ע��������ע����Ϣ�� */
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_PS, NAS_MML_REG_FAIL_CAUSE_NULL);
        NAS_MMC_ClearHplmnRejDomainInfo(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_PS);

        /* ע��ɹ�ʱ����Ҫɾ��ForbPlmn,ForbLa,ForbGprs����Ϣ */
        NAS_MMC_DelForbInfo_GuRegRsltSucc(NAS_MMC_REG_DOMAIN_PS);

        NAS_MMC_NotifyModeChange(NAS_MML_GetCurrNetRatType(),
                                 NAS_MML_GetSimPsRegStatus());


        NAS_MMC_SetUserSpecPlmnRegisterStatus(VOS_TRUE);

        NAS_MMC_UpdatePsRegStatePsRegSucc();

#if   (FEATURE_ON == FEATURE_LTE)

        /* �����̲�֪ͨ��LMM */
        if ( VOS_TRUE == NAS_MML_IsRegFailCauseNotifyLmm(pstPsRegRsltInd->enRegFailCause) )
        {
            stActionRslt.enProcType     = enProcType;
            stActionRslt.enRegRst       = NAS_MML_REG_RESULT_SUCCESS;
            stActionRslt.enCnCause      = NAS_MML_REG_FAIL_CAUSE_NULL;
            stActionRslt.ulAttemptCount = 0;

            NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);
        }

#endif

        NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_PS, VOS_TRUE, pstPsRegRsltInd->enRegFailCause);
    }
    else
    {
        enAdditionalAction = NAS_MMC_ProcPsRegFail(GMM_MMC_PS_REG_RESULT_IND,
                                                   pstPsRegRsltInd);


        /* ������ԭ��ֵΪ#15, ����ӽ�ֹ������Ϣ��ѡ��״̬���Ľ�ֹ����LA������ȥ,
           ���û�ָ������ʱ������Ҫ�ѽ�ֹLA��������� */
        enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
        if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
        {
            NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
        }
        ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();
        if ( (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId, ulCurrentEventType))
          && (NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL == pstPsRegRsltInd->enRegFailCause) )
        {
            NAS_MML_AddForbRoamLa( NAS_MML_GetCurrCampLai(),
                                   NAS_MMC_GetForbRoamLaInfo_PlmnSelection() );
        }

        if ( (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId, ulCurrentEventType))
          && (NAS_MML_REG_FAIL_CAUSE_GPRS_SERV_NOT_ALLOW_IN_PLMN == pstPsRegRsltInd->enRegFailCause) )
        {
            pstForbPlmnForGprs = NAS_MMC_GetForbPlmnForGrpsInfo_PlmnSelection();
            pstCurrCampOnPlmn  = NAS_MML_GetCurrCampPlmnId();

            *pstForbPlmnForGprs = *pstCurrCampOnPlmn;
        }

        /* �������Ҫ�ȴ���һ��ע����,�򲻸���ע��ԭ��ֵ,�ȴ�conut�ﵽ�������,
           �Ա����û�ָ�������ظ���� */
        if ((NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT != enAdditionalAction)
         && (NAS_MMC_ADDITIONAL_ACTION_BUTT             != enAdditionalAction))
        {
            NAS_MMC_SetPsRegCause_PlmnSelection(pstPsRegRsltInd->enRegFailCause);
        }

        if ( VOS_TRUE == NAS_MML_IsNetworkRegFailCause(pstPsRegRsltInd->enRegFailCause) )
        {
            /* ֻ�ڸ�������ʵ����������ʱ��Ÿ���ע����Ϣ�� */
            NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_PS, pstPsRegRsltInd->enRegFailCause);
        }

        NAS_MMC_UpdateUserSpecPlmnRegisterStatusWhenRegFail(pstPsRegRsltInd->enRegFailCause);

#if   (FEATURE_ON == FEATURE_LTE)

        /* С�� NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE�ľܾ�ԭ����UE���Է���ע���ԭ��ֵ����Ҫ֪ͨLMM */
        if ( VOS_TRUE == NAS_MML_IsRegFailCauseNotifyLmm(pstPsRegRsltInd->enRegFailCause) )
        {
            stActionRslt.enProcType     = enProcType;
            stActionRslt.enRegRst       = NAS_MML_REG_RESULT_FAILURE;
            stActionRslt.enCnCause      = pstPsRegRsltInd->enRegFailCause;
            stActionRslt.ulAttemptCount = pstPsRegRsltInd->ulRegCounter;

            NAS_MMC_SndLmmRegActionResultReq(&stActionRslt);
        }
#endif
    }

    /* AdditionalAction��Ч������PS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(enAdditionalAction);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn_PlmnSelection())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndGuAsEquPlmnReq(&stSndEquPlmnInfo, NAS_MML_GetCurrNetRatType());
        NAS_MMC_SndOmEquPlmn();

        NAS_MMC_SndMmaEplmnInfoInd(&stSndEquPlmnInfo);
    }

    return;
}
VOS_VOID  NAS_MMC_SndRslt_PlmnSelection(
    NAS_MMC_PLMN_SELECTION_RESULT_ENUM_UINT32               enPlmnSelectionRslt
)
{

    NAS_MMC_RAT_SEARCH_INFO_STRU       *pstSearchRatInfo = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList   = VOS_NULL_PTR;
    VOS_UINT32                          i;
    VOS_UINT32                          ulCampFlg;
    VOS_UINT32                          ulRatNum;

    MMA_MMC_PLMN_ID_STRU                stPlmnId;
    VOS_UINT32                          ulCurrentEventType;

    NAS_MMC_FSM_ID_ENUM_UINT32          enCurrentFsmId;

    pstPrioRatList      = NAS_MML_GetMsPrioRatList();

    pstSearchRatInfo    = NAS_MMC_GetRatCoverage_PlmnSelection();

    ulRatNum            = pstPrioRatList->ucRatNum;

    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();

    if (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING == NAS_MMC_GetSpecPlmnSearchState())
    {
        ulCampFlg = VOS_FALSE;
    }
    else
    {
        ulCampFlg = VOS_TRUE;
    }

    stPlmnId.ulMcc     = NAS_MML_INVALID_MCC;
    stPlmnId.ulMnc     = NAS_MML_INVALID_MNC;
    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();

    /* ����ǻ�ȡ������������ͻ�ȡ����ʧ�ܻظ���Ϣ */
    if (NAS_BuildEventType(WUEPS_PID_MMA, ID_MMA_MMC_ACQ_REQ) == ulCurrentEventType)
    {
        NAS_MMC_SndMmaAcqCnf(MMC_MMA_ACQ_RESULT_FAILURE, &stPlmnId, 0);
    }


    /* �����ֶ������쳣����ֱ�ӻظ�����ʧ�� */
    if (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId, ulCurrentEventType))
    {
        NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_FAIL);

        /*���������ϢΪ��ͨ���������󣬺������µ��û�����ʱ�����Դ�ϸ�״̬��*/
        NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection();

    #if (FEATURE_ON == FEATURE_LTE)
        if ( VOS_TRUE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_LTE))
        {
            NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
        }
    #endif

        NAS_MMC_SndRrMmCellSelReq(RRC_NAS_FORBLA_CHANGE);    
    }

    /* �����ע�����������ע��ظ���Ϣ */
    if (VOS_TRUE == NAS_MMC_IsMmaRegReq_PlmnSelection())
    {
        if (VOS_TRUE == NAS_MMC_IsNormalServiceStatus())
        {
            NAS_MMC_SndMmaRegCnf(MMC_MMA_REG_RESULT_SUCCESS, NAS_MML_GetCurrCampPlmnId(),
                                 NAS_MML_GetCurrCampArfcn());
        }
        else
        {
            if (VOS_TRUE == ulCampFlg)
            {
                NAS_MMC_SndMmaRegCnf(MMC_MMA_REG_RESULT_REG_FAILURE, NAS_MML_GetCurrCampPlmnId(),
                                 NAS_MML_GetCurrCampArfcn());
            }
            else
            {
                NAS_MMC_SndMmaRegCnf(MMC_MMA_REG_RESULT_ACQ_FAILURE, &stPlmnId, 0x0);
            }
        }
    }

    /* ��ȡ��ǰ���ڸ��ǵ�����ģʽ */
    for (i = 0; i < NAS_MML_MAX_RAT_NUM; i++)
    {
        /* ���й�ȫƵ�����ҵ�ǰû�и��� */
        if ((VOS_TRUE == NAS_MML_IsNetRatSupported(pstSearchRatInfo[i].enRatType))
         && (VOS_TRUE == pstSearchRatInfo[i].ucSearchAllBand)
         && (NAS_MMC_COVERAGE_TYPE_NONE == pstSearchRatInfo[i].enCoverageType))
        {
            ulRatNum--;
        }


        /* �����쳣���� */
        if ( 0 == ulRatNum )
        {
            break;
        }
    }


    NAS_MMC_SndPlmnSelectionRslt(enPlmnSelectionRslt, ulCampFlg, pstSearchRatInfo, ulRatNum);

    return;
}

#if   (FEATURE_ON == FEATURE_LTE)
VOS_VOID  NAS_MMC_ProcLmmCombinedAttachInd_AttRsltSucc_PlmnSelection(
    LMM_MMC_ATTACH_IND_STRU                                *pstLmmAttachIndMsg,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegRsltCause,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{
    if (VOS_TRUE == pstLmmAttachIndMsg->bitOpCnRslt)
    {
        if (MMC_LMM_ATT_CN_RSLT_COMBINED_EPS_IMSI == pstLmmAttachIndMsg->ulCnRslt)
        {
            NAS_MMC_ProcLmmCombinedAttachInd_AttRsltCsEpsSucc(pstLmmAttachIndMsg,
                          penCsAddition, penPsAddition);
        }
        else if (MMC_LMM_ATT_CN_RSLT_EPS_ONLY == pstLmmAttachIndMsg->ulCnRslt)
        {
            NAS_MMC_ProcLmmCombinedAttachInd_AttRsltEpsOnlySucc(pstLmmAttachIndMsg,enCsRegRsltCause,
                          penCsAddition, penPsAddition);
        }
        else
        {
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcLmmCombinedAttachInd_AttRsltSucc_PlmnSelection:Unexpected cn result!");
        }
    }
    else
    {
        /* ���õ�ǰע��״̬Ϊ�ɹ� */
        NAS_MMC_SetUserSpecPlmnRegisterStatus(VOS_TRUE);

        NAS_MMC_SetCsServiceStatus(NAS_MMC_NORMAL_SERVICE);
        /* ������ע��ɹ��ϱ��������񷵻� */
        *penPsAddition = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

        NAS_MMC_SetPsServiceStatus(NAS_MMC_NORMAL_SERVICE);
        NAS_MMC_SetCsServiceStatus(NAS_MMC_NORMAL_SERVICE);

        *penCsAddition = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE);
        
        

        NAS_MMC_UpdateCsRegStateCsRegSucc();

        NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_CS_PS, VOS_TRUE, NAS_MML_REG_FAIL_CAUSE_NULL);
    }


    NAS_MMC_NotifyModeChange(NAS_MML_GetCurrNetRatType(),
                             NAS_MML_GetSimPsRegStatus());


    return;
}
VOS_VOID  NAS_MMC_ProcLmmCombinedAttachInd_PlmnSelection(
    LMM_MMC_ATTACH_IND_STRU            *pstLmmAttachIndMsg
)
{
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enPsRegRsltCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegRsltCause;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;

    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU                               stSndEquPlmnInfo;

    enPsAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;
    enCsAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;

    NAS_MMC_ConverLmmCombinedAttachRsltToMMLCause(pstLmmAttachIndMsg,&enPsRegRsltCause, &enCsRegRsltCause);

    switch (pstLmmAttachIndMsg->ulAttachRslt)
    {
        case MMC_LMM_ATT_RSLT_SUCCESS:
            NAS_MMC_ProcLmmCombinedAttachInd_AttRsltSucc_PlmnSelection(pstLmmAttachIndMsg,
                       enCsRegRsltCause, &enCsAdditionalAction, &enPsAdditionalAction);

            break;

        case MMC_LMM_ATT_RSLT_FAILURE:
            if (VOS_FALSE == pstLmmAttachIndMsg->bitOpAtmpCnt)
            {
                enPsAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
                enCsAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
                NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);
                NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

                NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

                NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_CS_PS, VOS_FALSE, NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE);
            }
            else
            {
                NAS_MMC_ProcCombinedAttachEpsRegFail(LMM_MMC_ATTACH_IND, pstLmmAttachIndMsg,
                                      &enPsAdditionalAction, &enCsAdditionalAction);
            }
            break;

        case MMC_LMM_ATT_RSLT_ESM_FAILURE:
        case MMC_LMM_ATT_RSLT_MO_DETACH_FAILURE:
        case MMC_LMM_ATT_RSLT_MT_DETACH_FAILURE:
        case MMC_LMM_ATT_RSLT_T3402_RUNNING:
        case MMC_LMM_ATT_RSLT_TIMER_EXP:
        case MMC_LMM_ATT_RSLT_ACCESS_BAR:
        case MMC_LMM_ATT_RSLT_FORBID_PLMN:
        case MMC_LMM_ATT_RSLT_FORBID_TA_FOR_RPOS:
        case MMC_LMM_ATT_RSLT_FORBID_PLMN_FOR_GPRS:
        case MMC_LMM_ATT_RSLT_FORBID_TA_FOR_ROAMING:
        case MMC_LMM_ATT_RSLT_AUTH_REJ:
        case MMC_LMM_ATT_RSLT_PS_ATT_NOT_ALLOW:
            NAS_MMC_ProcCombinedAttachEpsRegFail(LMM_MMC_ATTACH_IND, pstLmmAttachIndMsg,
                                  &enPsAdditionalAction, &enCsAdditionalAction);
            break;

        case MMC_LMM_ATT_RSLT_CN_REJ:
            if ((VOS_TRUE == pstLmmAttachIndMsg->bitOpCnRslt)
             && (MMC_LMM_ATT_CN_RSLT_EPS_ONLY == pstLmmAttachIndMsg->ulCnRslt))
            {
                /* ��ATTACH��������Ϊ���ϣ�������Ӧ��ATTACH�������ΪEPS ONLY��
                   ��ԭ��ֵ��ΪЭ��24301 5.5.1.3.4.3�½����л���û��Я��ԭ��ֵ��
                   ��LMMͨ��ATTACH�����MMC_LMM_ATT_RSLT_CN_REJ֪ͨMMC��
                   ��Я����Ӧ��ATTACH�������͡�������Ӧ��ATTCH������͡�ԭ��ֵ��
                   ��Я�������ԭ����attach attempt counterֵ */
                NAS_MMC_ProcLmmCombinedAttachInd_AttRsltEpsOnlySucc(pstLmmAttachIndMsg,enCsRegRsltCause,
                                  &enCsAdditionalAction, &enPsAdditionalAction);

            }
            else
            {
                NAS_MMC_ProcCombinedAttachEpsRegFail(LMM_MMC_ATTACH_IND, pstLmmAttachIndMsg,
                                      &enPsAdditionalAction, &enCsAdditionalAction);
            }
            break;

        default:
            /* �쳣��ӡ */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcLmmCombinedAttachInd_PlmnSelection:Unexpected attach result!");
            enPsAdditionalAction = NAS_MMC_GetPsRegAdditionalAction_PlmnSelection();
            enCsAdditionalAction = NAS_MMC_GetCsRegAdditionalAction_PlmnSelection();
            break;
    }

    /* AdditionalAction��Ч������EPS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enPsAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(enPsAdditionalAction);
    }

    /* AdditionalAction��Ч������CS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enCsAdditionalAction )
    {
        NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(enCsAdditionalAction);
    }

    /* �������Ҫ�ȴ���һ��ע����,�򲻸���ע��ԭ��ֵ,�ȴ�conut�ﵽ�������,
       �Ա����û�ָ�������ظ���� */
    if (NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT != enPsAdditionalAction)
    {
        NAS_MMC_SetPsRegCause_PlmnSelection(enPsRegRsltCause);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn_PlmnSelection())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndLmmEquPlmnReq(&stSndEquPlmnInfo);

        NAS_MMC_SndOmEquPlmn();
    }


    NAS_MMC_UpdateUserSpecPlmnRegisterStatus(enPsRegRsltCause);

    return;

}
VOS_VOID  NAS_MMC_ProcLmmMmcEpsOnlyAttachInd_PlmnSelection(
    LMM_MMC_ATTACH_IND_STRU            *pstLmmAttachIndMsg
)
{
    VOS_UINT32                          ulEquPlmnNum;
    MMC_LMM_PLMN_ID_STRU               *pstLmmEquPlmnList = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU           stLEplmnInfo;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enRegRsltCause;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAdditionalAction;

    NAS_MML_EQUPLMN_INFO_STRU          *pstEquPlmnInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU           stSndEquPlmnInfo;

    VOS_UINT8                           ucSimCsRegStatus;

    NAS_MMC_ConverLmmAttachRsltToMMLCause(pstLmmAttachIndMsg, &enRegRsltCause);

    if (MMC_LMM_ATT_RSLT_SUCCESS == pstLmmAttachIndMsg->ulAttachRslt)
    {
        enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

        NAS_MMC_SaveRegRsltCtx(LMM_MMC_ATTACH_IND, pstLmmAttachIndMsg);

        /* ����ע��������ע����Ϣ�� */
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_EPS, NAS_MML_REG_FAIL_CAUSE_NULL);

        NAS_MMC_SetPsServiceStatus(NAS_MMC_NORMAL_SERVICE);

        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE);

        /* EPS����ע��ɹ�ʱ����Ҫɾ��ForbPlmn,ForbGprs����Ϣ */
        NAS_MMC_DelForbInfo_LmmAttRsltSucc(VOS_NULL_PTR);

        /* ���CS����ע�����CS����Ч�����CSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
        ucSimCsRegStatus = NAS_MML_GetSimCsRegStatus();
        if ((VOS_TRUE == NAS_MML_GetCsAttachAllowFlg())
         && (VOS_TRUE == ucSimCsRegStatus))
        {
            NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        /* ����EPLMN */
        if ( VOS_TRUE == pstLmmAttachIndMsg->bitOpEplmn)
        {
            ulEquPlmnNum        = pstLmmAttachIndMsg->stEplmnList.ulPlmnNum;
            pstLmmEquPlmnList   = pstLmmAttachIndMsg->stEplmnList.astEplmnList;

            /* ��LMM��PLMN ID��ʽת��ΪGU�ĸ�ʽ */
            NAS_MMC_ConvertLmmPlmnToGUNasPlmn(ulEquPlmnNum, pstLmmEquPlmnList, (stLEplmnInfo.astEquPlmnAddr));
            NAS_MML_SaveEquPlmnList(ulEquPlmnNum, stLEplmnInfo.astEquPlmnAddr);

            NAS_MMC_WriteEplmnNvim();
        }

        NAS_MMC_UpdateGURegRlstRPlmnIdInNV();


        NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_PS, VOS_TRUE, NAS_MML_REG_FAIL_CAUSE_NULL);

        NAS_MMC_SetUserSpecPlmnRegisterStatus(VOS_TRUE);

        NAS_MMC_NotifyModeChange(NAS_MML_GetCurrNetRatType(),
                                  NAS_MML_GetSimPsRegStatus());
    }
    else if ((MMC_LMM_ATT_RSLT_FAILURE == pstLmmAttachIndMsg->ulAttachRslt)
          && (VOS_FALSE == pstLmmAttachIndMsg->bitOpAtmpCnt))
    {
        enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
        NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

        NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_PS, VOS_FALSE, NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE);
    }
    else
    {
        enAdditionalAction= NAS_MMC_ProcEpsAttachRegFail(LMM_MMC_ATTACH_IND, pstLmmAttachIndMsg);

        NAS_MMC_UpdateUserSpecPlmnRegisterStatusWhenRegFail(enRegRsltCause);
    }

    /* AdditionalAction��Ч������EPS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(enAdditionalAction);
    }

    /* �������Ҫ�ȴ���һ��ע����,�򲻸���ע��ԭ��ֵ,�ȴ�conut�ﵽ�������,
       �Ա����û�ָ�������ظ���� */
    if (NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT != enAdditionalAction)
    {
        NAS_MMC_SetPsRegCause_PlmnSelection(enRegRsltCause);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn_PlmnSelection())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndLmmEquPlmnReq(&stSndEquPlmnInfo);

        NAS_MMC_SndOmEquPlmn();
    }

    return ;

}



VOS_VOID  NAS_MMC_ProcLmmAttachInd_PlmnSelection(
    LMM_MMC_ATTACH_IND_STRU            *pstLmmAttachIndMsg
)
{
    switch (pstLmmAttachIndMsg->ulReqType)
    {
        case MMC_LMM_ATT_TYPE_COMBINED_EPS_IMSI:

            /* ����lmm������attach��� */
            NAS_MMC_ProcLmmCombinedAttachInd_PlmnSelection(pstLmmAttachIndMsg);
            break;

        case MMC_LMM_ATT_TYPE_EPS_ONLY:

            /* ����lmm�ķ�����attach��EPS onlyע��Ľ�� */
            NAS_MMC_ProcLmmMmcEpsOnlyAttachInd_PlmnSelection(pstLmmAttachIndMsg);
            break;

        case MMC_LMM_ATT_TYPE_EPS_EMERGENCY:

            /* �ݲ�֧��δʵ�� */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcLmmAttachInd_PlmnSelection:Unsupported attach req type!");
            break;

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcLmmAttachInd_PlmnSelection:Unexpected attach req type!");
            break;

    }
    return;
}
VOS_VOID  NAS_MMC_ProcLmmEpsOnlyTauResultInd_PlmnSelection(
    LMM_MMC_TAU_RESULT_IND_STRU        *pstLmmTauIndMsg
)
{
    VOS_UINT32                          ulEquPlmnNum;
    MMC_LMM_PLMN_ID_STRU               *pstLmmEquPlmnList = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU           stLEplmnAddr;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enRegRsltCause;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAdditionalAction;

    NAS_MML_EQUPLMN_INFO_STRU          *pstEquPlmnInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU           stSndEquPlmnInfo;

    VOS_UINT8                           ucSimCsRegStatus;

    NAS_MMC_ConverTauResultToMMLCause(pstLmmTauIndMsg, &enRegRsltCause);

    if (MMC_LMM_TAU_RSLT_SUCCESS == pstLmmTauIndMsg->ulTauRst)
    {
        enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

        /* ����ע������Ϣ */
        NAS_MMC_SaveRegRsltCtx(LMM_MMC_TAU_RESULT_IND, pstLmmTauIndMsg);

        /* ����ע��������ע����Ϣ�� */
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_EPS, NAS_MML_REG_FAIL_CAUSE_NULL);

        NAS_MMC_SetPsServiceStatus(NAS_MMC_NORMAL_SERVICE);

        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE);

        /* EPS����ע��ɹ�ʱ����Ҫɾ��ForbPlmn,ForbGprs����Ϣ */
        NAS_MMC_DelForbInfo_LmmAttRsltSucc(VOS_NULL_PTR);

        /* �˴�����CS additionԭ��������TAU��EPS�ɹ�ʱ,LMM����T3411�ٴγ���TAU������,
           �û�Detach CS,LMM��ֱ�ӱ�TAU�ɹ�,���ٳ�������TAU */
        NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON);

        /* ���CS����ע�����CS����Ч�����CSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
        ucSimCsRegStatus = NAS_MML_GetSimCsRegStatus();
        if ((VOS_TRUE == NAS_MML_GetCsAttachAllowFlg())
         && (VOS_TRUE == ucSimCsRegStatus))
        {
            NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        

        /* ����EPLMN */
        if ( VOS_TRUE == pstLmmTauIndMsg->bitOpEplmn)
        {
            ulEquPlmnNum        = pstLmmTauIndMsg->stEplmnList.ulPlmnNum;
            pstLmmEquPlmnList   = pstLmmTauIndMsg->stEplmnList.astEplmnList;

            /* ��LMM��PLMN ID��ʽת��ΪMMC�ĸ�ʽ */
            NAS_MMC_ConvertLmmPlmnToGUNasPlmn(ulEquPlmnNum, pstLmmEquPlmnList, (stLEplmnAddr.astEquPlmnAddr));

            NAS_MML_SaveEquPlmnList(ulEquPlmnNum, stLEplmnAddr.astEquPlmnAddr);

            NAS_MMC_WriteEplmnNvim();
        }

        NAS_MMC_UpdateGURegRlstRPlmnIdInNV();
        
        NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_PS, VOS_TRUE, NAS_MML_REG_FAIL_CAUSE_NULL);

        NAS_MMC_SetUserSpecPlmnRegisterStatus(VOS_TRUE);

        NAS_MMC_NotifyModeChange(NAS_MML_GetCurrNetRatType(),
                                  NAS_MML_GetSimPsRegStatus());
    }
    else if ((MMC_LMM_TAU_RSLT_FAILURE == pstLmmTauIndMsg->ulTauRst)
          && (VOS_FALSE == pstLmmTauIndMsg->bitOpAtmpCnt))
    {
        enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
        NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

        NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_PS, VOS_FALSE, NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE);
    }
    else
    {
        enAdditionalAction = NAS_MMC_ProcEpsTauRegFail(LMM_MMC_TAU_RESULT_IND, pstLmmTauIndMsg);

        NAS_MMC_UpdateUserSpecPlmnRegisterStatusWhenRegFail(enRegRsltCause);
    }

    /* �������Ҫ�ȴ���һ��ע����,�򲻸���ע��ԭ��ֵ,�ȴ�conut�ﵽ�������,
       �Ա����û�ָ�������ظ���� */
    if (NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT != enAdditionalAction)
    {
        NAS_MMC_SetPsRegCause_PlmnSelection(enRegRsltCause);
    }

    /* AdditionalAction��Ч������EPS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(enAdditionalAction);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn_PlmnSelection())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndLmmEquPlmnReq(&stSndEquPlmnInfo);

        NAS_MMC_SndOmEquPlmn();
    }

    return ;

}



VOS_VOID NAS_MMC_ProcLmmCombinedTauResultInd_TauRsltSucc_PlmnSelection(
    LMM_MMC_TAU_RESULT_IND_STRU                            *pstLmmTauIndMsg,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegRsltCause,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{
    if (VOS_TRUE == pstLmmTauIndMsg->bitOpCnRst)
    {
        /* CS+PS��������TAUʱ,�����Ƿ�Я��CS CAUSEֵ,����Ϊ����ע��ɹ� */
        if ((MMC_LMM_COMBINED_TA_LA_UPDATED == pstLmmTauIndMsg->ulCnRst)
         || (MMC_LMM_COMBINED_TA_LA_UPDATED_ISR_ACTIVATED == pstLmmTauIndMsg->ulCnRst)
         || (MMC_LMM_CS_PS_PERIODIC_UPDATING == pstLmmTauIndMsg->ulReqType))
        {
            /* �����ཻ��EPS��CS��ע��ɹ� */
            NAS_MMC_ProcLmmCombinedTauResultInd_TauRsltCsEpsSucc(pstLmmTauIndMsg,
                          penCsAddition, penPsAddition);

        }
        else if ((MMC_LMM_TA_UPDATED == pstLmmTauIndMsg->ulCnRst)
              || (MMC_LMM_TA_UPDATED_ISR_ACTIVATED == pstLmmTauIndMsg->ulCnRst))
        {
            /* �����ཻ��EPS�ɹ�CSʧ�ܣ���ԭ��ֵΪЭ��24301 5.5.3.3.4.3�½����� */
            NAS_MMC_ProcLmmCombinedTauResultInd_TauRsltEpsOnlySucc(pstLmmTauIndMsg,enCsRegRsltCause,
                          penCsAddition, penPsAddition);

        }
        else
        {
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcLmmCombinedTauResultInd_TauRsltSucc_PlmnSelection:Unexpected cn result!");
        }
    }
    else
    {
        /* ������ע��ɹ��ϱ��������񷵻� */
        *penPsAddition = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;
        NAS_MMC_SetPsServiceStatus(NAS_MMC_NORMAL_SERVICE);

        NAS_MMC_SetCsServiceStatus(NAS_MMC_NORMAL_SERVICE);

        *penCsAddition = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE);
        


        NAS_MMC_UpdateCsRegStateCsRegSucc();

        NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_CS_PS, VOS_TRUE, NAS_MML_REG_FAIL_CAUSE_NULL);
    }

    NAS_MMC_NotifyModeChange(NAS_MML_GetCurrNetRatType(),
                             NAS_MML_GetSimPsRegStatus());


    return;
}
VOS_VOID  NAS_MMC_ProcLmmCombinedTauResultInd_PlmnSelection(
    LMM_MMC_TAU_RESULT_IND_STRU        *pstLmmTauIndMsg
)
{
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enPsRegRsltCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegRsltCause;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;

    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnInfo = VOS_NULL_PTR;
     NAS_MML_EQUPLMN_INFO_STRU                              stSndEquPlmnInfo;


    enPsAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;
    enCsAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;

    NAS_MMC_ConverLmmCombinedTauRsltToMMLCause(pstLmmTauIndMsg, &enPsRegRsltCause, &enCsRegRsltCause);

    switch (pstLmmTauIndMsg->ulTauRst)
    {
        case MMC_LMM_TAU_RSLT_SUCCESS:
            NAS_MMC_ProcLmmCombinedTauResultInd_TauRsltSucc_PlmnSelection(pstLmmTauIndMsg,
                       enCsRegRsltCause, &enCsAdditionalAction, &enPsAdditionalAction);

            break;

        case MMC_LMM_TAU_RSLT_FAILURE:
            if (VOS_FALSE == pstLmmTauIndMsg->bitOpAtmpCnt)
            {
                enPsAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
                enCsAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
                NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);
                NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

                NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

                NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_CS_PS, VOS_FALSE, NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE);
            }
            else
            {
                /* EPS��CSע���ʧ�� */
                NAS_MMC_ProcCombinedTauEpsRegFail(LMM_MMC_TAU_RESULT_IND, pstLmmTauIndMsg,
                                              &enPsAdditionalAction, &enCsAdditionalAction);
            }
            break;

        case MMC_LMM_TAU_RSLT_MO_DETACH_FAILURE:
        case MMC_LMM_TAU_RSLT_MT_DETACH_FAILURE:
        case MMC_LMM_TAU_RSLT_T3402_RUNNING:
        case MMC_LMM_TAU_RSLT_TIMER_EXP:
        case MMC_LMM_TAU_RSLT_ACCESS_BARED:
        case MMC_LMM_TAU_RSLT_FORBID_PLMN:
        case MMC_LMM_TAU_RSLT_FORBID_TA_FOR_RPOS:
        case MMC_LMM_TAU_RSLT_FORBID_PLMN_FOR_GPRS:
        case MMC_LMM_TAU_RSLT_FORBID_TA_FOR_ROAMING:
        case MMC_LMM_TAU_RSLT_AUTH_REJ:
            /* EPS��CSע���ʧ�� */
            NAS_MMC_ProcCombinedTauEpsRegFail(LMM_MMC_TAU_RESULT_IND, pstLmmTauIndMsg,
                                          &enPsAdditionalAction, &enCsAdditionalAction);
            break;

        case MMC_LMM_TAU_RSLT_CN_REJ:
            if ((VOS_TRUE == pstLmmTauIndMsg->bitOpCnRst)
             && ((MMC_LMM_TA_UPDATED == pstLmmTauIndMsg->ulCnRst)
              || (MMC_LMM_TA_UPDATED_ISR_ACTIVATED == pstLmmTauIndMsg->ulCnRst)))
            {
                /* ��TAU��������Ϊ���ϣ�������Ӧ��TAU�������ΪTA UPDATED��
                   ��ԭ��ֵ��ΪЭ��24301 5.5.3.3.4.3�½����л���û��Я��ԭ��ֵ��
                   ��LMMͨ��TAU�����MMC_LMM_TAU_RSLT_CN_REJ֪ͨMMC����Я����Ӧ
                   ��TAU�������͡�������Ӧ��TAU������͡�ԭ��ֵ����Я�������ԭ
                   ����tau attempt counterֵ */
                NAS_MMC_ProcLmmCombinedTauResultInd_TauRsltEpsOnlySucc(pstLmmTauIndMsg,enCsRegRsltCause,
                                  &enCsAdditionalAction, &enPsAdditionalAction);
            }
            else
            {
                /* EPS��CSע���ʧ�� */
                NAS_MMC_ProcCombinedTauEpsRegFail(LMM_MMC_TAU_RESULT_IND, pstLmmTauIndMsg,
                                              &enPsAdditionalAction, &enCsAdditionalAction);

            }
            break;

        default:
            /* �쳣��ӡ */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcLmmCombinedTauResultInd_PlmnSelection:Unexpected attach result!");
            enPsAdditionalAction = NAS_MMC_GetPsRegAdditionalAction_PlmnSelection();
            enCsAdditionalAction = NAS_MMC_GetCsRegAdditionalAction_PlmnSelection();
            break;

    }

    /* AdditionalAction��Ч������EPS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enPsAdditionalAction )
    {
        NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(enPsAdditionalAction);
    }

    /* AdditionalAction��Ч������CS����һ���Ķ������� */
    if ( NAS_MMC_ADDITIONAL_ACTION_BUTT != enCsAdditionalAction )
    {
        NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(enCsAdditionalAction);
    }

     /* �������Ҫ�ȴ���һ��ע����,�򲻸���ע��ԭ��ֵ,�ȴ�conut�ﵽ�������,
    �Ա����û�ָ�������ظ���� */
    if (NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT != enPsAdditionalAction)
    {
        NAS_MMC_SetPsRegCause_PlmnSelection(enPsRegRsltCause);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn_PlmnSelection())
    {
        pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
        PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(stSndEquPlmnInfo));
        NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
        NAS_MMC_SndLmmEquPlmnReq(&stSndEquPlmnInfo);

        NAS_MMC_SndOmEquPlmn();
    }


    NAS_MMC_UpdateUserSpecPlmnRegisterStatus(enPsRegRsltCause);

    return;
}


VOS_VOID  NAS_MMC_ProcLmmTauResultInd_PlmnSelection(
    LMM_MMC_TAU_RESULT_IND_STRU        *pstLmmTauIndMsg
)
{
    switch (pstLmmTauIndMsg->ulReqType)
    {
        case MMC_LMM_COMBINED_TA_LA_UPDATING:
        case MMC_LMM_COMBINED_TA_LA_WITH_IMSI:
        case MMC_LMM_CS_PS_PERIODIC_UPDATING:

            /* ����lmm������tau��� */
            NAS_MMC_ProcLmmCombinedTauResultInd_PlmnSelection(pstLmmTauIndMsg);
            break;

        case MMC_LMM_TA_UPDATING:
        case MMC_LMM_PS_PERIODIC_UPDATING:

            /* ����lmm�ķ�����tau��EPS onlyע��Ľ�� */
            NAS_MMC_ProcLmmEpsOnlyTauResultInd_PlmnSelection(pstLmmTauIndMsg);
            break;

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcLmmTauResultInd_PlmnSelection:Unexpected tau req type!");
            break;

    }

    return;
}



VOS_UINT32 NAS_MMC_RcvLmmMmcTauResultInd_PlmnSelection_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsRegCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsRegCause;
    LMM_MMC_TAU_RESULT_IND_STRU        *pstLmmTauIndMsg = VOS_NULL_PTR;
    NAS_MMC_FSM_ID_ENUM_UINT32          enCurrentFsmId;
    VOS_UINT32                          ulCurrentEventType;

    pstLmmTauIndMsg  = (LMM_MMC_TAU_RESULT_IND_STRU*)pstMsg;

    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* ��GMM��MMת��ע������Ϣ */
    NAS_MMC_SndMmLmmTauResultInd(pstLmmTauIndMsg);
    NAS_MMC_SndGmmLmmTauResultInd(pstLmmTauIndMsg);

    /* ����ע������� ,�û�����syscfg��Ҫ����������MMC�������ͷ����ӣ�
       �ڵȴ������ͷ�״̬��L���ܻ���ע��������Ҫ���� */
    NAS_MMC_ProcLmmTauResultInd_PlmnSelection(pstLmmTauIndMsg);

    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();
    if (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType))
    {
        enCsRegCause = NAS_MMC_GetCsRegCause_PlmnSelection();
        enPsRegCause = NAS_MMC_GetPsRegCause_PlmnSelection();

        if (VOS_TRUE == NAS_MMC_ProcUserSearchRegRslt_PlmnSelection(enCsRegCause,enPsRegCause))
        {
            /*���������ϢΪ��ͨ���������󣬺������µ��û�����ʱ�����Դ�ϸ�״̬��*/
            NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection();
        }
    }

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvLmmMmcAttachInd_PlmnSelection_WaitEpsConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enPsRegCause;
    LMM_MMC_ATTACH_IND_STRU                                *pstLmmAttachIndMsg = VOS_NULL_PTR;
    NAS_MMC_FSM_ID_ENUM_UINT32                              enCurrentFsmId;
    VOS_UINT32                                              ulCurrentEventType;
    NAS_MML_LTE_UE_OPERATION_MODE_ENUM_UINT8                enUeOperationMode;

    enUeOperationMode   =  NAS_MML_GetLteUeOperationMode();
    pstLmmAttachIndMsg  = (LMM_MMC_ATTACH_IND_STRU*)pstMsg;

    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_PlmnSelection(NAS_MMC_WAIT_REG_RESULT_IND_PS);

    /* LMM������δ�ظ�search cnfʱ�յ��û�detach ps����󣬺���������ע���mmc�ظ�attach result
       ΪMMC_LMM_ATT_RSLT_PS_ATT_NOT_ALLOW���������������MMC_LMM_ATT_TYPE_BUTT��
       MMC��Ҫ���ݵ�ǰue operation mode�������������¸�ֵ */
    if ((MMC_LMM_ATT_TYPE_BUTT == pstLmmAttachIndMsg->ulReqType)
     && (MMC_LMM_ATT_RSLT_PS_ATT_NOT_ALLOW == pstLmmAttachIndMsg->ulAttachRslt))
    {
        pstLmmAttachIndMsg->ulReqType = NAS_MMC_ConvertLmmAttachReqType(enUeOperationMode);
    }

    /* ��GMM��MMת��ע������Ϣ */
    NAS_MMC_SndMmLmmAttachInd(pstLmmAttachIndMsg);
    NAS_MMC_SndGmmLmmAttachInd(pstLmmAttachIndMsg);

    /* ����ע������� ,�û�����syscfg��Ҫ����������MMC�������ͷ����ӣ�
      �ڵȴ������ͷ�״̬��L���ܻ���ע��������Ҫ���� */
    NAS_MMC_ProcLmmAttachInd_PlmnSelection(pstLmmAttachIndMsg);

    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
    if (NAS_MMC_FSM_PLMN_SELECTION != enCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();
    if (VOS_TRUE == NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId,ulCurrentEventType))
    {
        enCsRegCause = NAS_MMC_GetCsRegCause_PlmnSelection();
        enPsRegCause = NAS_MMC_GetPsRegCause_PlmnSelection();

        if (VOS_TRUE == NAS_MMC_ProcUserSearchRegRslt_PlmnSelection(enCsRegCause,enPsRegCause))
        {
            /*���������ϢΪ��ͨ���������󣬺������µ��û�����ʱ�����Դ�ϸ�״̬��*/
            NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection();
        }
    }

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_IsNeedEnableLte_PlmnSelection(
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene
)
{
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;

    enLteCapabilityStatus = NAS_MML_GetLteCapabilityStatus();

    /* �������ʱ�Ѿ�����en_NV_Item_Rat_Forbidden_List_Accord_Imsi_Config�����ý�ֹ��LTE���򷵻�VOS_FALSE */
    if (NAS_MML_RAT_CAPABILITY_STATUS_DISABLE == NAS_MML_GetLteForbiddenStatusFlg())
    {
        return VOS_FALSE;
    }

    /* �����SIM��������ҪEnable Lte */
    if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
    {
        return VOS_FALSE;
    }

    /* PS����Ч, ����Ҫ���� Enable L */
    if ( VOS_FALSE == NAS_MML_GetSimPsRegStatus() )
    {
        return VOS_FALSE;
    }

    /* PS��������ע��,����Ҫ���� Enable L */
    if ( VOS_FALSE == NAS_MML_GetPsAttachAllowFlg() )
    {
        return VOS_FALSE;
    }

    /* ����������NAS_MMC_PLMN_SEARCH_SCENE_DISABLE_LTE,����Ҫ���� Enable L */
    if ( NAS_MMC_PLMN_SEARCH_SCENE_DISABLE_LTE == enPlmnSearchScene )
    {
        return VOS_FALSE;
    }

    /* disable ��enable lte�����ж�syscfg�Ƿ�֧��L */

    /* �Ѿ�Disable L,��Ҫ���� Enable L */
    if ( (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLteCapabilityStatus)
      || (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == enLteCapabilityStatus) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_IsDisableLteNeedWaitEpsConnRel_PlmnSelection(VOS_VOID)
{
    NAS_MML_PLMN_WITH_RAT_STRU                              stDestPlmn;
	
	/* ����汾Coverity���� */
	PS_MEM_SET(&stDestPlmn, 0, sizeof(NAS_MML_PLMN_WITH_RAT_STRU));
	
    if (VOS_TRUE == NAS_MMC_IsDisableLteNeedLocalReleaseEpsConn())
    {
        /* ���һ������*/
        if (VOS_FALSE == NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
        {
            /* �ظ�״̬���˳��ɹ� */
            NAS_MMC_SndRslt_PlmnSelection(NAS_MMC_PLMN_SELECTION_FAIL);

            /* ֱ���˳���ǰ״̬�� */
            NAS_MMC_FSM_QuitFsmL2();

            return VOS_FALSE;
        }

        NAS_MMC_SetRelRequestFlag_PlmnSelection(VOS_TRUE);

        NAS_MMC_SndLmmRelReq();
    }

    return VOS_TRUE;
}

#endif


VOS_UINT32  NAS_MMC_RcvTafSpecPlmnSearchAbortReq_PlmnSelection(
     VOS_UINT32                         ulEventType,
     struct MsgCB                      *pstMsg
)
{
    /* ��������״̬�������ϢΪ��ͨ���� */
    NAS_MMC_SaveCommonPlmnSrchEntryMsg_PlmnSelection();

#if (FEATURE_ON == FEATURE_LTE)
    /* ֪֪ͨͨLMM��ǰ�ڷ��û�ָ������ģʽ */
    NAS_MMC_SndLmmUserSpecPlmnSearchEndNotify();
#endif

    /* �ظ�TAFMMC_SPEC_PLMN_SEARCH_ABORT_CNF��MMA */
    NAS_MMC_SndMmaSpecPlmnSearchAbortCnf();
    return VOS_TRUE;
}
VOS_UINT32  NAS_MMC_RcvRrmmLimitServiceCampInd_PlmnSelection_WaitCsPsRegRsltInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    return NAS_MMC_RcvRrmmLimitServiceCampInd_PlmnSelection(ulEventType, pstMsg);

}


VOS_UINT32  NAS_MMC_RcvRrmmLimitServiceCampInd_PlmnSelection_WaitRrcConnRelInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    return NAS_MMC_RcvRrmmLimitServiceCampInd_PlmnSelection(ulEventType, pstMsg);

}


VOS_UINT32  NAS_MMC_RcvRrmmLimitServiceCampInd_PlmnSelection(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_LIMIT_SERVICE_CAMP_IND_STRU        *pstRrmmLimitCampOnMsg = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU               stDestPlmn;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU    *pastIntraPlmnSrchInfo = VOS_NULL_PTR;
    VOS_UINT32                               ulIndex;

    pstRrmmLimitCampOnMsg = (RRMM_LIMIT_SERVICE_CAMP_IND_STRU *)pstMsg;

    pastIntraPlmnSrchInfo = (NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                                             sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU) * NAS_MML_MAX_RAT_NUM);
    /* �����ڴ�ʧ�� */
    if (VOS_NULL_PTR == pastIntraPlmnSrchInfo)
    {
        return VOS_TRUE;
    }

    NAS_MMC_InitSearchedPlmnListInfo(pastIntraPlmnSrchInfo);

    /* ת��W�µ������ʽ */
    NAS_MMC_UpdateSearchedPlmnListInfo(&(pstRrmmLimitCampOnMsg->stPlmnIdList), pastIntraPlmnSrchInfo);


    /* ɾ���б��н�ֹ��PLMN */
    NAS_MMC_DelForbPlmnFromAsSrchedPlmnList(pastIntraPlmnSrchInfo);

    /* �ֶ�����ģʽ��ɾ�����û�ָ����PLMN */
    NAS_MMC_DelNotUserSpecPlmnFromSrchedPlmnList(pastIntraPlmnSrchInfo);

    /* ��ά�ɲ���Ϣ���� */
    NAS_MMC_LogAsPlmnList(pastIntraPlmnSrchInfo);

    if (VOS_TRUE == NAS_MMC_IsExistAvailPlmnInSearchedPlmnList(pastIntraPlmnSrchInfo))
    {
        for (ulIndex = 0; ulIndex < NAS_MML_MAX_RAT_NUM; ulIndex++)
        {
            NAS_MMC_UpdatePlmnSearchList_PlmnSelection(&(pastIntraPlmnSrchInfo[ulIndex]));
        }

        /* ����������һ����Ҫ����������ʱ�������������������򣬼����ȴ� */
        if (VOS_TRUE == NAS_MMC_GetNextSearchPlmn_PlmnSelection(&stDestPlmn))
        {
            /* ��ǰӦ�ÿ϶�������������,�������������,���ô��� */
            if (VOS_TRUE == NAS_MML_IsRrcConnExist())
            {
                /* ���ݵ�ǰ��ͬ�Ľ��뼼��,�����ͷ�����,Ǩ�Ƶ���ͬ�ĵȴ������ͷŵ�L2״̬��������������ʱ�� */
                NAS_MMC_SndRelReq_PlmnSelection(NAS_MML_NET_RAT_TYPE_WCDMA);
            }
        }
    }

    PS_MEM_FREE(WUEPS_PID_MMC, pastIntraPlmnSrchInfo);

    return VOS_TRUE;
}


VOS_VOID NAS_MMC_UpdateOosAreaLostPlmnSearchInfo_PlmnSelection(
    NAS_MMC_INTER_PLMN_SEARCH_REQ_STRU                     *pstPlmnSearchReq,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MMC_PLMN_TYPE_ENUM_UINT8        enPlmnType;
    VOS_UINT32                          i;
    VOS_UINT32                          j;

    enPlmnType = NAS_MMC_PLMN_TYPE_AVAIL_PLMN;

    for (i = 0; i < NAS_MML_MAX_RAT_NUM; i++)
    {
        if (NAS_MML_NET_RAT_TYPE_BUTT == pstPlmnSearchReq->astInterPlmnSearchInfo[i].enRatType)
        {
            continue;
        }

        stPrioRatList.aucRatPrio[0] = pstPlmnSearchReq->astInterPlmnSearchInfo[i].enRatType;
        stPrioRatList.ucRatNum      = 1;

        for (j = 0; j < pstPlmnSearchReq->astInterPlmnSearchInfo[i].ulHighPlmnNum; j++)
        {
            NAS_MMC_InsertPlmnToPlmnSelectionListHead(&pstPlmnSearchReq->astInterPlmnSearchInfo[i].astHighPlmnList[j],
                                                      enPlmnType,
                                                      &stPrioRatList,
                                                      pstPlmnSelectionListInfo);
        }

        for (j = 0; j < pstPlmnSearchReq->astInterPlmnSearchInfo[i].ulLowPlmnNum; j++)
        {
            NAS_MMC_InsertPlmnToPlmnSelectionListHead(&pstPlmnSearchReq->astInterPlmnSearchInfo[i].astLowPlmnList[j].stPlmnId,
                                                      enPlmnType,
                                                      &stPrioRatList,
                                                      pstPlmnSelectionListInfo);
        }
    }

    /* ��ά�ɲ⣬���ѡ���б���Ϣ */
    NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionListInfo);

    return;
}

VOS_UINT32 NAS_MMC_RcvMmcAbortFsmMsg_PlmnSelection(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
     /* ��¼��Ҫ�˳���ǰ״̬����־ */
    NAS_MMC_SetAbortFlag_PlmnSelection(VOS_TRUE);

    return VOS_TRUE;
}


NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32 NAS_MMC_GetPlmnSearchScene_PlmnSelection(VOS_VOID)
{
    NAS_MMC_INTER_PLMN_SEARCH_REQ_STRU *pstInterPlmnSearchReq = VOS_NULL_PTR;
    NAS_MMC_ENTRY_MSG_STRU             *pstEntryMsg           = VOS_NULL_PTR;

    /* ��ȡ״̬���������Ϣ��Ϣ */
    pstEntryMsg  = NAS_MMC_GetCurrEntryMsg();

    if (VOS_TRUE == NAS_MMC_IsInterPlmnSearch_PlmnSelection())
    {
        pstInterPlmnSearchReq = (NAS_MMC_INTER_PLMN_SEARCH_REQ_STRU*)(pstEntryMsg->aucEntryMsgBuffer);
        return pstInterPlmnSearchReq->enPlmnSearchScene;
    }

    /* ��������Ϣ��Ϊ�ڲ�������Ϣ���򷵻���Ч���������� */
    return NAS_MMC_PLMN_SEARCH_SCENE_BUTT;
}


VOS_UINT32  NAS_MMC_IsInterPlmnSearch_PlmnSelection(VOS_VOID)
{
    NAS_MMC_ENTRY_MSG_STRU             *pstEntryMsg    = VOS_NULL_PTR;

   /* ����FSM ID��ȡ��FSM �����Ϣ */
    pstEntryMsg     = NAS_MMC_GetCurrFsmMsgAddr();

    if (MMCMMC_INTER_PLMN_SEARCH_REQ == NAS_ExtractMsgNameFromEvtType(pstEntryMsg->ulEventType))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}
VOS_UINT32 NAS_MMC_IsNeedSndEplmn_PlmnSelection( VOS_VOID )
{
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEplmnInfo  = VOS_NULL_PTR;
    NAS_MML_DISABLED_RAT_PLMN_CFG_INFO_STRU                *pstDisabledRatPlmnCfg = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                                   *pstCurPlmnId = VOS_NULL_PTR;
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU                       *pstForbidPlmnInfo = VOS_NULL_PTR;
    NAS_MMC_FSM_ID_ENUM_UINT32                              enCurrentFsmId;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enCurRat;
    VOS_UINT32                                              ulCurrentEventType;
    VOS_UINT32                                              ulIsPlmnForbid;
    VOS_UINT32                                              ulIsUserSpecSrch;

    ulIsPlmnForbid  = VOS_FALSE;
    
    /* �޿�ʱ�����뷢�͵�Ч Plmn */
    if (VOS_FALSE == NAS_MML_GetSimPresentStatus())
    {
        return VOS_FALSE;
    }

    /*  ��ǰפ�����磬��Rplmnʱ�������� Eplmn������㣬�������㶪�� */
    pstEplmnInfo = NAS_MML_GetEquPlmnList();

    /* ��ǰ��EPlmn��֪ͨ�����,����RAU��������ATTACH,LAUʱ����Ҫɾ��Eplmn��24008:4.7.3.2.5
    if the attach attempt counter is smaller than 5 and, additionally,
    the update status is different from U1 UPDATED or the stored LAI is different
    from the one of the current serving cell, then the mobile station shall delete
    any LAI, TMSI, ciphering key sequence number stored in the SIM/USIM
    and list of equivalent PLMNs */
    if ( 0 == pstEplmnInfo->ucEquPlmnNum)
    {
        /* û��EPlmnʱ����֪ͨ����㵱ǰפ�������� */
        return VOS_TRUE;
    }

    if (VOS_FALSE == NAS_MML_CompareBcchPlmnwithSimPlmn(NAS_MML_GetCurrCampPlmnId(), &(pstEplmnInfo->astEquPlmnAddr[0])))
    {
        return VOS_FALSE;
    }

    pstCurPlmnId        = NAS_MML_GetCurrCampPlmnId();
    enCurRat            = NAS_MML_GetCurrNetRatType();

#if   (FEATURE_ON == FEATURE_LTE)
    /* ��������LTE��������ʱ */
    if ((VOS_TRUE == NAS_MMC_IsNeedDisableLteRoam(pstCurPlmnId->ulMcc))
     && (NAS_MML_NET_RAT_TYPE_LTE == enCurRat))
    {        
        ulIsPlmnForbid = VOS_TRUE;
    }
#endif    

    /* ��ȡ��ֹ���뼼����PLMN��Ϣ */
    pstDisabledRatPlmnCfg = NAS_MML_GetDisabledRatPlmnCfg();
    
    /* ��������뼼����PLMN ID */
    stPlmnWithRat.enRat          = NAS_MML_GetCurrNetRatType();
    stPlmnWithRat.stPlmnId.ulMcc = pstCurPlmnId->ulMcc;
    stPlmnWithRat.stPlmnId.ulMnc = pstCurPlmnId->ulMnc;
    
    /* �жϵ�ЧPLMN�Ƿ��ڽ�ֹ���뼼����PLMN�б���,������򲻽���PLMN ID����RRC */
    if (VOS_TRUE == NAS_MML_IsBcchPlmnIdWithRatInDestPlmnWithRatList(&stPlmnWithRat, 
                                                                     pstDisabledRatPlmnCfg->ulDisabledRatPlmnNum,
                                                                     pstDisabledRatPlmnCfg->astDisabledRatPlmnId))
    {   
        ulIsPlmnForbid = VOS_TRUE;
    }
    
    if (VOS_TRUE == NAS_MML_IsPlmnIdInForbidPlmnList(pstCurPlmnId))
    {
        ulIsPlmnForbid = VOS_TRUE;
    }

    pstForbidPlmnInfo = NAS_MML_GetForbidPlmnInfo();
    
    /* ����LTE��ֻҪ��forbidden plmn for gprs����Ϊ��ֹ */
    if ((VOS_TRUE == NAS_MML_IsSimPlmnIdInDestBcchPlmnList(pstCurPlmnId, pstForbidPlmnInfo->ucForbGprsPlmnNum, pstForbidPlmnInfo->astForbGprsPlmnList))
     && (NAS_MML_NET_RAT_TYPE_LTE == enCurRat))
    {
        ulIsPlmnForbid = VOS_TRUE;
    }
                   
    enCurrentFsmId     = NAS_MMC_GetCurrFsmId();
    ulCurrentEventType = NAS_MMC_GetCurrFsmEventType();
    
    ulIsUserSpecSrch    = NAS_MMC_IsUserSpecPlmnSrchType_PlmnSelection(enCurrentFsmId, ulCurrentEventType);

    /* �û�ָ�����������,EPLMN��Чʱ,��ǰפ��PLMN�ڽ�ֹ�б��ﲻӦ��֪ͨRRC��
       ����RRC����פ�� */
    if ((VOS_TRUE == ulIsUserSpecSrch)
     && (VOS_TRUE == ulIsPlmnForbid))
    {
        return VOS_FALSE;
    }
    
    return VOS_TRUE;
}

#if   (FEATURE_ON == FEATURE_LTE)

VOS_VOID NAS_MMC_DisableLteForSimReason_PlmnSelection(VOS_VOID)
{
    NAS_MML_SIM_TYPE_ENUM_UINT8                             enSimType;
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;

    enSimType           = NAS_MML_GetSimType();
    enLCapabilityStatus = NAS_MML_GetLteCapabilityStatus();

    if ((VOS_TRUE == NAS_MML_IsPlatformSupportLte())
     && (NAS_MML_SIM_TYPE_SIM == enSimType))
    {
        if ((VOS_FALSE == NAS_MML_GetDisableLteRoamFlg())
         && (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS != enLCapabilityStatus))
        {
            if (NAS_MML_RAT_CAPABILITY_STATUS_DISABLE != NAS_MML_GetLteForbiddenStatusFlg())
            {
                /* ��WAS/GAS����disable LTE֪ͨ��Ϣ */
                NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_DISABLE);
                NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_DISABLE);

                /* ��LMM����disable LTE��Ϣ */
                NAS_MMC_SndLmmDisableLteNotify(MMC_LMM_DISABLE_LTE_REASON_SIM_CARD_SWITCH_ON);

            }
        }

        /* ����disable LTE������� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS);
    }

    return;
}
#endif
VOS_VOID NAS_MMC_RefreshPlmnSelectionListRcvMmaRegReq_PlmnSelection(
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList = VOS_NULL_PTR;

    pstPlmnSelectionList = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    switch (enPlmnSearchScene)
    {
        case NAS_MMC_PLMN_SEARCH_SCENE_REG_NCELL:

            /* ���°�NCELL��ʼ��ѡ���б� */
            PS_MEM_SET(pstPlmnSelectionList, 0x0, sizeof(NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU));
            NAS_MMC_InitPlmnSelectionList_RegNCell(pstPlmnSelectionList, VOS_NULL_PTR);
            break;

        case NAS_MMC_PLMN_SEARCH_SCENE_REG_HPLMN:

            /* ɾ����HPLMN */
            NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(NAS_MMC_PLMN_TYPE_RPLMN, pstPlmnSelectionList);
            NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(NAS_MMC_PLMN_TYPE_UPLMN, pstPlmnSelectionList);
            NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(NAS_MMC_PLMN_TYPE_OPLMN, pstPlmnSelectionList);
            NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(NAS_MMC_PLMN_TYPE_EPLMN, pstPlmnSelectionList);
            NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(NAS_MMC_PLMN_TYPE_AVAIL_PLMN, pstPlmnSelectionList);
            break;

        case NAS_MMC_PLMN_SEARCH_SCENE_REG_PREF_PLMN:

            /* ɾ����HPLMN+UOPLMN */
            NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(NAS_MMC_PLMN_TYPE_RPLMN, pstPlmnSelectionList);
            NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(NAS_MMC_PLMN_TYPE_EPLMN, pstPlmnSelectionList);

            if (VOS_TRUE == NAS_MML_Get3GPP2UplmnNotPrefFlg())
            {
                NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(NAS_MMC_PLMN_TYPE_UPLMN, pstPlmnSelectionList);
            }

            NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(NAS_MMC_PLMN_TYPE_AVAIL_PLMN, pstPlmnSelectionList);
            break;

        default:

            /* ������������Ҫˢ��ѡ���б� */
            break;
    }

    /* ��ά�ɲ⣬���ѡ���б���Ϣ */
    NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionList);

    return;
}
VOS_UINT32 NAS_MMC_IsAllowedRegAfterAcqSucc_PlmnSelection(VOS_VOID)
{
    /* �����ǰע������3GPP MMC���Ʒ���VOS_TRUE */
    if (NAS_MMC_REG_CONTROL_BY_3GPP_MMC == NAS_MMC_GetRegCtrl())
    {
        return VOS_TRUE;
    }

    /* �����ǰע������3GPP2 CBP���ƶ�����MMA������ע�᷵��VOS_TRUE */
    if (VOS_TRUE == NAS_MMC_IsMmaRegReq_PlmnSelection())
    {
        return VOS_TRUE;
    }
    else
    {
        return VOS_FALSE;
    }
}


VOS_UINT32 NAS_MMC_IsMmaRegReq_PlmnSelection(VOS_VOID)
{
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene;

    enPlmnSearchScene = NAS_MMC_GetPlmnSearchScene_PlmnSelection();

    if ((NAS_MMC_PLMN_SEARCH_SCENE_REG_NCELL == enPlmnSearchScene)
     || (NAS_MMC_PLMN_SEARCH_SCENE_REG_HPLMN == enPlmnSearchScene)
     || (NAS_MMC_PLMN_SEARCH_SCENE_REG_PREF_PLMN == enPlmnSearchScene)
     || (NAS_MMC_PLMN_SEARCH_SCENE_REG_ANY_PLMN == enPlmnSearchScene))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_IsNeedSearchAvailPlmn_PlmnSelection(VOS_VOID)
{
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene;

    enPlmnSearchScene = NAS_MMC_GetPlmnSearchScene_PlmnSelection();

    if ((NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
     && (NAS_MMC_PLMN_SEARCH_SCENE_REG_HPLMN != enPlmnSearchScene)
     && (NAS_MMC_PLMN_SEARCH_SCENE_REG_PREF_PLMN != enPlmnSearchScene))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


#ifdef  __cplusplus
#if  __cplusplus
  }
#endif
#endif
