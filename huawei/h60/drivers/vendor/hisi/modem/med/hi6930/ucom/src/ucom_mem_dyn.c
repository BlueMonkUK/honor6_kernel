

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "ucom_mem_dyn.h"
#include "ucom_comm.h"
#include "om_log.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    ��ά�ɲ���Ϣ�а�����C�ļ���ź궨��
*****************************************************************************/
/*lint -e(767)*/
#define THIS_FILE_ID

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/* �ڴ���ƶ��� */
UCOM_MEM_DYN_CTRL_STRU                  g_stUcomMemDynCtrl;

/* DDR�ڴ�� */
VOS_UINT32                              g_auwUcomMemDynPoolDDR[UCOM_MEM_DYN_POOL_SIZE_DDR/sizeof(VOS_UINT32)];

/* TCM�ڴ�� */
UCOM_SEC_TCMBSS
VOS_UINT32                              g_auwUcomMemDynPoolTCM[UCOM_MEM_DYN_POOL_SIZE_TCM/sizeof(VOS_UINT32)];

/* �ڴ����ñ� */
VOS_UINT32                              g_auwUcomMemDynPoolCfgTbl[UCOM_MEM_DYN_TYPE_BUTT][UCOM_MEM_DYN_POOL_CFG_ITEM_BUTT] = \
{
    /* �ڴ��׵�ַ           |       �ڴ��С */
    {(VOS_UINT32)g_auwUcomMemDynPoolDDR,    UCOM_MEM_DYN_POOL_SIZE_DDR},    /* DDR�ڴ������ */
    {(VOS_UINT32)g_auwUcomMemDynPoolTCM,    UCOM_MEM_DYN_POOL_SIZE_TCM},    /* TCM�ڴ������ */
};

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_VOID UCOM_MEM_DYN_Init( VOS_VOID )
{
    UCOM_MEM_DYN_CTRL_STRU             *pstMemCtrlBlk;
    UCOM_MEM_DYN_LIST_STRU             *pstIdleList;
    UCOM_MEM_DYN_LIST_STRU             *pstFreeList;
    UCOM_MEM_DYN_LIST_STRU             *pstUnFreeList;
    UCOM_MEM_DYN_NODE_STRU             *pstNode;
    VOS_UINT32                         (*puwPoolCfgTbl)[UCOM_MEM_DYN_POOL_CFG_ITEM_BUTT];
    VOS_UINT32                          uwCnt;
    UCOM_MEM_DYN_TYPE_ENUM_UINT32       enType;
    VOS_UINT32                          uwMask;

    /* �������±�̹淶,��������ʱ���ɳ�ʼ�� */
    pstMemCtrlBlk = UCOM_MEM_DYN_GetMemCtrl();
    pstIdleList   = UCOM_MEM_DYN_GetIdleList(pstMemCtrlBlk);
    uwCnt         = 0;

    /* ��ʼ���ڴ���ƶ��� */
    UCOM_MemSet(pstMemCtrlBlk,
                0,
                sizeof(UCOM_MEM_DYN_CTRL_STRU));

    /* ��ʼ��δʹ�õ��ڴ����� */
    UCOM_MEM_DYN_InitList(pstIdleList);

    /* ���ڵ����μ���IdleList */
    for (uwCnt = 0; uwCnt < UCOM_MEM_DYN_NODE_MAX_NUM; uwCnt++)
    {
        UCOM_MEM_DYN_SetIdleNode(&(pstMemCtrlBlk->astNodes[uwCnt]));
    }

    puwPoolCfgTbl = UCOM_MEM_DYN_GetPoolCfgTbl();

    /* �����ֽڶ��� */
    uwMask = ~(UCOM_MEM_DYN_MALLOC_ALIGNMENT - 1);

    /* ��ʼ�����������ѷ����ڴ�������� */
    for (enType= 0; enType < UCOM_MEM_DYN_TYPE_BUTT; enType++)
    {
        pstFreeList   = UCOM_MEM_DYN_GetFreeList(pstMemCtrlBlk, enType);
        pstUnFreeList = UCOM_MEM_DYN_GetUnFreeList(pstMemCtrlBlk, enType);

        /* ��ʼ��������ڵ����� */
         UCOM_MEM_DYN_InitList(pstFreeList);

        /* ��ʼ���ѷ���ڵ����� */
        UCOM_MEM_DYN_InitList(pstUnFreeList);

        /* ��ȡһ��δʹ�ýڵ� */
        pstNode = UCOM_MEM_DYN_GetIdleNode();

        if (VOS_NULL != pstNode)
        {
            /* ��DDR�ڴ����Ϊ������ڵ���������ڵ����� */
            pstNode->stBlk.uwAddr   = (puwPoolCfgTbl[enType][UCOM_MEM_DYN_POOL_CFG_ITEM_ADDR] + (UCOM_MEM_DYN_MALLOC_ALIGNMENT - 1)) & uwMask;
            pstNode->stBlk.uwSize   = puwPoolCfgTbl[enType][UCOM_MEM_DYN_POOL_CFG_ITEM_SIZE] - UCOM_MEM_DYN_MALLOC_ALIGNMENT;

            UCOM_MEM_DYN_InsertNode(pstNode, pstFreeList);
        }
    }

    return;
}


VOS_VOID* UCOM_MEM_DYN_Malloc(
                VOS_UINT32                          uwSize,
                UCOM_MEM_DYN_REQUEST_ENUM_UINT32    enReq)
{
    VOS_UINT32                          uwAddr;

    uwAddr          = VOS_NULL;

    /* ʵ�ʷ���ʱ, ���շ����ڴ��������������� */
    uwSize  = ((uwSize + UCOM_MEM_DYN_MALLOC_ALIGNMENT) - 1) & (~(UCOM_MEM_DYN_MALLOC_ALIGNMENT - 1));

    /*������*/
    VOS_TaskLock();

    /* ����TCM�ڴ� */
    if(enReq & UCOM_MEM_DYN_REQUEST_TCM)
    {
        uwAddr = UCOM_MEM_DYN_AllocByType(uwSize, UCOM_MEM_DYN_TYPE_TCM);
    }

    /* ����DDR�ڴ� */
    if( (VOS_NULL == uwAddr) && (enReq & UCOM_MEM_DYN_REQUEST_DDR))
    {
        uwAddr = UCOM_MEM_DYN_AllocByType(uwSize, UCOM_MEM_DYN_TYPE_DDR);
    }

    /* �������� */
    VOS_TaskUnlock();

    if(VOS_NULL == uwAddr)
    {
        OM_LogFatalAndReset(UCOM_DYN_MEM_Overflow);
    }

    return (VOS_VOID*)uwAddr;

}
VOS_UINT32 UCOM_MEM_DYN_Free(VOS_VOID *pMem)
{
    VOS_UINT32                    (*puwPoolCfgTbl)[UCOM_MEM_DYN_POOL_CFG_ITEM_BUTT];
    UCOM_MEM_DYN_TYPE_ENUM_UINT32   enType;
    VOS_UINT32                      uwAddrStart;
    VOS_UINT32                      uwAddrEnd;
    VOS_UINT32                      uwAddr;
    VOS_UINT32                      uwRet;

    puwPoolCfgTbl   = UCOM_MEM_DYN_GetPoolCfgTbl();
    uwAddr          = (VOS_UINT32)pMem;
    uwRet           = VOS_ERR;

    /*������*/
    VOS_TaskLock();

    /* �����ڴ����� */
    for (enType= 0; enType < UCOM_MEM_DYN_TYPE_BUTT; enType++)
    {
        uwAddrStart = puwPoolCfgTbl[enType][UCOM_MEM_DYN_POOL_CFG_ITEM_ADDR];
        uwAddrEnd   = uwAddrStart + puwPoolCfgTbl[enType][UCOM_MEM_DYN_POOL_CFG_ITEM_SIZE];

        if((uwAddr >= uwAddrStart) && (uwAddr < uwAddrEnd))
        {
            uwRet = UCOM_MEM_DYN_FreeByType(uwAddr, enType);
            break;
        }
    }

    /* �������� */
    VOS_TaskUnlock();

    return uwRet;

}
VOS_UINT32  UCOM_MEM_DYN_AllocByType(
                VOS_UINT32                          uwSize,
                UCOM_MEM_DYN_TYPE_ENUM_UINT32       enType)
{
    UCOM_MEM_DYN_CTRL_STRU             *pstMemCtrlBlk;
    UCOM_MEM_DYN_LIST_STRU             *pstFreeList;
    UCOM_MEM_DYN_LIST_STRU             *pstUnFreeList;
    UCOM_MEM_DYN_NODE_STRU             *pstNode;
    UCOM_MEM_DYN_NODE_STRU             *pstSeek;
    VOS_UINT32                          uwAddr;
    VOS_UINT32                          uwBestFit;

    pstMemCtrlBlk   = UCOM_MEM_DYN_GetMemCtrl();
    uwAddr          = VOS_NULL;

    /* ��ȡ���������� */
    pstFreeList = UCOM_MEM_DYN_GetFreeList(pstMemCtrlBlk, enType);

    /* �����Ƿ���ڷ��������Ĵ�����ڵ� */
    pstSeek = UCOM_MEM_DYN_SeekNodeBySize(uwSize, pstFreeList, &uwBestFit);

    /* �ɽ��з���  */
    if(pstSeek != VOS_NULL)
    {
        /* ������ڴ��ַ */
        uwAddr = pstSeek->stBlk.uwAddr;

        /* ��ȡ�ѷ������� */
        pstUnFreeList = UCOM_MEM_DYN_GetUnFreeList(pstMemCtrlBlk, enType);

        /* ��������ڵ�Ĵ�С��������ͬ, ��ֱ�ӷ��� */
        if(VOS_OK == uwBestFit)
        {
            UCOM_MEM_DYN_DelNode(pstSeek, pstFreeList);
            UCOM_MEM_DYN_InsertNode(pstSeek, pstUnFreeList);
        }
        else
        {
            /* ��ȡһ��δʹ�ýڵ� */
            pstNode = UCOM_MEM_DYN_GetIdleNode();

            if(VOS_NULL == pstNode)
            {
                return VOS_NULL;
            }

            /* ��Ϊ�ѷ���ڵ�������� */
            pstNode->stBlk.uwAddr = pstSeek->stBlk.uwAddr;
            pstNode->stBlk.uwSize = uwSize;

            UCOM_MEM_DYN_InsertNode(pstNode, pstUnFreeList);

            /* ���´�����ڵ���Ϣ */
            pstSeek->stBlk.uwAddr += uwSize;
            pstSeek->stBlk.uwSize -= uwSize;
        }
    }

    return uwAddr;
}
VOS_UINT32  UCOM_MEM_DYN_FreeByType(
                VOS_UINT32                          uwAddr,
                UCOM_MEM_DYN_TYPE_ENUM_UINT32       enType)
{
    UCOM_MEM_DYN_CTRL_STRU             *pstMemCtrlBlk;
    UCOM_MEM_DYN_LIST_STRU             *pstFreeList;
    UCOM_MEM_DYN_LIST_STRU             *pstUnFreeList;
    UCOM_MEM_DYN_NODE_STRU             *pstSeek;
    UCOM_MEM_DYN_NODE_STRU             *pstNext;
    UCOM_MEM_DYN_NODE_STRU             *pstPrev;

    pstMemCtrlBlk   = UCOM_MEM_DYN_GetMemCtrl();

    /* ��ȡ�ѷ������� */
    pstUnFreeList = UCOM_MEM_DYN_GetUnFreeList(pstMemCtrlBlk, enType);

    /* �����Ƿ���ڷ����������ѷ���ڵ� */
    pstSeek = UCOM_MEM_DYN_SeekNodeByAddr(uwAddr, pstUnFreeList);

    /* �ڵ㲻����  */
    if(VOS_NULL == pstSeek)
    {
        return VOS_ERR;
    }

    /* ���ڵ���ѷ���������ɾ�� */
    UCOM_MEM_DYN_DelNode(pstSeek, pstUnFreeList);

    /* ��ȡ���������� */
    pstFreeList = UCOM_MEM_DYN_GetFreeList(pstMemCtrlBlk, enType);

    /* ���ڵ��ص������������� */
    UCOM_MEM_DYN_InsertNode(pstSeek, pstFreeList);

    /* ��ȡǰ���ͺ�̽ڵ� */
    pstPrev = pstSeek->pstPrev;
    pstNext = pstSeek->pstNext;

    /* �������̽ڵ�ϲ� */
    if((pstSeek->stBlk.uwAddr + pstSeek->stBlk.uwSize) == pstNext->stBlk.uwAddr)
    {
        /* �ϲ���̽ڵ�ռ� */
        pstSeek->stBlk.uwSize += pstNext->stBlk.uwSize;

        /* ɾ����̽ڵ� */
        UCOM_MEM_DYN_DelNode(pstNext, pstFreeList);

        /* ���ڵ����δʹ������ */
        UCOM_MEM_DYN_SetIdleNode(pstNext);
    }

    /* ������ǰ���ڵ�ϲ� */
    if((pstPrev->stBlk.uwAddr + pstPrev->stBlk.uwSize) == pstSeek->stBlk.uwAddr)
    {
        /* ǰ���ϲ���ǰ�ڵ�ռ� */
        pstPrev->stBlk.uwSize += pstSeek->stBlk.uwSize;

        /* ɾ����ǰ�ڵ� */
        UCOM_MEM_DYN_DelNode(pstSeek, pstFreeList);

        /* ���ڵ����δʹ������ */
        UCOM_MEM_DYN_SetIdleNode(pstSeek);
    }

    return VOS_OK;
}
UCOM_MEM_DYN_NODE_STRU*  UCOM_MEM_DYN_SeekNodeBySize(
                VOS_UINT32                          uwSize,
                UCOM_MEM_DYN_LIST_STRU             *pstList,
                VOS_UINT32                         *puwBestFit)
{
    UCOM_MEM_DYN_NODE_STRU             *pstNode;
    UCOM_MEM_DYN_NODE_STRU             *pstSeek;
    VOS_UINT32                          uwMaxSize;

    if(0 == pstList->uwLen)
    {
        return VOS_NULL;
    }

    pstSeek     = VOS_NULL;
    uwMaxSize   = 0xFFFFFFFF;   /* �޷����������ֵ */
    *puwBestFit = VOS_ERR;      /* ��ʼ��Ϊ�����ƥ�� */

    /* ȡ��1���ڵ� */
    pstNode = pstList->stHead.pstNext;

    while(pstNode != NULL)
    {
        /* �д�С��ȵ���������� */
        if(pstNode->stBlk.uwSize == uwSize)
        {
            pstSeek     = pstNode;
            *puwBestFit = VOS_OK;
            break;
        }

        /* ������С��ӽ��� */
        if( (pstNode->stBlk.uwSize > uwSize)
          &&(pstNode->stBlk.uwSize < uwMaxSize))
        {
            pstSeek     = pstNode;
            uwMaxSize   = pstNode->stBlk.uwSize;
        }


        pstNode = pstNode->pstNext;
    }

    /* ���ܷ���β�ڵ� */
    if(pstSeek == &pstList->stTail)
    {
        return VOS_NULL;
    }
    else
    {
        return pstSeek;
    }

}


UCOM_MEM_DYN_NODE_STRU*  UCOM_MEM_DYN_SeekNodeByAddr(
                VOS_UINT32                          uwAddr,
                UCOM_MEM_DYN_LIST_STRU             *pstList )
{
    UCOM_MEM_DYN_NODE_STRU             *pstNode;
    UCOM_MEM_DYN_NODE_STRU             *pstSeek;

    if(0 == pstList->uwLen)
    {
        return VOS_NULL;
    }

    pstSeek     = VOS_NULL;

    /* ȡ��1���ڵ� */
    pstNode = pstList->stHead.pstNext;

    while(pstNode != NULL)
    {
        /* �е�ַ��ȵ���������� */
        if(pstNode->stBlk.uwAddr == uwAddr)
        {
            pstSeek  = pstNode;
            break;
        }

        pstNode = pstNode->pstNext;
    }

    /* ���ܷ���β�ڵ� */
    if(pstSeek == &pstList->stTail)
    {
        return VOS_NULL;
    }
    else
    {
        return pstSeek;
    }

}
VOS_VOID UCOM_MEM_DYN_SetIdleNode(UCOM_MEM_DYN_NODE_STRU *pstNode)
{
    UCOM_MEM_DYN_CTRL_STRU             *pstMemCtrlBlk;
    UCOM_MEM_DYN_LIST_STRU             *pstIdleList;

    pstMemCtrlBlk = UCOM_MEM_DYN_GetMemCtrl();
    pstIdleList   = UCOM_MEM_DYN_GetIdleList(pstMemCtrlBlk);

    UCOM_MEM_DYN_AddNode(pstNode, pstIdleList);
}


UCOM_MEM_DYN_NODE_STRU* UCOM_MEM_DYN_GetIdleNode( VOS_VOID )
{
    UCOM_MEM_DYN_CTRL_STRU             *pstMemCtrlBlk;
    UCOM_MEM_DYN_LIST_STRU             *pstIdleList;
    UCOM_MEM_DYN_NODE_STRU             *pstNode;


    pstMemCtrlBlk = UCOM_MEM_DYN_GetMemCtrl();
    pstIdleList   = UCOM_MEM_DYN_GetIdleList(pstMemCtrlBlk);

    if(0 == pstIdleList->uwLen)
    {
        return VOS_NULL;
    }

    /* ȡ��һ���ڵ� */
    pstNode = pstIdleList->stHead.pstNext;

    /* ��Idle������ɾ������ڵ� */
    UCOM_MEM_DYN_DelNode(pstNode, pstIdleList);

    return pstNode;
}



 VOS_VOID UCOM_MEM_DYN_InitList(UCOM_MEM_DYN_LIST_STRU *pstList)
{
    UCOM_MEM_DYN_NODE_STRU      *pstNode;

    /* ����ͷ��� */
    pstNode = &pstList->stHead;
    /* ��β�ڵ��ַΪ32bit��С��ַ������ڵ���� */
    pstNode->stBlk.uwAddr = 0;
    pstNode->stBlk.uwSize = 0;
    pstNode->pstPrev = VOS_NULL;
    pstNode->pstNext = &pstList->stTail;

    /* ����β�ڵ� */
    pstNode = &pstList->stTail;
    /* ��β�ڵ��ַΪ32bit����ַ������ڵ���� */
    pstNode->stBlk.uwAddr = 0xFFFFFFFF;
    pstNode->stBlk.uwSize = 0;
    pstNode->pstPrev = &pstList->stHead;
    pstNode->pstNext = VOS_NULL;

    /* ���������� */
    pstList->uwLen = 0;
}
VOS_VOID UCOM_MEM_DYN_AddNode(
                UCOM_MEM_DYN_NODE_STRU      *pstNode,
                UCOM_MEM_DYN_LIST_STRU      *pstList )
{
    UCOM_MEM_DYN_NODE_STRU          *pstCurrNode;

    /* ȡ��һ���ڵ� */
    pstCurrNode = pstList->stHead.pstNext;

    /* ����ڵ� */
    pstCurrNode->pstPrev->pstNext   = pstNode;
    pstNode->pstPrev                = pstCurrNode->pstPrev;
    pstNode->pstNext                = pstCurrNode;
    pstCurrNode->pstPrev            = pstNode;

    /* ���������� */
    pstList->uwLen++;

}
VOS_VOID UCOM_MEM_DYN_InsertNode(
                UCOM_MEM_DYN_NODE_STRU      *pstNode,
                UCOM_MEM_DYN_LIST_STRU      *pstList )
{
    UCOM_MEM_DYN_NODE_STRU          *pstCurrNode;

    /* ȡ��һ���ڵ� */
    pstCurrNode = pstList->stHead.pstNext;

    /* ����������� */
    while(pstCurrNode != VOS_NULL)
    {
        if(pstCurrNode->stBlk.uwAddr >= pstNode->stBlk.uwAddr)
        {
            /* ����ڵ�, ��֤��ַ��С�������� */
            pstCurrNode->pstPrev->pstNext   = pstNode;
            pstNode->pstPrev                = pstCurrNode->pstPrev;
            pstNode->pstNext                = pstCurrNode;
            pstCurrNode->pstPrev            = pstNode;

            /* ���������� */
            pstList->uwLen++;

            break;
        }

        /* ��һ���ڵ� */
        pstCurrNode = pstCurrNode->pstNext;
    }
}
VOS_VOID UCOM_MEM_DYN_DelNode(
                UCOM_MEM_DYN_NODE_STRU      *pstNode,
                UCOM_MEM_DYN_LIST_STRU      *pstList )
{
   UCOM_MEM_DYN_NODE_STRU                   *pstPrevNode;
   UCOM_MEM_DYN_NODE_STRU                   *pstNextNode;

   /* ��ȡ�ڵ��ǰ��ڵ� */
   pstPrevNode = pstNode->pstPrev;
   pstNextNode = pstNode->pstNext;

   /* ɾ���ڵ� */
   pstPrevNode->pstNext = pstNextNode;
   pstNextNode->pstPrev = pstPrevNode;

   pstList->uwLen--;
}






#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

