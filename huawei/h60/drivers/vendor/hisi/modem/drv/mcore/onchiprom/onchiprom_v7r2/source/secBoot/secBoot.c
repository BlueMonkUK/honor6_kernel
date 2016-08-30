/*************************************************************************
*   ��Ȩ����(C) 1987-2004, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  secBoot.c
*
*   ��    �� :  wuzechun
*
*   ��    �� :  ��ȫ�����������̿���
*
*   �޸ļ�¼ :  2011��6��9��  v1.00  wuzechun  ����
*
*************************************************************************/

#include "sys.h"
#include "string.h"
#include "secBoot.h"
#include "OnChipRom.h"
#include "efuse.h"
#include "emmcMain.h"
#include "hsuart.h"
#include "apDl2.h"
#include "spiDev.h"
#include "ioinit.h"
#include "md5.h"

extern void usb3_driver_init(void);
extern void hsic_works(void);

/*****************************************************************************
* �� �� ��  : go
*
* ��������  : ��ת����
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
void go(FUNCPTR entry)
{
    (entry) ();
}

/*****************************************************************************
* �� �� ��  : usbBoot
*
* ��������  : usb�Ծ�
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
void usbBoot()
{
    print_info("\r\nUSB_boot!" );

    /* ���־��Ч���Ծٹܽ�Ϊ�͵�ƽ�������Ծ�����*/
    usb3_driver_init();

    /* USB�Ծ��˳�,��λϵͳ,���³��� */
    print_info("\r\nUSB_boot returns, reboot..." );

    setErrno(SYS_ERR_USB_BOOT_RETURNS);

    wdtRebootDelayMs(TIME_DELAY_MS_1000_FOR_UBOOT_RETURN);
}
/*****************************************************************************
* �� �� ��  : hsicBoot
*
* ��������  : hsic�Ծ�
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
void hsicBoot()
{
    /* ���־��Ч���Ծٹܽ�Ϊ�͵�ƽ�������Ծ�����*/
    hsic_works();

    /* USB�Ծ��˳�,��λϵͳ,���³��� */
    print_info("\r\nHSIC_boot returns, reboot..." );

    setErrno(SYS_ERR_HSIC_BOOT_RETURNS);

    wdtRebootDelayMs(TIME_DELAY_MS_1000_FOR_UBOOT_RETURN);
}
/*****************************************************************************
* �� �� ��  : nandReadBl
*
* ��������  : ����ʵ�ʳ��ȶ�ȡBootLoader
*
* �������  : dest BootLoader��ȡ��Ŀ�ĵ�
* �������  :
*
* �� �� ֵ  : OK ��ȡ�ɹ�
*                NAND_ECC_ERR ECC���ֲ��ɾ����Ĵ���
*                SEC_IMAGE_LEN_ERROR ���ȴ���
*
* ����˵��  :
*
*****************************************************************************/
int nandReadBl( UINT32 dest )
{
    UINT32 blLen;
    UINT32 ulEccType;

    /* ����IO����,NANDȡĬ�����á�*/
    NF_IOS_SYS_CONFIG();

    /* ��������Ϊ16 */
    OUTREG32(NANDC_PWIDTH, 0x555);

    delay(10);

    /* ����Ƿ�ΪBootģʽ,�������,��ֱ������,�ٴγ��Զ�ȡ */
    if(NANDC_OPMODE_BOOT != (INREG32(NANDC_CON) & NANDC_OPMODE_MASK))
    {
        print_info("\r\nnot in boot mode,reboot to try...");
        setErrno(NAND_NO_IN_BOOTMODE);

        wdtRebootDelayMs(TIME_DELAY_MS_6000_FOR_NF_OPBOOT);
    }

    /* ��ȡBootLoader���� */
    blLen = *(volatile UINT32 *)(FLASH_BOOT_ADDR+BL_LEN_INDEX);

    /* ��ȡECC Type */
    ulEccType = INREG32(NANDC_CON) & NAND_ECC_TYPE_MASK;

    /* ʹ��ECC�����,����ECC���ɾ����Ĵ��� */
    if((NAND_ECC_TYPE_0 != ulEccType)
        && (INREG32(NANDC_INTS) & ECC_ERR_INVALID))
    {
        print_info("\r\necc err!");
        return NAND_ECC_ERR;
    }

    /* �жϳ����Ƿ�Ϸ�:��Ϊ��/�ֶ���/����ת/������ */
    /* 0x1234ABCD - read retry failed */
    /* 0xABCD1234 - all block(0 to 7) is bad */
    if((0 == blLen)
        || (blLen % 4)
        || (blLen + IDIO_LEN + OEM_CA_LEN + IDIO_LEN < blLen)
        || (blLen + IDIO_LEN + OEM_CA_LEN + IDIO_LEN > BOOTLOAD_SIZE_MAX))
    {
        print_info_with_u32("\r\nBL len err:", blLen);

        return SEC_IMAGE_LEN_ERROR;
    }

    /* ���Ͼ���ǩ����OEM CA��OEM CAǩ���ĳ��� (��ȫУ��ʱ�����)*/
    blLen += IDIO_LEN + OEM_CA_LEN + IDIO_LEN;

    delay(10);
    /* Bootģʽ��ֱ�Ӷ�ȡ����BootLoader */
    memcpy((void*)dest, (void*)FLASH_BOOT_ADDR, blLen);

    return OK;
}


/*****************************************************************************
* �� �� ��  : idioIdentify
*
* ��������  : ͨ��У���������ݵ�hashֵ�� ��
*             ����ǩ����RSA���ܺ��ֵһ��
*             ��У���������ݵĺϷ��� *
*
* �������  : dataAddr--�������ݵĵ�ַ
*             dataLen--�������ݵĳ���
*             pubKey--��Կ
*             pIdio--��������ǩ����ָ��
* �������  :
*
* �� �� ֵ  : OK/ !OK
*
* ����˵��  :
*
*****************************************************************************/
int idioIdentify(UINT32 dataAddr, UINT32 dataLen, KEY_STRUCT *pubKey, UINT32* pIdio)
{
    UINT32 sha256Hash[SHA256_HASH_SIZE];      /*�������SHA256ֵ����ʱbuffer*/
    UINT32 rsa[IDIO_LEN/4];                   /*�������RSA����ֵ����ʱbuffer*/
    int i;

    memset(sha256Hash, 0, SHA256_HASH_SIZE*4);
    memset(rsa, 0, IDIO_LEN);

    /*ͨ��SHA256������������(data)��hashֵ*/
    if(OK != SHA256Hash(dataAddr, dataLen, sha256Hash))
    {
        print_info("\r\nhash calc err!" );
        return !OK;
    }

    /*ʹ�ù�Կ(pubKey),ͨ��RSA���ܳ�����ǩ��(dataIdioAddr)������*/
    if(OK != RSA(pubKey, pIdio, rsa))
    {
        print_info("\r\nrsa calc err!" );
        return !OK;
    }

    /*�Ƚ϶����Ƿ���ͬ*/
    for(i = 0; i < SHA256_HASH_SIZE; i++)
    {
        if( sha256Hash[i] != rsa[i] )
        {
            return !OK;
        }
    }

    return OK;
}
/*****************************************************************************
* �� �� ��  : timerInit
*
* ��������  : Timer Init
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
void timerInit()
{
    volatile UINT32 value;

    /* disable interrupt */
    value = INREG32(TIMER0_REGOFF_CTRL);
    value &= (~TIMER_CTRL_EN);
    value |= TIMER_CTRL_INTMASK;
    OUTREG32(TIMER0_REGOFF_CTRL, value);

    value = INREG32(TIMER6_REGOFF_CTRL);
    value &= (~TIMER_CTRL_EN);
    value |= TIMER_CTRL_INTMASK;
    OUTREG32(TIMER6_REGOFF_CTRL, value);

    /* select clock frequency to 19.2M  */
    value = INREG32(CRG_CLK_SEL2);
    value |= (1 << CRG_CLK_SEL2_TIMER0);
    value |= (1 << CRG_CLK_SEL2_TIMER6);
    OUTREG32(CRG_CLK_SEL2, value);


#ifdef START_TIME_TEST
    /* disable interrupt */
    value = INREG32(TIMER5_REGOFF_CTRL);
    value |= TIMER_CTRL_INTMASK;
    OUTREG32(TIMER5_REGOFF_CTRL, value);

    /* set load count */
    OUTREG32(TIMER5_REGOFF_LOAD, TIMER5_INIT_VALUE);

    value = INREG32(CRG_CLK_SEL2);
    value |= (1 << CRG_CLK_SEL2_TIMER5);
    OUTREG32(CRG_CLK_SEL2, value);

    do
    {
        value = INREG32(TIMER5_REGOFF_CTRL);
        value &= TIMER_CTRL_EN_ACK;
    }while(value != TIMER_CTRL_EN_ACK);

    /* enable interrupt */
    value = INREG32(TIMER5_REGOFF_CTRL);
    value |= TIMER_CTRL_EN;
    OUTREG32(TIMER5_REGOFF_CTRL, value);
#endif
}


/*****************************************************************************
* �� �� ��  : secCheck
*
* ��������  : ��ȫУ�������
*
* �������  : ulImageAddr--ӳ���ַ,�����ֶ���
*                  eImageTye--��������
* �������  :
*
* �� �� ֵ  : SEC_IMAGE_LEN_ERROR   ���񳤶ȹ���
*             SEC_EFUSE_READ_ERROR   Efuse��ȡ����
*             SEC_EFUSE_NOT_WRITE    Efuseδ��д
*             SEC_SHA_CALC_ERROR     SHA�������
*             SEC_ROOT_CA_ERROR      ��CAУ�����
*             SEC_OEMCA_ERROR        OEM CAУ�����
*             SEC_IMAGE_ERROR        ����ǩ��У�����
*
* ����˵��  :
*
*****************************************************************************/
int secCheck(UINT32 ulImageAddr, IMAGE_TYPE_E eImageTye)
{
    int i;
    UINT32 blLen = 0;
    KEY_STRUCT *pstKeyRoot;
    KEY_STRUCT *pstKeyBl;                       /*ӳ��Ĺ�Կ*/
    UINT32 sha256HashCa[SHA256_HASH_SIZE];      /*�������SHA256ֵ����ʱbuffer*/
    UINT32 md5HashEfuse[MD5_HASH_SIZE];      /*�������SHA256ֵ��MD5ֵ����ʱbuffer*/
    UINT32 md5HashCa[MD5_HASH_SIZE];            /*�������SHA256ֵ��MD5ֵ����ʱbuffer*/

    UINT32 imageIdioAddr;                   /*�����ǩ���ĵ�ַ*/
    UINT32 imageCaAddr;                     /*ӳ���CA�ĵ�ַ*/
    UINT32 imageCaIdioAddr;                 /*ӳ���CAǩ���ĵ�ַ*/

    MD5_CTX md5_contex;

    volatile tOcrShareData *pShareData = (tOcrShareData*)M3_TCM_SHARE_DATA_ADDR;

     /*��ȡbootload.bin�ļ��ĳ���(�ֽ���)*/
     if(IMAGE_TYPE_DOWNLOAD != eImageTye)
     {
        blLen = *(volatile UINT32 *)(ulImageAddr+BL_LEN_INDEX);
     }
     else
     {
        blLen = *(volatile UINT32 *)(ulImageAddr+DOWNLOAD_BL_LEN_INDEX);
     }

    /* �жϳ����Ƿ�Ϸ�:��Ϊ��/�ֶ���/����ת/������ */
    if((0 == blLen)
        || (blLen % 4)
        || (blLen + IDIO_LEN + OEM_CA_LEN + IDIO_LEN < blLen)
        || ((IMAGE_TYPE_DOWNLOAD != eImageTye) /* ��ΪUSB�Ծٰ汾������У��汾���� */
            && (blLen + IDIO_LEN + OEM_CA_LEN + IDIO_LEN > BOOTLOAD_SIZE_MAX)))
    {
    	print_info_with_u32("\r\nSec image len err: 0x", blLen);
        return SEC_IMAGE_LEN_ERROR;
    }

    /* ��ȡ����ǩ����λ�� */
    imageIdioAddr = ulImageAddr + blLen;

    /* ��ȡOEM CA��λ�� */
    imageCaAddr = imageIdioAddr + IDIO_LEN;

    /* ��ȡOEM CAǩ����λ�� */
    imageCaIdioAddr = imageCaAddr + OEM_CA_LEN;

    /* ��CA�Ѿ���RAM��ʼ��������У��ͨ�������س�������У���CA */
    if(IMAGE_TYPE_DOWNLOAD != eImageTye)
    {
        /* ��ȡEfuse�еĸ�CA��Hashֵ */
        if(OK != efuseRead(EFUSE_GRP_ROOT_CA, md5HashEfuse, MD5_HASH_SIZE))
        {
            print_info("\r\nEfuse read err!");
            return SEC_EFUSE_READ_ERROR;
        }

        /* ���Efuse�Ƿ�����д */
        if(EFUSE_NOT_WRITED == efuseWriteCheck(md5HashEfuse, MD5_HASH_SIZE))
        {
            pShareData->bRootCaInited = FALSE;
            pShareData->bSecEn = FALSE;         /* Efuseδ��д,�޷���ø�CA��ʹ�ܰ�ȫУ�� */
            print_info("\r\nEfuse not write, sec will be disabled!" );
            return SEC_EFUSE_NOT_WRITE;
        }

        pstKeyRoot = (KEY_STRUCT *)(ulImageAddr + ROOT_CA_INDEX);
        /* �������Կ��Hashֵ */
        if(OK != SHA256Hash((UINT32)pstKeyRoot, ROOT_CA_LEN, sha256HashCa))
        {
            print_info("\r\nsha256 err!");
            return SEC_SHA_CALC_ERROR;
        }

        MD5Init(&md5_contex);
        MD5Update(&md5_contex, (unsigned char *)sha256HashCa, sizeof(sha256HashCa));
        MD5Final(&md5_contex, (unsigned char*)md5HashCa);

        /* �Ƚϸ���Կ��Hashֵ */
        for(i=0; i<MD5_HASH_SIZE; i++)
        {
            if(md5HashEfuse[i] != md5HashCa[i])
            {
                print_info("\r\nRoot CA check err!");
                return SEC_ROOT_CA_ERROR;
            }
        }

        /* �������Կ�������BootLoader */
        pShareData->pRootKey = pstKeyRoot;
        pShareData->bRootCaInited = TRUE;

    }
    else
    {
        /* ��ֹδ����RAM��ʼ�������ֱ������USB���س��� */
        if(!pShareData->bRootCaInited)
        {
            print_info("root ca not inited!");
            return SEC_ROOT_CA_ERROR;
        }
    }

    /*��Ȩӳ��֤��*/
    if(OK != idioIdentify(imageCaAddr, OEM_CA_LEN, pShareData->pRootKey, (UINT32*)imageCaIdioAddr))
    {
        print_info("\r\nOEM CA check err!" );
        return SEC_OEMCA_ERROR;
    }

    /*��Ȩbootloadӳ��*/
    pstKeyBl = (KEY_STRUCT *)imageCaAddr;

    if(OK != idioIdentify(ulImageAddr, blLen, pstKeyBl, (UINT32*)imageIdioAddr))
    {
        print_info("\r\nimage check err!" );
        return SEC_IMAGE_ERROR;
    }

    return SEC_SUCCESS;
}

#if PLATFORM==PLATFORM_V7R2_PORTING

void testHash256()
{
    INT32 i;
    UINT32 hash[SHA256_HASH_SIZE] = { 0 };
    UINT32 data[128] = { 0 };

    for (i = 0; i < sizeof(data)/sizeof(data[0]); i++)
    {
        data[i] = i;
    }

    if(OK != SHA256Hash((UINT32)data, sizeof(data), hash))
    {
        print_info("\r\nsha256 err!");
    }
    else
    {
        for (i = 0; i < SHA256_HASH_SIZE; i++)
        {
            print_info_with_u32("\r\n 1: ", hash[i]);
        }
    }

    if(OK != SHA256Hash((UINT32)data, sizeof(data), hash))
    {
        print_info("\r\nsha256 err!");
    }
    else
    {
        for (i = 0; i < SHA256_HASH_SIZE; i++)
        {
            print_info_with_u32("\r\n 2: ", hash[i]);
        }
    }
}

void testEfuseRead()
{
    INT32 i, ret;
    UINT32 value = 0;

    for (i = 0; i < 8; i++)
    {
        ret =efuseRead(i, &value, 1);
        if (ret)
        {
            print_info_with_u32("\r\nefuse read error, ret = ", ret);
        }
        else
        {
            print_info_with_u32("\r\nvalue = ", value);
        }
    }
}

void testEfuseWrite()
{
    UINT32 value = EFUSEC_SEC_EN;
/*
������ROOT CA�Ĺ�ϣֵ
    UINT32 efuseValue[] =
    {
        0x73A0460D, 0xF26B85FB, 0xC0BA0AD1, 0x4A059D28,
        0x203092C0, 0xAB9095AB, 0x573F5A43, 0xE5F34A6C
    };
*/
    UINT32 efuseValue[] =
    {
        0x45003997, 0x68229d1a, 0xec67902c, 0xf4240b7a
    };

    if (efuseWrite(0, efuseValue, sizeof(efuseValue)/sizeof(efuseValue[0])))
    {
        print_info("\r\nfail to write efuse");
    }
    else
    {
        print_info("\r\nsucceed to write efuse");
    }

    /* ���ð�ȫУ�� */
    efuseWrite(EFUSE_GRP_CFG, &value, 1);
}

#endif

#ifndef ONCHIPROM_TEST
/*****************************************************************************
* �� �� ��  : secBoot
*
* ��������  :   ��ȫ����C��ں���
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
void secBoot(void)
{
    volatile UINT32 ulBootModeVal = 0x00;

    volatile tOcrShareData *pShareData = (tOcrShareData*)M3_TCM_SHARE_DATA_ADDR;
    int retVal = 0x00;
    int BOOT_RST_Addr = 0x00;

#if PLATFORM==PLATFORM_V7R2_PORTING

    //testEfuseRead();
    //testHash256();

#endif

    /* ��ʼ�� */
    timerInit();

    pShareData->errno = 0;
    pShareData->SHA256Hash    = NULL;
    pShareData->RSA         = NULL;
    pShareData->idioIdentify = NULL;
    pShareData->bSecEn = FALSE;
    pShareData->bRootCaInited = FALSE;
    pShareData->pRootKey    = NULL;
    pShareData->ulOcrInitedFlag = FALSE;

    /* ��ȡEfuse��ȫУ�����ã���ȡ��ȫ��־ */
    if(EFUSEC_SEC_EN == (INREG32(EFUSEC_HW_CFG) & EFUSEC_SEC_EN_MASK))
    {
        pShareData->bSecEn = TRUE;
    }

    /*�ж�USB�Ծ������־�Ƿ���Ч*/
    if((AUTO_ENUM_FLAG_VALUE == pShareData->ulEnumFlag)
        || (SC_AUTO_ENUM_EN == (INREG32(SC_STAT0) & SC_AUTO_ENUM_EN_BITMASK)))
    {
        /* ���־��Ч���Ծٹܽ�Ϊ�͵�ƽ���������־,�ٽ����Ծ�����*/
        /*pShareData->ulEnumFlag = 0;*/ /* �������־,��̽��������Ծ�ԭ�� */
        usbBoot();
    }

    /*��ȡBOOTMODE*/
    ulBootModeVal = INREG32(SC_STAT0) & SC_BOOTMODE_BITMASK;
    switch(ulBootModeVal)
    {
        case BOOT_MODE_NAND:
        {
            print_info("\r\nNF boot!" );
            /* NandĬ����Bootģʽ��,ֱ�ӿ���bootloadӳ�� */
            retVal = nandReadBl(M3_TCM_BL_ADDR);
            break;
        }

       case BOOT_MODE_AP_HSIC:
       {
            print_info("\r\nHSIC boot!" );

            /*ͨ��HSIC��bootloadӳ���ȡ*/
            hsicBoot();

            break;
        }

       case BOOT_MODE_SPI:
       {
            print_info("\r\nSPI boot!" );

            /*��SPI�н�bootloadӳ���ȡ*/
            retVal = spiDevReadBl((UINT8*)M3_TCM_BL_ADDR);

            break;
        }

        case BOOT_MODE_EMMC:
            print_info("\r\nEMMC boot!" );
            /*����bootloadӳ��*/
            retVal = emmc_read_bootloader(M3_TCM_BL_ADDR);

            break;

        case BOOT_MODE_AP_HSUART:
        {
            print_info("\r\nHSUART boot!" );

            /* ���ٷ��� */
            retVal = apDlMain(M3_TCM_BL_ADDR);

            /* ��ֹapDlMain����,�����ӡ���� */
            print_info_with_u32("\r\napDlMain exec err, ret:" , (UINT32)retVal);
            setErrno(SYS_ERR_AP_DL_RETURNS);
            /* ��ʱ��λ */
            wdtRebootDelayMs(TIME_DELAY_MS_4000_FOR_AP_RETURNS);

            break;  /*lint !e527*/
        }

        default:
        {
#if PLATFORM==PLATFORM_V7R2_PORTING

            /* PORTING�汾ʹ�ô�ģʽ��дEFUSE */
            print_info_with_u32("\r\nbootmode err, will write efuse on porting..." , ulBootModeVal);
            testEfuseWrite();
            usbBoot();
            break;

#else
            /* ��λ,�Է�ֹStick��̬û�д���,�Ӹ�λ���Ե�֪���� */
            print_info_with_u32("\r\nbootmode err:" , (ulBootModeVal>>SC_BOOTMODE_BITPOS));
            setErrno(SYS_ERR_BOOT_MODE);
            /* ��ʱ��λ */
            wdtRebootDelayMs(TIME_DELAY_MS_3000_FOR_BOOT_MODE);
            break;
#endif
        }
    }

    if(OK != retVal)
    {
        print_info_with_u32("\r\nBL read err, ret:", (UINT32)retVal);
        setErrno(retVal);

        usbBoot();
    }

    /*�ж�FLASH /E2/MMC�Ƿ�����ӳ��*/
    if(BL_CHECK_INSTRUCTION != *(volatile UINT32 *)BL_CHECK_ADDR)
    {
        /*ӳ��δ���룬��AP����Nack, ������USB�Ծ�����*/
        print_info("\r\nimage not program!" );
        setErrno(SEC_NO_IMAGE);
        usbBoot();
    }

    ocrShareSave();

    /*�ǰ�ȫ������ֱ������*/
    if(!pShareData->bSecEn)
    {
        print_info("\r\nUnSec_boot!" );
        /*�ǰ�ȫ������TCMִ��*/
        BOOT_RST_Addr = *(volatile UINT32 *)BOOT_RST_ADDR_PP;
        go((FUNCPTR)(M3_TCM_BL_ADDR + BOOT_RST_Addr));
    }

    /*��ȫ���������а�ȫУ��*/
    retVal = secCheck((UINT32)M3_TCM_BL_ADDR, IMAGE_TYPE_BOOTLOAD);

#ifdef START_TIME_TEST
    print_info_with_u32("\r\ntime(ms):", (TIMER5_INIT_VALUE - INREG32(TIMER5_REGOFF_VALUE))/MS_TICKS);
#endif

    switch(retVal)
    {
        case SEC_SUCCESS:/*��ȫУ��ͨ��*/
            print_info("\r\nSec check ok!" );  /*lint !e616*/
            /* ������һcase����BootLoader */
        case SEC_EFUSE_NOT_WRITE:     /*lint !e825*/ /*EFUSE δ��д*/
            /* ����TCM memִ��BOOTLOAD */
            BOOT_RST_Addr = *(volatile UINT32 *)BOOT_RST_ADDR_PP;
            go((FUNCPTR)(M3_TCM_BL_ADDR + BOOT_RST_Addr));

            break;

        case SEC_SHA_CALC_ERROR:  /* Hash���㲻ͨ�� */
        case SEC_OEMCA_ERROR:  /* OEM CAУ�鲻ͨ�� */
        case SEC_IMAGE_ERROR:   /* ӳ��У�鲻ͨ�� */
        case SEC_ROOT_CA_ERROR:/* ��CAУ����� */
        case SEC_IMAGE_LEN_ERROR:/*��ȫ�汾���ȴ���*/
            print_info("\r\nSec check err!" );
            setErrno(retVal);
            usbBoot();

            break;

        case SEC_EFUSE_READ_ERROR:/*Efuse��ȡʧ��,ʹ�ÿ��Ź���λ,�ٴγ��Զ�ȡ*/
            print_info("\r\nEfuse read err, reboot...");
            setErrno(SYS_ERR_EFUSE_READ);
            wdtRebootDelayMs(TIME_DELAY_MS_2000_FOR_EFUSE_READERR);
            break;

        default:
            print_info_with_u32("\r\nunhandered ret:",(UINT32)retVal);
            setErrno(SYS_ERR_SEC_UNKNOWN_RET);
            /* ��ʱ1s */
            wdtRebootDelayMs(TIME_DELAY_MS_5000_FOR_SEC_UNKNOWN_RET);
            /*break;     */  /*for pc lint*/

    }

    /* �쳣�������ô����� */
    setErrno(retVal);

}
#else
void secBoot(void)
{
    print_info("\r\nSEC test...");
}
#endif

