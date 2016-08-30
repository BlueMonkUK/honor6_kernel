/*
  * hisilicon efuse driver, hisi_efuse.c
  *
  * Copyright (c) 2013 Hisilicon Technologies CO., Ltd.
  *
  */
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <asm/delay.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/smp.h>
#include "hisi_efuse.h"
#include <linux/regulator/consumer.h>

#ifdef CONFIG_TZDRIVER
#include <teek_client_api.h>
#include <teek_client_id.h>
#endif

#define EFUSE_DEV_NAME "efuse"

struct efuse_data{
	struct regulator_bulk_data regu_burning;
	struct mutex	lock;
};

static struct efuse_data g_efuse_data;

static void __iomem *SCTRL_BASE = NULL;
static void __iomem *PMURTC_BASE = NULL;
static void __iomem *EFC_BASE = NULL;

#ifndef CONFIG_TZDRIVER

#define EFUSEC_CFG(r)			(r+0x0)
#define EFUSEC_PG_EN_MASK		(1<<0)
#define EFUSEC_PRE_PG_MASK		(1<<1)
#define EFUSEC_RD_EN_MASK		(1<<2)
#define EFUSEC_AIB_SIG_SELN_MASK	(1<<3)
#define EFUSEC_RREN_MASK		(1<<4)
#define EFUSEC_PD_EN_MASK		(1<<5)
#define EFUSEC_MR_EN_MASK		(1<<6)

#define EFUSEC_STATUS(r)		(r+0x04)
#define EFUSEC_PG_STATUS_MASK		(1<<0)
#define EFUSEC_RD_STATUS_MASK		(1<<1)
#define EFUSEC_PGENAB_STATUS_MASK	(1<<2)
#define EFUSEC_RD_ERROR_MASK		(1<<3)
#define EFUSEC_PD_STATUS_MASK		(1<<4)

#define EFUSEC_GROUP(r)		(r+0x08)
#define EFUSEC_GROUP_MASK		((1<<6)-1)

#define EFUSEC_PG_VALUE(r)		(r+0x0C)
#define EFUSEC_COUNT(r)		(r+0x10)
#define EFUSE_COUNT_MASK		((1<<8)-1)

#define EFUSEC_PGM_COUNT(r)		(r+0x14)
#define EFUSE_PGM_COUNT_MASK		((1<<16)-1)

#define EFUSEC_DATA(r)			(r+0x18)
#define EFUSEC_HW_CFG(r)		(r+0x1C)

#define EFUSEC_PG_EN			(1<<0)
#define EFUSEC_PRE_PG			(1<<1)
#define EFUSEC_RD_EN			(1<<2)
#define EFUSEC_APB_SIG_SEL		(1<<3)
#define EFUSEC_RR_EN			(1<<4)
#define EFUSEC_PD_EN			(1<<5)
#define EFUSEC_MR_EN			(1<<6)

#define EFUSEC_APB_PGM_DISABLE_MASK	(1<<0)

#define EFUSE_OP_TIMEOUT_COUNT	0x20
#define EFUSE_OP_TIMEOUT		50

#define SCPERCTRL0(r)			(r+0x200)
#define SCPERCTRL0_REMAP_DIS		0x0C9B
#define SCPERCTRL0_REMAP_EN		0x10C9B
#define SCEFUSECTRL(r)			(r+0x83C)
#define SYS_EFUSE_PAD_SEL		0x0
#define SYS_EFUSE_SOFT_SEL		0x1

#define SCPEREN1(r)			(r+0x030)
#define SCPERDIS1(r)			(r+0x034)
#define GT_PCLK_EFUSEC			(0x01UL<<30)


/* ��дʱ��Ҫ��, hi3630 asic�汾������Ĭ��ֵ
*  (EFUSEC_COUNT<<2)*Trefclk>120ns,11us<PGM_COUNT*Trefclk+EFUSEC_COUNT*Trefclk<13us,
*  ����EFUSEC_COUNT>=3
*/
#define EFUSE_COUNT_CFG     5
#define PGM_COUNT_CFG       0x500

static inline void efusec_enable_clk(void)
{
	writel(GT_PCLK_EFUSEC, SCPEREN1(SCTRL_BASE));
}

static inline void efusec_disable_clk(void)
{
	/* disenable system efusec clock */
	writel(GT_PCLK_EFUSEC, SCPERDIS1(SCTRL_BASE));
}

static void efuse_power_on(void)
{
	/* ����VOUT6���1.8V, ��eFuse�������ϵ� */
	int ret = 0;
	ret = regulator_bulk_enable(1, &g_efuse_data.regu_burning);
	if (ret)
		pr_err("failed to enable efuse regulators %d\n", ret);
	else
		pr_info("enable efuse regulators.\n");
}

static void efuse_power_down(void)
{
	/* ����VOUT6���0V, ��eFuse�������µ� */
	int ret = 0;
	ret = regulator_bulk_disable(1, &g_efuse_data.regu_burning);
	if (ret)
		pr_err("failed to disable efuse regulators %d\n", ret);
	else
		pr_info("disable efuse regulators.\n");
}

static inline int exit_power_down_mode(void)
{
	unsigned int loop_count = EFUSE_OP_TIMEOUT_COUNT;
	int pd_status = readl(EFUSEC_STATUS(EFC_BASE)) & EFUSEC_PD_STATUS_MASK;

	if (pd_status == 0) {
		pr_debug("Current pd status is nomal.\n");
		return OK;
	}

	writel(readl(EFUSEC_CFG(EFC_BASE)) & (~EFUSEC_PD_EN), EFUSEC_CFG(EFC_BASE));

	while ((readl(EFUSEC_STATUS(EFC_BASE)) & EFUSEC_PD_STATUS_MASK) > 0) {
		pr_warning("Current pd status is power down.\n");
		if (0 == loop_count) {
			pr_err("Loop is overrun, current pd status is power down.\n");
			return ERROR;
		}
		loop_count--;
		udelay(EFUSE_OP_TIMEOUT);
	}
	pr_info("Exit power down mode OK.\n");
	return OK;
}

static inline int enter_power_down_mode(void)
{
	unsigned int loop_count = EFUSE_OP_TIMEOUT_COUNT;

	int pd_status = readl(EFUSEC_STATUS(EFC_BASE)) & EFUSEC_PD_STATUS_MASK;
	if (pd_status > 0) {
		pr_debug("Current pd status is power down.\n");
		return OK;
	}

	writel(readl(EFUSEC_CFG(EFC_BASE)) | EFUSEC_PD_EN, EFUSEC_CFG(EFC_BASE));
	while (0 == (readl(EFUSEC_STATUS(EFC_BASE)) & EFUSEC_PD_STATUS_MASK)) {
		pr_warning("Current pd status is nomal.\n");
		if (0 == loop_count) {
			pr_err("Loop is over, current pd status is nomal.\n");
			return ERROR;
		}
		loop_count--;
		udelay(EFUSE_OP_TIMEOUT);
	}
	pr_info("Enter the power down mode OK.\n");
	return OK;
}

static void display_regtable(void)
{
	pr_info("EFUSEC_CFG: 0x%X.\n", readl(EFUSEC_CFG(EFC_BASE)));
	pr_info("EFUSEC_STATUS: 0x%X.\n", readl(EFUSEC_STATUS(EFC_BASE)));
	pr_info("EFUSEC_GROUP: 0x%X.\n", readl(EFUSEC_GROUP(EFC_BASE)));
	pr_info("EFUSEC_PG_VALUE: 0x%X.\n", readl(EFUSEC_PG_VALUE(EFC_BASE)));
	pr_info("EFUSEC_COUNT: 0x%X.\n", readl(EFUSEC_COUNT(EFC_BASE)));
	pr_info("EFUSEC_PGM_COUNT: 0x%X.\n", readl(EFUSEC_PGM_COUNT(EFC_BASE)));
	pr_info("EFUSEC_DATA: 0x%X.\n", readl(EFUSEC_DATA(EFC_BASE)));
	pr_info("EFUSEC_HW_CFG: 0x%X.\n", readl(EFUSEC_HW_CFG(EFC_BASE)));
}



/******************************************************************************
Function:	    bsp_efuse_write
Description:	    ��ָ��Wordsƫ�ƿ�ʼд��ָ��Words������eFuseֵ
Input:		    buf			- ������������Ҫд�뵽eFuse�е�ֵ
		    size		- ���������Ҫд���Words����
		    group		- �����������ָ����Wordsƫ�ƴ���ʼд�룬
					  ���б�ʾeFuse�������group
Output:		    none
Return:		    0: OK  ����: ERROR��
******************************************************************************/
int bsp_efuse_write(const unsigned int* buf,
		  const unsigned int group,
		  const unsigned int size)
{
	int result = OK;
	unsigned int *curr_value = (unsigned int *)buf;
	unsigned int count = 0;
	unsigned int group_index = group;
	unsigned int loop_count = EFUSE_OP_TIMEOUT_COUNT;
	unsigned int read_buf[64] = {0};

	pr_info("%s Enter.(NO TZDRIVER)\n", __func__);

	/* ����ж� */
	if (NULL == curr_value) {
		pr_err("bsp_efuse_write: puiValues is NULL!\n" );
		return ERROR;
	}

	if ((size == 0) ||
	    (group_index > EFUSEC_GROUP_MAX_COUNT) ||
	    (group_index + size > EFUSEC_GROUP_MAX_COUNT)) {
		pr_err("lineno:%d\n", __LINE__);
		pr_err("bsp_efuse_write: input para is overrun!"
		"size: %d, group: %d\n", size, group_index);
		return ERROR;
	}

	/* efuse���� */
	mutex_lock(&g_efuse_data.lock);

	/* ʹ�����ͨ· */
	writel(SYS_EFUSE_SOFT_SEL, SCEFUSECTRL(SCTRL_BASE));

	/* ����remapΪ0����������д��eFuse������eFuse������ */
	writel(SCPERCTRL0_REMAP_DIS, SCPERCTRL0(SCTRL_BASE));

	/* ��eFuse�������ϵ� */
	efuse_power_on();

	/* ʹ��efusecʱ�� */
	efusec_enable_clk();

	/* �˳�power_down״̬ */
	if (ERROR == exit_power_down_mode()) {
		pr_err("Can't exit power down mode!\n" );
		result = ERROR_EXIT_PD;
		goto end1;
	}

	/* �ж��Ƿ�������д */
	if ((readl(EFUSEC_HW_CFG(EFC_BASE)) & EFUSEC_APB_PGM_DISABLE_MASK) > 0) {
		pr_err("APB program is disnable!\n" );
		result = ERROR_APB_PGM_DIS;
		goto end1;
	}

	/* ѡ��efuse�ź�Ϊapb����efuse */
	writel(readl(EFUSEC_CFG(EFC_BASE)) | EFUSEC_APB_SIG_SEL, EFUSEC_CFG(EFC_BASE));

	/* ����ʱ��Ҫ��,hi3630 asic�汾����ʹ��Ĭ��ֵ */
	writel(EFUSE_COUNT_CFG, EFUSEC_COUNT(EFC_BASE));
	writel(PGM_COUNT_CFG, EFUSEC_PGM_COUNT(EFC_BASE));

	/* ʹ��Ԥ��д */
	writel((readl(EFUSEC_CFG(EFC_BASE)) | EFUSEC_PRE_PG),  EFUSEC_CFG(EFC_BASE));
	loop_count = EFUSE_OP_TIMEOUT_COUNT;
	while (0 == (readl(EFUSEC_STATUS(EFC_BASE)) & EFUSEC_PGENAB_STATUS_MASK)) {
		pr_warning("(Prefire)Current pgenab status is not finished.\n");
		if (0 == loop_count--) {
			pr_err("(Prefire)time out, current pgenab status is not finished.\n");
			result = ERROR_PRE_WRITE;
			goto end2;
		}
		udelay(EFUSE_OP_TIMEOUT);
	}

	for (count = 0; count < size; count++) {
		if (0 == *curr_value) {
			/* ��д��һ�� */
			group_index++;
			curr_value++;
			continue;
		}

		/* ����group */
		writel(group_index, EFUSEC_GROUP(EFC_BASE));

		/* ����value */
		writel(*curr_value, EFUSEC_PG_VALUE(EFC_BASE));

		/* ʹ����д */
		writel((readl(EFUSEC_CFG(EFC_BASE)) | EFUSEC_PG_EN),  EFUSEC_CFG(EFC_BASE));

		udelay(500);

		/* ��ѯ��д��� */
		loop_count = EFUSE_OP_TIMEOUT_COUNT;
		while (0 == (readl(EFUSEC_STATUS(EFC_BASE)) & EFUSEC_PG_STATUS_MASK)) {
			pr_debug("Current pg status is not finished.\n");
			if (0 == loop_count--) {
				pr_err("time out, current pg status is not finished.\n");
				result = ERROR_PG_OPERATION;
				goto end2;
			}
			udelay(EFUSE_OP_TIMEOUT);
		}
		pr_info("group:%d, write:0x%x, time:%d us! (count %d/%d)\n", group_index, *curr_value, 500+EFUSE_OP_TIMEOUT*(EFUSE_OP_TIMEOUT_COUNT-loop_count), loop_count, EFUSE_OP_TIMEOUT_COUNT);
		pr_debug("Current pg status is finished, then write next group.\n");

		/* ��д��һ�� */
		group_index++;
		curr_value++;
	}
end2:
	/* ��ʹ��Ԥ��д */
	writel((readl(EFUSEC_CFG(EFC_BASE)) & (~EFUSEC_PRE_PG)), EFUSEC_CFG(EFC_BASE));

	/* �޸�efuseĬ�ϵ��ٲ�ΪAIB */
	writel(readl(EFUSEC_CFG(EFC_BASE)) & (~EFUSEC_APB_SIG_SEL), EFUSEC_CFG(EFC_BASE));

end1:
	/* ��eFuse�������µ� */
	efuse_power_down();

	/* efuse���� */
	mutex_unlock(&g_efuse_data.lock);

	/* ���޸ĵ����ݸ��µ�efuse������ */
	if (OK != bsp_efuse_read(read_buf, group, size)) {
		pr_err("bsp_efuse_read(after write) failed!\n" );
	}

	/* ����remapΪ1, ֮��Ķ������Ǵ�eFuse�����л�� */
	writel(SCPERCTRL0_REMAP_EN, SCPERCTRL0((SCTRL_BASE)));

	if (OK != result) {
		pr_err("bsp_efuse_write failed!\n" );
		display_regtable();
	}

	pr_info("%s Exit.(NO TZDRIVER)\n", __func__);

	return result;
}
EXPORT_SYMBOL_GPL(bsp_efuse_write);


/******************************************************************************
Function:	    bsp_efuse_read
Description:	    ��ָ��Wordsƫ�ƿ�ʼ��ȡָ��Words������eFuseֵ
Input:		    buf			- ����&�����������Ŷ�ȡ����eFuseֵ��
					  �ɵ��÷���������ڴ�
		    group		- �����������ָ����Wordsƫ�ƴ���ʼ��ȡ��
					  ���б�ʾeFuse�������group
		    size		- ���������Ҫ��ȡ��Words����
Output:		    buf			- �����������Ŷ�ȡ����eFuseֵ��
					  �ɵ��÷���������ڴ�
Return:		    0: OK  ����: ERROR��
******************************************************************************/
int bsp_efuse_read(unsigned int* buf,
		  const unsigned int group,
		  const unsigned int size)
{
	int result = OK;
	unsigned int *curr_value = buf;
	unsigned int count = 0;
	unsigned int loop_count = EFUSE_OP_TIMEOUT_COUNT;

	pr_info("%s Enter.(NO TZDRIVER)\n", __func__);

	/* ����ж� */
	if (NULL == buf) {
		pr_err("bsp_efuse_read: puiValues is NULL!\n" );
		return ERROR;
	}

	if ((size == 0) ||
	    (group > EFUSEC_GROUP_MAX_COUNT) ||
	    (group + size > EFUSEC_GROUP_MAX_COUNT)) {
		pr_err("bsp_efuse_read: input para is overrun!"
		"size: %d, group: %d\n", size, group);
		return ERROR;
	}

	/* efuse���� */
	mutex_lock(&g_efuse_data.lock);

	/* ʹ�����ͨ· */
	writel(SYS_EFUSE_SOFT_SEL, SCEFUSECTRL(SCTRL_BASE));

	/* ʹ��efusecʱ�� */
	efusec_enable_clk();

	/* �˳�power_down״̬ */
	if (ERROR == exit_power_down_mode()) {
		pr_err("Can't exit power down mode!\n" );
		result = ERROR_EXIT_PD;
		goto end1;
	}

	/* ѡ��efuse�ź�Ϊapb����efuse */
	writel(readl(EFUSEC_CFG(EFC_BASE)) | EFUSEC_APB_SIG_SEL, EFUSEC_CFG(EFC_BASE));

	/* ѭ����ȡEfuseֵ */
	for (count = 0; count < size; count++) {
		/* ���ö�ȡ��ַ */
		writel(group + count, EFUSEC_GROUP(EFC_BASE));

		/* ʹ�ܶ�efuse���� */
		writel((readl(EFUSEC_CFG(EFC_BASE)) | EFUSEC_RD_EN),  EFUSEC_CFG(EFC_BASE));

		udelay(10);

		/* �ȴ������ */
		loop_count = EFUSE_OP_TIMEOUT_COUNT;

		while (0 == (readl(EFUSEC_STATUS(EFC_BASE)) & EFUSEC_RD_STATUS_MASK)) {
			pr_debug("Current efc read status is not finished.\n");
			if (0 == loop_count--) {
				pr_err("Current efc read timeout!\n");
				result = ERROR_EFUSEC_READ;
				goto end2;
			}
			udelay(EFUSE_OP_TIMEOUT);
		}
		pr_info("group:%d, read time:%d us!(count %d/%d)\n", group + count, 10+EFUSE_OP_TIMEOUT*(EFUSE_OP_TIMEOUT_COUNT-loop_count), loop_count, EFUSE_OP_TIMEOUT_COUNT);
		pr_debug("Current efc read operation is finished, then read next group.\n");

		/* ��ȡ���� */
		*curr_value = readl(EFUSEC_DATA(EFC_BASE));
		curr_value++;
	}
end2:
	/* �޸�efuseĬ�ϵ��ٲ�ΪAIB */
	writel(readl(EFUSEC_CFG(EFC_BASE)) & (~EFUSEC_APB_SIG_SEL), EFUSEC_CFG(EFC_BASE));
end1:

	/* efuse���� */
	mutex_unlock(&g_efuse_data.lock);

	if (OK != result) {
		display_regtable();
	}

	pr_info("%s Exit.(NO TZDRIVER)\n", __func__);

	return result;
}
EXPORT_SYMBOL_GPL(bsp_efuse_read);
#else
/******************************************************************************
Function:	    bsp_efuse_write
Description:	    ��ָ��Wordsƫ�ƿ�ʼд��ָ��Words������eFuseֵ
Input:		    buf			- ������������Ҫд�뵽eFuse�е�ֵ
		    size		- ���������Ҫд���Words����
		    group		- �����������ָ����Wordsƫ�ƴ���ʼд�룬
					  ���б�ʾeFuse�������group
Output:		    none
Return:		    0: OK  ����: ERROR��
******************************************************************************/
int bsp_efuse_write(const unsigned int* buf,
		  const unsigned int group,
		  const unsigned int size)

{
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Result result;
	TEEC_UUID svc_uuid = TEE_SERVICE_EFUSE;
	TEEC_Operation operation;
	uint32_t cmd_id;
	uint32_t origin;

	/* ����ж� */
	if (NULL == buf) {
		pr_err("bsp_efuse_write: puiValues is NULL!\n" );
		return ERROR;
	}

	if ((size == 0) ||
	    (group > EFUSEC_GROUP_MAX_COUNT) ||
	    (group + size > EFUSEC_GROUP_MAX_COUNT)) {
		pr_err("lineno:%d\n", __LINE__);
		pr_err("bsp_efuse_write: input para is overrun!"
		"size: %d, group: %d\n", size, group);
		return ERROR;
	}

	pr_info("%s Enter.(WHIT TZDRIVER)\n", __func__);

	mutex_lock(&g_efuse_data.lock);

	result = TEEK_InitializeContext(
			               NULL,
			               &context);

	if(result != TEEC_SUCCESS) {
		TEEC_Error("TEEK_InitializeContext failed, result=0x%x\n", result);
		result = ERROR;
		goto error1;
	}
	TEEC_Info("succeed to initialize context.\n");

	result = TEEK_OpenSession(
	                    &context,
	                    &session,
	                    &svc_uuid,
	                    TEEC_LOGIN_PUBLIC,
	                    NULL,
	                    NULL,
	                    NULL);

	if (result != TEEC_SUCCESS) {
		TEEC_Error("TEEK_OpenSession failed, result=0x%x\n", result);
		result = ERROR_SECURE_OS;
		goto error2;
	}
	TEEC_Info("succeed to open session.\n");

	memset(&operation, 0, sizeof(operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
						TEEC_VALUE_INPUT,
						TEEC_MEMREF_TEMP_INPUT,
						TEEC_NONE,
						TEEC_NONE);
	operation.params[0].value.a = size;
	operation.params[0].value.b = group;
	operation.params[1].tmpref.buffer = (void *)buf;
	operation.params[1].tmpref.size = size*sizeof(unsigned int);

	TEEC_Info("puiValues=0x%x, size=%d\n", virt_to_phys((void *)buf), size);

	cmd_id = ECHO_CMD_ID_EFUSE_WRITE;
	result = TEEK_InvokeCommand(
				&session,
				cmd_id,
				&operation,
				&origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n", result, origin);
		result = ERROR_SECURE_OS;
	}
	TEEC_Info("succeed to invoke command.\n");

	TEEK_CloseSession(&session);
error2:
	TEEK_FinalizeContext(&context);
error1:

	mutex_unlock(&g_efuse_data.lock);

	pr_info("%s Exit.(WHIT TZDRIVER)\n", __func__);
	return result;
}
EXPORT_SYMBOL_GPL(bsp_efuse_write);


/******************************************************************************
Function:	    bsp_efuse_read
Description:	    ��ָ��Wordsƫ�ƿ�ʼ��ȡָ��Words������eFuseֵ
Input:		    buf			- ����&�����������Ŷ�ȡ����eFuseֵ��
					  �ɵ��÷���������ڴ�
		    group		- �����������ָ����Wordsƫ�ƴ���ʼ��ȡ��
					  ���б�ʾeFuse�������group
		    size		- ���������Ҫ��ȡ��Words����
Output:		    buf			- �����������Ŷ�ȡ����eFuseֵ��
					  �ɵ��÷���������ڴ�
Return:		    0: OK  ����: ERROR��
******************************************************************************/
int bsp_efuse_read(unsigned int* buf,
		  const unsigned int group,
		  const unsigned int size)
{
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Result result;
	TEEC_UUID svc_uuid = TEE_SERVICE_EFUSE;
	TEEC_Operation operation;
	uint32_t cmd_id;
	uint32_t origin;

	/* ����ж� */
	if (NULL == buf) {
		TEEC_Error("bsp_efuse_read: puiValues is NULL!\n" );
		return ERROR;
	}

	if ((size == 0) ||
	    (group > EFUSEC_GROUP_MAX_COUNT) ||
	    (group + size > EFUSEC_GROUP_MAX_COUNT))
	{
		TEEC_Error("bsp_efuse_read: input para is overrun!"
		"size: %d, group: %d\n", size, group);
		return ERROR;
	}

	pr_info("%s Enter.(WHIT TZDRIVER)\n", __func__);

	mutex_lock(&g_efuse_data.lock);

	result = TEEK_InitializeContext(
					NULL,
					&context);

	if(result != TEEC_SUCCESS) {
		TEEC_Error("TEEK_InitializeContext failed, result=0x%x\n", result);
		result = ERROR;
		goto error1;
	}
	TEEC_Info("succeed to initialize context.\n");

	result = TEEK_OpenSession(
				&context,
				&session,
				&svc_uuid,
				TEEC_LOGIN_PUBLIC,
				NULL,
				NULL,
				NULL);

	if (result != TEEC_SUCCESS) {
		TEEC_Error("TEEK_OpenSession failed, result=0x%x\n", result);
		result = ERROR_SECURE_OS;
		goto error2;
	}
	TEEC_Info("succeed to open session.\n");

	memset(&operation, 0, sizeof(operation));
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(
						TEEC_VALUE_INPUT,
						TEEC_MEMREF_TEMP_OUTPUT,
						TEEC_NONE,
						TEEC_NONE);
	operation.params[0].value.a = size;
	operation.params[0].value.b = group;
	operation.params[1].tmpref.buffer = (void *)buf;
	operation.params[1].tmpref.size = size*sizeof(unsigned int);

	TEEC_Info("puiValues=0x%x, size=%d\n", virt_to_phys((void *)buf), size);

	cmd_id = ECHO_CMD_ID_EFUSE_READ;
	result = TEEK_InvokeCommand(
				&session,
				cmd_id,
				&operation,
				&origin);
	if (result != TEEC_SUCCESS) {
		TEEC_Error("invoke failed, codes=0x%x, origin=0x%x\n",
								result, origin);
		result = ERROR_SECURE_OS;
	}
	TEEC_Info("succeed to invoke command.\n");

	TEEK_CloseSession(&session);
error2:
	TEEK_FinalizeContext(&context);
error1:
	mutex_unlock(&g_efuse_data.lock);

	pr_info("%s Exit.(WHIT TZDRIVER)\n", __func__);

	return result;
}
EXPORT_SYMBOL_GPL(bsp_efuse_read);
#endif

int efuse_test(void);
int efuse_test01(int group, u32 efuse_write);
int efuse_test02(void);
int efuse_test03(int group);
int efuse_test04(int loop);

#define	HISI_EFUSE_TEST01		0x3000
#define	HISI_EFUSE_TEST02		0x3001
#define	HISI_EFUSE_TEST03		0x3002
#define	HISI_EFUSE_TEST04		0x3003
#define	HISI_EFUSE_QTEST		0x3004

/******************************************************************************
Function:	efuse_test
Description:	eFuseģ�����д���ٲ��ԣ�������adb shell��ͨ��ecall����
Procedure:
		1��ѡȡһ��efuse
		2������д������2^n-1(nΪ1��32)������ȡУ��
		3�����Ŀ��efuse���Ѿ������ݣ���У��ʱ����λ����
		4�����Ŀ��efuse��ԭ��������Ϊ0xffffffff����efuse_test����ʾ�޷�����д�빦��
Remark:
		1��efuseд����޷��ָ�!!!
		2�����ܽ�2022��2023��2024 bit����Ϊ1,��ֹʹHUK��SCP��DFT��֤���������APB���ɶ���
		3�����ܽ�2016bit����Ϊ1����ֹAPB��AIB������д��
		4��������д��27��(Reserved)
Return:		OK:���Գɹ�  ERROR��:����ʧ��
******************************************************************************/
int efuse_test(void)
{
	const u32 base = 0xffffffff;
	const int group = 57;
	u32 value = 0;
	u32 read_org = 0;
	u32 read = 0;
	int pos;
	int ret = 0;

	pr_info("[%s] efuse(group:%d) quick test begin!\n", __func__, group);

	if ((group < 0) || (group >= EFUSEC_GROUP_MAX_COUNT)) {
		pr_err("[%s]:Group %d is error! (Expected 0 to %d)\n", __func__, group, EFUSEC_GROUP_MAX_COUNT-1);
		ret = -1;
		goto fail_test;
	}

	if (OK != bsp_efuse_read(&read_org, group, 1)) {
		pr_err("[%s]:bsp_efuse_read orgdata failed!\n", __func__);
		ret = -1;
		goto fail_test;
	} else {
		if (0xffffffff == read_org) {
			pr_info("[%s] Warnning:Org value of group %d is 0xffffffff, so efuse_test connot verify bsp_efuse_write function!\n", __func__, group);
		} else {
			pr_info("[%s]:Org value of group %d is 0x%x!\n", __func__, group, read_org);
		}
	}

	for (pos = 1; pos <= 32; pos++) {
		/* prepare write data */
		value = ~(base<<pos);
		/* pr_info("Write Group:%d, Value:0x%x\n", group, value); */

		if (OK != bsp_efuse_write(&value, group, 1)) {
			pr_err("[%s] Group %d bsp_efuse_write 0x%x failed!\n", __func__, group, value);
			ret = -1;
			goto fail_test;
		}

		if (OK != bsp_efuse_read(&read, group, 1)) {
			pr_err("[%s]: Group %d bsp_efuse_read after write failed!\n", __func__, group);
			ret = -1;
			goto fail_test;
		}

		if ((value | read_org) != read) {
			pr_err("[%s]:Group %d Verify Fialed!(org_data:0x%x, write:0x%x, read:0x%x)\n", __func__, group, read_org, value, read);
			ret = -1;
			goto fail_test;
		} else {
			pr_info("[%s]:Group %d W/R 0x%x Test Passed!\n", __func__, group, value);
		}
	}

	if (0xffffffff == read_org) {
		pr_info("[%s] efuse quick test finished! efuse_read function passed, but cannot verify efuse_write function!\n", __func__);
	} else {
		pr_info("[%s] efuse quick test passed!\n", __func__);
	}

	return ret;

fail_test:
	pr_info("[%s] efuse quick test failed!\n", __func__);

	return ret;
}

/******************************************************************************
Function:	efuse_test01
Description:	eFuse�����д����
Procedure:
		1��ѡȡһ��efuse����ȡ����Ϊdata1
		2����ָ������data2д�����
		3����ȡ��������Ϊdata3
		4��У��data3�Ƿ����data1|data2
Parameter:
		group:��д����efuse��
		efuse_write:д������
Remark:
		1��efuseд����޷��ָ�!!!
		2�����ܽ�2022��2023��2024 bit����Ϊ1,��ֹʹHUK��SCP��DFT��֤���������APB���ɶ���
		3�����ܽ�2016bit����Ϊ1����ֹAPB��AIB������д��
		4��������д��27��(Reserved)
Return:		OK:���Գɹ�  ERROR��:����ʧ��
******************************************************************************/
int efuse_test01(int group, u32 efuse_write)
{
	u32 efuse_read1;
	u32 efuse_read2;
	int ret = 0;

	pr_info("[%s] Efuse W/R TEST begin. Group:%d, WriteData:0x%x.\n", __func__, group, efuse_write);

	if (OK == bsp_efuse_read(&efuse_read1, group, 1)) {
		pr_info("[%s]efuse_read1: 0x%x.\n", __func__, efuse_read1);
	} else {
		pr_err("[%s]Failed to read group %d before writting!\n", __func__, group);
		ret = -1;
		goto fail_test01;
	}

	if (OK == bsp_efuse_write(&efuse_write, group, 1)) {
		pr_info("[%s]efuse write:0x%x.\n", __func__, efuse_write);
	} else {
		pr_err("[%s]Failed to write group %d.\n", __func__, group);
		ret = -1;
		goto fail_test01;
	}

	if (OK == bsp_efuse_read(&efuse_read2, group, 1)) {
		pr_info("[%s]efuse_read2: 0x%x.\n", __func__, efuse_read2);
	} else {
		pr_err("[%s]Failed to read group %d after writting!\n", __func__, group);
		ret = -1;
		goto fail_test01;
	}

	if (efuse_read2 != (efuse_read1 | efuse_write)){
		pr_err("[%s] Group %d verify Failed!(org:0x%x,write0x%x,read0x%x)\n",\
			__func__, group, efuse_read1, efuse_write, efuse_read2);
		ret = -1;
		goto fail_test01;
	}

	pr_info("[%s] Efuse W/R TEST Success!\n", __func__);

	return ret;

fail_test01:
	pr_err("[%s] Efuse W/R TEST Failed!\n", __func__);
	return ret;
}

/******************************************************************************
Function:	efuse_test02
Description:	eFuse �����д����
Procedure:
		1������eFuse��д������group(0~26)����дֵ�ֱ�ȡ0xAAAAAAAA;
		2������eFuse��д������group 27����дֵȡ0x00000000��
		3������eFuse��д������group(28~62)����дֵ�ֱ�ȡ0xAAAAAAAA;
		4������eFuse��д������group 63����дֵȡ0x00000000��
		5����д��ɺ���eFuse�����̻ض����ݣ��鿴�ض������Ƿ����д�����Ƿ�һ�£�

Remark:
		1��efuseд����޷��ָ�!!!
		2�����ܽ�2022��2023��2024 bit����Ϊ1,��ֹʹHUK��SCP��DFT��֤���������APB���ɶ���
		3�����ܽ�2016bit����Ϊ1����ֹAPB��AIB������д��
		4��������д��27��(Reserved)
Return:		OK:���Գɹ�  ERROR��:����ʧ��
******************************************************************************/
int efuse_test02(void)
{
	const u32 TEST_VALUE = 0xAAAAAAAA;
	const u32 GROUP_27_VALUE = 0x00000000;
	const u32 GROUP_63_VALUE = 0x00000000;

	u32 uiGroupWrBuf[EFUSEC_GROUP_MAX_COUNT] = {0};
	u32 uiGroupRdBuf[EFUSEC_GROUP_MAX_COUNT] = {0};
	int i = 0;

	pr_info("[%s] Efuse Integrate Test Begin!\n", __func__);

	if (OK != bsp_efuse_read(uiGroupWrBuf, 0, EFUSEC_GROUP_MAX_COUNT)) {
		pr_err("[%s]:First bsp_efuse_read failed!\n", __func__);
		return ERROR;
	}

	pr_info("[%s]efuse read 1:\n", __func__);
	for (i = 0; i < EFUSEC_GROUP_MAX_COUNT; i++) {
		pr_info("%d:0x%x\n", i, uiGroupWrBuf[i]);
	}

	for (i = 0; i < EFUSEC_GROUP_MAX_COUNT; i++) {
		uiGroupWrBuf[i] |= TEST_VALUE;
	}
	uiGroupWrBuf[27] = GROUP_27_VALUE;
	uiGroupWrBuf[63] = GROUP_63_VALUE;

	pr_info("efuse set:\n");
	for (i = 0; i < EFUSEC_GROUP_MAX_COUNT; i++) {
		pr_info("%d:0x%x\n", i, uiGroupWrBuf[i]);
	}
/*
	if (OK != bsp_efuse_write(uiGroupWrBuf, 0, EFUSEC_GROUP_MAX_COUNT)) {
		pr_err("[%s]:bsp_efuse_write failed!\n", __func__);
		return ERROR;
	}
*/
	if (OK != bsp_efuse_read(uiGroupRdBuf, 0, EFUSEC_GROUP_MAX_COUNT)) {
		pr_err("[%s]:Second bsp_efuse_read failed!\n", __func__);
		return ERROR;
	}

	pr_info("efuse read 2:\n");
	for (i=0; i<EFUSEC_GROUP_MAX_COUNT; i++) {
		pr_info("%d:0x%x\n", i, uiGroupRdBuf[i]);
	}

	if (0 == memcmp(uiGroupWrBuf, uiGroupRdBuf, sizeof(u32)*EFUSEC_GROUP_MAX_COUNT)) {
		pr_info("[%s] Efuse Integrate Test Success!\n", __func__);
		return OK;
	} else {
		pr_info("[%s] Efuse Integrate Test Failed!\n", __func__);
		return ERROR;
	}
}

/******************************************************************************
Function:	efuse_test03
Description:	eFuse �����дѹ������
Procedure:
		1��ѡȡһ�����efuse
		2������д������2^n-1(nΪ1��32)������ȡУ��
		3���ò����ܹ�����efuseд������ʱ���������
Remark:
		1��efuseд����޷��ָ�!!!
		2�����ܽ�2022��2023��2024 bit����Ϊ1,��ֹʹHUK��SCP��DFT��֤���������APB���ɶ���
		3�����ܽ�2016bit����Ϊ1����ֹAPB��AIB������д��
		4��������д��27��(Reserved)
Return:		OK:���Գɹ�  ERROR��:����ʧ��
******************************************************************************/
int efuse_test03(int group)
{
	const u32 base = 0xffffffff;
	u32 value = 0;
	u32 read = 0;
	int pos;
	int ret = 0;

	pr_info("[%s] Efuse(group:%d) W/R Pressure TEST Begin!\n", __func__, group);

	if (group < 0 || group >= EFUSEC_GROUP_MAX_COUNT) {
		pr_err("[%s]:Group %d is error! (Expected 0 to %d)\n", __func__, group, EFUSEC_GROUP_MAX_COUNT-1);
		ret = -1;
		goto fail_verify_group;
	}

	if (OK != bsp_efuse_read(&read, group, 1)) {
		pr_err("[%s]:bsp_efuse_read failed!\n", __func__);
		ret = -1;
		goto fail_verify_orgdata;
	} else {
		if(read != 0) {
			pr_err("[%s]:Value of group %d is not zero but 0x%x!\n", __func__, group, read);
			ret = -1;
			goto fail_verify_orgdata;
		}
	}

	for (pos = 1; pos <= 32; pos++) {
		/* prepare write data */
		value = ~(base<<pos);
		/* pr_info("[%s]:Write Group:%d, Value:0x%x\n", __func__, group, value); */

		if (OK != bsp_efuse_write(&value, group, 1)) {
			pr_err("[%s]:bsp_efuse_write failed!(group:%d, vaule:0x%x)\n", __func__, group, value);
			ret = -1;
			goto fail_test;
		}

		if (OK != bsp_efuse_read(&read, group, 1)) {
			pr_err("[%s]:bsp_efuse_read failed!(group:%d)\n", __func__, group);
			ret = -1;
			goto fail_test;
		}

		if (value != read) {
			pr_err("[%s]:Verify Group %d Fialed! Write:0x%x,Read:0x%x\n", __func__, group, value, read);
			ret = -1;
			goto fail_test;
		} else {
			pr_err("[%s]:Group %d R/W 0x%x Pass!\n", __func__, group, value);
		}
	}

	pr_info("[%s] Efuse(group:%d) W/R Pressure TEST Success!\n", __func__, group);

	return ret;

fail_test:
fail_verify_orgdata:
fail_verify_group:
	pr_info("[%s] Efuse(group:%d) W/R Pressure TEST Failed!\n", __func__, group);
	return ret;
}

/******************************************************************************
Function:	efuse_test04
Description:	eFuse �����дѹ������
Procedure:
		1������eFuse��д������group(0~26)����дֵ�ֱ�ȡ0xAAAAAAAA;
		2������eFuse��д������group 27����дֵȡ0x00000000��
		3������eFuse��д������group(28~62)����дֵ�ֱ�ȡ0xAAAAAAAA;
		4������eFuse��д������group 63����дֵȡ0x00000000��
		5����д��ɺ���eFuse�����̻ض����ݣ��鿴�ض������Ƿ����д�����Ƿ�һ�£�
		6��ѭ��ִ��1��5����

Remark:
		1��efuseд����޷��ָ�!!!
		2�����ܽ�2022��2023��2024 bit����Ϊ1,��ֹʹHUK��SCP��DFT��֤���������APB���ɶ���
		3�����ܽ�2016bit����Ϊ1����ֹAPB��AIB������д��
		4��������д��27��(Reserved)
Return:		OK:���Գɹ�  ERROR��:����ʧ��
******************************************************************************/
int efuse_test04(int loop)
{
	int i=0;

	pr_info("[%s] Efuse Integrate Pressure Test Begin!\n", __func__);

	for (i=0; i<loop; i++) {
		if (OK == efuse_test02()) {
			pr_info("[%s] %d%% Finished!\n", __func__, (i+1)*100/loop);
		} else {
			pr_info("[%s] Efuse Integrate Pressure Test Failed! %d%% Finished!\n", __func__, i*100/loop);
			return ERROR;
		}
	}

	pr_info("[%s] Efuse Integrate Pressure Test Passed!\n", __func__);
	return OK;
}

/*
 * Function name:efusec_ioctl.
 * Discription:complement read efuse by terms of sending command-words.
 * return value:
 *          @ 0 - success.
 *          @ -1- failure.
 */
static long efusec_ioctl(struct file *file, u_int cmd, u_long arg)
{
	int ret = OK;
	void __user *argp = (void __user *)arg;
	unsigned char efuse_read_buf[256] = {0};

	u32 test_data[2];

	switch (cmd) {
	case HISI_EFUSE_READ_CHIPID:
		ret = bsp_efuse_read((unsigned int*)efuse_read_buf, 57, 2);
		if (ret) {
			pr_err("efusec_ioctl: bsp_efuse_read failed.\n");
			break;
		}

		/*send back to user*/
		if (copy_to_user(argp, efuse_read_buf, 8))
			ret = -EFAULT;

		break;
	case HISI_EFUSE_READ_DIEID:
		ret = bsp_efuse_read((unsigned int*)efuse_read_buf, 32, 5);
		if (ret) {
			pr_err("efusec_ioctl: bsp_efuse_read failed.\n");
			break;
		}

		/*send back to user*/
		if (copy_to_user(argp, efuse_read_buf, 20))
			ret = -EFAULT;

		break;
	case HISI_EFUSE_TEST01:
		if (copy_from_user(test_data, argp, 8)) {
			ret = -EFAULT;
			break;
		}
		pr_debug("group(test_data[0]):%d, write_value(test_data[1]):%d\n", test_data[0], test_data[1]);

		ret = efuse_test01(test_data[0], test_data[1]);
		if (ret) {
			pr_err("[%s]:efuse_test01 failed!\n", __func__);
		} else {
			pr_info("[%s]:efuse_test01 passed!\n", __func__);
		}
		break;
	case HISI_EFUSE_TEST02:
		ret = efuse_test02();
		if (ret) {
			pr_err("[%s]:efuse_test02 failed!\n", __func__);
		} else {
			pr_info("[%s]:efuse_test02 passed!\n", __func__);
		}
		break;
	case HISI_EFUSE_TEST03:
		if (copy_from_user(test_data, argp, 4)) {
			ret = -EFAULT;
			break;
		}
		pr_debug("group(test_data[0]):%d\n", test_data[0]);

		ret = efuse_test03(test_data[0]);
		if (ret) {
			pr_err("[%s]:efuse_test03 failed!\n", __func__);
		} else {
			pr_info("[%s]:efuse_test03 passed!\n", __func__);
		}
		break;
	case HISI_EFUSE_TEST04:
		if (copy_from_user(test_data, argp, 4)) {
			ret = -EFAULT;
			break;
		}
		pr_debug("loop count(test_data[0]):%d\n", test_data[0]);

		ret = efuse_test04(test_data[0]);
		if (ret) {
			pr_err("[%s]:efuse_test04 failed!\n", __func__);
		} else {
			pr_info("[%s]:efuse_test04 passed!\n", __func__);
		}
		break;
	case HISI_EFUSE_QTEST:
		ret = efuse_test();
		if (ret) {
			pr_err("[%s]:efuse_test failed!\n", __func__);
		} else {
			pr_info("[%s]:efuse_test passed!\n", __func__);
		}
		break;
	default:
		pr_err("[EFUSE][%s] Unknow command:%d!\n", __func__, cmd);
		ret = -ENOTTY;
		break;
	}

	return ret;
}

static const struct file_operations efusec_fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl = efusec_ioctl,
};

static int __init hisi_efusec_init(void)
{
	int ret = 0;
	int major = 0;
	struct class *efuse_class;
	struct device *pdevice;
	struct device_node *np = NULL;

	np = of_find_compatible_node(NULL, NULL, "hisilicon,sysctrl");
	if (!np) {
		pr_err("hisi efuse: No sysctrl compatible node found.\n");
		return -ENODEV;
	}
	SCTRL_BASE = of_iomap(np, 0);

	np = of_find_compatible_node(NULL, NULL, "hisilicon,hi6421-pmurtc");
	if (!np) {
		pr_err("hisi efuse: No pmurtc compatible node found.\n");
		return -ENODEV;
	}
	PMURTC_BASE = of_iomap(np, 0);

	np = of_find_compatible_node(NULL, NULL, "hisilicon, efuse");
	if (!np) {
		pr_err("hisi efuse: No efusec compatible node found.\n");
		return -ENODEV;
	}
	EFC_BASE = of_iomap(np, 0);

	pr_info("efuse SCTRL_BASE=%x, PMURTC_BASE=%x, EFC_BASE=%x",
		(unsigned int)SCTRL_BASE, (unsigned int)PMURTC_BASE,
		(unsigned int)EFC_BASE);

	major = register_chrdev(0, EFUSE_DEV_NAME, &efusec_fops);
	if (major <= 0) {
		ret = -EFAULT;
		pr_err("hisi efuse: unable to get major for memory devs.\n");
	}

	efuse_class = class_create(THIS_MODULE, EFUSE_DEV_NAME);
	if (IS_ERR(efuse_class)) {
		ret = -EFAULT;
		pr_err("hisi efuse: class_create error.\n");
		goto error1;
	}

	pdevice = device_create(efuse_class, NULL, MKDEV(major,0), NULL, EFUSE_DEV_NAME);
	if (IS_ERR(pdevice)) {
		ret = -EFAULT;
		pr_err("hisi efuse: device_create error.\n");
		goto error2;
	}
	pr_info("efuse init\n");

	pdevice->of_node = np;
	g_efuse_data.regu_burning.supply = "efuse-burning";

	ret = regulator_bulk_get(pdevice , 1, &g_efuse_data.regu_burning);
	if (ret) {
		dev_err(pdevice, "couldn't get efuse-burning regulator %d\n\r", ret);
		goto error3;
	} else {
		pr_info("get efuse-burning regulator success!\n");
	}

	mutex_init(&g_efuse_data.lock);

	return ret;
error3:
	device_destroy(efuse_class, MKDEV(major,0));
	pdevice = NULL;
error2:
	class_destroy(efuse_class);
	efuse_class = NULL;
error1:
	unregister_chrdev(major, EFUSE_DEV_NAME);
	return ret;
}

rootfs_initcall(hisi_efusec_init);

MODULE_DESCRIPTION("Hisilicon efusec driver");
MODULE_AUTHOR("chenya99@huawei.com");
MODULE_LICENSE("GPL");
