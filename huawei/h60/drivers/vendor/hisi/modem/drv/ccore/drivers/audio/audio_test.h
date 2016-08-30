

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#ifndef __AUDIO_TEST_H__
#define __AUDIO_TEST__

#include "bsp_memmap.h"
#include "drv_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define DRV_DMA_BASE_ADDR               (HI_EDMA_REGBASE_ADDR_VIRT)                           /* DMA��V7R1�еĻ���ַ */
#define DRV_SC_DMA_SEL_CTRL_ADDR        (0x900004E0U)                           /* V7R1��DMAѡ����ƼĴ��� */

/*****************************************************************************
  2 �궨��
*****************************************************************************/
/* 2.1 �ⲿֱ�����ö��� */
#define DRV_DMA_LLI_LINK(uwAddr)        (((uwAddr) & 0xffffffe0UL) | (0x2UL))   /* ��������LLI������ */
#define DRV_DMA_LLI_CHAN(uhwChanNo)     (((unsigned int)(uhwChanNo) << 8)|(0x1UL))/* ͨ������LLI������ */
#define DRV_DMA_LLI_DISABLE             (0x0UL)                                 /* ����ģʽ��ʹ�������� */

#define DRV_DMA_MEM_SIO_CFG             (0x83311057)                            /* ���͵�MEM->SIO��DMA���ã�Դ��ַ������16bitλ��burst4 */
#define DRV_DMA_SIO_MEM_CFG             (0x43311047)                            /* ���͵�SIO->MEM��DMA���ã�Ŀ�ĵ�ַ������16bitλ��burst4 */
#define DRV_DMA_MEM_MEM_CFG             (0xfff33003)                            /* ���͵�MEM->MEM��DMA���ã���ַ˫�������64bitλ��burst1  */

/* 2.2 �ڲ����ö��� */
#define DRV_DMA_MAX_CHANNEL_NUM         (16)                                    /* ���ͨ����                              */

#define DRV_DMA_CPU_NO                  (0)//(2)                               /* EDMA�ϴ��������                    */
#define DRV_DMA_INT_STAT_CPU(i)             (DRV_DMA_BASE_ADDR+(       (0x40*i)))  /* ������i���ж�״̬�Ĵ���                 */
#define DRV_DMA_INT_TC1_CPU(i)              (DRV_DMA_BASE_ADDR+(0x0004+(0x40*i)))  /* ������i��ͨ����������ж�״̬�Ĵ���     */
#define DRV_DMA_INT_TC2_CPU(i)              (DRV_DMA_BASE_ADDR+(0x0008+(0x40*i)))  /* ������i������ڵ㴫������ж�״̬�Ĵ��� */
#define DRV_DMA_INT_ERR1_CPU(i)             (DRV_DMA_BASE_ADDR+(0x000c+(0x40*i)))  /* ������i�����ô����ж�״̬�Ĵ���         */
#define DRV_DMA_INT_ERR2_CPU(i)             (DRV_DMA_BASE_ADDR+(0x0010+(0x40*i)))  /* ������i�����ݴ�������ж�״̬�Ĵ���     */
#define DRV_DMA_INT_ERR3_CPU(i)             (DRV_DMA_BASE_ADDR+(0x0014+(0x40*i)))  /* ������i�Ķ���������ж�״̬�Ĵ���       */
#define DRV_DMA_INT_TC1_MASK_CPU(i)         (DRV_DMA_BASE_ADDR+(0x0018+(0x40*i)))  /* ������i��ͨ����������ж����μĴ���     */
#define DRV_DMA_INT_TC2_MASK_CPU(i)         (DRV_DMA_BASE_ADDR+(0x001c+(0x40*i)))  /* ������i������ڵ㴫������ж����μĴ��� */
#define DRV_DMA_INT_ERR1_MASK_CPU(i)        (DRV_DMA_BASE_ADDR+(0x0020+(0x40*i)))  /* ������i�����ô����ж����μĴ���         */
#define DRV_DMA_INT_ERR2_MASK_CPU(i)        (DRV_DMA_BASE_ADDR+(0x0024+(0x40*i)))  /* ������i�����ݴ�������ж����μĴ���     */
#define DRV_DMA_INT_ERR3_MASK_CPU(i)        (DRV_DMA_BASE_ADDR+(0x0028+(0x40*i)))  /* ������i�������ȡ�����ж����μĴ���     */
#define DRV_DMA_INT_TC1_RAW             (DRV_DMA_BASE_ADDR+(0x0600))            /* ԭʼͨ����������ж�״̬�Ĵ���          */
#define DRV_DMA_INT_TC2_RAW             (DRV_DMA_BASE_ADDR+(0x0608))            /* ԭʼ����ڵ㴫������ж�״̬�Ĵ���      */
#define DRV_DMA_INT_ERR1_RAW            (DRV_DMA_BASE_ADDR+(0x0610))            /* ԭʼ���ô����ж�״̬�Ĵ���              */
#define DRV_DMA_INT_ERR2_RAW            (DRV_DMA_BASE_ADDR+(0x0618))            /* ԭʼ���ݴ�������ж�״̬�Ĵ���          */
#define DRV_DMA_INT_ERR3_RAW            (DRV_DMA_BASE_ADDR+(0x0620))            /* ԭʼ�����ȡ�����ж�״̬�Ĵ���          */
#define DRV_DMA_SREQ                    (DRV_DMA_BASE_ADDR+(0x0660))            /* ����������Ĵ���                        */
#define DRV_DMA_LSREQ                   (DRV_DMA_BASE_ADDR+(0x0664))            /* ĩ�ε���������Ĵ���                    */
#define DRV_DMA_BREQ                    (DRV_DMA_BASE_ADDR+(0x0668))            /* ͻ����������Ĵ���                      */
#define DRV_DMA_LBREQ                   (DRV_DMA_BASE_ADDR+(0x066C))            /* ĩ��ͻ����������Ĵ���                  */
#define DRV_DMA_FREQ                    (DRV_DMA_BASE_ADDR+(0x0670))            /* ������������Ĵ���                      */
#define DRV_DMA_LFREQ                   (DRV_DMA_BASE_ADDR+(0x0674))            /* ĩ��������������Ĵ���                  */
#define DRV_DMA_CH_PRI                  (DRV_DMA_BASE_ADDR+(0x0688))            /* ���ȼ����ƼĴ���                        */
#define DRV_DMA_CH_STAT                 (DRV_DMA_BASE_ADDR+(0x0690))            /* ȫ��DMA״̬�Ĵ���                       */
#define DRV_DMA_DMA_CTRL                (DRV_DMA_BASE_ADDR+(0x0698))            /* DMAȫ�ֿ��ƼĴ���                       */
#define DRV_DMA_CX_CURR_CNT1(j)         (DRV_DMA_BASE_ADDR+(0x0700+(0x10*j)))  /* ͨ��j����ά����ʣ��size״̬�Ĵ���       */
#define DRV_DMA_CX_CURR_CNT0(j)         (DRV_DMA_BASE_ADDR+(0x0704+(0x10*j)))  /* ͨ��j��һ����ά����ʣ��size״̬�Ĵ���   */
#define DRV_DMA_CX_CURR_SRC_ADDR(j)     (DRV_DMA_BASE_ADDR+(0x0708+(0x10*j)))  /* ͨ��j��Դ��ַ�Ĵ���                     */
#define DRV_DMA_CX_CURR_DES_ADDR(j)     (DRV_DMA_BASE_ADDR+(0x070C+(0x10*j)))  /* ͨ��j��Ŀ�ĵ�ַ�Ĵ���                   */
#define DRV_DMA_CX_LLI(j)               (DRV_DMA_BASE_ADDR+(0x0800+(0x40*j)))  /* ͨ��j�������ַ�Ĵ���                   */
#define DRV_DMA_CX_BINDX(j)             (DRV_DMA_BASE_ADDR+(0x0804+(0x40*j)))  /* ͨ��j�Ķ�ά��ַƫ�������üĴ���         */
#define DRV_DMA_CX_CINDX(j)             (DRV_DMA_BASE_ADDR+(0x0808+(0x40*j)))  /* ͨ��j����ά��ַƫ�������üĴ���         */
#define DRV_DMA_CX_CNT1(j)              (DRV_DMA_BASE_ADDR+(0x080C+(0x40*j)))  /* ͨ��j�Ĵ��䳤��1���üĴ���              */
#define DRV_DMA_CX_CNT0(j)              (DRV_DMA_BASE_ADDR+(0x0810+(0x40*j)))  /* ͨ��j�Ĵ��䳤�����üĴ���               */
#define DRV_DMA_CX_SRC_ADDR(j)          (DRV_DMA_BASE_ADDR+(0x0814+(0x40*j)))  /* ͨ��j��Դ��ַ�Ĵ���                     */
#define DRV_DMA_CX_DES_ADDR(j)          (DRV_DMA_BASE_ADDR+(0x0818+(0x40*j)))  /* ͨ��j��Ŀ�ĵ�ַ�Ĵ���                   */
#define DRV_DMA_CX_CONFIG(j)            (DRV_DMA_BASE_ADDR+(0x081C+(0x40*j)))  /* ͨ��j�����üĴ���                       */
#define DRV_DMA_CX_AXI_CONF(j)          (DRV_DMA_BASE_ADDR+(0x0820+(0x40*j)))  /* ͨ��j��AXI����������üĴ���            */

#define DRV_DMA_INT_STAT           (DRV_DMA_INT_STAT_CPU(DRV_DMA_CPU_NO))        
#define DRV_DMA_INT_TC1            (DRV_DMA_INT_TC1_CPU(DRV_DMA_CPU_NO))          
#define DRV_DMA_INT_TC2            (DRV_DMA_INT_TC2_CPU(DRV_DMA_CPU_NO))          
#define DRV_DMA_INT_ERR1           (DRV_DMA_INT_ERR1_CPU(DRV_DMA_CPU_NO))        
#define DRV_DMA_INT_ERR2           (DRV_DMA_INT_ERR2_CPU(DRV_DMA_CPU_NO))         
#define DRV_DMA_INT_ERR3           (DRV_DMA_INT_ERR3_CPU(DRV_DMA_CPU_NO))        
#define DRV_DMA_INT_TC1_MASK       (DRV_DMA_INT_TC1_MASK_CPU(DRV_DMA_CPU_NO))     
#define DRV_DMA_INT_TC2_MASK       (DRV_DMA_INT_TC2_MASK_CPU(DRV_DMA_CPU_NO))    
#define DRV_DMA_INT_ERR1_MASK      (DRV_DMA_INT_ERR1_MASK_CPU(DRV_DMA_CPU_NO))    
#define DRV_DMA_INT_ERR2_MASK      (DRV_DMA_INT_ERR2_MASK_CPU(DRV_DMA_CPU_NO))   
#define DRV_DMA_INT_ERR3_MASK      (DRV_DMA_INT_ERR3_MASK_CPU(DRV_DMA_CPU_NO))   

#define DRV_DMA_GetCxIsrPtr()           (&g_astDrvDmaCxIntIsr[0])               /* ��ȡDMA�жϴ�����ȫ�ֱ����׵�ַ */

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
 �� �� ��  : DRV_DMA_INT_TYPE_ENUM
 ��������  : DMAͨ���ж�����
*****************************************************************************/
typedef enum DRV_DMA_INT_TYPE_ENUM
{
    DRV_DMA_INT_TYPE_TC1 = 0,           /* ͨ�����     */
    DRV_DMA_INT_TYPE_TC2,               /* ���������(�����һ�����) */
    DRV_DMA_INT_TYPE_ERR1,              /* ���ô���     */
    DRV_DMA_INT_TYPE_ERR2,              /* ���ݴ������ */
    DRV_DMA_INT_TYPE_ERR3,              /* ��ȡ������� */
    DRV_DMA_INT_TYPE_BUTT
}drv_dma_int_type;

/*ת����32���ض�����*/
#define DMA_REG_READ(uwAddr)                    (*((volatile unsigned int *)(uwAddr)))

/*ת����32����д����*/
#define DMA_REG_WRITE(uwAddr, uwValue)         (*((volatile unsigned int *)(uwAddr)) = uwValue)

/*ת���尴���ض�����*/
#define DMA_REG_READBIT(uwAddr, uhwStartBit, uhwEndBit)               \
    BSP_REG_GETBITS(uwAddr, 0,  uhwStartBit, uhwEndBit - uhwStartBit)

/*ת���尴����д����*/
#define DMA_REG_WRITEBIT(uwAddr, uhwStartBit, uhwEndBit, swContent)    \
    BSP_REG_SETBITS(uwAddr, 0, uhwStartBit, uhwEndBit - uhwStartBit, swContent)

typedef enum DMA_BIT_ENUM
{
    DMA_BIT0 = 0,
    DMA_BIT1 ,
    DMA_BIT2 ,
    DMA_BIT3 ,
    DMA_BIT4 ,
    DMA_BIT5 ,
    DMA_BIT6 ,
    DMA_BIT7 ,
    DMA_BIT8 ,
    DMA_BIT9 ,
    DMA_BIT10,
    DMA_BIT11,
    DMA_BIT12,
    DMA_BIT13,
    DMA_BIT14,
    DMA_BIT15,
    DMA_BIT16,
    DMA_BIT17,
    DMA_BIT18,
    DMA_BIT19,
    DMA_BIT20,
    DMA_BIT21,
    DMA_BIT22,
    DMA_BIT23,
    DMA_BIT24,
    DMA_BIT25,
    DMA_BIT26,
    DMA_BIT27,
    DMA_BIT28,
    DMA_BIT29,
    DMA_BIT30,
    DMA_BIT31,
    DMA_BIT_BUTT
}dma_bit;

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
 ʵ������  : DRV_DMA_CXCFG_STRU
 ��������  : EMACͨ������ʵ��

LLI�Ĵ�������:
   b31-5, �����ַb31-5, ��32byte����
   b11-8, ����ͨ����
   b1-0,  ͨ������ʹ��, 00-��ʹ��;01-ͨ������;10-��������;

 config�Ĵ�������:
   b31 b30 b29   b28  b27-24 b23-20 b18-16 b14-12 b9-4   b3-2      b1    b0
   si  di smode dmode   sl     dl    sw     dw    peri flow_ctrl itc_en ch_en
 ����,
   sl/dl, src/dest burst len, ȡֵ0000-1111��ʾ����Ϊ1-16
   sw/dw, src/dest width, ȡֵ000-101��ʾ���Ϊ8/16/32/64/128/256bit
   flow_ctrl, 00-M2M;01-M2P;10-M2P��������
*****************************************************************************/
typedef struct
{
   unsigned int       uwLli;              /* ͨ��Linked List Item��ַ     */
   unsigned short       uhwDstBIndex;       /* ͨ���Ķ�άĿ�ĵ�ַƫ�������ã�ռ��16bit */
   unsigned short       uhwSrcBIndex;       /* ͨ���Ķ�άԴ��ַƫ�������ã�ռ��16bit */
   unsigned short       uhwDstCIndex;       /* ͨ������άĿ�ĵ�ַƫ�������ã�ռ��16bit */
   unsigned short       uhwSrcCIndex;       /* ͨ������άԴ��ַƫ�������ã�ռ��16bit */
   unsigned short       uhwCCount;          /* ��ά���䳤������,��λFrame��ռ��16bit   */
   unsigned short       uhwRsv;             /* ����λ��ռ��16bit                    */
   unsigned short       uhwACount;          /* һά���䳤������,��λByte��ռ��16bit */
   unsigned short       uhwBCount;          /* ��ά���䳤������,��λArray��ռ��16bit  */
   unsigned int       uwSrcAddr;          /* ͨ��Դ��ַ                   */
   unsigned int       uwDstAddr;          /* ͨ��Ŀ�ĵ�ַ                 */
   unsigned int       uwConfig;           /* ͨ�����üĴ���               */

} DRV_DMA_CXCFG_STRU __attribute__((aligned(32)));

/*****************************************************************************
 �� �� ��  : drv_dma_int_func
 ��������  : DMA�жϴ���ص�����
 �������  : enIntType - �ж�����, �μ�drv_dma_int_type
             uwPara    - �û�����
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
typedef void (*drv_dma_int_func)(
                drv_dma_int_type enIntType,
                unsigned int                   uwPara);

/*****************************************************************************
 ʵ������  : DRV_DMA_CXISR_STRU
 ��������  : ����DMAͨ���ص���������ʵ��
*****************************************************************************/
typedef struct
{
    drv_dma_int_func    pfFunc;         /* �ص�����ָ�� */
    unsigned int          uwPara;         /* �ص�����     */

} DRV_DMA_CXISR_STRU;

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/

void bsp_dma_init( void );
void bsp_dma_isr(void);
unsigned int bsp_dma_startwithcfg(
                       unsigned short              uhwChannelNo,
                       DRV_DMA_CXCFG_STRU     *pstCfg,
                       drv_dma_int_func        pfIntHandleFunc,
                       unsigned int              uwPara);
void bsp_dma_stop(unsigned short uhwChannelNo);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of drv_dma.h */
