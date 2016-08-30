
#ifndef _CIPHER_ACC_H
#define _CIPHER_ACC_H

#include "bsp_cipher.h"
#include "cipher_balong_common.h"
#include "cipher_balong.h"/*Ϊ��ʹ��BD/RD�ṹ��*/

#ifndef CHN_BUSY
#define CHN_BUSY                    0
#endif
#ifndef CHN_FREE
#define CHN_FREE                    1
#endif

#define ACC_FIFO_ERROR    0
#define ACC_FIFO_STAT_NUM ACC_STAT_BOTTOM
#define ACC_CHN_NUM       0	/*ACCʹ�õ�ͨ����*/
#define ACC_FIFO_NUM      2	/*ACCʹ�õ�FIFO������*/

/*������ټĴ���*/
#define CHN_STATBIT          0x40000000    /*ȡͨ��ʹ�ܼĴ����ĵ�30λ*/
#define CHN_PACK_ENBITS      0xBFFFFFFF    /*��ֹpack_enλ��ͨ��ʹ�ܼĴ����ĵ�30λ   */
#define CHN_ENBITS           0x1           /*ʹ��ch_enλ��ͨ��ʹ�ܼĴ����ĵ�0λ */
#define CHN_BDRESET          0x2           /*ͨ����λ�Ĵ��������BD���У���λ��ָ��*/
#define CHN_WPT              0xFFFFFC00    /*ͨ��дָ�븴λ*/
#define CHN_DENBITS          0xFFFFFFFE    /*ͨ��ʹ�ܼĴ������λ��0����ͣͨ��*/
#define CIPHER_INT0_CH0      0x3F          /*�жϼĴ���0�У���Ӧ��ͨ��0�ĵ�6λ��1*/
#define CIPHER_INT1_CH0      0xD           /*�жϼĴ���1�У���Ӧ��ͨ��0��0��2��3λ��1*/
#define CHN_CFGUSR           0x60          /*��ͨ��0�����üĴ�����userfiled�����3*/

#define ALIN_BYTES         8                          /* 8�ֽڶ���Ҫ�� */
#define CIPHER_DESC_SIZE   sizeof(struct cipher_bd_s) /* Ӳ������ÿ������������(Byte) */
#define ACC_FIFO_MAX_DEEP  200                        /* BDFIFO��������*/


#define ACC_ERROR_PRINT(errno) \
do{\
    {\
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_ACC, "[ACC API ERROR]:\n\t\t <func>: %s;  <line>: %d;  <Errno>: %s (0x%08x)\n\n", \
               (int)__FUNCTION__, (int)__LINE__, (int)#errno, errno);\
    }\
}while(0)
#define ACC_INFOR_PRINT(fmt, ...)  (bsp_trace(BSP_LOG_LEVEL_INFO,  BSP_MODU_ACC, "[ACC API INFOR]: <%s> "fmt, __FUNCTION__, ##__VA_ARGS__))

/*BDFIFO״̬��*/
enum ACC_FIFO_STATUS_E
{
    ACC_STAT_WORK = 0,           /* ���ڱ�ʹ�� */
    ACC_STAT_CFG,                /* �Ѿ������� */
    ACC_STAT_IDLE,               /* ����״̬ */
    ACC_STAT_BOTTOM
};

/* �������ģ�����ȫ�ֽṹ�� */
struct acc_fifo_mgr_s
{
	u32                     bd_fifo_addr;    /* FIFO����������ַ */
    enum ACC_FIFO_STATUS_E  fifo_stat;       /* BDFIFO״̬*/
    u32                     bd_fifo_cfg_num; /* ���ά���Ĵ�д��ַ */
};

/*ACC Debug �ṹ��*/
 struct acc_debug_s
{
    u32 get_fifo_times;
    u32 cfg_dma_times;
    u32 cfg_cipher_times;
    u32 enable_times;
    u32 drop_pak_times;
};

/*ACCģ��ר��ͨ������ṹ��*/
struct acc_chx_mgr_s
{
	struct acc_fifo_mgr_s acc_fifo_mgr[ACC_FIFO_NUM];
	struct acc_debug_s acc_debug;
	u32	acc_fifo_deep;
	u32 init_flag;
	u32 cur_used_fifo_num;
};


/*****************************************************************************
* �� �� ��  : bsp_acc_init
*
* ��������  : ��ʼ���������ģ��
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ  : �ɹ�/ʧ��
*
* �޸ļ�¼  : 2011��1��8��   wangjing  creat
*****************************************************************************/
s32 bsp_acc_init ();
/*****************************************************************************
* �� �� ��  : bsp_acc_get_bdfifo_addr
*
* ��������  : ���ڻ�ȡ��ǰ���õ�BDFIFO�׵�ַ
*
* �������  : ��
*
* �������  : ��
* �� �� ֵ  : BDFIFO�׵�ַ
*****************************************************************************/
u32 bsp_acc_get_bdfifo_addr();
/*****************************************************************************
* �� �� ��  : bsp_acc_dma
*
* ��������  : Ϊָ��ͨ�����������������ڵ�����DMA����
*
* �������  : bd_fifo_addr: BDFIFO�׵�ַ
*                          p_in_mem_mgr    : ����buffer���ݿ���׵�ַ
*                          p_out_mem_mgr   : ���buffer���ݿ���׵�ַ
* �������  : ��
* �� �� ֵ  : �ɹ�/ʧ��
*****************************************************************************/
s32 bsp_acc_dma(u32 bd_fifo_addr, const void* p_in_mem_mgr, const void* p_out_mem_mgr,
                            ACC_SINGLE_CFG_S *p_cfg);
/*****************************************************************************
* �� �� ��  : bsp_acc_cipher
*
* ��������  : Ϊָ��ͨ��������������������ҪCipher���м��ܵĲ���
*
* �������  : bd_fifo_addr: BDFIFO�׵�ַ
*                          p_in_mem_mgr    : ����buffer���ݿ���׵�ַ
*                          p_out_mem_mgr   : ���buffer���ݿ���׵�ַ
*                          p_cfg       : �������������Ϣ
* �������   : ��
* �� �� ֵ  : �ɹ�/ʧ��
*****************************************************************************/
s32 bsp_acc_cipher(u32 bd_fifo_addr, const void* p_in_mem_mgr, const void* p_out_mem_mgr,
                               ACC_SINGLE_CFG_S *p_cfg);
/*****************************************************************************
* �� �� ��  : bsp_acc_get_status
*
* ��������  : ��ȡ��ǰͨ��״̬
*
* �������  :  ��
* �������   : ��
* �� �� ֵ  : ͨ��æ/����
*****************************************************************************/
s32 bsp_acc_get_status();
/*****************************************************************************
* �� �� ��  : bsp_acc_enable
*
* ��������  : ʹ���������
*
* �������  : bd_fifo_addr : BDFIFO�׵�ַ
*
* �������  : ��
* �� �� ֵ  : �ɹ�/ʧ��
*****************************************************************************/
s32 bsp_acc_enable(u32 bd_fifo_addr);

void bsp_acc_debug_show();

#endif
