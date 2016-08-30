
#include        "MM_Inc.h"
#include        "stdlib.h"
#include        "GmmMmInterface.h"
#include        "NasMmSndOm.h"

#include "NasNvInterface.h"
#include "TafNvInterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

extern MM_SUSPEND_MSG_BUF_ST gstMmSuspendMsgQue;
extern MM_TIMER_ST  gstMmTimerSuspend;

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_MM_COM_C



VOS_VOID Mm_ComNasInfoSav (
                       VOS_UINT8    ucAttachState,
                       VOS_UINT8    ucCksnFlg
                       )
{
    /* ����CSע��״̬ */
    if ( g_MmSubLyrShare.MmShare.ucCsAttachState != ucAttachState )
    {
        g_MmSubLyrShare.MmShare.ucCsAttachState = ucAttachState;
        Mm_SndRrNasInfoChgReq(MM_NAS_INFO_ATTACH_FLG);
        if ((g_MmGlobalInfo.usCauseVal != NAS_MML_REG_FAIL_CAUSE_NETWORK_FAILURE)
          &&(g_MmGlobalInfo.usCauseVal != NAS_MML_REG_FAIL_CAUSE_TIMER_TIMEOUT)
          &&(g_MmGlobalInfo.usCauseVal != NAS_MML_REG_FAIL_CAUSE_RR_CONN_EST_FAIL)
          &&(g_MmGlobalInfo.usCauseVal != NAS_MML_REG_FAIL_CAUSE_RR_CONN_FAIL)
          &&(g_MmGlobalInfo.usCauseVal != NAS_MML_REG_FAIL_CAUSE_ACCESS_BARRED))
        {
            Mm_SndSmsRegStsInd( g_MmSubLyrShare.MmShare.ucCsAttachState );
        }
    }
    else
    {
        if ((g_MmGlobalInfo.usCauseVal == NAS_MML_REG_FAIL_CAUSE_NETWORK_FAILURE)
          ||(g_MmGlobalInfo.usCauseVal == NAS_MML_REG_FAIL_CAUSE_TIMER_TIMEOUT)
          ||(g_MmGlobalInfo.usCauseVal == NAS_MML_REG_FAIL_CAUSE_RR_CONN_EST_FAIL)
          ||(g_MmGlobalInfo.usCauseVal == NAS_MML_REG_FAIL_CAUSE_RR_CONN_FAIL)
          ||(g_MmGlobalInfo.usCauseVal == NAS_MML_REG_FAIL_CAUSE_ACCESS_BARRED))
        {

            Mm_SndSmsRegStsInd(MM_STATUS_ATTACHED);
        }
    }


    if (MM_STATUS_ATTACHED == g_MmSubLyrShare.MmShare.ucCsAttachState)
    {
        Mm_SndRrNasInfoChgReq(MM_NAS_INFO_SYNC_AS_FLG);
    }


    /* ����CS��ȫ���� */
    if ( NAS_MML_GetSimCsSecurityCksn() != ucCksnFlg )
    {
        NAS_MML_SetSimCsSecurityCksn(ucCksnFlg);
        if ( MM_CKSN_INVALID == NAS_MML_GetSimCsSecurityCksn() )
        {
            NAS_MML_InitCsSecurityUmtsCkInvalid();
            NAS_MML_InitCsSecurityUmtsIkInvalid();

            Mm_SndRrNasInfoChgReq(MM_NAS_INFO_DEL_KEY_FLG);
        }
        else
        {
            if (NAS_MML_NET_RAT_TYPE_GSM == NAS_MML_GetCurrNetRatType())
            {
                Mm_SndRrNasInfoChgReq(MM_NAS_INFO_SK_FLG);
            }
        }
    }
    return;                                                                     /* ����                                     */
}
/*******************************************************************************
  Module:   Mm_ComLaiJudge
  Function: �ж�LAI�Ƿ�仯
  Input:    VOS_VOID
  Output:
  NOTE:
  Return:   MM_TRUE     LAI�仯
            MM_FALSE    LAI���仯
  History:
      1.  ��־��     2003.12.10  �°�����
*******************************************************************************/

VOS_UINT8 Mm_ComLaiJudge()
{
    VOS_UINT8               ucRet;

    if ( ( g_MmGlobalInfo.MsCsInfo.CurLai.PlmnId.ulMcc ==
        g_MmGlobalInfo.MsCsInfo.OldLai.PlmnId.ulMcc ) &&
        ( g_MmGlobalInfo.MsCsInfo.CurLai.PlmnId.ulMnc ==
        g_MmGlobalInfo.MsCsInfo.OldLai.PlmnId.ulMnc ) &&
        ( g_MmGlobalInfo.MsCsInfo.CurLai.ulLac ==
        g_MmGlobalInfo.MsCsInfo.OldLai.ulLac ) )
    {
        ucRet = MM_FALSE;
    }
    else
    {
        ucRet = MM_TRUE;
    }
    return ucRet;
}

/*******************************************************************************
  Module:   Mm_ComRandJudge
  Function: �ж�RAND�Ƿ�仯
  Input:    VOS_VOID
  Output:
  NOTE:
  Return:   MM_TRUE     RAND�仯
            MM_FALSE    RAND���仯
  History:
      1.  ��־��     2003.12.10  �°�����
*******************************************************************************/

VOS_UINT8 Mm_ComRandJudge()
{
    VOS_UINT8                       i;
    VOS_UINT8                   ucRet = MM_FALSE;

    for ( i = 0; i < 16; i++ )
    {
        if ( g_MmGlobalInfo.AuthenCtrlInfo.aucCurRand[i] !=
            g_MmGlobalInfo.AuthenCtrlInfo.aucOldRand[i] )
        {
            ucRet = MM_TRUE;
            break;
        }
    }
    return ucRet;

}


/*******************************************************************************
  Module:   Mm_ComSvcStaJudge
  Function: �жϷ���״̬�Ƿ�ı�
  Input:    VOS_UINT8 ucSvcStatus
  Output:
  NOTE:
  Return:   VOS_VOID
  History:
      1.  ��־��     2003.12.16  �°�����
*******************************************************************************/

VOS_VOID Mm_ComSvcStaJudge(VOS_UINT8 ucSvcStatus)
{
    if ( ucSvcStatus != g_MmGlobalInfo.ucMmServiceState )
    {
        g_MmGlobalInfo.ucMmServiceState = ucSvcStatus;
    }
}



VOS_VOID Mm_ComRelAllMmConn(VOS_UINT32 ulRelCause)
{
    VOS_UINT8                                               i;




    for ( i = 0; i < 7; i++ )
    {
        if ( 0 != (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            aucMMConnExtFlg[0] & (0x01 << i) ) )
        {
            /* ����CC������MM����                       */
            Mm_SndCcRelInd(i, ulRelCause);
        }
        if ( 0 != (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            aucMMConnExtFlg[1] & (0x01 << i) ) )
        {
            /* ����CC������MM����                       */
            Mm_SndCcRelInd(i + 8, ulRelCause);
        }
        if ( 0 != (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            aucMMConnReestFlg[0] & (0x01 << i) ) )
        {
            /* ����CC�ؽ�����MM����                     */
            Mm_SndCcRelInd(i, ulRelCause);
        }
        if ( 0 != (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            aucMMConnReestFlg[1] & (0x01 << i) ) )
        {
            /* ����CC�ؽ�����MM����                     */
            Mm_SndCcRelInd(i + 8, ulRelCause);
        }
        if ( 0 != (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].
            aucMMConnExtFlg[0] & (0x01 << i) ) )
        {                                                                       /* ����SS������MM����                       */
            Mm_SndSsRelInd(i, ulRelCause);
        }
        if ( 0 != (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].
            aucMMConnExtFlg[1] & (0x01 << i) ) )
        {                                                                       /* ����SS������MM����                       */
            Mm_SndSsRelInd(i + 8, ulRelCause);
        }
        if ( 0 != (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].
            aucMMConnExtFlg[0] & (0x01 << i) ) )
        {                                                                       /* ����SMS������MM����                      */
            Mm_SndSmsRelInd(i , ulRelCause );
        }
        if ( 0 != (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].
            aucMMConnExtFlg[1] & (0x01 << i) ) )
        {                                                                       /* ����SMS������MM����                      */
            Mm_SndSmsRelInd(i + 8, ulRelCause );
        }
    }

    
    if ( NO_MM_CONN_ESTING != g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI )
    {
        Mm_SndCcRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI,
                       ulRelCause);


        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg = MM_FALSE;
    }

    if ( MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg )
    {
        Mm_SndCcRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulTransactionId,
                       ulRelCause);
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg = MM_FALSE;
    }

    if ( NO_MM_CONN_ESTING != g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].ucMMConnEstingTI )
    {
        Mm_SndSsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].ucMMConnEstingTI, 
                        ulRelCause);
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg = MM_FALSE;
    }

    if ( MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg )
    {
        Mm_SndSsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ulTransactionId, 
                        ulRelCause);
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg = MM_FALSE;
    }



    if ( NO_MM_CONN_ESTING != g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].ucMMConnEstingTI )
    {
        Mm_SndSmsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].ucMMConnEstingTI,
                        ulRelCause);
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg = MM_FALSE;
    }

    if ( MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg )
    {
        Mm_SndSmsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ulTransactionId,
                        ulRelCause);
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg = MM_FALSE;
    }

    /* ��MM���Ӵ��ڱ�־���ó�û�� */
    g_MmGlobalInfo.ucMMConnEstingPD  = NO_MM_CONN_ESTING;
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI       = NO_MM_CONN_ESTING;
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].ucMMConnEstingTI       = NO_MM_CONN_ESTING;
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].ucMMConnEstingTI      = NO_MM_CONN_ESTING;
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnExtFlg[0]     = 0;
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnExtFlg[1]     = 0;
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].aucMMConnExtFlg[0]     = 0;
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].aucMMConnExtFlg[1]     = 0;
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].aucMMConnExtFlg[0]    = 0;
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].aucMMConnExtFlg[1]    = 0;
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnReestFlg[0]   = 0;
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnReestFlg[1]   = 0;

    NAS_MML_SetCsServiceConnStatusFlg(VOS_FALSE);

    NAS_MM_UpdateCsServiceBufferStatusFlg();

    /* CSFB���� */

    NAS_MML_SetRelCauseCsfbHighPrioFlg(VOS_FALSE);

    NAS_MML_SetCsfbServiceStatus(NAS_MML_CSFB_SERVICE_STATUS_NOT_EXIST);

    NAS_MML_SetCsEmergencyServiceFlg(VOS_FALSE);
}

VOS_VOID Mm_ComRelAllMmConnExcEmergencyCall(
    NAS_MMCM_REL_CAUSE_ENUM_UINT32      enMmccRelCause
)
{
    VOS_UINT8               i;

    for (i = 0; i < 7; i++)
    {
        /* �����жϽ����ֻ֧��һ��CC���е��������֧�ֶ��CC���й���ʱ��
           �������ж���һ�������ǽ����� */
        if ((0 != (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            aucMMConnExtFlg[0] & (0x01 << i)))
            && (MM_FALSE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            ucEstingCallTypeFlg))
        {
            /* ����CC�ؽ�����MM���� */
            Mm_SndCcRelInd(i, NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);
        }
        /* �����жϽ����ֻ֧��һ��CC���е��������֧�ֶ��CC���й���ʱ��
           �������ж���һ�������ǽ����� */
        if ((0 != (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            aucMMConnExtFlg[1] & (0x01 << i)))
            && (MM_FALSE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            ucEstingCallTypeFlg))
        {
            /* ����CC������MM���Ӳ��Ҳ��ǽ�����         */
            Mm_SndCcRelInd(i + 8, NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);
        }
        /* �����жϽ����ֻ֧��һ��CC���е��������֧�ֶ��CC���й���ʱ��
           �������ж���һ�������ǽ����� */
        if ((0 != (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            aucMMConnReestFlg[0] & (0x01 << i)))
            && (MM_FALSE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            ucEstingCallTypeFlg))
        {
            /* ����CC�ؽ�����MM����                     */
            Mm_SndCcRelInd(i, NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);
        }
        /* �����жϽ����ֻ֧��һ��CC���е��������֧�ֶ��CC���й���ʱ��
           �������ж���һ�������ǽ����� */
        if ((0 != (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            aucMMConnReestFlg[1] & (0x01 << i)))
            && (MM_FALSE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            ucEstingCallTypeFlg))
        {
            /* ����CC�ؽ�����MM����                     */
            Mm_SndCcRelInd(i + 8, NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);
        }
        if (0 != (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].
            aucMMConnExtFlg[0] & (0x01 << i)))
        {                                                                       /* ����SS������MM����                       */
            Mm_SndSsRelInd(i, NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);
        }
        if (0 != (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].
            aucMMConnExtFlg[1] & (0x01 << i)))
        {                                                                       /* ����SS������MM����                       */
            Mm_SndSsRelInd(i + 8, NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);
        }
        if (0 != (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].
            aucMMConnExtFlg[0] & (0x01 << i)))
        {                                                                       /* ����SMS������MM����                      */
            Mm_SndSmsRelInd(i , NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);
        }
        if (0 != (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].
            aucMMConnExtFlg[1] & (0x01 << i)))
        {                                                                       /* ����SMS������MM����                      */
            Mm_SndSmsRelInd(i + 8, NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);
        }
    }
    /* �����жϽ����ֻ֧��һ��CC���е��������֧�ֶ��CC���й���ʱ��
       �������ж���һ�������ǽ����� */
    if ((NO_MM_CONN_ESTING !=
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI)
        && (MM_FALSE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
        ucEstingCallTypeFlg))
    {
        /* ���ڽ���MM�����Ҳ��ǽ�������MM����       */
        Mm_SndCcRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
                       ucMMConnEstingTI,
                       NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);
    }
    if (NO_MM_CONN_ESTING !=
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].ucMMConnEstingTI)
    {
        Mm_SndSsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].
                       ucMMConnEstingTI, NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);
    }
    if (NO_MM_CONN_ESTING !=
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].ucMMConnEstingTI)
    {
        Mm_SndSmsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].
                        ucMMConnEstingTI,
                        NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);
    }
    /* �����жϽ����ֻ֧��һ��CC���е��������֧�ֶ��CC���й���ʱ��
       �������ж���һ�������ǽ����� */
    if ((MM_TRUE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg)
        && (MMCC_EMERGENCY_CALL != g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
        RcvXXEstReq.ulCallType))
    {
        /* �л����MM���ӽ�������                   */
        Mm_SndCcRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
                       RcvXXEstReq.ulTransactionId,
                       NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);                       /* ָʾ�����MM���ӽ���ʧ��                 */

        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg
            = MM_FALSE;                                                         /* ��������־                             */
    }
    if (MM_TRUE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg)
    {                                                                           /* �л����MM���ӽ�������                   */
        Mm_SndSmsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].
                        RcvXXEstReq.ulTransactionId,
                        NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);                        /* ָʾ�����MM���ӽ���ʧ��                 */
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg
            = MM_FALSE;                                                         /* ��������־                             */
    }
    if (MM_TRUE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg)
    {                                                                           /* �л����MM���ӽ�������                   */
        Mm_SndSsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].
                       RcvXXEstReq.ulTransactionId, NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);

        /* ָʾ�����MM���ӽ���ʧ��                 */
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg
            = MM_FALSE;                                                         /* ��������־                             */
    }


    /* �����жϽ����ֻ֧��һ��CC���е��������֧�ֶ��CC���й���ʱ��
       �������ж���һ�������ǽ����� */
    if (MM_FALSE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            ucEstingCallTypeFlg)
    {
        g_MmGlobalInfo.ucMMConnEstingPD = NO_MM_CONN_ESTING;
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI =
            NO_MM_CONN_ESTING;
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnExtFlg[0] = 0;
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnExtFlg[1] = 0;
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnReestFlg[0] = 0;
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnReestFlg[1] = 0;
    }
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].ucMMConnEstingTI =
        NO_MM_CONN_ESTING;
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].ucMMConnEstingTI =
        NO_MM_CONN_ESTING;

    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].aucMMConnExtFlg[0]  = 0;
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].aucMMConnExtFlg[1]  = 0;
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].aucMMConnExtFlg[0] = 0;
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].aucMMConnExtFlg[1] = 0;

    if ( ( MM_CONST_NUM_0 ==
      g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnExtFlg[0] )
      && ( MM_CONST_NUM_0 ==
      g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnExtFlg[1] ))
    {
        NAS_MML_SetCsServiceConnStatusFlg(VOS_FALSE);
    }

}



VOS_VOID Mm_ComRejectCause11(VOS_UINT8 ucType)
{
    /* Added by libin 05-03-22 begin: NAS_IT_BUG_017 */
    VOS_UINT8                               ucOldUpdateSts;
    VOS_UINT8                               ucTempUpdateSts;
    MM_LAI_STRU                         OldLai;
    MM_LAI_STRU                         TempLai;
    /* Added by libin 05-03-22 end */

    if (NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW == g_MmGlobalInfo.usCauseVal)
    {
        /* ע�ᱻ12�ܾ����ó��Ժ����ط� */
        Mm_ComRelAllMmConn(NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);
    }
    else
    {
        Mm_ComRelAllMmConn(NAS_MMCM_REL_CAUSE_MM_LIMIT_SERVICE);
    }

    g_MmGlobalInfo.ucMmServiceState = MM_LIMITED_SERVICE;                       /* ���÷���״̬                             */
    /* Modified by libin 05-03-22 begin: NAS_IT_BUG_017 */
    ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
    PS_MEM_CPY( &OldLai,
                 &g_MmGlobalInfo.MsCsInfo.OldLai,
                 sizeof( MM_LAI_STRU ));

    NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_LOCATION_AREA_NOT_ALLOWED);                  /* ���ø���״̬                             */

    switch(ucType)
    {
    case MM_ATTACH:
    case MM_RAU:
    case MM_NET_DETACH:
    case MM_SERVICE_REQ:
        Mm_ComNasInfoSav(
                        MM_STATUS_DETACHED,
                        MM_CKSN_INVALID
                        );                                                      /* ����MM��Ϣ                               */
        Mm_ComDelLai();                                                         /* ɾ��LAI                                  */
        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &= ~MM_MS_ID_TMSI_PRESENT;

        NAS_MML_InitUeIdTmsiInvalid();
        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ����SIM������״̬                        */

        /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);*/                     /* ����SIM������״̬                        */
        if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
        }
        else
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
        }
        break;
    default:
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_ComRejectCause11:WARNING: Type Abnormal");
        break;
    }
    ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
    PS_MEM_CPY( &TempLai,
                 &g_MmGlobalInfo.MsCsInfo.OldLai,
                 sizeof( MM_LAI_STRU ));
    NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
    PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                 &OldLai,
                 sizeof( MM_LAI_STRU ));
    NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
    PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                 &TempLai,
                 sizeof( MM_LAI_STRU ));
    /* Modified by libin 05-03-22 end */
    if ( MM_CS_SIG_CONN_PRESENT == g_MmGlobalInfo.ucCsSigConnFlg )
    {
        Mm_SndRrRelReq(RRC_CELL_UNBARRED);                                                       /* ��RRC����RRMM_REL_REQ(CS��)              */
        g_MmGlobalInfo.ucStaAfterWaitForNwkCmd =
                                                MM_IDLE_LIMITED_SERVICE;        /* �����ͷ�RR����ʱ������״̬             */
        Mm_TimerStart(MM_TIMER_T3240);
        Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);


        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComRejectCause11:NORMAL: STATUS is WAIT_FOR_NETWORK_COMMAND");
    }
    else
    {
        Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);


        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComRejectCause11:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
    }
}

VOS_VOID Mm_ComRejectCause12(VOS_UINT8 ucType)
{
    /* Added by libin 05-03-22 begin: NAS_IT_BUG_017 */
    VOS_UINT8                               ucOldUpdateSts;
    VOS_UINT8                               ucTempUpdateSts;
    MM_LAI_STRU                         OldLai;
    MM_LAI_STRU                         TempLai;
    /* Added by libin 05-03-22 end */

    Mm_ComRelAllMmConn(NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);

    g_MmGlobalInfo.ucMmServiceState = MM_LIMITED_SERVICE;                       /* ���÷���״̬                             */
    /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                              */
    /*" \nMM:MmServiceState is MM_LIMITED_SERVICE \r " );                       */
    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComRejectCause12:INFO: MmServiceState is MM_LIMITED_SERVICE");
    /* Modified by libin 05-03-22 begin: NAS_IT_BUG_017 */
    ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
    PS_MEM_CPY( &OldLai,
                 &g_MmGlobalInfo.MsCsInfo.OldLai,
                 sizeof( MM_LAI_STRU ));

    NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_LOCATION_AREA_NOT_ALLOWED);                  /* ���ø���״̬                             */

    switch(ucType)
    {
    case MM_ATTACH:
    case MM_RAU:
    case MM_NET_DETACH:
    case MM_SERVICE_REQ:
        Mm_ComNasInfoSav(
                        MM_STATUS_DETACHED,
                        MM_CKSN_INVALID
                        );                                                      /* ����MM��Ϣ                               */
        Mm_ComDelLai();                                                         /* ɾ��LAI                                  */
        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &= ~MM_MS_ID_TMSI_PRESENT;

        NAS_MML_InitUeIdTmsiInvalid();

        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ����SIM������״̬                        */

        /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);*/                     /* ����SIM������״̬                        */
        if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
        }
        else
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
        }
        break;
    default:
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_ComRejectCause12:WARNING: Type Abnormal");
        break;
    }
    ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
    PS_MEM_CPY( &TempLai,
                 &g_MmGlobalInfo.MsCsInfo.OldLai,
                 sizeof( MM_LAI_STRU ));
    NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
    PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                 &OldLai,
                 sizeof( MM_LAI_STRU ));
    NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
    PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                 &TempLai,
                 sizeof( MM_LAI_STRU ));
    /* Modified by libin 05-03-22 end */
    if ( MM_CS_SIG_CONN_PRESENT == g_MmGlobalInfo.ucCsSigConnFlg )
    {
        Mm_SndRrRelReq(RRC_CELL_UNBARRED);                                      /* ��RRC����RRMM_REL_REQ(CS��)              */
        g_MmGlobalInfo.ucStaAfterWaitForNwkCmd =
                                                MM_IDLE_LIMITED_SERVICE;        /* �����ͷ�RR����ʱ������״̬             */
        Mm_TimerStart(MM_TIMER_T3240);
        Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);
        /*WUEPS_TRACE( MM_LOG_LEVEL_1,                                          */
        /*" \nMM:STATUS is  WAIT_FOR_NETWORK_COMMAND\r " );                     */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComRejectCause12:NORMAL: STATUS is WAIT_FOR_NETWORK_COMMAND");
    }
    else
    {
        Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
        /*WUEPS_TRACE( MM_LOG_LEVEL_1,                                          */
        /*" \nMM:STATUS is  MM_IDLE_LIMITED_SERVICE\r " );                      */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComRejectCause12:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
    }
}



VOS_VOID Mm_ComRejectCause13(VOS_UINT8 ucType)
{
    /* Added by libin 05-03-22 begin: NAS_IT_BUG_017 */
    VOS_UINT8                               ucOldUpdateSts;
    VOS_UINT8                               ucTempUpdateSts;
    MM_LAI_STRU                         OldLai;
    MM_LAI_STRU                         TempLai;
    /* Added by libin 05-03-22 end */

    Mm_ComRelAllMmConn(NAS_MMCM_REL_CAUSE_MM_LIMIT_SERVICE);

    g_MmGlobalInfo.ucMmServiceState = MM_LIMITED_SERVICE;                       /* ���÷���״̬                             */
    /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                              */
    /*" \nMM:MmServiceState is MM_LIMITED_SERVICE \r " );                       */
    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComRejectCause13:INFO: MmServiceState is MM_LIMITED_SERVICE");
    /* Modified by libin 05-03-22 begin: NAS_IT_BUG_017 */
    ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
    PS_MEM_CPY( &OldLai,
                 &g_MmGlobalInfo.MsCsInfo.OldLai,
                 sizeof( MM_LAI_STRU ));

    NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_LOCATION_AREA_NOT_ALLOWED);                  /* ���ø���״̬                             */

    switch(ucType)
    {
    case MM_ATTACH:
        Mm_ComNasInfoSav(
                        MM_STATUS_DETACHED,
                        MM_CKSN_INVALID
                        );                                                      /* ����MM��Ϣ                               */
        Mm_ComDelLai();                                                         /* ɾ��LAI                                  */
        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &= ~MM_MS_ID_TMSI_PRESENT;

        NAS_MML_InitUeIdTmsiInvalid();

        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ����SIM������״̬                        */

        /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);*/                     /* ����SIM������״̬                        */
        if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
        }
        else
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
        }

        break;
    case MM_RAU:
        Mm_ComNasInfoSav(
                        MM_STATUS_DETACHED,
                        NAS_MML_GetSimCsSecurityCksn()
                        );                                                      /* ����MM��Ϣ                               */
        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ����SIM������״̬                        */
        break;
    case MM_NET_DETACH:
        Mm_ComNasInfoSav(
                        MM_STATUS_DETACHED,
                        MM_CKSN_INVALID
                        );                                                      /* ����MM��Ϣ                               */
        Mm_ComDelLai();                                                         /* ɾ��LAI                                  */
        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &= ~MM_MS_ID_TMSI_PRESENT;

        NAS_MML_InitUeIdTmsiInvalid();

        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ����SIM������״̬                        */

        /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID); */                    /* ����SIM������״̬                        */
        if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
        }
        else
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
        }
        break;
    case MM_SERVICE_REQ:
        Mm_ComNasInfoSav(
                        MM_STATUS_DETACHED,
                        NAS_MML_GetSimCsSecurityCksn()
                        );                                                      /* ����MM��Ϣ                               */
        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ����SIM������״̬                        */
        break;
    default:
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_ComRejectCause13:WARNING: Type Abnormal");
        break;
    }
    ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
    PS_MEM_CPY( &TempLai,
                 &g_MmGlobalInfo.MsCsInfo.OldLai,
                 sizeof( MM_LAI_STRU ));
    NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
    PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                 &OldLai,
                 sizeof( MM_LAI_STRU ));
    NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
    PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                 &TempLai,
                 sizeof( MM_LAI_STRU ));
    /* Modified by libin 05-03-22 end */
    if ( MM_CS_SIG_CONN_PRESENT == g_MmGlobalInfo.ucCsSigConnFlg )
    {
        Mm_SndRrRelReq(RRC_CELL_UNBARRED);                                      /* ��RRC����RRMM_REL_REQ(CS��)              */
        g_MmGlobalInfo.ucStaAfterWaitForNwkCmd =
                                                MM_IDLE_LIMITED_SERVICE;        /* �����ͷ�RR����ʱ������״̬             */
        Mm_TimerStart(MM_TIMER_T3240);
        Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);


        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComRejectCause13:NORMAL: STATUS is WAIT_FOR_NETWORK_COMMAND");
    }
    else
    {
        Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);


        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComRejectCause13:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
    }
}



VOS_VOID Mm_ComRejectCause15(VOS_UINT8 ucType)
{
    /* Added by libin 05-03-22 begin: NAS_IT_BUG_017 */
    VOS_UINT8                               ucOldUpdateSts;
    VOS_UINT8                               ucTempUpdateSts;
    MM_LAI_STRU                         OldLai;
    MM_LAI_STRU                         TempLai;
    /* Added by libin 05-03-22 end */

    Mm_ComRelAllMmConn(NAS_MMCM_REL_CAUSE_MM_LIMIT_SERVICE);

    g_MmGlobalInfo.ucMmServiceState = MM_LIMITED_SERVICE;                       /* ���÷���״̬                             */
    /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                              */
    /*" \nMM:MmServiceState is MM_LIMITED_SERVICE \r " );                       */
    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComRejectCause15:INFO: MmServiceState is MM_LIMITED_SERVICE");
    /* Modified by libin 05-03-22 begin: NAS_IT_BUG_017 */
    ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
    PS_MEM_CPY( &OldLai,
                 &g_MmGlobalInfo.MsCsInfo.OldLai,
                 sizeof( MM_LAI_STRU ));

    NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_LOCATION_AREA_NOT_ALLOWED);                  /* ���ø���״̬                             */

    switch(ucType)
    {
    case MM_ATTACH:
        Mm_ComNasInfoSav(
                        MM_STATUS_DETACHED,
                        MM_CKSN_INVALID
                        );                                                      /* ����MM��Ϣ                               */
        Mm_ComDelLai();                                                         /* ɾ��LAI                                  */
        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &= ~MM_MS_ID_TMSI_PRESENT;

        NAS_MML_InitUeIdTmsiInvalid();

        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ����SIM������״̬                        */

        /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID); */                    /* ����SIM������״̬                        */
        if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
        }
        else
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
        }
        break;
    case MM_RAU:
        Mm_ComNasInfoSav(
                        MM_STATUS_DETACHED,
                        NAS_MML_GetSimCsSecurityCksn()
                        );                                                      /* ����MM��Ϣ                               */
        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ����SIM������״̬                        */
        break;
    case MM_NET_DETACH:
        Mm_ComNasInfoSav(
                        MM_STATUS_DETACHED,
                        MM_CKSN_INVALID
                        );                                                      /* ����MM��Ϣ                               */
        Mm_ComDelLai();                                                         /* ɾ��LAI                                  */
        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &= ~MM_MS_ID_TMSI_PRESENT;

        NAS_MML_InitUeIdTmsiInvalid();

        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ����SIM������״̬                        */
        /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);*/                     /* ����SIM������״̬                        */
        if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
        }
        else
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
        }
        break;
    case MM_SERVICE_REQ:
        Mm_ComNasInfoSav(
                        MM_STATUS_DETACHED,
                        NAS_MML_GetSimCsSecurityCksn()
                        );                                                      /* ����MM��Ϣ                               */
        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ����SIM������״̬                        */
        break;
    default:
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_ComRejectCause15:WARNING: Type Abnormal");
        break;
    }
    ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
    PS_MEM_CPY( &TempLai,
                 &g_MmGlobalInfo.MsCsInfo.OldLai,
                 sizeof( MM_LAI_STRU ));
    NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
    PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                 &OldLai,
                 sizeof( MM_LAI_STRU ));
    NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
    PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                 &TempLai,
                 sizeof( MM_LAI_STRU ));
    /* Modified by libin 05-03-22 end */
    if ( MM_CS_SIG_CONN_PRESENT == g_MmGlobalInfo.ucCsSigConnFlg )
    {
        Mm_SndRrRelReq(RRC_CELL_UNBARRED);                                      /* ��RRC����RRMM_REL_REQ(CS��)              */
        g_MmGlobalInfo.ucStaAfterWaitForNwkCmd =
                                                MM_IDLE_LIMITED_SERVICE;        /* �����ͷ�RR����ʱ������״̬             */
        Mm_TimerStart(MM_TIMER_T3240);
        Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);
        /*WUEPS_TRACE( MM_LOG_LEVEL_1,                                          */
        /*" \nMM:STATUS is  WAIT_FOR_NETWORK_COMMAND\r " );                     */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComRejectCause15:NORMAL: STATUS is WAIT_FOR_NETWORK_COMMAND");
    }
    else
    {
        Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
        /*WUEPS_TRACE( MM_LOG_LEVEL_1,                                          */
        /*" \nMM:STATUS is  MM_IDLE_LIMITED_SERVICE\r " );                      */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComRejectCause15:NORMAL: STATUS is  MM_IDLE_LIMITED_SERVICE");
    }
}



VOS_VOID Mm_ComAuthenRcvAuthenRej()
{
    VOS_UINT32                          ulRet;

    ulRet = PS_USIM_SERVICE_NOT_AVAILIABLE;

    Mm_TimerStop(MM_TIMER_T3210);
    Mm_TimerStop(MM_TIMER_T3230);

    Mm_ComRelAllMmConnExcEmergencyCall(NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);

    Mm_ComSaveProcAndCauseVal(MM_AUTH_FAIL_PROC, NAS_MML_REG_FAIL_CAUSE_NULL);            /* ��¼���̺�ԭ��ֵ                         */
    g_MmGlobalInfo.ucMmServiceState = MM_NO_IMSI;                               /* ���÷���״̬                             */
    /*WUEPS_TRACE( MM_LOG_LEVEL_6, " \nMM:MmServiceState is MM_NO_IMSI \r " );  */
    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComAuthenRcvAuthenRej:INFO: MmServiceState is MM_NO_IMSI");
    g_MmGlobalInfo.AuthenCtrlInfo.ucAuthenAttmptCnt = 0;
    g_MmGlobalInfo.AuthenCtrlInfo.ucLastFailCause = MM_AUTHEN_NO_CAUSE;
    NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_PLMN_NOT_ALLOWED);                  /* ���ø���״̬                             */
    NAS_MML_SetSimCsRegStatus(VOS_FALSE);
    Mm_ComNasInfoSav(MM_STATUS_DETACHED, MM_CKSN_INVALID);
    Mm_ComDelLai();                                                             /* ɾ��LAI                                  */
    g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &= ~MM_MS_ID_TMSI_PRESENT;
    NAS_MML_InitUeIdTmsiInvalid();
    g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                            ~MM_MS_ID_IMSI_PRESENT;

    Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                    /* ����SIM������״̬                        */

    /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);*/                     /* ����SIM������״̬                        */
    if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
    {
        Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
    }
    else
    {
        Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */

        /* DCM MM-18-01-22 Ҫ�����CS:4F20�ļ���PS����4F52�ļ� */
        ulRet = NAS_USIMMAPI_IsServiceAvailable(NAS_USIM_SVR_GSM_ACCESS_IN_USIM);        

        if (PS_USIM_SERVICE_AVAILIABLE == ulRet)
        {
            Mm_SndAgentUsimUpdateFileReq(NAS_MML_READ_USIM_CS_KC_FILE_ID);
        }
    }

    Mm_TimerStop(MM_TIMER_T3218);                                               /* ͣT3218                                  */
    Mm_ComDelRandRes();                                                         /* ɾ��RAND��RES                            */

    if ( MM_CS_SIG_CONN_PRESENT == g_MmGlobalInfo.ucCsSigConnFlg )
    {
        Mm_TimerStart(MM_TIMER_T3240);
        Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);

        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComAuthenRcvAuthenRej:NORMAL: STATUS is WAIT_FOR_NETWORK_COMMAND");
    }
    else
    {
        Mm_ComSetMmState(MM_IDLE_NO_IMSI);


        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComAuthenRcvAuthenRej:NORMAL: STATUS is MM_IDLE_NO_IMSI");
        Mm_ComProcCauseClear();
        /* A32D10964 ==> */
        if (MM_DETACH_NEED_DELAY == g_MmGlobalInfo.ucDetachType)
        {
            Mm_TimerStop(MM_TIMER_PROTECT_DETACH);
            g_MmGlobalInfo.ucDetachType = MM_DETACH_TYPE_BUTT;
            Mm_Cell_S0_E23(VOS_NULL_PTR);
        }
        /* <== A32D10964 */
    }
}




VOS_VOID Mm_ComTmsiReloc()
{
    NAS_MML_LAI_STRU                   *pstCsSuccLai;

    pstCsSuccLai    = NAS_MML_GetCsLastSuccLai(  );


    pstCsSuccLai->stPlmnId.ulMcc
        = g_MmMsgTmsiReallocCmd.MmIeLAI.IeLai.PlmnId.ulMcc;
    pstCsSuccLai->stPlmnId.ulMnc
        = g_MmMsgTmsiReallocCmd.MmIeLAI.IeLai.PlmnId.ulMnc;
    pstCsSuccLai->aucLac[0] = (g_MmMsgTmsiReallocCmd.MmIeLAI.IeLai.ulLac >> 8) & (0x00FF);
    pstCsSuccLai->aucLac[1] = (g_MmMsgTmsiReallocCmd.MmIeLAI.IeLai.ulLac) & (0x00FF);

    PS_MEM_CPY(
               &g_MmGlobalInfo.MsCsInfo.OldLai,
               &g_MmMsgTmsiReallocCmd.MmIeLAI.IeLai,
               sizeof( MM_LAI_STRU ));

    PS_MEM_CPY(
               &g_MmGlobalInfo.MsCsInfo.CurLai,
               &g_MmMsgTmsiReallocCmd.MmIeLAI.IeLai,
               sizeof( MM_LAI_STRU ));

    if ( MM_MS_ID_IMSI_PRESENT ==
        ( g_MmMsgTmsiReallocCmd.MmIeMobileId.MobileID.ucMsIdFlg &
                                                    MM_MS_ID_IMSI_PRESENT ) )
    {
        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                                ~MM_MS_ID_TMSI_PRESENT;         /* ����TMSI��Ч                             */

        NAS_MML_InitUeIdTmsiInvalid();

        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg |= MM_MS_ID_IMSI_PRESENT;    /* ����IMSI��Ч                             */

        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ��USIM����AGENT_USIM_UPDATE_FILE_REQ     */
    }
    else
    {
        if ( MM_MS_ID_TMSI_PRESENT ==

                    ( g_MmMsgTmsiReallocCmd.MmIeMobileId.MobileID.ucMsIdFlg &
                                                MM_MS_ID_TMSI_PRESENT ) )
        {
            g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg |= MM_MS_ID_TMSI_PRESENT;
            g_MmGlobalInfo.MsCsInfo.stPlmnId = g_MmGlobalInfo.MsCsInfo.OldLai.PlmnId;

            PS_MEM_CPY(NAS_MML_GetUeIdTmsi(),
                       g_MmMsgTmsiReallocCmd.MmIeMobileId.MobileID.aucTmsi,
                       NAS_MML_MAX_TMSI_LEN);

            Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);            /* ��USIM����AGENT_USIM_UPDATE_FILE_REQ     */
        }

    }
    Mm_ComMsgTmsiRelocCpltSnd();                                                /* ����TMSI REALLOCATION COMPLETE           */
}



VOS_VOID Mm_ComInfo()
{

    Mm_SndMmcInfoInd();
}

/*******************************************************************************
  Module:   Mm_ComIdentity
  Function: IDENTITY���̵Ĵ���
  Input:    VOS_VOID
  Output:
  NOTE:
  Return:   VOS_VOID
  History:
      1.  ��־��     2003.12.10  �°�����
*******************************************************************************/

VOS_VOID Mm_ComIdentity()
{
    MM_MSG_ID_RSP_STRU    Msg;

    NAS_MM_FillMobileID(&(Msg.MmIeMobileId.MobileID));

    switch( g_MmMsgIdReq.MmIeIdType.ucIDType )
    {
    case MM_IE_MI_ID_TYPE_IMSI:
        if ( 0 != ( g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg
            & MM_MS_ID_IMSI_PRESENT ) )
        {
            Msg.MmIeMobileId.MobileID.ucMsIdFlg = MM_MS_ID_IMSI_PRESENT;
        }
        else
        {
            Msg.MmIeMobileId.MobileID.ucMsIdFlg = MM_MS_ID_ABSENT;
        }
        break;
    case MM_IE_MI_ID_TYPE_IMEI:
        Msg.MmIeMobileId.MobileID.ucMsIdFlg = MM_MS_ID_IMEI_PRESENT;
        break;
    case MM_IE_MI_ID_TYPE_IMEISV:
        Msg.MmIeMobileId.MobileID.ucMsIdFlg = MM_MS_ID_IMEISV_PRESENT;
        break;
    case MM_IE_MI_ID_TYPE_TMSI:
        if ( 0 != ( g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg
            & MM_MS_ID_TMSI_PRESENT ) )
        {
            Msg.MmIeMobileId.MobileID.ucMsIdFlg = MM_MS_ID_TMSI_PRESENT;
        }
        else
        {
            Msg.MmIeMobileId.MobileID.ucMsIdFlg = MM_MS_ID_ABSENT;
        }
        break;
    default:
        break;
    }
    Mm_ComMsgIdRspSnd(&Msg);                                                    /* IDENTITY RESPONSE��Ϣ                    */
}

VOS_VOID Mm_ComAbortCause6()
{
    /* Added by libin 05-03-22 begin: NAS_IT_BUG_017 */
    VOS_UINT8                               ucOldUpdateSts;
    VOS_UINT8                               ucTempUpdateSts;
    MM_LAI_STRU                         OldLai;
    MM_LAI_STRU                         TempLai;
    /* Added by libin 05-03-22 end */

    Mm_ComRelAllMmConnExcEmergencyCall(NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);

    Mm_ComSaveProcAndCauseVal(MM_MM_NET_ABORT, NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME);        /* ��¼���̺�ԭ��ֵ                         */
    g_MmGlobalInfo.ucMmServiceState = MM_NO_IMSI;                               /* ���÷���״̬                             */
    /*WUEPS_TRACE( MM_LOG_LEVEL_6, " \nMM:MmServiceState is MM_NO_IMSI \r " );  */
    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComAbortCause6:INFO: MmServiceState is MM_NO_IMSI");
    /* Modified by libin 05-03-22 begin: NAS_IT_BUG_017 */
    ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
    PS_MEM_CPY( &OldLai,
                 &g_MmGlobalInfo.MsCsInfo.OldLai,
                 sizeof( MM_LAI_STRU ));

    NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_PLMN_NOT_ALLOWED);                  /* ���ø���״̬                             */

    NAS_MML_SetSimCsRegStatus(VOS_FALSE);
    Mm_ComNasInfoSav(MM_STATUS_DETACHED, MM_CKSN_INVALID);
    Mm_ComDelLai();                                                             /* ɾ��LAI                                  */
    g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &= ~MM_MS_ID_TMSI_PRESENT;

    NAS_MML_InitUeIdTmsiInvalid();

    g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                            ~MM_MS_ID_IMSI_PRESENT;

    Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                    /* ����SIM������״̬                        */

    /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);*/                     /* ����SIM������״̬                        */
    if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
    {
        Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
    }
    else
    {
        Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
    }

    ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
    PS_MEM_CPY( &TempLai,
                 &g_MmGlobalInfo.MsCsInfo.OldLai,
                 sizeof( MM_LAI_STRU ));
    NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
    PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                 &OldLai,
                 sizeof( MM_LAI_STRU ));
    NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
    PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                 &TempLai,
                 sizeof( MM_LAI_STRU ));
    /* Modified by libin 05-03-22 end */
    g_MmGlobalInfo.ucStaAfterWaitForNwkCmd = MM_IDLE_NO_IMSI;                   /* �����ͷ�RR����ʱ������״̬             */
    Mm_TimerStart(MM_TIMER_T3240);
    /* Mm_SndMmcCmSvcRejInd(NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME); */
    Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);
    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComAbortCause6:NORMAL: STATUS is WAIT_FOR_NETWORK_COMMAND");
}

VOS_VOID Mm_ComAbortCause17()
{
    Mm_ComRelAllMmConn(NAS_MMCM_REL_CAUSE_MM_WRONG_STATE);

    Mm_ComSaveProcAndCauseVal(MM_MM_NET_ABORT, NAS_MML_REG_FAIL_CAUSE_NETWORK_FAILURE);   /* ��¼���̺�ԭ��ֵ                         */
    g_MmGlobalInfo.ucStaAfterWaitForNwkCmd = MM_IDLE_NORMAL_SERVICE;            /* �����ͷ�RR����ʱ������״̬             */
    Mm_TimerStart(MM_TIMER_T3240);
    Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);
    /*WUEPS_TRACE( MM_LOG_LEVEL_1,                                              */
    /*" \nMM:STATUS is  WAIT_FOR_NETWORK_COMMAND\r " );                         */
    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComAbortCause17:NORMAL: STATUS is WAIT_FOR_NETWORK_COMMAND");
}

/*******************************************************************************
  Module:   Mm_ComT3214orT3216Expired
  Function: T3214����T3216����Ĵ���
  Input:    VOS_VOID
  Output:
  NOTE:
  Return:   VOS_VOID
  History:
      1.  ��־��     2003.12.12  �°�����
******************************************************************************/

VOS_VOID Mm_ComT3214orT3216Expired()
{
    Mm_ComAuthMsRej();
    /* g_MmGlobalInfo.AuthenCtrlInfo.ucAuthenAttmptCnt = 0;
    g_MmGlobalInfo.AuthenCtrlInfo.ucLastFailCause = MM_AUTHEN_NO_CAUSE;
    Mm_TimerStop(MM_TIMER_T3214);
    Mm_TimerStop(MM_TIMER_T3216);
    Mm_ComRelAllMmConn();                                                     */ /* �ͷ����ڽ������Ѿ�������MM����           */
    /* Mm_SndRrAbortReq(RRC_ABORT_RRC_CONN);                                  */ /* ����RRMM_ABORT_REQ(RRC����)              */

    /* g_MmGlobalInfo.ucState = WAIT_FOR_NETWORK_COMMAND;
    Mm_TimerStart(MM_TIMER_T3240); */

}

/*******************************************************************************
  Module:   Mm_ComDelRandRes
  Function: ɾ��RAND��RES�Ĵ���
  Input:    VOS_VOID
  Output:
  NOTE:
  Return:   VOS_VOID
  History:
      1.  ��  ��   2004.01.07   �°�����
      2.  ��־��   2004.10.25   ���ݴ洢�ṹ�����Ӧ
*******************************************************************************/

VOS_VOID Mm_ComDelRandRes()
{
    VOS_UINT8                   i;

    g_MmGlobalInfo.AuthenCtrlInfo.ucResFlg = MM_AUTHEN_RES_ABSENT;

    g_MmGlobalInfo.AuthenCtrlInfo.ucSresFlg = MM_AUTHEN_RES_ABSENT;

    /* Modified by ��־�� 2004.10.25 begin */
    /* g_MmGlobalInfo.AuthenCtrlInfo.ulRes = 0; */
    PS_MEM_SET(g_MmGlobalInfo.AuthenCtrlInfo.aucRes, 0, 4);
    /* Modified by ��־�� 2004.10.25 end */
    g_MmGlobalInfo.AuthenCtrlInfo.ucExtRspLength = 0;
    /* Modified by ��־�� 2004.10.25 begin */
    /* g_MmGlobalInfo.AuthenCtrlInfo.aulExtRes[0] = 0;
       g_MmGlobalInfo.AuthenCtrlInfo.aulExtRes[1] = 0;
       g_MmGlobalInfo.AuthenCtrlInfo.aulExtRes[2] = 0; */
    PS_MEM_SET(g_MmGlobalInfo.AuthenCtrlInfo.aucExtRes, 0, 12);
    /* Modified by ��־�� 2004.10.25 end */
    g_MmGlobalInfo.AuthenCtrlInfo.ucRandFlg = MM_AUTHEN_RAND_ABSENT;
    for ( i = 0; i < 16; i ++ )
    {
        g_MmGlobalInfo.AuthenCtrlInfo.aucCurRand[i] = 0;
        g_MmGlobalInfo.AuthenCtrlInfo.aucOldRand[i] = 0;
    }
}

/*******************************************************************************
  Module:   Mm_ComT3218Expired
  Function: T3218����Ĵ���
  Input:    VOS_VOID
  Output:
  NOTE:
  Return:   VOS_VOID
  History:
      1.  ��־��     2003.12.12  �°�����
*******************************************************************************/

VOS_VOID Mm_ComT3218Expired()
{
    Mm_TimerStop(MM_TIMER_T3218);
    Mm_ComDelRandRes();

}



VOS_VOID Mm_ComAuthenNoAutn()
{
    MM_MSG_AUTHENTICATION_FAIL_STRU     Msg;
    VOS_INT8                            cVersion;
    VOS_UINT8                           ucAuthFailFlg;

    ucAuthFailFlg   = VOS_FALSE;
    cVersion        = NAS_Common_Get_Supported_3GPP_Version(MM_COM_SRVDOMAIN_CS);


    Msg.MmIeRejCause.ucRejCause = NAS_MML_REG_FAIL_CAUSE_GSM_AUT_UNACCEPTABLE;

    Mm_ComMsgAuthFailSnd(&Msg);                                                 /* ����AUTHENTICATION FAILURE��Ϣ           */
    NAS_MM_SndMmcSimAuthFailInfo(NAS_MML_SIM_AUTH_FAIL_GSM_AUT_UNACCEPTABLE);

    Mm_TimerStop(MM_TIMER_T3218);                                               /* ͣT3218                                  */
    Mm_ComDelRandRes();                                                         /* ɾ��RAND��RES                            */

    g_MmGlobalInfo.AuthenCtrlInfo.ucAuthenAttmptCnt++;

    /* R6�汾�Ժ��������μ�Ȩʧ��UE������Ϊ��Ȩʧ�� */
    if ((PS_PTL_VER_R6  <= cVersion)
     && (MM_CONST_NUM_3 == g_MmGlobalInfo.AuthenCtrlInfo.ucAuthenAttmptCnt))
    {
        ucAuthFailFlg = VOS_TRUE;
    }

    /* �˴�ʵ��ԭ���ο�R5Э���������μ�Ȩʧ��UE������Ϊ��Ȩʧ�� */
    if ((PS_PTL_VER_R5  >= cVersion)
     && ((MM_CONST_NUM_3   == g_MmGlobalInfo.AuthenCtrlInfo.ucAuthenAttmptCnt)
      || (MM_MAC_CODE_FAIL == g_MmGlobalInfo.AuthenCtrlInfo.ucLastFailCause)))
    {
        ucAuthFailFlg = VOS_TRUE;
    }

    if (VOS_TRUE == ucAuthFailFlg)
    {
        /* Modified by libin for MM_Review_HW_BUG_012 20050214 begin */
        /* g_MmGlobalInfo.AuthenCtrlInfo.ucAuthenAttmptCnt = 0; */
        /* g_MmGlobalInfo.AuthenCtrlInfo.ucLastFailCause = MM_AUTHEN_NO_CAUSE; */
        /* Mm_ComRelAllMmConn();  */                                            /* �ͷ����ڽ������Ѿ�������MM����           */
        /* Mm_SndRrAbortReq(RRC_ABORT_RRC_CONN);  */                            /* ����RRMM_ABORT_REQ(RRC����)              */
        /* */
        /* g_MmGlobalInfo.ucState = WAIT_FOR_NETWORK_COMMAND; */
        /* Mm_TimerStart(MM_TIMER_T3240); */
        Mm_ComAuthMsRej();
        /* Modified by libin for MM_Review_HW_BUG_012 20050214 end */

    }
    else
    {
        g_MmGlobalInfo.AuthenCtrlInfo.ucLastFailCause = MM_MAC_CODE_FAIL;       /* ������һ�εĴ�����Ϣ                     */
        Mm_TimerStart(MM_TIMER_T3214);
    }
}

VOS_VOID Mm_ComAuthenRcvAuthenReq()
{
    VOS_UINT8                           i;
    MM_MSG_AUTHENTICATION_RSP_STRU      Msg;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRatType;

    enRatType   = NAS_MML_GetCurrNetRatType();

    /* ���� SIM ����Ȩ�� USIM ���� GSM �¼�Ȩ */
    if (VOS_FALSE == NAS_MML_GetSimPresentStatus()) /* �������� */
    {
        NAS_LOG(WUEPS_PID_MM, MM_AUTH, PS_PRINT_WARNING,
                    "Mm_ComAuthenRcvAuthenReq: WARNING: Sim is invalid!");

        /* ������Ϣ������, ��������ֱ�ӷ��� */
        return;
    }
    /* Ϊ SIM �������� USIM ���� GSM ���е� GSM ��Ȩ */
    else if ((NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
          || ((MM_IE_AUTN_ABSENT == g_MmMsgAuthReq.ucAutnFlg)
          && (NAS_MML_NET_RAT_TYPE_GSM == enRatType)))
    {

        MM_GsmAuthProc();

        return;
    }
    else
    {

    }
    /* USIM ���� UMTS ���м�Ȩ���� USIM ���� GSM ���е� UMTS ��Ȩ��������д��� */


    if (VOS_TRUE != NAS_MML_GetSimCsRegStatus())
    {                                                                           /* MS��֧��UMTS ALGO                        */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_ComAuthenRcvAuthenReq:WARNING: Not support UMTS ALGO");
        return;
    }
    PS_MEM_SET(&Msg,0,sizeof(MM_MSG_AUTHENTICATION_RSP_STRU));
    Mm_TimerStop(MM_TIMER_T3214);
    Mm_TimerStop(MM_TIMER_T3216);
    PS_MEM_CPY(
                g_MmGlobalInfo.AuthenCtrlInfo.aucCurRand,
                g_MmMsgAuthReq.MmIeRAND.aucRandValue,
                16 * sizeof(VOS_UINT8)
                );

    if ( MM_IE_AUTN_PRESENT == g_MmMsgAuthReq.ucAutnFlg )
    {                                                                           /* AUTN���ڵĳ���                           */
        g_MmGlobalInfo.AuthenCtrlInfo.ucAutnLen
            = g_MmMsgAuthReq.MmIeAUTN.ucAutnLen;
        for ( i = 0; i < g_MmMsgAuthReq.MmIeAUTN.ucAutnLen; i++ )
        {
            g_MmGlobalInfo.AuthenCtrlInfo.aucAutn[i]
                = g_MmMsgAuthReq.MmIeAUTN.aucAutn[i];
        }
        if ( MM_AUTHEN_RAND_ABSENT ==  g_MmGlobalInfo.AuthenCtrlInfo.ucRandFlg )
        {
            PS_MEM_CPY(
                        g_MmGlobalInfo.AuthenCtrlInfo.aucOldRand,
                        g_MmGlobalInfo.AuthenCtrlInfo.aucCurRand,
                        16 * sizeof(VOS_UINT8)
                        );
            g_MmGlobalInfo.AuthenCtrlInfo.ucRandFlg = MM_AUTHEN_RAND_PRESENT;   /* ����RAND���ڱ�ʶ                         */
            /*A32D07740==>*/
            g_MmGlobalInfo.AuthenCtrlInfo.ucRandRepeat   = MM_AUTHEN_RAND_REPEAT;
            g_MmGlobalInfo.AuthenCtrlInfo.ucLastAuthType = AUTHENTICATION_REQ_UMTS_CHALLENGE;
            /*<==A32D07740*/
            Mm_SndAgentUsimAuthenReq();                                         /* ����AGENT_USIM_AUTHENTICATION_REQ        */
            Mm_TimerStart(MM_TIMER_PROTECT_AGENT);                              /* ��������TIMER                            */
            /* Mm_TimerStart(MM_TIMER_PROTECT); */
        }
        else
        {
            if ( ( MM_FALSE == Mm_ComRandJudge() ) )
            {                                                                   /* RAND��ͬ                                 */
                /* ǰ���� RAND �Ƿ���ͬ */
                if (MM_AUTHEN_RAND_DIFFER == g_MmGlobalInfo.AuthenCtrlInfo.ucRandRepeat)
                {
                    /* ���� RAND ���ϴ���ͬ */
                    g_MmGlobalInfo.AuthenCtrlInfo.ucRandRepeat = MM_AUTHEN_RAND_REPEAT;

                    Mm_SndAgentUsimAuthenReq();                                 /* ����AGENT_USIM_AUTHENTICATION_REQ        */

                    g_MmGlobalInfo.AuthenCtrlInfo.ucLastAuthType = AUTHENTICATION_REQ_UMTS_CHALLENGE;

                    Mm_TimerStart(MM_TIMER_PROTECT_AGENT);                      /* ��������TIMER                            */
                }
                /* ǰ���� RAND ��ͬ���� RES ���ڣ����μ�Ȩ�������ϴ���ͬ */
                else if ((MM_AUTHEN_RES_PRESENT == g_MmGlobalInfo.AuthenCtrlInfo.ucResFlg)
                    && (AUTHENTICATION_REQ_UMTS_CHALLENGE == g_MmGlobalInfo.AuthenCtrlInfo.ucLastAuthType))
                /* if ( MM_AUTHEN_RES_PRESENT ==
                                g_MmGlobalInfo.AuthenCtrlInfo.ucResFlg ) */
                {                                                               /* RES���ڵĳ���                            */

                    /* Modified by ��־�� 2004.10.25 begin */
                    /* Msg.MmIeAuthRspPara.aucSgnfcntRES[0] =  */
                    /*    (VOS_UINT8)(g_MmGlobalInfo.AuthenCtrlInfo.ulRes & 0xFF);  */
                    /* Msg.MmIeAuthRspPara.aucSgnfcntRES[1] = */
                    /* (VOS_UINT8)( ( g_MmGlobalInfo.AuthenCtrlInfo.ulRes >> 8 )    */
                    /*                                                & 0xFF);  */
                    /* Msg.MmIeAuthRspPara.aucSgnfcntRES[2] =                   */
                    /* (VOS_UINT8)( ( g_MmGlobalInfo.AuthenCtrlInfo.ulRes >> 16 )   */
                    /*                                                 & 0xFF); */
                    /* Msg.MmIeAuthRspPara.aucSgnfcntRES[3] =                   */
                    /* (VOS_UINT8)( ( g_MmGlobalInfo.AuthenCtrlInfo.ulRes >> 24 )   */
                    /*                                                 & 0xFF); */
                    PS_MEM_CPY(Msg.MmIeAuthRspPara.aucSgnfcntRES,
                                g_MmGlobalInfo.AuthenCtrlInfo.aucRes, 4);
                    /* Modified by ��־�� 2004.10.25 end */
                    if ( MM_CONST_NUM_0 !=
                            g_MmGlobalInfo.AuthenCtrlInfo.ucExtRspLength )
                    {
                        Msg.ucAutnRspParaExtFlg = MM_IE_AUTN_EXT_PRESENT;
                        Msg.MmIeAuthRspParaExt.ucLength =
                        (VOS_UINT8)(g_MmGlobalInfo.AuthenCtrlInfo.ucExtRspLength);
                        /* Modified by ��־�� 2004.10.25 begin */
                        /* for ( i = 0; i < 4; i ++ )                    */
                        /* {                                              */
                        /*     Msg.MmIeAuthRspParaExt.aucRES[i] =          */
                        /* (VOS_UINT8)((g_MmGlobalInfo.AuthenCtrlInfo.aulExtRes[0] >>  */
                        /*                                 ( i * 8 ) ) & 0xFF);  */
                        /*     Msg.MmIeAuthRspParaExt.aucRES[i + 4] =    */
                        /* (VOS_UINT8)((g_MmGlobalInfo.AuthenCtrlInfo.aulExtRes[1] >>  */
                        /*                                 ( i * 8 ) ) & 0xFF);  */
                        /*     Msg.MmIeAuthRspParaExt.aucRES[i + 8] =          */
                        /* (VOS_UINT8)((g_MmGlobalInfo.AuthenCtrlInfo.aulExtRes[2] >>  */
                        /*                                 ( i * 8 ) ) & 0xFF);   */
                        /* }                                                  */
                        PS_MEM_CPY(Msg.MmIeAuthRspParaExt.aucRES,
                                  g_MmGlobalInfo.AuthenCtrlInfo.aucExtRes, 12);
                        /* Modified by ��־�� 2004.10.25 end */
                    }

                    NAS_MML_SetSimCsSecurityCksn(g_MmMsgAuthReq.MmIeCKSN.ucCksn);
                    
                    Mm_ComMsgAuthRspSnd(&Msg);                                  /* ����AUTHENTICATION RESPONSE(RES)         */
                }
                else
                {                                                               /* RES�����ڵĳ���                          */
                    /* Res �Ƿ���ڵı�־�����ڼ�Ȩ�ɹ������� */
                    /*g_MmGlobalInfo.AuthenCtrlInfo.ucResFlg =
                                                        MM_AUTHEN_RES_PRESENT;*/  /* ����RES���ڵı�ʶ                        */

                    Mm_SndAgentUsimAuthenReq();                                 /* ����AGENT_USIM_AUTHENTICATION_REQ        */

                    g_MmGlobalInfo.AuthenCtrlInfo.ucLastAuthType = AUTHENTICATION_REQ_UMTS_CHALLENGE;

                    Mm_TimerStart(MM_TIMER_PROTECT_AGENT);                      /* ��������TIMER                            */
                    /* Mm_TimerStart(MM_TIMER_PROTECT); */
                }
            }
            else
            {                                                                   /* RAND����ͬ�ĳ���                         */
                PS_MEM_CPY(
                        g_MmGlobalInfo.AuthenCtrlInfo.aucOldRand,
                        g_MmGlobalInfo.AuthenCtrlInfo.aucCurRand,
                        16 * sizeof(VOS_UINT8)
                        );

                /* ���� RAND ���ϴβ�ͬ����Ҫ���RES���ڱ�־ */
                g_MmGlobalInfo.AuthenCtrlInfo.ucRandRepeat   = MM_AUTHEN_RAND_REPEAT;
                g_MmGlobalInfo.AuthenCtrlInfo.ucLastAuthType = AUTHENTICATION_REQ_UMTS_CHALLENGE;
                g_MmGlobalInfo.AuthenCtrlInfo.ucResFlg       = MM_AUTHEN_RES_ABSENT;
                g_MmGlobalInfo.AuthenCtrlInfo.ucSresFlg      = MM_AUTHEN_RES_ABSENT;

                Mm_SndAgentUsimAuthenReq();
                Mm_TimerStart(MM_TIMER_PROTECT_AGENT);
            }
        }
    }
    else
    {
        Mm_ComAuthenNoAutn();
    }
}



VOS_VOID Mm_ComAuthenRcvUsimCnfSuccess()
{
    /* Deleted by ��־�� 2004.10.25 begin */
    /* VOS_UINT8                               i;*/
    /* Deleted by ��־�� 2004.10.25 end */
    MM_MSG_AUTHENTICATION_RSP_STRU    Msg;

    VOS_UINT32                          ulRet;

    ulRet = PS_USIM_SERVICE_NOT_AVAILIABLE;

    if ( ( WAIT_FOR_OUTGOING_MM_CONNECTION == g_MmGlobalInfo.ucState ) ||
        ( WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION ==
                                            g_MmGlobalInfo.ucState ) )
    {                                                                           /* ΪT3230���е�״̬�ĳ���                  */
        Mm_TimerStart(MM_TIMER_T3230);                                          /* ����T3230                                */
    }
    if ( LOCATION_UPDATING_INITIATED == g_MmGlobalInfo.ucState )
    {
        Mm_TimerStart(MM_TIMER_T3210);                                          /* ����T3210                                */
    }
    if ( IMSI_DETACH_INITIATED == g_MmGlobalInfo.ucState )
    {
        Mm_TimerStart(MM_TIMER_T3220);                                          /* ����T3220                                */
    }
    g_MmGlobalInfo.AuthenCtrlInfo.ucAuthenAttmptCnt = 0;
    g_MmGlobalInfo.AuthenCtrlInfo.ucLastFailCause = MM_AUTHEN_NO_CAUSE;


    if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
    {

        /* ���ú��� Mm_SndAgentUsimUpdateFileReq ���� KC��CKSN */
        Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);

    }
    else /* USIM �� */
    {
        /* ���� USIM �� CK IK CKSN */
        Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);

        /* DCM MM-18-01-22 Ҫ�����CS:4F20�ļ���PS����4F52�ļ� */
        ulRet = NAS_USIMMAPI_IsServiceAvailable(NAS_USIM_SVR_GSM_ACCESS_IN_USIM);        

        if (PS_USIM_SERVICE_AVAILIABLE == ulRet)
        {
            Mm_SndAgentUsimUpdateFileReq(NAS_MML_READ_USIM_CS_KC_FILE_ID);
        }
    }

    /* �� AS ����RRMM_NAS_INFO_CHANGE_REQ */
    Mm_SndRrNasInfoChgReq(MM_NAS_INFO_SK_FLG);

    /* �������� RAND ��ͬ��Ҫ��ʱ������ Res��RAND ��Ч�� */
    if (MM_AUTHEN_RAND_REPEAT == g_MmGlobalInfo.AuthenCtrlInfo.ucRandRepeat)
    {
        Mm_TimerStop(MM_TIMER_T3218);                                           /* ֹͣT3218                                */
        Mm_TimerStart(MM_TIMER_T3218);                                          /* ����T3218                                */
    }


    /*if ( MM_AUTHEN_RES_ABSENT == g_MmGlobalInfo.AuthenCtrlInfo.ucResFlg )*/
    /*{*/
    /*    Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID); */                    /* ����SIM��CK IK CKSN                      */
    /*    Mm_SndRrNasInfoChgReq(MM_NAS_INFO_SK_FLG);   */                           /* ��RRC����RRMM_NAS_INFO_CHANGE_REQ        */
    /*}*/
    /*else*/
    /*{*/
    /*    Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);  */                   /* ����SIM��CK IK CKSN                      */
    /*    Mm_SndRrNasInfoChgReq(MM_NAS_INFO_SK_FLG);  */                            /* ��RRC����RRMM_NAS_INFO_CHANGE_REQ        */
    /*     */
    /*    Mm_TimerStop(MM_TIMER_T3218);  */                                         /* ֹͣT3218                                */
    /*    Mm_TimerStart(MM_TIMER_T3218);  */                                        /* ����T3218                                */
    /*}                                                                                                                       */

    /* Modified by ��־�� 2004.10.25 begin */
    /* Msg.MmIeAuthRspPara.aucSgnfcntRES[0] =   */
    /*     (VOS_UINT8)(g_MmGlobalInfo.AuthenCtrlInfo.ulRes & 0x000000FF);  */
    /* Msg.MmIeAuthRspPara.aucSgnfcntRES[1] =   */
    /*     (VOS_UINT8)(( g_MmGlobalInfo.AuthenCtrlInfo.ulRes >> 8 ) & 0x000000FF);  */
    /* Msg.MmIeAuthRspPara.aucSgnfcntRES[2] =   */
    /*     (VOS_UINT8)(( g_MmGlobalInfo.AuthenCtrlInfo.ulRes >> 16 ) & 0x000000FF); */
    /* Msg.MmIeAuthRspPara.aucSgnfcntRES[3] = */
    /*     (VOS_UINT8)(( g_MmGlobalInfo.AuthenCtrlInfo.ulRes >> 24 ) & 0x000000FF); */

    PS_MEM_SET(&Msg, 0x0, sizeof(MM_MSG_AUTHENTICATION_RSP_STRU));

    PS_MEM_CPY(Msg.MmIeAuthRspPara.aucSgnfcntRES,
                g_MmGlobalInfo.AuthenCtrlInfo.aucRes, 4);
    /* Modified by ��־�� 2004.10.25 end */
    if ( MM_CONST_NUM_0 != g_MmGlobalInfo.AuthenCtrlInfo.ucExtRspLength )
    {
        Msg.ucAutnRspParaExtFlg = MM_IE_AUTN_EXT_PRESENT;
        Msg.MmIeAuthRspParaExt.ucLength =
            (VOS_UINT8)(g_MmGlobalInfo.AuthenCtrlInfo.ucExtRspLength);
        /* Modified by ��־�� 2004.10.25 begin */
        /* for ( i = 0; i < 4; i ++ )   */
        /* {              */
        /*     Msg.MmIeAuthRspParaExt.aucRES[i] =   */
        /*     (VOS_UINT8)( (g_MmGlobalInfo.AuthenCtrlInfo.aulExtRes[0] >>   */
        /*                                             ( i * 8 ) ) & 0x000000FF); */
        /*     Msg.MmIeAuthRspParaExt.aucRES[i + 4] =    */
        /*    (VOS_UINT8)(( g_MmGlobalInfo.AuthenCtrlInfo.aulExtRes[1] >>   */
        /*                                            ( i * 8 ) ) & 0x000000FF);*/
        /*    Msg.MmIeAuthRspParaExt.aucRES[i + 8] =   */
        /*    (VOS_UINT8)(( g_MmGlobalInfo.AuthenCtrlInfo.aulExtRes[2] >>   */
        /*                                            ( i * 8 ) ) & 0x000000FF);*/
        /* }        */
        PS_MEM_CPY(Msg.MmIeAuthRspParaExt.aucRES,
                    g_MmGlobalInfo.AuthenCtrlInfo.aucExtRes, 12);
        /* Modified by ��־�� 2004.10.25 end */
    }
    Mm_ComMsgAuthRspSnd(&Msg);                                                  /* ����AUTHENTICATION RESPONSE(RES)         */

}

VOS_VOID Mm_ComAuthenRcvUsimCnfFail()
{
    MM_MSG_AUTHENTICATION_FAIL_STRU     Msg;
    VOS_INT8                            cVersion;
    VOS_UINT8                           ucAuthFailFlg;

    ucAuthFailFlg   = VOS_FALSE;
    cVersion        = NAS_Common_Get_Supported_3GPP_Version(MM_COM_SRVDOMAIN_CS);

    Mm_TimerStop(MM_TIMER_T3230);                                               /* ֹͣT3230                                */
    Mm_TimerStop(MM_TIMER_T3220);                                               /* ֹͣT3220                                */
    Mm_TimerStop(MM_TIMER_T3210);                                               /* ֹͣT3210                                */
    Mm_TimerStop(MM_TIMER_T3218);                                               /* ͣT3218                                  */
    Mm_ComDelRandRes();                                                         /* ɾ��RAND��RES                            */

    if ( AGENT_AUTH_FAIL_MAC_CODE == g_AgentUsimAuthCnf.ucFailureCause )
    {                                                                           /* ʧ�ܵ�����ΪMAC CODE FAIL�ĳ���          */
        Msg.MmIeRejCause.ucRejCause = NAS_MML_REG_FAIL_CAUSE_MAC_FAILURE;
        Mm_ComMsgAuthFailSnd(&Msg);                                             /* ����AUTHENTICATION FAILURE               */

        /* 3GPP.24008 The MS shall deem that the network has failed the authentication check and behave
        as described in subclause 4.3.2.6.1, if any of the following occurs:
        -the timer T3216 expires;
        -the MS detects any combination of the authentication failures: "MAC failure", "invalid SQN",
        and "GSM authentication unacceptable", during three consecutive authentication challenges.
        The authentication challenges shall be considered as consecutive only, if the authentication
        challenges causing the second and third authentication failure are received by the MS, while
        the timer T3214 or T3216 started after the previous authentication failure is running. */
        g_MmGlobalInfo.AuthenCtrlInfo.ucAuthenAttmptCnt ++;

        /* R6�汾�Ժ��������μ�Ȩʧ��UE������Ϊ��Ȩʧ�� */
        if ((PS_PTL_VER_R6  <= cVersion)
         && (MM_CONST_NUM_3 == g_MmGlobalInfo.AuthenCtrlInfo.ucAuthenAttmptCnt))
        {
            ucAuthFailFlg = VOS_TRUE;
        }

        /* �˴�ʵ��ԭ���ο�R5Э���������μ�Ȩʧ��UE������Ϊ��Ȩʧ�� */
        if ((PS_PTL_VER_R5  >= cVersion)
         && ((MM_CONST_NUM_3   == g_MmGlobalInfo.AuthenCtrlInfo.ucAuthenAttmptCnt)
          || (MM_MAC_CODE_FAIL == g_MmGlobalInfo.AuthenCtrlInfo.ucLastFailCause)))
        {
            ucAuthFailFlg = VOS_TRUE;
        }

        if (VOS_TRUE == ucAuthFailFlg)
        {
            Mm_ComAuthMsRej();
            /* g_MmGlobalInfo.AuthenCtrlInfo.ucAuthenAttmptCnt = 0;
            g_MmGlobalInfo.AuthenCtrlInfo.ucLastFailCause = MM_AUTHEN_NO_CAUSE;
            Mm_ComRelAllMmConn();                                             */ /* �ͷ����ڽ������Ѿ�������MM����           */
            /* Mm_SndRrAbortReq(RRC_ABORT_RRC_CONN);                          */ /* ����RRMM_ABORT_REQ(RRC����)              */

            /* g_MmGlobalInfo.ucState = WAIT_FOR_NETWORK_COMMAND;
            Mm_TimerStart(MM_TIMER_T3240); */
        }
        else
        {
            g_MmGlobalInfo.AuthenCtrlInfo.ucLastFailCause = MM_MAC_CODE_FAIL;
            Mm_TimerStart(MM_TIMER_T3214);
        }
    }
    /* else */
    else if (AGENT_AUTH_FAIL_SQN == g_AgentUsimAuthCnf.ucFailureCause)
    {
        Msg.MmIeRejCause.ucRejCause = NAS_MML_REG_FAIL_CAUSE_SYNCH_FAILURE;
        Msg.MmIeAuthFailPara.ucLength =
             g_MmGlobalInfo.AuthenCtrlInfo.ucFailParaLength;
        /* Modified by ��־�� 2004.10.23 begin */
        /* for ( i = 0; i < 4; i ++ )     */
        /*{                                   */
        /*    Msg.MmIeAuthFailPara.aucAuthFailPara[i] =  */
        /*    (VOS_UINT8)( (g_MmGlobalInfo.AuthenCtrlInfo.aulFailPara[0] >>  */
        /*                                            ( i * 8 ) ) & 0xFF); */
        /*    Msg.MmIeAuthFailPara.aucAuthFailPara[i + 4] =           */
        /*    (VOS_UINT8)( (g_MmGlobalInfo.AuthenCtrlInfo.aulFailPara[1] >>    */
        /*                                            ( i * 8 ) ) & 0xFF);  */
        /*    Msg.MmIeAuthFailPara.aucAuthFailPara[i + 8] =   */
        /*    (VOS_UINT8)( (g_MmGlobalInfo.AuthenCtrlInfo.aulFailPara[2] >>  */
        /*                                            ( i * 8 ) ) & 0xFF);   */
        /*}  */
        /* Msg.MmIeAuthFailPara.aucAuthFailPara[12] =   */
        /* (VOS_UINT8)(g_MmGlobalInfo.AuthenCtrlInfo.aulFailPara[3] & 0xFF);   */
        /* Msg.MmIeAuthFailPara.aucAuthFailPara[13] =    */
        /* (VOS_UINT8)( (g_MmGlobalInfo.AuthenCtrlInfo.aulFailPara[3] >> 8 ) & 0xFF);   */
        PS_MEM_CPY(Msg.MmIeAuthFailPara.aucAuthFailPara,
                    g_MmGlobalInfo.AuthenCtrlInfo.aucFailPara, 14);
        /* Modified by ��־�� 2004.10.23 end */
        Mm_ComMsgAuthFailSnd(&Msg);                                             /* ����AUTHENTICATION FAILURE               */
        g_MmGlobalInfo.AuthenCtrlInfo.ucAuthenAttmptCnt ++;

        /* R6�汾�Ժ��������μ�Ȩʧ��UE������Ϊ��Ȩʧ�� */
        if ((PS_PTL_VER_R6  <= cVersion)
         && (MM_CONST_NUM_3 == g_MmGlobalInfo.AuthenCtrlInfo.ucAuthenAttmptCnt))
        {
            ucAuthFailFlg = VOS_TRUE;
        }

        if ((PS_PTL_VER_R5          >= cVersion)
         && ((MM_CONST_NUM_3        == g_MmGlobalInfo.AuthenCtrlInfo.ucAuthenAttmptCnt)
          || (MM_SQN_FAIL           == g_MmGlobalInfo.AuthenCtrlInfo.ucLastFailCause)))
        {
            ucAuthFailFlg = VOS_TRUE;
        }

        if (VOS_TRUE == ucAuthFailFlg)
        {
            Mm_ComAuthMsRej();
            /* g_MmGlobalInfo.AuthenCtrlInfo.ucAuthenAttmptCnt = 0;
            g_MmGlobalInfo.AuthenCtrlInfo.ucLastFailCause = MM_AUTHEN_NO_CAUSE;
            Mm_ComRelAllMmConn();                                             */ /* �ͷ����ڽ������Ѿ�������MM����           */
            /* Mm_SndRrAbortReq(RRC_ABORT_RRC_CONN);                          */ /* ����RRMM_ABORT_REQ(RRC����)              */

            /* g_MmGlobalInfo.ucState = WAIT_FOR_NETWORK_COMMAND;
            Mm_TimerStart(MM_TIMER_T3240); */

        }
        else
        {
            g_MmGlobalInfo.AuthenCtrlInfo.ucLastFailCause = MM_SQN_FAIL;
            Mm_TimerStart(MM_TIMER_T3216);
        }
    }
    else
    {
        /* �쳣���� */
        Msg.MmIeRejCause.ucRejCause = NAS_MML_REG_FAIL_CAUSE_PROTOCOL_ERROR;
        Mm_ComMsgAuthFailSnd(&Msg);

        MM_WARN_LOG(
                   " \nMM:UMTS AUTH RESULT IS NOT EXPECTED!\r " );

    }
}

VOS_VOID Mm_ComAuthenRcvUsimCnf()
{
    Mm_TimerStop(MM_TIMER_PROTECT_AGENT);                                       /* ֹͣ����TIMER                            */
    /* Mm_TimerStop(MM_TIMER_PROTECT); */
    if ( AGENT_AUTH_RST_SUCCESS == g_AgentUsimAuthCnf.ucCheckRst )
    {                                                                           /* USIM�Ľ���Ƿ�ɹ�                       */
        Mm_ComAuthenRcvUsimCnfSuccess();
    }
    else
    {
        Mm_ComAuthenRcvUsimCnfFail();
    }
}

VOS_VOID Mm_ComRejectCause4()
{
    g_MmGlobalInfo.ucMmServiceState = MM_ATTEMPTING_TO_UPDATE;                  /* ���÷���״̬                             */
    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComRejectCause4:INFO: MmServiceState is MM_ATTEMPTING_TO_UPDATE");
    NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_NOT_UPDATED);

    g_MmSubLyrShare.GmmShare.ucGsAssociationFlg = VOS_FALSE;

    Mm_ComNasInfoSav(
                    MM_STATUS_DETACHED,
                    MM_CKSN_INVALID
                    );
    g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &= ~MM_MS_ID_TMSI_PRESENT;       /* �����־λ                               */

    NAS_MML_InitUeIdTmsiInvalid();

    Mm_ComDelLai();                                                             /* ɾ��LAI                                  */

    Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                    /* ����SIM������״̬                        */

    if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
    {
        Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
    }
    else
    {
        Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
    }

    g_MmGlobalInfo.ucStaAfterWaitForNwkCmd = MM_IDLE_ATTEMPTING_TO_UPDATE;      /* �����ͷ�RR����ʱ������״̬             */
    Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);
    Mm_TimerStart(MM_TIMER_T3240);
    Mm_ComSaveProcAndCauseVal(
                                MM_MM_CONN_PROC,
                                NAS_MML_REG_FAIL_CAUSE_IMSI_UNKNOWN_IN_VLR
                             );                                                 /* ��¼���̺�ԭ��ֵ                         */
    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComRejectCause4:NORMAL: STATUS is WAIT_FOR_NETWORK_COMMAND");
}



VOS_VOID Mm_ComRejectCause6()
{
    Mm_ComRelAllMmConn(NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);

    Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);
    g_MmGlobalInfo.ucMmServiceState = MM_NO_IMSI;                               /* ���÷���״̬                             */
    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComRejectCause6:INFO: MmServiceState is MM_NO_IMSI");
    NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_PLMN_NOT_ALLOWED);                  /* ���ø���״̬                             */

    g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &= ~MM_MS_ID_TMSI_PRESENT;       /* �����־λ                               */

    NAS_MML_InitUeIdTmsiInvalid();

    Mm_ComDelLai();                                                             /* ɾ��LAI                                  */
    g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                            ~MM_MS_ID_IMSI_PRESENT;
    NAS_MML_SetSimCsRegStatus(VOS_FALSE);
    Mm_ComNasInfoSav(MM_STATUS_DETACHED, MM_CKSN_INVALID);
    Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                    /* ����SIM������״̬                        */

    if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
    {
        Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
    }
    else
    {
        Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
    }

    g_MmGlobalInfo.ucStaAfterWaitForNwkCmd = MM_IDLE_NO_IMSI;                   /* �����ͷ�RR����ʱ������״̬             */
    Mm_TimerStart(MM_TIMER_T3240);
    Mm_ComSaveProcAndCauseVal(
                                MM_MM_CONN_PROC,
                                NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME
                             );                                                 /* ��¼���̺�ԭ��ֵ                         */
    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComRejectCause6:NORMAL: STATUS is WAIT_FOR_NETWORK_COMMAND");
}


VOS_VOID Mm_T3240Expire(
                        VOS_VOID
                    )
{
    Mm_TimerStop(MM_TIMER_PROTECT_AGENT);
    g_MmGlobalInfo.ucRcvAgentFlg &= ~MM_RCV_AUTH_CNF_FLG;
    Mm_TimerStop(MM_TIMER_T3240);
    g_MmGlobalInfo.ucPreState   = g_MmGlobalInfo.ucState;                       /* ��¼Ǩ��֮ǰ��״̬                       */

    Mm_SndRrRelReq(RRC_CELL_UNBARRED);
    /*Mm_SndRrAbortReq(RRC_ABORT_CS_SIGN_CONN); */                                  /* ��RRC����RRMM_ABORT_REQ(CS��)            */

    /* if ( MM_LU_PROC != g_MmGlobalInfo.ucProc ) */
    /* {                            */                                          /* �������LU����                           */
    /*     Mm_SndMmcCmSvcInd(MM_CS_SERV_NOT_EXIST);  */                         /* ֪ͨMMC��CS���ҵ���Ѿ�ֹͣ              */
    /* } */
    Mm_TimerStart(MM_TIMER_T3240);
    return;
}


VOS_VOID Mm_T3230Expire(
                        VOS_VOID
                    )
 {
    VOS_UINT8   i = 0;                                                              /* ѭ����������                             */

    Mm_TimerStop(MM_TIMER_T3230);

    g_MmGlobalInfo.ucRcvAgentFlg &= ~MM_RCV_AUTH_CNF_FLG;
    Mm_TimerStop(MM_TIMER_PROTECT_AGENT);

    if ( WAIT_FOR_REESTABLISH_WAIT_FOR_EST_CNF != g_MmGlobalInfo.ucState )
    {                                                                           /* �����ؽ�����                             */
        switch ( g_MmGlobalInfo.ucMMConnEstingPD )
        {                                                                       /* ȷ�����ĸ�PD������MM����                 */
        case MM_IE_PD_CALL_CONTROL:

            /* CCҪ������MM����                       */
            Mm_SndCcRelInd( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
                ucMMConnEstingTI, NAS_MMCM_REL_CAUSE_MM_TIMER_T3230_EXP);            /* ֪ͨCC,MM���ӽ���ʧ��                    */

            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI
                = NO_MM_CONN_ESTING;                                            /* ������ڽ���MM���ӵı�־                 */
            break;
        case MM_IE_PD_NON_CALL_RLT_SS_MSG:                                      /* SSҪ������MM����                       */
            Mm_SndSsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].ucMMConnEstingTI,
                           NAS_MMCM_REL_CAUSE_MM_TIMER_T3230_EXP);

            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].ucMMConnEstingTI
                = NO_MM_CONN_ESTING;                                            /* ������ڽ���MM���ӵı�־                 */
            break;
        case MM_IE_PD_SMS_MSG:                                                  /* SMSҪ������MM����                      */
            Mm_SndSmsRelInd( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].
                ucMMConnEstingTI, NAS_MMCM_REL_CAUSE_MM_TIMER_T3230_EXP);              /* ֪ͨSMS,MM���ӽ���ʧ��                   */

            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].ucMMConnEstingTI
                = NO_MM_CONN_ESTING;                                            /* ������ڽ���MM���ӵı�־                 */
            break;
        default:
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_T3230Expire:WARNING: g_MmGlobalInfo.ucMMConnEstingPD Abnormal");
            break;
        }
    }
    g_MmGlobalInfo.ucMMConnEstingPD = NO_MM_CONN_ESTING;                        /* ����û�����ڽ���������                   */
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI =
        NO_MM_CONN_ESTING;                                                      /* ������ڽ�����MM���ӵı�־               */
    if ( ( MM_CONST_NUM_0 ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnExtFlg[0] )
        && ( MM_CONST_NUM_0 ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnExtFlg[1] )
        && ( MM_CONST_NUM_0 ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].aucMMConnExtFlg[0] )
        && ( MM_CONST_NUM_0 ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].aucMMConnExtFlg[1] )
        && ( MM_CONST_NUM_0 ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].aucMMConnExtFlg[0] )
        && ( MM_CONST_NUM_0 ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].aucMMConnExtFlg[1] ) )
    {                                                                           /* û��MM���Ӵ���                           */
        g_MmGlobalInfo.ucStaAfterWaitForNwkCmd =
            g_MmGlobalInfo.ucStaOfRcvXXEstReq;                                  /* ��¼Ҫ�����״̬                         */
        Mm_ComSaveProcAndCauseVal(
                                MM_MM_CONN_PROC,
                                NAS_MML_REG_FAIL_CAUSE_NULL
                             );                                                 /* ��¼���̺�ԭ��ֵ                         */

        g_MmGlobalInfo.ucPreState   = g_MmGlobalInfo.ucState;                   /* ��¼Ǩ��֮ǰ��״̬                       */
        Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);
        /*WUEPS_TRACE( MM_LOG_LEVEL_1,                                          */
        /*    " \nMM:STATUS is  WAIT_FOR_NETWORK_COMMAND\r " );                 */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_T3230Expire:NORMAL: STATUS is WAIT_FOR_NETWORK_COMMAND");
        Mm_TimerStart( MM_TIMER_T3240 );                                        /* ����TIMER3240                            */
    }
    else if ( WAIT_FOR_REESTABLISH_WAIT_FOR_EST_CNF ==
        g_MmGlobalInfo.ucState )
    {                                                                           /* �ؽ�����                                 */
        for ( i = 0; i < MM_CONST_NUM_7; i++ )
        {                                                                       /* �ؽ���MM����                             */
            if ( MM_CONST_NUM_0 !=
            ( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnExtFlg[0]
                & ( 0x01 << i ) ) )
            {
                /* ��TI��MM����ʧЧ                         */
                Mm_SndCcRelInd( i, NAS_MMCM_REL_CAUSE_MM_TIMER_T3230_EXP);           /* ֪ͨCC,MM���ӽ���ʧ��                    */
            }
            if ( MM_CONST_NUM_0 !=
            ( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnExtFlg[1]
                & ( 0x01 << i ) ) )
            {
                /* ��TI��MM����ʧЧ                         */
                Mm_SndCcRelInd( ( i + 8 ),
                    NAS_MMCM_REL_CAUSE_MM_TIMER_T3230_EXP);                          /* ֪ͨCC,MM���ӽ���ʧ��                    */
            }
        }
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnExtFlg[0] = 0;    /* �����־λ                               */
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnExtFlg[1] = 0;    /* �����־λ                               */
        Mm_ComSaveProcAndCauseVal(
                                MM_MM_CONN_PROC,
                                NAS_MML_REG_FAIL_CAUSE_NULL
                             );                                                 /* ��¼���̺�ԭ��ֵ                         */
        g_MmGlobalInfo.ucPreState   = g_MmGlobalInfo.ucState;                   /* ��¼Ǩ��֮ǰ��״̬                       */
        Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);
        /*WUEPS_TRACE( MM_LOG_LEVEL_1,                                          */
        /* " \nMM:STATUS is  WAIT_FOR_NETWORK_COMMAND\r " );                    */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_T3230Expire:NORMAL: STATUS is WAIT_FOR_NETWORK_COMMAND");
        g_MmGlobalInfo.ucStaAfterWaitForNwkCmd =
            g_MmGlobalInfo.ucStaOfRcvXXEstReq;                                  /* WAIT_FOR_NETWORK_COMMAND֮������״̬   */
        Mm_TimerStart( MM_TIMER_T3240 );                                        /* ����TIMER3240                            */
    }
    else
    {                                                                           /* �����ؽ����Ҳ��ǽ�����һ��MM����         */
        g_MmGlobalInfo.ucPreState   = g_MmGlobalInfo.ucState;                   /* ��¼Ǩ��֮ǰ��״̬                       */
        Mm_ComSetMmState(MM_CONNECTION_ACTIVE);
        /*WUEPS_TRACE( MM_LOG_LEVEL_1,                                          */
        /* " \nMM:STATUS is  MM_CONNECTION_ACTIVE\r " );                        */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_T3230Expire:NORMAL: STATUS is MM_CONNECTION_ACTIVE");
        Mm_ComCheckDelayMmConn( MM_FALSE );                                     /* ����Ƿ��б�������Ҫ������MM����         */
    }

    NAS_MM_UpdateCsServiceConnStatusFlg();


    return;
}
VOS_VOID Mm_ComCheckDelayMmConn_MM_IDLE_NORMAL_SERVICE_CC_EST_Exist(VOS_UINT8   ucLUFlg)
{
    MM_MSG_CM_SVC_REQ_STRU      CmSvcReq;                               /* Ҫ���͵�CM SERVICE REQ��Ϣ               */
    VOS_UINT16                  usMsgSize = 0;                          /* ��Ϣ����,���ֽ�Ϊ��λ                    */
    VOS_UINT8                  *pucCmSvcReq = 0;                        /* Ҫ���͵�CM SERVICE REQ��Ϣָ��           */

    g_MmGlobalInfo.ucStaOfRcvXXEstReq = g_MmGlobalInfo.ucState;     /* ��¼�յ���������ʱ��״̬                 */
    g_MmCcEstReq.ulCallType           = g_MmGlobalInfo.
        ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulCallType;       /* ���ú�������                             */
    g_MmCcEstReq.ulTransactionId      = g_MmGlobalInfo.
        ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulTransactionId;  /* ���ú���TI                               */
    if ( MMCC_MO_NORMAL_CALL ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.
        ulCallType )
    {                                                               /* ��������                                 */
        Mm_ComFillInCMSncReq(MM_IE_CM_SVC_TYPE_MO_CALL_EST,
            MM_FALSE, &CmSvcReq);                                   /* ���CM SERVICE REQUEST�ṹ��             */
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
        ucEstingCallTypeFlg = MM_FALSE;                             /* ��������                                 */
    }
    else
    {                                                               /* ��������                                 */
        Mm_ComFillInCMSncReq(MM_IE_CM_SVC_TYPE_EMG_CALL_EST,
            MM_FALSE, &CmSvcReq);                                   /* ���CM SERVICE REQUEST�ṹ��             */
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
        ucEstingCallTypeFlg = MM_TRUE;                              /* ��������                                 */
    }
    pucCmSvcReq =
        Mm_ComMsgCmSvcReqOfRrEstReq( &CmSvcReq, &usMsgSize );       /* ���CM SERVICE REQUEST ��Ϣ              */
    if ( VOS_NULL_PTR == pucCmSvcReq )
    {                                                               /* ��Ϣ����ʧ��                             */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_ComCheckDelayMmConn:WARNING: MAKE MSG CM SERVICE REQUEST ERROR!");
        return;                                                     /* ����                                     */
    }
    else
    {                                                               /* ��Ϣ�����ɹ�                             */
        if ( MMCC_MO_NORMAL_CALL == g_MmCcEstReq.ulCallType )
        {                                                           /* ��������                                 */
            Mm_SndRrEstReq( RRC_EST_CAUSE_ORIGIN_CONVERSAT_CALL,
                MM_FALSE,
                usMsgSize, pucCmSvcReq );                           /* ������Ϣ                                 */
        }
        else
        {                                                           /* ��������                                 */
            Mm_SndRrEstReq( RRC_EST_CAUSE_EMERGENCY_CALL, MM_FALSE,
                usMsgSize, pucCmSvcReq );                           /* ������Ϣ                                 */
        }
        MM_MEM_FREE(VOS_MEMPOOL_INDEX_NAS, pucCmSvcReq );             /* �ͷ�CM SERVICE REQUEST ��Ϣ�ṹ��        */
        if ( MM_TRUE == ucLUFlg )
        {
            /* ��LU����øú���                         */
            Mm_SndMmcCmSvcInd(MM_CS_SERV_EXIST);                    /* ֪ͨMMC��CS���ҵ���Ѿ�����              */
            NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_ESTING);
        }
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            ucMMConnEstingTI =
            (VOS_UINT8)g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            RcvXXEstReq.ulTransactionId;                            /* ��¼���ڽ�����MM����                     */
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucRat = NAS_MML_GetCurrNetRatType();
        g_MmGlobalInfo.ucMMConnEstingPD = MM_IE_PD_CALL_CONTROL;    /* ��¼���ڽ�����MM���ӵ�PD                 */
        g_MmGlobalInfo.ucPreState   = g_MmGlobalInfo.ucState;       /* ��¼Ǩ��֮ǰ��״̬                       */
        Mm_ComSetMmState(WAIT_FOR_RR_CONNECTION_MM_CONNECTION);
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComCheckDelayMmConn:NORMAL: STATUS is WAIT_FOR_RR_CONNECTION_MM_CONNECTION");
        Mm_TimerStart(MM_TIMER_PROTECT_SIGNALLING);                 /* ��������TIMER                            */
    }

    return;
}
VOS_VOID Mm_ComCheckDelayMmConn_MM_IDLE_NORMAL_SERVICE_SS_EST_Exist(VOS_VOID)
{
    MM_MSG_CM_SVC_REQ_STRU      CmSvcReq;                               /* Ҫ���͵�CM SERVICE REQ��Ϣ               */
    VOS_UINT16                  usMsgSize = 0;                          /* ��Ϣ����,���ֽ�Ϊ��λ                    */
    VOS_UINT8                  *pucCmSvcReq = 0;                        /* Ҫ���͵�CM SERVICE REQ��Ϣָ��           */

    g_MmGlobalInfo.ucStaOfRcvXXEstReq = g_MmGlobalInfo.ucState;     /* ��¼�յ���������ʱ��״̬                 */
    if ( ( MM_CONST_NUM_8 >
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.
        ulTransactionId )
        || ( MM_CONST_NUM_14 <
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.
        ulTransactionId ) )
    {                                                               /* TI���Ϸ�                                 */
        Mm_SndSsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ulTransactionId,
                       NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);
    }
    else
    {
        Mm_ComFillInCMSncReq(MM_IE_CM_SVC_TYPE_SS_ACT, MM_FALSE,
            &CmSvcReq);                                             /* ���CM SERVICE REQUEST�ṹ��             */
        pucCmSvcReq = Mm_ComMsgCmSvcReqOfRrEstReq( &CmSvcReq,
            &usMsgSize );                                           /* ���CM SERVICE REQUEST ��Ϣ              */
        if ( VOS_NULL_PTR == pucCmSvcReq )
        {                                                           /* ��Ϣ����ʧ��                             */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_ComCheckDelayMmConn:WARNING: MAKE MSG CM SERVICE REQUEST ERROR!");
            return;                                                 /* ����                                     */
        }
        else
        {                                                           /* ��Ϣ�����ɹ�                             */
            Mm_SndRrEstReq(
                RRC_EST_CAUSE_ORIGIN_HIGH_PRIORITY_SIGNAL,
                MM_FALSE, usMsgSize, pucCmSvcReq );                 /* ������Ϣ                                 */
            MM_MEM_FREE(VOS_MEMPOOL_INDEX_NAS, pucCmSvcReq );         /* �ͷ�CM SERVICE REQUEST ��Ϣ�ṹ��        */
        }
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].
            ucMMConnEstingTI =
            (VOS_UINT8)g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].
            RcvXXEstReq.ulTransactionId;                            /* ��¼���ڽ�����MM���ӵ�TI                 */
        g_MmGlobalInfo.ucMMConnEstingPD =
            MM_IE_PD_NON_CALL_RLT_SS_MSG;                           /* ��¼���ڽ�����MM���ӵ�PD                 */
        Mm_TimerStart(MM_TIMER_PROTECT_SIGNALLING);                 /* ��������TIMER                            */

        Mm_SndMmcCmSvcInd(MM_CS_SERV_EXIST);                        /* ֪ͨMMC��CS���ҵ���Ѿ�����              */
        NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_ESTING);
        g_MmGlobalInfo.ucPreState = g_MmGlobalInfo.ucState;         /* ��¼Ǩ��֮ǰ��״̬                       */
        Mm_ComSetMmState(WAIT_FOR_RR_CONNECTION_MM_CONNECTION);
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComCheckDelayMmConn:NORMAL: STATUS is WAIT_FOR_RR_CONNECTION_MM_CONNECTION");
    }

    return;
}
VOS_VOID Mm_ComCheckDelayMmConn_MM_IDLE_NORMAL_SERVICE_SMS_EST_Exist(VOS_VOID)
{
    MM_MSG_CM_SVC_REQ_STRU      CmSvcReq;                               /* Ҫ���͵�CM SERVICE REQ��Ϣ               */
    VOS_UINT16                  usMsgSize = 0;                          /* ��Ϣ����,���ֽ�Ϊ��λ                    */
    VOS_UINT8                  *pucCmSvcReq = 0;                        /* Ҫ���͵�CM SERVICE REQ��Ϣָ��           */

    g_MmGlobalInfo.ucStaOfRcvXXEstReq = g_MmGlobalInfo.ucState;
    if ( ( MM_CONST_NUM_8 >
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.
        ulTransactionId )
        || ( MM_CONST_NUM_14 <
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.
        ulTransactionId ) )
    {                                                               /* TI���Ϸ�                                 */
        Mm_SndSmsRelInd( g_MmGlobalInfo.
            ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.
            ulTransactionId,
            NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);                        /* ֪ͨSMS����ʧ��                          */
    }
    else
    {                                                               /* TI�Ϸ�                                   */
        Mm_ComFillInCMSncReq(MM_IE_CM_SVC_TYPE_SMS, MM_FALSE,
            &CmSvcReq);                                             /* ���CM SERVICE REQUEST�ṹ��             */
        pucCmSvcReq = Mm_ComMsgCmSvcReqOfRrEstReq( &CmSvcReq,
            &usMsgSize );                                           /* ���CM SERVICE REQUEST ��Ϣ              */
        if ( VOS_NULL_PTR == pucCmSvcReq )
        {                                                           /* ��Ϣ����ʧ��                             */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, ":WARNING: MAKE MSG CM SERVICE REQUEST ERROR!");
            return;                                                 /* ����                                     */
        }
        else
        {                                                           /* ��Ϣ�����ɹ�                             */
            Mm_SndRrEstReq(
               RRC_EST_CAUSE_ORIGIN_LOW_PRIORITY_SIGNAL,
               MM_FALSE,
               usMsgSize, pucCmSvcReq );                            /* ������Ϣ                                 */
            MM_MEM_FREE(VOS_MEMPOOL_INDEX_NAS, pucCmSvcReq );          /* �ͷ�CM SERVICE REQUEST ��Ϣ�ṹ��        */
        }
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].
            ucMMConnEstingTI =
            (VOS_UINT8)g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].
            RcvXXEstReq.ulTransactionId;                            /* ��¼���ڽ�����MM���ӵ�TI                 */
        g_MmGlobalInfo.ucMMConnEstingPD = MM_IE_PD_SMS_MSG;         /* ��¼���ڽ�����MM���ӵ�PD                 */
        Mm_TimerStart(MM_TIMER_PROTECT_SIGNALLING);                 /* ��������TIMER                            */

        Mm_SndMmcCmSvcInd(MM_CS_SERV_EXIST);                        /* ֪ͨMMC��CS���ҵ���Ѿ�����              */
        NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_ESTING);
        g_MmGlobalInfo.ucPreState = g_MmGlobalInfo.ucState;         /* ��¼Ǩ��֮ǰ��״̬                       */
        Mm_ComSetMmState(WAIT_FOR_RR_CONNECTION_MM_CONNECTION);
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComCheckDelayMmConn:NORMAL: STATUS is WAIT_FOR_RR_CONNECTION_MM_CONNECTION");
    }

    return;
}


VOS_VOID Mm_ComCheckDelayMmConn_MM_IDLE_NORMAL_SERVICE(VOS_UINT8   ucLUFlg)
{
    RRMM_PAGING_IND_STRU                stWPagingMsg;
    GRRMM_PAGING_IND_ST                 stGPagingMsg;

    if ( MM_TRUE ==
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg )
    {                                                                   /* �еȴ���CC��������Ҫ������MM����         */
        Mm_ComCheckDelayMmConn_MM_IDLE_NORMAL_SERVICE_CC_EST_Exist(ucLUFlg);
    }
    else if ( MM_TRUE ==
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg )
    {
        Mm_ComCheckDelayMmConn_MM_IDLE_NORMAL_SERVICE_SS_EST_Exist();
    }
    else if ( MM_TRUE ==
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg )
    {
        Mm_ComCheckDelayMmConn_MM_IDLE_NORMAL_SERVICE_SMS_EST_Exist();
    }
    else if (MM_PAGING_PLMNSRCH_TRUE == g_MmSubLyrShare.MmShare.ucPagingPlmnSrch)
    {
        g_MmSubLyrShare.MmShare.ucPagingPlmnSrch = MM_PAGING_PLMNSRCH_FALSE;
        if (NAS_MML_NET_RAT_TYPE_GSM == NAS_MML_GetCurrNetRatType())
        {
            Mm_MakeGPagingInd(&stGPagingMsg);
            gaMmStateTable[30][g_MmGlobalInfo.ucState](&stGPagingMsg);
        }
        else
        {
            Mm_MakeWPagingInd(&stWPagingMsg);
            gaMmStateTable[30][g_MmGlobalInfo.ucState](&stWPagingMsg);
        }
    }
    else
    {                                                                   /* û�еȴ���CCҪ������MM����             */
        if ( MM_FALSE == ucLUFlg )
        {
            Mm_SndMmcCmSvcInd(MM_CS_SERV_NOT_EXIST);                    /* ֪ͨMMC��CS���ҵ���Ѿ�ֹͣ              */
            NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_NOT_EXIST);
        }

        if ((VOS_TRUE == NAS_MM_IsDisableGprsCombineAttach())
         && (MM_TIMER_STOP == gstMmTimer[MM_TIMER_T3211].ucTimerStatus))
        {
            Mm_TimerStart(MM_TIMER_T3212);                              /* ����TIMER3230                            */
        }
    }

    NAS_MM_UpdateCsServiceConnStatusFlg();
    NAS_MM_UpdateCsServiceBufferStatusFlg();

    return;
}


VOS_VOID Mm_ComCheckDelayMmConn_MM_IDLE_ATTEMPTING_TO_UPDATE(VOS_UINT8   ucLUFlg)
{
    MM_MSG_CM_SVC_REQ_STRU   CmSvcReq;                                              /* Ҫ���͵�CM SERVICE REQ��Ϣ               */
    VOS_UINT16               usMsgSize = 0;                                         /* ��Ϣ����,���ֽ�Ϊ��λ                    */
    VOS_UINT8               *pucCmSvcReq = 0;                                       /* Ҫ���͵�CM SERVICE REQ��Ϣָ��           */

    RRMM_PAGING_IND_STRU     stWPagingMsg;
    GRRMM_PAGING_IND_ST      stGPagingMsg;

    if ( MM_TRUE ==
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg )
    {                                                                   /* �ɵȴ���CC��������Ҫ������MM����         */
        if ( MMCC_EMERGENCY_CALL ==
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.
            ulCallType )
        {                                                               /* �ǽ�������                               */
            g_MmCcEstReq.ulCallType           = g_MmGlobalInfo.
                ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulCallType;   /* ���ú�������                             */
            g_MmCcEstReq.ulTransactionId      = g_MmGlobalInfo.
                ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.
                ulTransactionId;                                        /* ���ú���TI                               */
            g_MmGlobalInfo.ucStaOfRcvXXEstReq = g_MmGlobalInfo.ucState; /* ��¼�յ���������ʱ��״̬                 */
            Mm_ComFillInCMSncReq(MM_IE_CM_SVC_TYPE_EMG_CALL_EST,
                MM_FALSE, &CmSvcReq);                                   /* ���CM SERVICE REQUEST�ṹ��             */
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            ucEstingCallTypeFlg = MM_TRUE;                              /* ��������                                 */
            pucCmSvcReq =
                Mm_ComMsgCmSvcReqOfRrEstReq( &CmSvcReq, &usMsgSize );   /* ���CM SERVICE REQUEST ��Ϣ              */
            if ( VOS_NULL_PTR == pucCmSvcReq )
            {                                                           /* ��Ϣ����ʧ��                             */
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_ComCheckDelayMmConn:WARNING: MAKE MSG CM SERVICE REQUEST ERROR!");
                return;                                                 /* ����                                     */
            }
            else
            {                                                           /* ��Ϣ�����ɹ�                             */
                if ( MM_TRUE == ucLUFlg )
                {                                                       /* ��LU����øú���                         */
                    Mm_SndMmcCmSvcInd(MM_CS_SERV_EXIST);                /* ֪ͨMMC��CS���ҵ���Ѿ�����              */
                    NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_ESTING);
                }
                Mm_SndRrEstReq( RRC_EST_CAUSE_EMERGENCY_CALL, MM_FALSE,
                    usMsgSize, pucCmSvcReq );                           /* ������Ϣ                                 */
                Mm_TimerStart(MM_TIMER_PROTECT_SIGNALLING);             /* ��������TIMER                            */
                g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
                    ucMMConnEstingTI =
                    (VOS_UINT8)g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
                    RcvXXEstReq.ulTransactionId;                        /* ��¼���ڽ�����MM����                     */
                g_MmGlobalInfo.ucMMConnEstingPD = MM_IE_PD_CALL_CONTROL;/* ��¼���ڽ�����MM���ӵ�PD                 */
                g_MmGlobalInfo.ucPreState   = g_MmGlobalInfo.ucState;   /* ��¼Ǩ��֮ǰ��״̬                       */
                Mm_ComSetMmState(WAIT_FOR_RR_CONNECTION_MM_CONNECTION);
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComCheckDelayMmConn:NORMAL: STATUS is WAIT_FOR_RR_CONNECTION_MM_CONNECTION");
            }
        }
        else
        {
            /* ����������                               */
            Mm_SndCcRelInd( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulTransactionId,
                NAS_MMCM_REL_CAUSE_MM_LIMIT_SERVICE);                      /* ֪ͨCCָ��������MM���ӽ���ʧ��           */

            if ( MM_FALSE == ucLUFlg )
            {
                Mm_SndMmcCmSvcInd(MM_CS_SERV_NOT_EXIST);                /* ֪ͨMMC��CS���ҵ���Ѿ�ֹͣ              */
                NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_NOT_EXIST);
            }
        }
    }
    else if ( MM_TRUE ==
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg )
    {
        Mm_SndSsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ulTransactionId,
                       NAS_MMCM_REL_CAUSE_MM_LIMIT_SERVICE);
    }
    else if ( MM_TRUE ==
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg )
    {
        Mm_SndSmsRelInd( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].
                RcvXXEstReq.ulTransactionId,
                NAS_MMCM_REL_CAUSE_MM_LIMIT_SERVICE);                        /* ֪ͨSMS����ʧ��                          */
    }
    else if (MM_PAGING_PLMNSRCH_TRUE == g_MmSubLyrShare.MmShare.ucPagingPlmnSrch)
    {
        g_MmSubLyrShare.MmShare.ucPagingPlmnSrch = MM_PAGING_PLMNSRCH_FALSE;
        if (NAS_MML_NET_RAT_TYPE_GSM == NAS_MML_GetCurrNetRatType())
        {
            Mm_MakeGPagingInd(&stGPagingMsg);
            gaMmStateTable[30][g_MmGlobalInfo.ucState](&stGPagingMsg);
        }
        else
        {
            Mm_MakeWPagingInd(&stWPagingMsg);
            gaMmStateTable[30][g_MmGlobalInfo.ucState](&stWPagingMsg);
        }
    }
    else
    {                                                                   /* û�еȴ���CCҪ������MM����             */
        if ( MM_FALSE == ucLUFlg )
        {
            Mm_SndMmcCmSvcInd(MM_CS_SERV_NOT_EXIST);                    /* ֪ͨMMC��CS���ҵ���Ѿ�ֹͣ              */
            NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_NOT_EXIST);
        }

        if ((VOS_TRUE == NAS_MM_IsDisableGprsCombineAttach())
         && (MM_TIMER_STOP == gstMmTimer[MM_TIMER_T3211].ucTimerStatus)
         && (MM_TIMER_STOP == gstMmTimer[MM_TIMER_T3213].ucTimerStatus))
        {
            Mm_TimerStart(MM_TIMER_T3212);                              /* ����TIMER3230 */
        }
    }

    return;
}


VOS_VOID Mm_ComCheckDelayMmConn_MM_CONNECTION_ACTIVE_CC_EST_Exist(VOS_VOID)
{
    MM_MSG_CM_SVC_REQ_STRU      CmSvcReq;                                       /* Ҫ���͵�CM SERVICE REQ��Ϣ               */

    g_MmCcEstReq.ulCallType           = g_MmGlobalInfo.
        ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulCallType;       /* ���ú�������                             */
    g_MmCcEstReq.ulTransactionId      = g_MmGlobalInfo.
        ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulTransactionId;  /* ���ú���TI                               */
    if ( ( MM_TRUE == Mm_ComJudgeLimitSvc( ) )
        ||( MM_CONST_NUM_0 !=
        ( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
        aucMMConnExtFlg[1] & ( 0x01 <<
        ( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.
        ulTransactionId - 8 ) ) ) )
        )
    {
        /* Ҫ������MM�����Ѿ����ڻ����޷���״̬   */
        Mm_SndCcRelInd( g_MmGlobalInfo.
            ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.
            ulTransactionId,
            NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);                      /* ֪ͨCCָ��������MM���ӽ���ʧ��           */
    }
    else
    {                                                               /* Ҫ������MM���Ӳ�����                   */
        /* ==>A32D12640 */
        /*
        g_MmGlobalInfo.ucStaOfRcvXXEstReq = g_MmGlobalInfo.ucState;
        */
                                                                    /* ��¼�յ���������ʱ��״̬                 */
        if ( MMCC_MO_NORMAL_CALL ==
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            RcvXXEstReq.ulCallType )
        {                                                           /* ������������                             */
            Mm_ComFillInCMSncReq(MM_IE_CM_SVC_TYPE_MO_CALL_EST,
                MM_FALSE, &CmSvcReq);                               /* ���CM SERVICE REQUEST�ṹ��             */
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            ucEstingCallTypeFlg = MM_FALSE;                         /* ��������                                 */
        }
        else if ( MMCC_EMERGENCY_CALL ==
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            RcvXXEstReq.ulCallType )
        {                                                           /* ���н�������                             */
            Mm_ComFillInCMSncReq(MM_IE_CM_SVC_TYPE_EMG_CALL_EST,
                MM_FALSE, &CmSvcReq);                               /* ���CM SERVICE REQUEST�ṹ��             */
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
                ucEstingCallTypeFlg = MM_TRUE;                      /* ��������                                 */
        }
        else
        {
        }
        Mm_ComMsgCmSvcReqSnd(&CmSvcReq);                            /* �����෢�ͽ���MM���ӵ�����               */
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            ucMMConnEstingTI = (VOS_UINT8)g_MmGlobalInfo.
            ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq
            .ulTransactionId;                                       /* ��¼���ڽ�����MM����                     */
        g_MmGlobalInfo.ucMMConnEstingPD = MM_IE_PD_CALL_CONTROL;    /* ��¼���ڽ�����MM���ӵ�PD                 */
        Mm_TimerStart(MM_TIMER_T3230);                              /* ����TIMER3230                            */
        g_MmGlobalInfo.ucPreState = g_MmGlobalInfo.ucState;         /* ��¼Ǩ��֮ǰ��״̬                       */
        Mm_ComSetMmState(WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION);
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComCheckDelayMmConn:NORMAL: STATUS is WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION");
    }

    return;
}


VOS_VOID Mm_ComCheckDelayMmConn_MM_CONNECTION_ACTIVE_SS_EST_Exist(VOS_VOID)
{
    MM_MSG_CM_SVC_REQ_STRU      CmSvcReq;                                       /* Ҫ���͵�CM SERVICE REQ��Ϣ               */

    if ( ( MM_TRUE == Mm_ComJudgeLimitSvc( ) )
        ||(8 > g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS]
                    .RcvXXEstReq.ulTransactionId)
        ||(14 < g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS]
                    .RcvXXEstReq.ulTransactionId)
        )
    {
        /* TI���Ϸ������޷���״̬   */

        /* ֪ͨSSָ��������MM���ӽ���ʧ��           */
        Mm_SndSsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ulTransactionId,
                       NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);
    }
    else if (MM_CONST_NUM_0 !=
            ( (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].aucMMConnExtFlg[1])
              &( 0x01 << ( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS]
                    .RcvXXEstReq.ulTransactionId - 8 ) ) ))
    {
        /* Ҫ������MM�����Ѿ�����                 */

        /* ��¼�յ���������ʱ��״̬                 */
        /* ==>A32D12640 */
        /*
        g_MmGlobalInfo.ucStaOfRcvXXEstReq = g_MmGlobalInfo.ucState;
        */
        /* ��MM���ӽ����ɹ�                         */
        Mm_SndSsEstCnf(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS]
                    .RcvXXEstReq.ulTransactionId,MMSS_EST_SUCCESS);
    }
    else
    {
        /* ���CM SERVICE REQUEST�ṹ��             */
        Mm_ComFillInCMSncReq(MM_IE_CM_SVC_TYPE_SS_ACT, MM_FALSE, &CmSvcReq);
        /* �����෢�ͽ���MM���ӵ�����               */
        Mm_ComMsgCmSvcReqSnd( &CmSvcReq );
        /* ��¼���ڽ�����MM���ӵ�TI                 */
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].ucMMConnEstingTI =
            (VOS_UINT8)(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS]
                    .RcvXXEstReq.ulTransactionId);
        /* ��¼���ڽ�����MM���ӵ�PD                 */
        g_MmGlobalInfo.ucMMConnEstingPD = MM_IE_PD_NON_CALL_RLT_SS_MSG;
        /* ����TIMER3230                            */
        Mm_TimerStart(MM_TIMER_T3230);
        /* ��¼Ǩ��֮ǰ��״̬                       */
        g_MmGlobalInfo.ucPreState = g_MmGlobalInfo.ucState;
        /* ״̬Ǩ��                                 */
        Mm_ComSetMmState(WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION);
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComCheckDelayMmConn:NORMAL: STATUS is WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION");
    }

    return;
}


VOS_VOID Mm_ComCheckDelayMmConn_MM_CONNECTION_ACTIVE_SMS_EST_Exist(VOS_VOID)
{
    MM_MSG_CM_SVC_REQ_STRU      CmSvcReq;                                       /* Ҫ���͵�CM SERVICE REQ��Ϣ               */

    if ( ( MM_TRUE == Mm_ComJudgeLimitSvc( ) )
        ||(8 > g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS]
                    .RcvXXEstReq.ulTransactionId)
        ||(14 < g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS]
                    .RcvXXEstReq.ulTransactionId)
        )
    {
        /* TI���Ϸ������޷���״̬   */

        /* ֪ͨSMSָ��������MM���ӽ���ʧ��           */
        Mm_SndSmsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS]
                    .RcvXXEstReq.ulTransactionId,
                    NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);
    }
    else if (MM_CONST_NUM_0 !=
            ( (g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].aucMMConnExtFlg[1])
              &( 0x01 << ( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS]
                    .RcvXXEstReq.ulTransactionId - 8 ) ) ))
    {
        /* Ҫ������MM�����Ѿ�����                 */

        /* ��¼�յ���������ʱ��״̬                 */
        /* ==>A32D12640 */
        /*
        g_MmGlobalInfo.ucStaOfRcvXXEstReq = g_MmGlobalInfo.ucState;
        */
        /* ��MM���ӽ����ɹ�                         */
        Mm_SndSmsEstCnf(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS]
                    .RcvXXEstReq.ulTransactionId);
    }
    else
    {
        /* ���CM SERVICE REQUEST�ṹ��             */
        Mm_ComFillInCMSncReq(MM_IE_CM_SVC_TYPE_SMS, MM_FALSE, &CmSvcReq);
        /* �����෢�ͽ���MM���ӵ�����               */
        Mm_ComMsgCmSvcReqSnd( &CmSvcReq );
        /* ��¼���ڽ�����MM���ӵ�TI                 */
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].ucMMConnEstingTI =
            (VOS_UINT8)(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS]
                    .RcvXXEstReq.ulTransactionId);
        /* ��¼���ڽ�����MM���ӵ�PD                 */
        g_MmGlobalInfo.ucMMConnEstingPD = MM_IE_PD_SMS_MSG;
        /* ����TIMER3230                            */
        Mm_TimerStart(MM_TIMER_T3230);
        /* ��¼Ǩ��֮ǰ��״̬                       */
        g_MmGlobalInfo.ucPreState = g_MmGlobalInfo.ucState;
        /* ״̬Ǩ��                                 */
        Mm_ComSetMmState(WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION);
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComCheckDelayMmConn:NORMAL: STATUS is WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION");
    }

    return;
}

/***********************************************************************
 *  MODULE   : Mm_ComCheckDelayMmConn_MM_CONNECTION_ACTIVE
 *  FUNCTION : Mm_ComCheckDelayMmConn���������Ӷ�: MM_CONNECTION_ACTIVE ״̬����
 *  INPUT    : VOS_VOID
 *  OUTPUT   : VOS_VOID
 *  RETURN   : VOS_VOID
 *  NOTE     :
 *  HISTORY  :
     1.  ŷ����   2009.06.11  �°�����
 ************************************************************************/
VOS_VOID Mm_ComCheckDelayMmConn_MM_CONNECTION_ACTIVE(VOS_VOID)
{
    if ( MM_TRUE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg )
    {                                                                   /* �еȴ���CC��������Ҫ������MM����         */
        Mm_ComCheckDelayMmConn_MM_CONNECTION_ACTIVE_CC_EST_Exist();
    }
    else if ( MM_TRUE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg )
    {
        /* �еȴ���SS��������Ҫ������MM����         */
        Mm_ComCheckDelayMmConn_MM_CONNECTION_ACTIVE_SS_EST_Exist();
    }
    else if ( MM_TRUE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg )
    {
        /* �еȴ���SMS��������Ҫ������MM����         */
        Mm_ComCheckDelayMmConn_MM_CONNECTION_ACTIVE_SMS_EST_Exist();
    }
    else
    {
    }

    return;
}
VOS_VOID Mm_ComCheckDelayMmConn_Other_State_Handling(VOS_UINT8   ucLUFlg)
{
    MM_MSG_CM_SVC_REQ_STRU      CmSvcReq;                                       /* Ҫ���͵�CM SERVICE REQ��Ϣ               */
    VOS_UINT16                  usMsgSize = 0;                                  /* ��Ϣ����,���ֽ�Ϊ��λ                    */
    VOS_UINT8                  *pucCmSvcReq = 0;                                /* Ҫ���͵�CM SERVICE REQ��Ϣָ��           */

    RRMM_PAGING_IND_STRU        stWPagingMsg;
    GRRMM_PAGING_IND_ST         stGPagingMsg;

    if ( MM_TRUE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg )
    {                                                                   /* �ɵȴ���CC��������Ҫ������MM����         */
        if ( MMCC_EMERGENCY_CALL ==
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            RcvXXEstReq.ulCallType )
        {                                                               /* �ǽ�������                               */
            g_MmCcEstReq.ulCallType           = g_MmGlobalInfo.
                ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulCallType;   /* ���ú�������                             */
            g_MmCcEstReq.ulTransactionId      = g_MmGlobalInfo.
                ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.
                ulTransactionId;                                        /* ���ú���TI                               */
            g_MmGlobalInfo.ucStaOfRcvXXEstReq = g_MmGlobalInfo.ucState; /* ��¼�յ���������ʱ��״̬                 */
            Mm_ComFillInCMSncReq(MM_IE_CM_SVC_TYPE_EMG_CALL_EST,
                MM_FALSE, &CmSvcReq);                                   /* ���CM SERVICE REQUEST�ṹ��             */
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
                ucEstingCallTypeFlg = MM_TRUE;                          /* ��������                                 */
            pucCmSvcReq =
                Mm_ComMsgCmSvcReqOfRrEstReq( &CmSvcReq, &usMsgSize );   /* ���CM SERVICE REQUEST ��Ϣ              */
            if ( VOS_NULL_PTR == pucCmSvcReq )
            {                                                           /* ��Ϣ����ʧ��                             */
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_ComCheckDelayMmConn:WARNING: MAKE MSG CM SERVICE REQUEST ERROR!");
                return;                                                 /* ����                                     */
            }
            else
            {                                                           /* ��Ϣ�����ɹ�                             */
                if ( MM_TRUE == ucLUFlg )
                {                                                       /* ��LU����øú���                         */
                    Mm_SndMmcCmSvcInd(MM_CS_SERV_EXIST);                /* ֪ͨMMC��CS���ҵ���Ѿ�����              */
                    NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_ESTING);
                }
                Mm_SndRrEstReq( RRC_EST_CAUSE_EMERGENCY_CALL, MM_FALSE,
                    usMsgSize, pucCmSvcReq );                           /* ������Ϣ                                 */
                Mm_TimerStart(MM_TIMER_PROTECT_SIGNALLING);             /* ��������TIMER                            */
                g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
                    ucMMConnEstingTI =
                    (VOS_UINT8)g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
                    RcvXXEstReq.ulTransactionId;                        /* ��¼���ڽ�����MM����                     */
                g_MmGlobalInfo.ucMMConnEstingPD = MM_IE_PD_CALL_CONTROL;/* ��¼���ڽ�����MM���ӵ�PD                 */
                g_MmGlobalInfo.ucPreState   = g_MmGlobalInfo.ucState;   /* ��¼Ǩ��֮ǰ��״̬                       */
                Mm_ComSetMmState(WAIT_FOR_RR_CONNECTION_MM_CONNECTION);
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComCheckDelayMmConn:NORMAL: STATUS is WAIT_FOR_RR_CONNECTION_MM_CONNECTION");
            }
        }
        else
        {
            if ( MM_IDLE_NO_IMSI == g_MmGlobalInfo.ucState )
            {
                /* ����������                               */
                Mm_SndCcRelInd( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulTransactionId,
                    NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);                      /* ֪ͨCCָ��������MM���ӽ���ʧ��           */
            }
            else if ((MM_IDLE_LIMITED_SERVICE == g_MmGlobalInfo.ucState)
                  && (NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW != g_MmGlobalInfo.usCauseVal))
            {
                /* �������ڱ���ԭ��ֵ12��������Ʒ���״̬���Գ��Ժ����ط�*/
                Mm_SndCcRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulTransactionId,
                    NAS_MMCM_REL_CAUSE_MM_LIMIT_SERVICE);
            }
            else
            {
                Mm_SndCcRelInd( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulTransactionId,
                          NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);                      /* ֪ͨCCָ��������MM���ӽ���ʧ��           */

            }

            if ( MM_FALSE == ucLUFlg )
            {
                Mm_SndMmcCmSvcInd(MM_CS_SERV_NOT_EXIST);                /* ֪ͨMMC��CS���ҵ���Ѿ�ֹͣ              */
                NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_NOT_EXIST);
            }
        }
    }
    else if ( MM_TRUE ==
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg )
    {
        if ((MM_IDLE_LIMITED_SERVICE == g_MmGlobalInfo.ucState)
              && (NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW != g_MmGlobalInfo.usCauseVal))
        {
            /* �������ڱ���ԭ��ֵ12��������Ʒ���״̬���Գ���ss�ط� */
            Mm_SndSsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ulTransactionId,
                           NAS_MMCM_REL_CAUSE_MM_LIMIT_SERVICE);
        }
        else
        {
            Mm_SndSsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ulTransactionId,
                           NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);
        }

    }
    else if ( MM_TRUE ==
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg )
    {
        if ((MM_IDLE_LIMITED_SERVICE == g_MmGlobalInfo.ucState)
              && (NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW != g_MmGlobalInfo.usCauseVal))
        {
            /* �������ڱ���ԭ��ֵ12��������Ʒ���״̬���Գ���ss�ط� */
            Mm_SndSmsRelInd( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ulTransactionId,
                           NAS_MMCM_REL_CAUSE_MM_LIMIT_SERVICE);
        }
        else
        {
            Mm_SndSmsRelInd( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ulTransactionId,
                           NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);
        }

    }
    else if (MM_PAGING_PLMNSRCH_TRUE == g_MmSubLyrShare.MmShare.ucPagingPlmnSrch)
    {
        g_MmSubLyrShare.MmShare.ucPagingPlmnSrch = MM_PAGING_PLMNSRCH_FALSE;
        if (NAS_MML_NET_RAT_TYPE_GSM == NAS_MML_GetCurrNetRatType())
        {
            Mm_MakeGPagingInd(&stGPagingMsg);
            gaMmStateTable[30][g_MmGlobalInfo.ucState](&stGPagingMsg);
        }
        else
        {
            Mm_MakeWPagingInd(&stWPagingMsg);
            gaMmStateTable[30][g_MmGlobalInfo.ucState](&stWPagingMsg);
        }
    }
    else
    {                                                                   /* û�еȴ���CCҪ������MM����             */
        if ( MM_FALSE == ucLUFlg )
        {
            Mm_SndMmcCmSvcInd(MM_CS_SERV_NOT_EXIST);                    /* ֪ͨMMC��CS���ҵ���Ѿ�ֹͣ              */
            NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_NOT_EXIST);
        }
    }

    return;
}



VOS_VOID Mm_ComCheckDelayMmConn(
                            VOS_UINT8   ucLUFlg                                     /* �Ƿ���LU��������õĸú���               */
                    )
{
    if ( ((MM_CONST_NUM_8 >
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.
        ulTransactionId ) ||(MM_CONST_NUM_14 <
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.
        ulTransactionId )) && (MM_TRUE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg) )
    {
        /* TI���Ϸ�                                 */
        Mm_SndCcRelInd(g_MmCcEstReq.ulTransactionId,
            NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);        /* ֪ͨCC����ʧ��                           */
    }
    else
    {                                                                           /* TI�Ϸ�                                   */
        switch ( g_MmGlobalInfo.ucState )
        {                                                                       /* ��ǰ״̬                                 */
        case MM_IDLE_NORMAL_SERVICE:
            Mm_ComCheckDelayMmConn_MM_IDLE_NORMAL_SERVICE(ucLUFlg);
            break;
        case MM_IDLE_ATTEMPTING_TO_UPDATE:
            Mm_ComCheckDelayMmConn_MM_IDLE_ATTEMPTING_TO_UPDATE(ucLUFlg);
            break;
        case MM_IDLE_LIMITED_SERVICE:
        case MM_IDLE_NO_IMSI:
            Mm_ComCheckDelayMmConn_Other_State_Handling(ucLUFlg);
            break;
        case MM_IDLE_LOCATION_UPDATE_NEEDED:
            if (MM_COM_SRVST_NORMAL_SERVICE == NAS_MM_GetLauUptNeededCsSrvStatus())
            {
                Mm_ComCheckDelayMmConn_MM_IDLE_NORMAL_SERVICE(ucLUFlg);
            }
            else
            {
                Mm_ComCheckDelayMmConn_Other_State_Handling(ucLUFlg);
            }
            break;
        case MM_CONNECTION_ACTIVE:
            Mm_ComCheckDelayMmConn_MM_CONNECTION_ACTIVE();
            break;
        default:
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_ComCheckDelayMmConn:WARNING: g_MmGlobalInfo.ucState Abnormal");
            break;
        }
    }

    g_MmSubLyrShare.MmShare.ucPagingPlmnSrch = MM_PAGING_PLMNSRCH_FALSE;

    return;
}
VOS_UINT8 Mm_ComRrConnRelChk()
{
    if ( ( ( MM_MM_CONN_PROC != g_MmGlobalInfo.ucProc ) ||
        ( NAS_MML_REG_FAIL_CAUSE_IMSI_UNKNOWN_IN_VLR !=
        g_MmGlobalInfo.usCauseVal ) )
        && (  MM_FALSE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg )
        && (  MM_FALSE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg )
        && (  MM_FALSE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg ))
    {                                                                           /* ԭ��ֵ����4�������̲���MM����û�л���    */
        return MM_TRUE;
    }
    else
    {
        if (( MM_MM_CONN_PROC == g_MmGlobalInfo.ucProc ) &&
        ( NAS_MML_REG_FAIL_CAUSE_IMSI_UNKNOWN_IN_VLR ==  g_MmGlobalInfo.usCauseVal )
        && (  MM_FALSE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg )
        && (  MM_FALSE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg )
        && (  MM_FALSE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg ))
        {                                                                       /* û�л���                                 */
            if ((VOS_TRUE == NAS_MML_GetPsAttachAllowFlg())
             && (MM_NET_MODE_I == g_MmGlobalInfo.ucNtMod)
             && (MM_FALSE == g_MmGlobalInfo.ucLikeB))
            {                                                                   /* �ж��Ƿ�ΪA+Iģʽ                        */
                return MM_TRUE;
            }
        }
    }
    return MM_FALSE;
}
/***********************************************************************
 *  MODULE   : Mm_ComTiChk
 *  FUNCTION : TI�ĺϷ��Լ��
 *  INPUT    : VOS_VOID
 *  OUTPUT   : VOS_VOID
 *  RETURN   : VOS_VOID
 *  NOTE     :
 *  HISTORY  :
 *     1.  ��־��     2005.01.31  �°�����
       2.  ��־��  2005.03.09  MM_Review_HW_BUG_027��Ӧ
 ************************************************************************/
VOS_UINT8 Mm_ComTiChk()
{
    if ( ((MM_CONST_NUM_8 >
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
                                RcvXXEstReq.ulTransactionId )
        ||(MM_CONST_NUM_14 <
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
                                RcvXXEstReq.ulTransactionId ))
        && (MM_TRUE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg) )
    {                                                                           /* TI���Ϸ�                                 */
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg
                                                                    = MM_FALSE;

        NAS_MM_UpdateCsServiceBufferStatusFlg();
        return MM_FALSE;
    }
    if ( MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS]
                                                    .RcvXXEstReq.ucFlg )
    {
        if ( ( MM_CONST_NUM_8 >
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.
            ulTransactionId )
            || ( MM_CONST_NUM_14 <
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.
            ulTransactionId ) )
        {                                                                       /* TI���Ϸ�                                 */
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS]
                                      .RcvXXEstReq.ucFlg = MM_FALSE;

            NAS_MM_UpdateCsServiceBufferStatusFlg();
            return MM_FALSE;
        }
    }
    if ( MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS]
                                                    .RcvXXEstReq.ucFlg )
    {
        if ( ( MM_CONST_NUM_8 >
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.
            ulTransactionId )
            || ( MM_CONST_NUM_14 <
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.
            ulTransactionId ) )
        {                                                                       /* TI���Ϸ�                                 */
            /* Modified by cxd for MM_Review_HW_BUG_027 2005.03.09 begin */
            /*return MM_FALSE;
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS]
                                       .RcvXXEstReq.ucFlg = MM_FALSE; */
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS]
                .RcvXXEstReq.ucFlg = MM_FALSE;

            NAS_MM_UpdateCsServiceBufferStatusFlg();
            return MM_FALSE;
            /* Modified by cxd for MM_Review_HW_BUG_027 2005.03.09 end   */
        }
    }

    return MM_TRUE;
}
VOS_VOID Mm_ComSigRelForDelayMmConnRej()
{
    switch ( g_MmGlobalInfo.ucState )
    {                                                                           /* ��ǰ״̬                                 */
    case MM_IDLE_NORMAL_SERVICE:
        break;
    case MM_CONNECTION_ACTIVE:
        break;
    case MM_IDLE_ATTEMPTING_TO_UPDATE:
    case MM_IDLE_LIMITED_SERVICE:
    case MM_IDLE_LOCATION_UPDATE_NEEDED:
    case MM_IDLE_NO_IMSI:
        if ( MM_TRUE ==
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg )
        {                                                                       /* �ɵȴ���CC��������Ҫ������MM����         */
            if ( MMCC_EMERGENCY_CALL ==
                g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
                RcvXXEstReq.ulCallType )
            {                                                                   /* �ǽ�������                               */
                Mm_SndMmcRrConnInfInd( MMC_MM_RR_CONN_ABSENT );
                NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_NOT_EXIST);
            }
            else
            {                                                                   /* ����������                               */
                Mm_SndMmcRrConnInfInd( MMC_MM_RR_CONN_ABSENT );
                NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_NOT_EXIST);
            }
        }
        else if ( MM_TRUE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg )
        {
            Mm_SndMmcRrConnInfInd( MMC_MM_RR_CONN_ABSENT );
            NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_NOT_EXIST);
        }
        else if ( MM_TRUE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg )
        {
            Mm_SndMmcRrConnInfInd( MMC_MM_RR_CONN_ABSENT );
            NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_NOT_EXIST);
        }
        else
        {

        }
        break;
    default:
        break;
    }
    return;
}

/*****************************************************************************
 �� �� ��  : NAS_MM_FillMobileID
 ��������  : ����MM��ȫ�ֱ���ȡֵ�����MobileID��ֵ
 �������  : ��
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2011��7��25��
   ��    ��   : zhoujun /40661
   �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID  NAS_MM_FillMobileID(
    MM_MOBILE_ID_STRU                  *pstMobileID
)
{
    VOS_UINT8                          *pucIMSI;

    pstMobileID->ucMsIdFlg = g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg;

    PS_MEM_CPY(pstMobileID->aucImei,
               g_MmGlobalInfo.MsCsInfo.MobileId.aucImei,
               sizeof(pstMobileID->aucImei));

    PS_MEM_CPY(pstMobileID->aucImeisv,
               g_MmGlobalInfo.MsCsInfo.MobileId.aucImeisv,
               sizeof(pstMobileID->aucImeisv));

    PS_MEM_CPY(pstMobileID->aucTmsi,
               NAS_MML_GetUeIdTmsi(),
               sizeof(pstMobileID->aucTmsi));

    pucIMSI = NAS_MML_GetSimImsi();

    pstMobileID->Imsi.ucLength = pucIMSI[0];

    PS_MEM_CPY(pstMobileID->Imsi.aucImsi,
               pucIMSI + 1,
               sizeof(pstMobileID->Imsi.aucImsi));


    return;
}



VOS_VOID Mm_ComLuOnly()
{
    MM_MSG_LU_REQ_STRU     Msg;

    NAS_MML_MS_CAPACILITY_INFO_STRU    *pstMsCapability = VOS_NULL_PTR;
    NAS_MML_SIM_TYPE_ENUM_UINT8         ucSimType;


    pstMsCapability = NAS_MML_GetMsCapability();
    ucSimType       = NAS_MML_GetSimType();


    /* ����LAUǰ������Ա�־λ */
    g_MmGlobalInfo.ucRetryLauFlg = VOS_FALSE;

    if (VOS_TRUE == NAS_MML_GetCsRestrictRegisterFlg())
    {
        return;
    }
    if ( VOS_TRUE == NAS_MML_GetCsAttachAllowFlg())
    {

        Mm_TimerStop(MM_TIMER_T3211);
        Mm_TimerStop(MM_TIMER_T3213);

        /* 4.4.4.1 Location updating initiation by the mobile station
           Any timer used for triggering the location updating procedure
           (e.g. T3211, T3212) is stopped if running. */
        Mm_TimerStop(MM_TIMER_T3212);



        Msg.MmIeLuType.ucLUT =
            g_MmGlobalInfo.LuInfo.ucLuType & MM_IE_LU_TYPE_LUT_MASK;            /* ����LU TYPE                              */
        if ( MM_TRUE ==
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg )
        {                                                                       /* �б�����MM����                           */
            if ( ( MM_CONST_NUM_7 > g_MmGlobalInfo.
                ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulTransactionId ) ||
                ( ( MM_CONST_NUM_7 < g_MmGlobalInfo.
                ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulTransactionId ) &&
                ( MM_CONST_NUM_15 > g_MmGlobalInfo.
                ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulTransactionId ) ) )
            {                                                                   /* TI�Ϸ�                                   */
                Msg.MmIeLuType.ucFOR = MM_IE_LU_TYPE_FOR_MASK;                          /* ����FOR                                  */
            }
            else
            {
                /* TI���Ϸ�, ֪ͨCC����ʧ�� */
                Mm_SndCcRelInd( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
                    RcvXXEstReq.ulTransactionId, NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);

                g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg =
                    MM_FALSE;                                                   /* ����û�л����MM����                     */
                Msg.MmIeLuType.ucFOR = MM_CONST_NUM_0;                          /* ����FOR                                  */
            }
        }
        else if ( MM_TRUE ==
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg )
        {                                                                       /* �б�����MM����                           */
            if ( ( MM_CONST_NUM_7 > g_MmGlobalInfo.
                ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ulTransactionId ) ||
                ( ( MM_CONST_NUM_7 < g_MmGlobalInfo.
                ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ulTransactionId ) &&
                ( MM_CONST_NUM_15 > g_MmGlobalInfo.
                ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ulTransactionId ) ) )
            {                                                                   /* TI�Ϸ�                                   */
                Msg.MmIeLuType.ucFOR = MM_IE_LU_TYPE_FOR_MASK;                          /* ����FOR                                  */
            }
            else
            {                                                                   /* TI���Ϸ�                                 */
                Mm_SndSsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ulTransactionId,
                               NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);

                g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg =
                    MM_FALSE;                                                   /* ����û�л����MM����                     */
                Msg.MmIeLuType.ucFOR = MM_CONST_NUM_0;                          /* ����FOR                                  */
            }
        }
        else if ( MM_TRUE ==
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg )
        {                                                                       /* �б�����MM����                           */
            if ( ( MM_CONST_NUM_7 > g_MmGlobalInfo.
                ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ulTransactionId ) ||
                ( ( MM_CONST_NUM_7 < g_MmGlobalInfo.
                ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ulTransactionId ) &&
                ( MM_CONST_NUM_15 > g_MmGlobalInfo.
                ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ulTransactionId ) ) )
            {                                                                   /* TI�Ϸ�                                   */
                Msg.MmIeLuType.ucFOR = MM_IE_LU_TYPE_FOR_MASK;                          /* ����FOR                                  */
            }
            else
            {                                                                   /* TI���Ϸ�                                 */
                Mm_SndSmsRelInd( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].
                    RcvXXEstReq.ulTransactionId,
                    NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);                            /* ֪ͨCC����ʧ��                           */

                g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.
                    ucFlg = MM_FALSE;                                           /* ����û�л����MM����                     */
                Msg.MmIeLuType.ucFOR = MM_CONST_NUM_0;                          /* ����FOR                                  */
            }
        }
        else
        {                                                                       /* û�б�����MM����                         */
            Msg.MmIeLuType.ucFOR = MM_CONST_NUM_0;                              /* ����FOR                                  */
        }
        /* SIM����W�½���CS ע��ʱ�������ǰLAI��Ч����CKSNҲ��Ϊ��Ч */
        if ((NAS_MML_NET_RAT_TYPE_WCDMA == NAS_MML_GetCurrNetRatType())
         && (NAS_MML_SIM_TYPE_SIM == ucSimType)
         && ((MM_INVALID_LAI == g_MmGlobalInfo.MsCsInfo.OldLai.ulLac)
          || (0x0 == g_MmGlobalInfo.MsCsInfo.OldLai.ulLac)))
        {
            NAS_MML_SetSimCsSecurityCksn(MM_CKSN_INVALID);
        }
        Msg.MmIeCksn.ucCksn = NAS_MML_GetSimCsSecurityCksn();             /* ����CKSN                                 */
        Msg.MmIeLAI.IeLai.PlmnId.ulMcc =
                            g_MmGlobalInfo.MsCsInfo.OldLai.PlmnId.ulMcc;        /* ����MCC                                  */
        Msg.MmIeLAI.IeLai.PlmnId.ulMnc =
                            g_MmGlobalInfo.MsCsInfo.OldLai.PlmnId.ulMnc;        /* ����MNC                                  */
        Msg.MmIeLAI.IeLai.ulLac = g_MmGlobalInfo.MsCsInfo.OldLai.ulLac;         /* ����LAC                                  */

        NAS_MM_FillMobileID(&(Msg.MmIeMobileId.MobileID));

        if ( MM_CONST_NUM_0 != pstMsCapability->aucClassmark2[0] )
        {
            Msg.ucMsClsMskFrUMTSFlg = MM_TRUE;                                  /* ����ClassMark For UMTS����               */
            MM_Fill_IE_ClassMark2(Msg.MmIeMsClsMskFrUMTS.aucClassmark2);
        }
        else
        {
            Msg.ucMsClsMskFrUMTSFlg = MM_FALSE;                                 /* ����ClassMark For UMTS������             */
        }


        /* ����LAUʱ�����ATTEMPT UPDATE LAI��Ϣ */
        NAS_MM_InitLaiInfo(NAS_MM_GetAttemptUpdateLaiInfo());
        
        Mm_ComMsgLuReqSndOfEstReq(&Msg);                                        /* ������Ϣ                                 */
        NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_ESTING);
        Mm_TimerStart(MM_TIMER_PROTECT_SIGNALLING);                             /* ��������TIMER                            */

        g_MmSubLyrShare.GmmShare.ucGsAssociationFlg = VOS_FALSE;

        g_MmGlobalInfo.ucStaOfRcvXXEstReq = g_MmGlobalInfo.ucState;

        NAS_MML_SetDelayedCsfbLauFlg(VOS_FALSE);


        Mm_ComSetMmState(WAIT_FOR_RR_CONNECTION_LOCATION_UPDATING);
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComLuOnly:NORMAL: STATUS is WAIT_FOR_RR_CONNECTION_LOCATION_UPDATING");

        NAS_EventReport(WUEPS_PID_MM,NAS_OM_EVENT_LOCATION_UPDATE_REQ,
                        &g_MmGlobalInfo.LuInfo.ucLuType,NAS_OM_EVENT_LAU_REQ_LEN);
    }
}


VOS_VOID Mm_ComInit ()
{

    VOS_UINT8                           i;
    VOS_UINT32                          ulHoWaitSysinfoTimeLen;
    ulHoWaitSysinfoTimeLen = NAS_MML_GetHoWaitSysinfoTimerLen();

    PS_MEM_SET( &gstMmTimer[0], 0, ( MM_TIMER_NUM * sizeof( MM_TIMER_STRU ) ) );/* ��ʼ��                                   */
    PS_MEM_SET( &g_MmGlobalInfo, 0, sizeof( MM_GLOBAL_CTRL_STRU ) );           /* ��ʼ��                                   */
    g_MmGlobalInfo.ucMmServiceState = MM_NO_SERVICE;

    PS_MEM_SET(&gstMmSuspendMsgQue, 0, sizeof(MM_SUSPEND_MSG_BUF_ST));        /* ��ʼ��                                   */
    PS_MEM_SET(&gstMmTimerSuspend, 0, sizeof(MM_TIMER_ST));                   /* ��ʼ��                                   */

    g_MmMsgLuAccpt.MmIePlmnList.ucPlmnNum = 0;

    gstMmTimerSuspend.ulTimerLength     = MM_TIMER_PROTECT_SUSPEND_VALUE;

    PS_MEM_SET(&g_MmMsgLuAccpt, 0, sizeof(MM_MSG_LU_ACCPT_STRU));
    PS_MEM_SET(&g_MmMsgMmInfo, 0, sizeof(MM_MSG_MM_INFO_STRU));

    PS_MEM_SET(&g_AgentUsimAuthCnf, 0, sizeof(MM_USIM_AUTHENTICATION_CNF_STRU));

    g_MmSubLyrShare.MmShare.ucCsAttachState = MM_STATUS_DETACHED;

    g_MmGlobalInfo.ucSysInfoFlg =  MM_FALSE;

    g_MmSubLyrShare.MmShare.ucSpecProFlg =  MM_NOT_UPDATED_SPECIFIC_PROCEDURE;

    g_MmGlobalInfo.ucConnRcvSysFlg = MM_FALSE;

    g_MmGlobalInfo.ucFollowOnFlg         = MM_NO_FOLLOW_ON;
    g_MmGlobalInfo.ucCsSigConnFlg        = MM_CS_SIG_CONN_ABSENT;

    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComInit:INFO: MmCsSigConnFlg is MM_CS_SIG_CONN_ABSENT");
    g_MmGlobalInfo.AuthenCtrlInfo.ucRandFlg = MM_AUTHEN_RAND_ABSENT;

    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI   =
        NO_MM_CONN_ESTING;                                                      /* û�����ڽ�����MM����                     */
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg  =
        MM_FALSE;
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].ucMMConnEstingTI   =
        NO_MM_CONN_ESTING;                                                      /* û�����ڽ�����MM����                     */
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg  =
        MM_FALSE;
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].ucMMConnEstingTI  =
        NO_MM_CONN_ESTING;                                                      /* û�����ڽ�����MM����                     */
    g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg =
        MM_FALSE;
    g_MmGlobalInfo.ucMMConnEstingPD                                 =
        NO_MM_CONN_ESTING;

    g_NasMmImportFunc.SigConnCtrl.RrEstReqFunc = MM_WasRrEstReq;
    g_NasMmImportFunc.SigConnCtrl.RrRelReqFunc = As_RrRelReq;
    g_NasMmImportFunc.SigDataTransfer.RrDataReqFunc = MM_WasRrDataReq;

    g_NasMmImportFunc.ucFuncEnvironment             = NAS_MML_NET_RAT_TYPE_WCDMA;
    g_stMmNsd.ucNsdMod = MM_NSD_MOD_R99_ONWARDS;

    g_MmGlobalInfo.ucLikeB = MM_FALSE;
    g_MmGlobalInfo.ucAttDelay = MM_FALSE;

    g_MmGlobalInfo.LuInfo.ucLuAttmptCnt = MM_CONST_NUM_0;
    g_MmGlobalInfo.LuInfo.ucRetryFlg = MM_FALSE;
    g_MmGlobalInfo.LuInfo.ucT3212ExpiredFlg = MM_FALSE;

    g_MmGlobalInfo.ucCsCipherAlgor = NAS_MM_CIPHER_ALGO_NOPRESENT;
    g_MmSubLyrShare.MmShare.ucCsSpecificFlg = MM_SPECIFIC_OFF;

    g_MmGlobalInfo.ucPowerOnFlg = MM_TRUE;

    for ( i = 0; i < MM_TIMER_NUM; i++ )
    {
        gstMmTimer[i].ucTimerStatus = MM_TIMER_STOP;

        /* ����ʱ���޸�Ϊ��ѭ����ʱ�� */
        gstMmTimer[i].ucMode        = VOS_RELTIMER_NOLOOP;
    }
    
    /* ��ʼ��TIMER��ʱ�� */
    gstMmTimer[MM_TIMER_T3210].ulTimerLen = MM_TIMER_T3210_VALUE;
    gstMmTimer[MM_TIMER_T3211].ulTimerLen = MM_TIMER_T3211_VALUE;
    gstMmTimer[MM_TIMER_T3213].ulTimerLen = MM_TIMER_T3213_VALUE;
    gstMmTimer[MM_TIMER_T3214].ulTimerLen = MM_TIMER_T3214_VALUE;
    gstMmTimer[MM_TIMER_T3216].ulTimerLen = MM_TIMER_T3216_VALUE;
    gstMmTimer[MM_TIMER_T3218].ulTimerLen = MM_TIMER_T3218_VALUE;
    gstMmTimer[MM_TIMER_T3220].ulTimerLen = MM_TIMER_T3220_VALUE;

    gstMmTimer[MM_TIMER_T3230].ulTimerLen = MM_TIMER_T3230_VALUE;

    gstMmTimer[MM_TIMER_T3240].ulTimerLen = MM_TIMER_T3240_VALUE;
    gstMmTimer[MM_TIMER_PROTECT_AGENT].ulTimerLen =
                                    MM_TIMER_PROTECT_AGENT_VALUE;
    gstMmTimer[MM_TIMER_PROTECT_DETACH].ulTimerLen =
                                    MM_TIMER_PROTECT_DETACH_VALUE;
    gstMmTimer[MM_TIMER_PROTECT_SIGNALLING].ulTimerLen
                                     = MM_TIMER_PROTECT_W_SIGNALLING_VALUE;

    gstMmTimer[MM_TIMER_PROTECT_CC].ulTimerLen = MM_TIMER_PROTECT_CC_VALUE;
    gstMmTimer[MM_TIMER_PROTECT_CCBS].ulTimerLen = MM_TIMER_PROTECT_CCBS_VALUE;
    gstMmTimer[MM_TIMER_DELAY_LU_GSM].ulTimerLen = MM_TIMER_DELAY_LU_GSM_VALUE;
    gstMmTimer[MM_TIMER_DELAY_CS_SERVICE_GSM].ulTimerLen = MM_TIMER_DELAY_CS_SERVICE_GSM_VALUE;

    gstMmTimer[MM_TIMER_WAIT_CONNECT_REL].ulTimerLen = MM_TIMER_WAIT_CONNECT_REL_VALUE;

    gstMmTimer[MM_TIMER_HO_WAIT_SYSINFO].ulTimerLen = ulHoWaitSysinfoTimeLen;

    gstMmTimer[MM_TIMER_EMERGENCY_CSFB_HO_WAIT_SYSINFO].ulTimerLen = ulHoWaitSysinfoTimeLen;


    gstMmTimer[MM_TIMER_MODE_I_CS_PS_POWER_OFF_PROTECT].ulTimerLen = NAS_MM_TIMER_MODE_I_CS_PS_POWER_OFF_PROTECT_LEN_VALUE;

    gstMmTimer[MM_TIMER_WAIT_GET_HO_SECU_INFO_CNF].ulTimerLen = MM_TIMER_WAIT_GET_HO_SECU_INFO_CNF_LEN_VALUE;

    g_MmSubLyrShare.MmShare.ucCsAttachState = MM_STATUS_DETACHED;


    g_MmSubLyrShare.MmShare.ucPagingPlmnSrch = MM_PAGING_PLMNSRCH_FALSE;
    g_MmGlobalInfo.AuthenCtrlInfo.ucRandRepeat              = MM_AUTHEN_RAND_DIFFER;
    g_MmGlobalInfo.AuthenCtrlInfo.ucLastAuthType            = MM_CONST_NUM_2;

    g_T3211Flag = 0;
    g_MmGlobalInfo.ucMmServiceState = MMC_MM_NO_SERVICE;

    g_MmSubLyrShare.MmShare.ucCsIntegrityProtect        = NAS_MML_RRC_INTEGRITY_PROTECT_DEACTIVE;


    g_MmSubLyrShare.MmShare.MmSuspendEvent              = MM_EVENT_ID_INVALID;


    /* ��ʼ��Ϊ���ӳٵ�LAU��־ */
    NAS_MML_SetDelayedCsfbLauFlg(VOS_FALSE);

    g_MmGlobalInfo.enPreRatType                         = NAS_MML_NET_RAT_TYPE_BUTT;

    g_MmGlobalInfo.ucLauAcceptContainDiffNbLaiFlg  = VOS_FALSE;

#if (FEATURE_ON == FEATURE_IMS)
    NAS_MM_SetSrvccFlg(VOS_FALSE);
#endif    

    NAS_MM_InitLaiInfo(NAS_MM_GetAttemptUpdateLaiInfo());

    return;                                                                     /* ����                                     */
}


VOS_VOID    Mm_Nop(VOS_VOID* pMsg)
{
}
VOS_VOID Mm_ComStart(
                 VOS_VOID
                 )
{

    VOS_UINT8                           i;
    NAS_MML_MS_CAPACILITY_INFO_STRU    *pstMsCapability = VOS_NULL_PTR;
    NAS_MML_LAI_STRU                   *pstLastSuccLai  = VOS_NULL_PTR;
    VOS_UINT8                           ulInvalidCnt;
    VOS_UINT8                          *pucTmpTmsi;

    /*���ݿ�״̬��Ϣ������CS��Sim��״̬*/
    if (VOS_TRUE == NAS_MML_GetSimPresentStatus())
    {
        NAS_MML_SetSimCsRegStatus(VOS_TRUE);
    }
    else
    {
        NAS_MML_SetSimCsRegStatus(VOS_FALSE);
    }

    pstLastSuccLai = NAS_MML_GetCsLastSuccLai();

    if (VOS_TRUE == NAS_MML_GetSimCsRegStatus())
    {                                                                           /* SIM��Ч                                  */
        /* IMSI */
        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg
           |= MM_MS_ID_IMSI_PRESENT;                                            /* ��Mobile ID���ڱ�־                      */

        g_MmGlobalInfo.MsCsInfo.OldLai.ulLac = ((pstLastSuccLai->aucLac[0] << 8) | (pstLastSuccLai->aucLac[1]));
        PS_MEM_CPY(&g_MmGlobalInfo.MsCsInfo.OldLai.PlmnId , &pstLastSuccLai->stPlmnId, sizeof(MM_PLMN_ID_STRU));

        g_MmGlobalInfo.MsCsInfo.CurLai.ulLac = ((pstLastSuccLai->aucLac[0] << 8) | (pstLastSuccLai->aucLac[1]));
        PS_MEM_CPY(&g_MmGlobalInfo.MsCsInfo.CurLai.PlmnId , &pstLastSuccLai->stPlmnId, sizeof(MM_PLMN_ID_STRU));


        /* ���ݵ�ǰ�洢��TMSI�Ƿ���Ч���趨�Ƿ����TMSI */
        ulInvalidCnt = 0;
        pucTmpTmsi = NAS_MML_GetUeIdTmsi();
        for (i = 0; i < NAS_MML_MAX_PTMSI_LEN; i++)
        {
            if (NAS_MML_TMSI_INVALID == pucTmpTmsi[i])
            {
                ulInvalidCnt++;
            }
        }

        if ( ulInvalidCnt < NAS_MML_MAX_PTMSI_LEN )
        {
            g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg
                |= MM_MS_ID_TMSI_PRESENT;

        }


        /* ������䵱ǰTMSI������ID */
        if ( MM_MS_ID_TMSI_PRESENT ==
              (MM_MS_ID_TMSI_PRESENT & g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg) )
        {
            g_MmGlobalInfo.MsCsInfo.stPlmnId = g_MmGlobalInfo.MsCsInfo.OldLai.PlmnId;
        }
    }

    /* IMEI(SV) */
    pstMsCapability = NAS_MML_GetMsCapability();
    g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg |= MM_MS_ID_IMEI_PRESENT;
    g_MmGlobalInfo.MsCsInfo.MobileId.aucImei[0]
       = (VOS_UINT8)((pstMsCapability->aucImeisv[0] << 4) | 0x0f);

    g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg |= MM_MS_ID_IMEISV_PRESENT;
    g_MmGlobalInfo.MsCsInfo.MobileId.aucImeisv[0]
       = g_MmGlobalInfo.MsCsInfo.MobileId.aucImei[0];

    for (i = MM_CONST_NUM_1; i < MM_CONST_NUM_8; i++ )
    {                                                                           /* �洢IMEI                                 */
        g_MmGlobalInfo.MsCsInfo.MobileId.aucImei[i]
           = (VOS_UINT8)((pstMsCapability->aucImeisv[2*i]<<4)
                       | pstMsCapability->aucImeisv[(2*i)-1]);
        g_MmGlobalInfo.MsCsInfo.MobileId.aucImeisv[i]
           = g_MmGlobalInfo.MsCsInfo.MobileId.aucImei[i];
    }
    g_MmGlobalInfo.MsCsInfo.MobileId.aucImei[7] &= 0x0f;
    g_MmGlobalInfo.MsCsInfo.MobileId.aucImeisv[8]
       = (VOS_UINT8)( pstMsCapability->aucImeisv[NV_ITEM_IMEI_SIZE-1]
                   & 0x0f);
    g_MmGlobalInfo.MsCsInfo.MobileId.aucImeisv[8] |= 0xf0;




    /* Ϊ�˷�ֹʹ�ò��ԺŶαܹ�IMSI�ļ�飬ʹ���ܹ�ʹ��TMSI����ע�ᣬ��Ҫ
       ���з��������ж�����NV��������£�����Ŷ�TMSI��Ϊ��Ч
    */
    if (VOS_TRUE == NAS_PreventTestImsiRegFlg())
    {
        NAS_MML_InitUeIdTmsiInvalid();

        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &= ~MM_MS_ID_TMSI_PRESENT;
    }


    Mm_SndMmcStartCnf();
}

VOS_VOID Mm_ComToU2Handle()
{
    /* Modified by libin agent for MM 20050131 begin */
    /* g_MmSubLyrShare.MmShare.ucUpdateSts = MM_UPDATE_STATUS_NOT_UPDATED; */
    NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_NOT_UPDATED);
    /* Modified by libin agent for MM 20050131 end */
    Mm_ComNasInfoSav(
                    MM_STATUS_DETACHED,
                    MM_CKSN_INVALID
                    );                                                          /* ����MM��Ϣ                               */
    Mm_ComDelLai();                                                             /* ɾ��LAI                                  */
    g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &= ~MM_MS_ID_TMSI_PRESENT;

    NAS_MML_InitUeIdTmsiInvalid();

    Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                    /* ����SIM������״̬                        */

    /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID); */                    /* ����SIM������״̬                        */
    if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
    {
        Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
    }
    else
    {
        Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
    }

    Mm_ComSetMmState(MM_IDLE_ATTEMPTING_TO_UPDATE);

    g_MmGlobalInfo.ucPowerOnFlg = MM_FALSE;

    /*WUEPS_TRACE( MM_LOG_LEVEL_1,                                              */
    /*" \nMM:STATUS is  MM_IDLE_ATTEMPTING_TO_UPDATE\r " );                     */
    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComToU2Handle:NORMAL: STATUS is MM_IDLE_ATTEMPTING_TO_UPDATE");
}

VOS_VOID Mm_ComLuCauseHandle(VOS_UINT16 usCause)
{
	NAS_MML_LOCATION_UPDATE_STATUS_ENUM_UINT8               enCsUpdateStatus;

    g_MmGlobalInfo.usCauseVal   = usCause;                            /* ��¼ԭ��ֵ                               */
    switch(usCause)
    {
    case NAS_MML_REG_FAIL_CAUSE_IMSI_UNKNOWN_IN_HLR:
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_MS:
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME:

        g_MmGlobalInfo.ucMmServiceState = MM_NO_IMSI;

        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComLuCauseHandle:INFO: MmServiceState is MM_NO_IMSI");

        NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_PLMN_NOT_ALLOWED);              /* ���ø���״̬                             */
        NAS_MML_SetSimCsRegStatus(VOS_FALSE);
        Mm_ComNasInfoSav(MM_STATUS_DETACHED, MM_CKSN_INVALID);
        Mm_ComDelLai();                                                         /* ɾ��LAI                                  */
        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &= ~MM_MS_ID_TMSI_PRESENT;

        NAS_MML_InitUeIdTmsiInvalid();

        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                                ~MM_MS_ID_IMSI_PRESENT;

        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ����SIM������״̬                        */

        if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
        }
        else
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
        }

        Mm_ComSetMmState(MM_IDLE_NO_IMSI);
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComLuCauseHandle:NORMAL: STATUS is MM_IDLE_NO_IMSI");
        break;
    case NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW:
        g_MmGlobalInfo.ucMmServiceState = MM_LIMITED_SERVICE;

        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComLuCauseHandle:INFO: MmServiceState is MM_LIMITED_SERVICE");

        g_MmGlobalInfo.LuInfo.ucLuAttmptCnt = MM_CONST_NUM_0;                   /* ����LU COUNTERΪ0                        */
        NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_LOCATION_AREA_NOT_ALLOWED);              /* ���ø���״̬                             */
        Mm_ComNasInfoSav(
                        MM_STATUS_DETACHED,
                        MM_CKSN_INVALID
                        );                                                      /* ����MM��Ϣ                               */
        Mm_ComDelLai();                                                         /* ɾ��LAI                                  */
        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &= ~MM_MS_ID_TMSI_PRESENT;   /* ����TMIS������                           */

        NAS_MML_InitUeIdTmsiInvalid();

        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ����SIM������״̬                        */

        if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
        }
        else
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
        }

        Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComLuCauseHandle:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
        break;
    case NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL:
        g_MmGlobalInfo.ucMmServiceState = MM_LIMITED_SERVICE;

        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComLuCauseHandle:INFO: MmServiceState is MM_LIMITED_SERVICE");

        g_MmGlobalInfo.LuInfo.ucLuAttmptCnt = MM_CONST_NUM_0;                   /* ����LU COUNTERΪ0                        */
        NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_LOCATION_AREA_NOT_ALLOWED);              /* ���ø���״̬                             */
        Mm_ComNasInfoSav(
                        MM_STATUS_DETACHED,
                        NAS_MML_GetSimCsSecurityCksn()
                        );                                                      /* ����MM��Ϣ                               */

        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ����SIM������״̬                        */


        Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComLuCauseHandle:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
        break;
    default:
        if (g_MmGlobalInfo.LuInfo.ucLuAttmptCnt < MM_CONST_NUM_4)
        {                                                                       /* �ж�LU Counter�Ƿ�С��4                  */
            enCsUpdateStatus    = NAS_MML_GetCsUpdateStatus();
            if ( ( MM_FALSE == Mm_ComLaiJudge() )
              && (NAS_MML_LOCATION_UPDATE_STATUS_UPDATED == enCsUpdateStatus) )
            {                                                                   /* �ж�LAI�Ƿ�δ�ı�,�Ҹ���״̬ΪU1         */
                g_MmGlobalInfo.ucMmServiceState = MM_NORMAL_SERVICE;

                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComLuCauseHandle:INFO: MmServiceState is MM_NORMAL_SERVICE");

                Mm_ComNasInfoSav(
                                 MM_STATUS_ATTACHED,
                                 NAS_MML_GetSimCsSecurityCksn()
                                 );
                Mm_ComSetMmState(MM_IDLE_NORMAL_SERVICE);

                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComLuCauseHandle:NORMAL: STATUS is MM_IDLE_NORMAL_SERVICE");
                Mm_TimerStart(MM_TIMER_T3211);                                  /* ����T3211                                */
            }
            else
            {
                if ( ( usCause >=
                    MM_IE_REJ_CAUSE_RETRY_UPON_ENTRY_INTO_NEW_CELL_LOW ) &&
                    ( usCause <=
                    MM_IE_REJ_CAUSE_RETRY_UPON_ENTRY_INTO_NEW_CELL_UP ) )
                {                                                               /* �ж�ԭ��ֵ�Ƿ���retry��Χ��              */
                    g_MmGlobalInfo.LuInfo.ucRetryFlg = MM_TRUE;                 /* ����ucRetryFlgΪMM_TRUE                  */
                }
                else
                {
                    g_MmGlobalInfo.LuInfo.ucRetryFlg = MM_FALSE;                /* ����ucRetryFlgΪMM_FALSE                 */
                }

                g_MmGlobalInfo.ucMmServiceState = MM_ATTEMPTING_TO_UPDATE;

                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComLuCauseHandle:INFO: MmServiceState is MM_ATTEMPTING_TO_UPDATE");


                /* ��U2״̬�����浱ǰ��LAI��Ϣ */
                PS_MEM_CPY(NAS_MM_GetAttemptUpdateLaiInfo(), 
                           &g_MmGlobalInfo.MsCsInfo.CurLai, sizeof(MM_LAI_STRU));            
            
                Mm_ComToU2Handle();
                Mm_TimerStart(MM_TIMER_T3211);                                  /* ����T3211                                */
            }
        }
        else
        {
            if ( ( usCause >
                MM_IE_REJ_CAUSE_RETRY_UPON_ENTRY_INTO_NEW_CELL_LOW ) &&
                ( usCause <
                MM_IE_REJ_CAUSE_RETRY_UPON_ENTRY_INTO_NEW_CELL_UP ) )
            {                                                                   /* �ж�ԭ��ֵ�Ƿ���retry��Χ��              */
                g_MmGlobalInfo.LuInfo.ucRetryFlg = MM_TRUE;                     /* ����ucRetryFlgΪMM_TRUE                  */
            }
            else
            {
                g_MmGlobalInfo.LuInfo.ucRetryFlg = MM_FALSE;                    /* ����ucRetryFlgΪMM_FALSE                 */
            }
            g_MmGlobalInfo.LuInfo.ucT3212ExpiredFlg = MM_FALSE;
            g_MmGlobalInfo.LuInfo.ucLuType = MM_IE_LUT_TYPE_BUTT;
            g_MmGlobalInfo.ucMmServiceState = MM_ATTEMPTING_TO_UPDATE;          /* ���÷���״̬                             */

            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComLuCauseHandle:INFO: MmServiceState is MM_ATTEMPTING_TO_UPDATE");


            /* ��U2״̬�����浱ǰ��LAI��Ϣ */
            PS_MEM_CPY(NAS_MM_GetAttemptUpdateLaiInfo(), 
                       &g_MmGlobalInfo.MsCsInfo.CurLai, sizeof(MM_LAI_STRU));            
            
            Mm_ComToU2Handle();


            
            /* SOR������MMC����MM����LAU */            
            Mm_TimerStart(MM_TIMER_T3212);
            
                

            /*A32D10964==>*/
            if (MM_DETACH_NEED_DELAY == g_MmGlobalInfo.ucDetachType)
            {
                Mm_TimerStop(MM_TIMER_PROTECT_DETACH);
                g_MmGlobalInfo.ucDetachType = MM_DETACH_TYPE_BUTT;
                Mm_Cell_S0_E23(VOS_NULL_PTR);
            }
            /*<==A32D10964*/
        }
        break;
    }

    g_MmGlobalInfo.ucProc     = MM_NULL_PROC;
    Mm_ComCheckDelayMmConn( MM_FALSE );
    g_MmGlobalInfo.usCauseVal = NAS_MML_REG_FAIL_CAUSE_NULL;
}



VOS_VOID Mm_ComGmmAttachCauseHandle(VOS_UINT16 usCause)
{
    /* Added by libin 05-03-22 begin: NAS_IT_BUG_017 */
    VOS_UINT8                               ucOldUpdateSts;
    VOS_UINT8                               ucTempUpdateSts;
    MM_LAI_STRU                         OldLai;
    MM_LAI_STRU                         TempLai;
    /* Added by libin 05-03-22 end */

    switch(usCause)
    {
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_MS:
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME:
		/* �ο�24.008��
		   If the MS is IMSI attached, the MS shall in addition
		   set the update status to U3 ROAMING NOT ALLOWED,
		   shall delete any TMSI, LAI and ciphering key sequence number.
		   If the MS is operating in MS operation mode A and an RR connection exists,
		   the MS shall abort the RR connection, unless an emergency call is ongoing.
		   The SIM/USIM shall be considered as invalid also for non-GPRS services
		   until switching off or the SIM/USIM is removed */
        /*if ( MM_STATUS_ATTACHED == g_MmSubLyrShare.MmShare.ucCsAttachState )*/
        {
            g_MmGlobalInfo.ucProc       = MM_GMM_ATTACH_PROC;
            /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                      */
            /*" \nMM:MmServiceState is MM_GMM_ATTACH_PROC \r " );               */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmAttachCauseHandle:INFO: MmServiceState is MM_GMM_ATTACH_PROC");
            g_MmGlobalInfo.usCauseVal   = usCause;
            g_MmGlobalInfo.ucMmServiceState = MM_NO_IMSI;                       /* ���÷���״̬                             */
            /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                      */
            /* " \nMM:MmServiceState is MM_NO_IMSI \r " );                      */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmAttachCauseHandle:INFO: MmServiceState is MM_NO_IMSI");
            /* Modified by libin 05-03-22 begin: NAS_IT_BUG_017 */
            ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
            PS_MEM_CPY( &OldLai,
                         &g_MmGlobalInfo.MsCsInfo.OldLai,
                         sizeof( MM_LAI_STRU ));
            NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_PLMN_NOT_ALLOWED);          /* ���ø���״̬                             */
            NAS_MML_SetSimCsRegStatus(VOS_FALSE);
            Mm_ComNasInfoSav(
                             MM_STATUS_DETACHED,
                             MM_CKSN_INVALID
                             );
            Mm_ComDelLai();                                                     /* ɾ��LAI                                  */
            g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                                ~MM_MS_ID_TMSI_PRESENT;

            NAS_MML_InitUeIdTmsiInvalid();

            g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                                    ~MM_MS_ID_IMSI_PRESENT;

           Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);            /* ����SIM������״̬                        */

            /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);  */                   /* ����SIM������״̬                        */
            if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
            {
                Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
            }
            else
            {
                Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
            }

            ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
            PS_MEM_CPY( &TempLai,
                         &g_MmGlobalInfo.MsCsInfo.OldLai,
                         sizeof( MM_LAI_STRU ));
            NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
            PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                         &OldLai,
                         sizeof( MM_LAI_STRU ));
            NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
            PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                         &TempLai,
                         sizeof( MM_LAI_STRU ));
            /* Modified by libin 05-03-22 end */
            Mm_ComSetMmState(MM_IDLE_NO_IMSI);
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComGmmAttachCauseHandle:NORMAL: STATUS is MM_IDLE_NO_IMSI");
            Mm_ComProcCauseClear();
        }
        break;
    case NAS_MML_REG_FAIL_CAUSE_GPRS_SERV_AND_NON_GPRS_SERV_NOT_ALLOW:
        g_MmGlobalInfo.ucProc       = MM_GMM_ATTACH_PROC;
        /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                          */
        /*" \nMM:MmServiceState is MM_GMM_ATTACH_PROC \r " );                   */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmAttachCauseHandle:INFO: MmServiceState is MM_GMM_ATTACH_PROC");
        g_MmGlobalInfo.usCauseVal   = usCause;
        g_MmGlobalInfo.ucMmServiceState = MM_NO_IMSI;                           /* ���÷���״̬                             */
        /*WUEPS_TRACE( MM_LOG_LEVEL_6, " \nMM:MmServiceState is MM_NO_IMSI \r " );  */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmAttachCauseHandle:INFO: MmServiceState is MM_NO_IMSI");
        /* Modified by libin 05-03-22 begin: NAS_IT_BUG_017 */
        ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
        PS_MEM_CPY( &OldLai,
                     &g_MmGlobalInfo.MsCsInfo.OldLai,
                     sizeof( MM_LAI_STRU ));
        NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_PLMN_NOT_ALLOWED);              /* ���ø���״̬                             */
        NAS_MML_SetSimCsRegStatus(VOS_FALSE);
        Mm_ComNasInfoSav(MM_STATUS_DETACHED, MM_CKSN_INVALID);
        Mm_ComDelLai();                                                         /* ɾ��LAI                                  */
        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &= ~MM_MS_ID_TMSI_PRESENT;

        NAS_MML_InitUeIdTmsiInvalid();

        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                                ~MM_MS_ID_IMSI_PRESENT;

        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ����SIM������״̬                        */

        /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID); */                    /* ����SIM������״̬                        */
        if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
        }
        else
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
        }

        ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
        PS_MEM_CPY( &TempLai,
                     &g_MmGlobalInfo.MsCsInfo.OldLai,
                     sizeof( MM_LAI_STRU ));
        NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
        PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                     &OldLai,
                     sizeof( MM_LAI_STRU ));
        NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
        PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                     &TempLai,
                     sizeof( MM_LAI_STRU ));
        /* Modified by libin 05-03-22 end */
        Mm_ComSetMmState(MM_IDLE_NO_IMSI);
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComGmmAttachCauseHandle:NORMAL: STATUS is MM_IDLE_NO_IMSI");
        Mm_ComProcCauseClear();
        break;
    case NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL:
        /* BEGIN: Modified by liurui id:40632, 2007/3/5   PN:A32D09094 for 9.4.2.3*/
        /*if ( MM_STATUS_ATTACHED == g_MmSubLyrShare.MmShare.ucCsAttachState )*/
        /* END:   Modified by liurui id:40632, 2007/3/5   PN:A32D09094 */
        {
            g_MmGlobalInfo.ucProc       = MM_GMM_ATTACH_PROC;
            /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                      */
            /* " \nMM:MmServiceState is MM_GMM_ATTACH_PROC \r " );              */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmAttachCauseHandle:INFO: MmServiceState is MM_GMM_ATTACH_PROC");
            g_MmGlobalInfo.usCauseVal   = usCause;
            g_MmGlobalInfo.ucMmServiceState = MM_LIMITED_SERVICE;               /* ���÷���״̬                             */
            /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                      */
            /*" \nMM:MmServiceState is MM_LIMITED_SERVICE \r " );               */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmAttachCauseHandle:INFO: MmServiceState is MM_LIMITED_SERVICE");
            /* Modified by libin 05-03-22 begin: NAS_IT_BUG_017 */
            ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
            PS_MEM_CPY( &OldLai,
                         &g_MmGlobalInfo.MsCsInfo.OldLai,
                         sizeof( MM_LAI_STRU ));
            g_MmGlobalInfo.LuInfo.ucLuAttmptCnt = MM_CONST_NUM_0;               /* ����LU COUNTERΪ0                        */
            NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_LOCATION_AREA_NOT_ALLOWED);          /* ���ø���״̬                             */
            Mm_ComNasInfoSav(
                            MM_STATUS_DETACHED,
                            MM_CKSN_INVALID
                            );                                                  /* ����MM��Ϣ                               */
            Mm_ComDelLai();                                                     /* ɾ��LAI                                  */
            g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                                ~MM_MS_ID_TMSI_PRESENT;         /* ����TMIS������                           */

            NAS_MM_UpdateEquPlmnInfo(usCause);

            NAS_MML_InitUeIdTmsiInvalid();

            Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);            /* ����SIM������״̬                        */

            /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);*/                     /* ����SIM������״̬                        */
            if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
            {
                Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
            }
            else
            {
                Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
            }

            ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
            PS_MEM_CPY( &TempLai,
                         &g_MmGlobalInfo.MsCsInfo.OldLai,
                         sizeof( MM_LAI_STRU ));
            NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
            PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                         &OldLai,
                         sizeof( MM_LAI_STRU ));
            NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
            PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                         &TempLai,
                         sizeof( MM_LAI_STRU ));
            /* Modified by libin 05-03-22 end */
            Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComGmmAttachCauseHandle:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
            Mm_ComProcCauseClear();

            if (NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW == usCause)
            {
                Mm_TimerStop(MM_TIMER_T3212);
            }
        }
        break;
    default:
        break;
    }
}

VOS_VOID Mm_ComGmmRauCauseHandle(VOS_UINT16 usCause)
{
    /* Added by libin 05-03-22 begin: NAS_IT_BUG_017 */
    VOS_UINT8                               ucOldUpdateSts;
    VOS_UINT8                               ucTempUpdateSts;
    MM_LAI_STRU                         OldLai;
    MM_LAI_STRU                         TempLai;
    /* Added by libin 05-03-22 end */

    switch(usCause)
    {
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_MS:
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME:
		/* �ο�24.008��
		   If the MS is IMSI attached, the MS shall in addition
		   set the update status to U3 ROAMING NOT ALLOWED,
		   shall delete any TMSI, LAI and ciphering key sequence number.
		   If the MS is operating in MS operation mode A and an RR connection exists,
		   the MS shall abort the RR connection, unless an emergency call is ongoing.
		   The SIM/USIM shall be considered as invalid also for non-GPRS services
		   until switching off or the SIM/USIM is removed */
        /*if ( MM_STATUS_ATTACHED == g_MmSubLyrShare.MmShare.ucCsAttachState )*/
        {
            g_MmGlobalInfo.ucProc       = MM_GMM_RAU_PROC;
            /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                      */
            /*" \nMM:MmServiceState is MM_GMM_RAU_PROC \r " );                  */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmRauCauseHandle:INFO: MmServiceState is MM_GMM_RAU_PROC");
            g_MmGlobalInfo.usCauseVal   = usCause;
            g_MmGlobalInfo.usCauseVal   = usCause;                                /* ��¼ԭ��ֵ                               */
            g_MmGlobalInfo.ucMmServiceState = MM_NO_IMSI;                       /* ���÷���״̬                             */
            /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                      */
            /*" \nMM:MmServiceState is MM_NO_IMSI \r " );                       */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmRauCauseHandle:INFO: MmServiceState is MM_NO_IMSI");
            /* Modified by libin 05-03-22 begin: NAS_IT_BUG_017 */
            ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
            PS_MEM_CPY( &OldLai,
                         &g_MmGlobalInfo.MsCsInfo.OldLai,
                         sizeof( MM_LAI_STRU ));
            NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_PLMN_NOT_ALLOWED);          /* ���ø���״̬                             */

            Mm_ComNasInfoSav(
                             MM_STATUS_DETACHED,
                             MM_CKSN_INVALID
                             );
            Mm_ComDelLai();                                                     /* ɾ��LAI                                  */
            g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                                ~MM_MS_ID_TMSI_PRESENT;

            NAS_MML_InitUeIdTmsiInvalid();

            g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                                    ~MM_MS_ID_IMSI_PRESENT;

            Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);            /* ����SIM������״̬                        */

            /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);   */                  /* ����SIM������״̬                        */
            if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
            {
                Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
            }
            else
            {
                Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
            }

            ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
            PS_MEM_CPY( &TempLai,
                         &g_MmGlobalInfo.MsCsInfo.OldLai,
                         sizeof( MM_LAI_STRU ));
            NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
            PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                         &OldLai,
                         sizeof( MM_LAI_STRU ));
            NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
            PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                         &TempLai,
                         sizeof( MM_LAI_STRU ));
            /* Modified by libin 05-03-22 end */

            NAS_MML_SetSimCsRegStatus(VOS_FALSE);

            Mm_ComSetMmState(MM_IDLE_NO_IMSI);
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComGmmRauCauseHandle:NORMAL: STATUS is MM_IDLE_NO_IMSI");
            Mm_ComProcCauseClear();
        }
        break;
    case NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW:
        if (VOS_TRUE == NAS_MML_GetCsAttachAllowFlg())
        {
            g_MmGlobalInfo.ucProc       = MM_GMM_RAU_PROC;
            /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                      */
            /*" \nMM:MmServiceState is MM_GMM_RAU_PROC \r " );                  */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmRauCauseHandle:INFO: MmServiceState is MM_GMM_RAU_PROC");
            g_MmGlobalInfo.usCauseVal   = usCause;
            g_MmGlobalInfo.ucMmServiceState = MM_LIMITED_SERVICE;               /* ���÷���״̬                             */
            /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                      */
            /* " \nMM:MmServiceState is MM_LIMITED_SERVICE \r " );              */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmRauCauseHandle:INFO: MmServiceState is MM_LIMITED_SERVICE");
            /* Modified by libin 05-03-22 begin: NAS_IT_BUG_017 */
            ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
            PS_MEM_CPY( &OldLai,
                         &g_MmGlobalInfo.MsCsInfo.OldLai,
                         sizeof( MM_LAI_STRU ));
            g_MmGlobalInfo.LuInfo.ucLuAttmptCnt = MM_CONST_NUM_0;               /* ����LU COUNTERΪ0                        */
            NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_LOCATION_AREA_NOT_ALLOWED);          /* ���ø���״̬                             */
            Mm_ComNasInfoSav(
                            MM_STATUS_DETACHED,
                            MM_CKSN_INVALID
                            );                                                  /* ����MM��Ϣ                               */
            Mm_ComDelLai();                                                     /* ɾ��LAI                                  */
            g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                                ~MM_MS_ID_TMSI_PRESENT;         /* ����TMIS������                           */

            NAS_MM_UpdateEquPlmnInfo(usCause);

            NAS_MML_InitUeIdTmsiInvalid();

            Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);            /* ����SIM������״̬                        */

            /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);  */                   /* ����SIM������״̬                        */
            if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
            {
                Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
            }
            else
            {
                Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
            }

            ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
            PS_MEM_CPY( &TempLai,
                         &g_MmGlobalInfo.MsCsInfo.OldLai,
                         sizeof( MM_LAI_STRU ));
            NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
            PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                         &OldLai,
                         sizeof( MM_LAI_STRU ));
            NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
            PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                         &TempLai,
                         sizeof( MM_LAI_STRU ));
            /* Modified by libin 05-03-22 end */
            Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComGmmRauCauseHandle:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
            Mm_ComProcCauseClear();

            if (NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW == usCause)
            {
                Mm_TimerStop(MM_TIMER_T3212);
            }
        }
        break;
    case NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL:
        /* BEGIN: Modified by liurui id:40632, 2007/01/26   PN:A32D08577 GCF for 9.4.2.4.2 */
        /*if ( MM_STATUS_ATTACHED == g_MmSubLyrShare.MmShare.ucCsAttachState )*/
        /* END:   Modified by liurui id:40632, 2007/01/26   PN:A32D08577 */
        {
            g_MmGlobalInfo.ucProc       = MM_GMM_RAU_PROC;
            /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                      */
            /*" \nMM:MmServiceState is MM_GMM_RAU_PROC \r " );                  */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmRauCauseHandle:INFO: MmServiceState is MM_GMM_RAU_PROC");
            g_MmGlobalInfo.usCauseVal   = usCause;
            g_MmGlobalInfo.ucMmServiceState = MM_LIMITED_SERVICE;               /* ���÷���״̬                             */
            /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                      */
            /*" \nMM:MmServiceState is MM_LIMITED_SERVICE \r " );               */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmRauCauseHandle:INFO: MmServiceState is MM_LIMITED_SERVICE");
            /* Modified by libin 05-03-22 begin: NAS_IT_BUG_017 */
            ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
            g_MmGlobalInfo.LuInfo.ucLuAttmptCnt = MM_CONST_NUM_0;               /* ����LU COUNTERΪ0                        */
            NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_LOCATION_AREA_NOT_ALLOWED);          /* ���ø���״̬                             */
            Mm_ComNasInfoSav(
                            MM_STATUS_DETACHED,
                            NAS_MML_GetSimCsSecurityCksn()
                            );                                                  /* ����MM��Ϣ                               */


            NAS_MM_UpdateEquPlmnInfo(usCause);


            Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);            /* ����SIM������״̬                        */

            ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
            NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
            NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
            /* Modified by libin 05-03-22 end */
            Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComGmmRauCauseHandle:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
            Mm_ComProcCauseClear();
        }
        break;
    default:
        break;
    }
}

VOS_VOID Mm_ComGmmSrCauseHandle(VOS_UINT16 usCause)
{
    /* Added by libin 05-03-22 begin: NAS_IT_BUG_017 */
    VOS_UINT8                               ucOldUpdateSts;
    VOS_UINT8                               ucTempUpdateSts;
    MM_LAI_STRU                         OldLai;
    MM_LAI_STRU                         TempLai;
    /* Added by libin 05-03-22 end */

    switch(usCause)
    {
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_MS:
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME:
        g_MmGlobalInfo.ucProc       = MM_GMM_SR_PROC;
        /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                          */
        /*" \nMM:MmServiceState is MM_GMM_SR_PROC \r " );                       */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmSrCauseHandle:INFO: MmServiceState is MM_GMM_SR_PROC");
        g_MmGlobalInfo.usCauseVal   = usCause;
        g_MmGlobalInfo.ucMmServiceState = MM_NO_IMSI;                           /* ���÷���״̬                             */
        /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                          */
        /*" \nMM:MmServiceState is MM_NO_IMSI \r " );                           */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmSrCauseHandle:INFO: MmServiceState is MM_NO_IMSI");
        /* Modified by libin 05-03-22 begin: NAS_IT_BUG_017 */
        ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
        PS_MEM_CPY( &OldLai,
                     &g_MmGlobalInfo.MsCsInfo.OldLai,
                     sizeof( MM_LAI_STRU ));
        NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_PLMN_NOT_ALLOWED);              /* ���ø���״̬                             */

        NAS_MML_SetSimCsRegStatus(VOS_FALSE);
        Mm_ComNasInfoSav(
                         MM_STATUS_DETACHED,
                         MM_CKSN_INVALID
                         );                                                     /* ��RRC����RRMM_NAS_INFO_CHANGE_REQ        */
        Mm_ComDelLai();                                                         /* ɾ��LAI                                  */
        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                            ~MM_MS_ID_TMSI_PRESENT;             /* ����TMSI������                           */

        NAS_MML_InitUeIdTmsiInvalid();

        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                                ~MM_MS_ID_IMSI_PRESENT;         /* ����IMSI������                           */

        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ����SIM������״̬                        */
        /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);*/                     /* ����SIM������״̬                        */
        if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
        }
        else
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
        }

        /* Deleted by libin for MM_Review_HW_BUG_033 20050328 begin */
        /* Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID); */          /* ����SIM������״̬                        */
        /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID); */               /* ����SIM������״̬                        */
        /* Deleted by libin for MM_Review_HW_BUG_033 20050328 end */
        ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
        PS_MEM_CPY( &TempLai,
                     &g_MmGlobalInfo.MsCsInfo.OldLai,
                     sizeof( MM_LAI_STRU ));
        NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
        PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                     &OldLai,
                     sizeof( MM_LAI_STRU ));
        NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
        PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                     &TempLai,
                     sizeof( MM_LAI_STRU ));
        /* Modified by libin 05-03-22 end */
        Mm_ComSetMmState(MM_IDLE_NO_IMSI);
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComGmmSrCauseHandle:NORMAL: STATUS is MM_IDLE_NO_IMSI");
        Mm_ComProcCauseClear();
        break;
    case NAS_MML_REG_FAIL_CAUSE_GPRS_SERV_NOT_ALLOW:
        if ((VOS_TRUE == NAS_MML_GetPsAttachAllowFlg())
         && (MM_NET_MODE_I == g_MmGlobalInfo.ucNtMod))
        {                                                                       /* ���ΪA+Iģʽ                            */
            if ( (MM_FALSE == g_MmGlobalInfo.ucLikeB)
             && ((MM_IDLE_NORMAL_SERVICE == g_MmGlobalInfo.ucState)
                || ((MM_IDLE_ATTEMPTING_TO_UPDATE == g_MmGlobalInfo.ucState)
                   && (MM_TIMER_STOP == gstMmTimer[MM_TIMER_T3211].ucTimerStatus))))
               
            {
                g_MmGlobalInfo.ucLikeB = MM_TRUE;                               /* ��������Bģʽ                            */
                /*WUEPS_TRACE( MM_LOG_LEVEL_6, " \nMM:MmLikeB is MM_TRUE \r " );*/
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmSrCauseHandle:INFO: MmLikeB is MM_TRUE");
                Mm_TimerStart(MM_TIMER_T3212);                                  /* ����T3212                                */
            }
        }
        break;
    case NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW:
        g_MmGlobalInfo.ucProc       = MM_GMM_SR_PROC;
        /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                          */
        /*" \nMM:MmServiceState is MM_GMM_SR_PROC \r " );                       */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmSrCauseHandle:INFO: MmServiceState is MM_GMM_SR_PROC");
        g_MmGlobalInfo.usCauseVal   = usCause;
        g_MmGlobalInfo.ucMmServiceState = MM_LIMITED_SERVICE;                   /* ���÷���״̬                             */
        /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                          */
        /*" \nMM:MmServiceState is MM_LIMITED_SERVICE \r " );                   */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmSrCauseHandle:INFO: MmServiceState is MM_LIMITED_SERVICE");
        /* Modified by libin 05-03-22 begin: NAS_IT_BUG_017 */
        ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
        PS_MEM_CPY( &OldLai,
                     &g_MmGlobalInfo.MsCsInfo.OldLai,
                     sizeof( MM_LAI_STRU ));
        NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_LOCATION_AREA_NOT_ALLOWED);              /* ���ø���״̬                             */
        Mm_ComNasInfoSav(
                        MM_STATUS_DETACHED,
                        MM_CKSN_INVALID
                        );                                                      /* ����MM��Ϣ                               */
        Mm_ComDelLai();                                                         /* ɾ��LAI                                  */
        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                            ~MM_MS_ID_TMSI_PRESENT;             /* ����TMIS������                           */

        NAS_MML_InitUeIdTmsiInvalid();

        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ����SIM������״̬                        */

        /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID); */                    /* ����SIM������״̬                        */
        if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
        }
        else
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
        }

        ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
        PS_MEM_CPY( &TempLai,
                     &g_MmGlobalInfo.MsCsInfo.OldLai,
                     sizeof( MM_LAI_STRU ));
        NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
        PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                     &OldLai,
                     sizeof( MM_LAI_STRU ));
        NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
        PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                     &TempLai,
                     sizeof( MM_LAI_STRU ));
        /* Modified by libin 05-03-22 end */
        Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComGmmSrCauseHandle:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
        Mm_ComProcCauseClear();

        Mm_TimerStop(MM_TIMER_T3212);

        break;
    case NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW:
        if ( MM_STATUS_ATTACHED == g_MmSubLyrShare.MmShare.ucCsAttachState )
        {
            g_MmGlobalInfo.ucProc       = MM_GMM_SR_PROC;
            /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                      */
            /*" \nMM:MmServiceState is MM_GMM_SR_PROC \r " );                   */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmSrCauseHandle:INFO: MmServiceState is MM_GMM_SR_PROC");
            g_MmGlobalInfo.usCauseVal   = usCause;
            g_MmGlobalInfo.ucMmServiceState = MM_LIMITED_SERVICE;               /* ���÷���״̬                             */
            /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                      */
            /*" \nMM:MmServiceState is MM_LIMITED_SERVICE \r " );               */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmSrCauseHandle:INFO: MmServiceState is MM_LIMITED_SERVICE");
            /* Modified by libin 05-03-22 begin: NAS_IT_BUG_017 */
            ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
            PS_MEM_CPY( &OldLai,
                         &g_MmGlobalInfo.MsCsInfo.OldLai,
                         sizeof( MM_LAI_STRU ));
            g_MmGlobalInfo.LuInfo.ucLuAttmptCnt = MM_CONST_NUM_0;               /* ����LU COUNTERΪ0                        */
            NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_LOCATION_AREA_NOT_ALLOWED);          /* ���ø���״̬                             */
            Mm_ComNasInfoSav(
                            MM_STATUS_DETACHED,
                            MM_CKSN_INVALID
                            );                                                  /* ����MM��Ϣ                               */
            Mm_ComDelLai();                                                     /* ɾ��LAI                                  */
            g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                                ~MM_MS_ID_TMSI_PRESENT;         /* ����TMIS������                           */

            NAS_MML_InitUeIdTmsiInvalid();

            Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);            /* ����SIM������״̬                        */

            /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);*/                     /* ����SIM������״̬                        */
            if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
            {
                Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
            }
            else
            {
                Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
            }

            ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
            PS_MEM_CPY( &TempLai,
                         &g_MmGlobalInfo.MsCsInfo.OldLai,
                         sizeof( MM_LAI_STRU ));
            NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
            PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                         &OldLai,
                         sizeof( MM_LAI_STRU ));
            NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
            PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                         &TempLai,
                         sizeof( MM_LAI_STRU ));
            /* Modified by libin 05-03-22 end */
            Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComGmmSrCauseHandle:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
            Mm_ComProcCauseClear();
        }
        break;
    case NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL:
        /* Modified by libin 05-04-14 begin: NAS_IT_BUG_055 */
        if ( MM_STATUS_ATTACHED == g_MmSubLyrShare.MmShare.ucCsAttachState )
        {
            g_MmGlobalInfo.ucProc       = MM_GMM_SR_PROC;
            /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                      */
            /* " \nMM:MmServiceState is MM_GMM_SR_PROC \r " );                  */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmSrCauseHandle:INFO: MmServiceState is MM_GMM_SR_PROC");
            g_MmGlobalInfo.usCauseVal   = usCause;
            g_MmGlobalInfo.ucMmServiceState = MM_LIMITED_SERVICE;               /* ���÷���״̬                             */
            /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                      */
            /* " \nMM:MmServiceState is MM_LIMITED_SERVICE \r " );              */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComGmmSrCauseHandle:INFO: MmServiceState is MM_LIMITED_SERVICE");
            /* Modified by libin 05-03-22 begin: NAS_IT_BUG_017 */
            ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
            g_MmGlobalInfo.LuInfo.ucLuAttmptCnt = MM_CONST_NUM_0;               /* ����LU COUNTERΪ0                        */
            NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_LOCATION_AREA_NOT_ALLOWED);          /* ���ø���״̬                             */
            Mm_ComNasInfoSav(
                            MM_STATUS_DETACHED,
                            NAS_MML_GetSimCsSecurityCksn()
                            );                                                  /* ����MM��Ϣ                               */

            Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);            /* ����SIM������״̬                        */

            ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
            NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
            NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
            /* Modified by libin 05-03-22 end */
            Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComGmmSrCauseHandle:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
            Mm_ComProcCauseClear();
        }
        /* Modified by libin 05-04-14 end */
        break;
    default:
        break;
    }
}



VOS_VOID Mm_ComNetDetachCauseHandle(VOS_UINT16 usCause)
{
    /* Added by libin 05-03-22 begin: NAS_IT_BUG_017 */
    VOS_UINT8                               ucOldUpdateSts;
    VOS_UINT8                               ucTempUpdateSts;
    MM_LAI_STRU                         OldLai;
    MM_LAI_STRU                         TempLai;
    /* Added by libin 05-03-22 end */

    switch(usCause)
    {
    case NAS_MML_REG_FAIL_CAUSE_IMSI_UNKNOWN_IN_HLR:
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_MS:
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME:
    case NAS_MML_REG_FAIL_CAUSE_GPRS_SERV_AND_NON_GPRS_SERV_NOT_ALLOW:
        g_MmGlobalInfo.ucProc       = MM_NET_DETACH_PROC;
        /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                          */
        /*" \nMM:MmServiceState is MM_NET_DETACH_PROC \r " );                   */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComNetDetachCauseHandle:INFO: MmServiceState is MM_NET_DETACH_PROC");
        g_MmGlobalInfo.usCauseVal   = usCause;
        g_MmGlobalInfo.ucMmServiceState = MM_NO_IMSI;                           /* ���÷���״̬                             */
        /*WUEPS_TRACE( MM_LOG_LEVEL_6, " \nMM:MmServiceState is MM_NO_IMSI \r " );  */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComNetDetachCauseHandle:INFO: MmServiceState is MM_NO_IMSI");
        /* Modified by libin 05-03-22 begin: NAS_IT_BUG_017 */
        ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
        PS_MEM_CPY( &OldLai,
                     &g_MmGlobalInfo.MsCsInfo.OldLai,
                     sizeof( MM_LAI_STRU ));
        NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_PLMN_NOT_ALLOWED);              /* ���ø���״̬                             */

        NAS_MML_SetSimCsRegStatus(VOS_FALSE);
        Mm_ComNasInfoSav(
                         MM_STATUS_DETACHED,
                         MM_CKSN_INVALID
                         );                                                     /* ��RRC����RRMM_NAS_INFO_CHANGE_REQ        */
        Mm_ComDelLai();                                                         /* ɾ��LAI                                  */
        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                            ~MM_MS_ID_TMSI_PRESENT;             /* ����TMSI������                           */

        NAS_MML_InitUeIdTmsiInvalid();

        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                                ~MM_MS_ID_IMSI_PRESENT;         /* ����IMSI������                           */

        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ����SIM������״̬                        */

        /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID); */                    /* ����SIM������״̬                        */
        if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
        }
        else
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
        }

        ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
        PS_MEM_CPY( &TempLai,
                     &g_MmGlobalInfo.MsCsInfo.OldLai,
                     sizeof( MM_LAI_STRU ));
        NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
        PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                     &OldLai,
                     sizeof( MM_LAI_STRU ));
        NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
        PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                     &TempLai,
                     sizeof( MM_LAI_STRU ));
        /* Modified by libin 05-03-22 end */
        Mm_ComSetMmState(MM_IDLE_NO_IMSI);
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComNetDetachCauseHandle:NORMAL: STATUS is MM_IDLE_NO_IMSI");
        Mm_ComProcCauseClear();
        break;
    case NAS_MML_REG_FAIL_CAUSE_GPRS_SERV_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_GPRS_SERV_NOT_ALLOW_IN_PLMN:
        if ( (VOS_TRUE == NAS_MML_GetPsAttachAllowFlg())
          && (MM_NET_MODE_I == g_MmGlobalInfo.ucNtMod))
        {                                                                       /* ���ΪA+Iģʽ                            */
            if ( (MM_FALSE == g_MmGlobalInfo.ucLikeB)
             && ((MM_IDLE_NORMAL_SERVICE == g_MmGlobalInfo.ucState)
                ||((MM_IDLE_ATTEMPTING_TO_UPDATE == g_MmGlobalInfo.ucState)
                  && (MM_TIMER_STOP == gstMmTimer[MM_TIMER_T3211].ucTimerStatus))))
            {
                g_MmGlobalInfo.ucLikeB = MM_TRUE;                               /* ����������Bģʽ                          */
                /*WUEPS_TRACE( MM_LOG_LEVEL_6, " \nMM:MmLikeB is MM_TRUE \r " );*/
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComNetDetachCauseHandle:INFO: MmLikeB is MM_TRUE");
                Mm_TimerStart(MM_TIMER_T3212);                                  /* ����T3212                                */
            }
        }
        /* Deleted by libin MM_Review_HW_BUG_021 20050303 begin */
        /* g_MmGlobalInfo.ucProc       = MM_NET_DETACH_PROC;    */
        /* g_MmGlobalInfo.usCauseVal   = ucCause;               */
        /* g_MmGlobalInfo.ucMmServiceState = MM_NORMAL_SERVICE; */              /* ���÷���״̬                             */
        /* Deleted by libin MM_Review_HW_BUG_021 20050303 end */
        break;
    case NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW:
        g_MmGlobalInfo.ucProc       = MM_NET_DETACH_PROC;
        /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                          */
        /*" \nMM:MmServiceState is MM_NET_DETACH_PROC \r " );                   */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComNetDetachCauseHandle:INFO: MmServiceState is MM_NET_DETACH_PROC");
        g_MmGlobalInfo.usCauseVal   = usCause;
        g_MmGlobalInfo.ucMmServiceState = MM_LIMITED_SERVICE;                   /* ���÷���״̬                             */
        /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                          */
        /*" \nMM:MmServiceState is MM_LIMITED_SERVICE \r " );                   */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComNetDetachCauseHandle:INFO: MmServiceState is MM_LIMITED_SERVICE");
        /* Modified by libin 05-03-22 begin: NAS_IT_BUG_017 */
        ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
        PS_MEM_CPY( &OldLai,
                     &g_MmGlobalInfo.MsCsInfo.OldLai,
                     sizeof( MM_LAI_STRU ));
        NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_LOCATION_AREA_NOT_ALLOWED);              /* ���ø���״̬                             */
        Mm_ComNasInfoSav(
                        MM_STATUS_DETACHED,
                        MM_CKSN_INVALID
                        );                                                      /* ����MM��Ϣ                               */
        Mm_ComDelLai();                                                         /* ɾ��LAI                                  */
        g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                            ~MM_MS_ID_TMSI_PRESENT;             /* ����TMIS������                           */

        NAS_MML_InitUeIdTmsiInvalid();

        Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                /* ����SIM������״̬                        */

        /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID); */                    /* ����SIM������״̬                        */
        if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
        }
        else
        {
            Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
        }

        ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
        PS_MEM_CPY( &TempLai,
                     &g_MmGlobalInfo.MsCsInfo.OldLai,
                     sizeof( MM_LAI_STRU ));
        NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
        PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                     &OldLai,
                     sizeof( MM_LAI_STRU ));
        NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
        PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                     &TempLai,
                     sizeof( MM_LAI_STRU ));
        /* Modified by libin 05-03-22 end */
        Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComNetDetachCauseHandle:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
        Mm_ComProcCauseClear();
        Mm_TimerStop(MM_TIMER_T3212);
        break;
    case NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL:
        if ( MM_STATUS_ATTACHED == g_MmSubLyrShare.MmShare.ucCsAttachState )
        {
            g_MmGlobalInfo.ucProc       = MM_NET_DETACH_PROC;
            /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                      */
            /*" \nMM:MmServiceState is MM_NET_DETACH_PROC \r " );               */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComNetDetachCauseHandle:INFO: MmServiceState is MM_NET_DETACH_PROC");
            g_MmGlobalInfo.usCauseVal   = usCause;
            g_MmGlobalInfo.ucMmServiceState = MM_LIMITED_SERVICE;               /* ���÷���״̬                             */
            /*WUEPS_TRACE( MM_LOG_LEVEL_6,                                      */
            /*" \nMM:MmServiceState is MM_LIMITED_SERVICE \r " );               */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComNetDetachCauseHandle:INFO: MmServiceState is MM_LIMITED_SERVICE");
            /* Modified by libin 05-03-22 begin: NAS_IT_BUG_017 */
            ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
            PS_MEM_CPY( &OldLai,
                         &g_MmGlobalInfo.MsCsInfo.OldLai,
                         sizeof( MM_LAI_STRU ));
            g_MmGlobalInfo.LuInfo.ucLuAttmptCnt = MM_CONST_NUM_0;               /* ����LU COUNTERΪ0                        */
            NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_LOCATION_AREA_NOT_ALLOWED);          /* ���ø���״̬                             */
            Mm_ComNasInfoSav(
                            MM_STATUS_DETACHED,
                            MM_CKSN_INVALID
                            );                                                  /* ����MM��Ϣ                               */
            Mm_ComDelLai();                                                     /* ɾ��LAI                                  */
            g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                                ~MM_MS_ID_TMSI_PRESENT;         /* ����TMIS������                           */

            NAS_MML_InitUeIdTmsiInvalid();

            Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);            /* ����SIM������״̬                        */

            /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);  */                   /* ����SIM������״̬                        */
            if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
            {
                Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
            }
            else
            {
                Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
            }

            ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
            PS_MEM_CPY( &TempLai,
                         &g_MmGlobalInfo.MsCsInfo.OldLai,
                         sizeof( MM_LAI_STRU ));
            NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
            PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                         &OldLai,
                         sizeof( MM_LAI_STRU ));
           NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
            PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                         &TempLai,
                         sizeof( MM_LAI_STRU ));
            /* Modified by libin 05-03-22 end */
            Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComNetDetachCauseHandle:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
            Mm_ComProcCauseClear();
        }
        break;
    case NAS_MML_REG_FAIL_CAUSE_NULL:

        if ((MM_IDLE_NORMAL_SERVICE == g_MmGlobalInfo.ucState)
                || ((MM_IDLE_ATTEMPTING_TO_UPDATE == g_MmGlobalInfo.ucState)
                   && (MM_TIMER_STOP == gstMmTimer[MM_TIMER_T3211].ucTimerStatus)))
        {
            Mm_TimerStart(MM_TIMER_T3212);                                          /* ����T3212                                */
        }

        g_MmGlobalInfo.ucLikeB = MM_TRUE;
        /*WUEPS_TRACE( MM_LOG_LEVEL_6, " \nMM:MmLikeB is MM_TRUE \r " );        */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComNetDetachCauseHandle:INFO: MmLikeB is MM_TRUE");
        break;
    default:
        break;
    }
}

/***********************************************************************
 *  MODULE   : Mm_ComCauseHandleAllProc
 *  FUNCTION : MM��ԭ��ֵ�Ĵ���
 *  INPUT    : VOS_UINT8   ucProcType                ��ǰ���̵�����
 *             VOS_UINT8   ucCause                   �����յ���ԭ��ֵ
 *  OUTPUT   : VOS_VOID
 *  RETURN   : VOS_VOID
 *  NOTE     :
 *  HISTORY  :
 *     1.  ��־��    04-03-10  �°�����,�������
 *     2.
 ************************************************************************/

VOS_VOID Mm_ComCauseHandleAllProc(
                              VOS_UINT8   ucProcType,
                              VOS_UINT16  usCause
                              )
{
    switch(ucProcType)
    {
    case MM_LU_PROC:
        Mm_ComLuCauseHandle(usCause);
        break;
    case MM_GMM_ATTACH_PROC:
        Mm_ComGmmAttachCauseHandle(usCause);
        break;
    case MM_GMM_RAU_PROC:
        Mm_ComGmmRauCauseHandle(usCause);
        break;
    case MM_GMM_SR_PROC:
        Mm_ComGmmSrCauseHandle(usCause);
        break;
    case MM_NET_DETACH_PROC:
        Mm_ComNetDetachCauseHandle(usCause);
        break;
    default:
        break;
    }
}

VOS_VOID NAS_MM_DelLaiTmsiCK(VOS_VOID)
{
    /* ɾ��ȫ�ֱ����е�LAI */
    Mm_ComDelLai();

    /* ɾ��ȫ�ֱ����е�TMSI */
    g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg              &= ~MM_MS_ID_TMSI_PRESENT;
    NAS_MML_InitUeIdTmsiInvalid();

    /* ɾ��SIM���е�LAI��IMSI */
    NAS_MM_WriteLocaInCard();

    NAS_MML_SetSimCsSecurityCksn(MM_CKSN_INVALID);

    /* ����SIM�����;�����ǰ��Ҫɾ��KC����CKIK*/
    if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
    {
        NAS_MM_WriteKCInCard();
    }
    else
    {
        NAS_MM_WriteCkIkInCard();
    }
}



VOS_VOID Mm_ComToNoImsiHandle()
{
    NAS_MML_SetCsUpdateStatus(NAS_MML_LOCATION_UPDATE_STATUS_PLMN_NOT_ALLOWED);                  /* ���ø���״̬                             */
    NAS_MML_SetSimCsRegStatus(VOS_FALSE);
    Mm_ComNasInfoSav(MM_STATUS_DETACHED, MM_CKSN_INVALID);
    Mm_ComDelLai();                                                             /* ɾ��LAI                                  */
    g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &= ~MM_MS_ID_TMSI_PRESENT;
    NAS_MML_InitUeIdTmsiInvalid();
    g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg &=
                                            ~MM_MS_ID_IMSI_PRESENT;

    Mm_SndAgentUsimUpdateFileReq(MM_READ_LOCA_INFO_FILE_ID);                    /* ����SIM������״̬                        */

    /* Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);  */                   /* ����SIM������״̬                        */

    if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
    {
        Mm_SndAgentUsimUpdateFileReq(MM_READ_KC_FILE_ID);                     /* ����SIM������״̬                        */
    }
    else
    {
        Mm_SndAgentUsimUpdateFileReq(MM_READ_CKIK_FILE_ID);                     /* ����SIM������״̬                        */
    }
}


VOS_VOID  NAS_MM_RcvSysInfoPsRestrictRegLaiNoChangeU1(VOS_VOID)
{

    /* ���LAI����,����״̬ΪU1                 */
    if ( MM_ATT_NOT_NEED == g_MmGlobalInfo.ucAttFlg )
    {
        Mm_ComNasInfoSav(
                         MM_STATUS_ATTACHED,
                         NAS_MML_GetSimCsSecurityCksn()
                         );

        if ( MM_TRUE == g_MmGlobalInfo.ucPowerOnFlg )
        {
            if (NAS_MML_NET_RAT_TYPE_GSM == NAS_MML_GetCurrNetRatType())
            {
                Mm_SndRrNasInfoChgReq(MM_NAS_INFO_SK_FLG);
            }

            Mm_SndRrNasInfoChgReq(MM_NAS_INFO_LOCA_INFO_FLG);
            g_MmGlobalInfo.ucPowerOnFlg = MM_FALSE;
        }

        g_MmGlobalInfo.ucMmServiceState = MM_NORMAL_SERVICE;/* ���÷���״̬                             */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComRcvSysHandle:INFO: MmServiceState is MM_NORMAL_SERVICE");


        NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                    MM_MMC_LU_RESULT_SUCCESS,
                                    NAS_MML_REG_FAIL_CAUSE_NULL);            /* ��MMC����MMCMM_LU_RESULT_IND             */

        NAS_MM_SndGmmLuResultInd(MMGMM_LU_SUCCESS,
                                 NAS_MML_REG_FAIL_CAUSE_NULL);            /* ��MMC����MMCMM_LU_RESULT_IND             */
        Mm_ComSetMmState(MM_IDLE_NORMAL_SERVICE);

        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComRcvSysHandle:NORMAL: STATUS is MM_IDLE_NORMAL_SERVICE");
        Mm_TimerStart(MM_TIMER_T3212);                      /* ����T3212                                */
    }
    else
    {
        if ( MM_TRUE == g_MmGlobalInfo.ucPowerOnFlg )
        {
            NAS_MM_ProcBufferedEmgCallOrLau_RcvSysinfo(MM_FALSE, MM_IE_LUT_IMSI_ATTACH);
        }
        else
        {
            Mm_ComNasInfoSav(
                     MM_STATUS_ATTACHED,
                     NAS_MML_GetSimCsSecurityCksn()
                     );
            g_MmGlobalInfo.ucMmServiceState = MM_NORMAL_SERVICE;                              /* ���÷���״̬                             */

            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComRcvSysHandle:INFO: MmServiceState is MM_NORMAL_SERVICE");


            NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                        MM_MMC_LU_RESULT_SUCCESS,
                                        NAS_MML_REG_FAIL_CAUSE_NULL);            /* ��MMC����MMCMM_LU_RESULT_IND             */
            NAS_MM_SndGmmLuResultInd(MMGMM_LU_SUCCESS,
                                     NAS_MML_REG_FAIL_CAUSE_NULL);            /* ��MMC����MMCMM_LU_RESULT_IND             */

            Mm_ComSetMmState(MM_IDLE_NORMAL_SERVICE);

            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComRcvSysHandle:NORMAL: STATUS is MM_IDLE_NORMAL_SERVICE");
            Mm_TimerStart(MM_TIMER_T3212);                      /* ����T3212                                */
        }
    }

    return;
}
VOS_VOID    Mm_ComSysInfoHandleWithLaiNoChange(VOS_UINT8     ucUserSpecificSearchFlg)
{
    VOS_UINT32                          ulLaiChangeFlag;

    ulLaiChangeFlag =  Mm_ComLaiJudge();

    if (NAS_MML_LOCATION_UPDATE_STATUS_UPDATED == NAS_MML_GetCsUpdateStatus())
    {
        /* ���LAI����,����״̬ΪU1                 */
        if ( MM_ATT_NOT_NEED == g_MmGlobalInfo.ucAttFlg )
        {
            if ( MM_TRUE ==
                g_MmGlobalInfo.LuInfo.ucT3212ExpiredFlg )
            {
                NAS_MM_ProcBufferedEmgCallOrLau_RcvSysinfo(ulLaiChangeFlag, MM_IE_LUT_PERIODIC_UPDATING);
            }
            else
            {
                Mm_ComNasInfoSav(
                                 MM_STATUS_ATTACHED,
                                 NAS_MML_GetSimCsSecurityCksn()
                                 );

                if ( MM_TRUE == g_MmGlobalInfo.ucPowerOnFlg )
                {
                    if (NAS_MML_NET_RAT_TYPE_GSM == NAS_MML_GetCurrNetRatType())
                    {
                        Mm_SndRrNasInfoChgReq(MM_NAS_INFO_SK_FLG);
                    }

                    Mm_SndRrNasInfoChgReq(MM_NAS_INFO_LOCA_INFO_FLG);
                    g_MmGlobalInfo.ucPowerOnFlg = MM_FALSE;
                }

                g_MmGlobalInfo.ucMmServiceState = MM_NORMAL_SERVICE;/* ���÷���״̬                             */
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComRcvSysHandle:INFO: MmServiceState is MM_NORMAL_SERVICE");


                NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                            MM_MMC_LU_RESULT_SUCCESS,
                                            NAS_MML_REG_FAIL_CAUSE_NULL);            /* ��MMC����MMCMM_LU_RESULT_IND             */

                NAS_MM_SndGmmLuResultInd(MMGMM_LU_SUCCESS,
                                         NAS_MML_REG_FAIL_CAUSE_NULL);            /* ��MMC����MMCMM_LU_RESULT_IND             */

                Mm_ComSetMmState(MM_IDLE_NORMAL_SERVICE);

                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComRcvSysHandle:NORMAL: STATUS is MM_IDLE_NORMAL_SERVICE");
                Mm_TimerStart(MM_TIMER_T3212);                      /* ����T3212                                */
            }
        }
        else
        {
            if ( MM_TRUE == g_MmGlobalInfo.ucPowerOnFlg )
            {
                NAS_MM_ProcBufferedEmgCallOrLau_RcvSysinfo(ulLaiChangeFlag, MM_IE_LUT_IMSI_ATTACH);
            }
            else if ( MM_TRUE ==
                g_MmGlobalInfo.LuInfo.ucT3212ExpiredFlg )
            {
                NAS_MM_ProcBufferedEmgCallOrLau_RcvSysinfo(ulLaiChangeFlag, MM_IE_LUT_PERIODIC_UPDATING);
            }
            /* ���ڷ���LAU��ʱ��ֹͣ��T3212��������T3212û�����У���ʾǰ���Ѿ������ */
            else if ((MM_TIMER_STOP == gstMmTimer[MM_TIMER_T3211].ucTimerStatus)
                  && (MM_TIMER_STOP == gstMmTimer[MM_TIMER_T3212].ucTimerStatus)
                  && (0x0 != gstMmTimer[MM_TIMER_T3212].ulTimerLen))
            {
                NAS_MM_ProcBufferedEmgCallOrLau_RcvSysinfo(ulLaiChangeFlag, MM_IE_LUT_NORMAL_LU);
            }
            else
            {
                Mm_ComNasInfoSav(
                         MM_STATUS_ATTACHED,
                         NAS_MML_GetSimCsSecurityCksn()
                         );
                g_MmGlobalInfo.ucMmServiceState = MM_NORMAL_SERVICE;                              /* ���÷���״̬                             */

                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComRcvSysHandle:INFO: MmServiceState is MM_NORMAL_SERVICE");


                NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                            MM_MMC_LU_RESULT_SUCCESS,
                                            NAS_MML_REG_FAIL_CAUSE_NULL);            /* ��MMC����MMCMM_LU_RESULT_IND             */
                NAS_MM_SndGmmLuResultInd(MMGMM_LU_SUCCESS,
                                         NAS_MML_REG_FAIL_CAUSE_NULL);            /* ��MMC����MMCMM_LU_RESULT_IND             */


                Mm_ComSetMmState(MM_IDLE_NORMAL_SERVICE);

                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComRcvSysHandle:NORMAL: STATUS is MM_IDLE_NORMAL_SERVICE");
                Mm_TimerStart(MM_TIMER_T3212);                      /* ����T3212                                */
            }
        }
    }
    else if ( (1 == ucUserSpecificSearchFlg)                        /* �û������ָ���������ҵ�ǰ����U1����ʹλ�������䣬Ҳ����ע�� */
            ||(MM_TRUE == g_MmGlobalInfo.ucPowerOnFlg))             /* �տ�����λ�������䣬Ҳ����ע�� */
    {
        /* ����״̬�����ֶ�����ʱ�������ATTMPTCNT */            
        if ( MM_IDLE_PLMN_SEARCH == g_MmGlobalInfo.ucState )
        {
            g_MmGlobalInfo.LuInfo.ucLuAttmptCnt = 0;
        }
        
        NAS_MM_ProcBufferedEmgCallOrLau_RcvSysinfo(ulLaiChangeFlag, MM_IE_LUT_NORMAL_LU);
    }
    else
    {   /* MM ״̬Ǩ�ص�����ǰ��״̬ */
        /* �ڷ�U1״̬��������ע�ᶨʱ��û������ʱ,Ҫ����һ��LU���� */
        if ((MM_TIMER_STOP == gstMmTimer[MM_TIMER_T3211].ucTimerStatus)
         && (MM_TIMER_STOP == gstMmTimer[MM_TIMER_T3212].ucTimerStatus)
         && (0x0 != gstMmTimer[MM_TIMER_T3212].ulTimerLen))
        {
            /* ��NOT UPDATED״̬��MMӦ�÷������NORMAL LU */
            g_MmGlobalInfo.LuInfo.ucT3212ExpiredFlg = MM_FALSE;
           NAS_MM_ProcBufferedEmgCallOrLau_RcvSysinfo(ulLaiChangeFlag, MM_IE_LUT_NORMAL_LU);
        }
        else if (MM_IDLE_PLMN_SEARCH == g_MmGlobalInfo.ucState)
        {
            Mm_ComSetMmState(MM_IDLE_ATTEMPTING_TO_UPDATE);

            
            /* ���T3212��������T3211�������У���֪ͨMMC��ǰT3212������,��������ȴ�T3211��ʱע�ᴦ�� */    
            if ( (MM_TIMER_STOP     == gstMmTimer[MM_TIMER_T3211].ucTimerStatus)
              && (MM_TIMER_RUNNING  == gstMmTimer[MM_TIMER_T3212].ucTimerStatus) )
            {
                NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                            MM_MMC_LU_RESULT_FAILURE,
                                            NAS_MML_REG_FAIL_CAUSE_T3212_RUNNING);
            }
            else
            {
                NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                            MM_MMC_LU_RESULT_FAILURE,
                                            NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE);/* ��MMC����MMCMM_LU_RESULT_IND,ʹMMCǨ��״̬ */

            }

            NAS_MM_SndGmmLuResultInd(MMGMM_LU_FAILURE,
                                     NAS_MML_REG_FAIL_CAUSE_NULL);              /* ��MMC����MMCMM_LU_RESULT_IND,ʹMMCǨ��״̬ */
        }
        else
        {

        }
    }
}

VOS_VOID Mm_ComRcvSysHandle(VOS_UINT8     ucUserSpecificSearchFlg)
{
    
    VOS_UINT32                          ulLaiChangeFlag;

    ulLaiChangeFlag =  Mm_ComLaiJudge();

    if ( VOS_FALSE == NAS_MML_GetCsAttachAllowFlg())
    {
        g_MmGlobalInfo.ucNtMod = g_MmGlobalInfo.ucNewNtMod;

        Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);


        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComRcvSysHandle:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
    }
    else
    {
        if ( ( MMCMM_NO_FORBIDDEN !=
            g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg ) &&
            ( MMCMM_FORBIDDEN_PLMN_FOR_GPRS !=
            g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg ) )
        {                                                                       /* ���û��CS���ֹ����Ϣ                   */

            NAS_MM_ProcCsForbidenFlg_RcvSysInfoInd(g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg);
        }
        else
        {
#if (FEATURE_ON == FEATURE_LTE)
            if (VOS_TRUE == NAS_MM_RcvSysInfoSurpportLte_PreProc())
            {
                return;
            }
#endif


            /* ��ǰ״̬��attempt tO updateʱ,����һ��פ����LAI��Ƚ�;
               ��ǰ״̬��limit serviceʱ,���ϴγɹ�פ����LAI��Ƚ� */
            if (MM_IDLE_PLMN_SEARCH == g_MmGlobalInfo.ucState)
            {
                if (NAS_MML_LOCATION_UPDATE_STATUS_NOT_UPDATED == NAS_MML_GetCsUpdateStatus())
                { 
                    /* �����NOT UPDATEʱ�򣬵�ǰLAI����ATTEMPT LAI�У�����LAU */
                    if (VOS_FALSE == NAS_MM_IsSameLaiInfo(NAS_MM_GetAttemptUpdateLaiInfo(),
                                                           &g_MmGlobalInfo.MsCsInfo.CurLai))
                    {
                        NAS_MM_ProcBufferedEmgCallOrLau_RcvSysinfo(MM_TRUE, MM_IE_LUT_NORMAL_LU);
                        return;
                    }
                }
                else if (MM_IDLE_LIMITED_SERVICE == g_MmGlobalInfo.ucNotCampONPreState)
                {
                    if (MM_TRUE == ulLaiChangeFlag)
                    {
                        NAS_MM_ProcBufferedEmgCallOrLau_RcvSysinfo(ulLaiChangeFlag, MM_IE_LUT_NORMAL_LU);
                        return;
                    }
                }
                else
                {
                }                
            }

            if ( g_MmGlobalInfo.ucNtMod == g_MmGlobalInfo.ucNewNtMod )
            {
                if (NAS_MML_LOCATION_UPDATE_STATUS_UPDATED == NAS_MML_GetCsUpdateStatus())
                {
                    if(MM_TRUE == Mm_ComLaiJudge())
                    {
                        Mm_TimerStop(MM_TIMER_T3213);
                        NAS_MM_ProcBufferedEmgCallOrLau_RcvSysinfo(ulLaiChangeFlag, MM_IE_LUT_NORMAL_LU);
                    }
                    else
                    {
                        Mm_ComSysInfoHandleWithLaiNoChange(ucUserSpecificSearchFlg);
                    }
                }
                else if (NAS_MML_LOCATION_UPDATE_STATUS_NOT_UPDATED == NAS_MML_GetCsUpdateStatus()) 
                {
                    /* �����NOT UPDATEʱ�򣬵�ǰLAI����ATTEMPT LAI�У�����LAU */
                    if (VOS_FALSE == NAS_MM_IsSameLaiInfo(NAS_MM_GetAttemptUpdateLaiInfo(),
                                                           &g_MmGlobalInfo.MsCsInfo.CurLai))
                    {
                        NAS_MM_ProcBufferedEmgCallOrLau_RcvSysinfo(MM_TRUE, MM_IE_LUT_NORMAL_LU);
                    }
                    else
                    {
                        /* LA û�����ı� */
                        Mm_ComSysInfoHandleWithLaiNoChange(ucUserSpecificSearchFlg);                        
                    }
                }
                else
                {
                    NAS_MM_ProcBufferedEmgCallOrLau_RcvSysinfo(ulLaiChangeFlag, MM_IE_LUT_NORMAL_LU);
                }
            }
            else
            {
                Mm_ComNetModeChange();
            }
        }
    }
}


/*******************************************************************************
  MODULE   : Mm_ComMmConnExist
  FUNCTION : �ж��Ƿ���MM���Ӵ���
  INPUT    : VOS_VOID
  OUTPUT   : VOS_VOID
  RETURN   : VOS_UINT8             ucResult         �Ƿ���MM���Ӵ��ڵĽ��
  NOTE     :
  HISTORY  :
     1.  ��־��      04-03-11  �°�����
*******************************************************************************/
VOS_UINT8 Mm_ComMmConnExist(
                        VOS_VOID
                    )
{
    VOS_UINT8           ucResult = MM_FALSE;                                        /* �Ƿ���MM���Ӵ��ڵĽ��                   */
    VOS_UINT8           i        = 0;                                               /* ѭ������                                 */

    for ( i = 0; i < MM_CONN_CM_ENTITY_NUM; i++)
    {                                                                           /* ����CMʵ�����                           */
        if ( ( MM_CONST_NUM_0 !=
            g_MmGlobalInfo.ConnCtrlInfo[i].aucMMConnExtFlg[0] )
            || ( MM_CONST_NUM_0 !=
            g_MmGlobalInfo.ConnCtrlInfo[i].aucMMConnExtFlg[1] ) )
        {                                                                       /* �д��ڵ�MM����                           */
            ucResult = MM_TRUE;                                                 /* ����ֵ���ó���MM���Ӵ���                 */
        }
    }

    return ucResult;
}


/*******************************************************************************
  MODULE   : Mm_ComSaveProcAndCauseVal
  FUNCTION : �洢���̺�ԭ��ֵ
  INPUT    : VOS_UINT8             ucProc          ����
             VOS_UINT8             ucCauseVal      ԭ��ֵ
  OUTPUT   : VOS_VOID
  RETURN   : VOS_VOID
  NOTE     :
  HISTORY  :
     1.  ��־��      04-03-15  �°�����
*******************************************************************************/
VOS_VOID Mm_ComSaveProcAndCauseVal(
                                VOS_UINT8             ucProc,
                                VOS_UINT16            usCauseVal
                                )
{
    if ( ( MM_NULL_PROC == g_MmGlobalInfo.ucProc )
        && ( NAS_MML_REG_FAIL_CAUSE_NULL == g_MmGlobalInfo.usCauseVal ) )
    {
        g_MmGlobalInfo.usCauseVal   = usCauseVal;
        g_MmGlobalInfo.ucProc       = ucProc;
    }
    else
    {
        if ( ( MM_NULL_PROC != ucProc )
            && ( NAS_MML_REG_FAIL_CAUSE_NULL != usCauseVal ) )
        {
            g_MmGlobalInfo.usCauseVal   = usCauseVal;
            g_MmGlobalInfo.ucProc       = ucProc;
        }
        else
        {
            if ( MM_AUTH_FAIL_PROC == ucProc )
            {
                g_MmGlobalInfo.usCauseVal   = usCauseVal;
                g_MmGlobalInfo.ucProc       = ucProc;
            }
        }
    }
    return;
}

/*******************************************************************************
  MODULE   : Mm_ComJudgeLimitSvc
  FUNCTION : �ж��Ƿ������޷���״̬
  INPUT    : VOS_VOID
  OUTPUT   : VOS_VOID
  RETURN   : VOS_UINT8             ucResult         �Ƿ������޷���
  NOTE     :
  HISTORY  :
     1.  ��־��      04-03-15  �°�����
     2.
*******************************************************************************/
VOS_UINT8 Mm_ComJudgeLimitSvc(
                         VOS_VOID
                         )
{
    VOS_UINT8             ucResult = MM_FALSE;                                      /* �Ƿ������޷���                           */
    if ( ( MM_LIMITED_SERVICE == g_MmGlobalInfo.ucMmServiceState )
        || ( MM_ATTEMPTING_TO_UPDATE == g_MmGlobalInfo.ucMmServiceState )
        || ( MM_NO_IMSI == g_MmGlobalInfo.ucMmServiceState ) )
    {                                                                           /* �����޷���                               */
        ucResult = MM_TRUE;                                                     /* ���������޷���                           */
    }

    return ucResult;                                                            /* ����                                     */
}

/*******************************************************************************
  MODULE   : Mm_ComFillInCMSncReq
  FUNCTION : �����Ϣ����
  INPUT    : VOS_UINT8                      ucCMSvcType          CM��������
             VOS_UINT8                      ucPrioLeveFlg        ���ȼ�
             MM_MSG_CM_SVC_REQ_STRU     *CmSvcReq            ��Ϣ���ݽṹ��ָ��
  OUTPUT   : VOS_VOID
  RETURN   : VOS_VOID
  NOTE     :
  HISTORY  :
     1.  ��־��      04-03-15  �°�����
     2.
*******************************************************************************/
VOS_VOID Mm_ComFillInCMSncReq(
                            VOS_UINT8                       ucCMSvcType,
                            VOS_UINT8                       ucPrioLeveFlg,
                            MM_MSG_CM_SVC_REQ_STRU      *CmSvcReq
                            )
{
    CmSvcReq->MmIeCKSN.ucCksn = NAS_MML_GetSimCsSecurityCksn();           /* �洢CKSN                                 */
    CmSvcReq->MmIeCmSvcType.ucCMSvcType = ucCMSvcType;                          /* Supplementary service activation         */
    NAS_MM_FillMobileID(&(CmSvcReq->MmIeMobileId.MobileID));
    CmSvcReq->ucPrioLeveFlg = ucPrioLeveFlg;                                    /* û�����ȼ�                               */

    return;
}
VOS_VOID Mm_ComFollowOnProc(
                        VOS_VOID
                        )
{
    MM_MSG_CM_SVC_REQ_STRU              CmSvcReq;                               /* Ҫ���͵�CM SERVICE REQ��Ϣ               */
    VOS_UINT8                           ucCmSvsType;
    VOS_UINT8                           ucPdType;
    NAS_MM_CONN_CTRL_ENUM_UINT8         i;

    /* �ֲ�������ʼ�� */
    ucCmSvsType = 0xFF;
    ucPdType    = 0xFF;
    i           = MM_CONN_CTRL_CC;

    /* �л���ĺϷ���CCҵ��Ĵ��� */
    if (  ( MM_CONST_NUM_7 <
       g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulTransactionId)
       && ( MM_CONST_NUM_15 >
       g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulTransactionId)
       && ( MM_TRUE ==
       g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg ) )
    {                                                                           /* �еȴ���CC��������Ҫ������MM����         */
        g_MmGlobalInfo.ucStaOfRcvXXEstReq = MM_IDLE_NORMAL_SERVICE;             /* ��¼�յ���������ʱ��״̬                 */
        if ( MMCC_MO_NORMAL_CALL ==
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulCallType)
        {                                                                       /* ��������                                 */
            Mm_ComFillInCMSncReq(MM_IE_CM_SVC_TYPE_MO_CALL_EST,
                MM_FALSE, &CmSvcReq);                                           /* ���CM SERVICE REQUEST�ṹ��             */
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucEstingCallTypeFlg
                = MM_FALSE;                                                     /* ��������                                 */
            NAS_MML_SetCsEmergencyServiceFlg( VOS_FALSE);
        }
        else
        {                                                                       /* ��������                                 */
            Mm_ComFillInCMSncReq(MM_IE_CM_SVC_TYPE_EMG_CALL_EST,
                MM_FALSE, &CmSvcReq);                                           /* ���CM SERVICE REQUEST�ṹ��             */
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucEstingCallTypeFlg
                = MM_TRUE;                                                      /* ��������                                 */
            NAS_MML_SetCsEmergencyServiceFlg( VOS_TRUE );
        }
        Mm_ComMsgCmSvcReqSnd(&CmSvcReq);                                        /* �����෢�ͽ���MM���ӵ�����               */
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI =
            (VOS_UINT8)g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
            RcvXXEstReq.ulTransactionId;                                        /* ��¼���ڽ�����MM����                     */
        g_MmGlobalInfo.ucMMConnEstingPD = MM_IE_PD_CALL_CONTROL;                /* ��¼���ڽ�����MM���ӵ�PD                 */
        g_MmGlobalInfo.ucPreState   = WAIT_FOR_RR_CONNECTION_MM_CONNECTION;     /* ��¼Ǩ��֮ǰ��״̬                       */
        Mm_ComSetMmState(WAIT_FOR_OUTGOING_MM_CONNECTION);
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComFollowOnProc:NORMAL: STATUS is WAIT_FOR_OUTGOING_MM_CONNECTION");
        Mm_TimerStart(MM_TIMER_T3230);                                          /* ����TIMER3230                            */
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg = MM_FALSE;  /* ������ڻ����MM���ӽ�������ı�־       */

        return;
    }

    /* �л���Ĳ��Ϸ���CCҵ��Ĵ��� */
    if ( MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg )
    {
        /* �еȴ���CCҪ������MM����,TI���Ϸ�      */
        Mm_SndCcRelInd(g_MmCcEstReq.ulTransactionId,
            NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);   /* ֪ͨCC����ʧ��  */

        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg = MM_FALSE;  /* ������ڻ����MM���ӽ�������ı�־       */

    }

    /* ѭ������CM����ҵ�� */
    for ( i = 0 ; i < MM_CONN_CTRL_BUTT; i++ )
    {
        if ( MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[i].RcvXXEstReq.ucFlg )
        {
            if (  ( MM_CONST_NUM_7 <  g_MmGlobalInfo.ConnCtrlInfo[i].RcvXXEstReq.ulTransactionId)
               && ( MM_CONST_NUM_15 > g_MmGlobalInfo.ConnCtrlInfo[i].RcvXXEstReq.ulTransactionId) )
            {
                if ( MM_CONN_CTRL_SS == i )
                {
                    ucCmSvsType = MM_IE_CM_SVC_TYPE_SS_ACT;
                    ucPdType    = MM_IE_PD_NON_CALL_RLT_SS_MSG;
                }
                else if ( MM_CONN_CTRL_SMS == i )    
                {
                    ucCmSvsType = MM_IE_CM_SVC_TYPE_SMS;
                    ucPdType    = MM_IE_PD_SMS_MSG;   
                }
                else
                {
                    /* ��SS/SMS��Ϣ�����쳣���� */
                    break;
                }
                
                                    
                g_MmGlobalInfo.ucStaOfRcvXXEstReq = MM_IDLE_NORMAL_SERVICE;         /* ��¼�յ���������ʱ��״̬                 */
                Mm_ComFillInCMSncReq(ucCmSvsType,
                    MM_FALSE, &CmSvcReq);                                           /* ���CM SERVICE REQUEST�ṹ��             */
                Mm_ComMsgCmSvcReqSnd(&CmSvcReq);                                    /* �����෢�ͽ���MM���ӵ�����               */

                g_MmGlobalInfo.ConnCtrlInfo[i].ucMMConnEstingTI =
                (VOS_UINT8)g_MmGlobalInfo.ConnCtrlInfo[i].RcvXXEstReq.ulTransactionId;                                        /* ��¼���ڽ�����MM����                     */
                g_MmGlobalInfo.ucMMConnEstingPD = ucPdType;                         /* ��¼���ڽ�����MM���ӵ�PD                 */
                g_MmGlobalInfo.ucPreState   = WAIT_FOR_RR_CONNECTION_MM_CONNECTION; /* ��¼Ǩ��֮ǰ��״̬                       */
                Mm_ComSetMmState(WAIT_FOR_OUTGOING_MM_CONNECTION);
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComFollowOnProc:NORMAL: STATUS is WAIT_FOR_OUTGOING_MM_CONNECTION");
                Mm_TimerStart(MM_TIMER_T3230);                                      /* ����TIMER3230                            */

                /* ������ڻ����MM���ӽ�������ı�־ */
                g_MmGlobalInfo.ConnCtrlInfo[i].RcvXXEstReq.ucFlg = MM_FALSE;
                
                /* ����CM ������� */
                return;
            }

            /* �л���ķǷ���CMҵ��Ĵ��� */
            if ( MM_CONN_CTRL_SS == i )
            {
                Mm_SndSsRelInd(g_MmGlobalInfo.ConnCtrlInfo[i].RcvXXEstReq.ulTransactionId,
                               NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);
            }
            else if ( MM_CONN_CTRL_SMS == i )
            {
                Mm_SndSmsRelInd( g_MmGlobalInfo.ConnCtrlInfo[i].
                    RcvXXEstReq.ulTransactionId,
                    NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);                        /* ֪ͨSMS����ʧ��                          */
            }
            else
            {
            }
            
            /* ������ڻ����MM���ӽ�������ı�־ */
            g_MmGlobalInfo.ConnCtrlInfo[i].RcvXXEstReq.ucFlg = MM_FALSE;
            
        }
        
    }

    /* �����ǰû�л�����߻������ϢTI�Ƿ��������״̬Ǩ�� */
    g_MmGlobalInfo.ucStaAfterWaitForNwkCmd = MM_IDLE_NORMAL_SERVICE;        /* ������е�״̬                           */
    Mm_ComSaveProcAndCauseVal( MM_MM_CONN_PROC, NAS_MML_REG_FAIL_CAUSE_NULL );        /* MM��������                               */
    g_MmGlobalInfo.ucPreState   = LOCATION_UPDATING_INITIATED;              /* ��¼Ǩ��֮ǰ��״̬                       */
    Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);
    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComFollowOnProc:NORMAL: STATUS is WAIT_FOR_NETWORK_COMMAND");
    Mm_TimerStart(MM_TIMER_T3240);

    return;
}

/*******************************************************************************
  MODULE   : Mm_ComChkAttDelay
  FUNCTION : ����Ƿ��б�����ATTACH
  INPUT    : VOS_VOID
  OUTPUT   : VOS_VOID
  RETURN   : VOS_VOID
  NOTE     :
  HISTORY  :
     1.  ��־��      04-03-15  �°�����
     2.
*******************************************************************************/
VOS_VOID Mm_ComChkAttDelay( )
{
    if ( MM_TRUE == g_MmGlobalInfo.ucAttDelay )
    {
        if ( ( MM_IDLE_LIMITED_SERVICE == g_MmGlobalInfo.ucState )
            && ( MM_IDLE_ATTEMPTING_TO_UPDATE == g_MmGlobalInfo.ucState )
            )
        {
            g_MmGlobalInfo.LuInfo.ucLuType = MM_IE_LUT_NORMAL_LU;
            Mm_ComLuOnly();
        }
        g_MmGlobalInfo.ucAttDelay = MM_FALSE;
    }
}


VOS_VOID Mm_ComProcCauseProcess_Handling_LU_PROC(VOS_VOID)
{
    if ( NAS_MML_REG_FAIL_CAUSE_NULL == g_MmGlobalInfo.usCauseVal )
    {
        Mm_ComSetMmState(MM_IDLE_NORMAL_SERVICE);
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComProcCauseProcess_Handling_LU_PROC:NORMAL: STATUS is MM_IDLE_NORMAL_SERVICE");
        Mm_TimerStart(MM_TIMER_T3212);                                      /* ����T3212                                */
        if (MM_DETACH_NEED_DELAY == g_MmGlobalInfo.ucDetachType)
        {
            Mm_TimerStop(MM_TIMER_PROTECT_DETACH);
            g_MmGlobalInfo.ucDetachType = MM_DETACH_TYPE_BUTT;
            Mm_Cell_S3_E23(VOS_NULL_PTR);
        }
    }
    else
    {
        Mm_ComLuCauseHandle(g_MmGlobalInfo.usCauseVal);                     /* ����ԭ��ֵ�Ĵ���                         */
    }

    return;
}


VOS_VOID Mm_ComProcCauseProcess_Handling_MM_GMM_ATTACH_PROC(VOS_VOID)
{
    switch ( g_MmGlobalInfo.usCauseVal )
    {
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_MS:
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME:
        Mm_ComSetMmState(MM_IDLE_NO_IMSI);


        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComProcCauseProcess_Handling_MM_GMM_ATTACH_PROC:NORMAL: STATUS is MM_IDLE_NO_IMSI");
        break;
    case NAS_MML_REG_FAIL_CAUSE_GPRS_SERV_AND_NON_GPRS_SERV_NOT_ALLOW:
        Mm_ComSetMmState(MM_IDLE_NO_IMSI);


        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComProcCauseProcess_Handling_MM_GMM_ATTACH_PROC:NORMAL: STATUS is MM_IDLE_NO_IMSI");
        break;
    case NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL:
        Mm_ComGmmAttachCauseHandle(g_MmGlobalInfo.usCauseVal);              /* ����ԭ��ֵ�Ĵ���                         */
        break;
    default:
        break;
    }

    return;
}
VOS_VOID Mm_ComProcCauseProcess_Handling_GMM_RAU_PROC(VOS_VOID)
{
    switch ( g_MmGlobalInfo.usCauseVal )
    {
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_MS:
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME:
        Mm_ComSetMmState(MM_IDLE_NO_IMSI);


        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComProcCauseProcess_Handling_GMM_RAU_PROC:NORMAL: STATUS is MM_IDLE_NO_IMSI");
        break;
    case NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL:
        Mm_ComGmmRauCauseHandle(g_MmGlobalInfo.usCauseVal);                 /* ����ԭ��ֵ�Ĵ���                         */
        break;
    default:
        break;
    }

    return;
}

/***********************************************************************
 *  MODULE   : Mm_ComProcCauseProcess_Handling_MM_GMM_SR_PROC
 *  FUNCTION : Mm_ComProcCauseProcess���������Ӷ�: MM_GMM_SR_PROC����
 *  INPUT    : VOS_VOID
 *  OUTPUT   : VOS_VOID
 *  RETURN   : VOS_VOID
 *  NOTE     :
 *  HISTORY  :
     1.  ŷ����   2009.06.11  �°�����
 ************************************************************************/
VOS_VOID Mm_ComProcCauseProcess_Handling_MM_GMM_SR_PROC(VOS_VOID)
{
    switch ( g_MmGlobalInfo.usCauseVal )
    {
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_MS:
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME:
        /* �ο�4.5.1.1 */
        Mm_ComLuCauseHandle(g_MmGlobalInfo.usCauseVal);

        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComProcCauseProcess_Handling_MM_GMM_SR_PROC:NORMAL: STATUS is MM_IDLE_NO_IMSI");
        break;
    case NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL:
        Mm_ComGmmSrCauseHandle(g_MmGlobalInfo.usCauseVal);                  /* ����ԭ��ֵ�Ĵ���                         */
        break;
    default:
        break;
    }

    return;
}


VOS_VOID Mm_ComProcCauseProcess_Handling_MM_NET_DETACH_PROC(VOS_VOID)
{
    VOS_UINT8                           ucOldUpdateSts;
    VOS_UINT8                           ucTempUpdateSts;
    MM_LAI_STRU                         OldLai;
    MM_LAI_STRU                         TempLai;

    switch ( g_MmGlobalInfo.usCauseVal )
    {
    case NAS_MML_REG_FAIL_CAUSE_IMSI_UNKNOWN_IN_HLR:
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_MS:
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME:
    case NAS_MML_REG_FAIL_CAUSE_GPRS_SERV_AND_NON_GPRS_SERV_NOT_ALLOW:
        Mm_ComSetMmState(MM_IDLE_NO_IMSI);


        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComProcCauseProcess_Handling_MM_NET_DETACH_PROC:NORMAL: STATUS is MM_IDLE_NO_IMSI");
        break;
    case NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW:
    case NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL:
        Mm_ComNetDetachCauseHandle(g_MmGlobalInfo.usCauseVal);              /* ����ԭ��ֵ�Ĵ���                         */
        break;
    case NAS_MML_REG_FAIL_CAUSE_NTDTH_IMSI:
        g_MmGlobalInfo.ucMmServiceState = MM_ATTEMPTING_TO_UPDATE;          /* ���÷���״̬                             */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComProcCauseProcess_Handling_MM_NET_DETACH_PROC:INFO: MmServiceState is MM_ATTEMPTING_TO_UPDATE");
        ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
        PS_MEM_CPY( &OldLai,
                     &g_MmGlobalInfo.MsCsInfo.OldLai,
                     sizeof( MM_LAI_STRU ));
        Mm_ComToU2Handle();

        ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
        PS_MEM_CPY( &TempLai,
                     &g_MmGlobalInfo.MsCsInfo.OldLai,
                     sizeof( MM_LAI_STRU ));
        NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
        PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                     &OldLai,
                     sizeof( MM_LAI_STRU ));
        NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
        PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                     &TempLai,
                     sizeof( MM_LAI_STRU ));

        if (VOS_TRUE == NAS_MM_IsDisableGprsCombineAttach())
        {
            Mm_TimerStart(MM_TIMER_T3212);                              /* ����TIMER3212                            */
        }
        break;
    default:
        break;
    }

    return;
}
VOS_VOID Mm_ComProcCauseProcess_Handling_MM_MM_CONN_PROC(VOS_VOID)
{
    VOS_UINT8                           ucPsSimStatus;

    ucPsSimStatus = NAS_MML_GetSimPsRegStatus();

    switch ( g_MmGlobalInfo.usCauseVal )
    {
    case NAS_MML_REG_FAIL_CAUSE_NULL:
        Mm_ComSetMmState(g_MmGlobalInfo.ucStaAfterWaitForNwkCmd);
        PS_LOG1(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComProcCauseProcess_Handling_MM_MM_CONN_PROC:NORMAL: ucState = ", g_MmGlobalInfo.ucState);
        break;
    case NAS_MML_REG_FAIL_CAUSE_IMSI_UNKNOWN_IN_VLR:

        /* ��ǰCS��Ч��LAIû�н�ֹ */
        if ( (VOS_TRUE == NAS_MML_GetSimCsRegStatus())
          && (( MMCMM_NO_FORBIDDEN == g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg )
           || ( MMCMM_FORBIDDEN_PLMN_FOR_GPRS == g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg )))
        {
            /*
            24.008 4.5.1.1 CM SERVICE REJECT Process: If cause value #4
            is received, If subsequently the RR connection is released
            or aborted, this will force the mobile station to initiate
            a normal location updating.
            */
            if ( (VOS_TRUE == NAS_MML_GetPsAttachAllowFlg())
              && (MM_NET_MODE_I == g_MmGlobalInfo.ucNtMod)
              && (VOS_TRUE == ucPsSimStatus)
              && (MMCMM_FORBIDDEN_PLMN_FOR_GPRS != g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg)
              && (MM_FALSE == g_MmGlobalInfo.ucLikeB))
            {                                                               /* �ж��Ƿ�ΪA+Iģʽ                        */
                /* MM�յ�CM Service Reject��Ϣʱ�Ѿ�֪ͨGMM,GMM���յ�
                CM_SERVICE_IND��Ϣָʾ��CS����ʱ���� COMBINED_RALAU_WITH_IMSI_ATTACH*/
                Mm_ComSetMmState(MM_IDLE_ATTEMPTING_TO_UPDATE);
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComProcCauseProcess_Handling_MM_MM_CONN_PROC:NORMAL: STATUS is MM_IDLE_ATTEMPTING_TO_UPDATE");
            }
            else
            {
                g_MmGlobalInfo.LuInfo.ucLuType = MM_IE_LUT_NORMAL_LU;       /* ����LU����                               */
                Mm_ComLuOnly();                                             /* LU REQ                                   */
            }
        }
        else
        {
            /* �ͷŵ�ǰ���� */
            Mm_TimerStop(MM_TIMER_PROTECT_SIGNALLING);

            Mm_ComRelAllMmConn(NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);

            /* ת�����ӽ���ǰ��״̬ */
            Mm_ComSetMmState(g_MmGlobalInfo.ucStaOfRcvXXEstReq);
        }
        break;
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME:
        Mm_ComSetMmState(MM_IDLE_NO_IMSI);


        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComProcCauseProcess_Handling_MM_MM_CONN_PROC:NORMAL: STATUS is MM_IDLE_NO_IMSI");
        break;
    default:
        break;
    }

    return;
}


VOS_VOID Mm_ComProcCauseProcess_Handling_MM_MM_NET_ABORT(VOS_VOID)
{
    switch ( g_MmGlobalInfo.usCauseVal )
    {
    case NAS_MML_REG_FAIL_CAUSE_NETWORK_FAILURE:
        switch ( g_MmGlobalInfo.ucMmServiceState )
        {
        case MM_NORMAL_SERVICE:
            Mm_ComSetMmState(MM_IDLE_NORMAL_SERVICE);
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComProcCauseProcess_Handling_MM_MM_NET_ABORT:NORMAL: STATUS is MM_IDLE_NORMAL_SERVICE");
            break;
        case MM_LIMITED_SERVICE:
            Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComProcCauseProcess_Handling_MM_MM_NET_ABORT:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
            break;
        case MM_ATTEMPTING_TO_UPDATE:
            Mm_ComSetMmState(MM_IDLE_ATTEMPTING_TO_UPDATE);


            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComProcCauseProcess_Handling_MM_MM_NET_ABORT:NORMAL: STATUS is MM_IDLE_ATTEMPTING_TO_UPDATE");
            break;
        case MM_NO_IMSI:
            Mm_ComSetMmState(MM_IDLE_NO_IMSI);


            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComProcCauseProcess_Handling_MM_MM_NET_ABORT:NORMAL: STATUS is MM_IDLE_NO_IMSI");
            break;
        default:
            break;
        }
        break;
    case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME:
        Mm_ComSetMmState(MM_IDLE_NO_IMSI);


        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComProcCauseProcess_Handling_MM_MM_NET_ABORT:NORMAL: STATUS is MM_IDLE_NO_IMSI");
        break;
    default:
        break;
    }

    return;
}


VOS_VOID Mm_ComProcCauseProcess_Handling_MM_IMSI_DETACH(VOS_VOID)
{
    Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComProcCauseProcess_Handling_MM_IMSI_DETACH:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
    return;
}


VOS_VOID Mm_ComProcCauseProcess( )
{
    switch ( g_MmGlobalInfo.ucProc )
    {
    case MM_NULL_PROC:
        break;
    case MM_LU_PROC:
        Mm_ComProcCauseProcess_Handling_LU_PROC();
        break;
    case MM_GMM_ATTACH_PROC:
        Mm_ComProcCauseProcess_Handling_MM_GMM_ATTACH_PROC();
        break;
    case MM_GMM_RAU_PROC:
        Mm_ComProcCauseProcess_Handling_GMM_RAU_PROC();
        break;
    case MM_COMB_ATTACH_PROC:
        break;
    case MM_COMB_RAU_PROC:
        break;
    case MM_GMM_SR_PROC:
        Mm_ComProcCauseProcess_Handling_MM_GMM_SR_PROC();
        break;
    case MM_NET_DETACH_PROC:
        Mm_ComProcCauseProcess_Handling_MM_NET_DETACH_PROC();
        break;
    case MM_AUTH_FAIL_PROC:
        Mm_ComSetMmState(MM_IDLE_NO_IMSI);

        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComProcCauseProcess:NORMAL: STATUS is MM_IDLE_NO_IMSI");
        break;
    case MM_MM_CONN_PROC:
        Mm_ComProcCauseProcess_Handling_MM_MM_CONN_PROC();
        break;
    case MM_MM_NET_ABORT:
        Mm_ComProcCauseProcess_Handling_MM_MM_NET_ABORT();
        break;
    case MM_IMSI_DETACH:
        Mm_ComProcCauseProcess_Handling_MM_IMSI_DETACH();
        break;
    case MM_MM_MS_AUTH_FAIL_PROC:

        Mm_ComSetMmState(g_MmGlobalInfo.ucStaOfRcvXXEstReq);

        PS_LOG1(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComProcCauseProcess:NORMAL: STATUS is ",g_MmGlobalInfo.ucState);
        break;
    default:
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_ComProcCauseProcess:WARNING: ucProc Abnormal");
        break;
    }
    Mm_ComProcCauseClear();
}

/*******************************************************************************
  MODULE   : Mm_ComDelLai
  FUNCTION : ɾ��LAI�Ĺ�������
  INPUT    : VOS_VOID
  OUTPUT   : VOS_VOID
  RETURN   : VOS_VOID
  NOTE     :
  HISTORY  :
     1.  ��־��    2004.04.12  �°�����,MM_CHK_BUG_021 ��Ӧ
*******************************************************************************/

VOS_VOID Mm_ComDelLai()
{
    NAS_MML_LAI_STRU                   *pstCsSuccLai;

    pstCsSuccLai    = NAS_MML_GetCsLastSuccLai(  );

    pstCsSuccLai->aucLac[0] = NAS_MML_LAC_LOW_BYTE_INVALID;
    pstCsSuccLai->aucLac[1] = NAS_MML_LAC_HIGH_BYTE_INVALID;

    g_MmGlobalInfo.MsCsInfo.OldLai.ulLac = MM_INVALID_LAI;

    pstCsSuccLai->ucRac = NAS_MML_RAC_INVALID;
}

/*******************************************************************************
  MODULE   : Mm_ComProcCauseClear
  FUNCTION : ���ԭ��ֵ������
  INPUT    : VOS_VOID
  OUTPUT   : VOS_VOID
  RETURN   : VOS_VOID
  NOTE     :
  HISTORY  :
     1. ��־��   2004.04.26 �¹�����
*******************************************************************************/

VOS_VOID Mm_ComProcCauseClear()
{
    g_MmGlobalInfo.ucProc       = MM_NULL_PROC;                                 /* ��������                                 */
    /*WUEPS_TRACE( MM_LOG_LEVEL_6, " \nMM:MmServiceState is MM_NULL_PROC \r " );*/
    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComProcCauseClear:INFO: MmServiceState is MM_NULL_PROC");
    g_MmGlobalInfo.usCauseVal   = NAS_MML_REG_FAIL_CAUSE_NULL;                            /* ��¼ԭ��ֵ                               */
    /*WUEPS_TRACE( MM_LOG_LEVEL_6, " \nMM:mmCauseVal is NAS_MML_REG_FAIL_CAUSE_NULL \r " );*/
    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComProcCauseClear:INFO: mmCauseVal is NAS_MML_REG_FAIL_CAUSE_NULL");
}


VOS_UINT8    Mm_ComChkLu( )
{
    VOS_UINT8                           ucRet = MM_FALSE;

    NAS_MML_CONN_STATUS_INFO_STRU      *pstConnStatus = VOS_NULL_PTR;


    /* ȡ�õ�ǰ��������Ϣ */
    pstConnStatus = NAS_MML_GetConnStatus();

    if (VOS_TRUE != pstConnStatus->ucRrcStatusFlg)
    {
        return MM_FALSE;
    }

    ucRet = NAS_MM_ProcessLuAfterCsConnectNotExist();

    return ucRet;
}

/*******************************************************************************
  MODULE   : MM_Com_RacChgChk
  FUNCTION : RAC�仯�ж�
  INPUT    : MMCMM_SYS_INFO_IND_STRU*
  OUTPUT   : VOS_VOID
  RETURN   : VOS_UINT8  MM_TRUE �仯
                    MM_FALSE δ�仯
  NOTE     :
  HISTORY  :
     1. ��־��   2004.06.15 �¹�����
*******************************************************************************/
VOS_UINT8    MM_Com_RacChgChk( MMCMM_SYS_INFO_IND_STRU* pSysInfo )
{
    VOS_UINT8   ucRet;
    VOS_UINT16  usRac;
    VOS_UINT32   ulLac;

    ucRet = MM_FALSE;
    usRac = (VOS_UINT16)pSysInfo->ucRac;
    ulLac = (VOS_UINT32)pSysInfo->usLac;

    if ( ( g_MmGlobalInfo.MsCsInfo.CurLai.PlmnId.ulMcc !=
            pSysInfo->PlmnId.ulMcc )
        || ( g_MmGlobalInfo.MsCsInfo.CurLai.PlmnId.ulMnc !=
            pSysInfo->PlmnId.ulMnc )
        || ( g_MmGlobalInfo.MsCsInfo.CurLai.ulLac != ulLac )
        || ( g_MmGlobalInfo.usRac != usRac )
        )
    {                                                                           /* UE������RA��Ϣ�Ƿ���ϵͳ��Ϣ�ϱ�����ͬ   */
        ucRet = MM_TRUE;
    }

    return ucRet;
}
/* added by libin for VOS 20050125 begin */
/*******************************************************************************
  Module:   Mm_TimerPause
  Function: MMʹ�ö�ʱ����ͣ
  Input:    VOS_UINT8       ucTimerId   Timer ID
  Output:
  NOTE:
  Return:   VOS_VOID
  History:
      1.  ��־��     2005.01.25  �°�����
*******************************************************************************/
VOS_VOID Mm_TimerPause( VOS_UINT8   ucTimerId )
{
    VOS_UINT8   i;

    if ( MM_TIMER_STOP_ALL == ucTimerId )
    {
        for ( i = 0; i < MM_TIMER_NUM; i++ )
        {
            if ( MM_TIMER_RUNNING == gstMmTimer[i].ucTimerStatus )
            {
                if( VOS_ERRNO_RELTM_STOP_TIMERINVALID ==
                    Mm_StopRelTimer(WUEPS_PID_MM, i, &gstMmTimer[i].hTimer))
                {                                                               /* ��ͣʧ�ܣ��Ѿ���ͣ��Timer������          */
                    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_TimerPause:WARNING: TIMER STOP ERROR!");
                }

                gstMmTimer[i].ucTimerStatus = MM_TIMER_PAUSE;
            }
        }
    }
    else
    {
        if ( MM_TIMER_RUNNING == gstMmTimer[ucTimerId].ucTimerStatus)
        {                                                                       /* ��timer�Ѿ�����                          */

            if ( VOS_ERRNO_RELTM_STOP_TIMERINVALID ==
                Mm_StopRelTimer(WUEPS_PID_MM, ucTimerId, &gstMmTimer[ucTimerId].hTimer) )
            {                                                                   /* ��ͣʧ��                                 */
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_TimerPause:WARNING: TIMER STOP ERROR!");
            }

            gstMmTimer[ucTimerId].ucTimerStatus = MM_TIMER_PAUSE;                /* ��Timer��������־                        */
        }
    }
}

/*******************************************************************************
  Module:   Mm_TimerResume
  Function: MMʹ�ö�ʱ���ָ�
  Input:    VOS_UINT8       ucTimerId   Timer ID
  Output:
  NOTE:
  Return:   VOS_VOID
  History:
      1.  ��־��     2005.01.25  �°�����
*******************************************************************************/
VOS_VOID Mm_TimerResume( VOS_UINT8   ucTimerId )
{
    VOS_UINT8   i;

    if ( MM_TIMER_STOP_ALL == ucTimerId )
    {
        for ( i = 0; i < MM_TIMER_NUM; i++ )
        {
            if ( MM_TIMER_PAUSE == gstMmTimer[i].ucTimerStatus )
            {
                

                

                if( VOS_OK != Mm_StartRelTimer(
                                           &gstMmTimer[i].hTimer,     /* ���TIMER ID�ĵ�ַ                       */
                                           WUEPS_PID_MM,
                                           gstMmTimer[i].ulTimerLen,         /* ʱ��                                     */
                                           (VOS_UINT32)i,            /* ��Ϣ���е�ID                             */
                                           gstMmTimer[i].ulParam,               /* ��Ϣ��ַ                                 */
                                           gstMmTimer[i].ucMode          /* ��Ϣ���ȼ�                               */
                                           ))
                {                                                               /* �ָ�ʧ�ܣ��Ѿ��ָ���Timer������          */
                    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_ERROR, "Mm_TimerResume:ERROR: TIMER START ERROR!");
                }

                gstMmTimer[i].ucTimerStatus = MM_TIMER_RUNNING;
            }
        }
    }
    else
    {
        if ( MM_TIMER_PAUSE == gstMmTimer[ucTimerId].ucTimerStatus)
        {                                                                       /* ��timer�Ѿ�����                          */
            


            
            

            if ( VOS_OK != Mm_StartRelTimer(
                                           &gstMmTimer[ucTimerId].hTimer,     /* ���TIMER ID�ĵ�ַ                       */
                                           WUEPS_PID_MM,
                                           gstMmTimer[ucTimerId].ulTimerLen,         /* ʱ��                                     */
                                           (VOS_UINT32)ucTimerId,            /* ��Ϣ���е�ID                             */
                                           gstMmTimer[ucTimerId].ulParam,               /* ��Ϣ��ַ                                 */
                                           gstMmTimer[ucTimerId].ucMode          /* ��Ϣ���ȼ�                               */
                                           ) )
            {                                                                   /* ��ͣʧ��                                 */
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_ERROR, "Mm_TimerResume:ERROR: TIMER START ERROR!");
            }

            gstMmTimer[ucTimerId].ucTimerStatus = MM_TIMER_RUNNING;              /* ��Timer��������־                        */
        }
    }
}



VOS_VOID Mm_ComT3212(VOS_UINT32  ulT3212Timer)
{
    VOS_UINT32                          ulTemp;
    VOS_UINT32                          ulTemp1;
    VOS_UINT32                          ulTempRemain;

    switch ( g_MmGlobalInfo.ucState )
    {
    case MM_NULL:
    case MM_IDLE_NO_CELL_AVAILABLE:
    case MM_IDLE_LIMITED_SERVICE:
    case MM_IDLE_PLMN_SEARCH:
    case MM_IDLE_PLMN_SEARCH_NORMAL_SERVICE:

        gstMmTimer[MM_TIMER_T3212].ulTimerLen = ulT3212Timer;                    /* T3212ʱ���ĸ�ֵ                          */

        break;

    default:
        gstMmTimer[MM_TIMER_T3212].ulTimerLen = ulT3212Timer;                    /* T3212ʱ���ĸ�ֵ                          */

        if ( MM_TIMER_RUNNING == gstMmTimer[MM_TIMER_T3212].ucTimerStatus )
        {                                                                       /* ���T3212����������                      */
            ulTemp1 = ulT3212Timer;


            ulTempRemain = MM_CONST_NUM_0;

            if (VOS_NULL_PTR != gstMmTimer[MM_TIMER_T3212].hTimer)
            {
                /* hTimer Ϊ��ʱ��˵���˶�ʱ���ѳ�ʱ��ֹͣ,��Ϊ��ʱ����ȡ��ʣ�೤�� */
                if (VOS_OK != VOS_GetRelTmRemainTime(&(gstMmTimer[MM_TIMER_T3212].hTimer), &ulTempRemain))
                {
                    ulTempRemain = MM_CONST_NUM_0;
                }
            }

            ulTemp = ulTempRemain * MM_TIMER_TICK;


            if ( VOS_ERRNO_RELTM_STOP_TIMERINVALID ==
                    Mm_StopRelTimer(WUEPS_PID_MM, MM_TIMER_T3212, &gstMmTimer[MM_TIMER_T3212].hTimer ) )
            {                                                                   /* ��ͣʧ��                                 */
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_ComT3212:WARNING: TIMER STOP FAILURE!");
            }

            if ( MM_CONST_NUM_0 == ( ulTemp%ulTemp1 ) )
            {
                ulTemp = 1000;
            }
            else
            {
                ulTemp = (VOS_UINT32)ulTemp%ulTemp1;
            }
            if (VOS_OK != Mm_StartRelTimer(
                                           &gstMmTimer[MM_TIMER_T3212].hTimer,     /* ���TIMER ID�ĵ�ַ                       */
                                           WUEPS_PID_MM,
                                           ulTemp,         /* ʱ��                                     */
                                           MM_TIMER_T3212,            /* ��Ϣ���е�ID                             */
                                           gstMmTimer[MM_TIMER_T3212].ulParam,               /* ��Ϣ��ַ                                 */
                                           gstMmTimer[MM_TIMER_T3212].ucMode          /* ��Ϣ���ȼ�                               */
                                           ) )
            {                                                                   /* ����ʧ�ܣ��Ѿ��ָ���Timer������          */
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_ComT3212:WARNING: TIMER START FAILURE!");
            }


        }
        else
        {
            ulTemp1 = ulT3212Timer;

            /* ������� */
            VOS_SetSeed(VOS_GetTick());

            g_MmGlobalInfo.ulT3212Value = VOS_Rand(ulTemp1);
        }
        break;
    }

}


VOS_UINT32  NAS_MM_QryTimerStatus(VOS_UINT8 ucTimerId)
{

    if ( MM_TIMER_STOP  != gstMmTimer[ucTimerId].ucTimerStatus )
    {
        return MM_TIMER_RUNNING;
    }
    else
    {
        return MM_TIMER_STOP;
    }

}

/*******************************************************************************
  Module:   Mm_TimerStart
  Function: MMʹ�ö�ʱ������
  Input:    VOS_UINT8       ucTimerId   Timer ID
  Output:
  NOTE:
  Return:   VOS_VOID
  History:
      1.  ��־��     2005.01.25  �°�����
*******************************************************************************/
VOS_VOID Mm_TimerStart( VOS_UINT8   ucTimerId )
{
    VOS_UINT32 ulResult;

    if ( MM_TIMER_STOP ==  gstMmTimer[ucTimerId].ucTimerStatus )
    {
        if ( MM_TIMER_T3212 == ucTimerId )
        {                                                                       /* ���ΪT3212                              */
            if ( MM_CONST_NUM_0 != gstMmTimer[ucTimerId].ulTimerLen )
            {
                if ( MM_CONST_NUM_0 != g_MmGlobalInfo.ulT3212Value )
                {
                    ulResult = Mm_StartRelTimer(
                                           &gstMmTimer[ucTimerId].hTimer,     /* ���TIMER ID�ĵ�ַ                       */
                                           WUEPS_PID_MM,
                                           g_MmGlobalInfo.ulT3212Value,         /* ʱ��                                     */
                                           MM_TIMER_T3212,            /* ��Ϣ���е�ID                             */
                                           gstMmTimer[ucTimerId].ulParam,               /* ��Ϣ��ַ                                 */
                                           gstMmTimer[ucTimerId].ucMode          /* ��Ϣ���ȼ�                               */
                                           );                                   /* ����֧����Ϣ�Ķ�ʱ��                     */
                    if ( VOS_OK != ulResult )
                    {                                                           /* ����ʧ��                                 */
                        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_TimerStart:WARNING: TIMER CREATE FAILURE!");
                        return;
                    }

                    g_MmGlobalInfo.ulT3212Value = MM_CONST_NUM_0;
                }
                else
                {
                    ulResult = Mm_StartRelTimer(
                                           &gstMmTimer[ucTimerId].hTimer,     /* ���TIMER ID�ĵ�ַ                       */
                                           WUEPS_PID_MM,
                                           gstMmTimer[ucTimerId].ulTimerLen,         /* ʱ��                                     */
                                           MM_TIMER_T3212,            /* ��Ϣ���е�ID                             */
                                           gstMmTimer[ucTimerId].ulParam,               /* ��Ϣ��ַ                                 */
                                           gstMmTimer[ucTimerId].ucMode          /* ��Ϣ���ȼ�                               */
                                           );                                                /* ����֧����Ϣ�Ķ�ʱ��                     */
                    if ( VOS_OK != ulResult )
                    {                                                           /* ����ʧ��                                 */
                        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_TimerStart:WARNING: TIMER CREATE FAILURE!");
                        return;
                    }

                }

                gstMmTimer[ucTimerId].ucTimerStatus = MM_TIMER_RUNNING;          /* ��Timer��������־                        */
                /*WUEPS_TRACE(MM_LOG_LEVEL_6,"\nMM StartTimer: %d\r",ucTimerId);*/
                PS_LOG1(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_TimerStart:INFO: StartTimer: ", ucTimerId);
            }
        }
        else
        {
            if (MM_TIMER_PROTECT_SIGNALLING  == ucTimerId)
            {
                if (NAS_MML_NET_RAT_TYPE_GSM == NAS_MML_GetCurrNetRatType())
                {
                    gstMmTimer[MM_TIMER_PROTECT_SIGNALLING].ulTimerLen = MM_TIMER_PROTECT_G_SIGNALLING_VALUE;
                }
                else
                {
                    gstMmTimer[MM_TIMER_PROTECT_SIGNALLING].ulTimerLen = MM_TIMER_PROTECT_W_SIGNALLING_VALUE;
                }
            }
            ulResult = Mm_StartRelTimer(  &gstMmTimer[ucTimerId].hTimer,     /* ���TIMER ID�ĵ�ַ                       */
                                           WUEPS_PID_MM,
                                           gstMmTimer[ucTimerId].ulTimerLen,         /* ʱ��                                     */
                                           (VOS_UINT32)ucTimerId,                       /* ��Ϣ���е�ID                             */
                                           gstMmTimer[ucTimerId].ulParam,               /* ��Ϣ��ַ                                 */
                                           gstMmTimer[ucTimerId].ucMode          /* ��Ϣ���ȼ�                               */
                                           );                                             /* ����֧����Ϣ�Ķ�ʱ��                     */
            if ( VOS_OK != ulResult )                        /* ����ʧ��                                 */
            {
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_TimerStart:WARNING: TIMER CREATE FAILURE!");
                return;
            }
            gstMmTimer[ucTimerId].ucTimerStatus = MM_TIMER_RUNNING;              /* ��Timer��������־                        */
            PS_LOG1(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_TimerStart:INFO: StartTimer: ", ucTimerId);
        }
    }

}
VOS_UINT32 NAS_MM_IsTimerIdInTimerList(
    VOS_UINT8                           ucTimerId,
    VOS_UINT8                           ucTimerNum,
    VOS_UINT8                          *pucTimer
)
{
    VOS_UINT8                           ucIndex;

    for ( ucIndex = 0 ; ucIndex < ucTimerNum; ucIndex++ )
    {
        if ( ucTimerId == *(pucTimer + ucIndex) )
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_VOID NAS_MM_TimerStopExceptSpecialTimers(
    VOS_UINT8                           ucTimerNum,
    VOS_UINT8                          *pucTimer
)
{
    VOS_UINT8                           i;

    for ( i = 0; i < MM_TIMER_NUM; i++ )
    {
        if ( VOS_TRUE == NAS_MM_IsTimerIdInTimerList(i, ucTimerNum, pucTimer) )
        {
            continue;
        }


        if ( MM_TIMER_STOP != gstMmTimer[i].ucTimerStatus )
        {
            if (VOS_ERRNO_RELTM_STOP_TIMERINVALID ==
            Mm_StopRelTimer(WUEPS_PID_MM, i, &gstMmTimer[i].hTimer))
            {                                                               /* ֹͣʧ�ܣ��Ѿ�ֹͣ��Timer������          */
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_TimerStop:WARNING: TIMER STOP FAILURE!");
            }

            gstMmTimer[i].ucTimerStatus = MM_TIMER_STOP;

            PS_LOG1(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_TimerStop:INFO: StopTimer: ", i);
        }
    }
}


/*******************************************************************************
  Module:   Mm_TimerStop
  Function: MMʹ�ö�ʱ��ֹͣ
  Input:    VOS_UINT8       ucTimerId   Timer ID
  Output:
  NOTE:
  Return:   VOS_VOID
  History:
      1.  ��־��     2005.01.25  �°�����
*******************************************************************************/
VOS_VOID Mm_TimerStop( VOS_UINT8   ucTimerId )
{
    VOS_UINT8   i;

    if ( MM_TIMER_STOP_ALL == ucTimerId )
    {
        for ( i = 0; i < MM_TIMER_NUM; i++ )
        {
            if ( MM_TIMER_STOP != gstMmTimer[i].ucTimerStatus )
            {
                if (VOS_ERRNO_RELTM_STOP_TIMERINVALID ==
                Mm_StopRelTimer(WUEPS_PID_MM, i, &gstMmTimer[i].hTimer))
                {                                                               /* ֹͣʧ�ܣ��Ѿ�ֹͣ��Timer������          */
                    PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_TimerStop:WARNING: TIMER STOP FAILURE!");
                }

                gstMmTimer[i].ucTimerStatus = MM_TIMER_STOP;

                PS_LOG1(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_TimerStop:INFO: StopTimer: ", ucTimerId);
            }
        }
    }
    else if ( ucTimerId < MM_TIMER_NUM )
    {
        if ( MM_TIMER_STOP != gstMmTimer[ucTimerId].ucTimerStatus )
        {
            if (VOS_ERRNO_RELTM_STOP_TIMERINVALID ==
                    Mm_StopRelTimer(WUEPS_PID_MM, ucTimerId, &gstMmTimer[ucTimerId].hTimer))
            {                                                                   /* ֹͣʧ�ܣ��Ѿ�ֹͣ��Timer������          */
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_TimerStop:WARNING: TIMER STOP FAILURE!");
            }

            gstMmTimer[ucTimerId].ucTimerStatus = MM_TIMER_STOP;
            PS_LOG1(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_TimerStop:INFO: StopTimer: ", ucTimerId);
        }
    }
    else
    {
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_TimerStop:WARNING: TIMER ID ERROR!");
    }

}
/* added by libin for VOS 20050125 end */

VOS_VOID Mm_ComAuthMsRej()
{
    g_MmGlobalInfo.AuthenCtrlInfo.ucAuthenAttmptCnt = 0;
    g_MmGlobalInfo.AuthenCtrlInfo.ucLastFailCause = MM_AUTHEN_NO_CAUSE;
    Mm_TimerStop(MM_TIMER_T3214);
    Mm_TimerStop(MM_TIMER_T3216);

    Mm_ComRelAllMmConn(NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);


    Mm_SndRrRelReq(RRC_CELL_BARRED);
    switch(g_MmGlobalInfo.ucState)
    {
    case WAIT_FOR_OUTGOING_MM_CONNECTION:
        Mm_TimerStart(MM_TIMER_T3230);
        break;
    case MM_CONNECTION_ACTIVE:
    case WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION:
        Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);
        /*WUEPS_TRACE( MM_LOG_LEVEL_1,                          */
        /* " \nMM:STATUS is  WAIT_FOR_NETWORK_COMMAND\r " );    */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComAuthMsRej:NORMAL: STATUS is WAIT_FOR_NETWORK_COMMAND");
        Mm_TimerStart(MM_TIMER_T3240);
        Mm_ComSaveProcAndCauseVal(
                                  MM_MM_MS_AUTH_FAIL_PROC,
                                  NAS_MML_REG_FAIL_CAUSE_NULL);
        break;
    case WAIT_FOR_NETWORK_COMMAND:
        break;
    case LOCATION_UPDATING_INITIATED:
        Mm_TimerStart(MM_TIMER_T3210);
        break;
    case LOCATION_UPDATE_REJECTED:
        break;
    case IMSI_DETACH_INITIATED:
        Mm_TimerStart(MM_TIMER_T3220);
        break;
    case PROCESS_CM_SERVICE_PROMPT:
        Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);
        /*WUEPS_TRACE( MM_LOG_LEVEL_1,                          */
        /* " \nMM:STATUS is  WAIT_FOR_NETWORK_COMMAND\r " );    */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComAuthMsRej:NORMAL: STATUS is WAIT_FOR_NETWORK_COMMAND");
        Mm_TimerStart(MM_TIMER_T3240);
        Mm_ComSaveProcAndCauseVal(
                                  MM_MM_MS_AUTH_FAIL_PROC,
                                  NAS_MML_REG_FAIL_CAUSE_NULL);
        break;
    default:
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_ComAuthMsRej:WARNING: g_MmGlobalInfo.ucState Abnormal");
        break;
    }
}

VOS_UINT8 Mm_ComTempIdJudge()
{
    VOS_UINT8 ucRet = MM_TRUE;

    if ( ( MM_MS_ID_TMSI_PRESENT ==
        ( g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg
/* Modified by libin for MM_Review_HW_BUG_010 20050214 begin */
/*                                & MM_MS_ID_IMSI_PRESENT ) ) && */
                                & MM_MS_ID_TMSI_PRESENT ) ) &&
/* Modified by libin for MM_Review_HW_BUG_010 20050214 end */
         ( MM_MS_ID_TMSI_PRESENT ==
        ( g_MmMsgLuAccpt.MmIeMobileId.MobileID.ucMsIdFlg
/* Modified by libin for MM_Review_HW_BUG_010 20050214 begin */
/*                                & MM_MS_ID_IMSI_PRESENT ) ) ) */
                                & MM_MS_ID_TMSI_PRESENT ) ) )
/* Modified by libin for MM_Review_HW_BUG_010 20050214 end */
    {
        if ( 0 == VOS_MemCmp(NAS_MML_GetUeIdTmsi(), g_MmMsgLuAccpt.MmIeMobileId.MobileID.aucTmsi, NAS_MML_MAX_TMSI_LEN))
        {
            ucRet = MM_FALSE;
        }

    }
    return ucRet;
}

VOS_VOID Mm_Com_UsimAuthenticationCnfChgFormat(
                               USIMM_AUTH_CNF_STRU *pUsimMsg
                               )
{
    VOS_UINT8                           i = 0;
    VOS_UINT8*                          pPointer;
    VOS_UINT8                           ucCause;
    USIMM_TELECOM_AUTH_CNF_STRU        *pstTELECnf = VOS_NULL_PTR;

    pstTELECnf = &(pUsimMsg->cnfdata.stTELECnf);

    /* UMTS AUTH SUCCESS or GSM AUTH SUCCESS */
    if ((USIMM_AUTH_UMTS_SUCCESS == pUsimMsg->enResult) 
     || (USIMM_AUTH_GSM_SUCCESS == pUsimMsg->enResult))
    {
        NAS_EventReport(WUEPS_PID_MM, NAS_OM_EVENT_AUTH_SUCCESS,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        g_AgentUsimAuthCnf.ucCheckRst = AGENT_AUTH_RST_SUCCESS;
        
        pPointer = pstTELECnf->aucAuthRes;

        if (pPointer[0] <= 4)
        {
            for ( i = 0; i < pPointer[0]; i++)
            {
                g_AgentUsimAuthCnf.aucResponse[i] = pPointer[i+1];
            }
            /* Added by libin for MM_Review_HW_BUG_053 20050629 begin */
            for ( ;i < 4; i++ )
            {
                g_AgentUsimAuthCnf.aucResponse[i] = 0xFF;
            }
            /* Added by libin for MM_Review_HW_BUG_053 20050629 end */

            g_AgentUsimAuthCnf.ucExtRspLength = 0x0;
        }
        else
        {
            for ( i = 0; i < 4; i++)
            {
                g_AgentUsimAuthCnf.aucResponse[i] = pPointer[i+1];
            }

            /* Modified by libin for MM_Review_HW_BUG_053 20050629 begin */
            if ( pPointer[0] > 16 )
            {
                g_AgentUsimAuthCnf.ucExtRspLength = 12;
            }
            else
            {
                g_AgentUsimAuthCnf.ucExtRspLength = pPointer[0] - 4;
            }
            /* Modified by libin for MM_Review_HW_BUG_053 20050629 end */

            for (i=0; i<g_AgentUsimAuthCnf.ucExtRspLength; i++)
            {
                g_AgentUsimAuthCnf.aucExtResponse[i] = pPointer[i+5];
            }
        }

        if (MM_ZERO == pstTELECnf->aucGsmKc[0]) /* ��Ȩ��������� KC */
        {
            g_AgentUsimAuthCnf.ucKcLength = 0;
        }
        else /* ��Ȩ������� KC, ��ȡ KC ֵ */
        {    
            for (i=0; i<8; i++)
            {
                /* skip the first length byte */
                g_AgentUsimAuthCnf.aucKc[i] = pstTELECnf->aucGsmKc[i+1];
            }

            /* get KC length from the first byte */
            g_AgentUsimAuthCnf.ucKcLength   = pstTELECnf->aucGsmKc[0];
        }

        for (i=0; i<16; i++)
        {
            /* skip the first length byte */
            g_AgentUsimAuthCnf.aucCipheringKey[i] = pstTELECnf->aucCK[i+1];
            
            /* skip the first length byte */        
            g_AgentUsimAuthCnf.aucIntegrityKey[i] = pstTELECnf->aucIK[i+1];
        }
    }
    else if (USIMM_AUTH_MAC_FAILURE == pUsimMsg->enResult)
    {
        g_AgentUsimAuthCnf.ucCheckRst = AGENT_AUTH_RST_FAILURE;
        g_AgentUsimAuthCnf.ucFailureCause = AGENT_AUTH_FAIL_MAC_CODE;
        ucCause = NAS_OM_MM_CAUSE_MAC_FAILURE;
        NAS_MM_SndMmcSimAuthFailInfo(NAS_MML_SIM_AUTH_FAIL_MAC_FAILURE);
    }
    else if (USIMM_AUTH_SYNC_FAILURE == pUsimMsg->enResult)
    {
        g_AgentUsimAuthCnf.ucFailParaLength = 14;
        
        pPointer = pstTELECnf->aucAuts;
        if ( pstTELECnf->aucAuts[0] > 14 )        
        {
            for ( i = 0; i < g_AgentUsimAuthCnf.ucFailParaLength; i++)
            {
                g_AgentUsimAuthCnf.aucFailPara[i] = pPointer[i+1];
            }
        }
        else
        {
            for ( i = 0; i < pstTELECnf->aucAuts[0]; i++)
            {
                g_AgentUsimAuthCnf.aucFailPara[i] = pPointer[i+1];
            }
            if ( pstTELECnf->aucAuts[0] < 14 )
            {
                for ( ;i < 14; i++ )
                {
                    g_AgentUsimAuthCnf.aucFailPara[i] = 0xFF;
                }
            }
        }
        
        g_AgentUsimAuthCnf.ucCheckRst = AGENT_AUTH_RST_FAILURE;
        g_AgentUsimAuthCnf.ucFailureCause = AGENT_AUTH_FAIL_SQN;
        ucCause = NAS_OM_MM_CAUSE_SYNCH_FAILURE;
        NAS_MM_SndMmcSimAuthFailInfo(NAS_MML_SIM_AUTH_FAIL_SYNC_FAILURE);
    }
    else if (USIMM_AUTH_GSM_OTHER_FAILURE == pUsimMsg->enResult) /* GSM AUTH FAILURE */
    {
        g_AgentUsimAuthCnf.ucCheckRst     = AGENT_AUTH_RST_FAILURE;
        g_AgentUsimAuthCnf.ucFailureCause = USIM_GAS_AUTH_FAIL;
        ucCause = NAS_OM_MM_CAUSE_AUT_GSM_OTHER_FAILURE;
        NAS_MM_SndMmcSimAuthFailInfo(NAS_MML_SIM_AUTH_FAIL_GSM_OTHER_FAILURE);
    }
    else
    {
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_Com_UsimAuthenticationCnfChgFormat:WARNING: ucResult Abnormal");
        /* WUEPS_ASSERT(0); */
        g_AgentUsimAuthCnf.ucCheckRst     = AGENT_AUTH_RST_FAILURE;
        g_AgentUsimAuthCnf.ucFailureCause = USIM_AUTH_FAIL_UNKNOWN;
        ucCause = NAS_OM_MM_CAUSE_AUT_UMTS_OTHER_FAILURE;
        NAS_MM_SndMmcSimAuthFailInfo(NAS_MML_SIM_AUTH_FAIL_UMTS_OTHER_FAILURE);
        NAS_EventReport(WUEPS_PID_MM, NAS_OM_EVENT_AUTH_FAILURE,
                        &ucCause, NAS_OM_EVENT_AUTH_FAIL_LEN);
        return ;
    }

    if(AGENT_AUTH_RST_FAILURE == g_AgentUsimAuthCnf.ucCheckRst)
    {
        NAS_EventReport(WUEPS_PID_MM, NAS_OM_EVENT_AUTH_FAILURE,
                        &ucCause, NAS_OM_EVENT_AUTH_FAIL_LEN);
    }
}


VOS_VOID Mm_ComNetModeChange()
{
    VOS_UINT8                               ucProcessLu;

    VOS_UINT32                          ulLaiChangeFlag;

    ulLaiChangeFlag =  Mm_ComLaiJudge();

    ucProcessLu     = MM_TRUE;

    if (NAS_MML_LOCATION_UPDATE_STATUS_UPDATED == NAS_MML_GetCsUpdateStatus())
    {
        if ( MM_TRUE
         == g_MmGlobalInfo.LuInfo.ucT3212ExpiredFlg )
        {
            g_MmGlobalInfo.ucNtMod = g_MmGlobalInfo.ucNewNtMod;

            NAS_MM_ProcBufferedEmgCallOrLau_RcvSysinfo(ulLaiChangeFlag, MM_IE_LUT_PERIODIC_UPDATING);
            return;
        }

        if ((g_MmGlobalInfo.MsCsInfo.ucOldRac == NAS_MML_GetCurrCampRac())
         && (g_MmGlobalInfo.MsCsInfo.OldLai.PlmnId.ulMcc == g_MmGlobalInfo.MsCsInfo.CurLai.PlmnId.ulMcc)
         && (g_MmGlobalInfo.MsCsInfo.OldLai.PlmnId.ulMnc == g_MmGlobalInfo.MsCsInfo.CurLai.PlmnId.ulMnc)
         && (g_MmGlobalInfo.MsCsInfo.OldLai.ulLac == g_MmGlobalInfo.MsCsInfo.CurLai.ulLac))
        {
            ucProcessLu = MM_FALSE;   /*�յ�ϵͳ��Ϣֻ��С��������ģʽ�ı����跢��LU*/


            Mm_SndRrNasInfoChgReq(MM_NAS_INFO_SYNC_AS_FLG);



            NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                        MM_MMC_LU_RESULT_SUCCESS,
                                        NAS_MML_REG_FAIL_CAUSE_NULL);

            NAS_MM_SndGmmLuResultInd(MMGMM_LU_SUCCESS,
                                     NAS_MML_REG_FAIL_CAUSE_NULL);


            Mm_ComSetMmState(MM_IDLE_NORMAL_SERVICE);
             if ((MM_TIMER_STOP == gstMmTimer[MM_TIMER_T3211].ucTimerStatus)
               && (MM_TIMER_STOP == gstMmTimer[MM_TIMER_T3212].ucTimerStatus)
               && (0x0 != gstMmTimer[MM_TIMER_T3212].ulTimerLen))
             {
                Mm_TimerStart(MM_TIMER_T3212);
             }

            g_MmGlobalInfo.ucNtMod = g_MmGlobalInfo.ucNewNtMod;

        }
    }
    else if (NAS_MML_LOCATION_UPDATE_STATUS_NOT_UPDATED == NAS_MML_GetCsUpdateStatus())
    {
        if ((g_MmGlobalInfo.MsCsInfo.ucOldRac == NAS_MML_GetCurrCampRac())
         && (g_MmGlobalInfo.MsCsInfo.OldLai.PlmnId.ulMcc == g_MmGlobalInfo.MsCsInfo.CurLai.PlmnId.ulMcc)
         && (g_MmGlobalInfo.MsCsInfo.OldLai.PlmnId.ulMnc == g_MmGlobalInfo.MsCsInfo.CurLai.PlmnId.ulMnc)
         && (g_MmGlobalInfo.MsCsInfo.OldLai.ulLac == g_MmGlobalInfo.MsCsInfo.CurLai.ulLac))
        {
            ucProcessLu = MM_FALSE;   /*�յ�ϵͳ��Ϣֻ��С��������ģʽ�ı����跢��LU*/

            NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                        MM_MMC_LU_RESULT_FAILURE,
                                        NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE);

            NAS_MM_SndGmmLuResultInd(MMGMM_LU_FAILURE,
                                     NAS_MML_REG_FAIL_CAUSE_NULL);

            Mm_ComSetMmState(MM_IDLE_ATTEMPTING_TO_UPDATE);


        }
    }
    else
    {
    }

    if (MM_TRUE == ucProcessLu)
    {
        g_MmGlobalInfo.ucNtMod = g_MmGlobalInfo.ucNewNtMod;

        NAS_MM_ProcBufferedEmgCallOrLau_RcvSysinfo(ulLaiChangeFlag, MM_IE_LUT_NORMAL_LU);

    }

}

/*******************************************************************************
  Module   : MM_Update_Revision_Level
  Function : ���¿տ���Ϣ�е�ClassMark IE �� revision level����
  Input    : classmark �ĵ�һ���ֽ�
  Output   : ��
  NOTE     : ��
  Return   : �ޡ�
  History  :
    1. ŷ����  2009.06.19  �¹�����
*******************************************************************************/
VOS_VOID MM_Update_Revision_Level(VOS_UINT8 *pucClassMark)
{
    VOS_INT8                    cVersion;
    VOS_UINT8                   ucRevisionLevel;

    cVersion = NAS_Common_Get_Supported_3GPP_Version(MM_COM_SRVDOMAIN_CS);

    /* R99��ǰ�汾���ϱ� revision level Ϊ GSM PHASE 2*/
    if(PS_PTL_VER_PRE_R99 == cVersion)
    {
        ucRevisionLevel = MM_CLASSMARK_REVISION_LEVEL_GSM_PH2;
    }
    else
    {
        ucRevisionLevel = MM_CLASSMARK_REVISION_LEVEL_R99_OR_LATER;
    }

    *pucClassMark &= 0x9f;
    *pucClassMark |= (VOS_UINT8)(ucRevisionLevel << 5);

    return;
}
VOS_VOID MM_Fill_IE_ClassMark1(
                               VOS_UINT8 *pucClassMark1                           /* ��дClassMark1 IE���׵�ַ                */
                              )
{
    GSM_BAND_SET_UN                     unSysCfgSetGsmBand;

    VOS_UINT8                           ucBandNum;
    NAS_MML_MS_CAPACILITY_INFO_STRU    *pstMsCapability = VOS_NULL_PTR;
    NAS_MML_MS_BAND_INFO_STRU          *pstUserSetBand  = VOS_NULL_PTR;
    NAS_MML_MS_BAND_INFO_STRU          *pstCurrBand     = VOS_NULL_PTR;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurrNetRat;

    enCurrNetRat    = NAS_MML_GetCurrNetRatType();
    pstMsCapability = NAS_MML_GetMsCapability();
    pstUserSetBand  = NAS_MML_GetMsSupportBand();
    pstCurrBand     = NAS_MML_GetCurrBandInfo();
    *pucClassMark1 = pstMsCapability->ucClassmark1;

    NAS_MML_Update_Revision_Level(pucClassMark1);

    if ( VOS_FALSE == NAS_MML_IsNetRatSupported(NAS_MML_NET_RAT_TYPE_GSM))
    {

        /* W only */
        /* bi8 ES IND => 0, A5/1 => 1, RF power capability => 111 */
        *pucClassMark1 &= 0xE0;
        *pucClassMark1 |= 0x0F;
    }
    else
    {
        /*��ȡ��ǰ��С������Ƶ��*/
        unSysCfgSetGsmBand.ulBand = pstCurrBand->unGsmBand.ulBand;
        if ( (NAS_MML_NET_RAT_TYPE_WCDMA == enCurrNetRat)
#if (FEATURE_ON == FEATURE_LTE)
              || (NAS_MML_NET_RAT_TYPE_LTE == enCurrNetRat)
#endif
           )
        {
            unSysCfgSetGsmBand.ulBand = pstUserSetBand->unGsmBand.ulBand;
        }

        /*���ݵ�ǰפ����Ƶ����дRF power capability*/
        if ((unSysCfgSetGsmBand.stBitBand.BandGsm1800)
         || (unSysCfgSetGsmBand.stBitBand.BandGsm1900))
        {
           /*power class 1*/
            *pucClassMark1 &= ~MM_CLASSMARK_RF_POWER_CAP_MASK;
            *pucClassMark1 |= MM_CLASSMARK_RF_POWER_CAP_CLASS1;
        }
        else /* ����Ƶ�� */
        {
            /*power class 4*/
            *pucClassMark1 &= ~MM_CLASSMARK_RF_POWER_CAP_MASK;
            *pucClassMark1 |= MM_CLASSMARK_RF_POWER_CAP_CLASS4;
        }

        /* �����ǰ����פ����W��,���û�����Ϊ��Ƶ�εĴ��� */
        if ( (NAS_MML_NET_RAT_TYPE_WCDMA == enCurrNetRat)
#if (FEATURE_ON == FEATURE_LTE)
              || (NAS_MML_NET_RAT_TYPE_LTE == enCurrNetRat)
#endif
           )
        {
            /* ��ȡ�û����õĿ���С����פ����Ƶ�� */
            NAS_MML_GetSupportBandNum(unSysCfgSetGsmBand.ulBand, &ucBandNum);

            /*���֧�ֶ�Ƶ�λ��߲�֧���κ�Ƶ�Σ�����RF power capability => 111*/
            if ((ucBandNum > 1) || (0 == ucBandNum))
            {
                *pucClassMark1 |= MM_CLASSMARK_RF_POWER_CAP_MASK;
            }
        }
    }
    return;
}



VOS_VOID NAS_MM_GetSupportBandNum(VOS_UINT32 ulBand, VOS_UINT8 *pucBandNum)
{
    /* ucIndicator����������λָʾ */
    VOS_UINT8                           ucIndicator;

    /* ucTotalLen��ʾ��������usBand����bitλ�� */
    VOS_UINT8                           ucTotalLen;

    /* ucBandNum������¼bitֵΪ1�ĸ���������Ϊ�����ķ���ֵ */
    *pucBandNum = 0;
    ucIndicator = 0;
    ucTotalLen  = sizeof(ulBand) * 8;

    /* ͨ��ѭ����λ����ulBand�е�bitֵΪ1���ܸ����������浽ucBandNum�� */
    while (ucIndicator < ucTotalLen)
    {
        if ((ulBand>>ucIndicator) & 0x01)
        {
            (*pucBandNum)++;
        }

        ucIndicator++;
    }

    return;
}

VOS_VOID MM_Fill_IE_ClassMark2(
    VOS_UINT8                          *pClassMark2                           /* ��дClassMark2 IE���׵�ַ                */
)
{
    NAS_MML_Fill_IE_ClassMark2(pClassMark2);

    return;
}
MMCC_CHANNEL_MODE_ENUM_U32 MM_ComGetChanMode(NAS_RR_CHANNEL_MODE_ENUM_U8 enChanMode)
{
    MMCC_CHANNEL_MODE_ENUM_U32   enMmCcChanMode;

    switch(enChanMode)
    {
    case RR_CHAN_MOD_SINGAL_ONLY:
        enMmCcChanMode = MMCC_CHAN_MOD_SIG_ONLY;
        break;
    case RR_CHAN_MOD_FR:
        enMmCcChanMode = MMCC_CHAN_MOD_FR;
        break;
    case RR_CHAN_MOD_EFR:
        enMmCcChanMode = MMCC_CHAN_MOD_EFR;
        break;
    case RR_CHAN_MOD_AFS:
        enMmCcChanMode = MMCC_CHAN_MOD_AMR;
        break;
    case RR_CHAN_MOD_CSD_2400:
        enMmCcChanMode = MMCC_CHAN_MOD_DATA_2400;
        break;
    case RR_CHAN_MOD_CSD_4800:
        enMmCcChanMode = MMCC_CHAN_MOD_DATA_4800;
        break;
    case RR_CHAN_MOD_CSD_9600:
        enMmCcChanMode = MMCC_CHAN_MOD_DATA_9600;
        break;
    case RR_NAS_DATA_CSD_14400:
        enMmCcChanMode = MMCC_CHAN_MOD_DATA_14400;
        break;
    case RR_CHAN_MOD_HR:
        enMmCcChanMode = MMCC_CHAN_MOD_HR;
        break;
    case RR_CHAN_MOD_AHR:
        enMmCcChanMode = MMCC_CHAN_MOD_AHR;
        break;
    case RR_CHAN_MOD_H24:
        enMmCcChanMode = MMCC_CHAN_MOD_SIG_ONLY;
        break;
    case RR_CHAN_MOD_H48:
        enMmCcChanMode = MMCC_CHAN_MOD_SIG_ONLY;
        break;

    case RR_CHAN_MOD_WFS:
        enMmCcChanMode = MMCC_CHAN_MOD_AMRWB;
        break;

    default :
        NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_WARNING,
                            "MM_ComGetChanMode:  Receive GAS_RR_CHAN_MOD_IND, parameter is error.");
        enMmCcChanMode = MMCC_CHAN_MOD_SIG_ONLY;
        break;
    }

    return enMmCcChanMode;
}


VOS_VOID Mm_ComSetMmState(VOS_UINT32 ulState)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurrentNetRatType;

    enCurrentNetRatType = NAS_MML_GetCurrNetRatType();

    if (( VOS_FALSE == NAS_MML_GetCsAttachAllowFlg())
     && (MM_IDLE_NORMAL_SERVICE == ulState))
    {
        g_MmGlobalInfo.ucState = MM_IDLE_LIMITED_SERVICE;
        g_MmSubLyrShare.MmShare.ucCsAttachState = MM_STATUS_DETACHED;
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "Mm_ComSetMmState:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
    }
    else
    {
        g_MmGlobalInfo.ucState = (VOS_UINT8)ulState;
    }

    NAS_MM_LogMmStateInfo(g_MmGlobalInfo.ucState);

    switch(ulState)
    {
        case MM_STATE_NULL:
        case MM_IDLE_NORMAL_SERVICE:
        case MM_IDLE_LIMITED_SERVICE:
        case MM_IDLE_ATTEMPTING_TO_UPDATE:
        case MM_IDLE_LOCATION_UPDATE_NEEDED:
        case MM_IDLE_NO_IMSI:
        case MM_IDLE_NO_CELL_AVAILABLE:
            /* �ظ�MMCMM_ATTACH_CNF�Ĳ���,����ȴ�ATTACH�����־λ */
            if (MM_WAIT_CS_ATTACH == (g_MmGlobalInfo.stAttachInfo.enAttachType
                                    & MM_WAIT_CS_ATTACH))
            {
                NAS_MM_SndMmcAttachCnf();
            }
            g_MmGlobalInfo.stAttachInfo.enAttachType = MM_WAIT_NULL_ATTACH;

            /* L��MM�����ϱ�����״̬ */
            if (NAS_MML_NET_RAT_TYPE_LTE != enCurrentNetRatType)
            {
                Mm_SndMmcSvcStaInd();
            }

            break;

        default:
            break;
    }


    return;
}

/*****************************************************************************
 �� �� ��  : MM_LocalEndAllCalls
 ��������  : �����ͷ����еĺ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2008��7��02��
    ��    ��   : ���� 49431
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID MM_LocalEndAllCalls(VOS_VOID)
{
    Mm_RcvRrmmRelInd();
    Mm_SndRrRelReq(RRC_CELL_UNBARRED);
    return;
}
/*******************************************************************************
  MODULE   : Mm_GetServiceStatus
  FUNCTION : ��ȡ��ǰ�ķ���״̬��ԭ��ֵ
  INPUT    : VOS_VOID
  OUTPUT   : VOS_VOID
  RETURN   : VOS_VOID
  NOTE     :
  HISTORY  :
     1.��    ��  : 2009��1��16��
       ��    ��  :
       �޸�����  :
*******************************************************************************/
VOS_VOID Mm_GetServiceStatus(VOS_UINT32 *pulCsServiceStatus, VOS_UINT32 *pulCsCause)
{
    *pulCsServiceStatus = g_MmGlobalInfo.ucMmServiceState;
    *pulCsCause = g_MmGlobalInfo.usCauseVal;
}


VOS_VOID Mm_ComCheckDelayDetach()
{
    MM_MSG_IMSI_DETACH_IND_STRU     Msg;

    if (NAS_MML_MS_MODE_PS_ONLY == NAS_MML_GetMsMode())
    {
        Mm_TimerStop(MM_TIMER_T3212);

        if (MM_IDLE_NORMAL_SERVICE == g_MmGlobalInfo.ucState)
        {
            /*Detach CS*/
            if ((MM_NET_MODE_I == g_MmGlobalInfo.ucNtMod)
            && ((GMM_STATUS_ATTACHED == g_MmSubLyrShare.GmmShare.ucAttachSta)))
            {
                Mm_SndMmcDeRegisterInit();
            }
            else if (MM_ATT_NEED == g_MmGlobalInfo.ucAttFlg)
            {
                g_MmGlobalInfo.ucDetachType = MM_DETACH_CS_NORMAL;

                NAS_MM_FillMobileID(&(Msg.MmIeMobileId.MobileID));

                Mm_ComMsgImsiDetachIniSndOfEstReq(&Msg);                        /* ��д��Ϣ,������Ϣ                        */

                Mm_TimerStart(MM_TIMER_PROTECT_SIGNALLING);                     /* ��������TIMER                            */

                g_MmGlobalInfo.ucPreState = g_MmGlobalInfo.ucState;
                Mm_ComSetMmState(WAIT_FOR_RR_CONNECTION_IMSI_DETACH);
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL,
                "Mm_ComCheckDelayDetach:NORMAL: STATUS is WAIT_FOR_RR_CONNECTION_IMSI_DETACH");
                g_MmGlobalInfo.ucMmServiceState = MM_DETACHED;
            }
            else
            {
                g_MmGlobalInfo.ucPreState = g_MmGlobalInfo.ucState;
                Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
                g_MmGlobalInfo.ucMmServiceState = MM_DETACHED;
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL,
                "Mm_ComCheckDelayDetach:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");

            }
        }
        else if ((MM_IDLE_ATTEMPTING_TO_UPDATE == g_MmGlobalInfo.ucState)
              || (MM_IDLE_LOCATION_UPDATE_NEEDED == g_MmGlobalInfo.ucState))
        {
            Mm_TimerStop(MM_TIMER_T3211);
            Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
            g_MmGlobalInfo.ucMmServiceState = MM_DETACHED;
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL,
                   "Mm_ComCheckDelayDetach:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");

        }
        else
        {
        }

        /*����attach not allow ��־*/
        NAS_MML_SetCsAttachAllowFlg(VOS_FALSE);
    }
}


VOS_UINT32 Mm_ComJudgeCsServExist(VOS_VOID)
{
    VOS_UINT32                          ulRet = MM_CS_SERV_NONE_EXIST;

    if(  ( MM_CONST_NUM_7 <
       g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulTransactionId)
       && ( MM_CONST_NUM_15 >
       g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulTransactionId)
       && ( MM_TRUE ==
       g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg ) )
    {
        if ( MMCC_EMERGENCY_CALL ==
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulCallType)
        {
            ulRet = MM_CS_SERV_EMERGENCY_CALL_EXIST;
        }
        else
        {
            ulRet = MM_CS_SERV_NORMAL_CALL_EXIST;
        }
    }

    else if ( MM_TRUE ==
         g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg )
    {
        ulRet = MM_CS_SERV_SS_EXIST;
    }

    else if ( MM_TRUE ==
         g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg )
    {
        ulRet = MM_CS_SERV_SMS_EXIST;
    }
    else if (MM_PAGING_PLMNSRCH_TRUE == g_MmSubLyrShare.MmShare.ucPagingPlmnSrch)
    {
        ulRet = MM_CS_SERV_MT_EXIST;
    }
    else
    {
        ;
    }

    return ulRet;
}





VOS_UINT32 NAS_MM_IsExistBufferService(VOS_VOID)
{
    if (MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg)
    {
        return VOS_TRUE;
    }

    if (MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg)
    {
        return VOS_TRUE;
    }
    if (MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg)
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT8  MM_GetSignalingStatus(VOS_VOID)
{
    /* ��ΪMMC��MM������������Ϣ��֪ͨ�ǲ����첽��Ϣ�������ڷ���PLMN����ǰ��Ҫ�ж�MM�ĵ�ǰ״̬��
    ��ע�����ȥע�������,������������ */
    if((WAIT_FOR_RR_CONNECTION_MM_CONNECTION == g_MmGlobalInfo.ucState)
        || (WAIT_FOR_RR_CONNECTION_LOCATION_UPDATING == g_MmGlobalInfo.ucState)
        || (WAIT_FOR_RR_CONNECTION_IMSI_DETACH == g_MmGlobalInfo.ucState)
        || (WAIT_FOR_REESTABLISH_WAIT_FOR_REEST_REQ == g_MmGlobalInfo.ucState)
        || (WAIT_FOR_REESTABLISH_WAIT_FOR_EST_CNF == g_MmGlobalInfo.ucState))
    {
        return MMC_MM_SIGNALING_STATUS_PRESENT;
    }
    else if (VOS_TRUE == NAS_MM_IsExistBufferService())
    {
        return MMC_MM_SIGNALING_STATUS_PRESENT;
    }
    else
    {
        return MMC_MM_SIGNALING_STATUS_ABSENT;
    }
}


VOS_UINT8  NAS_MM_CheckIfMmIsEstablishRRConnection()
{
    /* ��ΪMMC��MM������������Ϣ��֪ͨ�ǲ����첽��Ϣ�������ڷ���PLMN����ǰ��Ҫ�ж�MM�ĵ�ǰ״̬��
    ��ע�����ȥע�������,������������ */
    if( (WAIT_FOR_RR_CONNECTION_MM_CONNECTION == g_MmGlobalInfo.ucState)
     || (WAIT_FOR_RR_CONNECTION_LOCATION_UPDATING == g_MmGlobalInfo.ucState)
     || (WAIT_FOR_RR_CONNECTION_IMSI_DETACH == g_MmGlobalInfo.ucState)
     || (WAIT_FOR_REESTABLISH_WAIT_FOR_REEST_REQ == g_MmGlobalInfo.ucState)
     || (WAIT_FOR_REESTABLISH_WAIT_FOR_EST_CNF == g_MmGlobalInfo.ucState))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}
/* �������� */

VOS_UINT8 NAS_MM_RetryCurrentProcedureCheck(
    RRC_REL_CAUSE_ENUM_UINT32           ulRelCause,
    RRC_RRC_CONN_STATUS_ENUM_UINT32     ulRrcConnStatus
)
{
    /*24.008, 4.4.4.9 f1/4.5.1.2 a1:
      a.1) RR connection failure in Iu mode (i.e. RRC connection release) with,
      for example, cause "Normal", "User inactivity" or "Directed signalling
      connection re-establishment" (see 3GPP TS 25.331 [32c] and 3GPP TS 44.118
      [111])
      The MM connection establishment procedure shall be initiated again, if
      the following conditions apply:
      i) The original MM connection establishment was initiated over an existing
      RRC connection; and
      ii) No SECURITY MODE COMMAND message and no Non-Access Stratum (NAS) mess-
      ages relating to the CS signalling connection (e.g. CS authentication
      procedures, see subclause 4.3.2), were received after the CM SERVICE REQU-
      EST message was transmitted.
      NOTE 1:  The RRC connection release cause that triggers the re-initiation
      of the MM connection establishment procedure is implementation specific.*/

    VOS_UINT8                           ucRst;

    ucRst = VOS_FALSE;

    if (NAS_MML_NET_RAT_TYPE_WCDMA == NAS_MML_GetCurrNetRatType())
    {
        if (   (RRC_RRC_CONN_STATUS_ABSENT == ulRrcConnStatus)
            && (VOS_TRUE == g_MmGlobalInfo.stMmLinkCtrl.ucExistedRrcConn)
            && (VOS_FALSE
                    == g_MmGlobalInfo.stMmLinkCtrl.ucSmcOrFirstNasMsgRcvdFlg)
            && (g_MmGlobalInfo.ucProc
                    == g_MmGlobalInfo.stMmLinkCtrl.ucCurrentProc))
        {
            if (  (RRC_REL_CAUSE_RR_NORM_EVENT == ulRelCause)
                ||(RRC_REL_CAUSE_RR_DRIECT_SIGN_CONN_EST == ulRelCause)
                ||(RRC_REL_CAUSE_RR_USER_INACT == ulRelCause))
            {
                ucRst = VOS_TRUE;
            }
        }
    }

    /* �ж���Ϻ����MM��·���Ʊ��� */
    NAS_MM_ClearMmLinkCtrlStru();

    return ucRst;
}
VOS_VOID NAS_MM_RetryMmConnectionProc(
    VOS_UINT8                           ucMMConnEstingPD
)
{
    NAS_MM_CONN_CTRL_ENUM_UINT8         ucMmConnCtrl;

    switch (ucMMConnEstingPD)
    {
        case MM_IE_PD_CALL_CONTROL:
            ucMmConnCtrl = MM_CONN_CTRL_CC;
            break;
        case MM_IE_PD_NON_CALL_RLT_SS_MSG:
            ucMmConnCtrl = MM_CONN_CTRL_SS;
            break;
        case MM_IE_PD_SMS_MSG:
            ucMmConnCtrl = MM_CONN_CTRL_SMS;
            break;
        default:
            ucMmConnCtrl = MM_CONN_CTRL_BUTT;
            NAS_WARNING_LOG1(WUEPS_PID_MM, "NAS_MM_RetryMmConnectionProc: unexpected ucMMConnEstingPD received: ", ucMMConnEstingPD);
            break;
    }

    if (MM_CONN_CTRL_BUTT != ucMmConnCtrl)
    {
        NAS_MM_EstMmConnection(ucMmConnCtrl);
    }

    return;
}


VOS_VOID NAS_MM_GetPDFromConnCtrlEnum(
    NAS_MM_CONN_CTRL_ENUM_UINT8         ucMmConnCtrl,
    VOS_UINT8                           *pucMMConnEstingPD
)
{
    switch (ucMmConnCtrl)
    {
        case MM_CONN_CTRL_CC:
            *pucMMConnEstingPD = MM_IE_PD_CALL_CONTROL;
            break;
        case MM_CONN_CTRL_SS:
            *pucMMConnEstingPD = MM_IE_PD_NON_CALL_RLT_SS_MSG;
            break;
        case MM_CONN_CTRL_SMS:
            *pucMMConnEstingPD = MM_IE_PD_SMS_MSG;
            break;
        default:
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "NAS_MM_GetPDFromConnCtrlEnum: unexpected ucMmConnCtrl.");
            break;
    }

    return;
}



VOS_VOID NAS_MM_EstMmConnection(
    NAS_MM_CONN_CTRL_ENUM_UINT8         ucMmConnCtrl
)
{
    MM_MSG_CM_SVC_REQ_STRU              stCmSvcReq;
    VOS_UINT16                          usMsgSize;
    VOS_UINT8                           *pucCmSvcReq;
    VOS_UINT8                           ucCMSvcType;
    VOS_UINT32                          ulRrcEstCause;

    /* ������Ϣ�������ӽ������������������ѡ�����LU�ı䣬��LU��������Խ����ش� */
    NAS_MM_UpdateConnCtrlInfo(ucMmConnCtrl);

    NAS_MM_GetCMSvcType(ucMmConnCtrl, &ucCMSvcType);

    /* ���CM SERVICE REQUEST�ṹ��             */
    Mm_ComFillInCMSncReq(ucCMSvcType, MM_FALSE, &stCmSvcReq);

    /* ���CM SERVICE REQUEST ��Ϣ              */
    pucCmSvcReq = Mm_ComMsgCmSvcReqOfRrEstReq(&stCmSvcReq, &usMsgSize);
    if (VOS_NULL_PTR == pucCmSvcReq)
    {
        NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_EstMmConnection:WARNING: MAKE MSG CM SERVICE REQUEST ERROR!");
        return;
    }
    else
    {
        NAS_MM_GetRrcEstCause(ucMmConnCtrl, &ulRrcEstCause);

        Mm_SndRrEstReq(ulRrcEstCause, MM_FALSE, usMsgSize, pucCmSvcReq);

        MM_MEM_FREE(VOS_MEMPOOL_INDEX_NAS, pucCmSvcReq);
    }

    NAS_MM_GetPDFromConnCtrlEnum(ucMmConnCtrl, &g_MmGlobalInfo.ucMMConnEstingPD);

    /* ��������TIMER                            */
    Mm_TimerStart(MM_TIMER_PROTECT_SIGNALLING);
    /* ֪ͨMMC��CS���ҵ���Ѿ�����              */
    Mm_SndMmcCmSvcInd(MM_CS_SERV_EXIST);
    NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_ESTING);

    g_MmGlobalInfo.ucPreState = g_MmGlobalInfo.ucState;
    Mm_ComSetMmState(WAIT_FOR_RR_CONNECTION_MM_CONNECTION);
    NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_EstMmConnection:NORMAL: STATUS is WAIT_FOR_RR_CONNECTION_MM_CONNECTION");

    return;
}
VOS_VOID NAS_MM_UpdateConnCtrlInfo(
    NAS_MM_CONN_CTRL_ENUM_UINT8         ucMmConnCtrl
)
{
    g_MmGlobalInfo.ConnCtrlInfo[ucMmConnCtrl].RcvXXEstReq.ucFlg = MM_TRUE;
    g_MmGlobalInfo.ConnCtrlInfo[ucMmConnCtrl].ucRat = NAS_MML_GetCurrNetRatType();
    NAS_MML_SetCsServiceBufferStatusFlg(VOS_TRUE);

    switch (ucMmConnCtrl)
    {
        case MM_CONN_CTRL_CC:
            g_MmGlobalInfo.ConnCtrlInfo[ucMmConnCtrl].RcvXXEstReq.ulCallPri
                = g_MmCcEstReq.ulCallPri;
            g_MmGlobalInfo.ConnCtrlInfo[ucMmConnCtrl].RcvXXEstReq.ulCallType
                = g_MmCcEstReq.ulCallType;
            g_MmGlobalInfo.ConnCtrlInfo[ucMmConnCtrl].RcvXXEstReq.ulTransactionId
                = g_MmCcEstReq.ulTransactionId;
            g_MmGlobalInfo.ConnCtrlInfo[ucMmConnCtrl].RcvXXEstReq.ulCallMode
                = g_MmCcEstReq.ulCallMode;

            g_MmGlobalInfo.ConnCtrlInfo[ucMmConnCtrl].ucMMConnEstingTI
                = (VOS_UINT8)g_MmCcEstReq.ulTransactionId;

            if (MMCC_MO_NORMAL_CALL == g_MmCcEstReq.ulCallType)
            {
                g_MmGlobalInfo.ConnCtrlInfo[ucMmConnCtrl].ucEstingCallTypeFlg
                    = MM_FALSE;
            }
            else
            {
                g_MmGlobalInfo.ConnCtrlInfo[ucMmConnCtrl].ucEstingCallTypeFlg
                    = MM_TRUE;
            }
            break;

        case MM_CONN_CTRL_SS:
            g_MmGlobalInfo.ConnCtrlInfo[ucMmConnCtrl].RcvXXEstReq.ulCallPri
                = 0;
            g_MmGlobalInfo.ConnCtrlInfo[ucMmConnCtrl].RcvXXEstReq.ulCallType
                = 0;
            g_MmGlobalInfo.ConnCtrlInfo[ucMmConnCtrl].RcvXXEstReq.ulTransactionId
                = g_MmSsEstReq.ulTi;

            g_MmGlobalInfo.ConnCtrlInfo[ucMmConnCtrl].ucMMConnEstingTI
                = (VOS_UINT8)g_MmSsEstReq.ulTi;

            break;

        case MM_CONN_CTRL_SMS:
            g_MmGlobalInfo.ConnCtrlInfo[ucMmConnCtrl].RcvXXEstReq.ulCallPri
                = 0;
            g_MmGlobalInfo.ConnCtrlInfo[ucMmConnCtrl].RcvXXEstReq.ulCallType
                = 0;
            g_MmGlobalInfo.ConnCtrlInfo[ucMmConnCtrl].RcvXXEstReq.ulTransactionId
                = g_MmSmsEstReq.ulTi;
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].ucMMConnEstingTI
                = (VOS_UINT8)g_MmSmsEstReq.ulTi;
            break;

        default:
            break;
    }

    return;
}



VOS_VOID NAS_MM_GetCMSvcType(
    NAS_MM_CONN_CTRL_ENUM_UINT8         ucMmConnCtrl,
    VOS_UINT8                           *pucCmSrvType
)
{
    switch (ucMmConnCtrl)
    {
        case MM_CONN_CTRL_CC:
            if (MMCC_MO_NORMAL_CALL == g_MmCcEstReq.ulCallType)
            {
                *pucCmSrvType = MM_IE_CM_SVC_TYPE_MO_CALL_EST;
            }
            else
            {
                *pucCmSrvType = MM_IE_CM_SVC_TYPE_EMG_CALL_EST;
            }
            break;

        case MM_CONN_CTRL_SS:
            *pucCmSrvType = MM_IE_CM_SVC_TYPE_SS_ACT;
            break;

        case MM_CONN_CTRL_SMS:
            *pucCmSrvType = MM_IE_CM_SVC_TYPE_SMS;
            break;

        default:
            *pucCmSrvType = 0;
            break;
    }

    return;
}



VOS_VOID NAS_MM_GetRrcEstCause(
    NAS_MM_CONN_CTRL_ENUM_UINT8         ucMmConnCtrl,
    VOS_UINT32                          *pulRrcEstCause
)
{
    switch (ucMmConnCtrl)
    {
        case MM_CONN_CTRL_CC:
            if (MMCC_MO_NORMAL_CALL == g_MmCcEstReq.ulCallType)
            {
                *pulRrcEstCause = RRC_EST_CAUSE_ORIGIN_CONVERSAT_CALL;
            }
            else
            {
                *pulRrcEstCause = RRC_EST_CAUSE_EMERGENCY_CALL;
            }
            break;

        case MM_CONN_CTRL_SS:
            *pulRrcEstCause = RRC_EST_CAUSE_ORIGIN_HIGH_PRIORITY_SIGNAL;
            break;

        case MM_CONN_CTRL_SMS:
            *pulRrcEstCause = RRC_EST_CAUSE_ORIGIN_LOW_PRIORITY_SIGNAL;
            break;

        default:
            *pulRrcEstCause = RRC_EST_CAUSE_ORIGIN_CONVERSAT_CALL;
            break;
    }

    return;
}



VOS_VOID NAS_MM_UpdateMmLinkCtrlStru(VOS_VOID)
{
    NAS_MML_CONN_STATUS_INFO_STRU       *pstConnStatus;

    pstConnStatus   = NAS_MML_GetConnStatus();

    g_MmGlobalInfo.stMmLinkCtrl.ucCurrentProc = g_MmGlobalInfo.ucProc;
    g_MmGlobalInfo.stMmLinkCtrl.ucSmcOrFirstNasMsgRcvdFlg = VOS_FALSE;

    /* ��鵱ǰ�����Ƿ����ѽ�����RRC�����Ϸ��� */
    if (VOS_FALSE == pstConnStatus->ucRrcStatusFlg)
    {
        g_MmGlobalInfo.stMmLinkCtrl.ucExistedRrcConn = VOS_FALSE;
    }
    else
    {
        g_MmGlobalInfo.stMmLinkCtrl.ucExistedRrcConn = VOS_TRUE;
    }

    return;
}
VOS_VOID NAS_MM_ClearMmLinkCtrlStru( VOS_VOID )
{
    g_MmGlobalInfo.stMmLinkCtrl.ucCurrentProc = MM_NULL_PROC;
    g_MmGlobalInfo.stMmLinkCtrl.ucExistedRrcConn = VOS_FALSE;
    g_MmGlobalInfo.stMmLinkCtrl.ucSmcOrFirstNasMsgRcvdFlg = VOS_FALSE;
}


VOS_VOID NAS_MM_ClearConnCtrlInfo(VOS_UINT8 ucMMConnEstingPD)
{
    if(MM_IE_PD_CALL_CONTROL == ucMMConnEstingPD)
    {
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg
            = MM_FALSE;
    }
    else if(MM_IE_PD_SMS_MSG == ucMMConnEstingPD)
    {
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg
            = MM_FALSE;
    }
    else if(MM_IE_PD_NON_CALL_RLT_SS_MSG == ucMMConnEstingPD)
    {
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg
            = MM_FALSE;
    }
    else
    {
    }

    NAS_MM_UpdateCsServiceBufferStatusFlg();
    
    return;
}


VOS_VOID NAS_MM_ClearConnCtrlInfoExcEmgCall(VOS_UINT8 ucMMConnEstingPD)
{
    if ((MM_IE_PD_CALL_CONTROL == ucMMConnEstingPD)
     && (MMCC_EMERGENCY_CALL != g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulCallType))
    {
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg = MM_FALSE;
        NAS_MML_SetCsServiceBufferStatusFlg(VOS_FALSE);
        return;
    }

    if (MM_IE_PD_SMS_MSG == ucMMConnEstingPD)
    {
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg = MM_FALSE;
        NAS_MML_SetCsServiceBufferStatusFlg(VOS_FALSE);
        return;
    }

    if (MM_IE_PD_NON_CALL_RLT_SS_MSG == ucMMConnEstingPD)
    {
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg = MM_FALSE;
        NAS_MML_SetCsServiceBufferStatusFlg(VOS_FALSE);
    }

    return;
}

VOS_VOID NAS_MM_ProcEstCnfSuccessCauseAtState12(VOS_VOID)
{
    VOS_UINT32                          ulIsTestCard;
    VOS_UINT8                           ucSupportCsServiceFlg;                  /* �Ƿ�֧��cs����ҵ�������ͨ������vpҵ��VOS_TRUE:֧��cs����ҵ��VOS_FALSE:��֧��cs����ҵ��*/

    ulIsTestCard          = NAS_USIMMAPI_IsTestCard();
    ucSupportCsServiceFlg = NAS_MML_GetSupportCsServiceFLg();    
    
    g_MmGlobalInfo.ucPreState   = g_MmGlobalInfo.ucState;

    if ( (NO_MM_CONN_ESTING != g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI)
       ||(NO_MM_CONN_ESTING != g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].ucMMConnEstingTI)
       ||(NO_MM_CONN_ESTING != g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].ucMMConnEstingTI))
    {
        Mm_TimerStart(MM_TIMER_T3230);
        Mm_ComSetMmState(WAIT_FOR_OUTGOING_MM_CONNECTION);

        Mm_SndMmcCmSvcInd(MM_CS_SERV_EXIST);

        NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_ProcEstCnfSuccessCauseAtState12:NORMAL: STATUS is  WAIT_FOR_OUTGOING_MM_CONNECTION");
    }
    else
    {
        /* ������Ѱ����������ӽ���                 */
        g_MmGlobalInfo.ucStaAfterWaitForNwkCmd = g_MmGlobalInfo.ucStaOfRcvXXEstReq;
        Mm_ComSaveProcAndCauseVal( MM_MM_CONN_PROC, NAS_MML_REG_FAIL_CAUSE_NULL );
        Mm_ComSetMmState(WAIT_FOR_NETWORK_COMMAND);
        NAS_NORMAL_LOG(WUEPS_PID_MM, "NAS_MM_ProcEstCnfSuccessCauseAtState12:NORMAL: STATUS is  WAIT_FOR_NETWORK_COMMAND");

        if ( (VOS_TRUE  == ulIsTestCard)
          && (VOS_FALSE == ucSupportCsServiceFlg) )
        {
            gstMmTimer[MM_TIMER_T3240].ulTimerLen = MM_TIMER_MAX_VALUE;
            Mm_TimerStart( MM_TIMER_T3240 );
            gstMmTimer[MM_TIMER_T3240].ulTimerLen = MM_TIMER_T3240_VALUE;            
        }
        else
        {
            /*A32D12629==>�ڴ��ڼ���ܷ���Ƶ���ض��壬ʱ��Ϊ50�룬������Ӷ�ʱ������*/
            gstMmTimer[MM_TIMER_T3240].ulTimerLen += 50000;            
            Mm_TimerStart( MM_TIMER_T3240 );
            gstMmTimer[MM_TIMER_T3240].ulTimerLen -= 50000;
        }
        /*<==A32D12629*/
    }

    g_MmGlobalInfo.ucCsSigConnFlg = MM_CS_SIG_CONN_PRESENT;
    NAS_INFO_LOG(WUEPS_PID_MM, "NAS_MM_ProcEstCnfSuccessCauseAtState12:INFO: MmCsSigConnFlg is MM_CS_SIG_CONN_PRESENT");
    Mm_SndMmcRrConnInfInd( MMC_MM_RR_CONN_ESTED );
    return;
}


VOS_VOID NAS_MM_InformCmLayerEstFail(
    RRC_NAS_EST_RESULT_ENUM_UINT32      ulResult
)
{
    if ( NO_MM_CONN_ESTING != g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI )
    {
        /* CC�����RR���ӵĽ���, ֪ͨCC����ʧ��             */
        if (VOS_TRUE == NAS_MM_IsAbleRecover_EstCnfFailResult(ulResult))
        {
            Mm_SndCcRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI,
                           NAS_MMCM_REL_CAUSE_AS_REJ_LOW_LEVEL_FAIL);
        }
        else
        {
            Mm_SndCcRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI,
                       NAS_MMCM_REL_CAUSE_AS_REJ_OTHER_CAUSES);
        }

        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI = NO_MM_CONN_ESTING;
    }
    else if ( NO_MM_CONN_ESTING !=
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].ucMMConnEstingTI )
    {
        /* SS�����RR���ӵĽ��� ��֪ͨSS,MM���ӽ���ʧ��      */
        if (VOS_TRUE == NAS_MM_IsAbleRecover_EstCnfFailResult(ulResult))
        {
            Mm_SndSsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].ucMMConnEstingTI,
                           NAS_MMCM_REL_CAUSE_AS_REJ_LOW_LEVEL_FAIL);
        }
        else
        {
            Mm_SndSsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].ucMMConnEstingTI,
                           NAS_MMCM_REL_CAUSE_AS_REJ_OTHER_CAUSES);
        }

        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].ucMMConnEstingTI = NO_MM_CONN_ESTING;
    }
    else if ( NO_MM_CONN_ESTING !=
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].ucMMConnEstingTI )
    {
        /* SMS�����RR���ӵĽ��� ,֪ͨSMS,MM���ӽ���ʧ��      */
        if (VOS_TRUE == NAS_MM_IsAbleRecover_EstCnfFailResult(ulResult))
        {
            Mm_SndSmsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].ucMMConnEstingTI,
                           NAS_MMCM_REL_CAUSE_AS_REJ_LOW_LEVEL_FAIL);
        }
        else
        {
            Mm_SndSmsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].ucMMConnEstingTI,
                           NAS_MMCM_REL_CAUSE_AS_REJ_OTHER_CAUSES);
        }

        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].ucMMConnEstingTI = NO_MM_CONN_ESTING;
    }
    else
    {
        /* û�����ڽ�����MM���ӣ�������             */
    }


    return;
}


VOS_VOID NAS_MM_ProcEstCnfOtherFailCauseAtState12(
    RRC_NAS_EST_RESULT_ENUM_UINT32      ulResult
)
{
    VOS_UINT8               ucTiValidFlg;

    NAS_MM_InformCmLayerEstFail(ulResult);

    g_MmGlobalInfo.ucPreState = g_MmGlobalInfo.ucState;
    Mm_ComSetMmState(g_MmGlobalInfo.ucStaOfRcvXXEstReq);
    NAS_NORMAL_LOG1(WUEPS_PID_MM, "NAS_MM_ProcEstCnfOtherFailCauseAtState12:NORMAL: ucState = ",
                    g_MmGlobalInfo.ucState);

    /* ��Ϊ����Mm_ComChkLu���Ƿ�ǰ������̬���ж�,���������ӽ���ʧ��ʱ,
       ����ֱ�ӵ��ô˺���,��Ҫ����NAS_MM_CheckLuAfterCsConnectRelease���д��� */
    if ( MM_FALSE == NAS_MM_ProcessLuAfterCsConnectNotExist() )
    {
        if ((MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC]
                      .RcvXXEstReq.ucFlg)
            ||(MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS]
                      .RcvXXEstReq.ucFlg)
            ||(MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS]
                      .RcvXXEstReq.ucFlg))
        {
            ucTiValidFlg = Mm_ComTiChk();
            if ( MM_FALSE == ucTiValidFlg )
            {
                Mm_SndMmcRrConnInfInd( MMC_MM_RR_CONN_ABSENT );         /* RR���Ӳ�����                             */
                NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_NOT_EXIST);
            }
            else
            {
                Mm_ComSigRelForDelayMmConnRej();
            }
        }
        else
        {
            Mm_SndMmcRrConnInfInd( MMC_MM_RR_CONN_ABSENT );             /* RR���Ӳ�����                             */
            NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_NOT_EXIST);
        }
        Mm_ComCheckDelayMmConn( MM_FALSE );

        Mm_ComCheckDelayDetach();
    }
    else
    {
        if ( MM_TRUE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg )
        {                                                               /* �еȴ���CC��������Ҫ������MM����         */
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
                        RcvXXEstReq.ucFlg = MM_FALSE;

            if (VOS_TRUE == NAS_MM_IsAbleRecover_EstCnfFailResult(ulResult))
            {
                Mm_SndCcRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI,
                               NAS_MMCM_REL_CAUSE_AS_REJ_LOW_LEVEL_FAIL);
            }
            else
            {
                Mm_SndCcRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI,
                               NAS_MMCM_REL_CAUSE_AS_REJ_OTHER_CAUSES);
            }
        }
        if ( MM_TRUE ==
                g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].
                                        RcvXXEstReq.ucFlg )
        {                                                               /* �еȴ���CC��������Ҫ������MM����         */
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].
                                    RcvXXEstReq.ucFlg = MM_FALSE;

            if (VOS_TRUE == NAS_MM_IsAbleRecover_EstCnfFailResult(ulResult))
            {
                Mm_SndSmsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ulTransactionId,
                               NAS_MMCM_REL_CAUSE_AS_REJ_LOW_LEVEL_FAIL);
            }
            else
            {
                Mm_SndSmsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ulTransactionId,
                               NAS_MMCM_REL_CAUSE_AS_REJ_OTHER_CAUSES);
            }
        }
        if ( MM_TRUE ==
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].
                                    RcvXXEstReq.ucFlg )
        {                                                               /* �еȴ���CC��������Ҫ������MM����         */
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].
                                    RcvXXEstReq.ucFlg = MM_FALSE;

            if (VOS_TRUE == NAS_MM_IsAbleRecover_EstCnfFailResult(ulResult))
            {
                Mm_SndSsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ulTransactionId,
                               NAS_MMCM_REL_CAUSE_AS_REJ_LOW_LEVEL_FAIL);
            }
            else
            {
                Mm_SndSsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ulTransactionId,
                               NAS_MMCM_REL_CAUSE_AS_REJ_OTHER_CAUSES);
            }
        }
    }

    if ( g_T3211Flag )
    {
        Mm_TimerStart(MM_TIMER_T3211);
        g_T3211Flag = 0;
    }

    return;
}
VOS_VOID NAS_MM_ProcEstCnfCauseAtState12(
    RRC_NAS_EST_RESULT_ENUM_UINT32      ulResult
)
{
    switch (ulResult)
    {
        case RRC_EST_SUCCESS:
        case RRC_EST_PRESENT:
            NAS_MM_ProcEstCnfSuccessCauseAtState12();
            break;
        case RRC_EST_RJ_CONGEST:                                                /* RRC Connection Reject: Congestion        */
        case RRC_EST_RJ_UNSPEC:                                                 /* RRC Connection Reject: Unspecified       */
        case RRC_EST_EST_CONN_FAIL:                                             /* RR connection establish failure          */
        case RRC_EST_RJ_T3122_RUNNING:    
        case RRC_EST_RJ_INTER_RAT:
        case RRC_EST_IMMEDIATE_ASSIGN_REJECT:
        case RRC_EST_RANDOM_ACCESS_REJECT:

            NAS_MM_ProcEstCnfOtherFailCauseAtState12(ulResult);

            break;
        case RRC_EST_OTHER_ACCESS_BARRED:
            /* ��Ҫ�������ޣ����ñ�־λ */
            NAS_MM_ProcEstCnfOtherFailCauseAtState12(ulResult);

            break;
        case RRC_EST_PAGING_ACCESS_BARRED:
            /* ��Ҫ�������ޣ����ñ�־λ */
            NAS_MML_SetPsRestrictPagingFlg(VOS_TRUE);

            NAS_MM_ProcEstCnfOtherFailCauseAtState12(ulResult);
            break;
        case RRC_EST_ACCESS_BARRED:
            /* �ǽ������в���������ע���Ѱ����� */
            if (MMCC_EMERGENCY_CALL !=  g_MmCcEstReq.ulCallType)
            {
                /* �������ޣ����ñ�־λ */
                NAS_MML_SetCsRestrictPagingFlg(VOS_TRUE);
                NAS_MML_SetCsRestrictRegisterFlg(VOS_TRUE);
            }

            NAS_MM_ProcEstCnfOtherFailCauseAtState12(ulResult);
            break;
        case RRC_EST_ESTING:
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_Cell_S12_E32:WARNING: RR CONNECTION ESTING !");
            break;
        default:
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_WARNING, "Mm_Cell_S12_E32:WARNING: ulResult Abnormal");
            break;
    }
}
MM_DECODE_RESULT_ENUM_U32 NAS_MM_DecodeFullNameForNetworkIe(
    VOS_UINT8                           *pucRcvMsg,
    VOS_UINT16                          usMsgSize,
    VOS_UINT16                          *pusIndex,
    VOS_UINT8                           *pucEventId,
    VOS_UINT8                           *pucLongNetWorkNameExist
)
{
    VOS_UINT16                          usIndex;
    MM_DECODE_RESULT_ENUM_U32           ulRst;
    VOS_UINT16                          usIeLen;
    NAS_MML_OPERATOR_NAME_INFO_STRU    *pstCurrOperatorName = VOS_NULL_PTR;

    pstCurrOperatorName = NAS_MML_GetOperatorNameInfo();

    usIndex = *pusIndex;
    ulRst   = MM_DECODE_SUCCESS;

    if ( usMsgSize < (usIndex + MM_IE_TL_LENGTH))
    {
        usIndex = usMsgSize;
    }
    else
    {
        usIeLen = pucRcvMsg[usIndex + MM_IE_TAG_LENGTH];

        if ( 0 == usIeLen )
        {
            NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_DecodeFullNameForNetworkIe:WARNING: NETWORK NAME SIZE ERROR!");
            *pucEventId = MM_EVENT_ID_STATUS_95;
            ulRst       = MM_DECODE_END;

        }
        else if ( usMsgSize < (usIndex + usIeLen + MM_IE_TL_LENGTH) )
        {
            usIndex = usMsgSize;
        }
        else
        {
            if ( VOS_FALSE == MM_IeChkNtwkNam( &pucRcvMsg[usIndex + MM_IE_TAG_LENGTH] ) )
            {
                NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_DecodeFullNameForNetworkIe:WARNING: NETWORK NAME ERROR!");
                *pucEventId = MM_EVENT_ID_STATUS_95;
                ulRst       = MM_DECODE_END;
            }
            else
            {
                PS_MEM_SET(pstCurrOperatorName->aucOperatorNameLong, 0, NAS_MML_MAX_OPER_LONG_NAME_LEN);
                if (NAS_MML_MAX_OPER_LONG_NAME_LEN >= (pucRcvMsg[usIndex + MM_IE_TAG_LENGTH] + 1))
                {
                    PS_MEM_CPY(pstCurrOperatorName->aucOperatorNameLong, (VOS_UINT8 *)&pucRcvMsg[usIndex + MM_IE_TAG_LENGTH], pucRcvMsg[usIndex + MM_IE_TAG_LENGTH] + 1);
                }
                else
                {
                    PS_MEM_CPY(pstCurrOperatorName->aucOperatorNameLong, (VOS_UINT8 *)&pucRcvMsg[usIndex + MM_IE_TAG_LENGTH], NAS_MML_MAX_OPER_LONG_NAME_LEN);
                }
            }

            usIndex = usIndex + usIeLen + MM_IE_TL_LENGTH;
        }
    }

    *pusIndex = usIndex;

    return ulRst;
}


MM_DECODE_RESULT_ENUM_U32 NAS_MM_DecodeShortNameForNetworkIe(
    VOS_UINT8                           *pucRcvMsg,
    VOS_UINT16                          usMsgSize,
    VOS_UINT16                          *pusIndex,
    VOS_UINT8                           *pucEventId,
    VOS_UINT8                           *pucShortNetWorkNameExist
)
{
    VOS_UINT16                          usIndex;
    MM_DECODE_RESULT_ENUM_U32           ulRst;
    VOS_UINT16                          usIeLen;
    NAS_MML_OPERATOR_NAME_INFO_STRU    *pstCurrOperatorName = VOS_NULL_PTR;

    pstCurrOperatorName = NAS_MML_GetOperatorNameInfo();

    usIndex = *pusIndex;
    ulRst   = MM_DECODE_SUCCESS;

    if ( usMsgSize < (usIndex + MM_IE_TL_LENGTH))
    {
        usIndex = usMsgSize;
    }
    else
    {
        usIeLen = pucRcvMsg[usIndex + MM_IE_TAG_LENGTH];

        if ( 0 == usIeLen )
        {
            NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_DecodeShortNameForNetworkIe:WARNING: NETWORK NAME SIZE ERROR!");
            *pucEventId = MM_EVENT_ID_STATUS_95;
            ulRst       = MM_DECODE_END;
        }
        else if ( usMsgSize < (usIndex + usIeLen + MM_IE_TL_LENGTH) )
        {
            usIndex = usMsgSize;
        }
        else
        {
            if ( VOS_FALSE == MM_IeChkNtwkNam( &pucRcvMsg[usIndex + MM_IE_TAG_LENGTH] ) )
            {
                NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_DecodeShortNameForNetworkIe:WARNING: NETWORK NAME ERROR!");
                *pucEventId = MM_EVENT_ID_STATUS_95;
                ulRst       = MM_DECODE_END;
            }
            else
            {
                PS_MEM_SET(pstCurrOperatorName->aucOperatorNameShort, 0, NAS_MML_MAX_OPER_SHORT_NAME_LEN);
                if (NAS_MML_MAX_OPER_SHORT_NAME_LEN >= (pucRcvMsg[usIndex + MM_IE_TAG_LENGTH] + 1))
                {
                    PS_MEM_CPY(pstCurrOperatorName->aucOperatorNameShort, (VOS_UINT8 *)&pucRcvMsg[usIndex + MM_IE_TAG_LENGTH], pucRcvMsg[usIndex + MM_IE_TAG_LENGTH] + 1);
                }
                else
                {
                    PS_MEM_CPY(pstCurrOperatorName->aucOperatorNameShort, (VOS_UINT8 *)&pucRcvMsg[usIndex + MM_IE_TAG_LENGTH], NAS_MML_MAX_OPER_SHORT_NAME_LEN);
                }
            }

            usIndex = usIndex + usIeLen + MM_IE_TL_LENGTH;
        }
    }

    *pusIndex = usIndex;

    return ulRst;
}


MM_DECODE_RESULT_ENUM_U32 NAS_MM_DecodeLocalTimeZoneIe(
    VOS_UINT8                           *pucRcvMsg,
    VOS_UINT16                          usMsgSize,
    VOS_UINT16                          *pusIndex,
    VOS_UINT8                           *pucEventId
)
{
    VOS_UINT16                          usIndex;
    MM_DECODE_RESULT_ENUM_U32           ulRst;

    usIndex = *pusIndex;
    ulRst   = MM_DECODE_SUCCESS;

    if ( usMsgSize < (usIndex + MM_IE_TL_LENGTH) )
    {
        usIndex = usMsgSize;
    }
    else
    {
        if ( VOS_FALSE == MM_IeChkTimZone(&pucRcvMsg[usIndex + MM_IE_TAG_LENGTH]))
        {
            NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_DecodeLocalTimeZoneIe:WARNING: Local Time Zone Value ERROR!");
            *pucEventId = MM_EVENT_ID_STATUS_95;
            ulRst       = MM_DECODE_END;
        }
        usIndex = usIndex + MM_IE_LOCAL_TIME_ZONE_LEN;
    }

    *pusIndex = usIndex;

    return ulRst;
}


MM_DECODE_RESULT_ENUM_U32 NAS_MM_DecodeUniversalTimeAndLocalTimeZoneIe(
    VOS_UINT8                           *pucRcvMsg,
    VOS_UINT16                          usMsgSize,
    VOS_UINT16                          *pusIndex,
    VOS_UINT8                           *pucEventId
)
{
    VOS_UINT16                          usIndex;
    MM_DECODE_RESULT_ENUM_U32           ulRst;

    usIndex = *pusIndex;
    ulRst   = MM_DECODE_SUCCESS;

    if ( usMsgSize < (usIndex + MM_IE_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_LEN) )
    {
        usIndex = usMsgSize;
    }
    else
    {
        if ( VOS_FALSE == MM_IeChkTimAndTimZone(&pucRcvMsg[usIndex + MM_IE_TAG_LENGTH]))
        {
            NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_DecodeUniversalTimeAndLocalTimeZoneIe:WARNING: Universal Time and Zone value error!");
        }
        usIndex = usIndex + MM_IE_UNIVERSAL_TIME_AND_LOCAL_TIME_ZONE_LEN;
    }

    *pusIndex = usIndex;

    return ulRst;
}


MM_DECODE_RESULT_ENUM_U32 NAS_MM_DecodeLocalizedServiceAreaIe(
    VOS_UINT8                           *pucRcvMsg,
    VOS_UINT16                          usMsgSize,
    VOS_UINT16                          *pusIndex,
    VOS_UINT8                           *pucEventId
)
{
    VOS_UINT16                          usIndex;
    MM_DECODE_RESULT_ENUM_U32           ulRst;
    VOS_UINT16                          usIeLen;

    usIndex = *pusIndex;
    ulRst   = MM_DECODE_SUCCESS;

    if ( usMsgSize < (usIndex + MM_IE_TL_LENGTH) )
    {
        usIndex = usMsgSize;
    }
    else
    {
        usIeLen = pucRcvMsg[usIndex + MM_IE_TAG_LENGTH];

        if (usIeLen > MM_IE_LSA_IDENTITY_VALUE_MAX_LEN)
        {
            NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_DecodeLocalizedServiceAreaIe:WARNING: LSA ID SIZE ERROR!");
            *pucEventId = MM_EVENT_ID_STATUS_95;
            ulRst       = MM_DECODE_END;
        }
        if ( usMsgSize < (usIndex + usIeLen + MM_IE_TL_LENGTH) )
        {
            usIndex = usMsgSize;
        }
        else
        {
            if ( VOS_FALSE == MM_IeChkLsaIdtfr( &pucRcvMsg[usIndex + MM_IE_TAG_LENGTH] ))
            {
                NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_DecodeLocalizedServiceAreaIe:WARNING: LSA ID ERROR!");
                *pucEventId = MM_EVENT_ID_STATUS_95;
                ulRst       = MM_DECODE_END;
            }
            usIndex = usIndex + usIeLen + MM_IE_TL_LENGTH;
        }
    }

    *pusIndex = usIndex;

    return ulRst;
}


MM_DECODE_RESULT_ENUM_U32 NAS_MM_DecodeNetworkDaylightSavingTimeIe(
    VOS_UINT8                           *pucRcvMsg,
    VOS_UINT16                          usMsgSize,
    VOS_UINT16                          *pusIndex,
    VOS_UINT8                           *pucEventId
)
{
    VOS_UINT16                          usIndex;
    MM_DECODE_RESULT_ENUM_U32           ulRst;
    VOS_UINT16                          usIeLen;

    usIndex = *pusIndex;
    ulRst   = MM_DECODE_SUCCESS;

    if ( usMsgSize < (usIndex + MM_IE_NETWORK_DAYLIGHT_SAVING_TIME_LEN) )
    {
        usIndex = usMsgSize;
    }
    else
    {
        usIeLen = pucRcvMsg[usIndex + MM_IE_TAG_LENGTH];
        if ( MM_IE_NETWORK_DAYLIGHT_SAVING_TIME_VALUE_LEN !=  usIeLen)
        {
            NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_DecodeNetworkDaylightSavingTimeIe:WARNING: DAYLIGHT SAVING TIME SIZE ERROR!");
            *pucEventId = MM_EVENT_ID_STATUS_95;
            ulRst       = MM_DECODE_END;
        }

        if ( MM_FALSE == MM_IeChkDayLightSavTim( &pucRcvMsg[usIndex + 1]))
        {
            NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_DecodeNetworkDaylightSavingTimeIe:WARNING: DAYLIGHT SAVING TIME ERROR!");
            *pucEventId = MM_EVENT_ID_STATUS_95;
            ulRst       = MM_DECODE_END;
        }
        usIndex = usIndex + MM_IE_NETWORK_DAYLIGHT_SAVING_TIME_LEN;
    }

    *pusIndex = usIndex;

    return ulRst;
}


VOS_UINT32  NAS_MM_IsInRegisterProcedure(VOS_VOID)
{
    /* �жϵ�ǰ�Ƿ���ע������� */
    if((WAIT_FOR_RR_CONNECTION_LOCATION_UPDATING == g_MmGlobalInfo.ucState)
        || (LOCATION_UPDATING_INITIATED == g_MmGlobalInfo.ucState))
    {
        return VOS_TRUE;
    }
    else
    {
        return VOS_FALSE;
    }
}
VOS_BOOL NAS_MM_NeedSendEPlmnConnFail()
{
    if ( (NAS_MML_NET_RAT_TYPE_GSM == NAS_MML_GetCurrNetRatType())
      && (1 == g_MmGlobalInfo.LuInfo.ucT3213AttmptCnt) )
    {
        return VOS_FALSE;
    }
    else
    {
        return VOS_TRUE;
    }
}

/*****************************************************************************
 �� �� ��  : NAS_MM_IsPLmnIdInEplmnList
 ��������  : �жϵ�ǰPLMN ID�Ƿ���EPLMN��RPLMN��
 �������  : ��
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��11��
    ��    ��   : zhoujun 40661
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32  NAS_MM_IsPLmnIdInEplmnList (
    MM_PLMN_ID_STRU                    *pstPlmnId
)
{
    NAS_MML_EQUPLMN_INFO_STRU          *pstEplmnList = VOS_NULL_PTR;
    VOS_UINT32                          i;

    pstEplmnList    =   NAS_MML_GetEquPlmnList();
    for ( i = 0 ; i < pstEplmnList->ucEquPlmnNum; i++ )
    {
        if ( ( pstPlmnId->ulMcc == pstEplmnList->astEquPlmnAddr[i].ulMcc)
          && ( pstPlmnId->ulMnc == pstEplmnList->astEquPlmnAddr[i].ulMnc))
        {
            return VOS_TRUE;
        }
    }


    return VOS_FALSE;
}




VOS_VOID NAS_MM_ConvertToMmcServiceStatus(
    VOS_UINT8                           ucMmServiceStatus,
    VOS_UINT32                         *pCsServiceStatus
)
{
    switch(ucMmServiceStatus)
    {
        case MM_NORMAL_SERVICE:
            *pCsServiceStatus = MM_COM_SRVST_NORMAL_SERVICE;
            break;
        case MM_LIMITED_SERVICE:
            *pCsServiceStatus = MM_COM_SRVST_LIMITED_SERVICE;
            break;
        case MM_NO_IMSI:
            *pCsServiceStatus = MM_COM_SRVST_NO_IMSI;
            break;
        case MM_NO_SERVICE:
            *pCsServiceStatus = MM_COM_SRVST_NO_SERVICE;
            break;
        case MM_LIMIT_REGION_SERVICE:
            *pCsServiceStatus = MM_COM_SRVST_LIMITED_SERVICE_REGION;
            break;
        case MM_ATTEMPTING_TO_UPDATE:
            *pCsServiceStatus = MM_COM_SRVST_NO_CHANGE;
            break;
        case MM_DETACHED:
            *pCsServiceStatus = MM_COM_SRVST_LIMITED_SERVICE;
            break;
        case MM_DISABLE:
            *pCsServiceStatus = MM_COM_SRVST_LIMITED_SERVICE;
            break;
        default:
            break;
    }

    return;
}

/*****************************************************************************
 �� �� ��  : NAS_MM_HandleGmmCauseValAsLuAccept
 ��������  : MMλ�ø��³ɹ������GMM attach/rauʧ��ԭ��
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��1��7��
    ��    ��   : 12
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID NAS_MM_HandleGmmCauseValAsLuAccept()
{
    if ((MM_GMM_ATTACH_PROC == g_MmGlobalInfo.ucProc)
     || (MM_GMM_RAU_PROC == g_MmGlobalInfo.ucProc))
    {
        if ((NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW == g_MmGlobalInfo.usCauseVal)
         || (NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW  == g_MmGlobalInfo.usCauseVal)
         || (NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW == g_MmGlobalInfo.usCauseVal)
         || (NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL == g_MmGlobalInfo.usCauseVal))
        {
            Mm_ComProcCauseClear();
        }
    }

    return;
}


VOS_VOID NAS_MM_UpdateCsServiceConnStatusFlg(VOS_VOID)
{
    if ( ( MM_CONST_NUM_0 == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnExtFlg[0] )
      && ( MM_CONST_NUM_0 == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnExtFlg[1] )
      && ( MM_CONST_NUM_0 == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].aucMMConnExtFlg[0] )
      && ( MM_CONST_NUM_0 == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].aucMMConnExtFlg[1] )
      && ( MM_CONST_NUM_0 == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].aucMMConnExtFlg[0] )
      && ( MM_CONST_NUM_0 == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].aucMMConnExtFlg[1]))
    {
        NAS_MML_SetCsServiceConnStatusFlg(VOS_FALSE);
    }

    if ( ( MM_CONST_NUM_0 != g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnExtFlg[0] )
      || ( MM_CONST_NUM_0 != g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].aucMMConnExtFlg[1] )
      || ( MM_CONST_NUM_0 != g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].aucMMConnExtFlg[0] )
      || ( MM_CONST_NUM_0 != g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].aucMMConnExtFlg[1] )
      || ( MM_CONST_NUM_0 != g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].aucMMConnExtFlg[0] )
      || ( MM_CONST_NUM_0 != g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].aucMMConnExtFlg[1]))
    {
        NAS_MML_SetCsServiceConnStatusFlg(VOS_TRUE);
    }

    return;
}


VOS_VOID NAS_MM_UpdateCsServiceBufferStatusFlg(VOS_VOID)
{
    if ( ( MM_FALSE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg )
      && ( MM_FALSE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg )
      && ( MM_FALSE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg ) )
    {
        NAS_MML_SetCsServiceBufferStatusFlg(VOS_FALSE);
    }

    if ( ( MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg )
      || ( MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg )
      || ( MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg ) )
    {
        NAS_MML_SetCsServiceBufferStatusFlg(VOS_TRUE);
    }

    return;
}


VOS_VOID NAS_MM_SetLikeBFlg(VOS_VOID)
{
    if ((MMCMM_NO_FORBIDDEN == g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg)
     || (MMCMM_FORBIDDEN_PLMN_FOR_GPRS == g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg))
    {
        g_MmGlobalInfo.ucLikeB = MM_TRUE;                               /* ����������Bģʽ��ȫ����                  */
    }
}
VOS_VOID NAS_MM_WriteRplmnWithRatNvim(
    NAS_MML_RPLMN_CFG_INFO_STRU        *pstRplmnCfgInfo
)
{
    NAS_NVIM_RPLMN_WITH_RAT_STRU    stRplmn;

    stRplmn.ucLastRplmnRat          = (VOS_UINT8)pstRplmnCfgInfo->enLastRplmnRat;
    stRplmn.stGRplmn.ulMcc          = pstRplmnCfgInfo->stGRplmnInNV.ulMcc;
    stRplmn.stGRplmn.ulMnc          = pstRplmnCfgInfo->stGRplmnInNV.ulMnc;
    stRplmn.stWRplmn.ulMcc          = pstRplmnCfgInfo->stWRplmnInNV.ulMcc;
    stRplmn.stWRplmn.ulMnc          = pstRplmnCfgInfo->stWRplmnInNV.ulMnc;
    stRplmn.ucLastRplmnRatEnableFlg = pstRplmnCfgInfo->ucLastRplmnRatFlg;

    if(NV_OK != NV_Write(en_NV_Item_RPlmnWithRat, &stRplmn, sizeof(NAS_NVIM_RPLMN_WITH_RAT_STRU)))
    {
        NAS_ERROR_LOG(WUEPS_PID_MM, "NAS_MM_WriteRplmnWithRatNvim(): en_NV_Item_RPlmnWithRat Error");
    }

    return;
}


VOS_UINT32 NAS_MM_IsGURplmnChanged(
    NAS_MML_PLMN_ID_STRU               *pstRPlmnId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurrRat
)
{
    NAS_MML_RPLMN_CFG_INFO_STRU        *pstRplmnCfgInfo = VOS_NULL_PTR;
    VOS_UINT32                          ulGRplmnCompareRslt;
    VOS_UINT32                          ulWRplmnCompareRslt;

    /* Ĭ��RPlmn�ȽϽ��δ�����ı� */
    ulGRplmnCompareRslt = VOS_TRUE;
    ulWRplmnCompareRslt = VOS_TRUE;

    /* ���ڻ�ȡRPLMN�Ķ���������Ϣ */
    pstRplmnCfgInfo = NAS_MML_GetRplmnCfg();

    /* �ж�ȫ�ֱ����е�LastRPLMN�Ľ��뼼���Ƿ����ı� */
    if (enCurrRat != NAS_MML_GetLastRplmnRat())
    {
        /* RPlmn��Ϣ�����˸ı䣬����VOS_TRUE */
        return VOS_TRUE;
    }

    /* �ж�ȫ�ֱ����е�˫RPLMN�Ƿ�֧�� */
    if (VOS_FALSE == pstRplmnCfgInfo->ucMultiRATRplmnFlg)
    {
        ulGRplmnCompareRslt = NAS_MML_CompareBcchPlmnwithSimPlmn(pstRPlmnId,
                                                 &pstRplmnCfgInfo->stGRplmnInNV);

        ulWRplmnCompareRslt = NAS_MML_CompareBcchPlmnwithSimPlmn(pstRPlmnId,
                                                 &pstRplmnCfgInfo->stWRplmnInNV);
    }
    else
    {
        /* ֧��˫RPLMN, ��ֻ�ж϶�Ӧ���뼼����RPLMN */
        if (NAS_MML_NET_RAT_TYPE_GSM == enCurrRat)
        {
            ulGRplmnCompareRslt = NAS_MML_CompareBcchPlmnwithSimPlmn(pstRPlmnId,
                                                     &pstRplmnCfgInfo->stGRplmnInNV);
        }
        else if (NAS_MML_NET_RAT_TYPE_WCDMA == enCurrRat)
        {
            ulWRplmnCompareRslt = NAS_MML_CompareBcchPlmnwithSimPlmn(pstRPlmnId,
                                                     &pstRplmnCfgInfo->stWRplmnInNV);
        }
        else
        {
            ;
        }
    }

    /* RPlmn�ȽϽ����VOS_FALSE��ʾ�����˸ı� */
    if ((VOS_FALSE == ulGRplmnCompareRslt)
     || (VOS_FALSE == ulWRplmnCompareRslt))
    {
        /* RPlmn��Ϣ�����˸ı䣬����VOS_TRUE */
        return VOS_TRUE;
    }

    /* RPlmn��Ϣδ�����ı䣬����VOS_FALSE */
    return VOS_FALSE;

}
VOS_VOID NAS_MM_GenerateEPlmnInfo(
    VOS_UINT32                          ulLauAcceptPlmnNum,
    MM_PLMN_ID_STRU                    *pstLauAcceptPlmnList,
    NAS_MML_EQUPLMN_INFO_STRU          *pstEquPlmnInfo
)
{
    VOS_UINT32                          i;
    NAS_MML_PLMN_ID_STRU               *pstPlmnId       = VOS_NULL_PTR;

    /* ��������Eplmn����������15�� */
    if (ulLauAcceptPlmnNum > NAS_MML_MAX_NETWORK_EQUPLMN_NUM)
    {
        NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_GenerateEPlmnInfo:NETWORK EPlmn NUMBER > 15");
        ulLauAcceptPlmnNum = NAS_MML_MAX_NETWORK_EQUPLMN_NUM;
    }

    /* ����ӵ�ǰפ����plmn��Ϣ��EPlmn�б��� */
    pstPlmnId    = NAS_MML_GetCurrCampPlmnId();
    pstEquPlmnInfo->astEquPlmnAddr[0].ulMcc = pstPlmnId->ulMcc;
    pstEquPlmnInfo->astEquPlmnAddr[0].ulMnc = pstPlmnId->ulMnc;
    pstEquPlmnInfo->ucEquPlmnNum = 1;

    /* ��������EPlmn��EPLMN��Ϣ�ӵ�һ��λ�ÿ�ʼ��¼ */
    for ( i = 0; i < ulLauAcceptPlmnNum; i++ )
    {
        pstEquPlmnInfo->astEquPlmnAddr[1 + i].ulMcc = pstLauAcceptPlmnList[i].ulMcc;
        pstEquPlmnInfo->astEquPlmnAddr[1 + i].ulMnc = pstLauAcceptPlmnList[i].ulMnc;
        pstEquPlmnInfo->ucEquPlmnNum += 1;
    }

    /* ��EPLMN�б���ɾ����Ч ����ֹ�Ͳ��������ε����� */
    pstEquPlmnInfo->ucEquPlmnNum = (VOS_UINT8)NAS_MML_DelInvalidPlmnFromList(pstEquPlmnInfo->ucEquPlmnNum,
                                            pstEquPlmnInfo->astEquPlmnAddr);
    pstEquPlmnInfo->ucEquPlmnNum = (VOS_UINT8)NAS_MML_DelForbPlmnInList(pstEquPlmnInfo->ucEquPlmnNum,
                                            pstEquPlmnInfo->astEquPlmnAddr);

    return ;
}

VOS_VOID NAS_MM_ConvertMmlPlmnIdToNvimEquPlmn(
    NAS_MML_PLMN_ID_STRU               *pstMmlPlmnId,
    NVIM_PLMN_VALUE_STRU               *pstNvimPlmnId
)
{
    /* ת��MCC */
    pstNvimPlmnId->ucMcc[0] = (VOS_UINT8)(pstMmlPlmnId->ulMcc & NAS_MML_OCTET_LOW_FOUR_BITS);

    pstNvimPlmnId->ucMcc[1] = (VOS_UINT8)((pstMmlPlmnId->ulMcc >> 8) & NAS_MML_OCTET_LOW_FOUR_BITS);

    pstNvimPlmnId->ucMcc[2] = (VOS_UINT8)((pstMmlPlmnId->ulMcc >> 16) & NAS_MML_OCTET_LOW_FOUR_BITS);

    /* ת��MNC */
    pstNvimPlmnId->ucMnc[0] = (VOS_UINT8)(pstMmlPlmnId->ulMnc & NAS_MML_OCTET_LOW_FOUR_BITS);

    pstNvimPlmnId->ucMnc[1] = (VOS_UINT8)((pstMmlPlmnId->ulMnc >> 8) & NAS_MML_OCTET_LOW_FOUR_BITS);

    pstNvimPlmnId->ucMnc[2] = (VOS_UINT8)((pstMmlPlmnId->ulMnc >> 16) & NAS_MML_OCTET_LOW_FOUR_BITS);
}


VOS_VOID NAS_MM_ConvertMmlEquListToNvimEquPlmnList(
    NAS_MML_EQUPLMN_INFO_STRU          *pstMmlEPlmnList,
    NVIM_EQUIVALENT_PLMN_LIST_STRU     *pstNvimEPlmnList
)
{
    VOS_UINT8                           i;

    pstNvimEPlmnList->ucCount = pstMmlEPlmnList->ucEquPlmnNum;

    if (pstNvimEPlmnList->ucCount > NVIM_MAX_EPLMN_NUM)
    {
        pstNvimEPlmnList->ucCount = NVIM_MAX_EPLMN_NUM;
    }

    for (i = 0; i < pstNvimEPlmnList->ucCount; i++)
    {
        NAS_MM_ConvertMmlPlmnIdToNvimEquPlmn(&(pstMmlEPlmnList->astEquPlmnAddr[i]),
                                              &(pstNvimEPlmnList->struPlmnList[i]));

    }

    return;
}


VOS_UINT32 NAS_MM_IsInNvEplmnList(
    NVIM_PLMN_VALUE_STRU               *pstPlmnId,
    VOS_UINT8                           ucPlmnNum,
    NVIM_PLMN_VALUE_STRU               *pstPlmnIdList
)
{
    VOS_UINT32                          i;

    for ( i = 0 ; i < ucPlmnNum ; i++ )
    {
        if ( (pstPlmnId->ucMcc[0] == pstPlmnIdList[i].ucMcc[0])
          && (pstPlmnId->ucMcc[1] == pstPlmnIdList[i].ucMcc[1])
          && (pstPlmnId->ucMcc[2] == pstPlmnIdList[i].ucMcc[2])
          && (pstPlmnId->ucMnc[0] == pstPlmnIdList[i].ucMnc[0])
          && (pstPlmnId->ucMnc[1] == pstPlmnIdList[i].ucMnc[1])
          && (pstPlmnId->ucMnc[2] == pstPlmnIdList[i].ucMnc[2]) )
        {
            return VOS_TRUE;
        }
    }
    return VOS_FALSE;
}


VOS_UINT32 NAS_MM_IsNvimEPlmnListEqual(
    NVIM_EQUIVALENT_PLMN_LIST_STRU     *pstNvimEPlmnList1,
    NVIM_EQUIVALENT_PLMN_LIST_STRU     *pstNvimEPlmnList2
)
{
    VOS_UINT32                          i;

    if (pstNvimEPlmnList1->ucCount != pstNvimEPlmnList2->ucCount)
    {
        return VOS_FALSE;
    }

    /* Eplmn��������0,��Rplmn��ͬʱ��EplmnList����ͬ */
    if (pstNvimEPlmnList1->ucCount > 0)
    {
        if (VOS_FALSE == NAS_MM_IsInNvEplmnList(&(pstNvimEPlmnList1->struPlmnList[0]),
                                                 1,
                                                 pstNvimEPlmnList2->struPlmnList))
        {
            return VOS_FALSE;
        }
    }
    else
    {
        return VOS_TRUE;
    }

    /* ����Eplmnû�н������������ظ����ݣ���Ҫ�Ա�2���б����ȷ����ͬ */
    for ( i = 1 ; i < pstNvimEPlmnList1->ucCount ; i++ )
    {
        if (VOS_FALSE == NAS_MM_IsInNvEplmnList(&(pstNvimEPlmnList1->struPlmnList[i]),
                                                 pstNvimEPlmnList2->ucCount,
                                                 pstNvimEPlmnList2->struPlmnList))
        {
            return VOS_FALSE;
        }
    }

    for ( i = 1 ; i < pstNvimEPlmnList2->ucCount ; i++ )
    {
        if (VOS_FALSE == NAS_MM_IsInNvEplmnList(&(pstNvimEPlmnList2->struPlmnList[i]),
                                                 pstNvimEPlmnList1->ucCount,
                                                 pstNvimEPlmnList1->struPlmnList))
        {
            return VOS_FALSE;
        }
    }

    return VOS_TRUE;
}


VOS_VOID NAS_MM_DeleteEPlmnList()
{
    NAS_MML_EQUPLMN_INFO_STRU          *pstEPlmnList    = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU               *pstCurrPlmnId = VOS_NULL_PTR;

    pstEPlmnList = NAS_MML_GetEquPlmnList();

    NAS_MML_InitEquPlmnInfo(pstEPlmnList);

    /* ���NV�б����EPLMN */
    NAS_MM_WriteEplmnNvim( pstEPlmnList );

    /*����ǰ�������Eplmn�б�*/
    pstCurrPlmnId = NAS_MML_GetCurrCampPlmnId();

    pstEPlmnList->astEquPlmnAddr[0].ulMcc = pstCurrPlmnId->ulMcc;
    pstEPlmnList->astEquPlmnAddr[0].ulMnc = pstCurrPlmnId->ulMnc;

    pstEPlmnList->ucEquPlmnNum = 1;


    return;
}


VOS_VOID NAS_MM_WriteEplmnNvim(
    NAS_MML_EQUPLMN_INFO_STRU          *pstEplmnAddr
)
{

    VOS_UINT32                          ulUpdateNvFlag;
    NVIM_EQUIVALENT_PLMN_LIST_STRU     *pstNewNvEquPlmnList = VOS_NULL_PTR;
    NVIM_EQUIVALENT_PLMN_LIST_STRU     *pstOldNvEquPlmnList = VOS_NULL_PTR;

    ulUpdateNvFlag  = VOS_FALSE;

    pstNewNvEquPlmnList = (NVIM_EQUIVALENT_PLMN_LIST_STRU*)PS_MEM_ALLOC(
                                                      WUEPS_PID_MM,
                                                      sizeof(NVIM_EQUIVALENT_PLMN_LIST_STRU));

    if (VOS_NULL_PTR == pstNewNvEquPlmnList)
    {
        NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_WriteEplmnNvim:ERROR: Memory Alloc Error");
        return;
    }

    pstOldNvEquPlmnList = (NVIM_EQUIVALENT_PLMN_LIST_STRU*)PS_MEM_ALLOC(
                                                        WUEPS_PID_MM,
                                                        sizeof (NVIM_EQUIVALENT_PLMN_LIST_STRU));

    if (VOS_NULL_PTR == pstOldNvEquPlmnList)
    {
        NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_WriteEplmnNvim:ERROR: Memory Alloc Error");
        PS_MEM_FREE(WUEPS_PID_MM, pstNewNvEquPlmnList);
        return;
    }

    PS_MEM_SET(pstNewNvEquPlmnList, 0, sizeof(NVIM_EQUIVALENT_PLMN_LIST_STRU));
    PS_MEM_SET(pstOldNvEquPlmnList, 0, sizeof(NVIM_EQUIVALENT_PLMN_LIST_STRU));

    NAS_MM_ConvertMmlEquListToNvimEquPlmnList(pstEplmnAddr,
                                              pstNewNvEquPlmnList);

    /* ��ȡNV��EPLMN��Ϣ */
    if ( NV_OK == NV_Read(en_NV_Item_EquivalentPlmn,
                         pstOldNvEquPlmnList, sizeof(NVIM_EQUIVALENT_PLMN_LIST_STRU)) )
    {
        if (VOS_FALSE == NAS_MM_IsNvimEPlmnListEqual(pstNewNvEquPlmnList,
                                                     pstOldNvEquPlmnList))
        {
            ulUpdateNvFlag = VOS_TRUE;
        }
    }
    else
    {
        ulUpdateNvFlag = VOS_TRUE;
    }

    if ( VOS_TRUE == ulUpdateNvFlag )
    {
        if (NV_OK != NV_Write(en_NV_Item_EquivalentPlmn,
                              pstNewNvEquPlmnList,
                              sizeof(NVIM_EQUIVALENT_PLMN_LIST_STRU)))
        {
            NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_WriteEplmnNvim:WARNING: en_NV_Item_EquivalentPlmn Error");
        }
    }

    PS_MEM_FREE(WUEPS_PID_MM, pstNewNvEquPlmnList);
    PS_MEM_FREE(WUEPS_PID_MM, pstOldNvEquPlmnList);

}
VOS_VOID NAS_MM_UpdateEquPlmnInfo(
    VOS_UINT32                          ulCause
)
{
    NAS_MML_LOCATION_UPDATE_STATUS_ENUM_UINT8               enCsUpdateStatus;

    enCsUpdateStatus = NAS_MML_GetCsUpdateStatus();

    /* ���LAUʧ�ܣ���Ҫ�ж��Ƿ�ɾ��EPLMN */
    switch (ulCause)
    {
        case NAS_MML_REG_FAIL_CAUSE_IMSI_UNKNOWN_IN_HLR:
        case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_MS:
        case NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME:
        case NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW:
        case NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW:
        case NAS_MML_REG_FAIL_CAUSE_AUTH_REJ:

            NAS_MM_DeleteEPlmnList();

            break;

        case NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW:
        case NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL:
        case NAS_MML_REG_FAIL_CAUSE_ACCESS_BARRED:
        case NAS_MML_REG_FAIL_CAUSE_MS_CFG_DOMAIN_NOT_SUPPORT:
        case NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN:
        case NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_REG_PRVS_SVS:
        case NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_ROAM:
        case NAS_MML_REG_FAIL_CAUSE_NTDTH_IMSI:
        case NAS_MML_REG_FAIL_CAUSE_SERVING_CELL_DOMAIN_NOT_SUPPORT:

            break;

        default:
           /* ����ԭ��ֵ�����LAU attempt acounter����Roam broker����ע��ʧ�ܵĴ�������Ҫ���EPLMN */
           if ((MM_TRUE == Mm_ComLaiJudge())
            || (g_MmGlobalInfo.LuInfo.ucLuAttmptCnt >= MM_CONST_NUM_4)
            || (NAS_MML_LOCATION_UPDATE_STATUS_UPDATED != enCsUpdateStatus))
            {
                NAS_MM_DeleteEPlmnList();

            }
           break;

    }

    return;
}
VOS_VOID NAS_MM_CsSimValid(VOS_BOOL *pbCsSimValid)
{
    if (VOS_TRUE == NAS_MML_GetSimCsRegStatus())
    {
        *pbCsSimValid = VOS_TRUE;
    }
    else
    {
        *pbCsSimValid = VOS_FALSE;
    }

    return;
}


VOS_VOID NAS_MM_CsCellBar(VOS_BOOL *pbCsCellBar)
{
    if (VOS_TRUE == NAS_MML_GetCsRestrictNormalServiceFlg())
    {
        *pbCsCellBar = VOS_TRUE;
    }
    else
    {
        *pbCsCellBar = VOS_FALSE;
    }

    return;
}


NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8 NAS_MM_GetLauUptNeededCsSrvStatus(VOS_VOID)
{
    NAS_MML_LOCATION_UPDATE_STATUS_ENUM_UINT8   enCsUpdateStatus;

    enCsUpdateStatus = NAS_MML_GetCsUpdateStatus();

    /* ��ǰLAI���ϴ�ע��ɹ�LAI��ͬ���Ҹ���״̬ΪU1 */
    if((MM_FALSE == Mm_ComLaiJudge())
    && (NAS_MML_LOCATION_UPDATE_STATUS_UPDATED == enCsUpdateStatus)
    && ( MM_STATUS_ATTACHED == g_MmSubLyrShare.MmShare.ucCsAttachState ))
    {
        return MM_COM_SRVST_NORMAL_SERVICE;
    }
    else
    {
        return MM_COM_SRVST_LIMITED_SERVICE;
    }
}
VOS_VOID  NAS_MM_ProcCsAcChgRegisterNetModeIIUpdated()
{
    /* ���LAI����,����״̬ΪU1 */
    if (MM_ATT_NOT_NEED == g_MmGlobalInfo.ucAttFlg)
    {
        if (MM_TRUE == g_MmGlobalInfo.LuInfo.ucT3212ExpiredFlg)
        {
            g_MmGlobalInfo.LuInfo.ucLuType = MM_IE_LUT_PERIODIC_UPDATING;       /* ����LU����                               */
            Mm_ComLuOnly();
        }
        else
        {
            g_MmGlobalInfo.ucMmServiceState = MM_NORMAL_SERVICE;                /* ���÷���״̬                             */
            Mm_ComNasInfoSav(MM_STATUS_ATTACHED,
                             NAS_MML_GetSimCsSecurityCksn());
            Mm_ComSetMmState(MM_IDLE_NORMAL_SERVICE);
            Mm_TimerStart(MM_TIMER_T3212);                                      /* ����T3212                                */
        }
    }
    else
    {
        /* ��ǰ���ϵ翪�� */
        if (MM_TRUE == g_MmGlobalInfo.ucPowerOnFlg)
        {
            g_MmGlobalInfo.LuInfo.ucLuType = MM_IE_LUT_IMSI_ATTACH;             /* ����LU����                               */
            Mm_ComLuOnly();                                                     /* ����LU REQ                               */
        }
        else if (MM_TRUE == g_MmGlobalInfo.LuInfo.ucT3212ExpiredFlg)
        {
            g_MmGlobalInfo.LuInfo.ucLuType = MM_IE_LUT_PERIODIC_UPDATING;       /* ����LU����                               */
            Mm_ComLuOnly();                                                     /* ����LU REQ                               */
        }
        else
        {
            g_MmGlobalInfo.ucMmServiceState = MM_NORMAL_SERVICE;                /* ���÷���״̬                             */
            Mm_ComSetMmState(MM_IDLE_NORMAL_SERVICE);
            Mm_TimerStart(MM_TIMER_T3212);                      /* ����T3212                                */
        }
    }
}
VOS_VOID NAS_MM_ProcCsAcChgRegisterNetModeIINotUpdated(VOS_VOID)
{
    if (MM_TRUE == g_MmGlobalInfo.ucPowerOnFlg)                                 /* �տ�����λ�������䣬Ҳ����ע�� */
    {
        g_MmGlobalInfo.LuInfo.ucLuType = MM_IE_LUT_NORMAL_LU;                   /* ����LU����                               */
        Mm_ComLuOnly();                                                         /* ����LU REQ                               */
        return;
    }


    /* �ڷ�U1״̬��������ע�ᶨʱ��û������ʱ,Ҫ����һ��LU���� */
    if ((MM_TIMER_STOP == gstMmTimer[MM_TIMER_T3211].ucTimerStatus)
     && (MM_TIMER_STOP == gstMmTimer[MM_TIMER_T3212].ucTimerStatus)
     && (0x0 != gstMmTimer[MM_TIMER_T3212].ulTimerLen))
    {
        if (MM_TRUE == g_MmGlobalInfo.LuInfo.ucT3212ExpiredFlg)
        {
            g_MmGlobalInfo.LuInfo.ucLuType = MM_IE_LUT_PERIODIC_UPDATING;
        }
        else
        {
            g_MmGlobalInfo.LuInfo.ucLuType = MM_IE_LUT_NORMAL_LU;
        }

        Mm_ComLuOnly();
    }
    else
    {
        g_MmGlobalInfo.ucMmServiceState = MM_ATTEMPTING_TO_UPDATE;                /* ���÷���״̬                             */
        Mm_ComSetMmState(MM_IDLE_ATTEMPTING_TO_UPDATE);
    }
}


VOS_VOID NAS_MM_ProcCsAcChgRegisterNetModeII(VOS_VOID)
{
    /* LAI�����ı� */
    if (MM_TRUE == Mm_ComLaiJudge())
    {
        Mm_TimerStop(MM_TIMER_T3213);
        g_MmGlobalInfo.LuInfo.ucLuType = MM_IE_LUT_NORMAL_LU;                   /* ����LU����                               */
        Mm_ComLuOnly();                                                         /* ����LU REQ                               */
    }
    else
    {
        if (NAS_MML_LOCATION_UPDATE_STATUS_UPDATED == NAS_MML_GetCsUpdateStatus())
        {
            /* ��ǰCS UPDATE״̬ΪU1 */
            NAS_MM_ProcCsAcChgRegisterNetModeIIUpdated();
        }
        else
        {
            NAS_MM_ProcCsAcChgRegisterNetModeIINotUpdated();
        }
    }
}


VOS_VOID NAS_MM_UpdateMmStateCsRegRestrictBarToUnbar(VOS_VOID)
{
    NAS_MML_LOCATION_UPDATE_STATUS_ENUM_UINT8   enCsUpdateStatus;

    enCsUpdateStatus = NAS_MML_GetCsUpdateStatus();

    if (MM_IDLE_NO_IMSI == g_MmGlobalInfo.ucStaOfRcvXXEstReq)
    {
        return;
    }

    /* ��ǰCS����Ч */
    if (VOS_FALSE == NAS_MML_GetSimCsRegStatus())
    {
        g_MmGlobalInfo.ucStaOfRcvXXEstReq = MM_IDLE_NO_IMSI;
        g_MmGlobalInfo.ucStaAfterWaitForNwkCmd = MM_IDLE_NO_IMSI;
        g_MmGlobalInfo.ucMmServiceState   = MM_LIMITED_SERVICE;
        return;
    }

    /* �û����ò�֧��CS */
    if (VOS_FALSE == NAS_MML_GetCsAttachAllowFlg())
    {
        g_MmGlobalInfo.ucStaOfRcvXXEstReq = MM_IDLE_LIMITED_SERVICE;
        g_MmGlobalInfo.ucStaAfterWaitForNwkCmd = MM_IDLE_LIMITED_SERVICE;

        g_MmGlobalInfo.ucMmServiceState   = MM_LIMITED_SERVICE;
        return;
    }

    /* ��ǰ�����ڽ�ֹ�б��� */
    if ((MMCMM_NO_FORBIDDEN != g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg)
     && (MMCMM_FORBIDDEN_PLMN_FOR_GPRS != g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg))
    {
        g_MmGlobalInfo.ucStaOfRcvXXEstReq = MM_IDLE_LIMITED_SERVICE;
        g_MmGlobalInfo.ucStaAfterWaitForNwkCmd = MM_IDLE_LIMITED_SERVICE;

        g_MmGlobalInfo.ucMmServiceState   = MM_LIMITED_SERVICE;
        return;
    }

    /* ��ǰCS ATTACH״̬��LAIû�иı䣬����״̬ΪU1 */
    if ((MM_FALSE == Mm_ComLaiJudge())
     && (NAS_MML_LOCATION_UPDATE_STATUS_UPDATED == enCsUpdateStatus)
     && ( MM_STATUS_ATTACHED == g_MmSubLyrShare.MmShare.ucCsAttachState ))
    {
        g_MmGlobalInfo.ucStaOfRcvXXEstReq = MM_IDLE_NORMAL_SERVICE;
        g_MmGlobalInfo.ucStaAfterWaitForNwkCmd = MM_IDLE_NORMAL_SERVICE;

        g_MmGlobalInfo.ucMmServiceState   = MM_NORMAL_SERVICE;
    }
    else
    {
        g_MmGlobalInfo.ucStaOfRcvXXEstReq = MM_IDLE_LIMITED_SERVICE;
        g_MmGlobalInfo.ucStaAfterWaitForNwkCmd = MM_IDLE_NORMAL_SERVICE;

        g_MmGlobalInfo.ucMmServiceState   = MM_LIMITED_SERVICE;
    }
}




VOS_VOID NAS_MM_ProcEccNumList_RcvSysinfo(
    VOS_VOID                           *pRcvMsg
)
{
    MSG_HEADER_STRU                    *pstMsgHeader        = VOS_NULL_PTR;
    MMCMM_SYS_INFO_IND_STRU            *pstWasSysInfo       = VOS_NULL_PTR;
    MMCMM_GSM_SYS_INFO_IND_ST          *pstGasSysInfo       = VOS_NULL_PTR;
    NAS_MML_EMERGENCY_NUM_LIST_STRU    *pstEmergencyNumList = VOS_NULL_PTR;
    NAS_MML_EMERGENCY_NUM_LIST_STRU     EmcNumLst;
    VOS_UINT32                          ulMcc;

    pstMsgHeader = (MSG_HEADER_STRU*)pRcvMsg;

    if(MMCMM_SYS_INFO_IND ==pstMsgHeader->ulMsgName)
    {
        pstWasSysInfo = (MMCMM_SYS_INFO_IND_STRU*)pstMsgHeader;
        ulMcc         = pstWasSysInfo->PlmnId.ulMcc;
    }
    else if(MMCMM_GSM_SYS_INFO_IND ==pstMsgHeader->ulMsgName)
    {
        pstGasSysInfo = (MMCMM_GSM_SYS_INFO_IND_ST*)pstMsgHeader;
        ulMcc         = pstGasSysInfo->stPlmnId.ulMcc;
    }
    else
    {
        /* �쳣��ӡ */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MM_ProcEccNumList_RcvSysinfo: Msg Name error");
        return;
    }

    pstEmergencyNumList = NAS_MML_GetEmergencyNumList();

    if (g_MmGlobalInfo.MsCsInfo.CurLai.PlmnId.ulMcc != ulMcc)
    {                                                                       /* ��С����MCC���ϴ�פ��С����MCC��ͬ       */
        if (pstEmergencyNumList->ulMcc == ulMcc)
        {
            Mm_SndCcEmergencyNumberList(pstEmergencyNumList);               /* ֪ͨCC���������� */
        }
        else
        {
            EmcNumLst.ulMcc             = ulMcc;
            EmcNumLst.ucEmergencyNumber = 0;

            PS_MEM_SET(EmcNumLst.aucEmergencyList, 0xFF, NAS_MML_EMERGENCY_NUM_LIST_MAX_RECORDS);
            Mm_SndCcEmergencyNumberList(&EmcNumLst);                            /* ֪ͨCC������������Ч                     */
        }
    }

    return;
}


#if (FEATURE_LTE == FEATURE_ON)


VOS_UINT32  NAS_MM_IsLauNeededLaiNoChange_LInterSys(VOS_VOID)
{
    NAS_MML_LTE_CS_SERVICE_CFG_ENUM_UINT8                   enLteCsServiceCfg;

    VOS_UINT8                                               ucLauForceFlg;
    VOS_UINT8                                               ucImsVoiceAvailFlg;

    enLteCsServiceCfg   = NAS_MML_GetLteCsServiceCfg();

    /**
     * ������Ǵ� L ��ϵͳ�任 �򲻽��к����ж�
     */
    if (NAS_MML_NET_RAT_TYPE_LTE != g_MmGlobalInfo.enPreRatType)
    {
        return VOS_FALSE;
    }

    /* ���CSҵ������ΪSMS over SGs������TIN����Ϊ"GUTI" */
    if ((NAS_MML_TIN_TYPE_GUTI == NAS_MML_GetTinType())
     && ((NAS_MML_LTE_SUPPORT_SMS_OVER_SGS_ONLY == enLteCsServiceCfg)
      || (NAS_MML_LTE_SUPPORT_CSFB_AND_SMS_OVER_SGS == enLteCsServiceCfg)))
    {
        return VOS_TRUE;
    } 

    /**
     * �������������UE֧��IMS����ȡNv��������ѡ����CS ONLY)
     * �ҿ��ƴ�L��GU�¼�ʹLAIû�иı�Ҳ��Ҫ����LAU��Nv��򿪣�
     * ֻҪ��CSFB��������Ҫ����LAU
     */
    /**
     * VDF�ı���Ҫ��֧��IMS voice���ն��ڴ�L��ϵͳ�任��GU������������һ��LAU,ԭ�����£�
     * The LTE terminal supporting Voice over IMS (GSMA IR.92), 
     * shall always issue a Location Update following a RAT change. 
     * With terminal in IDLE mode a LAU is performed. 
     * If a call is initiated under CS network, 
     * all the procedures currently in use today shall be applicable.
     * For example, when a terminal that initially registers on 2G roams into LTE coverage, 
     * then roams back into 2G under the same LAC that it previously registered, 
     * will it always send a LU towards the MSC.
     */

    ucLauForceFlg = NAS_MML_GetImsVoiceInterSysLauEnableFlg();
    ucImsVoiceAvailFlg = NAS_MML_GetImsVoiceAvailFlg();

    if ((VOS_TRUE == ucLauForceFlg) 
     && (VOS_TRUE == ucImsVoiceAvailFlg))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MM_IsT3412ExpiredNeedRegist(
    NAS_MML_LTE_CS_SERVICE_CFG_ENUM_UINT8                   enLteCsServiceCfg,
    NAS_MML_TIN_TYPE_ENUM_UINT8                             enTinType,
    NAS_MML_TIMER_INFO_ENUM_UINT8                           enT3412Status
)
{
     /* 3GPP 24.008��4.4.1  Location updating procedure�½��������£�
        The normal location updating procedure shall also be started if the MS is
        configured to use CS fallback and SMS over SGs, or SMS over SGs only,
        and the TIN indicates "RAT-related TMSI",
        - when the periodic tracking area update timer T3412 expires and the
        network operates in network operation mode II or III
    */
    if ((NAS_MML_TIMER_EXP == enT3412Status)
     && ((NAS_MML_LTE_SUPPORT_CSFB_AND_SMS_OVER_SGS == enLteCsServiceCfg)
      || (NAS_MML_LTE_SUPPORT_SMS_OVER_SGS_ONLY == enLteCsServiceCfg))
     && (NAS_MML_TIN_TYPE_RAT_RELATED_TMSI == enTinType))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MM_IsT3423StatusChangeNeedRegist(
    NAS_MML_LTE_CS_SERVICE_CFG_ENUM_UINT8                   enLteCsServiceCfg,
    NAS_MML_TIN_TYPE_ENUM_UINT8                             enTinType,
    NAS_MML_TIMER_INFO_ENUM_UINT8                           enT3423Status
)
{
      /* 3GPP 24.008��4.4.1  Location updating procedure�½��������£�
       The normal location updating procedure shall also be started when the MS,
       configured to use CS fallback and SMS over SGs, or SMS over SGs only,
       enters a GERAN or UTRAN cell in network operation mode II or III and the
       E-UTRAN deactivate ISR timer T3423 has expired.

       The normal location updating procedure shall also be started if the MS is
       configured to use CS fallback and SMS over SGs, or SMS over SGs only,
       and the TIN indicates "RAT-related TMSI",
       -   when the MS enters a GERAN or UTRAN cell in network operation mode II
       or III and the E-UTRAN deactivate ISR timer T3423 is running.
    */
    if (((NAS_MML_TIMER_START == enT3423Status)
      || (NAS_MML_TIMER_EXP == enT3423Status))
     && ((NAS_MML_LTE_SUPPORT_CSFB_AND_SMS_OVER_SGS == enLteCsServiceCfg)
      || (NAS_MML_LTE_SUPPORT_SMS_OVER_SGS_ONLY == enLteCsServiceCfg))
     && (NAS_MML_TIN_TYPE_RAT_RELATED_TMSI == enTinType))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_VOID  NAS_MM_WriteTinInfoNvim(
    NAS_MML_TIN_TYPE_ENUM_UINT8         enTinType,
    VOS_UINT8                          *pucImsi
)
{
    NAS_MML_RPLMN_CFG_INFO_STRU        *pstRplmnCfgInfo = VOS_NULL_PTR;

    NAS_NVIM_TIN_INFO_STRU              stTinInfo;

    pstRplmnCfgInfo = NAS_MML_GetRplmnCfg();

    /*
        The following EMM parameter shall be stored in a non-volatile memory in the
        ME together with the IMSI from the USIM:
        -   TIN.
        This EMM parameter can only be used if the IMSI from the USIM matches the
        IMSI stored in the non-volatile memory of the ME; else the UE shall delete
        the EMM parameter.
    */

    if (pstRplmnCfgInfo->enTinType != enTinType)
    {
        /* �ȸ���MML�Ĳ��� */
        pstRplmnCfgInfo->enTinType  = enTinType;
        PS_MEM_CPY(pstRplmnCfgInfo->aucLastImsi, pucImsi, sizeof(pstRplmnCfgInfo->aucLastImsi));

        /* �ٸ���NVIM�еĲ��� */
        stTinInfo.ucTinType        = pstRplmnCfgInfo->enTinType;
        PS_MEM_CPY(stTinInfo.aucImsi, pstRplmnCfgInfo->aucLastImsi, sizeof(stTinInfo.aucImsi));
        PS_MEM_SET(stTinInfo.aucReserve, 0, sizeof(stTinInfo.aucReserve));

        /* ������NVIM�� */
        if (NV_OK != NV_Write (en_NV_Item_TIN_INFO,
                               &stTinInfo,
                               sizeof(stTinInfo)))
        {
            NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_WriteTinInfoNvim:Write NV Failed");
        }

        /* ��ά�ɲ⣬����Ϣ */
        NAS_MM_LogTinTypeInfo(enTinType);
    }
}


VOS_VOID NAS_MM_LogTinTypeInfo(
    NAS_MML_TIN_TYPE_ENUM_UINT8         enTinType
)
{
    NAS_MM_LOG_TIN_INFO_STRU           *pstTinTypeInfo = VOS_NULL_PTR;

    pstTinTypeInfo = (NAS_MM_LOG_TIN_INFO_STRU*)PS_MEM_ALLOC(WUEPS_PID_MM,
                             sizeof(NAS_MM_LOG_TIN_INFO_STRU));

    if ( VOS_NULL_PTR == pstTinTypeInfo )
    {
        NAS_ERROR_LOG(WUEPS_PID_MM, "NAS_MM_LogTinTypeInfo:ERROR:Alloc Mem Fail.");
        return;
    }

    pstTinTypeInfo->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstTinTypeInfo->stMsgHeader.ulSenderCpuId   = VOS_LOCAL_CPUID;
    pstTinTypeInfo->stMsgHeader.ulSenderPid     = WUEPS_PID_MM;
    pstTinTypeInfo->stMsgHeader.ulReceiverPid   = WUEPS_PID_MM;
    pstTinTypeInfo->stMsgHeader.ulLength        = sizeof(NAS_MM_LOG_TIN_INFO_STRU) - VOS_MSG_HEAD_LENGTH;;
    pstTinTypeInfo->stMsgHeader.ulMsgName       = NAS_MM_LOG_TIN_TYPE_INFO_IND;

    pstTinTypeInfo->enTinType = enTinType;

    OM_TraceMsgHook(pstTinTypeInfo);

    PS_MEM_FREE(WUEPS_PID_MM, pstTinTypeInfo);

    return;
}


VOS_UINT32 NAS_MM_IsNeedDeactiveIsrAfterLau(VOS_VOID)
{
    VOS_UINT32                                             ulT3423StatusChgNeedRegFlg;
    VOS_UINT32                                             ulT3412StatusChgNeedRegFlg;

    /* 3GPP 23401 4.3.5.6 Idle mode signalling reduction function����ȥ����ISR������
       After a LAU procedure if the UE has CS fallback and/or SMS over SGs activated.*/
    ulT3423StatusChgNeedRegFlg = NAS_MM_IsT3423StatusChangeNeedRegist(NAS_MML_GetLteCsServiceCfg(),
                                         NAS_MML_GetTinType(), NAS_MML_GetT3423Status());

    ulT3412StatusChgNeedRegFlg = NAS_MM_IsT3412ExpiredNeedRegist(NAS_MML_GetLteCsServiceCfg(),
                                         NAS_MML_GetTinType(), NAS_MML_GetT3412Status());

    if ((VOS_TRUE == ulT3423StatusChgNeedRegFlg)
     || (VOS_TRUE == ulT3412StatusChgNeedRegFlg))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MM_RcvSysInfoSurpportLte_PreProc(VOS_VOID)
{
    VOS_UINT32                          ulT3423StatusChgNeedRegFlg;

    VOS_UINT32                          ulLaiChangeFlag;
    LAU_TYPE_ENUM_UINT8                 enLauType;

    ulLaiChangeFlag = Mm_ComLaiJudge();
    enLauType       = MM_IE_LUT_TYPE_BUTT;
    if (VOS_TRUE == NAS_MML_IsCsfbServiceStatusExist())
    {
        NAS_MM_RcvSysInfo_CSFB();

        g_MmGlobalInfo.ucNtMod = g_MmGlobalInfo.ucNewNtMod;

        return VOS_TRUE;
    }

    if ( VOS_TRUE == NAS_MML_GetDelayedCsfbLauFlg() )
    {
        /* CSFB���̴�L��ϵͳ�任��GU,����ģʽII,LAIδ�ı�ʱ����ҵ��
           ��¼��Ҫ��ҵ���������LAU */
        NAS_MM_ProcBufferedEmgCallOrLau_RcvSysinfo(ulLaiChangeFlag, MM_IE_LUT_NORMAL_LU);

        g_MmGlobalInfo.ucNtMod = g_MmGlobalInfo.ucNewNtMod;

        return VOS_TRUE;
    }

    /* ���CSҵ������ΪSMS over SGs��CSFB�Ͷ��Ŷ�֧�֣�����TIN����Ϊ"GUTI"
       3GPP 24008 4.4.1:
       indicating to the network that the MS, configured to use CS fallback and
       SMS over SGs, or SMS over SGs only, has entered a GERAN or UTRAN cell
       in NMO II or III, after intersystem change from S1 mode to Iu
       or A/Gb mode, the TIN indicates "GUTI" and the location area of
       the current cell is the same as the stored location area; */

    if (VOS_TRUE == NAS_MM_IsLauNeededLaiNoChange_LInterSys())
    {
        /* ����LAU���� �н������Ƚ��н�����*/
        enLauType = NAS_MM_GetLuType();
        NAS_MM_ProcBufferedEmgCallOrLau_RcvSysinfo(ulLaiChangeFlag, enLauType);



        g_MmGlobalInfo.ucNtMod = g_MmGlobalInfo.ucNewNtMod;

        return VOS_TRUE;
    }

    /* 3GPP 24.008��4.4.1	Location updating procedure�½�����:
       The normal location updating procedure shall also be started if
       the MS is configured to use CS fallback and SMS over SGs,
       or SMS over SGs only, and the TIN indicates "RAT-related TMSI",
       when the MS enters a GERAN or UTRAN cell in network operation
       mode II or III and the E-UTRAN deactivate ISR timer T3423 is running.
       The normal location updating procedure shall also be started when
       the MS, configured to use CS fallback and SMS over SGs, or SMS over
       SGs only, enters a GERAN or UTRAN cell in network operation mode
       II or III and the E-UTRAN deactivate ISR timer T3423 has expired*/
    ulT3423StatusChgNeedRegFlg = NAS_MM_IsT3423StatusChangeNeedRegist(NAS_MML_GetLteCsServiceCfg(),
                           NAS_MML_GetTinType(), NAS_MML_GetT3423Status());

    if (VOS_TRUE == ulT3423StatusChgNeedRegFlg)
    {
        /* ����LAU���� �н������Ƚ��н�����*/
        NAS_MM_ProcBufferedEmgCallOrLau_RcvSysinfo(ulLaiChangeFlag, MM_IE_LUT_PERIODIC_UPDATING);


        g_MmGlobalInfo.ucNtMod = g_MmGlobalInfo.ucNewNtMod;
        return VOS_TRUE;
    }

    return VOS_FALSE;
}

#endif
VOS_UINT8 NAS_MM_GetLuType(VOS_VOID)
{
    if ( MM_TRUE == Mm_ComLaiJudge() )
    {
        return MM_IE_LUT_NORMAL_LU;
    }
    else if((NAS_MML_LOCATION_UPDATE_STATUS_UPDATED == NAS_MML_GetCsUpdateStatus())
         && (MM_TRUE     == g_MmGlobalInfo.ucPowerOnFlg)
         && (MM_ATT_NEED == g_MmGlobalInfo.ucAttFlg))
    {
        return MM_IE_LUT_IMSI_ATTACH;
    }
    else if ( MM_TRUE == g_MmGlobalInfo.LuInfo.ucT3212ExpiredFlg )
    {
        return MM_IE_LUT_PERIODIC_UPDATING;
    }
    else
    {
        return MM_IE_LUT_NORMAL_LU;
    }
}
VOS_VOID  NAS_MM_LogMmStateInfo(
    VOS_UINT8                           ucMmState
)
{
    NAS_MM_LOG_STATE_INFO_STRU         *pstMsg = VOS_NULL_PTR;

    pstMsg = (NAS_MM_LOG_STATE_INFO_STRU*)PS_MEM_ALLOC(WUEPS_PID_MM,
                                         sizeof(NAS_MM_LOG_STATE_INFO_STRU));

    if (VOS_NULL_PTR == pstMsg)
    {
        NAS_ERROR_LOG(WUEPS_PID_MM, "NAS_MM_LogMmStateInfo:ERROR:Alloc Mem Fail.");
        return;
    }

    PS_MEM_SET(pstMsg, 0x00, sizeof(NAS_MM_LOG_STATE_INFO_STRU));

    pstMsg->stMsgHeader.ulSenderCpuId   = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulSenderPid     = WUEPS_PID_MM;
    pstMsg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MM;
    pstMsg->stMsgHeader.ulLength        = sizeof(NAS_MM_LOG_STATE_INFO_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMsg->stMsgHeader.ulMsgName       = MMOM_LOG_STATE_INFO_IND;
    pstMsg->enMmState                   = (NAS_MM_STATE_ID_ENUM_UINT8)ucMmState;

    OM_TraceMsgHook(pstMsg);

    PS_MEM_FREE(WUEPS_PID_MM, pstMsg);

    return;
}


VOS_UINT32 NAS_MM_IsCcTiValid(VOS_VOID)
{
    if ((MM_CONST_NUM_8 > g_MmCcEstReq.ulTransactionId)
      ||(MM_CONST_NUM_14 < g_MmCcEstReq.ulTransactionId))
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_VOID NAS_MM_ProcBufferedEmgCallOrLau_RcvSysinfo(
    VOS_UINT32                          ulLaiChangeFlag,
    VOS_UINT8                           ucLuType
)
{
    VOS_UINT32                          ulCsfbEmgCallLaiChgLauFirstFlg;
    VOS_UINT32                          ulCsRestrictRegFlg;

    VOS_UINT32                          ulEmergencyCallFlg;

    ulEmergencyCallFlg             = Mm_ComJudgeCsServExist();

    ulCsRestrictRegFlg             = NAS_MML_GetCsRestrictRegisterFlg();
    ulCsfbEmgCallLaiChgLauFirstFlg = NAS_MML_GetCsfbEmgCallLaiChgLauFirstFlg();

    if (MM_FALSE == ulLaiChangeFlag)
    {
        /* LAIδ�ı��Ҵ��ڻ���Ľ����������Ƚ��н������У����н���������lau */
        if (MM_CS_SERV_EMERGENCY_CALL_EXIST == ulEmergencyCallFlg)
        {
#if (FEATURE_LTE == FEATURE_ON)
            /* ��¼delay lau��ʶ�����н������յ�ϵͳ��Ϣ��Ҫ��lau */
            NAS_MML_SetDelayedCsfbLauFlg(VOS_TRUE);
#endif

            /* ����ֹCS����LAI���ı�ʱ���������ͷź���Ҫ����LU */
            if ((MMCMM_NO_FORBIDDEN == g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg)
             || ((MMCMM_NO_FORBIDDEN != g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg)
                &&(MMCMM_FORBIDDEN_PLMN_FOR_GPRS != g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg)))
            {
                if ((NAS_MML_LOCATION_UPDATE_STATUS_UPDATED != NAS_MML_GetCsUpdateStatus())
                 && ((MM_TIMER_STOP == gstMmTimer[MM_TIMER_T3211].ucTimerStatus)
                  && (MM_TIMER_STOP == gstMmTimer[MM_TIMER_T3212].ucTimerStatus)
                  && (0x0 != gstMmTimer[MM_TIMER_T3212].ulTimerLen)))
                {
                    NAS_MML_SetDelayedCsfbLauFlg(VOS_TRUE);
                }
            }
            g_MmGlobalInfo.ucStaOfRcvXXEstReq = NAS_MM_GetStaOfRcvXXEstReq_CSFB();/* ��¼�յ���������ʱ��״̬                 */

            NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                        MM_MMC_LU_RESULT_SUCCESS,
                                        NAS_MML_REG_FAIL_CAUSE_NULL);   /* ��MMC����MMCMM_LU_RESULT_IND */

            Mm_ComSetMmState(NAS_MM_GetStaOfRcvXXEstReq_CSFB());

            NAS_MM_EstMmConnection(MM_CONN_CTRL_CC);
            return;
        }

        /* �޻���Ľ�������ֱ�ӽ���lau */
        g_MmGlobalInfo.LuInfo.ucLuType = ucLuType;
        Mm_ComLuOnly();
        return;
    }

    /* ���ڻ���Ľ�������lai�ı��Ƚ��н������н���������lau nv���� */
    if (((VOS_FALSE == ulCsfbEmgCallLaiChgLauFirstFlg)
      || ((VOS_TRUE == ulCsfbEmgCallLaiChgLauFirstFlg)
       && (VOS_TRUE == ulCsRestrictRegFlg)))
     && (MM_CS_SERV_EMERGENCY_CALL_EXIST == ulEmergencyCallFlg))
    {
#if (FEATURE_LTE == FEATURE_ON)
         /* ��¼delay lau��ʶ�����н������յ�ϵͳ��Ϣ��Ҫ��lau */
        NAS_MML_SetDelayedCsfbLauFlg(VOS_TRUE);
#endif
        /* ����ֹCS����LAI���ı�ʱ���������ͷź���Ҫ����LU */
        if ((MMCMM_NO_FORBIDDEN == g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg)
         || ((MMCMM_NO_FORBIDDEN != g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg)
            &&(MMCMM_FORBIDDEN_PLMN_FOR_GPRS != g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg)))
        {
            NAS_MML_SetDelayedCsfbLauFlg(VOS_TRUE);
        }
        g_MmGlobalInfo.ucStaOfRcvXXEstReq = NAS_MM_GetStaOfRcvXXEstReq_CSFB();/* ��¼�յ���������ʱ��״̬                 */

        NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                    MM_MMC_LU_RESULT_SUCCESS,
                                    NAS_MML_REG_FAIL_CAUSE_NULL);   /* ��MMC����MMCMM_LU_RESULT_IND */

        Mm_ComSetMmState(NAS_MM_GetStaOfRcvXXEstReq_CSFB());

         NAS_MM_EstMmConnection(MM_CONN_CTRL_CC);
         return;
    }

    /* �޻���Ľ�������ֱ�ӽ���lau */
    g_MmGlobalInfo.LuInfo.ucLuType = ucLuType;
    Mm_ComLuOnly();
    return;
}



VOS_UINT32  NAS_MM_IsCsEnableLau( VOS_VOID )
{

    /* CS��Ŀ���Ч,����Ҫ���� */
    if ( VOS_FALSE == NAS_MML_GetSimCsRegStatus()  )
    {
        return VOS_FALSE;
    }

    /* CS������aTTach,����Ҫ���� */
    if (VOS_FALSE == NAS_MML_GetCsAttachAllowFlg())
    {
        return VOS_FALSE;
    }

    /* CS���������,����Ҫ���� */
    if ( VOS_TRUE == NAS_MML_GetCsRestrictRegisterFlg())
    {
        return VOS_FALSE;
    }

    /* ��ǰ�ǽ�ֹ����,����Ҫ���� */
    if ((MMCMM_NO_FORBIDDEN !=g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg))
    {
        return VOS_FALSE;
    }

    /* T3211��ʱ������,����Ҫ���� */
    if (MM_TIMER_RUNNING == gstMmTimer[MM_TIMER_T3211].ucTimerStatus)
    {
        return VOS_FALSE;
    }


    return VOS_TRUE;
}
VOS_UINT32  NAS_MM_IsNeedLauCsSigRel( VOS_VOID )
{
    /* ���ô˺���ǰ,�����Ҫ�ж���������״̬,��Ҫ�ɵ��ô˺������ϼ�������֤ */

    if ( (MM_TRUE == Mm_ComLaiJudge())
      && ((MM_TRUE == g_MmGlobalInfo.ucConnRcvSysFlg)
       || (VOS_TRUE == g_MmGlobalInfo.ucLauAcceptContainDiffNbLaiFlg)) )
    {
        return VOS_TRUE;
    }

#if (FEATURE_LTE == FEATURE_ON)

    if (VOS_TRUE == NAS_MML_GetDelayedCsfbLauFlg())
    {
        return VOS_TRUE;
    }
#endif

    return VOS_FALSE;
}
VOS_UINT32  NAS_MM_IsAbleRecover_EstCnfFailResult(
    RRC_NAS_EST_RESULT_ENUM_UINT32      enEstResult
)
{
    if ((RRC_EST_IMMEDIATE_ASSIGN_REJECT == enEstResult)
     || (RRC_EST_ACCESS_BARRED           == enEstResult)
     || (RRC_EST_OTHER_ACCESS_BARRED     == enEstResult)
     || (RRC_EST_RJ_T3122_RUNNING        == enEstResult))
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32  NAS_MM_IsNeedCmServiceRetry_RelIndResult(
    RRC_REL_CAUSE_ENUM_UINT32           enRelCause
)
{
    return VOS_TRUE;
}

VOS_VOID NAS_MM_ProcConnFailLauAttmptCntLess4_RcvRelInd(
    RRC_REL_CAUSE_ENUM_UINT32                   enRelCauses
)
{
    MM_LAI_STRU                                 OldLai;
    MM_LAI_STRU                                 TempLai;
    NAS_MML_LOCATION_UPDATE_STATUS_ENUM_UINT8   enCsUpdateStatus;
    VOS_UINT8                                   ucGsmEstConnFailFlg = MM_FALSE;
    VOS_UINT8                                   ucOldUpdateSts;
    VOS_UINT8                                   ucTempUpdateSts;

    if ( (NAS_MML_NET_RAT_TYPE_GSM == NAS_MML_GetCurrNetRatType())
     && (RRC_REL_CAUSE_CONN_FAIL   == enRelCauses))
    {
        ucGsmEstConnFailFlg = MM_TRUE;
    }

    enCsUpdateStatus = NAS_MML_GetCsUpdateStatus();
    if ((MM_FALSE == Mm_ComLaiJudge())
      && (NAS_MML_LOCATION_UPDATE_STATUS_UPDATED == enCsUpdateStatus))
    {                                                                   /* ���LAI����,�Ҹ���״̬ΪU1��ʱ��         */
        g_MmGlobalInfo.ucPreState = g_MmGlobalInfo.ucState;             /* ��¼ǰ״̬                               */
        g_MmGlobalInfo.ucMmServiceState = MM_NORMAL_SERVICE;
        Mm_ComSetMmState(MM_IDLE_NORMAL_SERVICE);
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "NAS_MM_ProcConnFailLauAttmptCntLess4_RcvRelInd:INFO: MmServiceState is MM_NORMAL_SERVICE");

        if (MM_FALSE == ucGsmEstConnFailFlg)
        {
            NAS_MM_UpdateEquPlmnInfo(NAS_MML_REG_FAIL_CAUSE_RR_CONN_EST_FAIL);

            NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_TRUE,
                                        MM_MMC_LU_RESULT_FAILURE,
                                        NAS_MML_REG_FAIL_CAUSE_RR_CONN_EST_FAIL);            /* ��MMC����MMCMM_LU_RESULT_IND             */

            NAS_MM_SndGmmLuResultInd(MMGMM_LU_FAILURE,
                                     NAS_MML_REG_FAIL_CAUSE_RR_CONN_EST_FAIL);            /* ��MMC����MMCMM_LU_RESULT_IND             */

        }

        Mm_ComNasInfoSav(MM_STATUS_ATTACHED,
                         NAS_MML_GetSimCsSecurityCksn());
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "NAS_MM_ProcConnFailLauAttmptCntLess4_RcvRelInd: STATUS is MM_IDLE_NORMAL_SERVICE");
        Mm_TimerStart(MM_TIMER_T3211);                                  /* ����T3211                                */
        g_T3211Flag = 1;
    }
    else
    {
        g_MmGlobalInfo.ucMmServiceState = MM_ATTEMPTING_TO_UPDATE;
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "NAS_MM_ProcConnFailLauAttmptCntLess4_RcvRelInd: MmServiceState is MM_ATTEMPTING_TO_UPDATE");

        if (MM_FALSE == ucGsmEstConnFailFlg)
        {
            ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
            PS_MEM_CPY(&OldLai,
                       &g_MmGlobalInfo.MsCsInfo.OldLai,
                       sizeof(MM_LAI_STRU));


            /* ��U2״̬�����浱ǰ��LAI��Ϣ */
            PS_MEM_CPY(NAS_MM_GetAttemptUpdateLaiInfo(), 
                       &g_MmGlobalInfo.MsCsInfo.CurLai, sizeof(MM_LAI_STRU));            

            Mm_ComToU2Handle();                                             /* ��U2�Ĺ�������                           */

            ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();

            PS_MEM_CPY(&TempLai,
                       &g_MmGlobalInfo.MsCsInfo.OldLai,
                       sizeof(MM_LAI_STRU));

            NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);

            PS_MEM_CPY(&g_MmGlobalInfo.MsCsInfo.OldLai,
                       &OldLai,
                       sizeof(MM_LAI_STRU));

            NAS_MM_UpdateEquPlmnInfo(NAS_MML_REG_FAIL_CAUSE_RR_CONN_EST_FAIL);

            NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_TRUE,
                                        MM_MMC_LU_RESULT_FAILURE,
                                        NAS_MML_REG_FAIL_CAUSE_RR_CONN_EST_FAIL);

            NAS_MM_SndGmmLuResultInd(MMGMM_LU_FAILURE,
                                     NAS_MML_REG_FAIL_CAUSE_RR_CONN_EST_FAIL);

            NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
            PS_MEM_CPY(&g_MmGlobalInfo.MsCsInfo.OldLai,
                       &TempLai,
                       sizeof(MM_LAI_STRU));
        }
        else
        {
            Mm_ComSetMmState(MM_IDLE_ATTEMPTING_TO_UPDATE);
        }

        Mm_TimerStart(MM_TIMER_T3211);                                  /* ����T3211                                */
    }

}



VOS_VOID NAS_MM_ProcCsForbidenFlg_RcvSysInfoInd(
    VOS_UINT32                          ulCurFobidnFlg
)
{
    VOS_UINT32                          ulCause;    
    
    if (MMCMM_FORBIDDEN_PLMN == (MMCMM_FORBIDDEN_PLMN & ulCurFobidnFlg))
    {        
        ulCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN;
    }    
    else if (MMCMM_FORBIDDEN_LA_FOR_REG_PRVS_SVS == (MMCMM_FORBIDDEN_LA_FOR_REG_PRVS_SVS & ulCurFobidnFlg))
    {
        /* ��MMCMM_FORBIDDEN_LA_FOR_REG_PRVS_SVS�Ĵ���ʱ����Ҫ֪ͨMMC��CAUSEֵ */
        ulCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_REG_PRVS_SVS;
        
        if (MM_WAIT_CS_DETACH == (g_MmGlobalInfo.stDetachInfo.enDetachType & MM_WAIT_CS_DETACH))
        {
            NAS_MM_SndMmcDetachCnf(MMC_MM_LIMITED_SERVICE);
            
            /* ����ȴ���־ */
            g_MmGlobalInfo.stDetachInfo.enDetachType = MM_WAIT_NULL_DETACH;
        }
    }
    else
    {
        /* ��MMCMM_FORBIDDEN_LA_FOR_ROAM�Ĵ���ʱ����Ҫ֪ͨMMC��CAUSEֵ */
        ulCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_ROAM;
    }

    /* ��FORBIDEN��ͳһ���� */
    g_MmGlobalInfo.ucMmServiceState = MM_LIMITED_SERVICE;

    Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);

    NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                MM_MMC_LU_RESULT_FAILURE,
                                ulCause);
    
    NAS_MM_SndGmmLuResultInd(MMGMM_LU_FAILURE,
                             NAS_MML_REG_FAIL_CAUSE_NULL);        

    return;
}



VOS_VOID NAS_MM_SndMmcRrConn_RcvRelInd(VOS_VOID)
{
    VOS_UINT8                           ucTiValidFlg = MM_TRUE;

    if ((MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC]
                      .RcvXXEstReq.ucFlg)
     ||(MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS]
                      .RcvXXEstReq.ucFlg)
     ||(MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS]
                      .RcvXXEstReq.ucFlg))
    {
        ucTiValidFlg = Mm_ComTiChk();
        if (MM_FALSE == ucTiValidFlg)
        {
            Mm_SndMmcRrConnInfInd(MMC_MM_RR_CONN_ABSENT);                   /* RR���Ӳ�����                             */

            NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_NOT_EXIST);
        }
        else
        {
            Mm_ComSigRelForDelayMmConnRej();
        }
    }
    else
    {
        Mm_SndMmcRrConnInfInd(MMC_MM_RR_CONN_ABSENT);                      /* RR���Ӳ�����                             */

        NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_NOT_EXIST);
    }

}




VOS_VOID NAS_MM_LocalDetach_MmInterRatChange(VOS_VOID)
{
    Mm_TimerStop(MM_TIMER_STOP_ALL);  

    /* ֪ͨGMM��ǰCS���ѱ���DETACH��ʱ���� */
    NAS_MM_SndGmmCsLocalDetachInd();

    Mm_ComRelAllMmConn(NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);

    NAS_MML_SetCsAttachAllowFlg(VOS_FALSE);
    Mm_ComNasInfoSav(MM_STATUS_DETACHED,
                     NAS_MML_GetSimCsSecurityCksn());                           /* ����MM��Ϣ                               */

    /* ���÷���״̬ΪDETACHED */
    g_MmGlobalInfo.ucMmServiceState = MM_DETACHED;

    /* �жϵȴ���־�Ƿ���ڣ������������MMCMM_DETACH CNF */
    if (MM_WAIT_CS_DETACH== (g_MmGlobalInfo.stDetachInfo.enDetachType
                 & MM_WAIT_CS_DETACH))
    {
        NAS_MM_SndMmcDetachCnf(MM_DETACHED);
        /* ����ȴ���־ */
        g_MmGlobalInfo.stDetachInfo.enDetachType = MM_WAIT_NULL_DETACH;
    }

    /* ����ȫ�ֱ�����CS����б���detach����Ҫ����ɾ�� */
    if ( ( MMC_SUSPEND_CAUSE_CELLCHANGE == g_MmGlobalInfo.stSuspendShare.ucSuspendCause )
      && ( MM_IDLE_NORMAL_SERVICE == g_MmGlobalInfo.ucSuspendPreState ) )
    {
        NAS_MM_InitCcoFallbackCtx();
    }

    /* ����mmǨ����״̬,3212�ڹ���״̬δֹͣ,��ʱ��Ҫֹͣ */
    Mm_TimerStop(MM_TIMER_T3212);

    if (MM_IDLE_NO_IMSI == g_MmGlobalInfo.ucSuspendPreState)
    {
        g_MmGlobalInfo.ucMmServiceState = MM_NO_IMSI;
        g_MmGlobalInfo.ucSuspendPreState = MM_IDLE_NO_IMSI;
        g_MmGlobalInfo.ucPreState = MM_IDLE_NO_IMSI;
    }
    else
    {
        g_MmGlobalInfo.ucSuspendPreState = MM_IDLE_LIMITED_SERVICE;
        g_MmGlobalInfo.ucPreState = MM_IDLE_LIMITED_SERVICE;
        g_MmGlobalInfo.ucMmServiceState = MM_DETACHED;
    }

    NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                MM_MMC_LU_RESULT_FAILURE,
                                NAS_MML_REG_FAIL_CAUSE_MS_CFG_DOMAIN_NOT_SUPPORT);

    g_MmGlobalInfo.stBackupShare.ucState                    = g_MmGlobalInfo.ucSuspendPreState;

    g_MmGlobalInfo.stBackupShare.ucPreState                 = g_MmGlobalInfo.ucSuspendPreState;

    g_MmGlobalInfo.stBackupShare.enMmServiceState           = g_MmGlobalInfo.ucMmServiceState;

    return;
}
VOS_VOID NAS_MM_LocalDetach_MmNoImsi(VOS_VOID)
{
    NAS_MML_CONN_STATUS_INFO_STRU      *pstConnStatus = VOS_NULL_PTR;

    pstConnStatus = NAS_MML_GetConnStatus();

    /* ����ȥע��ͨ�ò��� */
    Mm_TimerStop(MM_TIMER_STOP_ALL);

    /* ֪ͨGMM��ǰCS���ѱ���DETACH��ʱ���� */
    NAS_MM_SndGmmCsLocalDetachInd();

    Mm_ComRelAllMmConn(NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);

    NAS_MML_SetCsAttachAllowFlg(VOS_FALSE);

    /* �������ɹ���ֱ�ӱ���DETACH */
    Mm_ComNasInfoSav(MM_STATUS_DETACHED,
                     NAS_MML_GetSimCsSecurityCksn());

    /* ���÷���״̬ΪDETACHED */
    g_MmGlobalInfo.ucMmServiceState = MM_DETACHED;

    /* �жϵȴ���־�Ƿ���ڣ������������MMCMM_DETACH CNF */
    if (MM_WAIT_CS_DETACH== (g_MmGlobalInfo.stDetachInfo.enDetachType & MM_WAIT_CS_DETACH))
    {
        NAS_MM_SndMmcDetachCnf(MM_NO_IMSI);

        /* ����ȴ���־ */
        g_MmGlobalInfo.stDetachInfo.enDetachType = MM_WAIT_NULL_DETACH;
    }

    /* ֪ͨMMC��ǰ����Ч */
    NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                MM_MMC_LU_RESULT_FAILURE,
                                NAS_MML_REG_FAIL_CAUSE_SIM_INVALID);

    /* �����ǰ���������ӣ����ͷ����� */
    if ( VOS_TRUE == pstConnStatus->ucCsSigConnStatusFlg )
    {
        Mm_SndRrRelReq(RRC_CELL_UNBARRED);
    }


    return;
}
VOS_VOID NAS_MM_RcvMmTimerProtectCsDetachExpired(VOS_VOID)
{ 
    NAS_MM_LocalDetach();
    
    return;
    
}


VOS_VOID NAS_MM_LocalDetach(VOS_VOID)
{   
    NAS_MML_CONN_STATUS_INFO_STRU      *pstConnStatus = VOS_NULL_PTR;
    VOS_UINT32                          ulMmState;
    
    pstConnStatus = NAS_MML_GetConnStatus();    
    ulMmState     = Mm_GetState();

    /* ����״̬���� */
    switch ( ulMmState )
    {
        case MM_INTER_RAT_CHANGE :

            /* ��ϵͳʱ��ı���ȥע�ᴦ���������ֱ�ӷ��� */
            NAS_MM_LocalDetach_MmInterRatChange();
            return;

        case MM_IDLE_NO_IMSI :

            /* NO IMSI�ı���ȥע�ᴦ���������ֱ�ӷ��� */
            NAS_MM_LocalDetach_MmNoImsi();
            return;

        /* ������״̬Ǩ�ƵĴ��� */
        case MM_STATE_NULL :
            break;

        default:
            Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
            break;
    }

    /* ����ȥע��ͨ�ò��� */
    Mm_TimerStop(MM_TIMER_STOP_ALL);

    /* ֪ͨGMM��ǰCS���ѱ���DETACH��ʱ���� */
    NAS_MM_SndGmmCsLocalDetachInd();

    Mm_ComRelAllMmConn(NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);

    NAS_MML_SetCsAttachAllowFlg(VOS_FALSE);

    /* �������ɹ���ֱ�ӱ���DETACH */
    Mm_ComNasInfoSav(MM_STATUS_DETACHED,
                     NAS_MML_GetSimCsSecurityCksn());

    /* ���÷���״̬ΪDETACHED */
    g_MmGlobalInfo.ucMmServiceState = MM_DETACHED;

    /* �жϵȴ���־�Ƿ���ڣ������������MMCMM_DETACH CNF */
    if (MM_WAIT_CS_DETACH== (g_MmGlobalInfo.stDetachInfo.enDetachType & MM_WAIT_CS_DETACH))
    {
        NAS_MM_SndMmcDetachCnf(MM_DETACHED);
        
        /* ����ȴ���־ */
        g_MmGlobalInfo.stDetachInfo.enDetachType = MM_WAIT_NULL_DETACH;
    }

    /* ֪ͨMMC��ǰ������֧�� */
    NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                MM_MMC_LU_RESULT_FAILURE,
                                NAS_MML_REG_FAIL_CAUSE_MS_CFG_DOMAIN_NOT_SUPPORT);

    /* �����ǰ���������ӣ����ͷ����� */
    if ( VOS_TRUE == pstConnStatus->ucCsSigConnStatusFlg )
    {
        Mm_SndRrRelReq(RRC_CELL_UNBARRED);
    }

    return;
    
}


VOS_VOID NAS_MM_StartDetachProtectingTimer(
    MMCMM_DETACH_REQ_STRU              *pstMsg
)
{
    /* Detach���̱�����ʱ��ʱ�� */
    VOS_UINT32                          ulDetachTimerVal;

    /* MM��CS��Detach������ʱ������״̬ */
    VOS_UINT32                          ulCsDetachTimerStatus;

    /* Ĭ�Ϸ�SYSCFG������DETACH��ʱ��: 90s */
    ulDetachTimerVal = NAS_MM_TIMER_PROTECT_DETACH_NOT_BY_SYSCFG_VALUE;

    /* ��ȡMM��Detach������ʱ������״̬ */
    ulCsDetachTimerStatus = NAS_MM_QryTimerStatus(MM_TIMER_PROTECT_CS_DETACH);

    /* �����ǰ��SYSCFG������DETACH,�����ñ�����ʱ����ʱ��Ϊ15s */
    if (  VOS_TRUE == NAS_MMA_GetServiceDomainSet() )
    {
        ulDetachTimerVal = NAS_MM_TIMER_PROTECT_DETACH_BY_SYSCFG_VALUE;
    }

    /* ���MM��CS��Detach������ʱ�������У�������CS��Detach������ʱ�� */
    if ( MM_TIMER_RUNNING == ulCsDetachTimerStatus )
    {
        NAS_WARNING_LOG(WUEPS_PID_MM, "NAS_MM_StartDetachProtectingTimer:WARN:TIMER HAS BEEN STARTED ALREADY");
        Mm_TimerStop(MM_TIMER_PROTECT_CS_DETACH);
    }
    
    /* ����MM��CS��Detach������ʱ��ʱ�� */
    gstMmTimer[MM_TIMER_PROTECT_CS_DETACH].ulTimerLen = ulDetachTimerVal;

    /* ����MM��CS��detach������ʱ�� */
    Mm_TimerStart(MM_TIMER_PROTECT_CS_DETACH);

    return;
}



VOS_VOID NAS_MM_RcvRrMmEstCnf_ExistCsDetachBuffer(
    RRMM_EST_CNF_STRU                  *pRrcEstCnf
)
{
    MM_MSG_IMSI_DETACH_IND_STRU         stMsg;

    Mm_TimerStop(MM_TIMER_PROTECT_SIGNALLING); 

    Mm_ComRelAllMmConn(NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES);

    /* �����ǰ�����ɹ������л����û������IMSI detach������ֱ�ӷ���ȥע�� */
    if ( (RRC_EST_SUCCESS == pRrcEstCnf->ulResult)
      || (RRC_EST_PRESENT == pRrcEstCnf->ulResult))
    {

        /* ����DETACH���ͣ��������յ�RELʱ�򣬲��ظ�MMC��DETACH CNF */
        g_MmGlobalInfo.ucDetachType = MM_DETACH_CS_NORMAL; 
        
        g_MmGlobalInfo.LuInfo.ucImmAccRejLuAttmptCnt = 0;
        g_MmGlobalInfo.ucCsSigConnFlg = MM_CS_SIG_CONN_PRESENT;             /* �������������Ѵ���                       */
        Mm_SndMmcRrConnInfInd(MMC_MM_RR_CONN_ESTED);                        /* RR���ӽ����ɹ�                           */
        
        /* ��д��Ϣ,����IMSI detach��Ϣ */
        NAS_MM_FillMobileID(&(stMsg.MmIeMobileId.MobileID));
        Mm_ComMsgImsiDetachIniSndOfDataReq(&stMsg);                         
        Mm_TimerStart(MM_TIMER_T3220);                                      /* ����T3220                                */
        Mm_ComSetMmState(IMSI_DETACH_INITIATED);
        return;
    }

    /* MM���б���DETACH */
    NAS_MM_LocalDetach();
    
    return;
    
}
VOS_UINT32 NAS_MM_IsNeedGmmProcUserDetach(VOS_VOID)
{
    NAS_MML_CAMP_PLMN_INFO_STRU        *pstCampInfo       = VOS_NULL_PTR;
    NAS_MML_NETWORK_INFO_STRU          *pstNetWorkInfo    = VOS_NULL_PTR;
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU   *pstForbidPlmnInfo = VOS_NULL_PTR;
    VOS_UINT32                          ulRet;

    pstForbidPlmnInfo = NAS_MML_GetForbidPlmnInfo();

    /* ��ȡ��ǰ����ģʽ */
    pstCampInfo       = NAS_MML_GetCurrCampPlmnInfo();

    /* ��ȡ��ǰ��������Ϣ */
    pstNetWorkInfo    =  NAS_MML_GetNetworkInfo();

    ulRet             = VOS_FALSE;
    
    
    /* ������ģʽI�£�����GMM�����û������detach */
    if ( NAS_MML_NET_MODE_I != pstCampInfo->enNetworkMode )
    {
        return VOS_FALSE;
    }

    /* PS��׼��ע�ᣬ����GMM�����û������detach */
    if ( VOS_FALSE == NAS_MML_GetPsAttachAllowFlg() )
    {
        return VOS_FALSE;
    }

    /* PS��ע�����ޣ�����GMM�����û������detach */
    if ( VOS_TRUE == NAS_MML_GetPsRestrictRegisterFlg() )
    {
        return VOS_FALSE;
    }

    /* PS ����Ч������GMM�����û������detach */
    if ( VOS_FALSE == NAS_MML_GetSimPsRegStatus())
    {
        return VOS_FALSE;
    }

    /* ��ǰС���Ƿ�֧��GPRS */
    if ( VOS_FALSE == pstNetWorkInfo->stPsDomainInfo.ucPsSupportFlg )
    {
        return VOS_FALSE;
    }

    /* ��ǰС���Ƿ���FORB GPRS PLMN���� */

    ulRet = NAS_MML_IsSimPlmnIdInDestBcchPlmnList( &(pstCampInfo->stLai.stPlmnId),
                                           pstForbidPlmnInfo->ucForbGprsPlmnNum,
                                           pstForbidPlmnInfo->astForbGprsPlmnList );
    if ( VOS_TRUE == ulRet )
    {
        return VOS_FALSE;
    }

    /* MM�Ѵ���ATTACHED״̬������ǰGS�ӿڲ����� */
    if ( (VOS_FALSE          == g_MmSubLyrShare.GmmShare.ucGsAssociationFlg)
      && (MM_STATUS_ATTACHED == g_MmSubLyrShare.MmShare.ucCsAttachState) )
    {
        return VOS_FALSE;
    }
   
    /* �����û������CS���DETACH������GMM���д��� */    
    return VOS_TRUE;
    
}



VOS_UINT32 NAS_MM_IsEmergencyCallEndTrigLau(VOS_VOID)
{
    VOS_UINT8                           ucEmergencyServiceFlg;
    VOS_UINT8                           ucCsAttachAllowFlg;
    VOS_UINT8                           ucSimCsRegStatus;                       /* SIM��CS���ע�������µĿ��Ƿ���ЧVOS_TRUE:CS��Ŀ���Ч,VOS_FALSE:CS��Ŀ���Ч*/
    VOS_UINT8                           ucRestrictRegister;                     /*�Ƿ�����ע�� */

    NAS_MML_CAMP_PLMN_INFO_STRU        *pstCampInfo         = VOS_NULL_PTR;
    NAS_MML_CONN_STATUS_INFO_STRU      *pstConnStatus       = VOS_NULL_PTR; 

    pstConnStatus         = NAS_MML_GetConnStatus();
    pstCampInfo           = NAS_MML_GetCurrCampPlmnInfo();
    ucEmergencyServiceFlg = NAS_MML_GetCsEmergencyServiceFlg();
    ucCsAttachAllowFlg    = NAS_MML_GetCsAttachAllowFlg();
    ucSimCsRegStatus      = NAS_MML_GetSimCsRegStatus();
    ucRestrictRegister    = NAS_MML_GetCsRestrictRegisterFlg();

    /* CSδע���£��������������յ�attach cs�����Ȼظ�attach��������������������ͷź�
       �ж������ǰ��PSҵ����Ҫ����LAU */
    if ( (VOS_TRUE            == pstConnStatus->ucRrcStatusFlg)
      && (VOS_TRUE            == ucEmergencyServiceFlg)
      && (MM_STATUS_DETACHED  == g_MmSubLyrShare.MmShare.ucCsAttachState)
      && (VOS_TRUE            == ucCsAttachAllowFlg)
      && (VOS_TRUE            == ucSimCsRegStatus)
      && (VOS_FALSE           == ucRestrictRegister)
      && (MM_TIMER_RUNNING    != gstMmTimer[MM_TIMER_T3212].ucTimerStatus )
      && (MM_TIMER_RUNNING    != gstMmTimer[MM_TIMER_T3211].ucTimerStatus )
      && (NAS_MML_NET_MODE_II  == pstCampInfo->enNetworkMode)
      && ((MMCMM_NO_FORBIDDEN            == g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg)
       || (MMCMM_FORBIDDEN_PLMN_FOR_GPRS == g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg)))
    {
        return VOS_TRUE;
    }

    
    return VOS_FALSE;
    
}


VOS_UINT32 NAS_MM_RelEstingMmConn(
    RRC_REL_CAUSE_ENUM_UINT32           ulRelCause
)
{
    if ( NO_MM_CONN_ESTING !=
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI )
    {
        if (VOS_TRUE == NAS_MM_IsNeedCmServiceRetry_RelIndResult(ulRelCause))
        {
            /* ��CC���ڽ�����MM����                     */
            Mm_SndCcRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI,
                           NAS_MMCM_REL_CAUSE_AS_REJ_LOW_LEVEL_FAIL);            /* ֪ͨCC,MM���ӽ���ʧ��                    */
        }
        else
        {
            /* ��CC���ڽ�����MM����                     */
            Mm_SndCcRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI,
                           NAS_MMCM_REL_CAUSE_AS_REJ_OTHER_CAUSES);
        }


        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].ucMMConnEstingTI =
            NO_MM_CONN_ESTING;                                          /* ������ڽ�����MM���ӵı�־               */
    }

    if ( NO_MM_CONN_ESTING !=
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].ucMMConnEstingTI )
    {                                                                   /* ��SS���ڽ�����MM����                     */
        if (VOS_TRUE == NAS_MM_IsNeedCmServiceRetry_RelIndResult(ulRelCause))
        {
            Mm_SndSsRelInd( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].ucMMConnEstingTI,
                            NAS_MMCM_REL_CAUSE_AS_REJ_LOW_LEVEL_FAIL);
        }
        else
        {
            Mm_SndSsRelInd( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].ucMMConnEstingTI,
                            NAS_MMCM_REL_CAUSE_AS_REJ_OTHER_CAUSES);                                          /* ֪ͨSS,MM���ӽ���ʧ��                    */
        }

        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].ucMMConnEstingTI =
            NO_MM_CONN_ESTING;                                          /* ������ڽ�����MM���ӵı�־               */
    }

    if ( NO_MM_CONN_ESTING !=
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].ucMMConnEstingTI )
    {                                                                   /* ��SMS���ڽ�����MM����                    */
        if (VOS_TRUE == NAS_MM_IsNeedCmServiceRetry_RelIndResult(ulRelCause))
        {
            Mm_SndSmsRelInd( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].ucMMConnEstingTI,
                            NAS_MMCM_REL_CAUSE_AS_REJ_LOW_LEVEL_FAIL);
        }
        else
        {
            Mm_SndSmsRelInd( g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].ucMMConnEstingTI,
                            NAS_MMCM_REL_CAUSE_AS_REJ_OTHER_CAUSES);                                          /* ֪ͨSS,MM���ӽ���ʧ��                    */
        }

        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].ucMMConnEstingTI =
            NO_MM_CONN_ESTING;                                          /* ������ڽ�����MM���ӵı�־               */
    }

    return VOS_TRUE;

}


VOS_UINT32 NAS_MM_RelBufferedServiceEstReq(VOS_VOID)
{
    if ( MM_TRUE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg )
    {                                                                           /* �еȴ���CC��������Ҫ������MM����         */
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].
                                                RcvXXEstReq.ucFlg
            = MM_FALSE;

        Mm_SndCcRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulTransactionId,
             NAS_MMCM_REL_CAUSE_AS_REJ_OTHER_CAUSES);                          /* ֪ͨCC����ʧ��                           */
    }
    if ( MM_TRUE ==
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].
                                    RcvXXEstReq.ucFlg )
    {                                                                           /* �еȴ���SMS��������Ҫ������MM����         */
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].
                                RcvXXEstReq.ucFlg = MM_FALSE;
        Mm_SndSmsRelInd(
            g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS]
            .RcvXXEstReq.ulTransactionId,
            NAS_MMCM_REL_CAUSE_AS_REJ_OTHER_CAUSES);
    }
    if ( MM_TRUE ==
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].
                                RcvXXEstReq.ucFlg )
    {                                                                           /* �еȴ���SS��������Ҫ������MM����         */
        g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].
                                RcvXXEstReq.ucFlg = MM_FALSE;

        Mm_SndSsRelInd(g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ulTransactionId,
                       NAS_MMCM_REL_CAUSE_AS_REJ_OTHER_CAUSES);
    }

    return VOS_TRUE;

}




VOS_UINT32 NAS_MM_NotifyBufferedServiceRRConnNotExist(VOS_VOID)
{
    VOS_UINT8                           ucTiValidFlg;

    if ( (MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC]
                  .RcvXXEstReq.ucFlg)
      || (MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS]
                  .RcvXXEstReq.ucFlg)
      || (MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS]
                  .RcvXXEstReq.ucFlg))
    {
        ucTiValidFlg = Mm_ComTiChk();
        if ( MM_FALSE == ucTiValidFlg )
        {
            Mm_SndMmcRrConnInfInd( MMC_MM_RR_CONN_ABSENT );         /* RR���Ӳ�����                             */
            NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_NOT_EXIST);
        }
        else
        {
            Mm_ComSigRelForDelayMmConnRej();
        }

        return VOS_TRUE;
    }

    return VOS_FALSE;

}




VOS_UINT32 NAS_MM_IsDisableGprsCombineAttach(VOS_VOID)
{

    NAS_MML_CAMP_PLMN_INFO_STRU        *pstCampInfo       = VOS_NULL_PTR;
    NAS_MML_NETWORK_INFO_STRU          *pstNetWorkInfo    = VOS_NULL_PTR;
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU   *pstForbidPlmnInfo = VOS_NULL_PTR;
    VOS_UINT32                          ulRet;

    pstForbidPlmnInfo = NAS_MML_GetForbidPlmnInfo();

    /* ��ȡ��ǰ����ģʽ */
    pstCampInfo       = NAS_MML_GetCurrCampPlmnInfo();

    /* ��ȡ��ǰ��������Ϣ */
    pstNetWorkInfo    =  NAS_MML_GetNetworkInfo();

    ulRet             = VOS_FALSE;

    /* CS��ע�����ޣ�MM����Ҫ��LAU */
    if ( VOS_TRUE == NAS_MML_GetCsRestrictRegisterFlg() )
    {
        return VOS_FALSE;
    }


    /* ������ģʽI�£�MM��Ҫ�Լ���LAU */
    if ( NAS_MML_NET_MODE_I != pstCampInfo->enNetworkMode )
    {
        return VOS_TRUE;
    }

    /* PS��׼��ע�ᣬMM��Ҫ�Լ���LAU */
    if ( VOS_FALSE == NAS_MML_GetPsAttachAllowFlg() )
    {
        return VOS_TRUE;
    }

    /* PS��ע�����ޣ�MM��Ҫ�Լ���LAU */
    if ( VOS_TRUE == NAS_MML_GetPsRestrictRegisterFlg() )
    {
        return VOS_TRUE;
    }

    /* PS ����Ч������GMM�����û������detach */
    if ( VOS_FALSE == NAS_MML_GetSimPsRegStatus())
    {
        return VOS_TRUE;
    }

    /* ��ǰС���Ƿ�֧��GPRS */
    if ( VOS_FALSE == pstNetWorkInfo->stPsDomainInfo.ucPsSupportFlg )
    {
        return VOS_TRUE;
    }

    /* ��ǰС���Ƿ���FORB GPRS PLMN���� */
    ulRet = NAS_MML_IsSimPlmnIdInDestBcchPlmnList( &(pstCampInfo->stLai.stPlmnId),
                                           pstForbidPlmnInfo->ucForbGprsPlmnNum,
                                           pstForbidPlmnInfo->astForbGprsPlmnList );
    if ( VOS_TRUE == ulRet )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_VOID NAS_MM_RestartT3240_WaitForNetworkCommand(VOS_VOID)
{
    NAS_MML_CONN_STATUS_INFO_STRU      *pstConnStatus       = VOS_NULL_PTR;
    VOS_UINT32                          ulIsTestCard;
    VOS_UINT8                           ucSupportCsServiceFlg;                  /* �Ƿ�֧��cs����ҵ�������ͨ������vpҵ��VOS_TRUE:֧��cs����ҵ��VOS_FALSE:��֧��cs����ҵ��*/
    VOS_UINT8                           ucCsServiceConnStatusFlg;   

    pstConnStatus            = NAS_MML_GetConnStatus();
    ulIsTestCard             = NAS_USIMMAPI_IsTestCard();
    ucSupportCsServiceFlg    = NAS_MML_GetSupportCsServiceFLg();
    ucCsServiceConnStatusFlg = pstConnStatus->ucCsServiceConnStatusFlg;

    /* ͣT3240 */    
    Mm_TimerStop( MM_TIMER_T3240 );                                             

    /* ��ǰ���Կ�����ǰ����Ϊ��֧��CSҵ�񣬵�ǰ��CSҵ��������T3240���ֵ */
    if ( (VOS_TRUE  == ulIsTestCard)
      && (VOS_FALSE == ucSupportCsServiceFlg)
      && (VOS_TRUE  == ucCsServiceConnStatusFlg) )
    {
        gstMmTimer[MM_TIMER_T3240].ulTimerLen = MM_TIMER_MAX_VALUE;
        Mm_TimerStart( MM_TIMER_T3240 );
        gstMmTimer[MM_TIMER_T3240].ulTimerLen = MM_TIMER_T3240_VALUE;            
    }
    else
    {
        Mm_TimerStart( MM_TIMER_T3240 );                                        /* ����T3240                                  */
    }

}

VOS_VOID NAS_MM_ResumeMmStateToPreState_LocationUpdatingPending(VOS_VOID)
{
    VOS_UINT8                           ucPreMmState;

    ucPreMmState  = g_MmGlobalInfo.ucPreState;

    /* CS����Ч��״̬Ǩ�� */
    if ( VOS_FALSE == NAS_MML_GetSimCsRegStatus() )
    {
        Mm_ComSetMmState(MM_IDLE_NO_IMSI);

        return;
    }

    /* CS��׼��ע���״̬Ǩ�� */
    if ( VOS_FALSE == NAS_MML_GetCsAttachAllowFlg() )
    {
        Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
        
        return;
    } 

    /* ��ǰ��ֹע���״̬Ǩ�� */
    if ( (MMCMM_NO_FORBIDDEN            != g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg)
      && (MMCMM_FORBIDDEN_PLMN_FOR_GPRS != g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg) )
    {
        Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);

        return;
    }

    /* CS�����޵�״̬Ǩ�� */    
    if ( VOS_TRUE == NAS_MML_GetCsRestrictRegisterFlg() )
    {
        Mm_ComSetMmState(MM_IDLE_LOCATION_UPDATE_NEEDED);
        
        return;        
    }

    /* MM��ǰ��MM_IDLE_NORMAL_SERVICE״̬������µ�ǰ״̬ΪMM_IDLE_NORMAL_SERVICE */
    if ( MM_IDLE_NORMAL_SERVICE == ucPreMmState )
    {
        Mm_ComSetMmState(MM_IDLE_NORMAL_SERVICE);

        return;
    }

    /* MM��ǰ��MM_IDLE_ATTEMPTING_TO_UPDATE״̬������µ�ǰ״̬ΪMM_IDLE_ATTEMPTING_TO_UPDATE */
    if ( MM_IDLE_ATTEMPTING_TO_UPDATE == ucPreMmState )
    {
        Mm_ComSetMmState(MM_IDLE_ATTEMPTING_TO_UPDATE);

        return;
    }

    /* ͳһǨ�Ƶ�MM_IDLE_ATTEMPTING_TO_UPDATE״̬ */
    Mm_ComSetMmState(MM_IDLE_ATTEMPTING_TO_UPDATE);

}
VOS_UINT32 NAS_MM_EndBufferedCSServiceExceptEmergencyCall_ImmediateAssignReject()
{
    /* ���ݻ���ҵ������ͣ���ȡ��Ӧ�ĺ���ָ���Լ�TransActionId */
    if ( (MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ucFlg)
      && (MMCC_MO_NORMAL_CALL == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_CC].RcvXXEstReq.ulCallType) )
    {
        Mm_SndCcRelInd(g_MmCcEstReq.ulTransactionId, NAS_MMCM_REL_CAUSE_AS_REJ_OTHER_CAUSES);
    }

    if (MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SS].RcvXXEstReq.ucFlg)
    {
        Mm_SndSsRelInd(g_MmSsEstReq.ulTi, NAS_MMCM_REL_CAUSE_AS_REJ_OTHER_CAUSES);
    }

    if (MM_TRUE == g_MmGlobalInfo.ConnCtrlInfo[MM_CONN_CTRL_SMS].RcvXXEstReq.ucFlg)
    {
        Mm_SndSmsRelInd(g_MmSmsEstReq.ulTi, NAS_MMCM_REL_CAUSE_AS_REJ_OTHER_CAUSES);
    }

    /* ֪ͨMMC��CS���ҵ���Ѿ�ֹͣ */
    Mm_SndMmcCmSvcInd(MM_CS_SERV_NOT_EXIST);
    NAS_MM_SndGmmCsConnectInd(MMGMM_CS_CONNECT_NOT_EXIST);

    return VOS_TRUE;
}
VOS_UINT32  NAS_MM_ProcessLuAfterCsConnectNotExist_NetModeI( VOS_VOID )
{
    VOS_UINT8                           ucRet;
    VOS_UINT8                           ucOldUpdateSts;
    VOS_UINT8                           ucTempUpdateSts;
    MM_LAI_STRU                         OldLai;
    MM_LAI_STRU                         TempLai;

    ucRet = MM_FALSE;

    if ( MM_UNSUPPORT_GPRS == g_MmGlobalInfo.usRac )
    {                                                               /* RACΪ0�ĳ���                             */
        g_MmGlobalInfo.ucLikeB = MM_TRUE;                           /* ����������Bģʽ��ȫ����                  */
        PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "Mm_ComChkLu:INFO: MmLikeB is MM_TRUE");
        if ( ( MMCMM_NO_FORBIDDEN !=
            g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg ) &&
            ( MMCMM_FORBIDDEN_PLMN_FOR_GPRS !=
            g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg ) )
        {
            if ( MMCMM_FORBIDDEN_PLMN
                == (MMCMM_FORBIDDEN_PLMN &
                g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg)
                )
            {
                g_MmGlobalInfo.ucMmServiceState =
                    MM_LIMITED_SERVICE;                             /* ���÷���״̬                             */
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "NAS_MM_CheckLuAfterCsConnectNotExist:INFO: MmServiceState is MM_LIMITED_SERVICE");
                Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);
    
                NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                            MM_MMC_LU_RESULT_FAILURE,
                                            NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN);
                NAS_MM_SndGmmLuResultInd(MMGMM_LU_FAILURE,
                                         NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW);
    
    
    
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "NAS_MM_CheckLuAfterCsConnectNotExist:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
            }
            else if ( MMCMM_FORBIDDEN_LA_FOR_REG_PRVS_SVS
                == (MMCMM_FORBIDDEN_LA_FOR_REG_PRVS_SVS
                    & g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg)
                )
            {
                g_MmGlobalInfo.ucMmServiceState =
                    MM_LIMITED_SERVICE;                             /* ���÷���״̬                             */
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "NAS_MM_CheckLuAfterCsConnectNotExist:INFO: MmServiceState is MM_LIMITED_SERVICE");
                ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
                PS_MEM_CPY( &OldLai,
                             &g_MmGlobalInfo.MsCsInfo.OldLai,
                             sizeof( MM_LAI_STRU ));
                Mm_ComLuCauseHandle(NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW);
    
                ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
                PS_MEM_CPY( &TempLai,
                             &g_MmGlobalInfo.MsCsInfo.OldLai,
                             sizeof( MM_LAI_STRU ));
                NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
                PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                             &OldLai,
                             sizeof( MM_LAI_STRU ));
    
    
                NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                            MM_MMC_LU_RESULT_FAILURE,
                                            NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_REG_PRVS_SVS);
                NAS_MM_SndGmmLuResultInd(MMGMM_LU_FAILURE,
                                         NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW);
                NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
                PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                             &TempLai,
                             sizeof( MM_LAI_STRU ));
            }
            else
            {
                g_MmGlobalInfo.ucMmServiceState =
                    MM_LIMITED_SERVICE;                             /* ���÷���״̬                             */
                PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "NAS_MM_CheckLuAfterCsConnectNotExist:INFO: MmServiceState is MM_LIMITED_SERVICE");
                ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
                PS_MEM_CPY( &OldLai,
                             &g_MmGlobalInfo.MsCsInfo.OldLai,
                             sizeof( MM_LAI_STRU ));
                Mm_ComLuCauseHandle(NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW);
    
                ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
                PS_MEM_CPY( &TempLai,
                             &g_MmGlobalInfo.MsCsInfo.OldLai,
                             sizeof( MM_LAI_STRU ));
                NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
                PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                             &OldLai,
                             sizeof( MM_LAI_STRU ));
    
    
                NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                            MM_MMC_LU_RESULT_FAILURE,
                                            NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_ROAM);
                NAS_MM_SndGmmLuResultInd(MMGMM_LU_FAILURE,
                                         NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW);
                NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
                PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                             &TempLai,
                             sizeof( MM_LAI_STRU ));
            }
        }
        else
        {
           g_MmGlobalInfo.LuInfo.ucLuType = MM_IE_LUT_NORMAL_LU;    /* ����LU����                               */
           Mm_ComLuOnly();                                          /* LU REQ                                   */
           ucRet = MM_TRUE;
        }
    }

    return ucRet;
}
VOS_UINT32  NAS_MM_ProcessLuAfterCsConnectNotExist_OhterNetMode( VOS_VOID )
{                                                                   /* ��A+Iģʽ                                */
    VOS_UINT8                           ucRet;
    VOS_UINT8                           ucOldUpdateSts;
    VOS_UINT8                           ucTempUpdateSts;
    MM_LAI_STRU                         OldLai;
    MM_LAI_STRU                         TempLai;

    ucRet = MM_FALSE;

    if ( ( MMCMM_NO_FORBIDDEN !=
        g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg ) &&
        ( MMCMM_FORBIDDEN_PLMN_FOR_GPRS !=
        g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg ) )
    {                                                               /* ���û��CS���ֹ����Ϣ                   */
        if ( MMCMM_FORBIDDEN_PLMN == ( MMCMM_FORBIDDEN_PLMN &
            g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg) )
        {
            g_MmGlobalInfo.ucMmServiceState = MM_LIMITED_SERVICE;   /* ���÷���״̬                             */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "NAS_MM_CheckLuAfterCsConnectNotExist:INFO: MmServiceState is MM_LIMITED_SERVICE");
            Mm_ComSetMmState(MM_IDLE_LIMITED_SERVICE);

            NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                        MM_MMC_LU_RESULT_FAILURE,
                                        NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN);
            NAS_MM_SndGmmLuResultInd(MMGMM_LU_FAILURE,
                                     NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW);


            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_NORMAL, "NAS_MM_CheckLuAfterCsConnectNotExist:NORMAL: STATUS is MM_IDLE_LIMITED_SERVICE");
        }
        else if ( MMCMM_FORBIDDEN_LA_FOR_REG_PRVS_SVS
            == ( MMCMM_FORBIDDEN_LA_FOR_REG_PRVS_SVS
                & g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg ) )
        {
            g_MmGlobalInfo.ucMmServiceState = MM_LIMITED_SERVICE;   /* ���÷���״̬                             */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "NAS_MM_CheckLuAfterCsConnectNotExist:INFO: MmServiceState is MM_LIMITED_SERVICE");
            ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
            PS_MEM_CPY( &OldLai,
                         &g_MmGlobalInfo.MsCsInfo.OldLai,
                         sizeof( MM_LAI_STRU ));
            Mm_ComLuCauseHandle(NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW);

            ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
            PS_MEM_CPY( &TempLai,
                         &g_MmGlobalInfo.MsCsInfo.OldLai,
                         sizeof( MM_LAI_STRU ));
            NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
            PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                         &OldLai,
                         sizeof( MM_LAI_STRU ));


            NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                        MM_MMC_LU_RESULT_FAILURE,
                                        NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_REG_PRVS_SVS);
            NAS_MM_SndGmmLuResultInd(MMGMM_LU_FAILURE,
                                     NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW);
            NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
            PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                         &TempLai,
                         sizeof( MM_LAI_STRU ));
        }
        else
        {
            g_MmGlobalInfo.ucMmServiceState = MM_LIMITED_SERVICE;   /* ���÷���״̬                             */
            PS_LOG(WUEPS_PID_MM, VOS_NULL, PS_PRINT_INFO, "NAS_MM_CheckLuAfterCsConnectNotExist:INFO: MmServiceState is MM_LIMITED_SERVICE");
            ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
            PS_MEM_CPY( &OldLai,
                         &g_MmGlobalInfo.MsCsInfo.OldLai,
                         sizeof( MM_LAI_STRU ));
            Mm_ComLuCauseHandle(NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW);

            ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
            PS_MEM_CPY( &TempLai,
                         &g_MmGlobalInfo.MsCsInfo.OldLai,
                         sizeof( MM_LAI_STRU ));
            NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
            PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                         &OldLai,
                         sizeof( MM_LAI_STRU ));

            NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_FALSE,
                                        MM_MMC_LU_RESULT_FAILURE,
                                        NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_ROAM);
            NAS_MM_SndGmmLuResultInd(MMGMM_LU_FAILURE,
                                     NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW);
            NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
            PS_MEM_CPY( &g_MmGlobalInfo.MsCsInfo.OldLai,
                         &TempLai,
                         sizeof( MM_LAI_STRU ));
        }
    }
    else
    {
        g_MmGlobalInfo.LuInfo.ucLuType = MM_IE_LUT_NORMAL_LU;       /* ����LU����                               */
        Mm_ComLuOnly();                                             /* ����LU REQ                               */
        ucRet = MM_TRUE;
    }

    return ucRet;
}
VOS_UINT32  NAS_MM_ProcessLuAfterCsConnectNotExist_T3212Expire( 
    VOS_UINT8                           ucPsRestrictionFlg
)
{
    VOS_UINT8                           ucRet;
    
    ucRet = MM_FALSE;
    
    switch ( g_MmGlobalInfo.ucState )
    {
    case MM_IDLE_NORMAL_SERVICE:
        ucRet = MM_TRUE;

        if((VOS_TRUE == g_MmSubLyrShare.GmmShare.ucGsAssociationFlg)
            && (VOS_TRUE == ucPsRestrictionFlg))
        {
            g_MmGlobalInfo.LuInfo.ucLuType = MM_IE_LUT_NORMAL_LU;               /* ����LU����                               */
        }
        else
        {
            g_MmGlobalInfo.LuInfo.ucLuType = MM_IE_LUT_PERIODIC_UPDATING;       /* ����LU����                               */
        }

        Mm_ComLuOnly();                                                     /* ����LU REQ                               */
        break;
    case MM_IDLE_ATTEMPTING_TO_UPDATE:
        ucRet = MM_TRUE;
        g_MmGlobalInfo.LuInfo.ucLuType = MM_IE_LUT_NORMAL_LU;               /* ����LU����                               */
        Mm_ComLuOnly();                                                     /* ����LU REQ                               */
        break;
    default:
        break;
    }

    return ucRet;
}
VOS_UINT32  NAS_MM_ProcessLuAfterCsConnectNotExist_RrcExist( VOS_VOID )
{
    VOS_UINT8                           ucRet;
    VOS_UINT8                           ucFlg;

    ucRet = MM_FALSE;
    ucFlg = MM_FALSE;
    
    g_MmGlobalInfo.ucConnRcvSysFlg = MM_FALSE;

    g_MmGlobalInfo.ucLauAcceptContainDiffNbLaiFlg = VOS_FALSE;

    switch ( g_MmGlobalInfo.ucState )
    {
    case MM_IDLE_NORMAL_SERVICE:
    case MM_IDLE_ATTEMPTING_TO_UPDATE:
    case MM_IDLE_LIMITED_SERVICE:
    case WAIT_FOR_RR_CONNECTION_MM_CONNECTION:
        ucFlg = MM_TRUE;
        break;
    default:
        break;
    }
    if ( MM_TRUE == ucFlg )
    {
        if ( (VOS_TRUE == NAS_MML_GetPsAttachAllowFlg())
          && (MM_NET_MODE_I == g_MmGlobalInfo.ucNtMod)
          && ( MM_FALSE == g_MmGlobalInfo.ucLikeB ) )
        {                                                                   /* ���ΪA+I,�Ҳ�������B��A+II����ĳ���    */
            ucRet = (VOS_UINT8)NAS_MM_ProcessLuAfterCsConnectNotExist_NetModeI();
        }
        else
        {
            ucRet = (VOS_UINT8)NAS_MM_ProcessLuAfterCsConnectNotExist_OhterNetMode();
        }
    }

    return ucRet;
}
VOS_UINT8 NAS_MM_ProcessLuAfterCsConnectNotExist(VOS_VOID)
{
    VOS_UINT8                           ucRet;
    VOS_UINT8                           ucFlg;
    VOS_UINT8                           ucPsRestrictionFlg;

    ucRet = MM_FALSE;
    ucFlg = MM_FALSE;
    ucPsRestrictionFlg      = NAS_MML_GetPsRestrictRegisterFlg();


    if ( g_MmGlobalInfo.ucNtMod != g_MmGlobalInfo.ucNewNtMod )
    {
        if ( ( MMCMM_FORBIDDEN_PLMN == g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg )
          || ( MMCMM_FORBIDDEN_LA_FOR_ROAM == g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg )
          || ( MMCMM_FORBIDDEN_LA_FOR_REG_PRVS_SVS == g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg ))
        {
            return ucRet;
        }

        g_MmGlobalInfo.ucNtMod = g_MmGlobalInfo.ucNewNtMod;
        switch ( g_MmGlobalInfo.ucState )
        {
        case MM_IDLE_NORMAL_SERVICE:
        case MM_IDLE_ATTEMPTING_TO_UPDATE:
        case MM_IDLE_LIMITED_SERVICE:
            ucFlg = MM_TRUE;
            break;
        default:
            break;
        }
        if  ( MM_NET_MODE_II == g_MmGlobalInfo.ucNtMod )
        {
            if (MM_TRUE == ucFlg)
            {
                g_MmGlobalInfo.LuInfo.ucLuType = MM_IE_LUT_NORMAL_LU;           /* ����LU����                               */
                Mm_ComLuOnly();                                                 /* LU REQ                                   */
                ucRet = MM_TRUE;
                return ucRet;
            }
        }
    }

    if (VOS_TRUE == NAS_MM_IsNeedLauCsSigRel())
    {
        ucRet = (VOS_UINT8)NAS_MM_ProcessLuAfterCsConnectNotExist_RrcExist(); 
    }
    else if ( MM_TRUE == g_MmGlobalInfo.LuInfo.ucT3212ExpiredFlg )
    {
        ucRet = (VOS_UINT8)NAS_MM_ProcessLuAfterCsConnectNotExist_T3212Expire(ucPsRestrictionFlg);
    }
    else
    {

    }

    return ucRet;
}
VOS_VOID  NAS_MM_LogAuthInfo(
    VOS_UINT8                           ucRcvOpId,
    VOS_UINT8                           ucExpectOpId
)
{
    NAS_MM_LOG_AUTH_INFO_STRU         *pstMsg = VOS_NULL_PTR;

    pstMsg = (NAS_MM_LOG_AUTH_INFO_STRU*)PS_MEM_ALLOC(WUEPS_PID_MM,
                                         sizeof(NAS_MM_LOG_AUTH_INFO_STRU));

    if (VOS_NULL_PTR == pstMsg)
    {
        NAS_ERROR_LOG(WUEPS_PID_MM, "NAS_MM_LogAuthInfo:ERROR:Alloc Mem Fail.");
        return;
    }

    PS_MEM_SET(pstMsg, 0x00, sizeof(NAS_MM_LOG_AUTH_INFO_STRU));

    pstMsg->stMsgHeader.ulSenderCpuId   = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulSenderPid     = WUEPS_PID_MM;
    pstMsg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MM;
    pstMsg->stMsgHeader.ulLength        = sizeof(NAS_MM_LOG_AUTH_INFO_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMsg->stMsgHeader.ulMsgName       = MMOM_LOG_AUTH_INFO_IND;
    pstMsg->ucExpectOpId                = ucExpectOpId;
    pstMsg->ucRcvOpId                   = ucRcvOpId;

    OM_TraceMsgHook(pstMsg);

    PS_MEM_FREE(WUEPS_PID_MM, pstMsg);

    return;
}



VOS_VOID  NAS_MM_ClearAuthInfo(VOS_VOID)
{
    g_MmGlobalInfo.AuthenCtrlInfo.ucAuthenAttmptCnt = 0;
    g_MmGlobalInfo.AuthenCtrlInfo.ucLastFailCause   = MM_AUTHEN_NO_CAUSE;

    g_MmGlobalInfo.AuthenCtrlInfo.ucRandRepeat      = MM_AUTHEN_RAND_DIFFER;

    /* 24.008 4.3.2.2
    The RAND and RES values stored in the mobile station shall be deleted and timer T3218, if running, shall be stopped:
    -   if the mobile station enters the MM state MM IDLE or NULL.
    */
    Mm_TimerStop(MM_TIMER_T3218);

    Mm_ComDelRandRes();

    if (MM_RCV_AUTH_CNF_FLG == (MM_RCV_AUTH_CNF_FLG & g_MmGlobalInfo.ucRcvAgentFlg))
    {
        g_MmGlobalInfo.ucRcvAgentFlg &= ~MM_RCV_AUTH_CNF_FLG;

        Mm_TimerStop(MM_TIMER_PROTECT_AGENT);
    }


    return;
}


#if (FEATURE_ON == FEATURE_LTE)  

VOS_VOID NAS_MM_RcvMmTimerGetHoSecuInfoCnfExpired(VOS_VOID)
{ 
    if ((NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
     && (MM_INTER_RAT_CHANGE      == g_MmGlobalInfo.ucState))
    {
        /* ��MMC�ظ�����rsp */
        NAS_MM_SndMmcSuspendRsp();        
    }
   
    return;    
}
VOS_UINT8 NAS_MM_GetSrvccFlg(VOS_VOID)
{ 
    return g_MmGlobalInfo.ucSrvccFlg;    
}


VOS_VOID NAS_MM_SetSrvccFlg(
    VOS_UINT8                           ucFlg
)
{ 
    g_MmGlobalInfo.ucSrvccFlg = ucFlg;    
}



VOS_VOID NAS_MM_ConvertToLmmSrvccStatus(
    NAS_MMCC_SRVCC_STATUS_ENUM_UINT32   enSrvccStatus,
    MM_LMM_SRVCC_STATUS_ENUM_UINT32    *penMmlSrvccStatus
)
{
    switch(enSrvccStatus)
    {
        case NAS_MMCC_SRVCC_STATUS_START:
            *penMmlSrvccStatus = MM_LMM_SRVCC_STATUS_START;
            break;
        case NAS_MMCC_SRVCC_STATUS_SUCCESS:
            *penMmlSrvccStatus = MM_LMM_SRVCC_STATUS_SUCC;
            break;
        case NAS_MMCC_SRVCC_STATUS_FAIL:
            *penMmlSrvccStatus = MM_LMM_SRVCC_STATUS_FAIL;
            break;
        default:
            *penMmlSrvccStatus = MM_LMM_SRVCC_STATUS_BUTT;
            break;
    }

    return;
}

#endif



VOS_UINT32 Mm_IsMultiSrvCollisionAllow(NAS_MM_CONN_CTRL_ENUM_UINT8  ucMmConnCtrl)
{
    if ((WAIT_FOR_RR_CONNECTION_MM_CONNECTION == g_MmGlobalInfo.ucState)
     || (WAIT_FOR_OUTGOING_MM_CONNECTION      == g_MmGlobalInfo.ucState)
     || (WAIT_FOR_NETWORK_COMMAND             == g_MmGlobalInfo.ucState))
    {
        /* ��ǰ��һ������ҵ�����ڽ��� */
        if (MM_MT_PAGING == g_MmGlobalInfo.ucPagingType)
        {        
            if (NAS_MML_NET_RAT_TYPE_GSM == NAS_MML_GetCurrNetRatType())
            {            
                /* ��ǰ�������ΪGSM���ܾ���ǰ��MO����ҵ������ԭ��: GSMҵ��������ҵ�����ӽ����׶Σ�
                 * �޷�֪����ǰ��ҵ������
                 */
                if (MM_CONN_CTRL_CC == ucMmConnCtrl)
                {
                    return VOS_FALSE;
                }

                return VOS_TRUE;
            }
            else
            {
                if ((MM_CONN_CTRL_CC                                 == ucMmConnCtrl)
                 && (RRC_PAGE_CAUSE_TERMINAT_CONVERSAT_CALL          == g_MmGlobalInfo.ucPagingCause))
                {
                    /* �����ǰMOҵ������Ϊ����ҵ����MTҵ������Ҳ�Ǻ��У��ܾ���ǰ��MOҵ�� */
                    return VOS_FALSE;
                }               
                else
                {
                    /* ��������,����ǰҵ���� */
                    return VOS_TRUE;
                }
            }
        }

        /* ������Ϊ�˹�ܶ��MOҵ������ͬʱ����ʱ�Ĵ�������
         * 
         * ע: δ���߰汾�����Կ��Ǹ���T3246��ʱ����ʽ������(Ref: section 4.5.1.1/4.5.1.2 
         *     in 24.008 version 10.6.1)
         */
        if (WAIT_FOR_NETWORK_COMMAND != g_MmGlobalInfo.ucState)
        {
            if ((MM_IE_PD_NON_CALL_RLT_SS_MSG == g_MmGlobalInfo.ucMMConnEstingPD)
             && (MM_CONN_CTRL_CC              == ucMmConnCtrl))
            {
                /* ��ǰһ��MO SSҵ���������ڽ���������յ�����ҵ�����󣬾ܾ���ǰ����ҵ������ */        
                return VOS_FALSE;
            }
            else if ((MM_IE_PD_CALL_CONTROL   == g_MmGlobalInfo.ucMMConnEstingPD)
                  && (MM_CONN_CTRL_SS         == ucMmConnCtrl))
            {
                /* ��ǰһ��MO ����ҵ���������ڽ���������յ�����ҵ�����󣬾ܾ���ǰ����ҵ������ */              
                return VOS_FALSE;
            }
            else
            {
                return VOS_TRUE;
            }
        }
    }
    
    return VOS_TRUE;
}




MM_LAI_STRU *NAS_MM_GetAttemptUpdateLaiInfo(VOS_VOID)
{
    return &(g_MmGlobalInfo.MsCsInfo.stAttemptToUpdateLai);
}


VOS_VOID NAS_MM_InitLaiInfo(
    MM_LAI_STRU                        *pstLai
)
{
    pstLai->PlmnId.ulMcc = NAS_MML_INVALID_MCC;
    pstLai->PlmnId.ulMnc = NAS_MML_INVALID_MNC;
    pstLai->ulLac        = MM_INVALID_LAI;
    
    return;
}


VOS_UINT32 NAS_MM_IsSameLaiInfo(
    MM_LAI_STRU                        *pstCurrLai,
    MM_LAI_STRU                        *pstOldLai
)
{
    if ( pstCurrLai->PlmnId.ulMcc != pstOldLai->PlmnId.ulMcc )
    {
        return VOS_FALSE;
    }

    if ( pstCurrLai->PlmnId.ulMnc != pstOldLai->PlmnId.ulMnc )
    {
        return VOS_FALSE;
    }
    
    if ( pstCurrLai->ulLac != pstOldLai->ulLac)
    {
        return VOS_FALSE;
    }
   
    
    return VOS_TRUE;
}




VOS_VOID NAS_MM_LocationUpdateAttemptCounter(
    VOS_UINT32                          ulMmCause
)
{    NAS_MML_LOCATION_UPDATE_STATUS_ENUM_UINT8  enCsUpdateStatus;    
    VOS_UINT8                                   ucOldUpdateSts;
    VOS_UINT8                                   ucTempUpdateSts;
    MM_LAI_STRU                                 OldLai;
    MM_LAI_STRU                                 TempLai;

    /* ������1 */
    g_MmGlobalInfo.LuInfo.ucLuAttmptCnt++;

    if (g_MmGlobalInfo.LuInfo.ucLuAttmptCnt < MM_CONST_NUM_4)
    {
        enCsUpdateStatus = NAS_MML_GetCsUpdateStatus();
        if ( (MM_FALSE == Mm_ComLaiJudge())
         &&  (NAS_MML_LOCATION_UPDATE_STATUS_UPDATED == enCsUpdateStatus))
        {
            g_MmGlobalInfo.ucMmServiceState = MM_NORMAL_SERVICE;

            NAS_MM_UpdateEquPlmnInfo(ulMmCause);

            NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_TRUE,
                                        MM_MMC_LU_RESULT_FAILURE,
                                        ulMmCause);                    /* ��MMC����MMCMM_LU_RESULT_IND             */
            NAS_MM_SndGmmLuResultInd(MMGMM_LU_FAILURE,
                                     ulMmCause);                    /* ��MMC����MMCMM_LU_RESULT_IND             */
            Mm_ComNasInfoSav(MM_STATUS_ATTACHED,
                             NAS_MML_GetSimCsSecurityCksn());
            g_MmGlobalInfo.ucPreState = g_MmGlobalInfo.ucState;                 /* ��¼ǰ״̬                               */
            Mm_ComSetMmState(MM_IDLE_NORMAL_SERVICE);
            Mm_TimerStop(MM_TIMER_T3211);
            Mm_TimerStart(MM_TIMER_T3211);                                      /* ����T3211                                */


        }
        else
        {
            g_MmGlobalInfo.ucMmServiceState = MM_ATTEMPTING_TO_UPDATE;
            
            ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();

            PS_MEM_CPY(&OldLai,
                       &g_MmGlobalInfo.MsCsInfo.OldLai,
                       sizeof(MM_LAI_STRU));


            /* ��U2״̬�����浱ǰ��LAI��Ϣ */
            PS_MEM_CPY(NAS_MM_GetAttemptUpdateLaiInfo(), 
                       &g_MmGlobalInfo.MsCsInfo.CurLai, sizeof(MM_LAI_STRU));            

            Mm_ComToU2Handle();                                                 /* ��U2�Ĺ�������                           */

            ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();

            PS_MEM_CPY(&TempLai,
                       &g_MmGlobalInfo.MsCsInfo.OldLai,
                       sizeof(MM_LAI_STRU));

            NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);

            PS_MEM_CPY(&g_MmGlobalInfo.MsCsInfo.OldLai,
                       &OldLai,
                       sizeof(MM_LAI_STRU));

            NAS_MM_UpdateEquPlmnInfo(ulMmCause);

            NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_TRUE,
                                        MM_MMC_LU_RESULT_FAILURE,
                                        ulMmCause);
            NAS_MM_SndGmmLuResultInd(MMGMM_LU_FAILURE,
                                     ulMmCause);
            NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);

            PS_MEM_CPY(&g_MmGlobalInfo.MsCsInfo.OldLai,
                       &TempLai,
                       sizeof(MM_LAI_STRU));

            Mm_TimerStop(MM_TIMER_T3211);
            Mm_TimerStart(MM_TIMER_T3211);                                      /* ����T3211                                */
        }
    }
    else
    {
        g_MmGlobalInfo.ucMmServiceState = MM_ATTEMPTING_TO_UPDATE;
        ucOldUpdateSts = NAS_MML_GetCsUpdateStatus();
        PS_MEM_CPY(&OldLai,
                   &g_MmGlobalInfo.MsCsInfo.OldLai,
                   sizeof(MM_LAI_STRU));


        /* ��U2״̬�����浱ǰ��LAI��Ϣ */
        PS_MEM_CPY(NAS_MM_GetAttemptUpdateLaiInfo(), 
                   &g_MmGlobalInfo.MsCsInfo.CurLai, sizeof(MM_LAI_STRU));            
            
        Mm_ComToU2Handle();                                                     /* ��U2�Ĺ�������                           */
        ucTempUpdateSts = NAS_MML_GetCsUpdateStatus();
        PS_MEM_CPY(&TempLai,
                   &g_MmGlobalInfo.MsCsInfo.OldLai,
                   sizeof(MM_LAI_STRU));
        NAS_MML_SetCsUpdateStatus(ucOldUpdateSts);
        PS_MEM_CPY(&g_MmGlobalInfo.MsCsInfo.OldLai,
                   &OldLai,
                   sizeof(MM_LAI_STRU));

        NAS_MM_UpdateEquPlmnInfo(ulMmCause);

        NAS_MM_SndMmcCsRegResultInd(MM_MMC_LU_PROC_TRUE,
                                    MM_MMC_LU_RESULT_FAILURE,
                                    ulMmCause);
        NAS_MM_SndGmmLuResultInd(MMGMM_LU_FAILURE,
                                 ulMmCause);

        NAS_MML_SetCsUpdateStatus(ucTempUpdateSts);
        PS_MEM_CPY(&g_MmGlobalInfo.MsCsInfo.OldLai,
                   &TempLai,
                   sizeof(MM_LAI_STRU));
        Mm_TimerStop(MM_TIMER_T3212);                                           /* T3212�Ƚ���ֹͣ                                */
        Mm_TimerStart(MM_TIMER_T3212);                                          /* ����T3212                                */
    }
}


VOS_VOID NAS_MM_UpdateLauRetryFlg_RcvRelInd(
    RRMM_REL_IND_STRU                  *pRrcRelInd
)
{   
    NAS_MML_LOCATION_UPDATE_STATUS_ENUM_UINT8   enCsUpdateStatus;

    enCsUpdateStatus    = NAS_MML_GetCsUpdateStatus();

    /* ucLuAttmptCnt�Ǽ�1֮��Ĵ��� */
    if (g_MmGlobalInfo.LuInfo.ucLuAttmptCnt < MM_CONST_NUM_4)
    {
        if ( (MM_FALSE == Mm_ComLaiJudge())
         &&  (NAS_MML_LOCATION_UPDATE_STATUS_UPDATED == enCsUpdateStatus))
        {
            g_MmGlobalInfo.LuInfo.ucRetryFlg = MM_FALSE;         
        }
        else
        {
            if ((RRC_REL_CAUSE_RR_UNSPEC == pRrcRelInd->ulRelCause)
                || (RRC_REL_CAUSE_NAS_DATA_ABSENT == pRrcRelInd->ulRelCause)
                || (RRC_REL_CAUSE_RRC_ERROR == pRrcRelInd->ulRelCause)
                || (RRC_REL_CAUSE_RL_FAILURE == pRrcRelInd->ulRelCause)
                || (RRC_REL_CAUSE_OTHER_REASON == pRrcRelInd->ulRelCause))
            {
                g_MmGlobalInfo.LuInfo.ucRetryFlg = MM_FALSE;
            }
            else
            {
                g_MmGlobalInfo.LuInfo.ucRetryFlg = MM_TRUE;
            }
        }
    }
    else
    {
        if ((RRC_REL_CAUSE_RR_UNSPEC == pRrcRelInd->ulRelCause)
            || (RRC_REL_CAUSE_NAS_DATA_ABSENT == pRrcRelInd->ulRelCause)
            || (RRC_REL_CAUSE_RRC_ERROR == pRrcRelInd->ulRelCause)
            || (RRC_REL_CAUSE_RL_FAILURE == pRrcRelInd->ulRelCause)
            || (RRC_REL_CAUSE_OTHER_REASON == pRrcRelInd->ulRelCause))
        {
            g_MmGlobalInfo.LuInfo.ucRetryFlg = MM_FALSE;
        }
        else
        {
            g_MmGlobalInfo.LuInfo.ucRetryFlg = MM_TRUE;
        }
    }
}



#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

