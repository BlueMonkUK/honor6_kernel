

#ifndef __SCCTX_H__
#define __SCCTX_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "ScFactory.h"
#include "ScComm.h"
#include "ScApSec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  7 STRUCT����
*****************************************************************************/

typedef struct
{
    VOS_CHAR                           *acSecretFileName[SC_SECRET_FILE_TYPE_BUTT];
    VOS_CHAR                           *acSignFileName[SC_SECRET_FILE_TYPE_BUTT];
}SC_COMM_GLOBAL_STRU;


typedef struct
{
    SC_AUTH_STATUS_ENUM_UINT8               enScFacAuthStatus;
    VOS_UINT8                               aucScFacRandData[SC_RAND_DATA_LEN];
    /* ������߼�Ȩ��־ VOS_FALSE: δ������߼�Ȩ; VOS_TRUE: �ѷ�����߼�Ȩ */
    VOS_UINT8                               ucIdentifyStartFlg;
    VOS_UINT8                               aucReserved[2];         /* �����ֽڣ��������ֽڶ��� */
}SC_FAC_GLOBAL_STRU;


typedef struct
{
    VOS_UINT32                          ulItemLen;
    VOS_CHAR                           *pucContent;
    VOS_UINT8                           aucHash[SC_HASH_LEN];
}SC_APSEC_ITEM_STRU;


typedef struct
{
    VOS_UINT8                                               ucSeqID;
    VOS_UINT8                                               aucReserved[2];
    VOS_UINT8                                               ucSecATkeyFlg;
    VOS_UINT8                                               aucSecATkey[SC_SEC_AT_KEY_LEN];
    SC_APSEC_FILE_STATE_INIT_ENUM_UINT32                    ulInitState;
    SC_APSEC_ITEM_STRU                                      astApSecItem[SC_SECURITY_ITEM_NUM];
    VOS_CHAR                                               *pApSecFileA;
    VOS_CHAR                                               *pApSecFileC;
}SC_APSEC_GLOBAL_STRU;


typedef struct
{
    SC_COMM_GLOBAL_STRU                 stScCommGlobalVar;
    SC_FAC_GLOBAL_STRU                  stScFacGlobalVar;
    SC_APSEC_GLOBAL_STRU                stScApSecGlobalVar;
}SC_CONTEXT_STRU;

/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/


extern SC_CONTEXT_STRU* SC_CTX_GetScCtxAddr(VOS_VOID);


extern SC_COMM_GLOBAL_STRU*  SC_CTX_GetCommGlobalVarAddr( VOS_VOID );


extern VOS_VOID  SC_CTX_InitFacGlobalVar( VOS_VOID );


extern SC_AUTH_STATUS_ENUM_UINT8  SC_CTX_GetFacAuthStatus( VOS_VOID );


extern VOS_VOID  SC_CTX_SetFacAuthStatus( SC_AUTH_STATUS_ENUM_UINT8 enScAuthStatus );


VOS_UINT8  SC_CTX_GetIdentifyStartFlg( VOS_VOID );


VOS_VOID  SC_CTX_SetIdentifyStartFlg( VOS_UINT8 ucIdentifyStartFlg );


extern VOS_UINT8*  SC_CTX_GetFacRandDataAddr( VOS_VOID );


extern VOS_UINT32*  SC_CTX_GetApSecInitStateAddr(VOS_VOID);


extern SC_APSEC_ITEM_STRU*  SC_CTX_GetApSecItemBaseAddr(VOS_VOID);


extern VOS_UINT8 SC_CTX_GetSecATkeyFlg( VOS_VOID );


extern VOS_VOID SC_CTX_SetSecATkeyFlg( VOS_UINT8 ucSecATkeyFlg );


extern VOS_UINT8* SC_CTX_GetSecATkeyAddr( VOS_VOID );


VOS_UINT8 SC_CTX_GetApSecSeqID(VOS_VOID);


VOS_VOID SC_CTX_SetApSecSeqID(VOS_UINT8 ucSeqID);




#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif
