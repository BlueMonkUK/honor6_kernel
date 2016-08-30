/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : mp3_dec.c
  �� �� ��   : ����
  ��    ��   : twx144307
  ��������   : 2012��12��20��
  ����޸�   :
  ��������   : mp3����ģ��
  �����б�   :

  �޸���ʷ   :
  1.��    ��   : 2012��8��2��
    ��    ��   : twx144307
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "mp3_dec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    ��ά�ɲ���Ϣ�а�����C�ļ���ź궨��
*****************************************************************************/
/*lint -e(767)*/
#define THIS_FILE_ID                    OM_FILE_ID_CODEC_MP3_DEC_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/* mp3�����handle,��init�г�ʼ����ֵ */
xa_codec_handle_t                         g_pstMp3ApiObj;

/* mp3ģ�����漰�ڴ���Ϣ��¼ */
XA_COMM_MEMORY_CFG_STRU                   g_stMp3MemCfg;

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*****************************************************************************
�� �� ����MP3_DECODER_SetParams
��������������MP3�������
��    �룺VOS_UINT32    uwPcmSize           pcmλ��
��    ������
�� �� ֵ��XA_ERRORCODE                      ������
���ú���  :
��������  :


 �޸���ʷ      :
  1.��    ��   : 2012��12��19��
    ��    ��   : twx144307
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_INT32  MP3_DECODER_SetParams(VOS_INT32 swPcmWidthSize)
{
    VOS_INT32        swErrCode;
    VOS_INT32        swCrcCheckFlg    = MP3_DEC_CRC_CHECK_DISABLE;
    xa_codec_func_t *pstMp3ProcessApi = xa_mp3_dec;

    /* ����PCMλ�� */
    swErrCode = (*pstMp3ProcessApi)(g_pstMp3ApiObj,
                          XA_API_CMD_SET_CONFIG_PARAM,
                          XA_MP3DEC_CONFIG_PARAM_PCM_WDSZ,
                          &swPcmWidthSize);

    CODEC_XA_ERRCODE_CHECK(swErrCode);

    /* ����CRCУ�鲻ʹ�� */
    swErrCode = (*pstMp3ProcessApi)(g_pstMp3ApiObj,
                          XA_API_CMD_SET_CONFIG_PARAM,
                          XA_MP3DEC_CONFIG_PARAM_CRC_CHECK,
                          &swCrcCheckFlg);

    CODEC_XA_ERRCODE_CHECK(swErrCode);

    return VOS_OK;
}

/*****************************************************************************
�� �� ����MP3_DECODER_GetParams
������������ȡMP3�������
��    �룺��
��    ����pstConfig       �����Уã͵���Ϣ�ṹ��
�� �� ֵ��VOS_OK �ɹ� ��VOS_ERR ʧ��
���ú���  :
��������  :


 �޸���ʷ      :
  1.��    ��   : 2012��12��19��
    ��    ��   : twx144307
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32  MP3_DECODER_GetParams(AUDIO_DEC_PARAMS_STRU* pstConfig)
{
    VOS_INT32 swErrCode;
    xa_codec_func_t *pstMp3ProcessApi = xa_mp3_dec;

    if(VOS_NULL == pstConfig)
    {
        return VOS_ERR;
    }

    /* ��ȡ������ */
    swErrCode = (*pstMp3ProcessApi)(g_pstMp3ApiObj,
                          XA_API_CMD_GET_CONFIG_PARAM,
                          XA_MP3DEC_CONFIG_PARAM_BITRATE,
                          &(pstConfig->swBitRate));
    XA_ERROR_HANDLE(swErrCode);


    /* ��ȡ������ */
    swErrCode = (*pstMp3ProcessApi)(g_pstMp3ApiObj,
                          XA_API_CMD_GET_CONFIG_PARAM,
                          XA_MP3DEC_CONFIG_PARAM_SAMP_FREQ,
                          &pstConfig->swSampleRate);
    XA_ERROR_HANDLE(swErrCode);

    /* ��ȡ������ */
    swErrCode = (*pstMp3ProcessApi)(g_pstMp3ApiObj,
                          XA_API_CMD_GET_CONFIG_PARAM,
                          XA_MP3DEC_CONFIG_PARAM_NUM_CHANNELS,
                          &(pstConfig->swChannel));
    XA_ERROR_HANDLE(swErrCode);

    /* ��ȡPCMλ�� */
    swErrCode = (*pstMp3ProcessApi)(g_pstMp3ApiObj,
                           XA_API_CMD_GET_CONFIG_PARAM,
                           XA_MP3DEC_CONFIG_PARAM_PCM_WDSZ,
                           &pstConfig->swPcmWidthSize);
    XA_ERROR_HANDLE(swErrCode);

    return VOS_OK;
}

/*****************************************************************************
�� �� ����MP3_DECODER_Init
����������MP3������ʼ��
��    �룺VOS_UINT32 uwPcmSize  pcmλ��
��    ������
�� �� ֵ��0 �ɹ� ����0 ʧ��
���ú���  :
��������  :


 �޸���ʷ      :
  1.��    ��   : 2012��12��19��
    ��    ��   : twx144307
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32  MP3_DECODER_Init(XA_AUDIO_DEC_USER_CFG_STRU *pstMp3UserCfg)
{
    VOS_INT32                       swErrCode;
    xa_codec_func_t                *pstMp3ProcessApi;
    XA_COMM_MEMORY_CFG_STRU        *pstMp3MemCfg       = MP3_DEC_GetMemCfgPtr();
    VOS_VOID                       *pApBufAddr;
    VOS_INT32                       swHeaderMemSize;
    VOS_UINT32                      uwPcmWidthSize;

    /* �쳣��μ�� */
    if (   (VOS_NULL == pstMp3UserCfg)
        || (VOS_NULL == pstMp3UserCfg->pvHeadBuff))
    {
        return VOS_ERR;
    }
    /* ��ȡ������Ϣ */
    pApBufAddr          = pstMp3UserCfg->pvHeadBuff;
    swHeaderMemSize     = pstMp3UserCfg->swHeaderMemSize;
    uwPcmWidthSize      = pstMp3UserCfg->uwPcmWidthSize;

    pstMp3MemCfg->swHeaderMemSize = swHeaderMemSize;
    pstMp3MemCfg->pHeaderBuff     = pApBufAddr;

    pstMp3ProcessApi = xa_mp3_dec;
    /* tensilica ����� Startup �׶�*/
    swErrCode = XA_COMM_Startup(pstMp3ProcessApi,
                               &g_pstMp3ApiObj,
                               pstMp3MemCfg);
    XA_ERROR_HANDLE(swErrCode);

    /* ����MP3�������,��Ҫ��λ�� */
    swErrCode = MP3_DECODER_SetParams((VOS_INT32)uwPcmWidthSize);
    XA_ERROR_HANDLE(swErrCode);

    /* tensilica�����MemoryAlloction�׶� */
    swErrCode = XA_COMM_MemoryAlloc(pstMp3ProcessApi,
                                   g_pstMp3ApiObj,
                                   pstMp3MemCfg);
    XA_ERROR_HANDLE(swErrCode);

    /* tensilica�����ִ�г�ʼ������׶Σ���ȡ�ļ�ͷ��ɳ�ʼ�� */
    swErrCode = XA_COMM_Init(pstMp3ProcessApi,
                            g_pstMp3ApiObj,
                            pstMp3MemCfg->pInputBuff,
                            pstMp3MemCfg->swInputMemSize,
                            pstMp3MemCfg->pHeaderBuff,
                            pstMp3MemCfg->swHeaderMemSize,
                           &pstMp3UserCfg->swConsumed);
    XA_ERROR_HANDLE(swErrCode);

    pstMp3UserCfg->swInSize = pstMp3MemCfg->swInputMemSize;

    return VOS_OK;
}



/*****************************************************************************
�� �� ����MP3_DECODER_Decode
����������һ֡���ݽ���
��    �룺uwInputSize       �������ݴ�С
��    ����puwOuputSize      ������ݴ�С
          puwConsumeSize    �������ݴ�С
          uwDecoderState    CODEC״̬��
�� �� ֵ��VOS_OK �ɹ�; VOS_ERR ʧ��
���ú���  :
��������  :
˵    ��  : ��Ҫͨ��USER���AUDIO PLAYERȡ������:
            stMP3UserCfg.enIsEndOfStream // End of Stream;
            stMP3UserCfg.uwPcmWidthSize  // λ��
            stMP3UserCfg.pvInBuff        // ����BUFF���׵�ַ
            stMP3UserCfg.swInBuffSize    // ����BUFF��Ч�ֽ���
            stMP3UserCfg.pvOutBuff;      // ���ָ��λ��(����Ŀ��)

            ��������������ڽ�������У�Ӧ���������������:
            stMP3UserCfg.swConsumed;     // �˴ν������ĵ�BUFF��С
            stMP3UserCfg.swOutputSize;   // �����������ݵ��ܴ�С
            stMP3UserCfg.uwDecoderState; // ����״̬

 �޸���ʷ      :
  1.��    ��   : 2012��12��19��
    ��    ��   : twx144307
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 MP3_DECODER_Decode(XA_AUDIO_DEC_USER_CFG_STRU *pstUserCfg)
{
    VOS_INT32                   swErrCode;
    VOS_INT32                   swDecodeFinish          = CODEC_DECODE_UNFINISH_FLAG;
    xa_codec_func_t             *pstMp3ProcessApi       = xa_mp3_dec;
    XA_COMM_MEMORY_CFG_STRU     *pstMemCfg              = MP3_DEC_GetMemCfgPtr();

    /* for input */
    VOS_INT32                   swInputSize     = 0;
    VOS_UINT32                  uwPcmWidthSize  = 0;
    VOS_UINT32                  enIsEndOfStream = 0;

    /* for output */
    VOS_INT32                    swOutputSize           = 0;
    VOS_INT32                    swConsumeSize          = 0;
    VOS_UINT32                   uwDecoderState         = CODEC_STATE_INITIALIZED;

    /* ��μ�� */
    if(VOS_NULL == pstUserCfg)
    {
        return VOS_ERR;
    }

    swInputSize     = pstUserCfg->swInSize;
    uwPcmWidthSize  = pstUserCfg->uwPcmWidthSize;
    enIsEndOfStream = pstUserCfg->enIsEndOfStream;

    if (16 != uwPcmWidthSize)
    {
        return VOS_ERR;
    }

    /* for input */
    UCOM_MemCpy(pstMemCfg->pInputBuff, pstUserCfg->pvInBuff, (VOS_UINT32)pstUserCfg->swInSize);


    swErrCode = XA_COMM_Decode(pstMp3ProcessApi,
                              g_pstMp3ApiObj,
                              swInputSize,
                              enIsEndOfStream,
                              &swOutputSize,
                              &swConsumeSize,
                              &swDecodeFinish);
    XA_ERROR_HANDLE_WITH_STATUS_CODE(swErrCode, &uwDecoderState);

    /* �ж��Ƿ������� */
    if(CODEC_DECODE_FINISH_FLAG == swDecodeFinish)
    {
        uwDecoderState = CODEC_STATE_FINISH;
    }

    /* �������buff */
    UCOM_MemCpy(pstUserCfg->pvOutBuff, pstMemCfg->pOutputBuff, (VOS_UINT32)swOutputSize);
    pstUserCfg->swConsumed     = swConsumeSize;
    pstUserCfg->uwDecoderState = uwDecoderState;
    pstUserCfg->swOutputSize   = swOutputSize;

    return VOS_OK;
}

/*****************************************************************************
�� �� ����MP3_DECODER_Seek
����������seek����
��    �룺VOS_UINT32                uwSkipSize                 //�������ֽ���
          CODEC_SEEK_DERECT_ENUM    enSeekDirect               //����
��    ����
�� �� ֵ��VOS_OK �ɹ� ��VOS_ERR ʧ��
���ú���  :
��������  :


 �޸���ʷ      :
  1.��    ��   : 2012��12��19��
    ��    ��   : twx144307
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32  MP3_DECODER_Seek(VOS_UINT32 uwSkipSize, CODEC_SEEK_DERECT_ENUM_UINT16 enSeekDirect)
{
    VOS_INT32    swErrCode;
    VOS_UINT32   uwCurrStreamPos;
    VOS_UINT32   uwNewStreamPos;
    xa_codec_func_t *pstMp3ProcessApi = xa_mp3_dec;

    //��ѯ��ǰ����λ��
    swErrCode = (*pstMp3ProcessApi)(g_pstMp3ApiObj,
                          XA_API_CMD_GET_CONFIG_PARAM,
                          XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS,
                          &uwCurrStreamPos);
    XA_ERROR_HANDLE(swErrCode);

    //����Ҫ�ƶ����Ĳ���λ��
    if(CODEC_SEEK_FORWARDS == enSeekDirect)
    {
        //�����λ���Ƿ񳬹����ļ���С����Ҫ֪���ļ���С
        uwNewStreamPos = uwCurrStreamPos + uwSkipSize;
    }
    else
    {
        if(uwSkipSize > uwCurrStreamPos)
        {
            return VOS_ERR;
        }
        uwNewStreamPos = uwCurrStreamPos - uwSkipSize;
    }

    //�����µĲ���λ��
    swErrCode = (*pstMp3ProcessApi)(g_pstMp3ApiObj,
                          XA_API_CMD_SET_CONFIG_PARAM,
                          XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS,
                          &uwNewStreamPos);
    XA_ERROR_HANDLE(swErrCode);

    swErrCode = (*pstMp3ProcessApi)(g_pstMp3ApiObj,
                          XA_API_CMD_EXECUTE,
                          XA_CMD_TYPE_DO_RUNTIME_INIT,
                          NULL);
    XA_ERROR_HANDLE(swErrCode);

    return VOS_OK;
}

/*****************************************************************************
�� �� ����MP3_DECODER_Close
�����������رտ�
��    �룺��
��    ������
�� �� ֵ����
���ú���  :
��������  :


 �޸���ʷ      :
  1.��    ��   : 2012��12��19��
    ��    ��   : twx144307
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_VOID MP3_DECODER_Close(VOS_VOID)
{
    XA_COMM_MEMORY_CFG_STRU            *pstMp3MemCfg    = MP3_DEC_GetMemCfgPtr();

    /* �ͷ����ж�̬����ռ� */
    XA_COMM_MemFree(pstMp3MemCfg);

    /* ����APIObj */
    g_pstMp3ApiObj = VOS_NULL;

    return;
}

/*****************************************************************************
�� �� ����MP3_DECODER_GetCurrDecPos
������������ѯ��ǰ������ȣ���ֹ��һ֡���Ѿ�������ֽ���
��    ����VOS_UINT32 *puwPosInBytes     �Ѿ������ֽ���
�� �� ֵ��VOS_OK �ɹ�:VOS_ERR ʧ��
���ú���  :
��������  :


 �޸���ʷ      :
  1.��    ��   : 2012��12��31��
    ��    ��   : twx144307
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_INT32 MP3_DECODER_GetCurrDecPos(VOS_UINT32 *puwPosInBytes)
{
    VOS_INT32 swErrCode;
    xa_codec_func_t *pstMp3ProcessApi = xa_mp3_dec;

    swErrCode = (*pstMp3ProcessApi)(g_pstMp3ApiObj,
                          XA_API_CMD_GET_CONFIG_PARAM,
                          XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS,
                          puwPosInBytes);
    return swErrCode;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif





