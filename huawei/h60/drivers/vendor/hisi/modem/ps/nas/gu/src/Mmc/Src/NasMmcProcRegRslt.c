
/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "pslog.h"
#include "om.h"
#include "PsTypeDef.h"
#include "NasMmlCtx.h"
#include "NasMmlLib.h"
#include "NasMmcSndGuAs.h"
#include "NasMmcSndGmm.h"
#include "NasMmcSndMm.h"
#include "NasMmcSndLmm.h"
#include "NasMmSublayerDef.h"
#if (FEATURE_ON == FEATURE_LTE)
#include "MmcLmmInterface.h"
#endif
#include "NasMmcSndMma.h"
#include "NasMmcSndLmm.h"
#include "NasMmcSndGuAs.h"
#include "NasMmcCtx.h"
#include "NVIM_Interface.h"
#include "NasMmcProcUsim.h"
#include "NasMmcProcNvim.h"
#include "NasMmcProcRegRslt.h"
/* ɾ��ExtAppMmcInterface.h*/
#include "NasMmcComFunc.h"
#include "NasMmcSndOm.h"
#include "NVIM_Interface.h"

#include "NasUsimmApi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_NAS_MMC_PROC_REG_RSLT_C

/*****************************************************************************
  2 ȫ�ֱ���
*****************************************************************************/

/* ��NasMmcProcRegRslt.c�ļ��д���ע�������У���Ҫ��ͨ��CAUSE�����д���
   Ȼ�������ݴ���Ϣ������ע�����ǲ��걸�ģ�Ʃ����Ҫ֪��Attemp Counter��Ϣ��
   ���ڽ�����ע��������Ϣ�����ڱ��ص�ȫ�ֱ������棬�����ڴ���ע����ʱ��
   �ɻ�����е�ע������Ϣ�������������������õ��˼�������ţ�ע����������
   ����ļܹ��õ��ȶ�  */
NAS_MMC_REG_RSLT_CTX_STRU               gstRegRsltCtx;


/* CSע�ᱻ��ԭ��ֵ��Э�鴦��,�������Ϊ:Э������ȷ�涨��ԭ��ֵ���Լ���Щ�涨��ԭ��ֵ
   ��ӳ������Ķ�Ӧԭ��ֵ�����ڱ��н�����ʽ�Ĵ���; ���򽫽����쳣��֧���д��� */
NAS_MMC_REG_FAIL_PROC_ENTITY             gastMmcProcCsRegFailCause[] =
{
    {NAS_MML_REG_FAIL_CAUSE_IMSI_UNKNOWN_IN_HLR,                      NAS_MMC_ProcCsRegFailCause2ImsiUnknownInHlr},
    {NAS_MML_REG_FAIL_CAUSE_ILLEGAL_MS,                               NAS_MMC_ProcCsRegFailCause3IllegalMs},
    {NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME,                               NAS_MMC_ProcCsRegFailCause6IllegalMe},
    {NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW,                           NAS_MMC_ProcCsRegFailCause11PlmnNotAllow},
    {NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW,                             NAS_MMC_ProcCsRegFailCause12LaNotAllow},
    {NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW,                           NAS_MMC_ProcCsRegFailCause13RoamNotAllow},
    {NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL,                          NAS_MMC_ProcCsRegFailCause15NoSuitableCell},

    {NAS_MML_REG_FAIL_CAUSE_NETWORK_FAILURE,                          NAS_MMC_ProcCsRegFailCause17NetWorkFailure},

    /* Upon reception of the cause codes # 95, # 96, # 97, # 99 and # 111 the MS should set the attempt counter to 4.*/
    {NAS_MML_REG_FAIL_CAUSE_SEMANTICALLY_INCORRECT_MSG,               NAS_MMC_ProcCsRegFailCause95SemanticallyIncorrectMsg},
    {NAS_MML_REG_FAIL_CAUSE_INVALID_MANDATORY_INF,                    NAS_MMC_ProcCsRegFailCause96InvalidMandatoryInf},
    {NAS_MML_REG_FAIL_CAUSE_MSG_NONEXIST_NOTIMPLEMENTE,               NAS_MMC_ProcCsRegFailCause97MsgNonExistNotImplement},
    {NAS_MML_REG_FAIL_CAUSE_IE_NONEXIST_NOTIMPLEMENTED,               NAS_MMC_ProcCsRegFailCause99IeNonExistNotImplement},
    {NAS_MML_REG_FAIL_CAUSE_PROTOCOL_ERROR,                           NAS_MMC_ProcCsRegFailCause111ProtocolError},

    {NAS_MML_REG_FAIL_CAUSE_AUTH_REJ,                                 NAS_MMC_ProcCsRegFailCause256AuthRej},
    {NAS_MML_REG_FAIL_CAUSE_ACCESS_BARRED,                            NAS_MMC_ProcCsRegFailCause257AccessBarrd},
    {NAS_MML_REG_FAIL_CAUSE_TIMER_TIMEOUT,                            NAS_MMC_ProcCsRegFailCause301TimerTimeOut},
    {NAS_MML_REG_FAIL_CAUSE_RR_CONN_EST_FAIL,                         NAS_MMC_ProcCsRegFailCause302RrConnEstFail},
    {NAS_MML_REG_FAIL_CAUSE_RR_CONN_FAIL,                             NAS_MMC_ProcCsRegFailCause303RrConnFail},

    {NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE,                              NAS_MMC_ProcCsRegFailCause401OtherCause},
    {NAS_MML_REG_FAIL_CAUSE_MS_CFG_DOMAIN_NOT_SUPPORT,                NAS_MMC_ProcCsRegFailCause402MsCfgDomainNotSupport},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN,                           NAS_MMC_ProcCsRegFailCause403ForbiddenPlmn},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_REG_PRVS_SVS,            NAS_MMC_ProcCsRegFailCause404ForbiddenLaForRegPrvsSvs},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_ROAM,                    NAS_MMC_ProcCsRegFailCause405ForbiddenLaForRoam},
    {NAS_MML_REG_FAIL_CAUSE_SIM_INVALID,                              NAS_MMC_ProcCsRegFailCause407SimInvalid},
    {NAS_MML_REG_FAIL_CAUSE_NTDTH_IMSI,                               NAS_MMC_ProcCsRegFailCause408NetDetachImsi},
    {NAS_MML_REG_FAIL_CAUSE_SERVING_CELL_DOMAIN_NOT_SUPPORT,          NAS_MMC_ProcCsRegFailCause409ServingCellDomainNotSupport},

    {NAS_MML_REG_FAIL_CAUSE_DELAY_CS_IN_TRANS_LAU,                    NAS_MMC_ProcCsRegFailCause416DealyCsInTransLau},

    {NAS_MML_REG_FAIL_CAUSE_T3212_RUNNING,                            NAS_MMC_ProcCsRegFailCause417T3212Running},
};

/* PSע�ᱻ��ԭ��ֵ��Э�鴦��,�������Ϊ:Э������ȷ�涨��ԭ��ֵ���Լ���Щ�涨��ԭ��ֵ
   ��ӳ������Ķ�Ӧԭ��ֵ�����ڱ��н�����ʽ�Ĵ���; ���򽫽����쳣��֧���д��� */
NAS_MMC_REG_FAIL_PROC_ENTITY             gastMmcProcPsRegFailCause[] =
{
    {NAS_MML_REG_FAIL_CAUSE_ILLEGAL_MS,                               NAS_MMC_ProcPsRegFailCause3IllegalMs},
    {NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME,                               NAS_MMC_ProcPsRegFailCause6IllegalMe},
    {NAS_MML_REG_FAIL_CAUSE_GPRS_SERV_NOT_ALLOW,                      NAS_MMC_ProcPsRegFailCause7GprsServNotAllow},
    {NAS_MML_REG_FAIL_CAUSE_GPRS_SERV_AND_NON_GPRS_SERV_NOT_ALLOW,    NAS_MMC_ProcPsRegFailCause8GprsAndNonGprsNotAllow},
    {NAS_MML_REG_FAIL_CAUSE_MS_ID_NOT_DERIVED,                        NAS_MMC_ProcPsRegFailCause9MsIdNotDerived},
    {NAS_MML_REG_FAIL_CAUSE_IMPLICIT_DETACHED,                        NAS_MMC_ProcPsRegFailCause10ImplicitDetached},
    {NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW,                           NAS_MMC_ProcPsRegFailCause11PlmnNotAllow},
    {NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW,                             NAS_MMC_ProcPsRegFailCause12LaNotAllow},
    {NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW,                           NAS_MMC_ProcPsRegFailCause13RoamNotAllow},
    {NAS_MML_REG_FAIL_CAUSE_GPRS_SERV_NOT_ALLOW_IN_PLMN,              NAS_MMC_ProcPsRegFailCause14GprsNotAllowInThisPlmn},
    {NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL,                          NAS_MMC_ProcPsRegFailCause15NoSuitableCell},
    {NAS_MML_REG_FAIL_CAUSE_NO_PDP_CONTEXT_ACT,                       NAS_MMC_ProcPsRegFailCause40NoPdpContextActived},

    /* Upon reception of the cause codes # 95, # 96, # 97, # 99 and # 111 the MS should set the GPRS attach attempt counter to 5.*/
    {NAS_MML_REG_FAIL_CAUSE_SEMANTICALLY_INCORRECT_MSG,               NAS_MMC_ProcPsRegFailCause95SemanticallyIncorrectMsg},
    {NAS_MML_REG_FAIL_CAUSE_INVALID_MANDATORY_INF,                    NAS_MMC_ProcPsRegFailCause96InvalidMandatoryInf},
    {NAS_MML_REG_FAIL_CAUSE_MSG_NONEXIST_NOTIMPLEMENTE,               NAS_MMC_ProcPsRegFailCause97MsgNonExistNotImplement},
    {NAS_MML_REG_FAIL_CAUSE_IE_NONEXIST_NOTIMPLEMENTED,               NAS_MMC_ProcPsRegFailCause99IeNonExistNotImplement},
    {NAS_MML_REG_FAIL_CAUSE_PROTOCOL_ERROR,                           NAS_MMC_ProcPsRegFailCause111ProtocolError},

    {NAS_MML_REG_FAIL_CAUSE_AUTH_REJ,                                 NAS_MMC_ProcPsRegFailCause256AuthRej},
    {NAS_MML_REG_FAIL_CAUSE_ACCESS_BARRED,                            NAS_MMC_ProcPsRegFailCause257AccessBarrd},
    {NAS_MML_REG_FAIL_CAUSE_TIMER_TIMEOUT,                            NAS_MMC_ProcPsRegFailCause301TimerTimeOut},
    {NAS_MML_REG_FAIL_CAUSE_RR_CONN_EST_FAIL,                         NAS_MMC_ProcPsRegFailCause302RrConnEstFail},
    {NAS_MML_REG_FAIL_CAUSE_RR_CONN_FAIL,                             NAS_MMC_ProcPsRegFailCause303RrConnFail},

    {NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE,                              NAS_MMC_ProcPsRegFailCause401OtherCause},
    {NAS_MML_REG_FAIL_CAUSE_MS_CFG_DOMAIN_NOT_SUPPORT,                NAS_MMC_ProcPsRegFailCause402MsCfgDomainNotSupport},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN,                           NAS_MMC_ProcPsRegFailCause403ForbiddenPlmn},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_REG_PRVS_SVS,            NAS_MMC_ProcPsRegFailCause404ForbiddenLaForRegPrvsSvs},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_ROAM,                    NAS_MMC_ProcPsRegFailCause405ForbiddenLaForRoam},

    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN_FOR_GPRS,                  NAS_MMC_ProcPsRegFailCause406ForbiddenPlmnForGprs},
    {NAS_MML_REG_FAIL_CAUSE_SIM_INVALID,                              NAS_MMC_ProcPsRegFailCause407SimInvalid},
    {NAS_MML_REG_FAIL_CAUSE_NTDTH_IMSI,                               NAS_MMC_ProcPsRegFailCause408NetDetachImsi},
    {NAS_MML_REG_FAIL_CAUSE_SERVING_CELL_DOMAIN_NOT_SUPPORT,          NAS_MMC_ProcPsRegFailCause409ServingCellDomainNotSupport},
};
#if   (FEATURE_ON == FEATURE_LTE)

/* EPSע�ᱻ��ԭ��ֵ��Э�鴦��,�������Ϊ:Э������ȷ�涨��ԭ��ֵ���Լ���Щ�涨��ԭ��ֵ
   ��ӳ������Ķ�Ӧԭ��ֵ�����ڱ��н�����ʽ�Ĵ���; ���򽫽����쳣��֧���д��� */

/* Eps ����attach�����򱻾�ԭ��ֵ��Э�鴦�� */
NAS_MMC_COMBINED_EPS_REG_REJ_PROC_ENTITY             gastMmcProcCombinedAttachEpsRegFailCause[] =
{
    {NAS_EMM_CAUSE_ILLEGAL_UE,                                        NAS_MMC_ProcCombinedEpsRegFailCause3IllegalMs},
    {NAS_EMM_CAUSE_ILLEGAL_ME,                                        NAS_MMC_ProcCombinedEpsRegFailCause6IllegalMe},
    {NAS_EMM_CAUSE_EPS_SERV_NOT_ALLOW,                                NAS_MMC_ProcCombinedEpsRegFailCause7EpsServNotAllow},
    {NAS_EMM_CAUSE_EPS_SERV_AND_NON_EPS_SERV_NOT_ALLOW,               NAS_MMC_ProcCombinedEpsRegFailCause8GprsAndNonGprsNotAllow},
    {NAS_EMM_CAUSE_PLMN_NOT_ALLOW,                                    NAS_MMC_ProcCombinedEpsRegFailCause11PlmnNotAllow},
    {NAS_EMM_CAUSE_TA_NOT_ALLOW,                                      NAS_MMC_ProcCombinedEpsRegFailCause12TaNotAllow},
    {NAS_EMM_CAUSE_ROAM_NOT_ALLOW,                                    NAS_MMC_ProcCombinedEpsRegFailCause13RoamNotAllow},
    {NAS_EMM_CAUSE_EPS_SERV_NOT_ALLOW_IN_PLMN,                        NAS_MMC_ProcCombinedEpsRegFailCause14GprsNotAllowInThisPlmn},
    {NAS_EMM_CAUSE_NO_SUITABL_CELL,                                   NAS_MMC_ProcCombinedEpsRegFailCause15NoSuitableCell},

    {NAS_EMM_CAUSE_ESM_FAILURE,                                       NAS_MMC_ProcCombinedEpsRegFailCause19EsmFailure},
    {NAS_EMM_CAUSE_REQUESTED_SER_OPTION_NOT_AUTHORIZED_IN_PLMN,       NAS_MMC_ProcCombinedEpsRegFailCause35ReqServOptNotAuthorizedInThisPlmn},
    {NAS_MML_REG_FAIL_CAUSE_AUTH_REJ,                                 NAS_MMC_ProcCombinedEpsRegFailCause256AuthRej},
    {NAS_MML_REG_FAIL_CAUSE_ACCESS_BARRED,                            NAS_MMC_ProcCombinedEpsRegFailCause257AccessBarrd},
    {NAS_MML_REG_FAIL_CAUSE_TIMER_TIMEOUT,                            NAS_MMC_ProcCombinedEpsRegFailCause301TimerTimeOut},
    {NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE,                              NAS_MMC_ProcCombinedEpsRegFailCause401OtherCause},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN,                           NAS_MMC_ProcCombinedEpsRegFailCause403ForbiddenPlmn},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_REG_PRVS_SVS,            NAS_MMC_ProcCombinedEpsRegFailCause404ForbiddenTaForRegPrvsSvs},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_ROAM,                    NAS_MMC_ProcCombinedEpsRegFailCause405ForbiddenLaForRoam},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN_FOR_GPRS,                  NAS_MMC_ProcCombinedEpsRegFailCause406ForbiddenPlmnForGprs},
    {NAS_MML_REG_FAIL_CAUSE_PS_ATTACH_NOT_ALLOW,                      NAS_MMC_ProcCombinedEpsRegFailCausePsAttachNotAllow},
    {NAS_MML_REG_FAIL_CAUSE_ESM_BEAR_FAILURE,                         NAS_MMC_ProcCombinedEpsRegFailCauseEpsBearerEstFail},
    {NAS_MML_REG_FAIL_CAUSE_MO_DETACH_FAILURE,                        NAS_MMC_ProcCombinedEpsRegFailCauseMoDetach},
    {NAS_MML_REG_FAIL_CAUSE_MT_DETACH_FAILURE,                        NAS_MMC_ProcCombinedEpsRegFailCauseMtDetach},
    {NAS_MML_REG_FAIL_CAUSE_T3402_RUNNING,                            NAS_MMC_ProcCombinedEpsRegFailCauseT3402Running},
};

/* Eps ����tau�����򱻾�ԭ��ֵ��Э�鴦�� */
NAS_MMC_COMBINED_EPS_REG_REJ_PROC_ENTITY             gastMmcProcCombinedTauEpsRegFailCause[] =
{
    {NAS_EMM_CAUSE_ILLEGAL_UE,                                        NAS_MMC_ProcCombinedEpsRegFailCause3IllegalMs},
    {NAS_EMM_CAUSE_ILLEGAL_ME,                                        NAS_MMC_ProcCombinedEpsRegFailCause6IllegalMe},
    {NAS_EMM_CAUSE_EPS_SERV_NOT_ALLOW,                                NAS_MMC_ProcCombinedEpsRegFailCause7EpsServNotAllow},
    {NAS_EMM_CAUSE_EPS_SERV_AND_NON_EPS_SERV_NOT_ALLOW,               NAS_MMC_ProcCombinedEpsRegFailCause8GprsAndNonGprsNotAllow},
    {NAS_EMM_CAUSE_MS_ID_CANNOT_BE_DERIVED_BY_NW,                     NAS_MMC_ProcCombinedEpsRegFailCause9MsidCannotDerivedByNw},
    {NAS_EMM_CAUSE_EPS_IMPLICITLY_DETACHED,                           NAS_MMC_ProcCombinedEpsRegFailCause10ImplicitlyDetached},
    {NAS_EMM_CAUSE_PLMN_NOT_ALLOW,                                    NAS_MMC_ProcCombinedEpsRegFailCause11PlmnNotAllow},
    {NAS_EMM_CAUSE_TA_NOT_ALLOW,                                      NAS_MMC_ProcCombinedEpsRegFailCause12TaNotAllow},
    {NAS_EMM_CAUSE_ROAM_NOT_ALLOW,                                    NAS_MMC_ProcCombinedEpsRegFailCause13RoamNotAllow},
    {NAS_EMM_CAUSE_EPS_SERV_NOT_ALLOW_IN_PLMN,                        NAS_MMC_ProcCombinedEpsRegFailCause14GprsNotAllowInThisPlmn},
    {NAS_EMM_CAUSE_NO_SUITABL_CELL,                                   NAS_MMC_ProcCombinedEpsRegFailCause15NoSuitableCell},
    {NAS_EMM_CAUSE_REQUESTED_SER_OPTION_NOT_AUTHORIZED_IN_PLMN,       NAS_MMC_ProcCombinedEpsRegFailCause35ReqServOptNotAuthorizedInThisPlmn},

    {NAS_EMM_CAUSE_NO_EPS_BEARER_CONTEXT_ACTIVATED,                   NAS_MMC_ProcCombinedEpsRegFailCause40NoEpsBearerActived},
    {NAS_MML_REG_FAIL_CAUSE_AUTH_REJ,                                 NAS_MMC_ProcCombinedEpsRegFailCause256AuthRej},
    {NAS_MML_REG_FAIL_CAUSE_ACCESS_BARRED,                            NAS_MMC_ProcCombinedEpsRegFailCause257AccessBarrd},
    {NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE,                              NAS_MMC_ProcCombinedEpsRegFailCause401OtherCause},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN,                           NAS_MMC_ProcCombinedEpsRegFailCause403ForbiddenPlmn},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_REG_PRVS_SVS,            NAS_MMC_ProcCombinedEpsRegFailCause404ForbiddenTaForRegPrvsSvs},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_ROAM,                    NAS_MMC_ProcCombinedEpsRegFailCause405ForbiddenLaForRoam},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN_FOR_GPRS,                  NAS_MMC_ProcCombinedEpsRegFailCause406ForbiddenPlmnForGprs},

    {NAS_MML_REG_FAIL_CAUSE_ESM_BEAR_FAILURE,                         NAS_MMC_ProcCombinedEpsRegFailCauseEpsBearerEstFail},
    {NAS_MML_REG_FAIL_CAUSE_MO_DETACH_FAILURE,                        NAS_MMC_ProcCombinedEpsRegFailCauseMoDetach},
    {NAS_MML_REG_FAIL_CAUSE_MT_DETACH_FAILURE,                        NAS_MMC_ProcCombinedEpsRegFailCauseMtDetach},
    {NAS_MML_REG_FAIL_CAUSE_T3402_RUNNING,                            NAS_MMC_ProcCombinedEpsRegFailCauseT3402Running},
};


/* Eps ����ע��Ps��ɹ���CS�򱻾�ԭ��ֵ��Э�鴦�� */
NAS_MMC_REG_FAIL_PROC_ENTITY                        gastMmcProcCombinedEpsRegOnlyEpsSuccCause[] =
{
    {NAS_EMM_CAUSE_IMSI_UNKNOWN_IN_HSS,                               NAS_MMC_ProcCombinedEpsRegOnlyEpsSuccCause2ImsiUnknownInHss},
    {NAS_MML_REG_FAIL_CAUSE_MSC_UNREACHABLE,                          NAS_MMC_ProcCombinedEpsRegOnlyEpsSuccCause16MscNotReachable},
    {NAS_EMM_CAUSE_NETWORKFAILURE,                                    NAS_MMC_ProcCombinedEpsRegOnlyEpsSuccCause17NetworkFailure},
    {NAS_EMM_CAUSE_CS_DOMAIN_NOT_AVAILABLE,                           NAS_MMC_ProcCombinedEpsRegOnlyEpsSuccCause18CsDomainNotAvailable},
    {NAS_EMM_CAUSE_CONGESTION,                                        NAS_MMC_ProcCombinedEpsRegOnlyEpsSuccCause22Congestion},
};


/* Eps ע�ᱻ��ԭ��ֵ��Э�鴦�� */
NAS_MMC_REG_FAIL_PROC_ENTITY             gastMmcProcEpsAttachRegFailCause[] =
{
    {NAS_EMM_CAUSE_ILLEGAL_UE,                                        NAS_MMC_ProcEpsAttachRegFailCause3IllegalMs},
    {NAS_EMM_CAUSE_ILLEGAL_ME,                                        NAS_MMC_ProcEpsAttachRegFailCause6IllegalMe},
    {NAS_EMM_CAUSE_EPS_SERV_NOT_ALLOW,                                NAS_MMC_ProcEpsAttachRegFailCause7EpsServNotAllow},
    {NAS_EMM_CAUSE_EPS_SERV_AND_NON_EPS_SERV_NOT_ALLOW,               NAS_MMC_ProcEpsAttachRegFailCause8GprsAndNonGprsNotAllow},
    {NAS_EMM_CAUSE_PLMN_NOT_ALLOW,                                    NAS_MMC_ProcEpsAttachRegFailCause11PlmnNotAllow},
    {NAS_EMM_CAUSE_TA_NOT_ALLOW,                                      NAS_MMC_ProcEpsAttachRegFailCause12TaNotAllow},
    {NAS_EMM_CAUSE_ROAM_NOT_ALLOW,                                    NAS_MMC_ProcEpsAttachRegFailCause13RoamNotAllow},
    {NAS_EMM_CAUSE_EPS_SERV_NOT_ALLOW_IN_PLMN,                        NAS_MMC_ProcEpsAttachRegFailCause14GprsNotAllowInThisPlmn},
    {NAS_EMM_CAUSE_NO_SUITABL_CELL,                                   NAS_MMC_ProcEpsAttachRegFailCause15NoSuitableCell},
    {NAS_EMM_CAUSE_REQUESTED_SER_OPTION_NOT_AUTHORIZED_IN_PLMN,       NAS_MMC_ProcEpsAttachRegFailCause35ReqServOptNotAuthorizedInThisPlmn},

    {NAS_MML_REG_FAIL_CAUSE_AUTH_REJ,                                 NAS_MMC_ProcEpsAttachRegFailCause256AuthRej},
    {NAS_MML_REG_FAIL_CAUSE_ACCESS_BARRED,                            NAS_MMC_ProcEpsAttachRegFailCause257AccessBarrd},
    {NAS_MML_REG_FAIL_CAUSE_TIMER_TIMEOUT,                            NAS_MMC_ProcEpsAttachRegFailCause301TimerTimeOut},
    {NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE,                              NAS_MMC_ProcEpsAttachRegFailCause401OtherCause},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN,                           NAS_MMC_ProcEpsAttachRegFailCause403ForbiddenPlmn},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_REG_PRVS_SVS,            NAS_MMC_ProcEpsAttachRegFailCause404ForbiddenTaForRegPrvsSvs},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_ROAM,                    NAS_MMC_ProcEpsAttachRegFailCause405ForbiddenLaForRoam},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN_FOR_GPRS,                  NAS_MMC_ProcEpsAttachRegFailCause406ForbiddenPlmnForGprs},
    {NAS_MML_REG_FAIL_CAUSE_PS_ATTACH_NOT_ALLOW,                      NAS_MMC_ProcEpsAttachRegFailCausePsAttachNotAllow},
    {NAS_MML_REG_FAIL_CAUSE_ESM_BEAR_FAILURE,                         NAS_MMC_ProcEpsAttachRegFailCauseEpsBearerEstFail},
    {NAS_MML_REG_FAIL_CAUSE_MO_DETACH_FAILURE,                        NAS_MMC_ProcEpsAttachRegFailCauseMoDetach},
    {NAS_MML_REG_FAIL_CAUSE_MT_DETACH_FAILURE,                        NAS_MMC_ProcEpsAttachRegFailCauseMtDetach},
    {NAS_MML_REG_FAIL_CAUSE_T3402_RUNNING,                            NAS_MMC_ProcEpsAttachRegFailCauseT3402Running},
};




/* Eps TAU����ԭ��ֵ��Э�鴦�� */
NAS_MMC_REG_FAIL_PROC_ENTITY             gastMmcProcEpsTauRegFailCause[] =
{
    {NAS_EMM_CAUSE_ILLEGAL_UE,                                        NAS_MMC_ProcEpsTauRegFailCause3IllegalMs},
    {NAS_EMM_CAUSE_ILLEGAL_ME,                                        NAS_MMC_ProcEpsTauRegFailCause6IllegalMe},
    {NAS_EMM_CAUSE_EPS_SERV_NOT_ALLOW,                                NAS_MMC_ProcEpsTauRegFailCause7EpsServNotAllow},

    {NAS_EMM_CAUSE_MS_ID_CANNOT_BE_DERIVED_BY_NW,                     NAS_MMC_ProcEpsTauRegFailCause9MsIdCannotBeDerivedByNW},
    {NAS_EMM_CAUSE_EPS_IMPLICITLY_DETACHED,                           NAS_MMC_ProcEpsTauRegFailCause10EpsImplicitlyDetached},

    {NAS_EMM_CAUSE_PLMN_NOT_ALLOW,                                    NAS_MMC_ProcEpsTauRegFailCause11PlmnNotAllow},
    {NAS_EMM_CAUSE_TA_NOT_ALLOW,                                      NAS_MMC_ProcEpsTauRegFailCause12TaNotAllow},
    {NAS_EMM_CAUSE_ROAM_NOT_ALLOW,                                    NAS_MMC_ProcEpsTauRegFailCause13RoamNotAllow},
    {NAS_EMM_CAUSE_EPS_SERV_NOT_ALLOW_IN_PLMN,                        NAS_MMC_ProcEpsTauRegFailCause14GprsNotAllowInThisPlmn},
    {NAS_EMM_CAUSE_NO_SUITABL_CELL,                                   NAS_MMC_ProcEpsTauRegFailCause15NoSuitableCell},
    {NAS_EMM_CAUSE_NO_EPS_BEARER_CONTEXT_ACTIVATED,                   NAS_MMC_ProcEpsTauRegFailCause40NoEpsBearerActived},
    {NAS_EMM_CAUSE_REQUESTED_SER_OPTION_NOT_AUTHORIZED_IN_PLMN,       NAS_MMC_ProcEpsTauRegFailCause35ReqServOptNotAuthorizedInThisPlmn},

    {NAS_MML_REG_FAIL_CAUSE_AUTH_REJ,                                 NAS_MMC_ProcEpsTauRegFailCause256AuthRej},
    {NAS_MML_REG_FAIL_CAUSE_ACCESS_BARRED,                            NAS_MMC_ProcEpsTauRegFailCause257AccessBarrd},
    {NAS_MML_REG_FAIL_CAUSE_TIMER_TIMEOUT,                            NAS_MMC_ProcEpsTauRegFailCause301TimerTimeOut},
    {NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE,                              NAS_MMC_ProcEpsTauRegFailCause401OtherCause},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN,                           NAS_MMC_ProcEpsTauRegFailCause403ForbiddenPlmn},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_REG_PRVS_SVS,            NAS_MMC_ProcEpsTauRegFailCause404ForbiddenTaForRegPrvsSvs},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_ROAM,                    NAS_MMC_ProcEpsTauRegFailCause405ForbiddenLaForRoam},
    {NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN_FOR_GPRS,                  NAS_MMC_ProcEpsTauRegFailCause406ForbiddenPlmnForGprs},
    {NAS_MML_REG_FAIL_CAUSE_MO_DETACH_FAILURE,                        NAS_MMC_ProcEpsTauRegFailCauseMoDetach},
    {NAS_MML_REG_FAIL_CAUSE_MT_DETACH_FAILURE,                        NAS_MMC_ProcEpsTauRegFailCauseMtDetach},
    {NAS_MML_REG_FAIL_CAUSE_T3402_RUNNING,                            NAS_MMC_ProcEpsTauRegFailCauseT3402Running},
};



/* Eps ����detach����ԭ��ֵ��Э�鴦�� */
NAS_MMC_EPS_DETACH_CAUSE_PROC_ENTITY                gastMmcProcEpsDetachCause[] =
{
    {NAS_EMM_CAUSE_IMSI_UNKNOWN_IN_HSS,                               NAS_MMC_ProcEpsDetachCause2ImsiUnknowInHss},
    {NAS_EMM_CAUSE_ILLEGAL_UE,                                        NAS_MMC_ProcEpsDetachCause3IllegalMs},
    {NAS_EMM_CAUSE_ILLEGAL_ME,                                        NAS_MMC_ProcEpsDetachCause6IllegalMe},
    {NAS_EMM_CAUSE_EPS_SERV_NOT_ALLOW,                                NAS_MMC_ProcEpsDetachCause7EpsServNotAllow},
    {NAS_EMM_CAUSE_EPS_SERV_AND_NON_EPS_SERV_NOT_ALLOW,               NAS_MMC_ProcEpsDetachCause8GprsAndNonGprsNotAllow},
    {NAS_EMM_CAUSE_PLMN_NOT_ALLOW,                                    NAS_MMC_ProcEpsDetachCause11PlmnNotAllow},
    {NAS_EMM_CAUSE_TA_NOT_ALLOW,                                      NAS_MMC_ProcEpsDetachCause12TaNotAllow},
    {NAS_EMM_CAUSE_ROAM_NOT_ALLOW,                                    NAS_MMC_ProcEpsDetachCause13RoamNotAllow},
    {NAS_EMM_CAUSE_EPS_SERV_NOT_ALLOW_IN_PLMN,                        NAS_MMC_ProcEpsDetachCause14GprsNotAllowInThisPlmn},
    {NAS_EMM_CAUSE_NO_SUITABL_CELL,                                   NAS_MMC_ProcEpsDetachCause15NoSuitableCell},
};

/* Eps SR����ԭ��ֵ��Э�鴦�� */
NAS_MMC_REG_FAIL_PROC_ENTITY            gastMmcProcEpsServiceRequestFailCause[] =
{
    {NAS_MML_REG_FAIL_CAUSE_ILLEGAL_MS,                                         NAS_MMC_ProcEpsServiceRequestFailCause3IllegalMs},
    {NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME,                                         NAS_MMC_ProcEpsServiceRequestFailCause6IllegalMe},
    {NAS_MML_REG_FAIL_CAUSE_GPRS_SERV_NOT_ALLOW,                                NAS_MMC_ProcEpsServiceRequestFailCause7EpsServNotAllow},

    {NAS_EMM_CAUSE_MS_ID_CANNOT_BE_DERIVED_BY_NW,                               NAS_MMC_ProcEpsServiceRequestFailCause9MsIdCannotBeDerivedByNW},
    {NAS_EMM_CAUSE_EPS_IMPLICITLY_DETACHED,                                     NAS_MMC_ProcEpsServiceRequestFailCause10EpsImplicitlyDetached},

    {NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW,                                     NAS_MMC_ProcEpsServiceRequestFailCause11PlmnNotAllow},
    {NAS_MML_REG_FAIL_CAUSE_LA_NOT_ALLOW,                                       NAS_MMC_ProcEpsServiceRequestFailCause12TaNotAllow},
    {NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW,                                     NAS_MMC_ProcEpsServiceRequestFailCause13RoamNotAllow},
    {NAS_MML_REG_FAIL_CAUSE_GPRS_SERV_NOT_ALLOW_IN_PLMN,                        NAS_MMC_ProcEpsServiceRequestFailCause14GprsNotAllowInThisPlmn},
    {NAS_MML_REG_FAIL_CAUSE_NO_SUITABL_CELL,                                    NAS_MMC_ProcEpsServiceRequestFailCause15NoSuitableCell},
    {NAS_EMM_CAUSE_REQUESTED_SER_OPTION_NOT_AUTHORIZED_IN_PLMN,                 NAS_MMC_ProcEpsServiceRequestFailCause35ReqServOptNotAuthorizedInThisPlmn},

    {NAS_EMM_CAUSE_CS_DOMAIN_NOT_AVAILABLE,                                     NAS_MMC_ProcEpsServiceRequestFailCause18CsDomainNotAvailable},
    {NAS_EMM_CAUSE_NOT_AUTHORIZED_FOR_THIS_CSG,                                 NAS_MMC_ProcEpsServiceRequestFailCause25NotAuthorizedForThisCsg},
    {NAS_EMM_CAUSE_CS_DOMAIN_TEMP_NOT_AVAILABLE,                                NAS_MMC_ProcEpsServiceRequestFailCause39CsDomainTempNotAvailable},

    {NAS_MML_REG_FAIL_CAUSE_NO_PDP_CONTEXT_ACT,                                 NAS_MMC_ProcEpsServiceRequestFailCause40NoEpsBearerActived},
    {NAS_MML_REG_FAIL_CAUSE_AUTH_REJ,                                           NAS_MMC_ProcEpsServiceRequestFailCause256AuthRej},
    {NAS_MML_REG_FAIL_CAUSE_ACCESS_BARRED,                                      NAS_MMC_ProcEpsServiceRequestFailCause257AccessBarrd},
};


#endif

NAS_MMC_ADDITIONAL_ACTION_PRIORITY_PAIR gastAdditionalActionPrioArr[] =
{
    {NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL,                           0},
    {NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION,                         1},
    {NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN,      2},
    {NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION,                         3},
    {NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT,                       4},
    {NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON,                         5},
    {NAS_MMC_ADDITIONAL_ACTION_ACCESS_BARRED,                          6},
    {NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION,                7},
    {NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON,                        8},
    {NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL,                  9},
    {NAS_MMC_ADDITIONAL_ACTION_BUTT,                                   10}
};

/*****************************************************************************
  3 �궨��
*****************************************************************************/

NAS_MMC_REG_RSLT_CTX_STRU* NAS_MMC_GetRegRsltCtxAddr(VOS_VOID)
{
    return &(gstRegRsltCtx);
}


VOS_VOID  NAS_MMC_SaveRegRsltCtx(
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8    enRegRsltType,
    VOS_VOID                           *pRegRsltIndMsg
)
{
    NAS_MMC_REG_RSLT_CTX_STRU          *pstRegRsltCtx = VOS_NULL_PTR;
    pstRegRsltCtx   = NAS_MMC_GetRegRsltCtxAddr();

    /* ���µ�ǰ��ע������Ϣ���� */
    pstRegRsltCtx->enRegRsltType = enRegRsltType;

    pstRegRsltCtx->ucIsLteRegTypeEpsOnly = VOS_FALSE;

    /* ������Ϣ������ */
    switch ( enRegRsltType )
    {
        case MM_MMC_CS_REG_RESULT_IND :
            pstRegRsltCtx->unRegRsltCtx.pstCsRegRsltInd = (MMMMC_CS_REG_RESULT_IND_STRU*)pRegRsltIndMsg;
            break;

        case GMM_MMC_PS_REG_RESULT_IND :
            pstRegRsltCtx->unRegRsltCtx.pstPsRegRsltInd = (GMMMMC_PS_REG_RESULT_IND_STRU*)pRegRsltIndMsg;
            break;

        case GMM_MMC_PS_SERVICE_RESULT_IND :
            pstRegRsltCtx->unRegRsltCtx.pstServiceRsltIndMsg = (GMMMMC_SERVICE_REQUEST_RESULT_IND_STRU*)pRegRsltIndMsg;
            break;

        case GMM_MMC_PS_NETWORK_DETACH_IND :
            pstRegRsltCtx->unRegRsltCtx.pstDetachIndMsg      = (MMCGMM_NETWORK_DETACH_IND_STRU*)pRegRsltIndMsg;
            break;

#if (FEATURE_ON == FEATURE_LTE)
        case LMM_MMC_ATTACH_IND :
            pstRegRsltCtx->unRegRsltCtx.pstLmmAttachIndMsg = (LMM_MMC_ATTACH_IND_STRU*)pRegRsltIndMsg;

            if (MMC_LMM_ATT_TYPE_EPS_ONLY == pstRegRsltCtx->unRegRsltCtx.pstLmmAttachIndMsg->ulReqType)
            {
                pstRegRsltCtx->ucIsLteRegTypeEpsOnly = VOS_TRUE;
            }

            NAS_MMC_SaveEpsAttachAttemptCounter((LMM_MMC_ATTACH_IND_STRU*)pRegRsltIndMsg);

            break;

        case LMM_MMC_TAU_RESULT_IND :
            pstRegRsltCtx->unRegRsltCtx.pstLmmTauIndMsg = (LMM_MMC_TAU_RESULT_IND_STRU*)pRegRsltIndMsg;

            if ((MMC_LMM_TA_UPDATING == pstRegRsltCtx->unRegRsltCtx.pstLmmTauIndMsg->ulReqType)
             || (MMC_LMM_PS_PERIODIC_UPDATING == pstRegRsltCtx->unRegRsltCtx.pstLmmTauIndMsg->ulReqType))
            {
                pstRegRsltCtx->ucIsLteRegTypeEpsOnly = VOS_TRUE;
            }

            NAS_MMC_SaveEpsTauAttemptCounter((LMM_MMC_TAU_RESULT_IND_STRU*)pRegRsltIndMsg);

            break;

        case LMM_MMC_DETACH_IND :
            pstRegRsltCtx->unRegRsltCtx.pstLmmDetachMsg = (LMM_MMC_DETACH_IND_STRU*)pRegRsltIndMsg;
            break;

        case LMM_MMC_SERVICE_RESULT_IND :
            pstRegRsltCtx->unRegRsltCtx.pstLmmServiceIndMsg = (LMM_MMC_SERVICE_RESULT_IND_STRU*)pRegRsltIndMsg;
            break;


#endif
        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SaveRegRsltCtx: UNEXPECTED MSG TYPE!");
            break;
    }

    return;
}



NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8 NAS_MMC_GetRegRsltType(VOS_VOID)
{
    return gstRegRsltCtx.enRegRsltType;
}





VOS_UINT32 NAS_MMC_GetRegRsltAttemptCounter(
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8    enRegRsltType
)
{
    NAS_MMC_REG_RSLT_CTX_STRU          *pstRegRsltCtx = VOS_NULL_PTR;
    VOS_UINT32                          ulAttempCounter;

    /* Attempt Counter ��ʼ��Ϊ0 */
    ulAttempCounter = 0;
    pstRegRsltCtx   = NAS_MMC_GetRegRsltCtxAddr();

    switch ( enRegRsltType )
    {
        case MM_MMC_CS_REG_RESULT_IND :

            /* ��ȡCSע������Ϣ�е�Attempt Counter��ֵ */
            ulAttempCounter = pstRegRsltCtx->unRegRsltCtx.pstCsRegRsltInd->ulLuAttemptCnt;
            break;

        case GMM_MMC_PS_REG_RESULT_IND :

            /* ��ȡPSע������Ϣ�е�Attempt Counter��ֵ */
            ulAttempCounter = pstRegRsltCtx->unRegRsltCtx.pstPsRegRsltInd->ulRegCounter;
            break;

#if (FEATURE_ON == FEATURE_LTE)
        case LMM_MMC_ATTACH_IND :

            /* ��ȡlmm ATTACHע������Ϣ�е�Attempt Counter��ֵ */
            ulAttempCounter = pstRegRsltCtx->ucEpsAttemptCounter;

            break;

        case LMM_MMC_TAU_RESULT_IND :

            /* ��ȡlmm TAUע������Ϣ�е�Attempt Counter��ֵ */
            ulAttempCounter = pstRegRsltCtx->ucEpsAttemptCounter;

            break;
#endif
        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_GetRegRsltAttemptCounter: UNEXPECTED MSG TYPE!");
            break;
    }

    return ulAttempCounter;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause2ImsiUnknownInHlr(VOS_VOID)
{
#if   (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                          ulDisablteRoamFlg;

    ulDisablteRoamFlg = NAS_MML_GetDisableLteRoamFlg();
#endif

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);

#if   (FEATURE_ON == FEATURE_LTE)
    if (VOS_TRUE == NAS_MMC_IsNeedEnableLte_CsRegFailCause2ImsiUnknownInHlr())
    {
        /* ������ڽ�ֹLTE�������ε��µ�disable LTE����δ�ص�����֮ǰ��ʱҲ��֪ͨenable LTE */
        if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == NAS_MML_GetLteCapabilityStatus())
         && (VOS_FALSE == ulDisablteRoamFlg))
        {
            /* ��WAS/GAS����enable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);

            /* ��LMM����enable LTE��Ϣ */
            NAS_MMC_SndLmmEnableLteNotify();
        }

        if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER))
        {
            NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER);
        }

        /* ����disable LTE������� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }
#endif

    return NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause3IllegalMs(VOS_VOID)
{
    /* ��ԭ��ֵ2���д��� */
    return NAS_MMC_ProcCsRegFailCause2ImsiUnknownInHlr();
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause6IllegalMe(VOS_VOID)
{
    /* ��ԭ��ֵ2���д��� */
    return NAS_MMC_ProcCsRegFailCause2ImsiUnknownInHlr();
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause11PlmnNotAllow(VOS_VOID)
{
    VOS_UINT8                           ucRejDomain;
    NAS_MML_MS_CFG_INFO_STRU           *pstMsCfg;
    NAS_MML_PLMN_ID_STRU               *pstCurPlmnId;

    pstCurPlmnId = NAS_MML_GetCurrCampPlmnId();

    /* ��ӽ�ֹ������Ϣ,�и���ʱ����Usim�����ļ� */
    if (VOS_TRUE == NAS_MML_AddForbPlmn(pstCurPlmnId))
    {
        NAS_MMC_SndUsimUpdateForbPlmnReq();
    }

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    if ( VOS_FALSE == NAS_MML_GetHplmnRegisterCtrlFlg())
    {
        /* ��E5�汾�������´���E5�汾Ҫ������HPLMN�Ϸ�������ע�� */

        if (VOS_TRUE == NAS_MML_ComparePlmnIdWithHplmn(NAS_MML_GetCurrCampPlmnId()))
        {
        /* ����ע��������ע����Ϣ�� */
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(),
                      NAS_MMC_REG_DOMAIN_CS, NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW);

        pstMsCfg = NAS_MML_GetMsCfgInfo();

            if ((NAS_MML_MS_MODE_PS_CS == pstMsCfg->stMsSysCfgInfo.enMsMode))
            {
                NAS_MMC_GetHplmnRejDomainInfo(&ucRejDomain);

                if (NAS_MMC_REG_DOMAIN_PS_CS != ucRejDomain)
                {
                    NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);

                    return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
                }
            }
        }
    }

    return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;

}

NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause12LaNotAllow(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8          enPsAdditionalAction;

    /*��ӽ�ֹ������Ϣ*/
    NAS_MML_AddForbRegLa(NAS_MML_GetCurrCampLai());

    NAS_MMC_StartTimer(TI_NAS_MMC_FORBID_LA_TIMER, TI_NAS_MMC_FORBID_LA_TIMER_LEN);

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE_REGION);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, NAS_MMC_LIMITED_SERVICE_REGION);

    enPsAdditionalAction = NAS_MMC_GetPsAdditionalAction();
    if ((NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL != enPsAdditionalAction)
     && (NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION != enPsAdditionalAction)
     && (NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN != enPsAdditionalAction))
    {
        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    return NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause13RoamNotAllow(VOS_VOID)
{
    NAS_MML_FORBIDPLMN_ROAMING_LAS_INFO_STRU                stForbRoamLaInfo;
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU                        *pstForbPlmnInfo = VOS_NULL_PTR;

    /* ���û�ָ����������ȫ�ֱ����л�ȡFORBIDEN ROAMING LAS��Ϣ */
    pstForbPlmnInfo                  = NAS_MML_GetForbidPlmnInfo();
    stForbRoamLaInfo.ucForbRoamLaNum = pstForbPlmnInfo->ucForbRoamLaNum;
    PS_MEM_CPY( stForbRoamLaInfo.astForbRomLaList,
                pstForbPlmnInfo->astForbRomLaList,
                sizeof(pstForbPlmnInfo->astForbRomLaList) );

    /* ��ӽ�ֹ������Ϣ��ȫ�ֱ����Ľ�ֹ����LA������ȥ */
    NAS_MML_AddForbRoamLa( NAS_MML_GetCurrCampLai(),
                           &stForbRoamLaInfo );
    pstForbPlmnInfo->ucForbRoamLaNum = stForbRoamLaInfo.ucForbRoamLaNum;
    PS_MEM_CPY( pstForbPlmnInfo->astForbRomLaList,
                stForbRoamLaInfo.astForbRomLaList,
                sizeof(pstForbPlmnInfo->astForbRomLaList));

    NAS_MMC_StartTimer(TI_NAS_MMC_FORBID_LA_TIMER, TI_NAS_MMC_FORBID_LA_TIMER_LEN);

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause15NoSuitableCell(VOS_VOID)
{

    NAS_MML_FORBIDPLMN_ROAMING_LAS_INFO_STRU                stForbRoamLaInfo;
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU                        *pstForbPlmnInfo = VOS_NULL_PTR;

    /* ���û�ָ����������ȫ�ֱ����л�ȡFORBIDEN ROAMING LAS��Ϣ */
    pstForbPlmnInfo                  = NAS_MML_GetForbidPlmnInfo();
    stForbRoamLaInfo.ucForbRoamLaNum = pstForbPlmnInfo->ucForbRoamLaNum;
    PS_MEM_CPY( stForbRoamLaInfo.astForbRomLaList,
                pstForbPlmnInfo->astForbRomLaList,
                sizeof(pstForbPlmnInfo->astForbRomLaList) );

    /* ��ӽ�ֹ������Ϣ��ȫ�ֱ����Ľ�ֹ����LA������ȥ */
    NAS_MML_AddForbRoamLa( NAS_MML_GetCurrCampLai(), &stForbRoamLaInfo );
    pstForbPlmnInfo->ucForbRoamLaNum = stForbRoamLaInfo.ucForbRoamLaNum;
    PS_MEM_CPY( pstForbPlmnInfo->astForbRomLaList,
                stForbRoamLaInfo.astForbRomLaList,
                sizeof(pstForbPlmnInfo->astForbRomLaList));


    NAS_MMC_StartTimer(TI_NAS_MMC_FORBID_LA_TIMER, TI_NAS_MMC_FORBID_LA_TIMER_LEN);

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause17NetWorkFailure(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8    enAction;
    VOS_UINT8                               ucRegFailMaxCnt;
    VOS_UINT32                              ulAttemptCnt;
    VOS_UINT8                               ulLaiSameFlg;
    NAS_MML_CAMP_PLMN_INFO_STRU            *pstCampInfo     = VOS_NULL_PTR;
    NAS_MML_LAI_STRU                       *pstLastSuccLai  = VOS_NULL_PTR;

    pstCampInfo    = NAS_MML_GetCurrCampPlmnInfo();
    pstLastSuccLai = NAS_MML_GetCsLastSuccLai();

    enAction        = NAS_MMC_ADDITIONAL_ACTION_BUTT;

    ucRegFailMaxCnt = NAS_MML_MAX_CS_REG_FAIL_CNT;

    /* ��roaming broker���Կ�������ʹ��ROAMING BROKER�����ʧ�ܸ�������������
       δ��ʱ��ʹ�õ�ֵΪNAS_MML_MAX_CS_REG_FAIL_CNT */
    if ( VOS_TRUE == NAS_MML_GetRoamBrokerActiveFlag() )
    {
        ucRegFailMaxCnt = NAS_MML_GetRoamBrokerRegisterFailCnt();
    }

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(MM_MMC_CS_REG_RESULT_IND);

    /* ���ƿ��ش� */
    if (VOS_TRUE == NAS_MML_GetRegFailNetWorkFailureCustomFlag())
    {
        ulLaiSameFlg = NAS_MML_CompareLai(&(pstCampInfo->stLai), pstLastSuccLai);

        /* ��ǰפ����LAI�����ϴγɹ�פ����LAI��CS UPDATE״̬����U1������ATTEMPCNT�ﵽ���ֵ��Ҫ�������� */
        if ((NAS_MML_LOCATION_UPDATE_STATUS_UPDATED != NAS_MML_GetCsUpdateStatus())
         || (VOS_FALSE == ulLaiSameFlg)
         || (ulAttemptCnt >= ucRegFailMaxCnt))
        {
            /* ���·���״̬ */
            NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

            /* ֪ͨTAF��ǰ�ķ���״̬ */
            NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

            return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
        }
    }

    if (ulAttemptCnt < ucRegFailMaxCnt)
    {
        enAction = NAS_MMC_ProcCsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcCsRegFailCause17AttemptCounterMaxTimes();
    }

    return enAction;
}

NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause95SemanticallyIncorrectMsg(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    if (ulAttemptCnt < NAS_MML_MAX_CS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcCsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcCsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause96InvalidMandatoryInf(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;

    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    if (ulAttemptCnt < NAS_MML_MAX_CS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcCsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcCsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause97MsgNonExistNotImplement(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    if (ulAttemptCnt < NAS_MML_MAX_CS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcCsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcCsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause99IeNonExistNotImplement(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    if (ulAttemptCnt < NAS_MML_MAX_CS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcCsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcCsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause111ProtocolError(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    if (ulAttemptCnt < NAS_MML_MAX_CS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcCsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcCsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause256AuthRej(VOS_VOID)
{
#if   (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                          ulDisablteRoamFlg;

    ulDisablteRoamFlg = NAS_MML_GetDisableLteRoamFlg();
#endif

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);
    NAS_MMC_ChangePsRegState(NAS_MML_REG_REGISTERED_DENIED);

#if   (FEATURE_ON == FEATURE_LTE)
    if (VOS_TRUE == NAS_MMC_IsNeedEnableLte_CsRegFailCause256AuthRej())
    {
        /* ������ڽ�ֹLTE�������ε��µ�disable LTE����δ�ص�����֮ǰ��ʱҲ��֪ͨenable LTE */
        if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == NAS_MML_GetLteCapabilityStatus())
         && (VOS_FALSE == ulDisablteRoamFlg))
        {
            /* ��WAS/GAS����enable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);

            /* ��LMM����enable LTE��Ϣ */
            NAS_MMC_SndLmmEnableLteNotify();
        }

        if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER))
        {
            NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER);
        }

        /* ����disable LTE������� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }
#endif

    return NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause257AccessBarrd(VOS_VOID)
{
    NAS_MML_LOCATION_UPDATE_STATUS_ENUM_UINT8   enCsUpdateStatus;
    NAS_MML_CAMP_PLMN_INFO_STRU                *pstCampInfo     = VOS_NULL_PTR;
    NAS_MML_LAI_STRU                           *pstLastSuccLai  = VOS_NULL_PTR;

    enCsUpdateStatus = NAS_MML_GetCsUpdateStatus();
    pstCampInfo      = NAS_MML_GetCurrCampPlmnInfo();
    pstLastSuccLai   = NAS_MML_GetCsLastSuccLai();

    /* ��ǰLAI���ϴ�ע��ɹ�LAI��ͬ���Ҹ���״̬ΪU1 */
    if ((VOS_TRUE == NAS_MML_CompareLai(&(pstCampInfo->stLai), pstLastSuccLai))
     && (NAS_MML_LOCATION_UPDATE_STATUS_UPDATED == enCsUpdateStatus)
     && (MM_STATUS_ATTACHED == (NAS_MMC_GetMmCtx())->ucCsAttachState))
    {
        NAS_MMC_UpdateCsRegStateCsRegSucc();
    }
    else
    {
        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    /*���Կ��ش��ҷ�HPLMN/RPLMN�����ֹʱ��Ҫ����PLMN����*/
    if (VOS_TRUE == NAS_MMC_IsAccBarPlmnSearch())
    {
        return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    }
    else
    {
        return NAS_MMC_ADDITIONAL_ACTION_ACCESS_BARRED;
    }

}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause301TimerTimeOut(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;


    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    if (ulAttemptCnt < NAS_MML_MAX_CS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcCsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_GetSingleDomainFailAction(NAS_MML_REG_DOMAIN_CS, NAS_MML_REG_FAIL_CAUSE_TIMER_TIMEOUT);

        if (NAS_MMC_ADDITIONAL_ACTION_BUTT != enAction)
        {
            /* ���·���״̬ */
            NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

            /* ֪ͨTAF��ǰ�ķ���״̬ */
            NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

            enPrioAdditionalAction = NAS_MMC_GetPsAdditionalAction();
            enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(enAction, enPrioAdditionalAction);
            if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
            {
                NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
            }

            return enAction;
        }
        else
        {
            enAction = NAS_MMC_ProcCsRegFailAttemptCounterMaxTimes();
        }
    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause302RrConnEstFail(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    if (ulAttemptCnt < NAS_MML_MAX_CS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcCsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcCsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause303RrConnFail(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    if (ulAttemptCnt < NAS_MML_MAX_CS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcCsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcCsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause401OtherCause(VOS_VOID)
{
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    if (ulAttemptCnt >= NAS_MML_MAX_CS_REG_FAIL_CNT)
    {
        /* ��ȡCS��PS��additon action��ȡ�����ȼ����ж��Ƿ���Ҫ������
           �������Ҫ����CSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
        enPsAdditionalAction   = NAS_MMC_GetPsAdditionalAction();
        enCsAdditionalAction   = NAS_MMC_GetCsAdditionalAction();

        enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(enCsAdditionalAction, enPsAdditionalAction);
        if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
        {
            NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }
    }

    /* ����NAS_MMC_ADDITIONAL_ACTION_BUTT����ʾ��Ч��AdditionalActionֵ */
    return NAS_MMC_ADDITIONAL_ACTION_BUTT;
}

NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause402MsCfgDomainNotSupport(VOS_VOID)
{
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;
    NAS_MMC_TIMER_STATUS_ENUM_U8                            enAvailableTimerStatus;
    VOS_UINT8                                               ucPsAttachAllow;
    VOS_UINT32                                              ulRlst;
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU                       *pstForbidPlmnInfo = VOS_NULL_PTR;


    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);

    enFsmId                = NAS_MMC_GetCurrFsmId();
    if (NAS_FSM_BUTT <= enFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    enAvailableTimerStatus = NAS_MMC_GetTimerStatus(TI_NAS_MMC_AVAILABLE_TIMER);
    ucPsAttachAllow        = NAS_MML_GetPsAttachAllowFlg();

    /* ��ǰ��ONPLMN,PLMN_LISTʱ�����յ����ΪCS��֧�֣�����Ҫ�жϴ�ʱ�Ƿ���Ҫ����������
       1)PS����LIMIT_CAMP_ON 2)AVAILABLE��ʱ��δ���� 3)PS��׼����ע��,��MMC��Ҫ��������
    */
    if ( (NAS_MMC_FSM_L1_MAIN   == enFsmId)
      || (NAS_MMC_FSM_PLMN_LIST == enFsmId) )
    {
        if (VOS_FALSE == NAS_MMC_IsNeedPlmnSearch_RegLocalFailCause())
        {
            return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
        }

        pstForbidPlmnInfo = NAS_MML_GetForbidPlmnInfo();
        /* ��forbid plmn for gprs�б��� */
        ulRlst = NAS_MML_IsBcchPlmnIdInDestSimPlmnList(NAS_MML_GetCurrCampPlmnId(),
                                              pstForbidPlmnInfo->ucForbGprsPlmnNum,
                                              pstForbidPlmnInfo->astForbGprsPlmnList);
        if ( (VOS_TRUE                  == ucPsAttachAllow)
          && (VOS_TRUE                  == ulRlst)
          && (NAS_MMC_TIMER_STATUS_STOP == enAvailableTimerStatus) )
        {
            return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
        }
    }

    return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
}
VOS_UINT32 NAS_MMC_IsNeedPlmnSearch_RegLocalFailCause( VOS_VOID )
{
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstUserSpecPlmn     = VOS_NULL_PTR;
    VOS_UINT32                                              ulGsmOnlyFlg;
    VOS_UINT32                                              ulGsmForbFlg;
    VOS_UINT32                                              ulUserSpecPlmnForbPlmnFlag;

    ulGsmOnlyFlg                = NAS_MML_IsGsmOnlyMode(NAS_MML_GetMsPrioRatList());
    ulGsmForbFlg                = NAS_MML_GetGsmForbidFlg();
    pstUserSpecPlmn             = NAS_MMC_GetUserSpecPlmnId();

    /* 1.USIM����ֹGSM�����û�����GSM only������VOS_FALSE,�������� */
    if ((VOS_TRUE == ulGsmForbFlg)
     && (VOS_TRUE == ulGsmOnlyFlg) )
    {
        return VOS_FALSE;
    }

    /* 2.���UE���õ�������ʽ������ֹ���������� */
    if (VOS_TRUE == NAS_MML_IsAllMsRatInForbiddenList())
    {
        return VOS_FALSE;
    }

    /* 3.����Ч���������� */
    if ( VOS_FALSE == NAS_MML_IsUsimStausValid())
    {
        return VOS_FALSE;
    }

    /* ��ǰΪ�ֶ�ģʽ���û�ָ���������ڽ�ֹ�����б��У��������� */
    ulUserSpecPlmnForbPlmnFlag    = NAS_MML_IsPlmnIdInForbidPlmnList( &(pstUserSpecPlmn->stPlmnId) );
    if ( (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
       && (VOS_TRUE == ulUserSpecPlmnForbPlmnFlag))
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}



NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause403ForbiddenPlmn(VOS_VOID)
{
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;
    NAS_MMC_TIMER_STATUS_ENUM_U8                            enAvailableTimerStatus;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstUserSpecPlmn     = VOS_NULL_PTR;
    VOS_UINT32                                              ulUserSpecPlmnFlag;
    NAS_MML_PLMN_ID_STRU                                   *pstCurPlmnId        = VOS_NULL_PTR;

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    enAvailableTimerStatus = NAS_MMC_GetTimerStatus(TI_NAS_MMC_AVAILABLE_TIMER);

    enFsmId = NAS_MMC_GetCurrFsmId();
    switch ( enFsmId )
    {
        case NAS_MMC_FSM_L1_MAIN :
        case NAS_MMC_FSM_PLMN_LIST :

            if (VOS_FALSE == NAS_MMC_IsNeedPlmnSearch_RegLocalFailCause())
            {
                return NAS_MMC_ADDITIONAL_ACTION_BUTT;
            }

            /* Ĭ�Ϸ���NAS_MMC_ADDITIONAL_ACTION_BUTT����ʾ��Ч��AdditionalActionֵ */
            enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;

            /* �����³�����Ҫ����:�ֶ�ģʽ�£���ǰPlmn�����û�ָ����PLMN,�ҵ�ǰPLMN����EPlmn�б��
               Available��ʱ��δ����������Ϊ��ǰplmn �Ѿ�ʧЧ�ˣ���Ҫ�������� */

            /* �ֶ�ģʽ�жϵ�ǰ�����Ƿ����û�ָ�������� */
            pstCurPlmnId       = NAS_MML_GetCurrCampPlmnId();
            pstUserSpecPlmn    = NAS_MMC_GetUserSpecPlmnId();
            ulUserSpecPlmnFlag = NAS_MML_CompareBcchPlmnwithSimPlmn(pstCurPlmnId,
                                                        &(pstUserSpecPlmn->stPlmnId));

            /* �ֶ�ģʽ�£���ǰPlmn�����û�ָ����PLMN,�ҵ�ǰPLMN����EPlmn�б��
               ����Ϊ��ǰplmn �Ѿ�ʧЧ�ˣ�Available ��ʱ��δ��������Ҫ�������� */
            if ( (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
              && (VOS_FALSE                          == ulUserSpecPlmnFlag)
              && (NAS_MMC_TIMER_STATUS_STOP          == enAvailableTimerStatus) )
            {
                enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
            }

            break;

        /* ��Ҫ�������� */
        default:
            enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
            break;
    }

    return enAdditionalAction;

}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause404ForbiddenLaForRegPrvsSvs(VOS_VOID)
{
    /* ��ע�ᱻ��ԭ��#12�Ĵ���һ�� */
    return NAS_MMC_ProcCsRegFailCause12LaNotAllow();
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause405ForbiddenLaForRoam(VOS_VOID)
{
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsCause;                              /* �洢ע����CS�򱻾���Ϣ */
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsCause;                              /* �洢ע����PS�򱻾���Ϣ */

    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);


    /* ��ǰ��ONPLMN����PLMN LIST״̬������ʱ��������פ��̬����δ��������ʵ��������������£�
       �����µ�ǰAdditionalActionֵ */
    enFsmId = NAS_MMC_GetCurrFsmId();
    if ( (NAS_MMC_FSM_L1_MAIN == enFsmId)
      || (NAS_MMC_FSM_PLMN_LIST == enFsmId) )
    {
        /* ����NAS_MMC_ADDITIONAL_ACTION_BUTT����ʾ��Ч��AdditionalActionֵ,MMC
           ��������ЧAdditionalActionֵ */
        return NAS_MMC_ADDITIONAL_ACTION_BUTT;
    }


    /* ��ǰ������ONPLMN����PLMN LIST״̬�����账��ܾ�ԭ��ֵ�����ز�ͬ���������� */

    /* ԭ��ֵ��ʼ��Ϊ#13,�������쳣������г�������� */
    enCsCause = NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW;
    enPsCause = NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW;

    NAS_MMC_GetDomainRegStatusInRegInfoList( NAS_MML_GetCurrCampPlmnId(),
                                             NAS_MML_GetCurrNetRatType(),
                                             &enCsCause,
                                             &enPsCause );

    /* ��CS�����PS����һ�����ԭ��ֵΪ#13,����г�������� */
    if ( (NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW == enCsCause)
      || (NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW == enPsCause))
    {
        return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
    }

    /* �����վܾ�ԭ��ֵ#15���������Խ������� */
    return NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN;


}

NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause407SimInvalid(VOS_VOID)
{
    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    /* �����ڱ�ʾ�϶���ע�ᱻ�ܹ� */
    if (VOS_TRUE == NAS_MML_GetSimPresentStatus())
    {
        NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);
    }
    else
    {
        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }


    enFsmId = NAS_MMC_GetCurrFsmId();
    if ( (NAS_MMC_FSM_L1_MAIN == enFsmId)
      || (NAS_MMC_FSM_PLMN_LIST == enFsmId) )
    {
        /* ����NAS_MMC_ADDITIONAL_ACTION_BUTT����ʾ��Ч��AdditionalActionֵ,MMC
           ��������ЧAdditionalActionֵ */
        return NAS_MMC_ADDITIONAL_ACTION_BUTT;
    }

    return NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause408NetDetachImsi(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    /* ��ȡCSPS addition,�������Ҫ�������������CSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
    enPsAdditionalAction   = NAS_MMC_GetPsAdditionalAction();
    enCsAdditionalAction   = NAS_MMC_GetCsAdditionalAction();

    enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(enCsAdditionalAction, enPsAdditionalAction);
    if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
    {
        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    /* ����NAS_MMC_ADDITIONAL_ACTION_BUTT����ʾ��Ч��AdditionalActionֵ,
       MMC��������ЧAdditionalActionֵ*/
    return NAS_MMC_ADDITIONAL_ACTION_BUTT;

}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause409ServingCellDomainNotSupport(VOS_VOID)
{
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;
    NAS_MMC_TIMER_STATUS_ENUM_U8                            enAvailableTimerStatus;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    enFsmId                = NAS_MMC_GetCurrFsmId();
    enAvailableTimerStatus = NAS_MMC_GetTimerStatus(TI_NAS_MMC_AVAILABLE_TIMER);

    /* ��ǰ��ONPLMNʱ�����յ����ΪCS��֧�֣�����Ҫ�жϴ�ʱ�Ƿ���Ҫ����������
       1)PS����LIMIT_CAMP_ON 2)AVAILABLE��ʱ��δ���� 3)PS��׼����ע��,��MMC��Ҫ��������
    */
    if ( (NAS_MMC_FSM_L1_MAIN   == enFsmId)
      || (NAS_MMC_FSM_PLMN_LIST == enFsmId) )
    {
        if (VOS_FALSE == NAS_MMC_IsNeedPlmnSearch_RegLocalFailCause())
        {
            return NAS_MMC_ADDITIONAL_ACTION_BUTT;
        }

        if (NAS_MMC_TIMER_STATUS_RUNING               == enAvailableTimerStatus)
        {
            return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
        }


        /* ��L1 MAINʱ�������ǰ���޿�����PS���ز���Ҫ���� */
        if ( VOS_FALSE == NAS_MML_GetSimPresentStatus() )
        {
            /* ����NAS_MMC_ADDITIONAL_ACTION_BUTT����ʾ��Ч��AdditionalActionֵ,MMC
                ��������ЧAdditionalActionֵ */
            return NAS_MMC_ADDITIONAL_ACTION_BUTT;
        }
    }


    enPrioAdditionalAction = NAS_MMC_GetPsAdditionalAction();
    enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION, enPrioAdditionalAction);
    if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
    {
        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;

}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause416DealyCsInTransLau(VOS_VOID)
{
    return NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
}




NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause417T3212Running(VOS_VOID)
{
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;    
    VOS_UINT8                                               ucSingleDomainSrchFlag;

    enFsmId                = NAS_MMC_GetCurrFsmId();

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    /* ȡ�õ�ǰ��CS/PS���ADDITIONAL ACTIONֵ */
    enCsAdditionalAction   = NAS_MMC_GetCsAdditionalAction();
    enPsAdditionalAction   = NAS_MMC_GetPsAdditionalAction();

    /* ��ѡ��״̬����Ҫ��ȡ��ǰ��CS���ADDITIONAL ACTIONֵ */
    if ( (NAS_MMC_FSM_PLMN_SELECTION == enFsmId)
      || (NAS_MMC_FSM_BG_PLMN_SEARCH == enFsmId) )
    {
        /* �����������Ʊ�ʶ */
        ucSingleDomainSrchFlag = NAS_MML_GetDtSingleDomainPlmnSearchFlag();

        /* ������ʧ�ܱ����Ƿ���������������VDF���ƴ򿪻�DT�����������ƴ�ʱ����������ʱ���ȴ�PS��ע���� */
        if ((VOS_TRUE == NAS_MML_GetCsRejSearchSupportFlg())
         || (VOS_TRUE == ucSingleDomainSrchFlag))
        {
            enCsAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
        }
        else
        {
            enCsAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
        }
    }
    
    enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(enCsAdditionalAction, enPsAdditionalAction);
    if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
    {
        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    return enCsAdditionalAction;    
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCauseAbnormalCause(
    VOS_UINT16                          usCause,
    VOS_UINT32                          ulAttemptCnt
)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT8                                               ucRegFailMaxCnt;

    /* ��ʼ��CS��Attempt Counter����ע��ʧ�ܵ�������Ϊ4 */
    ucRegFailMaxCnt = NAS_MML_MAX_CS_REG_FAIL_CNT;

    /* ��roaming broker���Կ�������ʹ��ROAMING BROKER�����ʧ�ܸ�������������
       δ��ʱ��ʹ�õ�ֵΪNAS_MML_MAX_CS_REG_FAIL_CNT */
    if ( VOS_TRUE == NAS_MML_GetRoamBrokerActiveFlag() )
    {
        ucRegFailMaxCnt = NAS_MML_GetRoamBrokerRegisterFailCnt();
    }

    if (ulAttemptCnt < ucRegFailMaxCnt)
    {
        enAction = NAS_MMC_ProcCsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcCsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}



NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailAttemptCounterMaxTimes(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT8                                               ucSingleDomainSrchFlag;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    /* �����������Ʊ�ʶ */
    ucSingleDomainSrchFlag = NAS_MML_GetDtSingleDomainPlmnSearchFlag();

    /* ������ʧ�ܱ����Ƿ���������������VDF���ƴ򿪻�DT�����������ƴ�ʱ����������ʱ���ȴ�PS��ע���� */
    if ((VOS_TRUE == NAS_MML_GetCsRejSearchSupportFlg())
     || (VOS_TRUE == ucSingleDomainSrchFlag))
    {
        enAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
    }
    else
    {
        enAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    }

    enPrioAdditionalAction = NAS_MMC_GetPsAdditionalAction();
    enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(enAction, enPrioAdditionalAction);
    if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
    {
        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    return enAction;
}




NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailCause17AttemptCounterMaxTimes(VOS_VOID)
{
    NAS_MML_SOR_ADDITIONAL_LAU_STRU                        *pstAdditionalLau    = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU                            *pstCampInfo         = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT8                                               ucSingleDomainSrchFlag;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    VOS_UINT8                                               ucRegFailMaxCnt;
    VOS_UINT32                                              ulAttemptCnt;

    ucRegFailMaxCnt = NAS_MML_MAX_CS_REG_FAIL_CNT;    

    /* ��roaming broker���Կ�������ʹ��ROAMING BROKER�����ʧ�ܸ�������������
       δ��ʱ��ʹ�õ�ֵΪNAS_MML_MAX_CS_REG_FAIL_CNT */
    if ( VOS_TRUE == NAS_MML_GetRoamBrokerActiveFlag() )
    {
        ucRegFailMaxCnt = NAS_MML_GetRoamBrokerRegisterFailCnt();
    }

    ulAttemptCnt        = NAS_MMC_GetRegRsltAttemptCounter(MM_MMC_CS_REG_RESULT_IND);

    pstAdditionalLau    = NAS_MML_GetSorAdditionalLauCtx();
    pstCampInfo         = NAS_MML_GetCurrCampPlmnInfo();

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    /* �����������Ʊ�ʶ */
    ucSingleDomainSrchFlag = NAS_MML_GetDtSingleDomainPlmnSearchFlag();

    /* ������ʧ�ܱ����Ƿ���������������VDF���ƴ�ʱ����������ʱ���ȴ�PS��ע���� */
    if (VOS_TRUE == NAS_MML_GetCsRejSearchSupportFlg())
    {

        /* SOR�ֶ�ѡ��ģʽ��������פ�� */
        if (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
        {
            enAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
        }
        
        /* ���SOR���Զ��ƴ򿪣���ǰCOUNTER�ﵽ��������������additional LAU��Ϣ */
        else if ( ulAttemptCnt == ucRegFailMaxCnt )
        {
            pstAdditionalLau->ucAdditionalLauFlag = VOS_TRUE;
            PS_MEM_CPY(&(pstAdditionalLau->stLai), &(pstCampInfo->stLai), sizeof(NAS_MML_LAI_STRU));

            enAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
        }
        else    
        {         
            if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_AVAILABLE_TIMER))
            {
                enAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
            }
            else
            {
                enAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
            }          
                
        }
    }

    /* DT�����������ƴ�ʱ����������ʱ���ȴ�PS��ע���� */
    else if (VOS_TRUE == ucSingleDomainSrchFlag)
    {
        enAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
    }
    else
    {
        enAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    }

    enPrioAdditionalAction = NAS_MMC_GetPsAdditionalAction();
    enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(enAction, enPrioAdditionalAction);
    if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
    {
        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFailAttemptCounterLessThanMaxTimes(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEplmnInfo        = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstUserSpecPlmn     = VOS_NULL_PTR;
    VOS_UINT32                                              ulInEplmnListFlag;
    VOS_UINT32                                              ulUserSpecPlmnFlag;
    VOS_UINT32                                              ulLaiNoChangeFlag;
    NAS_MML_PLMN_ID_STRU                                   *pstCurPlmnId        = VOS_NULL_PTR;

    NAS_MML_REG_STATUS_ENUM_UINT8                           enCsRegStatus;

    enCsRegStatus = NAS_MML_GetCsRegStatus();

    /* Ĭ�����õ�ǰ�����ȴ�ע���� */
    enAction      = NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;

    ulLaiNoChangeFlag = NAS_MML_IsLaiInDestLaiList( NAS_MML_GetCurrCampLai(),
                                                    0x01,
                                                    NAS_MML_GetCsLastSuccLai() );

    /* �����ǰ��LAI�����ı䣬����µ�ǰΪ���Ʒ���״̬ */
    if ( VOS_FALSE == ulLaiNoChangeFlag )
    {
        /* ���·���״̬ */
        NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

        /* ֪ͨTAF��ǰ�ķ���״̬ */
        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

        /* ���֮ǰע��ɹ�����Ҫ��ע��״̬����Ϊ0 */
        if ((NAS_MML_REG_REGISTERED_HOME_NETWORK == enCsRegStatus)
         || (NAS_MML_REG_REGISTERED_ROAM == enCsRegStatus))
        {
            NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }
    }

    /* �Զ�ģʽ����NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT */
    if ( NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode() )
    {
        return enAction;
    }

    /* �ֶ�ģʽ�жϵ�ǰ�����Ƿ����û�ָ�������� */
    pstCurPlmnId       = NAS_MML_GetCurrCampPlmnId();
    pstUserSpecPlmn    = NAS_MMC_GetUserSpecPlmnId();
    ulUserSpecPlmnFlag = NAS_MML_CompareBcchPlmnwithSimPlmn(pstCurPlmnId,
                                                &(pstUserSpecPlmn->stPlmnId));

    /* �жϵ�ǰ�����Ƿ���EPlmn�б��� */
    pstEplmnInfo       = NAS_MML_GetEquPlmnList();

    ulInEplmnListFlag  = NAS_MML_IsBcchPlmnIdInDestSimPlmnList(pstCurPlmnId,
                                              pstEplmnInfo->ucEquPlmnNum,
                                              pstEplmnInfo->astEquPlmnAddr);

    /* �ֶ�ģʽ�£���ǰPlmn�����û�ָ����PLMN,�ҵ�ǰPLMN����EPlmn�б��
       ����Ϊ��ǰplmn �Ѿ�ʧЧ�ˣ���Ҫ�������� */
    if ( (VOS_FALSE == ulUserSpecPlmnFlag)
      && (VOS_FALSE == ulInEplmnListFlag) )
    {
        enAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    }

    return enAction;
}




NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause3IllegalMs(VOS_VOID)
{
#if   (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                          ulDisablteRoamFlg;

    ulDisablteRoamFlg = NAS_MML_GetDisableLteRoamFlg();
#endif

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,VOS_FALSE);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);
    NAS_MMC_ChangePsRegState(NAS_MML_REG_REGISTERED_DENIED);

#if   (FEATURE_ON == FEATURE_LTE)
    if (VOS_TRUE == NAS_MMC_IsNeedEnableLte_PsRegFailCause3IllegalMs())
    {
        /* ������ڽ�ֹLTE�������ε��µ�disable LTE����δ�ص�����֮ǰ��ʱҲ��֪ͨenable LTE */
        if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == NAS_MML_GetLteCapabilityStatus())
         && (VOS_FALSE == ulDisablteRoamFlg))
        {
            /* ��WAS/GAS����enable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);

            /* ��LMM����enable LTE��Ϣ */
            NAS_MMC_SndLmmEnableLteNotify();
        }

        if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER))
        {
            NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER);
        }

        /* ����disable LTE������� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }
#endif

    return NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL;

}

NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause6IllegalMe(VOS_VOID)
{
    /* ��PsCause6 ���� */
    return NAS_MMC_ProcPsRegFailCause3IllegalMs();
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause7GprsServNotAllow(VOS_VOID)
{
    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,VOS_FALSE);

    NAS_MMC_ChangePsRegState(NAS_MML_REG_REGISTERED_DENIED);

#if (FEATURE_ON == FEATURE_LTE)

    /* �ж��Ƿ���Ҫdisable LTE */
    if (VOS_TRUE == NAS_MMC_IsNeedDisableL_PsFailCause7PsServNotAllow())
    {
        /* ������ε��µ�disable�Ѿ�֪ͨ��GU�����,��ʱ����Ҫ���ظ�֪ͨ */
        if (VOS_FALSE == NAS_MML_GetDisableLteRoamFlg())
        {
            /* ��WAS/GAS����disable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_DISABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_DISABLE);

            /* ��LMM����disable LTE��Ϣ */
            NAS_MMC_SndLmmDisableLteNotify(MMC_LMM_DISABLE_LTE_REASON_PS_USIM_INVALID);
        }

        /* ����disable LTE������� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS);

        NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_PS_USIM_INVALID);
    }
#endif
    return NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause8GprsAndNonGprsNotAllow(VOS_VOID)
{
#if   (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                          ulDisablteRoamFlg;

    ulDisablteRoamFlg = NAS_MML_GetDisableLteRoamFlg();
#endif

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,VOS_FALSE);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);
    NAS_MMC_ChangePsRegState(NAS_MML_REG_REGISTERED_DENIED);

#if   (FEATURE_ON == FEATURE_LTE)
    if (VOS_TRUE == NAS_MMC_IsNeedEnableLte_PsRegFailCause8GprsAndNonGprsNotAllow())
    {
        /* ������ڽ�ֹLTE�������ε��µ�disable LTE����δ�ص�����֮ǰ��ʱҲ��֪ͨenable LTE */
        if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == NAS_MML_GetLteCapabilityStatus())
         && (VOS_FALSE == ulDisablteRoamFlg))
        {
            /* ��WAS/GAS����enable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);

            /* ��LMM����enable LTE��Ϣ */
            NAS_MMC_SndLmmEnableLteNotify();
        }

        if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER))
        {
            NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER);
        }

        /* ����disable LTE������� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }
#endif

    return NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause9MsIdNotDerived(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    NAS_MMC_PLMN_SELECTION_MODE_ENUM_UINT8                  ucPlmnSelMode;

    /* �ֶ�ģʽ����Ҫattach������£�����Ҫɾ��EPlmn���Զ�ģʽ���߲���Ҫ
    ATTACH��ʱ����Ҫɾ��EPLMN���˴��޸Ĳ�����Э��淶���޸�Ϊ���û����顣
    �����ֶ�ģʽEPLMN��ɾ��ʱ����������ֶ�����RAU���ܴ�����ѭ����
    �˴��޸����NAS_GMM_DeleteEPlmnList����ֶ��������ܣ�������ѭ�����⡣*/
    ucPlmnSelMode = NAS_MMC_GetPlmnSelectionMode();

    if ((VOS_TRUE == NAS_MML_GetPsAttachAllowFlg())
     && (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == ucPlmnSelMode))
    {
        NAS_MML_SetEplmnValidFlg(VOS_TRUE);
    }

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    /* ��ǰ��GMM�ϱ���NET WORK DETACH IND��Ϣ���򷵻�LIMIT_CAMP_ON */
    if ( GMM_MMC_PS_NETWORK_DETACH_IND == enRegRsltType)
    {
        /* ��ȡCS addition,����NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON�Ƚϣ�
           �������Ҫ�������������PSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
        enPrioAdditionalAction = NAS_MMC_GetCsAdditionalAction();
        enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON, enPrioAdditionalAction);
        if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
        {
            NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
    }

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    /* ����ע�����ϱ���Attempt Counter�Ƿ�ﵽ�������ֱ���д��� */
    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause10ImplicitDetached(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    NAS_MMC_PLMN_SELECTION_MODE_ENUM_UINT8                  ucPlmnSelMode;

    /* �ֶ�ģʽ����Ҫattach������£�����Ҫɾ��EPlmn���Զ�ģʽ���߲���Ҫ
    ATTACH��ʱ����Ҫɾ��EPLMN���˴��޸Ĳ�����Э��淶���޸�Ϊ���û����顣
    �����ֶ�ģʽEPLMN��ɾ��ʱ����������ֶ�����RAU���ܴ�����ѭ����
    �˴��޸����NAS_GMM_DeleteEPlmnList����ֶ��������ܣ�������ѭ�����⡣*/
    ucPlmnSelMode = NAS_MMC_GetPlmnSelectionMode();

    if ((VOS_TRUE == NAS_MML_GetPsAttachAllowFlg())
     && (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == ucPlmnSelMode))
    {
        NAS_MML_SetEplmnValidFlg(VOS_TRUE);
    }

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    /* ��ǰ��GMM�ϱ���NET WORK DETACH IND��Ϣ���򷵻�LIMIT_CAMP_ON */
    if ( GMM_MMC_PS_NETWORK_DETACH_IND == enRegRsltType)
    {
        /* ��ȡCS addition,����NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON�Ƚϣ�
           �������Ҫ�������������PSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
        enPrioAdditionalAction = NAS_MMC_GetCsAdditionalAction();
        enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON, enPrioAdditionalAction);
        if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
        {
            NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }


        return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
    }

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    /* ����ע�����ϱ���Attempt Counter�Ƿ�ﵽ�������ֱ���д��� */
    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}

NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause11PlmnNotAllow(VOS_VOID)
{
    VOS_UINT8                           ucRejDomain;
    NAS_MML_MS_CFG_INFO_STRU           *pstMsCfg;
    NAS_MML_PLMN_ID_STRU               *pstCurPlmnId;

    pstCurPlmnId = NAS_MML_GetCurrCampPlmnId();

    pstMsCfg = NAS_MML_GetMsCfgInfo();

    /* ��ӽ�ֹ������Ϣ,�и���ʱ����Usim�����ļ� */
    if (VOS_TRUE == NAS_MML_AddForbPlmn(pstCurPlmnId))
    {
        NAS_MMC_SndUsimUpdateForbPlmnReq();
    }

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    /* פ���ڷ�HOME PLMN��ʱ��ע��ܾ�#11ʱ�������� */
    if (VOS_FALSE == NAS_MML_ComparePlmnIdWithHplmn(pstCurPlmnId))
    {
        /* ����ע��������ע����Ϣ�� */
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(),
                 NAS_MMC_REG_DOMAIN_PS, NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW);

        return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
    }

    if ( VOS_FALSE == NAS_MML_GetHplmnRegisterCtrlFlg())
    {
        /* ��E5�汾�������´���E5�汾Ҫ������HPLMN�Ϸ�������ע�� */
        /* ����ע��������ע����Ϣ�� */
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(),
                 NAS_MMC_REG_DOMAIN_PS, NAS_MML_REG_FAIL_CAUSE_PLMN_NOT_ALLOW);


        if ((NAS_MML_MS_MODE_PS_CS == pstMsCfg->stMsSysCfgInfo.enMsMode))
        {
            NAS_MMC_GetHplmnRejDomainInfo(&ucRejDomain);

            if (NAS_MMC_REG_DOMAIN_PS_CS != ucRejDomain)
            {
                NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);

                return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
            }
        }

    }
    else
    {
        /* EM��������:1��PS ONLY��2����ǰ��W��3���������#11ʱ,
           ��Ҫ��HOME PLMN��������ע�� */
        if ((NAS_MML_NET_RAT_TYPE_WCDMA ==  NAS_MML_GetCurrNetRatType())
         && (NAS_MML_MS_MODE_PS_ONLY == pstMsCfg->stMsSysCfgInfo.enMsMode))
        {
            NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);

            return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
        }
    }

    return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
}

NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause12LaNotAllow(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8          enCsAdditionalAction;

    /*��ӽ�ֹ������Ϣ*/
    NAS_MML_AddForbRegLa(NAS_MML_GetCurrCampLai());

    NAS_MMC_StartTimer(TI_NAS_MMC_FORBID_LA_TIMER, TI_NAS_MMC_FORBID_LA_TIMER_LEN);

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE_REGION);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, NAS_MMC_LIMITED_SERVICE_REGION);

    enCsAdditionalAction = NAS_MMC_GetCsAdditionalAction();
    if ((NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL != enCsAdditionalAction)
     && (NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION != enCsAdditionalAction)
     && (NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN != enCsAdditionalAction))
    {
        NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    return NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause13RoamNotAllow(VOS_VOID)
{
    NAS_MML_FORBIDPLMN_ROAMING_LAS_INFO_STRU                stForbRoamLaInfo;
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU                        *pstForbPlmnInfo = VOS_NULL_PTR;

    /* ���û�ָ����������ȫ�ֱ����л�ȡFORBIDEN ROAMING LAS��Ϣ */
    pstForbPlmnInfo                  = NAS_MML_GetForbidPlmnInfo();
    stForbRoamLaInfo.ucForbRoamLaNum = pstForbPlmnInfo->ucForbRoamLaNum;
    PS_MEM_CPY( stForbRoamLaInfo.astForbRomLaList,
                pstForbPlmnInfo->astForbRomLaList,
                sizeof(pstForbPlmnInfo->astForbRomLaList) );

    /* ��ӽ�ֹ������Ϣ��ȫ�ֱ����Ľ�ֹ����LA������ȥ */
    NAS_MML_AddForbRoamLa( NAS_MML_GetCurrCampLai(),
                           &stForbRoamLaInfo );
    pstForbPlmnInfo->ucForbRoamLaNum = stForbRoamLaInfo.ucForbRoamLaNum;
    PS_MEM_CPY( pstForbPlmnInfo->astForbRomLaList,
                stForbRoamLaInfo.astForbRomLaList,
                sizeof(pstForbPlmnInfo->astForbRomLaList));

    NAS_MMC_StartTimer(TI_NAS_MMC_FORBID_LA_TIMER, TI_NAS_MMC_FORBID_LA_TIMER_LEN);

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;

}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause14GprsNotAllowInThisPlmn(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    /* ��ӽ�ֹGPRS PLMN ��Ϣ */
    NAS_MML_AddForbGprsPlmn(NAS_MML_GetCurrCampPlmnId());

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    /* ����ж��Ƶ�#14�Ķ���������NV�ж��ƵĶ��� */
    enAction = NAS_MMC_GetSingleDomainFailAction(NAS_MML_REG_DOMAIN_PS, NAS_MML_REG_FAIL_CAUSE_GPRS_SERV_NOT_ALLOW_IN_PLMN);

    if (NAS_MMC_ADDITIONAL_ACTION_BUTT == enAction)
    {
        enAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    }

    /* ��ȡCS addition,����enAction�Ƚϣ��������Ҫ�������������PSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
    enPrioAdditionalAction = NAS_MMC_GetCsAdditionalAction();
    enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(enAction, enPrioAdditionalAction);
    if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
    {
        NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause15NoSuitableCell(VOS_VOID)
{
    NAS_MML_FORBIDPLMN_ROAMING_LAS_INFO_STRU                stForbRoamLaInfo;
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU                        *pstForbPlmnInfo = VOS_NULL_PTR;

    /* ���û�ָ����������ȫ�ֱ����л�ȡFORBIDEN ROAMING LAS��Ϣ */
    pstForbPlmnInfo                  = NAS_MML_GetForbidPlmnInfo();
    stForbRoamLaInfo.ucForbRoamLaNum = pstForbPlmnInfo->ucForbRoamLaNum;
    PS_MEM_CPY( stForbRoamLaInfo.astForbRomLaList,
                pstForbPlmnInfo->astForbRomLaList,
                sizeof(pstForbPlmnInfo->astForbRomLaList) );

    /* ��ӽ�ֹ������Ϣ��ȫ�ֱ����Ľ�ֹ����LA������ȥ */
    NAS_MML_AddForbRoamLa( NAS_MML_GetCurrCampLai(),
                           &stForbRoamLaInfo );
    pstForbPlmnInfo->ucForbRoamLaNum = stForbRoamLaInfo.ucForbRoamLaNum;
    PS_MEM_CPY( pstForbPlmnInfo->astForbRomLaList,
                stForbRoamLaInfo.astForbRomLaList,
                sizeof(pstForbPlmnInfo->astForbRomLaList));


    NAS_MMC_StartTimer(TI_NAS_MMC_FORBID_LA_TIMER, TI_NAS_MMC_FORBID_LA_TIMER_LEN);

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN;

}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause40NoPdpContextActived(VOS_VOID)
{
    return NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause95SemanticallyIncorrectMsg(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    /* ��ǰ��GMM�ϱ���PS SERVICE REG RSLT IND��Ϣ���򷵻�NORMAL_CAMP_ON */
    if ( GMM_MMC_PS_SERVICE_RESULT_IND ==  enRegRsltType)
    {
        return NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;
    }

    /* ��ǰ��GMM�ϱ���NET WORK DETACH IND��Ϣ���򷵻�LIMIT_CAMP_ON */
    if ( GMM_MMC_PS_NETWORK_DETACH_IND ==  enRegRsltType)
    {
        /* ��ȡCS addition,����NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON�Ƚϣ�
           �������Ҫ�������������PSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
        enPrioAdditionalAction = NAS_MMC_GetCsAdditionalAction();
        enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON, enPrioAdditionalAction);
        if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
        {
            NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
    }

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause96InvalidMandatoryInf(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    /* ��ǰ��GMM�ϱ���PS SERVICE REG RSLT IND��Ϣ���򷵻�NORMAL_CAMP_ON */
    if ( GMM_MMC_PS_SERVICE_RESULT_IND ==  enRegRsltType)
    {
        return NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;
    }

    /* ��ǰ��GMM�ϱ���NET WORK DETACH IND��Ϣ���򷵻�LIMIT_CAMP_ON */
    if ( GMM_MMC_PS_NETWORK_DETACH_IND ==  enRegRsltType)
    {
        /* ��ȡCS addition,����NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON�Ƚϣ�
           �������Ҫ�������������PSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
        enPrioAdditionalAction = NAS_MMC_GetCsAdditionalAction();
        enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON, enPrioAdditionalAction);
        if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
        {
            NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
    }

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause97MsgNonExistNotImplement(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    /* ��ǰ��GMM�ϱ���PS SERVICE REG RSLT IND��Ϣ���򷵻�NORMAL_CAMP_ON */
    if ( GMM_MMC_PS_SERVICE_RESULT_IND ==  enRegRsltType)
    {
        return NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;
    }

    /* ��ǰ��GMM�ϱ���NET WORK DETACH IND��Ϣ���򷵻�LIMIT_CAMP_ON */
    if ( GMM_MMC_PS_NETWORK_DETACH_IND ==  enRegRsltType)
    {
        /* ��ȡCS addition,����NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON�Ƚϣ�
           �������Ҫ�������������PSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
        enPrioAdditionalAction = NAS_MMC_GetCsAdditionalAction();
        enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON, enPrioAdditionalAction);
        if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
        {
            NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
    }

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause99IeNonExistNotImplement(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    /* ��ǰ��GMM�ϱ���PS SERVICE REG RSLT IND��Ϣ���򷵻�NORMAL_CAMP_ON */
    if ( GMM_MMC_PS_SERVICE_RESULT_IND ==  enRegRsltType)
    {
        return NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;
    }

    /* ��ǰ��GMM�ϱ���NET WORK DETACH IND��Ϣ���򷵻�LIMIT_CAMP_ON */
    if ( GMM_MMC_PS_NETWORK_DETACH_IND ==  enRegRsltType)
    {
        /* ��ȡCS addition,����NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON�Ƚϣ�
           �������Ҫ�������������PSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
        enPrioAdditionalAction = NAS_MMC_GetCsAdditionalAction();
        enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON, enPrioAdditionalAction);
        if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
        {
            NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
    }

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause111ProtocolError(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    /* ��ǰ��GMM�ϱ���PS SERVICE REG RSLT IND��Ϣ���򷵻�NORMAL_CAMP_ON */
    if ( GMM_MMC_PS_SERVICE_RESULT_IND == enRegRsltType)
    {
        return NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;
    }

    /* ��ǰ��GMM�ϱ���NET WORK DETACH IND��Ϣ���򷵻�LIMIT_CAMP_ON */
    if ( GMM_MMC_PS_NETWORK_DETACH_IND == enRegRsltType)
    {
        /* ��ȡCS addition,����NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON�Ƚϣ�
           �������Ҫ�������������PSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
        enPrioAdditionalAction = NAS_MMC_GetCsAdditionalAction();
        enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON, enPrioAdditionalAction);
        if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
        {
            NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
    }

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause256AuthRej(VOS_VOID)
{
#if   (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                          ulDisablteRoamFlg;

    ulDisablteRoamFlg = NAS_MML_GetDisableLteRoamFlg();
#endif

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);
    NAS_MMC_ChangePsRegState(NAS_MML_REG_REGISTERED_DENIED);

#if   (FEATURE_ON == FEATURE_LTE)
    if (VOS_TRUE == NAS_MMC_IsNeedEnableLte_PsRegFailCause256AuthRej())
    {
        /* ������ڽ�ֹLTE�������ε��µ�disable LTE����δ�ص�����֮ǰ��ʱҲ��֪ͨenable LTE */
        if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == NAS_MML_GetLteCapabilityStatus())
         && (VOS_FALSE == ulDisablteRoamFlg))
        {
            /* ��WAS/GAS����enable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);

            /* ��LMM����enable LTE��Ϣ */
            NAS_MMC_SndLmmEnableLteNotify();
        }

        if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER))
        {
            NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER);
        }

        /* ����disable LTE������� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }
#endif

    return NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause257AccessBarrd(VOS_VOID)
{
    NAS_MML_ROUTING_UPDATE_STATUS_ENUM_UINT8    enPsUpdateStatus;

    enPsUpdateStatus = NAS_MML_GetPsUpdateStatus();

    /* �����ǰRAI���ϴγɹ�ע���RAI��ͬ��UPDATE״̬ΪU1������ע��Ϊ1��5��
       ����ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
    if ((VOS_FALSE == NAS_MML_IsRaiChanged())
     && (NAS_MML_ROUTING_UPDATE_STATUS_UPDATED == enPsUpdateStatus)
     && (GMM_STATUS_ATTACHED == (NAS_MMC_GetGmmCtx())->ucAttachSta))
    {
        NAS_MMC_UpdatePsRegStatePsRegSucc();
    }
    else
    {
        NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    /*���Կ��ش��ҷ�HPLMN/RPLMN�����ֹʱ��Ҫ����PLMN����*/
    if (VOS_TRUE == NAS_MMC_IsAccBarPlmnSearch())
    {
        return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    }
    else
    {
        return NAS_MMC_ADDITIONAL_ACTION_ACCESS_BARRED;
    }
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause301TimerTimeOut(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    /* ��ǰ��GMM�ϱ���PS SERVICE REG RSLT IND��Ϣ���򷵻�NORMAL_CAMP_ON */
    if ( GMM_MMC_PS_SERVICE_RESULT_IND == enRegRsltType)
    {
        return NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;
    }

    /* ��ǰ��GMM�ϱ���NET WORK DETACH IND��Ϣ���򷵻�LIMIT_CAMP_ON */
    if ( GMM_MMC_PS_NETWORK_DETACH_IND == enRegRsltType)
    {
        /* ��ȡCS addition,����NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON�Ƚϣ�
           �������Ҫ�������������PSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
        enPrioAdditionalAction = NAS_MMC_GetCsAdditionalAction();
        enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON, enPrioAdditionalAction);
        if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
        {
            NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
    }

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause302RrConnEstFail(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    /* ��ǰ��GMM�ϱ���PS SERVICE REG RSLT IND��Ϣ���򷵻�NORMAL_CAMP_ON */
    if ( GMM_MMC_PS_SERVICE_RESULT_IND == enRegRsltType)
    {
        return NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;
    }

    /* ��ǰ��GMM�ϱ���NET WORK DETACH IND��Ϣ���򷵻�LIMIT_CAMP_ON */
    if ( GMM_MMC_PS_NETWORK_DETACH_IND == enRegRsltType)
    {
        /* ��ȡCS addition,����NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON�Ƚϣ�
           �������Ҫ�������������PSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
        enPrioAdditionalAction = NAS_MMC_GetCsAdditionalAction();
        enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON, enPrioAdditionalAction);
        if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
        {
            NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
    }

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause303RrConnFail(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;

    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    /* ��ǰ��GMM�ϱ���PS SERVICE REG RSLT IND��Ϣ���򷵻�NORMAL_CAMP_ON */
    if ( GMM_MMC_PS_SERVICE_RESULT_IND == enRegRsltType)
    {
        return NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;
    }

    /* ��ǰ��GMM�ϱ���NET WORK DETACH IND��Ϣ���򷵻�LIMIT_CAMP_ON */
    if ( GMM_MMC_PS_NETWORK_DETACH_IND == enRegRsltType)
    {
        /* ��ȡCS addition,����NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON�Ƚϣ�
           �������Ҫ�������������PSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
        enPrioAdditionalAction = NAS_MMC_GetCsAdditionalAction();
        enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON, enPrioAdditionalAction);
        if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
        {
            NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
    }

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause401OtherCause(VOS_VOID)
{
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    ulAttemptCnt  = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    /* ��ȡCS��PS��additon action��ȡ�����ȼ����ж��Ƿ���Ҫ������
       �������Ҫ����������Ps counter�Ѿ��ﵽ���
       �����PSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
    if (ulAttemptCnt >= NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        enPsAdditionalAction   = NAS_MMC_GetPsAdditionalAction();
        enCsAdditionalAction   = NAS_MMC_GetCsAdditionalAction();

        enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(enCsAdditionalAction, enPsAdditionalAction);
        if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
        {
            NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }
    }

    /* ����NAS_MMC_ADDITIONAL_ACTION_BUTT����ʾ��Ч��AdditionalActionֵ */
    return NAS_MMC_ADDITIONAL_ACTION_BUTT;
}

NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause402MsCfgDomainNotSupport(VOS_VOID)
{
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;
    NAS_MMC_TIMER_STATUS_ENUM_U8                            enAvailableTimerStatus;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsAdditionalAction;
    VOS_UINT8                                               ucCsAttachAllow;

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);


    /* ��ǰ��ONPLMNʱ�����յ����ΪPS��֧�֣�����Ҫ�ж��Ƿ���Ҫ����:
       1)CS���Ƿ���LIMIT_CAMP_ON 2)AVAILABLE��ʱ��δ���� 3)CS��׼����ע��,��MMC��Ҫ��������
    */
    enFsmId                = NAS_MMC_GetCurrFsmId();
    enCsAdditionalAction   = NAS_MMC_GetCsRegAdditionalAction_L1Main();
    enAvailableTimerStatus = NAS_MMC_GetTimerStatus(TI_NAS_MMC_AVAILABLE_TIMER);
    ucCsAttachAllow        = NAS_MML_GetCsAttachAllowFlg();

    if ( (NAS_MMC_FSM_L1_MAIN   == enFsmId)
      || (NAS_MMC_FSM_PLMN_LIST == enFsmId) )
    {
        if (VOS_FALSE == NAS_MMC_IsNeedPlmnSearch_RegLocalFailCause())
        {
            return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
        }

        if ( (VOS_TRUE                                  == ucCsAttachAllow)
          && (NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON == enCsAdditionalAction)
          && (NAS_MMC_TIMER_STATUS_STOP                 == enAvailableTimerStatus) )
        {
            return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
        }
    }

    return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause403ForbiddenPlmn(VOS_VOID)
{
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;
    NAS_MMC_TIMER_STATUS_ENUM_U8                            enAvailableTimerStatus;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstUserSpecPlmn     = VOS_NULL_PTR;
    VOS_UINT32                                              ulUserSpecPlmnFlag;
    NAS_MML_PLMN_ID_STRU                                   *pstCurPlmnId        = VOS_NULL_PTR;

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);


    /* �ֶ�ģʽ�жϵ�ǰ�����Ƿ����û�ָ�������� */
    pstCurPlmnId       = NAS_MML_GetCurrCampPlmnId();
    pstUserSpecPlmn    = NAS_MMC_GetUserSpecPlmnId();
    ulUserSpecPlmnFlag = NAS_MML_CompareBcchPlmnwithSimPlmn(pstCurPlmnId,
                                                &(pstUserSpecPlmn->stPlmnId));

    enFsmId = NAS_MMC_GetCurrFsmId();

    switch ( enFsmId )
    {
        case NAS_MMC_FSM_L1_MAIN :
        case NAS_MMC_FSM_PLMN_LIST :

            if (VOS_FALSE == NAS_MMC_IsNeedPlmnSearch_RegLocalFailCause())
            {
                return NAS_MMC_ADDITIONAL_ACTION_BUTT;
            }

            /* Ĭ�Ϸ���NAS_MMC_ADDITIONAL_ACTION_BUTT����ʾ��Ч��AdditionalActionֵ */
            enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;

            /* �����³�����Ҫ����:�ֶ�ģʽ�£���ǰPlmn�����û�ָ����PLMN,�ҵ�ǰPLMN����EPlmn�б��
               Available��ʱ��δ����������Ϊ��ǰplmn �Ѿ�ʧЧ�ˣ���Ҫ�������� */

            /* �ֶ�ģʽ�жϵ�ǰ�����Ƿ����û�ָ�������� */
            pstCurPlmnId       = NAS_MML_GetCurrCampPlmnId();
            pstUserSpecPlmn    = NAS_MMC_GetUserSpecPlmnId();
            ulUserSpecPlmnFlag = NAS_MML_CompareBcchPlmnwithSimPlmn(pstCurPlmnId,
                                                        &(pstUserSpecPlmn->stPlmnId));

            enAvailableTimerStatus = NAS_MMC_GetTimerStatus(TI_NAS_MMC_AVAILABLE_TIMER);

            /* �ֶ�ģʽ�£���ǰPlmn�����û�ָ����PLMN,�ҵ�ǰPLMN����EPlmn�б��
               ����Ϊ��ǰplmn �Ѿ�ʧЧ�ˣ�Available ��ʱ��δ��������Ҫ�������� */
            if ( (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
              && (VOS_FALSE                          == ulUserSpecPlmnFlag)
              && (NAS_MMC_TIMER_STATUS_STOP          == enAvailableTimerStatus) )
            {
                enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
            }
            break;

        /* ������Ҫ�������� */
        default:
            enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
            break;
    }

    return enAdditionalAction;

}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause404ForbiddenLaForRegPrvsSvs(VOS_VOID)
{
    /* ��ע�ᱻ��ԭ��#12�Ĵ���һ�� */
    return NAS_MMC_ProcPsRegFailCause12LaNotAllow();
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause405ForbiddenLaForRoam(VOS_VOID)
{
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsCause;                              /* �洢ע����CS�򱻾���Ϣ */
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsCause;                              /* �洢ע����PS�򱻾���Ϣ */

    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);


    /* ��ǰ��ONPLMN����PLMN LIST״̬������ʱ��������פ��̬����δ��������ʵ��������������£�
       �����µ�ǰAdditionalActionֵ */
    enFsmId = NAS_MMC_GetCurrFsmId();

    if ( (NAS_MMC_FSM_L1_MAIN   == enFsmId)
      || (NAS_MMC_FSM_PLMN_LIST == enFsmId) )
    {
        /* ����NAS_MMC_ADDITIONAL_ACTION_BUTT����ʾ��Ч��AdditionalActionֵ,MMC
           ��������ЧAdditionalActionֵ */
        return NAS_MMC_ADDITIONAL_ACTION_BUTT;
    }

    /* ��ǰ������ONPLMN����PLMN LIST״̬�����账��ܾ�ԭ��ֵ�����ز�ͬ���������� */

    /* ԭ��ֵ��ʼ��Ϊ#13,�������쳣������г�������� */
    enCsCause = NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW;
    enPsCause = NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW;

    NAS_MMC_GetDomainRegStatusInRegInfoList( NAS_MML_GetCurrCampPlmnId(),
                                             NAS_MML_GetCurrNetRatType(),
                                             &enCsCause,
                                             &enPsCause );

    /* ��CS�����PS����һ�����ԭ��ֵΪ#13,����г�������� */
    if ( (NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW == enCsCause)
      || (NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW == enPsCause))
    {
        return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
    }

    /* �����վܾ�ԭ��ֵ#15���������Խ������� */
    return NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN;


}



NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause406ForbiddenPlmnForGprs(VOS_VOID)
{
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    /* ��ӽ�ֹGPRS PLMN ��Ϣ */
    NAS_MML_AddForbGprsPlmn(NAS_MML_GetCurrCampPlmnId());

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    enFsmId = NAS_MMC_GetCurrFsmId();

    switch ( enFsmId )
    {
        /* ��ǰ��ONPLMN,�����ĵ�ǰ��ADDITIONAL ACTION��ֵ */
        case NAS_MMC_FSM_L1_MAIN :
            enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;
            break;

        /* ��ǰ��PLMN LIST,�����ĵ�ǰ��ADDITIONAL ACTION��ֵ*/
        case NAS_MMC_FSM_PLMN_LIST :
            enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;
            break;

        /* ������Ҫ���п�ѡ������ */
        default:
            enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
            break;
    }

    /* ��ȡCS addition,�������Ҫ�������������PSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
    enPrioAdditionalAction = NAS_MMC_GetCsAdditionalAction();
    enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(enAdditionalAction, enPrioAdditionalAction);
    if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
    {
        NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    return enAdditionalAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause407SimInvalid(VOS_VOID)
{
    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    /* �����ڱ�ʾ�϶���ע�ᱻ�ܹ� */
    if (VOS_TRUE == NAS_MML_GetSimPresentStatus())
    {
        NAS_MMC_ChangePsRegState(NAS_MML_REG_REGISTERED_DENIED);
    }
    else
    {
        NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }


    enFsmId = NAS_MMC_GetCurrFsmId();
    if ( (NAS_MMC_FSM_L1_MAIN == enFsmId)
      || (NAS_MMC_FSM_PLMN_LIST == enFsmId) )
    {
        /* ����NAS_MMC_ADDITIONAL_ACTION_BUTT����ʾ��Ч��AdditionalActionֵ,MMC
           ��������ЧAdditionalActionֵ */
        return NAS_MMC_ADDITIONAL_ACTION_BUTT;
    }

    return NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL;
}




NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause408NetDetachImsi(VOS_VOID)
{
    /* ������ЧACTIONֵ:NAS_MMC_ADDITIONAL_ACTION_BUTT,�����ĵ�ǰ��ADDITIONAL ACTION��ֵ*/
    return NAS_MMC_ADDITIONAL_ACTION_BUTT;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCause409ServingCellDomainNotSupport(VOS_VOID)
{

    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;
    NAS_MMC_TIMER_STATUS_ENUM_U8                            enAvailableTimerStatus;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    enFsmId                = NAS_MMC_GetCurrFsmId();
    enAvailableTimerStatus = NAS_MMC_GetTimerStatus(TI_NAS_MMC_AVAILABLE_TIMER);

    if ( (NAS_MMC_FSM_L1_MAIN   == enFsmId)
      || (NAS_MMC_FSM_PLMN_LIST == enFsmId) )
    {
        if (VOS_FALSE == NAS_MMC_IsNeedPlmnSearch_RegLocalFailCause())
        {
            return NAS_MMC_ADDITIONAL_ACTION_BUTT;
        }

        /* ��ǰAVAILABLE��ʱ�������У�����פ�� */
        if ( NAS_MMC_TIMER_STATUS_RUNING               == enAvailableTimerStatus )
        {
            return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
        }


        /* ��L1 MAINʱ�������ǰ���޿�����PS���ز���Ҫ���� */
        if ( VOS_FALSE == NAS_MML_GetSimPresentStatus() )
        {
            /* ����NAS_MMC_ADDITIONAL_ACTION_BUTT����ʾ��Ч��AdditionalActionֵ,MMC
                ��������ЧAdditionalActionֵ */
            return NAS_MMC_ADDITIONAL_ACTION_BUTT;
        }
    }



    enAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    
    /* SOR���ֶ�ģʽ����ʱ�򣬷���LIMIT CAMP ON */
    if (VOS_TRUE == NAS_MML_GetCsRejSearchSupportFlg())
    {
        if (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
        {
            enAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
        }        
    }   

    /* ��ȡCS addition,����NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION�Ƚϣ�
       �������Ҫ�������������PSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
    enPrioAdditionalAction = NAS_MMC_GetCsAdditionalAction();

    enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(enAction, enPrioAdditionalAction);
    
    if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
    {
        NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    return enAction;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailCauseAbnormalCause(
    VOS_UINT16                          usCause,
    VOS_UINT32                          ulAttemptCnt
)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;

    /* ע��ʧ��Attempt Counterδ�ﵽ������ */
    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterLessThanMaxTimes();
    }
    else
    {
        /* ע��ʧ��Attempt Counter�ﵽ�򳬹��������Ĵ��� */
        enAction = NAS_MMC_ProcPsRegFailAttemptCounterMaxTimes();
    }

    return enAction;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailAttemptCounterMaxTimes(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT8                                               ucSingleDomainSrchFlag;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    NAS_MMC_PLMN_SELECTION_MODE_ENUM_UINT8                  enSelectionMode;
    
    enSelectionMode = NAS_MMC_GetPlmnSelectionMode();

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    /* �����������Ʊ�ʶ */
    ucSingleDomainSrchFlag = NAS_MML_GetDtSingleDomainPlmnSearchFlag();


    enAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    
    /* SOR���ֶ�ģʽ����ʱ�򣬷���LIMIT CAMP ON */
    if ( (VOS_TRUE == NAS_MML_GetCsRejSearchSupportFlg())
      && (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == enSelectionMode) )
    {
        enAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;       
    }
    /* ������ʧ�ܱ����Ƿ���������������DT�����������ƴ�ʱ����; ����ʱ���ȴ�PS��ע���� */    
    else if (VOS_TRUE == ucSingleDomainSrchFlag)
    {
        enAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
    }
    else
    {
        enAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    }

    /* ��ȡCS addition,����enAction�Ƚϣ�
       �������Ҫ�������������PSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
    enPrioAdditionalAction = NAS_MMC_GetCsAdditionalAction();
    enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(enAction, enPrioAdditionalAction);
    if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
    {
        NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFailAttemptCounterLessThanMaxTimes(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEplmnInfo        = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstUserSpecPlmn     = VOS_NULL_PTR;
    VOS_UINT32                                              ulInEplmnListFlag;
    VOS_UINT32                                              ulUserSpecPlmnFlag;
    NAS_MML_PLMN_ID_STRU                                   *pstCurPlmnId        = VOS_NULL_PTR;
    VOS_UINT32                                              ulLaiNoChangeFlag;


    NAS_MML_CAMP_PLMN_INFO_STRU        *pstCurCampInfo   = VOS_NULL_PTR;
    NAS_MML_RAI_STRU                   *pstPsLastSuccRai = VOS_NULL_PTR;

    NAS_MML_REG_STATUS_ENUM_UINT8       enPsRegStatus;

    enPsRegStatus = NAS_MML_GetPsRegStatus();

    /* Ĭ�����õ�ǰ�����ȴ�ע���� */
    enAction = NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;

    pstCurCampInfo   = NAS_MML_GetCurrCampPlmnInfo();
    pstPsLastSuccRai = NAS_MML_GetPsLastSuccRai();

    ulLaiNoChangeFlag = NAS_MML_IsLaiInDestLaiList( NAS_MML_GetCurrCampLai(),
                                                   0x01,
                                                   &(pstPsLastSuccRai->stLai) );

    /* �����ǰ��RAI�����ı䣬����µ�ǰ��Ϊ���Ʒ���״̬ */
    if ( ( VOS_FALSE             == ulLaiNoChangeFlag )
      || ( pstCurCampInfo->ucRac != pstPsLastSuccRai->ucRac ) )
    {
        /* ���·���״̬ */
        NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

        /* ֪ͨTAF��ǰ�ķ���״̬ */
        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

        /* ���ps֮ǰע��ɹ���λ�����ı䣬ע��δ�ﵽ����������Ҫ��ע��״̬����Ϊ0 */
        if ((NAS_MML_REG_REGISTERED_HOME_NETWORK == enPsRegStatus)
         || (NAS_MML_REG_REGISTERED_ROAM == enPsRegStatus))
        {
            NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }
    }

    /* �Զ�ģʽʱ��ֱ�ӷ���NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT*/
    if ( NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode() )
    {
        return enAction;
    }

    /* �жϵ�ǰ�����Ƿ����û�ָ�������� */
    pstCurPlmnId       = NAS_MML_GetCurrCampPlmnId();
    pstUserSpecPlmn    = NAS_MMC_GetUserSpecPlmnId();
    ulUserSpecPlmnFlag = NAS_MML_CompareBcchPlmnwithSimPlmn(pstCurPlmnId,
                                                &(pstUserSpecPlmn->stPlmnId));

    /* �жϵ�ǰ�����Ƿ���EPlmn�б��� */
    pstEplmnInfo       = NAS_MML_GetEquPlmnList();

    ulInEplmnListFlag  = NAS_MML_IsBcchPlmnIdInDestSimPlmnList(pstCurPlmnId,
                                              pstEplmnInfo->ucEquPlmnNum,
                                              pstEplmnInfo->astEquPlmnAddr);

    /* �ֶ�ģʽ�£���ǰPlmn�����û�ָ����PLMN,�ҵ�ǰPLMN����EPlmn�б��
       ����Ϊ��ǰplmn �Ѿ�ʧЧ�ˣ���Ҫ�������� */
    if ( (VOS_FALSE == ulUserSpecPlmnFlag)
      && (VOS_FALSE == ulInEplmnListFlag) )
    {
        enAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    }

    return enAction;
}

#if   (FEATURE_ON == FEATURE_LTE)

VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause3IllegalMs(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ����cs ps��״̬Ϊ��Ч */
    NAS_MML_SetSimCsRegStatus(VOS_FALSE);
    NAS_MML_SetSimPsRegStatus(VOS_FALSE);

    NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,VOS_FALSE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);

    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL;
    *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL;

    return;
}



VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause6IllegalMe(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    NAS_MMC_ProcCombinedEpsRegFailCause3IllegalMs(penPsRegAdditionalAction, penCsRegAdditionalAction);

    return;
}



VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause7EpsServNotAllow(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstRatOrder;

    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    pstRatOrder     = NAS_MML_GetMsPrioRatList();

    NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,VOS_FALSE);

    /* ����ps��״̬Ϊ��Ч */
    NAS_MML_SetSimPsRegStatus(VOS_FALSE);

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    /* �ж��Ƿ���Ҫdisable L */
    if (VOS_TRUE == NAS_MMC_IsNeedDisableL_PsFailCause7PsServNotAllow())
    {
        /* ��¼����disable LTE��� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS);

        NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_PS_USIM_INVALID);
    }

    /* �����L��ģ����ԭ��ֵ7����Ҫanycell����,��ģ�����Ҫ���� */
    if (VOS_TRUE == NAS_MML_IsLteOnlyMode(pstRatOrder))
    {
        *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL;
        *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL;
    }
    else
    {
        *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL;
        *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
    }

    return;
}


VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause8GprsAndNonGprsNotAllow(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    NAS_MMC_ProcCombinedEpsRegFailCause3IllegalMs(penPsRegAdditionalAction, penCsRegAdditionalAction);
    return;
}


VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause9MsidCannotDerivedByNw(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
    *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
}
VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause10ImplicitlyDetached(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
    *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
}
VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause11PlmnNotAllow(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    NAS_MML_PLMN_ID_STRU               *pstCurPlmnId = VOS_NULL_PTR;

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    pstCurPlmnId = NAS_MML_GetCurrCampPlmnId();

    /* ��ӽ�ֹ������Ϣ,�и���ʱ����Usim�����ļ� */
    if (VOS_TRUE == NAS_MML_AddForbPlmn(pstCurPlmnId))
    {
        NAS_MMC_SndUsimUpdateForbPlmnReq();
    }

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;

    *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;

    return;
}


VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause12TaNotAllow(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE_REGION);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, NAS_MMC_LIMITED_SERVICE_REGION);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);

    *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION;

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE_REGION);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, NAS_MMC_LIMITED_SERVICE_REGION);

    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION;
}


VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause406ForbiddenPlmnForGprs(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, NAS_MMC_LIMITED_SERVICE);

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, NAS_MMC_LIMITED_SERVICE);

    /* ��ǰ��ONPLMN����PLMN LIST״̬������δ��������ʵ��������������£�
       ����NAS_MMC_ADDITIONAL_ACTION_BUTT����ʾ��Ч��AdditionalActionֵ��MMC�����µ�ǰADDITIONAL ACTION��ֵ */
    enFsmId = NAS_MMC_GetCurrFsmId();

    if ( (NAS_MMC_FSM_L1_MAIN   == enFsmId)
      || (NAS_MMC_FSM_PLMN_LIST == enFsmId) )
    {
        *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;
        *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;
        return ;
    }

    /* ��ǰ������ONPLMN����PLMN LIST״̬�����账��ܾ�ԭ��ֵ�����ؿ�ѡ���������� */
    *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
}
VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause13RoamNotAllow(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;

    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;

}


VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause14GprsNotAllowInThisPlmn(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    /* ��ӽ�ֹGPRS PLMN ��Ϣ */
    NAS_MML_AddForbGprsPlmn(NAS_MML_GetCurrCampPlmnId());

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;

    *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
}
VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause15NoSuitableCell(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
     /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN;


    *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN;
}


VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause19EsmFailure(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    VOS_UINT32                          ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8    enRegRsltType;
    enRegRsltType = NAS_MMC_GetRegRsltType();

    /* ȡ��Attempt Counter��ֵ */
    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    /* cs ps���ע��ʧ��,��ǰ������ONPLMN����PLMN LIST״̬���������AttemptCounter
       �����ز�ͬ��ADDITIONAL ACTIONֵ */
    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        /* ����EPSע��ʧ��Attemptδ�ﵽ�������Ĵ���Ĵ��� */
        NAS_MMC_ProcCombinedEpsRegFailAttemptCouterLessThanMaxTimes(penPsRegAdditionalAction,
                                                                    penCsRegAdditionalAction);
    }
    else
    {
        /* ɾ��EPLMN */
        NAS_MMC_DeleteEPlmnList();

        /* ���·���״̬ */
        NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);
        NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

        /* ֪ͨTAF��ǰ�ķ���״̬ */
        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

        if ( VOS_TRUE == NAS_MMC_IsNeedDisableL_CombinedRegRsltFailMaxTimes() )
        {
            /* ��ҪDisable Lȫ�ֱ��� */
            NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS);

            NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_LTE_VOICE_NOT_AVAILABLE);

            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER, NAS_MML_GetCsPsMode1EnableLteTimerLen());

            *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
            *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
        }
        else
        {
            *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
            *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
        }
    }

    return;
}


VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause35ReqServOptNotAuthorizedInThisPlmn(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    NAS_MML_MS_3GPP_REL_STRU                               *pstMs3GppRel = VOS_NULL_PTR;

    pstMs3GppRel = NAS_MML_GetMs3GppRel();
    if ( NAS_MML_3GPP_REL_R9 >= pstMs3GppRel->enLteNasRelease )
    {
        NAS_MMC_ProcCombinedEpsRegFailCauseAbnormalCause(penPsRegAdditionalAction, penCsRegAdditionalAction);
    }
    else
    {
        NAS_MMC_ProcCombinedEpsRegFailCause11PlmnNotAllow(penPsRegAdditionalAction, penCsRegAdditionalAction);
    }

    return;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCombinedEpsRegOnlyEpsSuccCause16MscNotReachable(VOS_VOID)
 {
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    VOS_UINT32                                              ulLaiNoChangeFlag;
    NAS_MML_REG_STATUS_ENUM_UINT8                           enCsRegStatus;

    enCsRegStatus = NAS_MML_GetCsRegStatus();

    /* ȡ�õ�ǰ��ע��������  */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_NORMAL_SERVICE);


    /*
    1)  attemptCount < MaxCountʱ������NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT
    2)  attemptCount >= MaxCountʱ������NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION
    */

    /* ps��ע��ɹ���cs��ע��ʧ�� */
    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        ulLaiNoChangeFlag = NAS_MML_IsLaiInDestLaiList( NAS_MML_GetCurrCampLai(),
                                                        0x01,
                                                        NAS_MML_GetCsLastSuccLai() );

        /* �����ǰ��LAI�����ı䣬����µ�ǰΪ���Ʒ���״̬ */
        if ((VOS_FALSE == ulLaiNoChangeFlag)
         && ((NAS_MML_REG_REGISTERED_HOME_NETWORK == enCsRegStatus)
          || (NAS_MML_REG_REGISTERED_ROAM == enCsRegStatus)))
        {
            /* ����ע��״̬Ϊ0 */
            NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        return NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
    }
    else
    {
        /* ���·���״̬ */
        NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

        /* ֪ͨTAF��ǰ�ķ���״̬ */
        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);

        return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    }


}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCombinedEpsRegOnlyEpsSuccCause17NetworkFailure(VOS_VOID)
{
    return NAS_MMC_ProcCombinedEpsRegOnlyEpsSuccCause16MscNotReachable();
}



NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCombinedEpsRegOnlyEpsSuccCause22Congestion(VOS_VOID)
{
    return NAS_MMC_ProcCombinedEpsRegOnlyEpsSuccCause16MscNotReachable();
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCombinedEpsRegOnlyEpsSuccCause18CsDomainNotAvailable(VOS_VOID)
{
    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_NORMAL_SERVICE);
    NAS_MMC_SetCsServiceStatus(NAS_MMC_NO_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_NO_SERVICE);


    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE);

        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);

    return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
    
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCombinedEpsRegOnlyEpsSuccCause2ImsiUnknownInHss(VOS_VOID)
{
    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_NORMAL_SERVICE);
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ����cs��״̬Ϊ��Ч */
    NAS_MML_SetSimCsRegStatus(VOS_FALSE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE);
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);

    return NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL;
}
VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause257AccessBarrd(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
     /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);

    /*���Կ��ش��ҷ�HPLMN/RPLMN�����ֹʱ��Ҫ����PLMN����*/
    if (VOS_TRUE == NAS_MMC_IsAccBarPlmnSearch())
    {
        *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
        *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    }
    else
    {
        *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_ACCESS_BARRED;

        *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_ACCESS_BARRED;
    }

}
VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause256AuthRej(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ����cs ps��״̬Ϊ��Ч */
    NAS_MML_SetSimCsRegStatus(VOS_FALSE);
    NAS_MML_SetSimPsRegStatus(VOS_FALSE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);

    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL;

    *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL;
}


VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause301TimerTimeOut(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsRegAdditionalAction
)
{
    VOS_UINT32                          ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    /* ȡ�õ�ǰ��ע������Ϣ���� */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType );

    /* cs ps���ע��ʧ��,��ǰ������ONPLMN����PLMN LIST״̬���������AttemptCounter
       �����ز�ͬ��ADDITIONAL ACTIONֵ */
    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        /* EPS����ע��ʧ��Attempt Counterδ�ﵽ���ʧ�ܴ����Ĵ��� */
        NAS_MMC_ProcCombinedEpsRegFailAttemptCouterLessThanMaxTimes(penPsRegAdditionalAction,
                                                                    penCsRegAdditionalAction);
    }
    else
    {
        /* ɾ��EPLMN */
        NAS_MMC_DeleteEPlmnList();

        /* ���·���״̬ */
        NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);
        NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

        /* ֪ͨTAF��ǰ�ķ���״̬ */
        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

        if ( VOS_TRUE == NAS_MMC_IsNeedDisableL_CombinedRegRsltFailMaxTimes() )
        {
            /* ��ҪDisable Lȫ�ֱ��� */
            NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS);

            NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_LTE_VOICE_NOT_AVAILABLE);

            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER, NAS_MML_GetCsPsMode1EnableLteTimerLen());

            *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
            *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
        }
        else
        {
            *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
            *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
        }
    }
    return;
}
VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause403ForbiddenPlmn(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);


    /* ��ǰ��ONPLMN����PLMN LIST״̬�����򷵻ص�ΪNAS_MMC_ADDITIONAL_ACTION_BUTT */
    enFsmId = NAS_MMC_GetCurrFsmId();

    if ( (NAS_MMC_FSM_L1_MAIN   == enFsmId)
      || (NAS_MMC_FSM_PLMN_LIST == enFsmId) )
    {
        *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;
        *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;
        return ;
    }

    /* ��ǰ������ONPLMN����PLMN LIST״̬��������������� */
    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;

    *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
}
VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause404ForbiddenTaForRegPrvsSvs(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    /* ������ע��ʧ��ԭ��ֵ#12�Ĵ���һ�� */
    NAS_MMC_ProcCombinedEpsRegFailCause12TaNotAllow(penPsRegAdditionalAction,
                                                    penCsRegAdditionalAction);
}
VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause405ForbiddenLaForRoam(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsRegAdditionalAction
)
{
    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;

    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsCause;                              /* �洢ע����CS�򱻾���Ϣ */
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsCause;                              /* �洢ע����PS�򱻾���Ϣ */

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);


    /* ��ǰ��ONPLMN����PLMN LIST״̬������δ��������ʵ��������������£�
       ����NAS_MMC_ADDITIONAL_ACTION_BUTT����ʾ��Ч��AdditionalActionֵ��MMC�����µ�ǰADDITIONAL ACTION��ֵ */
    enFsmId = NAS_MMC_GetCurrFsmId();

    if ( (NAS_MMC_FSM_L1_MAIN   == enFsmId)
      || (NAS_MMC_FSM_PLMN_LIST == enFsmId) )
    {
        *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;
        *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;
        return ;
    }

    /* ��ǰ������ONPLMN����PLMN LIST״̬�����账��ܾ�ԭ��ֵ�����ز�ͬ���������� */

    /* ԭ��ֵ��ʼ��Ϊ#13,�������쳣������#13���г�������� */
    enCsCause = NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW;
    enPsCause = NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW;

    NAS_MMC_GetDomainRegStatusInRegInfoList( NAS_MML_GetCurrCampPlmnId(),
                                             NAS_MML_GetCurrNetRatType(),
                                             &enCsCause,
                                             &enPsCause );

    /* ��CS�����PS����һ�����ԭ��ֵΪ#13,����г�������� */
    if ( (NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW == enCsCause)
      || (NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW == enPsCause))
    {
        *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
        *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
        return ;
    }

    /* �����վܾ�ԭ��ֵ#15���������Խ������� */
    *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN;
    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN;

    return;
}

VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCausePsAttachNotAllow(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;

    *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;

}


VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause40NoEpsBearerActived(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
    *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
    return;
}
VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCauseEpsBearerEstFail(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;

    return;
}


VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCauseMoDetach(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    VOS_UINT8                           ucCsAttachAllowFlg;

    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;

    /* ���������Detach ps��CS����Ч��Ҫ��������������פ�� */
    ucCsAttachAllowFlg = NAS_MML_GetCsAttachAllowFlg();
    if ((VOS_TRUE == NAS_MML_GetSimCsRegStatus())
     && (VOS_TRUE == ucCsAttachAllowFlg))
    {
        *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    }
    else
    {
        *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
    }

    return;
}
VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCauseMtDetach(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;

    return;
}


VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCauseT3402Running(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;
    NAS_MMC_TIMER_STATUS_ENUM_U8                            enAvailableTimerStatus;

    enFsmId                = NAS_MMC_GetCurrFsmId();
    enAvailableTimerStatus = NAS_MMC_GetTimerStatus(TI_NAS_MMC_AVAILABLE_TIMER);

    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);
    
    /* ��ǰ��ONPLMNʱ��AVAILABLE��ʱ��δ����,��MMC��Ҫ��������*/
    if ( NAS_MMC_FSM_L1_MAIN   == enFsmId)
    {
        if (NAS_MMC_TIMER_STATUS_RUNING == enAvailableTimerStatus)
        {
            *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
            *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;

            return;
        }
    }

    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;

    return;
}




NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCause3IllegalMs(VOS_VOID)
{

    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,VOS_FALSE);

    /* �ȸ���ȫ�ֱ���,���ڷ�����Ϣʱȫ�ֱ���ʱ���µ� */
    /* ����cs ps��״̬Ϊ��Ч */
    NAS_MML_SetSimCsRegStatus(VOS_FALSE);
    NAS_MML_SetSimPsRegStatus(VOS_FALSE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);


    return NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL;


}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCause6IllegalMe(VOS_VOID)
{

    return NAS_MMC_ProcEpsAttachRegFailCause3IllegalMs();

}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCause7EpsServNotAllow(VOS_VOID)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstRatOrder;
    NAS_MML_MS_MODE_ENUM_UINT8          enMsMode;

    pstRatOrder     = NAS_MML_GetMsPrioRatList();


    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,VOS_FALSE);

    /* ����ps��״̬Ϊ��Ч */
    NAS_MML_SetSimPsRegStatus(VOS_FALSE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    /* �ж��Ƿ���Ҫdisable L */
    if (VOS_TRUE == NAS_MMC_IsNeedDisableL_PsFailCause7PsServNotAllow())
    {
        /* ��¼����disable LTE��� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS);

        NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_PS_USIM_INVALID);
    }

    /* �����L��ģ����ԭ��ֵ7����Ҫanycell����,��ģ�����Ҫ���� */
    /* Lģ��PS ONLYʱҲ��Ҫ����anycell���� */
    enMsMode    =    NAS_MML_GetMsMode();
    if ( (VOS_TRUE == NAS_MML_IsLteOnlyMode(pstRatOrder))
      || (NAS_MML_MS_MODE_PS_ONLY == enMsMode))
    {
        return NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL;
    }
    else
    {
        return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
    }

}



NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCause8GprsAndNonGprsNotAllow(VOS_VOID)
{

    return NAS_MMC_ProcEpsAttachRegFailCause3IllegalMs();

}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCause11PlmnNotAllow(VOS_VOID)
{
    NAS_MML_PLMN_ID_STRU               *pstCurPlmnId;

    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    pstCurPlmnId = NAS_MML_GetCurrCampPlmnId();

    /* ��ӽ�ֹ������Ϣ,�и���ʱ����Usim�����ļ� */
    if (VOS_TRUE == NAS_MML_AddForbPlmn(pstCurPlmnId))
    {
        NAS_MMC_SndUsimUpdateForbPlmnReq();
    }

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;

}



NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCause12TaNotAllow(VOS_VOID)
{
    VOS_UINT8                           ucSimCsRegStatus;

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE_REGION);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, NAS_MMC_LIMITED_SERVICE_REGION);

    /* ���CS����ע�����CS����Ч�����CSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
    ucSimCsRegStatus = NAS_MML_GetSimCsRegStatus();
    if ((VOS_TRUE == NAS_MML_GetCsAttachAllowFlg())
     && (VOS_TRUE == ucSimCsRegStatus))
    {
        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    return NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCause13RoamNotAllow(VOS_VOID)
{
    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
}

NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCause14GprsNotAllowInThisPlmn(VOS_VOID)
{

    /* ��ӽ�ֹGPRS PLMN ��Ϣ */
    NAS_MML_AddForbGprsPlmn(NAS_MML_GetCurrCampPlmnId());

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;

}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCause15NoSuitableCell(VOS_VOID)
{
    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCause35ReqServOptNotAuthorizedInThisPlmn(VOS_VOID)
{
    NAS_MML_MS_3GPP_REL_STRU           *pstMs3GppRel = VOS_NULL_PTR;
    NAS_MMC_REG_RSLT_CTX_STRU          *pstRegRsltCtx = VOS_NULL_PTR;
    VOS_UINT32                          ulAttemptCnt;

    pstRegRsltCtx   = NAS_MMC_GetRegRsltCtxAddr();
    pstMs3GppRel    = NAS_MML_GetMs3GppRel();

    if ( NAS_MML_3GPP_REL_R9 >= pstMs3GppRel->enLteNasRelease )
    {
        /* ȡ��Attempt Counter��ֵ */
        ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(pstRegRsltCtx->enRegRsltType);
        return NAS_MMC_ProcEpsRegFailCauseAbnormalCause(NAS_EMM_CAUSE_REQUESTED_SER_OPTION_NOT_AUTHORIZED_IN_PLMN, ulAttemptCnt);
    }
    else
    {
        return NAS_MMC_ProcEpsAttachRegFailCause11PlmnNotAllow();
    }
}




NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCause256AuthRej(VOS_VOID)
{
    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ����cs ps��״̬Ϊ��Ч */
    NAS_MML_SetSimCsRegStatus(VOS_FALSE);
    NAS_MML_SetSimPsRegStatus(VOS_FALSE);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    return NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCause257AccessBarrd(VOS_VOID)
{
    VOS_UINT8                           ucSimCsRegStatus;

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    /* ���CS����ע�����CS����Ч�����CSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
    ucSimCsRegStatus = NAS_MML_GetSimCsRegStatus();
    if ((VOS_TRUE == NAS_MML_GetCsAttachAllowFlg())
     && (VOS_TRUE == ucSimCsRegStatus))
    {
        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    /*���Կ��ش��ҷ�HPLMN/RPLMN�����ֹʱ��Ҫ����PLMN����*/
    if (VOS_TRUE == NAS_MMC_IsAccBarPlmnSearch())
    {
        return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    }
    else
    {
        return NAS_MMC_ADDITIONAL_ACTION_ACCESS_BARRED;
    }
}

NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCause301TimerTimeOut(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    /* ȡ�õ�ǰ��ע������Ϣ���� */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType );

    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        /* AttemptCounterδ�ﵽ�������Ĵ��� */
        NAS_MMC_ProcEpsRegFailAttemptCouterLessThanMaxTimes(&enAction);
    }
    else
    {
        /* ɾ��EPLMN */
        NAS_MMC_DeleteEPlmnList();

        /* ���·���״̬ */
        NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

        /* ֪ͨTAF��ǰ�ķ���״̬ */
        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

        enAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;

    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCause401OtherCause(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    /* ȡ�õ�ǰ��ע������Ϣ���� */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType );

    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        /* AttemptCounterδ�ﵽ�������Ĵ��� */
        NAS_MMC_ProcEpsRegFailAttemptCouterLessThanMaxTimes(&enAction);
    }
    else
    {
        /* ɾ��EPLMN */
        NAS_MMC_DeleteEPlmnList();

        /* ���·���״̬ */
        NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

        /* ֪ͨTAF��ǰ�ķ���״̬ */
        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

        enAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;

    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCause403ForbiddenPlmn(VOS_VOID)
{
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    enFsmId = NAS_MMC_GetCurrFsmId();

    switch ( enFsmId )
    {
        /* ����NAS_MMC_ADDITIONAL_ACTION_BUTT����ʾ��Ч��AdditionalActionֵ��
            MMC�����µ�ǰADDITIONAL ACTION��ֵ */
        case NAS_MMC_FSM_L1_MAIN :
            enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;
            break;

        case NAS_MMC_FSM_PLMN_LIST :
            enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;
            break;

        /* ������Ҫ�������� */
        default:
            enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
            break;
    }

    return enAdditionalAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCause404ForbiddenTaForRegPrvsSvs(VOS_VOID)
{
    /* ���ܾ�ԭ��ֵ#12�Ĵ�����һ�� */
    return NAS_MMC_ProcEpsAttachRegFailCause12TaNotAllow();
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCause405ForbiddenLaForRoam(VOS_VOID)
{
    /* ��tau״̬�Ĵ���һ�� */
    return NAS_MMC_ProcEpsTauRegFailCause405ForbiddenLaForRoam();
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCause406ForbiddenPlmnForGprs(VOS_VOID)
{
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

    /* ��ӽ�ֹGPRS PLMN ��Ϣ */
    NAS_MML_AddForbGprsPlmn(NAS_MML_GetCurrCampPlmnId());

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    enFsmId = NAS_MMC_GetCurrFsmId();

    switch ( enFsmId )
    {
        /* ��ǰ��ONPLMN,����NAS_MMC_ADDITIONAL_ACTION_BUTT����ʾ��Ч��AdditionalAction
            ֵ��MMC�����µ�ǰADDITIONAL ACTION��ֵ */
        case NAS_MMC_FSM_L1_MAIN :
            enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;
            break;

        case NAS_MMC_FSM_PLMN_LIST :
            enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;
            break;

        /* ������Ҫ���п�ѡ������ */
        default:
            enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
            break;
    }

    return enAdditionalAction;

}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCausePsAttachNotAllow(VOS_VOID)
{
    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;

}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCauseEpsBearerEstFail(VOS_VOID)
{
    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
}


 NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCauseMoDetach(VOS_VOID)
 {
     /* ���·���״̬ */
     NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

     /* ֪ͨTAF��ǰ�ķ���״̬ */
     NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

     return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
 }


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCauseMtDetach(VOS_VOID)
{
    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFailCauseT3402Running(VOS_VOID)
{

    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;
    NAS_MMC_TIMER_STATUS_ENUM_U8                            enAvailableTimerStatus;

    enFsmId                = NAS_MMC_GetCurrFsmId();
    enAvailableTimerStatus = NAS_MMC_GetTimerStatus(TI_NAS_MMC_AVAILABLE_TIMER);

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);
    /* ��ǰ��ONPLMNʱ��AVAILABLE��ʱ��δ����,��MMC��Ҫ��������*/
    if ( NAS_MMC_FSM_L1_MAIN   == enFsmId)
    {
        if (NAS_MMC_TIMER_STATUS_RUNING == enAvailableTimerStatus)
        {
            return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
        }
    }

    return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
}





NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause3IllegalMs(VOS_VOID)
{
    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* �ȸ���ȫ�ֱ���,���ڷ�����Ϣʱ��ȡ����ȫ�ֱ��������µ� */    
    /* ����cs ps��״̬Ϊ��Ч */
    NAS_MML_SetSimCsRegStatus(VOS_FALSE);
    NAS_MML_SetSimPsRegStatus(VOS_FALSE);
    
    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);

    NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,VOS_FALSE);

    return NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL;

}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause6IllegalMe(VOS_VOID)
{
    return NAS_MMC_ProcEpsTauRegFailCause3IllegalMs();
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause7EpsServNotAllow(VOS_VOID)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstRatOrder;
    NAS_MML_MS_MODE_ENUM_UINT8          enMsMode;

    pstRatOrder     = NAS_MML_GetMsPrioRatList();

    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ����ps��״̬Ϊ��Ч */
    NAS_MML_SetSimPsRegStatus(VOS_FALSE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    /* �ж��Ƿ���Ҫdisable L */
    if (VOS_TRUE == NAS_MMC_IsNeedDisableL_PsFailCause7PsServNotAllow())
    {
        /* ��¼����disable LTE��� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS);

        NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_PS_USIM_INVALID);
    }

    NAS_MMC_NotifyModeChange(NAS_MML_NET_RAT_TYPE_BUTT,VOS_FALSE);

    enMsMode    =    NAS_MML_GetMsMode();
    if ( (VOS_TRUE == NAS_MML_IsLteOnlyMode(pstRatOrder))
      || (NAS_MML_MS_MODE_PS_ONLY == enMsMode))
    {
        return NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL;
    }
    else
    {
        return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
    }

}



NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause9MsIdCannotBeDerivedByNW(VOS_VOID)
{
    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause10EpsImplicitlyDetached(VOS_VOID)
{
    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause11PlmnNotAllow(VOS_VOID)
{
    NAS_MML_PLMN_ID_STRU               *pstCurPlmnId;

    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    pstCurPlmnId = NAS_MML_GetCurrCampPlmnId();

    /* ��ӽ�ֹ������Ϣ,�и���ʱ����Usim�����ļ� */
    if (VOS_TRUE == NAS_MML_AddForbPlmn(pstCurPlmnId))
    {
        NAS_MMC_SndUsimUpdateForbPlmnReq();
    }

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;

}



NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause12TaNotAllow(VOS_VOID)
{
    VOS_UINT8                           ucSimCsRegStatus;

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE_REGION);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, NAS_MMC_LIMITED_SERVICE_REGION);

    /* ���CS����ע�����CS����Ч�����CSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
    ucSimCsRegStatus = NAS_MML_GetSimCsRegStatus();
    if ((VOS_TRUE == NAS_MML_GetCsAttachAllowFlg())
     && (VOS_TRUE == ucSimCsRegStatus))
    {
        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    return NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause13RoamNotAllow(VOS_VOID)
{
    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
}

NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause14GprsNotAllowInThisPlmn(VOS_VOID)
{
    /* ��ӽ�ֹGPRS PLMN ��Ϣ */
    NAS_MML_AddForbGprsPlmn(NAS_MML_GetCurrCampPlmnId());

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause15NoSuitableCell(VOS_VOID)
{
    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN;
}





NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause40NoEpsBearerActived(VOS_VOID)
{
    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    return NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause35ReqServOptNotAuthorizedInThisPlmn(VOS_VOID)
{
    NAS_MML_MS_3GPP_REL_STRU           *pstMs3GppRel = VOS_NULL_PTR;
    NAS_MMC_REG_RSLT_CTX_STRU          *pstRegRsltCtx = VOS_NULL_PTR;
    VOS_UINT32                          ulAttemptCnt;

    pstRegRsltCtx   = NAS_MMC_GetRegRsltCtxAddr();
    pstMs3GppRel    = NAS_MML_GetMs3GppRel();

    if ( NAS_MML_3GPP_REL_R9 >= pstMs3GppRel->enLteNasRelease )
    {
        /* ȡ��Attempt Counter��ֵ */
        ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(pstRegRsltCtx->enRegRsltType);
        return NAS_MMC_ProcEpsRegFailCauseAbnormalCause(NAS_EMM_CAUSE_REQUESTED_SER_OPTION_NOT_AUTHORIZED_IN_PLMN, ulAttemptCnt);
    }
    else
    {
        return NAS_MMC_ProcEpsTauRegFailCause11PlmnNotAllow();
    }

}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause256AuthRej(VOS_VOID)
{
    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ����cs ps��״̬Ϊ��Ч */
    NAS_MML_SetSimCsRegStatus(VOS_FALSE);
    NAS_MML_SetSimPsRegStatus(VOS_FALSE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);

    return NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause257AccessBarrd(VOS_VOID)
{
    VOS_UINT8                           ucSimCsRegStatus;

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    /* ���CS����ע�����CS����Ч�����CSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
    ucSimCsRegStatus = NAS_MML_GetSimCsRegStatus();
    if ((VOS_TRUE == NAS_MML_GetCsAttachAllowFlg())
     && (VOS_TRUE == ucSimCsRegStatus))
    {
        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    /*���Կ��ش��ҷ�HPLMN/RPLMN�����ֹʱ��Ҫ����PLMN����*/
    if (VOS_TRUE == NAS_MMC_IsAccBarPlmnSearch())
    {
        return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    }
    else
    {
        return NAS_MMC_ADDITIONAL_ACTION_ACCESS_BARRED;
    }
}



NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause301TimerTimeOut(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    /* ȡ�õ�ǰ��ע������Ϣ���� */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType );

    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        /* AttemptCounterδ�ﵽ�������Ĵ��� */
        NAS_MMC_ProcEpsRegFailAttemptCouterLessThanMaxTimes(&enAction);
    }
    else
    {
        /* ɾ��EPLMN */
        NAS_MMC_DeleteEPlmnList();

        /* ���·���״̬ */
        NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

        /* ֪ͨTAF��ǰ�ķ���״̬ */
        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

        enAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;

    }

    return enAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause401OtherCause(VOS_VOID)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    VOS_UINT32                                              ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType;

    /* ȡ�õ�ǰ��ע������Ϣ���� */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType );
    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        /* AttemptCounterδ�ﵽ�������Ĵ��� */
        NAS_MMC_ProcEpsRegFailAttemptCouterLessThanMaxTimes(&enAction);
    }
    else
    {
        /* ɾ��EPLMN */
        NAS_MMC_DeleteEPlmnList();

        /* ���·���״̬ */
        NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

        /* ֪ͨTAF��ǰ�ķ���״̬ */
        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

        enAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    }

    return enAction;
}




NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause403ForbiddenPlmn(VOS_VOID)
{
    return NAS_MMC_ProcEpsAttachRegFailCause403ForbiddenPlmn();
}



NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause404ForbiddenTaForRegPrvsSvs(VOS_VOID)
{
    /* ���ܾ�ԭ��ֵ#12�Ĵ�����һ�� */
    return NAS_MMC_ProcEpsTauRegFailCause12TaNotAllow();
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause405ForbiddenLaForRoam(VOS_VOID)
{
    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;

    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsCause;                              /* �洢ע����CS�򱻾���Ϣ */
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsCause;                              /* �洢ע����PS�򱻾���Ϣ */

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);


    /* ��ǰ��ONPLMN����PLMN LIST״̬�����򷵻ص�ΪNAS_MMC_ADDITIONAL_ACTION_BUTT */
    enFsmId = NAS_MMC_GetCurrFsmId();

    if ( (NAS_MMC_FSM_L1_MAIN   == enFsmId)
      || (NAS_MMC_FSM_PLMN_LIST == enFsmId) )
    {
        return NAS_MMC_ADDITIONAL_ACTION_BUTT;
    }

    /* ��ǰ������ONPLMN����PLMN LIST״̬�����账��ܾ�ԭ��ֵ�����ز�ͬ���������� */

    /* ԭ��ֵ��ʼ��Ϊ#13,�������쳣������#13���г�������� */
    enCsCause = NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW;
    enPsCause = NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW;

    NAS_MMC_GetDomainRegStatusInRegInfoList( NAS_MML_GetCurrCampPlmnId(),
                                             NAS_MML_GetCurrNetRatType(),
                                             &enCsCause,
                                             &enPsCause );

    /* ��CS�����PS����һ�����ԭ��ֵΪ#13,����г�������� */
    if ( (NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW == enCsCause)
      || (NAS_MML_REG_FAIL_CAUSE_ROAM_NOT_ALLOW == enPsCause))
    {
        return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
    }

    /* �����վܾ�ԭ��ֵ#15���������Խ������� */
    return NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN;

}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCause406ForbiddenPlmnForGprs(VOS_VOID)
{
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAdditionalAction;

    /* ��ӽ�ֹGPRS PLMN ��Ϣ */
    NAS_MML_AddForbGprsPlmn(NAS_MML_GetCurrCampPlmnId());

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    enFsmId = NAS_MMC_GetCurrFsmId();

    switch ( enFsmId )
    {
        /* ����NAS_MMC_ADDITIONAL_ACTION_BUTT����ʾ��Ч��AdditionalActionֵ��
            MMC�����µ�ǰADDITIONAL ACTION��ֵ */
        case NAS_MMC_FSM_L1_MAIN :
            enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;
            break;

        case NAS_MMC_FSM_PLMN_LIST :
            enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_BUTT;
            break;

        /* ������Ҫ���п�ѡ������ */
        default:
            enAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
            break;
    }

    return enAdditionalAction;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCauseMoDetach(VOS_VOID)
{
    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCauseMtDetach(VOS_VOID)
{
    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFailCauseT3402Running(VOS_VOID)
{
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;
    NAS_MMC_TIMER_STATUS_ENUM_U8                            enAvailableTimerStatus;

    enFsmId                = NAS_MMC_GetCurrFsmId();
    enAvailableTimerStatus = NAS_MMC_GetTimerStatus(TI_NAS_MMC_AVAILABLE_TIMER);

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);
    /* ��ǰ��ONPLMNʱ��AVAILABLE��ʱ��δ����,��MMC��Ҫ��������*/
    if ( NAS_MMC_FSM_L1_MAIN   == enFsmId)
    {
        if (NAS_MMC_TIMER_STATUS_RUNING == enAvailableTimerStatus)
        {
            return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
        }
    }

    return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
}




/* ����detach EPS��causeֵ���� */

VOS_VOID NAS_MMC_ProcEpsDetachCause2ImsiUnknowInHss(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsRegAdditionalAction
)
{
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ����cs��״̬Ϊ��Ч */
    NAS_MML_SetSimCsRegStatus(VOS_FALSE);
    
    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);

    *penCsRegAdditionalAction  = NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL;
    *penPsRegAdditionalAction  = NAS_MMC_ADDITIONAL_ACTION_BUTT;
}


VOS_VOID NAS_MMC_ProcEpsDetachCause3IllegalMs(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{
    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ����cs ps��״̬Ϊ��Ч */
    NAS_MML_SetSimCsRegStatus(VOS_FALSE);
    NAS_MML_SetSimPsRegStatus(VOS_FALSE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);

    *penCsAddition  = NAS_MMC_ADDITIONAL_ACTION_BUTT;
    *penPsAddition  = NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL;
}



VOS_VOID NAS_MMC_ProcEpsDetachCause6IllegalMe(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{
    NAS_MMC_ProcEpsDetachCause3IllegalMs(penCsAddition, penPsAddition);
}



VOS_VOID NAS_MMC_ProcEpsDetachCause7EpsServNotAllow(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU                     *pstRatOrder;
    NAS_MML_MS_MODE_ENUM_UINT8                      enMsMode;

    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    pstRatOrder     = NAS_MML_GetMsPrioRatList();

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ����ps��״̬Ϊ��Ч */
    NAS_MML_SetSimPsRegStatus(VOS_FALSE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    /* �ж��Ƿ���Ҫdisable L */
    if (VOS_TRUE == NAS_MMC_IsNeedDisableL_PsFailCause7PsServNotAllow())
    {
        /* ��¼����disable LTE��� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS);

        NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_PS_USIM_INVALID);
    }

    /* �����L��ģ����ԭ��ֵ7����Ҫanycell����,��ģ�����Ҫ���� */
    enMsMode    =    NAS_MML_GetMsMode();
    if ( (VOS_TRUE == NAS_MML_IsLteOnlyMode(pstRatOrder))
      || (NAS_MML_MS_MODE_PS_ONLY == enMsMode))
    {
        *penPsAddition = NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL;
        *penCsAddition = NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL;
    }
    else
    {
        *penPsAddition = NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL;
        *penCsAddition = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
    }
}
VOS_VOID NAS_MMC_ProcEpsDetachCause8GprsAndNonGprsNotAllow(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{
    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ����cs ps��״̬Ϊ��Ч */
    NAS_MML_SetSimCsRegStatus(VOS_FALSE);
    NAS_MML_SetSimPsRegStatus(VOS_FALSE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);

    *penCsAddition  = NAS_MMC_ADDITIONAL_ACTION_BUTT;
    *penPsAddition  = NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL;
}
VOS_VOID NAS_MMC_ProcEpsDetachCause11PlmnNotAllow(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{
    *penCsAddition  = NAS_MMC_ADDITIONAL_ACTION_BUTT;
    *penPsAddition  = NAS_MMC_ProcEpsAttachRegFailCause11PlmnNotAllow();
}


VOS_VOID NAS_MMC_ProcEpsDetachCause12TaNotAllow(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{
    VOS_UINT8                           ucSimCsRegStatus;

    *penCsAddition = NAS_MMC_ADDITIONAL_ACTION_BUTT;

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE_REGION);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, NAS_MMC_LIMITED_SERVICE_REGION);

    /* ���CS����ע�����CS����Ч�����CSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
    ucSimCsRegStatus = NAS_MML_GetSimCsRegStatus();
    if ((VOS_TRUE == NAS_MML_GetCsAttachAllowFlg())
     && (VOS_TRUE == ucSimCsRegStatus))
    {
        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    *penPsAddition = NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION;
}
VOS_VOID NAS_MMC_ProcEpsDetachCause13RoamNotAllow(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{

    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    *penCsAddition  = NAS_MMC_ADDITIONAL_ACTION_BUTT;
    *penPsAddition  = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;

}
VOS_VOID NAS_MMC_ProcEpsDetachCause14GprsNotAllowInThisPlmn(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{

    /* ��ӽ�ֹGPRS PLMN ��Ϣ */
    NAS_MML_AddForbGprsPlmn(NAS_MML_GetCurrCampPlmnId());

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    *penCsAddition  = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
    *penPsAddition  = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;

}
VOS_VOID NAS_MMC_ProcEpsDetachCause15NoSuitableCell(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    *penCsAddition  = NAS_MMC_ADDITIONAL_ACTION_BUTT;
    *penPsAddition  = NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN;
}


VOS_VOID NAS_MMC_ProcEpsDetachCauseAbnormalCause(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{
    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    *penCsAddition = NAS_MMC_ADDITIONAL_ACTION_BUTT;

    *penPsAddition = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsServiceRequestFailCause3IllegalMs(VOS_VOID)
{
    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ����cs ps��״̬Ϊ��Ч */
    NAS_MML_SetSimCsRegStatus(VOS_FALSE);
    NAS_MML_SetSimPsRegStatus(VOS_FALSE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);


    return NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL;

}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsServiceRequestFailCause6IllegalMe(VOS_VOID)
{
    return NAS_MMC_ProcEpsServiceRequestFailCause3IllegalMs();
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsServiceRequestFailCause7EpsServNotAllow(VOS_VOID)
{
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstRatOrder;

    pstRatOrder     = NAS_MML_GetMsPrioRatList();


    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ����ps��״̬Ϊ��Ч */
    NAS_MML_SetSimPsRegStatus(VOS_FALSE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    /* �ж��Ƿ���Ҫdisable L */
    if (VOS_TRUE == NAS_MMC_IsNeedDisableL_PsFailCause7PsServNotAllow())
    {
        /* ��¼����disable LTE��� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS);

        NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_PS_USIM_INVALID);
    }

    if (VOS_TRUE == NAS_MML_IsLteOnlyMode(pstRatOrder))
    {
        return NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL;
    }
    else
    {
        return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
    }

}




NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsServiceRequestFailCause9MsIdCannotBeDerivedByNW(VOS_VOID)
{
    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsServiceRequestFailCause10EpsImplicitlyDetached(VOS_VOID)
{
    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
}




NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsServiceRequestFailCause11PlmnNotAllow(VOS_VOID)
{
    NAS_MML_PLMN_ID_STRU               *pstCurPlmnId;

    pstCurPlmnId = NAS_MML_GetCurrCampPlmnId();

    /* ��ӽ�ֹ������Ϣ,�и���ʱ����Usim�����ļ� */
    if (VOS_TRUE == NAS_MML_AddForbPlmn(pstCurPlmnId))
    {
        NAS_MMC_SndUsimUpdateForbPlmnReq();
    }

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;

}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsServiceRequestFailCause12TaNotAllow(VOS_VOID)
{
    VOS_UINT8                           ucSimCsRegStatus;

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE_REGION);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, NAS_MMC_LIMITED_SERVICE_REGION);

    /* ���CS����ע�����CS����Ч�����CSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
    ucSimCsRegStatus = NAS_MML_GetSimCsRegStatus();
    if ((VOS_TRUE == NAS_MML_GetCsAttachAllowFlg())
     && (VOS_TRUE == ucSimCsRegStatus))
    {
        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    return NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsServiceRequestFailCause13RoamNotAllow(VOS_VOID)
{
    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
}

NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsServiceRequestFailCause14GprsNotAllowInThisPlmn(VOS_VOID)
{
    /* ��ӽ�ֹGPRS PLMN ��Ϣ */
    NAS_MML_AddForbGprsPlmn(NAS_MML_GetCurrCampPlmnId());

    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);
    if (VOS_TRUE == NAS_MML_IsCsfbServiceStatusExist())
    {
        return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
    }
    return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsServiceRequestFailCause15NoSuitableCell(VOS_VOID)
{
    /* ���·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

    return NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsServiceRequestFailCause35ReqServOptNotAuthorizedInThisPlmn(VOS_VOID)
{
    NAS_MML_MS_3GPP_REL_STRU           *pstMs3GppRel = VOS_NULL_PTR;

    pstMs3GppRel    = NAS_MML_GetMs3GppRel();

    if ( NAS_MML_3GPP_REL_R9 >= pstMs3GppRel->enLteNasRelease )
    {
       return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
    }
    else
    {
        return NAS_MMC_ProcEpsServiceRequestFailCause11PlmnNotAllow();
    }
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsServiceRequestFailCause257AccessBarrd(VOS_VOID)
{


    if (VOS_TRUE == NAS_MML_IsCsfbServiceStatusExist())
    {
        return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
    }

    /* service request�������ֹ���������ܽ���TAU�Ľ��� */
    return NAS_MMC_ADDITIONAL_ACTION_BUTT;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsServiceRequestFailCause256AuthRej(VOS_VOID)
{
    /* ɾ��EPLMN */
    NAS_MMC_DeleteEPlmnList();

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ����cs ps��״̬Ϊ��Ч */
    NAS_MML_SetSimCsRegStatus(VOS_FALSE);
    NAS_MML_SetSimPsRegStatus(VOS_FALSE);

    /* ֪ͨTAF��ǰ�ķ���״̬ */
    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NO_IMSI);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);

    return NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsServiceRequestFailCause18CsDomainNotAvailable(VOS_VOID)
{
    VOS_UINT8                           ucSimCsRegStatus;

    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

    /* ���CS����ע�����CS����Ч�����CSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
    ucSimCsRegStatus = NAS_MML_GetSimCsRegStatus();
    if ((VOS_TRUE == NAS_MML_GetCsAttachAllowFlg())
     && (VOS_TRUE == ucSimCsRegStatus))
    {
        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

    return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsServiceRequestFailCause25NotAuthorizedForThisCsg(VOS_VOID)
{
    /* ���·���״̬ */
    NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);
    NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);
    return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsServiceRequestFailCause39CsDomainTempNotAvailable(VOS_VOID)
{
    
    return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsServiceRequestFailCause40NoEpsBearerActived(VOS_VOID)
{
    NAS_MML_MS_3GPP_REL_STRU           *pstMs3GppRel = VOS_NULL_PTR;

    pstMs3GppRel = NAS_MML_GetMs3GppRel();
    /* �汾ΪR10�汾���Ժ�汾����ǰԭ��ֵ��#40��ԭ��ֵת�����#10����Э��3GPP 
    24.301 5.5.3.3.5 */
    if (NAS_MML_3GPP_REL_R9 >= pstMs3GppRel->enLteNasRelease)    
    {
        return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
    }
    else
    {
        if (VOS_TRUE == NAS_MML_IsCsfbServiceStatusExist())
        {
            return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
        }

        return NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
    }
}




#endif


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcCsRegFail(
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8    enRegRsltType,
    MMMMC_CS_REG_RESULT_IND_STRU       *pstCsRegRsltInd
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulTblSize;
    NAS_MMC_PROC_REG_FAIL_FUNC_PTR      pRegFailProcFunc = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAction;

    /* ����ע������Ϣ */
    NAS_MMC_SaveRegRsltCtx(enRegRsltType, pstCsRegRsltInd);

        
    /* AT^REJINFO�����ϱ�����REJINFO�ϱ���MMA ������ע���ʱ����ϱ� */
    if (VOS_FALSE == pstCsRegRsltInd->ucIsComBined)
    {
        NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_CS, VOS_FALSE, pstCsRegRsltInd->enRegFailCause);
    }
    

#if (FEATURE_ON == FEATURE_PTM)
    /* ��¼CSע��ʧ���쳣log */
    NAS_MMC_CsRegErrRecord(pstCsRegRsltInd);
#endif

    ulTblSize = sizeof(gastMmcProcCsRegFailCause)/sizeof(gastMmcProcCsRegFailCause[0]);

    /*��CSע��ԭ���gastMmcProcCsRegFailCause�У����Ҷ�Ӧ�Ĵ�����*/
    for ( i = 0 ; i < ulTblSize; i++ )
    {
        if (pstCsRegRsltInd->enRegFailCause == gastMmcProcCsRegFailCause[i].enRegCause)
        {
            pRegFailProcFunc = gastMmcProcCsRegFailCause[i].pRegFailProcFunc;

            break;
        }

    }

    /*�ҵ��󣬵��ö�Ӧ�Ĵ���,�����ض�Ӧ�ĺ�������ʱ*/
    if (VOS_NULL_PTR != pRegFailProcFunc)
    {
        enAction = pRegFailProcFunc();

    }
    /*û���ҵ�ʱ������CS Other Cause����*/
    else
    {
        enAction = NAS_MMC_ProcCsRegFailCauseAbnormalCause(pstCsRegRsltInd->enRegFailCause,
                                                   pstCsRegRsltInd->ulLuAttemptCnt);
    }

    return enAction;

}



NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsRegFail(
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8    enRegRsltType,
    GMMMMC_PS_REG_RESULT_IND_STRU      *pstPsRegRsltInd
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulTblSize;
    NAS_MMC_PROC_REG_FAIL_FUNC_PTR      pRegFailProcFunc = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAction;

    /* ����ע������Ϣ */
    NAS_MMC_SaveRegRsltCtx(enRegRsltType, pstPsRegRsltInd);

    
    /* AT^REJINFO�����ϱ�����REJINFO�ϱ���MMA */
    if (GMM_MMC_REG_DOMAIN_PS_CS == pstPsRegRsltInd->enRsltDomain)
    {
        NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_CS_PS, VOS_FALSE, pstPsRegRsltInd->enRegFailCause);
    }
    else
    {
        NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_PS, VOS_FALSE, pstPsRegRsltInd->enRegFailCause);
    }
    

#if (FEATURE_ON == FEATURE_PTM)
    /* ��¼PSע��ʧ���쳣log */
    NAS_MMC_PsRegErrRecord(pstPsRegRsltInd);
#endif

    ulTblSize = sizeof(gastMmcProcPsRegFailCause)/sizeof(gastMmcProcPsRegFailCause[0]);

    /*��PSע��ԭ���gastMmcProcPsRegFailCause�У����Ҷ�Ӧ�Ĵ�����*/
    for ( i = 0 ; i < ulTblSize; i++ )
    {
        if (pstPsRegRsltInd->enRegFailCause == gastMmcProcPsRegFailCause[i].enRegCause)
        {
            pRegFailProcFunc = gastMmcProcPsRegFailCause[i].pRegFailProcFunc;

            break;
        }
    }

    /*�ҵ��󣬵��ö�Ӧ�Ĵ���,�����ض�Ӧ�ĺ�������ʱ*/
    if (VOS_NULL_PTR != pRegFailProcFunc)
    {
        enAction = pRegFailProcFunc();

    }
    /*û���ҵ�ʱ������PS Other Cause����*/
    else
    {
        enAction = NAS_MMC_ProcPsRegFailCauseAbnormalCause(pstPsRegRsltInd->enRegFailCause,
                                                           pstPsRegRsltInd->ulRegCounter);
    }

    return enAction;

}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcPsServiceRegFail(
    GMMMMC_SERVICE_REQUEST_RESULT_IND_STRU                 *pstServiceRsltInd
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulTblSize;
    NAS_MMC_PROC_REG_FAIL_FUNC_PTR      pRegFailProcFunc = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAction;

    /* ����ע������Ϣ */
    NAS_MMC_SaveRegRsltCtx(GMM_MMC_PS_SERVICE_RESULT_IND, pstServiceRsltInd);

#if (FEATURE_ON == FEATURE_PTM)
    NAS_MMC_PsServiceRegErrRecord(pstServiceRsltInd);
#endif

    ulTblSize = sizeof(gastMmcProcPsRegFailCause)/sizeof(gastMmcProcPsRegFailCause[0]);

    /*��PSע��ԭ���gastMmcProcPsRegFailCause�У����Ҷ�Ӧ�Ĵ�����*/
    for ( i = 0 ; i < ulTblSize; i++ )
    {
        if (pstServiceRsltInd->enRegFailCause == gastMmcProcPsRegFailCause[i].enRegCause)
        {
            pRegFailProcFunc = gastMmcProcPsRegFailCause[i].pRegFailProcFunc;

            break;
        }

    }
    /*�ҵ��󣬵��ö�Ӧ�Ĵ���,�����ض�Ӧ�ĺ�������ʱ*/
    if (VOS_NULL_PTR != pRegFailProcFunc)
    {
        enAction = pRegFailProcFunc();

    }
    /*û���ҵ�ʱ������פ��*/
    else
    {
        enAction = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;
    }

    return enAction;

}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcGmmNetworkDetachInd(
    MMCGMM_NETWORK_DETACH_IND_STRU     *pstDetachMsg
)
{
    NAS_MMC_SERVICE_ENUM_UINT8          enService;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAction;
    VOS_UINT32                          i;
    VOS_UINT32                          ulTblSize;
    NAS_MMC_PROC_REG_FAIL_FUNC_PTR      pRegFailProcFunc = VOS_NULL_PTR;

    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPrioAdditionalAction;

    /* ����ע������Ϣ */
    NAS_MMC_SaveRegRsltCtx(GMM_MMC_PS_NETWORK_DETACH_IND, pstDetachMsg);

    enAction            = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
    ulTblSize           = sizeof(gastMmcProcPsRegFailCause)/sizeof(gastMmcProcPsRegFailCause[0]);

    if (NAS_MML_REG_FAIL_CAUSE_IMSI_UNKNOWN_IN_HLR != pstDetachMsg->ulDetachCause)
    {
        /* ��MM�ķ���״̬ת��ΪMMC ��*/
        enService = NAS_MMC_ConverMmStatusToMmc(NAS_MMC_REG_DOMAIN_PS,
                                               (NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8)pstDetachMsg->ulServiceSts);

        /* ����PS��ķ���״̬ */
        NAS_MMC_SetPsServiceStatus(enService);
    }

    /* ��TAF����PS��ķ���״̬ */
    NAS_MMC_SndMmaDetachInd(MMA_MMC_SRVDOMAIN_PS, pstDetachMsg->ulDetachType,
                            pstDetachMsg->ulDetachCause);

    if (NAS_MMC_GMM_NT_DETACH_NOATTACH == pstDetachMsg->ulDetachType)
    {
        /*��Epsע��ԭ���gastMmcProcEpsRegFailCause�У����Ҷ�Ӧ�Ĵ�����*/
        for ( i = 0 ; i < ulTblSize; i++ )
        {
            if (pstDetachMsg->ulDetachCause == gastMmcProcPsRegFailCause[i].enRegCause)
            {
                pRegFailProcFunc = gastMmcProcPsRegFailCause[i].pRegFailProcFunc;

                break;
            }

        }

        /*�ҵ��󣬵��ö�Ӧ�Ĵ���,�����ض�Ӧ�ĺ�������ʱ*/
        if (VOS_NULL_PTR != pRegFailProcFunc)
        {
            enAction = pRegFailProcFunc();
        }
        else
        {
            /* ��ȡCS addition,����NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON�Ƚϣ�
               �������Ҫ�������������PSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
            enPrioAdditionalAction = NAS_MMC_GetCsAdditionalAction();
            enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON, enPrioAdditionalAction);
            if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
            {
                NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
            }

            enAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
        }
    }

    else if ( NAS_MMC_GMM_NT_DETACH_IMSI == pstDetachMsg->ulDetachType )
    {
        return NAS_MMC_ADDITIONAL_ACTION_BUTT;
    }
    
    else 
    {
        /* ��ȡCS addition,����NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT�Ƚϣ�
           �������Ҫ�������������PSע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH */
        enPrioAdditionalAction = NAS_MMC_GetCsAdditionalAction();
        enPrioAdditionalAction = NAS_MMC_GetPrioAddtionalAction(NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT, enPrioAdditionalAction);
        if (VOS_FALSE == NAS_MMC_IsAdditionalActionTrigerPlmnSrch(enPrioAdditionalAction))
        {
            NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        enAction = NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
    }

    return enAction;

}

#if   (FEATURE_ON == FEATURE_LTE)

VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCauseAbnormalCause(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    VOS_UINT32                          ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8    enRegRsltType;

    /* ȡ�õ�ǰ��ע������Ϣ���� */
    enRegRsltType = NAS_MMC_GetRegRsltType();

    /* ȡ��Attempt Counter��ֵ */
    ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    /* cs ps���ע��ʧ��,��ǰ������ONPLMN����PLMN LIST״̬���������AttemptCounter
       �����ز�ͬ��ADDITIONAL ACTIONֵ */
    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        /* ����EPSע��ʧ��Attemptδ�ﵽ�������Ĵ���Ĵ��� */
        NAS_MMC_ProcCombinedEpsRegFailAttemptCouterLessThanMaxTimes(penPsRegAdditionalAction,
                                                                    penCsRegAdditionalAction);
    }
    else
    {
        /* ɾ��EPLMN */
        NAS_MMC_DeleteEPlmnList();

        /* ���·���״̬ */
        NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);
        NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

        /* ֪ͨTAF��ǰ�ķ���״̬ */
        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

        if ( VOS_TRUE == NAS_MMC_IsNeedDisableL_CombinedRegRsltFailMaxTimes() )
        {
            /* ��ҪDisable Lȫ�ֱ��� */
            NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS);

            NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_LTE_VOICE_NOT_AVAILABLE);

            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER, NAS_MML_GetCsPsMode1EnableLteTimerLen());

            *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
            *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
        }
        else
        {
            *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
            *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
        }
    }

    return;
}
VOS_VOID NAS_MMC_ProcCombinedEpsRegFailCause401OtherCause(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    VOS_UINT32                          ulAttemptCnt;
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8    enRegRsltType;

    /* ȡ�õ�ǰ��ע������ */
    enRegRsltType = NAS_MMC_GetRegRsltType();
    ulAttemptCnt  = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);

    /* cs ps���ע��ʧ��,��ǰ������ONPLMN����PLMN LIST״̬���������AttemptCounter
       �����ز�ͬ��ADDITIONAL ACTIONֵ */
    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        NAS_MMC_ProcCombinedEpsRegFailAttemptCouterLessThanMaxTimes(penPsRegAdditionalAction,
                                                                    penCsRegAdditionalAction);
    }
    else
    {
        /* ɾ��EPLMN */
        NAS_MMC_DeleteEPlmnList();

        /* ���·���״̬ */
        NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);
        NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

        /* ֪ͨTAF��ǰ�ķ���״̬ */
        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

        if ( VOS_TRUE == NAS_MMC_IsNeedDisableL_CombinedRegRsltFailMaxTimes() )
        {
            /* ��ҪDisable Lȫ�ֱ��� */
            NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS);

            NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_LTE_VOICE_NOT_AVAILABLE);

            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER, NAS_MML_GetCsPsMode1EnableLteTimerLen());

            *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
            *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
        }
        else
        {
            *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
            *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
        }
    }
    return;
}


VOS_VOID NAS_MMC_ProcCombinedEpsRegFailAttemptCouterLessThanMaxTimes(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsRegAdditionalAction
)
{
    NAS_MML_EQUPLMN_INFO_STRU          *pstEplmnInfo        = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn     = VOS_NULL_PTR;
    VOS_UINT32                          ulInEplmnListFlag;
    VOS_UINT32                          ulUserSpecPlmnFlag;

    /* �жϵ�ǰ�����Ƿ����û�ָ�������� */
    pstUserSpecPlmn    = NAS_MMC_GetUserSpecPlmnId();
    ulUserSpecPlmnFlag = NAS_MML_CompareBcchPlmnwithSimPlmn(NAS_MML_GetCurrCampPlmnId(),
                                                &(pstUserSpecPlmn->stPlmnId));

    /* �жϵ�ǰ�����Ƿ���EPlmn�б��� */
    pstEplmnInfo       = NAS_MML_GetEquPlmnList();

    ulInEplmnListFlag  = NAS_MML_IsBcchPlmnIdInDestSimPlmnList(NAS_MML_GetCurrCampPlmnId(),
                                              pstEplmnInfo->ucEquPlmnNum,
                                              pstEplmnInfo->astEquPlmnAddr);

    /* �ֶ�ģʽ�£���ǰPlmn�����û�ָ����PLMN,�ҵ�ǰPLMN����EPlmn�б��
       ����Ϊ��ǰplmn��NOT ALLOWED����Ҫ�������� */
    if ( (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
      && (VOS_FALSE                          == ulUserSpecPlmnFlag)
      && (VOS_FALSE                          == ulInEplmnListFlag) )
    {
        *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
        *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
        return ;
    }

    /* ����mmc�����ȴ�ע���� */
    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
    *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
    return;
}
VOS_VOID NAS_MMC_ProcCombinedEpsRegOnlyEpsSuccCauseAbnormalCause(
    VOS_UINT16                                               usCause,
    VOS_UINT32                                               ulAttemptCnt,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    *penCsRegAdditionalAction
)
{
    /*
    1)  attemptCount < MaxCountʱ������NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT
    2)  attemptCount >= MaxCountʱ������NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION
    */

    /* ��������Э��,other causeҲ��ΪEPSע��ɹ�,��˸��·���״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_NORMAL_SERVICE);

    /* ps��ע��ɹ���cs��ע��ʧ�� */
    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
    }
    else
    {
        /* ɾ��EPLMN */
        NAS_MMC_DeleteEPlmnList();

        /* ���·���״̬ */
        NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

        /* ֪ͨTAF��ǰ�ķ���״̬ */
        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);

        *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;

    }

    return;
}
VOS_VOID NAS_MMC_ProcCombinedAttachEpsRegFail(
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType,
    LMM_MMC_ATTACH_IND_STRU                                *pstLmmAttachIndMsg,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsRegAdditionalAction
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              ulTblSize;
    NAS_MMC_PROC_COMBINED_EPS_REG_FAIL_FUNC_PTR             pRegFailProcFunc = VOS_NULL_PTR;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enPsRegRsltCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegRsltCause;

    /* ����ע������Ϣ */
    NAS_MMC_SaveRegRsltCtx(enRegRsltType, pstLmmAttachIndMsg);

    /* ������ע������Ϣ��ȡ�þܾ�ԭ��ֵ */
    NAS_MMC_ConverLmmCombinedAttachRsltToMMLCause(pstLmmAttachIndMsg,
                                          &enPsRegRsltCause, &enCsRegRsltCause);

    NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_CS_PS, VOS_FALSE, enPsRegRsltCause);    

    ulTblSize = sizeof(gastMmcProcCombinedAttachEpsRegFailCause)/sizeof(gastMmcProcCombinedAttachEpsRegFailCause[0]);

    /* ������Epsע��ԭ���gastMmcProcCombinedAttachEpsRegFailCause�У����Ҷ�Ӧ�Ĵ����� */
    for ( i = 0 ; i < ulTblSize; i++ )
    {
        if (enPsRegRsltCause == gastMmcProcCombinedAttachEpsRegFailCause[i].enRegCause)
        {
            pRegFailProcFunc = gastMmcProcCombinedAttachEpsRegFailCause[i].pCombinedEpsRegFailProcFunc;
            break;
        }

    }

    /*�ҵ��󣬵��ö�Ӧ�Ĵ���,�����ض�Ӧ�ĺ�������ʱ*/
    if (VOS_NULL_PTR != pRegFailProcFunc)
    {
        pRegFailProcFunc(penPsRegAdditionalAction, penCsRegAdditionalAction);

    }
    /*û���ҵ�ʱ������PS Other Cause����*/
    else
    {
        NAS_MMC_ProcCombinedEpsRegFailCauseAbnormalCause(penPsRegAdditionalAction,
                                                         penCsRegAdditionalAction);
    }

    if (VOS_TRUE == NAS_MML_IsNetworkRegFailCause(enPsRegRsltCause))
    {
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_EPS, enPsRegRsltCause);
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_CS, enPsRegRsltCause);
    }

    return;

}
VOS_VOID NAS_MMC_ProcCombinedTauEpsRegFail(
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType,
    LMM_MMC_TAU_RESULT_IND_STRU                            *pstLmmTauIndMsg,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsRegAdditionalAction
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              ulTblSize;
    NAS_MMC_PROC_COMBINED_EPS_REG_FAIL_FUNC_PTR             pRegFailProcFunc = VOS_NULL_PTR;

    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enPsRegRsltCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegRsltCause;

    /* ����ע������Ϣ */
    NAS_MMC_SaveRegRsltCtx(enRegRsltType, pstLmmTauIndMsg);

    /* ������ע������Ϣ��ȡ�þܾ�ԭ��ֵ */
    NAS_MMC_ConverLmmCombinedTauRsltToMMLCause(pstLmmTauIndMsg,
                                          &enPsRegRsltCause, &enCsRegRsltCause);

    NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_CS_PS, VOS_FALSE, enPsRegRsltCause);    

    ulTblSize = sizeof(gastMmcProcCombinedTauEpsRegFailCause)/sizeof(gastMmcProcCombinedTauEpsRegFailCause[0]);

    /* ������Epsע��ԭ���gastMmcProcCombinedTauEpsRegFailCause�У����Ҷ�Ӧ�Ĵ����� */
    for ( i = 0 ; i < ulTblSize; i++ )
    {
        if (enPsRegRsltCause == gastMmcProcCombinedTauEpsRegFailCause[i].enRegCause)
        {
            pRegFailProcFunc = gastMmcProcCombinedTauEpsRegFailCause[i].pCombinedEpsRegFailProcFunc;
            break;
        }

    }

    /*�ҵ��󣬵��ö�Ӧ�Ĵ���,�����ض�Ӧ�ĺ�������ʱ*/
    if (VOS_NULL_PTR != pRegFailProcFunc)
    {
        pRegFailProcFunc(penPsRegAdditionalAction, penCsRegAdditionalAction);

    }
    /*û���ҵ�ʱ������PS Other Cause����*/
    else
    {
        NAS_MMC_ProcCombinedEpsRegFailCauseAbnormalCause(penPsRegAdditionalAction,
                                                         penCsRegAdditionalAction);
    }

    if (VOS_TRUE == NAS_MML_IsNetworkRegFailCause(enPsRegRsltCause))
    {
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_EPS, enPsRegRsltCause);
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_CS, enPsRegRsltCause);
    }

    return;

}
VOS_VOID NAS_MMC_ProcCombinedEpsRegOnlyEpsSucc(
    VOS_UINT16                                              usCause,
    VOS_UINT32                                              ulAttemptCnt,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsRegAdditionalAction
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              ulTblSize;
    NAS_MMC_PROC_REG_FAIL_FUNC_PTR                          pRegFailProcFunc = VOS_NULL_PTR;

    ulTblSize = sizeof(gastMmcProcCombinedEpsRegOnlyEpsSuccCause)/sizeof(gastMmcProcCombinedEpsRegOnlyEpsSuccCause[0]);

    /* ������Epsע��ԭ���gastMmcProcCombinedEpsRegOnlyEpsSuccCause�У����Ҷ�Ӧ�Ĵ����� */
    for ( i = 0 ; i < ulTblSize; i++ )
    {
        if (usCause == gastMmcProcCombinedEpsRegOnlyEpsSuccCause[i].enRegCause)
        {
            pRegFailProcFunc = gastMmcProcCombinedEpsRegOnlyEpsSuccCause[i].pRegFailProcFunc;
            break;
        }

    }

    /*�ҵ��󣬵��ö�Ӧ�Ĵ���,�����ض�Ӧ�ĺ�������ʱ*/
    if (VOS_NULL_PTR != pRegFailProcFunc)
    {
        *penCsRegAdditionalAction = pRegFailProcFunc();

    }
    /*û���ҵ�ʱ������PS Other Cause����*/
    else
    {
        NAS_MMC_ProcCombinedEpsRegOnlyEpsSuccCauseAbnormalCause(usCause, ulAttemptCnt,
                                                             penCsRegAdditionalAction);
    }

    NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_EPS, NAS_MML_REG_FAIL_CAUSE_NULL);
    if (VOS_TRUE == NAS_MML_IsNetworkRegFailCause(usCause))
    {
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_CS, usCause);
    }

    return;

}



NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsRegFailCauseAbnormalCause(
    VOS_UINT16                          usCause,
    VOS_UINT32                          ulAttemptCnt
)
{
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAction;

    if (ulAttemptCnt < NAS_MML_MAX_PS_REG_FAIL_CNT)
    {
        /* AttemptCounterδ�ﵽ�������Ĵ��� */
        NAS_MMC_ProcEpsRegFailAttemptCouterLessThanMaxTimes(&enAction);
    }
    else
    {
        /* ɾ��EPLMN */
        NAS_MMC_DeleteEPlmnList();

        /* ���·���״̬ */
        NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);

        /* ֪ͨTAF��ǰ�ķ���״̬ */
        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

        enAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;

    }

    return enAction;
}
VOS_VOID NAS_MMC_ProcEpsRegFailAttemptCouterLessThanMaxTimes(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsRegAdditionalAction
)
{
    NAS_MML_EQUPLMN_INFO_STRU          *pstEplmnInfo        = VOS_NULL_PTR;
    NAS_MML_PLMN_WITH_RAT_STRU         *pstUserSpecPlmn     = VOS_NULL_PTR;
    VOS_UINT32                          ulInEplmnListFlag;
    VOS_UINT32                          ulUserSpecPlmnFlag;

    /* �жϵ�ǰ�����Ƿ����û�ָ�������� */
    pstUserSpecPlmn    = NAS_MMC_GetUserSpecPlmnId();
    ulUserSpecPlmnFlag = NAS_MML_CompareBcchPlmnwithSimPlmn(NAS_MML_GetCurrCampPlmnId(),
                                                &(pstUserSpecPlmn->stPlmnId));

    /* �жϵ�ǰ�����Ƿ���EPlmn�б��� */
    pstEplmnInfo       = NAS_MML_GetEquPlmnList();

    ulInEplmnListFlag  = NAS_MML_IsBcchPlmnIdInDestSimPlmnList(NAS_MML_GetCurrCampPlmnId(),
                                              pstEplmnInfo->ucEquPlmnNum,
                                              pstEplmnInfo->astEquPlmnAddr);

    /* �ֶ�ģʽ�£���ǰPlmn�����û�ָ����PLMN,�ҵ�ǰPLMN����EPlmn�б��
       ����Ϊ��ǰplmn��NOT ALLOWED����Ҫ�������� */
    if ( (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
      && (VOS_FALSE                          == ulUserSpecPlmnFlag)
      && (VOS_FALSE                          == ulInEplmnListFlag) )
    {
        *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;
        return ;
    }

    /* ����mmc�����ȴ�ע���� */
    *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;
    return;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsAttachRegFail(
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8    enRegRsltType,
    LMM_MMC_ATTACH_IND_STRU            *pstLmmAttachIndMsg
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulTblSize;
    NAS_MMC_PROC_REG_FAIL_FUNC_PTR      pRegFailProcFunc = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAction;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enRegRsltCause;
    VOS_UINT32                                              ulAttemptCnt;

    /* ����ע������Ϣ */
    NAS_MMC_SaveRegRsltCtx(enRegRsltType, pstLmmAttachIndMsg);

    /* ��LMM��ע��ʧ��ԭ��ֵת��ΪMM��ʽ��ԭ��ֵ */
    NAS_MMC_ConverLmmAttachRsltToMMLCause(pstLmmAttachIndMsg, &enRegRsltCause);

    NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_PS, VOS_FALSE, enRegRsltCause);

    ulTblSize = sizeof(gastMmcProcEpsAttachRegFailCause)/sizeof(gastMmcProcEpsAttachRegFailCause[0]);

    /*��Epsע��ԭ���gastMmcProcEpsRegFailCause�У����Ҷ�Ӧ�Ĵ�����*/
    for ( i = 0 ; i < ulTblSize; i++ )
    {
        if (enRegRsltCause == gastMmcProcEpsAttachRegFailCause[i].enRegCause)
        {
            pRegFailProcFunc = gastMmcProcEpsAttachRegFailCause[i].pRegFailProcFunc;

            break;
        }

    }

    /*�ҵ��󣬵��ö�Ӧ�Ĵ���,�����ض�Ӧ�ĺ�������ʱ*/
    if (VOS_NULL_PTR != pRegFailProcFunc)
    {
        enAction = pRegFailProcFunc();

    }
    /*û���ҵ�ʱ������PS Other Cause����*/
    else
    {
        /* ȡ��Attempt Counter��ֵ */
        ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);
        enAction = NAS_MMC_ProcEpsRegFailCauseAbnormalCause(enRegRsltCause, ulAttemptCnt);
    }


    if (VOS_TRUE == NAS_MML_IsNetworkRegFailCause(enRegRsltCause))
    {
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_EPS, enRegRsltCause);
    }
    return enAction;

}



NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcEpsTauRegFail(
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8    enRegRsltType,
    LMM_MMC_TAU_RESULT_IND_STRU        *pstLmmTauIndMsg
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              ulTblSize;
    NAS_MMC_PROC_REG_FAIL_FUNC_PTR                          pRegFailProcFunc = VOS_NULL_PTR;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enAction;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enRegRsltCause;
    VOS_UINT32                                              ulAttemptCnt;

    /* ����ע������Ϣ */
    NAS_MMC_SaveRegRsltCtx(enRegRsltType, pstLmmTauIndMsg);
    NAS_MMC_ConverTauResultToMMLCause(pstLmmTauIndMsg, &enRegRsltCause);

    NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_PS, VOS_FALSE, enRegRsltCause);

    ulTblSize = sizeof(gastMmcProcEpsTauRegFailCause)/sizeof(gastMmcProcEpsTauRegFailCause[0]);

    /*��Epsע��ԭ���gastMmcProcEpsRegFailCause�У����Ҷ�Ӧ�Ĵ�����*/
    for ( i = 0 ; i < ulTblSize; i++ )
    {
        if (enRegRsltCause == gastMmcProcEpsTauRegFailCause[i].enRegCause)
        {
            pRegFailProcFunc = gastMmcProcEpsTauRegFailCause[i].pRegFailProcFunc;

            break;
        }

    }

    /*�ҵ��󣬵��ö�Ӧ�Ĵ���,�����ض�Ӧ�ĺ�������ʱ*/
    if (VOS_NULL_PTR != pRegFailProcFunc)
    {
        enAction = pRegFailProcFunc();

    }
    /*û���ҵ�ʱ������EPS Abnormal Cause����*/
    else
    {
        /* ȡ��Attempt Counter��ֵ */
        ulAttemptCnt = NAS_MMC_GetRegRsltAttemptCounter(enRegRsltType);
        enAction     = NAS_MMC_ProcEpsRegFailCauseAbnormalCause(enRegRsltCause, ulAttemptCnt);
    }

    if (VOS_TRUE == NAS_MML_IsNetworkRegFailCause(enRegRsltCause))
    {
        NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_EPS, enRegRsltCause);
    }

    return enAction;

}
VOS_VOID NAS_MMC_ProcEpsDetachCause(
    NAS_MMC_REG_RSLT_TYPE_ENUM_UINT8                        enRegRsltType,
    LMM_MMC_DETACH_IND_STRU                                *pstDetachMsg,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              ulTblSize;
    NAS_MMC_PROC_EPS_DETACH_CAUSE_FUNC_PTR                  pRegFailProcFunc = VOS_NULL_PTR;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enRegRsltCause;

    /* ����ע������Ϣ */
    NAS_MMC_SaveRegRsltCtx(enRegRsltType, pstDetachMsg);

    /* ����CAUSEֵ��OTHER CAUSE����һ�� */
    if ( VOS_FALSE == pstDetachMsg->bitOpCnCause )
    {
        enRegRsltCause = NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE;
    }
    else
    {
        NAS_MMC_ConvertLmmCauseToMmCause((VOS_UINT8)pstDetachMsg->ucCnCause,
                                                &enRegRsltCause);
    }

    ulTblSize           = sizeof(gastMmcProcEpsDetachCause)/sizeof(gastMmcProcEpsDetachCause[0]);

    /*��Epsע��ԭ���gastMmcProcEpsDetachCause�У����Ҷ�Ӧ�Ĵ�����*/
    for ( i = 0 ; i < ulTblSize; i++ )
    {
        if (enRegRsltCause == gastMmcProcEpsDetachCause[i].enRegCause)
        {
            pRegFailProcFunc = gastMmcProcEpsDetachCause[i].pEpsDetachCauseProcFunc;

            break;
        }
    }

    /*�ҵ��󣬵��ö�Ӧ�Ĵ���,�����ض�Ӧ�ĺ�������ʱ*/
    if (VOS_NULL_PTR != pRegFailProcFunc)
    {
        pRegFailProcFunc(penCsAddition, penPsAddition);
    }
    else
    {
        NAS_MMC_ProcEpsDetachCauseAbnormalCause(penCsAddition, penPsAddition);
    }

    return;

}


VOS_VOID NAS_MMC_ProcLmmDetachInd(
    LMM_MMC_DETACH_IND_STRU                                *pstDetachMsg,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{
    MMC_LMM_MT_DETACH_TYPE_ENUM_UINT32                      enCnReqType;

    enCnReqType         = MMC_LMM_MT_DETACH_TYPE_BUTT;

   *penCsAddition       = NAS_MMC_ADDITIONAL_ACTION_BUTT;
   *penPsAddition       = NAS_MMC_ADDITIONAL_ACTION_BUTT;

   /* MMC_LMM_DETACH_LOCAL��Ԥ��������ɣ�����ɾ�� */


    /* ������Ӧ��detach���OP��ر� */
    if ( VOS_TRUE == pstDetachMsg->bitOpCnReqType)
    {
        enCnReqType = pstDetachMsg->ulCnReqType;
    }

    /* ���ഥ����DETACH���̣�ͨ��DETACH������MMC_LMM_DETACH_MT֪ͨMMC��
    ��Я����Ӧ������DETACH�������ͺ�ԭ��ֵ */
    switch (enCnReqType)
    {
        case MMC_LMM_MT_DET_REATTACH_NOT_REQUIRED:

            NAS_MMC_ProcEpsDetachCause(LMM_MMC_DETACH_IND, pstDetachMsg,
                                       penCsAddition, penPsAddition);
            
            /* �������PDN���ӱ�־,�ݲ���֪ͨEPLMN��Ϊ��״̬�յ�detach�����ʱ����Ҫ֪ͨEPLMN */
            NAS_MML_SetEmcPdpStatusFlg(VOS_FALSE);



            /* ��GMM��MMת��ע������Ϣ */
            NAS_MMC_SndMmLmmMtDetachInd(pstDetachMsg);

            NAS_MMC_SndGmmLmmMtDetachInd(pstDetachMsg);

            break;

        case MMC_LMM_MT_DET_REATTACH_REQUIRED:

            *penPsAddition = NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT;

            
            /* �������PDN���ӱ�־,�ݲ���֪ͨEPLMN��Ϊ��״̬�յ�detach�����ʱ����Ҫ֪ͨEPLMN */
            NAS_MML_SetEmcPdpStatusFlg(VOS_FALSE);

            /* ��GMM��MMת��ע������Ϣ */
            NAS_MMC_SndMmLmmMtDetachInd(pstDetachMsg);

            NAS_MMC_SndGmmLmmMtDetachInd(pstDetachMsg);

            break;
        case MMC_LMM_MT_DET_IMSI:

            /* ��MMת��ע������Ϣ */
            NAS_MMC_SndMmLmmMtDetachInd(pstDetachMsg);

            NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);

            break;

        default:

            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcLmmDetachInd:Unexpected detach cn req type!");
            break;
    }


    return;
}




NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_ProcLmmServiceRsltInd(
    LMM_MMC_SERVICE_RESULT_IND_STRU    *pstSerRsltMsg
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulTblSize;
    NAS_MMC_PROC_REG_FAIL_FUNC_PTR      pRegFailProcFunc = VOS_NULL_PTR;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enRegRsltCause;
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAction;

    /* LMM��SERVICE���̽��ͨ����ԭ��֪ͨMMC���ṹ�е�bit������ʾ�����Ƿ���Ч��
     SERVICE���̳ɹ�������֪ͨMMC��
     SERVICE���̱�����ܾ���ͨ��SERVICE�����MMC_LMM_SERVICE_RSLT_FAILURE֪ͨMMC����Я����Ӧ��ԭ��ֵ��
     ���UE����EXTENDED SERVICE���̣���Я��SERVICE���͡�
     ע��SERIVE������ʱ��ʹ�� */

    /* ����LMM�ϱ���SERVICE�����Ϣ */
    NAS_MMC_SaveRegRsltCtx(LMM_MMC_SERVICE_RESULT_IND, pstSerRsltMsg);

    ulTblSize = sizeof(gastMmcProcEpsServiceRequestFailCause)/sizeof(gastMmcProcEpsServiceRequestFailCause[0]);
    NAS_MMC_ConverLmmServiceRsltToMMLCause(pstSerRsltMsg,&enRegRsltCause);

    /* ֪ͨMM/GMM */
    NAS_MMC_SndMmLmmSerResultnd(pstSerRsltMsg);
    NAS_MMC_SndGmmLmmSerResultnd(pstSerRsltMsg);

    /*��Epsע��ԭ���gastMmcProcEpsRegFailCause�У����Ҷ�Ӧ�Ĵ�����*/
    for ( i = 0 ; i < ulTblSize; i++ )
    {
        if (enRegRsltCause == gastMmcProcEpsServiceRequestFailCause[i].enRegCause)
        {
            pRegFailProcFunc = gastMmcProcEpsServiceRequestFailCause[i].pRegFailProcFunc;

            break;
        }

    }

    /*�ҵ��󣬵��ö�Ӧ�Ĵ���,�����ض�Ӧ�ĺ�������ʱ*/
    if (VOS_NULL_PTR != pRegFailProcFunc)
    {
        enAction = pRegFailProcFunc();
    }
    else
    {
        enAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
    }

    if( VOS_TRUE == NAS_MML_IsCsfbServiceStatusExist() )
    {
        enAction = NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION ;
    }

    return enAction;

}


VOS_VOID  NAS_MMC_ProcLmmCombinedTauResultInd_TauRsltEpsOnlySucc(
    LMM_MMC_TAU_RESULT_IND_STRU                            *pstLmmTauIndMsg,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegRsltCause,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{
    VOS_UINT32                          ulEquPlmnNum;
    MMC_LMM_PLMN_ID_STRU               *pstLmmEquPlmnList = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU           stLEplmnInfo;

    NAS_MMC_SaveRegRsltCtx(LMM_MMC_TAU_RESULT_IND, pstLmmTauIndMsg);

    /* AT^REJINFO�����ϱ�����REJINFO�ϱ���MMA */    
    NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_PS, VOS_TRUE, NAS_MML_REG_FAIL_CAUSE_NULL);
    NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_CS, VOS_FALSE, enCsRegRsltCause);

    /* eps��ע��ɹ���cs��ע��ʧ�� */
    *penPsAddition = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

    /* ע��ɹ�����ATTACH_IND��Ϣ�в���LAIʱ����Ҫɾ��ForbPlmn,ForbGprs����Ϣ */
    NAS_MMC_DelForbInfo_LmmAttRsltSucc(VOS_NULL_PTR);

    /* ����EPLMN,����ע��ɹ���EPS only�ɹ���������ܴ�EPLMN */
    if ( VOS_TRUE == pstLmmTauIndMsg->bitOpEplmn)
    {
        ulEquPlmnNum        = pstLmmTauIndMsg->stEplmnList.ulPlmnNum;
        pstLmmEquPlmnList   = pstLmmTauIndMsg->stEplmnList.astEplmnList;

        /* ��LMM��PLMN ID��ʽת��ΪGU�ĸ�ʽ */
        NAS_MMC_ConvertLmmPlmnToGUNasPlmn(ulEquPlmnNum, pstLmmEquPlmnList, (stLEplmnInfo.astEquPlmnAddr));
        NAS_MML_SaveEquPlmnList(ulEquPlmnNum, stLEplmnInfo.astEquPlmnAddr);

        NAS_MMC_WriteEplmnNvim();
    }

    NAS_MMC_UpdateGURegRlstRPlmnIdInNV();

    NAS_MMC_ProcCombinedEpsRegOnlyEpsSucc(enCsRegRsltCause, pstLmmTauIndMsg->ulAttemptCount,
                                          penCsAddition);

    return;
}
VOS_VOID  NAS_MMC_ProcLmmCombinedTauResultInd_TauRsltCsEpsSucc(
    LMM_MMC_TAU_RESULT_IND_STRU                            *pstLmmTauIndMsg,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{
    VOS_UINT32                          ulEquPlmnNum;
    MMC_LMM_PLMN_ID_STRU               *pstLmmEquPlmnList = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU           stLEplmnInfo;

    NAS_MMC_SaveRegRsltCtx(LMM_MMC_TAU_RESULT_IND, pstLmmTauIndMsg);

     *penPsAddition = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;
     *penCsAddition = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

     /* ����ע��������ע����Ϣ�� */
     NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_EPS, NAS_MML_REG_FAIL_CAUSE_NULL);
     NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_CS, NAS_MML_REG_FAIL_CAUSE_NULL);

     /* �ϱ�����״̬ */
     NAS_MMC_SetPsServiceStatus(NAS_MMC_NORMAL_SERVICE);

     /* TAU�ɹ����Ҵ�LAIʱ����Ҫɾ��ForbPlmn,ForbLa,ForbGprs����Ϣ */
     if (VOS_TRUE == pstLmmTauIndMsg->bitOpLai)
     {
         NAS_MMC_DelForbInfo_LmmAttRsltSucc(&(pstLmmTauIndMsg->stLai));
     }
     else
     {
         /* TAU�ɹ����Ҳ���LAIʱ����Ҫɾ��ForbPlmn,ForbGprs����Ϣ */
         NAS_MMC_DelForbInfo_LmmAttRsltSucc(VOS_NULL_PTR);
     }


    *penCsAddition = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

    NAS_MMC_SetCsServiceStatus(NAS_MMC_NORMAL_SERVICE);

    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE);
    
    NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_CS_PS, VOS_TRUE, NAS_MML_REG_FAIL_CAUSE_NULL);


    NAS_MMC_UpdateCsRegStateCsRegSucc();

    /* ����EPLMN,����ע��ɹ���EPS only�ɹ���������ܴ�EPLMN */
    if ( VOS_TRUE == pstLmmTauIndMsg->bitOpEplmn)
    {
        ulEquPlmnNum        = pstLmmTauIndMsg->stEplmnList.ulPlmnNum;
        pstLmmEquPlmnList   = pstLmmTauIndMsg->stEplmnList.astEplmnList;

        /* ��LMM��PLMN ID��ʽת��ΪGU�ĸ�ʽ */
        NAS_MMC_ConvertLmmPlmnToGUNasPlmn(ulEquPlmnNum, pstLmmEquPlmnList, (stLEplmnInfo.astEquPlmnAddr));
        NAS_MML_SaveEquPlmnList(ulEquPlmnNum, stLEplmnInfo.astEquPlmnAddr);

        NAS_MMC_WriteEplmnNvim();
    }

    NAS_MMC_UpdateGURegRlstRPlmnIdInNV();

}


VOS_VOID  NAS_MMC_ProcLmmCombinedAttachInd_AttRsltCsEpsSucc(
    LMM_MMC_ATTACH_IND_STRU                                *pstLmmAttachIndMsg,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{
    VOS_UINT32                          ulEquPlmnNum;
    MMC_LMM_PLMN_ID_STRU               *pstLmmEquPlmnList = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU           stLEplmnInfo;

    NAS_MMC_SaveRegRsltCtx(LMM_MMC_ATTACH_IND, pstLmmAttachIndMsg);

    *penPsAddition = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

    /* �ϱ�����״̬ */
    NAS_MMC_SetPsServiceStatus(NAS_MMC_NORMAL_SERVICE);
    NAS_MMC_SetCsServiceStatus(NAS_MMC_NORMAL_SERVICE);

    
    
    if (VOS_TRUE == pstLmmAttachIndMsg->bitOpLai)
    {
        /* ע��ɹ�����ATTACH_IND��Ϣ�д�LAIʱ����Ҫɾ��ForbPlmn,ForbLa,ForbGprs����Ϣ */
        NAS_MMC_DelForbInfo_LmmAttRsltSucc(&(pstLmmAttachIndMsg->stLai));
    }
    else
    {
        /* ע��ɹ�����ATTACH_IND��Ϣ�в���LAIʱ����Ҫɾ��ForbPlmn,ForbGprs����Ϣ */
        NAS_MMC_DelForbInfo_LmmAttRsltSucc(VOS_NULL_PTR);
    }

    *penCsAddition = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

    NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE);
    
    

    NAS_MMC_UpdateCsRegStateCsRegSucc();

    /* ����ע��������ע����Ϣ�� */
    NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_EPS, NAS_MML_REG_FAIL_CAUSE_NULL);
    NAS_MMC_UpdatePlmnRegInfoList(NAS_MML_GetCurrCampPlmnId(), NAS_MMC_REG_DOMAIN_CS, NAS_MML_REG_FAIL_CAUSE_NULL);

    NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_CS_PS, VOS_TRUE, NAS_MML_REG_FAIL_CAUSE_NULL);

    /* ����EPLMN������ע��ɹ���EPS only�ɹ���������ܴ�EPLMN */
    if ( VOS_TRUE == pstLmmAttachIndMsg->bitOpEplmn)
    {
        ulEquPlmnNum        = pstLmmAttachIndMsg->stEplmnList.ulPlmnNum;
        pstLmmEquPlmnList   = pstLmmAttachIndMsg->stEplmnList.astEplmnList;

        /* ��LMM��PLMN ID��ʽת��ΪGU�ĸ�ʽ */
        NAS_MMC_ConvertLmmPlmnToGUNasPlmn(ulEquPlmnNum, pstLmmEquPlmnList, (stLEplmnInfo.astEquPlmnAddr));
        NAS_MML_SaveEquPlmnList(ulEquPlmnNum, stLEplmnInfo.astEquPlmnAddr);

        NAS_MMC_WriteEplmnNvim();
    }

    NAS_MMC_UpdateGURegRlstRPlmnIdInNV();
    return;
}


VOS_VOID  NAS_MMC_ProcLmmCombinedAttachInd_AttRsltEpsOnlySucc(
    LMM_MMC_ATTACH_IND_STRU                                *pstLmmAttachIndMsg,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegRsltCause,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{
    VOS_UINT32                          ulEquPlmnNum;
    MMC_LMM_PLMN_ID_STRU               *pstLmmEquPlmnList = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU           stLEplmnInfo;

    NAS_MMC_SaveRegRsltCtx(LMM_MMC_ATTACH_IND, pstLmmAttachIndMsg);

    /* AT^REJINFO�����ϱ�����REJINFO�ϱ���MMA */
    NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_PS, VOS_TRUE, NAS_MML_REG_FAIL_CAUSE_NULL);
    NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_CS, VOS_FALSE, enCsRegRsltCause);

    /* EPS��ע��ɹ���cs��ע��ʧ�� */
    *penPsAddition = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

    /* ע��ɹ�����ATTACH_IND��Ϣ�в���LAIʱ����Ҫɾ��ForbPlmn,ForbGprs����Ϣ */
    NAS_MMC_DelForbInfo_LmmAttRsltSucc(VOS_NULL_PTR);

    /* ����EPLMN������ע��ɹ���EPS only�ɹ���������ܴ�EPLMN */
    if ( VOS_TRUE == pstLmmAttachIndMsg->bitOpEplmn)
    {
        ulEquPlmnNum        = pstLmmAttachIndMsg->stEplmnList.ulPlmnNum;
        pstLmmEquPlmnList   = pstLmmAttachIndMsg->stEplmnList.astEplmnList;

        /* ��LMM��PLMN ID��ʽת��ΪGU�ĸ�ʽ */
        NAS_MMC_ConvertLmmPlmnToGUNasPlmn(ulEquPlmnNum, pstLmmEquPlmnList, (stLEplmnInfo.astEquPlmnAddr));
        NAS_MML_SaveEquPlmnList(ulEquPlmnNum, stLEplmnInfo.astEquPlmnAddr);

        NAS_MMC_WriteEplmnNvim();
    }

    NAS_MMC_UpdateGURegRlstRPlmnIdInNV();

    NAS_MMC_ProcCombinedEpsRegOnlyEpsSucc(enCsRegRsltCause, pstLmmAttachIndMsg->ulAttemptCount,
                                          penCsAddition);

    return;
}





VOS_VOID  NAS_MMC_ProcLmmCombinedAttachInd_AttRsltSucc(
    LMM_MMC_ATTACH_IND_STRU                                *pstLmmAttachIndMsg,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegRsltCause,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{
    if (VOS_TRUE == pstLmmAttachIndMsg->bitOpCnRslt)
    {
        if (MMC_LMM_ATT_CN_RSLT_COMBINED_EPS_IMSI == pstLmmAttachIndMsg->ulCnRslt)
        {
            NAS_MMC_ProcLmmCombinedAttachInd_AttRsltCsEpsSucc(pstLmmAttachIndMsg,
                          penCsAddition, penPsAddition);
        }
        else if (MMC_LMM_ATT_CN_RSLT_EPS_ONLY == pstLmmAttachIndMsg->ulCnRslt)
        {
            NAS_MMC_ProcLmmCombinedAttachInd_AttRsltEpsOnlySucc(pstLmmAttachIndMsg,enCsRegRsltCause,
                          penCsAddition, penPsAddition);

        }
        else
        {
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcLmmCombinedAttachInd_AttRsltSucc:Unexpected cn result!");
        }
    }
    else
    {
        /* ������ע��ɹ��ϱ��������񷵻� */
        *penPsAddition = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;
        *penCsAddition = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

         NAS_MMC_SetPsServiceStatus(NAS_MMC_NORMAL_SERVICE);
         NAS_MMC_SetCsServiceStatus(NAS_MMC_NORMAL_SERVICE);

         NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE);

         NAS_MMC_UpdateCsRegStateCsRegSucc();

         NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_CS_PS, VOS_TRUE, NAS_MML_REG_FAIL_CAUSE_NULL);
    }

    NAS_MMC_NotifyModeChange(NAS_MML_GetCurrNetRatType(),
	                         NAS_MML_GetSimPsRegStatus());


    return;
}
VOS_VOID NAS_MMC_ProcLmmCombinedTauResultInd_TauRsltSucc(
    LMM_MMC_TAU_RESULT_IND_STRU                            *pstLmmTauIndMsg,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegRsltCause,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsAddition,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsAddition
)
{
    if (VOS_TRUE == pstLmmTauIndMsg->bitOpCnRst)
    {
        /* CS+PS��������TAUʱ,�����Ƿ�Я��CS CAUSEֵ,����Ϊ����ע��ɹ� */ 
        if ((MMC_LMM_COMBINED_TA_LA_UPDATED == pstLmmTauIndMsg->ulCnRst)
         || (MMC_LMM_COMBINED_TA_LA_UPDATED_ISR_ACTIVATED == pstLmmTauIndMsg->ulCnRst)
         || (MMC_LMM_CS_PS_PERIODIC_UPDATING == pstLmmTauIndMsg->ulReqType))
        {
            /* �����ཻ��EPS��CS��ע��ɹ� */
            NAS_MMC_ProcLmmCombinedTauResultInd_TauRsltCsEpsSucc(pstLmmTauIndMsg,
                          penCsAddition, penPsAddition);
        }
        else if ((MMC_LMM_TA_UPDATED == pstLmmTauIndMsg->ulCnRst)
              || (MMC_LMM_TA_UPDATED_ISR_ACTIVATED == pstLmmTauIndMsg->ulCnRst))
        {
            /* �����ཻ��EPS�ɹ�CSʧ�ܣ���ԭ��ֵΪЭ��24301 5.5.3.3.4.3�½����� */
            NAS_MMC_ProcLmmCombinedTauResultInd_TauRsltEpsOnlySucc(pstLmmTauIndMsg,enCsRegRsltCause,
                          penCsAddition, penPsAddition);
        }
        else
        {
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcLmmCombinedTauResultInd_TauRsltSucc:Unexpected cn result!");
        }
    }
    else
    {
        /* ������ע��ɹ��ϱ��������񷵻� */
        *penPsAddition = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;
        *penCsAddition = NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

         NAS_MMC_SetPsServiceStatus(NAS_MMC_NORMAL_SERVICE);
         NAS_MMC_SetCsServiceStatus(NAS_MMC_NORMAL_SERVICE);

         NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE);

         NAS_MMC_UpdateCsRegStateCsRegSucc();

         NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_CS_PS, VOS_TRUE, NAS_MML_REG_FAIL_CAUSE_NULL);
    }

    NAS_MMC_NotifyModeChange(NAS_MML_GetCurrNetRatType(),
	                         NAS_MML_GetSimPsRegStatus());


    return;
}




VOS_VOID NAS_MMC_ProcLmmCombinedTauResult(
    LMM_MMC_TAU_RESULT_IND_STRU                            *pstLmmTauIndMsg,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegRsltCause,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penPsRegAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                   *penCsRegAdditionalAction
)
{
    switch (pstLmmTauIndMsg->ulTauRst)
{
    case MMC_LMM_TAU_RSLT_SUCCESS:
        NAS_MMC_ProcLmmCombinedTauResultInd_TauRsltSucc(pstLmmTauIndMsg,
                   enCsRegRsltCause, penCsRegAdditionalAction, penPsRegAdditionalAction);

        break;

    case MMC_LMM_TAU_RSLT_FAILURE:
        if (VOS_FALSE == pstLmmTauIndMsg->bitOpAtmpCnt)
        {
            *penPsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
            *penCsRegAdditionalAction = NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;
            NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);
            NAS_MMC_SetCsServiceStatus(NAS_MMC_LIMITED_SERVICE);

            NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS_PS, MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

            NAS_MMC_SndMmaRegResultInd(MMA_MMC_SRVDOMAIN_CS_PS, VOS_FALSE, NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE);
        }
        else
        {
            /* EPS��CSע���ʧ�� */
            NAS_MMC_ProcCombinedTauEpsRegFail(LMM_MMC_TAU_RESULT_IND, pstLmmTauIndMsg,
                                          penPsRegAdditionalAction, penCsRegAdditionalAction);
        }
        break;

    case MMC_LMM_TAU_RSLT_MO_DETACH_FAILURE:
    case MMC_LMM_TAU_RSLT_MT_DETACH_FAILURE:
    case MMC_LMM_TAU_RSLT_T3402_RUNNING:
    case MMC_LMM_TAU_RSLT_TIMER_EXP:
    case MMC_LMM_TAU_RSLT_ACCESS_BARED:
    case MMC_LMM_TAU_RSLT_FORBID_PLMN:
    case MMC_LMM_TAU_RSLT_FORBID_TA_FOR_RPOS:
    case MMC_LMM_TAU_RSLT_FORBID_PLMN_FOR_GPRS:
    case MMC_LMM_TAU_RSLT_FORBID_TA_FOR_ROAMING:
    case MMC_LMM_TAU_RSLT_AUTH_REJ:
        /* EPS��CSע���ʧ�� */
        NAS_MMC_ProcCombinedTauEpsRegFail(LMM_MMC_TAU_RESULT_IND, pstLmmTauIndMsg,
                                      penPsRegAdditionalAction, penCsRegAdditionalAction);
        break;

    case MMC_LMM_TAU_RSLT_CN_REJ:
        if ((VOS_TRUE == pstLmmTauIndMsg->bitOpCnRst)
         && ((MMC_LMM_TA_UPDATED == pstLmmTauIndMsg->ulCnRst)
          || (MMC_LMM_TA_UPDATED_ISR_ACTIVATED == pstLmmTauIndMsg->ulCnRst)))
        {
            /* ��TAU��������Ϊ���ϣ�������Ӧ��TAU�������ΪTA UPDATED��
               ��ԭ��ֵ��ΪЭ��24301 5.5.3.3.4.3�½����л���û��Я��ԭ��ֵ��
               ��LMMͨ��TAU�����MMC_LMM_TAU_RSLT_CN_REJ֪ͨMMC����Я����Ӧ
               ��TAU�������͡�������Ӧ��TAU������͡�ԭ��ֵ����Я�������ԭ
               ����tau attempt counterֵ */
            NAS_MMC_ProcLmmCombinedTauResultInd_TauRsltEpsOnlySucc(pstLmmTauIndMsg,enCsRegRsltCause,
                              penCsRegAdditionalAction, penPsRegAdditionalAction);

        }
        else
        {
            /* EPS��CSע���ʧ�� */
            NAS_MMC_ProcCombinedTauEpsRegFail(LMM_MMC_TAU_RESULT_IND, pstLmmTauIndMsg,
                                          penPsRegAdditionalAction, penCsRegAdditionalAction);
        }
        break;

    default:

        /* �쳣��ӡ */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcLmmCombinedTauResult:Unexpected tau result!");
        *penPsRegAdditionalAction = NAS_MMC_GetPsRegAdditionalAction_L1Main();
        *penCsRegAdditionalAction = NAS_MMC_GetCsRegAdditionalAction_L1Main();
        break;
    }

    return;
}

#endif

VOS_VOID NAS_MMC_GetAdditionalActionPrioValue(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAction,
    VOS_UINT8                           *pucPrioValue
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulTblSize;

    ulTblSize = sizeof(gastAdditionalActionPrioArr)/sizeof(gastAdditionalActionPrioArr[0]);

    for  ( i = 0 ; i < ulTblSize ; i++ )
    {
        if (enAction == gastAdditionalActionPrioArr[i].enAdditionalAction)
        {
           *pucPrioValue = gastAdditionalActionPrioArr[i].ucPriority;
            return;
        }
    }

    /* δ���ҵ��򷵻�BUTT��������ȼ� */
    *pucPrioValue = gastAdditionalActionPrioArr[NAS_MMC_ADDITIONAL_ACTION_BUTT].ucPriority;

}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8  NAS_MMC_GetPrioAddtionalAction(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enCsAdditionalAction,
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enPsAdditionalAction
)
{
    VOS_UINT8                           ucCsPriority;
    VOS_UINT8                           ucPsPriority;


    NAS_MMC_GetAdditionalActionPrioValue(enCsAdditionalAction, &ucCsPriority);
    NAS_MMC_GetAdditionalActionPrioValue(enPsAdditionalAction, &ucPsPriority);

    /* ȨֵԽС�����ȼ�Խ�� */
    if (ucCsPriority < ucPsPriority)
    {
        return enCsAdditionalAction;
    }
    else
    {
        return enPsAdditionalAction;
    }
}
VOS_UINT32 NAS_MMC_IsAdditionalActionTrigerPlmnSrch(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enAdditionalAction
)
{
    switch (enAdditionalAction)
    {
        case NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL:
        case NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION :
        case NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL :
        case NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN :
        case NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION:
            return VOS_TRUE;

        default:

            return VOS_FALSE;
    }

}


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_GetPsAdditionalAction(VOS_VOID)
{
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;

    /* ��ȡ��ǰ״̬��ID������״̬�����ò�ͬ״̬���Ļ�ȡPS AdditionalAction���� */
    enFsmId                = NAS_MMC_GetCurrFsmId();
    if (NAS_FSM_BUTT <= enFsmId)
    {
        NAS_ERROR_LOG1(WUEPS_PID_MMC, "NAS_MMC_GetPsAdditionalAction,ERROR:FsmId Error", enFsmId);

        return NAS_MMC_ADDITIONAL_ACTION_BUTT;
    }

    if (NAS_MMC_FSM_L1_MAIN == enFsmId)
    {
        return NAS_MMC_GetPsRegAdditionalAction_L1Main();
    }
    else if (NAS_MMC_FSM_PLMN_LIST == enFsmId)
    {
        return NAS_MMC_GetPsRegAdditionalAction_PlmnList();
    }
    else if (NAS_MMC_FSM_BG_PLMN_SEARCH == enFsmId)
    {
        return NAS_MMC_GetPsRegAdditionalAction_BgPlmnSearch();
    }
    else if (NAS_MMC_FSM_PLMN_SELECTION == enFsmId)
    {
        return NAS_MMC_GetPsRegAdditionalAction_PlmnSelection();
    }
    else
    {
        NAS_WARNING_LOG1(WUEPS_PID_MMC, "NAS_MMC_GetPsAdditionalAction,WARNING:This Fsm not have Ps Additon", enFsmId);
    }

    return NAS_MMC_ADDITIONAL_ACTION_BUTT;
}
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_GetCsAdditionalAction(VOS_VOID)
{
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;

    /* ��ȡ��ǰ״̬��ID������״̬�����ò�ͬ״̬���Ļ�ȡCS AdditionalAction���� */
    enFsmId                = NAS_MMC_GetCurrFsmId();
    if (NAS_FSM_BUTT <= enFsmId)
    {
        NAS_ERROR_LOG1(WUEPS_PID_MMC, "NAS_MMC_GetCsAdditionalAction,ERROR:FsmId Error", enFsmId);

        return NAS_MMC_ADDITIONAL_ACTION_BUTT;
    }

    if (NAS_MMC_FSM_L1_MAIN == enFsmId)
    {
        return NAS_MMC_GetCsRegAdditionalAction_L1Main();
    }
    else if (NAS_MMC_FSM_PLMN_LIST == enFsmId)
    {
        return NAS_MMC_GetCsRegAdditionalAction_PlmnList();
    }
    else if (NAS_MMC_FSM_BG_PLMN_SEARCH == enFsmId)
    {
        return NAS_MMC_GetCsRegAdditionalAction_BgPlmnSearch();
    }
    else if (NAS_MMC_FSM_PLMN_SELECTION == enFsmId)
    {
        return NAS_MMC_GetCsRegAdditionalAction_PlmnSelection();
    }
    else
    {
        NAS_WARNING_LOG1(WUEPS_PID_MMC, "NAS_MMC_GetCsAdditionalAction,WARNING:This Fsm not have Cs Additon", enFsmId);
    }

    return NAS_MMC_ADDITIONAL_ACTION_BUTT;
}

#if   (FEATURE_ON == FEATURE_LTE)
VOS_VOID  NAS_MMC_ConvertLmmCauseToMmCause(
    NAS_LMM_CN_CAUSE_ENUM_UINT8          enLmmCause,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  *penMmCnCause
)
{
    *penMmCnCause = (NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16)enLmmCause;
}


VOS_VOID NAS_MMC_ConverLmmCombinedAttachRsltToMMLCause(
    LMM_MMC_ATTACH_IND_STRU            *pstLmmAttachIndMsg,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16 *penPsRegRsltCause,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16 *penCsRegRsltCause
)
{
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16 enPsCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16 enCsCause;
    MMC_LMM_ATTACH_CN_RSLT_ENUM_UINT32 ulAttachCnRslt;

    enPsCause      = NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE;
    enCsCause      = NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE;
    ulAttachCnRslt = MMC_LMM_ATT_CN_RSLT_BUTT;

    if (VOS_TRUE == pstLmmAttachIndMsg->bitOpCnRslt)
    {
        ulAttachCnRslt = pstLmmAttachIndMsg->ulCnRslt;
    }

    switch (pstLmmAttachIndMsg->ulAttachRslt)
    {
        case MMC_LMM_ATT_RSLT_SUCCESS:
            if (MMC_LMM_ATT_CN_RSLT_COMBINED_EPS_IMSI == ulAttachCnRslt)
            {
                /* ����ע��cs ps���ע��ɹ� */
                enPsCause = NAS_MML_REG_FAIL_CAUSE_NULL;
                enCsCause = NAS_MML_REG_FAIL_CAUSE_NULL;
                break;
            }
            if (MMC_LMM_ATT_CN_RSLT_EPS_ONLY == ulAttachCnRslt)
            {
                /* ����ע��ps��ע��ɹ���cs��ע��ʧ�� */
                enPsCause = NAS_MML_REG_FAIL_CAUSE_NULL;
                NAS_MMC_ConvertLmmCauseToMmCause((VOS_UINT8)pstLmmAttachIndMsg->ucCnCause, &enCsCause);
                break;
            }

            NAS_WARNING_LOG(WUEPS_PID_MMC,"NAS_MMC_ConverLmmCombinedAttachRsltToMMLCause: attach ind cnRslt value is invalid");
            break;

        case MMC_LMM_ATT_RSLT_CN_REJ:
            if (MMC_LMM_ATT_CN_RSLT_EPS_ONLY == ulAttachCnRslt)
            {
                enPsCause = NAS_MML_REG_FAIL_CAUSE_NULL;
                if (VOS_TRUE == pstLmmAttachIndMsg->bitOpCnCause)
                {
                    NAS_MMC_ConvertLmmCauseToMmCause((VOS_UINT8)pstLmmAttachIndMsg->ucCnCause, &enCsCause);
                }
                break;
            }

            if (VOS_TRUE == pstLmmAttachIndMsg->bitOpCnCause)
            {
                NAS_MMC_ConvertLmmCauseToMmCause((VOS_UINT8)pstLmmAttachIndMsg->ucCnCause, &enPsCause);
                enCsCause = enPsCause;
            }
            break;

        case MMC_LMM_ATT_RSLT_ACCESS_BAR:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_ACCESS_BARRED;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_ACCESS_BARRED;
            break;

        case MMC_LMM_ATT_RSLT_TIMER_EXP :
            enPsCause = NAS_MML_REG_FAIL_CAUSE_TIMER_TIMEOUT;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_TIMER_TIMEOUT;
            break;

        case MMC_LMM_ATT_RSLT_AUTH_REJ :
            enPsCause = NAS_MML_REG_FAIL_CAUSE_AUTH_REJ;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_AUTH_REJ;
            break;

        case MMC_LMM_ATT_RSLT_FAILURE:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE;
            break;

        case MMC_LMM_ATT_RSLT_FORBID_PLMN:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN;
            break;

        case MMC_LMM_ATT_RSLT_FORBID_TA_FOR_RPOS:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_REG_PRVS_SVS;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_REG_PRVS_SVS;
            break;

        case MMC_LMM_ATT_RSLT_FORBID_PLMN_FOR_GPRS:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN_FOR_GPRS;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN_FOR_GPRS;
            break;

        case MMC_LMM_ATT_RSLT_FORBID_TA_FOR_ROAMING:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_ROAM;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_ROAM;
            break;
        case MMC_LMM_ATT_RSLT_PS_ATT_NOT_ALLOW:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_PS_ATTACH_NOT_ALLOW;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_PS_ATTACH_NOT_ALLOW;
            break;

        case MMC_LMM_ATT_RSLT_ESM_FAILURE:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_ESM_BEAR_FAILURE;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_ESM_BEAR_FAILURE;
            break;

        case MMC_LMM_ATT_RSLT_MO_DETACH_FAILURE:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_MO_DETACH_FAILURE;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_MO_DETACH_FAILURE;
            break;

        case MMC_LMM_ATT_RSLT_MT_DETACH_FAILURE:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_MT_DETACH_FAILURE;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_MT_DETACH_FAILURE;
            break;

        case MMC_LMM_ATT_RSLT_T3402_RUNNING:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_T3402_RUNNING;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_T3402_RUNNING;

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ConverLmmCombinedAttachRsltToMMLCause: attach ind ulAttachRslt invalid");
            break;
    }

    *penPsRegRsltCause = enPsCause;
    *penCsRegRsltCause = enCsCause;

    return;
}


VOS_VOID NAS_MMC_ConverLmmCombinedTauRsltToMMLCause(
    LMM_MMC_TAU_RESULT_IND_STRU        *pstLmmTauResultInd,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16 *penPsRegRsltCause,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16 *penCsRegRsltCause
)
{
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsCause;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsCause;
    MMC_LMM_TAU_CN_RSLT_ENUM_UINT32     ulTauCnRst;

    enPsCause  = NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE;
    enCsCause  = NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE;
    ulTauCnRst = MMC_LMM_TAU_CN_RSLT_BUTT;

    if (VOS_TRUE == pstLmmTauResultInd->bitOpCnRst)
    {
        ulTauCnRst = pstLmmTauResultInd->ulCnRst;
    }

    switch (pstLmmTauResultInd->ulTauRst)
    {
        case MMC_LMM_TAU_RSLT_SUCCESS:

            enPsCause = NAS_MML_REG_FAIL_CAUSE_NULL;

            if ( (MMC_LMM_COMBINED_TA_LA_UPDATED == ulTauCnRst)
              || (MMC_LMM_COMBINED_TA_LA_UPDATED_ISR_ACTIVATED == ulTauCnRst))
            {
                /* ����ע��cs ps���ע��ɹ� */
                enCsCause = NAS_MML_REG_FAIL_CAUSE_NULL;
                break;
            }

            if (((MMC_LMM_TA_UPDATED == ulTauCnRst)
              || (MMC_LMM_TA_UPDATED_ISR_ACTIVATED == ulTauCnRst))
             && (VOS_TRUE == pstLmmTauResultInd->bitOpCnCause) )
            {
                /* ����ע��ps��ע��ɹ���cs��ע��ʧ�� */
                NAS_MMC_ConvertLmmCauseToMmCause((VOS_UINT8)pstLmmTauResultInd->ucCnCause, &enCsCause);
                break;
            }

            NAS_WARNING_LOG(WUEPS_PID_MMC,"NAS_MMC_ConverLmmCombinedTauRsltToMMLCause: tau cnRslt value is invalid");
            break;

        case MMC_LMM_TAU_RSLT_CN_REJ:
            if ((MMC_LMM_TA_UPDATED == ulTauCnRst)
             || (MMC_LMM_TA_UPDATED_ISR_ACTIVATED == ulTauCnRst))
            {
                enPsCause = NAS_MML_REG_FAIL_CAUSE_NULL;
                if (VOS_TRUE == pstLmmTauResultInd->bitOpCnCause)
                {
                    NAS_MMC_ConvertLmmCauseToMmCause((VOS_UINT8)pstLmmTauResultInd->ucCnCause, &enCsCause);
                }
                break;
            }

            if (VOS_TRUE == pstLmmTauResultInd->bitOpCnCause)
            {
                NAS_MMC_ConvertLmmCauseToMmCause((VOS_UINT8)pstLmmTauResultInd->ucCnCause, &enPsCause);
                enCsCause = enPsCause;
            }
            break;

        case MMC_LMM_TAU_RSLT_ACCESS_BARED:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_ACCESS_BARRED;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_ACCESS_BARRED;
            break;

        case MMC_LMM_TAU_RSLT_TIMER_EXP :
            enPsCause = NAS_MML_REG_FAIL_CAUSE_TIMER_TIMEOUT;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_TIMER_TIMEOUT;
            break;

        case MMC_LMM_TAU_RSLT_AUTH_REJ :
            enPsCause = NAS_MML_REG_FAIL_CAUSE_AUTH_REJ;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_AUTH_REJ;
            break;

        case MMC_LMM_TAU_RSLT_FAILURE:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE;
            break;

        case MMC_LMM_TAU_RSLT_FORBID_PLMN:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN;
            break;

        case MMC_LMM_TAU_RSLT_FORBID_TA_FOR_RPOS:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_REG_PRVS_SVS;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_REG_PRVS_SVS;
            break;

        case MMC_LMM_TAU_RSLT_FORBID_PLMN_FOR_GPRS:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN_FOR_GPRS;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN_FOR_GPRS;
            break;

        case MMC_LMM_TAU_RSLT_FORBID_TA_FOR_ROAMING:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_ROAM;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_ROAM;
            break;
        case  MMC_LMM_TAU_RSLT_MO_DETACH_FAILURE:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_MO_DETACH_FAILURE;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_MO_DETACH_FAILURE;
            break;

        case  MMC_LMM_TAU_RSLT_MT_DETACH_FAILURE:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_MT_DETACH_FAILURE;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_MT_DETACH_FAILURE;
            break;

        case MMC_LMM_TAU_RSLT_T3402_RUNNING:
            enPsCause = NAS_MML_REG_FAIL_CAUSE_T3402_RUNNING;
            enCsCause = NAS_MML_REG_FAIL_CAUSE_T3402_RUNNING;
            break;

        default:
            break;
    }

    *penPsRegRsltCause = enPsCause;
    *penCsRegRsltCause = enCsCause;

    return;
}

VOS_VOID NAS_MMC_ConverLmmAttachRsltToMMLCause(
    LMM_MMC_ATTACH_IND_STRU            *pstLmmAttachIndMsg,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16 *penRegRsltCause
)
{
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16 enCause;

    enCause  = NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE;

    switch (pstLmmAttachIndMsg->ulAttachRslt)
    {
        case MMC_LMM_ATT_RSLT_SUCCESS:
            enCause = NAS_MML_REG_FAIL_CAUSE_NULL;
            break;

        case MMC_LMM_ATT_RSLT_CN_REJ:

            if (VOS_TRUE == pstLmmAttachIndMsg->bitOpCnCause)
            {
                NAS_MMC_ConvertLmmCauseToMmCause((VOS_UINT8)pstLmmAttachIndMsg->ucCnCause,
                                                                &enCause);
            }

            break;

        case MMC_LMM_ATT_RSLT_ACCESS_BAR:
            enCause = NAS_MML_REG_FAIL_CAUSE_ACCESS_BARRED;
            break;

        case MMC_LMM_ATT_RSLT_TIMER_EXP :
            enCause = NAS_MML_REG_FAIL_CAUSE_TIMER_TIMEOUT;
            break;

        case MMC_LMM_ATT_RSLT_AUTH_REJ :
            enCause = NAS_MML_REG_FAIL_CAUSE_AUTH_REJ;
            break;

        case MMC_LMM_ATT_RSLT_FAILURE:
            enCause = NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE;
            break;


        /* L�¼����̱���ԭ��ֵ#11���� */
        case MMC_LMM_ATT_RSLT_FORBID_PLMN:
            enCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN;
            break;

        /* L�¼����̱���ԭ��ֵ#12���� */
        case MMC_LMM_ATT_RSLT_FORBID_TA_FOR_RPOS:
            enCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_REG_PRVS_SVS;
            break;

        /* L�¼����̱���ԭ��ֵ#14���� */
        case MMC_LMM_ATT_RSLT_FORBID_PLMN_FOR_GPRS:
            enCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN_FOR_GPRS;
            break;

        /* L�¼����̱���ԭ��ֵ#13 #15���� */
        case MMC_LMM_ATT_RSLT_FORBID_TA_FOR_ROAMING:
            enCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_ROAM;
            break;

        case MMC_LMM_ATT_RSLT_PS_ATT_NOT_ALLOW:
            enCause = NAS_MML_REG_FAIL_CAUSE_PS_ATTACH_NOT_ALLOW;
            break;

        case MMC_LMM_ATT_RSLT_ESM_FAILURE:
            enCause = NAS_MML_REG_FAIL_CAUSE_ESM_BEAR_FAILURE;
            break;

        case MMC_LMM_ATT_RSLT_MO_DETACH_FAILURE:
            enCause = NAS_MML_REG_FAIL_CAUSE_MO_DETACH_FAILURE;
            break;

        case MMC_LMM_ATT_RSLT_MT_DETACH_FAILURE:
            enCause = NAS_MML_REG_FAIL_CAUSE_MT_DETACH_FAILURE;
            break;

        case MMC_LMM_ATT_RSLT_T3402_RUNNING:
            enCause = NAS_MML_REG_FAIL_CAUSE_T3402_RUNNING;
            break;

        default:
            break;
    }

    *penRegRsltCause = enCause;

    return;
}


VOS_VOID NAS_MMC_ConverLmmServiceRsltToMMLCause(
    LMM_MMC_SERVICE_RESULT_IND_STRU    *pstLmmSerRsltIndMsg,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16 *penRegRsltCause
)
{
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16 enCause;

    enCause  = NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE;

    switch (pstLmmSerRsltIndMsg->ulServiceRst)
    {
        case MMC_LMM_SERVICE_RSLT_CN_REJ:

            if ( VOS_TRUE == pstLmmSerRsltIndMsg->bitOpCnCause)
            {
                NAS_MMC_ConvertLmmCauseToMmCause((VOS_UINT8)pstLmmSerRsltIndMsg->ucCnCause,
                                                                    &enCause);
            }

            break;

        case MMC_LMM_SERVICE_RSLT_ACCESS_BARED:
            enCause = NAS_MML_REG_FAIL_CAUSE_ACCESS_BARRED;
            break;

        case MMC_LMM_SERVICE_RSLT_AUTH_REJ :
            enCause = NAS_MML_REG_FAIL_CAUSE_AUTH_REJ;
            break;

        case MMC_LMM_SERVICE_RSLT_FAILURE:
            enCause = NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE;
            break;

        default:
            break;
    }

    *penRegRsltCause = enCause;

    return;
}


VOS_VOID NAS_MMC_ConverTauResultToMMLCause(
    LMM_MMC_TAU_RESULT_IND_STRU        *pstLmmTauIndMsg,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16 *penRegRsltCause
)
{
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCause;

    enCause  = NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE;

    switch (pstLmmTauIndMsg->ulTauRst)
    {
        case MMC_LMM_TAU_RSLT_SUCCESS:
            enCause = NAS_MML_REG_FAIL_CAUSE_NULL;
            break;

        case MMC_LMM_TAU_RSLT_CN_REJ:

            if (VOS_TRUE == pstLmmTauIndMsg->bitOpCnCause)
            {
                NAS_MMC_ConvertLmmCauseToMmCause((VOS_UINT8)pstLmmTauIndMsg->ucCnCause,
                                                                &enCause);
            }

            break;

        case MMC_LMM_TAU_RSLT_ACCESS_BARED:
            enCause = NAS_MML_REG_FAIL_CAUSE_ACCESS_BARRED;
            break;

        case MMC_LMM_TAU_RSLT_TIMER_EXP :
            enCause = NAS_MML_REG_FAIL_CAUSE_TIMER_TIMEOUT;
            break;

        case MMC_LMM_TAU_RSLT_AUTH_REJ :
            enCause = NAS_MML_REG_FAIL_CAUSE_AUTH_REJ;
            break;

        case MMC_LMM_TAU_RSLT_FAILURE:
            enCause = NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE;
            break;


        /* L�¼����̱���ԭ��ֵ#11���� */
        case MMC_LMM_TAU_RSLT_FORBID_PLMN:
            enCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN;
            break;

        /* L�¼����̱���ԭ��ֵ#12���� */
        case MMC_LMM_TAU_RSLT_FORBID_TA_FOR_RPOS:
            enCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_REG_PRVS_SVS;
            break;

        /* L�¼����̱���ԭ��ֵ#14���� */
        case MMC_LMM_TAU_RSLT_FORBID_PLMN_FOR_GPRS:
            enCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_PLMN_FOR_GPRS;
            break;

        /* L�¼����̱���ԭ��ֵ#13 #15���� */
        case MMC_LMM_TAU_RSLT_FORBID_TA_FOR_ROAMING:
            enCause = NAS_MML_REG_FAIL_CAUSE_FORBIDDEN_LA_FOR_ROAM;
            break;


        case  MMC_LMM_TAU_RSLT_MO_DETACH_FAILURE:
            enCause = NAS_MML_REG_FAIL_CAUSE_MO_DETACH_FAILURE;
            break;

        case  MMC_LMM_TAU_RSLT_MT_DETACH_FAILURE:
            enCause = NAS_MML_REG_FAIL_CAUSE_MT_DETACH_FAILURE;
            break;

        case MMC_LMM_TAU_RSLT_T3402_RUNNING:
            enCause = NAS_MML_REG_FAIL_CAUSE_T3402_RUNNING;
            break;


        default:
            break;
    }

    *penRegRsltCause = enCause;

    return;
}


MMC_LMM_ATTACH_TYPE_ENUM_UINT32 NAS_MMC_ConvertLmmAttachReqType(
    NAS_MML_LTE_UE_OPERATION_MODE_ENUM_UINT8                enUeOperationMode
)
{
    if ((NAS_MML_LTE_UE_OPERATION_MODE_CS_PS_2 == enUeOperationMode)
     || (NAS_MML_LTE_UE_OPERATION_MODE_CS_PS_1 == enUeOperationMode))
    {
        return MMC_LMM_ATT_TYPE_COMBINED_EPS_IMSI;
    }
    else if ((NAS_MML_LTE_UE_OPERATION_MODE_PS_2 == enUeOperationMode)
          || (NAS_MML_LTE_UE_OPERATION_MODE_PS_1 == enUeOperationMode))
    {
        return MMC_LMM_ATT_TYPE_EPS_ONLY;
    }
    else
    {
        return MMC_LMM_ATT_TYPE_BUTT;
    }

}


VOS_VOID NAS_MMC_ConvertGmmRegDomainToMmcDomain(
	GMM_MMC_REG_DOMAIN_ENUM_UINT32		enRegDomain,
	NAS_MMC_REG_DOMAIN_ENUM_UINT8      *penMmcRegDomain
)
{
	if (GMM_MMC_REG_DOMAIN_PS_CS == enRegDomain)
	{
		*penMmcRegDomain = NAS_MMC_REG_DOMAIN_PS_CS;
	}
	else
	{
		*penMmcRegDomain =  NAS_MMC_REG_DOMAIN_PS;
	}

	return;
}


VOS_UINT32 NAS_MMC_IsNeedDisableL_CombinedRegRsltFailMaxTimes()
{
    NAS_MML_LTE_UE_OPERATION_MODE_ENUM_UINT8                enLUeMode;

    enLUeMode        = NAS_MML_GetLteUeOperationMode();

    /* ����CS/PS mode 1 */
    if (NAS_MML_LTE_UE_OPERATION_MODE_CS_PS_1 != enLUeMode)
    {
        return VOS_FALSE;
    }

    /* LTE only */
    if (VOS_TRUE == NAS_MML_IsLteOnlyMode(NAS_MML_GetMsPrioRatList()))
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_IsNeedDisableL_CombinedRegRsltSucc()
{
    NAS_MML_ADDITION_UPDATE_RSLT_INFO_ENUM_UINT8            enAdditionUpdateRslt;
    NAS_MML_LTE_UE_OPERATION_MODE_ENUM_UINT8                enLUeMode;

    enLUeMode            = NAS_MML_GetLteUeOperationMode();
    enAdditionUpdateRslt = NAS_MML_GetAdditionUpdateRslt();

    /* ����CS/PS mode 1 */
    if (NAS_MML_LTE_UE_OPERATION_MODE_CS_PS_1 != enLUeMode)
    {
        return VOS_FALSE;
    }


    /* LTE only */
    if (VOS_TRUE == NAS_MML_IsLteOnlyMode(NAS_MML_GetMsPrioRatList()))
    {
        return VOS_FALSE;
    }

    /* Additional Update Result IE��Ϣ��ָʾSMS Only��CSFB Not Preferred */
    if ((NAS_MML_ADDITION_UPDATE_RSLT_SMS_ONLY == enAdditionUpdateRslt)
     || (NAS_MML_ADDITION_UPDATE_RSLT_CSFB_NOT_PREFERRED == enAdditionUpdateRslt))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}
VOS_UINT32 NAS_MMC_IsNeedDisableL_CombinedRegRsltOnlyEpsSucc(
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCsRegRsltCause
)
{
    NAS_MML_LTE_UE_OPERATION_MODE_ENUM_UINT8                enLUeMode;

    enLUeMode        = NAS_MML_GetLteUeOperationMode();

    /* ����CS/PS mode 1 */
    if (NAS_MML_LTE_UE_OPERATION_MODE_CS_PS_1 != enLUeMode)
    {
        return VOS_FALSE;
    }


    /* LTE only */
    if (VOS_TRUE == NAS_MML_IsLteOnlyMode(NAS_MML_GetMsPrioRatList()))
    {
        return VOS_FALSE;
    }

    /* enCsRegRsltCauseΪ#16,#17,#22,other cause && NAS_MMC_GetRegRsltAttemptCounter�ﵽ5�� */
    if ((NAS_MML_MAX_PS_REG_FAIL_CNT                == NAS_MMC_GetRegRsltAttemptCounter(NAS_MMC_GetRegRsltType()))
     && (NAS_MML_REG_FAIL_CAUSE_IMSI_UNKNOWN_IN_HLR != enCsRegRsltCause))
    {
        return VOS_TRUE;
    }

    /* ����ע��ʧ��ʱ,����ӵ�#18�б���,��other cause����,��Ҫ5��֮��Disable LTE,
       ��ԭ��ֵ�жϲ�׼ȷ,�������ڱ���1��֮���Disable LTE */
    /* ��#18�б��� */
    if (VOS_TRUE == NAS_MMC_IsPlmnIdInCsDomainUnAvailPlmnList(NAS_MML_GetCurrCampPlmnId()))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}

VOS_UINT32 NAS_MMC_IsNeedDisableL_EpsOnlyRegSucc()
{
    NAS_MML_LTE_UE_OPERATION_MODE_ENUM_UINT8                enLUeMode;
    VOS_UINT8                                               ucSimCsRegStatus;

    enLUeMode        = NAS_MML_GetLteUeOperationMode();
    ucSimCsRegStatus = NAS_MML_GetSimCsRegStatus();

    /* CS������ע���CS����Ч */
    if ((VOS_FALSE == NAS_MML_GetCsAttachAllowFlg())
     || (VOS_FALSE == ucSimCsRegStatus))
    {
        return VOS_FALSE;
    }

    /* ����CS/PS mode 1 */
    if (NAS_MML_LTE_UE_OPERATION_MODE_CS_PS_1 != enLUeMode)
    {
        return VOS_FALSE;
    }


    /* LTE only */
    if (VOS_TRUE == NAS_MML_IsLteOnlyMode(NAS_MML_GetMsPrioRatList()))
    {
        return VOS_FALSE;
    }

    /* NAS_MMC_GetRegRsltAttemptCounter�ﵽ5��,��ʾ֮ǰ����ע���EPS�ɹ�,T3402δ��ʱ */
    if (NAS_MML_MAX_PS_REG_FAIL_CNT == NAS_MMC_GetRegRsltAttemptCounter(NAS_MMC_GetRegRsltType()))
    {
        return VOS_TRUE;
    }

    /* ��#18�б��� */
    if (VOS_TRUE == NAS_MMC_IsPlmnIdInCsDomainUnAvailPlmnList(NAS_MML_GetCurrCampPlmnId()))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}

VOS_UINT32 NAS_MMC_IsPlmnIdInCsDomainUnAvailPlmnList(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
)
{
    NAS_MML_PLMN_ID_STRU               *pstPlmnList = VOS_NULL_PTR;
    NAS_LMM_INFO_STRU                   stLmmInfo;

    PS_MEM_SET(&stLmmInfo, 0x0, sizeof(NAS_LMM_INFO_STRU));

    if (VOS_TRUE != NAS_MMC_GetLteInfo(NAS_LMM_CS_DOMAIN_UNAVAILABLE_PLMN_LIST, &stLmmInfo))
    {
        return VOS_FALSE;
    }

    /* ���붯̬�ڴ� */
    pstPlmnList = (NAS_MML_PLMN_ID_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC, (MMC_LMM_MAX_PLMN_NUM*sizeof(NAS_MML_PLMN_ID_STRU)));

    /* �������ʧ�ܣ��򷵻� */
    if ( VOS_NULL_PTR == pstPlmnList )
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_IsAvailTimerExpiredTriggerPlmnSearch_L1Main: Dynamic Memory Alloc Error");

        return VOS_FALSE;
    }

    NAS_MMC_ConvertLmmPlmnToGUNasPlmn(stLmmInfo.u.stCsDomainUnAvailPlmnList.ulPlmnNum,
                                      stLmmInfo.u.stCsDomainUnAvailPlmnList.astPlmnId,
                                      pstPlmnList);

    /* ��#18�б��� */
    if (VOS_TRUE == NAS_MML_IsBcchPlmnIdInDestSimPlmnList(pstPlmnId,
                                         (VOS_UINT8)(stLmmInfo.u.stCsDomainUnAvailPlmnList.ulPlmnNum),
                                         pstPlmnList))
    {
        /* �ͷŶ�̬�ڴ� */
        PS_MEM_FREE(WUEPS_PID_MMC, pstPlmnList);

        return VOS_TRUE;
    }

    /* �ͷŶ�̬�ڴ� */
    PS_MEM_FREE(WUEPS_PID_MMC, pstPlmnList);

    return VOS_FALSE;
}
VOS_UINT32 NAS_MMC_IsCsPsMode1NeedPeriodSearchGU(VOS_VOID)
{
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstRatOrder = VOS_NULL_PTR;
    NAS_MML_LTE_UE_OPERATION_MODE_ENUM_UINT8                enLUeMode;
    NAS_MML_ADDITION_UPDATE_RSLT_INFO_ENUM_UINT8            enAdditionUpdateRslt;

    pstRatOrder          = NAS_MML_GetMsPrioRatList();
    enLUeMode            = NAS_MML_GetLteUeOperationMode();
    enAdditionUpdateRslt = NAS_MML_GetAdditionUpdateRslt();

    /* ��ǰLTE��ģ */
    if (VOS_TRUE == NAS_MML_IsLteOnlyMode(pstRatOrder))
    {
        return VOS_FALSE;
    }

    /* ��ǰ���뼼��ΪGU */
    if (NAS_MML_NET_RAT_TYPE_LTE != NAS_MML_GetCurrNetRatType())
    {
        return VOS_FALSE;
    }

    /* ����CS/PS mode 1��IMS VOICE���� */
    if ((VOS_TRUE                               == NAS_MML_GetImsVoiceAvailFlg())
     || (NAS_MML_LTE_UE_OPERATION_MODE_CS_PS_1  != enLUeMode))
    {
        return VOS_FALSE;
    }

    /* Additional Update Result IE��Ϣ��ָʾSMS Only��CSFB Not Preferred */
    if ((NAS_MML_ADDITION_UPDATE_RSLT_SMS_ONLY == enAdditionUpdateRslt)
     || (NAS_MML_ADDITION_UPDATE_RSLT_CSFB_NOT_PREFERRED == enAdditionUpdateRslt))
    {
        return VOS_TRUE;
    }

    /* NAS_MMC_GetRegRsltAttemptCounterΪ5 */
    if (NAS_MML_MAX_PS_REG_FAIL_CNT == NAS_MMC_GetRegRsltAttemptCounter(NAS_MMC_GetRegRsltType()))
    {
        return VOS_TRUE;
    }

    /* ��#18�б��� */
    if (VOS_TRUE == NAS_MMC_IsPlmnIdInCsDomainUnAvailPlmnList(NAS_MML_GetCurrCampPlmnId()))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_IsNeedDisableL_PsFailCause7PsServNotAllow(VOS_VOID)
{
    VOS_UINT8                                               ucSimCsRegStatus;

    ucSimCsRegStatus = NAS_MML_GetSimCsRegStatus();

    /* CS������ע���CS����Ч */
    if ((VOS_FALSE == NAS_MML_GetCsAttachAllowFlg())
     || (VOS_FALSE == ucSimCsRegStatus))
    {
        return VOS_FALSE;
    }

    /* ����Ѿ�disable ����Ҫ��disable LTE */
    if (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == NAS_MML_GetLteCapabilityStatus())
    {
        return VOS_FALSE;
    }

    /* disable ��enable lte�����ж�syscfg�Ƿ�֧��L */

    /* ƽ̨��֧��LTEʱ����Ҫdisable LTE */
    if (VOS_FALSE == NAS_MML_IsPlatformSupportLte())
    {
        return VOS_FALSE;
    }

    /* LTE only */
    if (VOS_TRUE == NAS_MML_IsLteOnlyMode(NAS_MML_GetMsPrioRatList()))
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_IsNeedEnableLte_CsRegFailCause2ImsiUnknownInHlr(VOS_VOID)
{
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;
    NAS_MML_SIM_TYPE_ENUM_UINT8                             enSimType;

    /* ����������������MMC��Ҫ����enable LTE��
    (1)����disable LTE�ı��Ϊ��
    (2)Csע��ʧ��ԭ��ֵ#2��#3��#6
    (3)PS����Ч��USIM�� */

    /* ��ȡLTE��ʹ��״̬ */
    enLteCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();
    enSimType              = NAS_MML_GetSimType();

    /* �������ʱ�Ѿ�����en_NV_Item_Rat_Forbidden_List_Accord_Imsi_Config�����ý�ֹ��LTE���򷵻�VOS_FALSE */
    if (NAS_MML_RAT_CAPABILITY_STATUS_DISABLE == NAS_MML_GetLteForbiddenStatusFlg())
    {
        return VOS_FALSE;
    }

    /* ���֮ǰû��disable LTE,����Ҫenable LTE */
    if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS != enLteCapabilityStatus)
     && (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS != enLteCapabilityStatus))
    {
        return VOS_FALSE;
    }

    /* PS����Ч��USIM��,��ҪEnable Lte */
    if ( (VOS_FALSE == NAS_MML_GetSimPsRegStatus())
      || (NAS_MML_SIM_TYPE_USIM == enSimType) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_IsNeedEnableLte_CsRegFailCause256AuthRej(VOS_VOID)
{
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;
    NAS_MML_SIM_TYPE_ENUM_UINT8                             enSimType;

    /* ����������������MMC��Ҫ����enable LTE��
    (1)����disable LTE�ı��Ϊ��
    (2)Csע��ʧ��ԭ��ֵ#256
    (3)PS����Ч��USIM�� */

    /* ��ȡLTE��ʹ��״̬ */
    enLteCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();
    enSimType              = NAS_MML_GetSimType();

    /* �������ʱ�Ѿ�����en_NV_Item_Rat_Forbidden_List_Accord_Imsi_Config�����ý�ֹ��LTE���򷵻�VOS_FALSE */
    if (NAS_MML_RAT_CAPABILITY_STATUS_DISABLE == NAS_MML_GetLteForbiddenStatusFlg())
    {
        return VOS_FALSE;
    }


    /* ���֮ǰû��disable LTE,����Ҫenable LTE */
    if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS != enLteCapabilityStatus)
     && (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS != enLteCapabilityStatus))
    {
        return VOS_FALSE;
    }

    /* PS����Ч��USIM��,��ҪEnable Lte */
    if ( (VOS_FALSE == NAS_MML_GetSimPsRegStatus())
      || (NAS_MML_SIM_TYPE_USIM == enSimType) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_IsNeedEnableLte_PsRegFailCause256AuthRej(VOS_VOID)
{
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;

    /* ����������������MMC��Ҫ����enable LTE��
    (1)����disable LTE�ı��Ϊ��
    (2)Cs����Psע��ʧ��ԭ��ֵ#256 */

    /* ��ȡLTE��ʹ��״̬ */
    enLteCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();

    /* �������ʱ�Ѿ�����en_NV_Item_Rat_Forbidden_List_Accord_Imsi_Config�����ý�ֹ��LTE���򷵻�VOS_FALSE */
    if (NAS_MML_RAT_CAPABILITY_STATUS_DISABLE == NAS_MML_GetLteForbiddenStatusFlg())
    {
        return VOS_FALSE;
    }

    /* ��SIM��,����ҪEnable Lte */
    if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
    {
        return VOS_FALSE;
    }

    /* ���֮ǰ�Ѿ�disable LTE,��Ҫenable LTE */
    if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == enLteCapabilityStatus)
     || (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLteCapabilityStatus))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_UINT32 NAS_MMC_IsNeedEnableLte_PsRegFailCause3IllegalMs(VOS_VOID)
{
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;

    /* ����������������MMC��Ҫ����enable LTE��
    (1)����disable LTE�ı��Ϊ��
    (2)Psע��ʧ��ԭ��ֵ#3 */

    /* ��ȡLTE��ʹ��״̬ */
    enLteCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();

    /* �������ʱ�Ѿ�����en_NV_Item_Rat_Forbidden_List_Accord_Imsi_Config�����ý�ֹ��LTE���򷵻�VOS_FALSE */
    if (NAS_MML_RAT_CAPABILITY_STATUS_DISABLE == NAS_MML_GetLteForbiddenStatusFlg())
    {
        return VOS_FALSE;
    }

    /* ��SIM��,����ҪEnable Lte */
    if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
    {
        return VOS_FALSE;
    }

    /* ���֮ǰ�Ѿ�disable LTE,��Ҫenable LTE */
    if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == enLteCapabilityStatus)
     || (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLteCapabilityStatus))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_IsNeedEnableLte_PsRegFailCause8GprsAndNonGprsNotAllow(VOS_VOID)
{
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;

    /* ����������������MMC��Ҫ����enable LTE��
    (1)����disable LTE�ı��Ϊ��
    (2)Psע��ʧ��ԭ��ֵ#8 */

    /* ��ȡLTE��ʹ��״̬ */
    enLteCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();

    /* �������ʱ�Ѿ�����en_NV_Item_Rat_Forbidden_List_Accord_Imsi_Config�����ý�ֹ��LTE���򷵻�VOS_FALSE */
    if (NAS_MML_RAT_CAPABILITY_STATUS_DISABLE == NAS_MML_GetLteForbiddenStatusFlg())
    {
        return VOS_FALSE;
    }

    /* ��SIM��,����ҪEnable Lte */
    if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
    {
        return VOS_FALSE;
    }

    /* ���֮ǰ�Ѿ�disable LTE,��Ҫenable LTE */
    if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == enLteCapabilityStatus)
     || (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLteCapabilityStatus))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32  NAS_MMC_IsNeedEnableLte_CmServiceRejectInd(
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCmServiceRejCause
)
{
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;
    NAS_MML_SIM_TYPE_ENUM_UINT8                             enSimType;

    /* ����������������MMC��Ҫ����enable LTE��
    (1)����disable LTE�ı��Ϊ��
    (2)Cm Service����ԭ��ֵ#6
    (3)PS����Ч��USIM�� */

    /* ��ȡLTE��ʹ��״̬ */
    enLteCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();
    enSimType              = NAS_MML_GetSimType();

    /* �������ʱ�Ѿ�����en_NV_Item_Rat_Forbidden_List_Accord_Imsi_Config�����ý�ֹ��LTE���򷵻�VOS_FALSE */
    if (NAS_MML_RAT_CAPABILITY_STATUS_DISABLE == NAS_MML_GetLteForbiddenStatusFlg())
    {
        return VOS_FALSE;
    }


    /* ���֮ǰû��disable LTE,����Ҫenable LTE */
    if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS != enLteCapabilityStatus)
     && (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS != enLteCapabilityStatus))
    {
        return VOS_FALSE;
    }

    /* ��SIM��,����ҪEnable Lte */
    if (NAS_MML_SIM_TYPE_SIM == enSimType)
    {
        return VOS_FALSE;
    }


    /* Cm Service����ԭ��ֵ#6����Ҫenable LTE */
    if (NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME == enCmServiceRejCause)
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;

}
VOS_UINT32  NAS_MMC_IsNeedEnableLte_MmAbortInd(
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enAbortCause
)
{
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;
    NAS_MML_SIM_TYPE_ENUM_UINT8                             enSimType;

    /* ����������������MMC��Ҫ����enable LTE��
    (1)����disable LTE�ı��Ϊ��
    (2)Mm Abortԭ��ֵ#6
    (3)PS����Ч��USIM�� */

    /* ��ȡLTE��ʹ��״̬ */
    enLteCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();
    enSimType              = NAS_MML_GetSimType();

    /* �������ʱ�Ѿ�����en_NV_Item_Rat_Forbidden_List_Accord_Imsi_Config�����ý�ֹ��LTE���򷵻�VOS_FALSE */
    if (NAS_MML_RAT_CAPABILITY_STATUS_DISABLE == NAS_MML_GetLteForbiddenStatusFlg())
    {
        return VOS_FALSE;
    }

    /* ���֮ǰδdisable LTE������Ҫenable LTE */
    if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS != enLteCapabilityStatus)
     && (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS != enLteCapabilityStatus))
    {
        return VOS_FALSE;
    }

    /* ��SIM��,����ҪEnable Lte */
    if (NAS_MML_SIM_TYPE_SIM == enSimType)
    {
        return VOS_FALSE;
    }
    

    /* Mm Abortԭ��ֵ#6����Ҫenable LTE */
    if (NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME == enAbortCause)
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;

}
VOS_UINT32  NAS_MMC_IsNeedEnableLte_EnableLteTimerExp(VOS_VOID)
{
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;
    NAS_MML_SIM_TYPE_ENUM_UINT8                             enSimType;

    MMC_LMM_DISABLE_LTE_REASON_ENUM_UINT32                  enDisableLteReason;

    enDisableLteReason = NAS_MML_GetDisableLteReason();

    /* ����������������MMC��Ҫ����enable LTE��
    (1)����disable LTE�ı��Ϊ��
    (2)disable LTEԭ��ֵEM��VOICE NOT SUPPORT
    (3)PS����Ч��USIM�� */

    /* ��ȡLTE��ʹ��״̬ */
    enLteCapabilityStatus  = NAS_MML_GetLteCapabilityStatus();
    enSimType              = NAS_MML_GetSimType();

    /* �������ʱ�Ѿ�����en_NV_Item_Rat_Forbidden_List_Accord_Imsi_Config�����ý�ֹ��LTE���򷵻�VOS_FALSE */
    if (NAS_MML_RAT_CAPABILITY_STATUS_DISABLE == NAS_MML_GetLteForbiddenStatusFlg())
    {
        return VOS_FALSE;
    }

    /* ���֮ǰδdisable LTE������Ҫenable LTE */
    if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS != enLteCapabilityStatus)
     && (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS != enLteCapabilityStatus))
    {
        return VOS_FALSE;
    }

    /* PS����Ч����SIM��,����ҪEnable Lte */
    if ( (VOS_TRUE == NAS_MML_GetSimPsRegStatus())
      && (NAS_MML_SIM_TYPE_SIM == enSimType) )
    {
        return VOS_FALSE;
    }

    /* disable LTEԭ��ΪEMC��IMS VOICE�����ã���Ҫenable LTE */
    if ((MMC_LMM_DISABLE_LTE_REASON_EMERGENCY_CALL          == enDisableLteReason)
     || (MMC_LMM_DISABLE_LTE_REASON_LTE_VOICE_NOT_AVAILABLE == enDisableLteReason))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;

}
VOS_VOID NAS_MMC_SaveEpsAttachAttemptCounter(
    LMM_MMC_ATTACH_IND_STRU            *pstEpsAttachIndMsg
)
{
    NAS_MMC_REG_RSLT_CTX_STRU          *pstRegRsltCtx = VOS_NULL_PTR;
    pstRegRsltCtx   = NAS_MMC_GetRegRsltCtxAddr();

    /* ��LMMģ��ȷ�ϣ����Attach���ΪCN REJ��MT_DETACH_FAILURE��MO_DETACH_FAILURE
       ���߳ɹ���Я����Cn Rslt���������Ϣ��Я����ulAttemptCount���£��������
       ���������������������Ϣ��Я����ulAttemptCount���£����򲻸��� */
    if ((MMC_LMM_ATT_RSLT_CN_REJ == pstEpsAttachIndMsg->ulAttachRslt)
     || (MMC_LMM_ATT_RSLT_MT_DETACH_FAILURE == pstEpsAttachIndMsg->ulAttachRslt)
     || (MMC_LMM_ATT_RSLT_MO_DETACH_FAILURE == pstEpsAttachIndMsg->ulAttachRslt)
     || ((MMC_LMM_ATT_RSLT_SUCCESS == pstEpsAttachIndMsg->ulAttachRslt)
      && (VOS_TRUE == pstEpsAttachIndMsg->bitOpCnRslt)))
    {
        if (VOS_TRUE == pstEpsAttachIndMsg->bitOpAtmpCnt)
        {
            pstRegRsltCtx->ucEpsAttemptCounter = (VOS_UINT8)pstEpsAttachIndMsg->ulAttemptCount;
        }
        else
        {
            pstRegRsltCtx->ucEpsAttemptCounter = 0x0;
        }
    }
    else
    {
        if (VOS_TRUE == pstEpsAttachIndMsg->bitOpAtmpCnt)
        {
            pstRegRsltCtx->ucEpsAttemptCounter = (VOS_UINT8)pstEpsAttachIndMsg->ulAttemptCount;
        }
    }

    return;
}


VOS_VOID NAS_MMC_SaveEpsTauAttemptCounter(
    LMM_MMC_TAU_RESULT_IND_STRU        *pstEpsTauIndMsg
)
{
    NAS_MMC_REG_RSLT_CTX_STRU          *pstRegRsltCtx = VOS_NULL_PTR;
    pstRegRsltCtx   = NAS_MMC_GetRegRsltCtxAddr();

    /* ��LMMģ��ȷ�ϣ����TAU���ΪCN REJ��MT_DETACH_FAILURE��MO_DETACH_FAILURE
       ���߳ɹ���Я����Cn Rslt���������Ϣ��Я����ulAttemptCount���£��������
       ���������������������Ϣ��Я����ulAttemptCount���£����򲻸��� */
    if ((MMC_LMM_TAU_RSLT_CN_REJ == pstEpsTauIndMsg->ulTauRst)
     || (MMC_LMM_TAU_RSLT_MT_DETACH_FAILURE == pstEpsTauIndMsg->ulTauRst)
     || (MMC_LMM_TAU_RSLT_MO_DETACH_FAILURE == pstEpsTauIndMsg->ulTauRst)
     || ((MMC_LMM_TAU_RSLT_SUCCESS == pstEpsTauIndMsg->ulTauRst)
      && (VOS_TRUE == pstEpsTauIndMsg->bitOpCnRst)))
    {
        if (VOS_TRUE == pstEpsTauIndMsg->bitOpAtmpCnt)
        {
            pstRegRsltCtx->ucEpsAttemptCounter = (VOS_UINT8)pstEpsTauIndMsg->ulAttemptCount;
        }
        else
        {
            pstRegRsltCtx->ucEpsAttemptCounter = 0x0;
        }
    }
    else
    {
        if (VOS_TRUE == pstEpsTauIndMsg->bitOpAtmpCnt)
        {
            pstRegRsltCtx->ucEpsAttemptCounter = (VOS_UINT8)pstEpsTauIndMsg->ulAttemptCount;
        }
    }

    return;
}


VOS_VOID NAS_MMC_ProcEmcPdpRelease_DetachPs( VOS_VOID )
{
    NAS_MML_EQUPLMN_INFO_STRU          *pstEquPlmnInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU           stSndEquPlmnInfo;
    VOS_UINT8                           ucEmcPdpStatusFlg;

    /* Э����������:
        1.if there is no PDN connection for emergency bearer services established, 
        the UE shall remove from the list any PLMN code that is already in the list of "forbidden PLMNs" 
        or in the list of "forbidden PLMNs for GPRS service". 
        If there is a PDN connection for emergency bearer services established, 
        the UE shall remove from the list of equivalent PLMNs any PLMN code present in the list of forbidden PLMNs 
        or in the list of "forbidden PLMNs for GPRS service" when the PDN connection for emergency bearer services is released.

        2.if the attach procedure is not for emergency bearer services, 
        the UE shall remove from the list any PLMN code that is already in the list of "forbidden PLMNs" 
        or in the list of "forbidden PLMNs for GPRS service".
    
        3.The UE shall delete the stored list if the USIM is removed 
        or when the UE attached for emergency bearer services enters the state EMM-DEREGISTERED.
    */

    ucEmcPdpStatusFlg   = NAS_MML_GetEmcPdpStatusFlg();

    /* ����PDN������ʱ������Ҫ��������PDN����ʱ�������־����LMM����EPLMN֪ͨ */
    if (VOS_TRUE == ucEmcPdpStatusFlg)
    {
        /* �������PDN���ӱ�־ */
        NAS_MML_SetEmcPdpStatusFlg(VOS_FALSE);

        
        /* ��������ʱ������Ҫ����EPLMN;������ʱ����LMM����EPLMN֪ͨ��ɾ��Forb Plmn */
        if (VOS_TRUE == NAS_MML_GetSimPresentStatus())
        {
            pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
            PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(NAS_MML_EQUPLMN_INFO_STRU));
            NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
            NAS_MMC_SndLmmEquPlmnReq(&stSndEquPlmnInfo);
        
            NAS_MMC_SndOmEquPlmn();
        }
    }

    return;
}
VOS_UINT32 NAS_MMC_IsNeedSndEplmn_LmmDetachCnf(
    LMM_MMC_DETACH_CNF_STRU            *pstLmmDetachCnf
)
{
    VOS_UINT32                          ulRst;

    ulRst   = VOS_FALSE;

    /* ����������PS��CS+PS��detach��detach��Ϊfailureʱ��
        ����Ϊ����PDN�����ͷţ��������PDN���ӱ�ǣ�
        ͬʱ֪ͨLMM��ǰEPLMN�б���Ϣ��ɾ��forbidden plmn�б�
    */

    switch (pstLmmDetachCnf->ulReqType)
    {
        case MMC_LMM_MO_DET_PS_ONLY:
        case MMC_LMM_MO_DET_CS_PS:

            if ( (MMC_LMM_DETACH_RSLT_SUCCESS       == pstLmmDetachCnf->ulDetachRslt)
              || (MMC_LMM_DETACH_RSLT_ACCESS_BARED  == pstLmmDetachCnf->ulDetachRslt)
              || (MMC_LMM_DETACH_RSLT_AUTH_REJ      == pstLmmDetachCnf->ulDetachRslt) )
            {
                ulRst = VOS_TRUE;
            }
            
            break;

        default:

            ulRst   = VOS_FALSE;

            break;
    }

    return ulRst;
}



VOS_VOID NAS_MMC_ConvertLmmDetachIndTypeToMmcType(
    MMC_LMM_MT_DETACH_TYPE_ENUM_UINT32                      enCnReqType,
    MMA_MMC_NETWORK_DETACH_TYPE_ENUM_UINT8                 *penDetachType
)
{
    switch ( enCnReqType )
    {
        case MMC_LMM_MT_DET_REATTACH_REQUIRED:
            *penDetachType = MMA_MMC_NETWORK_DETACH_TYPE_REATTACH;
            break;

        case MMC_LMM_MT_DET_REATTACH_NOT_REQUIRED:
            *penDetachType = MMA_MMC_NETWORK_DETACH_TYPE_NOATTACH;
            break;

        case MMC_LMM_MT_DET_IMSI:
            *penDetachType = MMA_MMC_NETWORK_DETACH_TYPE_IMSI;
            break;

        default:
            *penDetachType = MMA_MMC_NETWORK_DETACH_TYPE_BUTT;
            break;
    }
}

#endif


VOS_VOID    NAS_MMC_ConverGmmActionTypeToMml(
    GMM_MMC_ACTION_TYPE_ENUM_U32        enGmmActionType,
    NAS_MML_PROC_TYPE_ENUM_U32         *penProcType
)
{
    switch (enGmmActionType)
    {
        case GMM_MMC_ACTION_RAU:

            *penProcType = NAS_MML_PROC_RAU;
            break;

        case GMM_MMC_ACTION_ATTACH:

            *penProcType = NAS_MML_PROC_ATTACH;
            break;

        case GMM_MMC_ACTION_COMBINED_RAU:

            *penProcType = NAS_MML_PROC_COMBINED_RAU;
            break;

        case GMM_MMC_ACTION_COMBINED_ATTACH:

            *penProcType = NAS_MML_PROC_COMBINED_ATTACH;
            break;
        
        case GMM_MMC_ACTION_PERIODC_RAU:
            
            *penProcType = NAS_MML_PROC_PERIODC_RAU;
            break;

        default:

            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ConverGmmActionTypeToMml: Invalid action type!");
            *penProcType = NAS_MML_PROC_BUTT;
            break;
    }
}



NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8    NAS_MMC_GetSingleDomainFailAction(
    NAS_MML_REG_DOMAIN_ENUM_UINT8           enDomain,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16      enFailCause
)
{
    VOS_UINT8                                           i;
    NAS_MML_SINGLE_DOMAIN_FAIL_ACTION_LIST_CTX_STRU*    pstSingleDomainFailActionCtx;

    pstSingleDomainFailActionCtx = NAS_MML_GetSingleDomainFailActionCtx();

    /* ����NVû�м��� */
    if (VOS_FALSE == pstSingleDomainFailActionCtx->ucActiveFlag)
    {
        return NAS_MMC_ADDITIONAL_ACTION_BUTT;
    }

    for (i = 0; i < pstSingleDomainFailActionCtx->ucCount; i++)
    {
        /* ��Ӧ��ע�����ע��ԭ��ֵ�ڶ����б��� */
        if ((enDomain == pstSingleDomainFailActionCtx->astSingleDomainFailActionList[i].enDomain)
         && (enFailCause == pstSingleDomainFailActionCtx->astSingleDomainFailActionList[i].enRegCause))
        {
            switch (pstSingleDomainFailActionCtx->astSingleDomainFailActionList[i].enAction)
            {
                case NAS_MML_SINGLE_DOMAIN_REG_FAIL_ACTION_PLMN_SELECTION:
                     return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;

                case NAS_MML_SINGLE_DOMAIN_REG_FAIL_ACTION_NORMAL_CAMP_ON:
                     return NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON;

                case NAS_MML_SINGLE_DOMAIN_REG_FAIL_ACTION_OPTIONAL_PLMN_SELECTION:
                     return NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION;

                case NAS_MML_SINGLE_DOMAIN_REG_FAIL_ACTION_LIMITED_CAMP_ON:
                     return NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON;

                case NAS_MML_SINGLE_DOMAIN_ROAMING_REG_FAIL_ACTION_PLMN_SELECTION:

                     /* ��������������������������ԭ�����̴��� */
                     if ( VOS_TRUE == NAS_MMC_IsRoam() )
                     {
                         return NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION;
                     }
                     
                     return NAS_MMC_ADDITIONAL_ACTION_BUTT;

                default:
                     return NAS_MMC_ADDITIONAL_ACTION_BUTT;
            }
        }
    }

    return NAS_MMC_ADDITIONAL_ACTION_BUTT;
}
VOS_UINT32 NAS_MMC_IsCurrPlmnInEplmnList(VOS_VOID)
{
    VOS_UINT32                          i;
    NAS_MML_PLMN_ID_STRU               *pstCurplmn   =  VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU          *pstEplmnList =  VOS_NULL_PTR;
    
    i            = 0;
    pstCurplmn   = NAS_MML_GetCurrCampPlmnId();
    pstEplmnList = NAS_MML_GetEquPlmnList();
    
    if ((VOS_FALSE == NAS_MML_GetEplmnValidFlg())
     || (0         == pstEplmnList->ucEquPlmnNum))
    {
        return VOS_FALSE;
    }

    for ( i = 0 ; i < pstEplmnList->ucEquPlmnNum; i++ )
    {
        if ( ( pstCurplmn->ulMcc == pstEplmnList->astEquPlmnAddr[i].ulMcc)
          && ( pstCurplmn->ulMnc == pstEplmnList->astEquPlmnAddr[i].ulMnc))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
    
}



VOS_UINT32 NAS_MMC_IsAccBarPlmnSearch(VOS_VOID)
{
    VOS_UINT32                          ulIsRoam;
    VOS_UINT32                          ulIsAccBarPlmnSearchFlg;
    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;
    NAS_MMC_TIMER_STATUS_ENUM_U8        enAvailableTimerStatus;
    NAS_MML_PLMN_ID_STRU                stRPlmnId;
    NAS_MML_PLMN_ID_STRU               *pstCurplmn =  VOS_NULL_PTR;

    /* ��ȡNV���Կ��� */
    ulIsAccBarPlmnSearchFlg = (VOS_UINT32)NAS_MML_GetSupportAccBarPlmnSearchFlg();

    /* ��ȡ����״̬ */
    ulIsRoam                = NAS_MMC_IsRoam();

    /* ��ȡ��ǰCamp������*/
    pstCurplmn              = NAS_MML_GetCurrCampPlmnId();

    /* ��ȡ��ǰ״̬��Id*/
    enFsmId                 = NAS_MMC_GetCurrFsmId();

    /* ��ѯMMC_AVAILABLE_TIMER��ʱ��״̬*/
    enAvailableTimerStatus  = NAS_MMC_GetTimerStatus(TI_NAS_MMC_AVAILABLE_TIMER);

    if (VOS_FALSE == NAS_MMC_IsNeedPlmnSearch_RegLocalFailCause())
    {
        return VOS_FALSE;
    }


    /* ���Ӳ��Կ����������Կ�ʱ���޸ģ�ֱ�ӷ��� */
    if (VOS_TRUE == NAS_USIMMAPI_IsTestCard())
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_IsAccBarPlmnSearch(): The sim is Test card!");
        return VOS_FALSE;
    }

    /* ���Կ���δ�򿪻������״̬�������ֹ����Ҫ���� */
    if ((VOS_FALSE == ulIsAccBarPlmnSearchFlg)
     || (VOS_FALSE == ulIsRoam))
    {
        return VOS_FALSE;
    }

    /* ��L1״̬������available��ʱ��������ʱ������Ҫ�ٴ������� */
    if ((NAS_MMC_FSM_L1_MAIN == enFsmId)
     && (NAS_MMC_TIMER_STATUS_RUNING == enAvailableTimerStatus))
    {
        return VOS_FALSE;
    }

    /* ��ǰRplmn������ʱ����Ҫ���� */
    if (VOS_FALSE == NAS_MML_GetRPlmn(NAS_MML_GetLastRplmnRat(), &stRPlmnId))
    {
        return VOS_TRUE;
    }

    /* �����ǰ����ΪEPLMN����Ҫ���� */
    if (VOS_TRUE == NAS_MMC_IsCurrPlmnInEplmnList())
    {
        return VOS_FALSE;
    }

    /* פ����ֹ����ΪRPLMN������Ҫ����*/
    /* ���ﻹ����Ҫ�ж�RPLMN����ֹRPLMN��ЧEPLMN�ĳ�����©�� */
    if ( VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstCurplmn, &stRPlmnId))
    {
        return VOS_FALSE;
    }
    else
    {
        return VOS_TRUE;
    }
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

