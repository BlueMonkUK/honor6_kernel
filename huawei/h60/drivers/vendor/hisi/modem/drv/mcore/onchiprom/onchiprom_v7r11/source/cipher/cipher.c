#ifdef __cplusplus
extern "C" {
#endif

#include "cipher.h"
#include "types.h"
#include "string.h"
#include "sys.h"

/* �����ж���غ�������*/
#define CIPHER_CHECK_PTR(ptr) \
    do{\
        if (NULL == (ptr))\
        {\
            return CIPHER_NULL_PTR;\
        }\
    }while(0)


#define CIPHER_CHECK_ENUM(en, last) \
    do{\
        if ((UINT32)(en) >= (UINT32)(last))\
        {\
            return CIPHER_INVALID_ENUM;\
        }\
    }while(0)


#define CIPHER_REG_READ(base, reg, value) (value) = (*(volatile UINT32 *)((UINT32)(base) + (reg)))
#define CIPHER_REG_WRITE(base, reg, value) (*(volatile UINT32 *)((UINT32)(base) + (reg))) = (value)

/*****************************************************************************
* �� �� ��  : CIPHER_Init
*
* ��������  : Cipher��ʼ������
*
* �������  : void
* �������  : ��
*
* �� �� ֵ  : �ɹ�/ʧ��
*
* ����˵��  : ��
*
*****************************************************************************/
INT32 CIPHER_Init(void *bdq_pt, void *rdq_pt)
{
    UINT32 u32Value = 0;
    INT32 s32Ret = CIPHER_SUCCESS;
	UINT32 i = 0;

	/*��λͨ��KDF_CHN_NUM*/
	CIPHER_REG_WRITE(CIPHER_REGBASE_ADDR, CIPHER_CHN_RESET(KDF_CHN_NUM), 7);
	for(i = 0; i < 1000; i++)
	{
		CIPHER_REG_READ(CIPHER_REGBASE_ADDR, CIPHER_CHN_RESET(KDF_CHN_NUM), u32Value);
		if(0 == (u32Value & 7))
			break;
		delay(1);/*˯1us*/
	}
	if(i == 1000)
	{
		return CIPHER_TIME_OUT;
	}

    /*ͨ��BDQ�ռ��ȡ*/
    memset(bdq_pt, 0, BDQ_SIZE);
    /*����ͨ��BDQ����ַ*/
    CIPHER_REG_WRITE(CIPHER_REGBASE_ADDR, CIPHER_CHNBDQ_BASE(KDF_CHN_NUM), (UINT32)bdq_pt);
    /*����ͨ��BDQ��С*/
    CIPHER_REG_WRITE(CIPHER_REGBASE_ADDR, CIPHER_CHNBDQ_SIZE(KDF_CHN_NUM), 1);


	/*ͨ��RDQ�ռ���0*/
    memset(rdq_pt, 0, RDQ_SIZE);
    /*����ͨ��RDQ����ַ*/
    CIPHER_REG_WRITE(CIPHER_REGBASE_ADDR, CIPHER_CHNRDQ_BASE(KDF_CHN_NUM), (UINT32)rdq_pt);
    /*����ͨ��RDQ��С*/
    CIPHER_REG_WRITE(CIPHER_REGBASE_ADDR, CIPHER_CHNRDQ_SIZE(KDF_CHN_NUM), 1);


    /* ʹ��KDFͨ��*/
    CIPHER_REG_WRITE(CIPHER_REGBASE_ADDR, CIPHER_CHN_ENABLE(KDF_CHN_NUM), 0x1);
    /* ����KDFͨ��config�Ĵ���*/
    u32Value = CHN_IV_SEL \
               | (CHN_RDQ_CTRL << CHN_RDQCTRL_BIT) \
               | KDF_CHN_USRFIELD_LEN << CHN_USRFIELDLEN_BIT;
    CIPHER_REG_WRITE(CIPHER_REGBASE_ADDR, CIPHER_CHN_CONFIG(KDF_CHN_NUM), u32Value);

    /* KeyRam�е���������Ϊ0��ֻʹ����0������λ��*/
	for(i = 0; i < 8/*1��key�8 words*/; i++)
		*(((UINT32*)(CIPHER_REGBASE_ADDR + CIPHER_KEYRAM_OFFSET)) + i) = 0;

    return s32Ret;
}

INT32 CIPHER_KdfBDConfigure(KDF_BDCONFIG_INFO_S *stKeyCfgInfo)
{
    KDF_BD_SCPT_S *pWaddr = NULL;
	UINT32 reg_val_bdq_wptr;

	CIPHER_REG_READ(CIPHER_REGBASE_ADDR, CIPHER_CHNBDQ_WRPTR(KDF_CHN_NUM), reg_val_bdq_wptr);
	pWaddr = (KDF_BD_SCPT_S *)reg_val_bdq_wptr;
	if(!pWaddr)
	{
		return CIPHER_NULL_PTR;
	}

    pWaddr->u32KdfConfig=
                    0              								|
                    (stKeyCfgInfo->enShaKeySource & 0x3) << 0x2 |
                    (stKeyCfgInfo->enShaSSource & 0x1)   << 0x4 |
                    (stKeyCfgInfo->u32ShaKeyIndex & 0xf) << 0x7 |
                    (stKeyCfgInfo->u32ShaSIndex & 0xf)   << 0xb |
                    (stKeyCfgInfo->u32Length & 0x1ff)    << 0xf |
                    (stKeyCfgInfo->u32DestIndex & 0xf)   << 0x18;

    pWaddr->u32Address = (UINT32)(stKeyCfgInfo->pAddress);

    return CIPHER_SUCCESS;
}

INT32 Cipher_StartKdfChannel()
{
    UINT32 u32RegValue = 0;
	UINT32 reg_val_bdq_size;
    KDF_RD_SCPT_S *pCurRDAddr = NULL;
	UINT32 delay_loops = 20000;/*ѭ��20000�Σ�ÿ���ӳ�1us�����ӳ�20000us*/

	CIPHER_REG_READ(CIPHER_REGBASE_ADDR, CIPHER_CHNBDQ_RWPTR(KDF_CHN_NUM), u32RegValue);
	u32RegValue &= 0x3FF;
	u32RegValue += 1;
	CIPHER_REG_READ(CIPHER_REGBASE_ADDR, CIPHER_CHNBDQ_SIZE(KDF_CHN_NUM), reg_val_bdq_size);
	/* �ƶ��Ĵ���дָ�� */
	CIPHER_REG_WRITE(CIPHER_REGBASE_ADDR, CIPHER_CHNBDQ_RWPTR(KDF_CHN_NUM), u32RegValue % (1 + reg_val_bdq_size));

	do
    {
        CIPHER_REG_READ(CIPHER_REGBASE_ADDR, CIPHER_CHN_ENABLE(KDF_CHN_NUM),u32RegValue);
		--delay_loops;
		delay(1);/*˯1us*/
    }
    while((u32RegValue & 0x80000000) && (delay_loops));
	if(!delay_loops)
	{
		return CIPHER_TIME_OUT;
	}

    /* ��ȡ��ǰ��RD������*/
	CIPHER_REG_READ(CIPHER_REGBASE_ADDR, CIPHER_CHNRDQ_RDPTR(KDF_CHN_NUM), u32RegValue);
    pCurRDAddr = (KDF_RD_SCPT_S *)u32RegValue;
    /* �ύ����Ϊ��������ʱ����Ҫ�ƶ�RD*/
    /* ���RD��Чλ*/
    pCurRDAddr->u32KdfConfig &= (~0x80000000);
    /* �ж��Ƿ��������Լ�����*/
    if(CIPHER_STAT_CHECK_ERR == ((pCurRDAddr->u32KdfConfig >> 29) & 0x3))
    {
        return CIPHER_CHECK_ERROR;
    }
	/* �ƶ��Ĵ����е�RD��ַ*/
	CIPHER_REG_READ(CIPHER_REGBASE_ADDR, CIPHER_CHNRDQ_RWPTR(KDF_CHN_NUM), u32RegValue);
	u32RegValue &= 0x3FF0000;
	u32RegValue = u32RegValue >> 16;
	u32RegValue += 1;
	u32RegValue = (u32RegValue) % (reg_val_bdq_size + 1);
	u32RegValue = u32RegValue << 16;
	CIPHER_REG_WRITE(CIPHER_REGBASE_ADDR, CIPHER_CHNRDQ_RWPTR(KDF_CHN_NUM), u32RegValue);

    return CIPHER_SUCCESS;
}

/*****************************************************************************
* �� �� ��  : BSP_KDF_KeyMake
*
* ��������  : KDF���㣬���ڲ����µ�Key
*
* �������  : stKeyCfgInfo: KDF����ʱ����sha_key��������
            : stKeyCfgInfo: KDF����ʱ����sha_s��������
            : enDestIndex : KDF���������Key������KeyRam�е�λ��
* �������  : ��
* �� �� ֵ  : �ɹ�:OK
*           : ʧ��:BSP_ERROR
*****************************************************************************/
INT32 KDF_KeyMake(KEY_CONFIG_INFO_S *pstKeyCfgInfo, S_CONFIG_INFO_S *pstSCfgInfo,
                        UINT32 u32DestIndex, KEY_MAKE_S *pstKeyMake)
{
    KDF_BDCONFIG_INFO_S stBDCfgInfo = {0};

    stBDCfgInfo.enShaKeySource = pstKeyCfgInfo->enShaKeySource;
    stBDCfgInfo.enShaSSource   = pstSCfgInfo->enShaSSource;
    stBDCfgInfo.u32ShaKeyIndex  = pstKeyCfgInfo->u32ShaKeyIndex;
    stBDCfgInfo.u32ShaSIndex    = pstSCfgInfo->u32ShaSIndex;
    stBDCfgInfo.u32Length      = pstSCfgInfo->u32ShaSLength - 1;/*Ӧ�ü�1���߼����1*/
    stBDCfgInfo.u32DestIndex   = u32DestIndex;
    stBDCfgInfo.pAddress       = pstSCfgInfo->pSAddr;

    CIPHER_KdfBDConfigure(&stBDCfgInfo);

    if(CIPHER_SUCCESS != Cipher_StartKdfChannel())
    {
    	return CIPHER_CHECK_ERROR;
    }
	/* �ϲ���Ҫ����Keyֵ��Ŀ�ĵ�ַ*/
	if(pstKeyMake->enKeyOutput == CIPHER_KEY_OUTPUT)
	{
		if(!(pstKeyMake->stKeyGet.pKeyAddr) || !(pstKeyMake->stKeyGet.penOutKeyLen))
		{
			return CIPHER_NULL_PTR;
		}

		/* ��Key�ĳ��ȷ������ϲ�*/
		(*(pstKeyMake->stKeyGet.penOutKeyLen)) = (UINT32)CIPHER_KEY_L256;

		/* ���ʵ��Key�ĳ��ȴ����ϲ�ϣ���ĳ��ȣ����ش���*/
		if((UINT32)CIPHER_KEY_L256 > (pstKeyMake->stKeyGet.enKeyLen))
		{
			return CIPHER_KEYLEN_ERROR;
		}
	}
    return CIPHER_SUCCESS;
}

int bsp_calc_hash(unsigned long * s_addr, long size, unsigned long * key_addr,
	unsigned long * out_hash, unsigned long * out_hash_len)
{
	struct Data_Buff BD_RD_BUFF = {{0}, {0}};
	KEY_CONFIG_INFO_S key_input = {0};
	S_CONFIG_INFO_S s_input = {0};
	int ret = CIPHER_SUCCESS;
	UINT32	index = 0;
	KEY_MAKE_S key_make_out = {0};
	KEY_GET_S key_get = {0};
	int i = 0;

	if(!s_addr || !size || !key_addr || !out_hash || ! out_hash_len)
	{
		return CIPHER_NULL_PTR;
	}

	key_input.enShaKeySource = SHA_KEY_SOURCE_KEYRAM;
	key_input.pKeySourceAddr = key_addr;
	key_input.u32ShaKeyIndex = index;

	s_input.enShaSSource = SHA_S_SOURCE_DDR;
	s_input.pSAddr = s_addr;
	s_input.u32ShaSLength = size >= KDF_SHA_S_LENGTH_MAX ? KDF_SHA_S_LENGTH_MAX : size;
	s_input.u32ShaSIndex = index;

	key_get.enKeyLen = 256;
	key_get.pKeyAddr = out_hash;
	key_get.penOutKeyLen = out_hash_len;
	*(key_get.penOutKeyLen) = 0;

	key_make_out.enKeyOutput = CIPHER_KEY_OUTPUT;
	key_make_out.stKeyGet = key_get;

	CIPHER_Init(&(BD_RD_BUFF.BD_BUFF[0]), &(BD_RD_BUFF.RD_BUFF[0]));
	/*�������key�ȷ���key ram��0��λ��*/
	memcpy((void*)(CIPHER_REGBASE_ADDR + CIPHER_KEYRAM_OFFSET), key_addr, 32);//key��󳤶�256bit����32bytes

	do
	{
		ret = KDF_KeyMake(&key_input, &s_input, index, &key_make_out);
		if(CIPHER_SUCCESS != ret)
			return ret;

		size -= s_input.u32ShaSLength;
		s_addr = (unsigned long *)(((UINT8*)s_addr) + s_input.u32ShaSLength);
		s_input.pSAddr = s_addr;

		key_input.enShaKeySource = SHA_KEY_SOURCE_RESULT;
		s_input.u32ShaSLength = size >= KDF_SHA_S_LENGTH_MAX ? KDF_SHA_S_LENGTH_MAX : size;
	}while(size > 0);

	/* ���KeyMake���ɵ�Key�ĵ�ַ*/
	memcpy(out_hash, (const void *)(CIPHER_REGBASE_ADDR + CIPHER_KEYRAM_OFFSET), CIPHER_KEY_LEN);
	/*��ת�����*/
	for(i = 0; i < CIPHER_KEY_LEN/sizeof(UINT32); i++)
	{
		*(out_hash + i) = CIPHER_BSWAP32(*(out_hash + i));
	}

	return ret;
}

#ifdef __cplusplus
}
#endif


