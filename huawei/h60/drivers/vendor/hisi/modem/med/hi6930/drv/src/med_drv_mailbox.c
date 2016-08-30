

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "med_drv_mb_hifi.h"
#include "med_drv_ipc.h"
#include "med_drv_timer_hifi.h"
#include "om_log.h"
#include "ucom_stub.h"
#include "ucom_share.h"
#include "codec_typedefine.h"

#include "om_comm.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    ��ά�ɲ���Ϣ�а�����C�ļ���ź궨��
*****************************************************************************/
/*lint +e413*/
/*lint -e(767)*/
#define THIS_FILE_ID                    OM_FILE_ID_DRV_MAILBOX_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/* ���ݱ�, ��������˳���Ÿ�����ͷ��������׵�ַ�ͳ���, ��ʽΪ:
    ����0������ͷ�׵�ַ,�����׵�ַ,���䳤��
    ����1������ͷ�׵�ַ,�����׵�ַ,���䳤��
    ...
    ����n������ͷ�׵�ַ,�����׵�ַ,���䳤��*/
#if (VOS_CPU_TYPE == VOS_HIFI)

VOS_UINT32 g_uwDrvMailboxAddrTbl[DRV_MAILBOX_ID_BUTT][DRV_MAILBOX_INFO_NO] =
{
    {MAILBOX_HEAD_ADDR(HIFI, CCPU, MSG),
     MAILBOX_QUEUE_ADDR(HIFI, CCPU, MSG),
     MAILBOX_QUEUE_SIZE(HIFI, CCPU, MSG)},                    /* HIFI2CCPU */
    {MAILBOX_HEAD_ADDR(CCPU, HIFI, MSG),
     MAILBOX_QUEUE_ADDR(CCPU, HIFI, MSG),
     MAILBOX_QUEUE_SIZE(CCPU, HIFI, MSG)},                    /* CCPU2HIFI */
    {MAILBOX_HEAD_ADDR(HIFI, ACPU, MSG),
     MAILBOX_QUEUE_ADDR(HIFI, ACPU, MSG),
     MAILBOX_QUEUE_SIZE(HIFI, ACPU, MSG)},                    /* HIFI2ACPU */
    {MAILBOX_HEAD_ADDR(ACPU, HIFI, MSG),
     MAILBOX_QUEUE_ADDR(ACPU, HIFI, MSG),
     MAILBOX_QUEUE_SIZE(ACPU, HIFI, MSG)},                    /* ACPU2HIFI */
    {MAILBOX_HEAD_ADDR(HIFI, BBE16, MSG),
     MAILBOX_QUEUE_ADDR(HIFI, BBE16, MSG),
     MAILBOX_QUEUE_SIZE(HIFI, BBE16, MSG)},                   /* HIFI 2 BBE16 */
    {MAILBOX_HEAD_ADDR(BBE16, HIFI, MSG),
     MAILBOX_QUEUE_ADDR(BBE16, HIFI, MSG),
     MAILBOX_QUEUE_SIZE(BBE16, HIFI, MSG)}                    /* BBE16 2 HIFI */
};

#endif

/* ���ݱ�, ��Ŷ�HiFi���Ը�CPU��Ӧ�ķ��͡���������������, ��ʽΪ:
    HiFi��CPU0�ķ�������������, HiFi��CPU0�Ľ�������������
    HiFi��CPU1�ķ�������������, HiFi��CPU1�Ľ�������������
    ...
    HiFi��CPUn�ķ�������������, HiFi��CPUn�Ľ�������������
    ˵��:RV_MAILBOX_ID_BUTT��ʾ��Ӧ�����䲻����, ������֧�ָ����䣻
         HIFI��AARM������Ϣ��ͨ��������CARMȻ����CARMת����AARM��� */

VOS_UINT16 g_uhwDrvMailboxIdTbl[VOS_CPU_ID_BUTT][DRV_MAILBOX_TXRX_BUTT] =
{
    {DRV_MAILBOX_ID_HIFI2CCPU,          DRV_MAILBOX_ID_CCPU2HIFI},
    {DRV_MAILBOX_ID_HIFI2ACPU,          DRV_MAILBOX_ID_ACPU2HIFI},
    {DRV_MAILBOX_ID_BUTT,               DRV_MAILBOX_ID_BUTT},
    {DRV_MAILBOX_ID_BUTT,               DRV_MAILBOX_ID_BUTT},
    {DRV_MAILBOX_ID_BUTT,               DRV_MAILBOX_ID_BUTT},
    {DRV_MAILBOX_ID_HIFI2BBE16,         DRV_MAILBOX_ID_BBE162HIFI}
};

/* ���ݱ�, ���HiFi���͵���CPU�Ŀ����Ϣ��Ӧ�Ļ����ж���Ϣ, ��ʽΪ:
    CPU0��IPC�ж�Ŀ��CPU ID, HiFi��CPU0�Ŀ����Ϣ֪ͨbitλ
    CPU1��IPC�ж�Ŀ��CPU ID, HiFi��CPU1�Ŀ����Ϣ֪ͨbitλ
    ...
    CPUn��IPC�ж�Ŀ��CPU ID, HiFi��CPUn�Ŀ����Ϣ֪ͨbitλ
    ˵��: DRV_IPC_MAX_INT_NUM��ʾ��Ӧ�Ŀ����Ϣbit������, �����CPU��֧�ֿ����Ϣ��
          HIFI��AARM������Ϣ��ͨ��������CARMȻ����CARMת����AARM��� */
VOS_UINT16 g_uhwDrvMailboxIpcTbl[VOS_CPU_ID_BUTT][DRV_MAILBOX_IPC_INFO_NUM] =
{
    {MAILBOX_CPUID_CCPU,               IPC_CCPU_INT_SRC_HIFI_MSG},
    {MAILBOX_CPUID_ACPU,               IPC_ACPU_INT_SRC_HIFI_MSG},
    {MAILBOX_CPUID_BUTT,               IPC_INT_BUTTOM},
    {MAILBOX_CPUID_BUTT,               IPC_INT_BUTTOM},
    {MAILBOX_CPUID_BUTT,               IPC_INT_BUTTOM},
    {MAILBOX_CPUID_BBE16,              IPC_INT_HIFI_DSP_MBX}
};

/* ���ݱ�, ���HiFi���͵���VOS CPU�Ŀ����Ϣ��Ӧ���ʱ�, ��ʽΪ:
    CPU0��ͨ��0�ʱ࣬ͨ��1�ʱ࣬ͨ��2�ʱ࣬ͨ��0�ʱ࣬
    CPU1��ͨ��0�ʱ࣬ͨ��1�ʱ࣬ͨ��2�ʱ࣬ͨ��0�ʱ࣬
    ...
    CPUn��ͨ��0�ʱ࣬ͨ��1�ʱ࣬ͨ��2�ʱ࣬ͨ��0�ʱ࣬
    ˵��: �ʱ�ΪVOS_NULL_DWORD��ʾ��Ӧ��ͨ���ʱ಻����, �����CPU��֧�ָ�ͨ�������Ϣ�� */
VOS_UINT32 g_uwDrvMailCodeTbl[VOS_CPU_ID_BUTT][DRV_MAILBOX_CHANNEL_BUTT] =
{
    {MAILBOX_MAILCODE_HIFI_TO_CCPU_VOS_MSG_NORMAL,
     MAILBOX_MAILCODE_HIFI_TO_CCPU_VOS_MSG_URGENT,
     VOS_NULL_DWORD,
     VOS_NULL_DWORD,
     VOS_NULL_DWORD,
     VOS_NULL_DWORD},
    {MAILBOX_MAILCODE_HIFI_TO_ACPU_VOS_MSG_NORMAL,
     MAILBOX_MAILCODE_HIFI_TO_ACPU_VOS_MSG_URGENT,
     MAILBOX_MAILCODE_HIFI_TO_ACPU_AUDIO,
     MAILBOX_MAILCODE_HIFI_TO_ACPU_MISC,
     MAILBOX_MAILCODE_HIFI_TO_ACPU_CCORE_RESET_ID,
     MAILBOX_MAILCODE_RESERVED(ACPU, HIFI, MSG)},
    {VOS_NULL_DWORD,
     VOS_NULL_DWORD,
     VOS_NULL_DWORD,
     VOS_NULL_DWORD,
     VOS_NULL_DWORD,
     VOS_NULL_DWORD},
    {VOS_NULL_DWORD,
     VOS_NULL_DWORD,
     VOS_NULL_DWORD,
     VOS_NULL_DWORD,
     VOS_NULL_DWORD,
     VOS_NULL_DWORD},
    {VOS_NULL_DWORD,
     VOS_NULL_DWORD,
     VOS_NULL_DWORD,
     VOS_NULL_DWORD,
     VOS_NULL_DWORD,
     VOS_NULL_DWORD},
    {MAILBOX_MAILCODE_HIFI_TO_BBE16_VOS_MSG_NORMAL,
     MAILBOX_MAILCODE_HIFI_TO_BBE16_VOS_MSG_URGENT,
     VOS_NULL_DWORD,
     VOS_NULL_DWORD,
     VOS_NULL_DWORD,
     VOS_NULL_DWORD}
};

/* �����������Ϣ���к�(SN��)��¼ȫ�ֱ����������кż�¼��DRV_MAILBOX_ID_ENUMö���������� */
VOS_UINT32 g_auwDrvMailboxSeqNum[DRV_MAILBOX_ID_BUTT];

/*  ���µ��־�������ʼ��ֻ�ڼ���ʱ���������µ粻����ʼ�� */
VOS_UINT32 g_uwMbInitFlag = 0x5a5a5a5a;

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_VOID DRV_MAILBOX_Init( VOS_VOID )
{
    VOS_UINT32                             *puwSeqNum = DRV_MAILBOX_GetMailBoxSeqAddr();
    VOS_CPU_SR                              uwCpuSr;
    DRV_MAILBOX_ID_ENUM_UINT16              enMailboxId;
    VOS_UINT32                              uwCpuId;
    VOS_UINT32                              uwMailboxHeadAddr;                  /* ����ͷ�׵�ַ */
    VOS_UINT32                              uwMailboxAddr;
    VOS_UINT32                              uwMailboxLen;
    MAILBOX_HEAD_STRU                      *pstMailboxHead;
    volatile VOS_UINT32                    *puwMbInitFlag = (VOS_UINT32*)UCOM_GetUncachedAddr((VOS_UINT32)&g_uwMbInitFlag);


    /*��ʼ����������Ϣ���кż�¼*/
    UCOM_MemSet(puwSeqNum, 0, sizeof(VOS_UINT32)*DRV_MAILBOX_ID_BUTT);

    /*ע��CCPU��Ϣ����ص�����*/
    DRV_IPC_RegIntSrc(IPC_HIFI_INT_SRC_CCPU_MSG,
                      DRV_MAILBOX_ReceiveMsg,
                      DRV_MAILBOX_GetMailBoxRx(VOS_CPU_ID_CCPU));

    /*ע��ACPU��Ϣ����ص�����*/
    DRV_IPC_RegIntSrc(IPC_HIFI_INT_SRC_ACPU_MSG,
                      DRV_MAILBOX_ReceiveMsg,
                      DRV_MAILBOX_GetMailBoxRx(VOS_CPU_ID_ACPU));

    /*ע��BBE16��Ϣ����ص�����*/
    DRV_IPC_RegIntSrc(IPC_HIFI_INT_SRC_BBE_MSG,
                      DRV_MAILBOX_ReceiveMsg,
                      DRV_MAILBOX_GetMailBoxRx(VOS_CPU_ID_BBE16));

    if(0x55aa55aa == *puwMbInitFlag)
    {
        return;
    }
    else
    {
        *puwMbInitFlag = 0x55aa55aa;
    }

#if (VOS_CPU_TYPE == VOS_HIFI)
    CARM_HIFI_DYN_ADDR_SHARE_STRU  *pstCarmShare    = UCOM_GetCarmShareAddrStru();
    CARM_HIFI_MAILBOX_STRU         *pstCarmShareMB  = &(pstCarmShare->stCarmHifiMB);
    AARM_HIFI_MAILBOX_STRU         *pstAarmShareMB  = &(pstCarmShare->stAarmHifiMB);

    /* HIFI2ACPU �����ַ*/
    DRV_MAILBOX_InitMBHeadAddr(DRV_MAILBOX_ID_HIFI2ACPU,
                pstAarmShareMB->uwHifiAarmHeadAddr);
    DRV_MAILBOX_InitMBAddr(DRV_MAILBOX_ID_HIFI2ACPU,
                pstAarmShareMB->uwHifiAarmBodyAddr);
    DRV_MAILBOX_InitMBLen(DRV_MAILBOX_ID_HIFI2ACPU,
                pstAarmShareMB->uwHifi2AarmMailBoxLen);

    /* ACPU2HIFI �����ַ */
    DRV_MAILBOX_InitMBHeadAddr(DRV_MAILBOX_ID_ACPU2HIFI,
                pstAarmShareMB->uwAarmHifiHeadAddr);
    DRV_MAILBOX_InitMBAddr(DRV_MAILBOX_ID_ACPU2HIFI,
                pstAarmShareMB->uwAarmHifiBodyAddr);
    DRV_MAILBOX_InitMBLen(DRV_MAILBOX_ID_ACPU2HIFI,
                pstAarmShareMB->uwAarm2HifiMailBoxLen);

    /* HIFI2CCPU �����ַ*/
    DRV_MAILBOX_InitMBHeadAddr(DRV_MAILBOX_ID_HIFI2CCPU,
                pstCarmShareMB->uwHifiCarmHeadAddr);
    DRV_MAILBOX_InitMBAddr(DRV_MAILBOX_ID_HIFI2CCPU,
                pstCarmShareMB->uwHifiCarmBodyAddr);
    DRV_MAILBOX_InitMBLen(DRV_MAILBOX_ID_HIFI2CCPU,
                pstCarmShareMB->uwHifi2CarmMailBoxLen);

    /* CCPU2HIFI �����ַ */
    DRV_MAILBOX_InitMBHeadAddr(DRV_MAILBOX_ID_CCPU2HIFI,
                pstCarmShareMB->uwCarmHifiHeadAddr);
    DRV_MAILBOX_InitMBAddr(DRV_MAILBOX_ID_CCPU2HIFI,
                pstCarmShareMB->uwCarmHifiBodyAddr);
    DRV_MAILBOX_InitMBLen(DRV_MAILBOX_ID_CCPU2HIFI,
                pstCarmShareMB->uwCarm2HifiMailBoxLen);

    /* HIFI2BBE �����ַ, ���䳤���ѳ�ʼ�� */
    DRV_MAILBOX_InitMBHeadAddr(DRV_MAILBOX_ID_HIFI2BBE16,
                UCOM_GetUncachedAddr(DRV_MAILBOX_GetMailBoxHeadAddr(DRV_MAILBOX_ID_HIFI2BBE16)));
    DRV_MAILBOX_InitMBAddr(DRV_MAILBOX_ID_HIFI2BBE16,
                UCOM_GetUncachedAddr(DRV_MAILBOX_GetMailBoxAddr(DRV_MAILBOX_ID_HIFI2BBE16)));

    /* BBE2HIFI �����ַ, ���䳤���ѳ�ʼ�� */
    DRV_MAILBOX_InitMBHeadAddr(DRV_MAILBOX_ID_BBE162HIFI,
                UCOM_GetUncachedAddr(DRV_MAILBOX_GetMailBoxHeadAddr(DRV_MAILBOX_ID_BBE162HIFI)));
    DRV_MAILBOX_InitMBAddr(DRV_MAILBOX_ID_BBE162HIFI,
                UCOM_GetUncachedAddr(DRV_MAILBOX_GetMailBoxAddr(DRV_MAILBOX_ID_BBE162HIFI)));

#endif

    /*���жϻ���*/
    uwCpuSr             = VOS_SplIMP();

    /*�Ը��������Ķ�������г�ʼ��(д�����ɶԷ����г�ʼ��)������ֵ�ָ���*/
    for (uwCpuId = 0; uwCpuId < VOS_CPU_ID_BUTT; uwCpuId++)
    {
        /*��ȡ��CPU��Ӧ��д������*/
        enMailboxId = DRV_MAILBOX_GetMailBoxRx(uwCpuId);

        /*�������ŷǷ�(���ݲ�֧��)�򲻽��г�ʼ��*/
        if (DRV_MAILBOX_ID_BUTT != enMailboxId)
        {
            /*��ȡ����ͷ��ַ*/
            uwMailboxHeadAddr   = DRV_MAILBOX_GetMailBoxHeadAddr(enMailboxId);

            /*��ȡ�����ַ*/
            uwMailboxAddr       = DRV_MAILBOX_GetMailBoxAddr(enMailboxId);

            /*��ȡ���䳤�ȣ���λbyte*/
            uwMailboxLen        = DRV_MAILBOX_GetMailBoxLen(enMailboxId);

            /*����������ͷ�ṹ���е�ַ���ݽ���*/
            pstMailboxHead      = (MAILBOX_HEAD_STRU *)uwMailboxHeadAddr;

            /*��ʼ������ͷ*/
            pstMailboxHead->ulProtectWord1  = DRV_MAILBOX_PROTECT1;
            pstMailboxHead->ulProtectWord2  = DRV_MAILBOX_PROTECT2;
            pstMailboxHead->ulFront         = 0;
            pstMailboxHead->ulFrontslice    = 0;
            pstMailboxHead->ulRear          = pstMailboxHead->ulFront;
            pstMailboxHead->ulRearslice     = 0;
            pstMailboxHead->ulProtectWord3  = DRV_MAILBOX_PROTECT1;
            pstMailboxHead->ulProtectWord4  = DRV_MAILBOX_PROTECT2;

            /*��ʼ������*/
            UCOM_MemSet((VOS_UCHAR *)uwMailboxAddr, 0, uwMailboxLen);

            /*��ʼ������ͷ��������*/
            UCOM_RegWr(uwMailboxAddr, DRV_MAILBOX_PROTECT1);
            UCOM_RegWr(uwMailboxAddr + DRV_MAILBOX_PROTECT_LEN, DRV_MAILBOX_PROTECT2);

            /*��ʼ������β��������*/
            UCOM_RegWr((uwMailboxAddr + uwMailboxLen) - (2*DRV_MAILBOX_PROTECT_LEN),
                       DRV_MAILBOX_PROTECT1);
            UCOM_RegWr((uwMailboxAddr + uwMailboxLen) - DRV_MAILBOX_PROTECT_LEN,
                       DRV_MAILBOX_PROTECT2);
        }
    }

    /*�ͷŻ���*/
    VOS_Splx(uwCpuSr);

}


VOS_VOID DRV_MAILBOX_WriteQueue(
                DRV_MAILBOX_QUEUE_STRU *pstQueue,
                VOS_UCHAR              *pucData,
                VOS_UINT32              uwSize)
{
    VOS_UINT32                          uwSizeToBottom;

    /*����дָ��λ�þ��뻷�λ���β������*/
    uwSizeToBottom  = (pstQueue->uwBase + pstQueue->uwLength) - pstQueue->uwFront;

    /*��дָ��໷�λ���β�����ȴ���Ҫд�����ݳ��ȣ���ֱ�ӿ������ݣ�������дָ��*/
    if (uwSizeToBottom > uwSize)
    {
        /*д��pucData��дָ�봦*/
        UCOM_MemCpy((VOS_UCHAR *)pstQueue->uwFront, pucData, uwSize);

        /*����дָ��*/
        pstQueue->uwFront += uwSize;
    }
    else
    {
        /*д��pucDataǰR���ȵ�дָ�봦*/
        UCOM_MemCpy((VOS_UCHAR *)(pstQueue->uwFront), pucData, uwSizeToBottom);

        /*д��pucData+R�����λ�����ʼ��*/
        UCOM_MemCpy((VOS_UCHAR *)(pstQueue->uwBase),
                    pucData + uwSizeToBottom,
                    uwSize - uwSizeToBottom);

        /*����дָ��*/
        pstQueue->uwFront = (pstQueue->uwBase + uwSize) - uwSizeToBottom;
    }
}
VOS_VOID DRV_MAILBOX_ReadQueue(
                DRV_MAILBOX_QUEUE_STRU *pstQueue,
                VOS_UCHAR              *pucData,
                VOS_UINT32              uwSize)
{
    VOS_UINT32                          uwSizeToBottom;

    /*�����ָ��λ�þ��뻷�λ���β������*/
    uwSizeToBottom  = (pstQueue->uwBase + pstQueue->uwLength) - pstQueue->uwRear;

    /*����ָ��໷�λ���β�����ȴ���Ҫд�����ݳ��ȣ���ֱ�ӿ������ݣ������¶�ָ��*/
    if (uwSizeToBottom > uwSize)
    {
        /*����ָ�봦���ݿ�����pucData��*/
        UCOM_MemCpy(pucData, (VOS_UCHAR *)(pstQueue->uwRear), uwSize);

        /*���¶�ָ��*/
        pstQueue->uwRear += uwSize;
    }
    else
    {
        /*����ָ�봦����ǰ����byte������pucData��*/
        UCOM_MemCpy(pucData, (VOS_UCHAR *)(pstQueue->uwRear), uwSizeToBottom);

        /*�ӻ��λ�����ʼ������ʣ�����ݵ�pucData*/
        UCOM_MemCpy(pucData + uwSizeToBottom,
                    (VOS_UCHAR *)(pstQueue->uwBase),
                    uwSize - uwSizeToBottom);

        /*���¶�ָ��*/
        pstQueue->uwRear = (pstQueue->uwBase + uwSize) - uwSizeToBottom;
    }
}
VOS_VOID DRV_MAILBOX_CheckRecMsg(
                VOS_UINT32              uwMailboxId,
                VOS_UINT32              uwTimeStamp,
                MAILBOX_MSG_HEADER    *pstMsgHead)
{
    VOS_UINT32                              uwSeqNum;

    /*��ȡ��ǰ��Ϣ���к�(����һ����Ϣ��Sn��)*/
    uwSeqNum        = DRV_MAILBOX_GetMailBoxSeqNum(uwMailboxId);

    /*��SN�Ų�����(ȥ����CPU���ܳ��ֱַ��µ�����)*/
    if ((  0 != uwSeqNum)
        &&(0 != pstMsgHead->ulSeqNum)
        &&((uwSeqNum + 1) != pstMsgHead->ulSeqNum))
    {
        /*�ϱ��쳣��ĳ������յ�SN�Ų�����*/
        OM_LogError2(OM_MAILBOX_RecieveMsg_MsgSeqNumErr, uwMailboxId, uwSeqNum);
    }

    /*����SN�ż�¼*/
    DRV_MAILBOX_SetMailBoxSeqNum(uwMailboxId, pstMsgHead->ulSeqNum);

    if ( pstMsgHead->ulWriteSlice >= uwTimeStamp)
    {
        uwTimeStamp = pstMsgHead->ulWriteSlice - uwTimeStamp;
    }
    else
    {
        uwTimeStamp = uwTimeStamp + (0xffffffff - pstMsgHead->ulWriteSlice);
    }

    if (uwTimeStamp >= DRV_MAILBOX_MSG_DELAY_THD)
    {
        /*�ϱ��쳣��ĳ������յ���Ϣ����ʱ�䳬��40ms*/
        OM_LogError1(OM_MAILBOX_RecieveMsg_MsgDelayErr, uwMailboxId);
    }

}
VOS_UINT32 DRV_MAILBOX_Write(
                DRV_MAILBOX_ID_ENUM_UINT16  enMailboxId,
                VOS_UINT32                  uwCpuId,
                VOS_UINT32                  uwPriority,
                VOS_UCHAR                  *pucwMsg,
                VOS_UINT32                  uwSize,
                VOS_UINT32                  uwMailCode)
{
    VOS_CPU_SR                              uwCpuSr;
    VOS_UINT32                              uwSeqNum;
    VOS_UINT32                              uwMailboxAddr;                      /* �����д�ռ���׵�ַ,������ͷ�׵�ַ�ޱ�Ȼ��ϵ */
    VOS_UINT32                              uwMailboxLen;                       /* ���䳤�ȣ���������ͷ�����������ֳ��� */
    VOS_UINT32                              uwMailboxHeadAddr;                  /* ����ͷ�׵�ַ */
    VOS_UINT32                              uwSizeLeft;
    VOS_UINT32                              uwMsgHeadLen;
    VOS_UINT32                              uwTimeStamp;
    MAILBOX_HEAD_STRU                      *pstMailboxHead;
    MAILBOX_MSG_HEADER                      stMsgHeader;
    DRV_MAILBOX_QUEUE_STRU                  stQueue;

    /*����������Ϣͷ��ʽ�̶�ΪMAILBOX_MSG_HEADER*/
    uwMsgHeadLen        = sizeof(MAILBOX_MSG_HEADER);

    /*��ȡ���䳤�ȣ�ȥ������ͷ����β�����ĸ�������(��16byte)*/
    uwMailboxLen        = DRV_MAILBOX_GetMailBoxLen(enMailboxId)
                           - (4*DRV_MAILBOX_PROTECT_LEN);

    /*��ȡ�����׵�ַ��ȥ������ͷ������������*/
    uwMailboxAddr       = DRV_MAILBOX_GetMailBoxAddr(enMailboxId)
                           + (2*DRV_MAILBOX_PROTECT_LEN);

    /*��ȡ����ͷ�׵�ַ*/
    uwMailboxHeadAddr   = DRV_MAILBOX_GetMailBoxHeadAddr(enMailboxId);

    /*���жϻ���*/
    uwCpuSr             = VOS_SplIMP();

    /*����������ͷ�ṹ���е�ַ���ݽ���*/
    pstMailboxHead      = (MAILBOX_HEAD_STRU *)uwMailboxHeadAddr;

    /*����������ͷ�����ֲ�ƥ�䣬˵������ͷ���ȣ���״̬�޷��ָ�������λ����*/
    if ((  DRV_MAILBOX_PROTECT1 != pstMailboxHead->ulProtectWord1)
        ||(DRV_MAILBOX_PROTECT1 != pstMailboxHead->ulProtectWord3)
        ||(DRV_MAILBOX_PROTECT2 != pstMailboxHead->ulProtectWord2)
        ||(DRV_MAILBOX_PROTECT2 != pstMailboxHead->ulProtectWord4))
    {
        VOS_Splx(uwCpuSr);

        /*��������ж�,������д����ʱ����ͷ�������쳣*/
        OM_LogError(OM_MAILBOX_SendMsg_MailboxHeadPartitionErr);
        return VOS_ERR;
    }

    /*��¼��ǰϵͳʱ��,��Ϊ�����޸ļ���Ϣͷ��ʱ��*/
    uwTimeStamp         = DRV_TIMER_ReadSysTimeStamp();

    /*��������ʣ��ռ䣬Ϊ�˷�ֹ����ͷβ�ص�ȥ����4byte�ռ�δʹ�ã���λbyte*/
    if (pstMailboxHead->ulRear > pstMailboxHead->ulFront)
    {
        uwSizeLeft      = ((pstMailboxHead->ulRear - pstMailboxHead->ulFront) - 1)
                            *sizeof(VOS_INT32);
    }
    else
    {
        uwSizeLeft      = (uwMailboxLen + (pstMailboxHead->ulRear*sizeof(VOS_INT32)))
                           - ((pstMailboxHead->ulFront + 1)*sizeof(VOS_INT32));
    }

    /*������ʣ��ռ䲻��*/
    if (uwSizeLeft < (uwSize + uwMsgHeadLen))
    {
        /*�ͷŻ���*/
        VOS_Splx(uwCpuSr);

        /*�ϱ��쳣��д����ռ䲻��*/
        OM_LogError1(OM_MAILBOX_SendMsg_MailboxFull, enMailboxId);

        /* ��λ */
        OM_LogFatalAndReset(OM_MAILBOX_SendMsg_MailboxFull);

        return VOS_ERR;
    }

    /*��ȡ����Ϣ��Ӧ��SN*/
    uwSeqNum = DRV_MAILBOX_GetMailBoxSeqNum(enMailboxId);

    /*�����Ϣͷ*/
    stMsgHeader.ulWriteSlice    = uwTimeStamp; /* uwTimeStamp */
    stMsgHeader.ulReadSlice     = 0;           /* ��ʼ��,�ɶ�ȡ����ֵ */
    stMsgHeader.ulPartition     = DRV_MAILBOX_MSGHEAD_NUMBER;
    stMsgHeader.ulPriority      = uwPriority;
    stMsgHeader.ulSeqNum        = uwSeqNum;
    stMsgHeader.ulMsgLength     = uwSize;
    stMsgHeader.ulMailCode      = uwMailCode;

    /*����SN������һ����Ϣʹ��*/
    DRV_MAILBOX_SetMailBoxSeqNum(enMailboxId, uwSeqNum + 1);

    /*��ʼ������״̬��¼��ʱ�ṹ��,д���н�����дָ��*/
    stQueue.uwBase              = uwMailboxAddr;
    stQueue.uwLength            = uwMailboxLen;
    stQueue.uwFront             = stQueue.uwBase + (pstMailboxHead->ulFront*sizeof(VOS_INT32));

    /*����������п�����Ϣͷ�������¶���״̬��¼��ʱ�ṹ��*/
    DRV_MAILBOX_WriteQueue(&stQueue, (VOS_UCHAR*)(&stMsgHeader), uwMsgHeadLen);

    /*����������п�����Ϣ���ݣ������¶���״̬��¼��ʱ�ṹ��*/
    DRV_MAILBOX_WriteQueue(&stQueue, pucwMsg, uwSize);

    /*����дָ�룬������ȡ��Ϊ4Byte*/
    pstMailboxHead->ulFront     = ((stQueue.uwFront - stQueue.uwBase)
                                    + (sizeof(VOS_INT32) - 1))/sizeof(VOS_INT32);

    /*��������ͷдָ��(ͷָ��)ʱ��*/
    pstMailboxHead->ulFrontslice = uwTimeStamp;

    /*�ͷŻ���*/
    VOS_Splx(uwCpuSr);

    return VOS_OK;
}


VOS_UINT32 DRV_MAILBOX_Read(
                IPC_MSG_BLK_STRU       *pstMsgBlk,
                DRV_MAILBOX_QUEUE_STRU *pstQueue)
{
    MsgBlock                               *pstSndOsaMsg;
    VOS_UINT32                              uwMsgPriority;
    VOS_UINT32                              uwCpuId;

    /* �����ʱ������Ӧ���� */
    switch (pstMsgBlk->stMsgHead.ulMailCode)
    {
        case MAILBOX_MAILCODE_ACPU_TO_HIFI_VOS_MSG_NORMAL :
        case MAILBOX_MAILCODE_CCPU_TO_HIFI_VOS_MSG_NORMAL :
        case MAILBOX_MAILCODE_BBE16_TO_HIFI_VOS_MSG_NORMAL:
        {
            uwMsgPriority = VOS_NORMAL_PRIORITY_MSG;

            /* ������Ϣͷ��Ϣ�е�VOSͷ(��Ҫ֪��PID) */
            DRV_MAILBOX_ReadQueue(pstQueue,
                                  (VOS_UCHAR*)&pstMsgBlk->ulSenderCpuId,
                                  sizeof(VOS_MSG_HEADER_STRU));
            break;
        }

        case MAILBOX_MAILCODE_ACPU_TO_HIFI_VOS_MSG_URGENT :
        case MAILBOX_MAILCODE_CCPU_TO_HIFI_VOS_MSG_URGENT :
        case MAILBOX_MAILCODE_BBE16_TO_HIFI_VOS_MSG_URGENT:
        {
            uwMsgPriority = VOS_EMERGENT_PRIORITY_MSG;

            /* ������Ϣͷ��Ϣ�е�VOSͷ(��Ҫ֪��PID) */
            DRV_MAILBOX_ReadQueue(pstQueue,
                                  (VOS_UCHAR*)&pstMsgBlk->ulSenderCpuId,
                                  sizeof(VOS_MSG_HEADER_STRU));
            break;
        }

        case MAILBOX_MAILCODE_ACPU_TO_HIFI_AUDIO :
        case MAILBOX_MAILCODE_ACPU_TO_HIFI_MISC:
        {
            uwMsgPriority = VOS_NORMAL_PRIORITY_MSG;

            /* ����VOS��Ϣ�����������VOSͷ */
            pstMsgBlk->ulSenderCpuId  = (pstMsgBlk->stMsgHead.ulMailCode>>MAILBOX_ID_SRC_CPU_OFFSET)&0xff;
            pstMsgBlk->ulSenderPid    = UEPS_PID_DL;                          /* �˴���дAP��DRV��Ӧ��PID,,���� */
            pstMsgBlk->ulReceiverCpuId= VOS_CPU_ID_MEDDSP;
            pstMsgBlk->ulReceiverPid  = DSP_PID_AUDIO_RT;                     /* ĿǰĬ��Ϊ������ͨ������Ϣ��Ϊ��ʵʱ��Ӧ����Ϣ */
            pstMsgBlk->ulLength       = pstMsgBlk->stMsgHead.ulMsgLength;
            pstMsgBlk->stMsgHead.ulMsgLength += sizeof(VOS_MSG_HEADER_STRU);
            break;
        }
        /* C�˸�λA��֪ͨhifi C�˸�λchannel */
        case MAILBOX_MAILCODE_ACPU_TO_HIFI_CCORE_RESET_ID:
        {
            uwMsgPriority = VOS_NORMAL_PRIORITY_MSG;

            /* ����VOS��Ϣ�����������VOSͷ */
            pstMsgBlk->ulSenderCpuId  = (pstMsgBlk->stMsgHead.ulMailCode>>MAILBOX_ID_SRC_CPU_OFFSET)&0xff;
            pstMsgBlk->ulSenderPid    = UEPS_PID_DL;                            /* �˴���дAP��DRV��Ӧ��PID,,���� */
            pstMsgBlk->ulReceiverCpuId= VOS_CPU_ID_MEDDSP;
            pstMsgBlk->ulReceiverPid  = DSP_PID_VOICE;                          /* ĿǰĬ��Ϊ������ͨ������Ϣ��Ϊ��ʵʱ��Ӧ����Ϣ */
            pstMsgBlk->ulLength       = pstMsgBlk->stMsgHead.ulMsgLength;
            pstMsgBlk->stMsgHead.ulMsgLength += sizeof(VOS_MSG_HEADER_STRU);
            break;
        }
        case MAILBOX_MAILCODE_RESERVED(CCPU, HIFI, MSG) :
        {
            uwCpuId = VOS_CPU_ID_CCPU;

            DRV_MAILBOX_Write(DRV_MAILBOX_GetMailBoxTx(uwCpuId),
                              DRV_MAILBOX_GetTargetCpu(uwCpuId),
                              VOS_NORMAL_PRIORITY_MSG,
                              (VOS_UCHAR *)pstQueue->uwRear,
                              pstMsgBlk->stMsgHead.ulMsgLength,
                              MAILBOX_MAILCODE_RESERVED(HIFI, CCPU, MSG));

            /*��Ŀ��CPU����IPC�ж�*/
            DRV_IPC_TrigInt(DRV_MAILBOX_GetTargetCpu(uwCpuId),
                            DRV_MAILBOX_GetIntrSrc(uwCpuId));

            return VOS_OK;
        }
        case MAILBOX_MAILCODE_RESERVED(ACPU, HIFI, MSG):
        {
            uwCpuId = VOS_CPU_ID_ACPU;

            DRV_MAILBOX_Write(DRV_MAILBOX_GetMailBoxTx(uwCpuId),
                              DRV_MAILBOX_GetTargetCpu(uwCpuId),
                              VOS_NORMAL_PRIORITY_MSG,
                              (VOS_UCHAR *)pstQueue->uwRear,
                              pstMsgBlk->stMsgHead.ulMsgLength,
                              MAILBOX_MAILCODE_RESERVED(HIFI, ACPU, MSG));

            /*��Ŀ��CPU����IPC�ж�*/
            DRV_IPC_TrigInt(DRV_MAILBOX_GetTargetCpu(uwCpuId),
                            DRV_MAILBOX_GetIntrSrc(uwCpuId));

            return VOS_OK;
        }
        default:
        {
            /*�ϱ��쳣��MailCode�쳣*/
            OM_LogError1(OM_MAILBOX_RecieveMsg_MsgChannelIdErr, pstMsgBlk->stMsgHead.ulMailCode);
            return VOS_ERR;
        }

    }


    /*����OSA��Ϣ����pMsg*/
    pstSndOsaMsg    = (MsgBlock*)VOS_AllocMsg(pstMsgBlk->ulSenderPid, pstMsgBlk->ulLength);

    /*���ڴ治��,�ͷŻ���,ֱ�ӷ���*/
    if (VOS_NULL == pstSndOsaMsg)
    {
        OM_LogError(OM_MAILBOX_SendMsg_AllocFail);

        /* ��λ */
        OM_LogFatalAndReset(OM_MAILBOX_SendMsg_AllocFail);

        return VOS_ERR;
    }

    /* ��VOS_HEADER���ݿ�������Ϣ�� */
    UCOM_MemCpy(pstSndOsaMsg,
                (VOS_VOID *)(&(pstMsgBlk->ulSenderCpuId)),
                sizeof(VOS_MSG_HEADER_STRU));

    /*�����������ж�����Ϣ����(����VOSͷ)�������¶���״̬��¼��ʱ�ṹ��*/
    DRV_MAILBOX_ReadQueue(pstQueue,
                          (VOS_UCHAR*)pstSndOsaMsg->aucValue,
                          pstMsgBlk->ulLength);

    /*������Ϣ���ȼ�����OSA��Ϣ*/
    if (VOS_EMERGENT_PRIORITY_MSG == uwMsgPriority)
    {
        return VOS_SendUrgentMsg(pstMsgBlk->ulSenderPid, pstSndOsaMsg, DRV_MAILBOX_CHANNEL_VOS_CH);
    }
    else
    {
        return VOS_SendMsg(pstMsgBlk->ulSenderPid, pstSndOsaMsg, DRV_MAILBOX_CHANNEL_VOS_CH);
    }

}
VOS_VOID DRV_MAILBOX_ReceiveMsg(VOS_UINT32 uwMailboxId)
{
    VOS_CPU_SR                              uwCpuSr;
    VOS_UINT32                              uwMailboxAddr;                      /* �����д�ռ���׵�ַ,������ͷ�׵�ַ�ޱ�Ȼ��ϵ */
    VOS_UINT32                              uwMailboxLen;                       /* ���䳤�ȣ���������ͷ�����������ֳ��� */
    VOS_UINT32                              uwTimeStamp;
    VOS_INT32                               swContentLen;                       /* ����δ������Ϣ���� */
    MAILBOX_HEAD_STRU                      *pstMailboxHead;
    IPC_MSG_BLK_STRU                        stMsgBlk;                           /* ��ʱ��Ϣ��ṹ�� */
    DRV_MAILBOX_QUEUE_STRU                  stQueue;                            /* ��ʱ������Ϣ״̬�ṹ�� */


    /*��ȡ���䳤�ȣ�ȥ������ͷ����β�����ĸ�������(��16byte)*/
    uwMailboxLen    = DRV_MAILBOX_GetMailBoxLen(uwMailboxId)
                       - (4*DRV_MAILBOX_PROTECT_LEN);

    /*��ȡ�����׵�ַ��ȥ������ͷ������������*/
    uwMailboxAddr       = DRV_MAILBOX_GetMailBoxAddr(uwMailboxId)
                           + (2*DRV_MAILBOX_PROTECT_LEN);

    /*���жϻ���*/
    uwCpuSr             = VOS_SplIMP();

    /*����������ͷ��ʽMAILBOX_HEAD_STRU��������ͷ��ַ���ݽ���*/
    pstMailboxHead      = (MAILBOX_HEAD_STRU*)DRV_MAILBOX_GetMailBoxHeadAddr(uwMailboxId);

    /*���������д�ȡ���ݳ��ȣ���λ4byte*/
    if (pstMailboxHead->ulFront >= pstMailboxHead->ulRear)
    {
        swContentLen    = (VOS_INT32)(pstMailboxHead->ulFront - pstMailboxHead->ulRear);
    }
    else
    {
        swContentLen    = (VOS_INT32)((pstMailboxHead->ulFront + (uwMailboxLen/sizeof(VOS_INT32)))
                            - (pstMailboxHead->ulRear));
    }

    /*��ʼ������״̬��¼��ʱ�ṹ��,�����н����Ķ�ָ��*/
    stQueue.uwBase      = uwMailboxAddr;
    stQueue.uwLength    = uwMailboxLen;

    /*����������ͷ�����ֲ�ƥ�䣬˵������ͷ���ȣ���״̬�޷��ָ�������λ����*/
    if ((  DRV_MAILBOX_PROTECT1 != pstMailboxHead->ulProtectWord1)
        ||(DRV_MAILBOX_PROTECT1 != pstMailboxHead->ulProtectWord3)
        ||(DRV_MAILBOX_PROTECT2 != pstMailboxHead->ulProtectWord2)
        ||(DRV_MAILBOX_PROTECT2 != pstMailboxHead->ulProtectWord4))
    {
        VOS_Splx(uwCpuSr);

        /*��������ж�,�����Ƕ�����ʱ����ͷ�������쳣*/
        OM_LogError(OM_MAILBOX_RecieveMsg_MailboxHeadPartitionErr);
        return;
    }

    /*����ȡ���ݳ��ȷ�0�����������ݷǿ�*/
    while (swContentLen > 0)
    {
        /*ȡ���¶�ָ��ֵ*/
        stQueue.uwRear  = (pstMailboxHead->ulRear*sizeof(VOS_INT32)) + stQueue.uwBase;

        /*���������ݶ����ж�����Ϣͷ��Ϣ�������¶���״̬��¼��ʱ�ṹ��*/
        DRV_MAILBOX_ReadQueue(&stQueue,
                      (VOS_UCHAR*)&stMsgBlk,
                      sizeof(MAILBOX_MSG_HEADER));

        /*��ȡ��ǰȫϵͳʱ��*/
        uwTimeStamp     = DRV_TIMER_ReadSysTimeStamp();

        /*��������Ϣ�ָ��ֲ�ƥ�䣬˵����ȡ��Ϣ�����쳣����״̬�޷��ָ�������λ����*/
        if (DRV_MAILBOX_MSGHEAD_NUMBER != stMsgBlk.stMsgHead.ulPartition)
        {
            VOS_Splx(uwCpuSr);

            /*��������ж�,��������Ϣ������쳣*/
            OM_LogFatalAndReset(OM_MAILBOX_RecieveMsg_MsgPartitionErr);
            return;
        }

        /*����������Ϣ�Ƿ�����쳣������SN���Ƿ���������Ϣ����ʱ���Ƿ����*/
        DRV_MAILBOX_CheckRecMsg(uwMailboxId, uwTimeStamp, &(stMsgBlk.stMsgHead));

        /* ��ȡ�ʼ����ݲ�ת��VOS��Ϣ���д��������л��޸�stQueue��ֵ */
        if (VOS_OK != DRV_MAILBOX_Read(&stMsgBlk, &stQueue))
        {
            OM_LogError(OM_MAILBOX_RecieveMsg_MsgReadErr);
        }

        /*���¶�ָ�룬������ȡ��Ϊ4Byte*/
        pstMailboxHead->ulRear      = ((stQueue.uwRear - stQueue.uwBase)
                                        + (sizeof(VOS_INT32) - 1))/sizeof(VOS_INT32);

        /*��������ͷ��ָ��(βָ��)ʱ��*/
        pstMailboxHead->ulRearslice = uwTimeStamp;

        /*���´�ȡ���ݳ���*/
        swContentLen -= (VOS_INT32)(((stMsgBlk.stMsgHead.ulMsgLength + sizeof(MAILBOX_MSG_HEADER))
                            + (sizeof(VOS_INT32) - 1))/sizeof(VOS_INT32));
    }

    /*�ͷŻ���*/
    VOS_Splx(uwCpuSr);

    return;
}

#ifndef VOS_VENUS_TEST_STUB
VOS_UINT32 DRV_MAILBOX_SendMsg(
                VOS_UINT32                  uwCpuId,
                VOS_UINT32                  uwChannel,
                VOS_UINT32                  uwPriority,
                VOS_UCHAR                  *pucMsg,
                VOS_UINT32                  uwMsgSize)
{
    VOS_UINT32                              uwRet;
    DRV_MAILBOX_ID_ENUM_UINT16              enMailboxId;
    VOS_UINT32                              uwMailCode;

    /*��ȡд���������*/
    enMailboxId = DRV_MAILBOX_GetMailBoxTx(uwCpuId);

    /*�������ŷǷ�(���ݲ�֧��),ֱ�ӷ��ش���*/
    if (DRV_MAILBOX_ID_BUTT == enMailboxId)
    {
        return VOS_ERR;
    }

    /* ������ͨVOSͨ������Ϣ��ȥ��VOSͷ��д������ */
    if (DRV_MAILBOX_CHANNEL_VOS_CH != uwChannel)
    {
        uwMsgSize   = uwMsgSize - sizeof(VOS_MSG_HEADER_STRU);
        pucMsg      = pucMsg + sizeof(VOS_MSG_HEADER_STRU);
    }

    uwMailCode = DRV_MAILBOX_GetMailCode(uwCpuId, uwChannel);

    /* д������ */
    uwRet           = DRV_MAILBOX_Write(enMailboxId,
                                        uwCpuId,
                                        uwPriority,
                                        pucMsg,
                                        uwMsgSize,
                                        uwMailCode);
    /*��д��ʧ��,ֱ�ӷ��ش���*/
    if (VOS_ERR == uwRet)
    {
        return VOS_ERR;
    }

    /*��Ŀ��CPU����IPC�ж�*/
    DRV_IPC_TrigInt(DRV_MAILBOX_GetTargetCpu(uwCpuId),
                    DRV_MAILBOX_GetIntrSrc(uwCpuId));

    return VOS_OK;

}
#endif

VOS_VOID DRV_SOCP_Init(VOS_VOID)
{
    VOS_UINT32                          uwStartAddr;
    VOS_UINT32                          uwBufLen         = 0;
    VOS_UINT32                          i;
    VOS_UINT16                          uhwSrcChanId     = DRV_SOCP_SRC_CHAN_ID_HIFI;
    VOS_UINT16                          uhwDestChanId    = DRV_SOCP_DES_CHAN_ID_HIFI;
    VOS_UINT32                          uwResetFlag;

    /* ��ȷ����ʼ��ַ��Buff������8�ֽڶ���� */
    uwStartAddr = DRV_SOCP_CHAN_START_ADDR_HIFI;
    uwBufLen    = DRV_SOCP_CHAN_LEN_HIFI;

    /* ��λͨ��,д����29(��Ӧͨ��29)ֵΪ1 */
    UCOM_RegBitWr(DRV_SOCP_ENC_SRST_REG, uhwSrcChanId, uhwSrcChanId, 1);

    /* �ȴ�ͨ������ */
    for(i=0; i<DRV_SOCP_RESET_TIMECNT; i++)
    {
        uwResetFlag = UCOM_RegBitRd(DRV_SOCP_ENC_SRST_REG, uhwSrcChanId, uhwSrcChanId);
        if(0 == uwResetFlag)
        {
            break;
        }
    }

    /* д����ʼ��ַ��Դbuffer��ʼ��ַ�Ĵ���*/
    UCOM_RegWr(DRV_SOCP_GET_ENCSRC_WPTR(uhwSrcChanId), uwStartAddr);
    UCOM_RegWr(DRV_SOCP_GET_ENCSRC_RPTR(uhwSrcChanId), uwStartAddr);
    UCOM_RegWr(DRV_SOCP_GET_ENCSRC_BUFADDR(uhwSrcChanId),uwStartAddr);
    UCOM_RegWr(DRV_SOCP_GET_ENCSRC_BUFCFG0(uhwSrcChanId), uwBufLen);

    /*����ԴBufferΪ�����洢���ݰ��ṹ*/
    UCOM_RegBitWr(DRV_SOCP_GET_ENCSRC_BUFCFG1(uhwSrcChanId),
                  UCOM_BIT1,
                  UCOM_BIT2,
                  DRV_SOCP_ENCSRC_CHNMODE_CTSPACKET);

    /*����Ŀ��Buffer ID*/
    UCOM_RegBitWr(DRV_SOCP_GET_ENCSRC_BUFCFG1(uhwSrcChanId),
                  UCOM_BIT4,
                  UCOM_BIT7,
                  uhwDestChanId);

    /*����Ϊ������ȼ�����*/
    UCOM_RegBitWr(DRV_SOCP_GET_ENCSRC_BUFCFG1(uhwSrcChanId),
                  UCOM_BIT8,
                  UCOM_BIT9,
                  DRV_SOCP_CHAN_PRIORITY_0);

    /*����ͨ��Bypass��ʹ��*/
    UCOM_RegBitWr(DRV_SOCP_GET_ENCSRC_BUFCFG1(uhwSrcChanId),
                  UCOM_BIT10,
                  UCOM_BIT10,
                  DRV_SOCP_ENCSRC_BYPASS_DISABLE);

    /*���ø�ͨ��ΪGU��ά�ɲ���������*/
    UCOM_RegBitWr(DRV_SOCP_GET_ENCSRC_BUFCFG1(uhwSrcChanId),
                  UCOM_BIT16,
                  UCOM_BIT23,
                  DRV_SOCP_DATA_TYPE_1);

    /* ʹ���ж�*/
    UCOM_RegBitWr(DRV_SOCP_REG_ENC_RAWINT1, uhwSrcChanId, uhwSrcChanId, 1);
    UCOM_RegBitWr(DRV_SOCP_REG_APP_MASK1, uhwSrcChanId, uhwSrcChanId, 0);

    /*����ͨ��*/
    UCOM_RegBitWr(DRV_SOCP_GET_ENCSRC_BUFCFG1(uhwSrcChanId),
                  UCOM_BIT0,
                  UCOM_BIT0,
                  1);

}
VOS_UINT32 DRV_SOCP_Write(UCOM_DATA_BLK_STRU *pstBlk, VOS_UINT32 uwBlkNum)
{
    VOS_CPU_SR                              uwCpuSr;
    VOS_UINT32                              uwBlkCount;
    VOS_UINT32                              uwTotalSize     = 0;                /* һ��SOCP����ʵ�����ݵ��ܳ��ȣ���λbyte*/
    VOS_UINT32                              uwLen           = 0;                /* һ��SOCP����8byte�����ʵ��д�뻺��������ܳ��ȣ���λbyte */
    VOS_UINT32                              uwSizeLeft;                         /* ����ʣ��Ŀռ� */
    VOS_UINT16                              uhwSrcChanId    = DRV_SOCP_SRC_CHAN_ID_HIFI;
    DRV_SOCP_EMPTY_MSG_STRU                 stEmptyMsg;                         /* SOCP��Ϣ����ʱ�ṹ�壬����ʵ����Ϣ���� */
    DRV_MAILBOX_QUEUE_STRU                  stQueue;                            /* ��ʱ����״̬��¼�ṹ�� */

    /*����BLK��Ϊһ��SOCP������,����ʵ�������ܳ���*/
    for (uwBlkCount = 0; uwBlkCount < uwBlkNum; uwBlkCount++)
    {
        uwTotalSize += pstBlk[uwBlkCount].uwSize;
    }

    /*����8bytes�����ܳ���,�˴�ʹ�ú귴�����������,���δ���궨��*/
    uwLen           = (uwTotalSize + 7)&0xfffffff8;

    /*��ȡSOCP�����׵�ַ,����Ϊ��ʱ����״̬�ṹ��*/
    /* ��ǰSOCP���洦��HIFI��ϵͳ��Cache���ʵ�ַ��Χ���˴���Ҫ�ƶ���Uncache����,��ͬ */
    stQueue.uwBase  = UCOM_RegRd(DRV_SOCP_GET_ENCSRC_BUFADDR(uhwSrcChanId));

    /*��ȡSOCP���泤��(��25λ),��λbyte,����Ϊ��ʱ����״̬�ṹ��*/
    stQueue.uwLength= UCOM_RegRd(DRV_SOCP_GET_ENCSRC_BUFCFG0(uhwSrcChanId))&0x1ffffff;

    /*���жϻ���*/
    uwCpuSr         = VOS_SplIMP();

    /*��ȡSOCP����д(ͷ)ָ��,����Ϊ��ʱ����״̬�ṹ��*/
    stQueue.uwFront = UCOM_RegRd(DRV_SOCP_GET_ENCSRC_WPTR(uhwSrcChanId));

    /*��ȡSOCP�����(β)ָ��,����Ϊ��ʱ����״̬�ṹ��*/
    stQueue.uwRear  = UCOM_RegRd(DRV_SOCP_GET_ENCSRC_RPTR(uhwSrcChanId));

    /*�����д�뻺�泤��*/
    if (stQueue.uwRear > stQueue.uwFront)
    {
        uwSizeLeft  = (stQueue.uwRear - stQueue.uwFront) - 1;
    }
    else
    {
        uwSizeLeft  = (stQueue.uwLength + stQueue.uwRear) - (stQueue.uwFront + 1);
    }


    /*����д�뻺�治�㣬ֱ�ӷ��ش���*/
    if (uwSizeLeft < (uwLen + sizeof(DRV_SOCP_EMPTY_MSG_STRU)))
    {
        /*�ͷŻ���*/
        VOS_Splx(uwCpuSr);
        return VOS_ERR;
    }

    /*���stEmptyMsgֵ*/
    stEmptyMsg.uwMagicNum               = DRV_SOCP_MAGIC_NUMBER;
    stEmptyMsg.uhwMsgLen                = (VOS_UINT16)uwTotalSize + DRV_SOCP_HEADER_LEN;
    stEmptyMsg.stFrmHead.ucCpuId        = DRV_SOCP_HIFI_SESSION_ID;
    stEmptyMsg.uwUsbHead                = DRV_SOCP_USBHEAD_NUMBER;
    stEmptyMsg.stFrmSegHead.uhwSegLen   = (VOS_UINT16)uwTotalSize
                                           + sizeof(stEmptyMsg.stFrmSegHead);
    stEmptyMsg.stFrmSegHead.ucSegNum    = DRV_SOCP_SEG_NUM_ONE;                 /*���ֶΣ��̶���1*/
    stEmptyMsg.stFrmSegHead.ucSegSn     = DRV_SOCP_SEG_SN_ONE;                  /*1��ʾ��ǰ�����Ϊ1*/
    stEmptyMsg.uwUsbTail                = DRV_SOCP_USBTAIL_NUMBER;

    /*д����Ϣ��ͷ����Ϣ*/
    DRV_MAILBOX_WriteQueue(&stQueue,
                           (VOS_UCHAR*)&stEmptyMsg,
                           sizeof(stEmptyMsg) - sizeof(stEmptyMsg.uwUsbTail));

    for (uwBlkCount = 0; uwBlkCount < uwBlkNum; uwBlkCount++)
    {
        /*���BLKд����������*/
        DRV_MAILBOX_WriteQueue(&stQueue, pstBlk[uwBlkCount].pucData, pstBlk[uwBlkCount].uwSize);
    }

    /*д����Ϣ�������*/
    DRV_MAILBOX_WriteQueue(&stQueue,
                           (VOS_UCHAR*)&stEmptyMsg.uwUsbTail,
                           sizeof(stEmptyMsg.uwUsbTail));

    /*��дָ�����8byte���϶���*/
    stQueue.uwFront = (stQueue.uwFront+ 7)&0xfffffff8;

    /*��дָ������λ��ԴBufferβ��������תȦΪBufferͷ��*/
    if (stQueue.uwFront == (stQueue.uwBase + stQueue.uwLength))
    {
        stQueue.uwFront = stQueue.uwBase;
    }

    /*��ӳ���ַת��Ϊ���ߵ�ַ�󣬸���stQueueдָ�뵽DRV_SOCP_ENCSRC_WPTR_REG*/
    UCOM_RegWr(DRV_SOCP_GET_ENCSRC_WPTR(uhwSrcChanId), stQueue.uwFront);

    /*�ͷŻ���*/
    VOS_Splx(uwCpuSr);

    return VOS_OK;
}


VOS_VOID DRV_SOCP_Stop(VOS_VOID)
{
    VOS_UINT16                          uhwSrcChanId     = DRV_SOCP_SRC_CHAN_ID_HIFI;

    /*ֹͣͨ��*/
    UCOM_RegBitWr(DRV_SOCP_GET_ENCSRC_BUFCFG1(uhwSrcChanId), UCOM_BIT0, UCOM_BIT0, 0);

}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

