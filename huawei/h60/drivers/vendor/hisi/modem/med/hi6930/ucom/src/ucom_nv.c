

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include  "ucom_nv.h"
#include  "ucom_share.h"


#ifndef __UCOM_NV_C__
#define __UCOM_NV_C__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*lint -e(767)*/
#define    THIS_FILE_ID         OM_FILE_ID_UCOM_NV_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/* ����NV���������׵�ַ(�����ַ),ȫ�ֱ��������ɸ��ģ���NV_Internalfunc.h�б���һ�� */
VOS_UINT32                      g_ulNVMemAddr;

/* ����NV���������׵�ַ(�����ַ)*/
VOS_UINT32                      g_ulNVMemAddrPhy;


/* ָ���Ÿ�NVʵ�����ݵ��ڴ��׵�ַ */
NV_CONTROL_FILE_INFO_STRU      *g_pstUcomNvDataBuf;

/* NV's ID coms from 1 to 20 */
NV_FILE_HANDLE_STRU             g_astUcomNvFileHandle[NV_BACKUP_FILE_MAX_NUM + 1];

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_UINT32 UCOM_NV_Init(VOS_VOID)
{
    VOS_UINT32                          ulOffset = 0;
    VOS_UINT32                          i;
    VOS_UINT32                         *puwMagicNum;
    NV_FILE_LIST_INFO_STRU             *pstFileListInfo;
    CARM_HIFI_DYN_ADDR_SHARE_STRU      *pstShare = UCOM_GetCarmShareAddrStru();

    /*����NV�������׵�ַ��ARM���ַ*/
    UCOM_SaveNvMemAddr(pstShare->uwNvBaseAddrVirt);

    /*����NV�������׵�ַ�������ַ*/
    UCOM_SaveNvMemAddrPhy(pstShare->uwNvBaseAddrPhy);

    puwMagicNum = (VOS_UINT32 *)(UCOM_NV_DATA_BASE_ADDR);

    /* ���CCPU�Ƿ��Ѿ����NV��ʼ����д�������,NV_MAGIC_NUM_LEN��λbyte */
    while (NV_MAGIC_NUM != *puwMagicNum)
    {
        UCOM_COMM_Sleep((VOS_UINT32)10000);

        /*����NV�������׵�ַ��ARM���ַ*/
        UCOM_SaveNvMemAddr(pstShare->uwNvBaseAddrVirt);

        /*����NV�������׵�ַ�������ַ*/
        UCOM_SaveNvMemAddrPhy(pstShare->uwNvBaseAddrPhy);

        puwMagicNum = (VOS_UINT32 *)(UCOM_NV_DATA_BASE_ADDR);

        OM_LogError(UCOM_NV_Init_NvDataUnInit);
    }

    /* ��ʼ��ȫ�ֱ���ָ��NVʵ������������׵�ַ */
    UCOM_SetNvDataBufPtr(UCOM_NV_DATA_BASE_ADDR);

    g_astUcomNvFileHandle[0].ulOffset = ulOffset;
    g_astUcomNvFileHandle[0].ulSize = g_pstUcomNvDataBuf->ulCtrlFileSize;

    ulOffset += g_astUcomNvFileHandle[0].ulSize;

    pstFileListInfo = (NV_FILE_LIST_INFO_STRU *)((VOS_CHAR *)g_pstUcomNvDataBuf + sizeof(NV_CONTROL_FILE_INFO_STRU));

    for(i = 1; i <= g_pstUcomNvDataBuf->ulFileListNum; i++)
    {
        g_astUcomNvFileHandle[i].ulOffset = ulOffset;
        g_astUcomNvFileHandle[i].ulSize   = pstFileListInfo[i-1].ulFileSize;

        ulOffset += pstFileListInfo[i-1].ulFileSize;
    }

    return NV_OK;
}


VOS_VOID UCOM_NVIM_MemCpy( VOS_VOID * Dest, const VOS_VOID * Src,  VOS_UINT32 ulnbytes)
{
    VOS_UINT8           *tmp = (VOS_UINT8 *)Dest;
    VOS_UINT8           *s   = (VOS_UINT8 *)Src;

    while (ulnbytes--)
    {
        *tmp++ = *s++;
    }

    tmp = VOS_NULL;
    s   = VOS_NULL;

    return;
}


VOS_VOID UCOM_NV_MemCpy( VOS_VOID * Dest, const VOS_VOID * Src,  VOS_UINT32 ulnbytes)
{
    if ( ((VOS_UINT32)Src < UCOM_NV_DATA_BASE_ADDR)                                  /*С��NV������������ʼ��ַ*/
        ||((VOS_UINT32)Src > (UCOM_NV_DATA_BASE_ADDR + NV_BUFFER_SIZE))              /*������NV�����������ս��ַ*/
        ||(((VOS_UINT32)Src + ulnbytes) > (VOS_UINT32)(UCOM_NV_DATA_BASE_ADDR + NV_BUFFER_SIZE)) )      /*�������ݴ�����NV�����������ս��ַ*/
    {
        /* �����¼ */
        OM_LogError(UCOM_NV_MemCpy_ParaInvalid);

        return;
    }

    UCOM_NVIM_MemCpy(Dest, Src, ulnbytes);

    return;
}


VOS_UINT32 UCOM_NV_Ctrl_File_Search_InMemory(VOS_UINT16                  usID,
                                                NV_CONTROL_FILE_INFO_STRU   *pstNVCtrlInfo,
                                                NV_FILE_LIST_INFO_STRU      *pstFileListInfo,
                                                NV_REFERENCE_DATA_INFO_STRU *pstNvIdInfo)
{
    VOS_UINT32                          ulLow;
    VOS_UINT32                          ulHigh;
    VOS_UINT32                          ulMid;
    VOS_UINT32                          ulOffset;
    VOS_CHAR                           *pfileContent;

    if(VOS_NULL_PTR == pstNVCtrlInfo)
    {
        OM_LogError(UCOM_NV_FileSearch_ParaError);

        return VOS_ERR;
    }

    pfileContent = (VOS_CHAR *)pstNVCtrlInfo;

    /* Get total ID Num */
    ulHigh = pstNVCtrlInfo->ulNvRefCount;
    ulLow  = 1;

    /* Binary Search */
    while(ulLow <= ulHigh)
    {
        ulMid = (ulLow + ulHigh)/2;

        /* Seek the fp to the beginning of ulMid ID */
        ulOffset = sizeof(NV_CONTROL_FILE_INFO_STRU) +
                        (sizeof(NV_FILE_LIST_INFO_STRU)*(pstNVCtrlInfo->ulFileListNum))
                        +(sizeof(NV_REFERENCE_DATA_INFO_STRU)*(ulMid-1));

        /* Get the Id info of the ulMid ID */
        UCOM_NVIM_MemCpy(pstNvIdInfo, pfileContent + ulOffset, sizeof(NV_REFERENCE_DATA_INFO_STRU));

        if(usID < pstNvIdInfo->usNvId)
        {
            ulHigh = ulMid - 1;
        }
        else if(usID > pstNvIdInfo->usNvId)
        {
            ulLow = ulMid + 1;
        }
        else
        {
            /* Get the file info of this ID */
            ulOffset = sizeof(NV_CONTROL_FILE_INFO_STRU) +
                                (sizeof(NV_FILE_LIST_INFO_STRU)* (pstNvIdInfo->usFileId-1));

            UCOM_NVIM_MemCpy(pstFileListInfo, pfileContent + ulOffset, sizeof(NV_FILE_LIST_INFO_STRU));

            return VOS_OK;
         }
    }

    /* ID not exist */
    return VOS_ERR;
}


VOS_UINT32 UCOM_NV_NvIdCheck(MODEM_ID_ENUM_UINT16               enModemID,
                        VOS_UINT16                              usID,
                        VOS_UINT32                              ulOffset,
                        VOS_VOID                               *pItem,
                        VOS_UINT32                              ulLength,
                        NV_ID_RETUEN_INFO_STRU                 *pstNvIdReturnInfo)
{
    NV_FILE_LIST_INFO_STRU              stFileListInfo;
    NV_REFERENCE_DATA_INFO_STRU         stNvIdInfo;

    UCOM_MemSet(&stFileListInfo, 0, sizeof(NV_FILE_LIST_INFO_STRU));
    UCOM_MemSet(&stNvIdInfo, 0, sizeof(NV_REFERENCE_DATA_INFO_STRU));

    if(MODEM_ID_BUTT <= enModemID)
    {
        OM_LogError1(UCOM_NV_Read_ModemIDError, enModemID);
        return NV_MODEM_ID_ERR;
    }

    if ((VOS_NULL_PTR == pItem)||(0 == ulLength))
    {
        /* Buffer is Null */
        OM_LogError2(UCOM_NV_Read_ParaError, pItem, ulLength);
        return NV_BUFFER_NOT_EXIST;
    }

    if (VOS_OK != UCOM_NV_Ctrl_File_Search_InMemory(usID, g_pstUcomNvDataBuf, &stFileListInfo, &stNvIdInfo))
    {
        /* ID not exist */
        OM_LogError(UCOM_NV_Read_NvNotExist);
        return NV_ID_NOT_EXIST;
    }

    if ((stNvIdInfo.usNvLength < ulOffset)||
            (ulLength > (VOS_UINT32)(stNvIdInfo.usNvLength- ulOffset)))
    {
        /* Offset large than NV data len. or buffer len. large than remain NV data */
        OM_LogError(UCOM_NV_Read_LenTooLong);
        return NV_PART_DATA_ERR;
    }

    UCOM_NVIM_MemCpy(pstNvIdReturnInfo->aucFileName, stFileListInfo.aucFileName,sizeof(stFileListInfo.aucFileName));

    pstNvIdReturnInfo->usFileId     = (VOS_UINT16)stFileListInfo.ulFileId;
    pstNvIdReturnInfo->ucModemNum   = stNvIdInfo.ucModemNum;
    pstNvIdReturnInfo->usProperty   = stNvIdInfo.usProperty;

    /* ���԰������������ȡ����е�ָ������ */
    if((enModemID + 1) <= stNvIdInfo.ucModemNum)
    {
        /* �ļ���NV����д���ƫ���� */
        pstNvIdReturnInfo->ulNvOffset = stNvIdInfo.ulNvOffset
                                        + (enModemID * stNvIdInfo.usNvLength)
                                        + ulOffset;

        /* �����е�ƫ���� = XXNV.bin�ڻ����е�ƫ���� + NVID��XXNV.bin�е�ƫ���� */
        /*ulFileId�Ǵ�1��ʼ�ۼ� */
        pstNvIdReturnInfo->ulTotalOffset = (g_astUcomNvFileHandle[stNvIdInfo.usFileId].ulOffset
                                            + stNvIdInfo.ulNvOffset)
                                            + (enModemID * stNvIdInfo.usNvLength)
                                            + ulOffset;
    }
    else    /* ֻ�ܶ�ȡ��һ������ */
    {
        pstNvIdReturnInfo->ulNvOffset = stNvIdInfo.ulNvOffset + ulOffset;

        /* �����е�ƫ���� = XXNV.bin�ڻ����е�ƫ���� + NVID��XXNV.bin�е�ƫ���� + ulOffset */
        pstNvIdReturnInfo->ulTotalOffset = g_astUcomNvFileHandle[stNvIdInfo.usFileId].ulOffset
                                            + stNvIdInfo.ulNvOffset
                                            + ulOffset;   /*ulFileId�Ǵ�1��ʼ�ۼ� */
    }


    return NV_OK;
}


VOS_UINT32 UCOM_NV_ReadPartEx(MODEM_ID_ENUM_UINT16              enModemID,
                                VOS_UINT16                      usID,
                                VOS_UINT32                      ulOffset,
                                VOS_VOID                        *pItem,
                                VOS_UINT32                      ulLength)
{
    NV_ID_RETUEN_INFO_STRU              stNvIdReturnInfo;
    VOS_UINT32                          ulResult;

    ulResult = UCOM_NV_NvIdCheck(enModemID, usID, ulOffset, pItem, ulLength, &stNvIdReturnInfo);

    if(NV_OK != ulResult)
    {
        return ulResult;
    }

    /*�ӻ����ж�ȡNV����*/
    UCOM_NV_MemCpy((VOS_CHAR*)pItem, ((VOS_CHAR*)g_pstUcomNvDataBuf + stNvIdReturnInfo.ulTotalOffset), ulLength);

    return NV_OK;
}
VOS_UINT32 UCOM_NV_Read(VOS_UINT16 uhwId, VOS_VOID *pItem, VOS_UINT32 uwLength)
{
    /* ֱ�ӵ��� NV_ReadPart��offset ����Ϊ0 */
    return UCOM_NV_ReadPartEx(MODEM_ID_0, uhwId, 0, pItem, uwLength);
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
#endif


