

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "xa_dts_pp.h"
#include "ucom_mem_dyn.h"
#include "om_log.h"



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*lint -e(767)*/
#define THIS_FILE_ID                    OM_FILE_ID_CODEC_XA_DTS_PP_C
/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/* DTS�ڲ�ά����Ա����:private */
XA_DTS_CURRENT_CFG_STRU             g_stDtsCurtCfg;

/* DTS�������Ʋ���ֻ�������ӿڸı��Աֵ
 * DTSģ�鲻�������ı䣬uninit��HIFI��̬���ز�����ʼ�� */
UCOM_SET_UNINIT
XA_DTS_SET_PARA_STRU                g_stDtsUintPara;

/* Max HPF filter order and number of TruEQ bands */
static SRSTrMediaHDCreateData       g_stCreateData = {{6, 4, 4}};

/* DTS��ǰ���ò���
 * ���ҽ���ʹ������豸ģʽ�����������ı䵱ǰ���õĲ��� */
SRSTrMediaHDControls                g_stDtsCurrentPara;

VOS_INT16                           g_ashwDefaultGeqBandGains[XA_DTS_GEQ_BAND_NUM]={4096,4096,4096,4096,4096,4096,4096,4096,4096,4096};

/*****************************************************************************
  4 ����ʵ��
*****************************************************************************/


VOS_UINT32 XA_DTS_Init( XA_DTS_USER_CFG_STRU * pstUserCfg )
{
    XA_DTS_SET_PARA_STRU                *pstDtsSetPara = XA_DTS_GetUinitDDRParaPtr();
    VOS_INT32                            swRet;

    /* ���֮ǰ��������Ϣ */
    XA_DTS_DeleObj();

    /* ����DTS���� */
    swRet = XA_DTS_CreatObj(pstUserCfg);

    /* �������ʧ��������DTS�����ϱ�OM��Ϣ */
    if (VOS_OK != swRet)
    {
        XA_DTS_DeleObj();

        OM_LogError1(XA_DTS_Init_Fail, VOS_ERR);

        return VOS_ERR;
    }

    /* �ٴε���XA_DTS_Init˵�������л����豣����һ�׵�DTS����
     * �����Ƿ�ʹ�ܡ�����豸������
     * ���ҽ��д˴�����ʹ�� */
    pstDtsSetPara->enIsNeedSetPara = XA_DTS_NEED_SET_PARA_YES;

    OM_LogInfo1(XA_DTS_Init_OK, pstDtsSetPara->enIsNeedSetPara);

    return VOS_OK;
}




VOS_INT32  XA_DTS_CreatObj(XA_DTS_USER_CFG_STRU * pstUserCfg)
{
    XA_DTS_CURRENT_CFG_STRU       *pstDtsCurtCfg      = XA_DTS_GetCurrentCfgPtr();
    VOS_UINT32                     uwBlockSize, uwFilterCfg, uwSampleRate;
    VOS_CHAR                      *pDtsObjBuff;
    VOS_INT32                     *pswLeftBuff, *pswRightBuff;
    VOS_VOID                      *pvWorkSpaceBuff;
    SRSTrMediaHDObj                stTmhdObj;
    VOS_INT32                      swRet = VOS_OK;

    /* �쳣��μ�� */
    if ( VOS_NULL == pstUserCfg)
    {
        /* �ϱ�OM��Ϣ */
        OM_LogError1(XA_DTS_InvalidPara, VOS_ERR);
        return VOS_ERR;
    }

    /* ��յ�ǰ������Ϣȫ�ֱ��� */
    UCOM_MemSet(pstDtsCurtCfg, 0, sizeof(XA_DTS_CURRENT_CFG_STRU));

    /* ��ȡ����¼BlockSize���˲������á������ʡ������� */
    uwBlockSize                 = pstUserCfg->uwBlockSize;
    uwFilterCfg                 = pstUserCfg->uwFilterCfg;
    uwSampleRate                = pstUserCfg->uwSampleRate;
    uwSampleRate                = uwSampleRate/1000;

    pstDtsCurtCfg->uwBlockSize  = uwBlockSize;
    pstDtsCurtCfg->uwFilterCfg  = uwFilterCfg;
    pstDtsCurtCfg->uwSampleRate = uwSampleRate;
    pstDtsCurtCfg->swChannelNum = pstUserCfg->swChannelNum;

    /* ��ʼ��FFT Lib��ռ� */
    XA_DTS_InitFFT_Lib();

    /* ����DTSObj�ڴ�ռ� */
    /*lint -e655*/
    pDtsObjBuff     = (VOS_CHAR *)UCOM_MemAllocFair(SRS_TrMediaHD_GetObjSize(&g_stCreateData));
    pstDtsCurtCfg->pDtsObjBuff  = pDtsObjBuff;
    pstDtsCurtCfg->uwDtsObjSize = (VOS_UINT32)SRS_TrMediaHD_GetObjSize(&g_stCreateData);

    /* �������������ڴ�ռ� */
    /*lint -e655*/
    pswLeftBuff         = (VOS_INT32 *)UCOM_MemAllocFair((uwBlockSize * XA_DTS_CHANNEL_NUM) * sizeof(VOS_INT32));
    pswRightBuff        = pswLeftBuff + uwBlockSize;
    pstDtsCurtCfg->pswLeftBuff      = pswLeftBuff;
    pstDtsCurtCfg->pswRightBuff     = pswRightBuff;
    pstDtsCurtCfg->uwLeftSize       = (uwBlockSize * 1) * sizeof(VOS_INT32);
    pstDtsCurtCfg->uwRightSize      = (uwBlockSize * 1) * sizeof(VOS_INT32);

    /* ����workspace�ڴ�ռ䣬scratch buffer */
    /*lint -e961*/
    /*lint -e506*/
    /*lint -e655*/
    pvWorkSpaceBuff      = UCOM_MemAllocFair(SRS_TRMEDIAHD_WORKSPACE_SIZE(uwBlockSize));
    pstDtsCurtCfg->pvWorkSpaceBuff  = pvWorkSpaceBuff;
    pstDtsCurtCfg->uwWorkSpaceSize  = SRS_TRMEDIAHD_WORKSPACE_SIZE(uwBlockSize);
    /*lint +e961*/
    /*lint +e506*/
    /*lint +e655*/

    swRet += SRS_TrMediaHD_CreateObj(&stTmhdObj, pDtsObjBuff, 0, 0, &g_stCreateData);

    /* ��¼DTS���� */
    pstDtsCurtCfg->stDtsObj         = stTmhdObj;

    /* ��ʼ��DTSObj */
    XA_DTS_InitObj(pstDtsCurtCfg->stDtsObj, (VOS_UINT16)uwFilterCfg, uwSampleRate);

    /*********************** ����Ĭ�ϲ��� ********************** */
    /* Ĭ������豸Ϊspeaker������speaker���� */
    swRet += (VOS_INT32)XA_DTS_SetDev(XA_DTS_DEV_SPEAKRE);

    /* Ĭ��DTS���� */
    swRet += (VOS_INT32)XA_DTS_SetEnable(XA_DTS_ENABLE_NO);

    swRet += XA_DTS_SetGEQBandGains(pstDtsCurtCfg->stDtsObj, g_ashwDefaultGeqBandGains);

    return swRet;

}
VOS_VOID XA_DTS_InitObj(SRSTrMediaHDObj stTmhdObj, XA_DTS_FILTER_CFG_ENUM_UINT16 uhwFset, VOS_UINT32 uwSampleRate)
{
    SRSTrMediaHDFilterConfig     stCfg;
    SRSTrMediaHDFilterConfig    *pstCfg;

    /*��ȡAP_NV�����ȡʧ����ʹ��Ĭ�ϲ���*/
    UCOM_NV_Read(XA_DTS_NV_ID_Speaker, &DTS_speaker, sizeof(SRSTrMediaHDControls));
    UCOM_NV_Read(XA_DTS_NV_ID_HeadPhone, &DTS_headphone, sizeof(SRSTrMediaHDControls));
    UCOM_NV_Read(XA_DTS_NV_ID_HpfOrder, &HPF_Order, sizeof(SRSTrMediaHDHpfOrder));
    UCOM_NV_Read(XA_DTS_NV_ID_UntitledHpf1, &Untitled_HPF1[0], sizeof(SRSInt16)*XA_DTS_HPF_PARA_LEN);
    UCOM_NV_Read(XA_DTS_NV_ID_UntitledHpf2, &Untitled_HPF2[0], sizeof(SRSInt16)*XA_DTS_HPF_PARA_LEN);
    UCOM_NV_Read(XA_DTS_NV_ID_UntitledPeq1, &Untitled_PEQ1[0], sizeof(SRSInt32)*XA_DTS_PEQ_PARA_LEN);
    UCOM_NV_Read(XA_DTS_NV_ID_UntitledPeq2, &Untitled_PEQ2[0], sizeof(SRSInt32)*XA_DTS_PEQ_PARA_LEN);
    UCOM_NV_Read(XA_DTS_NV_ID_UntitledTbhd1, &Untitled_TBHD1[0], sizeof(SRSInt32)*XA_DTS_TBHD_PARA_LEN);
    UCOM_NV_Read(XA_DTS_NV_ID_UntitledTbhd2, &Untitled_TBHD2[0], sizeof(SRSInt32)*XA_DTS_TBHD_PARA_LEN);

    /* Ĭ��ʹ��speaker */
    stCfg  = GetFilterNonflatPeqCfg((VOS_INT32)uwSampleRate, XA_DTS_DEV_SPEAKRE);
    pstCfg = &stCfg;

    /* ���ݲ�ͬ�����ʳ�ʼ����ͬ���� */
    switch(uwSampleRate)
    {
        case 32:
            SRS_TrMediaHD_InitObj32k(stTmhdObj, pstCfg); break;
        case 44:
            SRS_TrMediaHD_InitObj44k(stTmhdObj, pstCfg); break;
        case 48:
            SRS_TrMediaHD_InitObj48k(stTmhdObj, pstCfg); break;
        default:
        {
            /* for_pclint */
        }
    }

}


VOS_INT32  XA_DTS_ExeProcess(XA_DTS_USER_CFG_STRU * pstUserCfg)
{
    XA_DTS_CURRENT_CFG_STRU                     *pstDtsCurtCfg   = XA_DTS_GetCurrentCfgPtr();
    XA_DTS_SET_PARA_STRU                        *pstUinitDDRPara = XA_DTS_GetUinitDDRParaPtr();
    SRSTrMediaHDObj                              stDtsObj;
    SRSStereoCh                                  stAudioIO;
    VOS_VOID                                    *pvWorkSpace;
    VOS_INT32                                    swBlokSize;
    VOS_INT32                                    swRet = VOS_OK;

    /* �����Ҫ���ò���������������unit DDR �����DTS����
     * 1�������л�ʱ��Ҫ��������
     * 2������ֻ����Init֮���������ʹ�ܡ��豸������������ִ�к��������ö�������Ϣ��Ӧ���������� */
    if ( XA_DTS_NEED_SET_PARA_YES == pstUinitDDRPara->enIsNeedSetPara )
    {
        /* �ϱ�OM��Ϣ */
        OM_LogInfo2(XA_DTS_SetParaWhileExe, pstUinitDDRPara->enIsEnable, pstUinitDDRPara->enCurrentDev);

        /* �����Ƿ�ʹ�� */
        swRet += (VOS_INT32)XA_DTS_SetEnable(pstUinitDDRPara->enIsEnable);

        /* ��������豸ģʽ */
        swRet += (VOS_INT32)XA_DTS_SetDev(pstUinitDDRPara->enCurrentDev);

        if (VOS_OK != swRet)
        {
            XA_DTS_DeleObj();

            /* �ϱ�OM��Ϣ */
            OM_LogError1(XA_DTS_SetParaWhileExeFail, VOS_ERR);
            return VOS_ERR;
        }

        /* �ָ���־λ */
        pstUinitDDRPara->enIsNeedSetPara  = XA_DTS_NEED_SET_PARA_NO;

    }

    /* ���������ݽ���DTS��Ч���� */
    pstDtsCurtCfg       = XA_DTS_GetCurrentCfgPtr();
    stDtsObj            = pstDtsCurtCfg->stDtsObj;
    stAudioIO.Left      = (SRSInt32 *)pstDtsCurtCfg->pswLeftBuff;
    stAudioIO.Right     = (SRSInt32 *)pstDtsCurtCfg->pswRightBuff;
    pvWorkSpace         = pstDtsCurtCfg->pvWorkSpaceBuff;
    swBlokSize          = (VOS_INT32)pstUserCfg->uwBlockSize;

    /* ��ȡ�������� */
    XA_DTS_Hybrid2Stereo(pstUserCfg->pshwInputBuff, (VOS_INT32 *)stAudioIO.Left, (VOS_INT32 *)stAudioIO.Right, swBlokSize);

    swRet += SRS_TrMediaHD_Process(stDtsObj, &stAudioIO, swBlokSize, pvWorkSpace);

    /* ����ʧ�ܣ��򷵻ز�dele DTS����  */
    if ( SRS_STAT_NO_ERROR != swRet )
    {
        XA_DTS_DeleObj();

        /* �ϱ�OM��Ϣ */
        OM_LogError1(CODEC_TENSILICA_ERR, VOS_ERR);
        return VOS_ERR;
    }

    /* ������������ */
    XA_DTS_Stereo2Hybrid((VOS_INT32 *)stAudioIO.Left, (VOS_INT32 *)stAudioIO.Right, swBlokSize, (VOS_INT16 *)pstUserCfg->pshwOutputBuff);

    return swRet;

}



VOS_UINT32 XA_DTS_DeleObj( VOS_VOID )
{
    XA_DTS_CURRENT_CFG_STRU     *pstDtsCurtCfg      = XA_DTS_GetCurrentCfgPtr();
    SRSTrMediaHDControls        *pstCurrentPara     = XA_DTS_GetCurrentParaPtr();

    /* ��������ڴ�ռ��ͷ� */
    UCOM_MemFree(pstDtsCurtCfg->pDtsObjBuff);
    UCOM_MemFree(pstDtsCurtCfg->pswLeftBuff);
    UCOM_MemFree(pstDtsCurtCfg->pswRightBuff);
    UCOM_MemFree(pstDtsCurtCfg->pvWorkSpaceBuff);
    UCOM_MemFree(pstDtsCurtCfg->pvFFTLibBuff);

    /* ����Ա������Ϊdefault */
    UCOM_MemSet(pstDtsCurtCfg, 0, sizeof(XA_DTS_CURRENT_CFG_STRU));
    UCOM_MemSet(pstCurrentPara, 0, sizeof(SRSTrMediaHDControls));

    return VOS_OK;
}


VOS_UINT32 XA_DTS_MsgSetDtsEnableCmd (VOS_VOID *pvOsaMsg)
{
    XA_DTS_SET_PARA_STRU                   *pstUinitDDRPara = XA_DTS_GetUinitDDRParaPtr();
    AUDIO_PLAYER_DTS_SET_ENABLE_CMD_STRU   *pstSetParaReq;
    XA_DTS_ENABLE_ENUM_UINT16               enEnableFlag;

    /* �쳣��μ�� */
    if ( VOS_NULL == pvOsaMsg )
    {
        /* �ϱ�OM��Ϣ */
        OM_LogError1(XA_DTS_InvalidPara, VOS_ERR);
        return VOS_ERR;
    }

    /* ����DTS��Ч�����ṹ����� */
    pstSetParaReq   = (AUDIO_PLAYER_DTS_SET_ENABLE_CMD_STRU *)pvOsaMsg;

    /* ȡ��DTS�Ƿ�ʹ��������Ϣ */
    enEnableFlag    = pstSetParaReq->uhwEnableFlag;

    /* ���Ƿ�ʹ��DTS�����˱仯ʱ�����Ƿ�ʹ�� */
    if (enEnableFlag != pstUinitDDRPara->enIsEnable)
    {
        if ( (XA_DTS_ENABLE_NO != enEnableFlag) && (XA_DTS_ENABLE_YES != enEnableFlag) )
        {
            /* �澯 */
            OM_LogWarning1(XA_DTS_SetEableInvalidPara, enEnableFlag);

            enEnableFlag = XA_DTS_ENABLE_NO;
        }
        else
        {
            /* ��uint����¼�Ƿ�ʹ�� */
            pstUinitDDRPara->enIsEnable       = enEnableFlag;
        }

        /* ����ʹ��ֻ����Init֮�󣬹��ڴ�ֻ����ǣ���exe�����ò��� */
        pstUinitDDRPara->enIsNeedSetPara = XA_DTS_NEED_SET_PARA_YES;

        /* �ϱ�om��Ϣ */
        OM_LogInfo2(XA_DTS_SetDevOK, pstUinitDDRPara->enIsNeedSetPara, pstUinitDDRPara->enIsEnable);

    }

    return VOS_OK;

}
VOS_UINT32  XA_DTS_MsgSetDtsDevCmd( VOS_VOID *pvOsaMsg )
{
    XA_DTS_SET_PARA_STRU                *pstDtsUinitPara    = XA_DTS_GetUinitDDRParaPtr();
    AUDIO_EFFECT_DTS_SET_DEV_REQ_STRU   *pstDtsSetDev;
    XA_DTS_CURRENT_DEV_ENUM_UINT32       enDtsDev;

    /* �쳣��μ�� */
    if ( VOS_NULL == pvOsaMsg )
    {
        /* �ϱ�OM��Ϣ */
        OM_LogError1(XA_DTS_InvalidPara, VOS_ERR);
        return VOS_ERR;
    }

    /* ��������DTS��Ч����豸�ṹ����� */
    pstDtsSetDev   = (AUDIO_EFFECT_DTS_SET_DEV_REQ_STRU *)((MsgBlock *) pvOsaMsg)->aucValue;

    /* ȡ��DTS����豸������Ϣ */
    enDtsDev      = pstDtsSetDev->uhwDevMode;

    /* ���AP���͵��豸ģʽ�б任����������� */
    if (enDtsDev != pstDtsUinitPara->enCurrentDev)
    {
        if ( (XA_DTS_DEV_HEADSET != enDtsDev) && (XA_DTS_DEV_SPEAKRE != enDtsDev) )
        {
            /* �澯 */
            OM_LogWarning1(XA_DTS_SetDevInvalidPara, enDtsDev);

            /* δ֪�豸����Ϊspeaker */
            pstDtsUinitPara->enCurrentDev = XA_DTS_DEV_SPEAKRE;
        }
        else
        {
            /* ��Uint DDR��¼��ǰʹ�õ�����豸 */
            pstDtsUinitPara->enCurrentDev = enDtsDev;
        }

        /* �����豸ֻ����Init֮�󣬹��ڴ�ֻ����ǣ���exe�����ò��� */
        pstDtsUinitPara->enIsNeedSetPara = XA_DTS_NEED_SET_PARA_YES;

        /* �ϱ�om��Ϣ */
        OM_LogInfo2(XA_DTS_SetDevOK, pstDtsUinitPara->enIsNeedSetPara, pstDtsUinitPara->enCurrentDev);

    }

    return VOS_OK;

}


VOS_INT32 XA_DTS_SetGEQBandGains(SRSTrMediaHDObj stDtsObj, VOS_INT16 * pshwGeqGain)
{
    VOS_INT32                                   swCnt;
    VOS_INT32                                   swRet = VOS_OK;

    for ( swCnt = 0; swCnt < XA_DTS_GEQ_BAND_NUM; swCnt++)
    {
        swRet += SRS_TrMediaHD_SetGEQBandGain(stDtsObj, swCnt, pshwGeqGain[swCnt]);
    }

    return swRet;
}


VOS_INT32 XA_DTS_Hybrid2Stereo(
                VOS_INT16 *pshwI2S,
                VOS_INT32 *pswLeft,
                VOS_INT32 *pswRight,
                VOS_INT32  swBlockSize)
{
    XA_DTS_CURRENT_CFG_STRU                     *pstDtsCurtCfg  = XA_DTS_GetCurrentCfgPtr();
    VOS_INT32                                    swChannelNum;
    VOS_INT32                                    swCnt         = 0;

    /* ��ȡ��ǰ���������� */
    swChannelNum = pstDtsCurtCfg->swChannelNum;

    /* С�����ݴ���������16bit�������ڷֿ������������32bit�ĸ�16bit */
    for (swCnt = 0; swCnt < ((swBlockSize * swChannelNum)/2); swCnt++)
    {
        pswLeft[swCnt]     = pshwI2S[swCnt * 2]<<16;
        pswRight[swCnt]    = pshwI2S[(swCnt * 2) + 1]<<16;
    }

    return swCnt;
}

VOS_UINT32 XA_DTS_Stereo2Hybrid(
                VOS_INT32  *pswLeft,
                VOS_INT32  *pswRight,
                VOS_INT32   swBlockSize,
                VOS_INT16  *shwHybrid)
{
    VOS_INT32                  swCnt;

    for ( swCnt = 0; swCnt < swBlockSize; swCnt++ )
    {
        shwHybrid[swCnt * 2]            = (VOS_INT16)(pswLeft[swCnt]>>16);
        shwHybrid[(swCnt * 2) + 1]      = (VOS_INT16)(pswRight[swCnt]>>16);
    }

    return (VOS_UINT32)swCnt;
}


VOS_VOID  XA_DTS_InitFFT_Lib(VOS_VOID)
{
    XA_DTS_CURRENT_CFG_STRU     *pstDtsCurtCfg      = XA_DTS_GetCurrentCfgPtr();
    VOS_VOID                    *pFftLibBuf;
    VOS_INT32                    swPlanBuffSize;

    /* ��ȡ��С������FFTllib��buff */
    swPlanBuffSize  = SRS_Fft_GetPlanBufSize(SRS_WOWHDX_BLK_SZ*2);

    /*lint -e655*/
    pFftLibBuf = UCOM_MemAllocFair(swPlanBuffSize);

    /* ��¼FFTllib��buff��С */
    pstDtsCurtCfg->pvFFTLibBuff = pFftLibBuf;

    SRS_Fft_CreatePlan(pFftLibBuf, SRS_WOWHDX_BLK_SZ*2, SRS_RFFT_32C16);

}
VOS_UINT32  XA_DTS_SetDev( XA_DTS_CURRENT_DEV_ENUM_UINT32 enDevice )
{
    SRSTrMediaHDControls        *pstCurrentPara     = XA_DTS_GetCurrentParaPtr();
    XA_DTS_CURRENT_CFG_STRU     *pstDtsCurtCfg      = XA_DTS_GetCurrentCfgPtr();
    XA_DTS_SET_PARA_STRU        *pstDtsUinitPara    = XA_DTS_GetUinitDDRParaPtr();
    SRSTrMediaHDObj              stDtsObj           = pstDtsCurtCfg->stDtsObj;
    SRSTrMediaHDFilterConfig    *pstCfg             = 0;
    SRSTrMediaHDFilterConfig     stCfg;
    VOS_INT32                    swRet              = VOS_OK;

    /* �쳣��μ�� */
    if (XA_DTS_DEV_BUTT <= enDevice)
    {
        OM_LogError1(XA_DTS_SetDevError, enDevice);

        return VOS_ERR;
    }

    /* ����ģʽ */
    if (XA_DTS_DEV_HEADSET == enDevice)
    {
        /* ���ö���ģʽ�Ĳ��� */
        UCOM_MemCpy(pstCurrentPara, &DTS_headphone, sizeof(SRSTrMediaHDControls));

        /* ��ȡ�˲������� */
        stCfg  = GetFilterNonflatPeqCfg((VOS_INT32)pstDtsCurtCfg->uwSampleRate, XA_DTS_DEV_HEADSET);

        /* ����ʵʱ�ӿ�����headset�˲���ϵ�� */
        swRet += SRS_TrMediaHD_SetCshpTBHDCustomSpeakerFilterCoefs(stDtsObj, Untitled_TBHD2);
        swRet += SRS_TrMediaHD_SetWowhdxTBHDCustomSpeakerFilterCoefs(stDtsObj, Untitled_TBHD2);
    }
    else /* �����豸Ŀǰ������Ϊspeaker */
    {
        /* ����speakerģʽ���� */
        UCOM_MemCpy(pstCurrentPara, &DTS_speaker, sizeof(SRSTrMediaHDControls));

        /* ��ȡ�˲������� */
        stCfg  = GetFilterNonflatPeqCfg((VOS_INT32)pstDtsCurtCfg->uwSampleRate, XA_DTS_DEV_SPEAKRE);

        /* ����ʵʱ�ӿ�����speaker�˲���ϵ�� */
        swRet += SRS_TrMediaHD_SetCshpTBHDCustomSpeakerFilterCoefs(stDtsObj, Untitled_TBHD1);
        swRet += SRS_TrMediaHD_SetWowhdxTBHDCustomSpeakerFilterCoefs(stDtsObj, Untitled_TBHD1);
    }

    /* �����Ƿ����ܵ������� */
    if (XA_DTS_ENABLE_YES == pstDtsUinitPara->enIsEnable)
    {
        pstCurrentPara->Enable = XA_DTS_ENABLE_YES;
    }
    else if (XA_DTS_ENABLE_NO == pstDtsUinitPara->enIsEnable)
    {
        pstCurrentPara->Enable = XA_DTS_ENABLE_NO;
    }
    else
    {
        pstCurrentPara->Enable = XA_DTS_ENABLE_NO;

        OM_LogError1(XA_DTS_SetDevError, pstDtsUinitPara->enIsEnable);
    }

    /* ����ʵʱ�ӿ����ò��� */
    swRet += SRS_TrMediaHD_SetControls(stDtsObj, pstCurrentPara);

    /* �����˲���ϵ�� */
    pstCfg = &stCfg;
    swRet += SRS_TrMediaHD_InitObj48k(stDtsObj, pstCfg);

    /* �ϱ�ִ�н�� */
    if (SRS_STAT_NO_ERROR == swRet)
    {
        OM_LogInfo1(XA_DTS_SetDevOK, enDevice);
    }
    else
    {
        OM_LogError1(XA_DTS_SetDevError, enDevice);
    }

    return (VOS_UINT32)swRet;

}
VOS_UINT32  XA_DTS_SetEnable(XA_DTS_ENABLE_ENUM_UINT16 enEnableFlag)
{
    SRSTrMediaHDControls        *pstCurrentPara     = XA_DTS_GetCurrentParaPtr();
    XA_DTS_CURRENT_CFG_STRU     *pstDtsCurtCfg      = XA_DTS_GetCurrentCfgPtr();
    SRSTrMediaHDObj              stDtsObj           = pstDtsCurtCfg->stDtsObj;
    VOS_INT32                    swRet;

    /* ���ĵ�ǰ���в��� */
    if (XA_DTS_ENABLE_YES == enEnableFlag)
    {
        /* ����ʹ�ܲ��� */
        pstCurrentPara->Enable = XA_DTS_ENABLE_YES;
    }
    else /* (XA_DTS_ENABLE_NO == enEnableFlag)*/
    {
        /* ���ò�ʹ�ܲ��� */
        pstCurrentPara->Enable = XA_DTS_ENABLE_NO;
    }

    /* ���ò��� */
    swRet = SRS_TrMediaHD_SetControls(stDtsObj, pstCurrentPara);

    /* �ϱ�ִ�н�� */
    if (SRS_STAT_NO_ERROR == swRet)
    {
        OM_LogInfo1(XA_DTS_SetEnableOK, enEnableFlag);
    }
    else
    {
        OM_LogError1(XA_DTS_SetEnableFail, enEnableFlag);
    }

    return (VOS_UINT32)swRet;

}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

