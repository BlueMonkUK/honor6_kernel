/******************************************************************************/
/*  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_bbp_tstu.h*/
/* Version       : 2.0 */
/* Created       : 2013-09-13*/
/* Last Modified : */
/* Description   :  The C union definition file for the module bbp_tstu*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_BBP_TSTU_H__
#define __HI_BBP_TSTU_H__

/*
 * Project: hi
 * Module : bbp_tstu
 */

#ifndef HI_SET_GET
#define HI_SET_GET(a0,a1,a2,a3,a4)
#endif

/********************************************************************************/
/*    bbp_tstu �Ĵ���ƫ�ƶ��壨��Ŀ��_ģ����_�Ĵ�����_OFFSET)        */
/********************************************************************************/
#define    HI_BBP_TSTU_SOFT_RST_OFFSET                       (0x0) /* ��λ */
#define    HI_BBP_TSTU_SFN_OFFSET                            (0x10) /* ��֡�żĴ��� */
#define    HI_BBP_TSTU_SFN_OFT_OFFSET                        (0x14) /* ��֡��ƫ�ƼĴ��� */
#define    HI_BBP_TSTU_SFRM_FRAML_OFFSET                     (0x18) /* ��֡���ȼĴ��� */
#define    HI_BBP_TSTU_SFRM_FRAML_SET_OFFSET                 (0x1C) /* ��֡���ȵ����Ĵ��� */
#define    HI_BBP_TSTU_SLOT_INT_POS_OFFSET                   (0x20) /* DSPʱ϶�ж�λ�üĴ��� */
#define    HI_BBP_TSTU_TIME_ADJ_START_OFFSET                 (0x24) /* ��ʱ����ʹ�� */
#define    HI_BBP_TSTU_TIME_ADJ_ALM_OFFSET                   (0x28) /* ��ʱ�����澯 */
#define    HI_BBP_TSTU_DBG_TIME_SEL_OFFSET                   (0x2C)
#define    HI_BBP_TSTU_CLK_MSR_PRD_OFFSET                    (0x40) /* ʱ�Ӳ������� */
#define    HI_BBP_TSTU_CLK_STATE_CLEAR_OFFSET                (0x44) /* ʱ�Ӳ���״̬��� */
#define    HI_BBP_TSTU_CLK_MSR_START_OFFSET                  (0x48) /* ʱ�Ӳ���ʹ�� */
#define    HI_BBP_TSTU_LAYOUT_TIME_OFFSET                    (0x70) /* ����滮ʱ��Ĵ��� */
#define    HI_BBP_TSTU_LAYOUT_CHIP_OFFSET                    (0x74) /* ����滮ʱ����ƼĴ��� */
#define    HI_BBP_TSTU_LAYOUT_START_OFFSET                   (0x78) /* ����滮���ƼĴ��� */
#define    HI_BBP_TSTU_PUB_INT_MSK_OFFSET                    (0xA0) /* ��֡�ж����μĴ��� */
#define    HI_BBP_TSTU_DSP_INT_MSK_OFFSET                    (0xA4) /* DSPʱ϶�ж����μĴ��� */
#define    HI_BBP_TSTU_PUB_INT_CLEAR_OFFSET                  (0xA8) /* ��֡�ж�����Ĵ��� */
#define    HI_BBP_TSTU_DSP_INT_CLEAR_OFFSET                  (0xAC) /* DSPʱ϶�ж�����Ĵ��� */
#define    HI_BBP_TSTU_PUB_INT_ALM_OFFSET                    (0xB0) /* ��֡�жϸ澯�Ĵ��� */
#define    HI_BBP_TSTU_DSP_INT_ALM_OFFSET                    (0xB4) /* DSPʱ϶�жϸ澯�Ĵ��� */
#define    HI_BBP_TSTU_STU_CFG1_OFFSET                       (0x100) /* �����Ĵ��� */
#define    HI_BBP_TSTU_STU_CFG2_OFFSET                       (0x104) /* �����Ĵ��� */
#define    HI_BBP_TSTU_STU_CFG3_OFFSET                       (0x108) /* �����Ĵ��� */
#define    HI_BBP_TSTU_STU_CFG4_OFFSET                       (0x10C) /* �����Ĵ��� */
#define    HI_BBP_TSTU_TDS_STU_FRAMC_OFFSET                  (0x200) /* ��֡�ڼ����� */
#define    HI_BBP_TSTU_TDS_STU_SFN_OFFSET                    (0x204) /* ��֡�� */
#define    HI_BBP_TSTU_TDS_STU_FRAML_OFFSET                  (0x208) /* ��ǰ��֡���� */
#define    HI_BBP_TSTU_TDS_STU_SLOT_OFFSET                   (0x20C) /* ��ǰʱ϶�� */
#define    HI_BBP_TSTU_CLK_MSR_CNT_OFFSET                    (0x220) /* ʱ�Ӳ������ */
#define    HI_BBP_TSTU_CLK_MSR_STATE_OFFSET                  (0x224) /* ʱ�Ӳ���״̬ */
#define    HI_BBP_TSTU_LAYOUT_CUR_TIME_OFFSET                (0x230) /* ��ǰLAYOUTʣ��ʱ�� */
#define    HI_BBP_TSTU_LAYOUT_STATE_OFFSET                   (0x234) /* ����滮״̬�Ĵ��� */
#define    HI_BBP_TSTU_PUB_INT_STATE_OFFSET                  (0x240) /* ��֡�ж�״̬�Ĵ��� */
#define    HI_BBP_TSTU_DSP_INT_STATE_OFFSET                  (0x244) /* DSPʱ϶�ж�״̬�Ĵ��� */
#define    HI_BBP_TSTU_DBG_SNF_PRE_STU_OFFSET                (0x300) /* ����˯��ǰ�����stu����֡�żĴ��� */
#define    HI_BBP_TSTU_DBG_SFN_POST_OFFSET                   (0x304) /* �˳�˯�ߺ������drx��stu��֡�żĴ��� */
#define    HI_BBP_TSTU_DBG_FRAMC_PRE_STU_OFFSET              (0x308) /* ����˯��ǰ�����stu��֡�ڼ������Ĵ��� */
#define    HI_BBP_TSTU_DBG_FRAMC_POST_DRX_OFFSET             (0x30C) /* �˳�˯�ߺ������drx��֡�ڼ������Ĵ��� */
#define    HI_BBP_TSTU_DBG_FRAMC_POST_STU_OFFSET             (0x310) /* �˳�˯�ߺ������stu��֡�ڼ������Ĵ��� */

/********************************************************************************/
/*    bbp_tstu �Ĵ������壨��Ŀ��_ģ����_�Ĵ�����_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    soft_rst                   : 1; /* [0..0] STUģ����λ�Ĵ�����1'b1����λ��Ч����ʱ�̣�������Чʱ�̣����� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_SOFT_RST_T;    /* ��λ */

typedef union
{
    struct
    {
        unsigned int    sfn                        : 16; /* [15..0] ������֡�żĴ�����������ú���֡�ż��������ԸüĴ���Ϊ��ֵ������֡�ŵ��ۼӡ��ر�ע�⣬�üĴ�������ϵͳ֡������ʱ�̣����ö�ʱ����ʹ��ǰ��Чʱ�̣���ʱ����ʹ����Ч���Ҹ�ʹ����Чʱ����ǰ��֡��������֡�������޸�Ϊ����ֵ������Чһ�� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_SFN_T;    /* ��֡�żĴ��� */

typedef union
{
    struct
    {
        unsigned int    sfn_oft                    : 16; /* [15..0] ��֡��ƫ�ƼĴ�����ͨ���üĴ����ɵ�����֡�š���ϣ������ƫ����ΪB����üĴ���Ӧ����ΪB+1����ʱ�̣����ö�ʱ��������ǰ��Чʱ�̣���ʱ����ʱ����ǰ��֡����ʱ����ֵ���ۼӵ���ǰ��֡���ϣ�����Чһ�� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_SFN_OFT_T;    /* ��֡��ƫ�ƼĴ��� */

typedef union
{
    struct
    {
        unsigned int    sfrm_framl                 : 20; /* [19..0] TDS 5ms��֡���ȡ��üĴ���ֵΪ1/96 chip���ȣ�Ĭ��ֵΪ5ms��֡���ȼ�1����614339��֧����������á�����ʱ�̣�����ʱ����Чʱ�̣���֡�жϣ���һ��֡ */
        unsigned int    reserved                   : 12; /* [31..20] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_SFRM_FRAML_T;    /* ��֡���ȼĴ��� */

typedef union
{
    struct
    {
        unsigned int    sfrm_framl_set             : 20; /* [19..0] STD��֡���ȵ����Ĵ�����ͨ���üĴ���������ǰ��֡���ȡ������ö�ʱ����ʹ��ʱ��������õĸüĴ�����ֵС�ڵ��ڵ�ǰ��֡�ڼ�������ֵ����ǰ��֡���ȱ���ΪSFRM_FRAML��������֡������������Ϊ�üĴ�����ֵ��1/96 chip���ȡ����õ�ֵΪ��ʵֵ��1������ʱ�̣����ö�ʱ��������ǰ��Чʱ�̣���ʱ����ʹ����Чʱ��������Ч */
        unsigned int    reserved                   : 12; /* [31..20] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_SFRM_FRAML_SET_T;    /* ��֡���ȵ����Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    spec_slot_int_pos          : 16; /* [15..0] ����ʱ϶�ж�λ�üĴ�����ʱ϶�ж�Ĭ��λ����1/3��2/3ʱ϶λ�á�֧��Ĭ��ֵλ��ǰ��ɵ�1/16ʱ϶�����Ĵ������÷�ΧΪ9151~13375��1/96chip��������ʱ�̣�������Чʱ�̣����� */
        unsigned int    norm_slot_int_pos          : 16; /* [31..16] ��ͨʱ϶�ж�λ�üĴ�����ʱ϶�ж�Ĭ��λ����1/4��1/2��3/4ʱ϶λ�á�֧��Ĭ��ֵλ��ǰ��ɵ�1/16ʱ϶�����Ĵ������÷�ΧΪ15551~25919��1/96chip��������ʱ�̣�������Чʱ�̣����� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_SLOT_INT_POS_T;    /* DSPʱ϶�ж�λ�üĴ��� */

typedef union
{
    struct
    {
        unsigned int    time_track_start           : 1; /* [0..0] ��ʱ���������źţ�1'b1��������ʱ�л���������Ч����ʱ����ʱ���øüĴ������ڶ�ʱ����ʱ�����������ж�����ʱ�̣�����ʱ����Чʱ�̣��������߼����е��µ���֡ͷλ�� */
        unsigned int    time_switch_start          : 1; /* [1..1] ��ʱ�л������źţ�1'b1��������ʱ�л���������Ч����ʱ�л�ʱ���øüĴ������������ζ�ʱ�л������е��ж�����ʱ�̣�����ʱ����Чʱ�̣��������߼����е��µ���֡ͷλ�� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_TIME_ADJ_START_T;    /* ��ʱ����ʹ�� */

typedef union
{
    struct
    {
        unsigned int    time_adj_alm               : 1; /* [0..0] ��ʱ�����澯�Ĵ�����ͨ������sfrm_len_set�Ĵ�����������ʱʱ�������ǰ��֡�ڼ�����ֵ��������ֵ���˴�����ʧЧ���ø�λΪ1'b1�����λд��1'b1������澯����ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_TIME_ADJ_ALM_T;    /* ��ʱ�����澯 */

typedef union
{
    struct
    {
        unsigned int    dbg_time_sel               : 1; /* [0..0]  */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_DBG_TIME_SEL_T;

typedef union
{
    struct
    {
        unsigned int    clk_msr_prd                : 16; /* [15..0] ʱ�Ӳ������ڣ�32.768KHzʱ�Ӹ���������0���൱��xxms��Ĭ��ֵΪ2048�����Ĵ�������Ϊ0����������ʱ�Ӳ������߼����Ὺ��������������ʱ�̣���ʼʱ�Ӳ���ǰ������Чʱ�̣����� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_CLK_MSR_PRD_T;    /* ʱ�Ӳ������� */

typedef union
{
    struct
    {
        unsigned int    clk_state_clear            : 1; /* [0..0] ʱ�Ӳ���״̬�������üĴ���д��1'b1���ʱ�Ӳ���״̬������ʱ�̣�������Чʱ�̣����� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_CLK_STATE_CLEAR_T;    /* ʱ�Ӳ���״̬��� */

typedef union
{
    struct
    {
        unsigned int    clk_msr_start              : 1; /* [0..0] ʱ�Ӳ��������źţ�1'b1������ʱ�Ӳ���������ʱ�̣�������Чʱ�̣����� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_CLK_MSR_START_T;    /* ʱ�Ӳ���ʹ�� */

typedef union
{
    struct
    {
        unsigned int    layout_time                : 32; /* [31..0] ����滮ʱ��Ĵ�������ʱ�̣���������滮ʹ��ǰ��Чʱ�̣����� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_LAYOUT_TIME_T;    /* ����滮ʱ��Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    layout_chip                : 15; /* [14..0] ����滮ʱ����ƼĴ�������λΪchip�����üĴ�����ֵ����LAYOUT_TIMER�ĵݼ���������ʱ�̣���������滮ʹ��ǰ��Чʱ�̣����� */
        unsigned int    reserved                   : 17; /* [31..15] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_LAYOUT_CHIP_T;    /* ����滮ʱ����ƼĴ��� */

typedef union
{
    struct
    {
        unsigned int    layout_start               : 1; /* [0..0] ����滮�����Ĵ�����1'b1����������滮����ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_LAYOUT_START_T;    /* ����滮���ƼĴ��� */

typedef union
{
    struct
    {
        unsigned int    hifi_sfrm_int_msk          : 1; /* [0..0] hifi��֡�ж����Σ�1'b1ʱ��STUģ�������֡�жϣ����ϱ�����ʱ�̣�����ʱ����Чʱ�̣�������Ч */
        unsigned int    arm_sfrm_int_msk           : 1; /* [1..1] arm��֡�ж����Σ�1'b1ʱ��STUģ�������֡�жϣ����ϱ�����ʱ�̣�����ʱ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_PUB_INT_MSK_T;    /* ��֡�ж����μĴ��� */

typedef union
{
    struct
    {
        unsigned int    slot_int_msk               : 23; /* [22..0] DSPʱ϶�ж����Σ�1'b1ʱ����ʱ϶�жϣ�bit22:bit21���ֱ�Ϊ����ʱ϶��2/3��1/3�ж�bit20:bit0����ͨʱ϶0~6���ж����Σ�����ÿ����3-bitΪһ�飬��7�飬bitλ�ɵ͵��߷ֱ��ʾ����ʱ϶��1/4��1/2��3/�ж�����λ������ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    dsp_sfrm_int_msk           : 1; /* [23..23] DSP��֡�ж����Σ�1'b1ʱ��STUģ�������֡�жϣ����ϱ�����ʱ�̣�����ʱ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 8; /* [31..24] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_DSP_INT_MSK_T;    /* DSPʱ϶�ж����μĴ��� */

typedef union
{
    struct
    {
        unsigned int    hifi_sfrm_int_clear        : 1; /* [0..0] hifi��֡�ж�����źţ�1'b1�������֡�жϣ�����ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    arm_sfrm_int_clear         : 1; /* [1..1] arm��֡�ж�����źţ�1'b1�������֡�жϣ�����ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_PUB_INT_CLEAR_T;    /* ��֡�ж�����Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    slot_int_clear             : 23; /* [22..0] DSPʱ϶�ж������1'b1ʱ�����ʱ϶�жϣ�bit22:bit21���ֱ�Ϊ����ʱ϶��2/3��1/3�ж����bit20:bit0����ͨʱ϶0~6���ж����������ÿ����3-bitΪһ�飬��7�飬bitλ�ɵ͵��߷ֱ��ʾ����ʱ϶��1/4��1/2��3/�ж����λ������ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    dsp_sfrm_int_clear         : 1; /* [23..23] DSP��֡�ж�����źţ�1'b1�������֡�жϣ�����ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    reserved                   : 8; /* [31..24] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_DSP_INT_CLEAR_T;    /* DSPʱ϶�ж�����Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    hifi_sfrm_int_alm          : 1; /* [0..0] hifi��֡�жϸ澯�Ĵ����������жϵ�����ԭ�����жϻ�δ�����ʱ�������澯�źţ��ø�λΪ1'b1�����λд��1'b1������澯����ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    arm_sfrm_int_alm           : 1; /* [1..1] arm��֡�жϸ澯�Ĵ����������жϵ�����ԭ�����жϻ�δ�����ʱ�������澯�źţ��ø�λΪ1'b1�����λд��1'b1������澯����ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_PUB_INT_ALM_T;    /* ��֡�жϸ澯�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    slot_int_alm               : 23; /* [22..0] ʱ϶�жϸ澯�Ĵ����������жϵ�����֮ǰ���жϻ�δ�����ʱ�������澯���ø�λΪ1'b1�����λд��1'b1������澯λ��bit22:bit21���ֱ�Ϊ����ʱ϶��2/3��1/3�и澯bit20:bit0����ͨʱ϶0~6���жϸ澯������ÿ����3-bitΪһ�飬��7�飬bitλ�ɵ͵��߷ֱ��ʾ����ʱ϶��1/4��1/2��3/�жϸ澯λ������ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    dsp_sfrm_int_alm           : 1; /* [23..23] DSP��֡�жϸ澯�Ĵ����������жϵ�����ԭ�����жϻ�δ�����ʱ�������澯�źţ��ø�λΪ1'b1�����λд��1'b1������澯����ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    reserved                   : 8; /* [31..24] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_DSP_INT_ALM_T;    /* DSPʱ϶�жϸ澯�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    stu_cfg1                   : 32; /* [31..0] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_STU_CFG1_T;    /* �����Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    stu_cfg2                   : 32; /* [31..0] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_STU_CFG2_T;    /* �����Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    stu_cfg3                   : 32; /* [31..0] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_STU_CFG3_T;    /* �����Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    stu_cfg4                   : 32; /* [31..0] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_STU_CFG4_T;    /* �����Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    tds_stu_framc              : 20; /* [19..0] TDS 5ms��֡�ڼ�������1/96chip���� */
        unsigned int    reserved                   : 12; /* [31..20] reserved */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_TDS_STU_FRAMC_T;    /* ��֡�ڼ����� */

typedef union
{
    struct
    {
        unsigned int    tds_stu_sfn                : 16; /* [15..0] stuά������֡�� */
        unsigned int    reserved                   : 16; /* [31..16] reserved */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_TDS_STU_SFN_T;    /* ��֡�� */

typedef union
{
    struct
    {
        unsigned int    tds_stu_framl              : 20; /* [19..0] TDS STU��ǰ����֡���ȣ�1/96 chip���� */
        unsigned int    reserved                   : 12; /* [31..20] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_TDS_STU_FRAML_T;    /* ��ǰ��֡���� */

typedef union
{
    struct
    {
        unsigned int    tds_stu_slot               : 3; /* [2..0] STU��ǰ��ʱ϶�ţ�4'h7��ʱ϶6,4'h6��ʱ϶5,4'h5��ʱ϶4,4'h4��ʱ϶3,4'h3��ʱ϶2,4'h2��ʱ϶1,4'h1������ʱ϶,4'h0��ʱ϶0 */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_TDS_STU_SLOT_T;    /* ��ǰʱ϶�� */

typedef union
{
    struct
    {
        unsigned int    clk_msr_cnt                : 32; /* [31..0] ��ʱ�Ӳ���������,ϵͳʱ�ӵļ���������Ϊ�˱�֤�����ֶ�д��ͻ��DSP�ڶ�ȡ����Ĵ���ʱ����������ȡ���Σ������ν��һ������Ϊ��ȡ��ȷ���������¶�ȡ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_CLK_MSR_CNT_T;    /* ʱ�Ӳ������ */

typedef union
{
    struct
    {
        unsigned int    clk_msr_state              : 1; /* [0..0] ʱ�Ӳ���״ָ̬ʾ��1'b1������������ã�1'b0��������������á�ʱ�Ӳ��������У�������������� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_CLK_MSR_STATE_T;    /* ʱ�Ӳ���״̬ */

typedef union
{
    struct
    {
        unsigned int    layout_cur_time            : 32; /* [31..0] ��ǰ����滮ʣ��ʱ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_LAYOUT_CUR_TIME_T;    /* ��ǰLAYOUTʣ��ʱ�� */

typedef union
{
    struct
    {
        unsigned int    layout_state               : 1; /* [0..0] ��ǰ����滮״ָ̬ʾ��1'b1����ʾ����滮���ڽ���1'b0����ʾ����滮����� */
        unsigned int    reserved                   : 31; /* [31..1]  */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_LAYOUT_STATE_T;    /* ����滮״̬�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    hifi_sfrm_int_state        : 1; /* [0..0] hifi��֡�ж�״̬�Ĵ��������ж�����Ĵ���д��1'b1������ж�״̬ */
        unsigned int    arm_sfrm_int_state         : 1; /* [1..1] arm��֡�ж�״̬�Ĵ��������ж�����Ĵ���д��1'b1������ж�״̬ */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_PUB_INT_STATE_T;    /* ��֡�ж�״̬�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    slot_int_state             : 23; /* [22..0] ʱ϶�ж�״̬�Ĵ��������ж�����Ĵ���д��1'b1������ж�״̬bit22:bit21���ֱ�Ϊ����ʱ϶2/3��1/3�ж�״̬λbit20:bit0����ͨʱ϶0~6���ж�״̬������ÿ����3-bitΪһ�飬��7�飬bitλ�ɵ͵��߷ֱ��ʾ����ʱ϶��1/4��1/2��3/�ж�״̬λ */
        unsigned int    dsp_sfrm_int_state         : 1; /* [23..23] DSP��֡�ж�״̬�Ĵ��������ж�����Ĵ���д��1'b1������ж�״̬ */
        unsigned int    reserved                   : 8; /* [31..24] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_DSP_INT_STATE_T;    /* DSPʱ϶�ж�״̬�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_sfn_pre_stu            : 16; /* [15..0] ����˯��ʱ�����stu����֡�� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_DBG_SNF_PRE_STU_T;    /* ����˯��ǰ�����stu����֡�żĴ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_sfn_post_stu           : 16; /* [15..0] �˳�˯��ʱ�����stu����֡�� */
        unsigned int    dbg_sfn_post_drx           : 16; /* [31..16] �˳�˯��ʱ�����drx����֡�� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_DBG_SFN_POST_T;    /* �˳�˯�ߺ������drx��stu��֡�żĴ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_framc_pre_stu          : 20; /* [19..0] ����˯��ʱ�����stu����֡�ڼ�����ֵ��1/96 chip���� */
        unsigned int    reserved                   : 12; /* [31..20] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_DBG_FRAMC_PRE_STU_T;    /* ����˯��ǰ�����stu��֡�ڼ������Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_framc_post_drx         : 20; /* [19..0] �˳�˯��ʱ�����drx����֡�ڼ�����ֵ��1/96 chip���� */
        unsigned int    reserved                   : 12; /* [31..20] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_DBG_FRAMC_POST_DRX_T;    /* �˳�˯�ߺ������drx��֡�ڼ������Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dbg_framc_post_stu         : 20; /* [19..0] �˳�˯��ʱ�����stu����֡�ڼ�����ֵ��1/96 chip���� */
        unsigned int    reserved                   : 12; /* [31..20] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TSTU_DBG_FRAMC_POST_STU_T;    /* �˳�˯�ߺ������stu��֡�ڼ������Ĵ��� */


/********************************************************************************/
/*    bbp_tstu ��������Ŀ��_ģ����_�Ĵ�����_��Ա��_set)        */
/********************************************************************************/
HI_SET_GET(hi_bbp_tstu_soft_rst_soft_rst,soft_rst,HI_BBP_TSTU_SOFT_RST_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_SOFT_RST_OFFSET)
HI_SET_GET(hi_bbp_tstu_soft_rst_reserved,reserved,HI_BBP_TSTU_SOFT_RST_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_SOFT_RST_OFFSET)
HI_SET_GET(hi_bbp_tstu_sfn_sfn,sfn,HI_BBP_TSTU_SFN_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_SFN_OFFSET)
HI_SET_GET(hi_bbp_tstu_sfn_reserved,reserved,HI_BBP_TSTU_SFN_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_SFN_OFFSET)
HI_SET_GET(hi_bbp_tstu_sfn_oft_sfn_oft,sfn_oft,HI_BBP_TSTU_SFN_OFT_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_SFN_OFT_OFFSET)
HI_SET_GET(hi_bbp_tstu_sfn_oft_reserved,reserved,HI_BBP_TSTU_SFN_OFT_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_SFN_OFT_OFFSET)
HI_SET_GET(hi_bbp_tstu_sfrm_framl_sfrm_framl,sfrm_framl,HI_BBP_TSTU_SFRM_FRAML_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_SFRM_FRAML_OFFSET)
HI_SET_GET(hi_bbp_tstu_sfrm_framl_reserved,reserved,HI_BBP_TSTU_SFRM_FRAML_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_SFRM_FRAML_OFFSET)
HI_SET_GET(hi_bbp_tstu_sfrm_framl_set_sfrm_framl_set,sfrm_framl_set,HI_BBP_TSTU_SFRM_FRAML_SET_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_SFRM_FRAML_SET_OFFSET)
HI_SET_GET(hi_bbp_tstu_sfrm_framl_set_reserved,reserved,HI_BBP_TSTU_SFRM_FRAML_SET_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_SFRM_FRAML_SET_OFFSET)
HI_SET_GET(hi_bbp_tstu_slot_int_pos_spec_slot_int_pos,spec_slot_int_pos,HI_BBP_TSTU_SLOT_INT_POS_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_SLOT_INT_POS_OFFSET)
HI_SET_GET(hi_bbp_tstu_slot_int_pos_norm_slot_int_pos,norm_slot_int_pos,HI_BBP_TSTU_SLOT_INT_POS_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_SLOT_INT_POS_OFFSET)
HI_SET_GET(hi_bbp_tstu_time_adj_start_time_track_start,time_track_start,HI_BBP_TSTU_TIME_ADJ_START_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_TIME_ADJ_START_OFFSET)
HI_SET_GET(hi_bbp_tstu_time_adj_start_time_switch_start,time_switch_start,HI_BBP_TSTU_TIME_ADJ_START_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_TIME_ADJ_START_OFFSET)
HI_SET_GET(hi_bbp_tstu_time_adj_start_reserved,reserved,HI_BBP_TSTU_TIME_ADJ_START_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_TIME_ADJ_START_OFFSET)
HI_SET_GET(hi_bbp_tstu_time_adj_alm_time_adj_alm,time_adj_alm,HI_BBP_TSTU_TIME_ADJ_ALM_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_TIME_ADJ_ALM_OFFSET)
HI_SET_GET(hi_bbp_tstu_time_adj_alm_reserved,reserved,HI_BBP_TSTU_TIME_ADJ_ALM_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_TIME_ADJ_ALM_OFFSET)
HI_SET_GET(hi_bbp_tstu_dbg_time_sel_dbg_time_sel,dbg_time_sel,HI_BBP_TSTU_DBG_TIME_SEL_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DBG_TIME_SEL_OFFSET)
HI_SET_GET(hi_bbp_tstu_dbg_time_sel_reserved,reserved,HI_BBP_TSTU_DBG_TIME_SEL_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DBG_TIME_SEL_OFFSET)
HI_SET_GET(hi_bbp_tstu_clk_msr_prd_clk_msr_prd,clk_msr_prd,HI_BBP_TSTU_CLK_MSR_PRD_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_CLK_MSR_PRD_OFFSET)
HI_SET_GET(hi_bbp_tstu_clk_msr_prd_reserved,reserved,HI_BBP_TSTU_CLK_MSR_PRD_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_CLK_MSR_PRD_OFFSET)
HI_SET_GET(hi_bbp_tstu_clk_state_clear_clk_state_clear,clk_state_clear,HI_BBP_TSTU_CLK_STATE_CLEAR_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_CLK_STATE_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_tstu_clk_state_clear_reserved,reserved,HI_BBP_TSTU_CLK_STATE_CLEAR_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_CLK_STATE_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_tstu_clk_msr_start_clk_msr_start,clk_msr_start,HI_BBP_TSTU_CLK_MSR_START_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_CLK_MSR_START_OFFSET)
HI_SET_GET(hi_bbp_tstu_clk_msr_start_reserved,reserved,HI_BBP_TSTU_CLK_MSR_START_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_CLK_MSR_START_OFFSET)
HI_SET_GET(hi_bbp_tstu_layout_time_layout_time,layout_time,HI_BBP_TSTU_LAYOUT_TIME_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_LAYOUT_TIME_OFFSET)
HI_SET_GET(hi_bbp_tstu_layout_chip_layout_chip,layout_chip,HI_BBP_TSTU_LAYOUT_CHIP_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_LAYOUT_CHIP_OFFSET)
HI_SET_GET(hi_bbp_tstu_layout_chip_reserved,reserved,HI_BBP_TSTU_LAYOUT_CHIP_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_LAYOUT_CHIP_OFFSET)
HI_SET_GET(hi_bbp_tstu_layout_start_layout_start,layout_start,HI_BBP_TSTU_LAYOUT_START_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_LAYOUT_START_OFFSET)
HI_SET_GET(hi_bbp_tstu_layout_start_reserved,reserved,HI_BBP_TSTU_LAYOUT_START_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_LAYOUT_START_OFFSET)
HI_SET_GET(hi_bbp_tstu_pub_int_msk_hifi_sfrm_int_msk,hifi_sfrm_int_msk,HI_BBP_TSTU_PUB_INT_MSK_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_PUB_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_tstu_pub_int_msk_arm_sfrm_int_msk,arm_sfrm_int_msk,HI_BBP_TSTU_PUB_INT_MSK_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_PUB_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_tstu_pub_int_msk_reserved,reserved,HI_BBP_TSTU_PUB_INT_MSK_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_PUB_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_tstu_dsp_int_msk_slot_int_msk,slot_int_msk,HI_BBP_TSTU_DSP_INT_MSK_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DSP_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_tstu_dsp_int_msk_dsp_sfrm_int_msk,dsp_sfrm_int_msk,HI_BBP_TSTU_DSP_INT_MSK_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DSP_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_tstu_dsp_int_msk_reserved,reserved,HI_BBP_TSTU_DSP_INT_MSK_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DSP_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_tstu_pub_int_clear_hifi_sfrm_int_clear,hifi_sfrm_int_clear,HI_BBP_TSTU_PUB_INT_CLEAR_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_PUB_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_tstu_pub_int_clear_arm_sfrm_int_clear,arm_sfrm_int_clear,HI_BBP_TSTU_PUB_INT_CLEAR_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_PUB_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_tstu_pub_int_clear_reserved,reserved,HI_BBP_TSTU_PUB_INT_CLEAR_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_PUB_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_tstu_dsp_int_clear_slot_int_clear,slot_int_clear,HI_BBP_TSTU_DSP_INT_CLEAR_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DSP_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_tstu_dsp_int_clear_dsp_sfrm_int_clear,dsp_sfrm_int_clear,HI_BBP_TSTU_DSP_INT_CLEAR_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DSP_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_tstu_dsp_int_clear_reserved,reserved,HI_BBP_TSTU_DSP_INT_CLEAR_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DSP_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_tstu_pub_int_alm_hifi_sfrm_int_alm,hifi_sfrm_int_alm,HI_BBP_TSTU_PUB_INT_ALM_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_PUB_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_tstu_pub_int_alm_arm_sfrm_int_alm,arm_sfrm_int_alm,HI_BBP_TSTU_PUB_INT_ALM_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_PUB_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_tstu_pub_int_alm_reserved,reserved,HI_BBP_TSTU_PUB_INT_ALM_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_PUB_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_tstu_dsp_int_alm_slot_int_alm,slot_int_alm,HI_BBP_TSTU_DSP_INT_ALM_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DSP_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_tstu_dsp_int_alm_dsp_sfrm_int_alm,dsp_sfrm_int_alm,HI_BBP_TSTU_DSP_INT_ALM_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DSP_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_tstu_dsp_int_alm_reserved,reserved,HI_BBP_TSTU_DSP_INT_ALM_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DSP_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_tstu_stu_cfg1_stu_cfg1,stu_cfg1,HI_BBP_TSTU_STU_CFG1_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_STU_CFG1_OFFSET)
HI_SET_GET(hi_bbp_tstu_stu_cfg2_stu_cfg2,stu_cfg2,HI_BBP_TSTU_STU_CFG2_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_STU_CFG2_OFFSET)
HI_SET_GET(hi_bbp_tstu_stu_cfg3_stu_cfg3,stu_cfg3,HI_BBP_TSTU_STU_CFG3_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_STU_CFG3_OFFSET)
HI_SET_GET(hi_bbp_tstu_stu_cfg4_stu_cfg4,stu_cfg4,HI_BBP_TSTU_STU_CFG4_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_STU_CFG4_OFFSET)
HI_SET_GET(hi_bbp_tstu_tds_stu_framc_tds_stu_framc,tds_stu_framc,HI_BBP_TSTU_TDS_STU_FRAMC_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_TDS_STU_FRAMC_OFFSET)
HI_SET_GET(hi_bbp_tstu_tds_stu_framc_reserved,reserved,HI_BBP_TSTU_TDS_STU_FRAMC_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_TDS_STU_FRAMC_OFFSET)
HI_SET_GET(hi_bbp_tstu_tds_stu_sfn_tds_stu_sfn,tds_stu_sfn,HI_BBP_TSTU_TDS_STU_SFN_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_TDS_STU_SFN_OFFSET)
HI_SET_GET(hi_bbp_tstu_tds_stu_sfn_reserved,reserved,HI_BBP_TSTU_TDS_STU_SFN_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_TDS_STU_SFN_OFFSET)
HI_SET_GET(hi_bbp_tstu_tds_stu_framl_tds_stu_framl,tds_stu_framl,HI_BBP_TSTU_TDS_STU_FRAML_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_TDS_STU_FRAML_OFFSET)
HI_SET_GET(hi_bbp_tstu_tds_stu_framl_reserved,reserved,HI_BBP_TSTU_TDS_STU_FRAML_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_TDS_STU_FRAML_OFFSET)
HI_SET_GET(hi_bbp_tstu_tds_stu_slot_tds_stu_slot,tds_stu_slot,HI_BBP_TSTU_TDS_STU_SLOT_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_TDS_STU_SLOT_OFFSET)
HI_SET_GET(hi_bbp_tstu_tds_stu_slot_reserved,reserved,HI_BBP_TSTU_TDS_STU_SLOT_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_TDS_STU_SLOT_OFFSET)
HI_SET_GET(hi_bbp_tstu_clk_msr_cnt_clk_msr_cnt,clk_msr_cnt,HI_BBP_TSTU_CLK_MSR_CNT_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_CLK_MSR_CNT_OFFSET)
HI_SET_GET(hi_bbp_tstu_clk_msr_state_clk_msr_state,clk_msr_state,HI_BBP_TSTU_CLK_MSR_STATE_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_CLK_MSR_STATE_OFFSET)
HI_SET_GET(hi_bbp_tstu_clk_msr_state_reserved,reserved,HI_BBP_TSTU_CLK_MSR_STATE_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_CLK_MSR_STATE_OFFSET)
HI_SET_GET(hi_bbp_tstu_layout_cur_time_layout_cur_time,layout_cur_time,HI_BBP_TSTU_LAYOUT_CUR_TIME_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_LAYOUT_CUR_TIME_OFFSET)
HI_SET_GET(hi_bbp_tstu_layout_state_layout_state,layout_state,HI_BBP_TSTU_LAYOUT_STATE_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_LAYOUT_STATE_OFFSET)
HI_SET_GET(hi_bbp_tstu_layout_state_reserved,reserved,HI_BBP_TSTU_LAYOUT_STATE_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_LAYOUT_STATE_OFFSET)
HI_SET_GET(hi_bbp_tstu_pub_int_state_hifi_sfrm_int_state,hifi_sfrm_int_state,HI_BBP_TSTU_PUB_INT_STATE_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_PUB_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_tstu_pub_int_state_arm_sfrm_int_state,arm_sfrm_int_state,HI_BBP_TSTU_PUB_INT_STATE_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_PUB_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_tstu_pub_int_state_reserved,reserved,HI_BBP_TSTU_PUB_INT_STATE_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_PUB_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_tstu_dsp_int_state_slot_int_state,slot_int_state,HI_BBP_TSTU_DSP_INT_STATE_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DSP_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_tstu_dsp_int_state_dsp_sfrm_int_state,dsp_sfrm_int_state,HI_BBP_TSTU_DSP_INT_STATE_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DSP_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_tstu_dsp_int_state_reserved,reserved,HI_BBP_TSTU_DSP_INT_STATE_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DSP_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_tstu_dbg_snf_pre_stu_dbg_sfn_pre_stu,dbg_sfn_pre_stu,HI_BBP_TSTU_DBG_SNF_PRE_STU_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DBG_SNF_PRE_STU_OFFSET)
HI_SET_GET(hi_bbp_tstu_dbg_snf_pre_stu_reserved,reserved,HI_BBP_TSTU_DBG_SNF_PRE_STU_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DBG_SNF_PRE_STU_OFFSET)
HI_SET_GET(hi_bbp_tstu_dbg_sfn_post_dbg_sfn_post_stu,dbg_sfn_post_stu,HI_BBP_TSTU_DBG_SFN_POST_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DBG_SFN_POST_OFFSET)
HI_SET_GET(hi_bbp_tstu_dbg_sfn_post_dbg_sfn_post_drx,dbg_sfn_post_drx,HI_BBP_TSTU_DBG_SFN_POST_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DBG_SFN_POST_OFFSET)
HI_SET_GET(hi_bbp_tstu_dbg_framc_pre_stu_dbg_framc_pre_stu,dbg_framc_pre_stu,HI_BBP_TSTU_DBG_FRAMC_PRE_STU_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DBG_FRAMC_PRE_STU_OFFSET)
HI_SET_GET(hi_bbp_tstu_dbg_framc_pre_stu_reserved,reserved,HI_BBP_TSTU_DBG_FRAMC_PRE_STU_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DBG_FRAMC_PRE_STU_OFFSET)
HI_SET_GET(hi_bbp_tstu_dbg_framc_post_drx_dbg_framc_post_drx,dbg_framc_post_drx,HI_BBP_TSTU_DBG_FRAMC_POST_DRX_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DBG_FRAMC_POST_DRX_OFFSET)
HI_SET_GET(hi_bbp_tstu_dbg_framc_post_drx_reserved,reserved,HI_BBP_TSTU_DBG_FRAMC_POST_DRX_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DBG_FRAMC_POST_DRX_OFFSET)
HI_SET_GET(hi_bbp_tstu_dbg_framc_post_stu_dbg_framc_post_stu,dbg_framc_post_stu,HI_BBP_TSTU_DBG_FRAMC_POST_STU_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DBG_FRAMC_POST_STU_OFFSET)
HI_SET_GET(hi_bbp_tstu_dbg_framc_post_stu_reserved,reserved,HI_BBP_TSTU_DBG_FRAMC_POST_STU_T,HI_BBP_TSTU_BASE_ADDR, HI_BBP_TSTU_DBG_FRAMC_POST_STU_OFFSET)

#endif // __HI_BBP_TSTU_H__

