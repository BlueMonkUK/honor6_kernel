#ifndef __ANTEN_BALONG_H__
#define __ANTEN_BALONG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "drv_anten.h"

#define ANTEN_OK               (0)
#define ANTEN_ERROR            (-1)


/*****************************************************************************
 * �� �� ��  :bsp_anten_init
 *
 * ��������  : ���߲�κ˼�ͨ�ţ�C�˳�ʼ������
 *
 * �������  : ��
 * �������  : ��
 *
 * �� �� ֵ  : ANTAN_OK:    �����ɹ�
 *             ANTAN_ERROR: ����ʧ��
 *
 * ����˵��  : ��
 *
 *****************************************************************************/
s32 bsp_anten_init(void);

/****************************************************
Function:   bsp_anten_int_install   
Description:      �ж�ע�ắ����������õ�ǰ������״̬����׮��ASICƽ̨����Ҫʵ�֣�
Input:        NA;
Output:       NA;
Return:       NA;
Others:       NA;
****************************************************/ 
void bsp_anten_int_install(PWC_COMM_MODEM_E modem_id, void* routine,int para);

int anten_nv_init(void);

#ifdef __cplusplus
}
#endif

#endif
