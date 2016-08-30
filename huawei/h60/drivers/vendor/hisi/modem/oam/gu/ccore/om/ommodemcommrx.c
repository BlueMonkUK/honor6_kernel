/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : ommodemcommrx.c
  �� �� ��   : ����
  ��    ��   : ���� 46160
  ��������   : 2011��3��11��
  ����޸�   :
  ��������   :
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��3��11��
    ��    ��   : ���� 46160
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "errorlog.h"
#include "omrl.h"
#include "omprivate.h"
#include "DrvInterface.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_OM_COMMRX_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/*����CCPU��ICCͨ�������ݴ���ص������б�*/
OM_PORT_READ_DATA_CB                    g_astOMComRxCBTable[OM_ICC_CHANNEL_BUTT] = {0};

/*����CCPU��ICCͨ���Ŀ��������б�*/
OM_ICC_CHANNEL_CTRL_STRU                g_astOMCCPUIccCtrlTable[OM_ICC_CHANNEL_BUTT];

/* debug */
VOS_UINT32                              g_ulOmCcpuDbg = 0;

/*****************************************************************************
  3 �ⲿ��������
*****************************************************************************/
extern UDI_HANDLE                       g_OSAIccUDIHandle;

extern VOS_UINT V_ICC_OSAMsg_CB(VOS_UINT ulChannelID,VOS_INT lLen);

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

VOS_UINT32 OM_ComRx_ICCDataRec(OM_ICC_CHANNEL_ENUM_UINT32 enChannelId, VOS_INT32 lLen)
{
    VOS_UINT8 *pucData;
    VOS_INT32 lResult;

    /*�����ڴ����ڱ���ICC���ݵ�����*/
    pucData = (VOS_UINT8 *)VOS_MemAlloc(WUEPS_PID_OM, DYNAMIC_MEM_PT, (VOS_UINT32)lLen);

    if (VOS_NULL_PTR == pucData)
    {
        LogPrint1("OM_ComRx_ICCDataRec: VOS_MemAlloc is Failed, Data Len is %d.\n", lLen);

        return VOS_ERR;
    }

    /*��ȡICC���ݵ�����*/
    lResult = DRV_UDI_READ(g_astOMCCPUIccCtrlTable[enChannelId].UDIHdle, pucData, (VOS_UINT32)lLen);
    if (lLen != lResult)
    {
        VOS_MemFree(WUEPS_PID_OM, pucData);
        LogPrint("OM_ComRx_ICCDataRec: DRV_UDI_READ is Failed.\n");

        return VOS_ERR;
    }

    /*ICC���ݴ��������ش���*/
    if (VOS_OK != g_astOMComRxCBTable[enChannelId](pucData, (VOS_UINT16)lLen))
    {
        LogPrint("OM_ComRx_ICCDataRec: The CB function is Failed.\n");
    }

    VOS_MemFree(WUEPS_PID_OM, pucData);

    return VOS_OK;
}

VOS_UINT OM_ComRx_ICC_OM_CB(VOS_UINT ulChannelID,VOS_INT lLen)
{
    VOS_UINT32 ulResult;
    VOS_UINT8 *pucData;

    if (lLen <= 0)
    {
        LogPrint("OM_ComRx_ICC_OM_CB: The Data Len is 0.\n");

        g_stPcToUeErrRecord.stICCRecFailInfo.ulICCOmParaErrNum++;

        return VOS_ERR;
    }

    if (VOS_NULL_PTR == g_astOMComRxCBTable[OM_OM_ICC_CHANNEL])  /*��ǰ�ص�����Ϊ��*/
    {
        LogPrint("OM_ComRx_ICC_OM_CB The CB Function is Empty.\n");

        g_stPcToUeSucRecord.stICCRecSUCInfo.ulICCOmRecLen += lLen;
        g_stPcToUeSucRecord.stICCRecSUCInfo.ulICCOmRecMsgNum++;
        g_stPcToUeSucRecord.stICCRecSUCInfo.ulICCOmRecSlice = OM_GetSlice();

        pucData = (VOS_UINT8 *)VOS_MemAlloc(WUEPS_PID_OM, DYNAMIC_MEM_PT, (VOS_UINT32)lLen);

        if(VOS_NULL_PTR != pucData)
        {
            DRV_UDI_READ(g_astOMCCPUIccCtrlTable[OM_OM_ICC_CHANNEL].UDIHdle, pucData, (VOS_UINT32)lLen);
            VOS_MemFree(WUEPS_PID_OM, pucData);
        }

        return VOS_OK;
    }

    ulResult = OM_ComRx_ICCDataRec(OM_OM_ICC_CHANNEL, lLen);

    if (VOS_ERR == ulResult)
    {
        LogPrint("OM_ComRx_ICC_OM_CB The Send Data is Failed.\n");

        g_stPcToUeErrRecord.stICCRecFailInfo.ulICCOmRecErrLen += lLen;
        g_stPcToUeErrRecord.stICCRecFailInfo.ulICCOmRecErrMsgNum++;
        g_stPcToUeErrRecord.stICCRecFailInfo.ulICCOmRecErrSlice = OM_GetSlice();
    }
    else
    {
        g_stPcToUeSucRecord.stICCRecSUCInfo.ulICCOmRecLen += lLen;
        g_stPcToUeSucRecord.stICCRecSUCInfo.ulICCOmRecMsgNum++;
        g_stPcToUeSucRecord.stICCRecSUCInfo.ulICCOmRecSlice = OM_GetSlice();
    }

    return ulResult;
}

VOS_UINT OM_ComRx_ICCError_CB(VOS_UINT ulChanID, VOS_UINT ulEvent, VOS_VOID* pParam)
{
    OAM_MNTN_ICC_ERROR_EVENT_STRU stIccErrorInfo;

    stIccErrorInfo.ulEvent = ulEvent;

    if (g_astOMCCPUIccCtrlTable[OM_OM_ICC_CHANNEL].UDIHdle == ulChanID)
    {
        stIccErrorInfo.ulChannelID = OM_OM_ICC_CHANNEL;
    }
    else if(g_astOMCCPUIccCtrlTable[OM_OSA_MSG_ICC_CHANNEL].UDIHdle == ulChanID)
    {
        stIccErrorInfo.ulChannelID = OM_OSA_MSG_ICC_CHANNEL;
    }
    else
    {
        LogPrint("OM_ComRx_ICCError_CB: The Channel ID is not used by OAM\n");

        return VOS_ERR;
    }

    LogPrint2("OM_ComRx_ICCError_CB: The Error ICC Channel is %d, and Error Event is %d",
                    (VOS_INT32)stIccErrorInfo.ulChannelID, (VOS_INT32)ulEvent);

    /*ͨ��ID�ʹ���ԭ��д��ErrorLog*/
    MNTN_RecordErrorLog(MNTN_OAM_ICC_ERROR_EVENT, &stIccErrorInfo, sizeof(stIccErrorInfo));

    return VOS_OK;
}
VOS_VOID OM_ComRx_RegisterCB(OM_ICC_CHANNEL_ENUM_UINT32    enPortNum,
                                        OM_PORT_READ_DATA_CB          pfDataFun)
{
    if (enPortNum >= OM_ICC_CHANNEL_BUTT)    /* ����˿ںŲ��ں���Χ */
    {
        LogPrint("OM_ComRx_RegisterCB The enPortNum is Error.\n");

        return ;
    }

    g_astOMComRxCBTable[enPortNum]   = pfDataFun;

    return ;
}
VOS_VOID OM_ComRx_DeRegisterCB(OM_ICC_CHANNEL_ENUM_UINT32    enPortNum)
{
    if (enPortNum >= OM_ICC_CHANNEL_BUTT)    /*����˿ںŲ��ں���Χ*/
    {
        LogPrint("OM_ComRx_DeRegisterCB The enPortNum is Error.\n");

        return ;
    }

    g_astOMComRxCBTable[enPortNum] = VOS_NULL_PTR;

    return ;
}
VOS_UINT32 OM_ComRx_ICC_Init(VOS_VOID)
{
    VOS_INT32  i;

    VOS_MemSet(g_astOMComRxCBTable, 0 ,sizeof(g_astOMComRxCBTable));
    VOS_MemSet(g_astOMCCPUIccCtrlTable, 0, sizeof(g_astOMCCPUIccCtrlTable));

    for (i = 0; i < OM_ICC_CHANNEL_BUTT; i++)
    {
        g_astOMCCPUIccCtrlTable[i].stUDICtrl.pstICCAttr = (ICC_CHAN_ATTR_S*)VOS_MemAlloc(WUEPS_PID_OM,
                                                                                         STATIC_MEM_PT,
                                                                                         sizeof(ICC_CHAN_ATTR_S));
        if(VOS_NULL_PTR == g_astOMCCPUIccCtrlTable[i].stUDICtrl.pstICCAttr)
        {
            LogPrint1("OM_ComRx_ICC_Init: VOS_MemAlloc Failed, Index is %d.\n", i);

            return VOS_ERR;
        }
    }

    g_astOMCCPUIccCtrlTable[OM_OM_ICC_CHANNEL].stUDICtrl.enICCId                    = UDI_ICC_GUOM0_ID;
    g_astOMCCPUIccCtrlTable[OM_OM_ICC_CHANNEL].stUDICtrl.pstICCAttr->read_cb        = OM_ComRx_ICC_OM_CB;

    g_astOMCCPUIccCtrlTable[OM_OSA_MSG_ICC_CHANNEL].stUDICtrl.enICCId               = UDI_ICC_GUOM4_ID;
    g_astOMCCPUIccCtrlTable[OM_OSA_MSG_ICC_CHANNEL].stUDICtrl.pstICCAttr->read_cb   = V_ICC_OSAMsg_CB;

    for(i=0; i<OM_ICC_CHANNEL_BUTT; i++)
    {
        g_astOMCCPUIccCtrlTable[i].stUDICtrl.pstICCAttr->u32Priority    = OM_ICC_CHANNEL_PRIORITY;  /* ͳһʹ��������ȼ� */
        g_astOMCCPUIccCtrlTable[i].stUDICtrl.pstICCAttr->u32TimeOut     = OM_ICC_HANDSHAKE_TIME_MAX;

        if( i == OM_OSA_MSG_ICC_CHANNEL)
        {
            g_astOMCCPUIccCtrlTable[i].stUDICtrl.pstICCAttr->u32FIFOInSize  = OSA_ICC_BUFFER_SIZE;
            g_astOMCCPUIccCtrlTable[i].stUDICtrl.pstICCAttr->u32FIFOOutSize = OSA_ICC_BUFFER_SIZE;
        }
        else
        {
            g_astOMCCPUIccCtrlTable[i].stUDICtrl.pstICCAttr->u32FIFOInSize  = OM_ICC_BUFFER_SIZE;
            g_astOMCCPUIccCtrlTable[i].stUDICtrl.pstICCAttr->u32FIFOOutSize = OM_ICC_BUFFER_SIZE;
        }

        g_astOMCCPUIccCtrlTable[i].stUDICtrl.pstICCAttr->enChanMode     = ICC_CHAN_MODE_PACKET;
        g_astOMCCPUIccCtrlTable[i].stUDICtrl.pstICCAttr->event_cb       = OM_ComRx_ICCError_CB;

        g_astOMCCPUIccCtrlTable[i].UDIHdle = DRV_UDI_OPEN((UDI_OPEN_PARAM *)&(g_astOMCCPUIccCtrlTable[i].stUDICtrl));

        if (VOS_ERROR == g_astOMCCPUIccCtrlTable[i].UDIHdle)
        {
            /* ��ʧ��ʱ��¼��ǰICCͨ����Ϣ */
            DRV_SYSTEM_ERROR(OM_APP_ICC_INIT_ERROR, THIS_FILE_ID, __LINE__,
                            (VOS_CHAR*)&i, sizeof(VOS_INT32));

            return VOS_ERR;
        }
    }

    g_OSAIccUDIHandle = g_astOMCCPUIccCtrlTable[OM_OSA_MSG_ICC_CHANNEL].UDIHdle;

    return VOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif



