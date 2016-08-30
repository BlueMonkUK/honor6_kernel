/*************************************************************************
*   ��Ȩ����(C) 1987-2020, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  efuse.c
*
*   ��    �� :  wuzechun
*
*   ��    �� :  Efuse��ȡ
*
*   �޸ļ�¼ :  2011��6��9��  v1.00  wuzechun  ����
*
*************************************************************************/

#include "sys.h"
#include "OnChipRom.h"
#include "efuse.h"

/*********************************************************************************************
Function:           TestBitValue
Description:        ���ԼĴ����е�ĳλ���Ƿ����ָ����ֵ
Calls:              ��
Data Accessed:      ��
Data Updated:       ��
Input:              addr��  �Ĵ�����ַ
                    bitMask����Ҫ����λ��
                    bitValue��ָ����ֵ
Output:             ��
Return:             �ɹ��򷵻�TRUE�� ʧ���򷵻�FALSE
Others:             ��
*********************************************************************************************/
BOOL TestBitValue(UINT32 addr, UINT32 bitMask, UINT32 bitValue)
{
    UINT32 time;                       /*��ʱʱ��*/

    for(time=0; time<TIMEMAXDELAY; time++)
    {
        if(bitValue==(INREG32(addr)&bitMask))
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*****************************************************************************
* �� �� ��  : efuseRead
*
* ��������  : �����ȡEFUSE�е�����
*
* �������  : group  ��ʼgroup
*                   num  ���鳤��(word��,������512/4=128)
* �������  : pBuf ��EFUSE�е�����
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
int efuseRead(UINT32 group, UINT32* pBuf, UINT32 num)
{
    UINT32* pSh;
    UINT32 cnt;

    if((group+num > EFUSE_MAX_SIZE/WORD_SIZE)||(NULL == pBuf))
    {
        return ERROR;
    }
    pSh = pBuf;

    /* ����EFUSE�ź�ΪAPB�ź� */
    SETREG32(EFUSEC_CFG, EFUSEC_APB);

    /* ѭ����ȡEfuseֵ */
    for(cnt=group; cnt<num; cnt++)
    {
        /* ���ö�ȡ��ַ */
        OUTREG32(EFUSE_GROUP, group+cnt);

        /* ʹ�ܶ� */
        SETREG32(EFUSEC_CFG, EFUSEC_RD_EN);
        /* �ȴ���ʹ�����óɹ�����ʹ�ܳ�ʱ���ش��� */
        if(!TestBitValue(EFUSEC_CFG, EFUSEC_RD_EN, EFUSEC_RD_EN))
        {
            return ERROR;
        }
        /* �ȴ������ */
        if(!TestBitValue(EFUSEC_STATUS, EFUSEC_RD_STATUS, EFUSEC_RD_STATUS))
        {
            return ERROR;
        }

        /* ��ȡ���� */
        *pSh = INREG32(EFUSEC_DATA);
        pSh++;

    }

    return OK;
}

#if PLATFORM==PLATFORM_PORTING
int efuseWrite(UINT32 group, UINT32* pBuf, UINT32 num)
{
    int i;

    SETREG32(EFUSEC_CFG, EFUSEC_APB);
    SETBITVALUE32(EFUSEC_COUNT, EFUSE_COUNT_MASK, EFUSE_COUNT_CFG);
    SETBITVALUE32(EFUSEC_PGM_COUNT, EFUSE_PGM_COUNT_MASK, PGM_COUNT_CFG);
    SETREG32(EFUSEC_CFG, EFUSEC_PRE_PG);
    if (!TestBitValue(EFUSEC_STATUS, EFUSEC_PRE_PG_FIN, EFUSEC_PRE_PG_FIN))
    {
        return ERROR;
    }
    for (i = 0; i < num; i++)
    {
        SETBITVALUE32(EFUSE_GROUP, EFUSE_GP_MASK, group + i);
        OUTREG32(EFUSE_PG_VALUE, *(pBuf + i));
        SETREG32(EFUSEC_CFG, EFUSEC_PG_EN);
        if (!TestBitValue(EFUSEC_STATUS, EFUSEC_PG_STATUS, EFUSEC_PG_STATUS))
        {
            return ERROR;
        }
    }
    CLRREG32(EFUSEC_CFG, EFUSEC_PRE_PG);

    return OK;
}

#endif
/*****************************************************************************
* �� �� ��  : efuseWriteCheck
*
* ��������  : ����EFUSE�Ƿ��Ѿ���д
*
* �������  : pBuf   ��ȡ����EFUSE�е�����
*                   num  ���鳤��(word��,������512/4=128)
* �������  :
*
* �� �� ֵ  : EFUSE_NOT_WRITED EFUSEδ����д
*                EFUSE_HAD_WRITED EFUSE�Ѿ���д
*
* ����˵��  :
*
*****************************************************************************/
int efuseWriteCheck(UINT32* pBuf, UINT32 num)
{
    int i;
    /*��ȡEFUSE�е�ֵ*/
    for(i = 0; i < (int)num; i++)
    {
        if(0 != pBuf[i])
        {
            return EFUSE_HAD_WRITED;
        }
    }
    return EFUSE_NOT_WRITED;
}


