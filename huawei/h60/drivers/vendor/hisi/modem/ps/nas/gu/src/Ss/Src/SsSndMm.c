/*******************************************************************************
  Copyright    : 2005-2007, Huawei Tech. Co., Ltd.
  File name    : SsSndMm.c
  Description  : ����MM��Ϣ�Ĵ���
  Function List:
      1.  Ss_SndMmMsg                 SS��MM������Ϣ
      2.  Ss_SndMmssEstReq            SS֪ͨMM����MM����
      3.  Ss_SndMmssDataReq           ��MM��������
      4.  Ss_SndMmssRelReq            SS�ø�ԭ��Ҫ��MM�ͷ�MM����
  History:
      1.  ��־�� 2004.03.09   �°�����
      2.��    ��   : 2007��06��04��
        ��    ��   : luojian id:60022475
        �޸�����   : ���ⵥ��:A32D11456
*******************************************************************************/

#include "SsInclude.h"
#include "NasOmInterface.h"

#ifdef  __cplusplus
  #if  __cplusplus
  extern "C"{
  #endif
#endif


/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_SS_SNDMM_C


VOS_VOID NAS_SS_ReportM2NOtaMsg(
    SS_MSG_STRU                         *pNasMsg
)
{
    NAS_OTA_MSG_ID_ENUM_UINT16          enNasOtaMsyId;
    VOS_UINT8                           ucMsgType;

    enNasOtaMsyId = NAS_OTA_MSG_ID_BUTT;

    /* Message Type�ֶ�����Ϣ�е�����λ�� */
    ucMsgType = pNasMsg->aucSsMsg[1];

    SS_NORM_LOG1("NAS_SS_ReportM2NOtaMsg: [MsgType]", pNasMsg->aucSsMsg[1]);

    /* ��NAS���пտ���ϢID���ϱ���OM����ϢID��Ӧ����ͨ��OM���й�ȡ��ʾ */
    switch(ucMsgType)
    {
        case SS_MSG_REGISTER:
            enNasOtaMsyId = NAS_OTA_MSG_SS_REGISTER_UP;
            break;
        case SS_MSG_FACILITY:
            enNasOtaMsyId = NAS_OTA_MSG_SS_FACILITY_UP;
            break;
        case SS_MSG_RELCMPL:
            enNasOtaMsyId = NAS_OTA_MSG_SS_RELCMPL_UP;
            break;
        default:
            break;
    }

    if (NAS_OTA_MSG_ID_BUTT == enNasOtaMsyId)
    {
        SS_WARN_LOG1("NAS_SS_ReportN2MOtaMsg: invalid msg type!", enNasOtaMsyId);
        /* ������SSģ��Ŀտ���Ϣ������trace����ʾ */
        return;
    }

    /*�ϱ��տ���Ϣ��OM*/
    NAS_SendAirMsgToOM(WUEPS_PID_SS,
                       enNasOtaMsyId,
                       NAS_OTA_DIRECTION_UP,
                       pNasMsg->ulSsMsgSize + 4,
                       (VOS_UINT8*)pNasMsg);

    return;
}



VOS_VOID Ss_SndMmssEstReq(
                      VOS_UINT8 ucTi                                                /* ��Ӧ��MM���ӵ�TI                         */
                      )
{
    VOS_VOID    *pMsg;                                                              /* ������Ϣ������                           */

    pMsg = PS_ALLOC_MSG(WUEPS_PID_SS, sizeof(MMSS_EST_REQ_STRU) - VOS_MSG_HEAD_LENGTH);
    if ( VOS_NULL_PTR == pMsg )
    {
        return;
    }


    (( MMSS_EST_REQ_STRU *)pMsg )->MsgHeader.ulReceiverPid = WUEPS_PID_MM;
    (( MMSS_EST_REQ_STRU *)pMsg )->MsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;       /* ������Ϣ����                             */
    (( MMSS_EST_REQ_STRU *)pMsg )->MsgHeader.ulMsgName = MMSS_EST_REQ;          /* ������Ϣ����                             */
    (( MMSS_EST_REQ_STRU *)pMsg )->ulTi = ucTi;                                 /* ���ö�Ӧ��MM���ӵ�TI                     */

    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_SS, pMsg))                                               /* ��MM����                                 */
    {
        NAS_WARNING_LOG(WUEPS_PID_SS, "Ss_SndMmssRelReq():WARNING:SEND MSG FIAL");
    }                                                        
}


VOS_VOID Ss_SndMmssDataReq(
                       VOS_UINT8 ucTi,                                              /* ��Ӧ��MM���ӵ�TI                         */
                       VOS_UINT8 *pucSsMsg,                                         /* ����������                               */
                       VOS_UINT32 ulSsMsgSize                                        /* ��Ϣ����                                 */
                       )
{
    VOS_VOID            *pMsg;                                                  /* ������Ϣ������                           */
    MMSS_DATA_REQ_STRU  *pSndMsg;
    VOS_UINT32   ulMsgLen;                                                           /* ������Ϣ���ȱ���                         */

    ulMsgLen = (sizeof( MMSS_DATA_REQ_STRU ) + ulSsMsgSize) - 4;                  /* ������Ϣ����                             */
    if ( ulMsgLen < sizeof( MMSS_DATA_REQ_STRU ) )
    {
        ulMsgLen = sizeof( MMSS_DATA_REQ_STRU );
    }

    pMsg = PS_ALLOC_MSG(WUEPS_PID_SS, ulMsgLen - VOS_MSG_HEAD_LENGTH);
    if ( VOS_NULL_PTR == pMsg )
    {
        return;
    }
    pSndMsg = (MMSS_DATA_REQ_STRU *)pMsg;
    pSndMsg->MsgHeader.ulReceiverPid = WUEPS_PID_MM;
    pSndMsg->MsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;                       /* ������Ϣ����                             */
    pSndMsg->MsgHeader.ulMsgName = MMSS_DATA_REQ;                               /* ������Ϣ����                             */
    pSndMsg->ulTi = ucTi;                                                       /* ���ö�Ӧ��MM���ӵ�TI                     */
    pSndMsg->SsMsg.ulSsMsgSize = ulSsMsgSize;                                   /* ������Ϣ����                             */
    Ss_MemCpy(pSndMsg->SsMsg.aucSsMsg,
        pucSsMsg, ulSsMsgSize );                                                /* ������Ϣ�е�����                         */

    /* ͨ��OM��ȡSS�տ���Ϣ */
    NAS_SS_ReportM2NOtaMsg(&pSndMsg->SsMsg);

    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_SS, pSndMsg))                           /* ��MM����                                 */
    {
        NAS_WARNING_LOG(WUEPS_PID_SS, "Ss_SndMmssRelReq():WARNING:SEND MSG FIAL");
    }
}

VOS_VOID Ss_SndMmssRelReq(
                      VOS_UINT8 ucTi                                                /* ��Ӧ��MM���ӵ�TI                         */
                      )
{
    VOS_VOID    *pMsg;                                                              /* ������Ϣ������                           */

    pMsg = PS_ALLOC_MSG(WUEPS_PID_SS, sizeof(MMSS_REL_REQ_STRU) - VOS_MSG_HEAD_LENGTH);
    if ( VOS_NULL_PTR == pMsg )
    {
        NAS_MSG_FAIL();
        return;
    }
    /*A32D11456==>*/
    (( MMSS_REL_REQ_STRU *)pMsg )->MsgHeader.ulReceiverPid = WUEPS_PID_MM;
    (( MMSS_REL_REQ_STRU *)pMsg )->MsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;       /* ������Ϣ����                             */
    /*<==A32D11456*/
    (( MMSS_REL_REQ_STRU *)pMsg )->MsgHeader.ulMsgName = MMSS_REL_REQ;          /* ������Ϣ����                             */
    (( MMSS_REL_REQ_STRU *)pMsg )->ulTi = ucTi;                                 /* ���ö�Ӧ��MM���ӵ�TI                     */

    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_SS, pMsg))                                               /* ��MM����                                 */
    {
        NAS_WARNING_LOG(WUEPS_PID_SS, "Ss_SndMmssRelReq():WARNING:SEND MSG FIAL");
    }                                                        /* ��MM����                                 */
}
VOS_VOID Ss_SndMmssAbortReq(
                      VOS_UINT8 ucTi                                                /* ��Ӧ��MM���ӵ�TI                         */
                      )
{
    VOS_VOID    *pMsg;                                                              /* ������Ϣ������                           */

    pMsg = PS_ALLOC_MSG(WUEPS_PID_SS, sizeof(MMSS_ABORT_REQ_STRU) - VOS_MSG_HEAD_LENGTH);
    if ( VOS_NULL_PTR == pMsg )
    {
        NAS_MSG_FAIL();
        return;
    }
    (( MMSS_ABORT_REQ_STRU *)pMsg )->MsgHeader.ulReceiverPid = WUEPS_PID_MM;
    (( MMSS_ABORT_REQ_STRU *)pMsg )->MsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;   /* ������Ϣ����                             */
    (( MMSS_ABORT_REQ_STRU *)pMsg )->MsgHeader.ulMsgName = MMSS_ABORT_REQ;          /* ������Ϣ����                             */
    (( MMSS_ABORT_REQ_STRU *)pMsg )->ulTransactionId = ucTi;                        /* ���ö�Ӧ��MM���ӵ�TI                     */

    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_SS, pMsg))                                               /* ��MM����                                 */
    {
        NAS_WARNING_LOG(WUEPS_PID_SS, "Ss_SndMmssAbortReq():WARNING:SEND MSG FIAL");
    }    
}


#ifdef  __cplusplus
  #if  __cplusplus
  }
  #endif
#endif

