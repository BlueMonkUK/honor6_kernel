/******************************************************************************/
/*  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_BBP_TDSDRX.h */
/* Version       : 2.0 */
/* Created       : 2013-06-29*/
/* Last Modified : */
/* Description   :  The C union definition file for the module BBP_TDSDRX*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_BBP_TDSDRX_H__
#define __HI_BBP_TDSDRX_H__

/*
 * Project: hi
 * Module : BBP_TDSDRX
 */

#ifndef HI_SET_GET
#define HI_SET_GET(a0,a1,a2,a3,a4)
#endif

/********************************************************************************/
/*    BBP_TDSDRX �Ĵ���ƫ�ƶ��壨��Ŀ��_ģ����_�Ĵ�����_OFFSET)        */
/********************************************************************************/
#define    HI_BBP_TDSDRX_CLK_MSR_FRAC_OFFSET                 (0x10) /* ʱ�Ӳ���С������ */
#define    HI_BBP_TDSDRX_CLK_MSR_INTE_OFFSET                 (0x14) /* ʱ�Ӳ����������� */
#define    HI_BBP_TDSDRX_SFRM_FRAML_OFFSET                   (0x18) /* TDS��֡���ȼĴ��� */
#define    HI_BBP_TDSDRX_WAKEUP_32K_CNT_OFFSET               (0x1C) /* ˯�߻���ʱ�� */
#define    HI_BBP_TDSDRX_SWITCH_32K_CNT_OFFSET               (0x20) /* ʱ���л�ʱ�� */
#define    HI_BBP_TDSDRX_SLEEP_MODE_OFFSET                   (0x24) /* ˯��ʱ�̿��ƼĴ��� */
#define    HI_BBP_TDSDRX_SLEEP_START_OFFSET                  (0x28) /* ˯��ʹ�ܱ�־ */
#define    HI_BBP_TDSDRX_SWITCH_FORCE_START_OFFSET           (0x2C) /* ǿ�ƻ���ʹ�� */
#define    HI_BBP_TDSDRX_ARM_SLEEP_INT_MSK_OFFSET            (0x40) /* �ж����� */
#define    HI_BBP_TDSDRX_ARM_SLEEP_INT_CLEAR_OFFSET          (0x44) /* �ж���� */
#define    HI_BBP_TDSDRX_ARM_SLEEP_INT_ALM_OFFSET            (0x48) /* �жϸ澯 */
#define    HI_BBP_TDSDRX_DRX_CFG1_OFFSET                     (0x100) /* ECO�Ĵ��� */
#define    HI_BBP_TDSDRX_DRX_CFG2_OFFSET                     (0x104) /* ECO�Ĵ��� */
#define    HI_BBP_TDSDRX_DRX_CFG3_OFFSET                     (0x108) /* ECO�Ĵ��� */
#define    HI_BBP_TDSDRX_DRX_CFG4_OFFSET                     (0x10C) /* ECO�Ĵ��� */
#define    HI_BBP_TDSDRX_CLK_SWITCH_STATE_OFFSET             (0x200) /* ʱ��״ָ̬ʾ */
#define    HI_BBP_TDSDRX_AWAKE_TYPE_OFFSET                   (0x204) /* ǿ�ƻ���״ָ̬ʾ */
#define    HI_BBP_TDSDRX_DRX_SFN_ACC_OFFSET                  (0x208) /* ˯��ʱ�����ۼӵ���֡�� */
#define    HI_BBP_TDSDRX_SLP_TIME_CUR_OFFSET                 (0x20C) /* ��ǰ˯��ʱ�� */
#define    HI_BBP_TDSDRX_ARM_INT_STATE_OFFSET                (0x220) /* �ж�״̬�ϱ� */

/********************************************************************************/
/*    BBP_TDSDRX �Ĵ������壨��Ŀ��_ģ����_�Ĵ�����_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    clk_msr_frac               : 16; /* [15..0] ʱ�Ӳ�����С�����֣�С������ΪNλ���ȣ�С�����ֳ���2^N������ʱ�̣�DSP��BBP����˯��ʹ��sleep_enǰ������Чʱ�̣����� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_CLK_MSR_FRAC_T;    /* ʱ�Ӳ���С������ */

typedef union
{
    struct
    {
        unsigned int    clk_msr_inte               : 12; /* [11..0] ʱ�Ӳ������������֡�����ʱ�̣�DSP��BBP����˯��ʹ��sleep_enǰ������Чʱ�̣����� */
        unsigned int    reserved                   : 20; /* [31..12] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_CLK_MSR_INTE_T;    /* ʱ�Ӳ����������� */

typedef union
{
    struct
    {
        unsigned int    sfrm_framl                 : 20; /* [19..0] TDS 5ms��֡���ȣ���λΪcycle��Ĭ��ֵΪ5ms��֡����ֵ��ȥ1����614399����ʱ�̣�DSP��BBP����˯������sleep_enǰ������Чʱ�̣����� */
        unsigned int    reserved                   : 12; /* [31..20] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_SFRM_FRAML_T;    /* TDS��֡���ȼĴ��� */

typedef union
{
    struct
    {
        unsigned int    wakeup_32k_cnt             : 27; /* [26..0] UE�����ý���˯�ߵ��ϱ������ж����ʱ����32.768KHzʱ�ӵļ�������������0���ʱ��Ϊ1Сʱ������ʱ�̣�DSP��BBP����˯������sleep_enǰ������Чʱ�̣����� */
        unsigned int    reserved                   : 5; /* [31..27] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_WAKEUP_32K_CNT_T;    /* ˯�߻���ʱ�� */

typedef union
{
    struct
    {
        unsigned int    switch_32k_cnt             : 12; /* [11..0] UE���ϱ������жϵ����ʱ���л����ʱ����32.768KHzʱ�ӵļ�������������0������˯ʱ����������Ϊ8ms������ʱ�̣�DSP��BBP����˯������sleep_enǰ������Чʱ�̣����� */
        unsigned int    reserved                   : 20; /* [31..12] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_SWITCH_32K_CNT_T;    /* ʱ���л�ʱ�� */

typedef union
{
    struct
    {
        unsigned int    sleep_mode                 : 1; /* [0..0] ˯��ʱ�̿��ƼĴ�����1'b1������ʱ�̽���˯�ߣ�����sleep_en���߼���������˯��1'b0����������ʱ�̣�DSP��BBP����˯������sleep_enǰ������Чʱ�̣����� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_SLEEP_MODE_T;    /* ˯��ʱ�̿��ƼĴ��� */

typedef union
{
    struct
    {
        unsigned int    sleep_start                : 1; /* [0..0] ˯��������־��1'b1������˯�ߡ�����ʱ�̣�������Чʱ�̣����� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_SLEEP_START_T;    /* ˯��ʹ�ܱ�־ */

typedef union
{
    struct
    {
        unsigned int    switch_force_start         : 1; /* [0..0] ǿ�ƻ���������1'b1������ǿ�ƻ�������ʱ�̣�������Чʱ�̣����� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_SWITCH_FORCE_START_T;    /* ǿ�ƻ���ʹ�� */

typedef union
{
    struct
    {
        unsigned int    switch_int_msk             : 1; /* [0..0] ʱ���л��ж����Σ�1'b1�����ж�1'b0�������ж�����ʱ�̣�������Чʱ�̣����� */
        unsigned int    wakeup_int_msk             : 1; /* [1..1] ˯�߻����ж����Σ�1'b1�����ж�1'b0�������ж�����ʱ�̣�������Чʱ�̣����� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_ARM_SLEEP_INT_MSK_T;    /* �ж����� */

typedef union
{
    struct
    {
        unsigned int    switch_int_clear           : 1; /* [0..0] ʱ���л��жϣ����bitλд1�����Ӧ���жϼ�״̬������ʱ�̣�������Чʱ�̣����� */
        unsigned int    wakeup_int_clear           : 1; /* [1..1] ˯�߻����жϣ����bitλд1�����Ӧ���жϼ�״̬������ʱ�̣�������Чʱ�̣����� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_ARM_SLEEP_INT_CLEAR_T;    /* �ж���� */

typedef union
{
    struct
    {
        unsigned int    switch_int_alm             : 1; /* [0..0] ʱ���л��жϸ澯�������жϴ�����ԭ�ж�δ���ʱ�����澯��д1��0�澯λ������ʱ�̣�������Чʱ�̣����� */
        unsigned int    wakeup_int_alm             : 1; /* [1..1] �����жϸ澯�������жϴ�����ԭ�ж�δ���ʱ�����澯��д1��0�澯λ������ʱ�̣�������Чʱ�̣����� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_ARM_SLEEP_INT_ALM_T;    /* �жϸ澯 */

typedef union
{
    struct
    {
        unsigned int    drx_cfg1                   : 32; /* [31..0] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_DRX_CFG1_T;    /* ECO�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    drx_cfg2                   : 32; /* [31..0] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_DRX_CFG2_T;    /* ECO�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    drx_cfg3                   : 32; /* [31..0] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_DRX_CFG3_T;    /* ECO�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    drx_cfg4                   : 32; /* [31..0] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_DRX_CFG4_T;    /* ECO�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    clk_switch_state           : 1; /* [0..0] ʱ��״̬��־��1'b1��ʾʱ���л���32.768KHz��1'b0ʱ���л���ϵͳʱ�� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_CLK_SWITCH_STATE_T;    /* ʱ��״ָ̬ʾ */

typedef union
{
    struct
    {
        unsigned int    awake_type                 : 1; /* [0..0] ����Դָʾ��1'b1����ʾ�˴�˯�ߵĻ���Դ��ǿ�ƻ���1'b0����ʾ�˴�˯������Ȼ�����߼���������һ��˯��ʱ������üĴ��� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_AWAKE_TYPE_T;    /* ǿ�ƻ���״ָ̬ʾ */

typedef union
{
    struct
    {
        unsigned int    drx_sfn_acc                : 32; /* [31..0] ˯��ʱ���ڣ�DRX�ۼӵ���֡���������� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_DRX_SFN_ACC_T;    /* ˯��ʱ�����ۼӵ���֡�� */

typedef union
{
    struct
    {
        unsigned int    slp_time_cur               : 28; /* [27..0] ��ǰ˯��ʱ���ϱ��Ĵ��� */
        unsigned int    reserved                   : 4; /* [31..28] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_SLP_TIME_CUR_T;    /* ��ǰ˯��ʱ�� */

typedef union
{
    struct
    {
        unsigned int    switch_int_state           : 1; /* [0..0] ʱ���л��ж�״ָ̬ʾ�����ж�����Ĵ���ARM_INT_CLEAR��Ӧλд1'b1������жϱ�־ */
        unsigned int    wakeup_int_state           : 1; /* [1..1] ˯�߻����ж�״ָ̬ʾ�����ж�����Ĵ���ARM_INT_CLEAR��Ӧλд1'b1������жϱ�־ */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSDRX_ARM_INT_STATE_T;    /* �ж�״̬�ϱ� */


/********************************************************************************/
/*    BBP_TDSDRX ��������Ŀ��_ģ����_�Ĵ�����_��Ա��_set)        */
/********************************************************************************/
HI_SET_GET(hi_bbp_tdsdrx_clk_msr_frac_clk_msr_frac,clk_msr_frac,HI_BBP_TDSDRX_CLK_MSR_FRAC_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_CLK_MSR_FRAC_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_clk_msr_frac_reserved,reserved,HI_BBP_TDSDRX_CLK_MSR_FRAC_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_CLK_MSR_FRAC_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_clk_msr_inte_clk_msr_inte,clk_msr_inte,HI_BBP_TDSDRX_CLK_MSR_INTE_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_CLK_MSR_INTE_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_clk_msr_inte_reserved,reserved,HI_BBP_TDSDRX_CLK_MSR_INTE_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_CLK_MSR_INTE_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_sfrm_framl_sfrm_framl,sfrm_framl,HI_BBP_TDSDRX_SFRM_FRAML_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_SFRM_FRAML_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_sfrm_framl_reserved,reserved,HI_BBP_TDSDRX_SFRM_FRAML_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_SFRM_FRAML_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_wakeup_32k_cnt_wakeup_32k_cnt,wakeup_32k_cnt,HI_BBP_TDSDRX_WAKEUP_32K_CNT_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_WAKEUP_32K_CNT_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_wakeup_32k_cnt_reserved,reserved,HI_BBP_TDSDRX_WAKEUP_32K_CNT_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_WAKEUP_32K_CNT_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_switch_32k_cnt_switch_32k_cnt,switch_32k_cnt,HI_BBP_TDSDRX_SWITCH_32K_CNT_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_SWITCH_32K_CNT_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_switch_32k_cnt_reserved,reserved,HI_BBP_TDSDRX_SWITCH_32K_CNT_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_SWITCH_32K_CNT_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_sleep_mode_sleep_mode,sleep_mode,HI_BBP_TDSDRX_SLEEP_MODE_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_SLEEP_MODE_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_sleep_mode_reserved,reserved,HI_BBP_TDSDRX_SLEEP_MODE_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_SLEEP_MODE_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_sleep_start_sleep_start,sleep_start,HI_BBP_TDSDRX_SLEEP_START_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_SLEEP_START_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_sleep_start_reserved,reserved,HI_BBP_TDSDRX_SLEEP_START_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_SLEEP_START_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_switch_force_start_switch_force_start,switch_force_start,HI_BBP_TDSDRX_SWITCH_FORCE_START_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_SWITCH_FORCE_START_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_switch_force_start_reserved,reserved,HI_BBP_TDSDRX_SWITCH_FORCE_START_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_SWITCH_FORCE_START_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_arm_sleep_int_msk_switch_int_msk,switch_int_msk,HI_BBP_TDSDRX_ARM_SLEEP_INT_MSK_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_ARM_SLEEP_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_arm_sleep_int_msk_wakeup_int_msk,wakeup_int_msk,HI_BBP_TDSDRX_ARM_SLEEP_INT_MSK_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_ARM_SLEEP_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_arm_sleep_int_msk_reserved,reserved,HI_BBP_TDSDRX_ARM_SLEEP_INT_MSK_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_ARM_SLEEP_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_arm_sleep_int_clear_switch_int_clear,switch_int_clear,HI_BBP_TDSDRX_ARM_SLEEP_INT_CLEAR_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_ARM_SLEEP_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_arm_sleep_int_clear_wakeup_int_clear,wakeup_int_clear,HI_BBP_TDSDRX_ARM_SLEEP_INT_CLEAR_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_ARM_SLEEP_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_arm_sleep_int_clear_reserved,reserved,HI_BBP_TDSDRX_ARM_SLEEP_INT_CLEAR_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_ARM_SLEEP_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_arm_sleep_int_alm_switch_int_alm,switch_int_alm,HI_BBP_TDSDRX_ARM_SLEEP_INT_ALM_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_ARM_SLEEP_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_arm_sleep_int_alm_wakeup_int_alm,wakeup_int_alm,HI_BBP_TDSDRX_ARM_SLEEP_INT_ALM_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_ARM_SLEEP_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_arm_sleep_int_alm_reserved,reserved,HI_BBP_TDSDRX_ARM_SLEEP_INT_ALM_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_ARM_SLEEP_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_drx_cfg1_drx_cfg1,drx_cfg1,HI_BBP_TDSDRX_DRX_CFG1_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_DRX_CFG1_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_drx_cfg2_drx_cfg2,drx_cfg2,HI_BBP_TDSDRX_DRX_CFG2_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_DRX_CFG2_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_drx_cfg3_drx_cfg3,drx_cfg3,HI_BBP_TDSDRX_DRX_CFG3_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_DRX_CFG3_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_drx_cfg4_drx_cfg4,drx_cfg4,HI_BBP_TDSDRX_DRX_CFG4_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_DRX_CFG4_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_clk_switch_state_clk_switch_state,clk_switch_state,HI_BBP_TDSDRX_CLK_SWITCH_STATE_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_CLK_SWITCH_STATE_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_clk_switch_state_reserved,reserved,HI_BBP_TDSDRX_CLK_SWITCH_STATE_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_CLK_SWITCH_STATE_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_awake_type_awake_type,awake_type,HI_BBP_TDSDRX_AWAKE_TYPE_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_AWAKE_TYPE_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_awake_type_reserved,reserved,HI_BBP_TDSDRX_AWAKE_TYPE_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_AWAKE_TYPE_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_drx_sfn_acc_drx_sfn_acc,drx_sfn_acc,HI_BBP_TDSDRX_DRX_SFN_ACC_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_DRX_SFN_ACC_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_slp_time_cur_slp_time_cur,slp_time_cur,HI_BBP_TDSDRX_SLP_TIME_CUR_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_SLP_TIME_CUR_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_slp_time_cur_reserved,reserved,HI_BBP_TDSDRX_SLP_TIME_CUR_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_SLP_TIME_CUR_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_arm_int_state_switch_int_state,switch_int_state,HI_BBP_TDSDRX_ARM_INT_STATE_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_ARM_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_arm_int_state_wakeup_int_state,wakeup_int_state,HI_BBP_TDSDRX_ARM_INT_STATE_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_ARM_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_tdsdrx_arm_int_state_reserved,reserved,HI_BBP_TDSDRX_ARM_INT_STATE_T,HI_BBP_TDSDRX_BASE_ADDR, HI_BBP_TDSDRX_ARM_INT_STATE_OFFSET)

#endif // __HI_BBP_TDSDRX_H__

