/*************************************************************************
*   ��Ȩ����(C) 1987-2020, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  enumProt.h
*
*   ��    �� :  wuzechun
*
*   ��    �� :  �Ծ�Э��֡��ʽ����
*
*   �޸ļ�¼ :  2011��6��29��  v1.00  wuzechun  ����
*
*************************************************************************/

#ifndef __ENUMPROT_H__
#define __ENUMPROT_H__

/*--------------------------------------------------------------*
 * �궨��                                                       *
 *--------------------------------------------------------------*/

/* �Ծ�Э��֡�ṹ���� */
/* FILE FRAME: TYPE(1)+SEQ(1)+CSEQ(1)+FILE(1)+LENGTH(4)+ADDRESS(4)+CRC(2) */
/* DATA FRAME: TYPE(1)+SEQ(1)+CSEQ(1)+DATA(0~1024)+CRC(2) */
/* EOT  FRAME: TYPE(1)+SEQ(1)+CSEQ(1)+CRC(2) */
#define	ENUM_FRAME_HEAD	            0
#define	ENUM_FRAME_SEQ	            1
#define	ENUM_FRAME_CSEQ	            2
#define	ENUM_FRAME_FILE_TYPE		3
#define	ENUM_FRAME_DATA_LOC	        3
#define	ENUM_FRAME_FILE_LEN	        4
#define	ENUM_FRAME_FILE_ADDR		8
#define	ENUM_FRAME_FILE_CRC	        12
/* �Ծ�֡���Ͷ��� */
#define	ENUM_FRAME_TYPE_FILE		0xFE
#define	ENUM_FRAME_TYPE_DATA		0xDA
#define	ENUM_FRAME_TYPE_EOT	        0xED
/* �Ծ�֡��Ӧ���� */
#define	ENUM_RESP_TYPE_ACK	        0xAA
#define	ENUM_RESP_TYPE_NAK	        0x55
#define	ENUM_RESP_TYPE_SNAK	        0xEE
/* �Ծ�֡���ͳ��ȶ��� */
#define ENUM_FRAME_LEN_FILE            14
#define ENUM_FRAME_LEN_EOT             5

/* �Ծ��ļ��������Ͷ��� */
#define	ENUM_XFR_FTYPE_RAMINIT	            0x01
#define	ENUM_XFR_FTYPE_USB	            0x02

/* �Ծ�Э�鴦����ֵ���� */
#define	ENUM_XFR_PROT_OK	            0
#define	ENUM_XFR_PROT_ERR	            1
#define	ENUM_XFR_PROT_SKIP	            2
#define	ENUM_XFR_PROT_COMPLETE	        3
#define ENUM_XFR_PROT_INPROGRESS	    4


#endif /* __ENUMPROT_H__.h */

