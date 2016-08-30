/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : audio_pcm.c
  �� �� ��   : ����
  ��    ��   : C00137131
  ��������   : 2012��8��2��
  ����޸�   :
  ��������   : Hifi��Ƶ�������ģ��
  �����б�   :
              AUDIO_PCM_CaptureDmaIsr
              AUDIO_PCM_GetCurCaptureBuff
              AUDIO_PCM_GetCurPlayBuff
              AUDIO_PCM_MsgPcmCloseReq
              AUDIO_PCM_MsgPcmHwFreeReq
              AUDIO_PCM_MsgPcmHwParaReq
              AUDIO_PCM_MsgPcmOpenReq
              AUDIO_PCM_MsgPcmPointerReq
              AUDIO_PCM_MsgPcmPrepareReq
              AUDIO_PCM_MsgPcmSetBufCmd
              AUDIO_PCM_MsgPcmTriggerReq
              AUDIO_PCM_MsgUpdateCaptureBuffCmd
              AUDIO_PCM_MsgUpdatePlayBuffCmd
              AUDIO_PCM_PlayDmaIsr
              AUDIO_PCM_RegisterPcmCbFunc
              AUDIO_PCM_StartCaptureLoopDMA
              AUDIO_PCM_StartPlayLoopDMA
              AUDIO_PCM_UnRegisterPcmCbFunc
  �޸���ʷ   :
  1.��    ��   : 2012��8��2��
    ��    ��   : C00137131
    �޸�����   : �����ļ�
******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "audio_pcm.h"
#include "med_drv_dma.h"
#include "med_drv_sio.h"
#include "om.h"
#include "ucom_comm.h"
#include "codec_op_vec.h"
#include "audio_debug.h"
#include "med_drv_timer_hifi.h"
#include "ucom_nv.h"
#include "audio_enhance.h"
#include "om_cpuview.h"
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
#define THIS_FILE_ID                    OM_FILE_ID_AUDIO_PCM_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/* ������Ƶͨ·��ؿ�����Ϣ */
AUDIO_PCM_CTRL_OBJ_STRU                 g_stAudioPcmObj;

/* ������Ƶ���š�¼����Buffer��Ϣ */
UCOM_SEC_TCMBSS
AUDIO_PCM_PINGPONG_BUF_STRU             g_stAudioPcmPbBuff;

AUDIO_PCM_PINGPONG_BUF_STRU             g_stAudioPcmCpBuff;

/* ������Ƶ���š�¼������ʱBuffer��Ϣ */
AUDIO_PCM_TMP_BUF_STRU                  g_stAudioPcmTmpBuff;

UCOM_SEC_TCMBSS
UCOM_ALIGN(32)
DRV_DMA_CXCFG_STRU                      g_astAudioPlayDmaPara[AUDIO_PCM_PINGPONG_BUF_NUM] = {0};/* ��ǰ����ͨ��ѭ��Buf���˵�DMA����*/

UCOM_ALIGN(32)
DRV_DMA_CXCFG_STRU                      g_astAudioCaptureDmaPara[AUDIO_PCM_PINGPONG_BUF_NUM] = {0};/* ��ǰ¼��ͨ��ѭ��Buf���˵�DMA����*/

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
/*****************************************************************************
 �� �� ��  : AUDIO_PCM_Init
 ��������  : Audio PCMģ���ʼ��
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��5��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID AUDIO_PCM_Init(VOS_VOID)
{
    AUDIO_PCM_CTRL_OBJ_STRU         *pstAudioPcmObj = AUDIO_PcmGetPcmObjPtr();
    VOS_VOID                        *pvTmp          = VOS_NULL;

    /* ��ʼ��g_stAudioPcmObjΪȫ0 */
    UCOM_MemSet(pstAudioPcmObj, 0, sizeof(AUDIO_PCM_CTRL_OBJ_STRU));

    /* ��ʼ��g_stAudioPcmPbBuffȫ0���豣֤Uncache */
    pvTmp   = (VOS_VOID *)AUDIO_PcmGetPcmPbBufPtr();

    UCOM_MemSet(pvTmp, 0, sizeof(AUDIO_PCM_PINGPONG_BUF_STRU));

    /* ��ʼ��g_stAudioPcmCpBuffȫ0���豣֤Uncache */
    pvTmp   = (VOS_VOID *)AUDIO_PcmGetPcmCpBufPtr();

    UCOM_MemSet(pvTmp, 0, sizeof(AUDIO_PCM_PINGPONG_BUF_STRU));

    /* ��ʼ��g_astAudioPlayDmaParaȫ0���豣֤Uncache */
    pvTmp   = (VOS_VOID *)AUDIO_PcmGetPlayDmaCfgPtr();

    UCOM_MemSet(pvTmp, 0, AUDIO_PCM_PINGPONG_BUF_NUM * sizeof(DRV_DMA_CXCFG_STRU));

    /* ��ʼ��g_astAudioCaptureDmaParaȫ0���豣֤Uncache */
    pvTmp   = (VOS_VOID *)UCOM_GetUncachedAddr((VOS_UINT32)AUDIO_PcmGetCaptureDmaCfgPtr());

    UCOM_MemSet(pvTmp, 0, AUDIO_PCM_PINGPONG_BUF_NUM * sizeof(DRV_DMA_CXCFG_STRU));

}

/*****************************************************************************
 �� �� ��  : AUDIO_PCM_RegisterPcmCbFunc
 ��������  : ע��PCM����ʱHifi�Ļص���������¼��ʱ�ĵ�MIC���������л�����
 �������  : AUDIO_PCM_MODE_ENUM_UINT16         enPcmMode    --�������ģʽ
             AUDIO_PCM_OBJ_ENUM_UINT16          enPcmObj     --�����������
             AUDIO_CAPTURE_MODE_ENUM_UINT16     enCaptureMode--¼��ģʽ
             AUDIO_PCM_CALLBACK                 pfunCallBack --�ص�����
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��24��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 AUDIO_PCM_RegisterPcmCbFunc(
                AUDIO_PCM_MODE_ENUM_UINT16      enPcmMode,
                AUDIO_PCM_OBJ_ENUM_UINT16       enPcmObj,
                AUDIO_CAPTURE_MODE_ENUM_UINT16  enCaptureMode,
                AUDIO_PCM_CALLBACK              pfunCallBack)
{
    AUDIO_PCM_CFG_STRU                 *pstPcmCfg   = VOS_NULL;

    /* ������ */
    if (  (enPcmMode >= AUDIO_PCM_MODE_BUT)
        ||(enPcmObj >= AUDIO_PCM_OBJ_BUT)
        ||(enCaptureMode >= AUDIO_CAPTURE_MODE_BUT))
    {
        return UCOM_RET_FAIL;
    }

    /* ��ȡ��ͨ�����������������ṹ�� */
    pstPcmCfg   = AUDIO_PcmGetPcmCfgPtr(enPcmMode);

    /* Ϊ��ֹͨ��¼��ע�ắ�������ǣ�ͨ��¼��ʱע��������� */
    if (  (AUDIO_CAPTURE_MODE_CALL == enCaptureMode)
        ||(AUDIO_PCM_OBJ_HIFI == enPcmObj))
    {
        pstPcmCfg->astPcmBuf[enPcmObj].pfunVcRecordCB = pfunCallBack;
    }
    else
    {
        /* ���ص�����������ͨ�������ṹ���У��������ݽ������󣬼��ĸ�������Ҫ���øûص����� */
        pstPcmCfg->astPcmBuf[enPcmObj].pfunCallBack = pfunCallBack;
    }

    /* ��¼�����ͱ����ڵ�ǰͨ�������ṹ���� */
    pstPcmCfg->enCaptureMode    = enCaptureMode;

    OM_LogInfo3(AUDIO_PCM_RegisterCbkOK,
                enPcmMode,
                enPcmObj,
                enCaptureMode);

    return UCOM_RET_SUCC;
}

/*****************************************************************************
 �� �� ��  : AUDIO_PCM_UnRegisterPcmCbFunc
 ��������  : ȥע��PCM����ʱHifi�Ļص�����
 �������  : AUDIO_PCM_MODE_ENUM_UINT16 enPcmMode
             AUDIO_PCM_OBJ_ENUM_UINT16  enPcmObj
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��24��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 AUDIO_PCM_UnRegisterPcmCbFunc(
                AUDIO_PCM_MODE_ENUM_UINT16  enPcmMode,
                AUDIO_PCM_OBJ_ENUM_UINT16   enPcmObj)
{
    AUDIO_PCM_CFG_STRU                 *pstPcmCfg   = VOS_NULL;

    /* ������ */
    if (  (enPcmMode >= AUDIO_PCM_MODE_BUT)
        ||(enPcmObj >= AUDIO_PCM_OBJ_BUT))
    {
        return UCOM_RET_FAIL;
    }

    /* ��ȡ��ͨ�����������������ṹ�� */
    pstPcmCfg   = AUDIO_PcmGetPcmCfgPtr(enPcmMode);

    /* ��ͨ�������ṹ���еĻص��������� */
    pstPcmCfg->astPcmBuf[enPcmObj].pfunCallBack = VOS_NULL;

    return UCOM_RET_SUCC;
}

/*****************************************************************************
 �� �� ��  : AUDIO_PCM_MsgPcmOpenReq
 ��������  : ����ID_AP_AUDIO_PCM_OPEN_REQ��Ϣ������Hifi��Ƶ��������ͨ������
             ����������,����Ϣ��Ϊ�����ȼ�������Ӧ
 �������  : VOS_VOID *pvOsaMsg --ID_AP_AUDIO_PCM_OPEN_REQ��Ϣ��
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��5��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 AUDIO_PCM_MsgPcmOpenReq(VOS_VOID *pvOsaMsg)
{
    AUDIO_PCM_OPEN_REQ_STRU            *pstPcmOpenMsg   = VOS_NULL;

    /* ��AUDIO_PCM_OPEN_REQ_STRU��ʽ������Ϣ */
    pstPcmOpenMsg   = (AUDIO_PCM_OPEN_REQ_STRU*)((MsgBlock *) pvOsaMsg)->aucValue;

    /* ������ */
    if (pstPcmOpenMsg->enPcmMode >= AUDIO_PCM_MODE_BUT)
    {
        return UCOM_RET_ERR_PARA;
    }
    else
    {
        /* ����������Ƶͨ��,����PCMģ��״̬g_stAudioPcmObj.enPcmStatus */
        if (AUDIO_PCM_STATUS_DEACTIVE == AUDIO_PcmGetState())
        {
            AUDIO_PcmSetState(AUDIO_PCM_STATUS_ACTIVE);
        }
    }

    return UCOM_RET_SUCC;

}

/*****************************************************************************
 �� �� ��  : AUDIO_PCM_MsgPcmCloseReq
 ��������  : ����ID_AP_AUDIO_PCM_CLOSE_REQ��Ϣ��ֹͣHifi��������ͨ������
 �������  : VOS_VOID *pvOsaMsg     --ID_AP_AUDIO_PCM_CLOSE_REQ��Ϣ��
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��5��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 AUDIO_PCM_MsgPcmCloseReq(VOS_VOID *pvOsaMsg)
{
    AUDIO_PCM_CLOSE_REQ_STRU           *pstPcmCloseMsg  = VOS_NULL;
    AUDIO_PCM_CFG_STRU                 *pstPcmCfg       = VOS_NULL;
    AUDIO_PCM_ENHANCE_STOP_IND_STRU     stEnhanceStop;

    /* ��AP_AUDIO_PCM_CLOSE_REQ_STRU��ʽ������Ϣ */
    pstPcmCloseMsg  = (AUDIO_PCM_CLOSE_REQ_STRU*)((MsgBlock *) pvOsaMsg)->aucValue;

    /* ������ */
    if (pstPcmCloseMsg->enPcmMode >= AUDIO_PCM_MODE_BUT)
    {
        return UCOM_RET_ERR_PARA;
    }
    else
    {
        /* ��ȡ��ͨ�����������������ṹ�� */
        pstPcmCfg   = AUDIO_PcmGetPcmCfgPtr(pstPcmCloseMsg->enPcmMode);

        if (  (AUDIO_PCM_STATUS_DEACTIVE == pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_AP].enBufStatus)
            &&(AUDIO_PCM_STATUS_DEACTIVE == pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_HIFI].enBufStatus))
        {
            /* ��ʽ�رղ��Ż�¼��ͨ�� */
            AUDIO_PcmSetSpeState(pstPcmCloseMsg->enPcmMode, AUDIO_PCM_STATUS_DEACTIVE);
        }

        /* ������ͨ���ѹرգ�������PCMģ��Ҳ�ر� */
        if (  (AUDIO_PCM_STATUS_DEACTIVE == AUDIO_PcmGetSpeState(AUDIO_PCM_MODE_PLAYBACK))
            &&(AUDIO_PCM_STATUS_DEACTIVE == AUDIO_PcmGetSpeState(AUDIO_PCM_MODE_CAPTURE)))
        {
            AUDIO_PcmSetState(AUDIO_PCM_STATUS_DEACTIVE);

            /* �ر���ƵSIO�豸��SlimBusͨ���豸 */
            /* AUDIO_PCM_IO_Close(); */
        }

        /* �ر���Ƶ��ǿ */

        UCOM_MemSet(&stEnhanceStop, 0, sizeof(AUDIO_PCM_ENHANCE_STOP_IND_STRU));

        stEnhanceStop.uhwMsgId = ID_AUDIO_PCM_ENHANCE_STOP_IND;
        stEnhanceStop.enModule = AUDIO_ENHANCE_MODULE_DEFAULT;

        /* ����ͨ��VOS���ͽӿڣ�������Ϣ */
        UCOM_SendOsaMsg(DSP_PID_AUDIO_RT,
                        DSP_PID_AUDIO,
                       &stEnhanceStop,
                        sizeof(AUDIO_PCM_ENHANCE_STOP_IND_STRU));

    }

    return UCOM_RET_SUCC;

}


/*****************************************************************************
 �� �� ��  : AUDIO_PCM_MsgPcmHwParaReq
 ��������  : ����ID_AP_AUDIO_PCM_HW_PARA_REQ��Ϣ������DMAͨ·����س�ʼ��
 �������  : VOS_VOID *pvOsaMsg     --ID_AP_AUDIO_PCM_HW_PARA_REQ��Ϣ��
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��10��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 AUDIO_PCM_MsgPcmHwParaReq(VOS_VOID *pvOsaMsg)
{
    AUDIO_PCM_HW_PARAMS_REQ_STRU       *pstPcmHwParaMsg = VOS_NULL;
    AUDIO_PCM_CFG_STRU                 *pstPcmCfg       = VOS_NULL;
    AUDIO_PCM_ENHANCE_START_IND_STRU    stEnhanceStartInd;

    /* ��AUDIO_PCM_HW_PARAMS_REQ_STRU��ʽ������Ϣ */
    pstPcmHwParaMsg   = (AUDIO_PCM_HW_PARAMS_REQ_STRU*)((MsgBlock *) pvOsaMsg)->aucValue;

    /* ������ */
    if (  (pstPcmHwParaMsg->enPcmMode >= AUDIO_PCM_MODE_BUT)
        ||(pstPcmHwParaMsg->uwSampleRate > AUDIO_PCM_MAX_SAMPLE_RATE)
        ||(pstPcmHwParaMsg->uwChannelNum > AUDIO_PCM_MAX_CHANNEL_NUM))
    {
        return UCOM_RET_ERR_PARA;
    }
    else
    {
        /* ��ȡ��ǰ���Ż�¼��ͨ���Ľṹ��ָ�� */
        pstPcmCfg   = AUDIO_PcmGetPcmCfgPtr(pstPcmHwParaMsg->enPcmMode);

        /* ����ָ��ͨ����PCM��Ϣ�������ڶ�Ӧ��ȫ�ֱ���ͨ����AUDIO_PCM_CFG_STRU�� */
        pstPcmCfg->uwSampleRate = pstPcmHwParaMsg->uwSampleRate;
        pstPcmCfg->uwChannelNum = pstPcmHwParaMsg->uwChannelNum;

        /* ����DMA���˳��ȣ��ݽ�֧��16bit���ݴ��� */
        pstPcmCfg->uwBufSize    = ((pstPcmCfg->uwSampleRate*pstPcmCfg->uwChannelNum)\
                                    /AUDIO_PCM_FRAME_PER_SEC) * AUDIO_PCM_FORMAT_16_SIZE;

        /* ������Ƶ��ǿ */

        UCOM_MemSet(&stEnhanceStartInd, 0, sizeof(AUDIO_PCM_ENHANCE_START_IND_STRU));

        stEnhanceStartInd.uhwMsgId      = ID_AUDIO_PCM_ENHANCE_START_IND;
        stEnhanceStartInd.uwSampleRate  = pstPcmCfg->uwSampleRate;
        stEnhanceStartInd.enModule      = AUDIO_ENHANCE_MODULE_DEFAULT;

        /* ����ͨ��VOS���ͽӿڣ�������Ϣ */
        UCOM_SendOsaMsg(DSP_PID_AUDIO_RT,
                        DSP_PID_AUDIO,
                       &stEnhanceStartInd,
                        sizeof(AUDIO_PCM_ENHANCE_START_IND_STRU));
    }

    return UCOM_RET_SUCC;

}


/*****************************************************************************
 �� �� ��  : AUDIO_PCM_MsgPcmHwFreeReq
 ��������  : ����ID_AP_AUDIO_PCM_HW_FREE_REQ��Ϣ���ͷ�DMA�������Դ
 �������  : VOS_VOID *pvOsaMsg
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��10��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 AUDIO_PCM_MsgPcmHwFreeReq(VOS_VOID *pvOsaMsg)
{
    /* �ô���Ӧ����AP��PCM����ֱ����ɣ��˴�����Ϊһ�������ӿڣ�������ɾ�� */

    return UCOM_RET_SUCC;
}


/*****************************************************************************
 �� �� ��  : AUDIO_PCM_MsgPcmPrepareReq
 ��������  : ����ID_AP_AUDIO_PCM_PREPARE_REQ��Ϣ����ʼ������豸
 �������  : VOS_VOID *pvOsaMsg
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��9��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 AUDIO_PCM_MsgPcmPrepareReq(VOS_VOID *pvOsaMsg)
{
    /* ��AUDIO_PCM_PREPARE_REQ_STRU�ṹ������Ϣ */

    return UCOM_RET_SUCC;

}

/*****************************************************************************
 �� �� ��  : AUDIO_PCM_MsgPcmTriggerReq
 ��������  : ����ID_AP_AUDIO_PCM_TRIGGER_REQ��Ϣ������ĳ����ͨ����״̬
 �������  : VOS_VOID *pvOsaMsg     --ID_AP_AUDIO_PCM_TRIGGER_REQ��Ϣ��
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��5��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 AUDIO_PCM_MsgPcmTriggerReq(VOS_VOID *pvOsaMsg)
{
    AUDIO_PCM_TRIGGER_REQ_STRU         *pstPcmTriggerMsg    = VOS_NULL;
    AUDIO_PCM_CFG_STRU                 *pstPcmCfg           = VOS_NULL;
    AUDIO_PCM_OBJ_ENUM_UINT16           enPcmObj            = AUDIO_PCM_OBJ_BUT;
    DRV_SIO_INT_MASK_ENUM_UINT32        enMask          = DRV_SIO_INT_MASK_RX_RIGHT_FIFO_OVER;
    CODEC_NV_PARA_SIO_AUDIO_MASTER_STRU stIsAudioMaster;

    VOS_UINT32                          uwRet               = UCOM_RET_SUCC;

    /* ��AUDIO_PCM_TRIGGER_REQ_STRU��ʽ������Ϣ */
    pstPcmTriggerMsg    = (AUDIO_PCM_TRIGGER_REQ_STRU*)((MsgBlock *) pvOsaMsg)->aucValue;

    /* ������ */
    if (  (pstPcmTriggerMsg->enPcmMode >= AUDIO_PCM_MODE_BUT)
        ||(pstPcmTriggerMsg->enCmd >= AUDIO_PCM_TRIGGER_BUT)
        ||(pstPcmTriggerMsg->enPcmObj >= AUDIO_PCM_OBJ_BUT))
    {
        return UCOM_RET_ERR_PARA;
    }

    /* ��ȡTrigger��������Դ���� */
    enPcmObj    = pstPcmTriggerMsg->enPcmObj;

    /* ��ȡ��ͨ�����������������ṹ�� */
    pstPcmCfg   = AUDIO_PcmGetPcmCfgPtr(pstPcmTriggerMsg->enPcmMode);

    /*����triggerָ�������Ӧ����*/
    switch(pstPcmTriggerMsg->enCmd)
    {
        case AUDIO_PCM_TRIGGER_START:
        case AUDIO_PCM_TRIGGER_RESUME:
        case AUDIO_PCM_TRIGGER_PAUSE_RELEASE:
        {

            /* ����NV���趨PCMΪ����ӣ��ڴ˶���NV�� */
            UCOM_NV_Read(en_NV_Item_SIO_Audio_Master, &(stIsAudioMaster.uhwIsPcmMaster), sizeof(CODEC_NV_PARA_SIO_AUDIO_MASTER_STRU));
            /* ����ƵSIO�豸��SlimBusͨ���豸,���Ѿ���,Ҫ��ýӿ�ֱ�ӷ���OK,�������´򿪵Ķ��� */
            uwRet = AUDIO_PCM_IO_Open(enMask|DRV_SIO_INT_MASK_TX_RIGHT_FIFO_UNDER,
                                      pstPcmCfg->uwSampleRate,
                                      stIsAudioMaster.uhwIsPcmMaster,
                                      VOS_NULL,
                                      0);

            if (UCOM_RET_SUCC != uwRet)
            {
                return UCOM_RET_FAIL;
            }

            /* �����ͨ������������Buff��Ϣ����Ӧͨ���ṹ����,�뱣֤Uncache���� */
            pstPcmCfg->astPcmBuf[enPcmObj].uwBufAddr = pstPcmTriggerMsg->uwBufAddr;

            /* �����ͨ������Ƶ�������,�ñ�Ž���AP��Ƶͨ����Ч */
            pstPcmCfg->astPcmBuf[enPcmObj].pSubStream= pstPcmTriggerMsg->pSubStream;

            /* ���¸�ͨ����Buffʹ��״̬Ϊʹ��״̬ */
            pstPcmCfg->astPcmBuf[enPcmObj].enBufStatus = AUDIO_PCM_STATUS_ACTIVE;

            /* ��ʼ����ͨ����buff�Ƿ�ready��־λ */
            pstPcmCfg->astPcmBuf[enPcmObj].enBufReadyFlag = AUDIO_PCM_BUFF_ISREADY_NO;

            /* ��ǰͨ��Ϊ����ͨ����ͨ��δ��������������DMA��SIO */
            if (AUDIO_PCM_STATUS_ACTIVE != pstPcmCfg->enPcmStatus)
            {
                AUDIO_PcmSetState(AUDIO_PCM_STATUS_ACTIVE);

                pstPcmCfg->enPcmStatus  = AUDIO_PCM_STATUS_ACTIVE;

                /* ͨ��ģʽΪ���� */
                if (AUDIO_PCM_MODE_PLAYBACK == pstPcmTriggerMsg->enPcmMode)
                {
                    /* ��������DMA */
                    uwRet = AUDIO_PCM_StartPlayLoopDMA((VOS_UINT16)pstPcmCfg->uwBufSize, AUDIO_PCM_DMAC_CHN_PLAY);
                }
                /* ��ǰͨ��Ϊ¼��ͨ�����ҷ�Hifiͨ��¼��״̬ */
                else if (AUDIO_PCM_MODE_CAPTURE == pstPcmTriggerMsg->enPcmMode)
                {
                    /* ����¼��DMA */
                    uwRet = AUDIO_PCM_StartCaptureLoopDMA((VOS_UINT16)pstPcmCfg->uwBufSize, AUDIO_PCM_DMAC_CHN_CAPTURE);
                }
                /* ͨ��ģʽΪ��������HDMI�ȣ��ݲ�ʵ�� */
                else
                {
                    /* for pc-lint */
                }

                if (UCOM_RET_SUCC != uwRet)
                {
                    return uwRet;
                }
             }
        }
        break;
        case AUDIO_PCM_TRIGGER_STOP:
        case AUDIO_PCM_TRIGGER_SUSPEND:
        case AUDIO_PCM_TRIGGER_PAUSE_PUSH:
        {
            /* ���¸�ͨ����Buffʹ��״̬Ϊ��ʹ��״̬ */
            pstPcmCfg->astPcmBuf[enPcmObj].enBufStatus = AUDIO_PCM_STATUS_DEACTIVE;

            /* ��ʼ����ͨ����buff�Ƿ�ready��־λ */
            pstPcmCfg->astPcmBuf[enPcmObj].enBufReadyFlag = AUDIO_PCM_BUFF_ISREADY_NO;

            /* ��ǰͨ��AP��HIFI��ͨ��ͬʱΪDEACTIVE */
            if ((AUDIO_PCM_STATUS_DEACTIVE == pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_AP].enBufStatus)
                &&(AUDIO_PCM_STATUS_DEACTIVE == pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_HIFI].enBufStatus))
            {
                /* ���µ�ǰͨ��������״̬Ϊֹͣ */
                AUDIO_PcmSetSpeState(pstPcmTriggerMsg->enPcmMode, AUDIO_PCM_STATUS_DEACTIVE);

                /* ͨ��ģʽΪ���� */
                if (AUDIO_PCM_MODE_PLAYBACK == pstPcmTriggerMsg->enPcmMode)
                {
                    /* ֹͣ��Ƶ���ͨ����DMA���� */
                    DRV_DMA_Stop(AUDIO_PCM_DMAC_CHN_PLAY);
                }
                /* ��ǰͨ��Ϊ¼��ͨ�����ҷ�Hifiͨ��¼��״̬ */
                else if ( (AUDIO_PCM_MODE_CAPTURE == pstPcmTriggerMsg->enPcmMode)
                        &&(AUDIO_CAPTURE_MODE_CALL != pstPcmCfg->enCaptureMode))
                {
                    /* ֹͣ��Ƶ����ͨ����SIO�豸 */
                    DRV_DMA_Stop(AUDIO_PCM_DMAC_CHN_CAPTURE);
                }
                /* ͨ��ģʽΪ��������HDMI�ȣ��ݲ�ʵ�� */
                else
                {
                    /* for pc-lint */
                }
            }

            if (AUDIO_PCM_OBJ_AP == enPcmObj)
            {
                AUDIO_PCM_TRIGGER_CNF_STRU  stMsgTrigerCnf;
                stMsgTrigerCnf.uhwMsgId     = ID_AUDIO_AP_PCM_TRIGGER_CNF;
                stMsgTrigerCnf.enPcmMode    = pstPcmTriggerMsg->enPcmMode;
                stMsgTrigerCnf.pSubStream   = pstPcmCfg->astPcmBuf[enPcmObj].pSubStream;
                stMsgTrigerCnf.enCmd        = pstPcmTriggerMsg->enCmd;

                /* ���ÿ����Ƶchannel�ӿ�,֪ͨAP,PERIOD�������,����snd_pcm_period_elapsed,����Buffer */
                UCOM_SendAudioMsg(DSP_PID_AUDIO,
                                  ACPU_PID_OM,           /* �˴���дAP��DRV��Ӧ��PID,,���� */
                                  &stMsgTrigerCnf,
                                  sizeof(AUDIO_PCM_TRIGGER_CNF_STRU));
            }
        }
        break;

        default:
        break;
    }

    return UCOM_RET_SUCC;

}

/*****************************************************************************
 �� �� ��  : AUDIO_PCM_MsgPcmPointerReq
 ��������  : ����ID_AP_AUDIO_PCM_POINTER_REQ��Ϣ���ϱ���ǰDMA����PERIOD��״̬
 �������  : VOS_VOID *pvOsaMsg
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��5��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 AUDIO_PCM_MsgPcmPointerReq(VOS_VOID *pvOsaMsg)
{
    /* �ô���Ӧ����AP��PCM����ֱ����ɣ��˴�����Ϊһ�������ӿڣ�������ɾ�� */

    return UCOM_RET_SUCC;
}

/*****************************************************************************
 �� �� ��  : AUDIO_PCM_MsgPcmSetBufReq
 ��������  : ����ID_AP_AUDIO_PCM_SET_BUF_REQ��Ϣ������ָ��ͨ��������������
             ������ַ��������������и���ϢҪ��ÿ20ms����һ��
 �������  : VOS_VOID *pvOsaMsg     --ID_AP_AUDIO_PCM_SET_BUF_REQ��Ϣ��
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��20��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 AUDIO_PCM_MsgPcmSetBufCmd(VOS_VOID *pvOsaMsg)
{
    AUDIO_PCM_SET_BUF_CMD_STRU         *pstSetBufMsg    = VOS_NULL;
    AUDIO_PCM_CFG_STRU                 *pstPcmCfg       = VOS_NULL;

    /* ��AUDIO_PCM_SET_BUF_REQ_STRU��ʽ������Ϣ */
    pstSetBufMsg  = (AUDIO_PCM_SET_BUF_CMD_STRU*)((MsgBlock *) pvOsaMsg)->aucValue;

    /* ������ */
    if (  (pstSetBufMsg->enPcmMode >= AUDIO_PCM_MODE_BUT)
        ||(pstSetBufMsg->enPcmObj >= AUDIO_PCM_OBJ_BUT))
    {
        return UCOM_RET_ERR_PARA;
    }
    else
    {
        pstPcmCfg = AUDIO_PcmGetPcmCfgPtr(pstSetBufMsg->enPcmMode);

        /* ����ָ��ͨ�����������Buff����,�뱣֤Uncache���� */
        pstPcmCfg->astPcmBuf[pstSetBufMsg->enPcmObj].uwBufAddr = pstSetBufMsg->uwBufAddr;

        /* ���ø�ͨ����buff�Ƿ�ready��־λ */
        pstPcmCfg->astPcmBuf[pstSetBufMsg->enPcmObj].enBufReadyFlag = AUDIO_PCM_BUFF_ISREADY_YES;
    }

    return UCOM_RET_SUCC;
}

/*****************************************************************************
 �� �� ��  : AUDIO_PCM_StartPlayLoopDMA
 ��������  : ������Ƶ���ŵ�DMAͨ��
 �������  : VOS_UINT16 uhwLen          --ÿ�ΰ��˵ĳ���
             VOS_UINT16 usChNum         --���˵�DMAͨ����
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��23��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 AUDIO_PCM_StartPlayLoopDMA(
                VOS_UINT16 uhwLen,
                VOS_UINT16 usChNum)
{
    VOS_UINT32              uwRet;
    VOS_UINT32              uwSrcAddrA, uwSrcAddrB;     /* ��Ƶ���ŵ�DMAͨ����Դ ��ַ */
    VOS_UINT32              uwLliAddrA , uwLliAddrB;    /* ��Ƶ����DMA���ӵ�ַ������Ϊ���ߵ�ַ */
    VOS_INT16              *pshwCurCBuf = AUDIO_PCM_GetCurPlayBuff();
    VOS_INT16              *shwBufA     = AUDIO_PcmGetPlayBufAPtr();
    VOS_INT16              *shwBufB     = AUDIO_PcmGetPlayBufBPtr();

    DRV_DMA_CXCFG_STRU     *pstDmaCfgA  = VOS_NULL;
    DRV_DMA_CXCFG_STRU     *pstDmaCfgB  = VOS_NULL;

    /* ֹͣ��Ƶ���ŵ�DMA */
    DRV_DMA_Stop(usChNum);

    /* ����SrcAddr��ֵ */
    if (pshwCurCBuf == shwBufA)
    {
        uwSrcAddrA  = (VOS_UINT32)shwBufB;
        uwSrcAddrB  = (VOS_UINT32)shwBufA;
    }
    else
    {
        uwSrcAddrA  = (VOS_UINT32)shwBufA;
        uwSrcAddrB  = (VOS_UINT32)shwBufB;
    }

    UCOM_MemSet(shwBufA, 0, uhwLen);
    UCOM_MemSet(shwBufB, 0, uhwLen);

    /* ��ȡDMA����ȫ�ֱ�����Uncache���ʵ�ַ */
    uwLliAddrA      = (VOS_UINT32)AUDIO_PcmGetPlayDmaCfgAPtr();
    uwLliAddrB      = (VOS_UINT32)AUDIO_PcmGetPlayDmaCfgBPtr();

    pstDmaCfgA      = (DRV_DMA_CXCFG_STRU *)(uwLliAddrA);
    pstDmaCfgB      = (DRV_DMA_CXCFG_STRU *)(uwLliAddrB);

    /* ��ʼ��LLI����ĵ�ַ */
    UCOM_MemSet(pstDmaCfgA, 0, AUDIO_PCM_PINGPONG_BUF_NUM * sizeof(DRV_DMA_CXCFG_STRU));

    /* ����DMA���ò���,����Ϊ�������ӣ�ʹ��I2S���������ϲ���ͨ��LOOP���ã�ÿ���ڵ��ϱ�һ���ж� */
    pstDmaCfgA->uwLli            = DRV_DMA_LLI_LINK(uwLliAddrB);
    pstDmaCfgA->uhwACount        = uhwLen;
    pstDmaCfgA->uwSrcAddr        = uwSrcAddrA;
    pstDmaCfgA->uwDstAddr        = DRV_SIO_GetTxRegAddr(DRV_SIO_GetAudioCfgPtr());
    pstDmaCfgA->uwConfig         = DRV_DMA_GetAudioMemSioCfg();

    pstDmaCfgB->uwLli            = DRV_DMA_LLI_LINK(uwLliAddrA);
    pstDmaCfgB->uhwACount        = uhwLen;
    pstDmaCfgB->uwSrcAddr        = uwSrcAddrB;
    pstDmaCfgB->uwDstAddr        = DRV_SIO_GetTxRegAddr(DRV_SIO_GetAudioCfgPtr());
    pstDmaCfgB->uwConfig         = DRV_DMA_GetAudioMemSioCfg();

    /* ������Ƶ����DMAͨ���������ݰ��ˣ���ע����Ƶ����DMA�жϴ����� */
    uwRet = DRV_DMA_StartWithCfg(usChNum,
                                 pstDmaCfgA,
                                 AUDIO_PCM_PlayDmaIsr,
                                 0);

    return uwRet;
}

/*****************************************************************************
 �� �� ��  : AUDIO_PCM_StartCaptureLoopDMA
 ��������  : ������Ƶ�ɼ���DMAͨ��
 �������  : VOS_UINT16 uhwLen          --ÿ�ΰ��˵ĳ���
             VOS_UINT16 usChNum         --���˵�DMAͨ����
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��23��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 AUDIO_PCM_StartCaptureLoopDMA(
                VOS_UINT16 uhwLen,
                VOS_UINT16 usChNum)
{
    VOS_UINT32              uwRet;
    VOS_UINT32              uwDestAddrA, uwDestAddrB;   /* ��Ƶ�ɼ���DMAͨ����Ŀ�ĵ�ַ */
    VOS_UINT32              uwLliAddrA, uwLliAddrB;     /* ��Ƶ�ɼ�DMA���ӵ�ַ������Ϊ���ߵ�ַ */
    VOS_INT16              *pshwCurCBuf = (VOS_VOID *)UCOM_GetUncachedAddr((VOS_UINT32)AUDIO_PCM_GetCurCaptureBuff());
    VOS_INT16              *shwBufA     = (VOS_VOID *)UCOM_GetUncachedAddr((VOS_UINT32)AUDIO_PcmGetCaptureBufAPtr());
    VOS_INT16              *shwBufB     = (VOS_VOID *)UCOM_GetUncachedAddr((VOS_UINT32)AUDIO_PcmGetCaptureBufBPtr());
    DRV_DMA_CXCFG_STRU     *pstDmaCfgA  = VOS_NULL;
    DRV_DMA_CXCFG_STRU     *pstDmaCfgB  = VOS_NULL;

    /* ֹͣ��Ƶ�ɼ���DMA */
    DRV_DMA_Stop(usChNum);

    /* ����DestAddr��ֵ */
    if (pshwCurCBuf == shwBufA)
    {
        uwDestAddrA = (VOS_UINT32)shwBufB;
        uwDestAddrB = (VOS_UINT32)shwBufA;
    }
    else
    {
        uwDestAddrA = (VOS_UINT32)shwBufA;
        uwDestAddrB = (VOS_UINT32)shwBufB;
    }

    UCOM_MemSet(shwBufA, 0, uhwLen);
    UCOM_MemSet(shwBufB, 0, uhwLen);

    /* ��ȡDMA����ȫ�ֱ�����Uncache���ʵ�ַ */
    uwLliAddrA      = UCOM_GetUncachedAddr((VOS_UINT32)AUDIO_PcmGetCaptureDmaCfgAPtr());
    uwLliAddrB      = UCOM_GetUncachedAddr((VOS_UINT32)AUDIO_PcmGetCaptureDmaCfgBPtr());

    pstDmaCfgA      = (DRV_DMA_CXCFG_STRU *)(uwLliAddrA);
    pstDmaCfgB      = (DRV_DMA_CXCFG_STRU *)(uwLliAddrB);

    UCOM_MemSet(pstDmaCfgA, 0, AUDIO_PCM_PINGPONG_BUF_NUM * sizeof(DRV_DMA_CXCFG_STRU));

    /* ����DMA���ò���,����Ϊ�������ӣ�ʹ��I2S���������ϲ���ͨ��LOOP���ã�ÿ���ڵ��ϱ�һ���ж� */
    pstDmaCfgA->uwLli            = DRV_DMA_LLI_LINK(uwLliAddrB);
    pstDmaCfgA->uhwACount        = uhwLen;
    pstDmaCfgA->uwSrcAddr        = DRV_SIO_GetRxRegAddr(DRV_SIO_GetAudioCfgPtr());   /* ����  */
    pstDmaCfgA->uwDstAddr        = uwDestAddrA;
    pstDmaCfgA->uwConfig         = DRV_DMA_GetAudioSioMemCfg();

    pstDmaCfgB->uwLli            = DRV_DMA_LLI_LINK(uwLliAddrA);
    pstDmaCfgB->uhwACount        = uhwLen;
    pstDmaCfgB->uwSrcAddr        = DRV_SIO_GetRxRegAddr(DRV_SIO_GetAudioCfgPtr());   /* ����  */
    pstDmaCfgB->uwDstAddr        = uwDestAddrB;
    pstDmaCfgB->uwConfig         = DRV_DMA_GetAudioSioMemCfg();

    /* ������Ƶ�ɼ�DMAͨ���������ݰ��ˣ���ע����Ƶ�ɼ�DMA�жϴ����� */
    uwRet = DRV_DMA_StartWithCfg(usChNum,
                                 pstDmaCfgA,
                                 AUDIO_PCM_CaptureDmaIsr,
                                 0);

    return uwRet;
}

/*****************************************************************************
 �� �� ��  : AUDIO_PCM_GetCurPlayBuff
 ��������  : ����DMA״̬��ȡ����ƹ�һ���ĵ�ǰ״̬������ǰ�ɴ����ĸ�Buff����
             һ���ڽ���DMA����
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_INT16*
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��12��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_INT16* AUDIO_PCM_GetCurPlayBuff(VOS_VOID)
{
    VOS_UINT32              uwSrcAddr;              /* ��Ƶ���ŵ�DMAͨ����Դ��ַ */
    VOS_UINT32              uwSrcAddrB;             /* ��Ƶ����ƹ��Buff��ַ */
    AUDIO_PCM_CFG_STRU     *pstPcmCfg   =   AUDIO_PcmGetPcmCfgPtr(AUDIO_PCM_MODE_PLAYBACK);

    uwSrcAddrB  = (VOS_UINT32)AUDIO_PcmGetPlayBufBPtr();

    /* ��ȡͨ��1Դ��ַ�Ĵ��� */
    uwSrcAddr   = UCOM_RegRd(DRV_DMA_GetCxSrcAddr(AUDIO_PCM_DMAC_CHN_PLAY));

    /* ��Դ��ַ�Ѿ�ΪBufferB��Χ��(LOOPģʽ)��Դ��ַΪBufferA�����һ��λ��(��LOOPģʽ) */
    /* ���߼�������BufferA��BufferB��ַ��������BufferA��ǰ */
    if((uwSrcAddr >= (uwSrcAddrB - sizeof(VOS_UINT32)))
        &&(uwSrcAddr < (uwSrcAddrB + (pstPcmCfg->uwBufSize - sizeof(VOS_UINT32)))))
    {
        return AUDIO_PcmGetPlayBufAPtr();
    }
    else
    {
        return AUDIO_PcmGetPlayBufBPtr();
    }

}

/*****************************************************************************
 �� �� ��  : AUDIO_PCM_GetCurCaptureBuff
 ��������  : ����DMA״̬��ȡ¼��ƹ�һ���ĵ�ǰ״̬������ǰ�ɴ����ĸ�Buff����
             һ����DMAռ��
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_INT16*
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��12��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_INT16* AUDIO_PCM_GetCurCaptureBuff(VOS_VOID)
{
    VOS_UINT32              uwDestAddr;     /* ��Ƶ¼����DMAͨ����Ŀ�ĵ�ַ */
    VOS_UINT32              uwDestAddrB;    /* ��Ƶ¼����DMAͨ����Ŀ�ĵ�ַ */
    AUDIO_PCM_CFG_STRU     *pstPcmCfg   =   AUDIO_PcmGetPcmCfgPtr(AUDIO_PCM_MODE_CAPTURE);

    uwDestAddrB = UCOM_GetUncachedAddr((VOS_UINT32)AUDIO_PcmGetCaptureBufBPtr());

    /* ��ȡ��Ӧͨ����Ŀ�ĵ�ַ�Ĵ��� */
    uwDestAddr  = UCOM_RegRd(DRV_DMA_CX_DES_ADDR(AUDIO_PCM_DMAC_CHN_CAPTURE));

    /* ��Ŀ�ĵ�ַ�Ѿ�ΪBufferB��Χ��(LOOPģʽ)��Ŀ�ĵ�ַΪBufferA�����һ��λ��(��LOOPģʽ) */
    /* ���߼�������BufferA��BufferB��ַ��������BufferA��ǰ */
    if ( (uwDestAddr >= (uwDestAddrB - sizeof(VOS_UINT32)))
        &&(uwDestAddr < (uwDestAddrB + (pstPcmCfg->uwBufSize - sizeof(VOS_UINT32)))))
    {
        return AUDIO_PcmGetCaptureBufAPtr();
    }
    else
    {
        return AUDIO_PcmGetCaptureBufBPtr();
    }

}


/*****************************************************************************
 �� �� ��  : AUDIO_PCM_MsgUpdatePlayBuffCmd
 ��������  : ������ϢID_AUDIO_UPDATE_PLAY_BUFF_CMD,���²���ƹ��Buff����
 �������  : VOS_VOID *pvOsaMsg
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��12��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 AUDIO_PCM_MsgUpdatePlayBuffCmd(VOS_VOID *pvOsaMsg)

{
    VOS_INT16                          *pshwPlayBuf = VOS_NULL;
    VOS_INT16                          *pshwTmpBuf  = VOS_NULL;
    AUDIO_PCM_CFG_STRU                 *pstPcmCfg   = VOS_NULL ;
    VOS_UINT32                          uwIndex     = 0;
    AUDIO_PCM_UPDATE_PLAY_BUFF_CMD_STRU stUpdateBuffCmd;

    OM_CPUVIEW_EnterArea((VOS_UCHAR)OM_CPUVIEW_AREA_AUDIO_PCM_UPDATE_BUFF_PLAY);

    /* ����AUDIO_PCM_GetCurPlayBuff��ȡ��ǰ�ɴ������Ƶƹ��Buff��ַ */
    pshwPlayBuf = AUDIO_PCM_GetCurPlayBuff();

    /* ��ȡ������ʱ���� */
    pshwTmpBuf  = AUDIO_PcmGetPlayTmpBufPtr();

    pstPcmCfg   = AUDIO_PcmGetPcmCfgPtr(AUDIO_PCM_MODE_PLAYBACK);

    /* ��յ�ǰ����buf��ȷ��Uncache���� */
    UCOM_MemSet((VOS_VOID *)pshwPlayBuf,
                0,
                pstPcmCfg->uwBufSize);

    /* ��յ�ǰ����Tmpbuf */
    UCOM_MemSet((VOS_VOID *)pshwTmpBuf,
                0,
                pstPcmCfg->uwBufSize);

    /* Hifi�ಥ��ͨ��Buff״̬ΪActive��˵��Hifi�ṩ��Buff����������Ҫ���� */
    if (AUDIO_PCM_STATUS_ACTIVE == pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_HIFI].enBufStatus)
    {
        if (AUDIO_PCM_BUFF_ISREADY_YES == pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_HIFI].enBufReadyFlag)
        {
            /* ��HIFI player����Ƶ���ݷ�����ʱbuffer */
            UCOM_MemCpy((VOS_VOID *)pshwTmpBuf,
                        pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_HIFI].uwBufAddr,
                        pstPcmCfg->uwBufSize);

            /* ��buff��־λ�ָ� */
            pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_HIFI].enBufReadyFlag = AUDIO_PCM_BUFF_ISREADY_NO;
        }
        else
        {
            OM_LogError1(AUDIO_PCM_PlayBackBuffIsNotReady,
                         pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_HIFI].enBufReadyFlag);
        }

        /* Hifi����ͨ��Buff�ص������ǿգ������ */
        if (VOS_NULL !=pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_HIFI].pfunCallBack)
        {
            /* ���ûص����������ΪԴBuff�����в����ʱ任��ͨ��ƥ���жϼ�ת������������tmpBuf�� */
            pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_HIFI].pfunCallBack(pshwTmpBuf,
                                                                  pshwTmpBuf,
                                                                  pstPcmCfg->uwSampleRate,
                                                                  pstPcmCfg->uwChannelNum,
                                                                  pstPcmCfg->uwBufSize);
        }

        /* ������ϢID_AUDIO_UPDATE_PLAYER_BUFF_CMD���в���ͨ����Buff���ݸ��� */
        stUpdateBuffCmd.uhwMsgId    = ID_AUDIO_UPDATE_PLAYER_BUFF_CMD;

        /* ����ͨ��VOS���ͽӿڣ�������Ϣ */
        UCOM_SendOsaMsg(DSP_PID_AUDIO_RT,
                        DSP_PID_AUDIO,
                       &stUpdateBuffCmd,
                        sizeof(AUDIO_PCM_UPDATE_PLAY_BUFF_CMD_STRU));
    }

    /*AP�ಥ��ͨ��Buff״̬ΪActive��˵��AP�ṩ��Buff����������Ҫ����*/
    if (AUDIO_PCM_STATUS_ACTIVE == pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_AP].enBufStatus)
    {
        AUDIO_PCM_PERIOD_ELAPSED_CMD_STRU       stMsgPeriodElapsed;

        if (AUDIO_PCM_BUFF_ISREADY_YES == pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_AP].enBufReadyFlag)
        {
            /* ��buff��־λ�ָ� */
            pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_AP].enBufReadyFlag = AUDIO_PCM_BUFF_ISREADY_NO;
        }
        else
        {
            OM_LogError1(AUDIO_PCM_PlayBackBuffIsNotReady,
                         pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_AP].enBufReadyFlag);
        }

        /* pshwTmpBuf��AP����л�������ʱ�����������ȷ��Uncache����������Դ */
        CODEC_OpVvAdd(pshwTmpBuf,
                      (VOS_INT16 *)(pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_AP].uwBufAddr),
                      pstPcmCfg->uwBufSize>>1,
                      pshwTmpBuf);

        /* ����AP����������, add by tonglei */
        AUDIO_DEBUG_SendHookInd(AUDIO_HOOK_APIN_PCM,
                      (VOS_VOID *)pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_AP].uwBufAddr,
                      pstPcmCfg->uwBufSize);
        /* add end */
        stMsgPeriodElapsed.uhwMsgId     = ID_AUDIO_AP_PCM_PERIOD_ELAPSED_CMD;
        stMsgPeriodElapsed.enPcmMode    = AUDIO_PCM_MODE_PLAYBACK;
        stMsgPeriodElapsed.pSubStream   = pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_AP].pSubStream;

        /* ���ÿ����Ƶchannel�ӿ�,֪ͨAP,PERIOD�������,����snd_pcm_period_elapsed,����Buffer */
        UCOM_SendAudioMsg(DSP_PID_AUDIO,
                          ACPU_PID_OM,           /* �˴���дAP��DRV��Ӧ��PID,,���� */
                          &stMsgPeriodElapsed,
                          sizeof(AUDIO_PCM_PERIOD_ELAPSED_CMD_STRU));


    }

    /* AP����ͨ��Hifi�ص�����������ͨ������ص������ǿգ�����ý���Ƶ�ײ�����������Ч�� */
    if (VOS_NULL !=pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_AP].pfunCallBack)
    {
        /* ���ûص����������Ϊ��ǰ����Buff������Ƶ�ײ�����������Ч�� */
        pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_AP].pfunCallBack(pshwTmpBuf,
                                                            pshwTmpBuf,
                                                            pstPcmCfg->uwSampleRate,
                                                            pstPcmCfg->uwChannelNum,
                                                            pstPcmCfg->uwBufSize);
    }

    /* ��ȡDMA���˳�ȥ������ */
    AUDIO_DEBUG_SendHookInd(AUDIO_HOOK_CODECOUT_PCM, pshwTmpBuf, pstPcmCfg->uwBufSize);

    /* ����sio��������ȡ�����������������෴,��˸�16bit���16bit������ȷ��Uncacheд��DMA������ */
    for(uwIndex = 0; uwIndex < ((pstPcmCfg->uwBufSize)/2);uwIndex+=2)
    {
        pshwPlayBuf[uwIndex]   = pshwTmpBuf[uwIndex+1];
        pshwPlayBuf[uwIndex+1] = pshwTmpBuf[uwIndex];
    }

    OM_CPUVIEW_ExitArea((VOS_UCHAR)OM_CPUVIEW_AREA_AUDIO_PCM_UPDATE_BUFF_PLAY);

    return UCOM_RET_SUCC;
}

/*****************************************************************************
 �� �� ��  : AUDIO_PCM_MsgUpdateCaptureBuffCmd
 ��������  : ������ϢID_AUDIO_UPDATE_CAPTURE_BUFF_CMD,����¼��ƹ��Buff
             ����
 �������  : VOS_VOID *pvOsaMsg     --ID_AUDIO_UPDATE_CAPTURE_BUFF_CMD��Ϣ��
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��14��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 AUDIO_PCM_MsgUpdateCaptureBuffCmd(VOS_VOID *pvOsaMsg)
{
    VOS_INT16                          *pshwCaptureBuf  = VOS_NULL;
    VOS_INT16                          *pshwTmpBuf      = VOS_NULL;
    AUDIO_PCM_CFG_STRU                 *pstPcmCfg       = VOS_NULL;
    VOS_UINT32                          uwIndex         = 0;

    OM_CPUVIEW_EnterArea((VOS_UCHAR)OM_CPUVIEW_AREA_AUDIO_PCM_UPDATE_BUFF_CAPTURE);

    /* ����AUDIO_PCM_GetCurCaptureBuff��ȡ��ǰ�Ѱ�����ϵ�Buff */
    pshwCaptureBuf  = (VOS_INT16*)UCOM_GetUncachedAddr((VOS_UINT32)AUDIO_PCM_GetCurCaptureBuff());

    /* ��ȡ¼����ʱ���� */
    pshwTmpBuf      = AUDIO_PcmGetCaptureTmpBufPtr();

    pstPcmCfg       = AUDIO_PcmGetPcmCfgPtr(AUDIO_PCM_MODE_CAPTURE);

    /* ����sio��������ȡ�����������������෴,��˸�16bit���16bit������ȷ��Uncacheд��DMA������ */
    for(uwIndex = 0; uwIndex < ((pstPcmCfg->uwBufSize)/2);uwIndex+=2)
    {
        pshwTmpBuf[uwIndex]   = pshwCaptureBuf[uwIndex+1];
        pshwTmpBuf[uwIndex+1] = pshwCaptureBuf[uwIndex];
    }

    /* �����ͨ��¼����ע�ắ����Ϊ�գ���¼��������pp������������������䣬֮ǰ���ݷ��� */
    if (VOS_NULL !=pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_HIFI].pfunVcRecordCB)
    {
        pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_HIFI].pfunVcRecordCB(pshwTmpBuf,
                                                              pshwTmpBuf,
                                                              pstPcmCfg->uwSampleRate,
                                                              pstPcmCfg->uwChannelNum,
                                                              pstPcmCfg->uwBufSize);
    }
    else
    {
        /* AP��ͨ·�Ļص�����Ŀ�����ڽ����������ơ�Ƶ�첹���ȣ����ǿ������ */
        if (VOS_NULL !=pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_AP].pfunCallBack)
        {
            /* ���ûص����������ΪTmpBuff�����н�������tmpBuf�У���ͬ��¼����
            �����ص�������������ݲ�ͬ�����磬������ͨ��¼�����У��˻ص�����
            �轫�����л���������䵽tmpBuf��;��ͨ��Ƶ¼��ʱ��Ҫ���е�MIC�����*/
            pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_AP].pfunCallBack(pshwTmpBuf,
                                                                pshwTmpBuf,
                                                                pstPcmCfg->uwSampleRate,
                                                                pstPcmCfg->uwChannelNum,
                                                                pstPcmCfg->uwBufSize);
        }
    }

    /*AP��¼��ͨ��Buff״̬ΪActive��˵����Ҫ��AP�ṩ��Buff�����¼������*/
    if (AUDIO_PCM_STATUS_ACTIVE == pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_AP].enBufStatus)
    {
        AUDIO_PCM_PERIOD_ELAPSED_CMD_STRU       stMsgPeriodElapsed;

        /* ��¼�����Uncacheд��AP�ṩ��Buff�� */
        UCOM_MemCpy((VOS_VOID *)(pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_AP].uwBufAddr),
                    pshwTmpBuf,
                    pstPcmCfg->uwBufSize);

        /* ��ȡ¼������*/
        AUDIO_DEBUG_SendHookInd(AUDIO_HOOK_MICIN_PCM, pshwTmpBuf, pstPcmCfg->uwBufSize);

        stMsgPeriodElapsed.uhwMsgId     = ID_AUDIO_AP_PCM_PERIOD_ELAPSED_CMD;
        stMsgPeriodElapsed.enPcmMode    = AUDIO_PCM_MODE_CAPTURE;
        stMsgPeriodElapsed.pSubStream   = pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_AP].pSubStream;

        /* ���ÿ����Ƶchannel�ӿ�,֪ͨAP,PERIOD¼�����,����snd_pcm_period_elapsed,����Buffer */
        UCOM_SendAudioMsg(DSP_PID_AUDIO,
                          ACPU_PID_OM,           /* �˴���дAP��DRV��Ӧ��PID,,���� */
                          &stMsgPeriodElapsed,
                          sizeof(AUDIO_PCM_PERIOD_ELAPSED_CMD_STRU));
    }
    /*Hifi��¼��ͨ��Buff״̬ΪActive��˵����Ҫ��Hifi�ṩ��Buff�����¼������*/
    else
    {
        if (AUDIO_PCM_STATUS_ACTIVE == pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_HIFI].enBufStatus)
        {
            if (VOS_NULL !=pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_HIFI].pfunCallBack)
            {
                /* ���ûص����������ΪTmpBuff�����н�������tmpBuf�У���Ҫ���б������*/
                pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_HIFI].pfunCallBack(pshwTmpBuf,
                                                                      pshwTmpBuf,
                                                                      pstPcmCfg->uwSampleRate,
                                                                      pstPcmCfg->uwChannelNum,
                                                                      pstPcmCfg->uwBufSize);
            }

            /* ��¼�����д��Hifi�ṩ��Buff�� */
            UCOM_MemCpy((VOS_VOID *)pstPcmCfg->astPcmBuf[AUDIO_PCM_OBJ_HIFI].uwBufAddr,
                        pshwTmpBuf,
                        pstPcmCfg->uwBufSize);

            /* ����VOS��Ϣ��֪ͨHifi Recorder���б��뼰Buff���� */
        }
    }

    OM_CPUVIEW_ExitArea((VOS_UCHAR)OM_CPUVIEW_AREA_AUDIO_PCM_UPDATE_BUFF_CAPTURE);

    return UCOM_RET_SUCC;

}

/*****************************************************************************
 �� �� ��  : AUDIO_PCM_PlayDmaIsr
 ��������  : ������Ƶ�����DMA�жϣ����Ͳ���Buff������Ϣ
 �������  : DRV_DMA_INT_TYPE_ENUM_UINT16   enIntType   --DMA�ж�����
             VOS_UINT32                     uwPara      --�жϻص���������
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��10��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID AUDIO_PCM_PlayDmaIsr(
                DRV_DMA_INT_TYPE_ENUM_UINT16    enIntType,
                VOS_UINT32                      uwPara)
{
    AUDIO_PCM_UPDATE_PLAY_BUFF_CMD_STRU         stUpdateBuffCmd;

    /* ���ж�����ΪTC�ж�,Ϊ��Ƶ����DMA�����жϴ������� */
    if ((  DRV_DMA_INT_TYPE_TC1 == enIntType)
        ||(DRV_DMA_INT_TYPE_TC2 == enIntType))
    {

        /* ������ϢID_AUDIO_UPDATE_PLAY_BUFF_CMD���в���ͨ����Buff���ݸ��� */
        stUpdateBuffCmd.uhwMsgId    = ID_AUDIO_UPDATE_PLAY_BUFF_CMD;

        /* ����ͨ��VOS���ͽӿڣ�������Ϣ */
        UCOM_SendOsaMsg(DSP_PID_AUDIO_RT,
                        DSP_PID_AUDIO,
                        &stUpdateBuffCmd,
                        sizeof(AUDIO_PCM_UPDATE_PLAY_BUFF_CMD_STRU));

    }
    /* ���ж�����ΪERROR�ж�,��¼�쳣 */
    else
    {
        /*��¼�쳣������DMA Error�ж�*/
        OM_LogError1(AUDIO_PCM_PlayDmaIsr_ErrInt, enIntType);
    }
}

/*****************************************************************************
 �� �� ��  : AUDIO_PCM_CaptureDmaIsr
 ��������  : ������Ƶ�����DMA�ж�
 �������  : DRV_DMA_INT_TYPE_ENUM_UINT16   enIntType   --DMA�ж�����
             VOS_UINT32                     uwPara      --�жϻص���������
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��7��10��
    ��    ��   : C00137131
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID AUDIO_PCM_CaptureDmaIsr(
                DRV_DMA_INT_TYPE_ENUM_UINT16    enIntType,
                VOS_UINT32                      uwPara)
{
    AUDIO_PCM_UPDATE_CAPTURE_BUFF_CMD_STRU         stUpdateBuffCmd;

    /* ���ж�����ΪTC�ж�,Ϊ��Ƶ¼��DMA�����жϴ������� */
    if ((  DRV_DMA_INT_TYPE_TC1 == enIntType)
        ||(DRV_DMA_INT_TYPE_TC2 == enIntType))
    {

        /* ������ϢID_AUDIO_UPDATE_CAPTURE_BUFF_CMD���в���ͨ����Buff���ݸ��� */
        stUpdateBuffCmd.uhwMsgId    = ID_AUDIO_UPDATE_CAPTURE_BUFF_CMD;

         /* ����ͨ��VOS���ͽӿڣ�������Ϣ */
        UCOM_SendOsaMsg(DSP_PID_AUDIO_RT,
                        DSP_PID_AUDIO,
                        &stUpdateBuffCmd,
                        sizeof(AUDIO_PCM_UPDATE_CAPTURE_BUFF_CMD_STRU));

    }
    /* ���ж�����ΪERROR�ж�,��¼�쳣 */
    else
    {
        /*��¼�쳣������DMA Error�ж�*/
        OM_LogError1(AUDIO_PCM_CaptureDmaIsr_ErrInt, enIntType);
    }
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif



