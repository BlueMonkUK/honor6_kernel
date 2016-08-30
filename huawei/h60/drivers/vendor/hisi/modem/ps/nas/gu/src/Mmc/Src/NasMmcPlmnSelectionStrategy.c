/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : NasMmcPlmnSelectionStrategy.c
  �� �� ��   : ����
  ��    ��   : sunxibo /46746
  ��������   : 2011��07��04��
  ����޸�   :
  ��������   : NAS ��״̬��PlmnSelection�����������ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��07��04��
    ��    ��   : sunxibo /46746
    �޸�����   : Added for V7R1 phase II

******************************************************************************/


/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "pslog.h"
#include "om.h"
#include "stdlib.h"
#include "NasMmcPlmnSelectionStrategy.h"
#include "NasMmcFsmPlmnSelection.h"
#include "NasMmlLib.h"
#include "NasComm.h"
#include "NasMmcCtx.h"
#include "NasMmcSndOm.h"
#include "NasMmcComFunc.h"
#include "NasMmcProcNvim.h"
#include "NasMmcFsmBgPlmnSearchTbl.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_NAS_MMC_PLMN_SELECTION_STRATEGY_C

/* ��������������Ӧ�������б��ʼ��������������״̬����ʼ��ʱʹ�� */
NAS_MMC_PLMN_SELECTION_LIST_INIT_TBL_STRU         g_astNasMmcPlmnSearchInitTbl[] =
{
    {NAS_MMC_PLMN_SEARCH_SCENE_SWITCH_ON,                   NAS_MMC_InitPlmnSelectionList_SwitchOn},
    {NAS_MMC_PLMN_SEARCH_SCENE_AREA_LOST,                   NAS_MMC_InitPlmnSelectionList_AreaLost},
    {NAS_MMC_PLMN_SEARCH_SCENE_SYSCFG_SET,                  NAS_MMC_InitPlmnSelectionList_SyscfgSet},
    {NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED,     NAS_MMC_InitPlmnSelectionList_AvailTimerExpire},
    {NAS_MMC_PLMN_SEARCH_SCENE_USER_LIST,                   NAS_MMC_InitPlmnSelectionList_UserList},
    {NAS_MMC_PLMN_SEARCH_SCENE_REG_REJ_PLMN_SEARCH,         NAS_MMC_InitPlmnSelectionList_RegRejPlmnSearch},
    {NAS_MMC_PLMN_SEARCH_SCENE_REG_REJ_CURRENT_PLMN_SEARCH, NAS_MMC_InitPlmnSelectionList_RegRejCurPlmnSearch},
    {NAS_MMC_PLMN_SEARCH_SCENE_USER_SPEC_PLMN_SEARCH,       NAS_MMC_InitPlmnSelectionList_UserSpecPlmnSearch},
    {NAS_MMC_PLMN_SEARCH_SCENE_MANUAL_CHANGE_AUTO_MODE,     NAS_MMC_InitPlmnSelectionList_ManualChangeAutoMode},
    {NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_PLMN_SEARCH,       NAS_MMC_InitPlmnSelectionList_HighPrioPlmnSearch},
    {NAS_MMC_PLMN_SEARCH_SCENE_DISABLE_LTE,                 NAS_MMC_InitPlmnSelectionList_DisableLte},
    {NAS_MMC_PLMN_SEARCH_SCENE_INTER_SYS_OOS,               NAS_MMC_InitPlmnSelectionList_InterSysOos},
#if (FEATURE_ON == FEATURE_LTE)
    {NAS_MMC_PLMN_SEARCH_SCENE_CSFB_SERVIEC_REJ,       NAS_MMC_InitPlmnSelectionListCsfbServiceRej},
#endif
    {NAS_MMC_PLMN_SEARCH_SCENE_USER_AUTO_RESEL,             NAS_MMC_InitPlmnSelectionList_UserAutoResel},

    {NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_NCELL_SEARCH, NAS_MMC_InitPlmnSelectionList_AvailTimerExpire},

    {NAS_MMC_PLMN_SEARCH_SCENE_RF_ENABLE,                   NAS_MMC_InitPlmnSelectionListRfEnable},    

    {NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_RAT_HPLMN_SEARCH,  NAS_MMC_InitPlmnSelectionList_HighPrioRatHplmnSearch},
    {NAS_MMC_PLMN_SEARCH_SCENE_REG_NCELL,                   NAS_MMC_InitPlmnSelectionList_RegNCell},
    {NAS_MMC_PLMN_SEARCH_SCENE_REG_HPLMN,                   NAS_MMC_InitPlmnSelectionList_RegHPlmn},
    {NAS_MMC_PLMN_SEARCH_SCENE_REG_PREF_PLMN,               NAS_MMC_InitPlmnSelectionList_RegPrefPlmn},
    {NAS_MMC_PLMN_SEARCH_SCENE_REG_ANY_PLMN,                NAS_MMC_InitPlmnSelectionList_AvailTimerExpire},


    {NAS_MMC_PLMN_SEARCH_SCENE_AREA_LOST_ROAMING_CFG,       NAS_MMC_InitPlmnSelectionList_Roaming_AreaLost},
};
VOS_VOID NAS_MMC_InitPlmnSelectionList(
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                          i;
    pNasMmcPlmnSelectionListInitFunc    pfunPlmnSearchInit;

    pfunPlmnSearchInit = VOS_NULL_PTR;

    /* ���ݳ�����g_astNasMmcPlmnSearchInitTbl���������б��ʼ������ */
    for (i = 0; i < sizeof(g_astNasMmcPlmnSearchInitTbl)/sizeof(NAS_MMC_PLMN_SELECTION_LIST_INIT_TBL_STRU); i++)
    {
        if (enPlmnSearchScene == g_astNasMmcPlmnSearchInitTbl[i].enPlmnSearchScene)
        {
            pfunPlmnSearchInit = g_astNasMmcPlmnSearchInitTbl[i].pfunPlmnSearchInit;
            break;
        }
    }

    /* ���ҳɹ�ʱ���ó�ʼ���������г�ʼ�� */
    if (VOS_NULL_PTR != pfunPlmnSearchInit)
    {
        PS_MEM_SET(pstPlmnSelectionListInfo, 0x0, sizeof(NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU));

        pfunPlmnSearchInit(pstPlmnSelectionListInfo, pstSearchedPlmnListInfo);
    }

    /* �û��б�����ʱ,��ʼ����Ϻ���Ҫ�����������б���и���,
       ��˴˴��ݲ�Log��Ϣ,��������Ϻ�Log */
    if (NAS_MMC_PLMN_SEARCH_SCENE_USER_LIST != enPlmnSearchScene)
    {
        /* ��ά�ɲ⣬���ѡ���б���Ϣ */
        NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionListInfo);
    }

    return;
}
VOS_VOID NAS_MMC_InitPlmnSelectionList_SwitchOn(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_HPLMN_CFG_INFO_STRU        *pstHPlmnCfg     = VOS_NULL_PTR;
    NAS_MML_RPLMN_CFG_INFO_STRU        *pstRPlmnCfg     = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                stRPlmn;

    /* ��ȡ��ǰ֧�ֵĽ��뼼���������ȼ� */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* ��ȡHPLMN��������Ϣ */
    pstHPlmnCfg    = NAS_MML_GetHplmnCfg();

    /* ��ȡRPLMN��������Ϣ */
    pstRPlmnCfg    = NAS_MML_GetRplmnCfg();

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        if ( VOS_TRUE == NAS_MMC_IsAisRoamingEffect())
        {
            /* ��ѡ���б�����������Ҫ��������������*/
            NAS_MMC_AddAisRoamPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }


        /* ֧����Ӫ��P4�Ŀ�������Hplmn����֧����������Hplmn */
        if ((VOS_TRUE == NAS_MMC_IsHPlmnPrioSearch())
         || (VOS_TRUE == pstHPlmnCfg->ucHplmnSearchPowerOn))
        {
            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else if (VOS_TRUE == pstRPlmnCfg->ucLastRplmnRatFlg) /* last Rplmn rat���� */
        {
            /* ��ѡ���б�������RPLMN,��last Rplmn rat������ǰ�� */
            NAS_MML_SortSpecRatPrioHighest(pstRPlmnCfg->enLastRplmnRat, &stPrioRatList);
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
    }
    else
    {
        /* ��ȡ�û�ָ������ */
        pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

        /* last Rplmn rat���� */
        if (VOS_TRUE == pstRPlmnCfg->ucLastRplmnRatFlg)
        {
            /* ��ѡ���б��������û�ָ��PLMN,ֻ����last Rplmn ratָʾ�Ľ��뼼�� */
            stPrioRatList.ucRatNum      = 1;
            stPrioRatList.aucRatPrio[0] = pstRPlmnCfg->enLastRplmnRat;
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstUserSpecPlmn->stPlmnId),
                                                   NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                   &stPrioRatList,
                                                   pstPlmnSelectionListInfo);
        }

        /* ��ѡ���б��������û�ָ��PLMN */
        NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        NAS_MMC_GetRPlmn_PlmnSelectionStrategy(pstRPlmnCfg->enLastRplmnRat, &stRPlmn);

        /* �û�ָ��������RPLMN��ͬʱ��Ҫ��EPLMN��ӵ�ѡ���б� */
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&stRPlmn,
                                                           &(pstUserSpecPlmn->stPlmnId)))
        {
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);
        }

        /* ֧��R7��������Hplmn */
        if (VOS_TRUE == pstHPlmnCfg->ucManualSearchHplmnFlg)
        {
            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
    }

    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_AreaLost(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;
    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU          *pEplmnAddr  = VOS_NULL_PTR;

    pEplmnAddr = NAS_MML_GetEquPlmnList();

    /* ��ȡ��ǰ֧�ֵĽ��뼼���������ȼ� */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ֧����������Hplmn */
        if (VOS_TRUE == NAS_MMC_IsHPlmnPrioSearch())
        {
            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������RPLMN,��ǰ���뼼��������� */
            NAS_MML_SortSpecRatPrioLowest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ѡ���б�������RPLMN,��ǰ���뼼��������� */
            NAS_MML_SortSpecRatPrioLowest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б�������RPLMN,��ǰ���뼼��������� */
            NAS_MML_SortSpecRatPrioLowest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       &stPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ȡ�û�ָ������ */
            pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

            /* ��ѡ���б��������û�ָ��PLMN,��ǰ���뼼��������� */
            NAS_MML_SortSpecRatPrioLowest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstUserSpecPlmn->stPlmnId),
                                                   NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                   &stPrioRatList,
                                                   pstPlmnSelectionListInfo);
        }

    }
    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_Roaming_AreaLost(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;

    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU          *pEplmnAddr  = VOS_NULL_PTR;

    pEplmnAddr = NAS_MML_GetEquPlmnList();

    /* ��ȡ��ǰ֧�ֵĽ��뼼���������ȼ� */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /*���ѵ�ǰ���뼼��*/
        PS_MEM_SET(&stPrioRatList, 0x0, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));
        stPrioRatList.ucRatNum = 1;
        stPrioRatList.aucRatPrio[0] = NAS_MML_GetCurrNetRatType();
        /* ��ѡ���б�������RPLMN,���ѵ�ǰ���뼼�� */
        NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

        /* ��ѡ���б�������EPLMN */
        NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б�������RPLMN,��ǰ���뼼��������� */
            NAS_MML_SortSpecRatPrioLowest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       &stPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ȡ�û�ָ������ */
            pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

            /* ��ѡ���б��������û�ָ��PLMN,��ǰ���뼼��������� */
            NAS_MML_SortSpecRatPrioLowest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstUserSpecPlmn->stPlmnId),
                                                   NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                   &stPrioRatList,
                                                   pstPlmnSelectionListInfo);
        }

    }
    return;
}
VOS_VOID NAS_MMC_InitPlmnSelectionList_InterSysOos(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;
    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU          *pEplmnAddr  = VOS_NULL_PTR;

    pEplmnAddr = NAS_MML_GetEquPlmnList();

    /* ��ȡ��ǰ֧�ֵĽ��뼼���������ȼ� */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* ��OOS���̽����������ϱ���PLMN���������б� */
    NAS_MMC_AddSearchedPlmnListInfoInPlmnSelectionList(pstPlmnSelectionListInfo, pstSearchedPlmnListInfo);

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ��ѡ���б�������RPLMN,��ǰ���뼼��������� */
        NAS_MML_SortSpecRatPrioLowest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);
        NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

        /* ��ѡ���б�������EPLMN */
        NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б�������RPLMN,��ǰ���뼼��������� */
            NAS_MML_SortSpecRatPrioLowest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       &stPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ȡ�û�ָ������ */
            pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

            /* ��ѡ���б��������û�ָ��PLMN,��ǰ���뼼��������� */
            NAS_MML_SortSpecRatPrioLowest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstUserSpecPlmn->stPlmnId),
                                                   NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                   &stPrioRatList,
                                                   pstPlmnSelectionListInfo);
        }

    }

    /* ��ά�ɲ⣬���ѡ���б���Ϣ */
    NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionListInfo);

    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_UserAutoResel(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    /* ��ѡ���б�������HPLMN��EHPLMN */
    NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

    /* ��ѡ���б�������UPLMN��OPLMN */
    NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

    /* ɾ����ǰפ�������� */
    NAS_MMC_DelSpecPlmnWithRatInPlmnList(NAS_MML_GetCurrCampPlmnId(),
                                         NAS_MML_GetCurrNetRatType(),
                                         pstPlmnSelectionListInfo);

    /* ��ά�ɲ⣬���ѡ���б���Ϣ */
    NAS_MMC_LogPlmnSelectionList(pstPlmnSelectionListInfo);

    return;
}


NAS_MML_NET_RAT_TYPE_ENUM_UINT8 NAS_MMC_GetPrioRat_SyscfgSet(VOS_VOID)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRatType;
    NAS_MML_RPLMN_CFG_INFO_STRU        *pstRPlmnCfg     = VOS_NULL_PTR;
    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurrNetRatType;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;

    enRatType        = NAS_MML_NET_RAT_TYPE_BUTT;
    pstRPlmnCfg      = NAS_MML_GetRplmnCfg();
    enCurrNetRatType = NAS_MML_GetCurrNetRatType();
    pstPrioRatList   = NAS_MML_GetMsPrioRatList();

    /* SVLTE���Կ��Ļ�����ǰפ���Ľ��뼼������ */
    if ( (VOS_TRUE == NAS_MML_GetSvlteSupportFlag())
      && (NAS_MML_NET_RAT_TYPE_BUTT != enCurrNetRatType))
    {
        enRatType = enCurrNetRatType;
    }
    else
    {
        /* SVLTE���Թرգ���ԭ���߼����� */
        if ( VOS_TRUE == NAS_MML_GetSyscfgTriHighRatSrchFlg() )
        {
            enRatType = pstPrioRatList->aucRatPrio[0];
        }
        else if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
        {
            enRatType = pstRPlmnCfg->enLastRplmnRat;
        }
        else
        {
            ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

            if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
             && (VOS_TRUE == ulEplmnValidFlg))
            {
                enRatType = pstRPlmnCfg->enLastRplmnRat;
            }
            else
            {
                pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();
                enRatType       = pstUserSpecPlmn->enRat;
            }
        }
    }

    return enRatType;
}



VOS_VOID NAS_MMC_InitPlmnSelectionList_SyscfgSet(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU          *pEplmnAddr  = VOS_NULL_PTR;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRatType;

    pEplmnAddr = NAS_MML_GetEquPlmnList();

    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));


    enRatType = NAS_MMC_GetPrioRat_SyscfgSet();

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ��ѡ���б�������RPLMN,��last Rplmn rat������ǰ�� */
        NAS_MML_SortSpecRatPrioHighest(enRatType, &stPrioRatList);
        NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

        /* ��ѡ���б�������EPLMN */
        NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б�������RPLMN,��last Rplmn rat������ǰ�� */
            NAS_MML_SortSpecRatPrioHighest(enRatType, &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       &stPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ȡ�û�ָ������ */
            pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

            /* ��ѡ���б��������û�ָ��PLMN,���û�ָ����rat������ǰ�� */
            NAS_MML_SortSpecRatPrioHighest(enRatType, &stPrioRatList);
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstUserSpecPlmn->stPlmnId),
                                                   NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                   &stPrioRatList,
                                                   pstPlmnSelectionListInfo);
        }
    }

    return;

}
VOS_VOID NAS_MMC_InitPlmnSelectionList_AvailTimerExpire(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU          *pEplmnAddr  = VOS_NULL_PTR;

    pEplmnAddr = NAS_MML_GetEquPlmnList();

    pstPrioRatList = NAS_MML_GetMsPrioRatList();

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ֧����������Hplmn */
        if (VOS_TRUE == NAS_MMC_IsHPlmnPrioSearch())
        {
            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddRPlmnInPlmnSelectionList(pstPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddRPlmnInPlmnSelectionList(pstPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       pstPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }

    }

    return;
}
VOS_VOID NAS_MMC_InitPlmnSelectionList_UserList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;

    NAS_MML_RPLMN_CFG_INFO_STRU        *pstRPlmnCfg     = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;

    VOS_UINT8                           ucPsAttachAllowFlg;
    VOS_UINT8                           ucCsAttachAllowFlg;

    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU          *pEplmnAddr  = VOS_NULL_PTR;

    pEplmnAddr = NAS_MML_GetEquPlmnList();

    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    pstRPlmnCfg    = NAS_MML_GetRplmnCfg();

    /* ��ѡ���б�������RPLMN,��last Rplmn rat������ǰ�� */
    NAS_MML_SortSpecRatPrioHighest(pstRPlmnCfg->enLastRplmnRat, &stPrioRatList);

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ֧����������Hplmn */
        if (VOS_TRUE == NAS_MMC_IsHPlmnPrioSearch())
        {
            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddRPlmnInPlmnSelectionList(pstPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {

            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������HPLMN��EHPLMN */
            NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

            /* ��ѡ���б�������UPLMN��OPLMN */
            NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {

            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       &stPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }

    }

    /* �������б��������svlte ˫��ģʽ��g�Ľ��뼼���ŵ��б����,��������Ҳ���t��l��������� */
    ucPsAttachAllowFlg = NAS_MML_GetPsAttachAllowFlg();
    ucCsAttachAllowFlg = NAS_MML_GetCsAttachAllowFlg();

    if ((VOS_TRUE == NAS_MML_GetSvlteSupportFlag())
     && (VOS_FALSE == ucPsAttachAllowFlg)
     && (VOS_FALSE == ucCsAttachAllowFlg))
    {
        NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_GSM,
                                                pstPlmnSelectionListInfo);
    }

    return;

}
VOS_VOID NAS_MMC_InitPlmnSelectionList_RegRejPlmnSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ɾ����ǰע�ᱻ�ܵ����� */
        NAS_MMC_DelSpecPlmnWithRatInPlmnList(NAS_MML_GetCurrCampPlmnId(),
                                             NAS_MML_GetCurrNetRatType(),
                                             pstPlmnSelectionListInfo);
    }
    else
    {
        /* ��ѡ���б��������û�ָ������ */
        NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }


    return;

}


VOS_VOID NAS_MMC_InitPlmnSelectionList_DisableLte(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList = VOS_NULL_PTR;
    VOS_UINT32                                              ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU                              *pEplmnAddr  = VOS_NULL_PTR;

    pEplmnAddr = NAS_MML_GetEquPlmnList();

    pstPrioRatList = NAS_MML_GetMsPrioRatList();

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {

        /* ��ѡ���б������� RPLMN */
        NAS_MMC_AddRPlmnInPlmnSelectionList(pstPrioRatList, pstPlmnSelectionListInfo);

        /* ��ѡ���б������� EPLMN */
        NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);


        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }
    else
    {

        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б������� RPLMN */
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       pstPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б������� EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);
        }


        /* ��ѡ���б��������û�ָ������ */
        NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }

    return;

}


VOS_VOID NAS_MMC_InitPlmnSelectionList_RegRejCurPlmnSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MMC_PLMN_TYPE_ENUM_UINT8        enPlmnType;
    NAS_MML_EQUPLMN_INFO_STRU          *pstEquPlmnList = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU               *pstPlmnId = VOS_NULL_PTR;
    VOS_UINT32                          ulEplmnValidFlg;

    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    pstPlmnId      = NAS_MML_GetCurrCampPlmnId();
    pstEquPlmnList = NAS_MML_GetEquPlmnList();

    /* ��ӵ�Plmn������Ĭ��ΪAvailPlmn */
    enPlmnType = NAS_MMC_PLMN_TYPE_AVAIL_PLMN;


    /* Rplmn�� EplmnList[0]��ͬʱ�����ΪRplmn���� */
    if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstPlmnId, &(pstEquPlmnList->astEquPlmnAddr[0]) ))
    {
        enPlmnType = NAS_MMC_PLMN_TYPE_RPLMN;
    }


    /* ��ѡ���б������ӵ�ǰ����,��ǰ���뼼��������ǰ�� */
    NAS_MML_SortSpecRatPrioHighest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);

    NAS_MMC_AddSpecPlmnInPlmnSelectionList(pstPlmnId,
                                           enPlmnType,
                                           &stPrioRatList,
                                           pstPlmnSelectionListInfo);

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ��ѡ���б�������EPLMN */
        NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);
        }

        /* ��ѡ���б��������û�ָ������ */
        NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }

    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_UserSpecPlmnSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                stRPlmn;
    NAS_MML_RPLMN_CFG_INFO_STRU        *pstRPlmnCfg     = VOS_NULL_PTR;

    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

    /* ��ȡRPLMN��������Ϣ */
    pstRPlmnCfg    = NAS_MML_GetRplmnCfg();

    /* ��ѡ���б��������û�ָ������,�û�ָ�����뼼��������ǰ�� */
    NAS_MML_SortSpecRatPrioHighest(pstUserSpecPlmn->enRat, &stPrioRatList);
    NAS_MMC_AddSpecPlmnInPlmnSelectionList(&pstUserSpecPlmn->stPlmnId,
                                           NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                           &stPrioRatList,
                                           pstPlmnSelectionListInfo);

    NAS_MMC_GetRPlmn_PlmnSelectionStrategy(pstRPlmnCfg->enLastRplmnRat, &stRPlmn);

    /* �û�ָ��������RPLMN��ͬʱ��Ҫ��EPLMN��ӵ�ѡ���б� */
    if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&stRPlmn,
                                                       &(pstUserSpecPlmn->stPlmnId)))
    {
        NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);
    }

    return;
}
VOS_VOID NAS_MMC_InitPlmnSelectionList_ManualChangeAutoMode(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList   = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserReselPlmn = VOS_NULL_PTR;

    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* �Զ��޸�Ϊ�ֶ�ʱ�ṩ������������� */
    pstUserReselPlmn = NAS_MMC_GetUserReselPlmnId();

    /* �Զ��޸�Ϊ�ֶ�ʱ������ṩ������������Ľ��뼼��������ǰ�� */
    NAS_MML_SortSpecRatPrioHighest(pstUserReselPlmn->enRat, &stPrioRatList);
    NAS_MMC_AddSpecPlmnInPlmnSelectionList(&pstUserReselPlmn->stPlmnId,
                                           NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                           &stPrioRatList,
                                           pstPlmnSelectionListInfo);

    /* ��ѡ���б�������EPLMN */
    NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

    /* ��ѡ���б�������HPLMN��EHPLMN */
    NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

    /* ��ѡ���б�������UPLMN��OPLMN */
    NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

    return;
}

#if (FEATURE_ON == FEATURE_LTE)

VOS_VOID NAS_MMC_InitPlmnSelectionListCsfbServiceRej(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn = VOS_NULL_PTR;
    VOS_UINT32                          ulEplmnValidFlg;
    NAS_MML_EQUPLMN_INFO_STRU          *pEplmnAddr  = VOS_NULL_PTR;

    pEplmnAddr = NAS_MML_GetEquPlmnList();

    /* ��ȡ��ǰ֧�ֵĽ��뼼���������ȼ� */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* �Զ�����ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ��ѡ���б�������RPLMN */
        NAS_MMC_AddRPlmnInPlmnSelectionList(&stPrioRatList, pstPlmnSelectionListInfo);

        /* ��ѡ���б�������EPLMN */
        NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������HPLMN��EHPLMN */
        NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }
    else
    {
        /* �û�ָ������ע��ɹ���,��������RPLMN��EPLMN */
        ulEplmnValidFlg = NAS_MML_GetEplmnValidFlg();

        if ((VOS_TRUE == NAS_MMC_GetUserSpecPlmnRegisterStatus())
         && (VOS_TRUE == ulEplmnValidFlg))
        {
            /* ��ѡ���б�������RPLMN  */
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pEplmnAddr->astEquPlmnAddr[0]),
                                       NAS_MMC_PLMN_TYPE_RPLMN,
                                       &stPrioRatList,
                                       pstPlmnSelectionListInfo);

            /* ��ѡ���б�������EPLMN */
            NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(pstPlmnSelectionListInfo);

            /* ��ѡ���б��������û�ָ������ */
            NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
        }
        else
        {
            /* ��ȡ�û�ָ������ */
            pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

            /* ��ѡ���б�������RPLMN */
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstUserSpecPlmn->stPlmnId),
                                                   NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                   &stPrioRatList,
                                                   pstPlmnSelectionListInfo);
        }

    }

    if (VOS_TRUE == NAS_MMC_IsNetRatSupportedTdscdma())
    {
        NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_WCDMA,
                                                pstPlmnSelectionListInfo);
    }

    /* �������б�������򣬽�L���뼼���ŵ��б����*/
    NAS_MMC_SortPlmnSearchListSpecRatPrioLowest(NAS_MML_NET_RAT_TYPE_LTE,
                                                pstPlmnSelectionListInfo);
    return;
}
#endif
VOS_VOID NAS_MMC_InitPlmnSelectionListRfEnable(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stDestFirstPlmnRatInfo;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enNoRfPlmnRat;
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    VOS_UINT8                                               ucExistNoRFRat;
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList      = VOS_NULL_PTR;

    /* ������ʼ�� */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();    
    enNoRfPlmnRat  = NAS_MMC_GetPlmnSrchNoRfRatType();
    ucExistNoRFRat = VOS_FALSE;
    PS_MEM_SET(&stDestFirstPlmnRatInfo, 0X00, sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));

    /* �����ǰMMC�б�����ѡ���б���ָ����뱸�ݵ�ѡ���б� */
    if ( NAS_MMC_BACKUP_PLMN_SELECTION_LIST == NAS_MMC_GetPlmnSelectionListType() )
    {
        PS_MEM_CPY(pstPlmnSelectionListInfo, NAS_MMC_GetHighPrioPlmnList(), sizeof(NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU));      

        return;
    }

    /* ����������������ʱ����ʱ���������б�����NO RFʱ��Ľ��뼼������Ϊ��һ���ȼ� */
    NAS_MMC_InitPlmnSelectionList_AvailTimerExpire(pstPlmnSelectionListInfo, pstSearchedPlmnListInfo);

    /* �쳣�����������ǰĿ����뼼��ΪBUTT,�򲻽��е��� */
    if ( NAS_MML_NET_RAT_TYPE_BUTT == enNoRfPlmnRat )
    {
        return;
    }

    /* �쳣�����������ǰ���������ˣ��򲻽��й��� */
    if ( NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST == pstPlmnSelectionListInfo->usSearchPlmnNum )
    {
        return;
    }

    /* ��ǰ�Ľ��뼼������Ϊ1��������Ҫ���������б� */
    if ( 1 == pstPrioRatList->ucRatNum)
    {
        return;
    }

    /* ������ѡ���б��в���Ŀ����뼼����������Ϣ */
    for ( i = 0 ; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++ )
    {
        for (j = 0; j < pstPlmnSelectionListInfo->astPlmnSelectionList[i].ucRatNum; j++)
        {
            /* �ҵ�Ŀ������뼼����������ڵ�һλ���ܸ�������Ҫ���� */
            if ( enNoRfPlmnRat == pstPlmnSelectionListInfo->astPlmnSelectionList[i].astPlmnRatInfo[j].enRatType )
            {
                PS_MEM_CPY(&stDestFirstPlmnRatInfo, 
                           &(pstPlmnSelectionListInfo->astPlmnSelectionList[i]), 
                           sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));

                /* ��Ŀ������Ľ��뼼������Ϊ����һ�� */
                stDestFirstPlmnRatInfo.ucRatNum = 1;
                PS_MEM_CPY(&(stDestFirstPlmnRatInfo.astPlmnRatInfo[0]),
                           &(pstPlmnSelectionListInfo->astPlmnSelectionList[i].astPlmnRatInfo[j]), 
                           sizeof(NAS_MMC_PLMN_RAT_INFO_STRU) );

                ucExistNoRFRat = VOS_TRUE;
                
                break;                
            }
        }

        /* �ҵ���Ŀ����뼼������ֱ������ */    
        if ( VOS_TRUE == ucExistNoRFRat )
        {
            break;
        }
    }
    
    /* û�ҵ�Ŀ����뼼��������һ����Ӧ�Ľ��뼼�� */
    if ( VOS_FALSE == ucExistNoRFRat )
    {
        /* ����Ҳ�����Ӧ�Ľ��뼼��������Ϣ������������һ��Ŀ����뼼������ */
        PS_MEM_CPY(&stDestFirstPlmnRatInfo, 
                   &(pstPlmnSelectionListInfo->astPlmnSelectionList[0]), 
                   sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
        
        stDestFirstPlmnRatInfo.ucRatNum = 1;
        stDestFirstPlmnRatInfo.astPlmnRatInfo[0].enRatType      = enNoRfPlmnRat;
        stDestFirstPlmnRatInfo.astPlmnRatInfo[0].enNetStatus    = NAS_MMC_NET_STATUS_BUTT;
        stDestFirstPlmnRatInfo.astPlmnRatInfo[0].enQuality      = NAS_MMC_NET_QUALITY_BUTT;
        stDestFirstPlmnRatInfo.astPlmnRatInfo[0].ucReserve      = 0;
        stDestFirstPlmnRatInfo.astPlmnRatInfo[0].lRscp          = NAS_MML_UTRA_RSCP_UNVALID;        
    }

    /* ����ǰѡ���б����һλ�󣬽�Ŀ����뼼���������ѡ���б� */
    PS_MEM_MOVE(&(pstPlmnSelectionListInfo->astPlmnSelectionList[1]),
                &(pstPlmnSelectionListInfo->astPlmnSelectionList[0]),
                (pstPlmnSelectionListInfo->usSearchPlmnNum) * sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));


    PS_MEM_CPY(&(pstPlmnSelectionListInfo->astPlmnSelectionList[0]),
               &stDestFirstPlmnRatInfo,
               sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
    
    pstPlmnSelectionListInfo->usSearchPlmnNum++;    
    
    return;
}
VOS_VOID NAS_MMC_InitPlmnSelectionList_RegNCell(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    VOS_UINT32                          i;
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MMC_NCELL_SEARCH_INFO_STRU     *pstMmcNcellSearchInfo = VOS_NULL_PTR;

    pstMmcNcellSearchInfo = NAS_MMC_GetNcellSearchInfo();

    /* ĿǰNCELL��֧��LTE���뼼�� */
    stPrioRatList.ucRatNum      = 1;
    stPrioRatList.aucRatPrio[0] = NAS_MML_NET_RAT_TYPE_LTE;

    /* ��ѡ���б�������NCELL��Ϣ�е�PLMN ID */
    for (i = 0; i < pstMmcNcellSearchInfo->stOtherModemEplmnInfo.ucEquPlmnNum; i ++)
    {
        NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstMmcNcellSearchInfo->stOtherModemEplmnInfo.astEquPlmnAddr[0]),
                                   NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                   &stPrioRatList,
                                   pstPlmnSelectionListInfo);
    }

    return;
}


VOS_VOID NAS_MMC_InitPlmnSelectionList_RegHPlmn(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    /* ��ѡ���б�������HPLMN��EHPLMN */
    NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

    return;
}
VOS_VOID NAS_MMC_InitPlmnSelectionList_RegPrefPlmn(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    /* ��ѡ���б�������HPLMN��EHPLMN */
    NAS_MMC_AddEHPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);

    if (VOS_FALSE == NAS_MML_Get3GPP2UplmnNotPrefFlg())
    {
        /* ��ѡ���б�������UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }
    else
    {
        /* ��ѡ���б�������OPLMN */
        NAS_MMC_AddOPlmnInPlmnSelectionList(pstPlmnSelectionListInfo);
    }
    
    return;
}
VOS_VOID NAS_MMC_BuildPlmnSelectionPlmnInfo(
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType,
    VOS_UINT16                                              usSimRat,
    NAS_MML_PLMN_ID_STRU                                   *pstPlmnId,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnInfo
)
{
    VOS_UINT8                           ucRatNum;

    ucRatNum                = 0x0;

    PS_MEM_CPY(&pstPlmnInfo->stPlmnId, pstPlmnId, sizeof(NAS_MML_PLMN_ID_STRU));
    pstPlmnInfo->enPlmnType = enPlmnType;

#if (FEATURE_ON == FEATURE_LTE)
    if (NAS_MML_SIM_RAT_E_UTRN == (NAS_MML_SIM_RAT_E_UTRN & usSimRat))
    {
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enRatType   = NAS_MML_NET_RAT_TYPE_LTE;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enNetStatus = NAS_MMC_NET_STATUS_NO_SEARCHED;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enQuality   = NAS_MMC_NET_QUALITY_UNKNOWN;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].lRscp       = NAS_MML_UTRA_RSCP_UNVALID;
        ucRatNum++;
    }
#endif

    if (NAS_MML_SIM_RAT_UTRN == (NAS_MML_SIM_RAT_UTRN & usSimRat))
    {
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enRatType   = NAS_MML_NET_RAT_TYPE_WCDMA;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enNetStatus = NAS_MMC_NET_STATUS_NO_SEARCHED;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enQuality   = NAS_MMC_NET_QUALITY_UNKNOWN;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].lRscp       = NAS_MML_UTRA_RSCP_UNVALID;
        ucRatNum++;
    }

    if (NAS_MML_SIM_RAT_GSM == (NAS_MML_SIM_RAT_GSM & usSimRat))
    {
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enRatType   = NAS_MML_NET_RAT_TYPE_GSM;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enNetStatus = NAS_MMC_NET_STATUS_NO_SEARCHED;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enQuality   = NAS_MMC_NET_QUALITY_UNKNOWN;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].lRscp       = NAS_MML_UTRA_RSCP_UNVALID;
        ucRatNum++;
    }

    pstPlmnInfo->ucRatNum = ucRatNum;

    /* ����ʣ��λ�ø�ֵΪ��Чֵ */
    for (; ucRatNum < NAS_MML_MAX_RAT_NUM; ucRatNum++)
    {
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enRatType   = NAS_MML_NET_RAT_TYPE_BUTT;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enNetStatus = NAS_MMC_NET_STATUS_BUTT;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enQuality   = NAS_MMC_NET_QUALITY_BUTT;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].lRscp       = NAS_MML_UTRA_RSCP_UNVALID;
    }

    return;
}
VOS_VOID NAS_MMC_AddEHPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MML_SIM_EHPLMN_INFO_STRU                           *pstEHPlmnInfo = VOS_NULL_PTR;
    VOS_UINT32                                              i;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;

    /* ��ȡEHPLMN */
    pstEHPlmnInfo = NAS_MML_GetSimEhplmnList();

    for (i = 0; i< pstEHPlmnInfo->ucEhPlmnNum; i++)
    {
        /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
        NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_HPLMN,
                                           pstEHPlmnInfo->astEhPlmnInfo[i].usSimRat,
                                           &(pstEHPlmnInfo->astEhPlmnInfo[i].stPlmnId),
                                           &stPlmnInfo);

        /* ��ӵ������б� */
        NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
    }

    return;
}
VOS_VOID NAS_MMC_AddEHPlmnInDestPlmnList(
    NAS_MML_PLMN_LIST_WITH_RAT_STRU                        *pstDestPlmnList
)
{
    NAS_MML_SIM_EHPLMN_INFO_STRU                           *pstEHPlmnInfo = VOS_NULL_PTR;
    VOS_UINT32                                              i;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstPlmnWithRat = VOS_NULL_PTR;
    VOS_UINT32                                              ulIndexOfPlmnList;
    NAS_MML_PLMN_ID_STRU                                    stPlmnId;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnIdWithRat;
    NAS_MML_DISABLED_RAT_PLMN_CFG_INFO_STRU                *pstDisabledRatPlmnCfg   = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stTempPlmn;

    if ( 0 == pstDestPlmnList->ulPlmnNum )
    {
        return;
    }

    /* ��ȡ��ֹ���뼼����PLMN��Ϣ */
    pstDisabledRatPlmnCfg = NAS_MML_GetDisabledRatPlmnCfg();

    /* ���б�ĵ�0λΪRPLMN */
    pstPlmnWithRat = &pstDestPlmnList->astPlmnId[0];

    /* ��ȡEHPLMN */
    pstEHPlmnInfo = NAS_MML_GetSimEhplmnList();

    PS_MEM_SET(&stTempPlmn, 0x00, sizeof(stTempPlmn));

    for (i = 0, ulIndexOfPlmnList = 1; i< NAS_MML_MIN(pstEHPlmnInfo->ucEhPlmnNum, NAS_MML_MAX_EHPLMN_NUM); i++)
    {
        /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
        PS_MEM_SET(&stPlmnId, 0x00, sizeof(stPlmnId));
        PS_MEM_SET(&stPlmnIdWithRat, 0x00, sizeof(stPlmnIdWithRat));
        PS_MEM_CPY(&stPlmnId, &(pstEHPlmnInfo->astEhPlmnInfo[i].stPlmnId), sizeof(stPlmnId));
        PS_MEM_CPY(&stPlmnIdWithRat.stPlmnId, &stPlmnId, sizeof(stPlmnIdWithRat.stPlmnId));

        stPlmnIdWithRat.enRat = pstPlmnWithRat->enRat;

        if ( VOS_TRUE == NAS_MML_IsBcchPlmnIdWithRatInDestPlmnWithRatList(&stPlmnIdWithRat, pstDestPlmnList->ulPlmnNum, pstDestPlmnList->astPlmnId) )
        {
            continue;
        }

        /* �жϸ�PLMN�Ƿ��ںڰ������� */
        if ( VOS_TRUE == NAS_MML_IsPlmnIdForbiddenByWhiteBlackList(&stPlmnId) )
        {
            continue;
        }

        /* ��װstTempPlmn�������ж��Ƿ񱻽����뼼�� */
        stTempPlmn.enPlmnType                  = NAS_MMC_PLMN_TYPE_HPLMN;
        stTempPlmn.ucRatNum                    = 1;
        stTempPlmn.astPlmnRatInfo[0].enRatType = pstPlmnWithRat->enRat;
        PS_MEM_CPY(&stTempPlmn.stPlmnId, &stPlmnId, sizeof(stTempPlmn.stPlmnId));

        /* ɾ���ڽ�ֹ���뼼��������Ľ��뼼�� */
        NAS_MMC_DelPlmnSelectionRatInfoInPlmnWithRatList(&stTempPlmn,
                                                  pstDisabledRatPlmnCfg->ulDisabledRatPlmnNum,
                                                  pstDisabledRatPlmnCfg->astDisabledRatPlmnId);

        /* ɾ���ڽ�ֹ���뼼���б��еĽ��뼼�� */
        NAS_MMC_DeleteForbiddenRatInPlmnSeleInfo(&stTempPlmn);

        /* ���������Ľ��뼼������Ϊ�㣬����� */
        if (0 == stTempPlmn.ucRatNum)
        {
            continue;
        }

        if ( NAS_MML_MAX_EHPLMN_NUM <= pstDestPlmnList->ulPlmnNum )
        {
            break;
        }

        pstDestPlmnList->astPlmnId[ulIndexOfPlmnList].enRat          = pstPlmnWithRat->enRat;

        PS_MEM_CPY(&(pstDestPlmnList->astPlmnId[ulIndexOfPlmnList].stPlmnId), &stPlmnId, sizeof(NAS_MML_PLMN_ID_STRU));
        pstDestPlmnList->ulPlmnNum++;
        ulIndexOfPlmnList++;
    }

    return;
}
VOS_VOID NAS_MMC_AddUOPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MML_SIM_USERPLMN_INFO_STRU                         *pstUserPlmnList = VOS_NULL_PTR;
    NAS_MML_SIM_OPERPLMN_INFO_STRU                         *pstOperPlmnList = VOS_NULL_PTR;
    NAS_MML_SIM_SELPLMN_INFO_STRU                          *pstSelPlmnList  = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;
    VOS_UINT32                                              i;
    VOS_UINT16                                              usSimRat;

    /* ��ȡUOPLMN */
    pstUserPlmnList = NAS_MML_GetSimUserPlmnList();

    pstOperPlmnList = NAS_MML_GetSimOperPlmnList();

    /* UPLMN������Ϊ0����OPLMN������Ϊ0 */
    if ((pstUserPlmnList->ucUserPlmnNum > 0)
     || (pstOperPlmnList->usOperPlmnNum > 0))
    {
        for (i = 0; i< pstUserPlmnList->ucUserPlmnNum; i++)
        {

            /* UPLMN����Plmn��Ч����usSimRatΪ0ʱ��Ĭ�϶�֧�� */
            usSimRat = pstUserPlmnList->astUserPlmnInfo[i].usSimRat;

            if (0 == usSimRat)
            {
                usSimRat = NAS_MML_SIM_RAT_ALL_SUPPORT;
            }

            /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
            NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_UPLMN,
                                               usSimRat,
                                               &(pstUserPlmnList->astUserPlmnInfo[i].stPlmnId),
                                               &stPlmnInfo);

            NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
        }

        for (i = 0; i< pstOperPlmnList->usOperPlmnNum; i++)
        {
            /* OPLMN����Plmn��Ч����usSimRatΪ0ʱ��Ĭ�϶�֧�� */
            usSimRat = pstOperPlmnList->astOperPlmnInfo[i].usSimRat;

            if (0 == usSimRat)
            {
                usSimRat = NAS_MML_SIM_RAT_ALL_SUPPORT;
            }

            /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
            NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_OPLMN,
                                               usSimRat,
                                               &(pstOperPlmnList->astOperPlmnInfo[i].stPlmnId),
                                               &stPlmnInfo);

            NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
        }
    }
    else /* ��ʾ(U)SIM���в�����UPLMN��OPLMN�������PLMN Sel�ļ� */
    {
        pstSelPlmnList = NAS_MML_GetSimSelPlmnList();

        for (i = 0; i< pstSelPlmnList->ucSelPlmnNum; i++)
        {
            /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
            NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_OPLMN,
                                               NAS_MML_SIM_RAT_ALL_SUPPORT,
                                               &(pstSelPlmnList->astPlmnId[i]),
                                               &stPlmnInfo);

            NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
        }
    }

    return;
}


VOS_VOID NAS_MMC_AddOPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MML_SIM_USERPLMN_INFO_STRU                         *pstUserPlmnList = VOS_NULL_PTR;
    NAS_MML_SIM_OPERPLMN_INFO_STRU                         *pstOperPlmnList = VOS_NULL_PTR;
    NAS_MML_SIM_SELPLMN_INFO_STRU                          *pstSelPlmnList  = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;
    VOS_UINT32                                              i;
    VOS_UINT16                                              usSimRat;

    /* ��ȡUOPLMN */
    pstUserPlmnList = NAS_MML_GetSimUserPlmnList();

    pstOperPlmnList = NAS_MML_GetSimOperPlmnList();

    /* UPLMN������Ϊ0����OPLMN������Ϊ0 */
    if ((pstUserPlmnList->ucUserPlmnNum > 0)
     || (pstOperPlmnList->usOperPlmnNum > 0))
    {
        for (i = 0; i< pstOperPlmnList->usOperPlmnNum; i++)
        {
            /* OPLMN����Plmn��Ч����usSimRatΪ0ʱ��Ĭ�϶�֧�� */
            usSimRat = pstOperPlmnList->astOperPlmnInfo[i].usSimRat;

            if (0 == usSimRat)
            {
                usSimRat = NAS_MML_SIM_RAT_ALL_SUPPORT;
            }

            /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
            NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_OPLMN,
                                               usSimRat,
                                               &(pstOperPlmnList->astOperPlmnInfo[i].stPlmnId),
                                               &stPlmnInfo);

            NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
        }
    }
    else /* ��ʾ(U)SIM���в�����UPLMN��OPLMN�������PLMN Sel�ļ� */
    {
        pstSelPlmnList = NAS_MML_GetSimSelPlmnList();

        for (i = 0; i< pstSelPlmnList->ucSelPlmnNum; i++)
        {
            /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
            NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_OPLMN,
                                               NAS_MML_SIM_RAT_ALL_SUPPORT,
                                               &(pstSelPlmnList->astPlmnId[i]),
                                               &stPlmnInfo);

            NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
        }
    }

    return;
}


VOS_VOID NAS_MMC_AddEPlmnInPlmnSelectionListInAutoMode(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnList = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;
    VOS_UINT32                                              i;
    NAS_MML_PLMN_ID_STRU                                    stPlmnId;

    /* ��ǰRplmn������ʱ�������Eplmn */
    if (VOS_FALSE == NAS_MML_GetRPlmn(NAS_MML_GetLastRplmnRat(), &stPlmnId))
    {
        return;
    }

    pstEquPlmnList = NAS_MML_GetEquPlmnList();


    /* Rplmn�� EplmnList[0]����ͬʱ������Ҫ��� */
    if (VOS_FALSE == NAS_MML_CompareBcchPlmnwithSimPlmn(&stPlmnId, &(pstEquPlmnList->astEquPlmnAddr[0])))
    {
        return;
    }

    for (i = 1; i < pstEquPlmnList->ucEquPlmnNum; i++)
    {
        /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
        NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_EPLMN,
                                           NAS_MML_SIM_RAT_ALL_SUPPORT,
                                           &(pstEquPlmnList->astEquPlmnAddr[i]),
                                           &stPlmnInfo);

        NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
    }
}
VOS_VOID NAS_MMC_AddEPlmnInPlmnSelectionListInMannulMode(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnList = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;
    VOS_UINT32                                              i;


    pstEquPlmnList = NAS_MML_GetEquPlmnList();


    for (i = 1; i < pstEquPlmnList->ucEquPlmnNum; i++)
    {
        /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
        NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_EPLMN,
                                           NAS_MML_SIM_RAT_ALL_SUPPORT,
                                           &(pstEquPlmnList->astEquPlmnAddr[i]),
                                           &stPlmnInfo);

        NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
    }
}


VOS_VOID NAS_MMC_AddRPlmnInPlmnSelectionList(
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;
    VOS_UINT32                                              i;
    NAS_MML_PLMN_ID_STRU                                    stRPlmn;
    NAS_MML_RPLMN_CFG_INFO_STRU                            *pstRPlmnCfg  = VOS_NULL_PTR;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enRPlmnRat;

    /* ��ȡRPLMN������Ϣ */
    pstRPlmnCfg = NAS_MML_GetRplmnCfg();

    for (i = 0; i < pstPrioRatList->ucRatNum; i++)
    {
        enRPlmnRat = pstPrioRatList->aucRatPrio[i];

        /* �����RPLMN����δ����,��ʹ�����ע��ɹ���RPLMN��Ϊ���н��뼼����RPLMN
           ���ߵ�ǰΪ�ֶ�ѡ��ģʽ,Ҳֻ�����ע��ɹ���RPLMNΪָ�������EPLMN,����RPLMN�������� */
        if ((NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
         || (VOS_FALSE == pstRPlmnCfg->ucMultiRATRplmnFlg))
        {
            enRPlmnRat = pstRPlmnCfg->enLastRplmnRat;
        }

        /* RPLMN��ȡʧ�� */
        if (VOS_FALSE == NAS_MMC_GetRPlmn_PlmnSelectionStrategy(enRPlmnRat,
                                                               &stRPlmn))
        {
            continue;
        }

        switch (pstPrioRatList->aucRatPrio[i])
        {
#if   (FEATURE_ON == FEATURE_LTE)
            case NAS_MML_NET_RAT_TYPE_LTE:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_RPLMN,
                                                   NAS_MML_SIM_RAT_E_UTRN,
                                                   &stRPlmn,
                                                   &stPlmnInfo);

                NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
                break;
#endif

            case NAS_MML_NET_RAT_TYPE_WCDMA:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_RPLMN,
                                                   NAS_MML_SIM_RAT_UTRN,
                                                   &stRPlmn,
                                                   &stPlmnInfo);

                NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
                break;

            case NAS_MML_NET_RAT_TYPE_GSM:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_RPLMN,
                                                   NAS_MML_SIM_RAT_GSM,
                                                   &stRPlmn,
                                                   &stPlmnInfo);

                NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
                break;

            default:
                NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_AddRPlmnInPlmnSelectionList: Rat not support.");
                break;
        }
    }

    return;
}


VOS_VOID NAS_MMC_AddUserSpecSearchPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstUserSpecPlmn = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;

    pstUserSpecPlmn = NAS_MMC_GetUserSpecPlmnId();

    /* ���ݽ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
    NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                       NAS_MML_SIM_RAT_ALL_SUPPORT,
                                       &(pstUserSpecPlmn->stPlmnId),
                                       &stPlmnInfo);

    NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);

    return;
}
VOS_VOID NAS_MMC_AddAisRoamPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MML_AIS_ROAMING_CFG_STRU           *pstRoamingCfg   = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU   stPlmnInfo;
    VOS_UINT16                              usSimRat;

    pstRoamingCfg  = NAS_MML_GetAisRoamingCfg();



#if (FEATURE_ON == FEATURE_LTE)
    if ( NAS_MML_NET_RAT_TYPE_LTE == pstRoamingCfg->enHighPrioRatType )
    {
        usSimRat = NAS_MML_SIM_RAT_E_UTRN;
    }
#endif
    if ( NAS_MML_NET_RAT_TYPE_WCDMA == pstRoamingCfg->enHighPrioRatType )
    {
        usSimRat = NAS_MML_SIM_RAT_UTRN;
    }
    else if ( NAS_MML_NET_RAT_TYPE_GSM == pstRoamingCfg->enHighPrioRatType )
    {
        usSimRat = NAS_MML_SIM_RAT_GSM;
    }
    else
    {
        usSimRat = NAS_MML_SIM_NO_RAT;
    }

    /* ���ݽ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
    NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_HPLMN,
                                       usSimRat,
                                       &(pstRoamingCfg->stHighPrioPlmnId),
                                       &stPlmnInfo);

    NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);

}
VOS_VOID NAS_MMC_AddSpecPlmnInPlmnSelectionList(
    NAS_MML_PLMN_ID_STRU                                   *pstPlmn,
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType,
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;
    VOS_UINT32                                              i;

    for (i = 0; i < pstPrioRatList->ucRatNum; i++)
    {
        switch (pstPrioRatList->aucRatPrio[i])
        {
#if   (FEATURE_ON == FEATURE_LTE)
            case NAS_MML_NET_RAT_TYPE_LTE:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(enPlmnType,
                                                   NAS_MML_SIM_RAT_E_UTRN,
                                                   pstPlmn,
                                                   &stPlmnInfo);

                NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
                break;
#endif

            case NAS_MML_NET_RAT_TYPE_WCDMA:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(enPlmnType,
                                                   NAS_MML_SIM_RAT_UTRN,
                                                   pstPlmn,
                                                   &stPlmnInfo);

                NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
                break;

            case NAS_MML_NET_RAT_TYPE_GSM:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(enPlmnType,
                                                   NAS_MML_SIM_RAT_GSM,
                                                   pstPlmn,
                                                   &stPlmnInfo);

                NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstPlmnSelectionListInfo);
                break;

            default:
                NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_AddSpecPlmnInPlmnSelectionList: Rat not support.");
                break;
        }
    }

    return;
}
VOS_UINT32 NAS_MMC_AppendPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmn,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT16                                              usSearchPlmnNum;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stTempPlmn;

    NAS_MML_PLMN_LOCK_CFG_INFO_STRU                        *pstPlmnLockCfg          = VOS_NULL_PTR;
    NAS_MML_DISABLED_RAT_PLMN_CFG_INFO_STRU                *pstDisabledRatPlmnCfg   = VOS_NULL_PTR;

    /* ��ȡ��ֹ���뼼����PLMN��Ϣ */
    pstDisabledRatPlmnCfg = NAS_MML_GetDisabledRatPlmnCfg();

    /* ���ڻ�ȡ�ڰ����� */
    pstPlmnLockCfg = NAS_MML_GetPlmnLockCfg();

    /* ��������������pstPlmn����ʱ�ֲ�������ȥ����֤pstPlmn���ݲ��ᱻ��ǰ�����޸� */
    PS_MEM_CPY(&stTempPlmn, pstPlmn, sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU) );

    /* ��PLMN��Ч */
    if (VOS_FALSE == NAS_MML_IsPlmnIdValid(&(stTempPlmn.stPlmnId)))
    {
        return VOS_FALSE;
    }

    /* �жϵ�ǰ�������Ƿ��ں������б��У�������ֱ�ӷ���VOS_FALSE */
    if ( VOS_TRUE == NAS_MMC_IsSpecTypePlmnIdInDestPlmnList(&(stTempPlmn.stPlmnId),
                                           stTempPlmn.enPlmnType,
                                           pstPlmnLockCfg->ucBlackPlmnLockNum,
                                           pstPlmnLockCfg->astBlackPlmnId) )     /* ��PLMN�ں������б��� */
    {
        return VOS_FALSE;
    }

    if (pstPlmnLockCfg->ucWhitePlmnLockNum != 0)
    {
        /* �жϵ�ǰ�������Ƿ��ڰ������б��У���������ֱ�ӷ���VOS_FALSE */
        if (VOS_FALSE == NAS_MMC_IsSpecTypePlmnIdInDestPlmnList(&(stTempPlmn.stPlmnId),
                                               stTempPlmn.enPlmnType,
                                               pstPlmnLockCfg->ucWhitePlmnLockNum,
                                               pstPlmnLockCfg->astWhitePlmnId))     /* ��PLMN���ڰ������б��� */
        {
            return VOS_FALSE;
        }
    }

    if (VOS_FALSE == NAS_MML_IsPlmnIdRoamingAllowed(&(stTempPlmn.stPlmnId))) /* ��PLMN�������������б��� */
    {
        return VOS_FALSE;
    }

    /* ɾ���ڽ�ֹ���뼼��������Ľ��뼼�� */
    NAS_MMC_DelPlmnSelectionRatInfoInPlmnWithRatList(&stTempPlmn,
                                              pstDisabledRatPlmnCfg->ulDisabledRatPlmnNum,
                                              pstDisabledRatPlmnCfg->astDisabledRatPlmnId);

    /* ɾ���ڽ�ֹ���뼼���б��еĽ��뼼�� */
    NAS_MMC_DeleteForbiddenRatInPlmnSeleInfo(&stTempPlmn);

    /* ���������Ľ��뼼������Ϊ�㣬����� */
    if (0 == stTempPlmn.ucRatNum)
    {
        return VOS_FALSE;
    }

    usSearchPlmnNum = pstPlmnSelectionListInfo->usSearchPlmnNum;

    if (usSearchPlmnNum >= NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_AppendPlmnInPlmnSelectionList: list full.");

        return VOS_FALSE;
    }

    /*����PLMN�洢���б�β��,PLMN������1*/
    PS_MEM_CPY(&(pstPlmnSelectionListInfo->astPlmnSelectionList[usSearchPlmnNum]),
               &stTempPlmn, sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
    pstPlmnSelectionListInfo->usSearchPlmnNum++;

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_InsertPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmn,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    VOS_UINT32                                              ulPos
)
{
    VOS_UINT16                          usSearchPlmnNum;

    usSearchPlmnNum = pstPlmnSelectionListInfo->usSearchPlmnNum;

    if ((usSearchPlmnNum >= NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST)
     || (ulPos >= NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST))
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_InsertPlmnInPlmnSelectionList: list full.");

        return VOS_FALSE;
    }

    if ((NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST - 1) != ulPos)
    {
        /* ����ulPosλ�ÿճ� */
        PS_MEM_MOVE(&(pstPlmnSelectionListInfo->astPlmnSelectionList[ulPos+1]),
                    &(pstPlmnSelectionListInfo->astPlmnSelectionList[ulPos]),
                    (NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST-ulPos-1)*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
    }

    /* ����PLMN���뵽�����б��ulPosλ�� */
    PS_MEM_CPY(&(pstPlmnSelectionListInfo->astPlmnSelectionList[ulPos]),
               pstPlmn, sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
    pstPlmnSelectionListInfo->usSearchPlmnNum++;

    return VOS_TRUE;
}


VOS_VOID NAS_MMC_InsertPlmnToPlmnSelectionListHead(
    NAS_MML_PLMN_ID_STRU                                   *pstPlmn,
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType,
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;
    VOS_UINT32                                              i;
    VOS_UINT32                                              ulRet;
    VOS_UINT32                                              ulPos;

    ulRet                               = VOS_FALSE;
    ulPos                               = 0;

    for (i = 0; i < pstPrioRatList->ucRatNum; i++)
    {
        switch (pstPrioRatList->aucRatPrio[i])
        {
#if   (FEATURE_ON == FEATURE_LTE)
            case NAS_MML_NET_RAT_TYPE_LTE:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(enPlmnType,
                                                   NAS_MML_SIM_RAT_E_UTRN,
                                                   pstPlmn,
                                                   &stPlmnInfo);

                ulRet = NAS_MMC_InsertPlmnInPlmnSelectionList(&stPlmnInfo,
                                                              pstPlmnSelectionListInfo,
                                                              ulPos);
                break;
#endif

            case NAS_MML_NET_RAT_TYPE_WCDMA:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(enPlmnType,
                                                   NAS_MML_SIM_RAT_UTRN,
                                                   pstPlmn,
                                                   &stPlmnInfo);

                ulRet = NAS_MMC_InsertPlmnInPlmnSelectionList(&stPlmnInfo,
                                                              pstPlmnSelectionListInfo,
                                                              ulPos);
                break;

            case NAS_MML_NET_RAT_TYPE_GSM:

                /* ���ݵ�ǰ���뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
                NAS_MMC_BuildPlmnSelectionPlmnInfo(enPlmnType,
                                                   NAS_MML_SIM_RAT_GSM,
                                                   pstPlmn,
                                                   &stPlmnInfo);

                ulRet = NAS_MMC_InsertPlmnInPlmnSelectionList(&stPlmnInfo,
                                                              pstPlmnSelectionListInfo,
                                                              ulPos);
                break;

            default:
                NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_InsertSpecPlmnInPlmnSelectionList: Rat not support.");
                break;
        }

        if (VOS_TRUE == ulRet)
        {
            ulPos++;
            ulRet = VOS_FALSE;
        }
    }

    return;
}
VOS_UINT32 NAS_MMC_UpdateRatNetStatusInPlmnRatInfo(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enRat,
    NAS_MMC_NET_STATUS_ENUM_UINT8                           enNetStatus,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstDstPlmn
)
{
    VOS_UINT32                             i;

    /* ��ԭPLMN��ÿһ��֧�ֵĽ��뼼����Ŀ��PLMN��ÿһ��֧�ֵĽ��뼼�����бȽ� */
    for (i = 0; i < pstDstPlmn->ucRatNum; i++)
    {
        /* �����ͬ�������Ŀ��PLMN��Ϣ�е�״̬��Ϣ */
        if (pstDstPlmn->astPlmnRatInfo[i].enRatType == enRat)
        {
            pstDstPlmn->astPlmnRatInfo[i].enNetStatus = enNetStatus;

            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_UpdatePlmnNetStatusInPlmnSelectionList(
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstPlmn,
    NAS_MMC_NET_STATUS_ENUM_UINT8                           enNetStatus,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                             i;
    VOS_UINT32                             ulRet;

    ulRet = VOS_FALSE;

    /* ����PLMN������뼼���������б���ÿ��PLMN������뼼���Ƚ� */
    for (i = 0; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++)
    {
        /* �Ƚ�pstPlmn->stPlmnId,��pstPlmnSelectionListInfo->astPlmnSelectionList[i].stPlmnId */
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstPlmn->stPlmnId),
                                                           &(pstPlmnSelectionListInfo->astPlmnSelectionList[i].stPlmnId)))
        {
            /* �����ͬ�����������б���PLMN��Ӧ���뼼������Ϣ */
            if (VOS_TRUE == NAS_MMC_UpdateRatNetStatusInPlmnRatInfo(pstPlmn->enRat,
                                                                    enNetStatus,
                                                                    &(pstPlmnSelectionListInfo->astPlmnSelectionList[i])))
            {
                ulRet = VOS_TRUE;
            }
        }
    }

    return ulRet;
}
VOS_UINT32 NAS_MMC_GetSpecQualAvailPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstAvailPlmnList,
    NAS_MMC_NET_QUALITY_ENUM_UINT8                          enPlmnQuality,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    VOS_UINT32                                              ulPlmnNum;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnSelectionPlmnInfo = VOS_NULL_PTR;

    ulPlmnNum = 0x0;

    /* �������б�������ΪAVAIL�ź�����ΪenPlmnQuality�������ҳ� */
    for (i = 0; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++)
    {
        pstPlmnSelectionPlmnInfo = &(pstPlmnSelectionListInfo->astPlmnSelectionList[i]);

        if (NAS_MMC_PLMN_TYPE_AVAIL_PLMN == pstPlmnSelectionPlmnInfo->enPlmnType)
        {
            /* ѭ���������еĽ��뼼�����ҳ���Ӧ���뼼������ΪenPlmnQuality������ */
            for (j = 0; j < pstPlmnSelectionPlmnInfo->ucRatNum; j++)
            {
                if (enPlmnQuality == pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enQuality)
                {
                    /* ��ʼ��ѡ����Ϣ */
                    NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                                       NAS_MML_INVALID_SIM_RAT,
                                                       &(pstPlmnSelectionPlmnInfo->stPlmnId),
                                                       &(pstAvailPlmnList[ulPlmnNum]));

                    /* ��ȡ��PLMN�����Ϣ */
                    PS_MEM_CPY(&pstAvailPlmnList[ulPlmnNum].astPlmnRatInfo[0],
                               &(pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j]),
                               sizeof(NAS_MMC_PLMN_RAT_INFO_STRU));

                    pstAvailPlmnList[ulPlmnNum].ucRatNum++;

                    ulPlmnNum ++;
                }
            }
        }
    }

    return ulPlmnNum;
}


VOS_VOID NAS_MMC_SortRatPrioSpecRat(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enPrioRat,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat,
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulPrioRatIndex;
    VOS_UINT32                          ulRatIndex;
    VOS_UINT32                          ulFoundPrioRatFlg;
    VOS_UINT32                          ulFoundRatFlg;

    ulPrioRatIndex    = 0x0;
    ulRatIndex        = 0x0;

    ulFoundPrioRatFlg = VOS_FALSE;
    ulFoundRatFlg     = VOS_FALSE;

    /* �ҳ����Ƚ��뼼���ͷ����Ƚ��뼼����λ�� */
    for (i = 0; i < pstPrioRatList->ucRatNum; i++)
    {
        if (enPrioRat == pstPrioRatList->aucRatPrio[i])
        {
            ulPrioRatIndex    = i;
            ulFoundPrioRatFlg = VOS_TRUE;
        }

        if (enRat == pstPrioRatList->aucRatPrio[i])
        {
            ulRatIndex        = i;
            ulFoundRatFlg     = VOS_TRUE;
        }
    }

    /* �����ǰ��֧�����Ƚ��뼼��,����Ҫ���� */
    if ((VOS_FALSE == ulFoundPrioRatFlg)
     || (VOS_FALSE == ulFoundRatFlg))
    {
        return;
    }

    /* ���ǰ�����ȼ��ͣ���Ҫ�����������ȵ��Ƶ�ǰ�� */
    if (ulPrioRatIndex > ulRatIndex)
    {
        PS_MEM_MOVE(&(pstPrioRatList->aucRatPrio[ulRatIndex+1]),
                    &(pstPrioRatList->aucRatPrio[ulRatIndex]),
                    (ulPrioRatIndex-ulRatIndex)*sizeof(NAS_MML_NET_RAT_TYPE_ENUM_UINT8));

        pstPrioRatList->aucRatPrio[ulRatIndex] = enPrioRat;
    }

    return;

}
VOS_VOID NAS_MMC_SortPlmnListRandom(
    VOS_UINT32                                              ulPlmnNum,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnList
)
{
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU  stTempPlmnInfo;
    VOS_UINT32                             ulPlmnListNum;
    VOS_UINT32                             ulRand;

    ulPlmnListNum = ulPlmnNum;

    /* ������� */
    VOS_SetSeed(VOS_GetTick());

    for (; ulPlmnListNum > 1; ulPlmnListNum--)
    {
        /* ��ȡ����� */
        ulRand = VOS_Rand(ulPlmnListNum);
        if (ulRand >  (ulPlmnListNum - 1))
        {
            continue;
        }

        stTempPlmnInfo               = pstPlmnList[ulRand];
        pstPlmnList[ulRand]          = pstPlmnList[ulPlmnListNum-1];
        pstPlmnList[ulPlmnListNum-1] = stTempPlmnInfo;
    }

    return;
}


VOS_VOID NAS_MMC_SortPlmnListPrioRat(
    VOS_UINT32                                             *pulPlmnNum,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnList,
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    VOS_UINT32                                              ulTmpPlmnNum;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstTmpPlmnList = VOS_NULL_PTR;

    /* PlmnNum����Ϊ0ʱ���������� */
    if (0 == (*pulPlmnNum))
    {
        return;
    }

    pstTmpPlmnList = (NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                   ((*pulPlmnNum)*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU)));

    if (VOS_NULL_PTR == pstTmpPlmnList)
    {
        /* ��ӡ�쳣 */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SortPlmnListPrioRat: Mem alloc fail.");

        return;
    }

    ulTmpPlmnNum = 0x0;

    /*���򷽷���,ѭ�����еĽ��뼼��,��������ȼ��Ľ��뼼������ŵ����һ����ѭ����֤�����
      �ȼ����뼼�������綼����ǰ�棬���Ŷ����������������ͬ��ʽ���� */
    for (i = 0; i < pstPrioRatList->ucRatNum; i++)
    {
        for (j = 0; j < (*pulPlmnNum); j++)
        {
            if (pstPrioRatList->aucRatPrio[i] == pstPlmnList[j].astPlmnRatInfo[0].enRatType)
            {
                PS_MEM_CPY(&pstTmpPlmnList[ulTmpPlmnNum], &pstPlmnList[j], sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
                ulTmpPlmnNum++;
            }
        }
    }

    *pulPlmnNum = ulTmpPlmnNum;

    PS_MEM_CPY(pstPlmnList, pstTmpPlmnList, (ulTmpPlmnNum*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU)));

    PS_MEM_FREE(WUEPS_PID_MMC, pstTmpPlmnList);

    return;
}


VOS_VOID NAS_MMC_SortPlmnListSignalDecrease(
    VOS_UINT32                                              ulPlmnNum,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnList
)
{
    /* ��ͬ���뼼����������׼��һ�£���δȷ�����򷽷� */

    return;
}
VOS_VOID NAS_MMC_SortAvailPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU *pstHighQualityAvailPlmnList  = VOS_NULL_PTR;
    VOS_UINT32                             ulHighQualityAvailPlmnNum;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU *pstLowQualityAvailPlmnList   = VOS_NULL_PTR;
    VOS_UINT32                             ulLowQualityAvailPlmnNum;
    NAS_MML_PLMN_RAT_PRIO_STRU             stPrioRatList;
    NAS_MML_PLMN_RAT_PRIO_STRU            *pstPrioRatList;

    NAS_MML_MISCELLANEOUS_CFG_INFO_STRU   *pstMiscellaneousCfgInfo = VOS_NULL_PTR;

    pstMiscellaneousCfgInfo = NAS_MML_GetMiscellaneousCfgInfo();

    pstHighQualityAvailPlmnList = (NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                   (NAS_MML_MAX_RAT_NUM*RRC_MAX_HIGH_PLMN_NUM)*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));

    if (VOS_NULL_PTR == pstHighQualityAvailPlmnList)
    {
        /* ��ӡ�쳣 */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SortAvailPlmnInPlmnSelectionList: Mem alloc fail.");

        return;
    }

    pstLowQualityAvailPlmnList  = (NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                   (NAS_MML_MAX_RAT_NUM*RRC_MAX_HIGH_PLMN_NUM)*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));

    if (VOS_NULL_PTR == pstLowQualityAvailPlmnList)
    {
        /* ��ӡ�쳣 */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SortAvailPlmnInPlmnSelectionList: Mem alloc fail.");

        PS_MEM_FREE(WUEPS_PID_MMC, pstHighQualityAvailPlmnList);

        return;
    }

    /* �������б�������ΪAvailabe����Ϊ�ߵ�PLMNȡ�� */
    ulHighQualityAvailPlmnNum = NAS_MMC_GetSpecQualAvailPlmnInPlmnSelectionList(pstHighQualityAvailPlmnList,
                                                                            NAS_MMC_NET_HIGH_QUALITY,
                                                                            pstPlmnSelectionListInfo);

    /* �������б�������ΪAvailabe����Ϊ�͵�PLMNȡ�� */
    ulLowQualityAvailPlmnNum = NAS_MMC_GetSpecQualAvailPlmnInPlmnSelectionList(pstLowQualityAvailPlmnList,
                                                                            NAS_MMC_NET_LOW_QUALITY,
                                                                            pstPlmnSelectionListInfo);

    /* �������б�������ΪAvailabe����Ϊ�ߵ�PLMNɾ�� */
    NAS_MMC_DeleteSpecQualAvailPlmnInPlmnSelectionList(NAS_MMC_NET_HIGH_QUALITY, pstPlmnSelectionListInfo);

    /* �������б�������ΪAvailabe����Ϊ�͵�PLMNɾ�� */
    NAS_MMC_DeleteSpecQualAvailPlmnInPlmnSelectionList(NAS_MMC_NET_LOW_QUALITY, pstPlmnSelectionListInfo);

    /* �Ը�����PLMN����������� */
    NAS_MMC_SortPlmnListRandom(ulHighQualityAvailPlmnNum, pstHighQualityAvailPlmnList);

    if (NAS_MMC_NV_ITEM_ACTIVE == pstMiscellaneousCfgInfo->ucWcdmaPriorityGsmFlg)
    {
        pstPrioRatList = NAS_MML_GetMsPrioRatList();
        stPrioRatList  = *pstPrioRatList;

        /* �� W�Ľ��뼼���ƶ��� G��ǰ�� */
        NAS_MMC_SortRatPrioSpecRat(NAS_MML_NET_RAT_TYPE_WCDMA,
                                   NAS_MML_NET_RAT_TYPE_GSM,
                                  &stPrioRatList);

        NAS_MMC_SortPlmnListPrioRat(&ulHighQualityAvailPlmnNum,
                                        pstHighQualityAvailPlmnList,
                                        &stPrioRatList);
    }
    else if (NAS_MMC_NV_ITEM_ACTIVE == pstMiscellaneousCfgInfo->ucSortAvailalePlmnListRatPrioFlg)
    {
        pstPrioRatList = NAS_MML_GetMsPrioRatList();
        stPrioRatList  = *pstPrioRatList;

        /* ���������簴syscfg�������ȼ����� */
        NAS_MMC_SortPlmnListPrioRat(&ulHighQualityAvailPlmnNum,
                                     pstHighQualityAvailPlmnList,
                                    &stPrioRatList);

        /* ���������簴syscfg�������ȼ����� */
        NAS_MMC_SortPlmnListPrioRat(&ulLowQualityAvailPlmnNum,
                                     pstLowQualityAvailPlmnList,
                                    &stPrioRatList);
    }
    else
    {
    }

    /* �Ե�����PLMN���а��źŵݼ����� */
    NAS_MMC_SortPlmnListSignalDecrease(ulLowQualityAvailPlmnNum, pstLowQualityAvailPlmnList);



    /* ��������PLMN�ٷŻ�ѡ���б� */
    NAS_MMC_SaveAvailablePlmnInPlmnSelectionList(ulHighQualityAvailPlmnNum,
                                                 pstHighQualityAvailPlmnList,
                                                 pstPlmnSelectionListInfo);

    /* ��������PLMN�ڷŻ������б� */
    NAS_MMC_SaveAvailablePlmnInPlmnSelectionList(ulLowQualityAvailPlmnNum,
                                                 pstLowQualityAvailPlmnList,
                                                 pstPlmnSelectionListInfo);

    PS_MEM_FREE(WUEPS_PID_MMC, pstHighQualityAvailPlmnList);

    PS_MEM_FREE(WUEPS_PID_MMC, pstLowQualityAvailPlmnList);

    return;
}


VOS_VOID NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                             i;

    i = 0;

    /* �������б�������ΪenPlmnType������ɾ�� */
    while (i < pstPlmnSelectionListInfo->usSearchPlmnNum)
    {
        if (enPlmnType == pstPlmnSelectionListInfo->astPlmnSelectionList[i].enPlmnType)
        {
            pstPlmnSelectionListInfo->usSearchPlmnNum--;

            PS_MEM_MOVE(&pstPlmnSelectionListInfo->astPlmnSelectionList[i],
                        &pstPlmnSelectionListInfo->astPlmnSelectionList[i+1],
                        (pstPlmnSelectionListInfo->usSearchPlmnNum-i)*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));

            continue;
        }

        i++;
    }

    return;
}
VOS_VOID NAS_MMC_DeleteSpecQualAvailPlmnInPlmnSelectionList(
    NAS_MMC_NET_QUALITY_ENUM_UINT8                          enNetQuality,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnInfo = VOS_NULL_PTR;

    i = 0;

    /* �������б�������ΪAVAIL�ź�����ΪenNetQuality�������ҳ� */
    while (i < pstPlmnSelectionListInfo->usSearchPlmnNum)
    {
        pstPlmnInfo = &(pstPlmnSelectionListInfo->astPlmnSelectionList[i]);
        if (NAS_MMC_PLMN_TYPE_AVAIL_PLMN == pstPlmnInfo->enPlmnType)
        {
            j = 0;

            /* ѭ���������еĽ��뼼�����ҳ���Ӧ���뼼������ΪenPlmnQuality������ */
            while (j < pstPlmnInfo->ucRatNum)
            {
                if (enNetQuality == pstPlmnInfo->astPlmnRatInfo[j].enQuality)
                {
                    /* �������б��н���PLMN��Ӧ�Ľ��뼼����Ϣ��� */
                    pstPlmnInfo->ucRatNum --;

                    if (j < (NAS_MML_MAX_RAT_NUM - 1))
                    {
                        PS_MEM_MOVE(&pstPlmnInfo->astPlmnRatInfo[j],
                                    &pstPlmnInfo->astPlmnRatInfo[j+1],
                                    (pstPlmnInfo->ucRatNum - j)*sizeof(NAS_MMC_PLMN_RAT_INFO_STRU));
                    }

                    pstPlmnInfo->astPlmnRatInfo[pstPlmnInfo->ucRatNum].enRatType     = NAS_MML_NET_RAT_TYPE_BUTT;
                    pstPlmnInfo->astPlmnRatInfo[pstPlmnInfo->ucRatNum].enQuality     = NAS_MMC_NET_QUALITY_BUTT;
                    pstPlmnInfo->astPlmnRatInfo[pstPlmnInfo->ucRatNum].enNetStatus   = NAS_MMC_NET_STATUS_BUTT;

                    continue;
                }
                j++;
            }

            /* ɾ���������Ӧ���뼼����Ϣ������䲻֧���κν��뼼��������б���ɾ�������� */
            if (0 == pstPlmnInfo->ucRatNum)
            {
                pstPlmnSelectionListInfo->usSearchPlmnNum--;

                if (i < (NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST - 1))
                {
                    PS_MEM_MOVE(&pstPlmnSelectionListInfo->astPlmnSelectionList[i],
                                &pstPlmnSelectionListInfo->astPlmnSelectionList[i+1],
                                (pstPlmnSelectionListInfo->usSearchPlmnNum - i)*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
                }
                continue;
            }
        }

        i++;
    }

    return;
}
VOS_VOID NAS_MMC_DeleteSpecNetStatusAvailPlmnInPlmnSelectionList(
    NAS_MMC_NET_STATUS_ENUM_UINT8                           enNetStatus,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnInfo = VOS_NULL_PTR;

    i = 0;

    /* �������б�������ΪAVAIL,����״̬ΪNO SEARCHED�������ҳ� */
    while (i < pstPlmnSelectionListInfo->usSearchPlmnNum)
    {
        pstPlmnInfo = &(pstPlmnSelectionListInfo->astPlmnSelectionList[i]);
        if (NAS_MMC_PLMN_TYPE_AVAIL_PLMN == pstPlmnInfo->enPlmnType)
        {
            j = 0;

            /* ѭ���������еĽ��뼼�����ҳ�����״̬ΪNO SEARCHED�������ҳ� */
            while (j < pstPlmnInfo->ucRatNum)
            {
                if (enNetStatus == pstPlmnInfo->astPlmnRatInfo[j].enNetStatus)
                {
                    /* �������б��н���PLMN��Ӧ�Ľ��뼼����Ϣ��� */
                    pstPlmnInfo->ucRatNum --;

                    if (j < (NAS_MML_MAX_RAT_NUM - 1))
                    {
                        PS_MEM_MOVE(&pstPlmnInfo->astPlmnRatInfo[j],
                                    &pstPlmnInfo->astPlmnRatInfo[j+1],
                                    (pstPlmnInfo->ucRatNum - j)*sizeof(NAS_MMC_PLMN_RAT_INFO_STRU));
                    }

                    pstPlmnInfo->astPlmnRatInfo[pstPlmnInfo->ucRatNum].enRatType     = NAS_MML_NET_RAT_TYPE_BUTT;
                    pstPlmnInfo->astPlmnRatInfo[pstPlmnInfo->ucRatNum].enQuality     = NAS_MMC_NET_QUALITY_BUTT;
                    pstPlmnInfo->astPlmnRatInfo[pstPlmnInfo->ucRatNum].enNetStatus   = NAS_MMC_NET_STATUS_BUTT;

                    continue;
                }
                j++;
            }

            /* ɾ���������Ӧ���뼼����Ϣ������䲻֧���κν��뼼��������б���ɾ�������� */
            if (0 == pstPlmnInfo->ucRatNum)
            {
                pstPlmnSelectionListInfo->usSearchPlmnNum--;

                if (i < (NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST - 1))
                {
                    PS_MEM_MOVE(&pstPlmnSelectionListInfo->astPlmnSelectionList[i],
                                &pstPlmnSelectionListInfo->astPlmnSelectionList[i+1],
                                (pstPlmnSelectionListInfo->usSearchPlmnNum - i)*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
                }

                continue;
            }
        }

        i++;
    }

    return;
}


VOS_VOID NAS_MMC_DelSpecNetStatusRat(
    NAS_MMC_NET_STATUS_ENUM_UINT8       enNetStatus,
    NAS_MMC_NET_STATUS_ENUM_UINT8      *pstNetStatus,
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList
)
{
    VOS_UINT32                          i;

    i = 0;

    /* �쳣���� */
    if (0x0 == pstPrioRatList->ucRatNum)
    {
        return;

    }

    /* �������״̬ΪenNetStatus���Ѹ�״̬�Ľ��뼼����stPrioRatList��ɾ�� */
    while (i < (VOS_UINT32)(pstPrioRatList->ucRatNum-1))
    {
        if (enNetStatus == pstNetStatus[i])
        {
            pstPrioRatList->aucRatPrio[i] = NAS_MML_NET_RAT_TYPE_BUTT;

            PS_MEM_MOVE(&(pstPrioRatList->aucRatPrio[i]),
                        &(pstPrioRatList->aucRatPrio[i+1]),
                        (NAS_MML_MAX_RAT_NUM-i-1));

            PS_MEM_MOVE(&(pstNetStatus[i]),
                        &(pstNetStatus[i+1]),
                        (NAS_MML_MAX_RAT_NUM-i-1));

            (pstPrioRatList->ucRatNum)--;

            continue;
        }
        i++;
    }

    /* ���һ���Ĵ��� */
    if ((pstPrioRatList->ucRatNum > 0)
     && (enNetStatus == pstNetStatus[i]))
    {
        pstPrioRatList->aucRatPrio[i] = NAS_MML_NET_RAT_TYPE_BUTT;

        (pstPrioRatList->ucRatNum)--;
    }

    return;
}
VOS_VOID NAS_MMC_ModifyPlmnTypeInPlmnSelectionList(
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enSrcPlmnType,
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enDestPlmnType,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                             i;

    /* �������б�������ΪenSrcPlmnType������,�޸�������ΪenDestPlmnType */
    for (i = 0; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++)
    {
        if (enSrcPlmnType == pstPlmnSelectionListInfo->astPlmnSelectionList[i].enPlmnType)
        {
            pstPlmnSelectionListInfo->astPlmnSelectionList[i].enPlmnType = enDestPlmnType;
        }
    }

    return;
}
VOS_UINT32 NAS_MMC_GetNextSearchPlmn(
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstDestPlmn,
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList,
    VOS_UINT32                                              ulUserSpecPlmnSearch,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    VOS_UINT32                                              k;
    VOS_UINT32                                              ulIsPlmnSearchingAllowed;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnSelectionPlmnInfo = VOS_NULL_PTR;

    /* ���β��������б��е�ÿһ������ */
    for (i = 0; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++)
    {
        pstPlmnSelectionPlmnInfo = &(pstPlmnSelectionListInfo->astPlmnSelectionList[i]);

        /* ��SYSCFG���õĽ��뼼�����ȼ����β��� */
        for (k = 0; k < pstPrioRatList->ucRatNum; k++)
        {
            /* ���β���һ�����������֧�ֵĽ��뼼�� */
            for (j = 0; j < pstPlmnSelectionPlmnInfo->ucRatNum; j++)
            {
                /* ������ȵĽ��뼼���뵱ǰ���ҵ�����֧�ֵĽ��뼼����ͬ */
                if (pstPrioRatList->aucRatPrio[k] == pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enRatType)
                {
                    ulIsPlmnSearchingAllowed = NAS_MMC_IsPlmnSearchingAllowed_PlmnSelectionStrategy(&(pstPlmnSelectionPlmnInfo->stPlmnId),
                                                                                         pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enRatType,
                                                                                         pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enNetStatus,
                                                                                         ulUserSpecPlmnSearch);
                    if (VOS_FALSE == ulIsPlmnSearchingAllowed)
                    {
                        /* �洢�Ľ��뼼���������ظ� */
                        break;
                    }

                    pstDestPlmn->stPlmnId = pstPlmnSelectionPlmnInfo->stPlmnId;
                    pstDestPlmn->enRat    = pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enRatType;

                    return VOS_TRUE;
                }
            }
        }
    }

    return VOS_FALSE;
}
VOS_VOID  NAS_MMC_UpdatePlmnIdWithBCCHMnc(
    VOS_UINT32                          ulBcchMnc,
    NAS_MML_PLMN_ID_STRU               *pstDestPlmnId
)
{
    VOS_UINT8                           ucBcchMnc3;

    ucBcchMnc3 = (VOS_UINT8)((ulBcchMnc & 0x00ff0000) >> 16);

    if (0x0F == ucBcchMnc3)
    {
        pstDestPlmnId->ulMnc |= 0x0F0000;
    }
}


VOS_UINT32 NAS_MMC_UpdatePlmnRatInfoInSearchList(
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnSelectionInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstPlmnSearchInfo
)
{
    VOS_UINT32                               i;
    VOS_UINT32                               j;

    for (i = 0; i < pstPlmnSelectionInfo->ucRatNum; i++)
    {
        if (pstPlmnSearchInfo->enRatType == pstPlmnSelectionInfo->astPlmnRatInfo[i].enRatType)
        {
            break;
        }
    }

    if (i == pstPlmnSelectionInfo->ucRatNum)
    {
        return VOS_FALSE;
    }

    /* pstPlmnSelectionInfo->stPlmnId�Ƿ��ڸ������б��� */
    for (j = 0; j < pstPlmnSearchInfo->ulHighPlmnNum; j++)
    {
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstPlmnSearchInfo->astHighPlmnList[j]),
                                                           &(pstPlmnSelectionInfo->stPlmnId)))
        {
            /* �����ǰ���統ǰ���뼼��״̬Ϊinitial��not exist�������״̬Ϊ���ڣ�����Ϊ������ */
            if (NAS_MMC_NET_STATUS_SEARCHED_REGISTERED != pstPlmnSelectionInfo->astPlmnRatInfo[i].enNetStatus)
            {
                pstPlmnSelectionInfo->astPlmnRatInfo[i].enNetStatus = NAS_MMC_NET_STATUS_SEARCHED_EXIST;
            }
            pstPlmnSelectionInfo->astPlmnRatInfo[i].enQuality   = NAS_MMC_NET_HIGH_QUALITY;
            pstPlmnSelectionInfo->astPlmnRatInfo[i].lRscp       = NAS_MML_UTRA_RSCP_UNVALID;

            /* BCCH���ϱ���Mnc�п��ܳ��Ⱥ�SIM���д洢��Mnc��һ����ͳһ��ΪBCCH�е�Mnc */
            pstPlmnSelectionInfo->stPlmnId.ulMnc = pstPlmnSearchInfo->astHighPlmnList[j].ulMnc;
            return VOS_TRUE;
        }
    }

    for (j = 0; j < pstPlmnSearchInfo->ulLowPlmnNum; j++)
    {
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn((NAS_MML_PLMN_ID_STRU*)&(pstPlmnSearchInfo->astLowPlmnList[j].stPlmnId),
                                                           &(pstPlmnSelectionInfo->stPlmnId)))
        {
            /* �����ǰ���統ǰ���뼼��״̬Ϊinitial��not exist�������״̬Ϊ���ڣ�����Ϊ����������¼��������ֵ */
            if (NAS_MMC_NET_STATUS_SEARCHED_REGISTERED != pstPlmnSelectionInfo->astPlmnRatInfo[i].enNetStatus)
            {
                pstPlmnSelectionInfo->astPlmnRatInfo[i].enNetStatus = NAS_MMC_NET_STATUS_SEARCHED_EXIST;
            }
            pstPlmnSelectionInfo->astPlmnRatInfo[i].enQuality   = NAS_MMC_NET_LOW_QUALITY;
            pstPlmnSelectionInfo->astPlmnRatInfo[i].lRscp       = pstPlmnSearchInfo->astLowPlmnList[j].lRscp;

            /* BCCH���ϱ���Mnc�п��ܳ��Ⱥ�SIM���д洢��Mnc��һ����ͳһ��ΪBCCH�е�Mnc */
            pstPlmnSelectionInfo->stPlmnId.ulMnc = pstPlmnSearchInfo->astLowPlmnList[j].stPlmnId.ulMnc;
            return VOS_TRUE;
        }
    }

    /* �����ǰ���統ǰ���뼼��״̬Ϊinitial��exist��exist seached�������״̬Ϊ������ */
    pstPlmnSelectionInfo->astPlmnRatInfo[i].enNetStatus = NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST;
    pstPlmnSelectionInfo->astPlmnRatInfo[i].enQuality   = NAS_MMC_NET_QUALITY_UNKNOWN;

    return VOS_FALSE;
}


VOS_VOID NAS_MMC_DelPlmnInSearchList(
    NAS_MML_PLMN_ID_STRU                                   *pstPlmn,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstPlmnSearchInfo
)
{
    VOS_UINT32                           i;

    /* �Ӹ������б���ɾ�������� */
    for (i = 0; i < pstPlmnSearchInfo->ulHighPlmnNum; i++)
    {
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstPlmnSearchInfo->astHighPlmnList[i]),
                                                           pstPlmn))
        {
            PS_MEM_MOVE(&(pstPlmnSearchInfo->astHighPlmnList[i]),
                        &(pstPlmnSearchInfo->astHighPlmnList[i+1]),
                        (pstPlmnSearchInfo->ulHighPlmnNum-i-1)*sizeof(NAS_MML_PLMN_ID_STRU));

            (pstPlmnSearchInfo->ulHighPlmnNum)--;

            return;
        }
    }

    /* �ӵ������б���ɾ�������� */
    for (i = 0; i < pstPlmnSearchInfo->ulLowPlmnNum; i++)
    {
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn((NAS_MML_PLMN_ID_STRU*)&(pstPlmnSearchInfo->astLowPlmnList[i].stPlmnId),
                                                           pstPlmn))
        {
            PS_MEM_MOVE(&(pstPlmnSearchInfo->astLowPlmnList[i]),
                        &(pstPlmnSearchInfo->astLowPlmnList[i+1]),
                        (pstPlmnSearchInfo->ulLowPlmnNum-i-1)*sizeof(NAS_MMC_LOW_PLMN_INFO_STRU));

            (pstPlmnSearchInfo->ulLowPlmnNum)--;

            return;
        }
    }

    return;

}



VOS_VOID NAS_MMC_UpdatePlmnListInPlmnSelectionList(
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstPlmnSearchInfo,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    VOS_UINT32                                              ulAppendFlg
)
{
    VOS_UINT32                                              i;
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstTmpPlmnSearchInfo = VOS_NULL_PTR;


    pstTmpPlmnSearchInfo = (NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                   sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU));

    if (VOS_NULL_PTR == pstTmpPlmnSearchInfo)
    {
        /* ��ӡ�쳣 */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_UpdatePlmnListInPlmnSelectionList: Mem alloc fail.");

        return;
    }

    PS_MEM_CPY(pstTmpPlmnSearchInfo, pstPlmnSearchInfo, sizeof(NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU));

    /* ѭ��ѡ���б���ÿһ������ */
    for (i = 0; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++)
    {
        /* ����һ�����뼼����������Ϣ���������б��е�һ�����磬������³ɹ�����Ҫ��������Ϣ�ж�Ӧ����ɾ�� */
        if (VOS_TRUE == NAS_MMC_UpdatePlmnRatInfoInSearchList(&(pstPlmnSelectionListInfo->astPlmnSelectionList[i]),
                                               pstPlmnSearchInfo))
        {
            NAS_MMC_DelPlmnInSearchList(&(pstPlmnSelectionListInfo->astPlmnSelectionList[i].stPlmnId), pstTmpPlmnSearchInfo);
        }
    }

    /* �����Ҫ�����������б���ϢpastPlmnSearchInfo�е�ʣ�����粹�䵽�����б��� */
    if (VOS_TRUE == ulAppendFlg)
    {
        NAS_MMC_SavePlmnListInPlmnSelectionList(pstTmpPlmnSearchInfo, pstPlmnSelectionListInfo);
    }

    PS_MEM_FREE(WUEPS_PID_MMC, pstTmpPlmnSearchInfo);

    return;
}


NAS_MMC_PLMN_TYPE_ENUM_UINT8 NAS_MMC_GetPlmnTypeInPlmnSelectionList(
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstPlmnWithRat,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{

    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnInfo = VOS_NULL_PTR;

    /* ����PLMN������뼼���������б���ÿ��PLMN������뼼���Ƚ� */
    for (i = 0; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++)
    {
        pstPlmnInfo = &(pstPlmnSelectionListInfo->astPlmnSelectionList[i]);

        /* �Ƚ�pstPlmn->stPlmnId,��pstPlmnSelectionListInfo->astPlmnSelectionList[i].stPlmnId */
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(&(pstPlmnWithRat->stPlmnId),
                                                           &(pstPlmnInfo->stPlmnId)))
        {
            /* �����ͬ���ȽϽ��뼼�� */
            for (j = 0; j < pstPlmnInfo->ucRatNum; j++)
            {
                if (pstPlmnWithRat->enRat == pstPlmnInfo->astPlmnRatInfo[j].enRatType)
                {
                    return pstPlmnInfo->enPlmnType;
                }
            }
        }
    }

    return NAS_MMC_PLMN_TYPE_BUTT;
}
VOS_VOID NAS_MMC_GetSpecPlmnNetStatusInPlmnSelectionList(
    NAS_MML_PLMN_ID_STRU                                   *pstPlmn,
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList,
    NAS_MMC_NET_STATUS_ENUM_UINT8                          *paucNetStatus,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    VOS_UINT32                                              k;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnInfo = VOS_NULL_PTR;

    /* ����PLMN������뼼���������б���ÿ��PLMN������뼼���Ƚ� */
    for (i = 0; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++)
    {
        pstPlmnInfo = &(pstPlmnSelectionListInfo->astPlmnSelectionList[i]);

        /* �Ƚ�pstPlmn->stPlmnId,��pstPlmnSelectionListInfo->astPlmnSelectionList[i].stPlmnId */
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstPlmn,
                                                           &(pstPlmnInfo->stPlmnId)))
        {
            /* �����ͬ���ȽϽ��뼼�� */
            for (j = 0; j < pstPrioRatList->ucRatNum; j++)
            {
                for (k = 0; k < pstPlmnInfo->ucRatNum; k++)
                {
                    if (pstPrioRatList->aucRatPrio[j] == pstPlmnInfo->astPlmnRatInfo[k].enRatType)
                    {
                        paucNetStatus[j] = pstPlmnInfo->astPlmnRatInfo[k].enNetStatus;
                        break;
                    }
                }
            }
        }
    }

    return;
}
VOS_UINT32 NAS_MMC_GetRPlmn_PlmnSelectionStrategy(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat,
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_RPLMN_CFG_INFO_STRU        *pstRPlmnCfg  = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                stRPlmn;

    /* ��ʼ��Ϊ��ЧPLMN */
    pstPlmnId->ulMcc = NAS_MML_INVALID_MCC;
    pstPlmnId->ulMnc = NAS_MML_INVALID_MNC;

    /* ��ȡRPLMN������Ϣ */
    pstRPlmnCfg = NAS_MML_GetRplmnCfg();

#if (FEATURE_ON == FEATURE_LTE)
    if (NAS_MML_NET_RAT_TYPE_LTE == enRat)
    {
        /* ��ȡLTE��RPLMN */
        return NAS_MML_GetLteRPlmn(pstPlmnId);
    }
#endif

    /* ��ȡGU��RPLMN */
    if (VOS_FALSE == NAS_MML_GetGURPlmn(pstPlmnId))
    {
        return VOS_FALSE;
    }

    /* �����֧�ֶ���뼼��RPLMN,����Ҫ��ȡ���Խ��뼼���±�����NV�е�RPLMN */
    if (VOS_FALSE == pstRPlmnCfg->ucMultiRATRplmnFlg)
    {
        return VOS_TRUE;
    }

    if (NAS_MML_NET_RAT_TYPE_WCDMA == enRat)
    {
        stRPlmn = pstRPlmnCfg->stWRplmnInNV;
    }
    else
    {
        stRPlmn = pstRPlmnCfg->stGRplmnInNV;
    }

    /* ��NV�б����RPLMN��Ч�Ҳ��ڽ�ֹ�����У�����NV�д洢������,���򷵻�֮ǰ��SIM���л�ȡ������ */
    if (VOS_TRUE == NAS_MML_IsPlmnIdValid(&stRPlmn))
    {
        if (VOS_FALSE == NAS_MML_IsPlmnIdInForbidPlmnList(&stRPlmn))
        {
           *pstPlmnId = stRPlmn;
        }
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_IsPlmnSearchingAllowed_PlmnSelectionStrategy(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat,
    NAS_MMC_NET_STATUS_ENUM_UINT8       enNetStatus,
    VOS_UINT32                          ulUserSpecPlmnSearch
)
{
#if (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                          ulAllowed;
#endif
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU                       *pstForbidPlmnInfo     = VOS_NULL_PTR;
    NAS_MML_DISABLED_RAT_PLMN_CFG_INFO_STRU                *pstDisabledRatPlmnCfg = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRatId;

    /* ��ȡ��ֹ���뼼����PLMN��Ϣ */
    pstDisabledRatPlmnCfg               = NAS_MML_GetDisabledRatPlmnCfg();

    /* ��ʼ��PLMN WITH RAT��Ϣ */
    stPlmnWithRatId.stPlmnId.ulMcc      = pstPlmnId->ulMcc;
    stPlmnWithRatId.stPlmnId.ulMnc      = pstPlmnId->ulMnc;
    stPlmnWithRatId.enRat               = enRat;

#if (FEATURE_ON == FEATURE_LTE)
    ulAllowed = VOS_TRUE;

    /* ��ǰ����ΪLTEʱ */
    if (NAS_MML_NET_RAT_TYPE_LTE == enRat)
    {
        /* �ж��Ƿ�����LTE������������� */
        ulAllowed   = NAS_MMC_IsLtePlmnSearchingAllowed_PlmnSelectionStrategy(pstPlmnId);
    }

    if (VOS_FALSE == ulAllowed)
    {
        return VOS_FALSE;
    }

#endif

    /* ���USIM��GSM��ֹ���룬��ѡ��GSM���� */
    if ( (VOS_TRUE == NAS_MML_GetGsmForbidFlg())
      && (NAS_MML_NET_RAT_TYPE_GSM == enRat) )
    {
        return VOS_FALSE;
    }
    /* �����ǰ���������������ڽ�ֹ���뼼�������б���,��׼������ */
    if (VOS_TRUE == NAS_MML_IsBcchPlmnIdWithRatInDestPlmnWithRatList(&stPlmnWithRatId,
                                                                      pstDisabledRatPlmnCfg->ulDisabledRatPlmnNum,
                                                                      pstDisabledRatPlmnCfg->astDisabledRatPlmnId))
    {
       return VOS_FALSE;
    }

    /* �����ǰ���ԵĽ��뼼���ڽ�ֹ���뼼���б��У���׼������ */
    if (VOS_TRUE == NAS_MML_IsRatInForbiddenList(stPlmnWithRatId.enRat))
    {
        return VOS_FALSE;
    }


    /* ����״̬��Ϊδ����������������,���������������� */
    if ((NAS_MMC_NET_STATUS_NO_SEARCHED != enNetStatus)
     && (NAS_MMC_NET_STATUS_SEARCHED_EXIST != enNetStatus))
    {
        return VOS_FALSE;
    }

    /* �ֶ�ģʽ����Ҫ���ж��Ƿ��ڽ�ֹ�����б��� */
    if (VOS_TRUE == ulUserSpecPlmnSearch)
    {
        return VOS_TRUE;
    }

    pstForbidPlmnInfo = NAS_MML_GetForbidPlmnInfo();


    /* ��ֹ���磬��Ϊ�϶���BCCH�������磬��forbid plmn�б��� */
    if (VOS_TRUE == NAS_MML_IsSimPlmnIdInDestBcchPlmnList(pstPlmnId,
                                          pstForbidPlmnInfo->ucForbPlmnNum,
                                          pstForbidPlmnInfo->astForbPlmnIdList))
    {
        return VOS_FALSE;
    }

    /* ����forbid plmn for gprs�б��� */
    if (VOS_FALSE == NAS_MML_IsSimPlmnIdInDestBcchPlmnList(pstPlmnId,
                                          pstForbidPlmnInfo->ucForbGprsPlmnNum,
                                          pstForbidPlmnInfo->astForbGprsPlmnList))
    {
        return VOS_TRUE;
    }

#if (FEATURE_ON == FEATURE_LTE)

    /* LTE��֧��EPS,������PS����,��ʾ�����粻������ */
    if (NAS_MML_NET_RAT_TYPE_LTE == enRat)
    {
        return VOS_FALSE;
    }
#endif

    /* ����ΪPS ONLYʱ,��������ֹGPRS�б��е����� */
    if (NAS_MML_MS_MODE_PS_ONLY == NAS_MML_GetMsMode())
    {
        return VOS_FALSE;
    }

    /* CS����Чʱ */
    if (VOS_FALSE == NAS_MML_GetSimCsRegStatus())
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU* NAS_MMC_SaveAvailablePlmnInPlmnSelectionList(
    VOS_UINT32                                              ulAvailPlmnNum,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstAvailPlmnList,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    /* �ж�����������Ƿ�ᵼ��ѡ���б���� */
    if ((pstPlmnSelectionListInfo->usSearchPlmnNum + ulAvailPlmnNum) < NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST)
    {
        PS_MEM_CPY(&pstPlmnSelectionListInfo->astPlmnSelectionList[pstPlmnSelectionListInfo->usSearchPlmnNum],
                   pstAvailPlmnList,
                   ulAvailPlmnNum*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
        pstPlmnSelectionListInfo->usSearchPlmnNum += (VOS_UINT16)ulAvailPlmnNum;
    }
    else
    {
        PS_MEM_CPY(&pstPlmnSelectionListInfo->astPlmnSelectionList[pstPlmnSelectionListInfo->usSearchPlmnNum],
                   pstAvailPlmnList,
                   (NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST-pstPlmnSelectionListInfo->usSearchPlmnNum)*sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));
        pstPlmnSelectionListInfo->usSearchPlmnNum = NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST;

        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SaveAvailablePlmnInPlmnSelectionList: Plmn selection list full.");
    }

    return pstAvailPlmnList;
}
VOS_VOID NAS_MMC_SavePlmnListInPlmnSelectionList(
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstPlmnSearchInfo,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                                              i;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmn;

    /* ��ʼ��ѡ����Ϣ */
    NAS_MMC_BuildPlmnSelectionPlmnInfo(NAS_MMC_PLMN_TYPE_AVAIL_PLMN, NAS_MML_INVALID_SIM_RAT,
                                       &(stPlmn.stPlmnId), &stPlmn);

    stPlmn.ucRatNum++;
    stPlmn.astPlmnRatInfo[0].enRatType     = pstPlmnSearchInfo->enRatType;
    stPlmn.astPlmnRatInfo[0].enNetStatus   = NAS_MMC_NET_STATUS_SEARCHED_EXIST;

    /* ѭ���������б��л����ڵĸ��������� */
    for (i = 0; i < pstPlmnSearchInfo->ulHighPlmnNum; i++)
    {
        stPlmn.stPlmnId                        = pstPlmnSearchInfo->astHighPlmnList[i];

        stPlmn.astPlmnRatInfo[0].enQuality     = NAS_MMC_NET_HIGH_QUALITY;

        /* ���뵽ѡ���б��� */
        NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmn, pstPlmnSelectionListInfo);
    }

    /* ѭ���������б��л����ڵĵ��������� */
    for (i = 0; i < pstPlmnSearchInfo->ulLowPlmnNum; i++)
    {
        stPlmn.stPlmnId.ulMcc                  = pstPlmnSearchInfo->astLowPlmnList[i].stPlmnId.ulMcc;
        stPlmn.stPlmnId.ulMnc                  = pstPlmnSearchInfo->astLowPlmnList[i].stPlmnId.ulMnc;

        stPlmn.astPlmnRatInfo[0].enQuality     = NAS_MMC_NET_LOW_QUALITY;
        stPlmn.astPlmnRatInfo[0].lRscp         = pstPlmnSearchInfo->astLowPlmnList[i].lRscp;

        /* ���뵽ѡ���б��� */
        NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmn, pstPlmnSelectionListInfo);
    }

    return;
}
VOS_VOID NAS_MMC_InitNetStatusInRatSearchInfo(
    NAS_MMC_NET_STATUS_ENUM_UINT8      *paucNetStatus,
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList,
    NAS_MMC_RAT_SEARCH_INFO_STRU       *pstSearchRatInfo
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          j;
    VOS_UINT8                           ucSearchAllBandFlg;

    for (i = 0; i < pstPrioRatList->ucRatNum; i++)
    {
        ucSearchAllBandFlg = VOS_FALSE;

        for (j = 0; j < NAS_MML_MAX_RAT_NUM; j++)
        {
            if (pstPrioRatList->aucRatPrio[i] == pstSearchRatInfo[j].enRatType)
            {
                ucSearchAllBandFlg = pstSearchRatInfo[j].ucSearchAllBand;
                break;
            }
        }

        if (VOS_TRUE == ucSearchAllBandFlg)
        {
            paucNetStatus[i] = NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST;
        }
        else
        {
            paucNetStatus[i] = NAS_MMC_NET_STATUS_NO_SEARCHED;
        }
    }
}


#if (FEATURE_ON == FEATURE_LTE)

VOS_VOID NAS_MMC_RefreshPlmnSelectionList_DisableLte_CsPsMode1RegSuccess(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList,
    NAS_MMC_RAT_SEARCH_INFO_STRU                           *pstSearchRatInfo
)
{
    NAS_MML_EQUPLMN_INFO_STRU          *pstEPlmnList    = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList  = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MMC_NET_STATUS_ENUM_UINT8       aucNetStatus[NAS_MML_MAX_RAT_NUM];
    VOS_UINT32                          i;

    /* NAS_MML_GetEquPlmnList ��ȡ��ǰ�����EPLMN */
    pstEPlmnList   = NAS_MML_GetEquPlmnList();
    pstPrioRatList = NAS_MML_GetMsPrioRatList();

    /* ��ԭ�������б��EPLMNȥ��*/
    /* �ֶ�ѡ��ģʽɾ��RPLMN��EPLMN,�û�ָ�����粻ɾ��,Ŀ�������������뼼���ϼ��������û�ָ������ */
    if (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ��ѡ���б��н��û�ָ��PLMN�޸�ΪAvail plmn,��֤������ȡ��һ����������ʱ,
           ����䴦�ڽ�ֹ�б���,�������������� */
        NAS_MMC_ModifyPlmnTypeInPlmnSelectionList(NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                  NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                                  pstPlmnSelectionList);

        /* ��ѡ���б���ɾ��RPLMN */
        NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(NAS_MMC_PLMN_TYPE_RPLMN,
                                                      pstPlmnSelectionList);

        /* ��ѡ���б���ɾ��EPLMN */
        NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(NAS_MMC_PLMN_TYPE_EPLMN,
                                                      pstPlmnSelectionList);
    }
    else
    {
        /* ��ѡ���б��н�RPLMN�޸�ΪAvail plmn */
        NAS_MMC_ModifyPlmnTypeInPlmnSelectionList(NAS_MMC_PLMN_TYPE_RPLMN,
                                                  NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                                  pstPlmnSelectionList);

        /* ��ѡ���б��н�EPLMN�޸�ΪAvail plmn */
        NAS_MMC_ModifyPlmnTypeInPlmnSelectionList(NAS_MMC_PLMN_TYPE_EPLMN,
                                                  NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                                  pstPlmnSelectionList);

        /* ��ѡ���б���ɾ������ΪUNKNOW��Avail plmn */
        NAS_MMC_DeleteSpecQualAvailPlmnInPlmnSelectionList(NAS_MMC_NET_QUALITY_UNKNOWN,
                                                           pstPlmnSelectionList);

        /* ��������available plmn */
        NAS_MMC_SortAvailPlmnInPlmnSelectionList(pstPlmnSelectionList);
    }

    /* ��EPLMN�Ӻ���ǰ��ӵ�ѡ���б� */
    for ( i = pstEPlmnList->ucEquPlmnNum; i > 0; i-- )
    {
        NAS_MMC_InitNetStatusInRatSearchInfo(aucNetStatus, pstPrioRatList, pstSearchRatInfo);

        NAS_MMC_GetSpecPlmnNetStatusInPlmnSelectionList(&(pstEPlmnList->astEquPlmnAddr[i-1]),
                                                          pstPrioRatList,
                                                          aucNetStatus,
                                                          pstPlmnSelectionList);

        PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(stPrioRatList));

        /* ����״̬ΪNAS_MMC_NET_STATUS_SEARCHED_NO_EXIST,NAS_MMC_NET_STATUS_SEARCHED_REGISTERED�Ľ��뼼��,
           ��ʾ����û��Ҫ�ڽ���������,��˴�stPrioRatList��ɾ�� */
        NAS_MMC_DelSpecNetStatusRat(NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST, aucNetStatus, &stPrioRatList);

        NAS_MMC_DelSpecNetStatusRat(NAS_MMC_NET_STATUS_SEARCHED_REGISTERED, aucNetStatus, &stPrioRatList);

        if (i==1)
        {
            /* ��RPLMN������뵽ѡ���б���ǰ�� */
            NAS_MMC_InsertPlmnToPlmnSelectionListHead(&(pstEPlmnList->astEquPlmnAddr[i-1]),
                                                        NAS_MMC_PLMN_TYPE_RPLMN,
                                                       &stPrioRatList,
                                                        pstPlmnSelectionList);
        }
        else
        {
            /* ��EPLMN������뵽ѡ���б���ǰ�� */
            NAS_MMC_InsertPlmnToPlmnSelectionListHead(&(pstEPlmnList->astEquPlmnAddr[i-1]),
                                                        NAS_MMC_PLMN_TYPE_EPLMN,
                                                       &stPrioRatList,
                                                        pstPlmnSelectionList);
        }

    }

    return;
}

#endif

VOS_VOID NAS_MMC_RefreshEPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList
)
{
    /* EPLMN��Чֱ�ӷ���,ֻ����EPLMN��Ч������²Ÿ���ѡ���б� */
    if (VOS_TRUE == NAS_MML_GetEplmnValidFlg())
    {
        return;
    }


    /* �ֶ�ѡ��ģʽɾ��RPLMN��EPLMN,�û�ָ�����粻ɾ��,Ŀ�������������뼼���ϼ��������û�ָ������ */
    if (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ��ѡ���б��н��û�ָ��PLMN�޸�ΪAvail plmn,��֤������ȡ��һ����������ʱ,
           ����䴦�ڽ�ֹ�б���,�������������� */
        NAS_MMC_ModifyPlmnTypeInPlmnSelectionList(NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,
                                                  NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                                  pstPlmnSelectionList);

        /* ��ѡ���б���ɾ��RPLMN */
        NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(NAS_MMC_PLMN_TYPE_RPLMN,
                                                      pstPlmnSelectionList);

        /* ��ѡ���б���ɾ��EPLMN */
        NAS_MMC_DeleteSpecTypePlmnInPlmnSelectionList(NAS_MMC_PLMN_TYPE_EPLMN,
                                                      pstPlmnSelectionList);

        return;
    }

    /* ��ѡ���б��н�RPLMN�޸�ΪAvail plmn */
    NAS_MMC_ModifyPlmnTypeInPlmnSelectionList(NAS_MMC_PLMN_TYPE_RPLMN,
                                              NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                              pstPlmnSelectionList);

    /* ��ѡ���б��н�EPLMN�޸�ΪAvail plmn */
    NAS_MMC_ModifyPlmnTypeInPlmnSelectionList(NAS_MMC_PLMN_TYPE_EPLMN,
                                              NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                              pstPlmnSelectionList);


    /* ��ѡ���б���ɾ������״̬Ϊδ���������� */
    NAS_MMC_DeleteSpecNetStatusAvailPlmnInPlmnSelectionList(NAS_MMC_NET_STATUS_NO_SEARCHED,
                                               pstPlmnSelectionList);


    /* ��������available plmn */
    NAS_MMC_SortAvailPlmnInPlmnSelectionList(pstPlmnSelectionList);

    return;
}


VOS_UINT32 NAS_MMC_IsHPlmnPrioSearch( VOS_VOID )
{
    NAS_MML_HPLMN_CFG_INFO_STRU        *pstHplmnCfgInfo = VOS_NULL_PTR;
    NAS_MML_SIM_EHPLMN_INFO_STRU       *pstEHPlmnInfo   = VOS_NULL_PTR;

    /* ��ȡHPLMN��������Ϣ */
    pstHplmnCfgInfo = NAS_MML_GetHplmnCfg();

    /* ��ȡSim����Ehplmn��ص���Ϣ */
    pstEHPlmnInfo  = NAS_MML_GetSimEhplmnList();

    /* �Զ�����ģʽ ֧�ֿ�����������Hplmn */
    if ((NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
     && ( ( (NAS_MML_AUTO_SRCH_FLG_TYPE_PLMN_SEL_IND == pstHplmnCfgInfo->enAutoSearchHplmnFlg)
       && (NAS_MML_LAST_RPLMN_SEL_IND_HPLMN == pstEHPlmnInfo->enPLMNSelInd))
     || (NAS_MML_AUTO_SRCH_FLG_TYPE_HPLMN_FIRST == pstHplmnCfgInfo->enAutoSearchHplmnFlg) ))
    {
        return VOS_TRUE;
    }
    else
    {
       return VOS_FALSE;
    }

}
VOS_UINT32 NAS_MMC_IsRPlmnFirstSearchWithHPlmn(VOS_VOID)
{
    NAS_MML_HPLMN_CFG_INFO_STRU        *pstHplmnCfgInfo = VOS_NULL_PTR;

    /* ��ȡHPLMN��������Ϣ */
    pstHplmnCfgInfo = NAS_MML_GetHplmnCfg();


    /* �Զ�����ģʽ ֧�ֿ�����������Hplmn */
    if ((NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
     && (NAS_MML_AUTO_SRCH_FLG_TYPE_RPLMN_WITH_HPLMN_FIRST == pstHplmnCfgInfo->enAutoSearchHplmnFlg))
    {
        return VOS_TRUE;
    }
    else
    {
       return VOS_FALSE;
    }
}




VOS_VOID NAS_MMC_RefreshCurPlmnInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList,
    NAS_MMC_RAT_SEARCH_INFO_STRU                           *pstSearchRatInfo,
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPrioRatList       = VOS_NULL_PTR;
    NAS_MML_PLMN_RAT_PRIO_STRU          stPrioRatList;
    NAS_MMC_NET_STATUS_ENUM_UINT8       aucNetStatus[NAS_MML_MAX_RAT_NUM];
    VOS_UINT32                          i;


    PS_MEM_SET(aucNetStatus, 0x00, sizeof(aucNetStatus));

    pstPrioRatList = NAS_MML_GetMsPrioRatList();
    PS_MEM_CPY(&stPrioRatList, pstPrioRatList, sizeof(NAS_MML_PLMN_RAT_PRIO_STRU));

    /* ��ѡ���б��л�ȡ��ǰ���������״̬ */
    NAS_MMC_InitNetStatusInRatSearchInfo(aucNetStatus, pstPrioRatList, pstSearchRatInfo);

    NAS_MMC_GetSpecPlmnNetStatusInPlmnSelectionList(NAS_MML_GetCurrCampPlmnId(),
                                                    pstPrioRatList,
                                                    aucNetStatus,
                                                    pstPlmnSelectionList);

    i = 0;

    /* ����״̬ΪNAS_MMC_NET_STATUS_SEARCHED_NO_EXIST,NAS_MMC_NET_STATUS_SEARCHED_REGISTERED�Ľ��뼼��,
       ��ʾ����û��Ҫ�ڽ���������,��˴�stPrioRatList��ɾ�� */
    while (i < stPrioRatList.ucRatNum)
    {
        if ((NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST == aucNetStatus[i])
         || (NAS_MMC_NET_STATUS_SEARCHED_REGISTERED == aucNetStatus[i]))
        {
            stPrioRatList.aucRatPrio[i] = NAS_MML_NET_RAT_TYPE_BUTT;

            if (i < (NAS_MML_MAX_RAT_NUM - 1))
            {
                PS_MEM_MOVE(&(stPrioRatList.aucRatPrio[i]),
                            &(stPrioRatList.aucRatPrio[i+1]),
                            (NAS_MML_MAX_RAT_NUM-i-1));

                PS_MEM_MOVE(&(aucNetStatus[i]),
                            &(aucNetStatus[i+1]),
                            (NAS_MML_MAX_RAT_NUM-i-1));
            }
            stPrioRatList.ucRatNum--;

            continue;
        }
        i++;
    }

    /* �����������ΪButt,��ʾѡ���б���δ�ҵ��˽��뼼������,���������;�Ϊavail����,
       ��ʱֻ��Ҫ����ǰ���뼼��������ǰ�漴�� */
    if (NAS_MMC_PLMN_TYPE_BUTT == enPlmnType)
    {
        enPlmnType = NAS_MMC_PLMN_TYPE_AVAIL_PLMN;
        NAS_MML_SortSpecRatPrioHighest(NAS_MML_GetCurrNetRatType(), &stPrioRatList);
    }
    else
    {
        /* ����ǰ���뼼�����뵽��ǰ�� */
        PS_MEM_MOVE(&(stPrioRatList.aucRatPrio[1]),
                    &(stPrioRatList.aucRatPrio[0]),
                    (stPrioRatList.ucRatNum*sizeof(NAS_MML_NET_RAT_TYPE_ENUM_UINT8)));

        stPrioRatList.aucRatPrio[0] = NAS_MML_GetCurrNetRatType();

        stPrioRatList.ucRatNum++;
    }

    /* ����ǰ������뵽ѡ���б���ǰ�� */
    NAS_MMC_InsertPlmnToPlmnSelectionListHead(NAS_MML_GetCurrCampPlmnId(),
                                              enPlmnType,
                                              &stPrioRatList,
                                              pstPlmnSelectionList);

    return;
}


VOS_VOID NAS_MMC_LogPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList       = VOS_NULL_PTR;

    pstPrioRatList = NAS_MML_GetMsPrioRatList();

    /* ��ѡ���б���Ϣ���͵�OM��� */
    NAS_MMC_SndOmPlmnSelectionList(pstPlmnSelectionList, pstPrioRatList);

    return;
}
VOS_VOID NAS_MMC_LogAsPlmnList(
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    *pstSrchedPlmn
)
{
    NAS_MMC_LOG_AS_PLMN_LIST_MSG_STRU                      *pstMsg  = VOS_NULL_PTR;

    pstMsg = (NAS_MMC_LOG_AS_PLMN_LIST_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                          (sizeof(NAS_MMC_LOG_AS_PLMN_LIST_MSG_STRU)));
    if (TAF_NULL_PTR == pstMsg)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_LogAsPlmnList:ERROR:Alloc Mem Fail.");
        return;
    }

    pstMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulSenderPid     = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulMsgName       = MMCOM_LOG_AS_PLMN_SELECTION_LIST;

    PS_MEM_CPY(&pstMsg->stSrchedPlmn, pstSrchedPlmn,sizeof(pstMsg->stSrchedPlmn));

    OM_TraceMsgHook(pstMsg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstMsg);

    return;
}



VOS_VOID NAS_MMC_InitPlmnSelectionList_HighPrioPlmnSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstHighPrioPlmnSearchListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{   
    VOS_UINT8                           uc3GPPUplmnNotPrefFlg;

    uc3GPPUplmnNotPrefFlg = NAS_MML_Get3GPP2UplmnNotPrefFlg();
    
    NAS_MMC_SetPlmnSelectionListType(NAS_MMC_STORE_HIGH_PRIO_PLMN_SELECTION_LIST);
    
    if ( VOS_TRUE == NAS_MMC_IsAisRoamingEffect())
    {
        /* ��ѡ���б�����������Ҫ��������������*/
        NAS_MMC_AddAisRoamPlmnInPlmnSelectionList(pstHighPrioPlmnSearchListInfo);
    }

    /* ���EHPLMN */
    NAS_MMC_AddEHPlmnInPlmnSelectionList(pstHighPrioPlmnSearchListInfo);

    if (NAS_MMC_REG_CONTROL_BY_3GPP_MMC == NAS_MMC_GetRegCtrl()
     || VOS_FALSE == uc3GPPUplmnNotPrefFlg)
    {
        /* ���UPLMN��OPLMN */
        NAS_MMC_AddUOPlmnInPlmnSelectionList(pstHighPrioPlmnSearchListInfo);
    }
    else
    {
        /* ���OPLMN */
        NAS_MMC_AddOPlmnInPlmnSelectionList(pstHighPrioPlmnSearchListInfo);
    }

    return;
}


VOS_UINT32 NAS_MMC_IsPlmnRatSupported(
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnInfo
)
{
    VOS_UINT32                         i;

    for (i = 0; i < pstPlmnInfo->ucRatNum; i++)
    {
        if (VOS_TRUE == NAS_MML_IsNetRatSupported(pstPlmnInfo->astPlmnRatInfo[i].enRatType))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MMC_IsHighPrioPlmnSameCountry(
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstHighPrioPlmnInfo,
    NAS_MML_PLMN_ID_STRU                                   *pstVPlmnId
)
{
    VOS_UINT32                                      ulIsSameCountryFlg;
    NAS_MML_BG_SEARCH_CFG_INFO_STRU                 *pstBgSearchCfg = VOS_NULL_PTR;

    NAS_MMC_HPLMN_SEARCH_REGARDLESS_MCC_CTX_STRU    *pstBgSearchRegardlessMccCtx = VOS_NULL_PTR;

    pstBgSearchCfg              = NAS_MML_GetBgSearchCfg();

    pstBgSearchRegardlessMccCtx = NAS_MML_GetBgSearchRegardlessMccCtx();


    ulIsSameCountryFlg  = NAS_MML_ComparePlmnMcc(pstVPlmnId->ulMcc, pstHighPrioPlmnInfo->stPlmnId.ulMcc);

    /* ucSpecialRoamFlg�ر�ʱ,��������벻ͬ,����VOS_FALSE��
       ����ucSpecialRoamFlg��ʱ,plmn����Ϊplmn����ΪUPLMN��OPLMN,���������
       ��ͬ����VOS_FALSE,EHPLMN�޴�����*/

    if ((VOS_TRUE == pstBgSearchCfg->ucSpecialRoamFlg)
     && (NAS_MMC_PLMN_TYPE_HPLMN == pstHighPrioPlmnInfo->enPlmnType))
    {
        return VOS_TRUE;
    }

    /* ��ǰ�������ڶ��Ƶ�BG�Ѳ����ǹ�������б��� */
    if (VOS_TRUE == NAS_MML_IsMccInDestMccList(pstVPlmnId->ulMcc, pstBgSearchRegardlessMccCtx->ucCustomMccNum, pstBgSearchRegardlessMccCtx->aulCostumMccList))
    {
        return VOS_TRUE;
    }

    return ulIsSameCountryFlg;
}
VOS_UINT32 NAS_MMC_CompareVPlmnPrioWithHighPrioPlmn(
    NAS_MML_PLMN_ID_STRU                                   *pstVPlmnId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enRat,
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEplmnList,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstHighPrioPlmnInfo
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulIsPlmnInEplmnList;
    VOS_UINT32                          ulIsPlmnRatSupported;
    VOS_UINT32                          ulIsEPlmnListValid;

    ulIsPlmnInEplmnList = VOS_FALSE;

    ulIsEPlmnListValid  = VOS_FALSE;

    /* �ж�EPLMN�б��Ƿ�ΪVPLMN�ĵ�ЧPLMN�б�,����֮ǰע��ɹ�,��ѡ�������ֹ����,��ʱEPLMN�б�Ϊ��ǰ�����EPLMN�б� */
    if (pstEplmnList->ucEquPlmnNum != 0)
    {
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstVPlmnId, &(pstEplmnList->astEquPlmnAddr[0])))
        {
            ulIsEPlmnListValid = VOS_TRUE;
        }
    }

    /* �жϸ����ȼ�����Ľ��뼼���Ƿ�֧��,��������ȼ�������EPLMN���ҽ��뼼��
       ��ǰ֧�֣��򷵻�VOS_TRUE,��ģʱEPLMN���ֽ��뼼�� */
    ulIsPlmnRatSupported = NAS_MMC_IsPlmnRatSupported(pstHighPrioPlmnInfo);

    /* ����23.122 4.4.3.3:In the case
    that the mobile has a stored "Equivalent PLMNs" list the mobile shall
    only select a PLMN if it is of a higher priority than those of the same
    country as the current serving PLMN which are stored in the "Equivalent
    PLMNs" list,��ʱ����Ҫ����,������ȼ��������Ѿ���EPLMN�б��з���VOS_TRUE,
    EPLMN�Ǵ������±�1��ʼ��ŵģ������±�0��ŵ���RPLMN */
    if (VOS_TRUE == ulIsEPlmnListValid)
    {
        ulIsPlmnInEplmnList = NAS_MML_IsSimPlmnIdInDestBcchPlmnList(&(pstHighPrioPlmnInfo->stPlmnId),
                                                         pstEplmnList->ucEquPlmnNum - 1,
                                                         &pstEplmnList->astEquPlmnAddr[1]);
    }

    if ((VOS_TRUE == ulIsPlmnInEplmnList)
     && (VOS_TRUE == ulIsPlmnRatSupported))
    {
        return VOS_TRUE;
    }

    /* h) If the PLMN of the highest priority PLMN/access technology combination
    available is the current VPLMN,or one of the PLMNs in the ""Equivalent PLMNs""
    list,the MS shall remain on the current PLMN/access technology combination. */
    /* ��ǰפ��������PLMN ID��������ȼ�������һ��,
       �Ҹø����ȼ�����֧�ֵĽ��뼼����ǰ֧�ַ���VOS_TRUE */
    if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstVPlmnId, &(pstHighPrioPlmnInfo->stPlmnId)))
    {
        /* ���VPLMN��HPLMN, ��ʹ��ǰ�Ľ��뼼����������ȼ���ͬҲ��Ϊ����������ȼ� */
        if (VOS_TRUE == NAS_MMC_IsCampOnHPlmn())
        {
            return VOS_TRUE;
        }

        for (i = 0; i < pstHighPrioPlmnInfo->ucRatNum; i++)
        {
            if (pstHighPrioPlmnInfo->astPlmnRatInfo[i].enRatType == enRat)
            {
                return VOS_TRUE;
            }
        }

        /* If the PLMN of the highest priority PLMN/access technology combination
          available is the current VPLMN,or one of the PLMNs in the ""Equivalent PLMNs"" list,
          the MS shall remain on the current PLMN/access technology combination.
          ��VPLMN��ͬ�������������ȼ���������ڵ�ǰ���绷���д��ڣ���VPLMN��������ȼ�������
          ��PLMN��ͬ�������磺VPLMNΪG��23002��USIM����������ȼ�����ΪW��23002��״̬
          ΪSEARCHED_EXIST������Ϊ��פ����������ȼ�������*/
        for (i = 0; i < pstHighPrioPlmnInfo->ucRatNum; i++)
        {
            if ((VOS_TRUE ==  NAS_MML_IsNetRatSupported(pstHighPrioPlmnInfo->astPlmnRatInfo[i].enRatType)
             && (NAS_MMC_NET_STATUS_SEARCHED_EXIST == pstHighPrioPlmnInfo->astPlmnRatInfo[i].enNetStatus)))
            {
                return VOS_TRUE;
            }
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_GetNextSearchHighPrioPlmn(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstHighPrioPlmnSearchListInfo,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstHighPrioPlmnInfo,
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstVPlmn,
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEplmnList
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    VOS_UINT32                                              k;
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList = VOS_NULL_PTR;
    VOS_UINT32                                              ulIsPlmnSearchingAllowed;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnSelectionPlmnInfo = VOS_NULL_PTR;
    VOS_UINT32                                              ulCampHplmnFlg;

    ulCampHplmnFlg              = NAS_MML_ComparePlmnIdWithHplmn(&pstVPlmn->stPlmnId);

    /* ��ȡ��ǰ���뼼�����ȼ���Ϣ */
    pstPrioRatList = NAS_MML_GetMsPrioRatList();

    for ( i = 0 ; i < pstHighPrioPlmnSearchListInfo->usSearchPlmnNum; i++ )
    {
        pstPlmnSelectionPlmnInfo = &(pstHighPrioPlmnSearchListInfo->astPlmnSelectionList[i]);

        /* ��PLMN��VPLMN�������Ƿ���ͬ,�������ͬ,�򲻽������� */
        if ( VOS_FALSE == NAS_MMC_IsHighPrioPlmnSameCountry(pstPlmnSelectionPlmnInfo,
                                                                 &pstVPlmn->stPlmnId))
        {
            continue;
        }

        /* ��ǰVPLMN��HPLMN,HPLMN��BG�ѱȽ�����,��Ҫ���⴦�� */
        if (VOS_TRUE == ulCampHplmnFlg)
        {
            /* ��ȡ���ĸ����ȼ�PLMN����HPLMN������ */
            if (VOS_FALSE == NAS_MML_ComparePlmnIdWithHplmn(&pstPlmnSelectionPlmnInfo->stPlmnId))
            {
                continue;   
            }
        }
        else
        {
            /* �Ǹ����ȼ�RAT��HPLMN����ʱ,�Ƚ�VPLMN�͸����ȼ��������ȼ��Ƿ���ͬ�������ͬ����VOS_FALSE */
            if (VOS_TRUE == NAS_MMC_CompareVPlmnPrioWithHighPrioPlmn(&pstVPlmn->stPlmnId, 
                                                                      pstVPlmn->enRat, pstEplmnList, pstPlmnSelectionPlmnInfo))
            {
                return VOS_FALSE;
            }
        }


        /* ��SYSCFG���õĽ��뼼�����ȼ����β���,����Ҫ�����ĵ�ǰ�ĸ����ȼ�PLMNʱ���˳� */
        for (k = 0; k < pstPrioRatList->ucRatNum; k++)
        {
            /* ����SYSCFG���ú͸�Plmn֧�ֵĽ��뼼����ȡ��plmn Id�ͽ��뼼���Լ�����״̬ */
            for (j = 0; j < pstPlmnSelectionPlmnInfo->ucRatNum; j++)
            {
                /* ������뼼��֧����״̬��Ϊδ�����������Ѵ��� */
                if (pstPrioRatList->aucRatPrio[k] == pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enRatType)
                {
                    ulIsPlmnSearchingAllowed = NAS_MMC_IsPlmnSearchingAllowed_PlmnSelectionStrategy(&(pstPlmnSelectionPlmnInfo->stPlmnId),
                                                                                         pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enRatType,
                                                                                         pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enNetStatus,
                                                                                         VOS_FALSE);
                    if (VOS_FALSE == ulIsPlmnSearchingAllowed)
                    {
                        /* �洢�Ľ��뼼���������ظ� */
                        break;
                    }

                    /* פ��HPLMN���һ�ȡ���ĸ����ȼ�RAT��VPLMN��RATһ����������ȡ�����ȼ������б��е��¸�������PLMN */
                    /* ֱ���ڹ��������б�ʱ���� */

                    pstHighPrioPlmnInfo->stPlmnId                      = pstHighPrioPlmnSearchListInfo->astPlmnSelectionList[i].stPlmnId;
                    pstHighPrioPlmnInfo->ucRatNum                      = 1;
                    pstHighPrioPlmnInfo->astPlmnRatInfo[0].enRatType   = pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enRatType;
                    pstHighPrioPlmnInfo->astPlmnRatInfo[0].enNetStatus = pstPlmnSelectionPlmnInfo->astPlmnRatInfo[j].enNetStatus;

                    /* ��ȡ�ɹ�����VOS_TRUE */
                    return VOS_TRUE;

                }
            }
        }

    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_GetHighestPrioPlmnInHighPrioPlmnList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstHighPrioPlmnInfo
)
{
    VOS_UINT32                          i;
    NAS_MML_PLMN_ID_STRU               *pstCurrCampPlmnId = VOS_NULL_PTR;

    /* ȡ�õ�ǰפ����PLMN ID */
    pstCurrCampPlmnId = NAS_MML_GetCurrCampPlmnId();

    for ( i = 0 ; i < pstPlmnSelectionListInfo->usSearchPlmnNum; i++ )
    {
        /* ֻȡ��������ͬ��PLMN */
        if (VOS_FALSE == NAS_MML_ComparePlmnMcc(pstCurrCampPlmnId->ulMcc, pstPlmnSelectionListInfo->astPlmnSelectionList[i].stPlmnId.ulMcc))
        {
            continue;
        }

        /* ���������ȼ�������뼼����֧��,����ȡ��һ�� */
        if (VOS_FALSE == NAS_MMC_IsPlmnRatSupported(&pstPlmnSelectionListInfo->astPlmnSelectionList[i]))
        {
            continue;
        }

        PS_MEM_CPY(pstHighPrioPlmnInfo, &pstPlmnSelectionListInfo->astPlmnSelectionList[i], sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));

        return VOS_TRUE;
    }

    return VOS_FALSE;
}
VOS_VOID  NAS_MMC_DelSpecPlmnWithRatInPlmnList (
    NAS_MML_PLMN_ID_STRU                   *pstPlmnId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8         enRat,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU  *pstPlmnSelectionListInfo
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          j;

    /* ɾ���ظ���PLMN */
    for (i = 0; i < pstPlmnSelectionListInfo->usSearchPlmnNum; )
    {
        for (j = 0; j < pstPlmnSelectionListInfo->astPlmnSelectionList[i].ucRatNum; )
        {
            if ((VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstPlmnId,
                                                               &(pstPlmnSelectionListInfo->astPlmnSelectionList[i].stPlmnId)))
             && (enRat == pstPlmnSelectionListInfo->astPlmnSelectionList[i].astPlmnRatInfo[j].enRatType))
            {
                /* ɾ����Ӧ��RAT */
                PS_MEM_MOVE(&(pstPlmnSelectionListInfo->astPlmnSelectionList[i].astPlmnRatInfo[j]),
                            &(pstPlmnSelectionListInfo->astPlmnSelectionList[i].astPlmnRatInfo[j + 1]),
                            ((pstPlmnSelectionListInfo->astPlmnSelectionList[i]).ucRatNum - j - 1) * sizeof(NAS_MMC_PLMN_RAT_INFO_STRU));

                pstPlmnSelectionListInfo->astPlmnSelectionList[i].ucRatNum--;

            }
            else
            {
                j++;
            }
        }

        /* ���뼼������Ϊ0ʱ��ɾ����Ӧ��PLMN ID */
        if (0 == pstPlmnSelectionListInfo->astPlmnSelectionList[i].ucRatNum)
        {
            PS_MEM_MOVE(&(pstPlmnSelectionListInfo->astPlmnSelectionList[i]),
                        &(pstPlmnSelectionListInfo->astPlmnSelectionList[i + 1]),
                        (pstPlmnSelectionListInfo->usSearchPlmnNum - i - 1) * sizeof(NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU));

            pstPlmnSelectionListInfo->usSearchPlmnNum--;
        }
        else
        {
            i++;
        }
    }
}
VOS_VOID NAS_MMC_AddSearchedPlmnListInfoInPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    NAS_MML_PLMN_RAT_PRIO_STRU                              stPrioRatList;

    if (VOS_NULL_PTR == pstSearchedPlmnListInfo)
    {
        return;
    }

    /* ���������AREA_LOST��Ϣ���ϱ�������ģ���������ѡ���б� */
    for (i = 0; i < NAS_MML_MAX_RAT_NUM; i++)
    {
        if (NAS_MML_NET_RAT_TYPE_BUTT == pstSearchedPlmnListInfo[i].enRatType)
        {
            continue;
        }

        stPrioRatList.ucRatNum      = 1;
        stPrioRatList.aucRatPrio[0] = pstSearchedPlmnListInfo[i].enRatType;

        /* ���������������ѡ���б� */
        for (j = 0; j < pstSearchedPlmnListInfo[i].ulHighPlmnNum; j++)
        {
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstSearchedPlmnListInfo[i].astHighPlmnList[j]),
                                                   NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                                   &stPrioRatList,
                                                   pstPlmnSelectionListInfo);
        }

        /* ���������������ѡ���б� */
        for (j = 0; j < pstSearchedPlmnListInfo[i].ulLowPlmnNum; j++)
        {
            NAS_MMC_AddSpecPlmnInPlmnSelectionList(&(pstSearchedPlmnListInfo[i].astLowPlmnList[j].stPlmnId),
                                                   NAS_MMC_PLMN_TYPE_AVAIL_PLMN,
                                                   &stPrioRatList,
                                                   pstPlmnSelectionListInfo);
        }
    }

    return;
}
VOS_VOID NAS_MMC_GetSpecRatCoverageTypeInSearchedPlmnList(
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enRatType,
    NAS_MMC_COVERAGE_TYPE_ENUM_UINT8                       *penCoverType
)
{
    VOS_UINT32                                              i;

    /* ���������AREA_LOST��Ϣ���ϱ�������ģ���������ѡ���б� */
    for (i = 0; i < NAS_MML_MAX_RAT_NUM; i++)
    {
        if (enRatType == pstSearchedPlmnListInfo[i].enRatType)
        {
            *penCoverType = pstSearchedPlmnListInfo[i].enCoverType;
            break;
        }
    }

    return;
}
NAS_MMC_COVERAGE_TYPE_ENUM_UINT8 NAS_MMC_GetSpecRatCoverageTypeInRatSearchInfoList(
    NAS_MMC_RAT_SEARCH_INFO_STRU                           *pstSearchRatInfo,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                         enRatType
)
{
    VOS_UINT32                                              i;

    /* ���������AREA_LOST��Ϣ���ϱ�������ģ���������ѡ���б� */
    for (i = 0; i < NAS_MML_MAX_RAT_NUM; i++)
    {
        if (enRatType == pstSearchRatInfo[i].enRatType)
        {
            return pstSearchRatInfo[i].enCoverageType;
        }
    }

    return NAS_MMC_COVERAGE_TYPE_BUTT;
}


VOS_UINT8  NAS_MMC_IsNeedNcellSearch(VOS_VOID)
{
    VOS_UINT32                          ulCount;
    VOS_UINT32                          ulCurNcellSearchTimerCount;
    VOS_UINT32                          ulTotalNcellSearchTimerCount;
    VOS_UINT8                           ucNcellSearchFlag;
    NAS_MML_AVAIL_TIMER_CFG_STRU       *pstAvailTimerCfg   = VOS_NULL_PTR;
    VOS_UINT8                           ucNcellSearchTimerLen;

    VOS_UINT8                           ucPsAttachAllowFlag;
    VOS_UINT8                           ucCsAttachAllowFlag;

    ulCount                      = NAS_MMC_GetCurNormalAvailableTimerCount_L1Main();
    ulCurNcellSearchTimerCount   = NAS_MMC_GetCurNcellSearchTimerCount_OnPlmn();
    ucNcellSearchFlag            = NAS_MMC_GetNcellSearchFlag();
    pstAvailTimerCfg             = NAS_MML_GetAvailTimerCfg();
    ulTotalNcellSearchTimerCount = 0;
    ucPsAttachAllowFlag          = NAS_MML_GetPsAttachAllowFlg();
    ucCsAttachAllowFlag          = NAS_MML_GetCsAttachAllowFlg();

    /* �����ǰ�����Ĵ���������available ��ʱ���ĵ�һ�δ���,ʹ�õ�һ��ʱ��,����ʹ�õڶ��ε� */
    if (ulCount < pstAvailTimerCfg->ulFirstSearchTimeCount)
    {
        ucNcellSearchTimerLen        = NAS_MMC_GetNcellSearchFirstTimerLen();

        if (0 != ucNcellSearchTimerLen)
        {
            ulTotalNcellSearchTimerCount = ((pstAvailTimerCfg->ulFirstSearchTimeLen) / ucNcellSearchTimerLen);
        }
    }
    else
    {
        ucNcellSearchTimerLen        = NAS_MMC_GetNcellSearchSecondTimerLen();

        if (0 != ucNcellSearchTimerLen)
        {
            ulTotalNcellSearchTimerCount = ((pstAvailTimerCfg->ulDeepSearchTimeLen) / ucNcellSearchTimerLen);
        }
    }

    /* 1��svlte ���Կ���
       2��֧��ncell��������nv����
       3��modem1 gas�ϱ�����t��l������Ƶ����Ϣ����������Ϊ0ʱҲ������ncell�Ѷ�ʱ��
       4����ǰΪPS attach not allow/cs attach not allow,��ֹ������������ù���
       5��ncell��������δ�ﵽn(��M�׶ε��������ʱ��/ NcellQuickSearchTimer)��
          ��available��ʱ��ʱ������Ϊncell����ʱ�� */
    if ((VOS_TRUE == ucNcellSearchFlag)
     && (ulCurNcellSearchTimerCount < ulTotalNcellSearchTimerCount)
     && (VOS_FALSE == ucPsAttachAllowFlag)
     && (VOS_FALSE == ucCsAttachAllowFlag))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


#if (FEATURE_ON == FEATURE_LTE)
VOS_UINT32 NAS_MMC_IsLtePlmnSearchingAllowed_PlmnSelectionStrategy(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLCapabilityStatus;
    VOS_UINT32                                              ulEnableRoaming;
    VOS_UINT32                                              aulComparedMccList[NAS_MML_ROAM_ENABLED_MAX_MCC_NUMBER];
    VOS_UINT32                                              ulState;
    NAS_MML_SIM_TYPE_ENUM_UINT8                             enSimType;

    enSimType = NAS_MML_GetSimType();

    ulEnableRoaming   = VOS_TRUE;

    /* ��ֹLTE��������ʱ */
    if (VOS_FALSE == NAS_MML_GetLteRoamAllowedFlg())
    {
        NAS_MML_GetRoamEnabledMccList(aulComparedMccList);

        /* �жϸ�PLMN ID��MCC�Ƿ�������פ����MCC�б��� */
        ulEnableRoaming = NAS_MML_IsMccInDestMccList(pstPlmnId->ulMcc, NAS_MML_ROAM_ENABLED_MAX_MCC_NUMBER, aulComparedMccList);
    }

    /* ��ȡLTE����״̬ */
    enLCapabilityStatus = NAS_MML_GetLteCapabilityStatus();

    /* ������������,����Ҫ����LTE������:
        1)��ǰ����Ľ��뼼��ΪL�ҵ�ǰPS����Ч
        2)��ǰ����Ľ��뼼��ΪL�ҵ�ǰDISABLE LTE
        3)LTE��ֹ��������
        4)������ΪSIM��:��Ӧ�ĳ���Ϊ:SIM��ʱ���ó�LTE only����ȥ��Lģ������ʱ����ʱ����Lģ
    */
    if ((VOS_FALSE                             == NAS_MML_GetSimPsRegStatus())
     || (VOS_FALSE == ulEnableRoaming)
     || (NAS_MML_SIM_TYPE_SIM == enSimType))
    {
        return VOS_FALSE;
    }

    ulState = NAS_MMC_GetFsmTopState();    
   
    if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLCapabilityStatus)
     || (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == enLCapabilityStatus))
    { 
        /* BG���������л�ȡ�¸������ȼ�����ʱ������disable Lģ������ */
        if ((NAS_MMC_FSM_BG_PLMN_SEARCH                             == NAS_MMC_GetCurrFsmId())
         && ((NAS_MMC_BG_PLMN_SEARCH_STA_INIT                        == ulState)
          || (NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_WAS_BG_PLMN_SEARCH_CNF == ulState)
          || (NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_GAS_BG_PLMN_SEARCH_CNF == ulState)
          || (NAS_MMC_BG_PLMN_SEARCH_STA_WAIT_LMM_BG_PLMN_SEARCH_CNF == ulState)))
        {
            /* PS��������ע��,����Ҫ���� Enable L */
            if (VOS_FALSE == NAS_MML_GetPsAttachAllowFlg())
            {
                return VOS_FALSE;
            }

            /* �������ʱ�Ѿ�����en_NV_Item_Rat_Forbidden_List_Accord_Imsi_Config�����ý�ֹ��LTE���򷵻�VOS_FALSE */
            if (NAS_MML_RAT_CAPABILITY_STATUS_DISABLE == NAS_MML_GetLteForbiddenStatusFlg())
            {
                return VOS_FALSE;
            }
            
            return VOS_TRUE;
        }
    
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

#endif
VOS_VOID NAS_MMC_InitPlmnSelectionList_HighPrioRatHplmnSearch(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstHighPrioPlmnSearchListInfo,
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                   *pstSearchedPlmnListInfo
)
{
    NAS_MML_SIM_EHPLMN_INFO_STRU                           *pstEHPlmnInfo = VOS_NULL_PTR;
    VOS_UINT32                                              i;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo;

    NAS_MMC_SetPlmnSelectionListType(NAS_MMC_STORE_HIGH_PRIO_PLMN_SELECTION_LIST);

    PS_MEM_SET(&stPlmnInfo, 0x0, sizeof(stPlmnInfo));
        
    /* ��ȡEHPLMN */
    pstEHPlmnInfo = NAS_MML_GetSimEhplmnList();


    for (i = 0; i< pstEHPlmnInfo->ucEhPlmnNum; i++)
    {
        /* ����SIM���Ľ��뼼����Ϣ������ID���������͹���ѡ���б���PLMN ID��Ϣ */
        NAS_MMC_BuildPlmnSelectionPlmnInfo_ForHighPrioRatHplmnSearch(NAS_MMC_PLMN_TYPE_HPLMN,
                                           &(pstEHPlmnInfo->astEhPlmnInfo[i].stPlmnId),
                                           &stPlmnInfo);

        /* ��ӵ������б� */
        if (VOS_FALSE == NAS_MMC_IsPlmnInSearchList(stPlmnInfo, pstHighPrioPlmnSearchListInfo)) 
        {
            NAS_MMC_AppendPlmnInPlmnSelectionList(&stPlmnInfo, pstHighPrioPlmnSearchListInfo);
        }
    }

    return;
}



VOS_UINT32 NAS_MMC_IsPlmnInSearchList(
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                   stPlmnInfo,
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstHighPrioPlmnSearchListInfo
)
{
    VOS_UINT32                          ulPlmnNum;
    NAS_MML_PLMN_ID_STRU                stPlmnId;
    
    for (ulPlmnNum = 0; ulPlmnNum < pstHighPrioPlmnSearchListInfo->usSearchPlmnNum; ulPlmnNum++)
    {
        PS_MEM_CPY(&stPlmnId, &(pstHighPrioPlmnSearchListInfo->astPlmnSelectionList[ulPlmnNum].stPlmnId), sizeof(stPlmnId));
        if ((stPlmnId.ulMcc == stPlmnInfo.stPlmnId.ulMcc) 
         && (stPlmnId.ulMnc == stPlmnInfo.stPlmnId.ulMnc))
        {
            return VOS_TRUE;
        }
    }
    return VOS_FALSE;
}


VOS_VOID NAS_MMC_BuildPlmnSelectionPlmnInfo_ForHighPrioRatHplmnSearch(
    NAS_MMC_PLMN_TYPE_ENUM_UINT8                            enPlmnType,
    NAS_MML_PLMN_ID_STRU                                   *pstPlmnId,
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnInfo
)
{
    VOS_UINT8                           ucRatNum;
    VOS_UINT8                           i;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstPlmnRatPrio = VOS_NULL_PTR;

    ucRatNum                = 0x0;
    pstPlmnRatPrio = NAS_MML_GetMsPrioRatList();

    PS_MEM_CPY(&pstPlmnInfo->stPlmnId, pstPlmnId, sizeof(NAS_MML_PLMN_ID_STRU));
    pstPlmnInfo->enPlmnType = enPlmnType;

    for (i = 0; i < pstPlmnRatPrio->ucRatNum; i++) 
    {
        /* ��������ȼ����뼼��HPLMN�б�ʱ��ֱ�ӽ����ȼ��ȵ�ǰ���뼼���͵Ľ��뼼�����˵� */
        if ( pstPlmnRatPrio->aucRatPrio[i] == NAS_MML_GetCurrNetRatType() )
        {
            break;
        }

        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enRatType   = pstPlmnRatPrio->aucRatPrio[i];
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enNetStatus = NAS_MMC_NET_STATUS_NO_SEARCHED;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enQuality   = NAS_MMC_NET_QUALITY_UNKNOWN;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].lRscp       = NAS_MML_UTRA_RSCP_UNVALID;
        ucRatNum++;

    }

    pstPlmnInfo->ucRatNum = ucRatNum;
    /* ����ʣ��λ�ø�ֵΪ��Чֵ */
    for (; ucRatNum < NAS_MML_MAX_RAT_NUM; ucRatNum++)
    {
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enRatType   = NAS_MML_NET_RAT_TYPE_BUTT;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enNetStatus = NAS_MMC_NET_STATUS_BUTT;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].enQuality   = NAS_MMC_NET_QUALITY_BUTT;
        pstPlmnInfo->astPlmnRatInfo[ucRatNum].lRscp       = NAS_MML_UTRA_RSCP_UNVALID;
    }

    return;
}
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


