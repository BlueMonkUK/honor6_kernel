/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : med_vec_op.h
  �� �� ��   : ����
  ��    ��   : ��
  ��������   : 2011��5��24��
  ����޸�   :
  ��������   : med_vec_op.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __CODEC_VEC_OP_H__
#define __CODEC_VEC_OP_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "codec_op_etsi.h"
#ifndef _MED_C89_
#include "codec_op_vec_hifi.h"
#endif
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
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/


/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
#ifdef _MED_C89_

extern void CODEC_OpVcAnd(
                       const Word16 *pshwX,
                       const Word32  swLen,
                       const Word16  shwConst,
                       Word16 *pshwY);
extern Word32 CODEC_OpVcMac(
                       const Word16 *pshwX,
                       const Word32  swLen,
                       const Word16  shwConst,
                       Word32  swSum);
extern void CODEC_OpVcMult(
                       const Word16 *pshwX,
                       const Word32  swLen,
                       const Word16  shwConst,
                       Word16       *pshwZ);
#define CODEC_OpVcMultAlignedQuan         CODEC_OpVcMult
extern void CODEC_OpVcMultQ15Add(
                       const Word16 *pshwX,
                       const Word16 *pshwY,
                       const Word32  swLen,
                       const Word16  shwConst,
                       Word16       *pshwZ);
extern void CODEC_OpVcMultR(
                       const Word16 *pshwX,
                       const Word32  swLen,
                       const Word16  shwConst,
                       Word16       *pshwZ);
extern void CODEC_OpVcMultScale(
                       const Word16 *pshwX,
                       const Word32  swLen,
                       const Word16  shwConst,
                       Word16        shwScaleBit,
                       Word16       *pshwY);
extern VOS_VOID CODEC_OpVcSub(
                       const Word16 *pshwX,
                       const Word32  swLen,
                       const Word16  shwY,
                       Word16       *pshwZ);
extern VOS_VOID CODEC_OpVecAbs(
                             Word16 *pshwX,
                             Word32  swLen,
                             Word16 *pshwY);
extern void CODEC_OpVecCpy(
                       Word16       *pshwDes,
                       const Word16 *pshwSrc,
                       const Word32  swLen);
#define CODEC_OpVecAlign                  CODEC_OpVecCpy
extern Word16 CODEC_OpVecMax(
                       const Word16 *pshwX,
                       const Word32  swLen,
                       Word16       *pswMaxPos);
extern Word16 CODEC_OpVecMaxAbs(
                       const Word16 *pshwX,
                       const Word32  swLen,
                       Word16       *pswMaxPos);
extern Word16 CODEC_OpVecMin(
                       const Word16 *pshwX,
                       const Word32 swLen,
                       Word16      *pswMinPos);
extern Word32 CODEC_OpVecScaleMac(
                       const Word16 *pshwX,
                       const Word32  swLen,
                       Word32        swSum,
                       Word16        shwBits);
extern void CODEC_OpVecSet(
                       Word16       *pshwDes,
                       const Word32  swLen,
                       const Word16  shwValue);
extern void CODEC_OpVecShl(
                       const Word16 *pshwX,
                       const Word32  swLen,
                       Word16        shwBits,
                       Word16       *pshwY);
extern void CODEC_OpVecShr(
                       const Word16 *pshwX,
                       const Word32  swLen,
                       Word16        shwBits,
                       Word16       *pshwY);
extern VOS_VOID CODEC_OpVecShr_r(
                       Word16 *pshwX,
                       Word32  swLen,
                       Word16  shwBits,
                       Word16 *pshwY);
extern Word16 CODEC_OpVecSum(
                       const Word16 *pshwX,
                       const Word32  swLen);
extern void CODEC_OpVvAdd(
                       const Word16 *pshwX,
                       const Word16 *pshwY,
                       const Word32  swLen,
                       Word16       *pshwZ);
extern void CODEC_OpVvAdd32(
                       const Word32 *pswX,
                       const Word32 *pswY,
                       const Word32  swLen,
                       Word32       *pswZ);
extern void CODEC_OpVvFormWithCoef(
                       const Word16 *pshwX,
                       const Word16 *pshwY,
                       const Word32  swLen,
                       const Word16  shwA,
                       const Word16  shwB,
                       Word16 *pshwZ);
extern void CODEC_OpVvFormWithDimi(
                       Word16 *pshwX,
                       Word16 *pshwY,
                       Word32  swLen,
                       Word16 *pshwZ);
extern void CODEC_OpVvFormWithQuar(
                       Word16 *pshwX,
                       Word16 *pshwY,
                       Word32  swLen,
                       Word16 *pshwZ);
extern Word32 CODEC_OpVvMac(
                       const Word16 *pshwX,
                       const Word16 *pshwY,
                       const Word32  swLen,
                       Word32        swSum);
#define CODEC_OpVvMacAlignless                CODEC_OpVvMac
#define CODEC_OpVvMacAligned                  CODEC_OpVvMac
#define CODEC_OpVvMacAlignedEven              CODEC_OpVvMac
#define CODEC_OpVvMacAlignedQuan              CODEC_OpVvMac
#define CODEC_OpVvSelfMacAlignedQuan(x,l,s)   CODEC_OpVvMac((x),(x),(l),(s))

extern Word32 CODEC_OpVvMacD(
                       const Word16 *pshwX,
                       const Word16 *pshwY,
                       const Word32  swLen,
                       Word32        swSum);
extern Word32 CODEC_OpVvMsu(
                       const Word16 *pshwX,
                       const Word16 *pshwY,
                       const Word32  swLen,
                       Word32        swSum);
extern Word32 CODEC_OpVvMsuD(
                       const Word16 *pshwX,
                       const Word16 *pshwY,
                       const Word32  swLen,
                       Word32        swSum);
#define CODEC_OpVvMsuDAlignedEven         CODEC_OpVvMsuD
#define CODEC_OpVvMsuDAlignedQuan         CODEC_OpVvMsuD
#define CODEC_OpVvMsuDAlignedless         CODEC_OpVvMsuD
#define CODEC_OpVvMsuDSemiAlignedQuan     CODEC_OpVvMsuD
extern void CODEC_OpVvMult(
                       const Word16 *pshwX,
                       const Word16 *pshwY,
                       const Word32  swLen,
                       Word16       *pshwZ);
extern void CODEC_OpVvMultR(
                       const Word16 *pshwX,
                       const Word16 *pshwY,
                       const Word32  swLen,
                       Word16       *pshwZ);
extern VOS_VOID CODEC_OpVvMultRD(
                       Word16 *pshwX,
                       Word16 *pshwY,
                       Word32  swLen,
                       Word16 *pshwZ);
extern void CODEC_OpVvSub(
                       const Word16 *pshwX,
                       const Word16 *pshwY,
                       const Word32  swLen,
                       Word16       *pshwZ);
#define CODEC_OpVecMeanAbsAlignedQuan     CODEC_OpVecMeanAbs
extern VOS_VOID CODEC_OpVecShr_r32(
                       Word32 *pswX,
                       Word32  swLen,
                       Word16  shwBits,
                       Word32 *pswY);

#endif /* ifdef _MED_C89_ */

extern Word16 CODEC_OpVecMeanAbs(Word16 *pshwArr, Word32 swArrLen);
extern Word32 CODEC_OpVecMean32(Word32 *pswArr, Word32 swArrLen);
extern Word32 CODEC_OpVecMax32(Word32 *pArr, Word32 swLen);
extern Word32 CODEC_OpVecMin32(Word32 *pArr, Word32 swLen);
extern VOS_VOID CODEC_OpVecShr32(
                        Word32 *pswX,
                        Word32  swLen,
                        Word16  shwBits,
                        Word32 *pswY);
extern Word32 CODEC_OpVecSum32(Word32 *pswArray, Word32 swLen);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of med_com_vec_op.h */
