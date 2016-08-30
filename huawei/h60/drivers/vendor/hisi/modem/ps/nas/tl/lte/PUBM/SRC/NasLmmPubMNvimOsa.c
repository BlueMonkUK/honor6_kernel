


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include    "NasLmmPubMNvimOsa.h"
#include    "NasLmmPubMPrint.h"
#include    "LNasNvInterface.h"
#include    "NasEmmPubUMain.h"
#include    "NasLmmPubMOm.h"
#include    "NasLmmPubMEntry.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASMMPUBMNVIMOSA_C
/*lint +e767*/


/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/

/*****************************************************************************
  3 Function
*****************************************************************************/


/*lint -e960*/
/*lint -e961*/
VOS_UINT32  NAS_LMM_DecodeOneNvItem(NAS_NVIM_CTRL_TBL *pstNvCtrlTbl)
{

    if(NAS_NVIM_NULL_PTR == pstNvCtrlTbl->pNvData)
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeOneNvItem: WARNING->pstNvCtrlTbl is Null");
        return NAS_LMM_NVIM_FAIL;
    }

    /*Data len after decode*/
    pstNvCtrlTbl->usDataLen = pstNvCtrlTbl->usNvDataLen;

    /*Data after decode*/
    NAS_LMM_MEM_CPY(                   pstNvCtrlTbl->pData,
                                        pstNvCtrlTbl->pNvData,
                                        pstNvCtrlTbl->usNvDataLen);
    return NAS_LMM_NVIM_OK;
}


/*****************************************************************************
 Function Name  : NAS_LMM_DecodeNvRaCap
 Discription    : ����RaCapability
 Input          : pstNvCtrlTbl:��������ṹ
 Output         : ����������
 Return         :
 History:
*****************************************************************************/
VOS_UINT32  NAS_LMM_DecodeNvRaCap(NAS_NVIM_CTRL_TBL *pstNvCtrlTbl)
{

    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeNvRaCap is entered");

    return  NAS_LMM_DecodeOneNvItem(pstNvCtrlTbl);
}

/*****************************************************************************
 Function Name  : NAS_EMM_DecodeNvSupCodecs
 Discription    : ����Supported Codecs
 Input          : pstNvCtrlTbl:��������ṹ
 Output         : ����������
 Return         :
 History:
*****************************************************************************/
VOS_UINT32  NAS_LMM_DecodeNvSupCodecs(NAS_NVIM_CTRL_TBL *pstNvCtrlTbl)
{

    NAS_LMM_NVIM_LOG_INFO("NAS_EMM_DecodeNvSupCodecs is entered");

    return  NAS_LMM_DecodeOneNvItem(pstNvCtrlTbl);
}


VOS_UINT32  NAS_LMM_DecodeNvHplmnACT(  NAS_NVIM_CTRL_TBL *pstNvCtrlTbl )
{

    return  NAS_LMM_DecodeOneNvItem(pstNvCtrlTbl);
}



VOS_UINT32  NAS_LMM_DecodeSimLRplmnSI(NAS_NVIM_CTRL_TBL *pstNvCtrlTbl )
{
    #if 0
    NAS_LMM_LRPLMNSI_ENUM_UINT8         *penRplmnSI;
    VOS_UINT8                           *pucMsg;

    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeSimLRplmnSI is entered");

    pucMsg                           = (VOS_UINT8 *)pstNvCtrlTbl->pNvData;

    penRplmnSI = (NAS_LMM_LRPLMNSI_ENUM_UINT8 *)NAS_EMMC_GetMmcGlobleAddr(NAS_LMM_GLOBLE_PARA_LRPLMNSI);

    if(NAS_NVIM_NULL_PTR == pstNvCtrlTbl)
    {
        /* ��ӡ��ѯ�� ���� */
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimLRplmnSI: Point is Null");
        return NAS_LMM_NVIM_FAIL;
    }

    if(NAS_EMM_USIM_FEATURE_HARD == NAS_LMM_GetUsimFeature())
    {
        if (pstNvCtrlTbl->usNvDataLen == NAS_LMM_LRPLMNSI_LEN)
        {
            NAS_LMM_MEM_CPY(             penRplmnSI,
                                        pucMsg,
                                        pstNvCtrlTbl->usNvDataLen);

            return NAS_LMM_NVIM_OK;
        }
        else
        {
            NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimLRplmnSI: len is err!");

            return NAS_LMM_NVIM_FAIL;
        }
    }
    else
    #endif
    {
         return  NAS_LMM_DecodeOneNvItem(pstNvCtrlTbl);
    }

}



/*****************************************************************************
 Function Name  : NAS_LMM_DecodeNvMNCLen
 Discription    : ����MNC LEN
 Input          : pstNvCtrlTbl:��������ṹ
 Output         : ����������
 Return         :
 History:
*****************************************************************************/
VOS_UINT32  NAS_LMM_DecodeNvMNCLen(NAS_NVIM_CTRL_TBL *pstNvCtrlTbl)
{
    #if 0
    VOS_UINT8                           *pucMsg;
    pucMsg                              = (VOS_VOID*)pstNvCtrlTbl->pNvData;

    if(NAS_EMM_USIM_FEATURE_HARD        == NAS_LMM_GetUsimFeature())
    {
        if ((NAS_LMM_NVIM_MNCLEN_FILE_MIN_LEN > pstNvCtrlTbl->usNvDataLen)
           || (NAS_LMM_NVIM_MNCLEN_VALUE_3 < pucMsg[3])
           || (NAS_LMM_NVIM_MNCLEN_VALUE_2 > pucMsg[3]))
        {
            NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeNvMNCLen:MNC Decode Err: ");

            return  NAS_LMM_NVIM_FAIL;
        }

        /*g_ucNasMmcMncLen                = pucMsg[3];*/

       /* NAS_LMM_NVIM_LOG1_INFO("NAS_LMM_DecodeNvMNCLen:MNC LEN IS: ",g_ucNasMmcMncLen);*/

        return  NAS_LMM_NVIM_OK;
    }
    else
    #endif
    {
        return  NAS_LMM_DecodeOneNvItem(pstNvCtrlTbl);
    }
}
/*****************************************************************************
 Function Name  : NAS_LMM_DecodeNvFplmnList
 Discription    : ����Forbidden PLMN List
 Input          : pstNvCtrlTbl:��������ṹ
 Output         : ����������
 Return         :
 History:
*****************************************************************************/
VOS_UINT32 NAS_LMM_DecodeNvFplmnList(NAS_NVIM_CTRL_TBL *pstNvCtrlTbl)
{
    #if 0
    VOS_UINT32                          i = 0;
    VOS_UINT32                          ulValidPlmnNum = NAS_LMM_NULL;
    VOS_UINT8                           *pucMsg;
    NAS_EMMC_PLMN_LIST_STRU              *pstFplmn;
    VOS_UINT32                          *pulMaxNumOfFplmnByUsim = VOS_NULL_PTR;
    NAS_EMMC_PLMN_ID_STRU                stPlmnId;

    if(NAS_EMM_USIM_FEATURE_HARD        == NAS_LMM_GetUsimFeature())
    {
        NAS_LMM_MEM_SET(&stPlmnId,0,sizeof(NAS_MM_PLMN_ID_STRU));
        pstFplmn                        = (VOS_VOID*)NAS_EMMC_GetMmcGlobleAddr(NAS_LMM_GLOBLE_PARA_FPLMNLIST);
        pucMsg                          = (VOS_UINT8*)pstNvCtrlTbl->pNvData;


        /* ��ȡ���USIM�е� FPLMN�б��� PLMN������ ȫ�ֱ����ĵ�ַ */
        pulMaxNumOfFplmnByUsim = (VOS_UINT32 *)NAS_EMMC_GetMmcGlobleAddr(NAS_LMM_GLOBLE_PARA_MAX_NUM_OF_FPLMN_BY_USIM);

        /* ����USIM �е� FPLMN�б��� PLMN�ĸ���*/
        *pulMaxNumOfFplmnByUsim = pstNvCtrlTbl->usNvDataLen / NAS_LMM_NVIM_PLMN_ID_LEN;

        /* �� USIM �е� FPLMN�б��� PLMN�ĸ��� ����NAS�ڴ���������ֵ���򽫴�
        ��������ΪNAS�ڴ����е����ֵ*/
        if (NAS_MM_MAX_PLMN_NUM < (*pulMaxNumOfFplmnByUsim))
        {
            (*pulMaxNumOfFplmnByUsim) = NAS_MM_MAX_PLMN_NUM;
        }

        for(i = 0; i < (*pulMaxNumOfFplmnByUsim); i++)
        {
            /* ˳�򿽱�PLMN ID�������ж��Ƿ���Ч */
            NAS_LMM_MEM_CPY(             &stPlmnId,
                                        &pucMsg[NAS_LMM_NVIM_PLMN_ID_LEN*i],
                                        NAS_LMM_NVIM_PLMN_ID_LEN);

            /*���PLMN ID��Ч����ֵ��ȫ�ֱ���*/
            if (VOS_FALSE == NAS_LMM_PlmnIsUndef(&stPlmnId))
            {
                NAS_LMM_MEM_CPY(&(pstFplmn->astPlmnId[ulValidPlmnNum]),
                               &pucMsg[NAS_LMM_NVIM_PLMN_ID_LEN*i],
                               NAS_LMM_NVIM_PLMN_ID_LEN);
                ulValidPlmnNum ++;
            }

        }

        /* ��ֵȫ�ֱ���: ����ЧFPLMN ID���� */
        pstFplmn->ulPlmnNum             = ulValidPlmnNum;

        NAS_LMM_NVIM_LOG1_INFO("NAS_LMM_DecodeNvFplmnList: ulPlmnNum = ",
                                            pstFplmn->ulPlmnNum);
        NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeNvFplmnList: PLMN IS ");
        NAS_COMM_PrintArray(NAS_COMM_GET_MM_PRINT_BUF(), (VOS_UINT8*)pstFplmn->astPlmnId,(pstFplmn->ulPlmnNum) * 4);

        return  NAS_LMM_NVIM_OK;
    }
    else
    #endif
    {
        return  NAS_LMM_DecodeOneNvItem(pstNvCtrlTbl);
    }

}
VOS_UINT32  NAS_LMM_DecodeNvUplmn(NAS_NVIM_CTRL_TBL *pstNvCtrlTbl)
{
    #if 0
    NAS_LMM_PLMN_ACT_LIST_STRU           *pstUplmnAct;
    VOS_UINT8                           *pucMsg;
    VOS_UINT32                          i;
    VOS_UINT32                          ulValidPlmnNum = NAS_LMM_NULL;
    NAS_MM_PLMN_ID_STRU                 stPlmnId;

    if(NAS_EMM_USIM_FEATURE_HARD        == NAS_LMM_GetUsimFeature())
    {
        NAS_LMM_MEM_SET(&stPlmnId,0,sizeof(NAS_MM_PLMN_ID_STRU));

        pucMsg                              = (VOS_UINT8 *)pstNvCtrlTbl->pNvData;

        pstUplmnAct = (VOS_VOID *)NAS_EMMC_GetMmcGlobleAddr(NAS_LMM_GLOBLE_PARA_UPLMNLIST);

        if((NAS_NVIM_NULL_PTR == pstUplmnAct)
            || (NAS_NVIM_NULL_PTR == pstNvCtrlTbl->pNvData))
        {
            /* ��ӡ��ѯ�� ���� */
            NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeNvUplmn: Point is Null");
            return NAS_LMM_NVIM_FAIL;
        }

        if ((NAS_USIM_MIN_PLMN_LEN > pstNvCtrlTbl->usNvDataLen)
            || (NAS_LMM_NULL != pstNvCtrlTbl->usNvDataLen % NAS_LMM_PLMN_AND_ACT_LEN))
        {
             /* ��ӡ��ѯ�� ���� */
            NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeNvUplmn:len is err!");
            /*return NAS_LMM_NVIM_FAIL;*/
        }

        pstUplmnAct->ulPlmnNum = pstNvCtrlTbl->usNvDataLen / NAS_LMM_PLMN_AND_ACT_LEN;

        for(i= 0; i < pstUplmnAct->ulPlmnNum; i++ )
        {
            NAS_LMM_MEM_CPY(             &stPlmnId,
                                        &pucMsg[NAS_LMM_PLMN_AND_ACT_LEN*i],
                                        NAS_LMM_NVIM_PLMN_ID_LEN);

            if (VOS_FALSE == NAS_LMM_PlmnIsUndef(&stPlmnId))
            {
                NAS_LMM_MEM_CPY(&(pstUplmnAct->aucPlmnId[NAS_LMM_PLMN_AND_ACT_LEN*ulValidPlmnNum]),
                               &pucMsg[NAS_LMM_PLMN_AND_ACT_LEN*i],
                               NAS_LMM_PLMN_AND_ACT_LEN);
                ulValidPlmnNum ++;
            }

            if (NAS_EMMC_MAX_UPLMN_NUM == ulValidPlmnNum)
            {
                NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeNvUplmn: Valid PlmnNum is overflow");
                break;
            }
        }

        pstUplmnAct->ulPlmnNum = ulValidPlmnNum;

        NAS_LMM_NVIM_LOG1_INFO("NAS_LMM_DecodeNvUplmn: ulPlmnNum = ",
                                            pstUplmnAct->ulPlmnNum);
        NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeNvUplmn: PLMN IS ");
        NAS_COMM_PrintArray(NAS_COMM_GET_MM_PRINT_BUF(), (VOS_UINT8 *)pstUplmnAct->aucPlmnId,pstUplmnAct->ulPlmnNum * NAS_LMM_PLMN_AND_ACT_LEN);

        return  NAS_LMM_NVIM_OK;
    }
    else
    #endif
    {

        return  NAS_LMM_DecodeOneNvItem(pstNvCtrlTbl);
    }


}
VOS_UINT32  NAS_LMM_DecodeNvOplmn(NAS_NVIM_CTRL_TBL *pstNvCtrlTbl)
{
    #if 0
    NAS_LMM_PLMN_ACT_LIST_STRU           *pstOplmnAct;
    VOS_UINT8                           *pucMsg;
    VOS_UINT32                          i;
    VOS_UINT32                          ulValidPlmnNum = NAS_LMM_NULL;
    NAS_MM_PLMN_ID_STRU                 stPlmnId;

    NAS_LMM_MEM_SET(&stPlmnId,(VOS_CHAR)0xff,sizeof(stPlmnId));
    NAS_LMM_UndefPlmn(&stPlmnId);

    if(NAS_EMM_USIM_FEATURE_HARD        == NAS_LMM_GetUsimFeature())
    {
        pucMsg                              = (VOS_UINT8 *)pstNvCtrlTbl->pNvData;

        pstOplmnAct = (VOS_VOID *)NAS_EMMC_GetMmcGlobleAddr(NAS_LMM_GLOBLE_PARA_OPLMNLIST);

        if((NAS_NVIM_NULL_PTR == pstOplmnAct)
            || (NAS_NVIM_NULL_PTR == pstNvCtrlTbl->pNvData))
        {
            /* ��ӡ��ѯ�� ���� */
            NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeNvOplmn: Point is Null");
            return NAS_LMM_NVIM_FAIL;
        }

        if ((NAS_USIM_MIN_PLMN_LEN > pstNvCtrlTbl->usNvDataLen)
            || (NAS_LMM_NULL != pstNvCtrlTbl->usNvDataLen % NAS_LMM_PLMN_AND_ACT_LEN))
        {
            NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeNvOplmn:len is err!");
            /*return NAS_LMM_NVIM_FAIL;*/
        }

        pstOplmnAct->ulPlmnNum = pstNvCtrlTbl->usNvDataLen / NAS_LMM_PLMN_AND_ACT_LEN;
        for(i= 0; i < pstOplmnAct->ulPlmnNum; i++ )
        {
            NAS_LMM_MEM_CPY(             &stPlmnId,
                                        &pucMsg[NAS_LMM_PLMN_AND_ACT_LEN*i],
                                        NAS_LMM_NVIM_PLMN_ID_LEN);

            if (VOS_FALSE == NAS_LMM_PlmnIsUndef(&stPlmnId))
            {
                NAS_LMM_MEM_CPY(&(pstOplmnAct->aucPlmnId[NAS_LMM_PLMN_AND_ACT_LEN*ulValidPlmnNum]),
                               &pucMsg[NAS_LMM_PLMN_AND_ACT_LEN*i],
                               NAS_LMM_PLMN_AND_ACT_LEN);
                ulValidPlmnNum ++;
            }

            if (NAS_EMMC_MAX_OPLMN_NUM == ulValidPlmnNum)
            {
                NAS_LMM_NVIM_LOG_ERR("pstOplmnAct: Valid PlmnNum is overflow");
                break;
            }
        }
        pstOplmnAct->ulPlmnNum = ulValidPlmnNum;

        NAS_LMM_NVIM_LOG1_INFO("NAS_LMM_DecodeNvOplmn: ulPlmnNum = ",
                                            pstOplmnAct->ulPlmnNum);
        NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeNvOplmn: PLMN IS ");
        NAS_COMM_PrintArray(NAS_COMM_GET_MM_PRINT_BUF(), (VOS_UINT8 *)pstOplmnAct->aucPlmnId,pstOplmnAct->ulPlmnNum * NAS_LMM_PLMN_AND_ACT_LEN);

        return  NAS_LMM_NVIM_OK;
    }
    else
    #endif
    {

        return  NAS_LMM_DecodeOneNvItem(pstNvCtrlTbl);
    }

}
/*****************************************************************************
 Function Name  : NAS_LMM_DecodeNvHPLMNSelPriod
 Discription    : ��������ȼ�PLMNѡ������
 Input          : pstNvCtrlTbl:��������ṹ
 Output         : ����������
 Return         :
 History:
*****************************************************************************/
VOS_UINT32  NAS_LMM_DecodeNvHPLMNSelPriod(NAS_NVIM_CTRL_TBL *pstNvCtrlTbl)
{
#if 0
    /*������n��ֵΪ6����*/
    VOS_UINT8                           ucNumberOfUnit;

    if(NAS_EMM_USIM_FEATURE_HARD        == NAS_LMM_GetUsimFeature())
    {
        if (NAS_LMM_NVIM_HPLMN_PERI_FILE_LEN != pstNvCtrlTbl->usNvDataLen)
        {
            NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeNvHPLMNSelPriod: HPLMN Sel Peri File Len Err ");

            return NAS_LMM_NVIM_FAIL;
        }

        ucNumberOfUnit =  *(VOS_UINT8*)pstNvCtrlTbl->pNvData;

        if (NAS_LMM_NVIM_HPLMN_PERI_MAX_LEN < ucNumberOfUnit)
        {
            ucNumberOfUnit = NAS_LMM_NVIM_DEFAULT_HPLMN_PERI;
        }

        /*ulTimerLen�ĵ�λΪms*/
        NAS_EMMC_GetMmcHplmnPeriod()     = NAS_LMM_GET_MS_FROM_USIM(ucNumberOfUnit);

        NAS_LMM_NVIM_LOG1_INFO("NAS_LMM_DecodeNvHPLMNSelPriod: ucNumberOfUnit = ",
                                            ucNumberOfUnit);
        NAS_LMM_NVIM_LOG1_INFO("NAS_LMM_DecodeNvHPLMNSelPriod: ulTimerLen = (ms)",
                                            NAS_EMMC_GetMmcHplmnPeriod());

        return  NAS_LMM_NVIM_OK;
    }
    else
    #endif
    {
        return  NAS_LMM_DecodeOneNvItem(pstNvCtrlTbl);
    }
}
/*****************************************************************************
 Function Name  : NAS_LMM_DecodeNvCsLoc
 Discription    : ����CS_LOC_FILE
 Input          : pstNvCtrlTbl:��������ṹ
 Output         : ����������
 Return         :
 History:
*****************************************************************************/
VOS_UINT32  NAS_LMM_DecodeNvCsLoc(NAS_NVIM_CTRL_TBL *pstNvCtrlTbl)
{

    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeNvCsLoc is entered");

    return  NAS_LMM_DecodeOneNvItem(pstNvCtrlTbl);
}

/*****************************************************************************
 Function Name  : NAS_LMM_DecodeSimPsLoc
 Discription    : ����PS_LOC_FILE
 Input          : pstNvCtrlTbl:��������ṹ
 Output         : ����������
 Return         :
 History:
*****************************************************************************/
VOS_UINT32  NAS_LMM_DecodeSimPsLoc( NAS_NVIM_CTRL_TBL *pstNvCtrlTbl)
{
    VOS_UINT8                           *pucMsg;
    NAS_LMM_UEID_STRU                    *pstUeId;
    VOS_UINT32                          usIndex = 0;
    VOS_UINT32                          ulRslt = VOS_TRUE;
    VOS_UINT32                          ulRslt2 = VOS_TRUE;
    NAS_LMM_GUTI_STRU                    stGuti;
    NAS_MM_NETWORK_ID_STRU              stLrvTai;
    VOS_UINT8                           ucUpdataSta;
    VOS_UINT8                          *pstLastPsLoc;
    NAS_MM_PLMN_ID_STRU                 stPlmnId;


    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeSimPsLoc is entered ");

    NAS_LMM_MEM_SET(&stGuti, 0, sizeof(NAS_LMM_GUTI_STRU));
    NAS_LMM_MEM_SET(&stLrvTai, 0, sizeof(NAS_MM_NETWORK_ID_STRU));

    pstUeId  = NAS_LMM_GetEmmInfoUeidAddr();
    pstLastPsLoc  = NAS_LMM_GetEmmInfoLastWriteUsimPsLocAddr();

    pucMsg                              = (VOS_UINT8*)pstNvCtrlTbl->pNvData;

    /* �������е��������浽�ϴ�д����ȫ�ֱ�����*/
    NAS_LMM_MEM_CPY( pstLastPsLoc,
                     pucMsg,
                     pstNvCtrlTbl->usNvDataLen);

    /*****************GUTI*********************/
    /*����ļ�GUTIͷ����Ч��*/
    if ( NAS_LMM_NVIM_GUTI_AND_OETOI_LEN != pucMsg[usIndex])
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimPsLoc: GUTI HEAD ERR");
        usIndex += NAS_LMM_NVIM_GUTI_AND_OETOI_LEN;
        usIndex ++;
        pstUeId->bitOpGuti                  = NAS_NVIM_BIT_NO_SLCT;

    }
    else
    {
        stGuti.stGutiHead.ucLenth  = NAS_LMM_NVIM_GUTI_AND_OETOI_LEN;

        usIndex ++;

        stGuti.stGutiHead.ucOeToi  = pucMsg[usIndex] & NAS_EMM_LOW_HALF_BYTE_F;

        if (NAS_LMM_NVIM_GUTI_OETOI != stGuti.stGutiHead.ucOeToi)
        {
            NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimPsLoc: GUTI OETOI ERR");
            usIndex += NAS_LMM_NVIM_GUTI_AND_OETOI_LEN;
            pstUeId->bitOpGuti                  = NAS_NVIM_BIT_NO_SLCT;

        }
        else
        {
            usIndex ++;

            /*���GUTI���ݵĺϷ���*/
            ulRslt = NAS_EMM_CheckSimGutiValid(pucMsg, usIndex, NAS_LMM_NVIM_GUTI_LEN);


            /*���GUTI��PLMN ID�ĺϷ��� */
            stPlmnId.aucPlmnId[0] = pucMsg[0 + usIndex];

            stPlmnId.aucPlmnId[1] = pucMsg[1 + usIndex];

            stPlmnId.aucPlmnId[2] = pucMsg[2 + usIndex];

            ulRslt2 &= NAS_LMM_CheckPlmnIsInvalid(&stPlmnId);


            /*������ݵ���Ч��*/
            if ((VOS_FALSE == ulRslt) || (VOS_TRUE == ulRslt2))
            {
                NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimPsLoc: GUTI Content ERR");

                usIndex = usIndex + NAS_LMM_NVIM_GUTI_LEN;

                pstUeId->bitOpGuti                  = NAS_NVIM_BIT_NO_SLCT;
            }
            else
            {
                NAS_LMM_MEM_CPY(    &stGuti.stPlmnId,
                                    &stPlmnId,
                                    sizeof(NAS_MM_PLMN_ID_STRU));

                usIndex += 3;

                stGuti.stMmeGroupId.ucGroupId = pucMsg[usIndex];
                usIndex ++;

                stGuti.stMmeGroupId.ucGroupIdCnt = pucMsg[usIndex];
                usIndex ++;

                stGuti.stMmeCode.ucMmeCode = pucMsg[usIndex];
                usIndex ++;

                NAS_LMM_MEM_CPY(                    &(stGuti.stMTmsi),
                                                    &(pucMsg[usIndex]),
                                                    sizeof(NAS_LMM_MTMSI_STRU));
                usIndex += sizeof(NAS_LMM_MTMSI_STRU);


                pstUeId->bitOpGuti                  = NAS_NVIM_BIT_SLCT;
                NAS_LMM_MEM_CPY(                    &(pstUeId->stGuti),
                                                    &stGuti,
                                                    sizeof(NAS_LMM_GUTI_STRU));
            }
        }
    }
    /****************L.R.V TAI******************/
    /* ���LRV TAI�� PLMN�Ϸ��� */
    /*���PLMN��MCC��MNC�Ƿ���Ч*/
    NAS_LMM_MEM_CPY(                    &(stLrvTai.stPlmnId),
                                        &(pucMsg[usIndex]),
                                        NAS_LMM_NVIM_PLMN_ID_LEN);
    usIndex += NAS_LMM_NVIM_PLMN_ID_LEN;

    ulRslt = NAS_LMM_CheckPlmnIsInvalid(&(stLrvTai.stPlmnId));
    /*ulRslt = NAS_LMM_CheckWhetherAllBytesAreFF(pucMsg, usIndex, NAS_LMM_NVIM_PLMN_ID_LEN);*/
    /*plmn��Ч������ȫ�ֱ�����PLMNΪ0XFFFFFF,TACΪ0xfffe*/
    if (VOS_TRUE == ulRslt)
    {
        NAS_EMM_ClearLVRTai();
        usIndex = usIndex + NAS_LMM_NVIM_TAC_LEN;
    }
    else
    {
        NAS_LMM_MEM_CPY(                     &(stLrvTai.stTac),
                                            &(pucMsg[usIndex]),
                                            NAS_LMM_NVIM_TAC_LEN);
        usIndex += NAS_LMM_NVIM_TAC_LEN;

        if (VOS_TRUE == NAS_LMM_TacIsUnexist(&(stLrvTai.stTac)))
        {
            NAS_LMM_UndefTac(&(stLrvTai.stTac));
        }

        NAS_EMM_SetLVRTai(&stLrvTai);
    }
    /***************UPDATE STATUS**************/
    /*����ļ�UPDATE STATUS�����Ч��*/
    ucUpdataSta = (pucMsg[usIndex])& NAS_LMM_NVIM_LOW_3_BIT_1;

    if ( NAS_LMM_NVIM_VALID_UPDATESTA < ucUpdataSta)
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimPsLoc: UPDATE STATUS Content ERR");
    }
    else
    {
        NAS_EMM_MAIN_CONTEXT_SET_AUX_UPDATE_STAE(pucMsg[usIndex]);
    }

    usIndex ++;

    pstNvCtrlTbl->usNvDataLen           = (VOS_UINT16)(usIndex);

    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeSimPsLoc:GUTI IS: ");
    NAS_COMM_PrintArray(NAS_COMM_GET_MM_PRINT_BUF(),(VOS_UINT8 *)&pstUeId->stGuti,20);
    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeSimPsLoc:L.R.V TAI IS: ");
    NAS_COMM_PrintArray(NAS_COMM_GET_MM_PRINT_BUF(),(VOS_UINT8*)NAS_LMM_GetEmmInfoLastRegPlmnAddr(),4);
    NAS_COMM_PrintArray(NAS_COMM_GET_MM_PRINT_BUF(),(VOS_UINT8*)NAS_LMM_GetEmmInfoLastRegTacAddr(),4);
    NAS_LMM_NVIM_LOG1_INFO("NAS_LMM_DecodeSimPsLoc:UPDATE STATUS IS: ",NAS_EMM_UPDATE_STAE);

    return  NAS_LMM_NVIM_OK;
}


/*****************************************************************************
 Function Name  : NAS_LMM_DecodeSimImsi
 Discription    : ����IMSI
 Input          : pstNvCtrlTbl:��������ṹ
 Output         : ����������
 Return         :
 History:
*****************************************************************************/
VOS_UINT32  NAS_LMM_DecodeSimImsi(const NAS_NVIM_CTRL_TBL *pstNvCtrlTbl)
{
    VOS_UINT8                           *pucImsi;
    NAS_EMM_UEID_STRU                   *pstUeId;

    pstUeId = (VOS_VOID *)NAS_EMM_GetEmmGlobleAddr(NAS_LMM_GLOBLE_PAEA_UEID);

    pucImsi = (VOS_VOID *)NAS_EMM_GetEmmGlobleAddr(NAS_LMM_GLOBLE_PAEA_IMSI);

    if((NAS_NVIM_NULL_PTR == pstUeId) || (NAS_NVIM_NULL_PTR == pucImsi))
    {
        /* ��ӡ��ѯ�� ���� */
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimImsi: Point is Null");
        return NAS_LMM_NVIM_FAIL;
    }

    if (NAS_LMM_NVIM_IMSI_FILE_LEN != pstNvCtrlTbl->usNvDataLen)
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimImsi: USIM IMSI LENGTH ERR");

        return NAS_LMM_NVIM_FAIL;
    }

    pstUeId->bitOpImsi                  = NAS_NVIM_BIT_SLCT;

    NAS_LMM_MEM_CPY(                     pucImsi,
                                        pstNvCtrlTbl->pNvData,
                                        NAS_LMM_NVIM_IMSI_FILE_LEN);
/*PC REPLAY MODIFY BY LEILI DELETE*/

    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeSimImsi: IMSI IS ");
    NAS_COMM_PrintArray(NAS_COMM_GET_MM_PRINT_BUF(),pucImsi,NAS_LMM_NVIM_IMSI_FILE_LEN);

    return  NAS_LMM_NVIM_OK;
}


/*****************************************************************************
 Function Name  : NAS_LMM_DecodeNvCsCKIK
 Discription    : ����CS_CKIK
 Input          : pstNvCtrlTbl:��������ṹ
 Output         : ����������
 Return         :
 History:
*****************************************************************************/
VOS_UINT32  NAS_LMM_DecodeNvCsCKIK(NAS_NVIM_CTRL_TBL *pstNvCtrlTbl)
{

    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeNvCsCKIK is entered");

    return  NAS_LMM_DecodeOneNvItem(pstNvCtrlTbl);
}
/*****************************************************************************
 Function Name  : NAS_LMM_DecodeNvPsKey
 Discription    : ����PS_KEY
 Input          : pstNvCtrlTbl:��������ṹ
 Output         : ����������
 Return         :
 History:
*****************************************************************************/
VOS_UINT32  NAS_LMM_DecodeNvPsKey(NAS_NVIM_CTRL_TBL *pstNvCtrlTbl)
{

    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeNvPsKey is entered");

    return  NAS_LMM_DecodeOneNvItem(pstNvCtrlTbl);
}
/*****************************************************************************
 Function Name  : NAS_LMM_DecodeNvKC
 Discription    : ����KC
 Input          : pstNvCtrlTbl:��������ṹ
 Output         : ����������
 Return         :
 History:
*****************************************************************************/
VOS_UINT32  NAS_LMM_DecodeNvKC(NAS_NVIM_CTRL_TBL *pstNvCtrlTbl)
{

    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeNvKC is entered");

    return  NAS_LMM_DecodeOneNvItem(pstNvCtrlTbl);
}
/*****************************************************************************
 Function Name  : NAS_LMM_DecodeNvSecAlgrithm
 Discription    : ����Security Algorithms
 Input          : pstNvCtrlTbl:��������ṹ
 Output         : ����������
 Return         :
 History:
*****************************************************************************/
VOS_UINT32  NAS_LMM_DecodeNvSecAlgrithm(NAS_NVIM_CTRL_TBL *pstNvCtrlTbl)
{

    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeNvSecAlgrithm is entered");

    return  NAS_LMM_DecodeOneNvItem(pstNvCtrlTbl);
}
/*****************************************************************************
 Function Name  : NAS_LMM_DecodeNvMacAddr
 Discription    : ����MAC ADDR
 Input          : pstNvCtrlTbl:��������ṹ
 Output         : ����������
 Return         :
 History:
*****************************************************************************/
VOS_UINT32  NAS_LMM_DecodeNvMacAddr(NAS_NVIM_CTRL_TBL *pstNvCtrlTbl)
{

    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeNvMacAddr is entered");

    return  NAS_LMM_DecodeOneNvItem(pstNvCtrlTbl);
}
/*****************************************************************************
 Function Name  : NAS_LMM_DecodeNvIPAddr
 Discription    : ����IP ADDR
 Input          : pstNvCtrlTbl:��������ṹ
 Output         : ����������
 Return         :
 History:
*****************************************************************************/
VOS_UINT32  NAS_LMM_DecodeNvIPAddr(NAS_NVIM_CTRL_TBL *pstNvCtrlTbl)
{

    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeNvIPAddr is entered");

    return  NAS_LMM_DecodeOneNvItem(pstNvCtrlTbl);
}

VOS_UINT32  NAS_LMM_DecodeNvACC(const  NAS_NVIM_CTRL_TBL *pstNvCtrlTbl  )
{
    VOS_UINT8   ucTmp;

    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeNvACC is entered");

    if (NAS_LMM_NVIM_ACC_FILE_LEN != pstNvCtrlTbl->usNvDataLen)
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeNvACC : ACC File Len Err!");

        return NAS_LMM_NVIM_FAIL;
    }

    ucTmp      = *(VOS_UINT8 *)(pstNvCtrlTbl->pNvData);
    gstRrcNasAc.usAccClassMask = ucTmp << 8;
    ucTmp      = *((VOS_UINT8 *)pstNvCtrlTbl->pNvData + 1);
    gstRrcNasAc.usAccClassMask |= ucTmp;

    NAS_LMM_NVIM_LOG1_INFO("NAS_LMM_DecodeNvACC: access class is :",gstRrcNasAc.usAccClassMask);
    return NAS_LMM_NVIM_OK;
}



VOS_UINT32  NAS_LMM_DecodeSimSecContext( NAS_NVIM_CTRL_TBL *pstNvCtrlTbl )
{
    NAS_LMM_SECU_CONTEXT_STRU            stCurSecuContext;
    VOS_UINT16                          usIndex = 0;
    VOS_UINT8                          *pucMsg;

    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_DecodeSimSecContext is entered");

    /*��ʼ��stCurSecuContext*/
    NAS_LMM_MEM_SET(&(stCurSecuContext), 0, sizeof(NAS_LMM_SECU_CONTEXT_STRU));

    /*���氲ȫ�����ĵĳ���*/
    NAS_EMM_GET_USIM_SECU_FILE_LEN() = pstNvCtrlTbl->usNvDataLen;

    pucMsg     = (VOS_UINT8*)pstNvCtrlTbl->pNvData;


    NAS_LMM_MEM_SET(NAS_LMM_GET_USIM_EPS_SECU_CONTEXT_ADDR(),
                    0,
                    NAS_NVIM_SECU_CONTEXT_MAX_LEN);


    /* ����ļ�Tag�Ƿ�Ϸ� */
    if ( NAS_LMM_NVIM_SECU_CONTEXT_TAG != pucMsg[usIndex++])
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimSecContext: File Tag Err ");
        return  NAS_LMM_NVIM_FAIL;
    }

    /* ����ļ�Length�Ƿ�Ϸ� */
    if ( NAS_LMM_NVIM_SEC_MSG_LEN != pucMsg[usIndex++] )/*��ȷ��*/
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimSecContext: File LEN Err ");
        return  NAS_LMM_NVIM_FAIL;
    }

    /********************************* KSIASME *****************************************/
    if ( NAS_LMM_NVIM_SEC_KSI_TAG != pucMsg[usIndex++] )
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimSecContext: KSI TAG Err ");

        return  NAS_LMM_NVIM_FAIL;
    }

    if (NAS_LMM_NVIM_SEC_KSI_LEN != pucMsg[usIndex++])
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimSecContext: KSI LEN Err ");

        return  NAS_LMM_NVIM_FAIL;
    }

    /* ���KSI�ĺϷ��� */
    if ( NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE < pucMsg[usIndex] )
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimSecContext: KSI Content Err ");

        return  NAS_LMM_NVIM_FAIL;
    }

    stCurSecuContext.ucKSIasme = pucMsg[usIndex];

    usIndex                           += NAS_LMM_NVIM_SEC_KSI_LEN ;

    /*********************************** KASME ******************************************/
    if ( NAS_LMM_NVIM_SEC_KASME_TAG != pucMsg[usIndex++] )
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimSecContext: KASME TAG Err ");

        return  NAS_LMM_NVIM_FAIL;
    }

    if (NAS_LMM_NVIM_SEC_KASME_LEN != pucMsg[usIndex++])
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimSecContext: KASME LEN Err ");

        return  NAS_LMM_NVIM_FAIL;
    }

    NAS_LMM_MEM_CPY(                   (stCurSecuContext.aucKasme),
                                      &(pucMsg[usIndex]),
                                      NAS_LMM_NVIM_SEC_KASME_LEN);

    usIndex                           += NAS_LMM_NVIM_SEC_KASME_LEN ;

    /**************************** Uplink NAS count ************************************/
    if ( NAS_LMM_NVIM_SEC_UPNASCOUNT_TAG != pucMsg[usIndex++] )
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimSecContext: UPNASCOUNT TAG Err ");

        return  NAS_LMM_NVIM_FAIL;
    }

    if (NAS_LMM_NVIM_SEC_NAS_COUNT_LEN != pucMsg[usIndex++])
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimSecContext: UPNASCOUNT LEN Err ");

        return  NAS_LMM_NVIM_FAIL;
    }

    stCurSecuContext.ulUlNasCount = stCurSecuContext.ulUlNasCount | ((VOS_UINT32)pucMsg[usIndex++] << NAS_LMM_NVIM_MOVE_24_BIT);
    stCurSecuContext.ulUlNasCount = stCurSecuContext.ulUlNasCount | ((VOS_UINT32)pucMsg[usIndex++] << NAS_LMM_NVIM_MOVE_16_BIT);
    stCurSecuContext.ulUlNasCount = stCurSecuContext.ulUlNasCount | ((VOS_UINT32)pucMsg[usIndex++] << NAS_LMM_NVIM_MOVE_8_BIT);
    stCurSecuContext.ulUlNasCount = stCurSecuContext.ulUlNasCount | (VOS_UINT32)pucMsg[usIndex++];

    /* ���NAS count�ĺϷ��� */
    if (NAS_LMM_NVIM_MAX_NASCOUNT < stCurSecuContext.ulUlNasCount)
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimSecContext: UPNASCOUNT Content Err ");

        return  NAS_LMM_NVIM_FAIL;
    }


    /*************************** Downlink NAS count ***********************************/
    if ( NAS_LMM_NVIM_SEC_DNNASCOUNT_TAG != pucMsg[usIndex++] )
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimSecContext: DNNASCOUNT Tag Err ");

        return  NAS_LMM_NVIM_FAIL;
    }

    if (NAS_LMM_NVIM_SEC_NAS_COUNT_LEN != pucMsg[usIndex++])
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimSecContext: DNNASCOUNT Len Err ");

        return  NAS_LMM_NVIM_FAIL;
    }

    stCurSecuContext.ulDlNasCount = stCurSecuContext.ulDlNasCount | ((VOS_UINT32)pucMsg[usIndex++] << NAS_LMM_NVIM_MOVE_24_BIT);
    stCurSecuContext.ulDlNasCount = stCurSecuContext.ulDlNasCount | ((VOS_UINT32)pucMsg[usIndex++]<< NAS_LMM_NVIM_MOVE_16_BIT);
    stCurSecuContext.ulDlNasCount = stCurSecuContext.ulDlNasCount | ((VOS_UINT32)pucMsg[usIndex++]<< NAS_LMM_NVIM_MOVE_8_BIT);
    stCurSecuContext.ulDlNasCount = stCurSecuContext.ulDlNasCount | (VOS_UINT32)pucMsg[usIndex++];

    /* ���NAS count�ĺϷ��� */
    if (NAS_LMM_NVIM_MAX_NASCOUNT < stCurSecuContext.ulDlNasCount)
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimSecContext: DNNASCOUNT Content Err ");

        return  NAS_LMM_NVIM_FAIL;
    }

    /******** Identifiers of selected NAS integrity and encryption algorithms *********/
    if ( NAS_LMM_NVIM_SEC_ALG_TAG != pucMsg[usIndex++] )
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimSecContext: Alg Tag Err ");

        return  NAS_LMM_NVIM_FAIL;
    }

    if (NAS_LMM_NVIM_SEC_ENCINT_LEN != pucMsg[usIndex++])
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimSecContext: Alg LEN Err ");

        return  NAS_LMM_NVIM_FAIL;
    }

    stCurSecuContext.ucSecuAlg = pucMsg[usIndex];

    usIndex                           += NAS_LMM_NVIM_SEC_ENCINT_LEN;

    /*���Ϸ���*/
    if ( VOS_TRUE != NAS_LMM_CheckSecuAlgValid(stCurSecuContext.ucSecuAlg))
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_DecodeSimSecContext: Alg Content Err ");

        return  NAS_LMM_NVIM_FAIL;
    }


    /* ������ȳ���������������Ϊ�쳣�����������������������ڻ��������������λ���ѳ�ʼ��Ϊ0 */
    if(NAS_NVIM_SECU_CONTEXT_MAX_LEN >= pstNvCtrlTbl->usNvDataLen)
    {
        /* �����USIM���ж�ȡ���İ�ȫ����������*/
        NAS_LMM_MEM_CPY(NAS_LMM_GET_USIM_EPS_SECU_CONTEXT_ADDR(),
                        pucMsg,
                        pstNvCtrlTbl->usNvDataLen);
    }


    pstNvCtrlTbl->usDataLen           = usIndex;

    /*���ð�ȫ�����ĸ��º���,������NasCK ,NasIk*/
    NAS_EMM_SecuSetCurCntxt(&stCurSecuContext);

    return NAS_LMM_NVIM_OK;
}
VOS_UINT32  NAS_LMM_DecodeSoftUsimCnf( NAS_NVIM_CTRL_TBL *pstNvCtrlTbl )
{
    (VOS_VOID)pstNvCtrlTbl;

    return NAS_LMM_NVIM_OK;
}



VOS_UINT32  NAS_LMM_EncodeOneNvItem(NAS_NVIM_CTRL_TBL *pstNvCtrlTbl)
{
    if(NAS_NVIM_NULL_PTR == pstNvCtrlTbl->pData)
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_EncodeOneNvItem: WARNING->pData is Null");
          return NAS_LMM_NVIM_FAIL;
    }

    NAS_LMM_MEM_CPY(                   pstNvCtrlTbl->pNvData,
                                        pstNvCtrlTbl->pData,
                                        pstNvCtrlTbl->usDataLen);

    /*Data len after encode*/
    pstNvCtrlTbl->usNvDataLen           = pstNvCtrlTbl->usDataLen;
    return NAS_LMM_NVIM_OK;
}
VOS_UINT32  NAS_LMM_EncodeNvUplmn( NAS_NVIM_CTRL_TBL *pstNvCtrlTbl )
{
#if 0
    LNAS_LMM_NV_PLMN_LIST_STRU      *pstNvplmnList;
    NAS_MM_PLMN_LIST_STRU           *pstUplmnList;
    VOS_UINT32                      i;
    VOS_UINT32                      ulIndex;
    VOS_UINT8                       *pucMsg;

    if(NAS_NVIM_NULL_PTR == pstNvCtrlTbl->pData)
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_EncodeNvUplmn: WARNING->pData is Null");
        return NAS_LMM_NVIM_FAIL;
    }

    if(NAS_EMM_USIM_FEATURE_HARD        == NAS_LMM_GetUsimFeature())
    {
        pstNvplmnList   = (LNAS_LMM_NV_PLMN_LIST_STRU *)pstNvCtrlTbl->pData;
        pstUplmnList    = &(pstNvplmnList->stPlmnList);

        pucMsg          = (VOS_UINT8 *)pstNvCtrlTbl->pNvData;

        pstNvCtrlTbl->usNvDataLen = (VOS_UINT16)((pstUplmnList->ulPlmnNum) * NAS_LMM_PLMN_AND_ACT_LEN);
        for(i = 0; i < pstUplmnList->ulPlmnNum; i++)
        {
            ulIndex = i * NAS_LMM_PLMN_AND_ACT_LEN;
            NAS_LMM_MEM_CPY(           &pucMsg[ulIndex],
                                        &pstUplmnList->astPlmnId[i],
                                        NAS_LMM_NVIM_PLMN_ID_LEN);

            /* NAS_LMM_ACCESS_TECH_LTE Ϊ 0x40   NAS_LMM_ACCESS_TECH_RSV Ϊ 0x00*/
            pucMsg[ulIndex + 3] = NAS_LMM_ACCESS_TECH_LTE;
            pucMsg[ulIndex + 4] = NAS_LMM_ACCESS_TECH_RSV;
        }

        NAS_LMM_NVIM_LOG1_INFO("NAS_LMM_EncodeNvUplmn: LEN= ",
                                                  pstNvCtrlTbl->usNvDataLen);
        NAS_LMM_NVIM_LOG_INFO("NAS_LMM_EncodeNvUplmn: PLMN IS ");
        NAS_COMM_PrintArray(NAS_COMM_GET_MM_PRINT_BUF(),(VOS_UINT8 *)pstNvCtrlTbl->pNvData,pstNvCtrlTbl->usNvDataLen);

        return NAS_LMM_NVIM_OK;
    }
    else
    #endif
    {
        NAS_LMM_NVIM_LOG_INFO("NAS_LMM_EncodeNvUplmn is entered");

        return  NAS_LMM_EncodeOneNvItem(pstNvCtrlTbl);
    }

}
VOS_UINT32  NAS_LMM_EncodeSimSecContext( NAS_NVIM_CTRL_TBL *pstNvCtrlTbl )
{
    VOS_UINT8                          *pucNvData = VOS_NULL_PTR;
    LNAS_LMM_NV_EPS_SEC_CONTEXT_STRU   *pstNvSecContext = VOS_NULL_PTR;
    VOS_UINT16                          usIndex = 0;

    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_EncodeSimSecContext is entered");

    if(NAS_NVIM_NULL_PTR == pstNvCtrlTbl->pData)
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_EncodeSimSecContext: WARNING->pData is Null");
        return NAS_LMM_NVIM_FAIL;
    }

    pstNvSecContext                     = (LNAS_LMM_NV_EPS_SEC_CONTEXT_STRU *)pstNvCtrlTbl->pData;
    pucNvData                           = (VOS_UINT8 *)pstNvCtrlTbl->pNvData;

    /*��ʼ����ȫ������ΪȫFF*/
    NAS_LMM_SetArrayAllBytesFF(pucNvData,NAS_EMM_GET_USIM_SECU_FILE_LEN());

    /*EPS NAS Security Context Tag    SMC ����*/
    pucNvData[usIndex ++]               = NAS_LMM_NVIM_SECU_CONTEXT_TAG;

    pucNvData[usIndex ++]               = NAS_LMM_NVIM_SEC_MSG_LEN;

    /*********** KSIASME ************/
    pucNvData[usIndex ++]               = NAS_LMM_NVIM_SEC_KSI_TAG;
    pucNvData[usIndex ++]               = sizeof(VOS_UINT8);
    pucNvData[usIndex ++]               = pstNvSecContext->ucKSIasme;

    /*********** KASME **************/
    pucNvData[usIndex ++]               = NAS_LMM_NVIM_SEC_KASME_TAG;
    pucNvData[usIndex ++]               = NAS_LMM_NVIM_SEC_KASME_LEN;
    NAS_LMM_MEM_CPY(                   &(pucNvData[usIndex]),
                                        pstNvSecContext->aucKasme,
                                        NAS_LMM_NVIM_SEC_KASME_LEN);

    usIndex                             += NAS_LMM_NVIM_SEC_KASME_LEN;

    /*********** Uplink NAS count **************/
    pucNvData[usIndex ++]               = NAS_LMM_NVIM_SEC_UPNASCOUNT_TAG;
    pucNvData[usIndex ++]               = sizeof(VOS_UINT32);

    pucNvData[usIndex ++]               = (VOS_UINT8)((pstNvSecContext->ulUlNasCount >> NAS_LMM_NVIM_MOVE_24_BIT)
                                                      & NAS_LMM_NVIM_BYTE_FF);
    pucNvData[usIndex ++]               = (VOS_UINT8)((pstNvSecContext->ulUlNasCount >> NAS_LMM_NVIM_MOVE_16_BIT)
                                                      & NAS_LMM_NVIM_BYTE_FF);
    pucNvData[usIndex ++]               = (VOS_UINT8)((pstNvSecContext->ulUlNasCount >> NAS_LMM_NVIM_MOVE_8_BIT)
                                                      & NAS_LMM_NVIM_BYTE_FF);
    pucNvData[usIndex ++]               = (VOS_UINT8)((pstNvSecContext->ulUlNasCount)
                                                      & NAS_LMM_NVIM_BYTE_FF);

    /*********** Downlink NAS count **************/
    pucNvData[usIndex ++]               = NAS_LMM_NVIM_SEC_DNNASCOUNT_TAG;
    pucNvData[usIndex ++]               = sizeof(VOS_UINT32);

    pucNvData[usIndex ++]               = (VOS_UINT8)((pstNvSecContext->ulDlNasCount >> NAS_LMM_NVIM_MOVE_24_BIT)
                                                      & NAS_LMM_NVIM_BYTE_FF);
    pucNvData[usIndex ++]               = (VOS_UINT8)((pstNvSecContext->ulDlNasCount >> NAS_LMM_NVIM_MOVE_16_BIT)
                                                      & NAS_LMM_NVIM_BYTE_FF);
    pucNvData[usIndex ++]               = (VOS_UINT8)((pstNvSecContext->ulDlNasCount >> NAS_LMM_NVIM_MOVE_8_BIT)
                                                      & NAS_LMM_NVIM_BYTE_FF);
    pucNvData[usIndex ++]               = (VOS_UINT8)((pstNvSecContext->ulDlNasCount)
                                                      & NAS_LMM_NVIM_BYTE_FF);


    /*********** Identifiers of selected NAS integrity and encryption algorithms **************/
    pucNvData[usIndex ++]               = NAS_LMM_NVIM_SEC_ALG_TAG;
    pucNvData[usIndex ++]               = sizeof(VOS_UINT8);
    pucNvData[usIndex ++]               = pstNvSecContext->ucSecuAlg;

    pstNvCtrlTbl->usNvDataLen           = (VOS_UINT16)NAS_EMM_GET_USIM_SECU_FILE_LEN();

    NAS_LMM_NVIM_LOG1_INFO("NAS_LMM_EncodeSimSecContext: ESP SEC CONTEXT",pstNvCtrlTbl->usNvDataLen);

    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_EncodeSimSecContext: ESP SEC CONTEXT");
    NAS_COMM_PrintArray(NAS_COMM_GET_MM_PRINT_BUF(),pucNvData,pstNvCtrlTbl->usNvDataLen);

    /* ����ϴ�д���������뱾��Ҫд�����ȫ��ͬ��������д�� */
    if((0 == NAS_LMM_MEM_CMP( (const VOS_VOID *)NAS_LMM_GET_USIM_EPS_SECU_CONTEXT_ADDR(),
                              (const VOS_VOID *)pucNvData,
                              pstNvCtrlTbl->usNvDataLen)))
    {
        NAS_LMM_NVIM_LOG_INFO("NAS_LMM_EncodeSimPsLoc:  Equal  value!");
        return NAS_LMM_NVIM_WRITTEN_UNNEEDED;
    }

    /* ����ͬ������Ϊ����д������ */
    NAS_LMM_MEM_SET(NAS_LMM_GET_USIM_EPS_SECU_CONTEXT_ADDR(),
                    0,
                    NAS_NVIM_SECU_CONTEXT_MAX_LEN);

    /* ������ȴ��ڻ�����������Ϊ�쳣�������ϴ�д������ */
    if(NAS_NVIM_SECU_CONTEXT_MAX_LEN >= pstNvCtrlTbl->usNvDataLen)
    {
        NAS_LMM_MEM_CPY(NAS_LMM_GET_USIM_EPS_SECU_CONTEXT_ADDR(),
                        pucNvData,
                        pstNvCtrlTbl->usNvDataLen);
    }

    return NAS_LMM_NVIM_OK;

}


VOS_UINT32  NAS_LMM_EncodeSimPsLoc( NAS_NVIM_CTRL_TBL *pstNvCtrlTbl )
{
    NAS_LMM_GUTI_STRU                    stGuti;
    NAS_MM_TA_STRU                      stTai;
    VOS_UINT8                           *pucNvData;
    VOS_UINT8                           *pucData;
    VOS_UINT16                           usIndex = 0;
    VOS_UINT32                           ulLen = 0;
    VOS_UINT8                          *pstLastPsLoc;

    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_EncodeSimPsLoc is entered");

    pstLastPsLoc                        = NAS_LMM_GetEmmInfoLastWriteUsimPsLocAddr();

    if(NAS_NVIM_NULL_PTR == pstNvCtrlTbl->pData)
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_EncodeSimPsLoc: WARNING->pData is Null");
        return NAS_LMM_NVIM_FAIL;
    }

    pucData                             = (VOS_UINT8 *)pstNvCtrlTbl->pData;
    pucNvData                           = (VOS_UINT8 *)pstNvCtrlTbl->pNvData;

    /*****************GUTI*******************/
    ulLen                               = sizeof(VOS_UINT32);
    NAS_LMM_MEM_CPY(                     &stGuti,
                                        &(pucData[ulLen]),
                                        sizeof(NAS_LMM_GUTI_STRU));

    pucNvData[usIndex ++]               = NAS_LMM_NVIM_GUTI_AND_OETOI_LEN;
    pucNvData[usIndex ++]               = (stGuti.stGutiHead.ucOeToi |
                                           NAS_EMM_HIGH_HALF_BYTE_F);
    pucNvData[usIndex ++]               = stGuti.stPlmnId.aucPlmnId[0];
    pucNvData[usIndex ++]               = stGuti.stPlmnId.aucPlmnId[1];
    pucNvData[usIndex ++]               = stGuti.stPlmnId.aucPlmnId[2];
    pucNvData[usIndex ++]               = stGuti.stMmeGroupId.ucGroupId;
    pucNvData[usIndex ++]               = stGuti.stMmeGroupId.ucGroupIdCnt;
    pucNvData[usIndex ++]               = stGuti.stMmeCode.ucMmeCode;
    /*pucNvData[usIndex ++]               = stGuti.stMTmsi.ucMTmsi;*/
    NAS_LMM_MEM_CPY(                     &(pucNvData[usIndex]),
                                        &(stGuti.stMTmsi),
                                        sizeof(NAS_LMM_MTMSI_STRU));
    usIndex                             += sizeof(NAS_EMM_MTMSI_STRU);

    /*L.V.R TAI*/
    ulLen                               += sizeof(NAS_LMM_GUTI_STRU);
    NAS_LMM_MEM_CPY(                     &stTai,
                                        &(pucData[ulLen]),
                                        sizeof(NAS_MM_TA_STRU));

    pucNvData[usIndex ++]               = stTai.stPlmnId.aucPlmnId[0];
    pucNvData[usIndex ++]               = stTai.stPlmnId.aucPlmnId[1];
    pucNvData[usIndex ++]               = stTai.stPlmnId.aucPlmnId[2];
    pucNvData[usIndex ++]               = stTai.stTac.ucTac;
    pucNvData[usIndex ++]               = stTai.stTac.ucTacCnt;

    /*UPDATE STATUS*/
    ulLen                               += sizeof(NAS_MM_TA_STRU);
    pucNvData[usIndex++]                = pucData[ulLen];

    pstNvCtrlTbl->usNvDataLen           = usIndex;

    /* ����ϴ�д���������뱾��Ҫд�����ȫ��ͬ��������д�� */
    if((0 == NAS_LMM_MEM_CMP( (const VOS_VOID *)pstLastPsLoc,
                              (const VOS_VOID *)pucNvData,
                              pstNvCtrlTbl->usNvDataLen)))
    {
        NAS_LMM_NVIM_LOG_INFO("NAS_LMM_EncodeSimPsLoc:  Equal  value!");
        return NAS_LMM_NVIM_WRITTEN_UNNEEDED;
    }

    /* ����ͬ������Ϊ����д������ */
    NAS_LMM_MEM_SET( pstLastPsLoc,
                      0,
                      sizeof(LNAS_LMM_NV_EPS_LOC_STRU));

     NAS_LMM_MEM_CPY( pstLastPsLoc,
                      pucNvData,
                      pstNvCtrlTbl->usNvDataLen);

    return NAS_LMM_NVIM_OK;
}
/*lint +e961*/
/*lint +e960*/


VOS_UINT32  NAS_LMM_EncodeNvHPLMNSelPriod( NAS_NVIM_CTRL_TBL *pstNvCtrlTbl )
{
    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_EncodeNvHPLMNSelPriod is entered");

    return  NAS_LMM_EncodeOneNvItem(pstNvCtrlTbl);
}


VOS_VOID  NAS_LMM_CheckNvDrxParamAndTransVal(
                                         CONST  NAS_MM_DRX_STRU *pstNvimDrxParam,
                                                NAS_MM_DRX_STRU *pstTransDrxParam)
{
    /*������ָ��*/
    if ((VOS_NULL_PTR == pstNvimDrxParam)
      ||(VOS_NULL_PTR == pstTransDrxParam))
    {
        NAS_LMM_PUBM_LOG_ERR("NAS_LMM_CheckNvDrxParamAndTransVal: Input NULL PTR.");
        return;
    }

    /*SPLIT on CCCH��Ч����֤,��Ч��ֵ 0*/
    if (NAS_EMM_PARA_INVALID == NAS_LMM_IsDrxSplitOnCcchValid(pstNvimDrxParam->ucSplitOnCcch))
    {
        NAS_LMM_PUBM_LOG_ERR("NAS_LMM_CheckNvDrxParamAndTransVal: SplitOnCcch InValid.");
        pstTransDrxParam->ucSplitOnCcch = NAS_LMM_SPLIT_ON_CCCH_NOT_SUPPORT;
    }
    else
    {
        pstTransDrxParam->ucSplitOnCcch = pstNvimDrxParam->ucSplitOnCcch;
    }

    /*non_DRX Timer��Ч����֤����Ч��ֵ 0*/
    if (NAS_EMM_PARA_INVALID == NAS_LMM_IsDrxNonDrxTimerValid(pstNvimDrxParam->ucNonDrxTimer))
    {
        NAS_LMM_PUBM_LOG_ERR("NAS_LMM_CheckNvDrxParamAndTransVal: NonDrxTimer InValid.");
        pstTransDrxParam->ucNonDrxTimer = NAS_LMM_NO_NONE_DRX_MODE;
    }
    else
    {
        pstTransDrxParam->ucNonDrxTimer = pstNvimDrxParam->ucNonDrxTimer;
    }

    /*SPLIT PG CYCLE CODE��Ч��ֱ�Ӹ�ֵ����Ч����Э��ת��*/
    if (NAS_EMM_PARA_INVALID == NAS_LMM_IsDrxPgCycleCodeValid(pstNvimDrxParam->ucSplitPgCode))
    {
        pstTransDrxParam->ucSplitPgCode = NAS_LMM_SPLIT_PG_CYCLE_CODE_ONE;
    }
    else
    {
        pstTransDrxParam->ucSplitPgCode = pstNvimDrxParam->ucSplitPgCode;
    }

     /*DRX Valude for S1 Mode��Ч��ֱ�Ӹ�ֵ����Ч����Э��ת��*/
    if (NAS_EMM_PARA_INVALID == NAS_LMM_IsUeDrxCycleValid(pstNvimDrxParam->ucPsDrxLen))
    {
        pstTransDrxParam->ucPsDrxLen = NAS_LMM_UE_NOT_SPEC_DRX_VALUE;
    }
    else
    {
        pstTransDrxParam->ucPsDrxLen    = pstNvimDrxParam->ucPsDrxLen;
    }

    return;
}



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

