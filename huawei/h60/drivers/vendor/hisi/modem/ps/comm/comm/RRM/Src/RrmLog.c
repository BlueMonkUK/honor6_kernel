



/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "RrmLog.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_RRM_LOG_C

#if (FEATURE_ON == FEATURE_DSDS)

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
RRM_LOG_ENT_STRU                        g_stRrmLogEnt;     /* RRM��ӡʵ�� */


/* �����ź��� */
VOS_UINT32                            g_ulRrmInitLogMutexSem;
VOS_UINT32                            g_ulRrmLogSaveMutexSem;


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

VOS_VOID RRM_MNTN_InitLogEnt(RRM_LOG_ENT_STRU *pstLogEnt)
{
    if (VOS_OK != VOS_SmP(g_ulRrmInitLogMutexSem, 0))
    {
        return;
    }

    pstLogEnt->ulCnt = 0;

    VOS_SmV(g_ulRrmInitLogMutexSem);

    return;
}


VOS_VOID RRM_MNTN_LogSave
(
    RRM_LOG_ENT_STRU               *pstLogEnt,
    VOS_UINT32                      ulPid,
    LOG_LEVEL_EN                    enLevel,
    PS_FILE_ID_DEFINE_ENUM_UINT32   enFile,
    VOS_UINT32                      ulLine,
    VOS_INT32                       lpara1,
    VOS_INT32                       lpara2,
    VOS_INT32                       lpara3,
    VOS_INT32                       lpara4
)
{
    VOS_UINT32      ulCnt;
    VOS_UINT32      ulCurrentSlice;
    VOS_UINT32      ulTimeStamp;
    VOS_UINT32      ulTimeStampTmp;

    if (VOS_OK != VOS_SmP(g_ulRrmLogSaveMutexSem, 0))
    {
        return;
    }

    ulCnt           = pstLogEnt->ulCnt;

    /* Ϊ����SDT�����TimeStampһ�£�����SDT�Ļ��㷽��:(0xFFFFFFFF - OM_GetSlice())%32768*100����Slice���л��� */
    ulCurrentSlice  = OM_GetSlice();
    /* SliceΪ�ݼ�ʱ��ulCurrentSlice  = 0xFFFFFFFF - ulCurrentSlice������ʱ�����ü� */

    ulTimeStampTmp  = ulCurrentSlice&0x7FFF;
    ulTimeStampTmp  = ulTimeStampTmp *100;
    ulTimeStampTmp  = ulTimeStampTmp>>15;

    ulTimeStamp     = ulCurrentSlice>>15;
    ulTimeStamp     = ulTimeStamp*100;
    ulTimeStamp     = ulTimeStamp + ulTimeStampTmp;
    if( ulCnt < RRM_LOG_RECORD_MAX_NUM )
    {
        pstLogEnt->astData[ulCnt].ulTick        = ulTimeStamp;
        pstLogEnt->astData[ulCnt].enFile        = enFile;
        pstLogEnt->astData[ulCnt].ulLine        = ulLine;
        pstLogEnt->astData[ulCnt].enLevel       = enLevel;
        pstLogEnt->astData[ulCnt].alPara[0]     = lpara1;
        pstLogEnt->astData[ulCnt].alPara[1]     = lpara2;
        pstLogEnt->astData[ulCnt].alPara[2]     = lpara3;
        pstLogEnt->astData[ulCnt].alPara[3]     = lpara4;
        ulCnt++;
        pstLogEnt->ulCnt                        = ulCnt;
    }

    VOS_SmV(g_ulRrmLogSaveMutexSem);

    /* ��¼���ˣ��Զ����� */
    if (ulCnt >= RRM_LOG_RECORD_MAX_NUM)
    {
        RRM_MNTN_LogOutput(pstLogEnt, ulPid);
    }

    return;
}
VOS_VOID RRM_MNTN_LogOutput(RRM_LOG_ENT_STRU *pstLogEnt, VOS_UINT32 ulPid)
{
    RRM_TRACE_LOG_MSG_STRU          stRrmTraceLogMsg;


    /*  ����ݹ���ѭ�� */
    if ((pstLogEnt->ulCnt > RRM_LOG_RECORD_MAX_NUM) || (0 == pstLogEnt->ulCnt))
    {
        RRM_MNTN_InitLogEnt(pstLogEnt);
        return ;
    }

    stRrmTraceLogMsg.ulSenderCpuId      = VOS_LOCAL_CPUID;
    stRrmTraceLogMsg.ulSenderPid        = ulPid;
    stRrmTraceLogMsg.ulReceiverCpuId    = VOS_LOCAL_CPUID;
    stRrmTraceLogMsg.ulReceiverPid      = ulPid;
    stRrmTraceLogMsg.ulLength           = (sizeof(RRM_TRACE_LOG_MSG_STRU) - VOS_MSG_HEAD_LENGTH);
    stRrmTraceLogMsg.usMsgType          = ID_RRM_TRACE_LOG_MSG;
    stRrmTraceLogMsg.usTransId          = 0;

    DRV_RT_MEMCPY( &stRrmTraceLogMsg.stLogMsgCont, pstLogEnt, sizeof(RRM_LOG_ENT_STRU) );

    OM_TraceMsgHook(&stRrmTraceLogMsg);

    RRM_MNTN_InitLogEnt(pstLogEnt);

    return ;
}
VOS_UINT32 RRM_MNTN_CreateMutexSem(VOS_UINT32 ulPid)
{
    VOS_UINT32      ulResult;

    ulResult    = VOS_SmMCreate("INIT", VOS_SEMA4_FIFO,
                    (VOS_UINT32 *)(&g_ulRrmInitLogMutexSem));
    if (VOS_OK != ulResult)
    {
        RRM_WARNING_LOG(UEPS_PID_RRM, "Create INIT g_ulRrmLogSaveMutexSem fail!");
        return VOS_ERR;
    }

    ulResult    = VOS_SmMCreate("SAVE", VOS_SEMA4_FIFO,
                    (VOS_UINT32 *)(&g_ulRrmLogSaveMutexSem));
    if (VOS_OK != ulResult)
    {
        RRM_WARNING_LOG(UEPS_PID_RRM, "Create SAVE g_ulRrmLogSaveMutexSem fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}

#endif /* FEATURE_ON == FEATURE_DSDS */


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

