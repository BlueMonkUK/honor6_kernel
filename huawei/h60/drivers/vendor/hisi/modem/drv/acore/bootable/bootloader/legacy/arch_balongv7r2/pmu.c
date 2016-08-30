
#include <product_config.h>
#include <pmu.h>
#include <boot/boot.h>

#ifdef CONFIG_PMIC_HI6451
#include <balongv7r2/pmic_hi6451.h>
#endif
#ifdef CONFIG_PMIC_HI6551
#include <balongv7r2/pmic_hi6551.h>
#include <balongv7r2/dr_hi6551.h>
#endif
#ifdef CONFIG_PMIC_HI6559
#include <balongv7r2/pmic_hi6559.h>
#include <balongv7r2/dr_hi6559.h>
#endif

struct pmu_adp_ops{
    int (*lcd_io_power_switch)(power_switch_e sw);
    int (*lcd_analog_power_switch)(power_switch_e sw);
    int (*lcd_analog_volt_set)(int);
    int (*lcd_bl_cur_set)(int);
    int (*lcd_bl_power_switch)(power_switch_e sw);
    int (*efuse_power_switch)(power_switch_e sw);
    int (*efuse_volt_set)(int);
    bool (*hreset_state_get)(void);
    void (*hreset_state_clear)(void);
    bool (*usb_state_get)(void);
    bool (*key_state_get)(void);
    u8 (*version_get)(void);
    int (*get_boot_state)(void);
    void (*set_by_nv)(void);
    int (*dr_bre_time_set)(dr_id_e dr_id, DR_BRE_TIME *dr_bre_time_st);
    void (*pmu_init)(void);
};
/*****************************************************************************
 �� �� ��  : dummy_set_volt_dr
 ��������  : ����PMU��ѹ����Դ��׮����
 �������  : para:����
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :
*****************************************************************************/
int dummy_volt_switch(power_switch_e sw)
{
    sw = sw;
    pmic_print_info("no pmic macro defined or not need onoff,use dummy!");
    return  0;
}
/*****************************************************************************
 �� �� ��  : dummy_set_volt_dr
 ��������  : ����PMU��ѹ����Դ��׮����
 �������  : para:����
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :
*****************************************************************************/
int dummy_volt_set(int para)
{
    para = para;
    pmic_print_info("no pmic macro defined,use dummy!");
    return  0;
}

/*****************************************************************************
 �� �� ��  : dummy_get_part_state
 ��������  :ϵͳ����ʱ���pmu��usb\��������״̬��Ĭ�ϴ�׮����
 �������  : void
 �������  : reset.log
 �� �� ֵ  : pmu�����ok
 ���ú���  :
 ��������  :��ŵ硢���ػ�
*****************************************************************************/
bool dummy_get_part_state(void)
{
    pmic_print_error("no pmic macro defined,use dummy!");
    return  0;
}
/*****************************************************************************
 �� �� ��  : dummy_get_boot_state
 ��������  :ϵͳ����ʱ���pmu�Ĵ���״̬��Ĭ�ϴ�׮����
 �������  : void
 �������  : reset.log
 �� �� ֵ  : pmu�����ok
 ���ú���  :
 ��������  :ϵͳ��ά�ɲ�
*****************************************************************************/
int dummy_get_boot_state(void)
{
    pmic_print_error("no pmic macro defined,use dummy!");
    return  0;
}
/*****************************************************************************
 �� �� ��  : dummy_default_set_handler
 ��������  : ͨ��nv�����Ĭ�����õĴ�׮����
 �������  : ��
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :
*****************************************************************************/
void dummy_clear_part_state(void)
{
    pmic_print_info("pmic macro hi6551 not defined,use dummy!");
}
/*****************************************************************************
 �� �� ��  : dummy_info_get_handler
 ��������  : ��ѯPMU�汾�ŵĴ�׮����
 �������  : ��
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :
*****************************************************************************/
u8 dummy_info_get_handler(void)
{
    pmic_print_error("no pmic macro defined,use dummy!");
    return  0;
}
/*****************************************************************************
 �� �� ��  : dummy_default_set_handler
 ��������  : ͨ��nv�����Ĭ�����õĴ�׮����
 �������  : ��
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :
*****************************************************************************/
void dummy_set_by_nv_handler(void)
{
    pmic_print_info("pmic macro hi6551 not defined,use dummy!");
}
/*****************************************************************************
 �� �� ��  : dummy_dr_fla_time_set
 ��������  : DR��˸ʱ�����ã�Ĭ�ϴ�׮����
 �������  : void
 �������  : reset.log
 �� �� ֵ  : pmu�����ok
 ���ú���  :
 ��������  : LEDģ��
*****************************************************************************/
int dummy_dr_fla_time_set(DR_FLA_TIME* dr_fla_time_st)
{
    dr_fla_time_st = dr_fla_time_st;
    pmic_print_error("no pmic macro defined,use dummy!");
    return  BSP_PMU_NO_PMIC;
}
/*****************************************************************************
 �� �� ��  : dummy_dr_bre_time_set
 ��������  : DR����ʱ�����ã�Ĭ�ϴ�׮����
 �������  : void
 �������  : ��
 �� �� ֵ  : pmu�����ok
 ���ú���  :
 ��������  : LEDģ��
*****************************************************************************/
int dummy_dr_bre_time_set(dr_id_e dr_id, DR_BRE_TIME* dr_bre_time_st)
{
    dr_bre_time_st = dr_bre_time_st;
    pmic_print_error("no pmic macro defined,use dummy!");
    return  BSP_PMU_NO_PMIC;
}

/*****************************************************************************
 �� �� ��  : dummy_pmu_init
 ��������  : ��ʼ��Ĭ�ϴ�׮����
 �������  : void
 �������  : ��
 �� �� ֵ  : �� 
 ���ú���  :
 ��������  : LEDģ��
*****************************************************************************/
void dummy_pmu_init(void)
{
    pmic_print_error("no pmic macro defined,use dummy!");
    return  BSP_PMU_NO_PMIC;
}

#if defined (CONFIG_PMIC_HI6451)
struct pmu_adp_ops pmu_adp_ops = {
    .lcd_io_power_switch = dummy_volt_switch,
    .lcd_analog_power_switch= hi6451_ldo14_switch,
    .lcd_analog_volt_set = hi6451_ldo14_volt_set,
    .lcd_bl_cur_set = dummy_volt_set,
    .lcd_bl_power_switch = dummy_volt_switch,
    .efuse_power_switch= dummy_volt_switch,
    .efuse_volt_set = dummy_volt_set,
    .hreset_state_get = hi6451_hreset_state_get,
    .hreset_state_clear = dummy_clear_part_state,
    .usb_state_get = hi6451_usb_state_get,
    .key_state_get = hi6451_power_key_state_get,
    .version_get = hi6451_version_get,
    .get_boot_state = hi6451_get_boot_state,
    .set_by_nv = dummy_set_by_nv_handler,
    .dr_bre_time_set = dummy_dr_bre_time_set,
    .pmu_init = pmu_hi6451_init,
    };
#elif defined (CONFIG_PMIC_HI6551)
struct pmu_adp_ops pmu_adp_ops = {
    .lcd_io_power_switch  = hi6551_lvs4_switch,
    .lcd_analog_power_switch  = hi6551_ldo14_switch,
    .lcd_analog_volt_set = hi6551_ldo14_volt_set,
    .lcd_bl_cur_set = hi6551_lcd_bl_current_set,
    .lcd_bl_power_switch = hi6551_lcd_bl_power_switch,
    .efuse_power_switch= hi6551_ldo23_switch,
    .efuse_volt_set = hi6551_ldo23_volt_set,
    .hreset_state_get = hi6551_hreset_state_get,
    .hreset_state_clear = hi6551_hreset_state_clear,
    .usb_state_get = hi6551_usb_state_get,
    .key_state_get = hi6551_power_key_state_get,
    .version_get = hi6551_version_get,
    .get_boot_state = hi6551_get_boot_state,
    .set_by_nv = hi6551_set_by_nv,
    .dr_bre_time_set = hi6551_dr_bre_time_set,
    .pmu_init = pmu_hi6551_init,
    };
#elif defined (CONFIG_PMIC_HI6559)
struct pmu_adp_ops pmu_adp_ops = {
    .lcd_io_power_switch  = dummy_volt_switch, /* 711��BUCK6 always on�����ܲ��� */
    .lcd_analog_power_switch  = hi6559_ldo14_switch,
    .lcd_analog_volt_set = hi6559_ldo14_volt_set,
    .lcd_bl_cur_set = hi6559_lcd_bl_current_set,
    .lcd_bl_power_switch = hi6559_lcd_bl_power_switch,
    .efuse_power_switch= hi6559_ldo23_switch,
    .efuse_volt_set = hi6559_ldo23_volt_set,
    .hreset_state_get = hi6559_hreset_state_get,
    .hreset_state_clear = hi6559_hreset_state_clear,
    .usb_state_get = hi6559_usb_state_get,
    .key_state_get = hi6559_power_key_state_get,
    .version_get = hi6559_version_get,
    .get_boot_state = hi6559_get_boot_state,
    .set_by_nv = hi6559_set_by_nv,
    .dr_bre_time_set = hi6559_dr_bre_time_set,
    .pmu_init = pmu_hi6559_init,
    };
#else
struct pmu_adp_ops pmu_adp_ops = {
    .lcd_io_power_switch = dummy_volt_switch,
    .lcd_analog_power_switch = dummy_volt_switch,
    .lcd_analog_volt_set = dummy_volt_set,
    .lcd_bl_cur_set = dummy_volt_set,
    .lcd_bl_power_switch = dummy_volt_switch,
    .efuse_power_switch= dummy_volt_switch,
    .efuse_volt_set = dummy_volt_set,
    .hreset_state_get = dummy_get_part_state,
    .hreset_state_clear = dummy_clear_part_state,
    .usb_state_get = dummy_get_part_state,
    .key_state_get = dummy_get_part_state,
    .version_get = dummy_info_get_handler,
    .get_boot_state = dummy_get_boot_state,
    .set_by_nv = dummy_set_by_nv_handler,
    .dr_bre_time_set = dummy_dr_bre_time_set,
    .pmu_init = dummy_pmu_init,
    };
#endif
/*****************************************************************************
 �� �� ��  : bsp_pmu_hreset_state_get
 ��������  : �ж�pmu�Ƿ�Ϊ������
 �������  : void
 �������  : ��
 �� �� ֵ  : pmu����������������(1:��������0:������)
 ���ú���  :
 ��������  : ���ػ�ģ��
*****************************************************************************/
bool bsp_pmu_hreset_state_get(void)
{
    return  pmu_adp_ops.hreset_state_get();
}
/*****************************************************************************
 �� �� ��  : bsp_pmu_hreset_state_get
 ��������  : ���pmu��������־
 �������  : void
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  : ���ػ�ģ��
*****************************************************************************/
void bsp_pmu_hreset_state_clear(void)
{
    return  pmu_adp_ops.hreset_state_clear();
}
/*****************************************************************************
 �� �� ��  : bsp_pmu_version_get
 ��������  : ��ȡusb�Ƿ���״̬
 �������  : void
 �������  : ��
 �� �� ֵ  : usb�����γ�
 ���ú���  :
 ��������  : ���ػ�ģ��
*****************************************************************************/
bool bsp_pmu_usb_state_get(void)
{
    return  pmu_adp_ops.usb_state_get();
}
/*****************************************************************************
 �� �� ��  : bsp_pmu_key_state_get
 ��������  : ��ȡ�����Ƿ���״̬
 �������  : void
 �������  : ��
 �� �� ֵ  : usb�����γ�
 ���ú���  :
 ��������  : ���ػ�ģ��
*****************************************************************************/
bool bsp_pmu_key_state_get(void)
{
    return  pmu_adp_ops.key_state_get();
}
/*****************************************************************************
 �� �� ��  : bsp_pmu_lcd_io_power_switch
 ��������  : ����lcdio�ĵ�ѹԴ(lvs04)
 �������  : power_switch_e sw��POWER_ON:�򿪣�POWER_OFF:�رգ�
 �������  : ��
 �� �� ֵ  : �����ɹ���ʧ��
 ���ú���  :
 ��������  : ��
*****************************************************************************/
int bsp_pmu_lcd_io_power_switch(power_switch_e sw)
{
    return  pmu_adp_ops.lcd_io_power_switch(sw);
}
/*****************************************************************************
 �� �� ��  : bsp_pmu_lcd_analog_power_switch
 ��������  : ����lcdģ���Դ(ldo14)
 �������  : ��
 �������  : ��
 �� �� ֵ  : �����ɹ���ʧ��
 ���ú���  :
 ��������  : fastboot��lcd�ļ�����
*****************************************************************************/
int bsp_pmu_lcd_analog_power_switch(power_switch_e sw)
{
    return  pmu_adp_ops.lcd_analog_power_switch(sw);
}
/*****************************************************************************
 �� �� ��  : bsp_pmu_lcd_analog_volt_set
 ��������  : ����lcdģ���Դ(ldo14)��ѹ
 �������  : int voltage:Ҫ���õĵ�ѹֵ����λuV!
 �������  : ��
 �� �� ֵ  : �����ɹ���ʧ��
 ���ú���  :
 ��������  : fastboot��lcd�ļ�����
*****************************************************************************/
int bsp_pmu_lcd_analog_volt_set(int voltage)
{
    return  pmu_adp_ops.lcd_analog_volt_set(voltage);
}
/*****************************************************************************
 �� �� ��  : bsp_pmu_lcd_bl_power_switch
 ��������  : ����lcd����
 �������  :
 �������  : ��
 �� �� ֵ  : �����ɹ���ʧ��
 ���ú���  :
 ��������  : fastboot��lcd�ļ�����
*****************************************************************************/
int bsp_pmu_lcd_bl_power_switch(power_switch_e sw)
{
    return  pmu_adp_ops.lcd_bl_power_switch(sw);
}
/*****************************************************************************
 �� �� ��  : bsp_pmu_lcd_analog_volt_set
 ��������  : ����lcd����ĵ���
 �������  : int cur:Ҫ���õĵ���ֵ����λuA!
 �������  : ��
 �� �� ֵ  : �����ɹ���ʧ��
 ���ú���  :
 ��������  : fastboot��lcd�ļ�����
*****************************************************************************/
int bsp_pmu_lcd_bl_current_set(int cur)
{
    return  pmu_adp_ops.lcd_bl_cur_set(cur);
}
/*****************************************************************************
 �� �� ��  : bsp_pmu_efuse_power_switch
 ��������  : ����efuse��Դ(ldo23)
 �������  : ��
 �������  : ��
 �� �� ֵ  : �����ɹ���ʧ��
 ���ú���  :
 ��������  : fastboot��efuse�ļ�����
*****************************************************************************/
int bsp_pmu_efuse_power_switch(power_switch_e sw)
{
    return  pmu_adp_ops.efuse_power_switch(sw);
}
/*****************************************************************************
 �� �� ��  : bsp_pmu_efuse_volt_set
 ��������  : ����lcdģ���Դ(ldo14)��ѹ
 �������  : int voltage:Ҫ���õĵ�ѹֵ����λuV!
 �������  : ��
 �� �� ֵ  : �����ɹ���ʧ��
 ���ú���  :
 ��������  : fastboot��lcd�ļ�����
*****************************************************************************/
int bsp_pmu_efuse_volt_set(int voltage)
{
    return  pmu_adp_ops.efuse_volt_set(voltage);
}
/*****************************************************************************
 �� �� ��  : bsp_pmu_version_get
 ��������  : ��ȡpmu�汾��
 �������  : void
 �������  : ��
 �� �� ֵ  : pmu�汾��
 ���ú���  :
 ��������  : ����hso��msp����
*****************************************************************************/
u8 bsp_pmu_version_get(void)
{
    u8 version = 0;
    version = pmu_adp_ops.version_get();
    return version;
}
/*****************************************************************************
 �� �� ��  : bsp_pmu_get_boot_state
 ��������  :ϵͳ����ʱ���pmu�Ĵ���״̬��
                ȷ���Ƿ�����pmu���������
 �������  : void
 �������  : reset.log
 �� �� ֵ  : pmu�����ok
 ���ú���  :
 ��������  :ϵͳ��ά�ɲ�
*****************************************************************************/
int bsp_pmu_get_boot_state(void)
{
    return  pmu_adp_ops.get_boot_state();
}

/*****************************************************************************
 �� �� ��  : bsp_pmu_default_set
 ��������  : PMUģ�����nv����������Ĭ�ϵ�ѹ
 �������  : void
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  : fastboot main
*****************************************************************************/
void bsp_pmu_set_by_nv(void)
{
    pmu_adp_ops.set_by_nv();
}
/*****************************************************************************
 �� �� ��  : bsp_dr_bre_time_set
 ��������  : ����dr�ĺ���ʱ��(��������������������������ʱ��)
 �������  : dr_id:Ҫ���õĵ���Դ���;dr_bre_time_st:����ʱ������ṹ��;
            û���ҵ����õ�ʱ�䣬������Ϊ����
 �������  : ��
 �� �� ֵ  : ���óɹ�����ʧ��
 ���ú���  :
 ��ע˵��  : ��Ӧ�Ĵ���ֻ������ʱ��Ĺ��ܣ�����Ҫ������
*****************************************************************************/
int bsp_dr_bre_time_set(dr_id_e dr_id, DR_BRE_TIME *dr_bre_time_st)
{
    return  pmu_adp_ops.dr_bre_time_set(dr_id,dr_bre_time_st);
}
/*****************************************************************************
 �� �� ��  : bsp_pmu_init
 ��������  : pmuģ���ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  : fastboot��pmuģ���ʼ��ʱ����
*****************************************************************************/
void bsp_pmu_init(void)
{
    pmu_adp_ops.pmu_init();
}

/*��������־����*/
void bsp_pmu_hreset_test(void)
{
    bool state = 0;
    state = bsp_pmu_hreset_state_get();
    pmic_print_error("bsp_pmu_hreset_state is %d!",state);
    bsp_pmu_hreset_state_clear();
}