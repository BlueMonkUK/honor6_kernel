

#ifndef __CSDCSTINTERFACE_H__
#define __CSDCSTINTERFACE_H__


/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "vos.h"


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
 �ṹ��    : CST_CSD_DATA_IND_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : CSTͨ��DICCͨ�������������ݽṹ
*****************************************************************************/

typedef struct
{
    VOS_UINT16                         usLen;            /* ���ݳ��ȵ�λ�ֽ� */
    VOS_UINT16                         usResrved;        /* �ֽڶ��� */
    VOS_UINT8                         *pucData;          /* ָ������ָ�� */
    VOS_VOID                          *pGarbage;         /* �ڴ����ָ�룬ָ��TTFMEMͷ */

}CST_CSD_DATA_IND_STRU;


/*****************************************************************************
 �ṹ��    : CSD_CST_DATA_REQ_STRU
 Э�����  :
 ASN.1���� :
 �ṹ˵��  : CSDͨ��DICCͨ�������������ݽṹ
*****************************************************************************/

typedef struct
{
    VOS_UINT16                         usLen;            /* ���ݳ��ȵ�λ�ֽ� */
    VOS_UINT16                         usResrved;        /* �ֽڶ��� */
    VOS_UINT8                         *pucData;          /* ָ������ָ�� */
    VOS_VOID                          *pGarbage;         /* �ڴ����ָ�룬ָ��IMMͷ */
}CSD_CST_DATA_REQ_STRU;

/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/








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

#endif /* end of CsdCstInterface.h */
