

#ifdef  __cplusplus
#if  __cplusplus
extern "C"{
#endif
#endif

/*lint -e767 �޸���:���� 47350;ԭ��:Log��ӡ */
#define THIS_FILE_ID PS_FILE_ID_LITTLE_IMAGE_LOG_C
/*lint +e767 �޸���:���� 47350;*/

#include "product_config.h"

#if (FEATURE_ON == FEATURE_MANUFACTURE_LOG)

/********************ͷ�ļ�****************************************************/
#include "vos.h"
#include "pslogadapter.h"
#include "FileSysInterface.h"
#include "omprivate.h"
#include "pslog.h"
#include "DrvInterface.h"
#include "NVIM_Interface.h"
#include "OmLittleImageLog.h"

/******************ȫ�ֱ�������************************************************/



/* LOG�ļ����������Ϣ */
LOG_FILE_SAVE_INFO_STRU g_stLogFileSaveInfo;

/* LOGĿ¼ */
#if (VOS_WIN32==VOS_OS_VER)
VOS_CHAR g_acFileSystemRoot[] = "";
VOS_CHAR g_acLogFileSaveDir[LOG_FILE_PATH_MAX_LENGTH] = "/Log";
VOS_CHAR g_acLogFileName[] = "/Log/LOG";
#else
VOS_CHAR g_acFileSystemRoot[] = MANUFACTURE_ROOT_PATH;
VOS_CHAR g_acLogFileSaveDir[LOG_FILE_PATH_MAX_LENGTH] = "/manufacture/Log";
VOS_CHAR g_acLogFileName[] = "/manufacture/Log/LOG";
#endif

/* LOG�ļ���� */
FILE  *g_lFileHandle = VOS_NULL_PTR;

VOS_UINT32  g_ulLittleImgLogErr = LOG_OPERATION_OK;

VOS_UINT32 g_ulLogFileSaveFlg = LOG_FILE_DISABLE;

extern FILE* OM_Open(const VOS_CHAR *pcFileName, VOS_INT lFlag);
VOS_UINT32 OM_LittleImgFilterMsg(VOS_UINT8 *pucMsg)
{
    OM_APP_TRACE_STRU       *pstAppTrace;
    VOS_UINT8               ucFuncType;

    if (VOS_NULL_PTR == pucMsg)
    {
        return VOS_ERR;
    }

    pstAppTrace = (OM_APP_TRACE_STRU*)pucMsg;

    ucFuncType = pstAppTrace->stAppHeader.ucFuncType;

    /* ������BTУ׼��Gģ����Ҫ�ϱ���Ϣ��ATE */
    if ((ucFuncType == OM_TRANS_FUNC) &&(pstAppTrace->usPrimId == OM_APP_TRACE_IND))
    {
        return VOS_OK;
    }

    /* ������ϢFUNCTYPE */
    if ((ucFuncType == OM_WRF_FUNC) || (ucFuncType == OM_GRF_FUNC) ||
        (ucFuncType == OM_QUERY_FUNC) || (ucFuncType == OM_NV_FUNC) ||
        (ucFuncType == OM_NOSIG_FUNC) || (ucFuncType == OM_AUTH_NV_FUNC) ||
        (ucFuncType == OM_FS_FUNC))
    {
        return VOS_OK;
    }
#if (VOS_WIN32==VOS_OS_VER)
    return VOS_OK;
#endif
    return VOS_ERR;
}


VOS_UINT32 OM_LittleImgCloseLogFile()
{
    if (VOS_NULL_PTR != g_lFileHandle)
    {
        DRV_FILE_CLOSE(g_lFileHandle);

        g_lFileHandle = VOS_NULL_PTR;

        return VOS_OK;
    }

    return VOS_ERR;
}


VOS_VOID OM_LittleImgRemoveLogFile(VOS_UINT32 ulFileId)
{
    VOS_CHAR acFileName[LOG_FILE_PATH_MAX_LENGTH] = {0};
	LOG_FILE_DISK_INFO_STRU stDiskInfo;

    /* ƴ��LOG�ļ����� */
    VOS_sprintf((VOS_CHAR *)acFileName, "%s_%04d",
            g_acLogFileName,
            ulFileId);

    DRV_FILE_RMFILE(acFileName);

    /* ����ȫ����Ϣ */
    g_stLogFileSaveInfo.ulFileMinId++;

    /* ���õ���ӿڷ��� */
    if (DRV_ERROR == DRV_FILE_GET_DISKSPACE(
                    g_acFileSystemRoot,
                    (VOS_UINT*)&stDiskInfo.ulDiskSpace,
                    (VOS_UINT*)&stDiskInfo.ulUsedSpace,
                    (VOS_UINT*)&stDiskInfo.ulValidSpace))
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_LittleImgRemoveLogFile: DRV_FILE_GET_DISKSPACE Failed!");
        return;
    }

    g_stLogFileSaveInfo.ulFSFreeSpace = stDiskInfo.ulValidSpace;

    return;
}
VOS_UINT32 OM_LittleImgCreateLogFile(VOS_UINT32 ulFileSize)
{
    VOS_CHAR acFileName[LOG_FILE_PATH_MAX_LENGTH] = {0};
    LOG_FILE_DISK_INFO_STRU stDiskInfo;

    /* ���õ���ӿڷ��� */
    if (DRV_ERROR == DRV_FILE_GET_DISKSPACE(
                    g_acFileSystemRoot,
                    (VOS_UINT*)&stDiskInfo.ulDiskSpace,
                    (VOS_UINT*)&stDiskInfo.ulUsedSpace,
                    (VOS_UINT*)&stDiskInfo.ulValidSpace))
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_LittleImgRemoveLogFile: DRV_FILE_GET_DISKSPACE Failed!");
        return VOS_ERR;
    }

    g_stLogFileSaveInfo.ulFSFreeSpace = stDiskInfo.ulValidSpace;

    /*�ļ�ϵͳ�����޷�����LOG�ļ�*/
    while ((ulFileSize >= g_stLogFileSaveInfo.ulFSFreeSpace) &&
            (g_stLogFileSaveInfo.ulFileMinId != g_stLogFileSaveInfo.ulFileMaxId))
    {
        /*ɾ�����ϵ�LOG�ļ�*/
        OM_LittleImgRemoveLogFile(g_stLogFileSaveInfo.ulFileMinId);
    }

    /* ������ID�����ļ����� */
    VOS_sprintf((VOS_CHAR *)acFileName, "%s_%04d",
            g_acLogFileName,
            g_stLogFileSaveInfo.ulFileMaxId);

    g_lFileHandle = OM_Open((VOS_CHAR *)acFileName, DRV_O_CREAT|DRV_O_TRUNC|DRV_O_RDWR);

    if (DRV_FILE_NULL == g_lFileHandle)
    {
        g_ulLittleImgLogErr = LOG_OPERATION_OPEN_FILE_FAIL;

        return VOS_ERR;
    }

    /* ����ȫ�ֱ�����Ϣ */
    g_stLogFileSaveInfo.ulFileMaxId++;
    g_stLogFileSaveInfo.ulCurLogFileSize = 0;

    return VOS_OK;
}
VOS_UINT32 OM_LittleImgWriteLogFile(VOS_CHAR *pcLogData, VOS_UINT32 ulLength)
{
    VOS_UINT32 ulResult;

    /* ������Ϣ */
    ulResult = OM_LittleImgFilterMsg((VOS_UINT8*)pcLogData);

    /* �ж��Ƿ�ʹ�� */
    if (LOG_FILE_DISABLE == g_ulLogFileSaveFlg)
    {
        /*LogPrint("\r\n OM_LittleImgWriteLogFile: LOG_FILE_SAVE is Disable. \r\n"); */
        return ulResult;
    }

        /* ���ٽ���Դg_astLogFileInfo���б��� */
    if (VOS_OK != VOS_SmP(g_stLogFileSaveInfo.ulSem, LOG_SEM_TIMEOUT_VALUE))
    {
        g_ulLittleImgLogErr = LOG_OPERATION_POST_SEM_FAIL;
        return ulResult;
    }

    if (VOS_NULL_PTR == g_lFileHandle)
    {
        /*LogPrint("\r\n OM_LittleImgWriteLogFile: The file handle is null. \r\n"); */
        VOS_SmV(g_stLogFileSaveInfo.ulSem);
        return ulResult;
    }

    if (VOS_NULL_PTR == pcLogData)
    {
        VOS_SmV(g_stLogFileSaveInfo.ulSem);
        return ulResult;
    }

    /* д�볤�ȴ���LOG�ļ������ֵ */
    if (ulLength > (g_stLogFileSaveInfo.ulCurLogFileMaxSize*FILE_SIZE_THRESHOLD_PERCENT))
    {
        LogPrint("\r\n OM_LittleImgWriteLogFile: The file size is too large. \r\n");
        VOS_SmV(g_stLogFileSaveInfo.ulSem);
        return ulResult;
    }

    /* LOG�ļ�д���� */
    if ((ulLength + g_stLogFileSaveInfo.ulCurLogFileSize) >
                (g_stLogFileSaveInfo.ulCurLogFileMaxSize * FILE_SIZE_THRESHOLD_PERCENT))
    {
        /* �رյ�ǰLOG�ļ���� */
        OM_LittleImgCloseLogFile();

        /* ��������ļ�ʧ�ܣ�ֹͣLOG�ļ����湦�� */
        if (VOS_OK != OM_LittleImgCreateLogFile(g_stLogFileSaveInfo.ulCurLogFileMaxSize))
        {
            g_ulLogFileSaveFlg = LOG_FILE_DISABLE;

            g_ulLittleImgLogErr = LOG_OPERATION_OPEN_FILE_FAIL;

            VOS_SmV(g_stLogFileSaveInfo.ulSem);

            return ulResult;
        }
    }

    /* дLOG�ļ�ʧ�� */
    if ( DRV_ERROR == DRV_FILE_WRITE(pcLogData, sizeof(VOS_CHAR), (DRV_SIZE_T)ulLength, g_lFileHandle))
    {
        /* �رյ�ǰLOG�ļ� */
        OM_LittleImgCloseLogFile();

        g_ulLogFileSaveFlg = LOG_FILE_DISABLE;

        g_ulLittleImgLogErr = LOG_OPERATION_WRITE_FILE_FAIL;

        VOS_SmV(g_stLogFileSaveInfo.ulSem);

        return ulResult;
    }

    /* ���µ�ǰLOG�ļ��Ĵ�С */
    g_stLogFileSaveInfo.ulCurLogFileSize += ulLength;

    VOS_SmV(g_stLogFileSaveInfo.ulSem);

    return ulResult;

}


VOS_VOID OM_LittleImgClearAllLogFile()
{
    VOS_UINT32 ulFileIndex;

    /* �رյ�ǰ�����LOG�ļ� */
    if (VOS_OK != OM_LittleImgCloseLogFile())
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "OM_LittleImgClearAllLogFile: File handle is null.\n");
        /*return; */
    }

    for (ulFileIndex = g_stLogFileSaveInfo.ulFileMinId; ulFileIndex < g_stLogFileSaveInfo.ulFileMaxId; ulFileIndex++)
    {
        OM_LittleImgRemoveLogFile(ulFileIndex);
    }

    /*VOS_MemSet(&g_stLogFileSaveInfo, 0, sizeof(g_stLogFileSaveInfo)); */
}
VOS_UINT32 OM_LittleImgInitLogFile(VOS_UINT32 ulFileSize)
{
    LOG_FILE_DISK_INFO_STRU stDiskInfo;

    /* ����LOG�ļ���Ӧ���ź��� */
    if (VOS_OK != VOS_SmMCreate("FILE", VOS_SEMA4_FIFO, &g_stLogFileSaveInfo.ulSem))
    {
        g_ulLittleImgLogErr = LOG_OPERATION_CREATE_SEM_FAIL;

        return VOS_ERR;
    }

    if (VOS_OK != VOS_SmP(g_stLogFileSaveInfo.ulSem, LOG_SEM_TIMEOUT_VALUE))
    {
        g_ulLittleImgLogErr = LOG_OPERATION_POST_SEM_FAIL;

        return VOS_ERR;
    }

    /* �ж�LOGĿ¼�Ƿ��Ѿ����� */
    if (VOS_OK != DRV_FILE_ACCESS(g_acLogFileSaveDir, 0))
    {
        /* ���Ŀ¼�������򴴽�LOGĿ¼ */
        if(DRV_ERROR == DRV_FILE_MKDIR(g_acLogFileSaveDir))
        {
            g_ulLittleImgLogErr = LOG_OPERATION_MKDIR_FAIL;
            VOS_SmV(g_stLogFileSaveInfo.ulSem);
            return VOS_ERR;
        }
    }

    g_stLogFileSaveInfo.ulFileMinId = 1;
    g_stLogFileSaveInfo.ulFileMaxId = 1;
    g_stLogFileSaveInfo.ulCurLogFileMaxSize = ulFileSize * LOG_FILE_UNIT_SIZE;

    /* ���õ���ӿڷ��� */
    if (DRV_ERROR == DRV_FILE_GET_DISKSPACE(
                    g_acFileSystemRoot,
                    (VOS_UINT*)&stDiskInfo.ulDiskSpace,
                    (VOS_UINT*)&stDiskInfo.ulUsedSpace,
                    (VOS_UINT*)&stDiskInfo.ulValidSpace))
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_LittleImgInitLogFile: DRV_FILE_GET_DISKSPACE Failed!");
        return VOS_ERR;
    }

    g_stLogFileSaveInfo.ulFSFreeSpace = stDiskInfo.ulValidSpace;

    /* ���NV�����ļ���С���Ϸ����޸�ΪĬ��ֵ */
    if (g_stLogFileSaveInfo.ulCurLogFileMaxSize > g_stLogFileSaveInfo.ulFSFreeSpace)
    {
        g_stLogFileSaveInfo.ulCurLogFileMaxSize = LOG_FILE_SIZE * LOG_FILE_UNIT_SIZE;
    }

    /* ���Ĭ��ֵ�����ļ�ϵͳ��С������ʧ�� */
    if (g_stLogFileSaveInfo.ulCurLogFileMaxSize > g_stLogFileSaveInfo.ulFSFreeSpace)
    {
        g_ulLittleImgLogErr = LOG_OPERATION_LENGTH_TOOBIG;
        VOS_SmV(g_stLogFileSaveInfo.ulSem);
        return VOS_ERR;
    }

    if (VOS_OK != OM_LittleImgCreateLogFile(g_stLogFileSaveInfo.ulCurLogFileMaxSize))
    {
        LogPrint("\r\n OM_LittleImgInitLogFile: Create Log File Failed.! \r\n");
        VOS_SmV(g_stLogFileSaveInfo.ulSem);
        return VOS_ERR;
    }

    g_ulLogFileSaveFlg = LOG_FILE_ENABLE;

    VOS_SmV(g_stLogFileSaveInfo.ulSem);

    return VOS_OK;

}


VOS_UINT32 OM_StartUpLogSaveReq(APP_OM_MSG_EX_STRU *pstAppToOmMsg,
                                            VOS_UINT16 usReturnPrimId)
{
    VOS_UINT32                ulResult;
    LOG_FILE_SAVE_CONFIG_STRU stLogFileSaveCfg;

    /* ��ȡNV�����Ĭ���ļ���С�Լ�У׼��� */
    ulResult = NV_Read(en_NV_Item_LOG_FILE_SAVE, &stLogFileSaveCfg, sizeof(LOG_FILE_SAVE_CONFIG_STRU));

    if (NV_OK != ulResult)
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "OM_StartUpLogSaveReq: NV_Read failed.\n");

        /* ���ؽ��ATE */
        OM_SendResult(pstAppToOmMsg->ucFuncType, ulResult, usReturnPrimId);

        return VOS_OK;
    }

    if ( VOS_OK != stLogFileSaveCfg.ulResult )
    {
        /* ���ؽ��ATE */
        OM_SendResult(pstAppToOmMsg->ucFuncType, stLogFileSaveCfg.ulResult, usReturnPrimId);

        return VOS_OK;
    }

    /*��ʼ��LOG�ļ�����*/
    ulResult = OM_LittleImgInitLogFile(stLogFileSaveCfg.ulLogFileSize);

    /* ���ؽ��ATE */
    OM_SendResult(pstAppToOmMsg->ucFuncType, ulResult, usReturnPrimId);

    return VOS_OK;
}
VOS_UINT32 OM_StopLogSaveReq(APP_OM_MSG_EX_STRU *pstAppToOmMsg,
                                            VOS_UINT16 usReturnPrimId)
{
    VOS_UINT32 ulResult = VOS_ERR;

    /* ��ȡУ׼��� */
    ulResult = *(VOS_UINT32*)(pstAppToOmMsg->aucPara);

    if ( LOG_FILE_DISABLE == g_ulLogFileSaveFlg )
    {
        OM_SendResult(pstAppToOmMsg->ucFuncType, ulResult, usReturnPrimId);

        return VOS_OK;
    }

    /* ���ٽ���Դg_stLogFileInfo���б��� */
    if (VOS_OK != VOS_SmP(g_stLogFileSaveInfo.ulSem, LOG_SEM_TIMEOUT_VALUE))
    {
        g_ulLittleImgLogErr = LOG_OPERATION_POST_SEM_FAIL;

        OM_SendResult(pstAppToOmMsg->ucFuncType, ulResult, usReturnPrimId);

        return VOS_ERR;
    }

    /* ����ʹ�ܱ�� */
    g_ulLogFileSaveFlg = LOG_FILE_DISABLE;

    if (VOS_OK == ulResult)
    {
        /*���LOG�ļ�*/
        OM_LittleImgClearAllLogFile();
    }
    else
    {
        OM_LittleImgCloseLogFile();
    }

    VOS_SmV(g_stLogFileSaveInfo.ulSem);

    /* ��������浽NV���� */
    ulResult = NV_WritePart(en_NV_Item_LOG_FILE_SAVE, 0, (VOS_UINT32*)(pstAppToOmMsg->aucPara), sizeof(VOS_UINT32));

    OM_SendResult(pstAppToOmMsg->ucFuncType, ulResult, usReturnPrimId);

    /* �ͷ��ź�����Դ */
    if (VOS_OK != VOS_SmDelete(g_stLogFileSaveInfo.ulSem))
    {
        g_ulLittleImgLogErr = LOG_OPERATION_DELETE_SEM_FAIL;

        return VOS_ERR;
    }

    return VOS_OK;
}

#endif  /*(FEATURE_ON == FEATURE_MANUFACTURE_LOG)*/

#ifdef  __cplusplus
#if  __cplusplus
}
#endif
#endif


