/*************************************************************************
*   ��Ȩ����(C) 1987-2004, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  emmcMain.c
*
*   ��    �� :  wangzhongshun
*
*   ��    �� :  emmc�Ծٵ������̿���
*
*   �޸ļ�¼ :  2011��6��15��  v1.00  wangzhongshun  ����
*
*************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  ͷ�ļ�
*****************************************************************************/
#include "sys.h"
#include "secBoot.h"
#include "OnChipRom.h"
#include "emmcConfig.h"
#include "emmcIp.h"
#include "emmcErrcode.h"
#include "emmcIdmac.h"
#include "emmcNormal.h"
#include "emmcMain.h"
#include "ioinit.h"

/*****************************************************************************
  ȫ�ֱ���
*****************************************************************************/
extern BOOL emmc_get_card_present(UINT32 slot);

/*****************************************************************************
  ����ʵ��
*****************************************************************************/
VOID emmc_ulong2byte(UINT8 *pucByte, UINT32 ulLong)
{
    pucByte[0] = (ulLong>>0)  & 0xff;
    pucByte[1] = (ulLong>>8)  & 0xff;
    pucByte[2] = (ulLong>>16) & 0xff;
    pucByte[3] = (ulLong>>24) & 0xff;
}

UINT32 emmc_byte2ulong(UINT8 *pucByte)
{
    return ((pucByte[0])|(pucByte[1]<<8)|(pucByte[2]<<16)|(pucByte[3]<<24));
}

VOID emmc_delay_ms(UINT32 ulMsecond)
{
    TIMER0_VAR_DECLAR;

    TIMER0_INIT_TIMEOUT(ulMsecond);
    for( ; ; )
    {
        if(TIMER0_IS_TIMEOUT())
        {
            break ;
        }
    }
}

#ifdef EMMC_DEBUG
VOID emmc_print_current_tick(VOID)
{
    volatile UINT32 ulTick;

    ulTick = INREG32(TIMER0_REGOFF_VALUE);

    print_info_with_u32((UINT8 *)"time=0x", ulTick);
}

VOID emmc_reg_dump(VOID)
{
    UINT32 i;
    UINT32 ulReg;

    print_info("\r\nemmc reg show:\r\n");
    for(i=0;i<=(EMMC_REG_BUFADDR-EMMC_REG_CTRL);i+=4)
    {
        ulReg = INREG32(i+EMMC_REG_CTRL);
        print_info_with_u32("0x",ulReg);
        print_info(" ");
        if(0 == (i+4)%16)
        {
            print_info("\r\n");
        }
    }
    print_info("\r\n");
}

VOID emmc_packet_dump(UINT8 *pcAddr)
{
    UINT32 i;

    print_info("\r\n");
    for(i=0; i<32;i++)
    {
        print_info_with_u32(" 0x",pcAddr[i]);
        if(!((i+1)%4))
        {
            print_info("\r\n");
        }
    }
}
#endif

#if 0
/*****************************************************************************
* �� �� ��  : emmc_card_power
*
* ��������  : �����µ�
*
* �������  : BOOL bPowerOn  : TRUE :�ϵ� FALSE:�µ�
* �������  : ��
*
* �� �� ֵ  : ��
*
* ����˵��  : ����Hi6451V100 PMU��MMC��SD�����µ磬��EMMC����Ӱ�죬EMMC��ʹ��
*             ����LDOһֱ����.Ӳ������������������PMU����MMC��SD���Ծٹ�����Ч
*
*****************************************************************************/
UINT32 emmc_card_power(BOOL bPowerOn)
{
#if PLATFORM != PLATFORM_P530_PORTING
    UINT8 u8Data = 0;
    INT32 iRetVal = 0;

    if(bPowerOn)
    {
        iRetVal = pmu_read(EMMC_PMU_LDO10_ADDR,&u8Data);
        if(iRetVal)
        {
            return ERRPMUOP;
        }

        if(EMMC_PMU_LDO10_ON != (u8Data&EMMC_PMU_LDO10_MASK))
        {
            u8Data &= ~EMMC_PMU_LDO10_MASK;
            u8Data |= EMMC_PMU_LDO10_ON;
            iRetVal = pmu_write(EMMC_PMU_LDO10_ADDR,u8Data);
            if(iRetVal)
            {
                return ERRPMUOP;
            }
        }
    }
    else
    {
        iRetVal = pmu_read(EMMC_PMU_LDO10_ADDR,&u8Data);
        if(iRetVal)
        {
            return ERRPMUOP;
        }

        if(EMMC_PMU_LDO10_OFF != (u8Data&EMMC_PMU_LDO10_MASK))
        {
            u8Data &= ~EMMC_PMU_LDO10_MASK;
            u8Data |= EMMC_PMU_LDO10_OFF;
            iRetVal = pmu_write(EMMC_PMU_LDO10_ADDR,u8Data);
            if(iRetVal)
            {
                return ERRPMUOP;
            }
        }
    }
#endif

    return ERRNOERROR;
}/*lint !e715*/
#endif
/*****************************************************************************
* �� �� ��  : emmc_read_bootloader
*
* ��������  : ��eMMC/MMC/SD�ж�ȡbootloader
*
* �������  : UINT32 dst
* �������  : ��
*
* �� �� ֵ  : 0 :��ȡ�ɹ�
*             -1:��ȡʧ��
*
* ����˵��  : ��
*
*****************************************************************************/
INT32 emmc_read_bootloader(UINT32 dst)
{
    UINT32 ulRetVal = ERRCOMERR;
    UINT32 slot = EMMC_CURRENT_SLOT;

#ifdef EMMC_DEBUG
    memset((VOID *)dst, 0x0, BOOTLOAD_SIZE_MAX);
#endif

    /* default value is not suitable for emmc, need config*/
    /*IO�ܽŸ������� */
    EMMC_IOS_SYS_CONFIG();
    /*ϵͳ���ã���ʾ����ʱ���Ż��ر�*/
    EMMC_SYS_CONFIG();

    EMMC_SET_CLK();
    /*
        Config the emmc timer.
        emmc use it for timeout check.
        Now the timer is active.
    */
    TIMER0_INIT();

    /* ����ΪEMMC����USE_HOLD=1 */
    ulRetVal = emmc_read_bootloader_normal(slot, dst, TRUE);

    if(ERRENUMERATE == ulRetVal)
    {
        /* ����ΪEMMC����USE_HOLD=0 */
        ulRetVal = emmc_read_bootloader_normal(slot, dst, FALSE);
    }

    TIMER0_DEINIT();

    return ulRetVal?(-1):0;
}

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif
