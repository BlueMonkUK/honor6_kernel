

#ifndef __TTFMEMRB_H__
#define __TTFMEMRB_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "product_config.h"
#include "TTFLink.h"
#include "vos.h"
#include "VosPidDef.h"
#include "TTFUtil.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  1 ��Ϣͷ����
*****************************************************************************/

/*****************************************************************************
  2 �궨��
*****************************************************************************/

#define TTF_MEM_RB_TASK_FAIL_CNT_MAX    (10000)

/* IMM ��װ�Ĵ�ӡ */
#ifdef __UT_CENTER__

#if (OSA_CPU_ACPU == VOS_OSA_CPU)
#define TTF_LOG(ModulePID, Level, String)\
                OM_LOG((ModulePID), 0, (Level), (String))
#define TTF_LOG1(ModulePID, Level, String, Para1)\
                OM_LOG1((ModulePID), 0, (Level), (String), (Para1))
#define TTF_LOG2(ModulePID, Level, String, Para1, Para2)\
                OM_LOG2((ModulePID), 0, (Level), (String), (Para1), (Para2))
#define TTF_LOG3(ModulePID, Level, String, Para1, Para2, Para3)\
                OM_LOG3((ModulePID), 0, (Level), (String), (Para1), (Para2), (Para3))
#define TTF_LOG4(ModulePID, Level, String, Para1, Para2, Para3, Para4)\
                OM_LOG4((ModulePID), 0, (Level), (String), (Para1), (Para2), (Para3), (Para4))
#elif (OSA_CPU_CCPU == VOS_OSA_CPU)
#define TTF_LOG(ModulePID, Level, String)\
                PS_LOG((ModulePID), 0, (Level), (String))
#define TTF_LOG1(ModulePID, Level, String, Para1)\
                PS_LOG1((ModulePID), 0, (Level), (String), (Para1))
#define TTF_LOG2(ModulePID, Level, String, Para1, Para2)\
                PS_LOG2((ModulePID), 0, (Level), (String), (Para1), (Para2))
#define TTF_LOG3(ModulePID, Level, String, Para1, Para2, Para3)\
                PS_LOG3((ModulePID), 0, (Level), (String), (Para1), (Para2), (Para3))
#define TTF_LOG4(ModulePID, Level, String, Para1, Para2, Para3, Para4)\
                PS_LOG4((ModulePID), 0, (Level), (String), (Para1), (Para2), (Para3), (Para4))
#endif

#else

#define TTF_LOG(ModulePID, Level, String)\
                PS_LOG((ModulePID), 0, (Level), (String))
#define TTF_LOG1(ModulePID, Level, String, Para1)\
                PS_LOG1((ModulePID), 0, (Level), (String), (Para1))
#define TTF_LOG2(ModulePID, Level, String, Para1, Para2)\
                PS_LOG2((ModulePID), 0, (Level), (String), (Para1), (Para2))
#define TTF_LOG3(ModulePID, Level, String, Para1, Para2, Para3)\
                PS_LOG3((ModulePID), 0, (Level), (String), (Para1), (Para2), (Para3))
#define TTF_LOG4(ModulePID, Level, String, Para1, Para2, Para3, Para4)\
                PS_LOG4((ModulePID), 0, (Level), (String), (Para1), (Para2), (Para3), (Para4))

#endif

#if (FEATURE_ON == FEATURE_SKB_EXP)

/* ��C99 ��׼�� __func__ תΪVOS����ĺ� __FUNCTION__ */
#define __func__ __FUNCTION__

extern VOS_UINT8                               g_ucTtfMemDebugPrint;

/* ���Դ�ӡ */
/*
#define TTF_DEBUG_PRINT(...)\
             do{\
                 if ( PS_TRUE == g_ucTtfMemDebugPrint )\
                 {\
                     vos_printf(__VA_ARGS__);
                 }\
             }while(0)
*/

#define TTF_DEBUG_TRACE_FUNC            FEATURE_OFF

#ifdef _lint

#define TTF_DEBUG_TRACE_FUNC_ENTER()
#define TTF_DEBUG_TRACE_FUNC_LEAVE()

#else


#if(TTF_DEBUG_TRACE_FUNC == FEATURE_ON)
#define TTF_DEBUG_TRACE_FUNC_ENTER()\
            do{\
                if ( PS_TRUE == g_ucTtfMemDebugPrint )\
                {\
                    vos_printf("Enter Function: ");\
                    vos_printf(__func__);\
                    vos_printf("\n");\
                }\
            }while(0)

#define TTF_DEBUG_TRACE_FUNC_LEAVE()\
            do{\
                if ( PS_TRUE == g_ucTtfMemDebugPrint )\
                {\
                    vos_printf("Normal Leave Function: ");\
                    vos_printf(__func__);\
                    vos_printf("\n");\
                }\
            }while(0)
#else
#define TTF_DEBUG_TRACE_FUNC_ENTER()
#define TTF_DEBUG_TRACE_FUNC_LEAVE()
#endif
#endif

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/

/*****************************************************************************
  7 STRUCT����
*****************************************************************************/
#pragma pack(4)


/*****************************************************************************
 �ṹ��    : TTF_MEM_RB_DATA_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �˼����ݴ�����Ϣ�ṹ
*****************************************************************************/
typedef struct
{
    VOS_UINT8                            ucUserId;           /* �û�ID */
    VOS_UINT8                            ucReserve;
    VOS_UINT16                           usDataLen;          /* ���ݳ��� */
    VOS_UINT32                           ulReserveLen;       /* C��֪ͨA�� head�� data֮��Ԥ��PAD�ĳ��� */
    VOS_UINT8                           *pDataAddr;          /* �����׵�ַ */
    VOS_VOID                            *pRetAddr;           /* ���������ͷŵĵ�ַ */
}TTF_MEM_RB_DATA_STRU;

/*****************************************************************************
 �ṹ��    : TTF_MEM_RB_FREE_MEM_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �˼��ͷŴ�����Ϣ�ṹ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          *pRetAddr;         /* sk_buff �׵�ַ������֪ͨ�ͷ��ڴ�ĵ�ַ */
}TTF_MEM_RB_FREE_MEM_STRU;

/*****************************************************************************
 �ṹ��    : TTF_MEM_RB_MNTN_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �˼��ά�ɲ���Ϣ
*****************************************************************************/
typedef struct TTF_MEM_RB_MNTN_INFO
{
    VOS_UINT32          ulTtfMemRbSendIntCnt;       /* TtfMemRb ��IPF�����жϴ��� */
    VOS_UINT32          ulTtfMemRbReceiveIntCnt;    /* TtfMemRb �յ�IPF�жϴ��� */
    VOS_UINT32          ulTtfMemRbEnQFailCnt;       /* TtfMemRb ���ʧ�ܴ��� */
    VOS_UINT32          ulTtfMemRbDeQFailCnt;       /* TtfMemRb ����ʧ�ܴ��� */
    VOS_UINT32          ulTtfMemRbEnQTotalCnt;      /* TtfMemRb ������� */
    VOS_UINT32          ulTtfMemRbDeQTotalCnt;      /* TtfMemRb �������� */
}TTF_MEM_RB_MNTN_INFO_STRU;

/*****************************************************************************
  8 UNION����
*****************************************************************************/

/*****************************************************************************
  9 OTHERS����
*****************************************************************************/

/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_VOID TTF_DebugLOG(VOS_CHAR *pcString);
extern VOS_VOID TTF_DebugLOG1(VOS_CHAR *pcString, VOS_INT32 lPara1);
extern VOS_VOID TTF_DebugLOG2(VOS_CHAR *pcString, VOS_INT32 lPara1, VOS_INT32 lPara2);
extern VOS_VOID TTF_DebugLOG3(VOS_CHAR *pcString, VOS_INT32 lPara1, VOS_INT32 lPara2, VOS_INT32 lPara3);
extern VOS_VOID TTF_DebugLOG4(VOS_CHAR *pcString, VOS_INT32 lPara1, VOS_INT32 lPara2, VOS_INT32 lPara3, VOS_INT32 lPara4);
extern VOS_VOID TTF_MemFreeFromQue(VOS_VOID);

#endif
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

#endif /* end of Adapter.h */







