/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : RFA.C
  �� �� ��   : ����
  ��    ��   : ���� 46160
  ��������   : 2008��2��1��
  ����޸�   :
  ��������   : RFУ׼ģ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2008��2��1��
    ��    ��   : ���� 46160
    �޸�����   : �����ļ�

******************************************************************************/


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
/*lint -e767 �޸��ˣ�m00128685�������ˣ�l46160��ԭ������������־�ļ���ID����*/
#define    THIS_FILE_ID        PS_FILE_ID_RFA_C
/*lint +e767 �޸��ˣ�m00128685�������ˣ�l46160*/


/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "rfa.h"
#include "om.h"

/* CQI��NV���ȡ�б� */ /* V3R3SFT ����ɾ������456��EEIC��صı� */
VOS_UINT16 g_ausCQINvIdReadList[]=
{
    en_NV_Item_W_HSPA_CQI_TABLE1,
    en_NV_Item_W_HSPA_CQI_TABLE2,
    en_NV_Item_W_HSPA_CQI_TABLE3,
};


/*****************************************************************************
 �� �� ��  : RFA_GetFixMemInfo
 ��������  : ���DSP������ڴ���Ϣ��ȡ��������������ַ
 �������  : ulMemType: �ڴ����ͣ��������ĺ궨��
             ulMemSize: �ڴ�ռ䣬����δʹ�ã���������̬�����ʱ����Ҫ
 �������  : pulMemAddr:�ڴ�ĵ�ַ
 �� �� ֵ  : VOS_OK/VOS_ERR
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��3��8��
    ��    ��   : zhuli
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 RFA_GetFixMemInfo(VOS_UINT32 ulMemType, VOS_UINT32 ulMemSize, VOS_UINT32 *pulMemAddr)
{
    BSP_DDR_SECT_QUERY  stDDRQuery;
    BSP_DDR_SECT_INFO   stDDRInfo;

    /*��д�ڴ�������Ϣ*/
    stDDRQuery.enSectType   = (BSP_DDR_SECT_TYPE_E)ulMemType;
    stDDRQuery.enSectAttr   = BSP_DDR_SECT_ATTR_NONCACHEABLE;
    stDDRQuery.enPVAddr     = BSP_DDR_SECT_PVADDR_EQU;
    stDDRQuery.ulSectSize   = ulMemSize;

    /*��ȡ��ǰ���ڴ���Ϣ*/
    if(BSP_OK != DRV_GET_FIX_DDR_ADDR(&stDDRQuery, &stDDRInfo))
    {
        PS_LOG1( UEPS_PID_APM, 0, PS_PRINT_ERROR," SHPA_GetMemInfoForDSP:DRV Get Mem %d Info Error.\n",(VOS_INT32)ulMemType);

        return VOS_ERR;     /*�쳣�˳�*/
    }

    *pulMemAddr = stDDRInfo.ulSectVirtAddr; /*������ʵ��ַһ�£���˷�����һ����*/

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : RFA_Init
 ��������  : RFAģ���ʼ��
 �������  : ��


 �������  : ��
 �� �� ֵ  :  VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ  :

*****************************************************************************/
VOS_UINT32 RFA_Init(VOS_VOID)
{
    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : RFA_MsgProc
 ��������  : RFAģ�����Ϣ������
 �������  : pMsg  ��Ϣ��


 �������  : ��
 �� �� ֵ  : �ɹ���ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ  :

*****************************************************************************/
VOS_UINT32 RFA_MsgProc(MsgBlock *pMsg)
{
    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : RFA_CQITableProc
 ��������  : RFAģ���CQI������
 �������  : ��


 �������  : ��
 �� �� ֵ  : �ɹ���ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ  :

*****************************************************************************/
VOS_UINT32 RFA_CQITableProc(VOS_VOID)
{
    VOS_UINT32                  i;
    VOS_UINT32                  ulMemAddr;
    VOS_UINT8                   *pucTemp;

    if(BSP_OK != RFA_GetFixMemInfo(BSP_DDR_SECT_TYPE_CQI, RFA_CQI_TOTAL_LENGTH, &ulMemAddr))
    {
        PS_LOG(WUEPS_PID_RFA, 0, PS_PRINT_ERROR, "RFA_CQITableProc:Get CQI Memory Fail.\n");

        return VOS_ERR;
    }

    pucTemp = (VOS_UINT8 *)ulMemAddr;    /*ARM_DSP_CQI_TBL_BASE_ADDR*/

    /* ��ȡCQI��NV ����뵽CQI���ڴ���*/
    /* EEICʹ�ܿ��ص�ֵusEEICMode��Ϊ�������±꣬0:��EEIC CQI�� 1:EEIC CQI ��*/
    for ( i = 0; i < sizeof(g_ausCQINvIdReadList)/sizeof(VOS_UINT16); i++)
    {

        if(NV_OK != NV_Read(g_ausCQINvIdReadList[i], pucTemp, RFA_CQI_TABLE_LENGTH))
        {
            PS_LOG1(WUEPS_PID_RFA, 0, PS_PRINT_ERROR, "RFA_CQITableProc: NV %d Read fail.\n"
                                    , g_ausCQINvIdReadList[i]);

            return VOS_ERR;
        }

        pucTemp += RFA_CQI_TABLE_LENGTH;

    }

    return VOS_OK;
}



/*****************************************************************************
 �� �� ��  : WuepsRfaPidInit
 ��������  :
 �������  : ��


 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ  :

*****************************************************************************/
VOS_UINT32 WuepsRfaPidInit ( enum VOS_INIT_PHASE_DEFINE ip )
{
    switch( ip )
    {
        case   VOS_IP_LOAD_CONFIG:

               RFA_Init();

               break;
        case   VOS_IP_FARMALLOC:
        case   VOS_IP_INITIAL:
        case   VOS_IP_ENROLLMENT:
        case   VOS_IP_LOAD_DATA:
        case   VOS_IP_FETCH_DATA:
        case   VOS_IP_STARTUP:
        case   VOS_IP_RIVAL:
        case   VOS_IP_KICKOFF:
        case   VOS_IP_STANDBY:
        case   VOS_IP_BROADCAST_STATE:
        case   VOS_IP_RESTART:
        case   VOS_IP_BUTT:
               break;
    }
    return VOS_OK;
}

#if 0
/********************************************************************
  Function:         RFA_AptNVListProc
  Description:      APT NV���ȡ
  Input:
  Output:
  Return:           VOS_OK/ VOS_ERR
  Others:
********************************************************************/
VOS_UINT32 RFA_AptNVListProc(VOS_UINT16* pusAPTNvIdList, RFA_APT_NV_DATA_STRU* pstAptNVItem)
{
    VOS_UINT32   i;
    VOS_UINT32   ulTempCount;
    VOS_INT16    asAPTCompHC[APT_NV_DATA_MAX_NUM];
    VOS_INT16    asAPTCompMC[APT_NV_DATA_MAX_NUM];
    VOS_INT16    asAPTCompLC[APT_NV_DATA_MAX_NUM];

    RFA_APT_NV_VBIAS_STRU stAptVbias;

    if(NV_OK != NV_Read((*pusAPTNvIdList++), &stAptVbias, sizeof(stAptVbias)))
    {
        PS_LOG1(WUEPS_PID_RFA, 0, PS_PRINT_ERROR, "RFA_AptNVListProc: NV %d Read fail.\n",*pusAPTNvIdList);

        return VOS_ERR;
    }

    if(NV_OK != NV_Read((*pusAPTNvIdList++), asAPTCompHC, sizeof(asAPTCompHC)))
    {
        PS_LOG1(WUEPS_PID_RFA, 0, PS_PRINT_ERROR, "RFA_AptNVListProc: NV %d Read fail.\n",*pusAPTNvIdList);

        return VOS_ERR;
    }

    if(NV_OK != NV_Read((*pusAPTNvIdList++), asAPTCompMC, sizeof(asAPTCompMC)))
    {
        PS_LOG1(WUEPS_PID_RFA, 0, PS_PRINT_ERROR, "RFA_AptNVListProc: NV %d Read fail.\n",*pusAPTNvIdList);

        return VOS_ERR;
    }

    if(NV_OK != NV_Read((*pusAPTNvIdList), asAPTCompLC, sizeof(asAPTCompLC)))
    {
        PS_LOG1(WUEPS_PID_RFA, 0, PS_PRINT_ERROR, "RFA_AptNVListProc: NV %d Read fail.\n",*pusAPTNvIdList);

        return VOS_ERR;
    }

    if (( APT_NV_DATA_MAX_NUM < stAptVbias.ulCount ) || ( APT_NV_DATA_MIN_NUM == stAptVbias.ulCount) )
    {
        PS_LOG(WUEPS_PID_RFA, 0, PS_PRINT_ERROR, "RFA_AptNVListProc: check NV fail.\n");

        return VOS_ERR;
    }

    ulTempCount = stAptVbias.ulCount;

    pstAptNVItem->ulCount = ulTempCount;

    /*NV���й��ʴӴ�С����ȡNV��������һ�η�ת*/
    for(i=0; i< ulTempCount; i++)
    {
        pstAptNVItem->asNvPower[i]   = stAptVbias.asNvItem[ulTempCount-1-i][0];
        pstAptNVItem->ausNvVolt[i]   = (VOS_UINT16)stAptVbias.asNvItem[ulTempCount-1-i][1];
        pstAptNVItem->asNvCompHC[i]  = asAPTCompHC[ulTempCount-1-i];
        pstAptNVItem->asNvCompMC[i]  = asAPTCompMC[ulTempCount-1-i];
        pstAptNVItem->asNvCompLC[i]  = asAPTCompLC[ulTempCount-1-i];
    }

    /*���ٽ�ֵ����ֵ*/
    pstAptNVItem->ausNvVolt[ulTempCount]   = (VOS_UINT16)stAptVbias.asNvItem[0][1];
    pstAptNVItem->asNvCompHC[ulTempCount]  = asAPTCompHC[0];
    pstAptNVItem->asNvCompMC[ulTempCount]  = asAPTCompMC[0];
    pstAptNVItem->asNvCompLC[ulTempCount]  = asAPTCompLC[0];

    return VOS_OK;
}

VOS_UINT32 RFA_ProcAptNvItem(VOS_INT16 *pItem, RFA_APT_NV_DATA_STRU stAptNVData , VOS_UINT32 ulGainMode )
{
    VOS_UINT32               i;
    VOS_UINT32               j;
    VOS_UINT32               ulCount;
    VOS_INT16                sTempOffset;
    VOS_INT16*               psAPTVbias;
    VOS_INT16*               psAPTHCComp;
    VOS_INT16*               psAPTMCComp;
    VOS_INT16*               psAPTLCComp;

    VOS_UINT16               ausNvPowerOffset[APT_NV_DATA_MAX_NUM+2];
    VOS_INT16                sPowerBase[] = {
                                                RFA_APT_Table_HIGH_GAIN_Base_P0,
                                                RFA_APT_Table_MID_GAIN_Base_P0,
                                                RFA_APT_Table_LOW_GAIN_Base_P0
                                            };

    /*����V16��V15��V14����V1��ӦAPT��ĵ�ַ��(P0+P16)/8��(P0+P15)/8��(P0+P3)/8��(P0+P1)/8*/
    ausNvPowerOffset[0] = 0;
    ulCount=stAptNVData.ulCount;

    for(i=0; i<ulCount; i++)
    {
        sTempOffset = stAptNVData.asNvPower[i]+sPowerBase[ulGainMode];

        /*��ƫ�������Ϸ��Լ��*/
        if(0 > sTempOffset )
        {
            return VOS_ERR;
        }

        /*��չ����Ϊ8  */
        ausNvPowerOffset[i+1]=(VOS_UINT16)sTempOffset>>3;
    }

    /*����ٽ����,,��ƫ�������������128������*/
    /*����ausNvPowerOffset�����ַ��0~128�����е�ַ*/
    if ( (RFA_APT_ITEM_SIZE) < ausNvPowerOffset[ulCount] )
    {
        ausNvPowerOffset[ulCount] = RFA_APT_ITEM_SIZE;
    }

    ausNvPowerOffset[ulCount+1] = RFA_APT_ITEM_SIZE;

    psAPTVbias  =  pItem;
    psAPTHCComp =  psAPTVbias + RFA_APT_GAIN_OFFSET;
    psAPTMCComp =  psAPTHCComp + RFA_APT_GAIN_OFFSET;
    psAPTLCComp =  psAPTMCComp + RFA_APT_GAIN_OFFSET;

    /*��ַ��0~(P0+P16)/8�ı�������V16����ַ��(P0+P16)/8~(P0+P15)/8*/
    /*�ı�������V15.�Դ����ƣ���ַ��(P0+P1)/8~127�ı�������V1*/
    for ( i = 0; i < ulCount+1; i++)
    {
        for ( j = ausNvPowerOffset[i]; j < ausNvPowerOffset[i+1]; j++ )
        {
            *(psAPTVbias+j)   = (VOS_INT16)stAptNVData.ausNvVolt[i];
            *(psAPTHCComp+j)  = stAptNVData.asNvCompHC[i];
            *(psAPTMCComp+j)  = stAptNVData.asNvCompMC[i];
            *(psAPTLCComp+j)  = stAptNVData.asNvCompLC[i];
        }

    }

    return VOS_OK;
}
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

