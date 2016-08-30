
#include <osl_sem.h>
#include <bsp_ipc.h>
#include <spi_balong.h>

#if defined(__VXWORKS__)
#define SPI_SEM_FULL    (1)
#define SPI_SEM_EMPTY   (0)
osl_sem_id    spi0_lock;

#define spi0_core_lock()        \
    do {                            \
        osl_sem_down(&spi0_lock);       \
        (void)bsp_ipc_spin_lock(IPC_SEM_SPI0);   \
    } while (0)
#define spi0_core_unlock()      \
    do {                            \
        (void)bsp_ipc_spin_unlock(IPC_SEM_SPI0);   \
        osl_sem_up(&spi0_lock);     \
    } while (0)
#elif defined(__M3_OS__)
#define spi0_core_lock()        \
    do {                                \
        (void)bsp_ipc_spin_lock(IPC_SEM_SPI0);   \
    } while (0)
#define spi0_core_unlock()      \
    do {                            \
        (void)bsp_ipc_spin_unlock(IPC_SEM_SPI0);    \
    } while (0)

#endif

const u32 spiBase[2] = {ECS_SPI0_BASE, ECS_SPI1_BASE};

void balong_spi_init(void)
{
#if defined(__VXWORKS__)
    osl_sem_init(SPI_SEM_FULL, &spi0_lock);
#endif
}

/*****************************************************************************
* �� �� ��  : spiRecv
*
* ��������  : SPI������ѯ��������
*
* �������  : spiNo:     SPI��������
*                       cs:      Ƭѡ��
*                   prevData:   ָ��Ҫ��ȡ���ݵ�ַ��ָ��
*                   recvSize��Ҫ��ȡ�����ݵĴ�С
*                   psendData: ָ��Ҫ���͵�����͵�ַ��ָ��
*                   sendSize:   Ҫ��������͵�ַ�ĳ���(3 ����4)
* �������  :
*
* �� �� ֵ  : OK or ERROR
*
* ����˵��  :
*
*****************************************************************************/
s32 balong_spi_recv (u32 spiNo, u32 cs, u16* prevData, u32 recvSize,u16* psendData,u32 sendSize )
{
    u16 *pRh;
    u16 *pSh;
    u32  i;
    u32 ulLoop = SPI_MAX_DELAY_TIMES;

    if (((0 != spiNo) && (1 != spiNo))
        || (SPI_CS_PMU != cs && SPI_CS_DEV != cs)
        || (NULL == psendData) || (NULL == prevData) || (0 == recvSize) || (0 == sendSize))
    {
        return ERROR;
    }

    pRh = prevData;
    pSh = psendData;

    spi0_core_lock();
    /* ��ֹSPI Slave */
    writel(0,SPI_SLAVE_EN(spiNo));

    /* ��ֹSPI Master */
    writel(0, SPI_EN(spiNo));

    /* ���ó�EEPROM��ģʽ */
    writel(((readl(SPI_CTRL0(spiNo)) & ~SPI_CTRL0_TMOD_BITMASK) | SPI_CTRL0_TMOD_SEND_RECV),SPI_CTRL0(spiNo));

    /* ���ý������ݵ���Ŀ */
    writel((recvSize-1),SPI_CTRL1(spiNo));

    /* ʹ��SPI Master */
    writel(1, SPI_EN(spiNo));

    /* ʹ��SPI Slave */
    writel(cs, SPI_SLAVE_EN(spiNo));

    /* �������� */
    for(i = 0; i < sendSize; i++)
    {
        /* �ȴ�����FIFO���� */
        while(!(readl(SPI_STATUS(spiNo)) & SPI_STATUS_TXNOTFULL)
            && (0 != --ulLoop))
        {
        }

        if(0 == ulLoop)
        {
            return ERROR;
        }

        writel(*pSh++, SPI_DR(spiNo));
    }

    /*������FIFO�е�����ȫ������*/
    while(!(readl(SPI_STATUS(spiNo)) & SPI_STATUS_TXEMPTY)
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
        while(!(readl(SPI_STATUS(spiNo)) & SPI_STATUS_RXNOTEMPTY)
            && (0 != --ulLoop))
        {
        }

        if(0 == ulLoop)
        {
            return ERROR;
        }

        *pRh++ = (u16)readl(SPI_DR(spiNo));

    }
    spi0_core_unlock();

/* SPI���������Զ���ʹ��,��������������� */
#if 0
    /* ��ֹSPI Slave*/
    writel(0, SPI_SLAVE_EN(spiNo));

    /* ��ֹSPI Master*/
    writel(0, SPI_EN(spiNo));
#endif
    return OK;
}

/*****************************************************************************
* �� �� ��  : spiSend
*
* ��������  : SPI������ѯ����
*
* �������  : spiNo: SPI��������
*                       cs: Ƭѡ��
*                    pData: ָ��Ҫ�������ݵ�ַ��ָ��
*                     ulLen: Ҫ���͵����ݵĴ�С
* �������  :
*
* �� �� ֵ  : OK or ERROR
*
* ����˵��  :
*
*****************************************************************************/
s32 balong_spi_send (u32 spiNo, u32 cs, u16* pData, u32 ulLen)
{
    u16 *pSh;
    u32  i;
    u32 ulLoop = SPI_MAX_DELAY_TIMES;
    u32 ulVal;

    if (((0 != spiNo) && (1 != spiNo))
        || (SPI_CS_PMU != cs && SPI_CS_DEV != cs)
        || (NULL == pData) || (0 == ulLen))
    {
        return ERROR;
    }

    pSh = (u16*)pData;

    spi0_core_lock();
    /* ��ֹSPI Slave*/
    writel(0, SPI_SLAVE_EN(spiNo));

    /* ��ֹSPI Master*/
    writel(0, SPI_EN(spiNo));

    /* ���óɷ���ģʽ */
    writel(((readl(SPI_CTRL0(spiNo) )& ~SPI_CTRL0_TMOD_BITMASK) | SPI_CTRL0_TMOD_SEND), SPI_CTRL0(spiNo));

    /*ʹ��SPI Master*/
    writel(1, SPI_EN(spiNo));

    /*ʹ��SPI Slave*/
    writel(cs, SPI_SLAVE_EN(spiNo));

    /* �������� */
    for(i = 0; i < ulLen; i++)
    {
        /* �ȴ�����FIFO���� */
        while(!(readl(SPI_STATUS(spiNo)) & SPI_STATUS_TXNOTFULL)
            && (0 != --ulLoop))
        {
        }

        if(0 == ulLoop)
        {
            return ERROR;
        }
        writel(*pSh++, SPI_DR(spiNo));
    }

    /*������FIFO�е�����ȫ������,�Ҳ�BUSY*/
    ulLoop = SPI_MAX_DELAY_TIMES;
    ulVal = readl(SPI_STATUS(spiNo));
    while(((!(ulVal & SPI_STATUS_TXEMPTY)) || (ulVal & SPI_STATUS_BUSY))
        && (0 != --ulLoop))
    {
        ulVal = readl(SPI_STATUS(spiNo));
    }
    spi0_core_unlock();

    if(0 == ulLoop)
    {
        return ERROR;
    }

    return OK;
}


