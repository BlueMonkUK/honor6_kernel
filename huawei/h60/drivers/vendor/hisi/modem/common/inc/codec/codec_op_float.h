


#ifndef __CODEC_BASICOP_FLOAT_H__
#define __CODEC_BASICOP_FLOAT_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "codec_typedefine.h"
#include "codec_op_lib.h"

#ifndef _MED_C89_
#include <xtensa/tie/xt_hifi2.h>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define CODEC_OP_Q13                      (VOS_INT16)13                           /* Q13 */
#define CODEC_OP_Q14                      (VOS_INT16)14                           /* Q14 */
#define CODEC_OP_Q15                      (VOS_INT16)15                           /* Q15 */
#define CODEC_OP_Q16                      (VOS_INT16)16                           /* Q16 */
#define CODEC_OP_Q17                      (VOS_INT16)17                           /* Q17 */
#define CODEC_OP_SHIFT_2                  (VOS_INT16)2                            /* 2λ��λ */
#define CODEC_OP_SHIFT_3                  (VOS_INT16)3                            /* 3λ��λ */
#define CODEC_OP_SHIFT_5                  (VOS_INT16)5                            /* 5λ��λ */
#define CODEC_OP_SHIFT_6                  (VOS_INT16)6                            /* 6λ��λ */
#define CODEC_OP_SHIFT_7                  (VOS_INT16)7                            /* 7λ��λ */
#define CODEC_OP_INT14_MAX                ((VOS_INT16)16383)                      /* 14bit�з��������ֵ */
#define CODEC_OP_INT16_HALF               ((VOS_INT16)16384)                      /* 16bit �з�����16384 */

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
/* α������ */
typedef struct
{
    VOS_INT16                           shwM;
    VOS_INT16                           shwE;
} CODEC_OP_FLOAT_STRU;

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern const CODEC_OP_FLOAT_STRU          CODEC_OP_FLOAT_ZERO;                      /* α����0 */
extern const CODEC_OP_FLOAT_STRU          CODEC_OP_FLOAT_ONE;                       /* α����1 */
extern const CODEC_OP_FLOAT_STRU          CODEC_OP_FLOAT_MAX16;                     /* α����32768 */

/*****************************************************************************
  10 ��������
*****************************************************************************/
#define CODEC_INLINE                      UCOM_INLINE


#ifndef _MED_C89_
 CODEC_INLINE CODEC_OP_FLOAT_STRU CODEC_OpToFloat(VOS_INT32 swX)
 {
    CODEC_OP_FLOAT_STRU    stRet = {0, 0};
    VOS_INT16            shwShift;

    ae_p24x2s  aepX;
    ae_q56s    aeqX;

    /* ֱ�ӷ��� */
    if (0 == swX)
    {
        return stRet;
    }

    /* shwShift = CODEC_OpNorm_l(swX); shwShift = CODEC_OP_Q16 - shwShift */
    aeqX     = AE_CVTQ48A32S(swX);
    shwShift = 24 - AE_NSAQ56S(aeqX);

    /* CODEC_OpL_shr_r(swX, shwShift) */
    if (shwShift >= 0)
    {
        aeqX = AE_SRAAQ56(aeqX, shwShift);
        aeqX = AE_ROUNDSQ32ASYM(aeqX);
    }
    else
    {
        aeqX = AE_SLLASQ56S(aeqX, -shwShift);
        aeqX = AE_SATQ48S(aeqX);
    }

    /* stRet.shwM = CODEC_OpSaturate() */
    aeqX = AE_SLLISQ56S(aeqX, 16);
    aeqX = AE_SATQ48S(aeqX);
    aepX = AE_TRUNCP24Q48(aeqX);

    stRet.shwM = AE_TRUNCA16P24S_L(aepX);

    /* stRet.shwM = shwShift */
    stRet.shwE = shwShift;

    return stRet;

 }
 #else
 CODEC_INLINE CODEC_OP_FLOAT_STRU CODEC_OpToFloat(VOS_INT32 swX)
 {
    CODEC_OP_FLOAT_STRU    stRet       = {0, 0};
    VOS_INT16            shwShift;

    /* ֱ�ӷ��� */
    if (0 == swX)
    {
        return stRet;
    }

    shwShift = CODEC_OpNorm_l(swX);
    shwShift = CODEC_OP_Q16 - shwShift;
    stRet.shwM = CODEC_OpSaturate(CODEC_OpL_shr_r(swX, shwShift));
    stRet.shwE = shwShift;

    return stRet;
 }
 #endif


CODEC_INLINE VOS_INT32 CODEC_OpFloatTo32(CODEC_OP_FLOAT_STRU stX)
{
    VOS_INT32       swTmp;

    if (stX.shwE >= 0)
    {
        swTmp = CODEC_OpL_shl(stX.shwM, stX.shwE);
    }
    else
    {
        swTmp = CODEC_OpL_shr_r(stX.shwM, CODEC_OpNegate(stX.shwE));
    }

    return swTmp;
}


 CODEC_INLINE VOS_INT16 CODEC_OpFloatTo16(CODEC_OP_FLOAT_STRU stX)
 {
    VOS_INT32                               swValTmp;

    swValTmp = CODEC_OpFloatTo32(stX);
    return (VOS_INT16)CODEC_OpSaturate(swValTmp);
 }


CODEC_INLINE CODEC_OP_FLOAT_STRU CODEC_OpFloatMul(
                CODEC_OP_FLOAT_STRU stA,
                CODEC_OP_FLOAT_STRU stB)
{
    CODEC_OP_FLOAT_STRU               stRet = {0, 0};

    if((0 == stA.shwM) || (0 == stB.shwM))
    {
         return stRet;
    }

    stRet.shwM = CODEC_OpMult(stA.shwM, stB.shwM);
    stRet.shwE = CODEC_OpAdd(CODEC_OpAdd(stA.shwE, stB.shwE), CODEC_OP_Q15);

    /* ��һ���� */
    if (CODEC_OpAbs_s(stRet.shwM) <= CODEC_OP_INT14_MAX)
    {
        stRet.shwM = CODEC_OpShl(stRet.shwM, 1);
        stRet.shwE -= 1;
    }

    return stRet;
}
CODEC_INLINE CODEC_OP_FLOAT_STRU CODEC_OpFloatAmult(
                CODEC_OP_FLOAT_STRU stA,
                CODEC_OP_FLOAT_STRU stB)
{
    CODEC_OP_FLOAT_STRU               stRet               = {0, 0};

    if((0 == stA.shwM) || (0 == stB.shwM))
    {
         return stRet;
    }

    stRet.shwM = CODEC_OpMult(stA.shwM, stB.shwM);
    stRet.shwE = stA.shwE + stB.shwE + CODEC_OP_Q15;

    return stRet;
}


CODEC_INLINE VOS_INT32 CODEC_OpFloatMul32(CODEC_OP_FLOAT_STRU stA, VOS_INT32 swB)
{
    VOS_INT32         swRet     = 0;

    swRet = CODEC_OpL_mpy_32_16(swB, stA.shwM);;
    swRet = CODEC_OpL_shr(swRet, CODEC_OpNegate(stA.shwE) - CODEC_OP_Q15);;

    return swRet;

}


CODEC_INLINE CODEC_OP_FLOAT_STRU CODEC_OpFloatMul32U(VOS_INT32 swA, VOS_INT32 swB)
{
    CODEC_OP_FLOAT_STRU               stRet               = {0, 0};
    VOS_INT16                           shwExpA             = 0;
    VOS_INT16                           shwExpB             = 0;
    VOS_INT16                           shwATmp;
    VOS_INT16                           shwBTmp;

    if((0 == swA) || (0 == swB))
    {
        stRet.shwM = 0;
        stRet.shwE = 0;
        return stRet;
    }

    shwExpA     = CODEC_OpLog2(CODEC_OpL_abs(swA));
    shwExpB     = CODEC_OpLog2(CODEC_OpL_abs(swB));

    shwATmp     = (VOS_INT16)CODEC_OpL_shr_r(swA, (shwExpA - CODEC_OP_Q14));
    shwBTmp     = (VOS_INT16)CODEC_OpL_shr_r(swB, (shwExpB - CODEC_OP_Q14));

    stRet.shwM  = CODEC_OpMult_r(shwATmp, shwBTmp);
    stRet.shwE  = (shwExpA + shwExpB) - CODEC_OP_Q13;

    /* ��һ���� */
    if (CODEC_OpAbs_s(stRet.shwM) < CODEC_OP_INT16_HALF)
    {
        stRet.shwM = CODEC_OpShl(stRet.shwM, 1);
        stRet.shwE -= 1;
    }

    return stRet;

}
CODEC_INLINE CODEC_OP_FLOAT_STRU CODEC_OpFloatAMul32U(
                VOS_INT32 swA,
                VOS_INT32 swB)
{
    CODEC_OP_FLOAT_STRU               stRet               = {0, 0};
    VOS_INT16                           shwExpA             = 0;
    VOS_INT16                           shwExpB             = 0;
    VOS_INT16                           shwATmp;
    VOS_INT16                           shwBTmp;

    if((0 == swA) || (0 == swB))
    {
        stRet.shwM = 0;
        stRet.shwE = 0;
        return stRet;
    }

    shwExpA     = CODEC_OpLog2(CODEC_OpL_abs(swA));
    shwExpB     = CODEC_OpLog2(CODEC_OpL_abs(swB));

    shwATmp     = (VOS_INT16)CODEC_OpL_shr(swA, (shwExpA - CODEC_OP_Q14));
    shwBTmp     = (VOS_INT16)CODEC_OpL_shr(swB, (shwExpB - CODEC_OP_Q14));

    stRet.shwM  = CODEC_OpMult(shwATmp, shwBTmp);
    stRet.shwE  = (shwExpA + shwExpB) - CODEC_OP_Q13;

    return stRet;

}


CODEC_INLINE CODEC_OP_FLOAT_STRU CODEC_OpFloatDivU(
                CODEC_OP_FLOAT_STRU stA,
                CODEC_OP_FLOAT_STRU stB)
{
    CODEC_OP_FLOAT_STRU                   stRet  = {0, 0};
    VOS_INT16                           shwExp = 0;
    VOS_INT16                           shwM   = 0;
    VOS_INT32                           swM    = 0;

    if (0 == stA.shwM)
    {
        return stRet;
    }

    if ((stA.shwM < 0) || (stB.shwM <= 0))
    {
        return CODEC_OP_FLOAT_ONE;
    }

    swM  = stA.shwM;
    shwM = stA.shwM;
    while (shwM >= stB.shwM)
    {
       shwExp++;
       shwM    = CODEC_OpShr_r(shwM, 1);
    }

    stRet.shwM = CODEC_OpSaturate(CODEC_OpL_shl(swM, CODEC_OP_Q15 - shwExp) / (VOS_INT32)stB.shwM);

    stRet.shwE = (stA.shwE - stB.shwE) - (CODEC_OP_Q15 - shwExp);

    return stRet;
}

extern  CODEC_OP_FLOAT_STRU CODEC_OpFloatSqrt(CODEC_OP_FLOAT_STRU stX);
extern VOS_INT16 CODEC_OpFloatLT(CODEC_OP_FLOAT_STRU stA, CODEC_OP_FLOAT_STRU stB);

extern CODEC_OP_FLOAT_STRU CODEC_OpFloatAdd(
                CODEC_OP_FLOAT_STRU stA,
                CODEC_OP_FLOAT_STRU stB);
extern CODEC_OP_FLOAT_STRU CODEC_OpFloatSub(
                CODEC_OP_FLOAT_STRU stA,
                CODEC_OP_FLOAT_STRU stB);


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of codec_op_float.h */

