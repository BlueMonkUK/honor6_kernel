

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "xa_comm.h"
#include "ucom_mem_dyn.h"
#include "xa_mp3_dec_api.h"
#include "audio_player.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    ��ά�ɲ���Ϣ�а�����C�ļ���ź궨��
*****************************************************************************/
/*lint -e(767)*/
#define THIS_FILE_ID                    OM_FILE_ID_CODEC_XA_COMM_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
/*****************************************************************************
�� �� ����XA_COMM_Startup
����������ΪAPI�ṹ������ڴ�ռ䣬��ʼ��API�ṹ�壬��ʼ��Ĭ������
��    �룺pfProcess         -- tensilica����ں���ָ��
          pApiObj           -- codec���
          pstMemCfg         -- ���ô˺�����ģ������ά��
��    ����
�� �� ֵ��XA_NO_ERROR �ɹ�����XA_NO_ERRORʧ��
���ú���  :
��������  :


 �޸���ʷ      :
  1.��    ��   : 2012��12��19��
    ��    ��   : twx144307
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_INT32 XA_COMM_Startup(xa_codec_func_t*  pfuncProcess,
                          xa_codec_handle_t*       ppstApiObj,
                          XA_COMM_MEMORY_CFG_STRU* pstMemCfg)
{
    VOS_INT32  err_code = XA_NO_ERROR;
    VOS_INT32  swApiObjSize;

    /* �쳣��μ�� */
    if(   (VOS_NULL == pfuncProcess)
       || (VOS_NULL == ppstApiObj)
       || (VOS_NULL == pstMemCfg) )
    {
        return XA_COM_CUSTOM_NULL_POINT_ERROR;
    }



   /* ��ȡAPI�ṹ��С */
    err_code = (*pfuncProcess)(NULL,
                               XA_API_CMD_GET_API_SIZE,
                               0,
                               (VOS_VOID *)(&swApiObjSize));

    CODEC_XA_ERRCODE_CHECK(err_code);

    /* ������DDR�ж�̬����ķ�ʽ�����ڴ棬�ڴ��׵�ַ��Ϊ8�ֽڶ��� */
    (*ppstApiObj) = (xa_codec_handle_t)UCOM_MemAlloc(swApiObjSize);

    if(VOS_NULL == (*ppstApiObj))
    {
        return XA_COM_CUSTOM_NULL_POINT_ERROR;
    }

    /* ��¼�����APIMem */
    pstMemCfg->pApiStructBuff= (*ppstApiObj);
    pstMemCfg->swApiObjSize  = swApiObjSize;

    /* ���API */
    UCOM_MemSet(pstMemCfg->pApiStructBuff, 0, (VOS_UINT32)pstMemCfg->swApiObjSize);

    /* ��ʼ��Ĭ������ */
    err_code = (*pfuncProcess)((*ppstApiObj),
                               XA_API_CMD_INIT,
                               XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS,
                               NULL);

    CODEC_XA_ERRCODE_CHECK(err_code);

    return err_code;
}

/*****************************************************************************
�� �� ����XA_COMM_SetBlksMemory
����������Tensilica�����ø����ڴ���ָ��
��    �룺xa_codec_func_t           p_xa_process_api        tensilica����ں���ָ��
          xa_codec_handle_t         p_process_handle		codec���
          CODEC_MEMORY_CFG_STRU*    pstMemoryCfg
��    ����
�� �� ֵ��XA_NO_ERROR �ɹ�  ������ʧ��
*****************************************************************************/
VOS_INT32 XA_COMM_SetBlksMemory(xa_codec_func_t*           pfuncProcess,
                                xa_codec_handle_t          pstApiObj,
                                XA_COMM_MEMORY_CFG_STRU* pstMemoryCfg)
{
    VOS_INT32  err_code;
    VOS_INT32  swMemBlockNum;
    VOS_INT32  swSize;
    VOS_INT32  swAlignment;
    VOS_INT32  swType;
    VOS_VOID*  pvMemory;
    VOS_INT32  swMemIndex;

    if(   (VOS_NULL == pfuncProcess)
       || (VOS_NULL == pstApiObj)
       || (VOS_NULL == pstMemoryCfg) )
    {
        return XA_COM_CUSTOM_NULL_POINT_ERROR;
    }

    /* ��ȡ��Ҫ���ڴ������ */
    err_code = (*pfuncProcess)(pstApiObj,
                               XA_API_CMD_GET_N_MEMTABS,
                               0,
                               &swMemBlockNum);
    CODEC_XA_ERRCODE_CHECK(err_code);

    for(swMemIndex=0; swMemIndex<swMemBlockNum; swMemIndex++)
    {
        /* ��ȡ��Ҫ�ڴ��С*/
        err_code = (*pfuncProcess)(pstApiObj,
                                   XA_API_CMD_GET_MEM_INFO_SIZE,
                                   swMemIndex,
                                   &swSize);

        /* ������DDR�ж�̬����ķ�ʽ�����ڴ棬�ڴ��׵�ַ��Ϊ8�ֽڶ��� */
        pvMemory = (VOS_VOID *)UCOM_MemAlloc(swSize);

        if(VOS_NULL == pvMemory)
        {
            return XA_COM_CUSTOM_NULL_POINT_ERROR;
        }

        CODEC_XA_ERRCODE_CHECK(err_code);

        /* ��ѯ������뷽ʽ */
        err_code = (*pfuncProcess)(pstApiObj,
                                   XA_API_CMD_GET_MEM_INFO_ALIGNMENT,
                                   swMemIndex,
                                   &swAlignment);
        CODEC_XA_ERRCODE_CHECK(err_code);

        /* ֻ֧��1/2/4/8�ֽڶ��� */
        if (   (1 != swAlignment)
            && (2 != swAlignment)
            && (4 != swAlignment)
            && (8 != swAlignment))
        {
            return XA_COM_CUSTOM_NULL_POINT_ERROR;
        }

        /* ��ѯ�����ڴ����� */
        err_code = (*pfuncProcess)(pstApiObj,
                                   XA_API_CMD_GET_MEM_INFO_TYPE,
                                   swMemIndex,
                                   &swType);
        CODEC_XA_ERRCODE_CHECK(err_code);

        switch(swType)
        {
        case XA_MEMTYPE_PERSIST:

            /* ��¼�����ڴ���Ϣ */
            pstMemoryCfg->pPersistBuff      = pvMemory;
            pstMemoryCfg->swPersistMemSize  = swSize;
            break;
        case XA_MEMTYPE_SCRATCH:

            /* ��¼�����ڴ���Ϣ */
            pstMemoryCfg->pScratchBuff      = pvMemory;
            pstMemoryCfg->swScratchMemSize  = swSize;
            break;
        case XA_MEMTYPE_INPUT:

            /* ��¼�����ڴ���Ϣ */
            pstMemoryCfg->pInputBuff        = pvMemory;
            pstMemoryCfg->swInputMemSize    = swSize;
            break;
        case XA_MEMTYPE_OUTPUT:

            /* ��¼�����ڴ���Ϣ */
            pstMemoryCfg->pOutputBuff       = pvMemory;
            pstMemoryCfg->swOutputMemSize   = swSize;
            break;
        default:
            return XA_COM_CUSTOM_MEM_BLOCK_SIZE_ERROR;
        }

        /* �����ڴ�ָ�� */
        err_code = (*pfuncProcess)(pstApiObj,
                                   XA_API_CMD_SET_MEM_PTR,
                                   swMemIndex,
                                   pvMemory);
        CODEC_XA_ERRCODE_CHECK(err_code);
    }

    return err_code;
}

/*****************************************************************************
�� �� ����XA_COMM_MemoryAlloc
����������Ϊtensilica�Ŀ⺯�����������ڴ�ռ�
��    �룺xa_codec_func_t           p_xa_process_api        tensilica����ں���ָ��
          xa_codec_handle_t         p_process_handle        API������Ϣ
          CODEC_MEMORY_CFG_STRU*    pstMemoryCfg            ���ڼ�¼��ģ������̬������ڴ棬
                                                            �����˳���ģ��ʱ�ͷ��ڴ����ã�
                                                            ���øú�����ģ�鸺��Ϊ�ýṹ������ڴ�
��    ����
�� �� ֵ��XA_NO_ERROR �ɹ�  ������ʧ��
*****************************************************************************/
VOS_INT32  XA_COMM_MemoryAlloc (xa_codec_func_t*           pfuncProcess,
                                          xa_codec_handle_t           pstApiObj,
                                          XA_COMM_MEMORY_CFG_STRU*  pstMemoryCfg)
{
    VOS_INT32   err_code;
    VOS_INT32   swTableMemSize;
    VOS_VOID*   pTableBuff;

    if(   (VOS_NULL == pfuncProcess)
       || (VOS_NULL == pstApiObj) )
    {
        return XA_COM_CUSTOM_NULL_POINT_ERROR;
    }

    /* ��ȡ memory info tables ��С */
    err_code = (*pfuncProcess)(pstApiObj,
                               XA_API_CMD_GET_MEMTABS_SIZE,
                               0,
                               &(swTableMemSize));
    CODEC_XA_ERRCODE_CHECK(err_code);

    /* ������DDR�ж�̬����ķ�ʽ�����ڴ棬�ڴ��׵�ַ��Ϊ8�ֽڶ��� */
    pTableBuff = (VOS_VOID *)UCOM_MemAlloc(swTableMemSize);

    /* ��¼�����TableMem */
    pstMemoryCfg->pTableBuff     = pTableBuff;
    pstMemoryCfg->swTableMemSize = swTableMemSize;

    /* ���� memory tables ָ�� */
    err_code = (*pfuncProcess)(pstApiObj,
                               XA_API_CMD_SET_MEMTABS_PTR,
                               0,
                               pTableBuff);
    CODEC_XA_ERRCODE_CHECK(err_code);

    /* ���ò��� */
    err_code = (*pfuncProcess)(pstApiObj,
                               XA_API_CMD_INIT,
                               XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS,
                               NULL);
    CODEC_XA_ERRCODE_CHECK(err_code);

    err_code = XA_COMM_SetBlksMemory(pfuncProcess, pstApiObj, pstMemoryCfg);
    CODEC_XA_ERRCODE_CHECK(err_code);

    return VOS_OK;
}

/*****************************************************************************
�� �� ����XA_COMM_Init
����������ִ�г�ʼ������
��    �룺pfuncProcess      tensilica����ں���ָ��
          pstApiObj         codec���
          pvInputBuff
          uwInputBuffSize   buff�ĳ���
          pfuncReadFilecb   ��ȡ���ݵĽӿں���
��    ����
�� �� ֵ��XA_NO_ERROR �ɹ� ������ʧ��
��    ��:
*****************************************************************************/
VOS_INT32 XA_COMM_Init(xa_codec_func_t*   pfuncProcess,
                       xa_codec_handle_t  pstApiObj,
                       VOS_VOID*          pvInputBuff,
                       VOS_INT32          swInputSize,
                       VOS_VOID*          pvHeaderBuff,
                       VOS_INT32          swHeaderSize,
                       VOS_INT32         *pswLastConsumed)
{
    VOS_INT32  err_code;
    VOS_INT32  swInitDone       = 0;
    VOS_INT32  swBuffSize       = 0;            /* ������ʵ�������� */
    VOS_INT32  swConsumeSize    = swInputSize;  /* ÿ�ν����inputʹ�õ����� */
    VOS_INT32  swLoopCnt;

    //��������
    if(   (VOS_NULL == pfuncProcess)
       || (VOS_NULL == pstApiObj)
       || (VOS_NULL == pvInputBuff)
       || (VOS_NULL == pvHeaderBuff) )
    {
        return XA_COM_CUSTOM_NULL_POINT_ERROR;
    }

    swBuffSize              = swInputSize;
    *pswLastConsumed        = 0;
    swLoopCnt               = XA_COMM_MAX_INIT_CNT;

    //ѭ��ֱ����ʼ����ɻ���ʧ��
    do{
        /* ѭ���������� */
        swLoopCnt--;
        if(swLoopCnt <= 0)
        {
            return (VOS_INT32)XA_FATAL_ERROR;
        }

        /* �ƶ�inputbuff */
        XA_COMM_ShiftBuff((VOS_CHAR*)pvInputBuff, swInputSize, swConsumeSize);

        /* ��ͷ�ļ��ж�ȡ���ݵ�inputbuffer*/
        if(swHeaderSize >= swConsumeSize)
        {
            UCOM_MemCpy(&(((VOS_CHAR *)pvInputBuff)[swInputSize - swConsumeSize]), pvHeaderBuff, (VOS_UINT32)swConsumeSize);
            swHeaderSize -= swConsumeSize;
        }
        else
        /* ��ͷ�ļ��Ѿ���ȫ��ȡ�������INPUT_OVER�����ʾ��ȡ�����ݽ��� */
        {
            /*���������ʾ����׼������*/
            err_code = (*pfuncProcess)(pstApiObj,
                                       XA_API_CMD_INPUT_OVER,
                                       0,
                                       NULL);
            CODEC_XA_ERRCODE_CHECK(err_code);
        }

        /*����Ҫ��������ݳ���*/
        err_code = (*pfuncProcess)(pstApiObj,
                                   XA_API_CMD_SET_INPUT_BYTES,
                                   0,
                                   &swBuffSize);
        CODEC_XA_ERRCODE_CHECK(err_code);

        /*���ͳ�ʼ������*/
        err_code = (*pfuncProcess)(pstApiObj,
                                   XA_API_CMD_INIT,
                                   XA_CMD_TYPE_INIT_PROCESS,
                                   NULL);
        CODEC_XA_ERRCODE_CHECK(err_code);

        /*��ѯ��ʼ���Ƿ����*/
        err_code = (*pfuncProcess)(pstApiObj,
                                   XA_API_CMD_INIT,
                                   XA_CMD_TYPE_INIT_DONE_QUERY,
                                   &swInitDone);
        CODEC_XA_ERRCODE_CHECK(err_code);

        /*��ѯ���ĵ���������*/
        err_code = (*pfuncProcess)(pstApiObj,
                                   XA_API_CMD_GET_CURIDX_INPUT_BUF,
                                   0,
                                   &swConsumeSize);
        CODEC_XA_ERRCODE_CHECK(err_code);

        /* ����ͷ��Ϣָ�� */
        pvHeaderBuff        = (VOS_VOID *)((VOS_CHAR *)pvHeaderBuff + swConsumeSize);
        (*pswLastConsumed) += swConsumeSize;
    }while(!swInitDone);

    return err_code;
}

/*****************************************************************************
�� �� ����XA_COMM_Decode
��������������һ֡����
��    �룺pfuncProcess      tensilica����ں���ָ��
          pstApiObj         codec���
          pswInputSize      �������ݴ�С
��    ����puwOutputSize     ������ݴ�С
          pswConsumeSize    �������ݴ�С
          puwExecuteDone    �Ƿ񵽴��ļ�β
�� �� ֵ��XA_NO_ERROR �ɹ�; ����ʧ��
��    ��:
*****************************************************************************/
VOS_INT32 XA_COMM_Decode(xa_codec_func_t*  pfuncProcess,
                         xa_codec_handle_t pstApiObj,
                         VOS_INT32         swInputSize,
                         VOS_UINT32        enIsEndOfStream,
                         VOS_INT32*        pswOutputSize,
                         VOS_INT32*        pswConsumeSize,
                         VOS_INT32*        pswDecodeDone)
{
    VOS_INT32 err_code;

    //��μ��
    if(   (VOS_NULL == pfuncProcess)
       || (VOS_NULL == pstApiObj)
       || (VOS_NULL == pswOutputSize)
       || (VOS_NULL == pswConsumeSize)
       || (VOS_NULL == pswDecodeDone) )
    {
        return XA_COM_CUSTOM_NULL_POINT_ERROR;
    }

    if (AUDIO_PLAYER_POS_END_OF_STEAM == enIsEndOfStream)
    {
        /* ���������ʾ����׼�����*/
        err_code = (*pfuncProcess)(pstApiObj,
                                   XA_API_CMD_INPUT_OVER,
                                   0,
                                   NULL);
        CODEC_XA_ERRCODE_CHECK(err_code);
    }

    /* ���ô��������ݴ�С*/
    err_code = (*pfuncProcess)(pstApiObj,
                               XA_API_CMD_SET_INPUT_BYTES,
                               0,
                               &swInputSize);
    CODEC_XA_ERRCODE_CHECK(err_code);

    /* ���� */
    err_code = (*pfuncProcess)(pstApiObj,
                               XA_API_CMD_EXECUTE,
                               XA_CMD_TYPE_DO_EXECUTE,
                               NULL);
    CODEC_XA_ERRCODE_CHECK(err_code);

    /* ��ѯ�����Ƿ���� */
    err_code = (*pfuncProcess)(pstApiObj,
                               XA_API_CMD_EXECUTE,
                               XA_CMD_TYPE_DONE_QUERY,
                               pswDecodeDone);
    CODEC_XA_ERRCODE_CHECK(err_code);

    /* ��ѯ�����output����*/
    err_code = (*pfuncProcess)(pstApiObj,
                               XA_API_CMD_GET_OUTPUT_BYTES,
                               0,
                               pswOutputSize);
    CODEC_XA_ERRCODE_CHECK(err_code);

    /* ��ѯinputʹ�����ݳ��� */
    err_code = (*pfuncProcess)(pstApiObj,
                               XA_API_CMD_GET_CURIDX_INPUT_BUF,
                               0,
                               pswConsumeSize);
    CODEC_XA_ERRCODE_CHECK(err_code);

    return err_code;
}

/*****************************************************************************
�� �� ����ShiftBuff
��������: ���INPUTBUFF���������ݣ�ʣ�������ƶ���buff�׵�ַ
��    �룺pchBuff           buff��ַ
          uwBuffSize        buffer��С
          uwConsumeBytes    ���������ݴ�С
��    ������
�� �� ֵ����
��    ��:
*****************************************************************************/

VOS_VOID XA_COMM_ShiftBuff(VOS_CHAR* pchBuff, VOS_INT32 swBuffSize, VOS_INT32 swConsumeBytes)
{
    //�ƶ�ʣ�����ݵ�buffͷ
    VOS_INT32 swRemainBytes;
    VOS_INT32 swIndex;

    if( (VOS_NULL == pchBuff) || (swBuffSize<0) || (swConsumeBytes<0) || (swBuffSize < swConsumeBytes))
    {
        return;
    }

    swRemainBytes = swBuffSize - swConsumeBytes;

    /* ʣ�������ƶ���buff�׵�ַ */
    for (swIndex=0; swIndex<swRemainBytes; swIndex++)
    {
        pchBuff[swIndex] = pchBuff[swIndex + swConsumeBytes];
    }

    /* ������������� */
    UCOM_MemSet(&pchBuff[swRemainBytes], 0, (VOS_UINT32)swConsumeBytes);
}


VOS_INT32 XA_COMM_MemFree(XA_COMM_MEMORY_CFG_STRU* pstMemoryCfg)
{
    /* �쳣��μ�� */
    if (VOS_NULL == pstMemoryCfg)
    {
        return XA_COM_CUSTOM_NULL_POINT_ERROR;
    }

    /********************** �����ͷ���������ڴ� **********************/

    /* API memory�ͷ� */
    if (VOS_NULL != pstMemoryCfg->pApiStructBuff)
    {
        UCOM_MemFree(pstMemoryCfg->pApiStructBuff);
    }

    /* table memory�ͷ� */
    if (VOS_NULL != pstMemoryCfg->pTableBuff)
    {
        UCOM_MemFree(pstMemoryCfg->pTableBuff);
    }

    /* Persist memory�ͷ� */
    if (VOS_NULL != pstMemoryCfg->pPersistBuff)
    {
        UCOM_MemFree(pstMemoryCfg->pPersistBuff);
    }

    /* Scratch memory�ͷ� */
    if (VOS_NULL != pstMemoryCfg->pScratchBuff)
    {
        UCOM_MemFree(pstMemoryCfg->pScratchBuff);
    }

    /* Input memory�ͷ� */
    if (VOS_NULL != pstMemoryCfg->pInputBuff)
    {
        UCOM_MemFree(pstMemoryCfg->pInputBuff);
    }

    /* Output memory�ͷ� */
    if (VOS_NULL != pstMemoryCfg->pOutputBuff)
    {
        UCOM_MemFree(pstMemoryCfg->pOutputBuff);
    }

    /* ���������ڴ�ָ�롢�ڴ��С */
    UCOM_MemSet(pstMemoryCfg, 0, sizeof(XA_COMM_MEMORY_CFG_STRU));

    return VOS_OK;

}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif



