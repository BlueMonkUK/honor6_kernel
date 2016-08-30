

#include "bsp_socp.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef ENABLE_BUILD_SOCP /* do not build socp, stub */
/*****************************************************************************
* �� �� ��  : DRV_SOCP_INIT
*
* ��������  : ģ���ʼ������
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��ʼ���ɹ��ı�ʶ��
*****************************************************************************/
BSP_S32 DRV_SOCP_INIT()
{
    return 0;
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_CORDER_SET_SRC_CHAN
 ��������  : �˽ӿ����SOCP������Դͨ���ķ��䣬���ݱ�����Դͨ����������ͨ�����ԣ�����Ŀ��ͨ�������غ���ִ�н����
 �������  : pSrcAttr:������Դͨ�������ṹ��ָ�롣
             pSrcChanID:���뵽��Դͨ��ID��
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:����Դͨ������ɹ���
             SOCP_ERROR:����Դͨ������ʧ�ܡ�
*****************************************************************************/
BSP_S32 DRV_SOCP_CORDER_SET_SRC_CHAN(SOCP_CODER_SRC_ENUM_U32 enSrcChanID, SOCP_CODER_SRC_CHAN_S *pSrcAttr)
{
    return 0;
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_FREE_CHANNEL
 ��������  : �˽ӿڸ���ͨ��ID�ͷŷ���ı����ͨ����
 �������  : u32ChanID:ͨ��ID��
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:ͨ���ͷųɹ���
             SOCP_ERROR:ͨ���ͷ�ʧ�ܡ�
*****************************************************************************/
BSP_S32 DRV_SOCP_FREE_CHANNEL(BSP_U32 u32ChanID)
{
    return 0;
}

/*****************************************************************************
* �� �� ��  : DRV_SOCP_CLEAN_ENCSRC_CHAN
*
* ��������  : ��ձ���Դͨ����ͬ��V9 SOCP�ӿ�
*
* �������  : enSrcChanID       ����ͨ����
*
* �������  : ��
*
* �� �� ֵ  : BSP_OK
*****************************************************************************/
BSP_U32 DRV_SOCP_CLEAN_ENCSRC_CHAN(SOCP_CODER_SRC_ENUM_U32 enSrcChanID)
{
    return 0;
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_REGISTER_EVENT_CB
 ��������  : �˽ӿ�Ϊ����ͨ��ע���¼��ص�������
 �������  : u32ChanID:ͨ��ID��
             EventCB:�¼��ص��������ο�socp_event_cb��������
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:ע���¼��ص������ɹ���
             SOCP_ERROR:ע���¼��ص�����ʧ�ܡ�
*****************************************************************************/
BSP_S32 DRV_SOCP_REGISTER_EVENT_CB(BSP_U32 u32ChanID, socp_event_cb EventCB)
{
    return 0;
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_START
 ��������  : �˽ӿ�����Դͨ��������������߽��롣
 �������  : u32SrcChanID:Դͨ��ID��
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:�������������ɹ���
             SOCP_ERROR:������������ʧ�ܡ�
*****************************************************************************/
BSP_S32 DRV_SOCP_START(BSP_U32 u32SrcChanID)
{
    return 0;
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_STOP
 ��������  : �˽ӿ�����Դͨ����ֹͣ������߽��롣
 �������  : u32SrcChanID:Դͨ��ID��
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:��������ֹͣ�ɹ���
             SOCP_ERROR:��������ֹͣʧ�ܡ�
*****************************************************************************/
BSP_S32 DRV_SOCP_STOP(BSP_U32 u32SrcChanID)
{
    return 0;
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_CHAN_SOFT_RESET
 ��������  : Դͨ����λ
 �������  : u32ChanID:ͨ��ID
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:�踴λ�ɹ���
             ����ֵ:��λʧ��
*****************************************************************************/
BSP_S32 DRV_SOCP_CHAN_SOFT_RESET(BSP_U32 u32ChanID)
{
    return 0;
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_GET_WRITE_BUFF
 ��������  : �˽ӿ����ڻ�ȡд����buffer��
 �������  : u32SrcChanID:Դͨ��ID
             pBuff:           :д����buffer

 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:��ȡд����buffer�ɹ���
             SOCP_ERROR:��ȡд����bufferʧ��
*****************************************************************************/
BSP_S32 DRV_SOCP_GET_WRITE_BUFF( BSP_U32 u32SrcChanID, SOCP_BUFFER_RW_S *pBuff)
{
    return 0;
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_WRITE_DONE
 ��������  : �ýӿ��������ݵ�д�������ṩд�����ݵĳ��ȡ�
 �������  : u32SrcChanID:Դͨ��ID
             u32WrtSize:   ��д�����ݵĳ���
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:д�����ݳɹ���
             SOCP_ERROR:д������ʧ��
*****************************************************************************/
BSP_S32 DRV_SOCP_WRITE_DONE(BSP_U32 u32SrcChanID, BSP_U32 u32WrtSize)
{
    return 0;
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_REGISTER_RD_CB
 ��������  : �ýӿ�����ע���RD�������ж�ȡ���ݵĻص�������
 �������  : u32SrcChanID:Դͨ��ID
             RdCB:  �¼��ص�����
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:ע��RD���λ����������ݻص������ɹ���
             SOCP_ERROR:ע��RD���λ����������ݻص�����ʧ��
*****************************************************************************/
BSP_S32 DRV_SOCP_REGISTER_RD_CB(BSP_U32 u32SrcChanID, socp_rd_cb RdCB)
{
    return 0;
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_GET_RD_BUFFER
 ��������  : �ô˽ӿ����ڻ�ȡRD buffer������ָ�롣
 �������  : u32SrcChanID:Դͨ��ID
             pBuff:  RD buffer
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:��ȡRD���λ������ɹ�
             SOCP_ERROR:��ȡRD���λ�����ʧ��
*****************************************************************************/
BSP_S32 DRV_SOCP_GET_RD_BUFFER( BSP_U32 u32SrcChanID,SOCP_BUFFER_RW_S *pBuff)
{
    return 0;
}

/*****************************************************************************
 �� �� ��  : bsp_socp_read_rd_done
 ��������  : �˽ӿ������ϲ�֪ͨSOCP��������RD buffer��ʵ�ʶ�ȡ�����ݡ�
 �������  : u32SrcChanID:Դͨ��ID
             u32RDSize:  ��RD buffer��ʵ�ʶ�ȡ�����ݳ���
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:��ȡRDbuffer�е����ݳɹ�
             SOCP_ERROR:��ȡRDbuffer�е�����ʧ��
*****************************************************************************/
BSP_S32 DRV_SOCP_READ_RD_DONE(BSP_U32 u32SrcChanID, BSP_U32 u32RDSize)
{
    return 0;
}

/*****************************************************************************
* �� �� ��  : DRV_SOCP_DSPCHN_START
* ��������  : enable DSP channel
* �������  : 
* �������  : ��
* �� �� ֵ  :
*****************************************************************************/
BSP_VOID  DRV_SOCP_DSPCHN_START(BSP_VOID)
{
    return; 
}

/*****************************************************************************
* �� �� ��  : DRV_SOCP_DSPCHN_STOP
* ��������  : disable DSP channel
* �������  : 
* �������  : ��
* �� �� ֵ  :
*****************************************************************************/
BSP_VOID  DRV_SOCP_DSPCHN_STOP(BSP_VOID)
{
    return;
}

/*****************************************************************************
* �� �� ��  : DRV_BBPDMA_DRX_BAK_REG
* ��������  : BBPDMA����
* �������  : 
* �������  : ��
* �� �� ֵ  :
*****************************************************************************/
BSP_S32  DRV_BBPDMA_DRX_BAK_REG(BSP_VOID)
{
    return 0;
}

/*****************************************************************************
* �� �� ��  : DRV_BBPDMA_DRX_RESTORE_REG
* ��������  : �ָ�BBPDMA
* �������  : 
* �������  : ��
* �� �� ֵ  :
*****************************************************************************/
BSP_S32  DRV_BBPDMA_DRX_RESTORE_REG(BSP_VOID)
{
    return 0;
}


/*****************************************************************************
* �� �� ��  : DRV_SOCP_GET_STATE
*
* ��������  : ��ȡSOCP״̬
*
* �� �� ֵ  : SOCP_IDLE    ����
*             SOCP_BUSY    æµ
*****************************************************************************/
SOCP_STATE_ENUM_UINT32 DRV_SOCP_GET_STATE(BSP_VOID)
{
    return 0;
}

BSP_U32  DRV_SOCP_INIT_LTE_DSP(u32 ulChanId,u32 ulPhyAddr,u32 ulSize)
{
    return 0;
}

BSP_U32  DRV_SOCP_INIT_LTE_BBP_LOG(BSP_U32 ulChanId,BSP_U32 ulPhyAddr,BSP_U32 ulSize)
{
    return 0;
}

BSP_U32  DRV_SOCP_INIT_LTE_BBP_DS(BSP_U32 ulChanId,BSP_U32 ulPhyAddr,BSP_U32 ulSize)
{
    return 0;
}

BSP_VOID DRV_SOCP_ENABLE_LTE_BBP_DSP(BSP_U32 ulChanId)
{
    return 0;
}

/*****************************************************************************
* �� �� ��  : DRV_SOCP_VOTE_TO_MCORE
* ��������  : SOCPͶƱ�ӿڣ��ýӿ�ֻ��C���ṩ������LDSP�״μ��ص�SOCP�ϵ�����
* �������  : type --- ͶƱ����
* �������  : ��
* �� �� ֵ  : BSP_S32 0 --- ͶƱ�ɹ���0xFFFFFFFF --- ͶƱʧ��
*****************************************************************************/
BSP_S32 DRV_SOCP_VOTE_TO_MCORE(SOCP_VOTE_TYPE_ENUM_U32 type)
{
    return 0;
}

#else
/*****************************************************************************
* �� �� ��  : DRV_SOCP_INIT
*
* ��������  : ģ���ʼ������
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��ʼ���ɹ��ı�ʶ��
*****************************************************************************/
BSP_S32 DRV_SOCP_INIT()
{
    return socp_init();
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_CORDER_SET_SRC_CHAN
 ��������  : �˽ӿ����SOCP������Դͨ���ķ��䣬���ݱ�����Դͨ����������ͨ�����ԣ�����Ŀ��ͨ�������غ���ִ�н����
 �������  : pSrcAttr:������Դͨ�������ṹ��ָ�롣
             pSrcChanID:���뵽��Դͨ��ID��
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:����Դͨ������ɹ���
             SOCP_ERROR:����Դͨ������ʧ�ܡ�
*****************************************************************************/
BSP_S32 DRV_SOCP_CORDER_SET_SRC_CHAN(SOCP_CODER_SRC_ENUM_U32 enSrcChanID, SOCP_CODER_SRC_CHAN_S *pSrcAttr)
{
    return bsp_socp_coder_set_src_chan(enSrcChanID, pSrcAttr);
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_FREE_CHANNEL
 ��������  : �˽ӿڸ���ͨ��ID�ͷŷ���ı����ͨ����
 �������  : u32ChanID:ͨ��ID��
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:ͨ���ͷųɹ���
             SOCP_ERROR:ͨ���ͷ�ʧ�ܡ�
*****************************************************************************/
BSP_S32 DRV_SOCP_FREE_CHANNEL(BSP_U32 u32ChanID)
{
    return bsp_socp_free_channel(u32ChanID);
}

/*****************************************************************************
* �� �� ��  : DRV_SOCP_CLEAN_ENCSRC_CHAN
*
* ��������  : ��ձ���Դͨ����ͬ��V9 SOCP�ӿ�
*
* �������  : enSrcChanID       ����ͨ����
*
* �������  : ��
*
* �� �� ֵ  : BSP_OK
*****************************************************************************/
BSP_U32 DRV_SOCP_CLEAN_ENCSRC_CHAN(SOCP_CODER_SRC_ENUM_U32 enSrcChanID)
{
    return bsp_socp_clean_encsrc_chan(enSrcChanID);
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_REGISTER_EVENT_CB
 ��������  : �˽ӿ�Ϊ����ͨ��ע���¼��ص�������
 �������  : u32ChanID:ͨ��ID��
             EventCB:�¼��ص��������ο�socp_event_cb��������
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:ע���¼��ص������ɹ���
             SOCP_ERROR:ע���¼��ص�����ʧ�ܡ�
*****************************************************************************/
BSP_S32 DRV_SOCP_REGISTER_EVENT_CB(BSP_U32 u32ChanID, socp_event_cb EventCB)
{
    return bsp_socp_register_event_cb(u32ChanID, EventCB);
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_START
 ��������  : �˽ӿ�����Դͨ��������������߽��롣
 �������  : u32SrcChanID:Դͨ��ID��
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:�������������ɹ���
             SOCP_ERROR:������������ʧ�ܡ�
*****************************************************************************/
BSP_S32 DRV_SOCP_START(BSP_U32 u32SrcChanID)
{
    return bsp_socp_start(u32SrcChanID);
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_STOP
 ��������  : �˽ӿ�����Դͨ����ֹͣ������߽��롣
 �������  : u32SrcChanID:Դͨ��ID��
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:��������ֹͣ�ɹ���
             SOCP_ERROR:��������ֹͣʧ�ܡ�
*****************************************************************************/
BSP_S32 DRV_SOCP_STOP(BSP_U32 u32SrcChanID)
{
    return bsp_socp_stop(u32SrcChanID);
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_CHAN_SOFT_RESET
 ��������  : Դͨ����λ
 �������  : u32ChanID:ͨ��ID
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:�踴λ�ɹ���
             ����ֵ:��λʧ��
*****************************************************************************/
BSP_S32 DRV_SOCP_CHAN_SOFT_RESET(BSP_U32 u32ChanID)
{
    return bsp_socp_chan_soft_reset(u32ChanID);
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_GET_WRITE_BUFF
 ��������  : �˽ӿ����ڻ�ȡд����buffer��
 �������  : u32SrcChanID:Դͨ��ID
             pBuff:           :д����buffer

 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:��ȡд����buffer�ɹ���
             SOCP_ERROR:��ȡд����bufferʧ��
*****************************************************************************/
BSP_S32 DRV_SOCP_GET_WRITE_BUFF( BSP_U32 u32SrcChanID, SOCP_BUFFER_RW_S *pBuff)
{
    return bsp_socp_get_write_buff(u32SrcChanID, pBuff);
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_WRITE_DONE
 ��������  : �ýӿ��������ݵ�д�������ṩд�����ݵĳ��ȡ�
 �������  : u32SrcChanID:Դͨ��ID
             u32WrtSize:   ��д�����ݵĳ���
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:д�����ݳɹ���
             SOCP_ERROR:д������ʧ��
*****************************************************************************/
BSP_S32 DRV_SOCP_WRITE_DONE(BSP_U32 u32SrcChanID, BSP_U32 u32WrtSize)
{
    return bsp_socp_write_done(u32SrcChanID, u32WrtSize);
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_REGISTER_RD_CB
 ��������  : �ýӿ�����ע���RD�������ж�ȡ���ݵĻص�������
 �������  : u32SrcChanID:Դͨ��ID
             RdCB:  �¼��ص�����
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:ע��RD���λ����������ݻص������ɹ���
             SOCP_ERROR:ע��RD���λ����������ݻص�����ʧ��
*****************************************************************************/
BSP_S32 DRV_SOCP_REGISTER_RD_CB(BSP_U32 u32SrcChanID, socp_rd_cb RdCB)
{
    return bsp_socp_register_rd_cb(u32SrcChanID, RdCB);
}

/*****************************************************************************
 �� �� ��  : DRV_SOCP_GET_RD_BUFFER
 ��������  : �ô˽ӿ����ڻ�ȡRD buffer������ָ�롣
 �������  : u32SrcChanID:Դͨ��ID
             pBuff:  RD buffer
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:��ȡRD���λ������ɹ�
             SOCP_ERROR:��ȡRD���λ�����ʧ��
*****************************************************************************/
BSP_S32 DRV_SOCP_GET_RD_BUFFER( BSP_U32 u32SrcChanID,SOCP_BUFFER_RW_S *pBuff)
{
    return bsp_socp_get_rd_buffer(u32SrcChanID, pBuff);
}

/*****************************************************************************
 �� �� ��  : bsp_socp_read_rd_done
 ��������  : �˽ӿ������ϲ�֪ͨSOCP��������RD buffer��ʵ�ʶ�ȡ�����ݡ�
 �������  : u32SrcChanID:Դͨ��ID
             u32RDSize:  ��RD buffer��ʵ�ʶ�ȡ�����ݳ���
 �������  : �ޡ�
 �� �� ֵ  : SOCP_OK:��ȡRDbuffer�е����ݳɹ�
             SOCP_ERROR:��ȡRDbuffer�е�����ʧ��
*****************************************************************************/
BSP_S32 DRV_SOCP_READ_RD_DONE(BSP_U32 u32SrcChanID, BSP_U32 u32RDSize)
{
    return bsp_socp_read_rd_done(u32SrcChanID, u32RDSize);
}

/*****************************************************************************
* �� �� ��  : DRV_SOCP_DSPCHN_START
* ��������  : enable DSP channel
* �������  : 
* �������  : ��
* �� �� ֵ  :
*****************************************************************************/
BSP_VOID  DRV_SOCP_DSPCHN_START(BSP_VOID)
{
    bsp_socp_start_dsp(); 
}

/*****************************************************************************
* �� �� ��  : DRV_SOCP_DSPCHN_STOP
* ��������  : disable DSP channel
* �������  : 
* �������  : ��
* �� �� ֵ  :
*****************************************************************************/
BSP_VOID  DRV_SOCP_DSPCHN_STOP(BSP_VOID)
{
    bsp_socp_stop_dsp();
}

/*****************************************************************************
* �� �� ��  : DRV_BBPDMA_DRX_BAK_REG
* ��������  : BBPDMA����
* �������  : 
* �������  : ��
* �� �� ֵ  :
*****************************************************************************/
extern BSP_S32 BSP_BBPDMA_DrxBakReg(BSP_VOID);
BSP_S32  DRV_BBPDMA_DRX_BAK_REG(BSP_VOID)
{
    return (BSP_S32)BSP_BBPDMA_DrxBakReg();
}

/*****************************************************************************
* �� �� ��  : DRV_BBPDMA_DRX_RESTORE_REG
* ��������  : �ָ�BBPDMA
* �������  : 
* �������  : ��
* �� �� ֵ  :
*****************************************************************************/
extern BSP_S32 BSP_BBPDMA_DrxRestoreReg(BSP_VOID);
BSP_S32  DRV_BBPDMA_DRX_RESTORE_REG(BSP_VOID)
{
    return BSP_BBPDMA_DrxRestoreReg();
}


/*****************************************************************************
* �� �� ��  : DRV_SOCP_GET_STATE
*
* ��������  : ��ȡSOCP״̬
*
* �� �� ֵ  : SOCP_IDLE    ����
*             SOCP_BUSY    æµ
*****************************************************************************/
SOCP_STATE_ENUM_UINT32 DRV_SOCP_GET_STATE(BSP_VOID)
{
    return bsp_socp_get_state();
}

extern BSP_U32 bsp_socp_init_lte_dsp(u32 ulChanId,u32 ulPhyAddr,u32 ulSize);
BSP_U32 DRV_SOCP_INIT_LTE_DSP(u32 ulChanId,u32 ulPhyAddr,u32 ulSize)
{
    return bsp_socp_init_lte_dsp(ulChanId,ulPhyAddr,ulSize);
}

extern BSP_U32 bsp_socp_init_lte_bbp_log(BSP_U32 ulChanId,BSP_U32 ulPhyAddr,BSP_U32 ulSize);
BSP_U32 DRV_SOCP_INIT_LTE_BBP_LOG(BSP_U32 ulChanId,BSP_U32 ulPhyAddr,BSP_U32 ulSize)
{
    return bsp_socp_init_lte_bbp_log(ulChanId,ulPhyAddr,ulSize);
}

extern BSP_U32 bsp_socp_int_lte_bbp_ds(BSP_U32 ulChanId,BSP_U32 ulPhyAddr,BSP_U32 ulSize);
BSP_U32 DRV_SOCP_INIT_LTE_BBP_DS(BSP_U32 ulChanId,BSP_U32 ulPhyAddr,BSP_U32 ulSize)
{
    return bsp_socp_int_lte_bbp_ds(ulChanId,ulPhyAddr,ulSize);
}

extern BSP_VOID bsp_socp_enable_lte_dsp_bbp(BSP_U32 ulChanId);
BSP_VOID DRV_SOCP_ENABLE_LTE_BBP_DSP(BSP_U32 ulChanId)
{
    bsp_socp_enable_lte_dsp_bbp(ulChanId);
}

/*****************************************************************************
* �� �� ��  : DRV_SOCP_VOTE_TO_MCORE
* ��������  : SOCPͶƱ�ӿڣ��ýӿ�ֻ��C���ṩ������LDSP�״μ��ص�SOCP�ϵ�����
* �������  : type --- ͶƱ����
* �������  : ��
* �� �� ֵ  : BSP_S32 0 --- ͶƱ�ɹ���0xFFFFFFFF --- ͶƱʧ��
*****************************************************************************/
BSP_S32 DRV_SOCP_VOTE_TO_MCORE(SOCP_VOTE_TYPE_ENUM_U32 type)
{
#if(FEATURE_SOCP_ON_DEMAND == FEATURE_ON)
    return bsp_socp_vote_to_mcore(type);
#else
    return 0;
#endif
}

#endif /* ENABLE_BUILD_SOCP */

BSP_VOID BSP_SOCP_RefreshSDLogCfg(BSP_U32 ulTimerLen)
{
	return;
}

#ifdef __cplusplus
}
#endif

