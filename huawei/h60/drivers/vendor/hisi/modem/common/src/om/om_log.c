/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : om_log.c
  �� �� ��   : ����
  ��    ��   : ��ׯ�� 59026
  ��������   : 2011��5��31��
  ����޸�   :
  ��������   : ��ά�ɲ�-��־�ϱ�����־��¼����ʵ��
  �����б�   :
              OM_LOG_DefauleCfg
              OM_LOG_Init
              OM_LOG_InnerRecord
              OM_LOG_IsrSoftIntr
              OM_LOG_LogReport
              OM_LOG_MsgSetLogReq
  �޸���ʷ   :
  1.��    ��   : 2011��5��31��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "om.h"
#include "med_drv_timer_hifi.h"
#include "ucom_comm.h"
#include "ucom_low_power.h"

#ifdef _OM_UART_PRINT
#include "stdlib.h"
#include "stdio.h"
#include "med_drv_uart.h"
#endif

#ifdef _OM_STD_PRINT
#include "stdlib.h"
#include "stdio.h"
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
#define THIS_FILE_ID                    OM_FILE_ID_OM_LOG_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/* OMģ���ʼ����־ */
VOS_UINT32              g_uwOmLogInitFlag = 0;

/* ��־����ȫ�ֱ��� */
OM_LOG_CTRL_STRU        g_stOmLogCtrl;

/* �ڲ�����¼, λ��TCM */
OM_LOG_INNER_OBJ_STRU   g_stOmLogInnerRec;

/* ���һ�������¼ */
OM_LOG_ITEM_STRU        g_stOmLogLastErrRec;

/* APR��־��¼ */
OM_LOG_APR_OBJ_STRU     g_stOmAprLog;

/* log���𵽱�����ϢIDӳ��� */
VOS_UINT16              g_auhwOmLogLevelToMsgIdTable[] =
{
    ID_CODEC_OM_LOG_INFO_IND,
    ID_CODEC_OM_LOG_WARNING_IND,
    ID_CODEC_OM_LOG_ERR_IND
};

/* �����жϷ�װ����Ķ��壬�ڲ��洢�����ж���Ϣ */
extern VOS_UINT32       g_uwSoftIntInfo;

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*****************************************************************************
 �� �� ��  : OM_LOG_Init
 ��������  : ��ά�ɲ���־���ܳ�ʼ��
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��3��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_LOG_Init(VOS_VOID)
{
    /* ��ʼ��ȫ�ֱ����ռ� */
    UCOM_MemSet(OM_LOG_GetCtrlObjPtr(), 0, sizeof(OM_LOG_CTRL_STRU));
    UCOM_MemSet(OM_LOG_GetInnerObjPtr(), 0, sizeof(OM_LOG_INNER_OBJ_STRU));
    UCOM_MemSet(OM_LOG_GetAprLogPtr(), 0, sizeof(OM_LOG_APR_OBJ_STRU));

    /* ע�����ж����ڼ�¼�������� */
    VOS_ConnectInterrupt(OS_INTR_CONNECT_01, OM_LOG_IsrSoftIntr);
    VOS_EnableInterrupt(OS_INTR_CONNECT_01);

    /* ��־Ϊ�ѳ�ʼ�� */
    OM_LOG_SetInit();
}

/*****************************************************************************
 �� �� ��  : OM_LOG_DefaultCfg
 ��������  : ͨ��NV������log�ϱ�����
 �������  : CODEC_LOG_HOOK_CONFIG_STRU *
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��11��21��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 OM_LOG_DefaultCfg(CODEC_LOG_HOOK_CONFIG_STRU *pstLogHookCfg)
{
    /* �ж�NV�����õ�ֵ�ĺϷ��� */
    if (   (pstLogHookCfg->uhwEnable >= OM_SWITCH_BUTT)
        || (pstLogHookCfg->uhwLevel >= OM_LOG_LEVEL_BUTT))
    {
        return UCOM_RET_ERR_PARA;
    }

    /* ����NV�������Ӧ���޸� */
    OM_LOG_SetEnable(pstLogHookCfg->uhwEnable);
    OM_LOG_SetLevel(pstLogHookCfg->uhwLevel);

    return UCOM_RET_SUCC;
}

/*****************************************************************************
 �� �� ��  : OM_LOG_InnerRecord
 ��������  : �����־���ڲ���¼
 �������  : OM_LOG_ID_ENUM_UINT32 enLogId - log id
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��18��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_LOG_InnerRecord(
                OM_LOG_ID_ENUM_UINT32   enLogId,
                VOS_UINT16              uhwFileID,
                VOS_UINT16              uhwLineID)
{
    OM_LOG_INNER_OBJ_STRU      *pstInnerObj = OM_LOG_GetInnerObjPtr();
    OM_LOG_INNER_RECORD_STRU   *pstRecord   = VOS_NULL;
    VOS_UINT32                  uwIndx;
    VOS_CPU_SR                  srSave;

    /* δ��ʼ����ֱ���˳� */
    if (!OM_LOG_CheckInit())
    {
        return;
    }

    /* ���ж� */
    srSave = VOS_SplIMP();

    /* д���¼ */
    uwIndx                  = pstInnerObj->uwCurrIdx;
    pstRecord               = OM_LOG_GetInnerRecord(uwIndx);
    pstRecord->enLogId      = enLogId;
    pstRecord->uwTimeStamp  = DRV_TIMER_ReadSysTimeStamp();
    pstRecord->uhwFileID    = uhwFileID;
    pstRecord->uhwLineID    = uhwLineID;
    pstInnerObj->uwCurrIdx  = (uwIndx + 1) % OM_LOG_INNER_MAX_NUM;

    /* �ͷ��ж� */
    VOS_Splx(srSave);

}

#ifndef VOS_VENUS_TEST_STUB
/*****************************************************************************
 �� �� ��  : OM_LOG_LogReport
 ��������  : �ϱ�������־
 �������  : OM_LOG_RPT_LEVEL_ENUM_UINT16        enLevel    - ��־����
             OM_FILE_ID_ENUM_UINT16              enFileNo   - ��־�ļ���
             VOS_UINT16                          uhwLineNo  - ��־�к�
             OM_LOG_ID_ENUM_UINT32               enLogId    - ��־���
             VOS_UINT16                          uhwLogFreq - ��־Ƶ��
             VOS_INT32                           swValue1   - ����ֵ1
             VOS_INT32                           swValue2   - ����ֵ2
             VOS_INT32                           swValue3   - ����ֵ3
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��18��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 OM_LOG_LogReport(
                OM_LOG_RPT_LEVEL_ENUM_UINT16        enLevel,
                OM_FILE_ID_ENUM_UINT16              enFileNo,
                VOS_UINT16                          uhwLineNo,
                OM_LOG_ID_ENUM_UINT32               enLogId,
                VOS_UINT16                          uhwLogFreq,
                VOS_INT32                           swValue1,
                VOS_INT32                           swValue2,
                VOS_INT32                           swValue3)
{
    CODEC_OM_LOG_IND_STRU               stLogMsg;
    VOS_UINT32                          uwCntIndx;
    VOS_UINT16                          uhwCycleCnt;
    OM_LOG_ITEM_STRU                   *pstLastErr;

    UCOM_MemSet(&stLogMsg, 0, sizeof(stLogMsg));

    if (!OM_LOG_CheckInit())
    {
        return UCOM_RET_SUCC;
    }

    /* ��¼��־��Ϣ���ڲ�TCM, ����λ���� */
    OM_LOG_InnerRecord(enLogId, enFileNo, uhwLineNo);

    /* ���һ�������¼�м�¼��Ϣ */
    if(OM_LOG_LEVEL_ERROR == enLevel)
    {
        pstLastErr              = OM_LOG_GetLastErrRec();

        pstLastErr->enLogId     = enLogId;
        pstLastErr->uwTimeStamp = DRV_TIMER_ReadSysTimeStamp();
        pstLastErr->uhwFileID   = enFileNo;
        pstLastErr->uhwLineID   = uhwLineNo;

        pstLastErr->swValue1 = swValue1;
        pstLastErr->swValue2 = swValue2;
        pstLastErr->swValue3 = swValue3;
    }

    /* log�ϱ�δ�����˳� */
    if (OM_SWITCH_ON != OM_LOG_GetEnalbe())
    {
        return UCOM_RET_SUCC;
    }

    /* ��log��Ϣ����δ�ﵽ���ü������˳� */
    if (enLevel < OM_LOG_GetLevel())
    {
        return UCOM_RET_SUCC;
    }

    /* ��ѯ����־�ϱ�����Ƿ����������ϱ���־��Χ�� */
    if (   (enLogId >= OM_LOG_COUNTER_BASE)
        && (enLogId <  (OM_LOG_COUNTER_BASE + OM_LOG_COUNTER_MAX_NUM)))
    {
        uwCntIndx   = enLogId - OM_LOG_COUNTER_BASE;
        uhwCycleCnt = OM_LOG_GetCycleCnt(uwCntIndx);
        uhwCycleCnt = (uhwCycleCnt + 1) % uhwLogFreq;

        /*��¼����ֵ*/
        OM_LOG_SetCycleCnt(uwCntIndx, uhwCycleCnt);

        /* ����δ���ϱ����ڣ���ֱ�ӷ��� */
        if (uhwCycleCnt > 0)
        {
            return UCOM_RET_SUCC;
        }
    }

    /* ���log��Ϣ*/
    stLogMsg.uhwMsgId       = OM_LOG_GetLogMsgId(enLevel);
    stLogMsg.uhwFileId      = enFileNo;
    stLogMsg.uhwLineNo      = uhwLineNo;
    stLogMsg.uwLogId        = enLogId;
    stLogMsg.aswData[0]     = swValue1;
    stLogMsg.aswData[1]     = swValue2;
    stLogMsg.aswData[2]     = swValue3;

    /* ��͸����Ϣ���� */
    OM_COMM_SendTranMsg(&stLogMsg, sizeof(stLogMsg));

    /* ͨ�����ڴ�ӡ */
    #ifdef _OM_UART_PRINT
    {
        VOS_CHAR    ascString[140]; /* ��ʽ�ַ�����, �ٳ�ֿ���7�������ֵ�Ŀ�� */

        sprintf(ascString,
                "OM_LOG:: Level %d, File %d, Line %d, Log id %d, Log value %x, %x, %x.\n",
                enLevel,
                enFileNo,
                uhwLineNo,
                enLogId,
                swValue1,
                swValue2,
                swValue3);

        /* ���º���Ϊ����ʽ����д, ʵ�ʵ���ʱ�ɿ��������ַ������� */
        DRV_UART_SendData(ascString, strlen(ascString) + 1);

    }
    #endif

    /* ��PC����ʹ�� */
    #ifdef _OM_STD_PRINT
    {
        printf("OM_LOG:: Level %d, File %d, Line %d, Log id %d, Log value %x, %x, %x.\n",
               enLevel,
               enFileNo,
               uhwLineNo,
               enLogId,
               swValue1,
               swValue2,
               swValue3);
    }
    #endif

    return UCOM_RET_SUCC;

}
#endif

/*****************************************************************************
 �� �� ��  : OM_LOG_MsgCfgLog
 ��������  : HIFI��־�ϱ�������ϢMSG_OM_VOICE_SET_LOG_REQ������
 �������  : VOS_VOID *pvOsaMsg - VOS��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��5��31��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 OM_LOG_MsgSetLogReq(VOS_VOID *pvOsaMsg)
{
    VOS_UINT16                  uhwRet     = UCOM_RET_SUCC;
    OM_CODEC_SET_LOG_REQ_STRU    *pstReqMsg  = VOS_NULL;
    OM_LOG_CTRL_STRU           *pstCtrlObj = OM_LOG_GetCtrlObjPtr();
    CODEC_OM_SET_LOG_CNF_STRU     stCnfMsg;

    UCOM_MemSet(&stCnfMsg, 0, sizeof(stCnfMsg));

    /*��OM_CODEC_SET_LOG_REQ_STRU������Ϣ�ṹ*/
    pstReqMsg = (OM_CODEC_SET_LOG_REQ_STRU *)pvOsaMsg;

    /* ������飬���Ƿ����򷵻ز������� */
    if (   (pstReqMsg->enLogEnable >= OM_SWITCH_BUTT)
        || (pstReqMsg->enLogLevel  >= OM_LOG_LEVEL_BUTT))
    {
        uhwRet                  = UCOM_RET_ERR_PARA;
    }
    /*���Ϸ��������ÿ�ά�ɲ���־����ȫ�ֱ��� */
    else
    {
        UCOM_MemSet(pstCtrlObj->auhwCycleCnt, 0, sizeof(pstCtrlObj->auhwCycleCnt));
        OM_LOG_SetEnable(pstReqMsg->enLogEnable);
        OM_LOG_SetLevel(pstReqMsg->enLogLevel);
    }

    /* �ظ���Ϣ */
    stCnfMsg.uhwMsgId           = ID_CODEC_OM_SET_LOG_CNF;
    stCnfMsg.enReturn           = uhwRet;

    OM_COMM_SendTranMsg(&stCnfMsg, sizeof(stCnfMsg));

    return uhwRet;

}

/*****************************************************************************
 �� �� ��  : OM_LOG_IsrSoftIntr
 ��������  : ���жϴ���, ���ж��������ϵͳ�����쳣ͳһ����, �������жϺ���뱾
             ������, ��¼�����Ϣ��ϵͳ������ѭ���ȴ���Ӳ���ػ�����λ
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��18��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_LOG_IsrSoftIntr(VOS_VOID)
{
    OM_LOG_INNER_RESET_STRU            *pstCnter;

    /* ��¼����ŵ��ڲ���־ */
    OM_LOG_InnerRecord(OM_LOG_GetSoftIntrInfo(), (VOS_UINT16)THIS_FILE_ID, (VOS_UINT16)__LINE__);

    /* ������ѭ��, �ȴ�Ӳ������Ⲣ��λϵͳ */
    pstCnter = OM_LOG_GetInnerCnt();
    for ( ; ;)
    {
        pstCnter->uwCnt++;
        pstCnter->uwTimeStamp = DRV_TIMER_ReadSysTimeStamp();
    }
}


VOS_VOID  OM_LOG_AprLogReport(
                OM_APR_MODULE_ID_ENUM_UINT32        enModuleId,
                VOS_UINT32                          uwLogId)
{
    OM_LOG_APR_OBJ_STRU        *pstAprLog;

    /* �쳣��μ�� */
    if (  (!OM_LOG_CheckInit())
        ||(OM_LOG_APR_MAX_NUM <= uwLogId)
        ||(OM_APR_MODULE_BUTT <= enModuleId))
    {
        /* ������־��¼ */
        return;
    }

    /* ��ȡapr��־�ṹ�� */
    pstAprLog      = OM_LOG_GetAprLogPtr();

    /* ����Ӧ��log������һ */
    if (OM_LOG_APR_MAX_LOG_CNT_NUM > pstAprLog->auwAprCnt[enModuleId][uwLogId])
    {
        pstAprLog->auwAprCnt[enModuleId][uwLogId]++;
    }

    return;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

