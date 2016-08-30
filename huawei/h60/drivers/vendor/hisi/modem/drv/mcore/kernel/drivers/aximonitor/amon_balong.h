
#ifndef _AMON_BALONG_H_
#define _AMON_BALONG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp_memmap.h"
#include "osl_types.h"
#include "drv_amon.h"
#include "hi_amon.h"
#include "bsp_shared_ddr.h"

/**************************************************************************
                                 �Ĵ�������
**************************************************************************/
/* AXI monitor���������÷���: CPUFAST, SOC */
#define AXI_MON_CPUFAST_BASE_ADDR       HI_AMON_CPUFAST_REGBASE_ADDR_VIRT /* AXI monitor CPUFAST����ַ */
#define AXI_MON_SOC_BASE_ADDR           HI_AMON_SOC_REGBASE_ADDR_VIRT     /* AXI monitor SOC����ַ */

/* AXI monitor�Ĵ���ƫ�Ƶ�ַ���� begin */
#define AXI_MON_CNT_RESET               HI_AMON_CNT_RESET_OFFSET                /* AXI monitorͳ�Ƽ�������λ�Ĵ���  */
#define AXI_MON_CNT_CTRL                HI_AMON_CNT_CTRL_OFFSET                 /* AXI monitorϵͳ���üĴ��� */
#define AXI_MON_CNT_STATE_INT           HI_AMON_CNT_STATE_INT_OFFSET            /* AXI monitorϵͳ״̬�Ĵ��� */
#define AXI_MON_WIN_COUNTER             HI_AMON_WIN_COUNTER_OFFSET              /* AXI monitorϵͳʱ�䴰������ */
#define AXI_MON_IDLE_COUNTER            HI_AMON_IDLE_COUNTER_OFFSET             /* AXI monitorϵͳidleͳ�Ƽ����� */
#define AXI_MON_PORT_SEL                HI_AMON_PORT_SEL_OFFSET                 /* AXI monitor���ID��AXI port���� */
#define AXI_MON_INT_CLR                 HI_AMON_INT_CLR_OFFSET                  /* AXI monitor�ж�����Ĵ��� */
#define AXI_ID_INT_SRC                  HI_AMON_ID_INT_SRC_OFFSET               /* ID��д����ж�ԭʼ�Ĵ��� */
#define AXI_ID_INT_MASK                 HI_AMON_ID_INT_MASK_OFFSET              /* ID��д����ж����μĴ��� */
#define AXI_ID_INT_STAT                 HI_AMON_ID_INT_STAT_OFFSET              /* ID��д����ж�״̬�Ĵ��� */

/* nΪport��ȡֵ��Χ0-7 */
#define AXI_MON_RD_WAIT_CYCLE_PORTS(n)  (HI_AMON_RD_WAIT_CYCLE_PORTS_0_OFFSET + n*0x10)   /* ���PORTsΪ��ȷ��λ���ȴ����������ĵ�ʱ�����ڼĴ��� */
#define AXI_MON_WR_WAIT_CYCLE_PORTS(n)  (HI_AMON_WR_WAIT_CYCLE_PORTS_0_OFFSET + n*0x10)   /* ���PORTsΪ��ȷ��λд�ȴ����������ĵ�ʱ�����ڼĴ��� */
/* nΪID, ȡֵ��Χ0-7 */
#define AXI_MON_CNT_TYPE(n)             (HI_AMON_CNT_TYPE_0_OFFSET + n*0x4)         /* ���ID�ļ������ѡ�� */
#define AXI_MON_CNT_ID(n)               (HI_AMON_CNT_ID_0_OFFSET + n*0x100)         /* AXI monitorϵͳID��Ϣ�Ĵ��� */
#define AXI_MON_ID_ADDR_DES(n)          (HI_AMON_ID_ADDR_DES_0_OFFSET + n*0x100)    /* ��ص�ַ�±߽� */
#define AXI_MON_ID_ADDR_DES_M(n)        (HI_AMON_ID_ADDR_DES_M_0_OFFSET + n*0x100)  /* ��ص�ַ�ϱ߽� */

/* ͳ�Ƽ����� begin */
#define AXI_MON_INCR1_ID(n)             (HI_AMON_INCR1_ID_0_OFFSET + n*0x100)               /* ���ID n����INCR1���������� */
#define AXI_MON_INCR2_ID(n)             (HI_AMON_INCR2_ID_0_OFFSET + n*0x100)               /* ���ID n����INCR2���������� */
#define AXI_MON_INCR4_ID(n)             (HI_AMON_INCR4_ID_0_OFFSET + n*0x100)               /* ���ID n����INCR4���������� */
#define AXI_MON_INCR8_ID(n)             (HI_AMON_INCR8_ID_0_OFFSET + n*0x100)               /* ���ID n����INCR8���������� */
#define AXI_MON_INCR16_ID(n)            (HI_AMON_INCR16_ID_0_OFFSET + n*0x100)              /* ���ID n����INCR16���������� */
#define AXI_MON_WRAP_ID(n)              (HI_AMON_WRAP_ID_0_OFFSET + n*0x100)                /* ���ID n����WRAP���������� */
#define AXI_MON_BURST_ID(n)             (HI_AMON_BURST_ID_0_OFFSET + n*0x100)               /* ���ID n����BURST���������� */
#define AXI_MON_FINISH_ID(n)            (HI_AMON_FINISH_ID_0_OFFSET + n*0x100)              /* ���ID n���BURST���������� */
#define AXI_MON_RD_WAIT_ID_LOW(n)       (HI_AMON_READ_WAIT_ID_0_OFFSET + n*0x100)           /* ���ID n���ȴ����ڼ���������32λ(64λ�Ĵ�����0~43��Ч) */
#define AXI_MON_RD_WAIT_ID_HIGH(n)      (HI_AMON_READ_WAIT_ID_0_OFFSET + 0x4 + n*0x100)     /* ���ID n���ȴ����ڼ���������32λ(64λ�Ĵ�����0~43��Ч) */
#define AXI_MON_WR_WAIT_ID_LOW(n)       (HI_AMON_WRITE_WAIT_ID_0_OFFSET + n*0x100)          /* ���ID nд�ȴ����ڼ���������32λ(64λ�Ĵ�����0~43��Ч) */
#define AXI_MON_WR_WAIT_ID_HIGH(n)      (HI_AMON_WRITE_WAIT_ID_0_OFFSET + 0x4 + n*0x100)    /* ���ID nд�ȴ����ڼ���������32λ(64λ�Ĵ�����0~43��Ч) */
#define AXI_MON_WR_WAIT_RESP_ID_LOW(n)  (HI_AMON_WRITE_WAIT_RESP_ID_0_OFFSET + n*0x100)     /* ���ID nд��Ӧ�ȴ����ڼ���������32λ(64λ�Ĵ�����0~43��Ч) */
#define AXI_MON_WR_WAIT_RESP_ID_HIGH(n) (HI_AMON_WRITE_WAIT_RESP_ID_0_OFFSET + 0x4 + n*0x100)/* ���ID nд��Ӧ�ȴ����ڼ���������32λ(64λ�Ĵ�����0~43��Ч) */
#define AXI_MON_RD_MAX_WAIT_ID(n)       (HI_AMON_READ_MAX_WAIT_ID_0_OFFSET + n*0x100)       /* ���ID n�����ȴ����ڼ����� */
#define AXI_MON_WR_MAX_WAIT_ID(n)       (HI_AMON_WRITE_MAX_WAIT_ID_0_OFFSET + n*0x100)      /* ���ID nд���ȴ����ڼ����� */
#define AXI_MON_WR_MAX_WAIT_RESP_ID(n)  (HI_AMON_WRITE_MAX_WAIT_RESP_ID_0_OFFSET + n*0x100) /* ���ID nд��Ӧ���ȴ����ڼ����� */
#define AXI_MON_RD_BYTES_ID_LOW(n)      (HI_AMON_READ_BYTES_ID_0_OFFSET + n*0x100)          /* ���ID n������������byteΪ��λ����32λ(64λ�Ĵ�����0~39��Ч) */
#define AXI_MON_RD_BYTES_ID_HIGH(n)     (HI_AMON_READ_BYTES_ID_0_OFFSET + 0x4 + n*0x100)    /* ���ID n������������byteΪ��λ����32λ(64λ�Ĵ�����0~39��Ч) */
#define AXI_MON_WR_BYTES_ID_LOW(n)      (HI_AMON_WRITE_BYTES_ID_0_OFFSET + n*0x100)         /* ���ID nд����������byteΪ��λ����32λ(64λ�Ĵ�����0~39��Ч) */
#define AXI_MON_WR_BYTES_ID_HIGH(n)     (HI_AMON_WRITE_BYTES_ID_0_OFFSET + 0x4 + n*0x100)   /* ���ID nд����������byteΪ��λ����32λ(64λ�Ĵ�����0~39��Ч) */
/* ͳ�Ƽ����� end */
/* AXI monitor�Ĵ���ƫ�Ƶ�ַ���� end */

#define AXI_MON_BACK_ADDR               (SHM_MEM_M3_MNTN_ADDR + 32*1024)
#define AXI_MON_BACK_SIZE               (2*1024)
/**************************************************************************
                               ���ݽṹ����
**************************************************************************/
/* AXI����ѡ�� */
enum axi_config_enum
{
    AXI_CPUFAST_CONFIG,         /* CPUFAST���� */
    AXI_SOC_CONFIG,             /* SOC���� */
    AXI_CONFIG_BUTT
};
typedef unsigned int axi_config_enum_uint32;

/* AXI monitor״̬ */
enum axi_state_enum
{
    AXI_IDLE            = 0x0,  /* IDLE״̬ */
    AXI_WIN_RUNNING     = 0x4,  /* ��ʱ�䴰�ļ��״̬ */
    AXI_UNWIN_RUNNING   = 0x8,  /* ����ʱ�䴰�ļ��״̬ */
    AXI_STOP            = 0xC,  /* STOP״̬ */
    AXI_RESET_FINISH    = 0x10, /* ��λ��� */
    AXI_STATE_BUTT      = 0xFFFFFFFF
};
typedef unsigned int axi_state_enum_uint32;

/* �͹���: �Ĵ������ݡ��ָ� */
typedef struct
{ 
    u32     ctrl_reg;                           /* ���ƼĴ��� begin */
    u32     port_reg;
    u32     int_mask;
    u32     id_reg[AXI_MAX_CONFIG_ID];
    u32     addr_start_reg[AXI_MAX_CONFIG_ID];
    u32     addr_end_reg[AXI_MAX_CONFIG_ID];   /* ���ƼĴ��� end */
    u32     incr1_reg[AXI_MAX_CONFIG_ID];      /* ͳ�ƼĴ��� begin */
    u32     incr2_reg[AXI_MAX_CONFIG_ID]; 
    u32     incr4_reg[AXI_MAX_CONFIG_ID];
    u32     incr8_reg[AXI_MAX_CONFIG_ID]; 
    u32     incr16_reg[AXI_MAX_CONFIG_ID]; 
    u32     wrap_reg[AXI_MAX_CONFIG_ID]; 
    u32     burst_send_reg[AXI_MAX_CONFIG_ID]; 
    u32     burst_fin_reg[AXI_MAX_CONFIG_ID]; 
    u32     rd_cnt_low_reg[AXI_MAX_CONFIG_ID];
    u32     rd_cnt_high_reg[AXI_MAX_CONFIG_ID];
    u32     wr_cnt_low_reg[AXI_MAX_CONFIG_ID];
    u32     wr_cnt_high_reg[AXI_MAX_CONFIG_ID];/* ͳ�ƼĴ��� end */   
} axi_reg_bak_t;
void bsp_amon_suspend();
void bsp_amon_resume();


#ifdef __cplusplus
}
#endif

#endif /* _AMON_BALONG_H_ */

