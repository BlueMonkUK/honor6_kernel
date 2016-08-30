
#include <bsp_spi.h>

/*c���иú���ֻ��testʱ����*/
s32 spi_init(u32 spiNo)
{
    u32 reg;
    if ((0 != spiNo) && (1 != spiNo))
    {
        return ERROR;
    }

    /*����ģʽ*/
    reg = readl(HI_SYSCTRL_BASE_ADDR+0x040C);
    reg &= ~0x100;
    writel(reg,HI_SYSCTRL_BASE_ADDR+0x040C);
    mdelay(10);//10ms

    /* ��ֹSPI Slave*/
    writel(0,SPI_SLAVE_EN(spiNo));

    /* ��ֹSPI Master*/
    writel(0,SPI_EN(spiNo));

    if(spiNo)
    {   /*spi1*/
        writel((0x08<<SPI_CTRL0_DFS_BITPOS)    \
                | (0<<SPI_CTRL0_SCPH_BITPOS)    \
                | (0<<SPI_CTRL0_SCPOL_BITPOS)   \
                | (0 << SPI_CTRL0_FRF_BITPOS)   \
                | (0x00<<SPI_CFS_BITPOS),SPI_CTRL0(spiNo));
         /* ����SPI������ΪSSI CLK��1/24����48/6=8MHz */
        writel(6,SPI_BAUD(spiNo));
    }
    else
    {
        /*spi0*/
        /* ����ctrl0�Ĵ����������ֳ���Ϊ1�����ݿ��Ϊ8,�����ش���,�͵�ƽ��Ч */
        writel((0x0f<<SPI_CTRL0_DFS_BITPOS)    \
                | (1<<SPI_CTRL0_SCPH_BITPOS)    \
                | (1<<SPI_CTRL0_SCPOL_BITPOS)   \
                | (0 << SPI_CTRL0_FRF_BITPOS)   \
                | (0x00<<SPI_CFS_BITPOS),SPI_CTRL0(spiNo));

    /* ����SPI������ΪSSI CLK��1/24����48/24=2MHz */
    writel(24,SPI_BAUD(spiNo));
    }

    /*��ֹ�����ж��ź�*/
    writel(0,SPI_IMR(spiNo));

    /*����DMA����*/
    writel(0, SPI_DMAC(spiNo));

    return OK;
}

/*****************************************************************************
* �� �� ��  : spiDevDetect
*
* ��������  : ��̬̽��SPI�����Ͻ�����豸���ͣ���֤SPI��������
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
s32 spi_dev_detect( eSpiDevType *devType )
{
	u16 ucCmdRDID = SPI_DEV_CMD_RDID;
	u32 ulRecv = 0;

	if(NULL == devType)
	{
		spi_print_error("\r\nPARA *devType is NULL!");
		return BSP_SPI_ERROR;
	}

	/* EEPROM�Ը����֧�֣����ղ������ݳ�ʱ�˳� */
	if(BSP_SPI_OK != spi_recv(SPI_NO_DEV, SPI_CS_DEV, (u16*)&ulRecv, 3, &ucCmdRDID, 1))
	{
	    spi_print_error("\r\nread ID err");
	    *devType = E_SPI_DEV_TYPE_EEPROM;

	    return BSP_SPI_OK;
	}

	spi_print_info("\r\nID: %x", ulRecv);

	/* EEPROM��֧�ָ�����᷵��ȫ0��ȫ1 */
	if(0 != ulRecv && 0xFFFFFF != ulRecv)
	{
	    *devType = E_SPI_DEV_TYPE_SFLASH;
	}
	else
	{
	    *devType = E_SPI_DEV_TYPE_EEPROM;
	}
	spi_print_info("\r\ndevType is :  @ %x", *devType);

	return BSP_SPI_OK;

}
/*****************************************************************************
* �� �� ��  : spi_test
*
* ��������  : ͨ����̬̽��SPI�����Ͻ�����豸���ͣ���֤SPI��������,��֤�豸Ϊeeprom��sflash
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
s32 spi_test( void )
{
	eSpiDevType edevType;

	(void)spi_init(SPI_NO_DEV);

	(void)spi_dev_detect(&edevType);

	if(E_SPI_DEV_TYPE_EEPROM == edevType)
	{
	    spi_print_info("\r\nE2PROM_boot!" );
	}
	else
	{
	    spi_print_info("\r\nSF_boot!" );
	}
	return 0;
}


