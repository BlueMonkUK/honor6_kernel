


#ifdef __cplusplus
extern "C"
{
#endif

#include "nandc_inc.h"
/*����NANDC_TRACE��ӡ��Ϣ�ļ���Ĭ���������NANDC_TRACE�Ǵ�ӡerror��warning��Ϣ*/
u32 nandc_trace_mask = (u32)(NANDC_TRACE_ALL | NANDC_TRACE_ERRO | NANDC_TRACE_WARNING);


/*����nandc��������ʼ����������Ϣ,ͨ�������ж���V400����V600��nandc������*/
#if defined (NANDC_USE_V400)

struct nandc_init_info  *nandc_init_seed = &nandc4_init_info;

#elif defined (NANDC_USE_V600)

struct nandc_init_info  *nandc_init_seed = &nandc6_init_info;

#else

struct nandc_init_info  *nandc_init_seed = NULL;

#endif

/*����nandcģ��Ĵ�ӡ����*/
/*ʹ��nandcģ���normal����Ĵ�ӡ*/
void nandc_enable_trace(void)
{
    nandc_trace_mask = (nandc_trace_mask | NANDC_TRACE_NORMAL);
}

void nandc_disable_trace(void)
{
    nandc_trace_mask = (nandc_trace_mask &(~NANDC_TRACE_NORMAL));
}

#ifdef __cplusplus
}
#endif


