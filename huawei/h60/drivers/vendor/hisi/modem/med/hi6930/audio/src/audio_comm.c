

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "om.h"
#include "fsm.h"
#include "audio_player.h"
#include "audio_recorder.h"
#include "audio_pcm.h"
#include "audio_comm.h"
#include "ucom_nv.h"
#include "audio_debug.h"
#include "dm3.h"
#include "audio_enhance.h"
#include "xa_dts_pp.h"
#include "audio_recorder.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    ��ά�ɲ���Ϣ�а�����C�ļ���ź궨��
*****************************************************************************/
/*lint -e(767)*/
#define THIS_FILE_ID                    OM_FILE_ID_AUDIO_COMM_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/* AUDIO��Ϣ״̬��(����,������״̬�л�) */
STA_STRU g_stAudioStatusDesc;

/* AUDIOʵʱ��Ϣ״̬��(����,������״̬�л�) */
STA_STRU g_stAudioRtStatusDesc;

/* AUDIO��ʵʱ��Ϣ�����,�˴�û������ */
ACT_STRU g_astAudioMsgFuncTable[] =
{
    /* ������ʽ: PID(reserve), ��Ϣ����, ��Ӧ������ */
    FSM_ACT_TBL_ITEM(0x0000, ID_AUDIO_UPDATE_PLAY_BUFF_CMD,     AUDIO_PCM_MsgUpdatePlayBuffCmd),
    FSM_ACT_TBL_ITEM(0x0000, ID_AUDIO_UPDATE_CAPTURE_BUFF_CMD,  AUDIO_PCM_MsgUpdateCaptureBuffCmd),
    FSM_ACT_TBL_ITEM(0x0000, ID_OM_AUDIO_SET_HOOK_REQ,          AUDIO_DEBUG_MsgSetHookReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_OM_AUDIO_QUERY_STATUS_REQ,      AUDIO_DEBUG_MsgQueryStatusReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_AUDIO_UPDATE_PLAYER_BUFF_CMD,   AUDIO_PLAYER_MsgUpdatePcmBuffCmd),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_HIFI_TEST_CMD,               AUDIO_DEBUG_MsgTestCmd),
    FSM_ACT_TBL_ITEM(0x0000, ID_AUDIO_OM_SET_NV_REQ,            AUDIO_ENHANCE_MsgOmSetNvReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_AUDIO_OM_GET_NV_REQ,            AUDIO_ENHANCE_MsgOmGetNvReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_AUDIO_PCM_ENHANCE_START_IND,    AUDIO_ENHANCE_MsgStartInd),
    FSM_ACT_TBL_ITEM(0x0000, ID_AUDIO_PCM_ENHANCE_STOP_IND,     AUDIO_ENHANCE_MsgStopInd),
    FSM_ACT_TBL_ITEM(0x0000, ID_AUDIO_PCM_ENHANCE_SET_DEVICE_IND,   AUDIO_ENHANCE_MsgSetDeviceInd),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_MLIB_SET_PARA_IND,     AUDIO_ENHANCE_MsgSetParaInd),
    FSM_ACT_TBL_ITEM(0x0000, ID_AUDIO_PLAYER_START_DECODE_IND,  AUDIO_PLAYER_MsgStartDecodeInd),
    FSM_ACT_TBL_ITEM(0x0000, ID_AUDIO_PLAYER_STOP_DECODE_IND,   AUDIO_PLAYER_MsgStopDecodeInd),
};

/* AUDIOʵʱ��Ϣ�����,�˴�û������ */
ACT_STRU g_astAudioRtMsgFuncTable[] =
{
    /* ������ʽ: PID(reserve), ��Ϣ����, ��Ӧ������ */
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_PLAY_START_REQ,        AUDIO_PLAYER_MsgStartReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_PLAY_PAUSE_REQ,        AUDIO_PLAYER_MsgStopReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_PLAY_UPDATE_BUF_CMD,   AUDIO_PLAYER_MsgUpdateApBuffCmd),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_PLAY_WAKEUPTHREAD_REQ, AUDIO_PLAYER_MsgPlayDoneSignalReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_PLAY_SET_VOL_CMD,      AUDIO_PLAYER_MsgSetVolReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_PLAY_QUERY_TIME_REQ,   AUDIO_PLAYER_MsgQueryTimeReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_PLAY_QUERY_STATUS_REQ, AUDIO_PLAYER_MsgQueryStatusReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_PLAY_SEEK_REQ,         AUDIO_PLAYER_MsgSeekReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_LOOP_BEGIN_REQ,        AUDIO_DEBUG_MsgAudioLoopOpenReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_LOOP_END_REQ,          AUDIO_DEBUG_MsgAudioLoopCloseReq),

    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_RECORD_START_CMD,      AUDIO_RECORDER_MsgRecordStartCmd),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_RECORD_STOP_CMD,       AUDIO_RECORDER_MsgRecordStopCmd),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_PCM_OPEN_REQ,          AUDIO_PCM_MsgPcmOpenReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_PCM_CLOSE_REQ,         AUDIO_PCM_MsgPcmCloseReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_PCM_HW_PARA_REQ,       AUDIO_PCM_MsgPcmHwParaReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_PCM_HW_FREE_REQ,       AUDIO_PCM_MsgPcmHwFreeReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_PCM_PREPARE_REQ,       AUDIO_PCM_MsgPcmPrepareReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_PCM_TRIGGER_REQ,       AUDIO_PCM_MsgPcmTriggerReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_PCM_POINTER_REQ,       AUDIO_PCM_MsgPcmPointerReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_PCM_SET_BUF_CMD,       AUDIO_PCM_MsgPcmSetBufCmd),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_HIFI_TEST_CMD,               AUDIO_DEBUG_MsgTestCmd),

    /* DTS��Ч��Ϣ������ */
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_SET_DTS_ENABLE_CMD,    AUDIO_PLAYER_MsgSetDtsEnableCmd),
    FSM_ACT_TBL_ITEM(0x0000, ID_AUDIO_PLAYER_SET_DTS_ENABLE_IND, XA_DTS_MsgSetDtsEnableCmd),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_SET_DTS_DEV_CMD,       XA_DTS_MsgSetDtsDevCmd),

    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_ENHANCE_START_REQ,     AUDIO_ENHANCE_MsgStartReqRt),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_ENHANCE_STOP_REQ,      AUDIO_ENHANCE_MsgStopReqRt),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_ENHANCE_SET_DEVICE_REQ,AUDIO_ENHANCE_MsgSetDeviceReqRt),
    FSM_ACT_TBL_ITEM(0x0000, ID_AP_AUDIO_MLIB_SET_PARA_IND,     AUDIO_ENHANCE_MsgSetParaIndRt),
};


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/



VOS_VOID AUDIO_COMM_Init( VOS_VOID )
{
    VOS_UINT32 uwRet = VOS_OK;

    CODEC_NV_PARA_AUDIO_TRACE_CFG_STRU  stAudioTraceCfg;

    AUDIO_PLAYER_InitBuff();

    AUDIO_PCM_Init();

    /* ��ȡAudio TRACE���NV�� */
    uwRet = UCOM_NV_Read(en_NV_AudioTraceCfg,
                         &stAudioTraceCfg,
                         sizeof(stAudioTraceCfg));

    if (VOS_OK != uwRet)
    {
        /* NV ��ȡʧ�� */
        OM_LogWarning1(UCOM_NV_Read_ParaError,uwRet);
    }

    /* audio���Թ��ܳ�ʼ�� */
    AUDIO_DEBUG_Init(&stAudioTraceCfg);

    /* enhance��ʼ�� */
    AUDIO_ENHANCE_Init();

    /* ¼����ʼ�� */
    AUDIO_RECOEDER_Init();

    return;
}
VOS_UINT32 AUDIO_COMM_PidInit(enum VOS_INIT_PHASE_DEFINE enInitPhrase)
{
    switch (enInitPhrase)
    {
        case VOS_IP_LOAD_CONFIG:
        {
            /*��ʼ��PID_AUDIO״̬��*/
            AUDIO_COMM_FuncTableInit();

            /* audioģ���ʼ�� */
            AUDIO_COMM_Init();
            break;
        }
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
        {
            break;
        }
        default:
        {
            break;
        }
    }

    return VOS_OK;
}


VOS_VOID AUDIO_COMM_PidProc(MsgBlock *pstOsaMsg)
{
    VOS_UINT16     *puhwMsgId           = VOS_NULL;
    ACT_STRU       *pstRetAct           = VOS_NULL;
    STA_STRU       *pstMsgStatus        = AUDIO_COMM_GetMsgStatusPtr();
    ACT_STRU        stKeyAct;

    /* ��ȡ��ϢID */
    puhwMsgId = (VOS_UINT16*)pstOsaMsg->aucValue;

    stKeyAct.ulEventType = *puhwMsgId;

    /* ���ö��ֲ��Һ���bsearch���¼�������в�����Ӧ���¼������� */
    /* ����Ҳ�����Ӧ���¼�������,����NULL */
    pstRetAct = (ACT_STRU*)FSM_Bsearch((VOS_INT16 *)(&stKeyAct),
                                       (VOS_UINT8 *)(pstMsgStatus->pActTable),
                                       pstMsgStatus->ulSize,
                                       sizeof(ACT_STRU),
                                       FSM_ActCompare);

    /* ������Ϣ��������֮��Ӧ,����֮ */
    if (VOS_NULL != pstRetAct)
    {
        (VOS_VOID )(*pstRetAct->pfActionFun)(pstOsaMsg);
    }
    else
    {
        /* ��¼�쳣,DSP_PID_AUDIO�յ�δ֪��Ϣ,δ���� */
        OM_LogWarning1(AUDIO_COMM_PidProc_UnknownMsg, stKeyAct.ulEventType);
    }

    return;
}
VOS_UINT32 AUDIO_COMM_RtPidInit(enum VOS_INIT_PHASE_DEFINE enInitPhrase)
{
    switch (enInitPhrase)
    {
        case VOS_IP_LOAD_CONFIG:
        {
            /* ��ʼ��PID_MED_RT״̬�� */
            AUDIO_COMM_RtFuncTableInit();

            break;
        }
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
        {
            break;
        }
        default:
        {
            break;
        }
    }

    return VOS_OK;
}
VOS_VOID AUDIO_COMM_RtPidProc(MsgBlock *pstOsaMsg)
{
    VOS_UINT16     *puhwMsgId           = VOS_NULL;
    ACT_STRU       *pstRetAct           = VOS_NULL;
    STA_STRU       *pstMsgStatus        = AUDIO_COMM_GetRtMsgStatusPtr();
    ACT_STRU        stKeyAct;

    /* ��ȡ��ϢID */
    puhwMsgId = (VOS_UINT16*)pstOsaMsg->aucValue;

    stKeyAct.ulEventType = *puhwMsgId;

    /* ���ö��ֲ��Һ���bsearch���¼�������в�����Ӧ���¼������� */
    /* ����Ҳ�����Ӧ���¼�������,����NULL */
    pstRetAct = (ACT_STRU*)FSM_Bsearch((VOS_INT16 *)(&stKeyAct),
                                       (VOS_UINT8 *)(pstMsgStatus->pActTable),
                                       pstMsgStatus->ulSize,
                                       sizeof(ACT_STRU),
                                       FSM_ActCompare);

    /* ������Ϣ��������֮��Ӧ,����֮ */
    if (VOS_NULL != pstRetAct)
    {
        (VOS_VOID )(*pstRetAct->pfActionFun)(pstOsaMsg);
    }
    else
    {
        /* ��¼�쳣,DSP_PID_AUDIO_RT�յ�δ֪��Ϣ,δ���� */
        OM_LogWarning1(AUDIO_COMM_RtPidProc_UnknownMsg, stKeyAct.ulEventType);
    }

    return;
}
VOS_VOID AUDIO_COMM_FuncTableInit( VOS_VOID )
{
    STA_STRU       *pstStatusDec        = AUDIO_COMM_GetMsgStatusPtr();

    /* ��ʼ��״̬������������������*/
    pstStatusDec->pActTable  = AUDIO_COMM_GetMsgFuncTbl();
    pstStatusDec->ulSize     = sizeof(AUDIO_COMM_GetMsgFuncTbl())/sizeof(ACT_STRU);

    /* ����������ϢID�������򣬴ӵ͵���˳������ */
    FSM_Sort((VOS_UINT8 *)pstStatusDec->pActTable,
             pstStatusDec->ulSize,
             sizeof(ACT_STRU),
             FSM_ActCompare);

    return;
}


VOS_VOID AUDIO_COMM_RtFuncTableInit( VOS_VOID )
{
    STA_STRU       *pstStatusDec        = AUDIO_COMM_GetRtMsgStatusPtr();

    /* ��ʼ��״̬������������������*/
    pstStatusDec->pActTable  = AUDIO_COMM_GetRtMsgFuncTbl();
    pstStatusDec->ulSize     = sizeof(AUDIO_COMM_GetRtMsgFuncTbl())/sizeof(ACT_STRU);

    /* ����������ϢID�������򣬴ӵ͵���˳������ */
    FSM_Sort((VOS_UINT8 *)pstStatusDec->pActTable,
             pstStatusDec->ulSize,
             sizeof(ACT_STRU),
             FSM_ActCompare);

    return;
}


VOS_UINT32  AUDIO_COMM_IsIdle(VOS_VOID)
{
    /*��ʾ��ҵ�����У�ͶƱ:���µ�*/
    if ((AUDIO_PCM_STATUS_DEACTIVE == AUDIO_PcmGetState())
        && (AUDIO_PLAYER_STATE_IDLE == AUDIO_PlayerGetState()))
    {
        return VOS_OK;
    }
    else
    {
        return VOS_ERR;
    }
}
VOS_VOID  AUDIO_COMM_CheckContext( VOS_VOID )
{
    VOS_UINT32      uwRet;

    uwRet = AUDIO_PLAYER_CheckBuff();

    if(uwRet != 0)
    {
        OM_LogFatalAndReset(AUDIO_COMM_BuffOverflow);
    }

}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


