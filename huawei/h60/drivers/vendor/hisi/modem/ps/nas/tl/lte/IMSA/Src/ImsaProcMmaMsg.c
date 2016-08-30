/******************************************************************************

   Copyright(C)2013,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : ImsaProcMmaMsg.c
  Description     : ��C�ļ�ʵ��MMA��Ϣ�����MMA��Ϣ����
  History           :
     1.sunbing 49683      2013-06-19  Draft Enact

******************************************************************************/

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "PsTypeDef.h"
#include "ImsaProcMmaMsg.h"
#include "ImsaEntity.h"
#include "ImsaPublic.h"
#include "ImsaServiceManagement.h"
#include "ImsaImsAdaption.h"
#include "ImsaMain.h"
#include "ImsaImsApi.h"
#include "ImsaProcSpmMsg.h"

/*lint -e767*/
#define    THIS_FILE_ID       PS_FILE_ID_IMSAPROCMMAMSG_C
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


/*****************************************************************************
  3 Function
*****************************************************************************/


/*lint -e960*/
/*lint -e961*/
/*****************************************************************************
 Function Name  : IMSA_ProcMmaMsg()
 Description    : MMA��Ϣ������
 Input          : VOS_VOID *pRcvMsg
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-06-21  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_ProcMmaMsg(const VOS_VOID *pRcvMsg )
{
    /* ������Ϣͷָ��*/
    PS_MSG_HEADER_STRU          *pHeader = VOS_NULL_PTR;
    IMSA_CONTROL_MANAGER_STRU   *pstControlManager;

    /* ��ȡ��Ϣͷָ��*/
    pHeader = (PS_MSG_HEADER_STRU *) pRcvMsg;

    pstControlManager = IMSA_GetControlManagerAddress();

    /* �ػ������У��յ������ػ���Ϣ�����Ϣ����ֱ�Ӷ��� */
    if ((IMSA_STATUS_STOPING == pstControlManager->enImsaStatus)
        && (ID_MMA_IMSA_START_REQ != pHeader->ulMsgName)
        && (ID_MMA_IMSA_STOP_REQ != pHeader->ulMsgName))
    {
        IMSA_WARN_LOG("IMSA_ProcMmaMsg: Status is Stoping and msg is not start and stop!");
        return;
    }

    switch(pHeader->ulMsgName)
    {
        case ID_MMA_IMSA_START_REQ:
            IMSA_ProcMmaMsgStartReq();
            break;

        case ID_MMA_IMSA_STOP_REQ:
            IMSA_ProcMmaMsgStopReq();
            break;

        case ID_MMA_IMSA_DEREG_REQ:
            IMSA_ProcMmaMsgDeregReq();
            break;

        case ID_MMA_IMSA_SERVICE_CHANGE_IND:
            IMSA_ProcMmaMsgServiceChangeInd(pRcvMsg);
            break;

        case ID_MMA_IMSA_CAMP_INFO_CHANGE_IND:
            IMSA_ProcMmaMsgCampInfoChangeInd(pRcvMsg);
            break;
        default:
            break;
    }
}

/*****************************************************************************
 Function Name  : IMSA_ProcMmaMsgStartReq()
 Description    : MMA������Ϣ������
 Input          : VOS_VOID *pRcvMsg
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-06-21  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_ProcMmaMsgStartReq(VOS_VOID  )
{
    IMSA_CONTROL_MANAGER_STRU  *pstControlManager;
    static VOS_UINT8            ucD2ImsTaskStart = 0;

    pstControlManager = IMSA_GetControlManagerAddress();

    if(ucD2ImsTaskStart == 0)
    {
        /*��ʼ��D2Э��ջ*/
        (VOS_VOID)IMSA_ImsInit();

        ucD2ImsTaskStart ++;
    }

    /*�жϿ���״̬������ѿ�������ظ������ɹ�*/
    if(pstControlManager->enImsaStatus == IMSA_STATUS_STARTED)
    {
        IMSA_SndMmaMsgStartCnf();
        return;
    }

    /*�Ѵ��ڿ�������״̬������*/
    if(pstControlManager->enImsaStatus == IMSA_STATUS_STARTING)
    {
        IMSA_WARN_LOG("IMSA_ProcMmaMsgStartReq: Status is Starting!");
        return;
    }

    IMSA_StopTimer(&pstControlManager->stProtectTimer);

    /*��ʼ��*/

    /*��ȡNV*/
    IMSA_ReadImsaNV();

    /*֪ͨIMS������ת״̬*/
    pstControlManager->enImsaStatus = IMSA_STATUS_STARTING;

    IMSA_SndImsMsgStartReq();

    IMSA_StartTimer(&pstControlManager->stProtectTimer);

    /*����λ�����ȡ����Ϣ*/
    if(pstControlManager->enImsaIsimStatus == IMSA_ISIM_STATUS_AVAILABLE)
    {
        IMSA_INFO_LOG("IMSA_ProcMmaMsgStartReq: ISIM is available!");
        IMSA_ReadIsimFile();
    }
}

/*****************************************************************************
 Function Name  : IMSA_SndMmaMsgStartCnf()
 Description    : ��MMA���Ϳ����ظ���Ϣ
 Input          : VOS_VOID *pRcvMsg
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-06-26  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndMmaMsgStartCnf(VOS_VOID  )
{
    IMSA_MMA_START_CNF_STRU           *pstImsaMmaStartCnf;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstImsaMmaStartCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_MMA_START_CNF_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pstImsaMmaStartCnf)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndMmaMsgStartCnf:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstImsaMmaStartCnf), 0, IMSA_GET_MSG_LENGTH(pstImsaMmaStartCnf));

    /*��д��Ϣͷ*/
    IMSA_WRITE_MMA_MSG_HEAD(pstImsaMmaStartCnf, ID_IMSA_MMA_START_CNF);

    /*������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstImsaMmaStartCnf);
}




/*****************************************************************************
 Function Name  : IMSA_ProcMmaMsgStopReq()
 Description    : MMA�ػ���Ϣ������
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-06-21  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_ProcMmaMsgStopReq(VOS_VOID  )
{
    IMSA_CONTROL_MANAGER_STRU *pstControlManager;

    IMSA_INFO_LOG("IMSA_ProcMmaMsgStopReq: enter!");

    pstControlManager = IMSA_GetControlManagerAddress();

    /* SRVCC�쳣(����ػ���DEREG REQ��״̬Ǩ��CONN+REG)���建�� */
    IMSA_SrvccAbormalClearBuff(IMSA_SRVCC_ABNORMAL_STOP_REQ);

    /*�ж�״̬������ѹػ�����ظ��ػ��ɹ�*/
    if(pstControlManager->enImsaStatus == IMSA_STATUS_NULL)
    {
        IMSA_SndMmaMsgStopCnf();
        return;
    }

    /*�Ѵ��ڹػ�״̬������*/
    if(pstControlManager->enImsaStatus == IMSA_STATUS_STOPING)
    {
        IMSA_WARN_LOG("IMSA_ProcMmaMsgStopReq: Status is Stoping!");
        return;
    }

    /*ֹͣ���ػ�������ʱ��*/
    IMSA_StopTimer(&pstControlManager->stProtectTimer);

    /*֪ͨIMS�ػ���ת״̬*/
    pstControlManager->enImsaStatus = IMSA_STATUS_STOPING;

    /*�ػ��󣬿���λʱ����Ҫ����״̬��Ϊ��Ч����Ϊ�����п���Ч�ĳ�������Ҫ�ػ������״̬*/
    if(pstControlManager->enImsaIsimStatus != IMSA_ISIM_STATUS_ABSENT)
    {
        pstControlManager->enImsaIsimStatus = IMSA_ISIM_STATUS_AVAILABLE;
    }

    IMSA_SndImsMsgStopReq();

    /*�������ػ�������ʱ��*/
    IMSA_StartTimer(&pstControlManager->stProtectTimer);
}

/*****************************************************************************
 Function Name  : IMSA_SndMmaMsgStopCnf()
 Description    : ��MMA���Ϳ����ظ���Ϣ
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-06-21  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndMmaMsgStopCnf(VOS_VOID  )
{
    IMSA_MMA_STOP_CNF_STRU           *pstImsaMmaStopCnf;

    IMSA_INFO_LOG("IMSA_SndMmaMsgStopCnf:enter!");

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstImsaMmaStopCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_MMA_STOP_CNF_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pstImsaMmaStopCnf)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndMmaMsgStopCnf:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstImsaMmaStopCnf), 0, IMSA_GET_MSG_LENGTH(pstImsaMmaStopCnf));

    /*��д��Ϣͷ*/
    IMSA_WRITE_MMA_MSG_HEAD(pstImsaMmaStopCnf, ID_IMSA_MMA_STOP_CNF);

    /*������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstImsaMmaStopCnf);
}



/*****************************************************************************
 Function Name  : IMSA_ProcMmaMsgDeregReq()
 Description    : MMAȥע����Ϣ������
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-06-21  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_ProcMmaMsgDeregReq(VOS_VOID )
{
    /* SRVCC�쳣(����ػ���DEREG REQ��״̬Ǩ��CONN+REG)���建�� */
    IMSA_SrvccAbormalClearBuff(IMSA_SRVCC_ABNORMAL_DEREG_REQ);

    IMSA_SRV_ProcDeregReq(IMSA_SRV_DEREG_CAUSE_MMA_DEREG_REQ);

    return ;
}

/*****************************************************************************
 Function Name  : IMSA_ProcMmaMsgDeregReq()
 Description    : MMA SERVICE CHANGE IND��Ϣ������
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-06-21  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_ProcMmaMsgServiceChangeInd
(
    const VOS_VOID                     *pRcvMsg
)
{
    IMSA_SRV_ProcServiceChangeInd((MMA_IMSA_SERVICE_CHANGE_IND_STRU*)pRcvMsg);

    return ;
}


VOS_VOID IMSA_ProcMmaMsgCampInfoChangeInd
(
    const VOS_VOID                     *pRcvMsg
)
{
    IMSA_SRV_ProcCampInfoChangeInd((MMA_IMSA_CAMP_INFO_CHANGE_IND_STRU*)pRcvMsg);

    return ;
}


/*****************************************************************************
 Function Name  : IMSA_SndMmaMsgDeregCnf()
 Description    : ��MMA����ȥע��ظ���Ϣ
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-06-26  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_SndMmaMsgDeregCnf(VOS_VOID )
{
    IMSA_MMA_DEREGISTER_CNF_STRU           *pstImsaMmaDeregCnf;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstImsaMmaDeregCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_MMA_DEREGISTER_CNF_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pstImsaMmaDeregCnf)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndMmaMsgDeregCnf:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstImsaMmaDeregCnf), 0, IMSA_GET_MSG_LENGTH(pstImsaMmaDeregCnf));

    /*��д��Ϣͷ*/
    IMSA_WRITE_MMA_MSG_HEAD(pstImsaMmaDeregCnf, ID_IMSA_MMA_DEREG_CNF);

    /*������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstImsaMmaDeregCnf);
}

/*****************************************************************************
 Function Name  : IMSA_ProcImsMsgStartOrStopCnf()
 Description    : ����IMS�������߹ػ����
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-06-24  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_ProcImsMsgStartOrStopCnf(VOS_VOID)
{
    IMSA_CONTROL_MANAGER_STRU *pstControlManager;

    IMSA_NORM_LOG("IMSA_ProcImsMsgStartOrStopCnf: Enter!");

    pstControlManager = IMSA_GetControlManagerAddress();

    /*ֹͣ���ػ�������ʱ��*/
    IMSA_StopTimer(&pstControlManager->stProtectTimer);

    /*�ػ�����*/
    if(pstControlManager->enImsaStatus == IMSA_STATUS_STOPING)
    {
        /*�����Դ*/
        IMSA_ClearResource();

        /*����ȴ�����״̬*/
        pstControlManager->enImsaStatus = IMSA_STATUS_NULL;

        IMSA_SndMmaMsgStopCnf();
    }
    else if(pstControlManager->enImsaStatus == IMSA_STATUS_STARTING)
    {
        /*���뿪��״̬*/
        pstControlManager->enImsaStatus = IMSA_STATUS_STARTED;

        IMSA_SndMmaMsgStartCnf();

        /* ��IMS����UE���� */
        IMSA_ConfigUeCapabilityInfo2Ims();

        if ((IMSA_OP_TRUE == pstControlManager->stImsaCommonInfo.stImsaUeId.bitOpImpi)
            && (IMSA_OP_TRUE == pstControlManager->stImsaCommonInfo.stImsaUeId.bitOpTImpu)
            && (IMSA_OP_TRUE == pstControlManager->stImsaCommonInfo.bitOpHomeNetDomainName))
        {
            /* config account info to ims */
            IMSA_ConfigAccoutInfo2Ims(IMSA_REG_TYPE_NORMAL);

            /* config account info to ims */
            IMSA_ConfigAccoutInfo2Ims(IMSA_REG_TYPE_EMC);
        }

        /* set imei */
        IMSA_ConfigImei2Ims();

        /* set retry time */
        IMSA_ConfigTimerLength2Ims();

        IMSA_ConfigSipPort2Ims();

        /* ��IMS����SIP��Ϣ */
        IMSA_ConfigSipInfo2Ims();

        IMSA_ConfigVoipInfo2Ims();

        IMSA_ConfigCodeInfo2Ims();

        IMSA_ConfigSsConfInfo2Ims();

        IMSA_ConfigSecurityInfo2Ims();
    }
    else
    {
        IMSA_WARN_LOG("IMSA_ProcImsMsgStartOrStopCnf: err stat!");
    }


}
 VOS_VOID IMSA_SndMmaMsgImsVoiceCapNotify(MMA_IMSA_IMS_VOICE_CAP_ENUM_UINT8 enImsVoiceCap)
 {
     IMSA_MMA_IMS_VOICE_CAP_NOTIFY_STRU     *pstImsaMmaImsVoiceCap;

     /*����ռ䲢��������Ƿ�ɹ�*/
     pstImsaMmaImsVoiceCap = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_MMA_IMS_VOICE_CAP_NOTIFY_STRU));

     /*����Ƿ����ɹ�*/
     if (VOS_NULL_PTR == pstImsaMmaImsVoiceCap)
     {
         /*��ӡ�쳣��Ϣ*/
         IMSA_ERR_LOG("IMSA_SndMmaMsgDeregCnf:ERROR:Alloc Msg fail!");
         return ;
     }

     /*���*/
     IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstImsaMmaImsVoiceCap), 0, IMSA_GET_MSG_LENGTH(pstImsaMmaImsVoiceCap));

     /*��д��Ϣͷ*/
     IMSA_WRITE_MMA_MSG_HEAD(pstImsaMmaImsVoiceCap, ID_IMSA_MMA_IMS_VOICE_CAP_NOTIFY);


     pstImsaMmaImsVoiceCap->enImsVoiceCap = enImsVoiceCap;

     /*������Ϣ���ͺ��� */
     IMSA_SND_MSG(pstImsaMmaImsVoiceCap);
 }
/*lint +e961*/
/*lint +e960*/



#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
/* end of ImsaProcMmaMsg.c */



