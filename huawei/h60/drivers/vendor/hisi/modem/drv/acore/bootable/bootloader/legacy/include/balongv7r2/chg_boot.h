#ifndef _CHG_BOOT_H_
#define _CHG_BOOT_H_

/* ��ʱ�ļ�������������ɾ�� */

typedef enum
{
    CHARGE_BATTERY_GOOD = 0,/*���/charge ��λ�����ҵ���Ǻõ� voltage >= 3.0. go on  **/
    CHARGE_BATTERY_ONLY,    /*�����λ(no charge)�����Ҵﵽ��������( >= 3.4v), go on*/
    CHARGE_BATTERY_LOWER,  /* �����(no charger) ==> power off. < 3.4   */
    CHARGE_BATTERY_LOWER_WITH_CHARGE, /* �����(charger) ==> power off. < 3.4   */
    CHARGE_BATTERY_BAD,     /*�����: charger ==> Wait  show message, only wait  charge be remove, then power off.  **/
    BATTERY_BAD_WITHOUT_CHARGE,/* �����ʱ�γ�charger */
    CHARGE_BATTERY_OVER_TEMP, /* ���� wait resume */
    CHARGE_BATTERY_MAX
}chg_batt_check_type;

chg_batt_check_type chg_battery_check_boot();
int display_logo(void);
void display_string(unsigned char* string);
void display_upgrade();
void boot_show_battery_bad();
void boot_show_battery_over_tmp();
void boot_show_battery_low();

#endif
