
/*lint --e{537}*/
#ifndef _BBP_BALONG_H_
#define _BBP_BALONG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_memmap.h"
#include "bsp_bbp.h"

/*CHn addr = CH0 addr + 0x10*n,nָ����dmaͨ��*/
#define BBP_FAST_CH0_BBP_ADDR_OFFSET    0x0a00  /*BBP��ʼ��ַ*/
#define BBP_FAST_CH0_SOC_ADDR_OFFSET    0x0a04 /*SOC��ʼ��ַ��������DDR����TCM*/
#define BBP_FAST_CH0_CTL_OFFSET    0x0a0c /*���ƼĴ���*/

#define  BBP_TASK_STK_SIZE            (0x1000)

/*��������*/
int bbp_poweron(void);
int bbp_poweroff(void);

#ifdef __cplusplus
}
#endif

#endif
