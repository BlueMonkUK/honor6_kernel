/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : om_log.h
  �� �� ��   : ����
  ��    ��   : ��ׯ�� 59026
  ��������   : 2011��5��31��
  ����޸�   :
  ��������   : om_log.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��5��31��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "HifiOmInterface.h"
#include "OmCodecInterface.h"
#include "omerrorlog.h"


#ifndef __OM_LOG_H__
#define __OM_LOG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/

#define OM_LOG_COUNTER_BASE             (0x7f00)                                /* �����ϱ���־����ʼ��� */
#define OM_LOG_COUNTER_MAX_NUM          (100)                                   /* ֧�������ϱ������־�� */
#define OM_LOG_INNER_MAX_NUM            (500)                                   /* �ڲ���־��¼���� */
#define OM_LOG_APR_MAX_NUM              (10)                                    /* APR���Լ�¼��������� */
#define OM_LOG_APR_MAX_LOG_CNT_NUM      (0x7fffffff)                            /* APR�ɶ�ͬһlog��¼�����ֵ */


/* ����Ϊȫ�ֱ������ʷ�װ */

#define OM_LOG_GetCtrlObjPtr()          (&g_stOmLogCtrl)
#define OM_LOG_GetEnalbe()              (g_stOmLogCtrl.enEnable)
#define OM_LOG_GetLevel()               (g_stOmLogCtrl.enLevel)
#define OM_LOG_SetEnable(uhwEnable)     (g_stOmLogCtrl.enEnable = uhwEnable)
#define OM_LOG_SetLevel(uhwLevel)       (g_stOmLogCtrl.enLevel = uhwLevel)
#define OM_LOG_GetCycleCnt(uhwIndex)    (g_stOmLogCtrl.auhwCycleCnt[uhwIndex])
#define OM_LOG_SetCycleCnt(uhwIdx, uhwCnt)                  \
                                        (g_stOmLogCtrl.auhwCycleCnt[uhwIdx] = uhwCnt)
#define OM_LOG_GetInnerObjPtr()         ((VOS_VOID *)UCOM_GetUncachedAddr((VOS_UINT32)&g_stOmLogInnerRec))
#define OM_LOG_GetInnerRecord(uwIndx)   (&(g_stOmLogInnerRec.astRecords[uwIndx]))
#define OM_LOG_GetInnerCnt()            (&(g_stOmLogInnerRec.stWaitReset))
#define OM_LOG_GetSoftIntrInfo()        (g_uwSoftIntInfo)
#define OM_LOG_GetLogMsgId(enLevel)     (g_auhwOmLogLevelToMsgIdTable[enLevel])
#define OM_LOG_SetInit()                (g_uwOmLogInitFlag = 0x12345678)
#define OM_LOG_CheckInit()              (0x12345678 == g_uwOmLogInitFlag)

#define OM_LOG_GetAprLogPtr()           (&g_stOmAprLog)
#define OM_LOG_GetLastErrRec()          (&g_stOmLogLastErrRec)


/* ����Ϊȫ�ֱ������ʷ�װ */

/* ��¼�������󲢸�λϵͳ */
#define OM_LogFatalAndReset(enLogId)    VOS_SoftIntTriger(enLogId)

/* ERROR��־��¼ */
#define OM_LogError(enLogId)                                \
    OM_LOG_LogReport(OM_LOG_LEVEL_ERROR,  THIS_FILE_ID,__LINE__,enLogId,1,0,0,0)
#define OM_LogError1(enLogId,swValue1)                      \
    OM_LOG_LogReport(OM_LOG_LEVEL_ERROR,  THIS_FILE_ID,__LINE__,enLogId,1,(VOS_INT32)(swValue1),0,0)
#define OM_LogError2(enLogId,swValue1,swValue2)             \
    OM_LOG_LogReport(OM_LOG_LEVEL_ERROR,  THIS_FILE_ID,__LINE__,enLogId,1,(VOS_INT32)(swValue1),(VOS_INT32)(swValue2),0)
#define OM_LogError3(enLogId,swValue1,swValue2,swValue3)    \
    OM_LOG_LogReport(OM_LOG_LEVEL_ERROR,  THIS_FILE_ID,__LINE__,enLogId,1,(VOS_INT32)(swValue1),(VOS_INT32)(swValue2),(VOS_INT32)(swValue3))

/* WARNING��־��¼ */
#define OM_LogWarning(enLogId)                              \
    OM_LOG_LogReport(OM_LOG_LEVEL_WARNING,THIS_FILE_ID,__LINE__,enLogId,1,0,0,0)
#define OM_LogWarning1(enLogId,swValue1)                    \
    OM_LOG_LogReport(OM_LOG_LEVEL_WARNING,THIS_FILE_ID,__LINE__,enLogId,1,(VOS_INT32)(swValue1),0,0)
#define OM_LogWarning2(enLogId,swValue1,swValue2)           \
    OM_LOG_LogReport(OM_LOG_LEVEL_WARNING,THIS_FILE_ID,__LINE__,enLogId,1,(VOS_INT32)(swValue1),(VOS_INT32)(swValue2),0)
#define OM_LogWarning3(enLogId,swValue1,swValue2,swValue3)  \
    OM_LOG_LogReport(OM_LOG_LEVEL_WARNING,THIS_FILE_ID,__LINE__,enLogId,1,(VOS_INT32)(swValue1),swValue2,swValue3)

/* INFO��־��¼ */
#define OM_LogInfo(enLogId)                                 \
    OM_LOG_LogReport(OM_LOG_LEVEL_INFO,   THIS_FILE_ID,__LINE__,enLogId,1,0,0,0)
#define OM_LogInfo1(enLogId,swValue1)                       \
    OM_LOG_LogReport(OM_LOG_LEVEL_INFO,   THIS_FILE_ID,__LINE__,enLogId,1,(VOS_INT32)(swValue1),0,0)
#define OM_LogInfo2(enLogId,swValue1,swValue2)              \
    OM_LOG_LogReport(OM_LOG_LEVEL_INFO,   THIS_FILE_ID,__LINE__,enLogId,1,(VOS_INT32)(swValue1),(VOS_INT32)(swValue2),0)
#define OM_LogInfo3(enLogId,swValue1,swValue2,swValue3)     \
    OM_LOG_LogReport(OM_LOG_LEVEL_INFO,   THIS_FILE_ID,__LINE__,enLogId,1,(VOS_INT32)(swValue1),(VOS_INT32)(swValue2),(VOS_INT32)(swValue3))
#define OM_LogFreqInfo(enLogId,uhwLogFreq)                  \
    OM_LOG_LogReport(OM_LOG_LEVEL_INFO,   THIS_FILE_ID,__LINE__,enLogId,uhwLogFreq,0,0,0)

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
 ö����    : OM_FILE_ID_ENUM
 ö��˵��  : C�ļ����ö�ٶ���, ȫϵͳ�ļ��ڴ�ͳһ���
*****************************************************************************/
enum OM_FILE_ID_ENUM
{
    OM_FILE_ID_OM_BEGIN                 = 0,
    OM_FILE_ID_OM_COMM_C,
    OM_FILE_ID_OM_LOG_C,
    OM_FILE_ID_OM_MSGHOOK_C,
    OM_FILE_ID_OM_CPUVIEW_C,
    OM_FILE_ID_OM_END,

    OM_FILE_ID_DRV_BEGIN                = 50,
    OM_FILE_ID_DRV_MAILBOX_C,
    OM_FILE_ID_DRV_END,

    OM_FILE_ID_UCOM_BEGIN               = 100,
    OM_FILE_ID_UCOM_COMM_C,
    OM_FILE_ID_UCOM_NV_C,
    OM_FILE_ID_UCOM_MEM_DYN,
    OM_FILE_ID_UCOM_PCM_C,
    OM_FILE_ID_UCOM_END,

    OM_FILE_ID_VOICE_BEGIN              = 200,
    OM_FILE_ID_VOICE_API_C,
    OM_FILE_ID_VOICE_MC_C,
    OM_FILE_ID_VOICE_PROC_C,
    OM_FILE_ID_VOICE_PCM_C,
    OM_FILE_ID_VOICE_LOG_C,
    OM_FILE_ID_VOICE_AMR_MODE_SC,
    OM_FILE_ID_VOICE_DEBUG_C,
    OM_FILE_ID_VOICE_SJB_C,
    OM_FILE_ID_VOICE_JB_INTERFACE_C,
    OM_FILE_ID_VOICE_END,
    OM_FILE_ID_VOICE_DIAGNOSE_C,
    OM_FILE_ID_BALONG_PP_BEGIN          = 260,
    OM_FILE_ID_MED_PP_MAIN_C,
    OM_FILE_ID_MODULE_BALONG_PP_C,
    OM_FILE_ID_BALONG_PP_END,

    OM_FILE_ID_AUDIO_BEGIN             = 300,
    OM_FILE_ID_AUDIO_COMM_C,
    OM_FILE_ID_AUDIO_PCM_C,
    OM_FILE_ID_AUDIO_PLAYER_C,
    OM_FILE_ID_AUDIO_RECORDER_C,
    OM_FILE_ID_AUDIO_DEBUG_C,
    OM_FILE_ID_AUDIO_VOIP_C,
    OM_FILE_ID_AUDIO_ENHANCE_C,
    OM_FILE_ID_AUDIO_END,

    OM_FILE_ID_CODEC_AUDIO_BEGIN       = 400,
    OM_FILE_ID_CODEC_XA_COMM_C,
    OM_FILE_ID_CODEC_MP3_DEC_C,
    OM_FILE_ID_CODEC_DM3_C,
    OM_FILE_ID_CODEC_XA_SRC_PP_C,
    OM_FILE_ID_CODEC_XA_DTS_PP_C,
    OM_FILE_ID_CODEC_XA_AAC_DEC_C,
    OM_FILE_ID_CODEC_AUDIO_END,

	OM_FILE_ID_VOICE_AJB_BEGIN         = 500,
    OM_FILE_ID_VOICE_AJB_JB_C,
    OM_FILE_ID_VOICE_AJB_PACKET_MANAGER_C,
    OM_FILE_ID_VOICE_AJB_PCM_PROCESS_C,
    OM_FILE_ID_VOICE_AJB_ALGO_C,
	OM_FILE_ID_VOICE_AJB_END,

    OM_FILE_ID_BUTT
};
typedef VOS_UINT16 OM_FILE_ID_ENUM_UINT16;

/*****************************************************************************
 ö����    : OM_LOG_ID_ENUM
 ö��˵��  : HIFI�������־���, ��������˷ֶ����ж���
*****************************************************************************/
enum OM_LOG_ID_ENUM
{
    OM_LOG_ID_UCOM_BEGIN                = 0x0000,
    UCOM_NV_Init_NvDataUnInit,
    UCOM_NV_Init_ApNvDataInitOK,
    UCOM_NV_Init_NvDataTooLong,
    UCOM_NV_MemCpy_ParaInvalid,
    UCOM_NV_FileSearch_ParaError,
    UCOM_NV_Read_ModemIDError,
    UCOM_NV_Read_ParaError,
    UCOM_NV_Read_NvNotExist,
    UCOM_NV_Read_LenTooLong,
    UCOM_COMM_SendMsg_NoMem,
    UCOM_COMM_SendMsg_Err,
    UCOM_DYN_MEM_Overflow,
    UCOM_DYN_MEM_AddTrace,
    UCOM_DYN_MEM_BadPara,
    UCOM_DYN_MEM_Memary_NotEnough,

    OM_LOG_ID_UCOM_END                  = 0x00ff,
    OM_LOG_ID_OM_BEGIN                  = 0x0100,
    OM_CPUVIEW_MsgCfgReq_StartOk,
    OM_CPUVIEW_MsgCfgReq_StopOk,
    OM_CPUVIEW_ReportBasicInfo_MacroDefErr,
    OM_CPUVIEW_StatTimerHandler_AllocMsgFail,
    OM_CPUVIEW_StatTimerHandler_SendMsgFail,
    OM_COMM_SendTrace_SocpWriteFailed,
    OM_COMM_SendTrans_SocpWriteFailed,
    OM_COMM_PidProc_UnknownMsg,
    OM_COMM_MsgExtDefReq_OK,
    OM_LOG_ID_OM_END                    = 0x02ff,

    OM_LOG_ID_MAILBOX_BEGIN             = 0x0300,
    OM_MAILBOX_SendMsg_MailboxFull,
    OM_MAILBOX_SendMsg_MailboxHeadPartitionErr,
    OM_MAILBOX_RecieveMsg_MailboxHeadPartitionErr,
    OM_MAILBOX_RecieveMsg_SendVosMsgErr,
    OM_MAILBOX_RecieveMsg_MsgPartitionErr,
    OM_MAILBOX_RecieveMsg_MsgSeqNumErr,
    OM_MAILBOX_RecieveMsg_MsgDelayErr,
    OM_MAILBOX_RecieveMsg_MsgChannelIdErr,
    OM_MAILBOX_RecieveMsg_MsgReadErr,
    OM_MAILBOX_SendMsg_AllocFail,
    OM_LOG_ID_MAILBOX_END               = 0x03ff,

    OM_LOG_ID_VOICE_BEGIN                 = 0x0400,

    /* API ý�������ӿڲ� */
    VOICE_ApiPidProc_FsmErrHandled,
    VOICE_ApiPidProc_FsmErrUnHandled,
    VOICE_ApiRtPidProc_FsmErrHandled,
    VOICE_ApiRtPidProc_FsmErrUnHandled,

    /* MC ý������ģ�� */
    VOICE_MsgStartReq_Ok,
    VOICE_MsgStartReq_JbInitFail,
    VOICE_MsgStopReq_OK,
    VOICE_MsgStopReq_Fail,
    VOICE_MsgQueryStatusReq_Ok,
    VOICE_McChkMicConflick_MicInDmaRest,
    VOICE_McStartPcmTrans_MicIn,
    VOICE_McStartPcmTrans_SpkOut,
    VOICE_MsgUpdateGsmParaCmd_Ok,
    VOICE_MsgGphyRxDataIndError,
    VOICE_MsgDoDec_ProcDecodeError,
    VOICE_McDoPp_ProcPreError,
    VOICE_McDoPp_ProcPostError,
    VOICE_McDoEnc_ProcEncodeError,
    VOICE_MsgSuspendCmd_OK,
    VOICE_MsgSuspendCmd_ErrState,
    VOICE_McDoResume_OK,
    VOICE_TimerTxIsr_WarningState,
    VOICE_TimerRxIsr_WarningState,
    VOICE_Timer20msIsr_ErrState,
    VOICE_McCheckStartMsg_InvalidNetMode,
    VOICE_McCheckStartMsg_InvalidCodecType,
    VOICE_McCheckStartMsg_UnmatchableWcdmaCodec,
    VOICE_McCheckStartMsg_UnmatchableTDCodec,
    VOICE_McCheckStartMsg_UnmatchableGsmCodec,
    VOICE_McCheckStartMsg_UnmatchableImsCodec,
    VOICE_MsgPlayInd_Done,
    VOICE_MsgRecordInd_Done,
    VOICE_MsgEncReq_ErrState,
    VOICE_MsgGphyRxDataInd_CodecTypeMismatch,
    VOICE_MsgGphyRxDataInd_ModemMismatch,
    VOICE_MsgUpdateGsmParaCmd_CodecTypeMismatch,
    VOICE_McDoEnc_CodecTypeMismatch,
    VOICE_MsgPlayInd_CodecTypeMismatch,
    VOICE_MsgRecordInd_CodecTypeMismatch,
    VOICE_MsgPhyBackGroundCnf_SUCC,
    VOICE_MsgPhyBackGroundCnf_FAIL,
    VOICE_MsgPhyForeGroundCnf_SUCC,
    VOICE_MsgPhyForeGroundCnf_FAIL,
    VOICE_MsgApNoteCCPUReset_SUCC,
    VOICE_MsgGphySyncInd_Done,
    VOICE_GetModemNumError,
    VOICE_MsgImsaRxDataInd_CodecTypeMismatch,
    VOICE_McTxReceivePid_UNSET,
	VOICE_McPostTxProc_NetModeMisMatch,
    VOICE_McTxDataToImsa_Done,
    VOICE_McImsaCfgDone,
    VOICE_McRxProcDecBuffOverFlow,
    VOICE_McSetCodecRegAjbCodec,
    /* PROC ǰ���� */
    VOICE_ProcInitCodec_AmrEncInit_Done,
    VOICE_ProcInitCodec_EfrEncInit_Done,
    VOICE_ProcInitCodec_FrEncInit_Done,
    VOICE_ProcInitCodec_HrEncInit_Done,
    VOICE_ProcInitCodec_AmrDecInit_Done,
    VOICE_ProcInitCodec_EfrDecInit_Done,
    VOICE_ProcInitCodec_FrDecInit_Done,
    VOICE_ProcInitCodec_HrDecInit_Done,
    VOICE_ProcInitCodec_AmrWbEncInit_Done,
    VOICE_ProcInitCodec_AmrWbDecInit_Done,
    VOICE_ProcInitCodec_UnkownEncoder,
    VOICE_ProcInitCodec_UnkownDecoder,
    VOICE_ProcInit_Ok,
    VOICE_ProcInit_ReadNvError,
    VOICE_ProcInit_ParamError,
    VOICE_CfgUmtsDlCodecError,
    VOICE_CfgUmtsUlCodecError,
    VOICE_ReadNvError,
    VOICE_SetNvError,
    VOICE_UpdateNvOk,
    VOICE_AmrModeCtrlInit_Ok,
    VOICE_AmrModeCtrlSetType_Ok,
    VOICE_MsgMacModeSetInd_BandWidthMismatch,
    VOICE_MsgMacModeSetInd_InvalidAcs,
    VOICE_MsgMacModeSetInd_InvalidMode,
    VOICE_MsgMacModeSetInd_InvalidSid,
    VOICE_MsgMacModeSetInd_Ok,
    VOICE_MsgDoChangeAmrModeInd_InvalidCodecType,
    VOICE_MsgDoChangeAmrModeInd_InvalidMode,
    VOICE_MsgDoChangeAmrModeInd_Ok,
    VOICE_AmrModeCtrlChangeMode_BandWidthMismatch,
    VOICE_AmrModeCtrlChangeMode_TargetOverflowAcsTop,
    VOICE_AmrModeCtrlChangeMode_TargetOverflowAcsBottom,
    VOICE_AmrModeCtrlChangeMode_AcsNotSet,
    VOICE_AmrModeCtrlChangeMode_InvalidTarget,
    VOICE_ProcSendFrameTypeError,
    VOICE_ProcCodecToSampleRateMapError,
    VOICE_ProcCodecPtrIsNULLError,
    VOICE_ProcGetEcRef_GetSmartPADataOk,
    VOICE_ProcGsmRxFacch,

    /* PCM ������ */
    VOICE_MicDmaIsr_NotRunning,
    VOICE_MicDmaIsr_ErrInt,
    VOICE_SpkDmaIsr_NotRunning,
    VOICE_SpkDmaIsr_ErrInt,
    VOICE_SioIsr_DownOverFlow,
    VOICE_SioIsr_UpOverFlow,
    VOICE_SioIsr_ErrInt,
    VOICE_PcmStartDMAToDataOut_RingBufUnInit,
    VOICE_PcmStartDMAToDataOut_ReadAddrErr,
    VOICE_PcmStartDMAToDataOut_DmaChanError,
    VOICE_PcmStartDMAFromDataIn_DmaChanError,
    VOICE_PcmStartDMAFromDataIn_RingBufUnInit,
    VOICE_PcmStartDMAFromDataIn_WriteAddrErr,
    VOICE_PcmStartDMAFromDataIn_InsertFrame,
    VOICE_PcmStartDMAFromDataIn_DelFrame,
    VOICE_PcmStartDMAToDataOut_RingBufFull,
    VOICE_PcmAdjustRingBufSize_BufSizeInc,
    VOICE_PcmAdjustRingBufSize_BufSizeDec,
    VOICE_PcmMicInStartLoopDMA_DmaChanError,
    VOICE_PcmSpkOutStartLoopDMA_DmaChanError,
    VOICE_PcmStartDMAFromDataIn_InvalidMagicNum,

    VOICE_EcRefDmaIsr_NotRunning,
    VOICE_EcRefDmaIsr_ErrInt,

    VOICE_PcmVcLoopMicIsr_IntTypeError,
    VOICE_PcmVcLoopSpkIsr_IntTypeError,

    VOICE_PcmVcLoopMic,
    VOICE_PcmVcLoopSpk,

    /* DEBUG ����ģ�� */
    VOICE_MsgSetLoopReq_Ok,
    VOICE_MsgSetHookReq_Ok,
    VOICE_MsgSetHookReq_Fail,
    VOICE_MsgSetCodecReq_StateError,
    VOICE_MsgSetCodecReq_Ok,
    VOICE_MsgSetDevReq_ParamError,
    VOICE_MsgSetDevReq_Ok,
    VOICE_MsgSetNvReq_ParamError,
    VOICE_MsgSetNvReq_Ok,
    VOICE_MsgSetNvReq_Fail,
    VOICE_MsgGetNvReq_Ok,
    VOICE_MsgGetNvReq_Get_Fail,
    VOICE_MsgGetNvReq_State_Err,
    VOICE_MsgSetNvReq_Creat_Fail,
    VOICE_MsgSetVolReq_ParamError,
    VOICE_MsgSetVolReq_Ok,
    VOICE_ProcDestory_Fail,

    VOICE_UtilSendMsg_NoMem,
    VOICE_UtilSendMsg_Err,

    /* ErrorLog */
    VOICE_ErrLogEventNumBeyondNvItems,
    VOICE_ErrLogInvalidNvItemValue,
    VOICE_ErrLogNvItemsBroken,
    VOICE_ErrLogNvDisableErrLog,
    VOICE_ErrLogReportEvent,

    VOICE_MsgGphyRxDataOverFlow,
    VOICE_ProcCfgDlCodec_UnkownMode,
    VOICE_ProcCfgDlCodecError,
    VOICE_ProcAddDecSerialWrite_RemoveExpiredDecSerial,
    VOICE_MsgWphyRxDataOverFlow,
    VOICE_MsgTDphyRxDataOverFlow,
    VOICE_McRxFrmLost,
    VOICE_McChkSpkConflick_SpkOutDmaRest,
    VOICE_McChkMicRightConflick_MicInDmaRest,
    VOICE_McChkMicLeftConflick_MicInDmaRest,
    VOICE_DbgSaveCodecObj_CannotWriteVOICE_PROC_CODEC_STRUtoDbgBuf,
    VOICE_DiagnoseGoodFrameNum_UnkownMode,
    VOICE_DiagnoseMsgReport_OneWayNoSound,
    VOICE_DiagnoseMsgReport_VoiceAlarm,
    VOICE_MsgGphyChannelQualityIndR_Overflow,
    VOICE_MsgGphyChannelQualityIndR_BadCell,
    VOICE_MsgWphyChannelQualityIndR_Overflow,
    VOICE_MsgWphyChannelQualityIndR_BadCell,
    VOICE_MsgWphyChannelQualityIndR_ServCellId_Err,

    OM_LOG_ID_VOICE_END                 = 0x1fff,

    OM_LOG_ID_AUDIO_BEGIN               = 0x2000,

    /* audio_comm */
    AUDIO_COMM_PidProc_UnknownMsg,
    AUDIO_COMM_RtPidProc_UnknownMsg,
    AUDIO_COMM_BuffOverflow,

    /* audio_pcm */
    AUDIO_PCM_PlayDmaIsr_ErrInt,
    AUDIO_PCM_CaptureDmaIsr_ErrInt,
    AUDIO_PCM_PlayBackBuffIsNotReady,
    AUDIO_PCM_RegisterCbkOK,

    /* audio_debug */
    AUDIO_MsgSetHookReq_Ok,
    AUDIO_MsgSetHookReq_Fail,
    AUDIO_MsgQueryStatusReq_Ok,

    /* audio player */
    AUDIO_PlayerXADecode_InitFail,
    AUDIO_PlayerReadFile_ParamError,
    AUDIO_PlayerUpdateApBuffCmd_StateError,
    AUDIO_PlayerFillBuff_NoBakDataLeft,
    AUDIO_PlayerFillBuff_NoApDataUpdate,
    AUDIO_PlayerDecoderSeekFailed,
    AUDIO_PlayerSeekDecBufCalcErr,
    AUDIO_PlayerSeekSucc,
    AUDIO_PlayerSeekFail,
    AUDIO_PlayerSeekWhenIdle,
    AUDIO_PlayerSeekTo0SetLoop,
    AUDIO_PlayerSeekToBegin,
    AUDIO_PlayerProtectWordFail,
    AUDIO_PlayerDecBuffSizeWarning,
    AUDIO_PlayerSrcBuffSizeWarning,
    AUDIO_PlayerShiftBuffError,
    AUDIO_PlayerProcOK,
    AUDIO_PlayerProcError,
    AUDIO_PlayerCpySwapBuffOK,
    AUDIO_PlayerCpySwapBuffFail,
    AUDIO_PlayerSetDtsEnableFail,

    /* audio recorder */
    AUDIO_RecorderBadPara,
    AUDIO_RECORDER_VoiceCall_ErrInt,
    AUDIO_RECORDER_BuffNullErr,
    AUDIO_RECORDER_MsgStartErr,
    AUDIO_RECORDER_RegisterVcCbOK,
    AUDIO_RECORDER_UnregisterVcCbOK,
    AUDIO_RECORDER_StopRecordWhileRefIs0,
    AUDIO_RECORDER_StartVcCallWhileIdle,
    AUDIO_RecorderSmpRateErr,
    AUDIO_RecorderSmpRateChange,
    AUDIO_RecorderInitSrcFailed,

    /* tensilica lib */
    CODEC_TENSILICA_INFO,
    CODEC_TENSILICA_ERR,
    CODEC_TENSILICA_ERR2,
    XA_SRC_Para_Error,
    XA_SRC_Init_Fail,
    XA_SRC_ExeProcess_Fail,

    /* DTS */
    XA_DTS_InvalidPara,
    XA_DTS_Init_OK,
    XA_DTS_Init_Fail,
    XA_DTS_SetParaWhileExe,
    XA_DTS_SetParaWhileExeFail,
    XA_DTS_SetEnableOK,
    XA_DTS_SetEnableFail,
    XA_DTS_SetEnableInvalid,
    XA_DTS_SetEableInvalidPara,
    XA_DTS_SetEnableVolOK,
    XA_DTS_SetEnableVolErr,
    XA_DTS_SetDevOK,
    XA_DTS_SetDevError,
    XA_DTS_SetDevInvalid,
    XA_DTS_SetDevInvalidPara,

    /* audio voip */
    AUDIO_VOIP_MallocFail,
    AUDIO_VOIP_SetPara_MicInFail,
    AUDIO_VOIP_SetPara_SpkOutFail,
    AUDIO_VOIP_ProcMicIn_InitErr,
    AUDIO_VOIP_ProcSpkOut_InitErr,
    AUDIO_VOIP_ProcMicIn_FuncDisable,
    AUDIO_VOIP_ProcSpkOut_FuncDisable,
    AUDIO_VOIP_ProcMicIn_SampleRate_Incorrect,
    AUDIO_VOIP_ProcSpkOut_SampleRate_Incorrect,

    /* audio enhance */
    AUDIO_ENHANCE_MallocFail,
    AUDIO_ENHANCE_SetPara_PlayFail,
    AUDIO_ENHANCE_SetPara_RecordFail,
    AUDIO_ENHANCE_ProcMicIn_InitErr,
    AUDIO_ENHANCE_ProcSpkOut_InitErr,
    AUDIO_ENHANCE_ProcMicIn_FuncDisable,
    AUDIO_ENHANCE_ProcSpkOut_FuncDisable,
    AUDIO_ENHANCE_ProcMicIn_SampleRate_Incorrect,
    AUDIO_ENHANCE_ProcSpkOut_SampleRate_Incorrect,
    AUDIO_ENHANCE_Init_SampleRate_UnSupport,
    AUDIO_ENHANCE_Init_ProcInitFailed,
    AUDIO_ENHANCE_Init_ProcSetParaFailed,
    AUDIO_ENHANCE_InitSucc,
    AUDIO_ENHANCE_InitFailed,
    AUDIO_ENHANCE_StopSucc,
    AUDIO_ENHANCE_StopRefCntInconsistent,
    AUDIO_ENHANCE_StopFailed,
    AUDIO_ENHANCE_MsgStartReq_InvalidModule,
    AUDIO_ENHANCE_MsgStartReq_Failed,
    AUDIO_ENHANCE_MsgStartReq_Succ,
    AUDIO_ENHANCE_MsgStopReq_InvalidModule,
    AUDIO_ENHANCE_MsgStopReq_Failed,
    AUDIO_ENHANCE_MsgStopReq_Succ,
    AUDIO_ENHANCE_MsgSetDeviceReq_InvalidDevice,
    AUDIO_ENHANCE_MsgSetDeviceReq_Failed,
    AUDIO_ENHANCE_MsgSetDeviceReq_Succ,
    AUDIO_ENHANCE_StartModuleError,
    AUDIO_ENHANCE_SetDeviceModuleError,
    AUDIO_ENHANCE_StopModuleError,
    AUDIO_ENHANCE_MsgOmSetNvReqModuleError,
    AUDIO_ENHANCE_InitSrcError,
    AUDIO_ENHANCE_InitSrcSucc,
    AUDIO_ENHANCE_InitSrcFlag,
    AUDIO_ENHANCE_CloseSrcError,
    AUDIO_ENHANCE_CloseSrcSucc,
    AUDIO_ENHANCE_CloseSrcFlag,
    AUDIO_ENHANCE_DoSrcError,
    AUDIO_ENHANCE_DoSrcError_UnInit,
    AUDIO_ENHANCE_Module_MicIn,
    AUDIO_ENHANCE_Module_SpkOut,
    OM_LOG_ID_AUDIO_END                 =0x3fff,

    OM_LOG_ID_MLIB_EXT,

    OM_LOG_ID_SMARTPA_BEGIN             = 0x4010,
    UCOM_PCM_SmartPaPcmInitOk,
    UCOM_PCM_SmartPaPcmInitFail,
    UCOM_PCM_SmartPaStartLoopDma_ErrStatus,
    UCOM_PCM_SmartPaStartLoopDma_ErrChanNum,
    UCOM_PCM_SmartPaStartLoopDma_InvalidFrameLen,

    OM_LOG_ID_SMARTPA_END               = 0x4020,


    OM_LOG_ID_VOICE_SJB_BEGIN           = 0x4030,
    VOICE_SJB_InitTxSucc,
    VOICE_SJB_SetPktTimeTxSucc,
    VOICE_SJB_SetPktTimeTxFail,
    VOICE_SJB_StorePacketTxFail,
    VOICE_SJB_InsertPktTxSucc,
    VOICE_SJB_GetPacketTxFail,
    VOICE_SJB_InitRxSucc,
    VOICE_SJB_StorePacketRx_TsTooOld,
    VOICE_SJB_StorePacketRx_InitJB,
    VOICE_SJB_StorePacketRx_NoSpace,
    VOICE_SJB_StorePacketRx_SamePkt,
    VOICE_SJB_InsertPktRxSucc,
    VOICE_SJB_GetPacketRx_UnInit,
    VOICE_SJB_GetPacketRx_NoPkt,
    VOICE_SJB_GetPacketRx_NoWantedPkt,
    VOICE_SJB_GetPacketRx_InitJB,
    OM_LOG_ID_VOICE_SJB_END             = 0x4050,

    OM_LOG_ID_VOICE_JB_INTERFACE_BEGIN  = 0x4060,
    VOICE_JB_GetSjbPkt_Fail,
    VOICE_JB_AjbDecode_Enter,
    VOICE_JB_AjbDecode_Exit,
    OM_LOG_ID_VOICE_JB_INTERFACE_END    = 0x4080,

    /* VOICE_AJB����ö��ȡֵ���ɸ��� */
    OM_LOG_ID_VOICE_AJB_BEGIN           = 0x5000,
    VOICE_AJB_GetPcmData_InvalidDelay,
    VOICE_AJB_AddPacket_GetFirstPacket,
    VOICE_AJB_AddPacket_SsrcChange,
    VOICE_AJB_AddPacket_RepeatePacket,
    VOICE_AJB_AddPacket_PktTooLate,
    VOICE_AJB_AddPacket_PktRemoveErr,
    VOICE_AJB_GetPcmData_Get3NullPkts,
    VOICE_AJB_GetPcmData_Get20NullPkts,
    VOICE_AJB_GetPcmData_PktLoss,
    VOICE_AJB_GetPcmData_DecErr,
    VOICE_AJB_CreateGraph_JbPlcErr,
    VOICE_AJB_GetPcmData_DecPlcErr,
    VOICE_AJB_GetPcmData_TsmErr,
    VOICE_AJB_GetPcmData_ReInit,
    VOICE_AJB_RegDecode_InitForCodecChange,
    VOICE_AJB_GetPcmData_InitForBeyond,
    VOICE_AJB_GetPcmData_InitOk,
    VOICE_AJB_CreateGraph_Print,
    VOICE_AJB_GetPcmData_PktLossIn5S,
    VOICE_AJB_GetPcmData_GetPcmIn,
    VOICE_AJB_AddPacket_AddPktIn,
    VOICE_AJB_GetPcmData_GetPcmExit,
    VOICE_AJB_GetPcmData_TsmInfo,
    VOICE_AJB_CreateGraph_BuffDelay,
    VOICE_AJB_AddPacket_AddPktExit,
    VOICE_AJB_AddPacket_AddPktTooLarge,
    OM_LOG_ID_VOICE_AJB_END             = 0x5050,

    OM_LOG_ID_CNTER_BEGIN               = OM_LOG_COUNTER_BASE,
    OM_LOG_ID_CNTER_VOICE_BEGIN         = OM_LOG_COUNTER_BASE + 1,
    VOICE_McDoDec_Done,
    VOICE_McDoPp_Done,
    VOICE_McDoEnc_Done,
    VOICE_MsgWphySyncInd_Done,
    VOICE_ProcOutputFrameType_Done,
    VOICE_ProcVolume_InvalidVolume,
    OM_LOG_ID_CNTER_VOICE_END           = OM_LOG_COUNTER_BASE + 20,

    OM_LOG_ID_CNTER_END                 = OM_LOG_COUNTER_BASE + OM_LOG_COUNTER_MAX_NUM,
    OM_LOG_ID_BUTT
} ;
typedef VOS_UINT32 OM_LOG_ID_ENUM_UINT32;

/*****************************************************************************
 ʵ������  : OM_APR_LOG_ID_ENUM
 ��������  : APR�ϱ�ϵͳ��־���ö�ٶ��壬Hifi����ģ���ڴ�ͳһ���
*****************************************************************************/
enum
{
    /* voice */

    /* audio, Warning MAX Number is 9 (OM_LOG_APR_MAX_NUM) */
    APR_LOG_AUDIO_Illegal_Access_Memary     = 0,                                /* ���ڴ� */
    APR_LOG_AUDIO_Memary_NotEnough          = 1,                                /* �ڴ�ʹ���ʳ���90% */
    APR_LOG_AUDIO_Decoder_Fatal_error       = 2,                                /* �������������� */
    APR_LOG_AUDIO_Src_Fatal_error           = 3,                                /* ������������� */
    APR_LOG_AUDIO_Dts_Fatal_error           = 4,                                /* dts��Ч�������� */
    APR_LOG_AUDIO_Request_New_data_Fail     = 5,                                /* ����������ʧ�� */
    APR_LOG_AUDIO_Dma_Int_Abnormal          = 6,                                /* DMA�ж��쳣 */
    APR_LOG_AUDIO_Set_Buff_Abnormal         = 7,                                /* ap��hifi���������쳣 */

    /* balong_pp */

    /* ext_pp */

};
typedef VOS_UINT32 OM_APR_LOG_ID_ENUM_UINT32;

/*****************************************************************************
 ʵ������  : OM_APR_MODULE_ID_ENUM
 ��������  : APR�ϱ�ϵͳģ����ö�ٶ��壬Hifi����ģ���ڴ�ͳһ���
*****************************************************************************/
enum
{
    OM_APR_MODULE_VOICE             = 0,
    OM_APR_MODULE_AUDIO             = 1,
    OM_APR_MODULE_BALONG_PP         = 2,
    OM_APR_MODULE_EXT_PP            = 3,
    OM_APR_MODULE_BUTT
};
typedef VOS_UINT32 OM_APR_MODULE_ID_ENUM_UINT32;

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/

/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
/*****************************************************************************
 ʵ������  : OM_LOG_CTRL_STRU
 ��������  : ��ά�ɲ�ģ����־���ƽṹ��
 *****************************************************************************/
typedef struct
{
    OM_SWITCH_ENUM_UINT16               enEnable;                               /* ��־ʹ�� */
    OM_LOG_RPT_LEVEL_ENUM_UINT16        enLevel;                                /* ��־����,0:Info, 1:WARNING,2:ERROR */
    VOS_UINT16                          auhwCycleCnt[OM_LOG_COUNTER_MAX_NUM];   /*��������־��ӡ����*/

}OM_LOG_CTRL_STRU;

/*****************************************************************************
 ʵ������  : OM_LOG_INNER_RECORD_STRU
 ��������  : �ڲ�������¼
 *****************************************************************************/
typedef struct
{
    OM_LOG_ID_ENUM_UINT32               enLogId;                                /* ��־��� */
    VOS_UINT32                          uwTimeStamp;                            /* ʱ��     */
    VOS_UINT16                          uhwFileID;                              /* �ļ��� */
    VOS_UINT16                          uhwLineID;                              /* �к� */
}OM_LOG_INNER_RECORD_STRU;

/*****************************************************************************
 ʵ������  : OM_LOG_INNER_RESET_STRU
 ��������  : �ȴ���λʱ�ڲ�������
 *****************************************************************************/
typedef struct
{
    VOS_UINT32                          uwCnt;                                  /* �ȴ�ѭ��ʱ������ */
    VOS_UINT32                          uwTimeStamp;                            /* �����¼���ʱ�� */
}OM_LOG_INNER_RESET_STRU;

/*****************************************************************************
 ʵ������  : OM_LOG_INNER_OBJ_STRU
 ��������  : �ڲ���־��¼
 *****************************************************************************/
typedef struct
{
    VOS_UINT32                          uwCurrIdx;                              /* �ڲ���־��ǰ */
    OM_LOG_INNER_RECORD_STRU            astRecords[OM_LOG_INNER_MAX_NUM];       /* �ڲ���־��¼ */
    OM_LOG_INNER_RESET_STRU             stWaitReset;                            /* �ȴ���λ��Ϣ */
}OM_LOG_INNER_OBJ_STRU;

/*****************************************************************************
 ʵ������  : OM_LOG_APR_OBJ_STRU
 ��������  : APR��־��¼
 *****************************************************************************/
typedef struct
{
    VOS_UINT32                          auwAprCnt[OM_APR_MODULE_BUTT][OM_LOG_APR_MAX_NUM];
}OM_LOG_APR_OBJ_STRU;

/*****************************************************************************
 ʵ������  : OM_LOG_ITEM_STRU
 ��������  : ��־��¼����
 *****************************************************************************/
typedef struct
{
    OM_LOG_ID_ENUM_UINT32               enLogId;                                /* ��־��� */
    VOS_UINT32                          uwTimeStamp;                            /* ʱ��     */
    VOS_UINT16                          uhwFileID;                              /* �ļ��� */
    VOS_UINT16                          uhwLineID;                              /* �к� */
    VOS_INT32                           swValue1;
    VOS_INT32                           swValue2;
    VOS_INT32                           swValue3;
}OM_LOG_ITEM_STRU;


/*****************************************************************************
 ʵ������  : OM_ERR_LOG_DIAG_STRU
 ��������  : �������ģ���ϱ��Ĺ��ϻ�澯���ݽṹ
 *****************************************************************************/
typedef struct
{
    OM_ERR_LOG_HEADER_STRU              stOmErrLogHeader;
    VOS_UINT16                          auhwContent[4];                         /* ���ϻ�澯���� */
}OM_ERR_LOG_DIAG_STRU;

/*****************************************************************************
 ʵ������  : HIFI_ERROR_EVENT_ONE_WAY_NO_SOUND_STRU
 ��������  : ������ͨʱ��Ӧ��HIFI���Ͻṹ��
 *****************************************************************************/
typedef struct
{
    VOS_UINT16              uhwDevMode;                                         /*�����豸����*/
    VOS_UINT16              uhwActiveModem;                                     /*��ǰ�����Modem��*/
    VOS_UINT16              uhwNetMode;                                         /*������ʽ*/
    VOS_UINT16              uhwIsBadCell;                                       /*��ǰС���ź��Ƿ�ܲ� */
    VOS_UINT16              uhwCodecType;                                       /*�����������*/
    VOS_UINT16              uhwEncoderInited;                                   /*�����ʼ����־*/
    VOS_UINT16              uhwDecoderInited;                                   /*�����ʼ����־*/
    VOS_UINT16              uhwCheckPoint;                                      /*������ͨ��λ��*/
    VOS_UINT32              uwData[16];                                         /*������Ϣ*/
} HIFI_ERROR_EVENT_ONE_WAY_NO_SOUND_STRU;

/*****************************************************************************
 ʵ������  : HIFI_ERR_EVT_VOICE_CONTROL_FAULT_STRU
 ��������  : HIFI�ڲ����������쳣�澯�ṹ��
 *****************************************************************************/
typedef struct
{
    VOS_UINT16              uhwErrorReason;                                     /*�����쳣ԭ��*/
    VOS_UINT16              uhwDevMode;                                         /*�����豸����*/
    VOS_UINT16              uhwActiveModem;                                     /*��ǰ�����Modem��*/
    VOS_UINT16              uhwNetMode;                                         /*������ʽ*/
    VOS_UINT16              uhwCodecType;                                       /*�����������*/
    VOS_UINT16              uhwTxDmaErr;                                        /*����DMA�쳣��־λ*/
    VOS_UINT16              uhwRxDmaErr;                                        /*����DMA�쳣��־λ*/
    VOS_UINT16              uhwReaerve[5];                                      /*������Ϣ*/
}HIFI_ERR_EVT_VOICE_CONTROL_FAULT_STRU;

/*****************************************************************************
 ʵ������  : HIFI_ERR_EVT_VOICE_SUSPEND_SLOW_STRU
 ��������  : CS������ϵͳ�л�ʱ������澯�ṹ��
 *****************************************************************************/
typedef struct
{
    VOS_UINT16              uhwErrorReason;                                     /*�����쳣ԭ��*/
    VOS_UINT16              uhwActiveModem;                                     /*��ǰ�����Modem��*/
    VOS_UINT16              uhwCurNetMode;                                      /*��ǰ������ʽ*/
    VOS_UINT16              uhwPreNetMode;                                      /*�л�ǰ������ʽ*/
    VOS_UINT32              uwSuspendTime;                                     /*�����л��ڼ������������ͣʱ��*/
    VOS_UINT16              uhwReserve[4];
}HIFI_ERR_EVT_VOICE_SUSPEND_SLOW_STRU;

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern OM_LOG_APR_OBJ_STRU     g_stOmAprLog;

/*****************************************************************************
  10 ��������
*****************************************************************************/

extern VOS_UINT32 OM_LOG_DefaultCfg(CODEC_LOG_HOOK_CONFIG_STRU *pstLogHookCfg);
extern VOS_VOID OM_LOG_Init(VOS_VOID);
extern VOS_VOID OM_LOG_InnerRecord(
                OM_LOG_ID_ENUM_UINT32   enLogId,
                VOS_UINT16              uhwFileID,
                VOS_UINT16              uhwLineID);
extern VOS_VOID OM_LOG_IsrSoftIntr(VOS_VOID);
extern VOS_UINT32 OM_LOG_LogReport(
                       OM_LOG_RPT_LEVEL_ENUM_UINT16        enLevel,
                       OM_FILE_ID_ENUM_UINT16              enFileNo,
                       VOS_UINT16                          uhwLineNo,
                       OM_LOG_ID_ENUM_UINT32               enLogId,
                       VOS_UINT16                          uhwLogFreq,
                       VOS_INT32                           swValue1,
                       VOS_INT32                           swValue2,
                       VOS_INT32                           swValue3);
extern VOS_UINT32 OM_LOG_MsgSetLogReq(VOS_VOID *pvOsaMsg);
extern VOS_VOID  OM_LOG_AprLogReport(
                OM_APR_MODULE_ID_ENUM_UINT32        enModuleId,
                VOS_UINT32                          uwLogId);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of om_log.h */
