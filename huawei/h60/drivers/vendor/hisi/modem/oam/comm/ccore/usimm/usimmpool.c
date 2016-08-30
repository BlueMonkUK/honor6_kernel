
/*lint  --e{537,451,767,718,746,740,958}*/

#include "usimmbase.h"
#include "usimmapdu.h"
#include "usimmdl.h"
#include "pslog.h"
#include "NVIM_Interface.h"
#include "FileSysInterface.h"
#include "omnvinterface.h"
#include "usimmt1dl.h"
#include "om.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/

#define    THIS_FILE_ID        PS_FILE_ID_USIMM_POOL_C


#define NV_FILE_OPEN_MODE_RW            "rb+"   /* open binary file for reading and writing */
#define NV_FILE_OPEN_MODE_NEW_RW        "wb+"   /* creat binary file for reading and writing*/

#define NV_FILE_SEEK_SET                0       /* the beginning of the file*/


VOS_VOID USIMM_PoolDataInit(VOS_VOID)
{
    gstUSIMMPOOL.enPoolStatus = USIMM_POOL_INITIALED;   /*����ȫ�ֱ�������*/

    gstUSIMMPOOL.ucNowLen     = 0x00;                  /*�����Ѵ����ݳ���*/

    VOS_MemSet(gstUSIMMPOOL.astpoolRecord, 0, sizeof(gstUSIMMPOOL.astpoolRecord));/*��յ�Ԫ����*/
}
VOS_UINT32 USIMM_PoolFindFile(
    VOS_UINT16                          usFileId,
    VOS_UINT32                         *pulData,
    USIMM_APP_TYPE_ENUM_UINT32          enAppType)
{
    VOS_UINT32 i;

    if(0 == gstUSIMMPOOL.ucNowLen)  /*��ǰ����Ϊ��*/
    {
        if(0 == usFileId)       /*��ѯ��λ��*/
        {
            *pulData = 0;

            return VOS_OK;
        }

        return VOS_ERR;
    }

    /*��ҪУ��PIN���PUK��ʱ��ֻ������ļ����ж�ȡ����PIN�뱣�����ļ�*/
    if ((gstUSIMMPOOL.enPoolStatus == USIMM_POOL_NEED_PASSWORD)&&(0 != usFileId))
    {
        if ((EFPL != usFileId)&&(EFICCID != usFileId)&&(EFLP != usFileId))
        {
            return VOS_ERR;
        }
    }

    for(i=0; i<POOLMAXNUM; i++)
    {
        /* �ļ���Ҫƥ���ļ�ID��APP���Ͳ����ҵ����ļ�������ATT��UMTS��GSM
        ���ߴ����APP����ΪUNLIMIT APP����ƥ��APP����ֻƥ���ļ�ID
        ����ļ�IDΪ�գ���Ϊ����һ�����еĵط� */
        if (gstUSIMMPOOL.astpoolRecord[i].usFId == usFileId)
        {
            if ((gstUSIMMPOOL.astpoolRecord[i].enAppType == enAppType)
                || (USIMM_UNLIMIT_APP == enAppType)
                || (0 == usFileId))
            {
                /*�����ļ����ڵ�Ԫ��*/
                *pulData = i;

                return VOS_OK;
            }
        }
    }

    return VOS_ERR;
}
VOS_UINT32 USIMM_PoolInsertOneFile(
    VOS_UINT16                          usFileId,
    VOS_UINT16                          usLen,
    VOS_UINT8                          *pData,
    USIMM_APP_TYPE_ENUM_UINT32          enAppType,
    USIMM_EF_TYPE_ENUM_UINT32           enFileType)
{
    VOS_UINT32  ulRecordNum;
    VOS_UINT32  ulResult;

    if((USIMM_POOL_NOINTIAL == gstUSIMMPOOL.enPoolStatus)
        ||(POOLMAXNUM == gstUSIMMPOOL.ucNowLen))   /*״̬���*/
    {
        USIMM_WARNING_LOG("USIMM_PoolInsertOneFile: Warning: Parameter is wrong");/*��ӡ����*/

        return VOS_ERR;
    }

    ulResult = USIMM_PoolFindFile(0, &ulRecordNum, enAppType);/*��ѯ��λ��*/

    if(VOS_ERR == ulResult)/*��ǰ�ļ�������*/
    {
        USIMM_WARNING_LOG("USIMM_PoolInsertOneFile: Warning: File Could not Found");/*��ӡ����*/

        return VOS_ERR;
    }

    gstUSIMMPOOL.astpoolRecord[ulRecordNum].pucContent = (VOS_UINT8 *)VOS_MemAlloc(WUEPS_PID_USIM,
                                                                    STATIC_MEM_PT,  usLen);/*�������ݴ洢�ռ�*/

    if(VOS_NULL_PTR == gstUSIMMPOOL.astpoolRecord[ulRecordNum].pucContent)
    {
        USIMM_WARNING_LOG("USIMM_PoolInsertOneFile: Warning: VOS_MemAlloc is Error");/*��ӡ����*/

        return VOS_ERR;
    }

    VOS_MemCpy(gstUSIMMPOOL.astpoolRecord[ulRecordNum].pucContent, pData, usLen);/*������������*/

    gstUSIMMPOOL.astpoolRecord[ulRecordNum].usFId       =   usFileId;   /* ��¼�ļ�ID */
    gstUSIMMPOOL.astpoolRecord[ulRecordNum].usLen       =   usLen;      /* ��¼�ļ����� */
    gstUSIMMPOOL.astpoolRecord[ulRecordNum].enAppType   =   enAppType;  /* ��¼Ӧ������ */
    gstUSIMMPOOL.astpoolRecord[ulRecordNum].enFileType  =   enFileType; /* ��¼�ļ����� */

    gstUSIMMPOOL.ucNowLen++;/*����������ۼ�*/

    return VOS_OK;
}
VOS_UINT32 USIMM_PoolDelOneFile(VOS_UINT16 usFileId, USIMM_APP_TYPE_ENUM_UINT32 enAppType)
{
    VOS_UINT32  ulRecordNum;

    if((USIMM_POOL_NOINTIAL == gstUSIMMPOOL.enPoolStatus)
        ||(0 == gstUSIMMPOOL.ucNowLen)||(0 == usFileId))   /*״̬���*/
    {
        USIMM_ERROR_LOG("USIMM_PoolDelOneFile: Wrong State or Parameter");/*��ӡ����*/

        return VOS_ERR;
    }

    if(VOS_ERR == USIMM_PoolFindFile(usFileId, &ulRecordNum, enAppType))/*��ѯ�ļ�λ��*/
    {
        USIMM_WARNING_LOG("USIMM_PoolDelOneFile: File Could Not Found");/*��ӡ����*/

        return VOS_ERR;
    }

    if(VOS_NULL_PTR == gstUSIMMPOOL.astpoolRecord[ulRecordNum].pucContent)
    {
        USIMM_ERROR_LOG("USIMM_PoolDelOneFile: File Content is Empty");/*��ӡ����*/

        return VOS_ERR;
    }

    /* �ͷŴ洢�ռ䲢��ָ����� */
    VOS_MemFree(WUEPS_PID_USIM, gstUSIMMPOOL.astpoolRecord[ulRecordNum].pucContent);

    gstUSIMMPOOL.astpoolRecord[ulRecordNum].pucContent  = VOS_NULL_PTR;

    gstUSIMMPOOL.astpoolRecord[ulRecordNum].usFId       = 0;/*�ļ�ID���Ϊ��*/

    gstUSIMMPOOL.astpoolRecord[ulRecordNum].usLen       = 0;/*�������*/

    gstUSIMMPOOL.ucNowLen--;/*��������һ*/

    return VOS_OK;
}
VOS_UINT32 USIMM_PoolReadOneFile(
    VOS_UINT16                          usFileId,
    VOS_UINT16                          usLen,
    VOS_UINT8                          *pucData,
    USIMM_APP_TYPE_ENUM_UINT32          enAppType)
{
    VOS_UINT32  ulRecordNum;
    VOS_UINT32  ulResult;
    VOS_UINT32  ulDataLen;

    if((USIMM_POOL_NOINTIAL == gstUSIMMPOOL.enPoolStatus)
        ||(0 == gstUSIMMPOOL.ucNowLen)||(0 == usFileId))   /*״̬���*/
    {
        USIMM_WARNING_LOG("USIMM_PoolReadOneFile: Parameter wrong");/*��ӡ����*/

        return VOS_ERR;
    }

    ulResult = USIMM_PoolFindFile(usFileId, &ulRecordNum, enAppType);/*��ѯ�ļ�λ��*/

    if(VOS_ERR == ulResult)
    {
        USIMM_WARNING_LOG("USIMM_PoolReadOneFile: File Could not Found");/*��ӡ����*/

        return VOS_ERR;
    }

    if(VOS_NULL_PTR == gstUSIMMPOOL.astpoolRecord[ulRecordNum].pucContent)
    {
        USIMM_ERROR_LOG("USIMM_PoolReadOneFile: File Content is Empty");/*��ӡ����*/

        return VOS_ERR;
    }

    if(usLen == USIMM_READ_ALL_FILE_CONTENT)
    {
        ulDataLen = gstUSIMMPOOL.astpoolRecord[ulRecordNum].usLen;
    }
    else if(usLen < gstUSIMMPOOL.astpoolRecord[ulRecordNum].usLen)
    {
        ulDataLen = usLen;
    }
    else
    {
        ulDataLen = gstUSIMMPOOL.astpoolRecord[ulRecordNum].usLen;
    }

    VOS_MemCpy(pucData,gstUSIMMPOOL.astpoolRecord[ulRecordNum].pucContent,ulDataLen);

    return VOS_OK;
}
VOS_VOID USIMM_PoolDelAll(VOS_VOID)
{
    VOS_UINT32 i;

    for(i=0; i<POOLMAXNUM;i++)
    {
        if(gstUSIMMPOOL.astpoolRecord[i].pucContent != VOS_NULL_PTR)/*�ҵ����ļ�*/
        {
            VOS_MemFree(WUEPS_PID_USIM, gstUSIMMPOOL.astpoolRecord[i].pucContent);
        }
    }

    USIMM_PoolDataInit();

    return;
}
VOS_UINT32  USIMM_PoolUpdateOneFile(
    VOS_UINT16                          usFileId,
    USIMM_APP_TYPE_ENUM_UINT32          enAppType,
    VOS_UINT32                          ulDataLen,
    VOS_UINT8                          *pucData)
{
    VOS_UINT32                          ulFileNum;
    VOS_UINT32                          ulUpdateLen;

    if (VOS_OK != USIMM_PoolFindFile(usFileId, &ulFileNum, enAppType))
    {
        USIMM_WARNING_LOG("USIMM_PoolUpdateOneFile: File Could not Found");/*��ӡ����*/

        return VOS_ERR;
    }

    if (VOS_NULL_PTR == gstUSIMMPOOL.astpoolRecord[ulFileNum].pucContent)
    {
        USIMM_WARNING_LOG("USIMM_PoolUpdateOneFile: File in Pool is Null");/*��ӡ����*/

        return VOS_ERR;
    }

    ulUpdateLen =   ((ulDataLen > gstUSIMMPOOL.astpoolRecord[ulFileNum].usLen)?\
                    gstUSIMMPOOL.astpoolRecord[ulFileNum].usLen:ulDataLen);

    VOS_MemCpy(gstUSIMMPOOL.astpoolRecord[ulFileNum].pucContent, pucData, ulUpdateLen);

    return VOS_OK;
}




VOS_UINT32 USIMM_InsertFileToPool(USIMM_APP_TYPE_ENUM_UINT32 enAppType, VOS_UINT16 usFileID)
{
    VOS_UINT8                          *pucData;
    VOS_UINT32                          ulResult;
    VOS_UINT16                          usDataLen;
    VOS_UINT16                          ausPathGDF[4]={MF,ADF,0x5F3B,0x4F00};
    VOS_UINT16                          ausPathMF[2]={MF, 0x2F00};
    VOS_UINT16                          ausPathGSM[3]={MF, DFGSM, 0x6F00};
    VOS_UINT16                          ausPathADF[3]={MF, ADF, 0x6F00};
    VOS_UINT16                         *pusPath;
    VOS_UINT32                          ulPathLen;

    if(0x4F00 == (usFileID&0xFF00))
    {
        ausPathGDF[3]   = usFileID;
        pusPath         = ausPathGDF;
        ulPathLen       = ARRAYSIZE(ausPathGDF);
    }
    else if(0x2F00 == (usFileID&0xFF00))
    {
        ausPathMF[1]    = usFileID;
        pusPath         = ausPathMF;
        ulPathLen       = ARRAYSIZE(ausPathMF);
    }
    else
    {
        if(USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
        {
            ausPathADF[2]   = usFileID;
            pusPath         = ausPathADF;
            ulPathLen       = ARRAYSIZE(ausPathADF);
        }
        else
        {
            ausPathGSM[2]   = usFileID;
            pusPath         = ausPathGSM;
            ulPathLen       = ARRAYSIZE(ausPathGSM);
        }
    }

    ulResult = USIMM_SelectFile(enAppType, USIMM_NEED_FCP, ulPathLen, pusPath);

    if(VOS_OK != ulResult)/*�жϽ��*/
    {
        USIMM_WARNING_LOG("USIMM_InsertFileToPool:Select File Failed");

        return VOS_ERR;
    }

    if(gstUSIMMCurFileInfo.stEFInfo.usFileLen>USIMM_MAX_FILE_SIZE)
    {
        usDataLen = USIMM_MAX_FILE_SIZE;
    }
    else
    {
        usDataLen = gstUSIMMCurFileInfo.stEFInfo.usFileLen;
    }

    pucData = (VOS_UINT8 *)VOS_MemAlloc(WUEPS_PID_USIM, DYNAMIC_MEM_PT, usDataLen);

    if(VOS_NULL_PTR == pucData)/*�жϽ��*/
    {
        USIMM_ERROR_LOG("USIMM_InsertFileToPool: VOS_MemAlloc Error");

        return VOS_ERR;
    }

    ulResult = USIMM_ReadTFFile(usDataLen, pucData);

    if(USIMM_SW_OK != ulResult)/*�жϽ��*/
    {
        VOS_MemFree(WUEPS_PID_USIM, pucData);

        USIMM_ERROR_LOG("USIMM_InsertFileToPool: USIMM_ReadTFFile Error");

        return VOS_ERR;
    }

    if(VOS_OK != USIMM_PoolInsertOneFile(usFileID,
                                         gstUSIMMCurFileInfo.stEFInfo.usFileLen,
                                         pucData,
                                         enAppType,
                                         USIMM_EFSTRUCTURE_TRANSPARENT))
    {
        USIMM_ERROR_LOG("USIMM_InsertFileToPool: USIMM_PoolInsertOneFile Error");

        g_stUSIMMInitInfo.ucInsertPoolFailFlag = VOS_TRUE;

        VOS_MemFree(WUEPS_PID_USIM, pucData);

        return VOS_ERR;
    }

    VOS_MemFree(WUEPS_PID_USIM, pucData);

    return VOS_OK;
}
VOS_VOID USIMM_LogDataSave(VOS_UINT8 *pucData, VOS_UINT8 ucDataLen)
{
    if((USIMM_NO_NEED_LOG == g_stUSIMMLogData.enLogState)   /*��ǰ����Ҫ��¼log*/
        ||((g_stUSIMMLogData.ulDataLen + 2 + ucDataLen) > USIMM_LOG_DATA_MAX_LEN)/*��¼��log���ݳ������ֵ,����Ҫ����tag��len��2���ֽ�*/
        ||(VOS_NULL_PTR == g_stUSIMMLogData.pucData))       /*��ǰlog������������*/
    {
        return;
    }

    g_stUSIMMLogData.pucData[g_stUSIMMLogData.ulDataLen] = USIMM_LOG_DATA_TAG;  /*��¼log*/

    g_stUSIMMLogData.ulDataLen ++;              /*����+1*/

    g_stUSIMMLogData.pucData[g_stUSIMMLogData.ulDataLen] = ucDataLen;   /*��¼���ݳ���*/

    g_stUSIMMLogData.ulDataLen ++;              /*����+1*/

    VOS_MemCpy(&(g_stUSIMMLogData.pucData[g_stUSIMMLogData.ulDataLen]), pucData, ucDataLen);

    g_stUSIMMLogData.ulDataLen += ucDataLen;    /*����+���ݳ���*/

    return;
}


VOS_VOID USIMM_LogDataClear(VOS_VOID)
{
    VOS_CacheMemFree(g_stUSIMMLogData.pucData);

    g_stUSIMMLogData.ulDataLen = 0;

    g_stUSIMMLogData.enLogState = USIMM_NO_NEED_LOG;

    return;
}


VOS_VOID USIMM_SaveLogFile(VOS_VOID)
{
    FILE                                *ctrlfp;
    FILE                                *logfp;
     /* coverity[val_decl] */
    USIMM_LOG_CTRLFILE_STRU             stCtrlData;
    VOS_CHAR                            aucLogPath[100] = {0};
    VOS_CHAR                            aucLogCtrlFile[USIMM_FILE_PATH_MAX_LEN] = {0};
    VOS_CHAR                            aucUniteryLogPath[USIMM_FILE_PATH_MAX_LEN] = {0};

    /*��ǰ�Ƿ��б�������*/
    if((VOS_NULL_PTR == g_stUSIMMLogData.pucData)||(0 == g_stUSIMMLogData.ulDataLen))
    {
        return;
    }

    if (VOS_OK != OM_GetLogPath(aucLogCtrlFile, USIMM_LOGCTRL_FILE, USIMM_UNITARY_LOGCTRL_FILE))
    {
        return;
    }

    if (VOS_OK != OM_GetLogPath(aucUniteryLogPath, USIMM_LOG_PATH, USIMM_UNITARY_LOG_PATH))
    {
        return;
    }

    /*�򿪿����ļ�*/
    ctrlfp = DRV_FILE_OPEN(aucLogCtrlFile, NV_FILE_OPEN_MODE_RW);

    if(VOS_NULL_PTR == ctrlfp)
    {
        USIMM_LogDataClear();/* [false alarm]:*/
        return;
    }

    /*��ȡ�����ļ�*/
    if(sizeof(USIMM_LOG_CTRLFILE_STRU) != DRV_FILE_READ((VOS_CHAR*)&stCtrlData,
                                                    sizeof(VOS_CHAR),
                                                    sizeof(USIMM_LOG_CTRLFILE_STRU),
                                                    ctrlfp))
    {
        USIMM_LogDataClear();
        DRV_FILE_CLOSE(ctrlfp);
        return;
    }
 /* coverity[uninit_use] */
    if(stCtrlData.ulCurFile >= USIMM_LOG_FILE_MAX_NUM)
    {
        USIMM_LogDataClear();
        DRV_FILE_CLOSE(ctrlfp);
        DRV_FILE_RMFILE(aucLogCtrlFile);
        return;
    }

    /* ƴ��LOG�ļ�·�� */
    VOS_sprintf(aucLogPath, "%s/%s", aucUniteryLogPath, stCtrlData.aucFileName[stCtrlData.ulCurFile]);

    /*��log�ļ������������Ҫ���*/
    logfp = DRV_FILE_OPEN(aucLogPath, NV_FILE_OPEN_MODE_NEW_RW);

    if(VOS_NULL_PTR == logfp)
    {
        USIMM_LogDataClear();/* [false alarm]:*/
        return;
    }

    /*д������*/
    DRV_FILE_WRITE((VOS_CHAR*)g_stUSIMMLogData.pucData, sizeof(VOS_CHAR), g_stUSIMMLogData.ulDataLen, logfp);

    DRV_FILE_CLOSE(logfp);

    /*�������*/
    USIMM_LogDataClear();

    /*�����´�д����ļ���*/
    stCtrlData.ulCurFile = ((stCtrlData.ulCurFile + 1) % USIMM_LOG_FILE_MAX_NUM);

    /*д���´θ��µ��ļ���*/
    /* coverity[unchecked_value] */
    DRV_FILE_LSEEK(ctrlfp, 0, NV_FILE_SEEK_SET);

    DRV_FILE_WRITE((VOS_CHAR*)&stCtrlData.ulCurFile, sizeof(VOS_CHAR), sizeof(VOS_UINT32), ctrlfp);

    DRV_FILE_CLOSE(ctrlfp);

    return;
}


VOS_VOID USIMM_InitLogFile(VOS_VOID)
{
    VOS_CHAR                    aucLogFile[USIMM_LOG_FILE_NAME_LEN] = {0};/* [false alarm]:����Fortify���� */
    VOS_UINT32                  i;/* [false alarm]:����Fortify���� */
    VOS_UINT32                  ulStartFlag = USIMM_LOG_START_TAG;/* [false alarm]:����Fortify���� */
    USIMM_LOG_CTRLFILE_STRU     stCtrlData;/* [false alarm]:����Fortify���� */
    FILE                        *fp;
    VOS_CHAR                            aucLogCtrlFile[USIMM_FILE_PATH_MAX_LEN] = {0};
    VOS_CHAR                            aucLogPath[USIMM_FILE_PATH_MAX_LEN] = {0};

    if (VOS_OK != OM_GetLogPath(aucLogPath, USIMM_LOG_PATH, USIMM_UNITARY_LOG_PATH))
    {
        return;
    }

    if (VOS_OK != OM_GetLogPath(aucLogCtrlFile, USIMM_LOGCTRL_FILE, USIMM_UNITARY_LOGCTRL_FILE))
    {
        return;
    }

    g_stUSIMMLogData.ulDataLen  = 0;

    g_stUSIMMLogData.enLogState = USIMM_NEED_LOG;

    g_stUSIMMLogData.pucData = (VOS_UINT8*)VOS_CacheMemAlloc(USIMM_LOG_DATA_MAX_LEN);

    if(VOS_NULL_PTR == g_stUSIMMLogData.pucData)
    {
        LogPrint("\r\n USIMM_InitLogFile: malloc log buffer Error");

        g_stUSIMMLogData.enLogState = USIMM_NO_NEED_LOG;

        return;
    }

    /*�ȼ�¼һ����ʼ��ǣ���ʾ��ʼ��ʼ������*/
    USIMM_LogDataSave((VOS_UINT8*)&ulStartFlag, sizeof(VOS_UINT32));

    fp = DRV_FILE_OPEN(aucLogCtrlFile, NV_FILE_OPEN_MODE_RW);

    if(VOS_NULL_PTR != fp)  /*�ļ����ڲ���Ҫ����*/
    {
        DRV_FILE_CLOSE(fp);

        return;
    }

    if (DRV_ERROR == DRV_FILE_MKDIR(aucLogPath))/* [false alarm]:����Fortify���� */
    {
        LogPrint("\r\n USIMM_InitLogFile: Mk Dir Error");/* [false alarm]:����Fortify���� */
    }

    fp = DRV_FILE_OPEN(aucLogCtrlFile, NV_FILE_OPEN_MODE_NEW_RW);  /*�����ļ�*/

    if(VOS_NULL_PTR == fp)
    {
        LogPrint("\r\n USIMM_InitLogFile: Creat Log Ctrl File Error");/* [false alarm]:����Fortify���� */

		VOS_CacheMemFree(g_stUSIMMLogData.pucData);

		g_stUSIMMLogData.pucData = VOS_NULL_PTR;

		return;
    }

    for(i=0; i<USIMM_LOG_FILE_MAX_NUM; i++)
    {
        /* ƴ��LOG�ļ����� */
        VOS_sprintf(aucLogFile, "%s_%d", "UsimmInitLog", i);

        VOS_StrNCpy(stCtrlData.aucFileName[i], aucLogFile, USIMM_LOG_FILE_NAME_LEN);
    }

    stCtrlData.ulCurFile = 0;

    DRV_FILE_WRITE((VOS_CHAR*)&stCtrlData, sizeof(VOS_CHAR), sizeof(USIMM_LOG_CTRLFILE_STRU), fp);

    DRV_FILE_CLOSE(fp);

    return;
}
VOS_VOID USIMM_ExcLogDataClear(VOS_VOID)
{
    VOS_CacheMemFree(g_stUSIMMExcLogData.pucData);

    g_stUSIMMExcLogData.pucData = VOS_NULL_PTR;

    g_stUSIMMExcLogData.ulDataLen = 0;

    g_stUSIMMExcLogData.enLogState = USIMM_NO_NEED_LOG;

    g_ulUSIMMExcLogLenBK = 0;

    return;
}


VOS_VOID USIMM_SaveExcLogFile(VOS_VOID)
{
    FILE                                *logfp;
    VOS_CHAR                            aucLogPathFile[100] = {0};
    VOS_CHAR                            aucLogPath[USIMM_FILE_PATH_MAX_LEN] = {0};

    /*��ǰ�Ƿ��б�������*/
    if((VOS_NULL_PTR == g_stUSIMMExcLogData.pucData)||(0 == g_stUSIMMExcLogData.ulDataLen))
    {
        return;
    }

    if (VOS_OK != OM_GetLogPath(aucLogPath, USIMM_LOG_PATH, USIMM_UNITARY_LOG_PATH))
    {
        return;
    }

    /* ƴ��LOG�ļ�·�� */
    VOS_sprintf(aucLogPathFile, "%s/%s", aucLogPath, "USIMMExcFile.bin");

    /*��log�ļ������������Ҫ���*/
    logfp = DRV_FILE_OPEN(aucLogPathFile, NV_FILE_OPEN_MODE_NEW_RW);

    if(VOS_NULL_PTR == logfp)
    {
        USIMM_ExcLogDataClear();/* [false alarm]:*/
        return;
    }

    /*д������*/
    DRV_FILE_WRITE((VOS_CHAR*)g_stUSIMMExcLogData.pucData, sizeof(VOS_CHAR), g_stUSIMMExcLogData.ulDataLen, logfp);

    DRV_FILE_CLOSE(logfp);

    /*�������*/
    USIMM_ExcLogDataClear();

    return;
}


VOS_VOID USIMM_ExcLogDataBK(VOS_VOID)
{
    g_ulUSIMMExcLogLenBK = g_stUSIMMExcLogData.ulDataLen;

    return;
}


VOS_VOID USIMM_ExcLogDataRE(VOS_VOID)
{
    g_stUSIMMExcLogData.ulDataLen = g_ulUSIMMExcLogLenBK ;

    return;
}


VOS_VOID USIMM_ExcLogDataSave(VOS_UINT8 *pucData, VOS_UINT8 ucDataLen)
{
    VOS_UINT32      ulSlice;

    if((USIMM_NO_NEED_LOG == g_stUSIMMExcLogData.enLogState)   /*��ǰ����Ҫ��¼log*/
        ||((g_stUSIMMExcLogData.ulDataLen + 6 + ucDataLen) > USIMM_EXCLOG_DATA_MAX_LEN)/*��¼��log���ݳ������ֵ,����Ҫ����tag��len��2���ֽں�ʱ���4�ֽ�*/
        ||(VOS_NULL_PTR == g_stUSIMMExcLogData.pucData))       /*��ǰlog������������*/
    {
        return;
    }

    g_stUSIMMExcLogData.pucData[g_stUSIMMExcLogData.ulDataLen] = USIMM_LOG_DATA_TAG;  /*��¼log*/

    g_stUSIMMExcLogData.ulDataLen ++;              /*����+1*/

    ulSlice = OM_GetSlice();

    VOS_MemCpy(&(g_stUSIMMExcLogData.pucData[g_stUSIMMExcLogData.ulDataLen]), &ulSlice, sizeof(ulSlice));

    g_stUSIMMExcLogData.ulDataLen += sizeof(ulSlice);              /*����+1*/

    g_stUSIMMExcLogData.pucData[g_stUSIMMExcLogData.ulDataLen] = ucDataLen;   /*��¼���ݳ���*/

    g_stUSIMMExcLogData.ulDataLen ++;              /*����+1*/

    VOS_MemCpy(&(g_stUSIMMExcLogData.pucData[g_stUSIMMExcLogData.ulDataLen]), pucData, ucDataLen);

    g_stUSIMMExcLogData.ulDataLen += ucDataLen;    /*����+���ݳ���*/

    return;
}
VOS_VOID USIMM_InitExcLogFile(VOS_VOID)
{
    g_stUSIMMExcLogData.ulDataLen  = 0;

    g_stUSIMMExcLogData.enLogState = USIMM_NEED_LOG;

    g_ulUSIMMExcLogLenBK = 0;

    g_stUSIMMExcLogData.pucData = (VOS_UINT8*)VOS_CacheMemAlloc(USIMM_EXCLOG_DATA_MAX_LEN);

    if(VOS_NULL_PTR == g_stUSIMMExcLogData.pucData)
    {
        LogPrint("\r\n USIMM_InitLogFile: malloc log buffer Error");

        g_stUSIMMLogData.enLogState = USIMM_NO_NEED_LOG;

        return;
    }

    return;
}


VOS_VOID USIMM_InitGlobalVarOnPower(VOS_VOID)
{
    gulUSIMMAPIMessageNum   = 0;

    VOS_MemSet(&gstUSIMMADFInfo, 0, sizeof(gstUSIMMADFInfo));

    USIMM_FeatureGlobalInit();

    VOS_MemSet(&g_stUSIMMInitInfo, 0, sizeof(g_stUSIMMInitInfo));

    USIMM_PoolDataInit();

    VOS_MemSet(&g_stT1StatisticInfo, 0, sizeof(g_stT1StatisticInfo));

    VOS_MemSet(&g_stUSIMMT1Para, 0, sizeof(g_stUSIMMT1Para));

    g_enUSIMMTMode = USIMM_DL_T_MODE_T0;

#if(FEATURE_ON == FEATURE_PTM)
    VOS_MemSet(&g_stCardErrorLog, 0, sizeof(g_stCardErrorLog));

    if (VOS_OK != NV_Read(en_NV_Item_ErrLogCtrlInfo, &g_stErrLogCtrlInfo, sizeof(g_stErrLogCtrlInfo)))
    {
        /* Error Log�ϱ�����NV��,���ʧ�ܻ�ԭ��ʼ��nv�� */
        VOS_MemSet(&g_stErrLogCtrlInfo, 0, sizeof(g_stErrLogCtrlInfo));
    }

#endif

    return;
}
VOS_VOID USIMM_InitGlobalVarOnReset(VOS_VOID)
{
    VOS_UINT32                          i;

    VOS_MemSet(&gstUSIMMAPDU,       0, sizeof(gstUSIMMAPDU));

    VOS_MemSet(&gstUSIMMBaseInfo,    0, sizeof(gstUSIMMBaseInfo));

    VOS_MemSet(&gstUSIMMCurFileInfo, 0, sizeof(gstUSIMMCurFileInfo));

    VOS_MemSet(&gstUSIMMDLControl,   0, sizeof(gstUSIMMDLControl));

    VOS_MemSet(&gstUSIMMDrvInfo,     0, sizeof(gstUSIMMDrvInfo));

    VOS_MemSet(g_stUSIMMCardFileMap, 0, sizeof(g_stUSIMMCardFileMap));

    VOS_MemSet(g_astUSIMMChCtrl, 0, sizeof(g_astUSIMMChCtrl));

    VOS_MemSet(g_astUSIMMChAIDFCP, 0, sizeof(g_astUSIMMChAIDFCP));

    VOS_MemSet(g_astAidInfo, 0, sizeof(g_astAidInfo));

    USIMM_PoolDelAll();

    g_ulATTSpecErrSWCnt = 0;

    for(i = 0; i < USIMM_UICC_BUTT; i++)
    {
        gastUSIMMCardAppInfo[i].enCardType = USIMM_CARD_NOCARD;
        gastUSIMMCardAppInfo[i].enCardService = USIMM_CARD_SERVIC_ABSENT;
    }

    gastUSIMMCardAppInfo[USIMM_UICC_USIM].enStateChange = USIMM_CARD_STATE_CHANGED;

    gastUSIMMCardAppInfo[USIMM_UICC_ISIM].enStateChange = USIMM_CARD_STATE_NOCHANGE;

    /* ISIM��ر�����ʼ�� */
    gulUsimSupportIMS                       = VOS_FALSE;

    USIMM_T1InitGlobalVar();
    
    VOS_MemSet(&g_stUSIMMCardVersionType, VOS_NULL, sizeof(g_stUSIMMCardVersionType));

    return;
}
VOS_VOID USIMM_FeatureGlobalInit(VOS_VOID)
{
    /* NV���ȡʧ�ܰ����ж��ƶ���֧�ִ��� */
    if(VOS_OK != NV_Read(en_NV_Item_Usim_Support_Feature_Config,
                         (VOS_VOID*)&g_stUsimmFeatureCfg, sizeof(g_stUsimmFeatureCfg)))
    {
        VOS_MemSet(g_stUsimmFeatureCfg.unCfg.aulValue , VOS_FALSE, sizeof(g_stUsimmFeatureCfg.unCfg.aulValue));
    }

    return;
}
VOS_VOID USIMM_ClearCurDFInfo(USIMM_CURFILEINFO_ST *pstCurFileInfo)
{
    pstCurFileInfo->usCurDFID       = USIMM_NULL_ID;

    pstCurFileInfo->usCurDFFcpLen   = 0;

    VOS_MemSet(&(pstCurFileInfo->stDFInfo), 0, sizeof(USIMM_DFFCP_ST));

    return;
}


VOS_VOID USIMM_ClearCurEFInfo(USIMM_CURFILEINFO_ST *pstCurFileInfo)
{
    pstCurFileInfo->usCurEFID       = USIMM_NULL_ID;

    VOS_MemSet(&(pstCurFileInfo->stEFInfo), 0, sizeof(USIMM_EFFCP_ST));

    return;
}


VOS_VOID USIMM_ClearCurFileInfo(VOS_VOID)
{
    gstUSIMMCurFileInfo.usFilePathLen  = 0;

    VOS_MemSet(gstUSIMMCurFileInfo.ausFilePath, 0, sizeof(gstUSIMMCurFileInfo.ausFilePath));

    USIMM_ClearCurDFInfo(&gstUSIMMCurFileInfo);

    USIMM_ClearCurEFInfo(&gstUSIMMCurFileInfo);

    return;
}


VOS_VOID USIMM_SaveDFPinInfo(VOS_VOID)
{
    VOS_MemCpy(&gstUSIMMADFInfo.stPINInfo, &gstUSIMMCurFileInfo.stDFInfo.stPINInfo, sizeof(USIMM_PIN_INFO_STRU));

    gstUSIMMADFInfo.ucPIN1RefNum = gstUSIMMCurFileInfo.stDFInfo.ucPIN1RefNum;

    gstUSIMMADFInfo.ucPIN2RefNum = gstUSIMMCurFileInfo.stDFInfo.ucPIN2RefNum;

    return;
}


VOS_UINT32 USIMM_GetDFFcpLenInMap(VOS_UINT16 usDFId, VOS_UINT32 *pulFcpLen)
{
    VOS_UINT32 i;

    for(i=0; i<ARRAYSIZE(g_stUSIMMCardFileMap); i++)
    {
        if(g_stUSIMMCardFileMap[i].stDFFileMap.stFileCont.usDfID == usDFId)
        {
            *pulFcpLen = g_stUSIMMCardFileMap[i].stDFFileMap.ulDfFcpLen;

            return VOS_OK;
        }

        if(g_stUSIMMCardFileMap[i].stGDFFileMap.stFileCont.usDfID == usDFId)
        {
            *pulFcpLen = g_stUSIMMCardFileMap[i].stGDFFileMap.ulDfFcpLen;

            return VOS_OK;
        }
    }

    return VOS_ERR;
}


VOS_VOID USIMM_UpdateFileByRelativePath(USIMM_SELECT_PARA_STUR *pstSelectPara,
                                            USIMM_U16_LVDATA_STRU    *pstFilePath,
                                            USIMM_CURFILEINFO_ST     *pstUSIMMCurFileInfo)
{
    VOS_UINT32                          ulMaxLen;
    VOS_UINT16                          ausFileAllPath[USIMM_MAX_PATH_LEN] = {0};
    USIMM_U16_LVDATA_STRU               stFileAllPath;
    VOS_UINT32                          ulTempLen;

    if (0 == pstFilePath->ulDataLen)
    {
        USIMM_ERROR_LOG("USIMM_UpdateFileByRelativePath: The Data Path is empty.");

        return;
    }

    /* ����·������Ŀ¼���ȫ·������ */
    switch(USIMM_FILE_TYPE(pstFilePath->pusData[0]))
    {
        case EFUNDERMF:
            ulMaxLen  = 1;
            ulTempLen = 2;
            break;

        case MFTAG:
            ulMaxLen  = 4;
            ulTempLen = pstFilePath->ulDataLen;
            break;

        case EFUNDERGRANDADF:
            ulMaxLen  = 1;
            ulTempLen = 4;
            break;

        case DFUNDERDF:
            ulMaxLen  = 2;
            ulTempLen = pstFilePath->ulDataLen + 2;
            break;

        case EFUNDERDF:
            ulMaxLen  = 1;
            ulTempLen = pstFilePath->ulDataLen + 2;
            break;

        case DFUNDERMF:
            ulMaxLen  = 3;
            ulTempLen = pstFilePath->ulDataLen + 1;
            break;

        default:
            return;
    }

    if (ulMaxLen < pstFilePath->ulDataLen)
    {
        USIMM_ERROR_LOG("USIMM_UpdateFileByRelativePath: The File Path is Too Long");

        return;
    }

    /* ����ӵ�ǰ·����Ҫ��õ�·������ */
    ulTempLen -= pstFilePath->ulDataLen;

    VOS_MemCpy(ausFileAllPath, pstUSIMMCurFileInfo->ausFilePath, ulTempLen*sizeof(VOS_UINT16));
    VOS_MemCpy(&ausFileAllPath[ulTempLen], pstFilePath->pusData, pstFilePath->ulDataLen*sizeof(VOS_UINT16));

    stFileAllPath.ulDataLen = ulTempLen + pstFilePath->ulDataLen;
    stFileAllPath.pusData   = ausFileAllPath;

    USIMM_UpdateFilePath(pstSelectPara, &stFileAllPath, pstUSIMMCurFileInfo);
}


VOS_VOID USIMM_UpdateFilePath(USIMM_SELECT_PARA_STUR      *pstSelectPara,
                                        USIMM_U16_LVDATA_STRU       *pstFilePath,
                                        USIMM_CURFILEINFO_ST        *pstUSIMMCurFileInfo)
{
    VOS_UINT16                          usFileId;
    VOS_UINT32                          ulDFFcpLen;

    if (0 == pstFilePath->ulDataLen)
    {
        return;
    }

    USIMM_NORMAL_LOG("USIMM_UpdateFilePath: Need Update the File Info");

    VOS_MemCpy(pstUSIMMCurFileInfo->ausFilePath, pstFilePath->pusData, (pstFilePath->ulDataLen)*sizeof(VOS_UINT16));

    pstUSIMMCurFileInfo->usFilePathLen = (VOS_UINT16)pstFilePath->ulDataLen;

    usFileId = pstUSIMMCurFileInfo->ausFilePath[pstFilePath->ulDataLen - 1];

    /* �ж��Ƿ�ΪEF�ļ�·�� */
    if (USIMM_IS_EF_FILE(usFileId))
    {
        pstUSIMMCurFileInfo->usCurEFID = usFileId;

        pstUSIMMCurFileInfo->usCurDFID = pstUSIMMCurFileInfo->ausFilePath[pstFilePath->ulDataLen - 2];;

        USIMM_NORMAL_LOG("USIMM_UpdateFilePath: Update the EF File Info");
    }
    else
    {
        pstUSIMMCurFileInfo->usCurEFID = USIMM_NULL_ID;

        pstUSIMMCurFileInfo->usCurDFID = usFileId;

        VOS_MemSet(&pstUSIMMCurFileInfo->stEFInfo, 0 ,sizeof(USIMM_EFFCP_ST));

        USIMM_NORMAL_LOG("USIMM_UpdateFilePath: Clean the EF File Info");
    }

    USIMM_NORMAL_LOG("USIMM_UpdateFilePath: Need Update the DF Fcp Len");

    if (VOS_OK == USIMM_GetDFFcpLenInMap(pstUSIMMCurFileInfo->usCurDFID, &ulDFFcpLen))
    {
        pstUSIMMCurFileInfo->usCurDFFcpLen   = (VOS_UINT16)ulDFFcpLen;
    }
    else if (pstSelectPara->enCardType != USIMM_CARD_SIM)
    {
        pstUSIMMCurFileInfo->usCurDFFcpLen   = 0;

        USIMM_WARNING_LOG("USIMM_UpdateFilePath: Clean the DF Fcp Len");
    }
    else
    {
        USIMM_WARNING_LOG("USIMM_UpdateFilePath: Can Not Find the DF Fcp Len");
    }

    return;
}
VOS_VOID USIMM_OpenSpeedFileInit(VOS_VOID)
{
    VOS_UINT8                           aucImsi[USIMM_EF6F07Len];
    VOS_UINT8                           aucImsiDefault[USIMM_EF6F07Len] = {0x8,0x9,0x10,0x10,0x10,0x32,0x54,0x6,0x36};
    VOS_UINT8                          *pucData;

    if (VOS_OK != NV_Read(en_NV_Item_EF6F07, (VOS_VOID *)aucImsi, USIMM_EF6F07Len))
    {
        pucData = aucImsiDefault;
    }
    else
    {
        pucData = aucImsi;
    }

    USIMM_PoolInsertOneFile(EFIMSI, USIMM_EF6F07Len, pucData, USIMM_UNLIMIT_APP,
                            USIMM_EFSTRUCTURE_TRANSPARENT);

    return;
}


VOS_VOID USIMM_InitOpenSpeed(VOS_VOID)
{
    /*��ʼ����ǰ�Ļ�������*/
    gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType           = USIMM_CARD_ROM_SIM;
    gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService        = USIMM_CARD_SERVIC_AVAILABLE;

    gstUSIMMBaseInfo.enCurPINType               = USIMM_PIN2;

    gstUSIMMADFInfo.stPINInfo.ucPin1RemainTime  = 3;

    gstUSIMMADFInfo.stPINInfo.ucPin2RemainTime  = 3;

    gstUSIMMADFInfo.stPINInfo.ucPuk1RemainTime  = 10;

    gstUSIMMADFInfo.stPINInfo.ucPuk2RemainTime  = 10;

    USIMM_OpenSpeedFileInit();

    USIMM_CardStatusInd(MAPS_PIH_PID, USIMM_CARD_ROM_SIM, USIMM_CARD_SERVIC_AVAILABLE);

    return;
}
VOS_UINT32 USIMM_UsimVoltageSwitch(VOS_UINT8 ucCharaByte, VOS_UINT8 *pucVoltageMismatchFlag)
{
    VOS_INT32                           lVoltageSwitchRst;
    VOS_UINT16                          usFileID;
    VOS_UINT32                          ulResult;

    if (USIMM_VOLTAGE_1_8V == (ucCharaByte & USIMM_VOLTAGE_1_8V))
    {
        USIMM_INFO_LOG("USIMM_UsimVoltageSwitch:the current Voltage is 1.8V.");
        return VOS_OK;
    }

    if (USIMM_VOLTAGE_3V == (ucCharaByte & USIMM_VOLTAGE_3V))
    {
        *pucVoltageMismatchFlag = VOS_TRUE;

        /* ���MF��FCP��ָʾ��ѹΪ3V�Ļ������л���ѹ������API�������3V��3V���л� */
        if (VOS_OK != USIMM_DLResetCard(USIMM_CHANGE_VCC, &lVoltageSwitchRst))
        {
            USIMM_ERROR_LOG("USIMM_UsimVoltageSwitch:SIM Card Change the Voltage Failed.");

            return VOS_ERR;
        }

        if (USIMM_SCI_NONEED_CHANGEVCC == lVoltageSwitchRst)
        {
            return VOS_OK;
        }

        /* ��ѹ�л�����ѡ��һ��MF */
        usFileID = MF;

        ulResult = USIMM_SelectFile(USIMM_UMTS_APP, USIMM_NEED_FCP, 1, &usFileID);

        if (VOS_OK != ulResult)
        {
            USIMM_ERROR_LOG("USIMM_InsertNoNeedPinFileToPool: Select MF is Failed");

            return VOS_ERR;
        }

        return VOS_OK;
    }

    if (USIMM_VOLTAGE_5V == (ucCharaByte & USIMM_VOLTAGE_5V))
    {
        *pucVoltageMismatchFlag = VOS_TRUE;

        USIMM_ERROR_LOG("USIMM_UsimVoltageSwitch:Voltage is 5V.");
    }
    else
    {
        USIMM_ERROR_LOG("USIMM_UsimVoltageSwitch:the current Voltage is ERROR.");
    }

    return VOS_OK;
}


VOS_VOID USIMM_UsimTerminalCapability(VOS_VOID)
{
    VOS_UINT32                          ulResult;
    USIMM_TERMINAL_CAPABILITY_STRU      stTerminalCapability;

    /* �ж��Ƿ���Ҫ�·�terminal capability */
    if(USIMM_SUPSYSCMD == gstUSIMMCurFileInfo.stDFInfo.ucSupCmd)
    {
        /* ��nv,�ж��Ƿ�֧��terminal capability�·���֧�ֵĻ���ȡ�·����ݵ�Ԫ */
        if(NV_OK == NV_Read(en_NV_Item_TERMINAL_CAPABILITY, &stTerminalCapability, sizeof(USIMM_TERMINAL_CAPABILITY_STRU)))
        {
            if((VOS_TRUE == stTerminalCapability.ulIsEnable) && (0 != stTerminalCapability.ulLen))
            {
                /* �·�terminal capability p1 p2��������0 */
                if(VOS_OK != USIMM_Terminal_Capability_APDU(0, 0, &stTerminalCapability))
                {
                    /* �������ӡ���󣬲�Ӱ���ʼ������ */
                    USIMM_ERROR_LOG("USIMM_UsimTerminalCapability: Send terminal capability is Failed");
                }

                ulResult = USIMM_CheckSW(&gstUSIMMAPDU);/*���ѡ����*/

                /*�жϼ����*/
                if(USIMM_SW_OK != ulResult)
                {
                    USIMM_WARNING_LOG("USIMM_UsimTerminalCapability:USIMM_CheckSW error");
                }
            }
        }
    }

    return;
}


VOS_UINT32 USIMM_InsertNoNeedPinFileToPool(VOS_VOID)
{
    USIMM_APP_TYPE_ENUM_UINT32          enAppType;
    VOS_UINT32                          ulResult;
    VOS_UINT32                          i;
    VOS_UINT16                          usFileID;

    USIMM_ClearCurFileInfo();   /*���� MF ����ѡ��*/

    usFileID = MF;

    ulResult = USIMM_SelectFile(USIMM_UMTS_APP, USIMM_NEED_FCP, 1, &usFileID);

    if(ulResult != VOS_OK)
    {
        USIMM_ERROR_LOG("USIMM_InsertNoNeedPinFileToPool: Select MF is Failed");

        gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService = USIMM_CARD_SERVIC_UNAVAILABLE;          /*����ǰ������*/

        return VOS_ERR;
    }

    /* USIM���ж��Ƿ���Ҫ���е�ѹ�л����Ƿ���Ҫ�·�terminal capability */
    if (USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
    {
        ulResult = USIMM_UsimVoltageSwitch(gstUSIMMCurFileInfo.stDFInfo.ucCharaByte,
                                           &g_stUSIMMInitInfo.stUSIMMInfo.ucVoltageSwitchFlag);

        if(ulResult != VOS_OK)
        {
            USIMM_ERROR_LOG("USIMM_InsertNoNeedPinFileToPool: Select MF is Failed");

            gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService = USIMM_CARD_SERVIC_UNAVAILABLE;          /*����ǰ������*/

            return VOS_ERR;
        }

        USIMM_UsimTerminalCapability();
    }

    /*��ʼ����ǰMF�µ��ļ�*/
    for(i=0; i<USIMM_INITFIRST_FILENUM; i++)
    {
        /* ���ݿ�����ȷ��APP���ͣ����ļ��صĳ�ʼ�� */
        enAppType   =   ((USIMM_CARD_SIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)?USIMM_GSM_APP:USIMM_UMTS_APP);
        ulResult    =   USIMM_InsertFileToPool(enAppType, gausInitFileOne[i]);

        if(VOS_OK != ulResult)/*�жϽ��*/
        {
            USIMM_WARNING_LOG("USIMM_InsertNoNeedPinFileToPool: Select File under MF is Failed");
        }
    }

    USIMM_INFO_LOG("USIMM_InsertNoNeedPinFileToPool: Init Card File First Step 1 Finished");

    return VOS_OK;
}
VOS_UINT32 USIMM_InsertLPFileToPool(VOS_VOID)
{
    VOS_UINT32                          ulResult;
    USIMM_APP_TYPE_ENUM_UINT32          enAppType;

    enAppType = ((USIMM_CARD_SIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)?USIMM_GSM_APP:USIMM_UMTS_APP);

    ulResult = USIMM_InsertFileToPool(enAppType, EFLP);/*��ʼ�������ļ�����*/

    if(VOS_OK != ulResult)/*�жϽ��*/
    {
        USIMM_WARNING_LOG("USIMM_InsertLPFileToPool: Select EFLP File is Failed");
    }

    gstUSIMMPOOL.enPoolStatus = USIMM_POOL_FIRST_STEP;

    return VOS_OK;
}


VOS_UINT32 USIMM_InsertNeedPinFileToPool(VOS_VOID)
{
    VOS_UINT32                          i;
    VOS_UINT16                         *pusFile;
    VOS_UINT32                          ulFileNum;
    USIMM_APP_TYPE_ENUM_UINT32          enAppType;

    if(USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
    {
        pusFile     =   gausUsimInitMFile;
        ulFileNum   =   USIMM_INITUSIM_MFILENUM;
        enAppType   =   USIMM_UMTS_APP;

    }
    else
    {
        pusFile     =   gausSimInitMFile;
        ulFileNum   =   USIMM_INITSIM_MFILENUM;
        enAppType   =   USIMM_GSM_APP;
    }

    for(i = 0;i < ulFileNum;i++ )
    {
        if(VOS_OK != USIMM_InsertFileToPool(enAppType, pusFile[i]))
        {
            USIMM_ERROR_LOG("USIMM_InsertNeedPinFileToPool:Read Mandatory File Failed");
        }
    }

    USIMM_NORMAL_LOG("USIMM_InsertNeedPinFileToPool: USIMM Init Card File Second is Finished");

    return VOS_OK;
}

/********************************************************************
  Function:     USIMM_AttFileInit
  Description:  AT&T�����ļ���ʼ������
  Input:        ��
  Output:       ��
  Return:       ��
  Others:
********************************************************************/
VOS_VOID USIMM_AttFileInit(VOS_VOID)
{
    VOS_UINT32                          i;
    VOS_UINT16                         *pusFile;
    VOS_UINT32                          ulFileNum;

    /* �ж�AT&T�����Ƿ�� */
    if (VOS_FALSE == g_stUsimmFeatureCfg.unCfg.stFeatureCfg.ulAtt_flg)
    {
        return;
    }

    if(USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
    {
            pusFile     =   g_ausAttUsimInitFile;
            ulFileNum   =   USIMM_USIM_ATT_INITFILE_NUM;
    }
    else
    {
            pusFile     =   g_ausAttSimInitFile;
            ulFileNum   =   USIMM_SIM_ATT_INITFILE_NUM;
    }

    for (i = 0;i < ulFileNum;i++ )
    {
        USIMM_InsertAttFileToPool(USIMM_ATT_APP, pusFile[i]);
    }

    return;
}

/********************************************************************
  Function:     USIMM_InsertAttFileToPool
  Description:  AT&T�����ļ������ļ�����ش�����
  Input:        USIMM_APP_TYPE_ENUM_UINT32 enAppType:�����APP����
                VOS_UINT8 usFile:������ļ�ID
  Output:       ��
  Return:       ��
  Others:
********************************************************************/
VOS_VOID USIMM_InsertAttFileToPool(USIMM_APP_TYPE_ENUM_UINT32 enAppType, VOS_UINT16 usFile)
{
    VOS_UINT32                          ulResult;
    VOS_UINT8                          *pucData;
    VOS_UINT16                          usDataLen;

    /* ��ATT��APP������ѡ���ļ� */
    if (VOS_OK != USIMM_SelectFile(enAppType, USIMM_NEED_FCP, 1, &usFile))
    {
        return;
    }

    /* �ж��ļ��Ƿ񳬳�����������������ֵ����Ȼ����FCP�е�ֵ */
    if (gstUSIMMCurFileInfo.stEFInfo.usFileLen > USIMM_MAX_FILE_SIZE)
    {
        usDataLen = USIMM_MAX_FILE_SIZE;
    }
    else
    {
        usDataLen = gstUSIMMCurFileInfo.stEFInfo.usFileLen;
    }

    pucData = (VOS_UINT8 *)VOS_MemAlloc(WUEPS_PID_USIM, DYNAMIC_MEM_PT, usDataLen);

    if (VOS_NULL_PTR == pucData)
    {
        USIMM_ERROR_LOG("USIMM_InsertAttFileToPool: VOS_MemAlloc Error");

        return;
    }

    /* ��ȡ�ļ����ݣ������˳�������USIMMģ���ʼ�� */
    ulResult = USIMM_ReadTFFile(usDataLen, pucData);

    if(USIMM_SW_OK != ulResult)
    {
        VOS_MemFree(WUEPS_PID_USIM, pucData);

        USIMM_ERROR_LOG("USIMM_InsertAttFileToPool: USIMM_ReadTFFile Error");

        return;
    }

    /* ��AT&T���ļ����뻺����� */
    if(VOS_OK != USIMM_PoolInsertOneFile(usFile,
                                         gstUSIMMCurFileInfo.stEFInfo.usFileLen,
                                         pucData,
                                         enAppType,
                                         gstUSIMMCurFileInfo.stEFInfo.enFileType))
    {
        USIMM_ERROR_LOG("USIMM_InsertAttFileToPool: USIMM_PoolInsertOneFile Error");

        VOS_MemFree(WUEPS_PID_USIM, pucData);

        return;
    }

    VOS_MemFree(WUEPS_PID_USIM, pucData);

    return;
}


VOS_UINT32 USIMM_InitCardOptionalFile(USIMM_MsgBlock *pMsg)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulFileNum;
    USIMM_APP_TYPE_ENUM_UINT32          enAppType;
    USIMM_OPFILE_LIST_STRU             *pstFileList;

    if (USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
    {
        pstFileList =   gastUsimInitOFile;
        ulFileNum   =   USIMM_INITUSIM_OFILENUM;
        enAppType   =   USIMM_UMTS_APP;
    }
    else
    {
        pstFileList =   gastSimInitOFile;
        ulFileNum   =   USIMM_INITSIM_OFILENUM;
        enAppType   =   USIMM_GSM_APP;
    }


    for (i =  0; i < ulFileNum; i++)
    {
        if (UICC_SVR_NONE != pstFileList[i].usService)
        {
            if (PS_USIM_SERVICE_NOT_AVAILIABLE == USIMM_IsServiceAvailable(pstFileList[i].usService))
            {
                continue;
            }
        }

        if (VOS_OK != USIMM_InsertFileToPool(enAppType, pstFileList[i].usFileId))
        {
            USIMM_WARNING_LOG("USIMM_InitCardOptionalFile: Read Optional File Failed");
        }
    }

    USIMM_NORMAL_LOG("USIMM_InitCardOptionalFile: USIMM Init Card File Third is Finished");

    return VOS_OK;
}


VOS_VOID USIMM_InitUsimPBPath(VOS_VOID)
{
    VOS_UINT32              ulResult;
    VOS_UINT16              ausPath[] = {MF, DFTELCOM};

    /*GOBAL �绰������*/
    ulResult = USIMM_SelectFile(USIMM_PB_APP, USIMM_NEED_FCP, ARRAYSIZE(ausPath), ausPath);

    if(ulResult != USIMM_SW_OK)
    {
        USIMM_NORMAL_LOG("USIMM_InitUsimPBPath: DF TELCOM Not here");

        ausPath[1] = ADF;

        ulResult = USIMM_SelectFile(USIMM_UMTS_APP, USIMM_NEED_FCP, ARRAYSIZE(ausPath), ausPath);
    }

    if(ulResult == USIMM_SW_OK)                  /*��������� 5F3A, Ҳ����Ҫ����Ŀ¼��Ϣ*/
    {
       gstUSIMMBaseInfo.stPBInfo.usPBDfID = ausPath[1];
    }

    return;
}


VOS_VOID USIMM_InitCardFile(VOS_UINT32 ulDfMax, USIMM_DF_CONTENT_ST *pstDFContent)
{
    VOS_UINT32          i;
    VOS_UINT32          j;
    VOS_UINT32          k = 0;
    VOS_UINT32          m;
    VOS_UINT32          ulResult;
    VOS_UINT16          ausPath1[2] = {MF, 0};
    VOS_UINT16          ausPath2[3] = {MF, 0, 0};

    for(i=0; i<ulDfMax; i++)    /*���ݿ�һ��Ŀ¼��������ѭ��*/
    {
        ausPath1[1] = pstDFContent[i].stDFCont.usDfID;

        ulResult = USIMM_SelectFile(USIMM_UNLIMIT_APP, USIMM_NEED_FCP, ARRAYSIZE(ausPath1), ausPath1);

        if(ulResult != USIMM_SW_OK)  /*һ��Ŀ¼ѡ��ʧ��*/
        {
            continue;
        }

        /*����һ��Ŀ¼��Ϣ*/
        g_stUSIMMCardFileMap[k].stDFFileMap.ulDfFcpLen          = gstUSIMMAPDU.ulRecDataLen;
        g_stUSIMMCardFileMap[k].stDFFileMap.stFileCont.usDfID   = pstDFContent[i].stDFCont.usDfID;
        g_stUSIMMCardFileMap[k].stDFFileMap.stFileCont.pusEFList= pstDFContent[i].stDFCont.pusEFList;
        g_stUSIMMCardFileMap[k].stDFFileMap.stFileCont.usEFNum  = pstDFContent[i].stDFCont.usEFNum;

        j = k;                  /*����һ��Ŀ¼��Ϣƫ��λ��*/

        for(m=0; m<pstDFContent[i].ulGDFNum; m++)       /*��ʼ������Ŀ¼��Ϣ*/
        {
            ausPath2[1] = pstDFContent[i].stDFCont.usDfID;
            ausPath2[2] = pstDFContent[i].pstGDFCont[m].usDfID;

            ulResult = USIMM_SelectFile(USIMM_UNLIMIT_APP, USIMM_NEED_FCP, ARRAYSIZE(ausPath2), ausPath2);

            if(ulResult == USIMM_SW_OK)
            {
                if(j != k)      /*����Ŀ¼����һ������Ҫ��ǰ���ٴα���һ��Ŀ¼��Ϣ*/
                {
                    VOS_MemCpy(&g_stUSIMMCardFileMap[j].stDFFileMap, &g_stUSIMMCardFileMap[k].stDFFileMap, sizeof(USIMM_SINGLE_FILEMAP_ST));
                }

                VOS_MemCpy(&g_stUSIMMCardFileMap[j].stGDFFileMap, &pstDFContent->pstGDFCont[m], sizeof(USIMM_FILE_CONTENT_ST));

                g_stUSIMMCardFileMap[j].stGDFFileMap.ulDfFcpLen = gstUSIMMAPDU.ulRecDataLen;

                j++;            /*������Ϣָ����һ����λ��*/
            }
        }

        if(k == j)                  /*��Ŀ¼�ж���Ŀ¼����*/
        {
            k = j + 1;              /*��Ҫ�½�Ŀ¼��Ϣ*/
        }
        else
        {
            k = j;
        }
    }

    return;
}
VOS_UINT32 USIMM_InitUsimCardFileMap(VOS_VOID)
{
    USIMM_InitCardFile(USIMM_USIMDFCONTENT_MAX, g_stUsimDFContent);

    USIMM_InitUsimPBPath(); /*��ɵ绰��Ŀ¼�ĳ�ʼ��*/

    return VOS_OK;
}
VOS_UINT32 USIMM_InitSimCardFileMap(VOS_VOID)
{
    gstUSIMMBaseInfo.stPBInfo.usPBDfID = DFTELCOM;  /*SIM��Ĭ�ϵ绰��Ŀ¼*/

    USIMM_InitCardFile(USIMM_SIMDFCONTENT_MAX, g_stSimDFContent);

    return VOS_OK;
}
VOS_UINT32 USIMM_InitPINRemainTime(VOS_VOID)
{
    VOS_UINT32 ulResult;

    /*��ʼ��PIN1ʣ�����*/
    ulResult = USIMM_GetPINRemainTime(gstUSIMMCurFileInfo.stDFInfo.ucPIN1RefNum);

    if(VOS_OK != ulResult)
    {
        return VOS_ERR;
    }

    /*��ʼ��PIN2ʣ�����*/
    ulResult = USIMM_GetPINRemainTime(gstUSIMMCurFileInfo.stDFInfo.ucPIN2RefNum);

    if(VOS_OK != ulResult)
    {
        return VOS_ERR;
    }

    /*��ʼ��PUK1ʣ�����*/
    ulResult = USIMM_GetPUKRemainTime(gstUSIMMCurFileInfo.stDFInfo.ucPIN1RefNum);

    if(VOS_OK != ulResult)
    {
        return VOS_ERR;
    }

    /*��ʼ��PUK2ʣ���ʱ*/
    ulResult = USIMM_GetPUKRemainTime(gstUSIMMCurFileInfo.stDFInfo.ucPIN2RefNum);

    if(VOS_OK != ulResult)
    {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 USIMM_InitCardPINInfo(VOS_VOID)
{
    VOS_UINT32 ulResult;

    if(USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
    {
        ulResult = USIMM_InitPINRemainTime();  /*��ʼ��PINʣ�����*/

        if(VOS_OK != ulResult)
        {
            return VOS_ERR;
        }
    }

    if((USIMM_PIN_ENABLED == gstUSIMMADFInfo.stPINInfo.enPin2Enable)
        &&(0 != gstUSIMMADFInfo.stPINInfo.ucPin2RemainTime))
    {
        gstUSIMMADFInfo.enPin2Verified = USIMM_PIN_NEED;
    }

    if((USIMM_PIN_ENABLED == gstUSIMMADFInfo.stPINInfo.enPin1Enable)
        &&(0 != gstUSIMMADFInfo.stPINInfo.ucPin1RemainTime))
    {
        gstUSIMMADFInfo.enPin1Verified = USIMM_PIN_NEED;
    }

    return VOS_OK;
}
VOS_VOID  USIMM_SimGetPhase(VOS_VOID)
{
    VOS_UINT8                           ucData;
    VOS_UINT16                          ausPath[] = {MF, DFGSM, EFPHASE};
    VOS_UINT32                          ulResult;

    /* EFphase�ļ���Ĭ��ֵΪ2�Ա������FDNBDN���̿��Լ�������ȥ */
    gstUSIMMBaseInfo.ulSimPhase = USIMM_SIM_PHASE_TWO;

    /* �жϽ�����ļ�״̬ */
    ulResult = USIMM_SelectFile(USIMM_GSM_APP, USIMM_NEED_FCP, ARRAYSIZE(ausPath), ausPath);

    if (VOS_OK != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_SimGetPhase: USIMM_SelectFileWithPath(EFPHASE) Error");

        return;
    }

    if (VOS_OK != USIMM_ReadTFFile(sizeof(ucData), &ucData))
    {
        USIMM_ERROR_LOG("USIMM_SimGetPhase:Read EFPHASE Failed");

        return;
    }

    /* ���浱ǰSIM��PHASE */
    gstUSIMMBaseInfo.ulSimPhase = (VOS_UINT32)ucData;

    return;
}
VOS_VOID USIMM_InitNeedPinSteps(USIMM_MODE_TYPE_ENUM_UINT8 ucMode)
{
    VOS_UINT32                          ulRet = VOS_OK;
    if(USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
    {
        /* ��ȡUST�ļ� */
        ulRet = USIMM_InsertFileToPool(USIMM_UMTS_APP, EFUST);


        /* ��ʹFDN���̳������ĳ�ʼ������Ҳ���� */
        USIMM_InitUsimFDNBDNStatus();
    }
    else
    {
        /* ��ȡSST�ļ� */
        ulRet = USIMM_InsertFileToPool(USIMM_GSM_APP, EFUST);

        USIMM_SimGetPhase();

        USIMM_SimProfileDownload(ucMode);

        USIMM_InitSimFDNBDNStatus();
    }

    if(VOS_OK != ulRet)
    {
        USIMM_WARNING_LOG("USIMM_InitNeedPinSteps: Read SST File Failed");
    }

    gstUSIMMPOOL.enPoolStatus = USIMM_POOL_AVAILABLE;

    if(VOS_ERR == USIMM_InsertNeedPinFileToPool())     /*��ʼ�������ļ�����*/
    {
        USIMM_ERROR_LOG("USIMM_InitNeedPinSteps: USIMM_InsertNeedPinFileToPool Error");
    }

    if(USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
    {
        USIMM_Status_APDU(USIMM_STATUS_CURAPP_INITIALIZED, USIMM_STATUS_NO_DATA_RETURNED, 0x00);
    }

    USIMM_NORMAL_LOG("USIMM_InitNeedPinSteps: USIMM_InsertNeedPinFileToPool Finished");

    return;
}
VOS_UINT32 USIMM_InitCardTypeByATR(VOS_VOID)
{
    VOS_UINT32                          ulResult    = VOS_ERR;
    VOS_UINT32  ulErrlogNumber;
    OAM_MNTN_NO_SIM_CARD_EVENT_STRU     stNoSimCard;
    VOS_INT32                           lVoltageSwitchRst;

    ulResult = USIMM_DLResetCard(USIMM_RESET_CARD, &lVoltageSwitchRst);

    if(VOS_OK != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_InitCardFirstStep: Reset Card is Failed");

        /* ��Ϊ˫��˫�����Ը�16λ��¼pid,��16λ��¼ԭ����id */
        ulErrlogNumber = WUEPS_PID_USIM;
        ulErrlogNumber <<= 16;
        ulErrlogNumber |= MNTN_OAM_NO_SIM_CARD_EVENT;

        if(VOS_OK != MNTN_RecordErrorLog(ulErrlogNumber, (void *)&stNoSimCard, sizeof(OAM_MNTN_NO_SIM_CARD_EVENT_STRU)))
        {
            USIMM_ERROR_LOG("USIMM_InitCardFirstStep: Fail to record no SIM card in Errorlog file");
        }
        return VOS_ERR;
    }

    ulResult = USIMM_DecodeATR(); /*�жϿ�����*/

    if(VOS_OK != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_InitCardFirstStep: USIMM_DecodeATR is Failed");/*��ӡ����*/

        gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType = USIMM_CARD_NOCARD;        /*��ǰ״̬Ϊ�޿�*/

        return VOS_ERR;
    }

    return VOS_OK;                                 /*��ʼ����һ�����*/
}
VOS_UINT32 USIMM_InitUsimGlobal(VOS_VOID)
{
    gstUSIMMBaseInfo.ucCLA        = USIMM_USIM_CLA;

    gastUSIMMCardAppInfo[USIMM_UICC_USIM].enStateChange = USIMM_CARD_STATE_CHANGED; /*��Ҫ�ϱ���״̬*/

    return VOS_OK;
}
VOS_UINT32 USIMM_InitSimGlobal(VOS_VOID)
{
    gstUSIMMBaseInfo.ucCLA                              = USIMM_SIM_CLA;

    gastUSIMMCardAppInfo[USIMM_UICC_USIM].enStateChange = USIMM_CARD_STATE_CHANGED; /*��Ҫ�ϱ���״̬*/

    /* SIM ��֧�ֹ��� */
    gstUSIMMBaseInfo.enSupportPath                      = USIMM_NOTSUPPORT_PATH;

    gstUSIMMBaseInfo.enSupportSFI                       = USIMM_NOTSUPPORT_SFI;

    gstUSIMMBaseInfo.enSupportChannel                   = USIMM_NOTSUPPORT_CHANNEL;

    return VOS_OK;
}


VOS_VOID USIMM_Reselect3GApp(VOS_VOID)
{
    VOS_UINT32                          ulResult;
    USIMM_APDU_ST                       stApduInfo = {0};
    VOS_UINT8                           ucP2;
    VOS_UINT8                           ucTmp;

    /* make pclint happy */
    ucP2 = USIMM_SELECT_TERMINATION_AID;
    ucTmp= USIMM_SELECT_RETURN_FCP_TEMPLATE;
    ucP2 |= ucTmp;

    stApduInfo.aucAPDU[P1] = USIMM_SELECT_BY_DF_NAME;
    stApduInfo.aucAPDU[P2] = ucP2;
    stApduInfo.aucAPDU[P3] = (VOS_UINT8)gstUSIMMADFInfo.ulAIDLen;

    VOS_MemCpy(stApduInfo.aucSendBuf, gstUSIMMADFInfo.aucAID, (VOS_UINT8)gstUSIMMADFInfo.ulAIDLen);

    if (VOS_OK != USIMM_SelectFile_APDU(&stApduInfo))
    {
        USIMM_ERROR_LOG("USIMM_Reselect3GApp: Termination ADF Fail");
    }

    /* ����AIDѡ��3GӦ�� */
    ulResult = USIMM_DecodeEFDIR();

    /* �������EFDIR�ɹ���˵����USIM������ѡAID */
    if (VOS_OK == ulResult)
    {
       USIMM_SelectUiccADF();
    }

    return;
}
VOS_UINT32 USIMM_RefreshAidVerify(VOS_UINT32 ulAidLen, VOS_UINT8 *pucAid)
{
    /* REFRESH������û��AIDʱ������ȷ */
    if (VOS_NULL == ulAidLen)
    {
        return VOS_OK;
    }

    /* ��ǰû��AID��������SIM�� */
    if (VOS_NULL == gstUSIMMADFInfo.ulAIDLen)
    {
        return VOS_OK;
    }

    if (VOS_OK == VOS_MemCmp(pucAid, gstUSIMMADFInfo.aucAID, ulAidLen))
    {
        return VOS_OK;
    }

    return VOS_ERR;
}


VOS_UINT32 USIMM_Select3GApp(VOS_VOID)
{
    VOS_UINT32              ulResult;

    /* ����AIDѡ��3GӦ�� */
    ulResult = USIMM_DecodeEFDIR();

    /* �������EFDIR�ɹ���˵����USIM������ATR���ݽ����ĵڶ��� */
    if (VOS_OK == ulResult)
    {
        USIMM_DecodeATRSecondStep();
    }

    return ulResult;
}


VOS_UINT32 USIMM_Select2GApp(VOS_VOID)
{
    VOS_INT32                           lVoltageSwitchRst;
    VOS_UINT16                          ausPath[] = {MF, DFGSM};
    VOS_UINT32                          ulResult;

    USIMM_INFO_LOG("USIMM_Select2GApp: This Card is Sim Card");

    if(VOS_OK != USIMM_CheckCLStep())
    {
        USIMM_WARNING_LOG("USIMM_Select2GApp: Check is Failed with CL enable");     /*��ӡ����*/

        return VOS_ERR;
    }

    ulResult = USIMM_SelectFile(USIMM_GSM_APP, USIMM_NEED_FCP, ARRAYSIZE(ausPath), ausPath);

    if (VOS_OK != ulResult)
    {
        USIMM_WARNING_LOG("USIMM_Select2GApp: Select SIM Card DFGSM is Failed");     /*��ӡ����*/

        gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType = USIMM_CARD_NOCARD;

        return VOS_ERR;
    }

    USIMM_SaveDFPinInfo();

    if (0x10 == (gstUSIMMCurFileInfo.stDFInfo.ucCharaByte & 0x70)) /*SIM�������ļ������ֽ��л���ѹ*/
    {
        USIMM_NORMAL_LOG("USIMM_Select2GApp: SIM Card Need Change the Voltage.");

        g_stUSIMMInitInfo.stUSIMMInfo.ucVoltageSwitchFlag = VOS_TRUE;

        /*����λʧ�ܰ����޿�����*/
        if (VOS_OK != USIMM_DLResetCard(USIMM_CHANGE_VCC, &lVoltageSwitchRst))
        {
            USIMM_ERROR_LOG("USIMM_Select2GApp:SIM Card Change the Voltage Failed.");

            return VOS_ERR;      /*����Ҫ�������жϴ���*/
        }

        /* ��ѹδ�л�ֱ�ӷ��� */
        if (USIMM_SCI_NONEED_CHANGEVCC == lVoltageSwitchRst)
        {
            return VOS_OK;
        }

        ulResult = USIMM_SelectFile(USIMM_GSM_APP, USIMM_NEED_FCP, ARRAYSIZE(ausPath), ausPath);

        /* ��ѹ�л�����ѡ��һ��DFGSM */
        if (VOS_OK != ulResult)
        {
            USIMM_ERROR_LOG("USIMM_Select2GApp: Select MF is Failed");

            gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType = USIMM_CARD_NOCARD;

            return VOS_ERR;      /*����Ҫ�������жϴ���*/
        }
    }

    return VOS_OK;
}


VOS_VOID USIMM_GetTerminalProfile(USIMM_STK_CFG_STRU *pstProfileContent)
{
    VOS_UINT32                          ulResult;
    VOS_UINT8                           aucUsimProfile[]=STK_USIM_PROFILE_DEFAULT;
    VOS_UINT8                           aucSimProfile[]= STK_SIM_PROFILE_DEFAULT;

    /*��ȡNV���жϵ�ǰ��SAT״̬*/
    ulResult = NV_Read(en_NV_Item_TerminalProfile_Set, pstProfileContent, sizeof(USIMM_STK_CFG_STRU));

    if(NV_OK == ulResult)/*��ȡʧ�ܰ���Ĭ���·�*/
    {
        return;
    }

    USIMM_WARNING_LOG("USIMM_GetTerminalProfile: NV_Read Failed");

    if (USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
    {
        VOS_MemCpy(pstProfileContent->stUsimProfile.aucProfile, aucUsimProfile, sizeof(aucUsimProfile));

        pstProfileContent->stUsimProfile.ucProfileLen = sizeof(aucUsimProfile);
    }
    else
    {
        VOS_MemCpy(pstProfileContent->stSimProfile.aucProfile, aucSimProfile, sizeof(aucSimProfile));

        pstProfileContent->stSimProfile.ucProfileLen = sizeof(aucSimProfile);
    }

    return;
}


VOS_UINT32 USIMM_UsimProfileDownload(VOS_VOID)
{
    USIMM_STK_CFG_STRU                  stProfileContent;

    /* ���֮ǰ�Ѿ�DOWNLOADʧ�ܣ��Ͳ�����Ҫ������ */
    if (USIMM_PROFILE_DOWNLOAD_FAIL == g_stUSIMMInitInfo.stUSIMMInfo.enTPDownloadStatus)
    {
        return USIMM_INIT_OK;
    }

    USIMM_GetTerminalProfile(&stProfileContent);

    if (VOS_OK != USIMM_TermimalProfile_APDU(stProfileContent.stUsimProfile.ucProfileLen,
                                             stProfileContent.stUsimProfile.aucProfile))
    {
        g_stUSIMMInitInfo.stUSIMMInfo.enTPDownloadStatus = USIMM_PROFILE_DOWNLOAD_FAIL;

        USIMM_ERROR_LOG("USIMM_UsimProfileDownload: USIMM_TermimalProfile_APDU Error");

        return USIMM_INIT_RESET;
    }

    return USIMM_INIT_OK;
}
VOS_VOID USIMM_SimWarmResetProc(VOS_VOID)
{
    VOS_INT32                           lResult;
    VOS_UINT32                          ulResult;

    /* ��λ���������λʧ�ܻὫ�����͵�ȫ�ֱ�����Ϊ�޿�����˸�λ�ɹ���Ҫ���ÿ����͵�ȫ�ֱ��� */
    if (VOS_OK != DRV_USIMMSCI_DEACT())
    {
        USIMM_ERROR_LOG("USIMM_SimHotResetProc: Deactive Card Error");

        return;
    }

    if (VOS_OK != USIMM_DLResetCard(USIMM_RESET_CARD, &lResult))
    {
        USIMM_ERROR_LOG("USIMM_SimHotResetProc: USIMM_DLResetCard Error");

        return;
    }

    if (VOS_OK != USIMM_Select2GApp())
    {
        USIMM_ERROR_LOG("USIMM_SimHotResetProc: USIMM_Select2GApp Error");

        return;
    }

    if (VOS_OK != USIMM_InitCardPINInfo())
    {
        USIMM_ERROR_LOG("USIMM_SimHotResetProc: USIMM_InitCardPINInfo Error");

        return;
    }

    USIMM_AdjudgeCardState();

    ulResult = USIMM_AutoVerifyPIN(gstUSIMMADFInfo.stPINInfo.enPin1Enable,
                                   &gstUSIMMADFInfo.enPin1Verified,
                                   gstUSIMMADFInfo.ucPIN1RefNum,
                                   gstUSIMMADFInfo.aucPin1);

    if (VOS_OK != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_SimHotResetProc: USIMM_AutoVerifyPIN Error");
    }

    USIMM_AdjudgeCardState();

    return;
}
VOS_VOID USIMM_SimProfileDownload(USIMM_MODE_TYPE_ENUM_UINT8 ucMode)
{
    VOS_UINT32                          ulResult;
    USIMM_STK_CFG_STRU                  stProfileContent;

    /* �ǵ縴λ��refresh����Ҫ�·�terminal profile */
    if (USIMM_REFRESH_MODE == ucMode)
    {
        return;
    }

    /* ���֮ǰ�Ѿ�DOWNLOADʧ�ܣ��Ͳ�����Ҫ������ */
    if (USIMM_PROFILE_DOWNLOAD_FAIL == g_stUSIMMInitInfo.stUSIMMInfo.enTPDownloadStatus )
    {
        return;
    }

    /* PHASE1��SIM������Ҫ�·�terminal profile �ʹ���BDN FDN���̣�ֻ��PHASE2+�Ŀ�����Ҫ */
    if(gstUSIMMBaseInfo.ulSimPhase < USIMM_SIM_PHASE_TWO)
    {
        USIMM_WARNING_LOG("USIMM_SimProfileDownload: SIM Not Support SAT");

        return;
    }

    /*��ȡNV���жϵ�ǰ��SAT״̬*/
    USIMM_GetTerminalProfile(&stProfileContent);

    ulResult = USIMM_TermimalProfile_APDU(stProfileContent.stSimProfile.ucProfileLen,
                                          stProfileContent.stSimProfile.aucProfile);

    if(VOS_OK != ulResult)
    {
        g_stUSIMMInitInfo.stUSIMMInfo.enTPDownloadStatus = USIMM_PROFILE_DOWNLOAD_FAIL;

        USIMM_ERROR_LOG("USIMM_SimProfileDownload: USIMM_TermimalProfile_APDU Error");

        USIMM_SimWarmResetProc();

        return;
    }

    /* ATT���ƹ��̣�ʧ����Ҳ������������ʼ�� */
    USIMM_AttUpdataSupportTable();

    return;

}

/********************************************************************
  Function:     USIMM_AttEfTerminalSupportTableValueGet
  Description:  ��ȡ�ն���������ֵ
  Input:        ��
  Output:       ��
  Return:       ��
  Others:
********************************************************************/
VOS_VOID USIMM_AttEfTerminalSupportTableValueGet(VOS_UINT8 *pucValue)
{
    VOS_UINT8                           aucSuppotTableDefault[USIMM_ATT_SUPPORT_TABLE_MAX]  =
                                                            {0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x02, 0x00,
                                                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    NVIM_SCAN_CTRL_STRU                 stNvScanCtrl;
    NVIM_ATT_ENS_CTRL_STRU              stNvEnsCtrl;
    NVIM_ACTING_HPLMN_SUPPORT_FLAG_STRU stNvActingHplmnCtrl;
    NAS_NVIM_MS_CLASS_STRU              stMsMode;

    if (VOS_OK == NV_Read(en_NV_Item_Scan_Ctrl_Para, (VOS_VOID *)&stNvScanCtrl, sizeof(NVIM_SCAN_CTRL_STRU)))
    {
        aucSuppotTableDefault[USIMM_ATT_SCAN_CTRL_OFFSET]           =   stNvScanCtrl.ucNvimActiveFlg;
    }

    if (VOS_OK == NV_Read(en_NV_Item_Att_Ens_Ctrl_Para, (VOS_VOID *)&stNvEnsCtrl, sizeof(NVIM_ATT_ENS_CTRL_STRU)))
    {
        aucSuppotTableDefault[USIMM_ATT_NETWORK_SELECTION_OFFSET]   =   stNvEnsCtrl.ucNetSelMenuFlg;
        aucSuppotTableDefault[USIMM_ATT_RAT_BALANCING]              =   stNvEnsCtrl.ucRatBalancingFlg;
    }

    if (VOS_OK == NV_Read(en_NV_Item_ACTING_PLMN_SUPPORT_FLAG, (VOS_VOID *)&stNvActingHplmnCtrl,
                          sizeof(NVIM_ACTING_HPLMN_SUPPORT_FLAG_STRU)))
    {
        aucSuppotTableDefault[USIMM_ATT_ACTING_HPLMN]               =    stNvActingHplmnCtrl.ucNvimActiveFlg;
    }

    if (VOS_OK == NV_Read(en_NV_Item_MMA_MsClass, &stMsMode, sizeof(stMsMode)))
    {
        aucSuppotTableDefault[USIMM_ATT_LTE_SUPPORT]                = (NV_MS_MODE_PS_ONLY == stMsMode.ucMsClass)?USIMM_ATT_LTE_DATA_ONLY:USIMM_ATT_LTE_DATE_VOICE_CSFB;
    }

    VOS_MemCpy(pucValue, aucSuppotTableDefault, sizeof(aucSuppotTableDefault));

    return;
}

/********************************************************************
  Function:     USIMM_AttUpdataSupportTable
  Description:  ATT���ն���������д��6FD2�ļ�
  Input:        ��
  Output:       ��
  Return:       ��
  Others:
********************************************************************/
VOS_VOID USIMM_AttUpdataSupportTable(VOS_VOID)
{
    VOS_UINT8                               aucSuppotTable[USIMM_ATT_SUPPORT_TABLE_MAX]={0};
    VOS_UINT16                              usActureLen;
    VOS_UINT16                              usFileID = EFTERMINALSUPPORTTABLE;

    /* �ж�AT&T�����Ƿ�� */
    if (VOS_FALSE == g_stUsimmFeatureCfg.unCfg.stFeatureCfg.ulAtt_flg)
    {
        return;
    }

    /* ѡ��6FD2�ļ� */
    if(VOS_OK != USIMM_SelectFile(USIMM_ATT_APP, USIMM_NEED_FCP, 1, &usFileID))
    {
        USIMM_ERROR_LOG("USIMM_AttUpdataSupportTable: Select File Error 0x6FD2");

        return;
    }

    USIMM_AttEfTerminalSupportTableValueGet(aucSuppotTable);

    /* 6FD2�ļ��ĳ���ֻ��Ϊ8��16��С��8��ֵ */
    if (USIMM_ATT_SUPPORT_TABLE_MAX <= gstUSIMMCurFileInfo.stEFInfo.usFileLen)
    {
        usActureLen =   USIMM_ATT_SUPPORT_TABLE_MAX;
    }
    else if (USIMM_ATT_SUPPORT_TABLE_MIN <= gstUSIMMCurFileInfo.stEFInfo.usFileLen)
    {
        usActureLen =   USIMM_ATT_SUPPORT_TABLE_MIN;
    }
    else
    {
        usActureLen =   gstUSIMMCurFileInfo.stEFInfo.usFileLen;
    }

    if (VOS_OK != USIMM_UpdateTFFile(usActureLen, aucSuppotTable))
    {
        USIMM_ERROR_LOG("USIMM_AttUpdataSupportTable: USIMM_UpdateTFFile Fail");
    }

    return;
}
VOS_UINT32 USIMM_InitUsimEccFile(VOS_VOID)
{
    VOS_UINT8                          *pData;
    VOS_UINT8                           ucEccType;
    VOS_UINT8                           ucRecordNum =0;
    VOS_UINT32                          ulResult;
    VOS_UINT16                          ausPath[] = {MF, ADF, EFECC};

    ulResult = USIMM_SelectFile(USIMM_UMTS_APP, USIMM_NEED_FCP, ARRAYSIZE(ausPath), ausPath);

    if(VOS_OK != ulResult)/*�жϽ��*/
    {
        g_stUSIMMInitInfo.stUSIMMInfo.enEccInitStatus = USIMM_ECC_SELECT_FAIL;

        USIMM_ERROR_LOG("USIMM_InitUsimEccFile:usim card select ECC file fail");

        /* ECC�ļ������ڵ��������PB�ϱ��ļ�����Ϊ0������ok������ֹ��ʼ������ */
        USIMM_EccNumberInd(ECC_USIM, VOS_NULL, VOS_NULL, VOS_NULL_PTR);

        return VOS_OK;
    }

    if(USIMM_EFSTRUCTURE_FIXED != gstUSIMMCurFileInfo.stEFInfo.enFileType)
    {
        g_stUSIMMInitInfo.stUSIMMInfo.enEccInitStatus = USIMM_ECC_TYPE_ERR;

        USIMM_ERROR_LOG("USIMM_InitUsimEccFile:ECC FILE TYPE error");

        /* ECC�ļ������ڵ��������PB�ϱ��ļ�����Ϊ0������ok������ֹ��ʼ������ */
        USIMM_EccNumberInd(ECC_USIM, VOS_NULL, VOS_NULL, VOS_NULL_PTR);

        return VOS_OK;
    }

    pData = (VOS_UINT8 *)VOS_MemAlloc(WUEPS_PID_USIM, DYNAMIC_MEM_PT,
                        gstUSIMMCurFileInfo.stEFInfo.usFileLen);/*�������ݴ洢�ռ�*/

    if(VOS_NULL_PTR == pData)/*�жϽ��*/
    {
        USIMM_ERROR_LOG("USIMM_InitUsimEccFile:VOS_MemAlloc error");

        return VOS_ERR;
    }

    ucEccType   = ECC_USIM;

    ucRecordNum = gstUSIMMCurFileInfo.stEFInfo.ucRecordNum;

    ulResult    = USIMM_ReadLFFile(&gstUSIMMCurFileInfo.stEFInfo, USIMM_READ_ALLRECORD, pData);

    if(USIMM_SW_OK != ulResult)/*�жϽ��*/
    {
        g_stUSIMMInitInfo.stUSIMMInfo.enEccInitStatus = USIMM_ECC_READ_FAIL;

        VOS_MemFree(WUEPS_PID_USIM, pData);

        USIMM_ERROR_LOG("USIMM_InitUsimEccFile:USIMM_ReadLFFile error");

        /* ECC�ļ������ڵ��������PB�ϱ��ļ�����Ϊ0������ok������ֹ��ʼ������ */
        USIMM_EccNumberInd(ECC_USIM, VOS_NULL, VOS_NULL, VOS_NULL_PTR);

        return VOS_OK;
    }

    USIMM_EccNumberInd(ucEccType, gstUSIMMCurFileInfo.stEFInfo.usFileLen, ucRecordNum, pData);/*�ϱ���ǰ��Ecc��������*/

    VOS_MemFree(WUEPS_PID_USIM, pData);

    return VOS_OK;

}


VOS_UINT32 USIMM_InitSimEccFile(VOS_VOID)
{
    VOS_UINT8                           *pData;
    VOS_UINT8                           ucEccType;
    VOS_UINT8                           ucRecordNum =0;
    VOS_UINT32                          ulResult;
    VOS_UINT16                          ausPath[] = {MF, DFGSM, EFECC};

    ulResult = USIMM_SelectFile(USIMM_GSM_APP, USIMM_NEED_FCP, ARRAYSIZE(ausPath), ausPath);

    if(VOS_OK != ulResult)/*�жϽ��*/
    {
        g_stUSIMMInitInfo.stUSIMMInfo.enEccInitStatus = USIMM_ECC_SELECT_FAIL;

        /* ECC�ļ������ڵ��������PB�ϱ��ļ�����Ϊ0������ok������ֹ��ʼ������ */
        USIMM_EccNumberInd(ECC_SIM, VOS_NULL, VOS_NULL, VOS_NULL_PTR);

        return VOS_OK;
    }

    if(USIMM_EFSTRUCTURE_TRANSPARENT != gstUSIMMCurFileInfo.stEFInfo.enFileType)
    {
        g_stUSIMMInitInfo.stUSIMMInfo.enEccInitStatus = USIMM_ECC_TYPE_ERR;

        USIMM_ERROR_LOG("USIMM_InitSimEccFile:ECC FILE TYPE error");

        /* ECC�ļ������ڵ��������PB�ϱ��ļ�����Ϊ0������ok������ֹ��ʼ������ */
        USIMM_EccNumberInd(ECC_SIM, VOS_NULL, VOS_NULL, VOS_NULL_PTR);

        return VOS_OK;
    }

    pData = (VOS_UINT8 *)VOS_MemAlloc(WUEPS_PID_USIM,DYNAMIC_MEM_PT,
                        gstUSIMMCurFileInfo.stEFInfo.usFileLen);/*�������ݴ洢�ռ�*/

    if(VOS_NULL_PTR == pData)/*�жϽ��*/
    {
        USIMM_ERROR_LOG("USIMM_InitSimEccFile:VOS_MemAlloc error");

        return VOS_ERR;
    }

    ucEccType = ECC_SIM;

    ulResult = USIMM_ReadTFFile(gstUSIMMCurFileInfo.stEFInfo.usFileLen, pData);

    if(USIMM_SW_OK != ulResult)/*�жϽ��*/
    {
        g_stUSIMMInitInfo.stUSIMMInfo.enEccInitStatus = USIMM_ECC_READ_FAIL;

        VOS_MemFree(WUEPS_PID_USIM, pData);

        USIMM_ERROR_LOG("USIMM_InitSimEccFile:USIMM_ReadTFFile error");

        /* ��ȡ�ļ�ʧ����ΪECC�ļ������ڣ���PB�ϱ��ļ�����Ϊ0������ok������ֹ��ʼ������ */
        USIMM_EccNumberInd(ECC_SIM, VOS_NULL, VOS_NULL, VOS_NULL_PTR);

        return VOS_OK;
    }

    USIMM_EccNumberInd(ucEccType, gstUSIMMCurFileInfo.stEFInfo.usFileLen, ucRecordNum, pData);/*�ϱ���ǰ��Ecc��������*/

    VOS_MemFree(WUEPS_PID_USIM, pData);

    return VOS_OK;
}
VOS_UINT32 USIMM_PowerDownResetPrepare(VOS_VOID)
{
    VOS_UINT32                          ulResult;
    VOS_INT32                           lVoltageSwitchRst;

    /* USIMM����ʼ����һ��ʧ��ʱ����SIM����ʼ������ʱ��Ҫ�������� */
    USIMM_InitGlobalVarOnReset();

    if(VOS_OK != DRV_USIMMSCI_DEACT())
    {
        USIMM_ERROR_LOG("USIMM_InitNoNeedPinSteps: Deactive Card Error");

        return VOS_ERR;
    }

    ulResult = USIMM_DLResetCard(USIMM_RESET_CARD, &lVoltageSwitchRst);

    if(VOS_OK != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_InitNoNeedPinSteps: Reset Card is Failed");

        return VOS_ERR;
    }

    return VOS_OK;
}

/*****************************************************************************
�� �� ��  :USIMM_CheckCLStep
��������  :C+Lѡ����2G���Ƿ����
�������  :��
�������  :��
�� �� ֵ  :VOS_ERR
           VOS_OK

�޶���¼  :
1. ��    ��   : 2014��01��16��
   ��    ��   : zhuli
   �޸�����   :
2. ��    ��   : 2014��6��16��
   ��    ��   : wangshaojun
   �޸�����   : DTS 2014061601766

*****************************************************************************/
VOS_UINT32 USIMM_CheckCLStep(VOS_VOID)
{
    VOS_UINT16                          ausCDMAPath[] = {MF, DFCDMA};
    VOS_UINT16                          ausGSMPath[] = {MF, DFGSM};
    VOS_UINT32                          ulCDMAResult = VOS_OK;
    VOS_UINT32                          ulGsmResult = VOS_OK;
    TAF_NV_LC_CTRL_PARA_STRU            stNVData;
    VOS_INT32                           lVoltageSwitchRst;

    if (NV_OK != NV_ReadEx(MODEM_ID_0, en_NV_Item_LC_Ctrl_PARA, &stNVData, sizeof(TAF_NV_LC_CTRL_PARA_STRU)))
    {
        return VOS_OK;
    }

    if (VOS_TRUE == stNVData.ucLCEnableFlg)
    {
        ulCDMAResult = USIMM_SelectFile(USIMM_GSM_APP, USIMM_NEED_FCP, ARRAYSIZE(ausCDMAPath), ausCDMAPath);
        ulGsmResult = USIMM_SelectFile(USIMM_GSM_APP, USIMM_NEED_FCP, ARRAYSIZE(ausGSMPath), ausGSMPath);

        g_stUSIMMCardVersionType.enCardMode     = USIMM_CARD_ICC_TYPE;
        g_stUSIMMCardVersionType.ucHasCModule   = (VOS_OK == ulCDMAResult);
        g_stUSIMMCardVersionType.ucHasWGModule  = (VOS_OK == ulGsmResult);

        if(VOS_OK != DRV_USIMMSCI_DEACT())      /*���ʿ����ճ�ʼ������Ҫ����*/
        {
            USIMM_ERROR_LOG("USIMM_CheckCLStep: Deactive Card Error");
        }

        if(VOS_OK != USIMM_DLResetCard(USIMM_RESET_CARD, &lVoltageSwitchRst))
        {
            USIMM_ERROR_LOG("USIMM_CheckCLStep: Reset Card is Failed");
        }
    }

    if (VOS_FALSE == USIMM_IsCLEnable())
    {
        USIMM_WARNING_LOG("USIMM_Select2GApp: C+L is Off");     /*��ӡ����*/

        return VOS_OK;
    }

    if (VOS_OK != ulCDMAResult)
    {
        USIMM_WARNING_LOG("USIMM_Select2GApp: Select Card 7F25 is Failed");     /*��ӡ����*/

        gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType = USIMM_CARD_NOCARD;

        return VOS_ERR;
    }

    if (TAF_NVIM_LC_RAT_COMBINED_CL == USIMM_GetRATMode())   /*�����ǹ��ʿ�����UIM�����˳������޿��ϱ�*/
    {
        gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType = USIMM_CARD_NOCARD;

        (VOS_VOID)USIMM_SelectFile(USIMM_GSM_APP, USIMM_NEED_FCP, ARRAYSIZE(ausCDMAPath), ausCDMAPath);

        return VOS_ERR;
    }
    else
    {
        return VOS_OK;
    }
}

VOS_UINT32 USIMM_InitNoNeedPinSteps(VOS_VOID)
{
    VOS_UINT32                          ulResult;
    VOS_UINT32                          i;
    VOS_UINT32                          j;

    if(USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)     /*��ǰΪUSIM*/
    {
        i = 0;
    }
    else         /*��ǰΪSIM*/
    {
        i = 1;
    }

    for( ; i < USIMM_CARD_INIT_TBL_SIZE; i++)
    {
        for(j = 1; j < g_aCardInitTbl[i].ulStepNum; j++)
        {
            ulResult = g_aCardInitTbl[i].pInitFunTbl[j].pProcFun();

            if(VOS_ERR == ulResult)
            {
                if(USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
                {
                    if(VOS_OK != USIMM_PowerDownResetPrepare())
                    {
                        return VOS_ERR;
                    }

                    /* ������ǰѭ��, ��SIM����ʼ�� */
                    gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType = USIMM_CARD_SIM;
                    g_stUSIMMInitInfo.stUSIMMInfo.enTPDownloadStatus = USIMM_PROFILE_DOWNLOAD_SUCCESS;

                    break;
                }
                else
                {
                    /* SIM��ĳ����ʼ��������ֱ�ӷ��� */
                    return VOS_ERR;
                }
            }

            /* ��������ģʽ��Ŀǰֻ��TP����ʧ��֮����Ҫ���� */
            if (USIMM_INIT_RESET == ulResult)
            {
                /* ���½����ʼ�� */
                if (VOS_OK != USIMM_PowerDownResetPrepare())
                {
                    return VOS_ERR;
                }

                /* ����USIMM_PowerDownResetPrepare��Ὣ�����͸ı䣬��Ҫ�Ļ��� */
                gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType = USIMM_CARD_USIM;

                j = 0;

                continue;
            }

            if (USIMM_INIT_FATAL_EXIT == ulResult)
            {
                return VOS_ERR;
            }
        }

        if(j == g_aCardInitTbl[i].ulStepNum)
        {
           break;
        }
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_InitIsimHandle(USIMM_MsgBlock *pMsg)
{
    VOS_UINT32                          ulResult;
    USIMM_USIMSUPIMS_STRU               stUsimSupIms;

    if(USIMM_CARD_USIM != gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
    {
        /*�ϱ��޿�*/
        gastUSIMMCardAppInfo[USIMM_UICC_ISIM].enCardType     = USIMM_CARD_ISIM;
        gastUSIMMCardAppInfo[USIMM_UICC_ISIM].enCardService  = USIMM_CARD_SERVIC_ABSENT;
    }
    else
    {
        if(USIMM_CARD_SERVIC_AVAILABLE == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService)
        {
            /* ͨ��NV�ж�usim�Ƿ�֧��IMS,Ĭ��֧�� */
            ulResult = NV_Read(en_NV_Item_Usim_Support_Ims, &stUsimSupIms, sizeof(USIMM_USIMSUPIMS_STRU));

            if((NV_OK == ulResult) && (PS_USIM_SERVICE_AVAILIABLE == stUsimSupIms.ulUsimSupImsEnable))/*��ȡʧ�ܰ���Ĭ���·�*/
            {
                /* �ϱ�ok ��ISIM��״̬������״̬ */
                gastUSIMMCardAppInfo[USIMM_UICC_ISIM].enCardType     = USIMM_CARD_ISIM;
                gastUSIMMCardAppInfo[USIMM_UICC_ISIM].enCardService  = USIMM_CARD_SERVIC_AVAILABLE;
                gulUsimSupportIMS                                    = VOS_TRUE;
            }
            else
            {
                /* �ϱ��޿� */
                gastUSIMMCardAppInfo[USIMM_UICC_ISIM].enCardType     = USIMM_CARD_ISIM;
                gastUSIMMCardAppInfo[USIMM_UICC_ISIM].enCardService  = USIMM_CARD_SERVIC_ABSENT;
            }
        }
        else
        {
            /* �ÿ�״̬ */
            gastUSIMMCardAppInfo[USIMM_UICC_ISIM].enCardType         = USIMM_CARD_ISIM;
            gastUSIMMCardAppInfo[USIMM_UICC_ISIM].enCardService      = gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService;
        }
    }

    gastUSIMMCardAppInfo[USIMM_UICC_ISIM].enStateChange = USIMM_CARD_STATE_CHANGED;

    return VOS_OK;
}


VOS_UINT32 USIMM_InitCard(USIMM_MsgBlock *pMsg)
{
    VOS_UINT32               ulResult;
    VOS_UINT32               ulRandNum;

    USIMM_InitGlobalVarOnReset();

    ulResult = USIMM_InitCardTypeByATR();   /*��ȡATR�жϿ�״̬*/

    if(VOS_OK != ulResult)     /*��ʼ����һ������*/
    {
        USIMM_ERROR_LOG("USIMM_InitCard: USIMM_InitCardFirstStep Error");   /*��ӡ����*/

        return VOS_ERR;
    }

    USIMM_NORMAL_LOG("USIMM_InitCard: USIMM_InitCardFirstStep is Finished");

    /* ��ʼ��������PIN�����Ĳ��� */
    ulResult = USIMM_InitNoNeedPinSteps();

    if(VOS_OK != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_InitCard: USIMM_InitNoNeedPinSteps Error");   /*��ӡ����*/

        return VOS_ERR;
    }

    /* ��ʼ��ATT�����ļ����䲻��PIN����� */
    USIMM_AttFileInit();

    /* �����߼�ͨ��Ϊ����ҵ��ר��,ͨ����Ϊ0 */
    ulRandNum = VOS_Rand(USIMM_SESSION_ID_MAX);

    /* ����SessionIdֵ����Ϊ0�����Ե������Ϊ0ʱ��Ҫ������ */
    if (0 == ulRandNum)
    {
        ulRandNum = USIMM_SESSION_ID_MAX;
    }

    g_astUSIMMChCtrl[0].ulSessionId   = USIMM_SESSION_ID(ulRandNum, 0);

    g_astUSIMMChCtrl[0].ulChanNum     = USIMM_BASE_CHANNEL_NUM;

    g_astUSIMMChCtrl[0].ulAIDLen      = gstUSIMMADFInfo.ulAIDLen;

    if (sizeof(g_astUSIMMChCtrl[0].aucADFName) >= gstUSIMMADFInfo.ulAIDLen)
    {
        /* coverity[overrun-buffer-arg] */
        VOS_MemCpy(g_astUSIMMChCtrl[0].aucADFName, gstUSIMMADFInfo.aucAID, gstUSIMMADFInfo.ulAIDLen);
    }

    /* ��PIN��ʹ��ʱ����һ����ʼ���ļ� */
    if(((0 != gstUSIMMADFInfo.stPINInfo.ucPin1RemainTime)
        &&(USIMM_PIN_ENABLED !=  gstUSIMMADFInfo.stPINInfo.enPin1Enable)))
    {
        USIMM_InitNeedPinSteps(USIMM_NORMAL_MODE);

        /* ��ʼ����ѡ�ļ� */
        if(VOS_OK != USIMM_InitThirdSteps())
        {
            USIMM_ERROR_LOG("USIMM_InitCard:USIMM_InitThirdSteps is Failed");

            return VOS_ERR;
        }

    }

    /*yangzhi Usim Init End 0520 */
    ddmPhaseScoreBoot("end usim init",__LINE__);
    
    return VOS_OK;
}


VOS_VOID USIMM_ChangePathToU16(VOS_UINT32 ulDataLen, VOS_UINT8 *pucData, VOS_UINT32 *pulLen, VOS_UINT16 *pusPath)
{
    VOS_UINT32 i;
    VOS_UINT32 j = 0;

    for(i=0; i<ulDataLen; i+=2, j++)
    {
        pusPath[j] = ((pucData[i]<<0x08)&0xFF00)+pucData[i+1];
    }

    *pulLen = j;

    return;
}


VOS_UINT32 USIMM_SearchFileInList(VOS_UINT16 usFileId, VOS_UINT16 usFileNum, VOS_UINT16 *pusFileList, VOS_UINT16 *pusOldList)
{
    VOS_UINT32 i;

    if(pusFileList == VOS_NULL_PTR)
    {
        USIMM_INFO_LOG("USIMM_SearchFileInList: The Input Para Error.");

        return VOS_ERR;
    }

    if(pusFileList == pusOldList)
    {
        return VOS_ERR;
    }

    for(i=0; i<usFileNum; i++)
    {
        if(usFileId == pusFileList[i])
        {
            USIMM_INFO_LOG("USIMM_SearchFileInList: Find the EF In List.");

            return VOS_OK;
        }
    }

    USIMM_ERROR_LOG("USIMM_SearchFileInList: Could not Find the EF In List.");

    return VOS_ERR;
}

/*****************************************************************************
�� �� ��  : USIMM_SelectUiccADF
��������  : ѡ��ADF
�������  : ��
�������  : ��
�� �� ֵ  :
���ú���  : ��
��������  : �ⲿ�ӿ�
History     :
1.��    ��  : 2009��6��26��
  ��    ��  :
  �޸�����  : Create
*****************************************************************************/
VOS_UINT32 USIMM_SelectUiccADF(VOS_VOID)
{
    VOS_UINT32                          ulResult;
    USIMM_APDU_ST                       stApduInfo = {0};

    stApduInfo.aucAPDU[P1] = USIMM_SELECT_BY_DF_NAME;
    stApduInfo.aucAPDU[P2] = USIMM_SELECT_RETURN_FCP_TEMPLATE;
    stApduInfo.aucAPDU[P3] = (VOS_UINT8)gstUSIMMADFInfo.ulAIDLen;

    VOS_MemCpy(stApduInfo.aucSendBuf, gstUSIMMADFInfo.aucAID, (VOS_UINT8)gstUSIMMADFInfo.ulAIDLen);

    ulResult = USIMM_ReselectFileAPDU(&stApduInfo);

    if(VOS_OK == ulResult) /*�ж�AIDѡ����*/
    {
        USIMM_ClearCurEFInfo(&gstUSIMMCurFileInfo);/*ѡ��DF�ļ����ϴ�ѡ�е�EF�ļ���Ϊ��Чֵ*/

        ulResult = USIMM_DecodeDFFcp(USIMM_CARD_USIM, &stApduInfo, &gstUSIMMCurFileInfo);
    }
    else
    {
        USIMM_ERROR_LOG("USIMM_SelectUiccADF: Select File Error");/*��ӡ����*/

        gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService = USIMM_CARD_SERVIC_UNAVAILABLE;          /* ����ǰ���񲻿��� */

        ulResult = VOS_ERR;
    }

    if(VOS_OK == ulResult)
    {
        gstUSIMMCurFileInfo.usCurDFID = ADF;

        USIMM_SaveDFPinInfo();
    }

    /* ATT���ƹ��̣�ʧ����Ҳ������������ʼ�� */
    /* ������USIMʱѡ��AID��ȷ������ѡ��6FD2�ļ� */
    USIMM_AttUpdataSupportTable();

    return ulResult;
}


VOS_UINT32 USIMM_SelectDFFile(VOS_UINT16                        usFileId,
                                    USIMM_SELECT_PARA_STUR           *pstSelectPara,
                                    USIMM_CURFILEINFO_ST             *pstCurFileInfo)
{
    VOS_UINT8                           aucDFid[2];
    VOS_UINT32                          ulResult;
    USIMM_APDU_ST                       stApduInfo = {0};

    aucDFid[0] = (VOS_INT8)((usFileId>>8)&0xFF);
    aucDFid[1] = (VOS_UINT8)(usFileId&0xFF);

    if(USIMM_CARD_SIM == pstSelectPara->enCardType)    /*SIM��*/
    {
        stApduInfo.aucAPDU[CLA] = USIMM_SIM_CLA;
        stApduInfo.aucAPDU[P2]  = USIMM_SIM_SELECT_P2_PARA;
    }
    else
    {
        stApduInfo.aucAPDU[CLA] = USIMM_USIM_CLA;
        stApduInfo.aucAPDU[P2] = USIMM_SELECT_RETURN_FCP_TEMPLATE;
    }

    stApduInfo.aucAPDU[P1] = USIMM_SELECT_BY_FILE_ID;

    stApduInfo.aucAPDU[P3] = sizeof(aucDFid);

    VOS_MemCpy(stApduInfo.aucSendBuf, aucDFid, sizeof(aucDFid));

    ulResult = USIMM_ReselectFileAPDU(&stApduInfo);/*ѡ���ļ�*/

    /*�����ȡ��ԭʼ���ݵ�ȫ�ֱ���*/
    VOS_MemCpy(&gstUSIMMAPDU, &stApduInfo, sizeof(stApduInfo));

    /*�жϼ����*/
    if(USIMM_SW_OK != ulResult)
    {
        USIMM_WARNING_LOG("USIMM_SelectDFFile:USIMM_ReselectFileAPDU error");

        return ulResult;
    }

    /*DF ������FCP��������ȡ������*/
    ulResult = USIMM_DecodeDFFcp(pstSelectPara->enCardType, &stApduInfo, pstCurFileInfo);

    if(VOS_OK != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_SelectDFFile:Decode Fcp error");

        return USIMM_SW_ERR;
    }

    return USIMM_SW_OK;
}
VOS_UINT32 USIMM_SelectEFByID(VOS_UINT16                     usFileId,
                                    USIMM_SELECT_PARA_STUR           *pstSelectPara,
                                    USIMM_CURFILEINFO_ST             *pstCurFileInfo)
{
    VOS_UINT8                           aucEfId[2];
    VOS_UINT32                          ulResult;
    USIMM_APDU_ST                       stApduInfo = {0};

    aucEfId[0] = (VOS_UINT8)((usFileId>>8)&0xFF);

    aucEfId[1] = (VOS_UINT8)(usFileId&0xFF);

    stApduInfo.aucAPDU[P1] = USIMM_SELECT_BY_FILE_ID;

    if(USIMM_CARD_SIM == pstSelectPara->enCardType)
    {
        stApduInfo.aucAPDU[CLA] = USIMM_SIM_CLA;
        stApduInfo.aucAPDU[P2]  = USIMM_SIM_SELECT_P2_PARA;
    }
    else
    {
        stApduInfo.aucAPDU[CLA] = USIMM_USIM_CLA;

        if (USIMM_NEED_FCP == pstSelectPara->enEfFcpFlag)
        {
            stApduInfo.aucAPDU[P2] = USIMM_SELECT_RETURN_FCP_TEMPLATE;
        }
        else
        {
            stApduInfo.aucAPDU[P2] = USIMM_SELECT_NO_DATA_RETURNED;
        }
    }

    stApduInfo.aucAPDU[P3] = sizeof(aucEfId);

    VOS_MemCpy(stApduInfo.aucSendBuf, aucEfId, sizeof(aucEfId));

    ulResult = USIMM_ReselectFileAPDU(&stApduInfo);  /*ѡ���ļ�*/

    VOS_MemCpy(&gstUSIMMAPDU, &stApduInfo, sizeof(stApduInfo));

    if(USIMM_SW_OK != ulResult)/*�жϼ����*/
    {
        USIMM_WARNING_LOG("USIMM_SelectEFByID:USIMM_ReselectFileAPDU error");

        return ulResult;
    }

    if (VOS_NULL != stApduInfo.ulRecDataLen)
    {
        ulResult = USIMM_DecodeEFFcp(pstSelectPara->enCardType, &stApduInfo, pstCurFileInfo);

        if(VOS_OK != ulResult)
        {
            USIMM_ERROR_LOG("USIMM_SelectEFByID:Decode FCP error");

            return USIMM_SW_ERR;
        }
    }

    return USIMM_SW_OK;
}


VOS_UINT32 USIMM_SelectFileNoPath(
    USIMM_SELECT_PARA_STUR             *pstSelectPara,
    USIMM_U16_LVDATA_STRU              *pstFilePath,
    USIMM_CURFILEINFO_ST               *pstCurFileInfo,
    VOS_UINT16                         *pusCurPathLen)
{
    VOS_UINT32 i;
    VOS_UINT32 ulResult = USIMM_SW_ERR;

    /*lint -e830 -e539*/
    for (i = 0; i < pstFilePath->ulDataLen; i++)
    {
        if (USIMM_IS_DF_FILE(pstFilePath->pusData[i]))
        {
            ulResult = USIMM_SelectDFFile(pstFilePath->pusData[i], pstSelectPara, pstCurFileInfo);
        }
        else
        {
            ulResult = USIMM_SelectEFByID(pstFilePath->pusData[i], pstSelectPara, pstCurFileInfo);
        }

        if ((USIMM_SW_OK != ulResult)&&(USIMM_SW_OK_WITH_SAT != ulResult))
        {
            USIMM_ERROR_LOG("USIMM_SelectFileNoPath: Select File Error.");

            break;
        }
    }

    /* �����Ƿ�ѡ�е�ǰ�ļ�·������Ҫ���� */
    if (VOS_NULL_PTR != pusCurPathLen)
    {
        *pusCurPathLen = (VOS_UINT16)i;
    }

    return ulResult;
}
VOS_UINT32 USIMM_GetDiffPos(USIMM_U16_LVDATA_STRU *pstFilePath,
                                  USIMM_CURFILEINFO_ST  *pstCurFileInfo)
{
    VOS_UINT32                          ulMinLen;
    VOS_UINT32                          ulDiffPos;

    if (pstFilePath->ulDataLen < pstCurFileInfo->usFilePathLen)
    {
        ulMinLen = pstFilePath->ulDataLen;
    }
    else
    {
        ulMinLen = pstCurFileInfo->usFilePathLen;
    }

    /* �Ƚ�ѡ��·���͵�ǰ·�� */
    for (ulDiffPos = 0; ulDiffPos < ulMinLen; ulDiffPos++)
    {
        if (pstCurFileInfo->ausFilePath[ulDiffPos] != pstFilePath->pusData[ulDiffPos])
        {
            break;
        }
    }

    return ulDiffPos;
}
VOS_UINT32 USIMM_GetCurDfPos(USIMM_CURFILEINFO_ST *pstCurFileInfo)
{
    VOS_UINT32                              ulCurDFPos;

    /* ��õ�ǰ·������λ�� */
    if (USIMM_NULL_ID == pstCurFileInfo->usCurEFID)
    {
        ulCurDFPos = pstCurFileInfo->usFilePathLen - 1;
    }
    else
    {
        ulCurDFPos = pstCurFileInfo->usFilePathLen - 2;
    }

    return ulCurDFPos;
}
VOS_UINT32 USIMM_SelectFileByRelativePath(
    USIMM_SELECT_PARA_STUR             *pstSelectPara,
    USIMM_U16_LVDATA_STRU              *pstFilePath,
    USIMM_CURFILEINFO_ST               *pstCurFileInfo)
{
    VOS_UINT32                          ulRelativeLen = 0;
    VOS_UINT16                          ausRelativePath[USIMM_MAX_PATH_LEN] = {0};
    USIMM_U16_LVDATA_STRU               stRelativePath = {0};
    VOS_UINT32                          ulDiffPos ;
    VOS_UINT32                          ulCurDFPos;

    /* �ϴ��ļ�û��ѡ�У������·�����Ǿ���·�� */
    if (0 == pstCurFileInfo->usFilePathLen)
    {
        return USIMM_SW_ERR;
    }

    ulDiffPos  = USIMM_GetDiffPos(pstFilePath, pstCurFileInfo);

    ulCurDFPos = USIMM_GetCurDfPos(pstCurFileInfo);

    if (ulCurDFPos == (ulDiffPos - 1))
    {
        /* ��Ҫѡ�е�ǰĿ¼ */
        if ((USIMM_NULL_ID != pstCurFileInfo->usCurEFID)
            && (ulDiffPos == pstFilePath->ulDataLen))
        {
            VOS_MemCpy(ausRelativePath, &pstFilePath->pusData[ulDiffPos - 1], sizeof(VOS_UINT16));

            ulRelativeLen = 1;
        }
        else /* ��ǰĿ¼�������ӽڵ��ļ���ѡ */
        {
            VOS_MemCpy(ausRelativePath, &pstFilePath->pusData[ulDiffPos], (pstFilePath->ulDataLen- ulDiffPos)*sizeof(VOS_UINT16));

            ulRelativeLen =  pstFilePath->ulDataLen - ulDiffPos;
        }
    }

    if (ulCurDFPos == ulDiffPos)
    {
        /* ��ǰĿ¼���ڵ��µ�DF�ļ���ѡ */
        if (ulDiffPos < pstFilePath->ulDataLen)
        {
            if (USIMM_IS_DF_FILE(pstFilePath->pusData[ulDiffPos])
                && (ADF != pstCurFileInfo->ausFilePath[ulCurDFPos]))
            {
                VOS_MemCpy(ausRelativePath, &pstFilePath->pusData[ulDiffPos], (pstFilePath->ulDataLen - ulDiffPos)*sizeof(VOS_UINT16));

                ulRelativeLen = pstFilePath->ulDataLen - ulDiffPos;
            }
        }
        else
        {
            /* ��ǰĿ¼�ĸ��ڵ��ѡ */
            VOS_MemCpy(ausRelativePath, &pstFilePath->pusData[ulDiffPos - 1], sizeof(VOS_UINT16));

            ulRelativeLen = 1;
        }
    }

    /* ����ѡ���·���뵱ǰ·����ͬ����ѡ�������ļ� */
    if ((ulDiffPos == pstCurFileInfo->usFilePathLen) && (ulDiffPos == pstFilePath->ulDataLen))
    {
        VOS_MemCpy(ausRelativePath, &pstFilePath->pusData[ulDiffPos - 1], sizeof(VOS_UINT16));

        ulRelativeLen = 1;
    }

    /* ���ܻ�����·�� */
    if (0 == ulRelativeLen)
    {
        USIMM_WARNING_LOG("USIMM_SelectFileByRelativePath: Can't get relative path.");

        return USIMM_SW_ERR;
    }

    stRelativePath.ulDataLen = ulRelativeLen;
    stRelativePath.pusData   = ausRelativePath;

    return USIMM_SelectFileNoPath(pstSelectPara, &stRelativePath, pstCurFileInfo, VOS_NULL_PTR);
}
VOS_UINT32 USIMM_SelectFileByPath(USIMM_SELECT_PARA_STUR         *pstSelectPara,
                                        USIMM_U16_LVDATA_STRU           *pstFilePath,
                                         USIMM_CURFILEINFO_ST           *pstCurFileInfo)
{
    VOS_UINT32                          ulResult;
    VOS_UINT8                           ucPathLen;
    VOS_UINT8                           aucPath[USIMM_MAX_PATH_LEN*2] = {0};
    VOS_UINT32                          i;
    USIMM_APDU_ST                       stApduInfo = {0};

    /* ·�����ٰ���MF��2Fxx */
    if ((pstFilePath->ulDataLen < 2)||(VOS_NULL_PTR == pstFilePath->pusData))
    {
        return USIMM_SW_ERR;
    }

    for (i = 0; i < (pstFilePath->ulDataLen-1); i++)    /*����ǰ���MF*/
    {
        aucPath[i*2] = (VOS_UINT8)((pstFilePath->pusData[i+1]>>0x08)&0x00FF);

        aucPath[(i*2)+1] = (VOS_UINT8)(pstFilePath->pusData[i+1]&0x00FF);
    }

    ucPathLen = (VOS_UINT8)((pstFilePath->ulDataLen-1)*sizeof(VOS_UINT16));

    stApduInfo.aucAPDU[P1] = USIMM_SELECT_BY_PATH_FROM_MF;

    if (USIMM_IS_DF_FILE(pstFilePath->pusData[pstFilePath->ulDataLen-1])
        || (USIMM_NEED_FCP == pstSelectPara->enEfFcpFlag))
    {
        stApduInfo.aucAPDU[P2] = USIMM_SELECT_RETURN_FCP_TEMPLATE;
    }
    else
    {
        stApduInfo.aucAPDU[P2] = USIMM_SELECT_NO_DATA_RETURNED;
    }

    stApduInfo.aucAPDU[P3] = ucPathLen;

    VOS_MemCpy(stApduInfo.aucSendBuf, aucPath, ucPathLen);

    ulResult = USIMM_ReselectFileAPDU(&stApduInfo);

    VOS_MemCpy(&gstUSIMMAPDU, &stApduInfo, sizeof(stApduInfo));

    if (USIMM_SW_OK != ulResult)
    {
        USIMM_WARNING_LOG("USIMM_SelectEFByPath: USIMM_ReselectFileAPDU Error");

        return ulResult;
    }

    if (VOS_NULL != stApduInfo.ulRecDataLen)
    {
        ulResult = USIMM_DecodeFileFcp(pstSelectPara->enCardType, pstFilePath, &stApduInfo, pstCurFileInfo);

        if(VOS_OK != ulResult)  /*ѡ�����ļ���ʱ����Ҫ����·��*/
        {
            USIMM_WARNING_LOG("USIMM_SelectEFByPath: Decode Fcp Error");

            return USIMM_SW_ERR;
        }
    }
    else
    {
        USIMM_ClearCurDFInfo(pstCurFileInfo);

        USIMM_ClearCurEFInfo(pstCurFileInfo);
    }

    return USIMM_SW_OK;
}


VOS_UINT32 USIMM_GetPBFilePath(VOS_UINT16 usFileId, VOS_UINT32 *pulPathLen, VOS_UINT16 *pusPath)
{
    VOS_UINT8   ucFileType;

    if(gstUSIMMBaseInfo.stPBInfo.usPBDfID == 0)
    {
        USIMM_WARNING_LOG("USIMM_GetPBFilePath: usPBDfID is NULL.");

        return USIMM_SW_NOFILE_FOUND;
    }

    ucFileType = (VOS_UINT8)((usFileId >> 0x08)&0x00FF);

    if(ucFileType == EFUNDERDF)
    {
        if(gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType == USIMM_CARD_USIM)
        {
            pusPath[0] = ADF;
        }
        else if(gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType == USIMM_CARD_SIM)
        {
            pusPath[0] = DFTELCOM;
        }
        else
        {
            return USIMM_SW_NOFILE_FOUND;
        }

        *pulPathLen = 2;
        pusPath[1] = usFileId;

        return VOS_OK;
    }
    else if(ucFileType == EFUNDERGRANDADF)
    {
        *pulPathLen = 3;
        pusPath[0] = gstUSIMMBaseInfo.stPBInfo.usPBDfID;
        pusPath[1]= DFPHONEBOOK;
        pusPath[2] = usFileId;

        return VOS_OK;
    }
    else
    {
        USIMM_WARNING_LOG("USIMM_GetPBFilePath: File Type is Wrong.");

        return USIMM_SW_NOFILE_FOUND;
    }
}

/********************************************************************
  Function:     USIMM_GetAttFilePath
  Description:  AT&T�����ļ�·����ȡ����
  Input:        VOS_UINT16 usFileId���ļ�ID
  Output:       VOS_UINT32 *pulPathLen�������·������
                VOS_UINT16 *pusPath��������ļ�·��
  Return:       VOID
  Others:
********************************************************************/
VOS_UINT32 USIMM_GetAttFilePath(VOS_UINT16 usFileId, VOS_UINT32 *pulPathLen, VOS_UINT16 *pusPath)
{
    VOS_UINT8                           ucFileType;

    ucFileType = (VOS_UINT8)((usFileId >> 0x08)&0x00FF);

    if (EFUNDERATT == ucFileType)
    {
        if (USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
        {
            *pulPathLen =   3;
            pusPath[0]  =   ADF;
            pusPath[1]  =   DFATT;
            pusPath[2]  =   usFileId;
        }
        else
        {
            *pulPathLen =   2;
            pusPath[0]  =   DFATT;
            pusPath[1]  =   usFileId;
        }
    }
    else if (EFUNDERATTANDDF == ucFileType)
    {
        if (USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
        {
            *pulPathLen =   4;
            pusPath[0]  =   ADF;
            pusPath[1]  =   DFATT;
            pusPath[2]  =   DFUNDERDFATT;
            pusPath[3]  =   usFileId;
        }
        else
        {
            *pulPathLen =   3;
            pusPath[0]  =   DFATT;
            pusPath[1]  =   DFUNDERDFATT;
            pusPath[2]  =   usFileId;
        }
    }
    else
    {
        return USIMM_SW_NOFILE_FOUND;
    }

    return VOS_OK;
}


VOS_UINT32 USIMM_GetCardFilePath(USIMM_APP_TYPE_ENUM_UINT32 enFileApp,
                                        VOS_UINT16                  usFileId,
                                        VOS_UINT32                  *pulPathLen,
                                        VOS_UINT16                  *pusPath)
{
    VOS_UINT32  ulResult    = USIMM_SW_NOFILE_FOUND;
    VOS_UINT16  usDFID      = 0;
    VOS_UINT32  ulPathLen   = 0;
    VOS_UINT8   ucFileType;
    VOS_UINT32  i;
    VOS_UINT16  *pusFileList = VOS_NULL_PTR;

    ucFileType = USIMM_FILE_TYPE(usFileId);

    if (MFTAG == ucFileType)
    {
        *pusPath = usFileId;
        *pulPathLen = 1;
        return VOS_OK;
    }

    switch(enFileApp)
    {
        case USIMM_GSM_APP:
            usDFID = DFGSM;
            break;

        case USIMM_UMTS_APP:
            usDFID = ADF;
            break;

        case USIMM_PB_APP:
            return USIMM_GetPBFilePath(usFileId, pulPathLen, pusPath);

        case USIMM_ATT_APP:
            return USIMM_GetAttFilePath(usFileId, pulPathLen, pusPath);

        default:
            break;
    }

    for(i=0; i<ARRAYSIZE(g_stUSIMMCardFileMap); i++)
    {
        if(usDFID != 0)
        {
            if(usDFID != g_stUSIMMCardFileMap[i].stDFFileMap.stFileCont.usDfID)
            {
                continue;
            }
        }

        switch(ucFileType)
        {
            case DFUNDERMF:
                if(usFileId == g_stUSIMMCardFileMap[i].stDFFileMap.stFileCont.usDfID)
                {
                    ulResult = VOS_OK;
                }
                break;

            case DFUNDERDF:
                if(usFileId == g_stUSIMMCardFileMap[i].stGDFFileMap.stFileCont.usDfID)
                {
                    ulResult = VOS_OK;
                }
                break;

            case EFUNDERDF:
                ulResult = USIMM_SearchFileInList(usFileId, g_stUSIMMCardFileMap[i].stDFFileMap.stFileCont.usEFNum,
                                                        g_stUSIMMCardFileMap[i].stDFFileMap.stFileCont.pusEFList,
                                                        pusFileList);
                pusFileList = g_stUSIMMCardFileMap[i].stDFFileMap.stFileCont.pusEFList;
                break;

            case EFUNDERGRANDADF:
                ulResult = USIMM_SearchFileInList(usFileId, g_stUSIMMCardFileMap[i].stGDFFileMap.stFileCont.usEFNum,
                                                        g_stUSIMMCardFileMap[i].stGDFFileMap.stFileCont.pusEFList,
                                                        pusFileList);
                pusFileList = g_stUSIMMCardFileMap[i].stDFFileMap.stFileCont.pusEFList;
                break;

            default:
                USIMM_ERROR_LOG("USIMM_GetCardFilePath: The File Type is Error");
                return USIMM_SW_NOFILE_FOUND;
        }

        if(ulResult == VOS_OK)
        {
            break;
        }
    }

    if(i >= ARRAYSIZE(g_stUSIMMCardFileMap))
    {
        USIMM_ERROR_LOG("USIMM_GetCardFilePath:Could Not Found the Card Path");

        return USIMM_SW_NOFILE_FOUND;
    }

    pusPath[0] = MF;
    pusPath[1] = g_stUSIMMCardFileMap[i].stDFFileMap.stFileCont.usDfID;

    ulPathLen = 2;

    if((ucFileType == EFUNDERGRANDADF)||(ucFileType == DFUNDERDF))
    {
        pusPath[2] = g_stUSIMMCardFileMap[i].stGDFFileMap.stFileCont.usDfID;
        ulPathLen ++;
    }

    if((ucFileType == EFUNDERDF)||(ucFileType == EFUNDERGRANDADF))
    {
        pusPath[ulPathLen] = usFileId;
        ulPathLen ++;
    }

    *pulPathLen = ulPathLen;

    return VOS_OK;
}


VOS_UINT32 USIMM_SelectFileWithPath(USIMM_SELECT_PARA_STUR          *pstSelectPara,
                                            USIMM_U16_LVDATA_STRU          *pstFilePath,
                                            USIMM_CURFILEINFO_ST           *pstCurFileInfo,
                                            VOS_UINT16                     *pusCurPathLen)
{
    VOS_UINT32                          ulResult;

    *pusCurPathLen = (VOS_UINT16)pstFilePath->ulDataLen;

    /* ���ȴ���ֻѡMF�ļ� */
    if ((1 == pstFilePath->ulDataLen) && (MFTAG == USIMM_FILE_TYPE(pstFilePath->pusData[0])))
    {
        ulResult = USIMM_SelectDFFile(pstFilePath->pusData[0], pstSelectPara, pstCurFileInfo);

        if (USIMM_SW_OK != ulResult)
        {
            *pusCurPathLen = 0;
        }

        return ulResult;
    }

    /* ��USIM����ͨ��·��ѡ��ʽ�����ļ�ѡ�� */
    if (USIMM_CARD_USIM == pstSelectPara->enCardType)
    {
        ulResult = USIMM_SelectFileByPath(pstSelectPara, pstFilePath, pstCurFileInfo);
    }
    /* ��SIM����ͨ�����·����ʽ���е���ѡ�� */
    else
    {
        ulResult = USIMM_SelectFileByRelativePath(pstSelectPara, pstFilePath, pstCurFileInfo);
    }

    if (USIMM_SW_OK != ulResult)
    {
        /* �����þ���·����ʽ���е���ѡ�� */
        ulResult = USIMM_SelectFileNoPath(pstSelectPara, pstFilePath, pstCurFileInfo, pusCurPathLen);
    }

    if (USIMM_SW_OK != ulResult)
    {
        USIMM_WARNING_LOG("USIMM_SelectFileWithPath: Select File Failed");
    }

    return ulResult;
}


VOS_UINT32 USIMM_GetRefreshEFPath(VOS_UINT8 *pucFileList,
                                       VOS_UINT32 ulFileLen,
                                       VOS_UINT8 *pucEFPath,
                                       VOS_UINT32 *pulPathLen)
{
    VOS_UINT32 ulFileCnt;

    for(ulFileCnt = 0; ((ulFileCnt+1) < ulFileLen); ulFileCnt += sizeof(VOS_UINT16))
    {
        if((ulFileCnt+1) >= (USIMM_MAX_PATH_LEN*2))
        {
            break;
        }

        pucEFPath[ulFileCnt] = pucFileList[ulFileCnt];
        pucEFPath[ulFileCnt+1] = pucFileList[ulFileCnt+1];

        if((EFUNDERMF != pucFileList[ulFileCnt])
            &&(EFUNDERDF != pucFileList[ulFileCnt])
            &&(EFUNDERGRANDADF != pucFileList[ulFileCnt]))/*�жϲ�ΪEF�ļ�*/
        {
            continue;
        }

        *pulPathLen = ulFileCnt+2;

        return VOS_OK;
    }

    return VOS_ERR;
}
VOS_VOID USIMM_RefreshNoFileIndHandle(USIMM_REFRESH_REQ_STRU *pstMsg)
{
    PS_USIM_REFRESH_IND_STRU            *pstRefreshMsg;

    pstRefreshMsg = (PS_USIM_REFRESH_IND_STRU*)VOS_AllocMsg(WUEPS_PID_USIM, sizeof(PS_USIM_REFRESH_IND_STRU)-VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstRefreshMsg)
    {
        USIMM_WARNING_LOG("USIMM_RefreshNoFileIndHandle: Alloc Msg is Fail");

        return;
    }

    pstRefreshMsg->ulReceiverPid = MAPS_PIH_PID;        /*��PIH����㲥*/
    pstRefreshMsg->ulMsgName     = PS_USIM_REFRESH_IND;
    pstRefreshMsg->usEfNum       = VOS_NULL;

    if (USIMM_SESSION_RESET_3G_ONLY == pstMsg->enRefreshType)
    {
        pstRefreshMsg->enRefreshType = USIMM_REFRESH_3G_SESSION_RESET;
    }
    else
    {
        pstRefreshMsg->enRefreshType = USIMM_REFRESH_ALL_FILE;
    }

    (VOS_VOID)VOS_SendMsg(WUEPS_PID_USIM, pstRefreshMsg);

    return;
}
VOS_VOID USIMM_RefreshIsimFileIndHandle(PS_USIM_REFRESH_IND_STRU *pstRefreshMsg)
{
    VOS_UINT16                          i;
    VOS_UINT16                          usFileNum = 0;
    VOS_UINT16                          ausEfId[USIMM_MAX_REFRESH_FILE_NUM] = {0};
    PS_USIM_REFRESH_IND_STRU           *pstIsimRefreshMsg;

    /* �жϵ�ǰ�Ƿ�֧��ISIM�� */
    if (USIMM_CARD_ISIM != gastUSIMMCardAppInfo[USIMM_UICC_ISIM].enCardType)
    {
        USIMM_INFO_LOG("USIMM_RefreshIsimFileIndHandle: IMSI is disable.");

        return;
    }

    for (i = 0; i < pstRefreshMsg->usEfNum; i++)
    {
        if (USIMM_UMTS_APP == pstRefreshMsg->astEfId[i].enAppType)
        {
            /* IMSI��Ӧ3��ISIM�ļ� */
            if (EFIMSI == pstRefreshMsg->astEfId[i].usFileId)
            {
                ausEfId[usFileNum]      = EFISIMIMPI;
                ausEfId[usFileNum + 1]  = EFISIMDOMAIN;
                ausEfId[usFileNum + 2]  = EFISIMIMPU;
                usFileNum += 3;

                continue;
            }

            /* �ж��Ƿ�ΪISIM��Ӧ�ļ� */
            ausEfId[usFileNum] = USIMM_UsimEFIDToIsimEFID(pstRefreshMsg->astEfId[i].usFileId);

            if (VOS_NULL_WORD != ausEfId[usFileNum])
            {
                usFileNum++;
            }
        }
    }

    /* û���漰ISIM�ļ�����֪ͨ */
    if (0 == usFileNum)
    {
        return;
    }

    /* ����PS_USIM_REFRESH_IND_STRU���Ѿ���һ��PS_USIM_REFRESH_FILE_STRU��Ԫ����usFileNum��Ҫ��1 */
    pstIsimRefreshMsg = (PS_USIM_REFRESH_IND_STRU *)VOS_AllocMsg(WUEPS_PID_USIM,
                                                     (sizeof(PS_USIM_REFRESH_IND_STRU)-VOS_MSG_HEAD_LENGTH)\
                                                     + ((usFileNum - 1) * sizeof(PS_USIM_REFRESH_FILE_STRU)));

    if (VOS_NULL_PTR == pstIsimRefreshMsg)
    {
        USIMM_ERROR_LOG("USIMM_RefreshIsimFileIndHandle: VOS_AllocMsg is Failed");

        return;
    }

    pstIsimRefreshMsg->ulReceiverPid    =   MAPS_PIH_PID;
    pstIsimRefreshMsg->ulMsgName        =   PS_USIM_REFRESH_IND;
    pstIsimRefreshMsg->enRefreshType    =   USIMM_REFRESH_FILE_LIST;
    pstIsimRefreshMsg->usEfNum          =   usFileNum;

    for (i = 0; i < usFileNum; i++)
    {
        pstIsimRefreshMsg->astEfId[i].enAppType = USIMM_ISIM_APP;
        pstIsimRefreshMsg->astEfId[i].usFileId  = ausEfId[i];
    }

    (VOS_VOID)VOS_SendMsg(WUEPS_PID_USIM, pstIsimRefreshMsg);

    return;
}
VOS_VOID USIMM_RefreshFileIndHandle(USIMM_REFRESH_REQ_STRU *pstMsg)
{
    VOS_UINT32                          ulOffset;
    VOS_UINT32                          ulTempLen;
    VOS_UINT32                          ulPathLen                               = 0;
    VOS_UINT8                           aucRefreshFile[USIMM_MAX_PATH_LEN*2]    = {0};/*��ǰ��һ�ļ�·����8����*/
    VOS_UINT16                          ausPath[USIMM_MAX_PATH_LEN]             = {0};/*��ǰ�ļ�·����16����*/
    VOS_UINT16                          usEfNum                                 = 0;
    VOS_UINT16                          ausEfId[USIMM_MAX_REFRESH_FILE_NUM]     = {0};
    VOS_UINT16                          i;
    USIMM_APP_TYPE_ENUM_UINT32          aenAppType[USIMM_MAX_REFRESH_FILE_NUM]  = {0};
    PS_USIM_REFRESH_IND_STRU           *pstRefreshMsg;

    for(ulOffset=0; ulOffset<pstMsg->usLen; ulOffset+=ulTempLen)
    {
        /*��ȡ��ǰEF��ȫ·������*/
        if(VOS_OK != USIMM_GetRefreshEFPath(&pstMsg->aucFileList[ulOffset],
                                            pstMsg->usLen-ulOffset,
                                            aucRefreshFile,
                                            &ulTempLen))
        {
            USIMM_WARNING_LOG("USIMM_RefreshHandle: Fail to Get File");

            break;
        }

        USIMM_ChangePathToU16(ulTempLen,aucRefreshFile,&ulPathLen, ausPath);

        ausEfId[usEfNum] = ausPath[ulPathLen - 1];

        USIMM_RefreshFileAppCheck(&aenAppType[usEfNum], ausPath, ulPathLen);

        usEfNum++;

        /* �����������ļ����������ֵʱ����ѭ�� */
        if (USIMM_MAX_REFRESH_FILE_NUM <= usEfNum)
        {
            USIMM_WARNING_LOG("USIMM_RefreshHandle: Refresh File too much");

            break;
        }
    }

    pstRefreshMsg = (PS_USIM_REFRESH_IND_STRU *)VOS_AllocMsg(WUEPS_PID_USIM,
                                                             (sizeof(PS_USIM_REFRESH_IND_STRU)-VOS_MSG_HEAD_LENGTH)\
                                                             + (usEfNum * sizeof(PS_USIM_REFRESH_FILE_STRU)));

    if (VOS_NULL_PTR == pstRefreshMsg)
    {
        USIMM_ERROR_LOG("USIMM_RefreshFileIndHandle: VOS_AllocMsg is Failed");

        return;
    }

    pstRefreshMsg->ulReceiverPid    =   MAPS_PIH_PID;
    pstRefreshMsg->ulMsgName        =   PS_USIM_REFRESH_IND;
    pstRefreshMsg->enRefreshType    =   USIMM_REFRESH_FILE_LIST;
    pstRefreshMsg->usEfNum          =   usEfNum;

    for (i = 0; i < usEfNum; i++)
    {
        pstRefreshMsg->astEfId[i].enAppType    =   aenAppType[i];
        pstRefreshMsg->astEfId[i].usFileId     =   ausEfId[i];
    }

    /* ����ISIM�ļ������ϱ� */
    USIMM_RefreshIsimFileIndHandle(pstRefreshMsg);

    (VOS_VOID)VOS_SendMsg(WUEPS_PID_USIM, pstRefreshMsg);

    return;
}
VOS_VOID USIMM_RefreshInd(USIMM_REFRESH_REQ_STRU *pstMsg)
{
    /* REFRESH������ָʾ�ļ����� */
    if(((USIMM_FILE_CHANGE_NOTIFY == pstMsg->enRefreshType)||(USIMM_INITIAL_FILE_CHANGE_NOTIFY == pstMsg->enRefreshType))
        &&(pstMsg->usLen != 0))
    {
        USIMM_RefreshFileIndHandle(pstMsg);
    }
    else
    {
        USIMM_RefreshNoFileIndHandle(pstMsg);
    }

    return;
}
VOS_VOID USIMM_RefreshCardStatusInd(VOS_VOID)
{
    if (VOS_TRUE == USIMM_IsSvlte())
    {
        USIMM_CardStatusInd(I0_WUEPS_PID_MMA, gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType, gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService);

        USIMM_CardStatusInd(I1_WUEPS_PID_MMA, gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType, gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService);
    }
    else
    {
        USIMM_CardStatusInd(WUEPS_PID_MMA, gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType, gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService);
    }
}


VOS_UINT32 USIMM_InitThirdSteps(VOS_VOID)
{
    NAS_NVIM_SYSTEM_APP_CONFIG_STRU stSysAppCfg;

    /* Ĭ�����öԽ�MP */
    stSysAppCfg.usSysAppConfigType = SYSTEM_APP_MP;

    if(NV_OK != NV_Read(en_NV_Item_System_APP_Config, &stSysAppCfg, sizeof(NAS_NVIM_SYSTEM_APP_CONFIG_STRU)))
    {
        USIMM_ERROR_LOG("USIMM_InitThirdSteps: Read en_NV_Item_System_APP_Config Failed");
    }

    if(SYSTEM_APP_ANDROID == stSysAppCfg.usSysAppConfigType)
    {
        return USIMM_InitCardOptionalFile(VOS_NULL_PTR);
    }

    return USIMM_SendInitCardMsg(WUEPS_PID_USIM, USIMM_CMDTYPE_INITTHIRD);
}
VOS_UINT32 USIMM_SimRefreshFilePbAppCheck(
    VOS_UINT16                         *pusFilePath,
    VOS_UINT32                          ulPathLen)
{
    if (pusFilePath[ulPathLen - 2] != DFTELCOM)
    {
        return VOS_FALSE;
    }

    if ((pusFilePath[ulPathLen - 1] == EFADN)
      ||(pusFilePath[ulPathLen - 1] == EFFDN)
      ||(pusFilePath[ulPathLen - 1] == EFBDN)
      ||(pusFilePath[ulPathLen - 1] == EFMSISDN))
    {
        return VOS_TRUE;
    }
    else
    {
        return VOS_FALSE;
    }
}



VOS_UINT32 USIMM_UsimRefreshFilePbAppCheck(
    VOS_UINT16                         *pusFilePath,
    VOS_UINT32                          ulPathLen)
{
    VOS_UINT16                          ausUsimDfPhoneBookPath[3]   = {MF, 0, DFPHONEBOOK};
    VOS_UINT16                          uacUsimAppPath[]            = {MF, ADF};

    ausUsimDfPhoneBookPath[1] = gstUSIMMBaseInfo.stPBInfo.usPBDfID;/*�ļ�ID�Ǹ���SIM�����ö���*/

    /*���ڵ绰��DF�µ������ļ�����������Ƿ��ǵ绰������ģ�����֪ͨ�绰��ģ��*/
    if (VOS_OK == VOS_MemCmp(ausUsimDfPhoneBookPath, pusFilePath, sizeof(ausUsimDfPhoneBookPath)))
    {
        return VOS_TRUE;
    }

    if (VOS_OK == VOS_MemCmp(uacUsimAppPath, pusFilePath, sizeof(uacUsimAppPath)))
    {
        if ((pusFilePath[ulPathLen - 1] == EFFDN)
          ||(pusFilePath[ulPathLen - 1] == EFBDN)
          ||(pusFilePath[ulPathLen - 1] == EFMSISDN))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}

/********************************************************************
  Function:     USIMM_RefreshFileAppCheck
  Description:  ����Refresh�����е��ļ�·��������������������
  Input:        VOS_UINT16 *pusFilePath:������ļ�·��
  Output:       USIMM_APP_TYPE_ENUM_UINT32 *penAppType�������Ӧ������
  Return:       ��
  Others:
********************************************************************/
VOS_VOID USIMM_RefreshFileAppCheck(
    USIMM_APP_TYPE_ENUM_UINT32         *penAppType,
    VOS_UINT16                         *pusFilePath,
    VOS_UINT32                          ulPathLen)
{
    VOS_UINT16                          ausAttSimPath[]         =   {MF, DFATT};
    VOS_UINT16                          ausAttUsimPath[]        =   {MF, ADF, DFATT};
    VOS_UINT16                          ausSimDfGsmPath[]       =   {MF, DFGSM};
    VOS_UINT16                          ausSimDfTelcomPath[]    =   {MF, DFTELCOM};

    if (USIMM_CARD_SIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
    {
        if (VOS_OK == VOS_MemCmp(ausAttSimPath, pusFilePath, sizeof(ausAttSimPath)))
        {
            *penAppType =   USIMM_ATT_APP;
        }
        else
        {
            if (VOS_TRUE == USIMM_SimRefreshFilePbAppCheck(pusFilePath, ulPathLen))
            {
                *penAppType =   USIMM_PB_APP;
            }
            else
            {
                *penAppType =   USIMM_GSM_APP;
            }
        }
    }
    else
    {
        if (VOS_TRUE == USIMM_UsimRefreshFilePbAppCheck(pusFilePath, ulPathLen))
        {
            *penAppType =   USIMM_PB_APP;
        }
        else if (VOS_OK == VOS_MemCmp(ausAttUsimPath, pusFilePath, sizeof(ausAttUsimPath)))
        {
            *penAppType =   USIMM_ATT_APP;
        }
        else if (VOS_OK == VOS_MemCmp(ausSimDfGsmPath, pusFilePath, sizeof(ausSimDfGsmPath)))
        {
            *penAppType =   USIMM_GSM_APP;
        }
        else if (VOS_OK == VOS_MemCmp(ausSimDfTelcomPath, pusFilePath, sizeof(ausSimDfTelcomPath)))
        {
            *penAppType =   USIMM_GSM_APP;
        }
        else
        {
            *penAppType =   USIMM_UMTS_APP;
        }
    }

    return;
}


VOS_VOID USIMM_RefreshFileRead(USIMM_REFRESH_REQ_STRU *pstMsg)
{
    VOS_UINT32                          ulOffset;
    VOS_UINT32                          ulTempLen = 0;
    VOS_UINT32                          ulPathLen = 0;
    VOS_UINT16                          ausPath[USIMM_MAX_PATH_LEN];/*��ǰ�ļ�·����16����*/
    VOS_UINT8                           aucRefreshFile[10];/*��ǰ��һ�ļ�·����8����*/
    USIMM_GETCNF_INFO_STRU              stCnfInfo;
    VOS_UINT32                          ulResult;
    USIMM_GET_COMM_FILE_STRU            stFileInfo ;

    VOS_MemSet(ausPath, 0, sizeof(ausPath));
    VOS_MemSet(aucRefreshFile, 0, sizeof(aucRefreshFile));
    VOS_MemSet(&stCnfInfo, 0, sizeof(stCnfInfo));
    VOS_MemSet(&stFileInfo, 0, sizeof(stFileInfo));

    for (ulOffset = 0; ulOffset < pstMsg->usLen; ulOffset += ulTempLen)
    {
        /*��ȡ��ǰEF��ȫ·������*/
        if (VOS_OK != USIMM_GetRefreshEFPath(&pstMsg->aucFileList[ulOffset],
                                             pstMsg->usLen-ulOffset,
                                             aucRefreshFile,
                                             &ulTempLen))
        {
            USIMM_WARNING_LOG("USIMM_RefreshFileRead: Fail to Get File");

            return;
        }

        USIMM_ChangePathToU16(ulTempLen,aucRefreshFile,&ulPathLen, ausPath);

        stFileInfo.enAppType    = USIMM_UNLIMIT_APP;
        stFileInfo.pusFilePath  = ausPath;
        stFileInfo.ulPathLen    = ulPathLen;
        stFileInfo.ucRecordNum  = USIMM_READ_ALLRECORD;

        ulResult = USIMM_GetFileFromCard(&stFileInfo, &stCnfInfo);

        if (VOS_OK != ulResult)
        {
            USIMM_WARNING_LOG("USIMM_RefreshFileRead: Fail to Get File");

            continue;
        }

        if (VOS_NULL_PTR != stCnfInfo.pucEf)
        {
            VOS_MemFree(WUEPS_PID_USIM, stCnfInfo.pucEf);

            stCnfInfo.pucEf = VOS_NULL_PTR;
        }
    }

    USIMM_ClearCurFileInfo();   /*���� MF ����ѡ��*/

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

