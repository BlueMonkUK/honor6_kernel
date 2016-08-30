
#ifndef	_CIPHER_BALONG_H
#define	_CIPHER_BALONG_H


#include "bsp_cipher.h"
#include "cipher_balong_common.h"
#include "kdf_balong.h"
#include "bsp_dpm.h"
#include "bsp_om.h"
#include <osl_spinlock.h>

/*=======================================�궨��================================================*/
#define CIPHER_MAX_CHN          0x4 /*ͨ��1��2��3��0ͨ������*/
#define CIPHER_DRB_CHN          0x3
#define CIPHER_MIN_CHN          0x1 /*��ͨ��1��ʼ*/

/* ����buffer list�ڵ������󱣻�ֵ ��ÿ��BD����ӵ�е����CD��*/
#define CIPHER_CHECK_CDLIST_CNT   512
/*ͨ��2��3���ȼ��Զ��л�������ֵ*/
#define CIPHER_CHN_STRMTHRESHHLD  4096
/*ͨ��RD����ж��ж�*/
#define CIPHER_INTR_CHN_COMPL(chn, status) ((0x8U << ((chn) << 3)) & (status))
/* Buffer List ������װ ��Ҫ����1/2/3ͨ������ṹ�� */
#define CIPHER_BUFLIST_GET_INBUF_PTR(p_node, chn) \
    ((u8*) (*((u32*) (((u32)(p_node)) + sg_module_mgr.chx_mgr[chn].in_buff_addr_oft))))

#define CIPHER_BUFLIST_GET_INBUF_LEN(p_node, chn) \
    ((u32) (*((u32*) (((u32)(p_node)) + sg_module_mgr.chx_mgr[chn].in_buff_len_oft))))

#define CIPHER_BUFLIST_GET_INBUFNEXT_PTR(p_node,chn) \
    ((u8*) (*((u32*) (((u32)(p_node)) + sg_module_mgr.chx_mgr[chn].in_buff_next_oft))))

#define CIPHER_BUFLIST_GET_OUTBUF_PTR(p_node, chn) \
    ((u8*) (*((u32*) (((u32)(p_node)) + sg_module_mgr.chx_mgr[chn].out_buff_addr_oft))))

#define CIPHER_BUFLIST_GET_OUTBUF_LEN(p_node, chn) \
    ((u32) (*((u32*) (((u32)(p_node)) + sg_module_mgr.chx_mgr[chn].out_buff_len_oft))))

#define CIPHER_BUFLIST_GET_OUTBUFNEXT_PTR(p_node,chn) \
    ((u8*) (*((u32*) (((u32)(p_node)) + sg_module_mgr.chx_mgr[chn].out_buff_next_oft))))
/*����CD������*/
#define CIPHER_CFG_BUF_LIST(node, buf_ptr, buf_len, b_last) \
    do{\
        (node).pt_attr = (                           \
             ((u32)(buf_len) & 0xFFFF) |            \
             (((u32)(b_last) & 0x1)) << 0x10);      \
        (node).pt = (u32)(buf_ptr);                   \
    }while(0)

#define CIPHER_ERROR_PRINT(errno) \
do{\
    {\
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER, "[CIPHER API ERROR]:\n\t\t\t <func>: %s;  <line>: %d;  <Errno>: %s (0x%08x)\n\n", \
               (int)__FUNCTION__, (int)__LINE__, (int)#errno, errno);\
    }\
}while(0)


/*=================�ṹ�嶨��=============*/
/* CIPHER BD�������ṹ */
struct cipher_bd_s
{
	u32 cipher_cfg;           /*Cipher config*/
	u32 input_attr;           /*Input Attr*/
	u32 output_attr;          /*Output Attr*/
	u32 input_addr;           /*Input Addr*/
	u32 output_addr;          /*Output Addr*/
	u32 aph_attr;             /*Aph for IV*/
	u32 count;                /*count for IV*/
	u32 usr_field1;           /*Usr Field1*/
	u32 usr_field2;           /*Usr Field2*/
	u32 usr_field3;           /*Usr Field3*/
};
/* CIPHER RD�������ṹ*/
struct cipher_rd_s
{
	u32 input_attr;           /*Input Attr*/
	u32 output_attr;          /*Output Attr*/
	u32 input_addr;           /*Input Addr*/
	u32 output_addr;          /*Output Addr*/
	u32 usr_field1;           /*Usr Field1*/
	u32 usr_field2;           /*Usr Field2*/
	u32 usr_field3;           /*Usr Field3*/
	u32 cipher_cfg;           /*Cipher Config*/
};
/*CD�������ṹ*/
struct cipher_cd_s
{
	u32 pt;                  /*ָ��*/
	u32 pt_attr;             /*ָ������*/
};
/* ͨ��1/2/3����ȫ�ֽṹ�� */
struct cipher_chx_mgr_s
{
	u32	chx_bd_number;						  /*��ǰͨ����BD��RD�ĸ���[BD��RD������ͬ]*/
	u32 cd_number_per_bd;				      /*RD��CD������BD��ͬ*/

	struct cipher_bd_s *p_bd_base;            /* ��ǰͨ��BD����������ַ */
	struct cipher_rd_s *p_rd_base;            /* ��ǰͨ��RD����������ַ */
	struct cipher_cd_s *p_cdlist_base;        /* ��ǰͨ��CD List����ַ */

    CIPHER_FREEMEM_CB_T p_func_free_inmem;    /* ����Buffer�ͷ�Callback����*/
    CIPHER_FREEMEM_CB_T p_func_free_outmem;   /* ���Buffer�ͷ�Callback����*/

    u32 in_buff_addr_oft;                     /* ����Buff��ַ��TTF�ṹ�е�ƫ��*/
    u32 in_buff_len_oft;                      /* ����Buff������TTF�ṹ�е�ƫ��*/
    u32 in_buff_next_oft;                     /* ��һ��TTF��TTF�ṹ�е�ƫ��*/
    u32 out_buff_addr_oft;                    /* ���Buff��ַ��TTF�ṹ�е�ƫ��*/
    u32 out_buff_len_oft;                     /* ���Buff������TTF�ṹ�е�ƫ��*/
    u32 out_buff_next_oft;                    /* ��һ��TTF��TTF�ṹ�е�ƫ��*/

    s32 b_purging;                            /*�Ƿ��������ͨ����־*/
    CIPHER_MUTEX_T mtx_chn_opt;               /* Cipher�����Ļ����� */

	u32 cd_list_start_addr;                   /*��ͨ��CD����ĵ�һ������CD��ַ*/
	u32 cd_list_end_addr;                     /*��ͨ��CD List�����һ������CD��ַ*/
};
/* CIPHERģ�����ȫ�ֽṹ�� */
struct cipher_mdl_mgr_s
{
    u32 reg_base_addr;                                /* ģ��Ĵ�������ַ */ 
    enum CIPHER_STATUS_E status;                      /* ��ʼ��״̬ */ 
    CIPHER_NOTIFY_CB_T p_func_notify;                 /* �û�ע���CallBack���� */
    struct cipher_chx_mgr_s chx_mgr[CIPHER_MAX_CHN];  /* ͨ������ṹ */
	struct kdf_chx_mgr_s    kdf_chx_mgr;              /* KDFͨ������ṹ */
	
    CIPHER_KEY_LEN_E key_type_len[CIPHER_KEY_NUM];    /* ����Key�ĳ���*/
    s32 debug_level;                                  /* ���Լ�������*/
	
	u32 bd_full_count[CIPHER_MAX_CHN];	              /* BD FIFO�ռ���ͳ�Ƽ���*/
	u32 single_count[CIPHER_MAX_CHN];	              /* ��ͨ�����õ��������Ĵ���*/
	u32 rela_count[CIPHER_MAX_CHN];	                  /* ��ͨ�����ù��������Ĵ���*/
	u32 invalid_rd_number[CIPHER_MAX_CHN];	          /* ��ЧRD�����Ĵ���*/

	u32 time_out_bd1[CIPHER_MAX_CHN];	              /* BD����ʱ�����Ĵ���,��дָ�벻��*/
	u32 time_out_bd2[CIPHER_MAX_CHN];	              /* BD����ʱ�����Ĵ���������û�����*/
	u32 time_out_kdf1;                                /* KDFͨ������ʱ�����Ĵ���,��дָ�벻��*/
	u32 time_out_kdf2;                                /* KDFͨ������ʱ�����Ĵ���������û�����*/

	u32 key_length_table[CIPHER_KEY_LEN_BUTTOM];      /* {0x10, 0x18, 0x20}*/

	u32 sgl_key_len[CIPHER_KEY_LEN_BUTTOM];           /* {0x2, 0x3, 0x4}*/
	u8 security_op[CIPHER_SINGLE_OPT_BUTTOM];         /* {1,1,0,0,0,0,0,0};*/
	u8 integrity_op[CIPHER_SINGLE_OPT_BUTTOM];        /* {0,0,1,1,1,1,1,1};*/
	/* ��������*/
	u8 single_dirt_tbl[CIPHER_SINGLE_OPT_BUTTOM];     /* {0,1,0,0,1,1,1,1};*/
	u8 sig_op_security_tbl[CIPHER_SINGLE_OPT_BUTTOM]; /* {1,1,0,0,0,0,0,0};*/
	u8 sig_op_integrity_tbl[CIPHER_SINGLE_OPT_BUTTOM];/* {0,0,1,1,1,1,1,1};*/
	u8 mac_position_single[CIPHER_SINGLE_OPT_BUTTOM]; /* {0,0,0,0,1,0,1,0};*/
	u8 mac_length_single[CIPHER_SINGLE_OPT_BUTTOM];   /* {0,0,1,0,1,1,0,0};*/
	/* ��������*/
	u8 rela_dirt_tbl[CIPHER_RELA_OPT_BUTTOM];         /* {0,1,0,1};*/
	u8 rela_op_security_tbl[CIPHER_RELA_OPT_BUTTOM];  /* {1,1,1,1};*/
	u8 rela_op_integrity_tbl[CIPHER_RELA_OPT_BUTTOM]; /* {1,1,1,1};*/
	u8 rela_op_first[CIPHER_RELA_OPT_BUTTOM];         /* {1,0,0,1};*/
	u8 mac_position_rela[CIPHER_RELA_OPT_BUTTOM];     /* {0,0,1,1};*/
	/* ����*/
	u8 inte_alg[CIPHER_ALG_BUTTOM];                   /* {0,1,2,3,4,5};*/
	u8 secu_alg[CIPHER_ALG_BUTTOM];                   /* {0,1,2,3,4,5};*/
	u8 int_enable_tbl[CIPHER_SUBM_BUTTOM];            /* {0,0,1};*/
	u8 herder_len_tbl[CIPHER_HDR_BIT_TYPE_BUTT];      /* {0,1,1,2,1,1,2};*/
	u8 aph_len_tbl[CIPHER_HDR_BIT_TYPE_BUTT];         /* {0,1,1,2,1,1,2};*/
	u8 aph_attr_tbl[CIPHER_HDR_BIT_TYPE_BUTT];        /* {0,1,1,1,0,0,0};*/
	u32 aph_mast_tbl[CIPHER_HDR_BIT_TYPE_BUTT];       /* {0x0,0x1f,0x7f,0xfff,0x1f,0x7f,0xfff};*/
	struct clk * cifer_clk;                           /* cipherʱ�� */

	u32 key_ram_bak_buf[128];                         /*���ڵ͹���ʱ����key ram��Ϣ*/
	u32 suspend_enable;								  /* ָʾ�Ƿ���Ҫ suspend/resume cipher*/
	s32 enabled;                                      /* 1:enable���������ù�;0:δ������ */
	struct spinlock spin_lock_irq;
	struct spinlock spin_lock_reg_conf;
};

/*=======================��������=======================*/
void cipher_set_debug_level(s32 level);
static void cipher_isr();
static void cipher_chn_mag_reset(u32 chx);
int cipher_open_clk(void);
int cipher_close_clk(void);
int cipher_reg_set(void);

static s32 cipher_init_kdf_chx_bdq_mem(u32 * org_kdf_bdq_addr);
static s32 cipher_init_kdf_chx_rdq_mem(u32 * org_kdf_rdq_addr);
s32 bsp_cipher_suspend(struct dpm_device *dev);
s32 bsp_cipher_resume(struct dpm_device *dev);

#endif
