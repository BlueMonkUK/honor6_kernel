/******************************************************************************/
/*  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_efuse.h */
/* Version       : 2.0 */
/* Created       : 2013-11-27*/
/* Last Modified : */
/* Description   :  The C union definition file for the module efuse*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_EFUSE_H__
#define __HI_EFUSE_H__

/*
 * Project: hi
 * Module : efuse
 */

#include "hi_base.h"
#define HI_EFUSE_BASE_ADDR	0x90021000
//#define HI_EFUSE_BASE_ADDR	0xA0000000

/********************************************************************************/
/*    efuse �Ĵ���ƫ�ƶ���(��Ŀ��_ģ����_�Ĵ�����_OFFSET)        */
/********************************************************************************/
#define    HI_HW_CFG_OFFSET                                  (0x1C) /* ���efuse�ϵ�⸴λ���group==15��ȡ�����ݼĴ��������������жϵȡ���ʼֵ���û�ͨ��efuse��дȷ�� */

/********************************************************************************/
/*    efuse �Ĵ�������(��Ŀ��_ģ����_�Ĵ�����_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    pgm_disable                : 1; /* [0..0] 0��64ҳ1��128ҳ */
        unsigned int    pad_disable                : 1; /* [1..1] 0��64ҳ1��128ҳ */
        unsigned int    jtag_en                    : 1; /* [2..2] 0��64ҳ1��128ҳ */
        unsigned int    boot_sel                   : 1; /* [3..3] 0��64ҳ1��128ҳ */
        unsigned int    secboot_en                 : 1; /* [4..4] 0��64ҳ1��128ҳ */
        unsigned int    nf_ctrl_ena0               : 1; /* [5..5] ���Ƶ�һ��nand��Ϣ�ӹܽŻ���efuse�ж�ȡ0��nandc�ӹܽŶ�ȡnand flash���ò���1��nandc��efuse�ж�ȡnand falsh���ò��� */
        unsigned int    nf_block_size1             : 1; /* [6..6] 0��64ҳ1��128ҳ */
        unsigned int    nf_ecc_type1               : 2; /* [8..7] 00����ECC01��1bitECC/4bitECC11��8bitECC10��24bitECC */
        unsigned int    nf_page_size1              : 2; /* [10..9] 00��page size 512�ֽ�01��page size 2K�ֽ�11��page size 4K�ֽ�10��page size 8K�ֽ� */
        unsigned int    nf_addr_num1               : 1; /* [11..11] 0��4���ڵ�ַ1��5���ڵ�ַ */
        unsigned int    nf_ctrl_ena1               : 1; /* [12..12] ���Ƶڶ���nand��Ϣ�ӹܽŻ���efuse�ж�ȡ0��nandc�ӹܽŶ�ȡnand flash���ò���1��nandc��efuse�ж�ȡnand falsh���ò��� */
        unsigned int    reserved_2                 : 3; /* [15..13]  */
        unsigned int    nf_block_size              : 1; /* [16..16] 0��64ҳ1��128ҳ */
        unsigned int    nf_ecc_type                : 2; /* [18..17] 00����ECC01��1bitECC/4bitECC11��8bitECC10��24bitECC */
        unsigned int    reserved_1                 : 1; /* [19..19]  */
        unsigned int    nf_page_size               : 2; /* [21..20] 00��page size 512�ֽ�01��page size 2K�ֽ�11��page size 4K�ֽ�10��page size 8K�ֽ� */
        unsigned int    nf_addr_num                : 1; /* [22..22] 0��4���ڵ�ַ1��5���ڵ�ַ */
        unsigned int    reserved_0                 : 2; /* [24..23] 0��nandc�ӹܽŶ�ȡnand flash���ò���1��nandc��efuse�ж�ȡnand falsh���ò��� */
        unsigned int    nf_ctrl_ena                : 1; /* [25..25] V721�в�ʹ��0��nandc�ӹܽŶ�ȡnand flash���ò���1��nandc��efuse�ж�ȡnand falsh���ò��� */
        unsigned int    undefined                  : 6; /* [31..26]  */
    } bits;
    unsigned int    u32;
}HI_HW_CFG_T;    /* ���efuse�ϵ�⸴λ���group==15��ȡ�����ݼĴ��������������жϵȡ���ʼֵ���û�ͨ��efuse��дȷ�� */


/********************************************************************************/
/*    efuse ����(��Ŀ��_ģ����_�Ĵ�����_��Ա��_set)        */
/********************************************************************************/
HI_SET_GET(hi_hw_cfg_pgm_disable,pgm_disable,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_pad_disable,pad_disable,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_jtag_en,jtag_en,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_boot_sel,boot_sel,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_secboot_en,secboot_en,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_ctrl_ena0,nf_ctrl_ena0,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_block_size1,nf_block_size1,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_ecc_type1,nf_ecc_type1,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_page_size1,nf_page_size1,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_addr_num1,nf_addr_num1,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_ctrl_ena1,nf_ctrl_ena1,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_reserved_2,reserved_2,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_block_size,nf_block_size,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_ecc_type,nf_ecc_type,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_reserved_1,reserved_1,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_page_size,nf_page_size,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_addr_num,nf_addr_num,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_reserved_0,reserved_0,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_nf_ctrl_ena,nf_ctrl_ena,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
HI_SET_GET(hi_hw_cfg_undefined,undefined,HI_HW_CFG_T,HI_EFUSE_BASE_ADDR, HI_HW_CFG_OFFSET)
#endif // __HI_EFUSE_H__

