
#include <linux/leds.h>
#include <linux/mutex.h>
#include <drv_leds.h>
#include <drv_nv_def.h>
#include <drv_nv_id.h>
#include <bsp_om.h>
#include <bsp_pmu.h>
#include <bsp_regulator.h>

/* LED id */
#define LED1_ID     PMU_DR01
#define LED2_ID     PMU_DR02
#define LED3_ID     PMU_DR03
#define LED4_ID     PMU_DR04
#define LED5_ID     PMU_DR05
#define LED_ID_MIN  LED1_ID
#define LED_ID_MAX  LED5_ID

/* LED name, for balong_ledX, X must not be less than LED_ID_MIN and bigger than LED_ID_MAX */
#define LED1_NAME    H6551_DR1_NM   /* ��������Ʊ�����regulator��nameһ�£�Ӧʹ�ú궨�� */
#define LED2_NAME    H6551_DR2_NM
#define LED3_NAME    H6551_DR3_NM
#define LED4_NAME    H6551_DR4_NM
#define LED5_NAME    H6551_DR5_NM

/* trigger name */
#define LED_TRIGGER_TIMER       "timer"     /* defined in timer_led_trigger, ledtrig-timer.c */
#define LED_TRIGGER_BREATH      "breath"    /* defined in breath_led_trigger, ledtrig-breath.c */

/* ����sysfsϵͳ��ʾ����ģʽ����ʱ���������ַ����ĳ��� */
#define LED_SYSFS_MAX_STR_LENTH   8

/* some important structure */
struct balong_led_platdata {
    char *name;
    unsigned long full_on, full_off, fade_on, fade_off;
    int	(*led_breath_set)(struct led_classdev *led_cdev, unsigned long *full_on, unsigned long *full_off, 
        unsigned long *fade_on, unsigned long *fade_off);
    struct regulator *pregulator;
	struct mutex		mlock;
};

struct balong_led_device{
     struct led_classdev cdev;
     struct balong_led_platdata *pdata;
};

/******************************************************************************************/
/* ��ɫ����صĺ� */

/* ��ɫֵ,0λ��ʾ��,1λ��ʾ��,2λ��ʾ��,����λ����
��Ϊ��+��,��Ϊ��+��,��Ϊ��+��*/

enum LED_COLOR
{
    LED_NULL    = 0x0,
    LED_BLUE    = 0x1,
    LED_GREEN   = 0x2,
    LED_CYAN    = 0x3,
    LED_RED     = 0x4,
    LED_PURPLE  = 0x5,
    LED_YELLOW  = 0x6,
    LED_WHITE   = 0x7
};

/* ��ɫ��Ĭ��״̬����*/
#define LED_GREEN_LIGHTING_DOUBLE               {{LED_GREEN, 1},{LED_NULL,1},{LED_GREEN, 1},{LED_NULL, 17},{0,0}}/*�̵�˫��*/
#define LED_GREEN_LIGHTING_SIGNAL               {{LED_GREEN, 1},{LED_NULL, 19},{0,0}}/*�̵Ƶ���*/
#define LED_GREEN_LIGHTING_FREQUENTLY           {{LED_GREEN, 1},{LED_NULL,1},{0,0}}/*�̵�Ƶ��*/
#define LED_GREEN_LIGHTING_ALWAYS               {{LED_GREEN, 1},{0,0}}/*�̵Ƴ���*/

#define LED_BULE_LIGHTING_SIGNAL                {{LED_BLUE, 1},{LED_NULL, 19},{0,0}}/*���Ƶ���*/
#define LED_BLUE_LIGHTING_FREQUENTLY            {{LED_BLUE, 1},{LED_NULL,1},{0,0}}/*����Ƶ��*/
#define LED_BLUE_LIGHTING_ALWAYS                {{LED_BLUE, 1},{0,0}}/*���Ƴ���*/

#define LED_CYAN_LIGHTING_SIGNAL                {{LED_CYAN, 1},{LED_NULL, 19},{0,0}}/*��Ƶ���*/
#define LED_CYAN_LIGHTING_ALWAYS                {{LED_CYAN, 1},{0,0}}/*��Ƴ���*/

#define LED_BLUE_GREEN_ALTERNATE                {{LED_BLUE, 1},{LED_NULL,1},{LED_GREEN, 1},{LED_NULL, 17},{0,0}}/*���̽���˫��*/  

#define LED_RED_LIGHTING_DOUBLE                 {{LED_RED, 1},{LED_NULL,1},{LED_RED, 1},{LED_NULL, 17},{0,0}}/*���˫��*/
#define LED_RED_LIGHTING_SIGNAL                 {{LED_RED, 1},{LED_NULL, 19},{0,0}}/*��Ƶ���*/
#define LED_RED_LIGHTING_ALWAYS                 {{LED_RED, 1},{0,0}}/*��Ƴ���*/

#define LED_SHUTDOWN_DEFINE                     {{LED_NULL, 1},{0,0}}/*����*/

/* some macro */
#define LED_NV_ALREADY_READ                     (0x0F0F0F0F)  /* nv already read flag */
#define MNTN_LED_STATUS_FLUSH                   (0x1)
#define MNTN_LED_TIMER_OCCURE                   (0x2)

#define RED_ON                                  led_on(LED3_ID - 1)
#define RED_OFF                                 led_off(LED3_ID - 1)
#define GREEN_ON                                led_on(LED4_ID - 1)
#define GREEN_OFF                               led_off(LED4_ID - 1)
#define BLUE_ON                                 led_on(LED5_ID - 1)
#define BLUE_OFF                                led_off(LED5_ID - 1)

 /* ��ɫ������״̬���и���*/
#define LED_CONFIG_MAX_LTE 10

#define LED_OM_CONFIG {LED_GREEN_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_SIGNAL,\
                                LED_BULE_LIGHTING_SIGNAL,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_CYAN_LIGHTING_ALWAYS,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_ALWAYS,\
                                LED_BLUE_LIGHTING_ALWAYS,\
                                LED_BLUE_GREEN_ALTERNATE,\
                                LED_BLUE_LIGHTING_FREQUENTLY,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_GREEN_LIGHTING_DOUBLE,\
                                LED_SHUTDOWN_DEFINE,\
                                LED_CYAN_LIGHTING_SIGNAL,\
                                LED_GREEN_LIGHTING_DOUBLE}

/* ��ɫ�Ƶ�ǰ״̬����*/
struct led_param
{
	unsigned char led_state_id;     /* ��ɫ��״̬����ӦLED_STATUS */
	unsigned char led_config_id;    /* ����״̬����ֵ */
	unsigned char led_color_id;     /* ��ɫ����ɫ����ӦLED_COLOR��ֵ */
	unsigned char led_time;         /* �����ó�����ʱ�䳤�ȣ���λ100ms */
};

/*****************************************************************************
 �ṹ��    : LED_CONTROL_NV_STRU
 �ṹ˵��  : LED_CONTROL_NV�ṹ ID=7
*****************************************************************************/
typedef struct
{
    unsigned char   ucLedColor;      /*��ɫ����ɫ����ӦLED_COLOR��ֵ*/
    unsigned char   ucTimeLength;    /*�����ó�����ʱ�䳤�ȣ���λ100ms*/
}LED_CONTROL_NV_STRU;

/* led nv�ṹ�壬����nv���Ѷ���� */
struct nv_led{
    LED_CONTROL_NV_STRU g_led_state_str_om[LED_LIGHT_STATE_MAX][LED_CONFIG_MAX_LTE];
    int g_already_read;
};

struct led_tled_arg
{
    unsigned char ctl;
    unsigned char new_state;
};

void balong_led_brightness_set(struct led_classdev *led_cdev, enum led_brightness brightness);
int led_threecolor_flush(u32 channel_id , u32 len, void* context);
int do_led_threecolor_flush(void);
int balong_led_breath_set(struct led_classdev *led_cdev, unsigned long *full_on, unsigned long *full_off,
                                        unsigned long *fade_on, unsigned long *fade_off);

