/*************************************************************************
*   ��Ȩ����(C) 1987-2020, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  spiDev.h
*
*   ��    �� :  wuzechun
*
*   ��    �� :  spiDev.h ��ͷ�ļ�
*
*   �޸ļ�¼ :  2011��6��9��  v1.00  wuzechun  ����
*
*************************************************************************/

#ifndef __SPIDEV_H__
#define __SPIDEV_H__

/* EEPROM�ʹ���Flashͨ������ */
#define SPI_DEV_CMD_WRSR     0x1     /*Write Status Register*/
#define SPI_DEV_CMD_WRITE    0x2     /*Write Data to Memory Array*/
#define SPI_DEV_CMD_READ     0x3     /*Read Data from Memory Array*/
#define SPI_DEV_CMD_WRDI     0x4     /*Reset Write Enable Latch*/
#define SPI_DEV_CMD_RDSR     0x5     /*Read Status Register*/
#define SPI_DEV_CMD_WREN     0x6     /*Set Write Enable Latch*/

/* ����Flash�������� */
#define SPI_DEV_CMD_RDID     0x9F    /* Read Identification */
#define SPI_DEV_CMD_PP      0x02     /* ҳ��� */
#define SPI_DEV_CMD_SE      0xD8    /* ����� */
#define SPI_DEV_CMD_BE      0xC7    /* Bulk Erase */

/* ״̬�Ĵ���״̬λ */
#define SPI_DEV_STATUS_WIP  (1<<0)  /* Write In Progress Bit */
#define SPI_DEV_STATUS_WEL  (1<<1)  /* Write Enable Latch Bit */
#define SPI_DEV_STATUS_BP0  (1<<2)  /* Block Protect Bits */
#define SPI_DEV_STATUS_BP1  (1<<3)  /* Block Protect Bits */
#define SPI_DEV_STATUS_BP2  (1<<4)  /* Block Protect Bits */
#define SPI_DEV_STATUS_SRWD  (1<<7)  /* Status Register Write Protect */

typedef enum eSpiDevType_t
{
    E_SPI_DEV_TYPE_EEPROM = 0,
    E_SPI_DEV_TYPE_SFLASH
}eSpiDevType;

spiReadStatus spiDevDetect( eSpiDevType *devType );
spiReadStatus spiDevReadBl(UINT8 *pReadBuf);

#endif
