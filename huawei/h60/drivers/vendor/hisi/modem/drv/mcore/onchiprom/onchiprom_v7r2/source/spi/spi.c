/*************************************************************************
*   ��Ȩ����(C) 1987-2020, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  spi.c
*
*   ��    �� :  wuzechun
*
*   ��    �� :  SPI��ȡ����
*
*   �޸ļ�¼ :  2011��6��9��  v1.00  wuzechun  ����
*
*************************************************************************/

#include "sys.h"
#include "OnChipRom.h"
#include "spi.h"
#include "ioinit.h"

const UINT32 spiBase[2] = {ECS_SPI0_BASE, ECS_SPI1_BASE};
/*****************************************************************************
* �� �� ��  : spiInit
*
* ��������  : ��ʼ��SPI����
*
* �������  : spiNo SPI��������
*
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
INT32 spiInit(UINT32 spiNo)
{
    if ((0 != spiNo) && (1 != spiNo))
    {
        return ERROR;
    }

    SPI_IOS_SYS_CONFIG();

    /* ��ֹSPI Slave*/
    OUTREG32(SPI_SLAVE_EN(spiNo), 0);

    /* ��ֹSPI Master*/
    OUTREG32(SPI_EN(spiNo), 0);

    /* ����ctrl0�Ĵ����������ֳ���Ϊ1�����ݿ��Ϊ8,�����ش���,�͵�ƽ��Ч */
    OUTREG32(SPI_CTRL0(spiNo), (0x07<<SPI_CTRL0_DFS_BITPOS)    \
            | (1<<SPI_CTRL0_SCPH_BITPOS)    \
            | (1<<SPI_CTRL0_SCPOL_BITPOS)   \
            | (0 << SPI_CTRL0_FRF_BITPOS)   \
            | (0x00<<SPI_CFS_BITPOS));

    /* ����SPI������ΪSSI CLK��1/24����48/24=2MHz */
    OUTREG32(SPI_BAUD(spiNo), 24);

    /*��ֹ�����ж��ź�*/
    OUTREG32(SPI_IMR(spiNo), 0);

    /*����DMA����*/
    OUTREG32(SPI_DMAC(spiNo), 0);

    return OK;
}

/*****************************************************************************
* �� �� ��  : spiRecv
*
* ��������  : SPI������ѯ��������
*
* �������  : spiNo SPI��������
*                  cs:   Ƭѡ��
*                  prevData:   ָ��Ҫ��ȡ���ݵ�ַ��ָ��
*                  recvSize��Ҫ��ȡ�����ݵĴ�С
*                  psendData: ָ��Ҫ���͵�����͵�ַ��ָ��
*                  sendSize:  Ҫ��������͵�ַ�ĳ���(3 ����4)
* �������  :
*
* �� �� ֵ  : OK or ERROR
*
* ����˵��  :
*
*****************************************************************************/
INT32 spiRecv (UINT32 spiNo, UINT32 cs, UINT8* prevData, UINT32 recvSize,UINT8* psendData,UINT32 sendSize )
{
    UINT8 *pRh;
    UINT8 *pSh;
    UINT32  i;
    UINT32 ulLoop = SPI_MAX_DELAY_TIMES;

    if (((0 != spiNo) && (1 != spiNo))
        || (SPI_CS_PMU != cs && SPI_CS_DEV != cs)
        || (NULL == psendData) || (NULL == prevData) || (0 == recvSize) || (0 == sendSize) ||(((0x1<<16) < recvSize)) )
    {
        return ERROR;
    }

    pRh = prevData;
    pSh = psendData;

    /* ��ֹSPI Slave*/
    OUTREG32(SPI_SLAVE_EN(spiNo), 0);

    /* ��ֹSPI Master*/
    OUTREG32(SPI_EN(spiNo), 0);

    /* ���ó�EEPROM��ģʽ */
    SETBITVALUE32(SPI_CTRL0(spiNo), SPI_CTRL0_TMOD_BITMASK, SPI_CTRL0_TMOD_EEPROM_READ);

    /* ���ý������ݵ���Ŀ*/
    OUTREG32(SPI_CTRL1(spiNo),(recvSize-1));

    //print_info_with_u32("\r\nctrl1:", INREG32(SPI_CTRL1));

    /*ʹ��SPI Master*/
    OUTREG32(SPI_EN(spiNo), 1);

    /*ʹ��SPI Slave*/
    OUTREG32(SPI_SLAVE_EN(spiNo), cs);

    /* �������� */
    for(i = 0; i < sendSize; i++)
    {
        /* �ȴ�����FIFO���� */
        while(!(INREG32(SPI_STATUS(spiNo)) & SPI_STATUS_TXNOTFULL)
            && (0 != --ulLoop))
        {
        }

        if(0 == ulLoop)
        {
            return ERROR;
        }

        OUTREG32(SPI_DR(spiNo), *pSh++);
    }

    /*������FIFO�е�����ȫ������*/
    while(!(INREG32(SPI_STATUS(spiNo)) & SPI_STATUS_TXEMPTY)
        && (0 != --ulLoop))
    {
    }

    if(0 == ulLoop)
    {
        return ERROR;
    }

    /* �������� */
    for(i = 0; i < recvSize; i++)
    {
        ulLoop = SPI_MAX_DELAY_TIMES;
        /* �ȴ���ȡ������ */
        while(!(INREG32(SPI_STATUS(spiNo)) & SPI_STATUS_RXNOTEMPTY)
            && (0 != --ulLoop))
        {
        }

        if(0 == ulLoop)
        {
            return ERROR;
        }

        *pRh++ = (UINT8)INREG32(SPI_DR(spiNo));
    }

#if 0 /* SPI���������Զ���ʹ��,��������������� */
    /* ��ֹSPI Slave*/
    OUTREG32(SPI_SLAVE_EN(spiNo), 0);

    /* ��ֹSPI Master*/
    OUTREG32(SPI_EN(spiNo), 0);
#endif
    return OK;
}

/*****************************************************************************
* �� �� ��  : spiSend
*
* ��������  : SPI������ѯ����
*
* �������  : spiNo SPI��������
*                  cs:   Ƭѡ��
*                  pData:   ָ��Ҫ�������ݵ�ַ��ָ��
*                  ulLen��Ҫ���͵����ݵĴ�С
* �������  :
*
* �� �� ֵ  : OK or ERROR
*
* ����˵��  :
*
*****************************************************************************/
INT32 spiSend (UINT32 spiNo, UINT32 cs, UINT8* pData, UINT32 ulLen)
{
    UINT8 *pSh;
    UINT32  i;
    UINT32 ulLoop = SPI_MAX_DELAY_TIMES;
    UINT32 ulVal;

    if (((0 != spiNo) && (1 != spiNo))
        || (SPI_CS_PMU != cs && SPI_CS_DEV != cs)
        || (NULL == pData) || (0 == ulLen))
    {
        return ERROR;
    }

    pSh = (UINT8*)pData;

    /* ��ֹSPI Slave*/
    OUTREG32(SPI_SLAVE_EN(spiNo), 0);

    /* ��ֹSPI Master*/
    OUTREG32(SPI_EN(spiNo), 0);

    /* ���óɷ���ģʽ */
    SETBITVALUE32(SPI_CTRL0(spiNo), SPI_CTRL0_TMOD_BITMASK, SPI_CTRL0_TMOD_SEND);

    /*ʹ��SPI Master*/
    OUTREG32(SPI_EN(spiNo), 1);

    /*ʹ��SPI Slave*/
    OUTREG32(SPI_SLAVE_EN(spiNo), cs);

    /* �������� */
    for(i = 0; i < ulLen; i++)
    {
        /* �ȴ�����FIFO���� */
        while(!(INREG32(SPI_STATUS(spiNo)) & SPI_STATUS_TXNOTFULL)
            && (0 != --ulLoop))
        {
        }

        if(0 == ulLoop)
        {
            return ERROR;
        }
        OUTREG32(SPI_DR(spiNo), *pSh++);
    }

    /*������FIFO�е�����ȫ������,�Ҳ�BUSY*/
    ulLoop = SPI_MAX_DELAY_TIMES;
    ulVal = INREG32(SPI_STATUS(spiNo));
    while(((!(ulVal & SPI_STATUS_TXEMPTY)) || (ulVal & SPI_STATUS_BUSY))
        && (0 != --ulLoop))
    {
        ulVal = INREG32(SPI_STATUS(spiNo));
    }

    if(0 == ulLoop)
    {
        return ERROR;
    }

    return OK;
}

