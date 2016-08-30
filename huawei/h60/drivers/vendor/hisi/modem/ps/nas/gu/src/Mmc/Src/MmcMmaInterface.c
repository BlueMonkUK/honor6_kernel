


/*****************************************************************************
   1 ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "MM_Share.h"
#include "MmcMmaItf.h"
#include "MmaAppLocal.h"
#include "NasMmcSndMma.h"
#include "NasMmcSndGuAs.h"
#include "NasMmlLib.h"
#include "NasMmcProcUsim.h"
#include "NasMmcSndOm.h"

#include "NasUtranCtrlInterface.h"

#include "NasMmcProcNvim.h"


#ifdef  __cplusplus
  #if  __cplusplus
  extern "C"{
  #endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_MMC_MMAINTERFACE_C



/*****************************************************************************
   2 ȫ�ֱ�������
*****************************************************************************/

extern GMM_GLOBAL_CTRL_STRU          g_GmmGlobalCtrl;

/*****************************************************************************
   3 ����ʵ��
*****************************************************************************/



VOS_UINT32 Api_MmaGetParaFromMmc(MMC_MMA_SHARE_PARA_ST *pstMmcPara)
{
    /* ������Ч���ж� */
    if ( VOS_NULL_PTR == pstMmcPara )
    {
        /* ָ��Ϊ��ָ�룬���ش��� */
        NAS_WARNING_LOG(WUEPS_PID_MMC,"Api_MmaGetParaFromMmc():WARNING:NULL PTR ");
        return VOS_FALSE;
    }

    if ( pstMmcPara->enMmaParaType >= EN_MMC_BEGIN_SET_MMC_PARA )
    {
        /* ���Ͳ�������,���ش���ֵ */
        NAS_WARNING_LOG(WUEPS_PID_MMC,"Api_MmaGetParaFromMmc():WARNING:PARA TYPE WRONG");
        return VOS_FALSE;
    }

    switch ( pstMmcPara->enMmaParaType )
    {

        case EN_MMC_OPLMN_INFO:
            NAS_MMC_GetOperPlmnInfoForMma(&(pstMmcPara->u.stOPlmnInfo));
            break;

        case EN_MMC_UPLMN_INFO:
            NAS_MMC_GetUserPlmnInfoForMma(&(pstMmcPara->u.stUPlmnInfo));
            break;

        case EN_MMC_HPLMN_INFO:
            NAS_MMC_GetHPlmnInfoForMma(&(pstMmcPara->u.stHPlmnInfo));
            break;


        /* PLMNSEL �� UPLMN ֻ���ܳ���һ�����ʶ����� stUPlmnInfo �� */
        case EN_MMC_PLMNSEL_INFO:
            NAS_MMC_GetSelPlmnInfoForMma(&(pstMmcPara->u.stUPlmnInfo));
            break;

        default:
             /* ��ӡ������Ϣ,���س����־ */
             NAS_WARNING_LOG(WUEPS_PID_MMC,"Api_MmaGetParaFromMmc():WARNING:PARA TYPE WRONG IN DEFAULT");
            return VOS_FALSE;
    }

    return VOS_TRUE;
}
VOS_UINT32 Api_MmaSetParaFromMmc(
    const MMC_MMA_SHARE_PARA_ST        *pstMmcPara
)
{
    VOS_UINT32                          ulI = 0;
    NAS_MML_SIM_SELPLMN_INFO_STRU      *pstSelPlmn  = VOS_NULL_PTR;
    NAS_MML_SIM_USERPLMN_INFO_STRU     *pstUserPlmn = VOS_NULL_PTR;

    pstUserPlmn = NAS_MML_GetSimUserPlmnList();
    pstSelPlmn  = NAS_MML_GetSimSelPlmnList();

    if ( VOS_NULL_PTR == pstMmcPara )
    {
        /* ָ��Ϊ��ָ�룬���ش��� */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "Api_MmaSetParaFromMmc():WARNING:Set Para Point is NULL");
        return VOS_ERR;
    }

    if ( (pstMmcPara->enMmaParaType < EN_MMC_BEGIN_SET_MMC_PARA)
      || (pstMmcPara->enMmaParaType > EN_MMC_END_SET_MMC_PARA))
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "Api_MmaSetParaFromMmc():WARNING:PARA TYPE WRONG");
        return VOS_ERR;
    }

    switch ( pstMmcPara->enMmaParaType)
    {


        case EN_MMC_SET_UPLMN_INFO:
             pstUserPlmn->ucUserPlmnNum      = (VOS_UINT8)pstMmcPara->u.stUPlmnInfo.usCurPlmnNum;
             for ( ulI = 0; ulI < pstMmcPara->u.stUPlmnInfo.usCurPlmnNum ; ulI++ )
             {
                 pstUserPlmn->astUserPlmnInfo[ulI].stPlmnId.ulMcc = pstMmcPara->u.stUPlmnInfo.astPlmnInfo[ulI].stPlmn.ulMcc;
                 pstUserPlmn->astUserPlmnInfo[ulI].stPlmnId.ulMnc = pstMmcPara->u.stUPlmnInfo.astPlmnInfo[ulI].stPlmn.ulMnc;
                 pstUserPlmn->astUserPlmnInfo[ulI].usSimRat       = pstMmcPara->u.stUPlmnInfo.astPlmnInfo[ulI].usRaMode;
             }

             break;

        case  EN_MMC_SET_PLMNSEL_INFO:
            pstSelPlmn->ucSelPlmnNum    = (VOS_UINT8)pstMmcPara->u.stUPlmnInfo.usCurPlmnNum;
            for ( ulI = 0; ulI < pstMmcPara->u.stUPlmnInfo.usCurPlmnNum ; ulI++ )
            {
                pstSelPlmn->astPlmnId[ulI].ulMcc = pstMmcPara->u.stUPlmnInfo.astPlmnInfo[ulI].stPlmn.ulMcc;
                pstSelPlmn->astPlmnId[ulI].ulMnc = pstMmcPara->u.stUPlmnInfo.astPlmnInfo[ulI].stPlmn.ulMnc;
            }

            break;

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "Api_MmaSetParaFromMmc():WARNING:PARA TYPE WRONG IN DEFAULT");
            return VOS_ERR;

    }
    return VOS_OK;
}
VOS_VOID MMC_FplmnQurey(
    VOS_UINT16                          ClientId,
    VOS_UINT8                           OpId
)
{
    TAF_USER_PLMN_LIST_STRU            *pstPlmnList   = VOS_NULL_PTR;
    VOS_UINT8                           ucForbPlmnNum = 0;
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU   *pstForbidPlmnList = VOS_NULL_PTR;

    pstPlmnList = (TAF_USER_PLMN_LIST_STRU*)PS_MEM_ALLOC(WUEPS_PID_MMC, sizeof(TAF_USER_PLMN_LIST_STRU));

    if (VOS_NULL_PTR == pstPlmnList)
    {
        MMA_ParaQueryReport ( ClientId, OpId, TAF_PH_FPLMN_PARA,TAF_ERR_NULL_PTR, VOS_NULL_PTR);
        return;
    }

    PS_MEM_SET(pstPlmnList, 0x00, sizeof(TAF_USER_PLMN_LIST_STRU));

    /* ��ȡ��ֹ�б��� */
    pstForbidPlmnList           = NAS_MML_GetForbidPlmnInfo();
    pstPlmnList->usPlmnNum      = (VOS_UINT16)pstForbidPlmnList->ucForbPlmnNum;

    if (pstPlmnList->usPlmnNum > NAS_MML_MAX_FORBPLMN_NUM)
    {
        pstPlmnList->usPlmnNum = NAS_MML_MAX_FORBPLMN_NUM;
    }

    /* ��ȡ��ֹ�б�PLMN */
    for ( ucForbPlmnNum = 0; ucForbPlmnNum < pstPlmnList->usPlmnNum; ucForbPlmnNum++ )
    {
        pstPlmnList->Plmn[ucForbPlmnNum].Mcc = pstForbidPlmnList->astForbPlmnIdList[ucForbPlmnNum].ulMcc;
        pstPlmnList->Plmn[ucForbPlmnNum].Mnc = pstForbidPlmnList->astForbPlmnIdList[ucForbPlmnNum].ulMnc;

        /* PLMN��ʽת�� */
        MMA_PlmnId2Bcd ( &pstPlmnList->Plmn[ucForbPlmnNum] );
    }

    /* ʹ�������ϱ����������ݴ���TAF */
    MMA_ParaQueryReport( ClientId, OpId, TAF_PH_FPLMN_PARA, TAF_ERR_NO_ERROR, (VOS_VOID*)pstPlmnList);

    PS_MEM_FREE(WUEPS_PID_MMC, pstPlmnList);
}


VOS_VOID MMC_FplmnDel(
    VOS_UINT16                          ClientId,
    VOS_UINT8                           OpId,
    TAF_PH_FPLMN_OPERATE_STRU          *pstFplmnOperate
)
{
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU   *pstForbidPlmnList = VOS_NULL_PTR;
    VOS_UINT32                          ulChgFlag;

    ulChgFlag            = VOS_FALSE;

    /* ��ȡ��Ҫɾ����PLMN */
    pstForbidPlmnList   = NAS_MML_GetForbidPlmnInfo();

    /* �����ɾ�����н�ֹPLMN��ֱ�ӵ���ɾ�����н�ֹPLMN�����������ؽ�� */
    if (TAF_PH_FPLMN_DEL_ALL == pstFplmnOperate->ulCmdType)
    {
        if (0 != pstForbidPlmnList->ucForbPlmnNum)
        {
            pstForbidPlmnList->ucForbPlmnNum = 0;
            ulChgFlag = VOS_TRUE;
        }
    }
    else
    {
        ulChgFlag = NAS_MML_DelForbPlmn((NAS_MML_PLMN_ID_STRU*) (&(pstFplmnOperate->stPlmn)));
    }

    if (VOS_TRUE == ulChgFlag)
    {
        NAS_MMC_SndUsimUpdateForbPlmnReq();
    }

    Taf_ParaSetResult(ClientId, OpId, TAF_PARA_OK, TAF_PH_FPLMN_PARA, VOS_NULL_PTR);
}

VOS_VOID MMC_FplmnAdd(
    VOS_UINT16                          ClientId,
    VOS_UINT8                           OpId,
    TAF_PLMN_ID_STRU                   *pstPlmn
)
{
    VOS_UINT32                          ulChgFlag;

    /* ����forbidden plmn list�е�plmn����hplmn�ż���,�����ֹ���� */
    if (VOS_TRUE == NAS_MML_ComparePlmnIdWithHplmn((NAS_MML_PLMN_ID_STRU*)pstPlmn))
    {
        /* �ϱ����ý����*/
        Taf_ParaSetResult(ClientId, OpId, TAF_PARA_SET_ERROR, TAF_PH_FPLMN_PARA, VOS_NULL_PTR);
        return;
    }

    /* �����ֹ�б� */
    ulChgFlag = NAS_MML_AddForbPlmn((NAS_MML_PLMN_ID_STRU*)pstPlmn);

    if (VOS_TRUE ==  ulChgFlag)
    {
        NAS_MMC_SndUsimUpdateForbPlmnReq();
    }

    /* �ϱ����ý����*/
    Taf_ParaSetResult(ClientId, OpId, TAF_PARA_OK, TAF_PH_FPLMN_PARA, VOS_NULL_PTR);
}

VOS_VOID Api_MmaFplmnHandleFromMmc(
    MN_CLIENT_ID_T                      ClientId,
    MN_OPERATION_ID_T                   OpId,
    TAF_PH_FPLMN_OPERATE_STRU           stFplmnOperate
)
{
    /* �����������ͷַ����� */
    switch (stFplmnOperate.ulCmdType)
    {
    case TAF_PH_FPLMN_QUREY:
        MMC_FplmnQurey(ClientId, OpId);
        break;

    case TAF_PH_FPLMN_DEL_ALL:
    case TAF_PH_FPLMN_DEL_ONE:
        MMC_FplmnDel(ClientId, OpId, &stFplmnOperate);
        break;

    case TAF_PH_FPLMN_ADD_ONE:
        MMC_FplmnAdd(ClientId, OpId, &(stFplmnOperate.stPlmn));
        break;

    default:
        break;
    }
}
VOS_VOID Api_AppMmLayerContextSndoutForPcReplay(VOS_VOID)
{
    NAS_MML_CONN_STATUS_INFO_STRU      *pstConnStatus;
    VOS_UINT32                          ulRrcConnExistFlg;

    ulRrcConnExistFlg = NAS_MML_IsRrcConnExist();

    pstConnStatus     = NAS_MML_GetConnStatus();

    /* PS���е�ǰҵ�����, �򲻷��ͻط���Ϣ */
    if ( (VOS_TRUE == pstConnStatus->ucPdpStatusFlg)
      && (VOS_TRUE == ulRrcConnExistFlg) )
    {
        return;
    }

    /* UTRANCTRLģ��Ļط���Ϣ */
    NAS_UTRANCTRL_SndOutsideContextData();

    NAS_MMC_SndOutsideContextData();
    NAS_MM_SndOutsideContextData();
    NAS_GMM_SndOutsideContextData();
}


VOS_VOID NAS_MMC_GetCsPsSimStatus(
    VOS_BOOL                           *pbCsSimStatus,
    VOS_BOOL                           *pbPsSimStatus)
{
    if (VOS_TRUE == NAS_MML_GetSimCsRegStatus())
    {
        *pbCsSimStatus = VOS_TRUE;
    }
    else
    {
        *pbCsSimStatus = VOS_FALSE;
    }

    if (VOS_TRUE == NAS_MML_GetSimPsRegStatus())
    {
        *pbPsSimStatus = VOS_TRUE;
    }
    else
    {
        *pbPsSimStatus = VOS_FALSE;
    }

    return;
}


#ifdef  __cplusplus
  #if  __cplusplus
  }
  #endif
#endif

