
#ifndef __MLIB_INTERFACE_H__
#define __MLIB_INTERFACE_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "mlib_typedef.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/

/* ֧�ֵ����ͨ���� */
#define MLIB_SOURCE_CHN_MAX_NUM         (5)

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/* ����ö�� */
enum MLIB_ERROR_ENUM
{
    MLIB_ERROR_NONE                     = 0,
    MLIB_ERROR_FAIL,
    MLIB_ERROR_NULL_FUNC,
    MLIB_ERROR_NULL_PTR,
    MLIB_ERROR_BUTT,
};
typedef MLIB_UINT32 MLIB_ERROR_ENUM_UINT32;

/* ģ��ö�� */
enum MLIB_MODULE_ENUM
{
    MLIB_MODULE_DEFAULT                 = 0,
    MLIB_MODULE_BALONG_PP,
    MLIB_MODULE_BUTT,
};
typedef MLIB_UINT32 MLIB_MODULE_ENUM_UINT32;

/* ͨ·ö�� */
enum MLIB_PATH_ENUM
{
    MLIB_PATH_CS_VOICE_CALL_MICIN       = 0,
    MLIB_PATH_CS_VOICE_CALL_SPKOUT,
    MLIB_PATH_VOIP_CALL_MICIN,
    MLIB_PATH_VOIP_CALL_SPKOUT,
    MLIB_PATH_AUDIO_PLAY,
    MLIB_PATH_AUDIO_RECORD,
    MLIB_PATH_SIRI_MICIN,
    MLIB_PATH_SIRI_SPKOUT,
    MLIB_PATH_BUTT,
};
typedef MLIB_UINT32 MLIB_PATH_ENUM_UINT32;


/* �豸ö�� */
enum MLIB_DEVICE_ENUM
{
    MLIB_DEVICE_HANDSET                 = 0,                /* �ֳ�ģʽ */
    MLIB_DEVICE_HANDFREE,                                   /* ����ģʽ */
    MLIB_DEVICE_CARFREE,                                    /* ��������ģʽ */
    MLIB_DEVICE_HEADSET,                                    /* ����ģʽ */
    MLIB_DEVICE_BLUETOOTH,                                  /* ����ģʽ */
    MLIB_DEVICE_PCVOICE,                                    /* PC-VOICEģʽ */
    MLIB_DEVICE_HEADPHONE,                                  /* ��������MIC�Ķ���ͨ�� */
    MLIB_DEVICE_BUTT,
};
typedef MLIB_UINT32 MLIB_DEVICE_ENUM_UINT32;

/* ������Դ */
enum MLIB_PARA_SOURCE_ENUM
{
    MLIB_PARA_SOURCE_NV                 = 0,                /* ��NV���ж�ȡ���� */
    MLIB_PARA_SOURCE_TUNER,                                 /* ���Թ����·� */
    MLIB_PARA_SOURCE_APP,                                   /* Ӧ�ó����·� */
    MLIB_PARA_SOURCE_BUTT,
};
typedef MLIB_UINT32 MLIB_PARA_SOURCE_ENUM_UINT32;

/*****************************************************************************
  4 STRUCT����
*****************************************************************************/

/* ����Դͨ������ */
typedef struct
{
    MLIB_UINT8                         *pucData;                                /* ���������׵�ַ(����֤�ֽڶ���) */
    MLIB_UINT32                         uwSize;                                 /* �������ݴ�С(��λByte) */
    MLIB_UINT32                         uwSampleRate;                           /* ������(8000 - 8KHz, 16000 - 16KHz, ...) */
    MLIB_UINT32                         uwResolution;                           /* ������λ��(16 - 16Bit) */
} MLIB_SOURCE_CHN_STRU;

/* ����Դ���� */
typedef struct
{
    MLIB_UINT32                         uwChnNum;                               /* ��ǰ���õ�ͨ���� */
    MLIB_SOURCE_CHN_STRU                astChn[MLIB_SOURCE_CHN_MAX_NUM];        /* ͨ���� */
} MLIB_SOURCE_STRU;

typedef struct
{
    MLIB_UINT32                         uwChnNum;                               /* ͨ���� */
    MLIB_UINT32                         uwSampleRate;                           /* ������(8000 - 8KHz, 16000 - 16KHz, ...) */
    MLIB_UINT32                         uwFrameLen;                             /* ֡��(��������) */
    MLIB_UINT32                         uwResolution;                           /* ������λ��(16 - 16Bit) */
    MLIB_DEVICE_ENUM_UINT32             enDevice;                               /* �豸ģʽ */
    MLIB_PARA_SOURCE_ENUM_UINT32        enParaSource;                           /* ������Դ */
    MLIB_UINT32                         uwDataSize;                             /* ʣ�����ݳ��� */
    MLIB_UINT8                          aucData[4];                             /* ʣ��������ʼ��ַ */
} MLIB_PARA_STRU;

/*****************************************************************************
  5 ����ָ��
*****************************************************************************/

/* ģ���ʼ�� */
typedef MLIB_ERROR_ENUM_UINT32 (*MLIB_ModuleInit)(MLIB_VOID);

/* ģ��������� */
typedef MLIB_ERROR_ENUM_UINT32 (*MLIB_ModuleSet)(           \
                    MLIB_PARA_STRU         *pstPara,        \
                    MLIB_UINT32             uwSize);

/* ģ�������ȡ */
typedef MLIB_ERROR_ENUM_UINT32 (*MLIB_ModuleGet)(           \
                    MLIB_VOID              *pvData,         \
                    MLIB_UINT32             uwSize);

/* ģ�����ݴ��� */
typedef MLIB_ERROR_ENUM_UINT32 (*MLIB_ModuleProc)(          \
                    MLIB_SOURCE_STRU       *pstSourceOut,   \
                    MLIB_SOURCE_STRU       *pstSourceIn );

/* ģ����� */
typedef MLIB_ERROR_ENUM_UINT32 (*MLIB_ModuleClear)(MLIB_VOID);

/* ģ��ע����Ϣ */
typedef struct
{
    MLIB_MODULE_ENUM_UINT32             enID;                                   /* ģ��ID */
    MLIB_ModuleInit                     pfInit;                                 /* ģ���ʼ�� */
    MLIB_ModuleSet                      pfSet;                                  /* ģ��������� */
    MLIB_ModuleGet                      pfGet;                                  /* ģ�������ȡ */
    MLIB_ModuleProc                     pfProc;                                 /* ģ�����ݴ��� */
    MLIB_ModuleClear                    pfClear;                                /* ģ����� */
} MLIB_MODULE_REG_STRU;

/* ����ע����Ϣ */
typedef struct
{
    MLIB_PATH_ENUM_UINT32               enID;                                   /* ͨ·ID */
    MLIB_MODULE_REG_STRU               *pstModules;                             /* ͨ·ע��Ĵ���ģ���׵�ַ */
    MLIB_UINT32                         uwModuleNum;                            /* ͨ·ע��Ĵ���ģ����� */
} MLIB_PATH_REG_STRU;


/* ������ṹ�� */
typedef struct
{
    MLIB_PATH_REG_STRU                 *pstPaths;                               /* ����������ָ�� */
    MLIB_UINT32                         uwNum;                                  /* ������Ŀ */
} MLIB_PROFILE_REG_STRU;

/* ������������ */
typedef struct
{
    MLIB_UINT32                         uwProfile;                              /* ��ǰѡ�õ����� */
} MLIB_CONFIG_STRU;

/*****************************************************************************
  6 OTHERS����
*****************************************************************************/


/*****************************************************************************
  7 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  8 ��������
*****************************************************************************/

extern MLIB_ERROR_ENUM_UINT32  MLIB_PathClear( MLIB_PATH_ENUM_UINT32   enPath );
extern MLIB_MODULE_REG_STRU*  MLIB_PathGetModules(
                       MLIB_PATH_ENUM_UINT32   enID,
                       MLIB_UINT32            *puwNum);
extern MLIB_ERROR_ENUM_UINT32  MLIB_PathInit( MLIB_PATH_ENUM_UINT32 enPath);
extern MLIB_ERROR_ENUM_UINT32  MLIB_PathModuleSet(
                       MLIB_PATH_ENUM_UINT32   enPath,
                       MLIB_MODULE_ENUM_UINT32 enModule,
                       MLIB_PARA_STRU         *pstPara,
                       MLIB_UINT32             uwSize );
extern MLIB_ERROR_ENUM_UINT32  MLIB_PathModuleGet(
                MLIB_PATH_ENUM_UINT32   enPath,
                MLIB_MODULE_ENUM_UINT32 enModule,
                MLIB_VOID              *pvData,
                MLIB_UINT32             uwSize );
extern MLIB_ERROR_ENUM_UINT32  MLIB_PathProc(
                       MLIB_PATH_ENUM_UINT32   enPath,
                       MLIB_SOURCE_STRU       *pstSourceOut,
                       MLIB_SOURCE_STRU       *pstSourceIn);
extern MLIB_VOID  MLIB_UtilitySetSource(
                        MLIB_SOURCE_STRU       *pstSource,
                        MLIB_UINT32             uwChnID,
                        MLIB_VOID              *pvData,
                        MLIB_UINT32             uwSize,
                        MLIB_UINT32             uwSampleRate);
extern MLIB_ERROR_ENUM_UINT32  MLIB_PathSet(
                        MLIB_PATH_ENUM_UINT32   enPath,
                        MLIB_PARA_STRU         *pstPara,
                        MLIB_UINT32             uwSize );

extern MLIB_ERROR_ENUM_UINT32 MLIB_SelectProfile( MLIB_UINT32 uwID);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of mlib_interface.h */
