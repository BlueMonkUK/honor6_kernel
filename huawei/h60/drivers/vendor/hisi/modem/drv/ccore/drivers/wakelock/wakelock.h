
#ifndef __WAKELOCK_H__
#define __WAKELOCK_H__

#include <bsp_wakelock.h>
/************************************************************************
*ʱ��ģ����Խӿ�
************************************************************************/
void debug_wake_lock(PWC_CLIENT_ID_E id);
void debug_wake_unlock(PWC_CLIENT_ID_E id);
void debug_wakelock(void);

/************************************************************************
*ʱ��ģ���Զ������Ժ���
************************************************************************/
int wake_lock_init_case1(void);
int wake_lock_case1(void);
int wake_unlock_case1(void);
#endif
