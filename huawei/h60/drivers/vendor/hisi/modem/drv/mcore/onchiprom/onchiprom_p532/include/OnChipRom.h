/*************************************************************************
*   ��Ȩ����(C) 1987-2020, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  OnChipRom.h
*
*   ��    �� :  wuzechun
*
*   ��    �� :  OnChipRomͨ��ͷ�ļ�
*
*   �޸ļ�¼ :  2011��6��9��  v1.00  wuzechun  ����
*
*************************************************************************/

#ifndef __ONCHIPROM_H__
#define __ONCHIPROM_H__


#include "config.h"
#include "osl_bio.h"

/* ƽ̨���Ͷ��壬ʵ�ʵ�ƽ̨������Makefile���� */
#define PLATFORM_ASIC      1
#define PLATFORM_PORTING   2
#define PLATFORM_EDA       3

#define YES                1
#define NO                 0

/* IMAGE FORMAT */
#define DOWNLOAD_BL_LEN_INDEX   (44)      /*bootrom���񳤶�ƫ��*/
#define BL_LEN_INDEX            (144 * 4)    /* bootload.bin�ļ�����(Byte)������bootload.bin�ļ��е�ƫ���ֽ��� */
#define ROOT_CA_INDEX           (BL_LEN_INDEX + 4)   /* ��CA��Image��λ�� */
#define BL_SEC_INDEX            (ROOT_CA_INDEX)


/* EFUSE */
#define WORD_SIZE   (sizeof(unsigned int)/sizeof(char))

#define TIMEMAXDELAY    0x1000000       /*����ӳ�ʱ��*/
#define SPI_MAX_DELAY_TIMES 0x10000

#define INTEGRATOR_DELAYFORSLAVE_VALUE 0x10100

/*�жϾ����Ƿ���д*/
#define BL_CHECK_ADDR_OFFSET    0x368  /*�жϾ���̶��ַ���ƫ�Ƶ�ַ*/
#define BL_CHECK_INSTRUCTION    0x79706F43 /*�����жϾ����Ƿ���д�Ĺ̶��ַ�ָ�� */
#define BL_CHECK_ADDR (M3_TCM_BL_ADDR + BL_CHECK_ADDR_OFFSET) /*�жϾ����й̶��ַ��ľ��Ե�ַ*/

/* EFUSE�ռ仮��*/
#define EFUSE_GRP_ROOT_CA   0x00
#define EFUSE_GRP_CFG       (EFUSE_MAX_SIZE / WORD_SIZE - 1)

/*
 * System Ctrl module register define
 */
 
/*ϵͳ������*/
#define INTEGRATOR_PERI_BASE             0x9001B000
#define INTEGRATOR_PERI_SIZE             (4*0x400)   /* 4K */

/*ϵͳ������*/
#define INTEGRATOR_SC_BASE              0x90000000
#define INTEGRATOR_SC_SIZE              (4*0x400)   /* 4K */

/* USB */
#define USB_REG_BASE                  	0x90200000
#define USB_REG_SIZE                    (1*1024*1024)   /* 1M */

/* bc_ctl */
#define BC_CTL_BASE                     0x9000C000
#define BC_CTL_SIZE                     (4*0x400)   /* 4K */

//#define SC_CTRL3        (INTEGRATOR_SC_BASE+0x40C)  /*spi�������üĴ���*/
//#define SC_CRG_DIV3     (INTEGRATOR_SC_BASE+0x108) /* CLK DIV 3 */

#define SC_STAT0        (INTEGRATOR_SC_BASE+0x0600)

//#define SC_CTRL_REGBASE_ADDR    0x9000C000

#define SC_BOOTMODE_BITPOS          0
#define SC_BOOTMODE_BITWIDTH        3
#define SC_BOOTMODE_BITMASK         (((1<<SC_BOOTMODE_BITWIDTH)-1)<<SC_BOOTMODE_BITPOS)
#define BOOT_MODE_EMMC              (0x01<<SC_BOOTMODE_BITPOS) /* p532��ʹ�� */
#define BOOT_MODE_NAND_BOOT         (0x02<<SC_BOOTMODE_BITPOS)
#define BOOT_MODE_NAND_EFUSE        (0x03<<SC_BOOTMODE_BITPOS)
#define BOOT_MODE_SPI               (0x04<<SC_BOOTMODE_BITPOS) /* p532��ʹ�� */
#define BOOT_MODE_AP_HSIC           (0x05<<SC_BOOTMODE_BITPOS) /* p532��ʹ�� */
#define BOOT_MODE_AP_HSUART         (0x06<<SC_BOOTMODE_BITPOS) /* p532��ʹ�� */
#define BOOT_MODE_NAND_ID           (0x07<<SC_BOOTMODE_BITPOS)

#define SC_AUTO_ENUM_EN_BITPOS      3
#define SC_AUTO_ENUM_EN_BITWIDTH    1
#define SC_AUTO_ENUM_EN_BITMASK     (((1<<SC_AUTO_ENUM_EN_BITWIDTH)-1)<<SC_AUTO_ENUM_EN_BITPOS)
#define SC_AUTO_ENUM_EN             (0<<SC_AUTO_ENUM_EN_BITPOS)

/*
 * Power Ctrl module register define
 */
#define PWR_CTRL_BASE   0x90000C00
#define PWR_CTRL0       (PWR_CTRL_BASE + 0x00)
#define PWR_CTRL13      (PWR_CTRL_BASE + 0x34)

/*
 * UART module register define
 */
#define UART0_BASE (INTEGRATOR_SC_BASE+0x7000)
#define REG_UARTDR          0x000  /* ���ݼĴ��� */
#define REG_UARTRSR_UARTECR 0x004  /* ����״̬�Ĵ���/��������Ĵ��� */
#define REG_UARTFR          0x018  /* ��ǩ�Ĵ��� */
#define REG_UARTIBRD        0x024  /* �����ʼĴ������������֡� */
#define REG_UARTFBRD        0x028  /* �����ʼĴ�����С�����֡� */
#define REG_UARTLCR_H       0x02C  /* �߿ؼĴ��� */
#define REG_UARTCR          0x030  /* ���ƼĴ��� */
#define REG_UARTIFLS        0x034  /* �ж�FIFO levelѡ��Ĵ���  FIFO-64 */
#define REG_UARTIMSC        0x038  /* �ж����μĴ��� */
#define REG_UARTICR         0x044  /* �ж�����Ĵ��� */
#define REG_UARTDMACR       0x048  /* DMA���ƼĴ��� */

/* HSUART */
#define UART_RBR   0x0
#define UART_THR   0x0
#define UART_DLL   0x0
#define UART_IER   0x4
#define UART_DLH   0x4
#define UART_IIR   0x8
#define UART_FCR   0x8
#define UART_LCR   0xC
#define UART_MCR   0x10
#define UART_LSR   0x14
#define UART_TXDEPTH 0x2C
#define UART_DEBUG 0x28
#define UART_FAR   0x70
#define UART_TFR   0x74
#define UART_RFW   0x78
#define UART_USR   0x7C
#define UART_TFL   0x80
#define UART_RFL   0x84
#define UART_HTX   0xA4
#define UART_DMASA 0xA8
#define UART_CPR   0xF4

/***********SPI*****************/
#define ECS_SPI0_BASE       (INTEGRATOR_SC_BASE + 0x00008000)
#define ECS_SPI0_SIZE		0x00001000

#define ECS_SPI1_BASE       (INTEGRATOR_SC_BASE + 0x00023000)
#define ECS_SPI1_SIZE		0x00001000

#if PLATFORM==PLATFORM_PORTING
/* OnChipRom E2PROMʹ��SPI0 CS0 */ /*need make sure*/
#define SPI_NO_DEV          0       /*SPI0*/
#define SPI_CS_EEPROM       (1<<0)   /* EEPROMʹ��SSI0 CS0 */
#define SPI_CS_SFLASH       (1<<1)   /* EEPROMʹ��SSI0 CS1 */
/*for porting test*/
//#define SPI_CS_DEV          SPI_CS_EEPROM
#define SPI_CS_DEV          SPI_CS_SFLASH
#else
/* ASIC OnChipRom E2PROM/SFlashʹ��SPI1 CS1 */
#define SPI_NO_DEV          1          /*SPI1*/
#define SPI_CS_DEV          (1<<1)     /* CS1 for EEPROM/SFlash */
#endif

/* PMUʹ��SPI0 CS0 */
#define SPI_NO_PMU          0
#define SPI_CS_PMU          (1<<0)     /* CS0,for eMMC/MMC/SD */

/*************************************************
*
*  NANDC   base address definition
*
*************************************************/
#define NANDC_BASE          0x900A4000
#define NANDC_SIZE          0x0001000

#define NANDC_CON           (NANDC_BASE + 0x00)
#define NANDC_OPMODE_OS     0  /* 1ΪNormalģʽ,0ΪBootģʽ */
#define NANDC_OPMODE_WIDTH  1
#define NANDC_OPMODE_MASK  (((1<<NANDC_OPMODE_WIDTH)-1)<<NANDC_OPMODE_OS)
#define NANDC_OPMODE_BOOT   (0<<NANDC_OPMODE_OS)  /* 1ΪNormalģʽ,0ΪBootģʽ */
#define NAND_ECC_TYPE_OS    9
#define NAND_ECC_TYPE_WIDTH 3
#define NAND_ECC_TYPE_MASK  (((1<<NAND_ECC_TYPE_WIDTH)-1)<<NAND_ECC_TYPE_OS)
#define NAND_ECC_TYPE_0     (0)
#define NAND_ECC_TYPE_1     (1<<NAND_ECC_TYPE_OS)
#define NAND_ECC_TYPE_4     (2<<NAND_ECC_TYPE_OS)
#define NAND_ECC_TYPE_24_512K    (3<<NAND_ECC_TYPE_OS)
#define NAND_ECC_TYPE_24_1K    (4<<NAND_ECC_TYPE_OS)

#define NANDC_PWIDTH        (NANDC_BASE + 0x04)

/* ECC������,�жϷ�ʽ,���ɿ� */
#define NANDC_INTEN         (NANDC_BASE + 0x24)
#define ECC_ERR_INVALID     (1<<6)
#define NANDC_INTS          (NANDC_BASE + 0x28)
/* ECC������,ʹ�ñ���ʽ */
#define NANDC_ECC_TEST      (NANDC_BASE + 0x5C)

/* WDT */
#define SC_WDTCTRL          (INTEGRATOR_SC_BASE + 0x0408)
#define WDT_REGBASE_ADDR    (INTEGRATOR_SC_BASE + 0x1000)

#define WDG_LOAD    (WDT_REGBASE_ADDR + 0x000)   /* ������ֵ�Ĵ��� */
#define WDG_VALUE   (WDT_REGBASE_ADDR + 0x004)   /* ��������ǰֵ�Ĵ��� */
#define WDG_CONTROL (WDT_REGBASE_ADDR + 0x008)   /* ���ƼĴ��� */
#define WDG_INTCLR  (WDT_REGBASE_ADDR + 0x00C)   /* �ж�����Ĵ��� */
#define WDG_RIS     (WDT_REGBASE_ADDR + 0x010)   /* ԭʼ�жϼĴ��� */
#define WDG_MIS     (WDT_REGBASE_ADDR + 0x014)   /* ���κ��жϼĴ��� */
#define WDG_LOCK    (WDT_REGBASE_ADDR + 0xC00)   /* LOCK�Ĵ��� */
#define WDT_LOCK_VALUE   0x1ACCE551

/* TIMER */
/* timer freq */
#define TIMER0_FREQ               (19200*1000) /* HZ */
#define TIMER2_FREQ               (19200*1000) /* HZ */

#define US_PER_SECEND             (1000000)
#define MS_TICKS                  (TIMER0_FREQ/1000)

#define TIMER_INIT_VALUE         (0xFFFFFFFF)

/*timer 0*/
#define TIMER0_REGBASE_ADDR       (0x90002000)
#define TIMER0_REGBASE_SIZE       (0x1000)

/* timer0 register */
#define TIMER0_REGOFF_LOAD	       (TIMER0_REGBASE_ADDR + 0x000)    /* Load (R/W) */
#define TIMER0_REGOFF_VALUE        (TIMER0_REGBASE_ADDR + 0x004)    /* Value (R/O) */
#define TIMER0_REGOFF_CTRL         (TIMER0_REGBASE_ADDR + 0x008)    /* Control (R/W) */
#define TIMER0_REGOFF_CLEAR        (TIMER0_REGBASE_ADDR + 0x00C)    /* Clear (W/O) */
#define TIMER0_REGOFF_INTSTATUS    (TIMER0_REGBASE_ADDR + 0x010)    /* INT STATUS (R/O) */

#define TIMER_ENABLE_ADRR          (INTEGRATOR_SC_BASE + 0x000)
#define TIMER_DISABLE_ADRR         (INTEGRATOR_SC_BASE + 0x004)
#define TIMER_CLK_SEL              (INTEGRATOR_SC_BASE + 0x140)

/*timer 2*/
#define TIMER2_REGBASE_ADDR       (0x90003000)
#define TIMER2_REGBASE_SIZE       (0x1000)

/* timer2 register */
#define TIMER2_REGOFF_LOAD	       (TIMER2_REGBASE_ADDR + 0x000)    /* Load (R/W) */
#define TIMER2_REGOFF_VALUE        (TIMER2_REGBASE_ADDR + 0x004)    /* Value (R/O) */
#define TIMER2_REGOFF_CTRL         (TIMER2_REGBASE_ADDR + 0x008)    /* Control (R/W) */
#define TIMER2_REGOFF_CLEAR        (TIMER2_REGBASE_ADDR + 0x00C)    /* Clear (W/O) */
#define TIMER2_REGOFF_INTSTATUS    (TIMER2_REGBASE_ADDR + 0x010)    /* INT STATUS (R/O) */

#ifdef START_TIME_TEST
#define TIMER4_REGBASE_ADDR       (0x90004000)
#define TIMER4_REGBASE_SIZE       (0x1000)
/* timer4 register */
#define TIMER4_REGOFF_LOAD	       (TIMER4_REGBASE_ADDR + 0x000)    /* Load (R/W) */
#define TIMER4_REGOFF_VALUE        (TIMER4_REGBASE_ADDR + 0x004)    /* Value (R/O) */
#define TIMER4_REGOFF_CTRL         (TIMER4_REGBASE_ADDR + 0x008)    /* Control (R/W) */
#define TIMER4_REGOFF_CLEAR        (TIMER4_REGBASE_ADDR + 0x00C)    /* Clear (W/O) */
#define TIMER4_REGOFF_INTSTATUS    (TIMER4_REGBASE_ADDR + 0x010)    /* INT STATUS (R/O) */


#endif

#endif /* end of onchiprom.h */

