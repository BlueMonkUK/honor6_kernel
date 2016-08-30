/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : om_cpuload.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2013��12��28��
  ����޸�   :
  ��������   : om_cpuload.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��12��28��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __OM_CPULOAD_H__
#define __OM_CPULOAD_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "vos.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define OM_CPULOAD_MAX_FUNC_NUM  (1024)

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
/* ����ͳ�ƺ���ö�� */
enum OM_CPULOAD_PERFORMANCE_CAL_ENUM
{
    OM_CPULOAD_PERFORMANCE_TxPp_1MIC            = 0,        //����
    OM_CPULOAD_PERFORMANCE_TxPp_2MIC,
    OM_CPULOAD_PERFORMANCE_1MIC_AECANR_Main,
    OM_CPULOAD_PERFORMANCE_2MIC_AECANR_Main,
    OM_CPULOAD_PERFORMANCE_EANR_1MIC_Main_TX,
    OM_CPULOAD_PERFORMANCE_AIG_Main_TX,
    OM_CPULOAD_PERFORMANCE_COMP_Main_TX,
    OM_CPULOAD_PERFORMANCE_PREEMP_Main_TX,
    OM_CPULOAD_PERFORMANCE_DEEMP_Main_TX,
    OM_CPULOAD_PERFORMANCE_HPF_Main_TX,
    OM_CPULOAD_PERFORMANCE_ANR_2MIC_TdProcess,
    OM_CPULOAD_PERFORMANCE_AEC_2MIC_Time2Freq,
    OM_CPULOAD_PERFORMANCE_AEC_2MIC_NlpMain,
    OM_CPULOAD_PERFORMANCE_ANR_2MIC_FdProcess,
    OM_CPULOAD_PERFORMANCE_AEC_2MIC_Freq2Time,
    OM_CPULOAD_PERFORMANCE_AEC_Time2Freq_TX,
    OM_CPULOAD_PERFORMANCE_AEC_Freq2Time_TX,
    OM_CPULOAD_PERFORMANCE_AEC_1MIC_NlpMain,
    OM_CPULOAD_PERFORMANCE_AEC_DtdMuteDetect,
    OM_CPULOAD_PERFORMANCE_AEC_AfMain,
    OM_CPULOAD_PERFORMANCE_AEC_Dtd,
    OM_CPULOAD_PERFORMANCE_AEC_DtdLine,
    OM_CPULOAD_PERFORMANCE_AEC_DtdNline,
    OM_CPULOAD_PERFORMANCE_AEC_HF_AfMain,
    OM_CPULOAD_PERFORMANCE_AEC_HF_NlpMain,
    OM_CPULOAD_PERFORMANCE_AEC_UpdateTx,
    OM_CPULOAD_PERFORMANCE_AEC_UpdateRx,

    OM_CPULOAD_PERFORMANCE_RxPp,       //����
    OM_CPULOAD_PERFORMANCE_HPF_Main_RX,
    OM_CPULOAD_PERFORMANCE_ANR_MBDRC_Rx_Main,
    OM_CPULOAD_PERFORMANCE_AIG_Main_RX,
    OM_CPULOAD_PERFORMANCE_COMP_Main_RX,
    OM_CPULOAD_PERFORMANCE_DEVGAIN_Main_RX,
    OM_CPULOAD_PERFORMANCE_ANR_RX_FFTProcess,
    OM_CPULOAD_PERFORMANCE_EANR_1MIC_Main_RX,
    OM_CPULOAD_PERFORMANCE_MBDRC_Main_RX,
    OM_CPULOAD_PERFORMANCE_ANR_RX_IFFTProcess,

    OM_CPULOAD_PERFORMANCE_BUTT

};

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
typedef struct
{
    VOS_UINT32  auwMaxTime[OM_CPULOAD_MAX_FUNC_NUM];             /* �������ʱ�� */
    VOS_UINT32  auwMinTime[OM_CPULOAD_MAX_FUNC_NUM];             /* ��С����ʱ�� */
    VOS_UINT32  auwEnterTS[OM_CPULOAD_MAX_FUNC_NUM];             /* ���뺯��ʱ��ʱ�� */
    VOS_UINT32  auwEnterTimes[OM_CPULOAD_MAX_FUNC_NUM];          /* ���ô��� */
}OM_CPULOAD_COUNT_STRU;

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
extern void OM_CPULOAD_FuncEnter(VOS_UINT32 uwFuncId);
extern void OM_CPULOAD_FuncExit(VOS_UINT32 uwFuncId);
extern void OM_CPULOAD_Init();



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of om_cpuload.h */
