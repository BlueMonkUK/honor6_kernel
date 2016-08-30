

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "TafMtaMntn.h"
#include "AtMtaInterface.h"
#include "TafSdcLib.h"
#include "TafMtaTimerMgmt.h"
#include "TafMtaMain.h"
#include "TafMtaCtx.h"
#include "TafMtaComm.h"
#include "DrvInterface.h"
#include "NasOmInterface.h"
#include "NasMtaInterface.h"
#include "TafAgentInterface.h"
#include "DrvInterface.h"

#include "NasErrorLog.h"

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
/*lint -e767 */
#define    THIS_FILE_ID                 PS_FILE_ID_TAF_MTA_MNTN_C
/*lint +e767 */

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_VOID TAF_MTA_RcvQryWrrAutotestReq(
    VOS_VOID                           *pMsg
)
{
    AT_MTA_MSG_STRU                    *pstQryReq       = VOS_NULL_PTR;
    MTA_WRR_AUTOTEST_PARA_STRU         *pstRrcAtQryPara = VOS_NULL_PTR;
    MTA_AT_WRR_AUTOTEST_QRY_CNF_STRU    stQryCnf;
    VOS_UINT32                          ulResult;
    /* ����UtranMode�ж��Ƿ��was������Ϣ����Ϊƽ̨֧��FDD������WAS������Ϣ */

    pstQryReq          = (AT_MTA_MSG_STRU *)pMsg;
    pstRrcAtQryPara    = (MTA_WRR_AUTOTEST_PARA_STRU *)pstQryReq->aucContent;
    ulResult           = VOS_ERR;

    /* �����ǰ��ʱ�����������򷵻�ʧ�� */
    if (TAF_MTA_TIMER_STATUS_RUNING == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_WRR_AUTOTEST_QRY_CNF))
    {
        PS_MEM_SET(&stQryCnf, 0x0, sizeof(stQryCnf));
        stQryCnf.ulResult = VOS_ERR;

        TAF_MTA_SndAtMsg(&pstQryReq->stAppCtrl,
                      ID_MTA_AT_WRR_AUTOTEST_QRY_CNF,
                      sizeof(stQryCnf),
                      (VOS_UINT8*)&stQryCnf );
        return;
    }

    /* ���ƽ̨֧��Wģ�����������as�������at�ظ�ʧ�� */
    if ( VOS_TRUE == TAF_SDC_IsPlatformSupportUtranFdd() )
    {
        /* �յ�at�������󣬷�������Ϣ֪ͨas */
        ulResult = TAF_MTA_SndWrrQryAutotestReqMsg(pstRrcAtQryPara);
    }

    if (ulResult != VOS_OK)
    {
        /* ��Ϣ����ʧ�ܣ���at�ظ�ʧ��*/
        PS_MEM_SET(&stQryCnf, 0x0, sizeof(stQryCnf));
        stQryCnf.ulResult = VOS_ERR;

        TAF_MTA_SndAtMsg(&pstQryReq->stAppCtrl,
                         ID_MTA_AT_WRR_AUTOTEST_QRY_CNF,
                         sizeof(stQryCnf),
                         (VOS_UINT8*)&stQryCnf );
        return;
    }

    /* ����������ʱ�� */
    TAF_MTA_StartTimer(TI_TAF_MTA_WAIT_WRR_AUTOTEST_QRY_CNF,
                        TI_TAF_MTA_WAIT_WRR_AUTOTEST_QRY_CNF_TIMER_LEN);

    /* �����Ϣ���ȴ����� */
    TAF_MTA_SaveItemInCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_AUTOTEST_QRY_CNF,
                                     (VOS_UINT8*)&pstQryReq->stAppCtrl,
                                     sizeof(AT_APPCTRL_STRU));

    return;
}
VOS_VOID TAF_MTA_RcvQryWrrCellInfoReq(
    VOS_VOID                           *pMsg
)
{
    AT_MTA_MSG_STRU                    *pstQryCellinfoReq = VOS_NULL_PTR;
    MTA_AT_WRR_CELLINFO_QRY_CNF_STRU    stQryCnf;
    VOS_UINT32                          ulResult;
    VOS_UINT32                          ulAtCmdCellInfo;
    /* ����UtranMode�ж��Ƿ��was������Ϣ����Ϊƽ̨֧��FDD������WAS������Ϣ */

    pstQryCellinfoReq      = (AT_MTA_MSG_STRU *)pMsg;
    ulResult               = VOS_ERR;

    PS_MEM_CPY(&ulAtCmdCellInfo, pstQryCellinfoReq->aucContent, sizeof(VOS_UINT32));

    PS_MEM_SET(&stQryCnf, 0x0, sizeof(stQryCnf));

    /* �����ǰ��ʱ�������������AT�ظ���ѯʧ�� */
    if (TAF_MTA_TIMER_STATUS_RUNING == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_WRR_CELLINFO_QRY_CNF))
    {
        stQryCnf.ulResult = VOS_ERR;
        TAF_MTA_SndAtMsg(&pstQryCellinfoReq->stAppCtrl,
                        ID_MTA_AT_WRR_CELLINFO_QRY_CNF,
                        sizeof(stQryCnf),
                        (VOS_UINT8*)&stQryCnf );
        return;
    }

    /* ���ƽ̨֧��Wģ�����������as�������at�ظ�ʧ�� */
    if ( VOS_TRUE == TAF_SDC_IsPlatformSupportUtranFdd() )
    {
        ulResult = TAF_MTA_SndWrrQryCellinfoReqMsg(ulAtCmdCellInfo);
    }

    /* �����ǰ��Utranģʽ����FDDģʽ,ulResultΪVOS_ERR��
       ���߸�����㷢����Ϣʧ��,ulResultҲΪVOS_ERR����AT�ظ���ѯʧ�� */
    if (VOS_OK != ulResult)
    {
        /* ��Ϣ����ʧ�ܣ���at�ظ�ʧ��*/
        stQryCnf.ulResult = VOS_ERR;

        TAF_MTA_SndAtMsg(&pstQryCellinfoReq->stAppCtrl,
                         ID_MTA_AT_WRR_CELLINFO_QRY_CNF,
                         sizeof(stQryCnf),
                         (VOS_UINT8*)&stQryCnf );

        return;
    }

    /* ����������ʱ�� */
    TAF_MTA_StartTimer(TI_TAF_MTA_WAIT_WRR_CELLINFO_QRY_CNF, TI_TAF_MTA_WAIT_WRR_CELLINFO_QRY_CNF_TIMER_LEN);

    /* �����Ϣ���ȴ����� */
    TAF_MTA_SaveItemInCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_CELLINFO_QRY_CNF,
                                     (VOS_UINT8*)&pstQryCellinfoReq->stAppCtrl,
                                     sizeof(AT_APPCTRL_STRU));

    return;
}
VOS_VOID TAF_MTA_RcvQryWrrMeanrptReq(
    VOS_VOID                           *pMsg
)
{

    AT_MTA_MSG_STRU                    *pstQryReq = VOS_NULL_PTR;
    MTA_AT_WRR_MEANRPT_QRY_CNF_STRU     stQryCnf;
    VOS_UINT32                          ulResult;
    /* ����UtranMode�ж��Ƿ��was������Ϣ����Ϊƽ̨֧��FDD������WAS������Ϣ */

    pstQryReq           = (AT_MTA_MSG_STRU *)pMsg;
    ulResult            = VOS_ERR;
    PS_MEM_SET(&stQryCnf, 0x0, sizeof(stQryCnf));

    /* �����ǰ��ʱ�������������AT�ظ���ѯʧ�� */
    if (TAF_MTA_TIMER_STATUS_RUNING == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_WRR_MEANRPT_QRY_CNF))
    {
        stQryCnf.ulResult = VOS_ERR;
        TAF_MTA_SndAtMsg(&pstQryReq->stAppCtrl,
                      ID_MTA_AT_WRR_MEANRPT_QRY_CNF,
                      sizeof(stQryCnf),
                      (VOS_UINT8*)&stQryCnf );
        return;
    }

    /* ���ƽ̨֧��Wģ�����������as�������at�ظ�ʧ�� */
    if ( VOS_TRUE == TAF_SDC_IsPlatformSupportUtranFdd() )
    {
        /* �յ�at�������󣬷�������Ϣ֪ͨas */
        ulResult = TAF_MTA_SndWrrQryMeanrptReqMsg();
    }

    if (VOS_OK != ulResult)
    {
        /* ��Ϣ����ʧ�ܣ���at�ظ�ʧ��*/
        stQryCnf.ulResult = VOS_ERR;

        TAF_MTA_SndAtMsg(&pstQryReq->stAppCtrl,
                         ID_MTA_AT_WRR_MEANRPT_QRY_CNF,
                         sizeof(stQryCnf),
                         (VOS_UINT8*)&stQryCnf );

        return;
    }

    /* ����������ʱ�� */
    TAF_MTA_StartTimer(TI_TAF_MTA_WAIT_WRR_MEANRPT_QRY_CNF, TI_TAF_MTA_WAIT_WRR_MEANRPT_QRY_CNF_TIMER_LEN);

    /* �����Ϣ���ȴ����� */
    TAF_MTA_SaveItemInCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_MEANRPT_QRY_CNF,
                                     (VOS_UINT8*)&pstQryReq->stAppCtrl,
                                     sizeof(AT_APPCTRL_STRU));

    return;
}
VOS_VOID TAF_MTA_RcvSetWrrFreqlockReq(
    VOS_VOID                           *pMsg
)
{
    AT_MTA_MSG_STRU                    *pstQryReq       = VOS_NULL_PTR;
    MTA_AT_WRR_FREQLOCK_CTRL_STRU      *pstRrcAtSetPara = VOS_NULL_PTR;
    MTA_AT_WRR_FREQLOCK_SET_CNF_STRU    stQryCnf;
    VOS_UINT32                          ulResult;
    /* ����UtranMode�ж��Ƿ��was������Ϣ����Ϊƽ̨֧��FDD������WAS������Ϣ */

    pstQryReq          = (AT_MTA_MSG_STRU *)pMsg;
    pstRrcAtSetPara    = (MTA_AT_WRR_FREQLOCK_CTRL_STRU *)pstQryReq->aucContent;
    ulResult           = VOS_ERR;

    /* �����ǰ��ʱ�����������򷵻�ʧ�� */
    if (TAF_MTA_TIMER_STATUS_RUNING == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_WRR_FREQLOCK_SET_CNF))
    {
        PS_MEM_SET(&stQryCnf, 0x0, sizeof(stQryCnf));
        stQryCnf.ulResult = VOS_ERR;

        TAF_MTA_SndAtMsg(&pstQryReq->stAppCtrl,
                      ID_MTA_AT_WRR_FREQLOCK_SET_CNF,
                      sizeof(stQryCnf),
                      (VOS_UINT8*)&stQryCnf );
        return;
    }

    /* ���ƽ̨֧��Wģ�����������as�������at�ظ�ʧ�� */
    if ( VOS_TRUE == TAF_SDC_IsPlatformSupportUtranFdd() )
    {
        /* �յ�at�������󣬷�������Ϣ֪ͨas */
        ulResult = TAF_MTA_SndWrrSetFreqlockReqMsg((MTA_WRR_FREQLOCK_CTRL_STRU *)pstRrcAtSetPara);
    }

    if (ulResult != VOS_OK)
    {
        /* ��Ϣ����ʧ�ܣ���at�ظ�ʧ��*/
        PS_MEM_SET(&stQryCnf, 0x0, sizeof(stQryCnf));
        stQryCnf.ulResult = VOS_ERR;

        TAF_MTA_SndAtMsg(&pstQryReq->stAppCtrl,
                         ID_MTA_AT_WRR_FREQLOCK_SET_CNF,
                         sizeof(stQryCnf),
                         (VOS_UINT8*)&stQryCnf );
        return;
    }

    /* ����������ʱ�� */
    TAF_MTA_StartTimer(TI_TAF_MTA_WAIT_WRR_FREQLOCK_SET_CNF,
                        TI_TAF_MTA_WAIT_WRR_FREQLOCK_SET_CNF_TIMER_LEN);

    /* �����Ϣ���ȴ����� */
    TAF_MTA_SaveItemInCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_FREQLOCK_SET_CNF,
                                     (VOS_UINT8*)&pstQryReq->stAppCtrl,
                                     sizeof(AT_APPCTRL_STRU));

    return;
}
VOS_VOID TAF_MTA_RcvSetWrrRrcVersionReq(
    VOS_VOID                           *pMsg
)
{
    AT_MTA_MSG_STRU                                        *pstSetReq = VOS_NULL_PTR;
    MTA_AT_WRR_RRC_VERSION_SET_CNF_STRU                     stSetCnf;
    VOS_UINT32                                              ulResult;
    /* ����UtranMode�ж��Ƿ��was������Ϣ����Ϊƽ̨֧��FDD������WAS������Ϣ */

    pstSetReq = (AT_MTA_MSG_STRU *)pMsg;
    ulResult  = VOS_ERR;

    /* �����ǰ��ʱ�����������򷵻�ʧ�� */
    if (TAF_MTA_TIMER_STATUS_RUNING == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_WRR_RRC_VERSION_SET_CNF))
    {
        PS_MEM_SET(&stSetCnf, 0x0, sizeof(stSetCnf));
        stSetCnf.ulResult = VOS_ERR;

        TAF_MTA_SndAtMsg(&pstSetReq->stAppCtrl,
                      ID_MTA_AT_WRR_RRC_VERSION_SET_CNF,
                      sizeof(stSetCnf),
                      (VOS_UINT8*)&stSetCnf );
        return;
    }

    /* ���ƽ̨֧��Wģ�����������as�������at�ظ�ʧ�� */
    if ( VOS_TRUE == TAF_SDC_IsPlatformSupportUtranFdd() )
    {
        /* �յ�at�������󣬷�������Ϣ֪ͨas */
        ulResult = TAF_MTA_SndWrrSetVersionReqMsg(pstSetReq->aucContent[0]);
    }

    if (ulResult != VOS_OK)
    {
        /* ��Ϣ����ʧ�ܣ���at�ظ�ʧ��*/
        PS_MEM_SET(&stSetCnf, 0x0, sizeof(stSetCnf));
        stSetCnf.ulResult = VOS_ERR;

        TAF_MTA_SndAtMsg(&pstSetReq->stAppCtrl,
                         ID_MTA_AT_WRR_RRC_VERSION_SET_CNF,
                         sizeof(stSetCnf),
                         (VOS_UINT8*)&stSetCnf );
        return;
    }

    /* ����������ʱ�� */
    TAF_MTA_StartTimer(TI_TAF_MTA_WAIT_WRR_RRC_VERSION_SET_CNF,
                       TI_TAF_MTA_WAIT_WRR_RRC_VERSION_SET_CNF_TIMER_LEN);

    /* �����Ϣ���ȴ����� */
    TAF_MTA_SaveItemInCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_RRC_VERSION_SET_CNF,
                                     (VOS_UINT8*)&pstSetReq->stAppCtrl,
                                     sizeof(AT_APPCTRL_STRU));

    return;
}
VOS_VOID TAF_MTA_RcvSetWrrCellsrhReq(
    VOS_VOID                           *pMsg
)
{
    AT_MTA_MSG_STRU                                        *pstSetReq = VOS_NULL_PTR;
    MTA_AT_WRR_CELLSRH_SET_CNF_STRU                         stSetCnf;
    VOS_UINT32                                              ulResult;
    /* ����UtranMode�ж��Ƿ��was������Ϣ����Ϊƽ̨֧��FDD������WAS������Ϣ */

    pstSetReq = (AT_MTA_MSG_STRU *)pMsg;
    ulResult  = VOS_ERR;

    /* �����ǰ��ʱ�����������򷵻�ʧ�� */
    if (TAF_MTA_TIMER_STATUS_RUNING == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_WRR_CELLSRH_SET_CNF))
    {
        PS_MEM_SET(&stSetCnf, 0x0, sizeof(stSetCnf));
        stSetCnf.ulResult = VOS_ERR;

        TAF_MTA_SndAtMsg(&pstSetReq->stAppCtrl,
                      ID_MTA_AT_WRR_CELLSRH_SET_CNF,
                      sizeof(stSetCnf),
                      (VOS_UINT8*)&stSetCnf );
        return;
    }

    /* ���ƽ̨֧��Wģ�����������as�������at�ظ�ʧ�� */
    if ( VOS_TRUE == TAF_SDC_IsPlatformSupportUtranFdd() )
    {
        /* �յ�at�������󣬷�������Ϣ֪ͨas */
        ulResult = TAF_MTA_SndWrrSetCellsrhReqMsg(pstSetReq->aucContent[0]);
    }

    if (ulResult != VOS_OK)
    {
        /* ��Ϣ����ʧ�ܣ���at�ظ�ʧ��*/
        PS_MEM_SET(&stSetCnf, 0x0, sizeof(stSetCnf));
        stSetCnf.ulResult = VOS_ERR;

        TAF_MTA_SndAtMsg(&pstSetReq->stAppCtrl,
                         ID_MTA_AT_WRR_CELLSRH_SET_CNF,
                         sizeof(stSetCnf),
                         (VOS_UINT8*)&stSetCnf );
        return;
    }

    /* ����������ʱ�� */
    TAF_MTA_StartTimer(TI_TAF_MTA_WAIT_WRR_CELLSRH_SET_CNF,
                       TI_TAF_MTA_WAIT_WRR_CELLSRH_SET_CNF_TIMER_LEN);

    /* �����Ϣ���ȴ����� */
    TAF_MTA_SaveItemInCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_CELLSRH_SET_CNF,
                                     (VOS_UINT8*)&pstSetReq->stAppCtrl,
                                     sizeof(AT_APPCTRL_STRU));

    return;
}
VOS_VOID TAF_MTA_RcvQryWrrFreqlockReq(
    VOS_VOID                           *pMsg
)
{
    AT_MTA_MSG_STRU                    *pstQryReq = VOS_NULL_PTR;
    MTA_AT_WRR_FREQLOCK_QRY_CNF_STRU    stQryCnf;
    VOS_UINT32                          ulResult;
    /* ����UtranMode�ж��Ƿ��was������Ϣ����Ϊƽ̨֧��FDD������WAS������Ϣ */

    pstQryReq          = (AT_MTA_MSG_STRU *)pMsg;
    ulResult           = VOS_ERR;

    /* �����ǰ��ʱ�����������򷵻�ʧ�� */
    if (TAF_MTA_TIMER_STATUS_RUNING == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_WRR_FREQLOCK_QRY_CNF))
    {
        PS_MEM_SET(&stQryCnf, 0x0, sizeof(stQryCnf));
        stQryCnf.ulResult = VOS_ERR;

        TAF_MTA_SndAtMsg(&pstQryReq->stAppCtrl,
                      ID_MTA_AT_WRR_FREQLOCK_QRY_CNF,
                      sizeof(stQryCnf),
                      (VOS_UINT8*)&stQryCnf );
        return;
    }

    /* ���ƽ̨֧��Wģ�����������as�������at�ظ�ʧ�� */
    if ( VOS_TRUE == TAF_SDC_IsPlatformSupportUtranFdd() )
    {
        /* �յ�at�������󣬷�������Ϣ֪ͨas */
        ulResult = TAF_MTA_SndWrrQryFreqlockReqMsg();
    }

    if (ulResult != VOS_OK)
    {
        /* ��Ϣ����ʧ�ܣ���at�ظ�ʧ��*/
        PS_MEM_SET(&stQryCnf, 0x0, sizeof(stQryCnf));
        stQryCnf.ulResult = VOS_ERR;

        TAF_MTA_SndAtMsg(&pstQryReq->stAppCtrl,
                         ID_MTA_AT_WRR_FREQLOCK_QRY_CNF,
                         sizeof(stQryCnf),
                         (VOS_UINT8*)&stQryCnf );
        return;
    }

    /* ����������ʱ�� */
    TAF_MTA_StartTimer(TI_TAF_MTA_WAIT_WRR_FREQLOCK_QRY_CNF,
                        TI_TAF_MTA_WAIT_WRR_FREQLOCK_QRY_CNF_TIMER_LEN);

    /* �����Ϣ���ȴ����� */
    TAF_MTA_SaveItemInCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_FREQLOCK_QRY_CNF,
                                     (VOS_UINT8*)&pstQryReq->stAppCtrl,
                                     sizeof(AT_APPCTRL_STRU));

    return;
}
VOS_VOID TAF_MTA_RcvQryWrrRrcVersionReq(
    VOS_VOID                           *pMsg
)
{
    AT_MTA_MSG_STRU                                        *pstQryVersionReq = VOS_NULL_PTR;
    MTA_AT_WRR_RRC_VERSION_QRY_CNF_STRU                     stQryCnf;
    VOS_UINT32                                              ulResult;
    /* ����UtranMode�ж��Ƿ��was������Ϣ����Ϊƽ̨֧��FDD������WAS������Ϣ */

    pstQryVersionReq = (AT_MTA_MSG_STRU *)pMsg;
    ulResult         = VOS_ERR;

    PS_MEM_SET(&stQryCnf, 0x0, sizeof(stQryCnf));

    /* �����ǰ��ʱ�������������AT�ظ���ѯʧ�� */
    if (TAF_MTA_TIMER_STATUS_RUNING == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_WRR_RRC_VERSION_QRY_CNF))
    {
        stQryCnf.ulResult = VOS_ERR;
        TAF_MTA_SndAtMsg(&pstQryVersionReq->stAppCtrl,
                      ID_MTA_AT_WRR_RRC_VERSION_QRY_CNF,
                      sizeof(stQryCnf),
                      (VOS_UINT8*)&stQryCnf );
        return;
    }

    /* ���ƽ̨֧��Wģ�����������as�������at�ظ�ʧ�� */
    if ( VOS_TRUE == TAF_SDC_IsPlatformSupportUtranFdd() )
    {
        ulResult = TAF_MTA_SndWrrQryRrcVersionReqMsg();
    }

    /* �����ǰ��Utranģʽ����FDDģʽ,ulResultΪVOS_ERR��
       ���߸�����㷢����Ϣʧ��,ulResultҲΪVOS_ERR����AT�ظ���ѯʧ�� */
    if (VOS_OK != ulResult)
    {
        /* ��Ϣ����ʧ�ܣ���at�ظ�ʧ��*/
        stQryCnf.ulResult = VOS_ERR;

        TAF_MTA_SndAtMsg(&pstQryVersionReq->stAppCtrl,
                         ID_MTA_AT_WRR_RRC_VERSION_QRY_CNF,
                         sizeof(stQryCnf),
                         (VOS_UINT8*)&stQryCnf );
        return;
    }

    /* ����������ʱ�� */
    TAF_MTA_StartTimer(TI_TAF_MTA_WAIT_WRR_RRC_VERSION_QRY_CNF, TI_TAF_MTA_WAIT_WRR_RRC_VERSION_QRY_CNF_TIMER_LEN);

    /* �����Ϣ���ȴ����� */
    TAF_MTA_SaveItemInCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_RRC_VERSION_QRY_CNF,
                                     (VOS_UINT8*)&pstQryVersionReq->stAppCtrl,
                                     sizeof(AT_APPCTRL_STRU));

    return;
}
VOS_VOID TAF_MTA_RcvQryWrrCellsrhReq(
    VOS_VOID                           *pMsg
)
{
    AT_MTA_MSG_STRU                                        *pstQryVersionReq = VOS_NULL_PTR;
    MTA_AT_WRR_CELLSRH_QRY_CNF_STRU                         stQryCnf;
    VOS_UINT32                                              ulResult;
    /* ����UtranMode�ж��Ƿ��was������Ϣ����Ϊƽ̨֧��FDD������WAS������Ϣ */

    pstQryVersionReq = (AT_MTA_MSG_STRU *)pMsg;
    ulResult         = VOS_ERR;

    PS_MEM_SET(&stQryCnf, 0x0, sizeof(stQryCnf));

    /* �����ǰ��ʱ�������������AT�ظ���ѯʧ�� */
    if (TAF_MTA_TIMER_STATUS_RUNING == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_WRR_CELLSRH_QRY_CNF))
    {
        stQryCnf.ulResult = VOS_ERR;
        TAF_MTA_SndAtMsg(&pstQryVersionReq->stAppCtrl,
                      ID_MTA_AT_WRR_CELLSRH_QRY_CNF,
                      sizeof(stQryCnf),
                      (VOS_UINT8*)&stQryCnf );
        return;
    }

    /* ���ƽ̨֧��Wģ�����������as�������at�ظ�ʧ�� */
    if ( VOS_TRUE == TAF_SDC_IsPlatformSupportUtranFdd() )
    {
        ulResult = TAF_MTA_SndWrrQryCellsrhReqMsg();
    }

    /* �����ǰ��Utranģʽ����FDDģʽ,ulResultΪVOS_ERR��
       ���߸�����㷢����Ϣʧ��,ulResultҲΪVOS_ERR����AT�ظ���ѯʧ�� */
    if (VOS_OK != ulResult)
    {
        /* ��Ϣ����ʧ�ܣ���at�ظ�ʧ��*/
        stQryCnf.ulResult = VOS_ERR;

        TAF_MTA_SndAtMsg(&pstQryVersionReq->stAppCtrl,
                         ID_MTA_AT_WRR_CELLSRH_QRY_CNF,
                         sizeof(stQryCnf),
                         (VOS_UINT8*)&stQryCnf );
        return;
    }

    /* ����������ʱ�� */
    TAF_MTA_StartTimer(TI_TAF_MTA_WAIT_WRR_CELLSRH_QRY_CNF, TI_TAF_MTA_WAIT_WRR_CELLSRH_QRY_CNF_TIMER_LEN);

    /* �����Ϣ���ȴ����� */
    TAF_MTA_SaveItemInCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_CELLSRH_QRY_CNF,
                                     (VOS_UINT8*)&pstQryVersionReq->stAppCtrl,
                                     sizeof(AT_APPCTRL_STRU));

    return;
}
VOS_VOID TAF_MTA_RcvAtSetNCellMonitorReq(
    VOS_VOID                           *pMsg
)
{
    AT_MTA_MSG_STRU                            *pstSetReq = VOS_NULL_PTR;
    AT_MTA_NCELL_MONITOR_SET_REQ_STRU          *pstSetRptReq = VOS_NULL_PTR;
    MTA_AT_RESULT_CNF_STRU                      stSetCnf;
    MTA_AT_RESULT_ENUM_UINT32                   enResult;

    pstSetReq    = (AT_MTA_MSG_STRU *)pMsg;
    pstSetRptReq = (AT_MTA_NCELL_MONITOR_SET_REQ_STRU *)pstSetReq->aucContent;
    enResult     = MTA_AT_RESULT_ERROR;

    /* �����ǰ��ʱ�����������򷵻�ʧ�� */
    if (TAF_MTA_TIMER_STATUS_RUNING == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_SET_CNF))
    {
        PS_MEM_SET(&stSetCnf, 0x0, sizeof(stSetCnf));
        stSetCnf.enResult = MTA_AT_RESULT_ERROR;

        TAF_MTA_SndAtMsg(&pstSetReq->stAppCtrl,
                      ID_MTA_AT_NCELL_MONITOR_SET_CNF,
                      sizeof(stSetCnf),
                      (VOS_UINT8*)&stSetCnf );
        return;
    }

    /* ���ƽ̨֧��Gģ�����������as�������at�ظ�ʧ�� */
    if ( VOS_TRUE == TAF_SDC_IsPlatformSupportGsm() )
    {
        /* �յ�at�������󣬷�������Ϣ֪ͨas */
        enResult = TAF_MTA_SndGrrNCellMonitorSetReq(pstSetRptReq->ucSwitch);
    }

    if (MTA_AT_RESULT_NO_ERROR != enResult)
    {
        /* ��Ϣ����ʧ�ܣ���at�ظ�ʧ��*/
        PS_MEM_SET(&stSetCnf, 0x0, sizeof(stSetCnf));
        stSetCnf.enResult = MTA_AT_RESULT_ERROR;

        TAF_MTA_SndAtMsg(&pstSetReq->stAppCtrl,
                         ID_MTA_AT_NCELL_MONITOR_SET_CNF,
                         sizeof(stSetCnf),
                         (VOS_UINT8*)&stSetCnf );
        return;
    }

    /* ����������ʱ�� */
    TAF_MTA_StartTimer(TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_SET_CNF,
                       TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_SET_CNF_TIMER_LEN);

    /* �����Ϣ���ȴ����� */
    TAF_MTA_SaveItemInCmdBufferQueue(TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_SET_CNF,
                                     (VOS_UINT8*)&pstSetReq->stAppCtrl,
                                     sizeof(AT_APPCTRL_STRU));

    return;
}


VOS_VOID TAF_MTA_RcvAtQryNCellMonitorReq(
    VOS_VOID                           *pMsg
)
{
    AT_MTA_MSG_STRU                            *pstQryReq = VOS_NULL_PTR;
    MTA_AT_NCELL_MONITOR_QRY_CNF_STRU           stQryCnf;
    MTA_AT_RESULT_ENUM_UINT32                   enResult;

    pstQryReq    = (AT_MTA_MSG_STRU *)pMsg;
    enResult     = MTA_AT_RESULT_ERROR;

    /* �����ǰ��ʱ�����������򷵻�ʧ�� */
    if (TAF_MTA_TIMER_STATUS_RUNING == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_QRY_CNF))
    {
        PS_MEM_SET(&stQryCnf, 0x0, sizeof(stQryCnf));
        stQryCnf.enResult = MTA_AT_RESULT_ERROR;

        TAF_MTA_SndAtMsg(&pstQryReq->stAppCtrl,
                      ID_MTA_AT_NCELL_MONITOR_QRY_CNF,
                      sizeof(stQryCnf),
                      (VOS_UINT8*)&stQryCnf );
        return;
    }

    /* ���ƽ̨֧��Gģ�����������as�������at�ظ�ʧ�� */
    if ( VOS_TRUE == TAF_SDC_IsPlatformSupportGsm() )
    {
        /* �յ�at�������󣬷�������Ϣ֪ͨas */
        enResult = TAF_MTA_SndGrrNCellMonitorQryReq();
    }

    if (MTA_AT_RESULT_NO_ERROR != enResult)
    {
        /* ��Ϣ����ʧ�ܣ���at�ظ�ʧ��*/
        PS_MEM_SET(&stQryCnf, 0x0, sizeof(stQryCnf));
        stQryCnf.enResult = MTA_AT_RESULT_ERROR;

        TAF_MTA_SndAtMsg(&pstQryReq->stAppCtrl,
                         ID_MTA_AT_NCELL_MONITOR_QRY_CNF,
                         sizeof(stQryCnf),
                         (VOS_UINT8*)&stQryCnf );
        return;
    }

    /* ����������ʱ�� */
    TAF_MTA_StartTimer(TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_QRY_CNF,
                       TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_QRY_CNF_TIMER_LEN);

    /* �����Ϣ���ȴ����� */
    TAF_MTA_SaveItemInCmdBufferQueue(TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_QRY_CNF,
                                     (VOS_UINT8*)&pstQryReq->stAppCtrl,
                                     sizeof(AT_APPCTRL_STRU));

    return;
}



VOS_VOID TAF_MTA_RcvAtSetJamDetectReq(
    VOS_VOID                           *pMsg
)
{
    AT_MTA_MSG_STRU                    *pstSetReq       = VOS_NULL_PTR;
    AT_MTA_SET_JAM_DETECT_REQ_STRU     *pstJamDetectReq = VOS_NULL_PTR;
    MTA_AT_SET_JAM_DETECT_CNF_STRU      stQryCnf;
    VOS_UINT32                          ulResult;

    pstSetReq         = (AT_MTA_MSG_STRU *)pMsg;
    pstJamDetectReq   = (AT_MTA_SET_JAM_DETECT_REQ_STRU *)(pstSetReq->aucContent);
    ulResult          = VOS_ERR;
    stQryCnf.enResult = MTA_AT_RESULT_ERROR;

    /* �����ǰ��ʱ�����������򷵻�ʧ�� */
    if (TAF_MTA_TIMER_STATUS_RUNING == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_JAM_DETECT_SET_CNF))
    {
        TAF_MTA_SndAtMsg(&pstSetReq->stAppCtrl,
                         ID_MTA_AT_SET_JAM_DETECT_CNF,
                         sizeof(stQryCnf),
                         (VOS_UINT8*)&stQryCnf);
        return;
    }

    /* ���ƽ̨֧��Gģ�����������as�������at�ظ�ʧ�� */
    if (VOS_TRUE == TAF_SDC_IsPlatformSupportGsm())
    {
        /* �յ�at�������󣬷�������Ϣ֪ͨas */
        ulResult = TAF_MTA_SndGrrSetJamDetectReq(pstJamDetectReq);
    }

    if (VOS_OK != ulResult)
    {
        /* ��Ϣ����ʧ�ܣ���at�ظ�ʧ��*/
        TAF_MTA_SndAtMsg(&pstSetReq->stAppCtrl,
                         ID_MTA_AT_SET_JAM_DETECT_CNF,
                         sizeof(stQryCnf),
                         (VOS_UINT8*)&stQryCnf);
        return;
    }

    /* ����������ʱ�� */
    TAF_MTA_StartTimer(TI_TAF_MTA_WAIT_JAM_DETECT_SET_CNF,
                       TI_TAF_MTA_WAIT_JAM_DETECT_SET_CNF_TIMER_LEN);

    /* �����Ϣ���ȴ����� */
    TAF_MTA_SaveItemInCmdBufferQueue(TI_TAF_MTA_WAIT_JAM_DETECT_SET_CNF,
                                    (VOS_UINT8*)&pstSetReq->stAppCtrl,
                                     sizeof(AT_APPCTRL_STRU));

    return;
}


VOS_VOID TAF_MTA_RcvWrrAutotestQryCnf(
    VOS_VOID                           *pWrrCnfMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBuf            = VOS_NULL_PTR;
    WRR_MTA_AUTOTEST_QRY_CNF_STRU      *pstRcvWrrAutotestCnf = VOS_NULL_PTR;
    MTA_AT_WRR_AUTOTEST_QRY_CNF_STRU    stSndAtAutotestCnf;

    /* �ж϶�ʱ���Ƿ����� */
    if (TAF_MTA_TIMER_STATUS_STOP == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_WRR_AUTOTEST_QRY_CNF))
    {
        MTA_WARNING_LOG("TAF_MTA_RcvWrrAutotestQryCnf: WARNING: Timer was already stop!");
        return;
    }

    /* ֹͣ������ʱ�� */
    TAF_MTA_StopTimer(TI_TAF_MTA_WAIT_WRR_AUTOTEST_QRY_CNF);

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_AUTOTEST_QRY_CNF);
    if (VOS_NULL_PTR == pstCmdBuf)
    {
        MTA_WARNING_LOG("TAF_MTA_RcvWrrAutotestQryCnf: get command buffer failed!");
        return;
    }

    pstRcvWrrAutotestCnf                 = (WRR_MTA_AUTOTEST_QRY_CNF_STRU *)pWrrCnfMsg;
    stSndAtAutotestCnf.ulResult          = pstRcvWrrAutotestCnf->ulResult;
    stSndAtAutotestCnf.stWrrAutoTestRslt.ulRsltNum = pstRcvWrrAutotestCnf->stWrrAutoTestRslt.ulRsltNum;
    PS_MEM_CPY(stSndAtAutotestCnf.stWrrAutoTestRslt.aulRslt,
               pstRcvWrrAutotestCnf->stWrrAutoTestRslt.aulRslt,
               sizeof(pstRcvWrrAutotestCnf->stWrrAutoTestRslt.aulRslt));

    /* ��at����Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_WRR_AUTOTEST_QRY_CNF,
                     sizeof(stSndAtAutotestCnf),
                     (VOS_UINT8*)&stSndAtAutotestCnf );

    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_AUTOTEST_QRY_CNF);
    return;
}
VOS_VOID TAF_MTA_RcvWrrCellInfoQryCnf(
    VOS_VOID                           *pWrrCnfMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBuf            = VOS_NULL_PTR;
    WRR_MTA_CELLINFO_QRY_CNF_STRU      *pstRcvWrrCellinfoCnf = VOS_NULL_PTR;
    MTA_AT_WRR_CELLINFO_QRY_CNF_STRU    stSndAtCellinfoQryCnf;

    /* �ж϶�ʱ���Ƿ����� */
    if (TAF_MTA_TIMER_STATUS_STOP == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_WRR_CELLINFO_QRY_CNF))
    {
        MTA_WARNING_LOG("TAF_MTA_RcvWrrCellInfoQryCnf: WARNING: Timer was already stop!");
        return;
    }

    /* ֹͣ������ʱ�� */
    TAF_MTA_StopTimer(TI_TAF_MTA_WAIT_WRR_CELLINFO_QRY_CNF);

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_CELLINFO_QRY_CNF);
    if (VOS_NULL_PTR == pstCmdBuf)
    {
        MTA_WARNING_LOG("TAF_MTA_RcvWrrCellInfoQryCnf: get command buffer failed!");
        return;
    }

    pstRcvWrrCellinfoCnf           = (WRR_MTA_CELLINFO_QRY_CNF_STRU *)pWrrCnfMsg;
    stSndAtCellinfoQryCnf.ulResult = pstRcvWrrCellinfoCnf->ulResult;
    PS_MEM_CPY(&(stSndAtCellinfoQryCnf.stWrrCellInfo),
                &(pstRcvWrrCellinfoCnf->stWrrCellInfo),
                sizeof(pstRcvWrrCellinfoCnf->stWrrCellInfo));

    /* ��at����Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_WRR_CELLINFO_QRY_CNF,
                     sizeof(stSndAtCellinfoQryCnf),
                     (VOS_UINT8*)&stSndAtCellinfoQryCnf );

    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_CELLINFO_QRY_CNF);
    return;
}
VOS_VOID TAF_MTA_RcvWrrMeanrptQryCnf(
    VOS_VOID                           *pWrrCnfMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBuf           = VOS_NULL_PTR;
    WRR_MTA_MEANRPT_QRY_CNF_STRU       *pstRcvWrrMeanrptCnf = VOS_NULL_PTR;
    MTA_AT_WRR_MEANRPT_QRY_CNF_STRU     stSndAtMeanrptQryCnf;

    /* �ж϶�ʱ���Ƿ����� */
    if (TAF_MTA_TIMER_STATUS_STOP == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_WRR_MEANRPT_QRY_CNF))
    {
        MTA_WARNING_LOG("TAF_MTA_RcvWrrMeanrptQryCnf: WARNING: Timer was already stop!");
        return;
    }

    /* ֹͣ������ʱ�� */
    TAF_MTA_StopTimer(TI_TAF_MTA_WAIT_WRR_MEANRPT_QRY_CNF);

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_MEANRPT_QRY_CNF);
    if (VOS_NULL_PTR == pstCmdBuf)
    {
        MTA_WARNING_LOG("TAF_MTA_RcvWrrMeanrptQryCnf: get command buffer failed!");
        return;
    }

    pstRcvWrrMeanrptCnf                         = (WRR_MTA_MEANRPT_QRY_CNF_STRU *)pWrrCnfMsg;
    stSndAtMeanrptQryCnf.ulResult               = pstRcvWrrMeanrptCnf->ulResult;
    stSndAtMeanrptQryCnf.stMeanRptRslt.ulRptNum = pstRcvWrrMeanrptCnf->stMeanRptRslt.ulRptNum;
    PS_MEM_CPY((stSndAtMeanrptQryCnf.stMeanRptRslt.astMeanRptInfo), pstRcvWrrMeanrptCnf->stMeanRptRslt.astMeanRptInfo, sizeof(pstRcvWrrMeanrptCnf->stMeanRptRslt.astMeanRptInfo));

    /* ��at����Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_WRR_MEANRPT_QRY_CNF,
                     sizeof(stSndAtMeanrptQryCnf),
                     (VOS_UINT8*)&stSndAtMeanrptQryCnf);

    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_MEANRPT_QRY_CNF);
    return;
}
VOS_VOID TAF_MTA_RcvWrrFreqlockSetCnf(
    VOS_VOID                           *pWrrCnfMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBuf            = VOS_NULL_PTR;
    WRR_MTA_FREQLOCK_SET_CNF_STRU      *pstRcvWrrFreqlockCnf = VOS_NULL_PTR;
    MTA_AT_WRR_FREQLOCK_SET_CNF_STRU    stSndAtFreqlockCnf;

    /* �ж϶�ʱ���Ƿ����� */
    if (TAF_MTA_TIMER_STATUS_STOP == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_WRR_FREQLOCK_SET_CNF))
    {
        MTA_WARNING_LOG("TAF_MTA_RcvWrrFreqlockSetCnf: WARNING: Timer was already stop!");
        return;
    }

    /* ֹͣ������ʱ�� */
    TAF_MTA_StopTimer(TI_TAF_MTA_WAIT_WRR_FREQLOCK_SET_CNF);

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_FREQLOCK_SET_CNF);
    if (VOS_NULL_PTR == pstCmdBuf)
    {
        MTA_WARNING_LOG("TAF_MTA_RcvWrrFreqlockSetCnf: get command buffer failed!");
        return;
    }

    pstRcvWrrFreqlockCnf        = (WRR_MTA_FREQLOCK_SET_CNF_STRU *)pWrrCnfMsg;
    stSndAtFreqlockCnf.ulResult = pstRcvWrrFreqlockCnf->ulResult;

    /* ��at����Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_WRR_FREQLOCK_SET_CNF,
                     sizeof(stSndAtFreqlockCnf),
                     (VOS_UINT8*)&stSndAtFreqlockCnf );

    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_FREQLOCK_SET_CNF);
    return;
}
VOS_VOID TAF_MTA_RcvWrrVersionSetCnf(
    VOS_VOID                           *pWrrCnfMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU                                *pstCmdBuf              = VOS_NULL_PTR;
    WRR_MTA_RRC_VERSION_SET_CNF_STRU                       *pstRcvWrrVersionSetCnf = VOS_NULL_PTR;
    MTA_AT_WRR_RRC_VERSION_SET_CNF_STRU                     stSndAtVersionSetCnf;

    /* �ж϶�ʱ���Ƿ����� */
    if (TAF_MTA_TIMER_STATUS_STOP == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_WRR_RRC_VERSION_SET_CNF))
    {
        MTA_WARNING_LOG("TAF_MTA_RcvWrrVersionSetCnf: WARNING: Timer was already stop!");
        return;
    }

    /* ֹͣ������ʱ�� */
    TAF_MTA_StopTimer(TI_TAF_MTA_WAIT_WRR_RRC_VERSION_SET_CNF);

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_RRC_VERSION_SET_CNF);
    if (VOS_NULL_PTR == pstCmdBuf)
    {
        MTA_WARNING_LOG("TAF_MTA_RcvWrrVersionSetCnf: get command buffer failed!");
        return;
    }

    pstRcvWrrVersionSetCnf        = (WRR_MTA_RRC_VERSION_SET_CNF_STRU *)pWrrCnfMsg;
    stSndAtVersionSetCnf.ulResult = pstRcvWrrVersionSetCnf->ulResult;

    /* ��at����Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_WRR_RRC_VERSION_SET_CNF,
                     sizeof(stSndAtVersionSetCnf),
                     (VOS_UINT8*)&stSndAtVersionSetCnf);

    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_RRC_VERSION_SET_CNF);
    return;
}
VOS_VOID TAF_MTA_RcvWrrCellsrhSetCnf(
    VOS_VOID                           *pWrrCnfMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU                                *pstCmdBuf              = VOS_NULL_PTR;
    WRR_MTA_CELLSRH_SET_CNF_STRU                           *pstRcvWrrCellsrhSetCnf = VOS_NULL_PTR;
    MTA_AT_WRR_CELLSRH_SET_CNF_STRU                         stSndAtCellsrhSetCnf;

    /* �ж϶�ʱ���Ƿ����� */
    if (TAF_MTA_TIMER_STATUS_STOP == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_WRR_CELLSRH_SET_CNF))
    {
        MTA_WARNING_LOG("TAF_MTA_RcvWrrCellsrhSetCnf: WARNING: Timer was already stop!");
        return;
    }

    /* ֹͣ������ʱ�� */
    TAF_MTA_StopTimer(TI_TAF_MTA_WAIT_WRR_CELLSRH_SET_CNF);

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_CELLSRH_SET_CNF);
    if (VOS_NULL_PTR == pstCmdBuf)
    {
        MTA_WARNING_LOG("TAF_MTA_RcvWrrCellsrhSetCnf: get command buffer failed!");
        return;
    }

    pstRcvWrrCellsrhSetCnf            = (WRR_MTA_CELLSRH_SET_CNF_STRU *)pWrrCnfMsg;
    stSndAtCellsrhSetCnf.ulResult     = pstRcvWrrCellsrhSetCnf->ulResult;

    /* ��at����Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_WRR_CELLSRH_SET_CNF,
                     sizeof(stSndAtCellsrhSetCnf),
                     (VOS_UINT8*)&stSndAtCellsrhSetCnf);

    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_CELLSRH_SET_CNF);
    return;
}
VOS_VOID TAF_MTA_RcvWrrFreqlockQryCnf(
    VOS_VOID                           *pWrrCnfMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBuf            = VOS_NULL_PTR;
    WRR_MTA_FREQLOCK_QRY_CNF_STRU      *pstRcvWrrFreqlockCnf = VOS_NULL_PTR;
    MTA_AT_WRR_FREQLOCK_QRY_CNF_STRU    stSndAtFreqlockCnf;

    /* �ж϶�ʱ���Ƿ����� */
    if (TAF_MTA_TIMER_STATUS_STOP == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_WRR_FREQLOCK_QRY_CNF))
    {
        MTA_WARNING_LOG("TAF_MTA_RcvWrrFreqlockQryCnf: WARNING: Timer was already stop!");
        return;
    }

    /* ֹͣ������ʱ�� */
    TAF_MTA_StopTimer(TI_TAF_MTA_WAIT_WRR_FREQLOCK_QRY_CNF);

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_FREQLOCK_QRY_CNF);
    if (VOS_NULL_PTR == pstCmdBuf)
    {
        MTA_WARNING_LOG("TAF_MTA_RcvWrrFreqlockQryCnf: get command buffer failed!");
        return;
    }

    pstRcvWrrFreqlockCnf              = (WRR_MTA_FREQLOCK_QRY_CNF_STRU *)pWrrCnfMsg;
    stSndAtFreqlockCnf.ulResult       = pstRcvWrrFreqlockCnf->ulResult;
    stSndAtFreqlockCnf.stFreqLockInfo.ucFreqLockEnable = pstRcvWrrFreqlockCnf->stFreqLockInfo.ucFreqLockEnable;
    stSndAtFreqlockCnf.stFreqLockInfo.usLockedFreq     = pstRcvWrrFreqlockCnf->stFreqLockInfo.usLockedFreq;
    PS_MEM_SET(stSndAtFreqlockCnf.stFreqLockInfo.aucReserved, 0, sizeof(stSndAtFreqlockCnf.stFreqLockInfo.aucReserved));

    /* ��at����Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_WRR_FREQLOCK_QRY_CNF,
                     sizeof(stSndAtFreqlockCnf),
                     (VOS_UINT8*)&stSndAtFreqlockCnf );

    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_FREQLOCK_QRY_CNF);
    return;
}
VOS_VOID TAF_MTA_RcvWrrVersionQryCnf(
    VOS_VOID                           *pWrrCnfMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBuf              = VOS_NULL_PTR;
    WRR_MTA_RRC_VERSION_QRY_CNF_STRU   *pstRcvWrrVersionQryCnf = VOS_NULL_PTR;
    MTA_AT_WRR_RRC_VERSION_QRY_CNF_STRU stSndAtVersionQryCnf;

    /* �ж϶�ʱ���Ƿ����� */
    if (TAF_MTA_TIMER_STATUS_STOP == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_WRR_RRC_VERSION_QRY_CNF))
    {
        MTA_WARNING_LOG("TAF_MTA_RcvWrrVersionQryCnf: WARNING: Timer was already stop!");
        return;
    }

    /* ֹͣ������ʱ�� */
    TAF_MTA_StopTimer(TI_TAF_MTA_WAIT_WRR_RRC_VERSION_QRY_CNF);

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_RRC_VERSION_QRY_CNF);
    if (VOS_NULL_PTR == pstCmdBuf)
    {
        MTA_WARNING_LOG("TAF_MTA_RcvWrrVersionQryCnf: get command buffer failed!");
        return;
    }

    pstRcvWrrVersionQryCnf            = (WRR_MTA_RRC_VERSION_QRY_CNF_STRU *)pWrrCnfMsg;
    stSndAtVersionQryCnf.ulResult     = pstRcvWrrVersionQryCnf->ulResult;
    stSndAtVersionQryCnf.ucRrcVersion = pstRcvWrrVersionQryCnf->ucRrcVersion;
    PS_MEM_SET((stSndAtVersionQryCnf.aucReserved), 0, sizeof(stSndAtVersionQryCnf.aucReserved));

    /* ��at����Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_WRR_RRC_VERSION_QRY_CNF,
                     sizeof(stSndAtVersionQryCnf),
                     (VOS_UINT8*)&stSndAtVersionQryCnf);

    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_RRC_VERSION_QRY_CNF);
    return;
}
VOS_VOID TAF_MTA_RcvWrrCellsrhQryCnf(
    VOS_VOID                           *pWrrCnfMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBuf              = VOS_NULL_PTR;
    WRR_MTA_CELLSRH_QRY_CNF_STRU       *pstRcvWrrCellsrhQryCnf = VOS_NULL_PTR;
    MTA_AT_WRR_CELLSRH_QRY_CNF_STRU     stSndAtCellsrhQryCnf;

    /* �ж϶�ʱ���Ƿ����� */
    if (TAF_MTA_TIMER_STATUS_STOP == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_WRR_CELLSRH_QRY_CNF))
    {
        MTA_WARNING_LOG("TAF_MTA_RcvWrrCellsrhQryCnf: WARNING: Timer was already stop!");
        return;
    }

    /* ֹͣ������ʱ�� */
    TAF_MTA_StopTimer(TI_TAF_MTA_WAIT_WRR_CELLSRH_QRY_CNF);

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_CELLSRH_QRY_CNF);
    if (VOS_NULL_PTR == pstCmdBuf)
    {
        MTA_WARNING_LOG("TAF_MTA_RcvWrrCellsrhQryCnf: get command buffer failed!");
        return;
    }

    pstRcvWrrCellsrhQryCnf                = (WRR_MTA_CELLSRH_QRY_CNF_STRU *)pWrrCnfMsg;
    stSndAtCellsrhQryCnf.ulResult         = pstRcvWrrCellsrhQryCnf->ulResult;
    stSndAtCellsrhQryCnf.ucCellSearchType = pstRcvWrrCellsrhQryCnf->ucCellSearchType;
    PS_MEM_SET((stSndAtCellsrhQryCnf.aucReserve), 0, sizeof(stSndAtCellsrhQryCnf.aucReserve));

    /* ��at����Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_WRR_CELLSRH_QRY_CNF,
                     sizeof(stSndAtCellsrhQryCnf),
                     (VOS_UINT8*)&stSndAtCellsrhQryCnf);

    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_CELLSRH_QRY_CNF);
    return;
}
VOS_VOID TAF_MTA_RcvRrcSetNCellMonitorCnf(
    VOS_VOID                           *pMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU                                *pstCmdBuf       = VOS_NULL_PTR;
    GRR_MTA_NCELL_MONITOR_SET_CNF_STRU                     *pstGrrMtaSetCnf = VOS_NULL_PTR;
    MTA_AT_UNSOLICITED_RPT_SET_CNF_STRU                     stMtaAtSetCnf;

    /* �ж϶�ʱ���Ƿ����� */
    if (TAF_MTA_TIMER_STATUS_STOP == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_SET_CNF))
    {
        MTA_WARNING_LOG("TAF_MTA_RcvRrcSetNCellMonitorCnf: WARNING: Timer was already stop!");
        return;
    }

    /* ֹͣ������ʱ�� */
    TAF_MTA_StopTimer(TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_SET_CNF);

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_SET_CNF);
    if (VOS_NULL_PTR == pstCmdBuf)
    {
        MTA_WARNING_LOG("TAF_MTA_RcvRrcSetNCellMonitorCnf: get command buffer failed!");
        return;
    }

    PS_MEM_SET(&stMtaAtSetCnf, 0, sizeof(stMtaAtSetCnf));
    pstGrrMtaSetCnf        = (GRR_MTA_NCELL_MONITOR_SET_CNF_STRU *)pMsg;

    if (VOS_OK == pstGrrMtaSetCnf->ulResult)
    {
        stMtaAtSetCnf.enResult = MTA_AT_RESULT_NO_ERROR;
    }
    else
    {
        stMtaAtSetCnf.enResult = MTA_AT_RESULT_ERROR;
    }

    /* ��at����Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_NCELL_MONITOR_SET_CNF,
                     sizeof(stMtaAtSetCnf),
                     (VOS_UINT8*)&stMtaAtSetCnf);

    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_SET_CNF);
    return;
}
VOS_VOID TAF_MTA_RcvRrcQryNCellMonitorCnf(
    VOS_VOID                           *pMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU                                *pstCmdBuf       = VOS_NULL_PTR;
    GRR_MTA_NCELL_MONITOR_QRY_CNF_STRU                     *pstGrrMtaQryCnf = VOS_NULL_PTR;
    MTA_AT_NCELL_MONITOR_QRY_CNF_STRU                       stMtaAtQryCnf;

    /* �ж϶�ʱ���Ƿ����� */
    if (TAF_MTA_TIMER_STATUS_STOP == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_QRY_CNF))
    {
        MTA_WARNING_LOG("TAF_MTA_RcvRrcQryNCellMonitorCnf: WARNING: Timer was already stop!");
        return;
    }

    /* ֹͣ������ʱ�� */
    TAF_MTA_StopTimer(TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_QRY_CNF);

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_QRY_CNF);
    if (VOS_NULL_PTR == pstCmdBuf)
    {
        MTA_WARNING_LOG("TAF_MTA_RcvRrcQryNCellMonitorCnf: get command buffer failed!");
        return;
    }

    PS_MEM_SET(&stMtaAtQryCnf, 0, sizeof(stMtaAtQryCnf));
    pstGrrMtaQryCnf            = (GRR_MTA_NCELL_MONITOR_QRY_CNF_STRU *)pMsg;
    stMtaAtQryCnf.enResult     = pstGrrMtaQryCnf->ulResult;
    stMtaAtQryCnf.ucSwitch     = pstGrrMtaQryCnf->ucSwitch;
    stMtaAtQryCnf.ucNcellState = pstGrrMtaQryCnf->enNcellState;

    /* ��at����Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_NCELL_MONITOR_QRY_CNF,
                     sizeof(stMtaAtQryCnf),
                     (VOS_UINT8*)&stMtaAtQryCnf);

    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_QRY_CNF);
    return;
}
VOS_VOID TAF_MTA_RcvRrcNCellMonitorInd(
    VOS_VOID                           *pMsg
)
{
    GRR_MTA_NCELL_MONITOR_IND_STRU                     *pstGrrMtaQryCnf = VOS_NULL_PTR;
    MTA_AT_NCELL_MONITOR_IND_STRU                       stMtaAtInd;
    AT_APPCTRL_STRU                                     stAppCtrl;

    /* ��д�����ϱ�����Ϣ���� */
    PS_MEM_SET(&stMtaAtInd, 0, sizeof(stMtaAtInd));
    pstGrrMtaQryCnf        = (GRR_MTA_NCELL_MONITOR_IND_STRU *)pMsg;
    stMtaAtInd.ucNcellState = pstGrrMtaQryCnf->enNcellState;

    /* ��д��Ϣͷ���ϱ�����Ϊ�㲥�¼� */
    PS_MEM_SET(&stAppCtrl, 0, sizeof(stAppCtrl));
    stAppCtrl.usClientId = MTA_CLIENTID_BROADCAST;
    stAppCtrl.ucOpId     = MTA_INVALID_TAB_INDEX;

    /* ��at����Ϣ */
    TAF_MTA_SndAtMsg(&stAppCtrl,
                     ID_MTA_AT_NCELL_MONITOR_IND,
                     sizeof(stMtaAtInd),
                     (VOS_UINT8*)&stMtaAtInd);

    return;
}
VOS_VOID TAF_MTA_RcvRrcJamDetectCnf(
    VOS_VOID                           *pMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBuf       = VOS_NULL_PTR;
    RRC_MTA_JAM_DETECT_CNF_STRU        *pstGrrMtaSetCnf = VOS_NULL_PTR;
    MTA_AT_SET_JAM_DETECT_CNF_STRU      stMtaAtSetCnf;

    /* �ж϶�ʱ���Ƿ����� */
    if (TAF_MTA_TIMER_STATUS_STOP == TAF_MTA_GetTimerStatus(TI_TAF_MTA_WAIT_JAM_DETECT_SET_CNF))
    {
        MTA_WARNING_LOG("TAF_MTA_RcvRrcJamDetectCnf: Timer was already stop!");
        return;
    }

    /* ֹͣ������ʱ�� */
    TAF_MTA_StopTimer(TI_TAF_MTA_WAIT_JAM_DETECT_SET_CNF);

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_JAM_DETECT_SET_CNF);
    if (VOS_NULL_PTR == pstCmdBuf)
    {
        MTA_WARNING_LOG("TAF_MTA_RcvRrcJamDetectCnf: get command buffer failed!");
        return;
    }

    pstGrrMtaSetCnf = (RRC_MTA_JAM_DETECT_CNF_STRU *)pMsg;

    PS_MEM_SET(&stMtaAtSetCnf, 0, sizeof(stMtaAtSetCnf));

    if (MTA_RRC_RESULT_NO_ERROR == pstGrrMtaSetCnf->enResult)
    {
        stMtaAtSetCnf.enResult = MTA_AT_RESULT_NO_ERROR;
    }
    else
    {
        stMtaAtSetCnf.enResult = MTA_AT_RESULT_ERROR;
    }

    /* ��at����Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_SET_JAM_DETECT_CNF,
                     sizeof(stMtaAtSetCnf),
                     (VOS_UINT8*)&stMtaAtSetCnf);

    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(TI_TAF_MTA_WAIT_JAM_DETECT_SET_CNF);
    return;

}
VOS_VOID TAF_MTA_RcvRrcJamDetectInd(
    VOS_VOID                           *pMsg
)
{
    RRC_MTA_JAM_DETECT_IND_STRU        *pstJamDetectInd = VOS_NULL_PTR;
    MTA_AT_JAM_DETECT_IND_STRU          stMtaAtInd;
    AT_APPCTRL_STRU                     stAppCtrl;
    NV_NAS_JAM_DETECT_CFG_STRU          stNvJamDetect;

    /* ��ʼ����Ϣ���� */
    PS_MEM_SET(&stNvJamDetect, 0x0, sizeof(stNvJamDetect));

    pstJamDetectInd = (RRC_MTA_JAM_DETECT_IND_STRU *)pMsg;

    /* ��д�����ϱ�����Ϣ���� */
    PS_MEM_SET(&stMtaAtInd, 0, sizeof(stMtaAtInd));
    stMtaAtInd.enJamResult = (MTA_AT_JAM_RESULT_ENUM_UINT32)pstJamDetectInd->enResult;

    /* ��д��Ϣͷ���ϱ�����Ϊ�㲥�¼� */
    PS_MEM_SET(&stAppCtrl, 0, sizeof(stAppCtrl));
    stAppCtrl.usClientId = MTA_CLIENTID_BROADCAST;
    stAppCtrl.ucOpId     = MTA_INVALID_TAB_INDEX;

    /* ͨ����ȡNV����ȡJam Detect��ǰ����ֵ */
    if (NV_OK != NV_Read(en_NV_Item_JAM_DETECT_CFG,
                         &stNvJamDetect,
                         sizeof(NV_NAS_JAM_DETECT_CFG_STRU)))
    {
        MTA_ERROR_LOG("TAF_MTA_RcvRrcJamDetectInd: NV_Read en_NV_Item_JAM_DETECT_CFG fail!");
        return;
    }

    /* ���jam detect����δʹ�ܣ���ʹ�յ�GAS��Ϣ��Ҳ�����ϱ� */
    if (VOS_FALSE == stNvJamDetect.ucMode)
    {
        MTA_ERROR_LOG("TAF_MTA_RcvRrcJamDetectInd: Jam Detect function is disabled!");
        return ;
    }

    /* ��at����Ϣ */
    TAF_MTA_SndAtMsg(&stAppCtrl,
                     ID_MTA_AT_JAM_DETECT_IND,
                     sizeof(stMtaAtInd),
                     (VOS_UINT8*)&stMtaAtInd);

    return;
}


VOS_VOID TAF_MTA_RcvTiReqAutotestQryExpired(
    VOS_VOID                           *pMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBuf = VOS_NULL_PTR;
    MTA_AT_WRR_AUTOTEST_QRY_CNF_STRU    stQryAutotestCnf;

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_AUTOTEST_QRY_CNF);

    if (VOS_NULL_PTR == pstCmdBuf)
    {
        /*��ʱ����ʱlog*/
        MTA_ERROR_LOG("TAF_MTA_RcvTiReqAutotestQryExpired: get command buffer failed!");
        return;
    }

    PS_MEM_SET(&stQryAutotestCnf, 0x0, sizeof(stQryAutotestCnf));

    stQryAutotestCnf.ulResult = VOS_ERR;

    /* �ϱ���ATģ�������Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_WRR_AUTOTEST_QRY_CNF,
                     sizeof(stQryAutotestCnf),
                     (VOS_UINT8*)&stQryAutotestCnf );


    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(pstCmdBuf->enTimerId);

    return;

}


VOS_VOID TAF_MTA_RcvTiReqCellInfoQryExpired(
    VOS_VOID                           *pMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBuf = VOS_NULL_PTR;
    MTA_AT_WRR_CELLINFO_QRY_CNF_STRU    stCellinfoQryCnf;

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_CELLINFO_QRY_CNF);

    if (VOS_NULL_PTR == pstCmdBuf)
    {
        /*��ʱ����ʱlog*/
        MTA_ERROR_LOG("TAF_MTA_RcvTiReqCellInfoQryExpired: get command buffer failed!");
        return;
    }

    PS_MEM_SET(&stCellinfoQryCnf, 0x0, sizeof(stCellinfoQryCnf));

    stCellinfoQryCnf.ulResult = VOS_ERR;

    /* �ϱ���ATģ�������Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_WRR_CELLINFO_QRY_CNF,
                     sizeof(stCellinfoQryCnf),
                     (VOS_UINT8*)&stCellinfoQryCnf );


    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(pstCmdBuf->enTimerId);

    return;

}




VOS_VOID TAF_MTA_RcvTiReqMeanrptQryExpired(
    VOS_VOID                           *pMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBuf = VOS_NULL_PTR;
    MTA_AT_WRR_MEANRPT_QRY_CNF_STRU     stMeanrptQryCnf;

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_MEANRPT_QRY_CNF);

    if (VOS_NULL_PTR == pstCmdBuf)
    {
        /*��ʱ����ʱlog*/
        MTA_ERROR_LOG("TAF_MTA_RcvTiReqMeanrptQryExpired: get command buffer failed!");
        return;
    }

    PS_MEM_SET(&stMeanrptQryCnf, 0x0, sizeof(stMeanrptQryCnf));

    stMeanrptQryCnf.ulResult = VOS_ERR;

    /* �ϱ���ATģ�������Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_WRR_MEANRPT_QRY_CNF,
                     sizeof(stMeanrptQryCnf),
                     (VOS_UINT8*)&stMeanrptQryCnf );


    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(pstCmdBuf->enTimerId);

    return;

}


VOS_VOID TAF_MTA_RcvTiReqFreqlockSetExpired(
    VOS_VOID                           *pMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBuf = VOS_NULL_PTR;
    MTA_AT_WRR_FREQLOCK_SET_CNF_STRU    stSetFreqlockCnf;

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_FREQLOCK_SET_CNF);

    if (VOS_NULL_PTR == pstCmdBuf)
    {
        /*��ʱ����ʱlog*/
        MTA_ERROR_LOG("TAF_MTA_RcvTiReqFreqlockSetExpired: get command buffer failed!");
        return;
    }

    PS_MEM_SET(&stSetFreqlockCnf, 0x0, sizeof(stSetFreqlockCnf));

    stSetFreqlockCnf.ulResult = VOS_ERR;

    /* �ϱ���ATģ�������Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_WRR_FREQLOCK_SET_CNF,
                     sizeof(stSetFreqlockCnf),
                     (VOS_UINT8*)&stSetFreqlockCnf );


    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(pstCmdBuf->enTimerId);

    return;
}


VOS_VOID TAF_MTA_RcvTiReqRrcVersionSetExpired(
    VOS_VOID                           *pMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU                                *pstCmdBuf = VOS_NULL_PTR;
    MTA_AT_WRR_RRC_VERSION_SET_CNF_STRU                     stVersionSetCnf;

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_RRC_VERSION_SET_CNF);

    if (VOS_NULL_PTR == pstCmdBuf)
    {
        /*��ʱ����ʱlog*/
        MTA_ERROR_LOG("TAF_MTA_RcvTiReqRrcVersionSetExpired: get command buffer failed!");
        return;
    }

    PS_MEM_SET(&stVersionSetCnf, 0x0, sizeof(stVersionSetCnf));

    stVersionSetCnf.ulResult = VOS_ERR;

    /* �ϱ���ATģ�������Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_WRR_RRC_VERSION_SET_CNF,
                     sizeof(stVersionSetCnf),
                     (VOS_UINT8*)&stVersionSetCnf );


    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(pstCmdBuf->enTimerId);

    return;

}




VOS_VOID TAF_MTA_RcvTiReqCellsrhSetExpired(
    VOS_VOID                           *pMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU                                *pstCmdBuf = VOS_NULL_PTR;
    MTA_AT_WRR_CELLSRH_SET_CNF_STRU                         stCellsrhSetCnf;

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_CELLSRH_SET_CNF);

    if (VOS_NULL_PTR == pstCmdBuf)
    {
        /*��ʱ����ʱlog*/
        MTA_ERROR_LOG("TAF_MTA_RcvTiReqCellsrhSetExpired: get command buffer failed!");
        return;
    }

    PS_MEM_SET(&stCellsrhSetCnf, 0x0, sizeof(stCellsrhSetCnf));

    stCellsrhSetCnf.ulResult = VOS_ERR;

    /* �ϱ���ATģ�������Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_WRR_CELLSRH_SET_CNF,
                     sizeof(stCellsrhSetCnf),
                     (VOS_UINT8*)&stCellsrhSetCnf );


    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(pstCmdBuf->enTimerId);

    return;

}



VOS_VOID TAF_MTA_RcvTiReqFreqlockQryExpired(
    VOS_VOID                           *pMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBuf = VOS_NULL_PTR;
    MTA_AT_WRR_FREQLOCK_QRY_CNF_STRU    stQryFreqlockCnf;

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_FREQLOCK_QRY_CNF);

    if (VOS_NULL_PTR == pstCmdBuf)
    {
        /*��ʱ����ʱlog*/
        MTA_ERROR_LOG("TAF_MTA_RcvTiReqFreqlockQryExpired: get command buffer failed!");
        return;
    }

    PS_MEM_SET(&stQryFreqlockCnf, 0x0, sizeof(stQryFreqlockCnf));

    stQryFreqlockCnf.ulResult = VOS_ERR;

    /* �ϱ���ATģ�������Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_WRR_FREQLOCK_QRY_CNF,
                     sizeof(stQryFreqlockCnf),
                     (VOS_UINT8*)&stQryFreqlockCnf );


    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(pstCmdBuf->enTimerId);

    return;
}


VOS_VOID TAF_MTA_RcvTiReqRrcVersionQryExpired(
    VOS_VOID                           *pMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU                                *pstCmdBuf = VOS_NULL_PTR;
    MTA_AT_WRR_RRC_VERSION_QRY_CNF_STRU                     stVersionQryCnf;

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_RRC_VERSION_QRY_CNF);

    if (VOS_NULL_PTR == pstCmdBuf)
    {
        /*��ʱ����ʱlog*/
        MTA_ERROR_LOG("TAF_MTA_RcvTiReqRrcVersionQryExpired: get command buffer failed!");
        return;
    }

    PS_MEM_SET(&stVersionQryCnf, 0x0, sizeof(stVersionQryCnf));

    stVersionQryCnf.ulResult = VOS_ERR;

    /* �ϱ���ATģ�������Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_WRR_RRC_VERSION_QRY_CNF,
                     sizeof(stVersionQryCnf),
                     (VOS_UINT8*)&stVersionQryCnf );


    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(pstCmdBuf->enTimerId);

    return;

}




VOS_VOID TAF_MTA_RcvTiReqCellsrhQryExpired(
    VOS_VOID                           *pMsg
)
{
    TAF_MTA_CMD_BUFFER_STRU                                *pstCmdBuf = VOS_NULL_PTR;
    MTA_AT_WRR_CELLSRH_QRY_CNF_STRU                         stCellsrhQryCnf;

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_WRR_CELLSRH_QRY_CNF);

    if (VOS_NULL_PTR == pstCmdBuf)
    {
        /*��ʱ����ʱlog*/
        MTA_ERROR_LOG("TAF_MTA_RcvTiReqCellsrhQryExpired: get command buffer failed!");
        return;
    }

    PS_MEM_SET(&stCellsrhQryCnf, 0x0, sizeof(stCellsrhQryCnf));

    stCellsrhQryCnf.ulResult = VOS_ERR;

    /* �ϱ���ATģ�������Ϣ */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_WRR_CELLSRH_QRY_CNF,
                     sizeof(stCellsrhQryCnf),
                     (VOS_UINT8*)&stCellsrhQryCnf );


    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(pstCmdBuf->enTimerId);

    return;

}


VOS_VOID TAF_MTA_RcvTiWaitGrrSetNCellMonitorExpired(VOS_VOID *pMsg)
{
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBuf   = VOS_NULL_PTR;
    MTA_AT_RESULT_CNF_STRU              stSetCnf;

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_SET_CNF);

    if (VOS_NULL_PTR == pstCmdBuf)
    {
        return;
    }

    PS_MEM_SET(&stSetCnf, 0x0, sizeof(stSetCnf));

    stSetCnf.enResult = MTA_AT_RESULT_ERROR;

    /* �ϱ���ATģ�����ô��� */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_NCELL_MONITOR_SET_CNF,
                     sizeof(stSetCnf),
                     (VOS_UINT8*)&stSetCnf );

    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_SET_CNF);

    return;
}
VOS_VOID TAF_MTA_RcvTiWaitGrrQryNCellMonitorExpired(VOS_VOID *pMsg)
{
    TAF_MTA_CMD_BUFFER_STRU                *pstCmdBuf   = VOS_NULL_PTR;
    MTA_AT_NCELL_MONITOR_QRY_CNF_STRU       stQryCnf;

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_QRY_CNF);

    if (VOS_NULL_PTR == pstCmdBuf)
    {
        return;
    }

    PS_MEM_SET(&stQryCnf, 0x0, sizeof(stQryCnf));

    stQryCnf.enResult = MTA_AT_RESULT_ERROR;

    /* �ϱ���ATģ���ѯ���� */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_NCELL_MONITOR_QRY_CNF,
                     sizeof(stQryCnf),
                     (VOS_UINT8*)&stQryCnf );

    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_QRY_CNF);

    return;
}
VOS_VOID TAF_MTA_RcvTiWaitGrrSetJamDetectExpired(VOS_VOID *pMsg)
{
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBuf   = VOS_NULL_PTR;
    MTA_AT_SET_JAM_DETECT_CNF_STRU      stQryCnf;

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(TI_TAF_MTA_WAIT_JAM_DETECT_SET_CNF);

    if (VOS_NULL_PTR == pstCmdBuf)
    {
        return;
    }

    PS_MEM_SET(&stQryCnf, 0x0, sizeof(stQryCnf));

    stQryCnf.enResult = MTA_AT_RESULT_ERROR;

    /* �ϱ���ATģ���ѯ���� */
    TAF_MTA_SndAtMsg((AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                     ID_MTA_AT_SET_JAM_DETECT_CNF,
                     sizeof(stQryCnf),
                     (VOS_UINT8*)&stQryCnf );

    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(TI_TAF_MTA_WAIT_JAM_DETECT_SET_CNF);

    return;
}
VOS_UINT32 TAF_MTA_SndWrrQryAutotestReqMsg(
    MTA_WRR_AUTOTEST_PARA_STRU         *pAutotestQryPara
)
{
    VOS_UINT32                          ulLength;
    MTA_WRR_AUTOTEST_QRY_REQ_STRU      *pstMtaWrrQryReq = VOS_NULL_PTR;

    /* ������Ϣ�ṹ�ڴ� */
    ulLength              = sizeof(MTA_WRR_AUTOTEST_QRY_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMtaWrrQryReq       = (MTA_WRR_AUTOTEST_QRY_REQ_STRU *)PS_ALLOC_MSG(UEPS_PID_MTA, ulLength);

    if (VOS_NULL_PTR == pstMtaWrrQryReq)
    {
        MTA_ERROR_LOG("TAF_MTA_SndWrrQryAutotestReqMsg: Alloc msg fail!");
        return VOS_ERR;
    }

    /* ������Ϣ�ṹ�� */
    pstMtaWrrQryReq->ulSenderCpuId     = VOS_LOCAL_CPUID;
    pstMtaWrrQryReq->ulSenderPid       = UEPS_PID_MTA;
    pstMtaWrrQryReq->ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstMtaWrrQryReq->ulReceiverPid     = WUEPS_PID_WRR;
    pstMtaWrrQryReq->ulMsgName         = ID_MTA_WRR_AUTOTEST_QRY_REQ;

    /* ���ݸ���Ϣ������ */
    PS_MEM_CPY(&(pstMtaWrrQryReq->stWrrAutotestPara),
        pAutotestQryPara,
        sizeof(MTA_WRR_AUTOTEST_PARA_STRU));

    /* ������Ϣ��AS */
    if ( VOS_OK != PS_SEND_MSG(UEPS_PID_MTA, pstMtaWrrQryReq) )
    {
        MTA_ERROR_LOG("TAF_MTA_SndWrrQryAutotestReqMsg: Send msg fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 TAF_MTA_SndWrrQryCellinfoReqMsg(
    VOS_UINT32                          ulSetCellInfo
)
{
    VOS_UINT32                          ulLength;
    MTA_WRR_CELLINFO_QRY_REQ_STRU      *pstMtaWrrQryCellinfoReq = VOS_NULL_PTR;

    /* ������Ϣ�ṹ�ڴ� */
    ulLength                    = sizeof(MTA_WRR_CELLINFO_QRY_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMtaWrrQryCellinfoReq     = (MTA_WRR_CELLINFO_QRY_REQ_STRU*)PS_ALLOC_MSG(UEPS_PID_MTA, ulLength);
    if (VOS_NULL_PTR == pstMtaWrrQryCellinfoReq)
    {
        MTA_ERROR_LOG("TAF_MTA_SndWrrQryCellinfoReqMsg: Alloc msg fail!");
        return VOS_ERR;
    }

    /* ������Ϣ�ṹ�� */
    pstMtaWrrQryCellinfoReq->ulSenderCpuId     = VOS_LOCAL_CPUID;
    pstMtaWrrQryCellinfoReq->ulSenderPid       = UEPS_PID_MTA;
    pstMtaWrrQryCellinfoReq->ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstMtaWrrQryCellinfoReq->ulReceiverPid     = WUEPS_PID_WRR;
    pstMtaWrrQryCellinfoReq->ulMsgName         = ID_MTA_WRR_CELLINFO_QRY_REQ;
    pstMtaWrrQryCellinfoReq->ulSetCellInfo     = ulSetCellInfo;

    /* ������Ϣ��AS */
    if (VOS_OK != PS_SEND_MSG(UEPS_PID_MTA, pstMtaWrrQryCellinfoReq))
    {
        MTA_ERROR_LOG("TAF_MTA_SndWrrQryCellinfoReqMsg: Send msg fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 TAF_MTA_SndWrrQryMeanrptReqMsg( VOS_VOID )
{
    VOS_UINT32                          ulLength;
    MTA_WRR_MEANRPT_QRY_REQ_STRU       *pstMtaWrrQryMeanrptReq = VOS_NULL_PTR;

    /* ������Ϣ�ṹ�ڴ� */
    ulLength                    = sizeof(MTA_WRR_MEANRPT_QRY_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMtaWrrQryMeanrptReq      = (MTA_WRR_MEANRPT_QRY_REQ_STRU*)PS_ALLOC_MSG(UEPS_PID_MTA, ulLength);
    if (VOS_NULL_PTR == pstMtaWrrQryMeanrptReq)
    {
        MTA_ERROR_LOG("TAF_MTA_SndWrrQryMeanrptReqMsg: Alloc msg fail!");
        return VOS_ERR;
    }

    /* ������Ϣ�ṹ�� */
    pstMtaWrrQryMeanrptReq->ulSenderCpuId     = VOS_LOCAL_CPUID;
    pstMtaWrrQryMeanrptReq->ulSenderPid       = UEPS_PID_MTA;
    pstMtaWrrQryMeanrptReq->ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstMtaWrrQryMeanrptReq->ulReceiverPid     = WUEPS_PID_WRR;
    pstMtaWrrQryMeanrptReq->ulMsgName         = ID_MTA_WRR_MEASRPT_QRY_REQ;

    /* ������Ϣ��AS */
    if (VOS_OK != PS_SEND_MSG(UEPS_PID_MTA, pstMtaWrrQryMeanrptReq))
    {
        MTA_ERROR_LOG("TAF_MTA_SndWrrQryMeanrptReqMsg: Send msg fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 TAF_MTA_SndWrrSetFreqlockReqMsg(
    MTA_WRR_FREQLOCK_CTRL_STRU         *pFreqlockCtrlPara
)
{
    VOS_UINT32                          ulLength;
    MTA_WRR_FREQLOCK_SET_REQ_STRU      *pstMtaWrrSetReq = VOS_NULL_PTR;

    /* ������Ϣ�ṹ�ڴ� */
    ulLength              = sizeof(MTA_WRR_FREQLOCK_SET_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMtaWrrSetReq       = (MTA_WRR_FREQLOCK_SET_REQ_STRU *)PS_ALLOC_MSG(UEPS_PID_MTA, ulLength);

    if (VOS_NULL_PTR == pstMtaWrrSetReq)
    {
        MTA_ERROR_LOG("TAF_MTA_SndWrrSetFreqlockReqMsg: Alloc msg fail!");
        return VOS_ERR;
    }

    /* ������Ϣ�ṹ�� */
    pstMtaWrrSetReq->ulSenderCpuId     = VOS_LOCAL_CPUID;
    pstMtaWrrSetReq->ulSenderPid       = UEPS_PID_MTA;
    pstMtaWrrSetReq->ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstMtaWrrSetReq->ulReceiverPid     = WUEPS_PID_WRR;
    pstMtaWrrSetReq->ulMsgName         = ID_MTA_WRR_FREQLOCK_SET_REQ;

    /* ���ݸ���Ϣ������ */
    PS_MEM_CPY(&(pstMtaWrrSetReq->stFrelock), pFreqlockCtrlPara, sizeof(MTA_WRR_FREQLOCK_CTRL_STRU));

    /* ������Ϣ��AS */
    if ( VOS_OK != PS_SEND_MSG(UEPS_PID_MTA, pstMtaWrrSetReq) )
    {
        MTA_ERROR_LOG("TAF_MTA_SndWrrSetFreqlockReqMsg: Send msg fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 TAF_MTA_SndWrrSetVersionReqMsg(
    VOS_UINT8                           ucRrcVersion
)
{
    VOS_UINT32                          ulLength;
    MTA_WRR_RRC_VERSION_SET_REQ_STRU   *pstMtaWrrSetReq = VOS_NULL_PTR;

    /* ������Ϣ�ṹ�ڴ� */
    ulLength              = sizeof(MTA_WRR_RRC_VERSION_SET_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMtaWrrSetReq       = (MTA_WRR_RRC_VERSION_SET_REQ_STRU *)PS_ALLOC_MSG(UEPS_PID_MTA, ulLength);

    if (VOS_NULL_PTR == pstMtaWrrSetReq)
    {
        MTA_ERROR_LOG("TAF_MTA_SndWrrSetVersionReqMsg: Alloc msg fail!");
        return VOS_ERR;
    }

    /* ������Ϣ�ṹ�� */
    pstMtaWrrSetReq->ulSenderCpuId     = VOS_LOCAL_CPUID;
    pstMtaWrrSetReq->ulSenderPid       = UEPS_PID_MTA;
    pstMtaWrrSetReq->ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstMtaWrrSetReq->ulReceiverPid     = WUEPS_PID_WRR;
    pstMtaWrrSetReq->ulMsgName         = ID_MTA_WRR_RRC_VERSION_SET_REQ;
    pstMtaWrrSetReq->ucRRCVersion      = ucRrcVersion;

    /* ���ݸ���Ϣ������ */
    PS_MEM_SET((pstMtaWrrSetReq->aucReserv), 0, sizeof(pstMtaWrrSetReq->aucReserv));

    /* ������Ϣ��AS */
    if ( VOS_OK != PS_SEND_MSG(UEPS_PID_MTA, pstMtaWrrSetReq) )
    {
        MTA_ERROR_LOG("TAF_MTA_SndWrrSetVersionReqMsg: Send msg fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 TAF_MTA_SndWrrSetCellsrhReqMsg(
    VOS_UINT8                           ucCellSrh
)
{
    VOS_UINT32                          ulLength;
    MTA_WRR_CELLSRH_SET_REQ_STRU       *pstMtaWrrSetReq = VOS_NULL_PTR;

    /* ������Ϣ�ṹ�ڴ� */
    ulLength              = sizeof(MTA_WRR_CELLSRH_SET_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMtaWrrSetReq       = (MTA_WRR_CELLSRH_SET_REQ_STRU *)PS_ALLOC_MSG(UEPS_PID_MTA, ulLength);

    if (VOS_NULL_PTR == pstMtaWrrSetReq)
    {
        MTA_ERROR_LOG("TAF_MTA_SndWrrSetCellsrhReqMsg: Alloc msg fail!");
        return VOS_ERR;
    }

    /* ������Ϣ�ṹ�� */
    pstMtaWrrSetReq->ulSenderCpuId     = VOS_LOCAL_CPUID;
    pstMtaWrrSetReq->ulSenderPid       = UEPS_PID_MTA;
    pstMtaWrrSetReq->ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstMtaWrrSetReq->ulReceiverPid     = WUEPS_PID_WRR;
    pstMtaWrrSetReq->ulMsgName         = ID_MTA_WRR_CELLSRH_SET_REQ;
    pstMtaWrrSetReq->ucCellSrh         = ucCellSrh;

    /* ���ݸ���Ϣ������ */
    PS_MEM_SET((pstMtaWrrSetReq->aucReserve), 0, sizeof(pstMtaWrrSetReq->aucReserve));

    /* ������Ϣ��AS */
    if ( VOS_OK != PS_SEND_MSG(UEPS_PID_MTA, pstMtaWrrSetReq) )
    {
        MTA_ERROR_LOG("TAF_MTA_SndWrrSetCellsrhReqMsg: Send msg fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}



VOS_UINT32 TAF_MTA_SndWrrQryFreqlockReqMsg(VOS_VOID)
{

    VOS_UINT32                          ulLength;
    MTA_WRR_FREQLOCK_QRY_REQ_STRU      *pstMtaWrrQryReq = VOS_NULL_PTR;

    /* ������Ϣ�ṹ�ڴ� */
    ulLength              = sizeof(MTA_WRR_FREQLOCK_QRY_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMtaWrrQryReq       = (MTA_WRR_FREQLOCK_QRY_REQ_STRU *)PS_ALLOC_MSG(UEPS_PID_MTA, ulLength);

    if (VOS_NULL_PTR == pstMtaWrrQryReq)
    {
        MTA_ERROR_LOG("TAF_MTA_SndWrrQryFreqlockReqMsg: Alloc msg fail!");
        return VOS_ERR;
    }

    /* ������Ϣ�ṹ�� */
    pstMtaWrrQryReq->ulSenderCpuId     = VOS_LOCAL_CPUID;
    pstMtaWrrQryReq->ulSenderPid       = UEPS_PID_MTA;
    pstMtaWrrQryReq->ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstMtaWrrQryReq->ulReceiverPid     = WUEPS_PID_WRR;
    pstMtaWrrQryReq->ulMsgName         = ID_MTA_WRR_FREQLOCK_QRY_REQ;

    /* ������Ϣ��AS */
    if ( VOS_OK != PS_SEND_MSG(UEPS_PID_MTA, pstMtaWrrQryReq) )
    {
        MTA_ERROR_LOG("TAF_MTA_SndWrrQryFreqlockReqMsg: Send msg fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 TAF_MTA_SndWrrQryRrcVersionReqMsg( VOS_VOID )
{
    VOS_UINT32                          ulLength;
    MTA_WRR_RRC_VERSION_QRY_REQ_STRU   *pstMtaWrrQryVersionReq = VOS_NULL_PTR;

    /* ������Ϣ�ṹ�ڴ� */
    ulLength                    = sizeof(MTA_WRR_RRC_VERSION_QRY_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMtaWrrQryVersionReq      = (MTA_WRR_RRC_VERSION_QRY_REQ_STRU*)PS_ALLOC_MSG(UEPS_PID_MTA, ulLength);
    if (VOS_NULL_PTR == pstMtaWrrQryVersionReq)
    {
        MTA_ERROR_LOG("TAF_MTA_SndWrrQryRrcVersionReqMsg: Alloc msg fail!");
        return VOS_ERR;
    }

    /* ������Ϣ�ṹ�� */
    pstMtaWrrQryVersionReq->ulSenderCpuId     = VOS_LOCAL_CPUID;
    pstMtaWrrQryVersionReq->ulSenderPid       = UEPS_PID_MTA;
    pstMtaWrrQryVersionReq->ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstMtaWrrQryVersionReq->ulReceiverPid     = WUEPS_PID_WRR;
    pstMtaWrrQryVersionReq->ulMsgName         = ID_MTA_WRR_RRC_VERSION_QRY_REQ;

    /* ������Ϣ��AS */
    if (VOS_OK != PS_SEND_MSG(UEPS_PID_MTA, pstMtaWrrQryVersionReq))
    {
        MTA_ERROR_LOG("TAF_MTA_SndWrrQryRrcVersionReqMsg: Send msg fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 TAF_MTA_SndWrrQryCellsrhReqMsg( VOS_VOID )
{
    VOS_UINT32                          ulLength;
    MTA_WRR_CELLSRH_QRY_REQ_STRU       *pstMtaWrrQryCellsrhReq = VOS_NULL_PTR;

    /* ������Ϣ�ṹ�ڴ� */
    ulLength                    = sizeof(MTA_WRR_CELLSRH_QRY_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMtaWrrQryCellsrhReq      = (MTA_WRR_CELLSRH_QRY_REQ_STRU*)PS_ALLOC_MSG(UEPS_PID_MTA, ulLength);
    if (VOS_NULL_PTR == pstMtaWrrQryCellsrhReq)
    {
        MTA_ERROR_LOG("TAF_MTA_SndWrrQryCellsrhReqMsg: Alloc msg fail!");
        return VOS_ERR;
    }

    /* ������Ϣ�ṹ�� */
    pstMtaWrrQryCellsrhReq->ulSenderCpuId     = VOS_LOCAL_CPUID;
    pstMtaWrrQryCellsrhReq->ulSenderPid       = UEPS_PID_MTA;
    pstMtaWrrQryCellsrhReq->ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstMtaWrrQryCellsrhReq->ulReceiverPid     = WUEPS_PID_WRR;
    pstMtaWrrQryCellsrhReq->ulMsgName         = ID_MTA_WRR_CELLSRH_QRY_REQ;

    /* ������Ϣ��AS */
    if (VOS_OK != PS_SEND_MSG(UEPS_PID_MTA, pstMtaWrrQryCellsrhReq))
    {
        MTA_ERROR_LOG("TAF_MTA_SndWrrQryCellsrhReqMsg: Send msg fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 TAF_MTA_SndGrrNCellMonitorSetReq(
    VOS_UINT8                         ucRptCmdStatus
)
{
    MTA_GRR_NCELL_MONITOR_SET_REQ_STRU          *pstNCellMonitorSetReq  = VOS_NULL_PTR;

    /* �����ڴ� */
    pstNCellMonitorSetReq = (MTA_GRR_NCELL_MONITOR_SET_REQ_STRU *)PS_ALLOC_MSG(UEPS_PID_MTA,
                                                              sizeof(MTA_GRR_NCELL_MONITOR_SET_REQ_STRU) - VOS_MSG_HEAD_LENGTH);

    /* �ڴ�����쳣���� */
    if (VOS_NULL_PTR == pstNCellMonitorSetReq)
    {
        MTA_WARNING_LOG("TAF_MTA_SndGrrNCellMonitorSetReq:alloc msg failed.");
        return VOS_ERR;
    }

    /* ��д����Ϣ���� */
    PS_MEM_SET(pstNCellMonitorSetReq, 0, sizeof(MTA_GRR_NCELL_MONITOR_SET_REQ_STRU));
    pstNCellMonitorSetReq->ulSenderPid      = UEPS_PID_MTA;
    pstNCellMonitorSetReq->ulReceiverCpuId  = VOS_LOCAL_CPUID;
    pstNCellMonitorSetReq->ulSenderCpuId    = VOS_LOCAL_CPUID;
    pstNCellMonitorSetReq->ulReceiverPid    = UEPS_PID_GAS;
    pstNCellMonitorSetReq->ulMsgName        = ID_MTA_GRR_NCELL_MONITOR_SET_REQ;
    pstNCellMonitorSetReq->ulLength         = sizeof(MTA_GRR_NCELL_MONITOR_SET_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    pstNCellMonitorSetReq->ucSwitch         = ucRptCmdStatus;

    /* ������Ϣ�쳣���� */
    if (VOS_OK != PS_SEND_MSG(UEPS_PID_MTA, pstNCellMonitorSetReq))
    {
        MTA_WARNING_LOG("TAF_MTA_SndGrrNCellMonitorSetReq: PS_SEND_MSG ERROR");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 TAF_MTA_SndGrrNCellMonitorQryReq(VOS_VOID)
{
    MTA_GRR_NCELL_MONITOR_QRY_REQ_STRU          *pstNCellMonitorQryReq  = VOS_NULL_PTR;

    /* �����ڴ� */
    pstNCellMonitorQryReq = (MTA_GRR_NCELL_MONITOR_QRY_REQ_STRU *)PS_ALLOC_MSG(UEPS_PID_MTA,
                                                              sizeof(MTA_GRR_NCELL_MONITOR_QRY_REQ_STRU) - VOS_MSG_HEAD_LENGTH);

    /* �ڴ�����쳣���� */
    if (VOS_NULL_PTR == pstNCellMonitorQryReq)
    {
        MTA_WARNING_LOG("TAF_MTA_SndGrrNCellMonitorQryReq:alloc msg failed.");
        return VOS_ERR;
    }

    /* ��д����Ϣ���� */
    PS_MEM_SET(pstNCellMonitorQryReq, 0, sizeof(MTA_GRR_NCELL_MONITOR_QRY_REQ_STRU));
    pstNCellMonitorQryReq->ulSenderPid      = UEPS_PID_MTA;
    pstNCellMonitorQryReq->ulReceiverCpuId  = VOS_LOCAL_CPUID;
    pstNCellMonitorQryReq->ulSenderCpuId    = VOS_LOCAL_CPUID;
    pstNCellMonitorQryReq->ulReceiverPid    = UEPS_PID_GAS;
    pstNCellMonitorQryReq->ulMsgName        = ID_MTA_GRR_NCELL_MONITOR_QRY_REQ;
    pstNCellMonitorQryReq->ulLength         = sizeof(MTA_GRR_NCELL_MONITOR_QRY_REQ_STRU) - VOS_MSG_HEAD_LENGTH;

    /* ������Ϣ�쳣���� */
    if (VOS_OK != PS_SEND_MSG(UEPS_PID_MTA, pstNCellMonitorQryReq))
    {
        MTA_WARNING_LOG("TAF_MTA_SndGrrNCellMonitorQryReq: PS_SEND_MSG ERROR");
        return VOS_ERR;
    }

    return VOS_OK;
}



VOS_UINT32 TAF_MTA_SndGrrSetJamDetectReq(
    AT_MTA_SET_JAM_DETECT_REQ_STRU     *pstSetJdrReq
)
{
    VOS_UINT32                          ulLength;
    MTA_RRC_JAM_DETECT_REQ_STRU        *pstMtaGrrJamDetectReq = VOS_NULL_PTR;

    /* ������Ϣ�ṹ�ڴ� */
    ulLength              = sizeof(MTA_RRC_JAM_DETECT_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMtaGrrJamDetectReq = (MTA_RRC_JAM_DETECT_REQ_STRU*)PS_ALLOC_MSG(UEPS_PID_MTA, ulLength);
    if (VOS_NULL_PTR == pstMtaGrrJamDetectReq)
    {
        MTA_ERROR_LOG("TAF_MTA_SndGrrSetJamDetectReq: Alloc msg fail!");
        return VOS_ERR;
    }

    /* ������Ϣ�ṹ�� */
    pstMtaGrrJamDetectReq->stMsgHeader.ulSenderCpuId     = VOS_LOCAL_CPUID;
    pstMtaGrrJamDetectReq->stMsgHeader.ulSenderPid       = UEPS_PID_MTA;
    pstMtaGrrJamDetectReq->stMsgHeader.ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstMtaGrrJamDetectReq->stMsgHeader.ulReceiverPid     = UEPS_PID_GAS;
    pstMtaGrrJamDetectReq->stMsgHeader.ulLength          = ulLength;
    pstMtaGrrJamDetectReq->stMsgHeader.ulMsgName         = ID_MTA_RRC_JAM_DETECT_REQ;

    pstMtaGrrJamDetectReq->enMode      = (MTA_RRC_JAM_MODE_ENUM_UINT8)pstSetJdrReq->ucFlag;
    pstMtaGrrJamDetectReq->ucMethod    = pstSetJdrReq->ucMethod;
    pstMtaGrrJamDetectReq->ucThreshold = pstSetJdrReq->ucThreshold;
    pstMtaGrrJamDetectReq->ucFreqNum   = pstSetJdrReq->ucFreqNum;

    /* ������Ϣ��AS */
    if (VOS_OK != PS_SEND_MSG(UEPS_PID_MTA, pstMtaGrrJamDetectReq))
    {
        MTA_ERROR_LOG("TAF_MTA_SndGrrSetJamDetectReq: Send msg fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_VOID TAF_MTA_RcvNasAreaLostInd(
    VOS_VOID                           *pMsg
)
{
    VOS_INT32                           lRslt;

    /* ���õ���ӿڣ��������0����ʾPA�����������κβ��������ط�0��ʾPA�쳣������й��� */
#if defined(INSTANCE_1)
    lRslt = DRV_PASTAR_EXC_CHECK((PWC_COMM_MODEM_E)MODEM_ID_1);
#else
    lRslt = DRV_PASTAR_EXC_CHECK((PWC_COMM_MODEM_E)MODEM_ID_0);
#endif

    if (VOS_OK == lRslt)
    {
        return;
    }

    NAS_EventReport(UEPS_PID_MTA,
                    NAS_OM_EVENT_PA_STAR_ABNORMAL,
                    VOS_NULL_PTR,
                    NAS_OM_EVENT_NO_PARA);

    return;
}
VOS_VOID TAF_MTA_RcvTafAgentGetAntStateReq(
    VOS_VOID                           *pMsg
)
{
    VOS_UINT                                ulAntState;
    MODEM_ID_ENUM_UINT16                    enModemId;
    TAFAGENT_MTA_GET_ANT_STATE_CNF_STRU    *pstMsg= VOS_NULL_PTR;

    enModemId   = VOS_GetModemIDFromPid(UEPS_PID_MTA);
    ulAntState  = 0;

    pstMsg = (TAFAGENT_MTA_GET_ANT_STATE_CNF_STRU *)PS_ALLOC_MSG(UEPS_PID_MTA,
                                                                 sizeof(TAFAGENT_MTA_GET_ANT_STATE_CNF_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstMsg)
    {
        MTA_WARNING_LOG("TAF_MTA_RcvTafAgentGetAntStateReq:alloc msg failed.");

        return;
    }

    PS_MEM_SET(((VOS_UINT8 *)pstMsg + VOS_MSG_HEAD_LENGTH), 0, (sizeof(TAFAGENT_MTA_GET_ANT_STATE_CNF_STRU) - VOS_MSG_HEAD_LENGTH));

    /* ��Ϣͷ */
    pstMsg->ulSenderCpuId   = VOS_LOCAL_CPUID;
    pstMsg->ulSenderPid     = UEPS_PID_MTA;
    pstMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverPid   = ACPU_PID_TAFAGENT;
    pstMsg->enMsgId         = ID_TAFAGENT_MTA_GET_ANT_STATE_CNF;
    pstMsg->ulRslt          = VOS_ERR;
    pstMsg->usAntState      = 0;

    /* ���õ���ӿڻ�ȡ����״̬ */
    if (VOS_OK == DRV_GET_ANTEN_LOCKSTATE((PWC_COMM_MODEM_E)enModemId, &ulAntState))
    {
        pstMsg->ulRslt      = VOS_OK;
        pstMsg->usAntState  = (VOS_UINT16)ulAntState;
    }

    /* ��TAFAGENT���Ͳ�ѯ�ظ� */
    if (VOS_OK != PS_SEND_MSG(UEPS_PID_MTA, pstMsg))
    {
        MTA_WARNING_LOG("TAF_MTA_RcvTafAgentGetAntStateReq:send msg failed.");

        return;
    }

    return;
}

#if (FEATURE_ON == FEATURE_PTM)
VOS_VOID TAF_MTA_SndAcpuOmErrLogRptCnf(
    VOS_CHAR                           *pbuffer,
    VOS_UINT32                          ulBufUseLen
)
{
    OM_ERR_LOG_REPORT_CNF_STRU         *pstQryCnf = VOS_NULL_PTR;
    VOS_UINT32                          ulMsgLen;

    /* �ϱ�����Ϣ�ܳ��� */
    ulMsgLen  = sizeof(OM_ERR_LOG_REPORT_CNF_STRU) - VOS_MSG_HEAD_LENGTH - 4 + ulBufUseLen;

    /* ��Ϣ�ռ����� */
    pstQryCnf = (OM_ERR_LOG_REPORT_CNF_STRU *)PS_ALLOC_MSG(UEPS_PID_MTA, ulMsgLen);
    if (VOS_NULL_PTR == pstQryCnf)
    {
        MTA_ERROR_LOG("TAF_MTA_SndAcpuOmErrLogRptCnf: alloc msg fail!");
        return;
    }

    pstQryCnf->ulReceiverCpuId  = VOS_LOCAL_CPUID;
    pstQryCnf->ulSenderCpuId    = VOS_LOCAL_CPUID;
    pstQryCnf->ulReceiverPid    = ACPU_PID_OM;
    pstQryCnf->ulSenderPid      = UEPS_PID_MTA;
    pstQryCnf->ulMsgName        = ID_OM_ERR_LOG_REPORT_CNF;
    pstQryCnf->ulMsgType        = OM_ERR_LOG_MSG_ERR_REPORT;
    pstQryCnf->ulMsgSN          = 0;
    pstQryCnf->ulRptlen         = ulBufUseLen;

    /* buffer��Ϊ��ʱ��lenҲ�ǲ���Ϊ�յ� */
    if (VOS_NULL_PTR != pbuffer)
    {
        PS_MEM_CPY(pstQryCnf->aucContent, pbuffer, ulBufUseLen);
    }

    if (VOS_OK != PS_SEND_MSG(UEPS_PID_MTA, pstQryCnf))
    {
        MTA_ERROR_LOG("TAF_MTA_SndAcpuOmErrLogRptCnf: SEND MSG FAIL");
    }

    return;

}
VOS_VOID TAF_MTA_RcvAcpuOmErrLogRptReq(
    VOS_VOID                           *pMsg
)
{
    VOS_CHAR                           *pbuffer   = VOS_NULL_PTR;
    VOS_UINT32                          ulBufUseLen;
    VOS_UINT32                          ulRealLen;

    /* ��ѯһ��RING BUFFER���ж������ݣ��Ա�����ڴ� */
    ulBufUseLen = TAF_SDC_GetErrLogRingBufferUseBytes();

    /* ���RING BUFFER��û�����ݣ�Ҳ��Ҫ��OM������Ϣ */
    if (0 == ulBufUseLen)
    {
        /* ����ID_OM_ERR_LOG_REPORT_CNF����Ϊ�յ���Ϣ��OM */
        TAF_MTA_SndAcpuOmErrLogRptCnf(VOS_NULL_PTR, 0);
        return;
    }

    pbuffer = (VOS_CHAR *)PS_MEM_ALLOC(UEPS_PID_MTA, ulBufUseLen);
    if (VOS_NULL_PTR == pbuffer)
    {
        /* ����ID_OM_ERR_LOG_REPORT_CNF����Ϊ�յ���Ϣ��OM */
        TAF_MTA_SndAcpuOmErrLogRptCnf(VOS_NULL_PTR, 0);
        return;
    }

    PS_MEM_SET(pbuffer, 0, ulBufUseLen);

    /* ��ȡRING BUFFER������ */
    ulRealLen = TAF_SDC_GetErrLogRingBufContent(pbuffer, ulBufUseLen);
    if (ulRealLen != ulBufUseLen)
    {
        /* ����ID_OM_ERR_LOG_REPORT_CNF����Ϊ�յ���Ϣ��OM */
        TAF_MTA_SndAcpuOmErrLogRptCnf(VOS_NULL_PTR, 0);
        PS_MEM_FREE(UEPS_PID_MTA, pbuffer);
        return;
    }

    /* ��ȡ���˺���Ҫ��RINGBUFFER��� */
    TAF_SDC_CleanErrLogRingBuf();

    /* ����ID_OM_ERR_LOG_REPORT_CNF��Ϣ��ACPU OM */
    TAF_MTA_SndAcpuOmErrLogRptCnf(pbuffer, ulBufUseLen);

    PS_MEM_FREE(UEPS_PID_MTA, pbuffer);

    return;
}


VOS_VOID TAF_MTA_RcvAcpuOmErrLogCtrlInd(
    VOS_VOID                           *pMsg
)
{
    OM_ERROR_LOG_CTRL_IND_STRU         *pstRcvMsg = VOS_NULL_PTR;

    pstRcvMsg = (OM_ERROR_LOG_CTRL_IND_STRU*)pMsg;

    /* ����ERRLOG���Ʊ�ʶ */
    if ((VOS_FALSE == pstRcvMsg->ucAlmStatus)
     || (VOS_TRUE  == pstRcvMsg->ucAlmStatus))
    {
        TAF_SDC_SetErrlogCtrlFlag(pstRcvMsg->ucAlmStatus);
    }

    if ((pstRcvMsg->ucAlmLevel >= NAS_ERR_LOG_CTRL_LEVEL_CRITICAL)
     && (pstRcvMsg->ucAlmLevel <= NAS_ERR_LOG_CTRL_LEVEL_WARNING))
    {
        TAF_SDC_SetErrlogAlmLevel(pstRcvMsg->ucAlmLevel);
    }

    return;
}

#endif

#if (FEATURE_ON == FEATURE_LTE)
VOS_UINT32 TAF_MTA_SndLrrcSetDpdtTestFlagNtfMsg(
    AT_MTA_SET_DPDTTEST_FLAG_REQ_STRU  *pstSetDpdtFlagReq
)
{
    MTA_LRRC_SET_DPDTTEST_FLAG_NTF_STRU    *pstMsg = VOS_NULL_PTR;
    VOS_UINT32                              ulLength;

    /* ������Ϣ�ṹ�ڴ� */
    ulLength    = sizeof(MTA_LRRC_SET_DPDTTEST_FLAG_NTF_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMsg      = (MTA_LRRC_SET_DPDTTEST_FLAG_NTF_STRU *)PS_ALLOC_MSG(UEPS_PID_MTA, ulLength);

    if (VOS_NULL_PTR == pstMsg)
    {
        MTA_ERROR_LOG("TAF_MTA_SndLrrcSetDpdtTestFlagNtfMsg: Alloc msg fail!");
        return VOS_ERR;
    }

    PS_MEM_SET((VOS_INT8 *)pstMsg + VOS_MSG_HEAD_LENGTH, 0, ulLength);

    /* ��д����Ϣ���� */
    pstMsg->ulSenderCpuId           = VOS_LOCAL_CPUID;
    pstMsg->ulSenderPid             = UEPS_PID_MTA;
    pstMsg->ulReceiverCpuId         = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverPid           = PS_PID_ERRC;
    pstMsg->ulLength                = ulLength;
    pstMsg->ulMsgName               = ID_MTA_LRRC_SET_DPDTTEST_FLAG_NTF;
    pstMsg->ucFlag                  = pstSetDpdtFlagReq->ucFlag;

    /* ������Ϣ��LRRC */
    if (VOS_OK != PS_SEND_MSG(UEPS_PID_MTA, pstMsg))
    {
        MTA_ERROR_LOG("TAF_MTA_SndLrrcSetDpdtTestFlagNtfMsg: PS_SEND_MSG fail.");
        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 TAF_MTA_SndLrrcSetDpdtValueNtfMsg(
    AT_MTA_SET_DPDT_VALUE_REQ_STRU  *pstSetDpdtReq
)
{
    MTA_LRRC_SET_DPDT_VALUE_NTF_STRU   *pstMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulLength;

    /* ������Ϣ�ṹ�ڴ� */
    ulLength    = sizeof(MTA_LRRC_SET_DPDT_VALUE_NTF_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMsg      = (MTA_LRRC_SET_DPDT_VALUE_NTF_STRU *)PS_ALLOC_MSG(UEPS_PID_MTA, ulLength);

    if (VOS_NULL_PTR == pstMsg)
    {
        MTA_ERROR_LOG("TAF_MTA_SndLrrcSetDpdtValueNtfMsg: Alloc msg fail!");
        return VOS_ERR;
    }

    PS_MEM_SET((VOS_INT8 *)pstMsg + VOS_MSG_HEAD_LENGTH, 0, ulLength);

    /* ��д����Ϣ���� */
    pstMsg->ulSenderCpuId           = VOS_LOCAL_CPUID;
    pstMsg->ulSenderPid             = UEPS_PID_MTA;
    pstMsg->ulReceiverCpuId         = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverPid           = PS_PID_ERRC;
    pstMsg->ulLength                = ulLength;
    pstMsg->ulMsgName               = ID_MTA_LRRC_SET_DPDT_VALUE_NTF;
    pstMsg->ulDpdtValue             = pstSetDpdtReq->ulDpdtValue;

    /* ������Ϣ��LRRC */
    if (VOS_OK != PS_SEND_MSG(UEPS_PID_MTA, pstMsg))
    {
        MTA_ERROR_LOG("TAF_MTA_SndLrrcSetDpdtValueNtfMsg: PS_SEND_MSG fail.");
        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 TAF_MTA_SndLrrcQryDpdtValueReqMsg(
    AT_MTA_QRY_DPDT_VALUE_REQ_STRU     *pstSetDpdtReq
)
{
    MTA_LRRC_QRY_DPDT_VALUE_REQ_STRU   *pstMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulLength;

    /* ������Ϣ�ṹ�ڴ� */
    ulLength    = sizeof(MTA_LRRC_QRY_DPDT_VALUE_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMsg      = (MTA_LRRC_QRY_DPDT_VALUE_REQ_STRU *)PS_ALLOC_MSG(UEPS_PID_MTA, ulLength);

    if (VOS_NULL_PTR == pstMsg)
    {
        MTA_ERROR_LOG("TAF_MTA_SndLrrcQryDpdtValueReqMsg: Alloc msg fail!");
        return VOS_ERR;
    }

    PS_MEM_SET((VOS_INT8 *)pstMsg + VOS_MSG_HEAD_LENGTH, 0, ulLength);

    /* ��д����Ϣ���� */
    pstMsg->ulSenderCpuId           = VOS_LOCAL_CPUID;
    pstMsg->ulSenderPid             = UEPS_PID_MTA;
    pstMsg->ulReceiverCpuId         = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverPid           = PS_PID_ERRC;
    pstMsg->ulLength                = ulLength;
    pstMsg->ulMsgName               = ID_MTA_LRRC_QRY_DPDT_VALUE_REQ;

    /* ������Ϣ��LRRC */
    if (VOS_OK != PS_SEND_MSG(UEPS_PID_MTA, pstMsg))
    {
        MTA_ERROR_LOG("TAF_MTA_SndLrrcQryDpdtValueReqMsg: PS_SEND_MSG fail.");
        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_VOID TAF_MTA_RcvLrrcDpdtValueQryCnf(VOS_VOID *pMsg)
{
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBuf        = VOS_NULL_PTR;
    LRRC_MTA_QRY_DPDT_VALUE_CNF_STRU   *pstLrrcMtaQryCnf = VOS_NULL_PTR;
    MTA_AT_QRY_DPDT_VALUE_CNF_STRU      stMtaAtQryCnf;
    TAF_MTA_TIMER_ID_ENUM_UINT32        enTimerId;

    /* �ֲ�������ʼ�� */
    pstLrrcMtaQryCnf = (LRRC_MTA_QRY_DPDT_VALUE_CNF_STRU *)pMsg;
    enTimerId = TI_TAF_MTA_WAIT_DPDT_VALUE_QRY_CNF;

    if (TAF_MTA_TIMER_STATUS_RUNING != TAF_MTA_GetTimerStatus(enTimerId))
    {
        MTA_WARNING_LOG("TAF_MTA_RcvLrrcDpdtValueQryCnf: WARNING: Timer was already stop!");
        return;
    }

    /* ֹͣ������ʱ�� */
    TAF_MTA_StopTimer(enTimerId);

    /* ��ȡ��ǰ��ʱ����Ӧ����Ϣ���� */
    pstCmdBuf = TAF_MTA_GetItemFromCmdBufferQueue(enTimerId);
    if (VOS_NULL_PTR == pstCmdBuf)
    {
        return;
    }

    /* ������Ϣ�ṹ�� */
    if (MTA_RRC_RESULT_NO_ERROR == pstLrrcMtaQryCnf->enResult)
    {
        stMtaAtQryCnf.enResult = MTA_AT_RESULT_NO_ERROR;
        stMtaAtQryCnf.ulDpdtValue = pstLrrcMtaQryCnf->ulDpdtValue;
    }
    else
    {
        stMtaAtQryCnf.enResult = MTA_AT_RESULT_ERROR;
        stMtaAtQryCnf.ulDpdtValue = 0;
    }

    /* ������Ϣ��ATģ�� */
    TAF_MTA_SndAtMsg( (AT_APPCTRL_STRU *)pstCmdBuf->pucMsgInfo,
                      ID_MTA_AT_QRY_DPDT_VALUE_CNF,
                      sizeof(MTA_AT_QRY_DPDT_VALUE_CNF_STRU),
                      (VOS_UINT8 *)&stMtaAtQryCnf);

    /* �ӵȴ�������ɾ����Ϣ */
    TAF_MTA_DelItemInCmdBufferQueue(pstCmdBuf->enTimerId);

    return;
}
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

