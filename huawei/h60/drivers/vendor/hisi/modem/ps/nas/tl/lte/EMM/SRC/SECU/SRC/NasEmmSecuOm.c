


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include    "NasEmmSecuInclude.h"
#include    "NasEmmSecuInterface.h"
#include    "NasCommPrint.h"
#include    "MmcLmmInterface.h"
#include    "GmmLmmInterface.h"


/*lint -e7*/
/*lint -e322*/
#include  "DrvInterface.h"
/*lint +e322*/
/*lint +e7*/


/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASEMMSECUOM_C
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


/*lint -e960*/
/*lint -e961*/
VOS_VOID    NAS_EMM_SecuInit( VOS_VOID )
{

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuInit entered.");

    /* ���SECUģ���ȫ�ֿ��ƽṹ */
    NAS_LMM_MEM_SET(&g_NasEmmSecuControl, 0, sizeof(NAS_LMM_SECU_CONTROL_STRU));
    #if 0
    /* �����usim*/
    NAS_LMM_MEM_SET(NAS_EMM_GetSoftUsimAuthParamAddr(), 0, sizeof(NAS_EMM_SOFT_USIM_AUTH_PARAM_STRU));
    NAS_LMM_MEM_SET(&g_stUsimInfo, 0, sizeof(NAS_EMM_USIM_CONTROL_STRU));
    #endif
    /* ��ʼ����ǰ��ȫ�����ĺ��°�ȫ��������Ч*/
    NAS_EMM_SetSecuCurKSIsgsn(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);
    NAS_EMM_SetSecuCurKSIasme(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);
    NAS_EMM_SetSecuNewMappedKSIsgsn(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);
    NAS_EMM_SetSecuNewNativeKSIasme(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);
    NAS_EMM_SetSecuNonCurNativeKSIasme(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);

    /* ��ʼ��UMTS security context��Ч*/
    NAS_EMM_SetUmtsKsi(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);

    /* ��ʼ����ȫ״̬*/
    NAS_EMM_SecuStateConvert(           EMM_CUR_SECU_NOT_EXIST,
                                        EMM_NEW_SECU_NOT_EXIST);

    return;
}
VOS_VOID  NAS_EMM_FreeSecuDyn( VOS_VOID )
{
    /* ��ʱΪ�� */
    return;
}
VOS_VOID    NAS_Secu_FsmInit( VOS_VOID )
{

    NAS_LMM_FSM_STATE_STRU              *pSecuCurState;

    NAS_EMM_SECU_LOG_NORM("NAS_Secu_FsmInit                   START INIT...");

    /* ��ǰ״̬��ʼ�� */
    pSecuCurState = NAS_LMM_GetCurFsmAddr(NAS_LMM_PARALLEL_FSM_SECU);
    pSecuCurState->enFsmId               = NAS_LMM_PARALLEL_FSM_SECU;
    pSecuCurState->enMainState           = EMM_CUR_SECU_NOT_EXIST;
    pSecuCurState->enSubState            = EMM_NEW_SECU_NOT_EXIST;
    pSecuCurState->enStaTId              = NAS_LMM_STATE_TI_BUTT;

    return;
}
VOS_VOID  NAS_EMM_ConnToIdleTransSecuSta( VOS_VOID )
{

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_ConnToIdleTransSecuSta entered.");

    /*�����ǰ��ȫ�����Ĵ����Ҽ����״̬Ǩ�Ƶ�:EMM_CUR_SECU_EXIST_NOT_ACTIVE*/
    if(EMM_CUR_SECU_EXIST_ACTIVE == NAS_LMM_GetSecuCurFsmCS())
    {
        NAS_EMM_SecuStateConvert(       EMM_CUR_SECU_EXIST_NOT_ACTIVE,
                                        NAS_LMM_GetSecuCurFsmNS());
    }
    else
    {
       /*�����������ȫ������״̬����*/
    }

    return;
}


VOS_VOID  NAS_EMM_SecuSetCurCntxt( const NAS_LMM_SECU_CONTEXT_STRU *pstCurSecuCntxt )
{
    VOS_UINT32                          ulRslt;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuUpdateCurCntxt entered.");

    /* ���������İ�ȫ��������Ч */
    if(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE <= pstCurSecuCntxt->ucKSIasme)
    {
        NAS_EMM_SECU_LOG_WARN("NAS_EMM_SecuUpdateCurCntxt: KSI is unAvailable");
        return;
    }

    /*��ֵ����ǰ��ȫ������*/
    NAS_LMM_MEM_CPY(                    NAS_EMM_GetSecuCurCntxtAddr(),
                                        pstCurSecuCntxt,
                                        sizeof(NAS_LMM_SECU_CONTEXT_STRU));
    /*EPSϵͳ��KSIsgsnʼ����Ч*/
    NAS_EMM_GetSecuCurKSIsgsn()         = NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE;

    /*��������ɹ���ȡ��ǰ��ȫ�����ģ�����NasCk,NasIk*/
    ulRslt = NAS_EMM_ComputeNasKey(     NAS_EMM_GetSecuCurKasmeAddr(),
                                        NAS_EMM_GetSecuCurNasCkAddr(),
                                        NAS_EMM_SECU_CIPHER_KEY,
                                        NAS_EMM_GetSecuEca());
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuSetCurCntxt:Compute NasCK Err.");
    }

    ulRslt = NAS_EMM_ComputeNasKey(     NAS_EMM_GetSecuCurKasmeAddr(),
                                        NAS_EMM_GetSecuCurNasIkAddr(),
                                        NAS_EMM_SECU_INTEGRITY_KEY,
                                        NAS_EMM_GetSecuEia());
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuSetCurCntxt:Compute NasIK Err.");
    }

    /*���¼Ĵ����е�NAS��Կ*/
    /*NasIK*/
    (VOS_VOID)NAS_EMM_SecuSetKeyByKeyIndex(  NAS_EMM_GetSecuCurNasIkAddr(),
                                        NAS_EMM_SECU_INTEGRITY_KEY);
    /*NasCK*/
    (VOS_VOID)NAS_EMM_SecuSetKeyByKeyIndex(  NAS_EMM_GetSecuCurNasCkAddr(),
                                        NAS_EMM_SECU_CIPHER_KEY);

    /*��ȫ������״̬*/
    NAS_EMM_SecuStateConvert(           EMM_CUR_SECU_EXIST_NOT_ACTIVE,
                                        NAS_LMM_GetSecuCurFsmNS());

    return;

}


NAS_LMM_SECU_CONTEXT_STRU *  NAS_EMM_SecuGetCurCntxtAddr(VOS_VOID)
{
    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuGetCurCntxtAddr entered.");

    return NAS_EMM_GetSecuCurCntxtAddr();
}



VOS_VOID NAS_EMM_ClearCurSecuCntxt(VOS_VOID)
{
    VOS_UINT32                          ulRslt = NAS_EMM_SECU_SUCC;

    (VOS_VOID)ulRslt;
    NAS_EMM_SECU_LOG_INFO("NAS_EMM_ClearCurSecuCntxt entered.");

    /*��յ�ǰ��ȫ������MEM*/
    NAS_LMM_MEM_SET(NAS_EMM_GetSecuCurCntxtAddr(), 0, sizeof(NAS_LMM_SECU_CONTEXT_STRU));

    /*��ռĴ����е���Կ*/
    (VOS_VOID)NAS_EMM_SecuSetKeyByKeyIndex(  NAS_EMM_GetSecuCurNasIkAddr(),
                                        NAS_EMM_SECU_INTEGRITY_KEY);

    (VOS_VOID)NAS_EMM_SecuSetKeyByKeyIndex(  NAS_EMM_GetSecuCurNasCkAddr(),
                                        NAS_EMM_SECU_CIPHER_KEY);

    /*��ǰksi��Ч*/
    NAS_EMM_SetSecuCurKSIsgsn(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);
    NAS_EMM_SetSecuCurKSIasme(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);

    /*״̬*/
    NAS_EMM_SecuStateConvert(           EMM_CUR_SECU_NOT_EXIST,
                                        NAS_LMM_GetSecuCurFsmNS());
    return;
}


VOS_VOID  NAS_EMM_ClearNewMappedSecuCntxt( VOS_VOID )
{
    NAS_EMM_SECU_LOG_INFO("NAS_EMM_ClearNewMappedSecuCntxt entered.");

    /*��յ�ǰ��ȫ������MEM*/
    NAS_LMM_MEM_SET(NAS_EMM_GetSecuNewMappedCntxtAddr(), 0, sizeof(NAS_LMM_SECU_CONTEXT_STRU));

    /*��ǰksi��Ч*/
    NAS_EMM_SetSecuNewMappedKSIsgsn(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);

    return;
}


VOS_VOID  NAS_EMM_ClearNewNativeSecuCntxt( VOS_VOID )

{
    NAS_EMM_SECU_LOG_INFO("NAS_EMM_ClearNewNativeSecuCntxt entered.");

    /*��յ�ǰ��ȫ������MEM*/
    NAS_LMM_MEM_SET(NAS_EMM_GetSecuNewNativeCntxtAddr(), 0, sizeof(NAS_LMM_SECU_CONTEXT_STRU));

    /*��ǰksi��Ч*/
    NAS_EMM_SetSecuNewNativeKSIasme(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);

    return;
}


VOS_VOID  NAS_EMM_ClearCurMappedSecuCntxt( VOS_VOID )

{
    NAS_EMM_SECU_LOG_INFO("NAS_EMM_ClearCurMappedSecuCntxt entered.");

    /*��������� current mapped security context,��ֱ�ӷ���*/
    if(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE == NAS_EMM_GetSecuCurKSIsgsn())
    {
        return;
    }

    /*��յ�ǰ��ȫ������MEM*/
    NAS_LMM_MEM_SET(NAS_EMM_GetSecuCurCntxtAddr(), 0, sizeof(NAS_LMM_SECU_CONTEXT_STRU));

    /*����KSIsgsn��Ч*/
    NAS_EMM_SetSecuCurKSIasme(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);
    NAS_EMM_SetSecuCurKSIsgsn(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);

    return;
}
VOS_VOID  NAS_EMM_ClearNonCurNativeSecuCntxt( VOS_VOID )
{
    NAS_EMM_SECU_LOG_INFO("NAS_EMM_ClearNonCurNativeSecuCntxt entered.");

    /* ��������� non-current native security context,��ֱ�ӷ���*/
    if(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE == NAS_EMM_GetSecuNonCurNativeKSIasme())
    {
        return;
    }

    /* ��� non-current native security context*/
    NAS_LMM_MEM_SET(                    NAS_EMM_GetSecuNonCurNativeCntxtAddr(),
                                        0,
                                        sizeof(NAS_LMM_SECU_CONTEXT_STRU));
    /* ����KSIasme��Ч*/
    NAS_EMM_SetSecuNonCurNativeKSIasme(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);

    return;
}
VOS_VOID  NAS_EMM_ClearAllSecuCntxt( VOS_VOID )
{
    NAS_EMM_SECU_LOG_INFO("NAS_EMM_ClearAllSecuCntxt entered.");

    /* ���current security context*/
    NAS_EMM_ClearCurSecuCntxt();

    /* ���new mapped security context*/
    NAS_EMM_ClearNewMappedSecuCntxt();

    /* ���new native security context*/
    NAS_EMM_ClearNewNativeSecuCntxt();

    /* ���non-current security context*/
    NAS_EMM_ClearNonCurNativeSecuCntxt();

    /* ��ȫ������״̬*/
    NAS_EMM_SecuStateConvert(           EMM_CUR_SECU_NOT_EXIST,
                                        EMM_NEW_SECU_NOT_EXIST);
}
VOS_VOID  NAS_EMM_SecuDeregClrSecuCntxt( VOS_VOID )
{
    /* ɾ�� new mapped secu cntxt*/
    NAS_EMM_ClearNewMappedSecuCntxt();

    /* ɾ�� new native secu cntext*/
    NAS_EMM_ClearNewNativeSecuCntxt();

    /* ɾ�� current mapped secu cntxt*/
    NAS_EMM_ClearCurMappedSecuCntxt();

    /* ������� non-current native secu cntxt,
       ���仹ԭ�� current native secu cntxt,
       ��ɾ�� non-current native secu cntxt*/
    if(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE != NAS_EMM_GetSecuNonCurNativeKSIasme())
    {
        NAS_EMM_SecuRecovNonCurNativeToCurNative();
        NAS_EMM_ClearNonCurNativeSecuCntxt();
    }

    /* Ǩ�ư�ȫ�����ĵ�״̬*/
    if(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE != NAS_EMM_GetSecuCurKSIasme())
    {
        NAS_LMM_SetSecuCurFsmCS(EMM_CUR_SECU_EXIST_NOT_ACTIVE);
    }
    else
    {
        NAS_LMM_SetSecuCurFsmCS(EMM_CUR_SECU_NOT_EXIST);
    }

    NAS_LMM_SetSecuCurFsmNS(EMM_NEW_SECU_NOT_EXIST);

    return;
}
VOS_UINT8   NAS_EMM_SecuGetKSIValue( VOS_VOID )
{
    VOS_UINT8                           ucKsi = 0;

    /* �ж���� current KSIasme �� KSIsgsn����Ч���򷵻� NO_KEY*/
    if((NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE == NAS_EMM_GetSecuCurKSIasme())
     &&(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE == NAS_EMM_GetSecuCurKSIsgsn()))
    {
        NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuGetKSIValue: KSIasme and KSIsgsn are all invalid.");
        return NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE;
    }

    /* �ж� KSIasme��Ч�򷵻� KSIsgsn;��֮������ KSIasme*/
    if(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE == NAS_EMM_GetSecuCurKSIasme())
    {
        NAS_EMM_SECU_LOG1_INFO("NAS_EMM_SecuGetKSIValue: KSIsgsn valid, is ",
                                NAS_EMM_GetSecuCurKSIsgsn());
        ucKsi = (NAS_EMM_GetSecuCurKSIsgsn()|NAS_EMM_FOURTH_BITS_F);
    }
    else
    {
        NAS_EMM_SECU_LOG1_INFO("NAS_EMM_SecuGetKSIValue: KSIasme valid, is ",
                                NAS_EMM_GetSecuCurKSIasme());
        ucKsi = NAS_EMM_GetSecuCurKSIasme();
    }

    return ucKsi;

}


VOS_UINT32  NAS_EMM_SecuGetUmtsKsi( VOS_VOID )
{
    return NAS_EMM_GetUmtsKsi();
}


VOS_UINT32  NAS_EMM_SecuGetUlNasCountVAlue( VOS_VOID )
{
    return  NAS_EMM_GetSecuUlNasCount();
}


VOS_UINT8*  NAS_EMM_SecuGetNonceUE( VOS_VOID )
{
    return  NAS_EMM_GetSecuNonceUEAddr();
}


VOS_UINT8  NAS_EMM_SecuGetNonCurKSIasme( VOS_VOID )
{
   return NAS_EMM_GetSecuNonCurNativeKSIasme();
}



VOS_VOID NAS_EMM_SecuPrintCipherPara(CIHPER_SINGLE_CFG_S const *pstSingleSecuPara,
                                                VOS_UINT8  const *pucSrc)
{
    /*VOS_UINT8                          *pucNasKeyAddr = VOS_NULL_PTR;*/
    VOS_UINT8                           aucKeyBuf[NAS_LMM_SECU_NAS_KEY_LEN];
    KEY_GET_S                           stKeyGet;
    VOS_UINT32                          ulRet;
    CIPHER_KEY_LEN_E_U32                ulenKeyLen;

    NAS_EMM_SECU_LOG_INFO("======NAS SECU PRARM FOR CIPHER  are:======");

    NAS_EMM_SECU_LOG1_INFO("enOpType  = ", pstSingleSecuPara->enOpt);
    NAS_EMM_SECU_LOG1_INFO("enEia = ", pstSingleSecuPara->stAlgKeyInfo.enAlgIntegrity);
    NAS_EMM_SECU_LOG1_INFO("enEea = ", pstSingleSecuPara->stAlgKeyInfo.enAlgSecurity);
    NAS_EMM_SECU_LOG1_INFO("ulNasCount = ", pstSingleSecuPara->u32Count);
    NAS_EMM_SECU_LOG1_INFO("Direnction = ", pstSingleSecuPara->u8Direction);

    NAS_EMM_SECU_LOG1_INFO("enNasIkIndex = ", pstSingleSecuPara->stAlgKeyInfo.u32KeyIndexInt);

    stKeyGet.enKeyLen       = CIPHER_KEY_L128;
    stKeyGet.pKeyAddr       = aucKeyBuf;
    stKeyGet.penOutKeyLen   = &ulenKeyLen;

    ulRet = NAS_EMM_SecuGetKeyByIndex(LTE_SECU_NAS_INTEGRITY_KEY_INDEX, &stKeyGet);
    if(BSP_OK != ulRet)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuPrintCipherPara: err!! NasIk.");
        return;
    }

    NAS_EMM_SECU_LOG1_INFO("NasIK, Outlength = , key = :", ulenKeyLen);
    NAS_COMM_PrintArray(                NAS_COMM_GET_MM_PRINT_BUF(),
                                        stKeyGet.pKeyAddr,
                                        NAS_LMM_SECU_NAS_KEY_LEN);

    #if 0
    pucNasKeyAddr = NAS_EMM_SecuGetKeyByIndex(LTE_SECU_NAS_INTEGRITY_KEY_INDEX);
    if(VOS_NULL_PTR == pucNasKeyAddr)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuPrintCipherPara:NULL_NasIk.");
        return;
    }
    NAS_COMM_PrintArray(                NAS_COMM_GET_MM_PRINT_BUF(),
                                        pucNasKeyAddr,
                                        NAS_LMM_SECU_NAS_KEY_LEN);
    #endif


    NAS_EMM_SECU_LOG1_INFO("enNasCkIndex = ", pstSingleSecuPara->stAlgKeyInfo.u32KeyIndexSec);

    ulRet = NAS_EMM_SecuGetKeyByIndex(LTE_SECU_NAS_CIPHER_KEY_INDEX, &stKeyGet);
    if(BSP_OK != ulRet)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuPrintCipherPara:NULL_NasCk.");
        return;
    }

    NAS_EMM_SECU_LOG1_INFO("NasCK, Outlength = , key = :", ulenKeyLen);
    NAS_COMM_PrintArray(                NAS_COMM_GET_MM_PRINT_BUF(),
                                        stKeyGet.pKeyAddr,
                                        NAS_LMM_SECU_NAS_KEY_LEN);


    #if 0
    pucNasKeyAddr = NAS_EMM_SecuGetKeyByIndex(LTE_SECU_NAS_CIPHER_KEY_INDEX);
    if(VOS_NULL_PTR == pucNasKeyAddr)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuPrintCipherPara:NULL_NasCk.");
        return;
    }
    NAS_COMM_PrintArray(                NAS_COMM_GET_MM_PRINT_BUF(),
                                        pucNasKeyAddr,
                                        NAS_LMM_SECU_NAS_KEY_LEN);
    #endif

    NAS_EMM_SECU_LOG1_INFO("ulSrcLen  = ", pstSingleSecuPara->u32BlockLen);
    NAS_EMM_SECU_LOG_INFO("SrcMsg = ");
    NAS_COMM_PrintArray(NAS_COMM_GET_MM_PRINT_BUF(), pucSrc, pstSingleSecuPara->u32BlockLen);

    return;
}
VOS_UINT32  NAS_EMM_SecuSetKeyByKeyIndex( const VOS_UINT8 *pucKeyAddr,
                                    NAS_EMM_SECU_NAS_KEY_TYPE_ENUM_UINT8 enKeyType )
{
    VOS_INT32                           lRslt;
    VOS_UINT32                          ulKeyIndex;

    /* ��μ��*/
    if (VOS_NULL_PTR == pucKeyAddr)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuSetKeyByKeyIndex: NULL PTR.");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ������Կ���ͣ���ȡ����*/
    switch (enKeyType)
    {
        case NAS_EMM_SECU_INTEGRITY_KEY:
            ulKeyIndex = LTE_SECU_NAS_INTEGRITY_KEY_INDEX;
            break;

        case NAS_EMM_SECU_CIPHER_KEY:
            ulKeyIndex = LTE_SECU_NAS_CIPHER_KEY_INDEX;
            break;

        default:
            NAS_EMM_SECU_LOG_WARN("NAS_EMM_SecuSetKeyByKeyIndex: KeyType Err.");
            return NAS_EMM_SECU_FAIL;
    }

    /* ����BSP�ӿڣ���NAS��Կд��Ĵ���*/
    lRslt = BSP_CIPHER_SetKey(pucKeyAddr, CIPHER_KEY_L128, ulKeyIndex);
    if (CIPHER_SUCCESS != lRslt)
    {
        NAS_EMM_SECU_LOG1_ERR("NAS_EMM_SecuSetKeyByKeyIndex: BSP set Err.",ulKeyIndex);
        return NAS_EMM_SECU_FAIL;
    }

    return NAS_EMM_SECU_SUCC;
}
VOS_UINT32 NAS_EMM_SecuGetKeyByIndex(   VOS_UINT32      ulKeyIndex,
                                        KEY_GET_S      *pstKeyGet)
{
    BSP_S32 lRet;

    lRet = BSP_CIPHER_GetKey(  ulKeyIndex, pstKeyGet);
    NAS_EMM_SECU_LOG1_WARN("NAS_EMM_SecuGetKeyByIndex: BSP_CIPHER_GetKey, lRet = ",
                            lRet);

    return  (VOS_UINT32)lRet;
}

#if 0
VOS_UINT8 *  NAS_EMM_SecuGetKeyByIndex( VOS_UINT32 ulKeyIndex )
{
    return (VOS_UINT8 *)BSP_CIPHER_GetKey(ulKeyIndex);
}
#endif


VOS_INT32  NAS_CIPHER_SingleSubmitTask( VOS_UINT8 const  *pucSrcAddr,
                                        VOS_UINT8            const  *pucDstAddr,
                                        CIHPER_SINGLE_CFG_S         *pstSingleSecuPara)
{
    VOS_INT32                           lRslt;

    /* ����BSP�ӿڽⱣ��*/
    lRslt = BSP_CIPHER_SingleSubmitTask(LTE_SECURITY_CHANNEL_NAS, pucSrcAddr,
                            pucDstAddr, pstSingleSecuPara, CIPHER_SUBM_BLK_HOLD,
                            NAS_EMM_SECU_NO_PRIVATE);

     return lRslt;
}
VOS_UINT32  NAS_LMM_PreProcReselSecuInfoReq( MsgBlock * pMsg )
{
    LMM_GMM_RESEL_SECU_INFO_CNF_STRU   *pstLmmReselSecuInfoCnf = VOS_NULL_PTR;
    GMM_LMM_RESEL_SECU_INFO_REQ_STRU   *pstGmmReselSecuInfoReq = VOS_NULL_PTR;

    NAS_EMM_SECU_LOG_INFO("NAS_LMM_PreProcReselSecuInfoReq enter.");

    /* ��ȡ GMM_LMM_RESEL_SECU_INFO_REQ*/
    pstGmmReselSecuInfoReq = (GMM_LMM_RESEL_SECU_INFO_REQ_STRU *)pMsg;

    /* ����LMM_GMM_RESEL_SECU_INFO_CNF��Ϣ�ڴ�*/
    pstLmmReselSecuInfoCnf = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_GMM_RESEL_SECU_INFO_CNF_STRU));
    if (NAS_EMM_NULL_PTR == pstLmmReselSecuInfoCnf)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_LMM_PreProcReselSecuInfoReq: MSG ALLOC ERR!");
        return NAS_LMM_MSG_HANDLED;
    }

    /* ��� */
    NAS_LMM_MEM_SET(pstLmmReselSecuInfoCnf,0,sizeof(LMM_GMM_RESEL_SECU_INFO_CNF_STRU));

    /* LMM_GMM_RESEL_SECU_INFO_CNF��װ:�����Ϣͷ����ϢID*/
    NAS_EMM_COMP_GMM_MSG_HEADER(        pstLmmReselSecuInfoCnf,
                                        (sizeof(LMM_GMM_RESEL_SECU_INFO_CNF_STRU)-
                                        NAS_EMM_LEN_VOS_MSG_HEADER));
    pstLmmReselSecuInfoCnf->ulMsgId = ID_LMM_GMM_RESEL_SECU_INFO_CNF;
    pstLmmReselSecuInfoCnf->ulOpId = pstGmmReselSecuInfoReq->ulOpId;

    /* LMM_GMM_RESEL_SECU_INFO_CNF��װ: ��Ϣ�����*/
    NAS_LMM_CompLmmReselSecuInfoCnf(pstLmmReselSecuInfoCnf);

    /* ���� LMM_GMM_RESEL_SECU_INFO_CNF*/
    NAS_LMM_SendLmmMmcMsg((VOS_VOID*)pstLmmReselSecuInfoCnf);

    return NAS_LMM_MSG_HANDLED;
}



VOS_VOID  NAS_LMM_CompLmmReselSecuInfoCnf(
                    LMM_GMM_RESEL_SECU_INFO_CNF_STRU   *pstLmmReselSecuInfoCnf )
{
    VOS_UINT32                          ulCurEmmStat;

    /* ��ȡ EMM ��ǰ״̬�����ݵ�ǰ״̬��ͬ����*/
    ulCurEmmStat = NAS_LMM_PUB_COMP_EMMSTATE(NAS_EMM_CUR_MAIN_STAT,
                                            NAS_EMM_CUR_SUB_STAT);
    switch (ulCurEmmStat)
    {
        /* Dereg.NoCellAvailable״̬�£�
           �����ǰ��ȫ�����Ĳ����ڣ����سɹ�����bitOpSecuInfoΪ0;
           �����ǰ��ȫ�����Ĵ��ڣ����سɹ�����bitOpSecuInfoΪ1,���㰲ȫ����
        */
        case NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_DEREG, EMM_SS_DEREG_NO_CELL_AVAILABLE):
            pstLmmReselSecuInfoCnf->enRslt = LMM_GMM_SECU_INFO_RSLT_SUCC;
            if (EMM_CUR_SECU_NOT_EXIST == NAS_LMM_GetSecuCurFsmCS())
            {
                pstLmmReselSecuInfoCnf->bitOpSecuInfo = NAS_EMM_BIT_NO_SLCT;
            }
            else
            {
                pstLmmReselSecuInfoCnf->bitOpSecuInfo = NAS_EMM_BIT_SLCT;
                NAS_EMM_SecuGetUmtsSecuCntxtAndNasTokenIdle(
                                    &(pstLmmReselSecuInfoCnf->stReselSecuInfo));

                /* ÿ����һ��NAS_TOKEN,UP_NASCOUTN��Ҫ��1*/
                NAS_EMM_GetSecuUlNasCount()++;
            }
            break;

        /* Reg.NoCellAvailable״̬�£�
           �����ǰ��ȫ�����Ĳ����ڣ�����ʧ�ܣ���bitOpSecuInfoΪ0;
           �����ǰ��ȫ�����Ĵ��ڣ����سɹ�����bitOpSecuInfoΪ1,���㰲ȫ����
        */
        case NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_REG, EMM_SS_REG_NO_CELL_AVAILABLE):
            if (EMM_CUR_SECU_NOT_EXIST == NAS_LMM_GetSecuCurFsmCS())
            {
                pstLmmReselSecuInfoCnf->enRslt = LMM_GMM_SECU_INFO_RSLT_FAIL;
                pstLmmReselSecuInfoCnf->bitOpSecuInfo = NAS_EMM_BIT_NO_SLCT;
            }
            else
            {
                pstLmmReselSecuInfoCnf->enRslt = LMM_GMM_SECU_INFO_RSLT_SUCC;
                pstLmmReselSecuInfoCnf->bitOpSecuInfo = NAS_EMM_BIT_SLCT;
                NAS_EMM_SecuGetUmtsSecuCntxtAndNasTokenIdle(
                                    &(pstLmmReselSecuInfoCnf->stReselSecuInfo));
                /* ÿ����һ��NAS_TOKEN,UP_NASCOUTN��Ҫ��1*/
                NAS_EMM_GetSecuUlNasCount()++;
            }
            break;

        /* ���EMM���ڹ���̬����ֱ�ӻظ�GMMʧ��*/
        default:
            pstLmmReselSecuInfoCnf->enRslt = LMM_GMM_SECU_INFO_RSLT_FAIL;
            pstLmmReselSecuInfoCnf->bitOpSecuInfo = NAS_EMM_BIT_NO_SLCT;
            break;
    }

    return;
}


VOS_VOID  NAS_LMM_CompLmmHoSecuInfoCnf(
                    LMM_GMM_HO_SECU_INFO_CNF_STRU   *pstLmmHoSecuInfoCnf )
{
#if 0
    VOS_UINT32                          ulCurEmmStat;

    /* ��ȡ EMM ��ǰ״̬*/
    ulCurEmmStat = NAS_LMM_PUB_COMP_EMMSTATE(NAS_EMM_CUR_MAIN_STAT,
                                            NAS_EMM_CUR_SUB_STAT);

    /* EMM״̬������REG.NO_CELL_AVAILABLE��Ϊ����̬������ظ�ʧ��*/
     if((NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_REG, EMM_SS_REG_NO_CELL_AVAILABLE)
                                        !=ulCurEmmStat)
      ||(NAS_LMM_CUR_LTE_SUSPEND != NAS_EMM_GetCurLteState()))
    {
        pstLmmHoSecuInfoCnf->enRslt         = LMM_GMM_SECU_INFO_RSLT_FAIL;
        pstLmmHoSecuInfoCnf->bitOpSecuCntxt = NAS_EMM_BIT_NO_SLCT;

        return;
    }
#endif

    /* EMM״̬��ȷ:
       �����ǰ��ȫ�����Ĳ����ڣ��ظ�ʧ�ܣ���bitOpSecuInfoΪ0;
       �����ǰ��ȫ�����Ĵ��ڣ��ظ��ɹ�����bitOpSecuInfoΪ1,���㰲ȫ����*/
    if (EMM_CUR_SECU_NOT_EXIST == NAS_LMM_GetSecuCurFsmCS())
    {
        pstLmmHoSecuInfoCnf->enRslt         = LMM_GMM_SECU_INFO_RSLT_FAIL;
        pstLmmHoSecuInfoCnf->bitOpSecuCntxt = NAS_EMM_BIT_NO_SLCT;
    }
    else
    {
        pstLmmHoSecuInfoCnf->enRslt         = LMM_GMM_SECU_INFO_RSLT_SUCC;
        pstLmmHoSecuInfoCnf->bitOpSecuCntxt = NAS_EMM_BIT_SLCT;
        NAS_EMM_SecuGetUmtsSecuCntxtHo(&(pstLmmHoSecuInfoCnf->stHoUmtsSecuCntxt));
    }

    return;
}


VOS_VOID  NAS_LMM_CompLmmMmHoSecuInfoCnf(
                    LMM_MM_HO_SECU_INFO_CNF_STRU   *pstLmmHoSecuInfoCnf )
{
    NAS_LMM_SECU_CONTEXT_STRU           *pstCurSecuCntxt = VOS_NULL_PTR;

    /*��ȡ��ǰ��ȫ������*/
    pstCurSecuCntxt = NAS_EMM_SecuGetCurCntxtAddr();

    /* �����ǰ��ȫ�����Ĳ����ڣ��ظ�ʧ�ܣ���bitOpSecuInfoΪ0;
       �����ǰ��ȫ�����Ĵ��ڣ�ʹ���㷨ΪEIA0���ظ�ʧ��
       �����ǰ��ȫ�����Ĵ��ڣ��ظ��ɹ�����bitOpSecuInfoΪ1,���㰲ȫ����*/
    if (EMM_CUR_SECU_NOT_EXIST == NAS_LMM_GetSecuCurFsmCS())
    {
        pstLmmHoSecuInfoCnf->enRslt         = MM_LMM_SECU_INFO_RSLT_FAIL_OTHERS;
        pstLmmHoSecuInfoCnf->bitOpSecuCntxt = NAS_EMM_BIT_NO_SLCT;
    }
    /* ����㷨��EIA0�����ʾû�а�ȫ������ */
    else if (NAS_EMM_SECU_ALG_EIA0 == (pstCurSecuCntxt->ucSecuAlg & NAS_EMM_LOW_3_BITS_F))
    {
        pstLmmHoSecuInfoCnf->enRslt         = MM_LMM_SECU_INFO_RSLT_FAIL_EIA0;
        pstLmmHoSecuInfoCnf->bitOpSecuCntxt = NAS_EMM_BIT_NO_SLCT;
    }
    else
    {
        pstLmmHoSecuInfoCnf->enRslt         = MM_LMM_SECU_INFO_RSLT_SUCC;
        pstLmmHoSecuInfoCnf->bitOpSecuCntxt = NAS_EMM_BIT_SLCT;
        NAS_EMM_SecuGetMmUmtsSecuCntxtHo(&(pstLmmHoSecuInfoCnf->stHoSecuCntxt));
    }

    return;
}
VOS_UINT32  NAS_LMM_PreProcHoSecuInfoReq(  MsgBlock * pMsg )
{
    LMM_GMM_HO_SECU_INFO_CNF_STRU   *pstLmmHoSecuInfoCnf = VOS_NULL_PTR;
    GMM_LMM_HO_SECU_INFO_REQ_STRU   *pstGmmHoSecuInfoReq = VOS_NULL_PTR;

    NAS_EMM_SECU_LOG_INFO("NAS_LMM_PreProcHoSecuInfoReq enter.");

    /* ��ȡ GMM_LMM_HO_SECU_INFO_REQ*/
    pstGmmHoSecuInfoReq = (GMM_LMM_HO_SECU_INFO_REQ_STRU *)pMsg;

    /* ����LMM_GMM_HO_SECU_INFO_CNF��Ϣ�ڴ�*/
    pstLmmHoSecuInfoCnf = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_GMM_HO_SECU_INFO_CNF_STRU));
    if (NAS_EMM_NULL_PTR == pstLmmHoSecuInfoCnf)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_LMM_PreProcHoSecuInfoReq: MSG ALLOC ERR!");
        return NAS_LMM_MSG_HANDLED;
    }

    /* ��� */
    NAS_LMM_MEM_SET(pstLmmHoSecuInfoCnf, 0, sizeof(LMM_GMM_HO_SECU_INFO_CNF_STRU));

    /* LMM_GMM_HO_SECU_INFO_CNF��װ:�����Ϣͷ����ϢID*/
    NAS_EMM_COMP_GMM_MSG_HEADER(        pstLmmHoSecuInfoCnf,
                                        (sizeof(LMM_GMM_HO_SECU_INFO_CNF_STRU)-
                                        NAS_EMM_LEN_VOS_MSG_HEADER));
    pstLmmHoSecuInfoCnf->ulMsgId        = ID_LMM_GMM_HO_SECU_INFO_CNF;
    pstLmmHoSecuInfoCnf->ulOpId         = pstGmmHoSecuInfoReq->ulOpId;

    /* LMM_GMM_HO_SECU_INFO_CNF��װ: ��Ϣ�����*/
    NAS_LMM_CompLmmHoSecuInfoCnf(pstLmmHoSecuInfoCnf);

    /* ���� LMM_GMM_HO_SECU_INFO_CNF*/
    NAS_LMM_SendLmmMmcMsg((VOS_VOID*)pstLmmHoSecuInfoCnf);

    return NAS_LMM_MSG_HANDLED;

}


VOS_UINT32  NAS_LMM_PreProcMsgMmHoSecuInfoReq(  MsgBlock * pMsg )
{
    LMM_MM_HO_SECU_INFO_CNF_STRU   *pstLmmHoSecuInfoCnf = VOS_NULL_PTR;
    MM_LMM_HO_SECU_INFO_REQ_STRU   *pstMmHoSecuInfoReq = VOS_NULL_PTR;

    NAS_EMM_SECU_LOG_INFO("NAS_LMM_PreProcMsgMmHoSecuInfoReq enter.");

    /* ��ȡ MM_LMM_HO_SECU_INFO_REQ*/
    pstMmHoSecuInfoReq = (MM_LMM_HO_SECU_INFO_REQ_STRU *)pMsg;

    /* ����LMM_MM_HO_SECU_INFO_CNF��Ϣ�ڴ�*/
    pstLmmHoSecuInfoCnf = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MM_HO_SECU_INFO_CNF_STRU));
    if (NAS_EMM_NULL_PTR == pstLmmHoSecuInfoCnf)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_LMM_PreProcMsgMmHoSecuInfoReq: MSG ALLOC ERR!");
        return NAS_LMM_MSG_HANDLED;
    }

    /* ��� */
    NAS_LMM_MEM_SET(pstLmmHoSecuInfoCnf, 0, sizeof(LMM_MM_HO_SECU_INFO_CNF_STRU));

    /* LMM_MM_HO_SECU_INFO_CNF��װ:�����Ϣͷ����ϢID*/
    EMM_PUBU_COMP_MM_MSG_HEADER(        pstLmmHoSecuInfoCnf,
                                        (sizeof(LMM_MM_HO_SECU_INFO_CNF_STRU)-
                                        NAS_EMM_LEN_VOS_MSG_HEADER));
    pstLmmHoSecuInfoCnf->ulMsgId        = ID_LMM_MM_HO_SECU_INFO_CNF;
    pstLmmHoSecuInfoCnf->ulOpId         = pstMmHoSecuInfoReq->ulOpId;

    /* LMM_MM_HO_SECU_INFO_CNF��װ: ��Ϣ�����*/
    NAS_LMM_CompLmmMmHoSecuInfoCnf(pstLmmHoSecuInfoCnf);

    /* ���� LMM_MM_HO_SECU_INFO_CNF*/
    NAS_LMM_SendLmmMmcMsg((VOS_VOID*)pstLmmHoSecuInfoCnf);

    return NAS_LMM_MSG_HANDLED;

}


/*****************************************************************************
 Function Name   : STUB_NAS_LMM_CompLmmHoSecuInfoCnf
 Description     : ǰ�ڵ�HO������ΪGMM�ṩ�ú�����
                   GMM�ṩHO���ܺ󣬴˺�������ʹ�á�
 Input           : None
 Output          : pstLmmHoSecuInfoCnf��
                   ���в��ֳ�Ա��ʹ�ã�����: |- VOS_MSG_HEADER
                                                |  ulMsgId;
                                                |  ulOpId;
                                                |  bitSpare
                                                |- bitOpSecuCntxt  :

                   ʹ�õĳ�Ա����:            |- enRslt;
                                                |- stHoUmtsSecuCntxt;

                    enRslt: LMM_GMM_SECU_INFO_RSLT_SUCCʱ����ʾ��ȡ�ɹ�;
                            LMM_GMM_SECU_INFO_RSLT_FAILʱ����ʾ��ȡʧ��;

 Return          : MMC_LMM_RESULT_ID_ENUM,
                            GMM_LMM_FAILʱ����ʾ�����ָ��
                            GMM_LMM_SUCCʱ����ʾ����ָ��ǿ�
 History         :
    1.Hanlufeng 41410      2011-6-23  Draft Enact

*****************************************************************************/
GMM_LMM_RESULT_ID_ENUM_UINT32  STUB_NAS_LMM_CompLmmHoSecuInfoCnf(
                LMM_GMM_HO_SECU_INFO_CNF_STRU   *pstLmmHoSecuInfoCnf )
{

    if(VOS_NULL_PTR == pstLmmHoSecuInfoCnf)
    {
        NAS_EMM_SECU_LOG_ERR("STUB_NAS_LMM_CompLmmHoSecuInfoCnf: NULL PTR.");
        return  GMM_LMM_FAIL;
    }

    NAS_EMM_SECU_LOG_INFO("STUB_NAS_LMM_CompLmmHoSecuInfoCnf: enter.");

    NAS_LMM_CompLmmHoSecuInfoCnf(pstLmmHoSecuInfoCnf);
    return  GMM_LMM_SUCC;
}



VOS_UINT32  NAS_EMM_SecuGetUmtsSecuContext( VOS_VOID )
{
    MMC_LMM_RESULT_ID_ENUM_UINT32       enRslt;
    NAS_GUMM_INFO_STRU                  stGummInfoStru;

    /*��ʼ��*/
    NAS_LMM_MEM_SET(&stGummInfoStru, 0, sizeof(NAS_GUMM_INFO_STRU));

    NAS_EMM_SECU_LOG_NORM("NAS_EMM_SecuGetUmtsSecuContext is entered.");
    /*PC REPLAY MODIFY BY LEILI BEGIN*/
    /* ����GMM�ṩ�Ľӿڣ���ȡUMTS��ȫ������*/
    enRslt = NAS_EMM_GetGuInfo(NAS_GUMM_UMTS_SECU_PARA, &stGummInfoStru);
    /*PC REPLAY MODIFY BY LEILI END*/
    if(MMC_LMM_FAIL == enRslt)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuGetUmtsSecuContext: Nas_GetGuInfo Err.");
        return NAS_EMM_SECU_FAIL;
    }

    /* ����*/
    NAS_LMM_MEM_CPY(                     NAS_EMM_GetUmtsSecuContextAddr(),
                                       &(stGummInfoStru.u.stSecuContext),
                                        sizeof(NAS_GUMM_UMTS_CONTEXT_STRU));
    return NAS_EMM_SECU_SUCC;
}
VOS_VOID  NAS_EMM_SecuResumeIdle(VOS_VOID )
{
    VOS_UINT32                          ulRslt;

    /* ���Current��ȫ������״̬*/
    if (EMM_CUR_SECU_EXIST_ACTIVE == NAS_LMM_GetSecuCurFsmCS())
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuResumeIdle: Cur Sta Err.");
        return;
    }

    /* ���� NonceUE*/
    ulRslt = NAS_EMM_DeriveRandNonceUE(NAS_EMM_GetSecuNonceUEAddr());
    if(NAS_EMM_SECU_SUCC != ulRslt)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuResumeIdle: NAS_EMM_DeriveRandNonceUE Err.");
    }

    /* ��ȡ UMTS secu context*/
    ulRslt = NAS_EMM_SecuGetUmtsSecuContext();
    if(NAS_EMM_SECU_SUCC != ulRslt)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuResumeIdle: NAS_EMM_SecuGetUmtsSecuContext Err.");
    }

    /* �� new mapped secu cntxt�е� KSIsgsn��ֵΪ UMTS KSI*/
    NAS_EMM_SetSecuNewMappedKSIsgsn(NAS_EMM_GetUmtsKsi());

    /* ��ȫ������״̬Ǩ��:��״̬���䣬��״̬Ǩ�Ƶ� EMM_NEW_SECU_EXIST*/
    NAS_EMM_SecuStateConvert(           NAS_LMM_GetSecuCurFsmCS(),
                                        EMM_NEW_SECU_EXIST);
    return;
}
VOS_VOID  NAS_EMM_SecuRrcSecuParamIndHo(
                                                const VOS_UINT8 *pucSecuParaHo )
{
    NAS_LMM_HO_LRRC_SECU_PARA_STRU           stHoLrrcSecuPara = {0};
    VOS_UINT32                               ulRslt;
    VOS_UINT8                                ucEia = NAS_EMM_NULL;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuRrcSecuParamIndHo entered.");

    /* ����LRRC�ϱ��İ�ȫ����*/
    NAS_EMM_SecuDecodeRrcSecuPara(pucSecuParaHo, &stHoLrrcSecuPara);

    ucEia = stHoLrrcSecuPara.ucSecuAlg & NAS_LMM_LOW_3_BITS_F;
    if (NAS_EMM_SECU_ALG_EIA0 == ucEia)
    {
        /* ���LRRC�ϱ��İ�ȫ��������Ч��:
           1)Eea,Eia�Ƿ���UE_NET_CAP֧�ֵ��㷨
           2)EeaΪEea0
           2)KsiType������ mapped ����
           3)KsiValueȡֵ��Χ[0,6]*/
        ulRslt = NAS_EMM_Eia0SecuVerifyRrcSecuPara(&stHoLrrcSecuPara);
    }
    else
    {
        /* ���LRRC�ϱ��İ�ȫ��������Ч��:
           1)Eea,Eia�Ƿ���UE_NET_CAP֧�ֵ��㷨
           2)KsiType������ mapped ����
           3)KsiValueȡֵ��Χ[0,6]*/
        ulRslt = NAS_EMM_NotEia0SecuVerifyRrcSecuPara(&stHoLrrcSecuPara);
    }

    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        return;
    }

    /* �������current native security context��
       ���䱣�浽 non-current native seucrity context,
       ɾ�� new native security context*/
    if(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE != NAS_EMM_GetSecuCurKSIasme())
    {
        NAS_EMM_SecuSaveCurNativeToNonCurNative();
        NAS_EMM_ClearNewNativeSecuCntxt();
    }

    /* ɾ�� current security context*/
    NAS_EMM_ClearCurSecuCntxt();

    /* ��ȡUMTS security context*/
    ulRslt = NAS_EMM_SecuGetUmtsSecuContext();
    if((NAS_EMM_SECU_SUCC != ulRslt) && (NAS_EMM_SECU_ALG_EIA0 != ucEia))
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuRrcSecuParamIndHo:\
                                          NAS_EMM_SecuGetUmtsSecuContext Err.");
        return;
    }

    /* ����LRRC�ϱ��İ�ȫ����������current mapped security context*/
    ulRslt = NAS_EMM_SecuHoCurMappedSecuCntxt(&stHoLrrcSecuPara);
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuRrcSecuParamIndHo:\
                                        NAS_EMM_SecuHoCurMappedSecuCntxt Err.");
        return;
    }

    /* ��¼������Mapped��ȫ�����ģ�����������̳���HOʧ��
       ����ݸñ�־�ж��Ƿ���Ҫ�ָ���ȫ������*/
    NAS_EMM_SetFlagInterRatHoMapped(NAS_EMM_SECU_INTER_RAT_HO_MAPPED);

    /* �ظ�LRRC_LMM_SECU_PARA_RSP*/
    NAS_EMM_SecuSendLrrcSecuParaRsp();

    /* Ǩ�ư�ȫ������״̬*/
    NAS_LMM_SetSecuCurFsmCS(EMM_CUR_SECU_EXIST_ACTIVE);
    if((NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE != NAS_EMM_GetSecuNewMappedKSIsgsn())
     ||(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE != NAS_EMM_GetSecuNewNativeKSIasme())
     ||(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE != NAS_EMM_GetSecuNonCurNativeKSIasme()))
    {
        NAS_LMM_SetSecuCurFsmNS(EMM_NEW_SECU_EXIST);
    }
    else
    {
        NAS_LMM_SetSecuCurFsmNS(EMM_NEW_SECU_NOT_EXIST);
    }

    return;

}


VOS_VOID  NAS_EMM_SecuDecodeRrcSecuPara(
                                  const VOS_UINT8 *pucSecuParaHo,
                                        NAS_LMM_HO_LRRC_SECU_PARA_STRU *pstHoLrrcSecuPara)
{
    VOS_UINT8                           ucIndex = 0;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuDecodeRrcSecuPara entered.");

    /* ��μ��*/
    if((VOS_NULL_PTR == pucSecuParaHo)
     ||(VOS_NULL_PTR == pstHoLrrcSecuPara))
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuDecodeRrcSecuPara:NULL PTR.");
        return;
    }

    /* ������ȫ����*/
    /* NonceMME*/
    NAS_LMM_MEM_CPY(                    pstHoLrrcSecuPara->aucNonceMME,
                                        pucSecuParaHo,
                                        NAS_EMM_SECU_NONCE_MME_LEN);
    ucIndex += NAS_EMM_SECU_NONCE_MME_LEN;

    /* SecuAlg*/
    pstHoLrrcSecuPara->ucSecuAlg = pucSecuParaHo[ucIndex];
    ucIndex++;

    /* KsiType, KsiValue*/
    pstHoLrrcSecuPara->ucKsiValue = (pucSecuParaHo[ucIndex]) & NAS_EMM_LOW_3_BITS_F;
    pstHoLrrcSecuPara->ucKsiType = ((pucSecuParaHo[ucIndex]) & NAS_EMM_FOURTH_BITS_F)
                                                >>NAS_EMM_SECU_KSI_TYPE_BIT_POS;

    return;
}
VOS_UINT32  NAS_EMM_Eia0SecuVerifyRrcSecuPara
(
    const NAS_LMM_HO_LRRC_SECU_PARA_STRU *pstHoLrrcSecuPara
)
{
    VOS_UINT32                      ulRslt;
    VOS_UINT8                       ucEea;
    VOS_UINT8                       ucEia;


    NAS_EMM_SECU_LOG_INFO("NAS_EMM_Eia0SecuVerifyRrcSecuPara entered.");

    /* ���EEA,EIA*/
    ucEia =  (pstHoLrrcSecuPara->ucSecuAlg) & NAS_EMM_LOW_3_BITS_F;
    ucEea =  (pstHoLrrcSecuPara->ucSecuAlg>>4) & NAS_EMM_LOW_3_BITS_F;
    ulRslt = NAS_EMM_Eia0SmcVerifyAlg(ucEea, ucEia);
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_Eia0SecuVerifyRrcSecuPara:Alg Err.");
        return NAS_EMM_SECU_FAIL;
    }

    /* ���KsiValue:[0,6]*/
    if(NAS_EMM_SECU_MAX_KSI < pstHoLrrcSecuPara->ucKsiValue)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_Eia0SecuVerifyRrcSecuPara:KsiValue Err.");
        return NAS_EMM_SECU_FAIL;
    }

    /* ���KSI�Ƿ�Ϊ0 */
    if (0 != pstHoLrrcSecuPara->ucKsiValue)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_Eia0SecuVerifyRrcSecuPara:KsiType is not zero.");
    }

    /* ���KsiType:������Mapped����*/
    if(NAS_EMM_SECU_KSI_TYPE_MAPPED != pstHoLrrcSecuPara->ucKsiType)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_Eia0SecuVerifyRrcSecuPara:KsiType Err.");
    }

    /* Ϊ�˼��ݲ�ͬ���磬ʵ�п���ϳ�ԭ���ݲ��ж��Ƿ��н������� */

    return NAS_EMM_SECU_SUCC;
}
VOS_UINT32  NAS_EMM_NotEia0SecuVerifyRrcSecuPara(
                        const NAS_LMM_HO_LRRC_SECU_PARA_STRU *pstHoLrrcSecuPara )
{
    VOS_UINT32                      ulRslt;
    VOS_UINT8                       ucEea;
    VOS_UINT8                       ucEia;


    NAS_EMM_SECU_LOG_INFO("NAS_EMM_NotEia0SecuVerifyRrcSecuPara entered.");

    /* ���EEA,EIA*/
    ucEia =  (pstHoLrrcSecuPara->ucSecuAlg) & NAS_EMM_LOW_3_BITS_F;
    ucEea =  (pstHoLrrcSecuPara->ucSecuAlg>>4) & NAS_EMM_LOW_3_BITS_F;
    ulRslt = NAS_EMM_IsAlgUeSupported(ucEea, ucEia);
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_NotEia0SecuVerifyRrcSecuPara:Alg Err.");
        return NAS_EMM_SECU_FAIL;
    }

    /* ���KsiValue:[0,6]*/
    if(NAS_EMM_SECU_MAX_KSI < pstHoLrrcSecuPara->ucKsiValue)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_NotEia0SecuVerifyRrcSecuPara:KsiValue Err.");
        return NAS_EMM_SECU_FAIL;
    }

    /* ���KsiType:������Mapped����*/
    if(NAS_EMM_SECU_KSI_TYPE_MAPPED != pstHoLrrcSecuPara->ucKsiType)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_NotEia0SecuVerifyRrcSecuPara:KsiType Err.");
    }

    return NAS_EMM_SECU_SUCC;

}
VOS_VOID  NAS_EMM_SecuSaveCurNativeToNonCurNative( VOS_VOID )
{
    NAS_LMM_SECU_CONTEXT_STRU          *pstNonCurNatvieSecuCntxt = VOS_NULL_PTR;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuSaveCurNativeToNonCurNative entered.");

    /* ��ȡ non-current native security context*/
    pstNonCurNatvieSecuCntxt = NAS_EMM_GetSecuNonCurNativeCntxtAddr();

    /* �� current native security context
       ���浽 non-current native security context*/
    /* KSIasme*/
    pstNonCurNatvieSecuCntxt->ucKSIasme = NAS_EMM_GetSecuCurKSIasme();

    /* Kasme*/
    NAS_LMM_MEM_CPY(        pstNonCurNatvieSecuCntxt->aucKasme,
                            NAS_EMM_GetSecuCurKasmeAddr(),
                            NAS_LMM_SECU_AUTH_KEY_ASME_LEN);

    /* UP_NASCOUNT �� DL_NASCOUNT*/
    pstNonCurNatvieSecuCntxt->ulUlNasCount = NAS_EMM_GetSecuUlNasCount();
    pstNonCurNatvieSecuCntxt->ulDlNasCount = NAS_EMM_GetSecuDlNasCount();

    /* Eea,Eia*/
    pstNonCurNatvieSecuCntxt->ucSecuAlg = NAS_EMM_GetSecuAlg();

    /* NasCK, NasIK*/
    NAS_LMM_MEM_CPY(        &(pstNonCurNatvieSecuCntxt->stNasKey),
                              NAS_EMM_GetSecuCurNasKeyAddr(),
                              sizeof(NAS_LMM_SECU_KEY_STRU));
    return;

}
VOS_VOID  NAS_EMM_SecuRecovNonCurNativeToCurNative( VOS_VOID )
{
    NAS_LMM_SECU_CONTEXT_STRU          *pstNonCurNatvieSecuCntxt = VOS_NULL_PTR;
    VOS_UINT32                          ulRslt = NAS_EMM_SECU_SUCC;

    (VOS_VOID)ulRslt;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuRecovNonCurNativeToCurNative entered.");

    /* ��ȡ non-current native security context*/
    pstNonCurNatvieSecuCntxt = NAS_EMM_GetSecuNonCurNativeCntxtAddr();

    /* KSIasme*/
    NAS_EMM_SetSecuCurKSIasme(NAS_EMM_GetSecuNonCurNativeKSIasme());

    /* KSIsgsn*/
    NAS_EMM_SetSecuCurKSIsgsn(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);

    /* Kasme*/
    NAS_LMM_MEM_CPY(                    NAS_EMM_GetSecuCurKasmeAddr(),
                                        pstNonCurNatvieSecuCntxt->aucKasme,
                                        NAS_LMM_SECU_AUTH_KEY_ASME_LEN);

    /* UP_NASCOUNT �� DL_NASCOUNT*/
    NAS_EMM_GetSecuUlNasCount() = pstNonCurNatvieSecuCntxt->ulUlNasCount;
    NAS_EMM_GetSecuDlNasCount() = pstNonCurNatvieSecuCntxt->ulDlNasCount;

    /* Eea,Eia*/
    NAS_EMM_SetSecuAlg(pstNonCurNatvieSecuCntxt->ucSecuAlg);

    /* NasCK, NasIK*/
    NAS_LMM_MEM_CPY(                    NAS_EMM_GetSecuCurNasKeyAddr(),
                                       &(pstNonCurNatvieSecuCntxt->stNasKey),
                                        sizeof(NAS_LMM_SECU_KEY_STRU));
    /* д��Ĵ���*/
    (VOS_VOID)NAS_EMM_SecuSetKeyByKeyIndex(  NAS_EMM_GetSecuCurNasIkAddr(),
                                    NAS_EMM_SECU_INTEGRITY_KEY);

    (VOS_VOID)NAS_EMM_SecuSetKeyByKeyIndex(  NAS_EMM_GetSecuCurNasCkAddr(),
                                    NAS_EMM_SECU_CIPHER_KEY);
    return;

}



VOS_UINT32  NAS_EMM_SecuHoCurMappedSecuCntxt(
                        const NAS_LMM_HO_LRRC_SECU_PARA_STRU *pstHoLrrcSecuPara )

{
    VOS_UINT32                          ulRslt;
     VOS_UINT8                          ucEea;
    VOS_UINT8                           ucEia;

    /* KSIsgsn*/
    NAS_EMM_SetSecuCurKSIsgsn(pstHoLrrcSecuPara->ucKsiValue);
    NAS_EMM_SetSecuCurKSIasme(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);

    /* SecuAlg*/
    NAS_EMM_SetSecuAlg(pstHoLrrcSecuPara->ucSecuAlg);

    /* NonceMME*/
    NAS_LMM_MEM_CPY(        NAS_EMM_GetSecuNonceMMEAddr(),
                            pstHoLrrcSecuPara->aucNonceMME,
                            NAS_EMM_SECU_NONCE_MME_LEN);
    /* ���� Kasme*/
    ucEia =  (pstHoLrrcSecuPara->ucSecuAlg) & NAS_EMM_LOW_3_BITS_F;
    ucEea =  (pstHoLrrcSecuPara->ucSecuAlg>>4) & NAS_EMM_LOW_3_BITS_F;

    /* ����K'ASME */
    if (NAS_EMM_SECU_ALG_EIA0 == ucEia)
    {
        NAS_LMM_MEM_CPY(    NAS_EMM_GetSecuCurKasmeAddr(),
                            NAS_EMM_GetEia0Kasme(),
                            NAS_LMM_SECU_AUTH_KEY_ASME_LEN);
    }
    else
    {
        ulRslt = NAS_EMM_DeriveKasmeFromUmtsHo(NAS_EMM_GetSecuCurKasmeAddr());
        if(NAS_EMM_SECU_FAIL == ulRslt)
        {
            NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuHoCurMappedSecuCntxt:\
                                            NAS_EMM_DeriveKasmeFromUmtsHo Err.");
            return NAS_EMM_SECU_FAIL;
        }
    }

    /* UP_NASCOUNT,DL_NASCOUNT����*/
    NAS_EMM_GetSecuDlNasCount() = 0;
    NAS_EMM_GetSecuUlNasCount() = 0;

    /* ���� NasCk,NasIk*/
    NAS_EMM_DeriveNewNasKey(NAS_EMM_GetSecuCurKasmeAddr(), ucEea, ucEia);
    NAS_LMM_MEM_CPY(                    NAS_EMM_GetSecuCurNasIkAddr(),
                                        NAS_EMM_GetSecuTempNasIkAddr(),
                                        NAS_LMM_SECU_AUTH_KEY_NAS_INT_LEN);
    NAS_LMM_MEM_CPY(                    NAS_EMM_GetSecuCurNasCkAddr(),
                                        NAS_EMM_GetSecuTempNasCkAddr(),
                                        NAS_LMM_SECU_AUTH_KEY_NAS_ENC_LEN);

    /* �� NasCk,NasIkд��Ĵ���*/
    (VOS_VOID)NAS_EMM_SecuSetKeyByKeyIndex(  NAS_EMM_GetSecuCurNasIkAddr(),
                                        NAS_EMM_SECU_INTEGRITY_KEY);

    (VOS_VOID)NAS_EMM_SecuSetKeyByKeyIndex(  NAS_EMM_GetSecuCurNasCkAddr(),
                                        NAS_EMM_SECU_CIPHER_KEY);

    return NAS_EMM_SECU_SUCC;
}


VOS_VOID  NAS_EMM_SecuSendLrrcSecuParaRsp( VOS_VOID )
{
    LRRC_LMM_SECU_PARA_RSP_STRU        *pstLrrcSecuParaRsp = VOS_NULL_PTR;
    VOS_UINT32                          ulMsgLen;

    /* ���㳤�Ȳ�����VOS��Ϣͷ*/
    ulMsgLen     = NAS_EMM_MSG_ID_LEN + sizeof(LRRC_LNAS_SECURITY_KEY_STRU);

    /* �����ڴ�*/
    pstLrrcSecuParaRsp = (VOS_VOID *)NAS_LMM_ALLOC_MSG(ulMsgLen + NAS_EMM_LEN_VOS_MSG_HEADER);
    if(NAS_EMM_NULL_PTR == pstLrrcSecuParaRsp)
    {
        NAS_EMM_SECU_LOG_WARN("NAS_EMM_SecuSendLrrcSecuParaRsp: MSG ALLOC ERR!");
        return;
    }

    /* ������뵽����Ϣ�ռ�*/
    NAS_LMM_MEM_SET(pstLrrcSecuParaRsp, 0, sizeof(LRRC_LMM_SECU_PARA_RSP_STRU));

    /* ���VOS��Ϣͷ*/
    NAS_EMM_SET_RRC_MSG_HEADER(pstLrrcSecuParaRsp, ulMsgLen);

    /* �����ϢID*/
    pstLrrcSecuParaRsp->enMsgId           = ID_LRRC_LMM_SECU_PARA_RSP;

    /* �����Ϣ�� */
    /* UP_NASCOUNT*/
    pstLrrcSecuParaRsp->stSecuKey.ulNasCount = 0xFFFFFFFF;

    /* Kasme*/
    NAS_LMM_MEM_CPY(                    pstLrrcSecuParaRsp->stSecuKey.aucKasme,
                                        NAS_EMM_GetSecuCurKasmeAddr(),
                                        NAS_LMM_SECU_AUTH_KEY_ASME_LEN);

    /* ���� LRRC_LMM_SECU_PARA_RSP */
    NAS_LMM_SEND_MSG(pstLrrcSecuParaRsp);

    return;
}
VOS_VOID  NAS_EMM_SecuRrcRelIndHoFail(VOS_VOID )
{
    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuRrcRelIndHoFail entered.");

    /* �ж����û�в���Mapped��ȫ�����ģ�����Ҫ�ı䰲ȫ������*/
    if(NAS_EMM_SECU_INTER_RAT_HO_NOT_MAPPED == NAS_EMM_GetFlagInterRatHoMapped())
    {
        NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuRrcRelIndHoFail:no need recovery.");
        return;
    }

    /* ɾ����ǰ��ȫ������*/
    NAS_EMM_ClearCurSecuCntxt();

    /* ������� non-current native security context�����仹ԭ�� current,
       ɾ�� non-current native security context*/
    if(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE != NAS_EMM_GetSecuNonCurNativeKSIasme())
    {
        NAS_EMM_SecuRecovNonCurNativeToCurNative();
        NAS_EMM_ClearNonCurNativeSecuCntxt();
    }

    /* HOʧ�ܣ��ָ���ȫ�����ĺ������־*/
    NAS_EMM_SetFlagInterRatHoMapped(NAS_EMM_SECU_INTER_RAT_HO_NOT_MAPPED);

    /* Ǩ�ư�ȫ�����ĵ�״̬*/
    if(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE != NAS_EMM_GetSecuCurKSIasme())
    {
        NAS_LMM_SetSecuCurFsmCS(EMM_CUR_SECU_EXIST_NOT_ACTIVE);
    }
    else
    {
        NAS_LMM_SetSecuCurFsmCS(EMM_CUR_SECU_NOT_EXIST);
    }

    if((NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE != NAS_EMM_GetSecuNewMappedKSIsgsn())
     ||(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE != NAS_EMM_GetSecuNewNativeKSIasme())
     ||(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE != NAS_EMM_GetSecuNonCurNativeKSIasme()))
    {
        NAS_LMM_SetSecuCurFsmNS(EMM_NEW_SECU_EXIST);
    }
    else
    {
        NAS_LMM_SetSecuCurFsmNS(EMM_NEW_SECU_NOT_EXIST);
    }

    return;

}


VOS_VOID  NAS_EMM_SecuUpdateDlNasCountHo( VOS_UINT8 ucCnDlNasCount )
{
    VOS_UINT32                          ulUeDlNasCount = 0;
    VOS_UINT8                           ucUeLow4Bit = 0;

    NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuUpdateDlNasCountHo entered.");

    /* ��ȡUE���� DL_NASCOUNT �ĵ�4bit*/
    ulUeDlNasCount = NAS_EMM_GetSecuDlNasCount();
    ucUeLow4Bit = (VOS_UINT8)(ulUeDlNasCount & NAS_EMM_LOW_HALF_BYTE_F);

    /* ���CN�·�DL_NASCOUNT�ĵ� 4bit <= UE����DL_NASCOUNT�ĵ� 4bit��
       ��bit5��1*/
    if(ucCnDlNasCount <= ucUeLow4Bit)
    {
        ulUeDlNasCount += NAS_EMM_SECU_HO_DL_OVERFLOW;
    }

    /* ���UE DL_NASCOUNT�ĵ� 4bit*/
    ulUeDlNasCount &= NAS_EMM_SECU_MASK_HO_OVERFLOW_COUNT;

    /* ��4bit��ֵΪ�����·���ֵ*/
    ulUeDlNasCount += ucCnDlNasCount;

    /* ���� current DL_NASCOUNT*/
    NAS_EMM_SetSecuDlNasCount(ulUeDlNasCount);

    return;
}
VOS_VOID  NAS_EMM_SecuHoOrCcoReverse( VOS_VOID)
{
    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuHoOrCcoReverse entered.");

    /* �����ǰ��ȫ������״̬���Ǵ���δ����򱨴�*/
    if(EMM_CUR_SECU_EXIST_NOT_ACTIVE != NAS_LMM_GetSecuCurFsmCS())
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuHoOrCcoReverse: Cur Secu Sta Err.");
        return;
    }


    /* ��ǰ��ȫ������״̬�ָ�������̬*/
    NAS_EMM_SecuStateConvert(           EMM_CUR_SECU_EXIST_ACTIVE,
                                        NAS_LMM_GetSecuCurFsmNS());
    return;
}


VOS_VOID  NAS_EMM_SecuGetUmtsSecuCntxtAndNasTokenIdle(
                        LMM_GMM_RESEL_SECU_INFO_STRU       *pstReselSecuInfo )
{
    LMM_GMM_UMTS_SECU_CONTEXT_STRU     *pstUmtsSecuCntxt = VOS_NULL_PTR;
    VOS_UINT32                          ulRslt = 0;

    /* ���ָ����*/
    if (VOS_NULL_PTR == pstReselSecuInfo)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuGetUmtsSecuCntxtAndNasTokenIdle:NULL PTR.");
        return;
    }

    pstUmtsSecuCntxt = &(pstReselSecuInfo->stUmtsSecuContext);

    /* ��ȡ mapped UMTS security context*/
    /* KSI: ��ǰ��ȫ�����ĵ�KSI*/
    if(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE == NAS_EMM_GetSecuCurKSIsgsn())
    {
        pstUmtsSecuCntxt->KSI = NAS_EMM_GetSecuCurKSIasme();
    }
    else
    {
        pstUmtsSecuCntxt->KSI = NAS_EMM_GetSecuCurKSIsgsn();
    }

    /* CK,IK*/
    ulRslt = NAS_EMM_DeriveUmtsCkIkFromEpsIdle(
                            pstReselSecuInfo->stUmtsSecuContext.aucUmtsCk,
                            pstReselSecuInfo->stUmtsSecuContext.aucUmtsIk);
    if(NAS_EMM_SECU_SUCC != ulRslt)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuGetUmtsSecuCntxtAndNasTokenIdle: NAS CK||IK ERR.");
    }

    /* ��ȡNAS TOKEN*/
    ulRslt = NAS_EMM_DeriveNasToken(pstReselSecuInfo->aucNasToken);
    if(NAS_EMM_SECU_SUCC != ulRslt)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuGetUmtsSecuCntxtAndNasTokenIdle: NAS TOKEN ERR.");
    }

    return;
}
VOS_VOID  NAS_EMM_SecuGetUmtsSecuCntxtHo(
                                LMM_GMM_UMTS_SECU_CONTEXT_STRU *pstUmtsSecuCntxt)
{
    VOS_UINT32                          ulRslt;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuGetUmtsSecuCntxtHo entered.");

    /* KSI:current KSIasme or KSIsgsn*/
    if(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE == NAS_EMM_GetSecuCurKSIasme())
    {
        pstUmtsSecuCntxt->KSI           = NAS_EMM_GetSecuCurKSIsgsn();
    }
    else
    {
        pstUmtsSecuCntxt->KSI           = NAS_EMM_GetSecuCurKSIasme();
    }

    /* UmtsCK, UmtsIK*/
    ulRslt = NAS_EMM_DeriveUmtsCkIkFromEpsHo(pstUmtsSecuCntxt->aucUmtsCk,
                                             pstUmtsSecuCntxt->aucUmtsIk);
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuGetUmtsSecuCntxtHo:\
                                        NAS_EMM_DeriveUmtsCkIkFromEpsHo Err.");
    }

    return;
}


VOS_VOID  NAS_EMM_SecuGetMmUmtsSecuCntxtHo(
                                MM_LMM_GU_SECU_CONTEXT_STRU *pstUmtsSecuCntxt)
{
    VOS_UINT32                          ulRslt;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuGetUmtsSecuCntxtHo entered.");

    /* KSI:current KSIasme or KSIsgsn*/
    if(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE == NAS_EMM_GetSecuCurKSIasme())
    {
        pstUmtsSecuCntxt->ulKSI           = NAS_EMM_GetSecuCurKSIsgsn();
    }
    else
    {
        pstUmtsSecuCntxt->ulKSI           = NAS_EMM_GetSecuCurKSIasme();
    }

    /* UmtsCK, UmtsIK*/
    ulRslt = NAS_EMM_DeriveUmtsCkIkFromEpsSrvcc(pstUmtsSecuCntxt->aucCk,
                                                pstUmtsSecuCntxt->aucIk);
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuGetMmUmtsSecuCntxtHo:\
                                        NAS_EMM_DeriveUmtsCkIkFromEpsHo Err.");
    }

    return;
}



VOS_UINT32  NAS_EMM_DeriveRandNonceUE( VOS_UINT8 *pucNonceUE )
{
    VOS_UINT8                           i;
    VOS_UINT32                          ulRandom;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_DeriveRandNonceUE entered.");

    /* ��ָ����*/
    if (VOS_NULL_PTR == pucNonceUE)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_DeriveRandNonceUE:NULL PTR.");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ���ԭ NonceUE*/
    NAS_LMM_MEM_SET(pucNonceUE, 0, NAS_EMM_SECU_NONCE_UE_LEN);

#if(VOS_WIN32 == VOS_OS_VER)
    /* ������Դ����ʵ���Ի���*/
    pucNonceUE[0] = 0xdf;
    pucNonceUE[1] = 0xdd;
    pucNonceUE[2] = 0xf5;
    pucNonceUE[3] = 0xe9;
#else
    /* ���������*/
    for (i = 0; i < NAS_EMM_SECU_NONCE_UE_LEN; i++)
    {
        ulRandom = VOS_Rand(NAS_EMM_SECU_RANDOM_RANGE);
        pucNonceUE[i] = (VOS_UINT8)ulRandom;
    }
#endif

    return NAS_EMM_SECU_SUCC;
}


VOS_UINT32  NAS_EMM_DeriveKasmeFromUmtsIdle( VOS_UINT8 *pucKasme )
{
    VOS_UINT8                       aucAlgParam[NAS_EMM_SECU_MAX_ALG_PARAM_LEN];
    VOS_UINT8                       aucKeyParam[NAS_LMM_SECU_AUTH_KEY_ASME_LEN];
    VOS_UINT32                      ulIndex      = 0;
    VOS_UINT32                      ulRslt = NAS_EMM_SECU_FAIL;
    VOS_UINT8                      *pucNonceUE = VOS_NULL_PTR;
    VOS_UINT8                      *pucNonceMME = VOS_NULL_PTR;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_DeriveKasmeFromUmtsIdle entered.");

    /* ��ָ����*/
    if (VOS_NULL_PTR == pucKasme)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_DeriveKasmeFromUmtsIdle: NULL PTR");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ����KDF�㷨��Ҫ�Ĳ��� */

    /* encode FC*/
    aucAlgParam[ulIndex++] = NAS_EMM_SECU_ALG_PARAM_MAPPED_KASME_IDLE_FC;

    /* encode NONCEue&Len*/
    pucNonceUE = NAS_EMM_GetSecuNonceUEAddr();

    aucAlgParam[ulIndex++] = pucNonceUE[0];
    aucAlgParam[ulIndex++] = pucNonceUE[1];
    aucAlgParam[ulIndex++] = pucNonceUE[2];
    aucAlgParam[ulIndex++] = pucNonceUE[3];

    aucAlgParam[ulIndex++] = NAS_EMM_SECU_ALG_PARAM_KASME_PRE_LEN;
    aucAlgParam[ulIndex++] = NAS_EMM_SECU_NONCE_UE_LEN;

    /* encode NONCEmme&Len */
    pucNonceMME = NAS_EMM_GetSecuNonceMMEAddr();

    aucAlgParam[ulIndex++] = pucNonceMME[0];
    aucAlgParam[ulIndex++] = pucNonceMME[1];
    aucAlgParam[ulIndex++] = pucNonceMME[2];
    aucAlgParam[ulIndex++] = pucNonceMME[3];

    aucAlgParam[ulIndex++] = NAS_EMM_SECU_ALG_PARAM_KASME_PRE_LEN;
    aucAlgParam[ulIndex++] = NAS_EMM_SECU_NONCE_MME_LEN;

    /* encode UMTS CK||IK*/

    NAS_LMM_MEM_CPY((VOS_VOID *)aucKeyParam,NAS_EMM_GetUmtsCkAddr(),NAS_UMTS_CK_LENGTH);

    NAS_LMM_MEM_CPY((VOS_VOID *)&aucKeyParam[NAS_UMTS_CK_LENGTH],
                                      NAS_EMM_GetUmtsIkAddr(),NAS_UMTS_IK_LENGTH);

    ulRslt = NAS_EMM_KDF_HmacSha256(aucAlgParam,ulIndex,aucKeyParam,pucKasme);

    return ulRslt;

}


VOS_UINT32  NAS_EMM_DeriveKasmeFromUmtsHo( VOS_UINT8 *pucKasme )
{
    VOS_UINT8                       aucAlgParam[NAS_EMM_SECU_MAX_ALG_PARAM_LEN];
    VOS_UINT8                       aucKeyParam[NAS_LMM_SECU_AUTH_KEY_ASME_LEN];
    VOS_UINT32                      ulIndex      = 0;
    VOS_UINT32                      ulRslt = NAS_EMM_SECU_FAIL;
    VOS_UINT8                      *pucNonceMME = VOS_NULL_PTR;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_DeriveKasmeFromUmtsHo entered.");

    /* ��ָ����*/
    if (VOS_NULL_PTR == pucKasme)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_DeriveKasmeFromUmtsHo: NULL PTR");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }


    /* ����KDF�㷨��Ҫ�Ĳ��� */

    /* encode FC*/
    aucAlgParam[ulIndex++] = NAS_EMM_SECU_ALG_PARAM_MAPPED_KASME_HO_FC;

    /* encode NONCEmme&Len */
    pucNonceMME = NAS_EMM_GetSecuNonceMMEAddr();

    aucAlgParam[ulIndex++] = pucNonceMME[0];
    aucAlgParam[ulIndex++] = pucNonceMME[1];
    aucAlgParam[ulIndex++] = pucNonceMME[2];
    aucAlgParam[ulIndex++] = pucNonceMME[3];

    aucAlgParam[ulIndex++] = NAS_EMM_SECU_ALG_PARAM_KASME_PRE_LEN;
    aucAlgParam[ulIndex++] = NAS_EMM_SECU_NONCE_MME_LEN;

    /* encode UMTS CK||IK*/

    NAS_LMM_MEM_CPY((VOS_VOID *)aucKeyParam,NAS_EMM_GetUmtsCkAddr(),NAS_UMTS_CK_LENGTH);

    NAS_LMM_MEM_CPY((VOS_VOID *)&aucKeyParam[NAS_UMTS_CK_LENGTH],
                                      NAS_EMM_GetUmtsIkAddr(),NAS_UMTS_IK_LENGTH);

    ulRslt = NAS_EMM_KDF_HmacSha256(aucAlgParam,ulIndex,aucKeyParam,pucKasme);

    return ulRslt;

}
VOS_UINT32  NAS_EMM_DeriveUmtsCkIkFromEpsIdle(       VOS_UINT8 *pucUmtsCK,
                                                                   VOS_UINT8 *pucUmtsIk )
{
    VOS_UINT8                       aucAlgParam[NAS_EMM_SECU_MAX_ALG_PARAM_LEN];
    VOS_UINT8                       aucKeyParam[NAS_LMM_SECU_AUTH_KEY_ASME_LEN];
    VOS_UINT32                      ulIndex      = 0;
    VOS_UINT32                      ulRslt = NAS_EMM_SECU_FAIL;
    VOS_UINT8                       aucUmtsCKIK[NAS_LMM_SECU_AUTH_KEY_ASME_LEN];
    VOS_UINT8                      *pucEpsKasme = VOS_NULL_PTR;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_DeriveUmtsCkIkFromEpsIdle entered.");

    /* ��ָ����*/
    if ((VOS_NULL_PTR == pucUmtsCK)||(VOS_NULL_PTR == pucUmtsIk))
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_DeriveUmtsCkIkFromEpsIdle: NULL PTR");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ����KDF�㷨��Ҫ�Ĳ��� */

    /* encode FC*/
    aucAlgParam[ulIndex++] = NAS_EMM_SECU_ALG_PARAM_UMTS_CKIK_IDLE_FC;

    /* encode UP_NASCOUNT&Len */

    /*����UP_NASCOUNTΪP0*/
    NAS_SECU_SET_32BIT_PARA(&(aucAlgParam[ulIndex]), NAS_EMM_GetSecuUlNasCount());
    ulIndex += NAS_EMM_SECU_NASCOUNT_LEN;

    aucAlgParam[ulIndex++] = NAS_EMM_SECU_ALG_PARAM_KASME_PRE_LEN;
    aucAlgParam[ulIndex++] = NAS_EMM_SECU_NASCOUNT_LEN;

    /* encode Kasme*/
    pucEpsKasme = NAS_EMM_GetSecuCurKasmeAddr();

    NAS_LMM_MEM_CPY(         (VOS_VOID *)aucKeyParam,
                                        pucEpsKasme,
                                        NAS_LMM_SECU_AUTH_KEY_ASME_LEN);

    ulRslt = NAS_EMM_KDF_HmacSha256(aucAlgParam,ulIndex,aucKeyParam,aucUmtsCKIK);

    if (NAS_EMM_SECU_SUCC == ulRslt)
    {
        /* UmtsCK*/
        NAS_LMM_MEM_CPY(pucUmtsCK, aucUmtsCKIK, NAS_UMTS_CK_LENGTH);

        /* UmtsIK*/
        NAS_LMM_MEM_CPY(pucUmtsIk, &(aucUmtsCKIK[NAS_UMTS_CK_LENGTH]), NAS_UMTS_IK_LENGTH);

        return NAS_EMM_SECU_SUCC;
    }
    else
    {
        return NAS_EMM_SECU_FAIL;
    }

}


VOS_UINT32  NAS_EMM_DeriveUmtsCkIkFromEpsHo( VOS_UINT8 *pucUmtsCK,
                                                            VOS_UINT8 *pucUmtsIk)
{
    VOS_UINT8                       aucAlgParam[NAS_EMM_SECU_MAX_ALG_PARAM_LEN];
    VOS_UINT8                       aucKeyParam[NAS_LMM_SECU_AUTH_KEY_ASME_LEN];
    VOS_UINT32                      ulIndex      = 0;
    VOS_UINT32                      ulRslt = NAS_EMM_SECU_FAIL;
    VOS_UINT8                       aucUmtsCKIK[NAS_LMM_SECU_AUTH_KEY_ASME_LEN];
    VOS_UINT8                      *pucEpsKasme = VOS_NULL_PTR;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_DeriveUmtsCkIkFromEpsHo entered.");

    /* ��ָ����*/
    if ((VOS_NULL_PTR == pucUmtsCK)||(VOS_NULL_PTR == pucUmtsIk))
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_DeriveUmtsCkIkFromEpsHo: NULL PTR");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ����KDF�㷨��Ҫ�Ĳ��� */

    /* encode FC*/
    aucAlgParam[ulIndex++] = NAS_EMM_SECU_ALG_PARAM_UMTS_CKIK_HO_FC;

    /* encode DL_NASCOUNT&Len */

    /*����DL_NASCOUNTΪP0*/
    NAS_SECU_SET_32BIT_PARA(&(aucAlgParam[ulIndex]), NAS_EMM_GetSecuDlNasCount());
    ulIndex += NAS_EMM_SECU_NASCOUNT_LEN;

    aucAlgParam[ulIndex++] = NAS_EMM_SECU_ALG_PARAM_KASME_PRE_LEN;
    aucAlgParam[ulIndex++] = NAS_EMM_SECU_NASCOUNT_LEN;

    /* encode Kasme*/
    pucEpsKasme = NAS_EMM_GetSecuCurKasmeAddr();

    NAS_LMM_MEM_CPY(         (VOS_VOID *)aucKeyParam,
                                        pucEpsKasme,
                                        NAS_LMM_SECU_AUTH_KEY_ASME_LEN);

    ulRslt = NAS_EMM_KDF_HmacSha256(aucAlgParam,ulIndex,aucKeyParam,aucUmtsCKIK);

    if (NAS_EMM_SECU_SUCC == ulRslt)
    {
        /* UmtsCK*/
        NAS_LMM_MEM_CPY(pucUmtsCK, aucUmtsCKIK, NAS_UMTS_CK_LENGTH);

        /* UmtsIK*/
        NAS_LMM_MEM_CPY(pucUmtsIk, &(aucUmtsCKIK[NAS_UMTS_CK_LENGTH]), NAS_UMTS_IK_LENGTH);

        return NAS_EMM_SECU_SUCC;
    }
    else
    {
        return NAS_EMM_SECU_FAIL;
    }

}


VOS_UINT32  NAS_EMM_DeriveUmtsCkIkFromEpsSrvcc( VOS_UINT8 *pucUmtsCK,
                                                            VOS_UINT8 *pucUmtsIk)
{
    VOS_UINT8                       aucAlgParam[NAS_EMM_SECU_MAX_ALG_PARAM_LEN];
    VOS_UINT8                       aucKeyParam[NAS_LMM_SECU_AUTH_KEY_ASME_LEN];
    VOS_UINT32                      ulIndex      = 0;
    VOS_UINT32                      ulRslt = NAS_EMM_SECU_FAIL;
    VOS_UINT8                       aucUmtsCKIK[NAS_LMM_SECU_AUTH_KEY_ASME_LEN];
    VOS_UINT8                      *pucEpsKasme = VOS_NULL_PTR;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_DeriveUmtsCkIkFromEpsSrvcc entered.");

    /* ��ָ����*/
    if ((VOS_NULL_PTR == pucUmtsCK)||(VOS_NULL_PTR == pucUmtsIk))
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_DeriveUmtsCkIkFromEpsSrvcc: NULL PTR");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ����KDF�㷨��Ҫ�Ĳ��� */

    /* encode FC*/
    aucAlgParam[ulIndex++] = NAS_EMM_SECU_ALG_PARAM_UMTS_CKIK_SRVCC_FC;

    /* encode DL_NASCOUNT&Len */

    /*����DL_NASCOUNTΪP0*/
    NAS_SECU_SET_32BIT_PARA(&(aucAlgParam[ulIndex]), NAS_EMM_GetSecuDlNasCount());
    ulIndex += NAS_EMM_SECU_NASCOUNT_LEN;

    aucAlgParam[ulIndex++] = NAS_EMM_SECU_ALG_PARAM_KASME_PRE_LEN;
    aucAlgParam[ulIndex++] = NAS_EMM_SECU_NASCOUNT_LEN;

    /* encode Kasme*/
    pucEpsKasme = NAS_EMM_GetSecuCurKasmeAddr();

    NAS_LMM_MEM_CPY(         (VOS_VOID *)aucKeyParam,
                                        pucEpsKasme,
                                        NAS_LMM_SECU_AUTH_KEY_ASME_LEN);

    ulRslt = NAS_EMM_KDF_HmacSha256(aucAlgParam,ulIndex,aucKeyParam,aucUmtsCKIK);

    if (NAS_EMM_SECU_SUCC == ulRslt)
    {
        /* UmtsCK*/
        NAS_LMM_MEM_CPY(pucUmtsCK, aucUmtsCKIK, NAS_UMTS_CK_LENGTH);

        /* UmtsIK*/
        NAS_LMM_MEM_CPY(pucUmtsIk, &(aucUmtsCKIK[NAS_UMTS_CK_LENGTH]), NAS_UMTS_IK_LENGTH);

        return NAS_EMM_SECU_SUCC;
    }
    else
    {
        return NAS_EMM_SECU_FAIL;
    }

}



VOS_UINT32  NAS_EMM_DeriveNasToken( VOS_UINT8 *pucNasToken )
{
    VOS_UINT8                       aucAlgParam[NAS_EMM_SECU_MAX_ALG_PARAM_LEN];
    VOS_UINT8                       aucKeyParam[NAS_LMM_SECU_AUTH_KEY_ASME_LEN];
    VOS_UINT32                      ulIndex      = 0;
    VOS_UINT32                      ulRslt = NAS_EMM_SECU_FAIL;
    VOS_UINT8                      *pucEpsKasme = VOS_NULL_PTR;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_DeriveNasToken entered.");

    /* ��ָ����*/
    if (VOS_NULL_PTR == pucNasToken)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_DeriveNasToken: NULL PTR");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ����KDF�㷨��Ҫ�Ĳ��� */

    /* encode FC*/
    aucAlgParam[ulIndex++] = NAS_EMM_SECU_ALG_PARAM_NAS_TOKEN_FC;

    /* encode UP_NASCOUNT&Len */

    /*����UP_NASCOUNTΪP0*/
    NAS_SECU_SET_32BIT_PARA(&(aucAlgParam[ulIndex]), NAS_EMM_GetSecuUlNasCount());
    ulIndex += NAS_EMM_SECU_NASCOUNT_LEN;

    aucAlgParam[ulIndex++] = NAS_EMM_SECU_ALG_PARAM_KASME_PRE_LEN;
    aucAlgParam[ulIndex++] = NAS_EMM_SECU_NASCOUNT_LEN;

    /* encode Kasme*/
    pucEpsKasme = NAS_EMM_GetSecuCurKasmeAddr();

    NAS_LMM_MEM_CPY(         (VOS_VOID *)aucKeyParam,
                                        pucEpsKasme,
                                        NAS_LMM_SECU_AUTH_KEY_ASME_LEN);

    ulRslt = NAS_EMM_KDF_HmacSha256(aucAlgParam,ulIndex,aucKeyParam,pucNasToken);

    return ulRslt;

}


VOS_UINT32 NAS_EMM_KDF_HmacSha256
(
    VOS_UINT8 *pucBuf,
    VOS_UINT32 ulBuflen,
    VOS_UINT8 *pucKey,
    VOS_UINT8 aucDigest[NAS_LMM_SECU_AUTH_KEY_ASME_LEN]
)
{
    KEY_CONFIG_INFO_S                   stKeyCfgInfo;
    S_CONFIG_INFO_S                     stSCfgInfo;
    BSP_S32                             lRslt;
    KEY_MAKE_S                          stKeyMake;
    CIPHER_KEY_LEN_E_U32                ulenKeyLen;
    /*PC REPLAY MODIFY BY LEILI BEGIN*/
    NAS_LMM_BSP_INPUT_PARA_STRU         stBspInputPara = {0};
    NAS_LMM_BSP_OUTPUT_PARA_STRU        stBspOutputPara= {0};
    /*PC REPLAY MODIFY BY LEILI END*/
    NAS_EMM_SECU_LOG_INFO("NAS_EMM_KDF_HmacSha256 entered.");

    (void)aucDigest;
    if ((NAS_EMM_NULL_PTR == pucBuf) ||
        (NAS_EMM_NULL_PTR == pucKey) ||
        (NAS_EMM_NULL_PTR == aucDigest))
    {
        NAS_EMM_SECU_LOG_INFO("NAS_EMM_KDF_HmacSha256: Input Para is valid");
        return NAS_EMM_SECU_FAIL;
    }

    NAS_LMM_MEM_SET(&stKeyCfgInfo,NAS_EMM_NULL,sizeof(KEY_CONFIG_INFO_S));
    NAS_LMM_MEM_SET(&stSCfgInfo,NAS_EMM_NULL,sizeof(S_CONFIG_INFO_S));

    /*��ֵKEY ������Ϣ*/
    stKeyCfgInfo.enShaKeySource         = SHA_KEY_SOURCE_DDR;
    stKeyCfgInfo.u32ShaKeyIndex         = NAS_EMM_NULL;
    stKeyCfgInfo.pKeySourceAddr         = pucKey;

    /*��ֵS ������Ϣ*/
    stSCfgInfo.enShaSSource             = SHA_S_SOURCE_DDR;
    stSCfgInfo.u32ShaSIndex             = NAS_EMM_NULL;
    stSCfgInfo.u32ShaSLength            = ulBuflen;
    stSCfgInfo.pSAddr                   = pucBuf;

    /* �����Կ�Ľṹ׼�� */
    stKeyMake.enKeyOutput               = CIPHER_KEY_OUTPUT;
    stKeyMake.stKeyGet.enKeyLen         = CIPHER_KEY_L256;
    stKeyMake.stKeyGet.pKeyAddr         = aucDigest;    /* ��Կ�����������ַ */
    stKeyMake.stKeyGet.penOutKeyLen     = &ulenKeyLen;  /* ��Կ���ȵ������ַ */

    /*leili add 2313-5-8 begin*/
    /*PC�طŹ�������Ĳ���*/
    stBspInputPara.ulBuflen = ulBuflen;
    NAS_LMM_MEM_CPY(                    stBspInputPara.aucAlgParam,
                                        pucBuf,
                                        ulBuflen);
    NAS_LMM_MEM_CPY(                    stBspInputPara.aucKeyParam,
                                        pucKey,
                                        NAS_LMM_SECU_AUTH_KEY_ASME_LEN);

    /*leili add 2313-5-8 end*/

    /*zhengjunyan add print begin*/
    vos_printf("hard input sAddr: 0x%x",stSCfgInfo.pSAddr);
    vos_printf("hard input KeyAddr: 0x%x",stKeyCfgInfo.pKeySourceAddr);

    NAS_EMM_SECU_LOG1_INFO("hard input s len:",stSCfgInfo.u32ShaSLength);
    NAS_EMM_SECU_LOG_INFO("hard input s:");
    NAS_COMM_PrintArray(                NAS_COMM_GET_MM_PRINT_BUF(),
                                        stSCfgInfo.pSAddr ,
                                        stSCfgInfo.u32ShaSLength);
    NAS_EMM_SECU_LOG_INFO("hard input key:");
    NAS_COMM_PrintArray(                NAS_COMM_GET_MM_PRINT_BUF(),
                                        stKeyCfgInfo.pKeySourceAddr ,
                                        32);
    /*zhengjunyan add print end*/
    /*PC REPLAY MODIFY BY LEILI BEGIN*/
    #ifndef __PS_WIN32_RECUR__
    lRslt = BSP_KDF_KeyMake(            &stKeyCfgInfo,
                                        &stSCfgInfo,
                                        LTE_SECU_LMM_KDF_OUTPUT_INDEX,
                                        &stKeyMake);
    if (BSP_OK == lRslt)
    {
        NAS_EMM_SECU_LOG2_INFO("NAS_EMM_KDF_HmacSha256:  enKeyOutput = , enKeyLen = ",
                                        stKeyMake.enKeyOutput,
                                        stKeyMake.stKeyGet.enKeyLen);

        NAS_EMM_SECU_LOG2_INFO("NAS_EMM_KDF_HmacSha256:  pKeyAddr = , penOutKeyLen = ",
                                        stKeyMake.stKeyGet.pKeyAddr,
                                        stKeyMake.stKeyGet.penOutKeyLen);

        NAS_EMM_SECU_LOG1_INFO("NAS_EMM_KDF_HmacSha256: from BSP_KDF_KeyMake, KeyLen = , key = ",
                                        ulenKeyLen);
        NAS_COMM_PrintArray(NAS_COMM_GET_MM_PRINT_BUF(),
                                        aucDigest,
                                        NAS_LMM_SECU_AUTH_KEY_ASME_LEN);


        NAS_LMM_LogBspKdfKeyMake(&stBspInputPara,&stBspOutputPara,lRslt);
        return NAS_EMM_SECU_SUCC;
    }
    else
    {
        NAS_EMM_SECU_LOG1_INFO("NAS_EMM_KDF_HmacSha256: from BSP_KDF_KeyMake, err! lRslt = ",
                                        lRslt);
        NAS_LMM_LogBspKdfKeyMake(&stBspInputPara,&stBspOutputPara,lRslt);
        return NAS_EMM_SECU_FAIL;
    }
    #else

    lRslt = Stub_BSP_KDF_KeyMake(stBspInputPara, &stBspOutputPara);
    if (BSP_OK == lRslt)
    {
        return NAS_EMM_SECU_SUCC;
    }
    else
    {
        return NAS_EMM_SECU_FAIL;
    }

    #endif
    /*PC REPLAY MODIFY BY LEILI END*/

}


VOS_VOID  NAS_EMM_SecuSetInterRatHoNotMapped( VOS_VOID )
{
    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuSetInterRatHoNotMapped entered.");
    NAS_EMM_SetFlagInterRatHoMapped(NAS_EMM_SECU_INTER_RAT_HO_NOT_MAPPED);
}


VOS_VOID  NAS_EMM_SecuCurrentContextUpNasCountBack(VOS_VOID)
{
    if(NAS_EMM_SMC_OPEN == g_ulSmcControl)
    {
        if((EMM_CUR_SECU_EXIST_ACTIVE == NAS_LMM_GetSecuCurFsmCS())
            || (EMM_CUR_SECU_EXIST_NOT_ACTIVE == NAS_LMM_GetSecuCurFsmCS()))
        {
            if(0 < NAS_EMM_GetSecuUlNasCount())
            {
                NAS_EMM_GetSecuUlNasCount()--;
            }

        }
    }

    return;
}
/*lint +e961*/
/*lint +e960*/




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

