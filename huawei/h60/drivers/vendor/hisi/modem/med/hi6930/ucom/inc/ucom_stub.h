/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : ucom_stub.h
  �� �� ��   : ����
  ��    ��   : ��ׯ�� 59026
  ��������   : 2011��6��22��
  ����޸�   :
  ��������   : ucom_stub.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��6��22��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "drv_mailbox.h"
#include "drv_mailbox_cfg.h"

#ifdef _MED_TEST_UT
#include <stdio.h>
#include <stdlib.h>
#endif

#ifndef __UCOM_STUB_H__
#define __UCOM_STUB_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  2 �궨��
*****************************************************************************/
#if (VOS_CPU_TYPE != VOS_HIFI)
/* UT������Ҫ�Ե�ַ���д�׮ */
#define CODEC_PC_VOICE_OM_CODEC_ADDR        ((VOS_UINT32)&g_auwRegStub[0x0038])
#define CODEC_PC_VOICE_CODEC_OM_ADDR        ((VOS_UINT32)&g_auwRegStub[0x0048])
#define CODEC_PC_VOICE_SET_HOOK           ((VOS_UINT32)&g_auwRegStub[0x0058])

#define UCOM_NV_GetPhyAddr(addr)    (addr)
#define UCOM_NV_MAICNUM_ADDR            ((VOS_UINT32)(&g_aucNvStub[0]))
#define UCOM_NV_DATA_BASE_ADDR          ((VOS_UINT32)(&g_aucNvStub[400]))
#define UCOM_NV_FILEHANDLE_ADDR         ((VOS_UINT32)(&g_aucNvStub[144]))
#define UCOM_NV_CTRLFILE_ADDR           ((VOS_UINT32)(&g_aucNvStub[16]))
#ifndef SHM_MEM_NV_SIZE
#define SHM_MEM_NV_SIZE                 (0x00100000)
#endif
#endif
/* UT����Ĭ�Ͼ߱�PC Voice���� */
/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
#if (VOS_CPU_TYPE != VOS_HIFI)
extern VOS_UCHAR                        g_aucNvStub[0xffff];
extern VOS_UINT32                       g_auwRegStub[0xffff];
extern MAILBOX_HEAD_STRU                g_astMailboxHeader[6];
extern VOS_UCHAR                        g_aucMailbox[0xffff];
extern VOS_UCHAR                        g_aucStubSocpBuff[0x2000];
extern VOS_UINT32 g_uwDrvMailboxAddrTbl[][3];
#endif

/*****************************************************************************
  10 ��������
*****************************************************************************/
#if (VOS_CPU_TYPE != VOS_HIFI)
extern unsigned xthal_get_ccount(void);
extern void     xthal_set_ccompare(int, unsigned);
extern unsigned xthal_get_ccompare(int);
#endif

#ifdef _MED_TEST_UT
extern FILE *MED_TstOpenFile(char *pscFileName);
extern VOS_INT32 MED_TstCmpSn(
                VOS_INT16              *pshwSn1,
                VOS_INT16              *pshwSn2,
                VOS_INT32               swLen,
                VOS_INT16               shwErr,
                VOS_INT16              *pshwMaxErr,
                VOS_INT32              *pswMaxErrCnt);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of ucom_stub.h */
