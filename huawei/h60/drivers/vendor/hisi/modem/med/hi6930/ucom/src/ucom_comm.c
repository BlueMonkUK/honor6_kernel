/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : ucom_comm.c
  �� �� ��   : ����
  ��    ��   : �³�Ⱥ
  ��������   : 2011��5��12��
  ����޸�   :
  ��������   : �ṩ������������
  �����б�   :
              UCOM_COMM_CopyMem
              UCOM_COMM_IdleHook
              UCOM_COMM_NormalFidInit
              UCOM_COMM_RegBitRd
              UCOM_COMM_RegBitWr
              UCOM_COMM_RtFidInit
              UCOM_COMM_SetMem
  �޸���ʷ   :
  1.��    ��   : 2011��5��12��
    ��    ��   : �³�Ⱥ
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "ucom_comm.h"
#include "ucom_mem_dyn.h"
#include "om.h"
#include "med_drv_dma.h"
#include "med_drv_ipc.h"
#include "med_drv_mb_hifi.h"
#include "med_drv_timer_hifi.h"
#include "med_drv_uart.h"
#include "med_drv_sio.h"
#include "med_drv_watchdog.h"
#include "ucom_low_power.h"
#include "voice_api.h"
#include "ucom_nv.h"
#include "audio_comm.h"
#include "om_cpuview.h"
#include "ucom_share.h"

#ifdef VOS_VENUS_TEST_STUB
#include "venus_stub.h"
#endif

#ifndef __UCOM_COMM_C__
#define __UCOM_COMM_C__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID                  OM_FILE_ID_UCOM_COMM_C

/*****************************************************************************
  2 �궨��
*****************************************************************************/

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  3 ��������
*****************************************************************************/


VOS_UINT32 UCOM_COMM_LowFidInit(enum VOS_INIT_PHASE_DEFINE enInitPhrase)
{
    VOS_UINT32  uwRet = VOS_OK;
    switch (enInitPhrase)
    {
        case VOS_IP_LOAD_CONFIG:
        {
            /* ����VOS_RegisterPIDInfoע��DSP_PID_HIFI_OM����Ϣ��ں��� */
            uwRet = VOS_RegisterPIDInfo(DSP_PID_HIFI_OM, OM_COMM_PidInit, OM_COMM_PidProc);
            if( VOS_OK != uwRet )
            {
                return uwRet;
            }

             /* ����VOS_RegisterPIDInfoע��DSP_FID_LOW�������ȼ�Ϊ�� */
            uwRet = VOS_RegisterMsgTaskPrio(DSP_FID_LOW, VOS_PRIORITY_P4);
            if( VOS_OK != uwRet )
            {
                return uwRet;
            }

           break;
        }
        case VOS_IP_FARMALLOC:
        case VOS_IP_INITIAL:
        case VOS_IP_ENROLLMENT:
        case VOS_IP_LOAD_DATA:
        case VOS_IP_FETCH_DATA:
        case VOS_IP_STARTUP:
        case VOS_IP_RIVAL:
        case VOS_IP_KICKOFF:
        case VOS_IP_STANDBY:
        case VOS_IP_BROADCAST_STATE:
        case VOS_IP_RESTART:
        {
            break;
        }
        default:
        {
            break;
        }
    }

    return uwRet;

}


/*****************************************************************************
 �� �� ��  : UCOM_NormalFidInit
 ��������  : ��ͨ���ȼ�FID��ʼ�����������л����DSP_PID_VOICE/DSP_PID_OM��PID��ʼ������
 �������  : enum VOS_INIT_PHASE_DEFINE enInitPhrase
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��4��28��
    ��    ��   : �³�Ⱥ
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 UCOM_COMM_NormalFidInit(enum VOS_INIT_PHASE_DEFINE enInitPhrase)
{
    VOS_UINT32  uwRet = VOS_OK;
    switch (enInitPhrase)
    {
        case VOS_IP_LOAD_CONFIG:
        {
            /* ����VOS_RegisterPIDInfoע��DSP_PID_VOICE����Ϣ��ں��� */
            uwRet = VOS_RegisterPIDInfo(DSP_PID_VOICE, VOICE_ApiPidInit, VOICE_ApiPidProc);
            if( VOS_OK != uwRet )
            {
                return uwRet;
            }

            /* ����VOS_RegisterPIDInfoע��DSP_PID_AUDIO����Ϣ��ں��� */
            /*uwRet = VOS_RegisterPIDInfo(DSP_PID_AUDIO, AUDIO_COMM_PidInit, AUDIO_COMM_PidProc);
            if( VOS_OK != uwRet )
            {
                return uwRet;
            }*/

            /* ����VOS_RegisterPIDInfoע��DSP_FID_NORMAL�������ȼ�Ϊ�� */
            uwRet = VOS_RegisterMsgTaskPrio(DSP_FID_NORMAL, VOS_PRIORITY_P5);
            if( VOS_OK != uwRet )
            {
                return uwRet;
            }

            break;
        }
        case VOS_IP_FARMALLOC:
        case VOS_IP_INITIAL:
        case VOS_IP_ENROLLMENT:
        case VOS_IP_LOAD_DATA:
        case VOS_IP_FETCH_DATA:
        case VOS_IP_STARTUP:
        case VOS_IP_RIVAL:
        case VOS_IP_KICKOFF:
        case VOS_IP_STANDBY:
        case VOS_IP_BROADCAST_STATE:
        case VOS_IP_RESTART:
        {
            break;
        }
        default:
        {
            break;
        }
    }

    return uwRet;

}

/*****************************************************************************
 �� �� ��  : UCOM_COMM_RtFidInit
 ��������  : �����ȼ�FID��ʼ�����������л����DSP_PID_VOICE_RT��PID��ʼ������
 �������  : enum VOS_INIT_PHASE_DEFINE enInitPhrase
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��4��28��
    ��    ��   : �³�Ⱥ
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 UCOM_COMM_RtFidInit(enum VOS_INIT_PHASE_DEFINE enInitPhrase)
{
    VOS_UINT32  uwRet = VOS_OK;
    switch (enInitPhrase)
    {
        case VOS_IP_LOAD_CONFIG:
        {
            /* 0.����VOS_RegisterPIDInfoע��DSP_PID_VOICE_RT����Ϣ��ں��� */
            uwRet = VOS_RegisterPIDInfo(DSP_PID_VOICE_RT, VOICE_ApiRtPidInit, VOICE_ApiRtPidProc);
            if( VOS_OK != uwRet )
            {
                return uwRet;
            }

            /* 0.����VOS_RegisterPIDInfoע��DSP_PID_AUDIO_RT����Ϣ��ں��� */
            /*uwRet = VOS_RegisterPIDInfo(DSP_PID_AUDIO_RT, AUDIO_COMM_RtPidInit, AUDIO_COMM_RtPidProc);
            if( VOS_OK != uwRet )
            {
                return uwRet;
            }*/

            /* 0.����VOS_RegisterPIDInfoע��DSP_FID_RT�������ȼ�Ϊ�� */
            uwRet = VOS_RegisterMsgTaskPrio(DSP_FID_RT, VOS_PRIORITY_P6 );
            if( VOS_OK != uwRet )
            {
                return uwRet;
            }

            /* 0.����IDLE������ע�� */
            VOS_RegIdleHook(UCOM_COMM_IdleHook);

            /* 1.���ȳ�ʼ��OMģ�� */
            OM_COMM_Init();

            /* �ڴ����ģ���ʼ�� */
            UCOM_MEM_DYN_Init();

            /* HIFI�ڲ�����γ�ʼ�� */
            UCOM_SHARE_Init();

            /* 2.��������ʼ�� */
            DRV_IPC_Init();
            DRV_MAILBOX_Init();
            DRV_SOCP_Init();
            DRV_DMA_Init();
            DRV_SIO_Init();
            DRV_TIMER_Init();

            #ifdef _OM_UART_PRINT
            DRV_UART_Init();
            #endif
#if (VOS_HIFI == VOS_CPU_TYPE)
            UCOM_NV_Init();
#endif
            UCOM_LOW_PowerInit();

            /* 8.����ST����ʱ�ҽ��ж�2��7�����ж�7�з��Ͳ�����Ϣ */
            #ifdef VOS_VENUS_TEST_STUB
            xthal_set_intset(0x80000002);
            xthal_set_intclear(0x80000002);
            xthal_set_intenable(0x00000000);

            VOS_ConnectInterrupt(DRV_IPC_INT_NO_HIFI, VOS_IntHandlers_02);
            VOS_ConnectInterrupt(OS_INTR_CONNECT_07, VOS_IntHandlers_07);

            VOS_EnableInterrupt(DRV_IPC_INT_NO_HIFI);
            VOS_EnableInterrupt(OS_INTR_CONNECT_07);
            #endif

            /* 9.�������Ź� */
            if(UCOM_GET_WATCHDOG_ENABLE())
            {
                //DRV_WATCHDOG_Start(UCOM_GET_WD_TIMEOUT_LEN());
            }

            break;
        }
        case VOS_IP_FARMALLOC:
        case VOS_IP_INITIAL:
        case VOS_IP_ENROLLMENT:
        case VOS_IP_LOAD_DATA:
        case VOS_IP_FETCH_DATA:
        case VOS_IP_STARTUP:
        case VOS_IP_RIVAL:
        case VOS_IP_KICKOFF:
        case VOS_IP_STANDBY:
        case VOS_IP_BROADCAST_STATE:
        case VOS_IP_RESTART:
        {
            break;
        }
        default:
        {
            break;
        }
    }

    return uwRet;

}


VOS_VOID UCOM_COMM_CopyMem(
                VOS_VOID               *pvDes,
                VOS_VOID               *pvSrc,
                VOS_UINT32              uwCount)
{
    VOS_UCHAR  *pucDes = (VOS_UCHAR*)pvDes;
    VOS_UCHAR  *pucSrc = (VOS_UCHAR*)pvSrc;

    /* ��ͬ����Ҫ���� */
    if (pvSrc == pvDes)
    {
        return;
    }
    /* DES��SRC���, ��DES������SRCͷ�� */
    else if (pucSrc > pucDes)
    {
        for(; pucDes < ((VOS_UCHAR*)pvDes + uwCount); pucDes++)
        {
            *pucDes = *pucSrc;
            pucSrc++;
        }
    }
    /* DES��SRC�ұ�, ��DES������β�� */
    else
    {
        pucSrc = pucSrc + (uwCount - 1);
        for(pucDes = pucDes + (uwCount - 1); pucDes >= (VOS_UCHAR*)pvDes; pucDes--)
        {
            *pucDes = *pucSrc;
            pucSrc--;
        }
    }
}
VOS_VOID UCOM_COMM_SetMem(
                VOS_VOID               *pvDes,
                VOS_UCHAR               ucData,
                VOS_UINT32              uwCount)
{
    VOS_UINT32              i;
    VOS_UCHAR              *pucDes  = (VOS_UCHAR*)pvDes;

    /*��byte���и�ֵ*/
    for(i = 0; i < uwCount; i++)
    {
       *pucDes++  = ucData;
    }
}
VOS_VOID UCOM_COMM_GetChnBit16(
                VOS_VOID               *pvDes,
                VOS_UINT32              uwSize,
                VOS_UINT32              uwChnID,
                VOS_VOID               *pvSrc,
                VOS_UINT32              uwChnNum)
{
    VOS_INT16 *pshwSrc, *pshwDes;
    VOS_UINT32  i, uwLen;

    pshwSrc     = (VOS_INT16*)pvSrc;
    pshwDes     = (VOS_INT16*)pvDes;

    uwLen = (uwSize / sizeof(VOS_INT16));

    for(i=0; i< uwLen; i++)
    {
        pshwDes[i] = pshwSrc[(uwChnNum * i) + uwChnID];
    }

}


VOS_VOID UCOM_COMM_SetChnBit16(
                VOS_VOID               *pvDes,
                VOS_UINT32              uwChnNum,
                VOS_VOID               *pvSrc,
                VOS_UINT32              uwSize,
                VOS_UINT32              uwChnID
                )
{
    VOS_INT16 *pshwSrc, *pshwDes;
    VOS_UINT32  i, uwLen;

    pshwSrc     = (VOS_INT16*)pvSrc;
    pshwDes     = (VOS_INT16*)pvDes;

    uwLen = (uwSize / sizeof(VOS_INT16));

    for(i=0; i< uwLen; i++)
    {
        pshwDes[(uwChnNum * i) + uwChnID] = pshwSrc[i];
    }

}


VOS_UINT32 UCOM_COMM_RegBitRd(
                VOS_UINT32              uwAddr,
                VOS_UINT16              uhwStartBit,
                VOS_UINT16              uhwEndBit)
{
    VOS_UINT32                          uwContent;
    VOS_UINT32                          uwTmpMask;
    VOS_UINT32                          uwTmpBit;

    /*�����쳣ʱ���ȡ����32bit����*/
    if ((uhwEndBit < uhwStartBit)
        || (uhwStartBit > UCOM_BIT31)
        || (uhwEndBit > UCOM_BIT31))
    {
        uhwStartBit = UCOM_BIT0;
        uhwEndBit   = UCOM_BIT31;
    }

    /*������ʼ�ͽ���������������*/
    uwTmpBit        = UCOM_BIT31 - uhwEndBit;
    uwTmpMask       = UCOM_COMM_UINT32_MAX << uwTmpBit;
    uwTmpMask       = uwTmpMask >> ( uhwStartBit + uwTmpBit );
    uwTmpMask       = uwTmpMask << uhwStartBit;

    uwContent       = UCOM_RegRd(uwAddr);
    uwContent      &= uwTmpMask;
    uwContent       = uwContent >> uhwStartBit;

    return uwContent;

}

VOS_VOID UCOM_COMM_RegBitWr(
                VOS_UINT32              uwAddr,
                VOS_UINT16              uhwStartBit,
                VOS_UINT16              uhwEndBit,
                VOS_UINT32              uwContent)
{
    VOS_UINT32                          uwSetValue;
    VOS_UINT32                          uwRegContent;
    VOS_UINT32                          uwTmpMask;
    VOS_UINT32                          uwTmpBit;

    /*���������쳣ʱ�����쳣����*/
    if ((uhwEndBit < uhwStartBit)
        || (uhwStartBit > UCOM_BIT31)
        || (uhwEndBit > UCOM_BIT31))
    {
        return;
    }

    /*���ݴ�д�����ݺ���ʼ������Ϣ������д�Ĵ���������*/
    uwSetValue      = uwContent;
    uwSetValue      = uwSetValue << uhwStartBit;

    /*������ʼ�ͽ���������������*/
    uwTmpBit        = UCOM_BIT31 - uhwEndBit;
    uwTmpMask       = UCOM_COMM_UINT32_MAX << uwTmpBit;
    uwTmpMask       = uwTmpMask >> ( uhwStartBit + uwTmpBit);
    uwTmpMask       = uwTmpMask << uhwStartBit;

    /*�ȶ�ȡWBBP�Ĵ�����ַ�е����ݣ���д��ָ��bit����*/
    uwRegContent    = UCOM_RegRd(uwAddr);
    uwRegContent   &= (~uwTmpMask);
    uwSetValue     &= uwTmpMask;
    UCOM_RegWr(uwAddr, (uwRegContent | uwSetValue));

    return;
}

/*****************************************************************************
 �� �� ��  : UCOM_COMM_SendDSPMsg
 ��������  : ������Ϣ
 �������  : uwSenderPid   - ������Ϣ��PID
             uwReceiverPid - ������Ϣ��PID
             uwChannel     - ������Ϣ��ChannelID
             pvMsg         - ��Ϣ�׵�ַ
             uwLen         - ����(8bit)

 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��16��
    ��    ��   : л���� 58441
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID UCOM_COMM_SendDspMsg(
            VOS_UINT32              uwSenderPid,
            VOS_UINT32              uwReceiverPid,
            VOS_VOID               *pvMsg,
            VOS_UINT32              uwLen,
            VOS_VOID               *pvTail,
            VOS_UINT32              uwTailLen)
{
    VOS_UINT32                          uwRet;
    MsgBlock                           *pstSndMsg = VOS_NULL;
    VOS_UINT32                          uwMsgLen  = (uwLen + uwTailLen);

    /* ������Ϣ�ڴ� */
    pstSndMsg = VOS_AllocMsg(uwSenderPid, uwMsgLen);

    if (VOS_NULL == pstSndMsg)
    {
        OM_LogError(UCOM_COMM_SendMsg_NoMem);

        /* ��λ */
        OM_LogFatalAndReset(UCOM_COMM_SendMsg_NoMem);

        return;
    }

    /* �����Ϣ���� */
    pstSndMsg->uwReceiverPid        = uwReceiverPid;
    pstSndMsg->uwLength             = uwMsgLen;

    /* ������������Ϣ���� */
    UCOM_MemCpy(pstSndMsg->aucValue , pvMsg, uwLen);

    /* ����cmdId */
    UCOM_MemCpy((pstSndMsg->aucValue + uwLen), pvTail, uwTailLen);

    /* ������Ϣ */
    uwRet = VOS_SendMsg(uwSenderPid, pstSndMsg, DRV_MAILBOX_CHANNEL_DSP_CH);

    /* ��������Ϣʧ��,��¼�쳣,���ش��� */
    if (VOS_OK != uwRet)
    {
        OM_LogError2(UCOM_COMM_SendMsg_Err, uwRet, pstSndMsg);

        /* ��λ */
        OM_LogFatalAndReset(UCOM_COMM_SendMsg_Err);

        return;
    }

    return;
}

/*****************************************************************************
 �� �� ��  : UCOM_COMM_SendMsg
 ��������  : ������Ϣ
 �������  : uwSenderPid   - ������Ϣ��PID
             uwReceiverPid - ������Ϣ��PID
             uwChannel     - ������Ϣ��ChannelID
             pvMsg         - ��Ϣ�׵�ַ
             uwLen         - ����(8bit)

 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��16��
    ��    ��   : л���� 58441
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID UCOM_COMM_SendMsg(
                VOS_UINT32              uwSenderPid,
                VOS_UINT32              uwReceiverPid,
                VOS_UINT32              uwChannel,
                VOS_VOID               *pvMsg,
                VOS_UINT32              uwLen)
{
    VOS_UINT32                          uwRet;
    MsgBlock                           *pstSndMsg = VOS_NULL;

    /* ������Ϣ�ڴ� */
    pstSndMsg = VOS_AllocMsg(uwSenderPid, uwLen);
    if (VOS_NULL == pstSndMsg)
    {
        OM_LogError(UCOM_COMM_SendMsg_NoMem);

        /* ��λ */
        OM_LogFatalAndReset(UCOM_COMM_SendMsg_NoMem);

        return;
    }

    /* �����Ϣ���� */
    pstSndMsg->uwReceiverPid        = uwReceiverPid;
    pstSndMsg->uwLength             = uwLen;

    /* ������������Ϣ���� */
    UCOM_MemCpy(pstSndMsg->aucValue, pvMsg, uwLen);

    /* ������Ϣ */
    uwRet = VOS_SendMsg(uwSenderPid, pstSndMsg, uwChannel);

    /* ��������Ϣʧ��,��¼�쳣,���ش��� */
    if (VOS_OK != uwRet)
    {
        OM_LogError2(UCOM_COMM_SendMsg_Err, uwRet, pstSndMsg);

        /* ��λ */
        OM_LogFatalAndReset(UCOM_COMM_SendMsg_Err);

        return;
    }

    return;
}

/*****************************************************************************
 �� �� ��  : UCOM_COMM_IdleHook
 ��������  : IDLE��������
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��5��31��
    ��    ��   : �³�Ⱥ
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID UCOM_COMM_IdleHook(VOS_UINT32 ulVosCtxSw)
{
    VOS_CPU_SR                          srSave;

    /* ���watchdogʹ�ܣ��˴�ι�� */
    if(UCOM_GET_WATCHDOG_ENABLE())
    {
        /* ���ж� */
        srSave = VOS_SplIMP();

        /* ����DRV_WATCHDOG_Feed()ι�� */
        DRV_WATCHDOG_Feed();

        /* �ͷ��ж� */
        VOS_Splx(srSave);
    }

    //AUDIO_COMM_CheckContext();

    OM_CPUVIEW_RecordStackUsage();

    UCOM_LOW_PowerTask(ulVosCtxSw);

}


VOS_VOID UCOM_COMM_Sleep(VOS_UINT32 ulSleepTimeLen)
{
    VOS_UINT32 uwTimer;

    /* ��ʱ�� */
    DRV_TIMER_Start(&uwTimer,
                    ulSleepTimeLen,
                    DRV_TIMER_MODE_ONESHOT,
                    VOS_NULL_PTR,
                    0);
    /* ����WFI */
    UCOM_SET_WFI(0);
}


VOS_VOID UCOM_COMM_TimeDelayFor( VOS_UINT32 uwTimeLen )
{
    volatile VOS_UINT32                 uwTmpI;
    VOS_UINT32                          uwDelayCycleNum;

    /*����ʱ��Ƶ�ʼ�������ӳٵ�ʱ������*/
    uwDelayCycleNum     = (VOS_UINT32)( uwTimeLen * ( VOS_UINT32 )( ( ( UCOM_HIFI_FREQ/1000 )/UCOM_COMM_LOOP_CLOCK_PERIOD )/12 ) );

    for ( uwTmpI = 0; uwTmpI < uwDelayCycleNum; uwTmpI++ )
    {
    }

    return;
}
VOS_UINT32 UCOM_GetUncachedAddr(VOS_UINT32 uwAddr)
{
    return ((VOS_UINT32)(uwAddr) - UCOM_MEMMAP_OFFSET);
}


VOS_UINT32 UCOM_GetUnmappedAddr(VOS_UINT32 uwAddr)
{
    return ((VOS_UINT32)(uwAddr) - UCOM_CACHED2PHY_OFFSET);
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of ucom_comm.c */
