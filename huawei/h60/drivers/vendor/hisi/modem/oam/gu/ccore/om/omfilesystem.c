
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*lint -e767 �޸���:���� 47350;ԭ��:Log��ӡ */
#define    THIS_FILE_ID        PS_FILE_ID_OMFS_C
/*lint +e767 �޸���:���� 47350;*/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/

#include "omfilesystem.h"
#include "pslogadapter.h"
#include "pslog.h"
/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
VOS_CHAR g_acDumpDir[100];

/*LOG�����������ļ����ڵ�Ŀ¼*/
#if ((VOS_OS_VER == VOS_VXWORKS)||(VOS_OS_VER == VOS_RTOSCK))
#if (FEATURE_ON == FEATURE_MANUFACTURE_LOG)
#define OM_FILE_SYSTEM_DUMP_DIR             "/yaffs2/Exc"
#define OM_FILE_SYSTEM_UNITARY_DUMP_DIR     "/modem_log/Exc"
#else
#if (FEATURE_ON == FEATURE_MULTI_FS_PARTITION) /* SFT board*/
#define OM_FILE_SYSTEM_DUMP_DIR             "/data/modemlog/Exc"
#define OM_FILE_SYSTEM_UNITARY_DUMP_DIR     "/modem_log/Exc"
#else
#define OM_FILE_SYSTEM_DUMP_DIR             "/yaffs0/Exc"
#define OM_FILE_SYSTEM_UNITARY_DUMP_DIR     "/modem_log/Exc"
#endif
#endif

#elif (VOS_WIN32 == VOS_OS_VER)
#define OM_FILE_SYSTEM_DUMP_DIR             ""
#define OM_FILE_SYSTEM_UNITARY_DUMP_DIR     ""
#endif
extern VOS_CHAR g_acLogDir[];

#if (FEATURE_ON == FEATURE_BROWSER_NV_FILE_IMEI_PROTECT)
extern VOS_UINT32 OM_QueryPrivilegeLevel(VOS_VOID);
extern VOS_BOOL NV_CheckNVFile(VOS_CHAR  *pcFilePath);
#endif
/* �������ļ�����Ϣ */
OM_FILE_INFO_STRU g_stOmFileInfo = {DRV_FILE_NULL, OM_FILE_BUTT, VOS_NULL_PTR};

/* �������һ���ϴ������ߵĴ���ֵ */
VOS_UINT32        g_ulOmSendToolErrNo = 0;
/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_UINT32 OM_QueryDir(APP_OM_MSG_EX_STRU *pstAppToOmMsg, VOS_UINT16 usReturnPrimId)
{
    VOS_UINT32             ulFolderType;
    OM_APP_MSG_EX_STRU    *pstOmToAppMsg;
    OM_APP_QUERY_DIR_STRU *pstOmQueryDir;
    VOS_UINT32             ulTotalSize;

    ulTotalSize = OM_APP_MSG_EX_LEN + sizeof(OM_APP_QUERY_DIR_STRU);

    pstOmToAppMsg = (OM_APP_MSG_EX_STRU*)VOS_MemAlloc(WUEPS_PID_OM, DYNAMIC_MEM_PT, ulTotalSize);
    if (VOS_NULL_PTR == pstOmToAppMsg)
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_QueryDir:VOS_MemAlloc fail!");
        OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_FILE_MEM_ALLOC_ERR, usReturnPrimId);
        return VOS_ERR;
    }
    pstOmQueryDir = (OM_APP_QUERY_DIR_STRU*)(pstOmToAppMsg->aucPara);
    /*�ж������ļ��е�����*/
    ulFolderType = *((VOS_UINT32*)(pstAppToOmMsg->aucPara));

    /*��־�ļ����ڵ�·��*/
    if (OM_ROOT_FOLDER == ulFolderType)
    {
        /* ������ӿڻ�ȡ�ļ�ϵͳ·�� */
        pstOmQueryDir->ulLength = DRV_NAME_MAX;

        VOS_MemSet(pstOmQueryDir->aucDirPath, 0, DRV_NAME_MAX + 1);
        if(VOS_OK != DRV_GET_FS_ROOT_DIRECTORY((VOS_CHAR*)pstOmQueryDir->aucDirPath,
                                               pstOmQueryDir->ulLength))
        {
            VOS_MemFree(WUEPS_PID_OM, pstOmToAppMsg);
            PS_LOG1(WUEPS_PID_OM, 0, PS_PRINT_ERROR,
                    "OM_QueryDir: Get fs root directory from Drv fail!", (VOS_INT32)ulFolderType);
            OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_DRV_GET_DIRECTORY_ERR, usReturnPrimId);

            return VOS_ERR;
        }

        g_stOmFileInfo.ulFileType = OM_ROOT_FOLDER;

    }
    else if (OM_LOG_FOLDER == ulFolderType)
    {
        pstOmQueryDir->ulLength = VOS_StrLen((VOS_CHAR*)g_acLogDir);
        VOS_MemCpy(pstOmQueryDir->aucDirPath,
                      g_acLogDir, pstOmQueryDir->ulLength);
        g_stOmFileInfo.ulFileType = OM_LOG_FOLDER;
    }
    /*���������ļ����ڵ�·��*/
    else if (OM_DUMP_FOLDER == ulFolderType)
    {
        pstOmQueryDir->ulLength = VOS_StrLen((VOS_CHAR*)g_acDumpDir);
        VOS_MemCpy(pstOmQueryDir->aucDirPath,
                      g_acDumpDir, pstOmQueryDir->ulLength);
        g_stOmFileInfo.ulFileType = OM_DUMP_FOLDER;
    }
    else
    {
        VOS_MemFree(WUEPS_PID_OM, pstOmToAppMsg);
        PS_LOG1(WUEPS_PID_OM, 0, PS_PRINT_WARNING,
                  "OM_QueryDir:Unknown ulFolderType fail!", (VOS_INT32)ulFolderType);
        OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_FILE_RECEIVE_FOLDER_TYPE_ERR, usReturnPrimId);

        return VOS_ERR;
    }
    /*����ulTotalSize�ڷ����ڴ�ռ�ʱ��ʹ�õ������ֵ��
      ����ʵ�ʵĳ�����Ҫ��ȥ���ڵ��ֽ�*/
    ulTotalSize = ulTotalSize - (DRV_NAME_MAX - pstOmQueryDir->ulLength);
    pstOmQueryDir->ulResult = VOS_OK;
    pstOmToAppMsg->usLength = (VOS_UINT16)(ulTotalSize - VOS_OM_HEADER_LEN);
    OM_SendContent(pstAppToOmMsg->ucFuncType, pstOmToAppMsg, usReturnPrimId);
    VOS_MemFree(WUEPS_PID_OM, pstOmToAppMsg);
    return VOS_OK;
}
/*****************************************************************************
�� �� ��  : OM_GetDirInfo
��������  : ��ȡĿ¼�����е��ļ����ļ������ֵĳ��Ⱥ͸���
�������  : pstDir - ָ���ļ�ϵͳ��Ŀ¼��ָ��
�������  : pulTotalLen - �ļ����ļ������ֵĳ���
            pulTotalNum - �ļ����ļ��еĸ���
�� �� ֵ  : ��

�޸���ʷ      :
    1.��    ��   : 2008��9��9��
      ��    ��   : ����
      �޸�����   : �����˺���
*****************************************************************************/
VOS_VOID OM_GetDirInfo(DRV_DIR_STRU *pstDir,
                           VOS_UINT32 *pulTotalLen, VOS_UINT32 *pulTotalNum)
{
    DRV_DIRENT_STRU     *pstTmpDirent;
    VOS_UINT32           ulTotalLen = 0;
    VOS_UINT32           ulTotalNum = 0;

    do
    {
        pstTmpDirent = DRV_FILE_READDIR(pstDir);
        /*readdir�ķ���ֵΪ�գ�����Ŀ¼������*/
        if (VOS_NULL_PTR != pstTmpDirent)
        {
            ulTotalLen += VOS_StrLen(pstTmpDirent->d_name);
            ulTotalNum++;
        }
    }while (VOS_NULL_PTR != pstTmpDirent);
    *pulTotalLen = ulTotalLen;
    *pulTotalNum = ulTotalNum;
}
/*****************************************************************************
�� �� ��  : OM_GetItemInfo
��������  : ��ȡ�ļ����ļ��еĻ�����Ϣ
�������  : pstDir        - ָ��Ŀ¼�ṹ��ָ��
            pstAppToOmMsg - ָ�򹤾߲�������Ϣ����ָ��
            pstOmScanDir  - ָ���Ż�����Ϣ�ڴ��ָ��
�������  : ��
�� �� ֵ  : VOS_ERR - ʧ��
            VOS_OK  - �ɹ�

�޸���ʷ      :
    1.��    ��   : 2008��9��9��
      ��    ��   : ����
      �޸�����   : �����˺���
*****************************************************************************/
VOS_UINT32 OM_GetItemInfo(DRV_DIR_STRU *pstDir, APP_OM_MSG_EX_STRU *pstAppToOmMsg,
                                OM_APP_SCAN_DIR_STRU *pstOmScanDir)
{
    VOS_UINT8              *pucOmItemInfo;
    DRV_DIRENT_STRU        *pstTmpDirent;
    DRV_STAT_STRU           stStat;
    VOS_UINT8              *pucItemPath;
    VOS_UINT16              usDirLen;
    VOS_UINT32              ulTotalNum = 0;
    OM_ITEM_INFO_STRU       stOmItemInfo;

    usDirLen = (VOS_UINT16)VOS_StrLen((VOS_CHAR*)(pstAppToOmMsg->aucPara));
    /*����ռ���������ļ����ļ��е�·������1����Ϊ������Ҫ���б��*/
    pucItemPath = (VOS_UINT8*)VOS_MemAlloc(WUEPS_PID_OM, DYNAMIC_MEM_PT,
                                 usDirLen + DRV_NAME_MAX + 1);
    if (VOS_NULL_PTR == pucItemPath)
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_GetItemInfo:VOS_MemAlloc fail!");
        return OM_FILE_MEM_ALLOC_ERR;
    }
    /*��Ŀ¼·���������ļ�·����*/
    VOS_MemCpy(pucItemPath, pstAppToOmMsg->aucPara, usDirLen);
    /*����Ŀ¼·���ǲ���б�ܽ����������ļ������ʱ����Ҫ���б��*/
    pucItemPath[usDirLen] = '/';
    usDirLen++;

    pucOmItemInfo = (VOS_UINT8*)(pstOmScanDir->stOmItemInfo);
    do
    {
        /*��������pstDirָ��·���е������ļ����ļ���*/
        pstTmpDirent = DRV_FILE_READDIR(pstDir);
        /*readdir�ķ���ֵΪ�գ�����Ŀ¼������*/
        if (VOS_NULL_PTR != pstTmpDirent)
        {
            /*�õ��ļ����ļ������ĳ���*/
            stOmItemInfo.ulNameLen = (VOS_UINT16)VOS_StrLen(pstTmpDirent->d_name);
            /*�����ļ����ļ���·����Ҫ��'\0'��Ϊ������������Copyʱ�����ȼ�1*/
            VOS_MemCpy(pucItemPath + usDirLen,
                         pstTmpDirent->d_name, stOmItemInfo.ulNameLen + 1);
            /*ͨ��stat��ȡ�ļ����ļ��е���Ϣ*/
            if (DRV_ERROR == DRV_FILE_STAT((VOS_CHAR*)pucItemPath, &stStat))
            {
                stOmItemInfo.ulItemType = OM_ITEM_FILE;
                stOmItemInfo.ulSize    = 0;
                stOmItemInfo.st_atime  = 0;
                stOmItemInfo.st_mtime  = 0;
                stOmItemInfo.st_ctime  = 0;
            }
            else
            {
                /*Ŀ¼*/
                if (0 != (DRV_S_IFDIR&stStat.st_mode))
                {
                    stOmItemInfo.ulItemType = OM_ITEM_FOLDER;
                }
                /*�ļ�*/
                else
                {
                    stOmItemInfo.ulItemType = OM_ITEM_FILE;
                }
                stOmItemInfo.ulSize    = (stStat.st_size & 0xFFFFFFFF);/* Ŀǰ�ļ���С���ᳬ��32λ��С */
                stOmItemInfo.st_atime  = stStat.st_atime;
                stOmItemInfo.st_mtime  = stStat.st_mtime;
                stOmItemInfo.st_ctime  = stStat.st_ctime;
            }
            VOS_MemCpy(pucOmItemInfo, &stOmItemInfo, OM_ITEM_INFO_HEAD_LEN);
            pucOmItemInfo += OM_ITEM_INFO_HEAD_LEN;
            /*�ļ����ļ�����*/
            VOS_MemCpy(pucOmItemInfo, pstTmpDirent->d_name, stOmItemInfo.ulNameLen);
            pucOmItemInfo += stOmItemInfo.ulNameLen;
            /*������¼�ļ����ļ��е��ܸ���*/
            ulTotalNum++;
        }
    }while(VOS_NULL_PTR != pstTmpDirent);

    pstOmScanDir->ulResult = VOS_OK;
    pstOmScanDir->ulNumber = ulTotalNum;
    VOS_MemFree(WUEPS_PID_OM, pucItemPath);
    return VOS_OK;
}
/*****************************************************************************
�� �� ��  : OM_ScanDir
��������  : ��������Ŀ¼�����ظ����߲൱ǰĿ¼�������ļ����ļ��е���Ϣ��
            �����ļ����ƣ����Եȡ�
�������  : pstAppToOmMsg  - ָ�򹤾߲෢����������Ϣ��
            usReturnPrimId - ��Ӧ���ص�Prim ID.
�������  : ��
�� �� ֵ  : VOS_OK  - �����ɹ�
            VOS_ERR - ����ʧ��

�޸���ʷ      :
    1.��    ��   : 2008��9��9��
      ��    ��   : ����
      �޸�����   : �����˺���
*****************************************************************************/
VOS_UINT32 OM_ScanDir(APP_OM_MSG_EX_STRU *pstAppToOmMsg, VOS_UINT16 usReturnPrimId)
{
    OM_APP_MSG_EX_STRU     *pstOmToAppMsg;
    DRV_DIR_STRU           *pstTmpDir;
    VOS_UINT32              ulTotalSize;
    VOS_UINT32              ulTotalNameLen;
    VOS_UINT32              ulTotalNum;
    VOS_UINT32              ulRest;

    /*��Ŀ¼*/
    pstTmpDir = DRV_FILE_OPENDIR((VOS_CHAR*)(pstAppToOmMsg->aucPara));
    if (VOS_NULL_PTR == pstTmpDir)
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_ScanDir:opendir fail!");
        OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_DRV_FILE_OPENDIR_ERR, usReturnPrimId);
        return VOS_ERR;
    }
    /*�õ�Ŀ¼�ڣ��ļ����ļ��е��ܸ������ܵ����ֳ���*/
    OM_GetDirInfo(pstTmpDir, &ulTotalNameLen, &ulTotalNum);
    /*�ر�Ŀ¼*/
    if (DRV_ERROR == DRV_FILE_CLOSEDIR(pstTmpDir))
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_ScanDir:closedir fail!");
        OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_DRV_FILE_CLOSEDIR_ERR, usReturnPrimId);
        return VOS_ERR;
    }
    /*���㷵�ظ����߲���Ϣ���ĳ���*/
    ulTotalSize = OM_APP_MSG_EX_LEN + OM_APP_SCAN_DIR_HEAD_LEN
                  + (ulTotalNum*OM_ITEM_INFO_HEAD_LEN) + ulTotalNameLen;

    pstOmToAppMsg = (OM_APP_MSG_EX_STRU*)VOS_MemAlloc(WUEPS_PID_OM, DYNAMIC_MEM_PT, ulTotalSize);
    if (VOS_NULL_PTR == pstOmToAppMsg)
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_ScanDir:VOS_MemAlloc fail!");
        OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_FILE_MEM_ALLOC_ERR, usReturnPrimId);
        return VOS_ERR;
    }

    /*�ٴδ�Ŀ¼*/
    pstTmpDir = DRV_FILE_OPENDIR((VOS_CHAR*)(pstAppToOmMsg->aucPara));

    /*��ȡÿ���ļ����ļ��е���Ϣ*/
    ulRest = OM_GetItemInfo(pstTmpDir, pstAppToOmMsg,
                         (OM_APP_SCAN_DIR_STRU*)(pstOmToAppMsg->aucPara));
    if (VOS_OK != ulRest)
    {
        VOS_MemFree(WUEPS_PID_OM, pstOmToAppMsg);
        DRV_FILE_CLOSEDIR(pstTmpDir);
        OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, ulRest, usReturnPrimId);
        return VOS_ERR;
    }
    DRV_FILE_CLOSEDIR(pstTmpDir);
    pstOmToAppMsg->usLength = (VOS_UINT16)(ulTotalSize - VOS_OM_HEADER_LEN);
    OM_SendContent(pstAppToOmMsg->ucFuncType, pstOmToAppMsg, usReturnPrimId);
    VOS_MemFree(WUEPS_PID_OM, pstOmToAppMsg);
    return VOS_OK;
}
/*****************************************************************************
�� �� ��  : OM_CloseFile
��������  : �ر��ļ��������ȫ�ֱ���
�������  : ��
�������  : ��
�� �� ֵ  : ��

�޸���ʷ      :
    1.��    ��   : 2008��9��9��
      ��    ��   : ����
      �޸�����   : �����˺���
*****************************************************************************/
VOS_VOID OM_CloseFile(VOS_VOID)
{
    if (DRV_FILE_NULL != g_stOmFileInfo.lFile)
    {
        DRV_FILE_CLOSE(g_stOmFileInfo.lFile);
        g_stOmFileInfo.lFile = DRV_FILE_NULL;
    }
}

/*****************************************************************************
�� �� ��  : OM_OpenFile
��������  : ��ָ�����ļ�
�������  : pstAppToOmMsg  - ָ�򹤾߲෢����������Ϣ��
            usReturnPrimId - ��Ӧ���ص�Prim ID.
�������  : ��
�� �� ֵ  : VOS_OK  - �����ɹ�
            VOS_ERR - ����ʧ��

�޸���ʷ      :
    1.��    ��   : 2008��9��9��
      ��    ��   : ����
      �޸�����   : �����˺���
*****************************************************************************/
VOS_UINT32 OM_OpenFile(APP_OM_MSG_EX_STRU *pstAppToOmMsg, VOS_UINT16 usReturnPrimId)
{
    APP_OM_OPEN_INFO_STRU *pstOmOpenInfo;
    VOS_CHAR              *pcPath;
    VOS_UINT32             ulFlag;
    VOS_INT32              lFlags = 0;

    /*��֧�ֲ����ļ����������*/
    if (DRV_FILE_NULL != g_stOmFileInfo.lFile)
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING,
                               "OM_OpenFile:Last Operation hasn't finished.");
        OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_DRV_FILE_OPERATE_NOT_FINISH, usReturnPrimId);
        return VOS_ERR;
    }

    pstOmOpenInfo = (APP_OM_OPEN_INFO_STRU*)pstAppToOmMsg->aucPara;

    pcPath = (VOS_CHAR*)(pstOmOpenInfo->aucPara);

#if (FEATURE_ON == FEATURE_BROWSER_NV_FILE_IMEI_PROTECT)
    if (VOS_TRUE == NV_CheckNVFile(pcPath))
    {
        PS_LOG(WUEPS_PID_OM, 0, LOG_LEVEL_INFO,
                               "#info: OM_OpenFile: open is nv file.");
        /* IMEI ��֤ */
        if (LEVEL_ADVANCED != OM_QueryPrivilegeLevel())
        {
            PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING,
                               "OM_OpenFile:IMEI verify fail.");
            OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_FILE_OPEN_SC_ERR, usReturnPrimId);
            return VOS_ERR;
        }
    }
#endif

    /* Ӳ���ܰ汾�в��ܲ��������ļ� */
    if (VOS_YES == DRV_SEC_CHECK())
    {
        if (VOS_OK == VOS_StrCmp(OM_CFDNV_FILE_PATH, pcPath))
        {
            PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "OM_OpenFile:CFD NV File is secured.");
            OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_FILE_OPERATE_SEC_FILE_FORBID_CFD, usReturnPrimId);
            return VOS_ERR;
        }

        if (VOS_OK == VOS_StrCmp(OM_ZSP_FILE_PATH, pcPath))
        {
            PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "OM_OpenFile:ZSP File is secured.");
            OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_FILE_OPERATE_SEC_FILE_FORBID_ZSP, usReturnPrimId);
            return VOS_ERR;
        }
    }

    ulFlag = pstOmOpenInfo->ulFlag;
    /*�ж��Ƿ�Ϊ��д*/
    if (0 != ISWIRTE(ulFlag))
    {
        lFlags = DRV_O_WRONLY | DRV_O_TRUNC;
    }
    /*�ж��Ƿ�Ϊ�ɶ�*/
    if (0 != ISREAD(ulFlag))
    {
        if (0 != lFlags)
        {
            lFlags = DRV_O_RDWR;
        }
        else
        {
            lFlags = DRV_O_RDONLY;
        }
    }
    /*�ж��Ƿ�Ϊ�����ļ�*/
    if (0 != ISCREATE(ulFlag))
    {
        lFlags = lFlags | DRV_O_CREAT;
    }
    /*��ֻ���ķ�ʽ���ļ�*/
    g_stOmFileInfo.lFile = OM_Open(pcPath, lFlags);
    if (DRV_FILE_NULL == g_stOmFileInfo.lFile)
    {
        OM_CloseFile();
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "OM_OpenFile:open file fail.");
        OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_DRV_FILE_OPEN_ERR, usReturnPrimId);
        return VOS_ERR;
    }
    /*������ʱ��*/
    g_stOmFileInfo.hTimer = VOS_NULL_PTR;
    if (VOS_ERR == OM_START_FILETIMER(&g_stOmFileInfo.hTimer))
    {
        OM_CloseFile();
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "OM_OpenFile:Start Timer failed.");
        OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_FILE_START_FILE_TIMER_ERR, usReturnPrimId);
        return VOS_ERR;
    }

    OM_SendResult(pstAppToOmMsg->ucFuncType, VOS_OK, usReturnPrimId);
    return VOS_OK;
}
/*****************************************************************************
�� �� ��  : OM_ReadFile
��������  : ��ȡָ���ļ�������
�������  : pstAppToOmMsg  - ָ�򹤾߲෢����������Ϣ��
            usReturnPrimId - ��Ӧ���ص�Prim ID.
�������  : ��
�� �� ֵ  : VOS_OK  - �����ɹ�
            VOS_ERR - ����ʧ��

�޸���ʷ      :
    1.��    ��   : 2008��9��9��
      ��    ��   : ����
      �޸�����   : �����˺���
*****************************************************************************/
VOS_UINT32 OM_ReadFile(APP_OM_MSG_EX_STRU *pstAppToOmMsg, VOS_UINT16 usReturnPrimId)
{
    OM_APP_MSG_EX_STRU     *pstOmToAppMsg;
    OM_APP_READ_FILE_STRU  *pstOmReadFile;
    VOS_UINT32              ulTotalSize;
    VOS_UINT32              ulReadSize;

    if (DRV_FILE_NULL != g_stOmFileInfo.lFile)
    {
        /*ֹͣ��ʱ��*/
        OM_STOP_FILETIMER(&g_stOmFileInfo.hTimer);
    }
    else
    {
        return VOS_ERR;
    }
    /*���㷢�͸����߲���Ϣ�����ܳ���*/
    ulTotalSize = OM_APP_MSG_EX_LEN + sizeof(OM_APP_READ_FILE_STRU);

    pstOmToAppMsg = (OM_APP_MSG_EX_STRU*)VOS_MemAlloc(WUEPS_PID_OM, DYNAMIC_MEM_PT, ulTotalSize);
    if (VOS_NULL_PTR == pstOmToAppMsg)
    {
        OM_CloseFile();
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_ReadFile:VOS_MemAlloc fail!");
        OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_FILE_MEM_ALLOC_ERR, usReturnPrimId);
        return VOS_ERR;
    }
    pstOmReadFile = (OM_APP_READ_FILE_STRU*)(pstOmToAppMsg->aucPara);
    ulReadSize = (VOS_UINT32)DRV_FILE_READ((VOS_CHAR*)pstOmReadFile->acFileContent, sizeof(VOS_CHAR), OM_READ_MAX_LEN, g_stOmFileInfo.lFile);
    /*��ȡ�ļ����ִ���*/
    if (DRV_ERROR == (VOS_INT32)ulReadSize)
    {
        VOS_MemFree(WUEPS_PID_OM, pstOmToAppMsg);
        OM_CloseFile();
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_ReadFile:read fail!");
        OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_DRV_FILE_READ_ERR, usReturnPrimId);
        return VOS_ERR;
    }

    pstOmReadFile->ulResult = VOS_OK;
    pstOmReadFile->ulLength = ulReadSize;
    /*�����Ѿ�û�����ݿ��Զ�ȡ���ļ�����ȫ������*/
    if (0 == ulReadSize)
    {
        OM_CloseFile();
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_INFO, "OM_ReadFile:Completed!");
    }
    else
    {
        /*������ʱ��*/
        g_stOmFileInfo.hTimer = VOS_NULL_PTR;
        if (VOS_OK != OM_START_FILETIMER(&g_stOmFileInfo.hTimer))
        {
            VOS_MemFree(WUEPS_PID_OM, pstOmToAppMsg);
            OM_CloseFile();
            PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_ReadFile:OM_START_FILETIMER fail!");
            OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_FILE_START_FILE_TIMER_ERR, usReturnPrimId);
            return VOS_ERR;
        }
    }
    pstOmToAppMsg->usLength = (VOS_UINT16)(((ulTotalSize - VOS_OM_HEADER_LEN)
                               - OM_READ_MAX_LEN) + ulReadSize);
    OM_SendContent(pstAppToOmMsg->ucFuncType, pstOmToAppMsg, usReturnPrimId);
    VOS_MemFree(WUEPS_PID_OM, pstOmToAppMsg);
    return VOS_OK;
}
/*****************************************************************************
�� �� ��  : OM_WriteFile
��������  : д��ָ���ļ�������
�������  : pstAppToOmMsg  - ָ�򹤾߲෢����������Ϣ��
            usReturnPrimId - ��Ӧ���ص�Prim ID.
�������  : ��
�� �� ֵ  : VOS_OK  - �����ɹ�
            VOS_ERR - ����ʧ��

�޸���ʷ      :
    1.��    ��   : 2008��9��9��
      ��    ��   : ����
      �޸�����   : �����˺���
*****************************************************************************/
VOS_UINT32 OM_WriteFile(APP_OM_MSG_EX_STRU *pstAppToOmMsg, VOS_UINT16 usReturnPrimId)
{
    VOS_UINT32  ulWriteSize;
    VOS_INT32   lResult;
    VOS_UINT8  *pucContent;

    if (DRV_FILE_NULL != g_stOmFileInfo.lFile)
    {
        /*ֹͣ��ʱ��*/
        OM_STOP_FILETIMER(&g_stOmFileInfo.hTimer);
    }
    else
    {
        return VOS_ERR;
    }

    ulWriteSize = *((VOS_UINT32*)pstAppToOmMsg->aucPara);
    /*д�ļ���ϣ��ر��ļ�*/
    if (0 == ulWriteSize)
    {
        OM_CloseFile();
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_INFO, "OM_WriteFile:Completed!");
        return VOS_OK;
    }
    /*ָ����Ҫд���ļ����ݵ�ָ��*/
    pucContent = (VOS_UINT8*)pstAppToOmMsg->aucPara + sizeof(ulWriteSize);
    lResult = DRV_FILE_WRITE((VOS_CHAR*)pucContent, sizeof(VOS_CHAR), ulWriteSize, g_stOmFileInfo.lFile);
    /*д�ļ�����ʧ��*/
    if (DRV_ERROR == lResult)
    {
        OM_CloseFile();
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_WriteFile:DRV_FILE_WRITE fail!");
        OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_DRV_FILE_WRITE_ERR, usReturnPrimId);
        return VOS_ERR;
    }
    /*д�볤�Ȳ���ȷ*/
    if (lResult != ulWriteSize)
    {
        OM_CloseFile();
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR,
               "OM_WriteFile: the written size is not correct!");
        OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_DRV_FILE_WRITE_LEN_ERR, usReturnPrimId);
        return VOS_ERR;
    }
    /*������ʱ��*/
    if (VOS_OK != OM_START_FILETIMER(&g_stOmFileInfo.hTimer))
    {
        OM_CloseFile();
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_WriteFile:OM_START_FILETIMER fail!");
        OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_FILE_START_FILE_TIMER_ERR, usReturnPrimId);
        return VOS_ERR;
    }
    OM_SendResult(pstAppToOmMsg->ucFuncType, VOS_OK, usReturnPrimId);
    return VOS_OK;
}
/*****************************************************************************
�� �� ��  : OM_MakeDir
��������  : ����ָ��Ŀ¼
�������  : pstAppToOmMsg  - ָ�򹤾߲෢����������Ϣ��
            usReturnPrimId - ��Ӧ���ص�Prim ID.
�������  : ��
�� �� ֵ  : VOS_OK  - �����ɹ�
            VOS_ERR - ����ʧ��

�޸���ʷ      :
    1.��    ��   : 2008��9��9��
      ��    ��   : ����
      �޸�����   : �����˺���
*****************************************************************************/
VOS_UINT32 OM_MakeDir(APP_OM_MSG_EX_STRU *pstAppToOmMsg, VOS_UINT16 usReturnPrimId)
{
    if (DRV_ERROR == DRV_FILE_MKDIR((VOS_CHAR*)(pstAppToOmMsg->aucPara)))
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_MakeDir: DRV_FILE_MKDIR failed!");
        OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_DRV_FILE_MKDIR_ERR, usReturnPrimId);

        return VOS_ERR;
    }
    OM_SendResult(pstAppToOmMsg->ucFuncType, VOS_OK, usReturnPrimId);
    return VOS_OK;
}
/*****************************************************************************
�� �� ��  : OM_DeleteFile
��������  : ����ָ��Ŀ¼
�������  : pstAppToOmMsg  - ָ�򹤾߲෢����������Ϣ��
            usReturnPrimId - ��Ӧ���ص�Prim ID.
�������  : ��
�� �� ֵ  : VOS_OK  - �����ɹ�
            VOS_ERR - ����ʧ��

�޸���ʷ      :
    1.��    ��   : 2008��9��9��
      ��    ��   : ����
      �޸�����   : �����˺���
*****************************************************************************/
VOS_UINT32 OM_DeleteFile(APP_OM_MSG_EX_STRU *pstAppToOmMsg, VOS_UINT16 usReturnPrimId)
{
    VOS_UINT32  ulItemType;
    VOS_CHAR   *pcPath;
    /*�õ���ǰ���������ļ������ļ���*/
    ulItemType = *((VOS_UINT32*)pstAppToOmMsg->aucPara);
    /*���·��*/
    pcPath = (VOS_CHAR*)pstAppToOmMsg->aucPara + sizeof(ulItemType);
    if (OM_ITEM_FOLDER == ulItemType)
    {
        if (DRV_ERROR == DRV_FILE_RMDIR(pcPath))
        {
            PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_DeleteFile: DRV_FILE_RMDIR failed!");
            OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_DRV_FILE_RMDIR_ERR, usReturnPrimId);
            return VOS_ERR;
        }
        OM_SendResult(pstAppToOmMsg->ucFuncType, VOS_OK, usReturnPrimId);
        return VOS_OK;
    }
    else if (OM_ITEM_FILE == ulItemType)
    {
        /* Ӳ���ܰ汾�в��ܲ��������ļ� */
        if (VOS_YES == DRV_SEC_CHECK())
        {
            if (VOS_OK == VOS_StrCmp(OM_CFDNV_FILE_PATH, pcPath))
            {
                PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "OM_OpenFile:CFD NV File is secured.");
                OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_FILE_OPERATE_SEC_FILE_FORBID_CFD, usReturnPrimId);
                return VOS_ERR;
            }

            if (VOS_OK == VOS_StrCmp(OM_ZSP_FILE_PATH, pcPath))
            {
                PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "OM_OpenFile:ZSP File is secured.");
                OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_FILE_OPERATE_SEC_FILE_FORBID_ZSP, usReturnPrimId);
                return VOS_ERR;
            }
        }

        if (DRV_ERROR == DRV_FILE_RMFILE(pcPath))
        {
            PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_DeleteFile: DRV_FILE_RMFILE failed!");
            OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_DRV_FILE_RMFILE_ERR, usReturnPrimId);
            return VOS_ERR;
        }
        OM_SendResult(pstAppToOmMsg->ucFuncType, VOS_OK, usReturnPrimId);
        return VOS_OK;
    }
    else
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_DeleteFile: Unknown type!");
        OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_FILE_RECEIVE_ITEM_TYPE_ERR, usReturnPrimId);
    }
    return VOS_ERR;
}
/*****************************************************************************
�� �� ��  : OM_QuerySpace
��������  : ��ѯ�ļ�ϵͳ���ڴ�ռ�ʹ�����
�������  : pstAppToOmMsg  - ָ�򹤾߲෢����������Ϣ��
            usReturnPrimId - ��Ӧ���ص�Prim ID.
�������  : ��
�� �� ֵ  : VOS_OK  - �����ɹ�
            VOS_ERR - ����ʧ��

�޸���ʷ      :
    1.��    ��   : 2009��5��11��
      ��    ��   : ����
      �޸�����   : �����˺���
*****************************************************************************/
VOS_UINT32 OM_QuerySpace(APP_OM_MSG_EX_STRU *pstAppToOmMsg, VOS_UINT16 usReturnPrimId)
{
    OM_APP_QUERY_DISKSPACE_STRU     stOmDiskSpace;

    if (DRV_ERROR == DRV_FILE_GET_DISKSPACE(
                    (VOS_CHAR*)(pstAppToOmMsg->aucPara),
                    (VOS_UINT*)&stOmDiskSpace.ulDiskSpace,
                    (VOS_UINT*)&stOmDiskSpace.ulUsedSpace,
                    (VOS_UINT*)&stOmDiskSpace.ulValidSpace))
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_QuerySpace: DRV_FILE_GET_DISKSPACE");
        OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_DRV_FILE_GET_DISKSPACE_ERR, usReturnPrimId);

        return VOS_ERR;
    }

    stOmDiskSpace.ulResult = VOS_OK;
    stOmDiskSpace.usLength = (VOS_UINT16)(sizeof(OM_APP_QUERY_DISKSPACE_STRU)
                            - VOS_OM_HEADER_LEN);

    OM_SendContent(pstAppToOmMsg->ucFuncType, (OM_APP_MSG_EX_STRU*)&stOmDiskSpace, usReturnPrimId);
    return VOS_OK;
}

/* ���湤�߲෢����������Ϣ�Ͷ�Ӧ�������Ĺ�ϵ*/
static OM_MSG_FUN_STRU g_astOmFSMsgTbl[] =
{
    {APP_OM_QUERY_DIR_REQ,      OM_QueryDir,     OM_APP_QUERY_DIR_IND},
    {APP_OM_SCAN_DIR_REQ,       OM_ScanDir,      OM_APP_SCAN_DIR_IND},
    {APP_OM_OPEN_FILE_REQ,      OM_OpenFile,     OM_APP_OPEN_FILE_CNF},
    {APP_OM_READ_FILE_REQ,      OM_ReadFile,     OM_APP_READ_FILE_IND},
    {APP_OM_WRITE_FILE_REQ,     OM_WriteFile,    OM_APP_WRITE_FILE_CNF},
    {APP_OM_MAKE_DIR_REQ,       OM_MakeDir,      OM_APP_MAKE_DIR_CNF},
    {APP_OM_DELETE_FILE_REQ,    OM_DeleteFile,   OM_APP_DELETE_FILE_CNF},
    {APP_OM_QUERY_SPACE_REQ,    OM_QuerySpace,   OM_APP_QUERY_SPACE_CNF}
};

/*****************************************************************************
�� �� ��  : OM_FileSystemMsgProc
��������  : ����OM���ļ�ϵͳ�Ĳ�����Ϣ
�������  : pstAppToOmMsg - ָ�򹤾߲෢������Ϣ��ָ��
�������  : ��
�� �� ֵ  : VOS_OK

�޸���ʷ      :
    1.��    ��   : 2008��9��8��
      ��    ��   : ����
      �޸�����   : �����˺���
*****************************************************************************/
VOS_VOID OM_FileSystemMsgProc(OM_REQ_PACKET_STRU *pRspPacket, OM_RSP_FUNC *pRspFuncPtr)
{
    APP_OM_MSG_EX_STRU      *pstAppToOmMsg;
    VOS_UINT32               ulIndex;
    VOS_UINT32               ulTotalNum;
    VOS_UINT16               usPrimId;
    VOS_UINT16               usReturnPrimId;
    VOS_UINT32               ulResult = VOS_ERR;

    pstAppToOmMsg = (APP_OM_MSG_EX_STRU*)pRspPacket;
    ulTotalNum = sizeof(g_astOmFSMsgTbl)/sizeof(OM_MSG_FUN_STRU);
    usPrimId = pstAppToOmMsg->usPrimId;
    /*Search the corresponding function and return PrimId*/
    for (ulIndex = 0; ulIndex < ulTotalNum; ulIndex++)
    {
        if (usPrimId == (VOS_UINT16)(g_astOmFSMsgTbl[ulIndex].ulPrimId))
        {
            usReturnPrimId = (VOS_UINT16)(g_astOmFSMsgTbl[ulIndex].ulReturnPrimId);
            ulResult = g_astOmFSMsgTbl[ulIndex].pfFun(pstAppToOmMsg, usReturnPrimId);
            if (VOS_ERR == ulResult)
            {
                g_stOmFileInfo.ulFileType = OM_FILE_BUTT;
            }
            break;
        }
    }
    /*Can't find the function handles the usPrimId.*/
    if (ulIndex == ulTotalNum)
    {
        PS_LOG1(WUEPS_PID_OM, 0, PS_PRINT_INFO,
          "OM_FileSystemMsgProc: PrimId can't be handled", (VOS_INT32)usPrimId);
        OM_FILE_SEND_RESULT(pstAppToOmMsg->ucFuncType, OM_FILE_RECEIVE_REQ_FIND_FUNC_ERR, usPrimId);
    }
    return;
}

/*****************************************************************************
�� �� ��  : OM_Open
��������  : ����Open��ʽ���ļ���ͨ�������򿪷�ʽ�����ö�Ӧ��fopen��ģʽ
�������  : pcFileName  - �ļ�·��
            lFlag       - �ļ��򿪷�ʽ.
�������  : ��
�� �� ֵ  : ���ļ��ľ��

�޸���ʷ      :
    1.��    ��   : 2012��4��11��
      ��    ��   : ף�
      �޸�����   : �����˺���
*****************************************************************************/
FILE* OM_Open(const VOS_CHAR *pcFileName, VOS_INT lFlag)
{
    VOS_CHAR    *pucMode;
    FILE        *fp;

    switch(lFlag)
    {
        case (DRV_O_CREAT|DRV_O_APPEND|DRV_O_RDWR):
            pucMode = "ab+";
            break;

        case (DRV_O_CREAT|DRV_O_APPEND|DRV_O_WRONLY):
            pucMode = "ab";
            break;

        case (DRV_O_CREAT|DRV_O_TRUNC|DRV_O_RDWR):
            pucMode = "wb+";
            break;

        case (DRV_O_CREAT|DRV_O_TRUNC|DRV_O_WRONLY):
            pucMode = "wb";
            break;

        case DRV_O_RDWR:
            pucMode = "rb+";
            break;

        case DRV_O_RDONLY:
            pucMode = "rb";
            break;

        default:
            LogPrint1("\r\nOM_Open: The File Mode 0x%x is Error", lFlag);
            return DRV_FILE_NULL;
    }

    fp = DRV_FILE_OPEN(pcFileName, pucMode);

    return fp;
}


/*****************************************************************************
�� �� ��  : OM_FileSystemInit
��������  : �ļ�ϵͳ��ر�����ʼ��
�������  : ��
�������  : ��
�� �� ֵ  : VOS_ERR/VOS_OK

�޸���ʷ      :
    1.��    ��   : 2014��3��11��
      ��    ��   : jinni
      �޸�����   : �����˺���
*****************************************************************************/
VOS_UINT32 OM_FileSystemInit(VOS_VOID)
{
    /* ��ʼ���ļ�ϵͳ·����Ϣ */
    if (VOS_OK != OM_GetLogPath(g_acDumpDir, OM_FILE_SYSTEM_DUMP_DIR, OM_FILE_SYSTEM_UNITARY_DUMP_DIR))
    {
        return VOS_ERR;
    }

    return VOS_OK;
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

