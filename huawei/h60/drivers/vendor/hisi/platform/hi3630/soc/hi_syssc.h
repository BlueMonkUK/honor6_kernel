/******************************************************************************/
/*  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_syssc.h */
/* Version       : 2.0 */
/* Author        : xujingcui*/
/* Created       : 2013-09-25*/
/* Last Modified : */
/* Description   :  The C union definition file for the module syssc*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Author        : xujingcui*/
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_SYSSC_H__
#define __HI_SYSSC_H__

/*
 * Project: hi
 * Module : syssc
 */

#ifndef HI_SET_GET
#define HI_SET_GET(a0,a1,a2,a3,a4)
#endif

/********************************************************************************/
/*    syssc �Ĵ���ƫ�ƶ��壨��Ŀ��_ģ����_�Ĵ�����_OFFSET)        */
/********************************************************************************/
#define    HI_CRG_CLKEN3_OFFSET                              (0x18) /* ʱ��ʹ��3 */
#define    HI_CRG_CLKDIS3_OFFSET                             (0x1C) /* ʱ�ӹر�3 */
#define    HI_CRG_CLKSTAT3_OFFSET                            (0x20) /* ʱ��״̬3 */
#define    HI_CRG_CLKEN4_OFFSET                              (0x24) /* ʱ��ʹ��4 */
#define    HI_CRG_CLKDIS4_OFFSET                             (0x28) /* ʱ�ӹر�4 */
#define    HI_CRG_CLKSTAT4_OFFSET                            (0x2C) /* ʱ�ӿ���״̬4 */
#define    HI_CRG_CLKEN5_OFFSET                              (0x30) /* ʱ��ʹ��5��BBP�� */
#define    HI_CRG_CLKDIS5_OFFSET                             (0x34) /* ʱ�ӹرտ��ƼĴ���0(BBP) */
#define    HI_CRG_CLKSTAT5_OFFSET                            (0x38) /* ʱ��״̬���ƼĴ���0(BBP) */
#define    HI_CRG_SRSTEN1_OFFSET                             (0x60) /* ��λ����ʹ��1 */
#define    HI_CRG_SRSTDIS1_OFFSET                            (0x64) /* ��λ����ر�1 */
#define    HI_CRG_SRSTSTAT1_OFFSET                           (0x68) /* ��λ����״̬1 */
#define    HI_CRG_SRSTEN3_OFFSET                             (0x78) /* ��λ����ʹ��3��ABB+BBP�� */
#define    HI_CRG_SRSTDIS3_OFFSET                            (0x7C) /* ��λ����ر�3��ABB+BBP�� */
#define    HI_CRG_SRSTSTAT3_OFFSET                           (0x80) /* ��λ����״̬3��ABB+BBP�� */
#define    HI_CRG_CLKDIV2_OFFSET                             (0x104) /* ʱ�ӷ�Ƶ����2(���߷�Ƶ�� */
#define    HI_CRG_CLKDIV5_OFFSET                             (0x110) /* ʱ�ӷ�Ƶ����5(BBEDSP/CIPHERʱ�ӷ�Ƶ) */
#define    HI_CRG_CLK_SEL2_OFFSET                            (0x144) /* ʱ��Դѡ�����2(TIMER) */
#define    HI_CRG_CLK_SEL3_OFFSET                            (0x148) /* ʱ��Դѡ�����3��BBP�� */
#define    HI_CRG_A9PLL_CFG0_OFFSET                          (0x200) /* A9PLL���üĴ���0 */
#define    HI_CRG_A9PLL_CFG1_OFFSET                          (0x204) /* A9PLL���üĴ���1 */
#define    HI_CRG_A9PLL_CFG2_OFFSET                          (0x208) /* A9PLL���üĴ���2 */
#define    HI_CRG_DSPPLL_CFG0_OFFSET                         (0x20C) /* BBEPLL���üĴ���0 */
#define    HI_CRG_DSPPLL_CFG1_OFFSET                         (0x210) /* BBEPLL���üĴ���1 */
#define    HI_CRG_DSPPLL_CFG2_OFFSET                         (0x214) /* BBEPLL���üĴ���2 */
#define    HI_CRG_BBPPLL_CFG0_OFFSET                         (0x224) /* BBPPLL���üĴ���0 */
#define    HI_CRG_BBPPLL_CFG1_OFFSET                         (0x228) /* BBPPLL���üĴ���1 */
#define    HI_CRG_BBPPLL_CFG2_OFFSET                         (0x22C) /* BBPPLL���üĴ���2 */
#define    HI_SC_CTRL0_OFFSET                                (0x400) /* ModemA9����Remap���ƼĴ��� */
#define    HI_SC_CTRL2_OFFSET                                (0x408) /* ���Ź����üĴ����� */
#define    HI_SC_CTRL3_OFFSET                                (0x40C) /* �������üĴ����� */
#define    HI_SC_CTRL4_OFFSET                                (0x410) /* CICOM�ӽ���ģʽ���üĴ����� */
#define    HI_SC_CTRL5_OFFSET                                (0x414) /* �����Ĵ��� */
#define    HI_SC_CTRL6_OFFSET                                (0x418) /* �����Ĵ��� */
#define    HI_SC_CTRL9_OFFSET                                (0x424) /* BBE16������ַ���üĴ��� */
#define    HI_SC_CTRL10_OFFSET                               (0x428) /* �����Ĵ��� */
#define    HI_SC_CTRL11_OFFSET                               (0x42C) /* Modem A9���üĴ����� */
#define    HI_SC_CTRL12_OFFSET                               (0x430) /* �����Ĵ��� */
#define    HI_SC_CTRL13_OFFSET                               (0x434) /* BBE16���üĴ��� */
#define    HI_SC_CTRL17_OFFSET                               (0x444) /* HPM���ƼĴ��� */
#define    HI_SC_CTRL20_OFFSET                               (0x450) /* �Զ��ſ�ʱ�ӿ��ƼĴ���0 */
#define    HI_SC_CTRL21_OFFSET                               (0x454) /* �Զ��ſ�ʱ�ӿ��ƼĴ���1 */
#define    HI_SC_CTRL22_OFFSET                               (0x458) /* BBE16 memory ʱ�����üĴ��� */
#define    HI_SC_CTRL23_OFFSET                               (0x45C) /* SOC memory ʱ�����üĴ��� */
#define    HI_SC_CTRL24_OFFSET                               (0x460) /* memory ʱ�����üĴ���2��CCPU+L2)�� */
#define    HI_SC_CTRL25_OFFSET                               (0x464) /* SOC �����ź�ѡ��Ĵ���(������ */
#define    HI_SC_CTRL26_OFFSET                               (0x468) /* SOC �����ź�bypass�Ĵ���(������ */
#define    HI_SC_CTRL28_OFFSET                               (0x470) /* ��ַ�����׵�ַ(ACP�ռ���ʼ��ַ) */
#define    HI_SC_CTRL29_OFFSET                               (0x474) /* ��ַ����β��ַ(ACP�ռ������ַ) */
#define    HI_SC_CTRL30_OFFSET                               (0x478) /* �����Ĵ��� */
#define    HI_SC_CTRL45_OFFSET                               (0x4B4) /* uicc���ƼĴ��� */
#define    HI_SC_CTRL52_OFFSET                               (0x4D0) /* BBE16���������жϼĴ����� */
#define    HI_SC_CTRL55_OFFSET                               (0x4DC) /* memory�͹������üĴ���0 */
#define    HI_SC_CTRL56_OFFSET                               (0x4E0) /* BBP LBUS���ƼĴ����� */
#define    HI_SC_CTRL57_OFFSET                               (0x4E4) /* BBP�������ƼĴ����� */
#define    HI_SC_CTRL68_OFFSET                               (0x510) /* memory�͹������üĴ���2 */
#define    HI_SC_CTRL69_OFFSET                               (0x514) /* ϵͳ�����ַ���ʹ��ܼĴ��� */
#define    HI_SC_CTRL70_OFFSET                               (0x518) /* �������ȼ����üĴ���0 */
#define    HI_SC_CTRL71_OFFSET                               (0x51C) /* �������ȼ����üĴ���1 */
#define    HI_SC_CTRL72_OFFSET                               (0x520) /* �������ȼ����üĴ���2��DW_axi_mst) */
#define    HI_SC_CTRL73_OFFSET                               (0x524) /* ���������ʿռ���ƼĴ���0 */
#define    HI_SC_CTRL74_OFFSET                               (0x528) /* ���������ʿռ���ƼĴ���1 */
#define    HI_SC_CTRL75_OFFSET                               (0x52C) /* ���������ʿռ���ƼĴ���2 */
#define    HI_SC_CTRL76_OFFSET                               (0x530) /* ���������ʿռ���ƼĴ���3 */
#define    HI_SC_CTRL77_OFFSET                               (0x534) /* ���������ʿռ���ƼĴ���4 */
#define    HI_SC_CTRL78_OFFSET                               (0x538) /* ���������ʿռ���ƼĴ���5 */
#define    HI_SC_CTRL79_OFFSET                               (0x53C) /* ���������ʿռ���ƼĴ���6 */
#define    HI_SC_CTRL80_OFFSET                               (0x540) /* ���������ʿռ���ƼĴ���7 */
#define    HI_SC_CTRL103_OFFSET                              (0x59C) /* slave active���μĴ���0 */
#define    HI_SC_CTRL104_OFFSET                              (0x5A0) /* �����Ĵ��� */
#define    HI_SC_CTRL105_OFFSET                              (0x5A4) /* slave active���μĴ���2 */
#define    HI_SC_STAT1_OFFSET                                (0x604) /* Modem A9״̬�Ĵ��� */
#define    HI_SC_STAT2_OFFSET                                (0x608) /* BBE16 DSP״̬�Ĵ��� */
#define    HI_SC_STAT3_OFFSET                                (0x60C) /* HPM״̬�Ĵ��� */
#define    HI_SC_STAT5_OFFSET                                (0x614) /* BBE16�ж�״̬�Ĵ��� */
#define    HI_SC_STAT6_OFFSET                                (0x618) /* Modem A9У��״̬�Ĵ��� */
#define    HI_SC_STAT9_OFFSET                                (0x624) /* AXI������״̬�Ĵ���0 */
#define    HI_SC_STAT10_OFFSET                               (0x628) /* AXI������״̬�Ĵ���1 */
#define    HI_SC_STAT15_OFFSET                               (0x63C) /* slave active״̬�Ĵ��� */
#define    HI_SC_STAT22_OFFSET                               (0x658) /* AXI������״̬�Ĵ���2(BBPHY) */
#define    HI_SC_STAT26_OFFSET                               (0x668) /* master״̬�Ĵ��� */
#define    HI_SC_STAT27_OFFSET                               (0x66C) /* slave active״̬�Ĵ���0 */
#define    HI_SC_STAT29_OFFSET                               (0x674) /* slave active״̬�Ĵ���2 */
#define    HI_SC_STAT32_OFFSET                               (0x680) /* BBP����״̬�Ĵ����� */
#define    HI_SC_STAT35_OFFSET                               (0x68C) /* ��ȫKey�Ĵ���0 */
#define    HI_SC_STAT36_OFFSET                               (0x690) /* ��ȫKey�Ĵ���1 */
#define    HI_SC_STAT37_OFFSET                               (0x694) /* ��ȫKey�Ĵ���2 */
#define    HI_SC_STAT38_OFFSET                               (0x698) /* ��ȫKey�Ĵ���3 */
#define    HI_SC_STAT41_OFFSET                               (0x6A4) /* master���ʴ���״̬�Ĵ���0�� */
#define    HI_SC_STAT42_OFFSET                               (0x6A8) /* master���ʴ���״̬�Ĵ���1�� */
#define    HI_SC_STAT43_OFFSET                               (0x6AC) /* ahb������ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT44_OFFSET                               (0x6B0) /* apb������ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT46_OFFSET                               (0x6B8) /* glb��m1�ڣ�dw_axi_bbphy��д���ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT47_OFFSET                               (0x6BC) /* glb��m1�ڣ�dw_axi_bbphy�������ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT48_OFFSET                               (0x6C0) /* glb��m2�ڣ�dw_axi_mst��д���ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT49_OFFSET                               (0x6C4) /* glb��m2�ڣ�dw_axi_mst�������ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT50_OFFSET                               (0x6C8) /* glb��m3�ڣ�app a9 m0�ڣ�д���ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT51_OFFSET                               (0x6CC) /* glb��m3�ڣ�app a9 m0�ڣ������ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT52_OFFSET                               (0x6D0) /* glb��m4�ڣ�mdm a9 m0�ڣ�д���ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT53_OFFSET                               (0x6D4) /* glb��m4�ڣ�mdm a9 m0�ڣ������ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT54_OFFSET                               (0x6D8) /* glb��m5�ڣ�reserved��д���ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT55_OFFSET                               (0x6DC) /* glb��m5�ڣ�reserved�������ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT56_OFFSET                               (0x6E0) /* glb��m6�ڣ�hifi��д���ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT57_OFFSET                               (0x6E4) /* glb��m6�ڣ�hifi�������ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT62_OFFSET                               (0x6F8) /* bbphy��m1�ڣ�dw_axi_glb��д���ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT63_OFFSET                               (0x6FC) /* bbphy��m1�ڣ�dw_axi_glb�������ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT64_OFFSET                               (0x700) /* bbphy��m2�ڣ�dsp0��д���ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT65_OFFSET                               (0x704) /* bbphy��m2�ڣ�dsp0�������ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT66_OFFSET                               (0x708) /* bbphy��m3�ڣ�bbp��д���ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT67_OFFSET                               (0x70C) /* bbphy��m3�ڣ�bbp�������ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT68_OFFSET                               (0x710) /* bbphy��m4�ڣ�bbp��д���ʴ����ַ�Ĵ����� */
#define    HI_SC_STAT69_OFFSET                               (0x714) /* bbphy��m4�ڣ�bbp�������ʴ����ַ�Ĵ����� */
#define    HI_PWR_CTRL2_OFFSET                               (0xC08) /* TCXO���ƼĴ��� */
#define    HI_PWR_CTRL4_OFFSET                               (0xC10) /* ISOCELL����ʹ�ܿ��ƼĴ��� */
#define    HI_PWR_CTRL5_OFFSET                               (0xC14) /* ISOCELL����رտ��ƼĴ��� */
#define    HI_PWR_CTRL6_OFFSET                               (0xC18) /* MTCMOS�ϵ�ʹ�ܿ��ƼĴ��� */
#define    HI_PWR_CTRL7_OFFSET                               (0xC1C) /* MTCMOS�µ�ʹ�ܿ��ƼĴ��� */
#define    HI_PWR_STAT1_OFFSET                               (0xE04) /* MTCMOS_RDY״ָ̬ʾ�Ĵ��� */
#define    HI_PWR_STAT3_OFFSET                               (0xE0C) /* ISOCELL�������״̬�Ĵ��� */
#define    HI_PWR_STAT4_OFFSET                               (0xE10) /* MTCMOS���µ����״̬�Ĵ��� */
#define    HI_SEC_CTRL0_OFFSET                               (0xF04) /* ��ȫ���ƼĴ���0(slave�ӿڣ� */
#define    HI_SEC_CTRL1_OFFSET                               (0xF08) /* ��ȫ���ƼĴ���1(GLB_MASTER�ӿڣ� */
#define    HI_SEC_CTRL2_OFFSET                               (0xF0C) /* ��ȫ���ƼĴ���2 */
#define    HI_SEC_CTRL3_OFFSET                               (0xF10) /* ��ȫ���ƼĴ���3 */
#define    HI_SEC_CTRL4_OFFSET                               (0xF14) /* ��ȫ���ƼĴ���4 */
#define    HI_SEC_CTRL5_OFFSET                               (0xF18) /* ��ȫ���ƼĴ���5 */


#ifndef __ASSEMBLY__

/********************************************************************************/
/*    syssc �Ĵ������壨��Ŀ��_ģ����_�Ĵ�����_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 24; /* [23..0] ���� */
        unsigned int    bbe_refclk_en              : 1; /* [24..24] BBE16����ο�ʱ��ʹ�ܿ��ƣ���AP��ʹ��)��1 - ʹ�ܣ�0 - ��Ӱ�� */
        unsigned int    a9_refclk_en               : 1; /* [25..25] A9����������ο�ʱ��ʹ�ܿ��ƣ���AP��ʹ�ã���1 - ʹ�ܣ�0 - ��Ӱ�� */
        unsigned int    hpm_clk_en                 : 1; /* [26..26] HPMʱ��ʹ�ܿ��ơ�1 - ʹ�ܣ�0 - ��Ӱ�� */
        unsigned int    reserved_0                 : 5; /* [31..27] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKEN3_T;    /* ʱ��ʹ��3 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 24; /* [23..0] ���� */
        unsigned int    bbe_refclk_dis             : 1; /* [24..24] BBE16����ο�ʱ�ӹرտ��ƣ���AP��ʹ�ã���1 - ʹ�ܣ�0 - ��Ӱ�� */
        unsigned int    a9_refclk_dis              : 1; /* [25..25] A9����������ο�ʱ�ӹرտ��ƣ���AP��ʹ�ã���1 - ʹ�ܣ�0 - ��Ӱ�� */
        unsigned int    hpm_clk_dis                : 1; /* [26..26] HPMʱ�ӹرտ��ơ�1 - ʹ�ܣ�0 - ��Ӱ�� */
        unsigned int    reserved_0                 : 5; /* [31..27] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKDIS3_T;    /* ʱ�ӹر�3 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 24; /* [23..0] ���� */
        unsigned int    bbe_refclk_status          : 1; /* [24..24] BBE16����ο�ʱ��״̬���ƣ���AP��ʹ�ã���1 - ʹ�ܣ�0 - �ر�(default) */
        unsigned int    a9_refclk_status           : 1; /* [25..25] A9����������ο�ʱ��״̬���ƣ���AP��ʹ�ã���1 - ʹ�ܣ�0 - �ر�(default) */
        unsigned int    hpm_clk_status             : 1; /* [26..26] HPMʱ��״̬���ơ�1 - ʹ�ܣ�0 - �ر�(default) */
        unsigned int    reserved_0                 : 5; /* [31..27] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKSTAT3_T;    /* ʱ��״̬3 */

typedef union
{
    struct
    {
        unsigned int    bbe_pd_clk_en              : 1; /* [0..0] BBE16 DSP��ϵͳʱ��ʹ�ܿ��ơ�0����Ӱ�죻1��ʱ��ʹ�� */
        unsigned int    bbe_core_clk_en            : 1; /* [1..1] BBE16 DSP��ϵͳCOREʱ��ʹ�ܿ��ơ�0����Ӱ�죻1��ʱ��ʹ�ܸ�bitֻ����DSP COREʱ�ӡ� */
        unsigned int    reserved_2                 : 1; /* [2..2] ���� */
        unsigned int    mcpu_pd_clk_en             : 1; /* [3..3] ModemA9��Դ��ʱ��ʹ�ܿ��ơ�0����Ӱ�죻1��ʱ��ʹ�� */
        unsigned int    l2c_clk_en                 : 1; /* [4..4] L2Cacheʱ��ʹ�ܿ��ơ�0����Ӱ�죻1��ʱ��ʹ�� ��bitͬʱ�����͸�AP��ʱ�ӡ� */
        unsigned int    reserved_1                 : 1; /* [5..5] ���� */
        unsigned int    mcpu_mbist_clk_en          : 1; /* [6..6] ModemA9 MemoryBistʱ��ʹ�ܿ��ơ�0����Ӱ�죻1��ʱ��ʹ�� */
        unsigned int    reserved_0                 : 1; /* [7..7] ������ */
        unsigned int    cicom0_clk_en              : 1; /* [8..8] CICOM0ʱ��ʹ�ܡ�0����Ӱ�죻1��ʱ��ʹ�� */
        unsigned int    cicom1_clk_en              : 1; /* [9..9] CICOM1ʱ��ʹ�ܡ�1��ʱ��ʹ�� 0����Ӱ�죻 */
        unsigned int    ipf_clk_en                 : 1; /* [10..10] IPFʱ��ʹ�ܿ��ơ�1��ʱ��ʹ�� 0����Ӱ�죻 */
        unsigned int    upacc_clk_en               : 1; /* [11..11] UPACCʱ��ʹ�ܡ�1��ʱ��ʹ�� 0����Ӱ�죻 */
        unsigned int    sci0_clk_en                : 1; /* [12..12] SCI0ʱ��ʹ�ܿ��ƣ���AP��ʹ�ã���1��ʱ��ʹ�� 0����Ӱ�죻 */
        unsigned int    sci1_clk_en                : 1; /* [13..13] SCI1ʱ��ʹ�ܿ��ƣ���AP��ʹ�ã���0����Ӱ�죻1��ʱ��ʹ�� */
        unsigned int    uicc_clk_en                : 1; /* [14..14] UICCģ��ʱ��ʹ�ܿ��ơ�0����Ӱ�죻1��ʱ��ʹ�� */
        unsigned int    uart_clk_en                : 1; /* [15..15] UART����ʱ��ʹ�ܿ��ơ�0����Ӱ�죻1��ʱ��ʹ�� */
        unsigned int    bbe_vic_clk_en             : 1; /* [16..16] BBE VIC�жϿ�����ʱ��ʹ�ܿ��ơ�0����Ӱ�죻1��ʱ��ʹ�� */
        unsigned int    cipher_clk_en              : 1; /* [17..17] CIPHER�жϿ�����ʱ��ʹ�ܿ��ơ�0����Ӱ�죻1��ʱ��ʹ�� */
        unsigned int    edmac_clk_en               : 1; /* [18..18] EDMACʱ��ʹ�ܿ��ơ�0����Ӱ�죻1��ʱ��ʹ�� */
        unsigned int    ipcm_clk_en                : 1; /* [19..19] IPCMʱ��ʹ�ܿ��ơ�0����Ӱ�죻1��ʱ��ʹ�� */
        unsigned int    hsuart_clk_en              : 1; /* [20..20] HSUARTʱ��ʹ�ܿ��ơ�0����Ӱ�죻1��ʱ��ʹ�� */
        unsigned int    aximem_clk_en              : 1; /* [21..21] on-chip memoryʱ��ʹ�ܿ��ơ�0����Ӱ�죻1��ʱ��ʹ�� */
        unsigned int    aximon_clk_en              : 1; /* [22..22] AXI monitorʱ��ʹ�ܿ��ơ�0����Ӱ�죻1��ʱ��ʹ�� */
        unsigned int    timer_clk_en               : 8; /* [30..23] TIMER0-7ʱ��ʹ�ܿ��ơ�0����Ӱ�죻1��ʱ��ʹ�� */
        unsigned int    wdog_clk_en                : 1; /* [31..31] WDOGʱ��ʹ�ܿ��ơ�0����Ӱ�죻1��ʱ��ʹ�ܡ� */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKEN4_T;    /* ʱ��ʹ��4 */

typedef union
{
    struct
    {
        unsigned int    dsp0_pd_clk_dis            : 1; /* [0..0] DSP��ϵͳʱ�ӹرտ��ơ�0����Ӱ�죻1��ʱ�ӹر� */
        unsigned int    dsp0_core_clk_dis          : 1; /* [1..1] DSP��ϵͳCOREʱ�ӹرտ��ơ�0����Ӱ�죻1��ʱ�ӹر� */
        unsigned int    dsp0_dbg_clk_dis           : 1; /* [2..2] DSP��ϵͳDBGʱ�ӹرտ��ơ�0����Ӱ�죻1��ʱ�ӹر� */
        unsigned int    mcpu_pd_clk_dis            : 1; /* [3..3] ModemA9��Դ��ʱ�ӹرտ��ơ�0����Ӱ�죻1��ʱ�ӹر� */
        unsigned int    l2c_clk_dis                : 1; /* [4..4] L2Cacheʱ�ӹرտ��ơ�0����Ӱ�죻1��ʱ�ӹر� */
        unsigned int    mcpu_dbg_clk_dis           : 1; /* [5..5] ModemA9 Debugʱ�ӹرտ��ơ�0����Ӱ�죻1��ʱ�ӹر� */
        unsigned int    mcpu_mbist_clk_dis         : 1; /* [6..6] ModemA9 MemoryBistʱ�ӹرտ��ơ�0����Ӱ�죻1��ʱ�ӹر� */
        unsigned int    reserved                   : 1; /* [7..7] ���� */
        unsigned int    cicom0_clk_dis             : 1; /* [8..8] CICOM0ʱ�ӹرտ��ơ�0����Ӱ�죻1��ʱ�ӹر� */
        unsigned int    cicom1_clk_dis             : 1; /* [9..9] CICOM1ʱ�ӹرտ��ơ�0����Ӱ�죻1��ʱ�ӹر� */
        unsigned int    ipf_clk_dis                : 1; /* [10..10] IPFʱ�ӹرտ��ơ�0����Ӱ�죻1��ʱ�ӹر� */
        unsigned int    upacc_clk_dis              : 1; /* [11..11] UPACCʱ�ӹرտ��ơ�0����Ӱ�죻1��ʱ�ӹر� */
        unsigned int    sci0_clk_dis               : 1; /* [12..12] SCI0ʱ�ӹرտ��ơ�                            0����Ӱ�죻                                  1��ʱ�ӹر� */
        unsigned int    sci1_clk_dis               : 1; /* [13..13] SCI1ʱ�ӹرտ��ơ�                            0����Ӱ�죻                                  1��ʱ�ӹر� */
        unsigned int    uicc_clk_dis               : 1; /* [14..14] UICCģ��ʱ�ӹرտ��ơ�                            0����Ӱ�죻                                  1��ʱ�ӹر� */
        unsigned int    uart_clk_dis               : 1; /* [15..15] UART����ʱ�ӹرտ��ơ�                            0����Ӱ�죻                                  1��ʱ�ӹر� */
        unsigned int    bbe_vic_clk_dis            : 1; /* [16..16] BBE VIC�жϿ�����ʱ�ӹرտ��ơ�                            0����Ӱ�죻                                  1��ʱ�ӹر� */
        unsigned int    cipher_clk_dis             : 1; /* [17..17] CIPHER�жϿ�����ʱ�ӹرտ��ơ�                            0����Ӱ�죻                                  1��ʱ�ӹر� */
        unsigned int    edmac_clk_dis              : 1; /* [18..18] EDMACʱ�ӹرտ��ơ�                            0����Ӱ�죻                                  1��ʱ�ӹر� */
        unsigned int    ipcm_clk_dis               : 1; /* [19..19] IPCMʱ�ӹرտ��ơ�                            0����Ӱ�죻                                  1��ʱ�ӹر� */
        unsigned int    hsuart_clk_dis             : 1; /* [20..20] HSUARTʱ�ӹرտ��ơ�                            0����Ӱ�죻                                  1��ʱ�ӹر� */
        unsigned int    aximem_clk_dis             : 1; /* [21..21] on-chip memoryʱ�ӹرտ��ơ�                            0����Ӱ�죻                                  1��ʱ�ӹر� */
        unsigned int    aximon_clk_dis             : 1; /* [22..22] AXI monitorʱ�ӹرտ��ơ�                            0����Ӱ�죻                                  1��ʱ�ӹر� */
        unsigned int    timer_clk_dis              : 8; /* [30..23] TIMER0-7ʱ�ӹرտ��ơ�                            0����Ӱ�죻                                  1��ʱ�ӹر� */
        unsigned int    wdog_clk_dis               : 1; /* [31..31] WDOGʱ�ӹرտ��ơ�                            0����Ӱ�죻                                  1��ʱ�ӹر� */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKDIS4_T;    /* ʱ�ӹر�4 */

typedef union
{
    struct
    {
        unsigned int    bbe_pd_clk_status          : 1; /* [0..0] BBEDSP��ϵͳʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    bbe_core_clk_status        : 1; /* [1..1] BBEDSP��ϵͳCOREʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    bbe_dbg_clk_status         : 1; /* [2..2] BBEDSP��ϵͳDBGʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� ��bitδʹ�á� */
        unsigned int    mcpu_pd_clk_status         : 1; /* [3..3] ModemA9��Դ��ʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    l2c_clk_status             : 1; /* [4..4] L2Cacheʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    mcpu_dbg_clk_status        : 1; /* [5..5] ModemA9 Debugʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� ��bitδʹ�á� */
        unsigned int    mcpu_mbist_clk_status      : 1; /* [6..6] ModemA9 MemoryBistʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    reserved                   : 1; /* [7..7] ���� */
        unsigned int    cicom0_clk_status          : 1; /* [8..8] CICOM0ʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    cicom1_clk_status          : 1; /* [9..9] CICOM1ʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    ipf_clk_status             : 1; /* [10..10] IPFʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    upacc_clk_status           : 1; /* [11..11] UPACCʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    sci0_clk_status            : 1; /* [12..12] SCI0ʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    sci1_clk_status            : 1; /* [13..13] SCI1ʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    uicc_clk_status            : 1; /* [14..14] UICCģ��ʱ��״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    uart_clk_status            : 1; /* [15..15] UART����ʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    bbe_vic_clk_status         : 1; /* [16..16] BBE VIC�жϿ�����ʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    cipher_clk_status          : 1; /* [17..17] CIPHER������ʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    edmac_clk_status           : 1; /* [18..18] EDMACʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    ipcm_clk_status            : 1; /* [19..19] IPCMʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    hsuart_clk_status          : 1; /* [20..20] HSUARTʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    aximem_clk_status          : 1; /* [21..21] on-chip memoryʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    aximon_clk_status          : 1; /* [22..22] AXI monitorʱ��ʹ����״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    timer_clk_status           : 8; /* [30..23] TIMER0-7ʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
        unsigned int    wdog_clk_status            : 1; /* [31..31] WDOGʱ�ӿ���״̬��0��ʱ�ӹرգ�1��ʱ��ʹ�� */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKSTAT4_T;    /* ʱ�ӿ���״̬4 */

typedef union
{
    struct
    {
        unsigned int    bbp_ao_clk_en              : 1; /* [0..0] BBP AO��ʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    bbpcommon_2a_clk_en        : 1; /* [1..1] BBPCOMMONģ��ʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    ltebbp_pd_clk_en           : 1; /* [2..2] LTE BBP������ʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    g1bbp_pd_clk_en            : 1; /* [3..3] G1 BBP������ʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    g2bbp_pd_clk_en            : 1; /* [4..4] G2 BBP������ʱ��ʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    twbbp_pd_clk_en            : 1; /* [5..5] TW BBP������ʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    wbbp_pd_clk_en             : 1; /* [6..6] WBBP������ʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    irm_pd_clk_en              : 1; /* [7..7] IRM BBP������ʱ��ʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    lbbp_axi_clk_en            : 1; /* [8..8] LTE BBP������AXI�ӿ�ʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    lbbp_pdt_clk_en            : 1; /* [9..9] LTE BBP������ʱ�򲿷�ʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    lbbp_pdf_clk_en            : 1; /* [10..10] LTE BBP������Ƶ�򲿷�ʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    lbbp_tdl_clk_en            : 1; /* [11..11] LTE BBP������TDL���ʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    lbbp_vdl_clk_en            : 1; /* [12..12] LTE BBP������VDL���ʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    lbbp_tds_clk_en            : 1; /* [13..13] LTE BBP������Ƶ�򲿷�ʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    reserved_1                 : 2; /* [15..14] ���� */
        unsigned int    irm_turbo_clk_en           : 1; /* [16..16] IRM BBP������ turboʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    irm_bbc_245m_clk_en        : 1; /* [17..17] IRM BBP������ 245MHzʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    irm_wbbp_122m_clk_en       : 1; /* [18..18] IRM BBP����122MHzʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    twbbp_wbbp_122m_clk_en     : 1; /* [19..19] WTDS/W BBP������TDSģ122MHzʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    twbbp_wbbp_61m_clk_en      : 1; /* [20..20] TDS/W BBP������ WBBP 61MHzʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    twbbp_tbbp_122m_clk_en     : 1; /* [21..21] PDTW BBP������tbbp 122MHzʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    twbbp_tbbp_245m_clk_en     : 1; /* [22..22] PDTW BBP������tbbp 245MHzʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    wbbp_61m_clk_en            : 1; /* [23..23] PDW BBP������61MHzʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    wbbp_122m_clk_en           : 1; /* [24..24] PDW BBP������122MHzʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    wbbp_245m_clk_en           : 1; /* [25..25] PDW BBP������245MHzʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    wbbp_axi_clk_en            : 1; /* [26..26] PDW BBP������AXIʱ��ʹ�ܿ��ơ�1 - ʱ�Ӵ�0 - ��Ӱ�� */
        unsigned int    reserved_0                 : 5; /* [31..27] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKEN5_T;    /* ʱ��ʹ��5��BBP�� */

typedef union
{
    struct
    {
        unsigned int    bbp_ao_clk_dis             : 1; /* [0..0] BBP DRXģ��ʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    bbpcommon_2a_clk_dis       : 1; /* [1..1] BBPCOMMON_2Aģ��ʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    lbbp_pd_clk_dis            : 1; /* [2..2] LTEBBP��Դ��ʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    g1bbp_pd_clk_dis           : 1; /* [3..3] G1BBP��Դ��ʱ��ʹ�رտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    g2bbp_pd_clk_dis           : 1; /* [4..4] G2BBP��Դ��ʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    twbbp_pd_clk_dis           : 1; /* [5..5] TWBBP��Դ��ʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    wbbp_pd_clk_dis            : 1; /* [6..6] WBBP��Դ��ʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    irmbbp_pd_clk_dis          : 1; /* [7..7] IRMBBP��Դ��ʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    lbbp_axi_clk_dis           : 1; /* [8..8] LTEBBP AXIʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    lbbp_pdt_clk_dis           : 1; /* [9..9] LTEBBPʱ�����ʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    lbbp_pdf_clk_dis           : 1; /* [10..10] LTEBBPƵ�����ʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    lbbp_tdl_clk_dis           : 1; /* [11..11] LTEBBP TDL���ʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    lbbp_vdl_clk_dis           : 1; /* [12..12] LTEBBP VDL���ʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    lbbp_tds_clk_dis           : 1; /* [13..13] LTEBBP TDS���ʱ��ʹ�ܿ��ơ�1 - ʱ��ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    reserved_1                 : 2; /* [15..14] ���� */
        unsigned int    irm_turbo_clk_dis          : 1; /* [16..16] IRM turbo���ʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    irm_bbc_245m_clk_dis       : 1; /* [17..17] IRM 245MHz���ʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    irm_wbbp_clk_dis           : 1; /* [18..18] IRM WBBP���ʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    twbbp_wbbp_122m_clk_dis    : 1; /* [19..19] TWBBP wbbp 122MHzʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    twbbp_wbbp_61m_clk_dis     : 1; /* [20..20] TWBBP wbbp 61MHzʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    twbbp_tbbp_122m_clk_dis    : 1; /* [21..21] TWBBP tbbp 122MHzʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    twbbp_tbbp_245m_clk_dis    : 1; /* [22..22] TWBBP tbbp 245MHzʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    wbbp_61m_clk_dis           : 1; /* [23..23] WBBP��Դ��61MHzʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    wbbp_122m_clk_dis          : 1; /* [24..24] WBBP��Դ��122MHzʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    wbbp_245m_clk_dis          : 1; /* [25..25] WBBP��Դ��245MHzʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    wbbp_axi_clk_dis           : 1; /* [26..26] WBBP��Դ��AXIʱ�ӹرտ��ơ�1 - ʱ�ӹرգ�0 - ��Ӱ�졣 */
        unsigned int    reserved_0                 : 5; /* [31..27] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKDIS5_T;    /* ʱ�ӹرտ��ƼĴ���0(BBP) */

typedef union
{
    struct
    {
        unsigned int    bbp_ao_clk_stat            : 1; /* [0..0] BBP DRXģ��ʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    bbpcom_clk_stat            : 1; /* [1..1] BBPCOM_2Aģ��ʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    lbbp_pd_clk_stat           : 1; /* [2..2] LTEBBP��Դ��ʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    g1bbp_pd_clk_stat          : 1; /* [3..3] G1BBP��Դ��ʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    g2bbp_pd_clk_stat          : 1; /* [4..4] G2BBP��Դ��ʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    twbbp_pd_clk_stat          : 1; /* [5..5] TWBBP��Դ��ʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    wbbp_pd_clk_stat           : 1; /* [6..6] WBBP��Դ��ʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    irmbbp_pd_clk_stat         : 1; /* [7..7] IRMBBP��Դ��ʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    lbbp_axi_clk_stat          : 1; /* [8..8] LTEBBP AXIʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    lbbp_pdt_clk_stat          : 1; /* [9..9] LTEBBPʱ�����ʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    lbbp_pdf_clk_stat          : 1; /* [10..10] LTEBBPƵ�����ʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    lbbp_tdl_clk_stat          : 1; /* [11..11] LTEBBP TDL���ʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    lbbp_vdl_clk_stat          : 1; /* [12..12] LTEBBP VDL���ʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    lbbp_tds_clk_stat          : 1; /* [13..13] LTEBBP TDS���ʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    reserved_1                 : 2; /* [15..14] ���� */
        unsigned int    irm_turbo_clk_stat         : 1; /* [16..16] IRM turbo���ʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    irm_bbc_245m_clk_stat      : 1; /* [17..17] IRM 245MHz���ʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    irm_wbbp_clk_stat          : 1; /* [18..18] IRM WBBP���ʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    twbbp_wbbp_122m_clk_stat   : 1; /* [19..19] TWBBP wbbp 122MHzʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    twbbp_wbbp_61m_clk_stat    : 1; /* [20..20] TWBBP wbbp 61MHzʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    twbbp_tbbp_122m_clk_stat   : 1; /* [21..21] TWBBP tbbp 122MHzʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    twbbp_tbbp_245m_clk_stat   : 1; /* [22..22] TWBBP tbbp 245MHzʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    wbbp_61m_clk_stat          : 1; /* [23..23] WBBP��Դ��61MHzʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    wbbp_122m_clk_stat         : 1; /* [24..24] WBBP��Դ��122MHzʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    wbbp_245m_clk_stat         : 1; /* [25..25] WBBP��Դ��245MHzʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    wbbp_axi_clk_stat          : 1; /* [26..26] WBBP��Դ��AXIʱ��״ָ̬ʾ��1 - ʱ��ʹ�ܣ�0 - ʱ�ӹرա� */
        unsigned int    reserved_0                 : 5; /* [31..27] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKSTAT5_T;    /* ʱ��״̬���ƼĴ���0(BBP) */

typedef union
{
    struct
    {
        unsigned int    bbe_pd_srst_en             : 1; /* [0..0] BBEDSP��ϵͳ��λ����ʹ�ܿ��ơ�0����Ӱ��1����λ������Ч */
        unsigned int    bbe_core_srst_en           : 1; /* [1..1] BBEDSP core��λ����ʹ�ܿ��ơ�0����Ӱ��1����λ������Ч */
        unsigned int    bbe_dbg_srst_en            : 1; /* [2..2] BBEDSP DBG��λ����ʹ�ܿ��ơ�0����Ӱ��1����λ������Ч    DSP debug����߼�����bit��0����λ������ƣ�DSP�µ�ʱ����Ҫ���ø�bit��λdebug����߼� */
        unsigned int    reserved_4                 : 3; /* [5..3] ���� */
        unsigned int    moda9_cpu_srst_en          : 1; /* [6..6] MODA9 CPU CORE��λ����ʹ�ܿ��ơ�0����Ӱ��1����λ������Ч */
        unsigned int    moda9_dbg_srst_en          : 1; /* [7..7] MODA9 Debug��λ����ʹ�ܿ��ơ�0����Ӱ��1����λ������Ч */
        unsigned int    moda9_peri_srst_en         : 1; /* [8..8] MODA9 ������λ����ʹ�ܿ��ơ�0����Ӱ��1����λ������Ч */
        unsigned int    moda9_scu_srst_en          : 1; /* [9..9] MODA9 SCU��λ����ʹ�ܿ��ơ�0����Ӱ��1����λ������Ч */
        unsigned int    moda9_ptm_srst_en          : 1; /* [10..10] MODA9 PTM��λ����ʹ�ܿ��ơ�0����Ӱ��1����λ������Ч */
        unsigned int    moda9_wd_srst_en           : 1; /* [11..11] MODA9 WD��λ����ʹ�ܿ��ơ�0����Ӱ��1����λ������Ч */
        unsigned int    reserved_3                 : 3; /* [14..12] ���� */
        unsigned int    mdma9_pd_srst_en           : 1; /* [15..15] MDMA9�ϵ縴λ����ʹ�ܿ��ơ�0����Ӱ��1����λ������Ч */
        unsigned int    reserved_2                 : 10; /* [25..16] ���� */
        unsigned int    uicc_srst_en               : 1; /* [26..26] UICC��λ����ʹ�ܿ��ơ�0����Ӱ��1����λ������Ч */
        unsigned int    reserved_1                 : 2; /* [28..27] ���� */
        unsigned int    amon_soc_srst_en           : 1; /* [29..29] SOC AMONITOR��λ����ʹ�ܿ��ơ�0����Ӱ��1����λ������Ч */
        unsigned int    reserved_0                 : 1; /* [30..30] ���� */
        unsigned int    hpm_srst_en                : 1; /* [31..31] HPM��λ����ʹ�ܿ��ơ�0����Ӱ��1����λ������Ч */
    } bits;
    unsigned int    u32;
}HI_CRG_SRSTEN1_T;    /* ��λ����ʹ��1 */

typedef union
{
    struct
    {
        unsigned int    bbe_pd_srst_dis            : 1; /* [0..0] BBEDSP��ϵͳ��λ����رտ��ơ�0����Ӱ�죻1����λ������Ч */
        unsigned int    bbe_core_srst_dis          : 1; /* [1..1] BBEDSP core��λ����ʹ�رտ��ơ�0����Ӱ�죻1����λ������Ч */
        unsigned int    bbe_dbg_srst_dis           : 1; /* [2..2] BBEDSP DBG��λ����رտ��ơ�0����Ӱ�죻1����λ������ЧDSP debug����߼�����bit��0����λ������ƣ�DSP�µ�ʱ����Ҫ���ø�bit��λdebug����߼� */
        unsigned int    reserved_4                 : 3; /* [5..3] ���� */
        unsigned int    moda9_core_srst_dis        : 1; /* [6..6] MODA9 CPU CORE��λ����رտ��ơ�0����Ӱ�죻1����λ������Ч */
        unsigned int    moda9_dbg_srst_dis         : 1; /* [7..7] MODA9 Debug��λ����رտ��ơ�0����Ӱ�죻1����λ������Ч */
        unsigned int    moda9_peri_srst_dis        : 1; /* [8..8] MODA9 ������λ����رտ��ơ�0����Ӱ�죻1����λ������Ч */
        unsigned int    moda9_scu_srst_dis         : 1; /* [9..9] MODA9 SCU��λ����رտ��ơ�0����Ӱ�죻1����λ������Ч */
        unsigned int    moda9_ptm_srst_dis         : 1; /* [10..10] MODA9 PTM��λ����رտ��ơ�0����Ӱ�죻1����λ������Ч */
        unsigned int    moda9_wd_srst_dis          : 1; /* [11..11] MODA9 WD��λ����رտ��ơ�0����Ӱ�죻1����λ������Ч */
        unsigned int    reserved_3                 : 3; /* [14..12] ���� */
        unsigned int    mdma9_pd_srst_dis          : 1; /* [15..15] MDMA9�ϵ縴λ����رտ��ơ�0����Ӱ��1����λ������Ч */
        unsigned int    reserved_2                 : 10; /* [25..16] ���� */
        unsigned int    uicc_srst_dis              : 1; /* [26..26] UICC��λ����رտ��ơ�0����Ӱ��1����λ������Ч */
        unsigned int    reserved_1                 : 2; /* [28..27] ���� */
        unsigned int    amon_soc_srst_dis          : 1; /* [29..29] SOC AMONITOR��λ����رտ��ơ�0����Ӱ�죻1����λ������Ч */
        unsigned int    reserved_0                 : 1; /* [30..30] ���� */
        unsigned int    hpm_srst_dis               : 1; /* [31..31] HPMC��λ����رտ��ơ�0����Ӱ��1����λ������Ч */
    } bits;
    unsigned int    u32;
}HI_CRG_SRSTDIS1_T;    /* ��λ����ر�1 */

typedef union
{
    struct
    {
        unsigned int    bbe_pd_srst_status         : 1; /* [0..0] BBEDSP��ϵͳ��λ����״̬��0����λ������Ч��1����λ������Ч */
        unsigned int    bbe_core_srst_status       : 1; /* [1..1] BBEDSP core��λ����״̬��0����λ������Ч��1����λ������Ч */
        unsigned int    bbe_dbg_srst_status        : 1; /* [2..2] BBEDSP DBG��λ����״̬��0����λ������Ч��1����λ������ЧDSP debug����߼�����bit��0����λ������ƣ�DSP�µ�ʱ����Ҫ���ø�bit��λdebug����߼� */
        unsigned int    reserved_4                 : 3; /* [5..3] ���� */
        unsigned int    moda9_core_srst_status     : 1; /* [6..6] MODA9 CPU CORE��λ����״̬��0����λ������Ч��1����λ������Ч */
        unsigned int    moda9_dbg_srst_status      : 1; /* [7..7] MODA9 Debug��λ����״̬��0����λ������Ч��1����λ������Ч */
        unsigned int    moda9_peri_srst_status     : 1; /* [8..8] MODA9 ������λ����״̬��0����λ������Ч��1����λ������Ч */
        unsigned int    moda9_scu_srst_status      : 1; /* [9..9] MODA9 SCU��λ����״̬��0����λ������Ч��1����λ������Ч */
        unsigned int    moda9_ptm_srst_status      : 1; /* [10..10] MODA9 PTM��λ����״̬��0����λ������Ч��1����λ������Ч */
        unsigned int    moda9_wd_srst_status       : 1; /* [11..11] MODA9 WD��λ����״̬��0����λ������Ч��1����λ������Ч */
        unsigned int    reserved_3                 : 3; /* [14..12] ���� */
        unsigned int    mdma9_pd_srst_status       : 1; /* [15..15] MDMA9�ϵ縴λ����״̬��0����λ������Ч��1����λ������Ч */
        unsigned int    reserved_2                 : 10; /* [25..16] ���� */
        unsigned int    uicc_srst_status           : 1; /* [26..26] UICC��λ����״̬��0����λ������Ч��1����λ������Ч */
        unsigned int    reserved_1                 : 2; /* [28..27] ���� */
        unsigned int    amon_soc_srst_status       : 1; /* [29..29] SOC AMONITOR��λ����״̬��0����λ������Ч��1����λ������Ч */
        unsigned int    reserved_0                 : 1; /* [30..30] ���� */
        unsigned int    hpm_srst_status            : 1; /* [31..31] HPM��λ����״̬��0����λ������Ч��1����λ������Ч */
    } bits;
    unsigned int    u32;
}HI_CRG_SRSTSTAT1_T;    /* ��λ����״̬1 */

typedef union
{
    struct
    {
        unsigned int    bbp_crg_srst_en            : 1; /* [0..0] BBPʱ�Ӹ�λģ����λ����ʹ�ܿ��ơ�0 - ��Ӱ��1 - ��λ��Ч */
        unsigned int    bbpcommon_2a_srst_en       : 1; /* [1..1] BBPCOMMON_2A��λ����ʹ�ܿ��ơ�0 - ��Ӱ��1 - ��λ��Ч */
        unsigned int    lbbp_pd_srst_en            : 1; /* [2..2] LTEBBP����������������λ����ʹ�ܿ��ơ�0 - ��Ӱ��1 - ��λ��Ч */
        unsigned int    g1bbp_pd_srst_en           : 1; /* [3..3] G1BBP������������λ����ʹ�ܿ��ơ�0 - ��Ӱ��1 - ��λ��Ч */
        unsigned int    g2bbp_pd_srst_en           : 1; /* [4..4] G2BBP������������λ����ʹ�ܿ��ơ�0 - ��Ӱ��1 - ��λ��Ч */
        unsigned int    twbbp_pd_srst_en           : 1; /* [5..5] TWBBP����������������λ����ʹ�ܿ��ơ�0 - ��Ӱ��1 - ��λ��Ч */
        unsigned int    wbbp_pd_srst_en            : 1; /* [6..6] WBBP����������������λ����ʹ�ܿ��ơ�0 - ��Ӱ��1 - ��λ��Ч */
        unsigned int    irm_pd_srst_en             : 1; /* [7..7] IRM������������λ����ʹ�ܿ��ơ�0 - ��Ӱ��1 - ��λ��Ч */
        unsigned int    bbc_lbbp_pdt_srst_en       : 1; /* [8..8] BBPCOMMON_2A LBBP PDT��λԴ���ơ�0 - ��Ӱ��1 - ��bit��1 */
        unsigned int    bbc_lbbp_pdf_srst_en       : 1; /* [9..9] BBPCOMMON_2A LBBP PDF��λԴ���ơ�0 - ��Ӱ��1 - ��bit��1 */
        unsigned int    bbc_lbbp_tdl_srst_en       : 1; /* [10..10] BBPCOMMON_2A LBBP TDL��λԴ���ơ�0 - ��Ӱ��1 - ��bit��1 */
        unsigned int    bbc_tbbp_245m_srst_en      : 1; /* [11..11] BBPCOMMON_2A TBBP 245MHz�����߼���λԴ���ơ�0 - ��Ӱ��1 - ��bit��1 */
        unsigned int    bbc_twbbp_122m_srst_en     : 1; /* [12..12] BBPCOMMON_2A TWBBP 122MHz�����߼���λԴ���ơ�0 - ��Ӱ��1 - ��bit��1 */
        unsigned int    bbc_g1bbp_104m_srst_en     : 1; /* [13..13] BBPCOMMON_2A G1BBP 104MHz�����߼���λԴ���ơ�0 - ��Ӱ��1 - ��bit��1 */
        unsigned int    bbc_g2bbp_104m_srst_en     : 1; /* [14..14] BBPCOMMON_2A G2BBP 104MHz�����߼���λԴ���ơ�0 - ��Ӱ��1 - ��bit��1 */
        unsigned int    reserved                   : 16; /* [30..15] ������ */
        unsigned int    abb_srst_en                : 1; /* [31..31] ABB��λ����ʹ�ܿ��ơ�0 - ��Ӱ��1 - ��λ��Ч */
    } bits;
    unsigned int    u32;
}HI_CRG_SRSTEN3_T;    /* ��λ����ʹ��3��ABB+BBP�� */

typedef union
{
    struct
    {
        unsigned int    bbp_crg_srst_dis           : 1; /* [0..0] BBPʱ�Ӹ�λģ����λ����رտ��ơ�0 - ��Ӱ��1 - ��λ��Ч�� */
        unsigned int    bbpcommon_2a_pd_srst_dis   : 1; /* [1..1] BBPCOMMON_2A��λ����رտ��ơ�0 - ��Ӱ��1 - ��λ��Ч�� */
        unsigned int    lbbp_pd_srst_dis           : 1; /* [2..2] LTEBBP����������������λ����رտ��ơ�0 - ��Ӱ��1 - ��λ��Ч�� */
        unsigned int    g1bbp_pd_srst_dis          : 1; /* [3..3] G1BBP������������λ����رտ��ơ�0 - ��Ӱ��1 - ��λ��Ч�� */
        unsigned int    g2bbp_pd_srst_dis          : 1; /* [4..4] G2BBP������������λ����رտ��ơ�0 - ��Ӱ��1 - ��λ��Ч�� */
        unsigned int    twbbp_pd_srst_dis          : 1; /* [5..5] TWBBP����������������λ����رտ��ơ�0 - ��Ӱ��1 - ��λ��Ч�� */
        unsigned int    wbbp_pd_srst_dis           : 1; /* [6..6] WBBP����������������λ����رտ��ơ�0 - ��Ӱ��1 - ��λ��Ч�� */
        unsigned int    irm_pd_srst_dis            : 1; /* [7..7] IRM������������λ����رտ��ơ�0 - ��Ӱ��1 - ��λ��Ч�� */
        unsigned int    bbc_lbbp_pdt_srst_dis      : 1; /* [8..8] BBPCOMMON_2A LBBP PDT��λ���ơ�0 - ��Ӱ��1 - ��bit��0 */
        unsigned int    bbc_lbbp_pdf_srst_dis      : 1; /* [9..9] BBPCOMMON_2A LBBP PDF��λ������λ���ơ�0 - ��Ӱ��1 - ��bit��0 */
        unsigned int    bbc_lbbp_tdl_srst_dis      : 1; /* [10..10] BBPCOMMON_2A LBBP TDL��λ������λ���ơ�0 - ��Ӱ��1 - ��bit��0 */
        unsigned int    bbc_tbbp_245m_srst_dis     : 1; /* [11..11] BBPCOMMON_2A TBBP 245MHz�����߼���λ���ơ�0 - ��Ӱ��1 - ��bit��0 */
        unsigned int    bbc_twbbp_122m_srst_dis    : 1; /* [12..12] BBPCOMMON_2A TWBBP 122MHz�����߼���λ���ơ�0 - ��Ӱ��1 - ��bit��0 */
        unsigned int    bbc_g1bbp_104m_srst_dis    : 1; /* [13..13] BBPCOMMON_2A G1BBP 104MHz�����߼���λ���ơ�0 - ��Ӱ��1 - ��bit��0 */
        unsigned int    bbc_g2bbp_104m_srst_dis    : 1; /* [14..14] BBPCOMMON_2A G2BBP 104MHz�����߼���λ���ơ�0 - ��Ӱ��1 - ��bit��0 */
        unsigned int    reserved                   : 16; /* [30..15] ���� */
        unsigned int    abb_srst_dis               : 1; /* [31..31] ABB��λ����رտ��ơ�0 - ��Ӱ��1 - ��λ��Ч */
    } bits;
    unsigned int    u32;
}HI_CRG_SRSTDIS3_T;    /* ��λ����ر�3��ABB+BBP�� */

typedef union
{
    struct
    {
        unsigned int    bbp_crg_srst_stat          : 1; /* [0..0] BBPʱ�Ӹ�λģ����λ����״̬���ơ�0 - ��λ��Ч1 - ��λ��Ч */
        unsigned int    bbpcommon_2a_srst_stat     : 1; /* [1..1] BBPCOMMON_2A��λ����״̬���ơ�0 - ��λ��Ч1 - ��λ��Ч */
        unsigned int    lbbp_pd_srst_stat          : 1; /* [2..2] LTEBBP����������������λ����״̬���ơ�0 - ��λ��Ч1 - ��λ��Ч */
        unsigned int    g1bbp_pd_srst_stat         : 1; /* [3..3] G1BBP������������λ����״̬���ơ�0 - ��λ��Ч1 - ��λ��Ч */
        unsigned int    g2bbp_pd_srst_stat         : 1; /* [4..4] G2BBP������������λ����״̬���ơ�0 - ��λ��Ч1 - ��λ��Ч */
        unsigned int    twbbp_pd_srst_stat         : 1; /* [5..5] TWBBP����������������λ����״̬���ơ�0 - ��λ��Ч1 - ��λ��Ч */
        unsigned int    wbbp_pd_srst_stat          : 1; /* [6..6] WBBP����������������λ����״̬���ơ�0 - ��λ��Ч1 - ��λ��Ч */
        unsigned int    irm_pd_srst_stat           : 1; /* [7..7] IRM������������λ����״̬���ơ�0 - ��λ��Ч1 - ��λ��Ч */
        unsigned int    bbc_lbbp_pdt_srstctrl_stat : 1; /* [8..8] BBPCOMMON_2A LBBP PDT��λ������ơ�0 - ��λ��LTEBBP��ѹ����λ���ƣ�1 - ��λ��BBPCOMMON_2A��λ���ơ� */
        unsigned int    bbc_lbbp_pdf_srstctrl_stat : 1; /* [9..9] BBPCOMMON_2A LBBP PDF��λ������λ���ơ�0 - ��λ��LTEBBP��ѹ����λ���ƣ�1 - ��λ��BBPCOMMON_2A��λ���ơ� */
        unsigned int    bbc_lbbp_tdl_srstctrl_stat : 1; /* [10..10] BBPCOMMON_2A LBBP TDL��λ������λ������ơ�0 - ��λ��LTEBBP��ѹ����λ���ƣ�1 - ��λ��BBPCOMMON_2A��λ���ơ� */
        unsigned int    bbc_tbbp_245m_srstctrl_stat : 1; /* [11..11] BBPCOMMON_2A TBBP 245MHz�����߼���λ������ơ�0 - ��λ��TWEBBP��ѹ����λ���ƣ�1 - ��λ��BBPCOMMON_2A��λ���ơ� */
        unsigned int    bbc_twbbp_122m_srstctrl_stat : 1; /* [12..12] BBPCOMMON_2A TWBBP 122MHz�����߼���λ������ơ�0 - ��λ��TWEBBP��ѹ����λ���ƣ�1 - ��λ��BBPCOMMON_2A��λ���ơ� */
        unsigned int    bbc_g1bbp_104m_srstctrl_stat : 1; /* [13..13] BBPCOMMON_2A G1BBP 104MHz�����߼���λ������ơ�0 - ��λ��G1EBBP��ѹ����λ���ƣ�1 - ��λ��BBPCOMMON_2A��λ���ơ� */
        unsigned int    bbc_g2bbp_104m_srstctrl_stat : 1; /* [14..14] BBPCOMMON_2A G2BBP 104MHz�����߼���λ��λ������ơ�0 - ��λ��G2EBBP��ѹ����λ���ƣ�1 - ��λ��BBPCOMMON_2A��λ���ơ� */
        unsigned int    reserved                   : 16; /* [30..15] ���� */
        unsigned int    abb_srst_status            : 1; /* [31..31] ABB��λ����״̬���ơ�0 - ��λ��Ч1 - ��λ��Ч */
    } bits;
    unsigned int    u32;
}HI_CRG_SRSTSTAT3_T;    /* ��λ����״̬3��ABB+BBP�� */

typedef union
{
    struct
    {
        unsigned int    reserved_4                 : 4; /* [3..0] ���� */
        unsigned int    mdma9_clk_div              : 4; /* [7..4] modem A9��Ƶϵ����ֻҪ�����������߼��������ߵ�Ƶ�ʱȣ�������0-15���κ�ֵ��modem A9���666MHz����A9_PLL postdivʱ�ӱ��˷�Ƶϵ����Ƶ���Ƶ�ʡ�Ĭ��A9PLL POSTDIV���ʱ��Ƶ��Ϊ400MHz */
        unsigned int    a92slow_freqmode           : 3; /* [10..8] modem A9 COREʱ������������Ƶ�ʱ�����(1/N)��ֻҪ�����A9���������ߵ�Ƶ�ʱȣ�������1-8���κ�ֵ�� */
        unsigned int    reserved_3                 : 1; /* [11..11] ���� */
        unsigned int    a92fast_freqmode           : 3; /* [14..12] modem A9 COREʱ����L2C����Ƶ�ʱ�����(1/N)��ֻҪ�����A9���������ߵ�Ƶ�ʱȣ�������1-8���κ�ֵ�� */
        unsigned int    reserved_2                 : 1; /* [15..15] ���� */
        unsigned int    a9_clksw_req               : 3; /* [18..16] A9������ʱ���л�����ֻ����1bitsΪ1.bit[0] - ѡ��A9PLL���ʱ�ӣ�bit[1] - ѡ��DSPPLL���ʱ�ӣ�bit[2] - ѡ��19��2MHzʱ��(default) */
        unsigned int    reserved_1                 : 11; /* [29..19] ���� */
        unsigned int    apb_freqmode               : 1; /* [30..30] APB������SlowBus���߷�Ƶ�����á�0 - 1:11 - 2:1(default) */
        unsigned int    reserved_0                 : 1; /* [31..31] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKDIV2_T;    /* ʱ�ӷ�Ƶ����2(���߷�Ƶ�� */

typedef union
{
    struct
    {
        unsigned int    bbe_clk_div_num            : 2; /* [1..0] BBE16 COREʱ�ӷ�Ƶϵ������߹���Ƶ��Ϊ400MHz��0 - 1��ƵN - N+1��Ƶ */
        unsigned int    reserved_2                 : 2; /* [3..2] ���� */
        unsigned int    bbe_freqmode               : 2; /* [5..4] DSP CORE�����߷�Ƶ�ȡ����ߵ���߹���Ƶ��Ϊ200MHz.0 - 1:11 - 2:1(default)10 -3:111- 4:1 */
        unsigned int    reserved_1                 : 22; /* [27..6] ���� */
        unsigned int    cipher_clk_div             : 3; /* [30..28] cipher����ʱ�ӷ�Ƶϵ������Ƶ��������ο�ʱ��ΪBBPPLL��491.52MHz��N��N��Ƶ(N=3~7)����Ĭ������Ϊ3��Ƶ��cipher_clk=491.53MHz/3=163MHz. */
        unsigned int    reserved_0                 : 1; /* [31..31] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_CLKDIV5_T;    /* ʱ�ӷ�Ƶ����5(BBEDSP/CIPHERʱ�ӷ�Ƶ) */

typedef union
{
    struct
    {
        unsigned int    timer_clk_sel              : 8; /* [7..0] TIMER0-7����ʱ��Դѡ�񣬾�̬�л���1 - ѡ��19.2MHzʱ�ӣ�0 - ѡ��32KHzʱ��Note���л�ʱ��ʱ������ȹرն�Ӧ��ʱ�������Ȼ���л�ʱ�ӣ��л���Ϻ��ʱ�� */
        unsigned int    gps_tcxo_clk_sel           : 1; /* [8..8] GPS TCXOʱ��ѡ�񡣾�̬�л�0 - TCXO0 CLK1 - TCXO1 CLK */
        unsigned int    hpm_refclk_sel             : 1; /* [9..9] HPM����ʱ��ѡ��0 - ѡ��a9_refclk;1 - ѡ��dsp_refclk�� */
        unsigned int    reserved_1                 : 10; /* [19..10] ���� */
        unsigned int    sc_a9wdt_rst_en            : 1; /* [20..20] A9 WDT��λ������Ч��1 - A9��ϵͳ�ڵ�WDT��λ��Чʱ����λA90 - A9��ϵͳ�ڵ�WDT��λ��Чʱ������λA9 */
        unsigned int    reserved_0                 : 11; /* [31..21] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_CLK_SEL2_T;    /* ʱ��Դѡ�����2(TIMER) */

typedef union
{
    struct
    {
        unsigned int    abb_tcxo_clk_sel           : 1; /* [0..0] ABB TCXOʱ��Դѡ��0 - TCXO0ʱ�ӣ�1 - TCXO1ʱ�� */
        unsigned int    bbppll_refclk_sel          : 1; /* [1..1] BBPPLL�ο�ʱ��Դѡ�񡣾�̬�л���0 - ABBͨ��0 TCXOʱ�ӣ�1 - ABBͨ��1 TCXOʱ�� */
        unsigned int    bbp_tcxo_clk_sel           : 1; /* [2..2] ������ */
        unsigned int    ch0_tcxo_clk_sel           : 1; /* [3..3] ͨ��0 19.2MHzʱ��ѡ�񡣾�̬�л���0 - ABBͨ��0 TCXOʱ�ӣ�1 - ABBͨ��1 TCXOʱ�� */
        unsigned int    ch1_tcxo_clk_sel           : 1; /* [4..4] ͨ��1 19.2MHzʱ��ѡ�񡣾�̬�л���0 - ABBͨ��0 TCXOʱ�ӣ�1 - ABBͨ��1 TCXOʱ�� */
        unsigned int    g1bbp_104m_clk_sel         : 1; /* [5..5] G1BBPͨ��ѡ�񡣾�̬�л���0 - ѡ��ͨ��0��1 - ѡ��ͨ��1 */
        unsigned int    g2bbp_104m_clk_sel         : 1; /* [6..6] G2BBPͨ��ѡ�񡣾�̬�л���0 - ѡ��ͨ��0��1 - ѡ��ͨ��1 */
        unsigned int    tdl_clk_sel                : 1; /* [7..7] TDLʱ��Դѡ��0 - 122MHz/245MHz��1 - 245MHz/491MHz */
        unsigned int    lbbp_vdl_clk_sel           : 1; /* [8..8] VDLʱ��Դѡ��0 - 122MHz��1 - 245MHz */
        unsigned int    lbbp_pdf_clk_sel           : 1; /* [9..9] LTEBBP Ƶ��ʱ��Դѡ��0 - 122MHz��1 - 245MHz */
        unsigned int    reserved                   : 22; /* [31..10] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_CLK_SEL3_T;    /* ʱ��Դѡ�����3��BBP�� */

typedef union
{
    struct
    {
        unsigned int    pll_en                     : 1; /* [0..0] DFS����ģʽ��1����������ģʽ��0��Power Downģʽ�� */
        unsigned int    pll_bp                     : 1; /* [1..1] DFS��·���ơ�0����������fOUT=fOUT��1����·fOUT=fREF�� */
        unsigned int    pll_refdiv                 : 6; /* [7..2] Ƶ�ʺϳ�������ο�ʱ�ӷ�Ƶϵ���� */
        unsigned int    pll_intprog                : 12; /* [19..8] Ƶ�ʺϳ�����Ƶ�������֡� */
        unsigned int    pll_postdiv1               : 3; /* [22..20] VCOһ����Ƶϵ���� */
        unsigned int    pll_postdiv2               : 3; /* [25..23] VCO������Ƶϵ���� */
        unsigned int    pll_lock                   : 1; /* [26..26] DFS����ָʾ������Ч��0��ʧ����1�������� */
        unsigned int    reserved                   : 5; /* [31..27] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_A9PLL_CFG0_T;    /* A9PLL���üĴ���0 */

typedef union
{
    struct
    {
        unsigned int    pll_fracdiv                : 24; /* [23..0] Ƶ�ʺϳ�����ƵС�����֡� */
        unsigned int    pll_int_mod                : 1; /* [24..24] ������Ƶ����ģʽ��0��ʹ�ܷ�����Ƶ��1����ֹ������Ƶ�� */
        unsigned int    pll_cfg_vld                : 1; /* [25..25] PLL������Ч��־0 - ������Ч��1 - ������Ч */
        unsigned int    pll_clk_gt                 : 1; /* [26..26] PLL���ʱ���ſ��źš�0 - PLL���ʱ���ſء�1 - PLL���ʱ�Ӳ��ſء���bit��ҪPLL���������á� */
        unsigned int    reserved                   : 5; /* [31..27] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_A9PLL_CFG1_T;    /* A9PLL���üĴ���1 */

typedef union
{
    struct
    {
        unsigned int    pll_ssc_reset              : 1; /* [0..0] ��λ��Ч���ߵ�ƽ��Ч��0 - ��λ���룻1 - ��λ��Ч����bit������PLL��Чʱ���ã�����ᵼ��PLLʧ���� */
        unsigned int    pll_ssc_disable            : 1; /* [1..1] Bypass the modulator,�ߵ�ƽ��Ч��0 - ����������1 - bypass */
        unsigned int    pll_ssc_downspread         : 1; /* [2..2] 0 - center spread1 - down spread */
        unsigned int    pll_ssc_spread             : 3; /* [5..3] ���õ������(spread%).000 - 0001 - 0.049%^010 - 0.098%011 - 0.195%100 - 0.391%101 - 0.781%110 - 1.563%111 - 3.125% */
        unsigned int    pll_ssc_divval             : 4; /* [9..6] ����Ƶ�׵��Ƶ�Ƶ�ʣ�һ��ԼΪ32KHzfreq=(CLKSSCG/(DIVCAL* #points),����points=128;CLKSSCG=FREF(REFDIV=1ʱ�� */
        unsigned int    reserved                   : 22; /* [31..10] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_A9PLL_CFG2_T;    /* A9PLL���üĴ���2 */

typedef union
{
    struct
    {
        unsigned int    pll_en                     : 1; /* [0..0] DFS����ģʽ��1����������ģʽ��0��Power Downģʽ�� */
        unsigned int    pll_bp                     : 1; /* [1..1] DFS��·���ơ�0����������fOUT=fOUT��1����·fOUT=fREF�� */
        unsigned int    pll_refdiv                 : 6; /* [7..2] Ƶ�ʺϳ�������ο�ʱ�ӷ�Ƶϵ���� */
        unsigned int    pll_intprog                : 12; /* [19..8] Ƶ�ʺϳ�����Ƶ�������֡� */
        unsigned int    pll_postdiv1               : 3; /* [22..20] VCO������Ƶϵ���� */
        unsigned int    pll_postdiv2               : 3; /* [25..23] VCO������Ƶϵ���� */
        unsigned int    pll_lock                   : 1; /* [26..26] DFS����ָʾ������Ч��0��ʧ����1�������� */
        unsigned int    reserved                   : 5; /* [31..27] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_DSPPLL_CFG0_T;    /* BBEPLL���üĴ���0 */

typedef union
{
    struct
    {
        unsigned int    pll_fracdiv                : 24; /* [23..0] Ƶ�ʺϳ�����ƵС�����֡� */
        unsigned int    pll_int_mod                : 1; /* [24..24] ������Ƶ����ģʽ��0��ʹ�ܷ�����Ƶ��1����ֹ������Ƶ�� */
        unsigned int    pll_cfg_vld                : 1; /* [25..25] PLL������Ч��־0 - ������Ч��1 - ������Ч */
        unsigned int    pll_clk_gt                 : 1; /* [26..26] PLL���ʱ���ſ��źš�0 - PLL���ʱ���ſء�1 - PLL���ʱ�Ӳ��ſء���bit��ҪPLL���������á� */
        unsigned int    reserved                   : 5; /* [31..27] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_DSPPLL_CFG1_T;    /* BBEPLL���üĴ���1 */

typedef union
{
    struct
    {
        unsigned int    pll_ssc_reset              : 1; /* [0..0] ��λ��Ч���ߵ�ƽ��Ч��0 - ��λ���룻1 - ��λ��Ч����bit������PLL��Чʱ���ã�����ᵼ��PLLʧ���� */
        unsigned int    pll_ssc_disable            : 1; /* [1..1] Bypass the modulator,�ߵ�ƽ��Ч��0 - ����������1 - bypass */
        unsigned int    pll_ssc_downspread         : 1; /* [2..2] 0 - center spread1 - down spread */
        unsigned int    pll_ssc_spread             : 3; /* [5..3] ���õ������(spread%).000 - 0001 - 0.049%^010 - 0.098%011 - 0.195%100 - 0.391%101 - 0.781%110 - 1.563%111 - 3.125% */
        unsigned int    pll_ssc_divval             : 4; /* [9..6] ����Ƶ�׵��Ƶ�Ƶ�ʣ�һ��ԼΪ32KHzfreq=(CLKSSCG/(DIVCAL* #points),����points=128;CLKSSCG=FREF(REFDIV=1ʱ�� */
        unsigned int    reserved                   : 22; /* [31..10] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_DSPPLL_CFG2_T;    /* BBEPLL���üĴ���2 */

typedef union
{
    struct
    {
        unsigned int    pll_en                     : 1; /* [0..0] DFS����ģʽ��1����������ģʽ��0��Power Downģʽ�� */
        unsigned int    pll_bp                     : 1; /* [1..1] DFS��·���ơ�0����������fOUT=fOUT��1����·fOUT=fREF�� */
        unsigned int    pll_refdiv                 : 6; /* [7..2] Ƶ�ʺϳ�������ο�ʱ�ӷ�Ƶϵ���� */
        unsigned int    pll_intprog                : 12; /* [19..8] Ƶ�ʺϳ�����Ƶ�������֡� */
        unsigned int    pll_postdiv1               : 3; /* [22..20] VCO������Ƶϵ���� */
        unsigned int    pll_postdiv2               : 3; /* [25..23] VCO������Ƶϵ���� */
        unsigned int    pll_lock                   : 1; /* [26..26] DFS����ָʾ������Ч��0��ʧ����1�������� */
        unsigned int    reserved                   : 5; /* [31..27] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_BBPPLL_CFG0_T;    /* BBPPLL���üĴ���0 */

typedef union
{
    struct
    {
        unsigned int    pll_fracdiv                : 24; /* [23..0] Ƶ�ʺϳ�����ƵС�����֡� */
        unsigned int    pll_int_mod                : 1; /* [24..24] ������Ƶ����ģʽ��0��ʹ�ܷ�����Ƶ��1����ֹ������Ƶ�� */
        unsigned int    pll_cfg_vld                : 1; /* [25..25] PLL������Ч��־0 - ������Ч��1 - ������Ч */
        unsigned int    pll_clk_gt                 : 1; /* [26..26] PLL���ʱ���ſ��źš�0 - PLL���ʱ���ſء�1 - PLL���ʱ�Ӳ��ſء���bit��ҪPLL���������á� */
        unsigned int    reserved                   : 5; /* [31..27] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_BBPPLL_CFG1_T;    /* BBPPLL���üĴ���1 */

typedef union
{
    struct
    {
        unsigned int    pll_ssc_reset              : 1; /* [0..0] ��λ��Ч���ߵ�ƽ��Ч��0 - ��λ���룻1 - ��λ��Ч����bit������PLL��Чʱ���ã�����ᵼ��PLLʧ���� */
        unsigned int    pll_ssc_disable            : 1; /* [1..1] Bypass the modulator,�ߵ�ƽ��Ч��0 - ����������1 - bypass */
        unsigned int    pll_ssc_downspread         : 1; /* [2..2] 0 - center spread1 - down spread */
        unsigned int    pll_ssc_spread             : 3; /* [5..3] ���õ������(spread%).000 - 0001 - 0.049%^010 - 0.098%011 - 0.195%100 - 0.391%101 - 0.781%110 - 1.563%111 - 3.125% */
        unsigned int    pll_ssc_divval             : 4; /* [9..6] ����Ƶ�׵��Ƶ�Ƶ�ʣ�һ��ԼΪ32KHzfreq=(CLKSSCG/(DIVCAL* #points),����points=128;CLKSSCG=FREF(REFDIV=1ʱ�� */
        unsigned int    reserved                   : 22; /* [31..10] ���� */
    } bits;
    unsigned int    u32;
}HI_CRG_BBPPLL_CFG2_T;    /* BBPPLL���üĴ���2 */

typedef union
{
    struct
    {
        unsigned int    mcpu_boot_remap_clear      : 1; /* [0..0] ModemA9������ַ��ӳ����ơ�0 - remap��A9��0��ַ��ӳ�䵽���õ�������ַ1 - 0��ַ����ӳ���bit����һֱ����Ϊ0����ʱModemA9���ʲ�����64KB��DDR�ռ䡣 */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL0_T;    /* ModemA9����Remap���ƼĴ��� */

typedef union
{
    struct
    {
        unsigned int    wdt_clk_en                 : 1; /* [0..0] ���Ź�ʱ��ʹ�ܿ��ơ�0����ʹ�ܣ�1��ʹ�ܣ�Ĭ��ֵ����ע�⣺�Ը�λ����д0����ʱ����Ҫ�Ƚ��üĴ�����16λֵдΪ0x6920������wdt_en_ctrlλдΪ0x6920���򿪱�����Ȼ���ٶԴ˱���д��ֵ�ſ�����Ч������������� */
        unsigned int    reserved_1                 : 2; /* [2..1] ���������ݿ�д�룩�� */
        unsigned int    wdt_en_ov                  : 1; /* [3..3] WDTʱ��ǿ��ʹ�ܿ���λ��0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ���wdt_en_ovʹ��ʱ�����Ź�������ʱ�ӣ�pclk��������������32Kʱ�Ӽ�������λһ������£�����Ҫʹ�ã�һֱ����Ϊ0�� */
        unsigned int    reserved_0                 : 12; /* [15..4] ���������ݿ�д�룩�� */
        unsigned int    wdt_en_ctrl                : 16; /* [31..16] wdt_enдʹ�ܿ��ơ��ÿ���λ��Ϊ�˷�ֹ�����������Ź��������ı����߼������Ź�ʱ��ʹ�ܿ���ֻ���ڸ�λ����Ϊ6920�󣬲ſɸ��ġ� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL2_T;    /* ���Ź����üĴ����� */

typedef union
{
    struct
    {
        unsigned int    reserved_3                 : 10; /* [9..0] ���� */
        unsigned int    amon_monitor_start         : 1; /* [10..10] ���Global AXI����axi monitor����ֹͣ�źš�0��ֹͣ��1�������� */
        unsigned int    reserved_2                 : 4; /* [14..11] ������ */
        unsigned int    dsp0_uart_en               : 2; /* [16..15] LTEDSP UARTѡ��bit16��ѡ��UART0���ơ�bit15��ѡ��hs uart���ơ�ÿ����ֵΪ1��ʾʹ�ö�Ӧuart��0��ʾ��ʹ�� */
        unsigned int    reserved_1                 : 14; /* [30..17] ������ */
        unsigned int    reserved_0                 : 1; /* [31..31] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL3_T;    /* �������üĴ����� */

typedef union
{
    struct
    {
        unsigned int    cicom0_sel_mod             : 1; /* [0..0] ģʽѡ��λ��0��WCDMA�ӽ���ģʽ��Ĭ��ֵ����1��GSM�ӽ���ģʽ�� */
        unsigned int    cicom1_sel_mod             : 1; /* [1..1] ģʽѡ��λ��0��WCDMA�ӽ���ģʽ��Ĭ��ֵ����1��GSM�ӽ���ģʽ�� */
        unsigned int    reserved                   : 30; /* [31..2] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL4_T;    /* CICOM�ӽ���ģʽ���üĴ����� */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL5_T;    /* �����Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL6_T;    /* �����Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 16; /* [15..0] ������ */
        unsigned int    sw_dsp0_boot_addr          : 16; /* [31..16] BBE16�ⲿ������ַ���á���ַ�ռ��С64KB���õ�ַ��Ҫ64KB���롣��������ⲿ������ʽ����Ҫ����ʵ��������ַ���øüĴ����� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL9_T;    /* BBE16������ַ���üĴ��� */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] ���� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL10_T;    /* �����Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_2                 : 1; /* [0..0] ������ */
        unsigned int    moda9_cfgnmfi              : 1; /* [1..1] �����жϲ������ο���λ��0�������1�����ã�Ĭ��ֵ����ֻ��CPU resetʱ�������� */
        unsigned int    moda9_cfgsdisable          : 1; /* [2..2] ������������Ҫ���üĴ���д������0����ʹ�ܣ�1��ʹ�ܡ��ñ���ʹ�ܺ���Է�ֹdistributor��A9�������ӿڡ�LSPIs�Ĳ��ּĴ������޸ġ������Ҫʹ�ô˹��ܣ�ARM�����û���boot�׶�������������ؼĴ�����ʹ�ܸù��ܡ������ϣ�ϵͳֻ����Ӳ��λʱ��Ҫ�رոñ������ܡ� */
        unsigned int    moda9_cp15sdisable         : 1; /* [3..3] secure registerд������0����ʹ�ܣ�1��ʹ�ܡ�reset 0�� then 1 by software������CPU�ڲ��Ĵ������ã���ϵͳ�Ĵ����߼�������ֵֻ�ɸı�һ�Ρ� */
        unsigned int    moda9_pwrctli0             : 2; /* [5..4] CPU0״̬�Ĵ���[1:0]��λֵ�����޵�Դ���������0�� */
        unsigned int    reserved_1                 : 10; /* [15..6] ���� */
        unsigned int    moda9_l2_waysize           : 3; /* [18..16] moda9 L2 cache way size��000��reserved001��16KB010��32KB(default)others��reserved */
        unsigned int    reserved_0                 : 1; /* [19..19] ������ */
        unsigned int    moda9_l2_regfilebase       : 12; /* [31..20] moda9 L2 cache �Ĵ�������ַ�� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL11_T;    /* Modem A9���üĴ����� */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] ���� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL12_T;    /* �����Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 16; /* [15..0] ������ */
        unsigned int    ocdhaltonreset             : 1; /* [16..16] BBE16�ڽ⸴λʱǿ�н���OCDHaltģʽ��0������ģʽ��Ĭ��ֵ����1��OCDHaltģʽ����bit�ڽ⸴λǰ6�����ڼ���10�����ڲ����޸ġ��ڽ⸴λ��ɺ󣬸ı��bitû��Ӱ�졣 */
        unsigned int    runstall                   : 1; /* [17..17] BBE16����/ֹͣ��0�����У�1��ֹͣ����Ĭ��ֵ�� */
        unsigned int    statvectorsel              : 1; /* [18..18] BBE16������ַѡ��0���ڲ�TCM������Ĭ��ֵ��1���ⲿ������������ַ���ⲿ������ַ�Ĵ������� */
        unsigned int    breaksync_en               : 3; /* [21..19]  */
        unsigned int    crosstrig_en               : 3; /* [24..22]  */
        unsigned int    reserved_0                 : 7; /* [31..25] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL13_T;    /* BBE16���üĴ��� */

typedef union
{
    struct
    {
        unsigned int    hpm_clk_div                : 6; /* [5..0] HPM clock dividor */
        unsigned int    reserved_1                 : 1; /* [6..6] ���� */
        unsigned int    hpm_en                     : 1; /* [7..7] HPMʹ�ܿ��ơ�1 - ʹ�ܡ� */
        unsigned int    hpmx_en                    : 1; /* [8..8] HPMXʹ�ܿ��ơ�1 - ʹ�ܡ� */
        unsigned int    reserved_0                 : 23; /* [31..9] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL17_T;    /* HPM���ƼĴ��� */

typedef union
{
    struct
    {
        unsigned int    axi_mem_gatedclock_en      : 1; /* [0..0] axi_mem�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ� */
        unsigned int    cicom0_auto_clk_gate_en    : 1; /* [1..1] cicom0�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ� */
        unsigned int    cicom0_soft_gate_clk_en    : 1; /* [2..2] cicom0ʱ���ſؿ���(���Զ��ſ�ʹ��Ϊ0ʱ��Ч)��0�����ضϣ�Ĭ��ֵ����1���ض�ʱ�ӡ� */
        unsigned int    cicom1_auto_clk_gate_en    : 1; /* [3..3] cicom1�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ� */
        unsigned int    cicom1_soft_gate_clk_en    : 1; /* [4..4] cicom1ʱ���ſؿ���(���Զ��ſ�ʹ��Ϊ0ʱ��Ч)��0�����ضϣ�Ĭ��ֵ����1���ض�ʱ�ӡ� */
        unsigned int    hs_uart_gatedclock_en      : 1; /* [5..5] hs_uart�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ� */
        unsigned int    uart_gatedclock_en         : 1; /* [6..6] �ǵ�����uart0, ������uart1/2/3�Զ��ſ�ʹ��1��ʹ��0����ʹ�ܣ�Ĭ��ֵ�� */
        unsigned int    reserved_2                 : 12; /* [18..7] ������ */
        unsigned int    uicc_gatedclock_en         : 1; /* [19..19] UICCʱ���ſؿ���(���Զ��ſ�ʹ��Ϊ0ʱ��Ч)��0�����ضϣ�Ĭ��ֵ����1���ض�ʱ�ӡ� */
        unsigned int    uicc_ss_scaledown_mode     : 2; /* [21..20] uicc������ٿ��ơ�00�����ٹرգ�Ĭ��ֵ����01��ʹ�ܳ�speed enumeration,HNP/SRP,Host mode suspend nad resume֮���timingvalue��10����ʹ��device mode suspend and resume timing value��11��ʹ�ܱ���0��1 scale-down timing values���ۺ�ʱ����Ϊ00�� */
        unsigned int    upacc_auto_clk_gate_en     : 1; /* [22..22] upacc�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ� */
        unsigned int    upacc_soft_gate_clk_en     : 1; /* [23..23] upaccʱ���ſؿ���(���Զ��ſ�ʹ��Ϊ0ʱ��Ч)��0�����ضϣ�Ĭ��ֵ����1���ض�ʱ�ӡ� */
        unsigned int    bbe16_cg_en                : 1; /* [24..24] dsp0 ram�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ� */
        unsigned int    reserved_1                 : 3; /* [27..25] ������ */
        unsigned int    ipcm_auto_clk_gate_en      : 1; /* [28..28] ipcm�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ� */
        unsigned int    ipcm_soft_gate_clk_en      : 1; /* [29..29] ipcmʱ���ſؿ���(���Զ��ſ�ʹ��Ϊ0ʱ��Ч)��0�����ضϣ�Ĭ��ֵ����1���ض�ʱ�ӡ� */
        unsigned int    reserved_0                 : 1; /* [30..30] ������ */
        unsigned int    sysreg_auto_gate_en        : 1; /* [31..31] ϵͳ���������ýӿ�ʱ���Զ��ſ�ʹ�ܿ��ơ�1 - �Զ��ſ�ʹ�ܡ�0 -��ʹ�ܡ� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL20_T;    /* �Զ��ſ�ʱ�ӿ��ƼĴ���0 */

typedef union
{
    struct
    {
        unsigned int    reserved_6                 : 4; /* [3..0] ������ */
        unsigned int    timer_gatedclock_en        : 1; /* [4..4] timer0-7�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ� */
        unsigned int    reserved_5                 : 5; /* [9..5] ������ */
        unsigned int    dw_axi_glb_cg_en           : 1; /* [10..10] global axi�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ� */
        unsigned int    dw_axi_mst_cg_en           : 1; /* [11..11] master axi�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ� */
        unsigned int    reserved_4                 : 1; /* [12..12] ������ */
        unsigned int    dw_axi_bbphy_cg_en         : 1; /* [13..13] bbphy axi�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ� */
        unsigned int    reserved_3                 : 1; /* [14..14] ���� */
        unsigned int    dw_x2x_async_cg_en         : 1; /* [15..15] async x2x�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ�async x2x�ֲ��ڣ�DSP0������l2c�ӿ� */
        unsigned int    reserved_2                 : 1; /* [16..16] ���� */
        unsigned int    dw_x2h_qsync_cg_en         : 1; /* [17..17] rs axi�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ� */
        unsigned int    dw_hmx_cg_en               : 1; /* [18..18] hmx�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ�λ��ahb2axi_mst���һ�� */
        unsigned int    dw_x2p_cg_en               : 1; /* [19..19] x2p�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ� */
        unsigned int    dw_gs_cg_en                : 1; /* [20..20] gs�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ�gsλ��axi mem�� */
        unsigned int    ashb_gatedclock_en         : 1; /* [21..21] h2x�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ�λ��m3��ahb2axi_mst���һ�� */
        unsigned int    dw_ahb_mst_gatedclock_en   : 1; /* [22..22] mst ahb�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ� */
        unsigned int    reserved_1                 : 1; /* [23..23] ������ */
        unsigned int    edmac_autogated_clk_en     : 1; /* [24..24] edmac�Զ��ſ�ʹ�ܡ�0����ʹ�ܣ�Ĭ��ֵ����1��ʹ�ܡ� */
        unsigned int    reserved_0                 : 7; /* [31..25] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL21_T;    /* �Զ��ſ�ʱ�ӿ��ƼĴ���1 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 16; /* [15..0] ������ */
        unsigned int    bbe16_ema                  : 3; /* [18..16] extra margin adjustment ,[0] = LSB */
        unsigned int    bbe16_emaw                 : 2; /* [20..19] extra margin adjustment write,[0] = LSB */
        unsigned int    bbe16_emas                 : 1; /* [21..21] extra margin adjustment sense amplifier pulse */
        unsigned int    reserved_0                 : 10; /* [31..22] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL22_T;    /* BBE16 memory ʱ�����üĴ��� */

typedef union
{
    struct
    {
        unsigned int    axi_mem_ema                : 3; /* [2..0] extra margin adjustment ,[0] = LSB */
        unsigned int    axi_mem_emaw               : 2; /* [4..3] extra margin adjustment write,[0] = LSB */
        unsigned int    axi_mem_emas               : 1; /* [5..5] extra margin adjustment sense amplifier pulse */
        unsigned int    reserved_1                 : 2; /* [7..6] ������ */
        unsigned int    soc_mem_ema                : 3; /* [10..8] extra margin adjustment ,[0] = LSB������soc spram ��dpram���Լ�tpram��A�ڡ���soc_mem��ע�⣺soc spram������hs_uart��ipf,cipher,uiccģ���ڵ�spram��soc tpram������edmac��amon��ipf��cipherģ���ڵ�tpram�� */
        unsigned int    soc_mem_spram_emaw         : 2; /* [12..11] extra margin adjustment write,[0] = LSB */
        unsigned int    soc_mem_spram_emas         : 1; /* [13..13] extra margin adjustment sense amplifier pulse */
        unsigned int    soc_mem_tpram_emab         : 3; /* [16..14] extra margin adjustment ,[0] = LSB������soc tpram��B�ڡ� */
        unsigned int    soc_mem_tpram_emasa        : 1; /* [17..17] extra margin adjustment sense amplifier pulse */
        unsigned int    soc_mem_dpram_emaw         : 2; /* [19..18] extra margin adjustment write,[0] = LSB */
        unsigned int    soc_mem_dpram_emas         : 1; /* [20..20] extra margin adjustment sense amplifier pulse */
        unsigned int    reserved_0                 : 11; /* [31..21] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL23_T;    /* SOC memory ʱ�����üĴ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 10; /* [9..0] ������ */
        unsigned int    moda9_hs_mem_adjust        : 10; /* [19..10] ModA9 memory ʱ��/�͹��Ŀ��ơ�[9:7]������[6]��ret1n, default=1'b0[5]��EMAS, default-1'b1[4:3]��EMAW, default=2'b01[2:0]��EMA, default=3'b011 */
        unsigned int    reserved_0                 : 6; /* [25..20] ������ */
        unsigned int    moda9_l2_ema               : 3; /* [28..26] extra margin adjustment ,[0] = LSB */
        unsigned int    moda9_l2_emaw              : 2; /* [30..29] extra margin adjustment write,[0] = LSB */
        unsigned int    moda9_l2_emas              : 1; /* [31..31] extra margin adjustment sense amplifier pulse */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL24_T;    /* memory ʱ�����üĴ���2��CCPU+L2)�� */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL25_T;    /* SOC �����ź�ѡ��Ĵ���(������ */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] ���� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL26_T;    /* SOC �����ź�bypass�Ĵ���(������ */

typedef union
{
    struct
    {
        unsigned int    acp_filter_start           : 32; /* [31..0] ��ַ�����׵�ַ����C����ʼ��ַΪ0x5000_0000�� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL28_T;    /* ��ַ�����׵�ַ(ACP�ռ���ʼ��ַ) */

typedef union
{
    struct
    {
        unsigned int    acp_filter_end             : 32; /* [31..0] ��ַ����β��ַ����C�ν�����ַΪ0x7FEF_FFFF�� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL29_T;    /* ��ַ����β��ַ(ACP�ռ������ַ) */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] ���� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL30_T;    /* �����Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    uicc_ic_usb_vbusvalid      : 1; /* [0..0] VBUS��Ч���ơ�0����Ч��1����Ч�� */
        unsigned int    reserved                   : 31; /* [31..1] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL45_T;    /* uicc���ƼĴ��� */

typedef union
{
    struct
    {
        unsigned int    dsp0_nmi                   : 16; /* [15..0] dsp0���������жϣ���һbitΪ�߼���Ч�� */
        unsigned int    reserved                   : 16; /* [31..16] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL52_T;    /* BBE16���������жϼĴ����� */

typedef union
{
    struct
    {
        unsigned int    reserved_7                 : 5; /* [4..0] ���� */
        unsigned int    uicc_ret1n                 : 1; /* [5..5] memory retention mode1���ơ�0������retention��1��������retention,Normal����ģʽ�� */
        unsigned int    reserved_6                 : 3; /* [8..6] ���� */
        unsigned int    amon_soc_ret1n             : 1; /* [9..9] memory retention mode1���ơ�0������retention��1��������retention,Normal����ģʽ�� */
        unsigned int    reserved_5                 : 3; /* [12..10] ���� */
        unsigned int    edmac_colldisn             : 1; /* [13..13] memory��д��ͻ��·disable���ơ�0����д��ͻ��·��ʹ�ܣ�1����д��ͻ��·ʹ��(default)�� */
        unsigned int    reserved_4                 : 3; /* [16..14] ���� */
        unsigned int    amon_soc_colldisn          : 1; /* [17..17] memory��д��ͻ��·disable���ơ�0��disable ��д��ͻ��·��1��enable ��д��ͻ��·�� */
        unsigned int    reserved_3                 : 1; /* [18..18] ���� */
        unsigned int    ipf_ret1n                  : 1; /* [19..19] memory retention mode1���ơ�0������retention��1��������retention�� */
        unsigned int    ipf_colldisn               : 1; /* [20..20] memory��д��ͻ��·disable���ơ�0����д��ͻ��·��ʹ�ܣ�1����д��ͻ��·ʹ��(default)�� */
        unsigned int    cipher_ret1n               : 1; /* [21..21] memory retention mode1���ơ�0������retention��1��������retention�� */
        unsigned int    cipher_colldisn            : 1; /* [22..22] memory��д��ͻ��·disable���ơ�0����д��ͻ��·��ʹ�ܣ�1����д��ͻ��·ʹ��(default)�� */
        unsigned int    reserved_2                 : 2; /* [24..23] ���� */
        unsigned int    hs_uart_ret1n              : 1; /* [25..25] memory retention mode1���ơ�0������retention��1��������retention�� */
        unsigned int    reserved_1                 : 3; /* [28..26] ���� */
        unsigned int    edmac_ret1n                : 1; /* [29..29] memory retention mode1���ơ�0������retention��1��������retention�� */
        unsigned int    reserved_0                 : 2; /* [31..30] ���� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL55_T;    /* memory�͹������üĴ���0 */

typedef union
{
    struct
    {
        unsigned int    prior_level                : 2; /* [1..0] �������ȼ����á�00�����ȼ�dma > axi;01�����ȼ�axi > dma. */
        unsigned int    gatedclock_en              : 1; /* [2..2] lbus�����Զ�ʱ���ſ�ʹ�ܡ�1��ʹ�ܣ�0����ֹ�� */
        unsigned int    dw_axi_rs_gatedclock_en    : 1; /* [3..3] axi rs�Զ��ſ�ʹ�ܡ�1��ʹ�ܣ�0����ֹ�� */
        unsigned int    dw_axi_gs_gatedclock_en    : 1; /* [4..4] axi gs�Զ��ſ�ʹ�ܡ�1��ʹ�ܣ�0����ֹ�� */
        unsigned int    overf_prot                 : 2; /* [6..5] ���߷���������ѡ�00���رշ�����������01��ʹ�ܷ�������������ʱʱ�䣺512 cycles��10��ʹ�ܷ�������������ʱʱ�䣺1024 cycles��11��ʹ�ܷ�������������ʱʱ�䣺2048 cycles */
        unsigned int    reserved                   : 9; /* [15..7] ������ */
        unsigned int    atpram_ctrl                : 16; /* [31..16] memory�͹��Ŀ��ơ� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL56_T;    /* BBP LBUS���ƼĴ����� */

typedef union
{
    struct
    {
        unsigned int    soc2lte_tbd                : 16; /* [15..0] BBP�����Ĵ��� */
        unsigned int    reserved                   : 16; /* [31..16] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL57_T;    /* BBP�������ƼĴ����� */

typedef union
{
    struct
    {
        unsigned int    moda9_l2_ret1n             : 1; /* [0..0] memory retention mode1���ơ�0������retention��1��������retention�� */
        unsigned int    moda9_l2_ret2n             : 1; /* [1..1] memory retention mode2���ơ�0������retention��1��������retention����bitδʹ�á� */
        unsigned int    moda9_l2_pgen              : 1; /* [2..2] memory power down���ơ�0��������power down��1������power down����bitδʹ�á� */
        unsigned int    reserved_2                 : 5; /* [7..3] ������ */
        unsigned int    dsp0_ret1n                 : 1; /* [8..8] memory retention mode1���ơ�0������retention��1��������retention�� */
        unsigned int    dsp0_ret2n                 : 1; /* [9..9] memory retention mode2���ơ�V7R2��memory��֧�ִ�bit����0������retention��1��������retention����bitδʹ�á� */
        unsigned int    dsp0_pgen                  : 1; /* [10..10] memory power down���ơ�V7R2��memory��֧�ִ�bit����0��������power down��1������power down����bitδʹ�á� */
        unsigned int    reserved_1                 : 5; /* [15..11] ������ */
        unsigned int    pd_axi_mem_ret1n           : 1; /* [16..16] memory retention mode1���ơ�0������retention��1��������retention�� */
        unsigned int    pd_axi_mem_ret2n           : 1; /* [17..17] memory retention mode2���ơ�0������retention��1��������retention����bitδʹ�á� */
        unsigned int    pd_axi_mem_pgen            : 1; /* [18..18] memory power down���ơ�0��������power down��1������power down����bitδʹ�á� */
        unsigned int    reserved_0                 : 13; /* [31..19] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL68_T;    /* memory�͹������üĴ���2 */

typedef union
{
    struct
    {
        unsigned int    axi_bbphy_xdcdr_sel        : 1; /* [0..0] AXI_BBPHY�������߼����ơ�0�����ʹ��ӡ���λ������ģ��������ı������߼���������DECERR��1�����ʹ��ӡ���λ������ģ�鱨�������뵽default slave���������߼�ʹ�ܣ�SC_CTRL103��Ӧbit����Ϊ0)������DECERR�� */
        unsigned int    reserved_2                 : 2; /* [2..1] ���� */
        unsigned int    axi_glb_xdcdr_sel          : 1; /* [3..3] AXI_GLB�������߼����ơ�0�����ʹ��ӡ���λ������ģ��������ı������߼���������DECERR��1�����ʹ��ӡ���λ������ģ�鱨�������뵽default slave���������߼�ʹ�ܣ�SC_CTRL103��Ӧbit����Ϊ0)������DECERR�� */
        unsigned int    reserved_1                 : 1; /* [4..4] ���� */
        unsigned int    socapb_pslverr_sel         : 1; /* [5..5] APB���������߼����ơ�0�����ʹ��ӡ���λ������ģ��������ı������߼���������DECERR��1�����ʹ��ӡ���λ������ģ�鱨�������뵽default slave���������߼�ʹ�ܣ�SC_CTRL69��Ӧbit����Ϊ0)������DECERR�� */
        unsigned int    x2h_hslverr_sel            : 1; /* [6..6] x2h�������߼���0�����ʹ��ӡ���λ������ģ��������ı������߼���ϵͳ���ܹ�����1�����ʹ��ӡ���λ������ģ�鱨�������뵽default slave���������߼�ʹ�ܣ�SC_CTRL105��Ӧbit����Ϊ0)��ϵͳ��������default slave����ok�� */
        unsigned int    mst_err_srst_req           : 1; /* [7..7] ���д�����Ϣ��λ����0������λ��1����λ����mst_err�жϺ�״̬�� */
        unsigned int    reserved_0                 : 24; /* [31..8] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL69_T;    /* ϵͳ�����ַ���ʹ��ܼĴ��� */

typedef union
{
    struct
    {
        unsigned int    axi_bbphy_priority_m1      : 2; /* [1..0] dw_axi_bbphy m1�ڣ�dw_axi_glb�ţ����ȼ����á�ֵԽ�����ȼ�Խ�ߡ� */
        unsigned int    axi_bbphy_priority_m2      : 2; /* [3..2] dw_axi_bbphy m2�ڣ�dsp0�����ȼ����á�ֵԽ�����ȼ�Խ�ߡ� */
        unsigned int    axi_bbphy_priority_m3      : 2; /* [5..4] dw_axi_bbphy m3�ڣ�LTEBBP/TDSBBP�����ȼ����á�ֵԽ�����ȼ�Խ�ߡ� */
        unsigned int    axi_bbphy_priority_m4      : 2; /* [7..6] dw_axi_bbphy m4�ڣ�WBBP�����ȼ����á� */
        unsigned int    reserved_1                 : 8; /* [15..8] dw_axi_guacc m4�ڣ�upacc�����ȼ����á�ֵԽ�����ȼ�Խ�ߡ���ֵͬ�Ķ˿ں�С�����ȼ��ߡ� */
        unsigned int    axi_mst_priority_m9        : 4; /* [19..16] dw_axi_mxt m9�ڣ����������ȼ����á�ֵԽ�����ȼ�Խ�ߡ� */
        unsigned int    axi_mst_priority_m10       : 4; /* [23..20] dw_axi_mxt m10�ڣ����������ȼ����á�ֵԽ�����ȼ�Խ�ߡ� */
        unsigned int    reserved_0                 : 8; /* [31..24] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL70_T;    /* �������ȼ����üĴ���0 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 4; /* [3..0] ���� */
        unsigned int    axi_glb_priority_m1        : 3; /* [6..4] dw_axi_glb m1�ڣ�dw_axi_bbphy�����ȼ����á�ֵԽ�����ȼ�Խ�ߡ� */
        unsigned int    axi_glb_priority_m2        : 3; /* [9..7] dw_axi_glb m2�ڣ�dw_axi_mst�����ȼ����á�ֵԽ�����ȼ�Խ�ߡ� */
        unsigned int    axi_glb_priority_m3        : 3; /* [12..10] dw_axi_glb m3�ڣ�AP�ࣩ���ȼ����á�ֵԽ�����ȼ�Խ�ߡ� */
        unsigned int    axi_glb_priority_m4        : 3; /* [15..13] dw_axi_glb m4�ڣ�mdma9�����ȼ����á�ֵԽ�����ȼ�Խ�ߡ� */
        unsigned int    axi_glb_priority_m5        : 3; /* [18..16] dw_axi_glb m5�ڣ�WBBP�����ȼ����á�ֵԽ�����ȼ�Խ�ߡ� */
        unsigned int    axi_glb_priority_m6        : 3; /* [21..19] dw_axi_glb m6�ڣ����������ȼ����á�ֵԽ�����ȼ�Խ�ߡ� */
        unsigned int    axi_mst_cache_cfg_en       : 1; /* [22..22] ModemCPU cache����ʹ�ܡ�0��ʹ������master cache��Ĭ�ϣ���1��ʹ��axi_mst_cache����ֵ����ַ��DDR�ռ䣩 */
        unsigned int    axi_mst_sideband           : 5; /* [27..23] ��GLB AXI�Ŵ��뵽ACP AW/ARUSERS�˵Ŀ����źš�[27:24]�ڲ�����0000��strongly-ordered��0001��device��0011��normal memory non-cacheable��0110��write-through��0111��write-back no write allocate��1111��write-back write allocate��[23]����AWIDMx[2]=0ʱ0��non-coherent request��1��coherent request�� */
        unsigned int    axi_mst_cache              : 4; /* [31..28] MOD CPU cacheֵ��0000��noncacheable��nonbufferable0001��bufferable only0010��cacheable��but do not allocate0011��cacheable��bufferable��but do not allocate0100��reserved0101��reserved0110��cachable write-through, allocate on reads only0111��cachable write-back, allocate on reads only1000��reserved1001��reserved1010��cachable write-through, allocate on writes only1011��cachable write-back, allocate on writes only1100��reserved1101��reserved1110��cachable write-through, allocate on write & reads 1111��cachable write-back, allocate on write &reads */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL71_T;    /* �������ȼ����üĴ���1 */

typedef union
{
    struct
    {
        unsigned int    axi_mst_priority_m1        : 4; /* [3..0] dw_axi_mxt m1�ڣ�dw_ahb-UICC�����ȼ����á�ֵԽ�����ȼ�Խ�ߡ� */
        unsigned int    axi_mst_priority_m2        : 4; /* [7..4] dw_axi_mxt m2�ڣ�edmac_ch16�����ȼ����á�ֵԽ�����ȼ�Խ�ߡ� */
        unsigned int    axi_mst_priority_m3        : 4; /* [11..8] dw_axi_mxt m3�ڣ�cipher�����ȼ����á�ֵԽ�����ȼ�Խ�ߡ� */
        unsigned int    axi_mst_priority_m4        : 4; /* [15..12] dw_axi_mxt m4�ڣ�ipf�����ȼ����á�ֵԽ�����ȼ�Խ�ߡ� */
        unsigned int    axi_mst_priority_m5        : 4; /* [19..16] dw_axi_mxt m5�ڣ�UPACC�����ȼ����á�ֵԽ�����ȼ�Խ�ߡ� */
        unsigned int    axi_mst_priority_m6        : 4; /* [23..20] dw_axi_mxt m6�ڣ�CICOM0�����ȼ����á�ֵԽ�����ȼ�Խ�ߡ� */
        unsigned int    axi_mst_priority_m7        : 4; /* [27..24] dw_axi_mxt m7�ڣ�CICOM1�����ȼ����á�ֵԽ�����ȼ�Խ�ߡ� */
        unsigned int    axi_mst_priority_m8        : 4; /* [31..28] ���� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL72_T;    /* �������ȼ����üĴ���2��DW_axi_mst) */

typedef union
{
    struct
    {
        unsigned int    peri_mst_region1_ctrl      : 8; /* [7..0] REGION1���ʿ��ƣ�ÿ��bit��ʾһ��Master��Bit[0] - UICCBit[1] - EDMACBit[2] - CIPHERBit[3] - IPFBit[4] - UPACCBit[5] - CICOM0Bit[6] - CICOM1Bit[7] - ������ÿ��bit���壺1 - ��Master���Է��ʸõ�ַ�ռ䡣0 - ��master�����Է��ʸõ�ַ�ռ䡣 */
        unsigned int    peri_mst_secctrl_bypass    : 1; /* [8..8] 0 - ������������Է������пռ�(default)1 - ����������ķ��ʿռ��ܿأ�����μ�peri_mst_regionN_ctrl������ */
        unsigned int    reserved                   : 1; /* [9..9] ������ */
        unsigned int    peri_mst_region1_staddr    : 22; /* [31..10] REGION1�ռ���ʼ��ַ����1KBΪ��λ�� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL73_T;    /* ���������ʿռ���ƼĴ���0 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 10; /* [9..0] ������ */
        unsigned int    peri_mst_region1_endaddr   : 22; /* [31..10] REGION1�ռ������ַ����1KBΪ��λ�� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL74_T;    /* ���������ʿռ���ƼĴ���1 */

typedef union
{
    struct
    {
        unsigned int    peri_mst_region2_ctrl      : 8; /* [7..0] REGION1���ʿ��ƣ�ÿ��bit��ʾһ��Master��Bit[0] - UICCBit[1] - EDMACBit[2] - CIPHERBit[3] - IPFBit[4] - UPACCBit[5] - CICOM0Bit[6] - CICOM1Bit[7] - ������ÿ��bit���壺1 - ��Master���Է��ʸõ�ַ�ռ䡣0 - ��master�����Է��ʸõ�ַ�ռ䡣 */
        unsigned int    reserved                   : 2; /* [9..8] ������ */
        unsigned int    peri_mst_region2_staddr    : 22; /* [31..10] REGION2�ռ���ʼ��ַ����1KBΪ��λ�� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL75_T;    /* ���������ʿռ���ƼĴ���2 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 10; /* [9..0] ������ */
        unsigned int    peri_mst_region2_endaddr   : 22; /* [31..10] REGION2�ռ������ַ����1KBΪ��λ�� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL76_T;    /* ���������ʿռ���ƼĴ���3 */

typedef union
{
    struct
    {
        unsigned int    peri_mst_region3_ctrl      : 8; /* [7..0] REGION1���ʿ��ƣ�ÿ��bit��ʾһ��Master��Bit[0] - UICCBit[1] - EDMACBit[2] - CIPHERBit[3] - IPFBit[4] - UPACCBit[5] - CICOM0Bit[6] - CICOM1Bit[7] - ������ÿ��bit���壺1 - ��Master���Է��ʸõ�ַ�ռ䡣0 - ��master�����Է��ʸõ�ַ�ռ䡣 */
        unsigned int    reserved                   : 2; /* [9..8] ������ */
        unsigned int    peri_mst_region3_staddr    : 22; /* [31..10] REGION3�ռ���ʼ��ַ����1KBΪ��λ�� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL77_T;    /* ���������ʿռ���ƼĴ���4 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 10; /* [9..0] ������ */
        unsigned int    peri_mst_region3_endaddr   : 22; /* [31..10] REGION3�ռ������ַ����1KBΪ��λ�� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL78_T;    /* ���������ʿռ���ƼĴ���5 */

typedef union
{
    struct
    {
        unsigned int    peri_mst_region4_ctrl      : 8; /* [7..0] REGION1���ʿ��ƣ�ÿ��bit��ʾһ��Master��Bit[0] - UICCBit[1] - EDMACBit[2] - CIPHERBit[3] - IPFBit[4] - UPACCBit[5] - CICOM0Bit[6] - CICOM1Bit[7] - ������ÿ��bit���壺1 - ��Master���Է��ʸõ�ַ�ռ䡣0 - ��master�����Է��ʸõ�ַ�ռ䡣 */
        unsigned int    reserved                   : 2; /* [9..8] ������ */
        unsigned int    peri_mst_region4_staddr    : 22; /* [31..10] REGION4�ռ���ʼ��ַ����1KBΪ��λ�� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL79_T;    /* ���������ʿռ���ƼĴ���6 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 10; /* [9..0] ������ */
        unsigned int    peri_mst_region4_endaddr   : 22; /* [31..10] REGION1�ռ������ַ����1KBΪ��λ�� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL80_T;    /* ���������ʿռ���ƼĴ���7 */

typedef union
{
    struct
    {
        unsigned int    reserved_8                 : 1; /* [0..0] ������ */
        unsigned int    bbphy_dsp0_axislv_active_mask : 1; /* [1..1] dw_axi_bbyphy����dsp0 axi slave��active�ź����ο��ơ�1 - ����ʹ��0 - ���β�ʹ�ܡ� */
        unsigned int    bbphy_bbp_axislv_active_mask : 1; /* [2..2] dw_axi_bbyphy����bbp axi slave��active�ź����ο��ƣ�LTEBBP/TWBBP/BBC_2A/BBC_2B)����1 - ����ʹ��0 - ���β�ʹ�ܡ� */
        unsigned int    reserved_7                 : 1; /* [3..3] ������ */
        unsigned int    reserved_6                 : 4; /* [7..4] ������ */
        unsigned int    mdmacp_axislv_active_mask  : 1; /* [8..8] dw_axi_glb����mdma9 ACP slave��active�ź����μĴ����� */
        unsigned int    reserved_5                 : 2; /* [10..9] ���� */
        unsigned int    axi_mem_axislv_active_mask : 1; /* [11..11] dw_axi_glb����axi mem slave��active�ź����ο��ơ�1 - ����ʹ��0 - ���β�ʹ�ܡ� */
        unsigned int    reserved_4                 : 1; /* [12..12] ���� */
        unsigned int    bbphy_axislv_active_mask   : 1; /* [13..13] dw_axi_glb����bbephy�� slave��active�ź����ο��ơ�1 - ����ʹ��0 - ���β�ʹ�ܡ� */
        unsigned int    reserved_3                 : 1; /* [14..14] ���� */
        unsigned int    wbbp_axislv_active_mask    : 1; /* [15..15] dw_axi_glb����WBBP/G1BBP/G2BBP slave��active�ź����ο��ơ�1 - ����ʹ��0 - ���β�ʹ�ܡ� */
        unsigned int    amon_axislv_active_mask    : 1; /* [16..16] dw_axi_glb����amon_slave��active�ź����ο��ơ�1 - ����ʹ��0 - ���β�ʹ�ܡ� */
        unsigned int    reserved_2                 : 2; /* [18..17] ������ */
        unsigned int    wdog_apbslv_active_mask    : 1; /* [19..19] wdog apb slave��active�ź����ο��ơ�1 - ����ʹ��0 - ���β�ʹ�ܡ� */
        unsigned int    timer0_7_apbslv_active_mask : 1; /* [20..20] timer0_7 apb slave��active�ź����ο��ơ�1 - ����ʹ��0 - ���β�ʹ�ܡ� */
        unsigned int    reserved_1                 : 4; /* [24..21] ������ */
        unsigned int    uart0_apbslv_active_mask   : 1; /* [25..25] uart0 apb slave��active�ź����ο��ơ�1 - ����ʹ��0 - ���β�ʹ�ܡ� */
        unsigned int    edmac_slv_active_mask      : 1; /* [26..26] edma  apb slave��active�ź����ο��ơ�1 - ����ʹ��0 - ���β�ʹ�ܡ� */
        unsigned int    reserved_0                 : 5; /* [31..27] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL103_T;    /* slave active���μĴ���0 */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 32; /* [31..0] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL104_T;    /* �����Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    uicc                       : 1; /* [0..0] uicc ahb slave��active�ź����μĴ�����0�������Σ������ڲ�����״̬���ر�ʱ�ӣ���λ��ʱ�����ʸ�ģ�齫����error �����ϱ��жϣ�ͬʱ��¼��ַ��Ϣ��1������active�߼���slave�̶�Ϊ�ɷ���״̬�� */
        unsigned int    reserved_6                 : 5; /* [5..1] ���� */
        unsigned int    ipcm                       : 1; /* [6..6] ipcm ahb slave��active�ź����μĴ�����0�������Σ������ڲ�����״̬���ر�ʱ�ӣ���λ��ʱ�����ʸ�ģ�齫����error �����ϱ��жϣ�ͬʱ��¼��ַ��Ϣ��1������active�߼���slave�̶�Ϊ�ɷ���״̬�� */
        unsigned int    reserved_5                 : 1; /* [7..7] ���� */
        unsigned int    hs_uart                    : 1; /* [8..8] hs uart ahb slave��active�ź����μĴ�����0�������Σ������ڲ�����״̬���ر�ʱ�ӣ���λ��ʱ�����ʸ�ģ�齫����error �����ϱ��жϣ�ͬʱ��¼��ַ��Ϣ��1������active�߼���slave�̶�Ϊ�ɷ���״̬�� */
        unsigned int    reserved_4                 : 2; /* [10..9] ���� */
        unsigned int    cipher                     : 1; /* [11..11] cipher ahb slave��active�ź����μĴ�����0�������Σ������ڲ�����״̬���ر�ʱ�ӣ���λ��ʱ�����ʸ�ģ�齫����error �����ϱ��жϣ�ͬʱ��¼��ַ��Ϣ��1������active�߼���slave�̶�Ϊ�ɷ���״̬�� */
        unsigned int    ipf                        : 1; /* [12..12] ipf ahb slave��active�ź����μĴ�����0�������Σ������ڲ�����״̬���ر�ʱ�ӣ���λ��ʱ�����ʸ�ģ�齫����error �����ϱ��жϡ�1������active�߼���slave�̶�Ϊ�ɷ���״̬�� */
        unsigned int    reserved_3                 : 6; /* [18..13] ���� */
        unsigned int    cicom0                     : 1; /* [19..19] cicom0 ahb slave��active�ź����μĴ�����0�������Σ������ڲ�����״̬���ر�ʱ�ӣ���λ��ʱ�����ʸ�ģ�齫����error �����ϱ��жϡ�1������active�߼���slave�̶�Ϊ�ɷ���״̬�� */
        unsigned int    cicom1                     : 1; /* [20..20] cicom1 ahb slave��active�ź����μĴ�����0�������Σ������ڲ�����״̬���ر�ʱ�ӣ���λ��ʱ�����ʸ�ģ�齫����error �����ϱ��жϣ�ͬʱ��¼��ַ��Ϣ��1������active�߼���slave�̶�Ϊ�ɷ���״̬�� */
        unsigned int    reserved_2                 : 4; /* [24..21] ���� */
        unsigned int    vic1                       : 1; /* [25..25] vic1 ahb slave��active�ź����μĴ�����0�������Σ������ڲ�����״̬���ر�ʱ�ӣ���λ��ʱ�����ʸ�ģ�齫����error �����ϱ��жϣ�ͬʱ��¼��ַ��Ϣ��1������active�߼���slave�̶�Ϊ�ɷ���״̬�� */
        unsigned int    reserved_1                 : 3; /* [28..26] ���� */
        unsigned int    upacc                      : 1; /* [29..29] upacc ahb slave��active�ź����μĴ�����0�������Σ������ڲ�����״̬���ر�ʱ�ӣ���λ��ʱ�����ʸ�ģ�齫����error �����ϱ��жϣ�ͬʱ��¼��ַ��Ϣ��1������active�߼���slave�̶�Ϊ�ɷ���״̬�� */
        unsigned int    reserved_0                 : 2; /* [31..30] ���� */
    } bits;
    unsigned int    u32;
}HI_SC_CTRL105_T;    /* slave active���μĴ���2 */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 14; /* [13..0] ������ */
        unsigned int    moda9_standbywfi           : 1; /* [14..14] ModA9 WFI״ָ̬ʾ��0����WFIģʽ��1������WFIģʽ�� */
        unsigned int    moda9_standbywfe           : 1; /* [15..15] ModA9 WFE״ָ̬ʾ��0����WFEģʽ��1������WFEģʽ�� */
        unsigned int    moda9_pmupriv              : 1; /* [16..16] ModA9״ָ̬ʾ��0���û�ģʽ��1����Ȩģʽ���ñ�����Ϣ��CoreSight�в��ṩ�� */
        unsigned int    moda9_pmusecure            : 1; /* [17..17] ModA9��ȫ״̬��0���ǰ�ȫ״̬��1����ȫ״̬���ñ�����Ϣ��CoreSight�в��ṩ�� */
        unsigned int    moda9_smpnamp              : 1; /* [18..18] ModA9 AMP/SMPģʽ��0�����Գƣ�1���Գơ� */
        unsigned int    moda9_scuevabort           : 1; /* [19..19] �쳣��־�źţ�ָʾ��һ��������ڼ��ⲿ��������ֹ����������CPU���ܼ�أ��������¿������ա�0��δ����1���ѷ��� */
        unsigned int    moda9_pwrctlo0             : 2; /* [21..20] CPU��ǰ����״̬��0x��CPU0�����ϵ磻10��CPU0���Խ���dormant mode��11��CPU0���Խ���powered-off mode�� */
        unsigned int    moda9_l2_tagclkouten       : 1; /* [22..22] l2 tagclkouten״̬�� */
        unsigned int    moda9_l2_dataclkouten      : 4; /* [26..23] l2 dataclkouten״̬�� */
        unsigned int    moda9_l2_idle              : 1; /* [27..27] l2 idle״̬�� */
        unsigned int    moda9_l2_clkstopped        : 1; /* [28..28] l2 clk_stopped ״̬�� */
        unsigned int    reserved_0                 : 3; /* [31..29] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_STAT1_T;    /* Modem A9״̬�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 16; /* [15..0] ������ */
        unsigned int    dsp0_pwaitmode             : 1; /* [16..16] ������˯��״ָ̬ʾ��0��������1��˯�ߡ���DSPִ��WAITIָ��ȴ��ж�ʱ�����ź������κ�û��disable���ж϶��ỽ�Ѵ������� */
        unsigned int    dsp0_xocdmode              : 1; /* [17..17] ָʾ����������OCD Haltģʽ��0��������1��OCD Haltģʽ�� */
        unsigned int    reserved_0                 : 14; /* [31..18] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_STAT2_T;    /* BBE16 DSP״̬�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    hpmx_opc                   : 10; /* [9..0] hpmx_opc��ֵ */
        unsigned int    hpmx_opc_vld               : 1; /* [10..10] hpm_opc��Чָʾ�źš�0����Ч��1����Ч�� */
        unsigned int    reserved_1                 : 1; /* [11..11] ���� */
        unsigned int    hpm_opc                    : 10; /* [21..12] hpm_opc��ֵ */
        unsigned int    hpm_opc_vld                : 1; /* [22..22] hpm_opc��Чָʾ�źš�0����Ч��1����Ч�� */
        unsigned int    reserved_0                 : 9; /* [31..23] ���� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT3_T;    /* HPM״̬�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp0_nmi                   : 1; /* [0..0] DSP�������ж� */
        unsigned int    intr_ipc_ns                : 1; /* [1..1] AP_IPCM_NS��BBE16�Ŀ����ж� */
        unsigned int    intr_ipc_ns_mbx            : 1; /* [2..2] AP_IPCM_NS��BBE16�������ж� */
        unsigned int    dsp_uart_int               : 1; /* [3..3] HS_UART��UART0������жϡ� */
        unsigned int    edmac_int1                 : 1; /* [4..4] EDMAC�ж�1 */
        unsigned int    lte_dsp_aagc_int           : 1; /* [5..5] AAGC�ϱ��ж� */
        unsigned int    lte_dsp_cell_int           : 1; /* [6..6] С�������ж� */
        unsigned int    lte_dsp_vdl_int            : 1; /* [7..7] viterbi������·�ж� */
        unsigned int    lte_dsp_cmu_int            : 1; /* [8..8] ���������ϱ��ж� */
        unsigned int    lte_dsp_pwrm_int           : 1; /* [9..9] ���������ϱ��ж� */
        unsigned int    lte_dsp_cfi_int            : 1; /* [10..10] CFI�ĸ����ж� */
        unsigned int    lte_dsp_tdl_int            : 1; /* [11..11] Turbo������·�ж� */
        unsigned int    lte_dsp_bbp_dma_int        : 1; /* [12..12] DMA����ͨ�����ͨ�� */
        unsigned int    lte_dsp_pub_int            : 1; /* [13..13] �ϲ��ж� */
        unsigned int    lte_dsp_synb_int           : 1; /* [14..14] DSP�����ж� */
        unsigned int    tds_stu_sfrm_int           : 1; /* [15..15] STU��֡�ж� */
        unsigned int    tds_hsupa_int              : 1; /* [16..16] TDS HSUPA����ж� */
        unsigned int    tds_harq_int               : 1; /* [17..17] TDS HARQ����ж� */
        unsigned int    tds_turbo_int              : 1; /* [18..18] TDS Turbo����ж� */
        unsigned int    tds_viterbi_int            : 1; /* [19..19] TDS Viterbi����ж� */
        unsigned int    tds_rfc_int                : 1; /* [20..20] ��Ƶǰ���ж� */
        unsigned int    tds_fpu_int                : 1; /* [21..21] ǰ���ж� */
        unsigned int    tds_stu_dsp_int            : 1; /* [22..22] STU��֡�жϺ�ʱ϶�ж� */
        unsigned int    ctu_int_lte                : 1; /* [23..23] LTEģCTU�ж� */
        unsigned int    ctu_int_tds                : 1; /* [24..24] TDSģCTU�ж� */
        unsigned int    dsp_vic_int                : 1; /* [25..25] DSP VIC�ж� */
        unsigned int    reserved                   : 6; /* [31..26] ���� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT5_T;    /* BBE16�ж�״̬�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 12; /* [11..0] ������ */
        unsigned int    moda9_parityfail0          : 8; /* [19..12] A9 RAM����У������0��У��ɹ���1��У��ʧ�ܡ�[7] BTAC parity error[6] GHB[5] Instruction tag RAM[4] Instruction data RAM[3] Main TLB[2] D outer RAM[1] Data tag RAM[0] Data data RAM�ð汾��֧�ָù��ܣ��̶�Ϊ0 */
        unsigned int    moda9_parityfailscu        : 1; /* [20..20] SCU tag RAMУ������0��У��ɹ���1��У��ʧ�ܡ��ð汾��֧�ָù��ܣ��̶�Ϊ0 */
        unsigned int    reserved_0                 : 11; /* [31..21] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_STAT6_T;    /* Modem A9У��״̬�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dw_axi_mst_dlock_wr        : 1; /* [0..0] ���������дָʾ��0������1��д�� */
        unsigned int    dw_axi_mst_dlock_slv       : 1; /* [1..1] ��������master��slave�˿ںš� */
        unsigned int    reserved_2                 : 2; /* [3..2] ������ */
        unsigned int    dw_axi_mst_dlock_mst       : 4; /* [7..4] ������master��С�˿ںš� */
        unsigned int    reserved_1                 : 12; /* [19..8] ������ */
        unsigned int    dw_axi_glb_dlock_wr        : 1; /* [20..20] ���������дָʾ��0������1��д�� */
        unsigned int    dw_axi_glb_dlock_slv       : 4; /* [24..21] ��������master��slave�˿ںš� */
        unsigned int    dw_axi_glb_dlock_mst       : 3; /* [27..25] ������master��С�˿ںš� */
        unsigned int    reserved_0                 : 4; /* [31..28] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_STAT9_T;    /* AXI������״̬�Ĵ���0 */

typedef union
{
    struct
    {
        unsigned int    dw_axi_glb_dlock_id        : 8; /* [7..0] GLB AXI������ID�� */
        unsigned int    reserved                   : 20; /* [27..8] ������ */
        unsigned int    dw_axi_mst_dlock_id        : 4; /* [31..28] MST AXI������ID�� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT10_T;    /* AXI������״̬�Ĵ���1 */

typedef union
{
    struct
    {
        unsigned int    apb_pslv_active0           : 9; /* [8..0] apb slave��active�ź�״̬0��bit[0]��1'b1��������bit[1]��wdogbit[2]��timer0��7bit[3]��1'b1(������bit[4]��1'b1(������bit[5]��1'b1(������bit[6]��1'b1(������bit[7]��uart0bit[8]��edmac */
        unsigned int    reserved                   : 23; /* [31..9] �����������س�1 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT15_T;    /* slave active״̬�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dw_axi_bbphy_dlock_mst     : 2; /* [1..0] ������������ʱ����¼����δ��ɵ���Сmaster�š�0 - axi_glb1 - BBE16 DSP2 - BBP_MST13 - BBP_MST2 */
        unsigned int    dw_axi_bbphy_dlock_slv     : 3; /* [4..2] dlock_mst master���ڷ��ʵ�Slave�š�0 - axi_glb1 - BBE16 DSP2 - BBP3 - ���� */
        unsigned int    dw_axi_bbphy_dlock_wr      : 1; /* [5..5] ���������дָʾ��0������1��д�����������������ʱ����д������δ��ɣ����ź�ָʾд������ */
        unsigned int    reserved_1                 : 2; /* [7..6] ������ */
        unsigned int    dw_axi_bbphy_dlock_id      : 12; /* [19..8] ��������ʱ����¼��ID�š� */
        unsigned int    reserved_0                 : 12; /* [31..20] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_STAT22_T;    /* AXI������״̬�Ĵ���2(BBPHY) */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 1; /* [0..0] ������ */
        unsigned int    ipf_idle                   : 1; /* [1..1] ipf�Ƿ����״̬ */
        unsigned int    cicom0_clk_state           : 1; /* [2..2] cicom0�Ƿ����״̬ */
        unsigned int    cicom1_clk_state           : 1; /* [3..3] cicom1�Ƿ����״̬ */
        unsigned int    reserved_0                 : 28; /* [31..4] hao */
    } bits;
    unsigned int    u32;
}HI_SC_STAT26_T;    /* master״̬�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    bbphy_slv_active           : 4; /* [3..0] dw_axi_bbphy��slave��active�ź�״̬��bit[0]��dw_axi_glbbit[1]��bbe16 dsp corebit[2]��bbpbit[3]��reserved */
        unsigned int    reserved_1                 : 4; /* [7..4] ������ */
        unsigned int    glb_slv_active             : 9; /* [16..8] dw_axi_glb��slave��active�ź�״̬��bit[8]��mdm acpbit[9]��reserved������1��bit[10]������������1��bit[11]��axi_membit[12]��x2h_peribit[13]��dw_axi_dspbit[14]��x2p������1��bit[15]������������1��bit[16]��axi_monitor */
        unsigned int    reserved_0                 : 15; /* [31..17] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_STAT27_T;    /* slave active״̬�Ĵ���0 */

typedef union
{
    struct
    {
        unsigned int    x2h_peri_slv_active        : 32; /* [31..0] bit[0]��uiccbit[5:1]���������̶�Ϊ1bit[6]��ipcmbit[7]���������̶�Ϊ1bit[8]��hs_uartbit[10:9]��reservedbit[11]��cipherbit[12]��ipfbit[18:13]��reservedbit[19]��cicom0bit[20]��cicom1bit[24:21]��reservedbit[25]��bbe_vicbit[28:26]��reservedbit[29]��upacc                                          bit[30]��reservedbit[31]��reserved */
    } bits;
    unsigned int    u32;
}HI_SC_STAT29_T;    /* slave active״̬�Ĵ���2 */

typedef union
{
    struct
    {
        unsigned int    lte2soc_tbd                : 16; /* [15..0] sc_ctrl57[15:0] */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT32_T;    /* BBP����״̬�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    ap2mdm_key0                : 32; /* [31..0] Key0 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT35_T;    /* ��ȫKey�Ĵ���0 */

typedef union
{
    struct
    {
        unsigned int    ap2mdm_key0                : 32; /* [31..0] Key1 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT36_T;    /* ��ȫKey�Ĵ���1 */

typedef union
{
    struct
    {
        unsigned int    ap2mdm_key2                : 32; /* [31..0] Key2 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT37_T;    /* ��ȫKey�Ĵ���2 */

typedef union
{
    struct
    {
        unsigned int    ap2mdm_key3                : 32; /* [31..0] Key3 */
    } bits;
    unsigned int    u32;
}HI_SC_STAT38_T;    /* ��ȫKey�Ĵ���3 */

typedef union
{
    struct
    {
        unsigned int    bbphy_mst_err              : 8; /* [7..0] dw_axi_bbphy�ŷ��ʴ���Ĵ�����Bit[0]��m1�ڣ�dw_axi_glb��д���ʴ���Bit[1]��m1�ڣ�dw_axi_glb�������ʴ���Bit[2]��m2�ڣ�dsp0��д���ʴ���Bit[3]��m2�ڣ�dsp0�������ʴ���Bit[4]��m3�ڣ�bbp��д���ʴ���Bit[5]��m3�ڣ�bbp�������ʴ���Bit[6]��m4�ڣ�reserved��д���ʴ���Bit[7]��m4�ڣ�reserved�������ʴ��� */
        unsigned int    reserved                   : 12; /* [19..8] ������ */
        unsigned int    glb_mst_err                : 12; /* [31..20] dw_axi_glb�ŷ��ʴ���Ĵ�����Bit[0]��m1�ڣ�dw_axi_bbphy��д���ʴ���Bit[1]��m1�ڣ�dw_axi_bbphy�������ʴ���Bit[2]��m2�ڣ�dw_axi_mst��д���ʴ���Bit[3]��m2�ڣ�dw_axi_mst�������ʴ���Bit[4]��m3�ڣ�app a9 m0�ڣ�д���ʴ���Bit[5]��m3�ڣ�app a9 m0�ڣ������ʴ���Bit[6]��m4�ڣ�mdm a9 m0�ڣ�д���ʴ���Bit[7]��m4�ڣ�mdm a9 m0�ڣ������ʴ���Bit[8]��m5�ڣ�reserved��д���ʴ���Bit[9]��m5�ڣ�reserved�������ʴ���Bit[10]��m6�ڣ�hifi��д���ʴ���Bit[11]��m6�ڣ�hifi�������ʴ��� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT41_T;    /* master���ʴ���״̬�Ĵ���0�� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 1; /* [0..0] ���� */
        unsigned int    socapb_psel_err            : 1; /* [1..1] ��master���ʲ�����(�ر�ʱ�ӣ���λ������)��APB����ʱ������״ָ̬ʾ����Bitֻ���ڶ�Ӧ�����slv_acitve_mask��SC_CTRL103��λ����Ϊ������ʱ�Ż������ */
        unsigned int    ahb_peri_mst_err           : 1; /* [2..2] ��master���ʲ�����(�ر�ʱ�ӣ���λ������)��APB����ʱ������״ָ̬ʾ����Bitֻ���ڶ�Ӧ�����slv_acitve_mask��SC_CTRL105��λ����Ϊ������ʱ�Ż������ */
        unsigned int    reserved_0                 : 29; /* [31..3] ������ */
    } bits;
    unsigned int    u32;
}HI_SC_STAT42_T;    /* master���ʴ���״̬�Ĵ���1�� */

typedef union
{
    struct
    {
        unsigned int    x2h_peri_addr_err          : 32; /* [31..0] ahb������ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT43_T;    /* ahb������ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    reserved                   : 12; /* [11..0] ������ */
        unsigned int    addr_err                   : 20; /* [31..12] soc���������apb������ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT44_T;    /* apb������ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb��m1�ڣ�dw_axi_bbphy��д���ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT46_T;    /* glb��m1�ڣ�dw_axi_bbphy��д���ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb��m1�ڣ�dw_axi_bbphy�������ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT47_T;    /* glb��m1�ڣ�dw_axi_bbphy�������ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb��m2�ڣ�dw_axi_mst��д���ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT48_T;    /* glb��m2�ڣ�dw_axi_mst��д���ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb��m2�ڣ�dw_axi_mst�������ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT49_T;    /* glb��m2�ڣ�dw_axi_mst�������ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb��m3�ڣ�app a9 m0�ڣ�д���ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT50_T;    /* glb��m3�ڣ�app a9 m0�ڣ�д���ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb��m3�ڣ�app a9 m0�ڣ������ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT51_T;    /* glb��m3�ڣ�app a9 m0�ڣ������ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb��m4�ڣ�mdm a9 m0�ڣ�д���ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT52_T;    /* glb��m4�ڣ�mdm a9 m0�ڣ�д���ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb��m4�ڣ�mdm a9 m0�ڣ������ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT53_T;    /* glb��m4�ڣ�mdm a9 m0�ڣ������ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb��m5�ڣ�reserved��д���ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT54_T;    /* glb��m5�ڣ�reserved��д���ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb��m5�ڣ�reserved�������ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT55_T;    /* glb��m5�ڣ�reserved�������ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb��m6�ڣ�hifi��д���ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT56_T;    /* glb��m6�ڣ�hifi��д���ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] glb��m6�ڣ�hifi�������ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT57_T;    /* glb��m6�ڣ�hifi�������ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] bbphy��m1�ڣ�dw_axi_glb��д���ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT62_T;    /* bbphy��m1�ڣ�dw_axi_glb��д���ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] bbphy��m1�ڣ�dw_axi_glb�������ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT63_T;    /* bbphy��m1�ڣ�dw_axi_glb�������ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] bbphy��m2�ڣ�dsp0��д���ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT64_T;    /* bbphy��m2�ڣ�dsp0��д���ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] bbphy��m2�ڣ�dsp0�������ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT65_T;    /* bbphy��m2�ڣ�dsp0�������ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] bbphy��m3�ڣ�bbp��д���ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT66_T;    /* bbphy��m3�ڣ�bbp��д���ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] bbphy��m3�ڣ�bbp�������ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT67_T;    /* bbphy��m3�ڣ�bbp�������ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] bbphy��m4�ڣ�bbp��д���ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT68_T;    /* bbphy��m4�ڣ�bbp��д���ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    addr_err                   : 32; /* [31..0] bbphy��m4�ڣ�bbp�������ʴ����ַ�Ĵ����� */
    } bits;
    unsigned int    u32;
}HI_SC_STAT69_T;    /* bbphy��m4�ڣ�bbp�������ʴ����ַ�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 20; /* [19..0] ������ */
        unsigned int    sc_tcxo0_en_ctrl           : 1; /* [20..20] TCXO0 BUFFERʹ�ܿ��ơ�1 - ʹ�ܡ�0 - ��ʹ�� */
        unsigned int    sc_tcxo1_en_ctrl           : 1; /* [21..21] TCXO1 BUFFERʹ�ܿ��ơ�1 - ʹ�ܡ�0 - ��ʹ�� */
        unsigned int    reserved_0                 : 9; /* [30..22] ���� */
        unsigned int    sc_tcxo_en_ctrl            : 1; /* [31..31] Modem��ϵͳSOC����TCXOʱ��ѡ����ơ�0 - ʹ������AP��TCXOʹ�ܿ��ƣ�tcxo��_clk_en��1 - ѡ��Modemϵͳ��ϵͳ���ƼĴ������μ�bit20��21˵���� */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL2_T;    /* TCXO���ƼĴ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_3                 : 1; /* [0..0] ���� */
        unsigned int    ccpu_iso_ctrl_en           : 1; /* [1..1] ModemA9��Դ�� ISOʹ�ܿ��ơ�1 - ����ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    reserved_2                 : 2; /* [3..2] ���� */
        unsigned int    bbe16_iso_ctrl_en          : 1; /* [4..4] BBEDSP��Դ�� ISOʹ�ܿ��ơ�1 - ����ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    reserved_1                 : 1; /* [5..5] ���� */
        unsigned int    ltebbp_iso_ctrl_en         : 1; /* [6..6] LTEBBP��Դ�� ISOʹ�ܿ��ơ�1 - ����ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    twbbp_iso_ctrl_en          : 1; /* [7..7] TWBBP��Դ�� ISOʹ�ܿ��ơ�1 - ����ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    wbbp_iso_ctrl_en           : 1; /* [8..8] WBBP��Դ�� ISOʹ�ܿ��ơ�1 - ����ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    g1bbp_iso_ctrl_en          : 1; /* [9..9] G1BBP��Դ�� ISOʹ�ܿ��ơ�1 - ����ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    g2bbp_iso_ctrl_en          : 1; /* [10..10] G2BBP��Դ�� ISOʹ�ܿ��ơ�1 - ����ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    irmbbp_iso_ctrl_en         : 1; /* [11..11] IRMBBP��Դ�� ISOʹ�ܿ��ơ�1 - ����ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    reserved_0                 : 20; /* [31..12] ���� */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL4_T;    /* ISOCELL����ʹ�ܿ��ƼĴ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_3                 : 1; /* [0..0] ���� */
        unsigned int    ccpu_iso_ctrl_dis          : 1; /* [1..1] ModemA9��Դ�� ISOʹ��ֹ���ơ�1 - �����ֹ��0 - ��Ӱ�졣 */
        unsigned int    reserved_2                 : 2; /* [3..2] ���� */
        unsigned int    bbe16_iso_ctrl_dis         : 1; /* [4..4] BBEDSP��Դ�� ISO��ֹ���ơ�1 - �����ֹ��0 - ��Ӱ�졣 */
        unsigned int    reserved_1                 : 1; /* [5..5] ���� */
        unsigned int    ltebbp_iso_ctrl_dis        : 1; /* [6..6] LTEBBP��Դ�� ISO��ֹ���ơ�1 - �����ֹ��0 - ��Ӱ�졣 */
        unsigned int    twbbp_iso_ctrl_dis         : 1; /* [7..7] TWBBP��Դ�� ISO��ֹ���ơ�1 - �����ֹ��0 - ��Ӱ�졣 */
        unsigned int    wbbp_iso_ctrl_dis          : 1; /* [8..8] WBBP��Դ�� ISO��ֹ���ơ�1 - �����ֹ��0 - ��Ӱ�졣 */
        unsigned int    g1bbp_iso_ctrl_dis         : 1; /* [9..9] G1BBP��Դ�� ISO��ֹ���ơ�1 - �����ֹ��0 - ��Ӱ�졣 */
        unsigned int    g2bbp_iso_ctrl_dis         : 1; /* [10..10] G2BBP��Դ�� ISO��ֹ���ơ�1 - ��ֹ��0 - ��Ӱ�졣 */
        unsigned int    irmbbp_iso_ctrl_dis        : 1; /* [11..11] IRMBBP��Դ�� ISO��ֹ���ơ�1 - �����ֹ��0 - ��Ӱ�졣 */
        unsigned int    reserved_0                 : 20; /* [31..12] ���� */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL5_T;    /* ISOCELL����رտ��ƼĴ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_3                 : 1; /* [0..0] ���� */
        unsigned int    ccpu_mtcmos_ctrl_en        : 1; /* [1..1] ModemA9��Դ�����µ�ʹ�ܿ��ơ�1 - �ϵ�ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    reserved_2                 : 2; /* [3..2] ���� */
        unsigned int    bbe16_mtcmos_ctrl_en       : 1; /* [4..4] BBEDSP��Դ�����µ�ʹ�ܿ��ơ�1 - �ϵ�ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    reserved_1                 : 1; /* [5..5] ���� */
        unsigned int    ltebbp_mtcmos_ctrl_en      : 1; /* [6..6] LTEBBP��Դ�����µ�ʹ�ܿ��ơ�1 - �ϵ�ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    twbbp_mtcmos_ctrl_en       : 1; /* [7..7] TWBBP��Դ�����µ�ʹ�ܿ��ơ�1 - �ϵ�ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    wbbp_mtcmos_ctrl_en        : 1; /* [8..8] WBBP��Դ�� ���µ�ʹ�ܿ��ơ�1 - �ϵ�ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    g1bbp_mtcmos_ctrl_en       : 1; /* [9..9] G1BBP��Դ�� ���µ�ʹ�ܿ��ơ�1 - �ϵ�ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    g2bbp_mtcmos_ctrl_en       : 1; /* [10..10] G2BBP��Դ�����µ�ʹ�ܿ��ơ�1 - �ϵ�ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    irmbbp_mtcmos_ctrl_en      : 1; /* [11..11] IRMBBP��Դ�����µ�ʹ�ܿ��ơ�1 - �ϵ�ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    reserved_0                 : 20; /* [31..12] ���� */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL6_T;    /* MTCMOS�ϵ�ʹ�ܿ��ƼĴ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_3                 : 1; /* [0..0] ���� */
        unsigned int    ccpu_mtcmos_ctrl_dis       : 1; /* [1..1] ModemA9��Դ���µ�ʹ�ܿ��ơ�1 - �µ�ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    reserved_2                 : 2; /* [3..2] ���� */
        unsigned int    bbe16_mtcmos_ctrl_dis      : 1; /* [4..4] BBEDSP��Դ���µ�ʹ�ܿ��ơ�1 - �µ�ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    reserved_1                 : 1; /* [5..5] ���� */
        unsigned int    ltebbp_mtcmos_ctrl_dis     : 1; /* [6..6] LTEBBP��Դ���µ�ʹ�ܿ��ơ�1 - �µ�ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    twbbp_mtcmos_ctrl_dis      : 1; /* [7..7] TWBBP��Դ���µ�ʹ�ܿ��ơ�1 - �µ�ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    wbbp_mtcmos_ctrl_dis       : 1; /* [8..8] WBBP��Դ�� �µ�ʹ�ܿ��ơ�1 - �µ�ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    g1bbp_mtcmos_ctrl_dis      : 1; /* [9..9] G1BBP��Դ�� �µ�ʹ�ܿ��ơ�1 - �µ�ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    g2bbp_mtcmos_ctrl_dis      : 1; /* [10..10] G2BBP��Դ���µ�ʹ�ܿ��ơ�1 - �µ�ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    irmbbp_mtcmos_ctrl_dis     : 1; /* [11..11] IRMBBP��Դ���µ�ʹ�ܿ��ơ�1 - �µ�ʹ�ܣ�0 - ��Ӱ�졣 */
        unsigned int    reserved_0                 : 20; /* [31..12] ���� */
    } bits;
    unsigned int    u32;
}HI_PWR_CTRL7_T;    /* MTCMOS�µ�ʹ�ܿ��ƼĴ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_4                 : 1; /* [0..0] ���� */
        unsigned int    ccpu_mtcmos_rdy_stat       : 1; /* [1..1] ModemA9��Դ�����µ�״ָ̬ʾ��1 - �ϵ�״̬��0 - �µ�״̬�� */
        unsigned int    reserved_3                 : 2; /* [3..2] ���� */
        unsigned int    bbe16_mtcmos_rdy_stat      : 1; /* [4..4] BBEDSP��Դ�����µ�״ָ̬ʾ��1 - �ϵ�״̬��0 - �µ�״̬�� */
        unsigned int    reserved_2                 : 1; /* [5..5] ���� */
        unsigned int    ltebbp_mtcmos_rdy_stat     : 1; /* [6..6] LTEBBP��Դ�����µ�״ָ̬ʾ��1 - �ϵ�״̬��0 - �µ�״̬�� */
        unsigned int    twbbp_mtcmos_rdy_stat      : 1; /* [7..7] TWBBP��Դ�����µ�״ָ̬ʾ��1 - �ϵ�״̬��0 - �µ�״̬�� */
        unsigned int    wbbp_mtcmos_rdy_stat       : 1; /* [8..8] WBBP��Դ�����µ�״ָ̬ʾ��1 - �ϵ�״̬��0 - �µ�״̬�� */
        unsigned int    g1bbp_mtcmos_rdy_stat      : 1; /* [9..9] G1BBP��Դ�����µ�״ָ̬ʾ��1 - �ϵ�״̬��0 - �µ�״̬�� */
        unsigned int    g2bbp_mtcmos_rdy_stat      : 1; /* [10..10] G2BBP��Դ�����µ�״ָ̬ʾ��1 - �ϵ�״̬��0 - �µ�״̬�� */
        unsigned int    irmbbp_mtcmos_rdy_stat     : 1; /* [11..11] IRMBBP��Դ�����µ�״ָ̬ʾ��1 - �ϵ�״̬��0 - �µ�״̬�� */
        unsigned int    reserved_1                 : 2; /* [13..12] ������ */
        unsigned int    ltebbp0_mtcmos_rdy_stat    : 1; /* [14..14] LTEBBP0��Դ�����µ�״ָ̬ʾ��1 - �ϵ�״̬��0 - �µ�״̬��LTEBBP0��LTEBBPΪͬһ����Դ�򣬲���ͬһ�����µ�͸�����ơ� */
        unsigned int    reserved_0                 : 17; /* [31..15] ���� */
    } bits;
    unsigned int    u32;
}HI_PWR_STAT1_T;    /* MTCMOS_RDY״ָ̬ʾ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_3                 : 1; /* [0..0] ���� */
        unsigned int    ccpu_iso_ctrl_stat         : 1; /* [1..1] ModemA9��Դ�� ISO����״ָ̬ʾ��1 - ����ʹ�ܣ�0 - �����ֹ�� */
        unsigned int    reserved_2                 : 2; /* [3..2] ���� */
        unsigned int    bbe16_iso_ctrl_stat        : 1; /* [4..4] BBEDSP��Դ�� ISO����״ָ̬ʾ��1 - ����ʹ�ܣ�0 - �����ֹ�� */
        unsigned int    reserved_1                 : 1; /* [5..5] ���� */
        unsigned int    ltebbp_iso_ctrl_stat       : 1; /* [6..6] LTEBBP��Դ�� ISO����״ָ̬ʾ��1 - ����ʹ�ܣ�0 - �����ֹ�� */
        unsigned int    twbbp_iso_ctrl_stat        : 1; /* [7..7] TWBBP��Դ�� ISO����״ָ̬ʾ��1 - ����ʹ�ܣ�0 - �����ֹ�� */
        unsigned int    wbbp_iso_ctrl_stat         : 1; /* [8..8] WBBP��Դ�� ISO����״ָ̬ʾ��1 - ����ʹ�ܣ�0 - �����ֹ�� */
        unsigned int    g1bbp_iso_ctrl_stat        : 1; /* [9..9] G1BBP��Դ�� ISO����״ָ̬ʾ��1 - ����ʹ�ܣ�0 - �����ֹ�� */
        unsigned int    g2bbp_iso_ctrl_stat        : 1; /* [10..10] G2BBP��Դ�� ISO����״ָ̬ʾ��1 - ����ʹ�ܣ�0 - �����ֹ�� */
        unsigned int    irmbbp_iso_ctrl_stat       : 1; /* [11..11] IRMBBP��Դ�� ISO����״ָ̬ʾ��1 - ����ʹ�ܣ�0 - �����ֹ�� */
        unsigned int    reserved_0                 : 20; /* [31..12] ���� */
    } bits;
    unsigned int    u32;
}HI_PWR_STAT3_T;    /* ISOCELL�������״̬�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_3                 : 1; /* [0..0] ���� */
        unsigned int    ccpu_mtcmos_ctrl_stat      : 1; /* [1..1] ModemA9��Դ�����µ����״ָ̬ʾ��1 - �ϵ�ʹ�ܣ�0 - �µ�ʹ�ܡ� */
        unsigned int    reserved_2                 : 2; /* [3..2] ���� */
        unsigned int    bbe16_mtcmos_ctrl_stat     : 1; /* [4..4] BBEDSP��Դ�� ���µ����״ָ̬ʾ��1 - �ϵ�ʹ�ܣ�0 - �µ�ʹ�ܡ� */
        unsigned int    reserved_1                 : 1; /* [5..5] ���� */
        unsigned int    ltebbp_mtcmos_ctrl_stat    : 1; /* [6..6] LTEBBP��Դ�����µ����״ָ̬ʾ��1 - �ϵ�ʹ�ܣ�0 - �µ�ʹ�ܡ� */
        unsigned int    twbbp_mtcmos_ctrl_stat     : 1; /* [7..7] TWBBP��Դ�����µ����״ָ̬ʾ��1 - �ϵ�ʹ�ܣ�0 - �µ�ʹ�ܡ� */
        unsigned int    wbbp_mtcmos_ctrl_stat      : 1; /* [8..8] WBBP��Դ�����µ����״ָ̬ʾ��1 - �ϵ�ʹ�ܣ�0 - �µ�ʹ�ܡ� */
        unsigned int    g1bbp_mtcmos_ctrl_stat     : 1; /* [9..9] G1BBP��Դ�����µ����״ָ̬ʾ��1 - �ϵ�ʹ�ܣ�0 - �µ�ʹ�ܡ� */
        unsigned int    g2bbp_mtcmos_ctrl_stat     : 1; /* [10..10] G2BBP��Դ�����µ����״ָ̬ʾ��1 - �ϵ�ʹ�ܣ�0 - �µ�ʹ�ܡ� */
        unsigned int    irmbbp_mtcmos_ctrl_stat    : 1; /* [11..11] IRMBBP��Դ�����µ����״ָ̬ʾ��1 - �ϵ�ʹ�ܣ�0 - �µ�ʹ�ܡ� */
        unsigned int    reserved_0                 : 20; /* [31..12] ���� */
    } bits;
    unsigned int    u32;
}HI_PWR_STAT4_T;    /* MTCMOS���µ����״̬�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    ipf_sec_w_ctrl             : 1; /* [0..0] AP�����IPF��ȫ���Կ��ơ�0 - �ǰ�ȫ�ɷ��ʣ�1 - ֻ�ܰ�ȫ���ʡ� */
        unsigned int    ipf_sec_r_ctrl             : 1; /* [1..1] AP�����IPF��ȫ���Կ��ơ�0 - �ǰ�ȫ�ɷ��ʣ�1 - ֻ�ܰ�ȫ���ʡ� */
        unsigned int    region1_sec_w_ctrl         : 1; /* [2..2] AP�����Modem��ϵͳRegion1����ȫ���Կ��ơ�0 - �ǰ�ȫ�ɷ��ʣ�1 - ֻ�ܰ�ȫ���ʡ� */
        unsigned int    region1_sec_r_ctrl         : 1; /* [3..3] AP�����Modem��ϵͳRegion1д��ȫ���Կ��ƣ��ɷ��ʿռ����ȫ�ռ�Ĵ���2.3����0 - �ǰ�ȫ�ɷ��ʣ�1 - ֻ�ܰ�ȫ���ʡ� */
        unsigned int    ipcm_sec_w_ctrl            : 1; /* [4..4] AP�����IPCM��ȫ���Կ��ơ�0 - �ǰ�ȫ�ɷ��ʣ�1 - ֻ�ܰ�ȫ���ʡ� */
        unsigned int    ipcm_sec_r_ctrl            : 1; /* [5..5] AP�����IPCM��ȫ���Կ��ơ�0 - �ǰ�ȫ�ɷ��ʣ�1 - ֻ�ܰ�ȫ���ʡ� */
        unsigned int    aximon_sec_w_ctrl          : 1; /* [6..6] AP�����AXIMON��ȫ���Կ��ơ�0 - �ǰ�ȫ�ɷ��ʣ�1 - ֻ�ܰ�ȫ���ʡ� */
        unsigned int    aximon_sec_r_ctrl          : 1; /* [7..7] AP�����AXIMON��ȫ���Կ��ơ�0 - �ǰ�ȫ�ɷ��ʣ�1 - ֻ�ܰ�ȫ���ʡ� */
        unsigned int    others_sec_w_ctrl          : 1; /* [8..8] AP�����������ַ��ȫ���Կ��ơ�0 - �ǰ�ȫ�ɷ��ʣ�1 - ֻ�ܰ�ȫ���ʡ� */
        unsigned int    others_sec_r_ctrl          : 1; /* [9..9] AP�����������ַ��ȫ���Կ��ơ�0 - �ǰ�ȫ�ɷ��ʣ�1 - ֻ�ܰ�ȫ���ʡ� */
        unsigned int    region2_sec_r_ctrl         : 2; /* [11..10] ���� */
        unsigned int    uicc_sec_w_ctrl            : 1; /* [12..12] AP�����UICC����ȫ���Կ��ơ�0 - �ǰ�ȫ�ɷ��ʣ�1 - ֻ�ܰ�ȫ���ʡ� */
        unsigned int    uicc_sec_r_ctrl            : 1; /* [13..13] AP�����UICCд��ȫ���Կ���0 - �ǰ�ȫ�ɷ��ʣ�1 - ֻ�ܰ�ȫ���ʡ� */
        unsigned int    reserved                   : 18; /* [31..14] ���� */
    } bits;
    unsigned int    u32;
}HI_SEC_CTRL0_T;    /* ��ȫ���ƼĴ���0(slave�ӿڣ� */

typedef union
{
    struct
    {
        unsigned int    noddr_bypass               : 1; /* [0..0] ��DDR�ռ�ֱͨ���á�1 - bypass(default)0 - ��bypass */
        unsigned int    reserved                   : 15; /* [15..1] ���� */
        unsigned int    sci0_sec_w_ctrl            : 1; /* [16..16] modem��ϵͳ����SCI0��ȫ���Կ��ƣ�д��������0 - ����ǰ�ȫ������1 - ����ȫ���� */
        unsigned int    sci0_sec_r_ctrl            : 1; /* [17..17] modem��ϵͳ����SCI0��ȫ���Կ��ƣ�����������0 - ����ǰ�ȫ������1 - ����ȫ���� */
        unsigned int    sci1_sec_w_ctrl            : 1; /* [18..18] modem��ϵͳ����SCI1��ȫ���Կ��ƣ�д��������0 - ����ǰ�ȫ������1 - ����ȫ���� */
        unsigned int    sci1_sec_r_ctrl            : 1; /* [19..19] modem��ϵͳ����SCI1��ȫ���Կ��ƣ�����������0 - ����ǰ�ȫ������1 - ����ȫ���� */
        unsigned int    ipcm_s_sec_w_ctrl          : 1; /* [20..20] modem��ϵͳ����IPCM_S��ȫ���Կ��ƣ�д��������0 - ����ǰ�ȫ������1 - ����ȫ���� */
        unsigned int    ipcm_s_sec_r_ctrl          : 1; /* [21..21] modem��ϵͳ����IPCM_S��ȫ���Կ��ƣ�����������0 - ����ǰ�ȫ������1 - ����ȫ���� */
        unsigned int    ipcm_ns_sec_w_ctrl         : 1; /* [22..22] modem��ϵͳ����IPCM_NS��ȫ���Կ��ƣ�д��������0 - ����ǰ�ȫ������1 - ����ȫ���� */
        unsigned int    ipcm_ns_sec_r_ctrl         : 1; /* [23..23] modem��ϵͳ����IPCM_NS��ȫ���Կ��ƣ�����������0 - ����ǰ�ȫ������1 - ����ȫ���� */
        unsigned int    secram_sec_w_ctrl          : 1; /* [24..24] modem��ϵͳ����SECRAM��ȫ���Կ��ƣ�д��������0 - ����ǰ�ȫ������1 - ����ȫ���� */
        unsigned int    secram_sec_r_ctrl          : 1; /* [25..25] modem��ϵͳ����SECRAM��ȫ���Կ��ƣ�����������0 - ����ǰ�ȫ������1 - ����ȫ���� */
        unsigned int    socp_sec_w_ctrl            : 1; /* [26..26] modem��ϵͳ����SSOCP��ȫ���Կ��ƣ�д��������0 - ����ǰ�ȫ������1 - ����ȫ���� */
        unsigned int    socp_sec_r_ctrl            : 1; /* [27..27] modem��ϵͳ����SOCP��ȫ���Կ��ƣ�����������0 - ����ǰ�ȫ������1 - ����ȫ���� */
        unsigned int    others_sec_w_ctrl          : 1; /* [28..28] modem��ϵͳ����SSOCP��ȫ���Կ��ƣ�д��������0 - ����ǰ�ȫ������1 - ����ȫ���� */
        unsigned int    others_sec_r_ctrl          : 1; /* [29..29] modem��ϵͳ����������ַ�ռ䰲ȫ���Կ��ƣ�����������0 - ����ǰ�ȫ������1 - ����ȫ���� */
        unsigned int    ap_sec_ctrl                : 2; /* [31..30] L2C master�ӿڰ�ȫ���ơ�Bit[31] - bypass����0 - a*prot͸��(default)1 - a*prot��bit[30]���ơ�Bit[30] - ��bypassģʽ�£�a*prot[1]���ơ�1 - ����ȫ������0 - ����ǰ�ȫ������default�� */
    } bits;
    unsigned int    u32;
}HI_SEC_CTRL1_T;    /* ��ȫ���ƼĴ���1(GLB_MASTER�ӿڣ� */

typedef union
{
    struct
    {
        unsigned int    region1_filter_st_addr     : 32; /* [31..0] AP�����Modem��ϵͳregion1����ʼ��ַ */
    } bits;
    unsigned int    u32;
}HI_SEC_CTRL2_T;    /* ��ȫ���ƼĴ���2 */

typedef union
{
    struct
    {
        unsigned int    region1_filter_end_addr    : 32; /* [31..0] AP�����Modem��ϵͳregion1�Ľ�����ַ */
    } bits;
    unsigned int    u32;
}HI_SEC_CTRL3_T;    /* ��ȫ���ƼĴ���3 */

typedef union
{
    struct
    {
        unsigned int    secram_filter_st_addr      : 32; /* [31..0] SECRA���ʵ���ʼ��ַ��Modem���ʸÿռ�İ�ȫ���Կɶ������ã��μ���ȫ���ƼĴ���1 */
    } bits;
    unsigned int    u32;
}HI_SEC_CTRL4_T;    /* ��ȫ���ƼĴ���4 */

typedef union
{
    struct
    {
        unsigned int    secram_filter_end_addr     : 32; /* [31..0] SECRAM���ʵĽ�����ַ */
    } bits;
    unsigned int    u32;
}HI_SEC_CTRL5_T;    /* ��ȫ���ƼĴ���5 */


/********************************************************************************/
/*    syssc ��������Ŀ��_ģ����_�Ĵ�����_��Ա��_set)        */
/********************************************************************************/
HI_SET_GET(hi_crg_clken3_reserved_1,reserved_1,HI_CRG_CLKEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_bbe_refclk_en,bbe_refclk_en,HI_CRG_CLKEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_a9_refclk_en,a9_refclk_en,HI_CRG_CLKEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_hpm_clk_en,hpm_clk_en,HI_CRG_CLKEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clken3_reserved_0,reserved_0,HI_CRG_CLKEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_reserved_1,reserved_1,HI_CRG_CLKDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_bbe_refclk_dis,bbe_refclk_dis,HI_CRG_CLKDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_a9_refclk_dis,a9_refclk_dis,HI_CRG_CLKDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_hpm_clk_dis,hpm_clk_dis,HI_CRG_CLKDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkdis3_reserved_0,reserved_0,HI_CRG_CLKDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_reserved_1,reserved_1,HI_CRG_CLKSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_bbe_refclk_status,bbe_refclk_status,HI_CRG_CLKSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_a9_refclk_status,a9_refclk_status,HI_CRG_CLKSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_hpm_clk_status,hpm_clk_status,HI_CRG_CLKSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkstat3_reserved_0,reserved_0,HI_CRG_CLKSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT3_OFFSET)
HI_SET_GET(hi_crg_clken4_bbe_pd_clk_en,bbe_pd_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_bbe_core_clk_en,bbe_core_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_reserved_2,reserved_2,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_mcpu_pd_clk_en,mcpu_pd_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_l2c_clk_en,l2c_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_reserved_1,reserved_1,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_mcpu_mbist_clk_en,mcpu_mbist_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_reserved_0,reserved_0,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_cicom0_clk_en,cicom0_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_cicom1_clk_en,cicom1_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_ipf_clk_en,ipf_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_upacc_clk_en,upacc_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_sci0_clk_en,sci0_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_sci1_clk_en,sci1_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_uicc_clk_en,uicc_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_uart_clk_en,uart_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_bbe_vic_clk_en,bbe_vic_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_cipher_clk_en,cipher_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_edmac_clk_en,edmac_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_ipcm_clk_en,ipcm_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_hsuart_clk_en,hsuart_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_aximem_clk_en,aximem_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_aximon_clk_en,aximon_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_timer_clk_en,timer_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clken4_wdog_clk_en,wdog_clk_en,HI_CRG_CLKEN4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_dsp0_pd_clk_dis,dsp0_pd_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_dsp0_core_clk_dis,dsp0_core_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_dsp0_dbg_clk_dis,dsp0_dbg_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_mcpu_pd_clk_dis,mcpu_pd_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_l2c_clk_dis,l2c_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_mcpu_dbg_clk_dis,mcpu_dbg_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_mcpu_mbist_clk_dis,mcpu_mbist_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_reserved,reserved,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_cicom0_clk_dis,cicom0_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_cicom1_clk_dis,cicom1_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_ipf_clk_dis,ipf_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_upacc_clk_dis,upacc_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_sci0_clk_dis,sci0_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_sci1_clk_dis,sci1_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_uicc_clk_dis,uicc_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_uart_clk_dis,uart_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_bbe_vic_clk_dis,bbe_vic_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_cipher_clk_dis,cipher_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_edmac_clk_dis,edmac_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_ipcm_clk_dis,ipcm_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_hsuart_clk_dis,hsuart_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_aximem_clk_dis,aximem_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_aximon_clk_dis,aximon_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_timer_clk_dis,timer_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkdis4_wdog_clk_dis,wdog_clk_dis,HI_CRG_CLKDIS4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_bbe_pd_clk_status,bbe_pd_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_bbe_core_clk_status,bbe_core_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_bbe_dbg_clk_status,bbe_dbg_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_mcpu_pd_clk_status,mcpu_pd_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_l2c_clk_status,l2c_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_mcpu_dbg_clk_status,mcpu_dbg_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_mcpu_mbist_clk_status,mcpu_mbist_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_reserved,reserved,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_cicom0_clk_status,cicom0_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_cicom1_clk_status,cicom1_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_ipf_clk_status,ipf_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_upacc_clk_status,upacc_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_sci0_clk_status,sci0_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_sci1_clk_status,sci1_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_uicc_clk_status,uicc_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_uart_clk_status,uart_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_bbe_vic_clk_status,bbe_vic_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_cipher_clk_status,cipher_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_edmac_clk_status,edmac_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_ipcm_clk_status,ipcm_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_hsuart_clk_status,hsuart_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_aximem_clk_status,aximem_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_aximon_clk_status,aximon_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_timer_clk_status,timer_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clkstat4_wdog_clk_status,wdog_clk_status,HI_CRG_CLKSTAT4_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT4_OFFSET)
HI_SET_GET(hi_crg_clken5_bbp_ao_clk_en,bbp_ao_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_bbpcommon_2a_clk_en,bbpcommon_2a_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_ltebbp_pd_clk_en,ltebbp_pd_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_g1bbp_pd_clk_en,g1bbp_pd_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_g2bbp_pd_clk_en,g2bbp_pd_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_twbbp_pd_clk_en,twbbp_pd_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_wbbp_pd_clk_en,wbbp_pd_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_irm_pd_clk_en,irm_pd_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_lbbp_axi_clk_en,lbbp_axi_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_lbbp_pdt_clk_en,lbbp_pdt_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_lbbp_pdf_clk_en,lbbp_pdf_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_lbbp_tdl_clk_en,lbbp_tdl_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_lbbp_vdl_clk_en,lbbp_vdl_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_lbbp_tds_clk_en,lbbp_tds_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_reserved_1,reserved_1,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_irm_turbo_clk_en,irm_turbo_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_irm_bbc_245m_clk_en,irm_bbc_245m_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_irm_wbbp_122m_clk_en,irm_wbbp_122m_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_twbbp_wbbp_122m_clk_en,twbbp_wbbp_122m_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_twbbp_wbbp_61m_clk_en,twbbp_wbbp_61m_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_twbbp_tbbp_122m_clk_en,twbbp_tbbp_122m_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_twbbp_tbbp_245m_clk_en,twbbp_tbbp_245m_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_wbbp_61m_clk_en,wbbp_61m_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_wbbp_122m_clk_en,wbbp_122m_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_wbbp_245m_clk_en,wbbp_245m_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_wbbp_axi_clk_en,wbbp_axi_clk_en,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clken5_reserved_0,reserved_0,HI_CRG_CLKEN5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKEN5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_bbp_ao_clk_dis,bbp_ao_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_bbpcommon_2a_clk_dis,bbpcommon_2a_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_lbbp_pd_clk_dis,lbbp_pd_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_g1bbp_pd_clk_dis,g1bbp_pd_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_g2bbp_pd_clk_dis,g2bbp_pd_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_twbbp_pd_clk_dis,twbbp_pd_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_wbbp_pd_clk_dis,wbbp_pd_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_irmbbp_pd_clk_dis,irmbbp_pd_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_lbbp_axi_clk_dis,lbbp_axi_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_lbbp_pdt_clk_dis,lbbp_pdt_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_lbbp_pdf_clk_dis,lbbp_pdf_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_lbbp_tdl_clk_dis,lbbp_tdl_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_lbbp_vdl_clk_dis,lbbp_vdl_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_lbbp_tds_clk_dis,lbbp_tds_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_reserved_1,reserved_1,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_irm_turbo_clk_dis,irm_turbo_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_irm_bbc_245m_clk_dis,irm_bbc_245m_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_irm_wbbp_clk_dis,irm_wbbp_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_twbbp_wbbp_122m_clk_dis,twbbp_wbbp_122m_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_twbbp_wbbp_61m_clk_dis,twbbp_wbbp_61m_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_twbbp_tbbp_122m_clk_dis,twbbp_tbbp_122m_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_twbbp_tbbp_245m_clk_dis,twbbp_tbbp_245m_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_wbbp_61m_clk_dis,wbbp_61m_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_wbbp_122m_clk_dis,wbbp_122m_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_wbbp_245m_clk_dis,wbbp_245m_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_wbbp_axi_clk_dis,wbbp_axi_clk_dis,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkdis5_reserved_0,reserved_0,HI_CRG_CLKDIS5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIS5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_bbp_ao_clk_stat,bbp_ao_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_bbpcom_clk_stat,bbpcom_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_lbbp_pd_clk_stat,lbbp_pd_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_g1bbp_pd_clk_stat,g1bbp_pd_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_g2bbp_pd_clk_stat,g2bbp_pd_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_twbbp_pd_clk_stat,twbbp_pd_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_wbbp_pd_clk_stat,wbbp_pd_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_irmbbp_pd_clk_stat,irmbbp_pd_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_lbbp_axi_clk_stat,lbbp_axi_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_lbbp_pdt_clk_stat,lbbp_pdt_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_lbbp_pdf_clk_stat,lbbp_pdf_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_lbbp_tdl_clk_stat,lbbp_tdl_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_lbbp_vdl_clk_stat,lbbp_vdl_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_lbbp_tds_clk_stat,lbbp_tds_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_reserved_1,reserved_1,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_irm_turbo_clk_stat,irm_turbo_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_irm_bbc_245m_clk_stat,irm_bbc_245m_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_irm_wbbp_clk_stat,irm_wbbp_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_twbbp_wbbp_122m_clk_stat,twbbp_wbbp_122m_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_twbbp_wbbp_61m_clk_stat,twbbp_wbbp_61m_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_twbbp_tbbp_122m_clk_stat,twbbp_tbbp_122m_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_twbbp_tbbp_245m_clk_stat,twbbp_tbbp_245m_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_wbbp_61m_clk_stat,wbbp_61m_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_wbbp_122m_clk_stat,wbbp_122m_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_wbbp_245m_clk_stat,wbbp_245m_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_wbbp_axi_clk_stat,wbbp_axi_clk_stat,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_clkstat5_reserved_0,reserved_0,HI_CRG_CLKSTAT5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKSTAT5_OFFSET)
HI_SET_GET(hi_crg_srsten1_bbe_pd_srst_en,bbe_pd_srst_en,HI_CRG_SRSTEN1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_bbe_core_srst_en,bbe_core_srst_en,HI_CRG_SRSTEN1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_bbe_dbg_srst_en,bbe_dbg_srst_en,HI_CRG_SRSTEN1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_reserved_4,reserved_4,HI_CRG_SRSTEN1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_moda9_cpu_srst_en,moda9_cpu_srst_en,HI_CRG_SRSTEN1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_moda9_dbg_srst_en,moda9_dbg_srst_en,HI_CRG_SRSTEN1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_moda9_peri_srst_en,moda9_peri_srst_en,HI_CRG_SRSTEN1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_moda9_scu_srst_en,moda9_scu_srst_en,HI_CRG_SRSTEN1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_moda9_ptm_srst_en,moda9_ptm_srst_en,HI_CRG_SRSTEN1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_moda9_wd_srst_en,moda9_wd_srst_en,HI_CRG_SRSTEN1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_reserved_3,reserved_3,HI_CRG_SRSTEN1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_mdma9_pd_srst_en,mdma9_pd_srst_en,HI_CRG_SRSTEN1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_reserved_2,reserved_2,HI_CRG_SRSTEN1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_uicc_srst_en,uicc_srst_en,HI_CRG_SRSTEN1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_reserved_1,reserved_1,HI_CRG_SRSTEN1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_amon_soc_srst_en,amon_soc_srst_en,HI_CRG_SRSTEN1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_reserved_0,reserved_0,HI_CRG_SRSTEN1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srsten1_hpm_srst_en,hpm_srst_en,HI_CRG_SRSTEN1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_bbe_pd_srst_dis,bbe_pd_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_bbe_core_srst_dis,bbe_core_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_bbe_dbg_srst_dis,bbe_dbg_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_reserved_4,reserved_4,HI_CRG_SRSTDIS1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_moda9_core_srst_dis,moda9_core_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_moda9_dbg_srst_dis,moda9_dbg_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_moda9_peri_srst_dis,moda9_peri_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_moda9_scu_srst_dis,moda9_scu_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_moda9_ptm_srst_dis,moda9_ptm_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_moda9_wd_srst_dis,moda9_wd_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_reserved_3,reserved_3,HI_CRG_SRSTDIS1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_mdma9_pd_srst_dis,mdma9_pd_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_reserved_2,reserved_2,HI_CRG_SRSTDIS1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_uicc_srst_dis,uicc_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_reserved_1,reserved_1,HI_CRG_SRSTDIS1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_amon_soc_srst_dis,amon_soc_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_reserved_0,reserved_0,HI_CRG_SRSTDIS1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srstdis1_hpm_srst_dis,hpm_srst_dis,HI_CRG_SRSTDIS1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS1_OFFSET)
HI_SET_GET(hi_crg_srststat1_bbe_pd_srst_status,bbe_pd_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_bbe_core_srst_status,bbe_core_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_bbe_dbg_srst_status,bbe_dbg_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_reserved_4,reserved_4,HI_CRG_SRSTSTAT1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_moda9_core_srst_status,moda9_core_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_moda9_dbg_srst_status,moda9_dbg_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_moda9_peri_srst_status,moda9_peri_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_moda9_scu_srst_status,moda9_scu_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_moda9_ptm_srst_status,moda9_ptm_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_moda9_wd_srst_status,moda9_wd_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_reserved_3,reserved_3,HI_CRG_SRSTSTAT1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_mdma9_pd_srst_status,mdma9_pd_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_reserved_2,reserved_2,HI_CRG_SRSTSTAT1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_uicc_srst_status,uicc_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_reserved_1,reserved_1,HI_CRG_SRSTSTAT1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_amon_soc_srst_status,amon_soc_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_reserved_0,reserved_0,HI_CRG_SRSTSTAT1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srststat1_hpm_srst_status,hpm_srst_status,HI_CRG_SRSTSTAT1_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT1_OFFSET)
HI_SET_GET(hi_crg_srsten3_bbp_crg_srst_en,bbp_crg_srst_en,HI_CRG_SRSTEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_bbpcommon_2a_srst_en,bbpcommon_2a_srst_en,HI_CRG_SRSTEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_lbbp_pd_srst_en,lbbp_pd_srst_en,HI_CRG_SRSTEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_g1bbp_pd_srst_en,g1bbp_pd_srst_en,HI_CRG_SRSTEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_g2bbp_pd_srst_en,g2bbp_pd_srst_en,HI_CRG_SRSTEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_twbbp_pd_srst_en,twbbp_pd_srst_en,HI_CRG_SRSTEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_wbbp_pd_srst_en,wbbp_pd_srst_en,HI_CRG_SRSTEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_irm_pd_srst_en,irm_pd_srst_en,HI_CRG_SRSTEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_bbc_lbbp_pdt_srst_en,bbc_lbbp_pdt_srst_en,HI_CRG_SRSTEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_bbc_lbbp_pdf_srst_en,bbc_lbbp_pdf_srst_en,HI_CRG_SRSTEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_bbc_lbbp_tdl_srst_en,bbc_lbbp_tdl_srst_en,HI_CRG_SRSTEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_bbc_tbbp_245m_srst_en,bbc_tbbp_245m_srst_en,HI_CRG_SRSTEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_bbc_twbbp_122m_srst_en,bbc_twbbp_122m_srst_en,HI_CRG_SRSTEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_bbc_g1bbp_104m_srst_en,bbc_g1bbp_104m_srst_en,HI_CRG_SRSTEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_bbc_g2bbp_104m_srst_en,bbc_g2bbp_104m_srst_en,HI_CRG_SRSTEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_reserved,reserved,HI_CRG_SRSTEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srsten3_abb_srst_en,abb_srst_en,HI_CRG_SRSTEN3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTEN3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_bbp_crg_srst_dis,bbp_crg_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_bbpcommon_2a_pd_srst_dis,bbpcommon_2a_pd_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_lbbp_pd_srst_dis,lbbp_pd_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_g1bbp_pd_srst_dis,g1bbp_pd_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_g2bbp_pd_srst_dis,g2bbp_pd_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_twbbp_pd_srst_dis,twbbp_pd_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_wbbp_pd_srst_dis,wbbp_pd_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_irm_pd_srst_dis,irm_pd_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_bbc_lbbp_pdt_srst_dis,bbc_lbbp_pdt_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_bbc_lbbp_pdf_srst_dis,bbc_lbbp_pdf_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_bbc_lbbp_tdl_srst_dis,bbc_lbbp_tdl_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_bbc_tbbp_245m_srst_dis,bbc_tbbp_245m_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_bbc_twbbp_122m_srst_dis,bbc_twbbp_122m_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_bbc_g1bbp_104m_srst_dis,bbc_g1bbp_104m_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_bbc_g2bbp_104m_srst_dis,bbc_g2bbp_104m_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_reserved,reserved,HI_CRG_SRSTDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srstdis3_abb_srst_dis,abb_srst_dis,HI_CRG_SRSTDIS3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTDIS3_OFFSET)
HI_SET_GET(hi_crg_srststat3_bbp_crg_srst_stat,bbp_crg_srst_stat,HI_CRG_SRSTSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_bbpcommon_2a_srst_stat,bbpcommon_2a_srst_stat,HI_CRG_SRSTSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_lbbp_pd_srst_stat,lbbp_pd_srst_stat,HI_CRG_SRSTSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_g1bbp_pd_srst_stat,g1bbp_pd_srst_stat,HI_CRG_SRSTSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_g2bbp_pd_srst_stat,g2bbp_pd_srst_stat,HI_CRG_SRSTSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_twbbp_pd_srst_stat,twbbp_pd_srst_stat,HI_CRG_SRSTSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_wbbp_pd_srst_stat,wbbp_pd_srst_stat,HI_CRG_SRSTSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_irm_pd_srst_stat,irm_pd_srst_stat,HI_CRG_SRSTSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_bbc_lbbp_pdt_srstctrl_stat,bbc_lbbp_pdt_srstctrl_stat,HI_CRG_SRSTSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_bbc_lbbp_pdf_srstctrl_stat,bbc_lbbp_pdf_srstctrl_stat,HI_CRG_SRSTSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_bbc_lbbp_tdl_srstctrl_stat,bbc_lbbp_tdl_srstctrl_stat,HI_CRG_SRSTSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_bbc_tbbp_245m_srstctrl_stat,bbc_tbbp_245m_srstctrl_stat,HI_CRG_SRSTSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_bbc_twbbp_122m_srstctrl_stat,bbc_twbbp_122m_srstctrl_stat,HI_CRG_SRSTSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_bbc_g1bbp_104m_srstctrl_stat,bbc_g1bbp_104m_srstctrl_stat,HI_CRG_SRSTSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_bbc_g2bbp_104m_srstctrl_stat,bbc_g2bbp_104m_srstctrl_stat,HI_CRG_SRSTSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_reserved,reserved,HI_CRG_SRSTSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_srststat3_abb_srst_status,abb_srst_status,HI_CRG_SRSTSTAT3_T,HI_SYSSC_BASE_ADDR, HI_CRG_SRSTSTAT3_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_reserved_4,reserved_4,HI_CRG_CLKDIV2_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_mdma9_clk_div,mdma9_clk_div,HI_CRG_CLKDIV2_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_a92slow_freqmode,a92slow_freqmode,HI_CRG_CLKDIV2_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_reserved_3,reserved_3,HI_CRG_CLKDIV2_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_a92fast_freqmode,a92fast_freqmode,HI_CRG_CLKDIV2_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_reserved_2,reserved_2,HI_CRG_CLKDIV2_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_a9_clksw_req,a9_clksw_req,HI_CRG_CLKDIV2_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_reserved_1,reserved_1,HI_CRG_CLKDIV2_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_apb_freqmode,apb_freqmode,HI_CRG_CLKDIV2_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv2_reserved_0,reserved_0,HI_CRG_CLKDIV2_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIV2_OFFSET)
HI_SET_GET(hi_crg_clkdiv5_bbe_clk_div_num,bbe_clk_div_num,HI_CRG_CLKDIV5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIV5_OFFSET)
HI_SET_GET(hi_crg_clkdiv5_reserved_2,reserved_2,HI_CRG_CLKDIV5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIV5_OFFSET)
HI_SET_GET(hi_crg_clkdiv5_bbe_freqmode,bbe_freqmode,HI_CRG_CLKDIV5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIV5_OFFSET)
HI_SET_GET(hi_crg_clkdiv5_reserved_1,reserved_1,HI_CRG_CLKDIV5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIV5_OFFSET)
HI_SET_GET(hi_crg_clkdiv5_cipher_clk_div,cipher_clk_div,HI_CRG_CLKDIV5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIV5_OFFSET)
HI_SET_GET(hi_crg_clkdiv5_reserved_0,reserved_0,HI_CRG_CLKDIV5_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLKDIV5_OFFSET)
HI_SET_GET(hi_crg_clk_sel2_timer_clk_sel,timer_clk_sel,HI_CRG_CLK_SEL2_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLK_SEL2_OFFSET)
HI_SET_GET(hi_crg_clk_sel2_gps_tcxo_clk_sel,gps_tcxo_clk_sel,HI_CRG_CLK_SEL2_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLK_SEL2_OFFSET)
HI_SET_GET(hi_crg_clk_sel2_hpm_refclk_sel,hpm_refclk_sel,HI_CRG_CLK_SEL2_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLK_SEL2_OFFSET)
HI_SET_GET(hi_crg_clk_sel2_reserved_1,reserved_1,HI_CRG_CLK_SEL2_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLK_SEL2_OFFSET)
HI_SET_GET(hi_crg_clk_sel2_sc_a9wdt_rst_en,sc_a9wdt_rst_en,HI_CRG_CLK_SEL2_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLK_SEL2_OFFSET)
HI_SET_GET(hi_crg_clk_sel2_reserved_0,reserved_0,HI_CRG_CLK_SEL2_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLK_SEL2_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_abb_tcxo_clk_sel,abb_tcxo_clk_sel,HI_CRG_CLK_SEL3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_bbppll_refclk_sel,bbppll_refclk_sel,HI_CRG_CLK_SEL3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_bbp_tcxo_clk_sel,bbp_tcxo_clk_sel,HI_CRG_CLK_SEL3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_ch0_tcxo_clk_sel,ch0_tcxo_clk_sel,HI_CRG_CLK_SEL3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_ch1_tcxo_clk_sel,ch1_tcxo_clk_sel,HI_CRG_CLK_SEL3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_g1bbp_104m_clk_sel,g1bbp_104m_clk_sel,HI_CRG_CLK_SEL3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_g2bbp_104m_clk_sel,g2bbp_104m_clk_sel,HI_CRG_CLK_SEL3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_tdl_clk_sel,tdl_clk_sel,HI_CRG_CLK_SEL3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_lbbp_vdl_clk_sel,lbbp_vdl_clk_sel,HI_CRG_CLK_SEL3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_lbbp_pdf_clk_sel,lbbp_pdf_clk_sel,HI_CRG_CLK_SEL3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_clk_sel3_reserved,reserved,HI_CRG_CLK_SEL3_T,HI_SYSSC_BASE_ADDR, HI_CRG_CLK_SEL3_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg0_pll_en,pll_en,HI_CRG_A9PLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg0_pll_bp,pll_bp,HI_CRG_A9PLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg0_pll_refdiv,pll_refdiv,HI_CRG_A9PLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg0_pll_intprog,pll_intprog,HI_CRG_A9PLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg0_pll_postdiv1,pll_postdiv1,HI_CRG_A9PLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg0_pll_postdiv2,pll_postdiv2,HI_CRG_A9PLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg0_pll_lock,pll_lock,HI_CRG_A9PLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg0_reserved,reserved,HI_CRG_A9PLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg1_pll_fracdiv,pll_fracdiv,HI_CRG_A9PLL_CFG1_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG1_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg1_pll_int_mod,pll_int_mod,HI_CRG_A9PLL_CFG1_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG1_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg1_pll_cfg_vld,pll_cfg_vld,HI_CRG_A9PLL_CFG1_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG1_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg1_pll_clk_gt,pll_clk_gt,HI_CRG_A9PLL_CFG1_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG1_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg1_reserved,reserved,HI_CRG_A9PLL_CFG1_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG1_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg2_pll_ssc_reset,pll_ssc_reset,HI_CRG_A9PLL_CFG2_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG2_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg2_pll_ssc_disable,pll_ssc_disable,HI_CRG_A9PLL_CFG2_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG2_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg2_pll_ssc_downspread,pll_ssc_downspread,HI_CRG_A9PLL_CFG2_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG2_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg2_pll_ssc_spread,pll_ssc_spread,HI_CRG_A9PLL_CFG2_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG2_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg2_pll_ssc_divval,pll_ssc_divval,HI_CRG_A9PLL_CFG2_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG2_OFFSET)
HI_SET_GET(hi_crg_a9pll_cfg2_reserved,reserved,HI_CRG_A9PLL_CFG2_T,HI_SYSSC_BASE_ADDR, HI_CRG_A9PLL_CFG2_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg0_pll_en,pll_en,HI_CRG_DSPPLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg0_pll_bp,pll_bp,HI_CRG_DSPPLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg0_pll_refdiv,pll_refdiv,HI_CRG_DSPPLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg0_pll_intprog,pll_intprog,HI_CRG_DSPPLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg0_pll_postdiv1,pll_postdiv1,HI_CRG_DSPPLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg0_pll_postdiv2,pll_postdiv2,HI_CRG_DSPPLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg0_pll_lock,pll_lock,HI_CRG_DSPPLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg0_reserved,reserved,HI_CRG_DSPPLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg1_pll_fracdiv,pll_fracdiv,HI_CRG_DSPPLL_CFG1_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG1_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg1_pll_int_mod,pll_int_mod,HI_CRG_DSPPLL_CFG1_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG1_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg1_pll_cfg_vld,pll_cfg_vld,HI_CRG_DSPPLL_CFG1_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG1_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg1_pll_clk_gt,pll_clk_gt,HI_CRG_DSPPLL_CFG1_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG1_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg1_reserved,reserved,HI_CRG_DSPPLL_CFG1_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG1_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg2_pll_ssc_reset,pll_ssc_reset,HI_CRG_DSPPLL_CFG2_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG2_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg2_pll_ssc_disable,pll_ssc_disable,HI_CRG_DSPPLL_CFG2_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG2_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg2_pll_ssc_downspread,pll_ssc_downspread,HI_CRG_DSPPLL_CFG2_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG2_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg2_pll_ssc_spread,pll_ssc_spread,HI_CRG_DSPPLL_CFG2_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG2_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg2_pll_ssc_divval,pll_ssc_divval,HI_CRG_DSPPLL_CFG2_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG2_OFFSET)
HI_SET_GET(hi_crg_dsppll_cfg2_reserved,reserved,HI_CRG_DSPPLL_CFG2_T,HI_SYSSC_BASE_ADDR, HI_CRG_DSPPLL_CFG2_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg0_pll_en,pll_en,HI_CRG_BBPPLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg0_pll_bp,pll_bp,HI_CRG_BBPPLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg0_pll_refdiv,pll_refdiv,HI_CRG_BBPPLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg0_pll_intprog,pll_intprog,HI_CRG_BBPPLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg0_pll_postdiv1,pll_postdiv1,HI_CRG_BBPPLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg0_pll_postdiv2,pll_postdiv2,HI_CRG_BBPPLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg0_pll_lock,pll_lock,HI_CRG_BBPPLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg0_reserved,reserved,HI_CRG_BBPPLL_CFG0_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG0_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg1_pll_fracdiv,pll_fracdiv,HI_CRG_BBPPLL_CFG1_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG1_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg1_pll_int_mod,pll_int_mod,HI_CRG_BBPPLL_CFG1_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG1_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg1_pll_cfg_vld,pll_cfg_vld,HI_CRG_BBPPLL_CFG1_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG1_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg1_pll_clk_gt,pll_clk_gt,HI_CRG_BBPPLL_CFG1_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG1_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg1_reserved,reserved,HI_CRG_BBPPLL_CFG1_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG1_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg2_pll_ssc_reset,pll_ssc_reset,HI_CRG_BBPPLL_CFG2_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG2_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg2_pll_ssc_disable,pll_ssc_disable,HI_CRG_BBPPLL_CFG2_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG2_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg2_pll_ssc_downspread,pll_ssc_downspread,HI_CRG_BBPPLL_CFG2_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG2_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg2_pll_ssc_spread,pll_ssc_spread,HI_CRG_BBPPLL_CFG2_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG2_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg2_pll_ssc_divval,pll_ssc_divval,HI_CRG_BBPPLL_CFG2_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG2_OFFSET)
HI_SET_GET(hi_crg_bbppll_cfg2_reserved,reserved,HI_CRG_BBPPLL_CFG2_T,HI_SYSSC_BASE_ADDR, HI_CRG_BBPPLL_CFG2_OFFSET)
HI_SET_GET(hi_sc_ctrl0_mcpu_boot_remap_clear,mcpu_boot_remap_clear,HI_SC_CTRL0_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL0_OFFSET)
HI_SET_GET(hi_sc_ctrl0_reserved,reserved,HI_SC_CTRL0_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL0_OFFSET)
HI_SET_GET(hi_sc_ctrl2_wdt_clk_en,wdt_clk_en,HI_SC_CTRL2_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL2_OFFSET)
HI_SET_GET(hi_sc_ctrl2_reserved_1,reserved_1,HI_SC_CTRL2_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL2_OFFSET)
HI_SET_GET(hi_sc_ctrl2_wdt_en_ov,wdt_en_ov,HI_SC_CTRL2_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL2_OFFSET)
HI_SET_GET(hi_sc_ctrl2_reserved_0,reserved_0,HI_SC_CTRL2_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL2_OFFSET)
HI_SET_GET(hi_sc_ctrl2_wdt_en_ctrl,wdt_en_ctrl,HI_SC_CTRL2_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL2_OFFSET)
HI_SET_GET(hi_sc_ctrl3_reserved_3,reserved_3,HI_SC_CTRL3_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_amon_monitor_start,amon_monitor_start,HI_SC_CTRL3_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_reserved_2,reserved_2,HI_SC_CTRL3_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_dsp0_uart_en,dsp0_uart_en,HI_SC_CTRL3_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_reserved_1,reserved_1,HI_SC_CTRL3_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl3_reserved_0,reserved_0,HI_SC_CTRL3_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL3_OFFSET)
HI_SET_GET(hi_sc_ctrl4_cicom0_sel_mod,cicom0_sel_mod,HI_SC_CTRL4_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL4_OFFSET)
HI_SET_GET(hi_sc_ctrl4_cicom1_sel_mod,cicom1_sel_mod,HI_SC_CTRL4_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL4_OFFSET)
HI_SET_GET(hi_sc_ctrl4_reserved,reserved,HI_SC_CTRL4_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL4_OFFSET)
HI_SET_GET(hi_sc_ctrl5_reserved,reserved,HI_SC_CTRL5_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL5_OFFSET)
HI_SET_GET(hi_sc_ctrl6_reserved,reserved,HI_SC_CTRL6_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL6_OFFSET)
HI_SET_GET(hi_sc_ctrl9_reserved,reserved,HI_SC_CTRL9_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL9_OFFSET)
HI_SET_GET(hi_sc_ctrl9_sw_dsp0_boot_addr,sw_dsp0_boot_addr,HI_SC_CTRL9_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL9_OFFSET)
HI_SET_GET(hi_sc_ctrl10_reserved,reserved,HI_SC_CTRL10_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL10_OFFSET)
HI_SET_GET(hi_sc_ctrl11_reserved_2,reserved_2,HI_SC_CTRL11_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_moda9_cfgnmfi,moda9_cfgnmfi,HI_SC_CTRL11_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_moda9_cfgsdisable,moda9_cfgsdisable,HI_SC_CTRL11_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_moda9_cp15sdisable,moda9_cp15sdisable,HI_SC_CTRL11_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_moda9_pwrctli0,moda9_pwrctli0,HI_SC_CTRL11_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_reserved_1,reserved_1,HI_SC_CTRL11_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_moda9_l2_waysize,moda9_l2_waysize,HI_SC_CTRL11_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_reserved_0,reserved_0,HI_SC_CTRL11_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl11_moda9_l2_regfilebase,moda9_l2_regfilebase,HI_SC_CTRL11_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL11_OFFSET)
HI_SET_GET(hi_sc_ctrl12_reserved,reserved,HI_SC_CTRL12_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL12_OFFSET)
HI_SET_GET(hi_sc_ctrl13_reserved_1,reserved_1,HI_SC_CTRL13_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL13_OFFSET)
HI_SET_GET(hi_sc_ctrl13_ocdhaltonreset,ocdhaltonreset,HI_SC_CTRL13_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL13_OFFSET)
HI_SET_GET(hi_sc_ctrl13_runstall,runstall,HI_SC_CTRL13_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL13_OFFSET)
HI_SET_GET(hi_sc_ctrl13_statvectorsel,statvectorsel,HI_SC_CTRL13_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL13_OFFSET)
HI_SET_GET(hi_sc_ctrl13_breaksync_en,breaksync_en,HI_SC_CTRL13_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL13_OFFSET)
HI_SET_GET(hi_sc_ctrl13_crosstrig_en,crosstrig_en,HI_SC_CTRL13_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL13_OFFSET)
HI_SET_GET(hi_sc_ctrl13_reserved_0,reserved_0,HI_SC_CTRL13_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL13_OFFSET)
HI_SET_GET(hi_sc_ctrl17_hpm_clk_div,hpm_clk_div,HI_SC_CTRL17_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL17_OFFSET)
HI_SET_GET(hi_sc_ctrl17_reserved_1,reserved_1,HI_SC_CTRL17_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL17_OFFSET)
HI_SET_GET(hi_sc_ctrl17_hpm_en,hpm_en,HI_SC_CTRL17_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL17_OFFSET)
HI_SET_GET(hi_sc_ctrl17_hpmx_en,hpmx_en,HI_SC_CTRL17_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL17_OFFSET)
HI_SET_GET(hi_sc_ctrl17_reserved_0,reserved_0,HI_SC_CTRL17_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL17_OFFSET)
HI_SET_GET(hi_sc_ctrl20_axi_mem_gatedclock_en,axi_mem_gatedclock_en,HI_SC_CTRL20_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_cicom0_auto_clk_gate_en,cicom0_auto_clk_gate_en,HI_SC_CTRL20_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_cicom0_soft_gate_clk_en,cicom0_soft_gate_clk_en,HI_SC_CTRL20_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_cicom1_auto_clk_gate_en,cicom1_auto_clk_gate_en,HI_SC_CTRL20_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_cicom1_soft_gate_clk_en,cicom1_soft_gate_clk_en,HI_SC_CTRL20_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_hs_uart_gatedclock_en,hs_uart_gatedclock_en,HI_SC_CTRL20_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_uart_gatedclock_en,uart_gatedclock_en,HI_SC_CTRL20_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_reserved_2,reserved_2,HI_SC_CTRL20_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_uicc_gatedclock_en,uicc_gatedclock_en,HI_SC_CTRL20_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_uicc_ss_scaledown_mode,uicc_ss_scaledown_mode,HI_SC_CTRL20_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_upacc_auto_clk_gate_en,upacc_auto_clk_gate_en,HI_SC_CTRL20_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_upacc_soft_gate_clk_en,upacc_soft_gate_clk_en,HI_SC_CTRL20_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_bbe16_cg_en,bbe16_cg_en,HI_SC_CTRL20_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_reserved_1,reserved_1,HI_SC_CTRL20_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_ipcm_auto_clk_gate_en,ipcm_auto_clk_gate_en,HI_SC_CTRL20_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_ipcm_soft_gate_clk_en,ipcm_soft_gate_clk_en,HI_SC_CTRL20_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_reserved_0,reserved_0,HI_SC_CTRL20_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl20_sysreg_auto_gate_en,sysreg_auto_gate_en,HI_SC_CTRL20_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL20_OFFSET)
HI_SET_GET(hi_sc_ctrl21_reserved_6,reserved_6,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_timer_gatedclock_en,timer_gatedclock_en,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_reserved_5,reserved_5,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_axi_glb_cg_en,dw_axi_glb_cg_en,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_axi_mst_cg_en,dw_axi_mst_cg_en,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_reserved_4,reserved_4,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_axi_bbphy_cg_en,dw_axi_bbphy_cg_en,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_reserved_3,reserved_3,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_x2x_async_cg_en,dw_x2x_async_cg_en,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_reserved_2,reserved_2,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_x2h_qsync_cg_en,dw_x2h_qsync_cg_en,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_hmx_cg_en,dw_hmx_cg_en,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_x2p_cg_en,dw_x2p_cg_en,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_gs_cg_en,dw_gs_cg_en,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_ashb_gatedclock_en,ashb_gatedclock_en,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_dw_ahb_mst_gatedclock_en,dw_ahb_mst_gatedclock_en,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_reserved_1,reserved_1,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_edmac_autogated_clk_en,edmac_autogated_clk_en,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl21_reserved_0,reserved_0,HI_SC_CTRL21_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL21_OFFSET)
HI_SET_GET(hi_sc_ctrl22_reserved_1,reserved_1,HI_SC_CTRL22_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL22_OFFSET)
HI_SET_GET(hi_sc_ctrl22_bbe16_ema,bbe16_ema,HI_SC_CTRL22_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL22_OFFSET)
HI_SET_GET(hi_sc_ctrl22_bbe16_emaw,bbe16_emaw,HI_SC_CTRL22_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL22_OFFSET)
HI_SET_GET(hi_sc_ctrl22_bbe16_emas,bbe16_emas,HI_SC_CTRL22_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL22_OFFSET)
HI_SET_GET(hi_sc_ctrl22_reserved_0,reserved_0,HI_SC_CTRL22_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL22_OFFSET)
HI_SET_GET(hi_sc_ctrl23_axi_mem_ema,axi_mem_ema,HI_SC_CTRL23_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_axi_mem_emaw,axi_mem_emaw,HI_SC_CTRL23_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_axi_mem_emas,axi_mem_emas,HI_SC_CTRL23_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_reserved_1,reserved_1,HI_SC_CTRL23_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_soc_mem_ema,soc_mem_ema,HI_SC_CTRL23_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_soc_mem_spram_emaw,soc_mem_spram_emaw,HI_SC_CTRL23_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_soc_mem_spram_emas,soc_mem_spram_emas,HI_SC_CTRL23_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_soc_mem_tpram_emab,soc_mem_tpram_emab,HI_SC_CTRL23_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_soc_mem_tpram_emasa,soc_mem_tpram_emasa,HI_SC_CTRL23_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_soc_mem_dpram_emaw,soc_mem_dpram_emaw,HI_SC_CTRL23_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_soc_mem_dpram_emas,soc_mem_dpram_emas,HI_SC_CTRL23_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl23_reserved_0,reserved_0,HI_SC_CTRL23_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL23_OFFSET)
HI_SET_GET(hi_sc_ctrl24_reserved_1,reserved_1,HI_SC_CTRL24_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL24_OFFSET)
HI_SET_GET(hi_sc_ctrl24_moda9_hs_mem_adjust,moda9_hs_mem_adjust,HI_SC_CTRL24_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL24_OFFSET)
HI_SET_GET(hi_sc_ctrl24_reserved_0,reserved_0,HI_SC_CTRL24_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL24_OFFSET)
HI_SET_GET(hi_sc_ctrl24_moda9_l2_ema,moda9_l2_ema,HI_SC_CTRL24_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL24_OFFSET)
HI_SET_GET(hi_sc_ctrl24_moda9_l2_emaw,moda9_l2_emaw,HI_SC_CTRL24_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL24_OFFSET)
HI_SET_GET(hi_sc_ctrl24_moda9_l2_emas,moda9_l2_emas,HI_SC_CTRL24_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL24_OFFSET)
HI_SET_GET(hi_sc_ctrl25_reserved,reserved,HI_SC_CTRL25_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL25_OFFSET)
HI_SET_GET(hi_sc_ctrl26_reserved,reserved,HI_SC_CTRL26_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL26_OFFSET)
HI_SET_GET(hi_sc_ctrl28_acp_filter_start,acp_filter_start,HI_SC_CTRL28_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL28_OFFSET)
HI_SET_GET(hi_sc_ctrl29_acp_filter_end,acp_filter_end,HI_SC_CTRL29_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL29_OFFSET)
HI_SET_GET(hi_sc_ctrl30_reserved,reserved,HI_SC_CTRL30_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL30_OFFSET)
HI_SET_GET(hi_sc_ctrl45_uicc_ic_usb_vbusvalid,uicc_ic_usb_vbusvalid,HI_SC_CTRL45_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL45_OFFSET)
HI_SET_GET(hi_sc_ctrl45_reserved,reserved,HI_SC_CTRL45_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL45_OFFSET)
HI_SET_GET(hi_sc_ctrl52_dsp0_nmi,dsp0_nmi,HI_SC_CTRL52_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL52_OFFSET)
HI_SET_GET(hi_sc_ctrl52_reserved,reserved,HI_SC_CTRL52_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL52_OFFSET)
HI_SET_GET(hi_sc_ctrl55_reserved_7,reserved_7,HI_SC_CTRL55_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_uicc_ret1n,uicc_ret1n,HI_SC_CTRL55_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_reserved_6,reserved_6,HI_SC_CTRL55_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_amon_soc_ret1n,amon_soc_ret1n,HI_SC_CTRL55_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_reserved_5,reserved_5,HI_SC_CTRL55_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_edmac_colldisn,edmac_colldisn,HI_SC_CTRL55_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_reserved_4,reserved_4,HI_SC_CTRL55_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_amon_soc_colldisn,amon_soc_colldisn,HI_SC_CTRL55_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_reserved_3,reserved_3,HI_SC_CTRL55_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_ipf_ret1n,ipf_ret1n,HI_SC_CTRL55_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_ipf_colldisn,ipf_colldisn,HI_SC_CTRL55_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_cipher_ret1n,cipher_ret1n,HI_SC_CTRL55_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_cipher_colldisn,cipher_colldisn,HI_SC_CTRL55_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_reserved_2,reserved_2,HI_SC_CTRL55_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_hs_uart_ret1n,hs_uart_ret1n,HI_SC_CTRL55_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_reserved_1,reserved_1,HI_SC_CTRL55_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_edmac_ret1n,edmac_ret1n,HI_SC_CTRL55_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl55_reserved_0,reserved_0,HI_SC_CTRL55_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL55_OFFSET)
HI_SET_GET(hi_sc_ctrl56_prior_level,prior_level,HI_SC_CTRL56_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL56_OFFSET)
HI_SET_GET(hi_sc_ctrl56_gatedclock_en,gatedclock_en,HI_SC_CTRL56_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL56_OFFSET)
HI_SET_GET(hi_sc_ctrl56_dw_axi_rs_gatedclock_en,dw_axi_rs_gatedclock_en,HI_SC_CTRL56_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL56_OFFSET)
HI_SET_GET(hi_sc_ctrl56_dw_axi_gs_gatedclock_en,dw_axi_gs_gatedclock_en,HI_SC_CTRL56_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL56_OFFSET)
HI_SET_GET(hi_sc_ctrl56_overf_prot,overf_prot,HI_SC_CTRL56_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL56_OFFSET)
HI_SET_GET(hi_sc_ctrl56_reserved,reserved,HI_SC_CTRL56_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL56_OFFSET)
HI_SET_GET(hi_sc_ctrl56_atpram_ctrl,atpram_ctrl,HI_SC_CTRL56_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL56_OFFSET)
HI_SET_GET(hi_sc_ctrl57_soc2lte_tbd,soc2lte_tbd,HI_SC_CTRL57_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL57_OFFSET)
HI_SET_GET(hi_sc_ctrl57_reserved,reserved,HI_SC_CTRL57_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL57_OFFSET)
HI_SET_GET(hi_sc_ctrl68_moda9_l2_ret1n,moda9_l2_ret1n,HI_SC_CTRL68_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_moda9_l2_ret2n,moda9_l2_ret2n,HI_SC_CTRL68_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_moda9_l2_pgen,moda9_l2_pgen,HI_SC_CTRL68_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_reserved_2,reserved_2,HI_SC_CTRL68_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_dsp0_ret1n,dsp0_ret1n,HI_SC_CTRL68_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_dsp0_ret2n,dsp0_ret2n,HI_SC_CTRL68_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_dsp0_pgen,dsp0_pgen,HI_SC_CTRL68_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_reserved_1,reserved_1,HI_SC_CTRL68_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_pd_axi_mem_ret1n,pd_axi_mem_ret1n,HI_SC_CTRL68_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_pd_axi_mem_ret2n,pd_axi_mem_ret2n,HI_SC_CTRL68_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_pd_axi_mem_pgen,pd_axi_mem_pgen,HI_SC_CTRL68_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl68_reserved_0,reserved_0,HI_SC_CTRL68_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL68_OFFSET)
HI_SET_GET(hi_sc_ctrl69_axi_bbphy_xdcdr_sel,axi_bbphy_xdcdr_sel,HI_SC_CTRL69_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL69_OFFSET)
HI_SET_GET(hi_sc_ctrl69_reserved_2,reserved_2,HI_SC_CTRL69_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL69_OFFSET)
HI_SET_GET(hi_sc_ctrl69_axi_glb_xdcdr_sel,axi_glb_xdcdr_sel,HI_SC_CTRL69_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL69_OFFSET)
HI_SET_GET(hi_sc_ctrl69_reserved_1,reserved_1,HI_SC_CTRL69_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL69_OFFSET)
HI_SET_GET(hi_sc_ctrl69_socapb_pslverr_sel,socapb_pslverr_sel,HI_SC_CTRL69_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL69_OFFSET)
HI_SET_GET(hi_sc_ctrl69_x2h_hslverr_sel,x2h_hslverr_sel,HI_SC_CTRL69_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL69_OFFSET)
HI_SET_GET(hi_sc_ctrl69_mst_err_srst_req,mst_err_srst_req,HI_SC_CTRL69_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL69_OFFSET)
HI_SET_GET(hi_sc_ctrl69_reserved_0,reserved_0,HI_SC_CTRL69_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL69_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_bbphy_priority_m1,axi_bbphy_priority_m1,HI_SC_CTRL70_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_bbphy_priority_m2,axi_bbphy_priority_m2,HI_SC_CTRL70_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_bbphy_priority_m3,axi_bbphy_priority_m3,HI_SC_CTRL70_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_bbphy_priority_m4,axi_bbphy_priority_m4,HI_SC_CTRL70_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_reserved_1,reserved_1,HI_SC_CTRL70_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_mst_priority_m9,axi_mst_priority_m9,HI_SC_CTRL70_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_axi_mst_priority_m10,axi_mst_priority_m10,HI_SC_CTRL70_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl70_reserved_0,reserved_0,HI_SC_CTRL70_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL70_OFFSET)
HI_SET_GET(hi_sc_ctrl71_reserved,reserved,HI_SC_CTRL71_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl71_axi_glb_priority_m1,axi_glb_priority_m1,HI_SC_CTRL71_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl71_axi_glb_priority_m2,axi_glb_priority_m2,HI_SC_CTRL71_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl71_axi_glb_priority_m3,axi_glb_priority_m3,HI_SC_CTRL71_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl71_axi_glb_priority_m4,axi_glb_priority_m4,HI_SC_CTRL71_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl71_axi_glb_priority_m5,axi_glb_priority_m5,HI_SC_CTRL71_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl71_axi_glb_priority_m6,axi_glb_priority_m6,HI_SC_CTRL71_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl71_axi_mst_cache_cfg_en,axi_mst_cache_cfg_en,HI_SC_CTRL71_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl71_axi_mst_sideband,axi_mst_sideband,HI_SC_CTRL71_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl71_axi_mst_cache,axi_mst_cache,HI_SC_CTRL71_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL71_OFFSET)
HI_SET_GET(hi_sc_ctrl72_axi_mst_priority_m1,axi_mst_priority_m1,HI_SC_CTRL72_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL72_OFFSET)
HI_SET_GET(hi_sc_ctrl72_axi_mst_priority_m2,axi_mst_priority_m2,HI_SC_CTRL72_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL72_OFFSET)
HI_SET_GET(hi_sc_ctrl72_axi_mst_priority_m3,axi_mst_priority_m3,HI_SC_CTRL72_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL72_OFFSET)
HI_SET_GET(hi_sc_ctrl72_axi_mst_priority_m4,axi_mst_priority_m4,HI_SC_CTRL72_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL72_OFFSET)
HI_SET_GET(hi_sc_ctrl72_axi_mst_priority_m5,axi_mst_priority_m5,HI_SC_CTRL72_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL72_OFFSET)
HI_SET_GET(hi_sc_ctrl72_axi_mst_priority_m6,axi_mst_priority_m6,HI_SC_CTRL72_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL72_OFFSET)
HI_SET_GET(hi_sc_ctrl72_axi_mst_priority_m7,axi_mst_priority_m7,HI_SC_CTRL72_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL72_OFFSET)
HI_SET_GET(hi_sc_ctrl72_axi_mst_priority_m8,axi_mst_priority_m8,HI_SC_CTRL72_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL72_OFFSET)
HI_SET_GET(hi_sc_ctrl73_peri_mst_region1_ctrl,peri_mst_region1_ctrl,HI_SC_CTRL73_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL73_OFFSET)
HI_SET_GET(hi_sc_ctrl73_peri_mst_secctrl_bypass,peri_mst_secctrl_bypass,HI_SC_CTRL73_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL73_OFFSET)
HI_SET_GET(hi_sc_ctrl73_reserved,reserved,HI_SC_CTRL73_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL73_OFFSET)
HI_SET_GET(hi_sc_ctrl73_peri_mst_region1_staddr,peri_mst_region1_staddr,HI_SC_CTRL73_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL73_OFFSET)
HI_SET_GET(hi_sc_ctrl74_reserved,reserved,HI_SC_CTRL74_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL74_OFFSET)
HI_SET_GET(hi_sc_ctrl74_peri_mst_region1_endaddr,peri_mst_region1_endaddr,HI_SC_CTRL74_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL74_OFFSET)
HI_SET_GET(hi_sc_ctrl75_peri_mst_region2_ctrl,peri_mst_region2_ctrl,HI_SC_CTRL75_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL75_OFFSET)
HI_SET_GET(hi_sc_ctrl75_reserved,reserved,HI_SC_CTRL75_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL75_OFFSET)
HI_SET_GET(hi_sc_ctrl75_peri_mst_region2_staddr,peri_mst_region2_staddr,HI_SC_CTRL75_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL75_OFFSET)
HI_SET_GET(hi_sc_ctrl76_reserved,reserved,HI_SC_CTRL76_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL76_OFFSET)
HI_SET_GET(hi_sc_ctrl76_peri_mst_region2_endaddr,peri_mst_region2_endaddr,HI_SC_CTRL76_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL76_OFFSET)
HI_SET_GET(hi_sc_ctrl77_peri_mst_region3_ctrl,peri_mst_region3_ctrl,HI_SC_CTRL77_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL77_OFFSET)
HI_SET_GET(hi_sc_ctrl77_reserved,reserved,HI_SC_CTRL77_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL77_OFFSET)
HI_SET_GET(hi_sc_ctrl77_peri_mst_region3_staddr,peri_mst_region3_staddr,HI_SC_CTRL77_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL77_OFFSET)
HI_SET_GET(hi_sc_ctrl78_reserved,reserved,HI_SC_CTRL78_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL78_OFFSET)
HI_SET_GET(hi_sc_ctrl78_peri_mst_region3_endaddr,peri_mst_region3_endaddr,HI_SC_CTRL78_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL78_OFFSET)
HI_SET_GET(hi_sc_ctrl79_peri_mst_region4_ctrl,peri_mst_region4_ctrl,HI_SC_CTRL79_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL79_OFFSET)
HI_SET_GET(hi_sc_ctrl79_reserved,reserved,HI_SC_CTRL79_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL79_OFFSET)
HI_SET_GET(hi_sc_ctrl79_peri_mst_region4_staddr,peri_mst_region4_staddr,HI_SC_CTRL79_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL79_OFFSET)
HI_SET_GET(hi_sc_ctrl80_reserved,reserved,HI_SC_CTRL80_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL80_OFFSET)
HI_SET_GET(hi_sc_ctrl80_peri_mst_region4_endaddr,peri_mst_region4_endaddr,HI_SC_CTRL80_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL80_OFFSET)
HI_SET_GET(hi_sc_ctrl103_reserved_8,reserved_8,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_bbphy_dsp0_axislv_active_mask,bbphy_dsp0_axislv_active_mask,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_bbphy_bbp_axislv_active_mask,bbphy_bbp_axislv_active_mask,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_reserved_7,reserved_7,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_reserved_6,reserved_6,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_mdmacp_axislv_active_mask,mdmacp_axislv_active_mask,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_reserved_5,reserved_5,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_axi_mem_axislv_active_mask,axi_mem_axislv_active_mask,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_reserved_4,reserved_4,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_bbphy_axislv_active_mask,bbphy_axislv_active_mask,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_reserved_3,reserved_3,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_wbbp_axislv_active_mask,wbbp_axislv_active_mask,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_amon_axislv_active_mask,amon_axislv_active_mask,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_reserved_2,reserved_2,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_wdog_apbslv_active_mask,wdog_apbslv_active_mask,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_timer0_7_apbslv_active_mask,timer0_7_apbslv_active_mask,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_reserved_1,reserved_1,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_uart0_apbslv_active_mask,uart0_apbslv_active_mask,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_edmac_slv_active_mask,edmac_slv_active_mask,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl103_reserved_0,reserved_0,HI_SC_CTRL103_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL103_OFFSET)
HI_SET_GET(hi_sc_ctrl104_reserved,reserved,HI_SC_CTRL104_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL104_OFFSET)
HI_SET_GET(hi_sc_ctrl105_uicc,uicc,HI_SC_CTRL105_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_reserved_6,reserved_6,HI_SC_CTRL105_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_ipcm,ipcm,HI_SC_CTRL105_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_reserved_5,reserved_5,HI_SC_CTRL105_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_hs_uart,hs_uart,HI_SC_CTRL105_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_reserved_4,reserved_4,HI_SC_CTRL105_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_cipher,cipher,HI_SC_CTRL105_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_ipf,ipf,HI_SC_CTRL105_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_reserved_3,reserved_3,HI_SC_CTRL105_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_cicom0,cicom0,HI_SC_CTRL105_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_cicom1,cicom1,HI_SC_CTRL105_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_reserved_2,reserved_2,HI_SC_CTRL105_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_vic1,vic1,HI_SC_CTRL105_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_reserved_1,reserved_1,HI_SC_CTRL105_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_upacc,upacc,HI_SC_CTRL105_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_ctrl105_reserved_0,reserved_0,HI_SC_CTRL105_T,HI_SYSSC_BASE_ADDR, HI_SC_CTRL105_OFFSET)
HI_SET_GET(hi_sc_stat1_reserved_1,reserved_1,HI_SC_STAT1_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_standbywfi,moda9_standbywfi,HI_SC_STAT1_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_standbywfe,moda9_standbywfe,HI_SC_STAT1_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_pmupriv,moda9_pmupriv,HI_SC_STAT1_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_pmusecure,moda9_pmusecure,HI_SC_STAT1_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_smpnamp,moda9_smpnamp,HI_SC_STAT1_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_scuevabort,moda9_scuevabort,HI_SC_STAT1_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_pwrctlo0,moda9_pwrctlo0,HI_SC_STAT1_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_l2_tagclkouten,moda9_l2_tagclkouten,HI_SC_STAT1_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_l2_dataclkouten,moda9_l2_dataclkouten,HI_SC_STAT1_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_l2_idle,moda9_l2_idle,HI_SC_STAT1_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_moda9_l2_clkstopped,moda9_l2_clkstopped,HI_SC_STAT1_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat1_reserved_0,reserved_0,HI_SC_STAT1_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT1_OFFSET)
HI_SET_GET(hi_sc_stat2_reserved_1,reserved_1,HI_SC_STAT2_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT2_OFFSET)
HI_SET_GET(hi_sc_stat2_dsp0_pwaitmode,dsp0_pwaitmode,HI_SC_STAT2_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT2_OFFSET)
HI_SET_GET(hi_sc_stat2_dsp0_xocdmode,dsp0_xocdmode,HI_SC_STAT2_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT2_OFFSET)
HI_SET_GET(hi_sc_stat2_reserved_0,reserved_0,HI_SC_STAT2_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT2_OFFSET)
HI_SET_GET(hi_sc_stat3_hpmx_opc,hpmx_opc,HI_SC_STAT3_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT3_OFFSET)
HI_SET_GET(hi_sc_stat3_hpmx_opc_vld,hpmx_opc_vld,HI_SC_STAT3_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT3_OFFSET)
HI_SET_GET(hi_sc_stat3_reserved_1,reserved_1,HI_SC_STAT3_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT3_OFFSET)
HI_SET_GET(hi_sc_stat3_hpm_opc,hpm_opc,HI_SC_STAT3_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT3_OFFSET)
HI_SET_GET(hi_sc_stat3_hpm_opc_vld,hpm_opc_vld,HI_SC_STAT3_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT3_OFFSET)
HI_SET_GET(hi_sc_stat3_reserved_0,reserved_0,HI_SC_STAT3_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT3_OFFSET)
HI_SET_GET(hi_sc_stat5_dsp0_nmi,dsp0_nmi,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_intr_ipc_ns,intr_ipc_ns,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_intr_ipc_ns_mbx,intr_ipc_ns_mbx,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_dsp_uart_int,dsp_uart_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_edmac_int1,edmac_int1,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_lte_dsp_aagc_int,lte_dsp_aagc_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_lte_dsp_cell_int,lte_dsp_cell_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_lte_dsp_vdl_int,lte_dsp_vdl_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_lte_dsp_cmu_int,lte_dsp_cmu_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_lte_dsp_pwrm_int,lte_dsp_pwrm_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_lte_dsp_cfi_int,lte_dsp_cfi_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_lte_dsp_tdl_int,lte_dsp_tdl_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_lte_dsp_bbp_dma_int,lte_dsp_bbp_dma_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_lte_dsp_pub_int,lte_dsp_pub_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_lte_dsp_synb_int,lte_dsp_synb_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_tds_stu_sfrm_int,tds_stu_sfrm_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_tds_hsupa_int,tds_hsupa_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_tds_harq_int,tds_harq_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_tds_turbo_int,tds_turbo_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_tds_viterbi_int,tds_viterbi_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_tds_rfc_int,tds_rfc_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_tds_fpu_int,tds_fpu_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_tds_stu_dsp_int,tds_stu_dsp_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_ctu_int_lte,ctu_int_lte,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_ctu_int_tds,ctu_int_tds,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_dsp_vic_int,dsp_vic_int,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat5_reserved,reserved,HI_SC_STAT5_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT5_OFFSET)
HI_SET_GET(hi_sc_stat6_reserved_1,reserved_1,HI_SC_STAT6_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT6_OFFSET)
HI_SET_GET(hi_sc_stat6_moda9_parityfail0,moda9_parityfail0,HI_SC_STAT6_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT6_OFFSET)
HI_SET_GET(hi_sc_stat6_moda9_parityfailscu,moda9_parityfailscu,HI_SC_STAT6_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT6_OFFSET)
HI_SET_GET(hi_sc_stat6_reserved_0,reserved_0,HI_SC_STAT6_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT6_OFFSET)
HI_SET_GET(hi_sc_stat9_dw_axi_mst_dlock_wr,dw_axi_mst_dlock_wr,HI_SC_STAT9_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_dw_axi_mst_dlock_slv,dw_axi_mst_dlock_slv,HI_SC_STAT9_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_reserved_2,reserved_2,HI_SC_STAT9_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_dw_axi_mst_dlock_mst,dw_axi_mst_dlock_mst,HI_SC_STAT9_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_reserved_1,reserved_1,HI_SC_STAT9_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_dw_axi_glb_dlock_wr,dw_axi_glb_dlock_wr,HI_SC_STAT9_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_dw_axi_glb_dlock_slv,dw_axi_glb_dlock_slv,HI_SC_STAT9_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_dw_axi_glb_dlock_mst,dw_axi_glb_dlock_mst,HI_SC_STAT9_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat9_reserved_0,reserved_0,HI_SC_STAT9_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT9_OFFSET)
HI_SET_GET(hi_sc_stat10_dw_axi_glb_dlock_id,dw_axi_glb_dlock_id,HI_SC_STAT10_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT10_OFFSET)
HI_SET_GET(hi_sc_stat10_reserved,reserved,HI_SC_STAT10_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT10_OFFSET)
HI_SET_GET(hi_sc_stat10_dw_axi_mst_dlock_id,dw_axi_mst_dlock_id,HI_SC_STAT10_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT10_OFFSET)
HI_SET_GET(hi_sc_stat15_apb_pslv_active0,apb_pslv_active0,HI_SC_STAT15_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT15_OFFSET)
HI_SET_GET(hi_sc_stat15_reserved,reserved,HI_SC_STAT15_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT15_OFFSET)
HI_SET_GET(hi_sc_stat22_dw_axi_bbphy_dlock_mst,dw_axi_bbphy_dlock_mst,HI_SC_STAT22_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT22_OFFSET)
HI_SET_GET(hi_sc_stat22_dw_axi_bbphy_dlock_slv,dw_axi_bbphy_dlock_slv,HI_SC_STAT22_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT22_OFFSET)
HI_SET_GET(hi_sc_stat22_dw_axi_bbphy_dlock_wr,dw_axi_bbphy_dlock_wr,HI_SC_STAT22_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT22_OFFSET)
HI_SET_GET(hi_sc_stat22_reserved_1,reserved_1,HI_SC_STAT22_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT22_OFFSET)
HI_SET_GET(hi_sc_stat22_dw_axi_bbphy_dlock_id,dw_axi_bbphy_dlock_id,HI_SC_STAT22_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT22_OFFSET)
HI_SET_GET(hi_sc_stat22_reserved_0,reserved_0,HI_SC_STAT22_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT22_OFFSET)
HI_SET_GET(hi_sc_stat26_reserved_1,reserved_1,HI_SC_STAT26_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT26_OFFSET)
HI_SET_GET(hi_sc_stat26_ipf_idle,ipf_idle,HI_SC_STAT26_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT26_OFFSET)
HI_SET_GET(hi_sc_stat26_cicom0_clk_state,cicom0_clk_state,HI_SC_STAT26_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT26_OFFSET)
HI_SET_GET(hi_sc_stat26_cicom1_clk_state,cicom1_clk_state,HI_SC_STAT26_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT26_OFFSET)
HI_SET_GET(hi_sc_stat26_reserved_0,reserved_0,HI_SC_STAT26_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT26_OFFSET)
HI_SET_GET(hi_sc_stat27_bbphy_slv_active,bbphy_slv_active,HI_SC_STAT27_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT27_OFFSET)
HI_SET_GET(hi_sc_stat27_reserved_1,reserved_1,HI_SC_STAT27_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT27_OFFSET)
HI_SET_GET(hi_sc_stat27_glb_slv_active,glb_slv_active,HI_SC_STAT27_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT27_OFFSET)
HI_SET_GET(hi_sc_stat27_reserved_0,reserved_0,HI_SC_STAT27_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT27_OFFSET)
HI_SET_GET(hi_sc_stat29_x2h_peri_slv_active,x2h_peri_slv_active,HI_SC_STAT29_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT29_OFFSET)
HI_SET_GET(hi_sc_stat32_lte2soc_tbd,lte2soc_tbd,HI_SC_STAT32_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT32_OFFSET)
HI_SET_GET(hi_sc_stat32_reserved,reserved,HI_SC_STAT32_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT32_OFFSET)
HI_SET_GET(hi_sc_stat35_ap2mdm_key0,ap2mdm_key0,HI_SC_STAT35_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT35_OFFSET)
HI_SET_GET(hi_sc_stat36_ap2mdm_key0,ap2mdm_key0,HI_SC_STAT36_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT36_OFFSET)
HI_SET_GET(hi_sc_stat37_ap2mdm_key2,ap2mdm_key2,HI_SC_STAT37_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT37_OFFSET)
HI_SET_GET(hi_sc_stat38_ap2mdm_key3,ap2mdm_key3,HI_SC_STAT38_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT38_OFFSET)
HI_SET_GET(hi_sc_stat41_bbphy_mst_err,bbphy_mst_err,HI_SC_STAT41_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT41_OFFSET)
HI_SET_GET(hi_sc_stat41_reserved,reserved,HI_SC_STAT41_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT41_OFFSET)
HI_SET_GET(hi_sc_stat41_glb_mst_err,glb_mst_err,HI_SC_STAT41_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT41_OFFSET)
HI_SET_GET(hi_sc_stat42_reserved_1,reserved_1,HI_SC_STAT42_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT42_OFFSET)
HI_SET_GET(hi_sc_stat42_socapb_psel_err,socapb_psel_err,HI_SC_STAT42_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT42_OFFSET)
HI_SET_GET(hi_sc_stat42_ahb_peri_mst_err,ahb_peri_mst_err,HI_SC_STAT42_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT42_OFFSET)
HI_SET_GET(hi_sc_stat42_reserved_0,reserved_0,HI_SC_STAT42_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT42_OFFSET)
HI_SET_GET(hi_sc_stat43_x2h_peri_addr_err,x2h_peri_addr_err,HI_SC_STAT43_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT43_OFFSET)
HI_SET_GET(hi_sc_stat44_reserved,reserved,HI_SC_STAT44_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT44_OFFSET)
HI_SET_GET(hi_sc_stat44_addr_err,addr_err,HI_SC_STAT44_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT44_OFFSET)
HI_SET_GET(hi_sc_stat46_addr_err,addr_err,HI_SC_STAT46_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT46_OFFSET)
HI_SET_GET(hi_sc_stat47_addr_err,addr_err,HI_SC_STAT47_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT47_OFFSET)
HI_SET_GET(hi_sc_stat48_addr_err,addr_err,HI_SC_STAT48_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT48_OFFSET)
HI_SET_GET(hi_sc_stat49_addr_err,addr_err,HI_SC_STAT49_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT49_OFFSET)
HI_SET_GET(hi_sc_stat50_addr_err,addr_err,HI_SC_STAT50_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT50_OFFSET)
HI_SET_GET(hi_sc_stat51_addr_err,addr_err,HI_SC_STAT51_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT51_OFFSET)
HI_SET_GET(hi_sc_stat52_addr_err,addr_err,HI_SC_STAT52_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT52_OFFSET)
HI_SET_GET(hi_sc_stat53_addr_err,addr_err,HI_SC_STAT53_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT53_OFFSET)
HI_SET_GET(hi_sc_stat54_addr_err,addr_err,HI_SC_STAT54_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT54_OFFSET)
HI_SET_GET(hi_sc_stat55_addr_err,addr_err,HI_SC_STAT55_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT55_OFFSET)
HI_SET_GET(hi_sc_stat56_addr_err,addr_err,HI_SC_STAT56_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT56_OFFSET)
HI_SET_GET(hi_sc_stat57_addr_err,addr_err,HI_SC_STAT57_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT57_OFFSET)
HI_SET_GET(hi_sc_stat62_addr_err,addr_err,HI_SC_STAT62_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT62_OFFSET)
HI_SET_GET(hi_sc_stat63_addr_err,addr_err,HI_SC_STAT63_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT63_OFFSET)
HI_SET_GET(hi_sc_stat64_addr_err,addr_err,HI_SC_STAT64_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT64_OFFSET)
HI_SET_GET(hi_sc_stat65_addr_err,addr_err,HI_SC_STAT65_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT65_OFFSET)
HI_SET_GET(hi_sc_stat66_addr_err,addr_err,HI_SC_STAT66_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT66_OFFSET)
HI_SET_GET(hi_sc_stat67_addr_err,addr_err,HI_SC_STAT67_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT67_OFFSET)
HI_SET_GET(hi_sc_stat68_addr_err,addr_err,HI_SC_STAT68_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT68_OFFSET)
HI_SET_GET(hi_sc_stat69_addr_err,addr_err,HI_SC_STAT69_T,HI_SYSSC_BASE_ADDR, HI_SC_STAT69_OFFSET)
HI_SET_GET(hi_pwr_ctrl2_reserved_1,reserved_1,HI_PWR_CTRL2_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL2_OFFSET)
HI_SET_GET(hi_pwr_ctrl2_sc_tcxo0_en_ctrl,sc_tcxo0_en_ctrl,HI_PWR_CTRL2_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL2_OFFSET)
HI_SET_GET(hi_pwr_ctrl2_sc_tcxo1_en_ctrl,sc_tcxo1_en_ctrl,HI_PWR_CTRL2_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL2_OFFSET)
HI_SET_GET(hi_pwr_ctrl2_reserved_0,reserved_0,HI_PWR_CTRL2_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL2_OFFSET)
HI_SET_GET(hi_pwr_ctrl2_sc_tcxo_en_ctrl,sc_tcxo_en_ctrl,HI_PWR_CTRL2_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL2_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_reserved_3,reserved_3,HI_PWR_CTRL4_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_ccpu_iso_ctrl_en,ccpu_iso_ctrl_en,HI_PWR_CTRL4_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_reserved_2,reserved_2,HI_PWR_CTRL4_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_bbe16_iso_ctrl_en,bbe16_iso_ctrl_en,HI_PWR_CTRL4_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_reserved_1,reserved_1,HI_PWR_CTRL4_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_ltebbp_iso_ctrl_en,ltebbp_iso_ctrl_en,HI_PWR_CTRL4_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_twbbp_iso_ctrl_en,twbbp_iso_ctrl_en,HI_PWR_CTRL4_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_wbbp_iso_ctrl_en,wbbp_iso_ctrl_en,HI_PWR_CTRL4_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_g1bbp_iso_ctrl_en,g1bbp_iso_ctrl_en,HI_PWR_CTRL4_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_g2bbp_iso_ctrl_en,g2bbp_iso_ctrl_en,HI_PWR_CTRL4_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_irmbbp_iso_ctrl_en,irmbbp_iso_ctrl_en,HI_PWR_CTRL4_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl4_reserved_0,reserved_0,HI_PWR_CTRL4_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL4_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_reserved_3,reserved_3,HI_PWR_CTRL5_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_ccpu_iso_ctrl_dis,ccpu_iso_ctrl_dis,HI_PWR_CTRL5_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_reserved_2,reserved_2,HI_PWR_CTRL5_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_bbe16_iso_ctrl_dis,bbe16_iso_ctrl_dis,HI_PWR_CTRL5_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_reserved_1,reserved_1,HI_PWR_CTRL5_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_ltebbp_iso_ctrl_dis,ltebbp_iso_ctrl_dis,HI_PWR_CTRL5_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_twbbp_iso_ctrl_dis,twbbp_iso_ctrl_dis,HI_PWR_CTRL5_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_wbbp_iso_ctrl_dis,wbbp_iso_ctrl_dis,HI_PWR_CTRL5_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_g1bbp_iso_ctrl_dis,g1bbp_iso_ctrl_dis,HI_PWR_CTRL5_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_g2bbp_iso_ctrl_dis,g2bbp_iso_ctrl_dis,HI_PWR_CTRL5_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_irmbbp_iso_ctrl_dis,irmbbp_iso_ctrl_dis,HI_PWR_CTRL5_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl5_reserved_0,reserved_0,HI_PWR_CTRL5_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL5_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_reserved_3,reserved_3,HI_PWR_CTRL6_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_ccpu_mtcmos_ctrl_en,ccpu_mtcmos_ctrl_en,HI_PWR_CTRL6_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_reserved_2,reserved_2,HI_PWR_CTRL6_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_bbe16_mtcmos_ctrl_en,bbe16_mtcmos_ctrl_en,HI_PWR_CTRL6_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_reserved_1,reserved_1,HI_PWR_CTRL6_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_ltebbp_mtcmos_ctrl_en,ltebbp_mtcmos_ctrl_en,HI_PWR_CTRL6_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_twbbp_mtcmos_ctrl_en,twbbp_mtcmos_ctrl_en,HI_PWR_CTRL6_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_wbbp_mtcmos_ctrl_en,wbbp_mtcmos_ctrl_en,HI_PWR_CTRL6_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_g1bbp_mtcmos_ctrl_en,g1bbp_mtcmos_ctrl_en,HI_PWR_CTRL6_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_g2bbp_mtcmos_ctrl_en,g2bbp_mtcmos_ctrl_en,HI_PWR_CTRL6_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_irmbbp_mtcmos_ctrl_en,irmbbp_mtcmos_ctrl_en,HI_PWR_CTRL6_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl6_reserved_0,reserved_0,HI_PWR_CTRL6_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL6_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_reserved_3,reserved_3,HI_PWR_CTRL7_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_ccpu_mtcmos_ctrl_dis,ccpu_mtcmos_ctrl_dis,HI_PWR_CTRL7_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_reserved_2,reserved_2,HI_PWR_CTRL7_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_bbe16_mtcmos_ctrl_dis,bbe16_mtcmos_ctrl_dis,HI_PWR_CTRL7_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_reserved_1,reserved_1,HI_PWR_CTRL7_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_ltebbp_mtcmos_ctrl_dis,ltebbp_mtcmos_ctrl_dis,HI_PWR_CTRL7_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_twbbp_mtcmos_ctrl_dis,twbbp_mtcmos_ctrl_dis,HI_PWR_CTRL7_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_wbbp_mtcmos_ctrl_dis,wbbp_mtcmos_ctrl_dis,HI_PWR_CTRL7_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_g1bbp_mtcmos_ctrl_dis,g1bbp_mtcmos_ctrl_dis,HI_PWR_CTRL7_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_g2bbp_mtcmos_ctrl_dis,g2bbp_mtcmos_ctrl_dis,HI_PWR_CTRL7_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_irmbbp_mtcmos_ctrl_dis,irmbbp_mtcmos_ctrl_dis,HI_PWR_CTRL7_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_ctrl7_reserved_0,reserved_0,HI_PWR_CTRL7_T,HI_SYSSC_BASE_ADDR, HI_PWR_CTRL7_OFFSET)
HI_SET_GET(hi_pwr_stat1_reserved_4,reserved_4,HI_PWR_STAT1_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_ccpu_mtcmos_rdy_stat,ccpu_mtcmos_rdy_stat,HI_PWR_STAT1_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_reserved_3,reserved_3,HI_PWR_STAT1_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_bbe16_mtcmos_rdy_stat,bbe16_mtcmos_rdy_stat,HI_PWR_STAT1_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_reserved_2,reserved_2,HI_PWR_STAT1_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_ltebbp_mtcmos_rdy_stat,ltebbp_mtcmos_rdy_stat,HI_PWR_STAT1_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_twbbp_mtcmos_rdy_stat,twbbp_mtcmos_rdy_stat,HI_PWR_STAT1_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_wbbp_mtcmos_rdy_stat,wbbp_mtcmos_rdy_stat,HI_PWR_STAT1_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_g1bbp_mtcmos_rdy_stat,g1bbp_mtcmos_rdy_stat,HI_PWR_STAT1_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_g2bbp_mtcmos_rdy_stat,g2bbp_mtcmos_rdy_stat,HI_PWR_STAT1_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_irmbbp_mtcmos_rdy_stat,irmbbp_mtcmos_rdy_stat,HI_PWR_STAT1_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_reserved_1,reserved_1,HI_PWR_STAT1_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_ltebbp0_mtcmos_rdy_stat,ltebbp0_mtcmos_rdy_stat,HI_PWR_STAT1_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat1_reserved_0,reserved_0,HI_PWR_STAT1_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT1_OFFSET)
HI_SET_GET(hi_pwr_stat3_reserved_3,reserved_3,HI_PWR_STAT3_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_ccpu_iso_ctrl_stat,ccpu_iso_ctrl_stat,HI_PWR_STAT3_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_reserved_2,reserved_2,HI_PWR_STAT3_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_bbe16_iso_ctrl_stat,bbe16_iso_ctrl_stat,HI_PWR_STAT3_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_reserved_1,reserved_1,HI_PWR_STAT3_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_ltebbp_iso_ctrl_stat,ltebbp_iso_ctrl_stat,HI_PWR_STAT3_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_twbbp_iso_ctrl_stat,twbbp_iso_ctrl_stat,HI_PWR_STAT3_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_wbbp_iso_ctrl_stat,wbbp_iso_ctrl_stat,HI_PWR_STAT3_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_g1bbp_iso_ctrl_stat,g1bbp_iso_ctrl_stat,HI_PWR_STAT3_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_g2bbp_iso_ctrl_stat,g2bbp_iso_ctrl_stat,HI_PWR_STAT3_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_irmbbp_iso_ctrl_stat,irmbbp_iso_ctrl_stat,HI_PWR_STAT3_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat3_reserved_0,reserved_0,HI_PWR_STAT3_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT3_OFFSET)
HI_SET_GET(hi_pwr_stat4_reserved_3,reserved_3,HI_PWR_STAT4_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_ccpu_mtcmos_ctrl_stat,ccpu_mtcmos_ctrl_stat,HI_PWR_STAT4_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_reserved_2,reserved_2,HI_PWR_STAT4_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_bbe16_mtcmos_ctrl_stat,bbe16_mtcmos_ctrl_stat,HI_PWR_STAT4_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_reserved_1,reserved_1,HI_PWR_STAT4_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_ltebbp_mtcmos_ctrl_stat,ltebbp_mtcmos_ctrl_stat,HI_PWR_STAT4_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_twbbp_mtcmos_ctrl_stat,twbbp_mtcmos_ctrl_stat,HI_PWR_STAT4_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_wbbp_mtcmos_ctrl_stat,wbbp_mtcmos_ctrl_stat,HI_PWR_STAT4_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_g1bbp_mtcmos_ctrl_stat,g1bbp_mtcmos_ctrl_stat,HI_PWR_STAT4_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_g2bbp_mtcmos_ctrl_stat,g2bbp_mtcmos_ctrl_stat,HI_PWR_STAT4_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_irmbbp_mtcmos_ctrl_stat,irmbbp_mtcmos_ctrl_stat,HI_PWR_STAT4_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_pwr_stat4_reserved_0,reserved_0,HI_PWR_STAT4_T,HI_SYSSC_BASE_ADDR, HI_PWR_STAT4_OFFSET)
HI_SET_GET(hi_sec_ctrl0_ipf_sec_w_ctrl,ipf_sec_w_ctrl,HI_SEC_CTRL0_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL0_OFFSET)
HI_SET_GET(hi_sec_ctrl0_ipf_sec_r_ctrl,ipf_sec_r_ctrl,HI_SEC_CTRL0_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL0_OFFSET)
HI_SET_GET(hi_sec_ctrl0_region1_sec_w_ctrl,region1_sec_w_ctrl,HI_SEC_CTRL0_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL0_OFFSET)
HI_SET_GET(hi_sec_ctrl0_region1_sec_r_ctrl,region1_sec_r_ctrl,HI_SEC_CTRL0_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL0_OFFSET)
HI_SET_GET(hi_sec_ctrl0_ipcm_sec_w_ctrl,ipcm_sec_w_ctrl,HI_SEC_CTRL0_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL0_OFFSET)
HI_SET_GET(hi_sec_ctrl0_ipcm_sec_r_ctrl,ipcm_sec_r_ctrl,HI_SEC_CTRL0_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL0_OFFSET)
HI_SET_GET(hi_sec_ctrl0_aximon_sec_w_ctrl,aximon_sec_w_ctrl,HI_SEC_CTRL0_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL0_OFFSET)
HI_SET_GET(hi_sec_ctrl0_aximon_sec_r_ctrl,aximon_sec_r_ctrl,HI_SEC_CTRL0_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL0_OFFSET)
HI_SET_GET(hi_sec_ctrl0_others_sec_w_ctrl,others_sec_w_ctrl,HI_SEC_CTRL0_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL0_OFFSET)
HI_SET_GET(hi_sec_ctrl0_others_sec_r_ctrl,others_sec_r_ctrl,HI_SEC_CTRL0_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL0_OFFSET)
HI_SET_GET(hi_sec_ctrl0_region2_sec_r_ctrl,region2_sec_r_ctrl,HI_SEC_CTRL0_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL0_OFFSET)
HI_SET_GET(hi_sec_ctrl0_uicc_sec_w_ctrl,uicc_sec_w_ctrl,HI_SEC_CTRL0_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL0_OFFSET)
HI_SET_GET(hi_sec_ctrl0_uicc_sec_r_ctrl,uicc_sec_r_ctrl,HI_SEC_CTRL0_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL0_OFFSET)
HI_SET_GET(hi_sec_ctrl0_reserved,reserved,HI_SEC_CTRL0_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL0_OFFSET)
HI_SET_GET(hi_sec_ctrl1_noddr_bypass,noddr_bypass,HI_SEC_CTRL1_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL1_OFFSET)
HI_SET_GET(hi_sec_ctrl1_reserved,reserved,HI_SEC_CTRL1_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL1_OFFSET)
HI_SET_GET(hi_sec_ctrl1_sci0_sec_w_ctrl,sci0_sec_w_ctrl,HI_SEC_CTRL1_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL1_OFFSET)
HI_SET_GET(hi_sec_ctrl1_sci0_sec_r_ctrl,sci0_sec_r_ctrl,HI_SEC_CTRL1_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL1_OFFSET)
HI_SET_GET(hi_sec_ctrl1_sci1_sec_w_ctrl,sci1_sec_w_ctrl,HI_SEC_CTRL1_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL1_OFFSET)
HI_SET_GET(hi_sec_ctrl1_sci1_sec_r_ctrl,sci1_sec_r_ctrl,HI_SEC_CTRL1_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL1_OFFSET)
HI_SET_GET(hi_sec_ctrl1_ipcm_s_sec_w_ctrl,ipcm_s_sec_w_ctrl,HI_SEC_CTRL1_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL1_OFFSET)
HI_SET_GET(hi_sec_ctrl1_ipcm_s_sec_r_ctrl,ipcm_s_sec_r_ctrl,HI_SEC_CTRL1_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL1_OFFSET)
HI_SET_GET(hi_sec_ctrl1_ipcm_ns_sec_w_ctrl,ipcm_ns_sec_w_ctrl,HI_SEC_CTRL1_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL1_OFFSET)
HI_SET_GET(hi_sec_ctrl1_ipcm_ns_sec_r_ctrl,ipcm_ns_sec_r_ctrl,HI_SEC_CTRL1_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL1_OFFSET)
HI_SET_GET(hi_sec_ctrl1_secram_sec_w_ctrl,secram_sec_w_ctrl,HI_SEC_CTRL1_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL1_OFFSET)
HI_SET_GET(hi_sec_ctrl1_secram_sec_r_ctrl,secram_sec_r_ctrl,HI_SEC_CTRL1_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL1_OFFSET)
HI_SET_GET(hi_sec_ctrl1_socp_sec_w_ctrl,socp_sec_w_ctrl,HI_SEC_CTRL1_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL1_OFFSET)
HI_SET_GET(hi_sec_ctrl1_socp_sec_r_ctrl,socp_sec_r_ctrl,HI_SEC_CTRL1_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL1_OFFSET)
HI_SET_GET(hi_sec_ctrl1_others_sec_w_ctrl,others_sec_w_ctrl,HI_SEC_CTRL1_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL1_OFFSET)
HI_SET_GET(hi_sec_ctrl1_others_sec_r_ctrl,others_sec_r_ctrl,HI_SEC_CTRL1_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL1_OFFSET)
HI_SET_GET(hi_sec_ctrl1_ap_sec_ctrl,ap_sec_ctrl,HI_SEC_CTRL1_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL1_OFFSET)
HI_SET_GET(hi_sec_ctrl2_region1_filter_st_addr,region1_filter_st_addr,HI_SEC_CTRL2_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL2_OFFSET)
HI_SET_GET(hi_sec_ctrl3_region1_filter_end_addr,region1_filter_end_addr,HI_SEC_CTRL3_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL3_OFFSET)
HI_SET_GET(hi_sec_ctrl4_secram_filter_st_addr,secram_filter_st_addr,HI_SEC_CTRL4_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL4_OFFSET)
HI_SET_GET(hi_sec_ctrl5_secram_filter_end_addr,secram_filter_end_addr,HI_SEC_CTRL5_T,HI_SYSSC_BASE_ADDR, HI_SEC_CTRL5_OFFSET)

#endif

#endif // __HI_SYSSC_H__


