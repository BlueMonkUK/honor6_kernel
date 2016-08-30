#include <osl_common.h>
#include <osl_bio.h>

#include "hi_base.h"
#include "hi_syssc.h"
#include "hi_tsensor.h"

#define TEMCODE_TO_TEMPERATURE(c) (((c)*200-60*255)*10/255)

short trim_array[]={0,8,16,23,31,39,47,55,-8,-16,-23,-31,-39,-47,-55,-62};


int tsensor_init(void)
{

    set_hi_temp_config_sample_num(0x2);    /*ͬһͨ�������Ĵ���*/

    set_hi_temp_config_sample_mode(0x2);    /*��β������þ�ֵ*/

    set_hi_temp_config_test(0x0);           /*Tsensorͨ��ѡ��local*/

    //set_hi_temp_config_ct_sel(0x01);        /*ת��ʱ��Ϊ6.144ms*/
    
    set_hi_temp_config_ct_sel(0x0);        /*ת��ʱ��Ϊ0.768ms*/
    
    set_hi_temp_dis_time_temp_dis_time(0x6);/*�л�Tsensorʱ����*/
    
    set_hi_temp_en_temp_en(0x1);    /*ʹ��*/
    
    
    return 0;
}

/* ***********************************
* chip�¶Ȼ�ȡ�ӿڣ�ע��:
* (1)�¶Ȳɼ�ʱ������Ϊ1.5ms
* (2)����1300(��130��)���붪��
*************************************/
int chip_tem_get(void)
{
    int main_tem = 0;
    int trim_tem = 0;
    
    int main_code = 0;
    int trim_code = 0;
	    
    main_code = get_hi_temp_temp();
    trim_code = get_hi_sc_stat86_efuse_tsensor_trim() & ((0x1 << 8) - 1);

    main_tem = TEMCODE_TO_TEMPERATURE(main_code);
    trim_tem = trim_array[trim_code & 0xF];
    
    return (main_tem + trim_tem);

}




