/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : om_cpuview.h
  �� �� ��   : ����
  ��    ��   : ��ׯ�� 59026
  ��������   : 2011��5��31��
  ����޸�   :
  ��������   : om_cpuview.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��5��31��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "om_comm.h"


#ifndef __OM_CPUVIEW_H__
#define __OM_CPUVIEW_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/

#define OM_CPUVIEW_TASK_NUM             VOS_MAX_TASKS                           /* ��ǰ����ϵͳ֧������������*/
#define OM_CPUVIEW_INTR_NUM             (32)                                    /* ��ǰϵͳ����жϸ���*/
#define OM_CPUVIEW_MAX_PERIOD           (5)                                     /* ����ϱ������� */
#define OM_CPUVIEW_Q8                   (8)                                     /* Q8 */
#define OM_CPUVIEW_DETAIL_RPT_MAX_NUM   (100)                                   /* CPU������ϸ��Ϣ�����ϱ��������*/
#define OM_CPUVIEW_DETAIL_MAX_NUM       (1024)                                  /* CPU������ϸ��Ϣ��¼�������*/
#define OM_CPUVIEW_DETAIL_RPT_GUARD_NUM (200)                                   /* cPU������ϸ��Ϣ�ϱ��ػ����� */

#define OM_CPUVIEW_GetCtrlObjPtr()      (&g_stOmCpuviewCtrl)
#define OM_CPUVIEW_GetBasicPtr()        (&g_stOmCpuviewBasic)
#define OM_CPUVIEW_GetDetailPtr()       (&g_stOmCpuviewDetails)
#define OM_CPUVIEW_GetStackSize(uhwIdx) (g_auwOmCpuviewStackSize[(uhwIdx)][1])
#define OM_CPUVIEW_GetStackId(uhwIdx)   (g_auwOmCpuviewStackSize[(uhwIdx)][0])
#define OM_CPUVIEW_SetStackUsed(uhwIdx,uwSize) (g_auwOmCpuviewStackSize[(uhwIdx)][2] = (uwSize))
#define OM_CPUVIEW_GetBasicEn()         (g_stOmCpuviewCtrl.enEnable)
#define OM_CPUVIEW_GetDetailEn()        (g_stOmCpuviewCtrl.enDetailEn)

#if (VOS_STK_CHK_EN == VOS_YES)
#define OM_CPUVIEW_GetStackUsed(uwFid)  VOS_GetMaxStackUsed(uwFid)              /* ��ȡָ��FID��ջ���ʹ���� */
#else
#define OM_CPUVIEW_GetStackUsed(uwFid)  (0)
#endif
/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/* CPUVIEW�е�������ö�� */
enum OM_CPUVIEW_AREA_ENUM
{
    OM_CPUVIEW_AREA_UCOM_WFI                                = 0,
    OM_CPUVIEW_AREA_UCOM_DRF,
    OM_CPUVIEW_AREA_UCOM_PD,

    OM_CPUVIEW_AREA_AUDIO_PCM_UPDATE_BUFF_PLAY              = 8,
    OM_CPUVIEW_AREA_AUDIO_PCM_UPDATE_BUFF_CAPTURE,
    OM_CPUVIEW_AREA_AUDIO_ENHANCE_PROC_MLIB_MICIN,
    OM_CPUVIEW_AREA_AUDIO_ENHANCE_PROC_MLIB_SPKOUT,
    OM_CPUVIEW_AREA_AUDIO_ENHANCE_PROC_MICIN,
    OM_CPUVIEW_AREA_AUDIO_ENHANCE_PROC_SPKOUT,
    OM_CPUVIEW_AREA_AUDIO_PLAYER_DECODE,
    OM_CPUVIEW_AREA_AUDIO_PLAYER_SRC,
    OM_CPUVIEW_AREA_AUDIO_PLAYER_DTS,

    OM_CPUVIEW_AREA_VOICE_PROC_MICIN                        = 24,
    OM_CPUVIEW_AREA_VOICE_PROC_SPKOUT,
    OM_CPUVIEW_AREA_VOICE_ENCODE,
    OM_CPUVIEW_AREA_VOICE_DECODE,

    OM_CPUVIEW_AREA_ENUM_BUTT
};
typedef VOS_UINT8   OM_CPUVIEW_AREA_ENUM_UINT8;

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/
/*****************************************************************************
 ʵ������  : TIMER_OM_CPUVIEW_RPT_IND_STRU
 ��������  : ID_TIMER_MED_CPUVIEW_RPT_IND�ṹ��
 *****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT16                          uhwMsgId;
    VOS_UINT16                          uhwReserve;
}TIMER_OM_CPUVIEW_RPT_IND_STRU;

/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
/*****************************************************************************
 ʵ������  : OM_CPUVIEW_TASK_RECORD_STRU
 ��������  : ����״̬�����ṹ��,ÿ�������Ӧһ���ṹ,������
 *****************************************************************************/
typedef struct
{
    VOS_UINT32                          uwRunTime;                              /* �������޳��жϴ�ϵ���������ʱ������λ��ʱ����ͬ */
}OM_CPUVIEW_TASK_RECORD_STRU;

/*****************************************************************************
 ʵ������  : OM_CPUVIEW_INTR_RECORD_STRU
 ��������  : �ж����������ṹ��,ÿ���ж϶�Ӧһ���ýṹ,������
 *****************************************************************************/
typedef struct
{
    VOS_UINT32                          uwRunTime;                              /* �������ж�������ʱ������λ��ʱ����ͬ */
}OM_CPUVIEW_INTR_RECORD_STRU;

/*****************************************************************************
 ʵ������  : OM_CPUVIEW_BASIC_STRU
 ��������  : ��ǰ����/�ж�����״̬�����ṹ��,��¼��ǰ����/�ж�����ʱ����Ϣ
 *****************************************************************************/
typedef struct
{
    VOS_UINT32                          uwEnterTaskTs;                          /*��ǰ����ʼʱ��*/
    VOS_UINT32                          uwEnterIntrTs;                          /*��ǰ�жϽ���ʱ��*/
    VOS_UINT32                          uwIntrTime;                             /*��ǰ�����ж�����ʱ��,ÿ�ν��������л�ʱ��������*/
    OM_CPUVIEW_TASK_RECORD_STRU         astTaskRec[OM_CPUVIEW_TASK_NUM];        /*����ͳ�Ƽ�¼*/
    OM_CPUVIEW_INTR_RECORD_STRU         astIntrRec[OM_CPUVIEW_INTR_NUM];        /*�ж�ͳ�Ƽ�¼*/
}OM_CPUVIEW_BASIC_STRU;

/*****************************************************************************
 ʵ������  : OM_CPUVIEW_DETAILS_STRU
 ��������  : ����ͳ����ϸ״̬��¼�ṹ��
 *****************************************************************************/
typedef struct
{
    VOS_UINT16                          uhwCurrIdx;                             /*��ǰ��¼�ı��(���ӵ�ǰ��¼��ʼ˳��ѭ������)*/
    VOS_UINT16                          uhwRptIdx;                              /*�ϱ���¼���*/
    OM_CPUVIEW_SLICE_RECORD_STRU        astRecords[OM_CPUVIEW_DETAIL_MAX_NUM];  /* CPU���ؼ�¼��ϸ��¼ */
}OM_CPUVIEW_DETAILS_STRU;

/*****************************************************************************
 ʵ������  : OM_CPUVIEW_CTRL_STRU
 ��������  : ����ͳ�ƿ�����Ϣ��¼�ṹ��
 *****************************************************************************/
typedef struct
{
    OM_SWITCH_ENUM_UINT16               enEnable;                               /* ����ͳ���ϱ����� */
    VOS_UINT16                          uhwReserved;                            /* ����*/
    VOS_UINT16                          uhwPeriod;                              /* ����ͳ���ϱ����ڣ���λ0.1ms*/
    VOS_UINT16                          enDetailEn;                             /* ����ͳ����ϸ��Ϣ�ϱ����� */
    VOS_UINT32                          uwTimer;                                /* ��ʱ�� */

}OM_CPUVIEW_CTRL_STRU;


/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/

extern VOS_VOID OM_CPUVIEW_AddSliceRecord(
                       OM_CPUVIEW_TARGET_ENUM_UINT8                enTarget,
                       VOS_UINT8                                   ucTargetId,
                       OM_CPUVIEW_ACTION_ENUM_UINT8                enAction,
                       VOS_UINT32                                  uwTimeStamp);
extern VOS_VOID OM_CPUVIEW_EnterArea(VOS_UCHAR ucAreaId);
extern VOS_VOID OM_CPUVIEW_EnterIntHook(VOS_UINT32 uwIntNo);
extern VOS_VOID OM_CPUVIEW_ExitArea(VOS_UCHAR ucAreaId);
extern VOS_VOID OM_CPUVIEW_ExitIntHook(VOS_UINT32 uwIntNo);
extern VOS_VOID OM_CPUVIEW_Init(VOS_VOID);
extern VOS_UINT32 OM_CPUVIEW_MsgCfgReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 OM_CPUVIEW_MsgRptInd(VOS_VOID *pvOsaMsg);
extern VOS_VOID OM_CPUVIEW_ReportBasicInfo(VOS_VOID);
extern VOS_VOID OM_CPUVIEW_ReportDetailInfo(VOS_VOID);
extern VOS_VOID OM_CPUVIEW_StatTimerHandler(VOS_UINT32 uwTimer, VOS_UINT32 uwPara);
extern VOS_VOID OM_CPUVIEW_TaskSwitchHook(VOS_VOID *pvOldTcb, VOS_VOID *pvNewTcb);
extern VOS_VOID OM_CPUVIEW_RecordStackUsage( VOS_VOID );


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of om_cpuview.h */
