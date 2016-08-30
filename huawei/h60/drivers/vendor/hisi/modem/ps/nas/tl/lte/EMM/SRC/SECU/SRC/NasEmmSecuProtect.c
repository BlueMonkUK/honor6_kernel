


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include    "NasEmmSecuInclude.h"
#include    "NasLmmPubMPrint.h"


/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASEMMSECUPROTECT_C
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

/*״̬�¼�������
typedef VOS_UINT32 ( * NAS_LMM_ACTION_FUN )
(
    VOS_UINT32, ��ȫͷ����
    VOS_VOID *  RRC_NAS_MSG_STRU���͵�����������: CN��Ϣ���Ⱥ�CN����Ϣ����
);
*/

/* EMM_CUR_SECU_NOT_EXIST BEGIN ===================================*/
/*================= EMM_NEW_SECU_NOT_EXIST ========================*/
NAS_EMM_ACT_STRU    g_astEmmCurNoExistNewNotExistTable[] =
{
    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_PLAIN,
                        NAS_EMM_CurNotActiveNewAnyMsgPlain),

    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_INTEGRITY,
                        NAS_EMM_CurNotActiveNewAnyMsgInt),

    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_NEW_KEY_INTEGRITY,
                        NAS_EMM_CurNotExistNewNotExistMsgSmc)
};
/*================= EMM_NEW_SECU_EXIST ========================*/
NAS_EMM_ACT_STRU    g_astEmmCurNoExistNewExistTable[] =
{
    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_PLAIN,
                        NAS_EMM_CurNotActiveNewAnyMsgPlain),

    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_INTEGRITY,
                        NAS_EMM_CurNotActiveNewAnyMsgInt),

    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_NEW_KEY_INTEGRITY,
                        NAS_EMM_CurAnyNewExistMsgSmc)
};

/* EMM_CUR_SECU_EXIST_NOT_ACTIVE BEGIN ===================================*/
/*================= EMM_NEW_SECU_NOT_EXIST ========================*/
NAS_EMM_ACT_STRU    g_astEmmCurExistNotActiveNewNotExistTable[] =
{
    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_PLAIN,
                        NAS_EMM_CurNotActiveNewAnyMsgPlain),

    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_INTEGRITY,
                        NAS_EMM_CurNotActiveNewAnyMsgInt),

    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_INTEGRITY_CIPHERED,
                        NAS_EMM_CurExistNotAcitveNewAnyMsgIntEnc),

    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_NEW_KEY_INTEGRITY,
                        NAS_EMM_OnlyCurExistMsgSmc)
};
/*================= EMM_NEW_SECU_EXIST ============================*/
NAS_EMM_ACT_STRU    g_astEmmCurExistNotActiveNewExistTable[] =
{
    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_PLAIN,
                        NAS_EMM_CurNotActiveNewAnyMsgPlain),

    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_INTEGRITY,
                        NAS_EMM_CurNotActiveNewAnyMsgInt),

    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_INTEGRITY_CIPHERED,
                        NAS_EMM_CurExistNotAcitveNewAnyMsgIntEnc),

    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_NEW_KEY_INTEGRITY,
                        NAS_EMM_CurAnyNewExistMsgSmc)
};

/* EMM_CUR_SECU_EXIST_ACTIVE BEGIN ===================================*/
/*================= EMM_NEW_SECU_NOT_EXIST ========================*/
NAS_EMM_ACT_STRU    g_astEmmCurExistActiveNewNotExistTable[] =
{
    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_PLAIN,
                        NAS_EMM_CurExistActiveNewAnyMsgPlain),

    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_INTEGRITY_CIPHERED,
                        NAS_EMM_CurExistActiveNewAnyMsgIntEnc),

    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_NEW_KEY_INTEGRITY,
                        NAS_EMM_OnlyCurExistMsgSmc)
};
/*================= EMM_NEW_SECU_EXIST ============================*/
NAS_EMM_ACT_STRU    g_astEmmCurExistActiveNewExistTable[] =
{
    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_PLAIN,
                        NAS_EMM_CurExistActiveNewAnyMsgPlain),

    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_INTEGRITY_CIPHERED,
                        NAS_EMM_CurExistActiveNewAnyMsgIntEnc),

    NAS_EMM_SECU_ACT_TBL_ITEM(
                        NAS_EMM_SECU_HEAD_TYPE_NEW_KEY_INTEGRITY,
                        NAS_EMM_CurAnyNewExistMsgSmc)
};

/* ״̬ת�Ʊ� */
NAS_EMM_STA_STRU     g_astEmmSecuStaTbl[] =
{
/*================= EMM_CUR_SECU_NOT_EXIST ״̬���������� ========*/
    NAS_LMM_STA_TBL_ITEM(EMM_CUR_SECU_NOT_EXIST,
                        EMM_NEW_SECU_NOT_EXIST,
                        g_astEmmCurNoExistNewNotExistTable),

    NAS_LMM_STA_TBL_ITEM(EMM_CUR_SECU_NOT_EXIST,
                        EMM_NEW_SECU_EXIST,
                        g_astEmmCurNoExistNewExistTable),

/*================= EMM_CUR_SECU_EXIST_NOT_ACTIVE ״̬���������� ========*/
    NAS_LMM_STA_TBL_ITEM(EMM_CUR_SECU_EXIST_NOT_ACTIVE,
                        EMM_NEW_SECU_NOT_EXIST,
                        g_astEmmCurExistNotActiveNewNotExistTable),

    NAS_LMM_STA_TBL_ITEM(EMM_CUR_SECU_EXIST_NOT_ACTIVE,
                        EMM_NEW_SECU_EXIST,
                        g_astEmmCurExistNotActiveNewExistTable),

/*================= EMM_CUR_SECU_EXIST_ACTIVE ״̬���������� ========*/
    NAS_LMM_STA_TBL_ITEM(EMM_CUR_SECU_EXIST_ACTIVE,
                        EMM_NEW_SECU_NOT_EXIST,
                        g_astEmmCurExistActiveNewNotExistTable),

    NAS_LMM_STA_TBL_ITEM(EMM_CUR_SECU_EXIST_ACTIVE,
                        EMM_NEW_SECU_EXIST,
                        g_astEmmCurExistActiveNewExistTable),


};

/* TAU��Ϣ���ܿ��أ�0Ϊ���ܣ�1Ϊ�����ܡ�Ĭ��Ϊ������ */
VOS_UINT32 g_ulTauCipheredFlag = 1;

/* ��ȫ��������ܷ�������REJ�տ���Ϣ: 0Ϊ���ܴ���1Ϊ�ܴ���Ĭ��Ϊ������ */
VOS_UINT32  g_ulNasPlainRejMsgFlag  = 0;

/*****************************************************************************
  3 Function
*****************************************************************************/


VOS_UINT32  NAS_LMM_SECU_FsmErr(         VOS_UINT32           ulMsgId,
                                        const VOS_VOID      *PMsg)
{
    (VOS_VOID)ulMsgId;
    (VOS_VOID)PMsg;

    return NAS_EMM_SECU_UNPROT_FAIL;
}


VOS_VOID  NAS_LMM_SECU_FsmReg(VOS_VOID)
{
    VOS_UINT32                          ulRslt;

    ulRslt                              = NAS_LMM_FAIL;

    /* ע��״̬�� */
    ulRslt = NAS_LMM_FsmRegisterFsm(NAS_LMM_PARALLEL_FSM_SECU,
                                   (sizeof(g_astEmmSecuStaTbl)/sizeof(NAS_LMM_STA_STRU)),
                                   g_astEmmSecuStaTbl,
                                   NAS_LMM_SECU_FsmErr);
    if (ulRslt != NAS_LMM_SUCC)
    {
        NAS_EMM_SECU_LOG_ERR("NAS_LMM_SECU_FsmReg:ERROR:register fail!");
    }
    else
    {
        NAS_EMM_SECU_LOG_INFO("NAS_LMM_SECU_FsmReg!");
    }

    return;
}
VOS_VOID  NAS_EMM_SecuStateConvert(NAS_EMM_MAIN_STATE_ENUM_UINT16 usCurSta,
                                   NAS_EMM_SUB_STATE_ENUM_UINT16 usNewSta )
{
    NAS_EMM_FSM_STATE_STRU              stDestState;

    stDestState.enFsmId                 = NAS_LMM_PARALLEL_FSM_SECU;
    stDestState.enMainState             = usCurSta;
    stDestState.enSubState              = usNewSta;
    stDestState.enStaTId                = NAS_LMM_STATE_TI_BUTT;

    /*״̬��Ǩ��*/
    NAS_LMM_FsmTranState(stDestState);
    /*NAS_LMM_StaTransProc(stDestState);*/
}


VOS_BOOL NAS_EMM_IsSpecNasMsg(          const VOS_UINT8     *pucRcvNasMsg,
                                        VOS_UINT32          *pulNasMsgSize)
{
    VOS_UINT8                           ucEmmMsgType = 0;
    VOS_UINT8                           ucIdenTypeId = 0;
    NAS_EMM_CN_CAUSE_ENUM_UINT8         ucRejCauseVal = NAS_LMM_CAUSE_NULL;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_IsSpecNasMsg entered.");
    (VOS_VOID)pulNasMsgSize;

    /*�������EMM����Ϣ��������Ȩ��Ϣ*/
    if(EMM_CN_MSG_PD_EMM != pucRcvNasMsg[NAS_EMM_SECU_CN_MSG_HEAD_POS])
    {
        NAS_EMM_SECU_LOG_INFO("Nas_Emm_IsSpcNasMsg: NOT EMM MSG.");
        return VOS_FALSE;
    }

    /*��ȡ�տ���Ϣ����*/
    ucEmmMsgType = pucRcvNasMsg[NAS_EMM_SECU_CN_MSG_TYPE_POS];

    /*24301-821-4.4.4.2:�ж�NAS��Ȩ��Ϣ*/
    switch(ucEmmMsgType)
    {
        case NAS_EMM_CN_MT_AUTH_REQ:
        case NAS_EMM_CN_MT_AUTH_REJ:
        case NAS_EMM_CN_MT_DETACH_ACP_MO:
            return VOS_TRUE;

        /*ATTACH/TAU/SERVICE REJ #25������Ȩ��Ϣ������ԭ��ֵ����Ϊ��Ȩ��Ϣ����*/
        case NAS_EMM_CN_MT_ATTACH_REJ:
        case NAS_EMM_CN_MT_TAU_REJ:
        case NAS_EMM_CN_MT_SER_REJ:
            ucRejCauseVal = pucRcvNasMsg[NAS_EMM_SECU_REJ_CAUSE_POS];
            if(NAS_LMM_CAUSE_NOT_AUTHORIZED_FOR_THIS_CSG == ucRejCauseVal)
            {
                NAS_EMM_SECU_LOG_INFO("Nas_Emm_IsSpcNasMsg: REJ #25������Ȩ��Ϣ.");
                return VOS_FALSE;
            }
            else
            {
                return VOS_TRUE;
            }

        case NAS_EMM_CN_MT_IDEN_REQ:
            ucIdenTypeId = pucRcvNasMsg[NAS_EMM_SECU_IDEN_REQ_TYPE_POS]
                         & NAS_EMM_LOW_3_BITS_F;
            if(NAS_EMM_MOBILE_ID_IMSI == ucIdenTypeId)
            {
                return VOS_TRUE;
            }
            else
            {
                NAS_EMM_SECU_LOG_INFO("Nas_Emm_IsSpcNasMsg: IDEN REQ���ǲ�ѯIMSI��������Ȩ��Ϣ.");
                return VOS_FALSE;
            }

        default:
            return VOS_FALSE;
    }
}


VOS_UINT32    NAS_EMM_CompDlNasCount(
                                        VOS_UINT32  ulLocDlNasCount,
                                        VOS_UINT8   ucRcvSn )
{
    VOS_UINT8                       ucLocDlCountLowByte;
    VOS_UINT32                      ulDlForUnprot = 0;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_CompDlNasCount enterd.");

    /* ��ȡ���� NAS DL COUNT�ĵ��ֽ� */
    ulDlForUnprot = ulLocDlNasCount;

    /* ��ȡ Local_DlNascount������ֽ�:SN*/
    ucLocDlCountLowByte     = ulDlForUnprot & NAS_EMM_SECU_MASK_NAS_COUNT_SQN;

    /*DL_NASCOUNT������ת*/
    if(ucRcvSn < ucLocDlCountLowByte)
    {
        ulDlForUnprot += NAS_EMM_SECU_COUNT_FLOW_OFFSET;
    }

    /* �������ֽ� */
    ulDlForUnprot &= NAS_EMM_SECU_MASK_OVERFLOW_COUNT;

    /* ����ֽڸ�ֵΪ�յ���SN */
    ulDlForUnprot += ucRcvSn;

    return ulDlForUnprot;
}
/*lint -e960*/
/*lint -e961*/
VOS_UINT32  NAS_EMM_SecuIntCheck( const NAS_EMM_PROT_PARAM_STRU   *pstUnProtParam)
{
/*PC����:�����ϢЯ����MAC����aucPcMac,��Ϊ�����Լ��ʧ��*/
#if(VOS_WIN32 == VOS_OS_VER)
    #ifdef PS_ITT_PC_TEST_NAS_ST
    VOS_UINT8                           aucPcMac[] = {0x5D, 0x22, 0x11, 0x02};
    #else
    VOS_UINT8                           aucPcMac[] = {0x02, 0x11, 0x22, 0x5D};
    #endif
    VOS_UINT8                          *pucMacOfMsg = VOS_NULL_PTR;

    pucMacOfMsg = &(pstUnProtParam->pucMsg[NAS_EMM_SECU_MAC_POS]);
    if(NAS_EMM_SECU_MEM_CMP_EQUAL !=  NAS_LMM_MEM_CMP(pucMacOfMsg,
                                                     aucPcMac,
                                                     NAS_EMM_SECU_MAC_LEN))
    {
        return NAS_EMM_SECU_FAIL;
    }

/* ���廷��*/
#else
     CIHPER_SINGLE_CFG_S                 stSingleSecuPara;
     VOS_INT32                           lRslt;
     VOS_UINT8                          *pucSrcAddr = VOS_NULL_PTR;
     VOS_UINT8                          *pucDstAddr = VOS_NULL_PTR;

     /* ���������Կ��㷨����ȥ����ȫͷ��Ȼ�󷵻� */
     if(PS_SECU_ALG_NULL == pstUnProtParam->ucSecuALg)
     {
         NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuIntCheck: ALG is NULL_ALG.");

         NAS_EMM_SecuDelSecuHead(pstUnProtParam->pucMsg,pstUnProtParam->pulMsgSize);

         return  NAS_EMM_SECU_SUCC;
     }

     /* �����Լ�飬������ֵ*/
     NAS_LMM_MEM_SET(&stSingleSecuPara, 0, sizeof(CIHPER_SINGLE_CFG_S));

     stSingleSecuPara.enOpt = CIPHER_OPT_CHECK_PRE_LMAC;
     stSingleSecuPara.u8Direction = NAS_EMM_SECU_DNLINK;
     stSingleSecuPara.u32Count = pstUnProtParam->ulNasCount;
     stSingleSecuPara.u32BlockLen = (*pstUnProtParam->pulMsgSize)
                                             - NAS_EMM_SECU_HEADER_LEN;
     stSingleSecuPara.stAlgKeyInfo.u32KeyIndexInt = LTE_SECU_NAS_INTEGRITY_KEY_INDEX;


     stSingleSecuPara.stAlgKeyInfo.enAlgIntegrity
                                = NAS_EMM_GetIntegrityAlg(pstUnProtParam->ucSecuALg);

     stSingleSecuPara.u8BearId = NAS_EMM_SECU_SECURITY_BEARERID;
     stSingleSecuPara.bMemBlock = PS_TRUE;
     stSingleSecuPara.enAppdHeaderLen = CIPHER_HDR_BIT_TYPE_0;
     stSingleSecuPara.enHeaderLen = CIPHER_HDR_BIT_TYPE_0;

     pucSrcAddr = NAS_EMM_GetSecuSrcSpace();
     NAS_LMM_MEM_CPY( pucSrcAddr,
                    &(pstUnProtParam->pucMsg[NAS_EMM_SECU_MAC_POS]),
                     stSingleSecuPara.u32BlockLen);
     pucDstAddr = NAS_EMM_NULL_PTR;

     /*��ӡCipher����*/
     NAS_EMM_SecuPrintCipherPara(&stSingleSecuPara, pucSrcAddr);

     /*�ڵ��� BSP �ӿ�֮ǰˢ�� cacheԴ��ַ�ռ䣬���cacheĿ�Ŀռ�*/
     LMM_DATA_CACHE_FLUSH(pucSrcAddr, stSingleSecuPara.u32BlockLen);

     /* ����BSP�ӿڽⱣ��*/
     lRslt = NAS_CIPHER_SingleSubmitTask(pucSrcAddr, pucDstAddr, &stSingleSecuPara);
     if (CIPHER_SUCCESS != lRslt)
     {
         NAS_EMM_SECU_LOG1_ERR("NAS_EMM_SecuIntCheck:BSP_CIPHER_SingleSubmitTask fail.lRslt = ",
                                  lRslt);
         return NAS_EMM_SECU_FAIL;
     }

    /*���廷�������Լ����ȷ����Ҫ������MACͷȥ��*/

#endif

    NAS_EMM_SecuDelSecuHead(pstUnProtParam->pucMsg,pstUnProtParam->pulMsgSize);

    return NAS_EMM_SECU_SUCC;
}


VOS_UINT32  NAS_EMM_SecuDecrypt(const NAS_EMM_PROT_PARAM_STRU   *pstUnProtParam)
{

/* ���廷����PC����������Ĳ��� */
#if(VOS_WIN32 != VOS_OS_VER)
    CIHPER_SINGLE_CFG_S                 stSingleSecuPara;
    VOS_INT32                           lRslt;
    VOS_UINT8                          *pucSrcAddr = VOS_NULL_PTR;
    VOS_UINT8                          *pucDstAddr = VOS_NULL_PTR;

    /* ���ǿ��㷨����ֱ�ӷ��� */
    if(PS_SECU_ALG_NULL == pstUnProtParam->ucSecuALg)
    {
        NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuDecrypt: ALG is NULL_ALG.");
        return  NAS_EMM_SECU_SUCC;
    }

    /* ��Ϣ���ܣ�������ֵ*/
    NAS_LMM_MEM_SET(&stSingleSecuPara, 0, sizeof(CIHPER_SINGLE_CFG_S));

    stSingleSecuPara.enOpt = CIPHER_OPT_DECRYPT;
    stSingleSecuPara.u8Direction = NAS_EMM_SECU_DNLINK;
    stSingleSecuPara.u32Count = pstUnProtParam->ulNasCount;
    stSingleSecuPara.u32BlockLen = *(pstUnProtParam->pulMsgSize);
    stSingleSecuPara.stAlgKeyInfo.u32KeyIndexSec = LTE_SECU_NAS_CIPHER_KEY_INDEX;

    stSingleSecuPara.stAlgKeyInfo.enAlgSecurity
                                = NAS_EMM_GetSecurityAlg(pstUnProtParam->ucSecuALg);
    stSingleSecuPara.u8BearId = NAS_EMM_SECU_SECURITY_BEARERID;
    stSingleSecuPara.bMemBlock = PS_TRUE;
    stSingleSecuPara.enAppdHeaderLen = CIPHER_HDR_BIT_TYPE_0;
    stSingleSecuPara.enHeaderLen = CIPHER_HDR_BIT_TYPE_0;

    pucSrcAddr = NAS_EMM_GetSecuSrcSpace();
    NAS_LMM_MEM_CPY(                     pucSrcAddr,
                                        pstUnProtParam->pucMsg,
                                       *(pstUnProtParam->pulMsgSize));
    pucDstAddr = NAS_EMM_GetSecuDstSpace();

    /*��ӡCipher����*/
    NAS_EMM_SecuPrintCipherPara(&stSingleSecuPara, pucSrcAddr);

    /*�ڵ��� BSP �ӿ�֮ǰˢ�� cacheԴ��ַ�ռ䣬���cacheĿ�Ŀռ�*/
    LMM_DATA_CACHE_FLUSH(pucSrcAddr, (stSingleSecuPara.u32BlockLen));

    /* ����BSP�ӿڽⱣ��*/
    lRslt = NAS_CIPHER_SingleSubmitTask(pucSrcAddr, pucDstAddr, &stSingleSecuPara);
    if (CIPHER_SUCCESS != lRslt)
    {
        NAS_EMM_SECU_LOG1_ERR("NAS_EMM_SecuDecrypt:BSP_CIPHER_SingleSubmitTask fail.lRslt = ",
                                 lRslt);
        return NAS_EMM_SECU_FAIL;
    }

    LMM_DATA_CACHE_INVALID(pucDstAddr, (stSingleSecuPara.u32BlockLen));

    /*�����Ŀ�����Dopra��Ϣ */
    *(pstUnProtParam->pulMsgSize)       = stSingleSecuPara.u32BlockLen;
    NAS_LMM_MEM_CPY(                     pstUnProtParam->pucMsg,
                                        NAS_EMM_GetSecuDstSpace(),
                                        stSingleSecuPara.u32BlockLen);

#endif

    return NAS_EMM_SECU_SUCC;
}
VOS_VOID  NAS_EMM_SecuDelSecuHead(VOS_UINT8          *pucRcvCnNasMsg,
                                 VOS_UINT32         *pulNasMsgSize)
{
    /*��ȡ������Ϣ*/
    *pulNasMsgSize = *pulNasMsgSize - NAS_EMM_SECU_MSG_HEAD_LEN;
    NAS_LMM_MEM_CPY(pucRcvCnNasMsg,
                    &pucRcvCnNasMsg[NAS_EMM_SECU_PLAIN_MSG_POS],
                    *pulNasMsgSize);
    return;
}
VOS_UINT32  NAS_EMM_IntEncMsgCommProc( LRRC_LNAS_MSG_STRU *pstNasMsg )
{

    VOS_UINT32                          ulRslt = NAS_EMM_SECU_FAIL;
    VOS_UINT32                          ulDlForUnprot = 0;
    NAS_EMM_PROT_PARAM_STRU             stUnProtParam;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_IntEncMsgCommProc entered.");

    /*����DL_NASCOUNT*/
    /*lint -e416 -e415*/
    ulDlForUnprot = NAS_EMM_CompDlNasCount(NAS_EMM_GetSecuDlNasCount(),
                                 pstNasMsg->aucNasMsg[NAS_EMM_SECU_MSG_SN_POS]);
    /*lint +e416 +e415*/

    /*��д�����Լ����Ҫ�Ĳ���*/
    stUnProtParam.ulNasCount            = ulDlForUnprot;
    stUnProtParam.ucSecuALg             = NAS_EMM_GetSecuEia();
    /*stUnProtParam.pucNasKey             = NAS_EMM_GetSecuCurNasIkAddr();*/
    stUnProtParam.pucMsg                = pstNasMsg->aucNasMsg;
    stUnProtParam.pulMsgSize            = &(pstNasMsg->ulNasMsgSize);

    /*�����Լ��*/
    ulRslt = NAS_EMM_SecuIntCheck(&stUnProtParam);
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SECU_LOG_NORM("NAS_EMM_IntEncMsgCommProc: Int Check Fail.");
        return NAS_EMM_SECU_FAIL;
    }

    /*�����Լ��ͨ��������ܱ���DL_NASCOUNT*/
    NAS_EMM_GetSecuDlNasCount() = ulDlForUnprot;

    /*��д������Ҫ�Ĳ���,*/
    stUnProtParam.ulNasCount            = ulDlForUnprot;
    stUnProtParam.ucSecuALg             = NAS_EMM_GetSecuEca();
    /*stUnProtParam.pucNasKey             = NAS_EMM_GetSecuCurNasCkAddr();*/
    stUnProtParam.pucMsg                = pstNasMsg->aucNasMsg;
    stUnProtParam.pulMsgSize            = &(pstNasMsg->ulNasMsgSize);

    /*����*/
    ulRslt = NAS_EMM_SecuDecrypt(&stUnProtParam);
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        NAS_EMM_SECU_LOG_NORM("NAS_EMM_IntEncMsgCommProc: Decrypt Fail.");
        return NAS_EMM_SECU_FAIL;
    }

    return NAS_EMM_SECU_SUCC;
}


NAS_EMM_SECU_UNPROT_RSLT_ENUM_UINT32  NAS_EMM_CurNotActiveNewAnyMsgPlain(
                                        VOS_UINT32  ulSecuHeadType,
                                        VOS_VOID   *pulRrcNasMsg)
{
    VOS_BOOL                            bRlst =  VOS_FALSE;
    LRRC_LNAS_MSG_STRU                   *pstNasMsg = VOS_NULL_PTR;

    (VOS_VOID)ulSecuHeadType;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_CurNotActiveNewAnyMsgPlain enterd.");

    pstNasMsg = (LRRC_LNAS_MSG_STRU *)pulRrcNasMsg;

    /*�ж��ǲ�����Ȩ��Ϣ*/
    bRlst = NAS_EMM_IsSpecNasMsg(       pstNasMsg->aucNasMsg,
                                        &(pstNasMsg->ulNasMsgSize));
    if(VOS_FALSE == bRlst)
    {
        return NAS_EMM_SECU_UNPROT_FAIL;
    }

    return NAS_EMM_SECU_UNPROT_SUCC;
}
NAS_EMM_SECU_UNPROT_RSLT_ENUM_UINT32  NAS_EMM_CurNotActiveNewAnyMsgInt(
                                        VOS_UINT32  ulSecuHeadType,
                                        VOS_VOID   *pulRrcNasMsg)
{
    VOS_BOOL                            bRlst =  VOS_FALSE;
    LRRC_LNAS_MSG_STRU                   *pstNasMsg = VOS_NULL_PTR;

    (VOS_VOID)ulSecuHeadType;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_CurNotActiveNewAnyMsgInt enterd.");

    pstNasMsg = (LRRC_LNAS_MSG_STRU *)pulRrcNasMsg;

    /*�����Ա�������Ϣ��ȥ����ȫͷ*/
    NAS_EMM_SecuDelSecuHead(pstNasMsg->aucNasMsg, &(pstNasMsg->ulNasMsgSize));

    /*�ж��ǲ�����Ȩ��Ϣ*/
    bRlst = NAS_EMM_IsSpecNasMsg(       pstNasMsg->aucNasMsg,
                                        &(pstNasMsg->ulNasMsgSize));
    if(VOS_FALSE == bRlst)
    {
        return NAS_EMM_SECU_UNPROT_FAIL;
    }

    return NAS_EMM_SECU_UNPROT_SUCC;

}


NAS_EMM_SECU_UNPROT_RSLT_ENUM_UINT32  NAS_EMM_CurExistNotAcitveNewAnyMsgIntEnc(
                                        VOS_UINT32  ulSecuHeadType,
                                        VOS_VOID   *pulRrcNasMsg)
{
    VOS_UINT32                          ulRslt = NAS_EMM_SECU_FAIL;
    LRRC_LNAS_MSG_STRU                   *pstNasMsg = VOS_NULL_PTR;

    (VOS_VOID)ulSecuHeadType;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_CurExistNotAcitveNewAnyMsgIntEnc enterd.");

    pstNasMsg = (LRRC_LNAS_MSG_STRU *)pulRrcNasMsg;

    /*�����Լ��,����*/
    ulRslt = NAS_EMM_IntEncMsgCommProc(pstNasMsg);
    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        return NAS_EMM_SECU_UNPROT_FAIL;
    }

    /*��ȫ������״̬Ǩ��*/
    NAS_EMM_SecuStateConvert(EMM_CUR_SECU_EXIST_ACTIVE,NAS_LMM_GetSecuCurFsmNS());

    return NAS_EMM_SECU_UNPROT_SUCC;
}


NAS_EMM_SECU_UNPROT_RSLT_ENUM_UINT32  NAS_EMM_CurExistActiveNewAnyMsgIntEnc(
                                        VOS_UINT32  ulSecuHeadType,
                                        VOS_VOID   *pulRrcNasMsg)
{
    VOS_UINT32                          ulRslt = NAS_EMM_SECU_FAIL;
    LRRC_LNAS_MSG_STRU                   *pstNasMsg = VOS_NULL_PTR;

    (VOS_VOID)ulSecuHeadType;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_CurExistActiveNewAnyMsgIntEnc enterd.");

    pstNasMsg = (LRRC_LNAS_MSG_STRU *)pulRrcNasMsg;

    /*�����Լ��,����*/
    ulRslt = NAS_EMM_IntEncMsgCommProc(pstNasMsg);

    if(NAS_EMM_SECU_FAIL == ulRslt)
    {
        return NAS_EMM_SECU_UNPROT_FAIL;
    }

    return NAS_EMM_SECU_UNPROT_SUCC;
}
NAS_EMM_SECU_UNPROT_RSLT_ENUM_UINT32  NAS_EMM_CurExistActiveNewAnyMsgPlain(
                                        VOS_UINT32  ulSecuHeadType,
                                        VOS_VOID   *pulRrcNasMsg)
{
    VOS_UINT8                           ucEmmMsgType = 0;
    VOS_UINT32                          ucRslt = 0;
    LRRC_LNAS_MSG_STRU                 *pstNasMsg = VOS_NULL_PTR;
    VOS_UINT8                          *pucRcvNasMsg = VOS_NULL_PTR;

    (VOS_VOID)ulSecuHeadType;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_CurExistActiveNewAnyMsgPlain enterd.");

    pstNasMsg = (LRRC_LNAS_MSG_STRU *)pulRrcNasMsg;
    pucRcvNasMsg = pstNasMsg->aucNasMsg;

    if(0 == g_ulNasPlainRejMsgFlag)
    {
        NAS_EMM_SECU_LOG_INFO("NAS_EMM_CurExistActiveNewAnyMsgPlain: g_ulSmcPlainRejMsgFlag isn't true, discard msg.");
        return NAS_EMM_SECU_UNPROT_FAIL;
    }

    /*�������EMM����Ϣ��ֱ�Ӷ���*/
    if(EMM_CN_MSG_PD_EMM != pucRcvNasMsg[NAS_EMM_SECU_CN_MSG_HEAD_POS])
    {
        NAS_EMM_SECU_LOG_INFO("NAS_EMM_CurExistActiveNewAnyMsgPlain: NOT EMM MSG, discard.");
        return NAS_EMM_SECU_UNPROT_FAIL;
    }

    /*��ȡ�տ���Ϣ����*/
    ucEmmMsgType = pucRcvNasMsg[NAS_EMM_SECU_CN_MSG_TYPE_POS];

    switch(ucEmmMsgType)
    {
        /* ATTACH/TAU/SERVICE REJ ��Ϊ��Ȩ��Ϣ���Խ�״̬������������Ϣ���� */
        case NAS_EMM_CN_MT_ATTACH_REJ:
        case NAS_EMM_CN_MT_TAU_REJ:
        case NAS_EMM_CN_MT_SER_REJ:

            ucRslt = NAS_EMM_SECU_UNPROT_SUCC;
            break;

       default:
            ucRslt = NAS_EMM_SECU_UNPROT_FAIL;
            break;
    }

    return ucRslt;
}
NAS_EMM_SECU_UNPROT_RSLT_ENUM_UINT32    NAS_EMM_OnlyCurExistMsgSmc(
                                        VOS_UINT32  ulSecuHeadType,
                                        VOS_VOID   *pulRrcNasMsg)
{
    VOS_UINT32                          ulRslt = NAS_EMM_SECU_FAIL;
    LRRC_LNAS_MSG_STRU                   *pstSmcMsg = VOS_NULL_PTR;

    (VOS_VOID)ulSecuHeadType;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_OnlyCurExistMsgSmc enterd.");

    pstSmcMsg = (LRRC_LNAS_MSG_STRU *)pulRrcNasMsg;

    ulRslt = NAS_EMM_SmcCommProcess(pstSmcMsg);
    if(NAS_EMM_SECU_SUCC == ulRslt)
    {
        /*��ȫ������״̬Ǩ��*/
        NAS_EMM_SecuStateConvert(       EMM_CUR_SECU_EXIST_ACTIVE,
                                        EMM_NEW_SECU_NOT_EXIST);
        /*SMC��Ϣ��֤�ɹ��Ĵ���*/
        NAS_EMM_SmcSucc(pstSmcMsg);
    }

    return NAS_EMM_SECU_UNPROT_SMC;
}


NAS_EMM_SECU_UNPROT_RSLT_ENUM_UINT32    NAS_EMM_CurAnyNewExistMsgSmc(
                                        VOS_UINT32  ulSecuHeadType,
                                        VOS_VOID   *pulRrcNasMsg)
{
    VOS_UINT32                          ulRslt = NAS_EMM_SECU_FAIL;
    LRRC_LNAS_MSG_STRU                   *pstSmcMsg = VOS_NULL_PTR;

    (VOS_VOID)ulSecuHeadType;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_CurAnyNewExistMsgSmc enterd.");

    pstSmcMsg = (LRRC_LNAS_MSG_STRU *)pulRrcNasMsg;

    ulRslt = NAS_EMM_SmcCommProcess(pstSmcMsg);
    if(NAS_EMM_SECU_SUCC == ulRslt)
    {
        NAS_LMM_SetSecuCurFsmCS(EMM_CUR_SECU_EXIST_ACTIVE);
        if((NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE != NAS_EMM_GetSecuNewMappedKSIsgsn())
         ||(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE != NAS_EMM_GetSecuNewNativeKSIasme())
         ||(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE != NAS_EMM_GetSecuNonCurNativeKSIasme()))
        {
            NAS_LMM_SetSecuCurFsmNS(EMM_NEW_SECU_EXIST);
        }
        else
        {
            NAS_LMM_SetSecuCurFsmNS(EMM_NEW_SECU_NOT_EXIST);
        }

        /*SMC��Ϣ��֤�ɹ��Ĵ���*/
        NAS_EMM_SmcSucc(pstSmcMsg);
    }


    return NAS_EMM_SECU_UNPROT_SMC;
}
NAS_EMM_SECU_UNPROT_RSLT_ENUM_UINT32 NAS_EMM_CurNotExistNewNotExistMsgSmc
(
    VOS_UINT32                          ulSecuHeadType,
    VOS_VOID                           *pulRrcNasMsg
)
{
    VOS_UINT32                          ulRslt      = NAS_EMM_SECU_FAIL;
    VOS_UINT8                           ucSmcEia    = NAS_EMM_NULL;
    LRRC_LNAS_MSG_STRU                 *pstSmcMsg   = VOS_NULL_PTR;

    (VOS_VOID)ulSecuHeadType;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_CurNotExistNewNotExistMsgSmc enterd.");

    pstSmcMsg = (LRRC_LNAS_MSG_STRU *)pulRrcNasMsg;

    /* �����������EIA0����ֱ���˳� */
    /*lint -e416 -e415*/
    ucSmcEia =  pstSmcMsg->aucNasMsg[NAS_EMM_SECU_SMC_ALG_POSE] & NAS_EMM_LOW_3_BITS_F;
    /*lint +e416 +e415*/
    if (NAS_EMM_SECU_ALG_EIA0 != ucSmcEia)
    {
        return NAS_EMM_SECU_UNPROT_FAIL;
    }

    ulRslt = NAS_EMM_SmcCommProcess(pstSmcMsg);
    if(NAS_EMM_SECU_SUCC == ulRslt)
    {
        /*��ȫ������״̬Ǩ��*/
        NAS_EMM_SecuStateConvert(       EMM_CUR_SECU_EXIST_ACTIVE,
                                        EMM_NEW_SECU_NOT_EXIST);
        /*SMC��Ϣ��֤�ɹ��Ĵ���*/
        NAS_EMM_SmcSucc(pstSmcMsg);
    }

    return NAS_EMM_SECU_UNPROT_SMC;
}
VOS_UINT32 NAS_EMM_VerifyDlMsgLen(      VOS_UINT32   ulSecuHeaderType,
                                        MsgBlock    *pMsg )
{
    LRRC_LMM_DATA_IND_STRU               *pstRrcNasMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulVerifyRslt = NAS_EMM_SECU_SUCC;

    pstRrcNasMsg = (LRRC_LMM_DATA_IND_STRU *)pMsg;

    switch(ulSecuHeaderType)
    {
        case NAS_EMM_SECU_HEAD_TYPE_PLAIN:
            if(pstRrcNasMsg->stNasMsg.ulNasMsgSize < NAS_EMM_MIN_PLAIN_MSG_LEN)
            {
                ulVerifyRslt = NAS_EMM_SECU_FAIL;
            }
            break;

        case NAS_EMM_SECU_HEAD_TYPE_INTEGRITY:
        case NAS_EMM_SECU_HEAD_TYPE_INTEGRITY_CIPHERED:
        case NAS_EMM_SECU_HEAD_TYPE_NEW_KEY_INTEGRITY:
            if(pstRrcNasMsg->stNasMsg.ulNasMsgSize < NAS_EMM_MIN_PROT_MSG_LEN)
            {
                ulVerifyRslt = NAS_EMM_SECU_FAIL;
            }
            break;

        default:
            ulVerifyRslt = NAS_EMM_SECU_FAIL;
             break;
    }

    return ulVerifyRslt;
}


VOS_UINT32  NAS_EMM_SecuUnProtect( MsgBlock  *pMsg )
{
    NAS_EMM_SECU_UNPROT_RSLT_ENUM_UINT32    enRslt = NAS_EMM_SECU_UNPROT_FAIL;
    NAS_LMM_PID_MSG_STRU                    *pMmPidMsg = VOS_NULL_PTR;
    LRRC_LMM_DATA_IND_STRU                   *pRrcMmDataIndMsg = VOS_NULL_PTR;
    VOS_UINT32                              ulSecuHeaderType = 0;
    VOS_UINT8                              *pucNasCnMsg = VOS_NULL_PTR;
    VOS_UINT8                               ucPd = 0;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuUnProtect enterd.");

    pMmPidMsg                           = (NAS_LMM_PID_MSG_STRU *)pMsg;
    pRrcMmDataIndMsg                    = (LRRC_LMM_DATA_IND_STRU *)pMsg;
    pucNasCnMsg                         = pRrcMmDataIndMsg->stNasMsg.aucNasMsg;

    /*��ȫ����û�򿪻��߲��ǿտ���Ϣ���򲻽Ᵽ����ֱ�ӽ�״̬��*/
    if( (NAS_EMM_SMC_OPEN != g_ulSmcControl) ||
        (ID_LRRC_LMM_DATA_IND != pMmPidMsg->ulMsgId))
    {
        NAS_EMM_SECU_LOG1_INFO("NAS_EMM_SecuUnProtect: SMC NOT OPEN or NOT AIR MSG.g_ulSmcControl = ",
                                g_ulSmcControl);
        return NAS_EMM_GO_INTO_EMM_FSM;
    }

    /*��ӡ��ǰ��ȫ������״̬�Ϳտ���Ϣԭʼ����*/
    NAS_LMM_PUBM_LOG_NORM("\n");
    NAS_LMM_PUBM_LOG_NORM("======NAS_CUR_SECU_CONTEXT_STATE:======");
    NAS_LMM_PrintFsmState(NAS_LMM_PARALLEL_FSM_SECU);
    NAS_LMM_PUBM_LOG_NORM("~~~~~~NAS Receive Message:~~~~~~");
    NAS_COMM_PrintArray(             NAS_COMM_GET_MM_PRINT_BUF(),
                                     pRrcMmDataIndMsg->stNasMsg.aucNasMsg,
                                     pRrcMmDataIndMsg->stNasMsg.ulNasMsgSize);
    /*�ж�ID_RRC_MM_DATA_IND�е�CN��Ϣ�İ�ȫͷ��Protocol discriminator��������
    EMM����Ϣ���˳�*/
    ucPd = pucNasCnMsg[NAS_EMM_SECU_CN_MSG_HEAD_POS]& NAS_EMM_LOW_HALF_BYTE_F;
    if(EMM_CN_MSG_PD_EMM != ucPd)
    {
        NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuUnProtect: NOT EMM MSG.");
        return NAS_EMM_NOT_GO_INTO_EMM_FSM;
    }

    /*��ȡ��ȫͷ����*/
    ulSecuHeaderType =  (VOS_UINT32)pucNasCnMsg[NAS_EMM_SECU_CN_MSG_HEAD_POS]
                        & NAS_EMM_HIGH_HALF_BYTE_F;
    NAS_EMM_SECU_LOG1_INFO("NAS_EMM_SecuUnProtect: ulSecuHeaderType = ",
                        ulSecuHeaderType);

    /*������пտ���Ϣ�����ĳ���*/
    if( NAS_EMM_SECU_FAIL == NAS_EMM_VerifyDlMsgLen(ulSecuHeaderType, pMsg))
    {
        NAS_EMM_SECU_LOG_WARN("NAS_EMM_SecuUnProtect: MsgLen Err or SecuHeadType Err.");
        return NAS_EMM_NOT_GO_INTO_EMM_FSM;
    }

    /*���Ҷ���������ִ��:������ϢΪNAS��Ϣ���ݣ�������DOPRA��Ϣͷ*/
    enRslt = NAS_LMM_MsgProcessInFsm(    NAS_LMM_PARALLEL_FSM_SECU,
                                        (MsgBlock  *)&(pRrcMmDataIndMsg->stNasMsg),
                                        ulSecuHeaderType,
                                        ulSecuHeaderType);

    /*�������������еļӡ��Ᵽ�����ܻ��޸�NAS��Ϣ�ĳ��ȣ����¼���DOPRA��Ϣ�ĳ���*/
    pRrcMmDataIndMsg->ulLength = pRrcMmDataIndMsg->stNasMsg.ulNasMsgSize
                               + sizeof(pRrcMmDataIndMsg->enMsgId)
                               + sizeof(pRrcMmDataIndMsg->stNasMsg.ulNasMsgSize);

    /*�����Ᵽ��֮�����Ϣ����*/
    NAS_LMM_PUBM_LOG_NORM("\n");
    NAS_LMM_PUBM_LOG_NORM("~~~~~~Message After UnProt~~~~~~");
    NAS_COMM_PrintArray(             NAS_COMM_GET_MM_PRINT_BUF(),
                                     pRrcMmDataIndMsg->stNasMsg.aucNasMsg,
                                     pRrcMmDataIndMsg->stNasMsg.ulNasMsgSize);
    /*������Ϣ�Ᵽ�����*/
    switch(enRslt)
    {
        case    NAS_EMM_SECU_UNPROT_SUCC:
                NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuUnProtect: NAS_EMM_SECU_UNPROT_SUCC");
                return NAS_EMM_GO_INTO_EMM_FSM;

        case    NAS_EMM_SECU_UNPROT_FAIL:
                NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuUnProtect: NAS_EMM_SECU_UNPROT_FAIL");
                return NAS_EMM_NOT_GO_INTO_EMM_FSM;

        case    NAS_EMM_SECU_UNPROT_SMC:

                /* Ȼ��MM�ڲ���Ϣ���� */
                (VOS_VOID)NAS_LMM_IntraMsgProcess();

                /*��ʼbuff�Ĵ���*/
                (VOS_VOID)NAS_LMM_BufMsgProcess();

                /*FOR PC_LINT*/

                /*�����SMC��Ϣ,������״̬������*/
                NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuUnProtect: NAS_EMM_SECU_UNPROT_SMC");
                return NAS_EMM_NOT_GO_INTO_EMM_FSM;
        default:
                NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuUnProtect: default");
                return NAS_EMM_NOT_GO_INTO_EMM_FSM;
    }

}


VOS_UINT32    NAS_EMM_SecuSerIntProt( LRRC_LNAS_MSG_STRU *pstSerReqMsg )
{
/* PC����:��MAC��SERVICE REQ��Ϣ��װʱ�Ѿ�����ֵΪ0��������Ҫ���� */
#if(VOS_WIN32 == VOS_OS_VER)
    VOS_UINT8                           aucPcShortMac[] = {0x11, 0x02};

    NAS_LMM_MEM_CPY(                   &(pstSerReqMsg->aucNasMsg[NAS_EMM_SECU_SHORT_MAC_POS]),
                                        aucPcShortMac,
                                        NAS_EMM_SECU_SHORT_MAC_LEN);

    return NAS_EMM_SUCC;

/* ���廷�� */
#else
     CIHPER_SINGLE_CFG_S                 stSingleSecuPara;
     VOS_INT32                           lRslt;
     VOS_UINT8                          *pucSrcAddr = VOS_NULL_PTR;
     VOS_UINT8                          *pucDstAddr = VOS_NULL_PTR;


     /* ���������Կ��㷨���������ö�MACΪ0��Ȼ�󷵻� */
     if(PS_SECU_ALG_NULL == NAS_EMM_GetSecuEia())
     {
         NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuSerIntProt: ALG is NULL_ALG.");
         NAS_LMM_MEM_SET(                 &(pstSerReqMsg->aucNasMsg[NAS_EMM_SECU_SHORT_MAC_POS]),
                                         0,
                                         NAS_EMM_SECU_SHORT_MAC_LEN);
         return  NAS_EMM_SECU_SUCC;
     }

     /*���MEM,�����Ա�����������ֵ*/
     NAS_LMM_MEM_SET(&stSingleSecuPara, 0, sizeof(CIHPER_SINGLE_CFG_S));

     stSingleSecuPara.enOpt = CIPHER_OPT_PROTECT_LMAC;
     stSingleSecuPara.u8Direction = NAS_EMM_SECU_UPLINK;
     stSingleSecuPara.u32Count = NAS_EMM_GetSecuUlNasCount();
     stSingleSecuPara.u32BlockLen = pstSerReqMsg->ulNasMsgSize - NAS_EMM_SECU_SHORT_MAC_LEN;;
     stSingleSecuPara.stAlgKeyInfo.u32KeyIndexInt = LTE_SECU_NAS_INTEGRITY_KEY_INDEX;
     stSingleSecuPara.stAlgKeyInfo.enAlgIntegrity
                                     = NAS_EMM_GetIntegrityAlg(NAS_EMM_GetSecuEia());

     stSingleSecuPara.u8BearId = NAS_EMM_SECU_SECURITY_BEARERID;
     stSingleSecuPara.bMemBlock = PS_TRUE;
     stSingleSecuPara.enAppdHeaderLen = CIPHER_HDR_BIT_TYPE_0;
     stSingleSecuPara.enHeaderLen = CIPHER_HDR_BIT_TYPE_0;

     pucSrcAddr = NAS_EMM_GetSecuSrcSpace();
     NAS_LMM_MEM_CPY(pucSrcAddr, pstSerReqMsg->aucNasMsg, stSingleSecuPara.u32BlockLen);
     pucDstAddr = NAS_EMM_GetSecuDstSpace();

     /*��ӡCipher����*/
     NAS_EMM_SecuPrintCipherPara(&stSingleSecuPara, pucSrcAddr);

     /*�ڵ��� BSP �ӿ�֮ǰˢ�� cacheԴ��ַ�ռ䣬���cacheĿ�Ŀռ�*/
     LMM_DATA_CACHE_FLUSH(pucSrcAddr, (stSingleSecuPara.u32BlockLen));

     /* ����BSP�ӿڽⱣ��*/
     lRslt = NAS_CIPHER_SingleSubmitTask(pucSrcAddr, pucDstAddr, &stSingleSecuPara);
     if (CIPHER_SUCCESS != lRslt)
     {
         NAS_EMM_SECU_LOG1_ERR("NAS_EMM_SecuSerIntProt:BSP_CIPHER_SingleSubmitTask fail.lRslt = ",
                                  lRslt);
         return NAS_EMM_SECU_FAIL;
     }

     LMM_DATA_CACHE_INVALID(pucDstAddr, NAS_EMM_SECU_MAC_LEN);

     /*�����Ա����ɹ���ɣ���װSER REQ��Ϣ*/
     NAS_LMM_MEM_CPY(     &(pstSerReqMsg->aucNasMsg[NAS_EMM_SECU_SHORT_MAC_POS]),
                           pucDstAddr + NAS_EMM_SECU_SHORT_MAC_POS,
                           NAS_EMM_SECU_SHORT_MAC_LEN);
     return NAS_EMM_SUCC;
#endif
}


VOS_UINT32  NAS_EMM_SecuNotSerIntProt( const LRRC_LNAS_MSG_STRU *pstRrcNasMsg )
{
/* PC����: MAC��ֵ */
#if(VOS_WIN32 == VOS_OS_VER)
    #ifdef PS_ITT_PC_TEST_NAS_ST
    VOS_UINT8                           aucPcMac[] = {0x5D, 0x22, 0x11, 0x02};
    #else
    VOS_UINT8                           aucPcMac[] = {0x02, 0x11, 0x22, 0x5D};
    #endif

    NAS_LMM_MEM_CPY(                     NAS_EMM_GetSecuDstSpace(),
                                        aucPcMac,
                                        NAS_EMM_SECU_MAC_LEN);
    return NAS_EMM_SUCC;
#else
/* ���廷�� */
       CIHPER_SINGLE_CFG_S                 stSingleSecuPara;
    VOS_INT32                           lRslt;
    VOS_UINT8                          *pucSrcAddr = VOS_NULL_PTR;
    VOS_UINT8                          *pucDstAddr = VOS_NULL_PTR;

    /* ���������Կ��㷨������Ŀ����������MACΪ0��Ȼ�󷵻� */
    if(PS_SECU_ALG_NULL == NAS_EMM_GetSecuEia())
    {
        NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuNotSerIntProt: ALG is NULL_ALG.");
        NAS_LMM_MEM_SET(                 NAS_EMM_GetSecuDstSpace(),
                                        0,
                                        NAS_EMM_SECU_MAC_LEN);
        return  NAS_EMM_SECU_SUCC;
    }

    /*���MEM,�����Ա�����������ֵ*/
    NAS_LMM_MEM_SET(&stSingleSecuPara, 0, sizeof(CIHPER_SINGLE_CFG_S));

    stSingleSecuPara.enOpt = CIPHER_OPT_PROTECT_LMAC;
    stSingleSecuPara.u8Direction = NAS_EMM_SECU_UPLINK;
    stSingleSecuPara.u32Count = NAS_EMM_GetSecuUlNasCount();
    stSingleSecuPara.u32BlockLen = pstRrcNasMsg->ulNasMsgSize + NAS_EMM_SECU_SN_LEN;
    stSingleSecuPara.stAlgKeyInfo.u32KeyIndexInt = LTE_SECU_NAS_INTEGRITY_KEY_INDEX;
    stSingleSecuPara.stAlgKeyInfo.enAlgIntegrity
                                = NAS_EMM_GetIntegrityAlg(NAS_EMM_GetSecuEia());

    stSingleSecuPara.u8BearId = NAS_EMM_SECU_SECURITY_BEARERID;
    stSingleSecuPara.bMemBlock = PS_TRUE;
    stSingleSecuPara.enAppdHeaderLen = CIPHER_HDR_BIT_TYPE_0;
    stSingleSecuPara.enHeaderLen = CIPHER_HDR_BIT_TYPE_0;

    pucSrcAddr = NAS_EMM_GetSecuSrcSpace();
    pucSrcAddr[0] = NAS_EMM_GetSecuUlSn();
    NAS_LMM_MEM_CPY(                     pucSrcAddr + NAS_EMM_SECU_SN_LEN,
                                        pstRrcNasMsg->aucNasMsg,
                                        pstRrcNasMsg->ulNasMsgSize);
    pucDstAddr = NAS_EMM_GetSecuDstSpace();

    /*��ӡCipher����*/
    NAS_EMM_SecuPrintCipherPara(&stSingleSecuPara, pucSrcAddr);

    /*�ڵ��� BSP �ӿ�֮ǰˢ�� cacheԴ��ַ�ռ䣬���cacheĿ�Ŀռ�*/
    LMM_DATA_CACHE_FLUSH(pucSrcAddr, (stSingleSecuPara.u32BlockLen));

    /* ����BSP�ӿڽⱣ��*/
    lRslt = NAS_CIPHER_SingleSubmitTask(pucSrcAddr, pucDstAddr, &stSingleSecuPara);
    if (CIPHER_SUCCESS != lRslt)
    {
        NAS_EMM_SECU_LOG1_ERR("NAS_EMM_SecuNotSerIntProt:BSP_CIPHER_SingleSubmitTask fail.lRslt = ",
                                 lRslt);
        return NAS_EMM_SECU_FAIL;
    }

    LMM_DATA_CACHE_INVALID(pucDstAddr, NAS_EMM_SECU_MAC_LEN);

    return NAS_EMM_SUCC;

#endif
}
VOS_VOID  NAS_EMM_SecuAddSecuHeader(NAS_EMM_SECU_HEAD_TYPE_ENUM_UINT8 enSecurHeadType,
                                  LRRC_LNAS_MSG_STRU                 *pstRrcNasMsg)
{
    VOS_UINT8                          *pucSecuBuf = VOS_NULL_PTR;


    /* pstRrcNasMsg �е�CN��Ϣ����6���ֽڣ�����ȫͷ�ĳ���*/
    pucSecuBuf = NAS_EMM_GetSecuBufSpace();

    NAS_LMM_MEM_SET(pucSecuBuf, 0, LRRC_LNAS_MAX_DATA_LENGTH);
    NAS_LMM_MEM_CPY(         pucSecuBuf,
                            pstRrcNasMsg->aucNasMsg,
                            pstRrcNasMsg->ulNasMsgSize);

    /*lint -e416 -e415*/
    NAS_LMM_MEM_CPY(        &(pstRrcNasMsg->aucNasMsg[NAS_EMM_SECU_MSG_HEAD_LEN]),
                            pucSecuBuf,
                            pstRrcNasMsg->ulNasMsgSize);
    /*lint +e416 +e415*/

    /* �޸� pstNasMsg �е�CN���� */
    pstRrcNasMsg->ulNasMsgSize += NAS_EMM_SECU_MSG_HEAD_LEN;

    /* 0�ֽ��ȫͷ���� */
    pstRrcNasMsg->aucNasMsg[0] = EMM_CN_MSG_PD_EMM | enSecurHeadType ;

    /* 1~4�ֽ�: MAC*/
    /*lint -e419*/
    NAS_LMM_MEM_CPY(        &(pstRrcNasMsg->aucNasMsg[NAS_EMM_SECU_MAC_POS]),
                            NAS_EMM_GetSecuDstSpace(),
                            NAS_EMM_SECU_MAC_LEN);
    /*lint +e419*/

    /*5�ֽ�:SN */
    /*lint -e416 -e415*/
    pstRrcNasMsg->aucNasMsg[NAS_EMM_SECU_MSG_SN_POS] = NAS_EMM_GetSecuUlSn();
    /*lint +e416 +e415*/

    return;
}
VOS_UINT32  NAS_EMM_SecuEncrypt( LRRC_LNAS_MSG_STRU *pstRrcNasMsg )
{
/*PC���������Ĳ��䣬ֱ�ӷ���*/
#if(VOS_WIN32 == VOS_OS_VER)
    return NAS_EMM_SUCC;

/*���廷����*/
#else
    CIHPER_SINGLE_CFG_S                 stSingleSecuPara;
    VOS_INT32                           lRslt;
    VOS_UINT8                          *pucSrcAddr = VOS_NULL_PTR;
    VOS_UINT8                          *pucDstAddr = VOS_NULL_PTR;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuEncrypt entered.");

    /*���ǿ��㷨����ֱ�ӷ��� */
    if(PS_SECU_ALG_NULL == NAS_EMM_GetSecuEca())
    {
        NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuEncrypt: Eca IS NULL.");
        return  NAS_EMM_SUCC;
    }

    /* ��Ϣ���ܣ�������ֵ*/
    NAS_LMM_MEM_SET(&stSingleSecuPara, 0, sizeof(CIHPER_SINGLE_CFG_S));

    stSingleSecuPara.enOpt = CIPHER_OPT_ENCRYPT;
    stSingleSecuPara.u8Direction = NAS_EMM_SECU_UPLINK;
    stSingleSecuPara.u32Count = NAS_EMM_GetSecuUlNasCount();
    stSingleSecuPara.u32BlockLen = pstRrcNasMsg->ulNasMsgSize;
    stSingleSecuPara.stAlgKeyInfo.u32KeyIndexSec = LTE_SECU_NAS_CIPHER_KEY_INDEX;
    stSingleSecuPara.stAlgKeyInfo.enAlgSecurity
                                = NAS_EMM_GetSecurityAlg(NAS_EMM_GetSecuEca());


    stSingleSecuPara.u8BearId = NAS_EMM_SECU_SECURITY_BEARERID;
    stSingleSecuPara.bMemBlock = PS_TRUE;
    stSingleSecuPara.enAppdHeaderLen = CIPHER_HDR_BIT_TYPE_0;
    stSingleSecuPara.enHeaderLen = CIPHER_HDR_BIT_TYPE_0;

    pucSrcAddr = NAS_EMM_GetSecuSrcSpace();
    NAS_LMM_MEM_CPY(                     pucSrcAddr,
                                        pstRrcNasMsg->aucNasMsg,
                                        pstRrcNasMsg->ulNasMsgSize);
    pucDstAddr = NAS_EMM_GetSecuDstSpace();

    /*��ӡCipher����*/
    NAS_EMM_SecuPrintCipherPara(&stSingleSecuPara, pucSrcAddr);

    /*�ڵ��� BSP �ӿ�֮ǰˢ�� cacheԴ��ַ�ռ䣬���cacheĿ�Ŀռ�*/
    LMM_DATA_CACHE_FLUSH(pucSrcAddr, (stSingleSecuPara.u32BlockLen));

    /* ����BSP�ӿڽⱣ��*/
    lRslt = NAS_CIPHER_SingleSubmitTask(pucSrcAddr, pucDstAddr, &stSingleSecuPara);
    if (CIPHER_SUCCESS != lRslt)
    {
        NAS_EMM_SECU_LOG1_ERR("NAS_EMM_SecuEncrypt:BSP_CIPHER_SingleSubmitTask fail.lRslt = ",
                                 lRslt);
        return NAS_EMM_SECU_FAIL;
    }

    LMM_DATA_CACHE_INVALID(pucDstAddr, (stSingleSecuPara.u32BlockLen));

    /*���տ���Ϣ�滻Ϊ����*/
    NAS_LMM_MEM_CPY(                     pstRrcNasMsg->aucNasMsg,
                                        NAS_EMM_GetSecuDstSpace(),
                                        pstRrcNasMsg->ulNasMsgSize);

    return NAS_EMM_SUCC;
#endif

}

/*****************************************************************************
 Function Name   : NAS_EMM_SecuMsgNeedNotEncrypt
 Description     : �ж���Ϣ�Ƿ���Լ���
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.sunbing49683      2010-9-20  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_SecuMsgNeedNotEncrypt( NAS_EMM_CN_MSG_TYPE_ENUM_UINT8 enMsgType)
{
    /* TAU��attach req��Ϣ��Զ���ܼ��ܣ����ܱ�ʶ���ں�����Ŀǰ����֧��TAU��attach��Ϣ�����ܹ���*/
    if(( 1 == g_ulTauCipheredFlag )
     &&(( NAS_EMM_CN_MT_TAU_REQ == enMsgType)||( NAS_EMM_CN_MT_ATTACH_REQ == enMsgType)))
    {
        NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuMsgNeedNotEncrypt: attach req or tau req cannot encrypt.");
        return NAS_EMM_SUCC;
    }
    else
    {
        return NAS_EMM_FAIL;
    }
}
VOS_UINT32  NAS_EMM_NotSerReqMsgProt( LRRC_LNAS_MSG_STRU *pstRrcNasMsg )
{
    VOS_UINT32                          ulRslt    = NAS_EMM_FAIL;
    NAS_EMM_CN_MSG_TYPE_ENUM_UINT8      enMsgType = NAS_EMM_CN_MT_BUTT;
    NAS_EMM_SECU_HEAD_TYPE_ENUM_UINT8   enSecurHeadType;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_NotSerReqMsgProt entered.");

    /* ��ȡ��Ϣ��� */
    enMsgType = pstRrcNasMsg->aucNasMsg[NAS_EMM_SECU_CN_MSG_TYPE_POS];

    /*��ʼ����ȫͷ0x10*/
    enSecurHeadType = NAS_EMM_SECU_HEAD_TYPE_INTEGRITY;

    /*�����ǰ״̬ΪCurExistActive,������Ϣ��Ҫ���ܣ������Ϣ�ȼ��ܣ���ȫͷ�޸�Ϊ 0x20*/
    if((EMM_CUR_SECU_EXIST_ACTIVE == NAS_LMM_GetSecuCurFsmCS())
     &&(NAS_EMM_FAIL == NAS_EMM_SecuMsgNeedNotEncrypt(enMsgType)))
    {
        enSecurHeadType = NAS_EMM_SECU_HEAD_TYPE_INTEGRITY_CIPHERED;

        /*EMM��Ϣ�������� SMC COMPLETE��Ϣ����ȫͷΪ 4*/
        if( NAS_EMM_CN_MT_SMC_CMP == enMsgType)
        {
            enSecurHeadType = NAS_EMM_SECU_HEAD_TYPE_NEW_KEY_INTEGRITY_CIPHERED;
        }

        ulRslt = NAS_EMM_SecuEncrypt(pstRrcNasMsg);
        if(NAS_EMM_FAIL == ulRslt)
        {
            return NAS_EMM_FAIL;
        }
    }

    /*��SER REQ��Ϣ�����Ա���:����MAC*/
    ulRslt = NAS_EMM_SecuNotSerIntProt(pstRrcNasMsg);
    if(NAS_EMM_FAIL == ulRslt)
    {
        return NAS_EMM_FAIL;
    }

    /*��װ��Ϣ*/
    NAS_EMM_SecuAddSecuHeader(enSecurHeadType,pstRrcNasMsg);

    return NAS_EMM_SUCC;
}
/*lint +e961*/
/*lint +e960*/

VOS_UINT32 NAS_EMM_SecuWhetherNeedProt(const LRRC_LNAS_MSG_STRU *pstNasMsg)

{
    (VOS_VOID)pstNasMsg;
    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuWhetherNeedProt entered.");

    /*1.�����ȫ���ش򿪣���ǰ��ȫ������״̬Ϊ:CurNotExit����ֱ�ӷ���*/
    if(EMM_CUR_SECU_NOT_EXIST == NAS_LMM_GetSecuCurFsmCS())
    {
        NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuWhetherNeedProt: Cur Secu Context Not Exit.");
        return NAS_EMM_SECU_NOT_NEED_PROT;
    }

    /*zhengjunyan mod 2010-4-21:SMC REJ�ӱ���*/
    #if 0

    /*2.�����ȫ���ش򿪣���ϢΪEMM��Ϣ������ΪSMC REJ�����۰�ȫ��������ʲô״̬��ֱ�ӷ���*/
    if( NAS_EMM_CN_MT_SMC_REJ == pstNasMsg->aucNasMsg[NAS_EMM_SECU_CN_MSG_TYPE_POS])
    {
        NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuWhetherNeedProt: SMC REJ.");
        return NAS_EMM_SECU_NOT_NEED_PROT;
    }
    #endif
    return NAS_EMM_SECU_NEED_PROT;
}
VOS_UINT32  NAS_EMM_SecuUpLayerMsgProt( LRRC_LNAS_MSG_STRU *pstNasMsg )
{
    VOS_UINT32                          ulRslt = NAS_EMM_FAIL;
    NAS_EMM_SECU_HEAD_TYPE_ENUM_UINT8   enSecurHeadType;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuUpLayerMsgProt entered.");

    /*ESM����͸����Ϣһ�����ڰ�ȫ���Ӵ��������,�������̴����д���*/
    if(EMM_CUR_SECU_EXIST_ACTIVE != NAS_LMM_GetSecuCurFsmCS())
    {
        NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuUpLayerMsgProt: ESM MSG PROT ERR.");
        return NAS_EMM_FAIL;
    }

    /*��ȫͷ��ֵ*/
    enSecurHeadType = NAS_EMM_SECU_HEAD_TYPE_INTEGRITY_CIPHERED;

    /*����*/
    ulRslt = NAS_EMM_SecuEncrypt(pstNasMsg);
    if(NAS_EMM_FAIL == ulRslt)
    {
        return NAS_EMM_FAIL;
    }

    /*�������Ա���:����MAC*/
    ulRslt = NAS_EMM_SecuNotSerIntProt(pstNasMsg);
    if(NAS_EMM_FAIL == ulRslt)
    {
        return NAS_EMM_FAIL;
    }

    /*��װ��Ϣ*/
    NAS_EMM_SecuAddSecuHeader(enSecurHeadType,pstNasMsg);

    return NAS_EMM_SUCC;
}
VOS_UINT32  NAS_EMM_SecuEmmMsgProt( LRRC_LNAS_MSG_STRU *pstNasMsg )
{
    NAS_EMM_SECU_HEAD_TYPE_ENUM_UINT8   enSecurHeadType;

    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuEmmMsgProt entered.");

    /* ��ȡ��ȫͷ*/
    enSecurHeadType = pstNasMsg->aucNasMsg[NAS_EMM_SECU_CN_MSG_HEAD_POS]
                    & NAS_EMM_SECU_MSG_HIGH_BIT;
    /*��SER REQ��Ϣ*/
    if( NAS_EMM_SECU_HEAD_TYPE_SERVICE_REQUEST == enSecurHeadType)
    {
        if(NAS_EMM_FAIL == NAS_EMM_SecuSerIntProt(pstNasMsg))
        {
            NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuEmmMsgProt:SER REQ PROT ERR.");
            return NAS_EMM_FAIL;
        }
    }
    /*����SER REQ��Ϣ*/
    else
    {
        if(NAS_EMM_FAIL == NAS_EMM_NotSerReqMsgProt(pstNasMsg))
        {
            NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuEmmMsgProt:NOT SER REQ PROT ERR.");
            return NAS_EMM_FAIL;
        }
    }

    return NAS_EMM_SUCC;
}
VOS_UINT32    NAS_EMM_SecuProtect(VOS_VOID  * pstMrrcDataReq)
{
    NAS_EMM_MRRC_DATA_REQ_STRU         *pstIntraDataReq = VOS_NULL_PTR;
    VOS_UINT8                           ucPd;


    NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuProtect entered.");

    /*��ȫ�����ĵ�ǰ״̬*/
    NAS_LMM_PUBM_LOG_NORM("\n");
    NAS_LMM_PUBM_LOG_NORM("======NAS_CUR_SECU_CONTEXT_STATE:======");
    NAS_LMM_PrintFsmState(NAS_LMM_PARALLEL_FSM_SECU);

    pstIntraDataReq = (NAS_EMM_MRRC_DATA_REQ_STRU *)pstMrrcDataReq;

    /*������Ӳ����ڣ���Ҫ��RRC�·�RRC_MM_INFO_CHANGE_REQ��Ϣ*/
    if(NAS_EMM_CONN_IDLE == NAS_EMM_GetConnState() )
    {
        NAS_EMM_SendSecuParamToRrc();
    }

    /*��ȡ��ϢЭ��ͷ*/
    ucPd = pstIntraDataReq->stNasMsg.aucNasMsg[NAS_EMM_SECU_CN_MSG_HEAD_POS]
         & NAS_EMM_LOW_HALF_BYTE_F;

    /*�����EMM��Ϣ*/
    if(EMM_CN_MSG_PD_EMM == ucPd)
    {
        /*�ж��Ƿ���Ҫ������*/
        if( NAS_EMM_SECU_NOT_NEED_PROT == NAS_EMM_SecuWhetherNeedProt(&(pstIntraDataReq->stNasMsg)))
        {
            NAS_EMM_SECU_LOG_INFO("NAS_EMM_SecuProtect: No need to protect.");
            return NAS_EMM_SUCC;
        }

        /*EMM��Ϣ���мӱ���*/
        if(NAS_EMM_FAIL == NAS_EMM_SecuEmmMsgProt(&(pstIntraDataReq->stNasMsg)))
        {
            NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuProtect: EMM MSG PROT ERR.");
            return NAS_EMM_FAIL;
        }
    }
    /*�ϲ�͸����Ϣ*/
    else
    {
        if(NAS_EMM_FAIL == NAS_EMM_SecuUpLayerMsgProt(&(pstIntraDataReq->stNasMsg)))
        {
            NAS_EMM_SECU_LOG_ERR("NAS_EMM_SecuProtect: ESM MSG PROT ERR.");
            return NAS_EMM_FAIL;
        }

    }

    /*��Ϣ�ӱ�����ɺ�NAS��Ϣ�ĳ��ȿ��ܻᷢ���仯����Ҫ����DOPRA��Ϣ�ĳ���*/
    pstIntraDataReq->ulLength = NAS_EMM_COUNT_INTRA_MSG_NO_HEADER_LEN(
                                    pstIntraDataReq->stNasMsg.ulNasMsgSize);

    /*�����ӱ���֮�����Ϣ����*/
    NAS_LMM_PUBM_LOG_NORM("\n");
    NAS_LMM_PUBM_LOG_NORM("~~~~~~Message After Prot~~~~~~");

    NAS_COMM_PrintArray(             NAS_COMM_GET_MM_PRINT_BUF(),
                                     pstIntraDataReq->stNasMsg.aucNasMsg,
                                     pstIntraDataReq->stNasMsg.ulNasMsgSize);

    /*����NASCOUNT�� 1*/
    NAS_EMM_GetSecuUlNasCount()++;

    return NAS_EMM_SUCC;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

