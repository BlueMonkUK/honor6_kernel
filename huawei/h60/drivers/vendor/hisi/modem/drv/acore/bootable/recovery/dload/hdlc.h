/*************************************************************************
*   ��Ȩ����(C) 1987-2004, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  HDLC.h
*
*   ��    �� :  w170956
*
*   ��    �� :  HDLCͷ�ļ�
*
*   �޸ļ�¼ :  2013��1��13��  v1.00  w170956  ����
*
*************************************************************************/
#ifndef __HDLC_H__
#define __HDLC_H__

/*--------------------------------------------------------------*
 * �궨��                                                       *
 *--------------------------------------------------------------*/
#define HDLC_FRAME_FLAG  0x7e        /* HDLC֡��־λ���̶�Ϊ0x7e�����ֲ�ͬ��HDLC֡ */
#define HDLC_ESC         0x7d        /* ת���ַ������������ַ���Ҫת�� */
#define HDLC_ESC_MASK    0x20        /* ת���ַ����� */
        
#define HDLC_INIT_FCS    0xffff      /* ����FCS�ĳ�ʼFCSֵ */
#define HDLC_GOOD_FCS    0xf0b8      /* ������Ϊ��FCSֵʱ��˵��FCSУ����ȷ */

#define HDLC_MODE_HUNT   0x1         /* ���װģʽ��bit0Ϊ1������δ�ҵ�֡��־��û�п�ʼ���װ */
#define HDLC_MODE_ESC    0x2         /* ���װģʽ��bit1Ϊ1�������ϸ��ַ�Ϊת���ַ�����ǰ�ַ�Ϊ�������ݵ����� */
        
#define HDLC_FCS_LEN     2           /* HDLC֡��FCSδת��ʱ�ĳ���(��λ: Byte) */

#define HDLC_BUF_SIZE    1024

#define HDLC_PACKET_MAXLEN  ((8*1024) + 512)

typedef enum _eHdlcState
{
    HDLC_OK = 0,                /* HDLC����ɹ� */
    HDLC_PARA_ERR,              /* ��������쳣 */
    HDLC_ERR_BUF_FULL,
    HDLC_ERR_INVALAID_FRAME,    /* û��������֡ */
    HDLC_ERR_FCS,               /* ������У����� */
    HDLC_ERR_DISCARD            /* �����쳣��������ǰHDLC֡ */
}eHdlcState;

typedef struct tagDLOAD_HDLC_S
{
    unsigned int  u32Mode;                       /* HDLC����ʹ�õ��ڲ�������HDLC����ά�� */
    unsigned int  u32Length;                     /* HDLC����ʱ�ڲ�ά�����м���� */
    unsigned int  u32DecapBufSize;               /* HDLC�������ݴ��BUFFER�ĳ��ȣ������߸���ָ�� */
    unsigned int  u32InfoLen;                    /* HDLC�����ɹ������ݳ��ȣ�HDLC����ά�� */
    unsigned char au8DecapBuf[HDLC_PACKET_MAXLEN];                   /* HDLC�������ݴ�ŵ�ַ���ɵ����߸���ָ���ռ� */
}DLOAD_HDLC_S;

void HDLC_Init(DLOAD_HDLC_S **pstHdlc);
eHdlcState HDLC_Encap( const char *pu8Src, unsigned short u16SrcLen, char *pu8Dst, int u16DestBufLen, int *pu16DstLen);
eHdlcState HDLC_Decap(DLOAD_HDLC_S *pstHdlc, unsigned char ucChar);
unsigned short HDLC_Fcs( unsigned char *pucData, unsigned int ulDataLen);


#endif /* hdlc.h */

