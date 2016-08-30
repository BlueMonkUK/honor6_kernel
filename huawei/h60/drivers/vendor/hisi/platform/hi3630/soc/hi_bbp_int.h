/******************************************************************************/
/*  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_bbp_int.h */
/* Version       : 2.0 */
/* Author        : xxx*/
/* Created       : 2013-02-22*/
/* Last Modified : */
/* Description   :  The C union definition file for the module bbp_int*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Author        : xxx*/
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_BBP_INT_H__
#define __HI_BBP_INT_H__

/*
 * Project: hi
 * Module : bbp_int
 */

#ifndef HI_SET_GET
#define HI_SET_GET(a0,a1,a2,a3,a4)
#endif

/********************************************************************************/
/*    bbp_int �Ĵ���ƫ�ƶ��壨��Ŀ��_ģ����_�Ĵ�����_OFFSET)        */
/********************************************************************************/
#define    HI_BBP_INT_DSP_SYM_INT_MSK_OFFSET                 (0x0) /* �Ĵ����� */
#define    HI_BBP_INT_DSP_SYM_INT_CLS_OFFSET                 (0x4) /* �Ĵ����� */
#define    HI_BBP_INT_DSP_SYM_INT_TYP_OFFSET                 (0x8) /* �Ĵ����� */
#define    HI_BBP_INT_DSP_SYM_INT_ALM_OFFSET                 (0xC) /* �Ĵ����� */
#define    HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET                 (0x10) /* �Ĵ����� */
#define    HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET                 (0x14) /* �Ĵ����� */
#define    HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET                 (0x18) /* �Ĵ����� */
#define    HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET                 (0x1C) /* �Ĵ����� */
#define    HI_BBP_INT_WARN_INT_MSK_OFFSET                    (0x20) /* �Ĵ����� */
#define    HI_BBP_INT_WARN_INT_CLS_OFFSET                    (0x24) /* �Ĵ����� */
#define    HI_BBP_INT_WARN_INT_TYP_OFFSET                    (0x28) /* �Ĵ����� */
#define    HI_BBP_INT_ARM_INT_MSK_OFFSET                     (0x30) /* �Ĵ����� */
#define    HI_BBP_INT_ARM_INT_CLS_OFFSET                     (0x34) /* �Ĵ����� */
#define    HI_BBP_INT_ARM_INT_TYP_OFFSET                     (0x38) /* �Ĵ����� */
#define    HI_BBP_INT_ARM_INT_ALM_OFFSET                     (0x3C) /* �Ĵ����� */
#define    HI_BBP_INT_APP_ARM_INT_MSK_OFFSET                 (0x40) /* �Ĵ����� */
#define    HI_BBP_INT_APP_ARM_INT_CLS_OFFSET                 (0x44) /* �Ĵ����� */
#define    HI_BBP_INT_APP_ARM_INT_TYP_OFFSET                 (0x48) /* �Ĵ����� */
#define    HI_BBP_INT_PRIOR_LEVEL_OFFSET                     (0x50) /* �Ĵ����� */
#define    HI_BBP_INT_INT2OUT_BAK1_OFFSET                    (0x60) /* �Ĵ����� */
#define    HI_BBP_INT_INT2OUT_BAK2_OFFSET                    (0x64) /* �Ĵ����� */
#define    HI_BBP_INT_INT2OUT_BAK3_OFFSET                    (0x68) /* �Ĵ����� */
#define    HI_BBP_INT_INT2OUT_BAK4_OFFSET                    (0x6C) /* ·���ж�״̬�Ĵ����� */
#define    HI_BBP_INT_INT2OUT_BAK5_OFFSET                    (0x70) /* ·���ж����μĴ����� */
#define    HI_BBP_INT_INT2OUT_BAK6_OFFSET                    (0x74) /* �Ĵ����� */
#define    HI_BBP_INT_INT2OUT_BAK7_OFFSET                    (0x78) /* �Ĵ����� */
#define    HI_BBP_INT_INT2OUT_BAK8_OFFSET                    (0x7C) /* �Ĵ����� */
#define    HI_BBP_INT_CLK_EN_SEL_OFFSET                      (0x160) /* �Ĵ����� */
#define    HI_BBP_INT_AHB2LBUS_CLK_SEL_OFFSET                (0x164) /* �Ĵ����� */
#define    HI_BBP_INT_RFIN_SRST_EN_OFFSET                    (0x178) /* �Ĵ����� */
#define    HI_BBP_INT_FPU_SRST_EN_OFFSET                     (0x17C) /* �Ĵ����� */
#define    HI_BBP_INT_UL_SRST_EN_OFFSET                      (0x180) /* �Ĵ����� */
#define    HI_BBP_INT_VDL_SRST_EN_OFFSET                     (0x184) /* �Ĵ����� */
#define    HI_BBP_INT_PB_SRST_EN_OFFSET                      (0x188) /* �Ĵ����� */
#define    HI_BBP_INT_DBG_SRST_EN_OFFSET                     (0x18C) /* �Ĵ����� */
#define    HI_BBP_INT_AXIM_SRST_EN_OFFSET                    (0x190) /* �Ĵ����� */
#define    HI_BBP_INT_AXIS_SRST_EN_OFFSET                    (0x194) /* �Ĵ����� */
#define    HI_BBP_INT_AHBS_SRST_EN_OFFSET                    (0x198) /* �Ĵ����� */
#define    HI_BBP_INT_INT_SRST_EN_OFFSET                     (0x1A4) /* �Ĵ����� */
#define    HI_BBP_INT_STU_SRST_EN_OFFSET                     (0x1A8) /* �Ĵ����� */
#define    HI_BBP_INT_PDM_SRST_EN_OFFSET                     (0x1B4) /* �Ĵ����� */
#define    HI_BBP_INT_ATPRAM_CTRL_OFFSET                     (0x200) /* �Ĵ����� */


#ifndef __ASSEMBLY__

/********************************************************************************/
/*    bbp_int �Ĵ������壨��Ŀ��_ģ����_�Ĵ�����_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    dsp_sym_int_msk_0          : 1; /* [0..0] 0.5�����ж����Ρ�0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    dsp_sym_int_msk_1          : 1; /* [1..1] 1.5�����ж����Ρ�0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    dsp_sym_int_msk_2          : 1; /* [2..2] 2.5�����ж����Ρ�0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    dsp_sym_int_msk_3          : 1; /* [3..3] 3.5�����ж����Ρ�0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    dsp_sym_int_msk_4          : 1; /* [4..4] 4.5�����ж����Ρ�0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    dsp_sym_int_msk_5          : 1; /* [5..5] 5.5�����ж����Ρ�0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    dsp_sym_int_msk_6          : 1; /* [6..6] 6.5�����ж����Ρ�0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    dsp_sym_int_msk_7          : 1; /* [7..7] 7.5�����ж����Ρ�0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    dsp_sym_int_msk_8          : 1; /* [8..8] 8.5�����ж����Ρ�0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    dsp_sym_int_msk_9          : 1; /* [9..9] 9.5�����ж����Ρ�0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    dsp_sym_int_msk_10         : 1; /* [10..10] 10.5�����ж����Ρ�0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    dsp_sym_int_mask_11        : 1; /* [11..11] 11.5�����ж����Ρ�0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    dsp_sym_int_msk_12         : 1; /* [12..12] 12.5�����ж����Ρ�0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    dsp_sym_int_msk_13         : 1; /* [13..13] 13.5�����ж����Ρ�0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    Reserved                   : 18; /* [31..14] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_DSP_SYM_INT_MSK_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    dsp_sym_int_cls_0          : 1; /* [0..0] 0.5�����ж�����Ĵ�������˼Ĵ���λд1�����0.5�����ж� */
        unsigned int    dsp_sym_int_cls_1          : 1; /* [1..1] 1.5�����ж�����Ĵ�������˼Ĵ���λд1�����1.5�����ж� */
        unsigned int    dsp_sym_int_cls_2          : 1; /* [2..2] 2.5�����ж�����Ĵ�������˼Ĵ���λд1�����2.5�����ж� */
        unsigned int    dsp_sym_int_cls_3          : 1; /* [3..3] 3.5�����ж�����Ĵ�������˼Ĵ���λд1�����3.5�����ж� */
        unsigned int    dsp_sym_int_cls_4          : 1; /* [4..4] 4.5�����ж�����Ĵ�������˼Ĵ���λд1�����4.5�����ж� */
        unsigned int    dsp_sym_int_cls_5          : 1; /* [5..5] 5.5�����ж�����Ĵ�������˼Ĵ���λд1�����5.5�����ж� */
        unsigned int    dsp_sym_int_cls_6          : 1; /* [6..6] 6.5�����ж�����Ĵ�������˼Ĵ���λд1�����6.5�����ж� */
        unsigned int    dsp_sym_int_cls_7          : 1; /* [7..7] 7.5�����ж�����Ĵ�������˼Ĵ���λд1�����7.5�����ж� */
        unsigned int    dsp_sym_int_cls_8          : 1; /* [8..8] 8.5�����ж�����Ĵ�������˼Ĵ���λд1�����8.5�����ж� */
        unsigned int    dsp_sym_int_cls_9          : 1; /* [9..9] 9.5�����ж�����Ĵ�������˼Ĵ���λд1�����9.5�����ж� */
        unsigned int    dsp_sym_int_cls_10         : 1; /* [10..10] 10.5�����ж�����Ĵ�������˼Ĵ���λд1�����10.5�����ж� */
        unsigned int    dsp_sym_int_cls_11         : 1; /* [11..11] 11.5�����ж�����Ĵ�������˼Ĵ���λд1�����11.5�����ж� */
        unsigned int    dsp_sym_int_cls_12         : 1; /* [12..12] 12.5�����ж�����Ĵ�������˼Ĵ���λд1�����12.5�����ж� */
        unsigned int    dsp_sym_int_cls_13         : 1; /* [13..13] 13.5�����ж�����Ĵ�������˼Ĵ���λд1�����13.5�����ж� */
        unsigned int    Reserved                   : 18; /* [31..14] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_DSP_SYM_INT_CLS_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    dsp_sym_int_typ_0          : 1; /* [0..0] 0.5�����жϱ�־ָʾ��������Ĵ����Ķ�Ӧλд1�����жϱ�־ */
        unsigned int    dsp_sym_int_typ_1          : 1; /* [1..1] 1.5�����жϱ�־ָʾ��������Ĵ����Ķ�Ӧλд1�����жϱ�־ */
        unsigned int    dsp_sym_int_typ_2          : 1; /* [2..2] 2.5�����жϱ�־ָʾ��������Ĵ����Ķ�Ӧλд1�����жϱ�־ */
        unsigned int    dsp_sym_int_typ_3          : 1; /* [3..3] 3.5�����жϱ�־ָʾ��������Ĵ����Ķ�Ӧλд1�����жϱ�־ */
        unsigned int    dsp_sym_int_typ_4          : 1; /* [4..4] 4.5�����жϱ�־ָʾ��������Ĵ����Ķ�Ӧλд1�����жϱ�־ */
        unsigned int    dsp_sym_int_typ_5          : 1; /* [5..5] 5.5�����жϱ�־ָʾ��������Ĵ����Ķ�Ӧλд1�����жϱ�־ */
        unsigned int    dsp_sym_int_typ_6          : 1; /* [6..6] 6.5�����жϱ�־ָʾ��������Ĵ����Ķ�Ӧλд1�����жϱ�־ */
        unsigned int    dsp_sym_int_typ_7          : 1; /* [7..7] 7.5�����жϱ�־ָʾ��������Ĵ����Ķ�Ӧλд1�����жϱ�־ */
        unsigned int    dsp_sym_int_typ_8          : 1; /* [8..8] 8.5�����жϱ�־ָʾ��������Ĵ����Ķ�Ӧλд1�����жϱ�־ */
        unsigned int    dsp_sym_int_typ_9          : 1; /* [9..9] 9.5�����жϱ�־ָʾ��������Ĵ����Ķ�Ӧλд1�����жϱ�־ */
        unsigned int    dsp_sym_int_typ_10         : 1; /* [10..10] 10.5�����жϱ�־ָʾ��������Ĵ����Ķ�Ӧλд1�����жϱ�־ */
        unsigned int    dsp_sym_int_typ_11         : 1; /* [11..11] 11.5�����жϱ�־ָʾ��������Ĵ����Ķ�Ӧλд1�����жϱ�־ */
        unsigned int    dsp_sym_int_typ_12         : 1; /* [12..12] 12.5�����жϱ�־ָʾ��������Ĵ����Ķ�Ӧλд1�����жϱ�־ */
        unsigned int    dsp_sym_int_typ_13         : 1; /* [13..13] 13.5�����жϱ�־ָʾ��������Ĵ����Ķ�Ӧλд1�����жϱ�־ */
        unsigned int    Reserved                   : 18; /* [31..14] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_DSP_SYM_INT_TYP_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    dsp_sym_int_alm_0          : 1; /* [0..0] 0.5�����жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_sym_int_alm_1          : 1; /* [1..1] 1.5�����жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_sym_int_alm_2          : 1; /* [2..2] 2.5�����жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_sym_int_alm_3          : 1; /* [3..3] 3.5�����жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_sym_int_alm_4          : 1; /* [4..4] 4.5�����жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_sym_int_alm_5          : 1; /* [5..5] 5.5�����жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_sym_int_alm_6          : 1; /* [6..6] 6.5�����жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_sym_int_alm_7          : 1; /* [7..7] 7.5�����жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_sym_int_alm_8          : 1; /* [8..8] 8.5�����жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_sym_int_alm_9          : 1; /* [9..9] 9.5�����жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_sym_int_alm_10         : 1; /* [10..10] 10.5�����жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_sym_int_alm_11         : 1; /* [11..11] 11.5�����жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_sym_int_alm_12         : 1; /* [12..12] 12.5�����жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_sym_int_alm_13         : 1; /* [13..13] 13.5�����жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    Reserved                   : 18; /* [31..14] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_DSP_SYM_INT_ALM_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    dsp_cells_int_msk          : 1; /* [0..0] С�������ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_pwrm_int_msk           : 1; /* [1..1] ���������ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_cqi_int_msk            : 1; /* [2..2] cqi�ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_pb_int_msk             : 1; /* [3..3] pb�ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_pdu0_int_msk           : 1; /* [4..4] �ŵ������ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_pdu1_int_msk           : 1; /* [5..5] pds����ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_pdu2_int_msk           : 1; /* [6..6] pdc����ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_cmu_int_msk            : 1; /* [7..7] cmu�ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_vdl_int_msk            : 1; /* [8..8] vdl�ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_cfi_int_msk            : 1; /* [9..9] cfi�ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_dma_int_msk            : 1; /* [10..10] dma�ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_tdl_int_mask           : 1; /* [11..11] tdl�ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_dcf_int_msk            : 1; /* [12..12] dcf�ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_ul_int_msk             : 1; /* [13..13] ul�ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_fpu_int_msk            : 1; /* [14..14] fpu�ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_aagc_int_msk           : 1; /* [15..15] aagc�ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_rstd_int_msk           : 1; /* [16..16] rstd�ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_dma_anten0_int_msk     : 1; /* [17..17] dma_anten_0�ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_dma_anten1_int_msk     : 1; /* [18..18] dma_anten_1�ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_wlan_priority_int_msk  : 1; /* [19..19] wlan_bt_priority�ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    dsp_wlan_tx_active_int_msk : 1; /* [20..20] wlan_bt_tx_active�ж����Ρ�1��ʾ���ж��ϱ���0��ʾ�ر��ж��ϱ� */
        unsigned int    Reserved                   : 11; /* [31..21] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_DSP_MOD_INT_MSK_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    dsp_cells_int_cls          : 1; /* [0..0] С�������ж�����Ĵ�������˼Ĵ���λд1�����С�������ж� */
        unsigned int    dsp_pwrm_int_cls           : 1; /* [1..1] ���������ж�����Ĵ�������˼Ĵ���λд1��������������ж� */
        unsigned int    dsp_cqi_int_cls            : 1; /* [2..2] cqi�ж�����Ĵ�������˼Ĵ���λд1�����cqi�ж� */
        unsigned int    dsp_pb_int_cls             : 1; /* [3..3] pb�ж�����Ĵ�������˼Ĵ���λд1�����pb�ж� */
        unsigned int    dsp_pdu0_int_cls           : 1; /* [4..4] �ŵ������ж�����Ĵ�������˼Ĵ���λд1������ŵ������ж� */
        unsigned int    dsp_pdu1_int_cls           : 1; /* [5..5] pds����ж�����Ĵ�������˼Ĵ���λд1�����pds����ж� */
        unsigned int    dsp_pdu2_int_cls           : 1; /* [6..6] pdc����ж�����Ĵ�������˼Ĵ���λд1�����pdc����ж� */
        unsigned int    dsp_cmu_int_cls            : 1; /* [7..7] cmu�ж�����Ĵ�������˼Ĵ���λд1�����cmu�ж� */
        unsigned int    dsp_vdl_int_cls            : 1; /* [8..8] vdl�ж�����Ĵ�������˼Ĵ���λд1�����vdl�ж� */
        unsigned int    dsp_cfi_int_cls            : 1; /* [9..9] cfi�ж�����Ĵ�������˼Ĵ���λд1�����cfi�ж� */
        unsigned int    dsp_dma_int_cls            : 1; /* [10..10] dma�ж�����Ĵ�������˼Ĵ���λд1�����dma�ж� */
        unsigned int    dsp_tdl_int_cls            : 1; /* [11..11] tdl�ж�����Ĵ�������˼Ĵ���λд1�����tdl�ж� */
        unsigned int    dsp_dcf_int_cls            : 1; /* [12..12] dcf�ж�����Ĵ�������˼Ĵ���λд1�����dcf�ж� */
        unsigned int    dsp_ul_int_cls             : 1; /* [13..13] ul�ж�����Ĵ�������˼Ĵ���λд1�����ul�ж� */
        unsigned int    dsp_fpu_int_cls            : 1; /* [14..14] fpu�ж�����Ĵ�������˼Ĵ���λд1�����fpu�ж� */
        unsigned int    dsp_aagc_int_cls           : 1; /* [15..15] aagc�ж�����Ĵ�������˼Ĵ���λд1�����aagc�ж� */
        unsigned int    dsp_rstd_int_cls           : 1; /* [16..16] rstd�ж�����Ĵ�������˼Ĵ���λд1�����rstd�ж� */
        unsigned int    dsp_dma_anten0_int_cls     : 1; /* [17..17] dma_anten_0�ж�����Ĵ�������˼Ĵ���λд1�����dma_anten_0�ж� */
        unsigned int    dsp_dma_anten1_int_cls     : 1; /* [18..18] dma_anten_1�ж�����Ĵ�������˼Ĵ���λд1�����dma_anten_1�ж� */
        unsigned int    dsp_wlan_priority_int_cls  : 1; /* [19..19] wlan_bt_priority�ж�����Ĵ�������˼Ĵ���λд1�����wlan_bt_priority�ж� */
        unsigned int    dsp_wlan_tx_active_int_cls : 1; /* [20..20] wlan_bt_tx_active�ж�����Ĵ�������˼Ĵ���λд1�����wlan_bt_tx_active�ж� */
        unsigned int    Reserved                   : 11; /* [31..21] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_DSP_MOD_INT_CLS_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    dsp_cells_int_typ          : 1; /* [0..0] С�������жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_pwrm_int_typ           : 1; /* [1..1] ���������жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_cqi_int_typ            : 1; /* [2..2] cqi�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_pb_int_typ             : 1; /* [3..3] pb�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_pdu0_int_typ           : 1; /* [4..4] �ŵ������жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_pdu1_int_typ           : 1; /* [5..5] pds����жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_pdu2_int_typ           : 1; /* [6..6] pdc����жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_cmu_int_typ            : 1; /* [7..7] cmu�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_vdl_int_typ            : 1; /* [8..8] vdl�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_cfi_int_typ            : 1; /* [9..9] cfi�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_dma_int_typ            : 1; /* [10..10] dma�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_tdl_int_typ            : 1; /* [11..11] tdl�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_dcf_int_typ            : 1; /* [12..12] dcf�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_ul_int_typ             : 1; /* [13..13] ul�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_fpu_int_typ            : 1; /* [14..14] fpu�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_aagc_int_typ           : 1; /* [15..15] aagc�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_rstd_int_typ           : 1; /* [16..16] rstd�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_dma_anten0_int_typ     : 1; /* [17..17] dma_anten_0�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_dma_anten1_int_typ     : 1; /* [18..18] dma_anten_1�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_wlan_priority_int_typ  : 1; /* [19..19] wlan_bt_priority�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    dsp_wlan_tx_active_int_typ : 1; /* [20..20] wlan_bt_tx_active�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    Reserved                   : 11; /* [31..21] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_DSP_MOD_INT_TYP_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    dsp_cells_int_alm          : 1; /* [0..0] С�������жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_pwrm_int_alm           : 1; /* [1..1] ���������жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_cqi_int_alm            : 1; /* [2..2] cqi�жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_pb_int_alm             : 1; /* [3..3] pb�жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_pdu0_int_alm           : 1; /* [4..4] �ŵ������жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_pdu1_int_alm           : 1; /* [5..5] pds����жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_pdu2_int_alm           : 1; /* [6..6] pdc����жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_cmu_int_alm            : 1; /* [7..7] cmu�жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_vdl_int_alm            : 1; /* [8..8] vdl�жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_cfi_int_alm            : 1; /* [9..9] cfi�жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_dma_int_alm            : 1; /* [10..10] dma�жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_tdl_int_alm            : 1; /* [11..11] tdl�жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_dcf_int_alm            : 1; /* [12..12] dcf�жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_ul_int_alm             : 1; /* [13..13] ul�жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_fpu_int_alm            : 1; /* [14..14] fpu�жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_aagc_int_alm           : 1; /* [15..15] aagc�жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_rstd_int_alm           : 1; /* [16..16] rstd�жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_dma_anten0_int_alm     : 1; /* [17..17] dma_anten_0�жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_dma_anten1_int_alm     : 1; /* [18..18] dma_anten_1�жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_wlan_priority_int_alm  : 1; /* [19..19] wlan_bt_priority�жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dsp_wlan_tx_active_int_alm : 1; /* [20..20] wlan_bt_tx_active�жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    Reserved                   : 11; /* [31..21] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_DSP_MOD_INT_ALM_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    rfin_warn_int_msk          : 1; /* [0..0] rfin�澯�ж����μĴ�����0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    fpu_warn_int_msk           : 1; /* [1..1] fpu�澯�ж����μĴ�����0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    ul_warn_int_msk            : 1; /* [2..2] ul�澯�ж����μĴ�����0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    cqi_warn_int_msk           : 1; /* [3..3] cqi�澯�ж����μĴ�����0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    pb_warn_int_msk            : 1; /* [4..4] pb�澯�ж����μĴ�����0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    vdl_warn_int_msk           : 1; /* [5..5] vdl�澯�ж����μĴ�����0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    pdu_warn_int_msk           : 1; /* [6..6] pdu�澯�ж����μĴ�����0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    tdl_warn_int_msk           : 1; /* [7..7] tdl�澯�ж����μĴ�����0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    dma_warn_int_msk           : 1; /* [8..8] dma�澯�ж����μĴ�����0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    Reserved                   : 23; /* [31..9] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_WARN_INT_MSK_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    rfin_warn_int_cls          : 1; /* [0..0] rfin�澯�ж�����Ĵ�������˼Ĵ���λд1�����ul�ж� */
        unsigned int    fpu_warn_int_cls           : 1; /* [1..1] fpu�澯�ж�����Ĵ�������˼Ĵ���λд1�����ul�ж� */
        unsigned int    ul_warn_int_cls            : 1; /* [2..2] ul�澯�ж�����Ĵ�������˼Ĵ���λд1�����ul�ж� */
        unsigned int    cqi_warn_int_cls           : 1; /* [3..3] cqi�澯�ж�����Ĵ�������˼Ĵ���λд1�����ul�ж� */
        unsigned int    pb_warn_int_cls            : 1; /* [4..4] pb�澯�ж�����Ĵ�������˼Ĵ���λд1�����ul�ж� */
        unsigned int    vdl_warn_int_cls           : 1; /* [5..5] vdl�澯�ж�����Ĵ�������˼Ĵ���λд1�����ul�ж� */
        unsigned int    pdu_warn_int_cls           : 1; /* [6..6] pdu�澯�ж�����Ĵ�������˼Ĵ���λд1�����ul�ж� */
        unsigned int    tdl_warn_int_cls           : 1; /* [7..7] tdl�澯�ж�����Ĵ�������˼Ĵ���λд1�����ul�ж� */
        unsigned int    dma_warn_int_cls           : 1; /* [8..8] dma�澯�ж�����Ĵ�������˼Ĵ���λд1�����ul�ж� */
        unsigned int    Reserved                   : 23; /* [31..9] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_WARN_INT_CLS_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    rfin_warn_int_typ          : 1; /* [0..0] rfin�澯�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    fpu_warn_int_typ           : 1; /* [1..1] fpu�澯�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    ul_warn_int_typ            : 1; /* [2..2] ul�澯�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    cqi_warn_int_typ           : 1; /* [3..3] cqi�澯�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    pb_warn_int_typ            : 1; /* [4..4] pb�澯�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    vdl_warn_int_typ           : 1; /* [5..5] vdl�澯�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    pdu_warn_int_typ           : 1; /* [6..6] pdu�澯�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    tdl_warn_int_typ           : 1; /* [7..7] tdl�澯�жϼĴ�����ָʾ�ж�״̬���� */
        unsigned int    dma_warn_int_typ           : 1; /* [8..8] dma�澯�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    Reserved                   : 23; /* [31..9] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_WARN_INT_TYP_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    arm_position_int_msk       : 1; /* [0..0] arm��֡��ʱ�ж����μĴ�����0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    arm_dl_dma_int_msk         : 1; /* [1..1] arm��dl_dma�ж����μĴ�����0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    Reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_ARM_INT_MSK_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    arm_position_int_cls       : 1; /* [0..0] arm��֡��ʱ�ж�����Ĵ�������˼Ĵ���λд1�������֡��ʱ�ж� */
        unsigned int    arm_dl_dma_int_cls         : 1; /* [1..1] arm��dl_dma�ж�����Ĵ�������˼Ĵ���λд1�����dl_dma�ж� */
        unsigned int    Reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_ARM_INT_CLS_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    arm_position_int_typ       : 1; /* [0..0] arm��֡��ʱ�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    arm_dl_dma_int_typ         : 1; /* [1..1] arm��dl_dma�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    Reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_ARM_INT_TYP_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    arm_position_int_alm       : 1; /* [0..0] arm��֡��ʱ�жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    arm_dl_dma_int_alm         : 1; /* [1..1] arm��dl_dma�жϸ澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    Reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_ARM_INT_ALM_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    aarm_dbg_msk               : 1; /* [0..0] app arm��dbg�澯�ж����μĴ�����0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    aarm_dma_msk               : 1; /* [1..1] app arm��dma�澯�ж����μĴ�����0�������ж��ϱ���1��ʹ���ж��ϱ�����ֵΪ0 */
        unsigned int    Reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_APP_ARM_INT_MSK_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    aarm_dbg_cls               : 1; /* [0..0] app arm��dbg�澯�ж�����Ĵ�������˼Ĵ���λд1�����dbg�澯�ж� */
        unsigned int    aarm_dma_cls               : 1; /* [1..1] app arm��dma�澯�ж�����Ĵ�������˼Ĵ���λд1�����dbg�澯�ж� */
        unsigned int    Reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_APP_ARM_INT_CLS_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    aarm_dbg_typ               : 1; /* [0..0] app arm��dbg�澯�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    aarm_dma_typ               : 1; /* [1..1] app arm��dma�澯�жϼĴ�����ָʾ�ж�״̬��0�����жϣ�1�����ϱ��жϡ� */
        unsigned int    Reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_APP_ARM_INT_TYP_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    prior_level                : 2; /* [1..0] �������ȼ� */
        unsigned int    Reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_PRIOR_LEVEL_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    xs_rs_gated_en             : 1; /* [0..0] AXI Slave����ʱ���Զ��ſؼĴ���1���Զ��ſع��ܴ�0���Զ��ſع��ܹرգ������͹��� */
        unsigned int    hs_rs_gated_en             : 1; /* [1..1] AXI_Sync Slave����ʱ���Զ��ſؼĴ���1���Զ��ſع��ܴ�0���Զ��ſع��ܹرգ������͹��� */
        unsigned int    xm_gated_en                : 1; /* [2..2] AXI Master����ʱ���Զ��ſؼĴ���1���Զ��ſع��ܴ�0���Զ��ſع��ܹرգ������͹��� */
        unsigned int    xs_gs_gated_en             : 1; /* [3..3] AXI Slave����ʱ���Զ��ſؼĴ���1���Զ��ſع��ܴ�0���Զ��ſع��ܹرգ������͹��� */
        unsigned int    hs_gs_gated_en             : 1; /* [4..4] AXI_Sync Slave����ʱ���Զ��ſؼĴ���1���Զ��ſع��ܴ�0���Զ��ſع��ܹرգ������͹��� */
        unsigned int    int2out_bak1               : 11; /* [15..5] ���ԼĴ��� */
        unsigned int    Reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_INT2OUT_BAK1_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    int2out_bak2               : 16; /* [15..0] ���ԼĴ��� */
        unsigned int    Reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_INT2OUT_BAK2_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    int2out_bak3               : 16; /* [15..0] ���ԼĴ��� */
        unsigned int    Reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_INT2OUT_BAK3_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    dt0_int_typ                : 1; /* [0..0] ·���ж�0״̬�Ĵ�����0�����жϣ�1�����ϱ��жϡ�д1��0 */
        unsigned int    dt1_int_typ                : 1; /* [1..1] ·���ж�1״̬�Ĵ�����0�����жϣ�1�����ϱ��жϡ�д1��0 */
        unsigned int    dt2_int_typ                : 1; /* [2..2] ·���ж�2״̬�Ĵ�����0�����жϣ�1�����ϱ��жϡ�д1��0 */
        unsigned int    dt3_int_typ                : 1; /* [3..3] ·���ж�3״̬�Ĵ�����0�����жϣ�1�����ϱ��жϡ�д1��0 */
        unsigned int    dt4_int_typ                : 1; /* [4..4] ·���ж�4״̬�Ĵ�����0�����жϣ�1�����ϱ��жϡ�д1��0 */
        unsigned int    dt5_int_typ                : 1; /* [5..5] ·���ж�5״̬�Ĵ�����0�����жϣ�1�����ϱ��жϡ�д1��0 */
        unsigned int    dt6_int_typ                : 1; /* [6..6] ·���ж�6״̬�Ĵ�����0�����жϣ�1�����ϱ��жϡ�д1��0 */
        unsigned int    dt7_int_typ                : 1; /* [7..7] ·���ж�7״̬�Ĵ�����0�����жϣ�1�����ϱ��жϡ�д1��0 */
        unsigned int    dt0_int_alm                : 1; /* [8..8] ·���ж�0�澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dt1_int_alm                : 1; /* [9..9] ·���ж�1�澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dt2_int_alm                : 1; /* [10..10] ·���ж�2�澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dt3_int_alm                : 1; /* [11..11] ·���ж�3�澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dt4_int_alm                : 1; /* [12..12] ·���ж�4�澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dt5_int_alm                : 1; /* [13..13] ·���ж�5�澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dt6_int_alm                : 1; /* [14..14] ·���ж�6�澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    dt7_int_alm                : 1; /* [15..15] ·���ж�7�澯ָʾ�������жϴ�����ԭ�ж�û���������ʱ�����澯��д1��0�澯λ */
        unsigned int    Reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_INT2OUT_BAK4_T;    /* ·���ж�״̬�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    Reserved_1                 : 8; /* [7..0] ���� */
        unsigned int    dt_int_msk                 : 1; /* [8..8] ·���ж����μĴ��������Σ���·���жϡ�0��ʾ�ر��ж��ϱ���1��ʾ���ж��ϱ��� */
        unsigned int    Reserved_0                 : 23; /* [31..9] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_INT2OUT_BAK5_T;    /* ·���ж����μĴ����� */

typedef union
{
    struct
    {
        unsigned int    int2out_bak6               : 16; /* [15..0] ���ԼĴ��� */
        unsigned int    Reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_INT2OUT_BAK6_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    int2out_bak7               : 16; /* [15..0] ���ԼĴ��� */
        unsigned int    Reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_INT2OUT_BAK7_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    int2out_bak8               : 16; /* [15..0] ���ԼĴ��� */
        unsigned int    Reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_INT2OUT_BAK8_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    Reserved_4                 : 1; /* [0..0] ���� */
        unsigned int    Reserved_3                 : 1; /* [1..1] ���� */
        unsigned int    rfin_clk_sel               : 1; /* [2..2] rfinģ��ʱ��(rfin_clk)ʹ��ѡ��0����ʾ��ʹ��ģ��ʱ�ӣ�1����ʾʹ��ģ��ʱ�ӣ���ʼֵΪ0 */
        unsigned int    fpu_clk_sel                : 1; /* [3..3] fpuģ��ʱ��(fpu_clk)ʹ��ѡ��0����ʾ��ʹ��ģ��ʱ�ӣ�1����ʾʹ��ģ��ʱ�ӣ���ʼֵΪ0 */
        unsigned int    ul_clk_sel                 : 1; /* [4..4] ulģ��ʱ��(ul_clk)ʹ��ѡ��0����ʾ��ʹ��ģ��ʱ�ӣ�1����ʾʹ��ģ��ʱ�ӣ���ʼֵΪ0 */
        unsigned int    vdl_clk_sel                : 1; /* [5..5] vdlģ��ʱ��(vdl_clk)ʹ��ѡ��0����ʾ��ʹ��ģ��ʱ�ӣ�1����ʾʹ��ģ��ʱ�ӣ���ʼֵΪ0 */
        unsigned int    pb_clk_sel                 : 1; /* [6..6] pbģ��ʱ��(pb_clk)ʹ��ѡ��0����ʾ��ʹ��ģ��ʱ�ӣ�1����ʾʹ��ģ��ʱ�ӣ���ʼֵΪ0 */
        unsigned int    dbg_clk_sel                : 1; /* [7..7] dbgģ��ʱ��(dbg_clk)ʹ��ѡ��0����ʾ��ʹ��ģ��ʱ�ӣ�1����ʾʹ��ģ��ʱ�ӣ���ʼֵΪ0 */
        unsigned int    axim_clk_sel               : 1; /* [8..8] aximģ��ʱ��(axim_clk)ʹ��ѡ��0����ʾ��ʹ��ģ��ʱ�ӣ�1����ʾʹ��ģ��ʱ�ӣ���ʼֵΪ0 */
        unsigned int    axis_clk_sel               : 1; /* [9..9] axisģ��ʱ��(axis_clk)ʹ��ѡ��0����ʾ��ʹ��ģ��ʱ�ӣ�1����ʾʹ��ģ��ʱ�ӣ���ʼֵΪ0 */
        unsigned int    ahbs_clk_sel               : 1; /* [10..10] ahbsģ��ʱ��(ahbs_clk)ʹ��ѡ��0����ʾ��ʹ��ģ��ʱ�ӣ�1����ʾʹ��ģ��ʱ�ӣ���ʼֵΪ0 */
        unsigned int    Reserved_2                 : 1; /* [11..11] ���� */
        unsigned int    Reserved_1                 : 1; /* [12..12] ���� */
        unsigned int    pdm_clk_sel                : 1; /* [13..13] pdmģ��ʱ��(pdm_clk)ʹ��ѡ��0����ʾ��ʹ��ģ��ʱ�ӣ�1����ʾʹ��ģ��ʱ�ӣ���ʼֵΪ0 */
        unsigned int    Reserved_0                 : 18; /* [31..14] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_CLK_EN_SEL_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    ahb2lbus_clk_sel           : 1; /* [0..0] AHB2LBUSģ��RAMʱ���ſ�ʹ�ܡ���0��ʾʱ�ӳ�������1��ʾʱ�����Զ��ſأ���ʼֵΪ0 */
        unsigned int    Reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_AHB2LBUS_CLK_SEL_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    rfin_srst_en               : 1; /* [0..0] rfin��ģ�����λ�źš���λ��0��·�ͼĴ�������1ʹ�ܸ�λ����0�����λ����ʼֵΪ0����λ�ڼ��ģ��Ĵ����޷����� */
        unsigned int    Reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_RFIN_SRST_EN_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    fpu_srst_en                : 1; /* [0..0] fpu��ģ�����λ�źš���λ��0��·�ͼĴ�������1ʹ�ܸ�λ����0�����λ����ʼֵΪ0����λ�ڼ��ģ��Ĵ����޷����� */
        unsigned int    Reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_FPU_SRST_EN_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    ul_srst_en                 : 1; /* [0..0] ul��ģ�����λ�źš���λ��0��·�ͼĴ�������1ʹ�ܸ�λ����0�����λ����ʼֵΪ0����λ�ڼ��ģ��Ĵ����޷����� */
        unsigned int    Reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_UL_SRST_EN_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    vdl_srst_en                : 1; /* [0..0] vdl��ģ�����λ�źš���λ��0��·�ͼĴ�������1ʹ�ܸ�λ����0�����λ����ʼֵΪ0 */
        unsigned int    Reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_VDL_SRST_EN_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    pb_srst_en                 : 1; /* [0..0] pb��ģ�����λ�źš���λ��0��·�ͼĴ�������1ʹ�ܸ�λ����0�����λ����ʼֵΪ0 */
        unsigned int    Reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_PB_SRST_EN_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    dbg_srst_en                : 1; /* [0..0] dbg��ģ�����λ�źš���λ��0��·�ͼĴ�������1ʹ�ܸ�λ����0�����λ����ʼֵΪ0 */
        unsigned int    Reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_DBG_SRST_EN_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    axim_srst_en               : 1; /* [0..0] axim��ģ�����λ�źš���λ��0��·�ͼĴ�������1ʹ�ܸ�λ����0�����λ����ʼֵΪ0 */
        unsigned int    Reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_AXIM_SRST_EN_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    axis_srst_en               : 1; /* [0..0] axis��ģ�����λ�źš���λ��0��·�ͼĴ�������1ʹ�ܸ�λ����0�����λ����ʼֵΪ0����λ�ڼ��ģ��Ĵ����޷����� */
        unsigned int    Reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_AXIS_SRST_EN_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    ahbs_srst_en               : 1; /* [0..0] ahbs��ģ�����λ�źš���λ��0��·�ͼĴ�������1ʹ�ܸ�λ����0�����λ����ʼֵΪ0����λ�ڼ��ģ��Ĵ����޷����� */
        unsigned int    Reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_AHBS_SRST_EN_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    int_srst_en                : 1; /* [0..0] int��ģ�����λ�źš���λ��0��·�ͼĴ�������1ʹ�ܸ�λ����0�����λ����ʼֵΪ0����λ�ڼ��ģ��Ĵ����޷����� */
        unsigned int    Reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_INT_SRST_EN_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    stu_srst_en                : 1; /* [0..0] stu��ģ�����λ�źš���λ��0��·�ͼĴ�������1ʹ�ܸ�λ����0�����λ����ʼֵΪ0����λ�ڼ��ģ��Ĵ����޷����� */
        unsigned int    Reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_STU_SRST_EN_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    pdm_srst_en                : 1; /* [0..0] pdm��ģ�����λ�źš���λ��0��·�ͼĴ�������1ʹ�ܸ�λ����0�����λ����ʼֵΪ0����λ�ڼ��ģ��Ĵ����޷����� */
        unsigned int    Reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_PDM_SRST_EN_T;    /* �Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    atpram_ctrl                : 16; /* [15..0] bbp_dma��bus2bbpģ���atpram_ctrl�Ĵ��� */
        unsigned int    Reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_INT_ATPRAM_CTRL_T;    /* �Ĵ����� */


/********************************************************************************/
/*    bbp_int ��������Ŀ��_ģ����_�Ĵ�����_��Ա��_set)        */
/********************************************************************************/
HI_SET_GET(hi_bbp_int_dsp_sym_int_msk_dsp_sym_int_msk_0,dsp_sym_int_msk_0,HI_BBP_INT_DSP_SYM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_msk_dsp_sym_int_msk_1,dsp_sym_int_msk_1,HI_BBP_INT_DSP_SYM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_msk_dsp_sym_int_msk_2,dsp_sym_int_msk_2,HI_BBP_INT_DSP_SYM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_msk_dsp_sym_int_msk_3,dsp_sym_int_msk_3,HI_BBP_INT_DSP_SYM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_msk_dsp_sym_int_msk_4,dsp_sym_int_msk_4,HI_BBP_INT_DSP_SYM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_msk_dsp_sym_int_msk_5,dsp_sym_int_msk_5,HI_BBP_INT_DSP_SYM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_msk_dsp_sym_int_msk_6,dsp_sym_int_msk_6,HI_BBP_INT_DSP_SYM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_msk_dsp_sym_int_msk_7,dsp_sym_int_msk_7,HI_BBP_INT_DSP_SYM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_msk_dsp_sym_int_msk_8,dsp_sym_int_msk_8,HI_BBP_INT_DSP_SYM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_msk_dsp_sym_int_msk_9,dsp_sym_int_msk_9,HI_BBP_INT_DSP_SYM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_msk_dsp_sym_int_msk_10,dsp_sym_int_msk_10,HI_BBP_INT_DSP_SYM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_msk_dsp_sym_int_mask_11,dsp_sym_int_mask_11,HI_BBP_INT_DSP_SYM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_msk_dsp_sym_int_msk_12,dsp_sym_int_msk_12,HI_BBP_INT_DSP_SYM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_msk_dsp_sym_int_msk_13,dsp_sym_int_msk_13,HI_BBP_INT_DSP_SYM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_msk_reserved,Reserved,HI_BBP_INT_DSP_SYM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_cls_dsp_sym_int_cls_0,dsp_sym_int_cls_0,HI_BBP_INT_DSP_SYM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_cls_dsp_sym_int_cls_1,dsp_sym_int_cls_1,HI_BBP_INT_DSP_SYM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_cls_dsp_sym_int_cls_2,dsp_sym_int_cls_2,HI_BBP_INT_DSP_SYM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_cls_dsp_sym_int_cls_3,dsp_sym_int_cls_3,HI_BBP_INT_DSP_SYM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_cls_dsp_sym_int_cls_4,dsp_sym_int_cls_4,HI_BBP_INT_DSP_SYM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_cls_dsp_sym_int_cls_5,dsp_sym_int_cls_5,HI_BBP_INT_DSP_SYM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_cls_dsp_sym_int_cls_6,dsp_sym_int_cls_6,HI_BBP_INT_DSP_SYM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_cls_dsp_sym_int_cls_7,dsp_sym_int_cls_7,HI_BBP_INT_DSP_SYM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_cls_dsp_sym_int_cls_8,dsp_sym_int_cls_8,HI_BBP_INT_DSP_SYM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_cls_dsp_sym_int_cls_9,dsp_sym_int_cls_9,HI_BBP_INT_DSP_SYM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_cls_dsp_sym_int_cls_10,dsp_sym_int_cls_10,HI_BBP_INT_DSP_SYM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_cls_dsp_sym_int_cls_11,dsp_sym_int_cls_11,HI_BBP_INT_DSP_SYM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_cls_dsp_sym_int_cls_12,dsp_sym_int_cls_12,HI_BBP_INT_DSP_SYM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_cls_dsp_sym_int_cls_13,dsp_sym_int_cls_13,HI_BBP_INT_DSP_SYM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_cls_reserved,Reserved,HI_BBP_INT_DSP_SYM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_typ_dsp_sym_int_typ_0,dsp_sym_int_typ_0,HI_BBP_INT_DSP_SYM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_typ_dsp_sym_int_typ_1,dsp_sym_int_typ_1,HI_BBP_INT_DSP_SYM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_typ_dsp_sym_int_typ_2,dsp_sym_int_typ_2,HI_BBP_INT_DSP_SYM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_typ_dsp_sym_int_typ_3,dsp_sym_int_typ_3,HI_BBP_INT_DSP_SYM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_typ_dsp_sym_int_typ_4,dsp_sym_int_typ_4,HI_BBP_INT_DSP_SYM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_typ_dsp_sym_int_typ_5,dsp_sym_int_typ_5,HI_BBP_INT_DSP_SYM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_typ_dsp_sym_int_typ_6,dsp_sym_int_typ_6,HI_BBP_INT_DSP_SYM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_typ_dsp_sym_int_typ_7,dsp_sym_int_typ_7,HI_BBP_INT_DSP_SYM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_typ_dsp_sym_int_typ_8,dsp_sym_int_typ_8,HI_BBP_INT_DSP_SYM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_typ_dsp_sym_int_typ_9,dsp_sym_int_typ_9,HI_BBP_INT_DSP_SYM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_typ_dsp_sym_int_typ_10,dsp_sym_int_typ_10,HI_BBP_INT_DSP_SYM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_typ_dsp_sym_int_typ_11,dsp_sym_int_typ_11,HI_BBP_INT_DSP_SYM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_typ_dsp_sym_int_typ_12,dsp_sym_int_typ_12,HI_BBP_INT_DSP_SYM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_typ_dsp_sym_int_typ_13,dsp_sym_int_typ_13,HI_BBP_INT_DSP_SYM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_typ_reserved,Reserved,HI_BBP_INT_DSP_SYM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_alm_dsp_sym_int_alm_0,dsp_sym_int_alm_0,HI_BBP_INT_DSP_SYM_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_alm_dsp_sym_int_alm_1,dsp_sym_int_alm_1,HI_BBP_INT_DSP_SYM_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_alm_dsp_sym_int_alm_2,dsp_sym_int_alm_2,HI_BBP_INT_DSP_SYM_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_alm_dsp_sym_int_alm_3,dsp_sym_int_alm_3,HI_BBP_INT_DSP_SYM_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_alm_dsp_sym_int_alm_4,dsp_sym_int_alm_4,HI_BBP_INT_DSP_SYM_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_alm_dsp_sym_int_alm_5,dsp_sym_int_alm_5,HI_BBP_INT_DSP_SYM_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_alm_dsp_sym_int_alm_6,dsp_sym_int_alm_6,HI_BBP_INT_DSP_SYM_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_alm_dsp_sym_int_alm_7,dsp_sym_int_alm_7,HI_BBP_INT_DSP_SYM_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_alm_dsp_sym_int_alm_8,dsp_sym_int_alm_8,HI_BBP_INT_DSP_SYM_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_alm_dsp_sym_int_alm_9,dsp_sym_int_alm_9,HI_BBP_INT_DSP_SYM_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_alm_dsp_sym_int_alm_10,dsp_sym_int_alm_10,HI_BBP_INT_DSP_SYM_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_alm_dsp_sym_int_alm_11,dsp_sym_int_alm_11,HI_BBP_INT_DSP_SYM_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_alm_dsp_sym_int_alm_12,dsp_sym_int_alm_12,HI_BBP_INT_DSP_SYM_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_alm_dsp_sym_int_alm_13,dsp_sym_int_alm_13,HI_BBP_INT_DSP_SYM_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_sym_int_alm_reserved,Reserved,HI_BBP_INT_DSP_SYM_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_SYM_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_cells_int_msk,dsp_cells_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_pwrm_int_msk,dsp_pwrm_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_cqi_int_msk,dsp_cqi_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_pb_int_msk,dsp_pb_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_pdu0_int_msk,dsp_pdu0_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_pdu1_int_msk,dsp_pdu1_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_pdu2_int_msk,dsp_pdu2_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_cmu_int_msk,dsp_cmu_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_vdl_int_msk,dsp_vdl_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_cfi_int_msk,dsp_cfi_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_dma_int_msk,dsp_dma_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_tdl_int_mask,dsp_tdl_int_mask,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_dcf_int_msk,dsp_dcf_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_ul_int_msk,dsp_ul_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_fpu_int_msk,dsp_fpu_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_aagc_int_msk,dsp_aagc_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_rstd_int_msk,dsp_rstd_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_dma_anten0_int_msk,dsp_dma_anten0_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_dma_anten1_int_msk,dsp_dma_anten1_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_wlan_priority_int_msk,dsp_wlan_priority_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_dsp_wlan_tx_active_int_msk,dsp_wlan_tx_active_int_msk,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_msk_reserved,Reserved,HI_BBP_INT_DSP_MOD_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_cells_int_cls,dsp_cells_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_pwrm_int_cls,dsp_pwrm_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_cqi_int_cls,dsp_cqi_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_pb_int_cls,dsp_pb_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_pdu0_int_cls,dsp_pdu0_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_pdu1_int_cls,dsp_pdu1_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_pdu2_int_cls,dsp_pdu2_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_cmu_int_cls,dsp_cmu_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_vdl_int_cls,dsp_vdl_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_cfi_int_cls,dsp_cfi_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_dma_int_cls,dsp_dma_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_tdl_int_cls,dsp_tdl_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_dcf_int_cls,dsp_dcf_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_ul_int_cls,dsp_ul_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_fpu_int_cls,dsp_fpu_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_aagc_int_cls,dsp_aagc_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_rstd_int_cls,dsp_rstd_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_dma_anten0_int_cls,dsp_dma_anten0_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_dma_anten1_int_cls,dsp_dma_anten1_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_wlan_priority_int_cls,dsp_wlan_priority_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_dsp_wlan_tx_active_int_cls,dsp_wlan_tx_active_int_cls,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_cls_reserved,Reserved,HI_BBP_INT_DSP_MOD_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_cells_int_typ,dsp_cells_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_pwrm_int_typ,dsp_pwrm_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_cqi_int_typ,dsp_cqi_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_pb_int_typ,dsp_pb_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_pdu0_int_typ,dsp_pdu0_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_pdu1_int_typ,dsp_pdu1_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_pdu2_int_typ,dsp_pdu2_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_cmu_int_typ,dsp_cmu_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_vdl_int_typ,dsp_vdl_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_cfi_int_typ,dsp_cfi_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_dma_int_typ,dsp_dma_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_tdl_int_typ,dsp_tdl_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_dcf_int_typ,dsp_dcf_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_ul_int_typ,dsp_ul_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_fpu_int_typ,dsp_fpu_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_aagc_int_typ,dsp_aagc_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_rstd_int_typ,dsp_rstd_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_dma_anten0_int_typ,dsp_dma_anten0_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_dma_anten1_int_typ,dsp_dma_anten1_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_wlan_priority_int_typ,dsp_wlan_priority_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_dsp_wlan_tx_active_int_typ,dsp_wlan_tx_active_int_typ,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_typ_reserved,Reserved,HI_BBP_INT_DSP_MOD_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_cells_int_alm,dsp_cells_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_pwrm_int_alm,dsp_pwrm_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_cqi_int_alm,dsp_cqi_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_pb_int_alm,dsp_pb_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_pdu0_int_alm,dsp_pdu0_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_pdu1_int_alm,dsp_pdu1_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_pdu2_int_alm,dsp_pdu2_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_cmu_int_alm,dsp_cmu_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_vdl_int_alm,dsp_vdl_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_cfi_int_alm,dsp_cfi_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_dma_int_alm,dsp_dma_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_tdl_int_alm,dsp_tdl_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_dcf_int_alm,dsp_dcf_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_ul_int_alm,dsp_ul_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_fpu_int_alm,dsp_fpu_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_aagc_int_alm,dsp_aagc_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_rstd_int_alm,dsp_rstd_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_dma_anten0_int_alm,dsp_dma_anten0_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_dma_anten1_int_alm,dsp_dma_anten1_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_wlan_priority_int_alm,dsp_wlan_priority_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_dsp_wlan_tx_active_int_alm,dsp_wlan_tx_active_int_alm,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_dsp_mod_int_alm_reserved,Reserved,HI_BBP_INT_DSP_MOD_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DSP_MOD_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_msk_rfin_warn_int_msk,rfin_warn_int_msk,HI_BBP_INT_WARN_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_msk_fpu_warn_int_msk,fpu_warn_int_msk,HI_BBP_INT_WARN_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_msk_ul_warn_int_msk,ul_warn_int_msk,HI_BBP_INT_WARN_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_msk_cqi_warn_int_msk,cqi_warn_int_msk,HI_BBP_INT_WARN_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_msk_pb_warn_int_msk,pb_warn_int_msk,HI_BBP_INT_WARN_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_msk_vdl_warn_int_msk,vdl_warn_int_msk,HI_BBP_INT_WARN_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_msk_pdu_warn_int_msk,pdu_warn_int_msk,HI_BBP_INT_WARN_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_msk_tdl_warn_int_msk,tdl_warn_int_msk,HI_BBP_INT_WARN_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_msk_dma_warn_int_msk,dma_warn_int_msk,HI_BBP_INT_WARN_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_msk_reserved,Reserved,HI_BBP_INT_WARN_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_cls_rfin_warn_int_cls,rfin_warn_int_cls,HI_BBP_INT_WARN_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_cls_fpu_warn_int_cls,fpu_warn_int_cls,HI_BBP_INT_WARN_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_cls_ul_warn_int_cls,ul_warn_int_cls,HI_BBP_INT_WARN_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_cls_cqi_warn_int_cls,cqi_warn_int_cls,HI_BBP_INT_WARN_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_cls_pb_warn_int_cls,pb_warn_int_cls,HI_BBP_INT_WARN_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_cls_vdl_warn_int_cls,vdl_warn_int_cls,HI_BBP_INT_WARN_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_cls_pdu_warn_int_cls,pdu_warn_int_cls,HI_BBP_INT_WARN_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_cls_tdl_warn_int_cls,tdl_warn_int_cls,HI_BBP_INT_WARN_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_cls_dma_warn_int_cls,dma_warn_int_cls,HI_BBP_INT_WARN_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_cls_reserved,Reserved,HI_BBP_INT_WARN_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_typ_rfin_warn_int_typ,rfin_warn_int_typ,HI_BBP_INT_WARN_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_typ_fpu_warn_int_typ,fpu_warn_int_typ,HI_BBP_INT_WARN_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_typ_ul_warn_int_typ,ul_warn_int_typ,HI_BBP_INT_WARN_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_typ_cqi_warn_int_typ,cqi_warn_int_typ,HI_BBP_INT_WARN_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_typ_pb_warn_int_typ,pb_warn_int_typ,HI_BBP_INT_WARN_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_typ_vdl_warn_int_typ,vdl_warn_int_typ,HI_BBP_INT_WARN_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_typ_pdu_warn_int_typ,pdu_warn_int_typ,HI_BBP_INT_WARN_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_typ_tdl_warn_int_typ,tdl_warn_int_typ,HI_BBP_INT_WARN_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_typ_dma_warn_int_typ,dma_warn_int_typ,HI_BBP_INT_WARN_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_warn_int_typ_reserved,Reserved,HI_BBP_INT_WARN_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_WARN_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_arm_int_msk_arm_position_int_msk,arm_position_int_msk,HI_BBP_INT_ARM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_ARM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_arm_int_msk_arm_dl_dma_int_msk,arm_dl_dma_int_msk,HI_BBP_INT_ARM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_ARM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_arm_int_msk_reserved,Reserved,HI_BBP_INT_ARM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_ARM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_arm_int_cls_arm_position_int_cls,arm_position_int_cls,HI_BBP_INT_ARM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_ARM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_arm_int_cls_arm_dl_dma_int_cls,arm_dl_dma_int_cls,HI_BBP_INT_ARM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_ARM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_arm_int_cls_reserved,Reserved,HI_BBP_INT_ARM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_ARM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_arm_int_typ_arm_position_int_typ,arm_position_int_typ,HI_BBP_INT_ARM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_ARM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_arm_int_typ_arm_dl_dma_int_typ,arm_dl_dma_int_typ,HI_BBP_INT_ARM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_ARM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_arm_int_typ_reserved,Reserved,HI_BBP_INT_ARM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_ARM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_arm_int_alm_arm_position_int_alm,arm_position_int_alm,HI_BBP_INT_ARM_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_ARM_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_arm_int_alm_arm_dl_dma_int_alm,arm_dl_dma_int_alm,HI_BBP_INT_ARM_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_ARM_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_arm_int_alm_reserved,Reserved,HI_BBP_INT_ARM_INT_ALM_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_ARM_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_int_app_arm_int_msk_aarm_dbg_msk,aarm_dbg_msk,HI_BBP_INT_APP_ARM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_APP_ARM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_app_arm_int_msk_aarm_dma_msk,aarm_dma_msk,HI_BBP_INT_APP_ARM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_APP_ARM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_app_arm_int_msk_reserved,Reserved,HI_BBP_INT_APP_ARM_INT_MSK_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_APP_ARM_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_int_app_arm_int_cls_aarm_dbg_cls,aarm_dbg_cls,HI_BBP_INT_APP_ARM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_APP_ARM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_app_arm_int_cls_aarm_dma_cls,aarm_dma_cls,HI_BBP_INT_APP_ARM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_APP_ARM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_app_arm_int_cls_reserved,Reserved,HI_BBP_INT_APP_ARM_INT_CLS_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_APP_ARM_INT_CLS_OFFSET)
HI_SET_GET(hi_bbp_int_app_arm_int_typ_aarm_dbg_typ,aarm_dbg_typ,HI_BBP_INT_APP_ARM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_APP_ARM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_app_arm_int_typ_aarm_dma_typ,aarm_dma_typ,HI_BBP_INT_APP_ARM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_APP_ARM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_app_arm_int_typ_reserved,Reserved,HI_BBP_INT_APP_ARM_INT_TYP_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_APP_ARM_INT_TYP_OFFSET)
HI_SET_GET(hi_bbp_int_prior_level_prior_level,prior_level,HI_BBP_INT_PRIOR_LEVEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_PRIOR_LEVEL_OFFSET)
HI_SET_GET(hi_bbp_int_prior_level_reserved,Reserved,HI_BBP_INT_PRIOR_LEVEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_PRIOR_LEVEL_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak1_xs_rs_gated_en,xs_rs_gated_en,HI_BBP_INT_INT2OUT_BAK1_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK1_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak1_hs_rs_gated_en,hs_rs_gated_en,HI_BBP_INT_INT2OUT_BAK1_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK1_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak1_xm_gated_en,xm_gated_en,HI_BBP_INT_INT2OUT_BAK1_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK1_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak1_xs_gs_gated_en,xs_gs_gated_en,HI_BBP_INT_INT2OUT_BAK1_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK1_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak1_hs_gs_gated_en,hs_gs_gated_en,HI_BBP_INT_INT2OUT_BAK1_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK1_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak1_int2out_bak1,int2out_bak1,HI_BBP_INT_INT2OUT_BAK1_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK1_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak1_reserved,Reserved,HI_BBP_INT_INT2OUT_BAK1_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK1_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak2_int2out_bak2,int2out_bak2,HI_BBP_INT_INT2OUT_BAK2_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK2_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak2_reserved,Reserved,HI_BBP_INT_INT2OUT_BAK2_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK2_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak3_int2out_bak3,int2out_bak3,HI_BBP_INT_INT2OUT_BAK3_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK3_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak3_reserved,Reserved,HI_BBP_INT_INT2OUT_BAK3_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK3_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak4_dt0_int_typ,dt0_int_typ,HI_BBP_INT_INT2OUT_BAK4_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK4_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak4_dt1_int_typ,dt1_int_typ,HI_BBP_INT_INT2OUT_BAK4_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK4_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak4_dt2_int_typ,dt2_int_typ,HI_BBP_INT_INT2OUT_BAK4_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK4_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak4_dt3_int_typ,dt3_int_typ,HI_BBP_INT_INT2OUT_BAK4_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK4_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak4_dt4_int_typ,dt4_int_typ,HI_BBP_INT_INT2OUT_BAK4_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK4_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak4_dt5_int_typ,dt5_int_typ,HI_BBP_INT_INT2OUT_BAK4_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK4_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak4_dt6_int_typ,dt6_int_typ,HI_BBP_INT_INT2OUT_BAK4_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK4_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak4_dt7_int_typ,dt7_int_typ,HI_BBP_INT_INT2OUT_BAK4_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK4_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak4_dt0_int_alm,dt0_int_alm,HI_BBP_INT_INT2OUT_BAK4_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK4_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak4_dt1_int_alm,dt1_int_alm,HI_BBP_INT_INT2OUT_BAK4_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK4_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak4_dt2_int_alm,dt2_int_alm,HI_BBP_INT_INT2OUT_BAK4_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK4_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak4_dt3_int_alm,dt3_int_alm,HI_BBP_INT_INT2OUT_BAK4_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK4_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak4_dt4_int_alm,dt4_int_alm,HI_BBP_INT_INT2OUT_BAK4_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK4_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak4_dt5_int_alm,dt5_int_alm,HI_BBP_INT_INT2OUT_BAK4_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK4_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak4_dt6_int_alm,dt6_int_alm,HI_BBP_INT_INT2OUT_BAK4_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK4_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak4_dt7_int_alm,dt7_int_alm,HI_BBP_INT_INT2OUT_BAK4_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK4_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak4_reserved,Reserved,HI_BBP_INT_INT2OUT_BAK4_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK4_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak5_reserved_1,Reserved_1,HI_BBP_INT_INT2OUT_BAK5_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK5_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak5_dt_int_msk,dt_int_msk,HI_BBP_INT_INT2OUT_BAK5_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK5_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak5_reserved_0,Reserved_0,HI_BBP_INT_INT2OUT_BAK5_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK5_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak6_int2out_bak6,int2out_bak6,HI_BBP_INT_INT2OUT_BAK6_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK6_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak6_reserved,Reserved,HI_BBP_INT_INT2OUT_BAK6_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK6_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak7_int2out_bak7,int2out_bak7,HI_BBP_INT_INT2OUT_BAK7_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK7_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak7_reserved,Reserved,HI_BBP_INT_INT2OUT_BAK7_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK7_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak8_int2out_bak8,int2out_bak8,HI_BBP_INT_INT2OUT_BAK8_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK8_OFFSET)
HI_SET_GET(hi_bbp_int_int2out_bak8_reserved,Reserved,HI_BBP_INT_INT2OUT_BAK8_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT2OUT_BAK8_OFFSET)
HI_SET_GET(hi_bbp_int_clk_en_sel_reserved_4,Reserved_4,HI_BBP_INT_CLK_EN_SEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_CLK_EN_SEL_OFFSET)
HI_SET_GET(hi_bbp_int_clk_en_sel_reserved_3,Reserved_3,HI_BBP_INT_CLK_EN_SEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_CLK_EN_SEL_OFFSET)
HI_SET_GET(hi_bbp_int_clk_en_sel_rfin_clk_sel,rfin_clk_sel,HI_BBP_INT_CLK_EN_SEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_CLK_EN_SEL_OFFSET)
HI_SET_GET(hi_bbp_int_clk_en_sel_fpu_clk_sel,fpu_clk_sel,HI_BBP_INT_CLK_EN_SEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_CLK_EN_SEL_OFFSET)
HI_SET_GET(hi_bbp_int_clk_en_sel_ul_clk_sel,ul_clk_sel,HI_BBP_INT_CLK_EN_SEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_CLK_EN_SEL_OFFSET)
HI_SET_GET(hi_bbp_int_clk_en_sel_vdl_clk_sel,vdl_clk_sel,HI_BBP_INT_CLK_EN_SEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_CLK_EN_SEL_OFFSET)
HI_SET_GET(hi_bbp_int_clk_en_sel_pb_clk_sel,pb_clk_sel,HI_BBP_INT_CLK_EN_SEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_CLK_EN_SEL_OFFSET)
HI_SET_GET(hi_bbp_int_clk_en_sel_dbg_clk_sel,dbg_clk_sel,HI_BBP_INT_CLK_EN_SEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_CLK_EN_SEL_OFFSET)
HI_SET_GET(hi_bbp_int_clk_en_sel_axim_clk_sel,axim_clk_sel,HI_BBP_INT_CLK_EN_SEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_CLK_EN_SEL_OFFSET)
HI_SET_GET(hi_bbp_int_clk_en_sel_axis_clk_sel,axis_clk_sel,HI_BBP_INT_CLK_EN_SEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_CLK_EN_SEL_OFFSET)
HI_SET_GET(hi_bbp_int_clk_en_sel_ahbs_clk_sel,ahbs_clk_sel,HI_BBP_INT_CLK_EN_SEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_CLK_EN_SEL_OFFSET)
HI_SET_GET(hi_bbp_int_clk_en_sel_reserved_2,Reserved_2,HI_BBP_INT_CLK_EN_SEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_CLK_EN_SEL_OFFSET)
HI_SET_GET(hi_bbp_int_clk_en_sel_reserved_1,Reserved_1,HI_BBP_INT_CLK_EN_SEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_CLK_EN_SEL_OFFSET)
HI_SET_GET(hi_bbp_int_clk_en_sel_pdm_clk_sel,pdm_clk_sel,HI_BBP_INT_CLK_EN_SEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_CLK_EN_SEL_OFFSET)
HI_SET_GET(hi_bbp_int_clk_en_sel_reserved_0,Reserved_0,HI_BBP_INT_CLK_EN_SEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_CLK_EN_SEL_OFFSET)
HI_SET_GET(hi_bbp_int_ahb2lbus_clk_sel_ahb2lbus_clk_sel,ahb2lbus_clk_sel,HI_BBP_INT_AHB2LBUS_CLK_SEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_AHB2LBUS_CLK_SEL_OFFSET)
HI_SET_GET(hi_bbp_int_ahb2lbus_clk_sel_reserved,Reserved,HI_BBP_INT_AHB2LBUS_CLK_SEL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_AHB2LBUS_CLK_SEL_OFFSET)
HI_SET_GET(hi_bbp_int_rfin_srst_en_rfin_srst_en,rfin_srst_en,HI_BBP_INT_RFIN_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_RFIN_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_rfin_srst_en_reserved,Reserved,HI_BBP_INT_RFIN_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_RFIN_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_fpu_srst_en_fpu_srst_en,fpu_srst_en,HI_BBP_INT_FPU_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_FPU_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_fpu_srst_en_reserved,Reserved,HI_BBP_INT_FPU_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_FPU_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_ul_srst_en_ul_srst_en,ul_srst_en,HI_BBP_INT_UL_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_UL_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_ul_srst_en_reserved,Reserved,HI_BBP_INT_UL_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_UL_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_vdl_srst_en_vdl_srst_en,vdl_srst_en,HI_BBP_INT_VDL_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_VDL_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_vdl_srst_en_reserved,Reserved,HI_BBP_INT_VDL_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_VDL_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_pb_srst_en_pb_srst_en,pb_srst_en,HI_BBP_INT_PB_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_PB_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_pb_srst_en_reserved,Reserved,HI_BBP_INT_PB_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_PB_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_dbg_srst_en_dbg_srst_en,dbg_srst_en,HI_BBP_INT_DBG_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DBG_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_dbg_srst_en_reserved,Reserved,HI_BBP_INT_DBG_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_DBG_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_axim_srst_en_axim_srst_en,axim_srst_en,HI_BBP_INT_AXIM_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_AXIM_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_axim_srst_en_reserved,Reserved,HI_BBP_INT_AXIM_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_AXIM_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_axis_srst_en_axis_srst_en,axis_srst_en,HI_BBP_INT_AXIS_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_AXIS_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_axis_srst_en_reserved,Reserved,HI_BBP_INT_AXIS_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_AXIS_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_ahbs_srst_en_ahbs_srst_en,ahbs_srst_en,HI_BBP_INT_AHBS_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_AHBS_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_ahbs_srst_en_reserved,Reserved,HI_BBP_INT_AHBS_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_AHBS_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_int_srst_en_int_srst_en,int_srst_en,HI_BBP_INT_INT_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_int_srst_en_reserved,Reserved,HI_BBP_INT_INT_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_INT_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_stu_srst_en_stu_srst_en,stu_srst_en,HI_BBP_INT_STU_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_STU_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_stu_srst_en_reserved,Reserved,HI_BBP_INT_STU_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_STU_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_pdm_srst_en_pdm_srst_en,pdm_srst_en,HI_BBP_INT_PDM_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_PDM_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_pdm_srst_en_reserved,Reserved,HI_BBP_INT_PDM_SRST_EN_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_PDM_SRST_EN_OFFSET)
HI_SET_GET(hi_bbp_int_atpram_ctrl_atpram_ctrl,atpram_ctrl,HI_BBP_INT_ATPRAM_CTRL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_ATPRAM_CTRL_OFFSET)
HI_SET_GET(hi_bbp_int_atpram_ctrl_reserved,Reserved,HI_BBP_INT_ATPRAM_CTRL_T,HI_BBP_INT_BASE_ADDR, HI_BBP_INT_ATPRAM_CTRL_OFFSET)

#endif

#endif // __HI_BBP_INT_H__

