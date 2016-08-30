

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "audio.h"
#include "audio_pcm.h"
#include "audio_enhance.h"
#include "audio_debug.h"
#include "om_log.h"
#include "ucom_mem_dyn.h"
#include "mlib_interface.h"
#include "ucom_nv.h"
#include "om_comm.h"
#include "om_cpuview.h"

#include "ucom_pcm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*lint -e(767)*/
#define THIS_FILE_ID                    OM_FILE_ID_AUDIO_ENHANCE_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/* ENHANCE���ƽṹ�� */
AUDIO_ENHANCE_CTRL_STRU                 g_stAudioEnhanceCtrl;

/* ENHANCE����ṹ�� */
AUDIO_ENHANCE_BUFF_STRU                 g_stAudioEnhanceBuff;

/* ENHANCEģ��ע��� */
AUDIO_ENHANCE_MODULE_REG_STRU           g_astAudioEnhanceModuleTab[AUDIO_ENHANCE_MODULE_BUTT] = \
{
    /* Default ģ��ע�� */
    { MLIB_PATH_AUDIO_RECORD,       MLIB_PATH_AUDIO_PLAY },
    { MLIB_PATH_VOIP_CALL_MICIN,    MLIB_PATH_VOIP_CALL_SPKOUT},
    { MLIB_PATH_SIRI_MICIN,         MLIB_PATH_SIRI_SPKOUT},
};

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/



VOS_UINT32  AUDIO_ENHANCE_MsgStartReqRt( VOS_VOID *pvOsaMsg )
{
    AP_AUDIO_ENHANCE_START_REQ_STRU    *pstStartReq;
    AUDIO_PCM_ENHANCE_START_IND_STRU    stStartInd;
    VOS_VOID                               *pvMsgTail      = VOS_NULL;

    pstStartReq = (AP_AUDIO_ENHANCE_START_REQ_STRU*)((MsgBlock *) pvOsaMsg)->aucValue;

    /* ����Ϣ��Ҫ��ԭ��Ϣ��CmdId��Sn�Żظ���Hifi_misc���� */
    pvMsgTail       = (VOS_VOID *)((VOS_UINT32)(((MsgBlock *) pvOsaMsg)->aucValue) + sizeof(AP_AUDIO_ENHANCE_START_REQ_STRU));

    UCOM_MemSet(&stStartInd, 0, sizeof(AUDIO_PCM_ENHANCE_START_IND_STRU));

    stStartInd.uhwMsgId     = ID_AUDIO_PCM_ENHANCE_START_IND;
    stStartInd.uwSampleRate = pstStartReq->uwSampleRate;
    stStartInd.enModule     = pstStartReq->enModule;

    /* ����ͨ��VOS���ͽӿڣ�������Ϣ */
    UCOM_SendOsaMsg(DSP_PID_AUDIO_RT,
                    DSP_PID_AUDIO,
                   &stStartInd,
                    sizeof(AUDIO_PCM_ENHANCE_START_IND_STRU));

    /* �ظ�APȷ����Ϣ */
    AUDIO_ENHANCE_MsgSyncConfirm(ID_AUDIO_AP_ENHANCE_START_CNF,
                                 UCOM_RET_SUCC,
                                 pvMsgTail,
                                 sizeof(AUDIO_HIFIMISC_MSG_TAIL_STRU));

    return UCOM_RET_SUCC;
}
VOS_UINT32  AUDIO_ENHANCE_MsgStopReqRt( VOS_VOID *pvOsaMsg )
{
    AP_AUDIO_ENHANCE_STOP_REQ_STRU     *pstStopReq;
    AUDIO_PCM_ENHANCE_STOP_IND_STRU     stStopInd;
    VOS_VOID                               *pvMsgTail      = VOS_NULL;

    pstStopReq = (AP_AUDIO_ENHANCE_STOP_REQ_STRU*)((MsgBlock *) pvOsaMsg)->aucValue;

    /* ����Ϣ��Ҫ��ԭ��Ϣ��CmdId��Sn�Żظ���Hifi_misc���� */
    pvMsgTail       = (VOS_VOID *)((VOS_UINT32)(((MsgBlock *) pvOsaMsg)->aucValue) + sizeof(AP_AUDIO_ENHANCE_STOP_REQ_STRU));

    UCOM_MemSet(&stStopInd, 0, sizeof(AUDIO_PCM_ENHANCE_STOP_IND_STRU));

    stStopInd.uhwMsgId     = ID_AUDIO_PCM_ENHANCE_STOP_IND;
    stStopInd.enModule     = pstStopReq->enModule;

    /* ����ͨ��VOS���ͽӿڣ�������Ϣ */
    UCOM_SendOsaMsg(DSP_PID_AUDIO_RT,
                    DSP_PID_AUDIO,
                   &stStopInd,
                    sizeof(AUDIO_PCM_ENHANCE_STOP_IND_STRU));

    /* �ظ�APȷ����Ϣ */
    AUDIO_ENHANCE_MsgSyncConfirm(ID_AUDIO_AP_ENHANCE_STOP_CNF,
                                 UCOM_RET_SUCC,
                                 pvMsgTail,
                                 sizeof(AUDIO_HIFIMISC_MSG_TAIL_STRU));

    return UCOM_RET_SUCC;
}
VOS_UINT32  AUDIO_ENHANCE_MsgSetDeviceReqRt( VOS_VOID *pvOsaMsg )
{
    AP_AUDIO_ENHANCE_SET_DEVICE_REQ_STRU   *pstSetReq;
    AUDIO_PCM_ENHANCE_SET_DEVICE_IND_STRU   stSetInd;
    VOS_VOID                               *pvMsgTail      = VOS_NULL;

    pstSetReq = (AP_AUDIO_ENHANCE_SET_DEVICE_REQ_STRU*)((MsgBlock *) pvOsaMsg)->aucValue;

    /* ����Ϣ��Ҫ��ԭ��Ϣ��CmdId��Sn�Żظ���Hifi_misc���� */
    pvMsgTail       = (VOS_VOID *)((VOS_UINT32)(((MsgBlock *) pvOsaMsg)->aucValue) + sizeof(AP_AUDIO_ENHANCE_SET_DEVICE_REQ_STRU));

    UCOM_MemSet(&stSetInd, 0, sizeof(AUDIO_PCM_ENHANCE_SET_DEVICE_IND_STRU));

    stSetInd.uhwMsgId   = ID_AUDIO_PCM_ENHANCE_SET_DEVICE_IND;
    stSetInd.uwDevice   = pstSetReq->uwDevice;

    /* ����ͨ��VOS���ͽӿڣ�������Ϣ */
    UCOM_SendOsaMsg(DSP_PID_AUDIO_RT,
                    DSP_PID_AUDIO,
                   &stSetInd,
                    sizeof(AUDIO_PCM_ENHANCE_SET_DEVICE_IND_STRU));

    /* �ظ�APȷ����Ϣ */
    AUDIO_ENHANCE_MsgSyncConfirm(ID_AUDIO_AP_ENHANCE_SET_DEVICE_CNF,
                                 UCOM_RET_SUCC,
                                 pvMsgTail,
                                 sizeof(AUDIO_HIFIMISC_MSG_TAIL_STRU));

    return UCOM_RET_SUCC;
}
VOS_UINT32 AUDIO_ENHANCE_MsgSetParaIndRt(VOS_VOID *pstOsaMsg)
{
    /* ���е����ȼ���Ϣת�� */
    UCOM_SendOsaMsg(
                DSP_PID_AUDIO_RT,
                DSP_PID_AUDIO,
                pstOsaMsg,
                (((MsgBlock *) pstOsaMsg)->uwLength + VOS_MSG_HEAD_LENGTH));

    return UCOM_RET_SUCC;
}
VOS_UINT32  AUDIO_ENHANCE_MsgStartInd( VOS_VOID *pvOsaMsg )
{
    VOS_UINT32                          uwRet;
    AUDIO_PCM_ENHANCE_START_IND_STRU   *pstStartInd;

    pstStartInd = (AUDIO_PCM_ENHANCE_START_IND_STRU*)pvOsaMsg;

    if(pstStartInd->enModule >= AUDIO_ENHANCE_MODULE_BUTT)
    {
        OM_LogError1(AUDIO_ENHANCE_MsgStartReq_InvalidModule, pstStartInd->enModule);

        return UCOM_RET_FAIL;
    }

    uwRet = AUDIO_ENHANCE_Start(pstStartInd->enModule, pstStartInd->uwSampleRate);

    if(uwRet != UCOM_RET_SUCC)
    {
        OM_LogError2(AUDIO_ENHANCE_MsgStartReq_Failed, pstStartInd->enModule, uwRet);
    }
    else
    {
        OM_LogInfo1(AUDIO_ENHANCE_MsgStartReq_Succ, pstStartInd->enModule);
    }

    return UCOM_RET_SUCC;
}


VOS_UINT32  AUDIO_ENHANCE_MsgStopInd( VOS_VOID *pvOsaMsg )
{
    VOS_UINT32                          uwRet;
    AUDIO_PCM_ENHANCE_STOP_IND_STRU    *pstStopInd;

    pstStopInd = (AUDIO_PCM_ENHANCE_STOP_IND_STRU*)pvOsaMsg;

    if(pstStopInd->enModule >= AUDIO_ENHANCE_MODULE_BUTT)
    {
        OM_LogError1(AUDIO_ENHANCE_MsgStopReq_InvalidModule, pstStopInd->enModule);

        return UCOM_RET_FAIL;
    }

    uwRet = AUDIO_ENHANCE_Stop(pstStopInd->enModule);

    if(uwRet != UCOM_RET_SUCC)
    {
        OM_LogError2(AUDIO_ENHANCE_MsgStopReq_Failed, pstStopInd->enModule, uwRet);
    }
    else
    {
        OM_LogInfo1(AUDIO_ENHANCE_MsgStopReq_Succ, pstStopInd->enModule);
    }

    return UCOM_RET_SUCC;
}


VOS_UINT32  AUDIO_ENHANCE_MsgSetDeviceInd( VOS_VOID *pvOsaMsg )
{
    VOS_UINT32                              uwRet;
    AUDIO_PCM_ENHANCE_SET_DEVICE_IND_STRU  *pstSetDeviceInd;

    pstSetDeviceInd = (AUDIO_PCM_ENHANCE_SET_DEVICE_IND_STRU*)pvOsaMsg;

    if(pstSetDeviceInd->uwDevice >= AUDIO_ENHANCE_DEVICE_BUTT)
    {
        OM_LogError1(AUDIO_ENHANCE_MsgSetDeviceReq_InvalidDevice, pstSetDeviceInd->uwDevice);

        return UCOM_RET_FAIL;
    }

    uwRet = AUDIO_ENHANCE_SetDevice(pstSetDeviceInd->uwDevice);

    if(uwRet != UCOM_RET_SUCC)
    {
        OM_LogError1(AUDIO_ENHANCE_MsgSetDeviceReq_Failed, uwRet);
    }
    else
    {
        OM_LogInfo(AUDIO_ENHANCE_MsgSetDeviceReq_Succ);
    }

    return UCOM_RET_SUCC;
 }


VOS_UINT32 AUDIO_ENHANCE_MsgSetParaInd(VOS_VOID *pstOsaMsg)
{
    VOS_UINT32                          uwRet;
    AP_AUDIO_MLIB_SET_PARA_IND_STRU    *pstApPara;
    MLIB_PARA_STRU                     *pstMlibPara;
    VOS_UINT8                          *pucBuf;
    VOS_UINT32                          uwBufSize;
    AUDIO_ENHANCE_CTRL_STRU            *pstEnchanceCtrl;
    AUDIO_ENHANCE_MODULE_CTRL_STRU     *pstModuleCtrl;

    /* ��ȡ���ƽṹ�� */
    pstEnchanceCtrl = AUDIO_ENHANCE_GetCtrl();

    /* ���ģ��Ϸ��� */
    if(pstEnchanceCtrl->enCurrModule >= AUDIO_ENHANCE_MODULE_BUTT)
    {
        OM_LogError1(AUDIO_ENHANCE_MsgOmSetNvReqModuleError, pstEnchanceCtrl->enCurrModule);

        return UCOM_RET_FAIL;
    }

    pstModuleCtrl   = AUDIO_ENHANCE_GetModuleCtrl(pstEnchanceCtrl->enCurrModule);

    pstApPara = (AP_AUDIO_MLIB_SET_PARA_IND_STRU*)((MsgBlock *) pstOsaMsg)->aucValue;

    /* �������ݱ��ĵĿռ� */
    uwBufSize = sizeof(MLIB_PARA_STRU) + pstApPara->uwSize;
    pucBuf = (VOS_UINT8*)UCOM_MemAlloc(uwBufSize);
    UCOM_MemSet(pucBuf, 0, uwBufSize);
    pstMlibPara = (MLIB_PARA_STRU*)pucBuf;

    /* �������ݱ���ͷ */
    pstMlibPara->uwChnNum     = 2;  /* Ĭ��˫���� */
    pstMlibPara->uwSampleRate = pstModuleCtrl->uwSampleRate;
    pstMlibPara->uwFrameLen   = ((pstModuleCtrl->uwSampleRate / 8000) * 160);
    pstMlibPara->uwResolution = 16;
    pstMlibPara->enDevice     = pstEnchanceCtrl->enDevice;
    pstMlibPara->enParaSource = MLIB_PARA_SOURCE_APP;
    pstMlibPara->uwDataSize   = pstApPara->uwSize;

    /* �������ݱ����� */
    UCOM_MemCpy(pstMlibPara->aucData, pstApPara->aucData, pstApPara->uwSize);

    /* �����ݱ��ķ�������Ӧģ�� */
    uwRet = MLIB_PathModuleSet(
                pstApPara->uwPathID,
                pstApPara->uwModuleID,
                pstMlibPara,
                uwBufSize);

    /* �ͷű����ڴ� */
    UCOM_MemFree(pucBuf);

    return uwRet;
}



VOS_UINT32 AUDIO_ENHANCE_MsgOmSetNvReq(VOS_VOID *pstOsaMsg)
{
    VOS_UINT32                          uwRet;
    MSG_CODEC_CNF_STRU                  stRspPrim;
    MSG_OM_MLIB_PARA_SET_STRU          *pstOmPara;
    MLIB_PARA_STRU                     *pstMlibPara;
    VOS_UINT8                          *pucBuf;
    VOS_UINT32                          uwBufSize;
    AUDIO_ENHANCE_CTRL_STRU            *pstEnchanceCtrl;
    AUDIO_ENHANCE_MODULE_CTRL_STRU     *pstModuleCtrl;

    /* ��ȡ���ƽṹ�� */
    pstEnchanceCtrl = AUDIO_ENHANCE_GetCtrl();

    /* ���ģ��Ϸ��� */
    if(pstEnchanceCtrl->enCurrModule >= AUDIO_ENHANCE_MODULE_BUTT)
    {
        OM_LogError1(AUDIO_ENHANCE_MsgOmSetNvReqModuleError, pstEnchanceCtrl->enCurrModule);

        return UCOM_RET_FAIL;
    }

    pstModuleCtrl   = AUDIO_ENHANCE_GetModuleCtrl(pstEnchanceCtrl->enCurrModule);

    pstOmPara = (MSG_OM_MLIB_PARA_SET_STRU*)pstOsaMsg;

    /* �������ݱ��ĵĿռ� */
    uwBufSize = sizeof(MLIB_PARA_STRU) + pstOmPara->uwSize;
    pucBuf = (VOS_UINT8*)UCOM_MemAlloc(uwBufSize);
    UCOM_MemSet(pucBuf, 0, uwBufSize);
    pstMlibPara = (MLIB_PARA_STRU*)pucBuf;

    /* �������ݱ���ͷ */
    pstMlibPara->uwChnNum     = 2;  /* Ĭ��˫���� */
    pstMlibPara->uwSampleRate = pstModuleCtrl->uwSampleRate;
    pstMlibPara->uwFrameLen   = ((pstModuleCtrl->uwSampleRate / 8000) * 160);
    pstMlibPara->uwResolution = 16;
    pstMlibPara->enDevice     = pstEnchanceCtrl->enDevice;
    pstMlibPara->enParaSource = MLIB_PARA_SOURCE_TUNER;
    pstMlibPara->uwDataSize   = pstOmPara->uwSize;

    /* �������ݱ����� */
    UCOM_MemCpy(pstMlibPara->aucData, pstOmPara->aucData, pstOmPara->uwSize);

    /* �����ݱ��ķ�������Ӧģ�� */
    uwRet = MLIB_PathModuleSet(
                pstOmPara->uwPathID,
                pstOmPara->uwModuleID,
                pstMlibPara,
                uwBufSize);

    /* ���ظ���ϢID */
    stRspPrim.usMsgName = ID_VOICE_OM_SET_NV_CNF;

    /* ���ִ�н�� */
    stRspPrim.enExeRslt = (VOS_UINT16)uwRet;

    /* ��SDT�ظ���ϢID_AUDIO_OM_SET_NV_CNF */
    OM_COMM_SendTranMsg(&stRspPrim, sizeof(stRspPrim));

    /* �ͷű����ڴ� */
    UCOM_MemFree(pucBuf);

    return uwRet;
}
VOS_UINT32 AUDIO_ENHANCE_MsgOmGetNvReq(VOS_VOID *pstOsaMsg)
{
    VOS_UINT32                          uwRet;
    MSG_OM_MLIB_PARA_GET_STRU          *pstOmPara;
    MSG_OM_MLIB_PARA_GET_STRU          *pstGetPara;
    VOS_UINT8                          *pucBuf;
    VOS_UINT32                          uwSize;

    pstOmPara = (MSG_OM_MLIB_PARA_GET_STRU*)pstOsaMsg;

    uwSize = sizeof(MSG_OM_MLIB_PARA_GET_STRU) + pstOmPara->uwSize;
    pucBuf = (VOS_UINT8*) UCOM_MemAlloc(uwSize);

    pstGetPara = (MSG_OM_MLIB_PARA_GET_STRU*)pucBuf;

    UCOM_MemCpy(pstGetPara, pstOmPara, sizeof(MSG_OM_MLIB_PARA_GET_STRU));
    UCOM_MemCpy(pstGetPara->aucData, pstOmPara->aucData, pstOmPara->uwSize);

    uwRet = MLIB_PathModuleGet(
                pstOmPara->uwPathID,
                pstOmPara->uwModuleID,
                pstGetPara->aucData,
                pstOmPara->uwSize);

    if(UCOM_RET_SUCC == uwRet)
    {
        pstGetPara->usMsgName = ID_VOICE_OM_GET_NV_CNF;

        OM_COMM_SendTranMsg(pstGetPara, uwSize);
    }

    UCOM_MemFree(pucBuf);

    return UCOM_RET_SUCC;
}


VOS_VOID AUDIO_ENHANCE_MsgSyncConfirm(VOS_UINT16  uhwMsgId,
                                                    VOS_UINT32  uwRet,
                                                    VOS_VOID   *pvTail,
                                                    VOS_UINT32  uwTailLen)
{
    APAUDIO_OP_RSLT_STRU    stCnfMsg;

    /* �����Ϣ */
    UCOM_MemSet(&stCnfMsg, 0, sizeof(APAUDIO_OP_RSLT_STRU));

    /* ������Ϣ�ͷ���ֵ */
    stCnfMsg.uhwMsgId   = uhwMsgId;
    stCnfMsg.uhwResult  = (VOS_UINT16)uwRet;

    /*  �ظ���Ϣ��AP */
    UCOM_COMM_SendDspMsg(DSP_PID_AUDIO,
                         ACPU_PID_OM,
                        &stCnfMsg,
                         sizeof(APAUDIO_OP_RSLT_STRU),
                         pvTail,
                         uwTailLen);
}
VOS_UINT32  AUDIO_ENHANCE_Init( VOS_VOID )
{
    AUDIO_ENHANCE_CTRL_STRU            *pstCtrl;
    AUDIO_ENHANCE_BUFF_STRU            *pstBuff;

    /* ��ȡ���ƽṹ�� */
    pstCtrl = AUDIO_ENHANCE_GetCtrl();

    UCOM_MemSet(pstCtrl, 0, sizeof(AUDIO_ENHANCE_CTRL_STRU));

    /* ���ñ����� */
    pstCtrl->uwProtectWord1 = UCOM_PROTECT_WORD1;
    pstCtrl->uwProtectWord2 = UCOM_PROTECT_WORD2;
    pstCtrl->uwProtectWord3 = UCOM_PROTECT_WORD3;
    pstCtrl->uwProtectWord4 = UCOM_PROTECT_WORD4;

    /* ��ȡ����ṹ�� */
    pstBuff = AUDIO_ENHANCE_GetBuff();

    UCOM_MemSet(pstBuff, 0, sizeof(AUDIO_ENHANCE_BUFF_STRU));

    /* ���ñ����� */
    pstBuff->uwProtectWord1 = UCOM_PROTECT_WORD1;
    pstBuff->uwProtectWord2 = UCOM_PROTECT_WORD2;
    pstBuff->uwProtectWord3 = UCOM_PROTECT_WORD3;
    pstBuff->uwProtectWord4 = UCOM_PROTECT_WORD4;

    return UCOM_RET_SUCC;
}


VOS_UINT32 AUDIO_ENHANCE_Start(
                AUDIO_ENHANCE_MODULE_ENUM_UINT32    enModule,
                VOS_UINT32                          uwSampleRate )
{
    VOS_UINT32                          uwRet;
    AUDIO_ENHANCE_CTRL_STRU            *pstEnhanceCtrl;
    AUDIO_ENHANCE_MODULE_CTRL_STRU     *pstModuleCtrl;

    /* ��ȡ���ƽṹ�� */
    pstEnhanceCtrl  = AUDIO_ENHANCE_GetCtrl();
    pstModuleCtrl   = AUDIO_ENHANCE_GetModuleCtrl(enModule);

    /* ���õ�ǰʹ�õ�ģ�� */
    pstEnhanceCtrl->enCurrModule = enModule;

    /* ���ü������� */
    pstModuleCtrl->uwRefCnt++;

    if(AUDIO_ENHANCE_STATUS_RUNNING == pstModuleCtrl->enStatus)
    {
        /* �ٴ�ע��ص����� */
        AUDIO_ENHANCE_RegisterPcmCallback();

        return UCOM_RET_SUCC;
    }

    /* ��¼������ */
    pstModuleCtrl->uwSampleRate = uwSampleRate;

    /* ��ʼ����ѧ���� */
    uwRet = AUDIO_ENHANCE_ProcInit(enModule);

    if(uwRet != UCOM_RET_SUCC)
    {
        OM_LogError2(AUDIO_ENHANCE_Init_ProcInitFailed, enModule, uwRet);

        return UCOM_RET_FUNC_DISABLE;
    }

    /* ������ѧ������� */
    uwRet = AUDIO_ENHANCE_ProcSetPara(enModule, pstEnhanceCtrl->enDevice);

    if(uwRet != UCOM_RET_SUCC)
    {
        OM_LogError2(AUDIO_ENHANCE_Init_ProcSetParaFailed, enModule, uwRet);

        return UCOM_RET_FUNC_DISABLE;
    }

    pstModuleCtrl->enStatus = AUDIO_ENHANCE_STATUS_RUNNING;

    /* ע��ͨ·����ص����� */
    AUDIO_ENHANCE_RegisterPcmCallback();

    /* ����Ҫ�����ʱ, ��ʼ������� */
    if (uwSampleRate != AUDIO_ENHANCE_DEFAULT_SAMPLE_RATE)
    {
        uwRet = AUDIO_ENHANCE_InitSrc(enModule, uwSampleRate);

        if(uwRet != UCOM_RET_SUCC)
        {
            OM_LogError1(AUDIO_ENHANCE_InitSrcError, uwRet);

            return UCOM_RET_FUNC_DISABLE;
        }
    }

    return UCOM_RET_SUCC;
}
VOS_UINT32 AUDIO_ENHANCE_SetDevice( AUDIO_ENHANCE_DEVICE_ENUM_UINT32 enDevice )
{
    AUDIO_ENHANCE_CTRL_STRU            *pstEnhanceCtrl;
    AUDIO_ENHANCE_MODULE_CTRL_STRU     *pstModuleCtrl;
    VOS_UINT32                          uwRet;

    uwRet = UCOM_RET_SUCC;

    /* ��ȡ���ƽṹ�� */
    pstEnhanceCtrl = AUDIO_ENHANCE_GetCtrl();

    /* ���ģ��Ϸ��� */
    if(pstEnhanceCtrl->enCurrModule >= AUDIO_ENHANCE_MODULE_BUTT)
    {
        OM_LogError1(AUDIO_ENHANCE_SetDeviceModuleError, pstEnhanceCtrl->enCurrModule);

        return UCOM_RET_FAIL;
    }

    pstModuleCtrl   = AUDIO_ENHANCE_GetModuleCtrl(pstEnhanceCtrl->enCurrModule);

    /* ��¼�豸ģʽ */
    pstEnhanceCtrl->enDevice = enDevice;

    /* ���´������ */
    if(AUDIO_ENHANCE_STATUS_RUNNING == pstModuleCtrl->enStatus)
    {
        uwRet = AUDIO_ENHANCE_ProcSetPara(pstEnhanceCtrl->enCurrModule, enDevice);
    }

    /* ��/�ر�SmartPA */
    if((AUDIO_ENHANCE_MODULE_VOIP == pstEnhanceCtrl->enCurrModule)
     ||(AUDIO_ENHANCE_MODULE_SIRI == pstEnhanceCtrl->enCurrModule))
    {
        AUDIO_ENHANCE_SetExRefPort(enDevice);
    }

    return uwRet;
}
VOS_UINT32 AUDIO_ENHANCE_Stop( AUDIO_ENHANCE_MODULE_ENUM_UINT32 enModule )
{
    AUDIO_ENHANCE_MODULE_CTRL_STRU     *pstModuleCtrl;

    /* ��ȡ���ƽṹ�� */
    pstModuleCtrl   = AUDIO_ENHANCE_GetModuleCtrl(enModule);

    if(0 == pstModuleCtrl->uwRefCnt)
    {
        OM_LogWarning1(AUDIO_ENHANCE_StopRefCntInconsistent, enModule);

        return UCOM_RET_FAIL;
    }

    /* ���ü����ݼ� */
    pstModuleCtrl->uwRefCnt--;

    if(0 == pstModuleCtrl->uwRefCnt)
    {
        if(AUDIO_ENHANCE_STATUS_RUNNING == pstModuleCtrl->enStatus)
        {
            /* �����ѧ���� */
            AUDIO_ENHANCE_ProcClear(enModule);

            /* ȥע��ͨ·����ص����� */
            AUDIO_PCM_RegisterPcmCbFunc(
                    AUDIO_PCM_MODE_CAPTURE,
                    AUDIO_PCM_OBJ_AP,
                    VOS_NULL,
                    VOS_NULL);

            AUDIO_PCM_RegisterPcmCbFunc(
                    AUDIO_PCM_MODE_PLAYBACK,
                    AUDIO_PCM_OBJ_AP,
                    VOS_NULL,
                    VOS_NULL);

            /* �رձ���� */
            if (pstModuleCtrl->uwSampleRate != AUDIO_ENHANCE_DEFAULT_SAMPLE_RATE)
            {
                AUDIO_ENHANCE_CloseSrc(enModule);
            }
        }

        pstModuleCtrl->enStatus = AUDIO_ENHANCE_STATUS_IDLE;
    }

    return UCOM_RET_SUCC;

}
VOS_VOID AUDIO_ENHANCE_RegisterPcmCallback( VOS_VOID )
{
    /* ע��ͨ·����ص����� */
    AUDIO_PCM_RegisterPcmCbFunc(AUDIO_PCM_MODE_CAPTURE,
                                AUDIO_PCM_OBJ_AP,
                                VOS_NULL,
                                (AUDIO_PCM_CALLBACK)AUDIO_ENHANCE_ProcMicIn);

    AUDIO_PCM_RegisterPcmCbFunc(AUDIO_PCM_MODE_PLAYBACK,
                                AUDIO_PCM_OBJ_AP,
                                VOS_NULL,
                                (AUDIO_PCM_CALLBACK)AUDIO_ENHANCE_ProcSpkOut);
}
VOS_UINT32 AUDIO_ENHANCE_ProcInit( AUDIO_ENHANCE_MODULE_ENUM_UINT32 enModule )
{
    VOS_UINT32                          uwRet;
    AUDIO_ENHANCE_MODULE_REG_STRU      *pstModule;

    pstModule = AUDIO_ENHANCE_GetModuleReg(enModule);

    /* ��ʼ����ѧ���� */
    uwRet = MLIB_PathInit(pstModule->uwMicInPathID);

    if(uwRet != UCOM_RET_SUCC)
    {
        OM_LogError2(AUDIO_ENHANCE_ProcMicIn_InitErr, pstModule->uwMicInPathID, uwRet);

        return UCOM_RET_FUNC_DISABLE;
    }

    uwRet = MLIB_PathInit(pstModule->uwSpkOutPathID);

    if(uwRet != UCOM_RET_SUCC)
    {
        OM_LogError2(AUDIO_ENHANCE_ProcSpkOut_InitErr, pstModule->uwSpkOutPathID, uwRet);

        return UCOM_RET_FUNC_DISABLE;
    }

    return uwRet;
}
VOS_UINT32 AUDIO_ENHANCE_ProcSetPara(
                AUDIO_ENHANCE_MODULE_ENUM_UINT32            enModule,
                AUDIO_ENHANCE_DEVICE_ENUM_UINT32            enDevice )
{
    VOS_UINT32                          uwRet;
    MLIB_PARA_STRU                      stPara;
    AUDIO_ENHANCE_MODULE_CTRL_STRU     *pstModuleCtrl;
    AUDIO_ENHANCE_MODULE_REG_STRU      *pstModuleReg;

    /* ��ȡ���ƽṹ�� */
    pstModuleCtrl = AUDIO_ENHANCE_GetModuleCtrl(enModule);
    /* ��ȡע��ṹ�� */
    pstModuleReg  = AUDIO_ENHANCE_GetModuleReg(enModule);

    UCOM_MemSet(&stPara, 0, sizeof(MLIB_PARA_STRU));

    /* ͨ·�������� */
    stPara.uwChnNum     = 2;
    stPara.uwSampleRate = pstModuleCtrl->uwSampleRate;
    stPara.uwFrameLen   =  ((pstModuleCtrl->uwSampleRate / 8000) * 160);
    stPara.uwResolution = 16;
    stPara.enDevice     = enDevice;
    stPara.enParaSource = MLIB_PARA_SOURCE_NV;
    stPara.uwDataSize   = 0;

    uwRet = MLIB_PathSet(
                pstModuleReg->uwMicInPathID,
                &stPara,
                sizeof(MLIB_PARA_STRU));

    if(uwRet != UCOM_RET_SUCC)
    {
        OM_LogError2(AUDIO_ENHANCE_SetPara_RecordFail, pstModuleReg->uwMicInPathID, uwRet);
    }

    /* ͨ·�������� */
    stPara.uwChnNum     = 2;

    uwRet = MLIB_PathSet(
                pstModuleReg->uwSpkOutPathID,
                &stPara,
                sizeof(MLIB_PARA_STRU));

    if(uwRet != UCOM_RET_SUCC)
    {
        OM_LogError2(AUDIO_ENHANCE_SetPara_RecordFail, pstModuleReg->uwSpkOutPathID, uwRet);
    }

    return uwRet;
}


VOS_UINT32 AUDIO_ENHANCE_ProcMicIn(
                VOS_VOID               *pshwBufOut,
                VOS_VOID               *pshwBufIn,
                VOS_UINT32              uwSampleRate,
                VOS_UINT32              uwChannelNum,
                VOS_UINT32              uwBufSize)
{
    VOS_UINT32                          uwRet;
    AUDIO_ENHANCE_CTRL_STRU            *pstEnhanceCtrl;
    AUDIO_ENHANCE_MODULE_CTRL_STRU     *pstModuleCtrl;
    AUDIO_ENHANCE_MODULE_REG_STRU      *pstModuleReg;
    MLIB_SOURCE_STRU                    stMlibIn, stMlibOut;
    VOS_UINT32                          uwProcSize;
    VOS_UINT32                          uwOutputBuffSize;

    /* ��ȡ���ƽṹ�� */
    pstEnhanceCtrl  = AUDIO_ENHANCE_GetCtrl();
    pstModuleCtrl   = AUDIO_ENHANCE_GetModuleCtrl(pstEnhanceCtrl->enCurrModule);
    pstModuleReg    = AUDIO_ENHANCE_GetModuleReg(pstEnhanceCtrl->enCurrModule);

    /* ��ӡ��ǰģ�� */
    OM_LogInfo1(AUDIO_ENHANCE_Module_MicIn, pstEnhanceCtrl->enCurrModule);

    /* �����������Ϣ��0 */
    UCOM_MemSet(&stMlibIn,  0, sizeof(MLIB_SOURCE_STRU));
    UCOM_MemSet(&stMlibOut, 0, sizeof(MLIB_SOURCE_STRU));

    /* ��Running̬����͸�� */
    if(pstModuleCtrl->enStatus != AUDIO_ENHANCE_STATUS_RUNNING)
    {
        UCOM_MemCpy(pshwBufOut, pshwBufIn, uwBufSize);

        OM_LogWarning1(AUDIO_ENHANCE_ProcMicIn_FuncDisable, pstEnhanceCtrl->enCurrModule);

        uwRet = UCOM_RET_SUCC;
    }
    else
    {
        OM_CPUVIEW_EnterArea((VOS_UCHAR)OM_CPUVIEW_AREA_AUDIO_ENHANCE_PROC_MICIN);

        /* ��ȡ�������� */
        AUDIO_DEBUG_SendHookInd(AUDIO_HOOK_ENHANCE_MICIN, pshwBufIn, uwBufSize);

        /* ������Ҫ���������(������) */
        uwProcSize = AUDIO_ENHANCE_GetMonoFrmSize(pstModuleCtrl->uwSampleRate);

        if(pstModuleCtrl->uwSampleRate == uwSampleRate)
        {
            /* �����������ɻ�����ݲ�ֳ�2· */
            UCOM_COMM_GetChnBit16(
                    AUDIO_ENHANCE_GetMicInL(AUDIO_ENHANCE_BUFF_IN),
                    uwProcSize,
                    0,
                    pshwBufIn,
                    2);

            UCOM_COMM_GetChnBit16(
                    AUDIO_ENHANCE_GetMicInR(AUDIO_ENHANCE_BUFF_IN),
                    uwProcSize,
                    1,
                    pshwBufIn,
                    2);
        }
        else
        {
            /* �����������ɻ�����ݲ�ֳ�2· */
            UCOM_COMM_GetChnBit16(
                    AUDIO_ENHANCE_GetMicInL(AUDIO_ENHANCE_BUFF_SWAP),
                    (uwBufSize/2),
                    0,
                    pshwBufIn,
                    2);

            UCOM_COMM_GetChnBit16(
                    AUDIO_ENHANCE_GetMicInR(AUDIO_ENHANCE_BUFF_SWAP),
                    (uwBufSize/2),
                    1,
                    pshwBufIn,
                    2);

            /* ִ�б���� */
            AUDIO_ENHANCE_DoSrc(XA_SRC_VOIP_MICIN_48K_16K_L,
                                AUDIO_ENHANCE_GetMicInL(AUDIO_ENHANCE_BUFF_SWAP),
                                AUDIO_ENHANCE_GetMicInL(AUDIO_ENHANCE_BUFF_IN),
                                &uwOutputBuffSize);

            AUDIO_ENHANCE_DoSrc(XA_SRC_VOIP_MICIN_48K_16K_R,
                                AUDIO_ENHANCE_GetMicInR(AUDIO_ENHANCE_BUFF_SWAP),
                                AUDIO_ENHANCE_GetMicInR(AUDIO_ENHANCE_BUFF_IN),
                                &uwOutputBuffSize);

            /* ��ȡ��������Ϊ16K������ */
            AUDIO_DEBUG_SendHookInd(
                        AUDIO_HOOK_ENHANCE_MLIB_MICIN_16K_L,
                        AUDIO_ENHANCE_GetMicInL(AUDIO_ENHANCE_BUFF_IN),
                        uwProcSize);

            /* ��ȡ��������Ϊ16K������ */
            AUDIO_DEBUG_SendHookInd(
                        AUDIO_HOOK_ENHANCE_MLIB_MICIN_16K_R,
                        AUDIO_ENHANCE_GetMicInR(AUDIO_ENHANCE_BUFF_IN),
                        uwProcSize);

        }

        /* ������������ */
        stMlibIn.uwChnNum = 4;

        /* 2·Mic���� */
        MLIB_UtilitySetSource(
                    &stMlibIn,
                    0,
                    AUDIO_ENHANCE_GetMicInL(AUDIO_ENHANCE_BUFF_IN),
                    uwProcSize,
                    pstModuleCtrl->uwSampleRate);

        MLIB_UtilitySetSource(
                    &stMlibIn,
                    1,
                    AUDIO_ENHANCE_GetMicInR(AUDIO_ENHANCE_BUFF_IN),
                    uwProcSize,
                    pstModuleCtrl->uwSampleRate);

        /* ���������������� */
        AUDIO_ENHANCE_ProcEcRef(pstModuleCtrl->uwSampleRate);

        /* 2·Spk���Ҳ��Ϊ���� */
        MLIB_UtilitySetSource(
                    &stMlibIn,
                    2,
                    AUDIO_ENHANCE_GetSpkOutL(AUDIO_ENHANCE_BUFF_OUT),
                    uwProcSize,
                    pstModuleCtrl->uwSampleRate);

        MLIB_UtilitySetSource(
                    &stMlibIn,
                    3,
                    AUDIO_ENHANCE_GetSpkOutR(AUDIO_ENHANCE_BUFF_OUT),
                    uwProcSize,
                    pstModuleCtrl->uwSampleRate);

        /* ����������� */
        stMlibOut.uwChnNum = 4;

        MLIB_UtilitySetSource(
                    &stMlibOut,
                    0,
                    AUDIO_ENHANCE_GetMicInL(AUDIO_ENHANCE_BUFF_OUT),
                    uwProcSize,
                    pstModuleCtrl->uwSampleRate);

        MLIB_UtilitySetSource(
                    &stMlibOut,
                    1,
                    AUDIO_ENHANCE_GetMicInR(AUDIO_ENHANCE_BUFF_OUT),
                    uwProcSize,
                    pstModuleCtrl->uwSampleRate);

        /* 2·Spk���Ҳ��Ϊ��� */
        MLIB_UtilitySetSource(
                    &stMlibOut,
                    2,
                    AUDIO_ENHANCE_GetSpkOutL(AUDIO_ENHANCE_BUFF_OUT),
                    uwProcSize,
                    pstModuleCtrl->uwSampleRate);

        MLIB_UtilitySetSource(
                    &stMlibOut,
                    3,
                    AUDIO_ENHANCE_GetSpkOutR(AUDIO_ENHANCE_BUFF_OUT),
                    uwProcSize,
                    pstModuleCtrl->uwSampleRate);

        OM_CPUVIEW_EnterArea((VOS_UCHAR)OM_CPUVIEW_AREA_AUDIO_ENHANCE_PROC_MLIB_MICIN);

        /* ������ѧ���� */
        uwRet = MLIB_PathProc(
            pstModuleReg->uwMicInPathID,
            &stMlibOut,
            &stMlibIn);

        OM_CPUVIEW_ExitArea((VOS_UCHAR)OM_CPUVIEW_AREA_AUDIO_ENHANCE_PROC_MLIB_MICIN);

        if(pstModuleCtrl->uwSampleRate == uwSampleRate)
        {
            /* ��������������2·��� */
            UCOM_COMM_SetChnBit16(
                    pshwBufOut,
                    2,
                    AUDIO_ENHANCE_GetMicInL(AUDIO_ENHANCE_BUFF_OUT),
                    uwProcSize,
                    0);

            UCOM_COMM_SetChnBit16(
                    pshwBufOut,
                    2,
                    AUDIO_ENHANCE_GetMicInR(AUDIO_ENHANCE_BUFF_OUT),
                    uwProcSize,
                    1);
        }
        else
        {

            /* ִ�б���� */
            AUDIO_ENHANCE_DoSrc(XA_SRC_VOIP_MICIN_16K_48K_L,
                                AUDIO_ENHANCE_GetMicInL(AUDIO_ENHANCE_BUFF_OUT),
                                AUDIO_ENHANCE_GetMicInL(AUDIO_ENHANCE_BUFF_SWAP),
                                &uwOutputBuffSize);

            AUDIO_ENHANCE_DoSrc(XA_SRC_VOIP_MICIN_16K_48K_R,
                                AUDIO_ENHANCE_GetMicInR(AUDIO_ENHANCE_BUFF_OUT),
                                AUDIO_ENHANCE_GetMicInR(AUDIO_ENHANCE_BUFF_SWAP),
                                &uwOutputBuffSize);

            /* ��ȡ��������Ϊ48K������ */
            AUDIO_DEBUG_SendHookInd(
                        AUDIO_HOOK_ENHANCE_MLIB_MICIN_48K_L,
                        AUDIO_ENHANCE_GetMicInL(AUDIO_ENHANCE_BUFF_SWAP),
                        (uwBufSize/2));

            /* ��ȡ��������Ϊ48K������ */
            AUDIO_DEBUG_SendHookInd(
                        AUDIO_HOOK_ENHANCE_MLIB_MICIN_48K_R,
                        AUDIO_ENHANCE_GetMicInR(AUDIO_ENHANCE_BUFF_SWAP),
                        (uwBufSize/2));

            /* ��������������2·��� */
            UCOM_COMM_SetChnBit16(
                    pshwBufOut,
                    2,
                    AUDIO_ENHANCE_GetMicInL(AUDIO_ENHANCE_BUFF_SWAP),
                    (uwBufSize/2),
                    0);

            UCOM_COMM_SetChnBit16(
                    pshwBufOut,
                    2,
                    AUDIO_ENHANCE_GetMicInR(AUDIO_ENHANCE_BUFF_SWAP),
                    (uwBufSize/2),
                    1);

        }

        OM_CPUVIEW_ExitArea((VOS_UCHAR)OM_CPUVIEW_AREA_AUDIO_ENHANCE_PROC_MICIN);

    }

    return uwRet;
}
VOS_UINT32 AUDIO_ENHANCE_ProcSpkOut(
                VOS_VOID               *pshwBufOut,
                VOS_VOID               *pshwBufIn,
                VOS_UINT32              uwSampleRate,
                VOS_UINT32              uwChannelNum,
                VOS_UINT32              uwBufSize)
{
    VOS_UINT32                          uwRet;
    AUDIO_ENHANCE_CTRL_STRU            *pstEnhanceCtrl;
    AUDIO_ENHANCE_MODULE_CTRL_STRU     *pstModuleCtrl;
    AUDIO_ENHANCE_MODULE_REG_STRU      *pstModuleReg;
    MLIB_SOURCE_STRU                    stMlibIn, stMlibOut;
    VOS_UINT32                          uwProcSize;
    VOS_UINT32                          uwOutputBuffSize;

    /* ��ȡ���ƽṹ�� */
    pstEnhanceCtrl  = AUDIO_ENHANCE_GetCtrl();
    pstModuleCtrl   = AUDIO_ENHANCE_GetModuleCtrl(pstEnhanceCtrl->enCurrModule);
    pstModuleReg    = AUDIO_ENHANCE_GetModuleReg(pstEnhanceCtrl->enCurrModule);

    /* ��ӡ��ǰģ�� */
    OM_LogInfo1(AUDIO_ENHANCE_Module_SpkOut, pstEnhanceCtrl->enCurrModule);

    /* �����������Ϣ��0 */
    UCOM_MemSet(&stMlibIn,  0, sizeof(MLIB_SOURCE_STRU));
    UCOM_MemSet(&stMlibOut, 0, sizeof(MLIB_SOURCE_STRU));

    /* ��Running̬����͸�� */
    if(pstModuleCtrl->enStatus != AUDIO_ENHANCE_STATUS_RUNNING)
    {
        UCOM_MemCpy(pshwBufOut, pshwBufIn, uwBufSize);

        OM_LogWarning1(AUDIO_ENHANCE_ProcSpkOut_FuncDisable, pstEnhanceCtrl->enCurrModule);

        uwRet = UCOM_RET_SUCC;
    }
    else
    {
        OM_CPUVIEW_EnterArea((VOS_UCHAR)OM_CPUVIEW_AREA_AUDIO_ENHANCE_PROC_SPKOUT);

        /* ��ȡ�������� */
        AUDIO_DEBUG_SendHookInd(AUDIO_HOOK_ENHANCE_SPKOUT, pshwBufIn, uwBufSize);

        /* ������Ҫ���������(������) */
        uwProcSize = AUDIO_ENHANCE_GetMonoFrmSize(pstModuleCtrl->uwSampleRate);

        if(pstModuleCtrl->uwSampleRate == uwSampleRate)
        {
            /* �����������ɻ�����ݲ�ֳ�2· */
            UCOM_COMM_GetChnBit16(
                    AUDIO_ENHANCE_GetSpkOutL(AUDIO_ENHANCE_BUFF_IN),
                    uwProcSize,
                    0,
                    pshwBufIn,
                    2);

            UCOM_COMM_GetChnBit16(
                    AUDIO_ENHANCE_GetSpkOutR(AUDIO_ENHANCE_BUFF_IN),
                    uwProcSize,
                    1,
                    pshwBufIn,
                    2);
        }
        else
        {
            /* �����������ɻ�����ݲ�ֳ�2· */
            UCOM_COMM_GetChnBit16(
                    AUDIO_ENHANCE_GetSpkOutL(AUDIO_ENHANCE_BUFF_SWAP),
                    (uwBufSize/2),
                    0,
                    pshwBufIn,
                    2);

            UCOM_COMM_GetChnBit16(
                    AUDIO_ENHANCE_GetSpkOutR(AUDIO_ENHANCE_BUFF_SWAP),
                    (uwBufSize/2),
                    1,
                    pshwBufIn,
                    2);

            /* ִ�б���� */
            AUDIO_ENHANCE_DoSrc(XA_SRC_VOIP_SPKOUT_48K_16K_L,
                                AUDIO_ENHANCE_GetSpkOutL(AUDIO_ENHANCE_BUFF_SWAP),
                                AUDIO_ENHANCE_GetSpkOutL(AUDIO_ENHANCE_BUFF_IN),
                                &uwOutputBuffSize);

            AUDIO_ENHANCE_DoSrc(XA_SRC_VOIP_SPKOUT_48K_16K_R,
                                AUDIO_ENHANCE_GetSpkOutR(AUDIO_ENHANCE_BUFF_SWAP),
                                AUDIO_ENHANCE_GetSpkOutR(AUDIO_ENHANCE_BUFF_IN),
                                &uwOutputBuffSize);

            /* ��ȡ������16K���������� */
            AUDIO_DEBUG_SendHookInd(
                        AUDIO_HOOK_ENHANCE_MLIB_SPKOUT_16K_L,
                        AUDIO_ENHANCE_GetSpkOutL(AUDIO_ENHANCE_BUFF_IN),
                        uwProcSize);

            /* ��ȡ������16K������ */
            AUDIO_DEBUG_SendHookInd(
                        AUDIO_HOOK_ENHANCE_MLIB_SPKOUT_16K_R,
                        AUDIO_ENHANCE_GetSpkOutR(AUDIO_ENHANCE_BUFF_IN),
                        uwProcSize);


        }

        /* ������������ */
        stMlibIn.uwChnNum = 2;

        MLIB_UtilitySetSource(
                    &stMlibIn,
                    0,
                    AUDIO_ENHANCE_GetSpkOutL(AUDIO_ENHANCE_BUFF_IN),
                    uwProcSize,
                    pstModuleCtrl->uwSampleRate);

        MLIB_UtilitySetSource(
                    &stMlibIn,
                    1,
                    AUDIO_ENHANCE_GetSpkOutR(AUDIO_ENHANCE_BUFF_IN),
                    uwProcSize,
                    pstModuleCtrl->uwSampleRate);


        /* ����������� */
        stMlibOut.uwChnNum = 2;

        MLIB_UtilitySetSource(
                    &stMlibOut,
                    0,
                    AUDIO_ENHANCE_GetSpkOutL(AUDIO_ENHANCE_BUFF_OUT),
                    uwProcSize,
                    pstModuleCtrl->uwSampleRate);

        MLIB_UtilitySetSource(
                    &stMlibOut,
                    1,
                    AUDIO_ENHANCE_GetSpkOutR(AUDIO_ENHANCE_BUFF_OUT),
                    uwProcSize,
                    pstModuleCtrl->uwSampleRate);

        OM_CPUVIEW_EnterArea((VOS_UCHAR)OM_CPUVIEW_AREA_AUDIO_ENHANCE_PROC_MLIB_SPKOUT);

        /* ������ѧ���� */
        uwRet = MLIB_PathProc(
            pstModuleReg->uwSpkOutPathID,
            &stMlibOut,
            &stMlibIn);

        OM_CPUVIEW_ExitArea((VOS_UCHAR)OM_CPUVIEW_AREA_AUDIO_ENHANCE_PROC_MLIB_SPKOUT);

        if(pstModuleCtrl->uwSampleRate == uwSampleRate)
        {
            /* ��������������2·��� */
            UCOM_COMM_SetChnBit16(
                    pshwBufOut,
                    2,
                    AUDIO_ENHANCE_GetSpkOutL(AUDIO_ENHANCE_BUFF_OUT),
                    uwProcSize,
                    0);

            UCOM_COMM_SetChnBit16(
                    pshwBufOut,
                    2,
                    AUDIO_ENHANCE_GetSpkOutR(AUDIO_ENHANCE_BUFF_OUT),
                    uwProcSize,
                    1);
        }
        else
        {
            /* ִ�б���� */
            AUDIO_ENHANCE_DoSrc(XA_SRC_VOIP_SPKOUT_16K_48K_L,
                                AUDIO_ENHANCE_GetSpkOutL(AUDIO_ENHANCE_BUFF_OUT),
                                AUDIO_ENHANCE_GetSpkOutL(AUDIO_ENHANCE_BUFF_SWAP),
                                &uwOutputBuffSize);

            AUDIO_ENHANCE_DoSrc(XA_SRC_VOIP_SPKOUT_16K_48K_R,
                                AUDIO_ENHANCE_GetSpkOutR(AUDIO_ENHANCE_BUFF_OUT),
                                AUDIO_ENHANCE_GetSpkOutR(AUDIO_ENHANCE_BUFF_SWAP),
                                &uwOutputBuffSize);

            /* ��ȡ������48K���� */
            AUDIO_DEBUG_SendHookInd(
                        AUDIO_HOOK_ENHANCE_MLIB_SPKOUT_48K_L,
                        AUDIO_ENHANCE_GetSpkOutL(AUDIO_ENHANCE_BUFF_SWAP),
                        (uwBufSize/2));

            /* ��ȡ������48K���� */
            AUDIO_DEBUG_SendHookInd(
                        AUDIO_HOOK_ENHANCE_MLIB_SPKOUT_48K_R,
                        AUDIO_ENHANCE_GetSpkOutR(AUDIO_ENHANCE_BUFF_SWAP),
                        (uwBufSize/2));

            /* ��������������2·��� */
            UCOM_COMM_SetChnBit16(
                    pshwBufOut,
                    2,
                    AUDIO_ENHANCE_GetSpkOutL(AUDIO_ENHANCE_BUFF_SWAP),
                    (uwBufSize/2),
                    0);

            UCOM_COMM_SetChnBit16(
                    pshwBufOut,
                    2,
                    AUDIO_ENHANCE_GetSpkOutR(AUDIO_ENHANCE_BUFF_SWAP),
                    (uwBufSize/2),
                    1);
        }

        OM_CPUVIEW_ExitArea((VOS_UCHAR)OM_CPUVIEW_AREA_AUDIO_ENHANCE_PROC_SPKOUT);
    }

    return uwRet;
}
VOS_UINT32 AUDIO_ENHANCE_ProcClear( AUDIO_ENHANCE_MODULE_ENUM_UINT32 enModule )
{
    AUDIO_ENHANCE_MODULE_REG_STRU      *pstModuleReg;

    /* ��ȡע��ṹ�� */
    pstModuleReg  = AUDIO_ENHANCE_GetModuleReg(enModule);

    MLIB_PathClear(pstModuleReg->uwMicInPathID);
    MLIB_PathClear(pstModuleReg->uwSpkOutPathID);

    return UCOM_RET_SUCC;
}
VOS_UINT32  AUDIO_ENHANCE_DoSrc(
                XA_SRC_PROC_ID_ENUM_UINT32 enProcId,
                VOS_VOID                  *pInputBuff,
                VOS_VOID                  *pOutputBuff,
                VOS_UINT32                *puwOutputSize)
{
    VOS_UINT32                          uwProcTimes;
    VOS_UINT32                          uwRet;
    AUDIO_ENHANCE_CTRL_STRU            *pstEnhanceCtrl;

    /* ��ȡ���ƽṹ�� */
    pstEnhanceCtrl  = AUDIO_ENHANCE_GetCtrl();

    /* δ��ʼ����ֹͣ���� */
    if(0 == pstEnhanceCtrl->uwSrcRefFlag)
    {
        OM_LogError(AUDIO_ENHANCE_DoSrcError_UnInit);

        return VOS_ERR;
    }

    /* ����enProcId���ò�ͬ�Ĵ������
     * ��48KתΪ16K������960���㡣һ�δ���240���㣬����4�� */
    if (  (XA_SRC_VOIP_MICIN_48K_16K_L  == enProcId)
        ||(XA_SRC_VOIP_MICIN_48K_16K_R  == enProcId)
        ||(XA_SRC_VOIP_SPKOUT_48K_16K_L == enProcId)
        ||(XA_SRC_VOIP_SPKOUT_48K_16K_R == enProcId)
        ||(XA_SRC_VOIP_SMARTPA_48K_16K_L == enProcId)
        ||(XA_SRC_VOIP_SMARTPA_48K_16K_R == enProcId))
    {
        uwProcTimes = 4;
    }
    /* ��16KתΪ48K������320���㡣һ�δ���160���㣬����2�� */
    else if (  (XA_SRC_VOIP_MICIN_16K_48K_L  == enProcId)
             ||(XA_SRC_VOIP_MICIN_16K_48K_R  == enProcId)
             ||(XA_SRC_VOIP_SPKOUT_16K_48K_L == enProcId)
             ||(XA_SRC_VOIP_SPKOUT_16K_48K_R == enProcId))
    {
        uwProcTimes = 2;
    }
    /* ��������չ */
    else
    {
        OM_LogError1(AUDIO_ENHANCE_DoSrcError, enProcId);

        return VOS_ERR;
    }

    uwRet = XA_SRC_Proc8x(enProcId,
                          pInputBuff,
                          pOutputBuff,
                          puwOutputSize,
                          uwProcTimes);

    if (VOS_OK != uwRet)
    {
        OM_LogError1(AUDIO_ENHANCE_DoSrcError, uwRet);
    }

    return uwRet;
}


VOS_UINT32  AUDIO_ENHANCE_InitSrc(
                AUDIO_ENHANCE_MODULE_ENUM_UINT32    enModule,
                VOS_UINT32                          uwSampleRate  )
{
    AUDIO_ENHANCE_CTRL_STRU            *pstEnhanceCtrl;
    XA_SRC_USER_CFG_STRU                stMicIn_UP;
    XA_SRC_USER_CFG_STRU                stMicIn_DOWN;
    VOS_INT32                           swRet = VOS_OK;

    /* ��ȡ���ƽṹ�� */
    pstEnhanceCtrl  = AUDIO_ENHANCE_GetCtrl();

    /* ���ñ��Ϊ0ʱ���г�ʼ�� */
    if(0 == pstEnhanceCtrl->uwSrcRefFlag)
    {
        /* ����MicIn������ */
        stMicIn_DOWN.swChannels         = 1;
        stMicIn_DOWN.swInputChunkSize   = XA_SRC_INPUT_CHUNKSIZE_240;
        stMicIn_DOWN.swInputSmpRat      = 48000;
        stMicIn_DOWN.swOutputSmpRat     = (VOS_INT32)uwSampleRate;

        /* ����MicIn������ */
        stMicIn_UP.swChannels           = 1;
        stMicIn_UP.swInputChunkSize     = XA_SRC_INPUT_CHUNKSIZE_160;
        stMicIn_UP.swInputSmpRat        = (VOS_INT32)uwSampleRate;
        stMicIn_UP.swOutputSmpRat       = 48000;

        /* ��ʼ��MicIn������������ */
        swRet += XA_SRC_Init(XA_SRC_VOIP_MICIN_48K_16K_L, &stMicIn_DOWN);

        /* ��ʼ��MicIn������������ */
        swRet += XA_SRC_Init(XA_SRC_VOIP_MICIN_48K_16K_R, &stMicIn_DOWN);

        /* ��ʼ��MicIn������������ */
        swRet += XA_SRC_Init(XA_SRC_VOIP_MICIN_16K_48K_L, &stMicIn_UP);

        /* ��ʼ��MicIn������������ */
        swRet += XA_SRC_Init(XA_SRC_VOIP_MICIN_16K_48K_R, &stMicIn_UP);


        /* ��ʼ��SpkOut������������ */
        swRet += XA_SRC_Init(XA_SRC_VOIP_SPKOUT_48K_16K_L, &stMicIn_DOWN);

        /* ��ʼ��SpkOut������������ */
        swRet += XA_SRC_Init(XA_SRC_VOIP_SPKOUT_48K_16K_R, &stMicIn_DOWN);

        /* ��ʼ��SpkOut������������ */
        swRet += XA_SRC_Init(XA_SRC_VOIP_SPKOUT_16K_48K_L, &stMicIn_UP);

        /* ��ʼ��SpkOut������������ */
        swRet += XA_SRC_Init(XA_SRC_VOIP_SPKOUT_16K_48K_R, &stMicIn_UP);

        /* ��ʼ��SmartPA������������ */
        swRet += XA_SRC_Init(XA_SRC_VOIP_SMARTPA_48K_16K_L, &stMicIn_DOWN);

        /* ��ʼ��SmartPA������������ */
        swRet += XA_SRC_Init(XA_SRC_VOIP_SMARTPA_48K_16K_R, &stMicIn_DOWN);

        if (VOS_OK != swRet)
        {
            OM_LogError1(AUDIO_ENHANCE_InitSrcError, swRet);

            return (VOS_UINT32)swRet;
        }
        else
        {
            OM_LogInfo(AUDIO_ENHANCE_InitSrcSucc);
        }
    }

    /* ����ʹ�ñ�� */
    pstEnhanceCtrl->uwSrcRefFlag |= (1 << enModule);

    OM_LogInfo1(AUDIO_ENHANCE_InitSrcFlag, pstEnhanceCtrl->uwSrcRefFlag);

    return VOS_OK;

}
VOS_VOID  AUDIO_ENHANCE_CloseSrc( AUDIO_ENHANCE_MODULE_ENUM_UINT32 enModule )
{
    AUDIO_ENHANCE_CTRL_STRU            *pstEnhanceCtrl;

    /* ��ȡ���ƽṹ�� */
    pstEnhanceCtrl  = AUDIO_ENHANCE_GetCtrl();

    /* δ��ʼ�� */
    if(0 == pstEnhanceCtrl->uwSrcRefFlag)
    {
        OM_LogError(AUDIO_ENHANCE_CloseSrcError);

        return;
    }

    /* ����Ӧģ��ʹ��λ��0 */
    pstEnhanceCtrl->uwSrcRefFlag &= (~(VOS_UINT32)(1<<enModule));

    if(0 == pstEnhanceCtrl->uwSrcRefFlag)
    {
        XA_SRC_Close(XA_SRC_VOIP_MICIN_48K_16K_L);
        XA_SRC_Close(XA_SRC_VOIP_MICIN_48K_16K_R);
        XA_SRC_Close(XA_SRC_VOIP_MICIN_16K_48K_L);
        XA_SRC_Close(XA_SRC_VOIP_MICIN_16K_48K_R);

        XA_SRC_Close(XA_SRC_VOIP_SPKOUT_48K_16K_L);
        XA_SRC_Close(XA_SRC_VOIP_SPKOUT_48K_16K_R);
        XA_SRC_Close(XA_SRC_VOIP_SPKOUT_16K_48K_L);
        XA_SRC_Close(XA_SRC_VOIP_SPKOUT_16K_48K_R);

        XA_SRC_Close(XA_SRC_VOIP_SMARTPA_48K_16K_L);
        XA_SRC_Close(XA_SRC_VOIP_SMARTPA_48K_16K_R);

        OM_LogInfo(AUDIO_ENHANCE_CloseSrcSucc);
    }

    OM_LogInfo1(AUDIO_ENHANCE_CloseSrcFlag, pstEnhanceCtrl->uwSrcRefFlag);
}
VOS_VOID  AUDIO_ENHANCE_SetExRefPort( AUDIO_ENHANCE_DEVICE_ENUM_UINT32 enDevice)
{
    VOICE_SMART_PA_CFG_NV_STRU          stPaNvCfg;
    AUDIO_PCM_CFG_STRU                 *pstPcmCfg;
    VOS_UINT32                          uwProcSize;
    AUDIO_ENHANCE_CTRL_STRU            *pstEnhanceCtrl;

    /* ��ȡ���ƽṹ�� */
    pstEnhanceCtrl  = AUDIO_ENHANCE_GetCtrl();

    UCOM_MemSet(&stPaNvCfg, 0, sizeof(VOICE_SMART_PA_CFG_NV_STRU));

    pstPcmCfg   = AUDIO_PcmGetPcmCfgPtr(AUDIO_PCM_MODE_PLAYBACK);

    /* ��ȡNV�� */
    UCOM_NV_Read(en_NV_SmartPACfg,
                 &stPaNvCfg,
                 sizeof(VOICE_SMART_PA_CFG_NV_STRU));

    /* ���������SMART PA���ã���������SMART PA SIO */
    if (   (AUDIO_ENHANCE_DEVICE_HANDFREE == enDevice)
        && (CODEC_SWITCH_ON == stPaNvCfg.enEnable))
    {
        /* ������Ҫ���������(������) */
        uwProcSize = AUDIO_ENHANCE_GetMonoFrmSize(pstPcmCfg->uwSampleRate);

        /* ����ģʽ�£�����SmartPA�����в�����ʽDMA���� */
        UCOM_PCM_SmartPaStartLoopDma((VOS_UINT16)(uwProcSize * UCOM_PCM_I2S_CHANNEL_NUM),
                                     pstPcmCfg->uwSampleRate,
                                     stPaNvCfg.uhwIsMaster,
                                     UCOM_PCM_GetSmartPaDmacChn(),
                                     VOS_NULL);

        pstEnhanceCtrl->enExRef = AUDIO_ENHANCE_EX_REF_ON;
    }
    else
    {
        /* �رս���Smart PA�ķ����ź� */
        UCOM_PCM_SmartPaStop();

        pstEnhanceCtrl->enExRef = AUDIO_ENHANCE_EX_REF_OFF;
    }

}


VOS_VOID AUDIO_ENHANCE_ProcEcRef( VOS_UINT32 uwSampleRate )
{
    AUDIO_ENHANCE_CTRL_STRU            *pstEnhanceCtrl;
    AUDIO_PCM_CFG_STRU                 *pstPcmCfg;
    VOS_INT16                          *pshwExRef;
    VOS_UINT32                          uwOutputBuffSize;

    /* ��ȡ���ƽṹ�� */
    pstEnhanceCtrl  = AUDIO_ENHANCE_GetCtrl();

    pstPcmCfg       = AUDIO_PcmGetPcmCfgPtr(AUDIO_PCM_MODE_PLAYBACK);

    if(AUDIO_ENHANCE_EX_REF_ON == pstEnhanceCtrl->enExRef)
    {
        pshwExRef = (VOS_INT16*)UCOM_PCM_SmartPaGetAvailVirtAddr();

        /* �����������ɻ�����ݲ�ֳ�2· */
        UCOM_COMM_GetChnBit16(
                AUDIO_ENHANCE_GetSpkOutL(AUDIO_ENHANCE_BUFF_SWAP),
                AUDIO_ENHANCE_GetMonoFrmSize(pstPcmCfg->uwSampleRate),
                0,
                pshwExRef,
                2);

        UCOM_COMM_GetChnBit16(
                AUDIO_ENHANCE_GetSpkOutR(AUDIO_ENHANCE_BUFF_SWAP),
                AUDIO_ENHANCE_GetMonoFrmSize(pstPcmCfg->uwSampleRate),
                1,
                pshwExRef,
                2);

        /* ִ�б���� */
        AUDIO_ENHANCE_DoSrc(XA_SRC_VOIP_SMARTPA_48K_16K_L,
                            AUDIO_ENHANCE_GetSpkOutL(AUDIO_ENHANCE_BUFF_SWAP),
                            AUDIO_ENHANCE_GetSpkOutL(AUDIO_ENHANCE_BUFF_OUT),
                            &uwOutputBuffSize);

        AUDIO_ENHANCE_DoSrc(XA_SRC_VOIP_SMARTPA_48K_16K_R,
                            AUDIO_ENHANCE_GetSpkOutR(AUDIO_ENHANCE_BUFF_SWAP),
                            AUDIO_ENHANCE_GetSpkOutR(AUDIO_ENHANCE_BUFF_OUT),
                            &uwOutputBuffSize);

        /* ��ȡ��������Ϊ16K������ */
        AUDIO_DEBUG_SendHookInd(
                    AUDIO_HOOK_ENHANCE_MLIB_SMARTPA_16K_L,
                    AUDIO_ENHANCE_GetSpkOutL(AUDIO_ENHANCE_BUFF_OUT),
                    uwOutputBuffSize);

        /* ��ȡ��������Ϊ16K������ */
        AUDIO_DEBUG_SendHookInd(
                    AUDIO_HOOK_ENHANCE_MLIB_SMARTPA_16K_R,
                    AUDIO_ENHANCE_GetSpkOutR(AUDIO_ENHANCE_BUFF_OUT),
                    uwOutputBuffSize);
    }
}
VOS_UINT32  AUDIO_ENHANCE_GetCurrentModule( VOS_VOID )
{
    AUDIO_ENHANCE_CTRL_STRU *pstEnhanceCtrl;

    /* ��ȡ���ƽṹ�� */
    pstEnhanceCtrl  = AUDIO_ENHANCE_GetCtrl();

    return pstEnhanceCtrl->enCurrModule;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

