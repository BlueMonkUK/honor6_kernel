/*************************************************************************
*   ��Ȩ����(C) 1987-2020, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  apDl2.h
*
*   ��    �� :  wuzechun
*
*   ��    �� :  apDl2.c ��ͷ�ļ�
*
*   �޸ļ�¼ :  2011��7��15��  v1.00  wuzechun  ����
*
*************************************************************************/

#ifndef __APDL2_H__
#define __APDL2_H__

#include "hdlc.h"

/*--------------------------------------------------------------*
 * �궨��                                                       *
 *--------------------------------------------------------------*/
#define AP_DL_CTRL_BUF_SIZE_SEND 32
#define AP_DL_CTRL_BUF_SIZE_RECV 1024

/* ���ݴ���ʱÿһ����Ч���ݵ���󳤶� */
#define DATA_PKT_LEN        512

/* ��������,����Ϊ��ȥHDLCЭ��ͷ/β/CRC���� */
#define PKT_LEN_CONNECT     3
#define PKT_LEN_BAUD_SET    7
#define PKT_LEN_AP_CMD      4
#define PKT_LEN_START_DATA  11
#define PKT_LEN_MIDST_DATA  (DATA_PKT_LEN + 4)
#define PKT_LEN_END_DATA    3
#define PKT_LEN_EXEC_DATA   3
#define PKT_LEN_REP         4

/* ����ʽ���� */
/****************************************************************************
����ʽ: ��ͷ ������    ����    FCS      ��β
            0x7E    1�ֽ�    N Bytes  2�ֽ�  0x7E

CONNECT:    0x7E    0x00        FCS    0x7E

������:  0x7E    0x10        ������(4�ֽ�)    FCS    0x7E

������:  0x7E    0x11        ָ��(1�ֽ�)    FCS    0x7E

START_DATA: 0x7E    0x12        �ļ�����(4�ֽ�)    ���ص�ַ(4�ֽ�)    FCS    0x7E

MIDST_DATA: 0x7E    0x13    ����(2�ֽ�)    �����(1�ֽ�)    ����(<512Bytes)    FCS    0x7E

END_DATA:   0x7E    0x14        FCS    0x7E

EXEC_DATA:  0x7E    0x15        FCS    0x7E 

Ӧ�����ʽ: 0x7E    0x01     ������(1�ֽ�)    FCS    0x7E
******************************************************************************/
#define PKT_TYPE_OS     0
#define PKT_TYPE_LEN    1

#define BAUD_SET_OS     (PKT_TYPE_OS+PKT_TYPE_LEN)
#define AP_CMD_OS       (PKT_TYPE_OS+PKT_TYPE_LEN)

#define START_DATA_FILELEN_OS   (PKT_TYPE_OS+PKT_TYPE_LEN)
#define FILELEN_LEN     4
#define START_DATA_ADDR_OS   (START_DATA_FILELEN_OS+FILELEN_LEN)

#define PKT_LEN_OS      (PKT_TYPE_OS+PKT_TYPE_LEN)
#define PKT_LEN_LEN     2
#define MIDST_DATA_SEQ_OS   (PKT_LEN_OS+PKT_LEN_LEN)
#define MIDST_DATA_SEQ_LEN  1
#define MIDST_DATA_OS       (MIDST_DATA_SEQ_OS+MIDST_DATA_SEQ_LEN)

#define REP_CODE_OS     (PKT_TYPE_OS+PKT_TYPE_LEN)

/*--------------------------------------------------------------*
 * ���ݽṹ                                                 *
 *--------------------------------------------------------------*/

/* �����Ͷ��� */
typedef enum _tagPktTypeE
{
    PKT_TYPE_CONNECT    = 0x00,
    PKT_TYPE_REP        = 0x01,
    
    PKT_TYPE_BAUD_SET   = 0x10,
    PKT_TYPE_AP_CMD     = 0x11,
    PKT_TYPE_STAT_DATA  = 0x12,
    PKT_TYPE_MIDST_DATA = 0x13,
    PKT_TYPE_END_DATA   = 0x14,
    PKT_TYPE_EXEC_DATA  = 0x15,
    PKT_TYPE_MAX
}pktTypeE;

/* ָ���ָ��� */
typedef enum _tagApCmdE
{
    AP_CMD_DL_BL_BURN_PRE = 0x20,   /* ����BootLoader����д�������� */
    AP_CMD_NAND_BOOT = 0x21,        /* ֱ�Ӵ�Nand���� */
    AP_CMD_EMMC_BOOT = 0x22,         /* ֱ�Ӵ�MMC�豸(eMMC/MMC/SD)���� */
    AP_CMD_HSIC_BOOT = 0x23          /* ֱ�Ӵ�SPI�豸(EEPROM/SFlash)���� */
}apCmdE;

/* Ӧ��������붨�� */
typedef enum _tagRepCodeE
{
    REP_ACK = 0x80,
    REP_BAUD_NOT_SUPPORT,
    REP_AP_CMD_NOT_SUPPORT,
    REP_NAND_READ_ERR,
    REP_SEQ_BIG,
    REP_FILE_TOOBIG,
    REP_FILE_RECV_ERR,
    REP_FILE_RECV_BUF_FULL,
    REP_FILE_RECV_TIMEOUT,
    REP_SEC_CHK_ERR
}RepCodeE;

/* ���ؽ������̴����붨�� */
typedef enum _tagApDlStatusE
{
    AP_DL_ERR_OK = 0,
    AP_DL_ERR_HDLC_ENCAP,
    AP_DL_ERR_HDLC_DECAP,
    AP_DL_ERR_RECV,
    AP_DL_BUF_FULL,
    AP_DL_RECV_TIMEOUT,
    AP_DL_ERR_MAX
}apDlStatusE;

typedef struct _tagApDlCtrlS
{
    UINT32  ulAddress;      /* ���ݴ�ŵĵ�ַ */
    UINT32  ulFileLength;   /* Ԥ��Ӧ���յ����ݵĳ��� */
    UINT32  ulPosition;     /* �Ѿ��յ����ݵ���Ŀ */
    UINT8   u8SeqNext;      /* ��һ��Ԥ���յ��İ��� */
    BOOL    bComplete;      /* ��ǰ֡�Ƿ������ */
    
    UINT16  u16SendLen;
    UINT16  u16RecvLen;

    RECV_STR *pstHsUartBuf;

    DLOAD_HDLC_S *pstHdlcCtrl;
}apDlCtrlS;

/*--------------------------------------------------------------*
 * ����ԭ������                                                 *
 *--------------------------------------------------------------*/
apDlStatusE apDlMain( UINT32 ulFileAddr );

#endif /* apDl2.h */

