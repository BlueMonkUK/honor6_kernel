/************************************************************************
  Copyright   : 2005-2007, Huawei Tech. Co., Ltd.
  File name   : TAf_ApsForRabm.h
  Author      : ��³��
  Version     : V200R001
  Date        : 2005-0
  Description : ��ͷ�ļ�������---
  History     :
  1. Date:2005-0 -
     Author: ---
     Modification:Create
  2. 2006-03-03 MODIFY BY H41410 FOR A32D02344
  3. 2006-08-09 modify by J60010247 for A32D03479
  4. 2006-03-03 modify by L47619 for A32D05709
************************************************************************/

#ifndef _TAF_APSFORRABM_H_
#define _TAF_APSFORRABM_H_


/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  #pragma pack(*)    �����ֽڶ��뷽ʽ
*****************************************************************************/
#pragma pack(4)

/*****************************************************************************
  2 �궨��
  #define
*****************************************************************************/
#define APS_1KBPS_INC_CLOSE_TOP             0x3F
#define APS_8KBPS_INC_CLOSE_TOP             0x7F
#define APS_64KBPS_INC_CLOSE_TOP            0xFE

/*---------------------------------------------------------------------------
        RABM����APS����Ϣ����
---------------------------------------------------------------------------*/
#define ID_RABM_APS_SYS_CHG_TO_GPRS_IND     0x0130

#define RABM_APS_LOCAL_PDP_DEACT_REQ_MSG    0x0160

#ifndef APS_SUCC
#define APS_SUCC                            0
#endif

#ifndef APS_FAIL
#define APS_FAIL                            1
#endif

/*******************************************************************************
  3 ö�ٶ���
  typedef enum
{
    ģ����_��������_ENUM
    MAPS_TEMPLATE_ENUM1 = 0,
    MAPS_TEMPLATE_ENUM2


    MAPS_TEMPLATE_BUTT,    �����ͳһ��BUTT����Ϊ���һ��ö��ֵ��

}MAPS_TEMPLATE_ENUM;   ģ����_ö����������_ENUM
*******************************************************************************/
typedef enum
{
    APS_2G_TO_3G_ENUM                       = 0,
    APS_3G_TO_2G_ENUM,
    APS_SYS_CHANGE_DIR_BUTT                 = 0xFF
} APS_SYS_CHANGE_DIR_ENUM;

typedef enum
{
    APS_3G_ENUM                             = 0,
    APS_2G_ENUM,
    APS_CUR_NET_BUTT                        = 0xFF
} APS_CUR_NET_ENUM;

typedef enum
{
    APS_SUCC_ENUM                           = 0,
    APS_FAIL_ENUM,
    APS_SYS_CHANGE_RESULT_BUTT              = 0xFF
} APS_SYS_CHANGE_RESULT_ENUM;


/*****************************************************************************
  4 ȫ�ֱ�������
extern VOS_UINT8* pgucMapsTemplateBuffer;   ָ�룫ȫ�ֱ�ʶ�����ͣ�ģ��������������
*****************************************************************************/




/*****************************************************************************
  6 ��Ϣ����
  typedef struct
{
    MAPS_TEMPLATE_HEADER
    VOS_UINT8    aucData[2];     ��Ϣ��

} MAPS_TEMPLATE_MSG;
*****************************************************************************/


/*****************************************************************************
  7 STRUCT����
  typedef struct
{
    VOS_UINT8    ucTemplate;
    VOS_UINT8    aucData[2];

} MAPS_TEMPLATE_ST;
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          UlTlli;
    APS_SYS_CHANGE_DIR_ENUM             SysChangeDir;
    APS_CUR_NET_ENUM                    CurNet;
    APS_SYS_CHANGE_RESULT_ENUM          SysChangeResult;
} APS_RABM_SYS_CHANGE_ST;


typedef struct
{
    /*
    aucByteRateList[i][0] == n : Rabid;
    aucByteRateList[i][1] == xx: The byte rate for uplink of Rabid n.
    aucByteRateList[i][2] == xx: The byte rate for downlink of Rabid n.
    The number XX means XX bytes per second.
    "i" belongs to [0, 10].
    "n" belongs to [5, 15].*/

    VOS_UINT8                           ucRabidNum;   /*�������˼���NSAPI*/
    VOS_UINT32                          aulByteRateList[11][3];
}APS_RABM_ALL_RATE_ST;       /*����Э�̵���������(���м����RABID)*/

typedef struct
{
    /*0 - The Rabid is not actived.
      1 - The Rabid is activated */
    VOS_UINT8                           ucFlag;

    /*The number XX means XX bytes per second.*/
    VOS_UINT32                          ulUplinkByteRate;
    VOS_UINT32                          ulDownlinkByteRate;
}APS_RABM_ONE_RATE_ST;      /*����Э�̵���������(ָ����RABID)*/

/*****************************************************************************
  8 UNION����
  typedef union
{
    VOS_UINT8    ucTemplate;
    VOS_UINT8    aucData[2];

} MAPS_TEMPLATE_UNION;
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣͷ����
  ģ���������士HEADER
#define MAPS_TEMPLATE_HEADER            VOS_UINT16    usTotalsMsgLen;\
                                        VOS_UINT32    ulSendModuleId;\
                                        VOS_UINT32    ulRecvModuleId;\
*****************************************************************************/
/*---------------------------------------------------------
            MSG used between APS and RABM.
---------------------------------------------------------*/
typedef struct{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgType;          /*��Ϣ����*/
}RABM_APS_SYS_CHANGE_MSG;

typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgType;          /*��Ϣ����*/
    VOS_UINT8                           ucNsapi;            /*NSAPI*/
    VOS_UINT8                           ucRsv;              /*�ֽڶ���*/
}RABM_APS_LOCAL_PDP_DEACT_REQ;    /*PDP����ȥ��������*/


/*****************************************************************************
  8 FUNCTIONS����
*****************************************************************************/
VOS_UINT32  Aps_GetAllNegoRate  (       APS_RABM_ALL_RATE_ST*   pAllRate );
VOS_UINT32  Aps_GetOneNegoRate  (       VOS_UINT8               ucRabId,
                                        APS_RABM_ONE_RATE_ST*   pOneRate );


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/

#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of MapsTemplate.h*/
