
#ifndef __UCOM_MEM_DYN_H__
#define __UCOM_MEM_DYN_H__

/*****************************************************************************
  0 ����ͷ�ļ�����
*****************************************************************************/
#include  "vos.h"

#ifdef _UCOM_USE_STD_MEM_LIB
#include <string.h>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  1 �ⲿ�ӿڶ���
*****************************************************************************/

/*****************************************************************************
 ��������  : UCOM_MemAlloc
 ��������  : ����λ��DDR�еĶ�̬�ڴ�
 ����˵��  : uwSize - �����ڴ�Ĵ�С, ��λByte
 ���ؽ��  : ����ɹ� - �������뵽�ĵ�ַ
             ����ʧ�� - ����VOS_NULL(0)
*****************************************************************************/
#ifdef _UCOM_USE_STD_MEM_LIB
#define UCOM_MemAlloc(uwSize)           malloc(uwSize)
#else
#define UCOM_MemAlloc(uwSize)           UCOM_MEM_DYN_Malloc((VOS_UINT32)(uwSize),\
                                                            UCOM_MEM_DYN_REQUEST_DDR)
#endif

/*****************************************************************************
 ��������  : UCOM_MemAllocLimited
 ��������  : ����λ��TCM�еĶ�̬�ڴ�(���ڴ�����ٶȱ�DDR����,���ɹ�ʹ�õĿռ�����)
 ����˵��  : uwSize - �����ڴ�Ĵ�С, ��λByte
 ���ؽ��  : ����ɹ� - �������뵽�ĵ�ַ
             ����ʧ�� - ����VOS_NULL(0)
*****************************************************************************/
#ifdef _UCOM_USE_STD_MEM_LIB
#define UCOM_MemAllocLimited(uwSize)    malloc(uwSize)
#else
#define UCOM_MemAllocLimited(uwSize)    UCOM_MEM_DYN_Malloc((VOS_UINT32)(uwSize),\
                                                            UCOM_MEM_DYN_REQUEST_TCM)
#endif

/*****************************************************************************
 ��������  : UCOM_MemAllocFair
 ��������  : ���ȳ�������TCM�еĶ�̬�ڴ�,��ʧ��,������DDR�еĶ�̬�ڴ�
 ����˵��  : uwSize - �����ڴ�Ĵ�С, ��λByte
 ���ؽ��  : ����ɹ� - �������뵽�ĵ�ַ
             ����ʧ�� - ����VOS_NULL(0)
*****************************************************************************/
#ifdef _UCOM_USE_STD_MEM_LIB
#define UCOM_MemAllocFair(uwSize)       malloc(uwSize)
#else
#define UCOM_MemAllocFair(uwSize)       UCOM_MEM_DYN_Malloc((VOS_UINT32)(uwSize),\
                                                            UCOM_MEM_DYN_REQUEST_TCM\
                                                            |UCOM_MEM_DYN_REQUEST_DDR)
#endif

/*****************************************************************************
 ��������  : UCOM_MemFree
 ��������  : �ͷ�������ڴ�
 ����˵��  : pMem - ��Ҫ�ͷŵ��ڴ��ַ
 ���ؽ��  : �ͷųɹ� - ����VOS_OK
             �ͷ�ʧ�� - ����VOS_ERR
*****************************************************************************/
#ifdef _UCOM_USE_STD_MEM_LIB
#define UCOM_MemFree(pMem)      free((void*)(pMem))
#else
#define UCOM_MemFree(pMem)      UCOM_MEM_DYN_Free((VOS_VOID*)(pMem))
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/

#define UCOM_MEM_DYN_POOL_SIZE_DDR      (512000L)           /* DDR�ڴ��С, �����MALLOC_ALIGNMENT�������� */
#define UCOM_MEM_DYN_POOL_SIZE_TCM      (0x4000)            /* TCM�ڴ��С, �����MALLOC_ALIGNMENT������������ʱ����Ϊ16KByte�������ٽ��е��� */
#define UCOM_MEM_DYN_MALLOC_ALIGNMENT   (sizeof(VOS_UINT32) * 2)/* �����ڴ�ĵ�ַ���뷽ʽ */

#define UCOM_MEM_DYN_NODE_MAX_NUM       (256)               /* ��������ڴ�ڵ��� */

/*****************************************************************************
 ��������  : UCOM_MEM_DYN_GetMemCtrl
 ��������  : ��ȡ�ڴ���ƽṹ��ָ��
*****************************************************************************/
#define UCOM_MEM_DYN_GetMemCtrl()       (&g_stUcomMemDynCtrl)

/*****************************************************************************
 ��������  : UCOM_MEM_DYN_GetPoolCfgTbl
 ��������  : ��ȡ�ڴ�����ñ�
*****************************************************************************/
#define UCOM_MEM_DYN_GetPoolCfgTbl()    (g_auwUcomMemDynPoolCfgTbl)

/*****************************************************************************
 ��������  : UCOM_MEM_DYN_GetIdleList
 ��������  : ��ȡδʹ������
*****************************************************************************/
#define UCOM_MEM_DYN_GetIdleList(pstCtrl) (&pstCtrl->stIdleList)

/*****************************************************************************
 ��������  : UCOM_MEM_DYN_GetFreeList
 ��������  : ��ȡ����������
*****************************************************************************/
#define UCOM_MEM_DYN_GetFreeList(pstCtrl,enType) (&pstCtrl->astFreeList[enType])

/*****************************************************************************
 ��������  : UCOM_MEM_DYN_GetUnFreeList
 ��������  : ��ȡ�ѷ�������
*****************************************************************************/
#define UCOM_MEM_DYN_GetUnFreeList(pstCtrl,enType) (&pstCtrl->astUnFreeList[enType])

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
 ʵ������  : UCOM_MEM_DYN_TYPE_ENUM
 ��������  : �����ڴ�洢����
*****************************************************************************/
enum UCOM_MEM_DYN_TYPE_ENUM
{
    UCOM_MEM_DYN_TYPE_DDR               = 0,                /* �ڴ��������λ��DDR */
    UCOM_MEM_DYN_TYPE_TCM,                                  /* �ڴ��������λ��TCM */
    UCOM_MEM_DYN_TYPE_BUTT,
};
typedef VOS_UINT32 UCOM_MEM_DYN_TYPE_ENUM_UINT32;

/*****************************************************************************
 ʵ������  : UCOM_MEM_DYN_REQUEST_ENUM
 ��������  : �����ڴ������������
*****************************************************************************/
enum UCOM_MEM_DYN_REQUEST_ENUM
{
    UCOM_MEM_DYN_REQUEST_DDR            = 0x1,              /* �����ڴ�����DDR�ڴ� */
    UCOM_MEM_DYN_REQUEST_TCM            = 0x2,              /* �����ڴ�����TCM�ڴ� */
    UCOM_MEM_DYN_REQUEST_BUTT           = 0x4,
};
typedef VOS_UINT32 UCOM_MEM_DYN_REQUEST_ENUM_UINT32;


/*****************************************************************************
 ʵ������  : UCOM_MEM_DYN_POOL_CFG_ITEM_ENUM
 ��������  : �����ڴ����ñ���
*****************************************************************************/
enum UCOM_MEM_DYN_POOL_CFG_ITEM_ENUM
{
    UCOM_MEM_DYN_POOL_CFG_ITEM_ADDR     = 0,                /* �ڴ���ʼ��ַ */
    UCOM_MEM_DYN_POOL_CFG_ITEM_SIZE,                        /* �ڴ��С */
    UCOM_MEM_DYN_POOL_CFG_ITEM_BUTT,
};
typedef VOS_UINT32 UCOM_MEM_DYN_POOL_CFG_ITEM_ENUM_UINT32;


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
 ʵ������  : UCOM_MEM_DYN_BLK_STRU
 ��������  : ʹ���ߵ��ڴ����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          uwAddr;             /* �ڴ���ַ */
    VOS_UINT32                          uwSize;             /* �ڴ���С */
} UCOM_MEM_DYN_BLK_STRU;

/*****************************************************************************
 ʵ������  : UCOM_MEM_DYN_NODE_STRU
 ��������  : �ڴ�ڵ���Ϣ
*****************************************************************************/
typedef struct _UCOM_MEM_DYN_NODE_STRU
{
    UCOM_MEM_DYN_BLK_STRU               stBlk;              /* ��ǰ�ڵ������ */
    struct _UCOM_MEM_DYN_NODE_STRU     *pstNext;            /* ��һ���ڵ� */
    struct _UCOM_MEM_DYN_NODE_STRU     *pstPrev;            /* ǰһ���ڵ� */
} UCOM_MEM_DYN_NODE_STRU;

/*****************************************************************************
 ʵ������  : UCOM_MEM_DYN_LIST_STRU
 ��������  : �ڴ�������Ϣ
*****************************************************************************/
typedef struct
{
    UCOM_MEM_DYN_NODE_STRU              stHead;             /* ����ͷ */
    UCOM_MEM_DYN_NODE_STRU              stTail;             /* ����β */
    VOS_UINT32                          uwLen;              /* ������ */
} UCOM_MEM_DYN_LIST_STRU;

/*****************************************************************************
 ʵ������  : UCOM_MEM_DYN_CTRL_STRU
 ��������  : �ڴ�����ʵ��
*****************************************************************************/
typedef struct
{
    UCOM_MEM_DYN_LIST_STRU              astFreeList[UCOM_MEM_DYN_TYPE_BUTT];    /* �������ڴ�ڵ� */
    UCOM_MEM_DYN_LIST_STRU              astUnFreeList[UCOM_MEM_DYN_TYPE_BUTT];  /* �ѷ����ڴ�ڵ� */
    UCOM_MEM_DYN_LIST_STRU              stIdleList;                             /* δʹ�õĽڵ����� */
    UCOM_MEM_DYN_NODE_STRU              astNodes[UCOM_MEM_DYN_NODE_MAX_NUM];    /* �������нڵ���Ϣ(�ڵ�δʹ�ö�̬����) */
} UCOM_MEM_DYN_CTRL_STRU;


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

extern VOS_VOID UCOM_MEM_DYN_AddNode(
                       UCOM_MEM_DYN_NODE_STRU      *pstNode,
                       UCOM_MEM_DYN_LIST_STRU      *pstList );
extern VOS_UINT32  UCOM_MEM_DYN_AllocByType(
                       VOS_UINT32                          uwSize,
                       UCOM_MEM_DYN_TYPE_ENUM_UINT32       enType);
extern VOS_VOID UCOM_MEM_DYN_DelNode(
                       UCOM_MEM_DYN_NODE_STRU      *pstNode,
                       UCOM_MEM_DYN_LIST_STRU      *pstList );
extern VOS_UINT32 UCOM_MEM_DYN_Free(VOS_VOID *pMem);
extern VOS_UINT32  UCOM_MEM_DYN_FreeByType(
                       VOS_UINT32                          uwAddr,
                       UCOM_MEM_DYN_TYPE_ENUM_UINT32       enType);
extern UCOM_MEM_DYN_NODE_STRU* UCOM_MEM_DYN_GetIdleNode( VOS_VOID );
extern VOS_VOID UCOM_MEM_DYN_Init( VOS_VOID );
extern  VOS_VOID UCOM_MEM_DYN_InitList(UCOM_MEM_DYN_LIST_STRU *pstList);
extern VOS_VOID UCOM_MEM_DYN_InsertNode(
                       UCOM_MEM_DYN_NODE_STRU      *pstNode,
                       UCOM_MEM_DYN_LIST_STRU      *pstList );
extern VOS_VOID* UCOM_MEM_DYN_Malloc(
                       VOS_UINT32                          uwSize,
                       UCOM_MEM_DYN_REQUEST_ENUM_UINT32    enReq);
extern UCOM_MEM_DYN_NODE_STRU*  UCOM_MEM_DYN_SeekNodeByAddr(
                       VOS_UINT32                          uwAddr,
                       UCOM_MEM_DYN_LIST_STRU             *pstList );
extern UCOM_MEM_DYN_NODE_STRU*  UCOM_MEM_DYN_SeekNodeBySize(
                       VOS_UINT32                          uwSize,
                       UCOM_MEM_DYN_LIST_STRU             *pstList,
                       VOS_UINT32                         *puwBestFit);
extern VOS_VOID UCOM_MEM_DYN_SetIdleNode(UCOM_MEM_DYN_NODE_STRU *pstNode);











#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of ucom_mem_dyn.h */
