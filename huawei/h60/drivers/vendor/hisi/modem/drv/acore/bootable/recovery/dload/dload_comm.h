/*************************************************************************
*   ��Ȩ����(C) 1987-2004, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  dload_comm.h
*
*   ��    �� :  wuzechun
*
*   ��    �� :  dload_lib.c ��ͷ�ļ�
*
*   �޸ļ�¼ :  2012��12��26��  v1.00  wuzechun  ����
*
*************************************************************************/
#include "at.h"
#include "dload.h"
#include "mtdutils/mtdutils.h"

#define u32  unsigned int
#define DLOAD_NAND_SPARE_ALIGN      (0x200000) 
#define DLOAD_NAND_SPARE_ALIGN_ADD (0x202000)


struct MtdPartition {
    int device_index;
    unsigned int size;
    unsigned int erase_size;
    char *name;
};

struct MtdWriteContext {
    const MtdPartition *partition;
    char *buffer;
    size_t stored;
    int fd;

    off_t* bad_block_offsets;
    int bad_block_alloc;
    int bad_block_count;
};

int dload_align_buffer_write(struct dload_buf_ctrl *write_buf);

/*****************************************************************************
* �� �� ��    : image_dload_to_flash
*
* ��������  :  �����յ������ݷ�����flash��д�������buf�У�������д�뺯��
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ    : 
			    0:�����ɹ�
			  -1:����ʧ��
* 
* ����˵��  : 
*
*****************************************************************************/
int image_dload_to_flash(struct dload_buf_ctrl **write_buf,unsigned int align_size);
/*****************************************************************************
* �� �� ��    : image_dload_process
*
* ��������  :  ����������д
*
* �������  : ��
* �������  : 
*
* �� �� ֵ    : 
			  0:�����ɹ�
			-1:����ʧ��
*
* ����˵��  : 
*
*****************************************************************************/
int image_dload_process(struct dload_buf_ctrl *write_buf,unsigned int align_size);

/*****************************************************************************
* �� �� ��  : dload_buf_init
*
* ��������  : �������ػ������ռ�
*
* �������  : 
			type: ����id
			len  : �����buf����
* �������  : 
*
* �� �� ֵ  : 
*			  0:�����ɹ�
			-1:����ʧ��

* ����˵��  : 
*
*****************************************************************************/
int dload_buf_init(enum IMAGE_TYPE type, unsigned int len,struct dload_buf_ctrl *write_buf);

int dloadNeedMidwayRestart(void);
/******************************************************************************
*  Function:  getAuthorityId
*  Description:
*      ��ȡ���ؼ�ȨЭ��Id
*
*  Calls:
*
*  Called By:
*
*  Data Accessed: 
*
*  Data Updated: 
*
*  Input:
*        buf: ������ؼ�ȨЭ��Id�Ļ���
*        len: ���泤��
*
*  Output:
*         NONE
*
*  Return:
*         0: �����ɹ�����
*         1: ����ʧ��
*
********************************************************************************/

int get_authorityId(unsigned char buf[], int len);

/*
*************************************************************

*	��������ʱ���޸ķ������ж�Ӧ������count ��ֵ 	*

*************************************************************
*/
int update_image_count(enum IMAGE_TYPE image_idx);

/*
*************************************************************

*	����ptable����ʱ���޸ķ������е�recovery ������count ��ֵ 	*

*************************************************************
*/
int update_ptable_count(void);


