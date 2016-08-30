


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include    "NasEmmSecuInclude.h"
#include    "NasLmmPubMPrint.h"
#include    "NasEmmTauSerInclude.h"
#include    "NasEmmAttDetInclude.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASEMMSECUSMC_C
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
VOS_UINT8                           g_aucEia0Kasme[NAS_LMM_SECU_AUTH_KEY_ASME_LEN] = {
        0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,
        0x19,0x84,0x05,0x14,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,
        0x66,0x66,0x66,0x66,};

/*****************************************************************************
  3 Function
*****************************************************************************/


VOS_VOID  NAS_EMM_DeriveNewNasKey(      VOS_UINT8  *pucKasme,
                                        VOS_UINT8   ucEca,
                                        VOS_UINT8   ucEia)
{
    VOS_UINT32                          ulRslt = NAS_EMM_SECU_FAIL;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_DeriveNewNasKey entered.");

    /*����NasCk*/
    ulRslt = NAS_EMM_ComputeNasKey(     pucKasme,
                                        NAS_EMM_GetSecuTempNasCkAddr(),
                                        NAS_EMM_SECU_CIPHER_KEY,
                                        ucEca);
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_DeriveNewNasKey:Compute NasCK Err.");
    }

    /*����NasIk*/
    ulRslt = NAS_EMM_ComputeNasKey(     pucKasme,
                                        NAS_EMM_GetSecuTempNasIkAddr(),
                                        NAS_EMM_SECU_INTEGRITY_KEY,
                                        ucEia);
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_DeriveNewNasKey:Compute NasIK Err.");
    }

    return;
}


VOS_UINT32  NAS_EMM_ComputeNasKey(  VOS_UINT8  *pucKasme,
                                    VOS_UINT8  *pNasK,
                                    NAS_EMM_SECU_NAS_KEY_TYPE_ENUM_UINT8 enNasKeyType,
                                    VOS_UINT8   enProtAlgoType)
{
    VOS_UINT8                       aucAlgParam[NAS_EMM_SECU_MAX_ALG_PARAM_LEN];
    VOS_UINT8                       aucKeyParam[NAS_LMM_SECU_AUTH_KEY_ASME_LEN];
    VOS_UINT32                      ulIndex = 0;
    VOS_UINT32                      ulRslt = NAS_EMM_SECU_FAIL;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_ComputeNasKey entered.");


    if(VOS_NULL_PTR == pucKasme)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_ComputeNasKey:-> pucKasme is null.");
        return NAS_EMM_SECU_FAIL;
    }

    /* ����KDF�㷨��Ҫ�Ĳ��� */

    /*encode FC*/
    aucAlgParam[ulIndex++]          = NAS_EMM_SECU_ALG_PARAM_NAS_KEY_FC;

    if (NAS_EMM_SECU_CIPHER_KEY == enNasKeyType)
    {
        /* encode distinguisher */
        aucAlgParam[ulIndex++]      = NAS_EMM_SECU_ALG_PARAM_CK_DISTINGUISHER;

        aucAlgParam[ulIndex++]      = NAS_EMM_SECU_ALG_PARAM_KASME_PRE_LEN;
        aucAlgParam[ulIndex++]      = NAS_EMM_SECU_ALG_PARAM_DISTINGUISHER_LEN;

        /* encode algorithm identity */
        aucAlgParam[ulIndex++] = enProtAlgoType;

        aucAlgParam[ulIndex++]     = NAS_EMM_SECU_ALG_PARAM_KASME_PRE_LEN;
        aucAlgParam[ulIndex++]     = NAS_EMM_SECU_ALG_PARAM_IDENTITY_LEN;
    }
    else
    {
        /* encode distinguisher */
        aucAlgParam[ulIndex++]     = NAS_EMM_SECU_ALG_PARAM_IK_DISTINGUISHER;

        aucAlgParam[ulIndex++]     = NAS_EMM_SECU_ALG_PARAM_KASME_PRE_LEN;
        aucAlgParam[ulIndex++]     = NAS_EMM_SECU_ALG_PARAM_DISTINGUISHER_LEN;

        /* encode algorithm identity */
        aucAlgParam[ulIndex++] = enProtAlgoType;

        aucAlgParam[ulIndex++]     = NAS_EMM_SECU_ALG_PARAM_KASME_PRE_LEN;
        aucAlgParam[ulIndex++]     = NAS_EMM_SECU_ALG_PARAM_IDENTITY_LEN;
    }

    ulRslt = NAS_EMM_KDF_HmacSha256(aucAlgParam,ulIndex,pucKasme,aucKeyParam);

    if (NAS_EMM_SECU_SUCC == ulRslt)
    {
        PS_GetLeastSignBitsByTrunc(aucKeyParam,pNasK);
        return NAS_EMM_SECU_SUCC;
    }
    else
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_ComputeNasKey->PS_KDF_HmacSha256 run Fail!");
        return NAS_EMM_SECU_FAIL;
    }
}


/*lint -e960*/
/*lint -e961*/
VOS_VOID  NAS_EMM_UpdateCurSecuCntxt(VOS_VOID)
{
    NAS_EMM_SECU_LOG_INFO("NAS_EMM_UpdateCurSecuCntxt enterd.");

    /*�� new mapped secu cntxt����Ϊ��ǰ��ȫ������*/
    if(NAS_EMM_SMC_UPDATE_NEW_MAPPED == NAS_EMM_GetSmcType())
    {
        NAS_EMM_SetSecuCurKSIasme(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);
        NAS_EMM_SetSecuCurKSIsgsn(NAS_EMM_GetSecuNewMappedKSIsgsn());
        NAS_LMM_MEM_CPY(                NAS_EMM_GetSecuCurKasmeAddr(),
                                        NAS_EMM_GetSecuNewMappedKasmeAddr(),
                                        NAS_LMM_SECU_AUTH_KEY_ASME_LEN);
        /*��� new mapped security context*/
        NAS_EMM_ClearNewMappedSecuCntxt();
    }
    /*�� new native secu cntxt����Ϊ��ǰ��ȫ������*/
    else if(NAS_EMM_SMC_UPDATE_NEW_NATIVE == NAS_EMM_GetSmcType())
    {
        NAS_EMM_SetSecuCurKSIsgsn(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);
        NAS_EMM_SetSecuCurKSIasme(NAS_EMM_GetSecuNewNativeKSIasme());
        NAS_LMM_MEM_CPY(                NAS_EMM_GetSecuCurKasmeAddr(),
                                        NAS_EMM_GetSecuNewNativeKasmeAddr(),
                                        NAS_LMM_SECU_AUTH_KEY_ASME_LEN);
        /*��� new native security context*/
        NAS_EMM_ClearNewNativeSecuCntxt();

        /*��� new mapped security context*/
        NAS_EMM_ClearNewMappedSecuCntxt();

    }
    /*�� non-current native secu cntxt����Ϊ��ǰ��ȫ������*/
    else
    {
        NAS_EMM_SetSecuCurKSIsgsn(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);
        NAS_EMM_SetSecuCurKSIasme(NAS_EMM_GetSecuNonCurNativeKSIasme());
        NAS_LMM_MEM_CPY(                NAS_EMM_GetSecuCurKasmeAddr(),
                                        NAS_EMM_GetSecuNonCurNativeKasmeAddr(),
                                        NAS_LMM_SECU_AUTH_KEY_ASME_LEN);
        /*��� non-current native security context*/
        NAS_EMM_ClearNonCurNativeSecuCntxt();
    }


    NAS_EMM_SECU_LOG2_INFO("NAS_EMM_UpdateCurSecuCntxt: CurKSIasme = , CurKSIsgsn = .",
                        NAS_EMM_GetSecuCurKSIasme(),
                        NAS_EMM_GetSecuCurKSIsgsn());


   return;
}
VOS_UINT32 NAS_EMM_Eia0SmcVerifyKsi
(
    VOS_UINT8                           ucKsi
)
{
    VOS_UINT8                           ucKsiValue = NAS_EMM_NULL;
    VOS_UINT8                           ucKsiType  = NAS_EMM_NULL;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_Eia0SmcVerifyKsi enter.");

    /* ��ȡ KSI���ͺ� KSI ��ֵ*/
    ucKsiValue  = ucKsi & NAS_EMM_LOW_3_BITS_F;
    ucKsiType   = ucKsi>>NAS_EMM_SECU_KSI_TYPE_BIT_POS;
    if(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE == ucKsiValue)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_Eia0SmcVerifyKsi:KSI is NO_KEY.");
        return NAS_EMM_SECU_FAIL;
    }

    /* Ϊ�˼��ݲ�ͬ���磬ʵ�п���ϳ�ԭ���ݲ��ж�KSI�Ƿ�Ϊ0 */
    if(0 != ucKsiValue)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_Eia0SmcVerifyKsi:KSI is not zero.");
    }

    /* Ϊ�˼��ݲ�ͬ���磬ʵ�п���ϳ�ԭ���ݲ��ж��Ƿ�ΪNATIVE */
    if (NAS_EMM_SECU_KSI_TYPE_NATIVE != ucKsiType)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_Eia0SmcVerifyKsi:KSI type is not native.");
    }

    NAS_EMM_SetSmcType(NAS_EMM_SMC_UPDATE_CUR);

    return  NAS_EMM_SECU_SUCC;
}
VOS_UINT32 NAS_EMM_NotEia0SmcVerifyKsi
(
    VOS_UINT8                           ucSqn,
    VOS_UINT8                           ucKsi
)
{
    VOS_UINT8                               ucKsiType;
    VOS_UINT8                               ucKsiValue;
    VOS_UINT32                              ulRslt;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_NotEia0SmcVerifyKsi enter.");

    /* ��ȡ KSI���ͺ� KSI ��ֵ*/
    ucKsiType   = ucKsi>>NAS_EMM_SECU_KSI_TYPE_BIT_POS;
    ucKsiValue  = ucKsi&NAS_EMM_LOW_3_BITS_F;
    if(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE == ucKsiValue)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_NotEia0SmcVerifyKsi:KSI is NO_KEY.");
        return NAS_EMM_SECU_FAIL;
    }

    /* ���SMCЯ����KSIΪ native ����*/
    if(NAS_EMM_SECU_KSI_TYPE_NATIVE == ucKsiType)
    {
        NAS_EMM_SECU_LOG_NORM("NAS_EMM_NotEia0SmcVerifyKsi:KSI is Native.");
        ulRslt = NAS_EMM_SmcVerifyNativeKsi(ucKsiValue);
    }
    else/* ���SMCЯ����KSIΪ Mapped ����*/
    {
        NAS_EMM_SECU_LOG_NORM("NAS_EMM_NotEia0SmcVerifyKsi:KSI is Mapped.");
        ulRslt = NAS_EMM_SmcVerifyMappedKsi(ucSqn, ucKsiValue);
    }

    return ulRslt;
}
VOS_UINT32  NAS_EMM_SmcVerifyNativeKsi( VOS_UINT8 ucKsiValue)
{
    NAS_EMM_SECU_LOG1_INFO("NAS_EMM_SmcVerifyNativeKsi: ucKsiValue = ", ucKsiValue);
    if(NAS_EMM_GetSecuNewNativeKSIasme() == ucKsiValue)
    {
        NAS_EMM_SetSmcType(NAS_EMM_SMC_UPDATE_NEW_NATIVE);
    }
    else if (NAS_EMM_GetSecuCurKSIasme() == ucKsiValue)
    {
        NAS_EMM_SetSmcType(NAS_EMM_SMC_UPDATE_CUR);
    }
    else if(NAS_EMM_GetSecuNonCurNativeKSIasme() == ucKsiValue)
    {
        NAS_EMM_SetSmcType(NAS_EMM_SMC_UPDATE_NON_CUR_NATIVE);
    }
    else
    {
        NAS_EMM_SECU_LOG_NORM("NAS_EMM_SmcVerifyNativeKsi: NAS_EMM_SECU_FAIL");
        return  NAS_EMM_SECU_FAIL;
    }

    return  NAS_EMM_SECU_SUCC;
}


VOS_UINT32  NAS_EMM_SmcVerifyMappedKsi
(
    VOS_UINT8                           ucSqn,
    VOS_UINT8                           ucKsiValue
)
{
    /* ���SQN�պ�Ϊ0��GU��ѡ��Lʱ����TAU��Ϣ��current��ȫ�����ĵ�KSI��mapped cksn��ȣ�
       ����SMC��Ϣ�е�KSI���ڴ˶��ߣ�������ʹ��new mapped����˴�����Ϊ�˹��
       �����·�SMC��ϢʱUE�޷�����ʹ�����ף�������������ʱ����ʹ��new mapped */
    if(    (0 == ucSqn)
        && (NAS_EMM_GetSecuNewMappedKSIsgsn() == NAS_EMM_GetSecuCurKSIsgsn())
        && (NAS_EMM_GetSecuNewMappedKSIsgsn() == ucKsiValue)   )
    {
        NAS_EMM_SECU_LOG_NORM("NAS_EMM_SmcVerifyMappedKsi: Update New mapped security context.");
        NAS_EMM_SetSmcType(NAS_EMM_SMC_UPDATE_NEW_MAPPED);
        return NAS_EMM_SECU_SUCC;
    }

    /* ����������������������� */
    if (NAS_EMM_GetSecuCurKSIsgsn() == ucKsiValue)
    {
        NAS_EMM_SECU_LOG_NORM("NAS_EMM_SmcVerifyMappedKsi: Update current mapped security context.");
        NAS_EMM_SetSmcType(NAS_EMM_SMC_UPDATE_CUR);
    }
    else if(NAS_EMM_GetSecuNewMappedKSIsgsn() == ucKsiValue)
    {
        NAS_EMM_SetSmcType(NAS_EMM_SMC_UPDATE_NEW_MAPPED);
    }
    else
    {
        NAS_EMM_SECU_LOG_NORM("NAS_EMM_SmcVerifyMappedKsi:NAS_EMM_SECU_FAIL.");
        return  NAS_EMM_SECU_FAIL;
    }

    return  NAS_EMM_SECU_SUCC;
}
VOS_UINT32 NAS_EMM_Eia0SmcVerifyAlg( VOS_UINT8 ucSelEca, VOS_UINT8 ucSelEia)
{
    if (NAS_EMM_SECU_ALG_EEA0 != ucSelEca)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_Eia0SmcVerifyAlg:NOT EEA0");
        return NAS_EMM_SECU_FAIL;
    }

    return NAS_EMM_IsAlgUeSupported(ucSelEca, ucSelEia);
}


VOS_UINT32 NAS_EMM_IsAlgUeSupported( VOS_UINT8 ucSelEca, VOS_UINT8 ucSelEia)
{
    NAS_MM_UE_NET_CAP_STRU             *pstUeNetCap = NAS_EMM_NULL_PTR;
    VOS_UINT8                           ucEEA;
    VOS_UINT8                           ucEIA;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_IsAlgUeSupported entered.");

    /*��ȡUE_NET_CAP*/
    pstUeNetCap = NAS_LMM_GetEmmInfoSecuParaUeNetCapAddr();

    /*��������Ա������߼����㷨��UE��֧��*/
    ucEEA = pstUeNetCap->aucUeNetCap[NAS_EMM_UE_NET_CAP_EEA_POS];
    ucEIA = pstUeNetCap->aucUeNetCap[NAS_EMM_UE_NET_CAP_EIA_POS];
    if((NAS_EMM_BIT_NO_SLCT == ((ucEIA << ucSelEia)& NAS_EMM_SECU_HIGHEST_BIT))
      ||(NAS_EMM_BIT_NO_SLCT == ((ucEEA << ucSelEca)& NAS_EMM_SECU_HIGHEST_BIT)))

    {
        return NAS_EMM_SECU_FAIL;
    }

    return NAS_EMM_SECU_SUCC;
}

CIPHER_ALGORITHM_E NAS_EMM_GetIntegrityAlg( VOS_UINT8 ucSelEia)
{

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_GetIntegrityAlg entered.");

    switch(ucSelEia)
    {
        case    NAS_EMM_SECU_ALG_EIA0:
            return CIPHER_ALG_NULL;

        case    NAS_EMM_SECU_ALG_EIA1:
            return CIPHER_ALG_SNOW3G;

        case    NAS_EMM_SECU_ALG_EIA2:
            return CIPHER_ALG_AES_128;

        case    NAS_EMM_SECU_ALG_EIA3:
            return CIPHER_ALG_ZUC;

        default:
            NAS_EMM_SECU_LOG_INFO("NAS_EMM_GetIntegrityAlg entered: ucSelEia err");
            return CIPHER_ALG_BUTTOM;

    }
}

CIPHER_ALGORITHM_E NAS_EMM_GetSecurityAlg( VOS_UINT8 ucSelEca)
{

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_GetSecurityAlg entered.");

    switch(ucSelEca)
    {
        case    NAS_EMM_SECU_ALG_EEA0:

            return CIPHER_ALG_NULL;

        case    NAS_EMM_SECU_ALG_EEA1:
            return CIPHER_ALG_SNOW3G;

        case    NAS_EMM_SECU_ALG_EEA2:
            return CIPHER_ALG_AES_128;

        case    NAS_EMM_SECU_ALG_EEA3:
            return CIPHER_ALG_ZUC;

        default:
            NAS_EMM_SECU_LOG_INFO("NAS_EMM_GetSecurityAlg entered: ucSelEca err");
            return CIPHER_ALG_BUTTOM;

    }
}



VOS_UINT32  NAS_EMM_SmcVerifyUeSecuCap(
                        const NAS_LMM_UE_SECURITY_CAP_STRU    *pstUeSecuCap )
{
    NAS_MM_UE_NET_CAP_STRU             *pstLocalUeNetCap = VOS_NULL_PTR;
    NAS_MM_MS_NET_CAP_STRU             *pstLocalMsNetCap = VOS_NULL_PTR;
    NAS_LMM_UE_SECURITY_CAP_STRU        stLocalUeSecuCap = {0};

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SmcVerifyUeSecuCap entered.");

    if(NAS_EMM_SMC_CLOSE== g_ulUeSecuCapMatch)
    {
        NAS_EMM_SECU_LOG_INFO("NAS_EMM_SmcVerifyUeSecuCap: g_ulUeSecuCapMatch = NAS_EMM_SMC_CLOSE");
        return NAS_EMM_SECU_SUCC;
    }

    /* ��ȡ UE ���ص� UE_NET_CAP�� MS_NET_CAP*/
    pstLocalUeNetCap = NAS_LMM_GetEmmInfoSecuParaUeNetCapAddr();
    pstLocalMsNetCap = NAS_LMM_GetEmmInfoMsNetCapAddr();


    /* ����Ƕ�ģģʽ*/
    if (NAS_EMM_SUCC == NAS_EMM_CheckMutiModeSupport())
    {
        /*stLocalUeSecuCap.ucUeSeuCapLen = pstLocalUeNetCap->ucUeNetCapLen + 1;*/
        stLocalUeSecuCap.ucEEA = pstLocalUeNetCap->aucUeNetCap[NAS_EMM_UE_NET_CAP_EEA_POS];
        stLocalUeSecuCap.ucEIA = pstLocalUeNetCap->aucUeNetCap[NAS_EMM_UE_NET_CAP_EIA_POS];
        stLocalUeSecuCap.ucUEA = pstLocalUeNetCap->aucUeNetCap[NAS_EMM_UE_NET_CAP_UEA_POS];
        stLocalUeSecuCap.ucUIA = (pstLocalUeNetCap->aucUeNetCap[NAS_EMM_UE_NET_CAP_UIA_POS])&NAS_EMM_7_BITS_F;
        stLocalUeSecuCap.ucGEA = ((pstLocalMsNetCap->aucMsNetCap[0]&NAS_EMM_HIGHES_BIT)
                                  >>NAS_LMM_MOVEMENT_1_BITS);
        stLocalUeSecuCap.ucGEA |= ((pstLocalMsNetCap->aucMsNetCap[1]&NAS_EMM_6_BITS_F)
                                  >>NAS_LMM_MOVEMENT_1_BITS);
#if 0
        /*���֧��GSM������������Ϊ5*/
        if(stLocalUeSecuCap.ucGEA != 0)
        {
        	stLocalUeSecuCap.ucUeSeuCapLen = 5;
        }
        /*��֧��GSM*/
        else
        {
        	/*���UE�����������ȴﵽ5����SMC��������Ӧ��Ϊ4������SMC��������ʵ�ʵ�UE������������*/
            if(5 == pstLocalUeNetCap->ucUeNetCapLen)
            {
                stLocalUeSecuCap.ucUeSeuCapLen = 4;
            }
            else
            {
                stLocalUeSecuCap.ucUeSeuCapLen = pstLocalUeNetCap->ucUeNetCapLen;
            }
        }
#endif
        /* �Ƚ�SMCЯ��Я����UE_SECURITY_CAP��UE���ص�UE_SECURITY_CAP��ֻ�Ƚ�����,���ٱȽϳ���*/    /*lint -e420 -e831*/
        /*lint -e420 -e831*/
        if(NAS_EMM_SECU_MEM_CMP_EQUAL != NAS_LMM_MEM_CMP(  (VOS_VOID*)&(stLocalUeSecuCap.ucEEA),
                                                            (VOS_VOID*)&(pstUeSecuCap->ucEEA),
                                                            NAS_EMM_SMC_MAX_UE_SECU_CAP_LEN ))
        /*lint +e420 +e831*/
        {
            NAS_EMM_SECU_LOG_ERR("NAS_EMM_SmcVerifyUeSecuCap: UE_SECU_CAP_NOT_MATCH.");
            return NAS_EMM_SECU_FAIL;
        }
    }
    /* Lģ��ģģʽ:ֻ��֤EEA,EIA�Ƿ���UE����ƥ�䣬������UEA,UIA*/
    else
    {
        stLocalUeSecuCap.ucUeSeuCapLen = pstLocalUeNetCap->ucUeNetCapLen;
        stLocalUeSecuCap.ucEEA = pstLocalUeNetCap->aucUeNetCap[NAS_EMM_UE_NET_CAP_EEA_POS];
        stLocalUeSecuCap.ucEIA = pstLocalUeNetCap->aucUeNetCap[NAS_EMM_UE_NET_CAP_EIA_POS];
        stLocalUeSecuCap.ucUEA = pstLocalUeNetCap->aucUeNetCap[NAS_EMM_UE_NET_CAP_UEA_POS];
        stLocalUeSecuCap.ucUIA = (pstLocalUeNetCap->aucUeNetCap[NAS_EMM_UE_NET_CAP_UIA_POS])&NAS_EMM_7_BITS_F;

        /* �Ƚ�SMCЯ����EEA,EIA��UE���ص�EEA,EIA*/
        if((pstUeSecuCap->ucEEA != stLocalUeSecuCap.ucEEA)
         ||(pstUeSecuCap->ucEIA != stLocalUeSecuCap.ucEIA))
        {
            NAS_EMM_SECU_LOG_ERR("NAS_EMM_SmcVerifyUeSecuCap: EEA or EIA NOT MATCH.");
            return NAS_EMM_SECU_FAIL;
        }

    }

    return NAS_EMM_SECU_SUCC;
}
VOS_UINT32  NAS_EMM_SmcVerifyNonceUE(
                                        const NAS_EMM_SECU_SMC_STRU *pstSmcStru )
{
    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SmcVerifyNonceUE entered.");

    /* ȷ��regplayed NonceUE�� NonceMME��Ԫ�������*/
    if((NAS_EMM_BIT_NO_SLCT == pstSmcStru->bitNonceUe)
     ||(NAS_EMM_BIT_NO_SLCT == pstSmcStru->bitNonceMme))
    {
        return NAS_EMM_SECU_FAIL;
    }

    /* ��UE���ص� NonceUE�Ƚϣ����ؽ��*/
    if(NAS_EMM_SECU_MEM_CMP_EQUAL != NAS_LMM_MEM_CMP(pstSmcStru->aucNonceUe,
                                                    NAS_EMM_GetSecuNonceUEAddr(),
                                                    NAS_EMM_SECU_NONCE_UE_LEN))
    {
        return NAS_EMM_SECU_FAIL;
    }

    return NAS_EMM_SECU_SUCC;

}


VOS_UINT32  NAS_EMM_DecodeSmcMsg(
                                        LRRC_LNAS_MSG_STRU      *pstCnSmcMsg,
                                        NAS_EMM_SECU_SMC_STRU   *pstSecurModeCtrl)/*lint -e662 -e661 -e831 -e416 -e415*/

{
    VOS_UINT32                          ulIndex             = NAS_EMM_NULL;
    VOS_UINT32                          ulMsgSize           = NAS_EMM_NULL;
    VOS_UINT8                          *pucSmcMsg           = VOS_NULL_PTR;
    VOS_UINT32                          ulTempMsgSize       = 0;
    VOS_UINT8                           ucReplayeUeSecuCapLen = 0;

    NAS_EMM_SECU_LOG_INFO( "NAS_EMM_DecodeCnSecurModeCtrlMsgNasMsg is entered.");

    /* �жϿտ���ϢSMC���������Ƿ���ȷ*/
    /* ֻ��������̳��ȣ�
       ������Ϣ��󳤶ȵ�������ڱ�֤��Ԫ������ȷ������£����ڶ���Ĵ����ֽڲ��迼��*/
    ulMsgSize  = pstCnSmcMsg->ulNasMsgSize;
    if(NAS_EMM_SMC_MIN_LEN > ulMsgSize)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_DecodeSmcMsg: Msg Size Err.");
        return NAS_EMM_SECU_FAIL;
    }

    /*��ȡSMC��Ϣ����*/
    pucSmcMsg = pstCnSmcMsg->aucNasMsg;

    /* ��ȡ��ѡIE*/
    ulIndex = NAS_EMM_SECU_PLAIN_MSG_POS;

    /*Protolcal Disciriminator and security header type*/
    pstSecurModeCtrl->ucPdSt = pucSmcMsg[ulIndex];
    ulIndex++;

    /*Message Type*/
    pstSecurModeCtrl->ucMt = pucSmcMsg[ulIndex];
    ulIndex++;

    /*selected NAS security algorithm*/
    pstSecurModeCtrl->ucSelAlgrthm = pucSmcMsg[ulIndex];
    ulIndex++;

    /*Key Set Identifier*/
    pstSecurModeCtrl->ucKSI = pucSmcMsg[ulIndex]& NAS_EMM_LOW_HALF_BYTE_F;
    ulIndex++;

    /*replayed UE security capability*/
    ucReplayeUeSecuCapLen = pucSmcMsg[ulIndex];
    ulIndex++;

    /*replayed UE secutity capability len:[2,5]*/
    if((NAS_EMM_SMC_MIN_UE_SECU_CAP_LEN > ucReplayeUeSecuCapLen)
     ||(NAS_EMM_SMC_MAX_UE_SECU_CAP_LEN < ucReplayeUeSecuCapLen))
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_DecodeSmcMsg: UE security cap len Err.");
        return NAS_EMM_SECU_FAIL;
    }

    /*���ʣ�������ĳ����Ƿ�����Ҫ��*/
    if((ulIndex + ucReplayeUeSecuCapLen) > ulMsgSize)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_DecodeSmcMsg: Msg Size Err.");
        return NAS_EMM_SECU_FAIL;
    }

    pstSecurModeCtrl->stUeSecuCap.ucUeSeuCapLen = ucReplayeUeSecuCapLen;
    NAS_LMM_MEM_CPY(                &(pstSecurModeCtrl->stUeSecuCap.ucEEA),
                                    &(pucSmcMsg[ulIndex]),
                                     ucReplayeUeSecuCapLen);
    ulIndex += ucReplayeUeSecuCapLen;

    /* ���spareλΪ0 */
    pstSecurModeCtrl->stUeSecuCap.ucUIA &= NAS_EMM_7_BITS_F;
    pstSecurModeCtrl->stUeSecuCap.ucGEA &= NAS_EMM_7_BITS_F;
    /*��ѡ��Ԫ������ɺ�ʣ�������ĳ���*/
    ulTempMsgSize =  ulMsgSize -  ulIndex;
    if(0 == ulTempMsgSize)
    {
        NAS_EMM_SECU_LOG_INFO("NAS_EMM_DecodeSmcMsg: decode complete.");
        return NAS_EMM_SECU_SUCC;
    }

    /*��ȡ��ѡ��Ԫ*/
    /*IMEISV request*/
    if(NAS_EMM_SMC_IMEISV_REQ_IEI == (pucSmcMsg[ulIndex]&NAS_EMM_HIGH_HALF_BYTE_F))
    {
        NAS_EMM_SECU_LOG_INFO("NAS_EMM_DecodeSmcMsg: IMEISV request IE.");
        pstSecurModeCtrl->bitImeisvReq = NAS_EMM_BIT_SLCT;
        pstSecurModeCtrl->ucImeisvRsq = pucSmcMsg[ulIndex];
        ulIndex++;
        ulTempMsgSize = ulMsgSize -  ulIndex;
    }

    /*�ж����ʣ������Ϊ0����������*/
    if(0 == ulTempMsgSize)
    {
        NAS_EMM_SECU_LOG_INFO("NAS_EMM_DecodeSmcMsg: decode complete.");
        return NAS_EMM_SECU_SUCC;
    }

    /*replayed NonceUE*/
    if(NAS_EMM_SMC_NONCE_UE_IEI == pucSmcMsg[ulIndex])
    {
        ulIndex++;
        if((ulIndex + NAS_EMM_SECU_NONCE_UE_LEN) <= ulMsgSize)
        {
            NAS_EMM_SECU_LOG_INFO("NAS_EMM_DecodeSmcMsg: replayed Nonce IE.");
            pstSecurModeCtrl->bitNonceUe = NAS_EMM_BIT_SLCT;
            NAS_LMM_MEM_CPY(             pstSecurModeCtrl->aucNonceUe,
                                        (pucSmcMsg + ulIndex),
                                        NAS_EMM_SECU_NONCE_UE_LEN);
            ulIndex += NAS_EMM_SECU_NONCE_UE_LEN;
            ulTempMsgSize = ulMsgSize -  ulIndex;

             /*�ж����ʣ������Ϊ0����������*/
            if(0 == ulTempMsgSize)
            {
                NAS_EMM_SECU_LOG_INFO("NAS_EMM_DecodeSmcMsg: decode complete.");
                return NAS_EMM_SECU_SUCC;
            }
        }
        else
        {
            NAS_EMM_SECU_LOG_ERR("NAS_EMM_DecodeSmcMsg: MSG SIZE ERR.");
            return NAS_EMM_SECU_FAIL;
        }
    }

    /*NonceMME*/
    if(NAS_EMM_SMC_NONCE_MME_IEI == pucSmcMsg[ulIndex])
    {
        ulIndex++;
        if((ulIndex + NAS_EMM_SECU_NONCE_MME_LEN) <= ulMsgSize)
        {
            NAS_EMM_SECU_LOG_INFO("NAS_EMM_DecodeSmcMsg: NonceMME IE.");
            pstSecurModeCtrl->bitNonceMme = NAS_EMM_BIT_SLCT;
            NAS_LMM_MEM_CPY(             pstSecurModeCtrl->aucNonceMme,
                                       (pucSmcMsg + ulIndex),
                                        NAS_EMM_SECU_NONCE_MME_LEN);
        }
        else
        {
            NAS_EMM_SECU_LOG_ERR("NAS_EMM_DecodeSmcMsg: MSG SIZE ERR.");
            return NAS_EMM_SECU_FAIL;
        }
    }

    return NAS_EMM_SECU_SUCC;

}/*lint +e662 +e661 +e831 +e416 +e415*/


VOS_UINT32  NAS_EMM_NotEia0SmcVerifyParam
(
    const NAS_EMM_SECU_SMC_STRU              *pstSmcStru,
    VOS_UINT8                           ucSqn
)
{
    VOS_UINT32                          ulRslt      = NAS_EMM_SECU_FAIL;
    VOS_UINT8                           ucSmcEia    = NAS_EMM_NULL;
    VOS_UINT8                           ucSmcEca    = NAS_EMM_NULL;

    /*lint -e415 -e416*/
    ulRslt = NAS_EMM_NotEia0SmcVerifyKsi(ucSqn, pstSmcStru->ucKSI);
    /*lint +e415 +e416*/

    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SecuSendSmcRej(NAS_EMM_SMC_REJ_UNSPECIFIED);
        NAS_EMM_SECU_LOG_NORM("NAS_EMM_NotEia0SmcVerifyParam:Ksi ERR.");
        return NAS_EMM_SECU_FAIL;
    }

    /*����㷨*/
    ucSmcEia =  pstSmcStru->ucSelAlgrthm & NAS_EMM_LOW_3_BITS_F;
    ucSmcEca =  (pstSmcStru->ucSelAlgrthm >>4) & NAS_EMM_LOW_3_BITS_F;
    ulRslt = NAS_EMM_IsAlgUeSupported(ucSmcEca, ucSmcEia);
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SecuSendSmcRej(NAS_EMM_SMC_REJ_UNSPECIFIED);
        NAS_EMM_SECU_LOG_NORM("NAS_EMM_NotEia0SmcVerifyParam:ALG ERR.");
        return NAS_EMM_SECU_FAIL;
    }

    /*���SMCЯ����replayed UE_SECURITY_CAP�Ƿ���UE����ά������ͬ*/
    ulRslt = NAS_EMM_SmcVerifyUeSecuCap(&(pstSmcStru->stUeSecuCap));
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SecuSendSmcRej(NAS_EMM_SMC_REJ_SECU_CAP_MISMATCH);
        NAS_EMM_SECU_LOG_NORM("NAS_EMM_NotEia0SmcVerifyParam:UeNetCap ERR.");
        return NAS_EMM_SECU_FAIL;
    }

    /*���SMCЯ����KSIΪnew mapped ksi������Ҫ��֤replaye NonceUE*/
    if(NAS_EMM_SMC_UPDATE_NEW_MAPPED == NAS_EMM_GetSmcType())
    {
        ulRslt = NAS_EMM_SmcVerifyNonceUE(pstSmcStru);
        if(NAS_EMM_SECU_FAIL == ulRslt)
        {
            NAS_EMM_SecuSendSmcRej(NAS_EMM_SMC_REJ_UNSPECIFIED);
            NAS_EMM_SECU_LOG_NORM("NAS_EMM_NotEia0SmcVerifyParam:NonceUE ERR.");
            return NAS_EMM_SECU_FAIL;
        }
    }

    return NAS_EMM_SECU_SUCC;
}
VOS_UINT32  NAS_EMM_Eia0SmcVerifyParam
(
    const NAS_EMM_SECU_SMC_STRU              *pstSmcStru
)
{
    VOS_UINT32                          ulRslt      = NAS_EMM_SECU_FAIL;
    VOS_UINT8                           ucSmcEia    = NAS_EMM_NULL;
    VOS_UINT8                           ucSmcEca    = NAS_EMM_NULL;

    /*lint -e415 -e416*/
    ulRslt = NAS_EMM_Eia0SmcVerifyKsi(pstSmcStru->ucKSI);
    /*lint +e415 +e416*/

    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SecuSendSmcRej(NAS_EMM_SMC_REJ_UNSPECIFIED);
        NAS_EMM_SECU_LOG_NORM("NAS_EMM_Eia0SmcVerifyParam:Ksi ERR.");
        return NAS_EMM_SECU_FAIL;
    }

    /*����㷨*/
    ucSmcEia =  pstSmcStru->ucSelAlgrthm & NAS_EMM_LOW_3_BITS_F;
    ucSmcEca =  (pstSmcStru->ucSelAlgrthm >>4) & NAS_EMM_LOW_3_BITS_F;
    ulRslt = NAS_EMM_Eia0SmcVerifyAlg(ucSmcEca, ucSmcEia);
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SecuSendSmcRej(NAS_EMM_SMC_REJ_UNSPECIFIED);
        NAS_EMM_SECU_LOG_NORM("NAS_EMM_Eia0SmcVerifyParam:ALG ERR.");
        return NAS_EMM_SECU_FAIL;
    }

    /*���SMCЯ����replayed UE_SECURITY_CAP�Ƿ���UE����ά������ͬ*/
    ulRslt = NAS_EMM_SmcVerifyUeSecuCap(&(pstSmcStru->stUeSecuCap));
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SecuSendSmcRej(NAS_EMM_SMC_REJ_SECU_CAP_MISMATCH);
        NAS_EMM_SECU_LOG_NORM("NAS_EMM_NotEia0SmcVerifyParam:UeNetCap ERR.");
        return NAS_EMM_SECU_FAIL;
    }

    /* Ϊ�˼��ݲ�ͬ���磬ʵ�п���ϳ�ԭ���ݲ��ж��Ƿ��н������ػ���
       ���ڽ����������� */

    return NAS_EMM_SECU_SUCC;
}


VOS_UINT32  NAS_EMM_SmcVerifyParam( LRRC_LNAS_MSG_STRU  *pstSmcMsg )
{
    VOS_UINT8                           ucSmcEia = 0;
    VOS_UINT32                          ulRslt = NAS_EMM_SECU_FAIL;
    NAS_EMM_SECU_SMC_STRU              *pstSmcStru = VOS_NULL_PTR;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SmcVerifyParam enterd.");

    /*SMC��Ϣ����*/
    pstSmcStru = (NAS_EMM_SECU_SMC_STRU *)NAS_EMM_GetSecuBufSpace();
    NAS_LMM_MEM_SET(pstSmcStru, 0, sizeof(NAS_EMM_SECU_SMC_STRU));
    ulRslt = NAS_EMM_DecodeSmcMsg(pstSmcMsg, pstSmcStru);
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SecuSendSmcRej(NAS_EMM_SMC_REJ_UNSPECIFIED);
        NAS_EMM_SECU_LOG_NORM("NAS_EMM_SmcVerifyParam:decode ERR.");
        return NAS_EMM_SECU_FAIL;
    }

    /*���KSI*/
    NAS_EMM_SECU_LOG1_INFO("NAS_EMM_SmcVerifyParam:pstSmcStru->ucKSI = ", pstSmcStru->ucKSI );

    ucSmcEia =  pstSmcStru->ucSelAlgrthm & NAS_EMM_LOW_3_BITS_F;
    if (NAS_EMM_SECU_ALG_EIA0 == ucSmcEia)
    {
        return NAS_EMM_Eia0SmcVerifyParam(      pstSmcStru);
    }
    else
    {
        /*lint -e416 -e415*/
        return NAS_EMM_NotEia0SmcVerifyParam(   pstSmcStru,
                                                pstSmcMsg->aucNasMsg[NAS_EMM_SECU_MSG_SN_POS]);
        /*lint +e416 +e415*/
    }
}


VOS_VOID  NAS_EMM_SecuGetNonceMmeInSmc( VOS_VOID )
{
    NAS_EMM_SECU_SMC_STRU              *pstSmcStru = VOS_NULL_PTR;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuGetNonceMmeInSmc enterd.");

    /* �ӽ�������Ϣ�ṹ�л�ȡ NONCEmme*/
    pstSmcStru = (NAS_EMM_SECU_SMC_STRU *)NAS_EMM_GetSecuBufSpace();
    NAS_LMM_MEM_CPY(                     NAS_EMM_GetSecuNonceMMEAddr(),
                                        pstSmcStru->aucNonceMme,
                                        NAS_EMM_SECU_NONCE_MME_LEN);
    return;
}

/*****************************************************************************
 Function Name   : NAS_EMM_Eia0SmcIntCheckPara
 Description     : ����EIA0ʱ����NAS CK��NAS IK
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2012-10-18  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_EMM_Eia0SmcIntCheckPara
(
    LRRC_LNAS_MSG_STRU                 *pstSmcMsg,
    NAS_EMM_PROT_PARAM_STRU            *pstUnProtParam
)
{
    VOS_UINT8                          *pucKasme        = VOS_NULL_PTR;
    VOS_UINT8                           ucSmcEia        = NAS_EMM_NULL;
    VOS_UINT8                           ucSmcEca        = NAS_EMM_NULL;
    VOS_UINT32                          ulDlForUnprot   = NAS_EMM_NULL;
    VOS_UINT8                           ucKsiValue      = NAS_EMM_NULL;
    VOS_UINT32                          ulRslt          = NAS_EMM_SECU_FAIL;


    /* ����SMCЯ����KSI�����ͣ���ȡ��ȫ������*/
    /*lint -e416 -e415*/
    ucKsiValue =  pstSmcMsg->aucNasMsg[NAS_EMM_SECU_SMC_KSI_POSE] & NAS_EMM_LOW_3_BITS_F;
    /*lint +e416 +e415*/
    if (ucKsiValue == NAS_EMM_GetSecuCurKSIasme())
    {
        pucKasme = NAS_EMM_GetSecuCurKasmeAddr();
    }
    else
    {
        pucKasme = NAS_EMM_GetEia0Kasme();
    }

    /*��ȡSMCЯ�����㷨*/
    /*lint -e416 -e415*/
    ucSmcEia =  pstSmcMsg->aucNasMsg[NAS_EMM_SECU_SMC_ALG_POSE] & NAS_EMM_LOW_3_BITS_F;
    ucSmcEca = (pstSmcMsg->aucNasMsg[NAS_EMM_SECU_SMC_ALG_POSE]>>4) & NAS_EMM_LOW_3_BITS_F;
    /*lint +e416 +e415*/

    /*�����µ� NasCK,NasIK*/
    NAS_EMM_DeriveNewNasKey(pucKasme,ucSmcEca,ucSmcEia);

    (VOS_VOID)ulRslt;

    /*SMC��Ϣ�����Լ����Ҫʹ�������ɵ� NasCK,NasIK���滻Register�е���Կ*/
    /*NasIK*/
    (VOS_VOID)NAS_EMM_SecuSetKeyByKeyIndex(  NAS_EMM_GetSecuTempNasIkAddr(),
                                        NAS_EMM_SECU_INTEGRITY_KEY);
    /*NasCK*/
    (VOS_VOID)NAS_EMM_SecuSetKeyByKeyIndex(  NAS_EMM_GetSecuTempNasCkAddr(),
                                        NAS_EMM_SECU_CIPHER_KEY);

    /*��װSMC��Ϣ�����Լ����Ҫ�Ĳ���*/
    pstUnProtParam->ulNasCount          = ulDlForUnprot;
    pstUnProtParam->ucSecuALg           = ucSmcEia;
    pstUnProtParam->pucMsg              = pstSmcMsg->aucNasMsg;
    pstUnProtParam->pulMsgSize          = &(pstSmcMsg->ulNasMsgSize);

    return;
}
VOS_VOID  NAS_EMM_NotEia0SmcIntCheckPara(
                                        LRRC_LNAS_MSG_STRU      *pstSmcMsg,
                                        NAS_EMM_PROT_PARAM_STRU *pstUnProtParam)
{
    VOS_UINT8                          *pucKasme = VOS_NULL_PTR;
    VOS_UINT8                           ucSmcEia = 0;
    VOS_UINT8                           ucSmcEca = 0;
    VOS_UINT32                          ulDlForUnprot = 0;
    VOS_UINT32                          ulRslt = NAS_EMM_SECU_FAIL;

    /* ����SMCЯ����KSI�����ͣ���ȡ��ȫ������*/
    switch (NAS_EMM_GetSmcType())
    {
        case NAS_EMM_SMC_UPDATE_CUR:
            /*lint -e415 -e416*/
            ulDlForUnprot = NAS_EMM_CompDlNasCount(NAS_EMM_GetSecuDlNasCount(),
                                pstSmcMsg->aucNasMsg[NAS_EMM_SECU_MSG_SN_POS]);
            /*lint +e415 +e416*/
            pucKasme = NAS_EMM_GetSecuCurKasmeAddr();
            break;

        case NAS_EMM_SMC_UPDATE_NEW_MAPPED:
            ulDlForUnprot = 0;
            pucKasme = NAS_EMM_GetSecuNewMappedKasmeAddr();

            /*��ȡSMC��ϢЯ���� NONCEmme*/
            NAS_EMM_SecuGetNonceMmeInSmc();

            /*���� new mapped security context�е� Kasme*/
            ulRslt = NAS_EMM_DeriveKasmeFromUmtsIdle(pucKasme);
            if(NAS_EMM_SECU_FAIL == ulRslt)
            {
                NAS_EMM_SECU_LOG_ERR("NAS_EMM_DeriveKasmeFromUmtsIdle ERR.");
            }
            break;

        case NAS_EMM_SMC_UPDATE_NEW_NATIVE:
            ulDlForUnprot = 0;
            pucKasme = NAS_EMM_GetSecuNewNativeKasmeAddr();
            break;

        case NAS_EMM_SMC_UPDATE_NON_CUR_NATIVE:
            /*lint -e416 -e415*/
            ulDlForUnprot = NAS_EMM_CompDlNasCount(
                                NAS_EMM_GetSecuNonCurDlNasCount(),
                                pstSmcMsg->aucNasMsg[NAS_EMM_SECU_MSG_SN_POS]);
            /*lint +e416 +e415*/
            pucKasme = NAS_EMM_GetSecuNonCurNativeKasmeAddr();
            break;

        default:
            NAS_EMM_SECU_LOG_ERR("NAS_EMM_NotEia0SmcIntCheckPara:SmcType ERR.");
            return;
    }

    /*��ȡSMCЯ�����㷨*/
    /*lint -e416 -e415*/
    ucSmcEia =  pstSmcMsg->aucNasMsg[NAS_EMM_SECU_SMC_ALG_POSE] & NAS_EMM_LOW_3_BITS_F;
    ucSmcEca = (pstSmcMsg->aucNasMsg[NAS_EMM_SECU_SMC_ALG_POSE]>>4) & NAS_EMM_LOW_3_BITS_F;
    /*lint +e416 +e415*/

    /*�����µ� NasCK,NasIK*/
    NAS_EMM_DeriveNewNasKey(pucKasme,ucSmcEca,ucSmcEia);

    (VOS_VOID)ulRslt;

    /*SMC��Ϣ�����Լ����Ҫʹ�������ɵ� NasCK,NasIK���滻Register�е���Կ*/
    /*NasIK*/
    (VOS_VOID)NAS_EMM_SecuSetKeyByKeyIndex(  NAS_EMM_GetSecuTempNasIkAddr(),
                                        NAS_EMM_SECU_INTEGRITY_KEY);
    /*NasCK*/
    (VOS_VOID)NAS_EMM_SecuSetKeyByKeyIndex(  NAS_EMM_GetSecuTempNasCkAddr(),
                                        NAS_EMM_SECU_CIPHER_KEY);

    /*��װSMC��Ϣ�����Լ����Ҫ�Ĳ���*/
    pstUnProtParam->ulNasCount          = ulDlForUnprot;
    pstUnProtParam->ucSecuALg           = ucSmcEia;
    pstUnProtParam->pucMsg              = pstSmcMsg->aucNasMsg;
    pstUnProtParam->pulMsgSize          = &(pstSmcMsg->ulNasMsgSize);

    return;
}
VOS_UINT32  NAS_EMM_SmcIntCheck( LRRC_LNAS_MSG_STRU  *pstSmcMsg )
{
    VOS_UINT32                          ulRslt          = NAS_EMM_SECU_FAIL;
    NAS_EMM_PROT_PARAM_STRU             stUnProtParam   = {0};
    VOS_UINT8                           ucSmcEia        = NAS_EMM_NULL;

    /*����SMCЯ���Ĳ����������µ� NasCK,NasIK,����SMC�����Լ��Ĳ���*/
    /*lint -e416 -e415*/
    ucSmcEia =  pstSmcMsg->aucNasMsg[NAS_EMM_SECU_SMC_ALG_POSE] & NAS_EMM_LOW_3_BITS_F;
    /*lint +e416 +e415*/
    if (NAS_EMM_SECU_ALG_EIA0 == ucSmcEia)
    {
        NAS_EMM_Eia0SmcIntCheckPara(pstSmcMsg, &stUnProtParam);
    }
    else
    {

        NAS_EMM_NotEia0SmcIntCheckPara(pstSmcMsg, &stUnProtParam);
    }

    /*�����Լ��*/
    ulRslt = NAS_EMM_SecuIntCheck(&stUnProtParam);
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SECU_LOG_NORM("NAS_EMM_SmcIntCheck: SMC Int Check Fail.");

        /*���ΰ�ȫ����ʧ�ܣ��µ�NasCK��NasIKû����Ч����ԭRegister�е���Կ*/
        /*NasIK*/
        (VOS_VOID) NAS_EMM_SecuSetKeyByKeyIndex(  NAS_EMM_GetSecuCurNasIkAddr(),
                                            NAS_EMM_SECU_INTEGRITY_KEY);
        /*NasCK*/
        (VOS_VOID) NAS_EMM_SecuSetKeyByKeyIndex(  NAS_EMM_GetSecuCurNasCkAddr(),
                                            NAS_EMM_SECU_CIPHER_KEY);

        /* �����Լ��ʧ��,ʹ�û�ԭ�����Կ��smc rej�տڽ��м��� */
        NAS_EMM_SecuSendSmcRej(NAS_EMM_SMC_REJ_UNSPECIFIED);
        return NAS_EMM_SECU_FAIL;
    }

    /*�����Լ��ͨ��������ܱ���DL_NASCOUNT*/
    NAS_EMM_GetSecuDlNasCount() = stUnProtParam.ulNasCount;

    return NAS_EMM_SECU_SUCC;
}
VOS_VOID  NAS_EMM_SendSecuParamToRrc( VOS_VOID )
{
    LRRC_LNAS_INFO_STRU                   stNasInfo;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SendSecuParamToRrc entered.");

    /*����ڴ�*/
    NAS_LMM_MEM_SET(&stNasInfo, 0, sizeof(LRRC_LNAS_INFO_STRU));

    /*��䰲ȫ����: Kasme + UL_NasCount*/
    stNasInfo.bitOpSecurityKey = NAS_EMM_BIT_SLCT;
    NAS_LMM_MEM_CPY(                     stNasInfo.stSecurityKey.aucKasme,
                                        NAS_EMM_GetSecuCurKasmeAddr(),
                                        NAS_LMM_SECU_AUTH_KEY_ASME_LEN);
    stNasInfo.stSecurityKey.ulNasCount = NAS_EMM_GetSecuUlNasCount();

    /*����RRC_MM_INFO_CHANGE_REQ��Ϣ��RRC*/
    NAS_EMM_SendInfoChangeReqMsg(&stNasInfo);

    return;

}
VOS_VOID    NAS_EMM_SecuSendSmcCmp(
                        NAS_EMM_IMEISV_REQUEST_INFO_ENUM_UINT8  enIfImeisvReq)
{
    VOS_UINT32                           ulPos                   = 0;
    NAS_EMM_MRRC_DATA_REQ_STRU          *pIntraMsg              = VOS_NULL_PTR;
    NAS_EMM_PUB_INFO_STRU               *pstEmmInfo             = VOS_NULL_PTR;
    VOS_UINT32                          uldataReqMsgLenNoHeader;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuSendSmcCmp is entered.");

    /*������Ϣ�ڴ�*/
    pIntraMsg = (VOS_VOID *)NAS_LMM_MEM_ALLOC(NAS_EMM_INTRA_MSG_MAX_SIZE);
    if (NAS_EMM_NULL_PTR == pIntraMsg)
    {
        NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuSendSmcCmp: MSG ALLOC ERR!");
        return;
    }

    /*����SMC COMPLETE��Ϣ*/
    pIntraMsg->stNasMsg.aucNasMsg[ulPos++]     = EMM_CN_MSG_PD_EMM;
    pIntraMsg->stNasMsg.aucNasMsg[ulPos++]     = NAS_EMM_CN_MT_SMC_CMP;

    if(NAS_EMM_IMEISV_REQUEST == enIfImeisvReq)
    {
        pIntraMsg->stNasMsg.aucNasMsg[ulPos++] = NAS_EMM_SECU_IMEISV_IEI;

        /* ��� IMEISV*/
        pstEmmInfo = NAS_LMM_GetEmmInfoAddr();
        /*lint -e419*/
        NAS_LMM_MEM_CPY((VOS_VOID *)&pIntraMsg->stNasMsg.aucNasMsg[ulPos],
                                        pstEmmInfo->stMmUeId.aucImeisv,
                                        NAS_MM_MAX_UEID_BUF_SIZE);
        /*lint +e419*/

        ulPos += NAS_MM_MAX_UEID_BUF_SIZE;
    }

    /*�����ϢID*/
    pIntraMsg->ulMsgId = ID_NAS_LMM_INTRA_MRRC_DATA_REQ;
    pIntraMsg->enEstCaue = LRRC_LNAS_EST_CAUSE_MO_SIGNALLING;
    pIntraMsg->enCallType = LRRC_LNAS_CALL_TYPE_ORIGINATING_SIGNALLING;
    pIntraMsg->enDataCnf = LRRC_LMM_DATA_CNF_NEED;

    /*����ڲ���Ϣ����*/
    pIntraMsg->enEmmMsgType = NAS_EMM_MSG_SMC_COMPL;

    /*�����Ϣ����*/
    pIntraMsg->stNasMsg.ulNasMsgSize = ulPos;

    /*���㲻������Ϣͷ����Ϣ����*/
    uldataReqMsgLenNoHeader =
        NAS_EMM_COUNT_INTRA_MSG_NO_HEADER_LEN(pIntraMsg->stNasMsg.ulNasMsgSize);

    /*�����Ϣͷ*/
    NAS_EMM_SET_SECU_INTRA_MSG_HEADER(pIntraMsg, uldataReqMsgLenNoHeader);

    /*�տ���Ϣ�ϱ� SECU_MODE_COMPLETE*/
    NAS_LMM_SendOmtAirMsg( NAS_EMM_OMT_AIR_MSG_UP,
                          NAS_EMM_SECU_CMP,
                          (NAS_MSG_STRU*)&(pIntraMsg->stNasMsg));

    /*�ؼ��¼��ϱ� SECU_MODE_COMPLETE*/
    NAS_LMM_SendOmtKeyEvent(EMM_OMT_KE_SECU_CMP);

    /* ��MRRC����SMC COMPLETE��Ϣ */
    NAS_EMM_SndUplinkNasMsg(pIntraMsg);

    NAS_LMM_MEM_FREE(pIntraMsg);

    return;
}
VOS_VOID NAS_EMM_SecuSendSmcRej(NAS_EMM_SMC_REJ_CAUSE_ENUM_UINT8 enCause)
{
    VOS_UINT32                          ulPos               = 0;
    NAS_EMM_MRRC_DATA_REQ_STRU         *pIntraMsg           = VOS_NULL;
    VOS_UINT32                          uldataReqMsgLenNoHeader;


    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuSendSmcRej is entered.");

    /* ������Ϣ�ڴ� */
    pIntraMsg = (VOS_VOID *)NAS_LMM_MEM_ALLOC(
                                        NAS_EMM_INTRA_MSG_MAX_SIZE);

    /* �ж�����������ʧ�ܴ�ӡ�����˳� */
    if (NAS_EMM_NULL_PTR == pIntraMsg)
    {
      NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuSendSmcRej: MSG ALLOC ERR!");
      return;
    }

    /*����SMC Reject ��Ϣ*/
    pIntraMsg->stNasMsg.aucNasMsg[ulPos++] = EMM_CN_MSG_PD_EMM;
    pIntraMsg->stNasMsg.aucNasMsg[ulPos++] = NAS_EMM_CN_MT_SMC_REJ;

    /* ��� EMM CAUSE*/
    pIntraMsg->stNasMsg.aucNasMsg[ulPos++] = enCause;

    /* �����ϢID */
    pIntraMsg->ulMsgId                  = ID_NAS_LMM_INTRA_MRRC_DATA_REQ;
    pIntraMsg->enEstCaue                = LRRC_LNAS_EST_CAUSE_MO_SIGNALLING;
    pIntraMsg->enCallType               = LRRC_LNAS_CALL_TYPE_ORIGINATING_SIGNALLING;
    pIntraMsg->enDataCnf                = LRRC_LMM_DATA_CNF_NEED;
    pIntraMsg->enEmmMsgType             = NAS_EMM_MAS_SMC_REJ;

    /* �����Ϣ���� */
    pIntraMsg->stNasMsg.ulNasMsgSize = ulPos;

    /*���㲻������Ϣͷ����Ϣ����*/
    uldataReqMsgLenNoHeader =
            NAS_EMM_COUNT_INTRA_MSG_NO_HEADER_LEN(pIntraMsg->stNasMsg.ulNasMsgSize);

    /*�����Ϣͷ*/
    NAS_EMM_SET_SECU_INTRA_MSG_HEADER(pIntraMsg, uldataReqMsgLenNoHeader);

    /*�տ���Ϣ�ϱ� SECU_MODE_REJ*/
    NAS_LMM_SendOmtAirMsg( NAS_EMM_OMT_AIR_MSG_UP,
                          NAS_EMM_SECU_REJ,
                          (NAS_MSG_STRU*)&(pIntraMsg->stNasMsg));

    /*�ؼ��¼��ϱ� SECU_MODE_REJ*/
    NAS_LMM_SendOmtKeyEvent(         EMM_OMT_KE_SECU_REJ);

    /*��MRRC����SMC Reject��Ϣ*/
    NAS_EMM_SndUplinkNasMsg(pIntraMsg);

    NAS_LMM_MEM_FREE(pIntraMsg);

    return;
}
NAS_EMM_IMEISV_REQUEST_INFO_ENUM_UINT8  NAS_EMM_IsImeisvReq( VOS_VOID)
{
    NAS_EMM_SECU_SMC_STRU              *pstSmcStru = VOS_NULL_PTR;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_IsImeisvReq enterd.");
    pstSmcStru = (NAS_EMM_SECU_SMC_STRU *)NAS_EMM_GetSecuBufSpace();

    if((NAS_EMM_BIT_SLCT == pstSmcStru->bitImeisvReq)
     &&(NAS_EMM_SMC_IMEISV_REQ == pstSmcStru->ucImeisvRsq))
    {
        return NAS_EMM_IMEISV_REQUEST;
    }
    else
    {
        return NAS_EMM_IMEISV_NOT_REQUEST;
    }

}


VOS_VOID  NAS_EMM_SmcSucc( const LRRC_LNAS_MSG_STRU    *pstSmcMsg )
{
    NAS_EMM_IMEISV_REQUEST_INFO_ENUM_UINT8  enIfImeisvReq   = NAS_EMM_IMEISV_REQUEST_INFO_BUTT;
    VOS_UINT8                               ucSmcEia        = 0;
    VOS_UINT8                               ucKsiValue      = 0;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SmcSucc enterd.");

    if((NAS_EMM_SMC_UPDATE_NEW_MAPPED == NAS_EMM_GetSmcType())
     ||(NAS_EMM_SMC_UPDATE_NEW_NATIVE == NAS_EMM_GetSmcType()))
    {
        /*DL_NASCOUNT��UP_NASCOUNT ����*/
        NAS_EMM_GetSecuDlNasCount() = 0;
        NAS_EMM_GetSecuUlNasCount() = 0;

        /*����������*/
        NAS_EMM_UpdateCurSecuCntxt();
    }
    else if(NAS_EMM_SMC_UPDATE_NON_CUR_NATIVE == NAS_EMM_GetSmcType())
    {
        /*Dl_NasCount��SMC�����Լ��ɹ�ʱ���Ѿ���ԭ���˴����ش���*/
        /*Ul_NasCount��ԭΪnon-current security context�е�Ul_NasCount*/
        NAS_EMM_SetSecuUlNasCount(NAS_EMM_GetSecuNonCurUlNasCount());

        /*����KSIasme��Kasme*/
        NAS_EMM_UpdateCurSecuCntxt();
    }
    else
    {
        /*ɾ�� new mapped security context*/
        NAS_EMM_ClearNewMappedSecuCntxt();
    }

    /* ���������EIA0������Ҫ��ֵKSI��KASME������������NAS COUNTΪ0,
       ɾ��������NV�еİ�ȫ������ */
    /*lint -e416 -e415*/
    ucSmcEia    = pstSmcMsg->aucNasMsg[NAS_EMM_SECU_SMC_ALG_POSE-NAS_EMM_SECU_PLAIN_MSG_POS] & NAS_EMM_LOW_3_BITS_F;
    ucKsiValue  = pstSmcMsg->aucNasMsg[NAS_EMM_SECU_SMC_KSI_POSE-NAS_EMM_SECU_PLAIN_MSG_POS] & NAS_EMM_LOW_3_BITS_F;
    /*lint +e416 +e415*/
    if (NAS_EMM_SECU_ALG_EIA0 == ucSmcEia)
    {
        if (ucKsiValue != NAS_EMM_GetSecuCurKSIasme())
        {
            NAS_LMM_MEM_CPY(    NAS_EMM_GetSecuCurKasmeAddr(),
                                NAS_EMM_GetEia0Kasme(),
                                NAS_LMM_SECU_AUTH_KEY_ASME_LEN);
        }

        NAS_EMM_SetSecuCurKSIsgsn(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE);
        NAS_EMM_SetSecuCurKSIasme(ucKsiValue);

        /*DL_NASCOUNT��UP_NASCOUNT ����*/
        NAS_EMM_GetSecuDlNasCount() = 0;
        NAS_EMM_GetSecuUlNasCount() = 0;

        /* ɾ��������NV�еİ�ȫ������ */
        NAS_LMM_WriteEpsSecuContext(NAS_NV_ITEM_DELETE);

        /* ֪ͨEMM EIA0�㷨���� */
        NAS_EMM_SendIntraEia0ActNotify();
    }

    /*����NasCK,NasIK*/
    NAS_LMM_MEM_CPY(                     NAS_EMM_GetSecuCurNasCkAddr(),
                                        NAS_EMM_GetSecuTempNasCkAddr(),
                                        NAS_LMM_SECU_NAS_KEY_LEN);
    NAS_LMM_MEM_CPY(                     NAS_EMM_GetSecuCurNasIkAddr(),
                                        NAS_EMM_GetSecuTempNasIkAddr(),
                                        NAS_LMM_SECU_NAS_KEY_LEN);

    /*�����㷨*/
    NAS_EMM_GetSecuAlg() = pstSmcMsg->aucNasMsg[NAS_EMM_PLAIN_SMC_ALG_POSE];

    /*����RRC_MM_INFO_CHNG_REQ��Ϣ*/
    NAS_EMM_SendSecuParamToRrc();

    /*����SECU_MODE_COMPLETE��Ϣ*/
    enIfImeisvReq = NAS_EMM_IsImeisvReq();
    NAS_EMM_SecuSendSmcCmp(enIfImeisvReq);

    return;
}

/*lint -e416*/
/*lint -e415*/
/*lint -e831*/

VOS_UINT32  NAS_EMM_SmcCommProcess(LRRC_LNAS_MSG_STRU  *pstSmcMsg)/*lint -e416 -e415*/
{
    VOS_UINT32                          ulRslt = NAS_EMM_SECU_FAIL;
    VOS_UINT8                           ucSmcEia = NAS_EMM_NULL;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SmcCommProcess enterd.");

    /*�����Ϣ����*/
    if(NAS_EMM_CN_MT_SMC_CMD != pstSmcMsg->aucNasMsg[NAS_EMM_SECU_SMC_MT_POSE])
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SmcCommProcess:SMC MT ERR.");
        return NAS_EMM_SECU_FAIL;
    }

    /*SMC��Ϣ�ϱ�*/
    NAS_LMM_SendOmtAirMsg(               NAS_EMM_OMT_AIR_MSG_DOWN,
                                        NAS_EMM_SECU_CMD,
                         (NAS_MSG_STRU*)pstSmcMsg);

    /*�ؼ��¼��ϱ� SECU_MODE_CMD*/
    NAS_LMM_SendOmtKeyEvent(EMM_OMT_KE_SECU_CMD);

    /*ֹͣT3416,ɾ�� RAND�� RES*/
    NAS_LMM_StopPtlTimer(                TI_NAS_EMM_PTL_T3416);
    NAS_EMM_SecuClearRandRes();


#if (VOS_OS_VER != VOS_WIN32)
    ucSmcEia =  pstSmcMsg->aucNasMsg[NAS_EMM_SECU_SMC_ALG_POSE] & NAS_EMM_LOW_3_BITS_F;

    /* �����EIA0�����ж��Ƿ��������ʹ��EIA0������ */
    if (NAS_EMM_SECU_ALG_EIA0 == ucSmcEia)
    {
        /* ��� (�Ȳ��ǽ���ע�������Ҳ��������������)�� ����GCF����������ģʽ
           (������ģʽ�Ҳ��ǽ�����)��������ʹ��EIA0 */
        if((NAS_EMM_YES != NAS_EMM_IsRegStatusEmcRegingOrContainEmcBear())
            && (PS_SUCC != LPS_OM_IsTestMode()))
        {
            NAS_EMM_SECU_LOG_NORM("NAS_EMM_SmcCommProcess:Rejected! it's not allowed to use EIA0.");
            NAS_EMM_SecuSendSmcRej(NAS_EMM_SMC_REJ_UNSPECIFIED);
            return NAS_EMM_SECU_FAIL;
        }
    }
#endif


    /*SMCЯ���Ĳ������:���ȶԿտ���Ϣ���룬Ȼ���SMC��ϢЯ���Ĳ������м���
      ����:KSI, �㷨, replayed UE_NET_CAP, replayed NONCEue,��������*/
    ulRslt = NAS_EMM_SmcVerifyParam(pstSmcMsg);
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        return NAS_EMM_SECU_FAIL;
    }

    /*��SMC��Ϣ���������Լ��*/
    ulRslt = NAS_EMM_SmcIntCheck(pstSmcMsg);
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        return NAS_EMM_SECU_FAIL;
    }

    /*SMC��Ϣ��֤�ɹ��Ĵ���*/
    /*NAS_EMM_SmcSucc(pstSmcMsg);*/

    return NAS_EMM_SECU_SUCC;
}
/*lint +e831*/
/*lint +e415*/
/*lint +e416*/

#if 0

VOS_UINT32 NAS_EMM_SndSmcCmpFailProc(VOS_VOID* pMsg)
{
    (VOS_VOID)pMsg;

    NAS_EMM_SECU_LOG1_INFO("NAS_EMM_SndSmcCmpFailProc enterd. CUR_MS", NAS_EMM_CUR_MAIN_STAT);

    /*ɾ��CUR��ȫ������ */
    NAS_EMM_ClearCurSecuCntxt();

    /*�����SR������ֱ�ӷ�TAU ,��TAU���SR����*/
    if (EMM_MS_SER_INIT == NAS_EMM_CUR_MAIN_STAT)
    {
        /*ֹͣT3417��ʱ��*/
        NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417);

        /*ֹͣT3417ext��ʱ��*/
        NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

        if (VOS_TRUE != NAS_EMM_SER_IsCsfbProcedure())
        {
            /*Inform RABM that SER fail*/
            NAS_EMM_SER_SendRabmReestInd(EMM_ERABM_REEST_STATE_FAIL);
        }

        /*�������̳�ͻ��־λ*/
        NAS_EMM_TAU_SaveEmmCollisionCtrl(NAS_EMM_COLLISION_SERVICE);

        /*NAS_EMM_TAU_SaveEmmTAUStartCause(     NAS_EMM_TAU_START_CAUSE_OTHERS);*/
        NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);

        /*����TAU Procedure*/
        NAS_EMM_TAU_StartTAUREQ();
    }
    else if (EMM_MS_TAU_INIT == NAS_EMM_CUR_MAIN_STAT)
    {
        /*TAU_INIT״̬SMC����ʧ�������·���TAU(Э���Ͻ�Re-initial TAU����˼�Ƿ���һ��
         �µ�TAU���̣�Attemp CounterҪ����*/

        /*ֹͣT3430��ʱ��*/
        NAS_LMM_StopStateTimer(              TI_NAS_EMM_STATE_TAU_T3430);

        /*����Attempt counter */
        NAS_EMM_TAU_SaveEmmTAUAttemptCnt(0);

        /*����TAU Procedure*/
        NAS_EMM_TAU_StartTAUREQ();

    }
    else
    {
        /*����״̬�ݲ�����*/
    }
    return NAS_EMM_SUCC;

}
#endif


VOS_VOID NAS_EMM_HandleLrrcMsgSndFailForHandOver(
                                        NAS_EMM_MRRC_MGMT_DATA_STRU  *pEmmMrrcMgmtData,
                                        VOS_UINT32 *pulIsDelBuff)
{
    NAS_LMM_MAIN_STATE_ENUM_UINT16       enMainState;

    /* �ӵ�ǰ״̬���л�ȡ״̬ */
    enMainState                         = NAS_LMM_GetEmmCurFsmMS();

    if (EMM_MS_TAU_INIT == enMainState)
    {
        /*ֹͣT3430��ʱ��*/
        NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_TAU_T3430);

        /*ֹͣT3440��ʱ��*/
        NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);

        NAS_EMM_TAU_StartTAUREQ();
    }
    else if (EMM_MS_SER_INIT == enMainState)
    {
        /*  ��Ҫ����Ӧ�Ļ�����Ϣ��ulHoWaitSysInfoFlag����Ϊtrue */
        pEmmMrrcMgmtData->ulHoWaitSysInfoFlag = VOS_TRUE;
        *pulIsDelBuff = VOS_FALSE;
    }
    else
    {

    }

}


VOS_UINT32 NAS_EMM_SndCommonProcedureMsgFailProc(VOS_VOID* pMsg,VOS_UINT32 *pulIsDelBuff)
{
    LRRC_LMM_DATA_CNF_STRU              *pstRrcMmDataCnf = VOS_NULL_PTR;
    NAS_LMM_MAIN_STATE_ENUM_UINT16       enMainState;
    NAS_LMM_SUB_STATE_ENUM_UINT16        enSubState;
    NAS_EMM_MRRC_MGMT_DATA_STRU         *pEmmMrrcMgmtData = NAS_EMM_NULL_PTR;

    *pulIsDelBuff = VOS_TRUE;

    switch(NAS_EMM_CUR_MAIN_STAT)
    {
        case    EMM_MS_AUTH_INIT:

            NAS_LMM_IfEmmHasBeenPushedThenPop();
            NAS_EMM_PUBU_LOG_INFO(" NAS_EMM_SndCommonProcedureMsgFailProc enter");
            break;

        default:
            NAS_EMM_PUBU_LOG_INFO(" NAS_EMM_SndCommonProcedureMsgFailProc enter");
            break;
    }

    NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_T3416);

    #if 0
    /* �����ǰ״̬ΪAUTH INITʱ���ڳ�ջʱ����ͣ����������ʱ�� */
    /* ͣT3420��T3418*/
    NAS_LMM_StopStateTimer(              TI_NAS_EMM_T3418);
    NAS_LMM_StopStateTimer(              TI_NAS_EMM_T3420);
    #endif

    NAS_EMM_SecuClearRandRes();

    /* �ӵ�ǰ״̬���л�ȡ״̬ */
    enMainState                         = NAS_LMM_GetEmmCurFsmMS();
    enSubState                          = NAS_LMM_GetEmmCurFsmSS();

    pstRrcMmDataCnf = (LRRC_LMM_DATA_CNF_STRU*) pMsg;

    pEmmMrrcMgmtData = NAS_EMM_FindMsgInDataReqBuffer(pstRrcMmDataCnf->ulOpId);

    if (NAS_EMM_NULL_PTR == pEmmMrrcMgmtData)
    {
        return NAS_EMM_FAIL;
    }

    switch (pstRrcMmDataCnf->enSendRslt)
    {
        case LRRC_LMM_SEND_RSLT_FAILURE_HO:
            NAS_EMM_HandleLrrcMsgSndFailForHandOver(pEmmMrrcMgmtData, pulIsDelBuff);

            break;
        case LRRC_LMM_SEND_RSLT_FAILURE_TXN:
        case LRRC_LMM_SEND_RSLT_FAILURE_RLF:
            if (EMM_MS_REG_INIT == enMainState)
            {
                /* ����ATTACH���� */
                NAS_EMM_RestartAttProcedural();
            }
            else if (EMM_MS_DEREG_INIT == enMainState)
            {
                NAS_LMM_StopStateTimer(     TI_NAS_EMM_T3421);

                /*����DETACH REQUEST��Ϣ*/
                NAS_EMM_SendDetachReqMo();
            }
            else if ((EMM_MS_REG == enMainState)
                       && (EMM_SS_REG_IMSI_DETACH_WATI_CN_DETACH_CNF == enSubState))
            {
                NAS_LMM_StopStateTimer(     TI_NAS_EMM_T3421);

                /* ����IMSI DETACH���� */
                NAS_EMM_SendImsiDetachReqMo();
            }
            else if (EMM_MS_TAU_INIT == enMainState)
            {
                /*ֹͣT3430��ʱ��*/
                NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_TAU_T3430);

                /*ֹͣT3440��ʱ��*/
                NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);

                NAS_EMM_TAU_StartTAUREQ();
            }
            else if (EMM_MS_SER_INIT == enMainState)
            {
                NAS_EMM_RestartSerProcedural();
            }
            else
            {

            }

            break;

        default:
            break;
        }

    return NAS_EMM_SUCC;

}
/*lint +e961*/
/*lint +e960*/

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

