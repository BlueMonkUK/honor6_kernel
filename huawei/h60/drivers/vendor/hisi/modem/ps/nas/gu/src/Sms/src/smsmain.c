
/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "smsinclude.h"
#include "LLInterface.h"
#include "NasGmmInterface.h"
#include "TafCbsInterface.h"
#if   (FEATURE_ON == FEATURE_LTE)
#include "LmmSmsInterface.h"
#endif
#include "SysNvId.h"
#include "NVIM_Interface.h"

#include "NasNvInterface.h"
#include "TafNvInterface.h"

#include "NasOmTrans.h"

#ifdef  __cplusplus
  #if  __cplusplus
  extern "C"{
  #endif
#endif

/*****************************************************************************
  2 ��������
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_SMS_MAIN_C

/*****************************************************************************
  3 ���Ͷ���
*****************************************************************************/
/*****************************************************************************
 Structure      : NAS_AT_OUTSIDE_RUNNING_CONTEXT_PART_ST
 Description    : PC�طŹ��̣��洢����AT��ص�ȫ�ֱ�����Ŀǰ���ж������ȫ�ֱ���
 Message origin :
 Note:
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           g_ucPsServiceStatus;                 /* GMM��PS���Ƿ�ע���־                    */
    VOS_UINT8                           g_ucCsRegFlg;                         /* MM��CS���Ƿ�ע���־                    */
    VOS_UINT8                           g_ucSmsMR;                            /* SMR��ά���Ķ���Ϣ��ʶ0��255              */
    VOS_UINT8                           g_SndDomain;                          /* ������ */
    SMR_ENTITY_STRU                     g_SmrEnt;                              /* smr��ʵ�嶨��                            */
    SMC_ENTITY_STRU                     g_SmcPsEnt;                           /* sms ps���ʵ�嶨��                       */
    SMC_ENTITY_STRU                     g_SmcCsEnt;                           /* sms cs���ʵ�嶨��                       */
}NAS_SMS_OUTSIDE_RUNNING_CONTEXT_PART_ST;

/*****************************************************************************
 Structure      : NAS_AT_SDT_AT_PART_ST
 Description    : PC�طŹ��̣�����AT��ص�ȫ�ֱ���ͨ��������Ϣ�ṹ����
 Message origin :
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                                   ulMsgName;
    NAS_SMS_OUTSIDE_RUNNING_CONTEXT_PART_ST      stOutsideCtx;
}NAS_SMS_SDT_SMS_PART_ST;

#define SMS_PC_REPLAY_MSG               0xffffffff          /* ΪPC���̻طŶ������Ϣ */
LOCAL SMS_TIMER_LENGTH_STRU                      f_stNasSmsTimerLength;
LOCAL VOS_UINT8                                  f_ucNasSmsTc1mMaxExpTimes;
/*****************************************************************************
  5 ����ʵ��
*****************************************************************************/

#ifdef __PS_WIN32_RECUR__
/*****************************************************************************
 �� �� ��  : NAS_SMS_RestoreContextData
 ��������  : �ָ�SMSȫ�ֱ�����
 �������  : struct MsgCB *pstMsg
 �������  : ��
 �� �� ֵ  : VOS_OK ���ݻָ��ɹ�,����,ʧ��;
 ���ú���  :
 ��������  :
 �޸���ʷ      :
  1.��    ��   : 2010��02��12��
    ��    ��   : ��ӳ�� 62575
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 NAS_SMS_RestoreContextData(
    struct MsgCB                        *pstMsg
)
{
    NAS_SMS_SDT_SMS_PART_ST                       *pstRcvMsgCB;
    NAS_SMS_OUTSIDE_RUNNING_CONTEXT_PART_ST       *pstOutsideCtx;

    pstRcvMsgCB     = (NAS_SMS_SDT_SMS_PART_ST *)pstMsg;

    if (SMS_PC_REPLAY_MSG == pstRcvMsgCB->ulMsgName)
    {
        pstOutsideCtx = &pstRcvMsgCB->stOutsideCtx;

        PS_MEM_CPY(&g_SmcPsEnt,&pstOutsideCtx->g_SmcPsEnt,sizeof(g_SmcPsEnt));
        PS_MEM_CPY(&g_SmcCsEnt,&pstOutsideCtx->g_SmcCsEnt, sizeof(g_SmcCsEnt));
        PS_MEM_CPY(&g_SmrEnt, &pstOutsideCtx->g_SmrEnt, sizeof(g_SmrEnt));
        g_ucPsServiceStatus = pstOutsideCtx->g_ucPsServiceStatus;
        g_ucCsRegFlg = pstOutsideCtx->g_ucCsRegFlg;
        g_ucSmsMR = pstOutsideCtx->g_ucSmsMR;
        g_SndDomain = pstOutsideCtx->g_SndDomain;

        SMS_LOG(PS_PRINT_INFO, "MSG: NAS_SMS_RestoreContextData data is restored.");
    }

    return VOS_OK;
}
#endif

/*****************************************************************************
 �� �� ��  : NAS_SMS_SndOutsideContextData
 ��������  : ��SMS�ⲿ��������ΪSDT��Ϣ���ͳ�ȥ���Ա��ڻط�ʱͨ��׮������ԭ
 �������  : ��
 �������  : ��
 �� �� ֵ  : VOS_OK ��Ϣ���ͳɹ�,����,ʧ��;
 ���ú���  :
 ��������  :
 �޸���ʷ      :
  1.��    ��   : 2010��02��12��
    ��    ��   : ��ӳ�� 62575
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 NAS_SMS_SndOutsideContextData()
{
    NAS_SMS_SDT_SMS_PART_ST                      *pSndMsgCB     = VOS_NULL_PTR;
    NAS_SMS_OUTSIDE_RUNNING_CONTEXT_PART_ST      *pstOutsideCtx;

    pSndMsgCB = (NAS_SMS_SDT_SMS_PART_ST *)PS_ALLOC_MSG(WUEPS_PID_SMS,
                                            sizeof(NAS_SMS_SDT_SMS_PART_ST));

    if ( VOS_NULL_PTR == pSndMsgCB )
    {
        SMS_LOG(PS_PRINT_ERROR, "NAS_SMS_SndOutsideContextData:ERROR: Alloc Memory Fail.");
        return VOS_ERR;
    }

    pSndMsgCB->ulReceiverPid = WUEPS_PID_SMS;
    pSndMsgCB->ulSenderPid   = WUEPS_PID_SMS;
    pSndMsgCB->ulSenderCpuId   = VOS_LOCAL_CPUID;
    pSndMsgCB->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pSndMsgCB->ulLength = sizeof(NAS_SMS_OUTSIDE_RUNNING_CONTEXT_PART_ST) + 4;
    pSndMsgCB->ulMsgName = SMS_PC_REPLAY_MSG;

    pstOutsideCtx = &pSndMsgCB->stOutsideCtx;

    PS_MEM_CPY(&pstOutsideCtx->g_SmcPsEnt, &g_SmcPsEnt, sizeof(g_SmcPsEnt));
    PS_MEM_CPY(&pstOutsideCtx->g_SmcCsEnt, &g_SmcCsEnt, sizeof(g_SmcCsEnt));
    PS_MEM_CPY(&pstOutsideCtx->g_SmrEnt, &g_SmrEnt, sizeof(g_SmrEnt));
    pstOutsideCtx->g_ucPsServiceStatus  = g_ucPsServiceStatus;
    pstOutsideCtx->g_ucCsRegFlg         = g_ucCsRegFlg;
    pstOutsideCtx->g_ucSmsMR            = g_ucSmsMR;
    pstOutsideCtx->g_SndDomain          = g_SndDomain;

    OM_TraceMsgHook(pSndMsgCB);

    PS_FREE_MSG(WUEPS_PID_SMS, pSndMsgCB);

    return VOS_OK;
}


/*******************************************************************************
  Module:   SMS_TaskEntry
  Function: SMSģ��TASK��ڴ���
  Input:    struct MsgCB* pMsg-----------��Ϣָ��
  Output:   VOS_VOID
  NOTE:
  Return:   VOS_VOID
  History:
      1.  ��־��   2005.01.28   �¹�����
      2.  ��־��  2005.03.08   ʹ��PID���зַ�����
*******************************************************************************/

#define     SMS_TASK_NOT_START       0                                          /* TASK SMS û������                        */
#define     SMS_TASK_START           1                                          /* TASK SMS �Ѿ�����                        */
VOS_UINT8   g_SmsStartFlg = SMS_TASK_NOT_START;                                 /* TASK SMS ������־                        */

/*******************************************************************************
  Module:      SMS_ReportN2MOtaMsg
  Function:    ��trace�й�ȡSMS�Ŀտ���Ϣ
  Input:       NAS_MSG_STRU*   pNasMsg     SMS�տ���Ϣ
  NOTE:
  Return:      VOS_VOID
  History:
      1. ³��    2009.07.11   �¹�����
*******************************************************************************/
VOS_VOID SMS_ReportN2MOtaMsg(NAS_MSG_STRU *pNasMsg)
{
    NAS_OTA_MSG_ID_ENUM_UINT16 usNasOtaMsyId = NAS_OTA_MSG_ID_BUTT;

    PS_LOG1(WUEPS_PID_SMS, VOS_NULL, PS_PRINT_NORMAL, "SMS_ReportN2MOtaMsg:NORMAL: msg type is ", pNasMsg->aucNasMsg[1]);

    switch(pNasMsg->aucNasMsg[1])
    {
        case SMC_DATA_TYPE_CP_DATA:
          	usNasOtaMsyId = NAS_OTA_MSG_SMS_DATA_TYPE_CP_DATA_DOWN;
            break;

        case SMC_DATA_TYPE_CP_ACK:
           	usNasOtaMsyId = NAS_OTA_MSG_SMS_DATA_TYPE_CP_ACK_DOWN;
            break;

        case SMC_DATA_TYPE_CP_ERR:
            usNasOtaMsyId = NAS_OTA_MSG_SMS_DATA_TYPE_CP_ERROR_DOWN;
            break;

        default:
            PS_LOG(WUEPS_PID_SMS, VOS_NULL, PS_PRINT_WARNING, "SMS_ReportN2MOtaMsg:WARNING: invalid msg id");
            /*������SMS�Ŀտ���Ϣ������trace����ʾ*/
            return;
    }

    NAS_SendAirMsgToOM(WUEPS_PID_SMS, usNasOtaMsyId, NAS_OTA_DIRECTION_DOWN, pNasMsg->ulNasMsgSize + 4, (VOS_UINT8*)pNasMsg);
}

VOS_VOID NAS_SMS_BuildSmsOmInfo(
    NAS_OM_SMS_CONFIRM_STRU             *pstMsg
)
{
    pstMsg->ucSmcCsMoState = g_SmcCsEnt.SmcMo.ucState;
    pstMsg->ucSmcCsMtState = g_SmcCsEnt.SmcMt.ucState;
    pstMsg->ucSmcPsMoState = g_SmcPsEnt.SmcMo.ucState;
    pstMsg->ucSmcPsMtState = g_SmcPsEnt.SmcMt.ucState;
    pstMsg->ucSmrMoState   = g_SmrEnt.SmrMo.ucState;
    pstMsg->ucSmrMtState   = g_SmrEnt.SmrMt.ucState;
}

VOS_VOID NAS_SMS_SndOmInquireCnfMsg(
    ID_NAS_OM_INQUIRE_STRU             *pstOmInquireMsg
)
{
    ID_NAS_OM_CNF_STRU                 *pstSmsOmCnf     ;

    pstSmsOmCnf     = VOS_NULL_PTR;
    
    pstSmsOmCnf = (ID_NAS_OM_CNF_STRU *)PS_MEM_ALLOC(WUEPS_PID_SMS,
        (sizeof(ID_NAS_OM_CNF_STRU) + sizeof(NAS_OM_SMS_CONFIRM_STRU)) - NAS_OM_DATA_PTR_LEN); /* �����ڴ� */

    if (VOS_NULL_PTR == pstSmsOmCnf)
    {
        NAS_ERROR_LOG(WUEPS_PID_SMS, "NAS_MMC_SndOmInquireCnfMsg:Memory Allocate fail!");
        return;
    }

    PS_MEM_SET((VOS_INT8*)pstSmsOmCnf, 0 ,sizeof(ID_NAS_OM_CNF_STRU));


    pstSmsOmCnf->ucFuncType         = 4;
    pstSmsOmCnf->ucReserve          = 0;
    pstSmsOmCnf->usLength           = NAS_OM_DATA_OFFSET + sizeof(NAS_OM_SMS_CONFIRM_STRU);
    pstSmsOmCnf->ulSn               = 0;
    pstSmsOmCnf->ulTimeStamp        = OM_GetSlice();
    pstSmsOmCnf->usPrimId           = ID_NAS_OM_SMS_CONFIRM;
    pstSmsOmCnf->usToolsId          = pstOmInquireMsg->usToolsId;

    NAS_SMS_BuildSmsOmInfo((NAS_OM_SMS_CONFIRM_STRU*)(pstSmsOmCnf->aucData));

    OM_SendData( (OM_RSP_PACKET_STRU*)pstSmsOmCnf, (sizeof(ID_NAS_OM_CNF_STRU) + sizeof(NAS_OM_SMS_CONFIRM_STRU)) - NAS_OM_DATA_PTR_LEN );                                   /* ԭ�﷢��                                 */

    PS_MEM_FREE(WUEPS_PID_SMS, pstSmsOmCnf);

}



VOS_VOID NAS_SMS_RcvOmMaintainInfoInd(
    struct MsgCB                       *pstMsg
)
{
    MMCM_OM_MAINTAIN_INFO_IND_STRU     *pstOmMaintainInfo;

    pstOmMaintainInfo = (MMCM_OM_MAINTAIN_INFO_IND_STRU*)pstMsg;

    if ( (VOS_TRUE == pstOmMaintainInfo->ucOmConnectFlg)
      && (VOS_TRUE == pstOmMaintainInfo->ucOmPcRecurEnableFlg) )
    {
        NAS_SMS_SndOutsideContextData();
    }

}


VOS_UINT32 NAS_SMS_RcvOmInquireReq(
    struct MsgCB                       *pstMsg
)
{
    ID_NAS_OM_INQUIRE_STRU             *pstOmMsg = VOS_NULL_PTR;

    pstOmMsg = (ID_NAS_OM_INQUIRE_STRU *)pstMsg;

    NAS_INFO_LOG(WUEPS_PID_SMS, "NAS_SMS_RcvOmInquireReq:  ");

    NAS_SMS_SndOmInquireCnfMsg(pstOmMsg);

    return VOS_TRUE;

}

VOS_VOID SMC_RcvOmMsgDistr(
                       VOS_VOID     *pRcvMsg                                        /* �յ�����Ϣ                               */
                       )
{
    switch(((MSG_HEADER_STRU *)pRcvMsg)->ulMsgName)
    {
        case ID_NAS_OM_SMS_INQUIRE:
            NAS_SMS_RcvOmInquireReq(pRcvMsg);
            break;
        default:
            break;
    }
}


VOS_VOID SMS_TaskEntry(struct MsgCB* pMsg )
{
    MSG_HEADER_STRU         *pHeader;                                           /* ������Ϣͷָ��                           */
    REL_TIMER_MSG           *pTimer;                                            /* ����TIMER��Ϣָ��                        */

    pHeader = (MSG_HEADER_STRU *) pMsg;                                         /* ��ȡ��Ϣͷָ��                           */
    pTimer  = (REL_TIMER_MSG *) pMsg;                                           /* ��ȡTIMER��Ϣָ��                        */

    if (SMS_TASK_START == g_SmsStartFlg)
    {                                                                           /* TASK SMS�Ѿ������ĳ���                   */
        if ( VOS_PID_TIMER == pMsg->ulSenderPid)
        {                                                                       /* �����TIMER��Ϣ                          */
            SMS_TimerMsgDistr( (VOS_UINT8)pTimer->ulName );                         /* TIMER����ַ�����                        */
        }
        else
        {
            switch( pHeader->ulSenderPid )
            {                                                                   /* ����Src Tsk Id�ַ���Ϣ                   */
            case WUEPS_PID_GMM:
                SMC_RcvGmmMsgDistr(pMsg);
                break;

#if (FEATURE_ON == FEATURE_LTE)
            case PS_PID_MM:
                NAS_SMS_RcvLmmMsgDistr(pMsg);
                break;
#endif
            case WUEPS_PID_MM:
                SMC_RcvMmMsgDistr(pMsg);
                break;
            case UEPS_PID_LL:
                SMC_RcvLlcMsgDistr(pMsg);
                break;
            case WUEPS_PID_TAF:
                NAS_SMS_ProcMnMsg(pMsg);
                break;
            case WUEPS_PID_ADMIN:
                if (WUEPS_RESET_REQ == pHeader->ulMsgName)
                {
                    g_SmsStartFlg = SMS_TASK_NOT_START;                         /* �ȴ�ADMIN��������ԭ��                    */
                }
                break;
            #ifdef __PS_WIN32_RECUR__
            case WUEPS_PID_SMS:
                NAS_SMS_RestoreContextData(pMsg);
                break;
            #endif
            #ifndef __PS_WIN32_RECUR__
            case WUEPS_PID_MMA:
                if (MMCM_OM_MAINTAIN_INFO_IND == pHeader->ulMsgName)
                {
                    NAS_SMS_RcvOmMaintainInfoInd(pMsg);
                }
                break;
            #endif
            case WUEPS_PID_OM:
                SMC_RcvOmMsgDistr(pMsg);
                break;
            default :
                PS_LOG(WUEPS_PID_SMS, VOS_NULL, PS_PRINT_WARNING, "LogRecord_Timer_HW:WARNING:Rcv Msg PID Error!");
                break;
            }
        }
    }
}
/*******************************************************************************
  Module:   SMS_InitEntity
  Function: ��ʼ��SMSʵ��
  Input:    VOS_VOID
  Output:   VOS_VOID
  NOTE:
  Return:   VOS_VOID
  History:
      1.  ��־��      2004.03.09   �¹�����
      2. ��    ��   : 2007��8��28��
         ��    ��   : l60022475
         �޸�����   : ���ⵥ�ţ�A32D12744,��ʼ��SMS Timer���
*******************************************************************************/
VOS_VOID SMS_InitEntity()
{
/* ȫ����MR��ʼ��Ϊ�� */
    g_ucSmsMR = 0;

/* SMCʵ��ĳ�ʼ�� */

    g_SmcPsEnt.SmcMo.ucTi                = 0;
    g_SmcPsEnt.SmcMo.ucState             = SMC_MO_IDLE;
    g_SmcPsEnt.SmcMo.ucMr                = 0;
    g_SmcPsEnt.SmcMo.ucCpAckFlg          = SMS_FALSE;
    g_SmcPsEnt.SmcMo.HoldSndMsg.ulMsgLen = 0;
    g_SmcPsEnt.SmcMo.HoldSndMsg.pMsg     = SMS_POINTER_NULL;
    g_SmcPsEnt.SmcMo.HoldRcvMsg.ulMsgLen = 0;
    g_SmcPsEnt.SmcMo.HoldRcvMsg.pMsg     = SMS_POINTER_NULL;

    g_SmcPsEnt.SmcMt.ucTi                = 0;
    g_SmcPsEnt.SmcMt.ucState             = SMC_MT_IDLE;
    g_SmcPsEnt.SmcMt.ucMr                = 0;
    g_SmcPsEnt.SmcMt.HoldSndMsg.ulMsgLen = 0;
    g_SmcPsEnt.SmcMt.HoldSndMsg.pMsg     = SMS_POINTER_NULL;



    g_SmcCsEnt.SmcMo.ucTi                = 0;
    g_SmcCsEnt.SmcMo.ucState             = SMC_MO_IDLE;
    g_SmcCsEnt.SmcMo.ucMr                = 0;
    g_SmcCsEnt.SmcMo.ucCpAckFlg          = SMS_FALSE;
    g_SmcCsEnt.SmcMo.HoldSndMsg.ulMsgLen = 0;
    g_SmcCsEnt.SmcMo.HoldSndMsg.pMsg     = SMS_POINTER_NULL;
    g_SmcCsEnt.SmcMo.HoldRcvMsg.ulMsgLen = 0;
    g_SmcCsEnt.SmcMo.HoldRcvMsg.pMsg     = SMS_POINTER_NULL;

    g_SmcCsEnt.SmcMt.ucTi                = 0;
    g_SmcCsEnt.SmcMt.ucState             = SMC_MT_IDLE;
    g_SmcCsEnt.SmcMt.ucMr                = 0;
    g_SmcCsEnt.SmcMt.HoldSndMsg.ulMsgLen = 0;
    g_SmcCsEnt.SmcMt.HoldSndMsg.pMsg     = SMS_POINTER_NULL;
    /* ==>A32D12744 */
    g_SmcPsEnt.SmcMo.TimerInfo.TimerId   = SMS_POINTER_NULL;
    g_SmcPsEnt.SmcMt.TimerInfo.TimerId   = SMS_POINTER_NULL;
    g_SmcCsEnt.SmcMo.TimerInfo.TimerId   = SMS_POINTER_NULL;
    g_SmcCsEnt.SmcMt.TimerInfo.TimerId   = SMS_POINTER_NULL;
    /* ==>A32D12744 */

    g_SmcPsEnt.SmcMo.TimerInfo.ucTimerSta    = SMS_TIMER_STATUS_STOP;
    g_SmcPsEnt.SmcMo.TimerInfo.ucExpireTimes = 0;

    g_SmcPsEnt.SmcMt.TimerInfo.ucTimerSta    = SMS_TIMER_STATUS_STOP;
    g_SmcPsEnt.SmcMt.TimerInfo.ucExpireTimes = 0;

    g_SmcCsEnt.SmcMo.TimerInfo.ucTimerSta    = SMS_TIMER_STATUS_STOP;
    g_SmcCsEnt.SmcMo.TimerInfo.ucExpireTimes = 0;

    g_SmcCsEnt.SmcMt.TimerInfo.ucTimerSta    = SMS_TIMER_STATUS_STOP;
    g_SmcCsEnt.SmcMt.TimerInfo.ucExpireTimes = 0;

/* SMRʵ��ĳ�ʼ�� */
    g_SmrEnt.SmrMo.ucState       = SMR_IDLE;
    g_SmrEnt.SmrMo.ucRetransFlg  = SMR_RETRANS_PERMIT;
    g_SmrEnt.SmrMo.ucMr          = 0;
    g_SmrEnt.SmrMo.ucMemAvailFlg = SMS_FALSE;
    g_SmrEnt.SmrMt.ucState  = SMR_IDLE;
    g_SmrEnt.SmrMt.ucMr     = 0;

    /* ==>A32D12744 */
    g_SmrEnt.SmrMo.aTimerInfo[0].TimerId = SMS_POINTER_NULL;
    g_SmrEnt.SmrMo.aTimerInfo[1].TimerId = SMS_POINTER_NULL;
    g_SmrEnt.SmrMo.aTimerInfo[2].TimerId = SMS_POINTER_NULL;
    g_SmrEnt.SmrMt.TimerInfo.TimerId = SMS_POINTER_NULL;
    g_SmrEnt.SmrMt.RelTimerInfo.TimerId = SMS_POINTER_NULL;
    /* <==A32D12744 */
    g_SmrEnt.SmrMo.aTimerInfo[0].ucTimerSta = SMS_TIMER_STATUS_STOP;

    g_SmrEnt.SmrMo.aTimerInfo[1].ucTimerSta = SMS_TIMER_STATUS_STOP;

    g_SmrEnt.SmrMo.aTimerInfo[2].ucTimerSta = SMS_TIMER_STATUS_STOP;

    g_SmrEnt.SmrMt.TimerInfo.ucTimerSta = SMS_TIMER_STATUS_STOP;

    g_SmrEnt.SmrMt.RelTimerInfo.ucTimerSta = SMS_TIMER_STATUS_STOP;
}
VOS_VOID SMS_Poweroff(VOS_VOID)
{
    /*ֹͣ���ж�ʱ��*/
    if ( SMS_FALSE == SMS_TimerStop( SMC_TIMER_ID_PS_TC1M_MO ) )
    {
        SMS_LOG( PS_PRINT_ERROR,
                 "SMS:SMS_Poweroff: SMC_TIMER_ID_PS_TC1M_MO Timer stop fails" );
    }

    if ( SMS_FALSE == SMS_TimerStop( SMC_TIMER_ID_PS_TC1M_MT ) )
    {
        SMS_LOG( PS_PRINT_ERROR,
                 "SMS:SMS_Poweroff: SMC_TIMER_ID_PS_TC1M_MT Timer stop fails" );
    }


    if ( SMS_FALSE == SMS_TimerStop( SMC_TIMER_ID_CS_TC1M_MO ) )
    {
        SMS_LOG( PS_PRINT_ERROR,
                 "SMS:SMS_Poweroff: SMC_TIMER_ID_CS_TC1M_MO Timer stop fails" );
    }


    if ( SMS_FALSE == SMS_TimerStop( SMC_TIMER_ID_CS_TC1M_MT ) )
    {
        SMS_LOG( PS_PRINT_ERROR,
                 "SMS:SMS_Poweroff: SMC_TIMER_ID_CS_TC1M_MO Timer stop fails" );
    }

    if ( SMS_FALSE == SMS_TimerStop( SMC_TIMER_ID_TR1M ) )
    {
        SMS_LOG( PS_PRINT_ERROR,
                 "SMS:SMS_Poweroff: SMC_TIMER_ID_TR1M Timer stop fails" );
    }


    if ( SMS_FALSE == SMS_TimerStop( SMC_TIMER_ID_TR2M ) )
    {
        SMS_LOG( PS_PRINT_ERROR,
                 "SMS:SMS_Poweroff: SMC_TIMER_ID_TR2M Timer stop fails" );
    }


    if ( SMS_FALSE == SMS_TimerStop( SMC_TIMER_ID_TRAM ) )
    {
        SMS_LOG( PS_PRINT_ERROR,
                 "SMS:SMS_Poweroff: SMC_TIMER_ID_TRAM Timer stop fails" );
    }

    /*�ͷ��������ڽ��еĶ��ŷ��ͻ���ܹ���*/
    g_ucCsRegFlg = SMS_FALSE;                                                   /* ��¼��״̬                               */

    SMC_ComCsMtErr(SMR_SMT_ERROR_NO_SERVICE, g_SmcCsEnt.SmcMt.ucTi);                                   /* ����CS��MTʵ��Ĵ���                     */

    SMC_ComCsMoErr(SMR_SMT_ERROR_NO_SERVICE, g_SmcCsEnt.SmcMo.ucTi);                         /* ����CS��MOʵ��Ĵ���                     */

    if (SMS_TRUE == g_SmcCsEnt.SmcMo.ucCpAckFlg)
    {                                                                   /* TI���                                   */
        g_SmcCsEnt.SmcMo.ucCpAckFlg = SMS_FALSE;
        SMS_SendMnMoLinkCloseInd();
    }

    g_ucPsServiceStatus = SMS_FALSE;                                            /* ��¼��״̬                               */

    SMC_ComPsMtErr(SMR_SMT_ERROR_NO_SERVICE);                                  /* ����PS��MTʵ��Ĵ���                     */
    SMC_ComPsMoErr(SMR_SMT_ERROR_NO_SERVICE);                                  /* ����PS��MOʵ��Ĵ���                     */

    if (SMS_TRUE == g_SmcPsEnt.SmcMo.ucCpAckFlg)
    {                                                                   /* TI���                                   */
        g_SmcPsEnt.SmcMo.ucCpAckFlg = SMS_FALSE;
        SMS_SendMnMoLinkCloseInd();
    }


#ifdef DMT
    SMS_InitEntity();
#endif

    return;
}


VOS_VOID SMC_RcvLlcMsgDistr(
                       VOS_VOID     *pRcvMsg                                        /* �յ�����Ϣ                               */
                       )
{
    LL_SMS_UNITDATA_IND_MSG             *pMsg;
    /*����NAS_EventReport��Ҫ����NAS_MSG_STRU*/
    NAS_MSG_STRU       *pNasMsg = VOS_NULL_PTR;
    VOS_UINT32          ulNasMsgLen;
    VOS_UINT32          ulMsgLen;

    if ( NAS_GMM_NET_RAT_TYPE_WCDMA == GMM_GetCurNetwork() )
    {
        PS_LOG(WUEPS_PID_SMS, VOS_NULL, PS_PRINT_WARNING,
            "SMC_RcvLlcMsgDistr:WARNING:In 3G mode, Message from LLC shouldn't be received");
        return;
    }

    pMsg = (LL_SMS_UNITDATA_IND_MSG *)pRcvMsg;

    switch(((LL_SMS_UNITDATA_IND_MSG *)pRcvMsg)->usMsgType)
    {
    case ID_LL_UNITDATA_IND:
    	/*Report event begin*/
        ulMsgLen = ((LL_SMS_UNITDATA_IND_MSG *)pRcvMsg)->stUnitDataInd.usPduLen;

        if(ulMsgLen > 4)
        {
            ulNasMsgLen = (sizeof(NAS_MSG_STRU) + ulMsgLen) - 4;
        }
        else
        {
            ulNasMsgLen = sizeof(NAS_MSG_STRU);
        }

        pNasMsg = (NAS_MSG_STRU *)SMS_Malloc(ulNasMsgLen);

        if(VOS_NULL_PTR == pNasMsg)
        {
            PS_LOG(WUEPS_PID_SMS, VOS_NULL, PS_PRINT_ERROR, "SMC_ReportM2NEvent:ERROR: pNasMsg is NULL");
        }
        else
        {
            pNasMsg->ulNasMsgSize = ulMsgLen;

            SMS_Memcpy(pNasMsg->aucNasMsg, ((LL_SMS_UNITDATA_IND_MSG *)pRcvMsg)->stUnitDataInd.aucPdu, pNasMsg->ulNasMsgSize);

            SMS_ReportN2MOtaMsg(pNasMsg);


            SMS_Free(pNasMsg);
        }
        /*Report event end*/

        if((((LL_SMS_UNITDATA_IND_MSG *)pRcvMsg)->stUnitDataInd.aucPdu[0]>>4)< 8)
        {                                                                       /* ��mt����                                 */
            if(SMC_MT_IDLE != g_SmcCsEnt.SmcMt.ucState)
            {                                                                   /* ͬʱ������mt����                         */
                SMC_SndGmmDataReq(
                        SMC_DATA_TYPE_CP_ERR,
                        SMS_CP_ERR_CONGEST,
                        (VOS_UINT8)((((LL_SMS_UNITDATA_IND_MSG *)pRcvMsg)->stUnitDataInd.
                              aucPdu[0]>>4) )                                   /* �����෢��CP_ERROR  TIȡ���յ���Ϣ��TI   */
                                );                                              /* �����౨��                               */
            }
            else
            {
                /* ����mt�Ĵ������ */
                SMC_RcvPsMtData((VOS_UINT8 *)(VOS_UINT32)pMsg->stUnitDataInd.aucPdu,
                                pMsg->stUnitDataInd.usPduLen);
            }
        }
        else
        {   /* ��mo���� */
            SMC_RcvPsMoData((VOS_UINT8 *)(VOS_UINT32)pMsg->stUnitDataInd.aucPdu,
                            pMsg->stUnitDataInd.usPduLen);
        }
        break;
    default:
        SMC_SndGmmDataReq( SMC_DATA_TYPE_CP_ERR,
                           SMS_CP_ERR_MSG_TYPE_NOT_IMPLEMENT,
                  (VOS_UINT8)((((LL_SMS_UNITDATA_IND_MSG *)pRcvMsg)->stUnitDataInd.
                              aucPdu[0] & 0xf0) >> 4));                         /* ���ϲ㱨��                               */
        break;
    }
}
VOS_VOID SMC_RcvGmmMsgDistr(
                       VOS_VOID     *pRcvMsg                                        /* �յ�����Ϣ                               */
                       )
{
    switch(((MSG_HEADER_STRU *)pRcvMsg)->ulMsgName)
    {
    case GMMSMS_REG_STATE_IND:
        /* ע��״̬���� CBS ʹ�� */
        /* SMC_RcvGmmStaInd ((GMMSMS_REG_STATE_IND_STRU *)pRcvMsg);               ����ע��״̬��Ϣ�Ĵ���                   */
        break;
    case GMMSMS_SERVICE_STATUS_IND:
        SMC_RcvGmmServStatusInd((GMMSMS_SERVICE_STATUS_IND_STRU *)pRcvMsg);    /* ���÷���״̬�Ĵ���                        */
        break;
    case PMMSMS_EST_CNF:
        SMC_RcvGmmEstCnf ();                                                    /* ���ý���ȷ��ԭ��Ĵ���                   */
        break;
    case PMMSMS_ERROR_IND:
        SMC_RcvGmmErrInd ((PMMSMS_ERROR_IND_STRU *)pRcvMsg);                    /* ���ô�����                             */
        break;
    case PMMSMS_UNITDATA_IND:
        SMS_ReportN2MOtaMsg(&(((PMMSMS_UNITDATA_IND_STRU *)pRcvMsg)->SmsMsg));


        if((((PMMSMS_UNITDATA_IND_STRU *)pRcvMsg)->SmsMsg.aucNasMsg[0]>>4)< 8)
        {                                                                       /* ��mt����                                 */
            if(SMC_MT_IDLE != g_SmcCsEnt.SmcMt.ucState)
            {                                                                   /* ͬʱ������mt����                         */
                SMC_SndGmmDataReq(
                        SMC_DATA_TYPE_CP_ERR,
                        SMS_CP_ERR_CONGEST,
                        (VOS_UINT8)((((PMMSMS_UNITDATA_IND_STRU *)pRcvMsg)->SmsMsg.
                              aucNasMsg[0]>>4) )                                /* �����෢��CP_ERROR  TIȡ���յ���Ϣ��TI   */
                                );                                              /* �����౨��                               */
            }
            else
            {
                /* ����mt�Ĵ������ */
                SMC_RcvPsMtData(((PMMSMS_UNITDATA_IND_TEMP_STRU *)pRcvMsg)->SmsMsg.aucNasMsg,
                                ((PMMSMS_UNITDATA_IND_TEMP_STRU *)pRcvMsg)->SmsMsg.ulNasMsgSize);
            }
        }
        else
        {   /* ��mo���̣�����mo�Ĵ������ */
            SMC_RcvPsMoData(((PMMSMS_UNITDATA_IND_TEMP_STRU *)pRcvMsg)->SmsMsg.aucNasMsg,
                            ((PMMSMS_UNITDATA_IND_TEMP_STRU *)pRcvMsg)->SmsMsg.ulNasMsgSize);
        }
        break;
    default:
        SMC_SndGmmDataReq( SMC_DATA_TYPE_CP_ERR,
                           SMS_CP_ERR_MSG_TYPE_NOT_IMPLEMENT,
                  (VOS_UINT8)((((PMMSMS_UNITDATA_IND_STRU *)pRcvMsg)->SmsMsg.
                              aucNasMsg[0] & 0xf0) >> 4));                      /* ���ϲ㱨��                               */
        break;
    }
}

#if (FEATURE_ON == FEATURE_LTE)
VOS_VOID NAS_SMS_RcvLmmMsgDistr(
    VOS_VOID                           *pRcvMsg
)
{
    VOS_UINT32                          ulMsgId;

    /* ��ȡ��Ϣ���� */
    PS_MEM_CPY(&ulMsgId, (VOS_INT8*)pRcvMsg + VOS_MSG_HEAD_LENGTH, sizeof(VOS_UINT32));

    switch ( ulMsgId )
    {
        case ID_LMM_SMS_EST_CNF:
            NAS_SMS_ProcLmmEstCnf(pRcvMsg);
            break;

        case ID_LMM_SMS_DATA_IND:
            NAS_SMS_ProcLmmDataInd(pRcvMsg);
            break;

        case ID_LMM_SMS_ERR_IND:
            NAS_SMS_ProcLmmErrInd(pRcvMsg);
            break;

        default:
            NAS_WARNING_LOG(WUEPS_PID_SMS, "NAS_SMS_RcvLmmMsgDistr():Invalid SMS MsgId");
            break;
    }
}
#endif
VOS_VOID SMC_RcvMmMsgDistr(
                       VOS_VOID     *pRcvMsg                                        /* �յ�����Ϣ                               */
                       )
{
    switch(((MSG_HEADER_STRU *)pRcvMsg)->ulMsgName)
    {
    case MMSMS_REG_STATE_IND:
        SMC_RcvMmRegStaInd((MMSMS_REG_STATE_IND_STRU *)pRcvMsg);                /* ����ע��״̬��Ϣ�Ĵ���                   */
        break;
    case MMSMS_EST_CNF:
        SMC_RcvMmEstCnf((MMSMS_EST_CNF_STRU *)pRcvMsg);                         /* ���ý���ȷ��ԭ��Ĵ���                   */
        break;
    case MMSMS_EST_IND:
        SMS_ReportN2MOtaMsg(&(((MMSMS_EST_IND_STRU *)pRcvMsg)->SmsMsg));


        /* ���ý���ָʾԭ��Ĵ���                   */
        SMC_RcvMmEstInd(((MMSMS_EST_IND_STRU *)pRcvMsg)->SmsMsg.aucNasMsg,
                        ((MMSMS_EST_IND_STRU *)pRcvMsg)->SmsMsg.ulNasMsgSize );
        break;
    case MMSMS_REL_IND:
        SMC_RcvMmRelInd((MMSMS_REL_IND_STRU *)pRcvMsg);                         /* ���ô�����                             */
        break;
    case MMSMS_DATA_IND:
        SMS_ReportN2MOtaMsg(&(((MMSMS_DATA_IND_STRU *)pRcvMsg)->SmsMsg));


        if((((MMSMS_DATA_IND_STRU *)pRcvMsg)->SmsMsg.aucNasMsg[0] >> 4) < 8)
        {                                                                       /* ��mt����                                 */
            if(SMC_MT_IDLE != g_SmcPsEnt.SmcMt.ucState)
            {                                                                   /* ͬʱ������mt����                         */
                SMC_SndMmDataReq(
                        SMC_DATA_TYPE_CP_ERR,
                        SMS_CP_ERR_CONGEST,
                        (VOS_UINT8)((((MMSMS_DATA_IND_STRU *)pRcvMsg)->SmsMsg.
                                                        aucNasMsg[0]) >> 4));   /* �����෢��CP_ERROR  TIȡ���յ���Ϣ��TI   */
            }
            else
            {   /* ����mt�Ĵ������                         */
                SMC_RcvNwCsMtData(((MMSMS_DATA_IND_STRU *)pRcvMsg)->SmsMsg.aucNasMsg,
                                  ((MMSMS_DATA_IND_STRU *)pRcvMsg)->SmsMsg.ulNasMsgSize );
            }
        }
        else
        {   /* ��mo����                                 */
            SMC_RcvNwCsMoData(((MMSMS_DATA_IND_STRU *)pRcvMsg)->SmsMsg.aucNasMsg,
                              ((MMSMS_DATA_IND_STRU *)pRcvMsg)->SmsMsg.ulNasMsgSize );
        }
        break;
    case MMSMS_NACK_DATA_IND:
        NAS_SMS_RcvNackMsg((MMSMS_NACK_DATA_IND_STRU *)pRcvMsg);
        break;
    default:
        break;
    }
}

VOS_VOID SMS_TimerMsgDistr(VOS_UINT32 ulTimerId)
{
    NAS_TIMER_EventReport(ulTimerId, WUEPS_PID_SMS, NAS_OM_EVENT_TIMER_OPERATION_EXPIRED);    
    
    switch(ulTimerId)
    {
    case SMC_TIMER_ID_PS_TC1M_MT:
    case SMC_TIMER_ID_PS_TC1M_MO:
    case SMC_TIMER_ID_CS_TC1M_MT:
    case SMC_TIMER_ID_CS_TC1M_MO:
        PS_LOG(WUEPS_PID_SMS, VOS_NULL, PS_PRINT_INFO, "SMS_TimerMsgDistr:INFO:SMS TIMER EXPIRED: TC1M");
        SMC_TimerTc1mExp((VOS_UINT8)ulTimerId);                                     /* ���ö�ʱ��TC1M����Ĵ���                 */
        break;
    case SMC_TIMER_ID_TR1M:
        PS_LOG(WUEPS_PID_SMS, VOS_NULL, PS_PRINT_INFO, "SMS_TimerMsgDistr:INFO:SMS TIMER EXPIRED: TR1M");
        SMR_TimerTr1mExpire();                                                  /* ���ö�ʱ��TR1M����Ĵ���                 */
        break;
    case SMC_TIMER_ID_TR2M:
        PS_LOG(WUEPS_PID_SMS, VOS_NULL, PS_PRINT_INFO, "SMS_TimerMsgDistr:INFO:SMS TIMER EXPIRED: TR2M");
        SMR_TimerTr2mExpire();                                                  /* ���ö�ʱ��TR2M����Ĵ���                 */
        break;
    case SMC_TIMER_ID_TRAM:
        PS_LOG(WUEPS_PID_SMS, VOS_NULL, PS_PRINT_INFO, "SMS_TimerMsgDistr:INFO:SMS TIMER EXPIRED: TCAM");
        SMR_TimerTramExpire();                                                  /* ���ö�ʱ��TRAM����Ĵ���                 */
        break;

    default:
        break;
    }
    return;
}

VOS_UINT8 SMS_TimerStop(
                        VOS_UINT8       ucTimerId
                   )
{
    HTIMER*     pTimer;
    VOS_UINT32  ulTimerLength;
    VOS_UINT8*  pucTimerSta;
    VOS_UINT32  ulRet;


    pucTimerSta = VOS_NULL_PTR;



    pTimer = VOS_NULL_PTR;

    SMC_GetTimerInfo( ucTimerId,
                      &pTimer,
                      &ulTimerLength,
                      &pucTimerSta );

    if( SMS_TIMER_STATUS_RUN == *pucTimerSta )
    {
        *pucTimerSta = SMS_TIMER_STATUS_STOP;
        ulRet = NAS_StopRelTimer(WUEPS_PID_SMS, ucTimerId, pTimer );                                               /* ������ʱ�� */
        if( VOS_OK != ulRet )
        {
            SMS_LOG( PS_PRINT_ERROR,
                     "SMS:SMS_TimerStop: Timer stop fails" );
            return SMS_FALSE;
        }
    }

    return SMS_TRUE;
}
VOS_UINT8 SMS_TimerStart(
                    VOS_UINT8               ucTimerId
                    )
{
    HTIMER*     pTimer;
    VOS_UINT32  ulTimerLength;
    VOS_UINT8*  pucTimerSta;
    VOS_UINT32  ulRet;


    pucTimerSta = VOS_NULL_PTR;



    ulTimerLength = 0;
    pTimer        = VOS_NULL_PTR;

    SMC_GetTimerInfo( ucTimerId,
                      &pTimer,
                      &ulTimerLength,
                      &pucTimerSta );

    if( SMS_TIMER_STATUS_STOP == *pucTimerSta )
    {
        *pucTimerSta = SMS_TIMER_STATUS_RUN;
        ulRet = NAS_StartRelTimer( pTimer,
                                   WUEPS_PID_SMS,
                                   ulTimerLength,
                                   ucTimerId,
                                   0,
                                   VOS_RELTIMER_NOLOOP
                                   );                                               /* ������ʱ�� */
        if( VOS_OK != ulRet )
        {
            SMS_LOG( PS_PRINT_ERROR,
                     "SMS:SMS_TimerStart: Timer start fails" );
            return SMS_FALSE;
        }
    }

    return SMS_TRUE;
}
VOS_VOID SMS_GetTimerLength(
    SMS_TIMER_LENGTH_STRU              *pstTimerLength
)
{
    VOS_UINT32                          ulRet;
    SMS_NVIM_TIMER_LENGTH_STRU          stTimerLength;

    PS_MEM_SET(pstTimerLength, 0, sizeof(SMS_TIMER_LENGTH_STRU));
    PS_MEM_SET(&stTimerLength, 0, sizeof(stTimerLength));

    ulRet = NV_Read(en_NV_Item_SMS_TIMER_LENGTH,
                    &stTimerLength,
                    sizeof(stTimerLength));
    if ((NV_OK != ulRet)
     || (0 == stTimerLength.usTc1mLength))
    {
        pstTimerLength->usTc1mLength = SMC_TIMER_TC1M_LENGTH;
        pstTimerLength->usTr1mLength = SMR_TIMER_TR1M_LENGTH;
        pstTimerLength->usTr2mLength = SMR_TIMER_TR2M_LENGTH;
        pstTimerLength->usTramLength = SMR_TIMER_TRAM_LENGTH;
    }
    else
    {
        pstTimerLength->usTc1mLength = stTimerLength.usTc1mLength;
        pstTimerLength->usTr1mLength = stTimerLength.usTr1mLength;
        pstTimerLength->usTr2mLength = stTimerLength.usTr2mLength;
        pstTimerLength->usTramLength = stTimerLength.usTramLength;
    }

    return;
}


VOS_VOID SMS_GetCustomizedPara(VOS_VOID)
{
    SMS_TIMER_LENGTH_STRU               stTimerLength;
    VOS_UINT16                          usWaitRpAckTime;
    VOS_UINT16                          usCpDataTimes;

    SMS_GetTimerLength(&stTimerLength);

    usCpDataTimes    = stTimerLength.usTr1mLength/stTimerLength.usTc1mLength;

    /* �ȴ�RP-ACK��Ϣ��ʱ��: �ش���CP-DATA���ͺ���ҪԤ��ʱ�������ظ�RP-ACK */
    usWaitRpAckTime  = stTimerLength.usTr1mLength%stTimerLength.usTc1mLength;

    /* ���Ʋ�����Ч�Լ��: ���Ʋ�����Ч��ָ���Ĭ������ */
    if ((0 == usWaitRpAckTime)
     || (usCpDataTimes < SMC_CP_DATA_MIN_TIMES)
     || (usCpDataTimes > SMC_CP_DATA_MAX_TIMES))
    {
        f_ucNasSmsTc1mMaxExpTimes           = SMC_TC1M_EXP_PROTOCOL_MIN_TIMES;
        f_stNasSmsTimerLength.usTc1mLength  = SMC_TIMER_TC1M_LENGTH;
        f_stNasSmsTimerLength.usTr1mLength  = SMR_TIMER_TR1M_LENGTH;
        f_stNasSmsTimerLength.usTr2mLength  = SMR_TIMER_TR2M_LENGTH;
        f_stNasSmsTimerLength.usTramLength  = SMR_TIMER_TRAM_LENGTH;
        return;
    }

    /* �ش����� = ����CP-DATA���� - �״η��͵�һ��CP-DATA */
    f_ucNasSmsTc1mMaxExpTimes = (VOS_UINT8)(usCpDataTimes - 1);
    PS_MEM_CPY(&f_stNasSmsTimerLength, &stTimerLength, sizeof(f_stNasSmsTimerLength));

    return;
}
VOS_VOID SMS_GetTc1mTimeOutRetryFlag(
    VOS_UINT8                           ucExpireTimes,
    VOS_BOOL                           *pbRetryFlag
)
{
    if (ucExpireTimes < f_ucNasSmsTc1mMaxExpTimes)
    {
        *pbRetryFlag = VOS_TRUE;
    }
    else
    {
        *pbRetryFlag = VOS_FALSE;
    }

    return;
}


VOS_VOID SMC_GetTimerInfo( VOS_UINT8    ucTimerId,
                           HTIMER**     ppTimer,
                           VOS_UINT32*  pulTimerLength,
                           VOS_UINT8**  ppucTimerSta )
{

    switch( ucTimerId )
    {
        /* SMC�Ķ�ʱ�� */
        case SMC_TIMER_ID_PS_TC1M_MO:
            *ppTimer = &g_SmcPsEnt.SmcMo.TimerInfo.TimerId;
            *pulTimerLength = f_stNasSmsTimerLength.usTc1mLength;
            *ppucTimerSta = &g_SmcPsEnt.SmcMo.TimerInfo.ucTimerSta;
            break;
        case SMC_TIMER_ID_PS_TC1M_MT:
            *ppTimer = &g_SmcPsEnt.SmcMt.TimerInfo.TimerId;
            *pulTimerLength = f_stNasSmsTimerLength.usTc1mLength;
            *ppucTimerSta = &g_SmcPsEnt.SmcMt.TimerInfo.ucTimerSta;
            break;
        case SMC_TIMER_ID_CS_TC1M_MO:
            *ppTimer = &g_SmcCsEnt.SmcMo.TimerInfo.TimerId;
            *pulTimerLength = f_stNasSmsTimerLength.usTc1mLength;
            *ppucTimerSta = &g_SmcCsEnt.SmcMo.TimerInfo.ucTimerSta;
            break;
        case SMC_TIMER_ID_CS_TC1M_MT:
            *ppTimer = &g_SmcCsEnt.SmcMt.TimerInfo.TimerId;
            *pulTimerLength = f_stNasSmsTimerLength.usTc1mLength;
            *ppucTimerSta = &g_SmcCsEnt.SmcMt.TimerInfo.ucTimerSta;
            break;

        /* SMR�Ķ�ʱ�� */
        case SMC_TIMER_ID_TR1M:
            *ppTimer = &g_SmrEnt.SmrMo.aTimerInfo[0].TimerId;
            *pulTimerLength = f_stNasSmsTimerLength.usTr1mLength;
            *ppucTimerSta = &g_SmrEnt.SmrMo.aTimerInfo[0].ucTimerSta;
            break;
        case SMC_TIMER_ID_TR2M:
            *ppTimer = &g_SmrEnt.SmrMt.TimerInfo.TimerId;
            *pulTimerLength = f_stNasSmsTimerLength.usTr2mLength;
            *ppucTimerSta = &g_SmrEnt.SmrMt.TimerInfo.ucTimerSta;
            break;
        case SMC_TIMER_ID_TRAM:
            *ppTimer = &g_SmrEnt.SmrMo.aTimerInfo[1].TimerId;
            *pulTimerLength = f_stNasSmsTimerLength.usTramLength;
            *ppucTimerSta = &g_SmrEnt.SmrMo.aTimerInfo[1].ucTimerSta;
            break;

        default:
            SMS_LOG( PS_PRINT_WARNING,
                     "SMS:SMC_GetTimerInfo: Not valid timer" );
            break;
    }
}


VOS_UINT32 WuepsSmsPidInit ( enum VOS_INIT_PHASE_DEFINE ip )
{
    switch( ip )
    {
    case VOS_IP_LOAD_CONFIG:
        g_SmsStartFlg = SMS_TASK_START;                                     /* ����TASK SMS������־δ�Ѿ�����           */
        SMS_InitEntity();                                                   /* SMC SMR ʵ���ʼ��                       */

        SMS_GetCustomizedPara();
        break;

    case VOS_IP_FARMALLOC:
    case VOS_IP_INITIAL:
    case VOS_IP_ENROLLMENT:
    case VOS_IP_LOAD_DATA:
    case VOS_IP_FETCH_DATA:
    case VOS_IP_STARTUP:
    case VOS_IP_RIVAL:
    case VOS_IP_KICKOFF:
    case VOS_IP_STANDBY:
    case VOS_IP_BROADCAST_STATE:
    case VOS_IP_RESTART:
    case VOS_IP_BUTT:
        break;
    default:
        break;
    }

    return VOS_OK;
}


#ifdef  __cplusplus
  #if  __cplusplus
  }
  #endif
#endif

