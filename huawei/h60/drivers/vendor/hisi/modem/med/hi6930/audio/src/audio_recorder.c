

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "audio_recorder.h"
#include "codec_com_codec.h"
#include "voice_proc.h"
#include "om_log.h"
#include "voice_mc.h"
#include "audio_debug.h"
#include "audio_enhance.h"
#include "xa_src_pp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    ��ά�ɲ���Ϣ�а�����C�ļ���ź궨��
*****************************************************************************/
/*lint -e(767)*/
#define THIS_FILE_ID                    OM_FILE_ID_AUDIO_RECORDER_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
AUDIO_RECORDER_OBJ_STRU                 g_stAudioRecorderObj;

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_UINT32  AUDIO_RECOEDER_Init(VOS_VOID)
{
    AUDIO_RECORDER_OBJ_STRU         *pstAudioRecorderObj = AUDIO_RecorderGetObjPtr();
    AUDIO_RECORDER_VOICE_STRU       *pstVoiceRecord      = &pstAudioRecorderObj->stVoiceRecord;

    /* ��ʼ�� */
    UCOM_MemSet(pstAudioRecorderObj, 0, sizeof(AUDIO_RECORDER_OBJ_STRU));

    /* ��ʽ���¼������Ϊnull */
    AUDIO_RecorderSetCaptureType(AUDIO_RECORD_TYPE_DISABLE);

    /* ��ʽ��ʼ��CSͨ��¼�����ü��� */
    pstVoiceRecord->uwRefCnt = 0;

    /* ����CSͨ��¼��buff������ */
    pstAudioRecorderObj->uwProtectWord1 = UCOM_PROTECT_WORD1;
    pstAudioRecorderObj->uwProtectWord2 = UCOM_PROTECT_WORD2;
    pstAudioRecorderObj->uwProtectWord3 = UCOM_PROTECT_WORD3;
    pstAudioRecorderObj->uwProtectWord4 = UCOM_PROTECT_WORD4;

    pstVoiceRecord->stRecordBuff.uwProtectWord1 = UCOM_PROTECT_WORD1;
    pstVoiceRecord->stRecordBuff.uwProtectWord2 = UCOM_PROTECT_WORD2;
    pstVoiceRecord->stRecordBuff.uwProtectWord3 = UCOM_PROTECT_WORD3;
    pstVoiceRecord->stRecordBuff.uwProtectWord4 = UCOM_PROTECT_WORD4;
    pstVoiceRecord->stRecordBuff.uwProtectWord5 = UCOM_PROTECT_WORD4;

    return VOS_OK;
}


VOS_UINT32 AUDIO_RECORDER_MsgRecordStartCmd(VOS_VOID *pvOsaMsg)
{
    AUDIO_RECORDER_OBJ_STRU         *pstAudioRecorderObj    = AUDIO_RecorderGetObjPtr();
    AUDIO_RECORD_START_STRU         *pstRecStartMsg         = VOS_NULL ;
    VOS_UINT32                       uwRet                  = VOS_OK;

    /* �쳣��μ�� */
    if (VOS_NULL == pvOsaMsg)
    {
        OM_LogError(AUDIO_RecorderBadPara);

        return VOS_ERR;
    }

    /* ��AUDIO_RECORD_START_STRU��ʽ������Ϣ */
    pstRecStartMsg = (AUDIO_RECORD_START_STRU *)((MsgBlock *) pvOsaMsg)->aucValue;

    switch(pstRecStartMsg->enRecordType)
    {
        case AUDIO_RECORD_TYPE_CALL:
        {
            uwRet += AUDIO_RECORDER_VoiceRcdStart(pstRecStartMsg);
        }
        break;
        case AUDIO_RECORD_TYPE_3RD_MODEM:
        {
            AUDIO_RecorderSetCaptureType(AUDIO_RECORD_TYPE_3RD_MODEM);

            /* ��������ȡ�Ե�����Modem */
            uwRet += AUDIO_PCM_RegisterPcmCbFunc(AUDIO_PCM_MODE_CAPTURE,
                        AUDIO_PCM_OBJ_AP,
                        AUDIO_CAPTURE_MODE_EXT_CALL,
                        AUDIO_RECORDER_SecondVoiceCallBack);

            /* ���ü�����һ */
            pstAudioRecorderObj->st3RDVoiceRecord.uwRefCnt++;
        }
        break;
        case AUDIO_RECORD_TYPE_AUDIO:
        {
            AUDIO_RecorderSetCaptureType(AUDIO_RECORD_TYPE_AUDIO);

            /* ע����Ƶ¼������Ļص����� */
            uwRet += AUDIO_PCM_RegisterPcmCbFunc(AUDIO_PCM_MODE_CAPTURE,
                            AUDIO_PCM_OBJ_AP,
                            AUDIO_CAPTURE_MODE_AUDIO,
                            AUDIO_RECORDER_AudioCallBack);

            /* ���ü�����һ */
            pstAudioRecorderObj->stAudioRecord.uwRefCnt++;
        }
        break;
        case AUDIO_RECORD_TYPE_FM:
        {
            AUDIO_RecorderSetCaptureType(AUDIO_RECORD_TYPE_FM);

            /* ע��FM¼������Ļص����� */
            uwRet += AUDIO_PCM_RegisterPcmCbFunc(AUDIO_PCM_MODE_CAPTURE,
                            AUDIO_PCM_OBJ_AP,
                            AUDIO_CAPTURE_MODE_FM,
                            AUDIO_RECORDER_FMCallBack);

            /* ���ü�����һ */
            pstAudioRecorderObj->stFMRecord.uwRefCnt++;
        }
        break;
        default:
        {
            AUDIO_RecorderSetCaptureType(AUDIO_RECORD_TYPE_DISABLE);

            OM_LogError1(AUDIO_RECORDER_MsgStartErr, pstRecStartMsg->enRecordType);

            return VOS_ERR;
        }
    }

    if(VOS_OK != uwRet)
    {
        AUDIO_RecorderSetCaptureType(AUDIO_RECORD_TYPE_DISABLE);

        OM_LogError1(AUDIO_RECORDER_MsgStartErr, uwRet);
    }

    return uwRet;

}


VOS_UINT32 AUDIO_RECORDER_MsgRecordStopCmd(VOS_VOID *pvOsaMsg)
{
    AUDIO_RECORDER_OBJ_STRU         *pstAudioRecorderObj = AUDIO_RecorderGetObjPtr();
    AUDIO_RECORD_STOP_STRU          *pstStopMsg;
    VOS_UINT32                       uwRet = VOS_OK;

    /* �쳣��μ�� */
    if (VOS_NULL == pvOsaMsg)
    {
        OM_LogError(AUDIO_RecorderBadPara);

        return VOS_ERR;
    }

    /* ��AUDIO_RECORD_STOP_STRU��ʽ������Ϣ */
    pstStopMsg = (AUDIO_RECORD_STOP_STRU *)((MsgBlock *) pvOsaMsg)->aucValue;

    switch( pstStopMsg->enRecordType)
    {
        case AUDIO_RECORD_TYPE_CALL:
        {
            AUDIO_RECORDER_VoiceRcdStop();
        }
        break;
        case AUDIO_RECORD_TYPE_3RD_MODEM:
        {
            /* Ԥ���ӿڣ������� */

            /* ���ü�����һ */
            pstAudioRecorderObj->st3RDVoiceRecord.uwRefCnt--;
        }
        break;
        case AUDIO_RECORD_TYPE_AUDIO:
        {
            /* Ԥ���ӿڣ������� */

            /* ���ü�����һ */
            pstAudioRecorderObj->stAudioRecord.uwRefCnt--;
        }
        break;
        case AUDIO_RECORD_TYPE_FM:
        {
            /* Ԥ���ӿڣ������� */

            /* ���ü�����һ */
            pstAudioRecorderObj->stFMRecord.uwRefCnt--;
        }
        break;
        default:
        {
            OM_LogError1(AUDIO_RECORDER_MsgStartErr, pstStopMsg->enRecordType);

            return VOS_ERR;
        }
    }

    return uwRet;
}


VOS_UINT32  AUDIO_RECORDER_VoiceRcdStart(AUDIO_RECORD_START_STRU *pstRecStartMsg)
{
    AUDIO_RECORDER_OBJ_STRU         *pstAudioRecorderObj    = AUDIO_RecorderGetObjPtr();
    VOS_UINT32                       uwRet = VOS_OK;

    /* ���voiceû�����У����ϱ����� */
    if ((VOS_OK != VOICE_McApiIsIdle()))
    {
        /* ��־ͨ��¼����ʼ���˺��轫ap���ṩ��¼��buff���Ϊhifi��������֮������� */
        AUDIO_RecorderSetCaptureType(AUDIO_RECORD_TYPE_CALL);

        /* ����Ŀ������� */
        pstAudioRecorderObj->stVoiceRecord.uwTargetSmpRate = pstRecStartMsg->uhwTarSmpRate;

        /* ע��voice_mcͨ��¼�������� */
        VOICE_McRegisterCaptureCallBack(AUDIO_RECORDER_VoiceVcMcCB);

        /* ע��audio_recorderͨ��¼�������� */
        uwRet += AUDIO_PCM_RegisterPcmCbFunc(AUDIO_PCM_MODE_CAPTURE,
                                            AUDIO_PCM_OBJ_HIFI,
                                            AUDIO_CAPTURE_MODE_CALL,
                                            AUDIO_RECORDER_VoiceAudioPcmCB);

        /* ���ü�����һ */
        pstAudioRecorderObj->stVoiceRecord.uwRefCnt++;

        /* om��Ϣ */
        OM_LogInfo3(AUDIO_RECORDER_RegisterVcCbOK,
                    pstAudioRecorderObj->stVoiceRecord.uwRefCnt,
                    AUDIO_RecorderGetCaptureType(),
                    uwRet);
    }
    else
    {
        /* om��Ϣ */
        OM_LogError(AUDIO_RECORDER_StartVcCallWhileIdle);

        return VOS_ERR;
    }

    return uwRet;

}


VOS_UINT32  AUDIO_RECORDER_VoiceRcdStop(VOS_VOID)
{
    AUDIO_RECORDER_OBJ_STRU         *pstAudioRecorderObj    = AUDIO_RecorderGetObjPtr();
    AUDIO_RECORDER_VOICE_STRU       *pstVoiceRecord         = &pstAudioRecorderObj->stVoiceRecord;
    VOS_UINT32                       uwRet = VOS_OK;

    /* ȥע��ͨ��¼�������� */
    VOICE_McUnRegisterCaptureCallBack();

    uwRet += AUDIO_PCM_RegisterPcmCbFunc(AUDIO_PCM_MODE_CAPTURE,
                                         AUDIO_PCM_OBJ_HIFI,
                                         AUDIO_CAPTURE_MODE_CALL,
                                         VOS_NULL);

    /* ��ǵ�ǰ¼����������Ϊdisable */
    AUDIO_RecorderSetCaptureType(AUDIO_RECORD_TYPE_DISABLE);

    /* ���ü�����һ */
    pstVoiceRecord->uwRefCnt--;
    pstVoiceRecord->uwOrignalSmpRate = 0;
    pstVoiceRecord->uwTargetSmpRate  = 0;

    /* ��ͨ��¼��׼�����buff��գ���ֹ�������� */
    UCOM_MemSet(pstVoiceRecord->stRecordBuff.ashwTxDataOut,
                0,
                AUDIO_RECORDER_BUFF_SIZE_1920 * sizeof(VOS_INT16));

    UCOM_MemSet(pstVoiceRecord->stRecordBuff.ashwRxDataOut,
                0,
                AUDIO_RECORDER_BUFF_SIZE_1920 * sizeof(VOS_INT16));

    /* om��Ϣ */
    OM_LogInfo1(AUDIO_RECORDER_UnregisterVcCbOK,
                pstAudioRecorderObj->stVoiceRecord.uwRefCnt);

    return uwRet;

}
VOS_UINT32  AUDIO_RECORDER_VoiceVcMcCB(
                VOS_INT16 *pshwTx,
                VOS_INT16 *pshwRx,
                VOS_UINT32 uwOrignalSmpRate)
{
    AUDIO_RECORDER_OBJ_STRU             *pstAudioRcdObj;
    AUDIO_RECORDER_VOICE_STRU           *pstVoiceRcdObj;
    XA_SRC_USER_CFG_STRU                 stVoiceSmpRate;
    VOS_UINT32                           uwBuffSize;
    VOS_UINT32                           uwRet = VOS_OK;

    /* ����ӿڣ��쳣��μ�� */
    if (  (VOS_NULL == pshwTx)
        ||(VOS_NULL == pshwRx)
        ||(AUDIO_RECORDER_SAMPLERATE_16K < uwOrignalSmpRate))
    {
        OM_LogError3(AUDIO_RecorderBadPara,
                     pshwTx,
                     pshwRx,
                     uwOrignalSmpRate);

        return VOS_ERR;
    }

    /* ��ȡ¼������ */
    pstAudioRcdObj = AUDIO_RecorderGetObjPtr();
    pstVoiceRcdObj = &pstAudioRcdObj->stVoiceRecord;

    /* �ɲ����ʻ�ȡ���ݳ��� */
    if (AUDIO_RECORDER_SAMPLERATE_8K == uwOrignalSmpRate)
    {
        uwBuffSize = AUDIO_RECORDER_BUFF_SIZE_320;
    }
    else if (AUDIO_RECORDER_SAMPLERATE_16K == uwOrignalSmpRate)
    {
        uwBuffSize = AUDIO_RECORDER_BUFF_SIZE_640;
    }
    else
    {
        OM_LogError1(AUDIO_RecorderBadPara, uwOrignalSmpRate);

        return VOS_ERR;
    }

    /* ��ȡ��Ӧbuff���� */
    UCOM_MemCpy(pstVoiceRcdObj->stRecordBuff.ashwTxDataIn,
                pshwTx,
                uwBuffSize);

    UCOM_MemCpy(pstVoiceRcdObj->stRecordBuff.ashwRxDataIn,
                pshwRx,
                uwBuffSize);

    /* ����״γ�ʼ������������խ���л��������³�ʼ�������ģ�� */
    if (pstVoiceRcdObj->uwOrignalSmpRate != uwOrignalSmpRate)
    {
        OM_LogInfo2(AUDIO_RecorderSmpRateChange,
                    pstVoiceRcdObj->uwOrignalSmpRate,
                    uwOrignalSmpRate);

        stVoiceSmpRate.swChannels           = 1;
        stVoiceSmpRate.swInputChunkSize     = XA_SRC_INPUT_CHUNKSIZE_160;
        stVoiceSmpRate.swInputSmpRat        = (VOS_INT32)uwOrignalSmpRate;
        stVoiceSmpRate.swOutputSmpRat       = (VOS_INT32)pstVoiceRcdObj->uwTargetSmpRate;

        /* ��ʼ�������ģ�� */
        uwRet += (VOS_UINT32)XA_SRC_Init(XA_SRC_VOICE_RECORD_TX, &stVoiceSmpRate);
        uwRet += (VOS_UINT32)XA_SRC_Init(XA_SRC_VOICE_RECORD_RX, &stVoiceSmpRate);

        /* ���¼�¼ԭʼ�������ݲ����� */
        pstVoiceRcdObj->uwOrignalSmpRate = uwOrignalSmpRate;

    }

    /* �����ʼ��ʧ�ܣ��ϱ�om��Ϣ */
    if (VOS_OK != uwRet)
    {
        OM_LogError1(AUDIO_RecorderInitSrcFailed, uwRet);
    }

    return uwRet;
}



VOS_VOID AUDIO_RECORDER_VoiceAudioPcmCB(
                VOS_VOID  *pshwBufOut,
                VOS_VOID  *pshwBufIn,
                VOS_UINT32 uwTarSmpRate,
                VOS_UINT32 uwChannelNum,
                VOS_UINT32 uwTarBufSize)
{
    AUDIO_RECORDER_OBJ_STRU             *pstAudioRcdObj;
    AUDIO_RECORDER_VOICE_STRU           *pstVoiceRcdObj;
    VOS_UINT32                           uwOutputSize;
    VOS_UINT32                           uwProcTimes;

    /* �쳣��μ�� */
    if (  (VOS_NULL == pshwBufOut)
        ||(VOS_NULL == pshwBufIn) )
    {
        OM_LogError(AUDIO_RecorderBadPara);
        return;
    }

    /* ��ȡ¼������ */
    pstAudioRcdObj = AUDIO_RecorderGetObjPtr();
    pstVoiceRcdObj = &pstAudioRcdObj->stVoiceRecord;

    /* ������audio SIO�ڰ��˹��������ݶ�����ʹ�þ������봦��������� */
    /* ���������봦�����������ͳһ�������48K */
    if (AUDIO_RECORDER_SAMPLERATE_8K == pstVoiceRcdObj->uwOrignalSmpRate)
    {
        /* �����ݱ������48K,ÿ�δ���160���㣬����һ�� */
        uwProcTimes = 1;
    }
    else if (AUDIO_RECORDER_SAMPLERATE_16K == pstVoiceRcdObj->uwOrignalSmpRate)
    {
        /* �����ݱ������48K,ÿ�δ���160���㣬����һ�� */
        uwProcTimes = 2;
    }
    else
    {
        OM_LogError1(AUDIO_RecorderSmpRateErr, pstVoiceRcdObj->uwOrignalSmpRate);
        return;
    }

    /* ���������ݱ������48K */
    XA_SRC_Proc8x(XA_SRC_VOICE_RECORD_TX,
                  pstVoiceRcdObj->stRecordBuff.ashwTxDataIn,
                  pstVoiceRcdObj->stRecordBuff.ashwTxDataOut,
                  &uwOutputSize,
                  uwProcTimes);

    /* ���������ݱ������48K */
    XA_SRC_Proc8x(XA_SRC_VOICE_RECORD_RX,
                  pstVoiceRcdObj->stRecordBuff.ashwRxDataIn,
                  pstVoiceRcdObj->stRecordBuff.ashwRxDataOut,
                  &uwOutputSize,
                  uwProcTimes);

    /* �����������ϲ���TX�����������������洢��RX�����������������洢 */
    UCOM_COMM_SetChnBit16(
                pshwBufOut,
                2,
                pstVoiceRcdObj->stRecordBuff.ashwTxDataOut,
                AUDIO_RECORDER_BUFF_SIZE_1920,
                0);

    UCOM_COMM_SetChnBit16(
                pshwBufOut,
                2,
                pstVoiceRcdObj->stRecordBuff.ashwRxDataOut,
                AUDIO_RECORDER_BUFF_SIZE_1920,
                1);

}
VOS_VOID AUDIO_RECORDER_AudioCallBack(
                VOS_VOID  *pshwBufOut,
                VOS_VOID  *pshwBufIn,
                VOS_UINT32 uwSampleRate,
                VOS_UINT32 uwChannelNum,
                VOS_UINT32 uwBufSize)
{

    /*͸����֮�����õ�Mic����*/
    if ((NULL == pshwBufIn) || (NULL == pshwBufOut))
    {
        OM_LogWarning(AUDIO_RECORDER_BuffNullErr);
        return;
    }

    /* �������в����˲� */
    AUDIO_ENHANCE_ProcMicIn(
                pshwBufOut,
                pshwBufIn,
                uwSampleRate,
                uwChannelNum,
                uwBufSize);
}


VOS_VOID AUDIO_RECORDER_FMCallBack(
                VOS_VOID  *pshwBufOut,
                VOS_VOID  *pshwBufIn,
                VOS_UINT32 uwSampleRate,
                VOS_UINT32 uwChannelNum,
                VOS_UINT32 uwBufSize)
{
    /*͸��*/
    if ((NULL == pshwBufIn) || (NULL == pshwBufOut))
    {
        OM_LogWarning(AUDIO_RECORDER_BuffNullErr);
        return;
    }

    UCOM_MemCpy(pshwBufOut, pshwBufIn, uwBufSize);
}
VOS_VOID AUDIO_RECORDER_SecondVoiceCallBack(
                VOS_VOID  *pshwBufOut,
                VOS_VOID  *pshwBufIn,
                VOS_UINT32 uwSampleRate,
                VOS_UINT32 uwChannelNum,
                VOS_UINT32 uwBufSize)
{
    /*͸��*/
    if ((NULL == pshwBufIn) || (NULL == pshwBufOut))
    {
        OM_LogWarning(AUDIO_RECORDER_BuffNullErr);
        return;
    }

    UCOM_MemCpy(pshwBufOut, pshwBufIn, uwBufSize);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif





