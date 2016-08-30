/*************************************************************************
*   ��Ȩ����(C) 1987-2004, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  OcrShare.h
*
*   ��    �� :  wuzechun
*
*   ��    �� :  OnChipRom��BootLoader����������
*
*   �޸ļ�¼ :  2011��4��15��  v1.00  wuzechun  ����
*
*************************************************************************/

#ifndef __OCRSHARE_H__
#define __OCRSHARE_H__

#include "types.h"
#include "config.h"

/*--------------------------------------------------------------*
 * �궨��                                                    *
 *--------------------------------------------------------------*/

/* ֻ��OEM CA�в���OEM ID��HWID,��CA���� */
#define KEY_OEMID_INDEX        sizeof(KEY_STRUCT)
#define KEY_HWID_INDEX         (KEY_OEMID_INDEX+0x04)

#define ROOT_CA_LEN            260      /*CA֤��ĳ��� 260 Byte*/
#define OEM_CA_LEN             268      /*CA֤��ĳ��� 268 Byte*/
#define IDIO_LEN               128      /*ǩ���ĳ��� 128 Byte*/

#define MAX_N_LENGTH    32

#define SHA256_HASH_SIZE  8       /*HASHǩ��ռ��word��*/

#define MD5_HASH_SIZE   4       /* MD5ժҪ��ռ��word�� */

#define NAND_ARGS_FULL_AVAIL	0x3a
#define NAND_ARGS_PART_AVAIL	0x45

/*--------------------------------------------------------------*
 * ���ݽṹ                                                     *
 *--------------------------------------------------------------*/

/* ��Կ��ʽ */
typedef struct
{
    unsigned short  eLen;    /* E������64bitsΪ��λ��ȡֵ��ΧΪ0~15��0��ʾ16 */
    unsigned short  nLen;    /* N������32bitsΪ��λ��ȡֵ��ΧΪ0~31��0��ʾ32 */
    unsigned long  e[MAX_N_LENGTH];       /* ��e(��Կ) */
    unsigned long  n[MAX_N_LENGTH];       /* ģ�� */
}KEY_STRUCT;


/* nand spec info��ÿ��������ȡֵ������ʵ��ֵ����page_size��ȡ2048��4096�ȣ�����b01\b11�� */
struct nand_spec_real
{
	unsigned long page_size;
	unsigned long page_num_per_block;
	unsigned long addr_cycle;
	unsigned long ecc_type;
	unsigned long oob_size;
	unsigned long chip_size;	/* ��MBΪ��λ */
};

/* onchiprom���nand spec info���浽SRAM��һ��32bit��buffer�У�������ÿ���ֶεĺ��� */
struct nand_spec_sram
{
	/* pagesize/ecctype/addrnum/pagenumperblock �⼸��������Ĵ���������ֵһ�� */
    unsigned int    page_size          : 4;  /* [0~3]   */
    unsigned int    ecc_type           : 4;  /* [4~7]   */
    unsigned int    addr_num           : 4;  /* [8~11]  */
    unsigned int    page_num_per_block : 4;  /* [12~15] */

	/* sparesize/chipsize�������������ܻ�ȡ���� */
	unsigned int    spare_size		   : 5;  /* [16~20] */
	unsigned int    chip_size		   : 4;	 /* [21~24] */	

	/* flag */
    unsigned int    flag           	   : 7;  /* [25~31] NAND_ARGS_FULL_AVAIL - ��ȫ��������NAND_ARGS_FULL_AVAIL - ��4��������else - bufferֵ��Ч */
};

/* ǩ��У�麯��ָ�� */
typedef int (*idioIdentifyPtr)(unsigned long dataAddr, unsigned long dataLen, KEY_STRUCT *pKey, unsigned long* pIdio);

/* ����Hashֵ����ָ�� */
typedef int (*SHA256HashPtr)(unsigned long dataAddr, unsigned long dataLen, unsigned long* hash);

/* ����RSA����ָ�� */
typedef int (*RSAPtr)(KEY_STRUCT *pKey, unsigned long* pIndata, unsigned long* pOutdata);

/* ��ȡnand flash���ĺ���ָ�룬������������ */
typedef int (*bsp_nand_get_spec_and_save_ptr)(volatile struct nand_spec_sram *spec_sram);

/* �������ݽṹ��,����AXI����,�������¼�Ԫ��(�������ǰ��) */
typedef struct tagOcrShareData
{
	struct nand_spec_sram nandspec;
	bsp_nand_get_spec_and_save_ptr bsp_nand_get_spec_and_save;	/* ������ָ�� */

    long errno;
	
    /* ������ָ�� */
    SHA256HashPtr     SHA256Hash;
    RSAPtr          RSA;
    idioIdentifyPtr idioIdentify;	

    /* �������ݶ� */
    BOOL    bSecEn;
    BOOL    bRootCaInited;
    KEY_STRUCT *pRootKey;

    unsigned long  ulEnumFlag;             /* �Ծ���λ��־,AXI Mem Top - 8 */
    unsigned long  ulOcrInitedFlag;        /* AXI Mem Top - 4 */
}tOcrShareData;                  /* AXI Mem Top */

int SHA256Hash(unsigned long dataAddr, unsigned long dataLen, unsigned long* hash);

int RSA(KEY_STRUCT *pubKey, unsigned long* pIndata, unsigned long* pOutdata);

void ocrShareSave( void );

int bsp_nand_get_spec_and_save(volatile struct nand_spec_sram *spec_sram);

#endif /* OcrShare.h */


