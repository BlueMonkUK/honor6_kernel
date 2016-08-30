

#ifndef __CDS_DEBUG_H__
#define __CDS_DEBUG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/


/*****************************************************************************
  3 Massage Declare
*****************************************************************************/

/*****************************************************************************
  4 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
   5 STRUCT����
*****************************************************************************/


/*****************************************************************************
 �ṹ��    : CDS_MODEM_STATS_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : ��ͬModem��ͳ����Ϣ
*****************************************************************************/
typedef struct
{
    /*GU����������Ϣ*/
    VOS_UINT32              ulULGURxPktNum;                  /*����GUģ���յ�PKT����*/
    VOS_UINT32              ulULGUFcFreePktNum;              /*GU�������ض�������*/
    VOS_UINT32              ulULGUSendToRabmNum;             /*GU���͵�RABM����*/
    VOS_UINT32              ulULGUSavePktNum;                /*GU���л��������*/
    VOS_UINT32              ulULGURabmStateErr;              /*GU����RABM״̬�쳣*/

    /*GU����������Ϣ*/
    VOS_UINT32              ulDLGSMRxSduNum;                 /*����GSMģ����SDU*/
    VOS_UINT32              ulDLGSMRxImsNum;                 /*GSM�½���IMS���ݰ�����*/
    VOS_UINT32              ulDLGSMFcFreeSduNum;             /*GSM����������Ķ���*/
    VOS_UINT32              ulDLGSMEnQueSucc;                /*��SDU���гɹ�����*/
    VOS_UINT32              ulDLGSMEnQueFail;                /*��SDU����ʧ�ܴ���*/

    VOS_UINT32              ulDLUMTSRxSduNum;                /*����UMTSģ����SDU*/
    VOS_UINT32              ulDLUMTSRxImsNum;                /*UMTS�½���IMS���ݰ�����*/
    VOS_UINT32              ulDLUMTSFcFreeSduNum;            /*UMTS����������Ķ���*/
    VOS_UINT32              ulDLUMTSDirectFreeSduNum;        /*UMTS����������Ķ���*/
    VOS_UINT32              ulDLUMTSEnQueSucc;               /*��SDU���гɹ�����*/
    VOS_UINT32              ulDLUMTSEnQueFail;               /*��SDU����ʧ�ܴ���*/

    VOS_UINT32              ulLBDLRxSduInGSM;                /*����GSMģ�½���LB SDU����*/
    VOS_UINT32              ulLBDLRxSduInUMTS;               /*����UMTSģ�½���LB SDU����*/

}CDS_MODEM_STATS_INFO_STRU;

/*****************************************************************************
 �ṹ��    : CDS_STATS_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : CDSͳ����
*****************************************************************************/
typedef struct CDS_STATS_INFO
{
    /*�������¼�ͳ��*/
    VOS_UINT32              ulIpfULIntNum;                   /*�յ�IPF�����жϴ���*/
    VOS_UINT32              ulULProcIpfIntNum;               /*���д���IPF�жϴ���*/

    VOS_UINT32              ulUlAdqEmtypIntNum;              /*����IPF ADQ���жϴ���*/
    VOS_UINT32              ulUlProcAdqEmptyIntNum;          /*���д���IPF ADQ���жϴ���*/

    VOS_UINT32              ulIpfDlCallMemFreeNum;           /*����IPF�����ڴ��ͷź�������*/
    VOS_UINT32              ulIpfDlFreeMemNum;               /*BSP���ûص������ͷ��ڴ�ĸ���*/

    VOS_UINT32              ulDL10msTmrTrigEvent;            /*����10ms�����Զ�ʱ�����������¼�����*/
    VOS_UINT32              ulDL1msIntTrigEvent;             /*LTE 1ms�жϴ��������¼�����*/
    VOS_UINT32              ulDLUmtsIntTrigEvent;            /*UMTS DSP�жϴ��������¼�����*/
    VOS_UINT32              ulDLProcEventNum;                /*CDS���������¼�����*/


    /*����RDͳ����Ϣ*/
    VOS_UINT32              ulULIpfRxRDNum;                  /*����IPF�յ�RD����*/
    VOS_UINT32              ulULIpfTransMemFailNum;          /*����IPF�ڴ�ת��ʧ�ܸ���*/
    VOS_UINT32              ulULIpfModemIdErrNum;            /*����IPF ModemId�������*/
    VOS_UINT32              ulULIpfFreePktInLB;              /*����IPF�ڻ���ģʽ���ͷŵ��ڴ�*/
    VOS_UINT32              ulULIpfRxNdisPktNum;             /*����IPF�յ�NDIS������*/
    VOS_UINT32              ulULIpfRxIpPktNum;               /*����IPF�յ�IP��(�Ƿ�Ƭ)����*/
    VOS_UINT32              ulULIpfRxIpSegNum;               /*����IPF�յ�IP��Ƭ����*/
    VOS_UINT32              ulULIpfFilterErrNum;             /*����IPF���˴��������*/

    VOS_UINT32              ulULRecvNdPktNum;                /*�����յ�ND���ĸ���*/

    /*����IP������*/
    VOS_UINT32              ulULASRxIpPktNum;                /*���н�����յ������ݰ�*/
    VOS_UINT32              ulULDirectFreePktNum;            /*���д�׮ֱ���ͷŵ����ݰ�����*/
    VOS_UINT32              ulULLTERxPktNum;                 /*����LTEģ����PKT����*/
    VOS_UINT32              ulULGURxPktNum;                  /*����GUģ���յ�PKT����*/
    VOS_UINT32              ulULNULLRxPktNum;                /*����NULLģ���յ�������*/
    VOS_UINT32              ulULRanModeErrFreeNum;           /*���н���ģʽ�����ͷ��ڴ����*/

    VOS_UINT32              ulULLTEFcFreePktNum;             /*LTE�������ض�������*/
    VOS_UINT32              ulULLTEProcIpSegFail;            /*LTE�����Ƭ��ʧ�ܸ���*/
    VOS_UINT32              ulULLTESendToLPdcpNum;           /*LTE���͵�LPDCP����*/
    VOS_UINT32              ulULLTESavePktNum;               /*LTE���л��������*/
    VOS_UINT32              ulULLTEERabmStateErr;            /*LTE����ERABM״̬�쳣�ͷ��ڴ����*/

    VOS_UINT32              ulULGUFcFreePktNum;              /*GU�������ض�������*/
    VOS_UINT32              ulULGUSendToRabmNum;             /*GU���͵�RABM����*/
    VOS_UINT32              ulULGUSavePktNum;                /*GU���л��������*/
    VOS_UINT32              ulULGURabmStateErr;              /*GU����RABM״̬�쳣*/

    VOS_UINT32              ulULBuffEnQueSucc;               /*���л���������ӳɹ�����*/
    VOS_UINT32              ulULBuffEnQueFail;               /*���л����������ʧ��*/
    VOS_UINT32              ulULBuffSoftFilterFail;          /*���з��ͻ�������ʧ�ܸ���*/
    VOS_UINT32              ulULBuffSendPktSuccNum;          /*���з��ͻ������ݸ���*/
    VOS_UINT32              ulULBuffFreePktNum;              /*������ջ������ݸ���*/

    /*��������ͳ����*/
    VOS_UINT32              ulULSoftFilterPktNum;            /*������������ݰ�����*/
    VOS_UINT32              ulULSoftFilterIPv4Num;           /*���������IPV4���ݰ�����*/
    VOS_UINT32              ulULSoftFilterIPv6Num;           /*���������IPV6���ݰ�����*/
    VOS_UINT32              ulULSoftFilterFailNum;           /*���������ʧ�ܸ���*/
    VOS_UINT32              ulULSoftFilterIPSegNum;          /*���������IP��Ƭ����*/
    VOS_UINT32              ulULSoftFilterIPSegSucc;         /*���������IP��Ƭ����ɹ�����*/
    VOS_UINT32              ulULSoftFilterIPSegFail;         /*���������IP��Ƭ����ʧ�ܸ���*/

    /*����AD��Ϣ*/
    VOS_UINT32              ulULADQCfgAdNum;                 /*ADQ���ø���*/
    VOS_UINT32              ulULADQCfgAd0Num;                /*AD0���ø���*/
    VOS_UINT32              ulULADQCfgAd1Num;                /*AD1���ø���*/
    VOS_UINT32              ulULADQAllocMemNum;              /*ΪADQ������ڴ����*/
    VOS_UINT32              ulULADQFreeMemNum;               /*ΪADQ����ʧ���ͷŵ��ڴ����*/
    VOS_UINT32              ulULADQCfgAdSucc;                /*ADQ����AD�ɹ�����*/
    VOS_UINT32              ulULADQCfgAdFail;                /*ADQ����ADʧ�ܴ���*/
    VOS_UINT32              ulULADQStartEmptyTmrNum;         /*ADQ�����ձ�����ʱ������*/
    VOS_UINT32              ulULADQStopEmptyTmrNum;          /*ADQֹͣ�ձ�����ʱ������*/
    VOS_UINT32              ulULADQEmptyTmrTimeoutNum;       /*ADQ�ձ�����ʱ����ʱ����*/
    VOS_UINT32              ulULADQAd0EmptyIntNum;           /*AD0������жϴ���*/
    VOS_UINT32              ulULADQAd1EmptyIntNum;           /*AD1������жϴ���*/

    /*����SDU��Ϣ*/
    VOS_UINT32              ulDLGSMRxSduNum;                 /*����GSMģ����SDU*/
    VOS_UINT32              ulDLGSMRxImsNum;                 /*����GSMģ����IMS���ݰ�����*/
    VOS_UINT32              ulDLGSMFcFreeSduNum;             /*GSM����������Ķ���*/
    VOS_UINT32              ulDLGSMGetDeftRabIdFail;         /*GSMģ���RABID��Ӧ��Ĭ��RABIDʧ��*/
    VOS_UINT32              ulDLGSMEnQueSucc;                /*��SDU���гɹ�����*/
    VOS_UINT32              ulDLGSMEnQueFail;                /*��SDU����ʧ�ܴ���*/

    VOS_UINT32              ulDLUMTSRxSduNum;                /*����UMTSģ����SDU*/
    VOS_UINT32              ulDLUMTSRxImsNum;                /*����UMTSģ����IMS���ݰ�����*/
    VOS_UINT32              ulDLUMTSFcFreeSduNum;            /*UMTS����������Ķ���*/
    VOS_UINT32              ulDLUMTSDirectFreeSduNum;        /*UMTS����������Ķ���*/
    VOS_UINT32              ulDLUMTSGetDeftRabIdFail;        /*UMTSģ���RABID��Ӧ��Ĭ��RABIDʧ��*/
    VOS_UINT32              ulDLUMTSEnQueSucc;               /*��SDU���гɹ�����*/
    VOS_UINT32              ulDLUMTSEnQueFail;               /*��SDU����ʧ�ܴ���*/

    VOS_UINT32              ulDLLteRxSduNum;                 /*����LTEģ����SDU*/
    VOS_UINT32              ulDLLteRxDbgPkt;                 /*����LTE L2��������*/
    VOS_UINT32              ulDLLteRxMbmsSdu;                /*����MBMS SDU����*/
    VOS_UINT32              ulDLLteRxImsNum;                 /*����LTEģ����IMS���ݰ�����*/
    VOS_UINT32              ulDLLteFcFreeSduNum;             /*LTE����������Ķ���*/
    VOS_UINT32              ulDLLteDirectFreeSduNum;         /*GSM����������Ķ���*/
    VOS_UINT32              ulDLLteGetEpsbIDFail;            /*LTEģ���DRB��Ӧ��EPSBIDʧ��*/
    VOS_UINT32              ulDLLteGetDeftEpsbIDFail;        /*LTEģ���DRB��Ӧ��Ĭ��EPSBIDʧ��*/
    VOS_UINT32              ulDLLteEnQueSucc;                /*��SDU���гɹ�����*/
    VOS_UINT32              ulDLLteEnQueFail;                /*��SDU����ʧ�ܴ���*/

    VOS_UINT32              ulDLEnSduQueSucc;                /*��SDU���гɹ�����*/
    VOS_UINT32              ulDLEnSduQueFail;                /*��SDU����ʧ�ܴ���*/
    VOS_UINT32              ulDLSduTrigEvent;                /*����SDU�����¼�����*/

    /*����IPF��Ϣ*/
    VOS_UINT32              ulDLIpfSaveSrcMemFail;           /*����IPFԴ�ڴ���в��㵼���ڴ�*/
    VOS_UINT32              ulDLIpfSaveSrcMemSucc;           /*GU���б���Դ�ڴ����*/
    VOS_UINT32              ulDLIpfFreeSrcMemNum;            /*GU�ͷ�Դ�ڴ����*/
    VOS_UINT32              ulDLIpfFreeSrcMemErr;            /*�ͷ��ڴ����*/

    VOS_UINT32              ulDLIpfCfgHaveNoBD;              /*��������IPFû��BD����*/
    VOS_UINT32              ulDLIpfCfgHaveNoCD;              /*��������IPFû��CD����*/
    VOS_UINT32              ulDLIpfCfgCDNotEnough;           /*��������IPF CD��������*/
    VOS_UINT32              ulDLIpfCfgBDSucc;                /*����BD�ɹ�����*/
    VOS_UINT32              ulDLIpfCfgBDFail;                /*����BDʧ�ܴ���*/
    VOS_UINT32              ulDLIpfCfgBDAllocMemFail;        /*����BD�����ڴ�ʧ�ܴ���*/
    VOS_UINT32              ulDLIpfCfgBDSduLenErr;           /*����BD SDU���ȴ���*/

    /*����ģʽ������Ϣ*/
    VOS_UINT32              ulLBDLRxSduInGSM;                 /*����GSMģ�½���LB SDU����*/
    VOS_UINT32              ulLBDLRxSduInUMTS;                /*����UMTSģ�½���LB SDU����*/
    VOS_UINT32              ulLBDLRxSduInLte;                 /*����LTEģ�½���LB SDU����*/
    VOS_UINT32              ulLBDLRxSduInNonModeB;            /*�����ڷ�ModemB���յ���SDU*/
    VOS_UINT32              ulLBDLAllocMemFail;               /*����ģʽ�����ڴ�ʧ��*/
    VOS_UINT32              ulLBDDLEnQueSucc;                 /*����ģʽ����ӳɹ�����*/
    VOS_UINT32              ulLBDLEnQueFail;                  /*���ʧ�ܴ���*/
    VOS_UINT32              ulLBULSoftFilterFail;             /*����ģʽ���������ʧ�ܴ���*/
    VOS_UINT32              ulLBUlSendPktNum;                 /*����ģʽ���з������ݰ�����*/
    VOS_UINT32              ulLBUlClearPktNum;                /*����ģʽ�����ͷ����ݰ�����*/

    /*IMS���ͳ����*/
    VOS_UINT32              ulImsULRxFunCallNum;                /*���н��պ������ô���*/
    VOS_UINT32              ulImsULRxFunParaErr;                /*���н��պ�����δ������*/
    VOS_UINT32              ulImsULRxNicPktNum;                 /*����IMS NIC���ݰ�����*/
    VOS_UINT32              ulImsULAllocMemFail;                /*����IMS�����ڴ�ʧ�ܸ���*/
    VOS_UINT32              ulImsULEnQueFail;                   /*����IMS���ʧ�ܸ���*/
    VOS_UINT32              ulImsULEnQueSucc;                   /*����IMS��ӳɹ�����*/
    VOS_UINT32              ulImsULTrigImsEvent;                /*���д���IMS_PROC�¼�����*/
    VOS_UINT32              ulImsULProcImsEvent;                /*���д���IMS_PROC�¼�����*/
    VOS_UINT32              ulImsULSoftFilterFail;              /*����IMS���ݰ������ʧ�ܴ���*/
    VOS_UINT32              ulImsULSendToRan;                   /*����IMS���ݰ����͵��տڸ���*/

    VOS_UINT32              ulImsDLRxImsPkt;                    /*���н���IMS���ݰ�����*/
    VOS_UINT32              ulImsDLNdPktNum;                    /*���н��յ�NDCLIENT���ݰ�����*/
    VOS_UINT32              ulImsDLAllocMemFail;                /*���������ڴ�ʧ�ܸ���*/
    VOS_UINT32              ulImsDLGetDestPortFail;             /*���л�ȡĿ�Ķ˿ں�ʧ�ܸ���*/
    VOS_UINT32              ulImsDLSendToNicFail;               /*���з��͵�IMS NIC���ݰ�ʧ�ܸ���*/
    VOS_UINT32              ulImsDLSendToNicSucc;               /*���з��͵�IMS NIC���ݰ��ɹ�����*/

    VOS_UINT32              ulImsDLRxUtPkt;                     /*���н���IMS Ut���ݰ�����*/
    VOS_UINT32              ulImsDLUtPktProcErrNum;             /*����IMS Ut���ݰ�����ʧ�ܸ���*/
    VOS_UINT32              ulImsDLUtPktProcSuccNum;            /*����IMS Ut���ݰ�����ɹ�����*/

    CDS_MODEM_STATS_INFO_STRU  astModemStats[MODEM_ID_BUTT + 1];
}CDS_STATS_INFO_STRU;

/*****************************************************************************
  6 UNION����
*****************************************************************************/


/*****************************************************************************
  7 ȫ�ֱ�������
*****************************************************************************/
extern   CDS_STATS_INFO_STRU    g_stCdsStats;

/*�������¼�ͳ��*/
#define  CDS_DBG_RECV_UL_IPF_INT(n)                (g_stCdsStats.ulIpfULIntNum += (n))
#define  CDS_DBG_UL_PROC_IPF_INT_NUM(n)            (g_stCdsStats.ulULProcIpfIntNum += (n))
#define  CDS_DBG_UL_RX_ADQ_EMPTY_INT(n)            (g_stCdsStats.ulUlAdqEmtypIntNum += (n))
#define  CDS_DBG_UL_PROC_ADQ_EMPTY_INT_NUM(n)      (g_stCdsStats.ulUlProcAdqEmptyIntNum += (n))
#define  CDS_DBG_RECV_DL_MEMFREE_CB_NUM(n)         (g_stCdsStats.ulIpfDlCallMemFreeNum += (n))
#define  CDS_DBG_DL_FREE_MEM_NUM(n)                (g_stCdsStats.ulIpfDlFreeMemNum += (n))
#define  CDS_DBG_DL_10MS_TMR_TRIG_EVENT(n)         (g_stCdsStats.ulDL10msTmrTrigEvent += (n))
#define  CDS_DBG_DL_LTE_1MS_INT_TRIG_EVENT(n)      (g_stCdsStats.ulDL1msIntTrigEvent += (n))
#define  CDS_DBG_DL_UMTS_INT_TRIG_EVENT(n)         (g_stCdsStats.ulDLUmtsIntTrigEvent += (n))
#define  CDS_DBG_DL_PROC_EVENT_NUM(n)              (g_stCdsStats.ulDLProcEventNum += (n))


/*����IPF RDͳ����*/
#define  CDS_DBG_UL_IPF_RX_RD_NUM(n)                (g_stCdsStats.ulULIpfRxRDNum += (n))
#define  CDS_DBG_UL_IPF_TRANS_MEM_FAIL_NUM(n)       (g_stCdsStats.ulULIpfTransMemFailNum += (n))
#define  CDS_DBG_UL_IPF_MODEMID_ERROR_NUM(n)        (g_stCdsStats.ulULIpfModemIdErrNum += (n))
#define  CDS_DBG_UL_IPF_FREE_PKT_IN_LB(n)           (g_stCdsStats.ulULIpfFreePktInLB += (n))
#define  CDS_DBG_UL_IPF_RX_IP_PKT_NUM(n)            (g_stCdsStats.ulULIpfRxIpPktNum += (n))
#define  CDS_DBG_UL_IPF_RX_IP_SEG_NUM(n)            (g_stCdsStats.ulULIpfRxIpSegNum += (n))
#define  CDS_DBG_UL_IPF_RX_NDIS_PKT_NUM(n)          (g_stCdsStats.ulULIpfRxNdisPktNum += (n))
#define  CDS_DBG_UL_IPF_FILTER_ERROR_NUM(n)         (g_stCdsStats.ulULIpfFilterErrNum += (n))

#define  CDS_DBG_UL_RECV_ND_PKT_NUM(n)              (g_stCdsStats.ulULRecvNdPktNum += (n))

/*����ADͳ����*/
#define  CDS_DBG_UL_ADQ_ALLOC_MEM_NUM(n)            (g_stCdsStats.ulULADQAllocMemNum += (n))
#define  CDS_DBG_UL_ADQ_FREE_MEM_NUM(n)             (g_stCdsStats.ulULADQFreeMemNum  += (n))
#define  CDS_DBG_UL_ADQ_CFG_ADQ_SUCC_NUM(n)         (g_stCdsStats.ulULADQCfgAdSucc += (n))
#define  CDS_DBG_UL_ADQ_CFG_ADQ_FAIL_NUM(n)         (g_stCdsStats.ulULADQCfgAdFail += (n))
#define  CDS_DBG_UL_ADQ_CFG_AD_NUM(n)               (g_stCdsStats.ulULADQCfgAdNum += (n))
#define  CDS_DBG_UL_ADQ_CFG_AD0_NUM(n)              (g_stCdsStats.ulULADQCfgAd0Num += (n))
#define  CDS_DBG_UL_ADQ_CFG_AD1_NUM(n)              (g_stCdsStats.ulULADQCfgAd1Num += (n))
#define  CDS_DBG_UL_ADQ_START_EMPTY_TMR_NUM(n)      (g_stCdsStats.ulULADQStartEmptyTmrNum += (n))
#define  CDS_DBG_UL_ADQ_STOP_EMPTY_TMR_NUM(n)       (g_stCdsStats.ulULADQStopEmptyTmrNum  += (n))
#define  CDS_DBG_UL_ADQ_EMPTY_TMR_TIMEOUT_NUM(n)    (g_stCdsStats.ulULADQEmptyTmrTimeoutNum  += (n))
#define  CDS_DBG_UL_ADQ_AD0_EMPTY_INT_NUM(n)        (g_stCdsStats.ulULADQAd0EmptyIntNum += (n))
#define  CDS_DBG_UL_ADQ_AD1_EMPTY_INT_NUM(n)        (g_stCdsStats.ulULADQAd1EmptyIntNum  += (n))


/*����IPFͳ����*/
#define  CDS_DBG_DL_IPF_CFG_HAVE_NO_CD(n)           (g_stCdsStats.ulDLIpfCfgHaveNoCD += (n))
#define  CDS_DBG_DL_IPF_CFG_HAVE_NO_BD(n)           (g_stCdsStats.ulDLIpfCfgHaveNoBD += (n))
#define  CDS_DBG_DL_IPF_CFG_CD_NOT_ENOUGH(n)        (g_stCdsStats.ulDLIpfCfgCDNotEnough += (n))
#define  CDS_DBG_DL_IPF_CFG_BD_SUCC_NUM(n)          (g_stCdsStats.ulDLIpfCfgBDSucc += (n))
#define  CDS_DBG_DL_IPF_CFG_BD_FAIL_NUM(n)          (g_stCdsStats.ulDLIpfCfgBDFail += (n))
/*�Ž�ģʽ*/
#define  CDS_DBG_DL_IPF_SAVE_SRCMEM_FAIL_NUM(n)     (g_stCdsStats.ulDLIpfSaveSrcMemFail   += (n))
#define  CDS_DBG_DL_IPF_SAVE_SRCMEM_SUCC_NUM(n)     (g_stCdsStats.ulDLIpfSaveSrcMemSucc += (n))
#define  CDS_DBG_DL_IPF_FREE_SRCMEM_NUM(n)          (g_stCdsStats.ulDLIpfFreeSrcMemNum   += (n))
#define  CDS_DBG_DL_IPF_FREE_SRCMEM_ERROR(n)        (g_stCdsStats.ulDLIpfFreeSrcMemErr   += (n))
/*���Ž�ģʽ*/
#define  CDS_DBG_DL_IPF_CFG_BD_ALLOC_MEM_FAIL(n)    (g_stCdsStats.ulDLIpfCfgBDAllocMemFail += (n))
#define  CDS_DBG_DL_IPF_CFG_BD_SDU_LEN_ERROR(n)     (g_stCdsStats.ulDLIpfCfgBDSduLenErr += (n))


/*����ģʽ��������*/
#define  CDS_DBG_LB_DL_RX_SDU_FROM_GSM(n)           (g_stCdsStats.ulLBDLRxSduInGSM  += (n))
#define  CDS_DBG_LB_DL_RX_SDU_FROM_UMTS(n)          (g_stCdsStats.ulLBDLRxSduInUMTS  += (n))
#define  CDS_DBG_LB_DL_RX_SDU_FROM_LTE(n)           (g_stCdsStats.ulLBDLRxSduInLte  += (n))
#define  CDS_DBG_LB_DL_RX_SDU_IN_NO_MODE_B(n)       (g_stCdsStats.ulLBDLRxSduInNonModeB  += (n))
#define  CDS_DBG_LB_DL_ALLOC_MEM_FAIL(n)            (g_stCdsStats.ulLBDLAllocMemFail += (n))
#define  CDS_DBG_LB_DL_ENQUE_SUCC_NUM(n)            (g_stCdsStats.ulLBDDLEnQueSucc += (n))
#define  CDS_DBG_LB_DL_ENQUE_FAIL_NUM(n)            (g_stCdsStats.ulLBDLEnQueFail += (n))
#define  CDS_DBG_LB_UL_SOFT_FILTER_FAIL_NUM(n)      (g_stCdsStats.ulLBULSoftFilterFail += (n))
#define  CDS_DBG_LB_UL_SEND_PKT_NUM(n)              (g_stCdsStats.ulLBUlSendPktNum += (n))
#define  CDS_DBG_LB_UL_CLEAR_PKT_NUM(n)             (g_stCdsStats.ulLBUlClearPktNum += (n))


/*����IP������ͳ����*/
#define  CDS_DBG_UL_AS_RX_IP_PKT_NUM(n)             (g_stCdsStats.ulULASRxIpPktNum += (n))
#define  CDS_DBG_UL_DIRECT_FREE_PKT_NUM(n)          (g_stCdsStats.ulULDirectFreePktNum += (n))
#define  CDS_DBG_UL_LTE_RX_PKT_NUM(n)               (g_stCdsStats.ulULLTERxPktNum += (n))
#define  CDS_DBG_UL_GU_RX_PKT_NUM(n)                (g_stCdsStats.ulULGURxPktNum  += (n))
#define  CDS_DBG_UL_NULL_RX_PKT_NUM(n)              (g_stCdsStats.ulULNULLRxPktNum  += (n))
#define  CDS_DBG_UL_RAN_MODE_ERROR_FREE_NUM(n)      (g_stCdsStats.ulULRanModeErrFreeNum  += (n))
/*LTEģ����ͳ����*/
#define  CDS_DBG_UL_LTE_FC_FREE_PKT_NUM(n)          (g_stCdsStats.ulULLTEFcFreePktNum += (n))
#define  CDS_DBG_UL_LTE_PROC_IPSEG_FAIL_NUM(n)      (g_stCdsStats.ulULLTEProcIpSegFail += (n))
#define  CDS_DBG_UL_LTE_SEND_TO_LPDCP_NUM(n)        (g_stCdsStats.ulULLTESendToLPdcpNum += (n))
#define  CDS_DBG_UL_LTE_SAVE_PKT_NUM(n)             (g_stCdsStats.ulULLTESavePktNum  += (n))
#define  CDS_DBG_UL_LTE_ERABM_STATE_ERROR(n)        (g_stCdsStats.ulULLTEERabmStateErr += (n))
/*GUģ����ͳ����*/
#define  CDS_DBG_UL_GU_FC_FREE_PKT_NUM(n)           (g_stCdsStats.ulULGUFcFreePktNum += (n))
#define  CDS_DBG_UL_GU_SEND_TO_RABM_NUM(n)          (g_stCdsStats.ulULGUSendToRabmNum  += (n))
#define  CDS_DBG_UL_GU_SAVE_PKT_NUM(n)              (g_stCdsStats.ulULGUSavePktNum  += (n))
#define  CDS_DBG_UL_GU_RABM_STATE_ERROR(n)          (g_stCdsStats.ulULGURabmStateErr += (n))
/*���л���ͳ����*/
#define  CDS_DBG_UL_BUFF_EN_QUE_SUCC_NUM(n)         (g_stCdsStats.ulULBuffEnQueSucc += (n))
#define  CDS_DBG_UL_BUFF_EN_QUE_FAIL_NUM(n)         (g_stCdsStats.ulULBuffEnQueFail += (n))
#define  CDS_DBG_UL_BUFF_SOFT_FILTER_FAIL_NUM(n)    (g_stCdsStats.ulULBuffSoftFilterFail += (n))
#define  CDS_DBG_UL_BUFF_SEND_PKT_SUCC_NUM(n)       (g_stCdsStats.ulULBuffSendPktSuccNum += (n))
#define  CDS_DBG_UL_BUFF_FREE_PKT_NUM(n)            (g_stCdsStats.ulULBuffFreePktNum += (n))

/*��������ͳ����*/
#define  CDS_DBG_UL_SOFT_FILTER_PKT_NUM(n)          (g_stCdsStats.ulULSoftFilterPktNum += (n))
#define  CDS_DBG_UL_SOFT_FILTER_IPV4_PKT_NUM(n)     (g_stCdsStats.ulULSoftFilterIPv4Num += (n))
#define  CDS_DBG_UL_SOFT_FILTER_IPV6_PKT_NUM(n)     (g_stCdsStats.ulULSoftFilterIPv6Num += (n))
#define  CDS_DBG_UL_SOFT_FILTER_FAIL_NUM(n)         (g_stCdsStats.ulULSoftFilterFailNum += (n))
#define  CDS_DBG_UL_SOFT_FILTER_IPSEG_PKT_NUM(n)    (g_stCdsStats.ulULSoftFilterIPSegNum += (n))
#define  CDS_DBG_UL_SOFT_FILTER_IPSEG_SUCC_NUM(n)   (g_stCdsStats.ulULSoftFilterIPSegSucc += (n))
#define  CDS_DBG_UL_SOFT_FILTER_IPSEG_FAIL_NUM(n)   (g_stCdsStats.ulULSoftFilterIPSegFail += (n))


/*����SDUͳ����*/
#define  CDS_DBG_DL_GSM_RX_SDU_NUM(n)               (g_stCdsStats.ulDLGSMRxSduNum += (n))
#define  CDS_DBG_DL_GSM_RX_IMS_PKT_NUM(n)           (g_stCdsStats.ulDLGSMRxImsNum += (n))
#define  CDS_DBG_DL_GSM_FC_FREE_SDU_NUM(n)          (g_stCdsStats.ulDLGSMFcFreeSduNum += (n))
#define  CDS_DBG_DL_GSM_ENQUE_SUCC_NUM(n)           (g_stCdsStats.ulDLGSMEnQueSucc += (n))
#define  CDS_DBG_DL_GSM_ENQUE_FAIL_NUM(n)           (g_stCdsStats.ulDLGSMEnQueFail += (n))
#define  CDS_DBG_DL_GSM_GET_DEFT_RABID_FAIL(n)      (g_stCdsStats.ulDLGSMGetDeftRabIdFail += (n))

#define  CDS_DBG_DL_UMTS_RX_SDU_NUM(n)              (g_stCdsStats.ulDLUMTSRxSduNum += (n))
#define  CDS_DBG_DL_UMTS_RX_IMS_PKT_NUM(n)           (g_stCdsStats.ulDLUMTSRxImsNum += (n))
#define  CDS_DBG_DL_UMTS_FC_FREE_SDU_NUM(n)         (g_stCdsStats.ulDLUMTSFcFreeSduNum += (n))
#define  CDS_DBG_DL_UMTS_DIRECT_FREE_SDU_NUM(n)     (g_stCdsStats.ulDLUMTSDirectFreeSduNum += (n))
#define  CDS_DBG_DL_UMTS_ENQUE_SUCC_NUM(n)          (g_stCdsStats.ulDLUMTSEnQueSucc += (n))
#define  CDS_DBG_DL_UMTS_ENQUE_FAIL_NUM(n)          (g_stCdsStats.ulDLUMTSEnQueFail += (n))
#define  CDS_DBG_DL_UMTS_GET_DEFT_RABID_FAIL(n)     (g_stCdsStats.ulDLUMTSGetDeftRabIdFail += (n))

#define  CDS_DBG_DL_LTE_RX_SDU_NUM(n)               (g_stCdsStats.ulDLLteRxSduNum += (n))
#define  CDS_DBG_DL_LTE_RX_IMS_PKT_NUM(n)           (g_stCdsStats.ulDLLteRxImsNum += (n))
#define  CDS_DBG_DL_LTE_RX_DBG_PKT_NUM(n)           (g_stCdsStats.ulDLLteRxDbgPkt  += (n))
#define  CDS_DBG_DL_LTE_RX_MBMS_SDU_NUM(n)          (g_stCdsStats.ulDLLteRxMbmsSdu  += (n))
#define  CDS_DBG_DL_LTE_FC_FREE_SDU_NUM(n)          (g_stCdsStats.ulDLLteFcFreeSduNum += (n))
#define  CDS_DBG_DL_LTE_DIRECT_FREE_SDU_NUM(n)      (g_stCdsStats.ulDLLteDirectFreeSduNum += (n))
#define  CDS_DBG_DL_LTE_GET_EPSBID_FAIL(n)          (g_stCdsStats.ulDLLteGetEpsbIDFail += (n))
#define  CDS_DBG_DL_LTE_GET_DEFT_EPSBID_FAIL(n)     (g_stCdsStats.ulDLLteGetDeftEpsbIDFail += (n))
#define  CDS_DBG_DL_LTE_ENQUE_SUCC_NUM(n)           (g_stCdsStats.ulDLLteEnQueSucc += (n))
#define  CDS_DBG_DL_LTE_ENQUE_FAIL_NUM(n)           (g_stCdsStats.ulDLLteEnQueFail += (n))

#define  CDS_DBG_DL_EN_SDU_QUEUE_SUCC_NUM(n)        (g_stCdsStats.ulDLEnSduQueSucc  += (n))
#define  CDS_DBG_DL_EN_SDU_QUEUE_FAIL_NUM(n)        (g_stCdsStats.ulDLEnSduQueFail  += (n))
#define  CDS_DBG_DL_SDU_TRIGGER_EVENT_NUM(n)        (g_stCdsStats.ulDLSduTrigEvent += (n))


/*IMS���ͳ����*/
#define  CDS_DBG_IMS_UL_RX_FUN_CALL_NUM(n)          (g_stCdsStats.ulImsULRxFunCallNum += (n))
#define  CDS_DBG_IMS_UL_RX_FUN_PARA_ERR(n)          (g_stCdsStats.ulImsULRxFunParaErr += (n))
#define  CDS_DBG_IMS_UL_RX_NIC_PKT_NUM(n)           (g_stCdsStats.ulImsULRxNicPktNum += (n))
#define  CDS_DBG_IMS_UL_ALLOC_MEM_FAIL_NUM(n)       (g_stCdsStats.ulImsULAllocMemFail += (n))
#define  CDS_DBG_IMS_UL_EN_QUE_FAIL_NUM(n)          (g_stCdsStats.ulImsULEnQueFail += (n))
#define  CDS_DBG_IMS_UL_EN_QUE_SUCC_NUM(n)          (g_stCdsStats.ulImsULEnQueSucc += (n))
#define  CDS_DBG_IMS_UL_TRIGGER_IMS_EVENT_NUM(n)    (g_stCdsStats.ulImsULTrigImsEvent += (n))
#define  CDS_DBG_IMS_UL_PROC_IMS_EVENT_NUM(n)       (g_stCdsStats.ulImsULProcImsEvent += (n))
#define  CDS_DBG_IMS_UL_SOFT_FILTER_FAIL_NUM(n)     (g_stCdsStats.ulImsULSoftFilterFail += (n))
#define  CDS_DBG_IMS_UL_SEND_PKT_TO_RAN_NUM(n)      (g_stCdsStats.ulImsULSendToRan += (n))

#define  CDS_DBG_IMS_DL_RX_IMS_PKT_NUM(n)           (g_stCdsStats.ulImsDLRxImsPkt += (n))
#define  CDS_DBG_IMS_DL_RX_ND_PKT_NUM(n)            (g_stCdsStats.ulImsDLNdPktNum += (n))
#define  CDS_DBG_IMS_DL_ALLOC_MEM_FAIL_NUM(n)       (g_stCdsStats.ulImsDLAllocMemFail += (n))
#define  CDS_DBG_IMS_DL_GET_DEST_PORT_FAIL_NUM(n)   (g_stCdsStats.ulImsDLGetDestPortFail += (n))
#define  CDS_DBG_IMS_DL_SEND_TO_NIC_SUCC_NUM(n)     (g_stCdsStats.ulImsDLSendToNicSucc += (n))
#define  CDS_DBG_IMS_DL_SEND_TO_NIC_FAIL_NUM(n)     (g_stCdsStats.ulImsDLSendToNicFail += (n))

#define  CDS_DBG_IMS_DL_RX_IMS_Ut_PKT_NUM(n)        (g_stCdsStats.ulImsDLRxUtPkt += (n))
#define  CDS_DBG_IMS_DL_Ut_PKT_PROC_ERR_NUM(n)      (g_stCdsStats.ulImsDLUtPktProcErrNum += (n))
#define  CDS_DBG_IMS_DL_Ut_PKT_PROC_SUCC_NUM(n)     (g_stCdsStats.ulImsDLUtPktProcSuccNum += (n))

/*Modem GU����ͳ����*/
#define  CDS_DBG_MODEM_UL_GU_RX_PKT_NUM(ModemId,n)                (g_stCdsStats.astModemStats[ModemId].ulULGURxPktNum  += (n))
#define  CDS_DBG_MODEM_UL_GU_FC_FREE_PKT_NUM(ModemId,n)           (g_stCdsStats.astModemStats[ModemId].ulULGUFcFreePktNum += (n))
#define  CDS_DBG_MODEM_UL_GU_SEND_TO_RABM_NUM(ModemId,n)          (g_stCdsStats.astModemStats[ModemId].ulULGUSendToRabmNum  += (n))
#define  CDS_DBG_MODEM_UL_GU_SAVE_PKT_NUM(ModemId,n)              (g_stCdsStats.astModemStats[ModemId].ulULGUSavePktNum  += (n))
#define  CDS_DBG_MODEM_UL_GU_RABM_STATE_ERROR(ModemId,n)          (g_stCdsStats.astModemStats[ModemId].ulULGURabmStateErr += (n))

/*Modem GU����ͳ����*/
#define  CDS_DBG_MODEM_DL_GSM_RX_SDU_NUM(ModemId,n)               (g_stCdsStats.astModemStats[ModemId].ulDLGSMRxSduNum += (n))
#define  CDS_DBG_MODEM_DL_GSM_RX_IMS_PKT_NUM(ModemId,n)           (g_stCdsStats.astModemStats[ModemId].ulDLGSMRxImsNum += (n))
#define  CDS_DBG_MODEM_ID_LB_DL_RX_SDU_FROM_GSM(ModemId,n)        (g_stCdsStats.astModemStats[ModemId].ulLBDLRxSduInGSM  += (n))
#define  CDS_DBG_MODEM_DL_GSM_FC_FREE_SDU_NUM(ModemId,n)          (g_stCdsStats.astModemStats[ModemId].ulDLGSMFcFreeSduNum += (n))
#define  CDS_DBG_MODEM_DL_GSM_ENQUE_SUCC_NUM(ModemId,n)           (g_stCdsStats.astModemStats[ModemId].ulDLGSMEnQueSucc += (n))
#define  CDS_DBG_MODEM_DL_GSM_ENQUE_FAIL_NUM(ModemId,n)           (g_stCdsStats.astModemStats[ModemId].ulDLGSMEnQueFail += (n))

#define  CDS_DBG_MODEM_DL_UMTS_RX_SDU_NUM(ModemId,n)              (g_stCdsStats.astModemStats[ModemId].ulDLUMTSRxSduNum += (n))
#define  CDS_DBG_MODEM_DL_UMTS_RX_IMS_PKT_NUM(ModemId,n)          (g_stCdsStats.astModemStats[ModemId].ulDLUMTSRxImsNum += (n))
#define  CDS_DBG_MODEM_ID_LB_DL_RX_SDU_FROM_UMTS(ModemId,n)       (g_stCdsStats.astModemStats[ModemId].ulLBDLRxSduInGSM  += (n))
#define  CDS_DBG_MODEM_DL_UMTS_FC_FREE_SDU_NUM(ModemId,n)         (g_stCdsStats.astModemStats[ModemId].ulDLUMTSFcFreeSduNum += (n))
#define  CDS_DBG_MODEM_DL_UMTS_DIRECT_FREE_SDU_NUM(ModemId,n)     (g_stCdsStats.astModemStats[ModemId].ulDLUMTSDirectFreeSduNum += (n))
#define  CDS_DBG_MODEM_DL_UMTS_ENQUE_SUCC_NUM(ModemId,n)          (g_stCdsStats.astModemStats[ModemId].ulDLUMTSEnQueSucc += (n))
#define  CDS_DBG_MODEM_DL_UMTS_ENQUE_FAIL_NUM(ModemId,n)          (g_stCdsStats.astModemStats[ModemId].ulDLUMTSEnQueFail += (n))


/*****************************************************************************
  8 ��������
*****************************************************************************/
extern VOS_VOID Cds_LomTraceRcvUlData(VOS_VOID);
extern VOS_VOID Cds_LomTraceSendDlData(VOS_VOID);


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif

