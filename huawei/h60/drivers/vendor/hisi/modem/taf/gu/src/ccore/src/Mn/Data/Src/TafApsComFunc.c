

#ifdef __cplusplus
#if __cplusplus
    extern "C" {
#endif
#endif

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "Taf_Aps.h"
#include "TafLog.h"
#include "TafApsCtx.h"
#include "TafApsFsmMainTbl.h"
#include "TafApsComFunc.h"
#include "MnApsComm.h"


#include "NasComm.h"
#include "TafSdcCtx.h"
#include "TafSdcLib.h"

#include "MnApsMultiMode.h"



/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_TAF_APS_COMFUNC_C

/******************************************************************************
   2 �ⲿ������������
******************************************************************************/

/******************************************************************************
   3 ˽�ж���
******************************************************************************/


/******************************************************************************
   4 ȫ�ֱ�������
*****************************************************************************/

TAF_APS_SM_ERR_CODE_MAP_STRU            g_astTafApsSmErrCodeMapTbl[] =
{
    { TAF_PS_CAUSE_SM_CONN_ESTABLISH_MAX_TIME_OUT,          SM_TAF_CAUSE_SM_CONN_ESTABLISH_MAX_TIME_OUT             },
    { TAF_PS_CAUSE_SM_MAX_TIME_OUT,                         SM_TAF_CAUSE_SM_MAX_TIME_OUT                            },
    { TAF_PS_CAUSE_SM_INVALID_NSAPI,                        SM_TAF_CAUSE_SM_INVALID_NSAPI                           },
    { TAF_PS_CAUSE_SM_MODIFY_COLLISION,                     SM_TAF_CAUSE_SM_MODIFY_COLLISION                        },
    { TAF_PS_CAUSE_SM_DUPLICATE,                            SM_TAF_CAUSE_SM_DUPLICATE                               },
    { TAF_PS_CAUSE_SM_RAB_SETUP_FAILURE,                    SM_TAF_CAUSE_SM_RAB_SETUP_FAIL                          },
    { TAF_PS_CAUSE_SM_SGSN_VER_PRE_R99,                     SM_TAF_CAUSE_SM_SGSN_VER_PRE_R99                        },
    { TAF_PS_CAUSE_UNKNOWN,                                 SM_TAF_CAUSE_SM_UNKNOWN                                 },

    { TAF_PS_CAUSE_SM_NW_OPERATOR_DETERMINED_BARRING,       SM_TAF_CAUSE_SM_NW_OPERATOR_DETERMINED_BARRING          },
    { TAF_PS_CAUSE_SM_NW_MBMS_BC_INSUFFICIENT,              SM_TAF_CAUSE_SM_NW_MBMS_BC_INSUFFICIENT                 },
    { TAF_PS_CAUSE_SM_NW_LLC_OR_SNDCP_FAILURE,              SM_TAF_CAUSE_SM_NW_LLC_OR_SNDCP_FAILURE                 },
    { TAF_PS_CAUSE_SM_NW_INSUFFICIENT_RESOURCES,            SM_TAF_CAUSE_SM_NW_INSUFFICIENT_RESOURCES               },
    { TAF_PS_CAUSE_SM_NW_MISSING_OR_UKNOWN_APN,             SM_TAF_CAUSE_SM_NW_MISSING_OR_UKNOWN_APN                },
    { TAF_PS_CAUSE_SM_NW_UNKNOWN_PDP_ADDR_OR_TYPE,          SM_TAF_CAUSE_SM_NW_UNKNOWN_PDP_ADDR_OR_TYPE             },
    { TAF_PS_CAUSE_SM_NW_USER_AUTHENTICATION_FAIL,          SM_TAF_CAUSE_SM_NW_USER_AUTHENTICATION_FAIL             },
    { TAF_PS_CAUSE_SM_NW_ACTIVATION_REJECTED_BY_GGSN,       SM_TAF_CAUSE_SM_NW_ACTIVATION_REJECTED_BY_GGSN          },
    { TAF_PS_CAUSE_SM_NW_ACTIVATION_REJECTED_UNSPECIFIED,   SM_TAF_CAUSE_SM_NW_ACTIVATION_REJECTED_UNSPECIFIED      },
    { TAF_PS_CAUSE_SM_NW_SERVICE_OPTION_NOT_SUPPORTED,      SM_TAF_CAUSE_SM_NW_SERVICE_OPTION_NOT_SUPPORTED         },
    { TAF_PS_CAUSE_SM_NW_REQUESTED_SERVICE_NOT_SUBSCRIBED,  SM_TAF_CAUSE_SM_NW_REQUESTED_SERVICE_NOT_SUBSCRIBED     },
    { TAF_PS_CAUSE_SM_NW_SERVICE_OPTION_TEMP_OUT_ORDER,     SM_TAF_CAUSE_SM_NW_SERVICE_OPTION_TEMP_OUT_ORDER        },
    { TAF_PS_CAUSE_SM_NW_NSAPI_ALREADY_USED,                SM_TAF_CAUSE_SM_NW_NSAPI_ALREADY_USED                   },
    { TAF_PS_CAUSE_SM_NW_REGULAR_DEACTIVATION,              SM_TAF_CAUSE_SM_NW_REGULAR_DEACTIVATION                 },
    { TAF_PS_CAUSE_SM_NW_QOS_NOT_ACCEPTED,                  SM_TAF_CAUSE_SM_NW_QOS_NOT_ACCEPTED                     },
    { TAF_PS_CAUSE_SM_NW_NETWORK_FAILURE,                   SM_TAF_CAUSE_SM_NW_NETWORK_FAILURE                      },
    { TAF_PS_CAUSE_SM_NW_REACTIVATION_REQUESTED,            SM_TAF_CAUSE_SM_NW_REACTIVATION_REQUESTED               },
    { TAF_PS_CAUSE_SM_NW_FEATURE_NOT_SUPPORT,               SM_TAF_CAUSE_SM_NW_FEATURE_NOT_SUPPORT                  },
    { TAF_PS_CAUSE_SM_NW_SEMANTIC_ERR_IN_TFT,               SM_TAF_CAUSE_SM_NW_SEMANTIC_ERR_IN_TFT                  },
    { TAF_PS_CAUSE_SM_NW_SYNTACTIC_ERR_IN_TFT,              SM_TAF_CAUSE_SM_NW_SYNTACTIC_ERR_IN_TFT                 },
    { TAF_PS_CAUSE_SM_NW_UKNOWN_PDP_CONTEXT,                SM_TAF_CAUSE_SM_NW_UKNOWN_PDP_CONTEXT                   },
    { TAF_PS_CAUSE_SM_NW_SEMANTIC_ERR_IN_PACKET_FILTER,     SM_TAF_CAUSE_SM_NW_SEMANTIC_ERR_IN_PACKET_FILTER        },
    { TAF_PS_CAUSE_SM_NW_SYNCTACTIC_ERR_IN_PACKET_FILTER,   SM_TAF_CAUSE_SM_NW_SYNCTACTIC_ERR_IN_PACKET_FILTER      },
    { TAF_PS_CAUSE_SM_NW_PDP_CONTEXT_WITHOUT_TFT_ACTIVATED, SM_TAF_CAUSE_SM_NW_PDP_CONTEXT_WITHOUT_TFT_ACTIVATED    },
    { TAF_PS_CAUSE_SM_NW_MULTICAST_GROUP_MEMBERHHSHIP_TIMEOUT,  SM_TAF_CAUSE_SM_NW_MULTICAST_GROUP_MEMBERHHSHIP_TIMEOUT },
    { TAF_PS_CAUSE_SM_NW_REQUEST_REJECTED_BCM_VIOLATION,    SM_TAF_CAUSE_SM_NW_REQUEST_REJECTED_BCM_VIOLATION       },
    { TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV4_ONLY_ALLOWED,        SM_TAF_CAUSE_SM_NW_PDP_TYPE_IPV4_ONLY_ALLOWED           },
    { TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV6_ONLY_ALLOWED,        SM_TAF_CAUSE_SM_NW_PDP_TYPE_IPV6_ONLY_ALLOWED           },
    { TAF_PS_CAUSE_SM_NW_SINGLE_ADDR_BEARERS_ONLY_ALLOWED,  SM_TAF_CAUSE_SM_NW_SINGLE_ADDR_BEARERS_ONLY_ALLOWED     },
    { TAF_PS_CAUSE_SM_NW_COLLISION_WITH_NW_INITIATED_REQUEST,   SM_TAF_CAUSE_SM_NW_COLLISION_WITH_NW_INITIATED_REQUEST  },
    { TAF_PS_CAUSE_SM_NW_BEARER_HANDLING_NOT_SUPPORTED,     SM_TAF_CAUSE_SM_NW_BEARER_HANDLING_NOT_SUPPORTED        },
    { TAF_PS_CAUSE_SM_NW_INVALID_TI,                        SM_TAF_CAUSE_SM_NW_INVALID_TI                           },
    { TAF_PS_CAUSE_SM_NW_SEMANTICALLY_INCORRECT_MESSAGE,    SM_TAF_CAUSE_SM_NW_SEMANTICALLY_INCORRECT_MESSAGE             },
    { TAF_PS_CAUSE_SM_NW_INVALID_MANDATORY_INFO,            SM_TAF_CAUSE_SM_NW_INVALID_MANDATORY_INFO               },
    { TAF_PS_CAUSE_SM_NW_MSG_TYPE_NON_EXISTENT,             SM_TAF_CAUSE_SM_NW_MSG_TYPE_NON_EXISTENT                },
    { TAF_PS_CAUSE_SM_NW_MSG_TYPE_NOT_COMPATIBLE,           SM_TAF_CAUSE_SM_NW_MSG_TYPE_NOT_COMPATIBLE              },
    { TAF_PS_CAUSE_SM_NW_IE_NON_EXISTENT,                   SM_TAF_CAUSE_SM_NW_IE_NON_EXISTENT                      },
    { TAF_PS_CAUSE_SM_NW_CONDITIONAL_IE_ERR,                SM_TAF_CAUSE_SM_NW_CONDITIONAL_IE_ERR                   },
    { TAF_PS_CAUSE_SM_NW_MSG_NOT_COMPATIBLE,                SM_TAF_CAUSE_SM_NW_MSG_NOT_COMPATIBLE                   },
    { TAF_PS_CAUSE_SM_NW_PROTOCOL_ERR_UNSPECIFIED,          SM_TAF_CAUSE_SM_NW_PROTOCOL_ERR_UNSPECIFIED             },
    { TAF_PS_CAUSE_SM_NW_APN_RESTRICTION_INCOMPATIBLE,      SM_TAF_CAUSE_SM_NW_APN_RESTRICTION_INCOMPATIBLE         },

    { TAF_PS_CAUSE_GMM_GPRS_NOT_SUPPORT,                    SM_TAF_CAUSE_GMM_GPRS_NOT_SUPPORT                       },
    { TAF_PS_CAUSE_UNKNOWN,                                 SM_TAF_CAUSE_GMM_ATTACH_MAX_TIMES                       },
    { TAF_PS_CAUSE_GMM_FORBID_LA,                           SM_TAF_CAUSE_GMM_FORBID_LA                              },
    { TAF_PS_CAUSE_GMM_AUTHENTICATION_FAIL,                 SM_TAF_CAUSE_GMM_AUTHENTICATION_FAIL                    },
    { TAF_PS_CAUSE_GMM_AUTHENTICATION_FAIL,                 SM_TAF_CAUSE_GMM_AUTHENTICATION_REJ                     },
    { TAF_PS_CAUSE_GMM_PS_DETACH,                           SM_TAF_CAUSE_GMM_PS_DETACH                              },
    { TAF_PS_CAUSE_UNKNOWN,                                 SM_TAF_CAUSE_GMM_RRC_EST_FAIL                           },
    { TAF_PS_CAUSE_GMM_ACCESS_BARRED,                       SM_TAF_CAUSE_GMM_ACCESS_BARRED                          },
    { TAF_PS_CAUSE_SIM_INVALID,                             SM_TAF_CAUSE_GMM_SIM_PS_DOMAIN_REG_INVALID              },
    { TAF_PS_CAUSE_UNKNOWN,                                 SM_TAF_CAUSE_GMM_T3310_EXPIRED                          },
    { TAF_PS_CAUSE_UNKNOWN,                                 SM_TAF_CAUSE_GMM_T3317_EXPIRED                          },
    { TAF_PS_CAUSE_UNKNOWN,                                 SM_TAF_CAUSE_GMM_TIMER_SIGNALING_PROTECT_EXPIRED        },
    { TAF_PS_CAUSE_UNKNOWN,                                 SM_TAF_CAUSE_GMM_STARTUP                                },
    { TAF_PS_CAUSE_UNKNOWN,                                 SM_TAF_CAUSE_GMM_NULL                                   },
    { TAF_PS_CAUSE_UNKNOWN,                                 SM_TAF_CAUSE_GMM_RESET                                  },
    { TAF_PS_CAUSE_GMM_NO_PDP_CONTEXT,                      SM_TAF_CAUSE_GMM_RAU_ACCEPT                             },
    { TAF_PS_CAUSE_GMM_NO_PDP_CONTEXT,                      SM_TAF_CAUSE_GMM_SERVICE_ACCEPT                         },
    { TAF_PS_CAUSE_UNKNOWN,                                 SM_TAF_CAUSE_GMM_UNKNOWN                                },

    { TAF_PS_CAUSE_GMM_NW_IMSI_UNKNOWN_IN_HLR,              SM_TAF_CAUSE_GMM_NW_IMSI_UNKNOWN_IN_HLR                 },
    { TAF_PS_CAUSE_SIM_INVALID,                             SM_TAF_CAUSE_GMM_NW_ILLEGAL_MS                          },
    { TAF_PS_CAUSE_GMM_NW_IMSI_NOT_ACCEPTED,                SM_TAF_CAUSE_GMM_NW_IMSI_NOT_ACCEPTED                   },
    { TAF_PS_CAUSE_SIM_INVALID,                             SM_TAF_CAUSE_GMM_NW_ILLEGAL_ME                          },
    { TAF_PS_CAUSE_SIM_INVALID,                             SM_TAF_CAUSE_GMM_NW_GPRS_SERV_NOT_ALLOW                 },
    { TAF_PS_CAUSE_SIM_INVALID,                             SM_TAF_CAUSE_GMM_NW_GPRS_SERV_AND_NON_GPRS_SERV_NOT_ALLOW  },
    { TAF_PS_CAUSE_GMM_NW_MS_ID_NOT_DERIVED,                SM_TAF_CAUSE_GMM_NW_MS_ID_NOT_DERIVED                   },
    { TAF_PS_CAUSE_GMM_NW_IMPLICIT_DETACHED,                SM_TAF_CAUSE_GMM_NW_IMPLICIT_DETACHED                   },
    { TAF_PS_CAUSE_GMM_NW_PLMN_NOT_ALLOW,                   SM_TAF_CAUSE_GMM_NW_PLMN_NOT_ALLOW                      },
    { TAF_PS_CAUSE_GMM_NW_LA_NOT_ALLOW,                     SM_TAF_CAUSE_GMM_NW_LA_NOT_ALLOW                        },
    { TAF_PS_CAUSE_GMM_NW_ROAMING_NOT_ALLOW_IN_LA,          SM_TAF_CAUSE_GMM_NW_ROAMING_NOT_ALLOW_IN_LA             },
    { TAF_PS_CAUSE_GMM_NW_GPRS_SERV_NOT_ALLOW_IN_PLMN,      SM_TAF_CAUSE_GMM_NW_GPRS_SERV_NOT_ALLOW_IN_PLMN         },
    { TAF_PS_CAUSE_GMM_NW_NO_SUITABL_CELL,                  SM_TAF_CAUSE_GMM_NW_NO_SUITABL_CELL                     },
    { TAF_PS_CAUSE_GMM_NW_MSC_UNREACHABLE,                  SM_TAF_CAUSE_GMM_NW_MSC_UNREACHABLE                     },
    { TAF_PS_CAUSE_GMM_NW_NETWORK_FAILURE,                  SM_TAF_CAUSE_GMM_NW_NETWORK_FAILURE                     },
    { TAF_PS_CAUSE_GMM_NW_MAC_FAILURE,                      SM_TAF_CAUSE_GMM_NW_MAC_FAILURE                         },
    { TAF_PS_CAUSE_GMM_NW_SYNCH_FAILURE,                    SM_TAF_CAUSE_GMM_NW_SYNCH_FAILURE                       },
    { TAF_PS_CAUSE_GMM_NW_PROCEDURE_CONGESTION,             SM_TAF_CAUSE_GMM_NW_PROCEDURE_CONGESTION                },
    { TAF_PS_CAUSE_GMM_NW_GSM_AUT_UNACCEPTABLE,             SM_TAF_CAUSE_GMM_NW_GSM_AUT_UNACCEPTABLE                },
    { TAF_PS_CAUSE_GMM_NW_NOT_AUTHORIZED_FOR_THIS_CSG,      SM_TAF_CAUSE_GMM_NW_NOT_AUTHORIZED_FOR_THIS_CSG         },
    { TAF_PS_CAUSE_GMM_NW_NO_PDP_CONTEXT_ACT,               SM_TAF_CAUSE_GMM_NW_NO_PDP_CONTEXT_ACT                  },
    { TAF_PS_CAUSE_GMM_NW_RETRY_UPON_ENTRY_CELL,            SM_TAF_CAUSE_GMM_NW_RETRY_UPON_ENTRY_CELL               },
    { TAF_PS_CAUSE_GMM_NW_SEMANTICALLY_INCORRECT_MSG,       SM_TAF_CAUSE_GMM_NW_SEMANTICALLY_INCORRECT_MSG          },
    { TAF_PS_CAUSE_GMM_NW_INVALID_MANDATORY_INF,            SM_TAF_CAUSE_GMM_NW_INVALID_MANDATORY_INF               },
    { TAF_PS_CAUSE_GMM_NW_MSG_NONEXIST_NOTIMPLEMENTE,       SM_TAF_CAUSE_GMM_NW_MSG_NONEXIST_NOTIMPLEMENTE          },
    { TAF_PS_CAUSE_GMM_NW_MSG_TYPE_NOT_COMPATIBLE,          SM_TAF_CAUSE_GMM_NW_MSG_TYPE_NOT_COMPATIBLE             },
    { TAF_PS_CAUSE_GMM_NW_IE_NONEXIST_NOTIMPLEMENTED,       SM_TAF_CAUSE_GMM_NW_IE_NONEXIST_NOTIMPLEMENTED          },
    { TAF_PS_CAUSE_GMM_NW_CONDITIONAL_IE_ERROR,             SM_TAF_CAUSE_GMM_NW_CONDITIONAL_IE_ERROR                },
    { TAF_PS_CAUSE_GMM_NW_MSG_NOT_COMPATIBLE,               SM_TAF_CAUSE_GMM_NW_MSG_NOT_COMPATIBLE                  },
    { TAF_PS_CAUSE_GMM_NW_PROTOCOL_ERROR,                   SM_TAF_CAUSE_GMM_NW_PROTOCOL_ERROR                      }
};

#if (FEATURE_ON == FEATURE_LTE)
TAF_APS_L4A_ERR_CODE_MAP_STRU           g_astTafApsL4aErrCodeMapTbl[] =
{
    { TAF_PS_CAUSE_SUCCESS,                                  APS_L4A_SUCCESS                                        },
    { TAF_PS_CAUSE_INVALID_PARAMETER,                        APS_L4A_ERR_PARA_ERROR                                 },
    { TAF_PS_CAUSE_SM_NW_PROTOCOL_ERR_UNSPECIFIED,           APS_L4A_ERR_TIME_OUT                                   },
    { TAF_PS_CAUSE_GMM_PS_DETACH,                            APS_L4A_ERR_SM_DETACHED                                },

    { TAF_PS_CAUSE_SM_NW_OPERATOR_DETERMINED_BARRING,        APS_L4A_ERR_SM_NW_OPERATOR_DETERMINED_BARRING          },
    { TAF_PS_CAUSE_SM_NW_INSUFFICIENT_RESOURCES,             APS_L4A_ERR_SM_NW_INSUFFICIENT_RESOURCES               },
    { TAF_PS_CAUSE_SM_NW_MISSING_OR_UKNOWN_APN,              APS_L4A_ERR_SM_NW_MISSING_OR_UKNOWN_APN                },
    { TAF_PS_CAUSE_SM_NW_UNKNOWN_PDP_ADDR_OR_TYPE,           APS_L4A_ERR_SM_NW_UNKNOWN_PDN_TYPE                     },
    { TAF_PS_CAUSE_SM_NW_USER_AUTHENTICATION_FAIL ,          APS_L4A_ERR_SM_NW_USER_AUTH_FAIL                       },
    { TAF_PS_CAUSE_SM_NW_ACTIVATION_REJECTED_BY_GGSN,        APS_L4A_ERR_SM_NW_REQ_REJ_BY_SGW_OR_PGW                },
    { TAF_PS_CAUSE_SM_NW_ACTIVATION_REJECTED_UNSPECIFIED,    APS_L4A_ERR_SM_NW_REQ_REJ_UNSPECITY                    },
    { TAF_PS_CAUSE_SM_NW_SERVICE_OPTION_NOT_SUPPORTED,       APS_L4A_ERR_SM_NW_SERVICE_OPTION_NOT_SUPPORT           },
    { TAF_PS_CAUSE_SM_NW_REQUESTED_SERVICE_NOT_SUBSCRIBED,   APS_L4A_ERR_SM_NW_REQ_SERVICE_NOT_SUBSCRIBE            },
    { TAF_PS_CAUSE_SM_NW_SERVICE_OPTION_TEMP_OUT_ORDER,      APS_L4A_ERR_SM_NW_SERVICE_OPTION_TEMP_OUT_ORDER        },
    { TAF_PS_CAUSE_SM_NW_NSAPI_ALREADY_USED,                 APS_L4A_ERR_SM_NW_PTI_ALREADY_IN_USE                   },
    { TAF_PS_CAUSE_SM_NW_REGULAR_DEACTIVATION,               APS_L4A_ERR_SM_NW_REGULAR_DEACT                        },
    { TAF_PS_CAUSE_SM_NW_QOS_NOT_ACCEPTED,                   APS_L4A_ERR_SM_NW_EPS_QOS_NOT_ACCEPT                   },
    { TAF_PS_CAUSE_SM_NW_NETWORK_FAILURE,                    APS_L4A_ERR_SM_NW_NET_FAIL                             },
    { TAF_PS_CAUSE_SM_NW_SEMANTIC_ERR_IN_TFT,                APS_L4A_ERR_SM_NW_SEMANTIC_ERR_IN_TFT                  },
    { TAF_PS_CAUSE_SM_NW_SYNTACTIC_ERR_IN_TFT,               APS_L4A_ERR_SM_NW_SYNTACTIC_ERR_IN_TFT                 },
    { TAF_PS_CAUSE_SM_NW_UKNOWN_PDP_CONTEXT,                 APS_L4A_ERR_SM_NW_INVALID_EPS_BERER_IDENTITY           },
    { TAF_PS_CAUSE_SM_NW_SEMANTIC_ERR_IN_PACKET_FILTER,      APS_L4A_ERR_SM_NW_SEMANTIC_ERR_IN_PACKET_FILTER        },
    { TAF_PS_CAUSE_SM_NW_SYNCTACTIC_ERR_IN_PACKET_FILTER,    APS_L4A_ERR_SM_NW_SYNCTACTIC_ERR_IN_PACKET_FILTER      },
    { TAF_PS_CAUSE_SM_NW_PDP_CONTEXT_WITHOUT_TFT_ACTIVATED,  APS_L4A_ERR_SM_NW_BEARER_WITHOUT_TFT_ACT               },
    { TAF_PS_CAUSE_SM_NW_LAST_PDN_DISCONN_NOT_ALLOWED,       APS_L4A_ERR_SM_NW_LAST_PDN_DISCONN_NOT_ALLOWED         },
    { TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV4_ONLY_ALLOWED,         APS_L4A_ERR_SM_NW_PDN_TPYE_IPV4_ONLY_ALLOWED           },
    { TAF_PS_CAUSE_SM_NW_PDP_TYPE_IPV6_ONLY_ALLOWED,         APS_L4A_ERR_SM_NW_PDN_TPYE_IPV6_ONLY_ALLOWED           },
    { TAF_PS_CAUSE_SM_NW_SINGLE_ADDR_BEARERS_ONLY_ALLOWED,   APS_L4A_ERR_SM_NW_SINGLE_ADDR_BERERS_ONLY_ALLOWED      },
    { TAF_PS_CAUSE_SM_NW_INFORMATION_NOT_RECEIVED,           APS_L4A_ERR_SM_NW_ESM_INFO_NOT_RECEIVED                },
    { TAF_PS_CAUSE_SM_NW_PDN_CONNECTION_DOES_NOT_EXIST,      APS_L4A_ERR_SM_NW_PDN_CONN_NOT_EXIST                   },
    { TAF_PS_CAUSE_SM_NW_SAME_APN_MULTI_PDN_CONNECTION_NOT_ALLOWED, APS_L4A_ERR_SM_NW_MULTI_PDN_CONN_FOR_ONE_APN_NOT_ALLOWED    },
    { TAF_PS_CAUSE_SM_NW_COLLISION_WITH_NW_INITIATED_REQUEST,APS_L4A_ERR_SM_NW_COLLISION_WITH_NW_INTIAL_REQEST      },
    { TAF_PS_CAUSE_SM_NW_UNSUPPORTED_QCI_VALUE,              APS_L4A_ERR_SM_NW_UNSUPPORTED_QCI_VALUE                },
    { TAF_PS_CAUSE_SM_NW_INVALID_TI,                         APS_L4A_ERR_SM_NW_INVALID_PTI_VALUE                    },
    { TAF_PS_CAUSE_SM_NW_SEMANTICALLY_INCORRECT_MESSAGE,     APS_L4A_ERR_SM_NW_SYNCTACTIC_INCORRECT_MSG             },
    { TAF_PS_CAUSE_SM_NW_INVALID_MANDATORY_INFO,             APS_L4A_ERR_SM_NW_INVALID_MANDATORY_INFOR              },
    { TAF_PS_CAUSE_SM_NW_MSG_TYPE_NON_EXISTENT,              APS_L4A_ERR_SM_NW_MSG_TYPE_NON_EXIST                   },
    { TAF_PS_CAUSE_SM_NW_MSG_TYPE_NOT_COMPATIBLE,            APS_L4A_ERR_SM_NW_MSG_TYPE_NOT_COMPATIBLE_WITH_PROT    },
    { TAF_PS_CAUSE_SM_NW_IE_NON_EXISTENT,                    APS_L4A_ERR_SM_NW_INFOR_ELEM_NON_EXIST                 },
    { TAF_PS_CAUSE_SM_NW_CONDITIONAL_IE_ERR,                 APS_L4A_ERR_SM_NW_CONDITIONAL_IE_ERROR                 },
    { TAF_PS_CAUSE_SM_NW_MSG_NOT_COMPATIBLE,                 APS_L4A_ERR_SM_NW_MSG_NOT_COMPATIBLE_WITH_PROT         },
    { TAF_PS_CAUSE_SM_NW_PROTOCOL_ERR_UNSPECIFIED,           APS_L4A_ERR_SM_NW_PROT_ERR_UNSPECIFIED                 },
    { TAF_PS_CAUSE_SM_NW_APN_RESTRICTION_INCOMPATIBLE,       APS_L4A_ERR_SM_NW_APN_RESTRICTION_INCOMPATIBLE_WITH_ACT_EPS_BEARER }
};
#endif




/******************************************************************************
   5 ����ʵ��
******************************************************************************/

VOS_UINT32   TAF_APS_AllocPdpId(
    TAF_CTRL_STRU                      *pstCtrl,
    VOS_UINT8                           ucCid,
    VOS_UINT8                          *pucPdpId
)
{
    VOS_UINT8                           ucAllocPdpId;
    VOS_UINT8                           i;

    ucAllocPdpId = TAF_APS_MAX_PDPID;

    /* ��0~6�ķ�Χ�ڲ���һ�����е�PDP ID */
    for (i = 0; i < TAF_APS_MAX_PDPID; i++)
    {
        /* PDP ID ��Χ��0-6 */
        if (VOS_FALSE == g_PdpEntity[i].ulUsedFlg)
        {
            /* �п���TI */
            ucAllocPdpId = i;
            break;
        }
    }

    if (ucAllocPdpId < TAF_APS_MAX_PDPID)
    {
        /* PDP ID ����ɹ� */
        g_PdpEntity[ucAllocPdpId].stClientInfo.aulModuleId[ucCid] = pstCtrl->ulModuleId;
        g_PdpEntity[ucAllocPdpId].stClientInfo.ausClientId[ucCid] = pstCtrl->usClientId;
        g_PdpEntity[ucAllocPdpId].stClientInfo.aucId[ucCid]       = pstCtrl->ucOpId;
        g_PdpEntity[ucAllocPdpId].stClientInfo.ucCid              = ucCid;
        g_PdpEntity[ucAllocPdpId].ulUsedFlg                       = VOS_TRUE;
        g_PdpEntity[ucAllocPdpId].ucPdpId                         = ucAllocPdpId;

        TAF_APS_SetPdpEntBitCidMask(ucAllocPdpId, ucCid);

        /* �������g_TafPsTiTab[ucAllocPdpId].ucUsed�� */
        *pucPdpId = ucAllocPdpId;

        return TAF_APS_ALLOC_PDPID_SUCCESS;
    }
    else
    {
        /* û�п���TI */
        return TAF_APS_ALLOC_PDPID_FAIL;
    }
}


VOS_UINT32   TAF_APS_IsPdpIdValid(
    VOS_UINT8                           ucPdpId
)
{
    return g_PdpEntity[ ucPdpId].ulUsedFlg;
}


VOS_UINT8 TAF_APS_GetCidValue(
    VOS_UINT32                          ulBearerActivateFlg,
    VOS_UINT8                          *pucCidList
)
{
    VOS_UINT8                           i;
    VOS_UINT32                          ulMaxCid;

    if (VOS_TRUE == ulBearerActivateFlg)
    {
#if (FEATURE_ON == FEATURE_LTE)
        ulMaxCid = APS_PDP_MAX_CID;
#else
        ulMaxCid = TAF_MAX_CID;
#endif
    }
    else
    {
        ulMaxCid = TAF_MAX_CID;
    }

    for (i = 1; i <= ulMaxCid; i++)
    {
        if ( APS_OPTION_THIS_CID == pucCidList[i])
        {
            return i;
        }
    }

    return 0;
}


VOS_UINT32 TAF_APS_CheckCidDefinedCtx(
    VOS_UINT8                           ucCid
)
{

    if (APS_USED == g_TafCidTab[ucCid].ucUsed)
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 TAF_APS_CheckBearerPdpTypeSuitable(
    VOS_UINT8                           ucDefaultPdpType,
    TAF_PDP_TYPE_ENUM_UINT8             enUserPdpType
)
{
    if (MN_APS_ADDR_IPV4V6 == ucDefaultPdpType)
    {
        return VOS_TRUE;
    }

    if (( (APS_ADDR_STATIC_IPV4 == ucDefaultPdpType)
       || (APS_ADDR_DYNAMIC_IPV4 == ucDefaultPdpType))
      && (TAF_PDP_IPV4 == enUserPdpType))
    {
        return VOS_TRUE;
    }

    if ((MN_APS_ADDR_IPV6 == ucDefaultPdpType)
      && (TAF_PDP_IPV6 == enUserPdpType))
    {
        return VOS_TRUE;
    }

    if (TAF_PDP_IPV4V6 == enUserPdpType)
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT8   TAF_APS_GetPdpIdByNsapi(
    VOS_UINT8                           ucNsapi
)
{
    VOS_UINT8                           ucTmpPdpId;

    for(ucTmpPdpId = 0; ucTmpPdpId < TAF_APS_MAX_PDPID; ucTmpPdpId++ )
    {
        if (( ucNsapi == g_PdpEntity[ucTmpPdpId].ucNsapi)
         && ( VOS_TRUE == g_PdpEntity[ucTmpPdpId].ulUsedFlg ))
        {
            return  ucTmpPdpId;
        }
    }
    return TAF_APS_INVALID_PDPID;

}


VOS_UINT8 TAF_APS_GetPdpIdByLlcSapi(
    VOS_UINT8                           ucLlcSapi
)
{
    VOS_UINT8                           ucTmpPdpId;

    for(ucTmpPdpId = 0; ucTmpPdpId < TAF_APS_MAX_PDPID; ucTmpPdpId++)
    {
        if ( (VOS_TRUE == g_PdpEntity[ucTmpPdpId].ulUsedFlg)
          && (VOS_TRUE == g_PdpEntity[ucTmpPdpId].PdpGprsParaFlag)
          && (VOS_TRUE == g_PdpEntity[ucTmpPdpId].GprsPara.Op_Sapi)
          && (ucLlcSapi == g_PdpEntity[ucTmpPdpId].GprsPara.ucSapi) )
        {
            return  ucTmpPdpId;
        }
    }

    return TAF_APS_INVALID_PDPID;
}


VOS_UINT32 TAF_APS_String2Hex(
    VOS_UINT8                          *pucStr,
    VOS_UINT16                          usStrLen,
    VOS_UINT32                         *pulResult
)
{
    VOS_UINT32                          ulCurrChar      = 0;                    /* current Char */
    VOS_UINT32                          ulCurrTotal     = 0;                    /* current total */
    VOS_UINT8                           ucCurrLen       = 0;                    /* current Length */

    ulCurrChar                          = (VOS_UINT32)(*(pucStr++));

    while (ucCurrLen++ < usStrLen)
    {
        if ( (ulCurrChar  >= '0') && (ulCurrChar  <= '9') )
        {
            ulCurrChar  = ulCurrChar  - '0';
        }
        else if ( (ulCurrChar  >= 'a') && (ulCurrChar  <= 'f') )
        {
            ulCurrChar  = ulCurrChar  - 'a' + 10;
        }
        else if ( (ulCurrChar  >= 'A') && (ulCurrChar  <= 'F') )
        {
            ulCurrChar  = ulCurrChar  - 'A' + 10;
        }
        else
        {
            return VOS_ERR;
        }

        if (ulCurrTotal > 0x0FFFFFFF)
        {
            /* ������ת, ����ʧ�� */
            return VOS_ERR;
        }
        else
        {
            ulCurrTotal = (ulCurrTotal << 4) + ulCurrChar;                      /* accumulate digit */
            ulCurrChar  = (VOS_UINT32)(*(pucStr++));                            /* get next Char */
        }
    }

    *pulResult          = ulCurrTotal;                                          /* return result, negated if necessary */

    return VOS_OK;
}
VOS_UINT8   TAF_APS_GetPdpIdByCid(
    VOS_UINT8                           ucCid
)
{
    VOS_UINT8                           i;

    if ( ucCid > TAF_MAX_CID )
    {
        return  TAF_APS_INVALID_PDPID;
    }

    for(i = 0; i < TAF_APS_MAX_PDPID; i++)
    {
        if ((VOS_TRUE == TAF_APS_IsPdpEntBitCidMaskExit(i, ucCid))
         && (VOS_TRUE == g_PdpEntity[i].ulUsedFlg))
        {
            /*�ҵ�����ͬ��CID,���Ҹ���������ʹ�õ�,��Ϊ�ҵ�����ȷ����*/
            return i;
        }
    }
    return TAF_APS_INVALID_PDPID;
}
VOS_UINT32   TAF_APS_GetPdpEntModuleId(
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid
)
{
    return g_PdpEntity[ucPdpId].stClientInfo.aulModuleId[ucCid];
}


VOS_UINT16   TAF_APS_GetPdpEntClientId(
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid
)
{
    return g_PdpEntity[ucPdpId].stClientInfo.ausClientId[ucCid];
}


VOS_UINT8   TAF_APS_GetPdpEntOpId(
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid
)
{
    return g_PdpEntity[ucPdpId].stClientInfo.aucId[ucCid];
}


VOS_VOID   TAF_APS_SetPdpEntClientInfo(
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid,
    VOS_UINT32                          ulModuleId,
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId
)
{
    PS_MEM_SET(&g_PdpEntity[ucPdpId].stClientInfo, 0, sizeof(TAF_APS_CLIENT_INFO_STRU));

    TAF_APS_SetPdpEntCidValue(ucPdpId, ucCid);
    TAF_APS_SetPdpEntModuleId(ucPdpId, ucCid, ulModuleId);
    TAF_APS_SetPdpEntClientId(ucPdpId, ucCid, usClientId);
    TAF_APS_SetPdpEntOpId(ucPdpId, ucCid, ucOpId);

    return;
}


VOS_VOID   TAF_APS_AddPdpEntClientInfo(
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid,
    CONST TAF_CTRL_STRU                *pstCtrl
)
{
    TAF_APS_SetPdpEntBitCidMask(ucPdpId, ucCid);

    TAF_APS_SetPdpEntModuleId(ucPdpId, ucCid, pstCtrl->ulModuleId);
    TAF_APS_SetPdpEntClientId(ucPdpId, ucCid, pstCtrl->usClientId);
    TAF_APS_SetPdpEntOpId(ucPdpId, ucCid, pstCtrl->ucOpId);

    return;
}


VOS_VOID TAF_APS_RmvPdpEntClientInfo (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid,
    TAF_APS_BITCID_INFO_STRU           *pstBitCid
)
{
    VOS_UINT8                           i;

    TAF_APS_RmvPdpEntBitCidMask(ucPdpId, ucCid);
    TAF_APS_SetPdpEntModuleId(ucPdpId, ucCid, TAF_APS_INVALID_MODULEID);
    TAF_APS_SetPdpEntClientId(ucPdpId, ucCid, TAF_APS_INVALID_CLIENTID);
    TAF_APS_SetPdpEntOpId(ucPdpId, ucCid, TAF_APS_INVALID_OPID);

    /* ���ȥ�����CIDΪ�����ó���ʱʹ�õģ���Ҫ֪ͨESM�µ�cid */
    if (ucCid == TAF_APS_GetPdpEntCurrCid(ucPdpId))
    {
        for (i = 0; i < pstBitCid->ulNum; i++)
        {
            if (ucCid != pstBitCid->aucCid[i])
            {
                break;
            }
        }

        TAF_APS_SetPdpEntCidValue(ucPdpId, pstBitCid->aucCid[i]);

#if (FEATURE_ON == FEATURE_LTE)
        /* ֪ͨESM��ǰ��Cidˢ�� */
        MN_APS_SndEsmPdpInfoInd(TAF_APS_GetPdpEntInfoAddr(ucPdpId),
                                SM_ESM_PDP_OPT_MODIFY);
#endif
    }
}


VOS_UINT8  TAF_APS_GetPdpIdByDefaultCid (VOS_VOID)
{
    VOS_UINT8                           i;

    /*TI��Ч��Χ��0-6,8-14*/
    for (i = 0; i < TAF_APS_MAX_PDPID; i++)
    {
        /* �����ǰ��CID��ͬ�� */
        if ( ( VOS_TRUE == g_PdpEntity[i].ulUsedFlg )
          && ( TAF_APS_DEFAULT_CID == g_PdpEntity[i].stClientInfo.ucCid ))
        {
            return i;
        }

    }

    TAF_NORMAL_LOG(WUEPS_PID_TAF,"TAF_APS_GetPdpIdByDefaultCid: Can not find PDP ID for CID");

    /* δ�ҵ�ƥ��ģ�������Ч��PDP ID */
    return TAF_APS_INVALID_PDPID;

}
VOS_VOID TAF_APS_SetPdpEntCidValue (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid
)
{
    g_PdpEntity[ucPdpId].stClientInfo.ucCid = ucCid;
    TAF_APS_SetPdpEntBitCidMask(ucPdpId, ucCid);
}


VOS_VOID TAF_APS_SetPdpEntModuleId (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid,
    VOS_UINT32                          ulModuleId
)
{
    if (ucCid <= TAF_MAX_CID)
    {
        g_PdpEntity[ucPdpId].stClientInfo.aulModuleId[ucCid] = ulModuleId;
    }
}

VOS_VOID TAF_APS_SetPdpEntClientId (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid,
    VOS_UINT16                          usClientId
)
{
    g_PdpEntity[ucPdpId].stClientInfo.ausClientId[ucCid] = usClientId;
}


VOS_VOID TAF_APS_SetPdpEntOpId (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid,
    VOS_UINT8                           ucOpId
)
{
    if (ucCid <= TAF_MAX_CID)
    {
        g_PdpEntity[ucPdpId].stClientInfo.aucId[ucCid] = ucOpId;
    }
}


VOS_VOID TAF_APS_SetPdpEntBitCidMask (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid
)
{
    VOS_UINT32                          ulCid;

    ulCid = 0x00000001 << ucCid;
    g_PdpEntity[ucPdpId].stClientInfo.ulBitCidMask |= ulCid;
}



VOS_VOID TAF_APS_SetPdpEntModDialInfo (
    VOS_UINT8                           ucPdpId,
    TAF_APS_PDP_MOD_DIAL_STRU          *pstModDial
)
{
    g_PdpEntity[ucPdpId].stModDial = *pstModDial;
}


VOS_VOID TAF_APS_GetPdpEntModDialInfo (
    VOS_UINT8                           ucPdpId,
    TAF_APS_PDP_MOD_DIAL_STRU          *pstModDial
)
{
     *pstModDial = g_PdpEntity[ucPdpId].stModDial;
}


VOS_VOID TAF_APS_RmvPdpEntBitCidMask (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid
)
{
    VOS_UINT32                          ulCid;

    ulCid = (0x00000001 << ucCid);
    g_PdpEntity[ucPdpId].stClientInfo.ulBitCidMask &= (~ulCid);
}


VOS_VOID TAF_APS_SetPdpEntDialPdpType(
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    g_PdpEntity[ucPdpId].stClientInfo.aenPdpType[ucCid] = enPdpType;
}


VOS_UINT8 TAF_APS_GetPdpEntDialPdpType(
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid
)
{
    return g_PdpEntity[ucPdpId].stClientInfo.aenPdpType[ucCid];
}

#if (FEATURE_ON == FEATURE_IMS)

VOS_UINT32 TAF_APS_CheckImsBearerByCid (
    VOS_UINT8                           ucCid
)
{
    VOS_UINT32                          ulCid;
    VOS_UINT8                           ucPdpId;

    for (ucPdpId = 0; ucPdpId < TAF_APS_MAX_PDPID; ucPdpId++)
    {
        if (TAF_APS_STA_ACTIVE == TAF_APS_GetPdpIdMainFsmState(ucPdpId))
        {
            ulCid = 0x00000001 << ucCid;
            if ((0 != (g_PdpEntity[ucPdpId].stClientInfo.ulBitCidMask & ulCid))
             && (PS_PID_IMSA == g_PdpEntity[ucPdpId].stClientInfo.aulModuleId[ucCid]))
            {
                return VOS_TRUE;
            }
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 TAF_APS_CheckImsBearerByRabId (
    VOS_UINT32                          ulRabId
)
{
    VOS_UINT32                          ulCid;
    VOS_UINT8                           ucBitCidIndex;
    VOS_UINT8                           ucPdpId;

    for (ucPdpId = 0; ucPdpId < TAF_APS_MAX_PDPID; ucPdpId++)
    {
        if (TAF_APS_STA_ACTIVE == TAF_APS_GetPdpIdMainFsmState(ucPdpId))
        {
            if (ulRabId != g_PdpEntity[ucPdpId].ucNsapi)
            {
                continue;
            }

            /* Ŀǰֻ֧�ֹ���IMSA�����IMS���ص�ר�г��� */
            for (ucBitCidIndex = 0; ucBitCidIndex <= TAF_MAX_CID; ucBitCidIndex++)
            {
                ulCid = 0x00000001 << ucBitCidIndex;
                if ((0 != (g_PdpEntity[ucPdpId].stClientInfo.ulBitCidMask & ulCid))
                 && (PS_PID_IMSA == g_PdpEntity[ucPdpId].stClientInfo.aulModuleId[ucBitCidIndex]))
                {
                    return VOS_TRUE;
                }
            }
        }
    }

    return VOS_FALSE;
}
VOS_VOID TAF_APS_GetImsBearerCid (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                          *pucCid
)
{
    VOS_UINT32                          ulCid;
    VOS_UINT8                           ucBitCidIndex;

    for (ucBitCidIndex = 0; ucBitCidIndex <= TAF_MAX_CID; ucBitCidIndex++)
    {
        ulCid = 0x00000001 << ucBitCidIndex;
        if ((PS_PID_IMSA == g_PdpEntity[ucPdpId].stClientInfo.aulModuleId[ucBitCidIndex])
         && (0 != (g_PdpEntity[ucPdpId].stClientInfo.ulBitCidMask & ulCid)))
        {
            *pucCid = ucBitCidIndex;
            return;
        }
    }
    return;
}

#endif


VOS_UINT32 TAF_APS_IsPdpEntBitCidMaskExit (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid
)
{
    VOS_UINT32                          ulCid;

    ulCid = 0x00000001 << ucCid;
    if (0 != (g_PdpEntity[ucPdpId].stClientInfo.ulBitCidMask & ulCid))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}

VOS_VOID TAF_APS_GetPdpEntBitCidInfo(
    VOS_UINT8                           ucPdpId,
    TAF_APS_BITCID_INFO_STRU           *pstCid
)
{
    VOS_UINT8                           ucBitCidIndex;

    pstCid->ulNum = 0;

    if (VOS_TRUE == g_PdpEntity[ucPdpId].ulUsedFlg)
    {
        /* ������32λ�����ݳ��ȣ���˲���32λ�е�ÿһ��bit */
        for (ucBitCidIndex = 0; ucBitCidIndex <= TAF_MAX_CID; ucBitCidIndex++)
        {
            /* �鿴��ǰ�ĸ�һ��BIT������ֵ�ģ�����Ϊ����ǵ�ǰ��CID���� */
            if (0 != (g_PdpEntity[ucPdpId].stClientInfo.ulBitCidMask & (0x00000001 << ucBitCidIndex)))
            {
                /* BIT1����CID 0��BIT32����CID 31 */
                pstCid->aucCid[pstCid->ulNum] = ucBitCidIndex;
                pstCid->ulNum ++;
            }
        }
    }

}
VOS_VOID TAF_APS_SetPdpEntAttachBearerFlag (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucFlag
)
{
    g_PdpEntity[ucPdpId].ucDefaultBearerFlag = ucFlag;
}


VOS_UINT8 TAF_APS_GetPdpEntAttachBearerFlag (
    VOS_UINT8                           ucPdpId
)
{
    return g_PdpEntity[ucPdpId].ucDefaultBearerFlag;
}


VOS_UINT32  TAF_APS_GetTiFromPdpId(
    VOS_UINT8                           ucPdpId,
    SM_TAF_TRANS_ID_STRU               *pstTi
)
{
    VOS_UINT8                           i;

    for(i = 0; i < TAF_APS_MAX_PDPID; i++)
    {
        if ( (ucPdpId == g_PdpEntity[i].ucPdpId)
            &&  (VOS_TRUE == g_PdpEntity[i].ulUsedFlg))
        {
            /*�ҵ�����ͬ��CID,���Ҹ���������ʹ�õ�,��Ϊ�ҵ�����ȷ����*/
            *pstTi = g_PdpEntity[i].stTransId;
            return VOS_OK;
        }

    }
    return VOS_ERR;
}
VOS_UINT32  TAF_APS_GetPdpIdFromTi(
    SM_TAF_TRANS_ID_STRU               *pstTi,
    VOS_UINT8                          *pucPdpId
)
{
    VOS_UINT8                           i;


    for(i = 0; i < TAF_APS_MAX_PDPID; i++)
    {
        if ( (VOS_TRUE == g_PdpEntity[i].ulUsedFlg)
          && (pstTi->ucTiFlag== g_PdpEntity[i].stTransId.ucTiFlag)
          && (pstTi->ucTiValue== g_PdpEntity[i].stTransId.ucTiValue))

        {
            /*�ҵ�����ͬ��CID,���Ҹ���������ʹ�õ�,��Ϊ�ҵ�����ȷ����*/
            *pucPdpId = g_PdpEntity[i].ucPdpId;
            return VOS_OK;
        }
    }

    return VOS_ERR;
}
VOS_VOID  TAF_APS_SetPdpEntityTi(
    VOS_UINT8                           ucPdpId,
    SM_TAF_TRANS_ID_STRU               *pstTi
)
{
    g_PdpEntity[ucPdpId].bitOpTransId = VOS_TRUE;

    g_PdpEntity[ucPdpId].stTransId    = *pstTi;
}
VOS_UINT32 TAF_APS_GetCidFromLteBitCid(
    VOS_UINT32                          ulBitCid
)
{
    VOS_UINT8                           ucCid;
    VOS_UINT8                           ucBitCidIndex;

    /*
        ���ݵ�ǰ��L4A�Ľӿڣ�ת����APS�ڲ������Cid��������32λ�����ݳ��ȣ�
        ��˲���32λ�е�ÿһ��bit
    */
    ucCid = TAF_INVALID_CID;
    for (ucBitCidIndex = 0; ucBitCidIndex < TAF_APS_32BIT_DATA_LEN; ucBitCidIndex++)
    {
        /* �鿴��ǰ�ĸ�һ��BIT������ֵ�ģ�����Ϊ����ǵ�ǰ��CID���� */
        if (ulBitCid & (0x00000001 << ucBitCidIndex))
        {
            /* BIT1����CID 0��BIT32����CID 31 */
            ucCid = ucBitCidIndex;
            break;
        }
    }

    return ucCid;
 }


VOS_UINT32  TAF_APS_GetActivedCid(VOS_UINT8 * pucActivedCid)
{
    VOS_UINT8                           i;
    TAF_APS_STA_ENUM_UINT32             enState;
    VOS_UINT8                           j;
    TAF_APS_BITCID_INFO_STRU                     stCid;

    if (VOS_NULL_PTR == pucActivedCid)
    {
        return VOS_ERR;
    }

    PS_MEM_SET(pucActivedCid, 0x00, TAF_MAX_CID + 1);

    /* PDP ID��Ч��Χ��0-6 */
    for (i = 0; i < TAF_APS_MAX_PDPID; i++)
    {
        PS_MEM_SET(&stCid, 0x00, sizeof(TAF_APS_BITCID_INFO_STRU));
        /* �����ǰ��CID����Чֵ�������� */
        TAF_APS_GetPdpEntBitCidInfo(i, &stCid);
        if (0 == stCid.ulNum)
        {
            continue;
        }

        /* �鿴��ǰʹ�õ�PDP ID����Ӧʵ���״̬������ڼ��modify�����У�
        ȥ��������У���ʱ��Ϊʵ���Ǽ���� */
        enState = TAF_APS_GetPdpIdMainFsmState(i);
        if ( (TAF_APS_STA_ACTIVE == enState)
          || (TAF_APS_STA_MS_MODIFYING == enState)
          || (TAF_APS_STA_MS_DEACTIVATING == enState) )
        {
            for (j = 0; j < stCid.ulNum; j++)
            {
                pucActivedCid[stCid.aucCid[j]] = VOS_TRUE;
            }
        }
    }

    return VOS_OK;
}



TAF_APS_USER_CONN_STATUS_ENUM_UINT8 TAF_APS_GetUserConnStatus(VOS_VOID)
{
    VOS_UINT8                           i;
    TAF_APS_STA_ENUM_UINT32             enState;


    for (i = 0; i < TAF_APS_MAX_PDPID; i++)
    {
        if (VOS_TRUE != TAF_APS_IsPdpIdValid(i))
        {
            continue;
        }

        /* �鿴��ǰʹ�õ�PDP ID����Ӧʵ���״̬������ڼ��modify�����У�
        ȥ��������У���ʱ��Ϊʵ���Ǽ���� */
        enState = TAF_APS_GetPdpIdMainFsmState(i);
        if ((TAF_APS_STA_ACTIVE == enState)
          || (TAF_APS_STA_MS_MODIFYING == enState)
          || (TAF_APS_STA_MS_DEACTIVATING == enState))
        {
            /* �������Ĭ�ϳ��ؽ��������ӣ��Լ�����PDP���������Ӷ�Ϊ����PDP���� */
            if ((TAF_APS_DEFAULT_CID != TAF_APS_GetPdpEntCurrCid(i))
              && (APS_PDP_ACT_PRI == TAF_APS_GetPdpEntActType(i)))
            {
                return TAF_APS_USER_CONN_EXIST;
            }
        }

    }

    return TAF_APS_USER_CONN_NOT_EXIST;
}
VOS_UINT32 TAF_APS_Ipv4AddrAtoi(
    VOS_CHAR                           *pcString,
    VOS_UINT8                          *pucNumber
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulStrLen;
    VOS_UINT32                          ulNumLen;
    VOS_UINT32                          ulDotNum;
    VOS_UINT32                          ulValTmp;
    VOS_UINT8                           aucAddr[TAF_IPV4_ADDR_LEN];

    ulStrLen = 0;
    ulNumLen = 0;
    ulDotNum = 0;
    ulValTmp = 0;
    PS_MEM_SET(aucAddr, 0x00 ,TAF_IPV4_ADDR_LEN);

    if ((VOS_NULL_PTR == pcString) || (VOS_NULL_PTR == pucNumber))
    {
        return VOS_ERR;
    }

    ulStrLen = VOS_StrLen(pcString);

    if (ulStrLen > VOS_StrLen("255.255.255.255"))
    {
        return VOS_ERR;
    }

    for (i = 0; i < ulStrLen; i++)
    {
        if ((pcString[i] >= '0') && (pcString[i] <= '9'))
        {
            ulValTmp = (ulValTmp * 10) + (pcString[i] - '0');

            ulNumLen++;
            continue;
        }
        else if (pcString[i] == '.')
        {
            if ( (0 == ulNumLen) || (ulNumLen > 3) )
            {
                return VOS_ERR;
            }

            if ( ulValTmp > 255 )
            {
                return VOS_ERR;
            }

            aucAddr[ulDotNum] = (VOS_UINT8)ulValTmp;

            ulValTmp = 0;
            ulNumLen = 0;

            /* ͳ��'.'�ĸ��� */
            ulDotNum++;

            continue;
        }
        else
        {
            /* ����ֱֵ�ӷ���ʧ�� */
            return VOS_ERR;
        }
    }

    /* �������3��'.'�򷵻�ʧ�� */
    if (3 != ulDotNum)
    {
        return VOS_ERR;
    }

    aucAddr[ulDotNum] = (VOS_UINT8)ulValTmp;

    PS_MEM_CPY(pucNumber, aucAddr, TAF_IPV4_ADDR_LEN);

    return VOS_OK;
}
VOS_UINT32 TAF_APS_ValidatePdpType(
    TAF_PDP_TYPE_ENUM_UINT8             enPdpTypeReq,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpTypeCnf
)
{
    if ((TAF_PDP_IPV4V6 == enPdpTypeReq)
     && ((TAF_PDP_IPV4V6 == enPdpTypeCnf)
      || (TAF_PDP_IPV4 == enPdpTypeCnf)
      || (TAF_PDP_IPV6 == enPdpTypeCnf)))
    {
        /* �������IPV4V6����,�ظ������Ϳ���ΪIPV4V6��IPV4��IPV6 */
        return VOS_OK;
    }
    else if((enPdpTypeReq == enPdpTypeCnf )
         && (TAF_PDP_TYPE_BUTT != enPdpTypeCnf))
    {
        /* �����PDP������ظ���PDP����һ�£����һظ������Ͳ���TAF_PDP_TYPE_BUTT */
        return VOS_OK;
    }
    else
    {
    }

    return VOS_ERR;
}


VOS_UINT32 TAF_APS_Ipv4AddrItoa(
    VOS_CHAR                           *pcString,
    VOS_UINT8                          *pucNumber
)
{
    VOS_CHAR                            astringTmp[8];

    PS_MEM_SET(astringTmp, 0x00, 8);

    if ((VOS_NULL_PTR == pcString) || (VOS_NULL_PTR == pucNumber))
    {
        return VOS_ERR;
    }
    else
    {
        PS_MEM_SET(pcString, 0, TAF_MAX_IPV4_ADDR_STR_LEN);
        sprintf(astringTmp, "%d", pucNumber[0]);
        strcat(pcString, astringTmp);

        sprintf(astringTmp, ".%d", pucNumber[1]);
        strcat(pcString, astringTmp);

        sprintf(astringTmp, ".%d", pucNumber[2]);
        strcat(pcString, astringTmp);

        sprintf(astringTmp, ".%d", pucNumber[3]);
        strcat(pcString, astringTmp);
    }

    return VOS_OK;
}



VOS_UINT8 TAF_APS_ConverUserDefPdpTypeToApsEntityPdpType (
    TAF_PDP_TYPE_ENUM_UINT8             enUserPdpType,
    VOS_UINT8                           ucDefaultPdpType
)
{
    if (TAF_PDP_IPV4V6 == enUserPdpType)
    {
        return ucDefaultPdpType;
    }

    if (TAF_PDP_IPV4 == enUserPdpType)
    {
        return APS_ADDR_STATIC_IPV4;
    }

    if (TAF_PDP_IPV6 == enUserPdpType)
    {
        return MN_APS_ADDR_IPV6;
    }

    return  APS_ADDR_STATIC_IPV4;
}


VOS_VOID TAF_APS_GetSpecPdpLinkedPdpInfo (
    TAF_APS_PDPID_LIST_STRU            *pstPdpIdList,
    VOS_UINT8                           ucNsapi
)
{
    VOS_UINT8                           i;

    for (i = 0; i < TAF_APS_MAX_PDPID; i++)
    {
        if (VOS_TRUE == g_PdpEntity[i].PdpNsapiFlag)
        {
            if (ucNsapi == g_PdpEntity[i].ucLinkedNsapi)
            {
                pstPdpIdList->aucPdpId[pstPdpIdList->ucPdpNum] = i;
                pstPdpIdList->ucPdpNum++;
            }
        }
    }
}


TAF_PS_CAUSE_ENUM_UINT32 TAF_APS_MapSmCause(
    SM_TAF_CAUSE_ENUM_UINT16            enSmCause
)
{
    TAF_APS_SM_ERR_CODE_MAP_STRU       *pstSmErrMapTblPtr = VOS_NULL_PTR;
    VOS_UINT32                          ulSmErrMapTblSize;
    VOS_UINT32                          ulCnt;

    pstSmErrMapTblPtr = TAF_APS_GET_SM_ERR_CODE_MAP_TBL_PTR();
    ulSmErrMapTblSize = TAF_APS_GET_SM_ERR_CODE_MAP_TBL_SIZE();

    for (ulCnt = 0; ulCnt < ulSmErrMapTblSize; ulCnt++)
    {
        if (pstSmErrMapTblPtr[ulCnt].enSmCause == enSmCause)
        {
            return pstSmErrMapTblPtr[ulCnt].enPsCuase;
        }
    }

    return TAF_PS_CAUSE_UNKNOWN;
}

#if (FEATURE_ON == FEATURE_LTE)

TAF_PS_CAUSE_ENUM_UINT32 TAF_APS_MapL4aCause(
    VOS_UINT32                          ulL4aCause
)
{
    TAF_APS_L4A_ERR_CODE_MAP_STRU      *pstL4aErrMapTblPtr = VOS_NULL_PTR;
    VOS_UINT32                          ulL4aErrMapTblSize;
    VOS_UINT32                          ulCnt;

    pstL4aErrMapTblPtr = TAF_APS_GET_L4A_ERR_CODE_MAP_TBL_PTR();
    ulL4aErrMapTblSize = TAF_APS_GET_L4A_ERR_CODE_MAP_TBL_SIZE();

    for (ulCnt = 0; ulCnt < ulL4aErrMapTblSize; ulCnt++)
    {
        if (pstL4aErrMapTblPtr[ulCnt].ulL4aCause == ulL4aCause)
        {
            return pstL4aErrMapTblPtr[ulCnt].enPsCuase;
        }
    }

    return TAF_PS_CAUSE_UNKNOWN;
}

#endif


TAF_PDP_TYPE_ENUM_UINT8 TAF_APS_ConvertPdpType(VOS_UINT8 ucPdpTypeNum)
{
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType;

    /* ת��APS�ڲ�PDP������TAF�ⲿPDP���� */
    switch (ucPdpTypeNum)
    {
        case APS_ADDR_STATIC_IPV4:
            enPdpType = TAF_PDP_IPV4;
            break;

        case APS_ADDR_PPP:
            enPdpType = TAF_PDP_PPP;
            break;

#if (FEATURE_ON == FEATURE_IPV6)
        case MN_APS_ADDR_IPV6:
            enPdpType = TAF_PDP_IPV6;
            break;

        case MN_APS_ADDR_IPV4V6:
            enPdpType = TAF_PDP_IPV4V6;
            break;
#endif

        default:
            enPdpType = TAF_PDP_IPV4;
            break;
    }

    return enPdpType;
}
VOS_UINT32 TAF_APS_GetCtrlHdrFromCmdBufferQueue(
    TAF_CTRL_STRU                      *pstCtrl,
    TAF_APS_TIMER_ID_ENUM_UINT32        enTimer,
    VOS_UINT32                          ulPara
)
{
    TAF_APS_CMD_BUFFER_STRU            *pstMsgInfo   = VOS_NULL_PTR;
    VOS_UINT32                          ulLength;

    if (VOS_NULL_PTR == pstCtrl)
    {
        return VOS_FALSE;
    }

    /* �ӻ�������л�ȡ��Ϣ */
    pstMsgInfo = TAF_APS_GetItemFromCmdBufferQueue(enTimer, ulPara);

    if (VOS_NULL_PTR == pstMsgInfo)
    {
        /* ��Ϣ��ȡʧ��, ��ӡ������Ϣ */
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_APS_GetCtrlHdrFromCmdBufferQueue: Get message info failed!");
        return VOS_FALSE;
    }

    ulLength = PS_MIN(pstMsgInfo->ulMsgInfoLen , sizeof(TAF_CTRL_STRU));
    PS_MEM_CPY((VOS_UINT8 *)pstCtrl, (VOS_UINT8 *)pstMsgInfo->pulMsgInfo, ulLength);

    return VOS_TRUE;

}


#if (FEATURE_ON == FEATURE_PTM)

VOS_VOID TAF_APS_PsCallFailErrRecord(TAF_PS_CAUSE_ENUM_UINT32 enCause)
{
    NAS_ERR_LOG_PS_CALL_FAIL_EVENT_STRU                     stPsCallFailEvent;
    TAF_APS_PDP_ENTITY_FSM_CTX_STRU                        *pstCurPdpEntityFsmCtx = VOS_NULL_PTR;
    VOS_UINT32                                              ulIsLogRecord;
    VOS_UINT32                                              ulLength;
    VOS_UINT32                                              ulResult;
    VOS_UINT16                                              usLevel;

    /* ��ѯ��ӦAlarm Id�Ƿ���Ҫ��¼�쳣��Ϣ */
    usLevel       = NAS_GetErrLogAlmLevel(NAS_ERR_LOG_ALM_PS_CALL_FAIL);
    ulIsLogRecord = TAF_SDC_IsErrLogNeedRecord(usLevel);

    /* ����Ҫ��¼��û���쳣ʱ���������쳣��Ϣ */
    if ((VOS_FALSE == ulIsLogRecord)
     || (TAF_PS_CAUSE_SUCCESS == enCause)
     || (TAF_PS_CAUSE_SM_NW_REGULAR_DEACTIVATION == enCause))
    {
        return;
    }

    ulLength = sizeof(NAS_ERR_LOG_PS_CALL_FAIL_EVENT_STRU);

    /* ��ȡ��ǰ״̬����Ϣ */
    pstCurPdpEntityFsmCtx = TAF_APS_GetCurrPdpEntityFsmAddr();

    /* ���PS����ʧ���쳣��Ϣ */
    PS_MEM_SET(&stPsCallFailEvent, 0x00, ulLength);

    NAS_COMM_BULID_ERRLOG_HEADER_INFO(&stPsCallFailEvent.stHeader,
                                      VOS_GetModemIDFromPid(WUEPS_PID_TAF),
                                      NAS_ERR_LOG_ALM_PS_CALL_FAIL,
                                      usLevel,
                                      VOS_GetSlice(),
                                      (ulLength - sizeof(OM_ERR_LOG_HEADER_STRU)));

    stPsCallFailEvent.ulPsCause                      = enCause;
    stPsCallFailEvent.ulPsRegState                   = (VOS_UINT32)TAF_SDC_GetPsServiceStatus();
    stPsCallFailEvent.stApsFsmInfo.stMainFsm.ulFsmId = pstCurPdpEntityFsmCtx->stMainFsm.enFsmId;
    stPsCallFailEvent.stApsFsmInfo.stMainFsm.ulState = pstCurPdpEntityFsmCtx->stMainFsm.ulState;
    stPsCallFailEvent.stApsFsmInfo.stSubFsm.ulFsmId  = pstCurPdpEntityFsmCtx->stSubFsm.enFsmId;
    stPsCallFailEvent.stApsFsmInfo.stSubFsm.ulState  = pstCurPdpEntityFsmCtx->stSubFsm.ulState;
    stPsCallFailEvent.stApsFsmInfo.ucPdpId           = pstCurPdpEntityFsmCtx->ucPdpId;

    /* ��ȡ��ǰλ����Ϣ */
    NAS_MMC_OutputPositionInfo(&stPsCallFailEvent.stPositionInfo);
    /* ��ȡ��ǰUsim��Ϣ */
    NAS_MMA_OutputUsimInfo(&stPsCallFailEvent.stUsimInfo);

    /*
       ���쳣��Ϣд��Buffer��
       ʵ��д����ַ�������Ҫд��Ĳ������ӡ�쳣
     */
    ulResult = TAF_SDC_PutErrLogRingBuf((VOS_CHAR *)&stPsCallFailEvent, ulLength);
    if (ulResult != ulLength)
    {
        NAS_ERROR_LOG(WUEPS_PID_TAF, "TAF_APS_PsCallFailErrRecord(): Push buffer error.");
    }

    NAS_COM_MntnPutRingbuf(NAS_ERR_LOG_ALM_PS_CALL_FAIL,
                           WUEPS_PID_TAF,
                           (VOS_UINT8 *)&stPsCallFailEvent,
                           sizeof(stPsCallFailEvent));
    return;
}
#endif


VOS_VOID   TAF_APS_SetPsCallEvtCtrl(
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid,
    TAF_CTRL_STRU                      *pstCtrl
)
{
    pstCtrl->ulModuleId     = g_PdpEntity[ucPdpId].stClientInfo.aulModuleId[ucCid];
    pstCtrl->usClientId     = g_PdpEntity[ucPdpId].stClientInfo.ausClientId[ucCid];
    pstCtrl->ucOpId         = g_PdpEntity[ucPdpId].stClientInfo.aucId[ucCid];

}


VOS_INT32 TAF_APS_CompareApn(
    const VOS_UINT8                 aucBearerApn[],
    VOS_UINT8                       ucBearerApnLen,
    const VOS_UINT8                 aucUserApn[],
    VOS_UINT8                       ucUserApnLen
)
{
    VOS_UINT8                       i;
    VOS_UINT8                       ucChar1;
    VOS_UINT8                       ucChar2;

    /* ���APN����, ���ص�APN��Я��MNC��MCC��Ϣ */
    if (ucBearerApnLen < ucUserApnLen)
    {
        return VOS_ERROR;
    }

    /* ����ֽڱȽ��û��ͳ��ص�APN */
    for (i = 0; i < ucUserApnLen; i++)
    {
        ucChar1 = UPCASE(aucBearerApn[i]);
        ucChar2 = UPCASE(aucUserApn[i]);

        if (ucChar1 != ucChar2)
        {
            return VOS_ERROR;
        }
    }

    return VOS_OK;
}


VOS_UINT32 TAF_APS_MatchCallWithAllBearer(
    CONST TAF_CTRL_STRU                *pstCtrl,
    CONST TAF_APS_MATCH_PARAM_STRU     *pstMatchParam,
    VOS_UINT8                          *pucPdpId
)
{
#if (FEATURE_ON == FEATURE_IMS)
    VOS_UINT8                          *pucPdpEntApnValue = VOS_NULL_PTR;
    VOS_UINT8                          *pucUsrApnValue = VOS_NULL_PTR;
    VOS_UINT8                           aucApn[APS_MAX_LENGTH_OF_APN];
    VOS_UINT8                           ucPdpEntApnLen;
    VOS_UINT8                           ucUsrApnLen;
    VOS_UINT8                           ucPdpId;

    PS_MEM_SET(aucApn, 0x00, APS_MAX_LENGTH_OF_APN);

    /* �����������Ƿ�ƥ�� */
    if (VOS_TRUE != pstMatchParam->ucPrimaryPdpFlg)
    {
        return VOS_FALSE;
    }

    /* �û�APN����Я�� */
    if (0 == pstMatchParam->stApn.ucLength)
    {
        return VOS_FALSE;
    }

    /* ���Ų����е�APNת��������Э���ʽ */
    Taf_LenStr2Apn(&pstMatchParam->stApn, aucApn);

    /* ��������ʵ��, ƥ�䲦�Ų��� */
    for (ucPdpId = 0; ucPdpId < TAF_APS_MAX_PDPID; ucPdpId++)
    {
        if (VOS_TRUE != TAF_APS_IsPdpIdValid(ucPdpId))
        {
            continue;
        }

        /* �����ص�IP�����Ƿ�ƥ�� */
        if (VOS_TRUE != TAF_APS_CheckBearerPdpTypeSuitable(
                                    TAF_APS_GetPdpEntPdpAddrType(ucPdpId),
                                    pstMatchParam->enPdpType))
        {
            continue;
        }

        /* ���������ص������Ƿ�ƥ�� */
        if (TAF_APS_GetPdpEntEmcFlg(ucPdpId) != pstMatchParam->enEmergencyInd)
        {
            continue;
        }

        /* ��ȡ����Э���ʽAPN�ܳ��Ⱥ����� */
        pucUsrApnValue = &aucApn[1];
        ucUsrApnLen    = aucApn[0];

        /* �����ص�APN�Ƿ�ƥ�� */
        if (VOS_TRUE == TAF_APS_GetPdpEntApnFlag(ucPdpId))
        {
            pucPdpEntApnValue = TAF_APS_GetPdpEntApnValue(ucPdpId);
            ucPdpEntApnLen    = TAF_APS_GetPdpEntApnLen(ucPdpId);

            /* �Ƚ�APN���� */
            if (VOS_OK == TAF_APS_CompareApn(pucPdpEntApnValue,
                                             ucPdpEntApnLen,
                                             pucUsrApnValue,
                                             ucUsrApnLen))
            {
                if ( (PS_PID_IMSA != TAF_APS_GetPdpEntModuleId(ucPdpId, TAF_APS_GetPdpEntCurrCid(ucPdpId)))
                  && (PS_PID_IMSA != pstCtrl->ulModuleId) )
                {
                    return VOS_FALSE;
                }

                *pucPdpId = ucPdpId;
                return VOS_TRUE;
            }
        }
    }
#endif

    return VOS_FALSE;
}
VOS_VOID TAF_APS_SetPdpEntAddrType(
    VOS_UINT8                           ucPdpId,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;

    pstPdpEntity   = TAF_APS_GetPdpEntInfoAddr(ucPdpId);

    if (TAF_PDP_IPV4 == enPdpType)
    {
        pstPdpEntity->PdpAddrFlag          = VOS_TRUE;
        pstPdpEntity->PdpAddr.ucPdpTypeNum = APS_ADDR_STATIC_IPV4;
    }
#if (FEATURE_ON == FEATURE_IPV6)
    else if (TAF_PDP_IPV6 == enPdpType)
    {
        pstPdpEntity->PdpAddrFlag          = VOS_TRUE;
        pstPdpEntity->PdpAddr.ucPdpTypeNum = MN_APS_ADDR_IPV6;
    }
    else if (TAF_PDP_IPV4V6 == enPdpType)
    {
        pstPdpEntity->PdpAddrFlag          = VOS_TRUE;
        pstPdpEntity->PdpAddr.ucPdpTypeNum = MN_APS_ADDR_IPV4V6;
    }
#endif
    else
    {
        pstPdpEntity->PdpAddrFlag          = VOS_TRUE;
        pstPdpEntity->PdpAddr.ucPdpTypeNum = APS_ADDR_STATIC_IPV4;
    }
}

#if (FEATURE_ON == FEATURE_LTE)

VOS_UINT32 TAF_APS_IsAttachBearerExist(VOS_VOID)
{
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;
    VOS_UINT8                           ucPdpId;

    for (ucPdpId = 0; ucPdpId < TAF_APS_MAX_PDPID; ucPdpId++)
    {
        pstPdpEntity = TAF_APS_GetPdpEntInfoAddr(ucPdpId);

        if (VOS_TRUE == pstPdpEntity->ucDefaultBearerFlag)
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}
#endif


VOS_UINT32 TAF_APS_IsAnyBearerExist(VOS_VOID)
{
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;
    VOS_UINT8                           ucPdpId;

    for (ucPdpId = 0; ucPdpId < TAF_APS_MAX_PDPID; ucPdpId++)
    {
        pstPdpEntity = TAF_APS_GetPdpEntInfoAddr(ucPdpId);

        if ( (VOS_TRUE == pstPdpEntity->ulUsedFlg)
          && (APS_JUDGE_NSAPI_VALID(pstPdpEntity->ucNsapi)) )
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT8 TAF_APS_GetAttachAllowFlg(VOS_VOID)
{
    return NAS_MML_GetPsAttachAllowFlg();
}


VOS_VOID TAF_APS_GenMatchParamByCidInfo(
    CONST TAF_PDP_TABLE_STRU           *pstPdpTblInfo,
    TAF_APS_MATCH_PARAM_STRU           *pstMatchParam
)
{
    /* ȱʡ���ر�ʶ */
    pstMatchParam->ucPrimaryPdpFlg = (VOS_TRUE == pstPdpTblInfo->ucPriCidFlag)?
                                     VOS_FALSE : VOS_TRUE;

    /* IP���� */
    pstMatchParam->enPdpType       = pstPdpTblInfo->CidTab.stPdpAddr.enPdpType;

    /* �������� */
    pstMatchParam->enEmergencyInd  = pstPdpTblInfo->CidTab.enEmergencyInd;

    /* APN��Ϣ */
    PS_MEM_CPY(&pstMatchParam->stApn, &pstPdpTblInfo->CidTab.stApn, sizeof(TAF_PDP_APN_STRU));

    return;
}


VOS_VOID TAF_APS_GenMatchParamByDailInfo(
    CONST TAF_PS_DIAL_PARA_STRU        *pstDialParaInfo,
    TAF_APS_MATCH_PARAM_STRU           *pstMatchParam
)
{
    /* Primary PDP��ʶ */
    pstMatchParam->ucPrimaryPdpFlg    = VOS_TRUE;

    /* IP���� */
    pstMatchParam->enPdpType          = pstDialParaInfo->enPdpType;

    /* �������� */
    if (VOS_TRUE == pstDialParaInfo->bitOpEmergencyInd)
    {
        pstMatchParam->enEmergencyInd = pstDialParaInfo->enEmergencyInd;
    }
    else
    {
        pstMatchParam->enEmergencyInd = TAF_PDP_NOT_FOR_EMC;
    }

    /* APN��Ϣ */
    if (VOS_TRUE == pstDialParaInfo->bitOpApn)
    {
        pstMatchParam->stApn.ucLength = (VOS_UINT8)VOS_StrLen((VOS_CHAR *)pstDialParaInfo->aucApn);
        PS_MEM_CPY(pstMatchParam->stApn.aucValue, pstDialParaInfo->aucApn, TAF_MAX_APN_LEN);
    }
    else
    {
        pstMatchParam->stApn.ucLength = 0;
    }

    return;
}



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


