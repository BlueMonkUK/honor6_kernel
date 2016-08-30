

#include "mailbox.h"
#include "bsp_sram.h"
#include "bsp_dump.h"
#include "bsp_dump_drv.h"
#include "osl_bio.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define MBX_PRINT_TIMES     10

/* ����ģ�������Ϣ */
MAILBOX_MAINCTRL_STRU   g_stMbxCtrl;
MAILBOX_MNTN_STRU       g_stMbxMntn;

MAILBOX_CHANNEL_TABLE_STRU g_astMbxChnTbl[EN_MAILBOX_SERVICE_BUTT] = 
{
    {EN_MAILBOX_SERVICE_LTE_PS,      MBX_ADDRESSER_LTE_PHY_RL,     MAILBOX_PS_DOWN_ADDR,    MAILBOX_PS_UP_ADDR, NULL},
    {EN_MAILBOX_SERVICE_LTE_CT,      MBX_ADDRESSER_LTE_PHY_CT,     MAILBOX_PS_DOWN_ADDR,    MAILBOX_PS_UP_ADDR, NULL},
    {EN_MAILBOX_SERVICE_LTE_BT,      MBX_ADDRESSER_LTE_PHY_BT,     MAILBOX_PS_DOWN_ADDR,    MAILBOX_PS_UP_ADDR, NULL},
    {EN_MAILBOX_SERVICE_LTE_OM,      MBX_ADDRESSER_LTE_PHY_OM,     MAILBOX_OM_DOWN_ADDR,    0                 , NULL},
    {EN_MAILBOX_SERVICE_LTE_COEX,    MBX_ADDRESSER_LTE_PHY_COEX,   MAILBOX_PS_DOWN_ADDR,    MAILBOX_PS_UP_ADDR, NULL},
    {EN_MAILBOX_SERVICE_LTE_HS_DIAG, MBX_ADDRESSER_LTE_PHY_HS_DIAG,MAILBOX_OM_DOWN_ADDR,    0                 , NULL},

    {EN_MAILBOX_SERVICE_TDS_PS,      MBX_ADDRESSER_TDS_PHY_RL,     MAILBOX_PS_DOWN_ADDR,    MAILBOX_PS_UP_ADDR, NULL},
    {EN_MAILBOX_SERVICE_TDS_CT,      MBX_ADDRESSER_TDS_PHY_CT,     MAILBOX_PS_DOWN_ADDR,    MAILBOX_PS_UP_ADDR, NULL},
    {EN_MAILBOX_SERVICE_TDS_BT,      MBX_ADDRESSER_TDS_PHY_BT,     MAILBOX_PS_DOWN_ADDR,    MAILBOX_PS_UP_ADDR, NULL},
    {EN_MAILBOX_SERVICE_TDS_OM,      MBX_ADDRESSER_TDS_PHY_OM,     MAILBOX_OM_DOWN_ADDR,    0                 , NULL},
    {EN_MAILBOX_SERVICE_TDS_HS_DIAG, MBX_ADDRESSER_TDS_PHY_HS_DIAG,MAILBOX_OM_DOWN_ADDR,    0                 , NULL},
    
    {EN_MAILBOX_SERVICE_RTT_SYS_CTRL,MBX_ADDRESSER_RTT_SYS_CTRL,   MAILBOX_PS_DOWN_ADDR,    MAILBOX_PS_UP_ADDR, NULL},
};


u32 mbx_IsChnModMatch(u32 ulChn, u32 ulMod)
{
    if(ulChn >= EN_MAILBOX_SERVICE_BUTT)
    {
        return ERR_MAILBOX_PARAM_INCORRECT;
    }
    
    switch(ulChn)
    {
        case EN_MAILBOX_SERVICE_LTE_PS:
        case EN_MAILBOX_SERVICE_TDS_PS:
        {
            if((MBX_ADDRESSER_LTE_PHY_RL == ulMod)
             ||(MBX_ADDRESSER_TDS_PHY_RL == ulMod))
            {
                return BSP_OK;
            }
            break;
        }
        
        case EN_MAILBOX_SERVICE_LTE_CT:
        case EN_MAILBOX_SERVICE_LTE_BT:
        case EN_MAILBOX_SERVICE_TDS_CT:
        case EN_MAILBOX_SERVICE_TDS_BT:
        {
            if((MBX_ADDRESSER_LTE_PHY_CT == ulMod)
             ||(MBX_ADDRESSER_LTE_PHY_BT == ulMod)
             ||(MBX_ADDRESSER_TDS_PHY_CT == ulMod)
             ||(MBX_ADDRESSER_TDS_PHY_BT == ulMod))
            {
                return BSP_OK;
            }
            break;
        }
        
        default :
        {
            if(g_astMbxChnTbl[ulChn].ulModule == ulMod)
            {
                return BSP_OK;
            }
        }
    }
    return ERR_MAILBOX_PARAM_INCORRECT;
}

u32 mbx_GetChn(u32 ulMod)
{
    u32 i;
    
    for(i = 0; i < sizeof(g_astMbxChnTbl)/sizeof(MAILBOX_CHANNEL_TABLE_STRU); i++)
    {
        if(ulMod == g_astMbxChnTbl[i].ulModule)
        {
            return g_astMbxChnTbl[i].ulMbxChn;
        }
    }

    return (u32)EN_MAILBOX_SERVICE_BUTT;
}

/*****************************************************************************
 �� �� ��  : mbx_mntnaddr
 ��������  : �����ַ�����Ϣ
*****************************************************************************/
void mbx_mntnaddr()
{
    printf("***************************************************************\n");
    printf("��ַ��Ϣ\n");
    printf("PS���������ַ                : %p.\n", (void*)MAILBOX_PS_DOWN_ADDR);
    printf("PS���������ַ                : %p.\n", (void*)MAILBOX_PS_UP_ADDR);
    printf("OM���������ַ                : %p.\n", (void*)MAILBOX_OM_DOWN_ADDR);
    printf("DSP���ֵ�ַ                   : %p.\n", (void*)BSP_DSP_INIT_RESULT_ADDR);
    printf("ģʽ���õ�ַ                  : %p.\n", (void*)LPHY_MAILBOX_LPHY_WORKING_MODE_ADDR);
    printf("***************************************************************\n");
}


/*****************************************************************************
 �� �� ��  : mbx_mntnabnormal
 ��������  : �����쳣��Ϣ
*****************************************************************************/
void mbx_mntnabnormal()
{
    printf("***************************************************************\n");
    printf("�쳣��Ϣ\n");
    printf("PS���������ϱ���CT/BT��ϢID   : %d\n", g_stMbxMntn.stAbnormal.ulMulCtBtMsg);
    printf("PS�����ϱ��Ĵ����ģ��ID      : %d\n", g_stMbxMntn.stAbnormal.ucComPsType);
    printf("����������ʱʱ��        : %d\n", g_stMbxMntn.stAbnormal.ulLongestReq);
    printf("***************************************************************\n");
}


/*****************************************************************************
 �� �� ��  : mbx_mntnother
 ��������  : ����������ά�ɲ���Ϣ
*****************************************************************************/
void mbx_mntnother()
{
    u32 i = 0, j = 0;
    u32 ulStart = 0;
    
    printf("***************************************************************\n");
    printf("������Ϣ\n");
    printf("PS�����Ƿ���֡�ж�  : %s\n", (g_stMbxCtrl.bIsSubFrameInt)?"yes":"no");
    printf("PS���䵱ǰ��Ϣ����  : %d\n", g_stMbxCtrl.ulMsgLen);

    printf("���ʮ�ε͹��Ļ���ʱ����Ϣ\n");

    ulStart = (g_stMbxMntn.stSleepInfo.ulPtr + MAILBOX_MNTN_NUMBER - MBX_PRINT_TIMES)%MAILBOX_MNTN_NUMBER;

    for(i = 0; i < MBX_PRINT_TIMES; i++)
    {
        j = (ulStart + i)%MAILBOX_MNTN_NUMBER;

        if(0 != g_stMbxMntn.stSleepInfo.astwakeup[j].ulSlice1)
        {
            printf("| slice1(0x%08x) - slice2(0x%08x) | = (0x%08x)   slice3 : 0x%08x\n",
                g_stMbxMntn.stSleepInfo.astwakeup[j].ulSlice1,
                g_stMbxMntn.stSleepInfo.astwakeup[j].ulSlice2,
                (g_stMbxMntn.stSleepInfo.astwakeup[j].ulSlice2 - g_stMbxMntn.stSleepInfo.astwakeup[j].ulSlice1),
                g_stMbxMntn.stSleepInfo.astwakeup[j].ulSlice3);
        }
    }
    
    printf("***************************************************************\n");
}


/*****************************************************************************
 �� �� ��  : mbx_mntnnum
 ��������  : ���������¼�����Ϣ
*****************************************************************************/
void mbx_mntnnum()
{
    printf("***************************************************************\n");
    printf("������Ϣ\n");
    printf("PS�����жϴ���      : %d\n", g_stMbxMntn.stNumber.ulComPsIntNum);
    printf("ר�������жϴ���    : %d\n", g_stMbxMntn.stNumber.ulSpIntNum);

    printf("PS����д�������    : %d\n", g_stMbxMntn.stNumber.ulPsComReqNum);
    printf("PS����PSд�������  : %d\n", g_stMbxMntn.stNumber.ulPsComPsReqNum);
    printf("PS����MSPд������� : %d\n", g_stMbxMntn.stNumber.ulPsComMspReqNum);
    printf("OM����д�������    : %d\n", g_stMbxMntn.stNumber.ulOmComReqNum);

    printf("PS����PS��ȡ����    : %d\n", g_stMbxMntn.stNumber.ulPsComPsRdNum);
    printf("PS��ȡ�ɹ�����      : %d\n", g_stMbxMntn.stNumber.ulPsComPsRdOKNum);
    printf("MSP��ȡ����         : %d\n", g_stMbxMntn.stNumber.ulPsComMspRdNum);
    printf("MSP��ȡ�ɹ�����     : %d\n", g_stMbxMntn.stNumber.ulPsComMspRdOKNum);

    printf("PS����PS�ص�����    : %d\n", g_stMbxMntn.stNumber.ulPsComPsNotifyNum);
    printf("PS����MSP�ص�����   : %d\n", g_stMbxMntn.stNumber.ulPsComMspNotifyNum);

    printf("***************************************************************\n");
}


/*****************************************************************************
 �� �� ��  : mbx_mntnmailfull
 ��������  : ������д�붪ʧ��Ϣ
*****************************************************************************/
void mbx_mntnmailfull()
{
    u32 i = 0, j = 0;
    u32 ulStart = 0;

    ulStart = g_stMbxMntn.stMailFull.ulPtr;

    printf("***************************************************************\n");
    printf("��������Ϣ\n");
    printf("����������          : %d\n", g_stMbxMntn.stMailFull.ulFullNum);

    for(i = 0; i < MAILBOX_MNTN_NUMBER; i++)
    {
        j = (ulStart + i)%MAILBOX_MNTN_NUMBER;

        if(0 != g_stMbxMntn.stMailFull.astFullInfo[j].ulSlice)
        {
            printf("���� : %d   ��ϢID : 0x%08x   ʱ�� : 0x%08x\n",
                g_stMbxMntn.stMailFull.astFullInfo[j].ulModId
                , g_stMbxMntn.stMailFull.astFullInfo[j].ulMsgId
                , g_stMbxMntn.stMailFull.astFullInfo[j].ulSlice);
        }
    }

    printf("***************************************************************\n");
}


/*****************************************************************************
 �� �� ��  : mbx_mntnreqinfo
 ��������  : ����д������Ϣ
*****************************************************************************/
void mbx_mntnreqinfo()
{
    u32 i = 0, j = 0;
    u32 ulStart = 0;

    printf("***************************************************************\n");
    printf("����д������Ϣ\n");

    ulStart = (g_stMbxMntn.stReqInfo.ulPtr + MAILBOX_MNTN_NUMBER - MBX_PRINT_TIMES)%MAILBOX_MNTN_NUMBER;

    for(i = 0; i < MBX_PRINT_TIMES; i++)
    {
        j = (ulStart + i)%MAILBOX_MNTN_NUMBER;

        if(0 != g_stMbxMntn.stReqInfo.astReq[j].ulSliceEnd)
        {
            printf("���� : %d   ��ϢID : 0x%08x   ����ʱ�� : 0x%08x   д��ʱ�� : 0x%08x\n",
                g_stMbxMntn.stReqInfo.astReq[j].ulModId, g_stMbxMntn.stReqInfo.astReq[j].ulMsgId,
                g_stMbxMntn.stReqInfo.astReq[j].ulSliceStart, g_stMbxMntn.stReqInfo.astReq[j].ulSliceEnd);
        }
    }

    printf("***************************************************************\n");
}


/*****************************************************************************
 �� �� ��  : mbx_mntncnfinfo
 ��������  : �����������Ϣ
*****************************************************************************/
void mbx_mntncnfinfo()
{
    u32 i = 0, j = 0;
    u32 ulStart = 0;

    printf("***************************************************************\n");
    
    printf("���ʮ�θ����жϵ���Ϣ\n");

    ulStart = (g_stMbxMntn.stIntInfo.ulPtr + MAILBOX_MNTN_NUMBER - MBX_PRINT_TIMES)%MAILBOX_MNTN_NUMBER;

    for(i = 0; i < MBX_PRINT_TIMES; i++)
    {
        j = (ulStart + i)%MAILBOX_MNTN_NUMBER;

        if(0 != g_stMbxMntn.stIntInfo.aulIntSlice[j])
        {
            printf("interrupt slice : 0x%08x\n", g_stMbxMntn.stIntInfo.aulIntSlice[j]);
        }
    }

    printf("���ʮ��ԭ���ȡ����Ϣ\n");

    ulStart = (g_stMbxMntn.stReadInfo.ulPsPtr + MAILBOX_MNTN_NUMBER - MBX_PRINT_TIMES)%MAILBOX_MNTN_NUMBER;

    for(i = 0; i < MBX_PRINT_TIMES; i++)
    {
        j = (ulStart + i)%MAILBOX_MNTN_NUMBER;

        if(0 != g_stMbxMntn.stReadInfo.astRead[j].ulSlice)
        {
            printf("app read , src 0x%x, slice 0x%08x\n", 
                g_stMbxMntn.stReadInfo.astRead[j].ulSrc, g_stMbxMntn.stReadInfo.astRead[j].ulSlice);
        }
    }

    ulStart = (g_stMbxMntn.stCnfInfo.ulPsPtr + MAILBOX_MNTN_NUMBER - MBX_PRINT_TIMES)%MAILBOX_MNTN_NUMBER;

    printf("����PS���10����������Ϣ\n");

    for(i = 0; i < MBX_PRINT_TIMES; i++)
    {
        j = (ulStart + i)%MAILBOX_MNTN_NUMBER;

        if(0 != g_stMbxMntn.stCnfInfo.astPsCnf[j].ulSlice)
        {
            printf("��ϢID : 0x%08x   ��ȡʱ�� : 0x%08x\n",
                g_stMbxMntn.stCnfInfo.astPsCnf[j].ulMsgId, g_stMbxMntn.stCnfInfo.astPsCnf[j].ulSlice);
        }
    }

    ulStart = (g_stMbxMntn.stCnfInfo.ulCbtPtr + MAILBOX_MNTN_NUMBER - MBX_PRINT_TIMES)%MAILBOX_MNTN_NUMBER;

    printf("����CT/BT���10����������Ϣ\n");

    for(i = 0; i < MBX_PRINT_TIMES; i++)
    {
        j = (ulStart + i)%MAILBOX_MNTN_NUMBER;

        if(0 != g_stMbxMntn.stCnfInfo.astCbtCnf[j].ulSlice)
        {
            printf("��ϢID : 0x%08x   ��ȡʱ�� : 0x%08x\n",
                g_stMbxMntn.stCnfInfo.astCbtCnf[j].ulMsgId, g_stMbxMntn.stCnfInfo.astCbtCnf[j].ulSlice);
        }
    }

    printf("***************************************************************\n");
}


/*****************************************************************************
 �� �� ��  : mbx_mntnerrorcode
 ��������  : ��������������Ϣ
*****************************************************************************/
void mbx_mntnerrorcode()
{
    printf("***************************************************************\n");
    printf("��������Ϣ\n");
    printf("1  : ����δ��ʼ��.\n");
    printf("2  : �����������.\n");
    printf("3  : ������.\n");
    printf("4  : ��ʱ.\n");
    printf("5  : ר���������ݸ���.\n");
    printf("6  : дʧ��.\n");
    printf("7  : ��ʧ��.\n");
    printf("8  : ����ϢΪ��.\n");
    printf("9  : DSP˯��.\n");
    printf("10 : ����.\n");
    printf("***************************************************************\n");
}


void mbx_debug(BSP_BOOL bDebug)
{
    g_stMbxMntn.bdbg = bDebug;
}

/*****************************************************************************
 �� �� ��  : mailbox_help
 ��������  : �����ӡ����
*****************************************************************************/
void mailbox_help()
{
    printf("***************************************************************\n");
    printf("�����ַ�����Ϣ                mbx_mntnaddr\n");
    printf("���������¼�����Ϣ            mbx_mntnnum\n");
    printf("����д������Ϣ                  mbx_mntnreqinfo\n");
    printf("�����������Ϣ                  mbx_mntncnfinfo\n");
    printf("������д�붪ʧ��Ϣ              mbx_mntnmailfull\n");
    printf("�����쳣��Ϣ                    mbx_mntnabnormal\n");
    printf("����������ά�ɲ���Ϣ            mbx_mntnother\n");
    printf("�������ʧ�ܴ�������Ϣ          mbx_mntnerrorcode\n");
    printf("�����ӡ��Ϣ����                mbx_debug 1/0\n");
    printf("***************************************************************\n");
}


/*lint -save -e545 -e813*/
/*****************************************************************************
 Prototype       : Mbx_BbeIpcProc
 Description     : ������쳣����
                   BBE�����쳣ʱ�������쳣��Ϣ��AXI memory�У�Ȼ��ͨ��IPC�ж�
                   ֪ͨ����ģ�飬������ģ���¼�쳣��Ϣ���������쳣�����ӿ�
 Return Value    : NONE
*****************************************************************************/
void Mbx_BbeIpcProc(void)
{
    u8  ucData[DUMP_USER_DATA_SIZE] = {0};     /* 4K���棬ǰ32�ֽڴ�DSP��Ϣ */
    u32 *pAddr;
    u16 usMode;

    usMode = *(u16*)SRAM_DSP_MNTN_INFO_ADDR;

    /* 0��ʾPS������ */
    if(0 == usMode)
    {
        /* ǰ4���ֱ��洫��Ĳ��� */
        pAddr = (u32*)ucData;
        
        *(pAddr++) = MBX_ERROR_DSP_IPC;
        *(pAddr++) = 0;
        *(pAddr++) = 0;
        *(pAddr++) = 0;

        /* ����DSP���쳣��Ϣ */
        memcpy((u8*)pAddr, (u8*)SRAM_DSP_MNTN_INFO_ADDR, SRAM_DSP_MNTN_SIZE);

        /* �ӵ�512�ֽڿ�ʼ���������������쳣��Ϣ */
        Mbx_DumpSave(ucData+MBX_ERROR_RESERVE_SIZE, (DUMP_USER_DATA_SIZE-MBX_ERROR_RESERVE_SIZE));

        system_error(DRV_ERRNO_MBX_DSP_IPC, MBX_ERROR_DSP_IPC, 0, (char*)ucData, DUMP_USER_DATA_SIZE);
    }
}
/*lint -restore*/


/*****************************************************************************
 �� �� ��  : bsp_mailbox_init
 ��������  : ����ģ���ʼ��
 �������  : None
 �������  : None
 �� �� ֵ  : None
*****************************************************************************/
void bsp_mailbox_init(void)
{
    s32 ret;

    if(BSP_TRUE == g_stMbxCtrl.bMbxInit)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_MBX, "[mailbox]: <%s> enter again.\n", __FUNCTION__);
        return;
    }

    memset(&g_stMbxCtrl, 0, sizeof(MAILBOX_MAINCTRL_STRU));         /*lint !e516*/

    memset((void*)(&g_stMbxMntn), 0, sizeof(MAILBOX_MNTN_STRU));

    g_stMbxCtrl.bDspWakeFlag = BSP_FALSE;

    osl_sem_init(0, &g_stMbxCtrl.semDspWakeup);

    osl_sem_init(1, &g_stMbxCtrl.semDspMutex);

    g_stMbxCtrl.pulAxiAddr = &((SRAM_SMALL_SECTIONS*)SRAM_SMALL_SECTIONS_ADDR)->SRAM_DSP_DRV;

    g_stMbxCtrl.lAxiCnt = 0;

    /* PS����Ĭ��ʹ��IPC�ж� */
    ret = (s32)BSP_IPC_IntConnect(IPC_INT_DSP_PS_PUB_MBX, (VOIDFUNCPTR)Mbx_ComPsIntProc, 0);
    if(BSP_OK != ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_MBX, "[mailbox]: <%s> BSP_IPC_IntConnect failed.\n", __FUNCTION__);
        return ;
    }

    ret = (s32)BSP_IPC_IntEnable(IPC_INT_DSP_PS_PUB_MBX);
    if(BSP_OK != ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_MBX, "[mailbox]: <%s> BSP_IPC_IntEnable failed.\n", __FUNCTION__);
        return ;
    }

    /* DSP��ά�ɲ�IPC�ж� */
    ret = (s32)BSP_IPC_IntConnect(IPC_CCPU_INT_SRC_DSP_MNTN, (VOIDFUNCPTR)Mbx_BbeIpcProc, 0);
    if(BSP_OK != ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_MBX, "[mailbox]: <%s> BSP_IPC_IntConnect failed.\n", __FUNCTION__);
        return ;
    }

    ret = (s32)BSP_IPC_IntEnable(IPC_CCPU_INT_SRC_DSP_MNTN);
    if(BSP_OK != ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_MBX, "[mailbox]: <%s> BSP_IPC_IntEnable failed.\n", __FUNCTION__);
        return ;
    }

    g_stMbxCtrl.bIsSubFrameInt = BSP_FALSE;

    bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_MBX, "[mailbox]: <%s> success.\n", __FUNCTION__);

    return;
}


/*****************************************************************************
 �� �� ��  : Mbx_ForbidDspSleep
 ��������  : ����AXI memory��ֹDSP����˯��
 �������  : None
 �������  : None
 �� �� ֵ  : None
*****************************************************************************/
void Mbx_ForbidDspSleep(void)
{
    s32 key;
    
    key = intLock();
    g_stMbxCtrl.lAxiCnt++;
    writel(1, (unsigned)(g_stMbxCtrl.pulAxiAddr));
    intUnlock(key);
}


/*****************************************************************************
 �� �� ��  : Mbx_ForbidDspSleep
 ��������  : ���AXI memory����DSP����˯��
 �������  : None
 �������  : None
 �� �� ֵ  : None
*****************************************************************************/
void Mbx_AllowDspSleep(void)
{
    s32 key;
    
    key = intLock();
    if((--g_stMbxCtrl.lAxiCnt) <= 0)
    {
        g_stMbxCtrl.lAxiCnt = 0;
        writel(0, (unsigned)(g_stMbxCtrl.pulAxiAddr));
    }
    intUnlock(key);
}



u32 Mbx_SpaceMalloc(MAILBOX_RINGBUFF_STRU* pMbxHandle,u32 ulSizeReq)
{
    u32 ulFreeBuffSize;
    u32 wr,rd,size;

    wr   = pMbxHandle->ulRbufWrPtr;
    rd   = pMbxHandle->ulRbufRdPtr;
    size = pMbxHandle->ulRbufSize;
    
    /* ���дָ��Ƿ����������� */
    if(wr > size)
    {
        Mbx_SystemError((u32)MBX_ERROR_UP_WR, wr, rd, ((__LINE__ << 16)|size));
    }

    if(wr < rd)
    {
        ulFreeBuffSize = (rd - wr);
    }
    else
    {
        ulFreeBuffSize = (size + rd - wr);
    }

    if(ulFreeBuffSize >= (ulSizeReq+4))
    {
        return BSP_OK;
    }
    else
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_MBX, 
            "[mailbox]: <%s> Mbx_SpaceMalloc failed, ulFreeBuffSize(%d), ulSizeReq(%d), wr(%d), rd(%d), size(%d).\n", 
            __FUNCTION__,ulFreeBuffSize, ulSizeReq, wr, rd, size);
        return (u32)BSP_ERROR;
    }
}


void Mbx_MntnMailFull(MAILBOX_MSG_HAED_STRU* pstMsg)
{
    g_stMbxMntn.stMailFull.ulFullNum++;

    g_stMbxMntn.stMailFull.astFullInfo[g_stMbxMntn.stMailFull.ulPtr].ulModId = (u32)(pstMsg->usDstMod);
    g_stMbxMntn.stMailFull.astFullInfo[g_stMbxMntn.stMailFull.ulPtr].ulMsgId = *(u32*)(pstMsg->pPayload);
    g_stMbxMntn.stMailFull.astFullInfo[g_stMbxMntn.stMailFull.ulPtr].ulSlice = BSP_GetSliceValue();

    g_stMbxMntn.stMailFull.ulPtr = (g_stMbxMntn.stMailFull.ulPtr+1)%MAILBOX_MNTN_NUMBER;
}


/*****************************************************************************
 Prototype       : Mbx_RbuffWrite
 Description     : ѭ��bufferд�ӿ�
 param           : pHead        ѭ��buffer���׵�ַ
                   pstMsg       ��д������ָ��
 Return Value    : BSP_OK : �ɹ�
                   ERR_MAILBOX_MEMORY_NOTENOUGH : ʣ��ռ䲻��
*****************************************************************************/
s32 Mbx_RbuffWrite(MAILBOX_RINGBUFF_STRU* pHead, MAILBOX_MSG_HAED_STRU* pstMsg)
{
    u32 ulMsgSize = pstMsg->usMsgSize + (sizeof(MAILBOX_MSG_HAED_STRU) - 4);
    u32 ulCpySize0;
    u32 ulCpySize1;
    u32 ulTempPPTR;

    /* ����ʣ��ռ��� */
    if(BSP_OK != Mbx_SpaceMalloc(pHead,ulMsgSize))
    {
        Mbx_MntnMailFull(pstMsg);
        return ERR_MAILBOX_MEMORY_NOTENOUGH;
    }

    /* ����Ϣд�� */
    if((ulMsgSize + pHead->ulRbufWrPtr) <= pHead->ulRbufSize)
    {
        memcpy((void*)(pHead->ulRbufWrPtr + pHead->pucRbufAddr + MAILBOX_ADDR_OFFSET),
                        (void*)pstMsg, 
                        (sizeof(MAILBOX_MSG_HAED_STRU) - 4));
        memcpy((void*)(pHead->ulRbufWrPtr + pHead->pucRbufAddr + MAILBOX_ADDR_OFFSET + (sizeof(MAILBOX_MSG_HAED_STRU) - 4)),
                        (void*)(pstMsg->pPayload), 
                        pstMsg->usMsgSize);     /*lint !e516*/
    }
    else
    {
        ulCpySize0 = pHead->ulRbufSize - pHead->ulRbufWrPtr;
        ulCpySize1 = ulMsgSize - ulCpySize0;

        if(ulCpySize0 < (sizeof(MAILBOX_MSG_HAED_STRU) - 4))
        {
            memcpy((void*)(pHead->pucRbufAddr + pHead->ulRbufWrPtr + MAILBOX_ADDR_OFFSET),
                            (void*)pstMsg, 
                            ulCpySize0);
            
            memcpy((void*)(pHead->pucRbufAddr + MAILBOX_ADDR_OFFSET),
                            (void*)((u32)(pstMsg)+ulCpySize0), 
                            ((sizeof(MAILBOX_MSG_HAED_STRU) - 4)-ulCpySize0));
            
            memcpy((void*)(pHead->pucRbufAddr + MAILBOX_ADDR_OFFSET + ((sizeof(MAILBOX_MSG_HAED_STRU) - 4)-ulCpySize0)), 
                            (void*)(pstMsg->pPayload), 
                            pstMsg->usMsgSize);     /*lint !e516*/
        }
        else
        {
            memcpy((void*)(pHead->pucRbufAddr + pHead->ulRbufWrPtr + MAILBOX_ADDR_OFFSET),
                            (void*)(pstMsg), 
                            (sizeof(MAILBOX_MSG_HAED_STRU) - 4));
            
            memcpy((void*)(pHead->pucRbufAddr + pHead->ulRbufWrPtr + MAILBOX_ADDR_OFFSET + (sizeof(MAILBOX_MSG_HAED_STRU) - 4)),
                            (void*)(pstMsg->pPayload), 
                            (ulCpySize0 - (sizeof(MAILBOX_MSG_HAED_STRU) - 4)));
            
            memcpy((void*)(pHead->pucRbufAddr + MAILBOX_ADDR_OFFSET), 
                            (void*)((u32)(pstMsg->pPayload) + (ulCpySize0 - (sizeof(MAILBOX_MSG_HAED_STRU) - 4))), 
                            ulCpySize1);
        }
    }

    /* дָ�����,��֤���ֽڶ��� */
    ulTempPPTR = ((pHead->ulRbufWrPtr + ulMsgSize + 3) & (~3));

    if(ulTempPPTR >= pHead->ulRbufSize)
    {
        ulTempPPTR -= pHead->ulRbufSize;
    }

    Mbx_MntnWriteEnd(pHead, pstMsg, ulTempPPTR);

    pHead->ulRbufWrPtr = ulTempPPTR;

    return BSP_OK;
}
u32 Mbx_GetNewMsgLen (MAILBOX_MSG_HAED_STRU* pMsg)
{
    return(u32)(pMsg->usMsgSize);
}



u32 Mbx_NewMsgReadOut(MAILBOX_MSG_HAED_STRU* pMsg,MAILBOX_RINGBUFF_STRU* pMbxHandle)
{
    u32 ulMsgSize;
    u32 ulCpySize0;
    u32 ulCpySize1;
    u32 ulTempCPTR;
    MAILBOX_MSG_HAED_STRU *pstMsg = NULL;

    pstMsg = (MAILBOX_MSG_HAED_STRU*)(pMbxHandle->pucRbufAddr + MAILBOX_ADDR_OFFSET + pMbxHandle->ulRbufRdPtr);
    ulMsgSize = Mbx_GetNewMsgLen(pstMsg) + (sizeof(MAILBOX_MSG_HAED_STRU) - 4);

    /* user space not enough, return failure, avoid overflow */
    if(pstMsg->usMsgSize > pMsg->usMsgSize)
    {
        pMsg->usMsgSize = pstMsg->usMsgSize;
        goto newmsgreadfailed;
    }

    /* ����Ϣ���� */
    if((ulMsgSize + pMbxHandle->ulRbufRdPtr) <= pMbxHandle->ulRbufSize)
    {
        memcpy((void*)pMsg, 
                (void*)(pMbxHandle->ulRbufRdPtr + pMbxHandle->pucRbufAddr + MAILBOX_ADDR_OFFSET), 
                (sizeof(MAILBOX_MSG_HAED_STRU) - 4));
        
        memcpy((void*)(pMsg->pPayload), 
                (void*)(pMbxHandle->ulRbufRdPtr + pMbxHandle->pucRbufAddr + MAILBOX_ADDR_OFFSET + (sizeof(MAILBOX_MSG_HAED_STRU) - 4)), 
                pstMsg->usMsgSize);     /*lint !e516*/
    }
    else
    {
        ulCpySize0 = pMbxHandle->ulRbufSize - pMbxHandle->ulRbufRdPtr;
        ulCpySize1 = ulMsgSize - ulCpySize0;

        if(ulCpySize0 < (sizeof(MAILBOX_MSG_HAED_STRU) - 4))
        {
            memcpy((void*)(pMsg), 
                    (void*)(pMbxHandle->pucRbufAddr + pMbxHandle->ulRbufRdPtr + MAILBOX_ADDR_OFFSET),
                    ulCpySize0);
            
            memcpy((void*)((u32)pMsg + ulCpySize0), 
                    (void*)(pMbxHandle->pucRbufAddr + MAILBOX_ADDR_OFFSET),
                    (sizeof(MAILBOX_MSG_HAED_STRU) - 4) - ulCpySize0);
            
            memcpy((void*)(pMsg->pPayload), 
                    (void*)(pMbxHandle->pucRbufAddr + MAILBOX_ADDR_OFFSET + (sizeof(MAILBOX_MSG_HAED_STRU) - 4) - ulCpySize0),
                    pstMsg->usMsgSize);     /*lint !e516*/
        }
        else
        {
            memcpy((void*)(pMsg), 
                    (void*)(pMbxHandle->pucRbufAddr + pMbxHandle->ulRbufRdPtr + MAILBOX_ADDR_OFFSET),
                    (sizeof(MAILBOX_MSG_HAED_STRU) - 4));
            
            memcpy((void*)(pMsg->pPayload), 
                    (void*)(pMbxHandle->pucRbufAddr + pMbxHandle->ulRbufRdPtr + MAILBOX_ADDR_OFFSET + (sizeof(MAILBOX_MSG_HAED_STRU) - 4)),
                    ulCpySize0 - (sizeof(MAILBOX_MSG_HAED_STRU) - 4));
            
            memcpy((void*)((u32)(pMsg->pPayload) + ulCpySize0 - (sizeof(MAILBOX_MSG_HAED_STRU) - 4)), 
                    (void*)(pMbxHandle->pucRbufAddr + MAILBOX_ADDR_OFFSET),
                    ulCpySize1);
        }
    }

newmsgreadfailed:

    /* ��ָ�����,��֤���ֽڶ��� */
    ulTempCPTR = ((pMbxHandle->ulRbufRdPtr + ulMsgSize + 3) & (~3));

    if((ulTempCPTR) >= pMbxHandle->ulRbufSize)
    {
        ulTempCPTR -= pMbxHandle->ulRbufSize;
    }

    Mbx_MntnReadEnd(pMsg->pPayload, (enum __MBX_ADDRESSEE_ID_ENUM__)pMsg->usDstMod, ulTempCPTR, pMbxHandle->ulRbufWrPtr);

    pMbxHandle->ulRbufRdPtr = ulTempCPTR;

    return (ulTempCPTR);
}


/*****************************************************************************
 Prototype       : BSP_MailBox_GetDspStatusReg
 Description     : ��ȡDSP˯��״̬�ص�����ע��ӿڣ��͹���ģ�����
 param           : pFun             ��ȡDSP˯��״̬������
 Return Value    : BSP_VOID
*****************************************************************************/
void BSP_MailBox_GetDspStatusReg(BSP_MBX_GET_STATUS_FUNC pFun)
{
    g_stMbxCtrl.pfnGetDspStatusProc = pFun;
}


/*****************************************************************************
 Prototype       : BSP_MailBox_DspForceAwakeReg
 Description     : DSPǿ�ƻ��ѻص�����ע��ӿڣ��͹���ģ�����
 param           : pFun             DSPǿ�ƻ��Ѵ�����
 Return Value    : BSP_VOID
*****************************************************************************/
void BSP_MailBox_DspForceAwakeReg(BSP_MBX_DSP_AWAKE_FUNC pFun)
{
    g_stMbxCtrl.pfnDspForceAwakeProc = pFun;
}


/*****************************************************************************
 Prototype       : BSP_MailBox_GetSpMsgStatusReg
 Description     : ��ȡר����������DSP˯��״̬�ص�����ע��ӿڣ�PS����
 param           : pFun             ��ȡDSP˯��״̬������
 Return Value    : BSP_VOID
*****************************************************************************/
void BSP_MailBox_GetSpMsgStatusReg(BSP_MBX_GET_STATUS_FUNC pFun)
{
    g_stMbxCtrl.pfnGetSpMsgProc = pFun;
}


/*****************************************************************************
 Prototype       : BSP_MailBox_DspAwakeProc
 Description     : DSP���Ѻ�����䴦��ӿڣ��͹���ģ�����
 param           : BSP_VOID
 Return Value    : BSP_VOID
*****************************************************************************/
void BSP_MailBox_DspAwakeProc(void)
{
    /* ֻ��������ģ�鷢��Ļ��� */
    if(BSP_TRUE == g_stMbxCtrl.bDspWakeFlag)
    {
        g_stMbxMntn.stSleepInfo.astwakeup[g_stMbxMntn.stSleepInfo.ulPtr].ulSlice2 = BSP_GetSliceValue();
        g_stMbxCtrl.bDspWakeFlag = BSP_FALSE;
        osl_sem_up(&g_stMbxCtrl.semDspWakeup);
    }
}


/*****************************************************************************
 Prototype       : BSP_MailBox_IsAllowDspSleep
 Description     : ��ѯ�Ƿ�����DSP˯�ߵĴ���ӿڣ��͹���ģ�����
                    ���ô˽ӿ�ʱ�͹���ģ�鱣֤DSPδ˯��
                    �˽ӿ�ֻ���PS�����ר�����䣬�����OM����(OMԭ����ʱ��Ҫ��)
 param           : BSP_VOID
 Return Value    : BSP_BOOL: TRUE-allow;FALSE-forbid
*****************************************************************************/
BSP_BOOL BSP_MailBox_IsAllowDspSleep(void)
{
    BSP_BOOL bFlag;
    MAILBOX_RINGBUFF_STRU* pstComPsUpMail = (MAILBOX_RINGBUFF_STRU*)MAILBOX_PS_UP_ADDR;
    MAILBOX_RINGBUFF_STRU* pstComPsDownMail = (MAILBOX_RINGBUFF_STRU*)MAILBOX_PS_DOWN_ADDR;
    MAILBOX_RINGBUFF_STRU* pstOmDownMail = (MAILBOX_RINGBUFF_STRU*)MAILBOX_OM_DOWN_ADDR;

    if(!(g_stMbxCtrl.bMbxInit))
    {
        g_stMbxMntn.stAbnormal.ulNInitSlic = BSP_GetSliceValue();
        return BSP_TRUE;
    }

    /* ��һ���ȼ��Ȳ�AXI memory��AXI memory����ʱ������DSP˯�� */
    if(*(g_stMbxCtrl.pulAxiAddr) == 1)
    {
        return BSP_FALSE;
    }

    /* ר��������ԭ��δ���߽�ֹ˯�� */
    if(NULL != g_stMbxCtrl.pfnGetSpMsgProc)
    {
        bFlag = g_stMbxCtrl.pfnGetSpMsgProc();
        if(BSP_TRUE != bFlag)
        {
            return bFlag;
        }
    }
    
    /* PS������ԭ��δ���߽�ֹ˯�� */
    if((pstComPsUpMail->ulRbufRdPtr != pstComPsUpMail->ulRbufWrPtr)
        || (pstComPsDownMail->ulRbufRdPtr != pstComPsDownMail->ulRbufWrPtr)
        || (pstOmDownMail->ulRbufRdPtr != pstOmDownMail->ulRbufWrPtr))
    {
        return BSP_FALSE;
    }

    return BSP_TRUE;
}


/*****************************************************************************
 Prototype       : BSP_UniMailboxWrite
 Description     : �����ͨ��д�ӿ�(�ɵ����߱�֤����ĵ�ַ�ĺϷ���)
                   ���ӿ�ʵ������memcpy����������
                   �ӿ��ڲ���֤DSP�Ļ��Ѳ������������ʧ�ܣ�����ERR_MAILBOX_TIMEOUT
 Return Value    : 
                   BSP_OK
                   ERR_MAILBOX_PARAM_INCORRECT
                   ERR_MAILBOX_TIMEOUT
*****************************************************************************/
u32 BSP_UniMailboxWrite(void *pDst, void *pSrc, u32 ulLen)
{
    u32 ulret;

    if((NULL == pDst) || (NULL == pSrc) || (0 == ulLen))
    {
        return ERR_MAILBOX_PARAM_INCORRECT;
    }
    
    ulret = BSP_Mailbox_ForbidDspSleep(EN_MAILBOX_SLEEP_WAKEUP);
    if(ulret)
    {
        return ulret;
    }

    memcpy(pDst, pSrc, ulLen);

    BSP_Mailbox_AllowDspSleep();

    return BSP_OK;
}


/*****************************************************************************
 Prototype       : BSP_MailboxAbnormalProc
 Description     : �����쳣����
                   Э��ջ������Ϣ��ʱ����ô˽ӿڱ������������Ϣ
                   ��Э��ջ�̶���ǰ0.5K����Э��ջ��Ϣ����3.5K����������Ϣ
 Input           : ucData       �����׵�ַ
 Return Value    : NONE
*****************************************************************************/
void BSP_MailboxAbnormalProc(u8 *ucData)
{
    if(BSP_NULL == ucData)
    {
        return;
    }

    bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_MBX, "[mailbox]: <%s> enter.\n", __FUNCTION__);
    
    Mbx_DumpSave(ucData, (DUMP_USER_DATA_SIZE-MBX_ERROR_RESERVE_SIZE));

    bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_MBX, "[mailbox]: <%s> exit.\n", __FUNCTION__);
}


/*****************************************************************************
 Prototype       : Mbx_DumpSave
 Description     : �����쳣������Ϣ����ӿ�
                    ��¼���е�������Ϣ
                    ��¼���еĶ�ȡ���󡢶�ȡ�����Ϣ
                    ��¼���һ��DSP IPC�ж��ϱ���ʱ��
                    ��¼����DSP��ʱ����Ϣ
 Input           :  pData ��������ʼ��ַ
                    ulSize ��������С
 Return Value    : NONE
*****************************************************************************/
void Mbx_DumpSave(u8* pData, u32 ulSize)
{
    u32 i,ptr;
    u32 ultmp = 0;

    /* ����Ŀռ��С��С3.5K */
    if((ulSize < (DUMP_USER_DATA_SIZE-MBX_ERROR_RESERVE_SIZE)) || (NULL == pData))
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_MBX, "[mailbox]: <%s> ulSize %d, pData %p.\n", ulSize, pData);
        return;
    }
    
    ptr = g_stMbxMntn.stReqInfo.ulPtr;
    for(i = 0; i < MAILBOX_MNTN_NUMBER; i++)
    {
        /* (msg ID/RWpointer/start slice/end slice) */
        memcpy(&pData[ultmp], (u8*)&(g_stMbxMntn.stReqInfo.astReq[ptr].ulMsgId), 4*sizeof(u32)); /*lint !e420*/ /* [false alarm]:fortify */
        ultmp += 4*sizeof(u32);
        ptr = (ptr+1)%MAILBOX_MNTN_NUMBER;
    }

    /* ultmp value is 1600 */
    printf("ultmp = %d.\n", ultmp);

    ptr = g_stMbxMntn.stCnfInfo.ulPsPtr;
    for(i = 0; i < MAILBOX_MNTN_NUMBER; i++)
    {
        /* (msg ID/slice) */
        memcpy(&pData[ultmp], (u8*)&(g_stMbxMntn.stCnfInfo.astPsCnf[ptr]), 2*sizeof(u32));
        ultmp += 2*sizeof(u32);
        ptr = (ptr+1)%MAILBOX_MNTN_NUMBER;
    }

    /* ultmp value is 2400 */
    printf("ultmp = %d.\n", ultmp);

    ptr = g_stMbxMntn.stReadInfo.ulPsPtr;
    for(i = 0; i < MAILBOX_MNTN_NUMBER; i++)
    {
        /* (read src/slice) */
        memcpy(&pData[ultmp], (u8*)&(g_stMbxMntn.stReadInfo.astRead[ptr]), 2*sizeof(u32));
        ultmp += 2*sizeof(u32);
        ptr = (ptr+1)%MAILBOX_MNTN_NUMBER;
    }

    /* ultmp value is 3200 */
    printf("ultmp = %d.\n", ultmp);

    /* �ռ����ޣ�ֻ��¼���ʮ�εĻ�����Ϣ */
    ptr = (g_stMbxMntn.stSleepInfo.ulPtr + MAILBOX_MNTN_NUMBER - MBX_ERROR_MNTN_NUM)%MAILBOX_MNTN_NUMBER;
    for(i = 0; i < MBX_ERROR_MNTN_NUM; i++)
    {
        /* (req slice/callback slice/proc slice) */
        memcpy(&pData[ultmp], (u8*)&(g_stMbxMntn.stSleepInfo.astwakeup[ptr]), 3*sizeof(u32));
        ultmp += 3*sizeof(u32);
        ptr = (ptr+1)%MAILBOX_MNTN_NUMBER;
    }

    /* ultmp value is 3320 */
    printf("ultmp = %d.\n", ultmp);

    /* �ռ����ޣ�ֻ��¼���ʮ�ε�IPCʱ����Ϣ */
    ptr = (g_stMbxMntn.stCallback.ulPtr + MAILBOX_MNTN_NUMBER - MBX_ERROR_MNTN_NUM)%MAILBOX_MNTN_NUMBER;
    for(i = 0; i < MBX_ERROR_MNTN_NUM; i++)
    {
        /* (callback timer) */
        memcpy(&pData[ultmp], (u8*)&(g_stMbxMntn.stCallback.aulIntSlice[ptr]), sizeof(u32));
        ultmp += sizeof(u32);
        ptr = (ptr+1)%MAILBOX_MNTN_NUMBER;
    }
    
    /* ultmp value is 3360 */
    printf("ultmp = %d.\n", ultmp);
    
    memcpy(&pData[ultmp], (u8*)&(g_stMbxMntn.stNumber), sizeof(g_stMbxMntn.stNumber));
    ultmp += sizeof(g_stMbxMntn.stNumber);

    /* ultmp value is 3408 */

    printf("ultmp = %d.\n", ultmp);

    memcpy(&pData[ultmp], (u8*)&(g_stMbxMntn.stAbnormal), sizeof(g_stMbxMntn.stAbnormal));
    ultmp += sizeof(g_stMbxMntn.stAbnormal);

    /* ultmp value is 3424 */
    printf("ultmp = %d.\n", ultmp);

    /* �����32���ֽڱ�������������ͷ��Ϣ */
    if(BSP_OK == BSP_Mailbox_ForbidDspSleep(EN_MAILBOX_SLEEP_WAKEUP))
    {
        memcpy(&pData[ulSize-(2*sizeof(MAILBOX_RINGBUFF_STRU))], 
            (BSP_VOID*)MAILBOX_PS_DOWN_ADDR, sizeof(MAILBOX_RINGBUFF_STRU)); /* [false alarm]:fortify */

        memcpy(&pData[ulSize - sizeof(MAILBOX_RINGBUFF_STRU)], 
            (BSP_VOID*)MAILBOX_PS_UP_ADDR, sizeof(MAILBOX_RINGBUFF_STRU)); /* [false alarm]:fortify */

        BSP_Mailbox_AllowDspSleep();
    }
    
}


/*****************************************************************************
 Prototype       : Mbx_SystemError
 Description     : �����쳣��������ӿ�
                    ��¼���е�������Ϣ
                    ��¼���еĶ�ȡ���󡢶�ȡ�����Ϣ
                    ��¼���һ��DSP IPC�ж��ϱ���ʱ��
                    ��¼����DSP��ʱ����Ϣ
 Input           : errcode  �����ڲ�������
                   ulPara1,ulPara2,ulPara3   ����1,2,3���쳣�����ֻ�������1
 Return Value    : NONE
*****************************************************************************/
void Mbx_SystemError(u32 errcode, u32 ulPara1, u32 ulPara2, u32 ulPara3)
{
    u8 *pData;
    u32 *pAddr;
    
    pData = malloc(DUMP_USER_DATA_SIZE);
    if(NULL == pData)
    {
        /* �����������DRV_ERRNO_MBX_WR_FULL��ʼ */
        system_error(DRV_ERRNO_MBX_WR_FULL + errcode, errcode, ulPara1, (char*)NULL, 0); /* [false alarm]:fortify */
        return ;
    }
    memset(pData, 0, DUMP_USER_DATA_SIZE);

    /* ǰ4���ֱ��洫��Ĳ��� */
    pAddr = (u32*)pData;
    
    *(pAddr++) = errcode;
    *(pAddr++) = ulPara1;
    *(pAddr++) = ulPara2;
    *(pAddr++) = ulPara3;

    /* �ӵ�512�ֽڴ���ʼ�洢��Ϣ����Э��ջ��DSP�쳣���� */
    Mbx_DumpSave((pData+MBX_ERROR_RESERVE_SIZE), (DUMP_USER_DATA_SIZE-MBX_ERROR_RESERVE_SIZE));

    system_error(DRV_ERRNO_MBX_WR_FULL + errcode, errcode, ulPara1, (char*)pData, DUMP_USER_DATA_SIZE);

    free(pData);
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


