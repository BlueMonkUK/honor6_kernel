/*************************************************************************
*   ��Ȩ����(C) 2008-2013, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  kdf_balong.h
*
*   ��    �� :  wangxiandong
*
*   ��    �� :  kdf����ʵ��ͷ�ļ�
*
*   �޸ļ�¼ :  2013��03��12��  v1.00  wangxiandong ����
*************************************************************************/
#ifndef	_CIPHER_KDF_H
#define	_CIPHER_KDF_H

#include "bsp_cipher.h"
#include "cipher_balong_common.h"


/* KDF��غ궨��*/
#define KDF_CHN_BD_NUM         0x10
#define KDF_CHN_RD_NUM         KDF_CHN_BD_NUM
#define KDF_CHN_NUM            0x4
#define KDF_CHN_USRFIELD_LEN   0x0
#define KDF_SHA_KEY_LENGTH     0x20
#define KDF_SHA_S_LENGTH_MAX   0x200

enum kdf_op_type_e
{
    kdf_op_keymake = 0,
    kdf_op_write = 1,
    kdf_op_read = 2,
    kdf_op_max
};

/* KDFʹ�õ�BD�ṹ*/
struct kdf_bd_s
{
    u32 kdf_config;
    u32 address;
};
/* KDFʹ�õ�RD�ṹ*/
struct kdf_rd_s
{
    u32 address;
    u32 kdf_config;
};
/* KDFͨ������ṹ�� */
struct kdf_chx_mgr_s
{   
    struct kdf_bd_s *p_bd_base;          /* ��ǰͨ��BD����������ַ */
    struct kdf_rd_s *p_rd_base;          /* ��ǰͨ��RD����������ַ */
    CIPHER_MUTEX_T mtx_chn_opt;          /* KDF���Ļ����� */
};


struct kdf_bdconfig_info_s
{
	u32	sha_key_index;                    /*sha_key��Դ��KeyRamʱ������keyRam�е�����*/
	u32	sha_s_index;                      /*sha_s��Դ��KeyRamʱ������keyRam�е�����*/
	u32 length;                           /*KDF������Ϊsha_s�ĳ���*/
	u32 dest_index;                       /*sha_result_sn��key_ram�е�λ��*/
	void * p_address;                     /*Ŀ��(Դ)��ַ*/
	enum kdf_op_type_e opt_type;          /*��������*/
	KDF_SHA_KEY_SOURCE_E sha_key_source;  /*sha_key��Դ*/
	KDF_SHA_S_SOURCE_E sha_s_source;      /*sha_s��Դ*/
};

void cipher_set_kdf_debug_level(s32 level);
static void cipher_kdf_chx_mag_reset();

#endif
