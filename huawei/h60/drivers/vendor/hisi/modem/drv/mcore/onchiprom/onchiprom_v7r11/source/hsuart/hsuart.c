/*************************************************************************
*   ��Ȩ����(C) 1987-2020, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  hsuart.c
*
*   ��    �� :  wuzechun
*
*   ��    �� :  ���ٴ�����������Ҫ��ɸ��ٴ��ڵĳ�ʼ�����ա���������
*
*   �޸ļ�¼ :  2011��6��13��  v1.00  wuzechun  ����
*
*************************************************************************/

#include "types.h"
#include "sys.h"
#include "OnChipRom.h"
#include "string.h"
#include "hsuart.h"
#include "secBoot.h"
#include "ioinit.h"


/*****************************************************************************
* �� �� ��  : IsQueNotFull
*
* ��������  : ���ٴ��ڽ��ջ���������δ��
*
* �������  : pstQue ����������ָ��
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
int IsQueNotFull(RECV_STR *pstQue)
{
    if(NULL == pstQue)
    {
        return ERROR;
    }

    if(pstQue->ulTotalNum < HSUART_BUFFER_SIZE)
    {
        return OK;
    }
    else
    {
        return ERROR;
    }
}

/*****************************************************************************
* �� �� ��  : InQue
*
* ��������  : ���ٴ��ڽ��ջ�������Ӻ���
*
* �������  : pstQue ����������ָ��
*                 ucData ���յ����ַ�
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
int InQue(RECV_STR *pstQue, UINT8 ucData)
{
    UINT32 ulTail = 0;

    if(NULL == pstQue)
    {
        return ERROR;
    }

    if(ERROR == IsQueNotFull(pstQue))
    {
        return ERROR;
    }

    ulTail = pstQue->ulWrite;
    pstQue->ucData[ulTail] = ucData;
    ulTail = ((ulTail+1) ==  HSUART_BUFFER_SIZE) ? 0 : (ulTail+1);
    pstQue->ulTotalNum++;
    pstQue->ulWrite = ulTail;

    return OK;
}

/*****************************************************************************
* �� �� ��  : InQue
*
* ��������  : ���ٴ��ڽ��ջ��������Ӻ���
*
* �������  : pstQue ����������ָ��
*                 pBuf �ӽ��ջ�������ȡ�����ݴ��λ��
*                 ulLong ����ȡ�ĳ���
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
int OutQue(RECV_STR *pstQue, UINT8 *pBuf,UINT32 ulLong)
{
    UINT32 ulRead = 0;

    if((NULL == pstQue) || (NULL == pBuf) || (0 == ulLong))
    {
        return ERROR;
    }

    if (pstQue->ulTotalNum < ulLong)
    {
        return  ERROR;
    }

    ulRead = pstQue->ulRead;

    while(ulLong)
    {
        *pBuf = pstQue->ucData[ulRead];
        ulRead = ((ulRead+1) == HSUART_BUFFER_SIZE)?0:(ulRead+1);
        pstQue->ulTotalNum--;
        ulLong--;
        pBuf++;
    }

    //print_info_with_u32("\r\n ulRead = ", ulRead);
    pstQue->ulRead = ulRead;

    return OK;
}



/*****************************************************************************
* �� �� ��  : hsUartInit
*
* ��������  : HS UART�Ļ��������ĳ�ʼ��
*
* �������  : ��
*
* �������  : ��
* �� �� ֵ  : HSUART_STATUS_OK ���óɹ�
*                HSUART_STATUS_BAUTRATE_ERR ���ò����ʳ�ʱʧ��
*
* �޸ļ�¼  :2010��12��16��   ³��  ����
*****************************************************************************/
HSUART_STATUS hsUartInit(RECV_STR *pstQue)
{
    UINT32 u32Divisor = 0;
    UINT32 u32DivisorLow = 0;
    UINT32 u32DivisorHigh = 0;
    UINT32 u32Discard = 0;
    UINT32 u32Fcr = 0;
    UINT32 u32Times = 0;

    if(NULL == pstQue)
    {
        return HSUART_STATUS_PARA_ERR;
    }

    /* �ܽŸ���Ϊ���ٴ��� */
    HSUART_IOS_SYS_CONFIG();

    /* �����Ƶ�� */
    u32Divisor = HSUART_CLOCK_FREQ / (16 * HSUART_BAUT_RATE_DEFAULT);
    u32DivisorHigh = (u32Divisor&0xFF00) >> 8;
    u32DivisorLow = u32Divisor&0xFF;

    /*дFCR���ѽ���FIFO �ͷ���FIFO ȫ����λ���,����FIFOʹ�ܣ���DMA Model */
    u32Fcr = UART_DEF_RT_ONEFOUR|UART_DEF_TET_NULL|UART_DEF_NO_DMA_MODE|UART_RESET_TX_FIFO|UART_RESET_RCV_FIFO|UART_DEF_FIFOE_ENABLE;

    OUTREG32(HSUART_BASE_ADDR + UART_FCR, u32Fcr);

    /*ȥʹ�������ж�*/
    OUTREG32(HSUART_BASE_ADDR + UART_IER, 0x00);

    /*clear line status interrupt*/
    u32Discard = INREG32(HSUART_BASE_ADDR + UART_LSR);
    /*clear rcv interrupt*/
    u32Discard = INREG32(HSUART_BASE_ADDR + UART_RBR);
    /*clear send empty interrupt*/
    u32Discard = INREG32(HSUART_BASE_ADDR + UART_IIR);
    /*clear line busy interrupt*/
    u32Discard = INREG32(HSUART_BASE_ADDR + UART_USR);

    /* ��ȡUART ״̬�Ĵ�������UART�Ƿ���idle״̬*/
    u32Discard = INREG32(HSUART_BASE_ADDR + UART_USR);
    while((u32Discard & 0x01)&&(u32Times<1000))/*uart is busy*/
    {
    	u32Discard = INREG32(HSUART_BASE_ADDR + UART_USR);
        u32Times++;
    }
    if(1000 == u32Times)
    {
        return HSUART_STATUS_BAUTRATE_ERR;
    }

    /* USR bit[0]=0 ��UART ����idle ̬ʱ�����ܶ�LCR bit[7]��λ����д����*/
    OUTREG32(HSUART_BASE_ADDR + UART_LCR, UART_DEF_LCR_DIVENABLE);

    /* ����DLH ��DLL �Ĵ��������÷�Ƶ��*/
    OUTREG32(HSUART_BASE_ADDR +  UART_DLL, u32DivisorLow);
    OUTREG32(HSUART_BASE_ADDR +  UART_DLH, u32DivisorHigh);

    /* ������Χ���ͺͽ������ݵ���ĿΪ8bit,1ֹͣλ,��У��λ,disable DLL&DLH */
    OUTREG32(HSUART_BASE_ADDR +  UART_LCR, UART_DEF_PARITY_NONE|UART_DEF_LCR_STP1|UART_DEF_LCR_CS8);

    /* ����MCR ,rts,������*/
    OUTREG32(HSUART_BASE_ADDR +  UART_MCR, 0x22);

    /* ���÷���FIFO */
    OUTREG32(HSUART_BASE_ADDR +  UART_TXDEPTH, 10);

    /* ʹ�ý����ж� */
    OUTREG32(HSUART_BASE_ADDR +  UART_IER, 0x11);

    pstQue->ulRead = 0;
    pstQue->ulWrite = 0;
    pstQue->ulTotalNum = 0;

    /* ��ʼ����ʱ��ʱ�� */
    TIMER0_INIT();

    return HSUART_STATUS_OK;
}

void send4(UINT32 * pucSrcAddr ,UINT32 ulTimes)
{
    UINT32 regval;
    UINT32 * pu32Buffer;

    pu32Buffer = pucSrcAddr;

    while(ulTimes)
    {
        regval = INREG32(HSUART_BASE_ADDR + UART_USR);
        if (0 != (regval & 0x02))
        {
            /* ���������е����ݷŵ����ݼĴ�����*/
            *(UINT32 *)(HSUART_BASE_ADDR + UART_THR) = *pu32Buffer;
            pu32Buffer++;
        	ulTimes--;
        }
        else
        {
            //print_info("Tx FIFO full\r\n");
        }
    }
    return;
}

void send2(UINT16 * pucSrcAddr ,UINT32 ulTimes)
{
    UINT32 regval;
    UINT16 * pu16Buffer;

    pu16Buffer = pucSrcAddr;

    while(ulTimes)
    {
        regval = INREG32(HSUART_BASE_ADDR + UART_USR);
        if (0 != (regval & 0x02))
        {
            /* ���������е����ݷŵ����ݼĴ�����*/
            *(UINT16 *)(HSUART_BASE_ADDR + UART_THR) = *pu16Buffer;
            pu16Buffer++;
        	ulTimes--;
        }
        else
        {
            //print_info("Tx FIFO full\r\n");
        }
    }
    return;
}

void send1(UINT8 * pucSrcAddr ,UINT32 ulTimes)
{
    UINT32 regval;
    UINT8 * pu8Buffer;

    pu8Buffer = pucSrcAddr;

    while(ulTimes)
    {
        regval = INREG32(HSUART_BASE_ADDR + UART_USR);
        if (0 != (regval & 0x02))
        {
            /* ���������е����ݷŵ����ݼĴ�����*/
            *(UINT8 *)(HSUART_BASE_ADDR + UART_THR) = *pu8Buffer;
            pu8Buffer++;
        	ulTimes--;
        }
        else
        {
            //print_info("Tx FIFO full\r\n");
        }
    }
    return;
}

/*****************************************************************************
* �� �� ��  : hsUartSend
*
* ��������  : HS UART�������ݽӿں���
*
* �������  : UINT32  u32SrcAddr       �跢�͵����ݵ��׵�ַ
*             UINT32  u32TransLength   �跢�͵����ݵĳ���
*
* �������  : ��
* �� �� ֵ  : OK       �ɹ�
*             ERROR    ʧ��
*
* �޸ļ�¼  :2010��12��16��   ³��  ����
*****************************************************************************/
HSUART_STATUS hsUartSend(UINT8 * pucSrcAddr, UINT32 u32TransLength)
{
//    UINT32 regval;
    UINT8 * pu8Buffer;
    UINT32 ulTimes;
    UINT32 ulLeft;

    /* ��������Ч�Լ�� */
    if((NULL == pucSrcAddr) || (0 == u32TransLength))
    {
    	return HSUART_STATUS_PARA_ERR;
    }

    pu8Buffer = pucSrcAddr;

    ulTimes = u32TransLength / 4;
    ulLeft = u32TransLength % 4;

    send4((UINT32*)pu8Buffer, ulTimes); /*lint !e826*/
    pu8Buffer = pu8Buffer + ulTimes*4;

    if(ulLeft == 1)
    {
        send1(pu8Buffer, 1);
    }
    else if(ulLeft == 2)
    {
        send2((UINT16*)pu8Buffer, 1); /*lint !e826*/
    }
    else if(ulLeft == 3)
    {
        send2((UINT16*)pu8Buffer, 1); /*lint !e826*/
        pu8Buffer = pu8Buffer + 1*2;
        send1(pu8Buffer, 1);
    }

    return HSUART_STATUS_OK;
}

/*****************************************************************************
* �� �� ��  : HSUART_Recv
*
* ��������  : HS UART�������ݽӿں�������ȡ��Ч����ʱ��Ҫ
*                 4�ֽڶ��룬����������������Ч���ݲ�����,
*                 �´ζ�ȡ��ʧ�ܡ�
*
* �������  : UINT8 *  pu8DstAddr       ���յ������ݴ�ŵĵ��׵�ַ
*             UINT32  u32TransLength   ���յ����ݵĳ���
* �������  : ��
* �� �� ֵ  : OK       �ɹ�
*             ERROR    ʧ��
*
* �޸ļ�¼  :2010��12��16��   ³��  ����
*****************************************************************************/
HSUART_STATUS hsUartRecv(RECV_STR *pstHsUartRecvData)
{
    UINT32 ulCharNum;
    UINT32 ulData;
    UINT8* ucTemp;
    UINT32 i;
    UINT32 ulInt = 0;

    ulInt = INREG32(HSUART_BASE_ADDR + UART_IIR);
    ulInt &= 0xF;

    if(ulInt == 0xC)
    {
        ulCharNum = INREG32(HSUART_BASE_ADDR + UART_RFL);
        while(ulCharNum)
        {
            if(ulCharNum >= 4)
            {
                ulData = INREG32(HSUART_BASE_ADDR + UART_RBR);
                ucTemp = (UINT8*)&ulData;
                for(i=0;i<4;i++)
                {
                    if(InQue(pstHsUartRecvData, ucTemp[i]) != OK)
                    {
                        print_info("\r\nbuffer full\r\n");
                        return HSUART_STATUS_BUFFER_FULL_ERR;
                    }
                }
                ulCharNum = ulCharNum - 4;
            }
            else
            {
                ulData = INREG32(HSUART_BASE_ADDR + UART_RBR);
                ucTemp = (UINT8*)&ulData;
                for(i=0;i<ulCharNum;i++)
                {
                    if(InQue(pstHsUartRecvData, ucTemp[i]) != OK)
                    {
                        print_info("\r\nbuffer full\r\n");
                        return HSUART_STATUS_BUFFER_FULL_ERR;
                    }
                }
                return HSUART_STATUS_OK;
            }
            //ulCharNum = INREG32(HSUART_BASE_ADDR + UART_RFL);
        }

    }

    else if(ulInt == 0x4)
    {
        ulCharNum = INREG32(HSUART_BASE_ADDR + UART_USR);
        while(ulCharNum & 0x8)
        {
            ulData = INREG32(HSUART_BASE_ADDR + UART_RBR);
            ucTemp = (UINT8*)&ulData;
            for(i=0;i<4;i++)
            {
                if(InQue(pstHsUartRecvData, ucTemp[i]) != OK)
                {
                    print_info("\r\nbuffer full\r\n");
                    return HSUART_STATUS_BUFFER_FULL_ERR;
                }
            }
            ulCharNum = INREG32(HSUART_BASE_ADDR + UART_USR);
        }

    }

    return HSUART_STATUS_OK;
}


INT32 hsUartRecvData(RECV_STR *pstQue, UINT8 *pBuf, UINT32 ulLong)
{
    TIMER0_VAR_DECLAR;

    if((NULL == pstQue) || (NULL == pBuf) || (0 == ulLong))
    {
        return ERROR;
    }

    TIMER0_INIT_TIMEOUT(HS_UART_RECV_TIMEOUT_MS);

    while(pstQue->ulTotalNum < ulLong)
    {
        if(hsUartRecv(pstQue) != HSUART_STATUS_OK)
        {
            return ERROR;
        }
        if(TIMER0_IS_TIMEOUT())
        {
            return HSUART_RECV_TIMEOUT;
        }
    }
    if(OutQue(pstQue, pBuf, ulLong) != OK)
    {
        return ERROR;
    }

    return OK;
}

/*****************************************************************************
* �� �� ��  : isHsSptBaud
*
* ��������  : ����Ƿ���֧�ֵĲ�����
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
BOOL isHsSptBaud(UINT32 u32BaudRate)
{
    if(BAUDRATE_9600 == u32BaudRate
        || BAUDRATE_115200 == u32BaudRate
        || BAUDRATE_1M == u32BaudRate
        || BAUDRATE_2M == u32BaudRate
        || BAUDRATE_3M == u32BaudRate
        || BAUDRATE_4M == u32BaudRate)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************
* �� �� ��  : hsUartSetBaud
*
* ��������  : HS UART���ò����ʽӿں���
*
* �������  : UINT32  u32BaudRate      ��Ҫ���õĲ�����
*
* �������  : ��
* �� �� ֵ  : OK       �ɹ�
*             ERROR    ʧ��
*
* �޸ļ�¼  :2011��7��15��   ³��  ����
*****************************************************************************/
HSUART_STATUS hsUartSetBaud(UINT32 u32BaudRate)
{
    UINT32 u32Divisor = 0;
    UINT32 u32DivisorLow = 0;
    UINT32 u32DivisorHigh = 0;
    UINT32 u32Discard = 0;
    UINT32 u32Times = 0;

    /* �����Ƶ�� */
    switch(u32BaudRate)
    {
        case BAUDRATE_115200:
            u32Divisor = HSUART_CLOCK_FREQ / (16 * BAUDRATE_115200);
            break;

        case BAUDRATE_9600:
            u32Divisor = HSUART_CLOCK_FREQ / (16 * BAUDRATE_9600);
            break;

        case BAUDRATE_1M:
            u32Divisor = HSUART_CLOCK_FREQ / (16 * BAUDRATE_1M);
            break;

        case BAUDRATE_2M:
            u32Divisor = HSUART_CLOCK_FREQ / (16 * BAUDRATE_2M);
            break;

        case BAUDRATE_3M:
            u32Divisor = HSUART_CLOCK_FREQ / (16 * BAUDRATE_3M);
            break;

        case BAUDRATE_4M:
            u32Divisor = HSUART_CLOCK_FREQ / (16 * BAUDRATE_4M);
            break;

        default:
            print_info_with_u32("\r\nbaudrate error baud :",u32BaudRate);
            return HSUART_STATUS_BAUTRATE_ERR;
            break;  /*lint !e527*/

    }
#if 0   /* �����ߵ��÷�֧ */
    /* �ж��Ƿ�֧�ָ÷�Ƶ�� */
    if(u32Divisor == 0)
    {
        print_info("\r\not support baudrate\r\n");
        return HSUART_STATUS_BAUTRATE_ERR;
    }
#endif
    u32DivisorHigh = (u32Divisor&0xFF00) >> 8;
    u32DivisorLow = u32Divisor&0xFF;

    /* ��ȡUART ״̬�Ĵ�������UART�Ƿ���idle״̬*/
    u32Discard = INREG32(HSUART_BASE_ADDR + UART_USR);
    while((u32Discard & 0x01)&&(u32Times<HSUUART_DELAY_TIMES))/*uart is busy*/
    {
    	u32Discard = INREG32(HSUART_BASE_ADDR + UART_USR);
        u32Times++;
        delay(100);
    }
    if(HSUUART_DELAY_TIMES == u32Times)
    {
        print_info("\r\nhsuart busy\r\n");
        return HSUART_STATUS_BAUTRATE_ERR;
    }

    /* USR bit[0]=0 ��UART ����idle ̬ʱ�����ܶ�LCR bit[7]��λ����д����*/
    OUTREG32(HSUART_BASE_ADDR + UART_LCR, UART_DEF_LCR_DIVENABLE);

    /* ����DLH ��DLL �Ĵ��������÷�Ƶ��*/
    OUTREG32(HSUART_BASE_ADDR +  UART_DLL, u32DivisorLow);
    OUTREG32(HSUART_BASE_ADDR +  UART_DLH, u32DivisorHigh);

    /* ������Χ���ͺͽ������ݵ���ĿΪ8bit,1ֹͣλ,��У��λ,disable DLL&DLH */
    OUTREG32(HSUART_BASE_ADDR +  UART_LCR, UART_DEF_PARITY_NONE|UART_DEF_LCR_STP1|UART_DEF_LCR_CS8);

    return HSUART_STATUS_OK;
}


