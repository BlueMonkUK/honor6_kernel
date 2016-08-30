/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_mailbox.c
  �� �� ��   : ����
  ��    ��   : ��ׯ�� 59026
  ��������   : 2012��12��15��
  ����޸�   :
  ��������   : mailbox�ӿ�
  �����б�   :

  �޸���ʷ   :
  1.��    ��   : 2012��12��15��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ļ�
******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "drv_mailbox.h"
#include "drv_mailbox_cfg.h"



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 ��������
*****************************************************************************/
unsigned long mailbox_send_msg(
                unsigned long            mailcode,
                void                    *data,
                unsigned long            length);

unsigned long mailbox_reg_msg_cb(
                unsigned long             mailcode,
                mb_msg_cb                 func,
                void                     *data);

unsigned long mailbox_read_msg_data(
                void                   *mail_handle,
                 char                  *buff,
                unsigned long          *size);
/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


unsigned long DRV_MAILBOX_SENDMAIL(
                unsigned long           MailCode,
                void                   *pData,
                unsigned long           Length)
{
    return mailbox_send_msg(MailCode, pData, Length);
}


unsigned long DRV_MAILBOX_REGISTERRECVFUNC(
                unsigned long           MailCode,
                mb_msg_cb               pFun,
                void                   *UserHandle)
{
    return mailbox_reg_msg_cb(MailCode, pFun, UserHandle);
}


unsigned long DRV_MAILBOX_READMAILDATA(
                void                   *MailHandle,
                unsigned char          *pData,
                unsigned long          *pSize)
{
    return mailbox_read_msg_data(MailHandle, (char *)pData, pSize);
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

