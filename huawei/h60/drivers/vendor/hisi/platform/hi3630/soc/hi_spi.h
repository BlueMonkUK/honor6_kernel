/******************************************************************************/
/*  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_spi.h */
/* Version       : 2.0 */
/* Created       : 2013-02-21*/
/* Last Modified : */
/* Description   :  The C union definition file for the module spi*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_SPI_H__
#define __HI_SPI_H__

/*
 * Project: hi
 * Module : spi
 */

#ifndef HI_SET_GET
#define HI_SET_GET(a0,a1,a2,a3,a4)
#endif

/********************************************************************************/
/*    spi �Ĵ���ƫ�ƶ��壨��Ŀ��_ģ����_�Ĵ�����_OFFSET)        */
/********************************************************************************/
#define    HI_SPI_CTRLR0_OFFSET                              (0x0) /* ���ƼĴ���0�� */
#define    HI_SPI_CTRLR1_OFFSET                              (0x4) /* ���ƼĴ���1�� */
#define    HI_SPI_SSIENR_OFFSET                              (0x8) /* SSIʹ�ܼĴ����� */
#define    HI_SPI_MWCR_OFFSET                                (0xC) /* Microwire���ƼĴ����� */
#define    HI_SPI_SER_OFFSET                                 (0x10) /* slaveʹ�ܼĴ����� */
#define    HI_SPI_BAUDR_OFFSET                               (0x14) /* ������ѡ�� */
#define    HI_SPI_TXFTLR_OFFSET                              (0x18) /* ����FIFO��ֵ�� */
#define    HI_SPI_RXFTLR_OFFSET                              (0x1C) /* ����FIFO��ֵ�� */
#define    HI_SPI_TXFLR_OFFSET                               (0x20) /* ����FIFOˮ�߼Ĵ����� */
#define    HI_SPI_RXFLR_OFFSET                               (0x24) /* ����FIFOˮ�߼Ĵ����� */
#define    HI_SPI_SR_OFFSET                                  (0x28) /* ״̬�Ĵ����� */
#define    HI_SPI_IMR_OFFSET                                 (0x2C) /* �ж����μĴ����� */
#define    HI_SPI_ISR_OFFSET                                 (0x30) /* �ж�״̬�Ĵ����� */
#define    HI_SPI_RISR_OFFSET                                (0x34) /* ԭʼ�жϼĴ����� */
#define    HI_SPI_TXOICR_OFFSET                              (0x38) /* ����FIFO����ж�����Ĵ����� */
#define    HI_SPI_RXOICR_OFFSET                              (0x3C) /* ����FIFO����ж�����Ĵ����� */
#define    HI_SPI_RXUICR_OFFSET                              (0x40) /* ����FIFO�����ж�����Ĵ����� */
#define    HI_SPI_MSTICR_OFFSET                              (0x44) /* ��master��ͻ�ж�����Ĵ����� */
#define    HI_SPI_ICR_OFFSET                                 (0x48) /* �ж�����Ĵ����� */
#define    HI_SPI_DMACR_OFFSET                               (0x4C) /* DMA���ƼĴ����� */
#define    HI_SPI_DMATDLR_OFFSET                             (0x50) /* DMA��������ˮ�ߡ� */
#define    HI_SPI_DMARDLR_OFFSET                             (0x54) /* DMA��������ˮ�ߡ� */
#define    HI_SPI_IDR_OFFSET                                 (0x58) /* ID�Ĵ����� */
#define    HI_SPI_SSI_COMP_VERSION_OFFSET                    (0x5C) /* �汾�Ĵ����� */
#define    HI_SPI_DR_OFFSET                                  (0x60) /* ���ݼĴ����� */
#define    HI_SPI_RX_SAMPLE_DLY_OFFSET                       (0xF0) /* ����ʱ���ӳټĴ��� */

/********************************************************************************/
/*    spi �Ĵ������壨��Ŀ��_ģ����_�Ĵ�����_T)        */
/********************************************************************************/
typedef union
{
    struct
    {
        unsigned int    DFS                        : 4; /* [3..0] ����֡��С������ֵ������ڵ���3����������ֵ�޷����� */
        unsigned int    FRF                        : 2; /* [5..4] ֡��ʽ��00��Motorola SPI01��Texas Instruments SSP10��National Semiconductors Microwire11��Reserved */
        unsigned int    SCPH                       : 1; /* [6..6] ����ʱ����λѡ��0������ʱ���ڵ�һ������λ�д�����1������ʱ���ڵ�һ������λ��ʼ��������SSI_HC_FRF=1��SSI_DFLT_FRF=0ʱ��SCPHֻ���� */
        unsigned int    SCPOL                      : 1; /* [7..7] ����ʱ�Ӽ���ѡ��0������ʱ�ӵ͵�ƽ��Ч��1������ʱ�Ӹߵ�ƽ��Ч����SSI_HC_FRF=1��SSI_DFLT_FRF=0ʱ��SCPOLֻ���� */
        unsigned int    TMOD                       : 2; /* [9..8] ����ģʽ��ѡ����ģʽ��00������&����01������10������11��EEPROM�� */
        unsigned int    SLV_OE                     : 1; /* [10..10] SLAVE���ʹ�ܣ�����ӦDW_apb_ssi����Ϊ����slave�豸��������Ϊ����master��Ч��0��ʹ��Slave txd1����ʹ��Slave txd */
        unsigned int    SRL                        : 1; /* [11..11] ��λ�Ĵ���ѭ��ʹ�ܣ������ã��ڲ����ӷ�����λ�Ĵ�������ͽ�����λ�Ĵ������롣 */
        unsigned int    CFS                        : 4; /* [15..12] ����֡��С��ѡ��Microwire֡��ʽ�����ֳ���0~2:����3~15:n+1λ�������ݴ��䣬n����CFS */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_CTRLR0_T;    /* ���ƼĴ���0�� */

typedef union
{
    struct
    {
        unsigned int    NDF                        : 16; /* [15..0] ��������֡��Ŀ��Ϊndf+1 */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_CTRLR1_T;    /* ���ƼĴ���1�� */

typedef union
{
    struct
    {
        unsigned int    SSI_EN                     : 1; /* [0..0] SSIʹ�ܡ� */
        unsigned int    reserved                   : 31; /* [31..1] ������ */
    } bits;
    unsigned int    u32;
}HI_SPI_SSIENR_T;    /* SSIʹ�ܼĴ����� */

typedef union
{
    struct
    {
        unsigned int    MWMOD                      : 1; /* [0..0] Microwire����ģʽ��0�������д��䡣1�����д��䡣 */
        unsigned int    MDD                        : 1; /* [1..1] Microwire���ơ���ʹ��Microwire����Э��ʱ�������ݷ���0������ģʽ��1������ģʽ�� */
        unsigned int    MHS                        : 1; /* [2..2] Microwire���֡�����ӦDW_apb_ssi���óɴ���master�豸ʱ��0�����ֽӿڲ�ʹ�ܡ�1�����ֽӿ�ʹ�ܡ� */
        unsigned int    reserved_1                 : 1; /* [3..3] ������ */
        unsigned int    mw_toggle_cyc              : 4; /* [7..4] д�������л����ڡ�0��0���л����ڡ�1��1���л����ڡ�2��2���л����ڡ����� */
        unsigned int    reserved_0                 : 24; /* [31..8] ������ */
    } bits;
    unsigned int    u32;
}HI_SPI_MWCR_T;    /* Microwire���ƼĴ����� */

typedef union
{
    struct
    {
        unsigned int    SER                        : 4; /* [3..0] Slaveѡ��ʹ�ܱ�־��ÿһλ��ʾһ��Slave��0��û��ѡ��1��ѡ�� */
        unsigned int    reserved                   : 28; /* [31..4] ������ */
    } bits;
    unsigned int    u32;
}HI_SPI_SER_T;    /* slaveʹ�ܼĴ����� */

typedef union
{
    struct
    {
        unsigned int    SCKDV                      : 16; /* [15..0] SSIʱ�ӷ�Ƶ�ȣ���֧��������Ƶ����0λ�Զ����Σ��Ա�֤��Ƶ��Ϊż�������磺����SCKDV=3��2'b11��,ʵ�ʷ�Ƶ��Ϊ2(2'b10) */
        unsigned int    reserved                   : 16; /* [31..16] ������ */
    } bits;
    unsigned int    u32;
}HI_SPI_BAUDR_T;    /* ������ѡ�� */

typedef union
{
    struct
    {
        unsigned int    TFT                        : 8; /* [7..0] ����FIFO�ż�����FIFO��������Ŀ����TFT�������жϡ� */
        unsigned int    reserved                   : 24; /* [31..8] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_TXFTLR_T;    /* ����FIFO��ֵ�� */

typedef union
{
    struct
    {
        unsigned int    RFT                        : 3; /* [2..0] ����FIFO�ż�����FIFO��������Ŀ����RFT�������жϡ� */
        unsigned int    reserved_1                 : 5; /* [7..3] �������ñ������ضν�ֹд���0ֵ�� */
        unsigned int    reserved_0                 : 24; /* [31..8] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_RXFTLR_T;    /* ����FIFO��ֵ�� */

typedef union
{
    struct
    {
        unsigned int    TXTFL                      : 9; /* [8..0] ����FIFO��������Ŀ�� */
        unsigned int    reserved                   : 23; /* [31..9] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_TXFLR_T;    /* ����FIFOˮ�߼Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    RXTFL                      : 4; /* [3..0] ����FIFO��������Ŀ�� */
        unsigned int    reserved                   : 28; /* [31..4] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_RXFLR_T;    /* ����FIFOˮ�߼Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    BUSY                       : 1; /* [0..0] SSIæ��־��0��DW_apb_ssi���л�δʹ��1��DW_apb_ssi���ڰ������� */
        unsigned int    TFNF                       : 1; /* [1..1] ����FIFOδ����0������FIFO��1������FIFOδ�� */
        unsigned int    TFE                        : 1; /* [2..2] ����FIFO�ա�0������FIFO�ǿ�1������FIFO�� */
        unsigned int    RFNE                       : 1; /* [3..3] ����FIFO�ǿա�0������FIFO��1������FIFO�ǿ� */
        unsigned int    RFF                        : 1; /* [4..4] ����FIFO����������FIFO������ȫ��ʱ��1��������0��0������FIFOδ��1������FIFO�� */
        unsigned int    TXE                        : 1; /* [5..5] ���ʹ��󡣵�������������FIFO��������TXE��1��0���޴�1�����ͳ����slaveģʽʱʹ�� */
        unsigned int    DCOL                       : 1; /* [6..6] ���ݳ�ͻ���󡣽���ӦDW_apb_ssi����Ϊmasterʱ������masterѡ����豸��Ϊslave��0���޴���1���������ݳ�ͻ���� */
        unsigned int    reserved                   : 25; /* [31..7] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_SR_T;    /* ״̬�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    TXEIM                      : 1; /* [0..0] ����FIFO���ж����Ρ�0��ssi_txe_intr�жϱ�����1��ssi_txe_intr�ж�û������ */
        unsigned int    TXOIM                      : 1; /* [1..1] ����FIFO����ж�����0��ssi_txo_intr�жϱ�����1��ssi_txo_intr�ж�û������ */
        unsigned int    RXUIM                      : 1; /* [2..2] ����FIFO������ж����Ρ�0��ssi_rxu_intr�жϱ�����1��ssi_rxu_intr�ж�û������ */
        unsigned int    RXOIM                      : 1; /* [3..3] ����FIFO����ж����Ρ�0��ssi_rxo_intr�жϱ�����1��ssi_rxo_intr�ж�û������ */
        unsigned int    RXFIM                      : 1; /* [4..4] ����FIFO���ж����Ρ�0��ssi_rxf_intr�жϱ�����1��ssi_rxf_intr�ж�û������ */
        unsigned int    MSTIM                      : 1; /* [5..5] ��master�����ж����Ρ�0��ssi_mst_intr�жϱ�����1��ssi_mst_intr�ж�δ���� */
        unsigned int    reserved                   : 26; /* [31..6] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_IMR_T;    /* �ж����μĴ����� */

typedef union
{
    struct
    {
        unsigned int    TXEIS                      : 1; /* [0..0] ����FIFO���ж�״̬��0��ssi_txe_intr�ж����κ�û�м���1��ssi_txe_intr�ж����κ󼤻� */
        unsigned int    TXOIS                      : 1; /* [1..1] ����FIFO����ж�״̬0��ssi_txo_intr�ж����κ�û�м���1��ssi_txo_intr�ж����κ󼤻� */
        unsigned int    RXUIS                      : 1; /* [2..2] ����FIFO������ж�״̬��0��ssi_rxo_intr�ж����κ�û�м���1��ssi_rxo_intr�ж����κ󼤻� */
        unsigned int    RXOIS                      : 1; /* [3..3] ����FIFO������ж�״̬��0��ssi_rxu_intr�ж����κ�û�м���1��ssi_rxu_intr�ж����κ󼤻� */
        unsigned int    RXFIS                      : 1; /* [4..4] ����FIFO���ж�״̬��0��ssi_rxf_intr�ж����κ�û�м���1��ssi_rxf_intr�ж����κ󼤻� */
        unsigned int    MSTIS                      : 1; /* [5..5] ��master�����ж�״̬��0��ssi_mst_intr�ж����κ�û�м���1��ssi_mst_intr�ж����κ󼤻� */
        unsigned int    reserved                   : 26; /* [31..6] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_ISR_T;    /* �ж�״̬�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    TXEIR                      : 1; /* [0..0] ����FIFO�����ж�״̬��0��ssi_txe_intr�ж�����ǰû�м���1��ssi_txe_intr�ж�����ǰ���� */
        unsigned int    TXOIR                      : 1; /* [1..1] ����FIFO������ж�״̬0��ssi_txo_intr�ж�����ǰû�м���1��ssi_txo_intr�ж�����ǰ���� */
        unsigned int    RXUIR                      : 1; /* [2..2] ����FIFO��������ж�״̬��0��ssi_rxu_intr�ж�����ǰû�м���1��ssi_rxu_intr�ж�����ǰ���� */
        unsigned int    RXOIR                      : 1; /* [3..3] ����FIFO������ж�״̬��0��ssi_rxo_intr�ж�����ǰû�м���1��ssi_rxo_intr�ж�����ǰ���� */
        unsigned int    RXFIR                      : 1; /* [4..4] ����FIFO�����ж�״̬��0��ssi_mst_intr�ж�����ǰû�м���1��ssi_mst_intr�ж�����ǰ���� */
        unsigned int    MSTIR                      : 1; /* [5..5] ��master�������ж�״̬��0��ssi_mst_intr�ж�����ǰû�м���1��ssi_mst_intr�ж�����ǰ���� */
        unsigned int    reserved                   : 26; /* [31..6] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_RISR_T;    /* ԭʼ�жϼĴ����� */

typedef union
{
    struct
    {
        unsigned int    TXOICR                     : 1; /* [0..0] ����0����FIFO����ж�ssi_txo_intr�� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_TXOICR_T;    /* ����FIFO����ж�����Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    RXOICR                     : 1; /* [0..0] ����0����FIFO����ж�ssi_rxo_intr�� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_RXOICR_T;    /* ����FIFO����ж�����Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    RXUICR                     : 1; /* [0..0] ����0����FIFO������ж�ssi_rxu_intr�� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_RXUICR_T;    /* ����FIFO�����ж�����Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    MSTICR                     : 1; /* [0..0] ����0��master�����ж�ssi_mst_intr�� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_MSTICR_T;    /* ��master��ͻ�ж�����Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    ICR                        : 1; /* [0..0] ����0�ж�ssi_txo_intr��ssi_rxu_intr��ssi_rxo_intr��ssi_mst_intr�� */
        unsigned int    reserved                   : 31; /* [31..1] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_ICR_T;    /* �ж�����Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    RDMAE                      : 1; /* [0..0] ����DMAʹ��0������DMA��ʹ��1������DMAʹ�� */
        unsigned int    TDMAE                      : 1; /* [1..1] ����DMAʹ��0������DMA��ʹ��1������DMAʹ�� */
        unsigned int    PMI0_IND                   : 30; /* [31..2] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_DMACR_T;    /* DMA���ƼĴ����� */

typedef union
{
    struct
    {
        unsigned int    DMATDL                     : 3; /* [2..0] ��������ˮƽ�ߡ���С�ڵ���DMATDL����Ч���ݽ��뷢��FIFO������TDMAE��1������dma_tx_req���� */
        unsigned int    reserved_1                 : 5; /* [7..3] �������ñ������ضν�ֹд���0ֵ�� */
        unsigned int    reserved_0                 : 24; /* [31..8] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_DMATDLR_T;    /* DMA��������ˮ�ߡ� */

typedef union
{
    struct
    {
        unsigned int    DMARDL                     : 3; /* [2..0] ��������ˮƽ�ߡ������ڵ���DMARDL��1����Ч���ݽ������FIFO������TDMAE��1������dma_tx_req���� */
        unsigned int    reserved_1                 : 5; /* [7..3] �������ñ������ضν�ֹд���0ֵ�� */
        unsigned int    reserved_0                 : 24; /* [31..8] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_DMARDLR_T;    /* DMA��������ˮ�ߡ� */

typedef union
{
    struct
    {
        unsigned int    IDCODE                     : 32; /* [31..0] ʶ���롣 */
    } bits;
    unsigned int    u32;
}HI_SPI_IDR_T;    /* ID�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    SSI_COMP_VERSION           : 32; /* [31..0] synopsys�����汾����ʾ4λASCII�롣 */
    } bits;
    unsigned int    u32;
}HI_SPI_SSI_COMP_VERSION_T;    /* �汾�Ĵ����� */

typedef union
{
    struct
    {
        unsigned int    DR                         : 16; /* [15..0] ���ݼĴ�����дʱ�����ҵ�������ʱ�Զ�������дʱDRΪ����FIFO���档��ʱDRΪ����FIFO���档 */
        unsigned int    reserved                   : 16; /* [31..16] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_DR_T;    /* ���ݼĴ����� */

typedef union
{
    struct
    {
        unsigned int    RSD                        : 8; /* [7..0] master�ض������ӳ���������0�����ӳ�1���ض������ӳ�1��ssi_clk2���ض������ӳ�2��ssi_clk���� */
        unsigned int    reserved                   : 24; /* [31..8] ���� */
    } bits;
    unsigned int    u32;
}HI_SPI_RX_SAMPLE_DLY_T;    /* ����ʱ���ӳټĴ��� */


/********************************************************************************/
/*    spi ��������Ŀ��_ģ����_�Ĵ�����_��Ա��_set)        */
/********************************************************************************/
HI_SET_GET(hi_spi_ctrlr0_dfs,dfs,HI_SPI_CTRLR0_T,HI_SPI_BASE_ADDR, HI_SPI_CTRLR0_OFFSET)
HI_SET_GET(hi_spi_ctrlr0_frf,frf,HI_SPI_CTRLR0_T,HI_SPI_BASE_ADDR, HI_SPI_CTRLR0_OFFSET)
HI_SET_GET(hi_spi_ctrlr0_scph,scph,HI_SPI_CTRLR0_T,HI_SPI_BASE_ADDR, HI_SPI_CTRLR0_OFFSET)
HI_SET_GET(hi_spi_ctrlr0_scpol,scpol,HI_SPI_CTRLR0_T,HI_SPI_BASE_ADDR, HI_SPI_CTRLR0_OFFSET)
HI_SET_GET(hi_spi_ctrlr0_tmod,tmod,HI_SPI_CTRLR0_T,HI_SPI_BASE_ADDR, HI_SPI_CTRLR0_OFFSET)
HI_SET_GET(hi_spi_ctrlr0_slv_oe,slv_oe,HI_SPI_CTRLR0_T,HI_SPI_BASE_ADDR, HI_SPI_CTRLR0_OFFSET)
HI_SET_GET(hi_spi_ctrlr0_srl,srl,HI_SPI_CTRLR0_T,HI_SPI_BASE_ADDR, HI_SPI_CTRLR0_OFFSET)
HI_SET_GET(hi_spi_ctrlr0_cfs,cfs,HI_SPI_CTRLR0_T,HI_SPI_BASE_ADDR, HI_SPI_CTRLR0_OFFSET)
HI_SET_GET(hi_spi_ctrlr0_reserved,reserved,HI_SPI_CTRLR0_T,HI_SPI_BASE_ADDR, HI_SPI_CTRLR0_OFFSET)
HI_SET_GET(hi_spi_ctrlr1_ndf,ndf,HI_SPI_CTRLR1_T,HI_SPI_BASE_ADDR, HI_SPI_CTRLR1_OFFSET)
HI_SET_GET(hi_spi_ctrlr1_reserved,reserved,HI_SPI_CTRLR1_T,HI_SPI_BASE_ADDR, HI_SPI_CTRLR1_OFFSET)
HI_SET_GET(hi_spi_ssienr_ssi_en,ssi_en,HI_SPI_SSIENR_T,HI_SPI_BASE_ADDR, HI_SPI_SSIENR_OFFSET)
HI_SET_GET(hi_spi_ssienr_reserved,reserved,HI_SPI_SSIENR_T,HI_SPI_BASE_ADDR, HI_SPI_SSIENR_OFFSET)
HI_SET_GET(hi_spi_mwcr_mwmod,mwmod,HI_SPI_MWCR_T,HI_SPI_BASE_ADDR, HI_SPI_MWCR_OFFSET)
HI_SET_GET(hi_spi_mwcr_mdd,mdd,HI_SPI_MWCR_T,HI_SPI_BASE_ADDR, HI_SPI_MWCR_OFFSET)
HI_SET_GET(hi_spi_mwcr_mhs,mhs,HI_SPI_MWCR_T,HI_SPI_BASE_ADDR, HI_SPI_MWCR_OFFSET)
HI_SET_GET(hi_spi_mwcr_reserved_1,reserved_1,HI_SPI_MWCR_T,HI_SPI_BASE_ADDR, HI_SPI_MWCR_OFFSET)
HI_SET_GET(hi_spi_mwcr_mw_toggle_cyc,mw_toggle_cyc,HI_SPI_MWCR_T,HI_SPI_BASE_ADDR, HI_SPI_MWCR_OFFSET)
HI_SET_GET(hi_spi_mwcr_reserved_0,reserved_0,HI_SPI_MWCR_T,HI_SPI_BASE_ADDR, HI_SPI_MWCR_OFFSET)
HI_SET_GET(hi_spi_ser_ser,ser,HI_SPI_SER_T,HI_SPI_BASE_ADDR, HI_SPI_SER_OFFSET)
HI_SET_GET(hi_spi_ser_reserved,reserved,HI_SPI_SER_T,HI_SPI_BASE_ADDR, HI_SPI_SER_OFFSET)
HI_SET_GET(hi_spi_baudr_sckdv,sckdv,HI_SPI_BAUDR_T,HI_SPI_BASE_ADDR, HI_SPI_BAUDR_OFFSET)
HI_SET_GET(hi_spi_baudr_reserved,reserved,HI_SPI_BAUDR_T,HI_SPI_BASE_ADDR, HI_SPI_BAUDR_OFFSET)
HI_SET_GET(hi_spi_txftlr_tft,tft,HI_SPI_TXFTLR_T,HI_SPI_BASE_ADDR, HI_SPI_TXFTLR_OFFSET)
HI_SET_GET(hi_spi_txftlr_reserved,reserved,HI_SPI_TXFTLR_T,HI_SPI_BASE_ADDR, HI_SPI_TXFTLR_OFFSET)
HI_SET_GET(hi_spi_rxftlr_rft,rft,HI_SPI_RXFTLR_T,HI_SPI_BASE_ADDR, HI_SPI_RXFTLR_OFFSET)
HI_SET_GET(hi_spi_rxftlr_reserved_1,reserved_1,HI_SPI_RXFTLR_T,HI_SPI_BASE_ADDR, HI_SPI_RXFTLR_OFFSET)
HI_SET_GET(hi_spi_rxftlr_reserved_0,reserved_0,HI_SPI_RXFTLR_T,HI_SPI_BASE_ADDR, HI_SPI_RXFTLR_OFFSET)
HI_SET_GET(hi_spi_txflr_txtfl,txtfl,HI_SPI_TXFLR_T,HI_SPI_BASE_ADDR, HI_SPI_TXFLR_OFFSET)
HI_SET_GET(hi_spi_txflr_reserved,reserved,HI_SPI_TXFLR_T,HI_SPI_BASE_ADDR, HI_SPI_TXFLR_OFFSET)
HI_SET_GET(hi_spi_rxflr_rxtfl,rxtfl,HI_SPI_RXFLR_T,HI_SPI_BASE_ADDR, HI_SPI_RXFLR_OFFSET)
HI_SET_GET(hi_spi_rxflr_reserved,reserved,HI_SPI_RXFLR_T,HI_SPI_BASE_ADDR, HI_SPI_RXFLR_OFFSET)
HI_SET_GET(hi_spi_sr_busy,busy,HI_SPI_SR_T,HI_SPI_BASE_ADDR, HI_SPI_SR_OFFSET)
HI_SET_GET(hi_spi_sr_tfnf,tfnf,HI_SPI_SR_T,HI_SPI_BASE_ADDR, HI_SPI_SR_OFFSET)
HI_SET_GET(hi_spi_sr_tfe,tfe,HI_SPI_SR_T,HI_SPI_BASE_ADDR, HI_SPI_SR_OFFSET)
HI_SET_GET(hi_spi_sr_rfne,rfne,HI_SPI_SR_T,HI_SPI_BASE_ADDR, HI_SPI_SR_OFFSET)
HI_SET_GET(hi_spi_sr_rff,rff,HI_SPI_SR_T,HI_SPI_BASE_ADDR, HI_SPI_SR_OFFSET)
HI_SET_GET(hi_spi_sr_txe,txe,HI_SPI_SR_T,HI_SPI_BASE_ADDR, HI_SPI_SR_OFFSET)
HI_SET_GET(hi_spi_sr_dcol,dcol,HI_SPI_SR_T,HI_SPI_BASE_ADDR, HI_SPI_SR_OFFSET)
HI_SET_GET(hi_spi_sr_reserved,reserved,HI_SPI_SR_T,HI_SPI_BASE_ADDR, HI_SPI_SR_OFFSET)
HI_SET_GET(hi_spi_imr_txeim,txeim,HI_SPI_IMR_T,HI_SPI_BASE_ADDR, HI_SPI_IMR_OFFSET)
HI_SET_GET(hi_spi_imr_txoim,txoim,HI_SPI_IMR_T,HI_SPI_BASE_ADDR, HI_SPI_IMR_OFFSET)
HI_SET_GET(hi_spi_imr_rxuim,rxuim,HI_SPI_IMR_T,HI_SPI_BASE_ADDR, HI_SPI_IMR_OFFSET)
HI_SET_GET(hi_spi_imr_rxoim,rxoim,HI_SPI_IMR_T,HI_SPI_BASE_ADDR, HI_SPI_IMR_OFFSET)
HI_SET_GET(hi_spi_imr_rxfim,rxfim,HI_SPI_IMR_T,HI_SPI_BASE_ADDR, HI_SPI_IMR_OFFSET)
HI_SET_GET(hi_spi_imr_mstim,mstim,HI_SPI_IMR_T,HI_SPI_BASE_ADDR, HI_SPI_IMR_OFFSET)
HI_SET_GET(hi_spi_imr_reserved,reserved,HI_SPI_IMR_T,HI_SPI_BASE_ADDR, HI_SPI_IMR_OFFSET)
HI_SET_GET(hi_spi_isr_txeis,txeis,HI_SPI_ISR_T,HI_SPI_BASE_ADDR, HI_SPI_ISR_OFFSET)
HI_SET_GET(hi_spi_isr_txois,txois,HI_SPI_ISR_T,HI_SPI_BASE_ADDR, HI_SPI_ISR_OFFSET)
HI_SET_GET(hi_spi_isr_rxuis,rxuis,HI_SPI_ISR_T,HI_SPI_BASE_ADDR, HI_SPI_ISR_OFFSET)
HI_SET_GET(hi_spi_isr_rxois,rxois,HI_SPI_ISR_T,HI_SPI_BASE_ADDR, HI_SPI_ISR_OFFSET)
HI_SET_GET(hi_spi_isr_rxfis,rxfis,HI_SPI_ISR_T,HI_SPI_BASE_ADDR, HI_SPI_ISR_OFFSET)
HI_SET_GET(hi_spi_isr_mstis,mstis,HI_SPI_ISR_T,HI_SPI_BASE_ADDR, HI_SPI_ISR_OFFSET)
HI_SET_GET(hi_spi_isr_reserved,reserved,HI_SPI_ISR_T,HI_SPI_BASE_ADDR, HI_SPI_ISR_OFFSET)
HI_SET_GET(hi_spi_risr_txeir,txeir,HI_SPI_RISR_T,HI_SPI_BASE_ADDR, HI_SPI_RISR_OFFSET)
HI_SET_GET(hi_spi_risr_txoir,txoir,HI_SPI_RISR_T,HI_SPI_BASE_ADDR, HI_SPI_RISR_OFFSET)
HI_SET_GET(hi_spi_risr_rxuir,rxuir,HI_SPI_RISR_T,HI_SPI_BASE_ADDR, HI_SPI_RISR_OFFSET)
HI_SET_GET(hi_spi_risr_rxoir,rxoir,HI_SPI_RISR_T,HI_SPI_BASE_ADDR, HI_SPI_RISR_OFFSET)
HI_SET_GET(hi_spi_risr_rxfir,rxfir,HI_SPI_RISR_T,HI_SPI_BASE_ADDR, HI_SPI_RISR_OFFSET)
HI_SET_GET(hi_spi_risr_mstir,mstir,HI_SPI_RISR_T,HI_SPI_BASE_ADDR, HI_SPI_RISR_OFFSET)
HI_SET_GET(hi_spi_risr_reserved,reserved,HI_SPI_RISR_T,HI_SPI_BASE_ADDR, HI_SPI_RISR_OFFSET)
HI_SET_GET(hi_spi_txoicr_txoicr,txoicr,HI_SPI_TXOICR_T,HI_SPI_BASE_ADDR, HI_SPI_TXOICR_OFFSET)
HI_SET_GET(hi_spi_txoicr_reserved,reserved,HI_SPI_TXOICR_T,HI_SPI_BASE_ADDR, HI_SPI_TXOICR_OFFSET)
HI_SET_GET(hi_spi_rxoicr_rxoicr,rxoicr,HI_SPI_RXOICR_T,HI_SPI_BASE_ADDR, HI_SPI_RXOICR_OFFSET)
HI_SET_GET(hi_spi_rxoicr_reserved,reserved,HI_SPI_RXOICR_T,HI_SPI_BASE_ADDR, HI_SPI_RXOICR_OFFSET)
HI_SET_GET(hi_spi_rxuicr_rxuicr,rxuicr,HI_SPI_RXUICR_T,HI_SPI_BASE_ADDR, HI_SPI_RXUICR_OFFSET)
HI_SET_GET(hi_spi_rxuicr_reserved,reserved,HI_SPI_RXUICR_T,HI_SPI_BASE_ADDR, HI_SPI_RXUICR_OFFSET)
HI_SET_GET(hi_spi_msticr_msticr,msticr,HI_SPI_MSTICR_T,HI_SPI_BASE_ADDR, HI_SPI_MSTICR_OFFSET)
HI_SET_GET(hi_spi_msticr_reserved,reserved,HI_SPI_MSTICR_T,HI_SPI_BASE_ADDR, HI_SPI_MSTICR_OFFSET)
HI_SET_GET(hi_spi_icr_icr,icr,HI_SPI_ICR_T,HI_SPI_BASE_ADDR, HI_SPI_ICR_OFFSET)
HI_SET_GET(hi_spi_icr_reserved,reserved,HI_SPI_ICR_T,HI_SPI_BASE_ADDR, HI_SPI_ICR_OFFSET)
HI_SET_GET(hi_spi_dmacr_rdmae,rdmae,HI_SPI_DMACR_T,HI_SPI_BASE_ADDR, HI_SPI_DMACR_OFFSET)
HI_SET_GET(hi_spi_dmacr_tdmae,tdmae,HI_SPI_DMACR_T,HI_SPI_BASE_ADDR, HI_SPI_DMACR_OFFSET)
HI_SET_GET(hi_spi_dmacr_pmi0_ind,pmi0_ind,HI_SPI_DMACR_T,HI_SPI_BASE_ADDR, HI_SPI_DMACR_OFFSET)
HI_SET_GET(hi_spi_dmatdlr_dmatdl,dmatdl,HI_SPI_DMATDLR_T,HI_SPI_BASE_ADDR, HI_SPI_DMATDLR_OFFSET)
HI_SET_GET(hi_spi_dmatdlr_reserved_1,reserved_1,HI_SPI_DMATDLR_T,HI_SPI_BASE_ADDR, HI_SPI_DMATDLR_OFFSET)
HI_SET_GET(hi_spi_dmatdlr_reserved_0,reserved_0,HI_SPI_DMATDLR_T,HI_SPI_BASE_ADDR, HI_SPI_DMATDLR_OFFSET)
HI_SET_GET(hi_spi_dmardlr_dmardl,dmardl,HI_SPI_DMARDLR_T,HI_SPI_BASE_ADDR, HI_SPI_DMARDLR_OFFSET)
HI_SET_GET(hi_spi_dmardlr_reserved_1,reserved_1,HI_SPI_DMARDLR_T,HI_SPI_BASE_ADDR, HI_SPI_DMARDLR_OFFSET)
HI_SET_GET(hi_spi_dmardlr_reserved_0,reserved_0,HI_SPI_DMARDLR_T,HI_SPI_BASE_ADDR, HI_SPI_DMARDLR_OFFSET)
HI_SET_GET(hi_spi_idr_idcode,idcode,HI_SPI_IDR_T,HI_SPI_BASE_ADDR, HI_SPI_IDR_OFFSET)
HI_SET_GET(hi_spi_ssi_comp_version_ssi_comp_version,ssi_comp_version,HI_SPI_SSI_COMP_VERSION_T,HI_SPI_BASE_ADDR, HI_SPI_SSI_COMP_VERSION_OFFSET)
HI_SET_GET(hi_spi_dr_dr,dr,HI_SPI_DR_T,HI_SPI_BASE_ADDR, HI_SPI_DR_OFFSET)
HI_SET_GET(hi_spi_dr_reserved,reserved,HI_SPI_DR_T,HI_SPI_BASE_ADDR, HI_SPI_DR_OFFSET)
HI_SET_GET(hi_spi_rx_sample_dly_rsd,rsd,HI_SPI_RX_SAMPLE_DLY_T,HI_SPI_BASE_ADDR, HI_SPI_RX_SAMPLE_DLY_OFFSET)
HI_SET_GET(hi_spi_rx_sample_dly_reserved,reserved,HI_SPI_RX_SAMPLE_DLY_T,HI_SPI_BASE_ADDR, HI_SPI_RX_SAMPLE_DLY_OFFSET)

#endif // __HI_SPI_H__

