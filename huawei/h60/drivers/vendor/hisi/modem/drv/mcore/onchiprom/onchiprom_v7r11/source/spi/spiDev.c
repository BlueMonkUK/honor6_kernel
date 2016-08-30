/*************************************************************************
*   ��Ȩ����(C) 1987-2020, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  spiDev.c
*
*   ��    �� :  wuzechun
*
*   ��    �� :  spi�豸����������Ҫ���ܰ�������
*                 SPI�ҽӵ��豸��EEPROM����SFlash���Լ�����
*                 �豸���Ͷ�ȡ�Ĺ��ܡ�
*
*   �޸ļ�¼ :  2011��6��9��  v1.00  wuzechun  ����
*
*************************************************************************/

#include "sys.h"
#include "OnChipRom.h"
#include "secBoot.h"
#include "spi.h"
#include "spiDev.h"

/*****************************************************************************
* �� �� ��  : spiDevDetect
*
* ��������  : ��̬̽��SPI�����Ͻ�����豸����
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
spiReadStatus spiDevDetect( eSpiDevType *devType )
{
    UINT8 ucCmdRDID = SPI_DEV_CMD_RDID;
    UINT32 ulRecv = 0;

    if(NULL == devType)
    {
        return SPI_READ_ERR_PARA;
    }

    /* EEPROM�Ը����֧�֣����ղ������ݳ�ʱ�˳� */
    if(OK != spiRecv(SPI_NO_DEV, SPI_CS_DEV, (UINT8*)&ulRecv, 3, &ucCmdRDID, 1))
    {
        print_info("\r\nread ID err");
        *devType = E_SPI_DEV_TYPE_EEPROM;

        return SPI_READ_OK;
    }

    print_info_with_u32("\r\nID:", ulRecv);

    /* EEPROM��֧�ָ�����᷵��ȫ0��ȫ1 */
    if(0 != ulRecv && 0xFFFFFF != ulRecv)
    {
        *devType = E_SPI_DEV_TYPE_SFLASH;
    }
    else
    {
        *devType = E_SPI_DEV_TYPE_EEPROM;
    }

    return SPI_READ_OK;

}

/*****************************************************************************
* �� �� ��  : spiDevGetStat
*
* ��������  : ��EEPROM״̬�Ĵ���
*
* �������  :
* �������  :
*
* �� �� ֵ  : EEPROM״̬�Ĵ�����ֵ
*
* ����˵��  :
*
*****************************************************************************/
UINT8 spiDevGetStat(void)
{
    UINT8 tx_cmd;
    UINT8 ucSpiDevStat;              /*spiDev״̬�Ĵ�����ֵ*/

    tx_cmd = SPI_DEV_CMD_RDSR;

    /*��ȡSPI�豸��״̬�Ĵ���*/
    (void)spiRecv(SPI_NO_DEV, SPI_CS_DEV, &ucSpiDevStat,1,&tx_cmd,1);

    //print_info_with_u32("\r\nsta:", ucSpiDevStat);
    return ucSpiDevStat;
}

/*****************************************************************************
* �� �� ��  : spiDevRead
*
* ��������  : SPI�豸��ȡ�ӿڡ�
*
* �������  : pReadBuf: ָ������Ŀ�ĵ�ַ��ָ��
*                 devType: �豸����,EEPROM��SFlash
* �������  :
*
* �� �� ֵ  : OK/ERROR
*                SEC_IMAGE_LEN_ERROR: ��ȡ��BootLoader���ȴ���
*
* ����˵��  :
*
*****************************************************************************/
spiReadStatus spiDevRead(eSpiDevType devType, UINT32 addr, UINT8 *pReadBuf, UINT32 len)
{
    UINT32 u16SendLen = 0;
    UINT32 u16RcveLen = 0;
    UINT32 ulLoop;
    INT32 ret;
    UINT8 ucCmd[4];
    UINT8 ucStatus;

    /*�ж�����ĺϷ��ԣ�����Ϸ�����������Ϸ��򷵻ء�*/
    if(NULL == pReadBuf)
    {
        return SPI_READ_ERR_PARA;
    }

    /* �ȴ����� */
    ulLoop = SPI_MAX_DELAY_TIMES;
    while(((ucStatus = spiDevGetStat()) & SPI_DEV_STATUS_WIP)
        && (0 != --ulLoop))
    {
    }

    if(0 == ulLoop)
    {
        print_info_with_u32("\r\nstatus:", ucStatus);
        return SPI_READ_ERR_GETSTATUS;
    }

    /* ��������,��0��ַ��ʼ�� ��������64k������Ҫ�ֶ��ѭ����*/
    while(len > 0)
    {
        u16SendLen = 0;
        ucCmd[u16SendLen++] = SPI_DEV_CMD_READ;
        
        if(len > (0x01 << 16))
            u16RcveLen = 0x01 << 16;
        else 
            u16RcveLen = len;

        if(E_SPI_DEV_TYPE_EEPROM == devType)
        {
            /* EEPROM��ַΪ2�ֽ� */
            ucCmd[u16SendLen++] = (UINT8)(addr>>8);
            ucCmd[u16SendLen++] = (UINT8)addr;
        }
        else
        {
            /* ����Flash��ַΪ3���ֽ� */
            ucCmd[u16SendLen++] = (UINT8)(addr>>16);
            ucCmd[u16SendLen++] = (UINT8)(addr>>8);
            ucCmd[u16SendLen++] = (UINT8)addr;
        }

        ret = spiRecv(SPI_NO_DEV, SPI_CS_DEV, pReadBuf, u16RcveLen, ucCmd, u16SendLen);
        if(OK != ret)
        {
            return SPI_READ_ERR_READ_BL;
        }
        addr += u16RcveLen;
        pReadBuf += u16RcveLen;
        len -= u16RcveLen;
        
    }

    return SPI_READ_OK;
}

/*****************************************************************************
* �� �� ��  : spiDevReadBl
*
* ��������  : SPI�豸��ȡ�ӿڡ�
*
* �������  : pReadBuf: ָ������Ŀ�ĵ�ַ��ָ��,���뱣֤�ֶ���
*
* �������  :
*
* �� �� ֵ  : OK/ERROR
*                SEC_IMAGE_LEN_ERROR: ��ȡ��BootLoader���ȴ���
*
* ����˵��  :
*
*****************************************************************************/
spiReadStatus spiDevReadBl(UINT8 *pReadBuf)
{
    UINT32 u16RecvLen;
    eSpiDevType edevType;

    /*�ж�����ĺϷ��ԣ�����Ϸ�����������Ϸ��򷵻ء�*/
    if((NULL == pReadBuf)
        || ((UINT32)pReadBuf % 4))
    {
        return SPI_READ_ERR_PARA;
    }

    (void)spiInit(SPI_NO_DEV);
    (void)spiDevDetect(&edevType);
    if(E_SPI_DEV_TYPE_EEPROM == edevType)
    {
        print_info("\r\nE2PROM_boot!" );
    }
    else
    {
        print_info("\r\nSF_boot!" );
    }

    /* �Ȼ�ȡ�����ֽ� */
    u16RecvLen = BL_LEN_INDEX + 4;
    if(SPI_READ_OK != spiDevRead(edevType, 0, pReadBuf, u16RecvLen))
    {
        return SPI_READ_ERR_READ_LENBYTE;
    }

    u16RecvLen = *(volatile UINT32*)(pReadBuf+BL_LEN_INDEX); /*lint !e826*/  /* ��֤pReadBuf��4�ֽڶ��� */

    /* �жϳ����Ƿ�Ϸ�:��Ϊ��/�ֶ���/����ת/������ */
    if((0 == u16RecvLen)
        || (u16RecvLen % 4)
        || (u16RecvLen + IDIO_LEN + OEM_CA_LEN + IDIO_LEN < u16RecvLen)
        || (u16RecvLen + IDIO_LEN + OEM_CA_LEN + IDIO_LEN > BOOTLOAD_SIZE_MAX))
    {
        print_info_with_u32("\r\nBL len err:", u16RecvLen);

        return SPI_READ_ERR_BL_LEN;
    }

    /* ���Ͼ���ǩ����OEM CA��OEM CAǩ���ĳ��� */
    u16RecvLen += IDIO_LEN + OEM_CA_LEN + IDIO_LEN;

    /* ��ȡ����BootLoader���� */
    if(SPI_READ_OK != spiDevRead(edevType, 0, pReadBuf, u16RecvLen))
    {
        return SPI_READ_ERR_READ_BL;
    }

    return SPI_READ_OK;
}

#ifdef SPI_TEST

#include "string.h"

#define SPI_FLASH_WR_DATA_SIZE 256    /* �ִ�д�룬ÿ��256�ֽ� */
#define SPI_EEPROM_WR_DATA_SIZE 64    /* �ִ�д�룬ÿ��64�ֽ� */
#define SPI_CMD_WR_SIZE_MAX 4

INT32 spiWrtStat(UINT8 val)
{
    UINT32 u16SendLen;
    UINT32 ulLoop;
    UINT8 ucCmd[2];
    UINT8 ucSpiDevStat;              /*spiDev״̬�Ĵ�����ֵ*/

    ucCmd[0] = SPI_DEV_CMD_WRSR;
    ucCmd[1] = val;

    /* ��������,ʹ��д */
    ucCmd[0] = SPI_DEV_CMD_WREN;
    u16SendLen = 1;

    /* ���� */
    if(OK != spiSend(SPI_NO_DEV, SPI_CS_DEV, ucCmd, u16SendLen))
    {
        return ERROR;
    }

    /*��ȡSPI�豸��״̬�Ĵ���*/
    (void)spiSend(SPI_NO_DEV, SPI_CS_DEV, ucCmd,2);

    /* �ȴ������� */
    ulLoop = SPI_MAX_DELAY_TIMES;
    while((spiDevGetStat() & SPI_DEV_STATUS_WIP)
        && (0 != --ulLoop))
    {
    }

    if(0 == ulLoop)
    {
        print_info("\r\nwrite timeout");
        return ERROR;
    }

    /* ��������,��ֹд */
    ucCmd[0] = SPI_DEV_CMD_WRDI;
    u16SendLen = 1;

    /* ���� */
    if(OK != spiSend(SPI_NO_DEV, SPI_CS_DEV, ucCmd, u16SendLen))
    {
        return ERROR;
    }

    if(val != (ucSpiDevStat = spiDevGetStat()))
    {
        print_info_with_u32("\r\nsta:", ucSpiDevStat);
    }

    return ERROR;

}
void spiWrtStop(void)
{
    return;
}
/*****************************************************************************
* �� �� ��  : spiDevWrite
*
* ��������  : SPI�豸д�ӿڡ�
*
* �������  : pReadBuf: ָ������Ŀ�ĵ�ַ��ָ��
*                 devType: �豸����,EEPROM��SFlash
* �������  :
*
* �� �� ֵ  : OK/ERROR
*                SEC_IMAGE_LEN_ERROR: ��ȡ��BootLoader���ȴ���
*
* ����˵��  :
*
*****************************************************************************/
int spiDevWrite(eSpiDevType devType, UINT32 destAddr, UINT8 *pBuf, UINT32 ulLen)
{
#define E2PROM_MAX_SIZE (16*0x400)

    UINT32 ulCmdLen = 0;
    UINT32 ulDataLen;
    UINT32 u16SendLen;
    UINT32 ulLenRemain = ulLen;
    UINT32 ulAddrToWrt = destAddr;
    UINT32 ulWrtMaxBytesPerTime;
    UINT8 *pWrtBuf = pBuf;
    UINT32 ulLoop;
    UINT8 ucCmd[SPI_FLASH_WR_DATA_SIZE + SPI_CMD_WR_SIZE_MAX];

    /*�ж�����ĺϷ��ԣ�����Ϸ�����������Ϸ��򷵻ء�*/
    if(NULL == pBuf)
    {
        return ERROR;
    }

    if((E_SPI_DEV_TYPE_EEPROM == devType)
        && (destAddr + ulLen > E2PROM_MAX_SIZE))
    {
        print_info("EEPRPM write out of range\r\n");
        return -1;
    }

    (void)spiInit(SPI_NO_DEV);

    /* ����Flash��EEPROMÿ��д��ĳ��Ȳ�ͬ */
    if(E_SPI_DEV_TYPE_SFLASH == devType)
    {
        ulWrtMaxBytesPerTime = SPI_FLASH_WR_DATA_SIZE;
    }
    else
    {
        ulWrtMaxBytesPerTime = SPI_EEPROM_WR_DATA_SIZE;
    }

    /* ����Flashд֮ǰ�Ȳ��� */
    if(E_SPI_DEV_TYPE_SFLASH == devType)
    {
        /* ��������,ʹ�ܲ�����д */
        ucCmd[0] = SPI_DEV_CMD_WREN;
        u16SendLen = 1;

        /* ���� */
        if(OK != spiSend(SPI_NO_DEV, SPI_CS_DEV, ucCmd, u16SendLen))
        {
            return ERROR;
        }

        /* ��������,����һ��sector:1024*256Bytes */
        ucCmd[0] = SPI_DEV_CMD_SE;

        ucCmd[1] = ulAddrToWrt>>16;
        ucCmd[2] = ulAddrToWrt>>8;
        ucCmd[3] = ulAddrToWrt;
        /* ���� */
        if(OK != spiSend(SPI_NO_DEV, SPI_CS_DEV, ucCmd, 4))
        {
            return ERROR;
        }

        /* �ȴ�������� */
        print_info("\r\n erasing...");
        ulLoop = SPI_MAX_DELAY_TIMES;
        while((spiDevGetStat() & SPI_DEV_STATUS_WIP)
            && (0 != --ulLoop))
        {
            if(ulLoop % 100 == (100-1))
            {
                print_info(".");
            }
        }

        if(0 == ulLoop)
        {
            print_info("\r\nerase timeout");
            return ERROR;
        }
        print_info_with_u32("\r\nulLoop for erase:", ulLoop);

        /* ��������,��ֹд */
        ucCmd[0] = SPI_DEV_CMD_WRDI;
        u16SendLen = 1;

        /* ���� */
        if(OK != spiSend(SPI_NO_DEV, SPI_CS_DEV, ucCmd, u16SendLen))
        {
            return ERROR;
        }

    }

    while(ulLenRemain>0)
    {

        /* ��������,ʹ��д */
        ucCmd[0] = SPI_DEV_CMD_WREN;
        u16SendLen = 1;

        /* ���� */
        if(OK != spiSend(SPI_NO_DEV, SPI_CS_DEV, ucCmd, u16SendLen))
        {
            return ERROR;
        }

        /* ����д���� */
        if(E_SPI_DEV_TYPE_EEPROM == devType)
        {
            ucCmd[0] = SPI_DEV_CMD_WRITE;
        }
        else
        {
            ucCmd[0] = SPI_DEV_CMD_PP;
        }
        //print_info_with_u32("\r\n\r\npragram addr: 0x", ulAddrToWrt);
        /* ����д����+��ַ */
        if(E_SPI_DEV_TYPE_EEPROM == devType)
        {
            /* EEPROM��ַΪ2�ֽ� */
            ucCmd[1] = ulAddrToWrt >> 8;
            ucCmd[2] = ulAddrToWrt;
            ulCmdLen = 3;
        }
        else
        {
            /* ����Flash��ַΪ3���ֽ� */
            ucCmd[1] = ulAddrToWrt >> 16;
            ucCmd[2] = ulAddrToWrt >> 8;
            ucCmd[3] = ulAddrToWrt;
            ulCmdLen = 4;
        }

        ulDataLen = ulLenRemain > ulWrtMaxBytesPerTime ? ulWrtMaxBytesPerTime : ulLenRemain;

        //print_info_with_u32("\r\nbuf addr: 0x", (UINT32)pWrtBuf);
        /* ������� */
        memcpy(ucCmd+ulCmdLen, pWrtBuf, ulDataLen);
        u16SendLen = ulCmdLen + ulDataLen;

        /* ���� */
        if(OK != spiSend(SPI_NO_DEV, SPI_CS_DEV, ucCmd, u16SendLen))
        {
            return ERROR;
        }

        /* �ȴ������� */
        ulLoop = SPI_MAX_DELAY_TIMES;
        while((spiDevGetStat() & SPI_DEV_STATUS_WIP)
            && (0 != --ulLoop))
        {
        }

        if(0 == ulLoop)
        {
            print_info("\r\nwrite timeout");
            return ERROR;
        }
        print_info_with_u32("\r\nulLoop:", ulLoop);

        print_info("#");
        /* �����Ա��´�д�� */
        ulAddrToWrt += ulDataLen;
        pWrtBuf += ulDataLen;
        ulLenRemain -= ulDataLen;

        /* ��������,��ֹд */
        ucCmd[0] = SPI_DEV_CMD_WRDI;
        u16SendLen = 1;

        /* ���� */
        if(OK != spiSend(SPI_NO_DEV, SPI_CS_DEV, ucCmd, u16SendLen))
        {
            return ERROR;
        }

    }

    print_info("\r\nspi write success");

    //for trace32 to Stop
    spiWrtStop();

    return OK;
}
#endif

