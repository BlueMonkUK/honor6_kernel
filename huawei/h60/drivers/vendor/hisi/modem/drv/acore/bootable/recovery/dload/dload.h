/*************************************************************************
*   ��Ȩ����(C) 1987-2004, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  dload.h
*
*   ��    �� :  wuzechun
*
*   ��    �� :  dload.c ��ͷ�ļ�
*
*   �޸ļ�¼ :  2012��12��13��  v1.00  wuzechun  ����
*
*************************************************************************/
#ifndef __DLOAD_H__
#define __DLOAD_H__
/*--------------------------------------------------------------*
 * �궨��                                                       *
 *--------------------------------------------------------------*/
 #include "dload_show.h"
 #include "ptable_com.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <errno.h>
#include <pthread.h>
#include "hdlc.h"

#define DEBUG
 
#ifdef DEBUG

extern pthread_mutex_t Dbug_lock;
#  define  Dbug(...)                                      \
            do {                                           \
                    int save_errno = errno;                \
                    pthread_mutex_lock(&Dbug_lock);               \
                    fprintf(stderr, "%s::%s():",           \
                            __FILE__, __FUNCTION__);       \
                    errno = save_errno;                    \
                    fprintf(stderr, __VA_ARGS__ );         \
                    fflush(stderr);                        \
                    pthread_mutex_unlock(&Dbug_lock);             \
                    errno = save_errno;                    \
            } while (0)
    
#else
#  define  Dbug(...)          ((void)0)
#endif

#define OK (0)
#ifndef ERROR
#define ERROR (-1)
#endif
#define TRUE (1)
#define FALSE (0)
#define NULL  0

#ifndef isupper
#define isupper(c)      (((c) >= 'A') && ((c) <= 'Z'))
#endif

#ifdef BCMDRIVER
#include <osl.h>
#include <bcmutils.h>
#define strtoul(nptr, endptr, base) bcm_strtoul((nptr), (endptr), (base))
#define tolower(c) (isupper((c)) ? ((c) + 'a' - 'A') : (c))
#else

#endif
#define bool unsigned long


#define RECV_BUF_SIZE 0x80000
#define SEND_BUF_SIZE 0x400

#define VER_LEN_MAX                 30              /*MNTN�ӿڹ̶�Ϊ30*/
#define CMD_VERIFY_LEN              20              /*��������ɹ��߹̶�*/
#define CMD_TCPU_VER_LEN            24              /*��������ɹ��߹̶�*/
#define CMD_BCPU_VER_LEN            64              /*��������ɹ��߹̶�*/
//#define CMD_VERIFY_FRONT_VAL        "HUAWEI "   /*���һ���ַ��ǿո�,zfh test i2c*/
#define CMD_VERIFY_FRONT_VAL "HUAWEI "

#define HDLC_SEND_BUF_MAX_LEN   0x400

#define CMD_VERIFY_LEN              20              /*��������ɹ��߹̶�*/
#define CMD_TCPU_VER_LEN            24              /*��������ɹ��߹̶�*/
#define CMD_BCPU_VER_LEN            64              /*��������ɹ��߹̶�*/
#define STUB_STR_HW_VER "DLOADID_STUB"
#define DEV_ACM_AT_NAME "/dev/acm_at"

/* �����ļ������Կ�����bufferΪ��λ������д��flash() */
//#define IMG_DLOAD_DIRECT_BLOCK_ALIGN    0x00

/* �����ļ�������һ��������buffer�����ҽ����ⲿģ�鴫���������ݱ������,�������д��flash*/
//#define IMG_DLOAD_PASSIVE_WHOLE_BUFF    0x01
#define IMAGE_NAME_SIZE 25

#define HDLC_CMD_NUM 20
#define HDLC_CMD_LEN 8
#define HDLC_CMD_RSP_NUM 25
#define HDLC_PACKET_MAXLEN  ((8*1024) + 512)


#define	CMD_ACK_LEN 5
#define	CMD_NAK_INVALID_FCS_LEN 	7
#define	CMD_NAK_INVALID_DEST_LEN	7
#define	CMD_NAK_INVALID_LEN_LEN	7
#define	CMD_NAK_EARLY_END_LEN		7
#define	CMD_NAK_TOO_LARGE_LEN	8
#define	CMD_NAK_INVALID_CMD_LEN	7
#define	CMD_NAK_FAILED_LEN			7
#define	CMD_NAK_WRONG_IID_LEN	7
#define	CMD_NAK_BAD_VPP_LEN		7
#define	CMD_NAK_VERIFY_FAILED_LEN	7
#define	CMD_NAK_NO_SEC_CODELEN	7
#define	CMD_NAK_BAD_SEC_CODE_LEN	7
#define	CMD_Reserved_LEN			7
#define	CMD_NAK_OP_NOT_PERMITTED_LEN 7
#define	CMD_NAK_INVALID_ADDR_LEN	7
#define	CMD_DLOAD_SWITCH_LEN		5
#define	CMD_CDROM_STATUS_LEN		6

/*--------------------------------------------------------------*
 * ���ݽṹ                                                     *
 *--------------------------------------------------------------*/
 
#if 0
/* �������� */
enum image_type
{
//zstart
    IMAGE_SFS_BOTTOM            = 0x0,        /*��̬�ļ� ��ʼ */
    IMAGE_CDROMISOVER          = 0x1,
    IMAGE_WEBUIVER             = 0x2,   
    IMAGE_SFS_TOP              = 0x50,      /*��̬�ļ�����*/
    

//zend

    /* �� ��flash����ӳ������ , �����������������Ҫ����������ӳ���־ */
    IMAGE_PART_START           = 0xff,      /*255*//*������������ʼ*/

    IMAGE_PTABLE                = 0x100,     /* ������ �����־*/
    IMAGE_M3BOOT               = 0x101,     /* M3 BOOT */
    IMAGE_FASTBOOT             = 0x102,     /* FASTBOOT���� �� */
    IMAGE_NVBACKLTE            = 0x108,     /* LTE NV ���� ������*/ 
    IMAGE_NVBACKGU             = 0x104,     /* GU  NV ���� ������*/
    IMAGE_NVDLD                 = 0x105,     /* ����NV �� */
    IMAGE_NVIMG                 = 0x106,     /* ����NV �� */
    IMAGE_NVFACTORY            = 0x115,      /* ����NV �� */
    IMAGE_RECOVERY		= 0x107,     /* RECOVERY���� ��  */
    IMAGE_MCORE                 = 0x103,     /* MCORE VWXORKS �� */
    IMAGE_DSP                   = 0x109,     /* DSP ���� �� */
    IMAGE_ACORE                = 0x10A,     /* ACORE LINUX �� */
    IMAGE_SYSTEM               = 0x10B,     /* SYSTEM.IMG ���� �� */
    IMAGE_MISC                  = 0x10C,     /* MISC ���� �� */
    IMAGE_CACHE                 = 0x10D,     /* CACHE ���� �� */
    IMAGE_OEMINFO             = 0x10E,      /* OEMONFO ���� �� */

    IMAGE_DATA                  = 0x10F,     /* DATA.IMG���� �� */
	IMAGE_APP                   = 0x110,     /* APP.IMG ���� �� */	
    IMAGE_ONLINE               = 0x111,     /* ONLINE ���� �� */
    IMAGE_WEBUI               = 0x112,     /* WEBUI ���� �� */
    IMAGE_CDROMISO             = 0x113,     /* CDROM ISO ������*/ 
	IMAGE_LOGO	               = 0x114,     /* LOGO �� */
    IMAGE_RECOVERY_B          = 0x116,     /* RECOVERY���� ��  */
    IMAGE_PART_TOP             = 0x200      /*���������� ����*/
};
#endif
/*HDLC USB��������*/
enum dload_cmd
{
    CMD_ACK                  = 0x02,    /*������Ӧ, UE->PC*/
    CMD_RESET                = 0x0A,    /*��λ����, PC->UE*/
    CMD_VERREQ               = 0x0C,    /*�����Ϣ��������, PC->UE*/
    CMD_VERRSP               = 0x0D,    /*�����Ϣ������Ӧ, UE->PC*/
    CMD_DLOAD_SWITCH         = 0x3A,    /*����ģʽ�л���������, UE<->PC*/
    CMD_DLOAD_INIT           = 0x41,    /*һ�����س�ʼ������, PC->UE*/
    CMD_DATA_PACKET          = 0x42,    /*�������ݰ���������, PC->UE*/
    CMD_DLOAD_END            = 0x43,    /*����һ����������, PC->UE*/
    CMD_VERIFY               = 0x45,    /*��ƷID��ѯ�뷵������, UE<->PC*/
    CMD_TCPU_VER             = 0x46,    /*��������汾��ѯ�뷵������,��������ǿ�Ƽ���ģʽ,UE<->PC*/  
    CMD_BCPU_VER             = 0x4B,    /*��������汾��ѯ�뷵������,��������bootrom����ģʽ,UE<->PC*/  
    CMD_CDROM_STATUS         = 0xFB,    /*CDROM״̬��ѯ, ��������ǿ�Ƽ���ģʽ,UE<->PC*/
    
    /*�Ƿ���Ӧ, UE->PC*/  
    CMD_NAK_INVALID_FCS      = 0x0301,    /*CRCУ�����*/  
    CMD_NAK_INVALID_DEST     = 0x0302,    /*Ŀ���ڴ�ռ䳬����Χ*/ 
    CMD_NAK_INVALID_LEN      = 0x0303,    /*���յ�����Ԥ�õĳ��Ȳ�һ��*/ 
    CMD_NAK_EARLY_END        = 0x0304,    /*���ݳ��ȹ���*/ 
    CMD_NAK_TOO_LARGE        = 0x0305,    /*���ݳ��ȹ���*/ 
    CMD_NAK_INVALID_CMD      = 0x0306,    /*��Ч������*/ 
    CMD_NAK_FAILED           = 0x0307,    /*�����޷����*/ 
    CMD_NAK_WRONG_IID        = 0x0308,    /*δʹ��*/ 
    CMD_NAK_BAD_VPP          = 0x0309,    /*δʹ��*/ 
    CMD_NAK_VERIFY_FAILED    = 0x030A,    /*��Ȩʧ��,û��Ȩ�޲���*/ 
    CMD_NAK_NO_SEC_CODE      = 0x030B,    /*û�а�ȫ��*/ 
    CMD_NAK_BAD_SEC_CODE     = 0x030C,    /*��ȫ�����/��Ч*/
    CMD_Reserved             = 0x030D,    /*Reserved */
    CMD_NAK_OP_NOT_PERMITTED = 0x030E,    /*�����Բ�����ò���*/ 
    CMD_NAK_INVALID_ADDR     = 0x030F,    /*�˵�ַ���ڴ��޷�����*/ 
    
    DLOAD_CMD_BUF
};

/*���ڴ洢iso_ver��webui_ver*/
struct version_image_type{
	char buf[IMAGE_NAME_SIZE];
	unsigned int len;
};

struct dload_buf_ctrl
{
    char  *data;               /*buffer����������СΪ1block*/
    unsigned int used;                 /*buffer����Ч���ݵĳ���*/  
    unsigned int size;                /*buffer����ռ�ĳ���*/  
    unsigned int type;                /*����ӳ������*/
};

/*DCB��*/
struct dload_ctrl_block
{
    enum dload_status dload_state;             /*��ǰ����״̬*/ 
    enum IMAGE_TYPE image_type;                 /*��ǰ����������ӳ������*/
    unsigned int        image_size;                 /*��ǰ���������Ĵ�С*/
    unsigned int        flash_current_offset;         /*��ǰ����FLASH�е���д��ַ��Է����׵�ַƫ��,��FLASHӳ�����*/
    unsigned int        ram_addr;                   /*��ǰ���յ��İ���RMA�е�Դ��ַ*/ 
    unsigned int        current_loaded_length;      /*��ǰ���յ��İ�����*/
    unsigned int        last_loaded_total_length;   /*�ϴ�Ϊֹ���յ��İ��ܳ���*/
    unsigned int        last_loaded_packet;         /*�ϴ����յ��İ����*/
    struct dload_buf_ctrl write_buf;
    enum IMAGE_TYPE finish_image_type;	
    int                 reserved[4];              /*����*/ 
    
};
struct dload_hdlc
{
    unsigned int  u32Mode;                       /* HDLC����ʹ�õ��ڲ�������HDLC����ά�� */
    unsigned int  u32Length;                     /* HDLC����ʱ�ڲ�ά�����м���� */
    unsigned int  u32DecapBufSize;               /* HDLC�������ݴ��BUFFER�ĳ��ȣ������߸���ָ�� */
    unsigned int  u32InfoLen;                    /* HDLC�����ɹ������ݳ��ȣ�HDLC����ά�� */
    unsigned char au8DecapBuf[HDLC_PACKET_MAXLEN];                   /* HDLC�������ݴ�ŵ�ַ���ɵ����߸���ָ���ռ� */
};

enum hdlc_cmd_id
{
	CMD_ACK_ID=0,
	CMD_NAK_INVALID_FCS_ID,
	CMD_NAK_INVALID_DEST_ID,
	CMD_NAK_INVALID_LEN_ID,
	CMD_NAK_EARLY_END_ID,
	CMD_NAK_TOO_LARGE_ID,
	CMD_NAK_INVALID_CMD_ID,
	CMD_NAK_FAILED_ID,
	CMD_NAK_WRONG_IID_ID,
	CMD_NAK_BAD_VPP_ID,
	CMD_NAK_VERIFY_FAILED_ID,
	CMD_NAK_NO_SEC_CODE_ID,
	CMD_NAK_BAD_SEC_CODE_ID,
	CMD_Reserved_ID,
	CMD_NAK_OP_NOT_PERMITTED_ID,
	CMD_NAK_INVALID_ADDR_ID,
	CMD_DLOAD_SWITCH_ID,
	CMD_CDROM_STATUS_ID
};
/*--------------------------------------------------------------*
 * ����ԭ������                                                 *
 *--------------------------------------------------------------*/

extern void dload_cleanup(void);
extern int dload_main(void);

#endif /* dload.h */
