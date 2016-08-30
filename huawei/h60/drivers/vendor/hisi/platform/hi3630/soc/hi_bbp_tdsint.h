/******************************************************************************/
/*  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_bbp_tdsint.h */
/* Version       : 2.0 */
/* Author        : xxx*/
/* Created       : 2013-04-09*/
/* Last Modified : */
/* Description   :  The C union definition file for the module bbp_tdsint*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Author        : xxx*/
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_BBP_TDSINT_H__
#define __HI_BBP_TDSINT_H__

/*
 * Project: hi
 * Module : bbp_tdsint
 */

#ifndef HI_SET_GET
#define HI_SET_GET(a0,a1,a2,a3,a4)
#endif

/********************************************************************************/
/*    bbp_tdsint �Ĵ���ƫ�ƶ��壨��Ŀ��_ģ����_�Ĵ�����_OFFSET)        */
/********************************************************************************/
#define    HI_BBP_TDSINT_PUB_INT_MSK_OFFSET                  (0xA0) /* ��֡�ж����μĴ��� */
#define    HI_BBP_TDSINT_DSP_INT_MSK_OFFSET                  (0xA4) /* DSPʱ϶�ж����μĴ��� */
#define    HI_BBP_TDSINT_PUB_INT_CLEAR_OFFSET                (0xA8) /* ��֡�ж�����Ĵ��� */
#define    HI_BBP_TDSINT_DSP_INT_CLEAR_OFFSET                (0xAC) /* DSPʱ϶�ж�����Ĵ��� */
#define    HI_BBP_TDSINT_PUB_INT_ALM_OFFSET                  (0xB0) /* ��֡�жϸ澯�Ĵ��� */
#define    HI_BBP_TDSINT_DSP_INT_ALM_OFFSET                  (0xB4) /* DSPʱ϶�жϸ澯�Ĵ��� */


#ifndef __ASSEMBLY__

/********************************************************************************/
/*    bbp_tdsint �Ĵ������壨��Ŀ��_ģ����_�Ĵ�����_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    hifi_sfrm_int_msk          : 1; /* [0..0] hifi��֡�ж����Σ�1'b1ʱ��STUģ�������֡�жϣ����ϱ�����ʱ�̣�����ʱ����Чʱ�̣�������Ч */
        unsigned int    arm_sfrm_int_msk           : 1; /* [1..1] arm��֡�ж����Σ�1'b1ʱ��STUģ�������֡�жϣ����ϱ�����ʱ�̣�����ʱ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSINT_PUB_INT_MSK_T;    /* ��֡�ж����μĴ��� */

typedef union
{
    struct
    {
        unsigned int    slot_int_msk               : 23; /* [22..0] DSPʱ϶�ж����Σ�1'b1ʱ����ʱ϶�жϣ�bit22:bit21���ֱ�Ϊ����ʱ϶��2/3��1/3�ж�bit20:bit0����ͨʱ϶0~6���ж����Σ�����ÿ����3-bitΪһ�飬��7�飬bitλ�ɵ͵��߷ֱ��ʾ����ʱ϶��1/4��1/2��3/�ж�����λ������ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    dsp_sfrm_int_msk           : 1; /* [23..23] DSP��֡�ж����Σ�1'b1ʱ��STUģ�������֡�жϣ����ϱ�����ʱ�̣�����ʱ����Чʱ�̣�������Ч */
        unsigned int    reserved                   : 8; /* [31..24] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSINT_DSP_INT_MSK_T;    /* DSPʱ϶�ж����μĴ��� */

typedef union
{
    struct
    {
        unsigned int    hifi_sfrm_int_clear        : 1; /* [0..0] hifi��֡�ж�����źţ�1'b1�������֡�жϣ�����ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    arm_sfrm_int_clear         : 1; /* [1..1] arm��֡�ж�����źţ�1'b1�������֡�жϣ�����ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSINT_PUB_INT_CLEAR_T;    /* ��֡�ж�����Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    slot_int_clear             : 23; /* [22..0] DSPʱ϶�ж������1'b1ʱ�����ʱ϶�жϣ�bit22:bit21���ֱ�Ϊ����ʱ϶��2/3��1/3�ж����bit20:bit0����ͨʱ϶0~6���ж����������ÿ����3-bitΪһ�飬��7�飬bitλ�ɵ͵��߷ֱ��ʾ����ʱ϶��1/4��1/2��3/�ж����λ������ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    dsp_sfrm_int_clear         : 1; /* [23..23] DSP��֡�ж�����źţ�1'b1�������֡�жϣ�����ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    reserved                   : 8; /* [31..24] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSINT_DSP_INT_CLEAR_T;    /* DSPʱ϶�ж�����Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    hifi_sfrm_int_alm          : 1; /* [0..0] hifi��֡�жϸ澯�Ĵ����������жϵ�����ԭ�����жϻ�δ�����ʱ�������澯�źţ��ø�λΪ1'b1�����λд��1'b1������澯����ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    arm_sfrm_int_alm           : 1; /* [1..1] arm��֡�жϸ澯�Ĵ����������жϵ�����ԭ�����жϻ�δ�����ʱ�������澯�źţ��ø�λΪ1'b1�����λд��1'b1������澯����ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    reserved                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSINT_PUB_INT_ALM_T;    /* ��֡�жϸ澯�Ĵ��� */

typedef union
{
    struct
    {
        unsigned int    slot_int_alm               : 23; /* [22..0] ʱ϶�жϸ澯�Ĵ����������жϵ�����֮ǰ���жϻ�δ�����ʱ�������澯���ø�λΪ1'b1�����λд��1'b1������澯λ��bit22:bit21���ֱ�Ϊ����ʱ϶��2/3��1/3�и澯bit20:bit0����ͨʱ϶0~6���жϸ澯������ÿ����3-bitΪһ�飬��7�飬bitλ�ɵ͵��߷ֱ��ʾ����ʱ϶��1/4��1/2��3/�жϸ澯λ������ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    dsp_sfrm_int_alm           : 1; /* [23..23] DSP��֡�жϸ澯�Ĵ����������жϵ�����ԭ�����жϻ�δ�����ʱ�������澯�źţ��ø�λΪ1'b1�����λд��1'b1������澯����ʱ�̣�����ʱ����Чʱ�̣����� */
        unsigned int    reserved                   : 8; /* [31..24] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_TDSINT_DSP_INT_ALM_T;    /* DSPʱ϶�жϸ澯�Ĵ��� */


/********************************************************************************/
/*    bbp_tdsint ��������Ŀ��_ģ����_�Ĵ�����_��Ա��_set)        */
/********************************************************************************/
HI_SET_GET(hi_bbp_tdsint_pub_int_msk_hifi_sfrm_int_msk,hifi_sfrm_int_msk,HI_BBP_TDSINT_PUB_INT_MSK_T,HI_BBP_TDSINT_BASE_ADDR, HI_BBP_TDSINT_PUB_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_tdsint_pub_int_msk_arm_sfrm_int_msk,arm_sfrm_int_msk,HI_BBP_TDSINT_PUB_INT_MSK_T,HI_BBP_TDSINT_BASE_ADDR, HI_BBP_TDSINT_PUB_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_tdsint_pub_int_msk_reserved,reserved,HI_BBP_TDSINT_PUB_INT_MSK_T,HI_BBP_TDSINT_BASE_ADDR, HI_BBP_TDSINT_PUB_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_tdsint_dsp_int_msk_slot_int_msk,slot_int_msk,HI_BBP_TDSINT_DSP_INT_MSK_T,HI_BBP_TDSINT_BASE_ADDR, HI_BBP_TDSINT_DSP_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_tdsint_dsp_int_msk_dsp_sfrm_int_msk,dsp_sfrm_int_msk,HI_BBP_TDSINT_DSP_INT_MSK_T,HI_BBP_TDSINT_BASE_ADDR, HI_BBP_TDSINT_DSP_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_tdsint_dsp_int_msk_reserved,reserved,HI_BBP_TDSINT_DSP_INT_MSK_T,HI_BBP_TDSINT_BASE_ADDR, HI_BBP_TDSINT_DSP_INT_MSK_OFFSET)
HI_SET_GET(hi_bbp_tdsint_pub_int_clear_hifi_sfrm_int_clear,hifi_sfrm_int_clear,HI_BBP_TDSINT_PUB_INT_CLEAR_T,HI_BBP_TDSINT_BASE_ADDR, HI_BBP_TDSINT_PUB_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_tdsint_pub_int_clear_arm_sfrm_int_clear,arm_sfrm_int_clear,HI_BBP_TDSINT_PUB_INT_CLEAR_T,HI_BBP_TDSINT_BASE_ADDR, HI_BBP_TDSINT_PUB_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_tdsint_pub_int_clear_reserved,reserved,HI_BBP_TDSINT_PUB_INT_CLEAR_T,HI_BBP_TDSINT_BASE_ADDR, HI_BBP_TDSINT_PUB_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_tdsint_dsp_int_clear_slot_int_clear,slot_int_clear,HI_BBP_TDSINT_DSP_INT_CLEAR_T,HI_BBP_TDSINT_BASE_ADDR, HI_BBP_TDSINT_DSP_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_tdsint_dsp_int_clear_dsp_sfrm_int_clear,dsp_sfrm_int_clear,HI_BBP_TDSINT_DSP_INT_CLEAR_T,HI_BBP_TDSINT_BASE_ADDR, HI_BBP_TDSINT_DSP_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_tdsint_dsp_int_clear_reserved,reserved,HI_BBP_TDSINT_DSP_INT_CLEAR_T,HI_BBP_TDSINT_BASE_ADDR, HI_BBP_TDSINT_DSP_INT_CLEAR_OFFSET)
HI_SET_GET(hi_bbp_tdsint_pub_int_alm_hifi_sfrm_int_alm,hifi_sfrm_int_alm,HI_BBP_TDSINT_PUB_INT_ALM_T,HI_BBP_TDSINT_BASE_ADDR, HI_BBP_TDSINT_PUB_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_tdsint_pub_int_alm_arm_sfrm_int_alm,arm_sfrm_int_alm,HI_BBP_TDSINT_PUB_INT_ALM_T,HI_BBP_TDSINT_BASE_ADDR, HI_BBP_TDSINT_PUB_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_tdsint_pub_int_alm_reserved,reserved,HI_BBP_TDSINT_PUB_INT_ALM_T,HI_BBP_TDSINT_BASE_ADDR, HI_BBP_TDSINT_PUB_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_tdsint_dsp_int_alm_slot_int_alm,slot_int_alm,HI_BBP_TDSINT_DSP_INT_ALM_T,HI_BBP_TDSINT_BASE_ADDR, HI_BBP_TDSINT_DSP_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_tdsint_dsp_int_alm_dsp_sfrm_int_alm,dsp_sfrm_int_alm,HI_BBP_TDSINT_DSP_INT_ALM_T,HI_BBP_TDSINT_BASE_ADDR, HI_BBP_TDSINT_DSP_INT_ALM_OFFSET)
HI_SET_GET(hi_bbp_tdsint_dsp_int_alm_reserved,reserved,HI_BBP_TDSINT_DSP_INT_ALM_T,HI_BBP_TDSINT_BASE_ADDR, HI_BBP_TDSINT_DSP_INT_ALM_OFFSET)

#endif

#endif // __HI_BBP_TDSINT_H__

