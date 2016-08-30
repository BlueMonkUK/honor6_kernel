/******************************************************************************

                  ��Ȩ���� (C), 2012-2013, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : audio_debug.c
  �� �� ��   : ����
  ��    ��   : ������ W164657
  ��������   : 2012��7��26��
  ����޸�   :
  ��������   : ���Թ���
  �����б�   :
              AUDIO_DEBUG_Init
              AUDIO_DEBUG_Info_Query
              AUDIO_DEBUG_SendHookInd
              AUDIO_DEBUG_MsgSetHookReq

  �޸���ʷ   :
  1.��    ��   : 2012��7��26��
    ��    ��   : ������ W164657
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "ucom_config.h"
#include "ucom_nv.h"
#include "audio_debug.h"
#include "om_comm.h"
#include "med_drv_timer_hifi.h"
#include "audio_enhance.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    ��ά�ɲ���Ϣ�а�����C�ļ���ź궨��
*****************************************************************************/
/*lint -e(767)*/
#define THIS_FILE_ID                    OM_FILE_ID_AUDIO_DEBUG_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/* HOOKʹ�ܡ���ȡĿ�ꡢ֡�� */
AUDIO_HOOK_STRU   g_stAudioDbgHookObj = {CODEC_SWITCH_ON, 0x07};

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*****************************************************************************
 �� �� ��  : AUDIO_DEBUG_Init
 ��������  : ��Ƶ����ģ���ʼ��
 �������  : pstAudioTraceCfg       --�����NV���������̶�NV��ʽ
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��26��
    ��    ��   : ������ W164657
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID AUDIO_DEBUG_Init( CODEC_NV_PARA_AUDIO_TRACE_CFG_STRU *pstAudioTraceCfg )
{
    AUDIO_HOOK_STRU               *pstHookCtrl = AUDIO_UtilGetHookPtr();

    /* �������nv�������г�ʼ����ֵ */
    pstHookCtrl->enHookEnable   = (pstAudioTraceCfg->stAudioHookCfg).uhwEnable;
    pstHookCtrl->usHookTarget   = (pstAudioTraceCfg->stAudioHookCfg).uhwTarget;

}
VOS_UINT32 AUDIO_DEBUG_MsgQueryStatusReq(VOS_VOID *pstOsaMsg)
{
    MSG_AUDIO_OM_QUERY_RSP_STRU         stQueryMsg;
    AUDIO_PCM_CFG_STRU                 *pstPcmCfg   = VOS_NULL;

    UCOM_MemSet(&stQueryMsg, 0, sizeof(stQueryMsg));

    /* ��ȡ����ͨ�����������������ṹ�� */
    pstPcmCfg   = AUDIO_PcmGetPcmCfgPtr(AUDIO_PCM_MODE_PLAYBACK);

    stQueryMsg.usMsgId              = ID_AUDIO_OM_QUERY_STATUS_CNF;
    stQueryMsg.enPlayBackStatus     = pstPcmCfg->enPcmStatus;
    stQueryMsg.uwPlayBackSampleRate = pstPcmCfg->uwSampleRate;
    stQueryMsg.uwPlayBackChannelNum = pstPcmCfg->uwChannelNum;

    /* ��ȡ¼��ͨ�����������������ṹ�� */
    pstPcmCfg   = AUDIO_PcmGetPcmCfgPtr(AUDIO_PCM_MODE_CAPTURE);

    stQueryMsg.enCaptureStatus      = pstPcmCfg->enPcmStatus;
    stQueryMsg.enCaptureMode        = pstPcmCfg->enCaptureMode;
    stQueryMsg.uwCaptureSampleRate  = pstPcmCfg->uwSampleRate;
    stQueryMsg.uwCaptureChannelNum  = pstPcmCfg->uwChannelNum;

    /* ��ȡ��Ƶ��ǿͨ·��ǰģ�� */
    stQueryMsg.uwEnhanceModule      = AUDIO_ENHANCE_GetCurrentModule();

    /*����OM_COMM_SendTranMsg�ӿڽ���Transͷ��䣬�����͸�SDT*/
    OM_COMM_SendTranMsg(&stQueryMsg, sizeof(MSG_AUDIO_OM_QUERY_RSP_STRU));

    OM_LogInfo(AUDIO_MsgQueryStatusReq_Ok);

    return UCOM_RET_SUCC;
}

/*****************************************************************************
 �� �� ��  : AUDIO_DEBUG_MsgSetHookReq
 ��������  : ��Ƶ��ȡ��������ԭ�ﴦ����
 �������  : pstOsaMsg          --��Ƶ���ݹ�ȡ������ϢID_AUDIO_OM_SET_HOOK_REQ
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��26��
    ��    ��   : ������ W164657
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 AUDIO_DEBUG_MsgSetHookReq(VOS_VOID *pstOsaMsg)
{
    VOS_UINT32                            uwRet      = UCOM_RET_SUCC;
    MSG_OM_AUDIO_HOOK_REQ_STRU           *pstPrim    = VOS_NULL;
    AUDIO_HOOK_STRU                      *pstHook    = AUDIO_UtilGetHookPtr();
    MSG_CODEC_CNF_STRU                    stRspPrim;

    if (VOS_NULL == pstOsaMsg)
    {
        return UCOM_RET_NUL_PTR;
    }

    UCOM_MemSet(&stRspPrim, 0, sizeof(stRspPrim));

    /* ԭ��ID�Ϸ����ж������Ѿ�����Ϣ״̬����֤���˴������ж� */
    pstPrim = (MSG_OM_AUDIO_HOOK_REQ_STRU *)pstOsaMsg;

    /* �����Ϸ����ж� */
    if ((CODEC_SWITCH_BUTT <= pstPrim->enHookEnable)
       || (AUDIO_HOOK_BUTT <= pstPrim->usHookTarget))
    {
        uwRet = UCOM_RET_ERR_PARA;

        /* ��¼��־��Ϣ */
        OM_LogError(AUDIO_MsgSetHookReq_Fail);
    }
    else
    {
        /* ��ԭ���л�ȡ���ݹ�ȡ��ر���ֵ����ȫ�ֱ��� */
        pstHook->enHookEnable   = pstPrim->enHookEnable;
        pstHook->usHookTarget   = pstPrim->usHookTarget;

        uwRet = UCOM_RET_SUCC;

        /* ��¼��־��Ϣ */
        OM_LogInfo(AUDIO_MsgSetHookReq_Ok);
    }

    /* �����ϢID��ִ�н�� */
    stRspPrim.usMsgName = ID_AUDIO_OM_SET_HOOK_CNF;
    stRspPrim.enExeRslt = (VOS_UINT16)uwRet;

    /* ��SDT�ظ���ϢID_AUDIO_OM_SET_HOOK_CNF */
    OM_COMM_SendTranMsg(&stRspPrim, sizeof(stRspPrim));

    return uwRet;
}

/*****************************************************************************
 �� �� ��  : AUDIO_DEBUG_SendHookInd
 ��������  : ��ȡָ��λ�����ݽ����ϱ�
 �������  : uhwPos:  ��ȡ����λ��
             pvData:  ��ȡ����buffer��ַ
             uhwLen:  ��ȡ����ʵ�����ݳ���(32bit)
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��26��
    ��    ��   : ������ W164657
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID AUDIO_DEBUG_SendHookInd(
                VOS_UINT16              uhwPos,
                VOS_VOID               *pvData,
                VOS_UINT32              uwLen)
{
    MSG_AUDIO_OM_HOOK_STRU       stHookHead;
    UCOM_DATA_BLK_STRU           astBlk[2];
    AUDIO_HOOK_STRU             *pstHookObj  = AUDIO_UtilGetHookPtr();
    VOS_UCHAR                   *pvSubData   = (VOS_UCHAR *)pvData;
    VOS_UINT16                   uhwHookLen  = 0;
    VOS_UINT16                   uhwBlkCnt   = 0;
    VOS_UINT16                   uhwLoop     = 0;

    if (CODEC_SWITCH_OFF == pstHookObj->enHookEnable )
    {
        return;
    }

    if(uhwPos == (uhwPos & (pstHookObj->usHookTarget)))
    {

        uhwBlkCnt = ((VOS_UINT16)uwLen / AUDIO_MAX_HOOK_LEN) + 1;

        /* ���ԭ������ */
        stHookHead.usMsgId      = (VOS_UINT16)ID_AUDIO_OM_HOOK_IND;
        stHookHead.usHookTarget = uhwPos;

        /* ѭ������blk���ݣ�ÿ�����3840 byte pvData */
        for(uhwLoop = 0; uhwLoop < uhwBlkCnt; uhwLoop++)
        {
            if(uhwLoop != (uhwBlkCnt - 1))
            {
                uhwHookLen = AUDIO_MAX_HOOK_LEN;
            }
            else
            {
                uhwHookLen = (VOS_UINT16)(uwLen - (uhwLoop * AUDIO_MAX_HOOK_LEN));
            }

            stHookHead.ulTimeStamp  = DRV_TIMER_ReadSysTimeStamp();
            stHookHead.usHookLength = uhwHookLen;

            /* ����ַ�ͳ��� */
            astBlk[0].pucData       = (VOS_UCHAR *)&stHookHead;
            astBlk[0].uwSize        = sizeof(stHookHead);

            /* ����ַ�ͳ��� */
            astBlk[1].pucData       = (VOS_UCHAR *)pvSubData;
            astBlk[1].uwSize        = uhwHookLen;

            /* �ϱ�HOOK��Ϣ */
            OM_COMM_SendTrans(astBlk, 2);

            pvSubData               = pvSubData + uhwHookLen;
        }
    }

    pvSubData = VOS_NULL;
}
VOS_UINT32 AUDIO_DEBUG_MsgAudioLoopOpenReq(VOS_VOID *pvOsaMsg)
{
    /* ������ʵ�� */
    AUDIO_LOOP_CNF_STRU         stAudioLoopCnfMsg;

    /* �ж����������Ƿ�ɹ� */
    if ( UCOM_RET_SUCC == DRV_SIO_Audio_Loop(DRV_SIO_SAMPLING_48K))
    {
        stAudioLoopCnfMsg.enAudioLoopCnf    = AUDIO_PLAY_RESULT_OK;
    }
    else
    {
        stAudioLoopCnfMsg.enAudioLoopCnf    = AUDIO_PLAY_RESULT_FAIL;
        DRV_SIO_Audio_Close();
    }

    /* ��AUDIO_PLAYER_DONE_IND_STRU��ʽ�����Ϣ */
    stAudioLoopCnfMsg.uhwMsgId          = ID_AUDIO_AP_LOOP_BEGIN_CNF;


    /* ���ÿ����Ƶchannel�ӿ�,֪ͨAP��Ƶ���ؽ�� */
    UCOM_SendAudioMsg(DSP_PID_AUDIO,
                      ACPU_PID_OM,           /* �˴���дAP��DRV��Ӧ��PID������ */
                     &stAudioLoopCnfMsg,
                      sizeof(AUDIO_LOOP_CNF_STRU));

    return UCOM_RET_SUCC;
}
VOS_UINT32 AUDIO_DEBUG_MsgAudioLoopCloseReq(VOS_VOID *pvOsaMsg)
{
    AUDIO_LOOP_CNF_STRU         stAudioLoopCnfMsg;

    /* �ر�SIO */
    DRV_SIO_Audio_Close();

    /* ��AUDIO_PLAYER_DONE_IND_STRU��ʽ�����Ϣ */
    stAudioLoopCnfMsg.uhwMsgId          = ID_AUDIO_AP_LOOP_END_CNF;
    stAudioLoopCnfMsg.enAudioLoopCnf    = AUDIO_PLAY_RESULT_OK;

    /* ���ÿ����Ƶchannel�ӿ�,֪ͨAP�ر���Ƶ���� */
    UCOM_SendAudioMsg(DSP_PID_AUDIO,
                      ACPU_PID_OM,           /* �˴���дAP��DRV��Ӧ��PID,,���� */
                     &stAudioLoopCnfMsg,
                      sizeof(AUDIO_LOOP_CNF_STRU));

    return UCOM_RET_SUCC;
}


VOS_UINT32  AUDIO_DEBUG_MsgTestCmd(VOS_VOID *pvOsaMsg)
{
    VOS_UINT32          uwRetValue  = 0x55aa55aa;
    TEST_HIFI_MSG_STRU *pstTestMsg  = (TEST_HIFI_MSG_STRU*)((MsgBlock *) pvOsaMsg)->aucValue;

    VOS_UINT32          uwLen       = sizeof(TEST_HIFI_MSG_STRU);

    UCOM_COMM_SendMsg(DSP_PID_AUDIO,DSP_PID_AUDIO_RT,DRV_MAILBOX_CHANNEL_VOS_CH,
        ((VOS_CHAR*)pstTestMsg+uwLen),pstTestMsg->ulMsgLen);

    UCOM_SendTestMsg(DSP_PID_AUDIO,ACPU_PID_OM,&uwRetValue,sizeof(VOS_UINT32));

    return UCOM_RET_SUCC;
}

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

