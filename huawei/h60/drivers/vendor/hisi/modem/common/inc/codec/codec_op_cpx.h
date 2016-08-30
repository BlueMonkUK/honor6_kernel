

#ifndef _CODEC_BASIOP_CPX_H_
#define _CODEC_BASIOP_CPX_H_

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "codec_typedefine.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
  4 STRUCT����
*****************************************************************************/

/* ���� */
typedef struct
{
    VOS_INT16                           shwR;                                   /* ����ʵ�� */
    VOS_INT16                           shwI;                                   /* �����鲿 */
} CODEC_OP_CMPX_STRU;

/* 32bit���� */
typedef struct
{
    VOS_INT32                           swR;                                    /* ����ʵ�� */
    VOS_INT32                           swI;                                    /* �����鲿 */
} CODEC_OP_CMPX32_STRU;

/*****************************************************************************
  5 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  6 UNION����
*****************************************************************************/

/*****************************************************************************
  7 OTHERS����
*****************************************************************************/

/*****************************************************************************
  8 ��������
*****************************************************************************/

/*****************************************************************************
  9 ��������ʵ��
*****************************************************************************/

#define CODEC_OpCpxAdd(ret, a, b) /*lint -e{717} */ \
    do { \
        (ret).shwR = CODEC_OpAdd( (a).shwR, (b).shwR ); \
        (ret).shwI = CODEC_OpAdd( (a).shwI, (b).shwI ); \
    }while(0)

#define CODEC_OpCpxSub(ret, a, b) /*lint -e{717} */ \
    do { \
        (ret).shwR = CODEC_OpSub( (a).shwR, (b).shwR ); \
        (ret).shwI = CODEC_OpSub( (a).shwI, (b).shwI ); \
    }while(0)

/* ע��: ret������a����bΪ��ͬ���� */
#define CODEC_OpCpxMul(ret32, a, b) /*lint -e{717} */ \
    do{ \
        (ret32).swR = CODEC_OpL_sub( CODEC_OpL_mult0((a).shwR,(b).shwR), CODEC_OpL_mult0((a).shwI,(b).shwI) ); \
        (ret32).swI = CODEC_OpL_add( CODEC_OpL_mult0((a).shwR,(b).shwI), CODEC_OpL_mult0((a).shwI,(b).shwR) ); \
    }while(0)

#define CODEC_OpCpxShr_r16(ret, n) /*lint -e{717}*/\
    do{ \
        (ret).shwR = CODEC_OpShr_r((ret).shwR, n); \
        (ret).shwI = CODEC_OpShr_r((ret).shwI, n); \
    }while(0)

#define CODEC_OpCpxAdd32(ret, a, b) /*lint -e{717} */ \
    do { \
        (ret).swR = CODEC_OpL_add( (a).swR, (b).swR ); \
        (ret).swI = CODEC_OpL_add( (a).swI, (b).swI ); \
    }while(0)

#define CODEC_OpCpxSub32(ret, a, b) /*lint -e{717} */ \
    do { \
        (ret).swR = CODEC_OpL_sub( (a).swR, (b).swR ); \
        (ret).swI = CODEC_OpL_sub( (a).swI, (b).swI ); \
    }while(0)

#define CODEC_OpCpxShl32(ret, n) /*lint -e{717} */ \
    do{ \
        (ret).swR = CODEC_OpL_shl((ret).swR, n); \
        (ret).swI = CODEC_OpL_shl((ret).swI, n); \
    }while(0)

#define CODEC_OpCpxShr_r32(ret, n) /*lint -e{717} */ \
    do{ \
        (ret).swR = CODEC_OpL_shr_r((ret).swR, n); \
        (ret).swI = CODEC_OpL_shr_r((ret).swI, n); \
    }while(0)

#define CODEC_OpCpx16To32(ret32, a16) /*lint -e{717} */ \
    do{ \
        (ret32).swR = (VOS_INT32)((a16).shwR); \
        (ret32).swI = (VOS_INT32)((a16).shwI); \
    }while(0)

#define CODEC_OpCpx32To16(ret16, a32) /*lint -e{717} */ \
    do{ \
        (ret16).shwR = (VOS_INT16)CODEC_OpSaturate((a32).swR); \
        (ret16).shwI = (VOS_INT16)CODEC_OpSaturate((a32).swI); \
    }while(0)

extern CODEC_OP_CMPX_STRU CODEC_OpCpxMulQ15(
                CODEC_OP_CMPX_STRU  stA,
                CODEC_OP_CMPX_STRU  stB);

extern CODEC_OP_CMPX_STRU CODEC_OpCpxMulQ17(
                CODEC_OP_CMPX_STRU  stA,
                CODEC_OP_CMPX_STRU  stB);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of codec_op_cpx.h*/



