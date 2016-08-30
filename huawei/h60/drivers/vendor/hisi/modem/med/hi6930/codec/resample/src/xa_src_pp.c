

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "xa_src_pp.h"
#include "ucom_mem_dyn.h"
#include "codec_op_lib.h"
#include "om_log.h"
#ifndef _MED_C89_
#include <xtensa/tie/xt_hifi2.h>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
    ��ά�ɲ���Ϣ�а�����C�ļ���ź궨��
*****************************************************************************/
/*lint -e(767)*/
#define THIS_FILE_ID                    OM_FILE_ID_CODEC_XA_SRC_PP_C


/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/* SRC����ں���ָ�� */
xa_codec_func_t *                       g_pfuncAudioSrcApi;

/* SRC������� */
xa_codec_handle_t                       g_pastAudioSrcApiObj[XA_SRC_PROC_ID_BUTT + 1];

/* SRC ģ���ڴ�������Ϣ */
XA_COMM_MEMORY_CFG_STRU                 g_astSrcMemCfg[XA_SRC_PROC_ID_BUTT + 1];

/* SRCģ�鵱ǰ����״̬������Ϣ */
XA_SRC_CURRENT_CFG_STRU                 g_astSrcCurrentCfg[XA_SRC_PROC_ID_BUTT + 1];

/* ����tensilica��Ҫ�������������buff */
UCOM_ALIGN(8)
XA_SRC_IO_CFG_STRU                      g_astIOBuffCfg[XA_SRC_PROC_ID_BUTT + 1];

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_INT32 XA_SRC_Init( XA_SRC_PROC_ID_ENUM_UINT32 enProcId,
                           XA_SRC_USER_CFG_STRU * pstSrcUserCfg)
{
    xa_codec_func_t                    *pfuncSrcApi;
    xa_codec_handle_t                   pstSrcApiObj;
    XA_COMM_MEMORY_CFG_STRU            *pstSrcMemCfg;
    XA_SRC_CURRENT_CFG_STRU            *pstCurrentCfg;
    VOS_INT32                           swErrorCode     = VOS_OK;

    /* �쳣��μ�� */
    if (  (VOS_NULL == pstSrcUserCfg)
        ||(pstSrcUserCfg->swChannels > XA_SRC_MAX_CHANLE_NUM)
        ||(pstSrcUserCfg->swInputChunkSize > XA_SRC_MAX_INPUT_CHUNK_LEN)
        ||(enProcId > XA_SRC_PROC_ID_BUTT))
    {
        return VOS_ERR;
    }

    /* ����Ӧ����ID����� */
    XA_SRC_Close(enProcId);

    /* ��ȡ���� */
    XA_SRC_SetApiPtr(xa_src_pp);
    pfuncSrcApi     = XA_SRC_GetApiPtr();
    pstSrcMemCfg    = XA_SRC_GetMemCfgPtr(enProcId);
    pstCurrentCfg   = XA_SRC_GetCurrentCfgPtr(enProcId);

    /* ����XA_COMM_Startup��Ϊg_pstAudioSrcApiObj�����ڴ�ռ䲢������Ϊȱʡֵ */
    swErrorCode = XA_COMM_Startup(pfuncSrcApi, &(XA_SRC_GetApiObjPtr(enProcId)), pstSrcMemCfg);
    XA_ERROR_HANDLE(swErrorCode);

    /* ��APIObj��ʱ������ֵ */
    pstSrcApiObj = XA_SRC_GetApiObjPtr(enProcId);

    /* ����XA_SRC_SetPara�������û���Ϣ */
    swErrorCode = XA_SRC_SetPara(enProcId, pstSrcUserCfg);
    XA_ERROR_HANDLE(swErrorCode);

    /* ����XA_COMM_MemoryAlloc������memory tables */
    swErrorCode = XA_SRC_MemoryAlloc(pfuncSrcApi, pstSrcApiObj, pstSrcMemCfg, pstSrcUserCfg, enProcId);
    XA_ERROR_HANDLE(swErrorCode);

    /* �����������Buff */
    XA_SRC_SetInputBuff(enProcId, pstSrcMemCfg->pInputBuff);
    XA_SRC_SetOutputBuff(enProcId, pstSrcMemCfg->pOutputBuff);

    /* ��ʼ��SRCģ�� */
    swErrorCode = (*pfuncSrcApi)(pstSrcApiObj,
                                 XA_API_CMD_INIT,
                                 XA_CMD_TYPE_INIT_PROCESS,
                                 NULL);
    XA_ERROR_HANDLE(swErrorCode);

    /* ������ʷ����Buff */
    swErrorCode = (*pfuncSrcApi)(pstSrcApiObj,
                                 XA_API_CMD_EXECUTE,
                                 XA_CMD_TYPE_DO_RUNTIME_INIT,
                                 NULL);
    XA_ERROR_HANDLE(swErrorCode);

    /* �������buff��СΪ�� */
    //pstSrcUserCfg->swOutputSize = 0;

    /* ��¼���γ�ʼ�������õ���Ϣ */
    pstCurrentCfg->swChannels           = pstSrcUserCfg->swChannels;
    pstCurrentCfg->swInputChunkSize     = pstSrcUserCfg->swInputChunkSize;
    pstCurrentCfg->swInputSmpRat        = pstSrcUserCfg->swInputSmpRat;
    pstCurrentCfg->swOutputSmpRat       = pstSrcUserCfg->swOutputSmpRat;
    pstCurrentCfg->swOutputChunkSize    = 0;
    pstCurrentCfg->enInitFlag           = XA_SRC_INIT_OK;

    return XA_NO_ERROR;

}


#if 1
VOS_VOID XA_SRC_Convert16To24bit(XA_SRC_PROC_ID_ENUM_UINT32 enProcId,
                                         VOS_VOID *pSrcBuff,
                                         VOS_INT32 *pswDestBuff)
{
    XA_SRC_CURRENT_CFG_STRU            *pstCurrentCfg   = XA_SRC_GetCurrentCfgPtr(enProcId);
    VOS_INT16                          *pshwOriginal    = (VOS_INT16 *)pSrcBuff;
    VOS_INT32                           swTotalNum;
    VOS_INT32                           swCnt;

    /* ��ȡ����Buff������������� */
    swTotalNum = (pstCurrentCfg->swChannels) * (pstCurrentCfg->swInputChunkSize);

    /* ����ÿһ��16bit���������źţ���32bit��ʾ16bit
     *
     *  |      8bit    | 8bit | 8bit | 8bit |
     *  |sign extension|    ռ��λ   |   0  |
     */
    for (swCnt = 0; swCnt < swTotalNum; swCnt++)
    {
        pswDestBuff[swCnt] = pshwOriginal[swCnt];

        pswDestBuff[swCnt] <<= 8;
    }

}
#else
VOS_VOID XA_SRC_Convert16To24bit(XA_SRC_USER_CFG_STRU * pstSrcUserCfg, VOS_INT32 *pswInputBuff)
{
    VOS_INT16                          *pshwOriginal    = pstSrcUserCfg->pInputBuff;
    VOS_INT32                          *pswInput;
    VOS_INT32                           swTotalNum;
    VOS_INT32                           k;
    ae_p24x2s                           aepX1, aepX2, aepY1, aepY2;


    /* ��ȡ����Buff������������� */
    swTotalNum = ((pstSrcUserCfg->swChannels) * (pstSrcUserCfg->swInputChunkSize))/4;

    /* ����ÿһ��16bit���������źţ���32bit��ʾ16bit
     *
     *  |      8bit    | 8bit | 8bit | 8bit |
     *  |sign extension|    ռ��λ   |   0  |
     */

    WUR_AE_SAR(8);

    pswInput = pswInputBuff;

    for (k = 0; k < swTotalNum; k++)
    {
        aepX1 = *((ae_p16x2s *)&pshwOriginal[4*k]);
        aepX2 = *((ae_p16x2s *)&pshwOriginal[4*k + 2]);

        aepY1 = AE_SRASP24(aepX1);
        aepY2 = AE_SRASP24(aepX2);

        *((ae_p24x2f *) &pswInputBuff[4*k])     = aepY1;
        *((ae_p24x2f *) &pswInputBuff[4*k + 2]) = aepY2;
    }

}
#endif


#ifdef _MED_C89_
VOS_VOID  XA_SRC_ConvertChn24To16bit( VOS_INT16* pshwDst, VOS_INT32 *pswSrc, VOS_UINT32 uwLen)
{
    VOS_UINT32  i;

    /* ����ÿһ������ */
    for(i = 0; i < uwLen; i++)
    {
        /* ��������8λ */
        pswSrc[i] = CODEC_OpL_shr(pswSrc[i], 8);

        pshwDst[i] = (VOS_INT16)(pswSrc[i]);
     }
}
#else
VOS_VOID  XA_SRC_ConvertChn24To16bit( VOS_INT16* pshwDst, VOS_INT32 *pswSrc, VOS_UINT32 uwLen)
{
    VOS_INT32                           k;
    ae_p24x2s                           aepX1, aepX2, aepY1, aepY2;
    VOS_UINT32                          uwCnt;

    uwCnt = uwLen / 4;

    WUR_AE_SAR(8);

    for (k = 0; k < uwCnt; k++)
    {
        aepY1 = *((ae_p24x2f *) &pswSrc[4*k]);
        aepY2 = *((ae_p24x2f *) &pswSrc[4*k + 2]);

        aepX1 = AE_SLLSP24(aepY1);
        aepX2 = AE_SLLSP24(aepY2);

        *((ae_p16x2s *)&pshwDst[4*k])     = aepX1;
        *((ae_p16x2s *)&pshwDst[4*k + 2]) = aepX2;
    }

    if(uwLen & 0x2)
    {
        uwCnt = (uwLen & 0xFFFFFFFC);

        aepY1 = *((ae_p24x2f *) &pswSrc[uwCnt]);

        aepX1 = AE_SLLSP24(aepY1);

        *((ae_p16x2s *)&pshwDst[uwCnt])     = aepX1;
    }

    if(uwLen & 0x1)
    {
        aepY1 = *((ae_p24f *) &pswSrc[uwLen - 1]);

        aepX1 = AE_SLLSP24(aepY1);

        *((ae_p16s *)&pshwDst[uwLen - 1])     = aepX1;
    }

}
#endif



VOS_VOID XA_SRC_Convert24To16bit( XA_SRC_PROC_ID_ENUM_UINT32 enProcId,
                                          VOS_INT32 **ppuwSrcBuff,
                                          VOS_VOID  *pDestBuff)
{
    XA_SRC_CURRENT_CFG_STRU            *pstCurrentCfg           = XA_SRC_GetCurrentCfgPtr(enProcId);
    VOS_UINT32                          uwChannleNum;
    VOS_INT16                          *pshwOut;

    pshwOut = (VOS_INT16*)pDestBuff;

    /* ����ÿһ��SRC�����24bit���ݣ�����ת��Ϊ16bit����ȡ��16bit
     *
     *  | 8bit | 8bit | 8bit | 8bit |
     *  |   0  |    ��ȡλ   |   0  |
     */
    for(uwChannleNum = 0; uwChannleNum < (VOS_UINT32)pstCurrentCfg->swChannels; ++uwChannleNum)
    {
        XA_SRC_ConvertChn24To16bit(
                    &pshwOut[uwChannleNum * (VOS_UINT32)pstCurrentCfg->swOutputChunkSize],
                    ppuwSrcBuff[uwChannleNum],
                    (VOS_UINT32)pstCurrentCfg->swOutputChunkSize);
    }

}



VOS_INT32 XA_SRC_SetPara(XA_SRC_PROC_ID_ENUM_UINT32 enProcId,
                              XA_SRC_USER_CFG_STRU *pstSrcUserCfg)
{
    xa_codec_func_t *                   pfuncSrcApi     = XA_SRC_GetApiPtr();
    xa_codec_handle_t                   pstSrcApiObj    = XA_SRC_GetApiObjPtr(enProcId);
    VOS_INT32                           swErrorCode     = 0;

    /* ������������� */
    swErrorCode = (*pfuncSrcApi)(pstSrcApiObj,
                                        XA_API_CMD_SET_CONFIG_PARAM,
                                        XA_SRC_PP_CONFIG_PARAM_INPUT_SAMPLE_RATE,
                                       &(pstSrcUserCfg->swInputSmpRat));
    XA_ERROR_HANDLE(swErrorCode);

    /* ������������� */
    swErrorCode = (*pfuncSrcApi)(pstSrcApiObj,
                                       XA_API_CMD_SET_CONFIG_PARAM,
                                       XA_SRC_PP_CONFIG_PARAM_OUTPUT_SAMPLE_RATE,
                                       &(pstSrcUserCfg->swOutputSmpRat));
    XA_ERROR_HANDLE(swErrorCode);

    /* ������������������������ */
    swErrorCode = (*pfuncSrcApi)(pstSrcApiObj,
                                       XA_API_CMD_SET_CONFIG_PARAM,
                                       XA_SRC_PP_CONFIG_PARAM_INPUT_CHANNELS,
                                       &(pstSrcUserCfg->swChannels));
    XA_ERROR_HANDLE(swErrorCode);

    /* ���������������������(chunk size) */
    swErrorCode = (*pfuncSrcApi)(pstSrcApiObj,
                                       XA_API_CMD_SET_CONFIG_PARAM,
                                       XA_SRC_PP_CONFIG_PARAM_INPUT_CHUNK_SIZE,
                                       &(pstSrcUserCfg->swInputChunkSize));
    XA_ERROR_HANDLE(swErrorCode);

    return VOS_OK;
}


VOS_UINT32 XA_SRC_ExeProcess( XA_SRC_PROC_ID_ENUM_UINT32 enProcId,
                                     VOS_VOID   *pInputBuff,
                                     VOS_VOID   *pOutputBuff,
                                     VOS_UINT32 *puwOutputBuffSize)
{
    xa_codec_func_t                    *pfuncSrcApi;
    xa_codec_handle_t                   pstSrcApiObj;
    XA_SRC_CURRENT_CFG_STRU            *pstCurrentCfg;
    VOS_INT32                           swErrorCode;

    /* �쳣��μ�� */
    if (  (VOS_NULL == pInputBuff)
        ||(VOS_NULL == pOutputBuff)
        ||(XA_SRC_PROC_ID_BUTT <= enProcId))
    {
        OM_LogError1(XA_SRC_Para_Error, enProcId);

        return VOS_ERR;
    }

    /* ��ȡ���� */
    pfuncSrcApi     = XA_SRC_GetApiPtr();
    pstCurrentCfg   = XA_SRC_GetCurrentCfgPtr(enProcId);
    pstSrcApiObj    = XA_SRC_GetApiObjPtr(enProcId);

    /* ���δ��ʼ�����򷵻�err */
    if (XA_SRC_INIT_OK != pstCurrentCfg->enInitFlag)
    {
        OM_LogError1(XA_SRC_Para_Error, pstCurrentCfg->enInitFlag);

        return VOS_ERR;
    }

    XA_SRC_Convert16To24bit(enProcId, pInputBuff, g_astIOBuffCfg[enProcId].paswInputBuff[0]);

    /* ��������Buff��������� */
    swErrorCode = (*pfuncSrcApi)(pstSrcApiObj,
                                 XA_API_CMD_SET_CONFIG_PARAM,
                                 XA_SRC_PP_CONFIG_PARAM_INPUT_CHUNK_SIZE,
                                 &(pstCurrentCfg->swInputChunkSize));
    XA_ERROR_HANDLE(swErrorCode);

    /* �������롢���Buff��Ҫ���׵�ַ8�ֽڶ��� */
    swErrorCode = (*pfuncSrcApi)(pstSrcApiObj,
                                 XA_API_CMD_SET_CONFIG_PARAM,
                                 XA_SRC_PP_CONFIG_PARAM_SET_INPUT_BUF_PTR,
                                 g_astIOBuffCfg[enProcId].paswInputBuff);
    XA_ERROR_HANDLE(swErrorCode);

    swErrorCode = (*pfuncSrcApi)(pstSrcApiObj,
                                 XA_API_CMD_SET_CONFIG_PARAM,
                                 XA_SRC_PP_CONFIG_PARAM_SET_OUTPUT_BUF_PTR,
                                 g_astIOBuffCfg[enProcId].paswOutputBuff);
    XA_ERROR_HANDLE(swErrorCode);

    /* ִ�б���� */
    swErrorCode = (*pfuncSrcApi)(pstSrcApiObj,
                                 XA_API_CMD_EXECUTE,
                                 XA_CMD_TYPE_DO_EXECUTE,
                                 NULL);
    XA_ERROR_HANDLE(swErrorCode);

    /* ��ȡ�����֮��chunk size(���������) */
    swErrorCode = (*pfuncSrcApi)(pstSrcApiObj,
                                 XA_API_CMD_GET_CONFIG_PARAM,
                                 XA_SRC_PP_CONFIG_PARAM_OUTPUT_CHUNK_SIZE,
                                 &(pstCurrentCfg->swOutputChunkSize));
    XA_ERROR_HANDLE(swErrorCode);

    /* ����ģ���ڲ���� */
    pstCurrentCfg->pswSrcOutput = g_astIOBuffCfg[enProcId].paswOutputBuff[0];

    /* ��SRCģ�������24bit����ת��Ϊ16bit */
    XA_SRC_Convert24To16bit(enProcId, g_astIOBuffCfg[enProcId].paswOutputBuff, pOutputBuff);

    /* ������������ */
    *puwOutputBuffSize = (VOS_UINT32)(pstCurrentCfg->swChannels * pstCurrentCfg->swOutputChunkSize) * sizeof(VOS_UINT16);

    return VOS_OK;

}


VOS_UINT32 XA_SRC_Close(XA_SRC_PROC_ID_ENUM_UINT32 enProcId)
{
    xa_codec_handle_t                   pstSrcApiObj    = XA_SRC_GetApiObjPtr(enProcId);
    XA_COMM_MEMORY_CFG_STRU            *pstSrcMemCfg    = XA_SRC_GetMemCfgPtr(enProcId);
    XA_SRC_CURRENT_CFG_STRU            *pstCurrentCfg   = XA_SRC_GetCurrentCfgPtr(enProcId);
    VOS_UINT32                          uwCnt;

    /* ���վ�� */
    XA_SRC_SetApiPtr(VOS_NULL);

    /* �ͷ�ApiObj�ռ� */
    UCOM_MemFree(pstSrcApiObj);
    XA_SRC_SetApiObjPtr(enProcId, VOS_NULL);

    /* �ͷ����ж�̬����ռ� */
    XA_COMM_MemFree(pstSrcMemCfg);

    /* ��ǰSRC������Ϣ��� */
    UCOM_MemSet(pstCurrentCfg, 0, sizeof(XA_SRC_CURRENT_CFG_STRU));

    /* ��ʽ��־��ʼ��λ */
    pstCurrentCfg->enInitFlag = XA_SRC_INIT_NO;

    /* ����ڲ�ģ���������ָ�� */
    for (uwCnt = 0; uwCnt < 8; uwCnt++)
    {
        g_astIOBuffCfg[enProcId].paswInputBuff[uwCnt]  = VOS_NULL;
        g_astIOBuffCfg[enProcId].paswOutputBuff[uwCnt] = VOS_NULL;
    }

    return VOS_OK;

}


/*****************************************************************************
�� �� ����XA_COMM_SetBlksMemory
����������Tensilica�����ø����ڴ���ָ��
��    �룺xa_codec_func_t           p_xa_process_api        tensilica����ں���ָ��
          xa_codec_handle_t         p_process_handle        codec���
          CODEC_MEMORY_CFG_STRU*    pstMemoryCfg
��    ����
�� �� ֵ��XA_NO_ERROR �ɹ�  ������ʧ��
*****************************************************************************/
VOS_INT32 XA_SRC_SetBlksMemory(xa_codec_func_t        *pfuncProcess,
                                xa_codec_handle_t               pstApiObj,
                                XA_COMM_MEMORY_CFG_STRU        *pstMemoryCfg,
                                XA_SRC_USER_CFG_STRU           *pstSrcUserCfg,
                                XA_SRC_PROC_ID_ENUM_UINT32      enProcId)
{
    VOS_INT32  err_code;
    VOS_INT32  swMemBlockNum;
    VOS_INT32  swSize;
    VOS_INT32  swAlignment;
    VOS_INT32  swType;
    VOS_VOID*  pvMemory;
    VOS_INT32  swMemIndex;
    VOS_INT*   ptemp;
    VOS_INT32  swCnt;
    VOS_INT32  swOutput;

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

        /* ֻ֧��8�ֽڶ��� */
        if (    (1 != swAlignment) &&
                (2 != swAlignment) &&
                (4 != swAlignment) &&
                (8 != swAlignment))
        {
            return VOS_ERR;
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

            /* ����ppin_buff */
            ptemp = (VOS_INT32 *)pvMemory;
            for(swCnt= 0; swCnt < pstSrcUserCfg->swChannels; ++swCnt)
            {
                //g_paswInputBuff[swCnt] = (VOS_INT32 *)(ptemp + swCnt);
                g_astIOBuffCfg[enProcId].paswInputBuff[swCnt] = (VOS_INT32 *)(ptemp + swCnt);
            }

            break;
        case XA_MEMTYPE_OUTPUT:

            /* ��¼�����ڴ���Ϣ */
            pstMemoryCfg->pOutputBuff       = pvMemory;
            pstMemoryCfg->swOutputMemSize   = swSize;

            /* �������buff */
            ptemp = (VOS_INT32 *)pvMemory;
            swOutput = pstMemoryCfg->swOutputMemSize/((pstSrcUserCfg->swChannels) * (VOS_INT32)sizeof(VOS_INT32));
            for(swCnt = 0; swCnt < pstSrcUserCfg->swChannels; ++swCnt)
            {
                //g_paswOutputBuff[swCnt] = (VOS_INT32 *)(ptemp + (swCnt * swOutput));
                g_astIOBuffCfg[enProcId].paswOutputBuff[swCnt] = (VOS_INT32 *)(ptemp + (swCnt * swOutput));
            }

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
VOS_INT32  XA_SRC_MemoryAlloc (xa_codec_func_t                  *pfuncProcess,
                                       xa_codec_handle_t                 pstApiObj,
                                       XA_COMM_MEMORY_CFG_STRU          *pstMemoryCfg,
                                       XA_SRC_USER_CFG_STRU             *pstSrcUserCfg,
                                       XA_SRC_PROC_ID_ENUM_UINT32        enProcId)
{
    VOS_INT32   err_code;
    VOS_INT32   swTableMemSize;
    VOS_VOID*   pTableBuff;

    if(   (VOS_NULL == pfuncProcess)
       || (VOS_NULL == pstApiObj)
       || (VOS_NULL == pstMemoryCfg)
       || (VOS_NULL == pstSrcUserCfg))
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
    pstMemoryCfg->pTableBuff        = pTableBuff;
    pstMemoryCfg->swTableMemSize    = swTableMemSize;

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

    err_code = XA_SRC_SetBlksMemory(pfuncProcess, pstApiObj, pstMemoryCfg, pstSrcUserCfg, enProcId);
    CODEC_XA_ERRCODE_CHECK(err_code);

    return VOS_OK;
}
VOS_UINT32  XA_SRC_Proc8x(XA_SRC_PROC_ID_ENUM_UINT32   enProcId,
                                VOS_INT16                   *pshwInputBuff,
                                VOS_INT16                   *pshwOutputBuff,
                                VOS_UINT32                  *puwOutPutSize,
                                VOS_UINT32                   uwProcTimes)
{
    VOS_VOID                *pInputBuffTmp;
    VOS_VOID                *pOutputBuffTmp;
    XA_SRC_CURRENT_CFG_STRU *pstCurrentCfg;
    VOS_UINT32               uwOutSize;
    VOS_UINT32               uwCnt;
    VOS_UINT32               uwRet;

    /* �쳣��μ�� */
    if (  (XA_SRC_PROC_ID_BUTT <= enProcId)
        ||(VOS_NULL == pshwInputBuff)
        ||(VOS_NULL == pshwOutputBuff))
    {
        OM_LogError(XA_SRC_Para_Error);

        return VOS_ERR;
    }

    /* ��ȡ��Ӧ������ */
    pstCurrentCfg = XA_SRC_GetCurrentCfgPtr(enProcId);

    /* �������buff */
    pOutputBuffTmp = (VOS_VOID *)pshwOutputBuff;

    /* �������buff��С */
    *puwOutPutSize = 0;

    /* ���÷ֶδ���ÿ�δ���AUDIO_PLAYER_CHUNK_SIZE�������� */
    for (uwCnt = 0; uwCnt < uwProcTimes; uwCnt++)
    {
        /* ��������buff */
        pInputBuffTmp = (VOS_VOID *)(pshwInputBuff + \
                                    (uwCnt * (VOS_UINT32)(pstCurrentCfg->swInputChunkSize* pstCurrentCfg->swChannels)));

        /* ִ�б���� */
        uwRet = XA_SRC_ExeProcess(enProcId,
                                  pInputBuffTmp,
                                  pOutputBuffTmp,
                                  &uwOutSize);
        /* ִ���쳣������ */
        if(uwRet != VOS_OK)
        {
            OM_LogError1(XA_SRC_ExeProcess_Fail, uwRet);

            return VOS_ERR;
        }

        /* �����������ָ�� */
        pOutputBuffTmp = (VOS_INT16*)pOutputBuffTmp + \
                      (uwOutSize/(VOS_INT32)sizeof(VOS_INT16));

        /* �������buff��С */
        *puwOutPutSize = *puwOutPutSize + uwOutSize;

    }

    return VOS_OK;

}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

