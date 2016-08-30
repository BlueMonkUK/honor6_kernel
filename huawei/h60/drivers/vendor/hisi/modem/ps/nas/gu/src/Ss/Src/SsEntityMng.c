/*******************************************************************************
  Copyright    : 2005-2007, Huawei Tech. Co., Ltd.
  File name    : SsEntityMng.c
  Description  : SSʵ�����
  Function List:
      1.  Ss_EntityBuildTi      ѡ��TI
      2.  Ss_EntityBuildCr      ѡ��CR
      3.  Ss_EntityCreat        ����ʵ��
      4.  Ss_EntityClear        �ͷ�ʵ��
      5.  Ss_EntitySelectTi     ����CR��TAF IDѡ�����Ӧ��TI
      6.  Ss_AribMalloc         Ϊ����ARIB�ṹ�ṩ�ڴ�ռ�
      7.  Ss_AribFree           �ͷŵ�����ARIB�ṹ������������ڴ�
  History:
      1.  ��־�� 2004.03.08   �°�����
      2.  ��  �� 2006.11.07   ���ⵥA32D07063
*******************************************************************************/

#include "SsInclude.h"


#ifdef  __cplusplus
  #if  __cplusplus
  extern "C"{
  #endif
#endif


/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_SS_ENTITYMNG_C


/***********************************************************************
*  MODULE   : Ss_EntityBuildTi
*  FUNCTION : ΪMO�����е�SSʵ��ѡ��һ���µ�TI
*  INPUT    : VOS_UINT8             ucCr        ��Ϣ�е�CR
*             VOS_UINT8             ucTafId     ��Ϣ�е�TAF ID
*  OUTPUT   : VOS_VOID
*  RETURN   : VOS_UINT8             ucTi    ��ѡ���TI
*  NOTE     : ��
*  HISTORY  :
*     1.  ��־�� 04-03-08  �°�����
************************************************************************/

VOS_UINT8 Ss_EntityBuildTi(
                            VOS_UINT8 ucCr
                       )
{
    VOS_UINT8   i;

    for( i = 8 ; i < SS_MAX_ENTITY_NUM ; i++ )
    {                                                                           /* ѭ����������TI                           */
        if(( ucCr == g_SsEntity[i].ucCr )
            && ( SS_S_NULL != g_SsEntity[i].ucState ))
        {                                                                       /* �ж�ָ��TI�Ƿ��Ѵ���                     */
            return SS_NONE_TI;                                                  /* ����û�ҵ�TI                             */
        }
    }

    for( i = 8 ; i < SS_MAX_ENTITY_NUM ; i++ )
    {                                                                           /* ѭ����������TI                           */
        if( SS_S_NULL == g_SsEntity[i].ucState )
        {                                                                       /* �ж��Ƿ���ڿ��е�TI                     */
            return i;                                                           /* ���ش�TI                                 */
        }
    }
    return SS_NONE_TI;                                                          /* ����û�ҵ�TI                             */
}

/***********************************************************************
*  MODULE   : Ss_EntityBuildCr
*  FUNCTION : ΪMT�����е�SSʵ��ѡ��һ���µ�CR
*  INPUT    : VOS_UINT8     ucTi        ָ��ʵ���TI
*  OUTPUT   : VOS_VOID
*  RETURN   : VOS_UINT8     ucCr        ��ѡ���CR
*  NOTE     : ��
*  HISTORY  :
*     1.  ��־�� 04-03-08  �°�����
************************************************************************/

VOS_UINT8 Ss_EntityBuildCr( VOS_UINT8 ucTi )
{
    if( SS_S_NULL == g_SsEntity[ucTi].ucState )
    {                                                                           /* �ж�ָ��TI�Ƿ��Ѵ���                     */
        return ((VOS_UINT8)( ucTi + 128 ));                                         /* ����û�ҵ�TI                             */
    }
    return SS_NONE_CR;                                                          /* ����û�ҵ�CR                             */
}

/***********************************************************************
*  MODULE   : Ss_EntityCreat
*  FUNCTION : ����ʵ��
*  INPUT    : VOS_UINT8     ucTi        ָ��ʵ���TI
*             VOS_UINT8     ucCr        ָ��ʵ���Ӧ��CR
*             VOS_UINT8     ucTafId     ָ��ʵ���Ӧ��TAF ID
*  OUTPUT   : VOS_VOID
*  RETURN   : VOS_VOID
*  NOTE     : ��
*  HISTORY  :
*     1.  ��־�� 04-03-08  �°�����
************************************************************************/

VOS_VOID Ss_EntityCreat(
                    VOS_UINT8 ucTi,
                    VOS_UINT8 ucCr
                    )
{
    SS_NORM_LOG( "\n Create SS entity()\r");
    g_SsEntity[ucTi].ucCr = ucCr;                                               /* ����CR                                   */
}

/***********************************************************************
*  MODULE   : Ss_EntityClear
*  FUNCTION : �ͷ�ʵ��
*  INPUT    : VOS_UINT8     ucTi        ָ��ʵ���TI
*  OUTPUT   : VOS_VOID
*  RETURN   : VOS_VOID
*  NOTE     : ��
*  HISTORY  :
*     1.  ��־�� 04-03-08  �°�����
*     2.  ��־�� 04-03-23  ����ֹͣT551�Ĵ���
************************************************************************/


VOS_VOID Ss_EntityClear(
                    VOS_UINT8 ucTi
                    )
{
    g_SsEntity[ucTi].ucState = SS_S_NULL;                                       /* ����ʵ��״̬                             */
    SS_NORM_LOG( "\n SS state = SS_S_NULL() \r" );
    SS_NORM_LOG( "\n Destroy SS entity()\r" );
    g_SsEntity[ucTi].ucCr = SS_NONE_CR;                                         /* ���CR                                   */
}


/***********************************************************************
*  MODULE   : Ss_EntitySelectTi
*  FUNCTION : ����CR��TAF IDѡ�����Ӧ��TI
*  INPUT    : VOS_UINT8     ucCr        ָ��ʵ���Ӧ��CR
*             VOS_UINT8     ucTafId     ָ��ʵ���Ӧ��TAF ID
*  OUTPUT   : VOS_VOID
*  RETURN   : VOS_UINT8     ucTi;
*  NOTE     : ��
*  HISTORY  :
*     1.  ��־�� 04-03-08  �°�����
*     2.
************************************************************************/

VOS_UINT8 Ss_EntitySelectTi(
                            VOS_UINT8 ucCr
                        )
{
    VOS_UINT8   i;                                                                  /* ����ѭ��                                 */
    VOS_UINT8   ucTi;                                                               /* �������ڷ��ص�TI                         */

    ucTi = SS_NONE_TI;                                                          /* ��ʼ��TI                                 */

    if( ucCr <= SS_MO_CR_NUM )
    {                                                                           /* MO����                                   */
        for( i = 8 ; i < SS_MAX_ENTITY_NUM ; i++ )
        {                                                                       /* ����MO���̵�����SSʵ��                   */
            if( ucCr == g_SsEntity[i].ucCr )
            {                                                                   /* �ҵ���ָ����CR��TAF ID                   */
                if( SS_S_CONNECTED == g_SsEntity[i].ucState )
                {                                                               /* ��Ӧ��SSʵ���״̬ΪSS_S_CONNECTED       */
                    ucTi = i;                                                   /* ���ظ�TI                                 */
                }
                else
                {                                                               /* ʵ��״̬����                             */
                    SS_WARN_LOG("\n [Error]SsEntity State Error! \r" );
                }
                break;                                                          /* ����ѭ��                                 */
            }
        }
    }
    else
    {                                                                           /* MT����                                   */
        for( i = 0 ; i < SS_MT_ENTITY_NUM ; i++ )
        {                                                                       /* ����MT���̵�����SSʵ��                   */
            if( ucCr == g_SsEntity[i].ucCr )
            {                                                                   /* �ҵ���ָ����CR                           */
                if( SS_S_CONNECTED == g_SsEntity[i].ucState )
                {                                                               /* ��Ӧ��SSʵ���״̬ΪSS_S_CONNECTED       */
                        ucTi = i;                                               /* ���ظ�TI                                 */
                    }
                else
                {                                                               /* ʵ��״̬����                             */
                    SS_WARN_LOG("\n [Error]SsEntity State Error! \r" );
                }
                break;                                                          /* ����ѭ��                                 */
            }
        }
    }
    return ucTi;                                                                /* �����ҵ���TI                             */
}

VOS_UINT8 Ss_GetSsTi(VOS_UINT8  ucCr)
{
    VOS_UINT8  i;
    VOS_UINT8  ucTi = SS_NONE_TI;

    /* ���緢��USSDҵ��Ҫ���û�ȷ�ϣ��û���ȷ������ʱ����ʱCr��Χ��8-14,ת����128-134 */
    if ((ucCr > (TAF_MIDDLE_SSA_TI))
     && (ucCr <= TAF_MAX_SSA_TI))
    {
        ucCr += 120;
    }

    for( i = 0 ; i < SS_MAX_ENTITY_NUM ; i++ )
    {
        if( ucCr == g_SsEntity[i].ucCr )
        {
            ucTi = i;
            break;
        }
    }

    return ucTi;
}


#ifdef  __cplusplus
  #if  __cplusplus
  }
  #endif
#endif

