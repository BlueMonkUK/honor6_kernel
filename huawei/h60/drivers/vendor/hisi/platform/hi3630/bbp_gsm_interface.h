

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/

#ifndef __PHY_GBBP_INTERFACE_H__
#define __PHY_GBBP_INTERFACE_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/

/***======================================================================***
                     (1/2) register_define_g_fe_ctrl
 ***======================================================================***/
/* �Ĵ���˵����
 bit[31:15]  ����
 bit[14:0]   ��RFCMOSʹ�þ��巽��ʱ�����gdsp���õ�У׼���¶Ȳ���NV��ֵ��
             ��NV��ֵ�ǿ���ʱ�������¶ȶ�Ӧ�������ߵĲ���ֵ��
   UNION�ṹ:  GBBP_G_AFC_THERM_COM_NV_UNION */
#define GBBP_G_AFC_THERM_COM_NV_ADDR                  (GBBP_BASE_ADDR + 0x8600)

/* �Ĵ���˵����
 bit[31:30]  ����
 bit[29:16]  ʹ�þ���AFC����ʱ�����RF��AFC������ֵ��
 bit[15:12]  ����
 bit[11:4]   ѡ���巽��ʱ�����ø�RF��TCXO SSI��ַbit��
 bit[3:1]    ����
 bit[0:0]    ����
   UNION�ṹ:  GBBP_G_RF_SSI_AFC_UNION */
#define GBBP_G_RF_SSI_AFC_ADDR                        (GBBP_BASE_ADDR + 0x8604)

/* �Ĵ���˵����
 bit[31:29]  ����
 bit[28:28]  ʹ��CPU��ʽ��һ��RF-SSI�����Ƿ�����BBP COMMģ���״̬�źţ������á�
             0����ʾ������1��ʾδ������
 bit[27:25]  ����
 bit[24:24]  GDSPʹ��CPU��ʽ����һ��RF-SSI�������źš�
             �߼������㡣
 bit[23:16]  GDSPʹ��CPU��ʽ����һ��RF-SSI�ļĴ�����ַ��
 bit[15:0]   GDSPʹ��CPU��ʽ����һ��RF-SSI�ļĴ�����Ӧ�����ݡ�
   UNION�ṹ:  GBBP_G_RF_SSI_WRONE_CFG_UNION */
#define GBBP_G_RF_SSI_WRONE_CFG_ADDR                  (GBBP_BASE_ADDR + 0x8608)

/* �Ĵ���˵����
 bit[31:17]  ����
 bit[16:16]  �� ssi����ʹ���ź�
 bit[15:13]  ����
 bit[12:12]  дһ��rf-ssi����ʹ���ź�
 bit[11:9]   ����
 bit[8:8]    g-afc ssi����ʹ���ź�
 bit[7:5]    ����
 bit[4:4]    g-rf-ram ssi����ʹ���ź�
 bit[3:1]    ����
 bit[0:0]    g-gagc ssi����ʹ���ź�
   UNION�ṹ:  GBBP_G_RF_SSI_REQ_EN_CFG_UNION */
#define GBBP_G_RF_SSI_REQ_EN_CFG_ADDR                 (GBBP_BASE_ADDR + 0x860C)

/* �Ĵ���˵����
 bit[31:13]  ����
 bit[12:12]  дssi-ram���ָʾ�źš�
             1����ʾ���ڻض���
             0����ʾ��ɻض�������
 bit[11:9]   ����
 bit[8:8]    �ض����ָʾ�źš�
             1����ʾ���ڻض���
             0����ʾ��ɻض�������
 bit[7:7]    ����
 bit[6:4]    RF-SSI�ض��Ĵ���������
             ����ֵΪ0~7����Ӧ�Ļض��Ĵ�������Ϊ1~8��
             ��ע������Ϊ0��ʾ�ض�һ���Ĵ�����
 bit[3:1]    ����
 bit[0:0]    ����RF-SSI�Ĵ����ض���
             �߼��������źš�
   UNION�ṹ:  GBBP_G_RF_SSI_RD_CFG_UNION */
#define GBBP_G_RF_SSI_RD_CFG_ADDR                     (GBBP_BASE_ADDR + 0x8610)

/* �Ĵ���˵����
 bit[31:24]  ����
 bit[23:0]   SSI�ض���������0��
   UNION�ṹ:  GBBP_G_RF_SSI_RD_0DATA_UNION */
#define GBBP_G_RF_SSI_RD_0DATA_ADDR                   (GBBP_BASE_ADDR + 0x8614)

/* �Ĵ���˵����
 bit[31:24]  ����
 bit[23:0]   SSI�ض���������1��
   UNION�ṹ:  GBBP_G_RF_SSI_RD_1DATA_UNION */
#define GBBP_G_RF_SSI_RD_1DATA_ADDR                   (GBBP_BASE_ADDR + 0x8618)

/* �Ĵ���˵����
 bit[31:24]  ����
 bit[23:0]   SSI�ض���������2��
   UNION�ṹ:  GBBP_G_RF_SSI_RD_2DATA_UNION */
#define GBBP_G_RF_SSI_RD_2DATA_ADDR                   (GBBP_BASE_ADDR + 0x861C)

/* �Ĵ���˵����
 bit[31:24]  ����
 bit[23:0]   SSI�ض���������3��
   UNION�ṹ:  GBBP_G_RF_SSI_RD_3DATA_UNION */
#define GBBP_G_RF_SSI_RD_3DATA_ADDR                   (GBBP_BASE_ADDR + 0x8620)

/* �Ĵ���˵����
 bit[31:24]  ����
 bit[23:0]   SSI�ض���������4��
   UNION�ṹ:  GBBP_G_RF_SSI_RD_4DATA_UNION */
#define GBBP_G_RF_SSI_RD_4DATA_ADDR                   (GBBP_BASE_ADDR + 0x8624)

/* �Ĵ���˵����
 bit[31:24]  ����
 bit[23:0]   SSI�ض���������5��
   UNION�ṹ:  GBBP_G_RF_SSI_RD_5DATA_UNION */
#define GBBP_G_RF_SSI_RD_5DATA_ADDR                   (GBBP_BASE_ADDR + 0x8628)

/* �Ĵ���˵����
 bit[31:24]  ����
 bit[23:0]   SSI�ض���������6��
   UNION�ṹ:  GBBP_G_RF_SSI_RD_6DATA_UNION */
#define GBBP_G_RF_SSI_RD_6DATA_ADDR                   (GBBP_BASE_ADDR + 0x862C)

/* �Ĵ���˵����
 bit[31:24]  ����
 bit[23:0]   SSI�ض���������7��
   UNION�ṹ:  GBBP_G_RF_SSI_RD_7DATA_UNION */
#define GBBP_G_RF_SSI_RD_7DATA_ADDR                   (GBBP_BASE_ADDR + 0x8630)

/* �Ĵ���˵����
 bit[31:5]   ����
 bit[4]      GTC��ʽMIPI�ͻ��˹���ʹ�ܿ��أ�1��ʾͨ���򿪣�0��ʾͨ���رա�
 bit[3:1]    ����
 bit[0]      CPU��ʽMIPI�ͻ��˹���ʹ�ܿ��أ�1��ʾͨ���򿪣�0��ʾͨ���رա�
   UNION�ṹ:  GBBP_G_MIPI_REQ_EN_CFG_UNION */
#define GBBP_G_MIPI_REQ_EN_CFG_ADDR                   (GBBP_BASE_ADDR + 0x8634)

/* �Ĵ���˵����
 bit[31:17]  ����
 bit[16:16]  �����ʽ����MIPI����ʱ�Ĵ��������źš�
 bit[15:14]  ����
 bit[13:8]   �����ʽ����MIPI����ʱ�����üĴ����ĸ��������÷�Χ��1��63��
 bit[7:6]    ����
 bit[5:0]    �����ʽ����MIPI����ʱ�����üĴ�������׵�ַ��
   UNION�ṹ:  GBBP_G_MIPI_CMD_CFG_UNION */
#define GBBP_G_MIPI_CMD_CFG_ADDR                      (GBBP_BASE_ADDR + 0x8638)

/* �Ĵ���˵����
 bit[31:17]  ����
 bit[16]     �������SSI״̬���쳣����ĸ�λ�����źš�
 bit[15:1]   ����
 bit[0]      �������MIPI״̬���쳣����ĸ�λ�����źš�
   UNION�ṹ:  GBBP_G_RF_SSI_MIPI_CLR_REG_UNION */
#define GBBP_G_RF_SSI_MIPI_CLR_REG_ADDR               (GBBP_BASE_ADDR + 0x863C)

/* �Ĵ���˵����
 bit[31:1]   ����
 bit[0]      ABB������������Ȼ�롢����ģʽѡ��
             1����ʾ��Ȼ�룬0����ʾ���룬GSMר�á�
   UNION�ṹ:  GBBP_G_ABB_CFG_SEL_UNION */
#define GBBP_G_ABB_CFG_SEL_ADDR                       (GBBP_BASE_ADDR + 0x8644)

/* �Ĵ���˵����
 bit[31:1]   ����
 bit[0:0]    ��ģ��G�ķ�ʽѡ��
             0�������ѣ�
             1���Ǳ����ѡ�
   UNION�ṹ:  GBBP_G_MASTER_MEA_G_EN_UNION */
#define GBBP_G_MASTER_MEA_G_EN_ADDR                   (GBBP_BASE_ADDR + 0x8658)

/* �Ĵ���˵����
 bit[31:2]   ����
 bit[1:1]    ����ģʽʹ�ܡ�
 bit[0:0]    ����ģʽ�£�����ͨ��ʹ�ܿ��ء�
   UNION�ṹ:  GBBP_G_ABB_TRX_LOOP_EN_UNION */
#define GBBP_G_ABB_TRX_LOOP_EN_ADDR                   (GBBP_BASE_ADDR + 0x865C)

/* �Ĵ���˵����
   ��      ��������
   UNION�ṹ ���� */
#define GBBP_G_DBG_REG0_ADDR                          (GBBP_BASE_ADDR + 0x8660)

/* �Ĵ���˵����
   ��      ��������
   UNION�ṹ ���� */
#define GBBP_G_DBG_REG1_ADDR                          (GBBP_BASE_ADDR + 0x8664)

/* �Ĵ���˵����
 bit[31:27]  ����
 bit[26:16]  AuxDAC�򿪿��������ã��ÿ�������gtc_gapc_en�߿��ź������ط�����AuxDAC��
 bit[15:11]  ����
 bit[10:0]   AuxDAC�򿪿��������ã��ÿ�������gtc_gapc_en�߿��½��ط�����AuxDAC��
   UNION�ṹ:  GBBP_G_AUXDAC_EN_CFG_UNION */
#define GBBP_G_AUXDAC_EN_CFG_ADDR                     (GBBP_BASE_ADDR + 0x8690)



/***======================================================================***
                     (2/2) register_define_g_fe_ctrl_mem
 ***======================================================================***/
/* �Ĵ���˵����g��Ƶ����ƵH��rfic-ssi RAM
 bit[31:24]  ����
 bit[23:16]  g��Ƶ��ǿ���rfic-ssi RAM��
             ���ݣ�
             [23:16]��rfic�ļĴ�����ַ��
 bit[15:0]   g��Ƶ��ǿ���rfic-ssi RAM��
             ����
             [15:0]��rfic�ļĴ������ݡ�
   UNION�ṹ:  GBBP_G_RFIC_SSI_RAM_UNION */
#define GBBP_G_RFIC_SSI_RAM_ADDR                      (GBBP_BASE_ADDR + 0x8200)
#define GBBP_G_RFIC_SSI_RAM_MEMDEPTH  (192)

/* �Ĵ���˵����g��Ƶ����Ƶ��mipi RAM
   ��      ����g��Ƶ����Ƶ��mipi RAM��
   UNION�ṹ ���� */
#define GBBP_G_MIPI_SSI_RAM_ADDR                      (GBBP_BASE_ADDR + 0x8000)
#define GBBP_G_MIPI_SSI_RAM_MEMDEPTH  (64)





/*****************************************************************************
  3 ö�ٶ���
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
  7 UNION����
*****************************************************************************/

/***======================================================================***
                     (1/2) register_define_g_fe_ctrl
 ***======================================================================***/
/*****************************************************************************
 �ṹ��    : GBBP_G_AFC_THERM_COM_NV_UNION
 �ṹ˵��  : G_AFC_THERM_COM_NV �Ĵ����ṹ���塣��ַƫ����:0x0600����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_afc_therm_com_nv_reg;
    struct
    {
        unsigned long  cpu_g_afc_therm_com_nv : 15; /* bit[0-14] : ��RFCMOSʹ�þ��巽��ʱ�����gdsp���õ�У׼���¶Ȳ���NV��ֵ��
                                                                   ��NV��ֵ�ǿ���ʱ�������¶ȶ�Ӧ�������ߵĲ���ֵ�� */
        unsigned long  reserved               : 17; /* bit[15-31]: ���� */
    } reg;
} GBBP_G_AFC_THERM_COM_NV_UNION;
#define GBBP_G_AFC_THERM_COM_NV_cpu_g_afc_therm_com_nv_START  (0)
#define GBBP_G_AFC_THERM_COM_NV_cpu_g_afc_therm_com_nv_END    (14)


/*****************************************************************************
 �ṹ��    : GBBP_G_RF_SSI_AFC_UNION
 �ṹ˵��  : G_RF_SSI_AFC �Ĵ����ṹ���塣��ַƫ����:0x0604����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_rf_ssi_afc_reg;
    struct
    {
        unsigned long  reserved_0          : 1;  /* bit[0-0]  : ���� */
        unsigned long  reserved_1          : 3;  /* bit[1-3]  : ���� */
        unsigned long  cpu_rf_afc_reg_addr : 8;  /* bit[4-11] : ѡ���巽��ʱ�����ø�RF��TCXO SSI��ַbit�� */
        unsigned long  reserved_2          : 4;  /* bit[12-15]: ���� */
        unsigned long  rf_ssi_afc_data_cpu : 14; /* bit[16-29]: ʹ�þ���AFC����ʱ�����RF��AFC������ֵ�� */
        unsigned long  reserved_3          : 2;  /* bit[30-31]: ���� */
    } reg;
} GBBP_G_RF_SSI_AFC_UNION;
#define GBBP_G_RF_SSI_AFC_cpu_rf_afc_reg_addr_START  (4)
#define GBBP_G_RF_SSI_AFC_cpu_rf_afc_reg_addr_END    (11)
#define GBBP_G_RF_SSI_AFC_rf_ssi_afc_data_cpu_START  (16)
#define GBBP_G_RF_SSI_AFC_rf_ssi_afc_data_cpu_END    (29)


/*****************************************************************************
 �ṹ��    : GBBP_G_RF_SSI_WRONE_CFG_UNION
 �ṹ˵��  : G_RF_SSI_WRONE_CFG �Ĵ����ṹ���塣��ַƫ����:0x0608����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_rf_ssi_wrone_cfg_reg;
    struct
    {
        unsigned long  cpu_rf_ssi_wrone_data    : 16; /* bit[0-15] : GDSPʹ��CPU��ʽ����һ��RF-SSI�ļĴ�����Ӧ�����ݡ� */
        unsigned long  cpu_rf_ssi_wrone_addr    : 8;  /* bit[16-23]: GDSPʹ��CPU��ʽ����һ��RF-SSI�ļĴ�����ַ�� */
        unsigned long  cpu_rf_ssi_wrone_ind_imi : 1;  /* bit[24-24]: GDSPʹ��CPU��ʽ����һ��RF-SSI�������źš�
                                                                     �߼������㡣 */
        unsigned long  reserved_0               : 3;  /* bit[25-27]: ���� */
        unsigned long  rf_ssi_wrone_end_cpu     : 1;  /* bit[28-28]: ʹ��CPU��ʽ��һ��RF-SSI�����Ƿ�����BBP COMMģ���״̬�źţ������á�
                                                                     0����ʾ������1��ʾδ������ */
        unsigned long  reserved_1               : 3;  /* bit[29-31]: ���� */
    } reg;
} GBBP_G_RF_SSI_WRONE_CFG_UNION;
#define GBBP_G_RF_SSI_WRONE_CFG_cpu_rf_ssi_wrone_data_START     (0)
#define GBBP_G_RF_SSI_WRONE_CFG_cpu_rf_ssi_wrone_data_END       (15)
#define GBBP_G_RF_SSI_WRONE_CFG_cpu_rf_ssi_wrone_addr_START     (16)
#define GBBP_G_RF_SSI_WRONE_CFG_cpu_rf_ssi_wrone_addr_END       (23)
#define GBBP_G_RF_SSI_WRONE_CFG_cpu_rf_ssi_wrone_ind_imi_START  (24)
#define GBBP_G_RF_SSI_WRONE_CFG_cpu_rf_ssi_wrone_ind_imi_END    (24)
#define GBBP_G_RF_SSI_WRONE_CFG_rf_ssi_wrone_end_cpu_START      (28)
#define GBBP_G_RF_SSI_WRONE_CFG_rf_ssi_wrone_end_cpu_END        (28)


/*****************************************************************************
 �ṹ��    : GBBP_G_RF_SSI_REQ_EN_CFG_UNION
 �ṹ˵��  : G_RF_SSI_REQ_EN_CFG �Ĵ����ṹ���塣��ַƫ����:0x060C����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_rf_ssi_req_en_cfg_reg;
    struct
    {
        unsigned long  cpu_rf_ssi_gagc_en  : 1;  /* bit[0-0]  : g-gagc ssi����ʹ���ź� */
        unsigned long  reserved_0          : 3;  /* bit[1-3]  : ���� */
        unsigned long  cpu_rf_ssi_wrram_en : 1;  /* bit[4-4]  : g-rf-ram ssi����ʹ���ź� */
        unsigned long  reserved_1          : 3;  /* bit[5-7]  : ���� */
        unsigned long  cpu_rf_ssi_afc_en   : 1;  /* bit[8-8]  : g-afc ssi����ʹ���ź� */
        unsigned long  reserved_2          : 3;  /* bit[9-11] : ���� */
        unsigned long  cpu_rf_ssi_wrone_en : 1;  /* bit[12-12]: дһ��rf-ssi����ʹ���ź� */
        unsigned long  reserved_3          : 3;  /* bit[13-15]: ���� */
        unsigned long  cpu_rf_ssi_rd_en    : 1;  /* bit[16-16]: �� ssi����ʹ���ź� */
        unsigned long  reserved_4          : 15; /* bit[17-31]: ���� */
    } reg;
} GBBP_G_RF_SSI_REQ_EN_CFG_UNION;
#define GBBP_G_RF_SSI_REQ_EN_CFG_cpu_rf_ssi_gagc_en_START   (0)
#define GBBP_G_RF_SSI_REQ_EN_CFG_cpu_rf_ssi_gagc_en_END     (0)
#define GBBP_G_RF_SSI_REQ_EN_CFG_cpu_rf_ssi_wrram_en_START  (4)
#define GBBP_G_RF_SSI_REQ_EN_CFG_cpu_rf_ssi_wrram_en_END    (4)
#define GBBP_G_RF_SSI_REQ_EN_CFG_cpu_rf_ssi_afc_en_START    (8)
#define GBBP_G_RF_SSI_REQ_EN_CFG_cpu_rf_ssi_afc_en_END      (8)
#define GBBP_G_RF_SSI_REQ_EN_CFG_cpu_rf_ssi_wrone_en_START  (12)
#define GBBP_G_RF_SSI_REQ_EN_CFG_cpu_rf_ssi_wrone_en_END    (12)
#define GBBP_G_RF_SSI_REQ_EN_CFG_cpu_rf_ssi_rd_en_START     (16)
#define GBBP_G_RF_SSI_REQ_EN_CFG_cpu_rf_ssi_rd_en_END       (16)


/*****************************************************************************
 �ṹ��    : GBBP_G_RF_SSI_RD_CFG_UNION
 �ṹ˵��  : G_RF_SSI_RD_CFG �Ĵ����ṹ���塣��ַƫ����:0x0610����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_rf_ssi_rd_cfg_reg;
    struct
    {
        unsigned long  cpu_rf_ssi_rd_ind_imi : 1;  /* bit[0-0]  : ����RF-SSI�Ĵ����ض���
                                                                  �߼��������źš� */
        unsigned long  reserved_0            : 3;  /* bit[1-3]  : ���� */
        unsigned long  cpu_rf_ssi_rd_num     : 3;  /* bit[4-6]  : RF-SSI�ض��Ĵ���������
                                                                  ����ֵΪ0~7����Ӧ�Ļض��Ĵ�������Ϊ1~8��
                                                                  ��ע������Ϊ0��ʾ�ض�һ���Ĵ����� */
        unsigned long  reserved_1            : 1;  /* bit[7-7]  : ���� */
        unsigned long  rf_ssi_rd_end_cpu     : 1;  /* bit[8-8]  : �ض����ָʾ�źš�
                                                                  1����ʾ���ڻض���
                                                                  0����ʾ��ɻض������� */
        unsigned long  reserved_2            : 3;  /* bit[9-11] : ���� */
        unsigned long  rf_ssi_wrram_end_cpu  : 1;  /* bit[12-12]: дssi-ram���ָʾ�źš�
                                                                  1����ʾ���ڻض���
                                                                  0����ʾ��ɻض������� */
        unsigned long  reserved_3            : 19; /* bit[13-31]: ���� */
    } reg;
} GBBP_G_RF_SSI_RD_CFG_UNION;
#define GBBP_G_RF_SSI_RD_CFG_cpu_rf_ssi_rd_ind_imi_START  (0)
#define GBBP_G_RF_SSI_RD_CFG_cpu_rf_ssi_rd_ind_imi_END    (0)
#define GBBP_G_RF_SSI_RD_CFG_cpu_rf_ssi_rd_num_START      (4)
#define GBBP_G_RF_SSI_RD_CFG_cpu_rf_ssi_rd_num_END        (6)
#define GBBP_G_RF_SSI_RD_CFG_rf_ssi_rd_end_cpu_START      (8)
#define GBBP_G_RF_SSI_RD_CFG_rf_ssi_rd_end_cpu_END        (8)
#define GBBP_G_RF_SSI_RD_CFG_rf_ssi_wrram_end_cpu_START   (12)
#define GBBP_G_RF_SSI_RD_CFG_rf_ssi_wrram_end_cpu_END     (12)


/*****************************************************************************
 �ṹ��    : GBBP_G_RF_SSI_RD_0DATA_UNION
 �ṹ˵��  : G_RF_SSI_RD_0DATA �Ĵ����ṹ���塣��ַƫ����:0x0614����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_rf_ssi_rd_0data_reg;
    struct
    {
        unsigned long  cpu_rfic_ssi_rd_0data : 24; /* bit[0-23] : SSI�ض���������0�� */
        unsigned long  reserved              : 8;  /* bit[24-31]: ���� */
    } reg;
} GBBP_G_RF_SSI_RD_0DATA_UNION;
#define GBBP_G_RF_SSI_RD_0DATA_cpu_rfic_ssi_rd_0data_START  (0)
#define GBBP_G_RF_SSI_RD_0DATA_cpu_rfic_ssi_rd_0data_END    (23)


/*****************************************************************************
 �ṹ��    : GBBP_G_RF_SSI_RD_1DATA_UNION
 �ṹ˵��  : G_RF_SSI_RD_1DATA �Ĵ����ṹ���塣��ַƫ����:0x0618����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_rf_ssi_rd_1data_reg;
    struct
    {
        unsigned long  cpu_rfic_ssi_rd_1data : 24; /* bit[0-23] : SSI�ض���������1�� */
        unsigned long  reserved              : 8;  /* bit[24-31]: ���� */
    } reg;
} GBBP_G_RF_SSI_RD_1DATA_UNION;
#define GBBP_G_RF_SSI_RD_1DATA_cpu_rfic_ssi_rd_1data_START  (0)
#define GBBP_G_RF_SSI_RD_1DATA_cpu_rfic_ssi_rd_1data_END    (23)


/*****************************************************************************
 �ṹ��    : GBBP_G_RF_SSI_RD_2DATA_UNION
 �ṹ˵��  : G_RF_SSI_RD_2DATA �Ĵ����ṹ���塣��ַƫ����:0x061C����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_rf_ssi_rd_2data_reg;
    struct
    {
        unsigned long  cpu_rfic_ssi_rd_2data : 24; /* bit[0-23] : SSI�ض���������2�� */
        unsigned long  reserved              : 8;  /* bit[24-31]: ���� */
    } reg;
} GBBP_G_RF_SSI_RD_2DATA_UNION;
#define GBBP_G_RF_SSI_RD_2DATA_cpu_rfic_ssi_rd_2data_START  (0)
#define GBBP_G_RF_SSI_RD_2DATA_cpu_rfic_ssi_rd_2data_END    (23)


/*****************************************************************************
 �ṹ��    : GBBP_G_RF_SSI_RD_3DATA_UNION
 �ṹ˵��  : G_RF_SSI_RD_3DATA �Ĵ����ṹ���塣��ַƫ����:0x0620����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_rf_ssi_rd_3data_reg;
    struct
    {
        unsigned long  cpu_rfic_ssi_rd_3data : 24; /* bit[0-23] : SSI�ض���������3�� */
        unsigned long  reserved              : 8;  /* bit[24-31]: ���� */
    } reg;
} GBBP_G_RF_SSI_RD_3DATA_UNION;
#define GBBP_G_RF_SSI_RD_3DATA_cpu_rfic_ssi_rd_3data_START  (0)
#define GBBP_G_RF_SSI_RD_3DATA_cpu_rfic_ssi_rd_3data_END    (23)


/*****************************************************************************
 �ṹ��    : GBBP_G_RF_SSI_RD_4DATA_UNION
 �ṹ˵��  : G_RF_SSI_RD_4DATA �Ĵ����ṹ���塣��ַƫ����:0x0624����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_rf_ssi_rd_4data_reg;
    struct
    {
        unsigned long  cpu_rfic_ssi_rd_4data : 24; /* bit[0-23] : SSI�ض���������4�� */
        unsigned long  reserved              : 8;  /* bit[24-31]: ���� */
    } reg;
} GBBP_G_RF_SSI_RD_4DATA_UNION;
#define GBBP_G_RF_SSI_RD_4DATA_cpu_rfic_ssi_rd_4data_START  (0)
#define GBBP_G_RF_SSI_RD_4DATA_cpu_rfic_ssi_rd_4data_END    (23)


/*****************************************************************************
 �ṹ��    : GBBP_G_RF_SSI_RD_5DATA_UNION
 �ṹ˵��  : G_RF_SSI_RD_5DATA �Ĵ����ṹ���塣��ַƫ����:0x0628����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_rf_ssi_rd_5data_reg;
    struct
    {
        unsigned long  cpu_rfic_ssi_rd_5data : 24; /* bit[0-23] : SSI�ض���������5�� */
        unsigned long  reserved              : 8;  /* bit[24-31]: ���� */
    } reg;
} GBBP_G_RF_SSI_RD_5DATA_UNION;
#define GBBP_G_RF_SSI_RD_5DATA_cpu_rfic_ssi_rd_5data_START  (0)
#define GBBP_G_RF_SSI_RD_5DATA_cpu_rfic_ssi_rd_5data_END    (23)


/*****************************************************************************
 �ṹ��    : GBBP_G_RF_SSI_RD_6DATA_UNION
 �ṹ˵��  : G_RF_SSI_RD_6DATA �Ĵ����ṹ���塣��ַƫ����:0x062C����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_rf_ssi_rd_6data_reg;
    struct
    {
        unsigned long  cpu_rfic_ssi_rd_6data : 24; /* bit[0-23] : SSI�ض���������6�� */
        unsigned long  reserved              : 8;  /* bit[24-31]: ���� */
    } reg;
} GBBP_G_RF_SSI_RD_6DATA_UNION;
#define GBBP_G_RF_SSI_RD_6DATA_cpu_rfic_ssi_rd_6data_START  (0)
#define GBBP_G_RF_SSI_RD_6DATA_cpu_rfic_ssi_rd_6data_END    (23)


/*****************************************************************************
 �ṹ��    : GBBP_G_RF_SSI_RD_7DATA_UNION
 �ṹ˵��  : G_RF_SSI_RD_7DATA �Ĵ����ṹ���塣��ַƫ����:0x0630����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_rf_ssi_rd_7data_reg;
    struct
    {
        unsigned long  cpu_rfic_ssi_rd_7data : 24; /* bit[0-23] : SSI�ض���������7�� */
        unsigned long  reserved              : 8;  /* bit[24-31]: ���� */
    } reg;
} GBBP_G_RF_SSI_RD_7DATA_UNION;
#define GBBP_G_RF_SSI_RD_7DATA_cpu_rfic_ssi_rd_7data_START  (0)
#define GBBP_G_RF_SSI_RD_7DATA_cpu_rfic_ssi_rd_7data_END    (23)


/*****************************************************************************
 �ṹ��    : GBBP_G_MIPI_REQ_EN_CFG_UNION
 �ṹ˵��  : G_MIPI_REQ_EN_CFG �Ĵ����ṹ���塣��ַƫ����:0x0634����ֵ:0x00000011�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_mipi_req_en_cfg_reg;
    struct
    {
        unsigned long  cpu_mipi_cmd_cfg_en       : 1;  /* bit[0]   : CPU��ʽMIPI�ͻ��˹���ʹ�ܿ��أ�1��ʾͨ���򿪣�0��ʾͨ���رա� */
        unsigned long  reserved_0                : 3;  /* bit[1-3] : ���� */
        unsigned long  cpu_gtc_mipi_start_cfg_en : 1;  /* bit[4]   : GTC��ʽMIPI�ͻ��˹���ʹ�ܿ��أ�1��ʾͨ���򿪣�0��ʾͨ���رա� */
        unsigned long  reserved_1                : 27; /* bit[5-31]: ���� */
    } reg;
} GBBP_G_MIPI_REQ_EN_CFG_UNION;
#define GBBP_G_MIPI_REQ_EN_CFG_cpu_mipi_cmd_cfg_en_START        (0)
#define GBBP_G_MIPI_REQ_EN_CFG_cpu_mipi_cmd_cfg_en_END          (0)
#define GBBP_G_MIPI_REQ_EN_CFG_cpu_gtc_mipi_start_cfg_en_START  (4)
#define GBBP_G_MIPI_REQ_EN_CFG_cpu_gtc_mipi_start_cfg_en_END    (4)


/*****************************************************************************
 �ṹ��    : GBBP_G_MIPI_CMD_CFG_UNION
 �ṹ˵��  : G_MIPI_CMD_CFG �Ĵ����ṹ���塣��ַƫ����:0x0638����ֵ:0x00000100�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_mipi_cmd_cfg_reg;
    struct
    {
        unsigned long  cpu_mipi_cmd_ini_addr : 6;  /* bit[0-5]  : �����ʽ����MIPI����ʱ�����üĴ�������׵�ַ�� */
        unsigned long  reserved_0            : 2;  /* bit[6-7]  : ���� */
        unsigned long  cpu_mipi_cmd_num      : 6;  /* bit[8-13] : �����ʽ����MIPI����ʱ�����üĴ����ĸ��������÷�Χ��1��63�� */
        unsigned long  reserved_1            : 2;  /* bit[14-15]: ���� */
        unsigned long  cpu_mipi_cmd_ind_imi  : 1;  /* bit[16-16]: �����ʽ����MIPI����ʱ�Ĵ��������źš� */
        unsigned long  reserved_2            : 15; /* bit[17-31]: ���� */
    } reg;
} GBBP_G_MIPI_CMD_CFG_UNION;
#define GBBP_G_MIPI_CMD_CFG_cpu_mipi_cmd_ini_addr_START  (0)
#define GBBP_G_MIPI_CMD_CFG_cpu_mipi_cmd_ini_addr_END    (5)
#define GBBP_G_MIPI_CMD_CFG_cpu_mipi_cmd_num_START       (8)
#define GBBP_G_MIPI_CMD_CFG_cpu_mipi_cmd_num_END         (13)
#define GBBP_G_MIPI_CMD_CFG_cpu_mipi_cmd_ind_imi_START   (16)
#define GBBP_G_MIPI_CMD_CFG_cpu_mipi_cmd_ind_imi_END     (16)


/*****************************************************************************
 �ṹ��    : GBBP_G_RF_SSI_MIPI_CLR_REG_UNION
 �ṹ˵��  : G_RF_SSI_MIPI_CLR_REG �Ĵ����ṹ���塣��ַƫ����:0x063C����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_rf_ssi_mipi_clr_reg_reg;
    struct
    {
        unsigned long  cpu_mipi_clr_ind_imi   : 1;  /* bit[0]    : �������MIPI״̬���쳣����ĸ�λ�����źš� */
        unsigned long  reserved_0             : 15; /* bit[1-15] : ���� */
        unsigned long  cpu_rf_ssi_clr_ind_imi : 1;  /* bit[16]   : �������SSI״̬���쳣����ĸ�λ�����źš� */
        unsigned long  reserved_1             : 15; /* bit[17-31]: ���� */
    } reg;
} GBBP_G_RF_SSI_MIPI_CLR_REG_UNION;
#define GBBP_G_RF_SSI_MIPI_CLR_REG_cpu_mipi_clr_ind_imi_START    (0)
#define GBBP_G_RF_SSI_MIPI_CLR_REG_cpu_mipi_clr_ind_imi_END      (0)
#define GBBP_G_RF_SSI_MIPI_CLR_REG_cpu_rf_ssi_clr_ind_imi_START  (16)
#define GBBP_G_RF_SSI_MIPI_CLR_REG_cpu_rf_ssi_clr_ind_imi_END    (16)


/*****************************************************************************
 �ṹ��    : GBBP_G_ABB_CFG_SEL_UNION
 �ṹ˵��  : G_ABB_CFG_SEL �Ĵ����ṹ���塣��ַƫ����:0x0644����ֵ:0x00000001�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_abb_cfg_sel_reg;
    struct
    {
        unsigned long  cpu_abb_code_sel_g : 1;  /* bit[0]   : ABB������������Ȼ�롢����ģʽѡ��
                                                              1����ʾ��Ȼ�룬0����ʾ���룬GSMר�á� */
        unsigned long  reserved           : 31; /* bit[1-31]: ���� */
    } reg;
} GBBP_G_ABB_CFG_SEL_UNION;
#define GBBP_G_ABB_CFG_SEL_cpu_abb_code_sel_g_START  (0)
#define GBBP_G_ABB_CFG_SEL_cpu_abb_code_sel_g_END    (0)


/*****************************************************************************
 �ṹ��    : GBBP_G_MASTER_MEA_G_EN_UNION
 �ṹ˵��  : G_MASTER_MEA_G_EN �Ĵ����ṹ���塣��ַƫ����:0x0658����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_master_mea_g_en_reg;
    struct
    {
        unsigned long  cpu_master_mea_g_en : 1;  /* bit[0-0] : ��ģ��G�ķ�ʽѡ��
                                                               0�������ѣ�
                                                               1���Ǳ����ѡ� */
        unsigned long  reserved            : 31; /* bit[1-31]: ���� */
    } reg;
} GBBP_G_MASTER_MEA_G_EN_UNION;
#define GBBP_G_MASTER_MEA_G_EN_cpu_master_mea_g_en_START  (0)
#define GBBP_G_MASTER_MEA_G_EN_cpu_master_mea_g_en_END    (0)


/*****************************************************************************
 �ṹ��    : GBBP_G_ABB_TRX_LOOP_EN_UNION
 �ṹ˵��  : G_ABB_TRX_LOOP_EN �Ĵ����ṹ���塣��ַƫ����:0x065C����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_abb_trx_loop_en_reg;
    struct
    {
        unsigned long  cpu_gabb_trx_loop_tx_en : 1;  /* bit[0-0] : ����ģʽ�£�����ͨ��ʹ�ܿ��ء� */
        unsigned long  cpu_gabb_trx_loop_en    : 1;  /* bit[1-1] : ����ģʽʹ�ܡ� */
        unsigned long  reserved                : 30; /* bit[2-31]: ���� */
    } reg;
} GBBP_G_ABB_TRX_LOOP_EN_UNION;
#define GBBP_G_ABB_TRX_LOOP_EN_cpu_gabb_trx_loop_tx_en_START  (0)
#define GBBP_G_ABB_TRX_LOOP_EN_cpu_gabb_trx_loop_tx_en_END    (0)
#define GBBP_G_ABB_TRX_LOOP_EN_cpu_gabb_trx_loop_en_START     (1)
#define GBBP_G_ABB_TRX_LOOP_EN_cpu_gabb_trx_loop_en_END       (1)


/*****************************************************************************
 �ṹ��    : GBBP_G_AUXDAC_EN_CFG_UNION
 �ṹ˵��  : G_AUXDAC_EN_CFG �Ĵ����ṹ���塣��ַƫ����:0x0690����ֵ:0x00000000�����:32
 �Ĵ���˵��: 
*****************************************************************************/
typedef union
{
    unsigned long      g_auxdac_en_cfg_reg;
    struct
    {
        unsigned long  cpu_g_auxdac_close_cfg : 11; /* bit[0-10] : AuxDAC�򿪿��������ã��ÿ�������gtc_gapc_en�߿��½��ط�����AuxDAC�� */
        unsigned long  reserved_0             : 5;  /* bit[11-15]: ���� */
        unsigned long  cpu_g_auxdac_open_cfg  : 11; /* bit[16-26]: AuxDAC�򿪿��������ã��ÿ�������gtc_gapc_en�߿��ź������ط�����AuxDAC�� */
        unsigned long  reserved_1             : 5;  /* bit[27-31]: ���� */
    } reg;
} GBBP_G_AUXDAC_EN_CFG_UNION;
#define GBBP_G_AUXDAC_EN_CFG_cpu_g_auxdac_close_cfg_START  (0)
#define GBBP_G_AUXDAC_EN_CFG_cpu_g_auxdac_close_cfg_END    (10)
#define GBBP_G_AUXDAC_EN_CFG_cpu_g_auxdac_open_cfg_START   (16)
#define GBBP_G_AUXDAC_EN_CFG_cpu_g_auxdac_open_cfg_END     (26)


/***======================================================================***
                     (2/2) register_define_g_fe_ctrl_mem
 ***======================================================================***/
/*****************************************************************************
 �ṹ��    : GBBP_G_RFIC_SSI_RAM_UNION
 �ṹ˵��  : G_RFIC_SSI_RAM �Ĵ����ṹ���塣��ַƫ����:0x200����ֵ:0x00000000�����:32
 �Ĵ���˵��: g��Ƶ����ƵH��rfic-ssi RAM
*****************************************************************************/
typedef union
{
    unsigned long      g_rfic_ssi_ram_reg;
    struct
    {
        unsigned long  g_rfic_ssi_ram_data : 16; /* bit[0-15] : g��Ƶ��ǿ���rfic-ssi RAM��
                                                                ����
                                                                [15:0]��rfic�ļĴ������ݡ� */
        unsigned long  g_rfic_ssi_ram_addr : 8;  /* bit[16-23]: g��Ƶ��ǿ���rfic-ssi RAM��
                                                                ���ݣ�
                                                                [23:16]��rfic�ļĴ�����ַ�� */
        unsigned long  reserved            : 8;  /* bit[24-31]: ���� */
    } reg;
} GBBP_G_RFIC_SSI_RAM_UNION;
#define GBBP_G_RFIC_SSI_RAM_g_rfic_ssi_ram_data_START  (0)
#define GBBP_G_RFIC_SSI_RAM_g_rfic_ssi_ram_data_END    (15)
#define GBBP_G_RFIC_SSI_RAM_g_rfic_ssi_ram_addr_START  (16)
#define GBBP_G_RFIC_SSI_RAM_g_rfic_ssi_ram_addr_END    (23)




/*****************************************************************************
  8 OTHERS����
*****************************************************************************/



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

#endif /* end of phy_gbbp_interface.h */
