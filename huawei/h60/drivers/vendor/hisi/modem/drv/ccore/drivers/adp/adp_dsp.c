

/*lint --e{537} */
#include <stdio.h>
#include <string.h>
#include <MemoryMap.h>
#include <product_config.h>
#include <drv_nv_id.h>
#include <drv_nv_def.h>
#include <bsp_nvim.h>
#include <bsp_dsp.h>

/*****************************************************************************
 �ṹ��    : PS_PHY_DYN_ADDR_SHARE_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : ������PS�Ĺ����ַ��OAM��Ҫ��ȡ������TTF���߳�ʼ������PHY��ȡ
*****************************************************************************/
typedef struct
{
    /*������ʼ��,PSֻ��*/
    unsigned int uwProtectWord1;                      /*0x5a5a5a5a
 */
    unsigned int uwPhyGttfShareAddr;                  /* GDSP��GTTF�����ڴ�ӿڵ�ַ,������ʼ��
 */

    /*PS�����ʼ��,PHYֻ��*/
    unsigned int uwProtectWord2;                      /*0x5a5a5a5a��PS��д
 */
    unsigned int uwHsupaUpaccMacePduBaseAddr;         /*HSUPA_UPACC_MACE_PDU_BASE_ADDR
 */
    unsigned int uwEcsArmDspBufferSize;               /*ECS_ARM_DSP_BUFFER_SIZE
 */
    unsigned int uwSocTimeSliceAddr;                  /*ʱ���ȡ��ַ
 */

    /*AXI addr*/
    unsigned int uwStatusFlagAddr;                    /*HIFI/DSP STATUS_FLAG_ADDR
 */
    unsigned int uwTempratureAddr;                    /*�±�
 */

    unsigned long uwHsdpaWttfBufAddr;                  /* HSDPA WTTF����buf��ַ */
    /*��������չ�á��Ժ���԰�ƽ̨��صĵ�ַ�ŵ����ﴫ�ݽ���*/
    unsigned long uwReserved[15];
}PS_PHY_DYN_ADDR_SHARE_STRU;

/*****************************************************************************
 �ṹ��    : PHY_HIFI_DYN_ADDR_SHARE_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : ������HIFI�Ĺ����ַ��OAM��Ҫ������HIFI��BOOT�����������е���Ӧ�ṹ��
*****************************************************************************/
typedef struct
{
    unsigned int uwProtectWord;           /*0x5a5a5a5a
 */
    unsigned int uwHifiPhyMboxHeadAddr;   /*HIFI��PHY�Ĺ��������壬PHY�����ʼ�����뾲̬��ʼ����
 */
    unsigned int uwHifiPhyMboxBodyAddr;
    unsigned int uwPhyHifiMboxHeadAddr;   /*PHY��HIFI�Ĺ��������壬PHY�����ʼ�����뾲̬��ʼ����
 */
    unsigned int uwPhyHifiMboxBodyAddr;
    unsigned int uwReserved[3];
}PHY_HIFI_DYN_ADDR_SHARE_STRU;


/*****************************************************************************
 �ṹ��    : PS_PHY_DYN_ADDR_SHARE_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : DSP�ϵ�ʱOAM��Ҫ��DSP��".dsp_share_addr"��д���½ṹ�����ݣ���ʼ��ַ��DRV����ZSP bin���󴫸�OAM
*****************************************************************************/
typedef struct
{
    PS_PHY_DYN_ADDR_SHARE_STRU      stPsPhyDynAddr;     /*PS��PHY�Ĺ��������� */
    PHY_HIFI_DYN_ADDR_SHARE_STRU    stPhyHifiDynAddr;   /*PHY��HIFI�Ĺ��������� */
}DSP_SHARE_ADDR_STRU;

/************************************************************************
 * FUNCTION
 *       BSP_GUDSP_UpateShareInfo
 * DESCRIPTION
 *       ���DSP����Ķ�̬��Ϣ��ȡ�͸��¹���
 * INPUTS
 *       NONE
 * OUTPUTS
 *       BSP_OK/BSP_ERR
 *************************************************************************/
extern int BSP_GUDSP_UpateShareInfo(unsigned long shareAddr);
int BSP_GUDSP_UpateShareInfo(unsigned long shareAddr)
{
    DSP_SHARE_ADDR_STRU      *pstDspShareInfo = (DSP_SHARE_ADDR_STRU*)shareAddr;

    if(0x5A5A5A5A != pstDspShareInfo->stPsPhyDynAddr.uwProtectWord1)
    {
        printf("\r\n BSP_GUDSP_UpateShareInfo: ZSP Share Info uwProtectWord1 Error.");
        return -1;     /*�쳣�˳��������*/
    }

    /*��дL2ʹ�õĵ�ַ�ռ��С*/
    pstDspShareInfo->stPsPhyDynAddr.uwHsupaUpaccMacePduBaseAddr   = ECS_UPA_BASE_ADDR;
    pstDspShareInfo->stPsPhyDynAddr.uwEcsArmDspBufferSize         = ECS_UPA_SIZE;

    pstDspShareInfo->stPsPhyDynAddr.uwHsdpaWttfBufAddr            = ECS_ARM_DSP_BUFFER_BASE_ADDR;

    /*��ǰ��Ϣд����ɱ�־*/
    pstDspShareInfo->stPsPhyDynAddr.uwProtectWord2    = 0x5A5A5A5A;

    return 0;
}


/*****************************************************************************
 �� �� ��  : bsp_dsp_is_hifi_exist
 ��������  : ��ȡCODEC����״̬
 �������  : ��
 �������  : ��
 �� �� ֵ  : 1:  CODEC����
             0��CODEC ������
*****************************************************************************/
int bsp_dsp_is_hifi_exist(void)
{
    int ret = 0;
    DRV_MODULE_SUPPORT_STRU   stSupportNv = {0};

    ret = (int)bsp_nvm_read(NV_ID_DRV_MODULE_SUPPORT, (u8*)&stSupportNv, sizeof(DRV_MODULE_SUPPORT_STRU));
    if (ret)
        ret = 0;
    else
        ret = (int)stSupportNv.hifi;

    return ret;
}

#ifndef CONFIG_DSP

int bsp_bbe_load_muti(void)
{
    return 0;
}

int bsp_bbe_run(void)
{
    return 0;
}

int bsp_bbe_stop(void)
{
    return 0;
}

int bsp_bbe_power_on(void)
{
    return 0;
}

int bsp_bbe_power_off(void)
{
    return 0;
}

int bsp_bbe_unreset(void)
{
    return 0;
}

int bsp_bbe_reset(void)
{
    return 0;
}

int bsp_bbe_clock_enable(void)
{
    return 0;
}

int bsp_bbe_clock_disable(void)
{
    return 0;
}

int bsp_bbe_is_power_on(void)
{
    return 0;
}

int bsp_bbe_is_clock_enable(void)
{
    return 0;
}

void bsp_dsp_init(void)
{
    return;
}

int DRV_GET_CODEC_FUNC_STATUS(void)
{
#if (FEATURE_HW_CODEC== FEATURE_ON)
    return 1;
#else
	return 0;
#endif
}

#endif

