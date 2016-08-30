/******************************************************************************/
/*  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_bbp_ctu.h */
/* Version       : 2.0 */
/* Created       : 2013-09-23*/
/* Last Modified : */
/* Description   :  The C union definition file for the module bbp_ctu*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_BBP_CTU_H__
#define __HI_BBP_CTU_H__

/*
 * Project: hi
 * Module : bbp_ctu
 */

#ifndef HI_SET_GET
#define HI_SET_GET(a0,a1,a2,a3,a4)
#endif

/********************************************************************************/
/*    bbp_ctu �Ĵ���ƫ�ƶ��壨��Ŀ��_ģ����_�Ĵ�����_OFFSET)        */
/********************************************************************************/
#define    HI_BBP_CTU_C0_PRIMARY_MODE_IND_OFFSET             (0x0) /* ͨ��0��Ӧ��ģ����ģ0����ģʽָʾ�Ĵ��� */
#define    HI_BBP_CTU_C0_SECOND_MODE_IND_OFFSET              (0x4) /* ��ģ0��Ӧ�Ĵ�ģ����ģ0����ģʽָʾ�Ĵ��� */
#define    HI_BBP_CTU_AFC0_VALUE_INTER_RAT_OFFSET            (0xC) /* �Ĵ��� */
#define    HI_BBP_CTU_C0_AFC_PDM_SELECT_MASK_OFFSET          (0x10) /* �Ĵ��� */
#define    HI_BBP_CTU_C0_ANT_SWITCH_PA_SELECT_MASK_OFFSET    (0x14) /* �Ĵ��� */
#define    HI_BBP_CTU_C0_RFIC_SELECT_MASK_OFFSET             (0x18) /* �Ĵ��� */
#define    HI_BBP_CTU_C0_MIPI_SELECT_MASK_OFFSET             (0x1C) /* �Ĵ��� */
#define    HI_BBP_CTU_C0_ABB_SELECT_MASK_OFFSET              (0x20) /* �Ĵ��� */
#define    HI_BBP_CTU_C0_PMU_SELECT_MASK_OFFSET              (0x24) /* �Ĵ��� */
#define    HI_BBP_CTU_DSP_C0_SEL_MASK_OFFSET                 (0x28) /* �Ĵ��� */
#define    HI_BBP_CTU_DSP_RFIC0_RSTN_OFFSET                  (0x2C) /* �Ĵ��� */
#define    HI_BBP_CTU_C0_SECOND_AFC_MODE_OFFSET              (0x30) /* �Ĵ��� */
#define    HI_BBP_CTU_BBP_SYS_0CONTROL_OFFSET                (0x34) /* �Ĵ��� */
#define    HI_BBP_CTU_MIPI0_RD_OVERTIME_FLAG_DSP_OFFSET      (0x38) /* �Ĵ��� */
#define    HI_BBP_CTU_SSI0_RD_OVERTIME_FLAG_DSP_OFFSET       (0x3C) /* �Ĵ��� */
#define    HI_BBP_CTU_MIPI0_RD_OVERTIME_CLR_OFFSET           (0x40) /* �Ĵ��� */
#define    HI_BBP_CTU_SSI0_RD_OVERTIME_CLR_OFFSET            (0x44) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC0_1_OFFSET               (0x48) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC0_2_OFFSET               (0x4C) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC0_3_OFFSET               (0x50) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC0_4_OFFSET               (0x54) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC0_5_OFFSET               (0x58) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC0_6_OFFSET               (0x5C) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC0_7_OFFSET               (0x60) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC0_8_OFFSET               (0x64) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC0_9_OFFSET               (0x68) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC0_10_OFFSET              (0x6C) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC0_11_OFFSET              (0x70) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC0_12_OFFSET              (0x74) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC0_13_OFFSET              (0x78) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC0_14_OFFSET              (0x7C) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC0_15_OFFSET              (0x80) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC0_16_OFFSET              (0x84) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_FLAG_RFIC0_OFFSET            (0x88) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_FLAG_RFIC0_CLR_OFFSET        (0x8C) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI0_1_OFFSET               (0x90) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI0_2_OFFSET               (0x94) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI0_3_OFFSET               (0x98) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI0_4_OFFSET               (0x9C) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI0_5_OFFSET               (0xA0) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI0_6_OFFSET               (0xA4) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI0_7_OFFSET               (0xA8) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI0_8_OFFSET               (0xAC) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI0_9_OFFSET               (0xB0) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI0_10_OFFSET              (0xB4) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI0_11_OFFSET              (0xB8) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI0_12_OFFSET              (0xBC) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI0_13_OFFSET              (0xC0) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI0_14_OFFSET              (0xC4) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI0_15_OFFSET              (0xC8) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI0_16_OFFSET              (0xCC) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_FLAG_MIPI0_OFFSET            (0xD0) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_FLAG_MIPI0_CLR_OFFSET        (0xD4) /* �Ĵ��� */
#define    HI_BBP_CTU_MIPI0_RD_DATA_LOW_SOFT_OFFSET          (0xD8) /* �Ĵ��� */
#define    HI_BBP_CTU_MIPI0_RD_DATA_HIGH_SOFT_OFFSET         (0xDC) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_END_FLAG_MIPI0_SOFT_OFFSET          (0xE0) /* �Ĵ��� */
#define    HI_BBP_CTU_MIPI0_GRANT_DSP_OFFSET                 (0xE4) /* �Ĵ��� */
#define    HI_BBP_CTU_DSP_MIPI0_WDATA_LOW_OFFSET             (0xE8) /* �Ĵ��� */
#define    HI_BBP_CTU_DSP_MIPI0_WDATA_HIGH_OFFSET            (0xEC) /* �Ĵ��� */
#define    HI_BBP_CTU_DSP_MIPI0_EN_IMI_OFFSET                (0xF0) /* �Ĵ��� */
#define    HI_BBP_CTU_DSP_MIPI0_CFG_IND_IMI_OFFSET           (0xF4) /* �Ĵ��� */
#define    HI_BBP_CTU_DSP_MIPI0_RD_CLR_OFFSET                (0xF8) /* �Ĵ��� */
#define    HI_BBP_CTU_ABB0_LINE_CONTROL_CMD_OFFSET           (0xFC) /* �Ĵ��� */
#define    HI_BBP_CTU_CH0_BBP_SEL_OFFSET                     (0x100)
#define    HI_BBP_CTU_CPU_MIPI0_FUNC_SEL_OFFSET              (0x104)
#define    HI_BBP_CTU_CPU_MIPI0_TEST_FUNC_OFFSET             (0x108)
#define    HI_BBP_CTU_CPU_MIPI0_SCLK_TEST_OFFSET             (0x10C)
#define    HI_BBP_CTU_CPU_MIPI0_SDATA_TEST_OFFSET            (0x110)
#define    HI_BBP_CTU_CPU_MIPI0_CLR_IND_IMI_OFFSET           (0x114)
#define    HI_BBP_CTU_CPU_MIPI0_FIFO_CLR_IMI_OFFSET          (0x118)
#define    HI_BBP_CTU_CPU_SSI0_FIFO_CLR_IMI_OFFSET           (0x11C)
#define    HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_OFFSET          (0x120)
#define    HI_BBP_CTU_C1_PRIMARY_MODE_IND_OFFSET             (0x300) /* ͨ��1��Ӧ��ģ����ģ1����ģʽָʾ�Ĵ��� */
#define    HI_BBP_CTU_C1_SECOND_MODE_IND_OFFSET              (0x304) /* ��ģ1��Ӧ�Ĵ�ģ����ģ1����ģʽָʾ�Ĵ��� */
#define    HI_BBP_CTU_C1_CH_NUM_IND_OFFSET                   (0x308) /* ��ģ1��Ӧͨ����ָʾ�Ĵ��� */
#define    HI_BBP_CTU_AFC1_VALUE_INTER_RAT_OFFSET            (0x30C) /* �Ĵ��� */
#define    HI_BBP_CTU_C1_AFC_PDM_SELECT_MASK_OFFSET          (0x310) /* �Ĵ��� */
#define    HI_BBP_CTU_C1_ANT_SWITCH_PA_SELECT_MASK_OFFSET    (0x314) /* �Ĵ��� */
#define    HI_BBP_CTU_C1_RFIC_SELECT_MASK_OFFSET             (0x318) /* �Ĵ��� */
#define    HI_BBP_CTU_C1_MIPI_SELECT_MASK_OFFSET             (0x31C) /* �Ĵ��� */
#define    HI_BBP_CTU_C1_ABB_SELECT_MASK_OFFSET              (0x320) /* �Ĵ��� */
#define    HI_BBP_CTU_C1_PMU_SELECT_MASK_OFFSET              (0x324) /* �Ĵ��� */
#define    HI_BBP_CTU_DSP_C1_SEL_MASK_OFFSET                 (0x328) /* �Ĵ��� */
#define    HI_BBP_CTU_DSP_RFIC1_RSTN_OFFSET                  (0x32C) /* �Ĵ��� */
#define    HI_BBP_CTU_C1_SECOND_AFC_MODE_OFFSET              (0x330) /* �Ĵ��� */
#define    HI_BBP_CTU_BBP_SYS_1CONTROL_OFFSET                (0x334) /* �Ĵ��� */
#define    HI_BBP_CTU_MIPI1_RD_OVERTIME_FLAG_DSP_OFFSET      (0x338) /* �Ĵ��� */
#define    HI_BBP_CTU_SSI1_RD_OVERTIME_FLAG_DSP_OFFSET       (0x33C) /* �Ĵ��� */
#define    HI_BBP_CTU_MIPI1_RD_OVERTIME_CLR_OFFSET           (0x340) /* �Ĵ��� */
#define    HI_BBP_CTU_SSI1_RD_OVERTIME_CLR_OFFSET            (0x344) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC1_1_OFFSET               (0x348) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC1_2_OFFSET               (0x34C) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC1_3_OFFSET               (0x350) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC1_4_OFFSET               (0x354) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC1_5_OFFSET               (0x358) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC1_6_OFFSET               (0x35C) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC1_7_OFFSET               (0x360) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC1_8_OFFSET               (0x364) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC1_9_OFFSET               (0x368) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC1_10_OFFSET              (0x36C) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC1_11_OFFSET              (0x370) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC1_12_OFFSET              (0x374) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC1_13_OFFSET              (0x378) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC1_14_OFFSET              (0x37C) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC1_15_OFFSET              (0x380) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_RFIC1_16_OFFSET              (0x384) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_FLAG_RFIC1_OFFSET            (0x388) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_FLAG_RFIC1_CLR_OFFSET        (0x38C) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI1_1_OFFSET               (0x390) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI1_2_OFFSET               (0x394) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI1_3_OFFSET               (0x398) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI1_4_OFFSET               (0x39C) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI1_5_OFFSET               (0x3A0) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI1_6_OFFSET               (0x3A4) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI1_7_OFFSET               (0x3A8) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI1_8_OFFSET               (0x3AC) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI1_9_OFFSET               (0x3B0) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI1_10_OFFSET              (0x3B4) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI1_11_OFFSET              (0x3B8) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI1_12_OFFSET              (0x3BC) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI1_13_OFFSET              (0x3C0) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI1_14_OFFSET              (0x3C4) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI1_15_OFFSET              (0x3C8) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_MIPI1_16_OFFSET              (0x3CC) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_FLAG_MIPI1_OFFSET            (0x3D0) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_RESULT_FLAG_MIPI1_CLR_OFFSET        (0x3D4) /* �Ĵ��� */
#define    HI_BBP_CTU_MIPI1_RD_DATA_LOW_SOFT_OFFSET          (0x3D8) /* �Ĵ��� */
#define    HI_BBP_CTU_MIPI1_RD_DATA_HIGH_SOFT_OFFSET         (0x3DC) /* �Ĵ��� */
#define    HI_BBP_CTU_RD_END_FLAG_MIPI1_SOFT_OFFSET          (0x3E0) /* �Ĵ��� */
#define    HI_BBP_CTU_MIPI1_GRANT_DSP_OFFSET                 (0x3E4) /* �Ĵ��� */
#define    HI_BBP_CTU_DSP_MIPI1_WDATA_LOW_OFFSET             (0x3E8) /* �Ĵ��� */
#define    HI_BBP_CTU_DSP_MIPI1_WDATA_HIGH_OFFSET            (0x3EC) /* �Ĵ��� */
#define    HI_BBP_CTU_DSP_MIPI1_EN_IMI_OFFSET                (0x3F0) /* �Ĵ��� */
#define    HI_BBP_CTU_DSP_MIPI1_CFG_IND_IMI_OFFSET           (0x3F4) /* �Ĵ��� */
#define    HI_BBP_CTU_DSP_MIPI1_RD_CLR_OFFSET                (0x3F8) /* �Ĵ��� */
#define    HI_BBP_CTU_ABB1_LINE_CONTROL_CMD_OFFSET           (0x3FC) /* �Ĵ��� */
#define    HI_BBP_CTU_CH1_BBP_SEL_OFFSET                     (0x400)
#define    HI_BBP_CTU_CPU_MIPI1_FUNC_SEL_OFFSET              (0x404)
#define    HI_BBP_CTU_CPU_MIPI1_TEST_FUNC_OFFSET             (0x408)
#define    HI_BBP_CTU_CPU_MIPI1_SCLK_TEST_OFFSET             (0x40C)
#define    HI_BBP_CTU_CPU_MIPI1_SDATA_TEST_OFFSET            (0x410)
#define    HI_BBP_CTU_CPU_MIPI1_CLR_IND_IMI_OFFSET           (0x414)
#define    HI_BBP_CTU_CPU_MIPI1_FIFO_CLR_IMI_OFFSET          (0x418)
#define    HI_BBP_CTU_CPU_SSI1_FIFO_CLR_IMI_OFFSET           (0x41C)
#define    HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_OFFSET          (0x420)
#define    HI_BBP_CTU_DELAY_CNT_OFFSET                       (0x618) /* ��ʱ�����ӳټĴ��� */
#define    HI_BBP_CTU_INT_WAIT_CNT_OFFSET                    (0x61C) /* �ж��ӳټĴ��� */
#define    HI_BBP_CTU_LTEBBP_INPUT_MASK_OFFSET               (0x620) /* �Ĵ��� */
#define    HI_BBP_CTU_WBBP_INPUT_MASK_OFFSET                 (0x624) /* �Ĵ��� */
#define    HI_BBP_CTU_GMBBP_INPUT_MASK_OFFSET                (0x628) /* �Ĵ��� */
#define    HI_BBP_CTU_GSBBP_INPUT_MASK_OFFSET                (0x62C) /* �Ĵ��� */
#define    HI_BBP_CTU_TDSBBP_INPUT_MASK_OFFSET               (0x630) /* �Ĵ��� */
#define    HI_BBP_CTU_LTE_MUX_FRAME_NUM_OFFSET               (0x634) /* �Ĵ��� */
#define    HI_BBP_CTU_TDS_MUX_FRAME_NUM_OFFSET               (0x638) /* �Ĵ��� */
#define    HI_BBP_CTU_W_MUX_FRAME_NUM_OFFSET                 (0x63C) /* �Ĵ��� */
#define    HI_BBP_CTU_MEAS_FLAG_L_RD_OFFSET                  (0x640) /* �Ĵ��� */
#define    HI_BBP_CTU_MEAS_FLAG_W_RD_OFFSET                  (0x644) /* �Ĵ��� */
#define    HI_BBP_CTU_MEAS_FLAG_T_RD_OFFSET                  (0x648) /* �Ĵ��� */
#define    HI_BBP_CTU_MEAS_FLAG_GM_RD_OFFSET                 (0x64C) /* �Ĵ��� */
#define    HI_BBP_CTU_MEAS_FLAG_GS_RD_OFFSET                 (0x650) /* �Ĵ��� */
#define    HI_BBP_CTU_WAKE_FLAG_L_WR_OFFSET                  (0x654) /* �Ĵ��� */
#define    HI_BBP_CTU_WAKE_FLAG_W_WR_OFFSET                  (0x658) /* �Ĵ��� */
#define    HI_BBP_CTU_WAKE_FLAG_T_WR_OFFSET                  (0x65C) /* �Ĵ��� */
#define    HI_BBP_CTU_WAKE_FLAG_GM_WR_OFFSET                 (0x660) /* �Ĵ��� */
#define    HI_BBP_CTU_WAKE_FLAG_GS_WR_OFFSET                 (0x664) /* �Ĵ��� */
#define    HI_BBP_CTU_TIMING_GET_IND_OFFSET                  (0x610) /* ��ʱ����ʹ��ָʾ�Ĵ��� */
#define    HI_BBP_CTU_TIMING_CLEAR_OFFSET                    (0x614) /* ��ʱ��־����Ĵ��� */
#define    HI_BBP_CTU_TIMING_VALID_OFFSET                    (0x668) /* �Ĵ��� */
#define    HI_BBP_CTU_W_TIMING_DSP_OFFSET                    (0x66C) /* �Ĵ��� */
#define    HI_BBP_CTU_GM_FN_DSP_OFFSET                       (0x670) /* �Ĵ��� */
#define    HI_BBP_CTU_GM_QB_DSP_OFFSET                       (0x674) /* �Ĵ��� */
#define    HI_BBP_CTU_GS_FN_DSP_OFFSET                       (0x678) /* �Ĵ��� */
#define    HI_BBP_CTU_GS_QB_DSP_OFFSET                       (0x67C) /* �Ĵ��� */
#define    HI_BBP_CTU_LTE_TIMING_DSP_OFFSET                  (0x680) /* �Ĵ��� */
#define    HI_BBP_CTU_TDS_TIMING1_DSP_OFFSET                 (0x684) /* �Ĵ��� */
#define    HI_BBP_CTU_TDS_TIMING2_DSP_OFFSET                 (0x688) /* �Ĵ��� */
#define    HI_BBP_CTU_CTU_TIMING_DSP_OFFSET                  (0x68C) /* �Ĵ��� */
#define    HI_BBP_CTU_CTU_BASECNT1_DSP_OFFSET                (0x690) /* �Ĵ��� */
#define    HI_BBP_CTU_CTU_BASECNT2_DSP_OFFSET                (0x694) /* �Ĵ��� */
#define    HI_BBP_CTU_CTU_BASECNT_DSP_OFFSET                 (0x6B8) /* �Ĵ��� */
#define    HI_BBP_CTU_DSP_CTU_TIMING_GET_IND_OFFSET          (0x6BC) /* �Ĵ��� */
#define    HI_BBP_CTU_CTU_TIMING_VALID_DSP_OFFSET            (0x6C0) /* �Ĵ��� */
#define    HI_BBP_CTU_DSP_CTU_TIMING_CLR_OFFSET              (0x6C4) /* �Ĵ��� */
#define    HI_BBP_CTU_W_INTRASYS_VALID_OFFSET                (0x698) /* �Ĵ��� */
#define    HI_BBP_CTU_GM_INTRASYS_VALID_OFFSET               (0x69C) /* �Ĵ��� */
#define    HI_BBP_CTU_GS_INTRASYS_VALID_OFFSET               (0x6A0) /* �Ĵ��� */
#define    HI_BBP_CTU_LTE_INTRASYS_VALID_OFFSET              (0x6A4) /* �Ĵ��� */
#define    HI_BBP_CTU_TDS_INTRASYS_VALID_OFFSET              (0x6A8) /* �Ĵ��� */
#define    HI_BBP_CTU_RFIC_CK_CFG_OFFSET                     (0x6AC) /* �Ĵ��� */
#define    HI_BBP_CTU_DSP_WAITTIME_OFFSET                    (0x6B4) /* �Ĵ��� */
#define    HI_BBP_CTU_S_W_CH_NUM_IND_OFFSET                  (0xA40) /* WΪ��ģʱ��Ӧͨ����ָʾ�Ĵ��� */
#define    HI_BBP_CTU_W_INTERSYS_MEASURE_TYPE_OFFSET         (0x6C8) /* �Ĵ��� */
#define    HI_BBP_CTU_W_MEASURE_REPORT_VALID_OFFSET          (0x6CC) /* �Ĵ��� */
#define    HI_BBP_CTU_W_PREINT_OFFSET_TIME_OFFSET            (0x6D0) /* �Ĵ��� */
#define    HI_BBP_CTU_W_INTERSYS_MEASURE_TIME_OFFSET         (0x6D4) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_W_RCV_LTE_OFFSET             (0x6D8) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_W_RCV_TDS_OFFSET             (0x6DC) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_W_RCV_GM_OFFSET              (0x6E0) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_W_RCV_GS_OFFSET              (0x6E4) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_W_RCV_W_OFFSET               (0x6E8) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_W_INT012_OFFSET              (0x6EC) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_W_RCV_LTE_OFFSET              (0x6F0) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_W_RCV_TDS_OFFSET              (0x6F4) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_W_RCV_GM_OFFSET               (0x6F8) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_W_RCV_GS_OFFSET               (0x6FC) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_W_RCV_W_OFFSET                (0x700) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_W_INT012_OFFSET               (0x704) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_W_RCV_LTE_OFFSET              (0x708) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_W_RCV_TDS_OFFSET              (0x70C) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_W_RCV_GM_OFFSET               (0x710) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_W_RCV_GS_OFFSET               (0x714) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_W_RCV_W_OFFSET                (0x718) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_W_INT012_OFFSET               (0x71C) /* �Ĵ��� */
#define    HI_BBP_CTU_W_SND_LTE_INT_13_OFFSET                (0x720) /* �Ĵ��� */
#define    HI_BBP_CTU_W_SND_TDS_INT_13_OFFSET                (0x724) /* �Ĵ��� */
#define    HI_BBP_CTU_W_SND_GM_INT_13_OFFSET                 (0x728) /* �Ĵ��� */
#define    HI_BBP_CTU_W_SND_GS_INT_13_OFFSET                 (0x72C) /* �Ĵ��� */
#define    HI_BBP_CTU_W_SND_W_INT_13_OFFSET                  (0x730) /* �Ĵ��� */
#define    HI_BBP_CTU_S_LTE_CH_NUM_IND_OFFSET                (0xA44) /* LTEΪ��ģʱ��Ӧͨ����ָʾ�Ĵ��� */
#define    HI_BBP_CTU_LTE_INTERSYS_MEASURE_TYPE_OFFSET       (0x734) /* �Ĵ��� */
#define    HI_BBP_CTU_LTE_MEASURE_REPORT_VALID_OFFSET        (0x738) /* �Ĵ��� */
#define    HI_BBP_CTU_LTE_PREINT_OFFSET_TIME_OFFSET          (0x73C) /* �Ĵ��� */
#define    HI_BBP_CTU_LTE_INTERSYS_MEASURE_TIME_OFFSET       (0x740) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_LTE_RCV_LTE_OFFSET           (0x744) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_LTE_RCV_TDS_OFFSET           (0x748) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_LTE_RCV_GM_OFFSET            (0x74C) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_LTE_RCV_GS_OFFSET            (0x750) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_LTE_RCV_W_OFFSET             (0x754) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_LTE_INT012_OFFSET            (0x758) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_LTE_RCV_LTE_OFFSET            (0x75C) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_LTE_RCV_TDS_OFFSET            (0x760) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_LTE_RCV_GM_OFFSET             (0x764) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_LTE_RCV_GS_OFFSET             (0x768) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_LTE_RCV_W_OFFSET              (0x76C) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_LTE_INT012_OFFSET             (0x770) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_LTE_RCV_W_OFFSET              (0x774) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_LTE_RCV_TDS_OFFSET            (0x778) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_LTE_RCV_GM_OFFSET             (0x77C) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_LTE_RCV_GS_OFFSET             (0x780) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_LTE_RCV_LTE_OFFSET            (0x784) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_LTE_INT012_OFFSET             (0x788) /* �Ĵ��� */
#define    HI_BBP_CTU_LTE_SND_LTE_INT_13_OFFSET              (0x78C) /* �Ĵ��� */
#define    HI_BBP_CTU_LTE_SND_TDS_INT_13_OFFSET              (0x790) /* �Ĵ��� */
#define    HI_BBP_CTU_LTE_SND_GM_INT_13_OFFSET               (0x794) /* �Ĵ��� */
#define    HI_BBP_CTU_LTE_SND_GS_INT_13_OFFSET               (0x798) /* �Ĵ��� */
#define    HI_BBP_CTU_LTE_SND_W_INT_13_OFFSET                (0x79C) /* �Ĵ��� */
#define    HI_BBP_CTU_S_TDS_CH_NUM_IND_OFFSET                (0xA48) /* TDSΪ��ģʱ��Ӧͨ����ָʾ�Ĵ��� */
#define    HI_BBP_CTU_TDS_INTERSYS_MEASURE_TYPE_OFFSET       (0x7A0) /* �Ĵ��� */
#define    HI_BBP_CTU_TDS_MEASURE_REPORT_VALID_OFFSET        (0x7A4) /* �Ĵ��� */
#define    HI_BBP_CTU_TDS_PREINT_OFFSET_TIME_OFFSET          (0x7A8) /* �Ĵ��� */
#define    HI_BBP_CTU_TDS_INTERSYS_MEASURE_TIME_OFFSET       (0x7AC) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_TDS_RCV_LTE_OFFSET           (0x7B0) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_TDS_RCV_TDS_OFFSET           (0x7B4) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_TDS_RCV_GM_OFFSET            (0x7B8) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_TDS_RCV_GS_OFFSET            (0x7BC) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_TDS_RCV_W_OFFSET             (0x7C0) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_TDS_INT012_OFFSET            (0x7C4) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_TDS_RCV_LTE_OFFSET            (0x7C8) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_TDS_RCV_TDS_OFFSET            (0x7CC) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_TDS_RCV_GM_OFFSET             (0x7D0) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_TDS_RCV_GS_OFFSET             (0x7D4) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_TDS_RCV_W_OFFSET              (0x7D8) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_TDS_INT012_OFFSET             (0x7DC) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_TDS_RCV_LTE_OFFSET            (0x7E0) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_TDS_RCV_W_OFFSET              (0x7E4) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_TDS_RCV_GM_OFFSET             (0x7E8) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_TDS_RCV_GS_OFFSET             (0x7EC) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_TDS_RCV_TDS_OFFSET            (0x7F0) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_TDS_INT012_OFFSET             (0x7F4) /* �Ĵ��� */
#define    HI_BBP_CTU_TDS_SND_LTE_INT_13_OFFSET              (0x7F8) /* �Ĵ��� */
#define    HI_BBP_CTU_TDS_SND_TDS_INT_13_OFFSET              (0x7FC) /* �Ĵ��� */
#define    HI_BBP_CTU_TDS_SND_GM_INT_13_OFFSET               (0x800) /* �Ĵ��� */
#define    HI_BBP_CTU_TDS_SND_GS_INT_13_OFFSET               (0x804) /* �Ĵ��� */
#define    HI_BBP_CTU_TDS_SND_W_INT_13_OFFSET                (0x808) /* �Ĵ��� */
#define    HI_BBP_CTU_S_GM_CH_NUM_IND_OFFSET                 (0xA4C) /* GMΪ��ģʱ��Ӧͨ����ָʾ�Ĵ��� */
#define    HI_BBP_CTU_GM_INTERSYS_MEASURE_TYPE_OFFSET        (0x80C) /* �Ĵ��� */
#define    HI_BBP_CTU_GM_MEASURE_REPORT_VALID_OFFSET         (0x810) /* �Ĵ��� */
#define    HI_BBP_CTU_GM_PREINT_OFFSET_TIME_OFFSET           (0x814) /* �Ĵ��� */
#define    HI_BBP_CTU_GM_INTERSYS_MEASURE_TIME_OFFSET        (0x818) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_GM_RCV_LTE_OFFSET            (0x81C) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_GM_RCV_TDS_OFFSET            (0x820) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_GM_RCV_GM_OFFSET             (0x824) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_GM_RCV_GS_OFFSET             (0x828) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_GM_RCV_W_OFFSET              (0x82C) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_GM_INT012_OFFSET             (0x830) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_GM_RCV_LTE_OFFSET             (0x834) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_GM_RCV_TDS_OFFSET             (0x838) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_GM_RCV_GM_OFFSET              (0x83C) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_GM_RCV_GS_OFFSET              (0x840) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_GM_RCV_W_OFFSET               (0x844) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_GM_INT012_OFFSET              (0x848) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_GM_RCV_LTE_OFFSET             (0x84C) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_GM_RCV_TDS_OFFSET             (0x850) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_GM_RCV_W_OFFSET               (0x854) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_GM_RCV_GS_OFFSET              (0x858) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_GM_RCV_GM_OFFSET              (0x85C) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_GM_INT012_OFFSET              (0x860) /* �Ĵ��� */
#define    HI_BBP_CTU_GM_SND_LTE_INT_13_OFFSET               (0x864) /* �Ĵ��� */
#define    HI_BBP_CTU_GM_SND_TDS_INT_13_OFFSET               (0x868) /* �Ĵ��� */
#define    HI_BBP_CTU_GM_SND_GM_INT_13_OFFSET                (0x86C) /* �Ĵ��� */
#define    HI_BBP_CTU_GM_SND_GS_INT_13_OFFSET                (0x870) /* �Ĵ��� */
#define    HI_BBP_CTU_GM_SND_W_INT_13_OFFSET                 (0x874) /* �Ĵ��� */
#define    HI_BBP_CTU_S_GS_CH_NUM_IND_OFFSET                 (0xA50) /* GSΪ��ģʱ��Ӧͨ����ָʾ�Ĵ��� */
#define    HI_BBP_CTU_GS_INTERSYS_MEASURE_TYPE_OFFSET        (0x878) /* �Ĵ��� */
#define    HI_BBP_CTU_GS_MEASURE_REPORT_VALID_OFFSET         (0x87C) /* �Ĵ��� */
#define    HI_BBP_CTU_GS_PREINT_OFFSET_TIME_OFFSET           (0x880) /* �Ĵ��� */
#define    HI_BBP_CTU_GS_INTERSYS_MEASURE_TIME_OFFSET        (0x884) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_GS_RCV_LTE_OFFSET            (0x888) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_GS_RCV_TDS_OFFSET            (0x88C) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_GS_RCV_GM_OFFSET             (0x890) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_GS_RCV_GS_OFFSET             (0x894) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_GS_RCV_W_OFFSET              (0x898) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_CLEAR_GS_INT012_OFFSET             (0x89C) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_GS_RCV_LTE_OFFSET             (0x8A0) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_GS_RCV_TDS_OFFSET             (0x8A4) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_GS_RCV_GM_OFFSET              (0x8A8) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_GS_RCV_GS_OFFSET              (0x8AC) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_GS_RCV_W_OFFSET               (0x8B0) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_MASK_GS_INT012_OFFSET              (0x8B4) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_GS_RCV_LTE_OFFSET             (0x8B8) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_GS_RCV_TDS_OFFSET             (0x8BC) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_GS_RCV_GM_OFFSET              (0x8C0) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_GS_RCV_GS_OFFSET              (0x8C4) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_GS_RCV_W_OFFSET               (0x8C8) /* �Ĵ��� */
#define    HI_BBP_CTU_INT_TYPE_GS_INT012_OFFSET              (0x8CC) /* �Ĵ��� */
#define    HI_BBP_CTU_GS_SND_LTE_INT_13_OFFSET               (0x8D0) /* �Ĵ��� */
#define    HI_BBP_CTU_GS_SND_TDS_INT_13_OFFSET               (0x8D4) /* �Ĵ��� */
#define    HI_BBP_CTU_GS_SND_GM_INT_13_OFFSET                (0x8D8) /* �Ĵ��� */
#define    HI_BBP_CTU_GS_SND_GS_INT_13_OFFSET                (0x8DC) /* �Ĵ��� */
#define    HI_BBP_CTU_GS_SND_W_INT_13_OFFSET                 (0x8E0) /* �Ĵ��� */
#define    HI_BBP_CTU_GBBP1_19M_SEL_OFFSET                   (0x8E4)
#define    HI_BBP_CTU_GBBP2_19M_SEL_OFFSET                   (0x8E8)
#define    HI_BBP_CTU_WBBP_19M_SEL_OFFSET                    (0x8EC)
#define    HI_BBP_CTU_LBBP_19M_SEL_OFFSET                    (0x8F0)
#define    HI_BBP_CTU_TBBP1_19M_SEL_OFFSET                   (0x8F4)
#define    HI_BBP_CTU_GBBP1_CLK_SEL_OFFSET                   (0x8F8)
#define    HI_BBP_CTU_GBBP2_CLK_SEL_OFFSET                   (0x8FC)
#define    HI_BBP_CTU_WBBP_CLK_SEL_OFFSET                    (0x900)
#define    HI_BBP_CTU_LBBP_CLK_SEL_OFFSET                    (0x904)
#define    HI_BBP_CTU_TBBP_CLK_SEL_OFFSET                    (0x908)
#define    HI_BBP_CTU_LTE_ABBIF_FMT_OFFSET                   (0x90C) /* �Ĵ��� */
#define    HI_BBP_CTU_W_ABBIF_FMT_OFFSET                     (0x910) /* �Ĵ��� */
#define    HI_BBP_CTU_TDS_ABBIF_FMT_OFFSET                   (0x914) /* �Ĵ��� */
#define    HI_BBP_CTU_GM_ABBIF_FMT_OFFSET                    (0x918) /* �Ĵ��� */
#define    HI_BBP_CTU_GS_ABBIF_FMT_OFFSET                    (0x91C) /* �Ĵ��� */
#define    HI_BBP_CTU_REG0_FOR_USE_OFFSET                    (0x920) /* �Ĵ��� */
#define    HI_BBP_CTU_REG1_FOR_USE_OFFSET                    (0x924) /* �Ĵ��� */
#define    HI_BBP_CTU_REG2_FOR_USE_OFFSET                    (0x928) /* �Ĵ��� */
#define    HI_BBP_CTU_REG3_FOR_USE_OFFSET                    (0x92C) /* �Ĵ��� */
#define    HI_BBP_CTU_REG4_FOR_USE_OFFSET                    (0x930) /* �Ĵ��� */
#define    HI_BBP_CTU_REG5_FOR_USE_OFFSET                    (0x934) /* �Ĵ��� */
#define    HI_BBP_CTU_REG6_FOR_USE_OFFSET                    (0x938) /* �Ĵ��� */
#define    HI_BBP_CTU_REG7_FOR_USE_OFFSET                    (0x93C) /* �Ĵ��� */
#define    HI_BBP_CTU_REG8_FOR_USE_OFFSET                    (0x940) /* �Ĵ��� */
#define    HI_BBP_CTU_REG9_FOR_USE_OFFSET                    (0x944) /* �Ĵ��� */
#define    HI_BBP_CTU_REG10_FOR_USE_OFFSET                   (0x948) /* �Ĵ��� */
#define    HI_BBP_CTU_REG11_FOR_USE_OFFSET                   (0x94C) /* �Ĵ��� */
#define    HI_BBP_CTU_REG12_FOR_USE_OFFSET                   (0x950) /* �Ĵ��� */
#define    HI_BBP_CTU_REG13_FOR_USE_OFFSET                   (0x954) /* �Ĵ��� */
#define    HI_BBP_CTU_REG14_FOR_USE_OFFSET                   (0x958) /* �Ĵ��� */
#define    HI_BBP_CTU_REG15_FOR_USE_OFFSET                   (0x95C) /* �Ĵ��� */
#define    HI_BBP_CTU_REG16_FOR_USE_OFFSET                   (0x960) /* �Ĵ��� */
#define    HI_BBP_CTU_REG17_FOR_USE_OFFSET                   (0x964) /* �Ĵ��� */
#define    HI_BBP_CTU_REG18_FOR_USE_OFFSET                   (0x968) /* �Ĵ��� */
#define    HI_BBP_CTU_REG19_FOR_USE_OFFSET                   (0x96C) /* �Ĵ��� */
#define    HI_BBP_CTU_REG20_FOR_USE_OFFSET                   (0x970) /* �Ĵ��� */
#define    HI_BBP_CTU_REG21_FOR_USE_OFFSET                   (0x974) /* �Ĵ��� */
#define    HI_BBP_CTU_REG22_FOR_USE_OFFSET                   (0x978) /* �Ĵ��� */
#define    HI_BBP_CTU_REG23_FOR_USE_OFFSET                   (0x97C) /* �Ĵ��� */
#define    HI_BBP_CTU_REG24_FOR_USE_OFFSET                   (0x980) /* �Ĵ��� */
#define    HI_BBP_CTU_REG25_FOR_USE_OFFSET                   (0x984) /* �Ĵ��� */
#define    HI_BBP_CTU_REG26_FOR_USE_OFFSET                   (0x988) /* �Ĵ��� */
#define    HI_BBP_CTU_REG27_FOR_USE_OFFSET                   (0x98C) /* �Ĵ��� */
#define    HI_BBP_CTU_REG28_FOR_USE_OFFSET                   (0x990) /* �Ĵ��� */
#define    HI_BBP_CTU_REG29_FOR_USE_OFFSET                   (0x994) /* �Ĵ��� */
#define    HI_BBP_CTU_REG30_FOR_USE_OFFSET                   (0x998) /* �Ĵ��� */
#define    HI_BBP_CTU_REG31_FOR_USE_OFFSET                   (0x99C) /* �Ĵ��� */
#define    HI_BBP_CTU_REG32_FOR_USE_OFFSET                   (0x9A0) /* �Ĵ��� */
#define    HI_BBP_CTU_REG33_FOR_USE_OFFSET                   (0x9A4) /* �Ĵ��� */
#define    HI_BBP_CTU_REG34_FOR_USE_OFFSET                   (0x9A8) /* �Ĵ��� */
#define    HI_BBP_CTU_REG35_FOR_USE_OFFSET                   (0x9AC) /* �Ĵ��� */
#define    HI_BBP_CTU_REG36_FOR_USE_OFFSET                   (0x9B0) /* �Ĵ��� */
#define    HI_BBP_CTU_REG37_FOR_USE_OFFSET                   (0x9B4) /* �Ĵ��� */
#define    HI_BBP_CTU_REG38_FOR_USE_OFFSET                   (0x9B8) /* �Ĵ��� */
#define    HI_BBP_CTU_REG39_FOR_USE_OFFSET                   (0x9BC) /* �Ĵ��� */
#define    HI_BBP_CTU_REG40_FOR_USE_OFFSET                   (0x9C0) /* �Ĵ��� */
#define    HI_BBP_CTU_REG41_FOR_USE_OFFSET                   (0x9C4) /* �Ĵ��� */
#define    HI_BBP_CTU_REG42_FOR_USE_OFFSET                   (0x9C8) /* �Ĵ��� */
#define    HI_BBP_CTU_REG43_FOR_USE_OFFSET                   (0x9CC) /* �Ĵ��� */
#define    HI_BBP_CTU_REG44_FOR_USE_OFFSET                   (0x9D0) /* �Ĵ��� */
#define    HI_BBP_CTU_REG45_FOR_USE_OFFSET                   (0x9D4) /* �Ĵ��� */
#define    HI_BBP_CTU_REG46_FOR_USE_OFFSET                   (0x9D8) /* �Ĵ��� */
#define    HI_BBP_CTU_REG47_FOR_USE_OFFSET                   (0x9DC) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG0_CPU_OFFSET                    (0x9E0) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG1_CPU_OFFSET                    (0x9E4) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG2_CPU_OFFSET                    (0x9E8) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG3_CPU_OFFSET                    (0x9EC) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG4_CPU_OFFSET                    (0x9F0) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG5_CPU_OFFSET                    (0x9F4) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG6_CPU_OFFSET                    (0x9F8) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG7_CPU_OFFSET                    (0x9FC) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG8_CPU_OFFSET                    (0xA00) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG9_CPU_OFFSET                    (0xA04) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG10_CPU_OFFSET                   (0xA08) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG11_CPU_OFFSET                   (0xA0C) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG12_CPU_OFFSET                   (0xA10) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG13_CPU_OFFSET                   (0xA14) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG14_CPU_OFFSET                   (0xA18) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG15_CPU_OFFSET                   (0xA1C) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG16_CPU_OFFSET                   (0xA20) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG17_CPU_OFFSET                   (0xA24) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG18_CPU_OFFSET                   (0xA28) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_REG19_CPU_OFFSET                   (0xA2C) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_CLK_GATE_EN_OFFSET                 (0xA30) /* �ſ�ʱ�����üĴ��� */
#define    HI_BBP_CTU_DBG_REG20_CPU_OFFSET                   (0xA34) /* �Ĵ��� */
#define    HI_BBP_CTU_DBG_COMM_EN_OFFSET                     (0xA38) /* �Ĵ��� */
#define    HI_BBP_CTU_L_CH_SW_OFFSET                         (0xB00)
#define    HI_BBP_CTU_W_CH_SW_OFFSET                         (0xB04)
#define    HI_BBP_CTU_T_CH_SW_OFFSET                         (0xB08)
#define    HI_BBP_CTU_GM_CH_SW_OFFSET                        (0xB0C)
#define    HI_BBP_CTU_GS_CH_SW_OFFSET                        (0xB10)
#define    HI_BBP_CTU_L_TCVR_SEL0_OFFSET                     (0xB20)
#define    HI_BBP_CTU_L_TCVR_SEL1_OFFSET                     (0xB24)
#define    HI_BBP_CTU_T_TCVR_SEL0_OFFSET                     (0xB28)
#define    HI_BBP_CTU_T_TCVR_SEL1_OFFSET                     (0xB2C)
#define    HI_BBP_CTU_W_TCVR_SEL0_OFFSET                     (0xB30)
#define    HI_BBP_CTU_W_TCVR_SEL1_OFFSET                     (0xB34)
#define    HI_BBP_CTU_GM_TCVR_SEL0_OFFSET                    (0xB38)
#define    HI_BBP_CTU_GM_TCVR_SEL1_OFFSET                    (0xB3C)
#define    HI_BBP_CTU_GS_TCVR_SEL0_OFFSET                    (0xB40)
#define    HI_BBP_CTU_GS_TCVR_SEL1_OFFSET                    (0xB44)
#define    HI_BBP_CTU_L_TCVR_VALUE0_OFFSET                   (0xB48)
#define    HI_BBP_CTU_L_TCVR_VALUE1_OFFSET                   (0xB4C)
#define    HI_BBP_CTU_T_TCVR_VALUE0_OFFSET                   (0xB50)
#define    HI_BBP_CTU_T_TCVR_VALUE1_OFFSET                   (0xB54)
#define    HI_BBP_CTU_W_TCVR_VALUE0_OFFSET                   (0xB58)
#define    HI_BBP_CTU_W_TCVR_VALUE1_OFFSET                   (0xB5C)
#define    HI_BBP_CTU_GM_TCVR_VALUE0_OFFSET                  (0xB60)
#define    HI_BBP_CTU_GM_TCVR_VALUE1_OFFSET                  (0xB64)
#define    HI_BBP_CTU_GS_TCVR_VALUE0_OFFSET                  (0xB68)
#define    HI_BBP_CTU_GS_TCVR_VALUE1_OFFSET                  (0xB6C)
#define    HI_BBP_CTU_RF_TCVR_STATE_OFFSET                   (0xB70)
#define    HI_BBP_CTU_MIPI_UNBIND_EN_OFFSET                  (0xB90)
#define    HI_BBP_CTU_MIPI_CH_SEL0_OFFSET                    (0xB94)
#define    HI_BBP_CTU_MIPI_CH_SEL1_OFFSET                    (0xB98)
#define    HI_BBP_CTU_APT_CH_SEL0_OFFSET                     (0xB9C)
#define    HI_BBP_CTU_APT_CH_SEL1_OFFSET                     (0xBA0)
#define    HI_BBP_CTU_AFC_CH_SEL0_OFFSET                     (0xBA4)
#define    HI_BBP_CTU_AFC_CH_SEL1_OFFSET                     (0xBA8)
#define    HI_BBP_CTU_ABB_TX_CH_SEL0_OFFSET                  (0xBAC)
#define    HI_BBP_CTU_ABB_TX_CH_SEL1_OFFSET                  (0xBB0)
#define    HI_BBP_CTU_PMU_CH_SEL0_OFFSET                     (0xBB4)
#define    HI_BBP_CTU_PMU_CH_SEL1_OFFSET                     (0xBB8)
#define    HI_BBP_CTU_VALID_NUM_OFFSET                       (0xBC0)
#define    HI_BBP_CTU_VALID_NUM1_OFFSET                      (0xBC4)
#define    HI_BBP_CTU_DBG_EN_OFFSET                          (0xF80)
#define    HI_BBP_CTU_DBG_ID_OFFSET                          (0xF84)
#define    HI_BBP_CTU_DBG_FLT_OFFSET                         (0xF88)
#define    HI_BBP_CTU_BBC_REV0_OFFSET                        (0xFE0)
#define    HI_BBP_CTU_BBC_REV1_OFFSET                        (0xFE4)
#define    HI_BBP_CTU_BBC_REV2_OFFSET                        (0xFE8)
#define    HI_BBP_CTU_BBC_REV3_OFFSET                        (0xFEC)
#define    HI_BBP_CTU_BBC_REV4_OFFSET                        (0xFF0)

/********************************************************************************/
/*    bbp_ctu �Ĵ������壨��Ŀ��_ģ����_�Ĵ�����_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    c0_primary_mode_ind        : 3; /* [2..0] ͨ��0��Ӧ����ģ������Ϊ��ģ0����ģʽָʾ����ʾLTE��W��Gm��Gs��TDS˭����ģ0��000��LTE ��ģ��001��W��ģ��010��TDS��ģ��011��Gm��ģ��100��Gs��ģ��101~111����Ч������ʱ�̣���ģ����ͨ�ų������� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C0_PRIMARY_MODE_IND_T;    /* ͨ��0��Ӧ��ģ����ģ0����ģʽָʾ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    c0_second_mode_ind         : 3; /* [2..0] ��ģ0��Ӧ�Ĵ�ģ������Ϊ��ģ0����ģʽָʾ����ʾ��ǰLTE/W/Gm/Gs/TDS˭�Ǵ�ģ0��000��LTE��ģ��001��W ��ģ��010��TDS��ģ��011��Gm��ģ��100��Gs��ģ��101~111����Ч������ʱ�̣���ģ����ͨ�ų�������ע��Լ����ģ0�̶���ͨ��0����ģ1�̶���ͨ��1�� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C0_SECOND_MODE_IND_T;    /* ��ģ0��Ӧ�Ĵ�ģ����ģ0����ģʽָʾ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    afc0_value_inter_rat       : 32; /* [31..0] AFC0ֵ����[15:0]:AFC������ֵ[16]:AFC����ָʾ����ʱ�̣�DSP����ģ�л�Ϊ��ģʱ���ã�DSP�Ӵ�ģ�л�Ϊ��ģʱ��ȡ���������ָʾ��Чʱ�̣�������Ч */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_AFC0_VALUE_INTER_RAT_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    c0_afc_pdm_select_mask     : 3; /* [2..0] ͨ��0 c0_afc_pdm_select�����źš�3��b000: c0_afc_pdm_select���������3��b001:ǿ��c0_afc_pdm_select���Ϊ3��b000��3��b010:ǿ��c0_afc_pdm_select���Ϊ3��b001��3��b011: ǿ��c0_afc_pdm_select���Ϊ3��b010��3��b100:ǿ��c0_afc_pdm_select���Ϊ3��b011��3��b101: ǿ��c0_afc_pdm_select���Ϊ3��b100��3'b111:��Ч;����Ϊ�����������á� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C0_AFC_PDM_SELECT_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    c0_ant_switch_pa_select_mask : 3; /* [2..0] ͨ��0 c0_ant_switch_pa_select�����źš�3��b000: c0_ant_switch_pa_select���������3��b001:ǿ��c0_ant_switch_pa_select���Ϊ3��b000��3��b010:ǿ��c0_ant_switch_pa_select���Ϊ3��b001��3��b011: ǿ��c0_ant_switch_pa_select���Ϊ3��b010��3��b100:ǿ��c0_ant_switch_pa_select���Ϊ3��b011��3��b101: ǿ��c0_ant_switch_pa_select���Ϊ3��b100��3'b110~3'b111:��Ч�������á� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C0_ANT_SWITCH_PA_SELECT_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    c0_rfic_select_mask        : 3; /* [2..0] ͨ��0 c0_rfic_select�����źš�3��b000: c0_rfic_select���������3��b001:ǿ��c0_rfic_select���Ϊ3��b000��3��b010:ǿ��c0_rfic_select���Ϊ3��b001��3��b011: ǿ��c0_rfic_select���Ϊ3��b010��3��b100:ǿ��c0_rfic_select���Ϊ3��b011��3��b101: ǿ��c0_rfic_select���Ϊ3��b100��3'b110~3'b111:��Ч�������á� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C0_RFIC_SELECT_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    c0_mipi_select_mask        : 3; /* [2..0] ͨ��0 c0_mipi_select�����źš�3��b000: c0_mipi_select���������3��b001:ǿ��c0_mipi_select���Ϊ3��b000��3��b010:ǿ��c0_mipi_select���Ϊ3��b001��3��b011: ǿ��c0_mipi_select���Ϊ3��b010��3��b100:ǿ��c0_mipi_select���Ϊ3��b011��3��b101: ǿ��c0_mipi_select���Ϊ3��b100��3'b111:��Ч;����Ϊ�����������á� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C0_MIPI_SELECT_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    c0_abb_select_mask         : 3; /* [2..0] ͨ��0 c0_abb_select�����źš�3��b000: c0_abb_select���������3��b001:ǿ��c0_abb_select���Ϊ3��b000��3��b010:ǿ��c0_abb_select���Ϊ3��b001��3��b011: ǿ��c0_abb_select���Ϊ3��b010��3��b100:ǿ��c0_abb_select���Ϊ3��b011��3��b101: ǿ��c0_abb_select���Ϊ3��b100��3'b111:��Ч;����Ϊ�����������á� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C0_ABB_SELECT_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    c0_pmu_select_mask         : 3; /* [2..0] ͨ��0 c0_pmu_select�����źš�3��b000: c0_pmu_select���������3��b001:ǿ��c0_pmu_select���Ϊ3��b000��3��b010:ǿ��c0_pmu_select���Ϊ3��b001��3��b011: ǿ��c0_pmu_select���Ϊ3��b010��3��b100:ǿ��c0_pmu_select���Ϊ3��b011��3��b101: ǿ��c0_pmu_select���Ϊ3��b100��3'b111:��Ч;����Ϊ�����������á� */
        unsigned int    reserved                   : 29; /* [31..3] ������������ */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C0_PMU_SELECT_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_c0_sel_mask            : 3; /* [2..0] ��ǰͨ��0������֮ǰ�������ǿ��ģʽ��ע���������ڸ�����Χ������ABB��RF��PA�ȣ������Ľ���ǿ�䡣�����á� */
        unsigned int    reserved                   : 29; /* [31..3] ������������ */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DSP_C0_SEL_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_rfic0_rstn             : 1; /* [0..0] ������õ�RFIC0��λ�źš� ����Ч��0����λ��1���⸴λ�� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DSP_RFIC0_RSTN_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    c0_second_afc_mode         : 1; /* [0..0] 0:ͨ��0��ģʹ��ͨ��0��ģafc������1:ͨ��0��ģʹ��ͨ��0��ģafc������ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C0_SECOND_AFC_MODE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    bbp_sys_0control           : 32; /* [31..0] ����bbp_sys_0control��GU���漰�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_BBP_SYS_0CONTROL_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    mipi0_rd_overtime_flag_dsp : 1; /* [0..0] DSP�ȴ��ض������ʱ��־λ1:��ʱ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_MIPI0_RD_OVERTIME_FLAG_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    ssi0_rd_overtime_flag_dsp  : 1; /* [0..0] DSP�ȴ��ض������ʱ��־λ1:��ʱ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_SSI0_RD_OVERTIME_FLAG_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    mipi0_rd_overtime_clr_imi  : 1; /* [0..0] DSP�ȴ��ض������ʱ��,�峭ʱ��־λ���� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_MIPI0_RD_OVERTIME_CLR_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    ssi0_rd_overtime_clr_imi   : 1; /* [0..0] DSP�ȴ��ض������ʱ��,�峭ʱ��־λ���� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_SSI0_RD_OVERTIME_CLR_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic0_1          : 32; /* [31..0] xbbp��SSI0 master�����ض���ָ��ʱ,DSP��ȡSSI�ض����,�ض���������ڴ�16�Ĵ���λ����dsp����,�ض������Ƿ���Ч�ο���־λ */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC0_1_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic0_2          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC0_2_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic0_3          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC0_3_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic0_4          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC0_4_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic0_5          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC0_5_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic0_6          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC0_6_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic0_7          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC0_7_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic0_8          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC0_8_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic0_9          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC0_9_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic0_10         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC0_10_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic0_11         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC0_11_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic0_12         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC0_12_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic0_13         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC0_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic0_14         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC0_14_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic0_15         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC0_15_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic0_16         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC0_16_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_flag_rfic0       : 1; /* [0..0] ���лض�ָ��ִ�����,����ȡ�߻ض�����1:���лض�ָ������,DSP���԰�λ�ö�ȡSSI�ض���� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_FLAG_RFIC0_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_flag_rfic0_clr   : 1; /* [0..0] 1:DSPȡ�����лض����ݺ�,д��clr�ź����־λ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_FLAG_RFIC0_CLR_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi0_1          : 32; /* [31..0] xbbp��MIPI0 master�����ض���ָ��ʱ,DSP��ȡSSI�ض����,�ض���������ڴ�16�Ĵ���λ����dsp����,�ض������Ƿ���Ч�ο���־λ */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI0_1_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi0_2          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI0_2_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi0_3          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI0_3_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi0_4          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI0_4_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi0_5          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI0_5_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi0_6          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI0_6_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi0_7          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI0_7_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi0_8          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI0_8_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi0_9          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI0_9_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi0_10         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI0_10_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi0_11         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI0_11_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi0_12         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI0_12_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi0_13         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI0_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi0_14         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI0_14_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi0_15         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI0_15_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi0_16         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI0_16_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_flag_mipi0       : 1; /* [0..0] ���лض�ָ��ִ�����,����ȡ�߻ض�����1:���лض�ָ������,DSP���԰�λ�ö�ȡSSI�ض����,�����������ֱ�ӿ���MIPI��־λ��ͬ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_FLAG_MIPI0_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_flag_mipi0_clr   : 1; /* [0..0] 1:DSPȡ�����лض����ݺ�,д��clr�ź����־λ,�����������ֱ�ӿ���MIPI��־λ��ͬ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_FLAG_MIPI0_CLR_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    mipi0_rd_data_low_soft     : 32; /* [31..0] �����ֱ�ӿ���MIPI0 master,������xbbp���ͻض�ָ��ʱ,�ض�ָ���ŵ�32bit. */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_MIPI0_RD_DATA_LOW_SOFT_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    mipi0_rd_data_high_soft    : 32; /* [31..0] �����ֱ�ӿ���MIPI0 master,������xbbp���ͻض�ָ��ʱ,�ض�ָ���Ÿ�32bit. */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_MIPI0_RD_DATA_HIGH_SOFT_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_end_flag_mipi0_soft     : 1; /* [0..0] �����ֱ�ӿ���MIPI0 master,������xbbp���ͻض�ָ��ʱ,�ض�������Чָʾ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_END_FLAG_MIPI0_SOFT_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    mipi0_grant_dsp            : 1; /* [0..0] �����ֱ�ӿ���MIPI0 master1:������Է���CFG_IND����mipi00:����ȴ�mipi0��Ȩʹ�� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_MIPI0_GRANT_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_mipi0_wdata_low        : 32; /* [31..0] �����ֱ�ӿ���MIPI0 master,������xbbp����дָ��ʱ,дָ���32bit */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DSP_MIPI0_WDATA_LOW_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_mipi0_wdata_high       : 32; /* [31..0] �����ֱ�ӿ���MIPI0 master,������xbbp����дָ��ʱ,дָ���32bit */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DSP_MIPI0_WDATA_HIGH_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_mipi0_en_imi           : 1; /* [0..0] �����ֱ�ӿ���MIPI0 master,���������ߴ�ָʾ,����xbbp��MIPI0�Ŀ��� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DSP_MIPI0_EN_IMI_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_mipi0_cfg_ind_imi      : 1; /* [0..0] �����ֱ�ӿ���MIPI0 master,���ߴ�dsp_mipi0_en��,��������������� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DSP_MIPI0_CFG_IND_IMI_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_mipi0_rd_clr           : 1; /* [0..0] �����ֱ�ӿ���MIPI0 master,ȡ�߻ض����ݺ�,���־λ���� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DSP_MIPI0_RD_CLR_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    abb0_line_control_cmd_sel  : 1; /* [0..0] ABB0�߿�ǿ��ģʽ��1����ʾ���ǿ�䣬��ʱ��������ǿ�����Ч��0����ʾ��CTU���ݸ�BBP������Ĭ��ֵΪ0�������á� */
        unsigned int    abb0_mode_sel_cmd          : 3; /* [3..1] ���ǿ��ģʽabb0_mode_sel. */
        unsigned int    abb0_tx_en_cmd             : 1; /* [4..4] ���ǿ��ģʽabb0_tx_en. */
        unsigned int    abb0_rxa_en_cmd            : 1; /* [5..5] ���ǿ��ģʽabb0_rxa_en. */
        unsigned int    abb0_rxb_en_cmd            : 1; /* [6..6] ���ǿ��ģʽabb0_rxb_en. */
        unsigned int    abb0_blka_en_cmd           : 1; /* [7..7] ���ǿ��ģʽabb0_rxa_blk_en. */
        unsigned int    abb0_blkb_en_cmd           : 1; /* [8..8] ���ǿ��ģʽabb0_rxb_blk_en. */
        unsigned int    reserved                   : 23; /* [31..9] ��������������Ϊ�����ã�Ĭ��ֵΪ0. */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_ABB0_LINE_CONTROL_CMD_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    ch0_bbp_sel                : 3; /* [2..0] V9R1�汾����������á� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CH0_BBP_SEL_T;

typedef union
{
    struct
    {
        unsigned int    cpu_mipi0_func_sel         : 1; /* [0..0] cpu����mipi����,����Ϊ1������������, ����ʱ����Ϊ0 */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CPU_MIPI0_FUNC_SEL_T;

typedef union
{
    struct
    {
        unsigned int    cpu_mipi0_test_func        : 1; /* [0..0] cpu����mipi����,������ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CPU_MIPI0_TEST_FUNC_T;

typedef union
{
    struct
    {
        unsigned int    cpu_mipi0_sclk_test        : 1; /* [0..0] cpu����mipi����, ������ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CPU_MIPI0_SCLK_TEST_T;

typedef union
{
    struct
    {
        unsigned int    cpu_mipi0_sdata_test       : 1; /* [0..0] cpu����mipi����, ������ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CPU_MIPI0_SDATA_TEST_T;

typedef union
{
    struct
    {
        unsigned int    cpu_mipi0_clr_ind_imi      : 1; /* [0..0] cpu����mipi����,mipi0master���쳣�ź� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CPU_MIPI0_CLR_IND_IMI_T;

typedef union
{
    struct
    {
        unsigned int    cpu_mipi0_fifo_clr_imi     : 1; /* [0..0] mipi0�洢����������ͬ��FIFO��λ�ź� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CPU_MIPI0_FIFO_CLR_IMI_T;

typedef union
{
    struct
    {
        unsigned int    cpu_ssi0_fifo_clr_imi      : 1; /* [0..0] ssi0�洢����������ͬ��FIFO��λ�ź� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CPU_SSI0_FIFO_CLR_IMI_T;

typedef union
{
    struct
    {
        unsigned int    cpu_rx0_lte_ckg_bypass     : 1; /* [0..0]  */
        unsigned int    cpu_rx0_tds_ckg_bypass     : 1; /* [1..1]  */
        unsigned int    cpu_rx0_w_ckg_bypass       : 1; /* [2..2]  */
        unsigned int    cpu_rx0_g1_ckg_bypass      : 1; /* [3..3]  */
        unsigned int    cpu_rx0_g2_ckg_bypass      : 1; /* [4..4]  */
        unsigned int    cpu_tx0_lte_ckg_bypass     : 1; /* [5..5]  */
        unsigned int    cpu_tx0_tds_ckg_bypass     : 1; /* [6..6]  */
        unsigned int    cpu_tx0_w_ckg_bypass       : 1; /* [7..7]  */
        unsigned int    cpu_tx0_g1_ckg_bypass      : 1; /* [8..8]  */
        unsigned int    cpu_tx0_g2_ckg_bypass      : 1; /* [9..9]  */
        unsigned int    reserved                   : 22; /* [31..10] ͨ��0������ʱ���ſ�bypass,���������� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_T;

typedef union
{
    struct
    {
        unsigned int    c1_primary_mode_ind        : 3; /* [2..0] ͨ��1��Ӧ����ģ������Ϊ��ģ1����ģʽָʾ����ʾLTE��W��Gm��Gs��TDS˭����ģ1��000��LTE ��ģ��001��W��ģ��010��TDS��ģ��011��Gm��ģ��100��Gs��ģ��101~111����Ч������ʱ�̣���ģ����ͨ�ų������� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C1_PRIMARY_MODE_IND_T;    /* ͨ��1��Ӧ��ģ����ģ1����ģʽָʾ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    c1_second_mode_ind         : 3; /* [2..0] ��ģ1��Ӧ�Ĵ�ģ������Ϊ��ģ1����ģʽָʾ����ʾLTE/W/Gm/Gs/TDS˭�Ǵ�ģ1��000��LTE��ģ��001��W ��ģ��010��TDS��ģ��011��Gm��ģ��100��Gs��ģ��101~111����Ч������ʱ�̣���ģ����ͨ�ų�������.ע��Լ����ģ0�̶���ͨ��0����ģ1�̶���ͨ��1�� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C1_SECOND_MODE_IND_T;    /* ��ģ1��Ӧ�Ĵ�ģ����ģ1����ģʽָʾ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    c1_ch_num_ind              : 1; /* [0..0] ��ģ1��Ӧ��ͨ��ѡ���źš�0��ѡ��ͨ��0��1��ѡ��ͨ��1������ʱ�̣���ģ����ͨ�ų������� */
        unsigned int    c1_ch_num_ind_sel          : 1; /* [1..1] ��ģ1��Ӧ��ͨ��ѡ���ź���Ч��ʽ��0����int1��Ч��1��������Ч�� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C1_CH_NUM_IND_T;    /* ��ģ1��Ӧͨ����ָʾ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    afc1_value_inter_rat       : 32; /* [31..0] AFC1ֵ����[15:0]:AFC������ֵ[16]:AFC����ָʾ����ʱ�̣�DSP����ģ�л�Ϊ��ģʱ���ã�DSP�Ӵ�ģ�л�Ϊ��ģʱ��ȡ���������ָʾ��Чʱ�̣�������Ч */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_AFC1_VALUE_INTER_RAT_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    c1_afc_pdm_select_mask     : 3; /* [2..0] ͨ��1 c1_afc_pdm_select�����źš�3��b000: c1_afc_pdm_select���������3��b001:ǿ��c1_afc_pdm_select���Ϊ3��b000��3��b010:ǿ��c1_afc_pdm_select���Ϊ3��b001��3��b011: ǿ��c1_afc_pdm_select���Ϊ3��b010��3��b100:ǿ��c1_afc_pdm_select���Ϊ3��b011��3��b101: ǿ��c1_afc_pdm_select���Ϊ3��b100��3'b110~3'b111:��Ч�������á� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C1_AFC_PDM_SELECT_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    c1_ant_switch_pa_select_mask : 3; /* [2..0] ͨ��1 c1_ant_switch_pa_select�����źš�3��b000: c1_ant_switch_pa_select���������3��b001:ǿ��c1_ant_switch_pa_select���Ϊ3��b000��3��b010:ǿ��c1_ant_switch_pa_select���Ϊ3��b001��3��b011: ǿ��c1_ant_switch_pa_select���Ϊ3��b010��3��b100:ǿ��c1_ant_switch_pa_select���Ϊ3��b011��3��b101: ǿ��c1_ant_switch_pa_select���Ϊ3��b100��3'b110~3'b111:��Ч�������á� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C1_ANT_SWITCH_PA_SELECT_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    c1_rfic_select_mask        : 3; /* [2..0] ͨ��1 c1_rfic_select�����źš�3��b000: c1_rfic_select���������3��b001:ǿ��c1_rfic_select���Ϊ3��b000��3��b010:ǿ��c1_rfic_select���Ϊ3��b001��3��b011: ǿ��c1_rfic_select���Ϊ3��b010��3��b100:ǿ��c1_rfic_select���Ϊ3��b011��3��b101: ǿ��c1_rfic_select���Ϊ3��b100��3'b110~3'b111:��Ч�������á� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C1_RFIC_SELECT_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    c1_mipi_select_mask        : 3; /* [2..0] ͨ��0 c1_mipi_select�����źš�3��b000: c1_mipi_select���������3��b001:ǿ��c1_mipi_select���Ϊ3��b000��3��b010:ǿ��c1_mipi_select���Ϊ3��b001��3��b011: ǿ��c1_mipi_select���Ϊ3��b010��3��b100:ǿ��c1_mipi_select���Ϊ3��b011��3��b101: ǿ��c1_mipi_select���Ϊ3��b100��3'b111:��Ч;����Ϊ�����������á� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C1_MIPI_SELECT_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    c1_abb_select_mask         : 3; /* [2..0] ͨ��0 c1_abb_select�����źš�3��b000: c1_abb_select���������3��b001:ǿ��c1_abb_select���Ϊ3��b000��3��b010:ǿ��c1_abb_select���Ϊ3��b001��3��b011: ǿ��c1_abb_select���Ϊ3��b010��3��b100:ǿ��c1_abb_select���Ϊ3��b011��3��b101: ǿ��c1_abb_select���Ϊ3��b100��3'b111:��Ч;����Ϊ�����������á� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C1_ABB_SELECT_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    c1_pmu_select_mask         : 3; /* [2..0] ͨ��0 c1_pmu_select�����źš�3��b000: c1_pmu_select���������3��b001:ǿ��c1_pmu_select���Ϊ3��b000��3��b010:ǿ��c1_pmu_select���Ϊ3��b001��3��b011: ǿ��c1_pmu_select���Ϊ3��b010��3��b100:ǿ��c1_pmu_select���Ϊ3��b011��3��b101: ǿ��c1_pmu_select���Ϊ3��b100��3'b111:��Ч;����Ϊ�����������á� */
        unsigned int    reserved                   : 29; /* [31..3] ������������ */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C1_PMU_SELECT_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_c1_sel_mask            : 3; /* [2..0] ��ǰͨ��1������֮ǰ�� �����ǿ��ģʽ��ע���������ڸ�����Χ������ABB��RF��PA�ȣ������Ľ���ǿ�䡣�����á� */
        unsigned int    reserved                   : 29; /* [31..3] ������������ */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DSP_C1_SEL_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_rfic1_rstn             : 1; /* [0..0] ������õ�RFIC1��λ�źš� ����Ч�� 0����λ��1���⸴λ�� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DSP_RFIC1_RSTN_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    c1_second_afc_mode         : 1; /* [0..0] 0:ͨ��1��ģʹ��ͨ��1��ģafc������1:ͨ��1��ģʹ��ͨ��1��ģafc������ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_C1_SECOND_AFC_MODE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    bbp_sys_1control           : 32; /* [31..0] ����bbp_sys_1controlGU���漰�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_BBP_SYS_1CONTROL_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    mipi1_rd_overtime_flag_dsp : 1; /* [0..0] DSP�ȴ��ض������ʱ��־λ1:��ʱ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_MIPI1_RD_OVERTIME_FLAG_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    ssi1_rd_overtime_flag_dsp  : 1; /* [0..0] DSP�ȴ��ض������ʱ��־λ1:��ʱ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_SSI1_RD_OVERTIME_FLAG_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    mipi1_rd_overtime_clr_imi  : 1; /* [0..0] DSP�ȴ��ض������ʱ��,�峭ʱ��־λ���� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_MIPI1_RD_OVERTIME_CLR_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    ssi1_rd_overtime_clr_imi   : 1; /* [0..0] DSP�ȴ��ض������ʱ��,�峭ʱ��־λ���� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_SSI1_RD_OVERTIME_CLR_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic1_1          : 32; /* [31..0] xbbp��SSI1 master�����ض���ָ��ʱ,DSP��ȡSSI�ض����,�ض���������ڴ�16�Ĵ���λ����dsp����,�ض������Ƿ���Ч�ο���־λ */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC1_1_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic1_2          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC1_2_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic1_3          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC1_3_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic1_4          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC1_4_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic1_5          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC1_5_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic1_6          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC1_6_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic1_7          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC1_7_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic1_8          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC1_8_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic1_9          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC1_9_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic1_10         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC1_10_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic1_11         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC1_11_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic1_12         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC1_12_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic1_13         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC1_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic1_14         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC1_14_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic1_15         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC1_15_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_rfic1_16         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_RFIC1_16_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_flag_rfic1       : 1; /* [0..0] ���лض�ָ��ִ�����,����ȡ�߻ض�����1:���лض�ָ������,DSP���԰�λ�ö�ȡSSI�ض���� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_FLAG_RFIC1_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_flag_rfic1_clr   : 1; /* [0..0] 1:DSPȡ�����лض����ݺ�,д��clr�ź����־λ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_FLAG_RFIC1_CLR_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi1_1          : 32; /* [31..0] xbbp��MIPI1 master�����ض���ָ��ʱ,DSP��ȡSSI�ض����,�ض���������ڴ�16�Ĵ���λ����dsp���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI1_1_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi1_2          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI1_2_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi1_3          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI1_3_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi1_4          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI1_4_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi1_5          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI1_5_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi1_6          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI1_6_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi1_7          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI1_7_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi1_8          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI1_8_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi1_9          : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI1_9_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi1_10         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI1_10_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi1_11         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI1_11_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi1_12         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI1_12_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi1_13         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI1_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi1_14         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI1_14_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi1_15         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI1_15_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_mipi1_16         : 32; /* [31..0] ͬ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_MIPI1_16_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_flag_mipi1       : 1; /* [0..0] ���лض�ָ��ִ�����,����ȡ�߻ض�����1:���лض�ָ������,DSP���԰�λ�ö�ȡSSI�ض����,�����������ֱ�ӿ���MIPI��־λ��ͬ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_FLAG_MIPI1_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_result_flag_mipi1_clr   : 1; /* [0..0] 1:DSPȡ�����лض����ݺ�,д��clr�ź����־λ,�����������ֱ�ӿ���MIPI��־λ��ͬ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_RESULT_FLAG_MIPI1_CLR_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    mipi1_rd_data_low_soft     : 32; /* [31..0] �����ֱ�ӿ���MIPI1 master,������xbbp���ͻض�ָ��ʱ,�ض�ָ���ŵ�32bit. */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_MIPI1_RD_DATA_LOW_SOFT_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    mipi1_rd_data_high_soft    : 32; /* [31..0] �����ֱ�ӿ���MIPI1 master,������xbbp���ͻض�ָ��ʱ,�ض�ָ���Ÿ�32bit. */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_MIPI1_RD_DATA_HIGH_SOFT_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    rd_end_flag_mipi1_soft     : 1; /* [0..0] �����ֱ�ӿ���MIPI1 master,������xbbp���ͻض�ָ��ʱ,�ض�������Чָʾ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RD_END_FLAG_MIPI1_SOFT_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    mipi1_grant_dsp            : 1; /* [0..0] �����ֱ�ӿ���MIPI1 master1:������Է���CFG_IND����mipi10:����ȴ�mipi1��Ȩʹ�� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_MIPI1_GRANT_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_mipi1_wdata_low        : 32; /* [31..0] �����ֱ�ӿ���MIPI1 master,������xbbp����дָ��ʱ,дָ���32bit */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DSP_MIPI1_WDATA_LOW_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_mipi1_wdata_high       : 32; /* [31..0] �����ֱ�ӿ���MIPI1 master,������xbbp����дָ��ʱ,дָ���32bit */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DSP_MIPI1_WDATA_HIGH_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_mipi1_en_imi           : 1; /* [0..0] �����ֱ�ӿ���MIPI1 master,���������ߴ�ָʾ,����xbbp��MIPI0�Ŀ��� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DSP_MIPI1_EN_IMI_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_mipi1_cfg_ind_imi      : 1; /* [0..0] �����ֱ�ӿ���MIPI1 master,���ߴ�dsp_mipi1_en��,��������������� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DSP_MIPI1_CFG_IND_IMI_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_mipi1_rd_clr           : 1; /* [0..0] �����ֱ�ӿ���MIPI1 master,ȡ�߻ض����ݺ�,���־λ���� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DSP_MIPI1_RD_CLR_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    abb1_line_control_cmd_sel  : 1; /* [0..0] ABB1�߿�ǿ��ģʽ��1����ʾ���ǿ�䣬��ʱ��������ǿ�����Ч��0����ʾ��CTU���ݸ�BBP������Ĭ��ֵΪ0�������á� */
        unsigned int    abb1_mode_sel_cmd          : 3; /* [3..1] ���ǿ��ģʽabb1_mode_sel. */
        unsigned int    abb1_tx_en_cmd             : 1; /* [4..4] ���ǿ��ģʽabb1_tx_en. */
        unsigned int    abb1_rxa_en_cmd            : 1; /* [5..5] ���ǿ��ģʽabb1_rxa_en. */
        unsigned int    abb1_rxb_en_cmd            : 1; /* [6..6] ���ǿ��ģʽabb1_rxb_en. */
        unsigned int    abb1_blka_en_cmd           : 1; /* [7..7] ���ǿ��ģʽabb1_rxa_blk_en. */
        unsigned int    abb1_blkb_en_cmd           : 1; /* [8..8] ���ǿ��ģʽabb1_rxb_blk_en. */
        unsigned int    reserved                   : 23; /* [31..9] ��������������Ϊ�����ã�Ĭ��ֵΪ0. */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_ABB1_LINE_CONTROL_CMD_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    ch1_bbp_sel                : 3; /* [2..0] V9R1�汾����������á� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CH1_BBP_SEL_T;

typedef union
{
    struct
    {
        unsigned int    cpu_mipi1_func_sel         : 1; /* [0..0] cpu����mipi����,����Ϊ1������������, ����ʱ����Ϊ0 */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CPU_MIPI1_FUNC_SEL_T;

typedef union
{
    struct
    {
        unsigned int    cpu_mipi1_test_func        : 1; /* [0..0] cpu����mipi����,������ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CPU_MIPI1_TEST_FUNC_T;

typedef union
{
    struct
    {
        unsigned int    cpu_mipi1_sclk_test        : 1; /* [0..0] cpu����mipi����,������ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CPU_MIPI1_SCLK_TEST_T;

typedef union
{
    struct
    {
        unsigned int    cpu_mipi1_sdata_test       : 1; /* [0..0] cpu����mipi����,������ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CPU_MIPI1_SDATA_TEST_T;

typedef union
{
    struct
    {
        unsigned int    cpu_mipi1_clr_ind_imi      : 1; /* [0..0] cpu����mipi����,mipi1master���쳣�ź� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CPU_MIPI1_CLR_IND_IMI_T;

typedef union
{
    struct
    {
        unsigned int    cpu_mipi1_fifo_clr_imi     : 1; /* [0..0] mipi1�洢����������ͬ��FIFO��λ�ź� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CPU_MIPI1_FIFO_CLR_IMI_T;

typedef union
{
    struct
    {
        unsigned int    cpu_ssi1_fifo_clr_imi      : 1; /* [0..0] ssi1�洢����������ͬ��FIFO��λ�ź� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CPU_SSI1_FIFO_CLR_IMI_T;

typedef union
{
    struct
    {
        unsigned int    cpu_rx1_lte_ckg_bypass     : 1; /* [0..0]  */
        unsigned int    cpu_rx1_tds_ckg_bypass     : 1; /* [1..1]  */
        unsigned int    cpu_rx1_w_ckg_bypass       : 1; /* [2..2]  */
        unsigned int    cpu_rx1_g1_ckg_bypass      : 1; /* [3..3]  */
        unsigned int    cpu_rx1_g2_ckg_bypass      : 1; /* [4..4]  */
        unsigned int    cpu_tx1_lte_ckg_bypass     : 1; /* [5..5]  */
        unsigned int    cpu_tx1_tds_ckg_bypass     : 1; /* [6..6]  */
        unsigned int    cpu_tx1_w_ckg_bypass       : 1; /* [7..7]  */
        unsigned int    cpu_tx1_g1_ckg_bypass      : 1; /* [8..8]  */
        unsigned int    cpu_tx1_g2_ckg_bypass      : 1; /* [9..9]  */
        unsigned int    reserved                   : 22; /* [31..10] ͨ��1������ʱ���ſ�bypass,���������� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_T;

typedef union
{
    struct
    {
        unsigned int    delay_cnt                  : 5; /* [4..0] CTU��⵽timing_get_ind��̶��ӳ�delay_cnt��CTUʱ�����ں�����timing_valid�źš�����ʱ�̣��ϵ��ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 27; /* [31..5] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DELAY_CNT_T;    /* ��ʱ�����ӳټĴ��� */

typedef union
{
    struct
    {
        unsigned int    int_wait_cnt               : 16; /* [15..0] CTU�ж��ϱ��ȴ�ʱ�䣬��λΪһ��CLK���ڡ�DSP����жϱ�־�Ĵ�����ĳ������bit��CTU�ϱ��жϵ�ƽ��Ҫ���ͣ������ʱ�жϱ�־�Ĵ������������ж�������Ч������ȴ�int_wait_cnt��ʱ�����ں�������CTU�ϱ��жϡ�����ʱ�̣��ϵ��ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_WAIT_CNT_T;    /* �ж��ӳټĴ��� */

typedef union
{
    struct
    {
        unsigned int    ltebbp_input_mask          : 2; /* [1..0] ��ƽ�źţ�����Ч����LTE��ģ��Ҫ����ϵͳ����ʱ�����ñ��Ĵ���Ϊ3����ϵͳ�������������ñ��Ĵ���Ϊ0��bit1:1��b0: lte_intrasys_valid������Ч����ǿ��Ϊ1��bit0:1��b0:lte_intersys_ingap_pre_ind������Ч����ǿ��Ϊ0��bit[1:0] = 11b: ��lte_intrasys_valid��lte_intersys_ingap_pre_ind������Ч������ʱ�̣�LTE��ģ��Ҫ����ϵͳ����ʱ�����ñ��Ĵ���Ϊ3����ϵͳ�������������ñ��Ĵ���Ϊ0����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_LTEBBP_INPUT_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    wbbp_input_mask            : 2; /* [1..0] ��ƽ�źţ�����Ч����W��ģ��Ҫ����ϵͳ����ʱ�����ñ��Ĵ���Ϊ3����ϵͳ�������������ñ��Ĵ���Ϊ0��bit1:1��b0: ��w_intrasys_valid������Ч����ǿ��Ϊ1��bit0:1��b0:w_intersys_ingap_pre_ind������Ч����ǿ��Ϊ0��bit[1:0] = 11b: ��w_intrasys_valid��w_intersys_ingap_pre_ind������Ч������ʱ�̣�W��ģ��Ҫ����ϵͳ����ʱ�����ñ��Ĵ���Ϊ3����ϵͳ�������������ñ��Ĵ���Ϊ0����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_WBBP_INPUT_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    gmbbp_input_mask           : 2; /* [1..0] ��ƽ�źţ�����Ч����Gm��ģ��Ҫ����ϵͳ����ʱ�����ñ��Ĵ���Ϊ3����ϵͳ�������������ñ��Ĵ���Ϊ0��bit1:1��b0: ��gm_intrasys_valid������Ч����ǿ��Ϊ1��bit0:1��b0:gm_intersys_ingap_pre_ind������Ч����ǿ��Ϊ0��bit[1:0] = 11b: ��gm_intrasys_valid��gm_intersys_ingap_pre_ind������Ч������ʱ�̣�Gm��ģ��Ҫ����ϵͳ����ʱ�����ñ��Ĵ���Ϊ3����ϵͳ�������������ñ��Ĵ���Ϊ0����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GMBBP_INPUT_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    gsbbp_input_mask           : 2; /* [1..0] ��ƽ�źţ�����Ч����Gs��ģ��Ҫ����ϵͳ����ʱ�����ñ��Ĵ���Ϊ3����ϵͳ�������������ñ��Ĵ���Ϊ0��bit1:1��b0: ��gs_intrasys_valid������Ч����ǿ��Ϊ1��bit0:1��b0:gs_intersys_ingap_pre_ind������Ч����ǿ��Ϊ0��bit[1:0] = 11b: ��gs_intrasys_valid��gs_intersys_ingap_pre_ind������Ч������ʱ�̣�Gs��ģ��Ҫ����ϵͳ����ʱ�����ñ��Ĵ���Ϊ3����ϵͳ�������������ñ��Ĵ���Ϊ0����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GSBBP_INPUT_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    tdsbbp_input_mask          : 2; /* [1..0] ��ƽ�źţ�����Ч����TDS��ģ��Ҫ����ϵͳ����ʱ�����ñ��Ĵ���Ϊ3����ϵͳ�������������ñ��Ĵ���Ϊ0��bit1:1��b0: ��tds_intrasys_valid������Ч����ǿ��Ϊ1��bit0:1��b0:tds_intersys_ingap_pre_ind������Ч����ǿ��Ϊ0��bit[1:0] = 11b: ��tds_intrasys_valid��tds_intersys_ingap_pre_ind������Ч������ʱ�̣�TDS��ģ��Ҫ����ϵͳ����ʱ�����ñ��Ĵ���Ϊ3����ϵͳ�������������ñ��Ĵ���Ϊ0����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TDSBBP_INPUT_MASK_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    lte_mux_frame_num          : 32; /* [31..0] �����ṩLTE�ĸ�֡�ţ���LDSP���¡�ȡֵ��Χ��0��1223������ʱ�̣���Чʱ�̣�������Ч */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_LTE_MUX_FRAME_NUM_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    tds_mux_frame_num          : 32; /* [31..0] �����ṩTDS�ĸ�֡�ţ���TDSP���¡�ȡֵ��Χ��0��1223������ʱ�̣���Чʱ�̣�������Ч */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TDS_MUX_FRAME_NUM_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    w_mux_frame_num            : 32; /* [31..0] �����ṩw�ĸ�֡�ţ���wDSP���¡�����ʱ�̣���Чʱ�̣�������Ч */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_W_MUX_FRAME_NUM_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    meas_flag_l_rd             : 32; /* [31..0] W/TD/GM/GSΪ��ģʱд�˼Ĵ�����LΪ��ģʱ��ȡ�˼Ĵ������˼Ĵ�����DSP֮����ж�д��BBP���ṩ�ӿڡ� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_MEAS_FLAG_L_RD_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    meas_flag_w_rd             : 32; /* [31..0] L/TD/GM/GSΪ��ģʱд�˼Ĵ�����WΪ��ģʱ��ȡ�˼Ĵ������˼Ĵ�����DSP֮����ж�д��BBP���ṩ�ӿڡ� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_MEAS_FLAG_W_RD_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    meas_flag_t_rd             : 32; /* [31..0] W/L/GM/GSΪ��ģʱд�˼Ĵ�����TDΪ��ģʱ��ȡ�˼Ĵ������˼Ĵ�����DSP֮����ж�д��BBP���ṩ�ӿڡ� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_MEAS_FLAG_T_RD_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    meas_flag_gm_rd            : 32; /* [31..0] W/TD/L/GSΪ��ģʱд�˼Ĵ�����GMΪ��ģʱ��ȡ�˼Ĵ������˼Ĵ�����DSP֮����ж�д��BBP���ṩ�ӿڡ� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_MEAS_FLAG_GM_RD_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    meas_flag_gs_rd            : 32; /* [31..0] W/TD/GM/LΪ��ģʱд�˼Ĵ�����GSΪ��ģʱ��ȡ�˼Ĵ������˼Ĵ�����DSP֮����ж�д��BBP���ṩ�ӿڡ� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_MEAS_FLAG_GS_RD_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    wake_flag_l_wr             : 32; /* [31..0] LΪ��ģʱд�˼Ĵ�����W/TD/GM/GS��ģ��ȡ�˼Ĵ������˼Ĵ�����DSP֮����ж�д��BBP���ṩ�ӿڡ� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_WAKE_FLAG_L_WR_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    wake_flag_w_wr             : 32; /* [31..0] WΪ��ģʱд�˼Ĵ�����L/TD/GM/GS��ģ��ȡ�˼Ĵ������˼Ĵ�����DSP֮����ж�д��BBP���ṩ�ӿڡ� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_WAKE_FLAG_W_WR_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    wake_flag_t_wr             : 32; /* [31..0] TDΪ��ģʱд�˼Ĵ�����W/L/GM/GS��ģ��ȡ�˼Ĵ������˼Ĵ�����DSP֮����ж�д��BBP���ṩ�ӿڡ� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_WAKE_FLAG_T_WR_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    wake_flag_gm_wr            : 32; /* [31..0] GMΪ��ģʱд�˼Ĵ�����W/TD/L/GS��ģ��ȡ�˼Ĵ������˼Ĵ�����DSP֮����ж�д��BBP���ṩ�ӿڡ� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_WAKE_FLAG_GM_WR_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    wake_flag_gs_wr            : 32; /* [31..0] GSΪ��ģʱд�˼Ĵ�����W/TD/GM/L��ģ��ȡ�˼Ĵ������˼Ĵ�����DSP֮����ж�д��BBP���ṩ�ӿڡ� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_WAKE_FLAG_GS_WR_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    timing_get_ind             : 1; /* [0..0] �߼������㡣DSP��Ҫ��ȡ��ʱ��ϵʱ������Ч��CTU�߼����ݴ��ź�����LTE/W/Gm/Gs/TDS��ϵͳ��ʱ��CTU���ڲ���ʱ��DSP��ѯ������ʱ�̣������Ҫ���涨ʱ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TIMING_GET_IND_T;    /* ��ʱ����ʹ��ָʾ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    timing_clear               : 1; /* [0..0] �߼������㡣��ʱ��־������������timing_valid��־������ʱ�̣�DSP��ȡ��ʱ��Ϻ�������Чʱ�̣�������Ч */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TIMING_CLEAR_T;    /* ��ʱ��־����Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_timing_valid             : 1; /* [0..0] ��ʱ�ϱ���Ч��־������Ч��DSP����timing_get_ind�󣬲�ѯ�˱�־�����Ϊ�ߣ�����Ի�ȡLTE��W��Gm��Gs, TDS��ϵͳ��ʱ��DSP��ȡ��ʱ��Ϻ�����timing_clear�Ĵ�����timing_valid����Ϊ��Ч��CTU��⵽timing_get_ind��̶��ӳ�delay_cnt������timing_valid������ʱ�̣���Чʱ�̣�������Ч */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TIMING_VALID_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    w_clk_cnt_dsp              : 4; /* [3..0] [3:0]Ϊchip��ʱ�Ӽ�������Χ0~15 */
        unsigned int    w_chip_cnt_dsp             : 12; /* [15..4] [15:4]Ϊʱ϶��chip��������Χ0~2559 */
        unsigned int    w_slot_cnt_dsp             : 4; /* [19..16] [19:16]Ϊʱ϶��������Χ0~14 */
        unsigned int    w_sfn_dsp                  : 12; /* [31..20] W��ϵͳ��ʱ��Ϣ����DSP ����timing_get_ind�����ϱ����ϱ�ʱ�̣�timing_validΪ��ʱ�ϱ���Ч.[31:20]��ʾW��SFN������Դ��w_timing_b[11:0]������Χ0~4095�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_W_TIMING_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_gm_fn_dsp                : 22; /* [21..0] GM��FN������Դ��gm_timing_a[21:0]������DSP ����timing_get_ind�����ϱ�������[10:0]��ʾfn_low,��Χ0~1325��[21:11]��ʾfn_hign,��Χ0~2047���ϱ�ʱ�̣�timing_validΪ��ʱ�ϱ���Ч */
        unsigned int    reserved                   : 10; /* [31..22] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GM_FN_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_gm_qb_dsp                : 13; /* [12..0] GM��ϵͳ��ʱQb��Ϣ����DSP ����timing_get_ind�����ϱ��� [12:0]��ʾ Gϵͳ��ʱ֡��Qb������Դ��gm_timing_b[12:0]������Χ0~4999���ϱ�ʱ�̣�timing_validΪ��ʱ�ϱ���Ч */
        unsigned int    reserved                   : 19; /* [31..13] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GM_QB_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    gs_fn_dsp                  : 22; /* [21..0] GS��FN������Դ��gs_timing_a[21:0]������DSP ����timing_get_ind�����ϱ�������[10:0]��ʾfn_low,��Χ0~1325��[21:11]��ʾfn_hign,��Χ0~2047���ϱ�ʱ�̣�timing_validΪ��ʱ�ϱ���Ч */
        unsigned int    reserved                   : 10; /* [31..22] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GS_FN_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_gs_qb_dsp                : 13; /* [12..0] GS��ϵͳ��ʱQb��Ϣ����DSP ����timing_get_ind�����ϱ��� [12:0]��ʾ GSϵͳ��ʱ֡��Qb������Դ��gs_timing_b[12:0]������Χ0~4999���ϱ�ʱ�̣�timing_validΪ��ʱ�ϱ���Ч */
        unsigned int    reserved                   : 19; /* [31..13] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GS_QB_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_lte_sys_ts_cnt_dsp       : 15; /* [14..0] [14:0]����֡�ڼ����� */
        unsigned int    reserved_1                 : 1; /* [15..15] ���� */
        unsigned int    o_lte_sys_subfrm_cnt_dsp   : 4; /* [19..16] [19:16]����֡���� */
        unsigned int    o_lte_sfn_dsp              : 10; /* [29..20] LTE��ϵͳ��ʱ����DSP ����timing_get_ind�����ϱ����ϱ�ʱ�̣�timing_validΪ��ʱ�ϱ���Ч������:[29:20]��ʾSFN��������Χ0~1023 */
        unsigned int    reserved_0                 : 2; /* [31..30] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_LTE_TIMING_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_tds_timing1_dsp          : 16; /* [15..0] TDS��ϵͳ��ʱ����DSP ����timing_get_ind�����ϱ������Ĵ����ϱ�����TDS��tcfsrϵͳ��֡��0~65535ѭ��������TDSʵ����Ч��֡��ΧΪ0~8191�ϱ�ʱ�̣�timing_validΪ��ʱ�ϱ���Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TDS_TIMING1_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_tds_timing2_dsp          : 18; /* [17..0] TDS��ϵͳ��ʱ����DSP ����timing_get_ind�����ϱ������Ĵ����ϱ�������TDS��framc֡�����ϱ�ʱ�̣�timing_validΪ��ʱ�ϱ���Ч */
        unsigned int    reserved                   : 14; /* [31..18] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TDS_TIMING2_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_ctu_timing_dsp           : 32; /* [31..0] CTU�ڲ���ʱ����DSP ����timing_get_ind�����ϱ���������ʹ���������ϱ�ʱ�̣�timing_validΪ��ʱ�ϱ���Ч */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CTU_TIMING_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_ctu_basecnt1_dsp         : 32; /* [31..0] CTU�ڲ���ʱ����������ʱ�Ӿͼ�����������ͷ��ʼ��������ʹ���������ϱ�ʱ�̣���ʱ��ȡ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CTU_BASECNT1_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_ctu_basecnt2_dsp         : 32; /* [31..0] CTU�ڲ���ʱ��ʹ��clk_19m_bbp_socʱ�Ӽ���������ģ��int0��int1��int2��ʱ�̴����ϱ��� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CTU_BASECNT2_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    ctu_basecnt_dsp            : 32; /* [31..0] CTU�ڲ�19.2M��������ʱ�������ϱ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CTU_BASECNT_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_ctu_timing_get_ind     : 1; /* [0..0] CTU�ڲ�19.2M��������ʱ�����������ȡʹ�� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DSP_CTU_TIMING_GET_IND_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    ctu_timing_valid_dsp       : 1; /* [0..0] CTU�ڲ�19.2M��������ʱ�����������ȡ��Чָʾ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_CTU_TIMING_VALID_DSP_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_ctu_timing_clr         : 1; /* [0..0] CTU�ڲ�19.2M��������ʱ�����������ȡ������ʹ�� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DSP_CTU_TIMING_CLR_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_w_intrasys_valid         : 1; /* [0..0] w��ģ�¿���RFָʾ����ƽ�źš�1��b0: W����RF����Ȩ��1��b1: W���п���RFȨ����ʵ������RF��w_intrasys_valid_delay�ź�Ϊ1���� */
        unsigned int    o_w_intrasys_valid_delay   : 1; /* [1..1] w��ģ��ʵ�ʿ���RFָʾ����ƽ�źš�Ϊ1����W����RF����ͨ����Ϊ0�����ģ���Կ���RFͨ�������ڵ�RFICʱ��Ƶ��������źŵ�ѡͨ��w_intrasys_valid_delay�������ر�w_intrasys_valid�������ͺ��½���һ�¡� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_W_INTRASYS_VALID_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_gm_intrasys_valid        : 1; /* [0..0] gm��ģ�¿���RFָʾ����ƽ�źš�1��b0: gm����RF����Ȩ��1��b1: gm���п���RFȨ����ʵ������RF��gm_intrasys_valid_delay�ź�Ϊ1���� */
        unsigned int    o_gm_intrasys_valid_delay  : 1; /* [1..1] gm��ģ��ʵ�ʿ���RFָʾ����ƽ�źš�Ϊ1����gm����RF����ͨ����Ϊ0�����ģ���Կ���RFͨ�������ڵ�RFICʱ��Ƶ��������źŵ�ѡͨ��gm_intrasys_valid_delay�������ر�gm_intrasys_valid�������ͺ��½���һ�¡� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GM_INTRASYS_VALID_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_gs_intrasys_valid        : 1; /* [0..0] gs��ģ�¿���RFָʾ����ƽ�źš�1��b0: gs����RF����Ȩ��1��b1: gs���п���RFȨ����ʵ������RF��gs_intrasys_valid_delay�ź�Ϊ1���� */
        unsigned int    o_gs_intrasys_valid_delay  : 1; /* [1..1] gs��ģ��ʵ�ʿ���RFָʾ����ƽ�źš�Ϊ1����gs����RF����ͨ����Ϊ0�����ģ���Կ���RFͨ�������ڵ�RFICʱ��Ƶ��������źŵ�ѡͨ��gs_intrasys_valid_delay�������ر�gs_intrasys_valid�������ͺ��½���һ�¡� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GS_INTRASYS_VALID_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_lte_intrasys_valid       : 1; /* [0..0] Lte��ģ�¿���RFָʾ����ƽ�źš�1��b0: LTE����RF����Ȩ��1��b1: LTE���п���RFȨ����ʵ������RF��lte_intrasys_valid_delay�ź�Ϊ1���� */
        unsigned int    o_lte_intrasys_valid_delay : 1; /* [1..1] LTE��ģ��ʵ�ʿ���RFָʾ����ƽ�źš�Ϊ1����LTE����RF����ͨ����Ϊ0�����ģ���Կ���RFͨ�������ڵ�RFICʱ��Ƶ��������źŵ�ѡͨ��lte_intrasys_valid_delay�������ر�lte_intrasys_valid�������ͺ��½���һ�¡� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_LTE_INTRASYS_VALID_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_tds_intrasys_valid       : 1; /* [0..0] TDS��ģ�¿���RFָʾ����ƽ�źš�1��b0: TDS����RF����Ȩ��1��b1: TDS���п���RFȨ����ʵ������RF��tds_intrasys_valid_delay�ź�Ϊ1���� */
        unsigned int    o_tds_intrasys_valid_delay : 1; /* [1..1] TDS��ģ��ʵ�ʿ���RFָʾ����ƽ�źš�Ϊ1����TDS����RF����ͨ����Ϊ0�����ģ���Կ���RFͨ�������ڵ�RFICʱ��Ƶ��������źŵ�ѡͨ��tds_intrasys_valid_delay�������ر�tds_intrasys_valid�������ͺ��½���һ�¡� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TDS_INTRASYS_VALID_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    cpu_rfic0_ck_cfg           : 3; /* [2..0] SSI1master�ȴ�ʱ�� */
        unsigned int    cpu_rfic1_ck_cfg           : 3; /* [5..3] SSI0master�ȴ�ʱ�� */
        unsigned int    reserved                   : 26; /* [31..6]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RFIC_CK_CFG_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_mipi_wait_time         : 8; /* [7..0] ͨ��xbbp��MIPI master�����ض���ָ��ʱ,DSP�ȴ��ض������������ */
        unsigned int    dsp_ssi_wait_time          : 8; /* [15..8] ͨ��xbbp��SSI master�����ض���ָ��ʱ,DSP�ȴ��ض������������ */
        unsigned int    reserved                   : 16; /* [31..16]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DSP_WAITTIME_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    s_w_ch_num_ind             : 1; /* [0..0] WΪ��ģʱ��Ӧ��ͨ��ѡ���źš�0��ѡ��ͨ��0��1��ѡ��ͨ��1������ʱ�̣���ģ����ͨ�ų������� */
        unsigned int    s_w_ch_num_ind_sel         : 1; /* [1..1] WΪ��ģʱ��Ӧ��ͨ��ѡ���ź���Ч��ʽ��0����int1��Ч��1��������Ч�� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_S_W_CH_NUM_IND_T;    /* WΪ��ģʱ��Ӧͨ����ָʾ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    w_intersys_measure_type    : 4; /* [3..0] ��ϵͳ�������͡�4��b0������3G��4��b1: 3G�����ѣ�4��d2: 3G ANR������Ԥ������Чʱ�̣�������Ч */
        unsigned int    reserved                   : 28; /* [31..4] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_W_INTERSYS_MEASURE_TYPE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    w_measure_report_valid     : 1; /* [0..0] ��Ƶ��������ڵ�ָʾ��L����TDS/Gm/Gs��Ϊ��ģ��֪ͨW DSP W�����ĵ�Ƶ�����ڵ��ֻ������̬����Ҫ���á���ƽ�źţ�����Ч����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_W_MEASURE_REPORT_VALID_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    w_preint_offset_time       : 32; /* [31..0] Ԥ�����ж�int0��int1֮���offset��Ϣ����λus������ȡ������������ʱ�̣���ģ����W��ģ��ϵͳ����ʱ������Чʱ�̣�������Ч */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_W_PREINT_OFFSET_TIME_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    w_intersys_measure_time    : 32; /* [31..0] W��Ϊ��ģʱ����ģ�����W������ϵͳ����ʱ�䳤�ȣ���֪WDSP����λus������ȡ����us������ʱ�̣���ģ����W��ģ��ϵͳ����ʱ������Чʱ�̣�������Ч */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_W_INTERSYS_MEASURE_TIME_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_w_rcv_lte        : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_W_RCV_LTE[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_W_RCV_LTE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_w_rcv_tds        : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_W_RCV_TDS[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_W_RCV_TDS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_w_rcv_gm         : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_W_RCV_GM[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_W_RCV_GM_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_w_rcv_gs         : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_W_RCV_GS[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_W_RCV_GS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_w_rcv_w          : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_W_RCV_W[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_W_RCV_W_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_w_int012         : 3; /* [2..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_W_INT012[2:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_W_INT012_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_w_rcv_lte         : 16; /* [15..0] �ж����α�־����������INT_TYPE_W_RCV_LTE[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_W_RCV_LTE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_w_rcv_tds         : 16; /* [15..0] �ж����α�־����������INT_TYPE_W_RCV_TDS[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_W_RCV_TDS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_w_rcv_gm          : 16; /* [15..0] �ж����α�־����������INT_TYPE_W_RCV_GM[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_W_RCV_GM_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_w_rcv_gs          : 16; /* [15..0] �ж����α�־����������INT_TYPE_W_RCV_GS[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_W_RCV_GS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_w_rcv_w           : 16; /* [15..0] �ж����α�־����������INT_TYPE_W_RCV_W[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_W_RCV_W_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_w_int012          : 3; /* [2..0] �ж����α�־����������INT_TYPE_W_INT012[2:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_W_INT012_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_w_rcv_lte       : 16; /* [15..0] �ж�״̬��־����bit������Ӧһ���ж����ͣ�����Ч��[0]: �ж�����0�� LTE����ģ����ǰ֪ͨW��ϵͳ���������жϡ�[1]: �ж�����1�� LTE����ģ����GAP�жϣ�֪ͨW������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2�� LTE����ģ����GAP�жϡ�[3]: �ж�����3��LTE����ģ�����������жϡ�[4]: �ж�����4��LTE����ģ��ǿ��W�˳���ϵͳ�����жϡ�[5]: �ж�����5��LTE����ģ��֪ͨW�ϱ����β��������[6]: ������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ���� */
        unsigned int    reserved                   : 16; /* [31..16] ������W�յ�LTE������жϣ� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_W_RCV_LTE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_w_rcv_tds       : 16; /* [15..0] �ж�״̬��־0��1��2ֻ�е�TDS������ģʱ���ܲ�����[0]: �ж�����0�� TDS����ģ����ǰ֪ͨW��ϵͳ���������жϡ�[1]: �ж�����1�� TDS����ģ����GAP�жϣ�֪ͨW������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2�� TDS����ģ����GAP�жϡ�[3]: �ж�����3��TDS����ģ�����������жϡ�[4]: �ж�����4��TDS����ģ��ǿ��W�˳���ϵͳ�����жϡ�[5]: �ж�����5��TDS����ģ��֪ͨW�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ���� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_W_RCV_TDS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_w_rcv_gm        : 16; /* [15..0] �ж�״̬��־����bit������Ӧһ���ж����ͣ�����Ч��[0]: �ж�����0��GM����ģ����ǰ֪ͨW��ϵͳ���������жϡ�[1]: �ж�����1��GM����ģ����GAP�жϣ�֪ͨW������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2��GM����ģ����GAP�жϡ�[3]: �ж�����3��GM����ģ�����������жϡ�[4]: �ж�����4��GM����ģ��ǿ��W�˳���ϵͳ�����жϡ�[5]: �ж�����5��GM����ģ��֪ͨW�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ�����ж�״̬��־0��1��2ֻ�е�GM������ģʱ���ܲ����� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_W_RCV_GM_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_w_rcv_gs        : 16; /* [15..0] [0]: �ж�����0��GS����ģ����ǰ֪ͨW��ϵͳ���������жϡ�[1]: �ж�����1��GS����ģ����GAP�жϣ�֪ͨW������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2��GS����ģ����GAP�жϡ�[3]: �ж�����3��GS����ģ�����������жϡ�[4]: �ж�����4��GS����ģ��ǿ��W�˳���ϵͳ�����жϡ�[5]: �ж�����5��GS����ģ��֪ͨW�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ�����ж�״̬��־0��1��2ֻ�е�TDS������ģʱ���ܲ����� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_W_RCV_GS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_w_rcv_w         : 16; /* [15..0] �ж�״̬��־����bit������Ӧһ���ж����ͣ�����Ч��[0]: �ж�����0��W����ģ����ǰ֪ͨW��Ƶ���������жϡ�[1]: �ж�����1��W����ģ����GAP�жϣ�֪ͨW������Ƶ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2��W����ģ����GAP�жϡ�[3]: �ж�����3��W����ģ������Ƶ�����������жϡ�[4]: �ж�����4��W����ģ��ǿ��W�˳���Ƶ�����жϡ�[5]: �ж�����5��W����ģ��֪ͨW�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ�����ж�״̬��־0��1��2ֻ�е�W������ģʱ���ܲ����� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_W_RCV_W_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_w_int012        : 3; /* [2..0] ������ģ����int0��1��2���ϱ���(������Ƶ����ϵͳ����)��������Ч��[0]: W��ģʱ��CTU����֪ͨWDSP����int0�жϡ�[1]: W��ģʱ��CTU����֪ͨWDSP����int1�жϡ�[2]: W��ģʱ��CTU����֪ͨWDSP����int2�жϡ� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_W_INT012_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0] ���� */
        unsigned int    w_snd_lte_int_13           : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧw_snd_lte_int_15 ~ 7��W DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_lte_rcv_w[15:7]����LDSP����Чʱ�̣�������Чbit6����Ӧw_snd_lte_int_6��WDSPͨ���˼Ĵ�����������6�жϣ�int_type_lte_rcv_w[6]����LDSP����Чʱ�̣�������Чbit5����Ӧw_snd_lte_int_5��W��ģʱ֪ͨLDSP L�������ڵ������Ч��W DSPͨ���˼Ĵ�����������5�жϣ�int_type_lte_rcv_w[5]����LDSP����Чʱ�̣�������Чbit4����Ӧw_snd_lte_int_4��W��ģʱǿ���˳���ϵͳ����ָʾ������Ч��W DSPͨ���˼Ĵ�����������4�жϣ�int_type_lte_rcv_w[4]����LDSP����Чʱ�̣�������Чbit3����Ӧw_snd_lte_int_3��W��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_lte_rcv_w[3]����LDSP��L��ģ�¶�W������ϵͳ����ʱ�����W����������W DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_W_SND_LTE_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0] ���� */
        unsigned int    w_snd_tds_int_13           : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧw_snd_tds_int_15 ~ 7��W DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_tds_rcv_w[15:7]����TDSP����Чʱ�̣�������Чbit6����Ӧw_snd_tds_int_6��WDSPͨ���˼Ĵ�����������6�жϣ�int_type_tds_rcv_w[6]����TDSP����Чʱ�̣�������Чbit5����Ӧw_snd_tds_int_5��W��ģʱ֪ͨTDSP T�������ڵ������Ч��W DSPͨ���˼Ĵ�����������5�жϣ�int_type_tds_rcv_w[5]����TDSP����Чʱ�̣�������Чbit4����Ӧw_snd_tds_int_4��W��ģʱǿ���˳���ϵͳ����ָʾ������Ч��W DSPͨ���˼Ĵ�����������4�жϣ�int_type_tds_rcv_w[4]����TDSP����Чʱ�̣�������Чbit3����Ӧw_snd_tds_int_3��W��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_tds_rcv_w[3]����TDSP��T��ģ�¶�W������ϵͳ����ʱ�����W����������W DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_W_SND_TDS_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0]  */
        unsigned int    w_snd_gm_int_13            : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧw_snd_gm_int_15 ~ 7��W DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_gm_rcv_w[15:7]����GMDSP����Чʱ�̣�������Чbit6����Ӧw_snd_gm_int_6��WDSPͨ���˼Ĵ�����������6�жϣ�int_type_gm_rcv_w[6]����GMDSP����Чʱ�̣�������Чbit5����Ӧw_snd_gm_int_5��W��ģʱ֪ͨGMDSP GM�������ڵ������Ч��W DSPͨ���˼Ĵ�����������5�жϣ�int_type_gm_rcv_w[5]����GMDSP����Чʱ�̣�������Чbit4����Ӧw_snd_gm_int_4��W��ģʱǿ���˳���ϵͳ����ָʾ������Ч��W DSPͨ���˼Ĵ�����������4�жϣ�int_type_gm_rcv_w[4]����GMDSP����Чʱ�̣�������Чbit3����Ӧw_snd_gm_int_3��W��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_gm_rcv_w[3]����GMDSP��GM��ģ�¶�W������ϵͳ����ʱ�����W����������W DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_W_SND_GM_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0]  */
        unsigned int    w_snd_gs_int_13            : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧw_snd_gs_int_15 ~ 7��W DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_gs_rcv_w[15:7]����GSDSP����Чʱ�̣�������Чbit6����Ӧw_snd_gs_int_6��WDSPͨ���˼Ĵ�����������6�жϣ�int_type_gs_rcv_w[6]����GSDSP����Чʱ�̣�������Чbit5����Ӧw_snd_gs_int_5��W��ģʱ֪ͨGSDSP GS�������ڵ������Ч��W DSPͨ���˼Ĵ�����������5�жϣ�int_type_gs_rcv_w[5]����GSDSP����Чʱ�̣�������Чbit4����Ӧw_snd_gs_int_4��W��ģʱǿ���˳���ϵͳ����ָʾ������Ч��W DSPͨ���˼Ĵ�����������4�жϣ�int_type_gs_rcv_w[4]����GSDSP����Чʱ�̣�������Чbit3����Ӧw_snd_gs_int_3��W��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_gs_rcv_w[3]����GSDSP��GS��ģ�¶�W������ϵͳ����ʱ�����W����������W DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_W_SND_GS_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0]  */
        unsigned int    w_snd_w_int_13             : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧw_snd_w_int_15 ~ 7��W DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_w_rcv_w[15:7]����WDSP����Чʱ�̣�������Чbit6����Ӧw_snd_w_int_6��WDSPͨ���˼Ĵ�����������6�жϣ�int_type_w_rcv_w[6]����WDSP����Чʱ�̣�������Чbit5����Ӧw_snd_w_int_5��W��ģʱ֪ͨWDSP W�������ڵ������Ч��W DSPͨ���˼Ĵ�����������5�жϣ�int_type_w_rcv_w[5]����WDSP����Чʱ�̣�������Чbit4����Ӧw_snd_w_int_4��W��ģʱǿ���˳���ϵͳ����ָʾ������Ч��W DSPͨ���˼Ĵ�����������4�жϣ�int_type_w_rcv_w[4]����WDSP����Чʱ�̣�������Чbit3����Ӧw_snd_w_int_3��W��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_w_rcv_w[3]����WDSP��W��ģ�¶�W������ϵͳ����ʱ�����W����������W DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_W_SND_W_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    s_lte_ch_num_ind           : 1; /* [0..0] LTEΪ��ģʱ��Ӧ��ͨ��ѡ���źš�0��ѡ��ͨ��0��1��ѡ��ͨ��1������ʱ�̣���ģ����ͨ�ų������� */
        unsigned int    s_lte_ch_num_ind_sel       : 1; /* [1..1] LTEΪ��ģʱ��Ӧ��ͨ��ѡ���ź���Ч��ʽ��0����int1��Ч��1��������Ч�� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_S_LTE_CH_NUM_IND_T;    /* LTEΪ��ģʱ��Ӧͨ����ָʾ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    lte_intersys_measure_type  : 4; /* [3..0] ��ϵͳ�������͡�4��b0������LTE��4��b1��������LTE������Ԥ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 28; /* [31..4] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_LTE_INTERSYS_MEASURE_TYPE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    lte_measure_report_valid   : 1; /* [0..0] ��Ƶ��������ڵ�ָʾ��W/Gm/Gs����TDS��Ϊ��ģ��֪ͨL DSP �����ĵ�Ƶ�����ڵ��ֻ������̬����Ҫ���ã���Gm��Lʱ����Ҫ���ã���ƽ�źţ�����Ч����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_LTE_MEASURE_REPORT_VALID_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    lte_preint_offset_time     : 32; /* [31..0] Ԥ�����ж�int0��int1֮���offset��Ϣ����λus������ȡ����������ʱ�̣�ÿ������LTE��ģ��ϵͳ����ʱ������Чʱ�̣�������Ч */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_LTE_PREINT_OFFSET_TIME_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    lte_intersys_measure_time  : 32; /* [31..0] LTE��Ϊ��ģʱ����ģ��ģ�����LTE������ϵͳ����ʱ�䳤��,��֪LTE DSP����λus������ȡ������ʱ�̣�ÿ������LTE��ģ��ϵͳ����ʱ������Чʱ�̣�������Ч */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_LTE_INTERSYS_MEASURE_TIME_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_lte_rcv_lte      : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_LTE_RCV_LTE[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_LTE_RCV_LTE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_lte_rcv_tds      : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_LTE_RCV_TDS[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_LTE_RCV_TDS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_lte_rcv_gm       : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_LTE_RCV_GM[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_LTE_RCV_GM_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_lte_rcv_gs       : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_LTE_RCV_GS[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_LTE_RCV_GS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_lte_rcv_w        : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_LTE_RCV_W[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_LTE_RCV_W_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_lte_int012       : 3; /* [2..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_LTE_INT012[2:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_LTE_INT012_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_lte_rcv_lte       : 16; /* [15..0] �ж����α�־����������INT_TYPE_LTE_RCV_LTE[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_LTE_RCV_LTE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_lte_rcv_tds       : 16; /* [15..0] �ж����α�־����������INT_TYPE_LTE_RCV_TDS[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_LTE_RCV_TDS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_lte_rcv_gm        : 16; /* [15..0] �ж����α�־����������INT_TYPE_LTE_RCV_GM[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_LTE_RCV_GM_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_lte_rcv_gs        : 16; /* [15..0] �ж����α�־����������INT_TYPE_LTE_RCV_GS[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_LTE_RCV_GS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_lte_rcv_w         : 16; /* [15..0] �ж����α�־����������INT_TYPE_LTE_RCV_W[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_LTE_RCV_W_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_lte_int012        : 3; /* [2..0] �ж����α�־����������INT_TYPE_LTE_INT012[2:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_LTE_INT012_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_lte_rcv_w       : 16; /* [15..0] �ж�״̬��־����bit������Ӧһ���ж����ͣ�����Ч��[0]: �ж�����0�� W����ģ����ǰ֪ͨLTE��ϵͳ���������жϡ�[1]: �ж�����1�� W����ģ����GAP�жϣ�֪ͨLTE������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2�� W����ģ����GAP�жϡ�[3]: �ж�����3��W����ģ�����������жϡ�[4]: �ж�����4��W����ģ��ǿ��LTE�˳���ϵͳ�����жϡ�[5]: �ж�����5��W����ģ��֪ͨLTE�ϱ����β��������[6]: ������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ���� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_LTE_RCV_W_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_lte_rcv_tds     : 16; /* [15..0] �ж�״̬��־0��1��2ֻ�е�TDS������ģʱ���ܲ�����[0]: �ж�����0�� TDS����ģ����ǰ֪ͨLTE��ϵͳ���������жϡ�[1]: �ж�����1�� TDS����ģ����GAP�жϣ�֪ͨLTE������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2�� TDS����ģ����GAP�жϡ�[3]: �ж�����3��TDS����ģ�����������жϡ�[4]: �ж�����4��TDS����ģ��ǿ��LTE�˳���ϵͳ�����жϡ�[5]: �ж�����5��TDS����ģ��֪ͨLTE�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ���� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_LTE_RCV_TDS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_lte_rcv_gm      : 16; /* [15..0] �ж�״̬��־����bit������Ӧһ���ж����ͣ�����Ч��[0]: �ж�����0��GM����ģ����ǰ֪ͨLTE��ϵͳ���������жϡ�[1]: �ж�����1��GM����ģ����GAP�жϣ�֪ͨLTE������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2��GM����ģ����GAP�жϡ�[3]: �ж�����3��GM����ģ�����������жϡ�[4]: �ж�����4��GM����ģ��ǿ��LTE�˳���ϵͳ�����жϡ�[5]: �ж�����5��GM����ģ��֪ͨLTE�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ�����ж�״̬��־0��1��2ֻ�е�GM������ģʱ���ܲ����� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_LTE_RCV_GM_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_lte_rcv_gs      : 16; /* [15..0] [0]: �ж�����0��GS����ģ����ǰ֪ͨLTE��ϵͳ���������жϡ�[1]: �ж�����1��GS����ģ����GAP�жϣ�֪ͨLTE������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2��GS����ģ����GAP�жϡ�[3]: �ж�����3��GS����ģ�����������жϡ�[4]: �ж�����4��GS����ģ��ǿ��LTE�˳���ϵͳ�����жϡ�[5]: �ж�����5��GS����ģ��֪ͨLTE�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ�����ж�״̬��־0��1��2ֻ�е�TDS������ģʱ���ܲ����� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_LTE_RCV_GS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_lte_rcv_lte     : 16; /* [15..0] �ж�״̬��־����bit������Ӧһ���ж����ͣ�����Ч��[0]: �ж�����0��LTE����ģ����ǰ֪ͨLTE��Ƶ���������жϡ�[1]: �ж�����1��LTE����ģ����GAP�жϣ�֪ͨLTE������Ƶ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2��LTE����ģ����GAP�жϡ�[3]: �ж�����3��LTE����ģ������Ƶ�����������жϡ�[4]: �ж�����4��LTE����ģ��ǿ��LTE�˳���Ƶ�����жϡ�[5]: �ж�����5��LTE����ģ��֪ͨLTE�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ�����ж�״̬��־0��1��2ֻ�е�LTE������ģʱ���ܲ����� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_LTE_RCV_LTE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_lte_int012      : 3; /* [2..0] ������ģ����int0��1��2���ϱ���(������Ƶ����ϵͳ����)��������Ч��[0]: L��ģʱ��CTU����֪ͨLDSP����int0�жϡ�[1]: L��ģʱ��CTU����֪ͨLDSP����int1�жϡ�[2]: L��ģʱ��CTU����֪ͨLDSP����int2�жϡ� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_LTE_INT012_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0] ���� */
        unsigned int    lte_snd_lte_int_13         : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧlte_snd_lte_int_15 ~ 7��L DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_lte_rcv_lte[15:7]����LDSP����Чʱ�̣�������Чbit6����Ӧlte_snd_lte_int_6��LDSPͨ���˼Ĵ�����������6�жϣ�int_type_lte_rcv_lte[6]����LDSP����Чʱ�̣�������Чbit5����Ӧlte_snd_lte_int_5��L��ģʱ֪ͨLDSP L�������ڵ������Ч��L DSPͨ���˼Ĵ�����������5�жϣ�int_type_lte_rcv_lte[5]����LDSP����Чʱ�̣�������Чbit4����Ӧlte_snd_lte_int_4��L��ģʱǿ���˳���ϵͳ����ָʾ������Ч��L DSPͨ���˼Ĵ�����������4�жϣ�int_type_lte_rcv_lte[4]����LDSP����Чʱ�̣�������Чbit3����Ӧlte_snd_lte_int_3��L��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_lte_rcv_lte[3]����LDSP��L��ģ�¶�L������ϵͳ����ʱ�����L����������L DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_LTE_SND_LTE_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0] ���� */
        unsigned int    lte_snd_tds_int_13         : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧlte_snd_tds_int_15 ~ 7��L DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_tds_rcv_lte[15:7]����TDSDSP����Чʱ�̣�������Чbit6����Ӧlte_snd_tds_int_6��LDSPͨ���˼Ĵ�����������6�жϣ�int_type_tds_rcv_lte[6]����TDSDSP����Чʱ�̣�������Чbit5����Ӧlte_snd_tds_int_5��L��ģʱ֪ͨTDSDSP TDS�������ڵ������Ч��L DSPͨ���˼Ĵ�����������5�жϣ�int_type_tds_rcv_lte[5]����TDSDSP����Чʱ�̣�������Чbit4����Ӧlte_snd_tds_int_4��L��ģʱǿ���˳���ϵͳ����ָʾ������Ч��L DSPͨ���˼Ĵ�����������4�жϣ�int_type_tds_rcv_lte[4]����TDSDSP����Чʱ�̣�������Чbit3����Ӧlte_snd_tds_int_3��L��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_tds_rcv_lte[3]����TDSDSP��TDS��ģ�¶�L������ϵͳ����ʱ�����L����������L DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_LTE_SND_TDS_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0]  */
        unsigned int    lte_snd_gm_int_13          : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧlte_snd_gm_int_15 ~ 7��L DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_gm_rcv_lte[15:7]����GMDSP����Чʱ�̣�������Чbit6����Ӧlte_snd_gm_int_6��LDSPͨ���˼Ĵ�����������6�жϣ�iint_type_gm_rcv_lte[6]����GMDSP����Чʱ�̣�������Чbit5����Ӧlte_snd_gm_int_5��L��ģʱ֪ͨGMDSP GM�������ڵ������Ч��L DSPͨ���˼Ĵ�����������5�жϣ�int_type_gm_rcv_lte[5]����GMDSP����Чʱ�̣�������Чbit4����Ӧlte_snd_gm_int_4��L��ģʱǿ���˳���ϵͳ����ָʾ������Ч��L DSPͨ���˼Ĵ�����������4�жϣ�int_type_gm_rcv_lte[4]����GMDSP����Чʱ�̣�������Чbit3����Ӧlte_snd_gm_int_3��L��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_gm_rcv_lte[3]����GMDSP��GM��ģ�¶�L������ϵͳ����ʱ�����L����������L DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_LTE_SND_GM_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0]  */
        unsigned int    lte_snd_gs_int_13          : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧlte_snd_gs_int_15 ~ 7��L DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_gs_rcv_lte[15:7]����GSDSP����Чʱ�̣�������Чbit6����Ӧlte_snd_gs_int_6��LDSPͨ���˼Ĵ�����������6�жϣ�int_type_gs_rcv_lte[6]����GSDSP����Чʱ�̣�������Чbit5����Ӧlte_snd_gs_int_5��L��ģʱ֪ͨGSDSP GS�������ڵ������Ч��L DSPͨ���˼Ĵ�����������5�жϣ�int_type_gs_rcv_lte[5]����GSDSP����Чʱ�̣�������Чbit4����Ӧlte_snd_gs_int_4��L��ģʱǿ���˳���ϵͳ����ָʾ������Ч��L DSPͨ���˼Ĵ�����������4�жϣ�int_type_gs_rcv_lte[4]����GSDSP����Чʱ�̣�������Чbit3����Ӧlte_snd_gs_int_3��L��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_gs_rcv_lte[3]����GSDSP��GS��ģ�¶�L������ϵͳ����ʱ�����L����������L DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_LTE_SND_GS_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0]  */
        unsigned int    lte_snd_w_int_13           : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧlte_snd_w_int_15 ~ 7��L DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_w_rcv_lte[15:7]����WDSP����Чʱ�̣�������Чbit6����Ӧlte_snd_w_int_6��LDSPͨ���˼Ĵ�����������6�жϣ�int_type_w_rcv_lte[6]����WDSP����Чʱ�̣�������Чbit5����Ӧlte_snd_w_int_5��L��ģʱ֪ͨWDSP W�������ڵ������Ч��L DSPͨ���˼Ĵ�����������5�жϣ�int_type_w_rcv_lte[5]����WDSP����Чʱ�̣�������Чbit4����Ӧlte_snd_w_int_4��L��ģʱǿ���˳���ϵͳ����ָʾ������Ч��L DSPͨ���˼Ĵ�����������4�жϣ�int_type_w_rcv_lte[4]����WDSP����Чʱ�̣�������Чbit3����Ӧlte_snd_w_int_3��L��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_w_rcv_lte[3]����WDSP��W��ģ�¶�L������ϵͳ����ʱ�����L����������L DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_LTE_SND_W_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    s_tds_ch_num_ind           : 1; /* [0..0] TDSΪ��ģʱ��Ӧ��ͨ��ѡ���źš�0��ѡ��ͨ��0��1��ѡ��ͨ��1������ʱ�̣���ģ����ͨ�ų������� */
        unsigned int    s_tds_ch_num_ind_sel       : 1; /* [1..1] TDSΪ��ģʱ��Ӧ��ͨ��ѡ���ź���Ч��ʽ��0����int1��Ч��1��������Ч�� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_S_TDS_CH_NUM_IND_T;    /* TDSΪ��ģʱ��Ӧͨ����ָʾ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    tds_intersys_measure_type  : 4; /* [3..0] ��ϵͳ�������� ��4��b0�� ����TDS4��b1�� ������TDS����Ԥ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 28; /* [31..4] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TDS_INTERSYS_MEASURE_TYPE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    tds_measure_report_valid   : 1; /* [0..0] ��Ƶ��������ڵ�ָʾ��U/Gm/Gs����L��Ϊ��ģ��֪ͨT DSP W�����ĵ�Ƶ�����ڵ��ֻ������̬����Ҫ���ã���G��TDSʱ����Ҫ���ã���ƽ�źţ�����Ч����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TDS_MEASURE_REPORT_VALID_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    tds_preint_offset_time     : 32; /* [31..0] Ԥ�����ж�int0��int1֮���offset��Ϣ����λus������ȡ����������ʱ�̣���ģ����TDS��ģ��ϵͳ����ʱ������Чʱ�̣�������Ч */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TDS_PREINT_OFFSET_TIME_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    tds_intersys_measure_time  : 32; /* [31..0] ����ʱ�䳤�ȣ���֪TDS DSP����λus������ȡ����us������ʱ�̣���ģ����TDS��ģ��ϵͳ����ʱ������Чʱ�̣�������Ч */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TDS_INTERSYS_MEASURE_TIME_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_tds_rcv_lte      : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_TDS_RCV_LTE[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_TDS_RCV_LTE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_tds_rcv_tds      : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_TDS_RCV_TDS[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_TDS_RCV_TDS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_tds_rcv_gm       : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_TDS_RCV_GM[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_TDS_RCV_GM_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_tds_rcv_gs       : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_TDS_RCV_GS[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_TDS_RCV_GS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_tds_rcv_w        : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_TDS_RCV_W[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_TDS_RCV_W_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_tds_int012       : 3; /* [2..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_TDS_INT012[2:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_TDS_INT012_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_tds_rcv_lte       : 16; /* [15..0] �ж����α�־����������INT_TYPE_TDS_RCV_LTE[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_TDS_RCV_LTE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_tds_rcv_tds       : 16; /* [15..0] �ж����α�־����������INT_TYPE_TDS_RCV_TDS[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_TDS_RCV_TDS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_tds_rcv_gm        : 16; /* [15..0] �ж����α�־����������INT_TYPE_TDS_RCV_GM[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_TDS_RCV_GM_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_tds_rcv_gs        : 16; /* [15..0] �ж����α�־����������INT_TYPE_TDS_RCV_GS[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_TDS_RCV_GS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_tds_rcv_w         : 16; /* [15..0] �ж����α�־����������INT_TYPE_TDS_RCV_W[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_TDS_RCV_W_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_tds_int012        : 3; /* [2..0] �ж����α�־����������INT_TYPE_TDS_INT012[2:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_TDS_INT012_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_tds_rcv_lte     : 16; /* [15..0] �ж�״̬��־����bit������Ӧһ���ж����ͣ�����Ч��[0]: �ж�����0�� LTE����ģ����ǰ֪ͨTDS��ϵͳ���������жϡ�[1]: �ж�����1�� LTE����ģ����GAP�жϣ�֪ͨTDS������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2�� LTE����ģ����GAP�жϡ�[3]: �ж�����3��LTE����ģ�����������жϡ�[4]: �ж�����4��LTE����ģ��ǿ��TDS�˳���ϵͳ�����жϡ�[5]: �ж�����5��LTE����ģ��֪ͨTDS�ϱ����β��������[6]: ������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ���� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_TDS_RCV_LTE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_tds_rcv_w       : 16; /* [15..0] �ж�״̬��־0��1��2ֻ�е�TDS������ģʱ���ܲ�����[0]: �ж�����0�� TDS����ģ����ǰ֪ͨTDS��ϵͳ���������жϡ�[1]: �ж�����1�� TDS����ģ����GAP�жϣ�֪ͨTDS������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2�� TDS����ģ����GAP�жϡ�[3]: �ж�����3��TDS����ģ�����������жϡ�[4]: �ж�����4��TDS����ģ��ǿ��TDS�˳���ϵͳ�����жϡ�[5]: �ж�����5��TDS����ģ��֪ͨTDS�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ���� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_TDS_RCV_W_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_tds_rcv_gm      : 16; /* [15..0] �ж�״̬��־����bit������Ӧһ���ж����ͣ�����Ч��[0]: �ж�����0��GM����ģ����ǰ֪ͨTDS��ϵͳ���������жϡ�[1]: �ж�����1��GM����ģ����GAP�жϣ�֪ͨTDS������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2��GM����ģ����GAP�жϡ�[3]: �ж�����3��GM����ģ�����������жϡ�[4]: �ж�����4��GM����ģ��ǿ��TDS�˳���ϵͳ�����жϡ�[5]: �ж�����5��GM����ģ��֪ͨTDS�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ�����ж�״̬��־0��1��2ֻ�е�GM������ģʱ���ܲ����� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_TDS_RCV_GM_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_tds_rcv_gs      : 16; /* [15..0] [0]: �ж�����0��GS����ģ����ǰ֪ͨTDS��ϵͳ���������жϡ�[1]: �ж�����1��GS����ģ����GAP�жϣ�֪ͨTDS������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2��GS����ģ����GAP�жϡ�[3]: �ж�����3��GS����ģ�����������жϡ�[4]: �ж�����4��GS����ģ��ǿ��TDS�˳���ϵͳ�����жϡ�[5]: �ж�����5��GS����ģ��֪ͨTDS�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ�����ж�״̬��־0��1��2ֻ�е�TDS������ģʱ���ܲ����� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_TDS_RCV_GS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_tds_rcv_tds     : 16; /* [15..0] �ж�״̬��־����bit������Ӧһ���ж����ͣ�����Ч��[0]: �ж�����0��TDS����ģ����ǰ֪ͨTDS��Ƶ���������жϡ�[1]: �ж�����1��TDS����ģ����GAP�жϣ�֪ͨTDS������Ƶ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2��TDS����ģ����GAP�жϡ�[3]: �ж�����3��TDS����ģ������Ƶ�����������жϡ�[4]: �ж�����4��TDS����ģ��ǿ��TDS�˳���Ƶ�����жϡ�[5]: �ж�����5��TDS����ģ��֪ͨTDS�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ�����ж�״̬��־0��1��2ֻ�е�TDS������ģʱ���ܲ����� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_TDS_RCV_TDS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_tds_int012      : 3; /* [2..0] ������ģ����int0��1��2���ϱ���(������Ƶ����ϵͳ����)��������Ч��[0]: T��ģʱ��CTU����֪ͨTDSP����int0�жϡ�[1]: T��ģʱ��CTU����֪ͨTDSP����int1�жϡ�[2]: T��ģʱ��CTU����֪ͨTDSP����int2�жϡ� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_TDS_INT012_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0] ���� */
        unsigned int    tds_snd_lte_int_13         : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧtds_snd_lte_int_15 ~ 7��TDS DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_lte_rcv_tds[15:7]����LDSP����Чʱ�̣�������Чbit6����Ӧtds_snd_lte_int_6��TDSDSPͨ���˼Ĵ�����������6�жϣ�int_type_lte_rcv_tds[6]����LDSP����Чʱ�̣�������Чbit5����Ӧtds_snd_lte_int_5��TDS��ģʱ֪ͨLDSP L�������ڵ������Ч��TDS DSPͨ���˼Ĵ�����������5�жϣ�int_type_lte_rcv_tds[5]����LDSP����Чʱ�̣�������Чbit4����Ӧtds_snd_lte_int_4��TDS��ģʱǿ���˳���ϵͳ����ָʾ������Ч��TDS DSPͨ���˼Ĵ�����������4�жϣ�int_type_lte_rcv_tds[4]����LDSP����Чʱ�̣�������Чbit3����Ӧtds_snd_lte_int_3��TDS��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_lte_rcv_tds[3]����LDSP��L��ģ�¶�TDS������ϵͳ����ʱ�����TDS����������TDS DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TDS_SND_LTE_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0] ���� */
        unsigned int    tds_snd_tds_int_13         : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧtds_snd_tds_int_15 ~ 7��TDS DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_tds_rcv_tds[15:7]����TDSP����Чʱ�̣�������Чbit6����Ӧtds_snd_tds_int_6��TDSDSPͨ���˼Ĵ�����������6�жϣ�int_type_tds_rcv_tds[6]����TDSP����Чʱ�̣�������Чbit5����Ӧtds_snd_tds_int_5��TDS��ģʱ֪ͨTDSP T�������ڵ������Ч��TDS DSPͨ���˼Ĵ�����������5�жϣ�int_type_tds_rcv_tds[5]����TDSP����Чʱ�̣�������Чbit4����Ӧtds_snd_tds_int_4��TDS��ģʱǿ���˳���ϵͳ����ָʾ������Ч��TDS DSPͨ���˼Ĵ�����������4�жϣ�int_type_tds_rcv_tds[4]����TDSP����Чʱ�̣�������Чbit3����Ӧtds_snd_tds_int_3��TDS��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_tds_rcv_tds[3]����TDSP��T��ģ�¶�TDS������ϵͳ����ʱ�����TDS����������TDS DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TDS_SND_TDS_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0]  */
        unsigned int    tds_snd_gm_int_13          : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧtds_snd_gm_int_15 ~ 7��TDS DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_gm_rcv_tds[15:7]����GMDSP����Чʱ�̣�������Чbit6����Ӧtds_snd_gm_int_6��TDSDSPͨ���˼Ĵ�����������6�жϣ�int_type_gm_rcv_tds[6]����GMDSP����Чʱ�̣�������Чbit5����Ӧtds_snd_gm_int_5��TDS��ģʱ֪ͨGMDSP GM�������ڵ������Ч��TDS DSPͨ���˼Ĵ�����������5�жϣ�int_type_gm_rcv_tds[5]����GMDSP����Чʱ�̣�������Чbit4����Ӧtds_snd_gm_int_4��TDS��ģʱǿ���˳���ϵͳ����ָʾ������Ч��TDS DSPͨ���˼Ĵ�����������4�жϣ�int_type_gm_rcv_tds[4]����GMDSP����Чʱ�̣�������Чbit3����Ӧtds_snd_gm_int_3��TDS��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_gm_rcv_tds[3]����GMDSP��GM��ģ�¶�TDS������ϵͳ����ʱ�����TDS����������TDS DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TDS_SND_GM_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0]  */
        unsigned int    tds_snd_gs_int_13          : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧtds_snd_gs_int_15 ~ 7��TDS DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_gs_rcv_tds[15:7]����GSDSP����Чʱ�̣�������Чbit6����Ӧtds_snd_gs_int_6��TDSDSPͨ���˼Ĵ�����������6�жϣ�int_type_gs_rcv_tds[6]����GSDSP����Чʱ�̣�������Чbit5����Ӧtds_snd_gs_int_5��TDS��ģʱ֪ͨGSDSP GS�������ڵ������Ч��TDS DSPͨ���˼Ĵ�����������5�жϣ�int_type_gs_rcv_tds[5]����GSDSP����Чʱ�̣�������Чbit4����Ӧtds_snd_gs_int_4��TDS��ģʱǿ���˳���ϵͳ����ָʾ������Ч��TDS DSPͨ���˼Ĵ�����������4�жϣ�int_type_gs_rcv_tds[4]����GSDSP����Чʱ�̣�������Чbit3����Ӧtds_snd_gs_int_3��TDS��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_gs_rcv_tds[3]����GSDSP��GS��ģ�¶�TDS������ϵͳ����ʱ�����TDS����������TDS DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TDS_SND_GS_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0]  */
        unsigned int    tds_snd_w_int_13           : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧtds_snd_w_int_15 ~ 7��TDS DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_w_rcv_tds[15:7]����WDSP����Чʱ�̣�������Чbit6����Ӧtds_snd_w_int_6��TDSDSPͨ���˼Ĵ�����������6�жϣ�int_type_w_rcv_tds[6]����WDSP����Чʱ�̣�������Чbit5����Ӧtds_snd_w_int_5��TDS��ģʱ֪ͨWDSP W�������ڵ������Ч��TDS DSPͨ���˼Ĵ�����������5�жϣ�int_type_w_rcv_tds[5]����WDSP����Чʱ�̣�������Чbit4����Ӧtds_snd_w_int_4��TDS��ģʱǿ���˳���ϵͳ����ָʾ������Ч��TDS DSPͨ���˼Ĵ�����������4�жϣ�int_type_w_rcv_tds[4]����WDSP����Чʱ�̣�������Чbit3����Ӧtds_snd_w_int_3��TDS��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_w_rcv_tds[3]����WDSP��W��ģ�¶�TDS������ϵͳ����ʱ�����TDS����������TDS DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TDS_SND_W_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    s_gm_ch_num_ind            : 1; /* [0..0] GMΪ��ģʱ��Ӧ��ͨ��ѡ���źš�0��ѡ��ͨ��0��1��ѡ��ͨ��1������ʱ�̣���ģ����ͨ�ų������� */
        unsigned int    s_gm_ch_num_ind_sel        : 1; /* [1..1] GMΪ��ģʱ��Ӧ��ͨ��ѡ���ź���Ч��ʽ��0����int1��Ч��1��������Ч�� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_S_GM_CH_NUM_IND_T;    /* GMΪ��ģʱ��Ӧͨ����ָʾ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    gm_intersys_measure_type   : 4; /* [3..0] ��ϵͳ�������͡�4��d0������GM RSSI��4��d1: ����GM bsic verification��4��d2: ����GM bsic confirm��4��d3: ������GM��4��d4: 2G ANR������Ԥ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 28; /* [31..4] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GM_INTERSYS_MEASURE_TYPE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    gm_measure_report_valid    : 1; /* [0..0] ��Ƶ��������ڵ�ָʾ��L����TDS/W/Gs��Ϊ��ģ��֪ͨGM DSP GM�����ĵ�Ƶ�����ڵ��ֻ������̬����Ҫ���á���ƽ�źţ�����Ч����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GM_MEASURE_REPORT_VALID_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    gm_preint_offset_time      : 32; /* [31..0] Ԥ�����ж�int0��int1֮���offset��Ϣ����λus������ȡ������������ʱ�̣���ģ����GM��ģ��ϵͳ����ʱ������Чʱ�̣�������Ч */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GM_PREINT_OFFSET_TIME_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    gm_intersys_measure_time   : 32; /* [31..0] GM��Ϊ��ģʱ����ģ�����GM������ϵͳ����ʱ�䳤�ȣ���֪GMDSP����λus������ȡ����us������ʱ�̣���ģ����GM��ģ��ϵͳ����ʱ������Чʱ�̣�������Ч */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GM_INTERSYS_MEASURE_TIME_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_gm_rcv_lte       : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_GM_RCV_LTE[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_GM_RCV_LTE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_gm_rcv_tds       : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_GM_RCV_TDS[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_GM_RCV_TDS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_gm_rcv_gm        : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_GM_RCV_GM[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_GM_RCV_GM_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_gm_rcv_gs        : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_GM_RCV_GS[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_GM_RCV_GS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_gm_rcv_w         : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_GM_RCV_W[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_GM_RCV_W_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_gm_int012        : 3; /* [2..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_GM_INT012[2:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_GM_INT012_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_gm_rcv_lte        : 16; /* [15..0] �ж����α�־����������INT_TYPE_GM_RCV_LTE[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_GM_RCV_LTE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_gm_rcv_tds        : 16; /* [15..0] �ж����α�־����������INT_TYPE_GM_RCV_TDS[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_GM_RCV_TDS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_gm_rcv_gm         : 16; /* [15..0] �ж����α�־����������INT_TYPE_GM_RCV_GM[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_GM_RCV_GM_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_gm_rcv_gs         : 16; /* [15..0] �ж����α�־����������INT_TYPE_GM_RCV_GS[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_GM_RCV_GS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_gm_rcv_w          : 16; /* [15..0] �ж����α�־����������INT_TYPE_GM_RCV_W[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_GM_RCV_W_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_gm_int012         : 3; /* [2..0] �ж����α�־����������INT_TYPE_GM_INT012[2:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_GM_INT012_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_gm_rcv_lte      : 16; /* [15..0] �ж�״̬��־����bit������Ӧһ���ж����ͣ�����Ч��[0]: �ж�����0�� LTE����ģ����ǰ֪ͨGM��ϵͳ���������жϡ�[1]: �ж�����1�� LTE����ģ����GAP�жϣ�֪ͨGM������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2�� LTE����ģ����GAP�жϡ�[3]: �ж�����3��LTE����ģ�����������жϡ�[4]: �ж�����4��LTE����ģ��ǿ��GM�˳���ϵͳ�����жϡ�[5]: �ж�����5��LTE����ģ��֪ͨGM�ϱ����β��������[6]: ������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ���� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_GM_RCV_LTE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_gm_rcv_tds      : 16; /* [15..0] �ж�״̬��־0��1��2ֻ�е�TDS������ģʱ���ܲ�����[0]: �ж�����0�� TDS����ģ����ǰ֪ͨGM��ϵͳ���������жϡ�[1]: �ж�����1�� TDS����ģ����GAP�жϣ�֪ͨGM������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2�� TDS����ģ����GAP�жϡ�[3]: �ж�����3��TDS����ģ�����������жϡ�[4]: �ж�����4��TDS����ģ��ǿ��GM�˳���ϵͳ�����жϡ�[5]: �ж�����5��TDS����ģ��֪ͨGM�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ���� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_GM_RCV_TDS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_gm_rcv_w        : 16; /* [15..0] �ж�״̬��־����bit������Ӧһ���ж����ͣ�����Ч��[0]: �ж�����0��W����ģ����ǰ֪ͨGM��ϵͳ���������жϡ�[1]: �ж�����1��W����ģ����GAP�жϣ�֪ͨGM������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2��W����ģ����GAP�жϡ�[3]: �ж�����3��W����ģ�����������жϡ�[4]: �ж�����4��W����ģ��ǿ��GM�˳���ϵͳ�����жϡ�[5]: �ж�����5��W����ģ��֪ͨGM�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ�����ж�״̬��־0��1��2ֻ�е�W������ģʱ���ܲ����� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_GM_RCV_W_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_gm_rcv_gs       : 16; /* [15..0] [0]: �ж�����0��GS����ģ����ǰ֪ͨGM��ϵͳ���������жϡ�[1]: �ж�����1��GS����ģ����GAP�жϣ�֪ͨGM������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2��GS����ģ����GAP�жϡ�[3]: �ж�����3��GS����ģ�����������жϡ�[4]: �ж�����4��GS����ģ��ǿ��GM�˳���ϵͳ�����жϡ�[5]: �ж�����5��GS����ģ��֪ͨGM�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ�����ж�״̬��־0��1��2ֻ�е�TDS������ģʱ���ܲ����� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_GM_RCV_GS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_gm_rcv_gm       : 16; /* [15..0] �ж�״̬��־����bit������Ӧһ���ж����ͣ�����Ч��[0]: �ж�����0��GM����ģ����ǰ֪ͨGM��Ƶ���������жϡ�[1]: �ж�����1��GM����ģ����GAP�жϣ�֪ͨGM������Ƶ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2��GM����ģ����GAP�жϡ�[3]: �ж�����3��GM����ģ������Ƶ�����������жϡ�[4]: �ж�����4��GM����ģ��ǿ��GM�˳���Ƶ�����жϡ�[5]: �ж�����5��GM����ģ��֪ͨGM�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ�����ж�״̬��־0��1��2ֻ�е�GM������ģʱ���ܲ����� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_GM_RCV_GM_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_gm_int012       : 3; /* [2..0] ������ģ����int0��1��2���ϱ���(������Ƶ����ϵͳ����)��������Ч��[0]: GM��ģʱ��CGMU����֪ͨGMDSP����int0�жϡ�[1]: GM��ģʱ��CGMU����֪ͨGMDSP����int1�жϡ�[2]: GM��ģʱ��CGMU����֪ͨGMDSP����int2�жϡ� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_GM_INT012_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0] ���� */
        unsigned int    gm_snd_lte_int_13          : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧgm_snd_lte_int_15 ~ 7��GM DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_lte_rcv_gm[15:7]����LTEDSP����Чʱ�̣�������Чbit6����Ӧgm_snd_lte_int_6��GMDSPͨ���˼Ĵ�����������6�жϣ�int_type_lte_rcv_gm[6]����LTEDSP����Чʱ�̣�������Чbit5����Ӧgm_snd_lte_int_5��GM��ģʱ֪ͨLTEDSP LTE�������ڵ������Ч��GM DSPͨ���˼Ĵ�����������5�жϣ�int_type_lte_rcv_gm[5]����LTEDSP����Чʱ�̣�������Чbit4����Ӧgm_snd_lte_int_4��GM��ģʱǿ���˳���ϵͳ����ָʾ������Ч��GM DSPͨ���˼Ĵ�����������4�жϣ�int_type_lte_rcv_gm[4]����LTEDSP����Чʱ�̣�������Чbit3����Ӧgm_snd_lte_int_3��GM��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_lte_rcv_gm[3]����LTEDSP��LTE��ģ�¶�GM������ϵͳ����ʱ�����GM����������GM DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GM_SND_LTE_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0] ���� */
        unsigned int    gm_snd_tds_int_13          : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧgm_snd_tds_int_15 ~ 7��GM DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_tds_rcv_gm[15:7]����TDSDSP����Чʱ�̣�������Чbit6����Ӧgm_snd_tds_int_6��GMDSPͨ���˼Ĵ�����������6�жϣ�int_type_tds_rcv_gm[6]����TDSDSP����Чʱ�̣�������Чbit5����Ӧgm_snd_tds_int_5��GM��ģʱ֪ͨTDSDSP tds�������ڵ������Ч��GM DSPͨ���˼Ĵ�����������5�жϣ�int_type_tds_rcv_gm[5]����TDSDSP����Чʱ�̣�������Чbit4����Ӧgm_snd_tds_int_4��GM��ģʱǿ���˳���ϵͳ����ָʾ������Ч��GM DSPͨ���˼Ĵ�����������4�жϣ�int_type_tds_rcv_gm[4]����TDSDSP����Чʱ�̣�������Чbit3����Ӧgm_snd_tds_int_3��GM��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_tds_rcv_gm[3]����TDSDSP��tds��ģ�¶�GM������ϵͳ����ʱ�����GM����������GM DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GM_SND_TDS_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0]  */
        unsigned int    gm_snd_gm_int_13           : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧgm_snd_gm_int_15 ~ 7��GM DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_gm_rcv_gm[15:7]����GMDSP����Чʱ�̣�������Чbit6����Ӧgm_snd_gm_int_6��GMDSPͨ���˼Ĵ�����������6�жϣ�int_type_gm_rcv_gm[6]����GMDSP����Чʱ�̣�������Чbit5����Ӧgm_snd_gm_int_5��GM��ģʱ֪ͨGMDSP GM�������ڵ������Ч��GM DSPͨ���˼Ĵ�����������5�жϣ�int_type_gm_rcv_gm[5]����GMDSP����Чʱ�̣�������Чbit4����Ӧgm_snd_gm_int_4��GM��ģʱǿ���˳���ϵͳ����ָʾ������Ч��GM DSPͨ���˼Ĵ�����������4�жϣ�int_type_gm_rcv_gm[4]����GMDSP����Чʱ�̣�������Чbit3����Ӧgm_snd_gm_int_3��GM��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_gm_rcv_gm[3]����GMDSP��GM��ģ�¶�GM������ϵͳ����ʱ�����GM����������GM DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GM_SND_GM_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0]  */
        unsigned int    gm_snd_gs_int_13           : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧgm_snd_gs_int_15 ~ 7��GM DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_gs_rcv_gm[15:7]����GSDSP����Чʱ�̣�������Чbit6����Ӧgm_snd_gs_int_6��GMDSPͨ���˼Ĵ�����������6�жϣ�int_type_gs_rcv_gm[6]����GSDSP����Чʱ�̣�������Чbit5����Ӧgm_snd_gs_int_5��GM��ģʱ֪ͨGSDSP GS�������ڵ������Ч��GM DSPͨ���˼Ĵ�����������5�жϣ�int_type_gs_rcv_gm[5]����GSDSP����Чʱ�̣�������Чbit4����Ӧgm_snd_gs_int_4��GM��ģʱǿ���˳���ϵͳ����ָʾ������Ч��GM DSPͨ���˼Ĵ�����������4�жϣ�int_type_gs_rcv_gm[4]����GSDSP����Чʱ�̣�������Чbit3����Ӧgm_snd_gs_int_3��GM��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_gs_rcv_gm[3]����GSDSP��GS��ģ�¶�GM������ϵͳ����ʱ�����GM����������GM DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GM_SND_GS_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0]  */
        unsigned int    gm_snd_w_int_13            : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧgm_snd_lte_int_15 ~ 7��GM DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_lte_rcv_gm[15:7]����LTEDSP����Чʱ�̣�������Чbit6����Ӧgm_snd_lte_int_6��GMDSPͨ���˼Ĵ�����������6�жϣ�int_type_lte_rcv_gm[6]����LTEDSP����Чʱ�̣�������Чbit5����Ӧgm_snd_lte_int_5��GM��ģʱ֪ͨLTEDSP LTE�������ڵ������Ч��GM DSPͨ���˼Ĵ�����������5�жϣ�int_type_lte_rcv_gm[5]����LTEDSP����Чʱ�̣�������Чbit4����Ӧgm_snd_lte_int_4��GM��ģʱǿ���˳���ϵͳ����ָʾ������Ч��GM DSPͨ���˼Ĵ�����������4�жϣ�int_type_lte_rcv_gm[4]����LTEDSP����Чʱ�̣�������Чbit3����Ӧgm_snd_lte_int_3��GM��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_lte_rcv_gm[3]����LTEDSP��LTE��ģ�¶�GM������ϵͳ����ʱ�����GM����������GM DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GM_SND_W_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    s_gs_ch_num_ind            : 1; /* [0..0] GSΪ��ģʱ��Ӧ��ͨ��ѡ���źš�0��ѡ��ͨ��0��1��ѡ��ͨ��1������ʱ�̣���ģ����ͨ�ų������� */
        unsigned int    s_gs_ch_num_ind_sel        : 1; /* [1..1] GSΪ��ģʱ��Ӧ��ͨ��ѡ���ź���Ч��ʽ��0����int1��Ч��1��������Ч�� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_S_GS_CH_NUM_IND_T;    /* GSΪ��ģʱ��Ӧͨ����ָʾ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    gs_intersys_measure_type   : 4; /* [3..0] ��ϵͳ�������͡�4��d0������GS RSSI��4��d1: ����GS bsic verification��4��d2: ����GS bsic confirm��4��d3: ������GS4��d4: 2G ANR������Ԥ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 28; /* [31..4] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GS_INTERSYS_MEASURE_TYPE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    gs_measure_report_valid    : 1; /* [0..0] ��Ƶ��������ڵ�ָʾ��L����TDS/W/Gm��Ϊ��ģ��֪ͨGS DSP GS�����ĵ�Ƶ�����ڵ��ֻ������̬����Ҫ���á���ƽ�źţ�����Ч����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GS_MEASURE_REPORT_VALID_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    gs_preint_offset_time      : 32; /* [31..0] Ԥ�����ж�int0��int1֮���offset��Ϣ����λus������ȡ������������ʱ�̣���ģ����GS��ģ��ϵͳ����ʱ������Чʱ�̣�������Ч */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GS_PREINT_OFFSET_TIME_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    gs_intersys_measure_time   : 32; /* [31..0] GS��Ϊ��ģʱ����ģ�����GS������ϵͳ����ʱ�䳤�ȣ���֪GSDSP����λus������ȡ����us������ʱ�̣���ģ����GS��ģ��ϵͳ����ʱ������Чʱ�̣�������Ч */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GS_INTERSYS_MEASURE_TIME_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_gs_rcv_lte       : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_GS_RCV_LTE[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_GS_RCV_LTE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_gs_rcv_tds       : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_GS_RCV_TDS[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_GS_RCV_TDS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_gs_rcv_gm        : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_GS_RCV_GM[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_GS_RCV_GM_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_gs_rcv_gs        : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_GS_RCV_GS[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_GS_RCV_GS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_gs_rcv_w         : 16; /* [15..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_GS_RCV_W[15:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_GS_RCV_W_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_clear_gs_int012        : 3; /* [2..0] �߼������㡣�жϱ�־���ָʾ������Ч���������INT_TYPE_GS_INT012[2:0]��Ӧbitλ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_CLEAR_GS_INT012_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_gs_rcv_lte        : 16; /* [15..0] �ж����α�־����������INT_TYPE_GS_RCV_LTE[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_GS_RCV_LTE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_gs_rcv_tds        : 16; /* [15..0] �ж����α�־����������INT_TYPE_GS_RCV_TDS[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_GS_RCV_TDS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_gs_rcv_gm         : 16; /* [15..0] �ж����α�־����������INT_TYPE_GS_RCV_GM[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_GS_RCV_GM_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_gs_rcv_gs         : 16; /* [15..0] �ж����α�־����������INT_TYPE_GS_RCV_GS[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_GS_RCV_GS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_gs_rcv_w          : 16; /* [15..0] �ж����α�־����������INT_TYPE_GS_RCV_W[15:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_GS_RCV_W_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    int_mask_gs_int012         : 3; /* [2..0] �ж����α�־����������INT_TYPE_GS_INT012[2:0]��Ӧbit�жϡ�0�������ж��ϱ���1��ʹ���ж��ϱ�������ʱ�̣���ʼ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_MASK_GS_INT012_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_gs_rcv_lte      : 16; /* [15..0] �ж�״̬��־����bit������Ӧһ���ж����ͣ�����Ч��[0]: �ж�����0�� LTE����ģ����ǰ֪ͨGS��ϵͳ���������жϡ�[1]: �ж�����1�� LTE����ģ����GAP�жϣ�֪ͨGS������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2�� LTE����ģ����GAP�жϡ�[3]: �ж�����3��LTE����ģ�����������жϡ�[4]: �ж�����4��LTE����ģ��ǿ��GS�˳���ϵͳ�����жϡ�[5]: �ж�����5��LTE����ģ��֪ͨGS�ϱ����β��������[6]: ������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ���� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_GS_RCV_LTE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_gs_rcv_tds      : 16; /* [15..0] �ж�״̬��־0��1��2ֻ�е�TDS������ģʱ���ܲ�����[0]: �ж�����0�� TDS����ģ����ǰ֪ͨGS��ϵͳ���������жϡ�[1]: �ж�����1�� TDS����ģ����GAP�жϣ�֪ͨGS������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2�� TDS����ģ����GAP�жϡ�[3]: �ж�����3��TDS����ģ�����������жϡ�[4]: �ж�����4��TDS����ģ��ǿ��GS�˳���ϵͳ�����жϡ�[5]: �ж�����5��TDS����ģ��֪ͨGS�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ���� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_GS_RCV_TDS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_gs_rcv_gm       : 16; /* [15..0] �ж�״̬��־����bit������Ӧһ���ж����ͣ�����Ч��[0]: �ж�����0��GM����ģ����ǰ֪ͨGS��ϵͳ���������жϡ�[1]: �ж�����1��GM����ģ����GAP�жϣ�֪ͨGS������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2��GM����ģ����GAP�жϡ�[3]: �ж�����3��GM����ģ�����������жϡ�[4]: �ж�����4��GM����ģ��ǿ��GS�˳���ϵͳ�����жϡ�[5]: �ж�����5��GM����ģ��֪ͨGS�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ�����ж�״̬��־0��1��2ֻ�е�GM������ģʱ���ܲ����� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_GS_RCV_GM_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_gs_rcv_gs       : 16; /* [15..0] [0]: �ж�����0��GS����ģ����ǰ֪ͨGS��ϵͳ���������жϡ�[1]: �ж�����1��GS����ģ����GAP�жϣ�֪ͨGS������ϵͳ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2��GS����ģ����GAP�жϡ�[3]: �ж�����3��GS����ģ�����������жϡ�[4]: �ж�����4��GS����ģ��ǿ��GS�˳���ϵͳ�����жϡ�[5]: �ж�����5��GS����ģ��֪ͨGS�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ�����ж�״̬��־0��1��2ֻ�е�TDS������ģʱ���ܲ����� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_GS_RCV_GS_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_gs_rcv_w        : 16; /* [15..0] �ж�״̬��־����bit������Ӧһ���ж����ͣ�����Ч��[0]: �ж�����0��W����ģ����ǰ֪ͨGS��Ƶ���������жϡ�[1]: �ж�����1��W����ģ����GAP�жϣ�֪ͨGS������Ƶ������Ƶͨ�����ã������л��Ȳ�����[2]: �ж�����2��W����ģ����GAP�жϡ�[3]: �ж�����3��W����ģ������Ƶ�����������жϡ�[4]: �ж�����4��W����ģ��ǿ��GS�˳���Ƶ�����жϡ�[5]: �ж�����5��W����ģ��֪ͨGS�ϱ����β��������[6]: �ж�����6��������[15:7]: ��Ӧ�ж�����[15:7]��Ԥ�����ж�״̬��־0��1��2ֻ�е�W������ģʱ���ܲ����� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_GS_RCV_W_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    o_int_type_gs_int012       : 3; /* [2..0] ������ģ����int0��1��2���ϱ���(������Ƶ����ϵͳ����)��������Ч��[0]: GS��ģʱ��CTU����֪ͨGSDSP����int0�жϡ�[1]: GS��ģʱ��CTU����֪ͨGSDSP����int1�жϡ�[2]: GS��ģʱ��CTU����֪ͨGSDSP����int2�жϡ� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_INT_TYPE_GS_INT012_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0] ���� GM */
        unsigned int    gs_snd_lte_int_13          : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧgs_snd_lte_int_15 ~ 7��GS DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_lte_rcv_gs[15:7]����LTEDSP����Чʱ�̣�������Чbit6����Ӧgs_snd_lte_int_6��GSDSPͨ���˼Ĵ�����������6�жϣ�int_type_lte_rcv_gs[6]����LTEDSP����Чʱ�̣�������Чbit5����Ӧgs_snd_lte_int_5��GS��ģʱ֪ͨLTEDSP LTE�������ڵ������Ч��GS DSPͨ���˼Ĵ�����������5�жϣ�int_type_lte_rcv_gs[5]����LTEDSP����Чʱ�̣�������Чbit4����Ӧgs_snd_lte_int_4��GS��ģʱǿ���˳���ϵͳ����ָʾ������Ч��GS DSPͨ���˼Ĵ�����������4�жϣ�int_type_lte_rcv_gs[4]����LTEDSP����Чʱ�̣�������Чbit3����Ӧgs_snd_lte_int_3��GS��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_lte_rcv_gs[3]����LTEDSP��LTE��ģ�¶�GS������ϵͳ����ʱ�����GS����������GS DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GS_SND_LTE_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0] ���� */
        unsigned int    gs_snd_tds_int_13          : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧgs_snd_tds_int_15 ~ 7��GS DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_tds_rcv_gs[15:7]����TDSDSP����Чʱ�̣�������Чbit6����Ӧgs_snd_tds_int_6��GSDSPͨ���˼Ĵ�����������6�жϣ�int_type_tds_rcv_gs[6]����TDSDSP����Чʱ�̣�������Чbit5����Ӧgs_snd_tds_int_5��GS��ģʱ֪ͨTDSDSP TDS�������ڵ������Ч��GS DSPͨ���˼Ĵ�����������5�жϣ�int_type_tds_rcv_gs[5]����TDSDSP����Чʱ�̣�������Чbit4����Ӧgs_snd_tds_int_4��GS��ģʱǿ���˳���ϵͳ����ָʾ������Ч��GS DSPͨ���˼Ĵ�����������4�жϣ�int_type_tds_rcv_gs[4]����TDSDSP����Чʱ�̣�������Чbit3����Ӧgs_snd_tds_int_3��GS��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_tds_rcv_gs[3]����TDSDSP��TDS��ģ�¶�GS������ϵͳ����ʱ�����GS����������GS DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GS_SND_TDS_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0]  */
        unsigned int    gs_snd_gm_int_13           : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧgs_snd_gm_int_15 ~ 7��GS DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_gm_rcv_gs[15:7]����GMDSP����Чʱ�̣�������Чbit6����Ӧgs_snd_gm_int_6��GSDSPͨ���˼Ĵ�����������6�жϣ�int_type_gm_rcv_gs[6]����GMDSP����Чʱ�̣�������Чbit5����Ӧgs_snd_gm_int_5��GS��ģʱ֪ͨGMDSP GM�������ڵ������Ч��GS DSPͨ���˼Ĵ�����������5�жϣ�int_type_gm_rcv_gs[5]����GMDSP����Чʱ�̣�������Чbit4����Ӧgs_snd_gm_int_4��GS��ģʱǿ���˳���ϵͳ����ָʾ������Ч��GS DSPͨ���˼Ĵ�����������4�жϣ�int_type_gm_rcv_gs[4]����GMDSP����Чʱ�̣�������Чbit3����Ӧgs_snd_gm_int_3��GS��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_gm_rcv_gs[3]����GMDSP��GM��ģ�¶�GS������ϵͳ����ʱ�����GS����������GS DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GS_SND_GM_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0]  */
        unsigned int    gs_snd_gs_int_13           : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧgs_snd_gs_int_15 ~ 7��GS DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_gs_rcv_gs[15:7]����GSDSP����Чʱ�̣�������Чbit6����Ӧgs_snd_gs_int_6��GSDSPͨ���˼Ĵ�����������6�жϣ�int_type_gs_rcv_gs[6]����GSDSP����Чʱ�̣�������Чbit5����Ӧgs_snd_gs_int_5��GS��ģʱ֪ͨGSDSP GS�������ڵ������Ч��GS DSPͨ���˼Ĵ�����������5�жϣ�int_type_gs_rcv_gs[5]����GSDSP����Чʱ�̣�������Чbit4����Ӧgs_snd_gs_int_4��GS��ģʱǿ���˳���ϵͳ����ָʾ������Ч��GS DSPͨ���˼Ĵ�����������4�жϣ�int_type_gs_rcv_gs[4]����GSDSP����Чʱ�̣�������Чbit3����Ӧgs_snd_gs_int_3��GS��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_gs_rcv_gs[3]����GSDSP��GS��ģ�¶�GS������ϵͳ����ʱ�����GS����������GS DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GS_SND_GS_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 3; /* [2..0]  */
        unsigned int    gs_snd_w_int_13            : 13; /* [15..3] �߼������㡣����Ч��bit15~7����Ӧgs_snd_w_int_15 ~ 7��GS DSPͨ���˼Ĵ�����������15~7�жϣ�int_type_w_rcv_gs[15:7]����WDSP����Чʱ�̣�������Чbit6����Ӧgs_snd_w_int_6��GSDSPͨ���˼Ĵ�����������6�жϣ�int_type_w_rcv_gs[6]����WDSP����Чʱ�̣�������Чbit5����Ӧgs_snd_w_int_5��GS��ģʱ֪ͨWDSP W�������ڵ������Ч��GS DSPͨ���˼Ĵ�����������5�жϣ�int_type_w_rcv_gs[5]����WDSP����Чʱ�̣�������Чbit4����Ӧgs_snd_w_int_4��GS��ģʱǿ���˳���ϵͳ����ָʾ������Ч��GS DSPͨ���˼Ĵ�����������4�жϣ�int_type_w_rcv_gs[4]����WDSP����Чʱ�̣�������Чbit3����Ӧgs_snd_w_int_3��GS��������ָʾ������Ч�����ڴ����ж�����3 ��int_type_w_rcv_gs[3]����WDSP��W��ģ�¶�GS������ϵͳ����ʱ�����GS����������GS DSP���ô˱�־Ϊ1�� ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GS_SND_W_INT_13_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    gbbp1_19m_sel              : 2; /* [1..0] V9R1�汾����������á� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GBBP1_19M_SEL_T;

typedef union
{
    struct
    {
        unsigned int    gbbp2_19m_sel              : 2; /* [1..0] V9R1�汾����������á� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GBBP2_19M_SEL_T;

typedef union
{
    struct
    {
        unsigned int    wbbp_19m_sel               : 2; /* [1..0] V9R1�汾����������á� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_WBBP_19M_SEL_T;

typedef union
{
    struct
    {
        unsigned int    lbbp_19m_sel               : 2; /* [1..0] V9R1�汾����������á� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_LBBP_19M_SEL_T;

typedef union
{
    struct
    {
        unsigned int    tbbp_19m_sel               : 2; /* [1..0] V9R1�汾����������á� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TBBP1_19M_SEL_T;

typedef union
{
    struct
    {
        unsigned int    gbbp1_clk_sel              : 2; /* [1..0] V9R1�汾����������á� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GBBP1_CLK_SEL_T;

typedef union
{
    struct
    {
        unsigned int    gbbp2_clk_sel              : 2; /* [1..0] V9R1�汾����������á� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GBBP2_CLK_SEL_T;

typedef union
{
    struct
    {
        unsigned int    wbbp_clk_sel               : 2; /* [1..0] V9R1�汾����������á� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_WBBP_CLK_SEL_T;

typedef union
{
    struct
    {
        unsigned int    lbbp_clk_sel               : 2; /* [1..0] V9R1�汾����������á� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_LBBP_CLK_SEL_T;

typedef union
{
    struct
    {
        unsigned int    tbbp_clk_sel               : 2; /* [1..0] V9R1�汾����������á� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TBBP_CLK_SEL_T;

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 1; /* [0..0] ���� */
        unsigned int    lte_rxb_iq_exchange        : 1; /* [1..1] 1'b1��LTE��RXBͨ·IQ·���ݽ���1'b0��LTE��RXBͨ·IQ·���ݲ����� */
        unsigned int    lte_rxb_q_inv              : 1; /* [2..2] 1'b1��LTE��RXBͨ·Q·����ȡ��1'b0��LTE��RXBͨ·Q·���ݲ�ȡ�� */
        unsigned int    lte_rxb_i_inv              : 1; /* [3..3] 1'b1��LTE��RXBͨ·I·����ȡ��1'b0��LTE��RXBͨ·I·���ݲ�ȡ�� */
        unsigned int    lte_rxa_iq_exchange        : 1; /* [4..4] 1'b1��LTE��RXAͨ·IQ·���ݽ���1'b0��LTE��RXAͨ·IQ·���ݲ����� */
        unsigned int    lte_rxa_q_inv              : 1; /* [5..5] 1'b1��LTE��RXAͨ·Q·����ȡ��1'b0��LTE��RXAͨ·Q·���ݲ�ȡ�� */
        unsigned int    lte_rxa_i_inv              : 1; /* [6..6] 1'b1��LTE��RXAͨ·I·����ȡ��1'b0��LTE��RXAͨ·I·���ݲ�ȡ�� */
        unsigned int    lte_tx_iq_exchange         : 1; /* [7..7] 1'b1��LTE��TXͨ·IQ·���ݽ���1'b0��LTE��TXͨ·IQ·���ݲ����� */
        unsigned int    lte_tx_q_inv               : 1; /* [8..8] 1'b1��LTE��TXͨ·Q·����ȡ��1'b0��LTE��TXͨ·Q·���ݲ�ȡ�� */
        unsigned int    lte_tx_i_inv               : 1; /* [9..9] 1'b1��LTE��TXͨ·I·����ȡ��1'b0��LTE��TXͨ·I·���ݲ�ȡ�� */
        unsigned int    reserved_0                 : 22; /* [31..10] ��������������Ϊ�����ã�Ĭ��ֵΪ0. */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_LTE_ABBIF_FMT_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 1; /* [0..0] ���� */
        unsigned int    w_rxb_iq_exchange          : 1; /* [1..1] 1'b1��W��RXBͨ·IQ·���ݽ���1'b0��W��RXBͨ·IQ·���ݲ����� */
        unsigned int    w_rxb_q_inv                : 1; /* [2..2] 1'b1��W��RXBͨ·Q·����ȡ��1'b0��W��RXBͨ·Q·���ݲ�ȡ�� */
        unsigned int    w_rxb_i_inv                : 1; /* [3..3] 1'b1��W��RXBͨ·I·����ȡ��1'b0��W��RXBͨ·I·���ݲ�ȡ�� */
        unsigned int    w_rxa_iq_exchange          : 1; /* [4..4] 1'b1��W��RXAͨ·IQ·���ݽ���1'b0��W��RXAͨ·IQ·���ݲ����� */
        unsigned int    w_rxa_q_inv                : 1; /* [5..5] 1'b1��W��RXAͨ·Q·����ȡ��1'b0��W��RXAͨ·Q·���ݲ�ȡ�� */
        unsigned int    w_rxa_i_inv                : 1; /* [6..6] 1'b1��W��RXAͨ·I·����ȡ��1'b0��W��RXAͨ·I·���ݲ�ȡ�� */
        unsigned int    w_tx_iq_exchange           : 1; /* [7..7] 1'b1��W��TXͨ·IQ·���ݽ���1'b0��W��TXͨ·IQ·���ݲ����� */
        unsigned int    w_tx_q_inv                 : 1; /* [8..8] 1'b1��W��TXͨ·Q·����ȡ��1'b0��W��TXͨ·Q·���ݲ�ȡ�� */
        unsigned int    w_tx_i_inv                 : 1; /* [9..9] 1'b1��W��TXͨ·I·����ȡ��1'b0��W��TXͨ·I·���ݲ�ȡ���� */
        unsigned int    reserved_0                 : 22; /* [31..10] ��������������Ϊ�����ã�Ĭ��ֵΪ0. */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_W_ABBIF_FMT_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 1; /* [0..0] ���� */
        unsigned int    tds_rxb_iq_exchange        : 1; /* [1..1] 1'b1��TDS��RXBͨ·IQ·���ݽ���1'b0��TDS��RXBͨ·IQ·���ݲ����� */
        unsigned int    tds_rxb_q_inv              : 1; /* [2..2] 1'b1��TDS��RXBͨ·Q·����ȡ��1'b0��TDS��RXBͨ·Q·���ݲ�ȡ�� */
        unsigned int    tds_rxb_i_inv              : 1; /* [3..3] 1'b1��TDS��RXBͨ·I·����ȡ��1'b0��TDS��RXBͨ·I·���ݲ�ȡ�� */
        unsigned int    tds_rxa_iq_exchange        : 1; /* [4..4] 1'b1��TDS��RXAͨ·IQ·���ݽ���1'b0��TDS��RXAͨ·IQ·���ݲ����� */
        unsigned int    tds_rxa_q_inv              : 1; /* [5..5] 1'b1��TDS��RXAͨ·Q·����ȡ��1'b0��TDS��RXAͨ·Q·���ݲ�ȡ�� */
        unsigned int    tds_rxa_i_inv              : 1; /* [6..6] 1'b1��TDS��RXAͨ·I·����ȡ��1'b0��TDS��RXAͨ·I·���ݲ�ȡ�� */
        unsigned int    tds_tx_iq_exchange         : 1; /* [7..7] 1'b1��TDS��TXͨ·IQ·���ݽ���1'b0��TDS��TXͨ·IQ·���ݲ����� */
        unsigned int    tds_tx_q_inv               : 1; /* [8..8] 1'b1��TDS��TXͨ·Q·����ȡ��1'b0��TDS��TXͨ·Q·���ݲ�ȡ�� */
        unsigned int    tds_tx_i_inv               : 1; /* [9..9] 1'b1��TDS��TXͨ·I·����ȡ��1'b0��TDS��TXͨ·I·���ݲ�ȡ�� */
        unsigned int    reserved_0                 : 22; /* [31..10] ��������������Ϊ�����ã�Ĭ��ֵΪ0. */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_TDS_ABBIF_FMT_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 1; /* [0..0] ���� */
        unsigned int    gm_rxb_iq_exchange         : 1; /* [1..1] 1'b1��GM��RXBͨ·IQ·���ݽ���1'b0��GM��RXBͨ·IQ·���ݲ����� */
        unsigned int    gm_rxb_q_inv               : 1; /* [2..2] 1'b1��GM��RXBͨ·Q·����ȡ��1'b0��GM��RXBͨ·Q·���ݲ�ȡ�� */
        unsigned int    gm_rxb_i_inv               : 1; /* [3..3] 1'b1��GM��RXBͨ·I·����ȡ��1'b0��GM��RXBͨ·I·���ݲ�ȡ�� */
        unsigned int    gm_rxa_iq_exchange         : 1; /* [4..4] 1'b1��GM��RXAͨ·IQ·���ݽ���1'b0��GM��RXAͨ·IQ·���ݲ����� */
        unsigned int    gm_rxa_q_inv               : 1; /* [5..5] 1'b1��GM��RXAͨ·Q·����ȡ��1'b0��GM��RXAͨ·Q·���ݲ�ȡ�� */
        unsigned int    gm_rxa_i_inv               : 1; /* [6..6] 1'b1��GM��RXAͨ·I·����ȡ��1'b0��GM��RXAͨ·I·���ݲ�ȡ�� */
        unsigned int    gm_tx_iq_exchange          : 1; /* [7..7] 1'b1��GM��TXͨ·IQ·���ݽ���1'b0��GM��TXͨ·IQ·���ݲ����� */
        unsigned int    gm_tx_q_inv                : 1; /* [8..8] 1'b1��GM��TXͨ·Q·����ȡ��1'b0��GM��TXͨ·Q·���ݲ�ȡ�� */
        unsigned int    gm_tx_i_inv                : 1; /* [9..9] 1'b1��GM��TXͨ·I·����ȡ��1'b0��GM��TXͨ·I·���ݲ�ȡ�� */
        unsigned int    reserved_0                 : 22; /* [31..10] ��������������Ϊ�����ã�Ĭ��ֵΪ0. */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GM_ABBIF_FMT_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reserved_1                 : 1; /* [0..0] ���� */
        unsigned int    gs_rxb_iq_exchange         : 1; /* [1..1] 1'b1��GS��RXBͨ·IQ·���ݽ���1'b0��GS��RXBͨ·IQ·���ݲ����� */
        unsigned int    gs_rxb_q_inv               : 1; /* [2..2] 1'b1��GS��RXBͨ·Q·����ȡ��1'b0��GS��RXBͨ·Q·���ݲ�ȡ�� */
        unsigned int    gs_rxb_i_inv               : 1; /* [3..3] 1'b1��GS��RXBͨ·I·����ȡ��1'b0��GS��RXBͨ·I·���ݲ�ȡ�� */
        unsigned int    gs_rxa_iq_exchange         : 1; /* [4..4] 1'b1��GS��RXAͨ·IQ·���ݽ���1'b0��GS��RXAͨ·IQ·���ݲ����� */
        unsigned int    gs_rxa_q_inv               : 1; /* [5..5] 1'b1��GS��RXAͨ·Q·����ȡ��1'b0��GS��RXAͨ·Q·���ݲ�ȡ�� */
        unsigned int    gs_rxa_i_inv               : 1; /* [6..6] 1'b1��GS��RXAͨ·I·����ȡ��1'b0��GS��RXAͨ·I·���ݲ�ȡ�� */
        unsigned int    gs_tx_iq_exchange          : 1; /* [7..7] 1'b1��GS��TXͨ·IQ·���ݽ���1'b0��GS��TXͨ·IQ·���ݲ����� */
        unsigned int    gs_tx_q_inv                : 1; /* [8..8] 1'b1��GS��TXͨ·Q·����ȡ��1'b0��GS��TXͨ·Q·���ݲ�ȡ�� */
        unsigned int    gs_tx_i_inv                : 1; /* [9..9] 1'b1��GS��TXͨ·I·����ȡ��1'b0��GS��TXͨ·I·���ݲ�ȡ�� */
        unsigned int    reserved_0                 : 22; /* [31..10] ��������������Ϊ�����ã�Ĭ��ֵΪ0. */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GS_ABBIF_FMT_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg0_for_use               : 32; /* [31..0] �Ĵ���3��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG0_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg1_for_use               : 32; /* [31..0] �Ĵ���4��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG1_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg2_for_use               : 32; /* [31..0] �Ĵ���4��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG2_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg3_for_use               : 32; /* [31..0] �Ĵ���6��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG3_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg4_for_use               : 32; /* [31..0] �Ĵ���7��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG4_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg5_for_use               : 32; /* [31..0] �Ĵ���8��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG5_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg6_for_use               : 32; /* [31..0] �Ĵ���9��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG6_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg7_for_use               : 32; /* [31..0] �Ĵ���7��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG7_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg8_for_use               : 32; /* [31..0] �Ĵ���8��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG8_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg9_for_use               : 32; /* [31..0] �Ĵ���9��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG9_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg10_for_use              : 32; /* [31..0] �Ĵ���10��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG10_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg11_for_use              : 32; /* [31..0] �Ĵ���11��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG11_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg12_for_use              : 32; /* [31..0] �Ĵ���12��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG12_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg13_for_use              : 32; /* [31..0] �Ĵ���13��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG13_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg14_for_use              : 32; /* [31..0] �Ĵ���14��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG14_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg15_for_use              : 32; /* [31..0] �Ĵ���15��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG15_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg16_for_use              : 32; /* [31..0] �Ĵ���16��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG16_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg17_for_use              : 32; /* [31..0] �Ĵ���17��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG17_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg18_for_use              : 32; /* [31..0] �Ĵ���18��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG18_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg19_for_use              : 32; /* [31..0] �Ĵ���19��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG19_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg20_for_use              : 32; /* [31..0] �Ĵ���20��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG20_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg21_for_use              : 32; /* [31..0] �Ĵ���21��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG21_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg22_for_use              : 32; /* [31..0] �Ĵ���22��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG22_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg23_for_use              : 32; /* [31..0] �Ĵ���23��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG23_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg24_for_use              : 32; /* [31..0] �Ĵ���24��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG24_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg25_for_use              : 32; /* [31..0] �Ĵ���25��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG25_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg26_for_use              : 32; /* [31..0] �Ĵ���26��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG26_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg27_for_use              : 32; /* [31..0] �Ĵ���27��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG27_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg28_for_use              : 32; /* [31..0] �Ĵ���28��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG28_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg29_for_use              : 32; /* [31..0] �Ĵ���27��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG29_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg30_for_use              : 32; /* [31..0] �Ĵ���28��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG30_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg31_for_use              : 32; /* [31..0] �Ĵ���28��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG31_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg32_for_use              : 32; /* [31..0] �Ĵ���32��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG32_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg33_for_use              : 32; /* [31..0] �Ĵ���33��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG33_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg34_for_use              : 32; /* [31..0] �Ĵ���34��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG34_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg35_for_use              : 32; /* [31..0] �Ĵ���35��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG35_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg36_for_use              : 32; /* [31..0] �Ĵ���36��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG36_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg37_for_use              : 32; /* [31..0] �Ĵ���37��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG37_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg38_for_use              : 32; /* [31..0] �Ĵ���38��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG38_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg39_for_use              : 32; /* [31..0] �Ĵ���39��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG39_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg40_for_use              : 32; /* [31..0] �Ĵ���40��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG40_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg41_for_use              : 32; /* [31..0] �Ĵ���41��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG41_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg42_for_use              : 32; /* [31..0] �Ĵ���42��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG42_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg43_for_use              : 32; /* [31..0] �Ĵ���43��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG43_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg44_for_use              : 32; /* [31..0] �Ĵ���44��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG44_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg45_for_use              : 32; /* [31..0] �Ĵ���45��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG45_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg46_for_use              : 32; /* [31..0] �Ĵ���46��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG46_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    reg47_for_use              : 32; /* [31..0] �Ĵ���47��Ԥ������DSP֮�䴫����Ϣ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_REG47_FOR_USE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg0_cpu               : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG0_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg1_cpu               : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG1_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg2_cpu               : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG2_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg3_cpu               : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG3_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg4_cpu               : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG4_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg5_cpu               : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG5_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg6_cpu               : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG6_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg7_cpu               : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG7_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg8_cpu               : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG8_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg9_cpu               : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG9_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg10_cpu              : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG10_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg11_cpu              : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG11_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg12_cpu              : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG12_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg13_cpu              : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG13_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg14_cpu              : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG14_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg15_cpu              : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG15_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg16_cpu              : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG16_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg17_cpu              : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG17_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg18_cpu              : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG18_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_reg19_cpu              : 32; /* [31..0]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG19_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_dbg_clk_gate_en        : 1; /* [0..0] bbp_comm_dbgģ���ſ�ʱ��ʹ���źţ�1��ֱ��ʹ������ʱ�ӣ�δ�����ſأ�0������ʱ����Ч����ʱ���е͹���״̬�� */
        unsigned int    dsp_dbg_clk_bypass         : 1; /* [1..1] bbp_comm_dbgģ��ʱ��bypass�����źţ�1��ֱ��ʹ������ʱ�ӣ��������ſش�����ʱdsp_dbg_clk_gate_en������Ч��0����ʾ����dsp_dbg_clk_gate_en�����ſش��� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_CLK_GATE_EN_T;    /* �ſ�ʱ�����üĴ��� */

typedef union
{
    struct
    {
        unsigned int    com2tds_busy_rf_ssi1       : 1; /* [0..0] TDS SSI1æ��־�� */
        unsigned int    com2tds_busy_rf_ssi0       : 1; /* [1..1] TDS SSI0æ��־�� */
        unsigned int    com2w_busy_rf_ssi1         : 1; /* [2..2] W SSI1æ��־�� */
        unsigned int    com2w_busy_rf_ssi0         : 1; /* [3..3] W SSI0æ��־�� */
        unsigned int    com2lte_busy_rf_ssi1       : 1; /* [4..4] LTE SSI1æ��־�� */
        unsigned int    com2lte_busy_rf_ssi0       : 1; /* [5..5] LTE SSI0æ��־�� */
        unsigned int    com2g2_busy_rf_ssi1        : 1; /* [6..6] G2 SSI1æ��־�� */
        unsigned int    com2g2_busy_rf_ssi0        : 1; /* [7..7] G2 SSI0æ��־�� */
        unsigned int    com2g1_busy_rf_ssi1        : 1; /* [8..8] G1 SSI1æ��־�� */
        unsigned int    com2g1_busy_rf_ssi0        : 1; /* [9..9] G1 SSI0æ��־�� */
        unsigned int    com2tds_busy_mipi1         : 1; /* [10..10] TDS MIPI1æ��־�� */
        unsigned int    com2tds_busy_mipi0         : 1; /* [11..11] TDS MIPI0æ��־�� */
        unsigned int    com2w_busy_mipi1           : 1; /* [12..12] W MIPI1æ��־�� */
        unsigned int    com2w_busy_mipi0           : 1; /* [13..13] W MIPI0æ��־�� */
        unsigned int    com2lte_busy_mipi1         : 1; /* [14..14] LTE MIPI1æ��־�� */
        unsigned int    com2lte_busy_mipi0         : 1; /* [15..15] LTE MIPI0æ��־�� */
        unsigned int    com2g2_busy_mipi1          : 1; /* [16..16] G2 MIPI1æ��־�� */
        unsigned int    com2g2_busy_mipi0          : 1; /* [17..17] G2 MIPI0æ��־�� */
        unsigned int    com2g1_busy_mipi1          : 1; /* [18..18] G1 MIPI1æ��־�� */
        unsigned int    com2g1_busy_mipi0          : 1; /* [19..19] G1 MIPI0æ��־�� */
        unsigned int    reserved                   : 12; /* [31..20]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_REG20_CPU_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_comm_en                : 1; /* [0..0] bbp_comm_dbgʹ���źţ�1��ʹ�ܣ�0����ʹ�ܡ� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_COMM_EN_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    l_ch_sw_rf                 : 1; /* [0..0] rf����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    l_ch_sw_mipi               : 1; /* [1..1] mipi����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    l_ch_sw_abb_tx             : 1; /* [2..2] abb tx����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    l_ch_sw_abb_rx             : 1; /* [3..3] abb rx����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    l_ch_sw_pmu                : 1; /* [4..4] pmu����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    l_ch_sw_apt                : 1; /* [5..5] apt����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    reserved                   : 26; /* [31..6] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_L_CH_SW_T;

typedef union
{
    struct
    {
        unsigned int    w_ch_sw_rf                 : 1; /* [0..0] rf����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    w_ch_sw_mipi               : 1; /* [1..1] mipi����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    w_ch_sw_abb_tx             : 1; /* [2..2] abb tx����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    w_ch_sw_abb_rx             : 1; /* [3..3] abb rx����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    w_ch_sw_pmu                : 1; /* [4..4] pmu����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    w_ch_sw_apt                : 1; /* [5..5] apt����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    reserved                   : 26; /* [31..6] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_W_CH_SW_T;

typedef union
{
    struct
    {
        unsigned int    t_ch_sw_rf                 : 1; /* [0..0] rf����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    t_ch_sw_mipi               : 1; /* [1..1] mipi����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    t_ch_sw_abb_tx             : 1; /* [2..2] abb tx����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    t_ch_sw_abb_rx             : 1; /* [3..3] abb rx����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    t_ch_sw_pmu                : 1; /* [4..4] pmu����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    t_ch_sw_apt                : 1; /* [5..5] apt����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    reserved                   : 26; /* [31..6] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_T_CH_SW_T;

typedef union
{
    struct
    {
        unsigned int    gm_ch_sw_rf                : 1; /* [0..0] rf����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    gm_ch_sw_mipi              : 1; /* [1..1] mipi����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    gm_ch_sw_abb_tx            : 1; /* [2..2] abb tx����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    gm_ch_sw_abb_rx            : 1; /* [3..3] abb rx����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    gm_ch_sw_pmu               : 1; /* [4..4] pmu����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    gm_ch_sw_apt               : 1; /* [5..5] apt����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    reserved                   : 26; /* [31..6] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GM_CH_SW_T;

typedef union
{
    struct
    {
        unsigned int    gs_ch_sw_rf                : 1; /* [0..0] rf����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    gs_ch_sw_mipi              : 1; /* [1..1] mipi����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    gs_ch_sw_abb_tx            : 1; /* [2..2] abb tx����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    gs_ch_sw_abb_rx            : 1; /* [3..3] abb rx����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    gs_ch_sw_pmu               : 1; /* [4..4] pmu����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    gs_ch_sw_apt               : 1; /* [5..5] apt����ź��Ƿ񽻻���1'b0��0/1ͨ��������1'b1��0/1ͨ������ */
        unsigned int    reserved                   : 26; /* [31..6] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GS_CH_SW_T;

typedef union
{
    struct
    {
        unsigned int    l_tcvr_sel0                : 1; /* [0..0] lteģ�����ͨ��0��tcvr_on�ź���Դ��1'b0��������lte bbp1'b1��������dsp����ֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_L_TCVR_SEL0_T;

typedef union
{
    struct
    {
        unsigned int    l_tcvr_sel1                : 1; /* [0..0] lteģ�����ͨ��1��tcvr_on�ź���Դ��1'b0��������lte bbp1'b1��������dsp����ֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_L_TCVR_SEL1_T;

typedef union
{
    struct
    {
        unsigned int    t_tcvr_sel0                : 1; /* [0..0] tdsģ�����ͨ��0��tcvr_on�ź���Դ��1'b0��������tds bbp1'b1��������dsp����ֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_T_TCVR_SEL0_T;

typedef union
{
    struct
    {
        unsigned int    t_tcvr_sel1                : 1; /* [0..0] tdsģ�����ͨ��1��tcvr_on�ź���Դ��1'b0��������tds bbp1'b1��������dsp����ֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_T_TCVR_SEL1_T;

typedef union
{
    struct
    {
        unsigned int    w_tcvr_sel0                : 1; /* [0..0] wcdmaģ�����ͨ��0��tcvr_on�ź���Դ��1'b0��������wcdma bbp1'b1��������dsp����ֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_W_TCVR_SEL0_T;

typedef union
{
    struct
    {
        unsigned int    w_tcvr_sel1                : 1; /* [0..0] wcdmaģ�����ͨ��1��tcvr_on�ź���Դ����1'b0��������wcdma bbp1'b1��������dsp����ֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_W_TCVR_SEL1_T;

typedef union
{
    struct
    {
        unsigned int    gm_tcvr_sel0               : 1; /* [0..0] gsm��m�������ͨ��0��tcvr_on�ź���Դ��1'b0��������gsm��m�� bbp1'b1��������dsp����ֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GM_TCVR_SEL0_T;

typedef union
{
    struct
    {
        unsigned int    gm_tcvr_sel1               : 1; /* [0..0] gsm��m�������ͨ��1��tcvr_on�ź���Դ��1'b0��������gsm��m�� bbp1'b1��������dsp����ֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GM_TCVR_SEL1_T;

typedef union
{
    struct
    {
        unsigned int    gs_tcvr_sel0               : 1; /* [0..0] gsm��s�������ͨ��0��tcvr_on�ź���Դ��1'b0��������gsm��s�� bbp1'b1��������dsp����ֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GS_TCVR_SEL0_T;

typedef union
{
    struct
    {
        unsigned int    gs_tcvr_sel1               : 1; /* [0..0] gsm��s�������ͨ��1��tcvr_on�ź���Դ��1'b0��������gsm��s�� bbp1'b1��������dsp����ֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GS_TCVR_SEL1_T;

typedef union
{
    struct
    {
        unsigned int    l_tcvr_value0              : 1; /* [0..0] dsp���õ�lteģ�͸�ͨ��0 ��tcvr_onֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_L_TCVR_VALUE0_T;

typedef union
{
    struct
    {
        unsigned int    l_tcvr_value1              : 1; /* [0..0] dsp���õ�lteģ�͸�ͨ��1 ��tcvr_onֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_L_TCVR_VALUE1_T;

typedef union
{
    struct
    {
        unsigned int    t_tcvr_value0              : 1; /* [0..0] dsp���õ�tdsģ�͸�ͨ��0 ��tcvr_onֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_T_TCVR_VALUE0_T;

typedef union
{
    struct
    {
        unsigned int    t_tcvr_value1              : 1; /* [0..0] dsp���õ�tdsģ�͸�ͨ��1 ��tcvr_onֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_T_TCVR_VALUE1_T;

typedef union
{
    struct
    {
        unsigned int    w_tcvr_value0              : 1; /* [0..0] dsp���õ�wcdmaģ�͸�ͨ��0 ��tcvr_onֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_W_TCVR_VALUE0_T;

typedef union
{
    struct
    {
        unsigned int    w_tcvr_value1              : 1; /* [0..0] dsp���õ�wcdmaģ�͸�ͨ��1 ��tcvr_onֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_W_TCVR_VALUE1_T;

typedef union
{
    struct
    {
        unsigned int    gm_tcvr_value0             : 1; /* [0..0] dsp���õ�gsm��m��ģ�͸�ͨ��0 ��tcvr_onֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GM_TCVR_VALUE0_T;

typedef union
{
    struct
    {
        unsigned int    gm_tcvr_value1             : 1; /* [0..0] dsp���õ�gsm��m��ģ�͸�ͨ��1��tcvr_onֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GM_TCVR_VALUE1_T;

typedef union
{
    struct
    {
        unsigned int    gs_tcvr_value0             : 1; /* [0..0] dsp���õ�gsm��s��ģ�͸�ͨ��0 ��tcvr_onֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GS_TCVR_VALUE0_T;

typedef union
{
    struct
    {
        unsigned int    gs_tcvr_value1             : 1; /* [0..0] dsp���õ�gsm��s��ģ�͸�ͨ��1 ��tcvr_onֵ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_GS_TCVR_VALUE1_T;

typedef union
{
    struct
    {
        unsigned int    l_tcvr_state0              : 1; /* [0..0] lte bbp�����ͨ��0 tcvr_on�ź�״̬ */
        unsigned int    l_tcvr_state1              : 1; /* [1..1] lte bbp�����ͨ��1 tcvr_on�ź�״̬ */
        unsigned int    t_tcvr_state0              : 1; /* [2..2] tds bbp�����ͨ��0 tcvr_on�ź�״̬ */
        unsigned int    reserved_4                 : 1; /* [3..3] ���� */
        unsigned int    w_tcvr_state0              : 1; /* [4..4] wcdma bbp�����ͨ��0 tcvr_on�ź�״̬ */
        unsigned int    reserved_3                 : 1; /* [5..5] ���� */
        unsigned int    gm_tcvr_state0             : 1; /* [6..6] gsm��m�� bbp�����ͨ��0 tcvr_on�ź�״̬ */
        unsigned int    reserved_2                 : 1; /* [7..7] ���� */
        unsigned int    gs_tcvr_state0             : 1; /* [8..8] gsm��s�� bbp�����ͨ��0 tcvr_on�ź�״̬ */
        unsigned int    reserved_1                 : 7; /* [15..9] ���� */
        unsigned int    ch_tcvr_state0             : 1; /* [16..16] ͨ��0 ���������tcvr_on�ź�״̬ */
        unsigned int    ch_tcvr_state1             : 1; /* [17..17] ͨ��1 ���������tcvr_on�ź�״̬ */
        unsigned int    reserved_0                 : 14; /* [31..18]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_RF_TCVR_STATE_T;

typedef union
{
    struct
    {
        unsigned int    mipi_unbind_en             : 1; /* [0..0] mipiͨ������ʹ�ܣ�1'b0��ͨ����1'b1��ͨ������ */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_MIPI_UNBIND_EN_T;

typedef union
{
    struct
    {
        unsigned int    mipi_ch_sel0               : 1; /* [0..0] ͨ��0ӳ��mipi������ϵ��1'b0��ͨ��0ӳ�䵽mipi01'b1��ͨ��0ӳ�䵽mipi1 */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_MIPI_CH_SEL0_T;

typedef union
{
    struct
    {
        unsigned int    mipi_ch_sel1               : 1; /* [0..0] ͨ��1ӳ��mipi������ϵ��1'b0��ͨ��1ӳ�䵽mipi01'b1��ͨ��1ӳ�䵽mipi1 */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_MIPI_CH_SEL1_T;

typedef union
{
    struct
    {
        unsigned int    apt_ch_sel0                : 1; /* [0..0] ���� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_APT_CH_SEL0_T;

typedef union
{
    struct
    {
        unsigned int    apt_ch_sel1                : 1; /* [0..0] ���� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_APT_CH_SEL1_T;

typedef union
{
    struct
    {
        unsigned int    afc_ch_sel0                : 1; /* [0..0] afc pdm0�ź���Դ��1'b0��������ͨ��01'b1��������ͨ��1 */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_AFC_CH_SEL0_T;

typedef union
{
    struct
    {
        unsigned int    afc_ch_sel1                : 1; /* [0..0] afc pdm1�ź���Դ��1'b0��������ͨ��01'b1��������ͨ��1 */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_AFC_CH_SEL1_T;

typedef union
{
    struct
    {
        unsigned int    abb_tx_ch_sel0             : 1; /* [0..0] ���� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_ABB_TX_CH_SEL0_T;

typedef union
{
    struct
    {
        unsigned int    abb_tx_ch_sel1             : 1; /* [0..0] ���� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_ABB_TX_CH_SEL1_T;

typedef union
{
    struct
    {
        unsigned int    pmu_ch_sel0                : 1; /* [0..0] ���� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_PMU_CH_SEL0_T;

typedef union
{
    struct
    {
        unsigned int    pmu_ch_sel1                : 1; /* [0..0] ���� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_PMU_CH_SEL1_T;

typedef union
{
    struct
    {
        unsigned int    valid_num                  : 16; /* [15..0] ���� */
        unsigned int    reserved                   : 16; /* [31..16]  */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_VALID_NUM_T;

typedef union
{
    struct
    {
        unsigned int    valid_num1                 : 16; /* [15..0] ���� */
        unsigned int    valid_num2                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_VALID_NUM1_T;

typedef union
{
    struct
    {
        unsigned int    dbg_en                     : 1; /* [0..0] ��ά�ɲ�ʹ���źţ���dbg_rpt_modeΪ0ʱ�߼����壬������Ҫд�塣1'b0����ά�ɲ⹦�ܲ�ʹ��                             1'b1����ά�ɲ⹦��ʹ�� */
        unsigned int    dbg_rpt_mode               : 1; /* [1..1] ��ά�ɲ������ϱ�ģʽ��'b0���������������dbg_pkg_num�����ݰ����Զ�������'b1��ֻҪdbg_enΪ1���ϱ���û���ϱ��������ơ� */
        unsigned int    reserved                   : 14; /* [15..2] ������ */
        unsigned int    dbg_pkg_num                : 16; /* [31..16] ��ά�ɲ�İ����� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_EN_T;

typedef union
{
    struct
    {
        unsigned int    dbg_id                     : 32; /* [31..0] ��ά�ɲ��ϱ����ݰ�ID�Ż�ַ */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_ID_T;

typedef union
{
    struct
    {
        unsigned int    dbg_flt                    : 16; /* [15..0] ��ά�ɲ��ڲ�����ָʾ��1���ϱ�ssiͨ��0����2���ϱ�ssiͨ��1����3���ϱ�mipiͨ��0����4���ϱ�mipiͨ��1�������������� */
        unsigned int    reserved                   : 16; /* [31..16] ������ */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_DBG_FLT_T;

typedef union
{
    struct
    {
        unsigned int    bbc_rev0                   : 32; /* [31..0] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_BBC_REV0_T;

typedef union
{
    struct
    {
        unsigned int    bbc_rev1                   : 32; /* [31..0] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_BBC_REV1_T;

typedef union
{
    struct
    {
        unsigned int    bbc_rev2                   : 32; /* [31..0] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_BBC_REV2_T;

typedef union
{
    struct
    {
        unsigned int    bbc_rev3                   : 32; /* [31..0] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_BBC_REV3_T;

typedef union
{
    struct
    {
        unsigned int    bbc_rev4                   : 32; /* [31..0] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_CTU_BBC_REV4_T;


/********************************************************************************/
/*    bbp_ctu ��������Ŀ��_ģ����_�Ĵ�����_��Ա��_set)        */
/********************************************************************************/
HI_SET_GET(hi_bbp_ctu_c0_primary_mode_ind_c0_primary_mode_ind,c0_primary_mode_ind,HI_BBP_CTU_C0_PRIMARY_MODE_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C0_PRIMARY_MODE_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_c0_primary_mode_ind_reserved,reserved,HI_BBP_CTU_C0_PRIMARY_MODE_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C0_PRIMARY_MODE_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_c0_second_mode_ind_c0_second_mode_ind,c0_second_mode_ind,HI_BBP_CTU_C0_SECOND_MODE_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C0_SECOND_MODE_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_c0_second_mode_ind_reserved,reserved,HI_BBP_CTU_C0_SECOND_MODE_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C0_SECOND_MODE_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_afc0_value_inter_rat_afc0_value_inter_rat,afc0_value_inter_rat,HI_BBP_CTU_AFC0_VALUE_INTER_RAT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_AFC0_VALUE_INTER_RAT_OFFSET)
HI_SET_GET(hi_bbp_ctu_c0_afc_pdm_select_mask_c0_afc_pdm_select_mask,c0_afc_pdm_select_mask,HI_BBP_CTU_C0_AFC_PDM_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C0_AFC_PDM_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c0_afc_pdm_select_mask_reserved,reserved,HI_BBP_CTU_C0_AFC_PDM_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C0_AFC_PDM_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c0_ant_switch_pa_select_mask_c0_ant_switch_pa_select_mask,c0_ant_switch_pa_select_mask,HI_BBP_CTU_C0_ANT_SWITCH_PA_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C0_ANT_SWITCH_PA_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c0_ant_switch_pa_select_mask_reserved,reserved,HI_BBP_CTU_C0_ANT_SWITCH_PA_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C0_ANT_SWITCH_PA_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c0_rfic_select_mask_c0_rfic_select_mask,c0_rfic_select_mask,HI_BBP_CTU_C0_RFIC_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C0_RFIC_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c0_rfic_select_mask_reserved,reserved,HI_BBP_CTU_C0_RFIC_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C0_RFIC_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c0_mipi_select_mask_c0_mipi_select_mask,c0_mipi_select_mask,HI_BBP_CTU_C0_MIPI_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C0_MIPI_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c0_mipi_select_mask_reserved,reserved,HI_BBP_CTU_C0_MIPI_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C0_MIPI_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c0_abb_select_mask_c0_abb_select_mask,c0_abb_select_mask,HI_BBP_CTU_C0_ABB_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C0_ABB_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c0_abb_select_mask_reserved,reserved,HI_BBP_CTU_C0_ABB_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C0_ABB_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c0_pmu_select_mask_c0_pmu_select_mask,c0_pmu_select_mask,HI_BBP_CTU_C0_PMU_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C0_PMU_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c0_pmu_select_mask_reserved,reserved,HI_BBP_CTU_C0_PMU_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C0_PMU_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_c0_sel_mask_dsp_c0_sel_mask,dsp_c0_sel_mask,HI_BBP_CTU_DSP_C0_SEL_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_C0_SEL_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_c0_sel_mask_reserved,reserved,HI_BBP_CTU_DSP_C0_SEL_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_C0_SEL_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_rfic0_rstn_dsp_rfic0_rstn,dsp_rfic0_rstn,HI_BBP_CTU_DSP_RFIC0_RSTN_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_RFIC0_RSTN_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_rfic0_rstn_reserved,reserved,HI_BBP_CTU_DSP_RFIC0_RSTN_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_RFIC0_RSTN_OFFSET)
HI_SET_GET(hi_bbp_ctu_c0_second_afc_mode_c0_second_afc_mode,c0_second_afc_mode,HI_BBP_CTU_C0_SECOND_AFC_MODE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C0_SECOND_AFC_MODE_OFFSET)
HI_SET_GET(hi_bbp_ctu_c0_second_afc_mode_reserved,reserved,HI_BBP_CTU_C0_SECOND_AFC_MODE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C0_SECOND_AFC_MODE_OFFSET)
HI_SET_GET(hi_bbp_ctu_bbp_sys_0control_bbp_sys_0control,bbp_sys_0control,HI_BBP_CTU_BBP_SYS_0CONTROL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_BBP_SYS_0CONTROL_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi0_rd_overtime_flag_dsp_mipi0_rd_overtime_flag_dsp,mipi0_rd_overtime_flag_dsp,HI_BBP_CTU_MIPI0_RD_OVERTIME_FLAG_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI0_RD_OVERTIME_FLAG_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi0_rd_overtime_flag_dsp_reserved,reserved,HI_BBP_CTU_MIPI0_RD_OVERTIME_FLAG_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI0_RD_OVERTIME_FLAG_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_ssi0_rd_overtime_flag_dsp_ssi0_rd_overtime_flag_dsp,ssi0_rd_overtime_flag_dsp,HI_BBP_CTU_SSI0_RD_OVERTIME_FLAG_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_SSI0_RD_OVERTIME_FLAG_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_ssi0_rd_overtime_flag_dsp_reserved,reserved,HI_BBP_CTU_SSI0_RD_OVERTIME_FLAG_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_SSI0_RD_OVERTIME_FLAG_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi0_rd_overtime_clr_mipi0_rd_overtime_clr_imi,mipi0_rd_overtime_clr_imi,HI_BBP_CTU_MIPI0_RD_OVERTIME_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI0_RD_OVERTIME_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi0_rd_overtime_clr_reserved,reserved,HI_BBP_CTU_MIPI0_RD_OVERTIME_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI0_RD_OVERTIME_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_ssi0_rd_overtime_clr_ssi0_rd_overtime_clr_imi,ssi0_rd_overtime_clr_imi,HI_BBP_CTU_SSI0_RD_OVERTIME_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_SSI0_RD_OVERTIME_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_ssi0_rd_overtime_clr_reserved,reserved,HI_BBP_CTU_SSI0_RD_OVERTIME_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_SSI0_RD_OVERTIME_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic0_1_rd_result_rfic0_1,rd_result_rfic0_1,HI_BBP_CTU_RD_RESULT_RFIC0_1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC0_1_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic0_2_rd_result_rfic0_2,rd_result_rfic0_2,HI_BBP_CTU_RD_RESULT_RFIC0_2_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC0_2_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic0_3_rd_result_rfic0_3,rd_result_rfic0_3,HI_BBP_CTU_RD_RESULT_RFIC0_3_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC0_3_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic0_4_rd_result_rfic0_4,rd_result_rfic0_4,HI_BBP_CTU_RD_RESULT_RFIC0_4_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC0_4_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic0_5_rd_result_rfic0_5,rd_result_rfic0_5,HI_BBP_CTU_RD_RESULT_RFIC0_5_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC0_5_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic0_6_rd_result_rfic0_6,rd_result_rfic0_6,HI_BBP_CTU_RD_RESULT_RFIC0_6_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC0_6_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic0_7_rd_result_rfic0_7,rd_result_rfic0_7,HI_BBP_CTU_RD_RESULT_RFIC0_7_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC0_7_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic0_8_rd_result_rfic0_8,rd_result_rfic0_8,HI_BBP_CTU_RD_RESULT_RFIC0_8_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC0_8_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic0_9_rd_result_rfic0_9,rd_result_rfic0_9,HI_BBP_CTU_RD_RESULT_RFIC0_9_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC0_9_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic0_10_rd_result_rfic0_10,rd_result_rfic0_10,HI_BBP_CTU_RD_RESULT_RFIC0_10_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC0_10_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic0_11_rd_result_rfic0_11,rd_result_rfic0_11,HI_BBP_CTU_RD_RESULT_RFIC0_11_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC0_11_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic0_12_rd_result_rfic0_12,rd_result_rfic0_12,HI_BBP_CTU_RD_RESULT_RFIC0_12_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC0_12_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic0_13_rd_result_rfic0_13,rd_result_rfic0_13,HI_BBP_CTU_RD_RESULT_RFIC0_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC0_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic0_14_rd_result_rfic0_14,rd_result_rfic0_14,HI_BBP_CTU_RD_RESULT_RFIC0_14_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC0_14_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic0_15_rd_result_rfic0_15,rd_result_rfic0_15,HI_BBP_CTU_RD_RESULT_RFIC0_15_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC0_15_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic0_16_rd_result_rfic0_16,rd_result_rfic0_16,HI_BBP_CTU_RD_RESULT_RFIC0_16_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC0_16_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_flag_rfic0_rd_result_flag_rfic0,rd_result_flag_rfic0,HI_BBP_CTU_RD_RESULT_FLAG_RFIC0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_FLAG_RFIC0_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_flag_rfic0_reserved,reserved,HI_BBP_CTU_RD_RESULT_FLAG_RFIC0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_FLAG_RFIC0_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_flag_rfic0_clr_rd_result_flag_rfic0_clr,rd_result_flag_rfic0_clr,HI_BBP_CTU_RD_RESULT_FLAG_RFIC0_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_FLAG_RFIC0_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_flag_rfic0_clr_reserved,reserved,HI_BBP_CTU_RD_RESULT_FLAG_RFIC0_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_FLAG_RFIC0_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi0_1_rd_result_mipi0_1,rd_result_mipi0_1,HI_BBP_CTU_RD_RESULT_MIPI0_1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI0_1_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi0_2_rd_result_mipi0_2,rd_result_mipi0_2,HI_BBP_CTU_RD_RESULT_MIPI0_2_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI0_2_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi0_3_rd_result_mipi0_3,rd_result_mipi0_3,HI_BBP_CTU_RD_RESULT_MIPI0_3_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI0_3_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi0_4_rd_result_mipi0_4,rd_result_mipi0_4,HI_BBP_CTU_RD_RESULT_MIPI0_4_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI0_4_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi0_5_rd_result_mipi0_5,rd_result_mipi0_5,HI_BBP_CTU_RD_RESULT_MIPI0_5_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI0_5_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi0_6_rd_result_mipi0_6,rd_result_mipi0_6,HI_BBP_CTU_RD_RESULT_MIPI0_6_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI0_6_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi0_7_rd_result_mipi0_7,rd_result_mipi0_7,HI_BBP_CTU_RD_RESULT_MIPI0_7_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI0_7_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi0_8_rd_result_mipi0_8,rd_result_mipi0_8,HI_BBP_CTU_RD_RESULT_MIPI0_8_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI0_8_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi0_9_rd_result_mipi0_9,rd_result_mipi0_9,HI_BBP_CTU_RD_RESULT_MIPI0_9_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI0_9_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi0_10_rd_result_mipi0_10,rd_result_mipi0_10,HI_BBP_CTU_RD_RESULT_MIPI0_10_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI0_10_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi0_11_rd_result_mipi0_11,rd_result_mipi0_11,HI_BBP_CTU_RD_RESULT_MIPI0_11_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI0_11_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi0_12_rd_result_mipi0_12,rd_result_mipi0_12,HI_BBP_CTU_RD_RESULT_MIPI0_12_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI0_12_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi0_13_rd_result_mipi0_13,rd_result_mipi0_13,HI_BBP_CTU_RD_RESULT_MIPI0_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI0_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi0_14_rd_result_mipi0_14,rd_result_mipi0_14,HI_BBP_CTU_RD_RESULT_MIPI0_14_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI0_14_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi0_15_rd_result_mipi0_15,rd_result_mipi0_15,HI_BBP_CTU_RD_RESULT_MIPI0_15_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI0_15_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi0_16_rd_result_mipi0_16,rd_result_mipi0_16,HI_BBP_CTU_RD_RESULT_MIPI0_16_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI0_16_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_flag_mipi0_rd_result_flag_mipi0,rd_result_flag_mipi0,HI_BBP_CTU_RD_RESULT_FLAG_MIPI0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_FLAG_MIPI0_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_flag_mipi0_reserved,reserved,HI_BBP_CTU_RD_RESULT_FLAG_MIPI0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_FLAG_MIPI0_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_flag_mipi0_clr_rd_result_flag_mipi0_clr,rd_result_flag_mipi0_clr,HI_BBP_CTU_RD_RESULT_FLAG_MIPI0_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_FLAG_MIPI0_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_flag_mipi0_clr_reserved,reserved,HI_BBP_CTU_RD_RESULT_FLAG_MIPI0_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_FLAG_MIPI0_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi0_rd_data_low_soft_mipi0_rd_data_low_soft,mipi0_rd_data_low_soft,HI_BBP_CTU_MIPI0_RD_DATA_LOW_SOFT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI0_RD_DATA_LOW_SOFT_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi0_rd_data_high_soft_mipi0_rd_data_high_soft,mipi0_rd_data_high_soft,HI_BBP_CTU_MIPI0_RD_DATA_HIGH_SOFT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI0_RD_DATA_HIGH_SOFT_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_end_flag_mipi0_soft_rd_end_flag_mipi0_soft,rd_end_flag_mipi0_soft,HI_BBP_CTU_RD_END_FLAG_MIPI0_SOFT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_END_FLAG_MIPI0_SOFT_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_end_flag_mipi0_soft_reserved,reserved,HI_BBP_CTU_RD_END_FLAG_MIPI0_SOFT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_END_FLAG_MIPI0_SOFT_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi0_grant_dsp_mipi0_grant_dsp,mipi0_grant_dsp,HI_BBP_CTU_MIPI0_GRANT_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI0_GRANT_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi0_grant_dsp_reserved,reserved,HI_BBP_CTU_MIPI0_GRANT_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI0_GRANT_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_mipi0_wdata_low_dsp_mipi0_wdata_low,dsp_mipi0_wdata_low,HI_BBP_CTU_DSP_MIPI0_WDATA_LOW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_MIPI0_WDATA_LOW_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_mipi0_wdata_high_dsp_mipi0_wdata_high,dsp_mipi0_wdata_high,HI_BBP_CTU_DSP_MIPI0_WDATA_HIGH_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_MIPI0_WDATA_HIGH_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_mipi0_en_imi_dsp_mipi0_en_imi,dsp_mipi0_en_imi,HI_BBP_CTU_DSP_MIPI0_EN_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_MIPI0_EN_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_mipi0_en_imi_reserved,reserved,HI_BBP_CTU_DSP_MIPI0_EN_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_MIPI0_EN_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_mipi0_cfg_ind_imi_dsp_mipi0_cfg_ind_imi,dsp_mipi0_cfg_ind_imi,HI_BBP_CTU_DSP_MIPI0_CFG_IND_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_MIPI0_CFG_IND_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_mipi0_cfg_ind_imi_reserved,reserved,HI_BBP_CTU_DSP_MIPI0_CFG_IND_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_MIPI0_CFG_IND_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_mipi0_rd_clr_dsp_mipi0_rd_clr,dsp_mipi0_rd_clr,HI_BBP_CTU_DSP_MIPI0_RD_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_MIPI0_RD_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_mipi0_rd_clr_reserved,reserved,HI_BBP_CTU_DSP_MIPI0_RD_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_MIPI0_RD_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb0_line_control_cmd_abb0_line_control_cmd_sel,abb0_line_control_cmd_sel,HI_BBP_CTU_ABB0_LINE_CONTROL_CMD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB0_LINE_CONTROL_CMD_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb0_line_control_cmd_abb0_mode_sel_cmd,abb0_mode_sel_cmd,HI_BBP_CTU_ABB0_LINE_CONTROL_CMD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB0_LINE_CONTROL_CMD_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb0_line_control_cmd_abb0_tx_en_cmd,abb0_tx_en_cmd,HI_BBP_CTU_ABB0_LINE_CONTROL_CMD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB0_LINE_CONTROL_CMD_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb0_line_control_cmd_abb0_rxa_en_cmd,abb0_rxa_en_cmd,HI_BBP_CTU_ABB0_LINE_CONTROL_CMD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB0_LINE_CONTROL_CMD_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb0_line_control_cmd_abb0_rxb_en_cmd,abb0_rxb_en_cmd,HI_BBP_CTU_ABB0_LINE_CONTROL_CMD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB0_LINE_CONTROL_CMD_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb0_line_control_cmd_abb0_blka_en_cmd,abb0_blka_en_cmd,HI_BBP_CTU_ABB0_LINE_CONTROL_CMD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB0_LINE_CONTROL_CMD_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb0_line_control_cmd_abb0_blkb_en_cmd,abb0_blkb_en_cmd,HI_BBP_CTU_ABB0_LINE_CONTROL_CMD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB0_LINE_CONTROL_CMD_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb0_line_control_cmd_reserved,reserved,HI_BBP_CTU_ABB0_LINE_CONTROL_CMD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB0_LINE_CONTROL_CMD_OFFSET)
HI_SET_GET(hi_bbp_ctu_ch0_bbp_sel_ch0_bbp_sel,ch0_bbp_sel,HI_BBP_CTU_CH0_BBP_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CH0_BBP_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_ch0_bbp_sel_reserved,reserved,HI_BBP_CTU_CH0_BBP_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CH0_BBP_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi0_func_sel_cpu_mipi0_func_sel,cpu_mipi0_func_sel,HI_BBP_CTU_CPU_MIPI0_FUNC_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI0_FUNC_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi0_func_sel_reserved,reserved,HI_BBP_CTU_CPU_MIPI0_FUNC_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI0_FUNC_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi0_test_func_cpu_mipi0_test_func,cpu_mipi0_test_func,HI_BBP_CTU_CPU_MIPI0_TEST_FUNC_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI0_TEST_FUNC_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi0_test_func_reserved,reserved,HI_BBP_CTU_CPU_MIPI0_TEST_FUNC_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI0_TEST_FUNC_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi0_sclk_test_cpu_mipi0_sclk_test,cpu_mipi0_sclk_test,HI_BBP_CTU_CPU_MIPI0_SCLK_TEST_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI0_SCLK_TEST_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi0_sclk_test_reserved,reserved,HI_BBP_CTU_CPU_MIPI0_SCLK_TEST_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI0_SCLK_TEST_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi0_sdata_test_cpu_mipi0_sdata_test,cpu_mipi0_sdata_test,HI_BBP_CTU_CPU_MIPI0_SDATA_TEST_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI0_SDATA_TEST_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi0_sdata_test_reserved,reserved,HI_BBP_CTU_CPU_MIPI0_SDATA_TEST_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI0_SDATA_TEST_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi0_clr_ind_imi_cpu_mipi0_clr_ind_imi,cpu_mipi0_clr_ind_imi,HI_BBP_CTU_CPU_MIPI0_CLR_IND_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI0_CLR_IND_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi0_clr_ind_imi_reserved,reserved,HI_BBP_CTU_CPU_MIPI0_CLR_IND_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI0_CLR_IND_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi0_fifo_clr_imi_cpu_mipi0_fifo_clr_imi,cpu_mipi0_fifo_clr_imi,HI_BBP_CTU_CPU_MIPI0_FIFO_CLR_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI0_FIFO_CLR_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi0_fifo_clr_imi_reserved,reserved,HI_BBP_CTU_CPU_MIPI0_FIFO_CLR_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI0_FIFO_CLR_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_ssi0_fifo_clr_imi_cpu_ssi0_fifo_clr_imi,cpu_ssi0_fifo_clr_imi,HI_BBP_CTU_CPU_SSI0_FIFO_CLR_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_SSI0_FIFO_CLR_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_ssi0_fifo_clr_imi_reserved,reserved,HI_BBP_CTU_CPU_SSI0_FIFO_CLR_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_SSI0_FIFO_CLR_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx0_tx0_ckg_bypass_cpu_rx0_lte_ckg_bypass,cpu_rx0_lte_ckg_bypass,HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx0_tx0_ckg_bypass_cpu_rx0_tds_ckg_bypass,cpu_rx0_tds_ckg_bypass,HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx0_tx0_ckg_bypass_cpu_rx0_w_ckg_bypass,cpu_rx0_w_ckg_bypass,HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx0_tx0_ckg_bypass_cpu_rx0_g1_ckg_bypass,cpu_rx0_g1_ckg_bypass,HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx0_tx0_ckg_bypass_cpu_rx0_g2_ckg_bypass,cpu_rx0_g2_ckg_bypass,HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx0_tx0_ckg_bypass_cpu_tx0_lte_ckg_bypass,cpu_tx0_lte_ckg_bypass,HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx0_tx0_ckg_bypass_cpu_tx0_tds_ckg_bypass,cpu_tx0_tds_ckg_bypass,HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx0_tx0_ckg_bypass_cpu_tx0_w_ckg_bypass,cpu_tx0_w_ckg_bypass,HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx0_tx0_ckg_bypass_cpu_tx0_g1_ckg_bypass,cpu_tx0_g1_ckg_bypass,HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx0_tx0_ckg_bypass_cpu_tx0_g2_ckg_bypass,cpu_tx0_g2_ckg_bypass,HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx0_tx0_ckg_bypass_reserved,reserved,HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX0_TX0_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_primary_mode_ind_c1_primary_mode_ind,c1_primary_mode_ind,HI_BBP_CTU_C1_PRIMARY_MODE_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_PRIMARY_MODE_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_primary_mode_ind_reserved,reserved,HI_BBP_CTU_C1_PRIMARY_MODE_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_PRIMARY_MODE_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_second_mode_ind_c1_second_mode_ind,c1_second_mode_ind,HI_BBP_CTU_C1_SECOND_MODE_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_SECOND_MODE_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_second_mode_ind_reserved,reserved,HI_BBP_CTU_C1_SECOND_MODE_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_SECOND_MODE_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_ch_num_ind_c1_ch_num_ind,c1_ch_num_ind,HI_BBP_CTU_C1_CH_NUM_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_CH_NUM_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_ch_num_ind_c1_ch_num_ind_sel,c1_ch_num_ind_sel,HI_BBP_CTU_C1_CH_NUM_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_CH_NUM_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_ch_num_ind_reserved,reserved,HI_BBP_CTU_C1_CH_NUM_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_CH_NUM_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_afc1_value_inter_rat_afc1_value_inter_rat,afc1_value_inter_rat,HI_BBP_CTU_AFC1_VALUE_INTER_RAT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_AFC1_VALUE_INTER_RAT_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_afc_pdm_select_mask_c1_afc_pdm_select_mask,c1_afc_pdm_select_mask,HI_BBP_CTU_C1_AFC_PDM_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_AFC_PDM_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_afc_pdm_select_mask_reserved,reserved,HI_BBP_CTU_C1_AFC_PDM_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_AFC_PDM_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_ant_switch_pa_select_mask_c1_ant_switch_pa_select_mask,c1_ant_switch_pa_select_mask,HI_BBP_CTU_C1_ANT_SWITCH_PA_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_ANT_SWITCH_PA_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_ant_switch_pa_select_mask_reserved,reserved,HI_BBP_CTU_C1_ANT_SWITCH_PA_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_ANT_SWITCH_PA_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_rfic_select_mask_c1_rfic_select_mask,c1_rfic_select_mask,HI_BBP_CTU_C1_RFIC_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_RFIC_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_rfic_select_mask_reserved,reserved,HI_BBP_CTU_C1_RFIC_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_RFIC_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_mipi_select_mask_c1_mipi_select_mask,c1_mipi_select_mask,HI_BBP_CTU_C1_MIPI_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_MIPI_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_mipi_select_mask_reserved,reserved,HI_BBP_CTU_C1_MIPI_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_MIPI_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_abb_select_mask_c1_abb_select_mask,c1_abb_select_mask,HI_BBP_CTU_C1_ABB_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_ABB_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_abb_select_mask_reserved,reserved,HI_BBP_CTU_C1_ABB_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_ABB_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_pmu_select_mask_c1_pmu_select_mask,c1_pmu_select_mask,HI_BBP_CTU_C1_PMU_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_PMU_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_pmu_select_mask_reserved,reserved,HI_BBP_CTU_C1_PMU_SELECT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_PMU_SELECT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_c1_sel_mask_dsp_c1_sel_mask,dsp_c1_sel_mask,HI_BBP_CTU_DSP_C1_SEL_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_C1_SEL_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_c1_sel_mask_reserved,reserved,HI_BBP_CTU_DSP_C1_SEL_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_C1_SEL_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_rfic1_rstn_dsp_rfic1_rstn,dsp_rfic1_rstn,HI_BBP_CTU_DSP_RFIC1_RSTN_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_RFIC1_RSTN_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_rfic1_rstn_reserved,reserved,HI_BBP_CTU_DSP_RFIC1_RSTN_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_RFIC1_RSTN_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_second_afc_mode_c1_second_afc_mode,c1_second_afc_mode,HI_BBP_CTU_C1_SECOND_AFC_MODE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_SECOND_AFC_MODE_OFFSET)
HI_SET_GET(hi_bbp_ctu_c1_second_afc_mode_reserved,reserved,HI_BBP_CTU_C1_SECOND_AFC_MODE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_C1_SECOND_AFC_MODE_OFFSET)
HI_SET_GET(hi_bbp_ctu_bbp_sys_1control_bbp_sys_1control,bbp_sys_1control,HI_BBP_CTU_BBP_SYS_1CONTROL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_BBP_SYS_1CONTROL_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi1_rd_overtime_flag_dsp_mipi1_rd_overtime_flag_dsp,mipi1_rd_overtime_flag_dsp,HI_BBP_CTU_MIPI1_RD_OVERTIME_FLAG_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI1_RD_OVERTIME_FLAG_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi1_rd_overtime_flag_dsp_reserved,reserved,HI_BBP_CTU_MIPI1_RD_OVERTIME_FLAG_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI1_RD_OVERTIME_FLAG_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_ssi1_rd_overtime_flag_dsp_ssi1_rd_overtime_flag_dsp,ssi1_rd_overtime_flag_dsp,HI_BBP_CTU_SSI1_RD_OVERTIME_FLAG_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_SSI1_RD_OVERTIME_FLAG_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_ssi1_rd_overtime_flag_dsp_reserved,reserved,HI_BBP_CTU_SSI1_RD_OVERTIME_FLAG_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_SSI1_RD_OVERTIME_FLAG_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi1_rd_overtime_clr_mipi1_rd_overtime_clr_imi,mipi1_rd_overtime_clr_imi,HI_BBP_CTU_MIPI1_RD_OVERTIME_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI1_RD_OVERTIME_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi1_rd_overtime_clr_reserved,reserved,HI_BBP_CTU_MIPI1_RD_OVERTIME_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI1_RD_OVERTIME_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_ssi1_rd_overtime_clr_ssi1_rd_overtime_clr_imi,ssi1_rd_overtime_clr_imi,HI_BBP_CTU_SSI1_RD_OVERTIME_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_SSI1_RD_OVERTIME_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_ssi1_rd_overtime_clr_reserved,reserved,HI_BBP_CTU_SSI1_RD_OVERTIME_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_SSI1_RD_OVERTIME_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic1_1_rd_result_rfic1_1,rd_result_rfic1_1,HI_BBP_CTU_RD_RESULT_RFIC1_1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC1_1_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic1_2_rd_result_rfic1_2,rd_result_rfic1_2,HI_BBP_CTU_RD_RESULT_RFIC1_2_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC1_2_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic1_3_rd_result_rfic1_3,rd_result_rfic1_3,HI_BBP_CTU_RD_RESULT_RFIC1_3_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC1_3_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic1_4_rd_result_rfic1_4,rd_result_rfic1_4,HI_BBP_CTU_RD_RESULT_RFIC1_4_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC1_4_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic1_5_rd_result_rfic1_5,rd_result_rfic1_5,HI_BBP_CTU_RD_RESULT_RFIC1_5_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC1_5_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic1_6_rd_result_rfic1_6,rd_result_rfic1_6,HI_BBP_CTU_RD_RESULT_RFIC1_6_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC1_6_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic1_7_rd_result_rfic1_7,rd_result_rfic1_7,HI_BBP_CTU_RD_RESULT_RFIC1_7_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC1_7_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic1_8_rd_result_rfic1_8,rd_result_rfic1_8,HI_BBP_CTU_RD_RESULT_RFIC1_8_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC1_8_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic1_9_rd_result_rfic1_9,rd_result_rfic1_9,HI_BBP_CTU_RD_RESULT_RFIC1_9_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC1_9_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic1_10_rd_result_rfic1_10,rd_result_rfic1_10,HI_BBP_CTU_RD_RESULT_RFIC1_10_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC1_10_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic1_11_rd_result_rfic1_11,rd_result_rfic1_11,HI_BBP_CTU_RD_RESULT_RFIC1_11_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC1_11_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic1_12_rd_result_rfic1_12,rd_result_rfic1_12,HI_BBP_CTU_RD_RESULT_RFIC1_12_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC1_12_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic1_13_rd_result_rfic1_13,rd_result_rfic1_13,HI_BBP_CTU_RD_RESULT_RFIC1_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC1_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic1_14_rd_result_rfic1_14,rd_result_rfic1_14,HI_BBP_CTU_RD_RESULT_RFIC1_14_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC1_14_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic1_15_rd_result_rfic1_15,rd_result_rfic1_15,HI_BBP_CTU_RD_RESULT_RFIC1_15_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC1_15_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_rfic1_16_rd_result_rfic1_16,rd_result_rfic1_16,HI_BBP_CTU_RD_RESULT_RFIC1_16_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_RFIC1_16_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_flag_rfic1_rd_result_flag_rfic1,rd_result_flag_rfic1,HI_BBP_CTU_RD_RESULT_FLAG_RFIC1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_FLAG_RFIC1_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_flag_rfic1_reserved,reserved,HI_BBP_CTU_RD_RESULT_FLAG_RFIC1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_FLAG_RFIC1_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_flag_rfic1_clr_rd_result_flag_rfic1_clr,rd_result_flag_rfic1_clr,HI_BBP_CTU_RD_RESULT_FLAG_RFIC1_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_FLAG_RFIC1_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_flag_rfic1_clr_reserved,reserved,HI_BBP_CTU_RD_RESULT_FLAG_RFIC1_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_FLAG_RFIC1_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi1_1_rd_result_mipi1_1,rd_result_mipi1_1,HI_BBP_CTU_RD_RESULT_MIPI1_1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI1_1_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi1_2_rd_result_mipi1_2,rd_result_mipi1_2,HI_BBP_CTU_RD_RESULT_MIPI1_2_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI1_2_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi1_3_rd_result_mipi1_3,rd_result_mipi1_3,HI_BBP_CTU_RD_RESULT_MIPI1_3_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI1_3_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi1_4_rd_result_mipi1_4,rd_result_mipi1_4,HI_BBP_CTU_RD_RESULT_MIPI1_4_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI1_4_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi1_5_rd_result_mipi1_5,rd_result_mipi1_5,HI_BBP_CTU_RD_RESULT_MIPI1_5_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI1_5_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi1_6_rd_result_mipi1_6,rd_result_mipi1_6,HI_BBP_CTU_RD_RESULT_MIPI1_6_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI1_6_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi1_7_rd_result_mipi1_7,rd_result_mipi1_7,HI_BBP_CTU_RD_RESULT_MIPI1_7_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI1_7_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi1_8_rd_result_mipi1_8,rd_result_mipi1_8,HI_BBP_CTU_RD_RESULT_MIPI1_8_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI1_8_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi1_9_rd_result_mipi1_9,rd_result_mipi1_9,HI_BBP_CTU_RD_RESULT_MIPI1_9_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI1_9_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi1_10_rd_result_mipi1_10,rd_result_mipi1_10,HI_BBP_CTU_RD_RESULT_MIPI1_10_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI1_10_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi1_11_rd_result_mipi1_11,rd_result_mipi1_11,HI_BBP_CTU_RD_RESULT_MIPI1_11_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI1_11_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi1_12_rd_result_mipi1_12,rd_result_mipi1_12,HI_BBP_CTU_RD_RESULT_MIPI1_12_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI1_12_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi1_13_rd_result_mipi1_13,rd_result_mipi1_13,HI_BBP_CTU_RD_RESULT_MIPI1_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI1_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi1_14_rd_result_mipi1_14,rd_result_mipi1_14,HI_BBP_CTU_RD_RESULT_MIPI1_14_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI1_14_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi1_15_rd_result_mipi1_15,rd_result_mipi1_15,HI_BBP_CTU_RD_RESULT_MIPI1_15_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI1_15_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_mipi1_16_rd_result_mipi1_16,rd_result_mipi1_16,HI_BBP_CTU_RD_RESULT_MIPI1_16_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_MIPI1_16_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_flag_mipi1_rd_result_flag_mipi1,rd_result_flag_mipi1,HI_BBP_CTU_RD_RESULT_FLAG_MIPI1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_FLAG_MIPI1_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_flag_mipi1_reserved,reserved,HI_BBP_CTU_RD_RESULT_FLAG_MIPI1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_FLAG_MIPI1_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_flag_mipi1_clr_rd_result_flag_mipi1_clr,rd_result_flag_mipi1_clr,HI_BBP_CTU_RD_RESULT_FLAG_MIPI1_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_FLAG_MIPI1_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_result_flag_mipi1_clr_reserved,reserved,HI_BBP_CTU_RD_RESULT_FLAG_MIPI1_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_RESULT_FLAG_MIPI1_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi1_rd_data_low_soft_mipi1_rd_data_low_soft,mipi1_rd_data_low_soft,HI_BBP_CTU_MIPI1_RD_DATA_LOW_SOFT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI1_RD_DATA_LOW_SOFT_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi1_rd_data_high_soft_mipi1_rd_data_high_soft,mipi1_rd_data_high_soft,HI_BBP_CTU_MIPI1_RD_DATA_HIGH_SOFT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI1_RD_DATA_HIGH_SOFT_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_end_flag_mipi1_soft_rd_end_flag_mipi1_soft,rd_end_flag_mipi1_soft,HI_BBP_CTU_RD_END_FLAG_MIPI1_SOFT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_END_FLAG_MIPI1_SOFT_OFFSET)
HI_SET_GET(hi_bbp_ctu_rd_end_flag_mipi1_soft_reserved,reserved,HI_BBP_CTU_RD_END_FLAG_MIPI1_SOFT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RD_END_FLAG_MIPI1_SOFT_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi1_grant_dsp_mipi1_grant_dsp,mipi1_grant_dsp,HI_BBP_CTU_MIPI1_GRANT_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI1_GRANT_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi1_grant_dsp_reserved,reserved,HI_BBP_CTU_MIPI1_GRANT_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI1_GRANT_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_mipi1_wdata_low_dsp_mipi1_wdata_low,dsp_mipi1_wdata_low,HI_BBP_CTU_DSP_MIPI1_WDATA_LOW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_MIPI1_WDATA_LOW_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_mipi1_wdata_high_dsp_mipi1_wdata_high,dsp_mipi1_wdata_high,HI_BBP_CTU_DSP_MIPI1_WDATA_HIGH_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_MIPI1_WDATA_HIGH_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_mipi1_en_imi_dsp_mipi1_en_imi,dsp_mipi1_en_imi,HI_BBP_CTU_DSP_MIPI1_EN_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_MIPI1_EN_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_mipi1_en_imi_reserved,reserved,HI_BBP_CTU_DSP_MIPI1_EN_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_MIPI1_EN_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_mipi1_cfg_ind_imi_dsp_mipi1_cfg_ind_imi,dsp_mipi1_cfg_ind_imi,HI_BBP_CTU_DSP_MIPI1_CFG_IND_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_MIPI1_CFG_IND_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_mipi1_cfg_ind_imi_reserved,reserved,HI_BBP_CTU_DSP_MIPI1_CFG_IND_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_MIPI1_CFG_IND_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_mipi1_rd_clr_dsp_mipi1_rd_clr,dsp_mipi1_rd_clr,HI_BBP_CTU_DSP_MIPI1_RD_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_MIPI1_RD_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_mipi1_rd_clr_reserved,reserved,HI_BBP_CTU_DSP_MIPI1_RD_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_MIPI1_RD_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb1_line_control_cmd_abb1_line_control_cmd_sel,abb1_line_control_cmd_sel,HI_BBP_CTU_ABB1_LINE_CONTROL_CMD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB1_LINE_CONTROL_CMD_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb1_line_control_cmd_abb1_mode_sel_cmd,abb1_mode_sel_cmd,HI_BBP_CTU_ABB1_LINE_CONTROL_CMD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB1_LINE_CONTROL_CMD_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb1_line_control_cmd_abb1_tx_en_cmd,abb1_tx_en_cmd,HI_BBP_CTU_ABB1_LINE_CONTROL_CMD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB1_LINE_CONTROL_CMD_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb1_line_control_cmd_abb1_rxa_en_cmd,abb1_rxa_en_cmd,HI_BBP_CTU_ABB1_LINE_CONTROL_CMD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB1_LINE_CONTROL_CMD_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb1_line_control_cmd_abb1_rxb_en_cmd,abb1_rxb_en_cmd,HI_BBP_CTU_ABB1_LINE_CONTROL_CMD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB1_LINE_CONTROL_CMD_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb1_line_control_cmd_abb1_blka_en_cmd,abb1_blka_en_cmd,HI_BBP_CTU_ABB1_LINE_CONTROL_CMD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB1_LINE_CONTROL_CMD_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb1_line_control_cmd_abb1_blkb_en_cmd,abb1_blkb_en_cmd,HI_BBP_CTU_ABB1_LINE_CONTROL_CMD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB1_LINE_CONTROL_CMD_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb1_line_control_cmd_reserved,reserved,HI_BBP_CTU_ABB1_LINE_CONTROL_CMD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB1_LINE_CONTROL_CMD_OFFSET)
HI_SET_GET(hi_bbp_ctu_ch1_bbp_sel_ch1_bbp_sel,ch1_bbp_sel,HI_BBP_CTU_CH1_BBP_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CH1_BBP_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_ch1_bbp_sel_reserved,reserved,HI_BBP_CTU_CH1_BBP_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CH1_BBP_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi1_func_sel_cpu_mipi1_func_sel,cpu_mipi1_func_sel,HI_BBP_CTU_CPU_MIPI1_FUNC_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI1_FUNC_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi1_func_sel_reserved,reserved,HI_BBP_CTU_CPU_MIPI1_FUNC_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI1_FUNC_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi1_test_func_cpu_mipi1_test_func,cpu_mipi1_test_func,HI_BBP_CTU_CPU_MIPI1_TEST_FUNC_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI1_TEST_FUNC_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi1_test_func_reserved,reserved,HI_BBP_CTU_CPU_MIPI1_TEST_FUNC_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI1_TEST_FUNC_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi1_sclk_test_cpu_mipi1_sclk_test,cpu_mipi1_sclk_test,HI_BBP_CTU_CPU_MIPI1_SCLK_TEST_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI1_SCLK_TEST_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi1_sclk_test_reserved,reserved,HI_BBP_CTU_CPU_MIPI1_SCLK_TEST_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI1_SCLK_TEST_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi1_sdata_test_cpu_mipi1_sdata_test,cpu_mipi1_sdata_test,HI_BBP_CTU_CPU_MIPI1_SDATA_TEST_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI1_SDATA_TEST_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi1_sdata_test_reserved,reserved,HI_BBP_CTU_CPU_MIPI1_SDATA_TEST_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI1_SDATA_TEST_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi1_clr_ind_imi_cpu_mipi1_clr_ind_imi,cpu_mipi1_clr_ind_imi,HI_BBP_CTU_CPU_MIPI1_CLR_IND_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI1_CLR_IND_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi1_clr_ind_imi_reserved,reserved,HI_BBP_CTU_CPU_MIPI1_CLR_IND_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI1_CLR_IND_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi1_fifo_clr_imi_cpu_mipi1_fifo_clr_imi,cpu_mipi1_fifo_clr_imi,HI_BBP_CTU_CPU_MIPI1_FIFO_CLR_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI1_FIFO_CLR_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_mipi1_fifo_clr_imi_reserved,reserved,HI_BBP_CTU_CPU_MIPI1_FIFO_CLR_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_MIPI1_FIFO_CLR_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_ssi1_fifo_clr_imi_cpu_ssi1_fifo_clr_imi,cpu_ssi1_fifo_clr_imi,HI_BBP_CTU_CPU_SSI1_FIFO_CLR_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_SSI1_FIFO_CLR_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_ssi1_fifo_clr_imi_reserved,reserved,HI_BBP_CTU_CPU_SSI1_FIFO_CLR_IMI_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_SSI1_FIFO_CLR_IMI_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx1_tx1_ckg_bypass_cpu_rx1_lte_ckg_bypass,cpu_rx1_lte_ckg_bypass,HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx1_tx1_ckg_bypass_cpu_rx1_tds_ckg_bypass,cpu_rx1_tds_ckg_bypass,HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx1_tx1_ckg_bypass_cpu_rx1_w_ckg_bypass,cpu_rx1_w_ckg_bypass,HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx1_tx1_ckg_bypass_cpu_rx1_g1_ckg_bypass,cpu_rx1_g1_ckg_bypass,HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx1_tx1_ckg_bypass_cpu_rx1_g2_ckg_bypass,cpu_rx1_g2_ckg_bypass,HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx1_tx1_ckg_bypass_cpu_tx1_lte_ckg_bypass,cpu_tx1_lte_ckg_bypass,HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx1_tx1_ckg_bypass_cpu_tx1_tds_ckg_bypass,cpu_tx1_tds_ckg_bypass,HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx1_tx1_ckg_bypass_cpu_tx1_w_ckg_bypass,cpu_tx1_w_ckg_bypass,HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx1_tx1_ckg_bypass_cpu_tx1_g1_ckg_bypass,cpu_tx1_g1_ckg_bypass,HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx1_tx1_ckg_bypass_cpu_tx1_g2_ckg_bypass,cpu_tx1_g2_ckg_bypass,HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_cpu_rx1_tx1_ckg_bypass_reserved,reserved,HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CPU_RX1_TX1_CKG_BYPASS_OFFSET)
HI_SET_GET(hi_bbp_ctu_delay_cnt_delay_cnt,delay_cnt,HI_BBP_CTU_DELAY_CNT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DELAY_CNT_OFFSET)
HI_SET_GET(hi_bbp_ctu_delay_cnt_reserved,reserved,HI_BBP_CTU_DELAY_CNT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DELAY_CNT_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_wait_cnt_int_wait_cnt,int_wait_cnt,HI_BBP_CTU_INT_WAIT_CNT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_WAIT_CNT_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_wait_cnt_reserved,reserved,HI_BBP_CTU_INT_WAIT_CNT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_WAIT_CNT_OFFSET)
HI_SET_GET(hi_bbp_ctu_ltebbp_input_mask_ltebbp_input_mask,ltebbp_input_mask,HI_BBP_CTU_LTEBBP_INPUT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTEBBP_INPUT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_ltebbp_input_mask_reserved,reserved,HI_BBP_CTU_LTEBBP_INPUT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTEBBP_INPUT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_wbbp_input_mask_wbbp_input_mask,wbbp_input_mask,HI_BBP_CTU_WBBP_INPUT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_WBBP_INPUT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_wbbp_input_mask_reserved,reserved,HI_BBP_CTU_WBBP_INPUT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_WBBP_INPUT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_gmbbp_input_mask_gmbbp_input_mask,gmbbp_input_mask,HI_BBP_CTU_GMBBP_INPUT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GMBBP_INPUT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_gmbbp_input_mask_reserved,reserved,HI_BBP_CTU_GMBBP_INPUT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GMBBP_INPUT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_gsbbp_input_mask_gsbbp_input_mask,gsbbp_input_mask,HI_BBP_CTU_GSBBP_INPUT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GSBBP_INPUT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_gsbbp_input_mask_reserved,reserved,HI_BBP_CTU_GSBBP_INPUT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GSBBP_INPUT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_tdsbbp_input_mask_tdsbbp_input_mask,tdsbbp_input_mask,HI_BBP_CTU_TDSBBP_INPUT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDSBBP_INPUT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_tdsbbp_input_mask_reserved,reserved,HI_BBP_CTU_TDSBBP_INPUT_MASK_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDSBBP_INPUT_MASK_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_mux_frame_num_lte_mux_frame_num,lte_mux_frame_num,HI_BBP_CTU_LTE_MUX_FRAME_NUM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_MUX_FRAME_NUM_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_mux_frame_num_tds_mux_frame_num,tds_mux_frame_num,HI_BBP_CTU_TDS_MUX_FRAME_NUM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_MUX_FRAME_NUM_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_mux_frame_num_w_mux_frame_num,w_mux_frame_num,HI_BBP_CTU_W_MUX_FRAME_NUM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_MUX_FRAME_NUM_OFFSET)
HI_SET_GET(hi_bbp_ctu_meas_flag_l_rd_meas_flag_l_rd,meas_flag_l_rd,HI_BBP_CTU_MEAS_FLAG_L_RD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MEAS_FLAG_L_RD_OFFSET)
HI_SET_GET(hi_bbp_ctu_meas_flag_w_rd_meas_flag_w_rd,meas_flag_w_rd,HI_BBP_CTU_MEAS_FLAG_W_RD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MEAS_FLAG_W_RD_OFFSET)
HI_SET_GET(hi_bbp_ctu_meas_flag_t_rd_meas_flag_t_rd,meas_flag_t_rd,HI_BBP_CTU_MEAS_FLAG_T_RD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MEAS_FLAG_T_RD_OFFSET)
HI_SET_GET(hi_bbp_ctu_meas_flag_gm_rd_meas_flag_gm_rd,meas_flag_gm_rd,HI_BBP_CTU_MEAS_FLAG_GM_RD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MEAS_FLAG_GM_RD_OFFSET)
HI_SET_GET(hi_bbp_ctu_meas_flag_gs_rd_meas_flag_gs_rd,meas_flag_gs_rd,HI_BBP_CTU_MEAS_FLAG_GS_RD_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MEAS_FLAG_GS_RD_OFFSET)
HI_SET_GET(hi_bbp_ctu_wake_flag_l_wr_wake_flag_l_wr,wake_flag_l_wr,HI_BBP_CTU_WAKE_FLAG_L_WR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_WAKE_FLAG_L_WR_OFFSET)
HI_SET_GET(hi_bbp_ctu_wake_flag_w_wr_wake_flag_w_wr,wake_flag_w_wr,HI_BBP_CTU_WAKE_FLAG_W_WR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_WAKE_FLAG_W_WR_OFFSET)
HI_SET_GET(hi_bbp_ctu_wake_flag_t_wr_wake_flag_t_wr,wake_flag_t_wr,HI_BBP_CTU_WAKE_FLAG_T_WR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_WAKE_FLAG_T_WR_OFFSET)
HI_SET_GET(hi_bbp_ctu_wake_flag_gm_wr_wake_flag_gm_wr,wake_flag_gm_wr,HI_BBP_CTU_WAKE_FLAG_GM_WR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_WAKE_FLAG_GM_WR_OFFSET)
HI_SET_GET(hi_bbp_ctu_wake_flag_gs_wr_wake_flag_gs_wr,wake_flag_gs_wr,HI_BBP_CTU_WAKE_FLAG_GS_WR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_WAKE_FLAG_GS_WR_OFFSET)
HI_SET_GET(hi_bbp_ctu_timing_get_ind_timing_get_ind,timing_get_ind,HI_BBP_CTU_TIMING_GET_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TIMING_GET_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_timing_get_ind_reserved,reserved,HI_BBP_CTU_TIMING_GET_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TIMING_GET_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_timing_clear_timing_clear,timing_clear,HI_BBP_CTU_TIMING_CLEAR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TIMING_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_ctu_timing_clear_reserved,reserved,HI_BBP_CTU_TIMING_CLEAR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TIMING_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_ctu_timing_valid_o_timing_valid,o_timing_valid,HI_BBP_CTU_TIMING_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TIMING_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_timing_valid_reserved,reserved,HI_BBP_CTU_TIMING_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TIMING_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_timing_dsp_w_clk_cnt_dsp,w_clk_cnt_dsp,HI_BBP_CTU_W_TIMING_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_TIMING_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_timing_dsp_w_chip_cnt_dsp,w_chip_cnt_dsp,HI_BBP_CTU_W_TIMING_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_TIMING_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_timing_dsp_w_slot_cnt_dsp,w_slot_cnt_dsp,HI_BBP_CTU_W_TIMING_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_TIMING_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_timing_dsp_w_sfn_dsp,w_sfn_dsp,HI_BBP_CTU_W_TIMING_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_TIMING_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_fn_dsp_o_gm_fn_dsp,o_gm_fn_dsp,HI_BBP_CTU_GM_FN_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_FN_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_fn_dsp_reserved,reserved,HI_BBP_CTU_GM_FN_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_FN_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_qb_dsp_o_gm_qb_dsp,o_gm_qb_dsp,HI_BBP_CTU_GM_QB_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_QB_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_qb_dsp_reserved,reserved,HI_BBP_CTU_GM_QB_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_QB_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_fn_dsp_gs_fn_dsp,gs_fn_dsp,HI_BBP_CTU_GS_FN_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_FN_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_fn_dsp_reserved,reserved,HI_BBP_CTU_GS_FN_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_FN_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_qb_dsp_o_gs_qb_dsp,o_gs_qb_dsp,HI_BBP_CTU_GS_QB_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_QB_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_qb_dsp_reserved,reserved,HI_BBP_CTU_GS_QB_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_QB_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_timing_dsp_o_lte_sys_ts_cnt_dsp,o_lte_sys_ts_cnt_dsp,HI_BBP_CTU_LTE_TIMING_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_TIMING_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_timing_dsp_reserved_1,reserved_1,HI_BBP_CTU_LTE_TIMING_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_TIMING_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_timing_dsp_o_lte_sys_subfrm_cnt_dsp,o_lte_sys_subfrm_cnt_dsp,HI_BBP_CTU_LTE_TIMING_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_TIMING_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_timing_dsp_o_lte_sfn_dsp,o_lte_sfn_dsp,HI_BBP_CTU_LTE_TIMING_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_TIMING_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_timing_dsp_reserved_0,reserved_0,HI_BBP_CTU_LTE_TIMING_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_TIMING_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_timing1_dsp_o_tds_timing1_dsp,o_tds_timing1_dsp,HI_BBP_CTU_TDS_TIMING1_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_TIMING1_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_timing1_dsp_reserved,reserved,HI_BBP_CTU_TDS_TIMING1_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_TIMING1_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_timing2_dsp_o_tds_timing2_dsp,o_tds_timing2_dsp,HI_BBP_CTU_TDS_TIMING2_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_TIMING2_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_timing2_dsp_reserved,reserved,HI_BBP_CTU_TDS_TIMING2_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_TIMING2_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_ctu_timing_dsp_o_ctu_timing_dsp,o_ctu_timing_dsp,HI_BBP_CTU_CTU_TIMING_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CTU_TIMING_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_ctu_basecnt1_dsp_o_ctu_basecnt1_dsp,o_ctu_basecnt1_dsp,HI_BBP_CTU_CTU_BASECNT1_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CTU_BASECNT1_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_ctu_basecnt2_dsp_o_ctu_basecnt2_dsp,o_ctu_basecnt2_dsp,HI_BBP_CTU_CTU_BASECNT2_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CTU_BASECNT2_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_ctu_basecnt_dsp_ctu_basecnt_dsp,ctu_basecnt_dsp,HI_BBP_CTU_CTU_BASECNT_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CTU_BASECNT_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_ctu_timing_get_ind_dsp_ctu_timing_get_ind,dsp_ctu_timing_get_ind,HI_BBP_CTU_DSP_CTU_TIMING_GET_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_CTU_TIMING_GET_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_ctu_timing_get_ind_reserved,reserved,HI_BBP_CTU_DSP_CTU_TIMING_GET_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_CTU_TIMING_GET_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_ctu_timing_valid_dsp_ctu_timing_valid_dsp,ctu_timing_valid_dsp,HI_BBP_CTU_CTU_TIMING_VALID_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CTU_TIMING_VALID_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_ctu_timing_valid_dsp_reserved,reserved,HI_BBP_CTU_CTU_TIMING_VALID_DSP_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_CTU_TIMING_VALID_DSP_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_ctu_timing_clr_dsp_ctu_timing_clr,dsp_ctu_timing_clr,HI_BBP_CTU_DSP_CTU_TIMING_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_CTU_TIMING_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_ctu_timing_clr_reserved,reserved,HI_BBP_CTU_DSP_CTU_TIMING_CLR_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_CTU_TIMING_CLR_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_intrasys_valid_o_w_intrasys_valid,o_w_intrasys_valid,HI_BBP_CTU_W_INTRASYS_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_INTRASYS_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_intrasys_valid_o_w_intrasys_valid_delay,o_w_intrasys_valid_delay,HI_BBP_CTU_W_INTRASYS_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_INTRASYS_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_intrasys_valid_reserved,reserved,HI_BBP_CTU_W_INTRASYS_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_INTRASYS_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_intrasys_valid_o_gm_intrasys_valid,o_gm_intrasys_valid,HI_BBP_CTU_GM_INTRASYS_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_INTRASYS_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_intrasys_valid_o_gm_intrasys_valid_delay,o_gm_intrasys_valid_delay,HI_BBP_CTU_GM_INTRASYS_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_INTRASYS_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_intrasys_valid_reserved,reserved,HI_BBP_CTU_GM_INTRASYS_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_INTRASYS_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_intrasys_valid_o_gs_intrasys_valid,o_gs_intrasys_valid,HI_BBP_CTU_GS_INTRASYS_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_INTRASYS_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_intrasys_valid_o_gs_intrasys_valid_delay,o_gs_intrasys_valid_delay,HI_BBP_CTU_GS_INTRASYS_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_INTRASYS_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_intrasys_valid_reserved,reserved,HI_BBP_CTU_GS_INTRASYS_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_INTRASYS_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_intrasys_valid_o_lte_intrasys_valid,o_lte_intrasys_valid,HI_BBP_CTU_LTE_INTRASYS_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_INTRASYS_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_intrasys_valid_o_lte_intrasys_valid_delay,o_lte_intrasys_valid_delay,HI_BBP_CTU_LTE_INTRASYS_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_INTRASYS_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_intrasys_valid_reserved,reserved,HI_BBP_CTU_LTE_INTRASYS_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_INTRASYS_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_intrasys_valid_o_tds_intrasys_valid,o_tds_intrasys_valid,HI_BBP_CTU_TDS_INTRASYS_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_INTRASYS_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_intrasys_valid_o_tds_intrasys_valid_delay,o_tds_intrasys_valid_delay,HI_BBP_CTU_TDS_INTRASYS_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_INTRASYS_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_intrasys_valid_reserved,reserved,HI_BBP_CTU_TDS_INTRASYS_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_INTRASYS_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_rfic_ck_cfg_cpu_rfic0_ck_cfg,cpu_rfic0_ck_cfg,HI_BBP_CTU_RFIC_CK_CFG_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RFIC_CK_CFG_OFFSET)
HI_SET_GET(hi_bbp_ctu_rfic_ck_cfg_cpu_rfic1_ck_cfg,cpu_rfic1_ck_cfg,HI_BBP_CTU_RFIC_CK_CFG_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RFIC_CK_CFG_OFFSET)
HI_SET_GET(hi_bbp_ctu_rfic_ck_cfg_reserved,reserved,HI_BBP_CTU_RFIC_CK_CFG_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RFIC_CK_CFG_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_waittime_dsp_mipi_wait_time,dsp_mipi_wait_time,HI_BBP_CTU_DSP_WAITTIME_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_WAITTIME_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_waittime_dsp_ssi_wait_time,dsp_ssi_wait_time,HI_BBP_CTU_DSP_WAITTIME_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_WAITTIME_OFFSET)
HI_SET_GET(hi_bbp_ctu_dsp_waittime_reserved,reserved,HI_BBP_CTU_DSP_WAITTIME_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DSP_WAITTIME_OFFSET)
HI_SET_GET(hi_bbp_ctu_s_w_ch_num_ind_s_w_ch_num_ind,s_w_ch_num_ind,HI_BBP_CTU_S_W_CH_NUM_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_S_W_CH_NUM_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_s_w_ch_num_ind_s_w_ch_num_ind_sel,s_w_ch_num_ind_sel,HI_BBP_CTU_S_W_CH_NUM_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_S_W_CH_NUM_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_s_w_ch_num_ind_reserved,reserved,HI_BBP_CTU_S_W_CH_NUM_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_S_W_CH_NUM_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_intersys_measure_type_w_intersys_measure_type,w_intersys_measure_type,HI_BBP_CTU_W_INTERSYS_MEASURE_TYPE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_INTERSYS_MEASURE_TYPE_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_intersys_measure_type_reserved,reserved,HI_BBP_CTU_W_INTERSYS_MEASURE_TYPE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_INTERSYS_MEASURE_TYPE_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_measure_report_valid_w_measure_report_valid,w_measure_report_valid,HI_BBP_CTU_W_MEASURE_REPORT_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_MEASURE_REPORT_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_measure_report_valid_reserved,reserved,HI_BBP_CTU_W_MEASURE_REPORT_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_MEASURE_REPORT_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_preint_offset_time_w_preint_offset_time,w_preint_offset_time,HI_BBP_CTU_W_PREINT_OFFSET_TIME_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_PREINT_OFFSET_TIME_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_intersys_measure_time_w_intersys_measure_time,w_intersys_measure_time,HI_BBP_CTU_W_INTERSYS_MEASURE_TIME_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_INTERSYS_MEASURE_TIME_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_w_rcv_lte_int_clear_w_rcv_lte,int_clear_w_rcv_lte,HI_BBP_CTU_INT_CLEAR_W_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_W_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_w_rcv_lte_reserved,reserved,HI_BBP_CTU_INT_CLEAR_W_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_W_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_w_rcv_tds_int_clear_w_rcv_tds,int_clear_w_rcv_tds,HI_BBP_CTU_INT_CLEAR_W_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_W_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_w_rcv_tds_reserved,reserved,HI_BBP_CTU_INT_CLEAR_W_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_W_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_w_rcv_gm_int_clear_w_rcv_gm,int_clear_w_rcv_gm,HI_BBP_CTU_INT_CLEAR_W_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_W_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_w_rcv_gm_reserved,reserved,HI_BBP_CTU_INT_CLEAR_W_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_W_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_w_rcv_gs_int_clear_w_rcv_gs,int_clear_w_rcv_gs,HI_BBP_CTU_INT_CLEAR_W_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_W_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_w_rcv_gs_reserved,reserved,HI_BBP_CTU_INT_CLEAR_W_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_W_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_w_rcv_w_int_clear_w_rcv_w,int_clear_w_rcv_w,HI_BBP_CTU_INT_CLEAR_W_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_W_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_w_rcv_w_reserved,reserved,HI_BBP_CTU_INT_CLEAR_W_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_W_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_w_int012_int_clear_w_int012,int_clear_w_int012,HI_BBP_CTU_INT_CLEAR_W_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_W_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_w_int012_reserved,reserved,HI_BBP_CTU_INT_CLEAR_W_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_W_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_w_rcv_lte_int_mask_w_rcv_lte,int_mask_w_rcv_lte,HI_BBP_CTU_INT_MASK_W_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_W_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_w_rcv_lte_reserved,reserved,HI_BBP_CTU_INT_MASK_W_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_W_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_w_rcv_tds_int_mask_w_rcv_tds,int_mask_w_rcv_tds,HI_BBP_CTU_INT_MASK_W_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_W_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_w_rcv_tds_reserved,reserved,HI_BBP_CTU_INT_MASK_W_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_W_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_w_rcv_gm_int_mask_w_rcv_gm,int_mask_w_rcv_gm,HI_BBP_CTU_INT_MASK_W_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_W_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_w_rcv_gm_reserved,reserved,HI_BBP_CTU_INT_MASK_W_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_W_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_w_rcv_gs_int_mask_w_rcv_gs,int_mask_w_rcv_gs,HI_BBP_CTU_INT_MASK_W_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_W_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_w_rcv_gs_reserved,reserved,HI_BBP_CTU_INT_MASK_W_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_W_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_w_rcv_w_int_mask_w_rcv_w,int_mask_w_rcv_w,HI_BBP_CTU_INT_MASK_W_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_W_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_w_rcv_w_reserved,reserved,HI_BBP_CTU_INT_MASK_W_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_W_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_w_int012_int_mask_w_int012,int_mask_w_int012,HI_BBP_CTU_INT_MASK_W_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_W_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_w_int012_reserved,reserved,HI_BBP_CTU_INT_MASK_W_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_W_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_w_rcv_lte_o_int_type_w_rcv_lte,o_int_type_w_rcv_lte,HI_BBP_CTU_INT_TYPE_W_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_W_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_w_rcv_lte_reserved,reserved,HI_BBP_CTU_INT_TYPE_W_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_W_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_w_rcv_tds_o_int_type_w_rcv_tds,o_int_type_w_rcv_tds,HI_BBP_CTU_INT_TYPE_W_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_W_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_w_rcv_tds_reserved,reserved,HI_BBP_CTU_INT_TYPE_W_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_W_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_w_rcv_gm_o_int_type_w_rcv_gm,o_int_type_w_rcv_gm,HI_BBP_CTU_INT_TYPE_W_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_W_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_w_rcv_gm_reserved,reserved,HI_BBP_CTU_INT_TYPE_W_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_W_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_w_rcv_gs_o_int_type_w_rcv_gs,o_int_type_w_rcv_gs,HI_BBP_CTU_INT_TYPE_W_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_W_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_w_rcv_gs_reserved,reserved,HI_BBP_CTU_INT_TYPE_W_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_W_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_w_rcv_w_o_int_type_w_rcv_w,o_int_type_w_rcv_w,HI_BBP_CTU_INT_TYPE_W_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_W_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_w_rcv_w_reserved,reserved,HI_BBP_CTU_INT_TYPE_W_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_W_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_w_int012_o_int_type_w_int012,o_int_type_w_int012,HI_BBP_CTU_INT_TYPE_W_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_W_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_w_int012_reserved,reserved,HI_BBP_CTU_INT_TYPE_W_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_W_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_snd_lte_int_13_reserved_1,reserved_1,HI_BBP_CTU_W_SND_LTE_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_SND_LTE_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_snd_lte_int_13_w_snd_lte_int_13,w_snd_lte_int_13,HI_BBP_CTU_W_SND_LTE_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_SND_LTE_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_snd_lte_int_13_reserved_0,reserved_0,HI_BBP_CTU_W_SND_LTE_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_SND_LTE_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_snd_tds_int_13_reserved_1,reserved_1,HI_BBP_CTU_W_SND_TDS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_SND_TDS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_snd_tds_int_13_w_snd_tds_int_13,w_snd_tds_int_13,HI_BBP_CTU_W_SND_TDS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_SND_TDS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_snd_tds_int_13_reserved_0,reserved_0,HI_BBP_CTU_W_SND_TDS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_SND_TDS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_snd_gm_int_13_reserved_1,reserved_1,HI_BBP_CTU_W_SND_GM_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_SND_GM_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_snd_gm_int_13_w_snd_gm_int_13,w_snd_gm_int_13,HI_BBP_CTU_W_SND_GM_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_SND_GM_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_snd_gm_int_13_reserved_0,reserved_0,HI_BBP_CTU_W_SND_GM_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_SND_GM_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_snd_gs_int_13_reserved_1,reserved_1,HI_BBP_CTU_W_SND_GS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_SND_GS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_snd_gs_int_13_w_snd_gs_int_13,w_snd_gs_int_13,HI_BBP_CTU_W_SND_GS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_SND_GS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_snd_gs_int_13_reserved_0,reserved_0,HI_BBP_CTU_W_SND_GS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_SND_GS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_snd_w_int_13_reserved_1,reserved_1,HI_BBP_CTU_W_SND_W_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_SND_W_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_snd_w_int_13_w_snd_w_int_13,w_snd_w_int_13,HI_BBP_CTU_W_SND_W_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_SND_W_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_snd_w_int_13_reserved_0,reserved_0,HI_BBP_CTU_W_SND_W_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_SND_W_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_s_lte_ch_num_ind_s_lte_ch_num_ind,s_lte_ch_num_ind,HI_BBP_CTU_S_LTE_CH_NUM_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_S_LTE_CH_NUM_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_s_lte_ch_num_ind_s_lte_ch_num_ind_sel,s_lte_ch_num_ind_sel,HI_BBP_CTU_S_LTE_CH_NUM_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_S_LTE_CH_NUM_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_s_lte_ch_num_ind_reserved,reserved,HI_BBP_CTU_S_LTE_CH_NUM_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_S_LTE_CH_NUM_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_intersys_measure_type_lte_intersys_measure_type,lte_intersys_measure_type,HI_BBP_CTU_LTE_INTERSYS_MEASURE_TYPE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_INTERSYS_MEASURE_TYPE_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_intersys_measure_type_reserved,reserved,HI_BBP_CTU_LTE_INTERSYS_MEASURE_TYPE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_INTERSYS_MEASURE_TYPE_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_measure_report_valid_lte_measure_report_valid,lte_measure_report_valid,HI_BBP_CTU_LTE_MEASURE_REPORT_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_MEASURE_REPORT_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_measure_report_valid_reserved,reserved,HI_BBP_CTU_LTE_MEASURE_REPORT_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_MEASURE_REPORT_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_preint_offset_time_lte_preint_offset_time,lte_preint_offset_time,HI_BBP_CTU_LTE_PREINT_OFFSET_TIME_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_PREINT_OFFSET_TIME_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_intersys_measure_time_lte_intersys_measure_time,lte_intersys_measure_time,HI_BBP_CTU_LTE_INTERSYS_MEASURE_TIME_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_INTERSYS_MEASURE_TIME_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_lte_rcv_lte_int_clear_lte_rcv_lte,int_clear_lte_rcv_lte,HI_BBP_CTU_INT_CLEAR_LTE_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_LTE_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_lte_rcv_lte_reserved,reserved,HI_BBP_CTU_INT_CLEAR_LTE_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_LTE_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_lte_rcv_tds_int_clear_lte_rcv_tds,int_clear_lte_rcv_tds,HI_BBP_CTU_INT_CLEAR_LTE_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_LTE_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_lte_rcv_tds_reserved,reserved,HI_BBP_CTU_INT_CLEAR_LTE_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_LTE_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_lte_rcv_gm_int_clear_lte_rcv_gm,int_clear_lte_rcv_gm,HI_BBP_CTU_INT_CLEAR_LTE_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_LTE_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_lte_rcv_gm_reserved,reserved,HI_BBP_CTU_INT_CLEAR_LTE_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_LTE_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_lte_rcv_gs_int_clear_lte_rcv_gs,int_clear_lte_rcv_gs,HI_BBP_CTU_INT_CLEAR_LTE_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_LTE_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_lte_rcv_gs_reserved,reserved,HI_BBP_CTU_INT_CLEAR_LTE_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_LTE_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_lte_rcv_w_int_clear_lte_rcv_w,int_clear_lte_rcv_w,HI_BBP_CTU_INT_CLEAR_LTE_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_LTE_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_lte_rcv_w_reserved,reserved,HI_BBP_CTU_INT_CLEAR_LTE_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_LTE_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_lte_int012_int_clear_lte_int012,int_clear_lte_int012,HI_BBP_CTU_INT_CLEAR_LTE_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_LTE_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_lte_int012_reserved,reserved,HI_BBP_CTU_INT_CLEAR_LTE_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_LTE_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_lte_rcv_lte_int_mask_lte_rcv_lte,int_mask_lte_rcv_lte,HI_BBP_CTU_INT_MASK_LTE_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_LTE_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_lte_rcv_lte_reserved,reserved,HI_BBP_CTU_INT_MASK_LTE_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_LTE_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_lte_rcv_tds_int_mask_lte_rcv_tds,int_mask_lte_rcv_tds,HI_BBP_CTU_INT_MASK_LTE_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_LTE_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_lte_rcv_tds_reserved,reserved,HI_BBP_CTU_INT_MASK_LTE_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_LTE_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_lte_rcv_gm_int_mask_lte_rcv_gm,int_mask_lte_rcv_gm,HI_BBP_CTU_INT_MASK_LTE_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_LTE_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_lte_rcv_gm_reserved,reserved,HI_BBP_CTU_INT_MASK_LTE_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_LTE_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_lte_rcv_gs_int_mask_lte_rcv_gs,int_mask_lte_rcv_gs,HI_BBP_CTU_INT_MASK_LTE_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_LTE_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_lte_rcv_gs_reserved,reserved,HI_BBP_CTU_INT_MASK_LTE_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_LTE_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_lte_rcv_w_int_mask_lte_rcv_w,int_mask_lte_rcv_w,HI_BBP_CTU_INT_MASK_LTE_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_LTE_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_lte_rcv_w_reserved,reserved,HI_BBP_CTU_INT_MASK_LTE_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_LTE_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_lte_int012_int_mask_lte_int012,int_mask_lte_int012,HI_BBP_CTU_INT_MASK_LTE_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_LTE_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_lte_int012_reserved,reserved,HI_BBP_CTU_INT_MASK_LTE_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_LTE_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_lte_rcv_w_o_int_type_lte_rcv_w,o_int_type_lte_rcv_w,HI_BBP_CTU_INT_TYPE_LTE_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_LTE_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_lte_rcv_w_reserved,reserved,HI_BBP_CTU_INT_TYPE_LTE_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_LTE_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_lte_rcv_tds_o_int_type_lte_rcv_tds,o_int_type_lte_rcv_tds,HI_BBP_CTU_INT_TYPE_LTE_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_LTE_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_lte_rcv_tds_reserved,reserved,HI_BBP_CTU_INT_TYPE_LTE_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_LTE_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_lte_rcv_gm_o_int_type_lte_rcv_gm,o_int_type_lte_rcv_gm,HI_BBP_CTU_INT_TYPE_LTE_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_LTE_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_lte_rcv_gm_reserved,reserved,HI_BBP_CTU_INT_TYPE_LTE_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_LTE_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_lte_rcv_gs_o_int_type_lte_rcv_gs,o_int_type_lte_rcv_gs,HI_BBP_CTU_INT_TYPE_LTE_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_LTE_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_lte_rcv_gs_reserved,reserved,HI_BBP_CTU_INT_TYPE_LTE_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_LTE_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_lte_rcv_lte_o_int_type_lte_rcv_lte,o_int_type_lte_rcv_lte,HI_BBP_CTU_INT_TYPE_LTE_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_LTE_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_lte_rcv_lte_reserved,reserved,HI_BBP_CTU_INT_TYPE_LTE_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_LTE_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_lte_int012_o_int_type_lte_int012,o_int_type_lte_int012,HI_BBP_CTU_INT_TYPE_LTE_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_LTE_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_lte_int012_reserved,reserved,HI_BBP_CTU_INT_TYPE_LTE_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_LTE_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_snd_lte_int_13_reserved_1,reserved_1,HI_BBP_CTU_LTE_SND_LTE_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_SND_LTE_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_snd_lte_int_13_lte_snd_lte_int_13,lte_snd_lte_int_13,HI_BBP_CTU_LTE_SND_LTE_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_SND_LTE_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_snd_lte_int_13_reserved_0,reserved_0,HI_BBP_CTU_LTE_SND_LTE_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_SND_LTE_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_snd_tds_int_13_reserved_1,reserved_1,HI_BBP_CTU_LTE_SND_TDS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_SND_TDS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_snd_tds_int_13_lte_snd_tds_int_13,lte_snd_tds_int_13,HI_BBP_CTU_LTE_SND_TDS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_SND_TDS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_snd_tds_int_13_reserved_0,reserved_0,HI_BBP_CTU_LTE_SND_TDS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_SND_TDS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_snd_gm_int_13_reserved_1,reserved_1,HI_BBP_CTU_LTE_SND_GM_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_SND_GM_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_snd_gm_int_13_lte_snd_gm_int_13,lte_snd_gm_int_13,HI_BBP_CTU_LTE_SND_GM_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_SND_GM_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_snd_gm_int_13_reserved_0,reserved_0,HI_BBP_CTU_LTE_SND_GM_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_SND_GM_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_snd_gs_int_13_reserved_1,reserved_1,HI_BBP_CTU_LTE_SND_GS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_SND_GS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_snd_gs_int_13_lte_snd_gs_int_13,lte_snd_gs_int_13,HI_BBP_CTU_LTE_SND_GS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_SND_GS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_snd_gs_int_13_reserved_0,reserved_0,HI_BBP_CTU_LTE_SND_GS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_SND_GS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_snd_w_int_13_reserved_1,reserved_1,HI_BBP_CTU_LTE_SND_W_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_SND_W_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_snd_w_int_13_lte_snd_w_int_13,lte_snd_w_int_13,HI_BBP_CTU_LTE_SND_W_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_SND_W_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_snd_w_int_13_reserved_0,reserved_0,HI_BBP_CTU_LTE_SND_W_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_SND_W_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_s_tds_ch_num_ind_s_tds_ch_num_ind,s_tds_ch_num_ind,HI_BBP_CTU_S_TDS_CH_NUM_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_S_TDS_CH_NUM_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_s_tds_ch_num_ind_s_tds_ch_num_ind_sel,s_tds_ch_num_ind_sel,HI_BBP_CTU_S_TDS_CH_NUM_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_S_TDS_CH_NUM_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_s_tds_ch_num_ind_reserved,reserved,HI_BBP_CTU_S_TDS_CH_NUM_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_S_TDS_CH_NUM_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_intersys_measure_type_tds_intersys_measure_type,tds_intersys_measure_type,HI_BBP_CTU_TDS_INTERSYS_MEASURE_TYPE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_INTERSYS_MEASURE_TYPE_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_intersys_measure_type_reserved,reserved,HI_BBP_CTU_TDS_INTERSYS_MEASURE_TYPE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_INTERSYS_MEASURE_TYPE_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_measure_report_valid_tds_measure_report_valid,tds_measure_report_valid,HI_BBP_CTU_TDS_MEASURE_REPORT_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_MEASURE_REPORT_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_measure_report_valid_reserved,reserved,HI_BBP_CTU_TDS_MEASURE_REPORT_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_MEASURE_REPORT_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_preint_offset_time_tds_preint_offset_time,tds_preint_offset_time,HI_BBP_CTU_TDS_PREINT_OFFSET_TIME_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_PREINT_OFFSET_TIME_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_intersys_measure_time_tds_intersys_measure_time,tds_intersys_measure_time,HI_BBP_CTU_TDS_INTERSYS_MEASURE_TIME_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_INTERSYS_MEASURE_TIME_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_tds_rcv_lte_int_clear_tds_rcv_lte,int_clear_tds_rcv_lte,HI_BBP_CTU_INT_CLEAR_TDS_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_TDS_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_tds_rcv_lte_reserved,reserved,HI_BBP_CTU_INT_CLEAR_TDS_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_TDS_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_tds_rcv_tds_int_clear_tds_rcv_tds,int_clear_tds_rcv_tds,HI_BBP_CTU_INT_CLEAR_TDS_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_TDS_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_tds_rcv_tds_reserved,reserved,HI_BBP_CTU_INT_CLEAR_TDS_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_TDS_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_tds_rcv_gm_int_clear_tds_rcv_gm,int_clear_tds_rcv_gm,HI_BBP_CTU_INT_CLEAR_TDS_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_TDS_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_tds_rcv_gm_reserved,reserved,HI_BBP_CTU_INT_CLEAR_TDS_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_TDS_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_tds_rcv_gs_int_clear_tds_rcv_gs,int_clear_tds_rcv_gs,HI_BBP_CTU_INT_CLEAR_TDS_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_TDS_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_tds_rcv_gs_reserved,reserved,HI_BBP_CTU_INT_CLEAR_TDS_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_TDS_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_tds_rcv_w_int_clear_tds_rcv_w,int_clear_tds_rcv_w,HI_BBP_CTU_INT_CLEAR_TDS_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_TDS_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_tds_rcv_w_reserved,reserved,HI_BBP_CTU_INT_CLEAR_TDS_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_TDS_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_tds_int012_int_clear_tds_int012,int_clear_tds_int012,HI_BBP_CTU_INT_CLEAR_TDS_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_TDS_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_tds_int012_reserved,reserved,HI_BBP_CTU_INT_CLEAR_TDS_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_TDS_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_tds_rcv_lte_int_mask_tds_rcv_lte,int_mask_tds_rcv_lte,HI_BBP_CTU_INT_MASK_TDS_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_TDS_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_tds_rcv_lte_reserved,reserved,HI_BBP_CTU_INT_MASK_TDS_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_TDS_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_tds_rcv_tds_int_mask_tds_rcv_tds,int_mask_tds_rcv_tds,HI_BBP_CTU_INT_MASK_TDS_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_TDS_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_tds_rcv_tds_reserved,reserved,HI_BBP_CTU_INT_MASK_TDS_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_TDS_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_tds_rcv_gm_int_mask_tds_rcv_gm,int_mask_tds_rcv_gm,HI_BBP_CTU_INT_MASK_TDS_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_TDS_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_tds_rcv_gm_reserved,reserved,HI_BBP_CTU_INT_MASK_TDS_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_TDS_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_tds_rcv_gs_int_mask_tds_rcv_gs,int_mask_tds_rcv_gs,HI_BBP_CTU_INT_MASK_TDS_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_TDS_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_tds_rcv_gs_reserved,reserved,HI_BBP_CTU_INT_MASK_TDS_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_TDS_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_tds_rcv_w_int_mask_tds_rcv_w,int_mask_tds_rcv_w,HI_BBP_CTU_INT_MASK_TDS_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_TDS_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_tds_rcv_w_reserved,reserved,HI_BBP_CTU_INT_MASK_TDS_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_TDS_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_tds_int012_int_mask_tds_int012,int_mask_tds_int012,HI_BBP_CTU_INT_MASK_TDS_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_TDS_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_tds_int012_reserved,reserved,HI_BBP_CTU_INT_MASK_TDS_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_TDS_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_tds_rcv_lte_o_int_type_tds_rcv_lte,o_int_type_tds_rcv_lte,HI_BBP_CTU_INT_TYPE_TDS_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_TDS_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_tds_rcv_lte_reserved,reserved,HI_BBP_CTU_INT_TYPE_TDS_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_TDS_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_tds_rcv_w_o_int_type_tds_rcv_w,o_int_type_tds_rcv_w,HI_BBP_CTU_INT_TYPE_TDS_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_TDS_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_tds_rcv_w_reserved,reserved,HI_BBP_CTU_INT_TYPE_TDS_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_TDS_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_tds_rcv_gm_o_int_type_tds_rcv_gm,o_int_type_tds_rcv_gm,HI_BBP_CTU_INT_TYPE_TDS_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_TDS_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_tds_rcv_gm_reserved,reserved,HI_BBP_CTU_INT_TYPE_TDS_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_TDS_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_tds_rcv_gs_o_int_type_tds_rcv_gs,o_int_type_tds_rcv_gs,HI_BBP_CTU_INT_TYPE_TDS_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_TDS_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_tds_rcv_gs_reserved,reserved,HI_BBP_CTU_INT_TYPE_TDS_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_TDS_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_tds_rcv_tds_o_int_type_tds_rcv_tds,o_int_type_tds_rcv_tds,HI_BBP_CTU_INT_TYPE_TDS_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_TDS_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_tds_rcv_tds_reserved,reserved,HI_BBP_CTU_INT_TYPE_TDS_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_TDS_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_tds_int012_o_int_type_tds_int012,o_int_type_tds_int012,HI_BBP_CTU_INT_TYPE_TDS_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_TDS_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_tds_int012_reserved,reserved,HI_BBP_CTU_INT_TYPE_TDS_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_TDS_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_snd_lte_int_13_reserved_1,reserved_1,HI_BBP_CTU_TDS_SND_LTE_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_SND_LTE_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_snd_lte_int_13_tds_snd_lte_int_13,tds_snd_lte_int_13,HI_BBP_CTU_TDS_SND_LTE_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_SND_LTE_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_snd_lte_int_13_reserved_0,reserved_0,HI_BBP_CTU_TDS_SND_LTE_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_SND_LTE_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_snd_tds_int_13_reserved_1,reserved_1,HI_BBP_CTU_TDS_SND_TDS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_SND_TDS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_snd_tds_int_13_tds_snd_tds_int_13,tds_snd_tds_int_13,HI_BBP_CTU_TDS_SND_TDS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_SND_TDS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_snd_tds_int_13_reserved_0,reserved_0,HI_BBP_CTU_TDS_SND_TDS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_SND_TDS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_snd_gm_int_13_reserved_1,reserved_1,HI_BBP_CTU_TDS_SND_GM_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_SND_GM_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_snd_gm_int_13_tds_snd_gm_int_13,tds_snd_gm_int_13,HI_BBP_CTU_TDS_SND_GM_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_SND_GM_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_snd_gm_int_13_reserved_0,reserved_0,HI_BBP_CTU_TDS_SND_GM_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_SND_GM_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_snd_gs_int_13_reserved_1,reserved_1,HI_BBP_CTU_TDS_SND_GS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_SND_GS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_snd_gs_int_13_tds_snd_gs_int_13,tds_snd_gs_int_13,HI_BBP_CTU_TDS_SND_GS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_SND_GS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_snd_gs_int_13_reserved_0,reserved_0,HI_BBP_CTU_TDS_SND_GS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_SND_GS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_snd_w_int_13_reserved_1,reserved_1,HI_BBP_CTU_TDS_SND_W_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_SND_W_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_snd_w_int_13_tds_snd_w_int_13,tds_snd_w_int_13,HI_BBP_CTU_TDS_SND_W_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_SND_W_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_snd_w_int_13_reserved_0,reserved_0,HI_BBP_CTU_TDS_SND_W_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_SND_W_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_s_gm_ch_num_ind_s_gm_ch_num_ind,s_gm_ch_num_ind,HI_BBP_CTU_S_GM_CH_NUM_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_S_GM_CH_NUM_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_s_gm_ch_num_ind_s_gm_ch_num_ind_sel,s_gm_ch_num_ind_sel,HI_BBP_CTU_S_GM_CH_NUM_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_S_GM_CH_NUM_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_s_gm_ch_num_ind_reserved,reserved,HI_BBP_CTU_S_GM_CH_NUM_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_S_GM_CH_NUM_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_intersys_measure_type_gm_intersys_measure_type,gm_intersys_measure_type,HI_BBP_CTU_GM_INTERSYS_MEASURE_TYPE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_INTERSYS_MEASURE_TYPE_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_intersys_measure_type_reserved,reserved,HI_BBP_CTU_GM_INTERSYS_MEASURE_TYPE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_INTERSYS_MEASURE_TYPE_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_measure_report_valid_gm_measure_report_valid,gm_measure_report_valid,HI_BBP_CTU_GM_MEASURE_REPORT_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_MEASURE_REPORT_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_measure_report_valid_reserved,reserved,HI_BBP_CTU_GM_MEASURE_REPORT_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_MEASURE_REPORT_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_preint_offset_time_gm_preint_offset_time,gm_preint_offset_time,HI_BBP_CTU_GM_PREINT_OFFSET_TIME_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_PREINT_OFFSET_TIME_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_intersys_measure_time_gm_intersys_measure_time,gm_intersys_measure_time,HI_BBP_CTU_GM_INTERSYS_MEASURE_TIME_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_INTERSYS_MEASURE_TIME_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gm_rcv_lte_int_clear_gm_rcv_lte,int_clear_gm_rcv_lte,HI_BBP_CTU_INT_CLEAR_GM_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GM_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gm_rcv_lte_reserved,reserved,HI_BBP_CTU_INT_CLEAR_GM_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GM_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gm_rcv_tds_int_clear_gm_rcv_tds,int_clear_gm_rcv_tds,HI_BBP_CTU_INT_CLEAR_GM_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GM_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gm_rcv_tds_reserved,reserved,HI_BBP_CTU_INT_CLEAR_GM_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GM_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gm_rcv_gm_int_clear_gm_rcv_gm,int_clear_gm_rcv_gm,HI_BBP_CTU_INT_CLEAR_GM_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GM_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gm_rcv_gm_reserved,reserved,HI_BBP_CTU_INT_CLEAR_GM_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GM_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gm_rcv_gs_int_clear_gm_rcv_gs,int_clear_gm_rcv_gs,HI_BBP_CTU_INT_CLEAR_GM_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GM_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gm_rcv_gs_reserved,reserved,HI_BBP_CTU_INT_CLEAR_GM_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GM_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gm_rcv_w_int_clear_gm_rcv_w,int_clear_gm_rcv_w,HI_BBP_CTU_INT_CLEAR_GM_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GM_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gm_rcv_w_reserved,reserved,HI_BBP_CTU_INT_CLEAR_GM_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GM_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gm_int012_int_clear_gm_int012,int_clear_gm_int012,HI_BBP_CTU_INT_CLEAR_GM_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GM_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gm_int012_reserved,reserved,HI_BBP_CTU_INT_CLEAR_GM_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GM_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gm_rcv_lte_int_mask_gm_rcv_lte,int_mask_gm_rcv_lte,HI_BBP_CTU_INT_MASK_GM_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GM_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gm_rcv_lte_reserved,reserved,HI_BBP_CTU_INT_MASK_GM_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GM_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gm_rcv_tds_int_mask_gm_rcv_tds,int_mask_gm_rcv_tds,HI_BBP_CTU_INT_MASK_GM_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GM_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gm_rcv_tds_reserved,reserved,HI_BBP_CTU_INT_MASK_GM_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GM_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gm_rcv_gm_int_mask_gm_rcv_gm,int_mask_gm_rcv_gm,HI_BBP_CTU_INT_MASK_GM_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GM_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gm_rcv_gm_reserved,reserved,HI_BBP_CTU_INT_MASK_GM_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GM_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gm_rcv_gs_int_mask_gm_rcv_gs,int_mask_gm_rcv_gs,HI_BBP_CTU_INT_MASK_GM_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GM_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gm_rcv_gs_reserved,reserved,HI_BBP_CTU_INT_MASK_GM_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GM_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gm_rcv_w_int_mask_gm_rcv_w,int_mask_gm_rcv_w,HI_BBP_CTU_INT_MASK_GM_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GM_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gm_rcv_w_reserved,reserved,HI_BBP_CTU_INT_MASK_GM_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GM_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gm_int012_int_mask_gm_int012,int_mask_gm_int012,HI_BBP_CTU_INT_MASK_GM_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GM_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gm_int012_reserved,reserved,HI_BBP_CTU_INT_MASK_GM_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GM_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gm_rcv_lte_o_int_type_gm_rcv_lte,o_int_type_gm_rcv_lte,HI_BBP_CTU_INT_TYPE_GM_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GM_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gm_rcv_lte_reserved,reserved,HI_BBP_CTU_INT_TYPE_GM_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GM_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gm_rcv_tds_o_int_type_gm_rcv_tds,o_int_type_gm_rcv_tds,HI_BBP_CTU_INT_TYPE_GM_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GM_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gm_rcv_tds_reserved,reserved,HI_BBP_CTU_INT_TYPE_GM_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GM_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gm_rcv_w_o_int_type_gm_rcv_w,o_int_type_gm_rcv_w,HI_BBP_CTU_INT_TYPE_GM_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GM_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gm_rcv_w_reserved,reserved,HI_BBP_CTU_INT_TYPE_GM_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GM_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gm_rcv_gs_o_int_type_gm_rcv_gs,o_int_type_gm_rcv_gs,HI_BBP_CTU_INT_TYPE_GM_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GM_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gm_rcv_gs_reserved,reserved,HI_BBP_CTU_INT_TYPE_GM_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GM_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gm_rcv_gm_o_int_type_gm_rcv_gm,o_int_type_gm_rcv_gm,HI_BBP_CTU_INT_TYPE_GM_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GM_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gm_rcv_gm_reserved,reserved,HI_BBP_CTU_INT_TYPE_GM_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GM_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gm_int012_o_int_type_gm_int012,o_int_type_gm_int012,HI_BBP_CTU_INT_TYPE_GM_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GM_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gm_int012_reserved,reserved,HI_BBP_CTU_INT_TYPE_GM_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GM_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_snd_lte_int_13_reserved_1,reserved_1,HI_BBP_CTU_GM_SND_LTE_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_SND_LTE_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_snd_lte_int_13_gm_snd_lte_int_13,gm_snd_lte_int_13,HI_BBP_CTU_GM_SND_LTE_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_SND_LTE_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_snd_lte_int_13_reserved_0,reserved_0,HI_BBP_CTU_GM_SND_LTE_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_SND_LTE_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_snd_tds_int_13_reserved_1,reserved_1,HI_BBP_CTU_GM_SND_TDS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_SND_TDS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_snd_tds_int_13_gm_snd_tds_int_13,gm_snd_tds_int_13,HI_BBP_CTU_GM_SND_TDS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_SND_TDS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_snd_tds_int_13_reserved_0,reserved_0,HI_BBP_CTU_GM_SND_TDS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_SND_TDS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_snd_gm_int_13_reserved_1,reserved_1,HI_BBP_CTU_GM_SND_GM_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_SND_GM_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_snd_gm_int_13_gm_snd_gm_int_13,gm_snd_gm_int_13,HI_BBP_CTU_GM_SND_GM_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_SND_GM_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_snd_gm_int_13_reserved_0,reserved_0,HI_BBP_CTU_GM_SND_GM_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_SND_GM_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_snd_gs_int_13_reserved_1,reserved_1,HI_BBP_CTU_GM_SND_GS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_SND_GS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_snd_gs_int_13_gm_snd_gs_int_13,gm_snd_gs_int_13,HI_BBP_CTU_GM_SND_GS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_SND_GS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_snd_gs_int_13_reserved_0,reserved_0,HI_BBP_CTU_GM_SND_GS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_SND_GS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_snd_w_int_13_reserved_1,reserved_1,HI_BBP_CTU_GM_SND_W_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_SND_W_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_snd_w_int_13_gm_snd_w_int_13,gm_snd_w_int_13,HI_BBP_CTU_GM_SND_W_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_SND_W_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_snd_w_int_13_reserved_0,reserved_0,HI_BBP_CTU_GM_SND_W_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_SND_W_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_s_gs_ch_num_ind_s_gs_ch_num_ind,s_gs_ch_num_ind,HI_BBP_CTU_S_GS_CH_NUM_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_S_GS_CH_NUM_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_s_gs_ch_num_ind_s_gs_ch_num_ind_sel,s_gs_ch_num_ind_sel,HI_BBP_CTU_S_GS_CH_NUM_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_S_GS_CH_NUM_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_s_gs_ch_num_ind_reserved,reserved,HI_BBP_CTU_S_GS_CH_NUM_IND_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_S_GS_CH_NUM_IND_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_intersys_measure_type_gs_intersys_measure_type,gs_intersys_measure_type,HI_BBP_CTU_GS_INTERSYS_MEASURE_TYPE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_INTERSYS_MEASURE_TYPE_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_intersys_measure_type_reserved,reserved,HI_BBP_CTU_GS_INTERSYS_MEASURE_TYPE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_INTERSYS_MEASURE_TYPE_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_measure_report_valid_gs_measure_report_valid,gs_measure_report_valid,HI_BBP_CTU_GS_MEASURE_REPORT_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_MEASURE_REPORT_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_measure_report_valid_reserved,reserved,HI_BBP_CTU_GS_MEASURE_REPORT_VALID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_MEASURE_REPORT_VALID_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_preint_offset_time_gs_preint_offset_time,gs_preint_offset_time,HI_BBP_CTU_GS_PREINT_OFFSET_TIME_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_PREINT_OFFSET_TIME_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_intersys_measure_time_gs_intersys_measure_time,gs_intersys_measure_time,HI_BBP_CTU_GS_INTERSYS_MEASURE_TIME_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_INTERSYS_MEASURE_TIME_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gs_rcv_lte_int_clear_gs_rcv_lte,int_clear_gs_rcv_lte,HI_BBP_CTU_INT_CLEAR_GS_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GS_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gs_rcv_lte_reserved,reserved,HI_BBP_CTU_INT_CLEAR_GS_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GS_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gs_rcv_tds_int_clear_gs_rcv_tds,int_clear_gs_rcv_tds,HI_BBP_CTU_INT_CLEAR_GS_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GS_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gs_rcv_tds_reserved,reserved,HI_BBP_CTU_INT_CLEAR_GS_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GS_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gs_rcv_gm_int_clear_gs_rcv_gm,int_clear_gs_rcv_gm,HI_BBP_CTU_INT_CLEAR_GS_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GS_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gs_rcv_gm_reserved,reserved,HI_BBP_CTU_INT_CLEAR_GS_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GS_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gs_rcv_gs_int_clear_gs_rcv_gs,int_clear_gs_rcv_gs,HI_BBP_CTU_INT_CLEAR_GS_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GS_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gs_rcv_gs_reserved,reserved,HI_BBP_CTU_INT_CLEAR_GS_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GS_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gs_rcv_w_int_clear_gs_rcv_w,int_clear_gs_rcv_w,HI_BBP_CTU_INT_CLEAR_GS_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GS_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gs_rcv_w_reserved,reserved,HI_BBP_CTU_INT_CLEAR_GS_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GS_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gs_int012_int_clear_gs_int012,int_clear_gs_int012,HI_BBP_CTU_INT_CLEAR_GS_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GS_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_clear_gs_int012_reserved,reserved,HI_BBP_CTU_INT_CLEAR_GS_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_CLEAR_GS_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gs_rcv_lte_int_mask_gs_rcv_lte,int_mask_gs_rcv_lte,HI_BBP_CTU_INT_MASK_GS_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GS_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gs_rcv_lte_reserved,reserved,HI_BBP_CTU_INT_MASK_GS_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GS_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gs_rcv_tds_int_mask_gs_rcv_tds,int_mask_gs_rcv_tds,HI_BBP_CTU_INT_MASK_GS_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GS_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gs_rcv_tds_reserved,reserved,HI_BBP_CTU_INT_MASK_GS_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GS_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gs_rcv_gm_int_mask_gs_rcv_gm,int_mask_gs_rcv_gm,HI_BBP_CTU_INT_MASK_GS_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GS_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gs_rcv_gm_reserved,reserved,HI_BBP_CTU_INT_MASK_GS_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GS_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gs_rcv_gs_int_mask_gs_rcv_gs,int_mask_gs_rcv_gs,HI_BBP_CTU_INT_MASK_GS_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GS_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gs_rcv_gs_reserved,reserved,HI_BBP_CTU_INT_MASK_GS_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GS_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gs_rcv_w_int_mask_gs_rcv_w,int_mask_gs_rcv_w,HI_BBP_CTU_INT_MASK_GS_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GS_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gs_rcv_w_reserved,reserved,HI_BBP_CTU_INT_MASK_GS_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GS_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gs_int012_int_mask_gs_int012,int_mask_gs_int012,HI_BBP_CTU_INT_MASK_GS_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GS_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_mask_gs_int012_reserved,reserved,HI_BBP_CTU_INT_MASK_GS_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_MASK_GS_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gs_rcv_lte_o_int_type_gs_rcv_lte,o_int_type_gs_rcv_lte,HI_BBP_CTU_INT_TYPE_GS_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GS_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gs_rcv_lte_reserved,reserved,HI_BBP_CTU_INT_TYPE_GS_RCV_LTE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GS_RCV_LTE_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gs_rcv_tds_o_int_type_gs_rcv_tds,o_int_type_gs_rcv_tds,HI_BBP_CTU_INT_TYPE_GS_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GS_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gs_rcv_tds_reserved,reserved,HI_BBP_CTU_INT_TYPE_GS_RCV_TDS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GS_RCV_TDS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gs_rcv_gm_o_int_type_gs_rcv_gm,o_int_type_gs_rcv_gm,HI_BBP_CTU_INT_TYPE_GS_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GS_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gs_rcv_gm_reserved,reserved,HI_BBP_CTU_INT_TYPE_GS_RCV_GM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GS_RCV_GM_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gs_rcv_gs_o_int_type_gs_rcv_gs,o_int_type_gs_rcv_gs,HI_BBP_CTU_INT_TYPE_GS_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GS_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gs_rcv_gs_reserved,reserved,HI_BBP_CTU_INT_TYPE_GS_RCV_GS_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GS_RCV_GS_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gs_rcv_w_o_int_type_gs_rcv_w,o_int_type_gs_rcv_w,HI_BBP_CTU_INT_TYPE_GS_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GS_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gs_rcv_w_reserved,reserved,HI_BBP_CTU_INT_TYPE_GS_RCV_W_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GS_RCV_W_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gs_int012_o_int_type_gs_int012,o_int_type_gs_int012,HI_BBP_CTU_INT_TYPE_GS_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GS_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_int_type_gs_int012_reserved,reserved,HI_BBP_CTU_INT_TYPE_GS_INT012_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_INT_TYPE_GS_INT012_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_snd_lte_int_13_reserved_1,reserved_1,HI_BBP_CTU_GS_SND_LTE_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_SND_LTE_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_snd_lte_int_13_gs_snd_lte_int_13,gs_snd_lte_int_13,HI_BBP_CTU_GS_SND_LTE_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_SND_LTE_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_snd_lte_int_13_reserved_0,reserved_0,HI_BBP_CTU_GS_SND_LTE_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_SND_LTE_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_snd_tds_int_13_reserved_1,reserved_1,HI_BBP_CTU_GS_SND_TDS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_SND_TDS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_snd_tds_int_13_gs_snd_tds_int_13,gs_snd_tds_int_13,HI_BBP_CTU_GS_SND_TDS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_SND_TDS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_snd_tds_int_13_reserved_0,reserved_0,HI_BBP_CTU_GS_SND_TDS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_SND_TDS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_snd_gm_int_13_reserved_1,reserved_1,HI_BBP_CTU_GS_SND_GM_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_SND_GM_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_snd_gm_int_13_gs_snd_gm_int_13,gs_snd_gm_int_13,HI_BBP_CTU_GS_SND_GM_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_SND_GM_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_snd_gm_int_13_reserved_0,reserved_0,HI_BBP_CTU_GS_SND_GM_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_SND_GM_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_snd_gs_int_13_reserved_1,reserved_1,HI_BBP_CTU_GS_SND_GS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_SND_GS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_snd_gs_int_13_gs_snd_gs_int_13,gs_snd_gs_int_13,HI_BBP_CTU_GS_SND_GS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_SND_GS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_snd_gs_int_13_reserved_0,reserved_0,HI_BBP_CTU_GS_SND_GS_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_SND_GS_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_snd_w_int_13_reserved_1,reserved_1,HI_BBP_CTU_GS_SND_W_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_SND_W_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_snd_w_int_13_gs_snd_w_int_13,gs_snd_w_int_13,HI_BBP_CTU_GS_SND_W_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_SND_W_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_snd_w_int_13_reserved_0,reserved_0,HI_BBP_CTU_GS_SND_W_INT_13_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_SND_W_INT_13_OFFSET)
HI_SET_GET(hi_bbp_ctu_gbbp1_19m_sel_gbbp1_19m_sel,gbbp1_19m_sel,HI_BBP_CTU_GBBP1_19M_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GBBP1_19M_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_gbbp1_19m_sel_reserved,reserved,HI_BBP_CTU_GBBP1_19M_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GBBP1_19M_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_gbbp2_19m_sel_gbbp2_19m_sel,gbbp2_19m_sel,HI_BBP_CTU_GBBP2_19M_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GBBP2_19M_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_gbbp2_19m_sel_reserved,reserved,HI_BBP_CTU_GBBP2_19M_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GBBP2_19M_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_wbbp_19m_sel_wbbp_19m_sel,wbbp_19m_sel,HI_BBP_CTU_WBBP_19M_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_WBBP_19M_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_wbbp_19m_sel_reserved,reserved,HI_BBP_CTU_WBBP_19M_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_WBBP_19M_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_lbbp_19m_sel_lbbp_19m_sel,lbbp_19m_sel,HI_BBP_CTU_LBBP_19M_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LBBP_19M_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_lbbp_19m_sel_reserved,reserved,HI_BBP_CTU_LBBP_19M_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LBBP_19M_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_tbbp1_19m_sel_tbbp_19m_sel,tbbp_19m_sel,HI_BBP_CTU_TBBP1_19M_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TBBP1_19M_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_tbbp1_19m_sel_reserved,reserved,HI_BBP_CTU_TBBP1_19M_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TBBP1_19M_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_gbbp1_clk_sel_gbbp1_clk_sel,gbbp1_clk_sel,HI_BBP_CTU_GBBP1_CLK_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GBBP1_CLK_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_gbbp1_clk_sel_reserved,reserved,HI_BBP_CTU_GBBP1_CLK_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GBBP1_CLK_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_gbbp2_clk_sel_gbbp2_clk_sel,gbbp2_clk_sel,HI_BBP_CTU_GBBP2_CLK_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GBBP2_CLK_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_gbbp2_clk_sel_reserved,reserved,HI_BBP_CTU_GBBP2_CLK_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GBBP2_CLK_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_wbbp_clk_sel_wbbp_clk_sel,wbbp_clk_sel,HI_BBP_CTU_WBBP_CLK_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_WBBP_CLK_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_wbbp_clk_sel_reserved,reserved,HI_BBP_CTU_WBBP_CLK_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_WBBP_CLK_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_lbbp_clk_sel_lbbp_clk_sel,lbbp_clk_sel,HI_BBP_CTU_LBBP_CLK_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LBBP_CLK_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_lbbp_clk_sel_reserved,reserved,HI_BBP_CTU_LBBP_CLK_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LBBP_CLK_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_tbbp_clk_sel_tbbp_clk_sel,tbbp_clk_sel,HI_BBP_CTU_TBBP_CLK_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TBBP_CLK_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_tbbp_clk_sel_reserved,reserved,HI_BBP_CTU_TBBP_CLK_SEL_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TBBP_CLK_SEL_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_abbif_fmt_reserved_1,reserved_1,HI_BBP_CTU_LTE_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_abbif_fmt_lte_rxb_iq_exchange,lte_rxb_iq_exchange,HI_BBP_CTU_LTE_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_abbif_fmt_lte_rxb_q_inv,lte_rxb_q_inv,HI_BBP_CTU_LTE_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_abbif_fmt_lte_rxb_i_inv,lte_rxb_i_inv,HI_BBP_CTU_LTE_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_abbif_fmt_lte_rxa_iq_exchange,lte_rxa_iq_exchange,HI_BBP_CTU_LTE_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_abbif_fmt_lte_rxa_q_inv,lte_rxa_q_inv,HI_BBP_CTU_LTE_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_abbif_fmt_lte_rxa_i_inv,lte_rxa_i_inv,HI_BBP_CTU_LTE_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_abbif_fmt_lte_tx_iq_exchange,lte_tx_iq_exchange,HI_BBP_CTU_LTE_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_abbif_fmt_lte_tx_q_inv,lte_tx_q_inv,HI_BBP_CTU_LTE_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_abbif_fmt_lte_tx_i_inv,lte_tx_i_inv,HI_BBP_CTU_LTE_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_lte_abbif_fmt_reserved_0,reserved_0,HI_BBP_CTU_LTE_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_LTE_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_abbif_fmt_reserved_1,reserved_1,HI_BBP_CTU_W_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_abbif_fmt_w_rxb_iq_exchange,w_rxb_iq_exchange,HI_BBP_CTU_W_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_abbif_fmt_w_rxb_q_inv,w_rxb_q_inv,HI_BBP_CTU_W_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_abbif_fmt_w_rxb_i_inv,w_rxb_i_inv,HI_BBP_CTU_W_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_abbif_fmt_w_rxa_iq_exchange,w_rxa_iq_exchange,HI_BBP_CTU_W_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_abbif_fmt_w_rxa_q_inv,w_rxa_q_inv,HI_BBP_CTU_W_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_abbif_fmt_w_rxa_i_inv,w_rxa_i_inv,HI_BBP_CTU_W_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_abbif_fmt_w_tx_iq_exchange,w_tx_iq_exchange,HI_BBP_CTU_W_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_abbif_fmt_w_tx_q_inv,w_tx_q_inv,HI_BBP_CTU_W_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_abbif_fmt_w_tx_i_inv,w_tx_i_inv,HI_BBP_CTU_W_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_abbif_fmt_reserved_0,reserved_0,HI_BBP_CTU_W_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_abbif_fmt_reserved_1,reserved_1,HI_BBP_CTU_TDS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_abbif_fmt_tds_rxb_iq_exchange,tds_rxb_iq_exchange,HI_BBP_CTU_TDS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_abbif_fmt_tds_rxb_q_inv,tds_rxb_q_inv,HI_BBP_CTU_TDS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_abbif_fmt_tds_rxb_i_inv,tds_rxb_i_inv,HI_BBP_CTU_TDS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_abbif_fmt_tds_rxa_iq_exchange,tds_rxa_iq_exchange,HI_BBP_CTU_TDS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_abbif_fmt_tds_rxa_q_inv,tds_rxa_q_inv,HI_BBP_CTU_TDS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_abbif_fmt_tds_rxa_i_inv,tds_rxa_i_inv,HI_BBP_CTU_TDS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_abbif_fmt_tds_tx_iq_exchange,tds_tx_iq_exchange,HI_BBP_CTU_TDS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_abbif_fmt_tds_tx_q_inv,tds_tx_q_inv,HI_BBP_CTU_TDS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_abbif_fmt_tds_tx_i_inv,tds_tx_i_inv,HI_BBP_CTU_TDS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_tds_abbif_fmt_reserved_0,reserved_0,HI_BBP_CTU_TDS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_TDS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_abbif_fmt_reserved_1,reserved_1,HI_BBP_CTU_GM_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_abbif_fmt_gm_rxb_iq_exchange,gm_rxb_iq_exchange,HI_BBP_CTU_GM_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_abbif_fmt_gm_rxb_q_inv,gm_rxb_q_inv,HI_BBP_CTU_GM_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_abbif_fmt_gm_rxb_i_inv,gm_rxb_i_inv,HI_BBP_CTU_GM_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_abbif_fmt_gm_rxa_iq_exchange,gm_rxa_iq_exchange,HI_BBP_CTU_GM_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_abbif_fmt_gm_rxa_q_inv,gm_rxa_q_inv,HI_BBP_CTU_GM_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_abbif_fmt_gm_rxa_i_inv,gm_rxa_i_inv,HI_BBP_CTU_GM_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_abbif_fmt_gm_tx_iq_exchange,gm_tx_iq_exchange,HI_BBP_CTU_GM_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_abbif_fmt_gm_tx_q_inv,gm_tx_q_inv,HI_BBP_CTU_GM_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_abbif_fmt_gm_tx_i_inv,gm_tx_i_inv,HI_BBP_CTU_GM_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_abbif_fmt_reserved_0,reserved_0,HI_BBP_CTU_GM_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_abbif_fmt_reserved_1,reserved_1,HI_BBP_CTU_GS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_abbif_fmt_gs_rxb_iq_exchange,gs_rxb_iq_exchange,HI_BBP_CTU_GS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_abbif_fmt_gs_rxb_q_inv,gs_rxb_q_inv,HI_BBP_CTU_GS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_abbif_fmt_gs_rxb_i_inv,gs_rxb_i_inv,HI_BBP_CTU_GS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_abbif_fmt_gs_rxa_iq_exchange,gs_rxa_iq_exchange,HI_BBP_CTU_GS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_abbif_fmt_gs_rxa_q_inv,gs_rxa_q_inv,HI_BBP_CTU_GS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_abbif_fmt_gs_rxa_i_inv,gs_rxa_i_inv,HI_BBP_CTU_GS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_abbif_fmt_gs_tx_iq_exchange,gs_tx_iq_exchange,HI_BBP_CTU_GS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_abbif_fmt_gs_tx_q_inv,gs_tx_q_inv,HI_BBP_CTU_GS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_abbif_fmt_gs_tx_i_inv,gs_tx_i_inv,HI_BBP_CTU_GS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_abbif_fmt_reserved_0,reserved_0,HI_BBP_CTU_GS_ABBIF_FMT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_ABBIF_FMT_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg0_for_use_reg0_for_use,reg0_for_use,HI_BBP_CTU_REG0_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG0_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg1_for_use_reg1_for_use,reg1_for_use,HI_BBP_CTU_REG1_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG1_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg2_for_use_reg2_for_use,reg2_for_use,HI_BBP_CTU_REG2_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG2_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg3_for_use_reg3_for_use,reg3_for_use,HI_BBP_CTU_REG3_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG3_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg4_for_use_reg4_for_use,reg4_for_use,HI_BBP_CTU_REG4_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG4_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg5_for_use_reg5_for_use,reg5_for_use,HI_BBP_CTU_REG5_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG5_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg6_for_use_reg6_for_use,reg6_for_use,HI_BBP_CTU_REG6_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG6_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg7_for_use_reg7_for_use,reg7_for_use,HI_BBP_CTU_REG7_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG7_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg8_for_use_reg8_for_use,reg8_for_use,HI_BBP_CTU_REG8_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG8_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg9_for_use_reg9_for_use,reg9_for_use,HI_BBP_CTU_REG9_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG9_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg10_for_use_reg10_for_use,reg10_for_use,HI_BBP_CTU_REG10_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG10_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg11_for_use_reg11_for_use,reg11_for_use,HI_BBP_CTU_REG11_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG11_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg12_for_use_reg12_for_use,reg12_for_use,HI_BBP_CTU_REG12_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG12_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg13_for_use_reg13_for_use,reg13_for_use,HI_BBP_CTU_REG13_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG13_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg14_for_use_reg14_for_use,reg14_for_use,HI_BBP_CTU_REG14_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG14_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg15_for_use_reg15_for_use,reg15_for_use,HI_BBP_CTU_REG15_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG15_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg16_for_use_reg16_for_use,reg16_for_use,HI_BBP_CTU_REG16_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG16_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg17_for_use_reg17_for_use,reg17_for_use,HI_BBP_CTU_REG17_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG17_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg18_for_use_reg18_for_use,reg18_for_use,HI_BBP_CTU_REG18_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG18_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg19_for_use_reg19_for_use,reg19_for_use,HI_BBP_CTU_REG19_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG19_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg20_for_use_reg20_for_use,reg20_for_use,HI_BBP_CTU_REG20_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG20_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg21_for_use_reg21_for_use,reg21_for_use,HI_BBP_CTU_REG21_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG21_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg22_for_use_reg22_for_use,reg22_for_use,HI_BBP_CTU_REG22_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG22_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg23_for_use_reg23_for_use,reg23_for_use,HI_BBP_CTU_REG23_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG23_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg24_for_use_reg24_for_use,reg24_for_use,HI_BBP_CTU_REG24_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG24_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg25_for_use_reg25_for_use,reg25_for_use,HI_BBP_CTU_REG25_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG25_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg26_for_use_reg26_for_use,reg26_for_use,HI_BBP_CTU_REG26_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG26_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg27_for_use_reg27_for_use,reg27_for_use,HI_BBP_CTU_REG27_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG27_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg28_for_use_reg28_for_use,reg28_for_use,HI_BBP_CTU_REG28_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG28_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg29_for_use_reg29_for_use,reg29_for_use,HI_BBP_CTU_REG29_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG29_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg30_for_use_reg30_for_use,reg30_for_use,HI_BBP_CTU_REG30_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG30_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg31_for_use_reg31_for_use,reg31_for_use,HI_BBP_CTU_REG31_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG31_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg32_for_use_reg32_for_use,reg32_for_use,HI_BBP_CTU_REG32_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG32_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg33_for_use_reg33_for_use,reg33_for_use,HI_BBP_CTU_REG33_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG33_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg34_for_use_reg34_for_use,reg34_for_use,HI_BBP_CTU_REG34_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG34_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg35_for_use_reg35_for_use,reg35_for_use,HI_BBP_CTU_REG35_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG35_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg36_for_use_reg36_for_use,reg36_for_use,HI_BBP_CTU_REG36_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG36_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg37_for_use_reg37_for_use,reg37_for_use,HI_BBP_CTU_REG37_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG37_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg38_for_use_reg38_for_use,reg38_for_use,HI_BBP_CTU_REG38_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG38_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg39_for_use_reg39_for_use,reg39_for_use,HI_BBP_CTU_REG39_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG39_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg40_for_use_reg40_for_use,reg40_for_use,HI_BBP_CTU_REG40_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG40_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg41_for_use_reg41_for_use,reg41_for_use,HI_BBP_CTU_REG41_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG41_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg42_for_use_reg42_for_use,reg42_for_use,HI_BBP_CTU_REG42_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG42_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg43_for_use_reg43_for_use,reg43_for_use,HI_BBP_CTU_REG43_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG43_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg44_for_use_reg44_for_use,reg44_for_use,HI_BBP_CTU_REG44_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG44_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg45_for_use_reg45_for_use,reg45_for_use,HI_BBP_CTU_REG45_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG45_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg46_for_use_reg46_for_use,reg46_for_use,HI_BBP_CTU_REG46_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG46_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_reg47_for_use_reg47_for_use,reg47_for_use,HI_BBP_CTU_REG47_FOR_USE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_REG47_FOR_USE_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg0_cpu_dbg_reg0_cpu,dbg_reg0_cpu,HI_BBP_CTU_DBG_REG0_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG0_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg1_cpu_dbg_reg1_cpu,dbg_reg1_cpu,HI_BBP_CTU_DBG_REG1_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG1_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg2_cpu_dbg_reg2_cpu,dbg_reg2_cpu,HI_BBP_CTU_DBG_REG2_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG2_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg3_cpu_dbg_reg3_cpu,dbg_reg3_cpu,HI_BBP_CTU_DBG_REG3_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG3_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg4_cpu_dbg_reg4_cpu,dbg_reg4_cpu,HI_BBP_CTU_DBG_REG4_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG4_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg5_cpu_dbg_reg5_cpu,dbg_reg5_cpu,HI_BBP_CTU_DBG_REG5_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG5_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg6_cpu_dbg_reg6_cpu,dbg_reg6_cpu,HI_BBP_CTU_DBG_REG6_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG6_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg7_cpu_dbg_reg7_cpu,dbg_reg7_cpu,HI_BBP_CTU_DBG_REG7_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG7_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg8_cpu_dbg_reg8_cpu,dbg_reg8_cpu,HI_BBP_CTU_DBG_REG8_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG8_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg9_cpu_dbg_reg9_cpu,dbg_reg9_cpu,HI_BBP_CTU_DBG_REG9_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG9_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg10_cpu_dbg_reg10_cpu,dbg_reg10_cpu,HI_BBP_CTU_DBG_REG10_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG10_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg11_cpu_dbg_reg11_cpu,dbg_reg11_cpu,HI_BBP_CTU_DBG_REG11_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG11_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg12_cpu_dbg_reg12_cpu,dbg_reg12_cpu,HI_BBP_CTU_DBG_REG12_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG12_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg13_cpu_dbg_reg13_cpu,dbg_reg13_cpu,HI_BBP_CTU_DBG_REG13_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG13_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg14_cpu_dbg_reg14_cpu,dbg_reg14_cpu,HI_BBP_CTU_DBG_REG14_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG14_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg15_cpu_dbg_reg15_cpu,dbg_reg15_cpu,HI_BBP_CTU_DBG_REG15_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG15_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg16_cpu_dbg_reg16_cpu,dbg_reg16_cpu,HI_BBP_CTU_DBG_REG16_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG16_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg17_cpu_dbg_reg17_cpu,dbg_reg17_cpu,HI_BBP_CTU_DBG_REG17_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG17_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg18_cpu_dbg_reg18_cpu,dbg_reg18_cpu,HI_BBP_CTU_DBG_REG18_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG18_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg19_cpu_dbg_reg19_cpu,dbg_reg19_cpu,HI_BBP_CTU_DBG_REG19_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG19_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_clk_gate_en_dsp_dbg_clk_gate_en,dsp_dbg_clk_gate_en,HI_BBP_CTU_DBG_CLK_GATE_EN_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_CLK_GATE_EN_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_clk_gate_en_dsp_dbg_clk_bypass,dsp_dbg_clk_bypass,HI_BBP_CTU_DBG_CLK_GATE_EN_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_CLK_GATE_EN_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_clk_gate_en_reserved,reserved,HI_BBP_CTU_DBG_CLK_GATE_EN_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_CLK_GATE_EN_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2tds_busy_rf_ssi1,com2tds_busy_rf_ssi1,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2tds_busy_rf_ssi0,com2tds_busy_rf_ssi0,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2w_busy_rf_ssi1,com2w_busy_rf_ssi1,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2w_busy_rf_ssi0,com2w_busy_rf_ssi0,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2lte_busy_rf_ssi1,com2lte_busy_rf_ssi1,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2lte_busy_rf_ssi0,com2lte_busy_rf_ssi0,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2g2_busy_rf_ssi1,com2g2_busy_rf_ssi1,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2g2_busy_rf_ssi0,com2g2_busy_rf_ssi0,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2g1_busy_rf_ssi1,com2g1_busy_rf_ssi1,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2g1_busy_rf_ssi0,com2g1_busy_rf_ssi0,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2tds_busy_mipi1,com2tds_busy_mipi1,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2tds_busy_mipi0,com2tds_busy_mipi0,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2w_busy_mipi1,com2w_busy_mipi1,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2w_busy_mipi0,com2w_busy_mipi0,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2lte_busy_mipi1,com2lte_busy_mipi1,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2lte_busy_mipi0,com2lte_busy_mipi0,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2g2_busy_mipi1,com2g2_busy_mipi1,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2g2_busy_mipi0,com2g2_busy_mipi0,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2g1_busy_mipi1,com2g1_busy_mipi1,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_com2g1_busy_mipi0,com2g1_busy_mipi0,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_reg20_cpu_reserved,reserved,HI_BBP_CTU_DBG_REG20_CPU_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_REG20_CPU_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_comm_en_dbg_comm_en,dbg_comm_en,HI_BBP_CTU_DBG_COMM_EN_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_COMM_EN_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_comm_en_reserved,reserved,HI_BBP_CTU_DBG_COMM_EN_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_COMM_EN_OFFSET)
HI_SET_GET(hi_bbp_ctu_l_ch_sw_l_ch_sw_rf,l_ch_sw_rf,HI_BBP_CTU_L_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_L_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_l_ch_sw_l_ch_sw_mipi,l_ch_sw_mipi,HI_BBP_CTU_L_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_L_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_l_ch_sw_l_ch_sw_abb_tx,l_ch_sw_abb_tx,HI_BBP_CTU_L_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_L_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_l_ch_sw_l_ch_sw_abb_rx,l_ch_sw_abb_rx,HI_BBP_CTU_L_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_L_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_l_ch_sw_l_ch_sw_pmu,l_ch_sw_pmu,HI_BBP_CTU_L_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_L_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_l_ch_sw_l_ch_sw_apt,l_ch_sw_apt,HI_BBP_CTU_L_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_L_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_l_ch_sw_reserved,reserved,HI_BBP_CTU_L_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_L_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_ch_sw_w_ch_sw_rf,w_ch_sw_rf,HI_BBP_CTU_W_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_ch_sw_w_ch_sw_mipi,w_ch_sw_mipi,HI_BBP_CTU_W_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_ch_sw_w_ch_sw_abb_tx,w_ch_sw_abb_tx,HI_BBP_CTU_W_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_ch_sw_w_ch_sw_abb_rx,w_ch_sw_abb_rx,HI_BBP_CTU_W_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_ch_sw_w_ch_sw_pmu,w_ch_sw_pmu,HI_BBP_CTU_W_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_ch_sw_w_ch_sw_apt,w_ch_sw_apt,HI_BBP_CTU_W_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_ch_sw_reserved,reserved,HI_BBP_CTU_W_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_t_ch_sw_t_ch_sw_rf,t_ch_sw_rf,HI_BBP_CTU_T_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_T_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_t_ch_sw_t_ch_sw_mipi,t_ch_sw_mipi,HI_BBP_CTU_T_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_T_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_t_ch_sw_t_ch_sw_abb_tx,t_ch_sw_abb_tx,HI_BBP_CTU_T_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_T_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_t_ch_sw_t_ch_sw_abb_rx,t_ch_sw_abb_rx,HI_BBP_CTU_T_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_T_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_t_ch_sw_t_ch_sw_pmu,t_ch_sw_pmu,HI_BBP_CTU_T_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_T_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_t_ch_sw_t_ch_sw_apt,t_ch_sw_apt,HI_BBP_CTU_T_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_T_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_t_ch_sw_reserved,reserved,HI_BBP_CTU_T_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_T_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_ch_sw_gm_ch_sw_rf,gm_ch_sw_rf,HI_BBP_CTU_GM_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_ch_sw_gm_ch_sw_mipi,gm_ch_sw_mipi,HI_BBP_CTU_GM_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_ch_sw_gm_ch_sw_abb_tx,gm_ch_sw_abb_tx,HI_BBP_CTU_GM_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_ch_sw_gm_ch_sw_abb_rx,gm_ch_sw_abb_rx,HI_BBP_CTU_GM_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_ch_sw_gm_ch_sw_pmu,gm_ch_sw_pmu,HI_BBP_CTU_GM_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_ch_sw_gm_ch_sw_apt,gm_ch_sw_apt,HI_BBP_CTU_GM_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_ch_sw_reserved,reserved,HI_BBP_CTU_GM_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_ch_sw_gs_ch_sw_rf,gs_ch_sw_rf,HI_BBP_CTU_GS_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_ch_sw_gs_ch_sw_mipi,gs_ch_sw_mipi,HI_BBP_CTU_GS_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_ch_sw_gs_ch_sw_abb_tx,gs_ch_sw_abb_tx,HI_BBP_CTU_GS_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_ch_sw_gs_ch_sw_abb_rx,gs_ch_sw_abb_rx,HI_BBP_CTU_GS_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_ch_sw_gs_ch_sw_pmu,gs_ch_sw_pmu,HI_BBP_CTU_GS_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_ch_sw_gs_ch_sw_apt,gs_ch_sw_apt,HI_BBP_CTU_GS_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_ch_sw_reserved,reserved,HI_BBP_CTU_GS_CH_SW_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_CH_SW_OFFSET)
HI_SET_GET(hi_bbp_ctu_l_tcvr_sel0_l_tcvr_sel0,l_tcvr_sel0,HI_BBP_CTU_L_TCVR_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_L_TCVR_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_l_tcvr_sel0_reserved,reserved,HI_BBP_CTU_L_TCVR_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_L_TCVR_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_l_tcvr_sel1_l_tcvr_sel1,l_tcvr_sel1,HI_BBP_CTU_L_TCVR_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_L_TCVR_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_l_tcvr_sel1_reserved,reserved,HI_BBP_CTU_L_TCVR_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_L_TCVR_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_t_tcvr_sel0_t_tcvr_sel0,t_tcvr_sel0,HI_BBP_CTU_T_TCVR_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_T_TCVR_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_t_tcvr_sel0_reserved,reserved,HI_BBP_CTU_T_TCVR_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_T_TCVR_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_t_tcvr_sel1_t_tcvr_sel1,t_tcvr_sel1,HI_BBP_CTU_T_TCVR_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_T_TCVR_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_t_tcvr_sel1_reserved,reserved,HI_BBP_CTU_T_TCVR_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_T_TCVR_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_tcvr_sel0_w_tcvr_sel0,w_tcvr_sel0,HI_BBP_CTU_W_TCVR_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_TCVR_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_tcvr_sel0_reserved,reserved,HI_BBP_CTU_W_TCVR_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_TCVR_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_tcvr_sel1_w_tcvr_sel1,w_tcvr_sel1,HI_BBP_CTU_W_TCVR_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_TCVR_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_tcvr_sel1_reserved,reserved,HI_BBP_CTU_W_TCVR_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_TCVR_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_tcvr_sel0_gm_tcvr_sel0,gm_tcvr_sel0,HI_BBP_CTU_GM_TCVR_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_TCVR_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_tcvr_sel0_reserved,reserved,HI_BBP_CTU_GM_TCVR_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_TCVR_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_tcvr_sel1_gm_tcvr_sel1,gm_tcvr_sel1,HI_BBP_CTU_GM_TCVR_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_TCVR_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_tcvr_sel1_reserved,reserved,HI_BBP_CTU_GM_TCVR_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_TCVR_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_tcvr_sel0_gs_tcvr_sel0,gs_tcvr_sel0,HI_BBP_CTU_GS_TCVR_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_TCVR_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_tcvr_sel0_reserved,reserved,HI_BBP_CTU_GS_TCVR_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_TCVR_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_tcvr_sel1_gs_tcvr_sel1,gs_tcvr_sel1,HI_BBP_CTU_GS_TCVR_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_TCVR_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_tcvr_sel1_reserved,reserved,HI_BBP_CTU_GS_TCVR_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_TCVR_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_l_tcvr_value0_l_tcvr_value0,l_tcvr_value0,HI_BBP_CTU_L_TCVR_VALUE0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_L_TCVR_VALUE0_OFFSET)
HI_SET_GET(hi_bbp_ctu_l_tcvr_value0_reserved,reserved,HI_BBP_CTU_L_TCVR_VALUE0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_L_TCVR_VALUE0_OFFSET)
HI_SET_GET(hi_bbp_ctu_l_tcvr_value1_l_tcvr_value1,l_tcvr_value1,HI_BBP_CTU_L_TCVR_VALUE1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_L_TCVR_VALUE1_OFFSET)
HI_SET_GET(hi_bbp_ctu_l_tcvr_value1_reserved,reserved,HI_BBP_CTU_L_TCVR_VALUE1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_L_TCVR_VALUE1_OFFSET)
HI_SET_GET(hi_bbp_ctu_t_tcvr_value0_t_tcvr_value0,t_tcvr_value0,HI_BBP_CTU_T_TCVR_VALUE0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_T_TCVR_VALUE0_OFFSET)
HI_SET_GET(hi_bbp_ctu_t_tcvr_value0_reserved,reserved,HI_BBP_CTU_T_TCVR_VALUE0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_T_TCVR_VALUE0_OFFSET)
HI_SET_GET(hi_bbp_ctu_t_tcvr_value1_t_tcvr_value1,t_tcvr_value1,HI_BBP_CTU_T_TCVR_VALUE1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_T_TCVR_VALUE1_OFFSET)
HI_SET_GET(hi_bbp_ctu_t_tcvr_value1_reserved,reserved,HI_BBP_CTU_T_TCVR_VALUE1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_T_TCVR_VALUE1_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_tcvr_value0_w_tcvr_value0,w_tcvr_value0,HI_BBP_CTU_W_TCVR_VALUE0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_TCVR_VALUE0_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_tcvr_value0_reserved,reserved,HI_BBP_CTU_W_TCVR_VALUE0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_TCVR_VALUE0_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_tcvr_value1_w_tcvr_value1,w_tcvr_value1,HI_BBP_CTU_W_TCVR_VALUE1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_TCVR_VALUE1_OFFSET)
HI_SET_GET(hi_bbp_ctu_w_tcvr_value1_reserved,reserved,HI_BBP_CTU_W_TCVR_VALUE1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_W_TCVR_VALUE1_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_tcvr_value0_gm_tcvr_value0,gm_tcvr_value0,HI_BBP_CTU_GM_TCVR_VALUE0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_TCVR_VALUE0_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_tcvr_value0_reserved,reserved,HI_BBP_CTU_GM_TCVR_VALUE0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_TCVR_VALUE0_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_tcvr_value1_gm_tcvr_value1,gm_tcvr_value1,HI_BBP_CTU_GM_TCVR_VALUE1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_TCVR_VALUE1_OFFSET)
HI_SET_GET(hi_bbp_ctu_gm_tcvr_value1_reserved,reserved,HI_BBP_CTU_GM_TCVR_VALUE1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GM_TCVR_VALUE1_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_tcvr_value0_gs_tcvr_value0,gs_tcvr_value0,HI_BBP_CTU_GS_TCVR_VALUE0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_TCVR_VALUE0_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_tcvr_value0_reserved,reserved,HI_BBP_CTU_GS_TCVR_VALUE0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_TCVR_VALUE0_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_tcvr_value1_gs_tcvr_value1,gs_tcvr_value1,HI_BBP_CTU_GS_TCVR_VALUE1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_TCVR_VALUE1_OFFSET)
HI_SET_GET(hi_bbp_ctu_gs_tcvr_value1_reserved,reserved,HI_BBP_CTU_GS_TCVR_VALUE1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_GS_TCVR_VALUE1_OFFSET)
HI_SET_GET(hi_bbp_ctu_rf_tcvr_state_l_tcvr_state0,l_tcvr_state0,HI_BBP_CTU_RF_TCVR_STATE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RF_TCVR_STATE_OFFSET)
HI_SET_GET(hi_bbp_ctu_rf_tcvr_state_l_tcvr_state1,l_tcvr_state1,HI_BBP_CTU_RF_TCVR_STATE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RF_TCVR_STATE_OFFSET)
HI_SET_GET(hi_bbp_ctu_rf_tcvr_state_t_tcvr_state0,t_tcvr_state0,HI_BBP_CTU_RF_TCVR_STATE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RF_TCVR_STATE_OFFSET)
HI_SET_GET(hi_bbp_ctu_rf_tcvr_state_reserved_4,reserved_4,HI_BBP_CTU_RF_TCVR_STATE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RF_TCVR_STATE_OFFSET)
HI_SET_GET(hi_bbp_ctu_rf_tcvr_state_w_tcvr_state0,w_tcvr_state0,HI_BBP_CTU_RF_TCVR_STATE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RF_TCVR_STATE_OFFSET)
HI_SET_GET(hi_bbp_ctu_rf_tcvr_state_reserved_3,reserved_3,HI_BBP_CTU_RF_TCVR_STATE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RF_TCVR_STATE_OFFSET)
HI_SET_GET(hi_bbp_ctu_rf_tcvr_state_gm_tcvr_state0,gm_tcvr_state0,HI_BBP_CTU_RF_TCVR_STATE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RF_TCVR_STATE_OFFSET)
HI_SET_GET(hi_bbp_ctu_rf_tcvr_state_reserved_2,reserved_2,HI_BBP_CTU_RF_TCVR_STATE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RF_TCVR_STATE_OFFSET)
HI_SET_GET(hi_bbp_ctu_rf_tcvr_state_gs_tcvr_state0,gs_tcvr_state0,HI_BBP_CTU_RF_TCVR_STATE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RF_TCVR_STATE_OFFSET)
HI_SET_GET(hi_bbp_ctu_rf_tcvr_state_reserved_1,reserved_1,HI_BBP_CTU_RF_TCVR_STATE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RF_TCVR_STATE_OFFSET)
HI_SET_GET(hi_bbp_ctu_rf_tcvr_state_ch_tcvr_state0,ch_tcvr_state0,HI_BBP_CTU_RF_TCVR_STATE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RF_TCVR_STATE_OFFSET)
HI_SET_GET(hi_bbp_ctu_rf_tcvr_state_ch_tcvr_state1,ch_tcvr_state1,HI_BBP_CTU_RF_TCVR_STATE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RF_TCVR_STATE_OFFSET)
HI_SET_GET(hi_bbp_ctu_rf_tcvr_state_reserved_0,reserved_0,HI_BBP_CTU_RF_TCVR_STATE_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_RF_TCVR_STATE_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi_unbind_en_mipi_unbind_en,mipi_unbind_en,HI_BBP_CTU_MIPI_UNBIND_EN_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI_UNBIND_EN_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi_unbind_en_reserved,reserved,HI_BBP_CTU_MIPI_UNBIND_EN_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI_UNBIND_EN_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi_ch_sel0_mipi_ch_sel0,mipi_ch_sel0,HI_BBP_CTU_MIPI_CH_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI_CH_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi_ch_sel0_reserved,reserved,HI_BBP_CTU_MIPI_CH_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI_CH_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi_ch_sel1_mipi_ch_sel1,mipi_ch_sel1,HI_BBP_CTU_MIPI_CH_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI_CH_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_mipi_ch_sel1_reserved,reserved,HI_BBP_CTU_MIPI_CH_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_MIPI_CH_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_apt_ch_sel0_apt_ch_sel0,apt_ch_sel0,HI_BBP_CTU_APT_CH_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_APT_CH_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_apt_ch_sel0_reserved,reserved,HI_BBP_CTU_APT_CH_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_APT_CH_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_apt_ch_sel1_apt_ch_sel1,apt_ch_sel1,HI_BBP_CTU_APT_CH_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_APT_CH_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_apt_ch_sel1_reserved,reserved,HI_BBP_CTU_APT_CH_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_APT_CH_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_afc_ch_sel0_afc_ch_sel0,afc_ch_sel0,HI_BBP_CTU_AFC_CH_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_AFC_CH_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_afc_ch_sel0_reserved,reserved,HI_BBP_CTU_AFC_CH_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_AFC_CH_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_afc_ch_sel1_afc_ch_sel1,afc_ch_sel1,HI_BBP_CTU_AFC_CH_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_AFC_CH_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_afc_ch_sel1_reserved,reserved,HI_BBP_CTU_AFC_CH_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_AFC_CH_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb_tx_ch_sel0_abb_tx_ch_sel0,abb_tx_ch_sel0,HI_BBP_CTU_ABB_TX_CH_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB_TX_CH_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb_tx_ch_sel0_reserved,reserved,HI_BBP_CTU_ABB_TX_CH_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB_TX_CH_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb_tx_ch_sel1_abb_tx_ch_sel1,abb_tx_ch_sel1,HI_BBP_CTU_ABB_TX_CH_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB_TX_CH_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_abb_tx_ch_sel1_reserved,reserved,HI_BBP_CTU_ABB_TX_CH_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_ABB_TX_CH_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_pmu_ch_sel0_pmu_ch_sel0,pmu_ch_sel0,HI_BBP_CTU_PMU_CH_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_PMU_CH_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_pmu_ch_sel0_reserved,reserved,HI_BBP_CTU_PMU_CH_SEL0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_PMU_CH_SEL0_OFFSET)
HI_SET_GET(hi_bbp_ctu_pmu_ch_sel1_pmu_ch_sel1,pmu_ch_sel1,HI_BBP_CTU_PMU_CH_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_PMU_CH_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_pmu_ch_sel1_reserved,reserved,HI_BBP_CTU_PMU_CH_SEL1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_PMU_CH_SEL1_OFFSET)
HI_SET_GET(hi_bbp_ctu_valid_num_valid_num,valid_num,HI_BBP_CTU_VALID_NUM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_VALID_NUM_OFFSET)
HI_SET_GET(hi_bbp_ctu_valid_num_reserved,reserved,HI_BBP_CTU_VALID_NUM_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_VALID_NUM_OFFSET)
HI_SET_GET(hi_bbp_ctu_valid_num1_valid_num1,valid_num1,HI_BBP_CTU_VALID_NUM1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_VALID_NUM1_OFFSET)
HI_SET_GET(hi_bbp_ctu_valid_num1_valid_num2,valid_num2,HI_BBP_CTU_VALID_NUM1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_VALID_NUM1_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_en_dbg_en,dbg_en,HI_BBP_CTU_DBG_EN_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_EN_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_en_dbg_rpt_mode,dbg_rpt_mode,HI_BBP_CTU_DBG_EN_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_EN_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_en_reserved,reserved,HI_BBP_CTU_DBG_EN_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_EN_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_en_dbg_pkg_num,dbg_pkg_num,HI_BBP_CTU_DBG_EN_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_EN_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_id_dbg_id,dbg_id,HI_BBP_CTU_DBG_ID_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_ID_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_flt_dbg_flt,dbg_flt,HI_BBP_CTU_DBG_FLT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_FLT_OFFSET)
HI_SET_GET(hi_bbp_ctu_dbg_flt_reserved,reserved,HI_BBP_CTU_DBG_FLT_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_DBG_FLT_OFFSET)
HI_SET_GET(hi_bbp_ctu_bbc_rev0_bbc_rev0,bbc_rev0,HI_BBP_CTU_BBC_REV0_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_BBC_REV0_OFFSET)
HI_SET_GET(hi_bbp_ctu_bbc_rev1_bbc_rev1,bbc_rev1,HI_BBP_CTU_BBC_REV1_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_BBC_REV1_OFFSET)
HI_SET_GET(hi_bbp_ctu_bbc_rev2_bbc_rev2,bbc_rev2,HI_BBP_CTU_BBC_REV2_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_BBC_REV2_OFFSET)
HI_SET_GET(hi_bbp_ctu_bbc_rev3_bbc_rev3,bbc_rev3,HI_BBP_CTU_BBC_REV3_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_BBC_REV3_OFFSET)
HI_SET_GET(hi_bbp_ctu_bbc_rev4_bbc_rev4,bbc_rev4,HI_BBP_CTU_BBC_REV4_T,HI_BBP_CTU_BASE_ADDR, HI_BBP_CTU_BBC_REV4_OFFSET)

#endif // __HI_BBP_CTU_H__

