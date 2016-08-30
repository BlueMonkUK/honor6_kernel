/*************************************************************************
*   ��Ȩ����(C) 1987-2020, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  sys.c
*
*   ��    �� :  wuzechun
*
*   ��    �� :  ϵͳ���ú���
*
*   �޸ļ�¼ :  2011��6��29��  v1.00  wuzechun  ����
*
*************************************************************************/

#include "OnChipRom.h"
#include "OcrShare.h"
#include "sys.h"

/*****************************************************************************
* �� �� ��  : delay
*
* ��������  : ʹ��Timer6��ʱ����׼΢����ʱ,����֧��223,696,213us
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
void delay(UINT32 us)
{
    /* ��ֹС��������/��� */
    volatile UINT32 ulDelayClks = (TIMER6_FREQ * 10 / US_PER_SECEND) * us / 10;

    /* disable */
    OUTREG32(TIMER6_REGOFF_CTRL, TIMER_CTRL_INTMASK);

    /* ���ó�ʱʱ�� */
    OUTREG32(TIMER6_REGOFF_LOAD, ulDelayClks);

    /* �ȴ�������� */
    while(TIMER_CTRL_EN_ACK != (INREG32(TIMER6_REGOFF_CTRL)&TIMER_CTRL_EN_ACK));

    /* enable�ж�,��ʼ��ʱ */
    OUTREG32(TIMER6_REGOFF_CTRL, 1);

    /* �ȴ���ʱ */
    while(TIMER_INS_INT != (TIMER_INS_INT & (INREG32(TIMER6_REGOFF_INTSTATUS))));

    /* disable */
    OUTREG32(TIMER6_REGOFF_CTRL, TIMER_CTRL_INTMASK);
}

/*****************************************************************************
* �� �� ��  : print_info
*
* ��������  : �ַ�����ӡ��������Ҫ��\r\n
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
void print_info(const void *pucBuffer)
{
#if PLATFORM!=PLATFORM_V7R2_EDA
    UINT32 ulStrLen = 0x00;
    UINT8 *pucTmp = (UINT8 *)pucBuffer;
    UINT32 u32Loop;

    while('\0' != *pucTmp++ )
    {
        ulStrLen++;
    }
    pucTmp = (UINT8 *)pucBuffer;
    while(ulStrLen--)
    {
        /*check tx fifo is empty*/
        u32Loop = 10000;
        while(0x00 == ((*(volatile UINT32 *)(UART0_BASE+UART_USR)) & 0x04))
        {
            /* ��ʱֱ�ӷ���,����û�и��õķ��� */
            if(0 == --u32Loop)
            {
                return;
            }
        }
        *(UINT32 *)(UART0_BASE + UART_THR) = *pucTmp;
        pucTmp++;
    }
#endif
}

/*****************************************************************************
* �� �� ��  : print_info_with_u32
*
* ��������  : print information and a value
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
void print_info_with_u32( const void *msg, UINT32 ulValue )
{
#if PLATFORM!=PLATFORM_V7R2_EDA

    static const unsigned char hex_value[] = "0123456789ABCDEF";
    UINT8 curr_msg [ 16 ];
    UINT8 *p;
    print_info( msg );
    p = curr_msg;
    *p++ = hex_value[ (ulValue >> 28) & 0x0F ];
    *p++ = hex_value[ (ulValue >> 24) & 0x0F ];
    *p++ = hex_value[ (ulValue >> 20) & 0x0F ];
    *p++ = hex_value[ (ulValue >> 16) & 0x0F ];
    *p++ = hex_value[ (ulValue >> 12) & 0x0F ];
    *p++ = hex_value[ (ulValue >>   8) & 0x0F ];
    *p++ = hex_value[ (ulValue >>   4) & 0x0F ];
    *p++ = hex_value[ (ulValue >>   0) & 0x0F ];
    *p++ = 0;

    print_info( curr_msg );
#endif
}

/*****************************************************************************
* �� �� ��  : waitBitSetWithTimeout
*
* ��������  : ��ʱ�ȴ��Ĵ���ĳλ�����ó�������ֵ
*
* �������  :
* �������  :
*
* �� �� ֵ  : OK �ɹ��ȵ�λ������
*                ERROR ��ʱδ�ȵ�λ������
*
* ����˵��  :
*
*****************************************************************************/
INT32 waitBitSetWithTimeout(UINT32 reg, UINT32 bitmask, UINT32 value, UINT32 timeout)
{
    volatile UINT32 ulLoop = timeout;
//print_info_with_u32("ulLoop:", ulLoop);
    while(value != (INREG32(reg) & bitmask)
          && (0 != --ulLoop));

//print_info_with_u32("ulLoop:", ulLoop);
    return ((0 == ulLoop) ? ERROR : OK);
}

/*****************************************************************************
* �� �� ��  : wdtReboot
*
* ��������  : ʹ�ÿ��Ź���λ
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
void wdtReboot( void )
{
    /*����ϵͳ������,���ÿ��Ź�����ʱ��ʹ��*/
    OUTREG32(SC_WDTCTRL, 0x69200009);

    /* �Ĵ������� */
    OUTREG32(WDG_LOCK, WDT_LOCK_VALUE);

    /*���ÿ��Ź���������ֵ*/
    OUTREG32(WDG_LOAD, 1000);

    /* ʹ�ܸ�λ���ж� */
    OUTREG32(WDG_CONTROL, 0x3);

    /* �Ĵ������� */
    OUTREG32(WDG_LOCK, 0);

    /* whileѭ���ȴ����Ź���ʱ��λ */
    while(1);   /*lint !e716*/

}

/*****************************************************************************
* �� �� ��  : wdtRebootDelayMs
*
* ��������  : ��ʱ��λ
*
* �������  : us,��ʱ������
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
void wdtRebootDelayMs( UINT32 ms )
{
    delay(ms * 1000);

    wdtReboot();
}

/*****************************************************************************
* �� �� ��  : timer0_get_current_tick
*
* ��������  : ��ȡ��ǰtick
*
* �������  : VOID
* �������  : ��
*
* �� �� ֵ  : ��ǰtickֵ
*
* ����˵��  : ��
*
*****************************************************************************/
UINT32 timer0_get_current_tick(VOID)
{
    volatile UINT32 ulTick;

    ulTick = INREG32(TIMER0_REGOFF_VALUE);

    return ulTick;
}

/*****************************************************************************
* �� �� ��  : setErrno
*
* ��������  : ��AXI Mem�����ô�����
*
* �������  : errno,������
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
void setErrno( INT32 errno )
{
    volatile tOcrShareData *pShareData = (tOcrShareData*)M3_TCM_SHARE_DATA_ADDR;
    pShareData->errno = errno;
}

