/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : ucom_stub.c
  �� �� ��   : ����
  ��    ��   : ��ׯ�� 59026
  ��������   : 2011��6��22��
  ����޸�   :
  ��������   : ����׮����ʵ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��6��22��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "ucom_stub.h"
#include "med_drv_mb_hifi.h"

#ifdef _MED_TEST_UT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codec_op_lib.h"
#endif

#ifdef VOS_VENUS_TEST_STUB
#include "venus_stub.h"
#include "om_log.h"
#include "HifiOmInterface_new.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
#if (VOS_CPU_TYPE != VOS_HIFI)
VOS_UCHAR                               g_aucNvStub[0xffff];
VOS_UINT32                              g_auwRegStub[0xffff];
VOS_UINT32                              g_uwSoftIntInfo;
MAILBOX_HEAD_STRU						g_astMailboxHeader[DRV_MAILBOX_ID_BUTT];
VOS_UCHAR                               g_aucMailbox[0xffff];
VOS_UCHAR                               g_aucStubSocpBuff[0x2000];

VOS_UINT32 g_uwDrvMailboxAddrTbl[DRV_MAILBOX_ID_BUTT][DRV_MAILBOX_INFO_NO] =
{
    {(VOS_UINT32)&g_astMailboxHeader[DRV_MAILBOX_ID_HIFI2CCPU], (VOS_UINT32)&g_aucMailbox[0],                           MAILBOX_QUEUE_SIZE(HIFI, CCPU, MSG)},
    {(VOS_UINT32)&g_astMailboxHeader[DRV_MAILBOX_ID_CCPU2HIFI], (VOS_UINT32)&g_aucMailbox[MAILBOX_QUEUE_SIZE(HIFI, CCPU, MSG)],  MAILBOX_QUEUE_SIZE(CCPU, HIFI, MSG)},
    {(VOS_UINT32)&g_astMailboxHeader[DRV_MAILBOX_ID_HIFI2ACPU], (VOS_UINT32)&g_aucMailbox[MAILBOX_QUEUE_SIZE(HIFI, CCPU, MSG)\
                                                                                          +MAILBOX_QUEUE_SIZE(CCPU, HIFI, MSG)], MAILBOX_QUEUE_SIZE(HIFI, ACPU, MSG)},
    {(VOS_UINT32)&g_astMailboxHeader[DRV_MAILBOX_ID_ACPU2HIFI], (VOS_UINT32)&g_aucMailbox[MAILBOX_QUEUE_SIZE(HIFI, CCPU, MSG)\
                                                                                          +MAILBOX_QUEUE_SIZE(CCPU, HIFI, MSG)\
                                                                                          +MAILBOX_QUEUE_SIZE(HIFI, ACPU, MSG)], MAILBOX_QUEUE_SIZE(ACPU, HIFI, MSG)},
	{(VOS_UINT32)&g_astMailboxHeader[DRV_MAILBOX_ID_HIFI2BBE16], (VOS_UINT32)&g_aucMailbox[MAILBOX_QUEUE_SIZE(HIFI, CCPU, MSG)\
                                                                                          +MAILBOX_QUEUE_SIZE(CCPU, HIFI, MSG)\
                                                                                          +MAILBOX_QUEUE_SIZE(HIFI, ACPU, MSG)\
	                                                                                      +MAILBOX_QUEUE_SIZE(ACPU, HIFI, MSG)], MAILBOX_QUEUE_SIZE(HIFI, BBE16, MSG)},
	{(VOS_UINT32)&g_astMailboxHeader[DRV_MAILBOX_ID_BBE162HIFI], (VOS_UINT32)&g_aucMailbox[MAILBOX_QUEUE_SIZE(HIFI, CCPU, MSG)\
                                                                                          +MAILBOX_QUEUE_SIZE(CCPU, HIFI, MSG)\
                                                                                          +MAILBOX_QUEUE_SIZE(HIFI, ACPU, MSG)\
	                                                                                      +MAILBOX_QUEUE_SIZE(ACPU, HIFI, MSG)\
	                                                                                      +MAILBOX_QUEUE_SIZE(HIFI, BBE16, MSG)], MAILBOX_QUEUE_SIZE(BBE16, HIFI, MSG)}};

#endif

#ifdef _MED_TEST_UT
char    g_szMedTstDataPath[] = ".\\data\\";
#endif

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
#ifdef _MED_TEST_UT

FILE *MED_TstOpenFile(char *pscFileName)
{
    char  szFileName[256];
    FILE *pfOpened = NULL;

    /* ��ָ���ļ� */
    strcpy(szFileName, g_szMedTstDataPath);
    strcat(szFileName, pscFileName);
    pfOpened  = fopen(szFileName, "rb");

    if (NULL == pfOpened)
    {
        printf("Failed to open %s\r\n", szFileName);
    }

    return pfOpened;
}
VOS_INT32 MED_TstCmpSn(
                VOS_INT16              *pshwSn1,
                VOS_INT16              *pshwSn2,
                VOS_INT32               swLen,
                VOS_INT16               shwErr,
                VOS_INT16              *pshwMaxErr,
                VOS_INT32              *pswMaxErrCnt)
{
    VOS_INT32 swCnt     = 0;
    VOS_INT16 shwMaxErr = 0;
    VOS_INT16 shwCurErr = 0;
    VOS_INT32 swMaxErrCnt = 0;

    for (swCnt = 0; swCnt < swLen; swCnt++)
    {
        shwCurErr = CODEC_OpSub(pshwSn1[swCnt], pshwSn2[swCnt]);
        shwCurErr = CODEC_OpAbs_s(shwCurErr);

        if (shwCurErr > shwMaxErr)
        {
            shwMaxErr       = shwCurErr;
            swMaxErrCnt     = swCnt;
        }

    }

    if (pshwMaxErr != VOS_NULL)
    {
        if (shwMaxErr > *pshwMaxErr)
        {
            *pshwMaxErr     = shwMaxErr;

            if (pswMaxErrCnt != VOS_NULL)
            {
                *pswMaxErrCnt  = swMaxErrCnt;
            }
        }
    }

    if (shwMaxErr > shwErr)
    {
        return 1;
    }

    return 0;
}
#endif

#ifdef VOS_VENUS_TEST_STUB

VOS_UINT32 DRV_MAILBOX_SendMsg(
                VOS_UINT32                  uwCpuId,
                VOS_UINT32                  uwChannel,
                VOS_UINT32                  uwPriority,
                VOS_UCHAR                  *pucMsg,
                VOS_UINT32                  uwMsgSize)

{
    #if VENUS_ST_OS
    VOS_DEBUG_LOG_CPU_INFO(OS_TEST_FLAG_MSG_INTER_CORE);
    VOS_DEBUG_LOG_CPU_INFO(uwCpuId);
    VOS_DEBUG_LOG_CPU_INFO(uwMsgSize);
    #endif

    VOS_DEBUG_LOG_CPU_INFO_A(pucMsg, uwMsgSize/4);

    return VOS_OK;
}

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
    VOS_UINT32  uwDataLen = 1;
    VOS_INT32   aswValue[4];

    aswValue[0] = enLogId;
    aswValue[1] = swValue1;
    aswValue[2] = swValue2;
    aswValue[3] = swValue3;

    VOS_VenusLogInfo((VOS_UINT32 *)aswValue, 4);

    return UCOM_RET_SUCC;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

