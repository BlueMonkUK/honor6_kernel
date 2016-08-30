/*************************************************************************
*   ��Ȩ����(C) 1987-2004, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  at.h
*
*   ��    �� :  wuzechun
*
*   ��    �� :  at.c ��ͷ�ļ�
*
*   �޸ļ�¼ :  2012��12��13��  v1.00  wuzechun  ����
*
*************************************************************************/

#ifndef __AT_H__
#define __AT_H__
/*--------------------------------------------------------------*
 * �궨��                                                       *
 *--------------------------------------------------------------*/
    
#define ATCOMMAND_ERROR             "\r\nERROR\r\n"
#define ATCOMMAND_OK                "\r\nOK\r\n"

#define ATCOMMAND_PROCESS_SUCEED    0
#define ATCOMMAND_PROCESS_FAIL      1
#define ATCOMMAND_NOT_FOUND         2

#define IMEI_LEN                    16
#define BCD_IMEI_LEN                9

#define DLOAD_VERSION               "2.0"                   /* ����Э��汾 */

/*--------------------------------------------------------------*
 * ���ݽṹ                                                     *
 *--------------------------------------------------------------*/
    
/*AT command*/
typedef enum
{
    DLOAD_AT_E0,    /*ate0*/
    DLOAD_AT_E1,    /*ate1*/
    DLOAD_AT_DLOADVER,    /*at^dloadver*/
    DLOAD_AT_DLOADINFO,     /*at^dloadinfo*/
    DLOAD_AT_AUTHORITYVER,        /*at^authorityver*/
    DLOAD_AT_AUTHORITYID,         /*at^authorityid*/
    DLOAD_AT_DATAMODE,  /*at^datamode*/
    DLOAD_AT_HWNATQRY,  /*at^hwnatqry*/
    DLOAD_AT_FLASHINFO,
    DLOAD_AT_CMEE,
    DLOAD_AT_GODLOAD,
    DLOAD_AT_NVBACKUP,//ZFH for test
    DLOAD_AT_BUF     /*��Ч״̬*/
}DLOAD_AT_COMMAND;

/*--------------------------------------------------------------*
 * ����ԭ������                                                 *
 *--------------------------------------------------------------*/

extern int at_cmd_proc( unsigned char *pcAtcmd, int len);

#endif /* at.h */

