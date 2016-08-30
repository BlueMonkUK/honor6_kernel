
#include <boot/boot.h>
#include <spi.h>
#include <hi_pmu.h>
#include <pmu.h>
#include "pmic_hi6451.h"

/*****************************************************************************
* �� �� ��  : hi6451_reg_write
*
* ��������  : ��pmuоƬ�Ĵ�����д����
*
* �������  : u8 u8Addr����д��ļĴ�����ַ
*                    u8 u8Data����д�������
*
* �������  : ��
*
* �� �� ֵ  : ERROR��д��ʧ��
*             OK��д��ɹ�
*****************************************************************************/
s32 bsp_hi6451_reg_write(u8 u8Addr, u8 u8Data)
{
    u16 frameSend = (u16)((u8Addr << ADDR_BIT_OFFSET) | u8Data);

    /* ͨ��SPI���豸д������ */
    return spi_send(SPI_NO_PMU, SPI_CS_PMU, &frameSend, 1);
}

/*****************************************************************************
* �� �� ��  : hi6451_reg_read
*
* ��������  : ��pmuоƬ�Ĵ����Ķ�����
*
* �������  : u8Addr�� ����ȡ�ļĴ�����ַ
*               pu8Data����ȡ�����ݴ�ŵĵ�ַ
*
* �������  : u8 *pu8Data����ȡ�����ݴ�ŵĵ�ַ
*
* �� �� ֵ  : ERROR����ȡʧ��
*                    OK����ȡ�ɹ�
*****************************************************************************/
s32 bsp_hi6451_reg_read(u8 u8Addr, u8 *pu8Data)
{
    u16 frameSend = (u16)((u8Addr << ADDR_BIT_OFFSET) | (0x1 << CMD_BIT_OFFSET));
    u16 frameRecv = 0;   /* A11�շ�֡��ʽ��Ϊ16bit������Ч����Ϊ8bit */
    s32 retval = 0;

    /* ͨ��SPI���豸��ȡ���� */
    retval = spi_recv(SPI_NO_PMU, SPI_CS_PMU, &frameRecv, 1, &frameSend, 1);
    *pu8Data = (u8)frameRecv;
    return retval;
}
/*****************************************************************************
 �� �� ��  : hi6451_reg_write_mask
 ��������  : ����hi6451�Ĵ�����ĳЩbit
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  : ����hi6451�Ĵ���ʱʹ��
*****************************************************************************/
int bsp_hi6451_reg_write_mask(u8 addr, u8 value, u8 mask)
{
    u8 reg_tmp = 0;

    bsp_hi6451_reg_read(addr, &reg_tmp);

    reg_tmp &= ~mask;
    reg_tmp |= value;

    return bsp_hi6451_reg_write(addr, reg_tmp);
}
/*****************************************************************************
 �� �� ��  : pmu_hi6451_init
 ��������  : pmu hi6451ģ���ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  : fastboot��pmuģ���ʼ��ʱ����
*****************************************************************************/
void pmu_hi6451_init(void)
{
    s32 iRetVal;
    u8 iregVal;

    (void)spi_init(SPI_NO_PMU);

    /*��ʼ��Ĭ������*/
    /*LDO14 2.6v,ON; LDO13\15,OFF*/
    iRetVal =  bsp_hi6451_reg_write(HI_PMU_SET_LDO14_OFFSET, 0x5);
    iRetVal |= bsp_hi6451_reg_read(HI_PMU_ONOFF4_OFFSET,&iregVal);
    iregVal |= 0x08;
    iRetVal |= bsp_hi6451_reg_write(HI_PMU_ONOFF4_OFFSET, iregVal);
    /*BUCK4 3.3V,ON; BUCK 1,2,3,4 ON*/
    iRetVal |= bsp_hi6451_reg_read(HI_PMU_ONOFF5_OFFSET,&iregVal);
    iregVal |=  0x02;
    iRetVal |= bsp_hi6451_reg_write(HI_PMU_ONOFF5_OFFSET, iregVal);

    if(iRetVal)
        cprintf("pmu_hi6451_init error!\n" );
    else
        cprintf("pmu_hi6451_init ok!\n" );
}
/*****************************************************************************
 �� �� ��  : hi6451_power_key_state_get
 ��������  : ��ȡpower���Ƿ���
 �������  : void
 �������  : ��
 �� �� ֵ  : power���Ƿ��£�1:���£�0:δ����)
 ���ú���  :
 ��������  : ���ػ�ģ��
*****************************************************************************/
bool hi6451_power_key_state_get(void)
{
    u8 regval = 0;

    bsp_hi6451_reg_read(HI_PMU_STATUS4_OFFSET, &regval);
    return  (regval & PMU_HI6451_POWER_KEY_MASK) ? 1 : 0;
}
/*****************************************************************************
 �� �� ��  : bsp_hi6451_usb_state_get
 ��������  : ��ȡusb�Ƿ���״̬
 �������  : void
 �������  : ��
 �� �� ֵ  : usb�����γ�(1:���룻0:�γ�)
 ���ú���  :
 ��������  : ���ػ�ģ��
*****************************************************************************/
bool hi6451_usb_state_get(void)
{

    u8 regval = 0;

    bsp_hi6451_reg_read(HI_PMU_STATUS5_OFFSET, &regval);
    return  (regval & PMU_HI6451_USB_STATE_MASK) ? 1 : 0;

}
/*****************************************************************************
 �� �� ��  : hi6451_hreset_state_get
 ��������  : �ж�pmu�Ƿ�Ϊ������
 �������  : void
 �������  : ��
 �� �� ֵ     : pmu����������������(1:��������0:������)
 ���ú���  :
 ��������  : ���ػ�ģ��
*****************************************************************************/
bool hi6451_hreset_state_get(void)
{
    u8 regval = 0;

    bsp_hi6451_reg_read(HI_PMU_NO_PWR_IRQ1_OFFSET, &regval);
    return  (regval & PMU_HI6451_HRESET_STATE_MASK) ? 1 : 0;
}
/*****************************************************************************
 �� �� ��  : hi6451_version_get
 ��������  : ��ȡpmu�İ汾��
 �������  : void
 �������  : ��
 �� �� ֵ  : pmu�汾��
 ���ú���  :
 ��������  : HSO����,MSP����
*****************************************************************************/
u8  hi6451_version_get(void)
{
    u8 regval = 0;

    bsp_hi6451_reg_read(HI_PMU_VERSION_OFFSET, &regval);
    return  regval;
}
/*****************************************************************************
 �� �� ��  : hi6451_get_boot_state
 ��������  :ϵͳ����ʱ���pmu�Ĵ���״̬��
                ȷ���Ƿ�����pmu���������
 �������  : void
 �������  : reset.log
 �� �� ֵ  : pmu�����ok
 ���ú���  :
 ��������  :ϵͳ��ά�ɲ�
*****************************************************************************/
int hi6451_get_boot_state(void)
{

    u8 a_ucRecordReg[3] = {0};
    u32 i = 0;
    int iret = PMU_STATE_OK;

    /*��ȡ���µ�reg*/
    for(i = 0; i < 3; i++)
    {
        bsp_hi6451_reg_read((HI_PMU_NO_PWR_IRQ1_OFFSET+i), &a_ucRecordReg[i]);
    }

    /*Ƿѹ����ѹ*/
    if(a_ucRecordReg[0] & (0x01 << HI6451_VINI_UNDER_2P7_OFFSET ))
        iret |= PMU_STATE_UNDER_VOL;
    if(a_ucRecordReg[0] & (0x01 << HI6451_VINI_OVER_6P0_OFFSET ))
        iret |= PMU_STATE_OVER_VOL;
    /*����*/
    if(a_ucRecordReg[1] & (0x01 << HI6451_CORE_OVER_CUR_OFFSET ) )
        iret |= PMU_STATE_OVER_CUR;
    /*����*/
    if(a_ucRecordReg[2] & (0x01 << HI6451_OTMP_150_OFFSET ) )
        iret |= PMU_STATE_OVER_TEMP;

    return iret;
}
/*****************************************************************************
 �� �� ��  : hi6451_ldo14_switch
 ��������  : ������ر�LDO14
 �������  : ��
 �������  : ��
 �� �� ֵ  : �����ɹ���ʧ��
 ���ú���  :
 ��������  : fastboot��lcd�ļ�����
*****************************************************************************/
void hi6451_ldo14_switch(power_switch_e sw)
{
    u8 regVal = 0;

    switch(sw)
    {
    case POWER_OFF:
        (void)bsp_hi6451_reg_read(HI_PMU_ONOFF4_OFFSET,&regVal);
        regVal &= ~ (0x01<<3);
        bsp_hi6451_reg_write(HI_PMU_ONOFF4_OFFSET,regVal);
        break;
    case POWER_ON:
        bsp_hi6451_reg_read(HI_PMU_ONOFF4_OFFSET,&regVal);
        regVal |=  (0x01<<3);
        bsp_hi6451_reg_write(HI_PMU_ONOFF4_OFFSET,regVal);
        break;
    default:
        break;
    }
    pmic_print_info("hi6451_ldo14_switch ok!");
}
/*****************************************************************************
 �� �� ��  : hi6451_ldo14_volt_set
 ��������  : ����ldo14�ĵ�ѹ
 �������  : int voltage�����õĵ�ѹֵ(ֻ֧��3/6/9/12/15/18/21/24uV)*
 �������  : ��
 �� �� ֵ  : �����ɹ���ʧ��
 ���ú���  :
 ��������  : fastboot��lcd�ļ�����
*****************************************************************************/
int hi6451_ldo14_volt_set(int voltage)
{
    unsigned int i = 0;
    int valid = 0;
    int ldo14_volt[] = {1100000,1200000,1800000,2200000,2500000,2600000,2800000,2850000};

    for(i = 0;i <= sizeof(ldo14_volt)/sizeof(int);i++)
    {
        if (voltage == ldo14_volt[i])
        {
            valid = 1;
            break;
        }
    }
    if(valid)
    {
        bsp_hi6451_reg_write_mask(HI_PMU_SET_LDO14_OFFSET, i, 0x7);
        pmic_print_info("hi6451_ldo14_volt_set ok!");
        return BSP_PMU_OK;
    }
    else
    {
        pmic_print_error("ldo14 cann't support %d uV!\n",voltage);
        return BSP_PMU_ERROR;
    }

}
