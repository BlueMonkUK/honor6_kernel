
/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "NasCcCtx.h"
#include "NasCcProcNvim.h"
#include "NVIM_Interface.h"
#include "NasCcCommon.h"

#include "NasNvInterface.h"
#include "TafNvInterface.h"

#include "NasUsimmApi.h"
#include "NasCcTimer.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_NAS_CC_PROC_NIVM_C

/*****************************************************************************
  2 ȫ�ֱ���
*****************************************************************************/

/*****************************************************************************
  3 �궨��
*****************************************************************************/

/*****************************************************************************
   4 ����ʵ��
*****************************************************************************/


VOS_VOID  NAS_CC_ReadCcTimerLenNvim( VOS_VOID )
{
    NAS_CC_NVIM_TIMER_LEN_STRU          stNasCcTimerLen;
    VOS_UINT32                          ulLength;
    VOS_UINT32                          ulTimerLength;

    ulLength = 0;

    ulTimerLength = 0;
    
    PS_MEM_SET(&stNasCcTimerLen, 0, sizeof(NAS_CC_NVIM_TIMER_LEN_STRU));
    
    NV_GetLength(en_NV_Item_CC_TimerLen, &ulLength);	
    if (ulLength > sizeof(NAS_CC_NVIM_TIMER_LEN_STRU))
    {
        return;
    }
    
    if (VOS_OK != NV_Read(en_NV_Item_CC_TimerLen,
                         &stNasCcTimerLen, ulLength))
    {
        NAS_CC_WARN_LOG("NAS_CC_ReadCcTimerLenNvim():WARNING: en_NV_Item_CC_TimerLen Error");

        return;
    }
    
    /* ���NV�����õ�ֵΪ0���ߴ��ڵ���30s������Э��Ĭ��ֵ30s����������Ϊ��Чֵ����Ҫ�������� */
    if ((stNasCcTimerLen.ucT305Len > 0)
      &&(stNasCcTimerLen.ucT305Len < 30))
    {
        /* ����T305��ʱ�� */
        ulTimerLength = NAS_CC_ONE_THOUSAND_MILLISECOND * ((VOS_UINT32)stNasCcTimerLen.ucT305Len);
        NAS_CC_SetNvTimerLen(TI_NAS_CC_T305, ulTimerLength);
    }
    else
    {
        /* ����T305��ʱ�� */
        ulTimerLength = 30000;
        NAS_CC_SetNvTimerLen(TI_NAS_CC_T305, ulTimerLength);
    }

    if ((stNasCcTimerLen.ucT308Len > 0)
      &&(stNasCcTimerLen.ucT308Len < 30))
    {
        /* ����T308��ʱ�� */
        ulTimerLength = NAS_CC_ONE_THOUSAND_MILLISECOND * ((VOS_UINT32)stNasCcTimerLen.ucT308Len);
        NAS_CC_SetNvTimerLen(TI_NAS_CC_T308, ulTimerLength);
    }
    else
    {
        /* ����T308��ʱ�� */
        ulTimerLength = 30000;
        NAS_CC_SetNvTimerLen(TI_NAS_CC_T308, ulTimerLength);
    }

    return;
}
VOS_VOID  NAS_CC_ReadCcbsNvim( VOS_VOID )
{
    NAS_CC_NVIM_CCBS_SUPPORT_FLG_STRU   stCcbsSupportFlg;
    NAS_CC_CUSTOM_CFG_INFO_STRU        *pstCcbsSupportFlg = VOS_NULL_PTR;
    VOS_UINT32                          ulLength;

    ulLength = 0;
    

    stCcbsSupportFlg.ucCcbsSupportFlg = NAS_CC_NV_ITEM_DEACTIVE;

    pstCcbsSupportFlg = NAS_CC_GetCustomCfgInfo();
    
    NV_GetLength(en_NV_Item_NVIM_CCBS_SUPPORT_FLG, &ulLength);	
    if (ulLength > sizeof(NAS_NVIM_CCBS_SUPPORT_FLG_STRU))
    {
        return;
    }
    
    if (VOS_OK != NV_Read(en_NV_Item_NVIM_CCBS_SUPPORT_FLG,
                         &stCcbsSupportFlg, ulLength))
    {
        NAS_CC_WARN_LOG("NAS_CC_ReadCcbsNvim():WARNING: en_NV_Item_NVIM_CCBS_SUPPORT_FLG Error");

        return;
    }


    if ((NAS_CC_NV_ITEM_ACTIVE != stCcbsSupportFlg.ucCcbsSupportFlg)
     && (NAS_CC_NV_ITEM_DEACTIVE != stCcbsSupportFlg.ucCcbsSupportFlg))
    {
        pstCcbsSupportFlg->ucCcbsSupportFlg = NAS_CC_NV_ITEM_DEACTIVE;
        NAS_CC_WARN_LOG("NAS_CC_ReadCcbsNvim():WARNING: NV parameter Error");
        return;
    }

    pstCcbsSupportFlg->ucCcbsSupportFlg = stCcbsSupportFlg.ucCcbsSupportFlg;

    return;
}



VOS_VOID NAS_CC_ReadNvimInfo(VOS_VOID)
{
    /* en_NV_Item_NVIM_CCBS_SUPPORT_FLG */
    NAS_CC_ReadCcbsNvim();

    /* en_NV_Item_CC_TimerLen */
    NAS_CC_ReadCcTimerLenNvim();

    return;
}



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif



