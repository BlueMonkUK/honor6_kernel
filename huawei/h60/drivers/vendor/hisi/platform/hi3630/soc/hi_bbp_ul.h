/******************************************************************************/
/*  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_bbp_ul.h */
/* Version       : 2.0 */
/* Created       : 2013-10-28*/
/* Last Modified : */
/* Description   :  The C union definition file for the module bbp_ul*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_BBP_UL_H__
#define __HI_BBP_UL_H__

/*
 * Project: hi
 * Module : bbp_ul
 */

#ifndef HI_SET_GET
#define HI_SET_GET(a0,a1,a2,a3,a4)
#endif

/********************************************************************************/
/*    bbp_ul �Ĵ���ƫ�ƶ��壨��Ŀ��_ģ����_�Ĵ�����_OFFSET)        */
/********************************************************************************/
#define    HI_BBP_UL_UL_TA_REG_OFFSET                        (0x250) /* ����TA�Ĵ��� */

/********************************************************************************/
/*    bbp_ul �Ĵ������壨��Ŀ��_ģ����_�Ĵ�����_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    nta_value                  : 16; /* [15..0] ������֡ͷ���ϵͳ��֡ͷ����ǰ����NTA+NTA_offset����λTs����ʱ�̣�ͬ����Чʱ�̣�ͬ�� */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_BBP_UL_UL_TA_REG_T;    /* ����TA�Ĵ��� */


/********************************************************************************/
/*    bbp_ul ��������Ŀ��_ģ����_�Ĵ�����_��Ա��_set)        */
/********************************************************************************/
HI_SET_GET(hi_bbp_ul_ul_ta_reg_nta_value,nta_value,HI_BBP_UL_UL_TA_REG_T,HI_BBP_UL_BASE_ADDR, HI_BBP_UL_UL_TA_REG_OFFSET)
HI_SET_GET(hi_bbp_ul_ul_ta_reg_reserved,reserved,HI_BBP_UL_UL_TA_REG_T,HI_BBP_UL_BASE_ADDR, HI_BBP_UL_UL_TA_REG_OFFSET)

#endif // __HI_BBP_UL_H__

