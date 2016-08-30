
/*lint --e{537}*/
#include "hi_base.h"
#include "bsp_memmap.h"
#include "hi_bbp_systime.h"

#include "bsp_bbp.h"
//#include "osl_types.h"
/*****************************************************************************
* ����  : bsp_bbp_getcurtime
* ����  : ��PS���ã�������ȡϵͳ��ȷʱ��
* ����  : void
* ���  : u64 *pcurtime
* ����  : u32
*****************************************************************************/
u32 bsp_bbp_getcurtime(unsigned long long *pcurtime)
{
	/*lint -save -e958*/
    unsigned long long  timervalue[4];
	/*lint -restore*/

    timervalue[0] = get_hi_bbp_systime_abs_timer_l_abs_timer_l();
    timervalue[1] = get_hi_bbp_systime_abs_timer_h_abs_timer_h();
    timervalue[2] = get_hi_bbp_systime_abs_timer_l_abs_timer_l();
    timervalue[3] = get_hi_bbp_systime_abs_timer_h_abs_timer_h();

    if(timervalue[2] < timervalue[0])
    {
        (*pcurtime) = ((timervalue[3] - 1) << 32) | timervalue[0];
    }
    else
    {
        (*pcurtime) = (timervalue[1] << 32) | timervalue[0];
    }

    return 0;
}


