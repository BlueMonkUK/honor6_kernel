

#include <bsp_wakelock.h>

void wake_lock_init(struct wake_lock *lock, int type, const char *name)
{
}
void wake_lock(struct wake_lock *lock)
{
}
void wake_unlock(struct wake_lock *lock)
{
}

int wake_lock_active(struct wake_lock *lock)
{
    return 0;
}
unsigned int has_wake_lock(int type)
{
    return 0;
}
void all_vote_wake_unlock(void)
{
    return;
}
/*****************************************************************************
 �� �� ��  : BSP_PWRCTRL_StandbyStateCcpu/BSP_PWRCTRL_StandbyStateAcpu
 ��������  : AT^PSTANDBY
 �������  :
 �������  :
 ����ֵ��
*****************************************************************************/
 unsigned int BSP_PWRCTRL_StandbyStateAcpu(unsigned int ulStandbyTime, unsigned int ulSwitchTime)
 {
	return 0;
 }
 unsigned int BSP_PWRCTRL_StandbyStateCcpu(unsigned int ulStandbyTime, unsigned int ulSwitchTime)
 {
	return 0;
 }


