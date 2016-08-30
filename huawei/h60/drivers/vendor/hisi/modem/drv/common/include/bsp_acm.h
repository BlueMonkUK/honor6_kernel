/*************************************************************************
*   ��Ȩ����(C) 1987-2011, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  drv_acm.h
*
*   ��    �� :  fanchangdong
*
*   ��    �� :  ncm drv�ڲ��ӿ�ͷ�ļ�
*
*   �޸ļ�¼ :  2013��2��2��  v1.00  fanchangdong����
*************************************************************************/

#ifndef __BSP_ACM_H__
#define __BSP_ACM_H__

#ifdef __cplusplus
extern "C" { /* allow C++ to use these headers */
#endif /* __cplusplus */

extern s32 bsp_acm_open(u32 dev_id);
extern s32 bsp_acm_close(s32 handle);
extern s32 bsp_acm_write(s32 handle, void *buf, u32 size);
extern s32 bsp_acm_read(s32 handle, void *buf, u32 size);
extern s32 bsp_acm_ioctl(s32 handle, u32 cmd, void *para);

#ifdef __cplusplus
} /* allow C++ to use these headers */
#endif /* __cplusplus */

#endif    /* End of __BSP_ACM_H__ */
