/*************************************************************************
*   ��Ȩ����(C) 1987-2020, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  apDl.c
*
*   ��    �� :  wuzechun
*
*   ��    �� :  ʹ��HDLCЭ��,��AP����BootLoader/��д������������
*
*   �޸ļ�¼ :  2011��7��15��  v1.00  wuzechun  ����
*
*************************************************************************/

#include "OnChipRom.h"
#include "OcrShare.h"
#include "string.h"
#include "sys.h"
#include "hsuart.h"
#include "hdlc.h"
#include "secBoot.h"
#include "emmcMain.h"
#include "apDl2.h"
#include "nand.h"

/*--------------------------------------------------------------*
 * �궨��                                                       *
 *--------------------------------------------------------------*/

/*--------------------------------------------------------------*
 * �ڲ�����ԭ��˵��                                             *
 *--------------------------------------------------------------*/

/*****************************************************************************
* �� �� ��  : apSecChk
*
* ��������  : AP��װ�İ�ȫУ�麯��
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
INT32 apSecChk( UINT32 ulImgAddr )
{
    INT32 iRet;
    volatile tOcrShareData *pShareData = (tOcrShareData *)M3_SRAM_SHARE_DATA_ADDR;

    if(BL_CHECK_INSTRUCTION != *(volatile UINT32 *)(ulImgAddr + BL_CHECK_ADDR_OFFSET))
    {
        /* ����δ����������,���ش��󹩵����ߴ��� */
        print_info("\r\nimage not program!" );

        return SEC_NO_IMAGE;
    }

    if(!pShareData->bSecEn)
    {
        print_info("\r\nUnSec_boot!" );
        return SEC_SUCCESS;
    }

    iRet = secCheck(ulImgAddr, IMAGE_TYPE_BOOTLOAD);
    switch(iRet)
    {
        case SEC_SUCCESS:/*��ȫУ��ͨ��*/
            print_info("\r\nSec check ok!" );  /*lint !e616*/
            break;
            /* ������һcase����BootLoader */
        case SEC_EFUSE_NOT_WRITE:     /*lint !e825*/ /*EFUSE δ��д*/
            break;

        case SEC_SHA_CALC_ERROR:  /* Hash���㲻ͨ�� */
        case SEC_OEMCA_ERROR:  /* OEM CAУ�鲻ͨ�� */
        case SEC_IMAGE_ERROR:   /* ӳ��У�鲻ͨ�� */
        case SEC_ROOT_CA_ERROR:/* ��CAУ����� */
        case SEC_IMAGE_LEN_ERROR:/*��ȫ�汾���ȴ���*/
            print_info("\r\nSec check err!" );

            break;

        case SEC_EFUSE_READ_ERROR:/*Efuse��ȡʧ��,ʹ�ÿ��Ź���λ,�ٴγ��Զ�ȡ*/
            print_info("\r\nEfuse read err, reboot...");

            /* Efuse��ȡʧ��,��λ���� */
            setErrno(SYS_ERR_EFUSE_READ);
            wdtRebootDelayMs(TIME_DELAY_MS_2000_FOR_EFUSE_READERR);
            break;

        default:
            while(1)    /*lint !e716*/
            {
                print_info_with_u32("\r\nunhandered ret:",(UINT32)iRet);
                delay(1000*1000);   /* ��ʱ1s */
            }
            /*break;     */  /*for pc lint*/

    }

    return iRet;
}

/*****************************************************************************
* �� �� ��  : apDlInit
*
* ��������  : ���س�ʼ��
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
void apDlInit( apDlCtrlS *pstDlCtrl )
{
    pstDlCtrl->ulFileLength = 0;
    pstDlCtrl->ulPosition = 0;
    pstDlCtrl->ulAddress = 0;
    pstDlCtrl->bComplete = FALSE;

    if(HSUART_STATUS_OK != hsUartInit(pstDlCtrl->pstHsUartBuf))
    {
        print_info("\r\nhsuart init failed!");
    }

    HDLC_Init(pstDlCtrl->pstHdlcCtrl);

}

/*****************************************************************************
* �� �� ��  : apDlRecvPkt
*
* ��������  : ����һ���������ݰ�
*
* �������  : pstDlCtrl,������ƽṹ��
*                  eRepCode,������
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
apDlStatusE apDlRecvPkt(apDlCtrlS *pstDlCtrl)
{
    INT32 iRet;
    UINT8 ucRecv;
//print_info("\r\nwait for a pkt...");
    while(1)    /*lint !e716*/
    {
        /* ÿ�δ���һ���ַ� */
        iRet = hsUartRecvData(pstDlCtrl->pstHsUartBuf, &ucRecv, 1);
        if(HSUART_RECV_TIMEOUT == iRet)
        {
            return AP_DL_RECV_TIMEOUT;
        }
        else if(ERROR == iRet)
        {
            print_info_with_u32("hsuart recv err:  ", (UINT32)iRet);
            return AP_DL_ERR_RECV;
        }

//print_info_with_u32(" ", (UINT32)ucRecv);

        iRet = HDLC_Decap(pstDlCtrl->pstHdlcCtrl, ucRecv);

        /* ��������һ������,����OK */
        if(HDLC_OK == iRet)
        {
            return AP_DL_ERR_OK;
        }
        else if(HDLC_ERR_BUF_FULL == iRet)
        {
            print_info("\r\nhdlc buf full!");
            return AP_DL_BUF_FULL;
        }

    }

}

/*****************************************************************************
* �� �� ��  : sendRepPkt
*
* ��������  : ��װӦ���
*
* �������  : pstDlCtrl,������ƽṹ��
*                  eRepCode,������
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
apDlStatusE sendRepPkt(apDlCtrlS *pstDlCtrl, RepCodeE eRepCode )
{
    INT32 iRet;
    UINT32 ulLen = 0;
    UINT8 aucBuf[PKT_LEN_REP];

    aucBuf[ulLen++] = (UINT8)PKT_TYPE_REP;

    aucBuf[ulLen++] = (UINT8)eRepCode;

    /* ʹ��HDLC�ڲ�Buf����HDLC�� */
    iRet = HDLC_Encap(aucBuf, (UINT16)ulLen, pstDlCtrl->pstHdlcCtrl->au8DecapBuf, (UINT16)HDLC_BUF_SIZE, &(pstDlCtrl->u16SendLen));
    if(HDLC_OK != iRet)
    {
        return AP_DL_ERR_HDLC_ENCAP;
    }

    (void)hsUartSend(pstDlCtrl->pstHdlcCtrl->au8DecapBuf, pstDlCtrl->u16SendLen);

    return AP_DL_ERR_OK;
}

/*****************************************************************************
* �� �� ��  : sendRepErrWaitReset
*
* ��������  : ��AP����Nak����ѭ���ȴ�AP��λ
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
void sendRepErrWaitReset(apDlCtrlS *pstDlCtrl, RepCodeE eRepCode )
{
    (void)sendRepPkt(pstDlCtrl, eRepCode);

	while(1) /*lint !e716*//* Wait AP reset modem */
    {
        print_info("\r\nwait for AP to reset me, loop forever...");
        delay(1000*1000);
    }
}

/*****************************************************************************
* �� �� ��  : handerBaudSetPkt
*
* ��������  : ����AP�����������
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
INT32 handerBaudSetPkt( apDlCtrlS *pstDlCtrl )
{
    UINT8 *pu8Buf = pstDlCtrl->pstHdlcCtrl->au8DecapBuf;

    UINT32 ulBaudRate = HSUART_MAKE_LONG(pu8Buf[BAUD_SET_OS], \
                            pu8Buf[BAUD_SET_OS+1],\
                            pu8Buf[BAUD_SET_OS+2],\
                            pu8Buf[BAUD_SET_OS+3]);
//print_info_with_u32("\r\nbaud:", ulBaudRate);
    /* ����Ack֮�����޸Ĳ����� */
    if(!isHsSptBaud(ulBaudRate))
    {
        (void)sendRepPkt(pstDlCtrl, REP_BAUD_NOT_SUPPORT);
        return ERROR;
    }

    (void)sendRepPkt(pstDlCtrl, REP_ACK);
 // print_info_with_u32("\r\nrep_ack ,baud:", ulBaudRate);
    /* ���²����� */
    if(hsUartSetBaud(ulBaudRate) != HSUART_STATUS_OK)
    {
        print_info("\r\n hsUartSetBaud error!!!\r\n");
        return ERROR;
    }

    return OK;

}

/*****************************************************************************
* �� �� ��  : handerApCmdPkt
*
* ��������  : ����AP�����������
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
INT32 handerApCmdPkt( apDlCtrlS *pstDlCtrl )
{
    INT32 iRet;
    int BOOT_RST_Addr = 0x00;
    volatile tOcrShareData *pShareData = (tOcrShareData*)M3_SRAM_SHARE_DATA_ADDR;
    UINT8 u8Cmd = pstDlCtrl->pstHdlcCtrl->au8DecapBuf[AP_CMD_OS];

    switch(u8Cmd)
    {
        case AP_CMD_NAND_BOOT:  /* ����nandֱ������ */
            print_info("\r\nNF_boot!" );
            iRet = nandReadBl(M3_TCM_BL_ADDR, NAND_PARSE_ID_MODE, &(pShareData->nandspec));
            break;

#if EMMC_CONFIG == YES
        case AP_CMD_EMMC_BOOT:  /* ����MMC�豸ֱ������ */
            print_info("\r\nEMMC_boot!" );
            iRet = emmc_read_bootloader(M3_TCM_BL_ADDR);
            break;
#endif

        case AP_CMD_DL_BL_BURN_PRE: /* �ȴ������ļ�,����ֱ�ӷ��سɹ� */
            (void)sendRepPkt(pstDlCtrl, REP_ACK);;
            return OK;

        default:    /* ��֧�ֵ����� */
            (void)sendRepPkt(pstDlCtrl, REP_AP_CMD_NOT_SUPPORT);
            return ERROR;
            /* break; */

    }

    /* ��������豸����,��ʼУ��;���������Ѿ�ֱ�ӷ�����,�˴����������� */
    if(OK != iRet)
    {
        /* ���Ͷ�ȡʧ��,�ȴ�AP��λ,���ٷ��� */
        sendRepErrWaitReset(pstDlCtrl, REP_NAND_READ_ERR);
    }

    iRet = apSecChk(M3_TCM_BL_ADDR);

    if(SEC_SUCCESS == iRet
        ||SEC_EFUSE_NOT_WRITE == iRet)
    {
        ocrShareSave();

        /* У��ͨ����δʹ�ܰ�ȫУ��,֪ͨAP����,׼�����о��� */
        (void)sendRepPkt(pstDlCtrl, REP_ACK);
        /* ���ٷ��� */
        /*��ת�ĵ�ַ��Ҫ����*/
        BOOT_RST_Addr = *(volatile UINT32 *)BOOT_RST_ADDR_PP;
        go((FUNCPTR)(M3_TCM_BL_ADDR + BOOT_RST_Addr));
    }
    else
    {
        /* ����У��ʧ��,�ȴ�AP��λ,���ٷ��� */
        sendRepErrWaitReset(pstDlCtrl, REP_SEC_CHK_ERR);
    }

    return OK;   /* for compiler happy */
}

/*****************************************************************************
* �� �� ��  : handerStartDataPkt
*
* ��������  : ����ʼ�����ļ���
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
INT32 handerStartDataPkt( apDlCtrlS *pstDlCtrl )
{
    UINT32 ulFileLength;
    UINT8 *pu8Buf = pstDlCtrl->pstHdlcCtrl->au8DecapBuf;

    ulFileLength = HSUART_MAKE_LONG(pu8Buf[START_DATA_FILELEN_OS], \
                            pu8Buf[START_DATA_FILELEN_OS+1],\
                            pu8Buf[START_DATA_FILELEN_OS+2],\
                            pu8Buf[START_DATA_FILELEN_OS+3]);

    if( ulFileLength >= BOOTLOAD_SIZE_MAX)
    {
        print_info("\r\nlen wrong" );
        (void)sendRepPkt(pstDlCtrl, REP_FILE_TOOBIG);
        return ERROR;
    }

    pstDlCtrl->ulFileLength = ulFileLength;

    pstDlCtrl->ulAddress = HSUART_MAKE_LONG(pu8Buf[START_DATA_ADDR_OS], \
                            pu8Buf[START_DATA_ADDR_OS+1],\
                            pu8Buf[START_DATA_ADDR_OS+2],\
                            pu8Buf[START_DATA_ADDR_OS+3]);

    (void)sendRepPkt(pstDlCtrl, REP_ACK);

    /* ����Ŵ�0��ʼ */
    pstDlCtrl->u8SeqNext = 0;
    pstDlCtrl->ulPosition = pstDlCtrl->ulAddress;

    return OK;
}

/*****************************************************************************
* �� �� ��  : handerMidstDataPkt
*
* ��������  : �����������ļ���
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
INT32 handerMidstDataPkt( apDlCtrlS *pstDlCtrl )
{
    UINT32 ulPktLen;
    UINT8 *pu8Buf = pstDlCtrl->pstHdlcCtrl->au8DecapBuf;

    /* ֡��ż�� */
    if(pstDlCtrl->u8SeqNext != pu8Buf[MIDST_DATA_SEQ_OS])
    {
        /* ������ط�����һDATA֡������ */
        if (pstDlCtrl->u8SeqNext == pu8Buf[MIDST_DATA_SEQ_OS] + 1)
        {
//print_info_with_u32("\r\nrecv a dup pkt, seq:", pstDlCtrl->u8SeqNext);
            (void)sendRepPkt(pstDlCtrl, REP_ACK);
            return OK;
        }
        else    /* ����,����AP�ӷ����еİ�,��֮ǰ��Ҳ���� */
        {
            (void)sendRepPkt(pstDlCtrl, REP_SEQ_BIG);   /* AP�յ�REP_SEQ_BIG��Ӧ��,AP��Ҫ�ش������ļ� */
            pstDlCtrl->u8SeqNext = 0;
            pstDlCtrl->ulPosition = pstDlCtrl->ulAddress;
            return ERROR;
        }
    }

    ulPktLen = HSUART_MAKE_WORD(pu8Buf[PKT_LEN_OS], pu8Buf[PKT_LEN_OS+1]);

    memcpy((void*)pstDlCtrl->ulPosition, (void*)&pu8Buf[MIDST_DATA_OS], ulPktLen);

    /* ����ָ�� */
    pstDlCtrl->ulPosition += ulPktLen;
    pstDlCtrl->u8SeqNext++;

//print_info_with_u32("\r\nseq:", pu8Buf[MIDST_DATA_SEQ_OS]);
//print_info_with_u32("\r\npktLen:", ulPktLen);
//print_info_with_u32("\r\npktPos:", pstDlCtrl->ulPosition);
    /* ���ͽ��ճɹ���Ϣ */
    (void)sendRepPkt(pstDlCtrl, REP_ACK);

    return OK;
}

/*****************************************************************************
* �� �� ��  : handerEndDataPkt
*
* ��������  : ����������ݰ�,���������Ƿ���������
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
INT32 handerEndDataPkt( apDlCtrlS *pstDlCtrl )
{
    /* ���ز�����,���ش��� */
    if(pstDlCtrl->ulPosition != pstDlCtrl->ulAddress + pstDlCtrl->ulFileLength)
    {
        (void)sendRepPkt(pstDlCtrl, REP_FILE_RECV_ERR);
        return ERROR;
    }

    /* ��������,����ACK */
    pstDlCtrl->bComplete = TRUE;
    (void)sendRepPkt(pstDlCtrl, REP_ACK);
    return OK;
}

/*****************************************************************************
* �� �� ��  : handerExecDataPkt
*
* ��������  : ����ִ�����ݰ�
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
INT32 handerExecDataPkt( apDlCtrlS *pstDlCtrl )
{
    INT32 iRet;
    int BOOT_RST_Addr;

    if(!pstDlCtrl->bComplete)
    {
        (void)sendRepPkt(pstDlCtrl, REP_FILE_RECV_ERR);
        return ERROR;
    }

    iRet = apSecChk(pstDlCtrl->ulAddress);

    if(SEC_SUCCESS == iRet
        ||SEC_EFUSE_NOT_WRITE == iRet)
    {

#ifdef START_TIME_TEST
    print_info_with_u32("\r\ntime(ms):", (TIMER5_INIT_VALUE - INREG32(TIMER5_REGOFF_VALUE))/MS_TICKS);
#endif
        ocrShareSave();
        (void)sendRepPkt(pstDlCtrl, REP_ACK);
        /* ���ٷ��� */
        BOOT_RST_Addr = *(volatile UINT32 *)(pstDlCtrl->ulAddress + BOOT_RST_ADDR_OFFEST);
        go((FUNCPTR)(pstDlCtrl->ulAddress + BOOT_RST_Addr));
    }
    else
    {
        /* ����У��ʧ��,�ȴ�AP��λ,���ٷ��� */
        sendRepErrWaitReset(pstDlCtrl, REP_SEC_CHK_ERR);
    }

    return ERROR;
}


/*****************************************************************************
* �� �� ��  : apDlMain
*
* ��������  : AP������ں���
*
* �������  : ulFileAddr �洢�ļ��ĵ�ַ
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
apDlStatusE apDlMain( UINT32 ulFileAddr )
{
    INT32 iRet;
    UINT8 ucPktType;

    apDlCtrlS       stApDlCtrl;
    RECV_STR        stHsRecvStr;
    DLOAD_HDLC_S    stHdlcCtrl;

    apDlCtrlS *pstApDlCtrl      = &stApDlCtrl;
    pstApDlCtrl->pstHdlcCtrl    = &stHdlcCtrl;
    pstApDlCtrl->pstHsUartBuf   = &stHsRecvStr;

    apDlInit(pstApDlCtrl);

    /* OnChipRom V7R1 ͨ��HSUART���ص�BootLoader����д����������ڹ̶�λ������ */
    pstApDlCtrl->ulAddress = ulFileAddr;

    while(1)    /*lint !e716*/
    {
        iRet = apDlRecvPkt(pstApDlCtrl);
        switch(iRet)
        {
            case AP_DL_BUF_FULL:
                /* never return */
                sendRepErrWaitReset(pstApDlCtrl, REP_FILE_RECV_BUF_FULL);
                break;

            case AP_DL_RECV_TIMEOUT:
                /* never return */
                (void)sendRepPkt(pstApDlCtrl, REP_FILE_RECV_TIMEOUT);
                break;

            case AP_DL_ERR_OK:
                break;

            default:
                continue;
        }

        ucPktType = pstApDlCtrl->pstHdlcCtrl->au8DecapBuf[PKT_TYPE_OS];

        //print_info_with_u32("\r\nrecv a pkt, type:", ucPktType);

        switch(ucPktType)
        {
            /* ���ְ�,����Ack */
            case PKT_TYPE_CONNECT:
                (void)sendRepPkt(pstApDlCtrl, REP_ACK);
                iRet = OK;

                break;

            case PKT_TYPE_BAUD_SET:
            {
                iRet = handerBaudSetPkt(pstApDlCtrl);

                break;
            }

            /* ִ��ָ��,����Ǵ�Flashֱ����������,���ٷ��� */
            case PKT_TYPE_AP_CMD:
            {
                iRet = handerApCmdPkt(pstApDlCtrl);

                break;
            }

            case PKT_TYPE_STAT_DATA:
            {
                iRet = handerStartDataPkt(pstApDlCtrl);

                break;
            }

            case PKT_TYPE_MIDST_DATA:
                iRet = handerMidstDataPkt(pstApDlCtrl);

                break;

            case PKT_TYPE_END_DATA:
                iRet = handerEndDataPkt(pstApDlCtrl);

                break;

                /* ִ������,���ٷ��� */
            case PKT_TYPE_EXEC_DATA:
                iRet = handerExecDataPkt(pstApDlCtrl);

                break;

            default:
                /* �����ʹ���,����Nack */
                break;

        }

        if(OK != iRet)
        {
            print_info_with_u32("\r\nhanderPkt err: ", (UINT32)iRet);
            print_info_with_u32(",PktType:", (UINT32)ucPktType);
        }
    }

}

