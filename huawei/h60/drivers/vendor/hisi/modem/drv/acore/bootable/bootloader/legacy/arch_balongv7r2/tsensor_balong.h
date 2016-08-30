#ifndef	_TSENSOR_BALONG_H_
#define	_TSENSOR_BALONG_H_
#include "product_config.h"

#define TSENSOR_OK      0
#define TSENSOR_ERROR   -1

typedef enum {    
    TSENS_REGION_0   = 0,
    TSENS_REGION_1   = 1,
    TSENS_REGION_2   = 2,
    MAX_TSENS_REGION
} TSENSOR_REGION;


#ifdef CONFIG_TSENSOR

/*tsensor��ʼ������*/
int tsensor_init();

/*tsensor��ȡоƬ�ڲ��¶���ֵ*/
int tsens_tem_get(TSENSOR_REGION region);

int chip_tem_get();

#ifndef __CMSIS_RTOS
/* chip�¶Ȼ�ȡ���������130�棬�붪����*/
int chip_tem_get(void);
#endif

#else
static inline int tsensor_init(void)
{
    return 0;
}

static inline int tsens_tem_get(TSENSOR_REGION region)
{
    return 60;
}

int chip_tem_get()
{
    return 60;
}

#endif

#endif

