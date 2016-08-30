
#include <osm.h>
#include <gen_msg.h>
/*lint -save -e537*/
#include <DrvInterface.h>
/*lint -restore*/
#include <ftm.h>
#include <ftm_ct.h>
/*lint -save -e537*/
#include <msp_errno.h>
/*lint -restore*/
#include <msp_drx.h>
/*lint -save -e767*/
#define    THIS_FILE_ID        MSP_FILE_ID_FTM_MAILBOX_PROC_C
/*lint -restore*/
/* �������ݿɶ�֪ͨ�ź���������ص����� */
VOS_SEM g_mailbox_readsem = 0; 

/*****************************************************************************
 �� �� ��  : ftm_mailbox_msgproc
 ��������  : �������ݶ�ȡ
 �������  : enMbxType ������������
 �������  : ��
 �� �� ֵ  : 0�����ݣ�����0Ϊ�������ݳ���
*****************************************************************************/
VOS_UINT32 ftm_mailbox_msgproc(MAILBOX_SERVICE_TYPE_E enMbxType)
{
    VOS_UINT32 ulRet      = 0;
    VOS_UINT32 ulMsgSize  = 0;
    VOS_VOID* pMailBoxMsg = NULL;
    MsgBlock* pMsgBlock   = NULL;
    VOS_UINT32 ret = ERR_MSP_UNKNOWN;

    /* ��ȡ��Ϣ���� */
    HAL_SDMLOG("[%s] BEIGN TO READ DATA.\n", __FUNCTION__);
    ulMsgSize = BSP_MailBox_ComMsgSize(enMbxType);
    HAL_SDMLOG("[%s] BEIGN TO READ DATA.LEN = %d\n", __FUNCTION__,ulMsgSize);
    if(ulMsgSize == 0 )
    {
        return ulMsgSize;
    }

    pMailBoxMsg = VOS_MemAlloc(MSP_SYS_FTM_PID, ((DYNAMIC_MEM_PT)), ulMsgSize);
    if(NULL == pMailBoxMsg)
    {
        HAL_SDMLOG("[%s] VOS_MemAlloc failed.\n", __FUNCTION__);
        return 0;
    }

    /* ��ȡ�������� */
    ulRet = BSP_MailBox_ComMsgRead(enMbxType, pMailBoxMsg, ulMsgSize, EN_MAILBOX_SLEEP_WAKEUP);
    if(ulRet != BSP_OK)
    {
        VOS_MemFree(MSP_SYS_FTM_PID, pMailBoxMsg);
        return 0;
    }

    /* ������Ϣ��MSP_SYS_FTM_PID���� */
    pMsgBlock = (MsgBlock*)VOS_AllocMsg(MSP_SYS_FTM_PID, sizeof(OS_MSG_STRU));

    if (pMsgBlock)
    {
        OS_MSG_STRU* pMsg = (OS_MSG_STRU*)pMsgBlock->aucValue;

        pMsgBlock->ulReceiverPid = MSP_SYS_FTM_PID;
        pMsgBlock->ulSenderPid   = MSP_SYS_FTM_PID;
        pMsg->ulMsgId  = ID_MSG_L1A_CT_IND;
        pMsg->ulParam1 = pMailBoxMsg;
        pMsg->ulParam2 = ulMsgSize;

        ret = VOS_SendMsg(MSP_SYS_FTM_PID, pMsgBlock);
        if (ret != VOS_OK)
        {
            HAL_SDMLOG("[%s] send msg to MSP_SYS_FTM_PID FAILED \n",  __FUNCTION__);
        }
    }

    return ulMsgSize;
}

/*****************************************************************************
 �� �� ��  : ftm_mailbox_taskproc
 ��������  : �����������Դ�������
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_VOID ftm_mailbox_taskproc()
{
	/*lint -save -e716*/
	while(1)
	/*lint -restore*/
    {
        if(VOS_ERR == VOS_SmP(g_mailbox_readsem, 0))
        {
            HAL_SDMLOG("[%s] VOS_Smp(g_mailbox_readsem, 0) error\n",  __FUNCTION__);
        }

		/*vote for lock*/
		drx_msp_fid_vote_lock(DRX_FTM_MAILBOX_VOTE);
       /* LTE CT ԭ�� */
        if(ftm_mailbox_msgproc(EN_MAILBOX_SERVICE_LTE_CT) > 0)
        {
            //VOS_SmV(g_mailbox_readsem);
            continue;
        }

      /* LTE BT ԭ�� */
        if(ftm_mailbox_msgproc(EN_MAILBOX_SERVICE_LTE_BT) > 0)
        {
            //VOS_SmV(g_mailbox_readsem);
            continue;
        }

        /* TDS CT ԭ�� */
        if(ftm_mailbox_msgproc(EN_MAILBOX_SERVICE_TDS_CT) > 0)
        {
            //VOS_SmV(g_mailbox_readsem);
            continue;
        }

        /* TDS BT ԭ�� */
        if(ftm_mailbox_msgproc(EN_MAILBOX_SERVICE_TDS_BT) > 0)
        {
            //VOS_SmV(g_mailbox_readsem);
            continue;
        }
		/*vote for lock*/
		drx_msp_fid_vote_unlock(DRX_FTM_MAILBOX_VOTE);
        
        //VOS_SmV(g_mailbox_readsem);
    }

}

/*****************************************************************************
 �� �� ��  : ftm_mailbox_ltect_cb
 ��������  : LTE CT�������ݵ��׻ص�
 �������  : enIntType �����ж�����
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_VOID ftm_mailbox_ltect_cb(MAILBOX_INT_TYPE_E enIntType)
{
    VOS_SmV(g_mailbox_readsem);

    return;
}

/*****************************************************************************
 �� �� ��  : ftm_mailbox_ltebt_cb
 ��������  : LTE BT�������ݵ��׻ص�
 �������  : enIntType �����ж�����
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_VOID ftm_mailbox_ltebt_cb(MAILBOX_INT_TYPE_E enIntType)
{
    VOS_SmV(g_mailbox_readsem);

    return;
}

/*****************************************************************************
 �� �� ��  : ftm_mailbox_tdsct_cb
 ��������  : TDS CT�������ݵ��׻ص�
 �������  : enIntType �����ж�����
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_VOID ftm_mailbox_tdsct_cb(MAILBOX_INT_TYPE_E enIntType)
{
    VOS_SmV(g_mailbox_readsem);

    return;
}

/*****************************************************************************
 �� �� ��  : ftm_mailbox_tdsbt_cb
 ��������  : TDS BT�������ݵ��׻ص�
 �������  : enIntType �����ж�����
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_VOID ftm_mailbox_tdsbt_cb(MAILBOX_INT_TYPE_E enIntType)
{
    VOS_SmV(g_mailbox_readsem);

    return;
}

/*****************************************************************************
 �� �� ��  : ftm_mailbox_ltect_write
 ��������  : дLTE CT�������ݵ�����
 �������  : param ԭ������
             ulLen ԭ�ﳤ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_UINT32 ftm_mailbox_ltect_write(VOS_VOID* param,VOS_UINT32 ulLen)
{
    VOS_UINT32 ulRet = ERR_MSP_SUCCESS;
    HAL_SDMLOG("[%s] : send lte ct to dsp.\n", __FUNCTION__);
    ulRet = BSP_MailBox_ComMsgWrite(EN_MAILBOX_SERVICE_LTE_CT, param, ulLen, EN_MAILBOX_SLEEP_WAKEUP);
    if(ulRet != BSP_OK)
    {
        HAL_SDMLOG("[%s] : send lte ct to dsp fail!!!.\n", __FUNCTION__);
        ulRet = ERR_MSP_FAILURE;
    }
    FTM_MNTN_FTM2RTT_MSG(*((VOS_UINT*)param))

    return ulRet;
}

/*****************************************************************************
 �� �� ��  : ftm_mailbox_ltebt_write
 ��������  : дLTE bT�������ݵ�����
 �������  : param ԭ������
             ulLen ԭ�ﳤ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_UINT32 ftm_mailbox_ltebt_write(VOS_VOID* param,VOS_UINT32 ulLen)
{
    VOS_UINT32 ulRet = ERR_MSP_SUCCESS;
    HAL_SDMLOG("[%s] : send lte ct to dsp.\n", __FUNCTION__);
    ulRet = BSP_MailBox_ComMsgWrite(EN_MAILBOX_SERVICE_LTE_BT, param, ulLen, EN_MAILBOX_SLEEP_WAKEUP);
    if(ulRet != BSP_OK)
    {
        HAL_SDMLOG("[%s] : send lte ct to dsp fail !!!.\n", __FUNCTION__);
        ulRet = ERR_MSP_FAILURE;
    }
    FTM_MNTN_FTM2RTT_MSG(*((VOS_UINT*)param))

    return ulRet;
}

/*****************************************************************************
 �� �� ��  : ftm_mailbox_tdsct_write
 ��������  : дTDS CT�������ݵ�����
 �������  : param ԭ������
             ulLen ԭ�ﳤ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_UINT32 ftm_mailbox_tdsct_write(VOS_VOID* param,VOS_UINT32 ulLen)
{
    VOS_UINT32 ulRet = ERR_MSP_SUCCESS;

    ulRet = BSP_MailBox_ComMsgWrite(EN_MAILBOX_SERVICE_TDS_CT, param, ulLen, EN_MAILBOX_SLEEP_WAKEUP);
    if(ulRet != BSP_OK)
    {
        ulRet = ERR_MSP_FAILURE;
    }
    FTM_MNTN_FTM2RTT_MSG(*((VOS_UINT*)param))

    return ulRet;
}

/*****************************************************************************
 �� �� ��  : ftm_mailbox_tdsbt_write
 ��������  : дLTE BT�������ݵ�����
 �������  : param ԭ������
             ulLen ԭ�ﳤ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_UINT32 ftm_mailbox_tdsbt_write(VOS_VOID* param,VOS_UINT32 ulLen)
{
    VOS_UINT32 ulRet = ERR_MSP_SUCCESS;

    ulRet = BSP_MailBox_ComMsgWrite(EN_MAILBOX_SERVICE_TDS_BT, param, ulLen, EN_MAILBOX_SLEEP_WAKEUP);
    if(ulRet != BSP_OK)
    {
        ulRet = ERR_MSP_FAILURE;
    }
    FTM_MNTN_FTM2RTT_MSG(*((VOS_UINT*)param))

    return ulRet;
}

/*****************************************************************************
 �� �� ��  : ftm_mailbox_ltect_write
 ��������  : дLTE CT�������ݵ�����
 �������  : param ԭ������
             ulLen ԭ�ﳤ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_UINT32 ftm_mailbox_ct_write(VOS_UINT32 mode, VOS_VOID* param,VOS_UINT32 ulLen)
{
    VOS_UINT32 ulRet = ERR_MSP_SUCCESS;

    if(EN_FCHAN_MODE_TD_SCDMA == mode)
    {
        vos_printf("set tds dsp msg to dsp\n");
        ulRet = ftm_mailbox_tdsct_write(param, ulLen);
    }
    else
    {
        vos_printf("set lte dsp msg to dsp\n");
        ulRet = ftm_mailbox_ltect_write(param, ulLen);
    }

    return ulRet;
}

/*****************************************************************************
 �� �� ��  : ftm_mailbox_bt_write
 ��������  : дLTE BT�������ݵ�����
 �������  : param ԭ������
             ulLen ԭ�ﳤ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_UINT32 ftm_mailbox_bt_write(VOS_UINT32 mode, VOS_VOID* param,VOS_UINT32 ulLen)
{
    VOS_UINT32 ulRet = ERR_MSP_SUCCESS;

    if(EN_FCHAN_MODE_TD_SCDMA == mode)
    {
        ulRet = ftm_mailbox_tdsbt_write(param, ulLen);
    }
    else
    {
        ulRet = ftm_mailbox_ltebt_write(param, ulLen);
    }

    return ulRet;
}


/*****************************************************************************
 �� �� ��  : ftm_mailbox_init
 ��������  : FTMģ�����������ʼ��
 �������  : param ԭ������
             ulLen ԭ�ﳤ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_UINT32 ftm_mailbox_init()
{
    if(VOS_OK != VOS_SmCCreate( "ftm_mailbox_taskproc", 0, VOS_SEMA4_FIFO, &g_mailbox_readsem))
    {
        HAL_SDMLOG("[%s] : VOS_SmCCreate failed.\n", __FUNCTION__);
        return ERR_MSP_FAILURE;
    }

    if(BSP_OK != BSP_MailBox_ComNotifyReg(EN_MAILBOX_SERVICE_LTE_CT, ftm_mailbox_ltect_cb))
    {
        HAL_SDMLOG("[%s] : BSP_MailBox_ComNotifyReg EN_MAILBOX_SERVICE_LTE_CT failed.\n", __FUNCTION__);
        return ERR_MSP_FAILURE;
    }

    if(BSP_OK != BSP_MailBox_ComNotifyReg(EN_MAILBOX_SERVICE_LTE_BT, ftm_mailbox_ltebt_cb))
    {
        HAL_SDMLOG("[%s] : BSP_MailBox_ComNotifyReg EN_MAILBOX_SERVICE_LTE_BT failed.\n", __FUNCTION__);
        return ERR_MSP_FAILURE;
    }

    if(BSP_OK != BSP_MailBox_ComNotifyReg(EN_MAILBOX_SERVICE_TDS_CT, ftm_mailbox_tdsct_cb))
    {
        HAL_SDMLOG("[%s] : BSP_MailBox_ComNotifyReg EN_MAILBOX_SERVICE_TDS_CT failed.\n", __FUNCTION__);
        return ERR_MSP_FAILURE;
    }

    if(BSP_OK != BSP_MailBox_ComNotifyReg(EN_MAILBOX_SERVICE_TDS_BT, ftm_mailbox_tdsbt_cb))
    {
        HAL_SDMLOG("[%s] : BSP_MailBox_ComNotifyReg EN_MAILBOX_SERVICE_TDS_BT failed.\n", __FUNCTION__);
        return ERR_MSP_FAILURE;
    }

    return ERR_MSP_SUCCESS;
}

