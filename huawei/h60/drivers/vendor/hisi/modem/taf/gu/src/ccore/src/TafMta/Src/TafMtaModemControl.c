

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "TafMtaMain.h"
#include "TafMtaComm.h"
#include "TafMtaCtx.h"
#include "TafSdcCtx.h"
#include "TafSdcLib.h"
#include "MtaMtcInterface.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_TAF_MTA_MODEM_CONTROL_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
#if (FEATURE_ON == FEATURE_MULTI_MODEM)

VOS_VOID TAF_MTA_RcvMtcPsTransferInd(
    VOS_VOID                           *pMsg
)
{
    MTC_MTA_PS_TRANSFER_IND_STRU       *pstPsTransferIndMsg;                    /* ������Ϣָ�� */
    AT_APPCTRL_STRU                     stAppCtrl;
    MTA_AT_PS_TRANSFER_IND_STRU         stPsTransferInd;

    /* ��ʼ���ֲ����� */
    pstPsTransferIndMsg     = (MTC_MTA_PS_TRANSFER_IND_STRU*)pMsg;
    PS_MEM_SET(&stAppCtrl, 0, sizeof(AT_APPCTRL_STRU));
    PS_MEM_SET(&stPsTransferInd, 0, sizeof(MTA_AT_PS_TRANSFER_IND_STRU));

    /* ��д��Ϣ�ṹ */
    stPsTransferInd.ucCause = pstPsTransferIndMsg->ucCause;

    /* ��д�¼��ͻ�������Ϊ�㲥�¼� */
    stAppCtrl.usClientId    = MTA_CLIENTID_BROADCAST;

    /* ��ATģ�鷢��ID_MTA_AT_PS_TRANSFER_IND��Ϣ */
    TAF_MTA_SndAtMsg( &stAppCtrl,
                      ID_MTA_AT_PS_TRANSFER_IND,
                      sizeof(MTA_AT_PS_TRANSFER_IND_STRU),
                      (VOS_UINT8*)&stPsTransferInd );

    return;
}
#endif
VOS_VOID TAF_MTA_SndMipiClkInfoToAt(VOS_UINT16 usMipiClk)
{
    AT_APPCTRL_STRU                     stAppCtrl;
    MTA_AT_RF_LCD_MIPICLK_IND_STRU      stMipClkInd;
    TAF_MTA_RF_LCD_CTX_STRU            *pstMtaRfLcdCtxAddr;

    /* ��ʼ���ֲ����� */
    PS_MEM_SET(&stAppCtrl, 0, sizeof(AT_APPCTRL_STRU));
    PS_MEM_SET(&stMipClkInd, 0, sizeof(MTA_AT_RF_LCD_MIPICLK_IND_STRU));

    /* ��ȡMTA�������б���Rf&Lcd���Ź�������ĵ�ַ */
    pstMtaRfLcdCtxAddr = TAF_MTA_GetRfLcdCtxAddr();

    /* ��д��Ϣ�ṹ */
    stMipClkInd.usMipiClk = usMipiClk;

    /* ��д�¼��ͻ�������Ϊ�㲥�¼� */
    stAppCtrl.usClientId    = MTA_CLIENTID_BROADCAST;

    /* ��ATģ�鷢��ID_MTA_AT_PS_TRANSFER_IND��Ϣ */
    TAF_MTA_SndAtMsg( &stAppCtrl,
                      ID_MTA_AT_MIPICLK_INFO_IND,
                      sizeof(MTA_AT_RF_LCD_MIPICLK_IND_STRU),
                      (VOS_UINT8*)&stMipClkInd );

    /* �����ϱ���MipiClkֵ */
    pstMtaRfLcdCtxAddr->usReportMipiClk= usMipiClk;

    if (0 != pstMtaRfLcdCtxAddr->usReportMipiClk)
    {
        /* ���ϱ���Ϊ��Чֵʱ��������ʱ�� */
        TAF_MTA_StartTimer(TI_TAF_MTA_RF_LCD_MIPI_CLK_REPORT, pstMtaRfLcdCtxAddr->ulTimerInterVal);
    }

    return;
}
VOS_UINT32 TAF_MTA_GetMipiClkRptCtl(VOS_VOID)
{
    TAF_SDC_CURC_RPT_CTRL_STRU         *pstCurcRptCtrl      = VOS_NULL_PTR;
    VOS_UINT32                          ulChkCurcRptFlg;

    /* �жϵ�ǰ�����ϱ��Ƿ�ر� */
    pstCurcRptCtrl  = TAF_SDC_GetCurcRptCtrl();
    ulChkCurcRptFlg = TAF_SDC_CheckRptCmdStatus(pstCurcRptCtrl->aucRptCfg,
                                                TAF_SDC_CMD_RPT_CTRL_BY_CURC,
                                                TAF_SDC_RPT_CMD_MIPICLK);

    return ulChkCurcRptFlg;

}
VOS_VOID TAF_MTA_RcvTiMipiClkExpired(VOS_VOID *pMsg)
{
    TAF_MTA_RF_LCD_CTX_STRU            *pstMtaRfLcdCtxAddr  = VOS_NULL_PTR;
    VOS_UINT32                          ulChkCurcRptFlg;

    /* ��ȡMTA�������б���Rf&Lcd���Ź�������ĵ�ַ */
    pstMtaRfLcdCtxAddr = TAF_MTA_GetRfLcdCtxAddr();

    if(pstMtaRfLcdCtxAddr->usNewMipiClk == pstMtaRfLcdCtxAddr->usReportMipiClk)
    {
        return;
    }

    ulChkCurcRptFlg = TAF_MTA_GetMipiClkRptCtl();
    if (VOS_FALSE == ulChkCurcRptFlg)
    {
        return;
    }

    /* �ϱ�mipiclk */
    TAF_MTA_SndMipiClkInfoToAt(pstMtaRfLcdCtxAddr->usNewMipiClk);

    return;

}


VOS_VOID TAF_MTA_RcvMtcMipiClkInfoInd(VOS_VOID *pMsg)
{
    MTC_MTA_MIPICLK_INFO_IND_STRU      *pstMipiClkIndMsg    = VOS_NULL_PTR;
    TAF_MTA_RF_LCD_CTX_STRU            *pstMtaRfLcdCtxAddr  = VOS_NULL_PTR;
    VOS_UINT32                          ulChkCurcRptFlg;
    TAF_MTA_TIMER_STATUS_ENUM_UINT8     enStatus;

    /* ��ȡMTA�������б���Rf&Lcd���Ź�������ĵ�ַ */
    pstMtaRfLcdCtxAddr                  = TAF_MTA_GetRfLcdCtxAddr();

    /* ��ʼ���ֲ����� */
    pstMipiClkIndMsg                    = (MTC_MTA_MIPICLK_INFO_IND_STRU *)pMsg;

    /* ���������ϱ���mipiclkֵ */
    pstMtaRfLcdCtxAddr->usNewMipiClk    = pstMipiClkIndMsg->usMipiCLk;

    /* ��ʱ���Ƿ������� */
    enStatus = TAF_MTA_GetTimerStatus(TI_TAF_MTA_RF_LCD_MIPI_CLK_REPORT);
    if (TAF_MTA_TIMER_STATUS_RUNING == enStatus)
    {
        return;
    }

    /* ���ϱ���MipiClk��֮ǰ�ϱ��� mipiclkֵ�Ƚ� */
    if (pstMtaRfLcdCtxAddr->usNewMipiClk == pstMtaRfLcdCtxAddr->usReportMipiClk)
    {
        return;
    }

    /* �жϵ�ǰ�����ϱ��Ƿ�ر� */
    ulChkCurcRptFlg = TAF_MTA_GetMipiClkRptCtl();
    if (VOS_FALSE == ulChkCurcRptFlg)
    {
        return;
    }

    /* ��ATģ�鷢����Ϣ */
    TAF_MTA_SndMipiClkInfoToAt(pstMtaRfLcdCtxAddr->usNewMipiClk);

    return;
}
VOS_VOID TAF_MTA_RcvAtQryMipiClkReq(VOS_VOID *pMsg)
{
    AT_MTA_MSG_STRU                    *pstAtMtaMsg         = VOS_NULL_PTR;
    TAF_MTA_RF_LCD_CTX_STRU            *pstMtaRfLcdCtx      = VOS_NULL_PTR;
    MTA_AT_RF_LCD_MIPICLK_CNF_STRU      stQryMipiCnf;
    TAF_MTA_TIMER_STATUS_ENUM_UINT8     enStatus;

    pstAtMtaMsg             = (AT_MTA_MSG_STRU *)pMsg;

    /* ��ȡMTA��Rf&Lcd������ */
    pstMtaRfLcdCtx          = TAF_MTA_GetRfLcdCtxAddr();

    /* �жϵ�ǰ��ʱ��״̬ */
    enStatus = TAF_MTA_GetTimerStatus(TI_TAF_MTA_RF_LCD_MIPI_CLK_REPORT);
    if (TAF_MTA_TIMER_STATUS_RUNING == enStatus)
    {
        TAF_MTA_StopTimer(TI_TAF_MTA_RF_LCD_MIPI_CLK_REPORT);
    }

    /* �����Ϣ���� */
    PS_MEM_SET(&stQryMipiCnf, 0x00, sizeof(stQryMipiCnf));
    stQryMipiCnf.usMipiClk      = pstMtaRfLcdCtx->usNewMipiClk;
    stQryMipiCnf.usResult       = VOS_OK;

    /* ���������δ����������ϲ�ظ�ʧ�� */
    if (0 == pstMtaRfLcdCtx->usEnableBitMap)
    {
        stQryMipiCnf.usResult   = VOS_ERR;
    }
    else
    {
        /* ������ʱ�� */
        TAF_MTA_StartTimer(TI_TAF_MTA_RF_LCD_MIPI_CLK_REPORT, pstMtaRfLcdCtx->ulTimerInterVal);

        /* �����ϱ�MIPICLKֵ */
        pstMtaRfLcdCtx->usReportMipiClk = pstMtaRfLcdCtx->usNewMipiClk;
    }

    /* ��AT���Ͳ�ѯMIPICLK�ظ� */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)&(pstAtMtaMsg->stAppCtrl),
                     ID_MTA_AT_MIPICLK_QRY_CNF,
                     sizeof(stQryMipiCnf),
                     (VOS_UINT8*)&stQryMipiCnf);

    return;
}




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


