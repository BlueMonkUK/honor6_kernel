

#ifndef __CODEC_COM_ETSI_OP_H__
#define __CODEC_COM_ETSI_OP_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#ifndef _MED_C89_
#include "codec_op_etsi_hifi.h"
#endif


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 �������Ͷ���
*****************************************************************************/

/* ��׼Cƽ̨���� */
#ifdef _MED_C89_

/* �����������Ͷ��� */
typedef char                            Char;
typedef signed char                     Word8;
typedef unsigned char                   UWord8;
typedef short                           Word16;
typedef unsigned short                  UWord16;
typedef long                            Word32;
typedef unsigned long                   UWord32;
typedef int                             Flag;

#endif


/*****************************************************************************
  3 �궨��
*****************************************************************************/
#define MAX_32                          ((Word32)0x7fffffffL)                   /*����32λ��*/
#define MIN_32                          ((Word32)0x80000000L)                   /*��С��32λ��*/
#define MAX_16                          ((Word16)0x7fff)                        /*����16λ��*/
#define MIN_16                          ((Word16)0x8000)                        /*��С��16λ��*/

/* ��׼Cƽ̨���� */
#ifdef _MED_C89_

#define CODEC_OpSetOverflow(swFlag)       (Overflow = swFlag)
#define CODEC_OpGetOverflow()             (Overflow)
#define CODEC_OpSetCarry(swFlag)          (Carry = swFlag)
#define CODEC_OpGetCarry()                (Carry)

#define XT_INLINE                       static
#endif

/*****************************************************************************
  4 ö�ٶ���
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
#define round(L_var1)                   round_etsi(L_var1)

/*****************************************************************************
  10 ȫ�ֱ�������
*****************************************************************************/
/* ��׼Cƽ̨���� */
#ifdef _MED_C89_

extern Flag Overflow;
extern Flag Carry;

#endif


/*****************************************************************************
  11 ��������
*****************************************************************************/
#ifdef _MED_C89_

/* ETSI : basic_op.h */
extern Word16 saturate (Word32 swVar);                                          /* 32bit->16bit */
extern Word16 add(Word16 shwVar1, Word16 shwVar2);                              /* Short add */
extern Word16 sub(Word16 shwVar1, Word16 shwVar2);                              /* Short sub */
extern Word16 abs_s(Word16 shwVar1);                                            /* Short abs */
extern Word16 shl(Word16 shwVar1, Word16 shwVar2);                              /* Short shift left */
extern Word16 shr(Word16 shwVar1, Word16 shwVar2);                              /* Short shift right*/
extern Word16 mult(Word16 shwVar1, Word16 shwVar2);                             /* Short mult */
extern Word32 L_mult(Word16 shwVar1, Word16 shwVar2);                           /* Long mult */
extern Word16 negate(Word16 shwVar1);                                           /* Short negate */
extern Word16 extract_h(Word32 swVar1);                                         /* Extract high */
extern Word16 extract_l(Word32 swVar1);                                         /* Extract low */
extern Word16 round_etsi(Word32 swVar1);                                        /* Round */
extern Word32 L_mac(Word32 swVar3, Word16 shwVar1, Word16 shwVar2);             /* Mac */
extern Word32 L_msu(Word32 swVar3, Word16 shwVar1, Word16 shwVar2);             /* Msu */
extern Word32 L_macNs(Word32 swVar3, Word16 shwVar1, Word16 shwVar2);           /* Mac without sat */
extern Word32 L_msuNs(Word32 swVar3, Word16 shwVar1, Word16 shwVar2);           /* Msu without sat */
extern Word32 L_add(Word32 swVar1, Word32 swVar2);                              /* Long add */
extern Word32 L_sub(Word32 swVar1, Word32 swVar2);                              /* Long sub */
extern Word32 L_add_c(Word32 swVar1, Word32 swVar2);                            /* Long add with c */
extern Word32 L_sub_c(Word32 swVar1, Word32 swVar2);                            /* Long sub with c */
extern Word32 L_negate(Word32 swVar1);                                          /* Long negate */
extern Word16 mult_r(Word16 shwVar1, Word16 shwVar2);                           /* Mult with round */
extern Word32 L_shl(Word32 swVar1, Word16 shwVar2);                             /* Long shift left */
extern Word32 L_shr(Word32 swVar1, Word16 shwVar2);                             /* Long shift right */
extern Word16 shr_r(Word16 shwVar1, Word16 shwVar2);                            /* Shift right with round */
extern Word16 mac_r(Word32 swVar3, Word16 shwVar1, Word16 shwVar2);             /* Mac with rounding */
extern Word16 msu_r(Word32 swVar3, Word16 shwVar1, Word16 shwVar2);             /* Msu with rounding */
extern Word32 L_deposit_h(Word16 shwVar1);                                      /* 16 bit shwVar1 -> MSB */
extern Word32 L_deposit_l(Word16 shwVar1);                                      /* 16 bit shwVar1 -> LSB */
extern Word32 L_shr_r(Word32 swVar1, Word16 shwVar2);                           /* Long shift right with round */
extern Word32 L_abs(Word32 swVar1);                                             /* Long abs */
extern Word32 L_sat(Word32 swVar1);                                             /* Long saturation */
extern Word16 norm_s(Word16 shwVar1);                                           /* Short norm */
extern Word16 div_s(Word16 shwVar1, Word16 shwVar2);                            /* Short division */
extern Word16 norm_l(Word32 swVar1);                                            /* Long norm */

/* ETSI : oper_32b.h */

/* Extract from a 32 bit integer two 16 bit DPF */
extern void L_Extract(Word32 swVar32, Word16 *pshwHi, Word16 *pshwLo);

/* Compose from two 16 bit DPF a 32 bit integer */
extern Word32 L_Comp(Word16 shwHi, Word16 shwLo);

/* Multiply two 32 bit integers (DPF). The result is divided by 2**31 */
extern Word32 Mpy_32(Word16 shwHi1, Word16 shwLo1, Word16 shwHi2, Word16 shwLo2);

/* Multiply a 16 bit integer by a 32 bit (DPF). The result is divided by 2**15 */
extern Word32 Mpy_32_16(Word16 shwHi, Word16 shwLo, Word16 shwN);

/* Fractional integer division of two 32 bit numbers */
extern Word32 Div_32(Word32 swNum, Word16 denom_hi, Word16 denom_lo);

/* ETSI : mac_32.h */

/* Multiply two 32 bit integers (DPF) and accumulate with (normal) 32 bit  integer.
   The multiplication result is divided by 2**31 */
extern Word32 Mac_32(Word32 swVar32, Word16 shwHi1, Word16 shwLo1, Word16 shwHi2, Word16 shwLo2);

/* Multiply a 16 bit integer by a 32 bit (DPF) and accumulate with (normal)32 bit integer.
   The multiplication result is divided by 2**15 */
extern Word32 Mac_32_16(Word32 swVar32, Word16 shwHi, Word16 shwLo, Word16 shwN);

#endif /* ifdef _MED_C89_ */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of med_com_etsi_op.h */
