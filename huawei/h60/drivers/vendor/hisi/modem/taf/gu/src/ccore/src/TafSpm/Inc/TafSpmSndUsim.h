
#ifndef _TAF_SPM_SND_USIM_H_
#define _TAF_SPM_SND_USIM_H_

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "PsTypeDef.h"
#include "om.h"
#include "TafAppSsa.h"
#include "UsimPsInterface.h"

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



/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
VOS_UINT32 TAF_SPM_SendPbUssdFdnCheckReq(
    VOS_UINT16                          usClientId,
    struct MsgCB                       *pstMsg
);

VOS_UINT32  TAF_SPM_SendPbSsFdnCheckReq(
    VOS_UINT16                          usClientId,
    struct MsgCB                       *pstMsg
);

VOS_UINT32  TAF_SPM_SendUsimUssdEnvelopeReq(
    VOS_UINT16                          usClientId,
    struct MsgCB                       *pstMsg
);

VOS_UINT32  TAF_SPM_SendUsimSsEnvelopeReq(
    VOS_UINT16                          usClientId,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_SPM_TransUssdMsgToAsciiStr(
    TAF_SS_PROCESS_USS_REQ_STRU        *pstUssdMsg,
    VOS_UINT8                          *pucAsciiStr,
    VOS_UINT32                         *pulAsciiStrLength
);

VOS_UINT32  TAF_SPM_BuildUssdFdnInfo(
    struct MsgCB                       *pstMsg,
    PS_PB_FDN_NUM_STRU                 *pstFdnInfo
);

VOS_UINT32  TAF_SPM_BuildSsFdnInfo(
    struct MsgCB                       *pstMsg,
    PS_PB_FDN_NUM_STRU                 *pstFdnInfo
);

VOS_UINT32 TAF_SPM_GetUssdStringSeptetLength(
    VOS_UINT32                          ulOctetLength,
    VOS_UINT8                           ucLastByte
);

VOS_UINT32 TAF_SPM_BuildUssdStringInfo(
    TAF_SS_DATA_CODING_SCHEME           ucDatacodingScheme,
    TAF_SS_USSD_STRING_STRU            *pstSrcUssd,
    TAF_SS_USSD_STRING_STRU            *pstDstUssd
);

VOS_UINT32 TAF_SPM_SendPbCallFdnCheckReq(
    VOS_UINT16                          usClientId,
    struct MsgCB                       *pstMsg
);

VOS_UINT32  TAF_SPM_SendUsimCallEnvelopeReq(
    VOS_UINT16                          usClientId,
    struct MsgCB                       *pstMsg
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

#endif /* end of TafSpmSndUsim.h */

