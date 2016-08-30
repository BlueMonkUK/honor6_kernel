/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : om_msghook.c
  �� �� ��   : ����
  ��    ��   : ��ׯ�� 59026
  ��������   : 2011��6��16��
  ����޸�   :
  ��������   :
  �����б�   :
              OM_MSGHOOK_Init
              OM_MSGHOOK_MsgCfgMsgHookReq
              OM_MSGHOOK_VosMsgHook
  �޸���ʷ   :
  1.��    ��   : 2011��6��16��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "om.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    ��ά�ɲ���Ϣ�а�����C�ļ���ź궨��
*****************************************************************************/
/*lint -e(767)*/
#define THIS_FILE_ID                    OM_FILE_ID_OM_MSGHOOK_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/* ��ά�ɲ�ģ������Ϣ��ȡ����ȫ�ֱ��� */
OM_MSGHOOK_STRU                         g_stOmMsghook;

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
/*****************************************************************************
 �� �� ��  : OM_MSGHOOK_Init
 ��������  : ��ά�ɲ���Ϣ��ȡ���ܳ�ʼ��
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��5��31��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_MSGHOOK_Init(VOS_VOID)
{
    /* Ĭ�Ϲر���Ϣ��ȡ */
   OM_MSGHOOK_SetMsgHookEnable(OM_SWITCH_OFF);
}
VOS_UINT32 OM_MSGHOOK_DefaultCfg(CODEC_MSG_HOOK_CONFIG_STRU *pstMsgHookCfg)
{
    /* �ж�NV�����õ�ֵ�ĺϷ��� */
    if (pstMsgHookCfg->uhwEnable >= OM_SWITCH_BUTT)
    {
        return UCOM_RET_ERR_PARA;
    }

    /* �����������������ȫ�ֱ��������ò����Ϣ��ȡ���� */
    OM_MSGHOOK_SetMsgHookEnable(pstMsgHookCfg->uhwEnable);

    return UCOM_RET_SUCC;
}


VOS_VOID OM_MSGHOOK_SetMsgHookEnable(VOS_UINT16 uhwEnable)
{
    g_stOmMsghook.uhwVosMsgHookEnable = uhwEnable;

    /* ע��ϵͳ��Ϣ���Ӻ��� */
    if (OM_SWITCH_ON == uhwEnable)
    {
        VOS_RegisterMsgGetHook(OM_MSGHOOK_VosMsgHook);
    }
    else
    {
        VOS_RegisterMsgGetHook(VOS_NULL);
    }
}

/*****************************************************************************
 �� �� ��  : OM_MSGHOOK_MsgCfgMsgHookReq
 ��������  : ����ID_OM_CODEC_MSG_HOOK_REQ��Ϣ�������Ƿ�ȡHiFi�Ͻ�����VOS��Ϣ
 �������  : VOS_VOID *pstOsaMsg
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��16��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 OM_MSGHOOK_MsgCfgMsgHookReq(VOS_VOID *pvOsaMsg)
{
    OM_CODEC_MSG_HOOK_REQ_STRU           *pstMsgHook;
    CODEC_OM_MSG_HOOK_CNF_STRU            stMsgCnf;
    VOS_UINT32                          uwRet   = UCOM_RET_SUCC;

    UCOM_MemSet(&stMsgCnf, 0, sizeof(stMsgCnf));

    /* ����OM_CODEC_MSG_HOOK_REQ_STRU�ṹ������Ϣ���� */
    pstMsgHook = (OM_CODEC_MSG_HOOK_REQ_STRU*)pvOsaMsg;

    /* ������飬����Ϣ������usHookEnable��Χ����OM_SWITCH_ENUMö�ٷ�Χ */
    if (pstMsgHook->enHookEnable >= OM_SWITCH_BUTT)
    {
        uwRet = UCOM_RET_ERR_PARA;
    }
    else
    {
        /* �����������������ȫ�ֱ��������²����Ϣ��ȡ���� */
        OM_MSGHOOK_SetMsgHookEnable(pstMsgHook->enHookEnable);
        uwRet = UCOM_RET_SUCC;
    }

    /* ���ظ���Ϣ��ͨ��͸����Ϣ���� */
    stMsgCnf.uhwMsgId   = ID_CODEC_OM_MSG_HOOK_CNF;
    stMsgCnf.enReturn   = (VOS_UINT16)uwRet;

    OM_COMM_SendTranMsg(&stMsgCnf, sizeof(stMsgCnf));

    return uwRet;
}

/*****************************************************************************
 �� �� ��  : OM_MSGHOOK_VosMsgHook
 ��������  : ��Ϣ��ȡ���Ӻ�������VOS��Ϣ����ͨ��OM�ϱ���ȥ
 �������  : VOS_VOID *pvOsaMsg - ���ϱ�����Ϣ
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��18��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_MSGHOOK_VosMsgHook(VOS_VOID *pvOsaMsg)
{
    MsgBlock    *pstMsg = (MsgBlock *)pvOsaMsg;

    /* ����Ϣ��ȡ�� */
    if (OM_SWITCH_ON == OM_MSGHOOK_GetMsgHookEnable())
    {
        OM_COMM_SendTrace((VOS_UCHAR *)pvOsaMsg, pstMsg->uwLength + VOS_MSG_HEAD_LENGTH);
    }
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

