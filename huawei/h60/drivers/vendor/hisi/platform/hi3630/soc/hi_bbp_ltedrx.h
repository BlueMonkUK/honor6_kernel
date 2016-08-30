/******************************************************************************/
/*  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_bbp_ltedrx.h */
/* Version       : 2.0 */
/* Author        : xxx*/
/* Created       : 2013-06-06*/
/* Last Modified : */
/* Description   :  The C union definition file for the module bbp_ltedrx*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Author        : xxx*/
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_BBP_LTEDRX_H__
#define __HI_BBP_LTEDRX_H__

/*
 * Project: hi
 * Module : bbp_ltedrx
 */

#ifndef HI_SET_GET
#define HI_SET_GET(a0,a1,a2,a3,a4)
#endif

/********************************************************************************/
/*    bbp_ltedrx �Ĵ���ƫ�ƶ��壨��Ŀ��_ģ����_�Ĵ�����_OFFSET)        */
/********************************************************************************/
#define    HI_BBP_LTEDRX_CLK_MSR_FRAC_OFFSET                 (0x10) /* ʱ�Ӳ���С������ */
#define    HI_BBP_LTEDRX_CLK_MSR_INTE_OFFSET                 (0x14) /* ʱ�Ӳ����������� */
#define    HI_BBP_LTEDRX_SFRM_FRAML_OFFSET                   (0x18) /* LTE��֡���ȼĴ��� */
#define    HI_BBP_LTEDRX_WAKEUP_32K_CNT_OFFSET               (0x1C) /* ˯�߻���ʱ�� */
#define    HI_BBP_LTEDRX_SWITCH_32K_CNT_OFFSET               (0x20) /* ʱ���л�ʱ�� */
#define    HI_BBP_LTEDRX_SLEEP_MODE_OFFSET                   (0x24) /* ˯��ʱ�̿��ƼĴ��� */
#define    HI_BBP_LTEDRX_SLEEP_START_OFFSET                  (0x28) /* ˯��ʹ�ܱ�־ */
#define    HI_BBP_LTEDRX_SWITCH_FORCE_START_OFFSET           (0x2C) /* ǿ�ƻ���ʹ�� */
#define    HI_BBP_LTEDRX_ARM_SLEEP_INT_MSK_OFFSET            (0x40) /* �ж����� */
#define    HI_BBP_LTEDRX_ARM_SLEEP_INT_CLEAR_OFFSET          (0x44) /* �ж���� */
#define    HI_BBP_LTEDRX_ARM_SLEEP_INT_ALM_OFFSET            (0x48) /* �жϸ澯 */
#define    HI_BBP_LTEDRX_LTE_SYS_TIMER_OFFSET                (0x70) /* ϵͳ���¶�ʱ�� */
#define    HI_BBP_LTEDRX_LTE_3412_TIMER_OFFSET               (0x74) /* TAU���¶�ʱ�� */
#define    HI_BBP_LTEDRX_LTE_VPLMN_TIMER_OFFSET              (0x78) /* VPLMN��ʱ�� */
#define    HI_BBP_LTEDRX_LTE_TSRHC_TIMER_OFFSET              (0x7C) /* ������ʱ�� */
#define    HI_BBP_LTEDRX_LTE_TA_TIMER_OFFSET                 (0x80) /* TA������ʱ�� */
#define    HI_BBP_LTEDRX_LTE_DEEPS_TIMER_OFFSET              (0x84) /* ��˯�߶�ʱ�� */
#define    HI_BBP_LTEDRX_LTE_PS32K_TIMER_OFFSET              (0x88) /* Э���Ҷ�ʱ����ʱ������ */
#define    HI_BBP_LTEDRX_LTE_PS32KBASE_TIMER_OFFSET          (0x8C) /* Э���Ҷ�ʱ����׼������ */
#define    HI_BBP_LTEDRX_LTE_PS32K_BASE_MODE_OFFSET          (0x90) /* Э���Ҷ�ʱ����׼����������ģʽ */
#define    HI_BBP_LTEDRX_DRX_TIMER_EN_OFFSET                 (0x94) /* DRX tim��ʱ��ʹ�� */
#define    HI_BBP_LTEDRX_DRX_TIMER_10MS_OFFSET               (0x98) /* 10M��ʱ���� */
#define    HI_BBP_LTEDRX_DRX_TIMER_MIN_OFFSET                (0x9C) /* 1���Ӷ�ʱ���� */
#define    HI_BBP_LTEDRX_DRX_TIMER_HOUR_OFFSET               (0xA0) /* 1Сʱ��ʱ���� */
#define    HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_OFFSET            (0xB0) /* �ж����� */
#define    HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_OFFSET          (0xB4) /* �ж���� */
#define    HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_OFFSET            (0xB8) /* �жϸ澯 */
#define    HI_BBP_LTEDRX_CLK_SWITCH_STATE_OFFSET             (0x200) /* ʱ��״ָ̬ʾ */
#define    HI_BBP_LTEDRX_AWAKE_TYPE_OFFSET                   (0x204) /* ����״ָ̬ʾ */
#define    HI_BBP_LTEDRX_DRX_SFN_ACC_OFFSET                  (0x208) /* ˯��ʱ�����ۼӵ���֡�� */
#define    HI_BBP_LTEDRX_SLP_TIME_CUR_OFFSET                 (0x20C) /* ��ǰ˯��ʱ�� */
#define    HI_BBP_LTEDRX_ARM_INT_STATE_OFFSET                (0x220) /* �ж�״̬�ϱ� */
#define    HI_BBP_LTEDRX_LTE_SYS_CUR_TIMER_OFFSET            (0x300) /* ϵͳ���¶�ʱ�� */
#define    HI_BBP_LTEDRX_LTE_3412_CUR_TIMER_OFFSET           (0x304) /* TAU���¶�ʱ�� */
#define    HI_BBP_LTEDRX_LTE_VPLMN_CUR_TIMER_OFFSET          (0x308) /* VPLMN��ʱ�� */
#define    HI_BBP_LTEDRX_LTE_TSRHC_CUR_TIMER_OFFSET          (0x30C) /* ������ʱ�� */
#define    HI_BBP_LTEDRX_LTE_TA_CUR_TIMER_OFFSET             (0x310) /* TA������ʱ�� */
#define    HI_BBP_LTEDRX_LTE_DEEPS_CUR_TIMER_OFFSET          (0x314) /* ��˯�߶�ʱ�� */
#define    HI_BBP_LTEDRX_LTE_PS32K_CUR_TIMER_OFFSET          (0x318) /* Э���Ҷ�ʱ����ʱ������ */
#define    HI_BBP_LTEDRX_LTE_PS32KBASE_CUR_TIMER_OFFSET      (0x31C) /* Э���Ҷ�ʱ����׼������ */
#define    HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_OFFSET          (0x340) /* �ж�״̬�ϱ� */


#ifndef __ASSEMBLY__

/********************************************************************************/
/*    bbp_ltedrx �Ĵ������壨��Ŀ��_ģ����_�Ĵ�����_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    clk_msr_frac               : 16; /* [15..0] ʱ�Ӳ�����С�����֣�С������ΪNλ���ȣ�С�����ֳ���2^N������ʱ�̣�DSP��BBP����˯��ʹ��sleep_enǰ������Чʱ�̣����� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_CLK_MSR_FRAC_T;    /* ʱ�Ӳ���С������ */

typedef union
{
    struct
    {
        unsigned int    clk_msr_inte               : 12; /* [11..0] ʱ�Ӳ������������֡�����ʱ�̣�DSP��BBP����˯��ʹ��sleep_enǰ������Чʱ�̣����� */
        unsigned int    reserved                   : 20; /* [31..12] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_CLK_MSR_INTE_T;    /* ʱ�Ӳ����������� */

typedef union
{
    struct
    {
        unsigned int    sfrm_framl                 : 20; /* [19..0] LTE 1ms��֡���ȣ���λΪcycle��Ĭ��ֵΪ1ms��֡����ֵ��ȥ1����122879����ʱ�̣�DSP��BBP����˯������sleep_enǰ������Чʱ�̣����� */
        unsigned int    reserved                   : 12; /* [31..20] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_SFRM_FRAML_T;    /* LTE��֡���ȼĴ��� */

typedef union
{
    struct
    {
        unsigned int    wakeup_32k_cnt             : 27; /* [26..0] UE�����ý���˯�ߵ��ϱ������ж����ʱ����32.768KHzʱ�ӵļ�������������0���ʱ��Ϊ1Сʱ������ʱ�̣�DSP��BBP����˯������sleep_enǰ������Чʱ�̣����� */
        unsigned int    reserved                   : 5; /* [31..27] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_WAKEUP_32K_CNT_T;    /* ˯�߻���ʱ�� */

typedef union
{
    struct
    {
        unsigned int    switch_32k_cnt             : 12; /* [11..0] UE���ϱ������жϵ����ʱ���л����ʱ����32.768KHzʱ�ӵļ�������������0������˯ʱ����������Ϊ8ms������ʱ�̣�DSP��BBP����˯������sleep_enǰ������Чʱ�̣����� */
        unsigned int    reserved                   : 20; /* [31..12] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_SWITCH_32K_CNT_T;    /* ʱ���л�ʱ�� */

typedef union
{
    struct
    {
        unsigned int    sleep_mode                 : 1; /* [0..0] ˯��ʱ�̿��ƼĴ�����1'b1������ʱ�̽���˯�ߣ�����sleep_start���߼���������˯��1'b0����������ʱ�̣�DSP��BBP����˯������sleep_startǰ������Чʱ�̣����� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_SLEEP_MODE_T;    /* ˯��ʱ�̿��ƼĴ��� */

typedef union
{
    struct
    {
        unsigned int    sleep_start                : 1; /* [0..0] ˯��������־��1'b1������˯�ߡ�����ʱ�̣�������Чʱ�̣����� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_SLEEP_START_T;    /* ˯��ʹ�ܱ�־ */

typedef union
{
    struct
    {
        unsigned int    switch_force_start         : 1; /* [0..0] ǿ�ƻ���������1'b1������ǿ�ƻ�������ʱ�̣�������Чʱ�̣����� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_SWITCH_FORCE_START_T;    /* ǿ�ƻ���ʹ�� */

typedef union
{
    struct
    {
        unsigned int    switch_int_msk             : 1; /* [0..0] ʱ���л��ж����Σ�1'b1�����ж�1'b0�������ж�����ʱ�̣�������Чʱ�̣����� */
        unsigned int    wakeup_int_msk             : 1; /* [1..1] ˯�߻����ж����Σ�1'b1�����ж�1'b0�������ж�����ʱ�̣�������Чʱ�̣����� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_ARM_SLEEP_INT_MSK_T;    /* �ж����� */

typedef union
{
    struct
    {
        unsigned int    switch_int_clear           : 1; /* [0..0] ʱ���л��жϣ����bitλд1�����Ӧ���жϼ�״̬������ʱ�̣�������Чʱ�̣����� */
        unsigned int    wakeup_int_clear           : 1; /* [1..1] ˯�߻����жϣ����bitλд1�����Ӧ���жϼ�״̬������ʱ�̣�������Чʱ�̣����� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_ARM_SLEEP_INT_CLEAR_T;    /* �ж���� */

typedef union
{
    struct
    {
        unsigned int    switch_int_alm             : 1; /* [0..0] ʱ���л��жϸ澯�������жϴ�����ԭ�ж�δ���ʱ�����澯��д1��0�澯λ������ʱ�̣�������Чʱ�̣����� */
        unsigned int    wakeup_int_alm             : 1; /* [1..1] �����жϸ澯�������жϴ�����ԭ�ж�δ���ʱ�����澯��д1��0�澯λ������ʱ�̣�������Чʱ�̣����� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_ARM_SLEEP_INT_ALM_T;    /* �жϸ澯 */

typedef union
{
    struct
    {
        unsigned int    lte_sys_timer              : 3; /* [2..0] ϵͳ���¶�ʱ������ʱ���� 3 hour����BBP����˯��ʱ���������������ȵ�BBP�����Ѻ����ϱ���ʱ�жϡ�����ʱ�̣������ö�ʱ��֮ǰ��Чʱ�̣����� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_LTE_SYS_TIMER_T;    /* ϵͳ���¶�ʱ�� */

typedef union
{
    struct
    {
        unsigned int    lte_3412_timer             : 8; /* [7..0] TAU���¶�ʱ������ʱ���� 0~186 min����BBP����˯��ʱ���������������ȵ�BBP�����Ѻ����ϱ���ʱ�жϡ�����ʱ�̣������ö�ʱ��֮ǰ��Чʱ�̣����� */
        unsigned int    reserved                   : 24; /* [31..8] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_LTE_3412_TIMER_T;    /* TAU���¶�ʱ�� */

typedef union
{
    struct
    {
        unsigned int    lte_vplmn_timer            : 3; /* [2..0] VPLMN״̬�����������ȼ�PLMN��ʱ������ʱ���� 2 min����BBP����˯��ʱ���������������ȵ�BBP�����Ѻ����ϱ���ʱ�жϡ�����ʱ�̣������ö�ʱ��֮ǰ��Чʱ�̣����� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_LTE_VPLMN_TIMER_T;    /* VPLMN��ʱ�� */

typedef union
{
    struct
    {
        unsigned int    lte_tsrhc_timer            : 3; /* [2..0] ��������������ʱ������ʱ���� 3 min����BBP����˯��ʱ���������������ȵ�BBP�����Ѻ����ϱ���ʱ�жϡ�����ʱ�̣������ö�ʱ��֮ǰ��Чʱ�̣����� */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_LTE_TSRHC_TIMER_T;    /* ������ʱ�� */

typedef union
{
    struct
    {
        unsigned int    lte_ta_timer               : 11; /* [10..0] TA������ʱ������ʱ����ȡֵΪ�� 500ms��750ms��1280ms��1920ms��2560ms��5120ms��10240ms��infinity����BBP����˯��ʱ���������������ȵ�BBP�����Ѻ����ϱ���ʱ�жϡ�����ʱ�̣������ö�ʱ��֮ǰ��Чʱ�̣����� */
        unsigned int    reserved                   : 21; /* [31..11] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_LTE_TA_TIMER_T;    /* TA������ʱ�� */

typedef union
{
    struct
    {
        unsigned int    lte_deeps_timer            : 7; /* [6..0] ��˯�߶�ʱ������ʱ���ڵ�λΪmin������ʱ�̣������ö�ʱ��֮ǰ��Чʱ�̣����� */
        unsigned int    reserved                   : 25; /* [31..7] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_LTE_DEEPS_TIMER_T;    /* ��˯�߶�ʱ�� */

typedef union
{
    struct
    {
        unsigned int    lte_ps32k_timer            : 32; /* [31..0] Э��ջ��ʱ����ʱ������������32.768KHz˯��ʱ�Ӽ�������λΪһ��32KHzʱ�����ڣ���������DRX˯������Ȼ��Ҫ���е������ʱ������ʱ����������������ã�������ʱ����������жϲ��ϱ���ARM����BBP����˯��ʱ���������������ȵ�BBP�����Ѻ����ϱ���ʱ�жϡ�����ʱ�̣������ö�ʱ��֮ǰ��Чʱ�̣����� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_LTE_PS32K_TIMER_T;    /* Э���Ҷ�ʱ����ʱ������ */

typedef union
{
    struct
    {
        unsigned int    lte_ps32kbase_timer        : 32; /* [31..0] Э��ջ��ʱ����׼�����������Բ���Э��ջ��ʱ����˯���ڼ�ļ���ֵ������32.768KHz˯��ʱ�Ӽ�������λΪһ��32KHzʱ�����ڣ��ü�����֧������ģʽ����1���û�����ģʽ����ʱ������������ã�������ֵ������ʱ����������жϲ��ϱ���ARM����BBP����˯��ʱ���������������ȵ�BBP�����Ѻ����ϱ���ʱ�жϡ���2��Freeģʽ���ϵ�󣬼�������ֵΪ0xFFFFFFFF���ݼ�������������0���Զ�ѭ���������¼���������ʱ�̣������ö�ʱ��֮ǰ��Чʱ�̣����� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_LTE_PS32KBASE_TIMER_T;    /* Э���Ҷ�ʱ����׼������ */

typedef union
{
    struct
    {
        unsigned int    lte_ps32k_base_mode        : 1; /* [0..0] Э���Ҷ�ʱ����׼����������ģʽѡ��1'b0��Freeģʽ1'b1���û�����ģʽ����ʱ�̣�������Чʱ�̣����� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_LTE_PS32K_BASE_MODE_T;    /* Э���Ҷ�ʱ����׼����������ģʽ */

typedef union
{
    struct
    {
        unsigned int    lte_sys_timer_en           : 1; /* [0..0] SYS��ʱ�������źţ�1'b1��������ʱ��ÿһ�ζ�ʱ����������ٴ�������ʱ��ʱ��Ҫ����ʱ��ʹ������Ϊ0��Ȼ��������Ϊ1����ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_3412_timer_en          : 1; /* [1..1] 3412��ʱ�������źţ�1'b1��������ʱ��ÿһ�ζ�ʱ����������ٴ�������ʱ��ʱ��Ҫ����ʱ��ʹ������Ϊ0��Ȼ��������Ϊ1����ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_vplmn_timer_en         : 1; /* [2..2] VPLMN��ʱ�������źţ�1'b1��������ʱ��ÿһ�ζ�ʱ����������ٴ�������ʱ��ʱ��Ҫ����ʱ��ʹ������Ϊ0��Ȼ��������Ϊ1����ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_tsrhc_timer_en         : 1; /* [3..3] TSRHC��ʱ�������źţ�1'b1��������ʱ��ÿһ�ζ�ʱ����������ٴ�������ʱ��ʱ��Ҫ����ʱ��ʹ������Ϊ0��Ȼ��������Ϊ1����ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_ta_timer_en            : 1; /* [4..4] TA��ʱ�������źţ�1'b1��������ʱ��ÿһ�ζ�ʱ����������ٴ�������ʱ��ʱ��Ҫ����ʱ��ʹ������Ϊ0��Ȼ��������Ϊ1����ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_deeps_timer_en         : 1; /* [5..5] ��˯�߶�ʱ�������źţ�1'b1��������ʱ��ÿһ�ζ�ʱ����������ٴ�������ʱ��ʱ��Ҫ����ʱ��ʹ������Ϊ0��Ȼ��������Ϊ1����ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_ps32k_timer_en         : 1; /* [6..6] Э��ջ��ʱ����ʱ�����������źţ�1'b1��������ʱ��ÿһ�ζ�ʱ����������ٴ�������ʱ��ʱ��Ҫ����ʱ��ʹ������Ϊ0��Ȼ��������Ϊ1����ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_ps32kbase_timer_en     : 1; /* [7..7] Э��ջ��ʱ����׼�����������źţ�1'b1��������ʱ��ÿһ�ζ�ʱ����������ٴ�������ʱ��ʱ��Ҫ����ʱ��ʹ������Ϊ0��Ȼ��������Ϊ1����ʱ�̣�������Чʱ�̣����� */
        unsigned int    reserved                   : 24; /* [31..8] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_DRX_TIMER_EN_T;    /* DRX tim��ʱ��ʹ�� */

typedef union
{
    struct
    {
        unsigned int    drx_timer_10ms             : 9; /* [8..0] 10ms��32Kʱ�Ӹ����������� */
        unsigned int    reserved                   : 23; /* [31..9] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_DRX_TIMER_10MS_T;    /* 10M��ʱ���� */

typedef union
{
    struct
    {
        unsigned int    drx_timer_min              : 21; /* [20..0] 1min��32Kʱ�Ӹ����������� */
        unsigned int    reserved                   : 11; /* [31..21] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_DRX_TIMER_MIN_T;    /* 1���Ӷ�ʱ���� */

typedef union
{
    struct
    {
        unsigned int    drx_timer_hour             : 6; /* [5..0] 1hour�еķ�������Ĭ��Ϊ60min�������� */
        unsigned int    reserved                   : 26; /* [31..6] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_DRX_TIMER_HOUR_T;    /* 1Сʱ��ʱ���� */

typedef union
{
    struct
    {
        unsigned int    lte_sys_int_msk            : 1; /* [0..0] SYS��ʱ���ж����Σ�1'b1�����ж�1'b0�������ж�����ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_3412_int_msk           : 1; /* [1..1] TAU��ʱ���ж����Σ�1'b1�����ж�1'b0�������ж�����ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_vplmn_int_msk          : 1; /* [2..2] VPLMN��ʱ���ж����Σ�1'b1�����ж�1'b0�������ж�����ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_tsrhc_int_msk          : 1; /* [3..3] TSRHC��ʱ���ж����Σ�1'b1�����ж�1'b0�������ж�����ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_ta_int_msk             : 1; /* [4..4] TA��ʱ���ж����Σ�1'b1�����ж�1'b0�������ж�����ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_deeps_int_msk          : 1; /* [5..5] ��˯�߶�ʱ���ж����Σ�1'b1�����ж�1'b0�������ж�����ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_ps32k_int_msk          : 1; /* [6..6] Э��ջ��ʱ����ʱ�������ж����Σ�1'b1�����ж�1'b0�������ж�����ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_ps32kbase_int_msk      : 1; /* [7..7] Э��ջ��ʱ����׼�������ж����Σ�1'b1�����ж�1'b0�������ж�����ʱ�̣�������Чʱ�̣����� */
        unsigned int    reserved                   : 24; /* [31..8] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_T;    /* �ж����� */

typedef union
{
    struct
    {
        unsigned int    lte_sys_int_clear          : 1; /* [0..0] SYS��ʱ���ж���������bitλд1�����Ӧ�Ķ�ʱ���жϼ�״̬������ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_3412_int_clear         : 1; /* [1..1] TAU��ʱ���ж���������bitλд1�����Ӧ�Ķ�ʱ���жϼ�״̬������ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_vplmn_int_clear        : 1; /* [2..2] VPLMN��ʱ���ж���������bitλд1�����Ӧ�Ķ�ʱ���жϼ�״̬������ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_tsrhc_int_clear        : 1; /* [3..3] TSRHC��ʱ���ж���������bitλд1�����Ӧ�Ķ�ʱ���жϼ�״̬������ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_ta_int_clear           : 1; /* [4..4] TA��ʱ���ж���������bitλд1�����Ӧ�Ķ�ʱ���жϼ�״̬������ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_deeps_int_clear        : 1; /* [5..5] ��˯�߶�ʱ���ж���������bitλд1�����Ӧ�Ķ�ʱ���жϼ�״̬������ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_ps32k_int_clear        : 1; /* [6..6] Э��ջ��ʱ����ʱ�������ж���������bitλд1�����Ӧ�Ķ�ʱ���жϼ�״̬������ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_ps32kbase_int_clear    : 1; /* [7..7] Э��ջ��ʱ����׼�������ж���������bitλд1�����Ӧ�Ķ�ʱ���жϼ�״̬������ʱ�̣�������Чʱ�̣����� */
        unsigned int    reserved                   : 24; /* [31..8] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_T;    /* �ж���� */

typedef union
{
    struct
    {
        unsigned int    lte_sys_int_alm            : 1; /* [0..0] SYS��ʱ���жϸ澯�������жϴ�����ԭ�ж�δ���ʱ�����澯��д1��0�澯λ������ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_3412_int_alm           : 1; /* [1..1] TAU��ʱ���жϸ澯�������жϴ�����ԭ�ж�δ���ʱ�����澯��д1��0�澯λ������ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_vplmn_int_alm          : 1; /* [2..2] VPLMN��ʱ���жϸ澯�������жϴ�����ԭ�ж�δ���ʱ�����澯��д1��0�澯λ������ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_tsrhc_int_alm          : 1; /* [3..3] TSRHC��ʱ���жϸ澯�������жϴ�����ԭ�ж�δ���ʱ�����澯��д1��0�澯λ������ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_ta_int_alm             : 1; /* [4..4] TA��ʱ���жϸ澯�������жϴ�����ԭ�ж�û�����ʱ�����澯��д1��0�澯λ������ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_deeps_int_alm          : 1; /* [5..5] ��˯�߶�ʱ���жϸ澯�������жϴ�����ԭ�ж�δ���ʱ�����澯��д1��0�澯λ������ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_ps32k_int_alm          : 1; /* [6..6] Э��ջ��ʱ����ʱ�������жϸ澯�������жϴ�����ԭ�ж�δ���ʱ�����澯��д1��0�澯λ������ʱ�̣�������Чʱ�̣����� */
        unsigned int    lte_ps32kbase_int_alm      : 1; /* [7..7] Э��ջ��ʱ����׼�������жϸ澯�������жϴ�����ԭ�ж�δ���ʱ�����澯��д1��0�澯λ������ʱ�̣�������Чʱ�̣����� */
        unsigned int    reserved                   : 24; /* [31..8] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_T;    /* �жϸ澯 */

typedef union
{
    struct
    {
        unsigned int    clk_switch_state           : 1; /* [0..0] ʱ��״̬��־��1'b1��ʾʱ���л���32.768KHz��1'b0ʱ���л���ϵͳʱ�� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_CLK_SWITCH_STATE_T;    /* ʱ��״ָ̬ʾ */

typedef union
{
    struct
    {
        unsigned int    awake_type                 : 1; /* [0..0] ����Դָʾ��1'b1����ʾ�˴�˯�ߵĻ���Դ��ǿ�ƻ���1'b0����ʾ�˴�˯������Ȼ�����߼���������һ��˯��ʱ������üĴ��� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_AWAKE_TYPE_T;    /* ����״ָ̬ʾ */

typedef union
{
    struct
    {
        unsigned int    drx_sfn_acc                : 32; /* [31..0] ˯��ʱ���ڣ�DRX�ۼӵ���֡���������� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_DRX_SFN_ACC_T;    /* ˯��ʱ�����ۼӵ���֡�� */

typedef union
{
    struct
    {
        unsigned int    slp_time_cur               : 28; /* [27..0] ��ǰ˯��ʱ���ϱ��Ĵ��� */
        unsigned int    reserved                   : 4; /* [31..28] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_SLP_TIME_CUR_T;    /* ��ǰ˯��ʱ�� */

typedef union
{
    struct
    {
        unsigned int    switch_int_state           : 1; /* [0..0] ʱ���л��ж�״ָ̬ʾ�����ж�����Ĵ���ARM_INT_CLEAR��Ӧλд1'b1������жϱ�־ */
        unsigned int    wakeup_int_state           : 1; /* [1..1] ˯�߻����ж�״ָ̬ʾ�����ж�����Ĵ���ARM_INT_CLEAR��Ӧλд1'b1������жϱ�־ */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_ARM_INT_STATE_T;    /* �ж�״̬�ϱ� */

typedef union
{
    struct
    {
        unsigned int    lte_sys_cur_timer          : 3; /* [2..0] ϵͳ���¶�ʱ����ǰֵ�����ö�ʱ��δ������Ĭ��ֵΪ0x0����λΪhour */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_LTE_SYS_CUR_TIMER_T;    /* ϵͳ���¶�ʱ�� */

typedef union
{
    struct
    {
        unsigned int    lte_3412_cur_timer         : 8; /* [7..0] TAU���¶�ʱ����ǰֵ�����ö�ʱ��δ������Ĭ��ֵΪ0x0����λΪmin */
        unsigned int    reserved                   : 24; /* [31..8] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_LTE_3412_CUR_TIMER_T;    /* TAU���¶�ʱ�� */

typedef union
{
    struct
    {
        unsigned int    lte_vplmn_cur_timer        : 3; /* [2..0] VPLMN״̬�����������ȼ�PLMN��ʱ����ǰֵ�����ö�ʱ��δ������Ĭ��ֵΪ0x0����λΪmin */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_LTE_VPLMN_CUR_TIMER_T;    /* VPLMN��ʱ�� */

typedef union
{
    struct
    {
        unsigned int    lte_tsrhc_cur_timer        : 3; /* [2..0] ��������������ʱ����ǰֵ�����ö�ʱ��δ������Ĭ��ֵΪ0x0����λΪmin */
        unsigned int    reserved                   : 29; /* [31..3] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_LTE_TSRHC_CUR_TIMER_T;    /* ������ʱ�� */

typedef union
{
    struct
    {
        unsigned int    lte_ta_cur_timer           : 11; /* [10..0] TA������ʱ����ǰֵ�����ö�ʱ��δ������Ĭ��ֵΪ0x0����λΪms */
        unsigned int    reserved                   : 21; /* [31..11] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_LTE_TA_CUR_TIMER_T;    /* TA������ʱ�� */

typedef union
{
    struct
    {
        unsigned int    lte_deeps_cur_timer        : 7; /* [6..0] ��˯�߶�ʱ����ǰֵ�����ö�ʱ��δ������Ĭ��ֵΪ0x0����λΪmin */
        unsigned int    reserved                   : 25; /* [31..7] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_LTE_DEEPS_CUR_TIMER_T;    /* ��˯�߶�ʱ�� */

typedef union
{
    struct
    {
        unsigned int    lte_ps32k_cur_timer        : 32; /* [31..0] Э��ջ��ʱ����ʱ��������ǰֵ�����ö�ʱ��δ������Ĭ��ֵΪ0x0����λΪһ��32.768KHzʱ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_LTE_PS32K_CUR_TIMER_T;    /* Э���Ҷ�ʱ����ʱ������ */

typedef union
{
    struct
    {
        unsigned int    lte_ps32kbase_cur_timer    : 32; /* [31..0] Э��ջ��ʱ����׼��������ǰֵ�����ö�ʱ��δ������Ĭ��ֵΪ0x0����λΪһ��32.768KHzʱ�� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_LTE_PS32KBASE_CUR_TIMER_T;    /* Э���Ҷ�ʱ����׼������ */

typedef union
{
    struct
    {
        unsigned int    lte_sys_int_state          : 1; /* [0..0] SYS��ʱ���ж�״ָ̬ʾ�����ж�����Ĵ���ARM_INT_CLEAR��Ӧλд1'b1������жϱ�־ */
        unsigned int    lte_3412_int_state         : 1; /* [1..1] TAU��ʱ���ж�״ָ̬ʾ�����ж�����Ĵ���ARM_INT_CLEAR��Ӧλд1'b1������жϱ�־ */
        unsigned int    lte_vplmn_int_state        : 1; /* [2..2] VPLMN��ʱ���ж�״ָ̬ʾ�����ж�����Ĵ���ARM_INT_CLEAR��Ӧλд1'b1������жϱ�־ */
        unsigned int    lte_tsrhc_int_state        : 1; /* [3..3] TSRHC��ʱ���ж�״ָ̬ʾ�����ж�����Ĵ���ARM_INT_CLEAR��Ӧλд1'b1������жϱ�־ */
        unsigned int    lte_ta_int_state           : 1; /* [4..4] TA��ʱ���ж�״ָ̬ʾ�����ж�����Ĵ���ARM_INT_CLEAR��Ӧλд1'b1������жϱ�־ */
        unsigned int    lte_deeps_int_state        : 1; /* [5..5] ��˯�߶�ʱ���ж�״ָ̬ʾ�����ж�����Ĵ���ARM_INT_CLEAR��Ӧλд1'b1������жϱ�־ */
        unsigned int    lte_ps32k_int_state        : 1; /* [6..6] Э��ջ��ʱ����ʱ�������ж�״ָ̬ʾ�����ж�����Ĵ���ARM_INT_CLEAR��Ӧλд1'b1������жϱ�־ */
        unsigned int    lte_ps32kbase_int_state    : 1; /* [7..7] Э��ջ��ʱ����׼�������ж�״ָ̬ʾ�����ж�����Ĵ���ARM_INT_CLEAR��Ӧλд1'b1������жϱ�־ */
        unsigned int    reserved                   : 24; /* [31..8] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_T;    /* �ж�״̬�ϱ� */


/********************************************************************************/
/*    bbp_ltedrx ��������Ŀ��_ģ����_�Ĵ�����_��Ա��_set)        */
/********************************************************************************/
HI_SET_GET(hi_bbp_ltedrx_clk_msr_frac_clk_msr_frac,clk_msr_frac,HI_BBP_LTEDRX_CLK_MSR_FRAC_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_CLK_MSR_FRAC_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_clk_msr_frac_reserved,reserved,HI_BBP_LTEDRX_CLK_MSR_FRAC_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_CLK_MSR_FRAC_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_clk_msr_inte_clk_msr_inte,clk_msr_inte,HI_BBP_LTEDRX_CLK_MSR_INTE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_CLK_MSR_INTE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_clk_msr_inte_reserved,reserved,HI_BBP_LTEDRX_CLK_MSR_INTE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_CLK_MSR_INTE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_sfrm_framl_sfrm_framl,sfrm_framl,HI_BBP_LTEDRX_SFRM_FRAML_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_SFRM_FRAML_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_sfrm_framl_reserved,reserved,HI_BBP_LTEDRX_SFRM_FRAML_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_SFRM_FRAML_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_wakeup_32k_cnt_wakeup_32k_cnt,wakeup_32k_cnt,HI_BBP_LTEDRX_WAKEUP_32K_CNT_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_WAKEUP_32K_CNT_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_wakeup_32k_cnt_reserved,reserved,HI_BBP_LTEDRX_WAKEUP_32K_CNT_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_WAKEUP_32K_CNT_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_switch_32k_cnt_switch_32k_cnt,switch_32k_cnt,HI_BBP_LTEDRX_SWITCH_32K_CNT_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_SWITCH_32K_CNT_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_switch_32k_cnt_reserved,reserved,HI_BBP_LTEDRX_SWITCH_32K_CNT_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_SWITCH_32K_CNT_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_sleep_mode_sleep_mode,sleep_mode,HI_BBP_LTEDRX_SLEEP_MODE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_SLEEP_MODE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_sleep_mode_reserved,reserved,HI_BBP_LTEDRX_SLEEP_MODE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_SLEEP_MODE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_sleep_start_sleep_start,sleep_start,HI_BBP_LTEDRX_SLEEP_START_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_SLEEP_START_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_sleep_start_reserved,reserved,HI_BBP_LTEDRX_SLEEP_START_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_SLEEP_START_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_switch_force_start_switch_force_start,switch_force_start,HI_BBP_LTEDRX_SWITCH_FORCE_START_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_SWITCH_FORCE_START_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_switch_force_start_reserved,reserved,HI_BBP_LTEDRX_SWITCH_FORCE_START_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_SWITCH_FORCE_START_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_sleep_int_msk_switch_int_msk,switch_int_msk,HI_BBP_LTEDRX_ARM_SLEEP_INT_MSK_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_SLEEP_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_sleep_int_msk_wakeup_int_msk,wakeup_int_msk,HI_BBP_LTEDRX_ARM_SLEEP_INT_MSK_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_SLEEP_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_sleep_int_msk_reserved,reserved,HI_BBP_LTEDRX_ARM_SLEEP_INT_MSK_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_SLEEP_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_sleep_int_clear_switch_int_clear,switch_int_clear,HI_BBP_LTEDRX_ARM_SLEEP_INT_CLEAR_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_SLEEP_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_sleep_int_clear_wakeup_int_clear,wakeup_int_clear,HI_BBP_LTEDRX_ARM_SLEEP_INT_CLEAR_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_SLEEP_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_sleep_int_clear_reserved,reserved,HI_BBP_LTEDRX_ARM_SLEEP_INT_CLEAR_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_SLEEP_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_sleep_int_alm_switch_int_alm,switch_int_alm,HI_BBP_LTEDRX_ARM_SLEEP_INT_ALM_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_SLEEP_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_sleep_int_alm_wakeup_int_alm,wakeup_int_alm,HI_BBP_LTEDRX_ARM_SLEEP_INT_ALM_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_SLEEP_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_sleep_int_alm_reserved,reserved,HI_BBP_LTEDRX_ARM_SLEEP_INT_ALM_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_SLEEP_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_sys_timer_lte_sys_timer,lte_sys_timer,HI_BBP_LTEDRX_LTE_SYS_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_SYS_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_sys_timer_reserved,reserved,HI_BBP_LTEDRX_LTE_SYS_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_SYS_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_3412_timer_lte_3412_timer,lte_3412_timer,HI_BBP_LTEDRX_LTE_3412_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_3412_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_3412_timer_reserved,reserved,HI_BBP_LTEDRX_LTE_3412_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_3412_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_vplmn_timer_lte_vplmn_timer,lte_vplmn_timer,HI_BBP_LTEDRX_LTE_VPLMN_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_VPLMN_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_vplmn_timer_reserved,reserved,HI_BBP_LTEDRX_LTE_VPLMN_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_VPLMN_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_tsrhc_timer_lte_tsrhc_timer,lte_tsrhc_timer,HI_BBP_LTEDRX_LTE_TSRHC_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_TSRHC_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_tsrhc_timer_reserved,reserved,HI_BBP_LTEDRX_LTE_TSRHC_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_TSRHC_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_ta_timer_lte_ta_timer,lte_ta_timer,HI_BBP_LTEDRX_LTE_TA_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_TA_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_ta_timer_reserved,reserved,HI_BBP_LTEDRX_LTE_TA_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_TA_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_deeps_timer_lte_deeps_timer,lte_deeps_timer,HI_BBP_LTEDRX_LTE_DEEPS_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_DEEPS_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_deeps_timer_reserved,reserved,HI_BBP_LTEDRX_LTE_DEEPS_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_DEEPS_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_ps32k_timer_lte_ps32k_timer,lte_ps32k_timer,HI_BBP_LTEDRX_LTE_PS32K_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_PS32K_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_ps32kbase_timer_lte_ps32kbase_timer,lte_ps32kbase_timer,HI_BBP_LTEDRX_LTE_PS32KBASE_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_PS32KBASE_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_ps32k_base_mode_lte_ps32k_base_mode,lte_ps32k_base_mode,HI_BBP_LTEDRX_LTE_PS32K_BASE_MODE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_PS32K_BASE_MODE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_ps32k_base_mode_reserved,reserved,HI_BBP_LTEDRX_LTE_PS32K_BASE_MODE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_PS32K_BASE_MODE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_drx_timer_en_lte_sys_timer_en,lte_sys_timer_en,HI_BBP_LTEDRX_DRX_TIMER_EN_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_DRX_TIMER_EN_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_drx_timer_en_lte_3412_timer_en,lte_3412_timer_en,HI_BBP_LTEDRX_DRX_TIMER_EN_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_DRX_TIMER_EN_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_drx_timer_en_lte_vplmn_timer_en,lte_vplmn_timer_en,HI_BBP_LTEDRX_DRX_TIMER_EN_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_DRX_TIMER_EN_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_drx_timer_en_lte_tsrhc_timer_en,lte_tsrhc_timer_en,HI_BBP_LTEDRX_DRX_TIMER_EN_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_DRX_TIMER_EN_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_drx_timer_en_lte_ta_timer_en,lte_ta_timer_en,HI_BBP_LTEDRX_DRX_TIMER_EN_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_DRX_TIMER_EN_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_drx_timer_en_lte_deeps_timer_en,lte_deeps_timer_en,HI_BBP_LTEDRX_DRX_TIMER_EN_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_DRX_TIMER_EN_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_drx_timer_en_lte_ps32k_timer_en,lte_ps32k_timer_en,HI_BBP_LTEDRX_DRX_TIMER_EN_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_DRX_TIMER_EN_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_drx_timer_en_lte_ps32kbase_timer_en,lte_ps32kbase_timer_en,HI_BBP_LTEDRX_DRX_TIMER_EN_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_DRX_TIMER_EN_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_drx_timer_en_reserved,reserved,HI_BBP_LTEDRX_DRX_TIMER_EN_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_DRX_TIMER_EN_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_drx_timer_10ms_drx_timer_10ms,drx_timer_10ms,HI_BBP_LTEDRX_DRX_TIMER_10MS_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_DRX_TIMER_10MS_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_drx_timer_10ms_reserved,reserved,HI_BBP_LTEDRX_DRX_TIMER_10MS_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_DRX_TIMER_10MS_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_drx_timer_min_drx_timer_min,drx_timer_min,HI_BBP_LTEDRX_DRX_TIMER_MIN_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_DRX_TIMER_MIN_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_drx_timer_min_reserved,reserved,HI_BBP_LTEDRX_DRX_TIMER_MIN_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_DRX_TIMER_MIN_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_drx_timer_hour_drx_timer_hour,drx_timer_hour,HI_BBP_LTEDRX_DRX_TIMER_HOUR_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_DRX_TIMER_HOUR_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_drx_timer_hour_reserved,reserved,HI_BBP_LTEDRX_DRX_TIMER_HOUR_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_DRX_TIMER_HOUR_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_msk_lte_sys_int_msk,lte_sys_int_msk,HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_msk_lte_3412_int_msk,lte_3412_int_msk,HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_msk_lte_vplmn_int_msk,lte_vplmn_int_msk,HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_msk_lte_tsrhc_int_msk,lte_tsrhc_int_msk,HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_msk_lte_ta_int_msk,lte_ta_int_msk,HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_msk_lte_deeps_int_msk,lte_deeps_int_msk,HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_msk_lte_ps32k_int_msk,lte_ps32k_int_msk,HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_msk_lte_ps32kbase_int_msk,lte_ps32kbase_int_msk,HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_msk_reserved,reserved,HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_clear_lte_sys_int_clear,lte_sys_int_clear,HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_clear_lte_3412_int_clear,lte_3412_int_clear,HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_clear_lte_vplmn_int_clear,lte_vplmn_int_clear,HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_clear_lte_tsrhc_int_clear,lte_tsrhc_int_clear,HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_clear_lte_ta_int_clear,lte_ta_int_clear,HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_clear_lte_deeps_int_clear,lte_deeps_int_clear,HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_clear_lte_ps32k_int_clear,lte_ps32k_int_clear,HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_clear_lte_ps32kbase_int_clear,lte_ps32kbase_int_clear,HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_clear_reserved,reserved,HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_alm_lte_sys_int_alm,lte_sys_int_alm,HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_alm_lte_3412_int_alm,lte_3412_int_alm,HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_alm_lte_vplmn_int_alm,lte_vplmn_int_alm,HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_alm_lte_tsrhc_int_alm,lte_tsrhc_int_alm,HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_alm_lte_ta_int_alm,lte_ta_int_alm,HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_alm_lte_deeps_int_alm,lte_deeps_int_alm,HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_alm_lte_ps32k_int_alm,lte_ps32k_int_alm,HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_alm_lte_ps32kbase_int_alm,lte_ps32kbase_int_alm,HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_alm_reserved,reserved,HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_clk_switch_state_clk_switch_state,clk_switch_state,HI_BBP_LTEDRX_CLK_SWITCH_STATE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_CLK_SWITCH_STATE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_clk_switch_state_reserved,reserved,HI_BBP_LTEDRX_CLK_SWITCH_STATE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_CLK_SWITCH_STATE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_awake_type_awake_type,awake_type,HI_BBP_LTEDRX_AWAKE_TYPE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_AWAKE_TYPE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_awake_type_reserved,reserved,HI_BBP_LTEDRX_AWAKE_TYPE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_AWAKE_TYPE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_drx_sfn_acc_drx_sfn_acc,drx_sfn_acc,HI_BBP_LTEDRX_DRX_SFN_ACC_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_DRX_SFN_ACC_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_slp_time_cur_slp_time_cur,slp_time_cur,HI_BBP_LTEDRX_SLP_TIME_CUR_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_SLP_TIME_CUR_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_slp_time_cur_reserved,reserved,HI_BBP_LTEDRX_SLP_TIME_CUR_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_SLP_TIME_CUR_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_int_state_switch_int_state,switch_int_state,HI_BBP_LTEDRX_ARM_INT_STATE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_int_state_wakeup_int_state,wakeup_int_state,HI_BBP_LTEDRX_ARM_INT_STATE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_int_state_reserved,reserved,HI_BBP_LTEDRX_ARM_INT_STATE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_sys_cur_timer_lte_sys_cur_timer,lte_sys_cur_timer,HI_BBP_LTEDRX_LTE_SYS_CUR_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_SYS_CUR_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_sys_cur_timer_reserved,reserved,HI_BBP_LTEDRX_LTE_SYS_CUR_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_SYS_CUR_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_3412_cur_timer_lte_3412_cur_timer,lte_3412_cur_timer,HI_BBP_LTEDRX_LTE_3412_CUR_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_3412_CUR_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_3412_cur_timer_reserved,reserved,HI_BBP_LTEDRX_LTE_3412_CUR_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_3412_CUR_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_vplmn_cur_timer_lte_vplmn_cur_timer,lte_vplmn_cur_timer,HI_BBP_LTEDRX_LTE_VPLMN_CUR_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_VPLMN_CUR_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_vplmn_cur_timer_reserved,reserved,HI_BBP_LTEDRX_LTE_VPLMN_CUR_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_VPLMN_CUR_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_tsrhc_cur_timer_lte_tsrhc_cur_timer,lte_tsrhc_cur_timer,HI_BBP_LTEDRX_LTE_TSRHC_CUR_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_TSRHC_CUR_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_tsrhc_cur_timer_reserved,reserved,HI_BBP_LTEDRX_LTE_TSRHC_CUR_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_TSRHC_CUR_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_ta_cur_timer_lte_ta_cur_timer,lte_ta_cur_timer,HI_BBP_LTEDRX_LTE_TA_CUR_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_TA_CUR_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_ta_cur_timer_reserved,reserved,HI_BBP_LTEDRX_LTE_TA_CUR_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_TA_CUR_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_deeps_cur_timer_lte_deeps_cur_timer,lte_deeps_cur_timer,HI_BBP_LTEDRX_LTE_DEEPS_CUR_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_DEEPS_CUR_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_deeps_cur_timer_reserved,reserved,HI_BBP_LTEDRX_LTE_DEEPS_CUR_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_DEEPS_CUR_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_ps32k_cur_timer_lte_ps32k_cur_timer,lte_ps32k_cur_timer,HI_BBP_LTEDRX_LTE_PS32K_CUR_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_PS32K_CUR_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_lte_ps32kbase_cur_timer_lte_ps32kbase_cur_timer,lte_ps32kbase_cur_timer,HI_BBP_LTEDRX_LTE_PS32KBASE_CUR_TIMER_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_LTE_PS32KBASE_CUR_TIMER_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_state_lte_sys_int_state,lte_sys_int_state,HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_state_lte_3412_int_state,lte_3412_int_state,HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_state_lte_vplmn_int_state,lte_vplmn_int_state,HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_state_lte_tsrhc_int_state,lte_tsrhc_int_state,HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_state_lte_ta_int_state,lte_ta_int_state,HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_state_lte_deeps_int_state,lte_deeps_int_state,HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_state_lte_ps32k_int_state,lte_ps32k_int_state,HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_state_lte_ps32kbase_int_state,lte_ps32kbase_int_state,HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_OFFSET)
HI_SET_GET(hi_bbp_ltedrx_arm_timer_int_state_reserved,reserved,HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_T,HI_BBP_LTEDRX_BASE_ADDR, HI_BBP_LTEDRX_ARM_TIMER_INT_STATE_OFFSET)

#endif

#endif // __HI_BBP_LTEDRX_H__

