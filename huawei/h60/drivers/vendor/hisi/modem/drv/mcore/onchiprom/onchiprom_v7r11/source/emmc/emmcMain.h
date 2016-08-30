/*************************************************************************
*   ��Ȩ����(C) 1987-2004, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  emmcMain.h
*
*   ��    �� :  wangzhongshun
*
*   ��    �� :  emmc�Ծٵ������̿���ͷ�ļ�
*
*   �޸ļ�¼ :  2011��6��15��  v1.00  wangzhongshun  ����
*
*************************************************************************/
#ifndef __EMMC_MAIN_H__
#define __EMMC_MAIN_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include  "emmcConfig.h" 

#ifdef EMMC_DEBUG
void emmc_print_current_tick( void );
void emmc_reg_dump(void);
VOID emmc_packet_dump(UINT8 *pcAddr);
#endif

VOID emmc_ulong2byte(UINT8 *pucByte, UINT32 ulLong);
UINT32 emmc_byte2ulong(UINT8 *pucByte);
void emmc_delay_ms( UINT32 ulMsecond );


/*****************************************************************************
* �� �� ��  : emmc_slave_dereset
*
* ��������  : ��emmc���ý⸴λ/�⸴λ
*
* �������  : bReset:
*             TRUE:��λ
*             FALSE:�⸴λ
* �������  : ��
*
* �� �� ֵ  : ��
*
* ����˵��  : ʹ�ùܽ�gpio3.3
*
*****************************************************************************/
VOID emmc_slave_reset(BOOL bReset);

/*****************************************************************************
* �� �� ��  : emmc_read_bootloader
*
* ��������  : ��eMMC/MMC/SD�ж�ȡbootloader
*
* �������  : UINT32 dst  
* �������  : ��
*
* �� �� ֵ  : 0 :��ȡ�ɹ�
*             -1:��ȡʧ��
*
* ����˵��  : ��
*
*****************************************************************************/
INT32 emmc_read_bootloader(UINT32 dst);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif /* end of emmc.h */
