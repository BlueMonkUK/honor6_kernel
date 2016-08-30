/******************************************************************************

                  ��Ȩ���� (C), 2001-2012, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : TafMtaComm.h
  �� �� ��   : ����
  ��    ��   : ��־��
  ��������   : 2012��6��27��
  ����޸�   :
  ��������   : MTAģ�鹫����������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��6��27��
    ��    ��   : Y00213812
    �޸�����   : A-GPS��Ŀ����

******************************************************************************/

#ifndef __TAFMTACOMM_H__
#define __TAFMTACOMM_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "pslog.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#if (VOS_WIN32 == VOS_OS_VER)
#define   MTA_WARNING_LOG(string)                vos_printf("%s \r\n", string)
#define   MTA_WARNING1_LOG(string, para1)        vos_printf("%s %d.\r\n", string, para1)
#define   MTA_WARNING2_LOG(string, para1, para2) vos_printf("%s %d, %d.\r\n", string, para1, para2)

#define   MTA_ERROR_LOG(string)                  vos_printf("%s \r\n", string)
#define   MTA_ERROR1_LOG(string, para1)          vos_printf("%s %d.\r\n", string, para1)
#define   MTA_ERROR2_LOG(string, para1, para2)   vos_printf("%s %d, %d.\r\n", string, para1, para2)
#else
#define   MTA_WARNING_LOG(string)                PS_LOG(UEPS_PID_MTA, 0, PS_PRINT_WARNING,  string)
#define   MTA_WARNING1_LOG(string, para1)        PS_LOG1(UEPS_PID_MTA, 0, PS_PRINT_WARNING,  string, (VOS_INT32)para1)
#define   MTA_WARNING2_LOG(string, para1, para2) PS_LOG2(UEPS_PID_MTA, 0, PS_PRINT_WARNING,  string, (VOS_INT32)para1, (VOS_INT32)para2)

#define   MTA_ERROR_LOG(string)                  PS_LOG(UEPS_PID_MTA, 0, PS_PRINT_ERROR,  string)
#define   MTA_ERROR1_LOG(string, para1)          PS_LOG1(UEPS_PID_MTA, 0, PS_PRINT_ERROR,  string, (VOS_INT32)para1)
#define   MTA_ERROR2_LOG(string, para1, para2)   PS_LOG2(UEPS_PID_MTA, 0, PS_PRINT_ERROR,  string, (VOS_INT32)para1, (VOS_INT32)para2)
#endif

/* �ַ���ת��Ϊ��ֵʱ�����õĺ� */
#define   MTA_UNSIGNED_LONG_FORMER_NINE_DIGIT    (429496729)      /* VOS_UINT32�������ֵ4294967295��ǰ9λ*/
#define   MTA_UNSIGNED_SHORT_FORMER_FOUR_DIGIT   (6553)           /* VOS_UINT16�������ֵ65535��ǰ4λ*/
#define   MTA_UNSIGNED_CHAR_FORMER_TWO_DIGIT     (25)             /* VOS_UINT8�������ֵ255��ǰ2λ*/
#define   MTA_SIGNED_LONG_FORMER_NINE_DIGIT      (214748364)      /* VOS_INT32�������ֵ2147483647��ǰ9λ*/

#define   MTA_UNSIGNED_LONG_LAST_DIGIT           '5'              /* VOS_UINT32�������ֵ4294967295�����1λ*/
#define   MTA_UNSIGNED_SHORT_LAST_DIGIT          '5'              /* VOS_UINT16�������ֵ65535�����1λ*/
#define   MTA_UNSIGNED_CHAR_LAST_DIGIT           '5'              /* VOS_UINT8�������ֵ255�����1λ*/
#define   MTA_SIGNED_LONG_POSITIVE_LAST_DIGIT    '7'              /* VOS_INT32�����������ֵ2147483647�����1λ*/
#define   MTA_SIGNED_LONG_NEGATIVE_LAST_DIGIT    '8'              /* VOS_INT32���͸�����Сֵ-2147483648�����1λ*/

#define   MTA_POSITIVE                           (1)              /* ���� */
#define   MTA_NEGATIVE                           (-1)             /* ���� */
/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/
/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/
/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/
/*****************************************************************************
  7 STRUCT����
*****************************************************************************/
/*****************************************************************************
  8 UNION����
*****************************************************************************/
/*****************************************************************************
  9 OTHERS����
*****************************************************************************/
/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_UINT32 TAF_MTA_Ac2ul(
    VOS_CHAR                           *pcStr,
    VOS_UINT32                          usLen,
    VOS_UINT32                         *pulRtn);
extern VOS_UINT32 TAF_MTA_Ac2us(
    VOS_CHAR                           *pcStr,
    VOS_UINT32                          ulLen,
    VOS_UINT16                         *pusRtn);
extern VOS_UINT32 TAF_MTA_Ac2uc(
    VOS_CHAR                           *pcStr,
    VOS_UINT32                          ulLen,
    VOS_UINT8                          *pucRtn);
extern VOS_UINT32 TAF_MTA_Ac2sl(
    VOS_CHAR                           *pcStr,
    VOS_UINT32                          usLen,
    VOS_INT32                          *plRtn);


#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif


