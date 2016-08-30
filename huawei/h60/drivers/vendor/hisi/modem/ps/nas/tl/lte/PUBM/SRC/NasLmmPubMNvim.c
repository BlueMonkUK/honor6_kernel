

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include    "NasLmmPubMInclude.h"
#include    "UsimPsInterface.h"
/* lihong00150010 ims begin */
#include    "SysNvId.h"
/* lihong00150010 ims begin */


/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASMMPUBMNVIM_C
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

VOS_UINT32                              g_aucNvimBuff[NAS_NVIM_MAX_BUFFER];

static NAS_LMM_NV_ACT_STRU               g_astNvDataMap[] =
{
    /*NVIM����*/
    {EN_NV_ID_UE_NET_CAPABILITY,    NAS_USIM_FILE_ID_INVALID,     NAS_LMM_DecodeOneNvItem,   NAS_LMM_EncodeOneNvItem},
    /*{EN_NV_ID_MS_NET_CAPABILITY,    NAS_USIM_FILE_ID_INVALID,     NAS_LMM_DecodeOneNvItem,   NAS_LMM_EncodeOneNvItem},*/
    {EN_NV_ID_IMSI,                 NAS_USIM_FILE_ID_INVALID,     NAS_LMM_DecodeOneNvItem,   NAS_LMM_EncodeOneNvItem},
    {EN_NV_ID_EPS_LOC,              NAS_USIM_FILE_ID_INVALID,     NAS_LMM_DecodeOneNvItem,   NAS_LMM_EncodeOneNvItem},
    {EN_NV_ID_SEC_CONTEXT,          NAS_USIM_FILE_ID_INVALID,     NAS_LMM_DecodeOneNvItem,   NAS_LMM_EncodeOneNvItem},
    {EN_NV_ID_UE_CENTER,            NAS_USIM_FILE_ID_INVALID,     NAS_LMM_DecodeOneNvItem,   NAS_LMM_EncodeOneNvItem},
	/* lihong00150010 ims begin */
    #if 0
    {EN_NV_ID_UE_CS_SERVICE,        NAS_USIM_FILE_ID_INVALID,     NAS_LMM_DecodeOneNvItem,   NAS_LMM_EncodeOneNvItem},
    #endif
    {en_NV_Item_UMTS_CODEC_TYPE,    NAS_USIM_FILE_ID_INVALID,     NAS_LMM_DecodeOneNvItem,   NAS_LMM_EncodeOneNvItem},
    {en_NV_Item_MED_CODEC_TYPE,    NAS_USIM_FILE_ID_INVALID,     NAS_LMM_DecodeOneNvItem,   NAS_LMM_EncodeOneNvItem},
	/* lihong00150010 ims end */
    {EN_NV_ID_UE_VOICE_DOMAIN,      NAS_USIM_FILE_ID_INVALID,     NAS_LMM_DecodeOneNvItem,   NAS_LMM_EncodeOneNvItem},
    {EN_NV_ID_NAS_RELEASE,          NAS_USIM_FILE_ID_INVALID,     NAS_LMM_DecodeOneNvItem,   NAS_LMM_EncodeOneNvItem},
    {EN_NV_ID_CONFIG_NWCAUSE,       NAS_USIM_FILE_ID_INVALID,     NAS_LMM_DecodeOneNvItem,   NAS_LMM_EncodeOneNvItem},

    /*SIM����*/
    {EN_NV_ID_EPS_LOC_FILE,         NAS_USIM_FILE_ID_EPS_LOC_INFO, NAS_LMM_DecodeSimPsLoc,     NAS_LMM_EncodeSimPsLoc},
    {EN_NV_ID_IMSI_FILE,            NAS_USIM_FILE_ID_IMSI,        NAS_LMM_DecodeSimImsi,       NAS_LMM_EncodeOneNvItem},
    {EN_NV_ID_SEC_CONTEXT_FILE,     NAS_USIM_FILE_ID_SEC_CONTEXT, NAS_LMM_DecodeSimSecContext, NAS_LMM_EncodeSimSecContext},
    {EN_NV_ID_ACC_CLASSMASK_FILE,   NAS_USIM_FILE_ID_AC_CLASS,    NAS_LMM_DecodeNvACC,         NAS_LMM_EncodeOneNvItem},

    /* ��USIM�Ĵ��� */
    {EN_NV_ID_USIM_END,             NAS_USIM_FILE_ID_BUTT,        NAS_LMM_DecodeSoftUsimCnf,    NAS_LMM_EncodeOneNvItem},
    /*OM����*/
};

VOS_UINT32 g_ulNvDataMapNum
            = sizeof(g_astNvDataMap)/sizeof(NAS_LMM_NV_ACT_STRU);


/* ����USIM��ȡ���ļ�ID */
VOS_UINT32 g_aulMmUsimEf[] = {  0x6F38, 0x6FAD, 0x6F7B, 0x6F31, 0x6F7E,
                                0x6F73, 0x6F07, 0x6F08, 0x6F09, 0x6FE4,
                                0x6F78};
VOS_UINT32 g_ulUsimSecuFileLen;


VOS_UINT8  g_aucUsimSecuContext[NAS_NVIM_SECU_CONTEXT_MAX_LEN] = {0}; /* �����ϴ�д���İ�ȫ���������� */


/*****************************************************************************
  3 Function
*****************************************************************************/
/*****************************************************************************
 Function Name  : NAS_LMM_NvimWrite
 Discription    : ����ģ����ñ�����дһ��ָ����NV_Item
 Input          : ulNVItemType: NV_Item����
*pData��NV������ָ��
                  usDataLen��Ҫд�����ݳ���
 Output         : None
 Return         : 0�����ݳɹ�д�룬������������
                  2��Ҫд��NV���ͳ������ֵ����
                  4��ϵͳ�쳣�������޷�д��
 History:
     1.sunbing   49683      2010-7-22  modify
*****************************************************************************/
/*lint -e960*/
/*lint -e961*/
VOS_UINT32  NAS_LMM_NvimWrite(   NAS_LMM_ITEM_TYPE_ENUM_UINT32    ulNVItemType,
                                VOS_VOID                        *pData,
                                VOS_UINT16                      usDataLen )
{
    VOS_UINT32                          ulRslt = NAS_LMM_NVIM_FAIL;
    NAS_NVIM_CTRL_TBL                   stNvCtrlTbl;
    VOS_UINT32                          i;
    NAS_LMM_NV_ACTION_FUN                pActFun = NAS_LMM_NULL_PTR;
    VOS_UINT16                          usEfId = 0;
    USIMM_SET_FILE_INFO_STRU            stSetFileInfo ={0};

    /*check input params*/
    if((NAS_NVIM_NULL_PTR == pData) ||
        (usDataLen == VOS_NULL) ||
        (ulNVItemType > EN_NV_ID_PS_END))
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_NvimWrite:input para ERR");
        return NAS_LMM_NVIM_FAIL;
    }

    /*Initalize NVIM ctrl tbl*/
    stNvCtrlTbl.pData = pData;
    stNvCtrlTbl.pNvData = g_aucNvimBuff;
    stNvCtrlTbl.usDataLen = usDataLen;
    stNvCtrlTbl.usNvDataLen = 0;

    /* ��g_astNvDataMap�в��Ҷ�Ӧ��NV TYPE*/
    for(i = 0; i < g_ulNvDataMapNum; i++)
    {
        /* ��NV������ͬ,���ҵ���,���ص�ǰ��NV�����������˳�ѭ�� */
        if(ulNVItemType == g_astNvDataMap[i].ulEventType)
        {
            pActFun             = g_astNvDataMap[i].pfEncodeActionFun;
            usEfId              = g_astNvDataMap[i].usUsimFileId;
            break;
        }
    }

    if (NAS_LMM_NULL_PTR == pActFun)
    {
        /* ����Ҳ�������������ʾ��ǰû�иô����� */
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_NvimWrite:Action functin is ERR");

        return NAS_LMM_NVIM_FAIL;
    }

    ulRslt = (*pActFun) (&stNvCtrlTbl);

    if ( NAS_LMM_NVIM_WRITTEN_UNNEEDED == ulRslt)
    {
        NAS_LMM_NVIM_LOG_INFO("NAS_LMM_NvimWrite: Don't need to write usim.");
        return NAS_LMM_NVIM_WRITTEN_UNNEEDED;
    }

    if(ulRslt != NAS_LMM_NVIM_OK)
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_NvimWrite:Encode is ERR");

        return NAS_LMM_NVIM_FAIL;
    }

    /*ӲUSIM�£�����NV ITEM����USIM�ķ�Χ������USIM��д�ļ�*/
    if((EN_NV_ID_USIM_BEGIN          <  ulNVItemType)
        &&(EN_NV_ID_USIM_END            >  ulNVItemType))
    {
        NAS_LMM_NVIM_LOG1_INFO("NAS_LMM_NvimWrite:USIMM_SetFileReq, ulNVItemType = ", ulNVItemType);

        /*ucRecordNumĬ��Ϊ1��NASĿǰ�漰���ļ��������ļ�����ֻ��һ����¼*/
        stSetFileInfo.enAppType    = USIMM_UMTS_APP;
        stSetFileInfo.ucRecordNum  = 1;
        stSetFileInfo.usEfId       = usEfId;
        stSetFileInfo.ulEfLen      = stNvCtrlTbl.usNvDataLen;
        stSetFileInfo.pucEfContent = stNvCtrlTbl.pNvData;

        /*
        ulRslt = USIMM_SetFileReq(      PS_USIM_CLIENT_LMM,
                                        usEfId,
                                       (VOS_UINT8*)stNvCtrlTbl.pNvData,
                                       (VOS_UINT8 )stNvCtrlTbl.usNvDataLen,
                                        ucRecordNum );*/

        ulRslt = USIMM_SetFileReq(      PS_PID_MM,
                                        0,
                                        &stSetFileInfo );

        NAS_LMM_NVIM_LOG3_INFO("usNvDataLen = , len = , usEfId = ",
                                        stNvCtrlTbl.usNvDataLen,
                                        stNvCtrlTbl.usNvDataLen,
                                        usEfId);
        NAS_COMM_PrintArray(NAS_COMM_GET_MM_PRINT_BUF(), (VOS_UINT8*)stNvCtrlTbl.pNvData,stNvCtrlTbl.usNvDataLen);

        return ulRslt;
    }

    /*���������NV��д��Ϣ*/
    /*lint -e718*/
    /*lint -e516*/
    /*lint -e732*/
    ulRslt = LPs_NvimItem_Write(     ulNVItemType,
                                    stNvCtrlTbl.pNvData,
                                    stNvCtrlTbl.usNvDataLen);
    /*lint +e732*/
    /*lint +e516*/
    /*lint +e718*/
    return ulRslt;

}

/*****************************************************************************
 Function Name  : NAS_LMM_NvimRead
 Discription    : ����ģ����ñ�������һ��ָ����NV_Item
 Input          : ulNVItemType: NV_Item����
                  *pData �����ݻ���ָ��
                  *pusDataLen���������ݳ���
 Output         : pusDataLen ���������ݳ���
 Return         : 0�����ݳɹ���ȡ��������������
                  1��δ�ҵ�Ҫ��ȡ��NV��
                  3������Ļ��治������
                  4��ϵͳ�쳣����ȡ����
 History:
      1.sunbing   49683      2010-7-22  modify
*****************************************************************************/
NAS_LMM_USIM_NV_RESULT  NAS_LMM_NvimRead(    NAS_LMM_ITEM_TYPE_ENUM_UINT32   ulNVItemType,
                                VOS_VOID                       *pData,
                                const VOS_UINT16                     *pusDataLen )
{
    VOS_UINT32                          ulRslt = NAS_LMM_NVIM_FAIL;
    NAS_NVIM_CTRL_TBL                   stNvCtrlTbl;
    VOS_UINT32                          i;
    NAS_LMM_NV_ACTION_FUN                pActFun = NAS_LMM_NULL_PTR;
    USIMM_GET_FILE_INFO_STRU            stGetFileInfo = {0};

    /*check input params*/
    if((NAS_NVIM_NULL_PTR == pData) ||
        (NAS_NVIM_NULL_PTR == pusDataLen) ||
        (ulNVItemType > EN_NV_ID_PS_END))
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_NvimRead :Input Para is error");
        return NAS_LMM_NVIM_FAIL;
    }

    NAS_LMM_MEM_SET(g_aucNvimBuff, 0, sizeof(g_aucNvimBuff));

    /*Initalize NVIM ctrl tbl*/
    stNvCtrlTbl.pData                   = pData;
    stNvCtrlTbl.pNvData                 = g_aucNvimBuff;
    stNvCtrlTbl.usDataLen               = *pusDataLen;
    stNvCtrlTbl.usNvDataLen             = *pusDataLen;

    /*ӲUSIM�£�����NV ITEM����USIM�ķ�Χ�����USIM�ж�ȡ�ļ�*/
    if((EN_NV_ID_USIM_BEGIN          <  ulNVItemType)
        &&(EN_NV_ID_USIM_END            >  ulNVItemType))
    {
        for(i = 0; i < g_ulNvDataMapNum; i++)
        {
            /* ��NV������ͬ,���ҵ���,���ص�ǰ��NV�����������˳�ѭ�� */
            if(ulNVItemType == g_astNvDataMap[i].ulEventType)
            {
                stGetFileInfo.usEfId      = g_astNvDataMap[i].usUsimFileId;
                break;
            }
        }

        if(NAS_USIM_FILE_ID_INVALID == stGetFileInfo.usEfId)
        {
            NAS_LMM_NVIM_LOG_INFO("NAS_LMM_NvimRead :Donot find Usim file ID.");

            return NAS_LMM_NVIM_FAIL;
        }

        /*ucRecordNumĬ��Ϊ1��NASĿǰ�漰���ļ��������ļ�����ֻ��һ����¼*/
        stGetFileInfo.ucRecordNum = 1;

        stGetFileInfo.enAppType = USIMM_UMTS_APP;
        /*
        ulRslt = Api_UsimGetFileReq(PS_USIM_CLIENT_MM,
                                    usEfId,
                                    ucRecordNum);
        */


        ulRslt = USIMM_GetFileReq(  PS_PID_MM,
                                    0,
                                    &stGetFileInfo);

        NAS_EMM_GET_SEND_COUNTER()++;

        /*��Ҫ����ʧ�ܣ����������ô����ַ���ʧ�ܺ󣬾Ͱ�ȫ�ֱ����ڴ���գ�
        �����������渳ֵ�������ĸ�ֵҪ�ȴ��յ�USIM��READ FILE��Ϣ�󣬲�����*/
        return NAS_LMM_NVIM_HARD_USIM;

    }

    /*���������NV�ж�ȡ��Ϣ*/
    /*lint -e718*/
    /*lint -e516*/
    /*lint -e732*/
    ulRslt = LPs_NvimItem_Read(      ulNVItemType,
                                    stNvCtrlTbl.pNvData,
                                    stNvCtrlTbl.usNvDataLen);
    /*lint +e732*/
    /*lint +e516*/
    /*lint +e718*/
    if(ulRslt != EN_NV_OK)
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_NvimRead:PS_NVIM_Read is fail");
        return NAS_LMM_NVIM_FAIL;
    }

    /* ��g_astNvDataMap�в��Ҷ�Ӧ��NV TYPE*/
    for(i = 0; i < g_ulNvDataMapNum; i++)
    {
        /* ��NV������ͬ,���ҵ���,���ص�ǰ��NV�����������˳�ѭ�� */
        if(ulNVItemType == g_astNvDataMap[i].ulEventType)
        {
           pActFun            = g_astNvDataMap[i].pfDecodeActionFun;
           break;
        }
    }

    if (NAS_LMM_NULL_PTR != pActFun)
    {
        ulRslt = (*pActFun) (&stNvCtrlTbl);
        if(NAS_LMM_NVIM_OK == ulRslt)
        {
            return NAS_LMM_NVIM_OK;
        }
        else
        {
            return NAS_LMM_NVIM_FAIL;
        }
    }
    else /* ����Ҳ�������������ʾ��ǰû�иô����� */
    {
        NAS_LMM_NVIM_LOG_ERR("NAS_LMM_NvimRead:Action functin is ERR");
        return NAS_LMM_NVIM_FAIL;
    }

}


VOS_UINT32  NAS_LMM_MsNullSsNullReadingUsimMsgUsimCnf( MsgBlock  * pMsg)
{
    PS_USIM_GET_FILE_CNF_STRU          *pstUsimCnf = NAS_LMM_NULL_PTR;
    VOS_UINT32                          ulRst;

    NAS_LMM_PUBM_LOG2_INFO("NAS_LMM_MsNullSsNullReadingUsimMsgUsimCnf: g_ulSendMsgCounter = ,NAS_EMM_GET_USIM_FILE_TYPE=",
                                        NAS_EMM_GET_SEND_COUNTER(),
                                        NAS_EMM_GET_USIM_FILE_TYPE());

    /* ״̬ƥ����,����ƥ��,�˳� */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_NULL, EMM_SS_NULL_WAIT_READING_USIM))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_LMM_MsNullSsNullReadingUsimMsgUsimCnf: STATE ERR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*�յ������*//*��ȫ�ֱ������б������Է����ָ���*/
    if (NAS_NVIM_SEND_COUNTER_NONE == NAS_EMM_GET_SEND_COUNTER())
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_LMM_MsNullSsNullReadingUsimMsgUsimCnf: Receive USIM cnf when Counter is 0!!!");
        return NAS_LMM_MSG_DISCARD;
    }
    else
    {
        NAS_EMM_GET_SEND_COUNTER() --;
    }

    pstUsimCnf = (PS_USIM_GET_FILE_CNF_STRU*)pMsg;


    NAS_LMM_PUBM_LOG1_WARN("NAS_LMM_MsNullSsNullReadingUsimMsgUsimCnf:===>",pstUsimCnf->ulResult);

    /*����ʧ��*/
    if (VOS_OK != pstUsimCnf->ulResult)
    {
        NAS_LMM_PUBM_LOG_WARN("NAS_LMM_MsNullSsNullReadingUsimMsgUsimCnf: Read file err !");


        if( NAS_NVIM_SEND_COUNTER_NONE == NAS_EMM_GET_SEND_COUNTER() )
        {/*��ѡ���һ���ļ���ȡʧ�ܣ����ж�NV��ת״̬�Ķ���*/

            NAS_LMM_DoAfterReadOptFileOver();
        }

        return NAS_LMM_MSG_HANDLED;
    }

    /* ���ļ����д��� */
    ulRst = NAS_LMM_ProcessHardUsimCnf(pstUsimCnf);
    if(NAS_LMM_NVIM_OK != ulRst)
    {
        NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_LMM_MsNullSsNullReadingUsimMsgUsimCnf : Decode USIM MSG err: ",
                                ulRst);
    }

    if( NAS_NVIM_SEND_COUNTER_NONE == NAS_EMM_GET_SEND_COUNTER())
    {   /* ����ض����ѡ�ļ� */

        NAS_LMM_DoAfterReadOptFileOver();
    }

    /* û�ж��꣬������ȴ�������Ӧ��Ϣ */

    return NAS_LMM_MSG_HANDLED;
}
VOS_VOID  NAS_LMM_DoAfterReadOptFileOver( VOS_VOID )
{
    NAS_LMM_FSM_STATE_STRU               EmmState;
    NAS_LMM_UE_OPERATION_MODE_ENUM_UINT32   ulLteUeMode;

    NAS_LMM_UEID_STRU                    *pstUeId;


    /* ֹͣTI_NAS_LMM_TIMER_WAIT_USIM_CNF */
    NAS_LMM_StopStateTimer(TI_NAS_LMM_TIMER_WAIT_USIM_CNF);
    /*�����ȡ����GUTI��Ч���ǰ�ȫ��������Ч����ɾ�����غ�NV�е�������*/
    pstUeId                             = NAS_LMM_GetEmmInfoUeidAddr();

    if((NAS_EMM_BIT_NO_SLCT == pstUeId->bitOpGuti)
        &&(EMM_CUR_SECU_NOT_EXIST!= NAS_LMM_GetSecuCurFsmCS()))
    {
        NAS_EMM_ClearCurSecuCntxt();
        NAS_LMM_WriteEpsSecuContext(NAS_NV_ITEM_DELETE);
    }

    /* Read NV Files*/
    NAS_LMM_ProcessNvData();

    /* ״̬ת�ɶ���ѡ�ļ�״̬ */
    /* V7�������ֱض���ѡ */
    /*NAS_EMM_GET_USIM_FILE_TYPE() = NAS_LMM_USIM_MANDATORY_FILE;*/
    /*PC REPLAY MODIFY BY LEILI END*/
    ulLteUeMode = NAS_LMM_ComputeLteUeMode( NAS_EMM_GetMsMode(),
                                          NAS_LMM_GetEmmInfoUeCenter());
    NAS_LMM_SetEmmInfoUeOperationMode(ulLteUeMode);
    /*PC REPLAY MODIFY BY LEILI END*/
    /* ����TI_NAS_EMM_WAIT_MMC_START_CNF_TIMER */
    NAS_LMM_StartStateTimer(TI_NAS_EMM_WAIT_MMC_START_CNF_TIMER);

    /* ת״̬ */
    EmmState.enFsmId             = NAS_LMM_PARALLEL_FSM_EMM;
    EmmState.enMainState         = EMM_MS_NULL;
    EmmState.enSubState          = EMM_SS_NULL_WAIT_MMC_START_CNF;
    EmmState.enStaTId            = TI_NAS_EMM_WAIT_MMC_START_CNF_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    /* ��MMC����MMC_EMM_START_REQ(��������)��Ϣ */
    (VOS_VOID)NAS_EMM_PLMN_SendMmcStartReq(EMMC_EMM_START_REQ_CAUSE_NORMAL_START);

    return;
}
VOS_VOID  NAS_LMM_ProcessNvData( VOS_VOID )
{
    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_NVIM_LOG_INFO("NAS_LMM_ProcessNvData is entered.");

    /*��ʼ��EMMȫ�ֱ���ReadNV*/
    NAS_EMM_ReadEmmNvim();

    /*NAS_LMM_UpdateNvImsi(NAS_LMM_GetEmmInfoUeidImsiAddr());*/
    /*��ȡMMC���NV*/
    /*NAS_EMMC_ReadMmcNvim();*/

    return;
}
VOS_UINT32  NAS_LMM_ProcessHardUsimCnf( PS_USIM_GET_FILE_CNF_STRU* pstUsimFileCnf)
{
    PS_USIM_GET_FILE_CNF_STRU           *pstUsimCnf = NAS_LMM_NULL_PTR;
    VOS_UINT32                          ulRslt = NAS_LMM_MSG_HANDLED;
    NAS_NVIM_CTRL_TBL                   stNvCtrlTbl;
    VOS_UINT32                          i;
    NAS_LMM_NV_ACTION_FUN                pActFun = NAS_LMM_NULL_PTR;

    pstUsimCnf = pstUsimFileCnf;

    /*Initalize NVIM ctrl tbl*/
    stNvCtrlTbl.pNvData                 = pstUsimCnf->aucEf;
    stNvCtrlTbl.usNvDataLen             = pstUsimCnf->usEfLen;

    NAS_LMM_PUBM_LOG2_INFO("NAS_LMM_ProcessHardUsimCnf:pstUsimCnf->usEfId=,pstUsimCnf->ucEfLen",
                                        pstUsimCnf->usEfId,
                                        pstUsimCnf->usEfLen);

    /* ��g_astNvDataMap�в��Ҷ�Ӧ��NV TYPE*/
    for(i = 0; i < g_ulNvDataMapNum; i++)
    {
        /* ��NV������ͬ,���ҵ���,���ص�ǰ��NV�����������˳�ѭ�� */
        if(pstUsimCnf->usEfId == g_astNvDataMap[i].usUsimFileId)
        {
           pActFun            = g_astNvDataMap[i].pfDecodeActionFun;
           break;
        }
    }

    if (NAS_LMM_NULL_PTR != pActFun)
    {
        ulRslt = (*pActFun) (&stNvCtrlTbl);
        return  ulRslt;
    }
    else
    {
        /* ����Ҳ�������������ʾ��ǰû�иô����� */
        NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_NvimRead: no fun deal with UsimFileId =  ",
                                        pstUsimCnf->usEfId);
        return  NAS_LMM_NVIM_FAIL;
    }

}


/*****************************************************************************
 Function Name   : NAS_LMM_ReadMmcUsimFile
 Description     : ����USIM�ļ���MMC��ȡ�ģ�LMM��MMC�������LMM����
                   ��V1R1�Ƚϣ���Щ�ļ���V7�в��ٶ�:
                        6F62    MMC_READ_HPLMN_SEL_FILE_ID
                        6F7B    MMC_READ_FORBIDDEN_PLMN_LIST_FILE_ID
                        6F60    MMC_READ_UPLMN_SEL_FILE_ID
                        6F61    MMC_READ_OPLMN_SEL_FILE_ID
                        6F31    MMC_READ_HPLMN_PERI_FILE_ID
                        6FAD    MMC_READ_MNC_LENGTH_FILE_ID
                        6FDC    Last RPLMN Selection Indication

 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.HanLufeng 41410      2011-2-23  Draft Enact

*****************************************************************************/
NAS_LMM_USIM_READ_RST_ENUM_UINT32  NAS_LMM_ReadMmcUsimFile(VOS_VOID)
{


    VOS_UINT32                          ulRst;
    PS_USIM_GET_FILE_CNF_STRU          *pstUsimCnfMsg;

    pstUsimCnfMsg = (PS_USIM_GET_FILE_CNF_STRU *)(g_aucNvimBuff);

    /*PC REPLAY MODIFY BY LEILI BEGIN*/
    /* 6F07    MMC_READ_IMSI_FILE_ID ��USIM���ṩ�Ľӿں��� */
    ulRst = NAS_EMM_GetCardIMSI((VOS_UINT8 *)(pstUsimCnfMsg->aucEf));
    /*PC REPLAY MODIFY BY LEILI END*/
    if(USIMM_API_SUCCESS != ulRst)
    {
        NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_LMM_ReadMmcUsimFile : read IMSI err: ",
                                  ulRst);
        return  NAS_LMM_USIM_READ_HARD_IMSI_ERR;
    }
    else
    {
        /* ��ӡ��USIM������ԭʼ���� */
        NAS_LMM_PUBM_LOG_NORM("\n");
        NAS_LMM_PUBM_LOG_NORM("====== USIMM_GetCardIMSI: IMSI:  ======");
        NAS_COMM_PrintArray(             NAS_COMM_GET_MM_PRINT_BUF(),
                                         pstUsimCnfMsg->aucEf,
                                         NAS_LMM_NVIM_IMSI_FILE_LEN);
        NAS_LMM_PUBM_LOG_NORM("\n");

        /* ׼��һ�� NAS_LMM_ProcessHardUsimCnf ����ڲ��� */
        pstUsimCnfMsg->usEfId   = NAS_USIM_FILE_ID_IMSI;
        pstUsimCnfMsg->usEfLen  = NAS_LMM_NVIM_IMSI_FILE_LEN;
        pstUsimCnfMsg->ucRecordNum = 0;
        pstUsimCnfMsg->ucTotalNum  = 0;

        /* ���� */
        ulRst = NAS_LMM_ProcessHardUsimCnf(pstUsimCnfMsg);
        if(NAS_LMM_NVIM_OK != ulRst)
        {
            NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_LMM_ReadMmcUsimFile : Decode IMSI err: ", ulRst);
            return  NAS_LMM_USIM_READ_HARD_IMSI_ERR;
        }
    }

    return  NAS_LMM_USIM_READ_SUCC;


    /* ���ٶ�ȡ :   6F38    MMC_READ_UST_FILE_ID */
}


/*****************************************************************************
 Function Name   : NAS_LMM_ReadLteUsimFile
 Description     : ��ȡLTE�Լ��ĸ��ļ�:
                    6FE3    NAS_USIM_FILE_ID_EPS_LOC_INFO
                    6FE4    NAS_USIM_FILE_ID_SEC_CONTEXT
                    ��ȫ�������ļ���GUTIӦ����ͬʱ���ڵģ���GUTI��Ӧ���а�ȫ����
                    ��ȫ����Ӧ����GUTI;

 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.HanLufeng 41410      2011-2-23  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_LMM_ReadLteUsimFile( VOS_VOID )
{

    VOS_UINT32 ulRslt;
    VOS_UINT32 ulReadHardUsimFileNum = NAS_LMM_SERTAB_SUPPORT_NONE;

    /*6F78    NAS_USIM_FILE_ID_AC_CLASS */
    ulRslt = NAS_EMM_ReadAccClassMask();
    if (NAS_LMM_ERR_CODE_HARD_USIM == ulRslt)
    {   /* ����ֵ  ��ζ�Ŷ�ӲSUIM�� */

        ulReadHardUsimFileNum++;
    }


    /*6FE3 ��ȫ�����ĺ� PS_LOC*/
    ulRslt = NAS_LMM_ReadUsimEmmInfoFile();
    if(NAS_LMM_ERR_CODE_HARD_USIM == ulRslt)
    {   /* ����ֵ NAS_LMM_SIM_SRV_AVAILABLE ��ζ�Ŷ�ӲSUIM�� */

        ulReadHardUsimFileNum++;
        ulReadHardUsimFileNum++;
    }

    
    NAS_LMM_PUBM_LOG1_INFO("NAS_LMM_ReadUsimOptFile: ulReadHardUsimFileNum = ",
                                        ulReadHardUsimFileNum);
    

    return;

}
/*lint +e961*/
/*lint +e960*/

#ifdef __cplusplus
    #if __cplusplus
            }
    #endif
#endif


