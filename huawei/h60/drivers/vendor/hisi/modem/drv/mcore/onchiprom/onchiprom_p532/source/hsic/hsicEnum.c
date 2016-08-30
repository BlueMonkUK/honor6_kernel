
#include "OnChipRom.h"
#include "hsicDrv.h"
#include "enumProt.h"
#include "OcrShare.h"

inline uint16_t BSP_UBOOT_CalcCRC(uint8_t byte, uint16_t crc)
{
    uint8_t  da = 0;
    uint32_t ta[16] = { 0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
                        0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF };

    da = ((uint8_t)(crc >> 8)) >> 4;
    crc <<= 4;
    crc ^= (uint16_t)ta[da ^ (byte >> 4)];
    da = ((uint8_t)(crc >> 8)) >> 4;
    crc <<= 4;
    crc ^= (uint16_t)ta[da ^ (byte & 0x0F)];

    return crc;
}

static uint32_t BSP_UBOOT_ImageIdentify(pUBOOT_XFR_CTRL_INFO puBootCtrl)
{
    int32_t ret_val;
    IMAGE_TYPE_E eImageType;
    volatile tOcrShareData *pShareData = (tOcrShareData *)M3_SRAM_SHARE_DATA_ADDR;

    /* �ǰ�ȫUSB����������У�� */
    if (pShareData->bSecEn == 0) {
        return ENUM_XFR_PROT_OK;
    }

    /* �ж�USB�Ծ����صĳ����ļ����� */
    if (puBootCtrl->u32uBootFileType == ENUM_XFR_FTYPE_RAMINIT) {
        eImageType = IMAGE_TYPE_RAM_INIT;
    } else {
        eImageType = IMAGE_TYPE_DOWNLOAD;
    }

    /* �����صĳ����ļ�ʵʩ��ȫУ�� */
    ret_val = secCheck((uint32_t)(*(puBootCtrl->puBootLoadFunc)), eImageType);

    /* ���س����ļ��İ�ȫУ���� */
    return ((ret_val == SEC_SUCCESS) || (ret_val == SEC_EFUSE_NOT_WRITE)) ?
            ENUM_XFR_PROT_OK : ENUM_XFR_PROT_ERR;
}

static void BSP_UBOOT_TransInfoReset(pUBOOT_XFR_CTRL_INFO puBootCtrl)
{
    puBootCtrl->u32uBootFileType = 0;
    puBootCtrl->u32uBootFileAddress = 0;
    puBootCtrl->u32uBootFileCapacity = 0;
    puBootCtrl->u32USBCoreRecvLen = 0;
    puBootCtrl->u32uBootTotalFrame = 0;
    puBootCtrl->u32uBootCurrFrame = 0;
    puBootCtrl->u8uBootNextFrame = 0;
    puBootCtrl->puBootLoadFunc = (void *)0;
}

static uint32_t BSP_UBOOT_FileFrameHandle(pUBOOT_XFR_CTRL_INFO puBootCtrl)
{
    pUBOOT_USB_XFR_INFO pXfrInfo = &puBootCtrl->stuBootXfrInfo[1];
    /* FILE֡��ű���Ϊ0�ҳ��ȱ���Ϊ14 */
    if ((puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_SEQ] != 0) ||
        (pXfrInfo->u32USBCoreXfrLen != ENUM_FRAME_LEN_FILE))
    {
        return ENUM_XFR_PROT_ERR;
    }
    /* �����ļ����ͱ���ΪRAM��USB */
    if ((puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_FILE_TYPE] != ENUM_XFR_FTYPE_RAMINIT) &&
        (puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_FILE_TYPE] != ENUM_XFR_FTYPE_USB))
    {
        return ENUM_XFR_PROT_ERR;
    }
    /* ��¼�����ļ����� */
    puBootCtrl->u32uBootFileType = puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_FILE_TYPE];
    /* ��¼�ļ��������� */
    puBootCtrl->u32uBootFileCapacity = (uint32_t)(puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_FILE_LEN+0]<<24) |
                                       (uint32_t)(puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_FILE_LEN+1]<<16) |
                                       (uint32_t)(puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_FILE_LEN+2]<<8) |
                                       (uint32_t)(puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_FILE_LEN+3]);
    /* ��¼�ļ����ص�ַ */
    puBootCtrl->u32uBootFileAddress = (uint32_t)(puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_FILE_ADDR+0]<<24) |
                                      (uint32_t)(puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_FILE_ADDR+1]<<16) |
                                      (uint32_t)(puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_FILE_ADDR+2]<<8) |
                                      (uint32_t)(puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_FILE_ADDR+3]);
    /* ��¼�ļ���Ҫ���ص�֡���� */
    puBootCtrl->u32uBootTotalFrame = puBootCtrl->u32uBootFileCapacity / UBOOT_FRAME_LEN_DATA +
                    (puBootCtrl->u32uBootFileCapacity % UBOOT_FRAME_LEN_DATA ? 2 : 1);
    /* ��¼���س����ļ���ִ�е�ַ */
    puBootCtrl->puBootLoadFunc = (pFuncPtr)puBootCtrl->u32uBootFileAddress;
    /* ׼������DATA֡ */
    puBootCtrl->u32USBCoreRecvLen = 0;
    puBootCtrl->u32uBootCurrFrame = 1;
    puBootCtrl->u8uBootNextFrame = 1;

    return ENUM_XFR_PROT_OK;
}

static uint32_t BSP_UBOOT_DataFrameHandle(pUBOOT_XFR_CTRL_INFO puBootCtrl)
{
    uint32_t data_len, code_len;
    uint8_t *pCodeDst = (void *)0;
    uint8_t *pCodeSrc = (void *)0;
    pUBOOT_USB_XFR_INFO pXfrInfo = &puBootCtrl->stuBootXfrInfo[1];
    /* ֡��ż�� */
    if (puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_SEQ] != puBootCtrl->u8uBootNextFrame)
    {
        /* ������ط�����һDATA֡��������һDATA֡���� */
        if ((puBootCtrl->u32uBootCurrFrame > 1) &&
            (puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_SEQ] == (puBootCtrl->u8uBootNextFrame - 1)))
        {
            puBootCtrl->u32uBootCurrFrame--;
            puBootCtrl->u8uBootNextFrame--;
        }
        else
        {
            return ENUM_XFR_PROT_ERR;
        }
    }
    /* ���㱾��DATA֡���յ����ݳ��� */
    if (puBootCtrl->u32uBootCurrFrame == (puBootCtrl->u32uBootTotalFrame - 1))
    {
        data_len = puBootCtrl->u32uBootFileCapacity - (puBootCtrl->u32uBootCurrFrame - 1) * UBOOT_FRAME_LEN_DATA;
    }
    else
    {
        data_len = UBOOT_FRAME_LEN_DATA;
    }

    if (pXfrInfo->u32USBCoreXfrLen != (data_len + UBOOT_FRAME_DATA_HEAD_LEN))
    {
        return ENUM_XFR_PROT_ERR;
    }
    /* ���㱾��DATA֡��Ŀ���ַ */
    pCodeDst = (uint8_t *)(puBootCtrl->u32uBootFileAddress + (puBootCtrl->u32uBootCurrFrame - 1) * UBOOT_FRAME_LEN_DATA);
    /* ���㱾��DATA֡��Դ��ַ */
    pCodeSrc = (uint8_t *)(puBootCtrl->uBootBlkXfrBuf + ENUM_FRAME_DATA_LOC);
    /* ���㱾��DATA֡��д�볤�� */
    code_len = pXfrInfo->u32USBCoreXfrLen - UBOOT_FRAME_DATA_HEAD_LEN;
    /* ����������д��ָ����ַ */
    while (code_len--)
    {
        *pCodeDst++ = *pCodeSrc++;
    }
    /* ׼��������һ֡���� */
    puBootCtrl->u32USBCoreRecvLen -= UBOOT_FRAME_DATA_HEAD_LEN;
    puBootCtrl->u32uBootCurrFrame++;
    puBootCtrl->u8uBootNextFrame++;

    return ENUM_XFR_PROT_OK;
}

static uint32_t BSP_UBOOT_EotFrameHandle(pUBOOT_XFR_CTRL_INFO puBootCtrl)
{
    /* ֡��ż�� & EOT֡���ȼ�� */
    if ((puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_SEQ] != puBootCtrl->u8uBootNextFrame) ||
        (puBootCtrl->stuBootXfrInfo[1].u32USBCoreXfrLen!= ENUM_FRAME_LEN_EOT))
    {
        return ENUM_XFR_PROT_ERR;
    }
    /* EOT֡����Ƿ�����֡������ͬ */
    if (puBootCtrl->u32uBootCurrFrame == puBootCtrl->u32uBootTotalFrame)
    {
        return ENUM_XFR_PROT_COMPLETE;
    }
    else
    {
        return ENUM_XFR_PROT_ERR;
    }
}

static uint32_t BSP_UBOOT_USBTrans2Prot(pUBOOT_XFR_CTRL_INFO puBootCtrl)
{
    uint32_t cnt;
    uint16_t crcgo = 0, crcval = 0;
    pUBOOT_USB_XFR_INFO pXfrInfo = &puBootCtrl->stuBootXfrInfo[1];
    /* У�����к��Ƿ����Э��Ҫ�� */
    if (((uint8_t)(puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_SEQ]) +
        (uint8_t)(puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_CSEQ])) != 0xFF)
    {
        return ENUM_XFR_PROT_ERR;
    }
    /* CRCУ������Լ�� */
    crcgo = (uint16_t)((puBootCtrl->uBootBlkXfrBuf[pXfrInfo->u32USBCoreXfrLen - 2]) << 8) |
            (uint16_t)((puBootCtrl->uBootBlkXfrBuf[pXfrInfo->u32USBCoreXfrLen - 1]) << 0);
    for (cnt = 0; cnt < (pXfrInfo->u32USBCoreXfrLen - 2); cnt++)
    {
        crcval = BSP_UBOOT_CalcCRC(puBootCtrl->uBootBlkXfrBuf[cnt], crcval);
    }
    if (crcval != crcgo)
    {
        print_info("CRC Error!\r\n");
        return ENUM_XFR_PROT_ERR;
    }
    /* �ļ�����ĵ�һ��֡������FILE֡�������� */
    if ((puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_HEAD] != ENUM_FRAME_TYPE_FILE) &&
        (puBootCtrl->u32uBootCurrFrame == 0))
    {
        return ENUM_XFR_PROT_SKIP;
    }
    /* ֡����ɢ������ */
    switch (puBootCtrl->uBootBlkXfrBuf[ENUM_FRAME_HEAD])
    {
        /* FILE֡�������� */
        case ENUM_FRAME_TYPE_FILE:
            return BSP_UBOOT_FileFrameHandle(puBootCtrl);
        /* DATA֡�������� */
        case ENUM_FRAME_TYPE_DATA:
            return BSP_UBOOT_DataFrameHandle(puBootCtrl);
        /* EOT֡�������� */
        case ENUM_FRAME_TYPE_EOT:
            return BSP_UBOOT_EotFrameHandle(puBootCtrl);
        default:
            return ENUM_XFR_PROT_SKIP;
    }
}

static void BSP_UBOOT_Response2App(pUBOOT_XFR_CTRL_INFO puBootCtrl, uint8_t res)
{
    puBootCtrl->uBootBlkXfrBuf[0] = res;
    puBootCtrl->stuBootXfrInfo[1].u32USBCoreXfrLen = 1;
    puBootCtrl->stuBootXfrInfo[1].pUSBCoreXfrBuf = puBootCtrl->uBootBlkXfrBuf;
    BSP_UBOOT_USBCoreInXfr(puBootCtrl, 1);
}

void hsic_works(void)
{
    UBOOT_XFR_CTRL_INFO  stUBootCtrl;
    pUBOOT_XFR_CTRL_INFO pstUBootCtrl = &stUBootCtrl;
    uint32_t instr_offset;
    uint32_t ret_val;

    hsic_memset((void *)pstUBootCtrl, 0, sizeof(UBOOT_XFR_CTRL_INFO));

    /* Initialize the circumstances */
    pstUBootCtrl->stuBootXfrInfo[0].u32USBMaxPktSize = SYN_USB_MPS_EP0;
    pstUBootCtrl->stuBootXfrInfo[1].u32USBMaxPktSize = SYN_USB_MPS_EP1;

    /* ��ʼ��USB������ */
    if (hsic_driver_init() != UBOOT_TRUE)
    {
        /* ��ΪHOST������,ֱ�ӷ���,�����߻���д���,by wuzechun */
        print_info("HsicInit failed!\r\n");
        return;
    }

    /* ��ѯ����USB�����շ���������ݽ��� */
    FOREVER
    {
        /* ����USB�����շ����� */
        hsic_handle_event(pstUBootCtrl);
        /* USB���ݽ��մ��� */
        ret_val = BSP_UBOOT_USBTrans2Prot(pstUBootCtrl);
        /* ����Э�鴦��������������������� */
        switch (ret_val)
        {
            /* FILE֡��DATA֡���� */
            case ENUM_XFR_PROT_OK:
                BSP_UBOOT_Response2App(pstUBootCtrl, ENUM_RESP_TYPE_ACK);
                break;
            /* FILE֡��DATA֡��EOT֡�쳣 */
            case ENUM_XFR_PROT_ERR:
                BSP_UBOOT_Response2App(pstUBootCtrl, ENUM_RESP_TYPE_NAK);
                break;
            /* EOT֡���� */
            case ENUM_XFR_PROT_COMPLETE:
                /* ���صĳ������ݾ�����ȫУ�� */
                if (BSP_UBOOT_ImageIdentify(pstUBootCtrl) == ENUM_XFR_PROT_OK)
                {
                    BSP_UBOOT_Response2App(pstUBootCtrl, ENUM_RESP_TYPE_ACK);
                    if (pstUBootCtrl->u32uBootFileType == ENUM_XFR_FTYPE_RAMINIT) {
                        //print_info("\r\nRamInit!");
                        /* ִ����HSIC BOOT��ʽ���ص�raminit���� */
                        instr_offset = *(volatile UINT32 *)(pstUBootCtrl->puBootLoadFunc + BOOT_RST_ADDR_OFFEST);
                        (pstUBootCtrl->puBootLoadFunc + instr_offset)();

                    } else {
                        //print_info("\r\nHSICDownload!");
                        ocrShareSave();
                        /*��֪A��������ַ������A�ˣ�ʹA��ִ�����ص�bootrom*/
                        OUTREG32((INTEGRATOR_SC_BASE+0x414), pstUBootCtrl->puBootLoadFunc);
                        SETREG32((INTEGRATOR_SC_BASE+0x64), 0x4000);
                        while(1);
                    }

                    /* ��λUSB boot����Э��֡���ƽṹ�� */
                    BSP_UBOOT_TransInfoReset(pstUBootCtrl);
                }
                /* ���صĳ������ݷǷ� */
                else
                {
                    print_info("\r\nSecCheck Err!");
                    BSP_UBOOT_Response2App(pstUBootCtrl, ENUM_RESP_TYPE_SNAK);
                }
                break;
            /* �����������Ͳ����κ���Ӧ */
            default:
                break;
        }
    }
}

