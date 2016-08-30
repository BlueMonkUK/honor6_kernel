/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : ImsNic.c
  �� �� ��   : ����
  ��    ��   : caikai
  ��������   : 2013��7��22��
  ����޸�   :
  ��������   : VoLTE������IMS���������Ĵ���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��7��22��
    ��    ��   : caikai
    �޸�����   : �����ļ�

******************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "ImsNic.h"

#if( FEATURE_ON == FEATURE_IMS )
#include "DrvInterface.h"
#include <stdio.h>
#include "stddef.h"

#if (VOS_OS_VER == VOS_WIN32)
#include  "VxWorksStub.h"
#else
#include  "v_sock.h"
#include  "muxLib.h"
#include  "endLib.h"
#endif


/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_IMS_NIC_C

LOCAL STATUS IMS_NIC_Start(IMS_NIC_DEV_STRU *pstImsNicDev);
LOCAL STATUS IMS_NIC_Stop(IMS_NIC_DEV_STRU *pstImsNicDev);
LOCAL STATUS IMS_NIC_Unload(IMS_NIC_DEV_STRU *pstImsNicDev);
LOCAL VOS_INT IMS_NIC_Ioctl(IMS_NIC_DEV_STRU *pstImsNicDev, VOS_INT cmd, caddr_t data);
LOCAL STATUS IMS_NIC_Send(IMS_NIC_DEV_STRU *pstImsNicDev, M_BLK_ID pstBuf);
LOCAL STATUS IMS_NIC_MCastAddrAdd(IMS_NIC_DEV_STRU *pstImsNicDev, VOS_CHAR *pAddress);
LOCAL STATUS IMS_NIC_MCastAddrDel(IMS_NIC_DEV_STRU *pstImsNicDev, VOS_CHAR *pAddress);
LOCAL STATUS IMS_NIC_MCastAddrGet(IMS_NIC_DEV_STRU *pstImsNicDev, MULTI_TABLE *pstMultiTable);
LOCAL STATUS IMS_NIC_PollSend(IMS_NIC_DEV_STRU *pstImsNicDev, M_BLK_ID pstBuf);
LOCAL STATUS IMS_NIC_PollRcv(IMS_NIC_DEV_STRU *pstImsNicDev, M_BLK_ID pstBuf);
#if 0
LOCAL M_BLK_ID IMS_NIC_FormAddress(
    M_BLK_ID                            pNBuff,
    M_BLK_ID                            pSrcAddr,
    M_BLK_ID                            pDstAddr,
    BOOL                                bcastFlag
);
LOCAL STATUS IMS_NIC_PacketDataGet(M_BLK_ID pMblk, LL_HDR_INFO *pLinkHdrInfo);
LOCAL STATUS IMS_NIC_AddrGet(
    M_BLK_ID                  pMblk,
    M_BLK_ID                  pSrc,
    M_BLK_ID                  pDst,
    M_BLK_ID                  pESrc,
    M_BLK_ID                  pEDst
);
#endif

#ifdef  ST_IMS_NIC
extern VOS_VOID  ST_IMS_NIC_InitQ(VOS_VOID);
extern VOS_UINT32  ST_IMS_NIC_PrintData( VOS_UINT8 *pData, VOS_UINT16 usLen );
#endif

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
IMS_NIC_ENTITY_STRU                     g_astImsNicEntity[IMS_NIC_MODEM_ID_BUTT] = {{0}};  /* IMS��������ʵ����Ϣ */

const IMS_NIC_MANAGE_TBL_STRU           g_stImsNicManageTbl    =
{
    {
        (FUNCPTR) IMS_NIC_Start,                                                     /* Function to start the device. */
        (FUNCPTR) IMS_NIC_Stop,                                                      /* Function to stop the device. */
        (FUNCPTR) IMS_NIC_Unload,                                                    /* Unloading function for the driver. */
        (FUNCPTR) IMS_NIC_Ioctl,                                                     /* Ioctl function for the driver. */
        (FUNCPTR) IMS_NIC_Send,                                                      /* Send function for the driver. */
        (FUNCPTR) IMS_NIC_MCastAddrAdd,                                              /* Multicast add function for the driver. */
        (FUNCPTR) IMS_NIC_MCastAddrDel,                                              /* Multicast delete function for the driver. */
        (FUNCPTR) IMS_NIC_MCastAddrGet,                                              /* Multicast retrieve function for the driver. */
        (FUNCPTR) IMS_NIC_PollSend,                                                  /* Polling send function */
        (FUNCPTR) IMS_NIC_PollRcv,                                                   /* Polling receive function */
        endEtherAddressForm,                                                         /* put address info into a NET_BUFFER */
        endEtherPacketDataGet,                                                       /* get pointer to data in NET_BUFFER */
        endEtherPacketAddrGet                                                        /* Get packet addresses. */
    },
    "ims_nic",
    {
        {
            {
                {
                    0x58, 0x02, 0x03, 0x04, 0x05, 0x06
                },
                {
                    0x00, 0x11, 0x09, 0x64, 0x01, 0x01
                },
                0x0008
            },
            {
                {
                    0x58, 0x02, 0x03, 0x04, 0x05, 0x06
                },
                {
                    0x00, 0x11, 0x09, 0x64, 0x01, 0x01
                },
                0xDD86
            }
        }
    },
    {
        "ipdnsc.primaryns",
        "ipdnsc.secondaryns",
        "ipdnsc.tertiaryns",
        "ipdnsc.quaternaryns"
    }
};

IMS_NIC_CTX_STRU    g_stImsNicCtx   =
{
    IMS_NIC_MTU_DEFAULT_VALUE,
    0,
    0,
    {
        VOS_NULL_PTR
    }
};

IMS_NIC_DEV_STRU                    g_astImsNicDev[IMS_NIC_MODEM_ID_BUTT];                  /* ���������豸�ռ� */

/* ����ʱʹ�õĴ�ӡ���� */
VOS_UINT8       g_ucImsNicDebugPrint    = 0;

/* ���Դ�ӡ */
#define IMS_NIC_DEBUG_PRINT(...)\
        {\
            if ( 1 ==  g_ucImsNicDebugPrint)\
            {\
                vos_printf(__VA_ARGS__);\
            }\
        }

#define         IMS_NIC_OK              (0)
#define         IMS_NIC_ERR             (1)
#define         IMS_NIC_ERROR           (-1)
#define         IMS_NIC_INVAL           (EINVAL)    /* �����Ч */
#define         IMS_NIC_NOTSUP          (ENOTSUP)   /* ��֧�� */


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

VOS_VOID IMS_NIC_MNTN_InitLogCnt(IMS_NIC_LOG_ENT_STRU *pstLogEnt)
{
    if(VOS_OK != VOS_SmP(g_stImsNicCtx.ulInitLogMutexSem, 0))
    {
        return;
    }

    pstLogEnt->ulCnt = 0;

    VOS_SmV(g_stImsNicCtx.ulInitLogMutexSem);

    return;
}


VOS_VOID IMS_NIC_MNTN_LogOutput(IMS_NIC_LOG_ENT_STRU *pstLogEnt, VOS_UINT32 ulPid)
{
    IMS_NIC_TRACE_LOG_MSG_STRU          stImsTraceLogMsg;


    /*  ����ݹ���ѭ�� */
    if( (pstLogEnt->ulCnt > IMS_NIC_LOG_RECORD_MAX_NUM) || (0 == pstLogEnt->ulCnt) )
    {
        IMS_NIC_MNTN_InitLogCnt(pstLogEnt);

        return;
    }

    stImsTraceLogMsg.ulSenderCpuId     = VOS_LOCAL_CPUID;
    stImsTraceLogMsg.ulSenderPid       = ulPid;
    stImsTraceLogMsg.ulReceiverCpuId   = VOS_LOCAL_CPUID;
    stImsTraceLogMsg.ulReceiverPid     = ulPid;
    stImsTraceLogMsg.ulLength          = (sizeof(IMS_NIC_TRACE_LOG_MSG_STRU) - VOS_MSG_HEAD_LENGTH);

    stImsTraceLogMsg.usMsgType         = ID_IMS_NIC_MNTN_TRACE_LOG_MSG;
    stImsTraceLogMsg.usTransId         = 0;

    DRV_RT_MEMCPY( &stImsTraceLogMsg.stLogMsgCont, pstLogEnt, sizeof(IMS_NIC_LOG_ENT_STRU) );

    OM_TraceMsgHook(&stImsTraceLogMsg);

    IMS_NIC_MNTN_InitLogCnt(pstLogEnt);

    return ;
}
VOS_VOID IMS_NIC_MNTN_LogSave
(
    IMS_NIC_LOG_ENT_STRU           *pstLogEnt,
    VOS_UINT32                      ulPid,
    VOS_UINT32                      enLevel,
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


    /* Ϊ����SDT�����TimeStampһ�£�����SDT�Ļ��㷽��:(0xFFFFFFFF - OM_GetSlice())%32768*100����Slice���л��� */
    ulCurrentSlice  = OM_GetSlice();
    /* SliceΪ�ݼ�ʱ��ulCurrentSlice  = 0xFFFFFFFF - ulCurrentSlice������ʱ�����ü� */

    ulTimeStampTmp  = ulCurrentSlice & 0x7FFF;
    ulTimeStampTmp  = ulTimeStampTmp * 100;
    ulTimeStampTmp  = ulTimeStampTmp >> 15;

    ulTimeStamp     = ulCurrentSlice >> 15;
    ulTimeStamp     = ulTimeStamp * 100;
    ulTimeStamp     = ulTimeStamp + ulTimeStampTmp;

    if(VOS_OK != VOS_SmP(g_stImsNicCtx.ulSaveLogMutexSem, 0))
    {
        return;
    }

    ulCnt           = pstLogEnt->ulCnt;

    if( ulCnt < IMS_NIC_LOG_RECORD_MAX_NUM )
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

    VOS_SmV(g_stImsNicCtx.ulSaveLogMutexSem);

    /* ��¼���ˣ��Զ����� */
    if( ulCnt >= IMS_NIC_LOG_RECORD_MAX_NUM )
    {
        IMS_NIC_MNTN_LogOutput(pstLogEnt, ulPid);
    }

    return;
}
VOS_UINT32 IMS_NIC_MNTN_LogEntInit(VOS_UINT32 ulPid)
{
    VOS_UINT32      ulResult;


    PS_MEM_SET(&g_stImsNicCtx.stLogEnt, 0, sizeof(IMS_NIC_LOG_ENT_STRU));

    ulResult    = VOS_SmMCreate("INIT", VOS_SEMA4_FIFO,
                    (VOS_UINT32 *)(&g_stImsNicCtx.ulInitLogMutexSem));

    if ( VOS_OK != ulResult )
    {
        IMS_NIC_INIT_LOG_PRINT1(ulPid, "IMS_NIC_MNTN_CreateMutexSem: Create g_ulInitLogMutexSem fail!", ulResult);

        return VOS_ERR;
    }

    ulResult    = VOS_SmMCreate("SAVE", VOS_SEMA4_FIFO,
                    (VOS_UINT32 *)(&g_stImsNicCtx.ulSaveLogMutexSem));

    if ( VOS_OK != ulResult )
    {
        IMS_NIC_INIT_LOG_PRINT1(ulPid, "IMS_NIC_MNTN_CreateMutexSem: Create g_ulLogSaveMutexSem fail!", ulResult);

        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_VOID IMS_NIC_MntnTrace(VOS_UINT16 usMsgId, VOS_UINT8 *pucData, VOS_UINT32 ulLen, VOS_UINT32 ulPara1, VOS_UINT32 ulPara2)
{
    IMS_NIC_MNTN_TRACE_MSG_STRU *pstMntnTrace;
    VOS_UINT32                   ulMsgLen;

    ulMsgLen = offsetof(IMS_NIC_MNTN_TRACE_MSG_STRU, aucData) + ulLen;

    pstMntnTrace = (IMS_NIC_MNTN_TRACE_MSG_STRU *)PS_MEM_ALLOC(UEPS_PID_IMSNIC, ulMsgLen);

    if (VOS_NULL_PTR == pstMntnTrace)
    {
        IMS_NIC_WARNING_LOG(UEPS_PID_IMSNIC, "IMS_NIC_MntnTrace, WARNING, Alloc msg fail.");

        return;
    }

    pstMntnTrace->ulReceiverPid        = UEPS_PID_IMSNIC;
    pstMntnTrace->ulSenderPid          = UEPS_PID_IMSNIC;
    pstMntnTrace->ulSenderCpuId        = VOS_LOCAL_CPUID;
    pstMntnTrace->ulReceiverCpuId      = VOS_LOCAL_CPUID;
    pstMntnTrace->ulLength             = ulMsgLen - VOS_MSG_HEAD_LENGTH;
    pstMntnTrace->usMsgId              = usMsgId;
    pstMntnTrace->ulPara1              = ulPara1;
    pstMntnTrace->ulPara2              = ulPara2;

    if (VOS_NULL_PTR != pucData)
    {
        DRV_RT_MEMCPY(pstMntnTrace->aucData, pucData, ulLen);
    }

    OM_TraceMsgHook(pstMntnTrace);

    PS_MEM_FREE(UEPS_PID_IMSNIC, pstMntnTrace);
}


VOS_VOID IMS_NIC_ResetAll(MODEM_ID_ENUM_UINT16 enModemId)
{
    IMS_NIC_ENTITY_STRU                 *pstImsNicEntity;
    VOS_UINT8                            aucCmd[IMS_NIC_CMD_LEN] = {0};
    VOS_INT                              iLen;


    pstImsNicEntity     = IMS_NIC_GET_ENTITY_STRU(enModemId);

#if 0
    /* ��IPV4·�� */
    iLen = IMS_NIC_IPV4_ROUTE_DEL_CMD(aucCmd, enModemId, IMS_NIC_GET_NIC_NAME);
    IMS_NIC_SET_CMD_END(aucCmd, iLen);
    IMS_NIC_RUN_CMD(aucCmd);

    /* ��IPV6·�� */
    iLen = IMS_NIC_IPV6_ROUTE_DEL_CMD(aucCmd, enModemId, IMS_NIC_GET_NIC_NAME);
    IMS_NIC_SET_CMD_END(aucCmd, iLen);
    IMS_NIC_RUN_CMD(aucCmd);
#endif

    /* down���� */
    iLen = IMS_NIC_DOWN_NIC_CMD(aucCmd, enModemId, IMS_NIC_GET_NIC_NAME);
    IMS_NIC_SET_CMD_END(aucCmd, iLen);
    IMS_NIC_RUN_CMD(aucCmd);

    PS_MEM_SET(&(pstImsNicEntity->stImsNicPdnInfo), 0, sizeof(IMS_NIC_PDN_INFO_STRU));
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_I2A
 ��������  : ����ת������(10��16), ������ת��ΪASCII��, �����������ַ���
 �������  : usValue    - ��ת��ΪASCII�������
             pcStr      - ���������ַ���
             usRadix    - ת������
 �������  : ��
 �� �� ֵ  : VOS_CHAR*
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_CHAR* IMS_NIC_I2A(VOS_UINT16 usValue, VOS_CHAR *pcStr, VOS_UINT16 usRadix)
{
    if (IMS_NIC_TYPE_HEX == usRadix)
    {
        pcStr += VOS_sprintf(pcStr, "%x", usValue);
    }
    else if(IMS_NIC_TYPE_DEC == usRadix)
    {
        pcStr += VOS_sprintf(pcStr, "%d", usValue);
    }
    else
    {
        IMS_NIC_WARNING_LOG1(UEPS_PID_IMSNIC, "IMS_NIC_I2A, WARNING, usRadix is Invalid", usRadix);
    }

    return pcStr;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_FindIpv6AddrZeroFieldsToBeCompressed
 ��������  : �ҳ���Ҫʹ��"::"��ʾ��IP��ַ�ε���ʼλ��
 �������  : ausAddrValue[]     - IPV6��ַ������
             ucTokensNum        - ��ַ�θ���
 �������  : pucZeroFieldStart  - ��ֵ����Ϊ0��IP��ַ����ʼλ��
             pucZeroFieldCount  - ��ֵ����Ϊ0��IP��ַ�θ���
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID IMS_NIC_FindIpv6AddrZeroFieldsToBeCompressed(
    VOS_UINT8                          *pucZeroFieldStart,
    VOS_UINT8                          *pucZeroFieldCount,
    VOS_UINT16                         *pusAddrValue,
    VOS_UINT8                           ucTokensNum
)
{
    VOS_UINT32                          ulIndx;
    VOS_UINT8                           ucStart;
    VOS_UINT8                           ucCount;


    ucStart                             = 0xFF;
    ucCount                             = 0;

    for (ulIndx = 0; ulIndx < (VOS_UINT32)(ucTokensNum - 1); ulIndx++)
    {
        if ( (0x0000 == pusAddrValue[ulIndx])
          && (0x0000 == pusAddrValue[ulIndx + 1]) )
        {
            /* ��¼��ֵ����Ϊ0��IP��ַ����ʼλ�� */
            if (0xFF == ucStart)
            {
                ucStart = (VOS_UINT8)ulIndx;
            }

            /* ������ֵ����Ϊ0��IP��ַ�θ��� */
            ucCount++;
        }
        else
        {
            /* ���´�ѹ����IP��ַ��λ��, �Լ�IP��ַ�θ��� */
            if (0xFF != ucStart)
            {
                if (ucCount > *pucZeroFieldCount)
                {
                    *pucZeroFieldStart = ucStart;
                    *pucZeroFieldCount = ucCount;
                }

                ucStart = 0xFF;
                ucCount = 0;
            }
        }
    }

    /* ��ֵ����Ϊ0��IP��ַ���ڽ�βʱ, ��Ҫ����һ�δ�ѹ����IP��ַ��λ��,
       �Լ�IP��ַ�θ��� */
    if (0xFF != ucStart)
    {
        if (ucCount > *pucZeroFieldCount)
        {
            *pucZeroFieldStart = ucStart;
            *pucZeroFieldCount = ucCount;
        }
    }
}


/*****************************************************************************
 �� �� ��  : IMS_NIC_ConvertIpv6AddrToCompressedStr
 ��������  : ��IPV6��ַ��ʽת��Ϊ�ַ���ѹ����ʽ
 �������  : aucIpAddr[]    - IPV6��ַ(Э���ʽ)
             ucTokensNum    - ��ַ�θ���
 �������  : aucAddrStr[]   - IPV6��ַ(�ַ�����ʽ)
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID IMS_NIC_ConvertIpv6AddrToCompressedStr(
    VOS_UINT8                          *pucAddrStr,
    VOS_UINT8                          *pucIpAddr,
    VOS_UINT8                           ucTokensNum
)
{
    VOS_UINT8                          *pucBuffer;
    VOS_UINT32                          ulIndx;
    VOS_UINT16                          ausAddrValue[IMS_NIC_IPV6_STR_RFC2373_TOKENS]; /* IMS_NIC_IPV6_STR_RFC2373_TOKENS]; */
    VOS_UINT16                          usAddrNum;
    VOS_UINT8                           ucDelimiter;
    VOS_UINT8                           ucRadix;
    VOS_UINT8                           ucZeroFieldStart;
    VOS_UINT8                           ucZeroFieldCount;


    PS_MEM_SET(ausAddrValue, 0x00, sizeof(ausAddrValue));

    pucBuffer                           = pucAddrStr;
    ucDelimiter                         = IMS_NIC_IPV6_STR_DELIMITER;
    ucRadix                             = IMS_NIC_TYPE_HEX;
    ucZeroFieldStart                    = 0xFF;
    ucZeroFieldCount                    = 0;

    /* ����IP�ַ�����ʽ�������, ��ȡ�ֶε�IP��ַ��ֵ */
    for (ulIndx = 0; ulIndx < ucTokensNum; ulIndx++)
    {
        usAddrNum = *pucIpAddr++;

        usAddrNum <<= 8;
        usAddrNum  |= *pucIpAddr++;

        ausAddrValue[ulIndx] = usAddrNum;
    }

    /* �ҳ���Ҫʹ��"::"��ʾ��IP��ַ�ε���ʼλ��  */
    IMS_NIC_FindIpv6AddrZeroFieldsToBeCompressed(&ucZeroFieldStart, &ucZeroFieldCount, ausAddrValue, ucTokensNum);

    /* ����IP��ַ�ֶ�, �����зָ�����ǵ�IP��ַ�ַ��� */
    for (ulIndx = 0; ulIndx < ucTokensNum; ulIndx++)
    {
        if (ucZeroFieldStart == ulIndx)
        {
            *pucBuffer++ = ucDelimiter;

            ulIndx += ucZeroFieldCount;

            /* ����ѵ�IP��ַ�ֶε����һ��, ��Ҫ����ָ��� */
            if ((ucTokensNum - 1) == ulIndx)
            {
                *pucBuffer++ = ucDelimiter;
            }
        }
        else
        {
            /* �����IP��ַ�ֶεĵ�һ��, ����Ҫ����ָ��� */
            if (0 != ulIndx)
            {
                *pucBuffer++ = ucDelimiter;
            }

            pucBuffer    = (VOS_UINT8 *)IMS_NIC_I2A(ausAddrValue[ulIndx], (VOS_CHAR*)pucBuffer, ucRadix);
        }
    }

    /* �����ַ��������� */
    if (pucAddrStr != pucBuffer)
    {
        *pucBuffer = '\0';
    }
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_IsSameIpAddr
 ��������  : ����IPv4,IPv6��ַ�Ա�������ַ�Ƿ���ͬ
 �������  : pucIpAddr1            -- ��Ҫ�Ƚϵĵ�ַ1
             pucIpAddr2            -- ��Ҫ�Ƚϵĵ�ַ2
             ucIpAddrLen           -- IP��ַ����
 �������  : ��
 �� �� ֵ  : ��ȷ���1������ȷ���0
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 IMS_NIC_IsSameIpAddr(VOS_UINT8 *pucIpAddr1, VOS_UINT8 *pucIpAddr2, VOS_UINT8 ucIpAddrLen)
{
    VOS_UINT32 *pulIpAddrTmp1 = (VOS_UINT32 *)pucIpAddr1;
    VOS_UINT32 *pulIpAddrTmp2 = (VOS_UINT32 *)pucIpAddr2;


    if ((VOS_NULL_PTR == pucIpAddr1) || (VOS_NULL_PTR == pucIpAddr2))
    {
        IMS_NIC_ERROR_LOG2(UEPS_PID_IMSNIC, "IMS_NIC_IsSameIpAddr, ERROR, Input para is NULL.", pucIpAddr1, pucIpAddr2);

        return 0;
    }

    if (IMS_NIC_IPV4_ADDR_LENGTH == ucIpAddrLen)
    {
        return (*(pulIpAddrTmp1) == *(pulIpAddrTmp2));
    }
    else
    {
        return !((pulIpAddrTmp1[0]^pulIpAddrTmp2[0]) | (pulIpAddrTmp1[1]^pulIpAddrTmp2[1])
        | (pulIpAddrTmp1[2]^pulIpAddrTmp2[2]) | (pulIpAddrTmp1[3]^pulIpAddrTmp2[3]));
    }
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_DnsAddrCheck
 ��������  : �������õ�DNS��������ַ�Ƿ��Ѿ����ù�
 �������  : pucIpAddr              -- �����õ�DNS������IP
             ulIpAddrLen            -- IP��ַ����
             pstImsNicDnsInfo       -- �ڲ��洢��DNS������Ϣ
 �������  : pucDnsIndx             -- ���ù��򷵻�DNS��������������û���ù��򷵻ؿ����õ�DNS������������
 �� �� ֵ  : ���ù�ΪVOS_TRUE��û�����ù�ΪVOS_FALSE
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 IMS_NIC_DnsAddrCheck (VOS_UINT8 *pucIpAddr, VOS_UINT8 ucIpAddrLen, IMS_NIC_DNS_INFO_STRU *pstDnsInfo, VOS_UINT8 *pucDnsIndx)
{
    VOS_UINT8                           ucIndex;
    VOS_UINT8                           aucIdle[IMS_NIC_MAX_DNS_SERVER_NUM] = {0};
    VOS_UINT8                           ucIdleNum;


    ucIdleNum   = 0;

    if ((VOS_NULL_PTR == pucIpAddr) || (VOS_NULL_PTR == pucDnsIndx) || (VOS_NULL_PTR == pstDnsInfo))
    {
        IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_DnsAddrCheck, ERROR, Input para is NULL.");

        return IMS_NIC_DNS_NOT_CFGED;
    }

    /* ����pstDnsInfo�д洢��DNS��Ϣ */
    for (ucIndex = 0; ucIndex < IMS_NIC_MAX_DNS_SERVER_NUM; ucIndex++)
    {
        if ( pstDnsInfo->astImsNicDnsInfo[ucIndex].cDnsRef > 0)
        {
            if (pstDnsInfo->astImsNicDnsInfo[ucIndex].ucIpAddrLen == ucIpAddrLen)
            {
                /*  �Ƚ�pucIpAddr��pstDnsInfo->astImsNicDnsInfo[ucIndex].aucDnsAddr���ڴ�(����ucIpAddrLen) */
                if ( IMS_NIC_IsSameIpAddr(pucIpAddr, pstDnsInfo->astImsNicDnsInfo[ucIndex].aucDnsAddr, ucIpAddrLen) )
                {
                    /* ȡ��ǰ���� */
                    *pucDnsIndx  = ucIndex;

                    return IMS_NIC_DNS_CFGED;
                }
            }
        }
        else
        {
            /* ��¼��ǰ���е�DNS��� */
            aucIdle[ucIdleNum]   = ucIndex;
            ucIdleNum++;
        }
    }

    /* ȡ��һ���������� */
    *pucDnsIndx  = aucIdle[0];

    return IMS_NIC_DNS_NOT_CFGED;

}

/*****************************************************************************
 �� �� ��  : IMS_NIC_DnsConfig
 ��������  : ����DNS������
 �������  : pucIpAddr              -- �����õ�DNS������IP
             ulIpAddrLen            -- IP��ַ����
             pstImsNicDnsInfo       -- �ڲ��洢��DNS������Ϣ
             pstImsNicPdnInfo       -- �ڲ��洢�Ķ�Ӧ����������������������Ϣ
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_SUCC��ʧ��Ϊ��Ӧ�Ĵ�����
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 IMS_NIC_DnsConfig (VOS_UINT8 *pucIpAddr, VOS_UINT8 ucIpAddrLen, IMS_NIC_DNS_INFO_STRU *pstDnsInfo, IMS_NIC_SINGLE_PDN_INFO_STRU *pstSinglePdnInfo)
{
    VOS_UINT8               ucDnsIndx;
    VOS_UINT32              ulResult;
    VOS_UINT8               aucIpAddrStr[IMS_NIC_IPV6_ADDR_STR_LEN + 1] = {0};
    VOS_INT                 iLen;


    ucDnsIndx = 0;

    if (VOS_NULL_PTR == pucIpAddr)
    {
        IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_DnsConfig, ERROR, pucIpAddr is Null Ptr.");

        return IMS_NIC_CONFIG_PTR_NULL;
    }

    if (VOS_NULL_PTR == pstDnsInfo)
    {
        IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_DnsConfig, ERROR, pstDnsInfo is Null Ptr.");

        return IMS_NIC_CONFIG_PTR_NULL;
    }

    if (VOS_NULL_PTR == pstSinglePdnInfo)
    {
        IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_DnsConfig, ERROR, pstSinglePdnInfo is Null Ptr.");

        return IMS_NIC_CONFIG_PTR_NULL;
    }

    /* �жϵ�ǰ�����õ�DNS�������Ƿ��Ѿ����ù� */
    ulResult     = IMS_NIC_DnsAddrCheck(pucIpAddr, ucIpAddrLen, pstDnsInfo, &ucDnsIndx);

    /* ��DNS������û�����ù� */
    if ( IMS_NIC_DNS_CFGED != ulResult )
    {
        if ( pstDnsInfo->ucDnsServerCnt < IMS_NIC_MAX_DNS_SERVER_NUM )
        {
            if (IMS_NIC_IPV6_ADDR_LENGTH == ucIpAddrLen)
            {
                /* IPv6��ַתΪ�ַ��� */
                IMS_NIC_ConvertIpv6AddrToCompressedStr(aucIpAddrStr, pucIpAddr, IMS_NIC_IPV6_STR_RFC2373_TOKENS);

                /* ����Indx����DNS Server��Interpeak���� */
                IMS_NIC_IPCOM_DNSS_CONFIG(ucDnsIndx, aucIpAddrStr);
            }
            else
            {
                /* IPv4��ַתΪ�ַ��� */
                iLen = IMS_NIC_IP4_TO_STR(aucIpAddrStr, pucIpAddr);
                IMS_NIC_SET_CMD_END(aucIpAddrStr, iLen);

                /* ����Indx����DNS Server��Interpeak���� */
                IMS_NIC_IPCOM_DNSS_CONFIG(ucDnsIndx, aucIpAddrStr);
            }

            DRV_RT_MEMCPY(pstDnsInfo->astImsNicDnsInfo[ucDnsIndx].aucDnsAddr, pucIpAddr, ucIpAddrLen);
            pstDnsInfo->astImsNicDnsInfo[ucDnsIndx].ucIpAddrLen  = ucIpAddrLen;
            pstDnsInfo->ucDnsServerCnt++;
        }
        else
        {
            IMS_NIC_STATS_INC(IMS_NIC_STATS_CFG_DNS_SERVER_FULL, 1, IMS_NIC_MODEM_ID_0);

            return IMS_NIC_DNS_SERVER_FULL;
        }
    }

    /* ����������Ѿ����ù��������óɹ������ü��� */
    pstSinglePdnInfo->aucDnsFlag[ucDnsIndx]  = VOS_TRUE;

    /* ����DNS��������Ӧ�����ü�����1 */
    pstDnsInfo->astImsNicDnsInfo[ucDnsIndx].cDnsRef++;

    return IMS_NIC_SUCC;
}



VOS_UINT32 IMS_NIC_RecfgUncfgedDnsIpv4(IMS_NIC_SINGLE_PDN_INFO_STRU *pstSinglePdnInfo, IMS_NIC_DNS_INFO_STRU *pstDnsInfo)
{
    VOS_UINT32 ulRst;


    ulRst = IMS_NIC_SUCC;

    if (VOS_NULL_PTR == pstSinglePdnInfo || VOS_NULL_PTR == pstDnsInfo)
    {
        IMS_NIC_WARNING_LOG(UEPS_PID_IMSNIC, "IMS_NIC_RecfgUncfgedDns, ERROR, Input para is NULL.");

        return IMS_NIC_CONFIG_PTR_NULL;
    }

    if ((VOS_TRUE == pstSinglePdnInfo->stIpv4PdnInfo.bitOpDnsPrim)
        && (VOS_FALSE == pstSinglePdnInfo->bitOpIpv4DnsPrimCfged))
    {
        /* ����DNS��Interpeak�� */
        ulRst = IMS_NIC_IPV4_DNS_CONFIG(pstSinglePdnInfo->stIpv4PdnInfo.aucDnsPrimAddr, pstDnsInfo, pstSinglePdnInfo);

        /* ���óɹ���ʾ��DNS�Ѿ������õ�Interpeak��,������ΪDNS�Ѿ�����������ǰDNSδ���õ�Interpeak */
        pstSinglePdnInfo->bitOpIpv4DnsPrimCfged = ((IMS_NIC_SUCC == ulRst) ? VOS_TRUE : VOS_FALSE);
    }

    if ((VOS_TRUE == pstSinglePdnInfo->stIpv4PdnInfo.bitOpDnsSec)
        && (VOS_FALSE == pstSinglePdnInfo->bitOpIpv4DnsSecCfged))
    {
        /* ����DNS��Interpeak�� */
        ulRst = IMS_NIC_IPV4_DNS_CONFIG(pstSinglePdnInfo->stIpv4PdnInfo.aucDnsSecAddr, pstDnsInfo, pstSinglePdnInfo);

        /* ���óɹ���ʾ��DNS�Ѿ������õ�Interpeak��,������ΪDNS�Ѿ�����������ǰDNSδ���õ�Interpeak */
        pstSinglePdnInfo->bitOpIpv4DnsSecCfged = ((IMS_NIC_SUCC == ulRst) ? VOS_TRUE : VOS_FALSE);
    }

    return ulRst;
}



VOS_UINT32 IMS_NIC_RecfgUncfgedDnsIpv6(IMS_NIC_SINGLE_PDN_INFO_STRU *pstSinglePdnInfo, IMS_NIC_DNS_INFO_STRU *pstDnsInfo)
{
    VOS_UINT32 ulRst;


    ulRst = IMS_NIC_SUCC;

    if (VOS_NULL_PTR == pstSinglePdnInfo || VOS_NULL_PTR == pstDnsInfo)
    {
        IMS_NIC_WARNING_LOG(UEPS_PID_IMSNIC, "IMS_NIC_RecfgUncfgedDns, ERROR, Input para is NULL.");

        return IMS_NIC_CONFIG_PTR_NULL;
    }

    if ((VOS_TRUE == pstSinglePdnInfo->stIpv6PdnInfo.bitOpDnsPrim)
        && (VOS_FALSE == pstSinglePdnInfo->bitOpIpv6DnsPrimCfged))
    {
        /* ����DNS��Interpeak�� */
        ulRst = IMS_NIC_IPV6_DNS_CONFIG(pstSinglePdnInfo->stIpv6PdnInfo.aucDnsPrimAddr, pstDnsInfo, pstSinglePdnInfo);

        /* ���óɹ���ʾ��DNS�Ѿ������õ�Interpeak��,������ΪDNS�Ѿ�����������ǰDNSδ���õ�Interpeak */
        pstSinglePdnInfo->bitOpIpv6DnsPrimCfged = ((IMS_NIC_SUCC == ulRst) ? VOS_TRUE : VOS_FALSE);
    }

    if ((VOS_TRUE == pstSinglePdnInfo->stIpv6PdnInfo.bitOpDnsSec)
        && (VOS_FALSE == pstSinglePdnInfo->bitOpIpv6DnsSecCfged))
    {
        /* ����DNS��Interpeak�� */
        ulRst = IMS_NIC_IPV6_DNS_CONFIG(pstSinglePdnInfo->stIpv6PdnInfo.aucDnsSecAddr, pstDnsInfo, pstSinglePdnInfo);

        /* ���óɹ���ʾ��DNS�Ѿ������õ�Interpeak��,������ΪDNS�Ѿ�����������ǰDNSδ���õ�Interpeak */
        pstSinglePdnInfo->bitOpIpv6DnsSecCfged = ((IMS_NIC_SUCC == ulRst) ? VOS_TRUE : VOS_FALSE);
    }

    return ulRst;
}



VOS_VOID  IMS_NIC_RecfgUncfgedDns(IMS_NIC_PDN_INFO_STRU *pstPdnInfo)
{
    VOS_UINT32                      ulIndx;
    IMS_NIC_SINGLE_PDN_INFO_STRU   *pstSinglePdnInfo;


    if (VOS_NULL_PTR == pstPdnInfo)
    {
        IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_RecfgUncfgedDns, ERROR, Input para is NULL.");

        return;
    }

    for(ulIndx = 0; ulIndx < IMS_NIC_RAB_ID_MAX_NUM; ulIndx++)
    {
        pstSinglePdnInfo = &pstPdnInfo->astImsNicPdnInfo[ulIndx];

        if (VOS_TRUE == pstSinglePdnInfo->bitOpIpv4PdnInfo)
        {
            /* һ�����ɹ��Ͳ���������,�˳�ѭ�� */
            if (IMS_NIC_SUCC != IMS_NIC_RecfgUncfgedDnsIpv4(pstSinglePdnInfo, &pstPdnInfo->stImsNicDnsInfo))
            {
                break;
            }
        }

        if (VOS_TRUE == pstSinglePdnInfo->bitOpIpv6PdnInfo)
        {
            /* һ�����ɹ��Ͳ���������,�˳�ѭ�� */
            if (IMS_NIC_SUCC != IMS_NIC_RecfgUncfgedDnsIpv6(pstSinglePdnInfo, &pstPdnInfo->stImsNicDnsInfo))
            {
                break;
            }
        }
    }
}


VOS_VOID IMS_NIC_DnsInfoDel(IMS_NIC_PDN_INFO_STRU *pstPdnInfo, VOS_UINT8 ucRabId)
{
    VOS_UINT32                           ulIndx;
    IMS_NIC_SINGLE_PDN_INFO_STRU        *pstSinglePdnInfo;
    IMS_NIC_DNS_INFO_STRU               *pstDnsInfo;


    if (VOS_NULL_PTR == pstPdnInfo)
    {
        IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_DnsInfoDel, ERROR, Input para is NULL.");

        return;
    }

    pstSinglePdnInfo = &(pstPdnInfo->astImsNicPdnInfo[ucRabId - IMS_NIC_RAB_ID_OFFSET]);
    pstDnsInfo       = &(pstPdnInfo->stImsNicDnsInfo);

    for (ulIndx = 0; ulIndx < IMS_NIC_MAX_DNS_SERVER_NUM; ulIndx++)
    {
        /* �ҵ���Ӧ��dns���� */
        if (VOS_TRUE == pstSinglePdnInfo->aucDnsFlag[ulIndx])
        {
            pstDnsInfo->astImsNicDnsInfo[ulIndx].cDnsRef--;
            pstSinglePdnInfo->aucDnsFlag[ulIndx]    = VOS_FALSE;

            /* �����ǰDNS�Ѿ�û�������ã���Interpeak��ɾ�� */
            if (0 >= pstDnsInfo->astImsNicDnsInfo[ulIndx].cDnsRef)
            {
                IMS_NIC_IPCOM_DNSS_DEL(ulIndx);

                PS_MEM_SET(&pstDnsInfo->astImsNicDnsInfo[ulIndx], 0, sizeof(IMS_NIC_SINGLE_DNS_INFO_STRU));
                pstDnsInfo->ucDnsServerCnt--;
            }
        }
    }

    /* ����д�Interpeak��ɾ��DNS Server������֮ǰ�����ÿ��Ƿ���δ���óɹ���DNS,�����䵽Interpeak�� */
    if (pstDnsInfo->ucDnsServerCnt < IMS_NIC_MAX_DNS_SERVER_NUM)
    {
        IMS_NIC_RecfgUncfgedDns(pstPdnInfo);
    }
}


/*****************************************************************************
 �� �� ��  : IMS_NIC_PdnInfoDelIpv4
 ��������  : IMS��������PDN IPv4������Ϣɾ��
 �������  : ucRabId        ���غ�
             enModemId      ��ǰmodemID

 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID   IMS_NIC_PdnInfoDelIpv4(VOS_UINT8 ucRabId, MODEM_ID_ENUM_UINT16 enModemId)
{
    VOS_UINT8                           aucCmd[IMS_NIC_CMD_LEN] = {0};
    VOS_INT                             iLen;
    IMS_NIC_ENTITY_STRU                *pstImsNicEntity;
    IMS_NIC_SINGLE_PDN_INFO_STRU       *pstSinglePdnInfo;
    IMS_NIC_PDN_INFO_STRU              *pstPdnInfo;


    pstImsNicEntity     = IMS_NIC_GET_ENTITY_STRU(enModemId);
    pstPdnInfo          = &(pstImsNicEntity->stImsNicPdnInfo);
    pstSinglePdnInfo    = &(pstPdnInfo->astImsNicPdnInfo[ucRabId - IMS_NIC_RAB_ID_OFFSET]);


    /* ɾ��IP��ַ */
    iLen = IMS_NIC_DEL_IPV4_CMD(aucCmd, enModemId, IMS_NIC_GET_NIC_NAME, pstSinglePdnInfo->stIpv4PdnInfo.aucIpV4Addr);
    IMS_NIC_SET_CMD_END(aucCmd, iLen);
    IMS_NIC_RUN_CMD(aucCmd);

    /* ����״̬���������¼��IP��Ϣ */
    pstSinglePdnInfo->bitOpIpv4PdnInfo      = VOS_FALSE;
    pstSinglePdnInfo->bitOpIpv4DnsPrimCfged = VOS_FALSE;
    pstSinglePdnInfo->bitOpIpv4DnsSecCfged  = VOS_FALSE;

    PS_MEM_SET(&(pstSinglePdnInfo->stIpv4PdnInfo), 0, sizeof(IMS_NIC_IPV4_PDN_INFO_STRU));
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_PdnInfoDelIpv6
 ��������  : IMS��������PDN IPv6������Ϣɾ��
 �������  : ucRabId        ���غ�
             enModemId      ��ǰmodemID

 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID   IMS_NIC_PdnInfoDelIpv6(VOS_UINT8 ucRabId, MODEM_ID_ENUM_UINT16 enModemId)
{
    VOS_UINT8                           aucCmd[IMS_NIC_CMD_LEN] = {0};
    VOS_UINT8                           aucIpAddrStr[IMS_NIC_IPV6_ADDR_STR_LEN + 1] = {0};
    VOS_INT                             iLen;
    IMS_NIC_ENTITY_STRU                *pstImsNicEntity;
    IMS_NIC_SINGLE_PDN_INFO_STRU       *pstSinglePdnInfo;
    IMS_NIC_PDN_INFO_STRU              *pstPdnInfo;


    pstImsNicEntity     = IMS_NIC_GET_ENTITY_STRU(enModemId);
    pstPdnInfo          = &(pstImsNicEntity->stImsNicPdnInfo);
    pstSinglePdnInfo    = &(pstPdnInfo->astImsNicPdnInfo[ucRabId - IMS_NIC_RAB_ID_OFFSET]);

    IMS_NIC_ConvertIpv6AddrToCompressedStr((VOS_UINT8 *)aucIpAddrStr, pstSinglePdnInfo->stIpv6PdnInfo.aucIpV6Addr, IMS_NIC_IPV6_STR_RFC2373_TOKENS);

    /* ɾ��IP��ַ */
    iLen = IMS_NIC_DEL_IPV6_CMD(aucCmd, enModemId, IMS_NIC_GET_NIC_NAME, aucIpAddrStr);
    IMS_NIC_SET_CMD_END(aucCmd, iLen);
    IMS_NIC_RUN_CMD(aucCmd);

    /* ����״̬���������¼��IP��Ϣ */
    pstSinglePdnInfo->bitOpIpv6PdnInfo      = VOS_FALSE;
    pstSinglePdnInfo->bitOpIpv6DnsPrimCfged = VOS_FALSE;
    pstSinglePdnInfo->bitOpIpv6DnsSecCfged  = VOS_FALSE;

    PS_MEM_SET(&(pstSinglePdnInfo->stIpv6PdnInfo), 0, sizeof(IMS_NIC_IPV6_PDN_INFO_STRU));
}


/*****************************************************************************
 �� �� ��  : IMS_NIC_PdnInfoDel
 ��������  : IMS��������PDN������Ϣɾ������IMSAģ����PDNȥ��������
 �������  : ucRabId        ���غ�
             enModemId      ��ǰmodemID
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_SUCC��ʧ��Ϊ��Ӧ�Ĵ�����
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 IMS_NIC_PdnInfoDel(VOS_UINT8 ucRabId, MODEM_ID_ENUM_UINT16 enModemId)
{
    IMS_NIC_ENTITY_STRU                 *pstImsNicEntity;
    IMS_NIC_SINGLE_PDN_INFO_STRU        *pstSinglePdnInfo;
    IMS_NIC_PDN_INFO_STRU               *pstPdnInfo;


    IMS_NIC_MNTN_TRACE_NO_DATA(ID_IMS_NIC_MNTN_TRACE_RCV_CONFIG_DEL, enModemId, ucRabId);

    if ( enModemId >= IMS_NIC_MODEM_ID_BUTT )
    {
        IMS_NIC_STATS_INC(IMS_NIC_STATS_CFG_DEL_MODEM_ID_ERR, 1, IMS_NIC_MODEM_ID_0);

        IMS_NIC_ERROR_LOG1(UEPS_PID_IMSNIC, "IMS_NIC_PdnInfoDel, ERROR Input modem id invalid!\n", enModemId);

        return IMS_NIC_MODEM_ID_INVALID;
    }

    /* �����غ��Ƿ���Ч��ȡֵ��Χ[5, 15] */
    if ( !IMS_NIC_IS_RAB_VALID(ucRabId) )
    {
        IMS_NIC_STATS_INC(IMS_NIC_STATS_CFG_DEL_RAB_ID_ERR, 1, enModemId);

        IMS_NIC_ERROR_LOG1(UEPS_PID_IMSNIC, "IMS_NIC_PdnInfoDel, ERROR Input Rab id invalid!\n", ucRabId);

        return IMS_NIC_RABID_INVALID;
    }


    pstImsNicEntity     = IMS_NIC_GET_ENTITY_STRU(enModemId);
    pstPdnInfo          = &(pstImsNicEntity->stImsNicPdnInfo);
    pstSinglePdnInfo    = &(pstPdnInfo->astImsNicPdnInfo[ucRabId - IMS_NIC_RAB_ID_OFFSET]);

    if (VOS_FALSE == (pstSinglePdnInfo->bitOpIpv4PdnInfo | pstSinglePdnInfo->bitOpIpv6PdnInfo))
    {
        IMS_NIC_STATS_INC(IMS_NIC_STATS_CFG_DEL_NO_PDN, 1, enModemId);

        IMS_NIC_WARNING_LOG2(UEPS_PID_IMSNIC, "IMS_NIC_PdnInfoDel, ERROR No Cfged PDN!\n", enModemId, ucRabId);

        return IMS_NIC_NO_CFG_PDN;
    }

    if (0 >= pstPdnInfo->iPdnCfgCnt)
    {
        IMS_NIC_STATS_INC(IMS_NIC_STATS_NO_CFGED_PDN, 1, enModemId);

        IMS_NIC_WARNING_LOG2(UEPS_PID_IMSNIC, "IMS_NIC_PdnInfoDel, ERROR Input Rab id invalid!\n", enModemId, ucRabId);

        /* �ó��������쳣����һ��״̬ */
        IMS_NIC_ResetAll(enModemId);

        return IMS_NIC_NO_CFG_PDN;
    }


    if ( VOS_TRUE == pstSinglePdnInfo->bitOpIpv4PdnInfo )
    {
       IMS_NIC_PdnInfoDelIpv4(ucRabId, enModemId);
    }

    if ( VOS_TRUE == pstSinglePdnInfo->bitOpIpv6PdnInfo )
    {
        IMS_NIC_PdnInfoDelIpv6(ucRabId, enModemId);
    }

    IMS_NIC_DnsInfoDel(pstPdnInfo, ucRabId);

    pstPdnInfo->iPdnCfgCnt--;

    if (0 >= pstPdnInfo->iPdnCfgCnt)
    {
        IMS_NIC_ResetAll(enModemId);
    }

    IMS_NIC_MNTN_TRACE2(ID_IMS_NIC_MNTN_TRACE_RCV_CONFIG_DEL_RSLT, pstPdnInfo, sizeof(IMS_NIC_PDN_INFO_STRU), enModemId, ucRabId);

    return IMS_NIC_SUCC;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_PdnInfoConfigIpv4
 ��������  : IMS��������IPv4���ýӿ�
 �������  : pstConfigInfo   IMS��������������Ϣ
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_SUCC��ʧ��Ϊ��Ӧ�Ĵ�����
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 IMS_NIC_PdnInfoConfigIpv4(IMS_NIC_PDN_INFO_CONFIG_STRU *pstConfigInfo)
{
    VOS_UINT8                           aucCmd[IMS_NIC_CMD_LEN] = {0};
    VOS_UINT32                          ulRst;
    VOS_INT                             iLen;
    IMS_NIC_ENTITY_STRU                *pstImsNicEntity;
    IMS_NIC_SINGLE_PDN_INFO_STRU       *pstSinglePdnInfo;
    IMS_NIC_DNS_INFO_STRU              *pstDnsInfo;


    ulRst = IMS_NIC_SUCC;

    if (VOS_NULL_PTR == pstConfigInfo)
    {
        IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_PdnInfoConfigIpv4, ERROR, Input Null Ptr.");

        return IMS_NIC_CONFIG_PTR_NULL;
    }

    pstImsNicEntity     = IMS_NIC_GET_ENTITY_STRU(pstConfigInfo->enModemId);
    pstSinglePdnInfo    = &(pstImsNicEntity->stImsNicPdnInfo.astImsNicPdnInfo[pstConfigInfo->ucRabId  - IMS_NIC_RAB_ID_OFFSET]);
    pstDnsInfo          = &(pstImsNicEntity->stImsNicPdnInfo.stImsNicDnsInfo);

    pstSinglePdnInfo->bitOpIpv4PdnInfo  = VOS_TRUE;
    DRV_RT_MEMCPY(pstSinglePdnInfo->stIpv4PdnInfo.aucIpV4Addr, pstConfigInfo->stIpv4PdnInfo.aucIpV4Addr, IMS_NIC_IPV4_ADDR_LENGTH);

    /* ����ipv4���� */
    iLen = IMS_NIC_ADD_IPV4_CMD(aucCmd, pstConfigInfo->enModemId, IMS_NIC_GET_NIC_NAME, pstConfigInfo->stIpv4PdnInfo.aucIpV4Addr);
    IMS_NIC_SET_CMD_END(aucCmd, iLen);
    IMS_NIC_RUN_CMD(aucCmd);

    if (VOS_TRUE == pstConfigInfo->stIpv4PdnInfo.bitOpDnsPrim)
    {
        /* ����DNS��ַ��Ϣ */
        DRV_RT_MEMCPY(pstSinglePdnInfo->stIpv4PdnInfo.aucDnsPrimAddr, pstConfigInfo->stIpv4PdnInfo.aucDnsPrimAddr, IMS_NIC_IPV4_ADDR_LENGTH);
        pstSinglePdnInfo->stIpv4PdnInfo.bitOpDnsPrim = VOS_TRUE;

        /* ����DNS��Interpeak�� */
        ulRst = IMS_NIC_IPV4_DNS_CONFIG(pstConfigInfo->stIpv4PdnInfo.aucDnsPrimAddr, pstDnsInfo, pstSinglePdnInfo);

        /* ���óɹ���ʾ��DNS�Ѿ������õ�Interpeak��,������ΪDNS�Ѿ�����������ǰDNSδ���õ�Interpeak */
        pstSinglePdnInfo->bitOpIpv4DnsPrimCfged = ((IMS_NIC_SUCC == ulRst) ? VOS_TRUE : VOS_FALSE);
    }

    if (VOS_TRUE == pstConfigInfo->stIpv4PdnInfo.bitOpDnsSec)
    {
        /* ����DNS��ַ��Ϣ */
        DRV_RT_MEMCPY(pstSinglePdnInfo->stIpv4PdnInfo.aucDnsSecAddr, pstConfigInfo->stIpv4PdnInfo.aucDnsSecAddr, IMS_NIC_IPV4_ADDR_LENGTH);
        pstSinglePdnInfo->stIpv4PdnInfo.bitOpDnsSec = VOS_TRUE;

        /* ����DNS��Interpeak�� */
        ulRst = IMS_NIC_IPV4_DNS_CONFIG(pstConfigInfo->stIpv4PdnInfo.aucDnsSecAddr, pstDnsInfo, pstSinglePdnInfo);

        /* ���óɹ���ʾ��DNS�Ѿ������õ�Interpeak��,������ΪDNS�Ѿ�����������ǰDNSδ���õ�Interpeak */
        pstSinglePdnInfo->bitOpIpv4DnsSecCfged = ((IMS_NIC_SUCC == ulRst) ? VOS_TRUE : VOS_FALSE);
    }

    return ulRst;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_PdnInfoConfigIpv6
 ��������  : IMS��������IPv6���ýӿ�
 �������  : pstConfigInfo   IMS��������������Ϣ
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_SUCC��ʧ��Ϊ��Ӧ�Ĵ�����
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 IMS_NIC_PdnInfoConfigIpv6(IMS_NIC_PDN_INFO_CONFIG_STRU *pstConfigInfo)
{
    VOS_UINT8                           aucCmd[IMS_NIC_CMD_LEN] = {0};
    VOS_UINT8                           aucIpAddrStr[IMS_NIC_IPV6_ADDR_STR_LEN + 1] = {0};
    VOS_UINT32                          ulRst;
    VOS_INT                             iLen;
    IMS_NIC_ENTITY_STRU                *pstImsNicEntity;
    IMS_NIC_SINGLE_PDN_INFO_STRU       *pstSinglePdnInfo;
    IMS_NIC_DNS_INFO_STRU              *pstDnsInfo;


    ulRst  = IMS_NIC_SUCC;

    if (VOS_NULL_PTR == pstConfigInfo)
    {
        IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_PdnInfoConfigIpv6, ERROR, Input Null Ptr.");

        return IMS_NIC_CONFIG_PTR_NULL;
    }

    if (pstConfigInfo->stIpv6PdnInfo.ulBitPrefixLen > IMS_NIC_IPV6_ADDR_BIT_LEN)
    {
        IMS_NIC_ERROR_LOG1(UEPS_PID_IMSNIC, "IMS_NIC_PdnInfoConfigIpv6, ERROR, Invalid Prefixlen.", pstConfigInfo->stIpv6PdnInfo.ulBitPrefixLen);

        IMS_NIC_STATS_INC(IMS_NIC_STATS_INVALID_IPV6_PREFIX_LEN, 1, pstConfigInfo->enModemId);

        return IMS_NIC_PREFIXLEN_INVALID;
    }

    pstImsNicEntity     = IMS_NIC_GET_ENTITY_STRU(pstConfigInfo->enModemId);
    pstSinglePdnInfo    = &(pstImsNicEntity->stImsNicPdnInfo.astImsNicPdnInfo[pstConfigInfo->ucRabId  - IMS_NIC_RAB_ID_OFFSET]);
    pstDnsInfo          = &(pstImsNicEntity->stImsNicPdnInfo.stImsNicDnsInfo);

    pstSinglePdnInfo->bitOpIpv6PdnInfo  = VOS_TRUE;
    pstSinglePdnInfo->stIpv6PdnInfo.ulBitPrefixLen    = pstConfigInfo->stIpv6PdnInfo.ulBitPrefixLen;
    DRV_RT_MEMCPY(pstSinglePdnInfo->stIpv6PdnInfo.aucIpV6Addr, pstConfigInfo->stIpv6PdnInfo.aucIpV6Addr, IMS_NIC_IPV6_ADDR_LENGTH);

    IMS_NIC_ConvertIpv6AddrToCompressedStr((VOS_UINT8 *)aucIpAddrStr, pstConfigInfo->stIpv6PdnInfo.aucIpV6Addr, IMS_NIC_IPV6_STR_RFC2373_TOKENS);

    /* ����ipv6���� */
    iLen = IMS_NIC_ADD_IPV6_CMD(aucCmd, pstConfigInfo->enModemId, IMS_NIC_GET_NIC_NAME, aucIpAddrStr, pstConfigInfo->stIpv6PdnInfo.ulBitPrefixLen);
    IMS_NIC_SET_CMD_END(aucCmd, iLen);
    IMS_NIC_RUN_CMD(aucCmd);

    if (VOS_TRUE == pstConfigInfo->stIpv6PdnInfo.bitOpDnsPrim)
    {
        /* ����DNS��ַ��Ϣ */
        DRV_RT_MEMCPY(pstSinglePdnInfo->stIpv6PdnInfo.aucDnsPrimAddr, pstConfigInfo->stIpv6PdnInfo.aucDnsPrimAddr, IMS_NIC_IPV6_ADDR_LENGTH);
        pstSinglePdnInfo->stIpv6PdnInfo.bitOpDnsPrim = VOS_TRUE;

        /* ������DNS��Interpeak�� */
        ulRst = IMS_NIC_IPV6_DNS_CONFIG(pstConfigInfo->stIpv6PdnInfo.aucDnsPrimAddr, pstDnsInfo, pstSinglePdnInfo);

        /* ���óɹ���ʾ��DNS�Ѿ������õ�Interpeak��,������ΪDNS�Ѿ�����������ǰDNSδ���õ�Interpeak */
        pstSinglePdnInfo->bitOpIpv6DnsPrimCfged = ((IMS_NIC_SUCC == ulRst) ? VOS_TRUE : VOS_FALSE);
    }

    if (VOS_TRUE == pstConfigInfo->stIpv6PdnInfo.bitOpDnsSec)
    {
        /* ����DNS��ַ��Ϣ */
        DRV_RT_MEMCPY(pstSinglePdnInfo->stIpv6PdnInfo.aucDnsSecAddr, pstConfigInfo->stIpv6PdnInfo.aucDnsSecAddr, IMS_NIC_IPV6_ADDR_LENGTH);
        pstSinglePdnInfo->stIpv6PdnInfo.bitOpDnsSec = VOS_TRUE;

        /* ���ø�DNS��Interpeak�� */
        ulRst = IMS_NIC_IPV6_DNS_CONFIG(pstConfigInfo->stIpv6PdnInfo.aucDnsSecAddr, pstDnsInfo, pstSinglePdnInfo);

        /* ���óɹ���ʾ��DNS�Ѿ������õ�Interpeak��,������ΪDNS�Ѿ�����������ǰDNSδ���õ�Interpeak */
        pstSinglePdnInfo->bitOpIpv6DnsSecCfged = ((IMS_NIC_SUCC == ulRst) ? VOS_TRUE : VOS_FALSE);
    }

    return ulRst;
}
VOS_VOID IMS_NIC_SetDefaultRoute(MODEM_ID_ENUM_UINT16 enModemId)
{
    VOS_UINT8                           aucCmd[IMS_NIC_CMD_LEN] = {0};
    VOS_INT                             iLen;


    /* ����IPV4Ĭ��·�ɵ����������� */
    iLen = IMS_NIC_IPV4_ROUTE_CMD(aucCmd, enModemId, IMS_NIC_GET_NIC_NAME);
    IMS_NIC_SET_CMD_END(aucCmd, iLen);
    IMS_NIC_RUN_CMD(aucCmd);

    /* ����IPV6Ĭ��·�ɵ����������� */
    iLen = IMS_NIC_IPV6_ROUTE_CMD(aucCmd, enModemId, IMS_NIC_GET_NIC_NAME);
    IMS_NIC_SET_CMD_END(aucCmd, iLen);
    IMS_NIC_RUN_CMD(aucCmd);

    return;
}


VOS_UINT32 IMS_NIC_SetSocketPort(VOS_UINT16 usMinPort, VOS_UINT16 usMaxPort)
{
    VOS_UINT8                           aucCmd[IMS_NIC_CMD_LEN] = {0};
    VOS_INT                             iLen;


    if (usMinPort > usMaxPort)
    {
        IMS_NIC_ERROR_LOG2(UEPS_PID_IMSNIC,
            "IMS_NIC_SetSocketPort, ERROR, min, max", usMinPort, usMaxPort);
        return IMS_NIC_FAIL;
    }

    /* ����Socket�˿����� */
    iLen = IMS_NIC_SET_PORT_MIN_CMD(aucCmd, usMinPort);
    IMS_NIC_SET_CMD_END(aucCmd, iLen);
    IMS_NIC_RUN_CMD(aucCmd);

    /* ����Socket�˿����� */
    iLen = IMS_NIC_SET_PORT_MAX_CMD(aucCmd, usMaxPort);
    IMS_NIC_SET_CMD_END(aucCmd, iLen);
    IMS_NIC_RUN_CMD(aucCmd);

    return IMS_NIC_SUCC;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_PdnInfoConfig
 ��������  : IMS�����������ýӿ�,��IMSAģ�����
 �������  : pstConfigInfo   IMS��������������Ϣ
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_SUCC��ʧ��Ϊ��Ӧ�Ĵ�����
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 IMS_NIC_PdnInfoConfig (IMS_NIC_PDN_INFO_CONFIG_STRU *pstConfigInfo)
{
    IMS_NIC_ENTITY_STRU                 *pstImsNicEntity;
    IMS_NIC_SINGLE_PDN_INFO_STRU        *pstSinglePdnInfo;
    IMS_NIC_PDN_INFO_STRU               *pstPdnInfo;
    VOS_UINT32                           ulResult;
    VOS_UINT32                           ulIsNewConfig;


    ulResult        = IMS_NIC_SUCC;
    ulIsNewConfig   = VOS_FALSE;

    if ( VOS_NULL_PTR == pstConfigInfo )
    {
        IMS_NIC_STATS_INC(IMS_NIC_STATS_CFG_INPUT_NULL_PTR, 1, IMS_NIC_MODEM_ID_0);

        IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_PdnInfoConfig, ERROR, Input NULL PTR!");

        return IMS_NIC_CONFIG_PTR_NULL;
    }

    /* ��ά�ɲ� */
    IMS_NIC_MNTN_TRACE(ID_IMS_NIC_MNTN_TRACE_RCV_CONFIG_INFO, pstConfigInfo, sizeof(IMS_NIC_PDN_INFO_CONFIG_STRU));

    if ( pstConfigInfo->enModemId >= IMS_NIC_MODEM_ID_BUTT )
    {
        IMS_NIC_STATS_INC(IMS_NIC_STATS_CFG_ADD_RAB_ID_ERR, 1, IMS_NIC_MODEM_ID_0);

        IMS_NIC_ERROR_LOG1(UEPS_PID_IMSNIC, "IMS_NIC_PdnInfoConfig, ERROR, Input Modem id invalid!", pstConfigInfo->enModemId);

        return IMS_NIC_MODEM_ID_INVALID;
    }

    /* �����غ��Ƿ���Ч��ȡֵ��Χ[5, 15] */
    if ( !IMS_NIC_IS_RAB_VALID(pstConfigInfo->ucRabId) )
    {
        IMS_NIC_STATS_INC(IMS_NIC_STATS_CFG_ADD_MODEM_ID_ERR, 1, pstConfigInfo->enModemId);

        IMS_NIC_ERROR_LOG1(UEPS_PID_IMSNIC, "IMS_NIC_PdnInfoConfig, ERROR Input Rab id invalid!\n", pstConfigInfo->ucRabId);

        return IMS_NIC_RABID_INVALID;
    }

    if (VOS_FALSE == (pstConfigInfo->bitOpIpv4PdnInfo | pstConfigInfo->bitOpIpv6PdnInfo))
    {
        IMS_NIC_STATS_INC(IMS_NIC_STATS_CFG_ADD_NO_PDN, 1, pstConfigInfo->enModemId);

        IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_PdnInfoConfig, ERROR No IPv4 and IPv6 info cfged!\n");

        return IMS_NIC_NO_CFG_PDN;
    }


    pstImsNicEntity     = IMS_NIC_GET_ENTITY_STRU(pstConfigInfo->enModemId);
    pstPdnInfo          = &(pstImsNicEntity->stImsNicPdnInfo);
    pstSinglePdnInfo    = &(pstPdnInfo->astImsNicPdnInfo[pstConfigInfo->ucRabId - IMS_NIC_RAB_ID_OFFSET]);

    /* ��鵱ǰ�������Ƿ񻹴��ھɵ���Ч���� */
    if ( ((pstConfigInfo->bitOpIpv4PdnInfo) & (pstSinglePdnInfo->bitOpIpv4PdnInfo))
        | ((pstConfigInfo->bitOpIpv6PdnInfo) & (pstSinglePdnInfo->bitOpIpv6PdnInfo)))
    {
        /* ɾ���ó����ϵ����� */
        IMS_NIC_PdnInfoDel(pstConfigInfo->ucRabId, pstConfigInfo->enModemId);
    }

    /* ������һ��PDN��ʱ�����ҪcfgCnt++��������Ҫ��ͨ�����ر����״̬ȷ�ϸ�rab���Ƿ��б����ù� */
    /* �ŵ�ɾ��֮�����жϣ����Ա�֤��rab�ϵ������Ǵ�������״̬ */
    if (VOS_FALSE == (pstSinglePdnInfo->bitOpIpv4PdnInfo | pstSinglePdnInfo->bitOpIpv6PdnInfo))
    {
        ulIsNewConfig = VOS_TRUE;
    }

    /* ����IPV4��� */
    if ( VOS_TRUE == pstConfigInfo->bitOpIpv4PdnInfo )
    {
        ulResult = IMS_NIC_PdnInfoConfigIpv4(pstConfigInfo);
    }

    /* ����IPV6��� */
    if ( VOS_TRUE == pstConfigInfo->bitOpIpv6PdnInfo )
    {
        ulResult |= IMS_NIC_PdnInfoConfigIpv6(pstConfigInfo);
    }

    if (0 == pstImsNicEntity->stImsNicPdnInfo.iPdnCfgCnt)
    {
        /* ����Ĭ��·�� */
        IMS_NIC_SetDefaultRoute(pstConfigInfo->enModemId);

        /* �˿����ƽ���������һ�μ���ʱ����һ�Σ���������ɾ������ */
        if (VOS_TRUE == pstConfigInfo->bitOpSockPortInfo)
        {
            /* ����Socket�˿� */
            ulResult |= IMS_NIC_SetSocketPort(pstConfigInfo->stSockPortInfo.usSockMinPort,
                pstConfigInfo->stSockPortInfo.usSockMaxPort);
        }
    }

    if (VOS_TRUE == ulIsNewConfig)
    {
        pstPdnInfo->iPdnCfgCnt++;
    }

    /* ǰ��Ŀ�ά�ɲ���ץ���ⲿ������Ϣ������ץȡ��ģ��ά����״̬��Ϣ */
    IMS_NIC_MNTN_TRACE2(ID_IMS_NIC_MNTN_TRACE_CONFIG_PROC_RSLT, pstPdnInfo, sizeof(IMS_NIC_PDN_INFO_STRU), ulResult, ulIsNewConfig);

    return ulResult;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_MtuSet
 ��������  : IMS��������MTU����
 �������  : ulMtu        ����͵�Ԫ
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_SUCC��ʧ��Ϊ��Ӧ�Ĵ�����
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 IMS_NIC_MtuSet(VOS_UINT32 ulMtu)
{
    VOS_UINT16                      usIndex;
    IMS_NIC_ENTITY_STRU            *pstImsNicEntity;
    VOS_CHAR                        acCmd[IMS_NIC_CMD_LEN] = {0};
    VOS_INT32                       iLen;


    /* ulMtu���Ȳ�����Ч��Χ�� */
    if ( (ulMtu < IMS_NIC_MIN_IPPKT_LEN) || (ulMtu > IMS_NIC_MAX_IPPKT_LEN) )
    {
        /* Ĭ�ϼ�¼��Modem0��ʵ���� */;
        IMS_NIC_STATS_INC(IMS_NIC_STATS_MTU_INVALID, 1, IMS_NIC_MODEM_ID_0);

        return IMS_NIC_MTU_INVALID;
    }

    for (usIndex = 0 ; usIndex < IMS_NIC_MODEM_ID_BUTT ; usIndex++)
    {
        pstImsNicEntity     = IMS_NIC_GET_ENTITY_STRU(usIndex);

        /* ���������Ѿ���ʼ��������ifconfig��������MTU */
        if ( VOS_TRUE == pstImsNicEntity->ulImsNicInitFlag )
        {
            iLen = IMS_NIC_SET_MTU_CMD(acCmd, usIndex, IMS_NIC_GET_NIC_NAME, ulMtu);
            IMS_NIC_SET_CMD_END(acCmd, iLen);
            /* ��������������������������MTU */
            IMS_NIC_RUN_CMD(acCmd);
        }

        /* �޸�IMS_NIC_MTU_VALUE��ֵ������������ʼ��ʱ�����ø�ֵΪMTU��С */
        IMS_NIC_SET_MTU(ulMtu);
    }

    return IMS_NIC_SUCC;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_RegUlDataSendCb
 ��������  : ע��IMS���������������ݷ��ͻص���������CDSģ�����
 �������  : pFunc        �������ݷ��ͻص��ӿ�
             enModemId    ��ǰmodemID
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_SUCC��ʧ��:IMS_NIC_FAIL
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 IMS_NIC_RegUlDataSendCb(IMS_NIC_SEND_UL_DATA_FUNC pFunc, MODEM_ID_ENUM_UINT16 enModemId)
{
    IMS_NIC_ENTITY_STRU     *pstImsNicEntity;


    if ( enModemId >= IMS_NIC_MODEM_ID_BUTT )
    {
        /* Ĭ�ϼ�¼��Modem0��ʵ���� */;
        IMS_NIC_STATS_INC(IMS_NIC_STATS_MODEM_ID_ERR_IN_REG_CB, 1, IMS_NIC_MODEM_ID_0);

        return IMS_NIC_MODEM_ID_INVALID;
    }

    pstImsNicEntity     = IMS_NIC_GET_ENTITY_STRU(enModemId);

    /* ����������û�г�ʼ�� */
    if ( VOS_TRUE != pstImsNicEntity->ulImsNicInitFlag )
    {
        IMS_NIC_SAVE_UL_DATA_FUNC(enModemId, pFunc);
    }
    else
    {
        pstImsNicEntity->pUlDataSendFunc = pFunc;
    }

    return IMS_NIC_SUCC;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_DlParaCheck
 ��������  : IMS���������������ݽ��ղ������
 �������  : pucData      IP������
             usLen        IP������
             enModemId    ��ǰmodemID
 �������  :
 �� �� ֵ  : �ɹ�:IMS_NIC_SUCC��ʧ��:IMS_NIC_FAIL
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 IMS_NIC_DlParaCheck(VOS_UINT8 *pucData, VOS_UINT16 usLen, MODEM_ID_ENUM_UINT16 enModemId)
{

    if ( enModemId >= IMS_NIC_MODEM_ID_BUTT )
    {
        /* Ĭ�ϼ�¼��Modem0��ʵ���� */;
        IMS_NIC_STATS_INC(IMS_NIC_STATS_MODE_ID_ERR_IN_DL_PROC, 1, IMS_NIC_MODEM_ID_0);

        IMS_NIC_ERROR_LOG1(UEPS_PID_IMSNIC, "IMS_NIC_DlParaCheck, ERROR, Input modem id invalid!\n", enModemId);

        return IMS_NIC_FAIL;
    }

    if ( VOS_NULL_PTR == pucData )
    {
        IMS_NIC_STATS_INC(IMS_NIC_STATS_DL_INPUT_NULL_PTR, 1, enModemId);

        IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_DlParaCheck, ERROR, input pucData is NULL!\n");

        return IMS_NIC_FAIL;
    }

    /* usLenֵ������Чֵ��Χ�� */
    if ( (usLen < IMS_NIC_MIN_IPPKT_LEN) || (usLen > IMS_NIC_CLUSTER_SIZE))
    {
        IMS_NIC_STATS_INC(IMS_NIC_STATS_DL_RCV_DATA_LEN_ERR, 1, enModemId);

        IMS_NIC_ERROR_LOG1(UEPS_PID_IMSNIC, "IMS_NIC_DlParaCheck, ERROR, usLen is invalid! usLen = %d\n", usLen);

        return IMS_NIC_FAIL;
    }

    return IMS_NIC_SUCC;

}

/*****************************************************************************
 �� �� ��  : IMS_NIC_DlDataRcv
 ��������  : IMS���������������ݽ��սӿڣ��ṩ��CDS����
 �������  : pucData      IP������
             usLen        IP������
             enModemId    ��ǰmodemID
 �������  :
 �� �� ֵ  : �ɹ�:IMS_NIC_SUCC��ʧ��:IMS_NIC_FAIL
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 IMS_NIC_DlDataRcv(VOS_UINT8 *pucData, VOS_UINT16 usLen, MODEM_ID_ENUM_UINT16 enModemId)
{
    IMS_NIC_DEV_STRU                    *pstImsNicDev;
    VOS_UINT8                            ucIpType;
    M_BLK_ID                             pstMblk;
    VOS_CHAR                            *pcCluster;
    VOS_CHAR                            *pcTmpCluster;
    CL_BLK_ID                            pstClBlk;
    IMS_NIC_ELEMENT_TAB_STRU            *pstImsNicElementTbl;
    VOS_UINT32                           ulResult;


    /* ��μ�� */
    ulResult        = IMS_NIC_DlParaCheck(pucData, usLen, enModemId);
    if ( IMS_NIC_SUCC != ulResult )
    {
        IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_DlDataRcv, ERROR, input para invalid!\n");

        return IMS_NIC_FAIL;
    }

    /* ͨ��enModemIdȡ���������������ݽṹ */
    pstImsNicDev    = IMS_NIC_GET_DEV_STRU_PTR(enModemId);

    /* ����ϵͳ����netClusterGet��ȡһ��cluster */
    pcCluster = IMS_NIC_CLUSTER_GET(pstImsNicDev->end.pNetPool, pstImsNicDev->pClPoolId);
    if ( VOS_NULL_PTR == pcCluster )
    {
        IMS_NIC_STATS_INC(IMS_NIC_STATS_GET_CLUSTER_FAIL, 1, enModemId);

        IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_DlDataRcv, ERROR, get cluster fail!\n");

        return IMS_NIC_FAIL;
    }

    pcTmpCluster            = pcCluster;
    pstImsNicElementTbl     = (IMS_NIC_ELEMENT_TAB_STRU *)IMS_NIC_GET_ELEMENT_TABLE(enModemId);

    /* ����IP���İ汾�� */
    ucIpType    = ((pucData[0] >> IMS_NIC_IP_VERSION_POS) & IMS_NIC_IP_VERSION_MASK);

    switch (ucIpType)
    {
        case IMS_NIC_IPV4_VERSION:
            IMS_NIC_STATS_INC(IMS_NIC_STATS_DL_RCV_IPV4_PKT, 1, enModemId);
            DRV_RT_MEMCPY(pcTmpCluster, &(pstImsNicElementTbl->stIpv4Ethhead), IMS_NIC_ETH_HDR_SIZE);
            break;

        case IMS_NIC_IPV6_VERSION:
            IMS_NIC_STATS_INC(IMS_NIC_STATS_DL_RCV_IPV6_PKT, 1, enModemId);
            DRV_RT_MEMCPY(pcTmpCluster, &(pstImsNicElementTbl->stIpv6Ethhead), IMS_NIC_ETH_HDR_SIZE);
            break;

        default:
            IMS_NIC_STATS_INC(IMS_NIC_STATS_DL_RCV_ERR_PKT, 1, enModemId);
            /* �ͷ�cluster�ڴ� */
            IMS_NIC_CLUSTER_FREE(pstImsNicDev->end.pNetPool, (VOS_UINT8 *)pcCluster);

            IMS_NIC_ERROR_LOG1(UEPS_PID_IMSNIC, "IMS_NIC_DlDataRcv, ERROR, ucIpType=%d error!\n", ucIpType);
            return IMS_NIC_FAIL;
    }

    pcTmpCluster    += IMS_NIC_ETH_HDR_SIZE;

    /* ���������pucData���ݵ���ȡ����cluster�ڴ��� */
    DRV_RT_MEMCPY(pcTmpCluster, pucData, usLen);

    /* ����ϵͳ����netClBlkGet����һ��ClBlk���ƿ� */
    pstClBlk = IMS_NIC_CLBLK_GET(pstImsNicDev->end.pNetPool);
    if ( VOS_NULL_PTR == pstClBlk )
    {
        /* �ͷ�cluster�ڴ� */
        IMS_NIC_CLUSTER_FREE(pstImsNicDev->end.pNetPool, (VOS_UINT8 *)pcCluster);
        IMS_NIC_STATS_INC(IMS_NIC_STATS_GET_CLBLK_FAIL, 1, enModemId);

        IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_DlDataRcv, ERROR, get ClBlk fail!\n");

        return IMS_NIC_FAIL;
    }

    /* ����ϵͳ����netMblkGet����һ��mBlk���ƿ� */
    pstMblk = IMS_NIC_MBLK_GET(pstImsNicDev->end.pNetPool);
    if ( VOS_NULL_PTR == pstMblk )
    {
        /* �ͷ�ClBlk���ƿ��ڴ��cluster�ڴ� */
        IMS_NIC_CLBLK_FREE(pstImsNicDev->end.pNetPool, pstClBlk);
        IMS_NIC_CLUSTER_FREE(pstImsNicDev->end.pNetPool, (VOS_UINT8 *)pcCluster);
        IMS_NIC_STATS_INC(IMS_NIC_STATS_GET_MBLK_FAIL, 1, enModemId);

        IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_DlDataRcv, ERROR, get mBlk fail!\n");

        return IMS_NIC_FAIL;
    }

    /* ����ϵͳ����netClBlkJoin��cluster��ӵ�ClBlk�ṹ�� */
    IMS_NIC_CLBLK_JOIN(pstClBlk, pcCluster, (VOS_INT)(usLen + IMS_NIC_ETH_HDR_SIZE));

    /* ����ϵͳ����netMblkClJoin��ClBlk��ӵ�mBlk�ṹ�� */
    IMS_NIC_MBLK_JOIN(pstMblk, pstClBlk);

    pstMblk->mBlkHdr.mData   = pcCluster;
    pstMblk->mBlkHdr.mFlags |= M_PKTHDR;
    pstMblk->mBlkHdr.mLen    = (VOS_UINT32)(usLen + IMS_NIC_ETH_HDR_SIZE);
    pstMblk->mBlkPktHdr.len  = (VOS_UINT32)(usLen + IMS_NIC_ETH_HDR_SIZE);

    /* �����ϲ�Э��(IP)�����ݽ��պ���END_RCV_RTN_CALL */
    END_RCV_RTN_CALL(&pstImsNicDev->end, pstMblk);

    /* ���з�������ͳ�� */
    IMS_NIC_STATS_INC(IMS_NIC_STATS_DL_SEND_PKT, 1, enModemId);

    /* ���������������ݽ����ֽ��� */
    IMS_NIC_STATS_INC(IMS_NIC_STATS_DL_RCV_BYTES, usLen, enModemId);

    return IMS_NIC_SUCC;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_UlDataSend
 ��������  : �ú�������MUX����������ݽ���CDS
 �������  : pucData      IP������
             usLen        IP������
             enModemId    ��ǰmodemID
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_SUCC��ʧ��ΪIMS_NIC_FAIL
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 IMS_NIC_UlDataSend(VOS_UINT8 *pucData, VOS_UINT16 usLen, MODEM_ID_ENUM_UINT16 enModemId)
{
    IMS_NIC_ENTITY_STRU                     *pstImsNicEntity;
    IMS_NIC_ETH_HEADER_STRU                 *pstEthHdr;
    VOS_UINT8                               *pucIpData;
    VOS_UINT16                               usIpDataLen;
    VOS_UINT32                               ulResult;
    VOS_UINT16                               usEtherType;


    pstImsNicEntity     = IMS_NIC_GET_ENTITY_STRU(enModemId);

    /* ����ָ��Ϊ�� */
    if ( VOS_NULL_PTR == pucData )
    {
        IMS_NIC_STATS_INC(IMS_NIC_STATS_UL_INPUT_NULL_PTR, 1, enModemId);

        IMS_NIC_ERROR_LOG_MSG(UEPS_PID_IMSNIC, "IMS_NIC_UlDataSend, ERROR, Input data is NULL!\n");

        return IMS_NIC_FAIL;
    }

    /* ���ݱ��ĳ��Ȳ�����Чֵ��Χ�ڣ���[MIN + OFFSET, MTU + OFFSET],
        ����MINΪ��СIP����С��OFFSETΪ14���ֽ�(14�ֽ���̫����ͷ����) */
    if ( (usLen < (IMS_NIC_MIN_IPPKT_LEN + IMS_NIC_ETH_HDR_SIZE)) ||
        (usLen > (IMS_NIC_GET_MTU + IMS_NIC_ETH_HDR_SIZE)) )
    {
        IMS_NIC_STATS_INC(IMS_NIC_STATS_UL_RCV_DATA_LEN_ERR, 1, enModemId);

        IMS_NIC_ERROR_LOG_MSG1(UEPS_PID_IMSNIC, "IMS_NIC_UlDataSend, ERROR, Input data len =%d is invalid!\n", usLen);

        return IMS_NIC_FAIL;
    }

    pstEthHdr   = (IMS_NIC_ETH_HEADER_STRU *)pucData;

    /* ������ת��Ϊ������ */
    usEtherType   = VOS_NTOHS(pstEthHdr->usEtherType);

    switch (usEtherType)
    {
        case IMS_NIC_ETHERTYPE_IPV4:
            IMS_NIC_STATS_INC(IMS_NIC_STATS_UL_RCV_IPV4_PKT, 1, enModemId);
            break;

        case IMS_NIC_ETHERTYPE_IPV6:
            IMS_NIC_STATS_INC(IMS_NIC_STATS_UL_RCV_IPV6_PKT, 1, enModemId);
            break;

        /* ��������ΪNOARP, �յ�ARP�������еݽ���ֱ�ӷ��� */
        case IMS_NIC_ETHERTYPE_ARP:
            IMS_NIC_STATS_INC(IMS_NIC_STATS_UL_RCV_ARP_PKT, 1, enModemId);
            return IMS_NIC_SUCC;

        default:
            IMS_NIC_STATS_INC(IMS_NIC_STATS_UL_RCV_ERR_PKT, 1, enModemId);
            IMS_NIC_ERROR_LOG_MSG1(UEPS_PID_IMSNIC, "IMS_NIC_UlDataSend, ERROR, receive err pkt, the usEthType = 0x%x!\n", usEtherType);
            return IMS_NIC_FAIL;
    }

    /* ����MACͷ��ƫ�Ƶ�ַ */
    pucIpData           = pucData + IMS_NIC_ETH_HDR_SIZE;
    usIpDataLen         = usLen - IMS_NIC_ETH_HDR_SIZE;

    pstImsNicEntity     = IMS_NIC_GET_ENTITY_STRU(enModemId);

    if ( VOS_NULL_PTR   == pstImsNicEntity->pUlDataSendFunc )
    {
        IMS_NIC_STATS_INC(IMS_NIC_STATS_UL_CB_FUN_NOT_REG, 1, enModemId);

        IMS_NIC_ERROR_LOG_MSG(UEPS_PID_IMSNIC, "IMS_NIC_UlDataSend, ERROR, pUlDataSendFunc is NULL!\n");

        return IMS_NIC_FAIL;
    }

    /* ����CDSע����������ݷ��ͺ��� */
    ulResult    = pstImsNicEntity->pUlDataSendFunc(pucIpData, usIpDataLen, enModemId);
    if( IMS_NIC_SUCC != ulResult )
    {
        IMS_NIC_STATS_INC(IMS_NIC_STATS_UL_CALL_CB_FUN_FAIL, 1, enModemId);

        IMS_NIC_ERROR_LOG_MSG(UEPS_PID_IMSNIC, "IMS_NIC_UlDataSend, ERROR, call pUlDataSendFunc fail!\n");

        return IMS_NIC_FAIL;
    }

    /* ���з�������ͳ�� */
    IMS_NIC_STATS_INC(IMS_NIC_STATS_UL_SEND_PKT, 1, enModemId);

    /* ���������������ݷ����ֽ��� */
    IMS_NIC_STATS_INC(IMS_NIC_STATS_UL_SEND_BYTES, usIpDataLen, enModemId);

    return IMS_NIC_SUCC;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_MemInit
 ��������  : ��ʼ�����绺��
 �������  : pstImsNicDev   -- ָ����Ҫ�������豸ʵ��
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_SUCC��ʧ��ΪIMS_NIC_FAIL
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 IMS_NIC_MemInit(IMS_NIC_DEV_STRU *pstImsNicDev)
{
    STATUS             iResult;


    /* ΪEND�����е�net pool�ṹ����ռ� */
    pstImsNicDev->end.pNetPool  = (NET_POOL_ID)IMS_NIC_MALLOC(sizeof(NET_POOL));
    if ( VOS_NULL_PTR == pstImsNicDev->end.pNetPool )
    {
        IMS_NIC_INIT_LOG_PRINT(UEPS_PID_IMSNIC, "IMS_NIC_MemInit, ERROR, Alloc pNetPool failed!\n");
        return IMS_NIC_FAIL;
    }

    /* ����mBlk���ƿ飬����mBlk��clBlk���ƿ�����Լ��Լ�Ԥ��������ڴ��С */
    pstImsNicDev->mclCfg.mBlkNum  = IMS_NIC_MBLK_NUM;
    pstImsNicDev->mclCfg.clBlkNum = IMS_NIC_CLBLK_NUM;
    pstImsNicDev->mclCfg.memSize  = (VOS_INT32)
        ((pstImsNicDev->mclCfg.mBlkNum * (MSIZE + sizeof (long))) +
        (pstImsNicDev->mclCfg.clBlkNum * (CL_BLK_SZ + sizeof(long))));

    /* ΪmBlk���ƿ����ռ� */
    pstImsNicDev->mclCfg.memArea =
        (VOS_CHAR *)IMS_NIC_MEMALIGN(sizeof(long), (VOS_UINT32)pstImsNicDev->mclCfg.memSize);
    if ( VOS_NULL_PTR == pstImsNicDev->mclCfg.memArea )
    {
        IMS_NIC_FREE(pstImsNicDev->end.pNetPool);
        IMS_NIC_INIT_LOG_PRINT(UEPS_PID_IMSNIC, "IMS_NIC_MemInit, ERROR, Failed to allocate memory for M_CL_CONFIG struct!\n");
        return IMS_NIC_FAIL;
    }

    /* ����Cluster���ƿ飬������������С�Լ�Ԥ��������ڴ��С */
    pstImsNicDev->clDesc.clSize  = IMS_NIC_CLUSTER_SIZE;
    pstImsNicDev->clDesc.clNum   = IMS_NIC_CLUSTER_NUM;
    pstImsNicDev->clDesc.memSize = (VOS_INT32)(pstImsNicDev->clDesc.clNum * (IMS_NIC_CLUSTER_SIZE + 8) + sizeof(int));

    /* ΪCluster���ƿ����ռ䣬��СΪpstImsNicDev->clDesc.memSize */
    pstImsNicDev->clDesc.memArea = IMS_NIC_MALLOC( (VOS_UINT32)(pstImsNicDev->clDesc.memSize) );
    if ( VOS_NULL_PTR == pstImsNicDev->clDesc.memArea )
    {
        IMS_NIC_FREE(pstImsNicDev->end.pNetPool);
        IMS_NIC_FREE(pstImsNicDev->mclCfg.memArea);
        IMS_NIC_INIT_LOG_PRINT(UEPS_PID_IMSNIC, "IMS_NIC_MemInit, ERROR, Failed to allocate memory for CL_DESC struct!\n");
        return IMS_NIC_FAIL;
    }

    /* ���õ�ϵͳ����netPoolInit��ʼ������� */
    iResult     = IMS_NETPOOL_INIT(pstImsNicDev->end.pNetPool, &pstImsNicDev->mclCfg, &pstImsNicDev->clDesc);
    if ( IMS_NIC_OK != iResult )
    {
        IMS_NIC_FREE(pstImsNicDev->end.pNetPool);
        IMS_NIC_FREE(pstImsNicDev->mclCfg.memArea);
        IMS_NIC_FREE(pstImsNicDev->clDesc.memArea);
        IMS_NIC_INIT_LOG_PRINT(UEPS_PID_IMSNIC, "IMS_NIC_MemInit, ERROR, Failed to initialize net pool!\n");
        return IMS_NIC_FAIL;
    }

    /* ���滺��ص�ID */
    pstImsNicDev->pClPoolId = IMS_NIC_POOL_ID_GET(pstImsNicDev->end.pNetPool, IMS_NIC_CLUSTER_SIZE);

    return IMS_NIC_SUCC;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_Parse
 ��������  : �����豸ʱ���øú������豸���г�ʼ��
 �������  : pinitString    -- ���ڳ�ʼ�����ַ���
 �������  : punit          -- ��������uint��
 �� �� ֵ  : �ɹ�����IMS_NIC_SUCC��ʧ�ܷ���IMS_NIC_FAIL
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 IMS_NIC_Parse(
    VOS_INT                             *punit,
    VOS_CHAR                            *pinitString
)
{
    VOS_CHAR        *pctok;
    VOS_CHAR        *pcHolder;


    pcHolder = VOS_NULL_PTR;

    if ( VOS_NULL_PTR == punit )
    {
        IMS_NIC_INIT_LOG_PRINT(UEPS_PID_IMSNIC, "IMS_NIC_Parse, ERROR, punit is NULL!\n");
        return IMS_NIC_FAIL;
    }

    /* ��pinitString��ȡ��unit number��Լ���ڵ�һ��ð��֮ǰ */
    pctok = strtok_r (pinitString, ":", &pcHolder);
    if ( VOS_NULL_PTR == pctok )
    {
        IMS_NIC_INIT_LOG_PRINT(UEPS_PID_IMSNIC, "IMS_NIC_Parse, ERROR, pctok is NULL!\n");
        return IMS_NIC_FAIL;
    }

    *punit = atoi(pctok);

    IMS_NIC_DEBUG_PRINT("IMS_NIC_Parse: *punit = %d\n", *punit);

    return IMS_NIC_SUCC;

}

/*****************************************************************************
 �� �� ��  : IMS_NIC_Start
 ��������  : �����豸ʱ���øú������豸���г�ʼ��
 �������  : pinitString    -- ���ڳ�ʼ�����ַ���
             nothing        -- ����Я��һЩ��������δʹ��
 �������  : ��
 �� �� ֵ  : ʧ�ܷ���VOS_NULL_PTR,�ɹ�����END_OBJ*
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
END_OBJ* IMS_NIC_Load(VOS_CHAR *pinitString, VOS_VOID *nothing)
{
    IMS_NIC_DEV_STRU                    *pstImsNicDev;
    VOS_UINT32                           ulResult;
    STATUS                               iResult;
    VOS_UINT8                           *pucMacAddr;
    LOCAL NET_FUNCS                     *pstImsNicEndFuncTable;
    VOS_INT                              unit;


    if ( VOS_NULL_PTR == pinitString )
    {
        return VOS_NULL_PTR;
    }

    if ( EOS == pinitString[0] )
    {
        /* ���������������ֿ�����pinitString�� */;
        bcopy (IMS_NIC_GET_NIC_NAME, pinitString, (VOS_INT)(VOS_StrLen(IMS_NIC_GET_NIC_NAME) + 1));
        return VOS_NULL_PTR;
    }

    /* ����pinitString�����л�ȡunit*/
    ulResult    = IMS_NIC_Parse(&unit, pinitString);
    if ( (IMS_NIC_SUCC != ulResult) || (unit >= (VOS_INT)IMS_NIC_MODEM_ID_BUTT) )
    {
        IMS_NIC_INIT_LOG_PRINT1(UEPS_PID_IMSNIC, "IMS_NIC_Load, ERROR, IMS_NIC_Parse failed! unit = %d\n", unit);
        return VOS_NULL_PTR;
    }

    /* ��ȡ���������豸�ռ� */
    pstImsNicDev    = IMS_NIC_GET_DEV_STRU_PTR(unit);
    /* ��ʼ������Ŀռ� */
    PS_MEM_SET(pstImsNicDev, 0, sizeof(IMS_NIC_DEV_STRU));

    pstImsNicDev->unit      = unit;

    pstImsNicEndFuncTable   = (NET_FUNCS *)IMS_NIC_GET_END_FUNC_TABLE_PTR;
    /* ����endLib�ṩ��END�����ʼ������END_OBJ_INIT��������������unit�Լ�ע���豸������ */
    iResult    =   END_OBJ_INIT(&pstImsNicDev->end,
                                    VOS_NULL_PTR,
                                    IMS_NIC_GET_NIC_NAME,
                                    pstImsNicDev->unit,
                                    pstImsNicEndFuncTable,
                                    "IMS NIC for VoLTE");
    if ( IMS_NIC_OK != iResult )
    {
        IMS_NIC_INIT_LOG_PRINT2(UEPS_PID_IMSNIC, "END_OBJ_INIT, ERROR, END_OBJ_INIT failed! iResult = 0x%x, unit = %d\n", iResult, pstImsNicDev->unit);
        return VOS_NULL_PTR;
    }

    pucMacAddr = (VOS_UINT8*)IMS_NIC_GET_DST_MAC_ADDR(pstImsNicDev->unit);

    /* ����endLib�ṩ��MIB��ʼ������END_MIB_INIT����������MAC��ַ��MTU��*/
    iResult    =   END_MIB_INIT( &pstImsNicDev->end,
                                     M2_ifType_ethernet_csmacd,
                                     pucMacAddr,
                                     IMS_NIC_ETHER_ADDR_LEN,
                                     (VOS_INT)IMS_NIC_GET_MTU,
                                     IMS_NIC_SPEED_100M);
    if ( IMS_NIC_OK != iResult )
    {
        IMS_NIC_INIT_LOG_PRINT1(UEPS_PID_IMSNIC, "END_OBJ_INIT, ERROR, END_MIB_INIT failed! iResult = 0x%x\n", iResult);
        return VOS_NULL_PTR;
    }

    /* ��ʼ��IMS�������������绺�� */
    ulResult    = IMS_NIC_MemInit(pstImsNicDev);
    if ( IMS_NIC_SUCC   != ulResult )
    {
        IMS_NIC_INIT_LOG_PRINT(UEPS_PID_IMSNIC, "IMS_NIC_MemInit, ERROR, IMS_NIC_MemInit failed!\n");
        return VOS_NULL_PTR;
    }

    /* ���ñ�ǣ������豸�Ѿ�׼������ */
    END_OBJ_READY(&pstImsNicDev->end, IFF_NOTRAILERS | IFF_NOARP);

    return ((END_OBJ *)pstImsNicDev);
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_Init
 ��������  : IMS����������ʼ������
 �������  : enModemId      --��ǰModem��
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_SUCC��ʧ��Ϊ��Ӧ�Ĵ�����
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 IMS_NIC_Init (MODEM_ID_ENUM_UINT16 enModemId)
{
    IMS_NIC_ENTITY_STRU         *pstImsNicEntity;
    IMS_NIC_SEND_UL_DATA_FUNC    pFunc;
    VOS_INT32                   *pulCookie;
    STATUS                       iResult;


    if ( enModemId >= IMS_NIC_MODEM_ID_BUTT )
    {
        /* Ĭ�ϼ�¼��Modem0��ʵ���� */;
        IMS_NIC_INIT_LOG_PRINT1(UEPS_PID_IMSNIC, "IMS_NIC_Init, ERROR, Input modem id invalid!\n", enModemId);

        return IMS_NIC_MODEM_ID_INVALID;
    }

    pstImsNicEntity     = IMS_NIC_GET_ENTITY_STRU(enModemId);

    /* �����������Ѿ���ʼ�� */
    if ( VOS_TRUE == pstImsNicEntity->ulImsNicInitFlag )
    {
        IMS_NIC_INIT_LOG_PRINT(UEPS_PID_IMSNIC, "IMS_NIC_Init, WARNNING, IMS NIC is already initialed!\n");
        return IMS_NIC_ALREADY_INITED;
    }

    /* ����muxDevLoad��������IMS��������������unit����enModemId��endLoad����IMS_NIC_Load���� */
    pulCookie = IMS_NIC_MUX_DEV_LOAD((VOS_INT)enModemId, (VOS_VOID *)IMS_NIC_Load);

    /* ����muxDevStart��������IMS�������� */
    iResult   = IMS_NIC_MUX_START(pulCookie);

    if ( IMS_NIC_OK != iResult )
    {
        IMS_NIC_INIT_LOG_PRINT1(UEPS_PID_IMSNIC, "IMS_NIC_Init, ERROR, muxDevStart failed! iResult = %d\n", iResult);
        return IMS_NIC_SYSCALLFAIL;
    }

    /* ��ʼ����������ʵ��pstImsNicEntity */
    PS_MEM_SET(pstImsNicEntity, 0, sizeof(IMS_NIC_ENTITY_STRU));

    pFunc   = IMS_NIC_GET_SAVED_UL_DATA_FUNC(enModemId);

    /* ��������������ݷ��ͻص�������ֵ��ʵ���� */
    pstImsNicEntity->pUlDataSendFunc    = pFunc;

    /* ��Э��ջ�������������� */
    IMS_NIC_DRV_ETH_INIT(IMS_NIC_GET_NIC_NAME, (VOS_INT)enModemId);

    /* ���ϳ�ʼ����� */
    pstImsNicEntity->ulImsNicInitFlag = VOS_TRUE;

    return IMS_NIC_SUCC;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_InitAll
 ��������  : ��ʼ�����е�IMS��������
 �������  : ��
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_SUCC��ʧ��ΪIMS_NIC_FAIL
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 IMS_NIC_InitAll(VOS_VOID)
{
    VOS_UINT32      ulResult;
    VOS_UINT16      usIndex;


    if (VOS_OK != IMS_NIC_MNTN_LogEntInit(UEPS_PID_IMSNIC))
    {
        IMS_NIC_INIT_LOG_PRINT(UEPS_PID_IMSNIC, "IMS_NIC_InitAll, ERROR, muxDevStart failed! iResult = %d\n");

        return IMS_NIC_FAIL;
    }

    /* ��ʼ��IMS�������� */
    for (usIndex = 0 ; usIndex < IMS_NIC_MODEM_ID_BUTT ; usIndex++)
    {
        ulResult    = IMS_NIC_Init(usIndex);
        if ( IMS_NIC_SUCC != ulResult )
        {
            /* ��ӡ��ulResult��ֵ */
            IMS_NIC_INIT_LOG_PRINT2(UEPS_PID_IMSNIC, "IMS_NIC_InitAll, ERROR, IMS_NIC_Init Failed, usIndex=%d, ulResult=0x%x\n", usIndex, ulResult);
            return IMS_NIC_FAIL;
        }
    }

#ifdef  ST_IMS_NIC
    ST_IMS_NIC_InitQ();
#endif

    return IMS_NIC_SUCC;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_Free
 ��������  : IMS��������ȥ��ʼ������
 �������  : enModemId      --��ǰModem��
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_SUCC��ʧ��Ϊ��Ӧ�Ĵ�����
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 IMS_NIC_Free (MODEM_ID_ENUM_UINT16 enModemId)
{
    IMS_NIC_ENTITY_STRU         *pstImsNicEntity;
    STATUS                       iResult;


    if ( enModemId >= IMS_NIC_MODEM_ID_BUTT )
    {
        /* Ĭ�ϼ�¼��Modem0��ʵ���� */;
        IMS_NIC_STATS_INC(IMS_NIC_STATS_MODEM_ID_ERR_OTHER, 1, IMS_NIC_MODEM_ID_0);

        IMS_NIC_ERROR_LOG1(UEPS_PID_IMSNIC, "IMS_NIC_Free, ERROR, Input modem id invalid!\n", enModemId);

        return IMS_NIC_MODEM_ID_INVALID;
    }

    pstImsNicEntity     = IMS_NIC_GET_ENTITY_STRU(enModemId);

    /* ����������û�г�ʼ�� */
    if ( VOS_TRUE != pstImsNicEntity->ulImsNicInitFlag )
    {
        IMS_NIC_STATS_INC(IMS_NIC_STATS_NOT_INITED_ERR, 1, enModemId);

        return IMS_NIC_NOT_INITED;
    }

    /* ж��IMS���������������ֱ�Ϊ��������enModemId */
    iResult     = IMS_NIC_MUX_DEV_UNLOAD(IMS_NIC_GET_NIC_NAME, (VOS_INT)enModemId);
    if ( IMS_NIC_OK != iResult )
    {
        IMS_NIC_STATS_INC(IMS_NIC_STATS_SYS_CALL_FAIL, 1, enModemId);

        IMS_NIC_ERROR_LOG1(UEPS_PID_IMSNIC, "IMS_NIC_Free, ERROR, call muxDevUnload fail!\n", enModemId);

        return IMS_NIC_SYSCALLFAIL;
    }

    pstImsNicEntity->ulImsNicInitFlag = VOS_FALSE;

    return IMS_NIC_SUCC;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_Start
 ��������  : �ú����������豸���غ������豸
 �������  : pstImsNicDev   -- ָ����Ҫ�������豸ʵ��
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_OK��ʧ��ΪIMS_NIC_ERR
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
LOCAL STATUS IMS_NIC_Start(IMS_NIC_DEV_STRU *pstImsNicDev)
{
    IMS_NIC_DEBUG_PRINT("IMS_NIC_Start: IMS NIC Start!\n");

    /* �������������ΪIFF_UP | IFF_RUNNING */
    END_FLAGS_SET(&pstImsNicDev->end, IFF_UP | IFF_RUNNING);

    return IMS_NIC_OK;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_Stop
 ��������  : �ú�������ֹͣ����������
 �������  : pstImsNicDev   -- ָ����Ҫ�������豸ʵ��
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_OK��ʧ��ΪIMS_NIC_ERR
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
LOCAL STATUS IMS_NIC_Stop(IMS_NIC_DEV_STRU *pstImsNicDev)
{
    IMS_NIC_DEBUG_PRINT("IMS_NIC_Stop: IMS NIC Stop!\n");

    /* �����������Startʱ�õ�flags */
    END_FLAGS_CLR(&pstImsNicDev->end, IFF_UP | IFF_RUNNING);

    return IMS_NIC_OK;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_Unload
 ��������  : �ú�������������ж��ʱ�����ã���������������ж��ʱ�����豸��Դ
 �������  : pstImsNicDev   -- ָ����Ҫ�������豸ʵ��
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_OK��ʧ��ΪIMS_NIC_ERR
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
LOCAL STATUS IMS_NIC_Unload(IMS_NIC_DEV_STRU *pstImsNicDev)
{
    IMS_NIC_ENTITY_STRU                 *pstImsNicEntity;
    IMS_NIC_MODEM_ID_ENUM_UINT16         enModemId;


    IMS_NIC_DEBUG_PRINT("IMS_NIC_Unload entered!\n");

    enModemId   = (IMS_NIC_MODEM_ID_ENUM_UINT16)(pstImsNicDev->unit);

    if ( enModemId >= IMS_NIC_MODEM_ID_BUTT )
    {
        /* Ĭ�ϼ�¼��Modem0��ʵ���� */;
        IMS_NIC_STATS_INC(IMS_NIC_STATS_MODEM_ID_ERR_OTHER, 1, IMS_NIC_MODEM_ID_0);

        IMS_NIC_ERROR_LOG1(UEPS_PID_IMSNIC, "IMS_NIC_Unload, ERROR, Input modem id invalid!\n", enModemId);

        return IMS_NIC_ERROR;
    }

    /* �����������ʵ�� */
    pstImsNicEntity     = IMS_NIC_GET_ENTITY_STRU(pstImsNicDev->unit);
    PS_MEM_SET(pstImsNicEntity, 0, sizeof(IMS_NIC_ENTITY_STRU));

    /* ����������������ļ�¼��Ϣ */
    IMS_NIC_SET_MTU(IMS_NIC_MTU_DEFAULT_VALUE);
    IMS_NIC_SAVE_UL_DATA_FUNC(enModemId, VOS_NULL_PTR);

    /* �����������绺�� */
    if ( VOS_NULL_PTR != pstImsNicDev->mclCfg.memArea )
    {
        IMS_NIC_FREE(pstImsNicDev->mclCfg.memArea);
        pstImsNicDev->mclCfg.memArea = VOS_NULL_PTR;
    }

    if ( VOS_NULL_PTR != pstImsNicDev->clDesc.memArea )
    {
        IMS_NIC_FREE(pstImsNicDev->clDesc.memArea);
        pstImsNicDev->clDesc.memArea = VOS_NULL_PTR;
    }

    if ( VOS_NULL_PTR != pstImsNicDev->end.pNetPool )
    {
        IMS_NIC_NETPOOL_DEL(pstImsNicDev->end.pNetPool);
        pstImsNicDev->end.pNetPool  = VOS_NULL_PTR;
    }

    /* �ͷ�����������END����*/
    END_OBJECT_UNLOAD(&pstImsNicDev->end);

    return IMS_NIC_OK;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_Ioctl
 ��������  : �ú�������ִ��IO��������
 �������  : pstImsNicDev   -- ָ����Ҫ�������豸ʵ��
             cmd            -- �����ID
             data           -- ���������Ĳ���
 �������  : ��
 �� �� ֵ  : IMS_NIC_OK��ʾ�ɹ���IMS_NIC_INVAL��ʾ������Ч��IMS_NIC_NOTSUP��ʾ��֧�ָ�����
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
LOCAL VOS_INT IMS_NIC_Ioctl(IMS_NIC_DEV_STRU *pstImsNicDev, VOS_INT cmd, caddr_t data)
{
    VOS_INT         iResult;
    VOS_INT         iFlag;


    iResult     = IMS_NIC_OK;
    IMS_NIC_DEBUG_PRINT("IMS_NIC_Ioctl: cmd = %d!\n", cmd);

    switch (cmd)
        {
            /* ����IMS����������MAC��ַ */
            case EIOCSADDR:
                if ( VOS_NULL_PTR == data )
                {
                    IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_Ioctl, ERROR, data is NULL!\n");
                    return IMS_NIC_INVAL;
                }

                IMS_NIC_DEBUG_PRINT("IMS_NIC_Ioctl: END_HADDR_LEN = %d!\n", IMS_NIC_END_HADDR_LEN(&pstImsNicDev->end));

                bcopy ((VOS_CHAR *)data,
                       (VOS_CHAR *) IMS_NIC_END_HADDR (&pstImsNicDev->end),
                       IMS_NIC_END_HADDR_LEN(&pstImsNicDev->end));

                break;

            /* ��ȡIMS����������MAC��ַ */
            case EIOCGADDR:
                if ( VOS_NULL_PTR == data )
                {
                    IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_Ioctl, ERROR, data is NULL!\n");
                    return IMS_NIC_INVAL;
                }

                IMS_NIC_DEBUG_PRINT("IMS_NIC_Ioctl: END_HADDR_LEN = %d!\n", IMS_NIC_END_HADDR_LEN(&pstImsNicDev->end));

                bcopy ((VOS_CHAR *)IMS_NIC_END_HADDR (&pstImsNicDev->end), (VOS_CHAR *)data,
                        IMS_NIC_END_HADDR_LEN (&pstImsNicDev->end));
                break;

            /* ��ȡIMS�����������豸��־λflags */
            case EIOCGFLAGS:
                if ( VOS_NULL_PTR == data )
                {
                    IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_Ioctl, ERROR, data is NULL!\n");
                    return IMS_NIC_INVAL;
                }
                *(VOS_INT *)data = END_FLAGS_GET(&pstImsNicDev->end);
                break;

            /* ����IMS�����������豸��־λflags */
            case EIOCSFLAGS:
                iFlag   = (VOS_INT)data;
                if ( iFlag < 0 )
                {
                    iFlag   = -iFlag;
                    iFlag--;
                    END_FLAGS_CLR (&pstImsNicDev->end, iFlag);
                }
                else
                {
                    END_FLAGS_SET (&pstImsNicDev->end, iFlag);
                }
                break;

            /* ��ȡ��̫����ͷ�Ĵ�С */
            case EIOCGHDRLEN:
                if ( VOS_NULL_PTR == data )
                {
                    IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_Ioctl, ERROR, data is NULL!\n");
                    return IMS_NIC_INVAL;
                }
                *(VOS_INT *)data = IMS_NIC_ETH_HDR_SIZE;
                break;

            /* ���� MIB ��Ϣ */
            case EIOCGMIB2233:
            case EIOCGMIB2:
                if( VOS_NULL_PTR == data )
                {
                    IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_Ioctl, ERROR, data is NULL!\n");
                    return IMS_NIC_INVAL;
                }
                bcopy((VOS_CHAR *)&pstImsNicDev->end.mib2Tbl, (VOS_CHAR *)data,
                        sizeof(pstImsNicDev->end.mib2Tbl));
                break;

            default:
                iResult = IMS_NIC_NOTSUP;
            break;
        }

        return iResult;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_MblkCopy
 ��������  : �ú������𽫲�������Mblk��Ӧ��cluster�ڴ濽�����������ڴ���
 �������  : pstSrcBuf   -- �洢���ݵĻ����ID
             pucDstData  -- Ŀ���ڴ�
 �������  : ��
 �� �� ֵ  : ���ؿ�������Ч����
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT16 IMS_NIC_MblkCopy(M_BLK_ID pstSrcBuf, VOS_UINT8 *pucDstData)
{
    VOS_UINT16       usDataLen;


    if ( (VOS_NULL_PTR == pstSrcBuf) || (VOS_NULL_PTR == pucDstData) )
    {
        IMS_NIC_ERROR_LOG_MSG(UEPS_PID_IMSNIC, "IMS_NIC_MblkCopy, ERROR, Input para invalid!\n");
        /* ���������򷵻ؿ�������Ч����Ϊ0 */
        return 0;
    }

    usDataLen   = 0;

    while ( VOS_NULL_PTR != pstSrcBuf )
    {
        usDataLen   += (VOS_UINT16)(pstSrcBuf->mBlkHdr.mLen);
        if ( usDataLen <= IMS_NIC_UL_SEND_BUFF_SIZE )
        {
            DRV_RT_MEMCPY(pucDstData, (VOS_UINT8 *)(pstSrcBuf->mBlkHdr.mData), (VOS_UINT16)(pstSrcBuf->mBlkHdr.mLen));
            pucDstData  += (VOS_UINT16)(pstSrcBuf->mBlkHdr.mLen);
            pstSrcBuf    = pstSrcBuf->mBlkHdr.mNext;
        }
        else
        {
            IMS_NIC_ERROR_LOG_MSG1(UEPS_PID_IMSNIC, "IMS_NIC_MblkCopy, ERROR, usDataLen = %d invalid!\n", usDataLen);
            return 0;
        }
    }

    return usDataLen;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_Send
 ��������  : �ú�������MUX����������ݽ���CDS
 �������  : pstImsNicDev   -- ָ����Ҫ�������豸ʵ��
             pBuf           -- �洢���ݵĻ����ID
 �������  : ��
 �� �� ֵ  : �ɹ�:VOS_OK��ʧ��ΪVOS_ERR
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
LOCAL STATUS IMS_NIC_Send(IMS_NIC_DEV_STRU *pstImsNicDev, M_BLK_ID pstBuf)
{
    M_BLK_ID                            pstTmpBuf;
    VOS_UINT32                          ulResult;
    VOS_UINT16                          usDataLen;
    VOS_UINT8                          *pucData;
    MODEM_ID_ENUM_UINT16                enModemId;


    pstTmpBuf   = pstBuf;
    enModemId   = (MODEM_ID_ENUM_UINT16)(pstImsNicDev->unit);

    /* pstImsNicDev�м�¼��unit�쳣 */
    if ( enModemId >= IMS_NIC_MODEM_ID_BUTT )
    {
        /* ����ϵͳ�����ͷŸ�mBlk��ʽ�ڴ� */
        IMS_NIC_MBLK_CHAIN_FREE(pstBuf);

        /* Ĭ�ϼ�¼��Modem0��ʵ���� */;
        IMS_NIC_STATS_INC(IMS_NIC_STATS_MODE_ID_ERR_IN_UL_PROC, 1, IMS_NIC_MODEM_ID_0);

        IMS_NIC_ERROR_LOG1(UEPS_PID_IMSNIC, "IMS_NIC_Send, ERROR, Input modem id invalid!\n", enModemId);

        return IMS_NIC_ERROR;
    }

    END_TX_SEM_TAKE(&pstImsNicDev->end, WAIT_FOREVER);

    while ( VOS_NULL_PTR != pstTmpBuf )
    {
        pucData     = (VOS_UINT8 *)(pstTmpBuf->mBlkHdr.mData);
        usDataLen   = (VOS_UINT16)(pstTmpBuf->mBlkHdr.mLen);

        /* �յ������ݰ��ڴ治�������������ݿ��� */
        if ( VOS_NULL_PTR != pstTmpBuf->mBlkHdr.mNext )
        {
            IMS_NIC_STATS_INC(IMS_NIC_STATS_UL_PKT_MEM_SEG, 1, enModemId);

            usDataLen   = IMS_NIC_MblkCopy(pstTmpBuf, pstImsNicDev->aucSendBuf);
            if ( 0 == usDataLen )
            {
                /* ���������¸����ݰ� */
                pstTmpBuf = pstTmpBuf->mBlkHdr.mNextPkt;
                IMS_NIC_STATS_INC(IMS_NIC_STATS_MBLK_COPY_ERR, 1, enModemId);
                continue;
            }

            pucData     = pstImsNicDev->aucSendBuf;
        }

        /* �������ݴ��� */
        ulResult    = IMS_NIC_UlDataSend(pucData, usDataLen, enModemId);
        if ( IMS_NIC_SUCC != ulResult )
        {
            IMS_NIC_ERROR_LOG_MSG(UEPS_PID_IMSNIC, "IMS_NIC_Send, ERROR, IMS_NIC_UlDataSend failed!\n");
        }

        pstTmpBuf = pstTmpBuf->mBlkHdr.mNextPkt;
    };

    /* ����ϵͳ�����ͷŸ�mBlk��ʽ�ڴ� */
    IMS_NIC_MBLK_CHAIN_FREE(pstBuf);

    END_TX_SEM_GIVE(&pstImsNicDev->end);

    return IMS_NIC_OK;

}

/*****************************************************************************
 �� �� ��  : IMS_NIC_MCastAddrAdd
 ��������  : ���豸�����һ���ಥ��ַ
 �������  : pstImsNicDev   -- ָ����Ҫ�������豸ʵ��
             pAddress       -- ��ַ�ַ���
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_OK��ʧ��ΪIMS_NIC_ERR
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
LOCAL STATUS IMS_NIC_MCastAddrAdd(IMS_NIC_DEV_STRU *pstImsNicDev, VOS_CHAR *pAddress)
{
    IMS_NIC_DEBUG_PRINT("IMS_NIC_MCastAddrAdd entered!\n");

    return IMS_NIC_OK;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_MCastAddrDel
 ��������  : ɾ���豸�ϵ�һ���ಥ��ַ
 �������  : pstImsNicDev   -- ָ����Ҫ�������豸ʵ��
             pAddress       -- ��ַ�ַ���
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_OK��ʧ��ΪIMS_NIC_ERR
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
LOCAL STATUS IMS_NIC_MCastAddrDel(IMS_NIC_DEV_STRU *pstImsNicDev, VOS_CHAR *pAddress)
{
    IMS_NIC_DEBUG_PRINT("IMS_NIC_MCastAddrDel entered!\n");

    return IMS_NIC_OK;
}


/*****************************************************************************
 �� �� ��  : IMS_NIC_MCastAddrGet
 ��������  : ��ѯ���豸��ע��Ķಥ��ַ�б�
 �������  : pstImsNicDev   -- ָ����Ҫ�������豸ʵ��
             pstMultiTable  -- �ಥ��ַ�б�
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_OK��ʧ��ΪIMS_NIC_ERR
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
LOCAL STATUS IMS_NIC_MCastAddrGet(IMS_NIC_DEV_STRU *pstImsNicDev, MULTI_TABLE *pstMultiTable)
{
    IMS_NIC_DEBUG_PRINT("IMS_NIC_MCastAddrGet entered!\n");

    return IMS_NIC_OK;
}


/*****************************************************************************
 �� �� ��  : IMS_NIC_PollSend
 ��������  : �豸����ѯģʽ�µ����ݷ��ͺ���
 �������  : pstImsNicDev   -- ָ����Ҫ�������豸ʵ��
             pstBuf         -- �洢���ݵĻ����ID
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_OK��ʧ��ΪIMS_NIC_ERR
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
LOCAL STATUS IMS_NIC_PollSend(IMS_NIC_DEV_STRU *pstImsNicDev, M_BLK_ID pstBuf)
{
    IMS_NIC_DEBUG_PRINT("IMS_NIC_PollSend entered!\n");

    return IMS_NIC_OK;
}


/*****************************************************************************
 �� �� ��  : IMS_NIC_PollRcv
 ��������  : �豸����ѯģʽ�µ����ݽ��պ���
 �������  : pstImsNicDev   -- ָ����Ҫ�������豸ʵ��
             pstBuf         -- �洢���ݵĻ����ID
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_OK��ʧ��ΪIMS_NIC_ERR
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
LOCAL STATUS IMS_NIC_PollRcv(IMS_NIC_DEV_STRU *pstImsNicDev, M_BLK_ID pstBuf)
{
    IMS_NIC_DEBUG_PRINT("IMS_NIC_PollRcv entered!\n");

    return IMS_NIC_OK;
}

#if 0
/*****************************************************************************
 �� �� ��  : IMS_NIC_FormAddress
 ��������  : Ϊ������������ݵ�mBlk�����һ��֡ͷ,(��ֱ�ӵ���endEtherAddressForm)
 �������  :
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_OK��ʧ��ΪIMS_NIC_ERR
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
LOCAL M_BLK_ID IMS_NIC_FormAddress(
    M_BLK_ID                            pNBuff,
    M_BLK_ID                            pSrcAddr,
    M_BLK_ID                            pDstAddr,
    BOOL                                bcastFlag
)
{
    IMS_NIC_DEBUG_PRINT("IMS_NIC_FormAddress entered!\n");

    return pNBuff;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_PacketDataGet
 ��������  : ��ȡ�������ݸ������ֵ�ƫ��,(��ֱ�ӵ���endEtherPacketDataGet)
 �������  :
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_OK��ʧ��ΪIMS_NIC_ERR
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
LOCAL STATUS IMS_NIC_PacketDataGet(M_BLK_ID pMblk, LL_HDR_INFO *pLinkHdrInfo)
{
    IMS_NIC_DEBUG_PRINT("IMS_NIC_PacketDataGet entered!\n");

    /* destination addr offset in mBlk */
    pLinkHdrInfo->destAddrOffset = 0;

    /* destination address size */
    pLinkHdrInfo->destSize       = 0;

    /* source address offset in mBlk */
    pLinkHdrInfo->srcAddrOffset  = 0;

    /* source address size */
    pLinkHdrInfo->srcSize        = 0;

    /* control info offset in mBlk */
    pLinkHdrInfo->ctrlAddrOffset = 0;

    /* control info size */
    pLinkHdrInfo->ctrlSize       = 0;

    /* type of the packet */
    pLinkHdrInfo->pktType        = IMS_NIC_ETHERTYPE_IPV4;

    /* data offset in the mBlk */
    pLinkHdrInfo->dataOffset     = 0;

    return IMS_NIC_OK;
}

/*****************************************************************************
 �� �� ��  : IMS_NIC_AddrGet
 ��������  : ��ȡ���ĵĵ�ַ��Ϣ,(��ֱ�ӵ���endEtherPacketAddrGet)
 �������  :
 �������  : ��
 �� �� ֵ  : �ɹ�:IMS_NIC_OK��ʧ��ΪIMS_NIC_ERR
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
LOCAL STATUS IMS_NIC_AddrGet(
    M_BLK_ID                  pMblk,
    M_BLK_ID                  pSrc,
    M_BLK_ID                  pDst,
    M_BLK_ID                  pESrc,
    M_BLK_ID                  pEDst
)
{
    IMS_NIC_DEBUG_PRINT("IMS_NIC_FormAddress entered!\n");

    return IMS_NIC_OK;
}
#endif

/*****************************************************************************
 �� �� ��  : IMS_NIC_GetMntnInfo
 ��������  : ��ȡIMS����������ͳ����Ϣ�������������ϱ�
 �������  : usType         -- ��ά�ɲ���Ϣ����
 �������  : pucData        -- �洢���ݵ��ڴ�
             pulLen         -- ���ݳ���
 �� �� ֵ  : �ɹ�:IMS_NIC_OK��ʧ��ΪIMS_NIC_FAIL
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : caikai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 IMS_NIC_GetMntnInfo
(
    VOS_UINT8                       *pucData,
    VOS_UINT32                      *pulLen,
    VOS_UINT16                       usType
)
{
    VOS_UINT16                      usIndex;
    IMS_NIC_ENTITY_STRU            *pstImsNicEntity;
    VOS_UINT32                      ulModemCnt;


    if ( VOS_NULL_PTR == pucData )
    {
        IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_GetMntnInfo, ERROR, Input pucData id null!\n");
        return IMS_NIC_FAIL;
    }

    if ( VOS_NULL_PTR == pulLen )
    {
        IMS_NIC_ERROR_LOG(UEPS_PID_IMSNIC, "IMS_NIC_GetMntnInfo, ERROR, Input pulLen id null!\n");
        return IMS_NIC_FAIL;
    }

    *pulLen     = 0;

    /* ��дModem���� */
    ulModemCnt  = (VOS_UINT32)IMS_NIC_MODEM_ID_BUTT;
    DRV_RT_MEMCPY(pucData, &ulModemCnt, sizeof(ulModemCnt));
    pucData     += sizeof(ulModemCnt);

    for (usIndex = 0 ; usIndex < IMS_NIC_MODEM_ID_BUTT ; usIndex++)
    {
        pstImsNicEntity     = IMS_NIC_GET_ENTITY_STRU(usIndex);

        /* ��дmodemId */
        DRV_RT_MEMCPY(pucData, &usIndex, IMS_NIC_MODEM_ID_LEN);
        pucData     += IMS_NIC_MODEM_ID_OFFSET;
        *pulLen     += IMS_NIC_MODEM_ID_OFFSET;

        /* ����ͳ����Ϣ */
        DRV_RT_MEMCPY(pucData, (VOS_UINT8 *)(&(pstImsNicEntity->stImsNicStatsInfo)), sizeof(IMS_NIC_STATS_INFO_STRU));
        pucData     += sizeof(IMS_NIC_STATS_INFO_STRU);
        *pulLen     += sizeof(IMS_NIC_STATS_INFO_STRU);
    }

    return IMS_NIC_SUCC;
}
VOS_VOID IMS_NIC_Help(VOS_VOID)
{
    vos_printf("��ӡ����״̬��Ϣ��IMS_NIC_ShowEntityState \n");
    vos_printf("��ӡ����ͳ����Ϣ��IMS_NIC_ShowStats \n");
    vos_printf("MuxShow��IMS_NIC_MuxShow \n");
}



VOS_VOID IMS_NIC_ShowEntityState(VOS_VOID)
{
    VOS_UINT8                       ucRabId;
    VOS_INT32                       iIndx1;
    VOS_INT32                       iIndx2;
    VOS_INT32                       iIndx3;
    VOS_UINT16                     *pusIpv6;
    VOS_UINT8                      *pucIpv4;
    IMS_NIC_ENTITY_STRU            *pstImsNicEntity;
    IMS_NIC_SINGLE_DNS_INFO_STRU   *pstSingleDnsInfo;
    IMS_NIC_SINGLE_PDN_INFO_STRU   *pstSinglePdnInfo;


    for (iIndx1 = 0; iIndx1 < IMS_NIC_MODEM_ID_BUTT; iIndx1++)
    {
        pstImsNicEntity = IMS_NIC_GET_ENTITY_STRU(iIndx1);
        vos_printf("======================Entity info for Modem %d ======================\n", iIndx1);
        vos_printf("ʵ���ʼ��״̬:             %d \n", pstImsNicEntity->ulImsNicInitFlag);
        vos_printf("���з����ص�����ָ���ַ:   0x%x \n", pstImsNicEntity->pUlDataSendFunc);
        vos_printf("��ǰ����PDN����:            %d \n", pstImsNicEntity->stImsNicPdnInfo.iPdnCfgCnt);
        vos_printf("��ǰ���õ�DNS����:          %d \n", pstImsNicEntity->stImsNicPdnInfo.stImsNicDnsInfo.ucDnsServerCnt);

        for (iIndx2 = 0; iIndx2 < IMS_NIC_MAX_DNS_SERVER_NUM; iIndx2++)
        {
            pstSingleDnsInfo = &pstImsNicEntity->stImsNicPdnInfo.stImsNicDnsInfo.astImsNicDnsInfo[iIndx2];
            if (IMS_NIC_IPV6_ADDR_LENGTH == pstSingleDnsInfo->ucIpAddrLen)
            {
                pusIpv6 = (VOS_UINT16 *)pstSingleDnsInfo->aucDnsAddr;

                vos_printf("%s �����ô���:%d, ��ַ����:%d, ��ַ:%x:%x:%x:%x:%x:%x:%x:%x \n",
                            g_stImsNicManageTbl.pcDnsSSysVarName[iIndx2], pstSingleDnsInfo->cDnsRef, pstSingleDnsInfo->ucIpAddrLen,
                            VOS_HTONS(pusIpv6[0]), VOS_HTONS(pusIpv6[1]), VOS_HTONS(pusIpv6[2]), VOS_HTONS(pusIpv6[3]), VOS_HTONS(pusIpv6[4]), VOS_HTONS(pusIpv6[5]), VOS_HTONS(pusIpv6[6]), VOS_HTONS(pusIpv6[7]));
            }
            else if (IMS_NIC_IPV4_ADDR_LENGTH == pstSingleDnsInfo->ucIpAddrLen)
            {
                pucIpv4 = pstSingleDnsInfo->aucDnsAddr;

                vos_printf("%s �����ô���:%d, ��ַ����:%d, ��ַ:%d.%d.%d.%d \n",
                            g_stImsNicManageTbl.pcDnsSSysVarName[iIndx2], pstSingleDnsInfo->cDnsRef, pstSingleDnsInfo->ucIpAddrLen,
                            pucIpv4[0], pucIpv4[1], pucIpv4[2], pucIpv4[3]);
            }
            else
            {
                vos_printf("%s �����ô���:%d, ��ַ����:%d \n", g_stImsNicManageTbl.pcDnsSSysVarName[iIndx2], pstSingleDnsInfo->cDnsRef, pstSingleDnsInfo->ucIpAddrLen);
            }

        }

        for (iIndx3 = 0; iIndx3 < IMS_NIC_RAB_ID_MAX_NUM; iIndx3++)
        {
            pstSinglePdnInfo = &pstImsNicEntity->stImsNicPdnInfo.astImsNicPdnInfo[iIndx3];

            ucRabId = (VOS_UINT8)iIndx3 + IMS_NIC_RAB_ID_OFFSET;

            vos_printf("PDN %d DNS ��������[primaryns]:%d, [secondaryns]:%d, [tertiaryns]:%d, [quaternaryns]:%d \n", ucRabId, pstSinglePdnInfo->aucDnsFlag[0], pstSinglePdnInfo->aucDnsFlag[1], pstSinglePdnInfo->aucDnsFlag[2], pstSinglePdnInfo->aucDnsFlag[3]);
            vos_printf("PDN %d �Ƿ�����IPv4:%d \n", ucRabId, pstSinglePdnInfo->bitOpIpv4PdnInfo);

            if (VOS_TRUE == pstSinglePdnInfo->bitOpIpv4PdnInfo)
            {
                pucIpv4 = pstSinglePdnInfo->stIpv4PdnInfo.aucIpV4Addr;

                vos_printf("PDN %d IPv4��ַ:%d.%d.%d.%d \n", ucRabId, pucIpv4[0], pucIpv4[1], pucIpv4[2], pucIpv4[3]);
                vos_printf("PDN %d IPv4��DNS��Ϣ, bitOpDnsPrim:%d, bitOpDnsPrimCfged:%d \n", ucRabId, pstSinglePdnInfo->stIpv4PdnInfo.bitOpDnsPrim, pstSinglePdnInfo->bitOpIpv4DnsPrimCfged);

                pucIpv4 = pstSinglePdnInfo->stIpv4PdnInfo.aucDnsPrimAddr;
                vos_printf("PDN %d IPv4��DNS��ַ:%d.%d.%d.%d \n", ucRabId, pucIpv4[0], pucIpv4[1], pucIpv4[2], pucIpv4[3]);

                vos_printf("PDN %d IPv4��DNS��Ϣ, bitOpDnsPrim:%d, bitOpDnsPrimCfged:%d \n", ucRabId, pstSinglePdnInfo->stIpv4PdnInfo.bitOpDnsSec, pstSinglePdnInfo->bitOpIpv4DnsSecCfged);

                pucIpv4 = pstSinglePdnInfo->stIpv4PdnInfo.aucDnsSecAddr;
                vos_printf("PDN %d IPv4��DNS��ַ:%d.%d.%d.%d \n", ucRabId, pucIpv4[0], pucIpv4[1], pucIpv4[2], pucIpv4[3]);
            }

            vos_printf("PDN %d �Ƿ�����IPv6:%d \n", ucRabId, pstSinglePdnInfo->bitOpIpv6PdnInfo);

            if (VOS_TRUE == pstSinglePdnInfo->bitOpIpv6PdnInfo)
            {
                pusIpv6 = (VOS_UINT16 *)pstSinglePdnInfo->stIpv6PdnInfo.aucIpV6Addr;

                vos_printf("PDN %d IPv6��ַ:%x:%x:%x:%x:%x:%x:%x:%x \n", ucRabId, VOS_HTONS(pusIpv6[0]), VOS_HTONS(pusIpv6[1]), VOS_HTONS(pusIpv6[2]), VOS_HTONS(pusIpv6[3]), VOS_HTONS(pusIpv6[4]), VOS_HTONS(pusIpv6[5]), VOS_HTONS(pusIpv6[6]), VOS_HTONS(pusIpv6[7]));
                vos_printf("PDN %d IPv6��DNS��Ϣ, bitOpDnsPrim:%d, bitOpDnsPrimCfged%d \n", ucRabId, pstSinglePdnInfo->stIpv6PdnInfo.bitOpDnsPrim, pstSinglePdnInfo->bitOpIpv6DnsPrimCfged);

                pusIpv6 = (VOS_UINT16 *)pstSinglePdnInfo->stIpv6PdnInfo.aucDnsPrimAddr;
                vos_printf("PDN %d IPv6��DNS��ַ:%x:%x:%x:%x:%x:%x:%x:%x \n", ucRabId, VOS_HTONS(pusIpv6[0]), VOS_HTONS(pusIpv6[1]), VOS_HTONS(pusIpv6[2]), VOS_HTONS(pusIpv6[3]), VOS_HTONS(pusIpv6[4]), VOS_HTONS(pusIpv6[5]), VOS_HTONS(pusIpv6[6]), VOS_HTONS(pusIpv6[7]));

                vos_printf("PDN %d IPv6��DNS��Ϣ, bitOpDnsPrim:%d, bitOpDnsPrimCfged:%d \n", ucRabId, pstSinglePdnInfo->stIpv6PdnInfo.bitOpDnsSec, pstSinglePdnInfo->bitOpIpv6DnsSecCfged);

                pusIpv6 = (VOS_UINT16 *)pstSinglePdnInfo->stIpv6PdnInfo.aucDnsSecAddr;
                vos_printf("PDN %d IPv6��DNS��ַ:%x:%x:%x:%x:%x:%x:%x:%x \n", ucRabId, VOS_HTONS(pusIpv6[0]), VOS_HTONS(pusIpv6[1]), VOS_HTONS(pusIpv6[2]), VOS_HTONS(pusIpv6[3]), VOS_HTONS(pusIpv6[4]), VOS_HTONS(pusIpv6[5]), VOS_HTONS(pusIpv6[6]), VOS_HTONS(pusIpv6[7]));
            }
            vos_printf("\n");
        }
    }
}


VOS_VOID IMS_NIC_ShowStats(VOS_VOID)
{
    VOS_INT32                i;


    for (i = 0; i < IMS_NIC_MODEM_ID_BUTT; i++)
    {
        vos_printf("======================Debug info for Modem %d ======================\n", i);

        IMS_INC_STATS_SHOW(IMS_NIC_STATS_DL_RCV_IPV4_PKT, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_DL_RCV_IPV6_PKT, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_DL_SEND_PKT, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_DL_RCV_BYTES, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_DL_RCV_ERR_PKT, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_DL_INPUT_NULL_PTR, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_DL_RCV_DATA_LEN_ERR, i);

        IMS_INC_STATS_SHOW(IMS_NIC_STATS_UL_RCV_IPV4_PKT, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_UL_RCV_IPV6_PKT, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_UL_SEND_PKT, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_UL_SEND_BYTES, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_UL_RCV_ERR_PKT, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_UL_RCV_ARP_PKT, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_UL_INPUT_NULL_PTR, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_UL_RCV_DATA_LEN_ERR, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_UL_CB_FUN_NOT_REG, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_UL_CALL_CB_FUN_FAIL, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_UL_PKT_MEM_SEG, i);

        IMS_INC_STATS_SHOW(IMS_NIC_STATS_CFG_DNS_SERVER_FULL, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_CFG_INPUT_NULL_PTR, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_CFG_ADD_MODEM_ID_ERR, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_CFG_DEL_MODEM_ID_ERR, i);

        IMS_INC_STATS_SHOW(IMS_NIC_STATS_CFG_ADD_RAB_ID_ERR, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_CFG_DEL_RAB_ID_ERR, i);

        IMS_INC_STATS_SHOW(IMS_NIC_STATS_GET_CLUSTER_FAIL, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_GET_CLBLK_FAIL, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_GET_MBLK_FAIL, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_MBLK_COPY_ERR, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_NOT_INITED_ERR, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_SYS_CALL_FAIL, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_FIND_DEV_FAIL, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_CFG_ADD_NO_PDN, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_CFG_DEL_NO_PDN, i);

        IMS_INC_STATS_SHOW(IMS_NIC_STATS_MODEM_ID_ERR_OTHER, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_MODEM_ID_ERR_IN_REG_CB, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_MODE_ID_ERR_IN_DL_PROC, i);
        IMS_INC_STATS_SHOW(IMS_NIC_STATS_MODE_ID_ERR_IN_UL_PROC, i);

        IMS_INC_STATS_SHOW(IMS_NIC_STATS_INVALID_IPV6_PREFIX_LEN, i);

        vos_printf("\n\n");
    }
}

VOS_VOID IMS_NIC_MuxShow(VOS_VOID)
{
    muxShow(IMS_NIC_GET_NIC_NAME, 0);

}

#ifdef IMS_NIC_DEBUG
VOS_VOID IMS_NIC_TestSdt(VOS_VOID)
{
    IMS_NIC_PDN_INFO_CONFIG_STRU    stConfigInfo;
    VOS_UINT8                       aucDstIpAddr1[4]            = {0x2, 0x1, 0x15, 0x27};
    VOS_UINT32                      ulResult;
    VOS_UINT8   ucRabId1    = 5;
    char *cmd_str1           = "ifconfig -a";
    char *cmd_str2           = "route -n show";

    VOS_MemSet(&stConfigInfo, 0, sizeof(IMS_NIC_PDN_INFO_CONFIG_STRU));

    stConfigInfo.bitOpIpv4PdnInfo   = PS_TRUE;
    stConfigInfo.bitOpIpv6PdnInfo   = PS_FALSE;
    stConfigInfo.enModemId          = 0;
    stConfigInfo.ucRabId            = ucRabId1;
    VOS_MemCpy(stConfigInfo.stIpv4PdnInfo.aucIpV4Addr, aucDstIpAddr1, 4);

    stConfigInfo.stIpv4PdnInfo.bitOpDnsPrim         = PS_FALSE;
    stConfigInfo.stIpv4PdnInfo.bitOpDnsSec          = PS_FALSE;

    ulResult    = IMS_NIC_PdnInfoConfig (&stConfigInfo);
    if (IMS_NIC_SUCC != ulResult)
    {
        vos_printf("ST_DL_RCV_001: config pdn fail! ulResult = %d\n", ulResult);
        return;
    }

    ipcom_run_cmd(cmd_str1);
    ipcom_run_cmd(cmd_str2);

    /* ɾ�������������� */
    IMS_NIC_PdnInfoDel(stConfigInfo.ucRabId, 0);

    ipcom_run_cmd(cmd_str1);
    ipcom_run_cmd(cmd_str2);

    return;
}
#endif

#endif  /*FEATURE_ON == FEATURE_IMS*/


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif



