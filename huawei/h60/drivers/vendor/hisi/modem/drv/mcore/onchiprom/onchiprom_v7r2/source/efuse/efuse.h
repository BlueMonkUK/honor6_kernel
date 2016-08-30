/*************************************************************************
*   ��Ȩ����(C) 1987-2020, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  efuse.h
*
*   ��    �� :  wuzechun
*
*   ��    �� :  efuse.c ��ͷ�ļ�
*
*   �޸ļ�¼ :  2011��6��9��  v1.00  wuzechun  ����
*
*************************************************************************/

#ifndef __EFUSE_H__
#define __EFUSE_H__

#ifdef __cplusplus
extern "C" {
#endif


/***************************** efuse register *****************************/

#define EFUSE_BASE_REG  0x90021000

#define HI_APB_CLK      (25000000)
#define EFUSE_COUNT_CFG (5)
#define PGM_COUNT_CFG   (HI_APB_CLK / 1000000 * 12 - EFUSE_COUNT_CFG)

#define EFUSEC_CFG       (EFUSE_BASE_REG+0x0)
#define EFUSEC_PG_EN     (1<<0)
#define EFUSEC_PRE_PG    (1<<1)
#define EFUSEC_RD_EN     (1<<2)
#define EFUSEC_APB     (1<<3)
#define EFUSEC_SIG_SEL_INNER     (1<<3)

#define EFUSEC_STATUS   (EFUSE_BASE_REG+0x04)
#define EFUSEC_PG_STATUS (1<<0)
#define EFUSEC_RD_STATUS (1<<1)
#define EFUSEC_PRE_PG_FIN (1<<2)

#define EFUSE_GROUP    (EFUSE_BASE_REG+0x08)
#define EFUSE_GP_MASK   ((1<<7)-1)

#define EFUSE_PG_VALUE    (EFUSE_BASE_REG+0x0C)

#define EFUSEC_COUNT    (EFUSE_BASE_REG+0x10)
#define EFUSE_COUNT_MASK   ((1<<8)-1)

#define EFUSEC_PGM_COUNT    (EFUSE_BASE_REG+0x14)
#define EFUSE_PGM_COUNT_MASK   ((1<<16)-1)

#define EFUSEC_DATA     (EFUSE_BASE_REG+0x18)

/* EFUSEδ��дʱĬ��ֵΪ0 */
#define EFUSEC_HW_CFG       (EFUSE_BASE_REG+0x1C)
/* ��д���� */
#define EFUSEC_PG_CTRL_POS      0
#define EFUSEC_PG_CTRL_WIDTH    1
#define EFUSEC_PG_CTRL_MASK     (((1<<EFUSEC_PG_CTRL_WIDTH)-1)<<EFUSEC_PG_CTRL_POS)
#define EFUSEC_PG_CTRL_DIS      (1<<EFUSEC_PG_CTRL_POS)
/* ��ȫУ��ʹ�ܱ�־ */
#define EFUSEC_SEC_EN_POS       4
#define EFUSEC_SEC_EN_WIDTH     1
#define EFUSEC_SEC_EN_MASK      (((1<<EFUSEC_SEC_EN_WIDTH)-1)<<EFUSEC_SEC_EN_POS)
#define EFUSEC_SEC_EN           (1<<EFUSEC_SEC_EN_POS)
/* JTAG���ʿ��� */
#define EFUSEC_JTAG_CTRL_POS     2
#define EFUSEC_JTAG_CTRL_WIDTH   1
#define EFUSEC_JTAG_CTRL_MASK    (((1<<EFUSEC_JTAG_CTRL_WIDTH)-1)<<EFUSEC_JTAG_CTRL_POS)
#define EFUSEC_JTAG_CTRL_EN      (0<<EFUSEC_JTAG_CTRL_POS)


#define EFUSE_MAX_SIZE  64     /* Efuse��С */
#define EFUSE_HAD_WRITED 1      /*Efuse �Ѿ���д*/
#define EFUSE_NOT_WRITED 0      /*Efuse û����д*/

int efuseRead(UINT32 group, UINT32* pBuf, UINT32 num);
int efuseWrite(UINT32 group, UINT32* pBuf, UINT32 num);
int efuseWriteCheck(UINT32* value, UINT32 num);

#ifdef __cplusplus
}
#endif

#endif /* efuse.h */

