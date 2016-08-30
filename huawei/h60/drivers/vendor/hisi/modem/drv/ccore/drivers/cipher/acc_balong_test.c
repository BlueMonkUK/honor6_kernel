
#include "memLib.h"
#include "semLib.h"
#include <stdlib.h>
#include <taskLib.h>
#include "product_config.h"
#include "cipher_balong_common.h"
#include "cipher_balong.h"
#include "acc_balong.h"
#include "kdf_balong.h"
#include "drv_cipher.h"

#define ERROR (-1)
#define OK 0	

#define ACC_CHN_NUM 0
#define CIPHER_CHN 1
static BSP_U32  ACC_APH_LEN = 0;

static SEM_ID g_UsrClkSendCountSem = NULL;
static BSP_U32 g_AccInitCmp = FALSE;
static BSP_U32 g_ChnBusy = 0;
static BSP_BOOL g_bIsAcc = TRUE;
static BSP_U32 g_keyRam[4] = {0x2b7e1516, 0x28aed2a6, 0xabf71588, 0x09cf4f3c};
//static BSP_S32 g_UsrClkID = -1;
//static BSP_U32 g_CipherCfgAddr = 0;
//static BSP_U32 g_CfgAddr = 0;
//static BSP_U32 g_InDataAddr = 0;
//static BSP_U32 g_OutDataAddr = 0;
/*****************************************************************************
* �� �� ��  : Acc_memcmp_ext
*
* ��������  :�ԱȰ���ǰ�Ͱ��ƺ������Ƿ���ͬ
*
* �������  :void* mem1 :�������ݿ��׵�ַ
                           void* mem2 :������ݿ��׵�ַ
                           BSP_U32 size :���ݳ���
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
* ����˵��  :
*
*****************************************************************************/
BSP_S32 Acc_memcmp_ext(const void* mem1, const void* mem2, BSP_U32 size)
{
    BSP_U32 i = 0;

    for (i = 0; i < size; i++)
    {
        if (*((char*)mem1 + i) != *((char*)mem2 + i ))
        {
            logMsg("diff_pos:%d,addr1(0x%x):%x,addr2(0x%x):%x\n", i, (int)((char*)mem1 + i), (int)*((char*)mem1+i),
                   (int)((char*)mem2 + i), (int)*((char*)mem2 + i), 0);
            return ERROR;
        }
    }

    return OK;
}

/*****************************************************************************
* �� �� ��  : AccDataCmp
*
* ��������  :�ԱȰ���ǰ�Ͱ��ƺ������Ƿ���ͬ
*
* �������  :void* pBlk1 :�������ݿ��׵�ַ
                           void* pBlk2 :������ݿ��׵�ַ
                           BSP_U32 u32BlockLen :���ݳ���
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
* ����˵��  :
*
*****************************************************************************/
BSP_S32 AccDataCmp(void* pBlk1, void* pBlk2,BSP_U32 u32BlockLen)
{
    BSP_S32 s32CmpResult = ERROR;

    void* pTempBlk1 = pBlk1;
    void* pTempBlk2 = pBlk2;

    if ((pBlk1 == NULL) || (pBlk2 == NULL))
    {
        return ERROR;
    }

    s32CmpResult = Acc_memcmp_ext(pTempBlk1, pTempBlk2,u32BlockLen);
    if (s32CmpResult != OK)
    {
        return ERROR;
    }

    return OK;
}

/*****************************************************************************
* �� �� ��  : Acc_CipherCfg
*
* ��������  :����cipher������Ϣ
*
* �������  :void* mem1 :�������ݿ��׵�ַ
                           void* mem2 :������ݿ��׵�ַ
                           BSP_U32 size :���ݳ���
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
* ����˵��  :
*
*****************************************************************************/
void Acc_CipherCfg(ACC_SINGLE_CFG_S *pstCfg, BSP_U32 CfgType)
{
    /*����pstCfg��Ϣ*/
    switch(CfgType)
    {
        case 0: 
            BSP_CIPHER_SetKey(g_keyRam , 0, 0);
            pstCfg->stAlgKeyInfo.u32KeyIndexSec = 0;                         /*���ܲ�������Ҫkey�����*/
            pstCfg->u32AppdHeaderLen = 0;                                  /*���Ӱ�ͷ�ĳ���*/
            pstCfg->u32HeaderLen = 0;                                        /*��ͷpacker head�ĳ���*/
            pstCfg->stAlgKeyInfo.enAlgSecurity = CIPHER_ALG_SNOW3G;                               /*���ܲ�������SNOW3G*/
            pstCfg->stAlgKeyInfo.enAlgIntegrity= 0; 
            pstCfg->u32BearId = 12;    
            pstCfg->u32Aph = 0x512;  
            pstCfg->u32Count = 0x512;
            break;
            
        case 1:
            BSP_CIPHER_SetKey(g_keyRam , 0, 0);
            pstCfg->stAlgKeyInfo.u32KeyIndexSec = 0;                         /*���ܲ�������Ҫkey�����*/
            pstCfg->u32AppdHeaderLen = 0;                                  /*���Ӱ�ͷ�ĳ���*/
            pstCfg->u32HeaderLen = 0;                                        /*��ͷpacker head�ĳ���*/
            pstCfg->stAlgKeyInfo.enAlgSecurity = CIPHER_ALG_AES_128;                               /*���ܲ�������AES192*/
            pstCfg->stAlgKeyInfo.enAlgIntegrity= 0; 
            pstCfg->u32BearId = 12;    
            pstCfg->u32Aph = 0x56432F;  
            pstCfg->u32Count = 0x56432F;
            break;
            
        case 2:
            BSP_CIPHER_SetKey(g_keyRam , 0, 0);
            pstCfg->stAlgKeyInfo.u32KeyIndexSec = 0;                         /*���ܲ�������Ҫkey�����*/
            pstCfg->u32AppdHeaderLen = 1;                                  /*���Ӱ�ͷ�ĳ���*/
            pstCfg->u32HeaderLen = 0;                                        /*��ͷpacker head�ĳ���*/
            pstCfg->stAlgKeyInfo.enAlgSecurity = CIPHER_ALG_AES_128;                               /*���ܲ�������AES128*/
            pstCfg->stAlgKeyInfo.enAlgIntegrity= 0; 
            pstCfg->u32BearId = 12;    
            pstCfg->u32Aph = 0x56432F;  
            pstCfg->u32Count = 0x56432F;
            break;
            
        case 3:
            BSP_CIPHER_SetKey(g_keyRam , 0, 0);
            pstCfg->stAlgKeyInfo.u32KeyIndexSec = 0;                         /*���ܲ�������Ҫkey�����*/
            pstCfg->u32AppdHeaderLen = 2;                                  /*���Ӱ�ͷ�ĳ���*/
            pstCfg->u32HeaderLen = 0;                                        /*��ͷpacker head�ĳ���*/
            pstCfg->stAlgKeyInfo.enAlgSecurity = CIPHER_ALG_AES_128;                               /*���ܲ�������AES256*/
            pstCfg->stAlgKeyInfo.enAlgIntegrity= 0; 
            pstCfg->u32BearId = 3;    
            pstCfg->u32Aph = 256;  
            pstCfg->u32Count = 256;
            break;
        case 4:
            BSP_CIPHER_SetKey(g_keyRam , 0, 0);
            pstCfg->stAlgKeyInfo.u32KeyIndexSec = 0;                         /*���ܲ�������Ҫkey�����*/
            pstCfg->u32AppdHeaderLen = 2;                                  /*���Ӱ�ͷ�ĳ���*/
            pstCfg->u32HeaderLen = 2;                                        /*��ͷpacker head�ĳ���*/
            pstCfg->stAlgKeyInfo.enAlgSecurity = CIPHER_ALG_AES_128;                               /*���ܲ�������NULL*/
            pstCfg->stAlgKeyInfo.enAlgIntegrity= 0; 
            pstCfg->u32BearId = 3;    
            pstCfg->u32Aph = 256;  
            pstCfg->u32Count = 256;
            break;            
    }
    return;
}

/*****************************************************************************
* �� �� ��  : ACC_ST_Init
*
* ��������  :ACC ST���Գ�ʼ��
*
* �������  : BSP_U32 u32BlockLen:���ݿ鳤��
                            BSP_U32 u32PakNums:������
* �������  :void *pInAddr:�������ݿ��׵�ַ
                           void *pOutAddr:������ݿ��׵�ַ
*
* �� �� ֵ  : OK&ERROR
*
* ����˵��  :
*
*****************************************************************************/
BSP_S32 ACC_ST_Init(BSP_U32 u32BlockLen, BSP_U32 u32PakNums, void **pInAddr, void **pOutAddr,
                        ACC_SINGLE_CFG_S ** pstCfg,CIHPER_SINGLE_CFG_S **pstCipherCfg)
{
    BSP_S32 i;
    char * ptempIn;

    *pstCipherCfg = (CIHPER_SINGLE_CFG_S *) CIPHER_MALLOC(sizeof(CIHPER_SINGLE_CFG_S));
    if(!*pstCipherCfg)
    {
        logMsg(">>>line %d FUNC %s fail, malloc failed !\n", (int)__LINE__, (int)__FUNCTION__, 0, 0, 0, 0);
        return ERROR;
    }

    *pstCfg = (ACC_SINGLE_CFG_S *)CIPHER_MALLOC(sizeof(ACC_SINGLE_CFG_S));
    if(!*pstCfg)
    {
        CIPHER_FREE(*pstCipherCfg);
        logMsg(">>>line %d FUNC %s fail, malloc failed !\n", (int)__LINE__, (int)__FUNCTION__, 0, 0, 0, 0);
        return ERROR;
    }
    
    *pInAddr = (void *)CIPHER_MALLOC(u32BlockLen * u32PakNums + ACC_APH_LEN* u32PakNums);
    if(NULL == *pInAddr)
    {
        CIPHER_FREE(*pstCipherCfg);
        CIPHER_FREE(*pstCfg);
        logMsg(">>>line %d FUNC %s fail, malloc failed !\n", (int)__LINE__, (int)__FUNCTION__, 0, 0, 0, 0);
        return ERROR;
    }
    
    *pOutAddr = (void *)CIPHER_MALLOC(u32BlockLen * u32PakNums + ACC_APH_LEN* u32PakNums);
    if(NULL == *pInAddr)
    {
        CIPHER_FREE(*pstCipherCfg);
        CIPHER_FREE(*pstCfg);
        CIPHER_FREE(*pInAddr);
        logMsg(">>>line %d FUNC %s fail, malloc failed !\n", (int)__LINE__, (int)__FUNCTION__, 0, 0, 0, 0);
        return ERROR;
    }
#if 0
    ptempIn = (BSP_U32* )*pInAddr;
    *ptempIn = 0xe2bec16b;
    ptempIn++;
    *ptempIn = 0x969f402e;
     ptempIn++;
    *ptempIn = 0x117e3de9;
     ptempIn++;
    *ptempIn = 0x2a179373;
#endif
    ptempIn = *pInAddr;
    for(i = 0; i < (BSP_S32)(u32BlockLen * u32PakNums + ACC_APH_LEN* u32PakNums) ; i ++)
    {
        *ptempIn = i%256;
        ptempIn ++;
    }
    //memset(*pInAddr, 1,u32BlockLen * u32PakNums + ACC_APH_LEN* u32PakNums);
    
    memset(*pOutAddr, 0, u32BlockLen * u32PakNums + ACC_APH_LEN* u32PakNums);
    
    return OK;
}


/*****************************************************************************
* �� �� ��  : ACC_ST_Init
*
* ��������  :ACC ST���Գ�ʼ��
*
* �������  : BSP_U32 u32BlockLen:���ݿ鳤��
                            BSP_U32 u32PakNums:������
* �������  :void *pInAddr:�������ݿ��׵�ַ
                           void *pOutAddr:������ݿ��׵�ַ
*
* �� �� ֵ  : OK&ERROR
*
* ����˵��  :
*
*****************************************************************************/
#if 0
BSP_S32 ACC_ST_STOP()
{
    g_bIsAcc = FALSE;
    BSP_USRCLK_Disable(g_UsrClkID);
    BSP_USRCLK_Free(g_UsrClkID);

    CIPHER_FREE((void*)g_CipherCfgAddr);
    CIPHER_FREE((void*)g_CfgAddr);
    CIPHER_FREE((void*)g_InDataAddr);
    CIPHER_FREE((void*)g_OutDataAddr);

    
}
#endif
/*****************************************************************************
* �� �� ��  : ACC_ST_DMA
*
* ��������  :��������������٣�����DMA���Ʒ�ʽ
*
* �������  : BSP_U32 u32BlockLen : ������
                            BSP_U32 u32PakNums : һ������İ�����
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
* ����˵��  :
*
*****************************************************************************/
BSP_S32 ACC_ST_DMA(BSP_U32 u32BlockLen, BSP_U32 u32PakNums)
{
    BSP_S32 s32ret;
    BSP_S32 i;
    BSP_U32 u32BdFifoAddr;
    ACC_SINGLE_CFG_S *pstCfg = NULL;
    CIHPER_SINGLE_CFG_S *pstCipherCfg = NULL;
    void *pInAddr = NULL;
    void *pOutAddr = NULL;
    void *pTempInAddr = NULL;
    void *pTempOutAddr = NULL;
	/*���ܲ�����*/
    BSP_U32 timeStart = 0;
    BSP_U32 timeEnd = 0;
   
    
    s32ret = ACC_ST_Init(u32BlockLen, u32PakNums, &pInAddr, &pOutAddr,&pstCfg,&pstCipherCfg);  
    if(ERROR == s32ret)
    {
        return ERROR;
    }
    
    pstCfg->u32BlockLen = u32BlockLen;
    pTempInAddr = pInAddr;
    pTempOutAddr = pOutAddr;
    
    /*��ʼ���������*/
    if(FALSE == g_AccInitCmp)
    {
        s32ret = bsp_acc_init();
        if(CIPHER_NO_MEM == s32ret)
        {
            logMsg(">>>line %d FUNC %s fail, malloc failed !\n", (int)__LINE__, (int)__FUNCTION__, 0, 0, 0, 0);
            goto ERRFREE;
        }
         g_AccInitCmp = TRUE;
    }
    

    /*��ȡ����FIFO�׵�ַ*/
    u32BdFifoAddr = BSP_Accelerator_GetBdFifoAddr();
    if((BSP_U32)CIPHER_UNKNOWN_ERROR == u32BdFifoAddr )
    {
        logMsg(">>>line %d FUNC %s fail��result = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, u32BdFifoAddr, 0, 0, 0);
        goto ERRFREE;
    }

   /*����DMA��ʽ*/ 
   for(i = 0; i <(BSP_S32)u32PakNums; i++)
    {
        s32ret = BSP_Accelerator_DMA(u32BdFifoAddr, pTempInAddr, pTempOutAddr, pstCfg);
        if(OK != s32ret)
        {
            logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
            goto ERRFREE;
        }
        pTempInAddr = (char *)((BSP_U32)pTempInAddr +u32BlockLen);
        pTempOutAddr = (char *)((BSP_U32)pTempOutAddr + u32BlockLen);
    }

    /*��ȡ��ǰͨ��״̬��������ʹ���������*/
    while(BSP_Accelerator_GetStatus() != CHN_FREE)
    {
        g_ChnBusy ++;
    }

   /* taskDelay(1000);*/
    
    s32ret = BSP_Accelerator_Enable(u32BdFifoAddr);
    if(CIPHER_UNKNOWN_ERROR == s32ret)
    {
        logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
        goto ERRFREE;
    }   

    timeStart = tickGet();
    /*�ȴ��������*/
    do{
        /*taskDelay(10);*//*Ϊ�˵�һʱ��֪��ͨ���Ѿ�ֹͣ����ʹ����ʱ������*/
    }while(BSP_Accelerator_GetStatus() != CHN_FREE);

	/*���ܲ���*/
    timeEnd = tickGet();
	logMsg("timeStart :%d timeEnd %d\n", timeStart, timeEnd, 0, 0, 0, 0);
    
    /*�Ƚ����ǰ�����Ƿ���ͬ*/
    s32ret = AccDataCmp(pInAddr, pOutAddr,u32BlockLen * u32PakNums);
    if(ERROR == s32ret)
    {
        logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
        goto ERRFREE;
    }

    CIPHER_FREE(pstCipherCfg);
    CIPHER_FREE(pstCfg);
    CIPHER_FREE(pInAddr);
    CIPHER_FREE(pOutAddr);

    return OK;
    ERRFREE: 
        CIPHER_FREE(pstCipherCfg);
        CIPHER_FREE(pstCfg);
        CIPHER_FREE(pInAddr);
        CIPHER_FREE(pOutAddr);
        return ERROR;
}


/*****************************************************************************
* �� �� ��  : ACC_ST_Cipher
*
* ��������  :��������������٣�����Cipher���ܷ�ʽ
*
* �������  : BSP_U32 u32BlockLen : ������
                            BSP_U32 u32PakNums : һ������İ�����
                            BSP_U32 CfgType : cipher��������
                            BSP_BOOL bAph :�Ƿ��APHͷ��0��ʾ������1��ʾ��
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
* ����˵��  :
*
*****************************************************************************/
BSP_S32 ACC_ST_Cipher(BSP_U32 u32BlockLen, BSP_U32 u32PakNums, BSP_U32 CfgType)
{
    BSP_S32 s32ret,i;
    BSP_U32 u32BdFifoAddr;
    ACC_SINGLE_CFG_S *pstCfg = NULL;
    CIHPER_SINGLE_CFG_S *pstCipherCfg = NULL;
    void *pInAddr = NULL;
    void *pOutAddr = NULL;
    void *pTempInAddr = NULL;
    void *pTempOutAddr = NULL;
   /* void *pTempOutAddrCip = 0xc3f00000;*/

    s32ret = ACC_ST_Init(u32BlockLen, u32PakNums, &pInAddr, &pOutAddr,&pstCfg,&pstCipherCfg);
     if(ERROR == s32ret)
    {
        return ERROR;
    }

    /*����pstCfg��Ϣ*/
    Acc_CipherCfg(pstCfg,CfgType);
    pstCfg->u32BlockLen = u32BlockLen;
    pstCfg->u32Offset = 0;
    pstCfg->u32OutLen = u32BlockLen + ACC_APH_LEN;

    pTempInAddr = (char *)((BSP_U32)pInAddr + ACC_APH_LEN);
    pTempOutAddr = (char *)((BSP_U32)pOutAddr);
    /*��ʼ���������*/
    if(FALSE == g_AccInitCmp)
    {
        s32ret = bsp_acc_init();
        if(CIPHER_NO_MEM == s32ret)
        {
            logMsg(">>>line %d FUNC %s fail, malloc failed !\n", (int)__LINE__, (int)__FUNCTION__, 0, 0, 0, 0);
            goto ERRFREE;
        }
        g_AccInitCmp = TRUE;
    }

    /*��ȡ����FIFO�׵�ַ*/
    u32BdFifoAddr = BSP_Accelerator_GetBdFifoAddr();
    if((BSP_U32)CIPHER_UNKNOWN_ERROR == u32BdFifoAddr )
    {
        logMsg(">>>line %d FUNC %s fail��result = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, u32BdFifoAddr, 0, 0, 0);
        goto ERRFREE;
    }

   /*����Cipher���ܷ�ʽ*/ 
   for(i = 0; i <(BSP_S32)u32PakNums; i++)
    {
        s32ret = BSP_Accelerator_Cipher(u32BdFifoAddr, pTempInAddr, pTempOutAddr, pstCfg);
        if(OK != s32ret)
        {
            logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
            goto ERRFREE;
        }
        pTempInAddr = (char *)((BSP_U32)pTempInAddr +u32BlockLen +ACC_APH_LEN) ;
        pTempOutAddr = (char *)((BSP_U32)pTempOutAddr + u32BlockLen + ACC_APH_LEN);
    }

    /*��ȡ��ǰͨ��״̬��������ʹ���������*/
    while(BSP_Accelerator_GetStatus() != CHN_FREE)
    {
        g_ChnBusy ++;
    }
    s32ret = BSP_Accelerator_Enable(u32BdFifoAddr);
    if(CIPHER_UNKNOWN_ERROR == s32ret)
    {
        logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
        goto ERRFREE;
    }   

    /*taskDelay(100);*/

    /*����Cipher���ܺ��������ܼ���֮�������****************************************/
    pstCipherCfg->bMemBlock = 1;
    pstCipherCfg->enOpt = 1;
    pstCipherCfg->u8Direction = 0;
    pstCipherCfg->stAlgKeyInfo = pstCfg->stAlgKeyInfo;
    pstCipherCfg->u32Count = pstCfg->u32Count;
    pstCipherCfg->enAppdHeaderLen = 0;
    pstCipherCfg->u8BearId = (BSP_U8)pstCfg->u32BearId;
    pstCipherCfg->enHeaderLen = 0;
    pstCipherCfg->u32BlockLen = pstCfg->u32BlockLen;
        
    pTempOutAddr = (char *)((BSP_U32)pOutAddr + ACC_APH_LEN);
    for(i = 0; i <(BSP_S32)u32PakNums; i++)
    {
        s32ret = BSP_CIPHER_SingleSubmitTask(CIPHER_CHN,pTempOutAddr, pTempOutAddr,pstCipherCfg,1,0);
        if(ERROR == s32ret)
        {
            logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
            goto ERRFREE;
        }
		/*
        taskDelay(50);
        BSP_REG_WRITE(CIPHER_REGBASE_ADDR, CIPHER_CHN_RESET(CIPHER_CHN),0x4);
        */
        pTempOutAddr = (char*)((BSP_U32)pTempOutAddr + u32BlockLen + ACC_APH_LEN);
       /* (char*)pTempOutAddrCip = (char*)pTempOutAddrCip + u32BlockLen + ACC_APH_LEN;*/
    }
        
    /*�Ƚ����ǰ�����Ƿ���ͬ*/
   /* pTempOutAddrCip = 0xc3f00000;*/
    for(i = 0; i <(BSP_S32)u32PakNums; i++)
    {
        pTempInAddr = (char *)((BSP_U32)pInAddr + ACC_APH_LEN);
        pTempOutAddr = (char *)((BSP_U32)pOutAddr + ACC_APH_LEN);
        s32ret = AccDataCmp(pTempInAddr, pTempOutAddr,u32BlockLen);
        if(ERROR == s32ret)
        {
            logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
            goto ERRFREE;
        }
        pTempInAddr = (char *)((BSP_U32)pTempInAddr +u32BlockLen +ACC_APH_LEN) ;
        pTempOutAddr = (char *)((BSP_U32)pTempOutAddr + u32BlockLen + ACC_APH_LEN);
    }

    CIPHER_FREE(pstCipherCfg);
    CIPHER_FREE(pstCfg);
    CIPHER_FREE(pInAddr);
    CIPHER_FREE(pOutAddr);

    return OK;
    ERRFREE: 
        CIPHER_FREE(pstCipherCfg);
        CIPHER_FREE(pstCfg);
        CIPHER_FREE(pInAddr);
        CIPHER_FREE(pOutAddr);
        return ERROR;
       
}

/*****************************************************************************
* �� �� ��  : ACC_ST_DMA_Cipher_001
*
* ��������  :��������������٣�����DMA ��Cipher���ܽ���ķ�ʽ
*
* �������  : BSP_U32 u32BlockLen : ������
                            BSP_U32 u32PakNums : һ������İ�����(���ܴ���64)
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
* ����˵��  :
*
*****************************************************************************/
BSP_S32 ACC_ST_DMA_Cipher_001(BSP_U32 u32BlockLen, BSP_U32 u32PakNums, BSP_U32 u32Offset, BSP_U32 CfgType)
{
    BSP_S32 s32ret = 0,i;
    BSP_U32 u32BdFifoAddr;
    ACC_SINGLE_CFG_S *pstCfg = NULL;
    CIHPER_SINGLE_CFG_S *pstCipherCfg = NULL;
    void *pInAddr = NULL ;
    void *pOutAddr = NULL;
    void *pTempInAddr = NULL;
    void *pTempOutAddr = NULL;

    if(cipher_init())
    {
        logMsg(">>>line %d FUNC %s fail, Cipher Init Failed! return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
        return ERROR;
    }

    
    s32ret = ACC_ST_Init(u32BlockLen, 2*u32PakNums, &pInAddr, &pOutAddr,&pstCfg,&pstCipherCfg);
    if(ERROR == s32ret)
    {
        return ERROR;
    }

     /*����pstCfg��Ϣ*/
    
    pstCfg->u32BlockLen = u32BlockLen;
    pstCfg->u32Offset = u32Offset;
    pstCfg->u32OutLen = u32BlockLen - u32Offset;

    pTempInAddr = pInAddr;
    pTempOutAddr = pOutAddr;
    /*��ʼ���������*/
    if(FALSE == g_AccInitCmp)
    {
        s32ret = bsp_acc_init();
        if(CIPHER_NO_MEM == s32ret)
        {
            logMsg(">>>line %d FUNC %s fail, malloc failed !\n", (int)__LINE__, (int)__FUNCTION__, 0, 0, 0, 0);
            goto ERRFREE;
        }
         g_AccInitCmp = TRUE;
    }
    
    /*��ȡ����FIFO�׵�ַ*/
    u32BdFifoAddr = BSP_Accelerator_GetBdFifoAddr();
    if((BSP_U32)CIPHER_UNKNOWN_ERROR == u32BdFifoAddr )
    {
        logMsg(">>>line %d FUNC %s fail��result = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, u32BdFifoAddr, 0, 0, 0);
        goto ERRFREE;
    }

   /*����Cipher���ܷ�ʽ*/ 
   for(i = 0; i <(BSP_S32)u32PakNums; i++)
    {
        s32ret = BSP_Accelerator_DMA(u32BdFifoAddr, pTempInAddr, pTempOutAddr, pstCfg);
        if(OK != s32ret)
        {
            logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
            goto ERRFREE;
        }
        pTempInAddr = (char *)((BSP_U32)pTempInAddr +u32BlockLen);
        pTempOutAddr = (char *)((BSP_U32)pTempOutAddr + u32BlockLen);

        Acc_CipherCfg(pstCfg,CfgType);
        s32ret = BSP_Accelerator_Cipher(u32BdFifoAddr, pTempInAddr, pTempOutAddr, pstCfg);
        if(OK != s32ret)
        {
            logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
            goto ERRFREE;
        }
        pTempInAddr = (char *)((BSP_U32)pTempInAddr +u32BlockLen);
        pTempOutAddr = (char *)((BSP_U32)pTempOutAddr + u32BlockLen);
    }

    /*��ȡ��ǰͨ��״̬��������ʹ���������*/
    while(BSP_Accelerator_GetStatus() != CHN_FREE)
    {
        g_ChnBusy ++;
    }
    s32ret = BSP_Accelerator_Enable(u32BdFifoAddr);
    if(CIPHER_UNKNOWN_ERROR == s32ret)
    {
        logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
        goto ERRFREE;
    }   

    /*����Cipher���ܺ��������ܼ���֮�������****************************************/

    pstCipherCfg->bMemBlock = 1;
    pstCipherCfg->enOpt = 1;
    pstCipherCfg->u8Direction = 0;
    pstCipherCfg->stAlgKeyInfo = pstCfg->stAlgKeyInfo;
    pstCipherCfg->u32Count = pstCfg->u32Count;
    pstCipherCfg->enAppdHeaderLen = 0;
    pstCipherCfg->u8BearId = (BSP_U8)pstCfg->u32BearId;
    pstCipherCfg->enHeaderLen = 0;
    pstCipherCfg->u32BlockLen = pstCfg->u32BlockLen;

    pTempOutAddr = (char *)((BSP_U32)pOutAddr + u32BlockLen);
    for(i = 0; i <(BSP_S32)u32PakNums; i++)
    {
        s32ret = BSP_CIPHER_SingleSubmitTask(CIPHER_CHN,pTempOutAddr, pTempOutAddr,pstCipherCfg,1,0);
        if(ERROR == s32ret)
        {
            logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
            goto ERRFREE;
        }
		/*
        taskDelay(50);
        BSP_REG_WRITE(CIPHER_REGBASE_ADDR, CIPHER_CHN_RESET(CIPHER_CHN),0x4);
        */
        pTempOutAddr = (char *)((BSP_U32)pTempOutAddr + u32BlockLen*2);
    }
        
    
    /*�Ƚ����ǰ�����Ƿ���ͬ*/
    s32ret = AccDataCmp(pInAddr, pOutAddr,2*u32BlockLen * u32PakNums);
    if(ERROR == s32ret)
    {
        logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
        goto ERRFREE;
    }

    CIPHER_FREE(pstCipherCfg);
    CIPHER_FREE(pstCfg);
    CIPHER_FREE(pInAddr);
    CIPHER_FREE(pOutAddr);

    return OK;
    ERRFREE: 
        CIPHER_FREE(pstCipherCfg);
        CIPHER_FREE(pstCfg);
        CIPHER_FREE(pInAddr);
        CIPHER_FREE(pOutAddr);
        return ERROR;
       
}

int ACC_UsrClkSendIsr()
{
    semGive(g_UsrClkSendCountSem);
    return OK;
}

/*****************************************************************************
* �� �� ��  : ACC_UsrClk_Send
*
* ��������  :�ҽӺ����ж�
*
* �������  : SendRate: �жϼ��
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
* ����˵��  :
*
*****************************************************************************/
#if 0
int ACC_UsrClk_Send(int SendRate)
{
    BSP_S32 * ps32UsrClkId;

    ps32UsrClkId = (BSP_S32 *)CIPHER_MALLOC(sizeof(BSP_S32));
    BSP_USRCLK_Alloc(ps32UsrClkId);
    g_UsrClkID = *ps32UsrClkId;

    if (OK != BSP_USRCLK_RateSet(SendRate, g_UsrClkID))
    {
    	logMsg(">>>line %d FUNC %s fail!\n", (int)__LINE__, (int)__FUNCTION__, 0, 0, 0, 0);
    	return ERROR;
    }

    if (OK != BSP_USRCLK_Connect((FUNCPTR)ACC_UsrClkSendIsr, 0, g_UsrClkID))
    {
    	logMsg(">>>line %d FUNC %s fail!\n", (int)__LINE__, (int)__FUNCTION__, 0, 0, 0, 0);
    	return ERROR;
    }

    if (OK != BSP_USRCLK_Enable(g_UsrClkID))
    {
    	logMsg(">>>line %d FUNC %s fail!\n", (int)__LINE__, (int)__FUNCTION__, 0, 0, 0, 0);
    	return ERROR;
    }

    return OK;
}
#endif

BSP_S32 ACC_EnableFunc(BSP_U32 u32BlockLen, BSP_U32 u32PakNums,    void *pInAddr, void *pOutAddr, 
                    ACC_SINGLE_CFG_S *pstCfg, CIHPER_SINGLE_CFG_S *pstCipherCfg)
{
    SEM_ID sem;
    BSP_S32 s32ret;
    BSP_U32 u32BdFifoAddr;
    void *pTempInAddr = NULL;
    void *pTempOutAddr = NULL;
    int i = 0;
    
    sem = g_UsrClkSendCountSem;
    
    
    while (g_bIsAcc)
    {
        pTempInAddr = pInAddr;
        pTempOutAddr = pOutAddr;
        if (semTake(sem, WAIT_FOREVER) == ERROR)
        {
            logMsg(">>>line %d FUNC %s fail\n", (int)__LINE__, (int)__FUNCTION__, 0, 0, 0, 0);
           goto ERRFREE;
        }
       
        /*��ȡ����FIFO�׵�ַ*/
        u32BdFifoAddr = BSP_Accelerator_GetBdFifoAddr();
        if((BSP_U32)CIPHER_UNKNOWN_ERROR == u32BdFifoAddr )
        {
            logMsg(">>>line %d FUNC %s fail��result = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, u32BdFifoAddr, 0, 0, 0);
            goto ERRFREE;
        }

        /*����Cipher���ܷ�ʽ*/ 
        for(i = 0; i <(BSP_S32)u32PakNums; i++)
        {
            s32ret = BSP_Accelerator_DMA(u32BdFifoAddr, pTempInAddr, pTempOutAddr, pstCfg);
            if(OK != s32ret)
            {
                logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
                goto ERRFREE;
            }
            pTempInAddr = (char *)((BSP_U32)pTempInAddr +u32BlockLen);
            pTempOutAddr = (char *)((BSP_U32)pTempOutAddr + u32BlockLen);
                
            s32ret = BSP_Accelerator_Cipher(u32BdFifoAddr, pTempInAddr, pTempOutAddr, pstCfg);
            if(OK != s32ret)
            {
                logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
                goto ERRFREE;
            }
            pTempInAddr = (char *)((BSP_U32)pTempInAddr +u32BlockLen);
            pTempOutAddr = (char *)((BSP_U32)pTempOutAddr + u32BlockLen);
        }

       
        /*logMsg("packetNum:%d\n",packetNum,0,0,0,0,0);*/
    
       

        /*��ȡ��ǰͨ��״̬��������ʹ���������*/
        while(BSP_Accelerator_GetStatus() != CHN_FREE)
        {
            semTake(sem, WAIT_FOREVER);
            g_ChnBusy ++;
        }
        s32ret = BSP_Accelerator_Enable(u32BdFifoAddr);
        if(CIPHER_UNKNOWN_ERROR == s32ret)
        {
            logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
            goto ERRFREE;
        }   

         /*����Cipher���ܺ��������ܼ���֮�������****************************************/

        pstCipherCfg->bMemBlock = 1;
        pstCipherCfg->enOpt = 1;
        pstCipherCfg->u8Direction = 0;
        pstCipherCfg->stAlgKeyInfo = pstCfg->stAlgKeyInfo;
        pstCipherCfg->u32Count = pstCfg->u32Count;
        pstCipherCfg->enAppdHeaderLen = 0;
        pstCipherCfg->u8BearId = (BSP_U8)pstCfg->u32BearId;
        pstCipherCfg->enHeaderLen = 0;
        pstCipherCfg->u32BlockLen = pstCfg->u32BlockLen;

        pTempOutAddr = (char *)pOutAddr + u32BlockLen;
        for(i = 0; i <(BSP_S32)u32PakNums; i++)
        {
            s32ret = BSP_CIPHER_SingleSubmitTask(CIPHER_CHN,pTempOutAddr, pTempOutAddr,pstCipherCfg,1,0);
            if(ERROR == s32ret)
            {
                logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
               goto ERRFREE;
            }
			/*
            taskDelay(50);
            BSP_REG_WRITE(CIPHER_BASE_ADDR, CIPHER_CHN_RESET(CIPHER_CHN),0x4);
            */
            pTempOutAddr = (char *)((BSP_U32)pTempOutAddr + u32BlockLen*2);
        }
            
        /*�Ƚ����ǰ�����Ƿ���ͬ*/
        s32ret = AccDataCmp(pInAddr, pOutAddr,2*u32BlockLen * u32PakNums);
        if(ERROR == s32ret)
        {
            logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
            goto ERRFREE;
        }

    }
  
    return OK;
    ERRFREE: 
        CIPHER_FREE(pstCipherCfg);
        CIPHER_FREE(pstCfg);
        CIPHER_FREE(pInAddr);
        CIPHER_FREE(pOutAddr);
        return ERROR;
}


/*****************************************************************************
* �� �� ��  : ACC_ST_DMA_Cipher_002
*
* ��������  :�ҽӺ����жϣ�ģ����ʵ����
*
* �������  : BSP_U32 u32BlockLen : ������
                            BSP_U32 u32PakNums : һ������İ�����(���ܴ���64)
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
* ����˵��  :
*
*****************************************************************************/
#if 0
BSP_S32 ACC_ST_DMA_Cipher_002(BSP_U32 u32BlockLen, BSP_U32 u32PakNums, BSP_U32 u32Offset, BSP_U32 CfgType)
{
    BSP_S32 s32ret;
    ACC_SINGLE_CFG_S *pstCfg = NULL;
    CIHPER_SINGLE_CFG_S *pstCipherCfg = NULL;
    void *pInAddr = NULL;
    void *pOutAddr = NULL;
    void *pTempInAddr = NULL;
    void *pTempOutAddr = NULL;

    if(cipher_init())
    {
        logMsg(">>>line %d FUNC %s fail, Cipher Init Failed! return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
        return ERROR;
    }
    if(BSP_USRCLK_Init())
    {
        logMsg(">>>line %d FUNC %s fail, Cipher Init Failed! return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
        return ERROR;
    }
   
    s32ret = ACC_ST_Init(u32BlockLen, 2*u32PakNums, &pInAddr, &pOutAddr,&pstCfg,&pstCipherCfg);
    if(ERROR == s32ret)
    {
        return ERROR;
    }

    g_CipherCfgAddr = (BSP_U32)pstCipherCfg;
    g_CfgAddr = (BSP_U32)pstCfg;
    g_InDataAddr = (BSP_U32)pInAddr;
    g_OutDataAddr = (BSP_U32)pOutAddr;

    /*����pstCfg��Ϣ*/
    Acc_CipherCfg(pstCfg,CfgType);
    pstCfg->u32BlockLen = u32BlockLen;
    pstCfg->u32Offset = u32Offset;
    pstCfg->u32OutLen = u32BlockLen - u32Offset;

    g_UsrClkSendCountSem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
    if (g_UsrClkSendCountSem == NULL)
    {
        logMsg(">>>line %d FUNC %s fail\n", (int)__LINE__, (int)__FUNCTION__, 0, 0, 0, 0);
         goto ERRFREE;
    }
    
    

    /*��ʼ���������*/
    if(FALSE == g_AccInitCmp)
    {
        s32ret = bsp_acc_init();
        if(CIPHER_NO_MEM == s32ret)
        {
            logMsg(">>>line %d FUNC %s fail, malloc failed !\n", (int)__LINE__, (int)__FUNCTION__, 0, 0, 0, 0);
            goto ERRFREE;
        }
         g_AccInitCmp = TRUE;
    }

    if (OK != ACC_UsrClk_Send(1000))
    {
        logMsg(">>>line %d FUNC %s fail\n", (int)__LINE__, (int)__FUNCTION__, 0, 0, 0, 0);
         goto ERRFREE;
    }
    s32ret = taskSpawn ("tmAccEnableFunc", 132, 0, 20000, (FUNCPTR)ACC_EnableFunc, (int)u32BlockLen,
                                                (int)u32PakNums, (int)pInAddr, (int)pOutAddr,(int)pstCfg, (int)pstCipherCfg, 0, 0, 0, 0);
        
    if (ERROR == s32ret)
    {
        logMsg(">>>line %d FUNC %s fail��s32ret = %d\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
         goto ERRFREE;
    }
    
    return OK;
    ERRFREE: 
        CIPHER_FREE(pstCipherCfg);
        CIPHER_FREE(pstCfg);
        CIPHER_FREE(pInAddr);
        CIPHER_FREE(pOutAddr);
        return ERROR;
    
}
#endif

/*****************************************************************************
* �� �� ��  : ACC_ST_DMA_001
*
* ��������  :�������е�������������٣�����DMA���Ʒ�ʽ��ÿ��������Ϊ23
*
* �������  :BSP_U32 u32LoopTimes : ѭ������
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
* ����˵��  :
*
*****************************************************************************/
BSP_S32 ACC_ST_DMA_001(BSP_U32 u32BlockLen, BSP_U32 u32PakNums,BSP_U32 u32LoopTimes)
{
    BSP_S32 s32ret = ERROR;
    
    while(u32LoopTimes > 0)
    {
        s32ret = ACC_ST_DMA(u32BlockLen,u32PakNums);
        if(ERROR == s32ret)
        {
            logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
            return ERROR;
        }
        u32LoopTimes--;
    }
    return OK;
    
}

/*****************************************************************************
* �� �� ��  : ACC_ST_DMA_002
*
* ��������  :��������������������������������٣�����DMA���Ʒ�ʽ
*
* �������  :BSP_U32 u32LoopTimes : ѭ������
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
* ����˵��  :
*
*****************************************************************************/
BSP_S32 ACC_ST_DMA_002(BSP_U32 u32LoopTimes)
{
    BSP_S32 s32ret = ERROR;
    BSP_U32 u32randLen = 0;
    BSP_U32 u32randPak = 0;

    while(u32LoopTimes > 0)
    {
        u32randLen = (double)rand() / (RAND_MAX + 1) * (65536 - 1) + 1;
        u32randPak = (double)rand() / (RAND_MAX + 1) * (128 - 1) + 1;
        s32ret = ACC_ST_DMA(u32randLen,u32randPak);
        if(ERROR == s32ret)
        {
            logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
            return ERROR;
        }
        u32LoopTimes--;
    }
    return OK;
    
}

/*****************************************************************************
* �� �� ��  : ACC_ST_Cipher_001
*
* ��������  :�������е�������������٣�����Cipher���ܷ�ʽ��ÿ��������Ϊ23
*
* �������  :BSP_U32 u32LoopTimes : ѭ������
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
* ����˵��  :
*
*****************************************************************************/
BSP_S32 ACC_ST_Cipher_001(BSP_U32 u32BlockLen, BSP_U32 u32PakNums,BSP_U32 u32LoopTimes, BSP_U32 CfgType)
{
    BSP_S32 s32ret = ERROR;
    
    if(cipher_init())
    {
        logMsg(">>>line %d FUNC %s fail, Cipher Init Failed! return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
        return ERROR;
    }
    
    while(u32LoopTimes > 0)
    {
        s32ret = ACC_ST_Cipher(u32BlockLen,u32PakNums, CfgType);
        if(ERROR == s32ret)
        {
            logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
            return ERROR;
        }
        u32LoopTimes--;
    }
    return OK;
    
}

/*****************************************************************************
* �� �� ��  : ACC_ST_Cipher_002
*
* ��������  :���������������������������������,����Cipher���ܷ�ʽ
*
* �������  :BSP_U32 u32LoopTimes : ѭ������
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
* ����˵��  :
*
*****************************************************************************/
BSP_S32 ACC_ST_Cipher_002(BSP_U32 u32LoopTimes, BSP_U32 CfgType)
{
    BSP_S32 s32ret = ERROR;
    BSP_U32 u32randLen = 0;
    BSP_U32 u32randPak = 0;

     if(cipher_init())
    {
        logMsg(">>>line %d FUNC %s fail, Cipher Init Failed! return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
        return ERROR;
    }

    while(u32LoopTimes > 0)
    {
        u32randLen = (double)rand() / (RAND_MAX + 1) * (65536 - 10) + 10;
        u32randPak = (double)rand() / (RAND_MAX + 1) * (128 - 1) + 1;
        s32ret = ACC_ST_Cipher(u32randLen,u32randPak,CfgType);
        if(ERROR == s32ret)
        {
            logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
            return ERROR;
        }
        u32LoopTimes--;
    }
    return OK;
    
}

/*****************************************************************************
* �� �� ��  : ACC_ST_Cipher_003
*
* ��������  :�������в��ֿ����,����Cipher���ܷ�ʽ
*
* �������  :BSP_U32 u32LoopTimes : ѭ������
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
* ����˵��  :
*
*****************************************************************************/
BSP_S32 ACC_ST_Cipher_003(BSP_U32 u32BlockLen, BSP_U32 u32PakNums, BSP_U32 u32Offset, BSP_U32 u32LoopTimes, BSP_U32 CfgType)
{
    BSP_S32 s32ret = ERROR;
    BSP_U32 u32BdFifoAddr,i;
    ACC_SINGLE_CFG_S *pstCfg = NULL;
    CIHPER_SINGLE_CFG_S *pstCipherCfg = NULL;
    void *pInAddr = NULL;
    void *pOutAddr = NULL;
    void *pTempInAddr = NULL;
    void *pTempOutAddr = NULL;

    if(cipher_init())
    {
        logMsg(">>>line %d FUNC %s fail, Cipher Init Failed! return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
        return ERROR;
    }

    s32ret = ACC_ST_Init(u32BlockLen, u32PakNums, &pInAddr, &pOutAddr,&pstCfg,&pstCipherCfg);
    if(ERROR == s32ret)
    {
        return ERROR;
    }

     /*����pstCfg��Ϣ*/
    Acc_CipherCfg(pstCfg,CfgType);
    
    
    pTempInAddr = (void*)((BSP_U32)pInAddr + ACC_APH_LEN);
    pTempOutAddr = pOutAddr;
    /*��ʼ���������*/
    if(FALSE == g_AccInitCmp)
    {
        s32ret = bsp_acc_init();
        if(CIPHER_NO_MEM == s32ret)
        {
            logMsg(">>>line %d FUNC %s fail, malloc failed !\n", (int)__LINE__, (int)__FUNCTION__, 0, 0, 0, 0);
            goto ERRFREE;
        }
         g_AccInitCmp = TRUE;
    }

    /*��ȡ����FIFO�׵�ַ*/
    u32BdFifoAddr = BSP_Accelerator_GetBdFifoAddr();
    if((BSP_U32)CIPHER_UNKNOWN_ERROR == u32BdFifoAddr )
    {
        logMsg(">>>line %d FUNC %s fail��result = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, u32BdFifoAddr, 0, 0, 0);
        goto ERRFREE;
    }

   /*����Cipher���ܷ�ʽ*/ 
   for(i = 0; i <u32PakNums; i++)
    {
        pstCfg->u32BlockLen = u32BlockLen;
        pstCfg->u32Offset = 0;
        pstCfg->u32OutLen = u32Offset;
        s32ret = BSP_Accelerator_Cipher(u32BdFifoAddr, pTempInAddr, pTempOutAddr, pstCfg);
        if(OK != s32ret)
        {
            logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
            goto ERRFREE;
        }
        pTempOutAddr = (char *)pTempOutAddr + u32Offset;
        pstCfg->u32BlockLen = u32BlockLen;
        pstCfg->u32Offset = u32Offset;
        pstCfg->u32OutLen = u32BlockLen - u32Offset + ACC_APH_LEN;
        s32ret = BSP_Accelerator_Cipher(u32BdFifoAddr, pTempInAddr, pTempOutAddr, pstCfg);
        if(OK != s32ret)
        {
            logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
            goto ERRFREE;
        }
        pTempInAddr = (char *)pTempInAddr +u32BlockLen + ACC_APH_LEN;
        pTempOutAddr = (char *)pTempOutAddr + (u32BlockLen - u32Offset) + ACC_APH_LEN;
    }

    /*��ȡ��ǰͨ��״̬��������ʹ���������*/
    while(BSP_Accelerator_GetStatus() != CHN_FREE)
    {
        g_ChnBusy ++;
    }
    s32ret = BSP_Accelerator_Enable(u32BdFifoAddr);
    if(CIPHER_UNKNOWN_ERROR == s32ret)
    {
        logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
        goto ERRFREE;
    }   

    /*����Cipher���ܺ��������ܼ���֮�������****************************************/
    pstCipherCfg->bMemBlock = 1;
    pstCipherCfg->enOpt = 1;
    pstCipherCfg->u8Direction = 0;
    pstCipherCfg->stAlgKeyInfo = pstCfg->stAlgKeyInfo;
    pstCipherCfg->u32Count = pstCfg->u32Count;
    pstCipherCfg->enAppdHeaderLen = 0;
    pstCipherCfg->u8BearId = (BSP_U8)pstCfg->u32BearId;
    pstCipherCfg->enHeaderLen = 0;
    pstCipherCfg->u32BlockLen = pstCfg->u32BlockLen;

    pTempOutAddr =  (void *)((BSP_U32)pOutAddr + ACC_APH_LEN);
    for(i = 0; i <u32PakNums; i++)
    {
        s32ret = BSP_CIPHER_SingleSubmitTask(CIPHER_CHN,pTempOutAddr, pTempOutAddr,pstCipherCfg,1,0);
        if(ERROR == s32ret)
        {
            logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
            goto ERRFREE;
        }
		/*
        taskDelay(50);
        BSP_REG_WRITE(CIPHER_BASE_ADDR, CIPHER_CHN_RESET(CIPHER_CHN),0x4);
        */
        pTempOutAddr = (char *)pTempOutAddr + u32BlockLen + ACC_APH_LEN;
    }
        
    /*�Ƚ����ǰ�����Ƿ���ͬ*/
     /*�Ƚ����ǰ�����Ƿ���ͬ*/
    for(i = 0; i <u32PakNums; i++)
    {
        pTempInAddr = (void *)((BSP_U32)pInAddr + ACC_APH_LEN);
        pTempOutAddr = (void *)((BSP_U32)pOutAddr + ACC_APH_LEN);
        s32ret = AccDataCmp(pTempInAddr, pTempOutAddr,u32BlockLen);
        if(ERROR == s32ret)
        {
            logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
            goto ERRFREE;
        }
        pTempInAddr = (void *)((BSP_U32)pTempInAddr +u32BlockLen +ACC_APH_LEN) ;
        pTempOutAddr = (void *)((BSP_U32)pTempOutAddr + u32BlockLen + ACC_APH_LEN);
    }

    CIPHER_FREE(pstCipherCfg);
    CIPHER_FREE(pstCfg);
    CIPHER_FREE(pInAddr);
    CIPHER_FREE(pOutAddr);

    return OK;
    ERRFREE: 
        CIPHER_FREE(pstCipherCfg);
        CIPHER_FREE(pstCfg);
        CIPHER_FREE(pInAddr);
        CIPHER_FREE(pOutAddr);
        return ERROR;
       
}

/*****************************************************************************
* �� �� ��  : ACC_ST_Cipher_004
*
* ��������  :����ͷ�ļ���
* �������  :BSP_U32 u32LoopTimes : ѭ������
* �������  : ��
*
* �� �� ֵ  : OK&ERROR
*
* ����˵��  :
*
*****************************************************************************/
BSP_S32 ACC_ST_Cipher_004(BSP_U32 u32BlockLen, BSP_U32 u32PakNums,BSP_U32 u32LoopTimes, 
                        BSP_U32 CfgType)
{
    BSP_S32 s32ret = ERROR;
    
    if(cipher_init())
    {
        logMsg(">>>line %d FUNC %s fail, Cipher Init Failed! return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
        return ERROR;
    }
    
    while(u32LoopTimes > 0)
    {
        s32ret = ACC_ST_Cipher(u32BlockLen,u32PakNums, CfgType);
        if(ERROR == s32ret)
        {
            logMsg(">>>line %d FUNC %s fail��return = 0x%x\n", (int)__LINE__, (int)__FUNCTION__, s32ret, 0, 0, 0);
            return ERROR;
        }
        u32LoopTimes--;
    }
    return OK;
    
}

BSP_S32 ACC_ST_DMA_01()
{
    return ACC_ST_DMA_001(1500, 127, 10);
}

BSP_S32 ACC_ST_Cipher_01()
{
    ACC_APH_LEN = 1;
    return ACC_ST_Cipher_001(1500, 127, 10, 2);
    
}

BSP_S32 ACC_ST_Cipher_02()
{
    ACC_APH_LEN = 0;
    return ACC_ST_Cipher_003(1500, 60,500,10,1 );
}

BSP_S32 ACC_ST_DMA_Cipher_01()
{
    return ACC_ST_DMA_Cipher_001(1500, 60, 0 ,1 );
}
