

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "med_drv_uart.h"
#include "ucom_comm.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_VOID DRV_UART_Init(VOS_VOID)
{
    /*�üĴ���ֻд��ʹ��FIFO,����Rxˮ��Ϊ1/4 FIFO��Txˮ��Ϊ1/4 FIFO,���FIFO*/
    UCOM_RegWr(DRV_UART_IIR_FCR, 0x67);

    /*�������ݳ���Ϊ8bit, bit0/bit1, 2b'11*/
    UCOM_RegBitWr(DRV_UART_LCR, UCOM_BIT0, UCOM_BIT1, 0x3);

    /*����STOP����Ϊ1, bit2, 1b'0*/
    UCOM_RegBitWr(DRV_UART_LCR, UCOM_BIT2, UCOM_BIT2, 0x1);

    /*����У�鹦�ܹر�, bit3/bit4,1b'0*/
    UCOM_RegBitWr(DRV_UART_LCR, UCOM_BIT3, UCOM_BIT4, 0x0);

    /*�򿪷�Ƶ���üĴ���DLL DLH��ʹ��(���DLL DLH�����ú�,�˱�������0), bit7,1b'1*/
    UCOM_RegBitWr(DRV_UART_LCR, UCOM_BIT7, UCOM_BIT7, 0x1);

    UCOM_RegBitWr(DRV_UART_RBR_THR_DLL, UCOM_BIT0, UCOM_BIT7, 0xA);

    UCOM_RegBitWr(DRV_UART_DLH_IER, UCOM_BIT0, UCOM_BIT7, 0x0);

    /*���DLL DLH�����ú�,��0 LCR bit7,ʹ�ܼĴ�����д*/
    UCOM_RegBitWr(DRV_UART_LCR, UCOM_BIT7, UCOM_BIT7, 0x0);

    /*��ֹ�ж�*/
    UCOM_RegBitWr(DRV_UART_DLH_IER, UCOM_BIT0, UCOM_BIT7, 0x0);
}
VOS_VOID DRV_UART_SendData( VOS_UCHAR *pucData, VOS_UINT32 uwDataLen )
{
    VOS_UINT32                          i;
    VOS_UINT32                          uwThreValue;
    VOS_UINT32                          uwFifoNotFullFlag;
    VOS_UINT32                          uwCnt;

    if (VOS_NULL_PTR == pucData)
    {
        return;
    }

    /*��byte����д�뷢�����ݼĴ���*/
    for (i = 0; i < uwDataLen; i++)
    {
        uwCnt = DRV_UART_WAIT_CNT;

        do
        {
            uwCnt--;

            /*��ȡTHRE��־��0��ʾ�ǿգ�1��ʾ��*/
            uwThreValue         = UCOM_RegBitRd(DRV_UART_LSR, UCOM_BIT5, UCOM_BIT5);

            /*��ȡTFNF��־��0��ʾFIFO����1��ʾFIFO����*/
            uwFifoNotFullFlag   = UCOM_RegBitRd(DRV_UART_USR, UCOM_BIT1, UCOM_BIT1);

        /*��THR�ǿ���FIFO��,���޷�����������,��ѭ�ȴ�*/
        }while ((0 == uwThreValue) && (0 == uwFifoNotFullFlag) && (uwCnt > 0));

        /*д��һ��byte���������ݼĴ���*/
        UCOM_RegWr(DRV_UART_RBR_THR_DLL, (VOS_UINT32)(*(pucData + i)));
    }
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

