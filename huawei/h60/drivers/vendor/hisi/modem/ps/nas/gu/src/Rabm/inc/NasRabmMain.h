
#ifndef _NAS_RABM_MAIN_H_
#define _NAS_RABM_MAIN_H_

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "AtRabmInterface.h"
#include "NVIM_Interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)


/*****************************************************************************
  2 �궨��
*****************************************************************************/

#define NAS_RABM_GetProFuncTblAddr(enSysMode)                   (g_pstNasRabmProcFuncTbl[enSysMode])
#define NAS_RABM_SetProFuncTblAddr(enSysMode, pstProcFucTbl)    (g_pstNasRabmProcFuncTbl[enSysMode] = (pstProcFucTbl))

#define NAS_RABM_GetCurrentSysMode()                (g_enNasRabmSysMode)
#define NAS_RABM_SetSysMode(enMode)                 (g_enNasRabmSysMode = (enMode))

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

enum NAS_RABM_FASTDORM_STATUS_ENUM
{
    NAS_RABM_FASTDORM_STOP,                                                     /* ��ǰFAST DORMANCY��ֹͣ״̬ */
    NAS_RABM_FASTDORM_INIT_DETECT,                                              /* ��ǰFAST DORMANCY�ڳ�ʼ������� */
    NAS_RABM_FASTDORM_WAIT_GMM_QRY_RESULT,                                      /* ��ǰFAST DORMANCY�ڵȴ�GMM����ҵ���ѯ���״̬ */
    NAS_RABM_FASTDORM_RUNNING,                                                  /* ����START DORMANCY�󣬵�ǰFAST DORMANCY������״̬ */
    NAS_RABM_FASTDORM_RETRY,                                                    /* ��ǰ��ҵ����������̣���ǰFAST DORMANCY��RETRY״̬ */
    NAS_RABM_FASTDORM_PAUSE,                                                    /* ��������������ǰFAST DORMANCY����ͣ״̬ */
    NAS_RABM_FASTDORM_SUSPEND,                                                  /* ����FAST DORMANCY�󣬵�2G��ʱ����ǰFAST DORMANCY�ڹ���״̬ */
    NAS_RABM_FASTDORM_BUTT
};
typedef VOS_UINT32 NAS_RABM_FASTDORM_STATUS_ENUM_UINT32;
enum NAS_RABM_OM_MSG_ID_ENUM
{
    /* MMC���͸�OM����Ϣ */
    NAS_RABM_OM_MSG_FASTDORM_STATUS = 0x1111,                                   /*_H2ASN_MsgChoice  NAS_MMC_LOG_FSM_INFO_STRU */
    NAS_RABM_OM_MSG_BUTT
};
typedef VOS_UINT32 NAS_RABM_OM_MSG_ID_ENUM_UINT32;


/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  7 STRUCT����
*****************************************************************************/

typedef VOS_VOID (*NAS_RABM_SYS_MODE_CHG_PROC_FUNC)(VOS_VOID);

typedef VOS_VOID (*NAS_RABM_SM_ACTIVATE_IND_PROC_FUNC)(VOS_VOID *pMsg);
typedef VOS_VOID (*NAS_RABM_SM_DEACTIVATE_IND_PROC_FUNC)(VOS_VOID *pMsg);
typedef VOS_VOID (*NAS_RABM_SM_MODIFY_IND_PROC_FUNC)(VOS_VOID *pMsg);

typedef VOS_VOID (*NAS_RABM_GMM_REESTABLISH_CNF_PROC_FUNC)(VOS_VOID *pMsg);
typedef VOS_VOID (*NAS_RABM_DATA_SUSPEND_IND_FUNC)(VOS_BOOL bRatChangeFlg);
typedef VOS_VOID (*NAS_RABM_DATA_RESUME_IND_FUNC)(VOS_UINT8 ucRebuildRabFlag);

typedef VOS_VOID (*NAS_RABM_SYS_MODE_CHG_IND_FUNC)(GMM_RABM_NET_RAT_ENUM_UINT32 enOldSysMode, GMM_RABM_NET_RAT_ENUM_UINT32 enNewSysMode);
typedef VOS_VOID (*NAS_RABM_TASK_ENTRY_FUNC)(struct MsgCB* pMsg);

/*****************************************************************************
 �ṹ��    : NAS_RABM_PROC_FUNC_TBL_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : RABM��Ϣ��������ṹ
*****************************************************************************/
typedef struct
{
    NAS_RABM_SM_ACTIVATE_IND_PROC_FUNC      pSmActivateIndProcFunc;
    NAS_RABM_SM_DEACTIVATE_IND_PROC_FUNC    pSmDeactivateIndProcFunc;
    NAS_RABM_SM_MODIFY_IND_PROC_FUNC        pSmModifyIndProcFunc;

    NAS_RABM_GMM_REESTABLISH_CNF_PROC_FUNC  pGmmReestablishCnfProcFunc;

    NAS_RABM_DATA_SUSPEND_IND_FUNC          pDataSuspendIndProcFunc;
    NAS_RABM_DATA_RESUME_IND_FUNC           pDataResumeIndProcFunc;
    NAS_RABM_SYS_MODE_CHG_IND_FUNC          pSysModeChgIndProcFunc;

    NAS_RABM_TASK_ENTRY_FUNC                pTaskEntry;

}NAS_RABM_PROC_FUNC_TBL_STRU;

/*****************************************************************************
 �ṹ��    : NAS_RABM_SYS_CHG_TBL_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :  ģʽ�ı䴦������ṹ
*****************************************************************************/
typedef struct
{
    GMM_RABM_NET_RAT_ENUM_UINT32        enOldMode;
    GMM_RABM_NET_RAT_ENUM_UINT32        enNewMode;
    NAS_RABM_SYS_MODE_CHG_PROC_FUNC     pSysModeChgProcFunc;

} NAS_RABM_SYS_CHG_TBL_STRU;



typedef struct
{
    VOS_UINT32                                              ulCurrNoFluxCnt;            /* ������ʱ������������Ϊ��λ */
    AT_RABM_FASTDORM_OPERATION_ENUM_UINT32                  enFastDormOperationType;    /* ��ǰFAST DORMANCY�Ĳ�����ʽ */
    VOS_UINT32                                              ulUserDefNoFluxCnt;         /* �û������������ʱ��Ҫ�� */
    NAS_RABM_FASTDORM_STATUS_ENUM_UINT32                    enCurrFastDormStatus;       /* �洢��ǰ��FAST DORMANCY����״̬ */

    VOS_UINT32                                              ulUlDataCnt;                /* �������ݼ����� */
    VOS_UINT32                                              ulDlDataCnt;                /* �������ݼ����� */

} NAS_RABM_FASTDORM_CTX_STRU;



typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;/* ��Ϣͷ                   */ /*_H2ASN_Skip*/
    NAS_RABM_FASTDORM_CTX_STRU          stFastDormCtx;
}NAS_RABM_LOG_FASTDORM_INFO_STRU;


/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/

/*****************************************************************************
 �� �� ��  : NAS_RABM_RegProcFuncTbl
 ��������  : ע��ָ��ϵͳģʽ�µ�RABM���������
 �������  : enSysMode      - ϵͳģʽ
             pstProcFucTbl  - ���������
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :
*****************************************************************************/
VOS_VOID NAS_RABM_RegProcFuncTbl(
    GMM_RABM_NET_RAT_ENUM_UINT32        enSysMode,
    NAS_RABM_PROC_FUNC_TBL_STRU        *pstProcFucTbl
);


/*****************************************************************************
 �� �� ��  : NAS_RABM_CtxInit
 ��������  : RABM��ʼ��
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :
*****************************************************************************/
VOS_VOID NAS_RABM_CtxInit(VOS_VOID);


/*****************************************************************************
 �� �� ��  : NAS_RABM_MsgProc
 ��������  : ����RABM��Ϣ, ������Ϣ���͵�����Ӧ�Ĵ�����
 �������  : struct MsgCB                       *pMsg
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :
*****************************************************************************/
VOS_VOID NAS_RABM_MsgProc(
    struct MsgCB                       *pMsg
);


/*****************************************************************************
 �� �� ��  : NAS_RABM_RcvSmMsg
 ��������  : ����SM��Ϣ, ������Ϣ���͵��ö�Ӧ������
 �������  : struct MsgCB                       *pMsg
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :
*****************************************************************************/
VOS_VOID NAS_RABM_RcvSmMsg(
    struct MsgCB                       *pMsg
);


/*****************************************************************************
 �� �� ��  : NAS_RABM_ProcessGmmMsg
 ��������  : ����GMM��Ϣ, ������Ϣ���͵��ö�Ӧ������
 �������  : struct MsgCB                       *pMsg
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :
*****************************************************************************/
VOS_VOID NAS_RABM_RcvGmmMsg(
    struct MsgCB                       *pMsg
);


/*****************************************************************************
 �� �� ��  : NAS_RABM_RcvSmActivateInd
 ��������  : ����ID_SM_RABM_ACTIVATE_INDԭ��
 �������  : pstSmActivateInd - ID_SM_RABM_ACTIVATE_IND��Ϣ����
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :
*****************************************************************************/
VOS_VOID NAS_RABM_RcvSmActivateInd(
    RABMSM_ACTIVATE_IND_STRU           *pstSmActivateInd
);


/*****************************************************************************
 �� �� ��  : NAS_RABM_RcvSmDeactivateInd
 ��������  : ����RABMSM_DEACTIVATE_INDԭ��
 �������  : pstSmDeactivateInd - RABMSM_DEACTIVATE_IND��Ϣ����
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :
*****************************************************************************/
VOS_VOID NAS_RABM_RcvSmDeactivateInd(
    RABMSM_DEACTIVATE_IND_STRU         *pstSmDeactivateInd
);


/*****************************************************************************
 �� �� ��  : NAS_RABM_RcvSmModifyInd
 ��������  : ����RABMSM_MODIFY_INDԭ��
 �������  : pstSmModifyInd - RABMSM_MODIFY_IND��Ϣ����
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :
*****************************************************************************/
VOS_VOID NAS_RABM_RcvSmModifyInd(
    RABMSM_MODIFY_IND_STRU             *pstSmModifyInd
);

#if (FEATURE_ON == FEATURE_LTE)

/*****************************************************************************
 �� �� ��  : NAS_RABM_RcvSmBearerActivateInd
 ��������  : ����ID_SM_RABM_BEARER_MODIFY_INDԭ��
 �������  : pstSmBearerActivateInd - ID_SM_RABM_BEARER_ACTIVATE_IND��Ϣ����
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :
*****************************************************************************/
VOS_VOID NAS_RABM_RcvSmBearerActivateInd(
    SM_RABM_BEARER_ACTIVATE_IND_STRU   *pstSmBearerActivateInd
);


/*****************************************************************************
 �� �� ��  : NAS_RABM_RcvSmBearerModifyInd
 ��������  : ����ID_SM_RABM_BEARER_DEACTIVATE_INDԭ��
 �������  : pstSmBearerModifyInd - ID_SM_RABM_BEARER_MODIFY_IND��Ϣ����
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :
*****************************************************************************/
VOS_VOID NAS_RABM_RcvSmBearerModifyInd(
    SM_RABM_BEARER_MODIFY_IND_STRU     *pstSmBearerModifyInd
);


/*****************************************************************************
 �� �� ��  : NAS_RABM_RcvSmBearerDeactivateInd
 ��������  : ����RABMSM_PDP_DEACTIVATE_INDԭ��
 �������  : pstSmBearerDeactivateInd - ID_SM_RABM_BEARER_DEACTIVATE_IND��Ϣ����
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :
*****************************************************************************/
VOS_VOID NAS_RABM_RcvSmBearerDeactivateInd(
    SM_RABM_BEARER_DEACTIVATE_IND_STRU *pstSmBearerDeactivateInd
);

#endif

/*****************************************************************************
 �� �� ��  : NAS_RABM_RcvGmmReestablishCnf
 ��������  : ����ID_GMM_RABM_REESTABLISH_CNFԭ��
 �������  : pstGmmReestablishCnf   - ID_GMM_RABM_REESTABLISH_CNF��Ϣ����
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :
*****************************************************************************/
VOS_VOID NAS_RABM_RcvGmmReestablishCnf(
    GMMRABM_REESTABLISH_CNF_STRU       *pstGmmReestablishCnf
);


/*****************************************************************************
 �� �� ��  : NAS_RABM_ProcSysSrvChangeInd
 ��������  : ����ϵͳ״̬�ı�ָʾ��Ϣ, ����ϵͳģʽ�ͷ���״̬
 �������  : pstGmmSysSrvChgInd - ID_GMM_RABM_SYS_SRV_CHG_IND��Ϣ����
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :
*****************************************************************************/
VOS_VOID NAS_RABM_RcvGmmSysSrvChgInd(
    GMM_RABM_SYS_SRV_CHG_IND_STRU      *pstGmmSysSrvChgInd
);


/*****************************************************************************
 �� �� ��  : NAS_RABM_RcvDataSuspendInd
 ��������  : �������ָʾ
 �������  : bRatChangeFlg - ��ϵͳ�л���־
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :
*****************************************************************************/
VOS_VOID NAS_RABM_RcvDataSuspendInd(
    VOS_BOOL                            bRatChangeFlg
);

/*****************************************************************************
 �� �� ��  : NAS_RABM_RcvDataResumeInd
 ��������  : ����ָ�ָʾ
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :
 �޸���ʷ  :
*****************************************************************************/
VOS_VOID NAS_RABM_RcvDataResumeInd(
    VOS_UINT8                           ucRebuildRabFlag
);


/*****************************************************************************
 �� �� ��  : NAS_RABM_RcvSysModeChgInd
 ��������  : ����ϵͳģʽ�ı�
 �������  : enNewSysMode - �µ�ϵͳģʽ
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :
*****************************************************************************/
VOS_VOID NAS_RABM_RcvSysModeChgInd(
    GMM_RABM_NET_RAT_ENUM_UINT32        enNewSysMode
);


/*****************************************************************************
 �� �� ��  : NAS_RABM_ProcessCommMsg
 ��������  :
 �������  : struct MsgCB                       *pMsg
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :
*****************************************************************************/
VOS_VOID NAS_RABM_RcvCommMsg(
    struct MsgCB                       *pMsg
);

extern VOS_UINT32    NAS_RABM_IsPdpActiveInWCDMA(VOS_VOID);
extern VOS_VOID NAS_RABM_ConvertAtFasTDormTypeToWrrFasTDormType(
    AT_RABM_FASTDORM_OPERATION_ENUM_UINT32                enAtFastDormType,
    RRC_FASTDORM_START_TYPE_ENUM_UINT32                 *penWrrFastDormType
);
extern VOS_VOID NAS_RABM_RcvSetFastDormParaReq(
    AT_RABM_SET_FASTDORM_PARA_REQ_STRU *pstFastDormPara
);
extern VOS_VOID NAS_RABM_RcvGetFastDormParaReq(
    AT_RABM_QRY_FASTDORM_PARA_REQ_STRU *pstFastDormPara
);

extern VOS_VOID NAS_RABM_RcvAtMsg(
    struct MsgCB                       *pMsg
);
extern VOS_VOID NAS_RABM_SuspendFastDorm(VOS_VOID);
extern VOS_VOID NAS_RABM_ResumeFastDorm(VOS_VOID);

extern VOS_VOID NAS_RABM_SysModeChgProcFastDorm(
    GMM_RABM_NET_RAT_ENUM_UINT32        enOldSysMode,
    GMM_RABM_NET_RAT_ENUM_UINT32        enNewSysMode
);
extern VOS_VOID NAS_RABM_RcvAtFastDormStartReq(
    AT_RABM_SET_FASTDORM_PARA_REQ_STRU *pstFastDormPara
);
extern VOS_VOID NAS_RABM_RcvAtFastDormStopReq(
    AT_RABM_SET_FASTDORM_PARA_REQ_STRU *pstFastDormPara
);
extern VOS_VOID NAS_RABM_SetFastDormUserDefNoFluxCntValue(
    VOS_UINT32                          ulNoFluxCnt
);
extern VOS_UINT32 NAS_RABM_GetFastDormUserDefNoFluxCntValue(VOS_VOID);
extern VOS_VOID NAS_RABM_SetFastDormCurrNoFluxCntValue(
    VOS_UINT32                          ulNoFluxCnt
);
extern VOS_UINT32 NAS_RABM_GetFastDormCurrNoFluxCntValue(VOS_VOID);
extern VOS_VOID NAS_RABM_SetFastDormOperationType(
    AT_RABM_FASTDORM_OPERATION_ENUM_UINT32 enFastDormOperationType
);
extern AT_RABM_FASTDORM_OPERATION_ENUM_UINT32 NAS_RABM_GetFastDormOperationType(VOS_VOID);
extern NAS_RABM_FASTDORM_STATUS_ENUM_UINT32 NAS_RABM_GetCurrFastDormStatus(VOS_VOID);
extern VOS_VOID NAS_RABM_RcvWasFastDormInfoInd(
    struct MsgCB                       *pMsg
);
extern VOS_VOID NAS_RABM_SetCurrFastDormStatus(
    NAS_RABM_FASTDORM_STATUS_ENUM_UINT32    enCurrFastDormStatus
);
extern VOS_VOID NAS_RABM_InitFastDormCtx(VOS_VOID);



VOS_VOID NAS_RABM_IncFastDormUlDataCnt(VOS_VOID);

VOS_VOID NAS_RABM_IncFastDormDlDataCnt(VOS_VOID);

VOS_UINT32 NAS_RABM_GetFastDormUlDataCnt(VOS_VOID);

VOS_UINT32 NAS_RABM_GetFastDormDlDataCnt(VOS_VOID);

VOS_VOID NAS_RABM_ClrFastDormUlDataCnt(VOS_VOID);

VOS_VOID NAS_RABM_ClrFastDormDlDataCnt(VOS_VOID);

VOS_VOID NAS_RABM_RcvCdsMsg(
    struct MsgCB                       *pMsg
);

VOS_VOID NAS_RABM_RcvCdsServiceInd(
    CDS_RABM_SERVICE_IND_STRU          *pstCdsServiceInd
);



VOS_UINT32 NAS_RABM_IsDataServiceRequestPending(VOS_VOID);
VOS_UINT32 NAS_RABM_IsRabReestablishPending(VOS_VOID);
VOS_VOID NAS_RABM_MNTN_TracePktLostEvt(
    NAS_RABM_PKT_FLOW_DIR_ENUM_UINT8    enPtkFlowDir,
    VOS_UINT8                           ucPoint
);

VOS_VOID NAS_RABM_RcvReleaseRrcReq(
    AT_RABM_RELEASE_RRC_REQ_STRU       *pstMsg
);

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

