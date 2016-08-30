

#ifndef _DSM_PUB_H
#define _DSM_PUB_H
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/wait.h>

#define CLIENT_NAME_LEN		(32) //max client name length

/*dsm error no define*/
#define DSM_ERR_NO_ERROR					(0)
#define DSM_PMU_IRQ_ERROR_NO				(10000)
#define DSM_PMU_OCP_ERROR_NO_BASE			(10100)
#define DSM_CHARGER_ERROR_NO  				(10200)
#define DSM_PMU_COUL_ERROR_NO				(10300)
#define DSM_BATTERY_ERROR_NO				(10300)
#define DSM_BATTERY_DETECT_ERROR_NO				(10400)
#define DSM_TP_I2C_RW_ERROR_NO 				(20000)
#define DSM_TP_FWUPDATE_ERROR_NO 			(20001)
#define DSM_TP_NO_IRQ_ERROR_NO 				(20002)
#define DSM_LCD_LDI_UNDERFLOW_ERROR_NO     	(20100)
#define DSM_LCD_TE_TIME_OUT_ERROR_NO		(20101)
#define DSM_LCD_STATUS_ERROR_NO				(20102)
#define DSM_LCD_POWER_STATUS_ERROR_NO		(20103)
#define DSM_LCD_PWM_ERROR_NO				(20104)
#define DSM_LCD_BRIGHTNESS_ERROR_NO			(20105)
#define DSM_CODEC_ERROR_NO     	 			(20200)
#define DSM_HIFI_ERROR_NO					(20201)
#define ANC_HS_OPEN_BOOST_ERR               (20202)
#define ANC_HS_CANCEL_WORK_BLOCK            (20203)
#define ANC_HS_CLOSE_BOOST_ERR              (20204)
#define ANC_HS_ADCH_READ_ERR                (20205)
#define ANC_HS_ADCL_READ_ERR                (20206)
#define ANC_HS_MIC_WITH_GPIO_ERR            (20207)
#define ANC_HS_QUEUE_WORK_ERR               (20208)
#define DSM_SDIO_RW_ERROR_NO				(20300)
#define DSM_ERR_SENSORHUB_IPC_TIMEOUT		(20400)
#define DSM_ERR_SENSORHUB_LOG_FATAL			(20401)
#define DSM_KEY_ERROR_NO					(20500)
#define DSM_POWER_KEY_ERROR_NO				(20501)
#define DSM_POWER_KEY_LONG_PRESS_ERROR_NO	(20502)
#define DSM_EXTISP_ERROR_NO                 (20603)
#define DSM_FLASH_OPEN_SHOTR_ERROR_NO                 (20607)
#define DSM_FLASH_HOT_DIE_ERROR_NO                 (20608)
#define DSM_WIFI_ERROR_NO                   (20700)
#define DSM_STORAGE_EXT4_ERROR_NO			(20701)
struct dsm_client_ops{
	int (*poll_state) (void);
	int (*dump_func) (int type, void *buff, int size);
};

struct dsm_dev{
	const char *name;
	struct dsm_client_ops *fops;
	size_t buff_size;
};

struct dsm_client{
	char client_name[CLIENT_NAME_LEN];
	int client_id;
	int error_no;
	unsigned long buff_flag;
	struct dsm_client_ops *cops;
	wait_queue_head_t waitq;
	size_t read_size;
	size_t used_size;
	size_t buff_size;
	u8 dump_buff[];
};

#ifdef CONFIG_HUAWEI_DSM
struct dsm_client *dsm_register_client (struct dsm_dev *dev);
int dsm_client_ocuppy(struct dsm_client *client);
int dsm_client_unocuppy(struct dsm_client *client);
int dsm_client_record(struct dsm_client *client, const char *fmt, ...);
int dsm_client_copy(struct dsm_client *client, void *src, int sz);
void dsm_client_notify(struct dsm_client *client, int error_no);
#else
static inline struct dsm_client *dsm_register_client (struct dsm_dev *dev)
{
	return NULL;
}
static inline int dsm_client_ocuppy(struct dsm_client *client)
{
	return 1;
}
static inline int dsm_client_unocuppy(struct dsm_client *client)
{
	return 0;
}

static inline int dsm_client_record(struct dsm_client *client, const char *fmt, ...)
{
	return 0;
}
static inline int dsm_client_copy(struct dsm_client *client, void *src, int sz)
{
	return 0;
}
static inline void dsm_client_notify(struct dsm_client *client, int error_no)
{
	return;
}
#endif

#endif
