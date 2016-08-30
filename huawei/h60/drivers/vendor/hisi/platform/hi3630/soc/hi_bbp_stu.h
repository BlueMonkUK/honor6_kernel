/******************************************************************************/
/*  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_bbp_stu.h */
/* Version       : 2.0 */
/* Author        : xxx*/
/* Created       : 2013-02-26*/
/* Last Modified : */
/* Description   :  The C union definition file for the module bbp_stu*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Author        : xxx*/
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_BBP_STU_H__
#define __HI_BBP_STU_H__

/*
 * Project: hi
 * Module : bbp_stu
 */

#ifndef HI_SET_GET
#define HI_SET_GET(a0,a1,a2,a3,a4)
#endif

/********************************************************************************/
/*    bbp_stu �Ĵ���ƫ�ƶ��壨��Ŀ��_ģ����_�Ĵ�����_OFFSET)        */
/********************************************************************************/
#define    HI_BBP_STU_CP_LENGTH_OFFSET                       (0x0) /* CP����ָʾ�Ĵ��� */
#define    HI_BBP_STU_TIM_SWITCH_OFFSET                      (0x4) /* ϵͳ��ʱ�л�ָʾ�Ĵ��� */
#define    HI_BBP_STU_TIM_TRACK_OFFSET                       (0x8) /* ϵͳ��ʱ����ָʾ�Ĵ��� */
#define    HI_BBP_STU_SYS_TIM_OFT_OFFSET                     (0xC) /* ��ʱ�����Ĵ��� */
#define    HI_BBP_STU_SFN_INIT_OFFSET                        (0x10) /* ϵͳ��ʼ֡�żĴ��� */
#define    HI_BBP_STU_SFN_RPT_OFFSET                         (0x14) /* ��ǰϵͳ֡�żĴ��� */
#define    HI_BBP_STU_SUBFRM_NUM_RPT_OFFSET                  (0x18) /* ��ǰ��֡�żĴ��� */
#define    HI_BBP_STU_ABS_TIM_RPT_OFFSET                     (0x1C) /* ��ǰ����ʱ��Ĵ��� */
#define    HI_BBP_STU_BACKUP_TIM_VLD_OFFSET                  (0x20) /* ϵͳ��ʱ������Ч��־�Ĵ��� */
#define    HI_BBP_STU_TIM_BACKUP_OFFSET                      (0x24) /* ϵͳ��ʱ���������Ĵ��� */
#define    HI_BBP_STU_BACKUP_TIM_OFT_OFFSET                  (0x28) /* ϵͳ��ʱ���ݶ�ʱ�Ĵ��� */
#define    HI_BBP_STU_BACKUP_SFN_RPT_OFFSET                  (0x2C) /* ϵͳ��ʱ����֡�żĴ��� */
#define    HI_BBP_STU_BBP_INT_RANGE_OFFSET                   (0x30) /* ��֡�ж�λ�üĴ��� */
#define    HI_BBP_STU_SYS_TIM_RPT_OFFSET                     (0x34) /* ��ǰϵͳ��ʱ�Ĵ��� */
#define    HI_BBP_STU_DSP_SYM_INT_POS_OFFSET                 (0x40) /* DSP�����ж�λ�üĴ��� */
#define    HI_BBP_STU_DEM_SYM_POS_OFFSET                     (0x44) /* �������λ�üĴ��� */
#define    HI_BBP_STU_ARM_SUBFRAME_INT_POS_OFFSET            (0x50) /* ARM��֡�ж�λ�üĴ��� */
#define    HI_BBP_STU_ARM_PUB_INT_POS_OFFSET                 (0x54) /* ARM���ö�ʱ�ж�λ�üĴ��� */
#define    HI_BBP_STU_BBP_VERSION_OFFSET                     (0x70) /* �汾�Ĵ��� */
#define    HI_BBP_STU_BACKUP2_TIM_VLD_OFFSET                 (0x80) /* �����м̶�ʱ��Ч��־�Ĵ��� */
#define    HI_BBP_STU_TIM_BACKUP2_OFFSET                     (0x84) /* �����м̶�ʱ������־�Ĵ��� */
#define    HI_BBP_STU_BACKUP2_TIM_OFT_OFFSET                 (0x88) /* �����м̶�ʱ�Ĵ��� */
#define    HI_BBP_STU_BACKUP2_SFN_RPT_OFFSET                 (0x8C) /* �����м̶�ʱϵͳ֡�żĴ��� */
#define    HI_BBP_STU_AFN_RPT_OFFSET                         (0x90) /* ����֡�żĴ��� */
#define    HI_BBP_STU_TIM_LOCK_OFFSET                        (0x94) /* �������ָʾ�Ĵ��� */
#define    HI_BBP_STU_ABS_INIT_OFFSET                        (0x98) /* ���ԼĴ��� */
#define    HI_BBP_STU_ABS_TIM_ADJ_EN_OFFSET                  (0xA0) /* ����ʱ�����ָʾ�Ĵ��� */
#define    HI_BBP_STU_ABS_TIM_ADJ_OFT_OFFSET                 (0xA4) /* ����ʱ������Ĵ��� */
#define    HI_BBP_STU_WIRELESS_RELAY_CFG_OFFSET              (0xA8) /* �����м��źſ��ƼĴ��� */
#define    HI_BBP_STU_HALF_FRAME_CFG_OFFSET                  (0xAC) /* �Ĵ��� */
#define    HI_BBP_STU_DEM_TIMER_FROM_FPU_OFFSET              (0xB0) /* �����ʱ��Դѡ��Ĵ��� */
#define    HI_BBP_STU_COEX_LINE_OUT_CTRL_OFFSET              (0xC0) /* �Ĵ��� */
#define    HI_BBP_STU_COEX_LINE_IN_CTRL_OFFSET               (0xC4) /* WLAN������ƼĴ��� */
#define    HI_BBP_STU_COEX_TIMER_AHEAD_RX_OFFSET             (0xC8) /* �Ĵ��� */
#define    HI_BBP_STU_COEX_TIMER_DELAY_RX_OFFSET             (0xCC) /* �Ĵ��� */
#define    HI_BBP_STU_COEX_FRAME_SYNC_PULSEWIDTH_OFFSET      (0xD0) /* ֡ͷ�����ȼĴ��� */
#define    HI_BBP_STU_COEX_LTE_RX_TX_ACTIVE_CMD_OFFSET       (0xD4) /* RX��TX�źŸߵ�ָʾ�Ĵ��� */
#define    HI_BBP_STU_COEX_WLAN_BT_LD0_EN_CMD_OFFSET         (0xD8) /* �Ĵ��� */
#define    HI_BBP_STU_COEX_LINE_IN_STATE_OFFSET              (0xDC) /* �Ĵ��� */
#define    HI_BBP_STU_COEX_FRAME_SYNC_OFT_OFFSET             (0xE0) /* ֡ͷ���λ�üĴ��� */
#define    HI_BBP_STU_COEX_OUTPUT_IND_OFFSET                 (0xE4) /* �Ĵ��� */
#define    HI_BBP_STU_COEX_TIMER_AHEAD_TX_OFFSET             (0xE8) /* �Ĵ��� */
#define    HI_BBP_STU_COEX_TIMER_DELAY_OFFSET                (0xEC) /* �Ĵ��� */


#ifndef __ASSEMBLY__

/********************************************************************************/
/*    bbp_stu �Ĵ������壨��Ŀ��_ģ����_�Ĵ�����_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    cp_length                  : 1; /* [0..0] CP����ָʾ��1'b0����CP��1'b1����CP����ʱ�̣�����ʱ����Ч��ʽ����֡ͷ��Ч */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_CP_LENGTH_T;    /* CP����ָʾ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    tim_switch                 : 1; /* [0..0] ϵͳ��ʱ�л���Ч��־��1'b0����Ч��1'b1����Ч����ʱ�̣�����ʱ����Ч��ʽ��������Ч���߼����е���С���������֡ͷ */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_TIM_SWITCH_T;    /* ϵͳ��ʱ�л�ָʾ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    tim_track                  : 1; /* [0..0] ϵͳ��ʱ������Ч��־��1'0����Ч��1'b1����Ч����ʱ�̣�����ʱ����Ч��ʽ��������Ч���߼����е��µ���֡ͷ */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_TIM_TRACK_T;    /* ϵͳ��ʱ����ָʾ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    sys_cyc_oft                : 3; /* [2..0] ���е�ϵͳ�ͽ��Ts�ڶ�ʱ����Ծ���ʱ���Ts��ƫ�ƣ�1/4Ts��������ʱ�̣�����ʱ����Ч��ʽ����ʱ�л���Чʱ��Ч */
        unsigned int    reserved_2                 : 1; /* [3..3] ���� */
        unsigned int    sys_ts_oft                 : 15; /* [18..4] ���е�ϵͳ�ͽ��֡��ʱ����Ծ���ʱ�����֡��ƫ�ƣ�Ts���ȣ�0��30719 */
        unsigned int    reserved_1                 : 1; /* [19..19] ���� */
        unsigned int    sys_sub_oft                : 4; /* [23..20] ���е�ϵͳ�ͽ��֡��ʱ����Ծ���ʱ�����֡��ƫ�ƣ�0��9����ʱ�̣�����ʱ����Ч��ʽ����ʱ�л���Чʱ��Ч */
        unsigned int    reserved_0                 : 8; /* [31..24] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_SYS_TIM_OFT_T;    /* ��ʱ�����Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    sfn_init                   : 10; /* [9..0] ��������SFN��ֵ����ʱ�̣�����ʱ����Ч��ʽ��������Ч */
        unsigned int    reserved_1                 : 6; /* [15..10] ���� */
        unsigned int    sfn_init_en                : 1; /* [16..16] SFN��ֵ��Чָʾ��1'b1��Ч���߼���������ʱ�̣�����ʱ����Ч��ʽ��������Ч */
        unsigned int    reserved_0                 : 15; /* [31..17] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_SFN_INIT_T;    /* ϵͳ��ʼ֡�żĴ��� */

typedef union
{
    struct
    {
        unsigned int    sfn_rpt                    : 10; /* [9..0] ��ǰ֡��SFNֵ��0��1023�ϱ�ʱ�̣�ϵͳ֡ͷ����ʱ�� */
        unsigned int    reserved                   : 22; /* [31..10] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_SFN_RPT_T;    /* ��ǰϵͳ֡�żĴ��� */

typedef union
{
    struct
    {
        unsigned int    subfrm_num_rpt             : 4; /* [3..0] ��ǰ��֡�ţ�0��9�ϱ�ʱ�̣�ϵͳ��֡ͷ����ʱ�� */
        unsigned int    reserved                   : 28; /* [31..4] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_SUBFRM_NUM_RPT_T;    /* ��ǰ��֡�żĴ��� */

typedef union
{
    struct
    {
        unsigned int    abs_cyc_rpt                : 3; /* [2..0] ����ʱ�굱ǰ����ֵ��Ts��ƫ�ƣ�1/4Ts�����ϱ�ʱ�̣�����ʱ�� */
        unsigned int    reserved_2                 : 1; /* [3..3] ���� */
        unsigned int    abs_ts_rpt                 : 15; /* [18..4] ����ʱ�굱ǰ����ֵ����֡��ƫ�ƣ�Ts���ȣ�0��30719 */
        unsigned int    reserved_1                 : 1; /* [19..19] ���� */
        unsigned int    abs_sub_rpt                : 4; /* [23..20] ����ʱ�굱ǰ����ֵ����֡��ƫ�ƣ�0��9 */
        unsigned int    reserved_0                 : 8; /* [31..24] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_ABS_TIM_RPT_T;    /* ��ǰ����ʱ��Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    backup_tim_vld             : 1; /* [0..0] ϵͳ��ʱ������Ч��־��1'b1��Ч����ʱ�̣�����ʱ����Ч��ʽ��������Ч */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_BACKUP_TIM_VLD_T;    /* ϵͳ��ʱ������Ч��־�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    tim_backup                 : 1; /* [0..0] ϵͳ��ʱ����������־��1'b1��Ч���߼���������ʱ�̣�����ʱ����Ч��ʽ��������Ч */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_TIM_BACKUP_T;    /* ϵͳ��ʱ���������Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    backup_cyc_oft             : 3; /* [2..0] ���ݵ�ϵͳ��ʱ����Ծ���ʱ���Ts��ƫ�ƣ�1/4Ts��������ʱ�̣�����ʱ����Ч��ʽ��������Ч */
        unsigned int    reserved_2                 : 1; /* [3..3] ���� */
        unsigned int    backup_ts_oft              : 15; /* [18..4] ���ݵ�ϵͳ��ʱ����Ծ���ʱ�����֡��ƫ�ƣ�Ts���ȣ�0��30719 */
        unsigned int    reserved_1                 : 1; /* [19..19] ���� */
        unsigned int    backup_sub_oft             : 4; /* [23..20] ���ݵ�ϵͳ��ʱ����Ծ���ʱ�����֡��ƫ�ƣ�0��9����ʱ�̣�����ʱ����Ч��ʽ����ʱ�л���Чʱ��Ч */
        unsigned int    reserved_0                 : 8; /* [31..24] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_BACKUP_TIM_OFT_T;    /* ϵͳ��ʱ���ݶ�ʱ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    backup_sfn_rpt             : 10; /* [9..0] ϵͳ��ʱ���ݵ�SFNֵ��0��1023�ϱ�ʱ�̣�����ʱ�� */
        unsigned int    reserved                   : 22; /* [31..10] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_BACKUP_SFN_RPT_T;    /* ϵͳ��ʱ����֡�żĴ��� */

typedef union
{
    struct
    {
        unsigned int    bbp_int_range              : 8; /* [7..0] ��ʱ�ж�λ�õķ�Χ��������ϵͳ��֡ͷ����BBP_INT_RANGE��Ts��Χ�ڣ�32��TsΪ��λ��ȱʡֵ320��Ts����Ҫ�̶�Ϊ0x0A�������Ҫ��оƬ��Աȷ�ϺϷ�ֵ����ʱ�̣�����ʱ����Ч��ʽ��������Ч */
        unsigned int    reserved                   : 24; /* [31..8] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_BBP_INT_RANGE_T;    /* ��֡�ж�λ�üĴ��� */

typedef union
{
    struct
    {
        unsigned int    sys_cyc_rpt                : 3; /* [2..0] ϵͳ��ʱ��ǰ����ֵ��Ts��ƫ�ƣ�1/4Ts�����ϱ�ʱ�̣�����ʱ�� */
        unsigned int    reserved_2                 : 1; /* [3..3] ���� */
        unsigned int    sys_ts_rpt                 : 15; /* [18..4] ϵͳ��ʱ��ǰ����ֵ����֡��ƫ�ƣ�Ts���ȣ�0��30719 */
        unsigned int    reserved_1                 : 1; /* [19..19] ���� */
        unsigned int    sys_sub_rpt                : 4; /* [23..20] ϵͳ��ʱ��ǰ����ֵ����֡��ƫ�ƣ�0��9 */
        unsigned int    reserved_0                 : 8; /* [31..24] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_SYS_TIM_RPT_T;    /* ��ǰϵͳ��ʱ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dsp_symint_ts_pos          : 12; /* [11..0] �����ж�λ�ã���ʾ�ӳ�ϵͳ��ʱ����OFDM������ʼʱ�̵�ʱ�䣬��λΪTsȡֵ��Χ��16��1615����ʱ�̣�����ʱ����Ч��ʽ��ϵͳ��֡ͷ��Ч */
        unsigned int    reserved                   : 20; /* [31..12] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_DSP_SYM_INT_POS_T;    /* DSP�����ж�λ�üĴ��� */

typedef union
{
    struct
    {
        unsigned int    dem_sym_pos                : 12; /* [11..0] �������λ�ã���ʾ�ӳ�ϵͳ��ʱ����OFDM������ʼʱ�̵�ʱ�䣬��λΪTsȡֵ��Χ��1096��1615����ʱ�̣�����ʱ����Ч��ʽ��������Ч */
        unsigned int    reserved                   : 20; /* [31..12] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_DEM_SYM_POS_T;    /* �������λ�üĴ��� */

typedef union
{
    struct
    {
        unsigned int    arm_subf_int_pos           : 15; /* [14..0] ARM��֡�жϵ�λ�ã��������ϵͳ��֡ͷλ�õ���ʱ����λΪTs��ȡֵ��Χ�� 0��30143����ʱ�̣�����ʱ����Ч��ʽ��ϵͳ��֡ͷ��Ч */
        unsigned int    reserved                   : 17; /* [31..15] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_ARM_SUBFRAME_INT_POS_T;    /* ARM��֡�ж�λ�üĴ��� */

typedef union
{
    struct
    {
        unsigned int    arm_pubint_pos             : 15; /* [14..0] ARM���ö�ʱ�жϵ�λ�ã��������ϵͳ��֡ͷλ�õ���ʱ����λΪTs��ȡֵ��Χ��0��30143����ʱ�̣�����ʱ����Ч��ʽ��ϵͳ��֡ͷ��Ч */
        unsigned int    reserved                   : 17; /* [31..15] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_ARM_PUB_INT_POS_T;    /* ARM���ö�ʱ�ж�λ�üĴ��� */

typedef union
{
    struct
    {
        unsigned int    bbp_version                : 32; /* [31..0] BBP�汾�Ĵ��� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_BBP_VERSION_T;    /* �汾�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    backup2_tim_vld            : 1; /* [0..0] �����м̶�ʱ��Ч��־��1����Ч����ʱ�̣�����ʱ����Ч��ʽ��������Ч */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_BACKUP2_TIM_VLD_T;    /* �����м̶�ʱ��Ч��־�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    tim_backup2                : 1; /* [0..0] �����м̶�ʱ������־��1����Ч����ʱ�̣�����ʱ����Ч��ʽ��ϵͳ��ʱ320Tsʱ����Ч */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_TIM_BACKUP2_T;    /* �����м̶�ʱ������־�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    backup2_cyc_oft            : 3; /* [2..0] �����м̶�ʱ���ϵͳ��ʱ��Ts��ƫ�ƣ�1/4Ts���ȣ������м̶�ʱ���ϵͳ��ʱ����֡��ƫ������Ϊ0����֡��ƫ������Ϊ0ʱ��Ts��ƫ������Ϊ0�������м̶�ʱ���ϵͳ��ʱ����֡��ƫ������Ϊ9����֡��ƫ������Ϊ9584~30719ʱ��Ts��ƫ������Ϊ0~3����ʱ�̣�����ʱ����Ч��ʽ��TIM_BACKUP2��Чʱ��Ч */
        unsigned int    reserved_2                 : 1; /* [3..3] ���� */
        unsigned int    backup2_ts_oft             : 15; /* [18..4] �����м̶�ʱ���ϵͳ��ʱ����֡��ƫ�ƣ�Ts���ȣ������м̶�ʱ���ϵͳ��ʱ����֡��ƫ������Ϊ0ʱ����֡��ƫ������Ϊ0�������м̶�ʱ���ϵͳ��ʱ����֡��ƫ������Ϊ9ʱ����֡��ƫ������Ϊ9584~30719����ʱ�̣�����ʱ����Ч��ʽ��TIM_BACKUP2��Чʱ��Ч */
        unsigned int    reserved_1                 : 1; /* [19..19] ���� */
        unsigned int    backup2_sub_oft            : 4; /* [23..20] �����м̶�ʱ���ϵͳ��ʱ����֡��ƫ�ƣ�����Ϊ0��9����ʱ�̣�����ʱ����Ч��ʽ��TIM_BACKUP2��Чʱ��Ч */
        unsigned int    reserved_0                 : 8; /* [31..24] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_BACKUP2_TIM_OFT_T;    /* �����м̶�ʱ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    backup2_sfn_rpt            : 10; /* [9..0] �����м̶�ʱ��SFNֵ��0��1023�ϱ�ʱ�̣�����ʱ�� */
        unsigned int    reserved                   : 22; /* [31..10] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_BACKUP2_SFN_RPT_T;    /* �����м̶�ʱϵͳ֡�żĴ��� */

typedef union
{
    struct
    {
        unsigned int    afn_rpt                    : 32; /* [31..0] ����֡�� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_AFN_RPT_T;    /* ����֡�żĴ��� */

typedef union
{
    struct
    {
        unsigned int    tim_lock                   : 1; /* [0..0] ����ָʾ�����壩�����ָʾ����ʱSTU��AFN������ʱ�ꡢSFN��ϵͳ��ʱ��ϵͳ��֡��������ס���ϱ� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_TIM_LOCK_T;    /* �������ָʾ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    abs_init_ts_pos            : 15; /* [14..0] ���ԼĴ��� */
        unsigned int    abs_init_ts_pos_en         : 1; /* [15..15] ���ԼĴ��� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_ABS_INIT_T;    /* ���ԼĴ��� */

typedef union
{
    struct
    {
        unsigned int    abs_tim_adj_en             : 1; /* [0..0] ����ʱ�������������Чָʾ������Ч���߼������㣬�߼��յ���ָʾ�󽫵�ǰ����ʱ�����ֵ��Ts�����abs_delta_ts��ģ30720�õ��µ�Ts���������õ���֡��λ����֡��λ�뵱ǰ����ʱ�����ֵ����֡���abs_delta_subf��ӣ����ģ10���õ��µ���֡�����ֵ��Ts�ڼ���ֵ���ֲ�������ʱ�̣���ģ��Ҫ�ָ�����ʱ��ʱ��Чʱ�̣�������Ч */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_ABS_TIM_ADJ_EN_T;    /* ����ʱ�����ָʾ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    abs_delta_ts               : 15; /* [14..0] ����ʱ����Ե���������֡��Ts�����������÷�Χ��0~30719����ʱ�̣���ģ��Ҫ�ָ�����ʱ��ʱ��Чʱ�̣�������Ч */
        unsigned int    reserved_1                 : 1; /* [15..15] ���� */
        unsigned int    abs_delta_subf             : 4; /* [19..16] ����ʱ����Ե���������֡�����������λΪ��֡�����÷�Χ��0~9����ʱ�̣���ģ��Ҫ�ָ�����ʱ��ʱ��Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 12; /* [31..20] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_ABS_TIM_ADJ_OFT_T;    /* ����ʱ������Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    relay_multi_sync_mode      : 1; /* [0..0] �����м̶���;�ź�ģʽ��1��ÿ֡�����0��ÿSFN mod 4 = 0�������ʱ�̣�����ʱ����Ч��ʽ��������Ч */
        unsigned int    reserved_1                 : 15; /* [15..1] ���� */
        unsigned int    relay_sync_interval        : 2; /* [17..16] �����м��ź�չ������00��SFN mod 128 = 0չ��01��SFN mod 64 = 0չ��10��SFN mod 256 = 0չ��11��SFN mod 512 = 0չ������ʱ�̣�����ʱ����Ч��ʽ��������Ч */
        unsigned int    reserved_0                 : 14; /* [31..18] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_WIRELESS_RELAY_CFG_T;    /* �����м��źſ��ƼĴ��� */

typedef union
{
    struct
    {
        unsigned int    cfg_subfrm_num0            : 4; /* [3..0] ָ����֡��0����ʱ�̣���ʱ�л�ǰ��Ч��ʽ��������Ч */
        unsigned int    cfg_subfrm_num1            : 4; /* [7..4] ָ����֡��1����ʱ�̣���ʱ�л�ǰ��Ч��ʽ��������Ч */
        unsigned int    reserved_1                 : 7; /* [14..8] ���� */
        unsigned int    half_frame_mask            : 1; /* [15..15] ��ʱ�л�������жϴ�(ָ����֡+1)��ʼ��Ч��1����ʱ�л�������жϴ�(ָ����֡+1)��ʼ��Ч0����ʱ�л�������жϴ���һ��֡��ʼ��Ч����ʱ�̣���ʱ�л�ǰ��Ч��ʽ��������Ч */
        unsigned int    reserved_0                 : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_HALF_FRAME_CFG_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    dem_timer_from_fpu         : 1; /* [0..0] �����ʱ��Դ��1�����Խ����ʱ1��0�����Խ����ʱ2��Ĭ������Ϊ1����ʱ�̣��ϵ��ʼ����Ч��ʽ��������Ч */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_DEM_TIMER_FROM_FPU_T;    /* �����ʱ��Դѡ��Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    lte_frame_sync_ctrl        : 2; /* [1..0] LTE_Frame_Sync����źſ���0�����ǿ��Ϊ0��1�����ǿ��Ϊ1��2���߼��Զ����ƣ�3��Invalid */
        unsigned int    reserved_3                 : 2; /* [3..2] ���� */
        unsigned int    lte_rx_active_ctrl         : 2; /* [5..4] LTE_RX_Active����źſ���0�����ǿ��Ϊ0��1�����ǿ��Ϊ1��2���߼��Զ����ƣ�3��Invalid */
        unsigned int    reserved_2                 : 2; /* [7..6] ���� */
        unsigned int    lte_tx_active_ctrl         : 2; /* [9..8] LTE_TX_Active����źſ���0�����ǿ��Ϊ0��1�����ǿ��Ϊ1��2���߼��Զ����ƣ�3��Invalid */
        unsigned int    reserved_1                 : 2; /* [11..10] ���� */
        unsigned int    wlan_bt_ldo_en_ctrl        : 2; /* [13..12] WLAN_BT_LDO_EN����źſ���0�����ǿ��Ϊ0��1�����ǿ��Ϊ1��2���߼��Զ����ƣ�3��Invalid */
        unsigned int    reserved_0                 : 18; /* [31..14] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_COEX_LINE_OUT_CTRL_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    wlan_bt_priority_ctrl      : 2; /* [1..0] WLAN_BT_Priority�����ź��Ƿ�����ƣ��˴���������ϱ������ݸ����к��ж�ģ��0������ʵ�����룬���ǿ��Ϊ0��1������ʵ�����룬���ǿ��Ϊ1��2��͸����3��ȡ�� */
        unsigned int    reserved_1                 : 2; /* [3..2] ���� */
        unsigned int    wlan_bt_tx_active_ctrl     : 2; /* [5..4] WLAN_BT_TX_ Active�����ź��Ƿ�����ƣ��˴���������ϱ������ݸ����к��ж�ģ��0������ʵ�����룬���ǿ��Ϊ0��1������ʵ�����룬���ǿ��Ϊ1��2��͸����3��ȡ�� */
        unsigned int    reserved_0                 : 26; /* [31..6] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_COEX_LINE_IN_CTRL_T;    /* WLAN������ƼĴ��� */

typedef union
{
    struct
    {
        unsigned int    coex_t_ahead_rx            : 16; /* [15..0] LTE_RX_Active�ź�����ʱ�̶�Ӧ��ϵͳ��ʱλ�ã���λTs30719>=coex_t_ahead_rx>(30719-9216)Ts */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_COEX_TIMER_AHEAD_RX_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    coex_t_delay_rx            : 16; /* [15..0] LTE_RX_Active�źŴӸ߱��ʱ��Ӧ��ϵͳ��ʱλ�ã���λTs0<=coex_t_delay_rx<30000Ts */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_COEX_TIMER_DELAY_RX_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    frame_sync_pulsewidth      : 16; /* [15..0] ֡ͷ�����ȣ���λTs1<=frame_sync_pulsewidth<128Ts */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_COEX_FRAME_SYNC_PULSEWIDTH_T;    /* ֡ͷ�����ȼĴ��� */

typedef union
{
    struct
    {
        unsigned int    coex_lte_rx_active_cmd     : 1; /* [0..0] LTE_RX_Active�źŸߵ�ָʾ,1����0���� */
        unsigned int    reserved_1                 : 15; /* [15..1] ���� */
        unsigned int    coex_lte_tx_active_cmd     : 1; /* [16..16] LTE_TX_Active�źŸߵ�ָʾ,1����0���� */
        unsigned int    reserved_0                 : 15; /* [31..17] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_COEX_LTE_RX_TX_ACTIVE_CMD_T;    /* RX��TX�źŸߵ�ָʾ�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    wlan_bt_ldo_en_cmd         : 1; /* [0..0] WLAN_BT_LDO_EN�Ƿ���COEX_LTE_RX_ACTIVE_CMD����ָʾ��0������1�������� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_COEX_WLAN_BT_LD0_EN_CMD_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    wlan_bt_priority_state     : 1; /* [0..0] wlan_bt_priority״̬�ϱ��������ϱ��Ѿ���COEX_LINE_IN_CTRL�Ĵ���ָʾ�Ĳ�����0����1���� */
        unsigned int    reserved_1                 : 3; /* [3..1] ���� */
        unsigned int    wlan_bt_tx_active_state    : 1; /* [4..4] wlan_bt_tx_active״̬�ϱ��������ϱ��Ѿ���COEX_LINE_IN_CTRL�Ĵ���ָʾ�Ĳ�����0����1���� */
        unsigned int    reserved_0                 : 27; /* [31..5] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_COEX_LINE_IN_STATE_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    coex_frame_sync_ts_oft     : 15; /* [14..0] LTE_Frame_Sync����ź���ϵͳ��ʱ�е�λ�ã���֡��Tsλ�ã����÷�Χ��0~30143����ʱ�̣�����ʱ����Чʱ�̣�������Ч */
        unsigned int    reserved_1                 : 1; /* [15..15] ���� */
        unsigned int    coex_frame_sync_subfrm_oft : 4; /* [19..16] LTE_Frame_Sync����ź���ϵͳ��ʱ�е�λ�ã���֡λ�ã���λΪ��֡�����÷�Χ��0~9����ʱ�̣�����ʱ����Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 12; /* [31..20] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_COEX_FRAME_SYNC_OFT_T;    /* ֡ͷ���λ�üĴ��� */

typedef union
{
    struct
    {
        unsigned int    wlan_bt_rx_active_ind      : 1; /* [0..0] lte_rx_active����ź���Чָʾ��1����������Ч0����������Ч����ʱ�̣�����ʱ����Чʱ�̣�������Ч */
        unsigned int    wlan_bt_lte_frame_sync_ind : 1; /* [1..1] lte_frame_sync����ź���Чָʾ��1����������Ч0����������Ч����ʱ�̣�����ʱ����Чʱ�̣�������Ч */
        unsigned int    reserved_1                 : 14; /* [15..2]  */
        unsigned int    wlan_bt_tx_active_ind      : 1; /* [16..16] lte_tx_active����ź���Чָʾ��1����������Ч0����������Ч����ʱ�̣�����ʱ����Чʱ�̣�������Ч */
        unsigned int    reserved_0                 : 15; /* [31..17] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_COEX_OUTPUT_IND_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    coex_t_ahead_tx            : 16; /* [15..0] LTE_TX_Active�ź�����ʱ�̶�Ӧ��ϵͳ��ʱλ�ã���λTs30719>=coex_t_ahead_tx>(30719-9216)Ts */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_COEX_TIMER_AHEAD_TX_T;    /* �Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    coex_t_delay_tx            : 16; /* [15..0] LTE_TX_Active�źŴӸ߱��ʱ��Ӧ��ϵͳ��ʱλ�ã���λTs0<=coex_t_delay_tx<30000Ts */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_STU_COEX_TIMER_DELAY_T;    /* �Ĵ��� */


/********************************************************************************/
/*    bbp_stu ��������Ŀ��_ģ����_�Ĵ�����_��Ա��_set)        */
/********************************************************************************/
HI_SET_GET(hi_bbp_stu_cp_length_cp_length,cp_length,HI_BBP_STU_CP_LENGTH_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_CP_LENGTH_OFFSET)
HI_SET_GET(hi_bbp_stu_cp_length_reserved,reserved,HI_BBP_STU_CP_LENGTH_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_CP_LENGTH_OFFSET)
HI_SET_GET(hi_bbp_stu_tim_switch_tim_switch,tim_switch,HI_BBP_STU_TIM_SWITCH_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_TIM_SWITCH_OFFSET)
HI_SET_GET(hi_bbp_stu_tim_switch_reserved,reserved,HI_BBP_STU_TIM_SWITCH_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_TIM_SWITCH_OFFSET)
HI_SET_GET(hi_bbp_stu_tim_track_tim_track,tim_track,HI_BBP_STU_TIM_TRACK_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_TIM_TRACK_OFFSET)
HI_SET_GET(hi_bbp_stu_tim_track_reserved,reserved,HI_BBP_STU_TIM_TRACK_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_TIM_TRACK_OFFSET)
HI_SET_GET(hi_bbp_stu_sys_tim_oft_sys_cyc_oft,sys_cyc_oft,HI_BBP_STU_SYS_TIM_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SYS_TIM_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_sys_tim_oft_reserved_2,reserved_2,HI_BBP_STU_SYS_TIM_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SYS_TIM_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_sys_tim_oft_sys_ts_oft,sys_ts_oft,HI_BBP_STU_SYS_TIM_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SYS_TIM_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_sys_tim_oft_reserved_1,reserved_1,HI_BBP_STU_SYS_TIM_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SYS_TIM_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_sys_tim_oft_sys_sub_oft,sys_sub_oft,HI_BBP_STU_SYS_TIM_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SYS_TIM_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_sys_tim_oft_reserved_0,reserved_0,HI_BBP_STU_SYS_TIM_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SYS_TIM_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_sfn_init_sfn_init,sfn_init,HI_BBP_STU_SFN_INIT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SFN_INIT_OFFSET)
HI_SET_GET(hi_bbp_stu_sfn_init_reserved_1,reserved_1,HI_BBP_STU_SFN_INIT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SFN_INIT_OFFSET)
HI_SET_GET(hi_bbp_stu_sfn_init_sfn_init_en,sfn_init_en,HI_BBP_STU_SFN_INIT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SFN_INIT_OFFSET)
HI_SET_GET(hi_bbp_stu_sfn_init_reserved_0,reserved_0,HI_BBP_STU_SFN_INIT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SFN_INIT_OFFSET)
HI_SET_GET(hi_bbp_stu_sfn_rpt_sfn_rpt,sfn_rpt,HI_BBP_STU_SFN_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SFN_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_sfn_rpt_reserved,reserved,HI_BBP_STU_SFN_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SFN_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_subfrm_num_rpt_subfrm_num_rpt,subfrm_num_rpt,HI_BBP_STU_SUBFRM_NUM_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SUBFRM_NUM_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_subfrm_num_rpt_reserved,reserved,HI_BBP_STU_SUBFRM_NUM_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SUBFRM_NUM_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_abs_tim_rpt_abs_cyc_rpt,abs_cyc_rpt,HI_BBP_STU_ABS_TIM_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ABS_TIM_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_abs_tim_rpt_reserved_2,reserved_2,HI_BBP_STU_ABS_TIM_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ABS_TIM_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_abs_tim_rpt_abs_ts_rpt,abs_ts_rpt,HI_BBP_STU_ABS_TIM_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ABS_TIM_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_abs_tim_rpt_reserved_1,reserved_1,HI_BBP_STU_ABS_TIM_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ABS_TIM_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_abs_tim_rpt_abs_sub_rpt,abs_sub_rpt,HI_BBP_STU_ABS_TIM_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ABS_TIM_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_abs_tim_rpt_reserved_0,reserved_0,HI_BBP_STU_ABS_TIM_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ABS_TIM_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_backup_tim_vld_backup_tim_vld,backup_tim_vld,HI_BBP_STU_BACKUP_TIM_VLD_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP_TIM_VLD_OFFSET)
HI_SET_GET(hi_bbp_stu_backup_tim_vld_reserved,reserved,HI_BBP_STU_BACKUP_TIM_VLD_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP_TIM_VLD_OFFSET)
HI_SET_GET(hi_bbp_stu_tim_backup_tim_backup,tim_backup,HI_BBP_STU_TIM_BACKUP_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_TIM_BACKUP_OFFSET)
HI_SET_GET(hi_bbp_stu_tim_backup_reserved,reserved,HI_BBP_STU_TIM_BACKUP_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_TIM_BACKUP_OFFSET)
HI_SET_GET(hi_bbp_stu_backup_tim_oft_backup_cyc_oft,backup_cyc_oft,HI_BBP_STU_BACKUP_TIM_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP_TIM_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_backup_tim_oft_reserved_2,reserved_2,HI_BBP_STU_BACKUP_TIM_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP_TIM_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_backup_tim_oft_backup_ts_oft,backup_ts_oft,HI_BBP_STU_BACKUP_TIM_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP_TIM_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_backup_tim_oft_reserved_1,reserved_1,HI_BBP_STU_BACKUP_TIM_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP_TIM_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_backup_tim_oft_backup_sub_oft,backup_sub_oft,HI_BBP_STU_BACKUP_TIM_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP_TIM_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_backup_tim_oft_reserved_0,reserved_0,HI_BBP_STU_BACKUP_TIM_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP_TIM_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_backup_sfn_rpt_backup_sfn_rpt,backup_sfn_rpt,HI_BBP_STU_BACKUP_SFN_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP_SFN_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_backup_sfn_rpt_reserved,reserved,HI_BBP_STU_BACKUP_SFN_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP_SFN_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_bbp_int_range_bbp_int_range,bbp_int_range,HI_BBP_STU_BBP_INT_RANGE_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BBP_INT_RANGE_OFFSET)
HI_SET_GET(hi_bbp_stu_bbp_int_range_reserved,reserved,HI_BBP_STU_BBP_INT_RANGE_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BBP_INT_RANGE_OFFSET)
HI_SET_GET(hi_bbp_stu_sys_tim_rpt_sys_cyc_rpt,sys_cyc_rpt,HI_BBP_STU_SYS_TIM_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SYS_TIM_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_sys_tim_rpt_reserved_2,reserved_2,HI_BBP_STU_SYS_TIM_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SYS_TIM_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_sys_tim_rpt_sys_ts_rpt,sys_ts_rpt,HI_BBP_STU_SYS_TIM_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SYS_TIM_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_sys_tim_rpt_reserved_1,reserved_1,HI_BBP_STU_SYS_TIM_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SYS_TIM_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_sys_tim_rpt_sys_sub_rpt,sys_sub_rpt,HI_BBP_STU_SYS_TIM_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SYS_TIM_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_sys_tim_rpt_reserved_0,reserved_0,HI_BBP_STU_SYS_TIM_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_SYS_TIM_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_dsp_sym_int_pos_dsp_symint_ts_pos,dsp_symint_ts_pos,HI_BBP_STU_DSP_SYM_INT_POS_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_DSP_SYM_INT_POS_OFFSET)
HI_SET_GET(hi_bbp_stu_dsp_sym_int_pos_reserved,reserved,HI_BBP_STU_DSP_SYM_INT_POS_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_DSP_SYM_INT_POS_OFFSET)
HI_SET_GET(hi_bbp_stu_dem_sym_pos_dem_sym_pos,dem_sym_pos,HI_BBP_STU_DEM_SYM_POS_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_DEM_SYM_POS_OFFSET)
HI_SET_GET(hi_bbp_stu_dem_sym_pos_reserved,reserved,HI_BBP_STU_DEM_SYM_POS_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_DEM_SYM_POS_OFFSET)
HI_SET_GET(hi_bbp_stu_arm_subframe_int_pos_arm_subf_int_pos,arm_subf_int_pos,HI_BBP_STU_ARM_SUBFRAME_INT_POS_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ARM_SUBFRAME_INT_POS_OFFSET)
HI_SET_GET(hi_bbp_stu_arm_subframe_int_pos_reserved,reserved,HI_BBP_STU_ARM_SUBFRAME_INT_POS_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ARM_SUBFRAME_INT_POS_OFFSET)
HI_SET_GET(hi_bbp_stu_arm_pub_int_pos_arm_pubint_pos,arm_pubint_pos,HI_BBP_STU_ARM_PUB_INT_POS_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ARM_PUB_INT_POS_OFFSET)
HI_SET_GET(hi_bbp_stu_arm_pub_int_pos_reserved,reserved,HI_BBP_STU_ARM_PUB_INT_POS_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ARM_PUB_INT_POS_OFFSET)
HI_SET_GET(hi_bbp_stu_bbp_version_bbp_version,bbp_version,HI_BBP_STU_BBP_VERSION_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BBP_VERSION_OFFSET)
HI_SET_GET(hi_bbp_stu_backup2_tim_vld_backup2_tim_vld,backup2_tim_vld,HI_BBP_STU_BACKUP2_TIM_VLD_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP2_TIM_VLD_OFFSET)
HI_SET_GET(hi_bbp_stu_backup2_tim_vld_reserved,reserved,HI_BBP_STU_BACKUP2_TIM_VLD_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP2_TIM_VLD_OFFSET)
HI_SET_GET(hi_bbp_stu_tim_backup2_tim_backup2,tim_backup2,HI_BBP_STU_TIM_BACKUP2_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_TIM_BACKUP2_OFFSET)
HI_SET_GET(hi_bbp_stu_tim_backup2_reserved,reserved,HI_BBP_STU_TIM_BACKUP2_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_TIM_BACKUP2_OFFSET)
HI_SET_GET(hi_bbp_stu_backup2_tim_oft_backup2_cyc_oft,backup2_cyc_oft,HI_BBP_STU_BACKUP2_TIM_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP2_TIM_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_backup2_tim_oft_reserved_2,reserved_2,HI_BBP_STU_BACKUP2_TIM_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP2_TIM_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_backup2_tim_oft_backup2_ts_oft,backup2_ts_oft,HI_BBP_STU_BACKUP2_TIM_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP2_TIM_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_backup2_tim_oft_reserved_1,reserved_1,HI_BBP_STU_BACKUP2_TIM_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP2_TIM_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_backup2_tim_oft_backup2_sub_oft,backup2_sub_oft,HI_BBP_STU_BACKUP2_TIM_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP2_TIM_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_backup2_tim_oft_reserved_0,reserved_0,HI_BBP_STU_BACKUP2_TIM_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP2_TIM_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_backup2_sfn_rpt_backup2_sfn_rpt,backup2_sfn_rpt,HI_BBP_STU_BACKUP2_SFN_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP2_SFN_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_backup2_sfn_rpt_reserved,reserved,HI_BBP_STU_BACKUP2_SFN_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_BACKUP2_SFN_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_afn_rpt_afn_rpt,afn_rpt,HI_BBP_STU_AFN_RPT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_AFN_RPT_OFFSET)
HI_SET_GET(hi_bbp_stu_tim_lock_tim_lock,tim_lock,HI_BBP_STU_TIM_LOCK_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_TIM_LOCK_OFFSET)
HI_SET_GET(hi_bbp_stu_tim_lock_reserved,reserved,HI_BBP_STU_TIM_LOCK_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_TIM_LOCK_OFFSET)
HI_SET_GET(hi_bbp_stu_abs_init_abs_init_ts_pos,abs_init_ts_pos,HI_BBP_STU_ABS_INIT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ABS_INIT_OFFSET)
HI_SET_GET(hi_bbp_stu_abs_init_abs_init_ts_pos_en,abs_init_ts_pos_en,HI_BBP_STU_ABS_INIT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ABS_INIT_OFFSET)
HI_SET_GET(hi_bbp_stu_abs_init_reserved,reserved,HI_BBP_STU_ABS_INIT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ABS_INIT_OFFSET)
HI_SET_GET(hi_bbp_stu_abs_tim_adj_en_abs_tim_adj_en,abs_tim_adj_en,HI_BBP_STU_ABS_TIM_ADJ_EN_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ABS_TIM_ADJ_EN_OFFSET)
HI_SET_GET(hi_bbp_stu_abs_tim_adj_en_reserved,reserved,HI_BBP_STU_ABS_TIM_ADJ_EN_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ABS_TIM_ADJ_EN_OFFSET)
HI_SET_GET(hi_bbp_stu_abs_tim_adj_oft_abs_delta_ts,abs_delta_ts,HI_BBP_STU_ABS_TIM_ADJ_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ABS_TIM_ADJ_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_abs_tim_adj_oft_reserved_1,reserved_1,HI_BBP_STU_ABS_TIM_ADJ_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ABS_TIM_ADJ_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_abs_tim_adj_oft_abs_delta_subf,abs_delta_subf,HI_BBP_STU_ABS_TIM_ADJ_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ABS_TIM_ADJ_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_abs_tim_adj_oft_reserved_0,reserved_0,HI_BBP_STU_ABS_TIM_ADJ_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_ABS_TIM_ADJ_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_wireless_relay_cfg_relay_multi_sync_mode,relay_multi_sync_mode,HI_BBP_STU_WIRELESS_RELAY_CFG_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_WIRELESS_RELAY_CFG_OFFSET)
HI_SET_GET(hi_bbp_stu_wireless_relay_cfg_reserved_1,reserved_1,HI_BBP_STU_WIRELESS_RELAY_CFG_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_WIRELESS_RELAY_CFG_OFFSET)
HI_SET_GET(hi_bbp_stu_wireless_relay_cfg_relay_sync_interval,relay_sync_interval,HI_BBP_STU_WIRELESS_RELAY_CFG_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_WIRELESS_RELAY_CFG_OFFSET)
HI_SET_GET(hi_bbp_stu_wireless_relay_cfg_reserved_0,reserved_0,HI_BBP_STU_WIRELESS_RELAY_CFG_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_WIRELESS_RELAY_CFG_OFFSET)
HI_SET_GET(hi_bbp_stu_half_frame_cfg_cfg_subfrm_num0,cfg_subfrm_num0,HI_BBP_STU_HALF_FRAME_CFG_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_HALF_FRAME_CFG_OFFSET)
HI_SET_GET(hi_bbp_stu_half_frame_cfg_cfg_subfrm_num1,cfg_subfrm_num1,HI_BBP_STU_HALF_FRAME_CFG_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_HALF_FRAME_CFG_OFFSET)
HI_SET_GET(hi_bbp_stu_half_frame_cfg_reserved_1,reserved_1,HI_BBP_STU_HALF_FRAME_CFG_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_HALF_FRAME_CFG_OFFSET)
HI_SET_GET(hi_bbp_stu_half_frame_cfg_half_frame_mask,half_frame_mask,HI_BBP_STU_HALF_FRAME_CFG_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_HALF_FRAME_CFG_OFFSET)
HI_SET_GET(hi_bbp_stu_half_frame_cfg_reserved_0,reserved_0,HI_BBP_STU_HALF_FRAME_CFG_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_HALF_FRAME_CFG_OFFSET)
HI_SET_GET(hi_bbp_stu_dem_timer_from_fpu_dem_timer_from_fpu,dem_timer_from_fpu,HI_BBP_STU_DEM_TIMER_FROM_FPU_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_DEM_TIMER_FROM_FPU_OFFSET)
HI_SET_GET(hi_bbp_stu_dem_timer_from_fpu_reserved,reserved,HI_BBP_STU_DEM_TIMER_FROM_FPU_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_DEM_TIMER_FROM_FPU_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_line_out_ctrl_lte_frame_sync_ctrl,lte_frame_sync_ctrl,HI_BBP_STU_COEX_LINE_OUT_CTRL_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LINE_OUT_CTRL_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_line_out_ctrl_reserved_3,reserved_3,HI_BBP_STU_COEX_LINE_OUT_CTRL_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LINE_OUT_CTRL_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_line_out_ctrl_lte_rx_active_ctrl,lte_rx_active_ctrl,HI_BBP_STU_COEX_LINE_OUT_CTRL_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LINE_OUT_CTRL_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_line_out_ctrl_reserved_2,reserved_2,HI_BBP_STU_COEX_LINE_OUT_CTRL_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LINE_OUT_CTRL_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_line_out_ctrl_lte_tx_active_ctrl,lte_tx_active_ctrl,HI_BBP_STU_COEX_LINE_OUT_CTRL_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LINE_OUT_CTRL_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_line_out_ctrl_reserved_1,reserved_1,HI_BBP_STU_COEX_LINE_OUT_CTRL_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LINE_OUT_CTRL_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_line_out_ctrl_wlan_bt_ldo_en_ctrl,wlan_bt_ldo_en_ctrl,HI_BBP_STU_COEX_LINE_OUT_CTRL_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LINE_OUT_CTRL_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_line_out_ctrl_reserved_0,reserved_0,HI_BBP_STU_COEX_LINE_OUT_CTRL_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LINE_OUT_CTRL_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_line_in_ctrl_wlan_bt_priority_ctrl,wlan_bt_priority_ctrl,HI_BBP_STU_COEX_LINE_IN_CTRL_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LINE_IN_CTRL_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_line_in_ctrl_reserved_1,reserved_1,HI_BBP_STU_COEX_LINE_IN_CTRL_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LINE_IN_CTRL_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_line_in_ctrl_wlan_bt_tx_active_ctrl,wlan_bt_tx_active_ctrl,HI_BBP_STU_COEX_LINE_IN_CTRL_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LINE_IN_CTRL_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_line_in_ctrl_reserved_0,reserved_0,HI_BBP_STU_COEX_LINE_IN_CTRL_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LINE_IN_CTRL_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_timer_ahead_rx_coex_t_ahead_rx,coex_t_ahead_rx,HI_BBP_STU_COEX_TIMER_AHEAD_RX_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_TIMER_AHEAD_RX_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_timer_ahead_rx_reserved,reserved,HI_BBP_STU_COEX_TIMER_AHEAD_RX_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_TIMER_AHEAD_RX_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_timer_delay_rx_coex_t_delay_rx,coex_t_delay_rx,HI_BBP_STU_COEX_TIMER_DELAY_RX_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_TIMER_DELAY_RX_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_timer_delay_rx_reserved,reserved,HI_BBP_STU_COEX_TIMER_DELAY_RX_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_TIMER_DELAY_RX_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_frame_sync_pulsewidth_frame_sync_pulsewidth,frame_sync_pulsewidth,HI_BBP_STU_COEX_FRAME_SYNC_PULSEWIDTH_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_FRAME_SYNC_PULSEWIDTH_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_frame_sync_pulsewidth_reserved,reserved,HI_BBP_STU_COEX_FRAME_SYNC_PULSEWIDTH_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_FRAME_SYNC_PULSEWIDTH_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_lte_rx_tx_active_cmd_coex_lte_rx_active_cmd,coex_lte_rx_active_cmd,HI_BBP_STU_COEX_LTE_RX_TX_ACTIVE_CMD_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LTE_RX_TX_ACTIVE_CMD_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_lte_rx_tx_active_cmd_reserved_1,reserved_1,HI_BBP_STU_COEX_LTE_RX_TX_ACTIVE_CMD_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LTE_RX_TX_ACTIVE_CMD_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_lte_rx_tx_active_cmd_coex_lte_tx_active_cmd,coex_lte_tx_active_cmd,HI_BBP_STU_COEX_LTE_RX_TX_ACTIVE_CMD_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LTE_RX_TX_ACTIVE_CMD_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_lte_rx_tx_active_cmd_reserved_0,reserved_0,HI_BBP_STU_COEX_LTE_RX_TX_ACTIVE_CMD_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LTE_RX_TX_ACTIVE_CMD_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_wlan_bt_ld0_en_cmd_wlan_bt_ldo_en_cmd,wlan_bt_ldo_en_cmd,HI_BBP_STU_COEX_WLAN_BT_LD0_EN_CMD_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_WLAN_BT_LD0_EN_CMD_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_wlan_bt_ld0_en_cmd_reserved,reserved,HI_BBP_STU_COEX_WLAN_BT_LD0_EN_CMD_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_WLAN_BT_LD0_EN_CMD_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_line_in_state_wlan_bt_priority_state,wlan_bt_priority_state,HI_BBP_STU_COEX_LINE_IN_STATE_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LINE_IN_STATE_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_line_in_state_reserved_1,reserved_1,HI_BBP_STU_COEX_LINE_IN_STATE_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LINE_IN_STATE_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_line_in_state_wlan_bt_tx_active_state,wlan_bt_tx_active_state,HI_BBP_STU_COEX_LINE_IN_STATE_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LINE_IN_STATE_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_line_in_state_reserved_0,reserved_0,HI_BBP_STU_COEX_LINE_IN_STATE_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_LINE_IN_STATE_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_frame_sync_oft_coex_frame_sync_ts_oft,coex_frame_sync_ts_oft,HI_BBP_STU_COEX_FRAME_SYNC_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_FRAME_SYNC_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_frame_sync_oft_reserved_1,reserved_1,HI_BBP_STU_COEX_FRAME_SYNC_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_FRAME_SYNC_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_frame_sync_oft_coex_frame_sync_subfrm_oft,coex_frame_sync_subfrm_oft,HI_BBP_STU_COEX_FRAME_SYNC_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_FRAME_SYNC_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_frame_sync_oft_reserved_0,reserved_0,HI_BBP_STU_COEX_FRAME_SYNC_OFT_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_FRAME_SYNC_OFT_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_output_ind_wlan_bt_rx_active_ind,wlan_bt_rx_active_ind,HI_BBP_STU_COEX_OUTPUT_IND_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_OUTPUT_IND_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_output_ind_wlan_bt_lte_frame_sync_ind,wlan_bt_lte_frame_sync_ind,HI_BBP_STU_COEX_OUTPUT_IND_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_OUTPUT_IND_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_output_ind_reserved_1,reserved_1,HI_BBP_STU_COEX_OUTPUT_IND_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_OUTPUT_IND_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_output_ind_wlan_bt_tx_active_ind,wlan_bt_tx_active_ind,HI_BBP_STU_COEX_OUTPUT_IND_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_OUTPUT_IND_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_output_ind_reserved_0,reserved_0,HI_BBP_STU_COEX_OUTPUT_IND_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_OUTPUT_IND_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_timer_ahead_tx_coex_t_ahead_tx,coex_t_ahead_tx,HI_BBP_STU_COEX_TIMER_AHEAD_TX_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_TIMER_AHEAD_TX_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_timer_ahead_tx_reserved,reserved,HI_BBP_STU_COEX_TIMER_AHEAD_TX_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_TIMER_AHEAD_TX_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_timer_delay_coex_t_delay_tx,coex_t_delay_tx,HI_BBP_STU_COEX_TIMER_DELAY_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_TIMER_DELAY_OFFSET)
HI_SET_GET(hi_bbp_stu_coex_timer_delay_reserved,reserved,HI_BBP_STU_COEX_TIMER_DELAY_T,HI_BBP_STU_BASE_ADDR, HI_BBP_STU_COEX_TIMER_DELAY_OFFSET)

#endif

#endif // __HI_BBP_STU_H__

