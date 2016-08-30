
#ifndef __CODEC_COM_VEC_HIFI_H__
#define __CODEC_COM_VEC_HIFI_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#ifndef _MED_C89_
#include <xtensa/tie/xt_hifi2.h>
#endif
#include "codec_op_etsi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/

/* DSPƽ̨���� */
#ifndef _MED_C89_

#define CODEC_OpCheck4ByteAligned(swAddr) (0 == (((Word32)(swAddr)) & 0x03))    /* �жϱ�����ַ�Ƿ�Ϊ4�ֽڶ��� */
#define CODEC_OpCheckBit0Set(swLen)       (0 != ((swLen)  & 0x01))               /* �ж����������Ƿ�Ϊ������ */
#define CODEC_OpCheckBit1Set(swLen)       (0 != ((swLen)  & 0x02))               /* �ж��������ȵĵ�1bit�Ƿ�Ϊ1 */
#define CODEC_OP_MULFP24S_MAX_LEN         (64)                                    /* ��֤AE_MULAAFP24S������������㳤�� */

#endif

/*****************************************************************************
  3 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  4 ������������
*****************************************************************************/

#ifndef _MED_C89_

/*****************************************************************************
 �� �� ��  : CODEC_OpVcAnd
 ��������  : Y[i] = X[i]&Const
 �������  : const Word16 *pshwX    - ����
             const Word32 swLen     - ��������
             const Word16 shwConst  - ����
             Word16 *pshwY          - ���������������
 �������  : Word16 *pshwY          - ������
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE void CODEC_OpVcAnd(
                const Word16 *pshwX,
                const Word32 swLen,
                const Word16 shwConst,
                Word16 *pshwY)
{
    Word32 k;
    ae_p24x2s aepX1, aepX2, aepY1, aepY2, aepC;

    if (swLen < 0)
    {
        return;
    }

    /* shwConst-->aepC.HL */
    aepC = AE_CVTP24A16(shwConst);

    /* ������ַΪ4�ֽڶ���ʱ����˫���� */
    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwY))
    {
        /* 2�鲢�м���,����nopָ�� */
        for (k = 0; k < swLen>>2; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
            aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

            aepY1 = AE_ANDP48(aepX1, aepC);
            aepY2 = AE_ANDP48(aepX2, aepC);

            *((ae_p16x2s *)&pshwY[4*k]) = aepY1;
            *((ae_p16x2s *)&pshwY[4*k+2]) = aepY2;
        }

        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = swLen & 0x7FFFFFFC;

            aepX1 = *((ae_p16x2s *)&pshwX[k]);
            aepY1 = AE_ANDP48(aepX1, aepC);
            *((ae_p16x2s *)&pshwY[k]) = aepY1;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepY1 = AE_ANDP48(aepX1, aepC);
            aepY2 = AE_ANDP48(aepX2, aepC);

            *((ae_p16s *)&pshwY[2*k]) = aepY1;
            *((ae_p16s *)&pshwY[2*k + 1]) = aepY2;
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        k = swLen - 1;
        aepX1 = *((ae_p16s *)&pshwX[k]);
        aepY1 = AE_ANDP48(aepX1, aepC);
        *((ae_p16s *)&pshwY[k]) = aepY1;
    }
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVcMac
 ��������  : Sum = L_mac(swSum, X[i], Const)
 �������  : const Word16 *pshwX    - ����
             const Word32 swLen     - ��������(���ó���64,�������������)
             const Word16 shwConst  - ����
             Word32 swSum           - �ۼƺͳ�ʼֵ
 �������  : ��
 �� �� ֵ  : Word32                 - �ۼӺ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE Word32 CODEC_OpVcMac(
                const Word16 *pshwX,
                const Word32  swLen,
                const Word16  shwConst,
                Word32        swSum)
{
    Word32 k;
    ae_p24x2s aepX1, aepX2, aepC;
    ae_q56s aeqS1;

    /* shwConst-->aepC.HL */
    aepC = AE_CVTP24A16(shwConst);

    /* swSum-->aeqS(17.48) */
    aeqS1 = AE_CVTQ48A32S(swSum);

    if (CODEC_OpCheck4ByteAligned(pshwX))
    {
        /* 2�鲢�м���,����nopָ�� */
        for (k = 0; k < swLen>>2; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
            aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

            AE_MULAAFP24S_HH_LL(aeqS1, aepX1, aepC);
            AE_MULAAFP24S_HH_LL(aeqS1, aepX2, aepC);
        }

        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = (swLen & 0x7FFFFFFC);

            aepX1 = *((ae_p16x2s *)&pshwX[k]);
            AE_MULAAFP24S_HH_LL(aeqS1, aepX1, aepC);
        }
        /* ���͵�48bit */
        aeqS1 = AE_SATQ48S(aeqS1);

    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            AE_MULAFS32P16S_LL(aeqS1, aepX1, aepC);
            AE_MULAFS32P16S_LL(aeqS1, aepX2, aepC);
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);
        AE_MULAFS32P16S_LL(aeqS1, aepX1, aepC);
    }

    return AE_TRUNCA32Q48(aeqS1);

}

/*****************************************************************************
�� �� ��  : CODEC_OpVcMult
��������  : Y[i] = mult(X[i], Const);
�������  : const Word16 *pshwX      - ����������
            const Word32  swLen      - ��������
            const Word16  shwConst   - ��������
            Word16 *pshwY            - ������������
�������  : Word16 *pshwY            - ������
�� �� ֵ  : void
���ú���  :
��������  :

�޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE void CODEC_OpVcMult(
                   const Word16 *pshwX,
                   const Word32  swLen,
                   const Word16  shwConst,
                   Word16       *pshwY)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepY1, aepX2, aepY2, aepC;
    ae_q56s aeqL1, aeqH1, aeqL2, aeqH2;

    /* shwConst-->aepC.HL */
    aepC = AE_CVTP24A16(shwConst);

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwY))
    {
        /* 2�鲢�м��� */
        for (k = 0; k < swLen>>2; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
            aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

            aeqH1 =  AE_MULFS32P16S_HH(aepX1, aepC);
            aeqL1 = AE_MULFS32P16S_LL(aepX1, aepC);
            aepY1 = AE_TRUNCP24Q48X2(aeqH1, aeqL1);

            aeqH2 =  AE_MULFS32P16S_HH(aepX2, aepC);
            aeqL2 = AE_MULFS32P16S_LL(aepX2, aepC);
            aepY2 = AE_TRUNCP24Q48X2(aeqH2, aeqL2);

            *((ae_p16x2s *)&pshwY[4*k])     = aepY1;
            *((ae_p16x2s *)&pshwY[4*k + 2]) = aepY2;
        }
        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = (swLen & 0x7FFFFFFC);

            aepX1 = *((ae_p16x2s *)&pshwX[k]);

            aeqH1 =  AE_MULFS32P16S_HH(aepX1, aepC);
            aeqL1 = AE_MULFS32P16S_LL(aepX1, aepC);
            aepY1 = AE_TRUNCP24Q48X2(aeqH1, aeqL1);

            *((ae_p16x2s *)&pshwY[k]) = aepY1;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aeqL1 = AE_MULFS32P16S_LL(aepX1, aepC);
            aeqL2 = AE_MULFS32P16S_LL(aepX2, aepC);

            aepY1 = AE_TRUNCP24Q48(aeqL1);
            aepY2 = AE_TRUNCP24Q48(aeqL2);

            *((ae_p16s *)&pshwY[2*k]) = aepY1;
            *((ae_p16s *)&pshwY[2*k + 1]) = aepY2;
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);

        aeqL1 = AE_MULFS32P16S_LL(aepX1, aepC);
        aepY1 = AE_TRUNCP24Q48(aeqL1);

        *((ae_p16s *)&pshwY[swLen - 1]) = aepY1;
    }
}

/*****************************************************************************
�� �� ��  : CODEC_OpVcMultAlignedQuan
��������  : Y[i] = mult(X[i], Const);
�������  : const Word16 *pshwX      - ����������(4byte����)
            const Word32  swLen      - ��������(4�ı���)
            const Word16  shwConst   - ��������
            Word16 *pshwY            - ������������(4byte����)
�������  : Word16 *pshwY            - ������
�� �� ֵ  : void
���ú���  :
��������  :

�޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE void CODEC_OpVcMultAlignedQuan(
                   const Word16 *pshwX,
                   const Word32  swLen,
                   const Word16  shwConst,
                   Word16       *pshwY)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepY1, aepX2, aepY2, aepC;
    ae_q56s aeqL1, aeqH1, aeqL2, aeqH2;

    /* shwConst-->aepC.HL */
    aepC = AE_CVTP24A16(shwConst);

    /* 2�鲢�м��� */
    for (k = 0; k < swLen>>2; k++)
    {
        aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
        aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

        aeqH1 =  AE_MULFS32P16S_HH(aepX1, aepC);
        aeqL1 = AE_MULFS32P16S_LL(aepX1, aepC);
        aepY1 = AE_TRUNCP24Q48X2(aeqH1, aeqL1);

        aeqH2 =  AE_MULFS32P16S_HH(aepX2, aepC);
        aeqL2 = AE_MULFS32P16S_LL(aepX2, aepC);
        aepY2 = AE_TRUNCP24Q48X2(aeqH2, aeqL2);

        *((ae_p16x2s *)&pshwY[4*k])     = aepY1;
        *((ae_p16x2s *)&pshwY[4*k + 2]) = aepY2;
    }
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVcMultQ15Add
 ��������  : Z[i] = add(X[i], mult(Y[i], Const));
 �������  : const Word16 *pshwX      - �����ӵ�����
             const Word16 *pshwY      - �볣����˵�����
             const Word32  swLen      - ��������
             const Word16  shwConst   - ����
             Word16 *pshwZ            - ������������
 �������  : Word16 *pshwZ            - ������
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE void CODEC_OpVcMultQ15Add(
                const Word16 *pshwX,
                const Word16 *pshwY,
                const Word32  swLen,
                const Word16  shwConst,
                Word16       *pshwZ)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepY1, aepZ1, aepX2, aepY2, aepZ2, aepC;
    ae_q56s aeqL1, aeqH1, aeqL2, aeqH2;

    if(swLen < 0)
    {
        return;
    }

    /* shwConst-->aepC.HL */
    aepC = AE_CVTP24A16(shwConst);

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwY|(Word32)pshwZ))
    {
        /* 2�鲢�м��� */
        for (k = 0; k < swLen>>2; k++)
        {
            aepY1 = *((ae_p16x2s *)&pshwY[4*k]);
            aepY2 = *((ae_p16x2s *)&pshwY[4*k + 2]);

            aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
            aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

            aeqH1 = AE_MULFS32P16S_HH(aepY1, aepC);
            aeqL1 = AE_MULFS32P16S_LL(aepY1, aepC);

            aeqH2 = AE_MULFS32P16S_HH(aepY2, aepC);
            aeqL2 = AE_MULFS32P16S_LL(aepY2, aepC);

            aepY1 = AE_TRUNCP24Q48X2(aeqH1, aeqL1);
            aepY1 = AE_TRUNCP16(aepY1);
            aepY2 = AE_TRUNCP24Q48X2(aeqH2, aeqL2);
            aepY2 = AE_TRUNCP16(aepY2);

            aepZ1 = AE_ADDSP24S(aepX1, aepY1);
            aepZ2 = AE_ADDSP24S(aepX2, aepY2);

            *((ae_p16x2s *)&pshwZ[4*k]) = aepZ1;
            *((ae_p16x2s *)&pshwZ[4*k + 2]) = aepZ2;
        }
        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = swLen & 0x7FFFFFFC;

            aepY1 = *((ae_p16x2s *)&pshwY[k]);
            aepX1 = *((ae_p16x2s *)&pshwX[k]);

            aeqH1 = AE_MULFS32P16S_HH(aepY1, aepC);
            aeqL1 = AE_MULFS32P16S_LL(aepY1, aepC);

            aepY1 = AE_TRUNCP24Q48X2(aeqH1, aeqL1);
            aepY1 = AE_TRUNCP16(aepY1);

            aepZ1 = AE_ADDSP24S(aepX1, aepY1);

            *((ae_p16x2s *)&pshwZ[k]) = aepZ1;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepY1 = *((ae_p16s *)&pshwY[2*k]);
            aepY2 = *((ae_p16s *)&pshwY[2*k + 1]);

            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aeqL1 = AE_MULFS32P16S_LL(aepY1, aepC);
            aeqL2 = AE_MULFS32P16S_LL(aepY2, aepC);

            aepY1 = AE_TRUNCP24Q48(aeqL1);
            aepY1 = AE_TRUNCP16(aepY1);

            aepY2 = AE_TRUNCP24Q48(aeqL2);
            aepY2 = AE_TRUNCP16(aepY2);

            aepZ1 = AE_ADDSP24S(aepX1, aepY1);
            aepZ2 = AE_ADDSP24S(aepX2, aepY2);

            *((ae_p16s *)&pshwZ[2*k]) = aepZ1;
            *((ae_p16s *)&pshwZ[2*k + 1]) = aepZ2;

        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        k = swLen - 1;

        aepY1 = *((ae_p16s *)&pshwY[k]);
        aepX1 = *((ae_p16s *)&pshwX[k]);

        aeqL1 = AE_MULFS32P16S_LL(aepY1, aepC);

        aepY1 = AE_TRUNCP24Q48(aeqL1);
        aepY1 = AE_TRUNCP16(aepY1);

        aepZ1 = AE_ADDSP24S(aepX1, aepY1);

        *((ae_p16s *)&pshwZ[k]) = aepZ1;
    }
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVcMultR
 ��������  : Z[i] = mult_r(X[i], Const)
 �������  : const Word16 *pshwX      - ����������
             const Word32  swLen      - ��������
             const Word16  shwConst   - ��������
             Word16 *pshwY      - ������������
 �������  : Word16 *pshwY      - ������
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE void CODEC_OpVcMultR(
                const Word16 *pshwX,
                const Word32  swLen,
                const Word16  shwConst,
                Word16       *pshwY)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepY1, aepX2, aepY2, aepC;
    ae_q56s aeqL1, aeqH1, aeqL2, aeqH2;

    /* shwConst-->aepC.HL */
    aepC = AE_CVTP24A16(shwConst);

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwY))
    {
        /* 2�鲢�м��� */
        for (k = 0; k < swLen>>2; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
            aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

            aeqH1  = AE_MULFS32P16S_HH(aepX1, aepC);
            aeqL1 = AE_MULFS32P16S_LL(aepX1, aepC);
            aepY1 = AE_TRUNCP24Q48X2(aeqH1, aeqL1);
            aepY1 = AE_ROUNDSP16ASYM(aepY1);

            *((ae_p16x2s *)&pshwY[4*k])     = aepY1;

            aeqH2  = AE_MULFS32P16S_HH(aepX2, aepC);
            aeqL2 = AE_MULFS32P16S_LL(aepX2, aepC);
            aepY2 = AE_TRUNCP24Q48X2(aeqH2, aeqL2);
            aepY2 = AE_ROUNDSP16ASYM(aepY2);

            *((ae_p16x2s *)&pshwY[4*k + 2]) = aepY2;
        }

        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = swLen & 0x7FFFFFFC;

            aepX1 = *((ae_p16x2s *)&pshwX[k]);

            aeqH1  = AE_MULFS32P16S_HH(aepX1, aepC);
            aeqL1 = AE_MULFS32P16S_LL(aepX1, aepC);
            aepY1 = AE_TRUNCP24Q48X2(aeqH1, aeqL1);
            aepY1 = AE_ROUNDSP16ASYM(aepY1);

            *((ae_p16x2s *)&pshwY[k]) = aepY1;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aeqL1 = AE_MULFS32P16S_LL(aepX1, aepC);
            aeqL2 = AE_MULFS32P16S_LL(aepX2, aepC);

            aepY1 = AE_ROUNDSP16Q48ASYM(aeqL1);
            aepY2 = AE_ROUNDSP16Q48ASYM(aeqL2);

            *((ae_p16s *)&pshwY[2*k]) = aepY1;
            *((ae_p16s *)&pshwY[2*k + 1]) = aepY2;
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);

        aeqL1 = AE_MULFS32P16S_LL(aepX1, aepC);
        aepY1 = AE_ROUNDSP16Q48ASYM(aeqL1);

        *((ae_p16s *)&pshwY[swLen - 1]) = aepY1;
    }
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVcMultScale
 ��������  : Y[i] = saturate(L_shr(L_mult(X[i], Const), ScaleBit))
 �������  : const Word16 *pshwX         - ��������
             const Word32  swLen         - ��������
             const Word16  shwConst      - ����
             Word16  shwScaleBit         - ����bit��
             Word16 *pshwY               - ������������
 �������  : Word16 *pshwY               - ������
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE void CODEC_OpVcMultScale(
                const Word16 *pshwX,
                const Word32  swLen,
                const Word16  shwConst,
                Word16        shwScaleBit,
                Word16       *pshwY)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepY1, aepX2, aepY2, aepC;
    ae_q56s aeqL1, aeqH1, aeqL2, aeqH2;

    /* shwConst-->aepC.HL */
    aepC = AE_CVTP24A16(shwConst);

    /* ����Ҫ���Ʊ�����д����λ�Ĵ��� */
    WUR_AE_SAR(shwScaleBit);

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwY))
    {
        /* ������ַΪ4�ֽڶ���ʱ����˫���� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[2*k]);

            aeqH1 = AE_MULFS32P16S_HH(aepX1, aepC);
            aeqL1 = AE_MULFS32P16S_LL(aepX1, aepC);

            aeqH1 = AE_SRASQ56(aeqH1);
            aeqL1 = AE_SRASQ56(aeqL1);

            aeqH1 = AE_SLLISQ56S(aeqH1, 16);
            aeqL1 = AE_SLLISQ56S(aeqL1, 16);

            aeqH1 = AE_SATQ48S(aeqH1);
            aeqL1 = AE_SATQ48S(aeqL1);

            aepY1 = AE_TRUNCP24Q48X2(aeqH1, aeqL1);

            *((ae_p16x2s *)&pshwY[2*k]) = aepY1;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aeqL1 = AE_MULFS32P16S_LL(aepX1, aepC);
            aeqL2 = AE_MULFS32P16S_LL(aepX2, aepC);

            aeqL1 = AE_SRASQ56(aeqL1);
            aeqL2 = AE_SRASQ56(aeqL2);

            aeqL1 = AE_SLLISQ56S(aeqL1, 16);
            aeqL2 = AE_SLLISQ56S(aeqL2, 16);

            aeqL1 = AE_SATQ48S(aeqL1);
            aeqL2 = AE_SATQ48S(aeqL2);

            aepY1 = AE_TRUNCP24Q48(aeqL1);
            aepY2 = AE_TRUNCP24Q48(aeqL2);

            *((ae_p16s *)&pshwY[2*k]) = aepY1;
            *((ae_p16s *)&pshwY[2*k + 1]) = aepY2;
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);

        aeqL1 = AE_MULFS32P16S_LL(aepX1, aepC);
        aeqL1 = AE_SRASQ56(aeqL1);
        aeqL1 = AE_SLLISQ56S(aeqL1, 16);
        aeqL1 = AE_SATQ48S(aeqL1);
        aepY1 = AE_TRUNCP24Q48(aeqL1);

        *((ae_p16s *)&pshwY[swLen - 1]) = aepY1;
    }
}
XT_INLINE void CODEC_OpVcSub(
        const Word16        *pshwX,
        const Word32         swLen,
        const Word16         shwY,
        Word16              *pshwZ)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepY1, aepZ1, aepX2, aepZ2;

    /* ��ʼ��aepY1��aepY2 */
    aepY1 = *((ae_p16s *)&shwY);

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwZ))
    {
        /* ������ַΪ4�ֽڶ���ʱ����˫���� */
        for (k = 0; k < swLen>>2; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
            aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

            aepZ1 = AE_SUBSP24S(aepX1, aepY1);
            aepZ2 = AE_SUBSP24S(aepX2, aepY1);

            *((ae_p16x2s *)&pshwZ[4*k]) = aepZ1;
            *((ae_p16x2s *)&pshwZ[4*k + 2]) = aepZ2;
        }

        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = swLen & 0x7FFFFFFC;

            aepX1 = *((ae_p16x2s *)&pshwX[k]);

            aepZ1 = AE_SUBSP24S(aepX1, aepY1);

            *((ae_p16x2s *)&pshwZ[k]) = aepZ1;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepZ1 = AE_SUBSP24S(aepX1, aepY1);
            aepZ2 = AE_SUBSP24S(aepX2, aepY1);

            *((ae_p16s *)&pshwZ[2*k]) = aepZ1;
            *((ae_p16s *)&pshwZ[2*k + 1]) = aepZ2;
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);

        aepZ1 = AE_SUBSP24S(aepX1, aepY1);

        *((ae_p16s *)&pshwZ[swLen - 1]) = aepZ1;
    }
}
XT_INLINE void CODEC_OpVecAbs(
                      Word16        *pshwX,
                      Word32         swLen,
                      Word16        *pshwZ)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepZ1, aepX2, aepZ2;


    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwZ))
    {
        /* ������ַΪ4�ֽڶ���ʱ����˫���� */
        for (k = 0; k < swLen>>2; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
            aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

            aepZ1 = AE_ABSSP24S(aepX1);
            aepZ2 = AE_ABSSP24S(aepX2);

            *((ae_p16x2s *)&pshwZ[4*k]) = aepZ1;
            *((ae_p16x2s *)&pshwZ[4*k + 2]) = aepZ2;
        }

        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = swLen & 0x7FFFFFFC;

            aepX1 = *((ae_p16x2s *)&pshwX[k]);

            aepZ1 = AE_ABSSP24S(aepX1);

            *((ae_p16x2s *)&pshwZ[k]) = aepZ1;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepZ1 = AE_ABSSP24S(aepX1);
            aepZ2 = AE_ABSSP24S(aepX2);

            *((ae_p16s *)&pshwZ[2*k]) = aepZ1;
            *((ae_p16s *)&pshwZ[2*k + 1]) = aepZ2;
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);

        aepZ1 = AE_ABSSP24S(aepX1);

        *((ae_p16s *)&pshwZ[swLen - 1]) = aepZ1;
    }
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVecCpy
 ��������  : Des[i] =  Src[i]���������ƣ�Src��Des�����ڴ治���ظ�
 �������  : Word16 *pshwDes          - Ŀ������
             const Word16 *pshwSrc    - Դ����
             const Word32 swLen       - ��������
 �������  : Word16 *pshwDes          - ���ƽ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE void CODEC_OpVecCpy(
                Word16       *pshwDes,
                const Word16 *pshwSrc,
                const Word32  swLen)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepX2;

    if(swLen < 0)
    {
        return;
    }

    if (CODEC_OpCheck4ByteAligned((Word32)pshwDes|(Word32)pshwSrc))
    {
        /* ������ַΪ4�ֽڶ���ʱ����˫���� */
        for (k = 0; k < swLen>>2; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwSrc[4*k]);
            aepX2 = *((ae_p16x2s *)&pshwSrc[4*k + 2]);

            *((ae_p16x2s *)&pshwDes[4*k]) = aepX1;
            *((ae_p16x2s *)&pshwDes[4*k + 2]) = aepX2;
        }

        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = swLen & 0x7FFFFFFC;

            aepX1 = *((ae_p16x2s *)&pshwSrc[k]);
            *((ae_p16x2s *)&pshwDes[k]) = aepX1;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwSrc[2*k]);
            aepX2 = *((ae_p16s *)&pshwSrc[2*k + 1]);

            *((ae_p16s *)&pshwDes[2*k]) = aepX1;
            *((ae_p16s *)&pshwDes[2*k + 1]) = aepX2;
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwSrc[swLen - 1]);

        *((ae_p16s *)&pshwDes[swLen - 1]) = aepX1;
    }
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVecAlign
 ��������  : Des[i] =  Src[i]���������룬Src��Des�����ڴ治���ظ�
 �������  : Word16 *pshwDes          - Ŀ������(4�ֽڶ���)
             const Word16 *pshwSrc    - Դ����(2�ֽڶ���)
             const Word32 swLen       - ��������
 �������  : Word16 *pshwDes          - ���ƽ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE void CODEC_OpVecAlign(
                Word16       *pshwDes,
                const Word16 *pshwSrc,
                const Word32  swLen)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepX2, aepY;

    aepX1 = *((ae_p16s *)&pshwSrc[2*k]);
    aepX2 = *((ae_p16s *)&pshwSrc[2*k + 1]);

    for (k = 0; k < swLen>>1; k++)
    {
        AE_MOVTP24X2(aepY, aepX2, 0x1);

        *((ae_p16x2s *)&pshwDes[2*k]) = aepY;

        aepX1 = *((ae_p16s *)&pshwSrc[2*k + 2]);
        aepX2 = *((ae_p16s *)&pshwSrc[2*k + 3]);
        AE_MOVTP24X2(aepY, aepX1, 0x2);
    }

    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwSrc[swLen - 1]);

        *((ae_p16s *)&pshwDes[swLen - 1]) = aepX1;
    }

}

/*****************************************************************************
 �� �� ��  : CODEC_OpVecMax
 ��������  : Ѱ������Ԫ�������һ�����ֵ
 �������  : Word16 *pshwX       - ��������
             Word32 swLen        - ��������
             Word16 *pswMaxPos   - �������ֵλ�õĵ�Ԫ
 �������  : Word16 *pswMaxPos   - ���ֵλ��
 �� �� ֵ  : ���ֵ
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���

*****************************************************************************/
XT_INLINE Word16 CODEC_OpVecMax(
                const Word16 *pshwX,
                const Word32  swLen,
                Word16       *pswMaxPos)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepX2, aepMax, aepMaxId, aepId1, aepId2;
    xtbool2 xtbHL;

    /* -32768 -> Max.H / Max.L */
    aepMax = AE_CVTP24A16(-32768);

    if (CODEC_OpCheck4ByteAligned(pshwX))
    {
        /* ������ַΪ4�ֽڶ���ʱ����˫���� */
        for (k = 0; k < swLen>>2; k++)
        {
            aepX1  = *((ae_p16x2s *)&pshwX[4*k]);
            aepX2  = *((ae_p16x2s *)&pshwX[4*k + 2]);

            aepId1 = AE_CVTP24A16X2(4*k,      4*k + 1);
            aepId2 = AE_CVTP24A16X2(4*k + 2,  4*k + 3);

            xtbHL = AE_LEP24S(aepMax, aepX1);
            AE_MOVTP24X2(aepMax,    aepX1,   xtbHL);
            AE_MOVTP24X2(aepMaxId,  aepId1,  xtbHL);

            xtbHL = AE_LEP24S(aepMax, aepX2);
            AE_MOVTP24X2(aepMax,    aepX2,   xtbHL);
            AE_MOVTP24X2(aepMaxId,  aepId2,  xtbHL);
        }

        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = swLen & 0x7FFFFFFC;

            aepX1 = *((ae_p16x2s *)&pshwX[k]);
            aepId1 = AE_CVTP24A16X2(k, k + 1);

            xtbHL = AE_LEP24S(aepMax, aepX1);
            AE_MOVTP24X2(aepMax,    aepX1,  xtbHL);
            AE_MOVTP24X2(aepMaxId,  aepId1, xtbHL);
        }

        /* ��H��L���ҳ�����ֵ */
        aepX1 = AE_SELP24_LH(aepMax, aepMax);
        aepId1 = AE_SELP24_LH(aepMaxId, aepMaxId);

        xtbHL = AE_LTP24S(aepMax, aepX1);
        AE_MOVTP24X2(aepMax, aepX1, xtbHL);
        AE_MOVTP24X2(aepMaxId, aepId1, xtbHL);

        aepId2 = AE_MAXP24S(aepMaxId, aepId1);
        xtbHL = AE_EQP24(aepMax, aepX1);
        AE_MOVTP24X2(aepMaxId, aepId2, xtbHL);
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepId1 = AE_CVTP24A16(2*k);
            aepId2 = AE_CVTP24A16(2*k + 1);

            xtbHL = AE_LEP24S(aepMax, aepX1);
            AE_MOVTP24X2(aepMax, aepX1, xtbHL);
            AE_MOVTP24X2(aepMaxId, aepId1, xtbHL);

            xtbHL = AE_LEP24S(aepMax, aepX2);
            AE_MOVTP24X2(aepMax, aepX2, xtbHL);
            AE_MOVTP24X2(aepMaxId, aepId2, xtbHL);

        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1   = *((ae_p16s *)&pshwX[swLen - 1]);
        aepId1  = AE_CVTP24A16(swLen - 1);

        xtbHL = AE_LEP24S(aepMax, aepX1);
        AE_MOVTP24X2(aepMax, aepX1, xtbHL);
        AE_MOVTP24X2(aepMaxId, aepId1, xtbHL);
    }

    if(pswMaxPos!=0)
    {
        *pswMaxPos = AE_TRUNCA16P24S_H(aepMaxId);
    }

    return AE_TRUNCA16P24S_H(aepMax);
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVecMaxAbs
 ��������  : Y= max(abs(X[i]))
 �������  : Word16 *pshwX       - ��������
             Word32 swLen        - ��������
             Word16 *pswMaxPos   - ����������ֵԪ������λ��
 �������  : Word16  *pswMaxPos  - ������ֵԪ������λ��
 �� �� ֵ  : Word16 ��������������ֵ
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���

*****************************************************************************/
XT_INLINE Word16 CODEC_OpVecMaxAbs(
                const Word16 *pshwX,
                const Word32  swLen,
                Word16       *pswMaxPos)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepX2, aepMax, aepMaxId, aepId1, aepId2;
    xtbool2 xtbHL;

    /* 0 -> Max.H / Max.L */
    aepMax = AE_CVTP24A16(0);

    if (CODEC_OpCheck4ByteAligned(pshwX))
    {
        /* ������ַΪ4�ֽڶ���ʱ����˫���� */
        for (k = 0; k < swLen>>2; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
            aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

            aepId1 = AE_CVTP24A16X2(4*k, 4*k + 1);
            aepId2 = AE_CVTP24A16X2(4*k + 2, 4*k + 3);

            aepX1 = AE_ABSSP24S(aepX1);
            aepX2 = AE_ABSSP24S(aepX2);

            xtbHL = AE_LEP24S(aepMax, aepX1);
            AE_MOVTP24X2(aepMax, aepX1, xtbHL);
            AE_MOVTP24X2(aepMaxId, aepId1, xtbHL);

            xtbHL = AE_LEP24S(aepMax, aepX2);
            AE_MOVTP24X2(aepMax, aepX2, xtbHL);
            AE_MOVTP24X2(aepMaxId, aepId2, xtbHL);
        }

        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = swLen & 0x7FFFFFFC;

            aepX1 = *((ae_p16x2s *)&pshwX[k]);
            aepId1 = AE_CVTP24A16X2(k, k + 1);

            aepX1 = AE_ABSSP24S(aepX1);

            xtbHL = AE_LEP24S(aepMax, aepX1);
            AE_MOVTP24X2(aepMax, aepX1, xtbHL);
            AE_MOVTP24X2(aepMaxId, aepId1, xtbHL);
        }

        /* ��H��L���ҳ�����ֵ */
        aepX1 = AE_SELP24_LH(aepMax, aepMax);
        aepId1 = AE_SELP24_LH(aepMaxId, aepMaxId);

        xtbHL = AE_LTP24S(aepMax, aepX1);
        AE_MOVTP24X2(aepMax, aepX1, xtbHL);
        AE_MOVTP24X2(aepMaxId, aepId1, xtbHL);

        aepId2 = AE_MAXP24S(aepMaxId, aepId1);
        xtbHL = AE_EQP24(aepMax, aepX1);
        AE_MOVTP24X2(aepMaxId, aepId2, xtbHL);

    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepId1 = AE_CVTP24A16(2*k);
            aepId2 = AE_CVTP24A16(2*k + 1);

            aepX1 = AE_ABSSP24S(aepX1);
            aepX2 = AE_ABSSP24S(aepX2);

            xtbHL = AE_LEP24S(aepMax, aepX1);
            AE_MOVTP24X2(aepMax, aepX1, xtbHL);
            AE_MOVTP24X2(aepMaxId, aepId1, xtbHL);

            xtbHL = AE_LEP24S(aepMax, aepX2);
            AE_MOVTP24X2(aepMax, aepX2, xtbHL);
            AE_MOVTP24X2(aepMaxId, aepId2, xtbHL);

        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);
        aepId1 = AE_CVTP24A16(swLen - 1);

        aepX1 = AE_ABSSP24S(aepX1);

        xtbHL = AE_LEP24S(aepMax, aepX1);
        AE_MOVTP24X2(aepMax, aepX1, xtbHL);
        AE_MOVTP24X2(aepMaxId, aepId1, xtbHL);
    }

    if (pswMaxPos!=0)
    {
        *pswMaxPos = AE_TRUNCA16P24S_H(aepMaxId);
    }

    return AE_TRUNCA16P24S_H(aepMax);
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVecMin
 ��������  : Ѱ������Ԫ����Сֵ
 �������  : Word16 *pshwX        - ��������
             Word32 swLen         - ��������
             Word16 *pswMinPos    - ������Сֵλ�õĵ�Ԫ
 �������  : Word16 *pswMinPos    - ��Сֵλ��
 �� �� ֵ  : Word16 ��Сֵ
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���

*****************************************************************************/
XT_INLINE Word16 CODEC_OpVecMin(
                const Word16 *pshwX,
                const Word32  swLen,
                Word16       *pswMinPos)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepX2, aepMin, aepMinId, aepId1, aepId2;
    xtbool2 xtbHL;

    /* 32767 -> Min.H / Min.L */
    aepMin = AE_CVTP24A16(32767);

    if (CODEC_OpCheck4ByteAligned(pshwX))
    {
        /* ������ַΪ4�ֽڶ���ʱ����˫���� */
        for (k = 0; k < swLen>>2; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
            aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

            aepId1 = AE_CVTP24A16X2(4*k, 4*k + 1);
            aepId2 = AE_CVTP24A16X2(4*k + 2, 4*k + 3);

            xtbHL = AE_LEP24S(aepX1, aepMin);
            AE_MOVTP24X2(aepMin, aepX1, xtbHL);
            AE_MOVTP24X2(aepMinId, aepId1, xtbHL);

            xtbHL = AE_LEP24S(aepX2, aepMin);
            AE_MOVTP24X2(aepMin, aepX2, xtbHL);
            AE_MOVTP24X2(aepMinId, aepId2, xtbHL);
        }

        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = swLen & 0x7FFFFFFC;

            aepX1 = *((ae_p16x2s *)&pshwX[k]);
            aepId1 = AE_CVTP24A16X2(k, k + 1);

            xtbHL = AE_LEP24S(aepX1, aepMin);
            AE_MOVTP24X2(aepMin, aepX1, xtbHL);
            AE_MOVTP24X2(aepMinId, aepId1, xtbHL);
        }

        /* ��H��L���ҳ���С��ֵ */
        aepX1 = AE_SELP24_LH(aepMin, aepMin);
        aepId1 = AE_SELP24_LH(aepMinId, aepMinId);

        xtbHL = AE_LTP24S(aepX1, aepMin);
        AE_MOVTP24X2(aepMin, aepX1, xtbHL);
        AE_MOVTP24X2(aepMinId, aepId1, xtbHL);

        aepId2 = AE_MAXP24S(aepMinId, aepId1);
        xtbHL = AE_EQP24(aepMin, aepX1);
        AE_MOVTP24X2(aepMinId, aepId2, xtbHL);
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepId1 = AE_CVTP24A16(2*k);
            aepId2 = AE_CVTP24A16(2*k + 1);

            xtbHL = AE_LEP24S(aepX1, aepMin);
            AE_MOVTP24X2(aepMin, aepX1, xtbHL);
            AE_MOVTP24X2(aepMinId, aepId1, xtbHL);

            xtbHL = AE_LEP24S(aepX2, aepMin);
            AE_MOVTP24X2(aepMin, aepX2, xtbHL);
            AE_MOVTP24X2(aepMinId, aepId2, xtbHL);

        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);
        aepId1 = AE_CVTP24A16(swLen - 1);

        xtbHL = AE_LEP24S(aepX1, aepMin);
        AE_MOVTP24X2(aepMin, aepX1, xtbHL);
        AE_MOVTP24X2(aepMinId, aepId1, xtbHL);
    }

    if (pswMinPos!=0)
    {
        *pswMinPos = AE_TRUNCA16P24S_H(aepMinId);
    }

    return AE_TRUNCA16P24S_H(aepMin);
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVecScaleMac
 ��������  : temp = shr(X[i], bits) �� Bits>=0
             sum = L_mac(sum, temp, temp)
 �������  : const Word16 *pshwX    - ����
             const Word32 swLen     - ��������
             Word32 swSum           - �ۼӺͳ�ʼֵ
             Word16 shwBits         - ��������λ��
 �������  : ��
 �� �� ֵ  : Word32  �ۼӽ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE Word32 CODEC_OpVecScaleMac(
                const Word16 *pshwX,
                const Word32  swLen,
                Word32        swSum,
                Word16        shwBits)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepX2;
    ae_q56s aeqS;

    if(swLen < 0)
    {
        return swSum;
    }

    /* ����Ҫ���Ʊ�����д����λ�Ĵ��� */
    WUR_AE_SAR(shwBits);

    /* swSum --> aeqS(17.48) */
    aeqS = AE_CVTQ48A32S(swSum);

    if (CODEC_OpCheck4ByteAligned(pshwX))
    {
        /* ������ַΪ4�ֽڶ���ʱ����˫���� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[2*k]);

            aepX1 = AE_SRASP24(aepX1);
            aepX1 = AE_TRUNCP16(aepX1);
            AE_MULAFS32P16S_HH(aeqS, aepX1, aepX1);
            AE_MULAFS32P16S_LL(aeqS, aepX1, aepX1);
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepX1 = AE_SRASP24(aepX1);
            aepX1 = AE_TRUNCP16(aepX1);
            aepX2 = AE_SRASP24(aepX2);
            aepX2 = AE_TRUNCP16(aepX2);

            AE_MULAFS32P16S_LL(aeqS, aepX1, aepX1);
            AE_MULAFS32P16S_LL(aeqS, aepX2, aepX2);
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);

        aepX1 = AE_SRASP24(aepX1);
        aepX1 = AE_TRUNCP16(aepX1);

        AE_MULAFS32P16S_LL(aeqS, aepX1, aepX1);
    }

    return AE_TRUNCA32Q48(aeqS);
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVecSet
 ��������  : ������ֵ, Des[i] = Value
 �������  : Word16 *pshwDes     - ����ֵ����
             Word32 swLen        - ��������
             Word16 pshwValue    - ��ֵĿ��ֵ
 �������  : Word16 *pshwDes     - ��ֵ���
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE void CODEC_OpVecSet(
                Word16       *pshwDes,
                const Word32  swLen,
                const Word16  shwValue)
{
    Word32 k = 0;
    ae_p24x2s aepV;

    if(swLen < 0)
    {
        return;
    }

    /* pshwValue-->aepV.HL */
    aepV = AE_CVTP24A16(shwValue);

    if (CODEC_OpCheck4ByteAligned(pshwDes))
    {
        /* ������ַΪ4�ֽڶ���ʱ����˫���� */
        for (k = 0; k < swLen>>2; k++)
        {
            *((ae_p16x2s *)&pshwDes[4*k]) = aepV;
            *((ae_p16x2s *)&pshwDes[4*k+2]) = aepV;
        }
        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = swLen & 0x7FFFFFFC;
            *((ae_p16x2s *)&pshwDes[k]) = aepV;
        }

    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            *((ae_p16s *)&pshwDes[2*k]) = aepV;
            *((ae_p16s *)&pshwDes[2*k + 1]) = aepV;
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        *((ae_p16s *)&pshwDes[swLen - 1]) = aepV;
    }
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVecShl
 ��������  : Y[i] = shl(X[i], Bits)
 �������  : const Word16 *pshwX     - ����λ����
             const Word32 swLen      - ��������
             Word16 shwBits          - ��λ��
             Word16 *pshwY           - ������������
 �������  : Word16 *pshwY           - ��λ���
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE void CODEC_OpVecShl(
                const Word16 *pshwX,
                const Word32  swLen,
                Word16        shwBits,
                Word16       *pshwY)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepY1, aepX2, aepY2;

    if(swLen < 0)
    {
        return;
    }

    /* ����λ���� */
    shwBits = shwBits > 16 ? 16 : shwBits;

    /* ����Ҫ���Ʊ�����д����λ�Ĵ��� */
    WUR_AE_SAR(shwBits);

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwY))
    {
        /* ������ַΪ4�ֽڶ���ʱ����˫���� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[2*k]);

            aepY1 = AE_SLLSSP24S(aepX1);

            *((ae_p16x2s *)&pshwY[2*k]) = aepY1;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepY1 = AE_SLLSSP24S(aepX1);
            aepY2 = AE_SLLSSP24S(aepX2);

            *((ae_p16s *)&pshwY[2*k]) = aepY1;
            *((ae_p16s *)&pshwY[2*k + 1]) = aepY2;
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);

        aepY1 = AE_SLLSSP24S(aepX1);

        *((ae_p16s *)&pshwY[swLen - 1]) = aepY1;
    }
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVecShr
 ��������  : Y[i] = shr(X[i], Bits)
 �������  : const Word16 *pshwX      - ����λ����
             const Word32 swLen       - ��������
             Word16 shwBits     - ��λ��
             Word16 *pshwY      - ������������
 �������  : Word16 *pshwY      - ��λ���
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE void CODEC_OpVecShr(
                const Word16 *pshwX,
                const Word32  swLen,
                Word16        shwBits,
                Word16       *pshwY)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepY1, aepX2, aepY2;

    if(swLen < 0)
    {
        return;
    }

    /* ����λ���� */
    shwBits = shwBits > 16 ? 16 : shwBits;

    /* ����Ҫ���Ʊ�����д����λ�Ĵ��� */
    WUR_AE_SAR(shwBits);

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwY))
    {
        /* ������ַΪ4�ֽڶ���ʱ����˫���� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[2*k]);
            aepY1 = AE_SRASP24(aepX1);
            *((ae_p16x2s *)&pshwY[2*k]) = aepY1;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepY1 = AE_SRASP24(aepX1);
            aepY2 = AE_SRASP24(aepX2);

            *((ae_p16s *)&pshwY[2*k]) = aepY1;
            *((ae_p16s *)&pshwY[2*k + 1]) = aepY2;
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);
        aepY1 = AE_SRASP24(aepX1);
        *((ae_p16s *)&pshwY[swLen - 1]) = aepY1;
    }
}
XT_INLINE void CODEC_OpVecShr_r(
        Word16 *pshwX,
        Word32  swLen,
        Word16        shwBits,
        Word16       *pshwY)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepY1, aepX2, aepY2;

    if(swLen < 0)
    {
        return;
    }

    /* ����Ҫ���Ʊ�����д����λ�Ĵ��� */
    WUR_AE_SAR(shwBits);

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwY))
    {
        /* ������ַΪ4�ֽڶ���ʱ����˫���� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[2*k]);
            aepY1 = AE_SRASP24(aepX1);

            /* ��aepY1���и�16���طǶԳ�ȡ��(0.5->1;-0.5->0) --> aepY1*/
            aepY1 = AE_ROUNDSP16ASYM(aepY1);

            *((ae_p16x2s *)&pshwY[2*k]) = aepY1;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepY1 = AE_SRASP24(aepX1);
            aepY2 = AE_SRASP24(aepX2);

            /* ��aepY1\aepY2���и�16���طǶԳ�ȡ��(0.5->1;-0.5->0) --> aepY1\aepY2 */
            aepY1 = AE_ROUNDSP16ASYM(aepY1);
            aepY2 = AE_ROUNDSP16ASYM(aepY2);

            *((ae_p16s *)&pshwY[2*k]) = aepY1;
            *((ae_p16s *)&pshwY[2*k + 1]) = aepY2;
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);
        aepY1 = AE_SRASP24(aepX1);

        /* ��aepY1���и�16���طǶԳ�ȡ��(0.5->1;-0.5->0) --> aepY1*/
        aepY1 = AE_ROUNDSP16ASYM(aepY1);

        *((ae_p16s *)&pshwY[swLen - 1]) = aepY1;
    }
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVecSum
 ��������  : ��������Ԫ���ۼӺ�
 �������  : Word16 *pshwX   - ����
             Word32 swLen    - ��������
 �������  : ��
 �� �� ֵ  : Word16 �ۼӺ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE Word16 CODEC_OpVecSum(
                const Word16 *pshwX,
                const Word32  swLen)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepS1, aepX2;

    if(swLen < 0)
    {
        return 0;
    }

    aepS1 = AE_ZEROP48();

    for (k = 0; k < swLen>>1; k++)
    {
        aepX1 = *((ae_p16s *)&pshwX[2*k]);
        aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

        aepS1 = AE_ADDSP24S(aepX1, aepS1);
        aepS1 = AE_ADDSP24S(aepX2, aepS1);
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);
        aepS1 = AE_ADDSP24S(aepX1, aepS1);
    }

    /* aepS.L-->shwSum */
    return AE_TRUNCA16P24S_H(aepS1);
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVvAdd
 ��������  : Z[i] = add(X[i], Y[i])
 �������  : Word16 *pshwX    - ����������
             Word16 *pshwY    - ��������
             Word32 swLen     - ��������
             Word16 *pshwZ    - ������������
 �������  : Word16 *pshwZ    - ������
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE void CODEC_OpVvAdd(
                const Word16 *pshwX,
                const Word16 *pshwY,
                const Word32  swLen,
                Word16       *pshwZ)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepY1, aepZ1, aepX2, aepY2, aepZ2;

    if(swLen < 0)
    {
        return;
    }

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwY|(Word32)pshwZ))
    {
        /* ������ַΪ4�ֽڶ���ʱ����˫���� */
        for (k = 0; k < swLen>>2; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
            aepY1 = *((ae_p16x2s *)&pshwY[4*k]);

            aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);
            aepY2 = *((ae_p16x2s *)&pshwY[4*k + 2]);

            aepZ1 = AE_ADDSP24S(aepX1, aepY1);
            aepZ2 = AE_ADDSP24S(aepX2, aepY2);

            *((ae_p16x2s *)&pshwZ[4*k]) = aepZ1;
            *((ae_p16x2s *)&pshwZ[4*k + 2]) = aepZ2;
        }

        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = swLen & 0x7FFFFFFC;

            aepX1 = *((ae_p16x2s *)&pshwX[k]);
            aepY1 = *((ae_p16x2s *)&pshwY[k]);

            aepZ1 = AE_ADDSP24S(aepX1, aepY1);

            *((ae_p16x2s *)&pshwZ[k]) = aepZ1;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepY1 = *((ae_p16s *)&pshwY[2*k]);
            aepY2 = *((ae_p16s *)&pshwY[2*k + 1]);

            aepZ1 = AE_ADDSP24S(aepX1, aepY1);
            aepZ2 = AE_ADDSP24S(aepX2, aepY2);

            *((ae_p16s *)&pshwZ[2*k]) = aepZ1;
            *((ae_p16s *)&pshwZ[2*k + 1]) = aepZ2;
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);
        aepY1 = *((ae_p16s *)&pshwY[swLen - 1]);

        aepZ1 = AE_ADDSP24S(aepX1, aepY1);

        *((ae_p16s *)&pshwZ[swLen - 1]) = aepZ1;
    }
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVvAdd32
 ��������  : Z[i] = L_add(X[i], Y[i])
 �������  : Word32 *pshwX    - ����������
             Word32 *pshwY    - ��������
             Word32 swLen     - ��������
             Word32 *pshwZ    - ������������
 �������  : Word32 *pshwZ    - ������
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE void CODEC_OpVvAdd32(
                const Word32 *pswX,
                const Word32 *pswY,
                const Word32  swLen,
                Word32       *pswZ)
{
    Word32 k = 0;

    ae_q56s aeqX1, aeqY1, aeqZ1, aeqX2, aeqY2, aeqZ2;

    if(swLen < 0)
    {
        return;
    }

    for (k = 0; k < swLen>>1; k++)
    {
        aeqX1 = *((ae_q32s *)&pswX[2*k]);
        aeqX2 = *((ae_q32s *)&pswX[2*k + 1]);

        aeqY1 = *((ae_q32s *)&pswY[2*k]);
        aeqY2 = *((ae_q32s *)&pswY[2*k + 1]);

        aeqZ1 = AE_ADDSQ56S(aeqX1, aeqY1);
        aeqZ2 = AE_ADDSQ56S(aeqX2, aeqY2);

        aeqZ1 = AE_SATQ48S(aeqZ1);
        aeqZ2 = AE_SATQ48S(aeqZ2);

        *((ae_q32s *)&pswZ[2*k]) = aeqZ1;
        *((ae_q32s *)&pswZ[2*k + 1]) = aeqZ2;
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aeqX1 = *((ae_q32s *)&pswX[swLen - 1]);
        aeqY1 = *((ae_q32s *)&pswY[swLen - 1]);

        aeqZ1 = AE_ADDSQ56S(aeqX1, aeqY1);
        aeqZ1 = AE_SATQ48S(aeqZ1);

        *((ae_q32s *)&pswZ[swLen - 1]) = aeqZ1;
    }
}
/*****************************************************************************
 �� �� ��  : CODEC_OpVvFormWithCoef
 ��������  : Z[i] = add(mult(X[i], a), mult(Y[i], b))
 �������  : Word16 *pshwX     - ����1
             Word16 *pshwY     - ����2
             Word32  swLen     - ��������
             Word16  shwA      - ����1�ļ�Ȩϵ��
             Word16  shwB      - ����2�ļ�Ȩϵ��
             Word16 *pshwZ     - ������������
 �������  : Word16 *pshwZ  - ������
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���

*****************************************************************************/
XT_INLINE void CODEC_OpVvFormWithCoef(
                const Word16 *pshwX,
                const Word16 *pshwY,
                const Word32  swLen,
                const Word16  shwA,
                const Word16  shwB,
                Word16 *pshwZ)
{
    Word32 k = 0;
    Word32 swLastIndex = 0;

    ae_p24x2s aepX, aepY, aepZ, aepA, aepB;

    ae_q56s aeqL, aeqH;

    if(swLen < 0)
    {
        return;
    }

    /* shwA-->aepA.HL */
    aepA = *((ae_p16s *)&shwA);

    /* shwB-->shwB.HL */
    aepB = *((ae_p16s *)&shwB);

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwY|(Word32)pshwZ))
    {
        /* ������ַΪ4�ֽڶ���ʱ����˫���� */
        for (k = 0; k < swLen>>1; k++)
        {
            /* pshwX[2*k]-->pshwX.H; pshwX[2*k+1]-->pshwX.L */
            aepX = *((ae_p16x2s *)&pshwX[2*k]);

            /* aepX.H*aepA.H*2������-->aeqH(1.48) */
            aeqH = AE_MULFS32P16S_HH(aepX, aepA);

            /* aepX.L*aepA.L*2������-->aeqL(1.48) */
            aeqL = AE_MULFS32P16S_LL(aepX, aepA);

            /* aeqH(25.48)-->aepX.H; aeqL(25.48)-->aepX.L */
            aepX = AE_TRUNCP24Q48X2(aeqH, aeqL);
            aepX = AE_TRUNCP16(aepX);

            /* pshwY[2*k]-->aepY.H; pshwY[2*k+1]-->aepY.L */
            aepY = *((ae_p16x2s *)&pshwY[2*k]);

            /* aepY.H*aepB.H*2������-->aeqH(1.48) */
            aeqH = AE_MULFS32P16S_HH(aepY, aepB);

            /* aepY.L*aepB.L*2������-->aeqL(1.48) */
            aeqL = AE_MULFS32P16S_LL(aepY, aepB);

            /* aeqH(25.48)-->aepY.H; aeqL(25.48)-->aepY.L */
            aepY = AE_TRUNCP24Q48X2(aeqH, aeqL);
            aepY = AE_TRUNCP16(aepY);

            /* aepX.L+aepY.L-->aepZ.L; aepX.H+aepY.H-->aepZ.H */
            aepZ = AE_ADDSP24S(aepX, aepY);

            /* aepZ.H-->pshwZ[2*k]; aepZ.L-->pshwZ[2*k+1]*/
            *((ae_p16x2s *)&pshwZ[2*k]) = aepZ;
        }

        /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
        if (CODEC_OpCheckBit0Set(swLen))
        {
            swLastIndex = swLen - 1;

            /* pshwX[swLen-1]-->pshwX.HL; */
            aepX = *((ae_p16s *)&pshwX[swLastIndex]);

            /* aepX.L*aepA.L*2������-->aeqL(1.48) */
            aeqL = AE_MULFS32P16S_LL(aepX, aepA);

            /* aeqL(25.48)-->aepX.L */
            aepX = AE_TRUNCP24Q48(aeqL);
            aepX = AE_TRUNCP16(aepX);

            /* pshwY[swLen-1]-->aepY.HL */
            aepY = *((ae_p16s *)&pshwY[swLastIndex]);

            /* aepY.L*aepB.L*2������-->aeqL(1.48) */
            aeqL = AE_MULFS32P16S_LL(aepY, aepB);

            /* aeqH(25.48)-->aepY.H; aeqL(25.48)-->aepY.L */
            aepY = AE_TRUNCP24Q48(aeqL);
            aepY = AE_TRUNCP16(aepY);

            /* aepX.L+aepY.L-->aepZ.L */
            aepZ = AE_ADDSP24S(aepX, aepY);

            /* aepZ.L-->pshwZ[swLen-1] */
            *((ae_p16s *)&pshwZ[swLastIndex]) = aepZ;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen; k++)
        {
            /* pshwX[k]-->pshwX.HL; */
            aepX = *((ae_p16s *)&pshwX[k]);

            /* aepX.L*aepA.L*2������-->aeqL(1.48) */
            aeqL = AE_MULFS32P16S_LL(aepX, aepA);

            /* aeqL(25.48)-->aepX.L */
            aepX = AE_TRUNCP24Q48(aeqL);
            aepX = AE_TRUNCP16(aepX);

            /* pshwY[k]-->aepY.HL */
            aepY = *((ae_p16s *)&pshwY[k]);

            /* aepY.L*aepB.L*2������-->aeqL(1.48) */
            aeqL = AE_MULFS32P16S_LL(aepY, aepB);

            /* aeqH(25.48)-->aepY.H; aeqL(25.48)-->aepY.L */
            aepY = AE_TRUNCP24Q48(aeqL);
            aepY = AE_TRUNCP16(aepY);

            /* aepX.L+aepY.L-->aepZ.L */
            aepZ = AE_ADDSP24S(aepX, aepY);

            /* aepZ.L-->pshwZ[k] */
            *((ae_p16s *)&pshwZ[k]) = aepZ;
        }
    }
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVvFormWithDimi
 ��������  : Z[i] = add(X[i]>>1, Y[i]>>1)
 �������  : Word16 *pshwX    - ����1
             Word16 *pshwY    - ����2
             Word32  swLen    - ��������
             Word16 *pshwZ    - ������������
 �������  : Word16 *pshwZ    - ������
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���

*****************************************************************************/
XT_INLINE void CODEC_OpVvFormWithDimi(
                Word16 *pshwX,
                Word16 *pshwY,
                Word32  swLen,
                Word16 *pshwZ)
{
    Word32 k = 0;
    Word32 swLastIndex = 0;

    ae_p24x2s aepX, aepY, aepZ;

    ae_q56s aeqL, aeqH;

    if(swLen < 0)
    {
        return;
    }

    /* ����Ҫ���Ʊ�����д����λ�Ĵ��� */
    WUR_AE_SAR(1);

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwY|(Word32)pshwZ))
    {
        /* ������ַΪ4�ֽڶ���ʱ����˫���� */
        for (k = 0; k < swLen>>1; k++)
        {
            /* pshwX[2*k]-->pshwX.H; pshwX[2*k+1]-->pshwX.L */
            aepX = *((ae_p16x2s *)&pshwX[2*k]);

            /* aepX.HL�������� --> aepX.HL */
            aepX = AE_SRASP24(aepX);
            aepX = AE_TRUNCP16(aepX);

            /* pshwY[2*k]-->aepY.H; pshwY[2*k+1]-->aepY.L */
            aepY = *((ae_p16x2s *)&pshwY[2*k]);

            /* aepY.HL�������� --> aepY.HL */
            aepY = AE_SRASP24(aepY);
            aepY = AE_TRUNCP16(aepY);

            /* aepX.L+aepY.L-->aepZ.L; aepX.H+aepY.H-->aepZ.H */
            aepZ = AE_ADDSP24S(aepX, aepY);

            /* aepZ.H-->pshwZ[2*k]; aepZ.L-->pshwZ[2*k+1]*/
            *((ae_p16x2s *)&pshwZ[2*k]) = aepZ;
        }

        /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
        if (CODEC_OpCheckBit0Set(swLen))
        {
            swLastIndex = swLen - 1;

            /* pshwX[swLen-1]-->pshwX.HL; */
            aepX = *((ae_p16s *)&pshwX[swLastIndex]);

            /* aepX.HL�������� --> aepX.HL */
            aepX = AE_SRASP24(aepX);
            aepX = AE_TRUNCP16(aepX);

            /* pshwY[swLen-1]-->aepY.HL */
            aepY = *((ae_p16s *)&pshwY[swLastIndex]);

            /* aepY.HL�������� --> aepY.HL */
            aepY = AE_SRASP24(aepY);
            aepY = AE_TRUNCP16(aepY);

            /* aepX.L+aepY.L-->aepZ.L */
            aepZ = AE_ADDSP24S(aepX, aepY);

            /* aepZ.L-->pshwZ[swLen-1] */
            *((ae_p16s *)&pshwZ[swLastIndex]) = aepZ;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen; k++)
        {
            /* pshwX[k]-->pshwX.HL; */
            aepX = *((ae_p16s *)&pshwX[k]);

            /* aepX.HL�������� --> aepX.HL */
            aepX = AE_SRASP24(aepX);
            aepX = AE_TRUNCP16(aepX);

            /* pshwY[k]-->aepY.HL */
            aepY = *((ae_p16s *)&pshwY[k]);

            /* aepY.HL�������� --> aepY.HL */
            aepY = AE_SRASP24(aepY);
            aepY = AE_TRUNCP16(aepY);

            /* aepX.L+aepY.L-->aepZ.L */
            aepZ = AE_ADDSP24S(aepX, aepY);

            /* aepZ.L-->pshwZ[k] */
            *((ae_p16s *)&pshwZ[k]) = aepZ;
        }
    }
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVvFormWithQuar
 ��������  : Z[i] = add(X[i]>>2, sub(Y[i],Y[i]>>2))
 �������  : Word16 *pshwX   - ��Ȩϵ��Ϊ0.25������
             Word16 *pshwY   - ��Ȩϵ��Ϊ0.75������
             Word32  swLen   - ��������
             Word16 *pshwZ   - ������������
 �������  : Word16 *pshwZ   - ������
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���

*****************************************************************************/
XT_INLINE void CODEC_OpVvFormWithQuar(
                Word16 *pshwX,
                Word16 *pshwY,
                Word32  swLen,
                Word16 *pshwZ)
{
    Word32 k = 0;
    Word32 swLastIndex = 0;

    ae_p24x2s aepX, aepY, aepT, aepZ;

    ae_q56s aeqL, aeqH;

    if(swLen < 0)
    {
        return;
    }

    /* ����Ҫ���Ʊ�����д����λ�Ĵ��� */
    WUR_AE_SAR(2);
    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwY|(Word32)pshwZ))
    {
        /* ������ַΪ4�ֽڶ���ʱ����˫���� */
        for (k = 0; k < swLen>>1; k++)
        {
            /* pshwX[2*k]-->pshwX.H; pshwX[2*k+1]-->pshwX.L */
            aepX = *((ae_p16x2s *)&pshwX[2*k]);

            /* aepX.HL�������� --> aepX.HL */
            aepX = AE_SRASP24(aepX);
            aepX = AE_TRUNCP16(aepX);

            /* pshwY[2*k]-->aepY.H; pshwY[2*k+1]-->aepY.L */
            aepY = *((ae_p16x2s *)&pshwY[2*k]);

            /* aepY.HL�������� --> aepT.HL */
            aepT = AE_SRASP24(aepY);
            aepT = AE_TRUNCP16(aepT);

            /* aepY.L-aepT.L-->aepY.L; aepY.H-aepT.H-->aepY.H */
            aepY = AE_SUBSP24S(aepY, aepT);

            /* aepX.L+aepY.L-->aepZ.L; aepX.H+aepY.H-->aepZ.H */
            aepZ = AE_ADDSP24S(aepX, aepY);

            /* aepZ.H-->pshwZ[2*k]; aepZ.L-->pshwZ[2*k+1]*/
            *((ae_p16x2s *)&pshwZ[2*k]) = aepZ;
        }

        /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
        if (CODEC_OpCheckBit0Set(swLen))
        {
            swLastIndex = swLen - 1;

            /* pshwX[swLen-1]-->pshwX.HL; */
            aepX = *((ae_p16s *)&pshwX[swLastIndex]);

            /* aepX.HL�������� --> aepX.HL */
            aepX = AE_SRASP24(aepX);
            aepX = AE_TRUNCP16(aepX);

            /* pshwY[swLen-1]-->aepY.HL */
            aepY = *((ae_p16s *)&pshwY[swLastIndex]);

            /* aepY.HL�������� --> aepT.HL */
            aepT = AE_SRASP24(aepY);
            aepT = AE_TRUNCP16(aepT);

            /* aepY.L-aepT.L-->aepY.L; */
            aepY = AE_SUBSP24S(aepY, aepT);

            /* aepX.L+aepY.L-->aepZ.L */
            aepZ = AE_ADDSP24S(aepX, aepY);

            /* aepZ.L-->pshwZ[swLen-1] */
            *((ae_p16s *)&pshwZ[swLastIndex]) = aepZ;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen; k++)
        {
            /* pshwX[k]-->pshwX.HL; */
            aepX = *((ae_p16s *)&pshwX[k]);

            /* aepX.HL�������� --> aepX.HL */
            aepX = AE_SRASP24(aepX);
            aepX = AE_TRUNCP16(aepX);

            /* pshwY[k]-->aepY.HL */
            aepY = *((ae_p16s *)&pshwY[k]);

            /* aepY.HL�������� --> aepT.HL */
            aepT = AE_SRASP24(aepY);
            aepT = AE_TRUNCP16(aepT);

            /* aepY.L-aepT.L-->aepY.L; */
            aepY = AE_SUBSP24S(aepY, aepT);

            /* aepX.L+aepY.L-->aepZ.L */
            aepZ = AE_ADDSP24S(aepX, aepY);

            /* aepZ.L-->pshwZ[k] */
            *((ae_p16s *)&pshwZ[k]) = aepZ;
        }
    }
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVvMac
 ��������  : Y = sum(X[i]*Y[i])+sum
 �������  : Word16 *pshwX    - ��������X
             Word16 *pshwY    - ��������Y
             Word32 swLen     - ��������
             Word32 swSum     - �ۼӳ�ʼֵ
 �������  : ��
 �� �� ֵ  : Word32 �ۼӺ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/

XT_INLINE Word32 CODEC_OpVvMac(
                const Word16 *pshwX,
                const Word16 *pshwY,
                const Word32  swLen,
                Word32        swSum)
{
    Word32 k;
    ae_p24x2s aepX1, aepX2, aepY1, aepY2;
    ae_q56s aeqS1;

    /* swSum-->aeqS(17.48) */
    aeqS1 = AE_CVTQ48A32S(swSum);

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwY))
    {
        /* 2�鲢�м���,����nopָ�� */
        for (k = 0; k < swLen>>2; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
            aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

            aepY1 = *((ae_p16x2s *)&pshwY[4*k]);
            aepY2 = *((ae_p16x2s *)&pshwY[4*k + 2]);

            AE_MULAAFP24S_HH_LL(aeqS1, aepX1, aepY1);
            AE_MULAAFP24S_HH_LL(aeqS1, aepX2, aepY2);

            /* ���͵�48bit */
            aeqS1 = AE_SATQ48S(aeqS1);
        }

        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = (swLen & 0x7FFFFFFC);

            aepX1 = *((ae_p16x2s *)&pshwX[k]);
            aepY1 = *((ae_p16x2s *)&pshwY[k]);

            AE_MULAAFP24S_HH_LL(aeqS1, aepX1, aepY1);
        }

        /* ���͵�48bit */
        aeqS1 = AE_SATQ48S(aeqS1);

    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepY1 = *((ae_p16s *)&pshwY[2*k]);
            aepY2 = *((ae_p16s *)&pshwY[2*k + 1]);

            AE_MULAFS32P16S_LL(aeqS1, aepX1, aepY1);
            AE_MULAFS32P16S_LL(aeqS1, aepX2, aepY2);
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);
        aepY1 = *((ae_p16s *)&pshwY[swLen - 1]);

        AE_MULAFS32P16S_LL(aeqS1, aepX1, aepY1);
    }

    return AE_TRUNCA32Q48(aeqS1);
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVvMacAlignless
 ��������  : Y = sum(X[i]*Y[i])+sum
 �������  : Word16 *pshwX    - ��������X
             Word16 *pshwY    - ��������Y
             Word32 swLen     - ��������
             Word32 swSum     - �ۼӳ�ʼֵ
 �������  : ��
 �� �� ֵ  : Word32 �ۼӺ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/

XT_INLINE Word32 CODEC_OpVvMacAlignless(
                const Word16 *pshwX,
                const Word16 *pshwY,
                const Word32  swLen,
                Word32        swSum)
{
    Word32 k;
    ae_p24x2s aepX1, aepX2, aepY1, aepY2;
    ae_q56s aeqS1;

    /* swSum-->aeqS(17.48) */
    aeqS1 = AE_CVTQ48A32S(swSum);

    /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
    for (k = 0; k < swLen>>1; k++)
    {
        aepX1 = *((ae_p16s *)&pshwX[2*k]);
        aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

        aepY1 = *((ae_p16s *)&pshwY[2*k]);
        aepY2 = *((ae_p16s *)&pshwY[2*k + 1]);

        AE_MULAFS32P16S_LL(aeqS1, aepX1, aepY1);
        AE_MULAFS32P16S_LL(aeqS1, aepX2, aepY2);
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);
        aepY1 = *((ae_p16s *)&pshwY[swLen - 1]);

        AE_MULAFS32P16S_LL(aeqS1, aepX1, aepY1);
    }

    return AE_TRUNCA32Q48(aeqS1);
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVvMacAligned
 ��������  : Y = sum(X[i]*Y[i])+sum
 �������  : Word16 *pshwX    - ��������X(4�ֽڶ���)
             Word16 *pshwY    - ��������Y(4�ֽڶ���)
             Word32 swLen     - ��������
             Word32 swSum     - �ۼӳ�ʼֵ
 �������  : ��
 �� �� ֵ  : Word32 �ۼӺ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE Word32 CODEC_OpVvMacAligned(
                const Word16 *pshwX,
                const Word16 *pshwY,
                const Word32  swLen,
                Word32        swSum)
{
    Word32 k;
    ae_p24x2s aepX1, aepX2, aepY1, aepY2;
    ae_q56s aeqS1;

    /* swSum-->aeqS(17.48) */
    aeqS1 = AE_CVTQ48A32S(swSum);

    /* 2�鲢�м���,����nopָ�� */
    for (k = 0; k < swLen>>2; k++)
    {
        aepX1 = *((ae_p16x2s *)&pshwX[4*k]);

        aepY1 = *((ae_p16x2s *)&pshwY[4*k]);
        aeqS1 = AE_SATQ48S(aeqS1);

        aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);
        aepY2 = *((ae_p16x2s *)&pshwY[4*k + 2]);

        AE_MULAAFP24S_HH_LL(aeqS1, aepX1, aepY1);
        AE_MULAAFP24S_HH_LL(aeqS1, aepX2, aepY2);

    }
    aeqS1 = AE_SATQ48S(aeqS1);

    /* 1�鲢�м��� */
    if (CODEC_OpCheckBit1Set(swLen))
    {
        k = (swLen & 0x7FFFFFFC);

        aepX1 = *((ae_p16x2s *)&pshwX[k]);
        aepY1 = *((ae_p16x2s *)&pshwY[k]);

        AE_MULAAFP24S_HH_LL(aeqS1, aepX1, aepY1);
    }

    /* ���͵�48bit */
    aeqS1 = AE_SATQ48S(aeqS1);

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);
        aepY1 = *((ae_p16s *)&pshwY[swLen - 1]);

        AE_MULAFS32P16S_LL(aeqS1, aepX1, aepY1);
    }

    return AE_TRUNCA32Q48(aeqS1);
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVvMacAlignedEven
 ��������  : Y = sum(X[i]*Y[i])+sum
 �������  : Word16 *pshwX    - ��������X(4�ֽڶ���)
             Word16 *pshwY    - ��������Y(4�ֽڶ���)
             Word32 swLen     - ��������(ż��)
             Word32 swSum     - �ۼӳ�ʼֵ
 �������  : ��
 �� �� ֵ  : Word32 �ۼӺ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/

XT_INLINE Word32 CODEC_OpVvMacAlignedEven(
                const Word16 *pshwX,
                const Word16 *pshwY,
                const Word32  swLen,
                Word32        swSum)
{
    Word32 k;
    ae_p24x2s aepX1, aepX2, aepY1, aepY2;
    ae_q56s aeqS1;

    /* swSum-->aeqS(17.48) */
    aeqS1 = AE_CVTQ48A32S(swSum);

    /* 2�鲢�м���,����nopָ�� */
    for (k = 0; k < swLen>>2; k++)
    {
        aepX1 = *((ae_p16x2s *)&pshwX[4*k]);

        aepY1 = *((ae_p16x2s *)&pshwY[4*k]);
        aeqS1 = AE_SATQ48S(aeqS1);

        aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);
        aepY2 = *((ae_p16x2s *)&pshwY[4*k + 2]);

        AE_MULAAFP24S_HH_LL(aeqS1, aepX1, aepY1);
        AE_MULAAFP24S_HH_LL(aeqS1, aepX2, aepY2);
    }
    aeqS1 = AE_SATQ48S(aeqS1);

    /* 1�鲢�м��� */
    if (CODEC_OpCheckBit1Set(swLen))
    {
        k = (swLen & 0x7FFFFFFC);

        aepX1 = *((ae_p16x2s *)&pshwX[k]);
        aepY1 = *((ae_p16x2s *)&pshwY[k]);

        AE_MULAAFP24S_HH_LL(aeqS1, aepX1, aepY1);
    }

    /* ���͵�48bit */
    aeqS1 = AE_SATQ48S(aeqS1);

    return AE_TRUNCA32Q48(aeqS1);
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVvMacAlignedQuan
 ��������  : Y = sum(X[i]*Y[i])+sum
 �������  : Word16 *pshwX    - ��������X(4�ֽڶ���)
             Word16 *pshwY    - ��������Y(4�ֽڶ���)
             Word32 swLen     - ��������(4��������)
             Word32 swSum     - �ۼӳ�ʼֵ
 �������  : ��
 �� �� ֵ  : Word32 �ۼӺ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/

XT_INLINE Word32 CODEC_OpVvMacAlignedQuan(
                const Word16 *pshwX,
                const Word16 *pshwY,
                const Word32  swLen,
                Word32        swSum)
{
    Word32 k;
    ae_p24x2s aepX1, aepX2, aepY1, aepY2;
    ae_q56s aeqS1;

    aepX1 = *((ae_p16x2s *)pshwX);
    aepY1 = *((ae_p16x2s *)pshwY);

    /* swSum-->aeqS(17.48) */
    aeqS1 = AE_CVTQ48A32S(swSum);

    /* 2�鲢�м���,����nopָ�� */
    for (k = 0; k < swLen>>2; k++)
    {
        aeqS1 = AE_SATQ48S(aeqS1);
        aepX2 = *((ae_p16x2s *)&pshwX[4*k+2]);
        aepY2 = *((ae_p16x2s *)&pshwY[4*k+2]);

        AE_MULAAFP24S_HH_LL(aeqS1, aepX1, aepY1);
        aepX1 = *((ae_p16x2s *)&pshwX[4*k+4]);
        aepY1 = *((ae_p16x2s *)&pshwY[4*k+4]);

        AE_MULAAFP24S_HH_LL(aeqS1, aepX2, aepY2);
    }
    aeqS1 = AE_SATQ48S(aeqS1);

    return AE_TRUNCA32Q48(aeqS1);
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVvSelfMacAlignedQuan
 ��������  : Y = sum(X[i]*X[i])+sum
 �������  : Word16 *pshwX    - ��������X(4�ֽڶ���)
             Word32 swLen     - ��������(4��������)
             Word32 swSum     - �ۼӳ�ʼֵ
 �������  : ��
 �� �� ֵ  : Word32 �ۼӺ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/

XT_INLINE Word32 CODEC_OpVvSelfMacAlignedQuan(
                const Word16 *pshwX,
                const Word32  swLen,
                Word32        swSum)
{
    Word32 k;
    ae_p24x2s aepX1, aepX2;
    ae_q56s aeqS1;

    aepX1 = *((ae_p16x2s *)pshwX);

    /* swSum-->aeqS(17.48) */
    aeqS1 = AE_CVTQ48A32S(swSum);

    /* 2�鲢�м���,����nopָ�� */
    for (k = 0; k < swLen>>2; k++)
    {
        aeqS1 = AE_SATQ48S(aeqS1);
        aepX2 = *((ae_p16x2s *)&pshwX[4*k+2]);

        AE_MULAAFP24S_HH_LL(aeqS1, aepX1, aepX1);
        aepX1 = *((ae_p16x2s *)&pshwX[4*k+4]);

        AE_MULAAFP24S_HH_LL(aeqS1, aepX2, aepX2);
    }

    aeqS1 = AE_SATQ48S(aeqS1);

    return AE_TRUNCA32Q48(aeqS1);
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVvMacD
 ��������  : Y = sum(X[i]*Y[-i])+sum
 �������  : Word16 *pshwX    - ��������X
             Word16 *pshwY    - ��������Y
             Word32 swLen     - ��������
             Word32 swSum     - �ۼӳ�ʼֵ
 �������  : ��
 �� �� ֵ  : Word32 �ۼӺ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : �½�����

*****************************************************************************/
XT_INLINE Word32 CODEC_OpVvMacD(
                const Word16 *pshwX,
                const Word16 *pshwY,
                const Word32  swLen,
                Word32        swSum)
{
    Word32 k;
    ae_p24x2s aepX1, aepX2, aepY1, aepY2;
    ae_q56s aeqS1;

    /* swSum-->aeqS(17.48) */
    aeqS1 = AE_CVTQ48A32S(swSum);

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)(pshwY-1)))
    {
        /* 2�鲢�м���,����nopָ�� */
        for (k = 0; k < swLen>>2; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
            aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

            aepY1 = *((ae_p16x2s *)&pshwY[- 4*k - 1]);
            aepY2 = *((ae_p16x2s *)&pshwY[- 4*k - 3]);

            AE_MULAAFP24S_HL_LH(aeqS1, aepX1, aepY1);
            AE_MULAAFP24S_HL_LH(aeqS1, aepX2, aepY2);
        }

        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = (swLen & 0x7FFFFFFC);

            aepX1 = *((ae_p16x2s *)&pshwX[k]);
            aepY1 = *((ae_p16x2s *)&pshwY[- k - 1]);

            AE_MULAAFP24S_HL_LH(aeqS1, aepX1, aepY1);
        }

        /* ���͵�48bit */
        aeqS1 = AE_SATQ48S(aeqS1);

    }
    else
    {

        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepY1 = *((ae_p16s *)&pshwY[- 2*k]);
            aepY2 = *((ae_p16s *)&pshwY[- 2*k - 1]);

            AE_MULAFS32P16S_LL(aeqS1, aepX1, aepY1);
            AE_MULAFS32P16S_LL(aeqS1, aepX2, aepY2);
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);
        aepY1 = *((ae_p16s *)&pshwY[1 - swLen]);

        AE_MULAFS32P16S_LL(aeqS1, aepX1, aepY1);
    }

    return AE_TRUNCA32Q48(aeqS1);
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVvMsu
 ��������  : Y = sum - sum(X[i]*Y[i])
 �������  : const Word16 *pshwX    - 16bit����������
             const Word16 *pshwY    - 16bit����������
             Word32 swLen           - ������������
             Word32 swSum           - �ۼ���ʼ��ֵ
 �������  : ��
 �� �� ֵ  : Word32 ����������ۼӳ�ʼ��ֵ����Ľ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE Word32 CODEC_OpVvMsu(
                const Word16 *pshwX,
                const Word16 *pshwY,
                const Word32  swLen,
                Word32        swSum)
{
    Word32 k, swPos, swPartLen;
    ae_p24x2s aepX1, aepX2, aepY1, aepY2;
    ae_q56s aeqS1, aeqS2;

    if(swLen < 0)
    {
        return swSum;
    }

    /* swSum-->aeqS(17.48) */
    aeqS1 = AE_CVTQ48A32S(swSum);

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwY))
    {
        /* ��¼��ǰ���ۼ������ݳ��� */
        swPos = 0;

        do
        {
            /* ÿ�μ��㳤�Ȳ��ܳ���CODEC_OP_MULFP24S_MAX_LEN��,������������� */
            swPartLen = (swLen - swPos) > CODEC_OP_MULFP24S_MAX_LEN ? CODEC_OP_MULFP24S_MAX_LEN : (swLen - swPos);

            aeqS2 = AE_ZEROQ56();

            /* 2�鲢�м���,����nopָ�� */
            for (k = 0; k < swPartLen>>2; k++)
            {
                aepX1 = *((ae_p16x2s *)&pshwX[swPos + 4*k]);
                aepX2 = *((ae_p16x2s *)&pshwX[swPos + 4*k + 2]);

                aepY1 = *((ae_p16x2s *)&pshwY[swPos + 4*k]);
                aepY2 = *((ae_p16x2s *)&pshwY[swPos + 4*k + 2]);

                AE_MULSSFP24S_HH_LL(aeqS2, aepX1, aepY1);
                AE_MULSSFP24S_HH_LL(aeqS2, aepX2, aepY2);
            }

            /* 1�鲢�м��� */
            if (CODEC_OpCheckBit1Set(swPartLen))
            {
                k = swPos + (swPartLen & 0x7FFFFFFC);

                aepX1 = *((ae_p16x2s *)&pshwX[k]);
                aepY1 = *((ae_p16x2s *)&pshwY[k]);

                AE_MULSSFP24S_HH_LL(aeqS2, aepX1, aepY1);
            }

            /* ���µ�ǰλ�� */
            swPos += swPartLen;

            /* ���б������� */
            aeqS1 = AE_ADDSQ56S(aeqS1, aeqS2);

        } while (swPos < swLen);

        /* ���͵�48bit */
        aeqS1 = AE_SATQ48S(aeqS1);

    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepY1 = *((ae_p16s *)&pshwY[2*k]);
            aepY2 = *((ae_p16s *)&pshwY[2*k + 1]);

            AE_MULSFS32P16S_LL(aeqS1, aepX1, aepY1);
            AE_MULSFS32P16S_LL(aeqS1, aepX2, aepY2);
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);
        aepY1 = *((ae_p16s *)&pshwY[swLen - 1]);

        AE_MULSFS32P16S_LL(aeqS1, aepX1, aepY1);
    }

    return AE_TRUNCA32Q48(aeqS1);
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVvMsuD
 ��������  : Y = sum - sum(X[i]*Y[-i])
 �������  : Word16 *pshwX    - ��������X
             Word16 *pshwY    - ��������Y
             Word32 swLen     - ��������
             Word32 swSum     - �ۼӳ�ʼֵ
 �������  : ��
 �� �� ֵ  : Word32 �ۼӺ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE Word32 CODEC_OpVvMsuD(
                       const Word16 *pshwX,
                       const Word16 *pshwY,
                       const Word32  swLen,
                       Word32        swSum)
{
    Word32 k;
    ae_p24x2s aepX1, aepX2, aepY1, aepY2;
    ae_q56s aeqS1;

    /* swSum-->aeqS(17.48) */
    aeqS1 = AE_CVTQ48A32S(swSum);

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX | (Word32)(pshwY-1)))
    {
        /* 2�鲢�м���,����nopָ�� */
        for (k = 0; k < swLen>>2; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
            aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

            aepY1 = *((ae_p16x2s *)&pshwY[- 4*k - 1]);
            aepY2 = *((ae_p16x2s *)&pshwY[- 4*k - 3]);

            AE_MULSSFP24S_HL_LH(aeqS1, aepX1, aepY1);
            AE_MULSSFP24S_HL_LH(aeqS1, aepX2, aepY2);
        }

        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = (swLen & 0x7FFFFFFC);

            aepX1 = *((ae_p16x2s *)&pshwX[k]);
            aepY1 = *((ae_p16x2s *)&pshwY[- k - 1]);

            AE_MULSSFP24S_HL_LH(aeqS1, aepX1, aepY1);
        }

        /* ���͵�48bit */
        aeqS1 = AE_SATQ48S(aeqS1);
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepY1 = *((ae_p16s *)&pshwY[-2*k]);
            aepY2 = *((ae_p16s *)&pshwY[-2*k - 1]);

            AE_MULSFS32P16S_LL(aeqS1, aepX1, aepY1);
            AE_MULSFS32P16S_LL(aeqS1, aepX2, aepY2);
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);
        aepY1 = *((ae_p16s *)&pshwY[1 - swLen]);

        AE_MULSFS32P16S_LL(aeqS1, aepX1, aepY1);
    }

    return AE_TRUNCA32Q48(aeqS1);
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVvMsuDAlignedEven
 ��������  : Y = sum - sum(X[i]*Y[-i])
 �������  : Word16 *pshwX    - ��������X(4�ֽڶ���)
             Word16 *pshwY    - ��������Y(Y[-1]4�ֽڶ���)
             Word32 swLen     - ��������(ż��)
             Word32 swSum     - �ۼӳ�ʼֵ
 �������  : ��
 �� �� ֵ  : Word32 �ۼӺ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE Word32 CODEC_OpVvMsuDAlignedEven(
                       const Word16 *pshwX,
                       const Word16 *pshwY,
                       const Word32  swLen,
                       Word32        swSum)
{
    Word32 k;
    ae_p24x2s aepX1, aepX2, aepY1, aepY2;
    ae_q56s aeqS1;

    /* swSum-->aeqS(17.48) */
    aeqS1 = AE_CVTQ48A32S(swSum);

     /* 2�鲢�м���,����nopָ�� */
    for (k = 0; k < swLen>>2; k++)
    {
        aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
        aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

        aepY1 = *((ae_p16x2s *)&pshwY[- 4*k - 1]);
        aepY2 = *((ae_p16x2s *)&pshwY[- 4*k - 3]);

        AE_MULSSFP24S_HL_LH(aeqS1, aepX1, aepY1);
        AE_MULSSFP24S_HL_LH(aeqS1, aepX2, aepY2);
     }

     /* 1�鲢�м��� */
     if (CODEC_OpCheckBit1Set(swLen))
     {
        k = (swLen & 0x7FFFFFFC);

        aepX1 = *((ae_p16x2s *)&pshwX[k]);
        aepY1 = *((ae_p16x2s *)&pshwY[- k - 1]);

        AE_MULSSFP24S_HL_LH(aeqS1, aepX1, aepY1);
     }

     /* ���͵�48bit */
     aeqS1 = AE_SATQ48S(aeqS1);

    return AE_TRUNCA32Q48(aeqS1);
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVvMsuDAlignedQuan
 ��������  : Y = sum - sum(X[i]*Y[-i])
 �������  : Word16 *pshwX    - ��������X(4�ֽڶ���)
             Word16 *pshwY    - ��������Y(Y[-1]4�ֽڶ���)
             Word32 swLen     - ��������(4�ı���)
             Word32 swSum     - �ۼӳ�ʼֵ
 �������  : ��
 �� �� ֵ  : Word32 �ۼӺ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE Word32 CODEC_OpVvMsuDAlignedQuan(
                       const Word16 *pshwX,
                       const Word16 *pshwY,
                       const Word32  swLen,
                       Word32        swSum)
{
    Word32 k;
    ae_p24x2s aepX1, aepX2, aepY1, aepY2;
    ae_q56s aeqS1;

    /* swSum-->aeqS(17.48) */
    aeqS1 = AE_CVTQ48A32S(swSum);

     /* 2�鲢�м���,����nopָ�� */
    for (k = 0; k < swLen>>2; k++)
    {
        aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
        aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

        aepY1 = *((ae_p16x2s *)&pshwY[- 4*k - 1]);
        aepY2 = *((ae_p16x2s *)&pshwY[- 4*k - 3]);

        AE_MULSSFP24S_HL_LH(aeqS1, aepX1, aepY1);
        AE_MULSSFP24S_HL_LH(aeqS1, aepX2, aepY2);
     }

     /* ���͵�48bit */
     aeqS1 = AE_SATQ48S(aeqS1);

    return AE_TRUNCA32Q48(aeqS1);
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVvMsuDAlignedless
 ��������  : Y = sum - sum(X[i]*Y[-i])
 �������  : Word16 *pshwX    - ��������X
             Word16 *pshwY    - ��������Y
             Word32 swLen     - ��������
             Word32 swSum     - �ۼӳ�ʼֵ
 �������  : ��
 �� �� ֵ  : Word32 �ۼӺ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE Word32 CODEC_OpVvMsuDAlignedless(
                       const Word16 *pshwX,
                       const Word16 *pshwY,
                       const Word32  swLen,
                       Word32        swSum)
{
    Word32 k;
    ae_p24x2s aepX1, aepX2, aepY1, aepY2;
    ae_q56s aeqS1;

    /* swSum-->aeqS(17.48) */
    aeqS1 = AE_CVTQ48A32S(swSum);

    /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
    for (k = 0; k < swLen>>1; k++)
    {
        aepX1 = *((ae_p16s *)&pshwX[2*k]);
        aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

        aepY1 = *((ae_p16s *)&pshwY[-2*k]);
        aepY2 = *((ae_p16s *)&pshwY[-2*k - 1]);

        AE_MULSFS32P16S_LL(aeqS1, aepX1, aepY1);
        AE_MULSFS32P16S_LL(aeqS1, aepX2, aepY2);
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);
        aepY1 = *((ae_p16s *)&pshwY[1 - swLen]);

        AE_MULSFS32P16S_LL(aeqS1, aepX1, aepY1);
    }

    return AE_TRUNCA32Q48(aeqS1);
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVvMsuDSemiAlignedQuan
 ��������  : Y = sum - sum(X[i]*Y[-i])
 �������  : Word16 *pshwX    - ��������X(4�ֽڶ���)
             Word16 *pshwY    - ��������Y(Y[-1]4�ֽڲ�����)
             Word32 swLen     - ��������(4�ı���)
             Word32 swSum     - �ۼӳ�ʼֵ
 �������  : ��
 �� �� ֵ  : Word32 �ۼӺ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE Word32 CODEC_OpVvMsuDSemiAlignedQuan(
                       const Word16 *pshwX,
                       const Word16 *pshwY,
                       const Word32  swLen,
                       Word32        swSum)
{
    Word32 k;
    ae_p24x2s aepX1, aepX2, aepY1, aepY2, aepY3, aepY4;
    ae_q56s aeqS1;

    /* swSum-->aeqS(17.48) */
    aeqS1 = AE_CVTQ48A32S(swSum);

     /* 2�鲢�м���,����nopָ�� */
    for (k = 0; k < swLen>>2; k++)
    {
        aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
        aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

        aepY1 = *((ae_p16s *)&pshwY[- 4*k]);
        aepY2 = *((ae_p16s *)&pshwY[- 4*k - 1]);
        aepY3 = *((ae_p16s *)&pshwY[- 4*k - 2]);
        aepY4 = *((ae_p16s *)&pshwY[- 4*k - 3]);

        AE_MULSFS32P16S_HL(aeqS1, aepX1, aepY1);
        AE_MULSFS32P16S_LL(aeqS1, aepX1, aepY2);
        AE_MULSFS32P16S_HL(aeqS1, aepX2, aepY3);
        AE_MULSFS32P16S_LL(aeqS1, aepX2, aepY4);
     }

    return AE_TRUNCA32Q48(aeqS1);
}


XT_INLINE void CODEC_OpVvMult(
                const Word16 *pshwX,
                const Word16 *pshwY,
                const Word32  swLen,
                Word16       *pshwZ)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepY1, aepZ1, aepX2, aepY2, aepZ2;
    ae_q56s aeqL1, aeqH1, aeqL2, aeqH2;

    if(swLen < 0)
    {
        return;
    }

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwY|(Word32)pshwZ))
    {
        for (k = 0; k < swLen>>2; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
            aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

            aepY1 = *((ae_p16x2s *)&pshwY[4*k]);
            aepY2 = *((ae_p16x2s *)&pshwY[4*k + 2]);

            aeqH1 = AE_MULFS32P16S_HH(aepX1, aepY1);
            aeqH2 = AE_MULFS32P16S_HH(aepX2, aepY2);
            aeqL1 = AE_MULFS32P16S_LL(aepX1, aepY1);
            aeqL2 = AE_MULFS32P16S_LL(aepX2, aepY2);
            aepZ1 = AE_TRUNCP24Q48X2(aeqH1, aeqL1);
            aepZ2 = AE_TRUNCP24Q48X2(aeqH2, aeqL2);

            *((ae_p16x2s *)&pshwZ[4*k]) = aepZ1;
            *((ae_p16x2s *)&pshwZ[4*k + 2]) = aepZ2;
        }

        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = swLen & 0x7FFFFFFC;

            aepX1 = *((ae_p16x2s *)&pshwX[k]);
            aepY1 = *((ae_p16x2s *)&pshwY[k]);

            aeqH1 = AE_MULFS32P16S_HH(aepX1, aepY1);
            aeqL1 = AE_MULFS32P16S_LL(aepX1, aepY1);
            aepZ1 = AE_TRUNCP24Q48X2(aeqH1, aeqL1);

            *((ae_p16x2s *)&pshwZ[k]) = aepZ1;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepY1 = *((ae_p16s *)&pshwY[2*k]);
            aepY2 = *((ae_p16s *)&pshwY[2*k + 1]);

            aeqL1 = AE_MULFS32P16S_LL(aepX1, aepY1);
            aeqL2 = AE_MULFS32P16S_LL(aepX2, aepY2);
            aepZ1 = AE_TRUNCP24Q48(aeqL1);
            aepZ2 = AE_TRUNCP24Q48(aeqL2);

            *((ae_p16s *)&pshwZ[2*k]) = aepZ1;
            *((ae_p16s *)&pshwZ[2*k + 1]) = aepZ2;
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);
        aepY1 = *((ae_p16s *)&pshwY[swLen - 1]);

        aeqL1 = AE_MULFS32P16S_LL(aepX1, aepY1);
        aepZ1 = AE_TRUNCP24Q48(aeqL1);

        *((ae_p16s *)&pshwZ[swLen - 1]) = aepZ1;
    }
}
XT_INLINE void CODEC_OpVvMultR(
                const Word16 *pshwX,
                const Word16 *pshwY,
                const Word32  swLen,
                Word16       *pshwZ)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepY1, aepZ1, aepX2, aepY2, aepZ2;
    ae_q56s aeqL1, aeqH1, aeqL2, aeqH2;

    if(swLen < 0)
    {
        return;
    }

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwY|(Word32)pshwZ))
    {
        for (k = 0; k < swLen>>2; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
            aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

            aepY1 = *((ae_p16x2s *)&pshwY[4*k]);
            aepY2 = *((ae_p16x2s *)&pshwY[4*k + 2]);

            aeqH1 = AE_MULFS32P16S_HH(aepX1, aepY1);
            aeqH2 = AE_MULFS32P16S_HH(aepX2, aepY2);
            aeqL1 = AE_MULFS32P16S_LL(aepX1, aepY1);
            aeqL2 = AE_MULFS32P16S_LL(aepX2, aepY2);

            aepZ1 = AE_TRUNCP24Q48X2(aeqH1, aeqL1);
            aepZ2 = AE_TRUNCP24Q48X2(aeqH2, aeqL2);

            aepZ1 = AE_ROUNDSP16ASYM(aepZ1);
            aepZ2 = AE_ROUNDSP16ASYM(aepZ2);

            *((ae_p16x2s *)&pshwZ[4*k]) = aepZ1;
            *((ae_p16x2s *)&pshwZ[4*k + 2]) = aepZ2;
        }

        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = swLen & 0x7FFFFFFC;

            aepX1 = *((ae_p16x2s *)&pshwX[k]);
            aepY1 = *((ae_p16x2s *)&pshwY[k]);

            aeqH1 = AE_MULFS32P16S_HH(aepX1, aepY1);
            aeqL1 = AE_MULFS32P16S_LL(aepX1, aepY1);

            aepZ1 = AE_TRUNCP24Q48X2(aeqH1, aeqL1);

            aepZ1 = AE_ROUNDSP16ASYM(aepZ1);

            *((ae_p16x2s *)&pshwZ[k]) = aepZ1;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepY1 = *((ae_p16s *)&pshwY[2*k]);
            aepY2 = *((ae_p16s *)&pshwY[2*k + 1]);

            aeqL1 = AE_MULFS32P16S_LL(aepX1, aepY1);
            aeqL2 = AE_MULFS32P16S_LL(aepX2, aepY2);
            aepZ1 = AE_TRUNCP24Q48(aeqL1);
            aepZ2 = AE_TRUNCP24Q48(aeqL2);

            aepZ1 = AE_ROUNDSP16ASYM(aepZ1);
            aepZ2 = AE_ROUNDSP16ASYM(aepZ2);

            *((ae_p16s *)&pshwZ[2*k]) = aepZ1;
            *((ae_p16s *)&pshwZ[2*k + 1]) = aepZ2;
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);
        aepY1 = *((ae_p16s *)&pshwY[swLen - 1]);

        aeqL1 = AE_MULFS32P16S_LL(aepX1, aepY1);
        aepZ1 = AE_TRUNCP24Q48(aeqL1);

        aepZ1 = AE_ROUNDSP16ASYM(aepZ1);

        *((ae_p16s *)&pshwZ[swLen - 1]) = aepZ1;
    }
}
XT_INLINE void CODEC_OpVvMultRD(
        Word16 *pshwX,
        Word16 *pshwY,
        Word32  swLen,
        Word16       *pshwZ)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepY1, aepZ1, aepX2, aepY2, aepZ2;
    ae_q56s aeqL1, aeqH1, aeqL2, aeqH2;

    if(swLen < 0)
    {
        return;
    }

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)(pshwY-1)|(Word32)pshwZ))
    {
        for (k = 0; k < swLen>>2; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
            aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

            aepY1 = *((ae_p16x2s *)&pshwY[- 4*k - 1]);
            aepY2 = *((ae_p16x2s *)&pshwY[- 4*k - 3]);

            aeqH1 = AE_MULFS32P16S_HL(aepX1, aepY1);
            aeqH2 = AE_MULFS32P16S_HL(aepX2, aepY2);
            aeqL1 = AE_MULFS32P16S_LH(aepX1, aepY1);
            aeqL2 = AE_MULFS32P16S_LH(aepX2, aepY2);

            aepZ1 = AE_TRUNCP24Q48X2(aeqH1, aeqL1);
            aepZ2 = AE_TRUNCP24Q48X2(aeqH2, aeqL2);

            aepZ1 = AE_ROUNDSP16ASYM(aepZ1);
            aepZ2 = AE_ROUNDSP16ASYM(aepZ2);

            *((ae_p16x2s *)&pshwZ[4*k]) = aepZ1;
            *((ae_p16x2s *)&pshwZ[4*k + 2]) = aepZ2;
        }

        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = swLen & 0x7FFFFFFC;

            aepX1 = *((ae_p16x2s *)&pshwX[k]);
            aepY1 = *((ae_p16x2s *)&pshwY[- k - 1]);

            aeqH1 = AE_MULFS32P16S_HH(aepX1, aepY1);
            aeqL1 = AE_MULFS32P16S_LL(aepX1, aepY1);

            aepZ1 = AE_TRUNCP24Q48X2(aeqH1, aeqL1);

            aepZ1 = AE_ROUNDSP16ASYM(aepZ1);

            *((ae_p16x2s *)&pshwZ[k]) = aepZ1;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepY1 = *((ae_p16s *)&pshwY[- 2*k]);
            aepY2 = *((ae_p16s *)&pshwY[- 2*k - 1]);

            aeqL1 = AE_MULFS32P16S_LL(aepX1, aepY1);
            aeqL2 = AE_MULFS32P16S_LL(aepX2, aepY2);
            aepZ1 = AE_TRUNCP24Q48(aeqL1);
            aepZ2 = AE_TRUNCP24Q48(aeqL2);

            aepZ1 = AE_ROUNDSP16ASYM(aepZ1);
            aepZ2 = AE_ROUNDSP16ASYM(aepZ2);

            *((ae_p16s *)&pshwZ[2*k]) = aepZ1;
            *((ae_p16s *)&pshwZ[2*k + 1]) = aepZ2;
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);
        aepY1 = *((ae_p16s *)&pshwY[1 - swLen]);

        aeqL1 = AE_MULFS32P16S_LL(aepX1, aepY1);
        aepZ1 = AE_TRUNCP24Q48(aeqL1);

        aepZ1 = AE_ROUNDSP16ASYM(aepZ1);

        *((ae_p16s *)&pshwZ[swLen - 1]) = aepZ1;
    }
}

/*****************************************************************************
 �� �� ��  : CODEC_OpVvSub
 ��������  : Z[i] = sub(X[i], Y[i])
 �������  : Word16 *pshwX    - ����������
             Word16 *pshwY    - ��������
             Word32 swLen     - ��������
             Word16 *pshwZ    - ������������
 �������  : Word16 *pshwZ    - ������
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��8��20��
    ��    ��   : ��
    �޸�����   : �����ɺ���
  2.��    ��   : 2011��9��1��
    ��    ��   : ����
    �޸�����   : ��������Ч��

*****************************************************************************/
XT_INLINE void CODEC_OpVvSub(
                const Word16 *pshwX,
                const Word16 *pshwY,
                const Word32  swLen,
                Word16       *pshwZ)
{
    Word32 k = 0;
    ae_p24x2s aepX1, aepY1, aepZ1, aepX2, aepY2, aepZ2;

    if(swLen < 0)
    {
        return;
    }

    if (CODEC_OpCheck4ByteAligned((Word32)pshwX|(Word32)pshwY|(Word32)pshwZ))
    {
        /* ������ַΪ4�ֽڶ���ʱ����˫���� */
        for (k = 0; k < swLen>>2; k++)
        {
            aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
            aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

            aepY1 = *((ae_p16x2s *)&pshwY[4*k]);
            aepY2 = *((ae_p16x2s *)&pshwY[4*k + 2]);

            aepZ1 = AE_SUBSP24S(aepX1, aepY1);
            aepZ2 = AE_SUBSP24S(aepX2, aepY2);

            *((ae_p16x2s *)&pshwZ[4*k]) = aepZ1;
            *((ae_p16x2s *)&pshwZ[4*k + 2]) = aepZ2;
        }

        /* 1�鲢�м��� */
        if (CODEC_OpCheckBit1Set(swLen))
        {
            k = swLen & 0x7FFFFFFC;

            aepX1 = *((ae_p16x2s *)&pshwX[k]);
            aepY1 = *((ae_p16x2s *)&pshwY[k]);

            aepZ1 = AE_SUBSP24S(aepX1, aepY1);

            *((ae_p16x2s *)&pshwZ[k]) = aepZ1;
        }
    }
    else
    {
        /* ������ַ��Ϊ4�ֽڶ���ʱ���õ����� */
        for (k = 0; k < swLen>>1; k++)
        {
            aepX1 = *((ae_p16s *)&pshwX[2*k]);
            aepX2 = *((ae_p16s *)&pshwX[2*k + 1]);

            aepY1 = *((ae_p16s *)&pshwY[2*k]);
            aepY2 = *((ae_p16s *)&pshwY[2*k + 1]);

            aepZ1 = AE_SUBSP24S(aepX1, aepY1);
            aepZ2 = AE_SUBSP24S(aepX2, aepY2);

            *((ae_p16s *)&pshwZ[2*k]) = aepZ1;
            *((ae_p16s *)&pshwZ[2*k + 1]) = aepZ2;
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aepX1 = *((ae_p16s *)&pshwX[swLen - 1]);
        aepY1 = *((ae_p16s *)&pshwY[swLen - 1]);

        aepZ1 = AE_SUBSP24S(aepX1, aepY1);

        *((ae_p16s *)&pshwZ[swLen - 1]) = aepZ1;
    }
}



XT_INLINE Word16 CODEC_OpVecMeanAbsAlignedQuan(Word16 *pshwX, Word32 swLen)
{
    Word32          swSumVal = 0;                                               /* Q0 */
    Word32          swCnt;
    Word16          shwAverEng;

    Word32 k = 0;
    ae_p24x2s aepX1, aepX2, aep1;
    ae_q56s   aeqSum, aeqX1, aeqX2, aeqX3, aeqX4;

    aeqSum = AE_ZEROQ56();
    aep1   = AE_CVTP24A16(1);

    for (k = 0; k < swLen>>2; k++)
    {
        aepX1 = *((ae_p16x2s *)&pshwX[4*k]);
        aepX2 = *((ae_p16x2s *)&pshwX[4*k + 2]);

        aepX1 = AE_ABSSP24S(aepX1);
        aepX2 = AE_ABSSP24S(aepX2);

        AE_MULAAP24S_HH_LL(aeqSum, aepX1, aep1);
        AE_MULAAP24S_HH_LL(aeqSum, aepX2, aep1);

        aeqSum = AE_SATQ48S(aeqSum);
    }

    swSumVal = AE_TRUNCA32Q48(aeqSum);

    /* ��ƽ��ֵ */
    shwAverEng = saturate(swSumVal / swLen);

    return shwAverEng;
}


XT_INLINE void CODEC_OpVecShr_r32(
                Word32        *pswX,
                Word32         swLen,
                Word16         shwBits,
                Word32        *pswY)
{
    Word32 k = 0;

    ae_q56s aeqX1, aeqX2;

    if(shwBits > 0)
    {
        /* ����λ���� */
        shwBits = shwBits > 32 ? 32 : shwBits;

        for (k = 0; k < swLen>>1; k++)
        {
            aeqX1 = *((ae_q32s *)&pswX[2*k]);
            aeqX2 = *((ae_q32s *)&pswX[2*k + 1]);

            aeqX1 = AE_SRAAQ56(aeqX1, shwBits);
            aeqX1 = AE_ROUNDSQ32ASYM(aeqX1);

            aeqX2 = AE_SRAAQ56(aeqX2, shwBits);
            aeqX2 = AE_ROUNDSQ32ASYM(aeqX2);

            *((ae_q32s *)&pswY[2*k])        = aeqX1;
            *((ae_q32s *)&pswY[2*k + 1])    = aeqX2;
        }
    }
    else
    {
        /* ����λ���� */
        shwBits = shwBits < -32 ? -32 : shwBits;

        for (k = 0; k < swLen>>1; k++)
        {
            aeqX1 = *((ae_q32s *)&pswX[2*k]);
            aeqX2 = *((ae_q32s *)&pswX[2*k + 1]);

            aeqX1 = AE_SLLASQ56S(aeqX1, -shwBits);
            aeqX1 = AE_SATQ48S(aeqX1);

            aeqX2 = AE_SLLASQ56S(aeqX2, -shwBits);
            aeqX2 = AE_SATQ48S(aeqX2);

            *((ae_q32s *)&pswY[2*k])        = aeqX1;
            *((ae_q32s *)&pswY[2*k + 1])    = aeqX2;
        }
    }

    /* ��������Ϊ����ʱ�������һ��Ԫ�ص����� */
    if (CODEC_OpCheckBit0Set(swLen))
    {
        aeqX1 = *((ae_q32s *)&pswX[swLen - 1]);

        if(shwBits > 0)
        {
            aeqX1 = AE_SRAAQ56(aeqX1, shwBits);
            aeqX1 = AE_ROUNDSQ32ASYM(aeqX1);
        }
        else
        {
            aeqX1 = AE_SLLASQ56S(aeqX1, -shwBits);
            aeqX1 = AE_SATQ48S(aeqX1);
        }

        *((ae_q32s *)&pswY[swLen - 1]) = aeqX1;
    }
}

#endif /* ifndef _MED_C89_ */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of med_com_vec_hifi.h */
