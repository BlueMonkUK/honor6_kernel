
#ifndef __MLIB_UTILITY_H__
#define __MLIB_UTILITY_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "mlib_typedef.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  ��̬�ڴ������ͷ�
*****************************************************************************/

/*
��̬�ڴ�����, �����Լ���������ͬ��׼��malloc
*/
extern MLIB_VOID*  MUTIL_Malloc( MLIB_UINT32  uwSize );

/*
��̬�ڴ��ͷ�, �����Լ���������ͬ��׼��free
*/
extern MLIB_VOID  MUTIL_Free( MLIB_VOID *pMem );


/*
�ڴ濽��, �����Լ���������ͬ��׼��memcpy
*/
extern MLIB_VOID MUTIL_MemCpy(
                       MLIB_VOID              *pvDes,
                       MLIB_VOID              *pvSrc,
                       MLIB_UINT32             uwCount);

/*
�ڴ�����, �����Լ���������ͬ��׼��memset
*/
extern MLIB_VOID MUTIL_MemSet(
                       MLIB_VOID               *pvDes,
                       MLIB_UINT8               ucData,
                       MLIB_UINT32              uwCount);

/*
NV���ȡ:
uhwID       - NV ID
pItem       - NV �������׵�ַ, ���ݿռ���ҪԤ�ȷ���
uwLength    - ��Ҫ��ȡ�� NV �����ݴ�С
*/
extern MLIB_UINT32 MUTIL_ReadNV(
                       MLIB_UINT16             uhwId,
                       MLIB_VOID               *pItem,
                       MLIB_UINT32              uwLength);

/*
���ݹ�ȡ:
uhwPos      - ������������ЧֵΪ����������ֻ����1λΪ1����λΪ��������1,2,4,8,16...
pvData      - ��Ҫ��ȡ�������׵�ַ
uwLength    - ��Ҫ��ȡ�����ݴ�С
*/
extern MLIB_VOID MUTIL_DataHook(
                    MLIB_UINT16              uhwPos,
                    MLIB_VOID               *pvData,
                    MLIB_UINT16              uhwLen);

/* ��־�ϱ����� */
enum MLIB_LOG_LEVEL_ENUM
{
    MLIB_LOG_LEVEL_INFO                 = 0,
    MLIB_LOG_LEVEL_WARNING,
    MLIB_LOG_LEVEL_ERROR,
    MLIB_LOG_LEVEL_BUTT
};
typedef MLIB_UINT16 MLIB_LOG_LEVEL_ENUM_UINT32;

/* apr�ϱ�logId,��Χ0~9 */
enum
{
    /* ext_pp */
    APR_LOG_EXT_PP_TMP     = 0,

};
typedef MLIB_UINT32 MLIB_APR_LOG_ID_ENUM_UINT32;

/*
��־��ӡ:
enLevel - ��ӡ��־����(ȡֵ��ΧMLIB_LOG_LEVEL_ENUM)
uhwFileID - �Զ����ļ���(Ϊ�����ͻ, �����0x0a00��ʼ���)
uhwLineID - ��ӡ���ڵ��к�
swValue1/2/3 - ��Ҫ��ӡ����ֵ

��Ҫֱ�ӵ������������ ���������װ�ĺ�
*/
extern MLIB_VOID MUTIL_LogReport(
                MLIB_LOG_LEVEL_ENUM_UINT32          enLevel,
                MLIB_UINT16                          uhwFileID,
                MLIB_UINT16                          uhwLineID,
                MLIB_INT32                           swValue1,
                MLIB_INT32                           swValue2,
                MLIB_INT32                           swValue3);

/* apr��־��ӡ:
*/
extern MLIB_VOID  MLIB_LogApr(MLIB_APR_LOG_ID_ENUM_UINT32 enLogId);

/* ��ӡ��ʾ��Ϣ */
#define MUTIL_LogInfo(enFileID,swValue1,swValue2,swValue3)          \
    MUTIL_LogReport(MLIB_LOG_LEVEL_INFO,enFileID,__LINE__,(swValue1),(swValue2),(swValue3))

/* ��ӡ������Ϣ */
#define MUTIL_LogWarning(enFileID,swValue1,swValue2,swValue3)       \
    MUTIL_LogReport(MLIB_LOG_LEVEL_WARNING,enFileID,__LINE__,(swValue1),(swValue2),(swValue3))

/* ��ӡ������Ϣ */
#define MUTIL_LogError(enFileID,swValue1,swValue2,swValue3)         \
    MUTIL_LogReport(MLIB_LOG_LEVEL_ERROR,enFileID,__LINE__,(swValue1),(swValue2),(swValue3))

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of mlib_utility.h */
