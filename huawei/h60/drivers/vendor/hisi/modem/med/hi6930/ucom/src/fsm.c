/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : fsm.c
  �� �� ��   : ����
  ��    ��   : �ۺ��� 45755
  ��������   : 2009��07��10��
  ����޸�   :
  ��������   : ״̬����ת��������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2009��07��10��
    ��    ��   : �ۺ��� 45755
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/

#include "vos.h"
#include "fsm.h"
#include "ucom_comm.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*****************************************************************************
 �� �� ��  : FSM_Swap
 ��������  : ֵ��������
 �������  : VOS_INT16 *pshwTarget
             VOS_INT16 *pshwSource
             VOS_UINT32     uwLength����λ:sizeof
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2009��7��24��
    ��    ��   : chenchaoqun
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID FSM_Swap(
                        VOS_INT16        *pshwTarget,
                        VOS_INT16        *pshwSource,
                        VOS_UINT32        uwLength )
{

    /*  ���ݽ����������޶����������ݳ��Ȳ�����FSM_COMP_LENGTH_MAX����λ:word */
    VOS_INT16                           auwTemp[FSM_COMP_LENGTH_MAX];

    UCOM_MemCpy(&auwTemp[0], pshwTarget, uwLength);
    UCOM_MemCpy(pshwTarget, pshwSource, uwLength);
    UCOM_MemCpy(pshwSource, &auwTemp[0], uwLength);
}
/*****************************************************************************
 �� �� ��  : FSM_Sort
 ��������  : ð��������
 �������  : VOS_INT16 * pTable
             VOS_UINT32 uwNum
             VOS_UINT32 uwWidth,pTable��ָ��ṹ�峤��,��λsizeof
             FSM_COMP_FUNC fCompareFunc
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2009��7��24��
    ��    ��   : chenchaoqun
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID FSM_Sort(
                      VOS_UINT8            *puhwTable,
                      VOS_UINT32            uwNum,
                      VOS_UINT32            uwWidth,
                      FSM_COMP_FUNC         fCompareFunc)
{
    VOS_INT16                       shwRet;
    VOS_UINT32                      uwLoop;
    VOS_UINT32                      uwSubLoop;


    for ( uwLoop = 0 ; uwLoop < uwNum ; uwLoop++ )
    {

        /*���±�Ϊ0->i֮�����ݽ�������*/
        for ( uwSubLoop = 0 ; uwSubLoop < ((uwNum - uwLoop) - 1 ); uwSubLoop++ )
        {
             shwRet = fCompareFunc( puhwTable + ( uwSubLoop * uwWidth ),
                                    puhwTable + ( (uwSubLoop + 1) * uwWidth ) );

             /* ��ǰ�ߴ��ں��ߣ����߽�������ǰ�ߺ���*/
             if ( 1 == shwRet )
             {
                 FSM_Swap((VOS_INT16*)(puhwTable + (uwSubLoop*uwWidth)),(VOS_INT16*)(puhwTable+((uwSubLoop+1)*uwWidth)),uwWidth);
             }

        }
    }
}

/*****************************************************************************
 �� �� ��  : FSM_Bsearch
 ��������  : �۰���Һ���,��������
 �������  : VOS_INT16 * pKey
             VOS_INT16 * pBase
             VOS_UINT32 uwNum
             VOS_UINT32 uwWidth
             FSM_COMP_FUNC fCompareFunc
 �������  : ��
 �� �� ֵ  : VOS_VOID *
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2009��7��24��
    ��    ��   : chenchaoqun
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID * FSM_Bsearch(
                             VOS_INT16      *pKey,
                             VOS_UINT8      *pBase,
                             VOS_UINT32      uwNum,
                             VOS_UINT32      uwWidth,
                             FSM_COMP_FUNC   fCompareFunc)
{
    VOS_INT32                           swLow;
    VOS_INT32                           swMid;
    VOS_INT32                           swHigh;
    VOS_VOID                           *pRet = VOS_NULL_PTR;
    VOS_INT16                           shwRet;

    swLow       = 0;
    swHigh      = (VOS_INT32)uwNum - 1;

    while(swLow <= swHigh)
    {

        swMid   = (swLow + swHigh) >> 1;
        shwRet  = fCompareFunc(pKey, pBase + ((VOS_UINT32)swMid * uwWidth));

        /* ��Ҫ���ҵ�ֵ���ڸ�ֵ�����Ը�ֵ����һ��ֵΪ��ʼ���һ������ */
        if (1 == shwRet)
        {
            swLow = swMid + 1;
        }
        /* ��Ҫ���ҵ�ֵС�ڸ�ֵ�����Ը�ֵ����һ��ֵΪ��ֹ���һ������ */
        else if ( -1 == shwRet )
        {
            swHigh = swMid - 1;
        }
        else
        {
            /* �����ҵ��򷵻ظ�λ�� */
            pRet = pBase + ((VOS_UINT32)swMid * uwWidth);
            break;
        }
    }
    return pRet;
}

/*****************************************************************************
 Function   : FSM_StaCompare
 Description: ״̬�ȽϺ������ڶ�״̬ת�Ʊ���п�������ʱ�õ�
 Calls      :
 Called By  :
 Input      : ״̬1 ״̬2
 Output     :
 Return     : 0 -- ״̬1��״̬2���
             -1 -- ״̬1С��״̬2
              1 -- ״̬1����״̬2
 Other      : �ڲ�����
 *****************************************************************************/
VOS_INT16 FSM_StaCompare( const VOS_VOID *arg1, const VOS_VOID *arg2 )
{
    STA_STRU                    *pstSta1;
    STA_STRU                    *pstSta2;

    pstSta1 = (STA_STRU*) arg1;
    pstSta2 = (STA_STRU*) arg2;

    /*  ���״̬1С��״̬2,����-1 */
    if (pstSta1->ulState < pstSta2->ulState)
    {
        return -1;
    }

    /*  ���״̬1����״̬2,����0 */
    if (pstSta1->ulState == pstSta2->ulState)
    {
        return 0;
    }

    /*  ���״̬1����״̬2,����1 */
    return 1;
}


/*****************************************************************************
 Function   : FSM_ActCompare
 Description: �¼��ȽϺ������ڶ��¼��������п�������ʱ�õ�
 Calls      :
 Called By  :
 Input      : �¼�1 �¼�2
 Output     :
 Return     : 0 -- �¼�1���¼�2���
             -1 -- �¼�1С���¼�2
              1 -- �¼�1�����¼�2
 Other      : �ڲ�����
 *****************************************************************************/
VOS_INT16 FSM_ActCompare( const VOS_VOID *arg1, const VOS_VOID *arg2 )
{
    ACT_STRU                    *pstAct1;
    ACT_STRU                    *pstAct2;

    pstAct1 = (ACT_STRU*) arg1;
    pstAct2 = (ACT_STRU*) arg2;

    /*  ����¼�1С���¼�2,����-1 */
    if (pstAct1->ulEventType < pstAct2->ulEventType)
    {
        return -1;
    }

    /*  ����¼�1�����¼�2,����0 */
    if (pstAct1->ulEventType == pstAct2->ulEventType)
    {
        return 0;
    }

    /*  ����¼�1�����¼�2,����0 */
    return 1;
}


/*****************************************************************************
 Function   : FSM_FindAct
 Description: ����״̬���¼�������������״̬ת�Ʊ��п��ٲ����¼�������
 Calls      :
 Called By  :
 Input      : ״̬�������� ״̬ �¼�����
 Output     :
 Return     : �¼�������
 Other      : �ڲ�����
 *****************************************************************************/
ACTION_FUN FSM_FindAct( FSM_DESC_STRU  *pFsmDesc,
                             VOS_UINT32      uwState,
                             VOS_UINT32      uwEventType)
{
    STA_STRU             stKeySta;      /* Ҫ���ҵ�״̬ */
    STA_STRU            *pstRetSta;     /* ���ص�״̬   */
    ACT_STRU             stKeyAct;      /* Ҫ���ҵ��¼� */
    ACT_STRU            *pstRetAct;     /* ���ص��¼�   */

    pstRetSta           = VOS_NULL_PTR;
    pstRetAct           = VOS_NULL_PTR;

    stKeySta.ulState    = uwState;

    /* ���ö��ֲ��Һ���bsearch��״̬ת�Ʊ��в�����Ӧ���¼������ */
    pstRetSta           = (STA_STRU*) FSM_Bsearch( (VOS_INT16 *)(&stKeySta),
                                                   (VOS_UINT8 *)pFsmDesc->pStaTable,
                                                   pFsmDesc->ulSize,
                                                   sizeof(STA_STRU)/sizeof(VOS_INT8),
                                                  (FSM_COMP_FUNC)FSM_StaCompare );

    /* ����Ҳ�����Ӧ���¼������,���ô�����,����VOS_NULL_PTR */
    if (VOS_NULL_PTR == pstRetSta)
    {
        /* �˴���ԭ������ͬ��������Dopra���ش����룬
           ����ֱ�ӷ��ؿ�ָ�룬��ʾ����Ϣû�б���״̬������ */
        return (ACTION_FUN) VOS_NULL_PTR;
    }

    stKeyAct.ulEventType = uwEventType;

    /* ���ö��ֲ��Һ���bsearch���¼�������в�����Ӧ���¼������� */
    pstRetAct = (ACT_STRU*) FSM_Bsearch ( (VOS_INT16 *)(&stKeyAct),
                                          (VOS_UINT8 *)pstRetSta->pActTable,
                                          pstRetSta->ulSize,
                                          sizeof(ACT_STRU)/sizeof(VOS_INT8),
                                          (FSM_COMP_FUNC)FSM_ActCompare );


    /* ����Ҳ�����Ӧ���¼�������,���ô�����,����VOS_NULL_PTR */
    if (VOS_NULL_PTR == pstRetAct)
    {
        /* �˴���ԭ������ͬ��������Dopra���ش����룬
           ����ֱ�ӷ��ؿ�ָ�룬��ʾ����Ϣû�б���״̬������ */
        return (ACTION_FUN) VOS_NULL_PTR;
    }

    return (pstRetAct->pfActionFun);

}

/*****************************************************************************
 Function   : FSM_StaSort
 Description: ��״̬ת�Ʊ�ͺ������ÿ���¼�����������п�������
 Calls      :
 Called By  :
 Input      : ״̬ת�Ʊ� ״̬ת�Ʊ�Ĵ�С
 Output     : ������״̬ת�Ʊ�
 Return     :
 Other      : �ڲ�����
 *****************************************************************************/
VOS_VOID FSM_StaSort(STA_STRU* pstTable, VOS_UINT32 uwSize)
{
    VOS_UINT32    uwIndex;

    /* ����ð�������pTable�������� */
    FSM_Sort( (VOS_UINT8 *)pstTable,
               uwSize,
               sizeof(STA_STRU),
              (FSM_COMP_FUNC)FSM_StaCompare);

    /* ��ÿһ���¼������������ð������������� */
    for (uwIndex = 0; uwIndex < uwSize; uwIndex++)
    {
        FSM_Sort( (VOS_UINT8 *)pstTable->pActTable,
                   pstTable->ulSize,
                   sizeof(ACT_STRU),
                  (FSM_COMP_FUNC)FSM_ActCompare );

        /* �������е���һ��STA_STRUԪ���еĶ������������ */
        pstTable++;
    }
}

/*****************************************************************************
 Function   : FSM_RegisterFsm
 Description: �Ǽ�״̬����
 Calls      :
 Called By  :
 Input      : ״̬������ ״̬ת�Ʊ� ״̬ת�Ʊ��С �쳣������
 Output     : ״̬��������
 Return     : SUCC          -- success
              other err code    -- failure
 Other      : �ⲿ����
 *****************************************************************************/
VOS_UINT32 FSM_RegisterFsm( FSM_DESC_STRU   *pstFsmDesc,
                                  VOS_UINT32       uwSize,
                                  STA_STRU        *pstStaTable,
                                  EXCEP_FUN        pfExceptHandle )
{

    /* �����ڲ��� */
    if ( ( VOS_NULL_PTR == pstFsmDesc ) || ( VOS_NULL_PTR == pstStaTable ) )
    {
        return VOS_ERR;
    }

    /* ��״̬���������ṹ���и�ֵ */
    pstFsmDesc->ulSize         = uwSize;
    pstFsmDesc->pStaTable      = pstStaTable;
    pstFsmDesc->pfExceptHandle = pfExceptHandle;

    /* ��״̬ת�Ʊ�������� */
    FSM_StaSort(pstStaTable, uwSize);

    return VOS_OK;

}


/*****************************************************************************
 Function   : FSM_ProcessEvent
 Description: ״̬�����¼�������
 Calls      :
 Called By  :
 Input      : ״̬�������� ��ǰ״̬ �����¼� ��ϢID ��Ϣָ��
 Output     :
 Return     :
 Other      : �ⲿ����
 *****************************************************************************/
VOS_UINT32 FSM_ProcessEvent(
                    FSM_DESC_STRU                   *pFsmDesc,
                    VOS_UINT32                       uwCurState,
                    VOS_UINT32                       uwEventType,
                    VOS_UINT16                       uhwMsgID,
                    VOS_VOID                        *pRcvMsg )
{
    ACTION_FUN                          pActFun;

    /* ����״̬ת�Ʊ��е���Ӧ�Ķ��������� */
    pActFun = FSM_FindAct(pFsmDesc, uwCurState, uwEventType);

    if (pActFun != VOS_NULL_PTR)
    {
        /* ������ص��¼���������Ϊ��,�����������¼����� */
        return (*pActFun) ( pRcvMsg );
    }
    /* ����Ҳ�����������������ʾ�쳣��������ʾ����Ϣû�б���ǰ״̬������
        ����������£���Ҫ���ø�״̬�����쳣��������������Ϣ�ܹ����쳣�������д��� */
    else
    {
        /* �쳣�������� */
        if (VOS_NULL_PTR != pFsmDesc->pfExceptHandle)
        {
            return (*pFsmDesc->pfExceptHandle) (uwEventType, pRcvMsg );
        }
        /* ���û���쳣����������ʾ����Ϣû�б���״̬������ */
        else
        {
            return VOS_ERR;
        }
    }
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

