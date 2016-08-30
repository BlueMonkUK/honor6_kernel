/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : Med_com_netsi_op.h
  �� �� ��   : ����
  ��    ��   : ��
  ��������   : 2011��9��1��
  ����޸�   :
  ��������   : Med_com_netsi_op.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��9��1��
    ��    ��   : ��
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "codec_op_etsi.h"
#include  "codec_typedefine.h"

#ifndef __CODEC_COM_NETSI_OP_H__
#define __CODEC_COM_NETSI_OP_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define             CODEC_OP_LOG_OFFSET                       ((VOS_INT32)626255212) /* 9.3319 scaled as 5,26 */
#define             CODEC_OP_SQROOT_PLUS_HALF                 ((VOS_INT32)0x40000000)/* 0.5 */
#define             CODEC_OP_SQROOT_MINUS_ONE                 ((VOS_INT32)0x80000000)/* -1 */
#define             CODEC_OP_SQROOT_TERM5_MULTIPLER           ((VOS_INT16)0x5000)    /* 0.625 */
#define             CODEC_OP_SQROOT_TERM6_MULTIPLER           ((VOS_INT16)0x7000)    /* 0.875 */
/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


/*****************************************************************************
  4 ������������
*****************************************************************************/
#ifndef _MED_C89_

/*****************************************************************************
 �� �� ��  : L_mult0
 ��������  : return (long)shwVar1*shwVar2
 �������  : Word16 shwVar1  - ����1
             Word16 shwVar2  - ����2
 �������  : ��
 �� �� ֵ  : Word32          - �˻�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��9��2��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
XT_INLINE Word32 L_mult0(Word16 shwVar1, Word16 shwVar2)
{
    Word32 swOut;

    swOut = (Word32)shwVar1 * (Word32)shwVar2;

    return (swOut);
}

/*****************************************************************************
 �� �� ��  : L_mac0
 ��������  : 16bit*16bit+32bit=32bit��16bit��ʱ������λ���ۼ�ʱ������
 �������  : Word32 swVar3       --�ۼӳ�ֵ
             Word16 shwVar1      --����
             Word16 shwVar2      --����
 �������  : ��
 �� �� ֵ  : Word32              --�����ۼ�ֵ
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��9��1��
    ��    ��   : ��
    �޸�����   : �����ɺ���

*****************************************************************************/
XT_INLINE Word32 L_mac0(Word32 swVar3, Word16 shwVar1, Word16 shwVar2)
{
    Word32      swVarOut;

    ae_p24x2s   p1, p2;
    ae_q56s     q1;

    /* shwVar(1.16) --> p(9.24) */
    p1 = AE_CVTP24A16(shwVar1);
    p2 = AE_CVTP24A16(shwVar2);

    /* swVar3��1.32�� --> q1(17.48) */
    q1 = AE_CVTQ48A32S(swVar3);

    /* q1(1.56)+p1(1.24)*p2(1.24) --> q1(1.56) */
    AE_MULAS56P24S_LL(q1, p1, p2);

    /* q1��48bit���� */
    q1 = AE_SATQ48S(q1);

    /* q1(17.48) --> swVarOut */
    swVarOut = AE_TRUNCA32Q48(q1);

    return swVarOut;

}

#endif

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
extern Word32 L_mac0(Word32 swVar3, Word16 shwVar1, Word16 shwVar2);
extern Word32 L_mult0(Word16 shwVar1, Word16 shwVar2);
#endif

extern Word32 L_mpy_ls(Word32 swVar1, Word16 shwVar2);
extern VOS_INT16 CODEC_OpNormDiv_16 (Word16 shwNum, Word16 shwDenom, Word16 shwQn);
extern VOS_INT32 CODEC_OpExp10(VOS_INT32 swA);
extern VOS_INT32 CODEC_OpfnExp2(VOS_INT32 swInput);
extern VOS_INT32 CODEC_OpfnLog10(VOS_INT32 swInput);
extern VOS_INT32 CODEC_OpfnLog2(VOS_INT32 swInput);
extern VOS_INT16 CODEC_OpLog10(VOS_INT16 shwQ, VOS_INT32 swA);
extern VOS_INT32 CODEC_OpL_divide(VOS_INT32 swNum, VOS_INT32 swDenom);
extern VOS_INT32 CODEC_OpL_mpy_32(VOS_INT32 L_var1, VOS_INT32 L_var2);
extern VOS_INT32 CODEC_OpL_mpy_32_16_r(VOS_INT32 swA, VOS_INT16 shwB);
extern VOS_INT16 CODEC_OpMult_shr(VOS_INT16 shwA, VOS_INT16 shwB, VOS_INT16 shwShift);
extern VOS_INT32 CODEC_OpNormDiv_32(VOS_INT32 swNum, VOS_INT32 swDenom, VOS_INT16 shwQ);
extern VOS_INT16 CODEC_OpSqrt(VOS_INT32 swA);
extern VOS_INT16 CODEC_OpSqroot(VOS_INT32 swSqrtIn);
extern VOS_INT16 CODEC_OpLog2(VOS_INT32 swX);








#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of Med_com_netsi_op.h */
