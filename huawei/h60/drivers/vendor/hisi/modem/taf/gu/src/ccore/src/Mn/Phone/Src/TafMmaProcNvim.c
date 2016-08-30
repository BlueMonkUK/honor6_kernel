
/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "UsimmApi.h"
#include "pscfg.h"
#if (FEATURE_LTE == FEATURE_ON)
#include "msp_nvim.h"
#endif
#include "ScInterface.h"
#include "NasNvInterface.h"
#include "TafNvInterface.h"
#include "TafMmaCtx.h"
#include "TafAppMma.h"
#include "MmaAppLocal.h"
#include "GasNvInterface.h"
#if ( FEATURE_ON == FEATURE_LTE )
#include "LPsNvInterface.h"
#include "LNvCommon.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_TAF_MMA_PROC_NVIM_C

/*****************************************************************************
  2 ȫ�ֱ���
*****************************************************************************/
extern NAS_MMA_NEW_ADD_GLABAL_VARIABLE_FOR_PC_REPLAY_ST g_MmaNewValue;
extern MMA_DEACT_SIM_WHEN_POWEROFF_ENUM_UINT8           g_ucMmaDeactSimWhenPoweroff;
extern VOS_UINT16                                       g_usMmaUeSupportCPHSFlg;
extern TAF_MMA_GLOBAL_VALUE_ST                          gstMmaValue;
extern VOS_UINT8                                        g_ucMmaOmConnectFlg;                           /* �Ƿ�����PC��������, VOS_TRUE: ������; VOS_FALSE: δ����.                                                                                   Ĭ��δ���� */
extern VOS_UINT8                                        g_ucMmaOmPcRecurEnableFlg;                     /* ��ǰPC�����Ƿ�ʹ����NAS��PC�ط���Ϣ�ķ���, VOS_TRUE: ʹ��; VOS_FASLE: δʹ��
                                                                                   Ĭ��ʹ��PC�ط� */
extern VOS_UINT32                                       g_ucUsimHotOutFlag;
extern VOS_UINT32                                       g_ucUsimHotInFlag;
extern MMA_UE_BAND_CAPA_ST                              gstMmaBandCapa;
extern MMA_UE_FORB_BAND_ST                              gstMmaForbBand;
extern ENUM_SYSTEM_APP_CONFIG_TYPE_U16                  g_usMmaAppConfigSupportType;
extern VOS_UINT8                                        g_aucMmaImei[TAF_PH_IMEI_LEN-1];


/*****************************************************************************
  3 �궨��
*****************************************************************************/

VOS_VOID TAF_MMA_ReadUeSupporttedGuBandNvim(VOS_VOID)
{
    /* �޸�Ӳ��Ƶ�εĶ�ȡΪ32λ��ʽ */
    VOS_UINT32                          aulSptBand[3];
    MMA_UE_SUPPORT_FREQ_BAND_STRU       stUeBand;
    MMA_USER_BAND_SET_UN                uUserSetBand;
    VOS_UINT32                          ulDataLen;

    ulDataLen = 0;

    /*��ȡ UE֧�ֵ� Band */
    PS_MEM_SET(&stUeBand,0x00,sizeof(stUeBand));
    PS_MEM_SET(&uUserSetBand,0x00,sizeof(uUserSetBand));


    PS_MEM_SET(aulSptBand, 0x00, sizeof(aulSptBand));

    NV_GetLength(en_NV_Item_WG_RF_MAIN_BAND, &ulDataLen);

    if (NV_OK != NV_Read(en_NV_Item_WG_RF_MAIN_BAND,aulSptBand,ulDataLen))
    {
        /*��ӡδ�ɹ���ʼ��ֵ��ԭֵ����*/
        MMA_WARNINGLOG("MMA_InitMeInfo():Read en_NV_Item_WG_RF_MAIN_BAND Failed!");
    }
    else
    {
        stUeBand.unWcdmaBand.ulBand = aulSptBand[0];
        gstMmaBandCapa.unWRFSptBand.ulBand = aulSptBand[0];
        MMA_SwitchUESupportBand2UserSetBand(stUeBand,&uUserSetBand);
        gstMmaBandCapa.ulUeWSptBand = uUserSetBand.ulPrefBand;

        PS_MEM_SET(&stUeBand,0x00,sizeof(stUeBand));
        PS_MEM_SET(&uUserSetBand,0x00,sizeof(uUserSetBand));
        /*V9R1 NV���ݽṹ��չΪ32λ */
        stUeBand.unGsmBand.ulBand = aulSptBand[2];
        gstMmaBandCapa.unGRFSptBand.ulBand = aulSptBand[2];
        /*V9R1 NV���ݽṹ��չΪ32λ */
        MMA_SwitchUESupportBand2UserSetBand(stUeBand,&uUserSetBand);
        gstMmaBandCapa.ulUeGSptBand = uUserSetBand.ulPrefBand;

        gstMmaBandCapa.ulAllUeBand = (gstMmaBandCapa.ulUeGSptBand | gstMmaBandCapa.ulUeWSptBand);
    }

    return;
}
VOS_VOID TAF_MMA_ReadForbBandNvim(VOS_VOID)
{
    VOS_UINT32                          ulLen;
    NAS_MMA_NVIM_FORBAND_STRU           stForband;
    VOS_UINT8                           ucOffSet;


    ulLen = 0;
    PS_MEM_SET(&stForband, 0x00, sizeof(stForband));

    NV_GetLength(en_NV_Item_Forbidden_Band, &ulLen);

    if (ulLen > MMA_FORB_BAND_NV_MAX_SIZE)
    {
        return;
    }

    PS_MEM_SET(&stForband, 0, ulLen);

    if (NV_OK != NV_Read(en_NV_Item_Forbidden_Band,&stForband, ulLen))
    {
        MMA_WARNINGLOG("MMA_GetForbBandFromNV():WARNING: en_NV_Item_Forbbiden_Band Error");
        return;
    }

    /*
    gstMmaForbBand������NV�е�ֵ��һ�£���Ҫ�������ֽڽ���ת��
    VOS_UINT8    ucActiveFlag  - NV���1���ֽ�
    VOS_UINT8    ucForbStatus  - NV���2���ֽ�
    TAF_UINT32   ulBandLow     - NV���3-6���ֽ�
    TAF_UINT32   ulBandHigh    - NV���7-10���ֽ�
    */
    ucOffSet = 0;
    gstMmaForbBand.ucActiveFlag = stForband.aucForband[ucOffSet++];

    if (NV_ITEM_DEACTIVE == gstMmaForbBand.ucActiveFlag)
    {
        return;
    }
    gstMmaForbBand.ucForbStatus = stForband.aucForband[ucOffSet++];

    PS_MEM_CPY(&(gstMmaForbBand.stForbBand.ulBandLow),stForband.aucForband+ucOffSet,
                                sizeof(gstMmaForbBand.stForbBand.ulBandLow));

    ucOffSet += sizeof(gstMmaForbBand.stForbBand.ulBandLow);

    PS_MEM_CPY(&(gstMmaForbBand.stForbBand.ulBandHigh),stForband.aucForband+ucOffSet,
                                sizeof(gstMmaForbBand.stForbBand.ulBandHigh));


    if ( (0 != gstMmaForbBand.stForbBand.ulBandHigh)
      || (TAF_PH_BAND_ANY != gstMmaForbBand.stForbBand.ulBandLow))
    {
        gstMmaForbBand.ulForbBand = gstMmaForbBand.stForbBand.ulBandHigh |
            (gstMmaForbBand.stForbBand.ulBandLow & ~TAF_PH_BAND_NEED_CHANGE_TO_64BIT);
    }
    else
    {
        gstMmaForbBand.ulForbBand = gstMmaForbBand.stForbBand.ulBandLow;
    }

    /* �ݴ��� */
    if (TAF_PH_BAND_ANY == gstMmaForbBand.ulForbBand)
    {
        gstMmaForbBand.ulForbBand = 0;
    }

    return;
}


#if (FEATURE_ON == FEATURE_LTE)
VOS_VOID TAF_MMA_ReadNvimUeSupporttedLteBand(VOS_VOID)
{
    TAF_USER_SET_PREF_BAND64                   *pstUeSupportLteBand = VOS_NULL_PTR;

    VOS_UINT32                                  ulRslt;
    LRRC_NV_UE_EUTRA_CAP_STRU                  *pstUECapability     = VOS_NULL_PTR;
    RRC_UE_CAP_RF_PARA_STRU                    *pstRfPara           = VOS_NULL_PTR;
    VOS_UINT32                                  i;
    VOS_UINT8                                   ucBandNo;

    pstUeSupportLteBand = MN_MMA_GetUeSupportLteBandAddr();

    PS_MEM_SET(pstUeSupportLteBand, 0X0, sizeof(TAF_USER_SET_PREF_BAND64));



    pstUECapability =(LRRC_NV_UE_EUTRA_CAP_STRU*)PS_MEM_ALLOC(WUEPS_PID_MMA,
                            sizeof(LRRC_NV_UE_EUTRA_CAP_STRU));

    if(VOS_NULL_PTR == pstUECapability)
    {
        MN_ERR_LOG("TAF_MMA_ReadNvimUeSupporttedLteBand():PS_MEM_ALLOC failed!");
        return ;
    }

    PS_MEM_SET(pstUECapability, 0x00, sizeof(LRRC_NV_UE_EUTRA_CAP_STRU));


    /* LTE NV��� */
    ulRslt = NVM_Read(EN_NV_ID_UE_CAPABILITY, pstUECapability, sizeof(LRRC_NV_UE_EUTRA_CAP_STRU));


    if ( (NV_OK != ulRslt)
      || (NVIM_EUTRA_MAX_SUPPORT_BANDS_NUM < pstUECapability->stRfPara.usCnt) )
    {
        /* ��NVʧ��,��ӡδ�ɹ���ʼ��ֵ��Ĭ�ϸ�ֵΪ֧������Ƶ�� */
        MN_ERR_LOG("TAF_MMA_ReadNvimUeSupporttedLteBand():read en_NV_Item_EUTRA_CAPA_COMM_INFO error");
        pstUeSupportLteBand->ulBandHigh = MN_MMA_LTE_HIGH_BAND_ALL;
        pstUeSupportLteBand->ulBandLow  = MN_MMA_LTE_LOW_BAND_ALL;

        PS_MEM_FREE(WUEPS_PID_MMA, pstUECapability);

        return;
    }

    /* ��ȡNV�е�Ƶ����Ϣ:
       ulBandLow�����bit��Ӧband1�����bit��Ӧband32
       ulBandHigh�����bit��Ӧband33�����bit��Ӧband64 */
    pstRfPara = &(pstUECapability->stRfPara);

    for ( i = 0; i < pstRfPara->usCnt; i++ )
    {
        ucBandNo = (pstRfPara->astSuppEutraBandList[i]).ucEutraBand;

        /* NV�е�Ƶ����ϢָʾΪ��32,����Ӧband1��band32*/
        if ( ucBandNo <= (NVIM_EUTRA_MAX_SUPPORT_BANDS_NUM / 2) )
        {
            pstUeSupportLteBand->ulBandLow  |= (VOS_UINT32)( 0x00000001 << (ucBandNo - 1) );

        }

        /* NV�е�Ƶ����ϢָʾΪ��32,����Ӧband33��band64 */
        else if ( ucBandNo <= NVIM_EUTRA_MAX_SUPPORT_BANDS_NUM  )
        {
            pstUeSupportLteBand->ulBandHigh |= (VOS_UINT32)( 0x00000001 << (ucBandNo - 32 - 1) );
        }
        else
        {
            /* band���쳣 */
            MN_ERR_LOG("TAF_MMA_ReadNvimUeSupporttedLteBand():LTE BAND NO IS BIG THAN 64, error");
        }
    }

    PS_MEM_FREE(WUEPS_PID_MMA, pstUECapability);

    return;
}
VOS_VOID TAF_MMA_ReadUserSetLteBands(VOS_VOID)
{
    MN_MMA_SYSCFG_USER_SET_BAND_STRU  *pstUserSetBand = VOS_NULL_PTR;

    pstUserSetBand = MN_MMA_GetSyscfgUserSetBandAddr();

    /* ���û����õ�LTEƵ�� NV�� */
    if (NV_OK != NV_Read(en_NV_Item_USER_SET_LTEBANDS,
                         &(pstUserSetBand->stUserSetLteBand), sizeof(TAF_USER_SET_PREF_BAND64)))
    {
        /* ��NVʧ��,��ӡδ�ɹ���ʼ��ֵ��Ĭ�ϸ�ֵΪ֧������Ƶ�� */
        pstUserSetBand->stUserSetLteBand.ulBandLow  = MN_MMA_LTE_LOW_BAND_ANY;
        pstUserSetBand->stUserSetLteBand.ulBandHigh = MN_MMA_LTE_HIGH_BAND_ANY;
    }

    return;
}


#endif

/*****************************************************************************
 �� �� ��  : TAF_MMA_ReadFreqBandNvim
 ��������  : ��NV��en_NV_Item_Support_Freqbands�л�ȡ��ǰϵͳ���õ�Ƶ��ֵ
 �������  : ��
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2010��4��23��
    ��    ��   : likelai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID  TAF_MMA_ReadFreqBandNvim(VOS_VOID)
{
    MN_MMA_SYSCFG_USER_SET_BAND_STRU   *pstUserSetBand = VOS_NULL_PTR;
    NVIM_UE_SUPPORT_FREQ_BAND_STRU      stUserSetFreqBand;

    PS_MEM_SET(&stUserSetFreqBand, 0x00, sizeof(stUserSetFreqBand));

    pstUserSetBand = MN_MMA_GetSyscfgUserSetBandAddr();

    if (NV_OK != NV_Read(en_NV_Item_Support_Freqbands,
                         &stUserSetFreqBand, sizeof(NVIM_UE_SUPPORT_FREQ_BAND_STRU)))
    {
        /* ��ӡδ�ɹ���ʼ��ֵ��ԭֵ���� */
        MN_WARN_LOG("TAF_MMA_ReadFreqBandNvim():Read Support_Freqbands Failed!");

        return;
    }

    pstUserSetBand->uUserSetUeFormatGuBand.unWcdmaBand.ulBand = stUserSetFreqBand.ulWcdmaBand;
    pstUserSetBand->uUserSetUeFormatGuBand.unGsmBand.ulBand   = stUserSetFreqBand.ulGsmBand;

    PS_MEM_CPY(pstUserSetBand->uUserSetUeFormatGuBand.aucUeSupportWcdmaBand,
                stUserSetFreqBand.aucUeSupportWcdmaBand,
                NVIM_MAX_FDD_FREQ_BANDS_NUM);

    PS_MEM_CPY(pstUserSetBand->uUserSetUeFormatGuBand.aucUeSupportGsmBand,
               stUserSetFreqBand.aucUeSupportGsmBand,
               NVIM_MAX_FDD_FREQ_BANDS_NUM);

    return;
}
VOS_VOID TAF_MMA_ReadUserSettedBandNvim(VOS_VOID)
{
    MN_MMA_SYSCFG_USER_SET_BAND_STRU  *pstUserSetBand = VOS_NULL_PTR;

    pstUserSetBand = MN_MMA_GetSyscfgUserSetBandAddr();

    /* ��ȡ�û����õ�GUƵ�� */
    if (NV_OK != NV_Read(en_NV_Item_User_Set_Freqbands,
                         &(pstUserSetBand->stOrigUserSetGuBand),
                         sizeof(TAF_USER_SET_PREF_BAND64)))
    {
        /*��ӡδ�ɹ���ʼ��ֵ��ԭֵ����*/
        pstUserSetBand->uUserSetGuBand.ulPrefBand      = TAF_PH_BAND_ANY;
        pstUserSetBand->stOrigUserSetGuBand.ulBandHigh = 0;
        pstUserSetBand->stOrigUserSetGuBand.ulBandLow  = TAF_PH_BAND_ANY;
        MMA_WARNINGLOG("TAF_MMA_ReadUserSettedBandNvim():Read UserSet_Freqbands Failed!");
    }
    else
    {
        if ( (0 != pstUserSetBand->stOrigUserSetGuBand.ulBandHigh )
          || ( TAF_PH_BAND_ANY != pstUserSetBand->stOrigUserSetGuBand.ulBandLow) )
        {
            pstUserSetBand->uUserSetGuBand.ulPrefBand = pstUserSetBand->stOrigUserSetGuBand.ulBandHigh |
                                        (pstUserSetBand->stOrigUserSetGuBand.ulBandLow & ~TAF_PH_BAND_NEED_CHANGE_TO_64BIT);
            /* ������ЧƵ�� */
            pstUserSetBand->uUserSetGuBand.ulPrefBand &= (gstMmaBandCapa.ulAllUeBand);
        }
        else
        {
            /* ��ǰƵ��ΪTAF_PH_BAND_ANYʱ����ֵΪ����֧�ֵ�����Ƶ�� */
            pstUserSetBand->uUserSetGuBand.ulPrefBand = gstMmaBandCapa.ulAllUeBand;
        }
    }

    MN_INFO_LOG1("TAF_MMA_ReadUserSettedBandNvim():Support_Freqbands:",(VOS_INT32)pstUserSetBand->uUserSetGuBand.ulPrefBand);


    return;
}



VOS_VOID  TAF_MMA_UpdateBandCapa(VOS_VOID)
{
    gstMmaBandCapa.ulUeWSptBand &= ~(gstMmaForbBand.ulForbBand);
    gstMmaBandCapa.ulUeGSptBand &= ~(gstMmaForbBand.ulForbBand);
    gstMmaBandCapa.ulAllUeBand = (gstMmaBandCapa.ulUeGSptBand | gstMmaBandCapa.ulUeWSptBand);

    return;
}


VOS_VOID  TAF_MMA_UpdateUserSetFreqBand(VOS_VOID)
{
    TAF_USER_SET_PREF_BAND64            stUserSetBandAny;  /*64λԭʼƵ��*/
    MN_MMA_SYSCFG_USER_SET_BAND_STRU   *pstUserSetBand = VOS_NULL_PTR;

    pstUserSetBand = MN_MMA_GetSyscfgUserSetBandAddr();

    stUserSetBandAny.ulBandHigh = 0;
    stUserSetBandAny.ulBandLow = TAF_PH_BAND_ANY;

    /* �����û�����Ƶ�ε�NVΪ3fffffff */
    if (MMA_SUCCESS != MMA_WriteValue2Nvim(en_NV_Item_User_Set_Freqbands,
                                           &stUserSetBandAny,
                                           sizeof(stUserSetBandAny)))
    {
        MMA_WARNINGLOG("MMA_UpdateBandNvRefForbBand():WARNING:NVIM_Write en_NV_Item_User_Set_Freqbands failed");
    }

    /* ��ǰƵ��ΪTAF_PH_BAND_ANYʱ����ֵΪ����֧�ֵ�����Ƶ�� */
    /* д��en_NV_Item_User_Set_Freqbands֮��ֱ�Ӹ���ȫ�ֱ������������������¶���NV */
    pstUserSetBand->uUserSetGuBand.ulPrefBand = gstMmaBandCapa.ulAllUeBand;

    return;
}


VOS_VOID  TAF_MMA_UpdateSupportFreqBand(
    MMA_UE_SUPPORT_FREQ_BAND_STRU      *pstOrigFreqBand
)
{
    MN_MMA_SYSCFG_USER_SET_BAND_STRU   *pstUserSetBand = VOS_NULL_PTR;

    pstUserSetBand = MN_MMA_GetSyscfgUserSetBandAddr();

    /* ����Ƶ��NV */
    if (MMA_SUCCESS != MMA_WriteValue2Nvim(en_NV_Item_Support_Freqbands,
                                           pstOrigFreqBand,
                                           sizeof(pstOrigFreqBand->unWcdmaBand)+sizeof(pstOrigFreqBand->unGsmBand)))
    {
        MMA_WARNINGLOG("MMA_UpdateBandNvRefForbBand():WARNING:NVIM_Write en_NV_Item_Support_Freqbands failed");
    }

    /* д��en_NV_Item_Support_Freqbands֮��ֱ�Ӹ���ȫ�ֱ������������������¶���NV */
    PS_MEM_CPY(&(pstUserSetBand->uUserSetUeFormatGuBand),
                pstOrigFreqBand,
                sizeof(MMA_UE_SUPPORT_FREQ_BAND_STRU));
    return;
}


VOS_VOID MMA_UpdateBandNvRefForbBand(VOS_VOID)
{
    MMA_UE_SUPPORT_FREQ_BAND_STRU       stOrigFreqBand;
    MMA_USER_BAND_SET_UN                uOrigFreqBand;
    MN_MMA_SYSCFG_USER_SET_BAND_STRU   *pstUserSetBand = VOS_NULL_PTR;

    pstUserSetBand = MN_MMA_GetSyscfgUserSetBandAddr();


    PS_MEM_SET(&uOrigFreqBand, 0, sizeof(MMA_USER_BAND_SET_UN));

    PS_MEM_SET(&stOrigFreqBand, 0x00, sizeof(stOrigFreqBand));

    stOrigFreqBand  = pstUserSetBand->uUserSetUeFormatGuBand;

    /*����ǰϵͳ���õ�Ƶ��ת��Ϊ�û����õĸ�ʽ*/
    MMA_SwitchUESupportBand2UserSetBand(stOrigFreqBand,&uOrigFreqBand);

    /* ��ǰϵͳ���õ�Ƶ����ulForbBand�Ƿ��н��� */
    if (0 != (uOrigFreqBand.ulPrefBand & gstMmaForbBand.ulForbBand))
    {
        /*���˵���ǰ��ֹ��Ƶ��*/
        uOrigFreqBand.ulPrefBand &= ~(gstMmaForbBand.ulForbBand);

        MMA_SwitchUserSetBand2UESupportBand(uOrigFreqBand, &stOrigFreqBand);

        /* дNV:en_NV_Item_User_Set_Freqbands,����ȫ�ֱ��� */
        TAF_MMA_UpdateUserSetFreqBand();

        /* дNV:en_NV_Item_Support_Freqbands,����ȫ�ֱ��� */
        TAF_MMA_UpdateSupportFreqBand(&stOrigFreqBand);
    }

}
/*****************************************************************************
 �� �� ��  : MMA_UpdateForbBandStatusToNV
 ��������  : ��ForbBand��NV��״̬����ΪVOS_TRUE
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2010��4��23��
    ��    ��   : likelai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID MMA_UpdateForbBandStatusToNV(VOS_VOID)
{
    VOS_UINT32                           ulRslt;

    gstMmaForbBand.ucForbStatus = VOS_TRUE;

    ulRslt = MMA_WriteValue2Nvim(en_NV_Item_Forbidden_Band,
                   &gstMmaForbBand,
                   sizeof(gstMmaForbBand.ucActiveFlag)+sizeof(gstMmaForbBand.ucForbStatus));

    if (MMA_SUCCESS != ulRslt)
    {
        MMA_WARNINGLOG("MMA_UpdateForbBandStatusToNV():WARNING:NVIM_Write en_NV_Item_Forbbiden_Band failed");
    }

    return;
}


VOS_VOID TAF_MMA_UpdateUeAllowedBandRange(VOS_VOID)
{
    /* Ƶ����ص�NV����:
    1.��en_NV_Item_WG_RF_MAIN_BAND
    2.��en_NV_Item_Forbidden_Band
    3.��en_NV_Item_EUTRA_CAPA_COMM_INFO
    4.��en_NV_Item_Support_Freqbands
    5.дen_NV_Item_User_Set_Freqbands��
      ͬʱ����gstMmaValue.stLastSyscfgSet.stUserSetBand.uUserSetGuBand.ulPrefBand
    6.дen_NV_Item_Support_Freqbands
      ͬʱ����gstMmaValue.stLastSyscfgSet.stUserSetBand.uUserSetUeFormatGuBand
    7.дen_NV_Item_Forbidden_Band
    8.��en_NV_Item_USER_SET_LTEBANDS
    */

    /* en_NV_Item_WG_RF_MAIN_BAND */
    /*��ȡӲ��֧�ֵ�Ƶ�η�Χ*/
    TAF_MMA_ReadUeSupporttedGuBandNvim();

    /* en_NV_Item_Forbidden_Band */
    /*��ȡ��ֹƵ�ε�״̬�ͷ�Χ*/
    TAF_MMA_ReadForbBandNvim();

#if (FEATURE_ON == FEATURE_LTE)
    /* en_NV_Item_EUTRA_CAPA_COMM_INFO */
    /* ��ȡ���֧�ֵ�LTE��Ƶ�η�Χ */
    TAF_MMA_ReadNvimUeSupporttedLteBand();

    /* en_NV_Item_USER_SET_LTEBANDS */
    /* ��ȡ�û����õ�LTE��Ƶ�η�Χ */
    TAF_MMA_ReadUserSetLteBands();
#endif

    /* en_NV_Item_Support_Freqbands */
    TAF_MMA_ReadFreqBandNvim();

    /* ��ȡen_NV_Item_User_Set_Freqbands*/
    TAF_MMA_ReadUserSettedBandNvim();

    if (NV_ITEM_ACTIVE == gstMmaForbBand.ucActiveFlag)
    {
        /* ����gstMmaBandCapa */
        TAF_MMA_UpdateBandCapa();
    }


    return;
}
VOS_VOID TAF_MMA_ReadPlmnSelectionModeNvim(VOS_VOID)
{
    NAS_NVIM_SELPLMN_MODE_STRU          stPlmnSelMode;
    VOS_UINT32                          ulLength;

    ulLength = 0;

    NV_GetLength(en_NV_Item_SelPlmn_Mode, &ulLength);
    if (ulLength > sizeof(NAS_NVIM_SELPLMN_MODE_STRU))
    {
        return;
    }


    stPlmnSelMode.usSelPlmnMode = MMA_MMC_PLMN_SEL_MODE_AUTO;

    if (NV_OK != NV_Read(en_NV_Item_SelPlmn_Mode, &stPlmnSelMode, ulLength))
    {
        MMA_ERRORLOG("TAF_MMA_ReadPlmnSelectionModeNvim(): Read NV Fail");
        return;
    }

    if ((stPlmnSelMode.usSelPlmnMode != MMA_MMC_PLMN_SEL_MODE_AUTO)
     && (stPlmnSelMode.usSelPlmnMode != MMA_MMC_PLMN_SEL_MODE_MANUAL))
    {
        MMA_ERRORLOG("TAF_MMA_ReadPlmnSelectionModeNvim(): ucPlmnSelMode is invalid");
        return;
    }


    TAF_MMA_SetPlmnSelectionMode((VOS_UINT8)stPlmnSelMode.usSelPlmnMode);

    return;
}
VOS_VOID TAF_MMA_ReadRatPrioListNvim(VOS_VOID)
{
    TAF_PH_RAT_ORDER_STRU              *pstRatPrioList = VOS_NULL_PTR;

    /* ���뼼��ƽ������δʹ�ܣ����뼼����NV��en_NV_Item_RAT_PRIO_LIST��ȡ */
    if (VOS_TRUE != TAF_MMA_GetRatBalancingEnableFlg())
    {
        /* ��ȡ��ǰ�Ľ���ģʽ���ȼ� */
        pstRatPrioList = MN_MMA_GetRatPrioListAddr();
        TAF_MMA_ReadNvimRatPrioList(pstRatPrioList);
        PS_MEM_CPY(&(gstMmaValue.pg_StatusContext->stRatPrioList),
                   pstRatPrioList, sizeof(TAF_PH_RAT_ORDER_STRU));

        TAF_SDC_SetMsPrioRatList((TAF_SDC_RAT_PRIO_STRU *)pstRatPrioList);
    }
    /* ���뼼��ƽ������ʹ�ܣ��յ���״̬���ҿ���λʱ���ٴ�4F36�ļ���ȡ */
    else
    {
        MMA_NORMAILLOG("TAF_MMA_InitRatPrioList(): WAIT FOR READING 4F36.");
    }

    return;
}
VOS_VOID TAF_MMA_ReadMsClassNvim(VOS_VOID)
{
    NAS_NVIM_MS_CLASS_STRU              stMsClass;
    VOS_UINT32                          ulLength;
    MN_MMA_LAST_SETTED_SYSCFG_SET_STRU *pstLastSyscfgSet = VOS_NULL_PTR;

    PS_MEM_SET(&stMsClass, 0x00, sizeof(NAS_NVIM_MS_CLASS_STRU));
    pstLastSyscfgSet = MN_MMA_GetLastSyscfgSetAddr();
    ulLength = 0;

    NV_GetLength(en_NV_Item_MMA_MsClass, &ulLength);
    if (NV_OK != NV_Read(en_NV_Item_MMA_MsClass,
                         &stMsClass,
                         ulLength))
    {
        pstLastSyscfgSet->ucSrvDomain = TAF_PH_SERVICE_CS_PS;
    }
    else
    {
        pstLastSyscfgSet->ucSrvDomain = stMsClass.ucMsClass;
    }

    MMA_ChangeSrv2Class(pstLastSyscfgSet->ucSrvDomain, &gstMmaValue.stSetMsClass.MsClassType);

    gstMmaValue.stSetMsClass.NewMsClassType             = gstMmaValue.stSetMsClass.MsClassType;
    /*ͬ���ò���λ��STA��ģ���е�ֵ*/
    gstMmaValue.pg_StatusContext->ucModeService         = gstMmaValue.stSetMsClass.MsClassType;
    return;
}
VOS_VOID TAF_MMA_ReadRoamCapaNvim(VOS_VOID)
{
    VOS_UINT32                          ulLength;
    NAS_NVIM_ROAM_CFG_INFO_STRU         stRoamFeature;
    MN_MMA_LAST_SETTED_SYSCFG_SET_STRU *pstLastSyscfgSet = VOS_NULL_PTR;

    PS_MEM_SET(&stRoamFeature, 0x00, sizeof(stRoamFeature));
    pstLastSyscfgSet = MN_MMA_GetLastSyscfgSetAddr();
    ulLength = 0;

    NV_GetLength(en_NV_Item_Roam_Capa, &ulLength);

    if ( NV_OK != NV_Read(en_NV_Item_Roam_Capa,
                          &stRoamFeature,
                          ulLength))
    {
        MMA_WARNINGLOG("TAF_MMA_ReadRoamCapaNvim():Read en_NV_Item_Roam_Capa Failed!");
        stRoamFeature.ucRoamFeatureFlg = MMA_MMC_ROAM_FEATURE_OFF;
        stRoamFeature.ucRoamCapability = MMA_MMC_ROAM_NATION_OFF_INTERNATION_OFF;
    }
    else
    {
        if (stRoamFeature.ucRoamFeatureFlg >= MMA_MMC_ROAM_FEATURE_BUTT)
        {
            stRoamFeature.ucRoamFeatureFlg = MMA_MMC_ROAM_FEATURE_OFF;
        }
        if (stRoamFeature.ucRoamCapability >= MMA_MMC_ROAM_BUTT)
        {
            stRoamFeature.ucRoamCapability = MMA_MMC_ROAM_NATION_OFF_INTERNATION_OFF;
        }
    }

    gstMmaValue.ucRoamFeatureStatus = stRoamFeature.ucRoamFeatureFlg;
    pstLastSyscfgSet->ucRoam        = stRoamFeature.ucRoamCapability;
    return;
}


VOS_VOID TAF_MMA_ReadRoamingBrokerInfoNvim(VOS_VOID)
{
    NAS_MMA_NVIM_ROAMING_BROKER_STRU    stRoamingBroker;
    VOS_UINT32                          ulLength;

    ulLength = 0;

    PS_MEM_SET(&stRoamingBroker, 0, sizeof(NAS_MMA_NVIM_ROAMING_BROKER_STRU));
    NV_GetLength(en_NV_Item_Roaming_Broker, &ulLength);

    if (NV_OK != NV_Read(en_NV_Item_Roaming_Broker, &stRoamingBroker, ulLength))
    {
        NAS_WARNING_LOG(WUEPS_PID_MMA, "TAF_MMA_ReadRoamingBrokerInfoNvim: Read NV Fail ");
        gstMmaValue.ucRoamBrokerSwitch = VOS_FALSE;
    }
    else
    {
        gstMmaValue.ucRoamBrokerSwitch = (stRoamingBroker.aucRoamingBroker)[0];

        /* ���NV�е�ֵ��Ч��������RoamBroker���ܹر� */
        if ( (VOS_FALSE != (stRoamingBroker.aucRoamingBroker)[0]) && (VOS_TRUE != (stRoamingBroker.aucRoamingBroker)[0]) )
        {
            gstMmaValue.ucRoamBrokerSwitch = VOS_FALSE;
        }
    }

    return;
}
VOS_VOID TAF_MMA_ReadReportPlmnSupportFlgNvim(VOS_VOID)
{
    TAF_MMA_NVIM_REPORT_PLMN_SUPPORT_FLG_STRU   stReportPlmn;
    VOS_UINT32                                  ulLength;


    PS_MEM_SET(&stReportPlmn, 0x00, sizeof(stReportPlmn));
    ulLength = 0;

    /* �Ȼ�ȡNV�ĳ��� */
    NV_GetLength(en_NV_Item_REPORT_PLMN_SUPPORT_FLAG, &ulLength);

    if (ulLength > sizeof(TAF_MMA_NVIM_REPORT_PLMN_SUPPORT_FLG_STRU))
    {
        NAS_ERROR_LOG(WUEPS_PID_MMA,"TAF_MMA_ReadNvimReportPlmnSupportFlg():WARNING: en_NV_Item_REPORT_PLMN_SUPPORT_FLAG length Error");
        gstMmaValue.ucReportPlmnFlg = VOS_FALSE;
        return;
    }

    if (NV_OK != NV_Read (en_NV_Item_REPORT_PLMN_SUPPORT_FLAG, &stReportPlmn, ulLength))
    {
        gstMmaValue.ucReportPlmnFlg = VOS_FALSE;
        return;
    }

    if ((VOS_TRUE  != stReportPlmn.ucNvimActiveFlg)
     && (VOS_FALSE != stReportPlmn.ucNvimActiveFlg))
    {
        gstMmaValue.ucReportPlmnFlg = VOS_FALSE;
        return;
    }

    /* ��NVֵ��ֵ��ȫ�ֱ��� */
    gstMmaValue.ucReportPlmnFlg = stReportPlmn.ucNvimActiveFlg;

    return;
}
VOS_VOID TAF_MMA_ReadAccessModeNvim(VOS_VOID)
{
    VOS_UINT32                          ulLength;
    NAS_MMA_NVIM_ACCESS_MODE_STRU       stAccessMode;
    MN_MMA_LAST_SETTED_SYSCFG_SET_STRU *pstLastSyscfgSet = VOS_NULL_PTR;

    PS_MEM_SET(&stAccessMode, 0x00, sizeof(NAS_MMA_NVIM_ACCESS_MODE_STRU));
    pstLastSyscfgSet = MN_MMA_GetLastSyscfgSetAddr();
    ulLength = 0;

    NV_GetLength(en_NV_Item_MMA_AccessMode, &ulLength);
    if (ulLength > sizeof(NAS_MMA_NVIM_ACCESS_MODE_STRU))
    {
        return;
    }

    if (NV_OK != NV_Read(en_NV_Item_MMA_AccessMode ,
                         &stAccessMode,
                         ulLength))

    {
        MN_WARN_LOG("TAF_MMA_ReadAccessModeNvim:Read:NV_Read runs failed");
        return ;
    }
    pstLastSyscfgSet->enPrioRat = (stAccessMode.aucAccessMode)[1];
    return;
}


VOS_VOID TAF_MMA_ReadFollowonOpenSpeedFlagNvim(VOS_VOID)
{
    VOS_UINT32                              ulLength;
    NAS_NVIM_FOLLOWON_OPENSPEED_FLAG_STRU   stQuickStartFlg;

    PS_MEM_SET(&stQuickStartFlg, 0x00, sizeof(NAS_NVIM_FOLLOWON_OPENSPEED_FLAG_STRU));
    ulLength = sizeof(NAS_NVIM_FOLLOWON_OPENSPEED_FLAG_STRU);

    if (NV_OK != NV_Read(en_NV_Item_FollowOn_OpenSpeed_Flag,
                          (VOS_VOID*)(&(stQuickStartFlg.ulQuickStartSta)),
                          ulLength))
    {
       gstMmaValue.ulQuickStartFlg = MMA_QUICK_START_DISABLE;

       return;
    }

    gstMmaValue.ulQuickStartFlg = stQuickStartFlg.ulQuickStartSta;
    return;
}


VOS_VOID TAF_MMA_ReadRfAutoTestFlagNvim(VOS_VOID)
{
    VOS_UINT32                          ulDataLen;
    VOS_UINT32                          ulAutoSwitchOnFlg;

    ulAutoSwitchOnFlg   = MMA_RF_AUTO_TEST_DISABLE;
    ulDataLen           = sizeof(VOS_UINT16);

    if (NV_OK != NV_Read(en_NV_Item_RF_Auto_Test_Flag,
                          (VOS_VOID*)(&(ulAutoSwitchOnFlg)),
                           ulDataLen))
    {
       TAF_MMA_SetAutoSwitchOnFlg(MMA_RF_AUTO_TEST_ENABLE);

       return;
    }

    TAF_MMA_SetAutoSwitchOnFlg(ulAutoSwitchOnFlg);

#if 0
#ifdef NAS_STUB
    TAF_MMA_SetAutoSwitchOnFlg(MMA_RF_AUTO_TEST_DISABLE);
#endif
#endif

    return;
}


VOS_VOID TAF_MMA_ReadEFustServiceCfgNvim(VOS_VOID)
{
    VOS_UINT32                          ulDataLen;

    ulDataLen = sizeof(MMA_EFUST_SERVICE_CFG_STRU);

    if (NV_OK != NV_Read(en_NV_Item_EFust_Service_Cfg,
                          (VOS_VOID*)(&(gstMmaValue.stEfustServiceCfg)),
                           ulDataLen))
    {
         /*��ӡδ�ɹ���ʼ��ֵ��ԭֵ����*/
        MMA_WARNINGLOG("TAF_MMA_ReadEFustServiceCfgNvim():Read EFust_Service_Cfg Failed!");
    }

    if (NV_ITEM_DEACTIVE == gstMmaValue.stEfustServiceCfg.ucEnableFlag)
    {
        gstMmaValue.stEfustServiceCfg.ucForbidSndMsg = 0;
        gstMmaValue.stEfustServiceCfg.ucForbidReg2GNetWork = 0;
    }

    return;
}
VOS_VOID TAF_MMA_ReadDisplaySpnFlagNvim(VOS_VOID)
{
    VOS_UINT32                          ulLength;
    NAS_MMA_NVIM_DISPLAY_SPN_FLAG_STRU  stSpnFlag;

    PS_MEM_SET(&stSpnFlag, 0x00, sizeof(NAS_MMA_NVIM_DISPLAY_SPN_FLAG_STRU));
    NV_GetLength(en_NV_Item_Display_Spn_Flag, &ulLength);

    if (NV_OK != NV_Read(en_NV_Item_Display_Spn_Flag,
                          (VOS_VOID*)(&stSpnFlag),
                           ulLength))
    {
       gstMmaValue.ucCopsNotDisplaySpnFlg = MMA_FALSE;
    }
    else if (NV_ITEM_ACTIVE == (stSpnFlag.aucDisplaySpnFlag)[0])
    {
        gstMmaValue.ucCopsNotDisplaySpnFlg = (stSpnFlag.aucDisplaySpnFlag)[1];
    }
    else
    {

    }

    return;
}


VOS_VOID TAF_MMA_ReadNotDisplayLocalNetworkNameNvim(VOS_VOID)
{
    VOS_UINT32                                              ulLength;
    TAF_MMA_NOT_DISPLAY_LOCAL_NETWORKNAME_NVIM_STRU         stNotDisplayLocalNetworkNameStru;

    PS_MEM_SET(&stNotDisplayLocalNetworkNameStru, 0x00, sizeof(TAF_MMA_NOT_DISPLAY_LOCAL_NETWORKNAME_NVIM_STRU));
    NV_GetLength(en_NV_Item_NotDisplayLocalNetworkName, &ulLength);

    /* ��NV�л�ȡ�û����õ��Ƿ���ʾ���ص�ȫ����Ӫ����Ϣ��־ */
    if (NV_OK == NV_Read(en_NV_Item_NotDisplayLocalNetworkName,
                         &stNotDisplayLocalNetworkNameStru,
                         ulLength))
    {
       gstMmaValue.ucNotDisplayLocalNetworkNameFlg = stNotDisplayLocalNetworkNameStru.ucNotDisplayLocalNetworkNameFlg;
    }


    return;
}
VOS_VOID TAF_MMA_ReadSystemAppCofigNvim(VOS_VOID)
{
    VOS_UINT32                          ulLength;
    NAS_NVIM_SYSTEM_APP_CONFIG_STRU     stSysAppConfig;

    PS_MEM_SET(&stSysAppConfig, 0x00, sizeof(NAS_NVIM_SYSTEM_APP_CONFIG_STRU));

    /* ��ȡen_NV_Item_System_APP_Config����ȡ�Խӵ��ϲ�Ӧ�� */
    NV_GetLength(en_NV_Item_System_APP_Config, &ulLength);

    if (NV_OK != NV_Read(en_NV_Item_System_APP_Config,
                         &stSysAppConfig, ulLength))
    {
        stSysAppConfig.usSysAppConfigType = SYSTEM_APP_MP;
        MMA_WARNINGLOG("TAF_MMA_ReadSystemAppCofigNvim():Read en_NV_Item_System_APP_Config Failed!");
    }

    if ( SYSTEM_APP_ANDROID < stSysAppConfig.usSysAppConfigType)
    {
        g_usMmaAppConfigSupportType = SYSTEM_APP_MP;
        MMA_WARNINGLOG("TAF_MMA_ReadSystemAppCofigNvim():en_NV_Item_System_APP_Config error!");
    }
    else
    {
        g_usMmaAppConfigSupportType = stSysAppConfig.usSysAppConfigType;
    }


    return;
}
VOS_VOID TAF_MMA_ReadImeiNvim(VOS_VOID)
{
    TAF_PH_MEINFO_STRU                 *pstMeInfo = VOS_NULL_PTR;
    VOS_UINT8                           aucBuf[TAF_PH_IMEI_LEN + 1];
    VOS_UINT8                           ucCheckData;
    VOS_UINT32                          ulDataLen;
    VOS_UINT32                          i;
    MODEM_ID_ENUM_UINT16                enModemId;

    ulDataLen   = TAF_PH_IMEI_LEN;
    ucCheckData = 0;
    pstMeInfo   = &(gstMmaValue.stMeInfo);
    PS_MEM_SET(aucBuf, 0x00, sizeof(aucBuf));

    /* ��PID��ȡMODEMID */
    enModemId = VOS_GetModemIDFromPid(WUEPS_PID_MMA);

    /* ����SC�ӿڶ�ȡIMEI���� */
    SC_PERS_NvRead(enModemId, en_NV_Item_IMEI, aucBuf, NV_ITEM_IMEI_SIZE);

    for (i = 0; i < (TAF_PH_IMEI_LEN-2); i += 2)
    {
        ucCheckData += aucBuf[i]
                      +((aucBuf[i+1] + aucBuf[i+1])/10)
                      +((aucBuf[i+1] + aucBuf[i+1])%10);
    }

    ucCheckData = (10 - (ucCheckData%10))%10;

    MMA_ImeiBcd2Ascii((VOS_UINT8)ulDataLen, aucBuf, (pstMeInfo->ImeisV.aucImei));

    (pstMeInfo->ImeisV.aucImei)[TAF_PH_IMEI_LEN-2] = (VOS_UINT8)(ucCheckData + 0x30);
    (pstMeInfo->ImeisV.aucImei)[TAF_PH_IMEI_LEN-1] = 0;

    PS_MEM_CPY(g_MmaNewValue.aucImeiBcd, aucBuf, ulDataLen);

    PS_MEM_CPY(g_aucMmaImei, pstMeInfo->ImeisV.aucImei, TAF_PH_IMEI_LEN - 1);

    return;
}


VOS_VOID TAF_MMA_ReadFmridNvim(VOS_VOID)
{
    VOS_UINT32                          ulDataLen;
    TAF_PH_MEINFO_STRU                 *pstMeInfo = VOS_NULL_PTR;

    pstMeInfo = &(gstMmaValue.stMeInfo);
    ulDataLen = TAF_MAX_MFR_ID_LEN + 1;

    /*��ȡ��������*/
    if (NV_OK != NV_Read(en_NV_Item_FMRID,
                         pstMeInfo->FmrId.aucMfrId,
                         ulDataLen))
    {
        /*��ӡδ�ɹ���ʼ��ֵ��ԭֵ����*/
        MMA_WARNINGLOG("TAF_MMA_ReadFmridNvim():Read FMRID Failed!");
    }


    return;
}



VOS_VOID TAF_MMA_ReadProductNameNvim(VOS_VOID)
{
    VOS_UINT32                          ulDataLen;
    TAF_PH_MEINFO_STRU                 *pstMeInfo = VOS_NULL_PTR;

    pstMeInfo = &(gstMmaValue.stMeInfo);
    ulDataLen = TAF_PH_PRODUCT_NAME_LEN + 1;

    /*��ȡ��Ʒ����*/
    if (NV_OK != NV_Read(en_NV_Item_ProductName,
                         pstMeInfo->ProductName.aucProductName,
                         ulDataLen))
    {
        /*��ӡδ�ɹ���ʼ��ֵ��ԭֵ����*/
       MMA_WARNINGLOG("TAF_MMA_ReadProductNameNvim():Read ProductName Failed!");
    }


    return;
}


VOS_VOID TAF_MMA_ReadAttEnsCustomNvim(VOS_VOID)
{
    NVIM_ATT_ENS_CTRL_STRU              stNvAttEns;


    PS_MEM_SET(&stNvAttEns, 0x00, sizeof(stNvAttEns));

    /* ��NV��en_NV_Item_Att_Ens_Ctrl_Paraʧ�� */
    if (NV_OK != NV_Read(en_NV_Item_Att_Ens_Ctrl_Para,
                         &stNvAttEns,
                         sizeof(NVIM_ATT_ENS_CTRL_STRU)))
    {
        MMA_ERRORLOG("TAF_MMA_ReadATTCustomNvim(): Read NV Fail");
        return;
    }

    /* ������ѡ��˵����Ʊ�ʶ */
    if (VOS_TRUE == stNvAttEns.ucNetSelMenuFlg)
    {
        TAF_MMA_SetNetworkSelectionMenuEnableFlg(VOS_TRUE);
    }

    /* �ý��뼼��ƽ����Ʊ�ʶ */
    if (VOS_TRUE == stNvAttEns.ucRatBalancingFlg)
    {
        TAF_MMA_SetRatBalancingEnableFlg(VOS_TRUE);
    }

    return;
}


#if (FEATURE_ON == FEATURE_LTE)
VOS_UINT32  TAF_MMA_ReadLastTinInfoNvim(
    NAS_NVIM_TIN_INFO_STRU             *pstTinInfo
)
{
    VOS_UINT32                          ulResult;
    VOS_UINT32                          ulLength;

    NV_GetLength(en_NV_Item_TIN_INFO, &ulLength);

    ulResult = NV_Read(en_NV_Item_TIN_INFO, pstTinInfo, sizeof(NAS_NVIM_TIN_INFO_STRU));

    if (NV_OK != ulResult)
    {
        return MMA_FAILURE;
    }

    return MMA_SUCCESS;
}

#endif


VOS_VOID TAF_MMA_ReadUeSupporttedCPHSFlagNvim(VOS_VOID)
{
    VOS_UINT32                          ulDataLen;
    NAS_MMC_NVIM_CPHS_SUPPORT_FLG_STRU  stSupportFlg;

    ulDataLen = 0;
    stSupportFlg.usSupportFlg = VOS_FALSE;

    ulDataLen = sizeof(stSupportFlg.usSupportFlg);

    if (NV_OK != NV_Read(en_NV_Item_SUPPORT_CPHS_FLAG,
                            (VOS_VOID*)(&stSupportFlg),ulDataLen))
    {
       stSupportFlg.usSupportFlg = NV_ITEM_DEACTIVE;
    }

    if (NV_ITEM_ACTIVE == stSupportFlg.usSupportFlg)
    {
        g_usMmaUeSupportCPHSFlg = NV_ITEM_ACTIVE;
    }
    else
    {
        g_usMmaUeSupportCPHSFlg = NV_ITEM_DEACTIVE;
    }

    return;
}


VOS_VOID TAF_MMA_ReadDeactUsimWhenPoweroffNvim(VOS_VOID)
{
    TAF_NVIM_DEACT_SIM_WHEN_POWEROFF_STRU                   stNvDeactSimWhenPoweroff;

    PS_MEM_SET(&stNvDeactSimWhenPoweroff, 0, sizeof(TAF_NVIM_DEACT_SIM_WHEN_POWEROFF_STRU));

    if (NV_OK != NV_Read(en_NV_Item_DEACT_USIM_WHEN_POWEROFF,
                         &stNvDeactSimWhenPoweroff,
                         sizeof(TAF_NVIM_DEACT_SIM_WHEN_POWEROFF_STRU)))
    {
        /* ��ȡNV��ʧ�ܣ�Ĭ�ϸ����Թر� */
        g_ucMmaDeactSimWhenPoweroff = MMA_DEACT_SIM_WHEN_POWEROFF_DISABLE;

        return;
    }

    /* NV��ȡ�ɹ�����ֵΪ1�������Դ򿪣�������Ϊ���Թر� */
    if (VOS_TRUE == stNvDeactSimWhenPoweroff.ucEnableFlag)
    {
        g_ucMmaDeactSimWhenPoweroff = MMA_DEACT_SIM_WHEN_POWEROFF_ENABLE;
    }
    else
    {
        g_ucMmaDeactSimWhenPoweroff = MMA_DEACT_SIM_WHEN_POWEROFF_DISABLE;
    }

    return;
}



VOS_UINT32 TAF_MMA_ReadAlphanumericNameByPlmnIdNvim( TAF_PH_OPERATOR_NAME_STRU   *pstOperName)
{
    VOS_UINT16                          usI;
    NAS_MMA_NVIM_OPERATOR_NAME_STRU     stRcvData;
    TAF_PH_OPERATOR_NAME_STRU           austOperName[TAF_PH_STORED_OPER_NAME_NUM_IN_NVIM];
    VOS_UINT16                          usDataLen;
    VOS_UINT32                          ulNvLen;


    PS_MEM_SET(&stRcvData,   0x00, sizeof(stRcvData));
    PS_MEM_SET(austOperName, 0x00, sizeof(austOperName));

    usDataLen = (TAF_PH_STORED_OPER_NAME_NUM_IN_NVIM*sizeof( TAF_PH_OPERATOR_NAME_STRU )) + 4;

    NV_GetLength(en_NV_Item_HUAWEI_NW_OPL_NAME_CUSTOMIZED, &ulNvLen);

    if ( ulNvLen < usDataLen)
    {
        return MMA_FAILURE;
    }

    if (NV_OK !=  NV_Read(en_NV_Item_HUAWEI_NW_OPL_NAME_CUSTOMIZED,(VOS_VOID *)(&stRcvData),  ulNvLen))
    {
        pstOperName->aucOperatorNameLong[0]  = '\0';
        pstOperName->aucOperatorNameShort[0] = '\0';
        MMA_WARNINGLOG("MMA_PhoneGetAlphanumericNameByPlmnIdFromNV():WARNING:Reading PLMN failed!");
        return MMA_FAILURE;
    }

    if( NV_ITEM_DEACTIVE == stRcvData.aucRcvData[0])
    {
        return MMA_FAILURE;
    }

    /*Compare the specified PLMN ID with that one in NVIM*/
    for ( usI = 0; usI < TAF_PH_STORED_OPER_NAME_NUM_IN_NVIM; usI++ )
    {
        MMA_MEM_CPY((VOS_VOID *)(&austOperName[usI]),
                    (VOS_VOID *)(stRcvData.aucRcvData + 4 +(usI*NV_ITEM_MMA_OPERATORNAME_SIZE)),
                    NV_ITEM_MMA_OPERATORNAME_SIZE);

        MMA_INFOLOG1("MMA_PhoneGetAlphanumericNameByPlmnId():austOperName[usI].Mcc =",
            (VOS_INT32)austOperName[usI].PlmnId.Mcc);
        MMA_INFOLOG1("MMA_PhoneGetAlphanumericNameByPlmnId():austOperName[usI].Mnc =",
            (VOS_INT32)austOperName[usI].PlmnId.Mnc);
        MMA_INFOLOG(austOperName[usI].aucOperatorNameLong);
        MMA_INFOLOG(austOperName[usI].aucOperatorNameShort);

        if( (austOperName[usI].PlmnId.Mcc == (pstOperName->PlmnId.Mcc & 0xfff))
          &&(austOperName[usI].PlmnId.Mnc == (pstOperName->PlmnId.Mnc & 0xfff))
            )
        {
            MMA_MEM_CPY(pstOperName->aucOperatorNameLong,
                austOperName[usI].aucOperatorNameLong, (TAF_PH_OPER_NAME_LONG ));
            MMA_MEM_CPY(pstOperName->aucOperatorNameShort,
                austOperName[usI].aucOperatorNameShort, (TAF_PH_OPER_NAME_SHORT));
            MMA_NORMAILLOG("MMA_PhoneGetAlphanumericNameByPlmnIdFromNV():the specified PLMN found in NVIM!");
            return MMA_SUCCESS;
        }
    }

    MMA_NORMAILLOG("MMA_PhoneGetAlphanumericNameByPlmnIdFromNV():the specified PLMN not found in NVIM!");
    pstOperName->aucOperatorNameLong[0]  = '\0';
    pstOperName->aucOperatorNameShort[0] = '\0';

    return MMA_FAILURE;
}
VOS_VOID  TAF_MMA_ReadPsStartNvim( VOS_VOID )
{
    NAS_MMA_NVIM_AUTO_START_STRU        stNvAutoStart;

    stNvAutoStart.ulAutoStart   = NAS_MMA_AUTOMATIC_START;

    if ( NV_OK != NV_Read(en_NV_Item_PS_START,
                          &stNvAutoStart,
                          sizeof(stNvAutoStart) ))
    {
        vos_printf("read auto start nv fail\r\n");
    }

    if ((stNvAutoStart.ulAutoStart) > NAS_MMA_MANUAL_START)
    {
        TAF_MMA_SetAutoInitPsFlg(VOS_TRUE);
        return;
    }

    if (NAS_MMA_MANUAL_START == stNvAutoStart.ulAutoStart)
    {
        TAF_MMA_SetAutoInitPsFlg(VOS_FALSE);
    }
    else
    {
        TAF_MMA_SetAutoInitPsFlg(VOS_TRUE);
    }

    return;
}
VOS_VOID  TAF_MMA_ReadCardLockStatusNvim( VOS_VOID )
{
    TAF_CUSTOM_CARDLOCK_STATUS_STRU     stCardLockStatus;
    VOS_UINT32                          ulResult;

    PS_MEM_SET(&stCardLockStatus, 0x00, sizeof(stCardLockStatus));

    ulResult = NV_Read(en_NV_Item_CardlockStatus,
                       &stCardLockStatus,
                       (VOS_UINT32)(sizeof(stCardLockStatus)));

    if (NV_OK != ulResult)
    {
        MN_WARN_LOG("TAF_MMA_ReadCardLockStatusNvim:WARNING:  read sim lock status fail");
    }

    g_MmaNewValue.stCardLockStatus = stCardLockStatus;

    return;
}


VOS_VOID  TAF_MMA_ReadSimLockMaxTimesNvim( VOS_VOID )
{
    TAF_CUSTOM_SIM_LOCK_MAX_TIMES_STRU  stSimLockMaxTimes;
    VOS_UINT32                          ulResult;
    VOS_UINT32                          ulLength;

    PS_MEM_SET(&stSimLockMaxTimes, 0x00, sizeof(stSimLockMaxTimes));
    ulLength = 0;

    /* �Ȼ�ȡNV�ĳ��� */
    NV_GetLength(en_NV_Item_CustomizeSimLockMaxTimes, &ulLength);

    if (ulLength > sizeof(TAF_CUSTOM_SIM_LOCK_MAX_TIMES_STRU))
    {
        NAS_ERROR_LOG(WUEPS_PID_MMA,"TAF_MMA_ReadNvimCustomizeSimLockMaxTimes():WARNING: en_NV_Item_CustomizeSimLockMaxTimes length Error");
        g_MmaNewValue.stSimLockMaxTimes.ulStatus = VOS_FALSE;
        g_MmaNewValue.stSimLockMaxTimes.ulLockMaxTimes = TAF_PH_CARDLOCK_DEFAULT_MAXTIME;
        return;
    }

    ulResult = NV_Read(en_NV_Item_CustomizeSimLockMaxTimes,
                               &stSimLockMaxTimes,
                               sizeof(stSimLockMaxTimes));

    if (NV_OK != ulResult)
    {
        g_MmaNewValue.stSimLockMaxTimes.ulStatus        = VOS_FALSE;
        g_MmaNewValue.stSimLockMaxTimes.ulLockMaxTimes  = TAF_PH_CARDLOCK_DEFAULT_MAXTIME;
    }
    else
    {
        if (VOS_FALSE == stSimLockMaxTimes.ulStatus)
        {
            g_MmaNewValue.stSimLockMaxTimes.ulLockMaxTimes = TAF_PH_CARDLOCK_DEFAULT_MAXTIME;
        }
        else
        {
            g_MmaNewValue.stSimLockMaxTimes = stSimLockMaxTimes;
        }
    }

    return;
}
VOS_VOID  TAF_MMA_ReadSimLockPlmnNvim( VOS_VOID )
{
    TAF_CUSTOM_SIM_LOCK_PLMN_INFO_STRU          stSimLockPlmnInfo;
    VOS_UINT32                                  ulResult;

    PS_MEM_SET(&stSimLockPlmnInfo, 0x00, sizeof(stSimLockPlmnInfo));

    ulResult = NV_Read(en_NV_Item_CustomizeSimLockPlmnInfo,
                       &stSimLockPlmnInfo,
                       sizeof(stSimLockPlmnInfo));
    if (NV_OK != ulResult)
    {
        g_MmaNewValue.stSimLockPlmnInfo.ulStatus = VOS_FALSE;
        PS_MEM_SET(g_MmaNewValue.stSimLockPlmnInfo.astSimLockPlmnRange, (VOS_CHAR)0xaa,
                       sizeof(g_MmaNewValue.stSimLockPlmnInfo.astSimLockPlmnRange));
    }
    else
    {
        g_MmaNewValue.stSimLockPlmnInfo = stSimLockPlmnInfo;
    }

    return;
}


VOS_UINT32  TAF_MMA_ReadLastImsiNvim(
    NAS_MMC_NVIM_LAST_IMSI_STRU        *pstLastImsi
)
{
    VOS_UINT32                          ulResult;
    VOS_UINT32                          ulLength;

    NV_GetLength(en_NV_Item_Last_Imsi, &ulLength);

    ulResult = NV_Read(en_NV_Item_Last_Imsi, pstLastImsi, ulLength);

    if (NV_OK != ulResult)
    {
        return MMA_FAILURE;
    }

    return MMA_SUCCESS;
}


VOS_VOID TAF_MMA_ReadSimPersonalisationPwdNvim( VOS_VOID )
{
    VOS_UINT8                           aucPwd[TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX +1];
    VOS_UINT32                          ulResult;
    VOS_UINT32                          ulLength;

    PS_MEM_SET(aucPwd, 0x00, sizeof(aucPwd));
    NV_GetLength(en_NV_Item_Sim_Personalisation_Pwd, &ulLength);

    ulResult = NV_Read(en_NV_Item_Sim_Personalisation_Pwd,
                     aucPwd,
                     ulLength);

    if (NV_OK != ulResult)
    {
        return;
    }

    PS_MEM_CPY(g_MmaNewValue.aucPwd, aucPwd, TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX);

    return;
}


VOS_VOID TAF_MMA_ReadWaitImsVoiceCapTimerLenNvim(VOS_VOID)
{
    TAF_NVIM_WAIT_IMS_VOICE_AVAIL_TIMER_LEN_STRU            stImsVoiceAvailTimerLen;
    VOS_UINT32                                              ulLength;

    PS_MEM_SET(&stImsVoiceAvailTimerLen, 0x00, sizeof(stImsVoiceAvailTimerLen));
    ulLength = 0;

    /* �Ȼ�ȡNV�ĳ��� */
    NV_GetLength(en_NV_Item_WAIT_IMS_VOICE_AVAIL_Timer_Len, &ulLength);

    if (ulLength > sizeof(TAF_NVIM_WAIT_IMS_VOICE_AVAIL_TIMER_LEN_STRU))
    {
        MN_WARN_LOG("TAF_MMA_ReadWaitImsVoiceCapTimerLenNvim(): en_NV_Item_WAIT_IMS_VOICE_AVAIL_Timer_Len length Error");
        return;
    }

    /* ��NVʧ�� */
    if (NV_OK != NV_Read(en_NV_Item_WAIT_IMS_VOICE_AVAIL_Timer_Len,
                         &stImsVoiceAvailTimerLen, ulLength))
    {
        MN_WARN_LOG("TAF_MMA_ReadWaitImsVoiceCapTimerLenNvim(): en_NV_Item_WAIT_IMS_VOICE_AVAIL_Timer_Len error");
        return;
    }

    if (VOS_TRUE == stImsVoiceAvailTimerLen.ucNvimActiveFlag)
    {
        /* NV�ĳ���Ϊ0ʹ��Ĭ��ʱ��,NV�ĵ�λΪS,��Ҫת��Ϊ���� */
        if (0 == stImsVoiceAvailTimerLen.ucWaitImsVoiceAvailTimerLen)
        {
            TAF_SDC_SetWaitImsVoiceAvailTimerLen(TI_TAF_SDC_WAIT_IMSA_IMS_VOICE_AVAIL_IND_LEN);
        }
        else
        {
            TAF_SDC_SetWaitImsVoiceAvailTimerLen(stImsVoiceAvailTimerLen.ucWaitImsVoiceAvailTimerLen * TAF_SDC_ONE_THOUSAND_MILLISECOND);
        }
    }

    return;
}
VOS_VOID TAF_MMA_ReadLcCtrlNvim( VOS_VOID )
{
    TAF_SDC_LC_CONFIG_PARA_STRU        *pstLcConfigInfo = VOS_NULL_PTR;
    TAF_NV_LC_CTRL_PARA_STRU            stLcCtrlInfo;
    VOS_UINT32                          ulResult;
    VOS_UINT32                          ulLength;

    pstLcConfigInfo = TAF_SDC_GetLcConfigPara();

    PS_MEM_SET(&stLcCtrlInfo, 0x00, sizeof(TAF_NV_LC_CTRL_PARA_STRU));

    NV_GetLength(en_NV_Item_LC_Ctrl_PARA, &ulLength);

    ulResult = NV_Read(en_NV_Item_LC_Ctrl_PARA,
                       &stLcCtrlInfo,
                       ulLength);

    if (NV_OK != ulResult)
    {
        return;
    }

    /* NV �����쳣���,��������쳣�����޸ĵ�ǰLCֵ */
    if ( (VOS_TRUE                      != stLcCtrlInfo.ucLCEnableFlg)
      && (VOS_FALSE                     != stLcCtrlInfo.ucLCEnableFlg) )
    {
        return;
    }

    if ( (TAF_NVIM_LC_RAT_COMBINED_GUL  != stLcCtrlInfo.enRatCombined)
      && (TAF_NVIM_LC_RAT_COMBINED_CL   != stLcCtrlInfo.enRatCombined)
      && (TAF_NVIM_LC_RAT_COMBINED_BUTT != stLcCtrlInfo.enRatCombined))
    {
        return;
    }

    if ( (TAF_NVIM_LC_INDEPENT_WORK     != stLcCtrlInfo.enLCWorkCfg)
      && (TAF_NVIM_LC_INTER_WORK        != stLcCtrlInfo.enLCWorkCfg)
      && (TAF_NVIM_LC_WORK_CFG_BUTT     != stLcCtrlInfo.enLCWorkCfg) )
    {
        return;
    }

    pstLcConfigInfo->ucLCEnableFlg = stLcCtrlInfo.ucLCEnableFlg;
    pstLcConfigInfo->enRatCombined = stLcCtrlInfo.enRatCombined;
    pstLcConfigInfo->enLCWorkCfg   = stLcCtrlInfo.enLCWorkCfg;

    return;
}



VOS_VOID TAF_MMA_WritePlmnSelectionModeNvim(VOS_UINT8 ucReselMode)
{
    NAS_NVIM_SELPLMN_MODE_STRU          stPlmnSelMode;
    VOS_UINT32                          ulNvLength;

    ulNvLength                  = 0;
    stPlmnSelMode.usSelPlmnMode = ucReselMode;

    NV_GetLength(en_NV_Item_SelPlmn_Mode, &ulNvLength);

    if (NV_OK != NV_Write(en_NV_Item_SelPlmn_Mode,
                      &stPlmnSelMode,
                      ulNvLength))
    {
        MN_WARN_LOG("TAF_MMA_WritePlmnSelectionModeNvim:WARNING:  read en_NV_Item_SelPlmn_Mode fail");
    }

    return;
}
VOS_VOID TAF_MMA_UpdateUeBandNvim()
{
    TAF_USER_SET_PREF_BAND64            stOrigUserSetGuBand;
    TAF_PH_PREF_BAND                    ulBand;
    MMA_USER_BAND_SET_UN                uUserSetBand;
    MMA_UE_SUPPORT_FREQ_BAND_STRU       stSetBand;
    MMA_UE_SUPPORT_FREQ_BAND_STRU       stUeOrigSupportFreqBand;
    MMA_USER_BAND_SET_UN                uUeOrigSupportFreqBand;
    MMA_USER_BAND_SET_UN                uValidSetBand;

    PS_MEM_SET(&stOrigUserSetGuBand, 0x00, sizeof(stOrigUserSetGuBand));
    PS_MEM_SET(&ulBand, 0x00, sizeof(ulBand));
    PS_MEM_SET(&uUserSetBand, 0x00, sizeof(uUserSetBand));
    PS_MEM_SET(&stSetBand, 0x00, sizeof(stSetBand));
    PS_MEM_SET(&stUeOrigSupportFreqBand, 0x00, sizeof(stUeOrigSupportFreqBand));
    PS_MEM_SET(&uUeOrigSupportFreqBand, 0x00, sizeof(uUeOrigSupportFreqBand));
    PS_MEM_SET(&uValidSetBand, 0x00, sizeof(uValidSetBand));

    /*��ForbBandNV����,����Ƶ�μ��� ����NV��*/
    if (NV_ITEM_ACTIVE == gstMmaForbBand.ucActiveFlag)
    {
        if (VOS_FALSE == gstMmaForbBand.ucForbStatus)
        {
            /*���ݽ�ֹƵ�ε���Ϣ����FreqBandNV��UsersetBandNV*/
            MMA_UpdateBandNvRefForbBand();

            /*���½�ֹƵ�ε�StatusΪTRUE 1*/
            MMA_UpdateForbBandStatusToNV();
        }
    }

    /* ��NV8265��stOrigUserSetGuBand�� */
    TAF_MMA_ReadUserSettedBandNvim();

    MN_MMA_Convert64BitBandTo32Bit(&gstMmaValue.stLastSyscfgSet.stUserSetBand.stOrigUserSetGuBand, &ulBand);

    MN_MMA_ConvertGUFrequencyBand(&ulBand);
    uUserSetBand.ulPrefBand = ulBand;

    /* ��NV8229��stUeOrigSupportFreqBand�� */
    TAF_MMA_ReadFreqBandNvim();

    /*����ǰϵͳ���õ�Ƶ��ת��Ϊ�û����õĸ�ʽ*/
    MMA_SwitchUESupportBand2UserSetBand(gstMmaValue.stLastSyscfgSet.stUserSetBand.uUserSetUeFormatGuBand, &uUeOrigSupportFreqBand);

    /* ����û�����Ƶ�Ρ���ƵƵ�ν�����NV 8229�Ľ�����NV 8229ֵ��ͬ����ֱ�ӷ��ز���ҪдNV */
    uValidSetBand.ulPrefBand = uUserSetBand.ulPrefBand & uUeOrigSupportFreqBand.ulPrefBand;
    if (uValidSetBand.ulPrefBand != uUeOrigSupportFreqBand.ulPrefBand)
    {
        MMA_SwitchUserSetBand2UESupportBand(uValidSetBand, &stSetBand);

        /* дNV:en_NV_Item_Support_Freqbands,����ȫ�ֱ��� */
        TAF_MMA_UpdateSupportFreqBand(&stSetBand);
    }

    return;
}



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif



