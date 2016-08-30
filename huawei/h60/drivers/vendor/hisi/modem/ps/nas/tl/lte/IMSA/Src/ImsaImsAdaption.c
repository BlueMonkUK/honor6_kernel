/******************************************************************************

   Copyright(C)2013,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : ImsaImsAdaption.c
  Description     : ��C�ļ�ʵ��IMS��ص����书�ܣ����ַ�ת����
  History           :
     1.sunbing 49683      2013-06-19  Draft Enact

******************************************************************************/

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "ImsaImsAdaption.h"
#include "ImsaPublic.h"
#include "ImsaImsInterface.h"
#include "ImsaImsApi.h"
#include "VcImsaInterface.h"
#include "ImsaProcImsRegMsg.h"
#include "ImsCodecInterface.h"
#include "ImsaProcImsCallMsg.h"
#include "ImsaProcImsSmsMsg.h"
#include "ImsaImsInterface.h"
#include "MnClient.h"
#include "NasCommPrint.h"
/* modify by jiqiang 2014.03.25 pclint fix error begin */
#include "ImsaProcImsUssdMsg.h"
/* modify by jiqiang 2014.03.25 pclint fix error end */

#include "ImsaProcAtMsg.h"
#include "ImsaRegManagement.h"
/*lint -e767*/
#define    THIS_FILE_ID    PS_FILE_ID_IMSAIMSADAPTION_C
/*lint +e767*/

/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (FEATURE_ON == FEATURE_IMS)
/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/
HTIMER    aphTimer[IMSA_IMS_TIMER_NAME_BUTT] = {0};


extern VOS_VOID IMSA_ImsMsgDistr(const VOS_VOID *pRcvMsg );
extern VOS_VOID IMSA_SndVcStartHifiMsg(IMSA_VOICE_CODEC_ENUM_UINT16 enCodecType );
extern VOS_VOID IMSA_SndVcChangeHifiParaMsg(IMSA_VOICE_CODEC_ENUM_UINT16 enCodecType );
extern VOS_VOID IMSA_SndVcStopHifiMsg(VOS_VOID);
extern VOS_VOID IMSA_SndSpmOpenCodecChannel(IMSA_VOICE_CODEC_ENUM_UINT16 enCodecType);
extern VOS_VOID IMSA_SndSpmCloseCodecChannel(VOS_VOID);


/*****************************************************************************
  3 Function
*****************************************************************************/
/*lint -e960*/
/*lint -e961*/
/*****************************************************************************
 Function Name  : IMSA_ImsaIntraMsgDistr()
 Description    : IMS��Ϣ�ַ�������ͨ��IMS��Ϣ���¼����жϷַ�����ͬ���ڲ�ģ�鴦��
 Input          : VOS_VOID *pRcvMsg
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-06-20  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_ImsMsgDistr(const VOS_VOID *pRcvMsg )
{
    /* ������Ϣͷָ��*/
    ImsaImsInterface_MSG          *pHeader = VOS_NULL_PTR;

    /* ��ȡ��Ϣͷָ��*/
    pHeader = (ImsaImsInterface_MSG *) pRcvMsg;

    /*�����¼�����*/
    switch(pHeader->stMsgData.ulMsgId)
    {
        case ID_IMS_IMSA_OUTPUT_SERVICE_MSG:
            IMSA_ImsMsgServiceEvent(&pHeader->stMsgData.aucMsg[0]);
            break;

        case ID_IMS_IMSA_OUTPUT_CALL_MSG:
            IMSA_ImsMsgCallEvent(&pHeader->stMsgData.aucMsg[0]);
            break;

        case ID_IMS_IMSA_OUTPUT_SMS_MSG:
            IMSA_ImsMsgSmsEvent(&pHeader->stMsgData.aucMsg[0]);
            break;

        case ID_IMS_IMSA_OUTPUT_SYSTEM_MSG:
            IMSA_ImsMsgSystemEvent(&pHeader->stMsgData.aucMsg[0]);
            break;

        case ID_IMS_IMSA_OUTPUT_PARA_MSG:
            IMSA_ImsMsgParaEvent(&pHeader->stMsgData.aucMsg[0]);
            break;

        case ID_IMS_IMSA_OUTPUT_USSD_MSG:
            IMSA_ImsMsgUssdEvent(&pHeader->stMsgData.aucMsg[0]);
            break;

        case ID_IMS_IMSA_OUTPUT_NV_INFO_MSG:
            IMSA_ImsMsgNvInfoEvent(&pHeader->stMsgData.aucMsg[0]);
            break;


        default:
            break;

    }
}
VOS_VOID IMSA_ProcHifiMsgTxDataInd
(
    const VOICE_IMSA_TX_DATA_IND_STRU  *pstTxDataInd
)
{
    IMSA_TX_VOICE_PARA_STRU             stTxPara = {0};

    IMSA_INFO_LOG("IMSA_ProcHifiMsgTxDataInd: enter!");

    stTxPara.usFrameType = pstTxDataInd->usFrameType;
    stTxPara.usStatus    = pstTxDataInd->usStatus;

    IMSA_Send2ImsTxVoiceData(&stTxPara,pstTxDataInd->ausData);
}


VOS_VOID IMSVA_ProcHifiMsg(const VOS_VOID *pRcvMsg )
{
    /* ������Ϣͷָ��*/
    PS_MSG_HEADER_STRU          *pHeader = VOS_NULL_PTR;

    /* ��ȡ��Ϣͷָ��*/
    pHeader = (PS_MSG_HEADER_STRU *) pRcvMsg;

    switch(pHeader->ulMsgName)
    {
        case ID_VOICE_IMSA_TX_DATA_IND:
            IMSA_ProcHifiMsgTxDataInd((VOICE_IMSA_TX_DATA_IND_STRU*)pRcvMsg);
            break;

        case ID_VOICE_IMSA_CFG_CNF:

            IMSA_INFO_LOG1("IMSA_VOICE_CFG_CNF,result=",((VOICE_IMSA_CFG_CNF_STRU*)pRcvMsg)->ulResult);
            break;

        default:

            /*��ӡ�쳣��Ϣ*/
            IMSA_WARN_LOG("IMSVA_ProcHifiMsg:Msg Id is err!");
            break;
    }
}
VOS_VOID IMSA_Snd2ImsaCallMsg
(
    const IMSA_IMS_OUTPUT_CALL_EVENT_STRU      *pstOutputCallEvent
)
{
    IMS_IMSA_OUTPUT_CALL_MSG_STRU      *pstMsgOutputCall = VOS_NULL_PTR;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstMsgOutputCall = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMS_IMSA_OUTPUT_CALL_MSG_STRU));

    if ( VOS_NULL_PTR == pstMsgOutputCall )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_Snd2ImsaCallMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgOutputCall), 0, \
               IMSA_GET_MSG_LENGTH(pstMsgOutputCall));

    /*��д��Ϣͷ*/
    IMSA_WRITE_IMSA_MSG_HEAD(pstMsgOutputCall,ID_IMS_IMSA_OUTPUT_CALL_MSG);

    IMSA_MEM_CPY(&pstMsgOutputCall->stOutputCall, \
                 pstOutputCallEvent, \
                 sizeof(IMSA_IMS_OUTPUT_CALL_EVENT_STRU));

    IMSA_SND_MSG(pstMsgOutputCall);
}

/*****************************************************************************
 Function Name  : IMSA_Snd2ImsaSmsMsg()
 Description    : ��IMSA����SMS��Ϣ
 Input          : pstHicsmOutputSms
 Output         : VOS_VOID
 Return Value   : unsigned int ���ؽ�� 0�ɹ���1ʧ��

 History        :
      1.sunbing 49683      2013-07-16  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_Snd2ImsaSmsMsg(const IMSA_IMS_OUTPUT_SMS_EVENT_STRU *pstImsaImsOutputSms)
{
    IMS_IMSA_OUTPUT_SMS_MSG_STRU                *pstMsgOutputSms;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstMsgOutputSms = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMS_IMSA_OUTPUT_SMS_MSG_STRU));

    if ( VOS_NULL_PTR == pstMsgOutputSms )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_Snd2ImsaSmsMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgOutputSms), 0, \
               IMSA_GET_MSG_LENGTH(pstMsgOutputSms));

    /*��д��Ϣͷ*/
    IMSA_WRITE_IMSA_MSG_HEAD(pstMsgOutputSms,ID_IMS_IMSA_OUTPUT_SMS_MSG);

    IMSA_MEM_CPY(&pstMsgOutputSms->stOutputSms, \
                 pstImsaImsOutputSms, \
                 sizeof(IMSA_IMS_OUTPUT_SMS_EVENT_STRU));

    IMSA_SND_MSG(pstMsgOutputSms);

}

/*****************************************************************************
 Function Name  : IMSA_Snd2ImsaServiceMsg()
 Description    : ��IMSA����SERVCIE��Ϣ
 Input          : pstHicsmOutputService
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-07-16  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_Snd2ImsaServiceMsg(const IMSA_IMS_OUTPUT_SERVICE_EVENT_STRU *pstOutputService)
{
    IMS_IMSA_OUTPUT_SERVICE_MSG_STRU      *pstMsgOutputService;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstMsgOutputService = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMS_IMSA_OUTPUT_SERVICE_MSG_STRU));

    if ( VOS_NULL_PTR == pstMsgOutputService )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_Snd2ImsaServiceMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgOutputService), 0, \
               IMSA_GET_MSG_LENGTH(pstMsgOutputService));

    /*��д��Ϣͷ*/
    IMSA_WRITE_IMSA_MSG_HEAD(pstMsgOutputService,ID_IMS_IMSA_OUTPUT_SERVICE_MSG);

    IMSA_MEM_CPY(&pstMsgOutputService->stOutputService, \
                 pstOutputService, \
                 sizeof(IMSA_IMS_OUTPUT_SERVICE_EVENT_STRU));

    IMSA_SND_MSG(pstMsgOutputService);

}


VOS_VOID IMSA_Snd2ImsaParaMsg(const IMSA_IMS_OUTPUT_PARA_EVENT_STRU *pstOutputService)
{
    IMS_IMSA_OUTPUT_PARA_MSG_STRU      *pstMsgOutputPara;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstMsgOutputPara = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMS_IMSA_OUTPUT_PARA_MSG_STRU));

    if ( VOS_NULL_PTR == pstMsgOutputPara )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_Snd2ImsaServiceMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgOutputPara), 0, \
               IMSA_GET_MSG_LENGTH(pstMsgOutputPara));

    /*��д��Ϣͷ*/
    IMSA_WRITE_IMSA_MSG_HEAD(pstMsgOutputPara,ID_IMS_IMSA_OUTPUT_PARA_MSG);

    IMSA_MEM_CPY(&pstMsgOutputPara->stOutputPara, \
                 pstOutputService, \
                 sizeof(IMSA_IMS_OUTPUT_PARA_EVENT_STRU));

    IMSA_SND_MSG(pstMsgOutputPara);

}


VOS_VOID IMSA_Snd2ImsaSystemMsg(const IMSA_IMS_OUTPUT_SYSTEM_EVENT_STRU *pstOutputSystem)
{
    IMS_IMSA_OUTPUT_SYSTEM_MSG_STRU      *pstMsgOutputSystem;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstMsgOutputSystem = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMS_IMSA_OUTPUT_SYSTEM_MSG_STRU));

    if ( VOS_NULL_PTR == pstMsgOutputSystem )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_Snd2ImsaServiceMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgOutputSystem), 0, \
               IMSA_GET_MSG_LENGTH(pstMsgOutputSystem));

    /*��д��Ϣͷ*/
    IMSA_WRITE_IMSA_MSG_HEAD(pstMsgOutputSystem,ID_IMS_IMSA_OUTPUT_SYSTEM_MSG);

    IMSA_MEM_CPY(&pstMsgOutputSystem->stOutputSystem, \
                 pstOutputSystem, \
                 sizeof(IMSA_IMS_OUTPUT_SYSTEM_EVENT_STRU));

    IMSA_SND_MSG(pstMsgOutputSystem);

}


#if 0
/*****************************************************************************
 Function Name  : IMSA_Snd2ImsaSupSrvMsg()
 Description    : ��IMSA����SupSrv��Ϣ
 Input          : pstHicsmOutputSupSrv
 Output         : VOS_VOID
 Return Value   : unsigned int ���ؽ�� 0�ɹ���1ʧ��

 History        :
      1.sunbing 49683      2013-07-16  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_Snd2ImsaSupSrvMsg(const HICSM_OutputSupSrv *pstHicsmOutputSupSrv)
{
    IMS_IMSA_OUTPUT_SUPSRV_EVENT_STRU      *pstMsgOutputSupSrv;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstMsgOutputSupSrv = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMS_IMSA_OUTPUT_SUPSRV_EVENT_STRU));

    if ( VOS_NULL_PTR == pstMsgOutputSupSrv )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_Snd2ImsaSupSrvMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgOutputSupSrv), 0, \
               IMSA_GET_MSG_LENGTH(pstMsgOutputSupSrv));

    /*��д��Ϣͷ*/
    IMSA_WRITE_IMSA_MSG_HEAD(pstMsgOutputSupSrv,ID_IMS_IMSA_OUTPUT_SUPSRV_EVENT);

    IMSA_MEM_CPY(&pstMsgOutputSupSrv->stOutputSupSrv, \
                 pstHicsmOutputSupSrv, \
                 sizeof(HICSM_OutputSupSrv));

    IMSA_SND_MSG(pstMsgOutputSupSrv);

}

/*****************************************************************************
 Function Name  : IMSA_Snd2ImsaUssdMsg()
 Description    : ��IMSA����USSD��Ϣ
 Input          : pstHicsmOutputUssd
 Output         : VOS_VOID
 Return Value   : unsigned int ���ؽ�� 0�ɹ���1ʧ��

 History        :
      1.sunbing 49683      2013-07-16  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_Snd2ImsaUssdMsg(const HICSM_OutputUssd *pstHicsmOutputUssd)
{
    IMS_IMSA_OUTPUT_USSD_EVENT_STRU      *pstMsgOutputUssd;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstMsgOutputUssd = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMS_IMSA_OUTPUT_USSD_EVENT_STRU));

    if ( VOS_NULL_PTR == pstMsgOutputUssd )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_Snd2ImsaUssdMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgOutputUssd), 0, \
               IMSA_GET_MSG_LENGTH(pstMsgOutputUssd));

    /*��д��Ϣͷ*/
    IMSA_WRITE_IMSA_MSG_HEAD(pstMsgOutputUssd,ID_IMS_IMSA_OUTPUT_USSD_EVENT);

    IMSA_MEM_CPY(&pstMsgOutputUssd->stOutputUssd, \
                 pstHicsmOutputUssd, \
                 sizeof(HICSM_OutputUssd));

    IMSA_SND_MSG(pstMsgOutputUssd);

}
#endif


unsigned int IMSA_Send2ImsaMsg(void *pstOutputEvent)
{
    IMSA_IMS_OUTPUT_EVENT_STRU  *pstImsaImsOutputEvent = pstOutputEvent;

    if( VOS_NULL_PTR == pstImsaImsOutputEvent)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_Snd2ImsaMsg:ERROR:Para is null!");
        return VOS_ERR;
    }

    switch(pstImsaImsOutputEvent->enEventType)
    {
        case IMSA_IMS_EVENT_TYPE_CALL:
            IMSA_Snd2ImsaCallMsg(&pstImsaImsOutputEvent->evt.stOutputCallEvent);
            break;
        case IMSA_IMS_EVENT_TYPE_SMS:
            IMSA_Snd2ImsaSmsMsg(&pstImsaImsOutputEvent->evt.stOutputSmsEvent);
            break;
        case IMSA_IMS_EVENT_TYPE_SERVICE:
            IMSA_Snd2ImsaServiceMsg(&pstImsaImsOutputEvent->evt.stOutputServiceEvent);
            break;

        case IMSA_IMS_EVENT_TYPE_PARA:
            IMSA_Snd2ImsaParaMsg(&pstImsaImsOutputEvent->evt.stOutputParaEvent);
            break;
        case IMSA_IMS_EVENT_TYPE_SYSTEM:
            IMSA_Snd2ImsaSystemMsg(&pstImsaImsOutputEvent->evt.stOutputSystemEvent);
            break;
        default:
            IMSA_WARN_LOG("IMSA_Snd2ImsaMsg:Type is err!");
            break;
    }

    return VOS_OK;

}
VOS_VOID IMSA_SndHifiCfgReq( unsigned long ulTransTime )
{
    IMSA_VOICE_CFG_REQ_STRU                 *pstImsaCfgReq = VOS_NULL_PTR;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstImsaCfgReq = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_VOICE_CFG_REQ_STRU));

    if ( VOS_NULL_PTR == pstImsaCfgReq )
    {
       /*��ӡ�쳣��Ϣ*/
       IMSA_ERR_LOG("IMSA_SndHifiCfgReq:ERROR:Alloc Msg fail!");
       return ;
    }

    pstImsaCfgReq->ulSenderCpuId    = VOS_LOCAL_CPUID;
    pstImsaCfgReq->ulSenderPid      = PS_PID_IMSVA;

    /*CPU ID����Ҫ��д��OSA�����Ŀ��pid�ó�CPU ID���ٸ���CPU ID������Ϣ���͵���Ӧ��CPU��*/
    pstImsaCfgReq->ulReceiverCpuId  = VOS_LOCAL_CPUID;
    pstImsaCfgReq->ulReceiverPid    = DSP_PID_VOICE;

    pstImsaCfgReq->usMsgId          = ID_IMSA_VOICE_CFG_REQ;

    pstImsaCfgReq->ulTransTime      = ulTransTime;

    /* ������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstImsaCfgReq);
}


/*****************************************************************************
 Function Name  : IMSA_SndVcStartHifiMsg()
 Description    : ��VC��������HIFI��Ϣ
 Input          : enCodecType
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-07-15  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndVcStartHifiMsg(IMSA_VOICE_CODEC_ENUM_UINT16 enCodecType )
{
    IMSA_VC_START_HIFI_NTF_STRU         *pstStartHifiNtf;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstStartHifiNtf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_VC_START_HIFI_NTF_STRU));

    if ( VOS_NULL_PTR == pstStartHifiNtf )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndVcStartHifiMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*��д��Ϣͷ*/
    IMSA_WRITE_VC_MSG_HEAD(pstStartHifiNtf,ID_IMSA_VC_START_HIFI_NTF);

    pstStartHifiNtf->enMode = IMSA_VC_MODE_EUTRAN_IMS;

    switch(enCodecType)
    {
        case IMSA_VOICE_CODEC_AMR_NB:
        case IMSA_VOICE_CODEC_AMR_NB2:
            pstStartHifiNtf->enCodeType = IMSA_VC_CODEC_TYPE_AMR;
            break;

        case IMSA_VOICE_CODEC_AMR_WB:
            pstStartHifiNtf->enCodeType = IMSA_VC_CODEC_TYPE_AMRWB;
            break;

        case IMSA_VOICE_CODEC_G711:
            pstStartHifiNtf->enCodeType = IMSA_VC_CODEC_TYPE_G711;
            break;

        default:
            IMSA_WARN_LOG("IMSA_SndVcStartHifiMsg:codec type is err!");
            pstStartHifiNtf->enCodeType = IMSA_VC_CODEC_TYPE_AMR;
            break;
    }

    /* ������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstStartHifiNtf);

}

/*****************************************************************************
 Function Name  : IMSA_SndVcChangeHifiParaMsg()
 Description    : ��VC����codec�����Ϣ
 Input          : enCodecType
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-07-15  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndVcChangeHifiParaMsg(IMSA_VOICE_CODEC_ENUM_UINT16 enCodecType )
{
    IMSA_VC_HIFI_PARA_CHANGED_NTF_STRU  *pstHifiParaChangeNtf;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstHifiParaChangeNtf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_VC_HIFI_PARA_CHANGED_NTF_STRU));

    if ( VOS_NULL_PTR == pstHifiParaChangeNtf )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndVcChangeHifiParaMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*��д��Ϣͷ*/
    IMSA_WRITE_VC_MSG_HEAD(pstHifiParaChangeNtf,ID_IMSA_VC_HIFI_PARA_CHANGED_NTF);

    pstHifiParaChangeNtf->enMode = IMSA_VC_MODE_EUTRAN_IMS;

    switch(enCodecType)
    {
        case IMSA_VOICE_CODEC_AMR_NB:
        case IMSA_VOICE_CODEC_AMR_NB2:
            pstHifiParaChangeNtf->enCodeType = IMSA_VC_CODEC_TYPE_AMR;
            break;

        case IMSA_VOICE_CODEC_AMR_WB:
            pstHifiParaChangeNtf->enCodeType = IMSA_VC_CODEC_TYPE_AMRWB;
            break;

        case IMSA_VOICE_CODEC_G711:
            pstHifiParaChangeNtf->enCodeType = IMSA_VC_CODEC_TYPE_G711;
            break;

        default:
            IMSA_WARN_LOG("IMSA_SndVcChangeHifiParaMsg:codec type is err!");
            pstHifiParaChangeNtf->enCodeType = IMSA_VC_CODEC_TYPE_AMR;
            break;
    }

    /* ������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstHifiParaChangeNtf);

}

/*****************************************************************************
 Function Name  : IMSA_SndVcStopHifiMsg()
 Description    : ��VC����ֹͣHIFI��Ϣ
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-08-16  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndVcStopHifiMsg(VOS_VOID)
{
    IMSA_VC_STOP_HIFI_NTF_STRU          *pstStopHifiNtf;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstStopHifiNtf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_VC_STOP_HIFI_NTF_STRU));

    if ( VOS_NULL_PTR == pstStopHifiNtf )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndVcStopHifiMsg:ERROR:Alloc Msg fail!");
        return;
    }

    /*��д��Ϣͷ*/
    IMSA_WRITE_VC_MSG_HEAD(pstStopHifiNtf,ID_IMSA_VC_STOP_HIFI_NTF);

    /* ������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstStopHifiNtf);
}


/*****************************************************************************
 Function Name  : IMSA_SndSpmOpenCodecChannel
 Description    : IMSA��SPM���ʹ�ͨ������Ϣ
 Input          : enCodecType     ��������
 Output         : VOS_VOID
 Return Value   : ���ͽ��

 History        :
      1.sunbing 49683      2013-08-16  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndSpmOpenCodecChannel(IMSA_VOICE_CODEC_ENUM_UINT16 enCodecType)
{
    IMSA_SPM_CHANNEL_INFO_IND_STRU *pstSpmChannelInfoMsg = VOS_NULL_PTR;

    /* ����ռ䲢��������Ƿ�ɹ� */
    pstSpmChannelInfoMsg = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_CHANNEL_INFO_IND_STRU));
    if (VOS_NULL_PTR == pstSpmChannelInfoMsg)
    {
        IMSA_ERR_LOG("IMSA_SndSpmOpenCodecChannel: alloc memory fail");
        return;
    }

    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstSpmChannelInfoMsg), 0,IMSA_GET_MSG_LENGTH(pstSpmChannelInfoMsg));

    /* ��д��Ϣͷ */
    IMSA_WRITE_SPM_MSG_HEAD(pstSpmChannelInfoMsg, ID_IMSA_SPM_CHANNEL_INFO_IND);

    /* ����Client Id */
    pstSpmChannelInfoMsg->usClientId                          = MN_CLIENT_ID_BROADCAST;

    pstSpmChannelInfoMsg->ucIsLocalAlertingFlag               = VOS_FALSE;

    switch(enCodecType)
    {
        case IMSA_VOICE_CODEC_AMR_NB:
            pstSpmChannelInfoMsg->enCodecType = MN_CALL_CODEC_TYPE_AMR;
            break;
        case IMSA_VOICE_CODEC_EFR:
            pstSpmChannelInfoMsg->enCodecType = MN_CALL_CODEC_TYPE_EFR;
            break;
        case IMSA_VOICE_CODEC_FR:
            pstSpmChannelInfoMsg->enCodecType = MN_CALL_CODEC_TYPE_FR;
            break;
        case IMSA_VOICE_CODEC_HR:
            pstSpmChannelInfoMsg->enCodecType = MN_CALL_CODEC_TYPE_HR;
            break;
        case IMSA_VOICE_CODEC_AMR_NB2:
            pstSpmChannelInfoMsg->enCodecType = MN_CALL_CODEC_TYPE_AMR2;
            break;
        case IMSA_VOICE_CODEC_AMR_WB:
            pstSpmChannelInfoMsg->enCodecType = MN_CALL_CODEC_TYPE_AMRWB;
            break;
        case IMSA_VOICE_CODEC_G711:
            pstSpmChannelInfoMsg->enCodecType = IMSA_VOICE_CODEC_G711;
            break;
        default:
            IMSA_WARN_LOG("IMSA_SndSpmOpenCodecChannel:codec type is err!");
            pstSpmChannelInfoMsg->enCodecType = MN_CALL_CODEC_TYPE_AMR;
            break;

    }

    /* lihong00150010 ��ȫ���Ӷ� 2014-04-21 begin */
    /*pstSpmChannelInfoMsg->enChannelEvent                      = MN_CALL_EVT_CHANNEL_OPEN;
    pstSpmChannelInfoMsg->stChannelInfo.bChannelEnable        = VOS_TRUE;
    pstSpmChannelInfoMsg->stChannelInfo.stChannelParam.enMode = MN_CALL_RADIO_MODE_IMS_EUTRAN;

    switch(enCodecType)
    {
        case IMSA_VOICE_CODEC_AMR_NB:
            pstSpmChannelInfoMsg->stChannelInfo.stChannelParam.enCodecType = MN_CALL_CODEC_TYPE_AMR;
            break;

        case IMSA_VOICE_CODEC_EFR:
            pstSpmChannelInfoMsg->stChannelInfo.stChannelParam.enCodecType = MN_CALL_CODEC_TYPE_EFR;
            break;

        case IMSA_VOICE_CODEC_FR:
            pstSpmChannelInfoMsg->stChannelInfo.stChannelParam.enCodecType = MN_CALL_CODEC_TYPE_FR;
            break;

        case IMSA_VOICE_CODEC_HR:
            pstSpmChannelInfoMsg->stChannelInfo.stChannelParam.enCodecType = MN_CALL_CODEC_TYPE_HR;
            break;

        case IMSA_VOICE_CODEC_AMR_NB2:
            pstSpmChannelInfoMsg->stChannelInfo.stChannelParam.enCodecType = MN_CALL_CODEC_TYPE_AMR2;
            break;

        case IMSA_VOICE_CODEC_AMR_WB:
            pstSpmChannelInfoMsg->stChannelInfo.stChannelParam.enCodecType = MN_CALL_CODEC_TYPE_AMRWB;
            break;

        case IMSA_VOICE_CODEC_G711:
            pstSpmChannelInfoMsg->stChannelInfo.stChannelParam.enCodecType = MN_CALL_CODEC_TYPE_G711;
            break;

        default:
            IMSA_WARN_LOG("IMSA_SndSpmOpenCodecChannel:codec type is err!");
            pstSpmChannelInfoMsg->stChannelInfo.stChannelParam.enCodecType = IMSA_VC_CODEC_TYPE_AMR;
            break;
    }*/
    /* lihong00150010 ��ȫ���Ӷ� 2014-04-21 end */

    /* ������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstSpmChannelInfoMsg);
}

/*****************************************************************************
 Function Name  : IMSA_SndSpmCloseCodecChannel
 Description    : IMSA��SPM���͹ر�ͨ������Ϣ
 Input          : enCodecType     ��������
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-08-16  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndSpmCloseCodecChannel(VOS_VOID)
{
    #if 0
    IMSA_SPM_CHANNEL_INFO_IND_STRU *pstSpmChannelInfoMsg = VOS_NULL_PTR;

    /* ����ռ䲢��������Ƿ�ɹ� */
    pstSpmChannelInfoMsg = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_CHANNEL_INFO_IND_STRU));
    if (VOS_NULL_PTR == pstSpmChannelInfoMsg)
    {
        IMSA_ERR_LOG("IMSA_SndSpmCloseCodecChannel: alloc memory fail");
        return;
    }

    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstSpmChannelInfoMsg), 0, IMSA_GET_MSG_LENGTH(pstSpmChannelInfoMsg));

    /* ��д��Ϣͷ */
    IMSA_WRITE_SPM_MSG_HEAD(pstSpmChannelInfoMsg, ID_IMSA_SPM_CHANNEL_INFO_IND);

    /* ����Client Id */
    pstSpmChannelInfoMsg->usClientId         = MN_CLIENT_ID_BROADCAST;
    pstSpmChannelInfoMsg->enChannelEvent     = MN_CALL_EVT_CHANNEL_CLOSE;

    /* ������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstSpmChannelInfoMsg);
    #endif
}


unsigned int IMSA_StartVoiceDsp
(
    IMSA_VOICE_CODEC_ENUM_UINT16    enCodecType,
    unsigned long                   ulPacketTime
)
{
    IMSA_INFO_LOG("IMSA_StartVoiceDsp:the function is entered!");

    /*֪ͨVC������HIFI*/
    IMSA_SndVcStartHifiMsg(enCodecType);

    /*֪ͨVC��codec���*/
    IMSA_SndVcChangeHifiParaMsg(enCodecType);

    /*֪ͨSPM����ģ��codec*/
    IMSA_SndSpmOpenCodecChannel(enCodecType);

    /*�������������Ϣ*/
    IMSA_CallClearLocalAlertInfo();

    /* ֪ͨHIFI�����ý������� */
    IMSA_SndHifiCfgReq(ulPacketTime);

    return VOS_OK;
}

/*****************************************************************************
 Function Name  : IMSA_StopVoiceDsp()
 Description    : ��˼�ṩ��D2 tech��API������D2 IMSЭ��ջ�ڹҶ����е绰�����
                  �ڸ�API�У�IMSA��֪ͨHIFI,ֹͣ���������
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-07-15  Draft Enact
*****************************************************************************/
unsigned int IMSA_StopVoiceDsp(void)
{
    IMSA_INFO_LOG("IMSA_StopVoiceDsp:the function is entered!");

    /*֪ͨVC��ֹͣHIFI*/
    IMSA_SndVcStopHifiMsg();

    /*ֹͣcodecͨ��*/
    /* IMSA_SndSpmCloseCodecChannel(); */

    return VOS_OK;
}

/*****************************************************************************
 Function Name  : IMSA_Send2HifiRxVoiceData()
 Description    : D2IMSЭ��ջ���ã����ڽ������������ݷ��͸�HIFI��HIFI�յ����������
 Input          : usLength ��ʾ�������ݳ��ȣ�pcData���ݴ�ŵ�ַ(�ڴ��ɵ������ͷ�)
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-07-15  Draft Enact
*****************************************************************************/
unsigned int IMSA_Send2HifiRxVoiceData(const IMSA_RX_VOICE_PARA_STRU *pstVoicePara,
                                               const unsigned short *pusData)
{
    IMSA_VOICE_RX_DATA_IND_STRU                 *pstRxDataInd;

    if((VOS_NULL_PTR == pstVoicePara)
        ||(VOS_NULL_PTR == pusData))
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_Send2HifiRxVoiceData:ERROR Input Para!");
        return VOS_ERR;
    }

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstRxDataInd = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_VOICE_RX_DATA_IND_STRU));

    if ( VOS_NULL_PTR == pstRxDataInd )
    {
       /*��ӡ�쳣��Ϣ*/
       IMSA_ERR_LOG("IMSA_Send2HifiRxVoiceData:ERROR:Alloc Msg fail!");
       return VOS_ERR;
    }

    pstRxDataInd->ulSenderCpuId    = VOS_LOCAL_CPUID;
    pstRxDataInd->ulSenderPid      = PS_PID_IMSVA;

    /*CPU ID����Ҫ��д��OSA�����Ŀ��pid�ó�CPU ID���ٸ���CPU ID������Ϣ���͵���Ӧ��CPU��*/
    pstRxDataInd->ulReceiverCpuId  = VOS_LOCAL_CPUID;
    pstRxDataInd->ulReceiverPid    = DSP_PID_VOICE_RT;

    pstRxDataInd->usMsgId          = ID_IMSA_VOICE_RX_DATA_IND;

    pstRxDataInd->usCodecType      = pstVoicePara->enCodecType;
    pstRxDataInd->usDtxEnable      = pstVoicePara->usDtxEnable;
    pstRxDataInd->usErrorFlag      = pstVoicePara->usErrorFlag;
    pstRxDataInd->usRateMode       = pstVoicePara->usRateMode;
    pstRxDataInd->usFrameType      = pstVoicePara->usFrameType;
    pstRxDataInd->usQualityIdx     = pstVoicePara->usQualityIdx;
    pstRxDataInd->usSN             = pstVoicePara->usSN;
    pstRxDataInd->ulTS             = pstVoicePara->ulTS;
    pstRxDataInd->ulSSRC           = pstVoicePara->ulSsrc;
    IMSA_MEM_CPY(pstRxDataInd->ausData, pusData, IMSA_CODEC_MAX_DATA_LEN*sizeof(unsigned short));

    /* ������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstRxDataInd);

    return VOS_OK;
}

/*****************************************************************************
 Function Name  : IMSA_SndD2ImsMsgStopReq()
 Description    : ��IMS���͹ػ���Ϣ
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-07-16  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndImsMsgStopReq(VOS_VOID  )
{
    IMSA_IMS_INPUT_EVENT_STRU                    *pstImsaImsInputEvent;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstImsaImsInputEvent = IMSA_MEM_ALLOC(sizeof(IMSA_IMS_INPUT_EVENT_STRU));

    if ( VOS_NULL_PTR == pstImsaImsInputEvent )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndImsMsgStopReq:ERROR:Alloc Mem fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(pstImsaImsInputEvent, 0, \
                 sizeof(IMSA_IMS_INPUT_EVENT_STRU));

    pstImsaImsInputEvent->enEventType = IMSA_IMS_EVENT_TYPE_SYSTEM;

    pstImsaImsInputEvent->evt.stInputSystemEvent.enInputSysReason = IMSA_IMS_INPUT_SYSTEM_REASON_STOP;

    pstImsaImsInputEvent->evt.stInputSystemEvent.ulOpId = IMSA_AllocImsOpId();

    IMSA_SndImsMsgSystemEvent(pstImsaImsInputEvent);

    /*�ͷ���Ϣ�ռ�*/
    IMSA_MEM_FREE(pstImsaImsInputEvent);

}


/*****************************************************************************
 Function Name  : IMSA_SndD2ImsMsgStartReq()
 Description    : ��IMS���Ϳ�����Ϣ
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-07-16  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndImsMsgStartReq(VOS_VOID  )
{
    IMSA_IMS_INPUT_EVENT_STRU                    *pstImsaImsInputEvent;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstImsaImsInputEvent = IMSA_MEM_ALLOC(sizeof(IMSA_IMS_INPUT_EVENT_STRU));

    if ( VOS_NULL_PTR == pstImsaImsInputEvent )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndImsMsgStartReq:ERROR:Alloc Mem fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(pstImsaImsInputEvent, 0, \
                 sizeof(IMSA_IMS_INPUT_EVENT_STRU));

    pstImsaImsInputEvent->enEventType = IMSA_IMS_EVENT_TYPE_SYSTEM;

    pstImsaImsInputEvent->evt.stInputSystemEvent.enInputSysReason = IMSA_IMS_INPUT_SYSTEM_REASON_START;

    pstImsaImsInputEvent->evt.stInputSystemEvent.ulOpId = IMSA_AllocImsOpId();

    IMSA_SndImsMsgSystemEvent(pstImsaImsInputEvent);

    /*�ͷ���Ϣ�ռ�*/
    IMSA_MEM_FREE(pstImsaImsInputEvent);

}


VOS_VOID IMSA_SndImsMsgSystemEvent(IMSA_IMS_INPUT_EVENT_STRU *pstImsaImsInputEvent)
{
    IMSA_IMS_INPUT_SYSTEM_MSG_STRU              *pstMsgInputSystem;

    IMSA_INFO_LOG("IMSA_SndD2ImsMsgSystemEvent:enter!");

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstMsgInputSystem = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_IMS_INPUT_SYSTEM_MSG_STRU));

    if ( VOS_NULL_PTR == pstMsgInputSystem )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndD2ImsMsgSystemEvent:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgInputSystem), 0, \
               IMSA_GET_MSG_LENGTH(pstMsgInputSystem));

    /*��д��Ϣͷ*/
    IMSA_WRITE_IMS_MSG_HEAD(pstMsgInputSystem,ID_IMSA_IMS_INPUT_SYSTEM_MSG);

    IMSA_MEM_CPY(&pstMsgInputSystem->stInputSystem, \
                 &pstImsaImsInputEvent->evt.stInputSystemEvent, \
                 sizeof(IMSA_IMS_INPUT_SYSTEM_EVENT_STRU));


#if(VOS_WIN32 == VOS_OS_VER)
    /*PC���ԣ�����Ϣ���ͳ�ȥ������ST��֤*/
    IMSA_SND_MSG(pstMsgInputSystem);

#else
    /*��Ϣ����HSO��*/
    (VOS_VOID)LTE_MsgHook(pstMsgInputSystem);

    /*�ͷ���Ϣ�ռ�*/
    IMSA_FREE_MSG(pstMsgInputSystem);

    (VOS_VOID)IMSA_WriteImsEvent((VOS_VOID*)pstImsaImsInputEvent);

#endif

}
VOS_VOID IMSA_SndImsMsgParaEvent(IMSA_IMS_INPUT_EVENT_STRU *pstImsaImsInputEvent)
{
    IMSA_IMS_INPUT_PARA_MSG_STRU              *pstMsgInputPara;

    IMSA_INFO_LOG("IMSA_SndImsMsgParaEvent:enter!");

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstMsgInputPara = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_IMS_INPUT_PARA_MSG_STRU));

    if ( VOS_NULL_PTR == pstMsgInputPara )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndImsMsgParaEvent:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgInputPara), 0, \
               IMSA_GET_MSG_LENGTH(pstMsgInputPara));

    /*��д��Ϣͷ*/
    IMSA_WRITE_IMS_MSG_HEAD(pstMsgInputPara,ID_IMSA_IMS_INPUT_PARA_MSG);

    IMSA_MEM_CPY(&pstMsgInputPara->stInputPara, \
                 &pstImsaImsInputEvent->evt.stInputParaEvent, \
                 sizeof(IMSA_IMS_INPUT_PARA_EVENT_STRU));


#if(VOS_WIN32 == VOS_OS_VER)
    /*PC���ԣ�����Ϣ���ͳ�ȥ������ST��֤*/
    IMSA_SND_MSG(pstMsgInputPara);

#else
    /*��Ϣ����HSO��*/
    (VOS_VOID)LTE_MsgHook(pstMsgInputPara);

    /*�ͷ���Ϣ�ռ�*/
    IMSA_FREE_MSG(pstMsgInputPara);

    (VOS_VOID)IMSA_WriteImsEvent((VOS_VOID*)pstImsaImsInputEvent);

#endif

}
VOS_VOID IMSA_SndImsMsgNvInfoEvent
(
    IMSA_IMS_INPUT_EVENT_STRU   *pstImsaImsInputEvent,
    VOS_UINT32                  ulNvInfoStruLen
)
{
    IMSA_IMS_INPUT_NV_INFO_MSG_STRU     *pstMsgInputNvInfo;

    IMSA_INFO_LOG("IMSA_SndImsMsgParaEvent:enter!");

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstMsgInputNvInfo = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_IMS_INPUT_NV_INFO_MSG_STRU)
                                                    + ulNvInfoStruLen - IMSA_NV_INFO_LEN);

    if ( VOS_NULL_PTR == pstMsgInputNvInfo )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndImsMsgParaEvent:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgInputNvInfo), 0, \
               IMSA_GET_MSG_LENGTH(pstMsgInputNvInfo));

    /*��д��Ϣͷ*/
    IMSA_WRITE_IMS_MSG_HEAD(pstMsgInputNvInfo,ID_IMSA_IMS_INPUT_NV_INFO_MSG);

    IMSA_MEM_CPY(&pstMsgInputNvInfo->stInputNvInfo, \
                 &pstImsaImsInputEvent->evt.stInputNvInfoEvent, \
                 sizeof(IMSA_IMS_INPUT_NV_INFO_EVENT_STRU)+ ulNvInfoStruLen - IMSA_NV_INFO_LEN);


#if(VOS_WIN32 == VOS_OS_VER)
    /*PC���ԣ�����Ϣ���ͳ�ȥ������ST��֤*/
    IMSA_SND_MSG(pstMsgInputNvInfo);

#else
    /*��Ϣ����HSO��*/
    (VOS_VOID)LTE_MsgHook(pstMsgInputNvInfo);

    /*�ͷ���Ϣ�ռ�*/
    IMSA_FREE_MSG(pstMsgInputNvInfo);

    (VOS_VOID)IMSA_WriteImsEvent((VOS_VOID*)pstImsaImsInputEvent);

#endif

}


/*****************************************************************************
 Function Name  : IMSA_SndImsMsgServiceEvent()
 Description    : ��IMS����Service�¼�
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-07-16  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndImsMsgServiceEvent(IMSA_IMS_INPUT_EVENT_STRU *pstHiInputEvent)
{
    IMSA_IMS_INPUT_SERVICE_MSG_STRU              *pstMsgInputSevice;

    IMSA_INFO_LOG("IMSA_SndImsMsgServiceEvent:enter!");

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstMsgInputSevice = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_IMS_INPUT_SERVICE_MSG_STRU));

    if ( VOS_NULL_PTR == pstMsgInputSevice )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndImsMsgServiceEvent:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgInputSevice), 0, \
               IMSA_GET_MSG_LENGTH(pstMsgInputSevice));

    /*��д��Ϣͷ*/
    IMSA_WRITE_IMS_MSG_HEAD(pstMsgInputSevice,ID_IMSA_IMS_INPUT_SERVICE_MSG);



    IMSA_MEM_CPY(&pstMsgInputSevice->stInputService, \
                 &pstHiInputEvent->evt.stInputServiceEvent, \
                 sizeof(IMSA_IMS_INPUT_SERVICE_EVENT_STRU));


#if(VOS_WIN32 == VOS_OS_VER)
    /*PC���ԣ�����Ϣ���ͳ�ȥ������ST��֤*/
    IMSA_SND_MSG(pstMsgInputSevice);

#else
    /*��Ϣ����HSO��*/
    (VOS_VOID)LTE_MsgHook(pstMsgInputSevice);

    /*�ͷ���Ϣ�ռ�*/
    IMSA_FREE_MSG(pstMsgInputSevice);

    (VOS_VOID)IMSA_WriteImsEvent((VOS_VOID*)pstHiInputEvent);

#endif

}
VOS_VOID IMSA_SndImsMsgCallEvent(const IMSA_IMS_INPUT_EVENT_STRU *pstInputEvent)
{
    IMSA_IMS_INPUT_CALL_MSG_STRU                 *pstMsgInputCall;

    if(VOS_NULL_PTR == pstInputEvent)
    {
        IMSA_ERR_LOG("IMSA_SndImsMsgCallEvent:Input para err!");
        return ;
    }

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstMsgInputCall = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_IMS_INPUT_CALL_MSG_STRU));

    if ( VOS_NULL_PTR == pstMsgInputCall )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndImsMsgCallEvent:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgInputCall), 0, \
               IMSA_GET_MSG_LENGTH(pstMsgInputCall));

    /*��д��Ϣͷ*/
    IMSA_WRITE_IMS_MSG_HEAD(pstMsgInputCall,ID_IMSA_IMS_INPUT_CALL_MSG);

    IMSA_MEM_CPY(&pstMsgInputCall->stInputCall, \
                 &pstInputEvent->evt.stInputCallEvent, \
                 sizeof(IMSA_IMS_INPUT_CALL_EVENT_STRU));

#if(VOS_WIN32 == VOS_OS_VER)
    /*PC���ԣ�����Ϣ���ͳ�ȥ������ST��֤*/
    IMSA_SND_MSG(pstMsgInputCall);

#else
    /*��Ϣ����HSO��*/
    (VOS_VOID)LTE_MsgHook(pstMsgInputCall);

    /*�ͷ���Ϣ�ռ�*/
    IMSA_FREE_MSG(pstMsgInputCall);

    IMSA_NORM_LOG1("before write ims event,tick value:", PS_GET_TICK());

    (VOS_VOID)IMSA_WriteImsEvent((VOS_VOID*)pstInputEvent);

    IMSA_NORM_LOG1("after write ims event,tick value:", PS_GET_TICK());

#endif

}


/*****************************************************************************
 Function Name  : IMSA_SndImsMsgSmsEvent()
 Description    : ��IMS����Sms�¼�
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-07-17  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndImsMsgSmsEvent(IMSA_IMS_INPUT_EVENT_STRU *pstImsaImsInputEvent)
{
    IMSA_IMS_INPUT_SMS_MSG_STRU                  *pstMsgInputSms;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstMsgInputSms = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_IMS_INPUT_SMS_MSG_STRU));

    if ( VOS_NULL_PTR == pstMsgInputSms )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndImsMsgSmsEvent:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgInputSms), 0, \
               IMSA_GET_MSG_LENGTH(pstMsgInputSms));

    /*��д��Ϣͷ*/
    IMSA_WRITE_IMS_MSG_HEAD(pstMsgInputSms,ID_IMSA_IMS_INPUT_SMS_MSG);

    IMSA_MEM_CPY(&pstMsgInputSms->stInputSms, \
                 &pstImsaImsInputEvent->evt.stInputSmsEvent, \
                 sizeof(IMSA_IMS_INPUT_SMS_EVENT_STRU));

#if(VOS_WIN32 == VOS_OS_VER)
    /*PC���ԣ�����Ϣ���ͳ�ȥ������ST��֤*/
    IMSA_SND_MSG(pstMsgInputSms);

#else
    /*��Ϣ����HSO��*/
    (VOS_VOID)LTE_MsgHook(pstMsgInputSms);

    /*�ͷ���Ϣ�ռ�*/
    IMSA_FREE_MSG(pstMsgInputSms);

    (VOS_VOID)IMSA_WriteImsEvent((VOS_VOID*)pstImsaImsInputEvent);

#endif

}
VOS_VOID IMSA_SndImsMsgUssdEvent(IMSA_IMS_INPUT_EVENT_STRU *pstImsaImsInputEvent)
{
    IMSA_IMS_INPUT_USSD_MSG_STRU                  *pstMsgInputUssd;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstMsgInputUssd = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_IMS_INPUT_USSD_MSG_STRU));

    if ( VOS_NULL_PTR == pstMsgInputUssd )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndImsMsgSmsEvent:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgInputUssd), 0, \
               IMSA_GET_MSG_LENGTH(pstMsgInputUssd));

    /*��д��Ϣͷ*/
    IMSA_WRITE_IMS_MSG_HEAD(pstMsgInputUssd,ID_IMSA_IMS_INPUT_USSD_MSG);

    IMSA_MEM_CPY(&pstMsgInputUssd->stInputUssd, \
                 &pstImsaImsInputEvent->evt.stInputUssdEvent, \
                 sizeof(IMSA_IMS_INPUT_USSD_EVENT_STRU));

#if(VOS_WIN32 == VOS_OS_VER)
    /*PC���ԣ�����Ϣ���ͳ�ȥ������ST��֤*/
    IMSA_SND_MSG(pstMsgInputUssd);

#else
    /*��Ϣ����HSO��*/
    (VOS_VOID)LTE_MsgHook(pstMsgInputUssd);

    /*�ͷ���Ϣ�ռ�*/
    IMSA_FREE_MSG(pstMsgInputUssd);

    (VOS_VOID)IMSA_WriteImsEvent((VOS_VOID*)pstImsaImsInputEvent);

#endif

}
VOS_UINT32 IMSA_ImsInputParaMsgSave(const IMSA_IMS_INPUT_PARA_EVENT_STRU *pstParaEvt)
{
    IMSA_ENTITY_STRU *pstImsaEntity = IMSA_CtxGet();

    IMSA_INFO_LOG("IMSA_ImsQryParaMsgSave is entered!");

    pstImsaEntity->stImsaControlManager.stImsMsg.ulCsmId     = pstParaEvt->ulOpId;
    pstImsaEntity->stImsaControlManager.stImsMsg.ulCsmReason = pstParaEvt->enInputParaReason;

    return VOS_TRUE;
}
#if 0

VOS_UINT32 IMSA_ImsInputParaMsgFind(VOS_UINT32 ulOpId)
{
    IMSA_ENTITY_STRU *pstImsaEntity = IMSA_CtxGet();

    IMSA_INFO_LOG("IMSA_ImsQryParaMsgFind is entered!");

    if (ulOpId == pstImsaEntity->stImsaControlManager.stImsMsg.ulCsmId)
    {
        return VOS_TRUE;
    }
    else
    {
        return VOS_FALSE;
    }
}
#endif

VOS_VOID IMSA_ImsInputParaMsgClear(VOS_VOID)
{
    IMSA_ENTITY_STRU *pstImsaEntity = IMSA_CtxGet();

    IMSA_INFO_LOG("IMSA_ImsQryParaMsgClear is entered!");

    pstImsaEntity->stImsaControlManager.stImsMsg.bitOpImsMsg = IMSA_FALSE;
    pstImsaEntity->stImsaControlManager.stImsMsg.ulCsmId     = 0;
    pstImsaEntity->stImsaControlManager.stImsMsg.ulCsmReason = 0xffffffff;
}


VOS_VOID IMSA_SendImsVolteImpuQryReq(VOS_VOID)
{
    IMSA_IMS_INPUT_EVENT_STRU          *pstImsaImsInputEvent = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_SendImsVolteImpuQryReq is entered!");

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstImsaImsInputEvent = IMSA_MEM_ALLOC(sizeof(IMSA_IMS_INPUT_EVENT_STRU));

    if ( VOS_NULL_PTR == pstImsaImsInputEvent )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SendImsVolteImpuQryReq:ERROR:Alloc Mem fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(pstImsaImsInputEvent, 0, \
                 sizeof(IMSA_IMS_INPUT_EVENT_STRU));

    pstImsaImsInputEvent->enEventType = IMSA_IMS_EVENT_TYPE_PARA;
    pstImsaImsInputEvent->evt.stInputParaEvent.enInputParaReason = IMAS_IMS_INPUT_PARA_REASON_QUERY_IMPU;
    pstImsaImsInputEvent->evt.stInputParaEvent.ulOpId = IMSA_AllocImsOpId();

    IMSA_SndImsMsgParaEvent(pstImsaImsInputEvent);

    /* �����ѯIMPU����Ϣ */
    (VOS_VOID)IMSA_ImsInputParaMsgSave(&pstImsaImsInputEvent->evt.stInputParaEvent);

    /*�ͷ���Ϣ�ռ�*/
    IMSA_MEM_FREE(pstImsaImsInputEvent);

    return;
}
VOS_VOID IMSA_SendImsCcwaiSetReq(VOS_UINT8 ucCallWaiting)
{
    IMSA_IMS_INPUT_EVENT_STRU          *pstImsaImsInputEvent = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_SendImsCcwaiSetReq is entered!");

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstImsaImsInputEvent = IMSA_MEM_ALLOC(sizeof(IMSA_IMS_INPUT_EVENT_STRU));

    if ( VOS_NULL_PTR == pstImsaImsInputEvent )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SendImsCcwaiSetReq:ERROR:Alloc Mem fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(pstImsaImsInputEvent, 0, \
                 sizeof(IMSA_IMS_INPUT_EVENT_STRU));

    pstImsaImsInputEvent->enEventType = IMSA_IMS_EVENT_TYPE_PARA;
    pstImsaImsInputEvent->evt.stInputParaEvent.enInputParaReason = IMAS_IMS_INPUT_PARA_REASON_SET_CALL_WAITING;
    pstImsaImsInputEvent->evt.stInputParaEvent.ulOpId = IMSA_AllocImsOpId();
    pstImsaImsInputEvent->evt.stInputParaEvent.u.stUeCapability.ucCallWaiting = ucCallWaiting;

    IMSA_SndImsMsgParaEvent(pstImsaImsInputEvent);

    /* ��������CCWAI����Ϣ */
    (VOS_VOID)IMSA_ImsInputParaMsgSave(&pstImsaImsInputEvent->evt.stInputParaEvent);

    /*�ͷ���Ϣ�ռ�*/
    IMSA_MEM_FREE(pstImsaImsInputEvent);

    return;
}
VOS_VOID IMSA_ProcImsMsgQryVolteImpuCnf(const IMSA_IMS_OUTPUT_PARA_EVENT_STRU *pstOutPara)
{
    VOS_UINT32                          ulResult = VOS_ERR;
    VOS_UINT32                          ulImpuLen = 0;
    VOS_CHAR                            aucImpu[IMSA_IMS_EVENT_STRING_SZ+1] = {0};

    IMSA_INFO_LOG("IMSA_ProcImsMsgQryVolteImpuCnf is entered!");

    if (VOS_TRUE == pstOutPara->bitOpResult)
    {
        ulResult = pstOutPara->ulResult;
    }

    if (VOS_TRUE == pstOutPara->bitOpImpu)
    {
        ulImpuLen = pstOutPara->ulImpuLen;

        IMSA_MEM_CPY(aucImpu, \
                 pstOutPara->aucImpu, \
                 IMSA_IMS_EVENT_STRING_SZ+1);
    }

    IMSA_SndMsgAtQryVolteImpuCnf(ulResult, ulImpuLen, aucImpu);

}


VOS_VOID IMSA_ProcImsMsgCcwaiSetCnf(const IMSA_IMS_OUTPUT_PARA_EVENT_STRU *pstOutPara)
{
    VOS_UINT32                          ulResult = VOS_ERR;

    IMSA_INFO_LOG("IMSA_ProcImsMsgCcwaiSetCnf is entered!");

    if (IMSA_IMS_OUTPUT_PARA_REASON_SET_OK == pstOutPara->enOutputParaReason)
    {
        ulResult = VOS_OK;
    }

    IMSA_SndMsgAtCcwaiSetCnf(ulResult);
}



#if 0
/*****************************************************************************
 Function Name  : IMSA_SndD2ImsMsgRadioEvent()
 Description    : ��IMS����Radio�¼�
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-07-17  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndD2ImsMsgRadioEvent(HICSM_InputEvent *pstHiInputEvent)
{
    IMSA_IMS_INPUT_RADIO_EVENT_STRU                *pstMsgInputRadio;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstMsgInputRadio = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_IMS_INPUT_RADIO_EVENT_STRU));

    if ( VOS_NULL_PTR == pstMsgInputRadio )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndD2ImsMsgRadioEvent:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgInputRadio), 0, \
               IMSA_GET_MSG_LENGTH(pstMsgInputRadio));

    /*��д��Ϣͷ*/
    IMSA_WRITE_IMS_MSG_HEAD(pstMsgInputRadio,ID_IMSA_IMS_INPUT_RADIO_EVENT);

    IMSA_MEM_CPY(&pstMsgInputRadio->stInputRadio, \
                 &pstHiInputEvent->evt.radio, \
                 sizeof(HICSM_InputRadio));

#if(VOS_WIN32 == VOS_OS_VER)
    /*PC���ԣ�����Ϣ���ͳ�ȥ������ST��֤*/
    IMSA_SND_MSG(pstMsgInputRadio);

#else
    /*��Ϣ����HSO��*/
    (VOS_VOID)LTE_MsgHook(pstMsgInputRadio);

    /*�ͷ���Ϣ�ռ�*/
    IMSA_FREE_MSG(pstMsgInputRadio);

    (VOS_VOID)IMSA_WriteImsEvent((VOS_VOID*)pstHiInputEvent);

#endif


}
#endif
#if 0
/*****************************************************************************
 Function Name  : IMSA_SndD2ImsMsgUssdEvent()
 Description    : ��IMS����Ussd�¼�
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-07-17  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndD2ImsMsgUssdEvent(HICSM_InputEvent *pstHiInputEvent)
{
    IMSA_IMS_INPUT_USSD_EVENT_STRU                 *pstMsgInputUssd;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstMsgInputUssd = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_IMS_INPUT_USSD_EVENT_STRU));

    if ( VOS_NULL_PTR == pstMsgInputUssd )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndD2ImsMsgUssdEvent:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgInputUssd), 0, \
               IMSA_GET_MSG_LENGTH(pstMsgInputUssd));

    /*��д��Ϣͷ*/
    IMSA_WRITE_IMS_MSG_HEAD(pstMsgInputUssd,ID_IMSA_IMS_INPUT_USSD_EVENT);

    IMSA_MEM_CPY(&pstMsgInputUssd->stInputUssd, \
                 &pstHiInputEvent->evt.ussd, \
                 sizeof(HICSM_InputUssd));

#if(VOS_WIN32 == VOS_OS_VER)
    /*PC���ԣ�����Ϣ���ͳ�ȥ������ST��֤*/
    IMSA_SND_MSG(pstMsgInputUssd);

#else
    /*��Ϣ����HSO��*/
    (VOS_VOID)LTE_MsgHook(pstMsgInputUssd);

    /*�ͷ���Ϣ�ռ�*/
    IMSA_FREE_MSG(pstMsgInputUssd);

    (VOS_VOID)IMSA_WriteImsEvent((VOS_VOID*)pstHiInputEvent);

#endif

}

/*****************************************************************************
 Function Name  : IMSA_SndD2ImsMsgUssdEvent()
 Description    : ��IMS����SupSrv�¼�
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-07-17  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndD2ImsMsgSupSrvEvent(HICSM_InputEvent *pstHiInputEvent)
{
    IMSA_IMS_INPUT_SUPSRV_EVENT_STRU               *pstMsgInputSupSrv;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstMsgInputSupSrv = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_IMS_INPUT_SUPSRV_EVENT_STRU));

    if ( VOS_NULL_PTR == pstMsgInputSupSrv )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndD2ImsMsgSupSrvEvent:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgInputSupSrv), 0, \
               IMSA_GET_MSG_LENGTH(pstMsgInputSupSrv));

    /*��д��Ϣͷ*/
    IMSA_WRITE_IMS_MSG_HEAD(pstMsgInputSupSrv,ID_IMSA_IMS_INPUT_SUPSRV_EVENT);

    IMSA_MEM_CPY(&pstMsgInputSupSrv->stInputSupSrv, \
                 &pstHiInputEvent->evt.supSrv, \
                 sizeof(HICSM_InputSupSrv));

#if(VOS_WIN32 == VOS_OS_VER)
    /*PC���ԣ�����Ϣ���ͳ�ȥ������ST��֤*/
    IMSA_SND_MSG(pstMsgInputSupSrv);

#else
    /*��Ϣ����HSO��*/
    (VOS_VOID)LTE_MsgHook(pstMsgInputSupSrv);

    /*�ͷ���Ϣ�ռ�*/
    IMSA_FREE_MSG(pstMsgInputSupSrv);

    (VOS_VOID)IMSA_writeImsEvent((VOS_VOID*)pstHiInputEvent);

#endif

}
#endif

/*****************************************************************************
 Function Name  : IMSA_ImsStartTimer()
 Description    : IMSЭ��ջ���ã�����������ʱ��
 Input          : pstTimerPara ��ʱ��������ŵ�ַ(�ڴ��ɵ������ͷ�)
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-07-25  Draft Enact
*****************************************************************************/
unsigned int IMSA_ImsStartTimer(const IMSA_TIMER_PARA_STRU  *pstTimerPara)
{
    if((VOS_NULL_PTR == pstTimerPara)
        ||(pstTimerPara->ulTimerName >= IMSA_IMS_TIMER_NAME_BUTT))
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_ImsStartTimer:ERROR input para!");
        return VOS_ERR;
    }

    /*�ж϶�ʱ���Ƿ�򿪣��Ѵ���ر�*/
    if(VOS_NULL_PTR != aphTimer[pstTimerPara->ulTimerName])
    {
        /*�ر�ʧ�ܣ��򱨾�����*/
        if (VOS_OK != PS_STOP_REL_TIMER(&(aphTimer[pstTimerPara->ulTimerName])))
        {
            /*��ӡ�쳣��Ϣ*/
            IMSA_WARN_LOG("IMSA_D2ImsStartTimer:WARN: stop reltimer error!");
            return VOS_ERR;
        }

        /*��ӡ�쳣��Ϣ*/
        IMSA_WARN_LOG1("(TimerType) Timer not close!",pstTimerPara->ulTimerName);
    }

    if (pstTimerPara->ulLen == 0)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_ImsStartTimer:ERROR: start unreasonable reltimer.");
        return VOS_ERR;
    }

    /*�趨��ʱ��NAME����ʧ���򱨾�����*/
    if (VOS_OK !=\
            PS_START_CALLBACK_REL_TIMER(&(aphTimer[pstTimerPara->ulTimerName]),\
                                        PS_PID_IMSVA,  pstTimerPara->ulLen, \
                                        pstTimerPara->ulTimerName,\
                                        pstTimerPara->ulParam, pstTimerPara->ucMode,\
                                        (REL_TIMER_FUNC)pstTimerPara->TimeOutRoutine, \
                                        pstTimerPara->enPrecision))
    {
          /*��ӡ�쳣��Ϣ*/
          IMSA_WARN_LOG("IMSA_ImsStartTimer:WARN: start reltimer error!");
          return VOS_ERR;
    }

    return VOS_OK;

}


/*****************************************************************************
 Function Name  : IMSA_D2ImsStartTimer()
 Description    : IMSЭ��ջ���ã�����ֹͣ��ʱ��
 Input          : pstTimerPara ��ʱ��������ŵ�ַ(�ڴ��ɵ������ͷ�)
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-07-25  Draft Enact
*****************************************************************************/
unsigned int IMSA_ImsStopTimer(unsigned int  ulTimerName)
{
    if(ulTimerName >= IMSA_IMS_TIMER_NAME_BUTT)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_ImsStopTimer:ERROR input para!");
        return VOS_ERR;
    }

    /*�ж϶�ʱ���Ƿ�򿪣��Ѵ���ر�*/
    if(VOS_NULL_PTR != aphTimer[ulTimerName])
    {
        /*�ر�ʧ�ܣ��򱨾�����*/
        if (VOS_OK != PS_STOP_REL_TIMER(&(aphTimer[ulTimerName])))
        {
            /*��ӡ�쳣��Ϣ*/
            IMSA_WARN_LOG("IMSA_ImsStopTimer:WARN: stop reltimer error!");
            return VOS_ERR;
        }

        aphTimer[ulTimerName] = VOS_NULL_PTR;
    }

    return VOS_OK;

}
/*lint +e961*/
/*lint +e960*/


#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
/* end of ImsaImsAdaption.c */



