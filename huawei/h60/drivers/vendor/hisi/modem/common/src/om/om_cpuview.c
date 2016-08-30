/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : om_cpuview.c
  �� �� ��   : ����
  ��    ��   : ��ׯ�� 59026
  ��������   : 2011��5��31��
  ����޸�   :
  ��������   : ��ά�ɲ�-����ͳ�ƹ���ʵ��
  �����б�   :
              OM_CPUVIEW_AddSliceRecord
              OM_CPUVIEW_EnterArea
              OM_CPUVIEW_EnterIntHook
              OM_CPUVIEW_ExitArea
              OM_CPUVIEW_ExitIntHook
              OM_CPUVIEW_Init
              OM_CPUVIEW_MsgCfgReq
              OM_CPUVIEW_MsgRptInd
              OM_CPUVIEW_ReportBasicInfo
              OM_CPUVIEW_ReportDetailInfo
              OM_CPUVIEW_StatTimerHandler
              OM_CPUVIEW_TaskSwitchHook
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

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    ��ά�ɲ���Ϣ�а�����C�ļ���ź궨��
*****************************************************************************/
/*lint -e(767)*/
#define THIS_FILE_ID                    OM_FILE_ID_OM_CPUVIEW_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/* ����ͳ�ƿ�����Ϣ��¼ */
OM_CPUVIEW_CTRL_STRU                    g_stOmCpuviewCtrl;

/* ����ͳ�ƻ���ͳ����Ϣ */
OM_CPUVIEW_BASIC_STRU                   g_stOmCpuviewBasic;

/* ����ͳ����ϸ��Ϣ��¼ */
OM_CPUVIEW_DETAILS_STRU                 g_stOmCpuviewDetails;

/* ����ջ��С���,ÿ��Ϊ��ջID����ջ��С */
VOS_UINT32 g_auwOmCpuviewStackSize[OM_CPUVIEW_TASK_NUM][3] =
{
    {DSP_FID_RT,        VOS_TSK_RT_STK_SIZE,        0},
    {DSP_FID_NORMAL,    VOS_TSK_NORMAL_STK_SIZE,    0},
    {DSP_FID_LOW,       VOS_TSK_LOW_STK_SIZE,       0},
    {IDLE_STK_ID,       VOS_TASK_IDLE_STK_SIZE,     0},
};


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*****************************************************************************
 �� �� ��  : OM_CPUVIEW_Init
 ��������  : ��ά�ɲ�ģ��CPUͳ�ƹ��ܴ�ʼ��
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
VOS_VOID OM_CPUVIEW_Init(VOS_VOID)
{

    /* Ĭ�Ϲرո���ͳ���ϱ� */
    UCOM_MemSet(OM_CPUVIEW_GetCtrlObjPtr(), 0, sizeof(OM_CPUVIEW_CTRL_STRU));
    UCOM_MemSet(OM_CPUVIEW_GetDetailPtr(), 0, sizeof(OM_CPUVIEW_DETAILS_STRU));

    /* ע�������л���¼���Ӻ��� */
    VOS_RegTaskSwitchHook(OM_CPUVIEW_TaskSwitchHook);

    /* ע���жϽ��������Ӻ��� */
    VOS_RegisterEnterIntrHook(OM_CPUVIEW_EnterIntHook);
    VOS_RegisterExitIntrHook(OM_CPUVIEW_ExitIntHook);

}

/*****************************************************************************
 �� �� ��  : OM_CPUVIEW_MsgCfgReq
 ��������  : ����CPU����ͳ�ƹ���������Ϣ
 �������  : VOS_VOID *pvOsaMsg - ID_OM_CODEC_CPU_VIEW_REQ��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��18��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 OM_CPUVIEW_MsgCfgReq(VOS_VOID *pvOsaMsg)
{
    VOS_UINT32                          uwRet     = UCOM_RET_SUCC;
    OM_CODEC_CPU_VIEW_REQ_STRU         *pstReqMsg = VOS_NULL;
    OM_CPUVIEW_CTRL_STRU               *pstCtrl   = OM_CPUVIEW_GetCtrlObjPtr();
    CODEC_OM_CPU_VIEW_CNF_STRU          stCnfMsg;
    OM_CPUVIEW_BASIC_STRU              *pstBasic  = OM_CPUVIEW_GetBasicPtr();
    VOS_CPU_SR                          srSave;

    UCOM_MemSet(&stCnfMsg, 0, sizeof(stCnfMsg));

    /* ����OM_CODEC_CPU_VIEW_REQ_STRU�ṹ������Ϣ���� */
    pstReqMsg = (OM_CODEC_CPU_VIEW_REQ_STRU*)pvOsaMsg;

    /* ������� */
    if (   (OM_SWITCH_BUTT <= pstReqMsg->enEnable)
        || (OM_SWITCH_BUTT <= pstReqMsg->enDetailEnable)
        || (0 == pstReqMsg->uhwPeriod)
        || (OM_CPUVIEW_MAX_PERIOD < pstReqMsg->uhwPeriod))
    {
        uwRet  = UCOM_RET_ERR_PARA;
    }
    else
    {
        /* ���ж� */
        srSave = VOS_SplIMP();

        /* ����ȫ�ֱ���,����Ϣ�����ñ��� */
        pstCtrl->enEnable       = pstReqMsg->enEnable;
        pstCtrl->enDetailEn     = pstReqMsg->enDetailEnable;
        pstCtrl->uhwPeriod      = (VOS_UINT16)(pstReqMsg->uhwPeriod
                                * OM_MSGHOOK_TENTH_SECOND_PER20MS);             /* ���ò�����λΪ20ms */

        /* ����������ͳ�ƹ��� */
        if (   (OM_SWITCH_ON == pstCtrl->enEnable)
            || (OM_SWITCH_ON == pstCtrl->enDetailEn))
        {
            /* ֹͣ��ʱ�� */
            uwRet = DRV_TIMER_Stop(&(pstCtrl->uwTimer));

            /* ��ղ����Ļ���ͳ������ */
            UCOM_MemSet(pstBasic, 0, sizeof(OM_CPUVIEW_BASIC_STRU));

            /* ������ʱ�� */
            uwRet = DRV_TIMER_Start(&(pstCtrl->uwTimer),
                                     pstCtrl->uhwPeriod,
                                     DRV_TIMER_MODE_PERIODIC,
                                     OM_CPUVIEW_StatTimerHandler,
                                     0);
            OM_LogInfo(OM_CPUVIEW_MsgCfgReq_StartOk);

        }
        /* �ر�������ͳ�ƹ��� */
        else
        {
            /* ֹͣ��ʱ�� */
            uwRet = DRV_TIMER_Stop(&(pstCtrl->uwTimer));

            /* ��ղ����Ļ���ͳ������ */
            UCOM_MemSet(pstBasic, 0, sizeof(OM_CPUVIEW_BASIC_STRU));
            OM_LogInfo(OM_CPUVIEW_MsgCfgReq_StopOk);
        }

        /* �ͷ��ж� */
        VOS_Splx(srSave);
    }

    /* ���ظ���Ϣ */
    stCnfMsg.uhwMsgId   = ID_CODEC_OM_CPU_VIEW_CNF;
    stCnfMsg.enReturn   = (UCOM_RET_ENUM_UINT16)uwRet;

    OM_COMM_SendTranMsg(&stCnfMsg, sizeof(stCnfMsg));

    return uwRet;
}

/*****************************************************************************
 �� �� ��  : OM_CPUVIEW_MsgRptInd
 ��������  : ��ʱ����ͳ����Ϣ������
 �������  : VOS_VOID *pvOsaMsg -
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��20��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 OM_CPUVIEW_MsgRptInd(VOS_VOID *pvOsaMsg)
{
    /* �ϱ�����CPU������Ϣ */
    OM_CPUVIEW_ReportBasicInfo();

    /* �ϱ�CPU������ϸ��Ϣ */
    OM_CPUVIEW_ReportDetailInfo();

    return UCOM_RET_SUCC;
}

/*****************************************************************************
 �� �� ��  : OM_CPUVIEW_ReportBasicInfo
 ��������  : �ϱ�CPU����ͳ�ƻ�����Ϣ
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��20��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_CPUVIEW_ReportBasicInfo(VOS_VOID)
{
    OM_CPUVIEW_BASIC_STRU              *pstBasic    = OM_CPUVIEW_GetBasicPtr();
    CODEC_OM_CPUVIEW_BASIC_IND_STRU     stBasicMsg;
    VOS_UINT32                          uwCnt;
    VOS_UINT32                          uwRate;
    VOS_CPU_SR                          srSave;

    UCOM_MemSet(&stBasicMsg, 0, sizeof(stBasicMsg));

    /* CPU����ͳ��δʹ����ֱ�ӷ��� */
    if (OM_SWITCH_OFF == OM_CPUVIEW_GetBasicEn())
    {
        return;
    }

    /* �ӿں궨��һ���Լ�� */ /*lint --e(506)*/
    if (   (HIFI_TASK_NUM != OM_CPUVIEW_TASK_NUM)
        || (HIFI_INTR_NUM != OM_CPUVIEW_INTR_NUM))
    {
        OM_LogError(OM_CPUVIEW_ReportBasicInfo_MacroDefErr);
        return;
    }

    /* ����ϱ���Ϣ */

    stBasicMsg.uhwMsgId = ID_CODEC_OM_CPUVIEW_BASIC_IND;

    /* ���������Ϣ */
    for (uwCnt = 0; uwCnt < OM_CPUVIEW_TASK_NUM; uwCnt++)
    {
        uwRate  = OM_CPUVIEW_GetStackUsed(OM_CPUVIEW_GetStackId(uwCnt));
        uwRate  = (uwRate << OM_CPUVIEW_Q8) / OM_CPUVIEW_GetStackSize(uwCnt);

        stBasicMsg.auwStackRate[uwCnt]  = uwRate;
        stBasicMsg.auwTaskTime[uwCnt]   = pstBasic->astTaskRec[uwCnt].uwRunTime;
    }

    /* ����ж�ʱ����Ϣ */
    for (uwCnt = 0; uwCnt < OM_CPUVIEW_INTR_NUM; uwCnt++)
    {
        stBasicMsg.auwIntrTime[uwCnt] = pstBasic->astIntrRec[uwCnt].uwRunTime;
    }

    /* �ϱ���Ϣ */
    OM_COMM_SendTranMsg(&stBasicMsg, sizeof(stBasicMsg));

    /* ���ж� */
    srSave = VOS_SplIMP();

    /* ��մ�ǰ�ļ�¼ */
    UCOM_MemSet(pstBasic->astIntrRec, 0, sizeof(pstBasic->astIntrRec));
    UCOM_MemSet(pstBasic->astTaskRec, 0, sizeof(pstBasic->astTaskRec));

    /* �ͷ��ж� */
    VOS_Splx(srSave);

}

/*****************************************************************************
 �� �� ��  : OM_CPUVIEW_ReportDetailInfo
 ��������  : �ϱ�CPU����ͳ����ϸ��Ϣ
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��20��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_CPUVIEW_ReportDetailInfo(VOS_VOID)
{
    OM_CPUVIEW_DETAILS_STRU            *pstDetails  = OM_CPUVIEW_GetDetailPtr();
    CODEC_OM_CPUVIEW_DETAIL_IND_STRU      stCnfMsg;
    UCOM_DATA_BLK_STRU                  astBlk[3];  /* 3��:��Ϣͷ��������¼�� */
    VOS_UINT32                          uwBlkCnt;
    VOS_UINT16                          uhwRecNum;
    VOS_UINT16                          uhwRptIdx;

    /* CPU����ͳ��δʹ����ֱ�ӷ��� */
    if (OM_SWITCH_OFF == OM_CPUVIEW_GetDetailEn())
    {
        return;
    }

    /* ����һ�����ݿ�:��Ϣͷ */
    stCnfMsg.uhwMsgId   = ID_CODEC_OM_CPUVIEW_DETAIL_IND;
    astBlk[0].pucData   = (VOS_UCHAR*)&stCnfMsg;
    astBlk[0].uwSize    = sizeof(stCnfMsg) - sizeof(stCnfMsg.astRecords);

    /* ������ϱ���¼�� */
    uhwRecNum           = UCOM_COMM_CycSub(pstDetails->uhwCurrIdx,
                                           pstDetails->uhwRptIdx,
                                           OM_CPUVIEW_DETAIL_MAX_NUM);

    /* û��������Ҫ�ϱ���ֱ���˳� */
    if (0 == uhwRecNum)
    {
        return;
    }

    /* �����ϱ����ȿ��� */
    if (uhwRecNum > OM_CPUVIEW_DETAIL_RPT_MAX_NUM)
    {
        uhwRecNum = OM_CPUVIEW_DETAIL_RPT_MAX_NUM;
    }
    uhwRptIdx = OM_COMM_CycAdd(pstDetails->uhwRptIdx,
                               uhwRecNum,
                               OM_CPUVIEW_DETAIL_MAX_NUM);

    /* �ϱ������ڻ��ζ��е��������Կռ� */
    if (uhwRptIdx > pstDetails->uhwRptIdx)
    {
        uwBlkCnt            = 2;        /* ��Ϣͷ+1���ϱ����� */
        astBlk[1].pucData   = (VOS_UCHAR*)&(pstDetails->astRecords[pstDetails->uhwRptIdx]);
        astBlk[1].uwSize    = uhwRecNum * sizeof(OM_CPUVIEW_SLICE_RECORD_STRU);
    }
    /* �ϱ�����λ�ڻ��ζ��е�β����ͷ��(���������������Կռ�) */
    else
    {
        uwBlkCnt            = 3;        /* ��Ϣͷ+2���ϱ����� */
        astBlk[1].pucData   = (VOS_UCHAR*)&(pstDetails->astRecords[pstDetails->uhwRptIdx]);
        astBlk[1].uwSize    = (OM_CPUVIEW_DETAIL_MAX_NUM - pstDetails->uhwRptIdx)
                            * sizeof(OM_CPUVIEW_SLICE_RECORD_STRU);
        astBlk[2].pucData   = (VOS_UCHAR*)&(pstDetails->astRecords[0]);
        astBlk[2].uwSize    = uhwRptIdx * sizeof(OM_CPUVIEW_SLICE_RECORD_STRU);
    }

    /* �ϱ���Ϣ */
    OM_COMM_SendTrans(astBlk, uwBlkCnt);

    /* ���´�(�´�)�ϱ���¼λ�� */
    pstDetails->uhwRptIdx = uhwRptIdx;

}

/*****************************************************************************
 �� �� ��  : OM_CPUVIEW_AddSliceRecord
 ��������  : �����һ��CPU������ϸ��¼
 �������  : OM_CPUVIEW_TARGET_ENUM_UINT8    enTarget   - ��¼��������
             VOS_UINT8                       ucTargetId - ��¼������
             OM_CPUVIEW_ACTION_ENUM_UINT8    enAction   - ��¼��������
             VOS_UINT32                      uwTimeStamp- ʱ��
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��18��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_CPUVIEW_AddSliceRecord(
                OM_CPUVIEW_TARGET_ENUM_UINT8                enTarget,
                VOS_UINT8                                   ucTargetId,
                OM_CPUVIEW_ACTION_ENUM_UINT8                enAction,
                VOS_UINT32                                  uwTimeStamp)
{
    VOS_UINT16                          uhwIdx;
    OM_CPUVIEW_SLICE_RECORD_STRU       *pstRec;
    OM_CPUVIEW_DETAILS_STRU            *pstDetails;
    VOS_CPU_SR                          srSave;

    /* ���ж� */
    srSave = VOS_SplIMP();

    /* ��ȡ��д��λ�� */
    pstDetails              = OM_CPUVIEW_GetDetailPtr();
    uhwIdx                  = pstDetails->uhwCurrIdx;
    pstRec                  = &(pstDetails->astRecords[uhwIdx]);

    /* д��һ���µ���ϸ��¼ */
    pstRec->uwTarget        = (enTarget & 0x3);             /* ȡ��2���� */
    pstRec->uwTargetId      = (ucTargetId & 0x1f);          /* ȡ��5���� */
    pstRec->uwAction        = (enAction & 0x1);             /* ȡ��1���� */
    pstRec->uwTimeStamp     = (uwTimeStamp & 0xffffff);     /* ȡ��24����*/
    pstDetails->uhwCurrIdx  = ((uhwIdx + 1) % OM_CPUVIEW_DETAIL_MAX_NUM);

    /* �ͷ��ж� */
    VOS_Splx(srSave);

}

/*****************************************************************************
 �� �� ��  : OM_CPUVIEW_TaskSwitchHook
 ��������  : �����л����Ӻ������������л����ݼ�¼����;ע����������л�ʱ����
 �������  : VOS_VOID *pvOldTcb - �˳�����,����A��TCBָ��
             VOS_VOID *pvNewTcb - ��������,����B��TCBָ��
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��20��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_CPUVIEW_TaskSwitchHook(VOS_VOID *pvOldTcb, VOS_VOID *pvNewTcb)
{
    VOS_UINT32                          uwTimeStamp;
    VOS_UINT32                          uwDuration;
    VOS_UINT8                           ucTaskId;
    OM_CPUVIEW_BASIC_STRU              *pstBasic;
    OM_CPUVIEW_DETAILS_STRU            *pstDetails;
    VOS_TCB                            *pstOldTcb   = (VOS_TCB*)pvOldTcb;
    VOS_TCB                            *pstNewTcb   = (VOS_TCB*)pvNewTcb;
    VOS_CPU_SR                          srSave;

    /* ��ȡ�����л�ʱ�� */
    uwTimeStamp = DRV_TIMER_ReadSysTimeStamp();

    /* ��ȡ��������� */
    ucTaskId    = (VOS_UINT8)(pstNewTcb->OSTCBPrio & 0x1f);

    /* ��ֻ��¼���������ϸ��Ϣ, ��ȡ����ŵ�5bit(ȡֵ0-31) */
    OM_CPUVIEW_AddSliceRecord(OM_CPUVIEW_TARGET_TASK,
                              ucTaskId,
                              OM_CPUVIEW_ACTION_ENTER,
                              uwTimeStamp);

    /* ������Ϣ�ϱ���ʱ */
    if (OM_SWITCH_ON == OM_CPUVIEW_GetBasicEn())
    {
        /* ���ж� */
        srSave = VOS_SplIMP();

        pstBasic    = OM_CPUVIEW_GetBasicPtr();

        /* ����OLD����������ʱ�γ���(���ж�) */
        uwDuration  = UCOM_COMM_CycSub(uwTimeStamp, pstBasic->uwEnterTaskTs, 0xffffffff);

        /* ����OLD����������ʱ�� */
        uwDuration  -= pstBasic->uwIntrTime;

        /* ����OLD����ʱ����Ϣ */
        ucTaskId    = (VOS_UINT8)(pstOldTcb->OSTCBPrio & 0x1f);
        pstBasic->astTaskRec[ucTaskId].uwRunTime += uwDuration;

        /* ���µ�ǰ������ʼʱ��, ����ж�����ʱ�� */
        pstBasic->uwIntrTime    = 0;
        pstBasic->uwEnterTaskTs = uwTimeStamp;

        /* �ͷ��ж� */
        VOS_Splx(srSave);

    }

    /* ��ϸ��Ϣ�ϱ���ʱ */
    if (OM_SWITCH_ON == OM_CPUVIEW_GetDetailEn())
    {
        pstDetails = OM_CPUVIEW_GetDetailPtr();

        /* ���ϱ���¼�����ػ���ֵʱ�����ϱ� */
        if (OM_CPUVIEW_DETAIL_RPT_GUARD_NUM
            < UCOM_COMM_CycSub(pstDetails->uhwCurrIdx,
                               pstDetails->uhwRptIdx,
                               OM_CPUVIEW_DETAIL_MAX_NUM))
        {
            /* �ϱ�����CPU������Ϣ */
            OM_CPUVIEW_ReportBasicInfo();

            /* �ϱ�CPU������ϸ��Ϣ */
            OM_CPUVIEW_ReportDetailInfo();
        }
    }

}

/*****************************************************************************
 �� �� ��  : OM_CPUVIEW_EnterIntHook
 ��������  : ��¼�����жϵ�ʱ����Ϣ, �����ж�ʱ������
 �������  : VOS_UINT32 uwIntNo - �жϺ�
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��18��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_CPUVIEW_EnterIntHook(VOS_UINT32 uwIntNo)
{
    VOS_UINT32                          uwTimeStamp;
    OM_CPUVIEW_BASIC_STRU              *pstBasic;

    /* ��ȡ�жϽ���ʱ�� */
    uwTimeStamp = DRV_TIMER_ReadSysTimeStamp();

    /* ��¼������Ϣ */
    if (OM_SWITCH_ON == OM_CPUVIEW_GetBasicEn())
    {
        pstBasic = OM_CPUVIEW_GetBasicPtr();
        pstBasic->uwEnterIntrTs = uwTimeStamp;
    }

    /* ��¼��ϸ��Ϣ, ��ȡ�жϺŵ�5bit(ȡֵ0-31) */
    OM_CPUVIEW_AddSliceRecord(OM_CPUVIEW_TARGET_INT,
                              (VOS_UINT8)(uwIntNo & 0x1f),
                              OM_CPUVIEW_ACTION_ENTER,
                              uwTimeStamp);
}

/*****************************************************************************
 �� �� ��  : OM_CPUVIEW_ExitIntHook
 ��������  : �ж��˳�ʱ������, �����ж�����ʱ��, �������жϲ�Ƕ��
 �������  : VOS_UINT32 uwIntNo - �жϺ�
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��18��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_CPUVIEW_ExitIntHook(VOS_UINT32 uwIntNo)
{
    VOS_UINT32                          uwTimeStamp;
    VOS_UINT32                          uwDuration;
    OM_CPUVIEW_BASIC_STRU              *pstBasic;

    /* ��ȡ�жϽ���ʱ�� */
    uwTimeStamp = DRV_TIMER_ReadSysTimeStamp();

    /* ��¼��ϸ��Ϣ, ��ȡ�жϺŵ�5bit(ȡֵ0-31) */
    OM_CPUVIEW_AddSliceRecord(OM_CPUVIEW_TARGET_INT,
                              (VOS_UINT8)(uwIntNo & 0x1f),
                              OM_CPUVIEW_ACTION_EXIT,
                              uwTimeStamp);

    if (OM_SWITCH_ON == OM_CPUVIEW_GetBasicEn())
    {
        pstBasic = OM_CPUVIEW_GetBasicPtr();

        /* �����жϽ���ʱ������ж�ͣ��ʱ�� */
        uwDuration = UCOM_COMM_CycSub(uwTimeStamp, pstBasic->uwEnterIntrTs, 0xffffffff);

        /* ���¶�Ӧ�жϵ�������ʱ�� */
        pstBasic->astIntrRec[uwIntNo].uwRunTime += uwDuration;
        pstBasic->uwIntrTime                    += uwDuration;
    }
}

/*****************************************************************************
 �� �� ��  : OM_CPUVIEW_EnterArea
 ��������  : ����ͳ���������
 �������  : VOS_UCHAR ucAreaId
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��18��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_CPUVIEW_EnterArea(VOS_UCHAR ucAreaId)
{
    VOS_UINT32              uwTimeStamp;

    uwTimeStamp = DRV_TIMER_ReadSysTimeStamp();

    OM_CPUVIEW_AddSliceRecord(OM_CPUVIEW_TARGET_REGION,
                              ucAreaId,
                              OM_CPUVIEW_ACTION_ENTER,
                              uwTimeStamp);
}

/*****************************************************************************
 �� �� ��  : OM_CPUVIEW_ExitArea
 ��������  : ����ͳ�������˳�
 �������  : VOS_UCHAR ucAreaId
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��18��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_CPUVIEW_ExitArea(VOS_UCHAR ucAreaId)
{
    VOS_UINT32              uwTimeStamp;

    uwTimeStamp = DRV_TIMER_ReadSysTimeStamp();

    OM_CPUVIEW_AddSliceRecord(OM_CPUVIEW_TARGET_REGION,
                              ucAreaId,
                              OM_CPUVIEW_ACTION_EXIT,
                              uwTimeStamp);
}

/*****************************************************************************
 �� �� ��  : OM_CPUVIEW_StatTimerHandler
 ��������  : ����������CPUͳ�ƵĶ�ʱ��������
 �������  : VOS_UINT32 uwTimer - ��ʱ����־
             VOS_UINT32 uwPara  - �ص�����
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��20��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_CPUVIEW_StatTimerHandler(VOS_UINT32 uwTimer, VOS_UINT32 uwPara)
{
    TIMER_OM_CPUVIEW_RPT_IND_STRU       stReqMsg;

    stReqMsg.uhwMsgId      = ID_TIMER_CODEC_CPUVIEW_RPT_IND;

    UCOM_SendOsaMsg(DSP_PID_HIFI_OM, DSP_PID_HIFI_OM, &stReqMsg, sizeof(stReqMsg));
}


VOS_VOID OM_CPUVIEW_RecordStackUsage( VOS_VOID )
{
    VOS_UINT32 uwCnt, uwSize;

    for (uwCnt = 0; uwCnt < OM_CPUVIEW_TASK_NUM; uwCnt++)
    {
        uwSize  = OM_CPUVIEW_GetStackUsed(OM_CPUVIEW_GetStackId(uwCnt));

        OM_CPUVIEW_SetStackUsed(uwCnt, uwSize);
    }
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

