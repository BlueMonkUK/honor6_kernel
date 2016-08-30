/*************************************************************************
*   ��Ȩ����(C) 1987-2011, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  DrvInterface.h
*
*   ��    �� :  yangzhi
*
*   ��    �� :  ���ļ�����Ϊ"DrvInterface.h", ����V7R1�����Э��ջ֮���API�ӿ�ͳ��
*
*   �޸ļ�¼ :  2011��1��18��  v1.00  yangzhi����
*************************************************************************/

#ifndef __DRV_CHG_H__
#define __DRV_CHG_H__

#include "drv_comm.h"

/*************************CHGģ��START********************************/

/*****************************************************************************
 �� �� ��  : BSP_CHG_GetCbcState
 ��������  : ���ص��״̬�͵���
 �������  :pusBcs 0:����ڹ��� 1:�������ӵ��δ���� 2:û����������
                          pucBcl  0:���������û����������
 �������  : pusBcs 0:����ڹ��� 1:�������ӵ��δ���� 2:û����������
                          pucBcl  0:���������û����������
 ����ֵ��   0 �����ɹ�
                         -1����ʧ��

*****************************************************************************/
extern int BSP_CHG_GetCbcState(unsigned char *pusBcs,unsigned char *pucBcl);
extern int DRV_CHG_GET_CBC_STATE(unsigned char *pusBcs,unsigned char *pucBcl);

typedef enum CHARGING_STATE_ENUM_tag
{
        CHARGING_INIT = -1,
        NO_CHARGING_UP = 0,  /*����δ���*/
        CHARGING_UP ,              /*���������*/
        NO_CHARGING_DOWN ,  /*�ػ�δ���*/
        CHARGING_DOWN         /*�ػ�δ���*/
}CHARGING_STATE_ENUM;

typedef enum BATT_LEVEL_ENUM_tag
{
        BATT_INIT = -2,
        BATT_LOW_POWER =-1,    /*��ص͵�*/
        BATT_LEVEL_0,                 /*0���ص���*/
        BATT_LEVEL_1,                  /*1���ص���*/
        BATT_LEVEL_2,                   /*2���ص���*/
        BATT_LEVEL_3,                    /*3���ص���*/
        BATT_LEVEL_4,                    /*4���ص���*/
        BATT_LEVEL_MAX
}BATT_LEVEL_ENUM;

typedef struct BATT_STATE_tag
{
    CHARGING_STATE_ENUM  charging_state;
    BATT_LEVEL_ENUM           battery_level;
}BATT_STATE_T;


/*AT ����ö������*/
typedef enum
{
    CHG_AT_BATTERY_CHECK,        /* BatteryУ׼*/
    CHG_AT_BATTERY_LEVEL,        /* Battery���� */
    CHG_AT_BATTERY_ADC,          /* Battery Temperature����*/
    CHG_AT_BATTERY_INVALID
} ENUM_CHG_ATCM_TYPE;

typedef enum ENUM_POWER_ON_MODE_tag
{
    POWER_ON_INVALID = 0,          	/* ��Ч����ģʽ*/
    POWER_ON_NORMAL,       	 	/* ��������ģʽ*/
    POWER_ON_CHARGING,       	/* �ػ����ģʽ*/
    POWER_ON_MAX
} ENUM_POWER_ON_MODE;
/*****************************************************************************
 �� �� ��  : BSP_CHG_GetBatteryState
 ��������  :��ȡ�ײ���״̬��Ϣ
 �������  :battery_state ������Ϣ
 �������  :battery_state ������Ϣ
 ����ֵ��   0 �����ɹ�
                         -1����ʧ��

*****************************************************************************/
extern int BSP_CHG_GetBatteryState(BATT_STATE_T *battery_state);
extern int DRV_CHG_GET_BATTERY_STATE(BATT_STATE_T *battery_state);

/*****************************************************************************
 �� �� ��  : BSP_CHG_ChargingStatus
 ��������  :��ѯĿǰ�Ƿ����ڳ����
 �������  :��
 �������  :��
 ����ֵ��   0 δ���
                          1 �����
*****************************************************************************/
extern int BSP_CHG_ChargingStatus(void);
extern int DRV_CHG_GET_CHARGING_STATUS(void);

/*****************************************************************************
 �� �� ��  : BSP_CHG_StateSet
 ��������  :ʹ�ܻ��߽�ֹ���
 �������  :ulState      0:��ֹ���
 						1:ʹ�ܳ��
 �������  :��
 ����ֵ��    ��
*****************************************************************************/
extern void DRV_CHG_STATE_SET(unsigned long ulState);

/*****************************************************************************
 �� �� ��  : BSP_CHG_StateGet
 ��������  :��ѯ
 �������  :
 �������  :��
 ����ֵ��    ��
*****************************************************************************/
extern BSP_S32 BSP_CHG_StateGet(void);
#define DRV_CHG_STATE_GET()    BSP_CHG_StateGet()

/*****************************************************************************
 �� �� ��  : BSP_CHG_Sply
 ��������  :��ѯ
 �������  :
 �������  :��
 ����ֵ��    ��
*****************************************************************************/
extern BSP_S32 BSP_CHG_Sply(void);
#define DRV_CHG_BATT_SPLY()    BSP_CHG_Sply()


/*****************************************************************************
* �� �� ��  : BSP_TBAT_Read
* ��������  :
* �������  :
* �������  :
* �� �� ֵ  :
* �޸ļ�¼  :
*****************************************************************************/
extern BSP_S32 BSP_TBAT_Read(ENUM_CHG_ATCM_TYPE atID, void *pItem);

/*****************************************************************************
* �� �� ��  : BSP_TBAT_Write
* ��������  :
* �������  :
* �������  :
* �� �� ֵ  :
*****************************************************************************/
extern BSP_S32 BSP_TBAT_Write(ENUM_CHG_ATCM_TYPE atID, void *pItem);

/**********************************************************************
�� �� ��      : BSP_TBAT_CHRStGet
��������  :  TBAT AT^TCHRENABLE?�Ƿ���Ҫ����

�������  : ��
�������  : ��
�� �� ֵ      : 1:��Ҫ����
			      0:����Ҫ����
ע������  : ��
***********************************************************************/
extern BSP_S32 BSP_TBAT_CHRStGet(void);

/**********************************************************************
�� �� ��      : BSP_TBAT_CHRSuply
��������  :  TBAT AT^TCHRENABLE=4����ʵ��
            			��Ҫ�ṩ����ɹ�LCD��ʾͼ��
�������  : ��
�������  : ��
�� �� ֵ      :
ע������  : ��
***********************************************************************/
extern BSP_S32 BSP_TBAT_CHRSply(void);

/**********************************************************************
�� �� ��      : BSP_ONOFF_StartupModeGet
��������  :  A���жϿ��ػ�ģʽ

�������  : ��
�������  : ��
�� �� ֵ      :
ע������  : ��
***********************************************************************/
ENUM_POWER_ON_MODE BSP_ONOFF_StartupModeGet( void );
/*****************************************************************************
* �� �� ��  : BSP_PMU_BattCali
*
* ��������  : ���У׼ǰ��С��������ӿڣ���AT����
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
extern void BSP_PMU_BattCali(void);



static INLINE BSP_S32 BSP_MNTN_GetBattState(void)
{
	return 0;
}
#define DRV_MNTN_GET_BATT_STATE() BSP_MNTN_GetBattState()


/*************************CHGģ�� END*********************************/

#endif

