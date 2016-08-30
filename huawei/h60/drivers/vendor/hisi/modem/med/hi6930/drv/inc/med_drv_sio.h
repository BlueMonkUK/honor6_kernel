
#ifndef __MED_DRV_SIO_H__
#define __MED_DRV_SIO_H__
/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "soc_baseaddr_interface.h"
#include "soc_sio_interface.h"
#include "CodecInterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
#if (VOS_CPU_TYPE == VOS_HIFI)
#define DRV_SC_CTRL_ADDR_AUDIO     (SOC_SCTRL_SC_CLKCFGALL0_ADDR(SOC_SC_OFF_BASE_ADDR))    /* ����V9R1��ƣ�VOICE��ʱ�ӿ��ƼĴ���ʹ��SIO1 */
#define DRV_SC_CTRL_ADDR_VOICE     (SOC_SCTRL_SC_CLKCFGALL1_ADDR(SOC_SC_OFF_BASE_ADDR))    /* ϵͳ���ƼĴ��� */
#define DRV_SC_CTRL_ADDR_SMART_PA  (SOC_SCTRL_SC_CLKCFGALL2_ADDR(SOC_SC_OFF_BASE_ADDR))    /* ϵͳ���ƼĴ��� */

#define DRV_SC_CLK_SEL1_ADDR       (SOC_SCTRL_SC_CLK_SEL1_ADDR(SOC_SC_OFF_BASE_ADDR))
#define DRV_SC_CLK_ENABLE_ADDR     (SOC_SCTRL_SC_PERIPH_CLKEN3_ADDR(SOC_SC_OFF_BASE_ADDR))  /* V9R1������ʱ��ʹ�ܼĴ��� */
#define DRV_SC_CLK_DISABLE_ADDR    (SOC_SCTRL_SC_PERIPH_CLKDIS3_ADDR(SOC_SC_OFF_BASE_ADDR)) /* V9R1������ʱ�ӹرռĴ��� */

#define DRV_SIO_BASE_ADDR_AUDIO    VOS_NULL                                     /* SIO����ַ(���ַ) */
#define DRV_SIO_BASE_ADDR_VOICE    (0x900A7000U)                                /* SIO����ַ(���ַ) */
#define DRV_SIO_BASE_ADDR_SMART_PA VOS_NULL                                     /* SIO����ַ(���ַ) */

#else
#define DRV_SC_CTRL_ADDR_AUDIO     ((VOS_UINT32)SOC_SCTRL_SC_CLKCFGALL0_ADDR(&g_auwRegStub[0]))
#define DRV_SC_CTRL_ADDR_VOICE     ((VOS_UINT32)SOC_SCTRL_SC_CLKCFGALL1_ADDR(&g_auwRegStub[0]))
#define DRV_SC_CTRL_ADDR_SMART_PA  ((VOS_UINT32)SOC_SCTRL_SC_CLKCFGALL2_ADDR(&g_auwRegStub[0]))

#define DRV_SC_CLK_SEL1_ADDR       ((VOS_UINT32)SOC_SCTRL_SC_CLK_SEL1_ADDR(&g_auwRegStub[0]))
#define DRV_SC_CLK_ENABLE_ADDR     ((VOS_UINT32)SOC_SCTRL_SC_PERIPH_CLKEN3_ADDR(&g_auwRegStub[0]))
#define DRV_SC_CLK_DISABLE_ADDR    ((VOS_UINT32)SOC_SCTRL_SC_PERIPH_CLKDIS3_ADDR(&g_auwRegStub[0]))

#define DRV_SIO_BASE_ADDR_AUDIO    ((VOS_UINT32)&g_auwRegStub[0x0c00])
#define DRV_SIO_BASE_ADDR_VOICE    ((VOS_UINT32)&g_auwRegStub[0x0d00])
#define DRV_SIO_BASE_ADDR_SMART_PA ((VOS_UINT32)&g_auwRegStub[0x0e00])
#define DRV_SC_CLK_RST_DIS_ADDR    ((VOS_UINT32)&g_auwRegStub[0x0f00])
#define DRV_SIO_MODE_SUBSYS_CTRL   ((VOS_UINT32)&g_auwRegStub[0x0f70])
#endif

#define DRV_SIO_VOICE_MIC_CHANNEL_NUM   (1)                                     /* MicIn  SIOͨ���� */
#define DRV_SIO_VOICE_SPK_CHANNEL_NUM   (1)
#define DRV_SIO_CHANNEL_NUM_PCM         (1)
#define DRV_SIO_CHANNEL_NUM_I2S         (2)


/* V7R2 SIO CTRL3���õ�ַ */
#define DRV_SC_CTRL3_ADDR             ((VOS_UINT32)(DRV_SC_CRG_BASE_ADDR + 0x040c))

/* V7R2 SIO��������bitλ */
#define DRV_SC_SIO_MASTER_MODE_BIT    (UCOM_BIT3)

/* V7R2 SIOʱ�ӷ�Ƶ���ƼĴ������õ�ַ */
#define DRV_CRG_CLKDIV1_ADDR          ((VOS_UINT32)(DRV_SC_CRG_BASE_ADDR + 0x0100))

/* V7R2 SIOʱ��xfs��Ƶbitλ���λ */
#define DRV_CRG_PCM_SYNC_DIV_BIT_LO   (UCOM_BIT16)

/* V7R2 SIOʱ��xfs��Ƶbitλ���λ */
#define DRV_CRG_PCM_SYNC_DIV_BIT_HI   (UCOM_BIT27)

/* V7R2 SIOʱ��CLK��Ƶbitλ���λ */
#define DRV_CRG_PCM_BCLK_DIV_BIT_LO   (UCOM_BIT0)

/* V7R2 SIOʱ��CLK��Ƶbitλ���λ */
#define DRV_CRG_PCM_BCLK_DIV_BIT_HI   (UCOM_BIT15)

/* V7R2 SIO����ʱ��ʹ�����õ�ַ */
#define DRV_CRG_CLKEN3_ADDR           ((VOS_UINT32)(DRV_SC_CRG_BASE_ADDR + 0x0018))

/* V7R2 SIOʹ������bitλ */
#define DRV_SC_LTE_SIO_CLK_EN_BIT     (UCOM_BIT29)

/* V7R2 SIO����ʱ��ʹ�����õ�ַ */
#define DRV_CRG_CLKDIS3_ADDR          ((VOS_UINT32)(DRV_SC_CRG_BASE_ADDR + 0x001c))

/* V7R2 SIO��ʹ������bitλ */
#define DRV_SC_LTE_SIO_CLK_DIS_BIT    (UCOM_BIT29)

/* V7R2 SIO����ʱ��ʹ��״̬��ѯ��ַ */
#define DRV_CRG_CLKSTAT3_ADDR         ((VOS_UINT32)(DRV_SC_CRG_BASE_ADDR + 0x0020))

/* V7R2 SIOʹ��״̬bitλ */
#define DRV_SC_LTE_SIO_CLK_STATUS_BIT (UCOM_BIT29)

/* V7R2 SIO DMA���ƼĴ��� */
#define DRV_SC_CTRL54_ADDR            ((VOS_UINT32)(DRV_SC_CRG_BASE_ADDR + 0x04d8))

/* V7R2 SIO DMAͨ��ѡ��bitλ*/
#define DRV_SC_SIO_DMA_SEL_BIT        (UCOM_BIT2)

/* V7R2 SIO SLAVEʱ��ʱ��ͬ��/�������üĴ��� */
#define DRV_SC_SIO_CLK_POSITIVE       ((VOS_UINT32)(DRV_SC_CRG_BASE_ADDR + 0x0140))
#define DRV_SC_SIO_CLK_POST_SEL_BIT   (UCOM_BIT6)

/*****************************************************************************
 ʵ������  : DRV_SIO_I2S_DUAL_RX_CHN_ADDR
 ��������  : I2S��ȡ�������ݼĴ�����
*****************************************************************************/
#define DRV_SIO_I2S_DUAL_RX_CHN_ADDR(uwAddr)    SOC_SIO_I2S_DUAL_RX_CHN_ADDR(uwAddr)

/*****************************************************************************
 ʵ������  : DRV_SIO_I2S_DUAL_TX_CHN_ADDR
 ��������  : I2Sд�������ݼĴ�����
*****************************************************************************/
#define DRV_SIO_I2S_DUAL_TX_CHN_ADDR(uwAddr)    SOC_SIO_I2S_DUAL_TX_CHN_ADDR(uwAddr)

/*****************************************************************************
 ʵ������  : DRV_SIO_PCM_RD_ADDR
 ��������  : PCM��ȡ�������ݼĴ�����
*****************************************************************************/
#define DRV_SIO_PCM_RD_ADDR(uwAddr)             SOC_SIO_PCM_RD_ADDR(uwAddr)

/*****************************************************************************
 ʵ������  : DRV_SIO_PCM_XD_ADDR
 ��������  : PCMд�������ݼĴ�����
*****************************************************************************/
#define DRV_SIO_PCM_XD_ADDR(uwAddr)             SOC_SIO_PCM_XD_ADDR(uwAddr)

/*****************************************************************************
 ʵ������  : DRV_SIO_GetCfgPtr
 ��������  : ��ȡSIO״̬�Ĵ���ָ��
*****************************************************************************/
#define DRV_SIO_GetAudioCfgPtr()                VOS_NULL//(&g_stDrvSioAudioCfg)�����SIO
#define DRV_SIO_GetVoiceCfgPtr()                (&g_stDrvSioVoiceCfg)
#define DRV_SIO_GetSmartPaCfgPtr()              VOS_NULL//(&g_stDrvSioSmartPaCfg)�����SIO

/*****************************************************************************
 ʵ������  : DRV_SIO_SetSyncMode
 ��������  : ����ͬ��ģʽmaster / slave,
             mode ��������ʹ��DRV_SIO_SYNC_MODE_ENUM_UINT32
*****************************************************************************/
//#define DRV_SIO_SetSyncMode(mode)               (g_stDrvSioVoiceCfg.enSyncMode=(mode))

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
/*****************************************************************************
 ʵ������  : DRV_SIO_INT_MASK_ENUM
 ��������  : SIO�����ж�����
*****************************************************************************/
enum DRV_SIO_INT_MASK_ENUM
{
    DRV_SIO_INT_MASK_RX_INTR                              = 0x1,                /* ����FIFO������ֵ�ж�                 */
    DRV_SIO_INT_MASK_TX_INTR                              = 0x2,                /* ����FIFO������ֵ�ж�                 */
    DRV_SIO_INT_MASK_RX_RIGHT_FIFO_OVER                   = 0x4,                /* I2Sģʽ��Ϊ����������FIFO������ж�  */
                                                                                /* PCMģʽ��ΪPCM����FIFO������ж�     */
    DRV_SIO_INT_MASK_RX_LEFT_FIFO_OVER                    = 0x8,                /* ����������FIFO�����,ֻ��I2Sģʽ��Ч */
    DRV_SIO_INT_MASK_TX_RIGHT_FIFO_UNDER                  = 0x10,               /* I2Sģʽ��Ϊ����������FIFO������ж�  */
                                                                                /* PCMģʽ��ΪPCM����FIFO������ж�     */
    DRV_SIO_INT_MASK_TX_LEFT_FIFO_UNDER                   = 0x20,               /* ����������FIFO�����,ֻ��I2Sģʽ��Ч */
    DRV_SIO_INT_MASK_BUTT                                 = 0x40
};
typedef VOS_UINT32 DRV_SIO_INT_MASK_ENUM_UINT32;

/*****************************************************************************
 ʵ������  : DRV_SIO_CLK_STATUS_ENUM
 ��������  : SIOʱ�ӹ���״̬
*****************************************************************************/
enum DRV_SIO_CLK_STATUS_ENUM
{
    DRV_SIO_CLK_STATUS_CLOSE                                = 0,                /* SIOʱ�ӹر� */
    DRV_SIO_CLK_STATUS_OPEN,                                                    /* SIOʱ�Ӵ� */
    DRV_SIO_CLK_STATUS_BUTT
};
typedef VOS_UINT32 DRV_SIO_CLK_STATUS_ENUM_UINT32;

/*****************************************************************************
 ʵ������  : DRV_SIO_MODE_ENUM
 ��������  : SIO����ģʽ
*****************************************************************************/
enum DRV_SIO_MODE_ENUM
{
    DRV_SIO_MODE_I2S                                        = 0,                /* SIO I2S����ģʽ */
    DRV_SIO_MODE_PCM,                                                           /* SIO PCM����ģʽ */
    DRV_SIO_MODE_BUTT
};
typedef VOS_UINT32 DRV_SIO_MODE_ENUM_UINT32;

/*****************************************************************************
 ʵ������  : DRV_SIO_SAMPLING_ENUM
 ��������  : SIO������
*****************************************************************************/
enum DRV_SIO_SAMPLEING_ENUM
{
    DRV_SIO_SAMPLING_8K                                   = 8000,               /* ������Ϊ8k */
    DRV_SIO_SAMPLING_16K                                  = 16000,              /* ������Ϊ16k */
    DRV_SIO_SAMPLING_48K                                  = 48000,              /* ������Ϊ48k */
    DRV_SIO_SAMPLING_BUTT
};
typedef VOS_UINT32 DRV_SIO_SAMPLING_ENUM_UINT32;

/*****************************************************************************
 ʵ������  : DRV_SIO_SYNC_MODE_ENUM
 ��������  : SIOͬ��ģʽ����
*****************************************************************************/
enum DRV_SIO_SYNC_MODE_ENUM
{
    DRV_SIO_SYNC_MODE_SLAVE                                 = 0,                /* ��ģʽ, ��Ҫģ��CODEC����ʱ���ź� */
    DRV_SIO_SYNC_MODE_MASTER,                                                   /* ��ģʽ, ��Ҫ���ʱ���źŵ�ģ��CODEC */
    DRV_SIO_SYNC_MODE_BUTT
};
typedef VOS_UINT32 DRV_SIO_SYNC_MODE_ENUM_UINT32;


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
 �� �� ��  : DRV_SIO_INT_FUNC
 ��������  : SIO�жϴ���ص�����
 �������  : enIntType - �ж�����, �μ�DRV_SIO_INT_TYPE_ENUM_UINT16
             uwPara    - �û�����
 �������  : ��
 �� �� ֵ  : VOS_VOID
*****************************************************************************/
typedef VOS_VOID (*DRV_SIO_INT_FUNC)(
                     DRV_SIO_INT_MASK_ENUM_UINT32 enIntType,
                     VOS_UINT32                   uwPara);


/*****************************************************************************
 ʵ������  : DRV_SIO_INT_ISR_STRU
 ��������  : ����SIO�жϻص���������ʵ��
*****************************************************************************/
typedef struct
{
    DRV_SIO_INT_MASK_ENUM_UINT32        enIntMask;                              /* SIO�ж����� */
    DRV_SIO_INT_FUNC                    pfFunc;                                 /* �ص�����ָ�� */
    VOS_UINT32                          uwPara;                                 /* �ص�����     */

} DRV_SIO_INT_ISR_STRU;

/*****************************************************************************
 ʵ������  : DRV_SIO_CFG_STRU
 ��������  : ����SIO����ʵ��
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          uwSioBaseAddr;                          /* SIO����ַ�Ĵ��� */
    DRV_SIO_CLK_STATUS_ENUM_UINT32      enClkStatus;                            /* ����SIOʱ�ӿ��� */
    DRV_SIO_MODE_ENUM_UINT32            enWorkMode;                             /* I2S / PCMģʽ���� */
    DRV_SIO_SYNC_MODE_ENUM_UINT32       enSyncMode;                             /* slave / master */
    DRV_SIO_INT_ISR_STRU                stIsr;                                  /* ����SIO�жϻص� */
} DRV_SIO_CFG_STRU;

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern DRV_SIO_CFG_STRU                 g_stDrvSioVoiceCfg;

/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_VOID DRV_SIO_Audio_Close(VOS_VOID);
extern VOS_VOID DRV_SIO_Audio_CloseClk(VOS_VOID);
extern VOS_UINT32 DRV_SIO_Audio_Loop(DRV_SIO_SAMPLING_ENUM_UINT32  enSamplingType);
extern VOS_UINT32 DRV_SIO_Audio_Open(
                       DRV_SIO_INT_MASK_ENUM_UINT32    enIntMask,
                       DRV_SIO_SAMPLING_ENUM_UINT32    enSamplingType,
                       VOS_UINT16                      uhwIsModeMaster,
                       DRV_SIO_INT_FUNC                pfIntHandleFunc,
                       VOS_UINT32                      uwPara);
extern VOS_VOID DRV_SIO_Audio_OpenClk(VOS_VOID);
extern VOS_VOID DRV_SIO_Close(DRV_SIO_CFG_STRU *pstCfg);
extern VOS_UINT32 DRV_SIO_GetRxRegAddr( DRV_SIO_CFG_STRU   *pstCfg );
extern VOS_UINT32 DRV_SIO_GetSioChannelNum( DRV_SIO_CFG_STRU   *pstCfg );
extern VOS_UINT32 DRV_SIO_GetTxRegAddr(DRV_SIO_CFG_STRU   *pstCfg);
extern VOS_UINT32 DRV_SIO_GetVoiceMicChn(VOS_VOID);
extern VOS_UINT32 DRV_SIO_GetVoiceSpkChn(VOS_VOID);
extern VOS_VOID DRV_SIO_Init(VOS_VOID);
extern VOS_UINT32 DRV_SIO_Open(
                       DRV_SIO_CFG_STRU               *pstCfg,
                       DRV_SIO_INT_MASK_ENUM_UINT32    enIntMask,
                       DRV_SIO_INT_FUNC                pfIntHandleFunc,
                       VOS_UINT32                      uwPara);
extern VOS_VOID DRV_SIO_Reset(DRV_SIO_CFG_STRU   *pstCfg);
extern VOS_VOID DRV_SIO_SetMaster(
                       DRV_SIO_CFG_STRU             *pstCfg,
                       DRV_SIO_SAMPLING_ENUM_UINT32  enSamplingRate);
extern VOS_VOID DRV_SIO_SetSlave(DRV_SIO_CFG_STRU   *pstCfg);
extern VOS_VOID DRV_SIO_SmartPa_Close(VOS_VOID);
extern VOS_VOID DRV_SIO_SmartPa_CloseClk(VOS_VOID);
extern VOS_UINT32 DRV_SIO_SmartPa_Open(
                       DRV_SIO_INT_MASK_ENUM_UINT32    enIntMask,
                       DRV_SIO_SAMPLING_ENUM_UINT32    enSamplingType,
                       VOS_UINT16                      uhwIsModeMaster,
                       DRV_SIO_INT_FUNC                pfIntHandleFunc,
                       VOS_UINT32                      uwPara);
extern VOS_VOID DRV_SIO_SmartPa_OpenClk(VOS_VOID);
extern VOS_VOID DRV_SIO_Voice_Close(VOS_VOID);
extern VOS_VOID DRV_SIO_Voice_CloseClk(VOS_VOID);
extern VOS_UINT32 DRV_SIO_Voice_GetWorkMode(VOS_UINT32 uwVoiceChanNum);
extern VOS_UINT32 DRV_SIO_Voice_Open(
                       DRV_SIO_INT_MASK_ENUM_UINT32    enIntMask,
                       DRV_SIO_SAMPLING_ENUM_UINT32    enSamplingType,
                       VOS_UINT16                      uhwIsModeMaster,
                       DRV_SIO_INT_FUNC                pfIntHandleFunc,
                       VOS_UINT32                      uwPara);
extern VOS_VOID DRV_SIO_Voice_OpenClk(VOS_VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of med_drv_sio.h */

