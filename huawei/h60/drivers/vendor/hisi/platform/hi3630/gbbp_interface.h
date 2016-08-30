#ifndef __GBB_PINTERFACE_H__
#define __GBB_PINTERFACE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "soc_interface.h"


/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define GBBP_ADDR_OFFSET                    (0x8000)

#ifndef BIT_X
#define BIT_X(num)                          (((unsigned long)0x01) << (num))
#endif

/*--------------------------------------------------------------------------------------------*
 *  BBP �Ĵ�����ַ
 *--------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------*
 * 1.1  ǰ��
 *--------------------------------------------------------------------------------------------*/

/* ʹ�õ�bbp�ж����bitλ */
#define GBBP_FRM_INT_BIT                ( BIT_X(0) )                            /* ֡�жϱ���λ */
#define GBBP_SYNC_INT_BIT               ( BIT_X(2) )                            /* ͬ�������жϱ���λ */
#define GBBP_DECODE_INT_BIT             ( BIT_X(3) )                            /* ���������ϱ��жϱ���λ */
#define GBBP_CFG_INT_BIT                ( BIT_X(8) )                            /* GSM�����жϱ���λ,4ʱ϶�����ж� */
#define GBBP_MEAS_INT_BIT               ( BIT_X(28) )                           /* �����жϱ���λ */
#define GBBP_DEMODE_INT_BIT             ( BIT_X(29) )                           /* GSM����жϱ���λ */
#define GBBP_WCELL_CFG_INT_BIT          ( BIT_X(12) )                           /* ��ϵͳ�����ж� */

#define GBBP_WCELL_STOP_INT_BIT         ( BIT_X(13) )                           /* ��ϵͳ�����ж� */
#define GBBP_WCELL_DELAY_INT_BIT        ( BIT_X(14) )                           /* ��ϵͳ�����ж� */

#define GPHY_BG_W_CFG_INT_BIT           ( BIT_X(15) )                           /* ΪW�����Ѵ������Զ����ж�  */
#define GPHY_BG_RFF_OFF_INT_BIT         ( BIT_X(16) )                           /* �쳣ֹͣw����ʱ�Ĵ��� */

#define GBBP_GAUGE_INT_BIT              ( BIT_X(27) )                           /* ��У׼32.768KHzʱ�Ӽ�������֮�󣬲���У׼�ж� */

#define GBBP_LMEAS_INT0_INT_BIT         ( BIT_X(24) )                           /* LTE����ʱ�����CTU��INT0�ж�(OccasionԤ�����ж�) */
#define GBBP_LMEAS_INT1_INT_BIT         ( BIT_X(25) )                           /* LTE����ʱ�����CTU��INT1�ж�(Occasion��ʼ�ж�) */
#define GBBP_LMEAS_INT2_INT_BIT         ( BIT_X(26) )                           /* LTE����ʱ�����CTU��INT2�ж�(Occasion�����ж�) */







/*************************************** CPU���� *********************************************/
#define HAL_GTC_RAM_ADDR                        ( 0x0000 )                      /* GTCָ��RAM */

/*************************************** GTC���� *********************************************/
#define HAL_GTC_NOP_ADDR                        ( 31 )                          /* GTC NOPָ���ַ */

/************************************* GTCָ��� ******************************************/
#define HAL_GTC_START_INSTRUCTION_CODE          ( 0 )                           /* STARTָ���� */
#define HAL_GTC_FRM_OFFSET_INSTRUCTION_CODE     ( 1 )                           /* FRM_OFFSETָ���� */
#define HAL_GTC_FRM_IT_INSTRUCTION_CODE         ( 2 )                           /* FRM_ITָ���� */
#define HAL_GTC_HOLD_INSTRUCTION_CODE           ( 3 )                           /* HOLDָ���� */
#define HAL_GTC_STOP_INSTRUCTION_CODE           ( 4 )                           /* STOPָ���� */
#define HAL_GTC_CONFIG1_INSTRUCTION_CODE        ( 5 )                           /* CONFIG1ָ���� */
#define HAL_GTC_CONFIG2_INSTRUCTION_CODE        ( 6 )                           /* CONFIG2ָ���� */
#define HAL_GTC_CONFIG3_INSTRUCTION_CODE        ( 7 )                           /* CONFIG3ָ���� */


/*--------------------------------------------------------------------------------------------*
 * 1.2  GBBP�жϿ���ģ��
 *--------------------------------------------------------------------------------------------*/

/*************************************** GTC���� *********************************************/
#define HAL_GTC_CPU_GRM_INT_ADDR                ( 28 )                          /* ��ʾ8��CPU�ɱ���ж� */
#define HAL_GTC_SLOTINT_OFFSET_L_ADDR           ( 30 )                          /* 8��ʱ϶ͷ�ж�ƫ��λ�ã�һ��ʱ϶�ڣ���8bit */
#define HAL_GTC_SLOTINT_OFFSET_H_ADDR           ( 31 )                          /* 8��ʱ϶ͷ�ж�ƫ��λ�ã�һ��ʱ϶�ڣ���8bit */

/*************************************** CPU���� *********************************************/
#define HAL_GBBP_AGC_DEC_INT_T3_FN_ADDR         ( 0x5214 )                      /* GBBP AGC����,NBҵ�������ж�����TDMA֡��ģ51����ֵ */
#define HAL_GBBP_DEM_INT_FN_QB_ADDR             ( 0x5218 )                      /* GBBP ����ж�����TDMA֡��ģ51����ֵ,QB����ֵ */
#define HAL_GBBP_IMI_INT_QB_RPT_ADDR            ( 0x7008 )                      /* IMIʱ���ж��ϱ�ʱGTC��TDMA QB������������ */
#define HAL_GBBP_DRX_SLEEP_INT_QB_RPT_ADDR      ( 0x700c )                      /* 32Kʱ���ж��ϱ�ʱGTC��TDMA QB������������ */


#define HAL_GBBP_ARM_INT_STA_ADDR               ( 0x1200 )                      /* GBBP �ж�״̬�Ĵ��� ARM*/
#define HAL_GBBP_ARM_INT_MASK_ADDR              ( 0x1204 )                      /* GBBP �ж�����λ ARM*/
#define HAL_GBBP_ARM_INT_CLR_ADDR               ( 0x1208 )                      /* GBBP �ж�����Ĵ��� ARM*/
#define HAL_GBBP_DSP_INT_STA_ADDR               ( 0x1210 )                      /* GBBP �ж�״̬�Ĵ��� */
#define HAL_GBBP_DSP_INT_MASK_ADDR              ( 0x1214 )                      /* GBBP �ж�����λ */
#define HAL_GBBP_DSP_INT_CLR_ADDR               ( 0x1218 )                      /* GBBP �ж�����Ĵ��� */
#define HAL_GBBP_IMI_INT_QB_RPT_POWERDOWN_ADDR  ( 0x1220 )                      /* GBBP ������ DSP IMIʱ���ж��ϱ�ʱGTC��TDMA QB���� */


/*--------------------------------------------------------------------------------------------*
 * 1.3  ��Ƶ����ģ��
 *--------------------------------------------------------------------------------------------*/

/*************************************** GTC���� *********************************************/
#define HAL_GTC_SPI_RD_START_ADDR                   ( 0 )                           /* GTC SPI����������SPI DATA RAM�е���ʼλ�� */
#define HAL_GTC_SPI_TRANS_DATA_NUM_ADDR             ( 1 )                           /* ������Ҫ���͵�SPI���������� */
#define HAL_GTC_SPI_RD_EN_ADDR                      ( 2 )                           /* GTC��������SPI�������� */
#define HAL_GTC_GSP_LINE1_CTRL_BITWISE_CLR_ADDR     ( 25 )                          /* ��һ���߿�������� */
#define HAL_GTC_GSP_LINE1_CTRL_BITWISE_SET_ADDR     ( 26 )                          /* ��һ���߿���λ���� */
#define HAL_GTC_GSP_LINE2_CTRL_BITWISE_CLR_ADDR     ( 14 )                          /* �ڶ����߿�������� */
#define HAL_GTC_GSP_LINE2_CTRL_BITWISE_SET_ADDR     ( 15 )                          /* �ڶ����߿���λ���� */
#define HAL_GTC_GSP_LINE3_CTRL_BITWISE_CLR_ADDR     ( 18 )                          /* �������߿�������� */
#define HAL_GTC_GSP_LINE3_CTRL_BITWISE_SET_ADDR     ( 19 )                          /* �������߿���λ���� */
#define HAL_GTC_GSP_LINE4_CTRL_BITWISE_CLR_ADDR     ( 20 )                          /* �������߿�������� */
#define HAL_GTC_GSP_LINE4_CTRL_BITWISE_SET_ADDR     ( 21 )                          /* �������߿���λ���� */
#define HAL_GTC_GSP_EXT_LINE_CTRL_BITWISE_CLR_ADDR  ( 22 )                          /* ��չ�߿�������� */
#define HAL_GTC_GSP_EXT_LINE_CTRL_BITWISE_SET_ADDR  ( 23 )                          /* ��չ�߿���λ���� */
#define HAL_GTC_GSP_LINE01_SEL_CFG_ADDR             ( 27 )                          /* ��ͨ����������ѡ���ź� */
#define HAL_GTC_GSP_MIPI_START_INI_ADDR             ( 16 )
#define HAL_GTC_GSP_MIPI_MUM_ADDR                   ( 17 )

/*************************************** CPU���� *********************************************/
#define HAL_GBBP_CPU_SPI_RF_CFG_ADDR                ( 0x1368 )                      /* SPI�Ĵ������� */
#define HAL_GBBP_CPU_SPI_RF_RD_ADDR                 ( 0x136C )                      /* CPU����SPI���ݷ������� */
#define HAL_GBBP_CPU_LINE_CTRL_ADDR                 ( 0x1364 )                      /* RF�߿��ź�CPU���õ�ַ */
#define HAL_GBBP_CPU_EXT_LINE_CTRL_ADDR             ( 0x5E04 )
#define HAL_GBBP_GTC_GSP_EXT_LINE_STATE_RPT_ADDR    ( 0x5E08 )
#define HAL_GBBP_CPU_LINE01_SEL_CFG_ADDR            ( 0x5e0c )
#define HAL_GBBP_CPU_LINE01_SEL_STATE_RPT_ADDR      ( 0x5e10 )

/*--------------------------------------------------------------------------------------------*
 * 1.4  GRIFģ��
 *--------------------------------------------------------------------------------------------*/
/*************************************** GTC���� *********************************************/
#define HAL_GTC_GSP_RX_LOW_LEN_ADDR                 ( 0 )
#define HAL_GTC_GSP_RX_HIGH_LEN_ADDR                ( 1 )
#define HAL_GTC_GSP_GRIF_CTRL_ADDR                  ( 2 )

/*************************************** CPU���� *********************************************/
#define HAL_GBBP_ADDR_RF_UNIFY_SPI_1REG_ADDR        ( 0xF260 )
#define HAL_GBBP_ADDR_RF_UNIFY_SPI_2REG_ADDR        ( 0xF264 )
#define HAL_GBBP_ADDR_RF_UNIFY_SPI_3REG_ADDR        ( 0xF268 )
#define HAL_GBBP_ADDR_RF_UNIFY_SPI_4REG_ADDR        ( 0xF26C )
#define HAL_GBBP_ADDR_RF_UNIFY_SPI_5REG_ADDR        ( 0xF270 )
#define HAL_GBBP_ADDR_RF_UNIFY_SPI_6REG_ADDR        ( 0xF274 )
#define HAL_GBBP_ADDR_RF_UNIFY_SPI_7REG_ADDR        ( 0xF278 )
#define HAL_GBBP_ADDR_RF_UNIFY_SPI_8REG_ADDR        ( 0xF27C )
#define HAL_GBBP_ADDR_RF_UNIFY_SPI_9REG_ADDR        ( 0xF280 )
#define HAL_GBBP_ADDR_RF_UNIFY_SPI_10REG_ADDR       ( 0xF284 )
#define HAL_GBBP_ABB_CONFIG_SEL_ADDR                ( 0xF288 )
#define HAL_GBBP_ADDR_G_TX_DC_OFFSET_ADDR           ( 0xF290 )
#define HAL_GBBP_ADDR_G_CH_PARA_1REG_ADDR           ( 0xF294 )
#define HAL_GBBP_ADDR_G_CH_PARA_2REG_ADDR           ( 0xF298 )
#define HAL_GBBP_ADDR_G_CH_PARA_3REG_ADDR           ( 0xF29C )
#define HAL_GBBP_ADDR_G_CH_PARA_4REG_ADDR           ( 0xF2A0 )
#define HAL_GBBP_ADDR_G_CH_PARA_5REG_ADDR           ( 0xF2A4 )
#define HAL_GBBP_ADDR_G_CH_PARA_6REG_ADDR           ( 0xF2A8 )
#define HAL_GBBP_ADDR_G_CH_PARA_7REG_ADDR           ( 0xF2AC )
#define HAL_GBBP_ADDR_G_CH_PARA_8REG_ADDR           ( 0xF2B0 )
#define HAL_GBBP_ADDR_G_CH_PARA_9REG_ADDR           ( 0xF2B8 )
#define HAL_GBBP_ADDR_G_CH_PARA_10REG_ADDR          ( 0xF2C4 )
#define HAL_GBBP_ADDR_G_CH_PARA_11REG_ADDR          ( 0xF2C8 )
#define HAL_GBBP_ADDR_G_CH_PARA_12REG_ADDR          ( 0xF2CC )
#define HAL_GBBP_ADDR_G_CH_PARA_13REG_ADDR          ( 0xF2D0 )
#define HAL_GBBP_ADDR_G_CH_PARA_14REG_ADDR          ( 0xF2D4 )
#define HAL_GBBP_ADDR_G_CH_PARA_15REG_ADDR          ( 0xF2D8 )
#define HAL_GBBP_ADDR_G_CH_PARA_16REG_ADDR          ( 0xF2DC )
#define HAL_GBBP_ADDR_G_CH_PARA_17REG_ADDR          ( 0xF2E0 )
#define HAL_GBBP_ADDR_G_CH_DC_OFFSET_ADDR           ( 0xF2B4 )
#define HAL_GBBP_ADDR_RF_UNIFY_TEST_PIN_SEL_ADDR    ( 0xF2BC )
#define HAL_GBBP_ADDR_RF_UNIFY_TEST_PIN_TRIG_ADDR   ( 0xF2C0 )
#define HAL_GBBP_RX_RSHIFT_NUM_SEL_ADDR             ( 0xF2E4 )
#define HAL_GBBP_CLIP_MODE_SEL_ADDR                 ( 0xF2E8 )
#define HAL_GBBP_CPU_GRIF_FIFO_RD_ADDR              ( 0xf2ec )
#define HAL_GBBP_CPU_GRIF_CONFIG_ADDR               ( 0xf2f0 )
#define HAL_GBBP_ADDR_G_32CH_PARA_1REG_ADDR         ( 0xF31C )
#define HAL_GBBP_ADDR_G_32CH_PARA_2REG_ADDR         ( 0xF320 )
#define HAL_GBBP_ADDR_G_32CH_PARA_3REG_ADDR         ( 0xF324 )
#define HAL_GBBP_ADDR_G_32CH_PARA_4REG_ADDR         ( 0xF328 )
#define HAL_GBBP_ADDR_G_32CH_PARA_5REG_ADDR         ( 0xF32C )
#define HAL_GBBP_ADDR_G_32CH_PARA_6REG_ADDR         ( 0xF330 )
#define HAL_GBBP_ADDR_G_32CH_PARA_7REG_ADDR         ( 0xF334 )
#define HAL_GBBP_ADDR_G_32CH_PARA_8REG_ADDR         ( 0xF338 )
#define HAL_GBBP_ADDR_G_32CH_PARA_9REG_ADDR         ( 0xF33C )
/*--------------------------------------------------------------------------------------------*
 *1.5   �Զ��������ģ��
 *--------------------------------------------------------------------------------------------*/

/*************************************** GTC���� *********************************************/
#define HAL_GTC_GSP_DAGC_CTRL_ADDR                  ( 3 )                       /* GSP DAGC�߿����� */
#define HAL_GTC_GSP_AGC_CTRL1_ADDR                  ( 20 )                      /* AGCģ�鹤������ 1 */
#define HAL_GTC_GSP_AGC_CTRL2_ADDR                  ( 21 )                      /* AGCģ�鹤������ 2 */
#define HAL_GTC_GSP_AGC_START_ADDR                  ( 22 )                      /* AGC�������� */
#define HAL_GTC_GSP_RSSI_FRQ_LOW_ADDR               ( 3 )                       /* ��ǰ������RSSI��Ӧ��Ƶ��ŵ�8bit */
#define HAL_GTC_GSP_RSSI_FRQ_HIGH_ADDR              ( 4 )                       /* ��ǰ������RSSI��Ӧ��Ƶ��Ÿ�4bit */
#define HAL_GTC_GSP_INIT_GAIN_RFIC_ADDR             ( 7 )                       /* RFÿʱ϶����ĳ�ʼ��λ */
#define HAL_GTC_GSP_AGC_TRUNC_WIDTH_ADDR            ( 13 )                      /* AGC��������ϸ������λλ�� */

/*************************************** CPU���� *********************************************/
#define HAL_GBBP_R2_RSSI1234_THRESHOLD_ADDR         ( 0x5204 )                  /* 2�ֲ������� */
#define HAL_GBBP_R2_RSSI4567_THRESHOLD_ADDR         ( 0x5208 )                  /* 2�ֲ������� */
#define HAL_GBBP_R3_RSSI1234_THRESHOLD_ADDR         ( 0x5210 )
#define HAL_GBBP_R3_RSSI4567_THRESHOLD_ADDR         ( 0x521c )
#define HAL_GBBP_AGC_WAIT_LENGTH_ADDR               ( 0x520c )                  /* ����ʱ��Ͳ���ģʽ���� */
#define HAL_GBBP_DC_IQ_REMOVE_RPT_ADDR              ( 0x5220 )                  /* ��·����ϸ����ƫ����� */
#define HAL_GBBP_DC_IQ_BURST_REMOVE_RPT_ADDR        ( 0x5224 )                  /* ʱ϶ǰ����I·, Q·ֱ��ƫ���ϱ� */
#define HAL_GBBP_AAGC_NEXT_GAIN_ADDR                ( 0x5250 )                  /* AGC������������ */
#define HAL_GBBP_AAGC_GAIN1_CPU_ADDR                ( 0x1460 )                  /* ���һ��ʱ϶AGC������������ */
#define HAL_GBBP_AAGC_GAIN2_CPU_ADDR                ( 0x1464 )                  /* �ν�һ��ʱ϶AGC������������ */
#define HAL_GBBP_AAGC_GAIN3_CPU_ADDR                ( 0x1468 )                  /* �δν�һ��ʱ϶AGC������������ */
#define HAL_GBBP_AAGC_GAIN4_CPU_ADDR                ( 0x146c )                  /* ��Զһ��ʱ϶AGC������������ */
#define HAL_GBBP_RSSI_QB_FRQ_ADDR                   ( 0x5254 )                  /* �����ж�ʱ��TDMA֡QBֵ */
#define HAL_GBBP_DRSSI_ENERGY_RPT_ADDR              ( 0x5258 )                  /* DRSSI���������ǰ��ֵ�ϱ� */
#define HAL_GBBP_AGC_FAST1_2_RSSI_CPU_ADDR          ( 0x525c )                  /* ���ٲ�����һ���Լ��ڶ���RSSI�ϱ��� */
#define HAL_GBBP_AAGC_GAIN_STATE_REG_ADDR           ( 0x5260 )                  /* ����RF��С�Լ���С������Ƶ�λ */
#define HAL_GBBP_AAGC_GAIN_THRD_REG_ADDR            ( 0x5264 )                  /* AGCĿ�������뵱ǰ����֮��Ŀ�����ֵ */
#define HAL_GBBP_CPU_OFC_RXCM_REG_ADDR              ( 0x5360 )                  /* OFC�������,agc ͨ������ƹ���л�ʹ�� */
#define HAL_GBBP_CPU_RX_ADDR                        ( 0x5368 )                  /* GSM������ƼĴ�����ַ */
#define HAL_GBBP_CPU_SAMPLE_REG_ADDR                ( 0x536C )                  /* ������ѡ����ֱ��ƫ����· */
#define HAL_GBBP_CPU_AGC_RAM_SWITCH_IND             ( 0x5370 )                  /* AGCֱ��ƫ��RAM��ͨ������RAM�ֶ��л�ָʾ�źţ����ź�Ϊ�����źţ������� */
#define HAL_GBBP_DAGC_MULFACTOR_FIX                 ( 0x537c )                  /* DRSSI�ϱ������ӦΪABB����źŵ����� */
#define HAL_GBBP_AGC_IP2_I_RPT_ADDR                 ( 0x5278 )                  /* I·4����������ƽ���ۼ��ϱ� */
#define HAL_GBBP_AGC_IP2_Q_RPT_ADDR                 ( 0x527C )                  /* Q·4����������ƽ���ۼ��ϱ� */
#define HAL_GBBP_CPU_SPI_WORD_SEL_ADDR              ( 0x5580 )                  /* ���н�������SPI�����ָ�ʽѡ�� */
#define HAL_GBBP_CPU_GAIN1_SPI_DATA_ADDR            ( 0x5584 )                  /* ��������ģʽ�µ�1��SPI������ */
#define HAL_GBBP_CPU_GAIN2_SPI_DATA_ADDR            ( 0x5588 )                  /* ��������ģʽ�µ�2��SPI������ */
#define HAL_GBBP_CPU_GAIN3_SPI_DATA_ADDR            ( 0x558c )                  /* ��������ģʽ�µ�3��SPI������ */
#define HAL_GBBP_CPU_GAIN4_SPI_DATA_ADDR            ( 0x5590 )                  /* ��������ģʽ�µ�4��SPI������ */
#define HAL_GBBP_CPU_GAIN5_SPI_DATA_ADDR            ( 0x5594 )                  /* ��������ģʽ�µ�5��SPI������ */
#define HAL_GBBP_CPU_GAIN6_SPI_DATA_ADDR            ( 0x5598 )                  /* ��������ģʽ�µ�6��SPI������ */
#define HAL_GBBP_CPU_GAIN7_SPI_DATA_ADDR            ( 0x559c )                  /* ��������ģʽ�µ�7��SPI������ */
#define HAL_GBBP_CPU_GAIN8_SPI_DATA_ADDR            ( 0x55a0 )                  /* ��������ģʽ�µ�8��SPI������ */
#define HAL_GBBP_CPU_DIV_GAIN1_SPI_DATA_ADDR        ( 0x55a4 )                  /* ��������ģʽ�µ�1��SPI������ */
#define HAL_GBBP_CPU_DIV_GAIN2_SPI_DATA_ADDR        ( 0x55a8 )                  /* ��������ģʽ�µ�2��SPI������ */
#define HAL_GBBP_CPU_DIV_GAIN3_SPI_DATA_ADDR        ( 0x55ac )                  /* ��������ģʽ�µ�3��SPI������ */
#define HAL_GBBP_CPU_DIV_GAIN4_SPI_DATA_ADDR        ( 0x55b0 )                  /* ��������ģʽ�µ�4��SPI������ */
#define HAL_GBBP_CPU_DIV_GAIN5_SPI_DATA_ADDR        ( 0x55b4 )                  /* ��������ģʽ�µ�5��SPI������ */
#define HAL_GBBP_CPU_DIV_GAIN6_SPI_DATA_ADDR        ( 0x55b8 )                  /* ��������ģʽ�µ�6��SPI������ */
#define HAL_GBBP_CPU_DIV_GAIN7_SPI_DATA_ADDR        ( 0x55bc )                  /* ��������ģʽ�µ�7��SPI������ */
#define HAL_GBBP_CPU_DIV_GAIN8_SPI_DATA_ADDR        ( 0x55c0 )                  /* ��������ģʽ�µ�8��SPI������ */
#define HAL_GBBP_DC_OFFSET_ADDR                     ( 0x6000 )                  /* DC Offset */
#define HAL_GBBP_CHANNEL_GAIN_ADDR                  ( 0x6400 )                  /* Channel Gain */
#define HAL_GBBP_SLOT_DCR_RSSI_THRESHOLD_ADDR       ( 0x5378 )
#define HAL_GBBP_GACI_THERSHOLD_ADDR                ( 0x5884 )                  /* ��Ƶ������޲��� */
/*--------------------------------------------------------------------------------------------*
 * 1.7  ��λģ��
 *--------------------------------------------------------------------------------------------*/

/*************************************** GTC���� *********************************************/

/*************************************** CPU���� *********************************************/
#define HAL_GBBP_CPU_RESET_ADDR                     ( 0x1350 )                  /* BBP��λ�����ź� */

/*--------------------------------------------------------------------------------------------*
 * 1.8  ��ģ��ʱ�ӿ���ģ��
 *--------------------------------------------------------------------------------------------*/

/*************************************** GTC���� *********************************************/

/*************************************** CPU���� *********************************************/
#define HAL_GBBP_CPU_SOFT_CLK_CTRL_ADDR             ( 0x1354 )                  /* ������ģ���ʱ�ӹرպʹ�ͨ���� */
#define HAL_GBBP_CPU_AUTO_CLK_BYPASS_ADDR           ( 0x135c )                  /* GBBP�Զ��ſ� */

/*--------------------------------------------------------------------------------------------*
 * 1.9  GTCģ��
 *--------------------------------------------------------------------------------------------*/

/*************************************** GTC���� *********************************************/

/*************************************** CPU���� *********************************************/
#define HAL_GBBP_CPU_GTC_CFG_ADDR                   ( 0x1360 )                  /* CPU����GTCģ�� */
#define HAL_GBBP_GTC_STATE_RPT_ADDR                 ( 0x1374 )                  /* GTC״̬ */
#define HAL_GBBP_GTC_FRM_OFFSET_RPT_ADDR            ( 0x137c )                  /* TDMA��ʱ��TimeBaseʱ��ƫ�� */
#define HAL_GBBP_GTC_FRM_IT_OFFSET_RPT_ADDR         ( 0x1380 )                  /* ֡�ж���TimeBaseʱ��ƫ�� */
#define HAL_GBBP_GTC_GSP_LINE_STATE_RPT_ADDR        ( 0x1384 )                  /* GSPʵ���߿�״̬ */
#define HAL_GBBP_CPU_GTC_T1_T2_T3_CNG_ADDR          ( 0x1388 )                  /* ����GTC��T1T2T3ֵ */
#define HAL_GBBP_GTC_T1_T2_T3_RPT_CPU_ADDR          ( 0x138c )                  /* GTC�ڲ�ά���ļ�����T1T2T3 */
#define HAL_GBBP_CPU_GTC_FRM_OFFSET_CFG_ADDR        ( 0x1390 )                  /* cpu����֡ͷ���� */
#define HAL_GBBP_CPU_GTC_FN_CNG_ADDR                ( 0x1394 )                  /* ֡�����õ�bbp�� */
#define HAL_GBBP_GTC_FN_RPT_ADDR                    ( 0x1398 )                  /* ��ǰ֡�� */
#define HAL_GBBP_GTC_FN_HIGH_RPT_ADDR               ( 0x139A )                  /* ��ǰ֡�� */
#define HAL_GBBP_GTC_QB_RPT_ADDR                    ( 0x139c )                  /* ��ǰʱ��TDMA��qb����ֵ */
#define HAL_GBBP_TIMEBASE_RPT_ADDR                  ( 0x13A8 )                  /* ����ʱ���ϱ� */
#define HAL_GBBP_CPU_TIMING_GET_TRIGGER_ADDR        ( 0x5a20 )                  /* FN��Qb��ȡ���� */
#define HAL_GBBP_TIMING_GET_FN_RPT_ADDR             ( 0x5a24 )                  /* ������FN */
#define HAL_GBBP_TIMING_GET_TIMEBASE_RPT_ADDR       ( 0x5a28 )                  /* ������Qb */


/*--------------------------------------------------------------------------------------------*
 * 1.10 ͬ������ģ��
 *--------------------------------------------------------------------------------------------*/

/*************************************** GTC���� *********************************************/
#define HAL_GTC_SRCH_CFG_ADDR                       ( 3 )                       /* SYNͬ�����ƣ�����ͬ��ʹ�ܡ�����ģʽ */
#define HAL_GTC_SRCH_START_ADDR                     ( 4 )                       /* SYNͬ������ */
#define HAL_GTC_NCO_INIT_VALUE_HI_ADDR              ( 14 )                      /* ����Ƶƫ��ֵ�ĸ�8bit */
#define HAL_GTC_NCO_INIT_VALUE_LOW_ADDR             ( 13 )                      /* ����Ƶƫ��ֵ�ĵ�8bit */

/*************************************** CPU���� *********************************************/
#define HAL_GBBP_CPU_SOFT_AFC_VALUE_ADDR            ( 0x1000 )                  /* �������AFC��ֵ */
#define HAL_GBBP_CPU_SOFT_AFC_RENEW_ADDR            ( 0x1004 )                  /* ����������������AFCֵ�������ź� */
#define HAL_GBBP_CPU_CI_THRES_CFG_ADDR              ( 0x1008 )                  /* gmsk/8psk�������� */
#define HAL_GBBP_CPU_NB_AFC_NUM_ADDR                ( 0x100c )                  /* ���ڼ���NB-AFC��NB����ѡ���ź� */
#define HAL_GBBP_CPU_AFC_LOOP1_CFG_ADDR             ( 0x1080 )                  /* �ϵ��NB-AFC��·�˲��ۼ���һ������ֵ */
#define HAL_GBBP_CPU_AFC_LOOP2_CFG_ADDR             ( 0x5E20 )
#define HAL_GBBP_NB_AFC_LOOP_SWITCH_ADDR            ( 0x5340 )
#define HAL_GBBP_ADDR_NB_AFC_LOOP_SLOW_ADDR         ( 0x5E14 )
#define HAL_GBBP_ADDR_NB_AFC_LOOP_FAST_ADDR         ( 0x5E18 )
#define HAL_GBBP_AFC_LOOP2_RPT_ADDR                 ( 0x5E1C )
#define HAL_GBBP_CPU_AFC_CFG_ADDR                   ( 0x1010 )                  /* AFC����ģʽѡ��AFCֵ����/д�� */
#define HAL_GBBP_AFC_VAULE_I_CPU_ADDR               ( 0x1014 )                  /* �ϱ���afcʵ������ۼ�ֵ */
#define HAL_GBBP_AFC_VAULE_Q_CPU_ADDR               ( 0x1018 )                  /* �ϱ���afc�鲿����ۼ�ֵ */
#define HAL_GBBP_CPU_AFC_PWM_WIDTH_SEL_ADDR         ( 0x101c )                  /* PDMת��ģ��ѡ�� */
#define HAL_GBBP_CPU_NB_AFC_NUM_CLR_ADDR            ( 0x1020 )                  /* ��ʾ���¿�ʼͳ��NB-AFC��NB-BURST���� */
#define HAL_GBBP_CPU_FB_KP_KINT_ADDR                ( 0x1024 )                  /* FB-AFC�������� */
#define HAL_GBBP_FB_TIMEBASE_RPT_ADDR               ( 0x1028 )                  /* �ͺ�FB 1215QB��timebase��QB��FN���� */
#define HAL_GBBP_FB_FN_RPT_ADDR                     ( 0x102c )                  /* ͬ������ʱ������ֵ����֡Ϊ��λ�� */
#define HAL_GBBP_CPU_FB_TIMES_THRES_ADDR            ( 0x1030 )                  /* FB������������ */
#define HAL_GBBP_CPU_FB_ENERG_THRES_ADDR            ( 0x1034 )                  /* FB�����õ��������� */
#define HAL_GBBP_AFC_RPT_ADDR                       ( 0x1038 )                  /* AFCֵ�ϱ� */
#define HAL_GBBP_AFC_LOOP1_RPT_ADDR                 ( 0x1078 )                  /* ��ǰNB-AFC��·�˲��ۼ���һ���ϱ�ֵ */
#define HAL_GBBP_AFC_PHASE_THRESHOLD_CFG_ADDR       ( 0x108c )                  /* NB-AFC�㷨��λ����ֵ�ο������� */
#define HAL_GBBP_AFC_FCUT_THRESHOLD_CFG_ADDR        ( 0x1090 )                  /* NB-AFC�㷨ϵ��fcut����Сֵ�Լ����ֵ */
#define HAL_GBBP_AFC_ALPHA_SEL_ADDR                 ( 0x1094 )                  /* NB-AFC�㷨���˲�ϵ��ѡ�� */
#define HAL_GBBP_FB_AFC_PHASE_THRESHOLD_CFG_ADDR    ( 0x1230 )                  /* FB-AFC�㷨��λ����ֵ�ο������� */
#define HAL_GBBP_FB_AFC_FCUT_THRESHOLD_CFG_ADDR     ( 0x1234 )                  /* FB-AFC�㷨ϵ��fcut����С,���ֵ */
#define HAL_GBBP_FB_AFC_ALPHA_SEL_ADDR              ( 0x1238 )                  /* NB-AFC�㷨���˲�ϵ��ѡ�� */
#define HAL_GBBP_FB_RPT1_ADDR                       ( 0x103c )                  /* FB�����õ���FBʱ϶ͷλ�ã������gtc_timebaseλ�� */
#define HAL_GBBP_FB_RPT2_ADDR                       ( 0x1040 )                  /* ����FB�������������ֵ */
#define HAL_GBBP_SB_POS_RPT1_ADDR                   ( 0x1044 )                  /* SB�����ɹ��õ���֡ͷ��TIMEBASE�Ĳ� */
#define HAL_GBBP_SB_SQUARE_RPT1_ADDR                ( 0x1048 )                  /* SB��������ֵ�����ֵ */
#define HAL_GBBP_SB_POS_RPT2_ADDR                   ( 0x104c )                  /* SB�����δ�ֵ�����timebase��Ӧ��λ�� */
#define HAL_GBBP_SB_SQUARE_RPT2_ADDR                ( 0x1050 )                  /* SB�����δ�ֵ */
#define HAL_GBBP_SB_POS_RPT3_ADDR                   ( 0x1054 )                  /* SB���������ĵ�����ֵ�����timebase��λ�� */
#define HAL_GBBP_SB_SQUARE_RPT3_ADDR                ( 0x1058 )                  /* SB���������ĵ�����ֵ */
#define HAL_GBBP_STATE_RPT_ADDR                     ( 0x105C )                  /* �����ж�״̬ */
#define HAL_GBBP_SRCH_QB_RPT_ADDR                   ( 0x1064 )                  /* �����ж�����gtc_timebase�ϱ� */
#define HAL_GBBP_SRCH_FN_RPT_ADDR                   ( 0x106c )                  /* �����ж�����λ�� */
#define HAL_GBBP_SB_ADVANCE_LEN_CFG_ADDR            ( 0x1070 )                  /* 001����ģʽ�£�SB��ǰ���յķ��Ÿ��� */
#define HAL_GBBP_SRCH_OPT_CFG_ADDR                  ( 0x1480 )                  /* �����Ż�ģʽ���� */
#define HAL_GBBP_CPU_SRCH_EN_CLR_ADDR               ( 0x1124 )                  /* ������ʹ�ܣ�����ʹ�� */
#define HAL_GBBP_NCELL_NCO_RPT                      ( 0x1084 )
#define HAL_GBBP_AFC_K_VALUE_ADDR                   ( 0x5e00 )                  /* AFC Kֵ������Ĭ��ֵΪ10'd128��TCXO�����±���Ĭ��ֵ��DCXO�����¸���ʵ��ϵͳ���е����� */

/*--------------------------------------------------------------------------------------------*
 * 1.11 ���н��ģ��
 *--------------------------------------------------------------------------------------------*/

/*************************************** GTC���� *********************************************/
#define HAL_GTC_DEMOD_PARA1_ADDR                    ( 16 )                      /* �������1 */
#define HAL_GTC_DEMOD_PARA2_ADDR                    ( 17 )                      /* �������2 */
#define HAL_GTC_DEMOD_PARA3_ADDR                    ( 18 )                      /* �������3 */
#define HAL_GTC_TOA_PARA_ADDR                       ( 29 )                      /* TOA���� */
#define HAL_GTC_DEMOD_START_ADDR                    ( 19 )                      /* ����NB��������ź� */

/*************************************** CPU���� *********************************************/
#define HAL_GBBP_NB_POS_RPT_ADDR                    ( 0x1060 )                  /* NBλ�ã������ϱ� */
#define HAL_GBBP_CPU_8PSK_PSP_ALPHA_PARA_ADDR       ( 0x1068 )                  /* 8psk����������PSP_EN�Ĳ���alpha */
#define HAL_GBBP_CPU_TOA_ALPHA_PARA_ADDR            ( 0x1074 )                  /* alpha�˲�ϵ��ѡ�� */
#define HAL_GBBP_CPU_CH_ECT_OPT_ADDR                ( 0x1088 )                  /* �ŵ������Ż� */
#define HAL_GBBP_NB_MOD_TYPE_RPT_ADDR               ( 0x1098 )                  /* �ĸ�BURST����ä����־�ϱ� */
#define HAL_GBBP_GROSS_CARRIER_ENERGY1_ADDR         ( 0x109C )                  /* ��һ����һ��BURST���ǰ�ֲ�Cֵ�ϱ� */
#define HAL_GBBP_GROSS_INTERFERE_ENERGY1_ADDR       ( 0x10A0 )                  /* ��һ����һ��BURST���ǰ�ֲ�Iֵ�ϱ� */
#define HAL_GBBP_GROSS_CARRIER_ENERGY2_ADDR         ( 0x10A4 )                  /* �ڶ�����һ��BURST���ǰ�ֲ�Cֵ�ϱ� */
#define HAL_GBBP_GROSS_INTERFERE_ENERGY2_ADDR       ( 0x10A8 )                  /* �ڶ�����һ��BURST���ǰ�ֲ�Iֵ�ϱ� */
#define HAL_GBBP_GROSS_CARRIER_ENERGY3_ADDR         ( 0x10AC )                  /* ��������һ��BURST���ǰ�ֲ�Cֵ�ϱ� */
#define HAL_GBBP_GROSS_INTERFERE_ENERGY3_ADDR       ( 0x10B0 )                  /* ��������һ��BURST���ǰ�ֲ�Iֵ�ϱ� */
#define HAL_GBBP_GROSS_CARRIER_ENERGY4_ADDR         ( 0x10B4 )                  /* ���Ľ���һ��BURST���ǰ�ֲ�Cֵ�ϱ� */
#define HAL_GBBP_GROSS_INTERFERE_ENERGY4_ADDR       ( 0x10B8 )                  /* ���Ľ�BURST���ǰ�ֲ�Iֵ�ϱ� */
#define HAL_GBBP_MQ1_FENZI1_ADDR                    ( 0x10C0 )                  /* ��һ��BURST���ǰ���BURST��MQƽ���ķ���ֵ */
#define HAL_GBBP_MQ1_FENMU1_ADDR                    ( 0x10C4 )                  /* ��һ��BURST���ǰ���BURST��MQƽ���ķ�ĸֵ */
#define HAL_GBBP_MQ2_FENZI1_ADDR                    ( 0x10C8 )                  /* ��һ��BURST�������BURST��MQƽ���ķ���ֵ */
#define HAL_GBBP_MQ2_FENMU1_ADDR                    ( 0x10CC )                  /* ��һ��BURST�������BURST��MQƽ���ķ�ĸֵ */
#define HAL_GBBP_MQ1_FENZI2_ADDR                    ( 0x10D0 )                  /* �ڶ���BURST���ǰ���BURST��MQƽ���ķ���ֵ */
#define HAL_GBBP_MQ1_FENMU2_ADDR                    ( 0x10D4 )                  /* �ڶ���BURST���ǰ���BURST��MQƽ���ķ�ĸֵ */
#define HAL_GBBP_MQ2_FENZI2_ADDR                    ( 0x10D8 )                  /* �ڶ���BURST�������BURST��MQƽ���ķ���ֵ */
#define HAL_GBBP_MQ2_FENMU2_ADDR                    ( 0x10DC )                  /* �ڶ���BURST�������BURST��MQƽ���ķ�ĸֵ */
#define HAL_GBBP_MQ1_FENZI3_ADDR                    ( 0x10E0 )                  /* ������BURST���ǰ���BURST��MQƽ���ķ���ֵ */
#define HAL_GBBP_MQ1_FENMU3_ADDR                    ( 0x10E4 )                  /* ������BURST���ǰ���BURST��MQƽ���ķ�ĸֵ */
#define HAL_GBBP_MQ2_FENZI3_ADDR                    ( 0x10E8 )                  /* ������BURST�������BURST��MQƽ���ķ���ֵ */
#define HAL_GBBP_MQ2_FENMU3_ADDR                    ( 0x10EC )                  /* ������BURST�������BURST��MQƽ���ķ�ĸֵ */
#define HAL_GBBP_MQ1_FENZI4_ADDR                    ( 0x10F0 )                  /* ���Ľ�BURST���ǰ���BURST��MQƽ���ķ���ֵ */
#define HAL_GBBP_MQ1_FENMU4_ADDR                    ( 0x10F4 )                  /* ���Ľ�BURST���ǰ���BURST��MQƽ���ķ�ĸֵ */
#define HAL_GBBP_MQ2_FENZI4_ADDR                    ( 0x10F8 )                  /* ���Ľ�BURST�������BURST��MQƽ���ķ���ֵ */
#define HAL_GBBP_MQ2_FENMU4_ADDR                    ( 0x10FC )                  /* ���Ľ�BURST�������BURST��MQƽ���ķ�ĸֵ */
#define HAL_GBBP_MQ1_FENZI5_ADDR                    ( 0x530c )                  /* �����BURST���ǰ���BURST��MQƽ���ķ���ֵ */
#define HAL_GBBP_MQ1_FENMU5_ADDR                    ( 0x5310 )                  /* �����BURST���ǰ���BURST��MQƽ���ķ�ĸֵ */
#define HAL_GBBP_MQ2_FENZI5_ADDR                    ( 0x5314 )                  /* �����BURST�������BURST��MQƽ���ķ���ֵ */
#define HAL_GBBP_MQ2_FENMU5_ADDR                    ( 0x5318 )                  /* �����BURST�������BURST��MQƽ���ķ�ĸֵ */
#define HAL_GBBP_VALPP_CARRIER_ENERGY0_ADDR         ( 0x1100 )                  /* ���burst��Ӧ���ز�����ֵ */
#define HAL_GBBP_VALPP_INTERFERE_ENERGY0_ADDR       ( 0x1150 )                  /* ���burst��Ӧ�ĸ�������ֵ */
#define HAL_GBBP_VALPP_DEM_TYPE_TOA0                ( 0x1104 )                  /* ���burst VALPP���TOA��type��Ϣ */
#define HAL_GBBP_VALPP_CARRIER_ENERGY1_ADDR         ( 0x1108 )                  /* �ڶ���burst��Ӧ���ز�����ֵ */
#define HAL_GBBP_VALPP_INTERFERE_ENERGY1_ADDR       ( 0x1154 )                  /* �ڶ���burst��Ӧ�ĸ�������ֵ */
#define HAL_GBBP_VALPP_DEM_TYPE_TOA1_ADDR           ( 0x110c )                  /* �ڶ���burst VALPP���TOA��type��Ϣ */
#define HAL_GBBP_VALPP_CARRIER_ENERGY2_ADDR         ( 0x1110 )                  /* ������burst��Ӧ���ز�����ֵ */
#define HAL_GBBP_VALPP_INTERFERE_ENERGY2_ADDR       ( 0x1158 )                  /* ������burst��Ӧ�ĸ�������ֵ */
#define HAL_GBBP_VALPP_DEM_TYPE_TOA2_ADDR           ( 0x1114 )                  /* ������burst VALPP���TOA��type��Ϣ */
#define HAL_GBBP_VALPP_CARRIER_ENERGY3_ADDR         ( 0x1118 )                  /* ���Ľ�burst��Ӧ���ز�����ֵ */
#define HAL_GBBP_VALPP_INTERFERE_ENERGY3_ADDR       ( 0x115c )                  /* ���Ľ�burst��Ӧ�ĸ�������ֵ */
#define HAL_GBBP_VALPP_DEM_TYPE_TOA3_ADDR           ( 0x111c )                  /* ���Ľ�burst VALPP���TOA��type��Ϣ */
#define HAL_GBBP_VALPP_CARRIER_ENERGY4_ADDR         ( 0x532C )                  /* �����burst��Ӧ���ز�����ֵ */
#define HAL_GBBP_VALPP_INTERFERE_ENERGY4_ADDR       ( 0x5330 )                  /* �����burst��Ӧ�ĸ�������ֵ */
#define HAL_GBBP_VALPP_DEM_TYPE_TOA4_ADDR           ( 0x5298 )                  /* �����burst VALPP���TOA��type��Ϣ */
#define HAL_GBBP_CPU_DEM_WIDTH_CFG_ADDR             ( 0x1120 )                  /* ���λ������ */
#define HAL_GBBP_CPU_DB_TYPE_CFG_ADDR               ( 0x1128 )                  /* DB������� */
#define HAL_GBBP_CPU_DEM_BEP_MODE_EN_ADDR           ( 0x112c )                  /* ���BEP���� */
#define HAL_GBBP_CPU_VALPP_CARRY_CTRL_ADDR          ( 0x1130 )                  /* ���ݰ������� */
#define HAL_GBBP_CPU_VALPP_CARRY_START_ADDR         ( 0x1134 )                  /* ���ݰ���ʹ�� */
#define HAL_GBBP_CPU_SAIC_CTRL_ADDR                 ( 0x1138 )                  /* SAIC������� */
#define HAL_GBBP_MAX_DB_DATA_ENERGY_RPT_ADDR        ( 0x113c )                  /* DummuBurst��� DATA��������ϱ� */
#define HAL_GBBP_MAX_DB_ENERGY_RPT_ADDR             ( 0x1140 )                  /* DummuBurst��� DB��������ϱ� */
#define HAL_GBBP_ENG_WEIGHT_ADDR                    ( 0x1144 )                  /* �������Ȩ�������� */
#define HAL_GBBP_LS_DC_CPU_ADDR                     ( 0x1160 )                  /* ��ǰburstֱ��I/Q·�ϱ� */
#define HAL_GBBP_PATH_ENERGY_CPU_RPT1_ADDR          ( 0x5730 )                  /* 8PSK��1����ֵ�ϱ� */
#define HAL_GBBP_PATH_ENERGY_CPU_RPT2_ADDR          ( 0x5734 )                  /* 8PSK��2����ֵ�ϱ� */
#define HAL_GBBP_PATH_ENERGY_CPU_RPT3_ADDR          ( 0x5738 )                  /* 8PSK��3����ֵ�ϱ� */
#define HAL_GBBP_PATH_ENERGY_CPU_RPT4_ADDR          ( 0x573c )                  /* 8PSK��4����ֵ�ϱ� */
#define HAL_GBBP_PATH_ENERGY_CPU_RPT5_ADDR          ( 0x5740 )                  /* 8PSK��5����ֵ�ϱ� */
#define HAL_GBBP_PATH_ENERGY_CPU_RPT6_ADDR          ( 0x5744 )                  /* 8PSK��6����ֵ�ϱ� */
#define HAL_GBBP_PATH_ENERGY_CPU_RPT7_ADDR          ( 0x5748 )                  /* 8PSK��7����ֵ�ϱ� */
#define HAL_GBBP_CARRY_RAM_ADDR                     ( 0x5400 )                  /* 0x05400~0x0555b:SB,NB�����ֵ��CPU�ӿڵİ���ram��4��6bit�����ֵ������ռ��imi_rddata��5��0��13��8��21��16��29��24bitλ */
#define HAL_GBBP_CPU_JITTER_CORR_CFG_ADDR           ( 0x53b4 )                  /* JITTER�����㷨���� */
#define HAL_GBBP_CPU_TSC_SWITCH_EN_ADDR             ( 0x53c4 )                  /* ѵ�����������ϱ����� */
#define HAL_GBBP_TSC_RAM_ADDR                       ( 0x2800 )                  /* ѵ�����������ϱ�RAM */
#define HAL_GBBP_GACI_INDEX_CPU_ADDR                ( 0x58a0 )                  /* �����Ƶ��������ϱ�CPU���ֿ��ܵĸ���״̬ */

/*--------------------------------------------------------------------------------------------*
 * 1.12 �����ŵ�����ģ��
 *--------------------------------------------------------------------------------------------*/

/*************************************** GTC���� *********************************************/
#define HAL_GTC_DL_CB_INDEX_ADDR                    ( 7 )                       /* ����ҵ����� */
#define HAL_GTC_DEC_BURST_POS_ADDR                  ( 8 )                       /* �����������ֵ�洢RAM�е�λ�� */
#define HAL_GTC_DEC_TYPE1_ADDR                      ( 9 )                       /* �������1 */
#define HAL_GTC_DEC_TYPE2_ADDR                      ( 10 )                      /* �������2 */
#define HAL_GTC_HARQ_POS1_ADDR                      ( 11 )                      /* ��HARQ��������HARQ�ϲ��ĵ�һ������ʼλ�� */
#define HAL_GTC_HARQ_POS2_ADDR                      ( 12 )                      /* ��HARQ��������HARQ�ϲ��ĵڶ�������ʼλ�� */
#define HAL_GTC_HARQ1_CB_INDEX_ADDR                 ( 15 )                      /* ����HARQ�ϲ����������CB_INDEX */
#define HAL_GTC_DL_BURST_CYCLE_ADDR                 ( 18 )                      /* ����������BURSTѭ�����ڣ���������ʹ���ź� */
#define HAL_GTC_GSP_DEC_TEST_MODE_EN_ADDR           ( 0 )                       /* ����ģʽʹ�ܣ�����Ѱ��ģʽ���� */
#define HAL_GTC_GSP_DEC_TEST_AMR_FRM_ADDR           ( 1 )                       /* ���������ģʽ�����õ�AMR֡���� */
#define HAL_GTC_GSP_DEC_TEST_PS_FRM_ADDR            ( 2 )                       /* ���������ģʽ�����õ�PS֡���� */
#define HAL_GTC_DEC_START_ADDR                      ( 19 )                      /* ���������źţ�����ʱ϶�������ϱ����� */
#define HAL_GTC_DEC_BURST_MAP_INFO_ADDR             ( 8 )                       /* ��������8��BURST�Ƿ���Ҫ�����ֵ�������������ʹ������ */

/*************************************** CPU���� *********************************************/
#define HAL_GBBP_CPU_BLER_BER_CLR_ADDR              ( 0x1300 )                  /* BLER,BER���㣬ͳ��ģʽ */
#define HAL_GBBP_CPU_AMR_CB_INDEX_ADDR              ( 0x1304 )                  /* AMR����������ڶ�Ӧ��AMR����֡cb_index */
#define HAL_GBBP_CPU_AMR_ACTIVE_SET_NUM_ADDR        ( 0x1308 )                  /* AMR�����������Ч����֡���͸��� */
#define HAL_GBBP_CPU_AMR_CRC_PRO_NUM_CFG_ADDR       ( 0x53cc )                  /* AMR����bit�Ż��㷨�ж�SPEECH CRC�ı������� */
#define HAL_GBBP_CPU_P0_PRIOR_REG_CFG_ADDR          ( 0x53d0 )                  /* AMR����bit�Ż��㷨��00��Ӧ��������� */
#define HAL_GBBP_CPU_P1_PRIOR_REG_CFG_ADDR          ( 0x53d4 )                  /* AMR����bit�Ż��㷨��01��Ӧ��������� */
#define HAL_GBBP_CPU_P2_PRIOR_REG_CFG_ADDR          ( 0x53d8 )                  /* AMR����bit�Ż��㷨��10��Ӧ��������� */
#define HAL_GBBP_CPU_P3_PRIOR_REG_CFG_ADDR          ( 0x53dc )                  /* AMR����bit�Ż��㷨��11��Ӧ��������� */
#define HAL_GBBP_CMI_P01_PRIOR_PRE_RPT_ADDR         ( 0x5380 )                  /* CMI����bit0��bit1����������ϱ� */
#define HAL_GBBP_CMI_P23_PRIOR_PRE_RPT_ADDR         ( 0x5384 )                  /* CMI����bit2��bit3����������ϱ� */
#define HAL_GBBP_CMC_P01_PRIOR_PRE_ADDR             ( 0x5388 )                  /* CMC����bit0��bit1����������ϱ� */
#define HAL_GBBP_CMC_P23_PRIOR_PRE_ADDR             ( 0x538c )                  /* CMC����bit2��bit3����������ϱ� */
#define HAL_GBBP_CPU_P01_MAPPRE_CFG_ADDR            ( 0x5390 )                  /* CMC����CMI ����bit0��bit1��Ӧ����ʷ�������ֵ */
#define HAL_GBBP_CPU_P23_MAPPRE_CFG_ADDR            ( 0x5394 )                  /* CMC����CMI ����bit2��bit3��Ӧ����ʷ�������ֵ */
#define HAL_GBBP_CPU_CMC_CMI_LOAD_EN_ADDR           ( 0x5398 )                  /* CMC����CMI�����Ϣ  */
#define HAL_GBBP_AMR_INFO1_RPT_ADDR                 ( 0x53f8 )                  /* AMR֡�����ϱ� */
#define HAL_GBBP_AMR_INFO2_RPT_ADDR                 ( 0x53fc )                  /* AMR�о��ϱ� */
#define HAL_GBBP_CPU_AMR_CORR_COEFF_CFG_ADDR        ( 0x53e0 )                  /* AFS/AHS������޲��� */
#define HAL_GBBP_BLER_ERRR_RPT_ADDR                 ( 0x130c )                  /* �ۼ�BLER������ */
#define HAL_GBBP_BLER_TOTAL_RPT_ADDR                ( 0x1310 )                  /* �ۼ�BLER���� */
#define HAL_GBBP_BER_ERRR_RPT_ADDR                  ( 0x1314 )                  /* �ۼ�BER������ */
#define HAL_GBBP_BER_TOTAL_RPT_ADDR                 ( 0x1318 )                  /* �ۼ�BER���� */
#define HAL_GBBP_CODEC_PARA_REG1_ADDR               ( 0x1320 )                  /* �������1 */
#define HAL_GBBP_CODEC_PARA_REG2_ADDR               ( 0x1324 )                  /* �������2 */
#define HAL_GBBP_CODEC_PARA_REG3_ADDR               ( 0x1328 )                  /* �������3 */
#define HAL_GBBP_CODEC_PARA_REG4_ADDR               ( 0x132c )                  /* �������4 */
#define HAL_GBBP_CODEC_PARA_REG5_ADDR               ( 0x1330 )                  /* �������5 */
#define HAL_GBBP_CODEC_PARA_REG6_ADDR               ( 0x1334 )                  /* �������6 */
#define HAL_GBBP_DEC_STATE_RPT1_ADDR                ( 0x1338 )                  /* ����״̬��״̬1 */
#define HAL_GBBP_DEC_STATE_RPT2_ADDR                ( 0x133c )                  /* ����״̬��״̬2 */
#define HAL_GBBP_Q_BIT_RPT1_ADDR                    ( 0x1340 )                  /* Q bit����͵֡bit��ֵ1 */
#define HAL_GBBP_Q_BIT_RPT2_ADDR                    ( 0x1344 )                  /* Q bit����͵֡bit��ֵ2 */
#define HAL_GBBP_DEC_INT_TOTAL_ADDR                 ( 0x1348 )                  /* �����жϴ����ϱ� */
#define HAL_GBBP_USF_CORR_SOFTDATA1_RPT_ADDR        ( 0x1500 )                  /* USF��ֵ�ϱ�1 */
#define HAL_GBBP_USF_CORR_SOFTDATA2_RPT_ADDR        ( 0x1504 )                  /* USF��ֵ�ϱ�2 */
#define HAL_GBBP_USF_CORR_SOFTDATA3_RPT_ADDR        ( 0x1508 )                  /* USF��ֵ�ϱ�3 */
#define HAL_GBBP_USF_CORR_SOFTDATA4_RPT_ADDR        ( 0x150c )                  /* USF��ֵ�ϱ�4 */
#define HAL_GBBP_USF_CORR_SOFTDATA5_RPT_ADDR        ( 0x1510 )                  /* USF��ֵ�ϱ�5 */
#define HAL_GBBP_USF_CORR_SOFTDATA6_RPT_ADDR        ( 0x1514 )                  /* USF��ֵ�ϱ�6 */
#define HAL_GBBP_USF_CORR_SOFTDATA7_RPT_ADDR        ( 0x1518 )                  /* USF��ֵ�ϱ�7 */
#define HAL_GBBP_USF_CORR_SOFTDATA8_RPT_ADDR        ( 0x151c )                  /* USF��ֵ�ϱ�8 */
#define HAL_GBBP_USF_CORR_SOFTDATA9_RPT_ADDR        ( 0x1520 )                  /* USF��ֵ�ϱ�9 */
#define HAL_GBBP_BFI_RSQ_METRIC_RPT_ADDR            ( 0x134c )                  /* ����RSQ_METRIC��RSQ_METRIC��BFI�ϱ� */
#define HAL_GBBP_DEC_RPT_RAM_ADDR                   ( 0x2000 )                  /* ����RAM */
#define HAL_GBBP_NB_DEC_CTRL_RPT_ADDR               ( 0x2004 )                  /* bbp���������Ϣ�ϱ���ʼRAMλ�� */
#define HAL_GBBP_SB_DEC_RPT_ADDR                    ( 0x2398 )                  /* SB �����ϱ������ַ */
#define HAL_GBBP_CODEC_WR_CONFLICT_CNT_ADDR         ( 0x5718 )                  /* VALPP RAM���ڷ��ʳ�ͻ������ */
#define HAL_GBBP_HARQ_MODE_ADDR                     ( 0x5a00 )                  /*harq ����ģʽ����*/
#define HAL_GBBP_HARQ_INIT_EN_ADDR                  ( 0x5a04 )                  /*harq ����ģʽ�쳣��λ�ź�*/
#define HAL_GBBP_HARQ_WR_BASE_ADDR                  ( 0x5a08 )                  /*harq ����ģʽ����harq DDR д����*/
#define HAL_HARQ_RD_BASE_ADDR                       ( 0x5a0c )                  /*harq ����ģʽ����harq DDR ������*/
#define HAL_DDR_HARQ_CFG_ADDR                       ( 0x5a10 )                  /*harq ����ģʽ����harq DDR FIFO����*/

/*--------------------------------------------------------------------------------------------*
 * 1.13 �����ŵ�����ģ��
 *--------------------------------------------------------------------------------------------*/

/*************************************** GTC���� *********************************************/
#define HAL_GTC_UL_CB1_INDEX_ADDR                   ( 20 )                      /* �������ҵ�������� */
#define HAL_GTC_UL_CB_NUM_ADDR                      ( 23 )                      /* �������ʼ��־,FACCH͵֡��־ */
#define HAL_GTC_IC_PUNC_INF_ADDR                    ( 24 )                      /* ����bit0��bit1��Ϣ�������Ϣ */
#define HAL_GTC_RACH_BSIC_ADDR                      ( 25 )                      /* RACHҵ����BS����վ����BSIC���� */
#define HAL_GTC_COD_BURST_POS_ADDR                  ( 26 )                      /* ��ǰ�������ݵ�һ��BURST�ڱ���洢RAM */
#define HAL_GTC_GSP_UL_BURST_CYCLE_ADDR             ( 27 )                      /* ���б�����BURSTѭ�����ڣ�����ʹ���ź� */
#define HAL_GTC_GSP_UL_COD_START_ADDR               ( 28 )                      /* ���б��������ź� */
#define HAL_GTC_UL_MAP_PARA_ADDR                    ( 29 )                      /* ���б�����Ϣ */
#define HAL_GTC_MAP_BURST_POS_ADDR                  ( 30 )                      /* MAPλ�� */
#define HAL_GTC_MAP_START_ADDR                      ( 31 )                      /* MAP BURST�����ź� */

/*************************************** CPU���� *********************************************/
#define HAL_GBBP_UL_CODE_STATE_RPT_ADDR             ( 0x13B0 )                  /* ����״̬ */
#define HAL_GBBP_UL_CODE_RAM_RST_ADDR               ( 0x13B4 )                  /* ���뽻֯��洢RAM�����ź� */
#define HAL_GBBP_LOOPC_MODE_CFG_ADDR                ( 0x53f0 )                  /* LoopC����ģʽ */
#define HAL_GBBP_COD_CBINDEX_RPT_ADDR               ( 0x53f4 )                  /* ���б�����Ϣ */
#define HAL_GBBP_UL_CODE_CFG_RAM_ADDR               ( 0x3000 )                  /* CPU���õı������� */
#define HAL_GBBP_UL_MAP_CFG_RAM_ADDR                ( 0x5600 )                  /* CPU���õĵ������� */

/*--------------------------------------------------------------------------------------------*
 * 1.14 ���е���ģ��
 *--------------------------------------------------------------------------------------------*/

/*************************************** GTC���� *********************************************/
#define HAL_GTC_MOD_START_POS_ADDR                  ( 12 )                      /* ��������λ�� */
#define HAL_GTC_MOD_SYMBOL_LENGTH_ADDR              ( 13 )                      /* ���Ƴ��� */
#define HAL_GTC_GSP_MOD_TYPE_ADDR                   ( 14 )                      /* �������� */
#define HAL_GTC_GSP_MOD_CTRL_ADDR                   ( 15 )                      /* ���ƿ����ź� */
#define HAL_GTC_GSP_ULMOD_TEST_MOD_LOW_ADDR         ( 5 )                       /* ���Ʋ���ģʽ */
#define HAL_GTC_GSP_ULMOD_TEST_MOD_HIGH_ADDR        ( 6 )                       /* Ԥ���Ƴ��� */

/*************************************** CPU���� *********************************************/
#define HAL_GBBP_CPU_IQ_MOD_DTA_ADDR                ( 0x53c8 )                  /* ���з���У׼ʱǿ��I/Q·���� */
#define HAL_GBBP_TX_DATA_MULFACTOR_ADDR             ( 0x5374 )                  /* �������� */
#define HAL_GBBP_MOD_BIT0_RPT_ADDR                  ( 0x53a0 )                  /* GMSK����������0 */
#define HAL_GBBP_MOD_BIT1_RPT_ADDR                  ( 0x53a4 )                  /* GMSK����������1 */
#define HAL_GBBP_MOD_BIT2_RPT_ADDR                  ( 0x53a8 )                  /* GMSK����������2 */
#define HAL_GBBP_MOD_BIT3_RPT_ADDR                  ( 0x53ac )                  /* GMSK����������3 */
#define HAL_GBBP_MOD_BIT4_RPT_ADDR                  ( 0x53b0 )                  /* GMSK����������4 */
#define HAL_GBBP_TX_IQ_MISMATCH_COMP_ADDR           ( 0x58BC )                  /* TX iq MISMATCH */
#define HAL_GBBP_TX_IQ_DC_OFFSET_COMP_ADDR          ( 0x58C0 )                  /* TX DCOC */
#define HAL_GBBP_TX_DIG_RMAP_MODE_SEL_ADDR          ( 0x58C4 )                  /* ��������ѡ�� */

/*--------------------------------------------------------------------------------------------*
 * 1.15 DRXģ��
 *--------------------------------------------------------------------------------------------*/

/*************************************** CPU���� *********************************************/
#define HAL_GBBP_WAKE_TEST_QB_FRAC_RPT_ADDR         ( 0x1224 )                  /* DRX�����л�ʱGTC���� */
#define HAL_GBBP_WAKE_TEST_T1_T2_T3_RPT_ADDR        ( 0x1228 )                  /* DRX�����л�ʱGTC��ʱT1��T2��T3���� */
#define HAL_GBBP_WAKE_GTC_FN_RPT_ADDR               ( 0x122c )                  /* DRX����ʱ��GTC��FN���� */

#define HAL_GBBP_GAUGE_EN_CNF_ADDR                  ( 0x7000 )                  /* ʱ��У׼���ȣ�ʹ���ź� */
#define HAL_GBBP_GAUGE_RESULT_RPT_ADDR              ( 0x7004 )                  /* ʱ��У׼��� */


/* �����ʹ�õ�DRX���ֵ�ַ,OAM��ʹ�� */
#define HAL_GBBP_DRX_DSP_INT_STA_ADDR               (( GDRX_BASE_ADDR+0x10 )&0xFFFF)        /* GBBP DRX �ж�״̬�Ĵ��� */
#define HAL_GBBP_DRX_DSP_INT_MASK_ADDR              (( GDRX_BASE_ADDR+0x14 )&0xFFFF)        /* GBBP DRX �ж�����λ  */
#define HAL_GBBP_DRX_DSP_INT_CLR_ADDR               (( GDRX_BASE_ADDR+0x18 )&0xFFFF)        /* GBBP DRX �ж�����Ĵ��� */
#define HAL_GBBP_DRX_INT_DELAY_CLK_ADDR             (( GDRX_BASE_ADDR+0x1c )&0xFFFF)        /* GBBP DRX �ж��ӳ��ϱ����ʱ���������λΪimi_clk */
#define HAL_GBBP_DRX_PHY_INT_DELAY_REG_PD_ADDR      (( GDRX_BASE_ADDR+0x28 )&0xFFFF)        /* �ǵ�����ARM 104M�ж��ӳ��ϱ����ʱ���������λΪimi_clk */

#define HAL_GBBP_DRX_SLEEP_EN_ADDR                  (( GDRX_BASE_ADDR+0x30 )&0xFFFF)        /* ˯�������� */
#define HAL_GBBP_DRX_WAKE_CNF_ADDR                  (( GDRX_BASE_ADDR+0x34 )&0xFFFF)        /* ʱ���л�ʱ�� */
#define HAL_GBBP_DRX_WAKEUP_LEN_ADDR                (( GDRX_BASE_ADDR+0x38 )&0xFFFF)        /* ˯�ߵ��ϱ�����ʱ�� */
#define HAL_GBBP_DRX_MSR_CNF_ADDR                   (( GDRX_BASE_ADDR+0x3c )&0xFFFF)        /* ˯��ʱ�� */
#define HAL_GBBP_DRX_CPU_DRX_DEPTH_CFG_ADDR         (( GDRX_BASE_ADDR+0x40 )&0xFFFF)        /* ˯��ʱ����ȵ�����ֵ */
#define HAL_GBBP_DRX_WAKE_GTC_QB_FRAC_RPT_ADDR      (( GDRX_BASE_ADDR+0x50 )&0xFFFF)        /* DRX�����л�ʱ���ص�GTC���� */
#define HAL_GBBP_DRX_WAKE_GTC_T1_T2_T3_RPT_ADDR     (( GDRX_BASE_ADDR+0x54 )&0xFFFF)        /* DRX�����л�ʱ���ص�GTC��T1��T2��T3���� */
#define HAL_GBBP_DRX_WAKE_GTC_FN_RPT_ADDR           (( GDRX_BASE_ADDR+0x58 )&0xFFFF)        /* DRX����ʱ��GTC��FN���� */
#define HAL_GBBP_DRX_INT_MASK_STATE_RPT_ADDR        (( GDRX_BASE_ADDR+0x5c )&0xFFFF)        /* DRX�ж�״̬�ϱ� */


/************************************ OAMʹ��DRX��ַ *****************************************/

/* OAMʹ�õ�DRX���ֵ�ַ,����㲻ʹ�� */
#define HAL_GBBP_DRX_ARM_INT_STA_ADDR               ( 0x00 )                    /* GBBP �ж�״̬�Ĵ��� ARM */
#define HAL_GBBP_DRX_ARM_INT_MASK_ADDR              ( 0x04 )                    /* GBBP �ж�����λ ARM */
#define HAL_GBBP_DRX_ARM_INT_CLR_ADDR               ( 0x08 )                    /* GBBP �ж�����Ĵ��� ARM */
#define HAL_GBBP_DRX_ARM_WAKE_EN_ADDR               ( 0x30 )                    /* ˯�߻��� */



/*--------------------------------------------------------------------------------------------*
 * 1.16 �ӽ���ģ��
 *--------------------------------------------------------------------------------------------*/

/*************************************** GTC���� *********************************************/
#define HAL_GTC_A5_DECIPH_CFG_ADDR                  ( 5 )                       /* ���н������� */
#define HAL_GTC_A5_CIPH_CFG_ADDR                    ( 6 )                       /* ���м������� */

/*************************************** CPU���� *********************************************/
#define HAL_GBBP_CPU_A5KC_LOW_ADDR                  ( 0x1450 )                  /* A51/A52�����Kc��32bit */
#define HAL_GBBP_CPU_A5KC_HIGH_ADDR                 ( 0x1454 )                  /* A51/A52�����Kc��32bit */
#define HAL_GBBP_CPU_A5_LSB_SEQ_SEL_ADDR            ( 0x1458 )                  /* A53�ӽ����㷨bit���˳������ */
#define HAL_GBBP_DECIPH_CFG_INFO1_CPU_ADDR          ( 0x1470 )                  /* ����ʱ��������1 */
#define HAL_GBBP_DECIPH_CFG_INFO2_CPU_ADDR          ( 0x1474 )                  /* ����ʱ��������2 */
#define HAL_GBBP_CIPH_CFG_INFO1_CPU_ADDR            ( 0x1478 )                  /* ����ʱ��������1 */
#define HAL_GBBP_CIPH_CFG_INFO2_CPU_ADDR            ( 0x147C )                  /* ����ʱ��������2 */

/*--------------------------------------------------------------------------------------------*
 * 1.17 GAPCģ��
 *--------------------------------------------------------------------------------------------*/

/*************************************** GTC���� *********************************************/
#define HAL_GTC_PA_CTRL_CFG_ADDR                    ( 4 )                       /* APC�����ź� */
#define HAL_GTC_PA_LEN_LOW_CFG_ADDR                 ( 5 )                       /* PA���³���Low Byte */
#define HAL_GTC_PA_LEN_HIGH_CFG_ADDR                ( 6 )                       /* PA���³���High Byte */
#define HAL_GTC_PA_TARGET_LOW_CFG_ADDR              ( 7 )                       /* PA����Ŀ���ѹLow Byte */
#define HAL_GTC_PA_TARGET_HIGH_CFG_ADDR             ( 8 )                       /* PA����Ŀ���ѹHigh Byte */
#define HAL_GTC_PA_INIT_LOW_CFG_ADDR                ( 9 )                       /* PA���³�ʼ��ѹLow Byte */
#define HAL_GTC_PA_INIT_HIGH_CFG_ADDR               ( 10 )                      /* PA���³�ʼ��ѹHigh Byte */


/*************************************** CPU���� *********************************************/
#define HAL_GBBP_CPU_GAPC_CONFIG_RAM_ADDR           ( 0x2500 )                  /* APC����ϵ������RAM */
#define HAL_GBBP_CPU_APC_SWITCH_EN_ADDR             ( 0x53c0 )                  /* APC����ϵ��pp�������� */

/*--------------------------------------------------------------------------------------------*
 * 1.18 ˫ģ�л�
 *--------------------------------------------------------------------------------------------*/

/*************************************** GTC���� *********************************************/
#define HAL_GTC_G2W_CTRL_ADDR                       ( 23 )                      /* G����W���� */
#define HAL_GTC_W2G_CTRL_ADDR                       ( 24 )                      /* W����G�������� */

/*************************************** CPU���� *********************************************/
/* 2G�½���4G�����ڼ䣬����Occasion���ȵĵ�ַ */
#define GPHY_DRV_CPU_2G4_GAP_LENGTH_ADDR            ( 0x1248 )

/* 2G�½���4G�����ڼ䣬GDSP��ǰֹͣoccasion���źţ������źš�GBBP�յ����źź�����ֹͣgap_cnt���������ϱ�int2�������жϣ���������ͨ������ָʾ�ź�wg_intrasys_valid_delay�������� */
#define GPHY_DRV_CPU_2G4_GAP_STOP_ADDR              ( 0x124c )

/* 2G��4Gʱ, INT0��INT1������õĵ�ַ */
#define GPHY_DRV_CPU_2G4_DIST_ADDR                  ( 0x1250 )

/* 2G��4Gʱ, INT0�������õĵ�ַ */
#define GPHY_DRV_CPU_2G4_INT0_GEN_QB_ADDR           ( 0x1254 )

/*--------------------------------------------------------------------------------------------*
 * 1.19 ���ز���ģʽ
 *--------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------*
 * 1.20 ���Թܽſ���ģ��
 *--------------------------------------------------------------------------------------------*/

/*************************************** CPU���� *********************************************/
#define HAL_GBBP_TEST_PIN_SEL_LOW_ADDR              ( 0x5700 )                  /* ��������ѡ���ź� */
#define HAL_GBBP_TEST_PIN_SEL_HIGH_ADDR             ( 0x5704 )                  /* ��������ѡ���ź� */
#define HAL_GBBP_TEST_MODULE_SEL_ADDR               ( 0x5710 )                  /* ģ��������� */
#define HAL_GBBP_DRX_TEST_PIN_SEL_ADDR              ( 0x5858 )                  /* GDRX�������� */
#define HAL_GBBP_GSP_START_TDMA_QB_RPT_ADDR         ( 0x5714 )                  /* ����ֵ */


/*--------------------------------------------------------------------------------------------*
 * 1.21 ���ԼĴ���ģ��
 *--------------------------------------------------------------------------------------------*/

/*************************************** CPU���� *********************************************/
#define HAL_GBBP_DSP_DEDICATE_CTRL_REG_ADDR         ( 0x5720 )                  /* DSPר�üĴ���дʹ�ܿ����� */
#define HAL_GBBP_DSP_DEDICATE_REG8_ADDR             ( 0x574c )                  /* DSPר�üĴ���8 */

/*--------------------------------------------------------------------------------------------*
 * 1.22 GBBP�汾�Ĵ���
 *--------------------------------------------------------------------------------------------*/

/*************************************** CPU���� *********************************************/
#define HAL_GBBP_VERSION_ADDR                       ( 0x5760 )                  /* GBBP�汾 */
#define HAL_GBBP_ERR_TRIG_ADDR                      ( 0x5764 )                  /* �߼�������Trigger�ź� */
#define HAL_GBBP_EDGE_VERSION_ADDR                  ( 0x5768 )                  /* �汾�� */



#define HAL_GBBP_CPU_GDUMP_FIFO_RD_ADDR             ( 0xF000 )                  /* CPU��ȡGRIF FIFO����ƫ�Ƶ�ַ */
#define HAL_GBBP_CPU_GDUMP_GEN_CONFIG_ADDR          ( 0xF004 )                  /* FIFOˮ�����ѡ��,��������Դѡ��,����ģ����������ѡ��,����ģ����ʹ���Ƶ�ַ */
#define HAL_GBBP_CPU_GDUMP_LEN_CONFIG_ADDR          ( 0xF008 )                  /* CPU���ò������ݸ����Ƶ�ַ */
#define HAL_GBBP_CPU_GDUMP_SAM_LOW_TIMING_ADDR      ( 0xF00C )                  /* ��������ʱ�̲���ģʽ���Ƶ�ַ */
#define HAL_GBBP_CPU_GDUMP_SAM_HIGH_TIMING_ADDR     ( 0xF010 )                  /* ��������ʱ�̲���ģʽ�£�������ʼλ���Ƶ�ַ */
#define HAL_GBBP_CPU_GDUMP_RECV_CONFIG_ADDR         ( 0xF014 )                  /* ��������ʱ�̲���ģʽ�£�������ʼλ���Ƶ�ַ */




/*--------------------------------------------------------------------------------------------*
 * 1.23 OAM�������GBBP�Ĵ���
 *--------------------------------------------------------------------------------------------*/

/************************************ OAMʹ��GBBP��ַ ****************************************/

#define OAM_GBBP_FRAME_INT_BIT                      BIT_N(0)
#define OAM_GBBP_WAKE_UP_INT_BIT                    BIT_N(24)

#define OAM_GBBP_DRX_CLK_SWITCH_32K_INT_BIT         BIT_N(25)
#define OAM_GBBP_DRX_CLK_SWITCH_52M_INT_BIT         BIT_N(27)


/* GBBP �ļĴ�������,���OAM����˫ʵ��,��ֻ��Ҫʹ��һ�ݵ�ַ */
#define OAM_GBBP_ARM_INT_STA_ADDR                   (GBBP_BASE_ADDR+HAL_GBBP_ARM_INT_STA_ADDR)
#define OAM_GBBP_ARM_INT_MASK_ADDR                  (GBBP_BASE_ADDR+HAL_GBBP_ARM_INT_MASK_ADDR)
#define OAM_GBBP_ARM_INT_CLR_ADDR                   (GBBP_BASE_ADDR+HAL_GBBP_ARM_INT_CLR_ADDR)

#define OAM_GBBP_BLER_ERRR_RPT_ADDR                 (GBBP_BASE_ADDR+HAL_GBBP_BLER_ERRR_RPT_ADDR)
#define OAM_GBBP_BLER_TOTAL_RPT_ADDR                (GBBP_BASE_ADDR+HAL_GBBP_BLER_TOTAL_RPT_ADDR)

#define OAM_GBBP_GTC_FN_RPT_ADDR                    (GBBP_BASE_ADDR+HAL_GBBP_GTC_FN_RPT_ADDR)
#define OAM_GBBP_GTC_FN_HIGH_RPT_ADDR               (GBBP_BASE_ADDR+HAL_GBBP_GTC_FN_HIGH_RPT_ADDR)

#define OAM_GBBP_DRX_ARM_INT_STA_ADDR               (GDRX_BASE_ADDR+HAL_GBBP_DRX_ARM_INT_STA_ADDR)
#define OAM_GBBP_DRX_ARM_INT_MASK_ADDR              (GDRX_BASE_ADDR+HAL_GBBP_DRX_ARM_INT_MASK_ADDR)
#define OAM_GBBP_DRX_ARM_INT_CLR_ADDR               (GDRX_BASE_ADDR+HAL_GBBP_DRX_ARM_INT_CLR_ADDR)
#define OAM_GBBP_DRX_ARM_WAKE_EN_ADDR               (GDRX_BASE_ADDR+HAL_GBBP_DRX_ARM_WAKE_EN_ADDR)


/* GBBP �ļĴ�������,���OAM���벻��˫ʵ��,��OAM��V9������ʾʹ�����µ�ַ,V3���ᶨ�����µ�ַ */
#define OAM_GBBP1_ARM_INT_STA_ADDR                  (SOC_BBP_GSM1_BASE_ADDR+HAL_GBBP_ARM_INT_STA_ADDR)
#define OAM_GBBP1_ARM_INT_MASK_ADDR                 (SOC_BBP_GSM1_BASE_ADDR+HAL_GBBP_ARM_INT_MASK_ADDR)
#define OAM_GBBP1_ARM_INT_CLR_ADDR                  (SOC_BBP_GSM1_BASE_ADDR+HAL_GBBP_ARM_INT_CLR_ADDR)

#define OAM_GBBP1_BLER_ERRR_RPT_ADDR                (SOC_BBP_GSM1_BASE_ADDR+HAL_GBBP_BLER_ERRR_RPT_ADDR)
#define OAM_GBBP1_BLER_TOTAL_RPT_ADDR               (SOC_BBP_GSM1_BASE_ADDR+HAL_GBBP_BLER_TOTAL_RPT_ADDR)

#define OAM_GBBP1_GTC_FN_RPT_ADDR                   (SOC_BBP_GSM1_BASE_ADDR+HAL_GBBP_GTC_FN_RPT_ADDR)
#define OAM_GBBP1_GTC_FN_HIGH_RPT_ADDR              (SOC_BBP_GSM1_BASE_ADDR+HAL_GBBP_GTC_FN_HIGH_RPT_ADDR)

#define OAM_GBBP1_DRX_ARM_INT_STA_ADDR              (SOC_BBP_GSM1_ON_BASE_ADDR+HAL_GBBP_DRX_ARM_INT_STA_ADDR)
#define OAM_GBBP1_DRX_ARM_INT_MASK_ADDR             (SOC_BBP_GSM1_ON_BASE_ADDR+HAL_GBBP_DRX_ARM_INT_MASK_ADDR)
#define OAM_GBBP1_DRX_ARM_INT_CLR_ADDR              (SOC_BBP_GSM1_ON_BASE_ADDR+HAL_GBBP_DRX_ARM_INT_CLR_ADDR)
#define OAM_GBBP1_DRX_ARM_WAKE_EN_ADDR              (SOC_BBP_GSM1_ON_BASE_ADDR+HAL_GBBP_DRX_ARM_WAKE_EN_ADDR)


/* �������� */
#define OAM_GBBP_CPU_GDUMP_FIFO_RD_ADDR             (GBBP_BASE_ADDR+HAL_GBBP_CPU_GDUMP_FIFO_RD_ADDR)         /* CPU��ȡGRIF FIFO���� */
#define OAM_GBBP_CPU_GDUMP_GEN_CONFIG_ADDR          (GBBP_BASE_ADDR+HAL_GBBP_CPU_GDUMP_GEN_CONFIG_ADDR)      /* FIFOˮ�����ѡ��,��������Դѡ��,����ģ����������ѡ��,����ģ����ʹ�� */
#define OAM_GBBP_CPU_GDUMP_LEN_CONFIG_ADDR          (GBBP_BASE_ADDR+HAL_GBBP_CPU_GDUMP_LEN_CONFIG_ADDR)      /* CPU���ò������ݸ�����ȫ0��ʾ�������ݸ������޳� */
#define OAM_GBBP_CPU_GDUMP_SAM_LOW_TIMING_ADDR      (GBBP_BASE_ADDR+HAL_GBBP_CPU_GDUMP_SAM_LOW_TIMING_ADDR)  /* ��������ʱ�̲���ģʽ�£�������ʼλ�ã�35'h7ffffffff��ʾ����������������Ϊ��35bit��[12:0]��Ĭ��ֵΪ��������ģʽ */
#define OAM_GBBP_CPU_GDUMP_SAM_HIGH_TIMING_ADDR     (GBBP_BASE_ADDR+HAL_GBBP_CPU_GDUMP_SAM_HIGH_TIMING_ADDR) /* ��������ʱ�̲���ģʽ�£�������ʼλ�� */
#define OAM_GBBP_CPU_GDUMP_RECV_CONFIG_ADDR         (GBBP_BASE_ADDR+HAL_GBBP_CPU_GDUMP_RECV_CONFIG_ADDR)     /* CPU���ò������������ź� */

#define OAM_GBBP0_GAUGE_RESULT_RPT_ADDR             (GBBP_BASE_ADDR+HAL_GBBP_GAUGE_RESULT_RPT_ADDR)
#define OAM_GBBP1_GAUGE_RESULT_RPT_ADDR             (SOC_BBP_GSM1_BASE_ADDR+HAL_GBBP_GAUGE_RESULT_RPT_ADDR)




/*****************************************************************************
  3 ö�ٶ���
******************************************************************************/

/*****************************************************************************
 ö����    :
 Э����  :
 ASN.1���� :
 ö��˵��  :
*****************************************************************************/


/*****************************************************************************
 ö����    :
 Э����  :
 ASN.1���� :
 ö��˵��  :
*****************************************************************************/










/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/
/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/
/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
/*****************************************************************************
 �ṹ��    :
 Э����  :
 �ṹ˵��  :
*****************************************************************************/
/*****************************************************************************
 �ṹ��    :
 Э����  :
 �ṹ˵��  :
*****************************************************************************/

/*****************************************************************************
  7 UNION����
*****************************************************************************/







/*--------------------------------------------------------------------------------------------*
 * GBBP�Ĵ�������
 *--------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------*
 * 1.1  ǰ��
 *--------------------------------------------------------------------------------------------*/
/*****************************************************************************
 ������    : HAL_GTC_DATA_TRANSFER_INSTRUCTION_UNION
 ����˵��  : GTC����ָ���ʽ,���ݴ���ָ��
*****************************************************************************/

typedef union
{
    unsigned short                          uhwInstructionValue;                    /* ָ���� */
    struct
    {
        unsigned short                      uhwDestAddress      : 5;                /* ָ���ַ */
        unsigned short                      uhwDataOperand      : 8;                /* ָ������ */
        unsigned short                      uhwCode             : 3;                /* ָ���� */
    }stReg;
}HAL_GTC_DATA_TRANSFER_INSTRUCTION_UNION;


/*****************************************************************************
 ������    : HAL_GTC_TIME_SCHEDULE_INSTRUCTION_UNION
 ����˵��  : GTCʱ��ָ���ʽ,ʱ�����ָ��
*****************************************************************************/
typedef union
{
    unsigned short                          uhwInstructionValue;                    /* ָ���� */
    struct
    {
        unsigned short                      uhwAbsoluteTime     : 13;               /* ����ʱ�䣬��λΪQB */
        unsigned short                      uhwCode             : 3;                /* ָ���� */
    }stReg;
}HAL_GTC_TIME_SCHEDULE_INSTRUCTION_UNION;





/*--------------------------------------------------------------------------------------------*
 * 1.2  GBBP�жϿ���ģ��
 *--------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------*
 * GTC����
 *--------------------------------------------------------------------------------------------*/

/*****************************************************************************
 ������    : HAL_GTC_CPU_GRM_INT_UNION
 ����˵��  : CPU_GRM_INT�Ĵ���
*****************************************************************************/
typedef union
{
    unsigned short                          uhwValue;
    struct
    {
        unsigned short                      gsp_cpu_grm_it      : 8;                /* bit7~bit0����ʾ8��CPU�ɱ���ж� */
        unsigned short                      reserved            : 8;                /* reserve */
    }stReg;
}HAL_GTC_CPU_GRM_INT_UNION;


/*****************************************************************************
 ������    : HAL_GTC_SLOTINT_OFFSET_L_UNION
 ����˵��  : SLOTINT_OFFSET_L�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short                          uhwValue;
    struct
    {
        unsigned short                      gsp_slot_int_offset_high    : 8;        /* bit7~0��8��ʱ϶ͷ�ж�ƫ��λ�ã�һ��ʱ϶�ڣ���8bit���̶�����ƫ�ƣ���λΪqb��1/4 symbol�� */
        unsigned short                      reserved                    : 8;        /* reserve */
    }stReg;
}HAL_GTC_SLOTINT_OFFSET_L_UNION;


/*****************************************************************************
 ������    : HAL_GTC_SLOTINT_OFFSET_H_UNION
 ����˵��  : SLOTINT_OFFSET_H�Ĵ�������
*****************************************************************************/

typedef union
{
    unsigned short                          uhwValue;
    struct
    {
        unsigned short                      gsp_slot_int_offset_low     : 2;        /* bit1~0��8��ʱ϶ͷ�ж�ƫ��λ�ã�һ��ʱ϶�ڣ���2bit���̶�����ƫ�ƣ���λΪqb��1/4 symbol�� */
        unsigned short                      reserved                    : 14;       /* reserve */
    }stReg;
}HAL_GTC_SLOTINT_OFFSET_H_UNION;



/*--------------------------------------------------------------------------------------------*
 * CPU����
 *--------------------------------------------------------------------------------------------*/
/*****************************************************************************
 ������    : HAL_GBBP_DSP_INT_CLR_UNION
 ����˵��  : DSP_INT_CLR�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long                          uwValue;
    struct
    {
        unsigned long                      dsp_int00_clr       : 1;                /* TDMA֡�ж������źţ�1��ʾ�ж����������0��ʾ���жϲ���� */
        unsigned long                      dsp_int01_clr       : 1;                /* GTC PAGE�ж�����Ĵ��� */
        unsigned long                      dsp_int02_clr       : 1;                /* ͬ�������ж������ź� */
        unsigned long                      dsp_int03_clr       : 1;                /* ���������ϱ��ж������ź� */
        unsigned long                      dsp_int04_clr       : 1;                /* ʱ϶0�ж������źţ�1��Ч */
        unsigned long                      dsp_int05_clr       : 1;                /* ʱ϶1�ж������ź� */
        unsigned long                      dsp_int06_clr       : 1;                /* ʱ϶2�ж������ź� */
        unsigned long                      dsp_int07_clr       : 1;                /* ʱ϶3�ж������ź� */
        unsigned long                      dsp_int08_clr       : 1;                /* ʱ϶4�ж������ź� */
        unsigned long                      dsp_int09_clr       : 1;                /* ʱ϶5�ж������ź� */
        unsigned long                      dsp_int10_clr       : 1;                /* ʱ϶6�ж������ź� */
        unsigned long                      dsp_int11_clr       : 1;                /* ʱ϶7�ж������ź� */
        unsigned long                      dsp_int12_clr       : 1;                /* �ɱ���ж�0�����ź� */
        unsigned long                      dsp_int13_clr       : 1;                /* �ɱ���ж�1�����ź� */
        unsigned long                      dsp_int14_clr       : 1;                /* �ɱ���ж�2�����ź� */
        unsigned long                      dsp_int15_clr       : 1;                /* �ɱ���ж�3�����ź� */
        unsigned long                      dsp_int16_clr       : 1;                /* �ɱ���ж�4�����ź� */
        unsigned long                      dsp_int17_clr       : 1;                /* �ɱ���ж�5�����ź� */
        unsigned long                      dsp_int18_clr       : 1;                /* �ɱ���ж�6�����ź� */
        unsigned long                      dsp_int19_clr       : 1;                /* �ɱ���ж�7�����ź� */
        unsigned long                      dsp_int20_clr       : 1;                /* RFICģ��DCR�ж������ź� */
        unsigned long                      reserved            : 7;                /* reserved */
        unsigned long                      dsp_int28_clr       : 1;                /* ��ƽ���������ж������ź� */
        unsigned long                      dsp_int29_clr       : 1;                /* ��������ж������ź� */
        unsigned long                      dsp_int30_clr       : 1;                /* ABB��GRIF FIFO��д����������ж������ź� */
        unsigned long                      dsp_int31_clr       : 1;                /* CPU��GRIF FIFO�Ķ��ղ����ж������ź� */
    }stReg;
}HAL_GBBP_DSP_INT_CLR_UNION;



/*--------------------------------------------------------------------------------------------*
 * 1.3  ��Ƶ����ģ��
 *--------------------------------------------------------------------------------------------*/

/*****************************************************************************
 ������    : HAL_GTC_SPI_RD_START_ADDR_UNION
 ����˵��  : SPI_RD_START_ADDR�Ĵ�������
*****************************************************************************/

typedef union
{
    unsigned short                          uhwValue;
    struct
    {
        unsigned short                      gtc_spi_rd_start_addr   : 8;            /* bit7~0��GTC SPI���ڴ�����������SPI DATA RAM�е���ʼλ��. */
        unsigned short                      reserved                : 8;            /* reserve */
    }stReg;
}HAL_GTC_SPI_RD_START_ADDR_UNION;


/*****************************************************************************
 ������    : HAL_GTC_SPI_TRANS_DATA_NUM_UNION
 ����˵��  : SPI_TRANS_DATA_NUM �Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short                          uhwValue;
    struct
    {
        unsigned short                      gtc_spi_trans_data_num  : 5;            /* bit4~bit0��������Ҫ���͵�SPI���������� */
        unsigned short                      reserved                : 11;           /* reserve */
    }stReg;
}HAL_GTC_SPI_TRANS_DATA_NUM_UNION;


/*****************************************************************************
 ������    : HAL_GTC_SPI_RD_EN_UNION
 ����˵��  : SPI_RD_EN �Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short                          uhwValue;
    struct
    {
        unsigned short                      gtc_spi_rd_en       : 1;                /* GTC��������SPI�������� */
        unsigned short                      reserved            : 15;               /* reserve */
    }stReg;
}HAL_GTC_SPI_RD_EN_UNION;



/*****************************************************************************
 ������    : HAL_GTC_GSP_LINE1_CTRL_BITWISE_CLR_UNION
 ����˵��  : GSP_LINE1_CTRL_BITWISE_CLR�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_line1_ctrl_bitwise_clr              : 8;      /* ��һ���߿�������ơ��Ա���λд1������Ч����ʹ�ܡ�д0�����塣��Ҫ��λ���뿴GTC�Ĵ�����ַ26�� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_GSP_LINE1_CTRL_BITWISE_CLR_UNION;


/*****************************************************************************
 ������    : HAL_GTC_GSP_LINE1_CTRL_BITWISE_SET_UNION
 ����˵��  : GSP_LINE1_CTRL_BITWISE_SET�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_line1_ctrl_bitwise_set              : 8;      /* ��һ���߿���λ���ơ��Ա���λд1������Ч����ʹ�ܡ�д0�����塣��Ҫ���㣬�뿴GTC�Ĵ�����ַ25�� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_GSP_LINE1_CTRL_BITWISE_SET_UNION;



/*****************************************************************************
 ������    : HAL_GTC_GSP_LINE2_CTRL_BITWISE_CLR_UNION
 ����˵��  : GSP_LINE2_CTRL_BITWISE_CLR�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_line2_ctrl_bitwise_clr              : 8;      /* ��һ���߿�������ơ��Ա���λд1������Ч����ʹ�ܡ�д0�����塣��Ҫ��λ���뿴GTC�Ĵ�����ַ26�� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_GSP_LINE2_CTRL_BITWISE_CLR_UNION;


/*****************************************************************************
 ������    : HAL_GTC_GSP_LINE2_CTRL_BITWISE_SET_UNION
 ����˵��  : GSP_LINE2_CTRL_BITWISE_SET�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_line2_ctrl_bitwise_set              : 8;      /* ��һ���߿���λ���ơ��Ա���λд1������Ч����ʹ�ܡ�д0�����塣��Ҫ���㣬�뿴GTC�Ĵ�����ַ25�� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_GSP_LINE2_CTRL_BITWISE_SET_UNION;




/*****************************************************************************
 ������    : HAL_GTC_GSP_LINE3_CTRL_BITWISE_CLR_UNION
 ����˵��  : GSP_LINE3_CTRL_BITWISE_CLR�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_line3_ctrl_bitwise_clr              : 8;      /* ��һ���߿�������ơ��Ա���λд1������Ч����ʹ�ܡ�д0�����塣��Ҫ��λ���뿴GTC�Ĵ�����ַ26�� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_GSP_LINE3_CTRL_BITWISE_CLR_UNION;


/*****************************************************************************
 ������    : HAL_GTC_GSP_LINE3_CTRL_BITWISE_SET_UNION
 ����˵��  : GSP_LINE3_CTRL_BITWISE_SET�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_line3_ctrl_bitwise_set              : 8;      /* ��һ���߿���λ���ơ��Ա���λд1������Ч����ʹ�ܡ�д0�����塣��Ҫ���㣬�뿴GTC�Ĵ�����ַ25�� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_GSP_LINE3_CTRL_BITWISE_SET_UNION;



/*****************************************************************************
 ������    : HAL_GTC_GSP_LINE4_CTRL_BITWISE_CLR_UNION
 ����˵��  : GSP_LINE4_CTRL_BITWISE_CLR�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_line4_ctrl_bitwise_clr              : 4;      /* ��һ���߿�������ơ��Ա���λд1������Ч����ʹ�ܡ�д0�����塣��Ҫ��λ���뿴GTC�Ĵ�����ַ26�� */
        unsigned short                                         : 12;     /* reserve */
    }stReg;
}HAL_GTC_GSP_LINE4_CTRL_BITWISE_CLR_UNION;


/*****************************************************************************
 ������    : HAL_GTC_GSP_LINE4_CTRL_BITWISE_SET_UNION
 ����˵��  : GSP_LINE4_CTRL_BITWISE_SET�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_line4_ctrl_bitwise_set              : 4;      /* ��һ���߿���λ���ơ��Ա���λд1������Ч����ʹ�ܡ�д0�����塣��Ҫ���㣬�뿴GTC�Ĵ�����ַ25�� */
        unsigned short                                         : 12;     /* reserve */
    }stReg;
}HAL_GTC_GSP_LINE4_CTRL_BITWISE_SET_UNION;




/*****************************************************************************
 ������    : HAL_GTC_GSP_EXT_LINE_CTRL_BITWISE_CLR_UNION
 ����˵��  : GSP_EXT_LINE_CTRL_BITWISE_CLR�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_ext_line_ctrl_bitwise_clr           : 8;      /* ��һ���߿�������ơ��Ա���λд1������Ч����ʹ�ܡ�д0�����塣��Ҫ��λ���뿴GTC�Ĵ�����ַ26�� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_GSP_EXT_LINE_CTRL_BITWISE_CLR_UNION;


/*****************************************************************************
 ������    : HAL_GTC_GSP_EXT_LINE_CTRL_BITWISE_SET_UNION
 ����˵��  : GSP_EXT_LINE_CTRL_BITWISE_SET�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_ext_line_ctrl_bitwise_set           : 8;      /* ��һ���߿���λ���ơ��Ա���λд1������Ч����ʹ�ܡ�д0�����塣��Ҫ���㣬�뿴GTC�Ĵ�����ַ25�� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_GSP_EXT_LINE_CTRL_BITWISE_SET_UNION;



/*****************************************************************************
 ������    : HAL_GTC_GSP_MIPI_START_INI_UNION
 ����˵��  : GSP_MIPI_START_INI�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_mipi_start                          : 1;      /* GSM����MIPI����д���������ź� */
        unsigned short gsp_mipi_init_addr                      : 6;      /* GSM����MIPI����д����ʱ����Ӧ�����üĴ�������ʼ��ַ */
        unsigned short                                         : 9;      /* reserve */
    }stReg;
}HAL_GTC_GSP_MIPI_START_INI_UNION;

/*****************************************************************************
 ������    : HAL_GTC_GSP_MIPI_MUM_UNION
 ����˵��  : GSP_MIPI_MUM�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_mipi_num                            : 6;      /* GSM����MIPI����д����ʱ�����β�����Ӧ�����ø��� */
        unsigned short                                         : 10;     /* reserve */
    }stReg;
}HAL_GTC_GSP_MIPI_MUM_UNION;

/*****************************************************************************
 ������    : HAL_GTC_GSP_LINE2_CTRL_UNION
 ����˵��  : GSP_LINE2_CTRL�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short ant_value1                              : 4;      /* �ڶ����߿��źš� */
        unsigned short saw_sel                                 : 1;
        unsigned short pa_band                                 : 1;
        unsigned short                                         : 10;      /* reserve */
    }stReg;
}HAL_GTC_GSP_LINE2_CTRL_UNION;

/*****************************************************************************
 ������    : HAL_GTC_GSP_LINE3_CTRL_UNION
 ����˵��  : GSP_LINE2_CTRL�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_line3_ctrl_part1                    : 6;      /* ��Ӧ�ڵ������߿��ź�gtc_gsp_line_ctrl[23:16] bit5~bit0����Ӧ�����߿��صĸ�6bit*/
        unsigned short gsp_line3_ctrl_part2                    : 2;
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_GSP_LINE3_CTRL_UNION;



/*--------------------------------------------------------------------------------------------*
 * CPU����
 *--------------------------------------------------------------------------------------------*/

/*****************************************************************************
 ������    : HAL_GBBP_CPU_LINE_CTRL_ADDR_UNION
 ����˵��  : HAL_GBBP_CPU_LINE_CTRL_ADDR�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_gtc_line_ctrl                      : 28;    /* bit0��ABB���й���������λʹ�ܣ�Ĭ��ֵ0 */
        unsigned long                                         : 3;     /* */
        unsigned long cpu_gtc_line_ctrl_wr_en                 : 1;     /* 16bit�߿��ź�дʹ��ָʾ�ź� */
    }stReg;
}HAL_GBBP_CPU_LINE_CTRL_ADDR_UNION;

/*****************************************************************************
 ������    : HAL_GBBP_GTC_GSP_LINE_STATE_RPT_ADDR_UNION
 ����˵��  : HAL_GBBP_GTC_GSP_LINE_STATE_RPT_ADDR�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long gtc_gsp_line_ctrl                       : 28;    /* bit0��ABB���й���������λʹ�ܣ�Ĭ��ֵ0 */
        unsigned long                                         : 4;     /* */
    }stReg;
}HAL_GBBP_GTC_GSP_LINE_STATE_RPT_ADDR_UNION;



/*****************************************************************************
 ������    : HAL_GBBP_CPU_EXT_LINE_CTRL_ADDR_UNION
 ����˵��  : HAL_GBBP_CPU_EXT_LINE_CTRL_ADDR�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_rf_trcv_on                          : 1;     /* bit0��bit0��Ӧrf_trcv_on������ƵоƬ����ʹ���ź� */
        unsigned long cpu_rf_tx_en                            : 1;     /* bit1��bit1��Ӧrf_tx_en������Ƶ���з��͹���ʹ���ź� */
        unsigned long cpu_rf_rst_en                           : 1;     /* bit2��bit2��Ӧrf_rst_n������Ƶ��λ�����ź� */
        unsigned long cpu_abb_rx_a_en                         : 1;     /* bit3��bit3��Ӧabb_rxa_en��ABB RX Aͨ��ʹ���ź� */
        unsigned long cpu_abb_rx_b_en                         : 1;     /* bit4: bit4��Ӧabb_rxb_en��RX Bͨ��ʹ���ź� */
        unsigned long cpu_abb_tx_en                           : 1;     /* bit5��bit5��Ӧabb_tx_en��ABB TXͨ��ʹ���ź� */
        unsigned long cpu_gapc_en                             : 1;     /* bit6��bit6��Ӧgapc_en��Auxdacͨ��ʹ���ź� */
        unsigned long                                         : 1;     /* bit7�� */
        unsigned long                                         : 23;    /* Rsv */
        unsigned long cpu_ext_line_ctrl_wr_en                 : 1;     /* 8bit�����߿��ź�CPUдʹ��ָʾ�ź� */
    }stReg;
}HAL_GBBP_CPU_EXT_LINE_CTRL_ADDR_UNION;



/*****************************************************************************
 ������    : HAL_GBBP_GTC_GSP_EXT_LINE_STATE_RPT_ADDR_UNION
 ����˵��  : HAL_GBBP_GTC_GSP_EXT_LINE_STATE_RPT_ADDR�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long gtc_gsp_ext_line_ctrl                   : 8;     /* bit5��RF���н�����λʹ���źţ�SiGe����GDSP�������rf_tcvr_on�߿أ�������RFCMOS����*/
        unsigned long                                         : 24;    /* bit4��PA����ģʽѡ�񣬵���λʹ����Ч��PA����ģʽΪ8PSK��Ĭ��ֵ0 */
    }stReg;
}HAL_GBBP_GTC_GSP_EXT_LINE_STATE_RPT_ADDR_UNION;



/*****************************************************************************
 ������    : HAL_GBBP_CPU_LINE01_SEL_CFG_ADDR_UNION
 ����˵��  : HAL_GBBP_CPU_LINE01_SEL_CFG_ADDR�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_Rfic_ch_sel                         : 1;     /* bit0��bit0��ӦRFIC0��RFIC1ͨ������ѡ�� */
        unsigned long cpu_abb_ch_sel                          : 1;     /* bit1��bit1��ӦABB CH0��ABB CH1ͨ������ѡ�� */
        unsigned long cpu_auxdac_ch_sel                       : 1;     /* bit2��bit2��ӦAUXDAC0��AUXDAC1����ѡ�� */
        unsigned long cpu_mipi_ch_sel                         : 1;     /* bit3��bit3��ӦMIPI0��MIPI1ͨ������ѡ�� */
        unsigned long                                         : 27;    /* Rsv */
        unsigned long cpu_line01_sel_ctrl_wr_en               : 1;     /* 4bit��ͨ����������ѡ���ź�CPUдʹ��ָʾ�ź� */
    }stReg;
}HAL_GBBP_CPU_LINE01_SEL_CFG_ADDR_UNION;



/*****************************************************************************
 ������    : HAL_GBBP_CPU_LINE01_SEL_STATE_RPT_ADDR_UNION
 ����˵��  : HAL_GBBP_CPU_LINE01_SEL_STATE_RPT_ADDR�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long gtc_line01_sel_ctrl                     : 4;     /* bit5��RF���н�����λʹ���źţ�SiGe����GDSP�������rf_tcvr_on�߿أ�������RFCMOS����*/
        unsigned long                                         : 28;    /* bit4��PA����ģʽѡ�񣬵���λʹ����Ч��PA����ģʽΪ8PSK��Ĭ��ֵ0 */
    }stReg;
}HAL_GBBP_CPU_LINE01_SEL_STATE_RPT_ADDR_UNION;




/*****************************************************************************
 ������    : HAL_GBBP_CPU_SPI_RF_CFG_UNION
 ����˵��  : CPU_SPI_RF_CFG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_spi_rd_start_addr                   : 8;      /* bit7~0��CPU�ӿ�����SPI���ڴ�����������SPI DATA RAM�е���ʼλ��. */
        unsigned long cpu_spi_trans_data_num                  : 5;      /* bit12~bit8��CPU�ӿ����ñ�����Ҫ���͵�SPI���������� */
        unsigned long                                         : 18;     /* reserve */
        unsigned long spi_gtc_sel                             : 1;      /* bit[31]: 1��ʾSPI��Ƶ���Ʋ�����GTCģ�������0��ʾSPI��Ƶ���Ʋ�����CPU�ӿ�ģ�������Ĭ��ֵΪ1;���ź�ֻ������������TA�ϴ�ʱ����RF��Ƶ��ʱ���⣻��������¸��źŶ�Ӧ������Ϊ1 */
    }stReg;
}HAL_GBBP_CPU_SPI_RF_CFG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_CH_PARA_1REG_UNION
 ����˵��  : ADDR_G_CH_PARA_1REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long g_ch_h0_ext                             : 12;     /* GSM�����ŵ��˲�������ϵ��0��12bit�з������� ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
        unsigned long g_ch_h1_ext                             : 12;     /* GSM�����ŵ��˲�������ϵ��1��12bit�з������� ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_CH_PARA_1REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_CH_PARA_2REG_UNION
 ����˵��  : ADDR_G_CH_PARA_2REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long g_ch_h2_ext                             : 12;     /* GSM�����ŵ��˲�������ϵ��2��12bit�з������� ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
        unsigned long g_ch_h3_ext                             : 12;     /* GSM�����ŵ��˲�������ϵ��3��12bit�з������� ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_CH_PARA_2REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_CH_PARA_3REG_UNION
 ����˵��  : ADDR_G_CH_PARA_3REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long g_ch_h4_ext                             : 12;     /* GSM�����ŵ��˲�������ϵ��4��12bit�з������� ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
        unsigned long g_ch_h5_ext                             : 12;     /* GSM�����ŵ��˲�������ϵ��5��12bit�з������� ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_CH_PARA_3REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_CH_PARA_4REG_UNION
 ����˵��  : ADDR_G_CH_PARA_4REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long g_ch_h6_ext                             : 12;     /* GSM�����ŵ��˲�������ϵ��6��12bit�з������� ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
        unsigned long g_ch_h7_ext                             : 12;     /* GSM�����ŵ��˲�������ϵ��7��12bit�з������� ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_CH_PARA_4REG_UNION;

/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_CH_PARA_5REG_UNION
 ����˵��  : ADDR_G_CH_PARA_5REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long g_ch_h8_ext                             : 12;     /* GSM�����ŵ��˲�������ϵ��8��12bit�з������� ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
        unsigned long g_ch_h9_ext                             : 12;     /* GSM�����ŵ��˲�������ϵ��9��12bit�з������� ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_CH_PARA_5REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_CH_PARA_6REG_UNION
 ����˵��  : ADDR_G_CH_PARA_6REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long g_ch_h10_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��10��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
        unsigned long g_ch_h11_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��11��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_CH_PARA_6REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_CH_PARA_7REG_UNION
 ����˵��  : ADDR_G_CH_PARA_7REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long g_ch_h12_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��12��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
        unsigned long g_ch_h13_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��13��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_CH_PARA_7REG_UNION;

/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_CH_PARA_8REG_UNION
 ����˵��  : ADDR_G_CH_PARA_8REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long g_ch_h14_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��14��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
        unsigned long g_ch_h15_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��15��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_CH_PARA_8REG_UNION;

/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_CH_PARA_9REG_UNION
 ����˵��  : ADDR_G_CH_PARA_9REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long g_ch_h16_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��16��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
        unsigned long g_ch_h17_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��17��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_CH_PARA_9REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_CH_PARA_10REG_UNION
 ����˵��  : ADDR_G_CH_PARA_10REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long g_ch_h18_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��18��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
        unsigned long g_ch_h19_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��19��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_CH_PARA_10REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_CH_PARA_11REG_UNION
 ����˵��  : ADDR_G_CH_PARA_11REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long g_ch_h20_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��20��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
        unsigned long g_ch_h21_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��21��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_CH_PARA_11REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_CH_PARA_12REG_UNION
 ����˵��  : ADDR_G_CH_PARA_12REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long g_ch_h22_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��22��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
        unsigned long g_ch_h23_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��23��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_CH_PARA_12REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_CH_PARA_13REG_UNION
 ����˵��  : ADDR_G_CH_PARA_13REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long g_ch_h24_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��24��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
        unsigned long g_ch_h25_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��25��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_CH_PARA_13REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_CH_PARA_14REG_UNION
 ����˵��  : ADDR_G_CH_PARA_14REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long g_ch_h26_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��26��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
        unsigned long g_ch_h27_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��27��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_CH_PARA_14REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_CH_PARA_15REG_UNION
 ����˵��  : ADDR_G_CH_PARA_15REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long g_ch_h28_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��28��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
        unsigned long g_ch_h29_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��29��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_CH_PARA_15REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_CH_PARA_1REG_UNION
 ����˵��  : ADDR_G_CH_PARA_16REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long g_ch_h30_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��30��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
        unsigned long g_ch_h31_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��31��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_CH_PARA_16REG_UNION;

/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_CH_PARA_17REG_UNION
 ����˵��  : ADDR_G_CH_PARA_17REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long g_ch_h32_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��32��12bit�з�������    ��ʼ�� */
        unsigned long                                         : 20;     /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_CH_PARA_17REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_32CH_PARA_1REG_UNION
 ����˵��  : ADDR_G_32CH_PARA_1REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long g_32ch_h0_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��0��12bit�з�������    ��ʼ�� */
        unsigned long                                          : 4;      /* reserve */
        unsigned long g_32ch_h1_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��1��12bit�з�������    ��ʼ�� */
        unsigned long                                          : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_32CH_PARA_1REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_32CH_PARA_2REG_UNION
 ����˵��  : ADDR_G_32CH_PARA_2REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long g_32ch_h2_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��2��12bit�з�������    ��ʼ�� */
        unsigned long                                          : 4;      /* reserve */
        unsigned long g_32ch_h3_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��3��12bit�з�������    ��ʼ�� */
        unsigned long                                          : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_32CH_PARA_2REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_32CH_PARA_3REG_UNION
 ����˵��  : ADDR_G_32CH_PARA_3REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long g_32ch_h4_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��4��12bit�з�������    ��ʼ�� */
        unsigned long                                          : 4;      /* reserve */
        unsigned long g_32ch_h5_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��5��12bit�з�������    ��ʼ�� */
        unsigned long                                          : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_32CH_PARA_3REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_32CH_PARA_4REG_UNION
 ����˵��  : ADDR_G_32CH_PARA_4REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long g_32ch_h6_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��6��12bit�з�������    ��ʼ�� */
        unsigned long                                          : 4;      /* reserve */
        unsigned long g_32ch_h7_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��7��12bit�з�������    ��ʼ�� */
        unsigned long                                          : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_32CH_PARA_4REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_32CH_PARA_5REG_UNION
 ����˵��  : ADDR_G_32CH_PARA_5REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long g_32ch_h8_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��8��12bit�з�������    ��ʼ�� */
        unsigned long                                          : 4;      /* reserve */
        unsigned long g_32ch_h9_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��9��12bit�з�������    ��ʼ�� */
        unsigned long                                          : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_32CH_PARA_5REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_32CH_PARA_6REG_UNION
 ����˵��  : ADDR_G_32CH_PARA_6REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long g_32ch_h10_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��10��12bit�з�������    ��ʼ�� */
        unsigned long                                           : 4;      /* reserve */
        unsigned long g_32ch_h11_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��11��12bit�з�������    ��ʼ�� */
        unsigned long                                           : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_32CH_PARA_6REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_32CH_PARA_7REG_UNION
 ����˵��  : ADDR_G_32CH_PARA_7REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long g_32ch_h12_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��12��12bit�з�������    ��ʼ�� */
        unsigned long                                           : 4;      /* reserve */
        unsigned long g_32ch_h13_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��13��12bit�з�������    ��ʼ�� */
        unsigned long                                           : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_32CH_PARA_7REG_UNION;

/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_32CH_PARA_8REG_UNION
 ����˵��  : ADDR_G_32CH_PARA_8REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long g_32ch_h14_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��14��12bit�з�������    ��ʼ�� */
        unsigned long                                           : 4;      /* reserve */
        unsigned long g_32ch_h15_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��15��12bit�з�������    ��ʼ�� */
        unsigned long                                           : 4;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_32CH_PARA_8REG_UNION;

/*****************************************************************************
 ������    : HAL_GBBP_ADDR_G_32CH_PARA_2REG_UNION
 ����˵��  : ADDR_G_32CH_PARA_2REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long g_32ch_h16_ext                            : 12;     /* GSM�����ŵ��˲�������ϵ��16��12bit�з�������    ��ʼ�� */
        unsigned long                                           : 20;      /* reserve */
    }stReg;
}HAL_GBBP_ADDR_G_32CH_PARA_9REG_UNION;




/*--------------------------------------------------------------------------------------------*
 * 1.4  GRIFģ��
 *--------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------*
 *1.5   �Զ��������ģ��
 *--------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------*
 * GTC����
 *--------------------------------------------------------------------------------------------*/

/*****************************************************************************
 ������    : HAL_GTC_GSP_INIT_GAIN_RFIC_ADDR_REG
 ����˵��  : GSP_INIT_GAIN_RFIC_ADDR�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_init_gain_rfic                      : 3;     /* bit2~0: RFÿʱ϶����ĳ�ʼ��λ������RF��λ���������ֵ��Ӧ��ϵ�μ���19�� */
        unsigned short                                         : 3;
        unsigned short gsp_dcr_burst_mode                      : 1;     /* ʱ϶ȥֱ��ģʽ */
        unsigned short gsp_rf_gain_word_sel                    : 1;     /* 8����Ƶ������ѡ���ź� */
        unsigned short                                         : 8;     /* reserve */
    }stReg;
}HAL_GTC_GSP_INIT_GAIN_RFIC_ADDR_REG;


/*****************************************************************************
 ������    : HAL_GTC_GSP_DAGC_CTRL_UNION
 ����˵��  : GSP_DAGC_CTRL�Ĵ�������
*****************************************************************************/

typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_dagc_enDEL                          : 1;      /* bit0:DAGCģ�鹤��ʹ���źš� */
        unsigned short gsp_dagc_process_sel                    : 1;      /* DAGC����ģʽѡ��1Ϊ����ģʽ��0ΪNB���ģʽ�� */
        unsigned short reserve                                 : 14;     /* reserve */
    }stReg;
}HAL_GTC_GSP_DAGC_CTRL_UNION;


/*****************************************************************************
 ������    : HAL_GTC_GSP_AGC_CTRL1_UNION
 ����˵��  : GSP_AGC_CTRL1�Ĵ�������
*****************************************************************************/

typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_agc_en                              : 1;      /* AGCģ�鹤��ʹ���źš� */
        unsigned short gsp_frq_sel                             : 3;      /* ֱ��ƫ�ú�ǰ��ͨ���������RAM��ַ����ѡ���źš� */
        unsigned short                                         : 2;      /* reserve */
        unsigned short gsp_fast_agc_mode                       : 1;      /* fast ����ģʽ */
        unsigned short gsp_gain_sel                            : 1;      /* AGC��λ���÷�ʽѡ��0����agc_init_gain���ã�1����agc_next_gain���á� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_GSP_AGC_CTRL1_UNION;

/*****************************************************************************
 ������    : HAL_GTC_GSP_AGC_CTRL2_UNION
 ����˵��  : GSP_AGC_CTRL2�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_agc_average_time_round_one          : 2;      /* AGC���������ۼӴ��������á� */
        unsigned short gsp_agc_average_time_round_two          : 2;      /* AGC���������ۼӴ��������á� */
        unsigned short gsp_agc_average_time_round_three        : 2;      /* AGC���������ۼӴ��������á� */
        unsigned short gsp_dcr_nv_sel                          : 1;      /* �ֵ�ֱ��ѡ�� */
        unsigned short gsp_slot_dcr_mode                       : 1;      /* ʱ϶ǰ����ֱ��ģʽ������ */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_GSP_AGC_CTRL2_UNION;


/*****************************************************************************
 ������    : HAL_GTC_GSP_AGC_START_UNION
 ����˵��  : GSP_AGC_START�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_register_clr                        : 1;      /* ��·����ϸ����ƫ�üĴ������㣬1��ϵͳ���ڵ������źš� */
        unsigned short gsp_init_gain_en                        : 1;      /* AGC����ʱ϶��ʼ��λ������Чָʾ��1��ϵͳ���ڵ������źš�����ǰ���ݽ���ʱ��100us�������źţ�����gsp_init_gain��gsp_gain_sel�ź����ʹ�á� */
        unsigned short gsp_burst_register_clr                  : 1;      /* ʱ϶ǰֱ���ۼ�ϸ���������źš� */
        unsigned short gsp_dc_cal_start                        : 1;      /* ʱ϶ǰֱ�����������źš� */
        unsigned short gsp_dcr_recv_active                     : 1;      /* ���������ͨ��ֱ��ƫ������ϸ��������ߵ�ƽ��Ч */
        unsigned short                                         : 2;      /* reserve */
        unsigned short gsp_agc_fast_start                      : 1;      /* AGCģ����ٲ�����ʼָʾ�źţ�1��ϵͳ���ڵ������źš��ڹ�����ʼָʾ�źŷ���֮ǰ��Ҫ���ú�AGC�ı�ʱ϶�ĳ�ʼ��λ�����ٲ��������н���������� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_GSP_AGC_START_UNION;


/*****************************************************************************
 ������    : HAL_GTC_GSP_RSSI_FRQ_ADDR_UNION
 ����˵��  : GSP_RSSI_FRQ_HIGH_ADDR�Ĵ�������
*****************************************************************************/

typedef union
{
    unsigned short uhwValue;

    struct
    {
        unsigned short low_byte                                : 8;
        unsigned short high_byte                               : 8;
    }stWord;

    struct
    {
        unsigned short gsp_rssi_frq_low                        : 8;      /* ��ǰ������RSSI��Ӧ��Ƶ��ŵ�8bit */
        unsigned short gsp_rssi_frq_high                       : 4;      /* ��ǰ������RSSI��Ӧ��Ƶ��Ÿ�4bit��GSP_RSSI_FRQ_LOW_ADDR��GSP_RSSI_FRQ_HIGH_ADDR���gsp_rssi_frq[11:0] */
        unsigned short gsp_rrc_filter_para_sel                 : 3;     /* reserve */
        unsigned short                                         : 1;
    }stReg;
}HAL_GTC_GSP_RSSI_FRQ_ADDR_UNION;


/*****************************************************************************
 ������    : HAL_GTC_GSP_AGC_TRUNC_WIDTH_UNION
 ����˵��  : GSP_AGC_TRUNC_WIDTH�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_agc_trunc_width                     : 3;      /* AGC��������ϸ������λλ��Ĭ��ֵ3'd3��                                        3'd0��8��                                  3'd1��10��                                  3'd2��12��                                  3'd3��14��                                  3'd4��4��                                  3'd5��6 */
        unsigned short gsp_dagc_trunc_width                    : 3;      /* DAGC��������ϸ������λλ��Ĭ��ֵ3'd3��                                        3'd0��8��                                  3'd1��10��                                  3'd2��12��                                  3'd3��14��                                  3'd4��4��                                  3'd5��6�� */
        unsigned short                                         : 2;
        unsigned short reserved                                : 8;      /* reserve */
    }stReg;
}HAL_GTC_GSP_AGC_TRUNC_WIDTH_UNION;

/*--------------------------------------------------------------------------------------------*
 * GPU����
 *--------------------------------------------------------------------------------------------*/

/*****************************************************************************
 ������    : HAL_GBBP_CPU_SAMPLE_REG_UNION
 ����˵��  : CPU_SAMPLE_REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long                                         : 8;      /* reserve */
        unsigned long cpu_sample_sel_group                    : 1;      /* ��������ѡ�����ã����ź���Ҫ��֡ͷ���档 */
        unsigned long                                         : 3;      /* reserve */
        unsigned long cpu_dcr_bypass                          : 1;      /* ����ͨ��ֱ��ƫ������ϸ������·���ܣ��ߵ�ƽ��Ч�� */
        unsigned long                                         : 19;     /* reserve */
    }stReg;
}HAL_GBBP_CPU_SAMPLE_REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_AGC_WAIT_LENGTH_UNION
 ����˵��  : AGC_WAIT_LENGTH�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_agc_wait                            : 5;                 /* ����AGC��������ÿ��֮��ĵȴ�ʱ�䣬��λsymbol��Ĭ��8�� */
        unsigned long                                         : 3;                 /* reserve */
        unsigned long cpu_dcr_recv_bypass                     : 1;                 /* ���������ͨ��ֱ��ƫ������ϸ������·���ܣ��ߵ�ƽ��Ч�����ź���Чʱ���൱������ϸ�������Խ������ݽ��з������� */
        unsigned long                                         : 3;
        unsigned long cpu_rssi_hypersis                       : 4;                 /* RSSI���ͷ�Χ����λdBm */
        unsigned long                                         : 8;
        unsigned long cpu_dcr_burst_mode                      : 1;                 /* ���ͨ��ֱ��ƫ������ϸ������·���ܣ��ߵ�ƽ��Ч�� */
        unsigned long                                         : 7;
    }stReg;
}HAL_GBBP_AGC_WAIT_LENGTH_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_R2_RSSI1234_THRESHOLD_UNION
 ����˵��  : R2_RSSI1234_THRESHOLD�Ĵ�������
*****************************************************************************/

typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_r2_rssi1_threshold                     : 8;      /* RSSI����1����λdBm */
        unsigned long cpu_r2_rssi2_threshold                     : 8;      /* RSSI����2����λdBm */
        unsigned long cpu_r2_rssi3_threshold                     : 8;      /* RSSI����3����λdBm */
        unsigned long                                            : 8;      /* reserve */
    }stReg;
}HAL_GBBP_R2_RSSI1234_THRESHOLD_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_R2_RSSI4567_THRESHOLD_UNION
 ����˵��  : R2_RSSI4567_THRESHOLD�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_r2_rssi4_threshold                     : 8;      /* RSSI����4����λdBm */
        unsigned long cpu_r2_rssi5_threshold                     : 8;      /* RSSI����5����λdBm */
        unsigned long cpu_r2_rssi6_threshold                     : 8;      /* RSSI����6����λdBm */
        unsigned long cpu_r2_rssi7_threshold                     : 8;      /* RSSI����7����λdBm */
    }stReg;
}HAL_GBBP_R2_RSSI4567_THRESHOLD_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_R3_RSSI1234_THRESHOLD_UNION
 ����˵��  : R3_RSSI1234_THRESHOLD�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_r3_rssi1_threshold                     : 8;      /* RSSI����1����λdBm */
        unsigned long cpu_r3_rssi2_threshold                     : 8;      /* RSSI����2����λdBm */
        unsigned long cpu_r3_rssi3_threshold                     : 8;      /* RSSI����3����λdBm */
        unsigned long                                            : 8;      /* reserve */
    }stReg;
}HAL_GBBP_R3_RSSI1234_THRESHOLD_UNION;



/*****************************************************************************
 ������    : HAL_GBBP_R3_RSSI4567_THRESHOLD_UNION
 ����˵��  : R3_RSSI4567_THRESHOLD�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_r3_rssi4_threshold                     : 8;      /* RSSI����4����λdBm */
        unsigned long cpu_r3_rssi5_threshold                     : 8;      /* RSSI����5����λdBm */
        unsigned long cpu_r3_rssi6_threshold                     : 8;      /* RSSI����6����λdBm */
        unsigned long cpu_r3_rssi7_threshold                     : 8;      /* RSSI����7����λdBm */
    }stReg;
}HAL_GBBP_R3_RSSI4567_THRESHOLD_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_CPU_OFC_RXCM_REG_UNION
 ����˵��  : CPU_OFC_RXCM_REG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long                                         : 31;      /* reserve */
        unsigned long cpu_agc_page_switch_en                  : 1;      /* AGCģ������ǰ��ͨ���������ֵ��RXCORR�����洢RAM ��DC_OFFSET �洢RAM  */
    }stReg;
}HAL_GBBP_CPU_OFC_RXCM_REG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_DC_OFFSET_UNION
 ����˵��  : DC_OFFSET�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long i_dc_offset                      : 14;      /* I·DC */
        unsigned long                                  : 2;
        unsigned long q_dc_offset                      : 14;      /* Q·DC */
        unsigned long                                  : 2;
    }stReg;
    struct
    {
        unsigned long i_dc_offset                      : 12;      /* I·DC */
        unsigned long                                  : 4;
        unsigned long q_dc_offset                      : 12;      /* Q·DC */
        unsigned long                                  : 4;
    }stRegNonPilot;
}HAL_GBBP_DC_OFFSET_UNION;



/*--------------------------------------------------------------------------------------------*
 * 1.7  ��λģ��
 *--------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------*
 * 1.8  ��ģ��ʱ�ӿ���ģ��
 *--------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------*
 * 1.9  GTCģ��
 *--------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------*
 * GTC����
 *--------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------*
 * CPU����
 *--------------------------------------------------------------------------------------------*/
/*****************************************************************************
 ������    : HAL_GBBP_CPU_GTC_T1_T2_T3_CNG_UNION
 ����˵��  : CPU_GTC_T1_T2_T3_CNG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long cpu_gtc_t1_cnt                          : 11;     /* ����֡��ά�������е�T1��ʼֵ���ò���ֵ��cpu_gtc_t2_cnt��cpu_gtc_t3_cnt�� */
        unsigned long                                         : 5;      /* reserve */
        unsigned long cpu_gtc_t2_cnt                          : 5;      /* ����֡��ά�������е�T2��ʼֵ���ò���ֵ��cpu_gtc_t1_cnt����GTC��TDMA֡�߽紦��Ч,����Ӧ����Ϊ��һ֡��֡�� */
        unsigned long                                         : 3;      /* reserve */
        unsigned long cpu_gtc_t3_cnt                          : 6;      /* ����֡��ά�������е�T3��ʼֵ���ò���ֵ��cpu_gtc_t1_cnt�� */
        unsigned long                                         : 2;      /* reserve */
    }stReg;
}HAL_GBBP_CPU_GTC_T1_T2_T3_CNG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_CPU_GTC_FRM_OFFSET_CFG_UNION
 ����˵��  : CPU_GTC_FRM_OFFSET_CFG�Ĵ�������
*****************************************************************************/

typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long cpu_gtc_frm_offset                      : 13;     /* bit[12:0] CPU���õ�gtc_frm_offset���� */
        unsigned long                                         : 3;      /* reserve */
        unsigned long cpu_gtc_frm_it_offset                   : 13;     /* bit[28:16] CPU���õ�gtc_frm_it_offset���� */
        unsigned long                                         : 2;      /* reserve */
        unsigned long cpu_gtc_frm_offset_wr_en                : 1;      /* CPU���õ�cpu_gtc_frm_offset��cpu_gtc_frm_it_offset������Чָʾ�źţ��߼������㡣 */
    }stReg;
}HAL_GBBP_CPU_GTC_FRM_OFFSET_CFG_UNION;



/*****************************************************************************
 ������    : HAL_GBBP_CPU_GTC_FN_CNG_UNION
 ����˵��  : CPU_GTC_FN_CNG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_gtc_fn_low                          : 11;     /* bit[10:0] CPU���õ�TDMA֡�ŵ�λ0~26x51-1ѭ������, CPU���ú���GTC��TDMA֡�߽紦��Ч,����Ӧ����Ϊ��һ֡��֡�� */
        unsigned long                                         : 5;      /* reserve */
        unsigned long cpu_gtc_fn_high                         : 11;     /* bit[26:16] CPU����TDMA֡�Ÿ�λ0~2047ѭ����������λ�����󣬸�λ�ż�1��CPU���ú���GTC��TDMA֡�߽紦��Ч,����Ӧ����Ϊ��һ֡��֡�� */
        unsigned long                                         : 4;      /* reserve */
        unsigned long cpu_gtc_fn_wr_en                        : 1;      /* ֡�ų�ʼ���źţ�1��Ч���߼������㣬ֻ�е���ҪT1,T2,T3��fn_low��fn_high��д��󷽿ɶԸ�bit��1 */
    }stReg;
}HAL_GBBP_CPU_GTC_FN_CNG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_GTC_FN_RPT_UNION
 ����˵��  : GTC_FN_RPT�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uhwValue;
    struct
    {
        unsigned long gtc_fn_low                              : 11;     /* GTC��ʱʵ��֡�ŵ�λ */
        unsigned long                                         : 5;      /* reserve */
        unsigned long gtc_fn_high                             : 11;     /* bit[26:16] GTC��ʱʵ��֡�Ÿ�λ */
        unsigned long                                         : 5;      /* reserve */
    }stReg;
}HAL_GBBP_GTC_FN_RPT_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_CPU_GTC_CFG_UNION
 ����˵��  : CPU_GTC_CFG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uhwValue;
    struct
    {
        unsigned long gtc_page_switch_en                      : 1;      /* 0�����л�ָ��RAMҳ��1��ƹ���л�ָ��RAMҳ����λ���һ������ʱӦ�л�ָ��ҳ�� */
        unsigned long cpu_gtc_start                           : 1;      /* д1����ָ��ҳִ�У��ڲ�����һ��52M���壬 */
        unsigned long gtc_auto_run                            : 1;      /* 1��GTCָ����TDMA֡ͷ���Զ�ִ��ʹ�ܣ�Ĭ��Ϊ0�� */
        unsigned long cpu_gtc_hold_page                       : 1;      /* 1��ʾ�����޸�����ִ�е�GTCָ��ҳ����Ҫ�޸�ʱ��Ҫ������1���޸���ɺ�����0  */
        unsigned long gtc_pro_en                              : 1;      /* GTCģ�鹤��ʹ���źţ��������ø��ź�Ϊ�ߣ�Ȼ��������GTC */
        unsigned long                                         : 3;      /* reserve */
        unsigned long cpu_drx_test_mode_en                    : 1;      /* 1��ʾDRX����ģʽʹ�ܣ���ʱGTCʱ��Ӧ���رգ�����DRX���Ѻ�Ķ�ʱά������ıȶԡ�����ʹ���¸��ź�Ӧ�ñ���0�������� */
        unsigned long                                         : 23;     /* reserve */
    }stReg;
}HAL_GBBP_CPU_GTC_CFG_UNION;

/*--------------------------------------------------------------------------------------------*
 * 1.10 ͬ������ģ��
 *--------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------*
 * GTC����
 *--------------------------------------------------------------------------------------------*/

/*****************************************************************************
 ������    : HAL_GTC_SRCH_CFG_UNION
 ����˵��  : GTC_SRCH_CFG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_srch_en                             : 1;      /* CPU����ʹ���źţ������������ڼ䶼����Ϊ�ߵ�ƽ�� */
        unsigned short gsp_srch_mod                            : 3;      /* ����ģʽ���ã� */
        unsigned short gsp_fb_pos_adj                          : 3;      /* CPU��FB������λ�õĵ�������ҪΪ�����Ʒ��ն���Ƶġ� */
        unsigned short gsp_soft_agc_drssi_25db_en              : 1;      /* ���ǿ������DRSSI��ʹ�ܿ��� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_SRCH_CFG_UNION;


/*****************************************************************************
 ������    : HAL_GTC_SRCH_START_UNION
 ����˵��  : GTC_SRCH_START�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_srch_start                          : 1;      /* cpu�������������źţ������źš�ÿ����������ʱ��Ҫ��һ�Σ�1��Ч����ͬ��,GTCģ���ڲ���������ʹ������Ҫ��qb�ڵĵ�һ��clk_sys����Ч,GTC�߼������㡣��gsp_srch_mod=001ʱ��gsp_srch_start������ǰ��ǰĬ�ϵ�SBλ��34�����Ų����� */
        unsigned short                                         : 5;      /* reserve */
        unsigned short gsp_nco_en                              : 1;      /* reserve */
        unsigned short gsp_ncell_srch_flag                     : 1;      /* 1��ʶ��ǰ������������0��ʶ��ǰ�Ƿ��������� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_SRCH_START_UNION;

/*--------------------------------------------------------------------------------------------*
 * CPU����
 *--------------------------------------------------------------------------------------------*/
/*****************************************************************************
 ������    : HAL_GBBP_CPU_AFC_CFG_UNION
 ����˵��  : CPU_AFC_CFG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long cpu_afc_init                            : 14;     /* CPU���õ�AFC�ĳ�ʼֵ��cpu_afc_init��Ҫ��cpu_afc_init_enǰ׼���ã��ڿ�����ĳ�ʼ��������Ҫ���ô�ֵ   mpu_int0 */
        unsigned long reserved0                               : 1;      /* reserve */
        unsigned long cpu_afc_init_en                         : 1;      /* AFC��ʼֵд��ʹ�ܣ�CPUд1���߼������㣬��������CPU_AFC_LOOP1_CFG��������CPU_AFC_CFG mpu_int0 */
        unsigned long reserved1                               : 16;     /* reserve */
    }stReg;
}HAL_GBBP_CPU_AFC_CFG_UNION;

/*****************************************************************************
 ������    : HAL_GBBP_AFC_RPT_UNION
 ����˵��  : AFC_RPT�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long afc_vctxo_cpu                           : 14;     /* AFC�����VCTXO�Ŀ��Ƶ�ѹ�ź�    mpu_int_2 */
        unsigned long                                         : 18;     /* reserve */
    }stReg;
}HAL_GBBP_AFC_RPT_UNION;

/*****************************************************************************
 ������    : HAL_GBBP_AFC_LOOP1_RPT_UNION
 ����˵��  : AFC_LOOP1_RPT�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long nbafc_1loop_cpu                         : 28;     /* ��ǰNB-AFC��·�˲��ۼ���һ���ϱ�ֵ */
        unsigned long                                         : 4;      /* reserve */
    }stReg;
}HAL_GBBP_AFC_LOOP1_RPT_UNION;



/*****************************************************************************
 ������    : HAL_GBBP_CPU_FB_TIMES_THRES_UNION
 ����˵��  : CPU_FB_TIMES_THRES�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_fb_failed_thres                     : 2;      /* FBͬ��ʧ�ܴ������ޡ� */
        unsigned long                                         : 2;      /* reserve */
        unsigned long cpu_sb_failed_thres                     : 2;      /* SB����ʧ�ܴ������ޡ� */
        unsigned long                                         : 6;      /* reserve */
        unsigned long cpu_fb_peak_window                      : 17;     /* FB�������ķ��Ÿ�������ֵ��ȱʡֵΪ13906����Ӧ11֡��1ʱ϶������������ʱ�䳤�ȴ��ڸ�ֵʱ��FB���������� */
        unsigned long                                         : 3;      /* reserve */
    }stReg;
}HAL_GBBP_CPU_FB_TIMES_THRES_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_CPU_FB_KP_KINT_UNION
 ����˵��  : CPU_FB_KP_KINT�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long                                         : 16;     /* reserve */
        unsigned long cpu_afc_maxfbnum                        : 5;      /* ����FB��AFC�о�����FB������ */
        unsigned long                                         : 11;     /* reserve */
    }stReg;
}HAL_GBBP_CPU_FB_KP_KINT_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_SB_ADVANCE_LEN_CFG_UNION
 ����˵��  : SB_ADVANCE_LEN_CFG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long cpu_sb_advance_len                      : 6;      /* 001����ģʽ�£�SB��ǰ���յķ��Ÿ���������SB֮ǰ�������cpu_sb_advance_len�����ţ���Χ��0��34 */
        unsigned long reserve                                 : 26;     /* reserve */
    }stReg;
}HAL_GBBP_SB_ADVANCE_LEN_CFG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_SRCH_QB_RPT_UNION
 ����˵��  : SRCH_QB_RPT�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long srch_timebase_qb                        : 13;     /* �����ж�����gtc_timebase�ϱ� */
        unsigned long                                         : 3;      /* reserve */
        unsigned long srch_int_mod                            : 3;      /* �����ж�ʱgsp_srch_mod�ϱ� */
        unsigned long                                         : 9;      /* reserve */
        unsigned long srch_start_mod                          : 3;      /* ��������ʱgsp_srch_mod�ϱ� */
        unsigned long ncell_srch_flag                         : 1;      /* ��ǰ����С�������ϱ���1��ʾ������0��ʾ������ */
    }stReg;
}HAL_GBBP_SRCH_QB_RPT_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_NB_AFC_LOOP_SWITCH_UNION
 ����˵��  : NB_AFC_LOOP_SWITCH�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_nb_afc_switch                       : 1;      /* һ�׻�����׻�ѡ������źš�0:��ʾ����1�׻�·��1:��ʾ����2�׻�·�� */
        unsigned long cpu_nb_afc_slow_fast_switch             : 1;      /* ���׻������ٽ׶ο��ơ�0:��ʾ�������ٸ��٣�1:��ʾ���ÿ��ٸ��١�*/
        unsigned long                                         : 29;
        unsigned long cpu_nb_afc_sel                          : 1;      /* NB-AFC���Ʒ�ʽѡ��0��GTC���ƣ�1��CPU���ơ�Ĭ��CPU���ơ� */
    }
    stReg;
}HAL_GBBP_NB_AFC_LOOP_SWITCH_UNION;


/*--------------------------------------------------------------------------------------------*
 * 1.11 ���н��ģ��
 *--------------------------------------------------------------------------------------------*/

/*****************************************************************************
 ������    : HAL_GTC_DEMOD_PARA1_UNION
 ����˵��  : GTC_DEMOD_PARA1�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_nb_demod_en                         : 1;      /* NB���ʹ���źţ��ߵ�ƽ��Ч����NB���������һֱΪ�ߣ�����������ǰҲ��Ҫ�����ź���1 */
        unsigned short gsp_dl_tsc_group_num                    : 3;      /* ѵ��������� */
        unsigned short gsp_demod_slot                          : 3;      /* �������ʱ��ʱ϶�ţ�����TOAֵ��·�ϱ� */
        unsigned short gsp_nserv_cell_ind                      : 1;      /* 1��ʾ�Ƿ���С�� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_DEMOD_PARA1_UNION;


/*****************************************************************************
 ������    : HAL_GTC_DEMOD_PARA2_UNION
 ����˵��  : GTC_DEMOD_PARA2�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_demod_type                          : 2;      /* ���е��Ʒ�ʽָʾ 00 GMSK���� 01 8PSK����,10��ʾ���е��Ʒ�ʽδ֪��11 �����壻 */
        unsigned short gsp_ls_dcr_en                           : 1;      /* reserve */
        unsigned short gsp_demod_flag_sav_pos                  : 5;      /* PS��DATAҵ���� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_DEMOD_PARA2_UNION;


/*****************************************************************************
 ������    : HAL_GTC_DEMOD_PARA3_UNION
 ����˵��  : GTC_DEMOD_PARA3�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_dem_burst_pos                       : 7;      /* ���BURST���ݴ洢��ʼλ��ָʾ */
        unsigned short                                         : 9;      /* reserve */
    }stReg;
}HAL_GTC_DEMOD_PARA3_UNION;


/*****************************************************************************
 ������    : HAL_GTC_TOA_PARA_UNION
 ����˵��  : GTC_TOA_PARA�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_nb_pos                              : 4;      /* ������ģʽ�����������ĵ�ǰNBλ�õĵ���ֵ������ģʽ�¸��źſ�������Ϊ0 */
        unsigned short gsp_nb_toa_alpha_sel                    : 1;      /* ѡ���Ƿ��ڱ�ʱ϶��������Alpha�˲�����ƽ�źš� */
        unsigned short gsp_nb_pos_sel                          : 1;      /* ������㵱ǰNBλ�õ�ѡ���źš� */
        unsigned short gsp_nb_nco_en                           : 1;      /* ������NBʱ����ƫʹ���ź� */
        unsigned short gsp_nb_afc_en                           : 1;      /* NB-AFCʹ���źţ�1��Ч�� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_TOA_PARA_UNION;


/*****************************************************************************
 ������    : HAL_GTC_DEMOD_START_UNION
 ����˵��  : GTC_DEMOD_START�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_demod_start                         : 1;      /* ����NB��������źţ������źţ� */
        unsigned short gsp_gaci_en                             : 1;      /* ��Ƶ���ģ��ʹ���źš���Ƶ���ƹ��ܽ������NB�����ʹ�ã�����ģʽ�£�FB��SB����������Ƶ��⡣ */
        unsigned short gsp_gaci_index                          : 2;      /* �˲���ϵ��ѡ��λ */
        unsigned short                                         : 12;     /* reserve */
    }stReg;
}HAL_GTC_DEMOD_START_UNION;





/*--------------------------------------------------------------------------------------------*
 * CPUģ��
 *--------------------------------------------------------------------------------------------*/

/*****************************************************************************
 ������    : HAL_GBBP_CPU_SAIC_CTRL_UNION
 ����˵��  : CPU_SAIC_CTRL�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_saic_shift_pre                      : 2;      /* �о�����ǰ��SAIC�˲����ֵ����λֵ��11��13���أ�Ĭ��ֵ2'b01����ֵ������������ƫ� */
        unsigned long                                         : 6;      /* reserve */
        unsigned long cpu_saic_shift_aft                      : 2;      /* �о�������SAIC�˲����ֵ����λֵ��10��12���أ�Ĭ��ֵ��2'b01����ֵ������������ƫ� */
        unsigned long                                         : 6;      /* reserve */
        unsigned long cpu_renew_toa_pos                       : 2;      /* DSP��֡ͷλ��������֡NBλ�õĵ������� */
        unsigned long                                         : 2;      /* reserve */
        unsigned long cpu_gmsk_ci_beta                        : 2;      /* GMSK CI�����ĵ���ϵ����Ĭ��ֵ��15(����Ϊ2'b00)����ѡֵ1��8 */
        unsigned long                                         : 2;      /* reserve */
        unsigned long cpu_cross_delta                         : 3;      /* case(cpu_cross_delta) */
        unsigned long                                         : 4;      /* reserve */
        unsigned long cpu_nb_info_rpt_mode                    : 1;      /* 1 ��ʾ�̶�ѡ��ż·��0��ʾѡ�����· Ĭ��ֵΪ0��                     ���ź�����ѡ���ϱ���nb_pos_cpu, nb_pos_adj_cpu��C/I�ȡ� */
    }stReg;
}HAL_GBBP_CPU_SAIC_CTRL_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_ENG_WEIGHT_UNION
 ����˵��  : ENG_WEIGHT�Ĵ�������
*****************************************************************************/

typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_mid_finger_weight                   : 3;      /* NBλ�ü���ʹ�õ����ľ�������Ȩֵ��Ĭ��ֵ0 */
        unsigned long                                         : 1;      /* reserve */
        unsigned long cpu_rptpos_use_weight                   : 1;      /* �ϱ���nb_pos��nb_pos_adjʹ�����ľ�������Ȩ�㷨��1Ϊʹ�ã�Ĭ��ֵ0 */
        unsigned long                                         : 3;      /* reserve */
        unsigned long cpu_dempos_use_weight                   : 1;      /* ���λ�ü̳�ʹ�����ľ�������Ȩ�㷨��1Ϊʹ�ã�Ĭ��ֵ0 */
        unsigned long                                         : 7;      /* reserve */
        unsigned long cpu_sb_finger_weight                    : 3;      /* SBλ�ü���ʹ�õ����ľ�������Ȩֵ��Ĭ��ֵ0 */
        unsigned long                                         : 13;     /* reserve */
    }stReg;
}HAL_GBBP_ENG_WEIGHT_UNION;



/*****************************************************************************
 ������    : HAL_GBBP_CPU_DEM_WIDTH_CFG_UNION
 ����˵��  : CPU_DEM_WIDTH_CFG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long cpu_demod_width_adjust                  : 2;      /* �����̬λ������ */
        unsigned long reserved0                               : 14;     /* reserve */
        unsigned long cpu_harq_width_mode                     : 1;      /* HARQ����ǰ����λ��ָ�ģʽ���� */
        unsigned long reserved1                               : 15;     /* reserve */
    }stReg;
}HAL_GBBP_CPU_DEM_WIDTH_CFG_UNION;

/*****************************************************************************
 ������    : HAL_GBBP_CPU_TOA_ALPHA_PARA_UNION
 ����˵��  : CPU_TOA_ALPHA_PARA�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long cpu_alpha_parameter                     : 3;      /* alpha�˲�ϵ��ѡ�� */
        unsigned long                                         : 29;     /* reserve */
    }stReg;
}HAL_GBBP_CPU_TOA_ALPHA_PARA_UNION;

/*****************************************************************************
 ������    : HAL_GBBP_CPU_CI_THRES_CFG_UNION
 ����˵��  : CPU_CI_THRES_CFG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_gmsk_ci_thres                       : 3;                 /* GMSK���Ʒ�ʽ�£�CI�о�����ѡ���źţ�����ɸѡnb-afc����� */
        unsigned long                                         : 13;                /* reserve */
        unsigned long cpu_psk_ci_thres                        : 3;                 /* 8PSK���Ʒ�ʽ�£�CI�о�����ѡ���źţ�����ɸѡnb-afc����� */
        unsigned long                                         : 13;                /* reserve */
    }stReg;
}HAL_GBBP_CPU_CI_THRES_CFG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_LS_DC_CPU_UNION
 ����˵��  : LS_DC_CPU�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long ls_dc_q_cpu                           : 8;                /* Q·DC */
        unsigned long                                       : 8;
        unsigned long ls_dc_i_cpu                           : 8;                /* I·DC */
        unsigned long                                       : 8;
    }stReg;
}HAL_GBBP_LS_DC_CPU_UNION;


/*--------------------------------------------------------------------------------------------*
 * 1.12 �����ŵ�����ģ��
 *--------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------*
 * GTCģ��
 *--------------------------------------------------------------------------------------------*/

/*****************************************************************************
 ������    : HAL_GTC_DL_CB_INDEX_UNION
 ����˵��  : GTC_DL_CB_INDEX�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_dl_cb_index                         : 7;      /* һ������±�ʾ����ҵ����� */
        unsigned short gsp_cycle_rpt_en                        : 1;      /* 1��ʾѭ���ϱ�ģʽʹ�ܣ���ʱ��������������ϱ��ж� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_DL_CB_INDEX_UNION;


/*****************************************************************************
 ������    : HAL_GTC_DEC_BURST_POS_UNION
 ����˵��  : GTC_DEC_BURST_POS�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_dec_burst_pos                       : 7;      /* ��ʾ���������ĵ�һ��BURST��VALPP��ֵ�洢RAM�е�λ�ñ�� */
        unsigned short gsp_amr_state_flag                      : 1;      /* 1��ʾ����AMR ��SPEECH̬ */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_DEC_BURST_POS_UNION;


/*****************************************************************************
 ������    : HAL_GTC_DEC_TYPE1_UNION
 ����˵��  : GTC_DEC_TYPE1�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_enhanced_dec_mode                   : 2;      /* gsp_enhanced_dec_mode[1]��1��ʾ��ǿ������ģʽʹ�ܣ�0��ʾ���ô�ͳVTB����ģʽ */
        unsigned short                                         : 2;      /* reserve */
        unsigned short gsp_dl_amr_cmc_frame                    : 1;      /* ��ʾ��ǰ֡�ŵ�CMI��CMCָʾ��0��ʾ��CMI֡��1��ʾ��CMC֡����λĬ��ֵΪ0 */
        unsigned short gsp_dl_amr_cmi_value                    : 2;      /* ������õ���ʷCMIֵ */
        unsigned short gsp_facch_dec_mode                      : 1;      /* FACCH����ģʽѡ��AFS��FS��EFS��1 ��ʾ����������ж��Ƿ�ΪFACCH͵֡�������,���߼�����FACCH����,�������,���߼���������������룻0 ��ʾֱ������FACCH������������CRCУ�����ж��Ƿ�ΪFACCH͵֡�� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_DEC_TYPE1_UNION;


/*****************************************************************************
 ������    : HAL_GTC_DEC_TYPE2_UNION
 ����˵��  : GTC_DEC_TYPE2�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_cs4_judge_mode                      : 2;      /* 00 ��ʾ��GPRS���磬�������������Ϣ����CS4��MCS1234����ΪCS4 */
        unsigned short                                         : 2;      /* reserve */
        unsigned short gsp_edge_usf_rxqual_en                  : 1;      /* 1��ʾ EDGEҵ����USF����BER��BEPͳ�ƣ�Ĭ��ֵΪ0 */
        unsigned short gsp_bep_en                              : 1;      /* 1��ʾ��bepͳ�ƹ��ܣ�0��ʾ�ر�bepͳ�ƹ��ܣ�Ĭ��ֵΪ0 */
        unsigned short gsp_ib_im_rxqual_en                     : 1;      /* 1��ʾ����bit ib��ID_MARKER bit������������ͳ�� */
        unsigned short gsp_harq_func_en                        : 1;      /* 1��ʾ�������HARQ����ʹ�ܣ�0��ʾ�ù������� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_DEC_TYPE2_UNION;

/*****************************************************************************
 ������    : HAL_GTC_HARQ_POS1_UNION
 ����˵��  : GTC_HARQ_POS1�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_harq1_pos                           : 7;      /* ��HARQ��������б�ʾ��Ҫ��harq�ϲ��ĵ�һ�����Ķ���ʼλ�ã���HARQ��ֵ���������Ϊ��һ������д��ʼλ�� */
        unsigned short                                         : 9;      /* reserve */
    }stReg;
}HAL_GTC_HARQ_POS1_UNION;


/*****************************************************************************
 ������    : HAL_GTC_HARQ_POS2_UNION
 ����˵��  : GTC_HARQ_POS2�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_harq2_pos                           : 7;      /* ��HARQ��������б�ʾ��Ҫ��harq�ϲ��ĵڶ������Ķ���ʼλ�ã���HARQ��ֵ���������Ϊ�ڶ�������д��ʼλ�� */
        unsigned short                                         : 9;      /* reserve */
    }stReg;
}HAL_GTC_HARQ_POS2_UNION;


/*****************************************************************************
 ������    : HAL_GTC_HARQ1_CB_INDEX_UNION
 ����˵��  : GTC_HARQ1_CB_INDEX�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_harq1_cb_index                      : 7;      /* bit6��bit0������HARQ�ϲ����������һ����飨Ӧ��ΪDATA�飩��CB_INDEX����λĬ��ֵΪ7'd52�������MCS6��9��MCS5��7�ϲ�����ô����ѡ������һ����飨Ӧ��ΪDATA�飩��cb_index�������� */
        unsigned short                                         : 9;      /* reserve */
    }stReg;
}HAL_GTC_HARQ1_CB_INDEX_UNION;


/*****************************************************************************
 ������    : HAL_GTC_HARQ2_CB_INDEX_UNION
 ����˵��  : GTC_HARQ2_CB_INDEX�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_harq2_cb_index                      : 7;      /* bit6��bit0������HARQ�ϲ�����ĵڶ�������CB_INDEX */
        unsigned short                                         : 9;      /* reserve */
    }stReg;
}HAL_GTC_HARQ2_CB_INDEX_UNION;


/*****************************************************************************
 ������    : HAL_GTC_DL_BURST_CYCLE_UNION
 ����˵��  : GTC_DL_BURST_CYCLE�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_dl_burst_sav_cycle                  : 6;      /* ����������BURSTѭ�����ڣ��ò���ֻ��CS��ҵ����Ч������λĬ��ֵΪ24����ʾ����Ϊ24����22BURST��֯��ҵ������Ҫ��������������á� */
        unsigned short gsp_bler_en                             : 1;      /* ��������BLERģ��ʹ���źţ��ߵ�ƽ��Ч������Ҫʱ�ضϣ��Խ�ʡ���ģ� */
        unsigned short gsp_ber_en                              : 1;      /* ��������BERģ��ʹ���źţ��ߵ�ƽ��Ч������Ҫʱ�ضϣ��Խ�ʡ���ģ� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_DL_BURST_CYCLE_UNION;


/*****************************************************************************
 ������    : HAL_GTC_GSP_DEC_TEST_MODE_EN_UNION
 ����˵��  : GSP_DEC_TEST_MODE_EN�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_dec_test_mode_en                    : 1;      /* 1��ʾ����ģʽ�ǲ���ģʽ����ʱ����ģ�鲻��֡�����о� */
        unsigned short gsp_enhanced_ut_en                      : 1;      /* 1��ʾ����ǿ�������UT��֤ģʽ�������� */
        unsigned short                                         : 2;      /* reserve */
        unsigned short gsp_reduce_page_mode                    : 4;      /* bit3~bit0���ֱ��ʾ��ǰ����Ѱ������ʱ��Ч��BURST�Ƿ���ڣ���0011��ʶ��һ�����ڶ���BURST���ڣ����������ĸ�BURST�������ݲ��㡣 */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_GSP_DEC_TEST_MODE_EN_UNION;


/*****************************************************************************
 ������    : HAL_GTC_GSP_DEC_TEST_AMR_FRM_UNION
 ����˵��  : GSP_DEC_TEST_AMR_FRM�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_dec_test_amr_frm                    : 6;      /* ���������ģʽ�����õ�AMR֡���ͣ���ʱ��Ȼ��Ҫ����gsp_dl_cb_indexΪAFS����AHSҵ�� */
        unsigned short                                         : 10;     /* reserve */
    }stReg;
}HAL_GTC_GSP_DEC_TEST_AMR_FRM_UNION;


/*****************************************************************************
 ������    : HAL_GTC_GSP_DEC_TEST_PS_FRM_UNION
 ����˵��  : GSP_DEC_TEST_PS_FRM�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_dec_test_ps_frm                     : 3;      /* ���������ģʽ�����õ�PSҵ��֡���� */
        unsigned short gsp_dec_test_ps_cps                     : 5;      /* ���������ģʽ�����õ�PSҵ���Ӧ����ҵ���CPSbit��Ϣ������Ϣ�����õ�PSҵ��֡���� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_GSP_DEC_TEST_PS_FRM_UNION;

/*****************************************************************************
 ������    : HAL_GTC_DEC_START_UNION
 ����˵��  : GTC_DEC_START�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_dec_start                           : 1;      /* NB����ҵ������������ź�,GTC�߼������� */
        unsigned short gsp_last_dec_flag                       : 1;      /* 1��ʾ����������Ҫ�ϱ��жϣ��������ж�ģʽֻ��gsp_cycle_rpt_enΪ0����Ч */
        unsigned short gsp_dec_slot                            : 3;      /* ������������ʱ��Ӧ��ʱ϶�� */
        unsigned short                                         : 1;      /* reserve */
        unsigned short gsp_bler_clr                            : 1;      /* ���������ϱ�BLER�����źţ������ź�,GTC�߼������� */
        unsigned short gsp_ber_clr                             : 1;      /* ���������ϱ�BER�����źţ������ź�,GTC�߼������� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_DEC_START_UNION;


/*--------------------------------------------------------------------------------------------*
 * CPUģ��
 *--------------------------------------------------------------------------------------------*/

/*****************************************************************************
 ������    : HAL_GBBP_CPU_AMR_CB_INDEX_UNION
 ����˵��  : CPU_AMR_CB_INDEX�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long cpu_amr0_cb_index                       : 7;      /* AMR�����������codec_mode=00��Ӧ��AMR����֡cb_index */
        unsigned long                                         : 1;      /* reserve */
        unsigned long cpu_amr1_cb_index                       : 7;      /* AMR�����������codec_mode=01��Ӧ��AMR����֡cb_index */
        unsigned long                                         : 1;      /* reserve */
        unsigned long cpu_amr2_cb_index                       : 7;      /* AMR�����������codec_mode=10��Ӧ��AMR����֡cb_index */
        unsigned long                                         : 1;      /* reserve */
        unsigned long cpu_amr3_cb_index                       : 7;      /* AMR�����������codec_mode=11��Ӧ��AMR����֡cb_index */
        unsigned long                                         : 1;      /* reserve */
    }stReg;
}HAL_GBBP_CPU_AMR_CB_INDEX_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_CPU_AMR_ACTIVE_SET_NUM_UNION
 ����˵��  : CPU_AMR_ACTIVE_SET_NUM�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_amr_active_set_num                  : 2;      /* AMR�����������Ч����֡���͸��� */
        unsigned long                                         : 2;      /* reserve */
        unsigned long cpu_speech_bfi_thres                    : 4;      /* ����֡BFI��������о����ޣ����÷�Χ0��8 */
        unsigned long                                         : 8;      /* reserve */
        unsigned long cpu_amr_active_set_num_upflag           : 1;      /* ��AMR������������仯ʱ����Ҫ���ø��ź�Ϊ1 */
        unsigned long                                         : 15;     /* reserve */
    }stReg;
}HAL_GBBP_CPU_AMR_ACTIVE_SET_NUM_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_CMI_P01_PRIOR_PRE_RPT_UNION
 ����˵��  : CMI_P01_PRIOR_PRE_RPT�Ĵ�������
*****************************************************************************/

typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long mappre_0_cmi                            : 16;     /* CMI����bit 0��Ӧ����������ϱ� */
        unsigned long mappre_1_cmi                            : 16;     /* CMI����bit 1��Ӧ����������ϱ� */
    }stReg;
}HAL_GBBP_CMI_P01_PRIOR_PRE_RPT_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_CMI_P23_PRIOR_PRE_RPT_UNION
 ����˵��  : CMI_P23_PRIOR_PRE_RPT�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long mappre_2_cmi                            : 16;     /* CMI����bit 2��Ӧ����������ϱ� */
        unsigned long mappre_3_cmi                            : 16;     /* CMI����bit 3��Ӧ����������ϱ� */
    }stReg;
}HAL_GBBP_CMI_P23_PRIOR_PRE_RPT_UNION;

/*****************************************************************************
 ������    : HAL_GBBP_CMC_P01_PRIOR_PRE_UNION
 ����˵��  : CMC_P01_PRIOR_PRE�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long mappre_0_cmc                            : 16;     /* CMC����bit 0��Ӧ����������ϱ� */
        unsigned long mappre_1_cmc                            : 16;     /* CMC����bit 1��Ӧ����������ϱ� */
    }stReg;
}HAL_GBBP_CMC_P01_PRIOR_PRE_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_CMC_P23_PRIOR_PRE_UNION
 ����˵��  : CMC_P23_PRIOR_PRE�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long mappre_2_cmc                            : 16;     /* CMC����bit 2��Ӧ����������ϱ� */
        unsigned long mappre_3_cmc                            : 16;     /* CMC����bit 3��Ӧ����������ϱ� */
    }stReg;
}HAL_GBBP_CMC_P23_PRIOR_PRE_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_CMC_P23_PRIOR_PRE_UNION
 ����˵��  : CMC_P23_PRIOR_PRE�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_cmi_cmc_mappre_load_en              : 1;      /* 1��ʶ��Ҫ������CMI����CMC����ʷ������ʣ����ź�����֮ǰ��Ҫ���������CPU_P01_MAPPRE_CFG��CPU_P23_MAPPRE_CFG�Ĵ��� */
        unsigned long cpu_cmi_cmc_mappre_load_mode            : 1;      /* 0��ʶ����CMI����ʷ������ʣ�1��ʶ����CMC����ʷ������� */
        unsigned long cpu_nodata_judge_mode                   : 1;      /* 1��ʶ����C02ͨ��״̬���о�NODATA��ģʽ */
        unsigned long                                         : 5;      /* reserve */
        unsigned long dec_cmc_cmi_rpt                         : 1;      /* 1��ʶ��ǰ�����ж�����������CMC���� */
        unsigned long                                         : 7;      /* reserve */
        unsigned long cpu_amr_corr_thres                      : 14;     /* AMR�о��㷨�о������ޣ�AFS�Ƽ�ֵΪ233��AHS�Ƽ�ֵΪ230 */
        unsigned long                                         : 2;      /* reserve */
    }stReg;
}HAL_GBBP_CPU_CMC_CMI_LOAD_EN_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_CPU_P01_MAPPRE_CFG_UNION
 ����˵��  : CPU_P01_MAPPRE_CFG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_p0_mappre                           : 16;     /* CMC */
        unsigned long cpu_p1_mappre                           : 16;     /* CMC */
    }stReg;
}HAL_GBBP_CPU_P01_MAPPRE_CFG_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_CPU_P23_MAPPRE_CFG_UNION
 ����˵��  : CPU_P23_MAPPRE_CFG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;
    struct
    {
        unsigned long cpu_p2_mappre                           : 16;     /* CMC */
        unsigned long cpu_p3_mappre                           : 16;     /* CMC */
    }stReg;
}HAL_GBBP_CPU_P23_MAPPRE_CFG_UNION;


/*--------------------------------------------------------------------------------------------*
 * 1.13 �����ŵ�����ģ��
 *--------------------------------------------------------------------------------------------*/

/*****************************************************************************
 ������    : HAL_GTC_UL_CB1_INDEX_UNION
 ����˵��  : UL_CB1_INDEX�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_ul_cb1_index                        : 7;      /* �������ҵ�������� */
        unsigned short                                         : 9;      /* reserve */
    }stReg;
}HAL_GTC_UL_CB1_INDEX_UNION;


/*****************************************************************************
 ������    : HAL_GTC_UL_CB_NUM_UNION
 ����˵��  : GTC_UL_CB_NUM�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short                                         : 4;      /* reserve */
        unsigned short gsp_ul_cb_start_pos                     : 2;      /* ���д����������ʼλ��ָʾ��־ */
        unsigned short gsp_facch_stolen_flag                   : 2;      /* FACCH͵֡��־ */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_UL_CB_NUM_UNION;

/*****************************************************************************
 ������    : HAL_GTC_IC_PUNC_INF_UNION
 ����˵��  : GTC_IC_PUNC_INF�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_ul_in_band_data_id0                 : 2;      /* [1:0]����ʾ���ڱ���id0��Ϣ */
        unsigned short gsp_ul_in_band_data_id1                 : 2;      /* [1:0]����ʾ���ڱ���id0��Ϣ */
        unsigned short gsp_punc_index_data1                    : 2;      /* [5:4]��ʾDATA1��DATA���Ĵ�׷�ʽ�� */
        unsigned short gsp_punc_index_data2                    : 2;      /* [5:4]��ʾDATA1��DATA���Ĵ�׷�ʽ�� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_IC_PUNC_INF_UNION;


/*****************************************************************************
 ������    : HAL_GTC_RACH_BSIC_UNION
 ����˵��  : GTC_RACH_BSIC�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_rach_bsic_data                      : 6;      /* RACHҵ����BS����վ����BSIC���� */
        unsigned short                                         : 10;     /* reserve */
    }stReg;
}HAL_GTC_RACH_BSIC_UNION;


/*****************************************************************************
 ������    : HAL_GTC_COD_BURST_POS_UNION
 ����˵��  : COD_BURST_POS�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_cod_burst_pos                       : 7;      /* ��ʾ��ǰ�������ݵ�һ��BURST�ڱ���洢RAM��ram611x16s���е�λ�ñ��: */
        unsigned short                                         : 9;      /* reserve */
    }stReg;
}HAL_GTC_COD_BURST_POS_UNION;


/*****************************************************************************
 ������    : HAL_GTC_GSP_UL_BURST_CYCLE_UNION
 ����˵��  : GSP_UL_BURST_CYCLE�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_ul_burst_sav_cycle                  : 6;      /* ���б�����BURSTѭ�����ڣ��ò���ֻ��CS��ҵ����Ч������CS��ҵ��ӳ��ĸ�֡���ڣ�. */
        unsigned short                                         : 1;      /* reserve */
        unsigned short gsp_ul_cod_en                           : 1;      /* ���б�����̵�ʹ���ź� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_GSP_UL_BURST_CYCLE_UNION;


/*****************************************************************************
 ������    : HAL_GTC_GSP_UL_COD_START_UNION
 ����˵��  : GSP_UL_COD_START�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_ul_cod_start                        : 1;      /* ���б��������źţ������ź�,GTC�߼������� */
        unsigned short                                         : 15;     /* reserve */
    }stReg;
}HAL_GTC_GSP_UL_COD_START_UNION;


/*****************************************************************************
 ������    : HAL_GTC_UL_MAP_PARA_UNION
 ����˵��  : UL_MAP_PARA�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_ul_mod_type                         : 1;      /* �������ͱ�־�� */
        unsigned short gsp_ul_burst_type                       : 1;      /* BURST���ͱ�־�� */
        unsigned short gsp_ul_tsc_group_num                    : 4;      /* ����NB��ѵ��������� */
        unsigned short gsp_demod_loop_mode_en                  : 1;      /* 1��ʾ�����н�����ز���ģʽ��0��ʾ�����������е��ƺ����н������ģʽ */
        unsigned short gsp_cpu_wrte_mod                        : 1;      /* 1 ��ʾ��CPU�����Ƶ���ǰRAM���ݵ�д�� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_UL_MAP_PARA_UNION;


/*****************************************************************************
 ������    : HAL_GTC_MAP_BURST_POS_UNION
 ����˵��  : MAP_BURST_POS�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_map_burst_pos                       : 7;      /* ��Ҫ��ȥ��BURST�����ݴ洢λ��ָʾ����ѡ��Ϊ����RAM��ÿ��BURST�洢����ʼ��ַ���ο�gsp_ul_cod_burst_pos[6:0]��λ�ñ�ŷ�ʽ������ͬ���ǣ����ź��·����ǵ�ǰ׼�����Ƶ�BURSTλ�ñ�ţ��������������������ʼBURST��λ�ñ�š��� */
        unsigned short gsp_map_ram_switch_en                   : 1;      /* 1��ʾʹ��MAP��RAM(����ģ����������RAM)ƹ���л�ʹ�� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_MAP_BURST_POS_UNION;


/*****************************************************************************
 ������    : HAL_GTC_MAP_START_UNION
 ����˵��  : MAP_START�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_ul_map_start                        : 1;      /* ��BURSTӳ�������źţ��ߵ�ƽ�����źţ���ǰ���������ź�6.25��Symble��,GTC�߼������� */
        unsigned short                                         : 15;     /* reserve */
    }stReg;
}HAL_GTC_MAP_START_UNION;


/*--------------------------------------------------------------------------------------------*
 * 1.14 ���е���ģ��
 *--------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------*
 * GTC����
 *--------------------------------------------------------------------------------------------*/
/*****************************************************************************
 ������    : HAL_GTC_MOD_START_POS_UNION
 ����˵��  : GTC_MOD_START_POS�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_mod_start_pos                       : 7;      /* bit6~0: ���źű�ʾgsp_mod_start��Ҫ��QB�ڲ���λ����Ϣ(������QB��48����һ�ĵ�λ�ò���) */
        unsigned short gsp_mod_symbol_h_length                 : 1;      /* ��gsp_mod_symbol_l_length[7:0]���9bit�ĵ��Ƴ��ȣ��ñ���Ϊ���λ��gsp_mod_symbol_l_length[7:0]Ϊ��8bit */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_MOD_START_POS_UNION;

/*****************************************************************************
 ������    : HAL_GTC_MOD_SYMBOL_LENGTH_UNION
 ����˵��  : GTC_MOD_SYMBOL_LENGTH�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_mod_symbol_l_length                 : 8;      /* ���Ʒ��Ÿ���(��1��burst�����ڼ��ǳ�������Χ��0��148*2)��������ʱ�����ż�������gsp_mod_symbol_lengthʱ������ģ��ֹͣ��� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_MOD_SYMBOL_LENGTH_UNION;

/*****************************************************************************
 ������    : HAL_GTC_GSP_MOD_TYPE_UNION
 ����˵��  : GSP_MOD_TYPE�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_mod_type                            : 1;      /* �������ͣ� */
        unsigned short                                         : 15;     /* reserve */
    }stReg;
}HAL_GTC_GSP_MOD_TYPE_UNION;


/*****************************************************************************
 ������    : HAL_GTC_GSP_MOD_CTRL_UNION
 ����˵��  : GTC_GSP_MOD_CTRL�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_mod_start                           : 1;      /* bit0: CPU����GTC�������ƣ�һ�����壬������ʱ϶ʱ��CPU����Ҫÿ��ʱ϶������һ�Σ�����ʱ���߼��������Ƿ��Ƕ�ʱ϶. */
        unsigned short gsp_mod_stop                            : 1;      /* bit1: CPU ����GTCֹͣ���ƣ�һ�����塣 */
        unsigned short                                         : 14;     /* reserve */
    }stReg;
}HAL_GTC_GSP_MOD_CTRL_UNION;


/*****************************************************************************
 ������    : HAL_GTC_GSP_ULMOD_TEST_MOD_LOW_UNION
 ����˵��  : GSP_ULMOD_TEST_MOD_LOW�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_ulmod_test_mod_low                  : 8;      /* bit[0]:����ʹ�ܡ�0Ϊԭʼ���ģʽ��1Ϊ����ģʽ */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_GSP_ULMOD_TEST_MOD_LOW_UNION;


/*****************************************************************************
 ������    : HAL_GTC_GSP_ULMOD_TEST_MOD_HIGH_UNION
 ����˵��  : GSP_ULMOD_TEST_MOD_HIGH�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_ulmod_test_mod_high                 : 4;      /* bit[3:0]:Ԥ���Ƴ��ȣ�0��ʾ������Ԥ���ƣ�Ĭ��ֵΪ4�����ƹ����У��ò���Ӧ���� */
        unsigned short                                         : 12;     /* reserve */
    }stReg;
}HAL_GTC_GSP_ULMOD_TEST_MOD_HIGH_UNION;



/*--------------------------------------------------------------------------------------------*
 * CPU����
 *--------------------------------------------------------------------------------------------*/

/*****************************************************************************
 ������    : HAL_GBBP_TX_IQ_MISMATCH_COMP_UNION
 ����˵��  : G_TX_IQ_MISMATCH_COMP�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long dsp_g_tx_amp_comp        : 7;                                /* G TX IQmismatch����У�����ӣ��з������� */
        unsigned long reserved0                : 9;
        unsigned long dsp_g_tx_phase_comp      : 7;                                /* G TX IQmismatch��λУ�����ӣ��з������� */
        unsigned long reserved1                : 9;
    }stReg;
}HAL_GBBP_TX_IQ_MISMATCH_COMP_UNION;

/*****************************************************************************
 ������    : HAL_GBBP_TX_IQ_DC_OFFSET_COMP_UNION
 ����˵��  : G_TX_IQ_DC_OFFSET_COMP1�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long dsp_g_tx_dc_offset_i1    : 12;                                /* G TX IQmismatch I·ֱ��ƫ�ò���ֵ���з����� */
        unsigned long reserved0                : 4;
        unsigned long dsp_g_tx_dc_offset_q1    : 12;                                /* G TX IQmismatch Q·ֱ��ƫ�ò���ֵ���з����� */
        unsigned long reserved1                : 4;
    }stReg;
}HAL_GBBP_TX_IQ_DC_OFFSET_COMP_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_TX_DIG_RMAP_MODE_SEL_UNION
 ����˵��  : TX_DIG_RMAP_MODE_SEL�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_gmsk_digramp_en      : 1;                                /* Gmsk�������� */
        unsigned long                          : 7;
        unsigned long cpu_8psk_digramp_en      : 1;                                /* G TX IQmismatch Q·ֱ��ƫ�ò���ֵ���з����� */
        unsigned long                          : 7;
        unsigned long cpu_ramp_default_value   : 10;                               /* ��������ʱ�����½������apc_rampĬ��ֵ */
        unsigned long                          : 6;
    }stReg;
}HAL_GBBP_TX_DIG_RMAP_MODE_SEL_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_TX_DATA_MULFACTOR_UNION
 ����˵��  : TX_DATA_MULFACTOR�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long cpu_gmsk_tx_data_mulfactor : 10;                             /* GMSK���е��������������� */
        unsigned long                            : 6;
        unsigned long cpu_psk_tx_data_mulfactor  : 10;                             /* 8PSK���е��������������� */
        unsigned long                            : 6;
    }stReg;
}HAL_GBBP_TX_DATA_MULFACTOR_UNION;


/*--------------------------------------------------------------------------------------------*
 * 1.15 DRXģ��
 *--------------------------------------------------------------------------------------------*/

/*****************************************************************************
 ������    : HAL_GBBP_MSR_CNF_UNION
 ����˵��  : MSR_CNF�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long gdrx_msr_frac                           : 11;     /* ˯�߼���С�����֣��ü������Ľ�λ��ʾ1��104Mʱ�ӡ� */
        unsigned long                                         : 5;      /* reserve */
        unsigned long gdrx_msr_integer                        : 7;      /* ˯�߼����������֣�����96��104MHzʱ�Ӹ����� */
        unsigned long                                         : 1;      /* reserve */
        unsigned long gdrx_msr_timebase                       : 6;      /* bit[29:24]˯�߼���96�����֣�ָʾ1��32K�ж��ٸ�96��104M�ӵı������� */
        unsigned long                                         : 2;      /* reserve */
    }stReg;
}HAL_GBBP_MSR_CNF_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_GAUGE_RESULT_RPT_UNION
 ����˵��  : GAUGE_RESULT_RPT�Ĵ�������
*****************************************************************************/

typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long gdrx_gauge_result                       : 28;     /* bit[27:0] ʱ��У׼���������������104�Ӹ��� */
        unsigned long                                         : 2;      /* reserve */
        unsigned long gdrx_gauge_state                        : 1;      /* bit[30] ����״̬��ѯ��1�����ڲ����� */
        unsigned long gdrx_sleep_state                        : 1;      /* bit[31] ˯��״̬��ѯ��1������˯�ߡ� */
    }stReg;
}HAL_GBBP_GAUGE_RESULT_RPT_UNION;


/*****************************************************************************
 ������    : HAL_GBBP_GAUGE_EN_CNF_UNION
 ����˵��  : GAUGE_EN_CNF�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned long uwValue;

    struct
    {
        unsigned long gdrx_gauge_len                          : 17;     /* ʱ��У׼32KHzʱ�Ӹ��� */
        unsigned long                                         : 13;     /* reserve */
        unsigned long gdrx_gauge_en                           : 1;      /* bit[0] ʱ��У׼ʹ�ܣ�д1������1��52M���塣�����㣬д0�����塣 */
        unsigned long gdrx_gauge_stop                         : 1;      /* bit[1] GDRXֹͣ���壬д1ֹͣ��1��52M���塣�����㣬д0�����塣 */
    }stReg;
}HAL_GBBP_GAUGE_EN_CNF_UNION;


/*--------------------------------------------------------------------------------------------*
 * 1.16 �ӽ���ģ��
 *--------------------------------------------------------------------------------------------*/

/*****************************************************************************
 ������    : HAL_GTC_A5_DECIPH_CFG_UNION
 ����˵��  : A5_DECIPH_CFG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_dl_deciph_en                        : 1;      /* ���н���ģ�鹤��ģʽѡ�� */
        unsigned short gsp_dl_gsm_alg_sel                      : 3;      /* ����A51����A52,A53�㷨ѡ�� */
        unsigned short gsp_dl_fn_adj_flag                      : 2;      /* ����֡�ŵ�����־ */
        unsigned short                                         : 10;     /* reserve */
    }stReg;
}HAL_GTC_A5_DECIPH_CFG_UNION;

/*****************************************************************************
 ������    : HAL_GTC_A5_CIPH_CFG_UNION
 ����˵��  : A5_CIPH_CFG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_ul_ciph_en                          : 1;      /* ���н���ģ�鹤��ģʽѡ�� */
        unsigned short gsp_ul_gsm_alg_sel                      : 3;      /* ������A51����A52,A53�㷨ѡ�� */
        unsigned short gsp_ul_fn_adj_flag                      : 2;      /* ����֡�ŵ�����־ */
        unsigned short                                         : 1;      /* reserve */
        unsigned short gsp_ul_a5_ciph_start                    : 1;      /* 1��ʾ��������A5���ܡ�ÿ���м��ܲ������źű������á� */
        unsigned short                                         : 8;      /* reserve */
    }stReg;
}HAL_GTC_A5_CIPH_CFG_UNION;


/*--------------------------------------------------------------------------------------------*
 * 1.17 GAPCģ��
 *--------------------------------------------------------------------------------------------*/

/*****************************************************************************
 ������    : HAL_GTC_PA_CTRL_CFG_UNION
 ����˵��  : PA_CTRL_CFG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gsp_pa_start                            : 1;      /* GSP����APC������1��52M�����壨������frac_cnt=46��������GTC�ڲ����������ø������ź�������APC���RAMP DAC����Ŀ����� */
        unsigned short gsp_pa_stop                             : 1;      /* GSPֹͣAPC�����������źš�д1ֹͣAPC�����������ڲ�����һ��52M���塣���ø������ź�������APC���RAMP DAC����Ŀ����� */
        unsigned short gsp_pa_single_start                     : 1;      /* ����APC���������źš�д1����Ŀ�깦�ʵ�ѹֵΪgsp_pa_init�ĵ���APC�������ڲ�����һ��52M���塣���߼��ڲ�һ�δﵽ��Ŀ�깦�ʵ�ѹֵ�������ø������ź�������APC���TXAGC DAC����Ŀ����� */
        unsigned short gsp_pa_change                           : 1;      /* target������APC�������������źš�д1����һ��target�����õ�APC����������Ӧ���ڶ�ʱ϶���ʱ仯�����ڲ�����һ��52M���壨������frac_cnt=46��������GTC�ڲ��������� */
        unsigned short gsp_pa_dac_sel                          : 1;      /* ����ABB�ڲ�GAPC DAC����ѡ���źš� */
        unsigned short                                         : 11;     /* reserve */
    }stReg;
}HAL_GTC_PA_CTRL_CFG_UNION;

/*****************************************************************************
 ������    : HAL_GTC_PA_LEN_CFG_UNION
 ����˵��  : PA_LEN_HIGH_CFG&PA_LEN_LOW_CFG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;

    struct
    {
        unsigned short low_byte                                : 8;
        unsigned short high_byte                               : 8;
    }stWord;

    struct
    {
        unsigned short gsp_pa_len                              : 13;      /* APC������������ʱ�䳤�ȣ�����ָ�״������µ�Ŀ��ֵgsp_pa_targetʱ��ʼ������ʼ�����µ���ʼֵ��ʱ�����ʱ�䣨����μ�����ʱ��ͼ���źŵĺ��壩����λQb��Ĭ��ֵΪ4��burst���ȡ� */
        unsigned short                                         : 3;     /* reserve */
    }stReg;
}HAL_GTC_PA_LEN_CFG_UNION;


/*****************************************************************************
 ������    : HAL_GTC_PA_TARGET_CFG_UNION
 ����˵��  : PA_TARGET_HIGH_CFG&PA_TARGET_LOW_CFG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;

    struct
    {
        unsigned short low_byte                                : 8;
        unsigned short high_byte                               : 8;
    }stWord;

    struct
    {
        unsigned short gsp_pa_target                           : 10;      /* GSP����APCĿ���ѹֵ�� */
        unsigned short                                         : 3;      /* reserve */
        unsigned short gsp_pa_coeff_index                      : 2;      /* ָʾ�������µ�����ϵ�������ţ�ȷ����gsp_pa_start����gsp_pa_change��Чʱ,��ֵ�Ѿ����á���������ȡֵ��Χ��0��3 */
        unsigned short                                         : 1;      /* reserve */
    }stReg;
}HAL_GTC_PA_TARGET_CFG_UNION;


/*****************************************************************************
 ������    : HAL_GTC_PA_INIT_CFG_UNION
 ����˵��  : PA_INIT_LOW_CFG&PA_INIT_HIGH_CFG�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;

    struct
    {
        unsigned short low_byte                                : 8;
        unsigned short high_byte                               : 8;
    }stWord;


    struct
    {
        unsigned short gsp_pa_init                             : 10;      /* APC���³�ʼ���ʵ�ѹֵ��Ҳ����Ϊ����APC������Ŀ�깦�ʵ�ѹֵ�� */
        unsigned short gsp_ramp_point                          : 5;      /* ָʾ��ǰ���µĵ��������º����µ�����ͬ�� */
        unsigned short                                         : 1;      /* reserve */
    }stReg;
}HAL_GTC_PA_INIT_CFG_UNION;




/*--------------------------------------------------------------------------------------------*
 * 1.18 ˫ģ�л�
 *--------------------------------------------------------------------------------------------*/

/*****************************************************************************
 ������    : HAL_GTC_G2W_CTRL_ADDR_UNION
 ����˵��  : G2W_CTRL_ADDR�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gtc_want_switch                         : 1;      /* �л�W���߿��أ�52M�����ź� */
        unsigned short gtc_wrf_on                              : 1;      /* ��W RF���С�ABB��52M�����ź� */
        unsigned short gtc_wpro_on                             : 1;      /* ָʾW��Ƶ���ȶ����ɴ���С������������������52M�����ź� */
        unsigned short gtc_wrf_spi                             : 1;      /* ����W SPIдspi_data1ʹ�ܣ�52M�����ź� */
        unsigned short gtc_woff                                : 1;      /* �ر�W ABB��RF���л����߿��أ�����W������52M�����ź� */
        unsigned short reserve                                 : 11;     /* reserve */
    }stReg;
}HAL_GTC_G2W_CTRL_ADDR_UNION;


/*****************************************************************************
 ������    : HAL_GTC_W2G_CTRL_ADDR_UNION
 ����˵��  : W2G_CTRL_ADDR�Ĵ�������
*****************************************************************************/
typedef union
{
    unsigned short uhwValue;
    struct
    {
        unsigned short gtc_g2g_switch                          : 1;      /* �л���G���ߣ��л���GTC�ڲ��Ĵ���27�µ�ѡ��5��2M�����ź� */
        unsigned short reserved                                : 15;     /* reserve */
    }stReg;
}HAL_GTC_W2G_CTRL_ADDR_UNION;


/*--------------------------------------------------------------------------------------------*
 * 1.19 ���ز���ģʽ
 *--------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------*
 * 1.20 ���Թܽſ���ģ��
 *--------------------------------------------------------------------------------------------*/




/*--------------------------------------------------------------------------------------------*
 * 1.21 ���ԼĴ���ģ��
 *--------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------*
 * 1.22 GBBP�汾�Ĵ���
 *--------------------------------------------------------------------------------------------*/



/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/




/*****************************************************************************
  10 ��������
*****************************************************************************/











#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


#endif /*  */


