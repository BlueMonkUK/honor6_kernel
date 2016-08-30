


#include "bsp_edma.h"
#include "drv_comm.h"
#include "logLib.h"

s32 EDMA_NormTaskLaunch(AXI_DMA_TASK_HANDLE_S * pstDMAHandle);
s32 EDMA_2VecTaskLaunch(AXI_DMA_TASK_HANDLE_S * pstDMAHandle);
s32 EDMA_QueryCHNLState(u32 u32ChIdx);
s32 EDMA_QueryCHNsLState(u32 u32ChIdxMap);

s32 EDMA_NormTaskLaunch(AXI_DMA_TASK_HANDLE_S * pstDMAHandle)
{
	u32 channel_id  = pstDMAHandle->ulChIdx;
    u32 src_addr = pstDMAHandle->u32SrcAddr;
    u32 des_addr = pstDMAHandle->u32DstAddr;
    u32 len = pstDMAHandle->ulLength;

    if(1!=bsp_edma_channel_is_idle(channel_id))
    {
        logMsg("channel %d is_idle failed in EDMA_NormTaskLaunch",channel_id ,0,0,0,0,0);
    	return EDMA_FAIL;
    }

    if(0!=bsp_edma_channel_set_config(channel_id,EDMA_M2M,EDMA_TRANS_WIDTH_64,EDMA_BUR_LEN_16))
    {
        logMsg("channel_set_config failed in EDMA_NormTaskLaunch",0 ,0,0,0,0,0);
        return EDMA_FAIL;
    }
    if(0!=bsp_edma_channel_async_start( channel_id, src_addr, des_addr, len))
    {
        logMsg("channel_start failed in EDMA_NormTaskLaunch",0 ,0,0,0,0,0);
        return EDMA_FAIL;
    }
    return EDMA_SUCCESS;
}

s32 EDMA_2VecTaskLaunch(AXI_DMA_TASK_HANDLE_S * pstDMAHandle)
{
	u32 channel_id  = pstDMAHandle->ulChIdx;
    u32 src_addr = pstDMAHandle->u32SrcAddr;
    u32 des_addr = pstDMAHandle->u32DstAddr;
    u32 len = pstDMAHandle->ulLength;

    if(1!=bsp_edma_channel_is_idle(channel_id))
    {
        logMsg("channel %d is_idle failed in EDMA_2VecTaskLaunch",channel_id ,0,0,0,0,0);
    	return EDMA_FAIL;
    }
    if(0!=bsp_edma_channel_set_config(channel_id,M2M_CONFIG,EDMA_TRANS_WIDTH_64,EDMA_BUR_LEN_16))
    {
        logMsg("channel_set_config failed in EDMA_2VecTaskLaunch",0 ,0,0,0,0,0);
        return EDMA_FAIL;
    }
    if(0!=bsp_edma_channel_2vec_start( channel_id,  src_addr,  des_addr,  len,  SZ_8K))
    {
        logMsg("channel_start failed in EDMA_2VecTaskLaunch",0 ,0,0,0,0,0);
        return EDMA_FAIL;
    }
    return EDMA_SUCCESS;
}

s32 EDMA_QueryCHNLState(u32 u32ChIdx)
{
	u32 ulChan = u32ChIdx;/* ͨ����*/
    if(1==bsp_edma_channel_is_idle(ulChan))
    {
    	return EDMA_SUCCESS;
    }
    else
    {
    	return EDMA_FAIL;
    }
}

s32 EDMA_QueryCHNsLState(u32 u32ChIdxMap)
{
    if( 1==bsp_edma_chanmap_is_idle(u32ChIdxMap) )
    {
    	return EDMA_SUCCESS;
    }
    else
    {
    	return EDMA_FAIL;
    }
}
/* end ---- adapt for v7r1 */



/* start---- adapt for v9r1 */
/*******************************************************************************
  ������:      BSP_S32 balong_dma_init(void)
  ��������:    DMA��ʼ�����򣬹ҽ��ж�
  �������:    ��
  �������:    ��
  ����ֵ:      0
*******************************************************************************/
BSP_S32 balong_dma_init(void)
{
    return (BSP_S32)bsp_edma_init();
}

/*******************************************************************************
  ������:       int balong_dma_current_transfer_address(UINT32 channel_id)
  ��������:     ���ĳͨ����ǰ������ڴ��ַ
  �������:     channel_id : ͨ��ID������balong_dma_channel_init�����ķ���ֵ
  �������:     ��
  ����ֵ:       �ɹ���ͨ����ǰ������ڴ��ַ
                ʧ�ܣ�����
*******************************************************************************/
int balong_dma_current_transfer_address(BSP_U32 channel_id)
{
    return (int)bsp_edma_current_transfer_address((u32) channel_id);
}

/*******************************************************************************
  ������:       int balong_dma_channel_stop(UINT32 channel_id)
  ��������:     ָֹͣ����DMAͨ��
  �������:     channel_id : ͨ��ID������balong_dma_channel_init�����ķ���ֵ
  �������:     ��
  ����ֵ:       �ɹ���ͨ����ǰ������ڴ��ַ
                ʧ�ܣ�����
*******************************************************************************/
BSP_S32 balong_dma_channel_stop(BSP_U32 channel_id)
{
    return (BSP_S32)bsp_edma_channel_stop((u32) channel_id);
}


/*******************************************************************************
  ������:      BALONG_DMA_CB *balong_dma_channel_get_lli_addr(UINT32 channel_id)
  ��������:    ��ȡָ��DMAͨ����������ƿ����ʼ��ַ
  �������:    channel_id��ͨ��ID,����balong_dma_channel_init�����ķ���ֵ
  �������:    ��
  ����ֵ:      �ɹ���0
               ʧ�ܣ�����
*******************************************************************************/
BALONG_DMA_CB *balong_dma_channel_get_lli_addr (BSP_U32 channel_id)
{
    return (BALONG_DMA_CB *)bsp_edma_channel_get_lli_addr((u32) channel_id);
}


/******************************************************************************
  ������:      int balong_dma_channel_init (BALONG_DMA_REQ req,
                  channel_isr pFunc, UINT32 channel_arg, UINT32 int_flag)
  ��������:    ��������ŷ���ͨ����ע��ͨ���жϻص���������ʼ����������ź�����
               �������д��config�Ĵ���
  �������:    req : ���������
               pFunc : �ϲ�ģ��ע���DMAͨ���жϴ�������NULLʱ������ע��
               channel_arg : pFunc�����1��
                             pFuncΪNULL������Ҫ�����������
               int_flag : pFunc�����2, �������ж����ͣ�ȡֵ��ΧΪ
                        BALONG_DMA_INT_DONE��BALONG_DMA_INT_LLT_DONE��
                      �BALONG_DMA_INT_CONFIG_ERR��BALONG_DMA_INT_TRANSFER_ERR��
                        BALONG_DMA_INT_READ_ERR֮һ��������ϡ�
                        pFuncΪNULL������Ҫ�����������
  �������:    ��
  ����ֵ:      �ɹ���ͨ����
               ʧ�ܣ�����
*******************************************************************************/
BSP_S32 balong_dma_channel_init (BALONG_DMA_REQ req, channel_isr pFunc, BSP_U32 channel_arg, BSP_U32 int_flag)
{
    return (BSP_S32)bsp_edma_channel_init(req, pFunc, (u32) channel_arg, (u32) int_flag);
}

/*******************************************************************************
  ������:      int balong_dma_channel_set_config (UINT32 channel_id,
                       UINT32 direction, UINT32 burst_width, UINT32 burst_len)
  ��������:    ����ʽDMA����ʱ�����ñ���������ͨ������
               ��ʽDMA����ʱ������Ҫʹ�ñ�������
  �������:    channel_id : ͨ��ID������balong_dma_channel_init�����ķ���ֵ
               direction : DMA���䷽��, ȡֵΪBALONG_DMA_P2M��BALONG_DMA_M2P��
                           BALONG_DMA_M2M֮һ
               burst_width��ȡֵΪ0��1��2��3����ʾ��burstλ��Ϊ8��16��32��64bit
               burst_len��ȡֵ��Χ0~15����ʾ��burst����Ϊ1~16
  �������:    ��
  ����ֵ:      �ɹ���0
               ʧ�ܣ�����
*******************************************************************************/
BSP_S32 balong_dma_channel_set_config (BSP_U32 channel_id, BSP_U32 direction,BSP_U32 burst_width, BSP_U32 burst_len)
{
    return (BSP_S32)bsp_edma_channel_set_config((u32) channel_id, (u32) direction, (u32) burst_width, (u32) burst_len);
}

/* /// Ϊ�������� ccore*/
BSP_S32 balong_dma_channel_dest_set_config (BSP_U32 channel_id, BSP_U32 burst_width, BSP_U32 burst_len)
{
    return (BSP_S32)bsp_edma_channel_dest_set_config((u32) channel_id, (u32) burst_width, (u32) burst_len);
}

/*******************************************************************************
  ������:      int balong_dma_channel_start (UINT32 channel_id, UINT32 src_addr,
                       UINT32 des_addr, UINT32 len)
  ��������:    ����һ��ͬ��DMA����, DMA������ɺ󣬲ŷ���
               ʹ�ñ�����ʱ������Ҫע���жϴ�����
  �������:    channel_id��ͨ��ID,����balong_dma_channel_init�����ķ���ֵ
               src_addr�����ݴ���Դ��ַ�������������ַ
               des_addr�����ݴ���Ŀ�ĵ�ַ�������������ַ
               len�����ݴ��䳤�ȣ���λ���ֽڣ�һ�δ������ݵ���󳤶���65535�ֽ�
  �������:    ��
  ����ֵ:      �ɹ���0
               ʧ�ܣ�����
*******************************************************************************/
BSP_S32 balong_dma_channel_start (BSP_U32 channel_id, BSP_U32 src_addr, BSP_U32 des_addr, BSP_U32 len)
{
    return (BSP_S32)bsp_edma_channel_start((u32) channel_id, (u32) src_addr, (u32) des_addr, (u32) len);
}

/*******************************************************************************
  ������:      int balong_dma_channel_async_start (UINT32 channel_id,
                unsigned int src_addr, unsigned int des_addr, unsigned int len)
  ��������:    ����һ���첽DMA���䡣����DMA����󣬾ͷ��ء����ȴ�DMA������ɡ�
               ʹ�ñ�����ʱ��ע���жϴ��������жϴ������д���DMA
��������¼�
               ���ߣ���ע���жϴ�������ʹ��balong_dma_channel_is_idle������ѯ
               DMA�����Ƿ����
  �������:    channel_id��ͨ��ID,����balong_dma_channel_init�����ķ���ֵ
               src_addr�����ݴ���Դ��ַ�������������ַ
               des_addr�����ݴ���Ŀ�ĵ�ַ�������������ַ
               len�����ݴ��䳤�ȣ���λ���ֽڣ�һ�δ������ݵ���󳤶���65535�ֽ�
  �������:    ��
  ����ֵ:      �ɹ���0
               ʧ�ܣ�����
*******************************************************************************/
BSP_S32 balong_dma_channel_async_start (BSP_U32 channel_id, BSP_U32 src_addr, BSP_U32 des_addr, BSP_U32 len)
{
    return (BSP_S32)bsp_edma_channel_async_start((u32) channel_id, (u32) src_addr, (u32) des_addr, (u32) len);
}

/*******************************************************************************
  ������:      int balong_dma_channel_lli_start (UINT32 channel_id)
  ��������:    ������ʽDMA���䡣����ʽDMA�����нڵ㴫�䶼ȫ����ɺ�ŷ��ء�
               ��ʽDMA��ÿ���ڵ����������䳤��Ϊ65535�ֽڡ�
               ע�⣺���ô˺���ǰ���������ú�������ƿ顣
  �������:    channel_id��ͨ��ID,����balong_dma_channel_init�����ķ���ֵ
  �������:    ��
  ����ֵ:      �ɹ���0
               ʧ�ܣ�����
*******************************************************************************/
BSP_S32 balong_dma_channel_lli_start (BSP_U32 channel_id)
{
    return (BSP_S32)bsp_edma_channel_lli_start((u32) channel_id);
}

/*******************************************************************************
  ������:      int balong_dma_channel_lli_start (UINT32 channel_id)
  ��������:    ������ʽDMA���䣬Ȼ���������أ����ȴ�DMA������ɡ�
               ��ʽDMA��ÿ���ڵ����������䳤��Ϊ65535�ֽڡ�
               ע�⣺���ô˺���ǰ���������ú�������ƿ顣
  �������:    channel_id��ͨ��ID,����balong_dma_channel_init�����ķ���ֵ
  �������:    ��
  ����ֵ:      �ɹ���0
               ʧ�ܣ�����
*******************************************************************************/
BSP_S32 balong_dma_channel_lli_async_start (BSP_U32 channel_id)
{
    return (BSP_S32)bsp_edma_channel_lli_async_start((u32) channel_id);
}

/******************************************************************************
*
  ������:       int balong_dma_channel_is_idle (UINT32 channel_id)
  ��������:     ��ѯDMAͨ���Ƿ����
  �������:     channel_id : ͨ��ID������balong_dma_channel_init�����ķ���ֵ
  �������:     ��
  ����ֵ:       0 : ͨ��æµ
                1 : ͨ������
                ���� : ʧ��
*******************************************************************************/
BSP_S32 balong_dma_channel_is_idle (BSP_U32 channel_id)
{
    return (BSP_S32)bsp_edma_channel_is_idle((u32) channel_id);
}
/* end ---- adapt for v9r1 */



