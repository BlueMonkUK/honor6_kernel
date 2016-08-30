
#ifndef __DLOAD_HDLC_H__
#define __DLOAD_HDLC_H__
    
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
    UINT32  u32Mode;                       /* HDLC����ʹ�õ��ڲ�������HDLC����ά�� */
    UINT32  u32Length;                     /* HDLC����ʱ�ڲ�ά�����м���� */
    UINT32  u32DecapBufSize;               /* HDLC�������ݴ��BUFFER�ĳ��ȣ������߸���ָ�� */
    UINT32  u32InfoLen;                    /* HDLC�����ɹ������ݳ��ȣ�HDLC����ά�� */
    UINT8   au8DecapBuf[HDLC_BUF_SIZE];                   /* HDLC�������ݴ�ŵ�ַ���ɵ����߸���ָ���ռ� */
}DLOAD_HDLC_S;

void HDLC_Init(DLOAD_HDLC_S *pstHdlc);
eHdlcState HDLC_Encap(const UINT8 *pu8Src,UINT16 u16SrcLen,UINT8 *pu8Dst,UINT16 u16DestBufLen,UINT16 *pu16DstLen);
eHdlcState HDLC_Decap(DLOAD_HDLC_S *pstHdlc, UINT8 ucChar);
UINT16 HDLC_Fcs( UINT8 *pucData, UINT32 ulDataLen);


#endif /* hdlc.h */

