/*
 * Huawei Touchpanel driver
 *
 * Copyright (C) 2013 Huawei Device Co.Ltd
 * License terms: GNU General Public License (GPL) version 2
 *
 */
#ifndef __HUAWEI_TOUCHSCREEN_H_
#define __HUAWEI_TOUCHSCREEN_H_

#include <linux/version.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/completion.h>
#include <linux/kernel.h>
#include <linux/of.h>
#if defined(CONFIG_HAS_EARLYSUSPEND)
#include <linux/earlysuspend.h>
#endif
#ifndef CONFIG_OF
#include <hsad/config_mgr.h>
#endif
#include <linux/debugfs.h>
#include <linux/platform_device.h>
#include <linux/hw_log.h>

/* marco define*/
#define NO_ERR 0

#define TS_DEV_NAME "huawei,touchscreen"
#define RAW_DATA_SIZE 8192
#define TS_WATCHDOG_TIMEOUT		1000

#define NO_SYNC_TIMEOUT		0
#define SHORT_SYNC_TIMEOUT		5
#define LONG_SYNC_TIMEOUT		10
#define LONG_LONG_SYNC_TIMEOUT	50

#define TS_CMD_QUEUE_SIZE  20
#define TS_MAX_FINGER 10
#define TS_ERR_NEST_LEVEL  5
#define TS_RAWDATA_BUFF_MAX 1500
#define TS_RAWDATA_RESULT_MAX	50

#define I2C_WAIT_TIME 25 //25ms wait period
#define I2C_RW_TRIES 3 //retry 3 times
#define I2C_DEFAULT_ADDR 0x70
#define TS_SUSPEND_LEVEL 1
#define TS_MAX_REG_VALUE_NUM 16

#define TS_NO_KEY_PRESS  (0)
#define TS_IO_UNDEFINE  (-1)
#define TS_IRQ_CFG_UNDEFINE  (-1)

#define TS_FINGER_SUPPRESS		(1 << 1)
#define TS_FINGER_AMP			(1 << 2)
#define TS_FINGER_VECTOR		(1 << 3)
#define TS_FINGER_MOVE		(1 << 4)
#define TS_FINGER_RELEASE		(1 << 5)
#define TS_FINGER_PRESS		(1 << 6)
#define TS_FINGER_DETECT		(1 << 7)

#define TS_ALGO_FUNC_0		(1<<0)
#define TS_ALGO_FUNC_1		(1<<1)
#define TS_ALGO_FUNC_2		(1<<2)
#define TS_ALGO_FUNC_3		(1<<3)
#define TS_ALGO_FUNC_4		(1<<4)
#define TS_ALGO_FUNC_5		(1<<5)
#define TS_ALGO_FUNC_6		(1<<6)
#define TS_ALGO_FUNC_7		(1<<7)
#define TS_ALGO_FUNC_ALL	(0xFF)

#define TS_GESTURE_ENABLE(x)  ((u32)(1<<(x)))
#define MAX_POSITON_NUMS 6

#define MAX_STR_LEN 32
#define FULL_NAME_MAX_LEN 	128

#define CHIP_INFO_LENGTH	16
#define RAWDATA_NUM 8

#ifndef CONFIG_OF
#define SUPPORT_IC_NUM 4
#define IC_NAME_UNDEF "null"
#define CHIP_FULL_NAME "touchscreen/support_chip_name_"
#define CHIP_SLAVE_ADDR_NAME "/slave_address"
#define INT_CONVERT_OFFSET	49
#endif

#define HWLOG_TAG	TS
HWLOG_REGIST();
#define TS_LOG_INFO(x...)		_hwlog_info(HWLOG_TAG, ##x)
#define TS_LOG_ERR(x...)		_hwlog_err(HWLOG_TAG, ##x)
#define TS_LOG_DEBUG(x...)	\
do { \
	if (g_ts_log_cfg)	\
		_hwlog_info(HWLOG_TAG, ##x);	\
}while(0)

/* external varible*/
extern u8 g_ts_log_cfg;

/* struct define*/
enum ts_cmd;
enum ts_bus_type;
enum ts_irq_config;
enum ts_action_status;
enum ts_dev_state;
struct ts_finger;
struct ts_fingers;
struct ts_algo_func;
struct algo_param;
struct fw_param;
struct ts_rawdata_info;
struct ts_chip_info_param;
struct ts_calibrate_info;
struct ts_glove_info;
struct ts_holster_info;
struct ts_hand_info;
struct ts_feature_info;
struct ts_dsm_info;
struct ts_cmd_param;
struct ts_cmd_node;
struct ts_cmd_queue;
struct ts_device_ops;
struct ts_device_data;
struct ts_bus_info;
struct ts_data;
struct ts_rs_info;

enum ts_cmd{
	TS_INT_PROCESS = 0,
	TS_INPUT_ALGO,
	TS_REPORT_INPUT,
	TS_POWER_CONTROL,
	TS_FW_UPDATE_BOOT,
	TS_FW_UPDATE_SD,
	TS_GET_CHIP_INFO,
	TS_READ_RAW_DATA,
	TS_CALIBRATE_DEVICE,
	TS_DSM_DEBUG,
	TS_GLOVE_SWITCH,
	TS_HAND_DETECT,
	TS_FORCE_RESET,
	TS_INT_ERR_OCCUR,
	TS_ERR_OCCUR,
	TS_CHECK_STATUS,
	TS_TEST_CMD,
	TS_HOLSTER_SWITCH,
	TS_TOUCH_WINDOW,
	TS_PALM_SWITCH,
	TS_REGS_STORE,
	TS_INVAILD_CMD = 255,
};

enum ts_pm_type {
	TS_BEFORE_SUSPEND = 0,
	TS_SUSPEND_DEVICE,
	TS_RESUME_DEVICE,
	TS_AFTER_RESUME,
};

enum ts_bus_type{
	TS_BUS_I2C = 0,
	TS_BUS_SPI,
	TS_BUS_UNDEF = 255,
};

enum ts_irq_config{
	TS_IRQ_LOW_LEVEL,
	TS_IRQ_HIGH_LEVEL,
	TS_IRQ_RAISE_EDGE,
	TS_IRQ_FALL_EDGE,
};

enum ts_action_status{
	TS_ACTION_READ,
	TS_ACTION_WRITE,
	TS_ACTION_SUCCESS,
	TS_ACTION_FAILED,
	TS_ACTION_UNDEF,
};

enum ts_dev_state{
	TS_UNINIT = 0,
	TS_SLEEP,
	TS_WORK,
	TS_WORK_IN_SLEEP,
	TS_STATE_UNDEFINE = 255,
};

enum ts_gesture_num {
//	TS_NUM_TOTAL = 12, /* total gesture numbers  */
	TS_DOUBLE_CLICK = KEY_F1, /*0.Double tap:KEY_F1*/
	TS_SLIDE_L2R = KEY_F2, /*1.Single finger slide from left to right:KEY_F2*/
	TS_SLIDE_R2L = KEY_F3, /*2.Single finger slide from right to left:KEY_F3*/
	TS_SLIDE_T2B = KEY_F4, /*3.Single finger slide from top to bottom:KEY_F4*/
	TS_SLIDE_B2T = KEY_F5, /*4.Single finger slide from bottom to top:KEY_F5*/
	TS_CIRCLE_SLIDE = KEY_F6, /*5.Single finger slide circle:KEY_F6*/
	TS_LETTER_c = KEY_F7, /*6.Single finger write letter c:KEY_F7*/
	TS_LETTER_e = KEY_F8, /*7.Single finger write letter e:KEY_F8*/
	TS_LETTER_m = KEY_F9, /*8.Single finger write letter m:KEY_F9*/
	TS_LETTER_w = KEY_F10, /*9.Single finger write letter w:KEY_F10*/
	TS_PALM_COVERED = KEY_F11, /*10.Palm off screen:KEY_F11*/
	TS_GESTURE_INVALID = 0xFF,/*FF.No gesture*/
};

enum ts_touchplus_num {
	TS_TOUCHPLUS_KEY0 = KEY_F21,
	TS_TOUCHPLUS_KEY1 = KEY_F22,
	TS_TOUCHPLUS_KEY2 = KEY_F23,
	TS_TOUCHPLUS_KEY3 = KEY_F19,
	TS_TOUCHPLUS_KEY4 = KEY_F20,
	TS_TOUCHPLUS_INVALID = 0xFF,
};

enum ts_gesture_enable_bit {
	GESTURE_DOUBLE_CLICK = 0,
	GESTURE_SLIDE_L2R,
	GESTURE_SLIDE_R2L,
	GESTURE_SLIDE_T2B,
	GESTURE_SLIDE_B2T,
	GESTURE_CIRCLE_SLIDE,
	GESTURE_LETTER_c,
	GESTURE_LETTER_e,
	GESTURE_LETTER_m,
	GESTURE_LETTER_w,
	GESTURE_PALM_COVERED,
	GESTURE_MAX,
	GESTURE_LETTER_ENABLE = 29,
	GESTURE_SLIDE_ENABLE = 30,
};

struct ts_finger{
	int status;
	int x;
	int y;
	int area;
	int pressure;
	int orientation;
	int major;
	int minor;
	int event;
};

struct ts_fingers{
	struct ts_finger fingers[TS_MAX_FINGER];
	int cur_finger_number;
	int gesture_wakeup_command;
	unsigned int gesture_wakeup_value;
	unsigned int special_button_key;
	unsigned int special_button_flag;
};

struct ts_algo_func{
	int algo_index; //from 0 to max
	char *algo_name;
	struct list_head node;
	int (*chip_algo_func) (struct ts_device_data *dev_data, struct ts_fingers *in_info, struct ts_fingers *out_info);
};

struct algo_param{
	u32 algo_order;
	struct ts_fingers info;
}; 

struct fw_param{
	char fw_name[MAX_STR_LEN*4]; //firmware name contain 4 parts
};

struct ts_rawdata_info{
	int status;
	int op_action;
	int used_size; // fill in rawdata size
	ktime_t time_stamp;
	int buff[TS_RAWDATA_BUFF_MAX];
	char result[TS_RAWDATA_RESULT_MAX];
};

struct ts_chip_info_param{
	int status;
	u8 chip_name[CHIP_INFO_LENGTH*2];
	u8 ic_vendor[CHIP_INFO_LENGTH*2];
	u8 fw_vendor[CHIP_INFO_LENGTH*2];
	u16 ttconfig_version;
	u8 fw_verctrl_num[CHIP_INFO_LENGTH];
};

struct ts_calibrate_info{
	int status;
};

struct ts_dsm_debug_info{
	int status;
};

struct ts_glove_info{
	u8 glove_switch;
	int op_action;
	int status;
};

struct ts_holster_info{
	u8 holster_switch;
	u8 writen_down_cmd;
	int op_action;
	int status;
};

struct ts_regs_info{
	unsigned int fhandler;
	int type;
	int offset;
	u8 values[TS_MAX_REG_VALUE_NUM];
	int num;
	u8 op_action;
	int status;
};

struct ts_window_info{
	int window_enable;
	int top_left_x0;
	int top_left_y0;
	int bottom_right_x1;
	int bottom_right_y1;
	int status;
};

struct ts_dsm_info{
	char fw_update_result[8];
	u8 constraints_LDO17_status;
	u8 constraints_LSW50_status;
};

struct ts_hand_info{
	u8 hand_value;
	int op_action;
	int status;
};

struct ts_feature_info {
	struct ts_holster_info holster_info;
	struct ts_window_info window_info;
};

struct ts_cmd_param{
	union{
		struct algo_param algo_param; //algo cal
		struct ts_fingers report_info; //report input
		struct fw_param firmware_info; //firmware update
		enum ts_pm_type pm_type;
	}pub_params;
	void *prv_params;
};

enum ts_timeout_flag {
	TS_TIMEOUT = 0,
	TS_NOT_TIMEOUT,
	TS_UNDEF = 255,
};

enum ts_sleep_mode {
	TS_POWER_OFF_MODE = 0,
	TS_GESTURE_MODE,
};

struct ts_rs_info {
	enum ts_sleep_mode sleep_mode;
	bool rs_flag;
	u32 ts_gesture_command_data;
	bool gesture_palm_set_flag;
	unsigned int easywake_position[MAX_POSITON_NUMS];
};
struct ts_palm_info{
	u8 palm_switch;
	int op_action;
	int status;
};

struct ts_cmd_sync {
	atomic_t timeout_flag;
	struct completion done;
};

struct ts_cmd_node{
	enum ts_cmd command;
	struct ts_cmd_sync * sync;
	struct ts_cmd_param cmd_param;
};

struct ts_cmd_queue{
	int wr_index;
	int rd_index;
	int cmd_count;
	int queue_size;
	spinlock_t spin_lock;
	struct ts_cmd_node ring_buff[TS_CMD_QUEUE_SIZE];
};

struct ts_device_ops{
	int (*chip_detect)(struct device_node *device, struct ts_device_data *data, struct platform_device *ts_dev);
	int (*chip_init)(void);
	int (*chip_parse_config)(struct device_node *device, struct ts_device_data *data);
	int (*chip_input_config)(struct input_dev *input_dev);
	int (*chip_register_algo)(struct ts_device_data *data);
	int (*chip_irq_top_half)(struct ts_cmd_node *cmd);
	int (*chip_irq_bottom_half)(struct ts_cmd_node *in_cmd, struct ts_cmd_node *out_cmd);
	int (*chip_reset)(void);
	void (*chip_shutdown)(void);
	int (*chip_get_info)(struct ts_chip_info_param *info);
	int (*chip_fw_update_boot)(char *file_name);
	int (*chip_fw_update_sd)(void);
	int (*chip_calibrate)(void);
	int (*chip_dsm_debug)(void);
	int (*chip_get_rawdata)(struct ts_rawdata_info *info, struct ts_cmd_node *out_cmd); 
	int (*chip_glove_switch)(struct ts_glove_info *info);
	int (*chip_palm_switch)(struct ts_palm_info *info);
	int (*chip_holster_switch)(struct ts_holster_info *info);
	int (*chip_hand_detect)(struct ts_hand_info *info);
	int (*chip_before_suspend)(void);
	int (*chip_suspend)(void);
	int (*chip_resume)(void);
	int (*chip_after_resume)(void *r_data);
	int (*chip_test)(struct ts_cmd_node *in_cmd, struct ts_cmd_node *out_cmd);
	int (*chip_check_status)(void);
	int (*chip_hw_reset)(void);
	int (*chip_regs_operate)(struct ts_regs_info *info);
};

struct ts_device_data{
	bool is_in_cell;
	char chip_name[MAX_STR_LEN];
	char module_name[MAX_STR_LEN];
	char version_name[MAX_STR_LEN];
	int raw_limit_buf[RAWDATA_NUM];
	u8 reg_values[TS_MAX_REG_VALUE_NUM];
	struct device_node *cnode;
	int slave_addr;
	int irq_gpio;
	int irq_config; // 0 - LOW LEVEL  1 - HIGH LEVEL  2 - RAISE EDGE  3 - FALL EDGE
	int reset_gpio;
	int algo_size;
	int algo_id;
	int reg_num;
	struct ts_device_ops *ops;
	void * prv_data;
	struct ts_bus_info *bops;
	struct i2c_client *client;		//client need to be used  modified by mengkun
	struct list_head algo_head;//algo function list
	struct ts_data *ts_data;
	struct ts_rs_info rs_info;
};

struct ts_bus_info{
	enum ts_bus_type btype;
	int bus_id;
	int (*bus_write) (u8 *buf, u16 length);
	int (*bus_read) (u8 *reg_addr, u16 reg_len, u8 *buf, u16 len);
};

struct ts_data{
	char product_name[MAX_STR_LEN];
	int dev_id; //reserved for more ts devices in handset
	int irq_id;
	atomic_t state;
	struct device_node *node;
	struct i2c_client *client;
	struct input_dev *input_dev;
	struct task_struct *ts_task;
	struct ts_device_data *chip_data;
	struct ts_bus_info *bops;
	struct platform_device *ts_dev;
#if defined(CONFIG_FB)
	struct notifier_block fb_notify;
#elif defined(CONFIG_HAS_EARLYSUSPEND)
	struct early_suspend early_suspend;
#endif
	struct ts_cmd_queue queue;
	struct timer_list watchdog_timer;
	struct work_struct watchdog_work;
	struct ts_feature_info feature_info;
	struct ts_dsm_info dsm_info;
};

int ts_power_control_notify(enum ts_pm_type pm_type,  int timeout);
int put_one_cmd(struct ts_cmd_node *cmd, int timeout);
int register_algo_func(struct ts_device_data *chip_data, struct ts_algo_func *fn);
 #ifdef HUAWEI_TOUCHSCREEN_TEST 
int test_dbg_cmd_test(struct ts_cmd_node *in_cmd, struct ts_cmd_node *out_cmd);
#endif

#endif

