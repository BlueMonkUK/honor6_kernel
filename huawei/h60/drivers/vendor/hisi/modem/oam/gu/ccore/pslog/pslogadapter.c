
/******************************************************************************

                ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : PsLogAdapter.c
  �� �� ��   : ����
  ��    ��   : ���� 47350
  ��������   : 2008��7��16��
  ����޸�   :
  ��������   : PsLogAdapter���ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2008��7��16��
    ��    ��   : ���� 47350
    �޸�����   : �����ļ�

*****************************************************************************/
#ifdef  __cplusplus
  #if  __cplusplus
  extern "C"{
  #endif
#endif

#include "pslogadapter.h"
#include "om.h"

/*lint -e718*/
/*lint -e746*/
/*lint -e40*/
/*lint -e64*/
/*lint -e63*/
/*****************************************************************************
 �� �� ��  : LOG_CreateMutex
 ��������  : �����������ź���
 �������  : pLogSem
 �������  : ��
 �� �� ֵ  : LOG_ERR - ����ʧ��
             LOG_OK  - �����ɹ�


 �޸���ʷ      :
  1.��    ��   : 2008��7��17��
    ��    ��   : ���� 47350
    �޸�����   : �����ɺ���
*****************************************************************************/
LOG_ULONG LOG_CreateMutex(LOG_SEM *pLogSem)
{
#if(VOS_OS_VER == VOS_VXWORKS)
    *pLogSem = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);

    if (LOG_NULL_PTR == *pLogSem)
    {
        return LOG_ERR;
    }
#elif(VOS_OS_VER == VOS_RTOSCK)
    if (SRE_OK != SRE_SemBCreate(OS_SEM_FULL, pLogSem, SEM_MODE_PRIOR))
    {
        return LOG_ERR;
    }
#endif
    return LOG_OK;
}
/*****************************************************************************
 �� �� ��  : LOG_CreateSemaphore
 ��������  : �����������ź���
 �������  : pLogSem
 �������  : ��
 �� �� ֵ  : LOG_ERR - ����ʧ��
             LOG_OK  - �����ɹ�


 �޸���ʷ      :
  1.��    ��   : 2008��7��17��
    ��    ��   : ���� 47350
    �޸�����   : �����ɺ���
*****************************************************************************/
LOG_ULONG LOG_CreateSemaphore(LOG_SEM *pLogSem)
{
#if(VOS_OS_VER == VOS_VXWORKS)
    *pLogSem = semCCreate(SEM_Q_FIFO, SEM_EMPTY);

    if (LOG_NULL_PTR == *pLogSem)
    {
        return LOG_ERR;
    }
#elif(VOS_OS_VER == VOS_RTOSCK)
    if (SRE_OK != SRE_SemCCreate(OS_SEM_EMPTY, pLogSem, SEM_MODE_FIFO))
    {
        return LOG_ERR;
    }
#endif

    return LOG_OK;
}
/*****************************************************************************
 �� �� ��  : LOG_DeleteSemaphore
 ��������  : ɾ���������ź���
 �������  : pLogSem
 �������  : ��
 �� �� ֵ  : LOG_VOID

 �޸���ʷ      :
  1.��    ��   : 2008��7��17��
    ��    ��   : ���� 47350
    �޸�����   : �����ɺ���
*****************************************************************************/
LOG_VOID LOG_DeleteSemaphore(LOG_SEM *pLogSem)
{
#if(VOS_OS_VER == VOS_VXWORKS)
    semDelete(*pLogSem);
#elif(VOS_OS_VER == VOS_RTOSCK)
    SRE_SemDelete(*pLogSem);
#endif
}
/*****************************************************************************
 �� �� ��  : Log_SmV
 ��������  : ���ź�������V����
 �������  : pLogSem
 �������  : ��
 �� �� ֵ  : LOG_ERR - ����ʧ��
             LOG_OK  - �����ɹ�


 �޸���ʷ      :
  1.��    ��   : 2008��7��17��
    ��    ��   : ���� 47350
    �޸�����   : �����ɺ���
*****************************************************************************/
LOG_ULONG Log_SmV(LOG_SEM *pLogSem)
{
#if(VOS_OS_VER == VOS_VXWORKS)
    if (LOG_OK != semGive(*pLogSem))
    {
        return LOG_ERR;
    }
#elif(VOS_OS_VER == VOS_RTOSCK)
    if (SRE_OK != SRE_SemPost(*pLogSem))
    {
        return LOG_ERR;
    }
#endif
    return LOG_OK;
}
/*****************************************************************************
 �� �� ��  : Log_SmV
 ��������  : ���ź�������P����
 �������  : pLogSem
 �������  : ��
 �� �� ֵ  : LOG_ERR - ����ʧ��
             LOG_OK  - �����ɹ�

 �޸���ʷ      :
  1.��    ��   : 2008��7��17��
    ��    ��   : ���� 47350
    �޸�����   : �����ɺ���
*****************************************************************************/
LOG_ULONG Log_SmP(LOG_SEM *pLogSem)
{
#if(VOS_OS_VER == VOS_VXWORKS)
    if (LOG_OK != semTake(*pLogSem, WAIT_FOREVER))
    {
        return LOG_ERR;
    }
#elif(VOS_OS_VER == VOS_RTOSCK)
    if (SRE_OK != SRE_SemPend (*pLogSem, WAIT_FOREVER))
    {
        return LOG_ERR;
    }
#endif

    return LOG_OK;
}

/*****************************************************************************
 �� �� ��  : LOG_RegisterDrv
 ��������  : LOG����ӿڣ����ڷ�VxWorksƽ̨��Ч
 �������  : fptr      - ָ���ӡLOG��Ϣ�ĺ���ָ��
 �������  : ��
 �� �� ֵ  : LOG_ERR - ����ʧ��
             LOG_OK  - �����ɹ�

 �޸���ʷ      :
  1.��    ��   : 2008��7��17��
    ��    ��   : ���� 47350
    �޸�����   : �����ɺ���
*****************************************************************************/
LOG_ULONG LOG_RegisterDrv(LOG_PFUN fptr, LOG_PFUN fptr1, LOG_PFUN fptr2,
                                  LOG_PFUN fptr3, LOG_PFUN fptr4)
{
#if((VOS_OS_VER == VOS_VXWORKS)||(VOS_OS_VER == VOS_RTOSCK))
    return (LOG_ULONG)DRV_LOG_INSTALL((VOIDFUNCPTR)fptr, (VOIDFUNCPTR)fptr1,
             (VOIDFUNCPTR)fptr2, (VOIDFUNCPTR)fptr3, (VOIDFUNCPTR)fptr4);
#else
    return LOG_OK;
#endif
}

#ifdef  __cplusplus
  #if  __cplusplus
  }
  #endif
#endif

