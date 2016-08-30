/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : ucom_comm.h
  �� �� ��   : ����
  ��    ��   : �³�Ⱥ
  ��������   : 2011��5��31��
  ����޸�   :
  ��������   : ucom_comm.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��5��31��
    ��    ��   : �³�Ⱥ
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "ucom_config.h"
#include "ucom_stub.h"  /* ���Դ�׮ */
#include <string.h>

#ifndef __UCOM_COMM_H__
#define __UCOM_COMM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  2 �궨��
*****************************************************************************/

#if (VOS_CPU_TYPE == VOS_HIFI)

/*****************************************************************************
 �� �� ��  : UCOM_SECTION
 ��������  : Ϊ��������ı���������ߺ�������ָ������.
 �������  : szSecName - �ַ���,����,�淶Ϊ.ģ��.����.��׺
                         ����, ��׺ȡֵΪdata/text/rodata/bss
                         szSecName ȡֵ����".osa_stack.bss"
 ʹ��ʾ��  :
             (1)ȫ�ֱ���, UCOM_SECTION�����ڱ�������ǰ��, Ϊ��֤�����Ķ�ʱSI��
             ����������, ��������:
                 UCOM_SECTION(".osa_stack.bss")
                 VOS_UINT32 g_auwOsaTaskStack[10];

             (2)����, UCOM_SECTION�����ں���ʵ��ǰ��, Ϊ��֤�����Ķ�ʱSI�ܹ���
             ������, ��������:
                 #define OSA_SEC_INTR   UCOM_SECTION(".osa_intr.text")
                 OSA_SEC_INTR
                 void interrupt1(void)
                 {
                    ......
                 }
*****************************************************************************/
#define UCOM_SECTION(szSecName)         __attribute__((section(szSecName)))

/*****************************************************************************
 �� �� ��  : UCOM_ALIGN
 ��������  : Ϊ��������ı�������ָ��������ֽ���.
 �������  : bytes - Ҫ��ָ��������ֽ���
 ʹ��ʾ��  :
             UCOM_ALIGN�����ڱ�������ǰ��, Ϊ��֤�����Ķ�ʱSI�ܹ���������,
             ��������:
                 UCOM_ALIGN(32)
                 VOS_UINT32 g_auwOsaTaskStack[10];
*****************************************************************************/
#define UCOM_ALIGN(bytes)               __attribute__((aligned(bytes)))

#define UCOM_INLINE                     static __inline__

/*ת����32���ض�����*/
#define UCOM_RegRd(uwAddr)              (*((volatile VOS_UINT32 *)(uwAddr)))

/*ת����32����д����*/
#define UCOM_RegWr(uwAddr, uwValue)     (*((volatile VOS_UINT32 *)(uwAddr)) = uwValue)

#else

#define UCOM_SECTION(szSecName)
#define UCOM_ALIGN(bytes)
#define UCOM_INLINE                     __inline

/*ת����32���ض�����*/
#define UCOM_RegRd(uwAddr)              /*lint -e(778)*/(g_auwRegStub[(uwAddr)&0xffff])

/*ת����32����д����*/
#define UCOM_RegWr(uwAddr, uwValue)     /*lint -e(778)*/(g_auwRegStub[(uwAddr)&0xffff] = uwValue)

#endif /* #if (VOS_CPU_TYPE == VOS_HIFI) */

/*****************************************************************************
 �� �� ��  : UCOM_COMM_CycSub
 ��������  : ���Ǹ�����Χ��ѭ��, ��������������Ĳ�
 �������  : Cur    - ������
             Pre    - ����
             CycLen - ���ζ������Կռ�ĳ���
 �������  : ��
 �� �� ֵ  : (Cur - Pre) ����(CycLen - Pre + Cur)
*****************************************************************************/
#define UCOM_COMM_CycSub(Cur, Pre, CycLen)                    \
    (((Cur) < (Pre)) ? (((CycLen) - (Pre)) + (Cur)) : ((Cur) - (Pre)))


/*ת���尴���ض�����*/
#define UCOM_RegBitRd(uwAddr, uhwStartBit, uhwEndBit)               \
    UCOM_COMM_RegBitRd(uwAddr, uhwStartBit, uhwEndBit)

/*ת���尴����д����*/
#define UCOM_RegBitWr(uwAddr, uhwStartBit, uhwEndBit, swContent)    \
    UCOM_COMM_RegBitWr(uwAddr, uhwStartBit, uhwEndBit, swContent)

/*ͨ��VOSͨ������OSA��Ϣ*/
#define UCOM_SendOsaMsg(uwSenderPid, uwReceiverPid, pvMsg, uhwLen)  \
    UCOM_COMM_SendMsg((uwSenderPid), (uwReceiverPid), DRV_MAILBOX_CHANNEL_VOS_CH, (((MsgBlock *)pvMsg)->aucValue), (uhwLen - VOS_MSG_HEAD_LENGTH))

/*ͨ����Ƶר��ͨ�����Ϳ����Ϣ*/
#define UCOM_SendAudioMsg(uwSenderPid, uwReceiverPid, pvMsg, uhwLen) \
    UCOM_COMM_SendMsg((uwSenderPid), (uwReceiverPid), DRV_MAILBOX_CHANNEL_AUDIO_CH, (pvMsg), (uhwLen))

/*ͨ��AP��hifiר��ͨ�����Ϳ����Ϣ*/
#define UCOM_SendDrvMsg(uwSenderPid, uwReceiverPid, pvMsg, uhwLen) \
    UCOM_COMM_SendMsg((uwSenderPid), (uwReceiverPid), DRV_MAILBOX_CHANNEL_DRV_CH, (pvMsg), (uhwLen))

/*ͨ��DSPר��ͨ�����Ϳ����Ϣ*/
#define UCOM_SendDspMsg(uwSenderPid, uwReceiverPid, pvMsg, uhwLen)  \
    UCOM_COMM_SendMsg((uwSenderPid), (uwReceiverPid), DRV_MAILBOX_CHANNEL_DSP_CH, (pvMsg), (uhwLen))

/*ͨ������ͨ�����Ϳ����Ϣ*/
#define UCOM_SendTestMsg(uwSenderPid, uwReceiverPid, pvMsg, uhwLen) \
    UCOM_COMM_SendMsg((uwSenderPid), (uwReceiverPid), DRV_MAILBOX_CHANNEL_AP_DEFAULT_CH, (pvMsg), (uhwLen))

/*ʹ�ú���Ƴ����ڴ��������Ϊ�⺯�����Զ��庯��*/
#ifdef _UCOM_USE_STD_LIB

#define UCOM_MemCpy(pvDes, pvSrc, uwCount) /*lint -e(419,420)*/\
    memcpy((pvDes), (const void *)(pvSrc), (uwCount))

#define UCOM_MemSet(pvDes, ucData, uwCount)     \
    memset((pvDes), (int)(ucData), (uwCount))

#else

#define UCOM_MemCpy(pvDes, pvSrc, uwCount)      \
    UCOM_COMM_CopyMem((pvDes), (pvSrc), (uwCount))

#define UCOM_MemSet(pvDes, ucData, uwCount)     \
    UCOM_COMM_SetMem((pvDes), (ucData), (uwCount))

#endif /* #ifdef _UCOM_USE_STD_LIB */

#define UCOM_COMM_UINT32_MAX            (0xffffffff)        /*32bit�޷��������ֵ*/

/* ���º궨����Ҫ��PIDȷ�������ת���� */
#define UCOM_PID_PS_NAS                 WUEPS_PID_VC
#define UCOM_PID_PS_WTTF                WUEPS_PID_MAC
#define UCOM_PID_PS_OM                  WUEPS_PID_OM
#define UCOM_PID_PS_TDTTF               TPS_PID_MAC
#define UCOM_PID_DSP_GPHY               DSP_PID_GPHY
#define UCOM_PID_DSP_WPHY               DSP_PID_WPHY
#define UCOM_PID_DSP_TDPHY              DSP_PID_TDPHY
#define UCOM_PID_DSP_IMSA               PS_PID_IMSVA

#if (VOS_CPU_TYPE == VOS_HIFI)
/* ����V7R2����ַӳ�估��ʵ��ַ����
   0x50000000 - 0x5fffffff : ʵ�ʵ�ַ; cached   address
   0xf0000000 - 0xffffffff : ӳ���ַ; uncached address
   ��ַ��ӳ���ϵ��reset.S������  */
#define UCOM_MEMMAP_OFFSET              ((-1)*(0xa0000000))                     /* ��ַӳ��ƫ�ƣ�HIFI��ͨ����ַӳ�䷽ʽʵ��cache��ַ��Χ��uncache���� */
#define UCOM_CACHED2PHY_OFFSET          (0x00000000)                            /* cached�ĵ�ַ����ӳ���ַ(�����ַ)��ƫ�� */
#else
#define UCOM_MEMMAP_OFFSET              (0x00000000)                            /* ��ַӳ��ƫ�ƣ���HIFIƽ̨���趨������cache��ַ��uncacheӳ�� */
#define UCOM_CACHED2PHY_OFFSET          (0x00000000)                            /* cached�ĵ�ַ����ӳ���ַ(�����ַ)��ƫ�� */
#endif

#define UCOM_SEC_TCMBSS                 UCOM_SECTION(".dram0.bss")              /* ָ��TCM bss�� */
#define UCOM_SEC_TCMDATA                UCOM_SECTION(".dram0.data")             /* ָ��TCM data�� */
#define UCOM_SEC_TCMTEXT                UCOM_SECTION(".dram0.text")             /* ָ��TCM text�� */
#define UCOM_SET_SRAMSHARE              UCOM_SECTION(".sram.shareaddr")         /* ָ��SRAM��ַ����� */
#define UCOM_SET_UNINIT                 UCOM_SECTION(".sram.uninit")            /* ָ������ʼ���� */

#define UCOM_PROTECT_WORD1              (0x5A5A5A5A)
#define UCOM_PROTECT_WORD2              (0x5A5A5A5A)
#define UCOM_PROTECT_WORD3              (0xA5A5A5A5)
#define UCOM_PROTECT_WORD4              (0xA5A5A5A5)

#define UCOM_HIFI_FREQ                  (360000)                                /* Hifi��Ƶ360MHz */
#define UCOM_COMM_LOOP_CLOCK_PERIOD     ( 1 )                                   /* ѭ����������ʱ������  */
/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
enum UCOM_BIT_ENUM
{
    UCOM_BIT0 = 0,
    UCOM_BIT1 ,
    UCOM_BIT2 ,
    UCOM_BIT3 ,
    UCOM_BIT4 ,
    UCOM_BIT5 ,
    UCOM_BIT6 ,
    UCOM_BIT7 ,
    UCOM_BIT8 ,
    UCOM_BIT9 ,
    UCOM_BIT10,
    UCOM_BIT11,
    UCOM_BIT12,
    UCOM_BIT13,
    UCOM_BIT14,
    UCOM_BIT15,
    UCOM_BIT16,
    UCOM_BIT17,
    UCOM_BIT18,
    UCOM_BIT19,
    UCOM_BIT20,
    UCOM_BIT21,
    UCOM_BIT22,
    UCOM_BIT23,
    UCOM_BIT24,
    UCOM_BIT25,
    UCOM_BIT26,
    UCOM_BIT27,
    UCOM_BIT28,
    UCOM_BIT29,
    UCOM_BIT30,
    UCOM_BIT31,
    UCOM_BIT_BUTT
};
/*****************************************************************************
 ʵ �� ��  : DRV_MAILBOX_CHANNEL_ENUM
 ��������  : ����˼�ͨѶ����������
*****************************************************************************/
enum DRV_MAILBOX_CHANNEL_ENUM
{
   DRV_MAILBOX_CHANNEL_VOS_CH = 0,
   DRV_MAILBOX_CHANNEL_VOS_CH_URGENT = 1,
   DRV_MAILBOX_CHANNEL_AUDIO_CH = 2,
   DRV_MAILBOX_CHANNEL_DSP_CH = 3,
   DRV_MAILBOX_CHANNEL_DRV_CH = 4,
   DRV_MAILBOX_CHANNEL_AP_DEFAULT_CH = 5,
   DRV_MAILBOX_CHANNEL_BUTT
};
typedef unsigned short DRV_MAILBOX_CHANNEL_ENUM_UINT16;
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
 ʵ������  : UCOM_DATA_BLK_STRU
 ��������  : ���ݿ�ṹ
*****************************************************************************/
typedef struct
{
    VOS_UCHAR      *pucData;            /* ���ݿ��ַ  */
    VOS_UINT32      uwSize;             /* pucData���� */
}UCOM_DATA_BLK_STRU;

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

extern VOS_VOID UCOM_COMM_CopyMem(
                       VOS_VOID               *pvDes,
                       VOS_VOID               *pvSrc,
                       VOS_UINT32              uwCount);
extern VOS_VOID UCOM_COMM_IdleHook(VOS_UINT32 ulVosCtxSw);
extern VOS_UINT32 UCOM_COMM_LowFidInit(enum VOS_INIT_PHASE_DEFINE enInitPhrase);
extern VOS_UINT32 UCOM_COMM_NormalFidInit(enum VOS_INIT_PHASE_DEFINE enInitPhrase);
extern VOS_UINT32 UCOM_COMM_RegBitRd(
                       VOS_UINT32              uwAddr,
                       VOS_UINT16              uhwStartBit,
                       VOS_UINT16              uhwEndBit);
extern VOS_VOID UCOM_COMM_RegBitWr(
                       VOS_UINT32              uwAddr,
                       VOS_UINT16              uhwStartBit,
                       VOS_UINT16              uhwEndBit,
                       VOS_UINT32              uwContent);
extern VOS_UINT32 UCOM_COMM_RtFidInit(enum VOS_INIT_PHASE_DEFINE enInitPhrase);
extern VOS_VOID UCOM_COMM_SendMsg(
                       VOS_UINT32              uwSenderPid,
                       VOS_UINT32              uwReceiverPid,
                       VOS_UINT32              uwChannel,
                       VOS_VOID               *pvMsg,
                       VOS_UINT32              uwLen);
extern VOS_VOID UCOM_COMM_SetMem(
                       VOS_VOID               *pvDes,
                       VOS_UCHAR               ucData,
                       VOS_UINT32              uwCount);
extern VOS_VOID UCOM_COMM_SendDspMsg(
                       VOS_UINT32              uwSenderPid,
                       VOS_UINT32              uwReceiverPid,
                       VOS_VOID               *pvMsg,
                       VOS_UINT32              uwLen,
                       VOS_VOID               *pvTail,
                       VOS_UINT32              uwTailLen);
extern VOS_VOID UCOM_COMM_Sleep(VOS_UINT32 ulSleepTimeLen);
extern VOS_VOID UCOM_COMM_GetChnBit16(
                       VOS_VOID               *pvDes,
                       VOS_UINT32              uwSize,
                       VOS_UINT32              uwChnID,
                       VOS_VOID               *pvSrc,
                       VOS_UINT32              uwChnNum);
extern VOS_VOID UCOM_COMM_SetChnBit16(
                       VOS_VOID               *pvDes,
                       VOS_UINT32              uwChnNum,
                       VOS_VOID               *pvSrc,
                       VOS_UINT32              uwSize,
                       VOS_UINT32              uwChnID);

extern VOS_VOID UCOM_COMM_TimeDelayFor( VOS_UINT32 uwTimeLen );
extern VOS_UINT32 UCOM_GetUncachedAddr(VOS_UINT32 uwAddr);
extern VOS_UINT32 UCOM_GetUnmappedAddr(VOS_UINT32 uwAddr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of ucom_comm.h */
