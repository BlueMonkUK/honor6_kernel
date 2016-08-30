

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "xa_aac_dec.h"
#include "ucom_mem_dyn.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    ��ά�ɲ���Ϣ�а�����C�ļ���ź궨��
*****************************************************************************/
/*lint -e(767)*/
#define THIS_FILE_ID                      OM_FILE_ID_XA_AAC_DEC_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
XA_COMM_MEMORY_CFG_STRU                   g_stAacDecMemCfg;       /* AAC�����ڴ����ù��� */
AAC_DEC_CONFIG_PARAMS_STRU                g_stAacDecParamCfg;     /* AAC����������ù��� */
AAC_DEC_PARAMS_STRU                       g_stAacParams;          /* AAC�������� */

/* AAC�����APIָ�� */
xa_codec_handle_t                         g_pstAacApiObj;

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_UINT32  AAC_DECODER_SetParams(AAC_DEC_CONFIG_PARAMS_STRU *pstParamCfg)
{
    VOS_INT32            swErrCode;
    xa_codec_func_t     *pfuncAacProcApi = xa_aac_dec;
    xa_codec_handle_t    pstAacApiObj    = XA_AACDEC_GetApiObjPtr();

    /* 0������ж� */
    if (VOS_NULL == pstParamCfg)
    {
        return VOS_ERR;
    }

    /* 1�������Ƿ�����̶�����Ϊ������ */
    swErrCode = (*pfuncAacProcApi)(pstAacApiObj,
                                    XA_API_CMD_SET_CONFIG_PARAM,
                                    XA_AACDEC_CONFIG_PARAM_TO_STEREO,
                                    (VOS_VOID *)&(pstParamCfg->swForceOutputStereo));
    /* ���uwErrCode����ֵ */
    XA_ERROR_HANDLE(swErrCode);

    /* 2������PCMλ�� */
    swErrCode = (*pfuncAacProcApi)(pstAacApiObj,
                                    XA_API_CMD_SET_CONFIG_PARAM,
                                    XA_AACDEC_CONFIG_PARAM_PCM_WDSZ,
                                    (VOS_VOID *)&(pstParamCfg->swPcmWidthSize));
    /* ���uwErrCode����ֵ */
    XA_ERROR_HANDLE(swErrCode);

    /* 3������������������ */
    /*swErrCode = (*pfuncAacProcApi)(pstAacApiObj,
                                    XA_API_CMD_SET_CONFIG_PARAM,
                                    XA_AACDEC_CONFIG_PARAM_OUTNCHANS,
                                    (VOS_VOID *)&(pstParamCfg->swMaxOutChanNum));*/
    /* ���uwErrCode����ֵ */
    //XA_ERROR_HANDLE(swErrCode);

    return VOS_OK;
}
VOS_UINT32  AAC_DECODER_GetParams(AAC_DEC_PARAMS_STRU *pstConfig)
{
    VOS_INT32            swErrCode;
    xa_codec_func_t     *pfuncAacProcApi  = xa_aac_dec;
    xa_codec_handle_t    pstAacApiObj     = XA_AACDEC_GetApiObjPtr();

    /* 0������ж� */
    if (VOS_NULL == pstConfig)
    {
        return VOS_ERR;
    }

    /* 1������XA�������ֱ��ȡ:*/
    /* ����AAC��ʽ */
    swErrCode = (*pfuncAacProcApi )(pstAacApiObj,
                                    XA_API_CMD_GET_CONFIG_PARAM,
                                    XA_AACDEC_CONFIG_PARAM_AAC_FORMAT,
                                   (void *)&(pstConfig->enAacFormat));

    XA_ERROR_HANDLE(swErrCode);

    /* ������ */
    swErrCode = (*pfuncAacProcApi )(pstAacApiObj,
                                    XA_API_CMD_GET_CONFIG_PARAM,
                                    XA_AACDEC_CONFIG_PARAM_NUM_CHANNELS,
                                   (VOS_VOID *)&(pstConfig->swNumChannels));

    XA_ERROR_HANDLE(swErrCode);

    /* ������ģʽ */
    swErrCode = (*pfuncAacProcApi )(pstAacApiObj,
                                    XA_API_CMD_GET_CONFIG_PARAM,
                                    XA_AACDEC_CONFIG_PARAM_ACMOD,
                                   (VOS_VOID *)&(pstConfig->enAcmod));

    XA_ERROR_HANDLE(swErrCode);

    /* ������ */
    swErrCode = (*pfuncAacProcApi )(pstAacApiObj,
                                    XA_API_CMD_GET_CONFIG_PARAM,
                                    XA_AACDEC_CONFIG_PARAM_SAMP_FREQ,
                                   (VOS_VOID *)&(pstConfig->swSampFreq));

    XA_ERROR_HANDLE(swErrCode);

    /* SBR���� */
    swErrCode = (*pfuncAacProcApi )(pstAacApiObj,
                                    XA_API_CMD_GET_CONFIG_PARAM,
                                    XA_AACDEC_CONFIG_PARAM_SBR_TYPE,
                                   (VOS_VOID *)&(pstConfig->swSbrType));

    XA_ERROR_HANDLE(swErrCode);

    /* AAC�������� */
    swErrCode = (*pfuncAacProcApi )(pstAacApiObj,
                                    XA_API_CMD_GET_CONFIG_PARAM,
                                    XA_AACDEC_CONFIG_PARAM_AAC_SAMPLERATE,
                                   (VOS_VOID *)&(pstConfig->swAacRate));

    XA_ERROR_HANDLE(swErrCode);

    /* ͨ��ӳ��� */
    swErrCode = (*pfuncAacProcApi )(pstAacApiObj,
                                    XA_API_CMD_GET_CONFIG_PARAM,
                                    XA_AACDEC_CONFIG_PARAM_CHANMAP,
                                   (VOS_VOID *)&(pstConfig->swChanMap));

    XA_ERROR_HANDLE(swErrCode);

    /* ������������������ */
    swErrCode = (*pfuncAacProcApi )(pstAacApiObj,
                                    XA_API_CMD_GET_CONFIG_PARAM,
                                    XA_AACDEC_CONFIG_PARAM_OUTNCHANS,
                                   (VOS_VOID *)&(pstConfig->swOutNChans));

    XA_ERROR_HANDLE(swErrCode);

    /* 2��ͨ��SBR��Ϣ����ȡAAC LIB FORMAT */
    if (XA_AACDEC_SBR_TYPE_2 == pstConfig->swSbrType)
    {
        pstConfig->enAacLibFormat = XA_AACDEC_EBITSTREAM_FORMAT_AAC_PLUS_V2;
    }
    else if (XA_AACDEC_SBR_TYPE_1 == pstConfig->swSbrType)
    {
        pstConfig->enAacLibFormat = XA_AACDEC_EBITSTREAM_FORMAT_AAC_PLUS;
    }
    else
    {
        pstConfig->enAacLibFormat = XA_AACDEC_EBITSTREAM_FORMAT_PLAIN_AAC;
    }

    /* 3����ǰ���ֻ֧��2��������Ҫǿ���޸�CHANNEL MAP��ֵ */
    //AAC_DECODER_SetOutChanFixedTo2(&pstConfig->swChanMap);

    return VOS_OK;
}



VOS_UINT32  AAC_DECODER_Init(XA_AUDIO_DEC_USER_CFG_STRU *pstUserCfg)
{
    VOS_UINT32                  uwErrCode;
    VOS_INT32                   swErrCode;
    xa_codec_func_t            *pfuncAacProcApi  = xa_aac_dec;                  /* AAC ������̹���API */
    AAC_DEC_CONFIG_PARAMS_STRU *pstParamCfg      = XA_AACDEC_GetParaCfgPtr(); /* AAC ����������ù��� */
    xa_codec_handle_t           pstAacApiObj;

    XA_COMM_MEMORY_CFG_STRU    *pstMemCfg        = XA_AACDEC_GetMemCfgPtr();

    /* ��ȡpstUserCfg������: λ���ļ�ͷָ�룬�ļ�ͷ���� */
    VOS_INT32                   swPcmWidthSize;

    /* 1���ж���εĺϷ��� */
    if (VOS_NULL == pstUserCfg)
    {
        return VOS_ERR;
    }

    /* ��ȡpstUserCfg������: λ���ļ�ͷָ�룬�ļ�ͷ���� */
    swPcmWidthSize              = (VOS_INT32)pstUserCfg->uwPcmWidthSize;
    pstMemCfg->pHeaderBuff      = pstUserCfg->pvHeadBuff;
    pstMemCfg->swHeaderMemSize  = pstUserCfg->swHeaderMemSize;

    /* 2�����ú���XA_COMM_Startup����APIָ��ָ���API������г�ʼ�� */
    swErrCode = XA_COMM_Startup(pfuncAacProcApi, &(XA_AACDEC_GetApiObjPtr()), pstMemCfg);

    /* 3����麯��XA_COMM_Startup����ֵ�Ƿ��쳣�������쳣��麯��XA_ERROR_HANDLE���м�顣*/
    XA_ERROR_HANDLE(swErrCode);

    pstAacApiObj     = XA_AACDEC_GetApiObjPtr();

    /*4.1 ��ʼ���ṹ�壬ע:AAC����Լ�����£�
    --�������Ϊ��������������̶�����Ϊ��������
    --������ͨ�����̶�Ϊ2��
    --PCM��ȹ̶�Ϊ16bit��
    --ֻ֧��ADTS��ADIF��ʽ��AAC����*/

    /*�����Ƿ��Ƶ�����Ϊ������ */
    pstParamCfg->swForceOutputStereo       = XA_AACDEC_SET_MONO2STEREO_ENABLE;
    /* ����PCMλ�� */
    pstParamCfg->swPcmWidthSize            = swPcmWidthSize;
    /* ����������ͨ���� */
    pstParamCfg->swMaxOutChanNum           = XA_AACDEC_SET_MAXOUTCHAN_2;

    /* 4.2 ���ú���AAC_DECODER_SetParams(pstParamCfg)�����в������ã�*/
    uwErrCode = AAC_DECODER_SetParams(pstParamCfg);

    /* 5����麯��AAC_DECODER_SetParams����ֵ�Ƿ��쳣 */
    if (VOS_OK != uwErrCode)
    {
        return VOS_ERR;
    }

    /* 6�����ú���XA_COMM_InitMemoryAlloc()������ָ��󶨡��ڴ����롢�ռ����Ͷ��������*/
    swErrCode = XA_COMM_MemoryAlloc(pfuncAacProcApi,
                                    pstAacApiObj,
                                    pstMemCfg);

    /* 7����麯��XA_COMM_InitMemoryAlloc����ֵ�Ƿ��쳣�������쳣��麯��XA_ERROR_HANDLE���м�飻*/
    XA_ERROR_HANDLE(swErrCode);

    /* 8��tensilica�����ִ�г�ʼ������׶Σ���ȡ�ļ�ͷ��ɳ�ʼ�� */
    swErrCode = XA_COMM_Init(pfuncAacProcApi,
                            pstAacApiObj,
                            pstMemCfg->pInputBuff,
                            pstMemCfg->swInputMemSize,
                            pstMemCfg->pHeaderBuff,
                            pstMemCfg->swHeaderMemSize,
                           &pstUserCfg->swConsumed);

    XA_ERROR_HANDLE(swErrCode);

    pstUserCfg->swInSize = pstMemCfg->swInputMemSize;

    return VOS_OK;
}



VOS_UINT32 AAC_DECODER_Decode(XA_AUDIO_DEC_USER_CFG_STRU *pstUserCfg)
{
    VOS_INT32                  swErrCode;
    VOS_INT32                  swDecodeFinish   = CODEC_DECODE_UNFINISH_FLAG;
    xa_codec_func_t           *pfuncAacProcApi  = xa_aac_dec;
    xa_codec_handle_t          pstAacApiObj     = XA_AACDEC_GetApiObjPtr();
    XA_COMM_MEMORY_CFG_STRU   *pstAacMemCfg     = XA_AACDEC_GetMemCfgPtr();

    /* for input */
    VOS_INT32                  swInputSize;
    VOS_UINT32                 uwPcmWidthSize;
    VOS_UINT32                 enIsEndOfStream;

    /* for output */
    VOS_INT32                  swConsumeSize   = 0;
    VOS_UINT32                 uwDecoderState  = CODEC_STATE_INITIALIZED;

    /* 1����μ�� */
    if(VOS_NULL == pstUserCfg)
    {
        return VOS_ERR;
    }

    swInputSize                 = pstUserCfg->swInSize;
    uwPcmWidthSize              = pstUserCfg->uwPcmWidthSize;
    enIsEndOfStream             = pstUserCfg->enIsEndOfStream;


    if (XA_AACDEC_SET_PCMWIDTH_16 != uwPcmWidthSize)
    {
        return VOS_ERR;
    }

    /* for input */
    UCOM_MemCpy(pstAacMemCfg->pInputBuff, pstUserCfg->pvInBuff, (VOS_UINT32)pstAacMemCfg->swInputMemSize);

    /* 2������XA��������AAC���н��� */
    swErrCode = XA_COMM_Decode(pfuncAacProcApi,
                               pstAacApiObj,
                               swInputSize,
                               enIsEndOfStream,
                               &pstAacMemCfg->swOutputMemSize,
                               &swConsumeSize,
                               &swDecodeFinish);

    /* 3�������� */
    XA_ERROR_HANDLE_WITH_STATUS_CODE(swErrCode, &uwDecoderState);

    /* 4���ж��Ƿ������� */
    if(CODEC_DECODE_FINISH_FLAG == swDecodeFinish)
    {
        uwDecoderState = CODEC_STATE_FINISH;
    }

    /* 5�����BUFF������ת�� */
    /* ����
       pstAacDecParams->swChanMap;       ͨ��ӳ���
       pstAacDecParams->swOutNChans;     ������������������
       ����Ϣ�������������ѭ��д�����ݣ���дunused channels
       ����DEMO�е�PCM_write������������ת����ֻ��Ҫ֧��16bitλ�� */
    if (VOS_OK != AAC_DECODER_PcmDataConvert(pstAacMemCfg->pOutputBuff,
                                           XA_AACDEC_SET_PCMWIDTH_16,
                                            pstAacMemCfg->swOutputMemSize,
                                           (VOS_INT16 *)pstUserCfg->pvOutBuff,
                                           &(pstUserCfg->swOutputSize)))
    {
        return VOS_ERR;
    }

    /* 6��������������ݽ���pstUserCfg */
    pstUserCfg->swConsumed     = swConsumeSize;
    pstUserCfg->uwDecoderState = uwDecoderState;

    return VOS_OK;
}
VOS_UINT32  AAC_DECODER_Seek(
                VOS_UINT32                    uwSkipSize,
                CODEC_SEEK_DERECT_ENUM_UINT16 enSeekDirect)
{
    VOS_INT32    swErrCode;
    VOS_UINT32   uwCurrStreamPos;
    VOS_UINT32   uwNewStreamPos;
    xa_codec_func_t *pstAacProcessApi = xa_aac_dec;

    /* ��ѯ��ǰ����λ�� */
    swErrCode = (*pstAacProcessApi)(g_pstAacApiObj,
                          XA_API_CMD_GET_CONFIG_PARAM,
                          XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS,
                          &uwCurrStreamPos);
    XA_ERROR_HANDLE(swErrCode);

    /* ����Ҫ�ƶ����Ĳ���λ�� */
    if(CODEC_SEEK_FORWARDS == enSeekDirect)
    {
        /* �����λ���Ƿ񳬹����ļ���С����Ҫ֪���ļ���С */
        uwNewStreamPos = uwCurrStreamPos + uwSkipSize;
    }
    else if (CODEC_SEEK_BACKWARDS == enSeekDirect)
    {
        if(uwSkipSize > uwCurrStreamPos)
        {
            return VOS_ERR;
        }
        uwNewStreamPos = uwCurrStreamPos - uwSkipSize;
    }
    else
    {
        return VOS_ERR;
    }

    /* �����µĲ���λ�� */
    swErrCode = (*pstAacProcessApi)(g_pstAacApiObj,
                          XA_API_CMD_SET_CONFIG_PARAM,
                          XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS,
                          &uwNewStreamPos);
    XA_ERROR_HANDLE(swErrCode);

    swErrCode = (*pstAacProcessApi)(g_pstAacApiObj,
                          XA_API_CMD_EXECUTE,
                          XA_CMD_TYPE_DO_RUNTIME_INIT,
                          NULL);
    XA_ERROR_HANDLE(swErrCode);

    return VOS_OK;
}


VOS_VOID AAC_DECODER_Close(VOS_VOID)
{
    XA_COMM_MEMORY_CFG_STRU    *pstMemCfg    = XA_AACDEC_GetMemCfgPtr();

    if (VOS_NULL != pstMemCfg)
    {
        XA_COMM_MemFree(pstMemCfg);
    }

    /* API����ָ����ڴ��Ѿ��������ͷţ���������API����ָ�룬��ֹ����Ұָ�� */
    XA_AACDEC_GetApiObjPtr()                 = VOS_NULL;
}
VOS_INT32 AAC_DECODER_GetCurrDecPos(VOS_UINT32 *puwPosInBytes)
{
    VOS_INT32            swErrCode;
    xa_codec_func_t     *pfuncAacProcApi  = xa_aac_dec;
    xa_codec_handle_t    pstAacApiObj     = XA_AACDEC_GetApiObjPtr();

    /* 1�����ÿ⺯����ѯ��ǰAAC���Ž��� */
    swErrCode = (*pfuncAacProcApi )(pstAacApiObj,
                                    XA_API_CMD_GET_CONFIG_PARAM,
                                    XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS,
                                    puwPosInBytes);

    /* 2����麯���ķ���ֵ�Ƿ��쳣�������쳣��麯��XA_ERROR_HANDLE���м�飻*/
    XA_ERROR_HANDLE(swErrCode);

    return VOS_OK;
}


VOS_UINT32 AAC_DECODER_PcmDataConvert
                (VOS_INT32       *pswTimeCh,
                 VOS_INT32        swPcmSampleSize,
                 VOS_INT32        swOutputSize,       /* AUDIO_DEC_MEMORY_CFG_STRU�е���ʱ���Buff�ĳ��� */
                 VOS_INT16       *pshwOutBuff,
                 VOS_INT32       *pswTotalOutSize)    /* ��������BUFF�ĳ��� */
{
    AAC_DEC_PARAMS_STRU        *pstParaCfg  = XA_AACDEC_GetParamsPtr();
    VOS_INT32                   swI;
    VOS_INT32                   swCh;
    VOS_INT32                   swChmap;
    VOS_INT32                   swOutNChans;
    VOS_INT32                   swOutchmap;
    VOS_INT32                   swFrameSize;
    VOS_INT16                  *pshwTimeChShort;
    VOS_INT16                  *pshwTmpOut  = pshwOutBuff;

    AAC_DECODER_GetParams(pstParaCfg);
    swOutNChans                 = pstParaCfg->swOutNChans;
    swOutchmap                  = pstParaCfg->swChanMap;
    swFrameSize                 = (swOutputSize / 2) / swOutNChans;
    *pswTotalOutSize            = 0;

    if(swPcmSampleSize != XA_AACDEC_SET_PCMWIDTH_16)
    {
        return VOS_ERR;
    }
    else
    {
        pshwTimeChShort = (VOS_INT16 *)pswTimeCh;
        for(swI = 0; swI < swFrameSize; swI++)
        {
            swChmap = swOutchmap;
            for(swCh = 0 ; swCh < XA_AACDEC_MAXCHAN_NUMBER; swCh++)
            {
                /* Check if any channel data is present at this sample offset */
                if ((swChmap & 0x0f) == 0x0f)
                {
                    /* Don't write unused channels. */
                }
                else
                {
                    (*pshwTmpOut) = pshwTimeChShort[(swI * swOutNChans) + swCh];
                    pshwTmpOut++;
                    (*pswTotalOutSize) += (VOS_INT32)sizeof(VOS_INT16);
                }
                swChmap >>= XA_AACDEC_CHANNELMAP_SHIFT_SCALE;
            }
         }
    }

    return VOS_OK;
}

#if 0

VOS_VOID AAC_DECODER_SetOutChanFixedTo2(VOS_INT32 *pswChanMap)
{
    VOS_INT32       swChanMap = *pswChanMap;

    /* ʹ�����»������� */
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif




