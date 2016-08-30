


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "CdsDebug.h"
#include "CDS.h"
#include "CdsIpfCtrl.h"
#include "CdsIpFragmentProc.h"


CDS_STATS_INFO_STRU    g_stCdsStats = {0};

VOS_UINT32   g_ulCdsOmSwitch = 0;

/*****************************************************************************
  2 �ⲿ��������
*****************************************************************************/

/******************************************************************************
   3 ˽�ж���
******************************************************************************/


/******************************************************************************
   4 ȫ�ֱ�������
******************************************************************************/


/******************************************************************************
   5 ����ʵ��
******************************************************************************/


VOS_VOID CDS_SetDbgRptBearerId(VOS_UINT8  ucEpsbId)
{
    /*LTEĬ�Ϸ���Modem0*/
    g_astCdsEntity[MODEM_ID_0].ucDbgBearerId = ucEpsbId;
    return;
}
VOS_VOID CDS_SetMbmsBearerId(VOS_UINT8  ucEpsbId)
{
    /*LTEĬ�Ϸ���Modem0*/
    g_astCdsEntity[MODEM_ID_0].ucMbmsBearerId = ucEpsbId;
    return;
}
VOS_VOID CDS_SetPktDiscardFlg(MODEM_ID_ENUM_UINT16 enModemId,VOS_UINT32  ulDir, VOS_UINT32 ulFlg)
{
    CDS_ENTITY_STRU         *pstCdsEntity;

    pstCdsEntity = CDS_GetCdsEntity(enModemId);
    if (VOS_NULL_PTR == pstCdsEntity)
    {
        return;
    }

    if (0 == ulDir)
    {
        pstCdsEntity->ulULPktDiscardFlg = ulFlg;
    }
    else
    {
        pstCdsEntity->ulDLPktDiscardFlg = ulFlg;
    }

    return ;
}


VOS_VOID CDS_ShowEventStats(VOS_VOID)
{

    vos_printf("�����յ�IPF�жϴ���                          %d\n",g_stCdsStats.ulIpfULIntNum);
    vos_printf("���д���IPF�жϴ���                          %d\n",g_stCdsStats.ulULProcIpfIntNum);
    vos_printf("�����յ�ADQ���жϴ���                        %d\n",g_stCdsStats.ulUlAdqEmtypIntNum);
    vos_printf("���д���ADQ���жϴ���                        %d\n",g_stCdsStats.ulUlProcAdqEmptyIntNum);
    vos_printf("����IPF�����ڴ��ͷź����ص�����              %d\n",g_stCdsStats.ulIpfDlCallMemFreeNum);
    vos_printf("����IPF�����ڴ溯���ͷ��ڴ����              %d\n",g_stCdsStats.ulIpfDlFreeMemNum);
    vos_printf("����10ms�����Զ�ʱ���������д����¼�����     %d\n",g_stCdsStats.ulDL10msTmrTrigEvent);
    vos_printf("����1ms��֡�жϴ������д����¼�����          %d\n",g_stCdsStats.ulDL1msIntTrigEvent);
    vos_printf("����UMTS DSP�жϴ������д����¼�����         %d\n",g_stCdsStats.ulDLUmtsIntTrigEvent);
    vos_printf("���д������д����¼�����                     %d\n",g_stCdsStats.ulDLProcEventNum);
    vos_printf("\r\n");

    return;
}


VOS_VOID CDS_ShowUlRdProcStats(VOS_VOID)
{
    vos_printf("����IPF�յ�RD����                            %d\n",g_stCdsStats.ulULIpfRxRDNum);
    vos_printf("����RD�ڴ�ת��ʧ�ܸ���                       %d\n",g_stCdsStats.ulULIpfTransMemFailNum);
    vos_printf("����IPF RD ModemID�������                   %d\n",g_stCdsStats.ulULIpfModemIdErrNum);
    vos_printf("���л���ģʽ�ͷ�RD����                       %d\n",g_stCdsStats.ulULIpfFreePktInLB);
    vos_printf("�����յ�IP������                             %d\n",g_stCdsStats.ulULIpfRxIpPktNum);
    vos_printf("�����յ�IP��Ƭ������                         %d\n",g_stCdsStats.ulULIpfRxIpSegNum);
    vos_printf("�����յ�NDIS������                           %d\n",g_stCdsStats.ulULIpfRxNdisPktNum);
    vos_printf("����IPF���˴������                          %d\n",g_stCdsStats.ulULIpfFilterErrNum);

    vos_printf("\r\n");
    return;
}


VOS_VOID CDS_ShowUlAdqProcStats(VOS_VOID)
{

    vos_printf("����ADQ�����ڴ����                          %d\n",g_stCdsStats.ulULADQAllocMemNum);
    vos_printf("����ADQ����ʧ���ͷ��ڴ����                  %d\n",g_stCdsStats.ulULADQFreeMemNum);
    vos_printf("��������AD 0�ڴ����                         %d\n",g_stCdsStats.ulULADQCfgAd0Num);
    vos_printf("��������AD 1�ڴ����                         %d\n",g_stCdsStats.ulULADQCfgAd1Num);
    vos_printf("��������AD�ڴ�����(AD0+AD1)                  %d\n",g_stCdsStats.ulULADQCfgAdNum);
    vos_printf("��������ADQ�ɹ�����                          %d\n",g_stCdsStats.ulULADQCfgAdSucc);
    vos_printf("��������ADQʧ�ܴ���                          %d\n",g_stCdsStats.ulULADQCfgAdFail);
    vos_printf("��������ADQ�ձ�����ʱ������                  %d\n",g_stCdsStats.ulULADQStartEmptyTmrNum);
    vos_printf("����ֹͣADQ�ձ�����ʱ������                  %d\n",g_stCdsStats.ulULADQStopEmptyTmrNum);
    vos_printf("����ADQ�ձ�����ʱ����ʱ����                  %d\n",g_stCdsStats.ulULADQEmptyTmrTimeoutNum);
    vos_printf("����IPF ADQ���жϸ���                        %d\n",g_stCdsStats.ulUlAdqEmtypIntNum);
    vos_printf("����IPF AD 0������жϸ���                   %d\n",g_stCdsStats.ulULADQAd0EmptyIntNum);
    vos_printf("����IPF AD 1������жϸ���                   %d\n",g_stCdsStats.ulULADQAd1EmptyIntNum);

    vos_printf("\r\n");
    return;
}



VOS_VOID CDS_ShowUlIpPktProcStats(VOS_VOID)
{

    vos_printf("���н�����յ���IP���ܺ�                     %d\n",g_stCdsStats.ulULASRxIpPktNum);
    vos_printf("���д�׮�ͷŵ�IP������                       %d\n",g_stCdsStats.ulULDirectFreePktNum);
    vos_printf("����LTEģ���յ�IP������                      %d\n",g_stCdsStats.ulULLTERxPktNum);
    vos_printf("����GUģ���յ�IP������                       %d\n",g_stCdsStats.ulULGURxPktNum);
    vos_printf("����NULLģ����IP������                       %d\n",g_stCdsStats.ulULNULLRxPktNum);
    vos_printf("���н���ģʽ�����ͷŵ�IP������               %d\n",g_stCdsStats.ulULRanModeErrFreeNum);
    vos_printf("\r\n");

    vos_printf("LTE���������ͷŵ�IP������                    %d\n",g_stCdsStats.ulULLTEProcIpSegFail);
    vos_printf("LTE���д���IP��Ƭʧ��IP������                %d\n",g_stCdsStats.ulULLTEProcIpSegFail);
    vos_printf("LTE���з��͵�LPDCP��IP������                 %d\n",g_stCdsStats.ulULLTESendToLPdcpNum);
    vos_printf("LTE���л���IP������                          %d\n",g_stCdsStats.ulULLTESavePktNum);
    vos_printf("LTE����ERABM״̬�����ͷŵ�IP������           %d\n",g_stCdsStats.ulULLTEERabmStateErr);
    vos_printf("\r\n");

    vos_printf("GU���������ͷŵ�IP������                     %d\n",g_stCdsStats.ulULGUFcFreePktNum);
    vos_printf("GU���з��͵�RABM��IP������                   %d\n",g_stCdsStats.ulULGUSendToRabmNum);
    vos_printf("GU���л���IP������                           %d\n",g_stCdsStats.ulULGUSavePktNum);
    vos_printf("GU����RABM״̬�����ͷŵ�IP������             %d\n",g_stCdsStats.ulULGURabmStateErr);
    vos_printf("\r\n");

    vos_printf("���л�����ӳɹ�����                         %d\n",g_stCdsStats.ulULBuffEnQueSucc);
    vos_printf("���л������ʧ�ܸ���                         %d\n",g_stCdsStats.ulULBuffEnQueFail);
    vos_printf("���л��������ʧ�ܸ���                       %d\n",g_stCdsStats.ulULBuffSoftFilterFail);
    vos_printf("���л��淢��IP���ɹ�����                     %d\n",g_stCdsStats.ulULBuffSendPktSuccNum);
    vos_printf("���л����ͷ�IP������                         %d\n",g_stCdsStats.ulULBuffFreePktNum);
    vos_printf("\r\n");

    vos_printf("����IP������ͳ���ܺ�\n");
    vos_printf("�����յ�NDCLIENT������                       %d\n",g_stCdsStats.ulULRecvNdPktNum);
    vos_printf("�����յ��������ݰ�����                       %d\n",g_stCdsStats.ulLBUlSendPktNum);
    vos_printf("�����յ�RD���ݰ�����                         %d\n",g_stCdsStats.ulULIpfRxIpPktNum + g_stCdsStats.ulULIpfRxIpSegNum);
    vos_printf("���л����������·��͸���                     %d\n",g_stCdsStats.ulULBuffSendPktSuccNum);
    vos_printf("���а�ģʽ�ַ������ݰ��ܺ�(ND+LB+IP+BUFF)    %d\n",g_stCdsStats.ulULASRxIpPktNum);

    return;
}


VOS_VOID CDS_ShowDlSduProcStats(VOS_VOID)
{

    vos_printf("����SDU����ͳ��\n\n");

    vos_printf("����GSMģ����SDU����                         %d\n",g_stCdsStats.ulDLGSMRxSduNum);
    vos_printf("����GSMģ����IMS���ݰ�����                   %d\n",g_stCdsStats.ulDLGSMRxImsNum);
    vos_printf("����GSMģ���ض���SDU����                     %d\n",g_stCdsStats.ulDLGSMFcFreeSduNum);
    vos_printf("����GSMģ����ģʽ�½���SDU����               %d\n",g_stCdsStats.ulLBDLRxSduInGSM);
    vos_printf("����GSM��ȡȱʡRABIDʧ�ܸ���                 %d\n",g_stCdsStats.ulDLGSMGetDeftRabIdFail);
    vos_printf("����GSMģ��ӳɹ�����                        %d\n",g_stCdsStats.ulDLGSMEnQueSucc);
    vos_printf("����GSMģ���ʧ�ܸ���                        %d\n",g_stCdsStats.ulDLGSMEnQueFail);
    vos_printf("\r\n");

    vos_printf("����UMTSģ����SDU����                        %d\n",g_stCdsStats.ulDLUMTSRxSduNum);
    vos_printf("����UMTSģ����IMS���ݰ�����                  %d\n",g_stCdsStats.ulDLUMTSRxImsNum);
    vos_printf("����UMTSģ���ض���SDU����                    %d\n",g_stCdsStats.ulDLUMTSFcFreeSduNum);
    vos_printf("����UMTSģ����ģʽ�½���SDU����              %d\n",g_stCdsStats.ulLBDLRxSduInUMTS);
    vos_printf("����UMTSģ��׮ֱ���ͷ�SDU����                %d\n",g_stCdsStats.ulDLUMTSDirectFreeSduNum);
    vos_printf("����UMTS��ȡȱʡRABIDʧ�ܸ���                %d\n",g_stCdsStats.ulDLUMTSGetDeftRabIdFail);
    vos_printf("����UMTSģ��ӳɹ�����                       %d\n",g_stCdsStats.ulDLUMTSEnQueSucc);
    vos_printf("����UMTSģ���ʧ�ܸ���                       %d\n",g_stCdsStats.ulDLUMTSEnQueFail);
    vos_printf("\r\n");

    vos_printf("����LTEģ����SDU����                         %d\n",g_stCdsStats.ulDLLteRxSduNum);
    vos_printf("����LTEģ����IMS���ݰ�����                   %d\n",g_stCdsStats.ulDLLteRxImsNum);
    vos_printf("����LTEģ���ض���SDU����                     %d\n",g_stCdsStats.ulDLLteFcFreeSduNum);
    vos_printf("����LTEģ��׮ֱ���ͷ�SDU����                 %d\n",g_stCdsStats.ulDLLteDirectFreeSduNum);
    vos_printf("����LTEģ����ģʽ�½���SDU����               %d\n",g_stCdsStats.ulLBDLRxSduInLte);
    vos_printf("����LTE��ȡ��ӦEPSBIDʧ�ܸ���                %d\n",g_stCdsStats.ulDLLteGetEpsbIDFail);
    vos_printf("����LTE��ȡȱʡEPSBIDʧ�ܸ���                %d\n",g_stCdsStats.ulDLLteGetDeftEpsbIDFail);
    vos_printf("����LTE����L2 Debug���ݰ�����                %d\n",g_stCdsStats.ulDLLteRxDbgPkt);
    vos_printf("����LTE����MBMS���ݰ�����                    %d\n",g_stCdsStats.ulDLLteRxMbmsSdu);
    vos_printf("����LTEģ��ӳɹ�����                        %d\n",g_stCdsStats.ulDLLteEnQueSucc);
    vos_printf("����LTEģ���ʧ�ܸ���                        %d\n",g_stCdsStats.ulDLLteEnQueFail);


    vos_printf("\r\n");

    vos_printf("����SDU���е�ǰ�������ݰ�����               %d\n",LUP_QueCnt(CDS_GET_IPF_DL_SDU_QUE()));
    vos_printf("����SDU��ӳɹ�����                          %d\n",g_stCdsStats.ulDLEnSduQueSucc);
    vos_printf("����SDU���ʧ������                          %d\n",g_stCdsStats.ulDLEnSduQueFail);
    vos_printf("\r\n");

    return;
}


VOS_VOID CDS_ShowDlIpfProcStats(VOS_VOID)
{
    vos_printf("����IPF����ͳ��\n\n");

    vos_printf("��������IPFû��BD����                       %d\n",g_stCdsStats.ulDLIpfCfgHaveNoBD);
    vos_printf("��������IPFû��CD����                       %d\n",g_stCdsStats.ulDLIpfCfgHaveNoCD);
    vos_printf("��������IPF CD��������                      %d\n",g_stCdsStats.ulDLIpfCfgCDNotEnough);
    vos_printf("��������BD�ɹ�����                          %d\n",g_stCdsStats.ulDLIpfCfgBDSucc);
    vos_printf("��������BDʧ�ܸ���                          %d\n",g_stCdsStats.ulDLIpfCfgBDFail);
    vos_printf("\r\n");

    vos_printf("�����Ž�ģʽ����Դ�ڴ�ɹ�����              %d\n",g_stCdsStats.ulDLIpfSaveSrcMemSucc);
    vos_printf("�����Ž�ģʽ����Դ�ڴ�ʧ�ܸ���              %d\n",g_stCdsStats.ulDLIpfSaveSrcMemFail);
    vos_printf("�����Ž�ģʽԴ�ڴ��ͷŶ��е�ǰ����          %d\n",LUP_QueCnt(CDS_IPF_DL_SRCMEM_FREE_QUE()));
    vos_printf("�����Ž�ģʽ�ͷ�Դ�ڴ�ɹ�����              %d\n",g_stCdsStats.ulDLIpfFreeSrcMemNum);
    vos_printf("�����Ž�ģʽ�ͷ�Դ�ڴ�ʧ�ܸ���              %d\n",g_stCdsStats.ulDLIpfFreeSrcMemErr);
    vos_printf("\r\n");

    vos_printf("���з��Ž�ģʽIPF�����ڴ�ʧ�ܴ���           %d\n",g_stCdsStats.ulDLIpfCfgBDAllocMemFail);
    vos_printf("���з��Ž�ģʽSDU���ȴ������               %d\n",g_stCdsStats.ulDLIpfCfgBDSduLenErr);
    vos_printf("���з��Ž�ģʽIPF�����ڴ��ͷź����ص�����   %d\n",g_stCdsStats.ulIpfDlCallMemFreeNum);
    vos_printf("���з��Ž�ģʽ�ͷ��ڴ����                  %d\n",g_stCdsStats.ulIpfDlFreeMemNum);

    vos_printf("\r\n");

    return;
}



VOS_VOID CDS_ShowLBProcStats(VOS_VOID)
{
    vos_printf("����ģʽ����ͳ��\n\n");
    vos_printf("�������д�GSM���յ�SDU����                  %d\n",g_stCdsStats.ulLBDLRxSduInGSM);
    vos_printf("�������д�UMTS���յ�SDU����                 %d\n",g_stCdsStats.ulLBDLRxSduInUMTS);
    vos_printf("�������д�LTE���յ�SDU����                  %d\n",g_stCdsStats.ulLBDLRxSduInLte);
    vos_printf("�������������ڴ�ʧ�ܵĸ���                  %d\n",g_stCdsStats.ulLBDLAllocMemFail);
    vos_printf("����������ӳɹ�����                        %d\n",g_stCdsStats.ulLBDDLEnQueSucc);
    vos_printf("�����������ʧ�ܸ���                        %d\n",g_stCdsStats.ulLBDLEnQueFail);
    vos_printf("�������������ʧ�ܸ���                      %d\n",g_stCdsStats.ulLBULSoftFilterFail);
    vos_printf("�������з��͵����ݰ�����                    %d\n",g_stCdsStats.ulLBUlSendPktNum);
    vos_printf("����������ն����ͷ����ݰ�����              %d\n",g_stCdsStats.ulLBUlClearPktNum);

    vos_printf("\r\n");

    return;
}



VOS_VOID CDS_ShowModemGUProcStats(VOS_UINT16 usModemId)
{

    if (usModemId >= MODEM_ID_BUTT)
    {
        vos_printf("ModemId�Ƿ�������ModemId=%d\n",usModemId);
        return;
    }

    vos_printf("Modem %d�ϵ�GU����ͳ����Ϣ\n",usModemId);
    vos_printf("����GUģ���յ�IP������                       %d\n",g_stCdsStats.astModemStats[usModemId].ulULGURxPktNum);
    vos_printf("����GU�����ͷŵ�IP������                     %d\n",g_stCdsStats.astModemStats[usModemId].ulULGUFcFreePktNum);
    vos_printf("����GU���͵�RABM��IP������                   %d\n",g_stCdsStats.astModemStats[usModemId].ulULGUSendToRabmNum);
    vos_printf("����GU����IP������                           %d\n",g_stCdsStats.astModemStats[usModemId].ulULGUSavePktNum);
    vos_printf("����GU RABM״̬�����ͷŵ�IP������            %d\n",g_stCdsStats.astModemStats[usModemId].ulULGURabmStateErr);
    vos_printf("\r\n");

    vos_printf("����GSMģ����SDU����                         %d\n",g_stCdsStats.astModemStats[usModemId].ulDLGSMRxSduNum);
    vos_printf("����GSMģ����IMS���ݰ�����                   %d\n",g_stCdsStats.astModemStats[usModemId].ulDLGSMRxImsNum);
    vos_printf("����GSMģ���ض���SDU����                     %d\n",g_stCdsStats.astModemStats[usModemId].ulDLGSMFcFreeSduNum);
    vos_printf("����GSMģ����ģʽ�½���SDU����               %d\n",g_stCdsStats.astModemStats[usModemId].ulLBDLRxSduInGSM);
    vos_printf("����GSMģ��ӳɹ�����                        %d\n",g_stCdsStats.astModemStats[usModemId].ulDLGSMEnQueSucc);
    vos_printf("����GSMģ���ʧ�ܸ���                        %d\n",g_stCdsStats.astModemStats[usModemId].ulDLGSMEnQueFail);
    vos_printf("\r\n");

    vos_printf("����UMTSģ����SDU����                        %d\n",g_stCdsStats.astModemStats[usModemId].ulDLUMTSRxSduNum);
    vos_printf("����UMTSģ����IMS���ݰ�����                  %d\n",g_stCdsStats.astModemStats[usModemId].ulDLUMTSRxImsNum);
    vos_printf("����UMTSģ���ض���SDU����                    %d\n",g_stCdsStats.astModemStats[usModemId].ulDLUMTSFcFreeSduNum);
    vos_printf("����UMTSģ����ģʽ�½���SDU����              %d\n",g_stCdsStats.astModemStats[usModemId].ulLBDLRxSduInUMTS);
    vos_printf("����UMTSģ��׮ֱ���ͷ�SDU����                %d\n",g_stCdsStats.astModemStats[usModemId].ulDLUMTSDirectFreeSduNum);
    vos_printf("����UMTSģ��ӳɹ�����                       %d\n",g_stCdsStats.astModemStats[usModemId].ulDLUMTSEnQueSucc);
    vos_printf("����UMTSģ���ʧ�ܸ���                       %d\n",g_stCdsStats.astModemStats[usModemId].ulDLUMTSEnQueFail);
    vos_printf("\r\n");

    vos_printf("\r\n");

    return;
}



VOS_VOID CDS_ShowCdsEntityInfo(VOS_UINT16 usModemId)
{
    char    strRanMode[][20]  = {"NULL","LTE","GU","��Ч����ģʽ"};
    char    strLBState[][20]  = {"����ģʽ","����ģʽ"};


    CDS_ENTITY_STRU         *pstCdsEntity;
    VOS_UINT32               ulCnt;

    pstCdsEntity = CDS_GetCdsEntity(usModemId);
    if (VOS_NULL_PTR == pstCdsEntity)
    {
        vos_printf("ModemId�Ƿ�������ModemId=%d\n",usModemId);
        return;
    }

    vos_printf("��ʾModem%d��CDSʵ��״̬  \r\n",usModemId);
    vos_printf("��ǰ����ģʽ              : %s\r\n",strRanMode[pstCdsEntity->enRanMode]);
    vos_printf("����SR������־            : %d\r\n",pstCdsEntity->ulServiceReqFlg);
    vos_printf("���л�����л������      : %d\r\n",LUP_QueCnt(pstCdsEntity->pstUlDataQue));
    vos_printf("���л������ݱ�����ʱ��    : 0x%x\r\n",pstCdsEntity->astTimer[CDS_TMR_ID_UL_DATA_PROCTECT].pstTmrHdr);
    vos_printf("����10ms�����Զ�ʱ��      : 0x%x\r\n",pstCdsEntity->astTimer[CDS_TMR_ID_DL_10MS_PERIODIC_TMR].pstTmrHdr);
    vos_printf("����ģʽ״̬              : %s\r\n",strLBState[pstCdsEntity->ulTestModeFlg]);
    vos_printf("����ģʽ����              : %d\r\n",pstCdsEntity->ulLoopBackMode);
    vos_printf("����ģʽB��ʱ��ʱ��       : %d\r\n",pstCdsEntity->astTimer[CDS_TMR_ID_LB_MODE_B_TMR].ulTmrLen);
    vos_printf("���ض��е�ǰ����SDU       : %d\r\n",LUP_QueCnt(pstCdsEntity->pstLBModeBQue));

    vos_printf("�������ݰ�ֱ�Ӷ�������    : %d\r\n",pstCdsEntity->ulULPktDiscardFlg);
    vos_printf("�������ݰ�ֱ�Ӷ�������    : %d\r\n",pstCdsEntity->ulDLPktDiscardFlg);

    vos_printf("\r\n");

    vos_printf("��ʾModem%d��IMS������Ϣ  \r\n",usModemId);

    vos_printf("IMS�˿ںŷ�Χ             :%d - %d\r\n",pstCdsEntity->stImsPortInfo.usMinImsPort,
                                                pstCdsEntity->stImsPortInfo.usMaxImsPort);

    vos_printf("IMS��������               : %d\r\n",pstCdsEntity->ulImsBearerNum);
    for (ulCnt = 0; ulCnt < pstCdsEntity->ulImsBearerNum; ulCnt ++)
    {
        vos_printf("IMS����ID                 : %d\r\n",pstCdsEntity->astImsBearerInfo[ulCnt].ucEpsbId);
        vos_printf("IMS��������               : %d\r\n",pstCdsEntity->astImsBearerInfo[ulCnt].enBearerType);
    }

    vos_printf("\r\n");
    return;
}


VOS_VOID CDS_PrintIpv4Addr(const IPV4_ADDR_UN  *pstIpv4Addr)
{
    CDS_ASSERT(VOS_NULL_PTR != pstIpv4Addr);

    vos_printf("%d.%d.%d.%d\r\n", pstIpv4Addr->aucIPAddr[0],
                           pstIpv4Addr->aucIPAddr[1],
                           pstIpv4Addr->aucIPAddr[2],
                           pstIpv4Addr->aucIPAddr[3]);
    return;
}


VOS_VOID CDS_PrintIpv6Addr(const IPV6_ADDR_UN  *pstIpv6Addr)
{

    VOS_UINT32       ulCnt;

    CDS_ASSERT(VOS_NULL_PTR != pstIpv6Addr);

    for (ulCnt = 0; ulCnt < IPV6_ADDR_LEN - 1; ulCnt ++)
    {
        vos_printf("%.2x:",pstIpv6Addr->aucIPAddr[ulCnt]);
    }

    vos_printf("%.2x\r\n",pstIpv6Addr->aucIPAddr[ulCnt]);

    return;
}

VOS_VOID CDS_ShowSoftFilterIPv4Info(const CDS_SOFTFILTER_INFO_STRU *pstSoftFilter)
{
    /*Service Type*/
    if (PS_TRUE == pstSoftFilter->bitOpTypeOfService)
    {
        vos_printf("TFT Service Type          : %d\r\n",pstSoftFilter->ucTypeOfService);
        vos_printf("TFT Service Type Mask     : %d\r\n",pstSoftFilter->ucTypeOfServiceMask);
    }

    /*Э��ID*/
    if (PS_TRUE == pstSoftFilter->bitOpProtocolId)
    {
        vos_printf("TFT Protocol ID           : %d\r\n",pstSoftFilter->ucTypeOfService);
    }

    /*ԴIP��ַ��Ϣ*/
    if (PS_TRUE == pstSoftFilter->bitOpLcIpv4AddrAndMask)
    {
        vos_printf("TFT IPv4 Src Address      : ");
        CDS_PrintIpv4Addr(&(pstSoftFilter->unLocalIpv4Addr));
        vos_printf("TFT IPv4 Src Address Mask : ");
        CDS_PrintIpv4Addr(&(pstSoftFilter->unLocalIpv4Mask));
    }

    /*Ŀ��IP��ַ��Ϣ*/
    if (PS_TRUE == pstSoftFilter->bitOpRmtIpv4AddrAndMask)
    {
        vos_printf("TFT IPv4 Dest Address     : ");
        CDS_PrintIpv4Addr(&(pstSoftFilter->unRmtIpv4Addr));
        vos_printf("TFT IPv4 Dest Address Mask: ");
        CDS_PrintIpv4Addr(&(pstSoftFilter->unRmtIpv4Mask));
    }

    return;
}



VOS_VOID CDS_ShowSoftFilterIPv6Info(const CDS_SOFTFILTER_INFO_STRU *pstSoftFilter)
{

    /*Service Type*/
    if (PS_TRUE == pstSoftFilter->bitOpTypeOfService)
    {
        vos_printf("TFT Service Type          : %d\r\n",pstSoftFilter->ucTypeOfService);
        vos_printf("TFT Service Type Mask     : %d\r\n",pstSoftFilter->ucTypeOfServiceMask);
    }

    /*Э��ID*/
    if (PS_TRUE == pstSoftFilter->bitOpProtocolId)
    {
        vos_printf("TFT Protocol ID           : %d\r\n",pstSoftFilter->ucTypeOfService);
    }

    /*FlowLable*/
    if (PS_TRUE == pstSoftFilter->bitOpFlowLabelType)
    {
        vos_printf("TFT FlowLable Type        : 0x%.8x\r\n",pstSoftFilter->ulFlowLabelType);
    }

    /*ԴIPv6��ַ*/
    if (PS_TRUE == pstSoftFilter->bitOpLcIpv6AddrAndMask)
    {
        vos_printf("TFT IPv6 Source Address    \r\n");
        CDS_PrintIpv6Addr(&(pstSoftFilter->unLocalIpv6Addr));
        vos_printf("TFT IPv6 Source Address Mask \r\n");
        CDS_PrintIpv6Addr(&(pstSoftFilter->unLocalIpv6Mask));
    }

    /*Ŀ��IPv6��ַ*/
    if (PS_TRUE == pstSoftFilter->bitOpRmtIpv6AddrAndMask)
    {
        vos_printf("TFT IPv6 Dest Address    \r\n");
        CDS_PrintIpv6Addr(&(pstSoftFilter->unRmtIpv6Addr));
        vos_printf("TFT IPv6 Dest Address Mask \r\n");
        CDS_PrintIpv6Addr(&(pstSoftFilter->unRmtIpv6Mask));
    }

    return;
}
VOS_VOID CDS_ShowSoftFilterTransportLayerInfo(const CDS_SOFTFILTER_INFO_STRU *pstSoftFilter)
{

    /*SPI*/
    if (PS_TRUE == pstSoftFilter->bitOpSecuParaIndex)
    {
        vos_printf("TFT IPSec SPI             : 0x%.8x\r\n",pstSoftFilter->ulSecuParaIndex);
    }

    /*Դ�˿���Ϣ*/
    if (PS_TRUE == pstSoftFilter->bitOpSingleLocalPort)
    {
        vos_printf("TFT Source Port           : %d\r\n",pstSoftFilter->usSingleLcPort);
    }

    if (PS_TRUE == pstSoftFilter->bitOpLocalPortRange)
    {
        vos_printf("TFT Source Port Range     : [%d,%d]\r\n",pstSoftFilter->usLcPortLowLimit,pstSoftFilter->usLcPortHighLimit);
    }

    /*Ŀ�Ķ˿���Ϣ*/
    if (PS_TRUE == pstSoftFilter->bitOpSingleRemotePort)
    {
        vos_printf("TFT Dest Port             : %d\r\n",pstSoftFilter->usSingleRmtPort);
    }

    if (PS_TRUE == pstSoftFilter->bitOpRemotePortRange)
    {
        vos_printf("TFT Dest Port Range       : [%d,%d]\r\n",pstSoftFilter->usRmtPortLowLimit,pstSoftFilter->usRmtPortHighLimit);
    }

    return;
}
VOS_VOID CDS_ShowUlSoftFilter(VOS_UINT16 usModemId)
{

    CDS_ENTITY_STRU             *pstCdsEntity;
    CDS_SOFTFILTER_INFO_STRU    *pstSoftFilter;
    VOS_UINT32                   ulCnt;

    pstCdsEntity = CDS_GetCdsEntity(usModemId);
    if (VOS_NULL_PTR == pstCdsEntity)
    {
        vos_printf("ModemId�Ƿ�������ModemId=%d\n",usModemId);
        return;
    }

    vos_printf("��ʾModem%d�������������������Ϊ%d  \r\n",usModemId,pstCdsEntity->ulUlSoftFilterNum);

    for (ulCnt = 0; ulCnt < pstCdsEntity->ulUlSoftFilterNum; ulCnt ++)
    {
        pstSoftFilter = &(pstCdsEntity->astUlSoftFilter[ulCnt]);
        vos_printf("�����������%d��ϸ��Ϣ\r\n",ulCnt);
        vos_printf("�����������ID            : %d\r\n",pstSoftFilter->ulPacketFilterId);
        vos_printf("���������������          : %d\r\n",pstSoftFilter->enDirection);
        vos_printf("���������������          : %d\r\n",pstSoftFilter->enFilterType);
        vos_printf("��������������ȼ�        : %d\r\n",pstSoftFilter->usPrecedence);
        vos_printf("�������������Ӧ�ĳ���ID  : %d\r\n",pstSoftFilter->ucRabId);

        if (CDS_SOFTFILTER_TYPE_IPV4 == pstSoftFilter->enFilterType)
        {
            CDS_ShowSoftFilterIPv4Info(pstSoftFilter);
        }
        else
        {
            CDS_ShowSoftFilterIPv6Info(pstSoftFilter);
        }

        CDS_ShowSoftFilterTransportLayerInfo(pstSoftFilter);

        vos_printf("\r\n");

    }


    vos_printf("\r\n");
    return;
}



VOS_VOID CDS_ShowUlSoftFilterStats(VOS_VOID)
{

    vos_printf("������������ݰ�����                         %d\n",g_stCdsStats.ulULSoftFilterPktNum);
    vos_printf("���������IPV4���ݰ�����                     %d\n",g_stCdsStats.ulULSoftFilterIPv4Num);
    vos_printf("���������IPV6���ݰ�����                     %d\n",g_stCdsStats.ulULSoftFilterIPv6Num);
    vos_printf("���������ʧ�ܸ���                           %d\n",g_stCdsStats.ulULSoftFilterFailNum);
    vos_printf("���������IP��Ƭ����                         %d\n",g_stCdsStats.ulULSoftFilterIPSegNum);
    vos_printf("���������IP��Ƭ����ɹ�����                 %d\n",g_stCdsStats.ulULSoftFilterIPSegSucc);
    vos_printf("���������IP��Ƭ����ʧ�ܸ���                 %d\n",g_stCdsStats.ulULSoftFilterIPSegFail);

    vos_printf("\r\n");
    return;
}



VOS_VOID CDS_ShowIMSStats(VOS_VOID)
{
    vos_printf("IMS�����հ��������ô���                      %d\n",g_stCdsStats.ulImsULRxFunCallNum);
    vos_printf("IMS�����հ�������ηǷ�����                  %d\n",g_stCdsStats.ulImsULRxFunParaErr);

    vos_printf("IMS���н���IMS NIC���ݰ�����                 %d\n",g_stCdsStats.ulImsULRxNicPktNum);
    vos_printf("IMS���������ڴ�ʧ�ܸ���                      %d\n",g_stCdsStats.ulImsULAllocMemFail);
    vos_printf("IMS�������ʧ�ܸ���                          %d\n",g_stCdsStats.ulImsULEnQueFail);
    vos_printf("IMS������ӳɹ�����                          %d\n",g_stCdsStats.ulImsULEnQueSucc);
    vos_printf("IMS���д���IMS_PROC�¼�����                  %d\n",g_stCdsStats.ulImsULTrigImsEvent);
    vos_printf("IMS���д���IMS_PROC�¼�����                  %d\n",g_stCdsStats.ulImsULProcImsEvent);
    vos_printf("IMS���������ʧ�ܴ���                        %d\n",g_stCdsStats.ulImsULSoftFilterFail);
    vos_printf("IMS�������ݰ����͵��տڸ���                  %d\n",g_stCdsStats.ulImsULSendToRan);

    vos_printf("IMS���н���IMS���ݰ�����                     %d\n",g_stCdsStats.ulImsDLRxImsPkt);
    vos_printf("IMS���н��յ�NDCLIENT���ݰ�����              %d\n",g_stCdsStats.ulImsDLNdPktNum);
    vos_printf("IMS���������ڴ�ʧ�ܸ���                      %d\n",g_stCdsStats.ulImsDLAllocMemFail);
    vos_printf("IMS���л�ȡĿ�Ķ˿ں�ʧ�ܸ���                %d\n",g_stCdsStats.ulImsDLGetDestPortFail);
    vos_printf("IMS���з��͵�IMS NIC���ݰ�ʧ�ܸ���           %d\n",g_stCdsStats.ulImsDLSendToNicFail);
    vos_printf("IMS���з��͵�IMS NIC���ݰ��ɹ�����           %d\n",g_stCdsStats.ulImsDLSendToNicSucc);

    vos_printf("IMS���н���Ut���ݰ�����                     %d\n",g_stCdsStats.ulImsDLRxUtPkt);
    vos_printf("IMS����Ut���ݰ�����ɹ�����                 %d\n",g_stCdsStats.ulImsDLUtPktProcSuccNum);
    vos_printf("IMS����Ut���ݰ�����ʧ�ܸ���                 %d\n",g_stCdsStats.ulImsDLUtPktProcErrNum);

    vos_printf("\r\n");
    return;
}


VOS_VOID CDS_ShowIpV4FragmentBuffInfo(VOS_VOID)
{

    VOS_UINT32                         ulCnt;
    CDS_IPV4_FRAGMENT_BUFF_STRU       *pstIpv4FragBuff;
    CDS_IPV4_FRAGMENT_INFO_STRU       *pstIpv4FragInfo;

    pstIpv4FragBuff = CDS_GET_IPV4_FRAGMENT_BUFF();
    for (ulCnt = 0; ulCnt < CDS_IPV4_FRAGMENT_BUFF_SIZE; ulCnt ++)
    {
        pstIpv4FragInfo = &(pstIpv4FragBuff->astSegBuff[ulCnt]);
        vos_printf("\n IPV4 Fragment Buffer Index : %d\n",ulCnt);
        vos_printf("EPS Bearer  ID:                 %d\n", pstIpv4FragInfo->ucEpsbID);
        vos_printf("Source IP Addr:                 ");
        CDS_PrintIpv4Addr(&(pstIpv4FragInfo->unSrcAddr));
        vos_printf("Dest   IP Addr:                 ");
        CDS_PrintIpv4Addr(&(pstIpv4FragInfo->unDstAddr));
        vos_printf("IP Fragment ID:                 %d\n", pstIpv4FragInfo->usIpIdentity);
    }

    return;
}


VOS_VOID CDS_ShowIpV6FragmentBuffInfo(VOS_VOID)
{

    VOS_UINT32                         ulCnt;
    CDS_IPV6_FRAGMENT_BUFF_STRU       *pstIpv6FragBuff;
    CDS_IPV6_FRAGMENT_INFO_STRU       *pstIpv6FragInfo;

    pstIpv6FragBuff = CDS_GET_IPV6_FRAGMENT_BUFF();
    for (ulCnt = 0; ulCnt < CDS_IPV6_FRAGMENT_BUFF_SIZE; ulCnt ++)
    {
        pstIpv6FragInfo = &(pstIpv6FragBuff->astSegBuff[ulCnt]);
        vos_printf("\n IPV6 Fragment Buffer Index : %d\n",ulCnt);
        vos_printf("EPS Bearer  ID:    %d\n", pstIpv6FragInfo->ucEpsbID);
        vos_printf("IP Fragment ID:    %d\n", pstIpv6FragInfo->ulIdentity);
        vos_printf("Src IPV6 Addr :    ");
        CDS_PrintIpv6Addr(&(pstIpv6FragInfo->unSrcAddr));
        vos_printf("Dst IPV6 Addr :    ");
        CDS_PrintIpv6Addr(&(pstIpv6FragInfo->unDstAddr));
    }

    return;
}



VOS_VOID CDS_ShowIpFragStats(VOS_VOID)
{

    vos_printf("����IPV4 ��Ƭ�ĸ���               %d\n",g_stCdsIpFragBuff.stStats.ulRxIpv4Frag);
    vos_printf("����IPV4 IPF���˳ɹ��ĸ���        %d\n",g_stCdsIpFragBuff.stStats.ulRxIpv4IpfSucc);
    vos_printf("����IPV4 IPF����ʧ�ܵĸ���        %d\n",g_stCdsIpFragBuff.stStats.ulRxIpv4IpfFail);
    vos_printf("����IPV6 ��Ƭ�ĸ���               %d\n",g_stCdsIpFragBuff.stStats.ulRxIpv6Frag);
    vos_printf("����IPV6 IPF���˳ɹ��ĸ���        %d\n",g_stCdsIpFragBuff.stStats.ulRxIpv6IpfSucc);
    vos_printf("����IPV6 IPF����ʧ�ܵĸ���        %d\n",g_stCdsIpFragBuff.stStats.ulRxIpv6IpfFail);
    vos_printf("���ձ��ش���ɹ��ĸ���            %d\n",g_stCdsIpFragBuff.stStats.ulLocalProcSucc);
    vos_printf("���ձ��ش���ʧ�ܵĸ���            %d\n",g_stCdsIpFragBuff.stStats.ulLocalProcFail);

    return;
}


VOS_VOID CDS_IpFrag_Help(VOS_VOID)
{
    vos_printf("CDS_ShowIpV4FragmentBuffInfo             ��ʾIPV4��Ƭ������Ϣ\n");
    vos_printf("CDS_ShowIpV6FragmentBuffInfo             ��ʾIPV6��Ƭ������Ϣ\n");
    vos_printf("CDS_ShowIpFragStats                      ��ʾIP��Ƭͳ����Ϣ\n");
    vos_printf("CDS_ClearIpFragBuff                      ���IP��Ƭ����");

    return;
}


VOS_VOID CDS_Help(VOS_VOID)
{

    vos_printf("********************CDS�����Ϣ************************\n");
    vos_printf("CDS_ShowCdsEntityInfo(ModemId)          ��ʾ��ӦModem��CDSʵ��״̬\n");
    vos_printf("CDS_ShowEventStats                      ��ʾCDS�¼�����ͳ����Ϣ\n");
    vos_printf("CDS_ShowUlRdProcStats                   ��ʾCDS����RD����ͳ����Ϣ\n");
    vos_printf("CDS_ShowUlAdqProcStats                  ��ʾCDS����ADQͳ����Ϣ\n");
    vos_printf("CDS_ShowUlIpPktProcStats                ��ʾCDS����IP������ͳ����Ϣ\n");
    vos_printf("CDS_ShowDlSduProcStats                  ��ʾCDS����SDU����ͳ����Ϣ\n");
    vos_printf("CDS_ShowDlIpfProcStats                  ��ʾCDS����IPF����ͳ����Ϣ\n");

    vos_printf("CDS_ShowModemGUProcStats(ModemId)       ��ʾGU��������ͳ����Ϣ\n");
    vos_printf("CDS_ShowLBProcStats                     ��ʾ����ģʽ����ͳ����Ϣ\n");
    vos_printf("CDS_IpFrag_Help                         ��ʾIP��Ƭ�����Ϣ\n");
    vos_printf("QosFc_ShowStats                         ��ʾ������������ͳ����Ϣ\n");

    vos_printf("CDS_ShowUlSoftFilter(ModemId)           ��ʾ���й���������Ϣ\n");
    vos_printf("CDS_ShowUlSoftFilterStats               ��ʾ��������˴���ͳ����Ϣ\n");
    vos_printf("CDS_ShowIMSStats                        ��ʾIMS����ͳ����Ϣ\n");

    return;
}
/*****************************************************************************
 �� �� ��  : LUP_CdsLomTraceRcvUlData
 ��������  : �û���ʱ��Trace�����ڼ�¼CDS������������ʱ���
 �������  :
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��5��23��
    ��    ��   : hujianbo 42180
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID Cds_LomTraceRcvUlData(VOS_VOID)
{
    if (1 == g_ulCdsOmSwitch)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS, "User plane latency trace: CDS rcv ul data\n");
    }

    return;
}

/*****************************************************************************
 �� �� ��  : Cds_LomTraceSendDlData
 ��������  : �û���ʱ��Trace�����ڼ�¼CDS������������ʱ���
 �������  :
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��5��23��
    ��    ��   : hujianbo 42180
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID Cds_LomTraceSendDlData(VOS_VOID)
{
    if (1 == g_ulCdsOmSwitch)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS, "User plane latency trace: CDS send dl data\n");
    }

    return;
}





#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif



