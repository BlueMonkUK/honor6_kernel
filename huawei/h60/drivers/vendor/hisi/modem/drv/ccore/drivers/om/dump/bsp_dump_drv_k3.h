

#ifndef	__BSP_DUMP_DRV_K3_H__
#define __BSP_DUMP_DRV_K3_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#ifndef __ASSEMBLY__
#include "product_config.h"
#include "osl_types.h"
#endif/*#ifndef __ASSEMBLY__*/

#include <config.h>
#include <bsp_memmap.h>

/**************************************************************************
  �궨��
**************************************************************************/
/*DUMP����TOP HEAD��ַ*/
#define DUMP_TOP_HEAD_ADDR                  (DDR_MNTN_ADDR_VIRT)

/* CP��ά�ɲ��ַ */
#define DUMP_COMM_SECTION_OFFSET            (0x480000)
#define DUMP_COMM_SECTION_ADDR              (DDR_MNTN_ADDR_VIRT+DUMP_COMM_SECTION_OFFSET) 
#define DUMP_COMM_SECTION_SIZE              (0x180000) 

/* CP��ά�ɲ�̶������С���ַ */
#define DUMP_CORE_BASE_ADDR                 (DUMP_COMM_SECTION_ADDR)
#define DUMP_HEAD_ADDR                      (DUMP_CORE_BASE_ADDR)
#define DUMP_HEAD_SIZE                      (0x20)
#define DUMP_MAP_ADDR                       (DUMP_HEAD_ADDR+DUMP_HEAD_SIZE)
#define DUMP_MAP_SIZE                       (0x400)
#define DUMP_BASE_INFO_ADDR                 (DUMP_MAP_ADDR+DUMP_MAP_SIZE)
#define DUMP_BASE_INFO_SIZE                 (0x180)
#define DUMP_TASK_SWITCH_ADDR               (DUMP_BASE_INFO_ADDR+DUMP_BASE_INFO_SIZE)
#define DUMP_TASK_SWITCH_SIZE               (0x10000)
#define DUMP_INTLOCK_ADDR                   (DUMP_TASK_SWITCH_ADDR+DUMP_TASK_SWITCH_SIZE)
#define DUMP_INTLOCK_SIZE                   (0x1000)
#define DUMP_TASK_STACK_ADDR                (DUMP_INTLOCK_ADDR+DUMP_INTLOCK_SIZE)
#define DUMP_TASK_STACK_SIZE                (0x2000)
#define DUMP_INT_STACK_ADDR                 (DUMP_TASK_STACK_ADDR+DUMP_TASK_STACK_SIZE)
#define DUMP_INT_STACK_SIZE                 (ISR_STACK_SIZE)
#define DUMP_ALLTASK_ADDR                   (DUMP_INT_STACK_ADDR+DUMP_INT_STACK_SIZE)
#define DUMP_ALLTASK_SIZE                   (0x800)
#define DUMP_ALLTASK_TCB_ADDR               (DUMP_ALLTASK_ADDR+DUMP_ALLTASK_SIZE)
#define DUMP_ALLTASK_TCB_SIZE               (0x10000)
#define DUMP_PRINT_ADDR                     (DUMP_ALLTASK_TCB_ADDR+DUMP_ALLTASK_TCB_SIZE)
#define DUMP_PRINT_SIZE                     (0x4000)
#define DUMP_DMESG_ADDR                     (DUMP_PRINT_ADDR+DUMP_PRINT_SIZE)
#define DUMP_DMESG_SIZE                     (0x4000)
#define DUMP_REGS_ADDR                      (DUMP_DMESG_ADDR+DUMP_DMESG_SIZE)
#define DUMP_REGS_SIZE                      (0x3000)                                     // ����
#define DUMP_CPUVIEW_ADDR                   (DUMP_REGS_ADDR+DUMP_REGS_SIZE)
#define DUMP_CPUVIEW_SIZE                   (0)
#define DUMP_MEMINFO_ADDR                   (DUMP_CPUVIEW_ADDR+DUMP_CPUVIEW_SIZE)
#define DUMP_MEMINFO_SIZE                   (0x8000)
#define DUMP_USER_DATA_ADDR                 (DUMP_MEMINFO_ADDR+DUMP_MEMINFO_SIZE)
#define DUMP_USER_DATA_SIZE                 (0x1000)
#define DUMP_BBE16_TCM_ADDR                 (DUMP_USER_DATA_ADDR+DUMP_USER_DATA_SIZE)
#define DUMP_BBE16_TCM_SIZE                 (0xC0000)
#define DUMP_EXT_OM_DRX_CCORE_ADDR          (DUMP_BBE16_TCM_ADDR+DUMP_BBE16_TCM_SIZE)
#define DUMP_EXT_OM_DRX_CCORE_SIZE          (0x2000)
#define DUMP_TASK_TCB_ADDR                  (DUMP_EXT_OM_DRX_CCORE_ADDR+DUMP_EXT_OM_DRX_CCORE_SIZE)
#define DUMP_TASK_TCB_SIZE                  (0x1000)
#define DUMP_RSV_ADDR                       (DUMP_TASK_TCB_ADDR+DUMP_TASK_TCB_SIZE)
#define DUMP_RSV_SIZE                       (0)
#define DUMP_DYNAMIC_ADDR                   (DUMP_RSV_ADDR+DUMP_RSV_SIZE)
#define DUMP_DYNAMIC_SIZE                   (DUMP_COMM_SECTION_SIZE-(DUMP_DYNAMIC_ADDR-DUMP_COMM_SECTION_ADDR))

/* ��˹�������ַ���� */
#define DUMP_GLOBAL_INTERNAL_OFFSET          (0x460000)
#define DUMP_GLOBAL_INTERNAL_ADDR            (DDR_MNTN_ADDR_VIRT+DUMP_GLOBAL_INTERNAL_OFFSET) 
#define DUMP_GLOBAL_INTERNAL_SIZE            (0x10000)
#define DUMP_GLOBAL_BASE_INFO_OFFSET         (0x40)
#define DUMP_GLOBAL_BASE_INFO_ADDR           (DUMP_GLOBAL_INTERNAL_ADDR+DUMP_GLOBAL_INTERNAL_SIZE+DUMP_GLOBAL_BASE_INFO_OFFSET)
#define DUMP_GLOBAL_BASE_INFO_SIZE           (0X98)

/* CP AREA magic num */
#define DUMP_CP_MAGIC_NUM                   (0x4D4D4F43)

#define DUMP_INIT_FLAG_PHASE1               (0x5B5B0000)
#define DUMP_INIT_FLAG_PHASE2               (0x5B5B0001)
#define DUMP_INIT_FLAG                      (0x5B5B0002)
#define DUMP_INIT_FLAG_WAIT                 (0x5C5C5C5C)

#ifndef __ASSEMBLY__
/**************************************************************************
  STRUCT����
**************************************************************************/
#define MAX_TRC_ARGS        20
#define MAX_SCAN_DEPTH      2000
#define MAX_TRACE_DEPTH		40

#define OFFSET_MASK         0x00000fff
#define INSTR_MASK          0xfffff000
#define STMDB_SP_MASK       0xffffc000
#define PUSH_SP_MASK        0xfffff000
#define FP_MASK             0x00000800
#define VALUE_MASK          0x000000ff
#define ROTATION_MASK       0x00000f00
#define MULEQ_SP_MASK       0xffff00f0

#define STDDB_SP_INSTR      0xe92d4000
#define PUSH_SP_INSTR       0xe92dd000
#define SUB_SP_SP_IMM_INSTR 0xe24dd000
#define STR_LR_SP_INSTR     0xe52de004
#define SUB_SP_SP_IP_INSTR  0xe04dd00c
#define MOV_IP_IMM_INSTR    0xe3a0c000
#define LDR_IP_PC_IMM_INSTR 0xe59fc000
#define MULEQ_SP_INSTR      0x000d0090

#define BOGUS_SP(sp, low, high)	((!ALIGNED(sp, _STACK_ALIGN_SIZE)) || ((sp) <= (low)) || ((sp) >= (high)))
#define PC_VALIDATE(pc, pCtx)	pCtx->pcValidateRtn((pc), pCtx)

#ifndef FUNC_VOID
typedef void (*func_void_t)(void);
#define FUNC_VOID func_void_t
#endif
//typedef s32 (*FUNCPTR)(void);

struct hpm_tem_print
{
    unsigned int hpm_hvt_opc;
	unsigned int hpm_uhvt_opc;
	unsigned int hpm_svt_opc;
	signed int	 temperature;
	unsigned char up_volt;
};

typedef struct trc_lvl_info
{
    char *	fp;		    /* frame pointer */
    INSTR *	prevPc;		/* call from PC */
    INSTR *	funcAddr;	/* function address */
} TRC_LVL_INFO;

typedef struct
{
    char *  buffer;
    u32 bufferLen;
    FUNCPTR callPrintFunc;
}UserTraceCtx;

typedef struct _dump_ctrl_s
{
    u32 current_task_info;           /*0xA0 */
}dump_ctrl_s;

typedef struct
{
    u32 wdtSwitch;
    u32 wdtTimerOut;  /*���Ź���ʱʱ�䣬��λms*/
    u32 sys_view;

    union
    {
        u32 uintValue;
        struct
        {
            u32 sci_switch     : 1; /* 00: excdump, 01: usbdump, 1x: no dump */
            u32 usb_switch     : 1; /* 2 ARM�쳣��⿪��*/
            u32 pmu_switch     : 1; /* 3 ��ջ�����⿪��*/
            u32 sd_switch      : 1; /* 3 ��ջ�����⿪��*/
            u32 icc_switch     : 1; /* 3 ��ջ�����⿪��*/
            u32 ifc_switch     : 1; /* 4 �����л���¼����*/
            u32 reserved1      : 26;
        } Bits;
    } mod_cfg;
} OM_NV;

typedef struct
{
    unsigned long tid;
    char          taskname[12];
} task_name_t;

/**************************************************************************
  UNION����
**************************************************************************/

/**************************************************************************
  OTHERS����
**************************************************************************/
#define dump_debug(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_ERROR,   BSP_MODU_DUMP, "[dump]: <%s> "fmt, __FUNCTION__, ##__VA_ARGS__))
#define dump_warning(fmt, ...)  (bsp_trace(BSP_LOG_LEVEL_WARNING, BSP_MODU_DUMP, "[dump]: <%s> "fmt, __FUNCTION__, ##__VA_ARGS__))
#define dump_error(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_ERROR,   BSP_MODU_DUMP, "[dump]: <%s> "fmt, __FUNCTION__, ##__VA_ARGS__))
#define dump_fetal(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_FATAL,   BSP_MODU_DUMP, "[dump]: <%s> "fmt, __FUNCTION__, ##__VA_ARGS__))


/**************************************************************************
  ��������
**************************************************************************/



#endif/*#ifndef __ASSEMBLY__*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif    /* End #define __BSP_DUMP_H__ */





