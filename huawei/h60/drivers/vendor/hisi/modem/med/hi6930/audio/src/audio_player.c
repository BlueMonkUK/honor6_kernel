
/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "audio_player.h"
#include "audio_pcm.h"
#include "mp3_dec.h"
#include "xa_aac_dec.h"
#include "om_log.h"
#include "xa_src_pp.h"
#include "dm3.h"
#include "ucom_mem_dyn.h"
#include "audio_enhance.h"
#include "codec_com_codec.h"
#include "om_cpuview.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    ��ά�ɲ���Ϣ�а�����C�ļ���ź궨��
*****************************************************************************/
/*lint -e(767)*/
#define THIS_FILE_ID                    OM_FILE_ID_AUDIO_PLAYER_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/* ����MP3/AAC����Ƶͨ·��ؿ�����Ϣ */
UCOM_SET_UNINIT
AUDIO_PLAYER_OBJ_STRU                   g_stAudioPlayerObj;

/* ����������������Buffer��Ϣ */
AUDIO_PLAYER_BUF_STRU                   g_stAudioPlayerBuff;

/* �����ý������� */
AUDIO_PLAYER_SWAP_BUF_STRU              g_stAudioPlayerSwapBuf;

/* ���뻺�� */
AUDIO_PLAYER_IN_BUF_STRU                g_stAudioPlayerInBuf;

/* ����ڴ�ָ�� */
VOS_VOID                               *g_pvOutputBuff = VOS_NULL;

/* SRCһ֡���������ݻ��� */
AUDIO_PLAYER_SRC_BUF_STRU               g_stAudioPlayerSrcBuff;

/* DTS��Ч����֮��Ļ���buff */
UCOM_SEC_TCMBSS
AUDIO_PLAYER_DTS_BUF_STRU               g_stAudioPlayerDtsBuff;


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_VOID AUDIO_PLAYER_InitBuff( VOS_VOID )
{
    AUDIO_PLAYER_OBJ_STRU              *pstPlayer;
    AUDIO_PLAYER_BUF_STRU              *pstAudioPlayerBuff;
    AUDIO_PLAYER_SWAP_BUF_STRU         *pstAudioPlayerSwapBuf;
    AUDIO_PLAYER_IN_BUF_STRU           *pstAudioPlayerInBuf;
    AUDIO_PLAYER_SRC_BUF_STRU          *pstAudioPlayerSrcBuff;
    AUDIO_PLAYER_DTS_BUF_STRU          *pstAudioPlayerDtsBuff;

    /* ����AudioPlayer���ƽṹ�� */
    pstPlayer  = AUDIO_PlayerGetPlayPtr();

    pstPlayer->uwProtectWord1 = UCOM_PROTECT_WORD1;
    pstPlayer->uwProtectWord2 = UCOM_PROTECT_WORD2;
    pstPlayer->uwProtectWord3 = UCOM_PROTECT_WORD3;
    pstPlayer->uwProtectWord4 = UCOM_PROTECT_WORD4;

    /* ����������� */
    pstAudioPlayerBuff = AUDIO_PlayerGetPlayBuf();

    UCOM_MemSet(pstAudioPlayerBuff, 0, sizeof(AUDIO_PLAYER_BUF_STRU));

    pstAudioPlayerBuff->uwProtectWord1 = UCOM_PROTECT_WORD1;
    pstAudioPlayerBuff->uwProtectWord2 = UCOM_PROTECT_WORD2;
    pstAudioPlayerBuff->uwProtectWord3 = UCOM_PROTECT_WORD3;
    pstAudioPlayerBuff->uwProtectWord4 = UCOM_PROTECT_WORD4;

    /* ���������� */
    pstAudioPlayerSwapBuf   = AUDIO_PlayerGetSwapBuf();

    UCOM_MemSet(pstAudioPlayerSwapBuf, 0, sizeof(AUDIO_PLAYER_SWAP_BUF_STRU));

    pstAudioPlayerSwapBuf->uwProtectWord1 = UCOM_PROTECT_WORD1;
    pstAudioPlayerSwapBuf->uwProtectWord2 = UCOM_PROTECT_WORD2;
    pstAudioPlayerSwapBuf->uwProtectWord3 = UCOM_PROTECT_WORD3;
    pstAudioPlayerSwapBuf->uwProtectWord4 = UCOM_PROTECT_WORD4;

    /* ������������ */
    pstAudioPlayerInBuf = AUDIO_PlayerGetInputBuf();

    UCOM_MemSet(pstAudioPlayerInBuf, 0, sizeof(AUDIO_PLAYER_IN_BUF_STRU));

    pstAudioPlayerInBuf->uwProtectWord1 = UCOM_PROTECT_WORD1;
    pstAudioPlayerInBuf->uwProtectWord2 = UCOM_PROTECT_WORD2;
    pstAudioPlayerInBuf->uwProtectWord3 = UCOM_PROTECT_WORD3;
    pstAudioPlayerInBuf->uwProtectWord4 = UCOM_PROTECT_WORD4;

    /* ��������� */
    pstAudioPlayerSrcBuff = AUDIO_PlayerGetSrcBuff();

    UCOM_MemSet(pstAudioPlayerSrcBuff, 0, sizeof(AUDIO_PLAYER_SRC_BUF_STRU));

    pstAudioPlayerSrcBuff->uwProtectWord1 = UCOM_PROTECT_WORD1;
    pstAudioPlayerSrcBuff->uwProtectWord2 = UCOM_PROTECT_WORD2;
    pstAudioPlayerSrcBuff->uwProtectWord3 = UCOM_PROTECT_WORD3;
    pstAudioPlayerSrcBuff->uwProtectWord4 = UCOM_PROTECT_WORD4;

    /* ����DTS */
    pstAudioPlayerDtsBuff = AUDIO_PlayerGetDtsBuff();

    UCOM_MemSet(pstAudioPlayerDtsBuff, 0, sizeof(AUDIO_PLAYER_DTS_BUF_STRU));

    pstAudioPlayerDtsBuff->uwProtectWord1 = UCOM_PROTECT_WORD1;
    pstAudioPlayerDtsBuff->uwProtectWord2 = UCOM_PROTECT_WORD2;
    pstAudioPlayerDtsBuff->uwProtectWord3 = UCOM_PROTECT_WORD3;
    pstAudioPlayerDtsBuff->uwProtectWord4 = UCOM_PROTECT_WORD4;

}


VOS_UINT32 AUDIO_PLAYER_CheckBuff( VOS_VOID )
{
    AUDIO_PLAYER_OBJ_STRU              *pstPlayer;
    AUDIO_PLAYER_BUF_STRU              *pstAudioPlayerBuff;
    AUDIO_PLAYER_SWAP_BUF_STRU         *pstAudioPlayerSwapBuf;
    AUDIO_PLAYER_IN_BUF_STRU           *pstAudioPlayerInBuf;
    AUDIO_PLAYER_SRC_BUF_STRU          *pstAudioPlayerSrcBuff;
    AUDIO_PLAYER_DTS_BUF_STRU          *pstAudioPlayerDtsBuff;
    VOS_UINT32                          uwRet;

    uwRet = 0;

    /* ���AudioPlayer���ƽṹ�� */
    pstPlayer  = AUDIO_PlayerGetPlayPtr();

    uwRet += (pstPlayer->uwProtectWord1 ^ UCOM_PROTECT_WORD1);
    uwRet += (pstPlayer->uwProtectWord2 ^ UCOM_PROTECT_WORD2);
    uwRet += (pstPlayer->uwProtectWord3 ^ UCOM_PROTECT_WORD3);
    uwRet += (pstPlayer->uwProtectWord4 ^ UCOM_PROTECT_WORD4);

    if(uwRet != 0)
    {
        OM_LogError1(AUDIO_PlayerProtectWordFail, uwRet);
    }

    /* ��������� */
    pstAudioPlayerBuff = AUDIO_PlayerGetPlayBuf();

    uwRet += (pstAudioPlayerBuff->uwProtectWord1 ^ UCOM_PROTECT_WORD1);
    uwRet += (pstAudioPlayerBuff->uwProtectWord2 ^ UCOM_PROTECT_WORD2);
    uwRet += (pstAudioPlayerBuff->uwProtectWord3 ^ UCOM_PROTECT_WORD3);
    uwRet += (pstAudioPlayerBuff->uwProtectWord4 ^ UCOM_PROTECT_WORD4);

    if(uwRet != 0)
    {
        OM_LogError1(AUDIO_PlayerProtectWordFail, uwRet);
    }

    /* ��齻���� */
    pstAudioPlayerSwapBuf   = AUDIO_PlayerGetSwapBuf();

    uwRet += (pstAudioPlayerSwapBuf->uwProtectWord1 ^ UCOM_PROTECT_WORD1);
    uwRet += (pstAudioPlayerSwapBuf->uwProtectWord2 ^ UCOM_PROTECT_WORD2);
    uwRet += (pstAudioPlayerSwapBuf->uwProtectWord3 ^ UCOM_PROTECT_WORD3);
    uwRet += (pstAudioPlayerSwapBuf->uwProtectWord4 ^ UCOM_PROTECT_WORD4);

    if(uwRet != 0)
    {
        OM_LogError1(AUDIO_PlayerProtectWordFail, uwRet);
    }

    /* ���������� */
    pstAudioPlayerInBuf = AUDIO_PlayerGetInputBuf();

    uwRet += (pstAudioPlayerInBuf->uwProtectWord1 ^ UCOM_PROTECT_WORD1);
    uwRet += (pstAudioPlayerInBuf->uwProtectWord2 ^ UCOM_PROTECT_WORD2);
    uwRet += (pstAudioPlayerInBuf->uwProtectWord3 ^ UCOM_PROTECT_WORD3);
    uwRet += (pstAudioPlayerInBuf->uwProtectWord4 ^ UCOM_PROTECT_WORD4);

    if(uwRet != 0)
    {
        OM_LogError1(AUDIO_PlayerProtectWordFail, uwRet);
    }

    /* ������� */
    pstAudioPlayerSrcBuff = AUDIO_PlayerGetSrcBuff();

    uwRet += (pstAudioPlayerSrcBuff->uwProtectWord1 ^ UCOM_PROTECT_WORD1);
    uwRet += (pstAudioPlayerSrcBuff->uwProtectWord2 ^ UCOM_PROTECT_WORD2);
    uwRet += (pstAudioPlayerSrcBuff->uwProtectWord3 ^ UCOM_PROTECT_WORD3);
    uwRet += (pstAudioPlayerSrcBuff->uwProtectWord4 ^ UCOM_PROTECT_WORD4);

    if(uwRet != 0)
    {
        OM_LogError1(AUDIO_PlayerProtectWordFail, uwRet);
    }

    /* ���DTS */
    pstAudioPlayerDtsBuff = AUDIO_PlayerGetDtsBuff();

    uwRet += (pstAudioPlayerDtsBuff->uwProtectWord1 ^ UCOM_PROTECT_WORD1);
    uwRet += (pstAudioPlayerDtsBuff->uwProtectWord2 ^ UCOM_PROTECT_WORD2);
    uwRet += (pstAudioPlayerDtsBuff->uwProtectWord3 ^ UCOM_PROTECT_WORD3);
    uwRet += (pstAudioPlayerDtsBuff->uwProtectWord4 ^ UCOM_PROTECT_WORD4);

    if(uwRet != 0)
    {
        OM_LogError1(AUDIO_PlayerProtectWordFail, uwRet);
    }

    return uwRet;

}


VOS_UINT32 AUDIO_PLAYER_ApiInit(VOS_VOID)
{
    AUDIO_PLAYER_OBJ_STRU       *pstPlayer   = VOS_NULL;
    VOS_UINT32                  uwPcmWdSz    = AUDIO_PLAYER_PCM_WIDTH_SIZE;
    XA_AUDIO_DEC_USER_CFG_STRU  stAacUserCfg;
    XA_AUDIO_DEC_USER_CFG_STRU  stMp3UserCfg;
    XA_SRC_USER_CFG_STRU        stSrcUserCfg;
    XA_DTS_USER_CFG_STRU        stDtsUserCfg;
    VOS_UINT32                  uwApBuffAddr;
    VOS_UINT32                  uwRet       = UCOM_RET_SUCC;

    pstPlayer  = AUDIO_PlayerGetPlayPtr();

    /* �����MP3�ļ�,�����1֡λ�� */
    if (AUDIO_FILE_FORMAT_MP3 == pstPlayer->enFormat)
    {
        /* ����MP3��ʼ����Ϣ */
        UCOM_MemSet(&stMp3UserCfg, 0, sizeof(XA_AUDIO_DEC_USER_CFG_STRU));
        stMp3UserCfg.pvHeadBuff         = (VOS_VOID *)pstPlayer->uwApBufAddr;
        stMp3UserCfg.swHeaderMemSize    = AUDIO_PLAYER_BUFF_SIZE_512K;
        stMp3UserCfg.uwPcmWidthSize     = uwPcmWdSz;

        /* ��Tensilica MP3�������г�ʼ�� */
        uwRet += MP3_DECODER_Init(&stMp3UserCfg);

        /* ���ڳ�ʼ�����ĵ�AP���ݣ�����APָ�룬APʣ������ */
        uwApBuffAddr    = AUDIO_PlayerGetApBufAddr();
        uwApBuffAddr    = uwApBuffAddr + (VOS_UINT32)stMp3UserCfg.swConsumed;
        pstPlayer->uwApDataLeft = pstPlayer->uwApDataLeft - (VOS_UINT32)stMp3UserCfg.swConsumed;
        AUDIO_PlayerSetApBufAddr(uwApBuffAddr);

        pstPlayer->uwDecInMemSize = (VOS_UINT32)stMp3UserCfg.swInSize;

    }
    else if (AUDIO_FILE_FORMAT_AAC == pstPlayer->enFormat)
    {
        /* ����AAC��ʼ����Ϣ */
        UCOM_MemSet(&stAacUserCfg, 0, sizeof(XA_AUDIO_DEC_USER_CFG_STRU));
        stAacUserCfg.pvHeadBuff         = (VOS_VOID *)pstPlayer->uwApBufAddr;
        stAacUserCfg.swHeaderMemSize    = AUDIO_PLAYER_BUFF_SIZE_512K;
        stAacUserCfg.uwPcmWidthSize     = uwPcmWdSz;

        /* ��Tensilica AAC�������г�ʼ�� */
        uwRet += AAC_DECODER_Init(&stAacUserCfg);

        /* ���ڳ�ʼ�����ĵ�AP���ݣ�����APָ�룬APʣ������ */
        uwApBuffAddr    = AUDIO_PlayerGetApBufAddr();
        uwApBuffAddr    = uwApBuffAddr + (VOS_UINT32)stAacUserCfg.swConsumed;
        pstPlayer->uwApDataLeft = pstPlayer->uwApDataLeft - (VOS_UINT32)stAacUserCfg.swConsumed;
        AUDIO_PlayerSetApBufAddr(uwApBuffAddr);

        pstPlayer->uwDecInMemSize = (VOS_UINT32)stAacUserCfg.swInSize;
    }
    else
    {   /* ������ʽ�ļ��������� */
        /* �ظ�ʧ��ID_AUDIO_AP_PLAY_START_CNF��AP */
        uwRet = UCOM_RET_ERR_PARA;
    }

    /* ��ʼ������֮��ı���� */
    stSrcUserCfg.swChannels         = AUDIO_PLAYER_PLAY_CHN_NUM;
    stSrcUserCfg.swInputChunkSize   = XA_SRC_INPUT_CHUNKSIZE_128;
    stSrcUserCfg.swInputSmpRat      = (VOS_INT32)pstPlayer->uwSampleRate;
    stSrcUserCfg.swOutputSmpRat     = AUDIO_PLAYER_OUTPUT_SAMPLE_RATE;

    XA_SRC_Init(XA_SRC_PlAYBACK, &stSrcUserCfg);


    /* ����DTS�����������Ϣ������DTS���� */
    stDtsUserCfg.swChannelNum       = AUDIO_PLAYER_PLAY_CHN_NUM;
    stDtsUserCfg.uwBlockSize        = AUDIO_PLAYER_DTS_CHUNK_SIZE;
    stDtsUserCfg.uwFilterCfg        = 0;
    stDtsUserCfg.uwSampleRate       = AUDIO_PLAYER_OUTPUT_SAMPLE_RATE;

    /* ��ʼ��DTS */
    uwRet += XA_DTS_Init(&stDtsUserCfg);

    return uwRet;
}


VOS_UINT32 AUDIO_PLAYER_Init(VOS_VOID)
{
    AUDIO_PLAYER_OBJ_STRU              *pstPlayer           = VOS_NULL;
    VOS_CHAR                           *pscPlayBufPool      = VOS_NULL;
    VOS_CHAR                           *pscSrcDolbyBufPool  = VOS_NULL;
    VOS_CHAR                           *pscDtsBufPool       = VOS_NULL;
    VOS_UINT32                          uwInitDataUse       = 0;
    VOS_UINT32                          uwCurPos            = 0;
    VOS_UINT32                          uwRet               = UCOM_RET_SUCC;

    /* �õ�playerȫ�ֿ��Ʊ��� */
    pstPlayer                   = AUDIO_PlayerGetPlayPtr();

    /* �õ��������ݳ�ָ�� */
    pscPlayBufPool              = AUDIO_PlayerGetPlayBufPoolPtr();

    /* �õ�SRC��DOLBY����֮��Ļ���� */
    pscSrcDolbyBufPool          = AUDIO_PlayerGetSrcBuffPtr();

    /* �õ�DTS����֮��Ļ���� */
    pscDtsBufPool              = AUDIO_PlayerGetDtsBuffPtr();

    /* ������״̬����Ϊruning */
    AUDIO_PlayerSetState(AUDIO_PLAYER_STATE_RUNNING);

    /* �����Ѳ���buffer�����ݶ�ȡ���� */
    pstPlayer->uwPlayedBytes    = 0;

    /* ��ʼ��dts��Ч����֡�� */
    pstPlayer->stVolume.uwDtsEnableFrame = 0;

    /* ���õ�ǰΪ���� */
    pstPlayer->stVolume.uwOriginal  = 0;
    pstPlayer->stVolume.uwCurr      = 0;

    pstPlayer->uwDecBufAddr     = pstPlayer->uwApBufAddr;

    pstPlayer->uwDecDataLeft    = pstPlayer->uwApDataLeft;

    pstPlayer->uhwIsDataBak     = VOS_FALSE;

    /* ���Ž����ʼ�� */
    pstPlayer->enPlayStatus     = AUDIO_PLAY_DONE_NORMAL;

    /* ��ս���󻺳����Ϣ */
    AUDIO_PlayerSetPlayBufPoolDepth(0);

    UCOM_MemSet(pscPlayBufPool, 0, AUDIO_PLAYER_BUFF_POOL_SIZE);

    /* ���SRC��DOLBY����֮��Ļ������Ϣ */
    AUDIO_PlayerSetSrcBufPoolDepth(0);

    UCOM_MemSet(pscSrcDolbyBufPool, 0, AUDIO_PLAYER_SRC_DOLBY_BUFF_SIZE);

    /* ���DTS����֮��Ļ���� */
    UCOM_MemSet(pscDtsBufPool, 0, AUDIO_PLAYER_DTS_BUFF_SIZE * sizeof(VOS_CHAR));

    /* ִ�н���/��Ч�����API��ʼ�� */
    uwRet = AUDIO_PLAYER_ApiInit();

    if (UCOM_RET_SUCC == uwRet)
    {
        /* ��ʼ����ǰ�Ѵ������ݼ�������������resume���� */
        pstPlayer->uwPlayedBytes    = pstPlayer->uwStartPos;

        /* ���AP Buff�а����ļ�ͷ��Ϣ�����ʱͷ��Ϣ�Ѵ����ֱ꣬�Ӵ�StartPos��ʼ���н��� */
        if (VOS_FALSE == pstPlayer->uwFileHeaderRdy)
        {
            pstPlayer->uwFileHeaderRdy  = VOS_TRUE;

            uwInitDataUse   = AUDIO_PLAYER_BUFF_SIZE_512K;
        }
        else
        {
            uwInitDataUse   = pstPlayer->uwStartPos;
        }

        /* ���������ַָ��AP��Buff���ݳ�ʼ��������֮��ĵ�ַ */
        pstPlayer->uwDecBufAddr     += uwInitDataUse;

        /* ��λ��ǰ����buffer������״̬ */
        pstPlayer->uwDecDataLeft    -= uwInitDataUse;

        /* ��ʼ��InputBuff�������ĵ����ݣ���ζ�ŵ�һ֡����ǰ��Ҫ����InputBuff */
        pstPlayer->uwDecConsumed    = pstPlayer->uwDecInMemSize;

        /* ���²���λ�� */
        uwCurPos = AUDIO_PLAYER_GetCurrDecPos(pstPlayer->enFormat);
        AUDIO_PLAYER_DoSeek(uwCurPos, CODEC_SEEK_BACKWARDS);
        AUDIO_PLAYER_DoSeek(pstPlayer->uwPlayedBytes, CODEC_SEEK_FORWARDS);

    }
    else
    {
        OM_LogError1(AUDIO_PlayerXADecode_InitFail, uwRet);
    }

    return uwRet;
}
VOS_UINT32  AUDIO_PLAYER_IsIdle(VOS_VOID)
{
    /*��ʾ��ҵ�����У�ͶƱ:���µ�*/
    if (AUDIO_PLAYER_STATE_IDLE   == AUDIO_PlayerGetState())
    {
        return VOS_OK;
    }
    else
    {
        return VOS_ERR;
    }
}
VOS_VOID  AUDIO_PLAYER_CpySwapBuffIsr(
                DRV_DMA_INT_TYPE_ENUM_UINT16 enIntType,
                VOS_UINT32                   uwPara)
{
    /* ���ж�����ΪTC�ж�,ΪDMA�����жϴ������� */
    if ((DRV_DMA_INT_TYPE_TC1 == enIntType)
        ||(DRV_DMA_INT_TYPE_TC2 == enIntType))
    {
        OM_LogInfo1(AUDIO_PlayerCpySwapBuffOK, enIntType);
    }
    /* ���ж�����ΪERROR�ж�,��¼�쳣 */
    else
    {
        /*��¼�쳣������DMA Error�ж�*/
        OM_LogError1(AUDIO_PlayerCpySwapBuffFail, enIntType);
    }
}
VOS_VOID AUDIO_PLAYER_FillBuff(AUDIO_PLAYER_OBJ_STRU *pstPlayer)
{
    VOS_UINT32                      uwDataLeft      = pstPlayer->uwDecDataLeft;
    VOS_UINT32                      uwDataFill      = pstPlayer->uwDecConsumed;
    VOS_UINT32                      uwDecInputRemind;
    VOS_UINT32                      uwTotalDataLeft = 0;
    VOS_UINT32                      uwAp2InputSize;
    VOS_UINT32                      uwCopy50KCnt,uwCopyLeft,uwCnt;

    if (pstPlayer->uwPlayedBytes < pstPlayer->uwTotalFileSize)
    {
        uwTotalDataLeft = pstPlayer->uwTotalFileSize - pstPlayer->uwPlayedBytes;
    }

    XA_COMM_ShiftBuff((VOS_CHAR*)AUDIO_PlayerGetInputBufPtr(),
                  (VOS_INT32)pstPlayer->uwDecInMemSize,
                  (VOS_INT32)uwDataFill);

    /* ��ǰ�������ݴ�����buffer */
    if (VOS_TRUE != pstPlayer->uhwIsDataBak)
    {
        /* ��ǰAP bufferʣ����������200kʱ�������ݱ��ݣ���ʱDataLeft��ȷ����DecInMemSize */
        if (uwDataLeft <= AUDIO_PLAYER_BUFF_SIZE_200K)
        {
            /* ������Ҫ����50K�Ĵ��� */
            uwCopy50KCnt    = uwDataLeft/AUDIO_PLAYER_BUFF_SIZE_50K;

            /* ���������ֵ���󣬷���;��ֹ���ڴ� */
            if (uwCopy50KCnt > 4)
            {
                OM_LogError1(AUDIO_PlayerCpySwapBuffFail, uwCopy50KCnt);

                return;
            }

            /* ���㲻��50K��Ҫ���˵Ĵ�С */
            uwCopyLeft      = uwDataLeft%AUDIO_PLAYER_BUFF_SIZE_50K;

            for (uwCnt = 0; uwCnt < uwCopy50KCnt; uwCnt++)
            {
                /* ����50K���� */
                DRV_DMA_MemCpy(DRV_DMA_AUDIO_MEMCPY_CHN_NUM,
                               (VOS_UCHAR *)UCOM_GetUncachedAddr((VOS_UINT32)AUDIO_PlayerGetSwapBufAddr()+(AUDIO_PLAYER_BUFF_SIZE_50K * uwCnt)),
                               (VOS_UCHAR *)(AUDIO_PlayerGetBufAddr()+(AUDIO_PLAYER_BUFF_SIZE_50K * uwCnt)),
                               AUDIO_PLAYER_BUFF_SIZE_50K,
                               AUDIO_PLAYER_CpySwapBuffIsr,
                               0);

                /* ˯��0.5ms��ȷ��DMA������ϡ�*/
                UCOM_COMM_TimeDelayFor(500);
            }

             /* ����ʣ�಻��50K������ */
            DRV_DMA_MemCpy(DRV_DMA_AUDIO_MEMCPY_CHN_NUM,
                           (VOS_UCHAR *)UCOM_GetUncachedAddr((VOS_UINT32)AUDIO_PlayerGetSwapBufAddr()+(AUDIO_PLAYER_BUFF_SIZE_50K * 3)),
                           (VOS_UCHAR *)(AUDIO_PlayerGetBufAddr()+(AUDIO_PLAYER_BUFF_SIZE_50K * uwCopy50KCnt)),
                           uwCopyLeft,
                           AUDIO_PLAYER_CpySwapBuffIsr,
                           0);

            /* ˯��0.5ms��ȷ��DMA������ϡ�*/
            UCOM_COMM_TimeDelayFor(500);

            /* �л�����������Buff��ַ */
            AUDIO_PlayerSetBufAddr((VOS_UINT32)AUDIO_PlayerGetSwapBufAddr());

            /* ��ʼ������������ */
            pstPlayer->uwDecDataLeft    = uwDataLeft;

            pstPlayer->uwApDataLeft     = 0;

            /* ��־���ݶ�ȡ�Ӹ���buffer��ʼ */
            pstPlayer->uhwIsDataBak     = VOS_TRUE;

            if (uwTotalDataLeft > AUDIO_PLAYER_BUFF_SIZE_200K)
            {
               /* ֪ͨAP���ڲ���������Ҫ���� */
               AUDIO_PLAYER_SendPlayDoneInd(AUDIO_PLAY_DONE_NORMAL);
            }
        }

        /* ��ȡ�ܹ����Ĵ��������ݴ�С����ʱ���Ա�֤�㹻���һ��DecInput������Ҫ������䳤�� */
    }
    else
    {
        if (uwDataLeft < pstPlayer->uwDecConsumed)
        {
            /* ��ǰ�������ݴ���sub buffer�ұ��ݻ��������ݲ��㣬�л���AP Buff����ֱ�����InputBuff*/
            if ((uwDataLeft + pstPlayer->uwApDataLeft)>= pstPlayer->uwDecConsumed)
            {
                uwDataFill = pstPlayer->uwDecInMemSize - uwDataLeft;

                /* tensilica lib�����ý���inputbuff��ʣ��������ݴ�С */
                uwDecInputRemind    = pstPlayer->uwDecInMemSize - pstPlayer->uwDecConsumed;

                /* tensilica lib�����ý���inputbuff����Ҫ��AP���������ݴ�С */
                uwAp2InputSize      = pstPlayer->uwDecInMemSize - (uwDecInputRemind + uwDataLeft);

                UCOM_MemCpy((AUDIO_PlayerGetInputBufPtr() + uwDecInputRemind),
                            AUDIO_PlayerGetBufAddr(),
                            uwDataLeft);

                UCOM_MemCpy((AUDIO_PlayerGetInputBufPtr() + uwDecInputRemind + uwDataLeft),
                            AUDIO_PlayerGetApBufAddr(),
                            uwAp2InputSize);

                /* �л�����������Buff��ַ */
                AUDIO_PlayerSetBufAddr(AUDIO_PlayerGetApBufAddr()+ uwAp2InputSize);

                /* ��ʼ������������ */
                pstPlayer->uwDecDataLeft    = pstPlayer->uwApDataLeft - uwAp2InputSize;

                /* ��־���ݶ�ȡ����buffer��ʼ */
                pstPlayer->uhwIsDataBak     = VOS_FALSE;

                /* ��ո�buff�������������� */
                UCOM_MemSet(AUDIO_PlayerGetSwapBufAddr(), 0, AUDIO_PLAYER_BUFF_SIZE_200K);

                return;
            }
            else
            {
                /* ���������ݲ�����AP������δ���£����������б������� */
                uwDataFill = uwDataLeft;

                pstPlayer->uwPlayedBytes = pstPlayer->uwTotalFileSize;

                /* �澯��������������������û������������������� */
                OM_LogWarning1(AUDIO_PlayerFillBuff_NoApDataUpdate, uwDataLeft);
            }
        }
    }

    /* �ӵ�ǰDecBuffer�����InputBuff */
    UCOM_MemCpy(((VOS_CHAR *)AUDIO_PlayerGetInputBufPtr() + pstPlayer->uwDecInMemSize) - uwDataFill,
                AUDIO_PlayerGetBufAddr(),
                uwDataFill);

    /* ���´�����Buff��ַ */
    AUDIO_PlayerSetBufAddr((VOS_UINT32)AUDIO_PlayerGetBufAddr() + uwDataFill);

    /* ���´��������ݳ��� */
    pstPlayer->uwDecDataLeft -= uwDataFill;

}



VOS_UINT32 AUDIO_PLAYER_DoPlay(VOS_VOID)
{
    AUDIO_PLAYER_OBJ_STRU         *pstPlayer    = VOS_NULL ;
    CODEC_STATE_ENUM_UINT16        enDecoderState;

    /* ��ȡ����ģ������� */
    pstPlayer      = AUDIO_PlayerGetPlayPtr();

    enDecoderState = CODEC_STATE_INITIALIZED;

    /* MP3/AAC����ͨ��Buff״̬ΪActive��˵��AP�ṩ��Buff����������Ҫ���� */
    if (  (AUDIO_PLAYER_STATE_RUNNING == pstPlayer->enState)
       || (AUDIO_PLAYER_STATE_PAUSE   == pstPlayer->enState))
    {
        /* ����MP3/AAC��Ƶ���ݣ��õ���������� */
        AUDIO_PLAYER_HandleFile(&enDecoderState);

        /* ���һ�������ļ�������ϣ��ر�ͨ·��֪ͨAP */
        if (( pstPlayer->uwPlayedBytes >= pstPlayer->uwTotalFileSize)
           ||(CODEC_STATE_INITIALIZED != enDecoderState))
        {
            /* ���������ϻ��쳣�жϣ��ر�ͨ· */
            AUDIO_PLAYER_DoStop();

            /* ����ֹͣPCM����ͨ�� */
            AUDIO_PLAYER_PcmStop();

            /* ���ÿ�˽ӿ�,�ظ�AP��ϢID_AUDIO_AP_PLAY_DONE_IND������� */
            if (CODEC_STATE_FINISH == enDecoderState)
            {
                pstPlayer->enPlayStatus = AUDIO_PLAY_DONE_COMPLETE;

                AUDIO_PLAYER_SendPlayDoneInd(AUDIO_PLAY_DONE_COMPLETE);
            }
            else
            {
                pstPlayer->enPlayStatus = AUDIO_PLAY_DONE_ABNORMAL;

                AUDIO_PLAYER_SendPlayDoneInd(AUDIO_PLAY_DONE_ABNORMAL);
            }

            return UCOM_RET_SUCC;
        }
    }

    return UCOM_RET_SUCC;
}
VOS_UINT32 AUDIO_PLAYER_DoSeek(VOS_UINT32 uwSkipSize, CODEC_SEEK_DERECT_ENUM_UINT16 enSeekDirect)
{
    VOS_UINT32                     uwResult     = VOS_OK;
    AUDIO_PLAYER_OBJ_STRU         *pstPlayer    = VOS_NULL;

    pstPlayer       = AUDIO_PlayerGetPlayPtr();

    /* ����TENSILICA SEEK API�ж����ؽ�� */
    if (AUDIO_FILE_FORMAT_MP3 == pstPlayer->enFormat)
    {
        uwResult = MP3_DECODER_Seek(uwSkipSize, enSeekDirect);
    }
    else if (AUDIO_FILE_FORMAT_AAC == pstPlayer->enFormat)
    {
        uwResult = AAC_DECODER_Seek(uwSkipSize, enSeekDirect);
    }
    else
    {
        uwResult = VOS_ERR;
    }

    return uwResult;
}
VOS_UINT32 AUDIO_PLAYER_DoStop(VOS_VOID)
{
    AUDIO_PLAYER_OBJ_STRU           *pstPlayer       = VOS_NULL;

    pstPlayer = AUDIO_PlayerGetPlayPtr();

    /* ����XA api�ر�decoder */
    if (AUDIO_FILE_FORMAT_MP3 == pstPlayer->enFormat)
    {
        MP3_DECODER_Close();
    }
    else if (AUDIO_FILE_FORMAT_AAC == pstPlayer->enFormat)
    {
        AAC_DECODER_Close();
    }
    else
    {
        return UCOM_RET_FAIL;
    }

    /* XA��Чapi�ر� */
    //AUDIO_EFFECT_Close();

    /* ���¸�ͨ����Buffʹ��״̬Ϊidle״̬ */
    AUDIO_PlayerSetState(AUDIO_PLAYER_STATE_IDLE);

    return UCOM_RET_SUCC;
}
VOS_UINT32  AUDIO_PLAYER_GetCurrDecPos(AUDIO_FILE_FORMAT_ENUM_UINT16 enFormat)
{
    VOS_UINT32              uwPosInBytes = 0;
    VOS_INT32               swErrCode    = XA_NO_ERROR;

    /* �������MP3 */
    if ( AUDIO_FILE_FORMAT_MP3 == enFormat )
    {
        swErrCode = MP3_DECODER_GetCurrDecPos(&uwPosInBytes);
    }
    else if ( AUDIO_FILE_FORMAT_AAC == enFormat )
    {
        swErrCode = AAC_DECODER_GetCurrDecPos(&uwPosInBytes);
    }
    else
    {
        return UCOM_RET_FAIL;
    }

    /* ��μ�飬����ú���ִ�д��󣬽����Ž�������Ϊ0 */
    if (swErrCode != XA_NO_ERROR)
    {
        uwPosInBytes = 0;
    }

    return uwPosInBytes;
}


VOS_UINT16 AUDIO_PLAYER_GetCurrVol( VOS_VOID )
{
    AUDIO_PLAYER_OBJ_STRU              *pstPlayer       = VOS_NULL;

    /* ��ȡ����ģ������� */
    pstPlayer       = AUDIO_PlayerGetPlayPtr();

    /* ������״̬ȫ������ */
    if(AUDIO_PlayerGetState() != AUDIO_PLAYER_STATE_RUNNING)
    {
        return 0;
    }

    if (AUDIO_PLAYER_DTS_ENABLE_VOL == pstPlayer->stVolume.enSetVolEnum)
    {
        AUDIO_PLAYER_DtsMorphingVol();
    }
    else
    {
        if(pstPlayer->stVolume.uwCurr != pstPlayer->stVolume.uwTarget)
        {
            /* �����𼶱䶯, 50֡������λ */
            pstPlayer->stVolume.uwCurr += (pstPlayer->stVolume.uwTarget - pstPlayer->stVolume.uwOriginal)/50;

            if(pstPlayer->stVolume.uwTarget >= pstPlayer->stVolume.uwOriginal)
            {
                if(pstPlayer->stVolume.uwCurr >= pstPlayer->stVolume.uwTarget)
                {
                    pstPlayer->stVolume.uwCurr = pstPlayer->stVolume.uwTarget;
                }
            }
            else
            {
                if(pstPlayer->stVolume.uwCurr <= pstPlayer->stVolume.uwTarget)
                {
                    pstPlayer->stVolume.uwCurr = pstPlayer->stVolume.uwTarget;
                }
            }
        }
    }

    return (VOS_UINT16) pstPlayer->stVolume.uwCurr;
}
VOS_UINT32 AUDIO_PLAYER_MsgStartReq(VOS_VOID *pvOsaMsg)
{
    AUDIO_PLAYER_START_REQ_STRU        *pstStartReqMsg      = VOS_NULL;
    AUDIO_PLAYER_OBJ_STRU              *pstPlayer           = VOS_NULL;
    AUDIO_PLAYER_DECODE_IND_STRU        stDecodeInd;
    VOS_UINT32                          uwRet;
    VOS_VOID                           *pvMsgTail      = VOS_NULL;

    /* ��AUDIO_PLAYER_START_REQ_STRU��ʽ������Ϣ */
    pstStartReqMsg    = (AUDIO_PLAYER_START_REQ_STRU*)((MsgBlock *) pvOsaMsg)->aucValue;

    /* ����Ϣ��Ҫ��ԭ��Ϣ��CmdId��Sn�Żظ���Hifi_misc���� */
    pvMsgTail       = (VOS_VOID *)((VOS_UINT32)(((MsgBlock *) pvOsaMsg)->aucValue) + sizeof(AUDIO_PLAYER_START_REQ_STRU));

    /* ������ */
    if (  (pstStartReqMsg->enFormat >= AUDIO_FILE_FORMAT_BUT)
        ||(pstStartReqMsg->uwSampleRate > AUDIO_PCM_MAX_SAMPLE_RATE))
    {
        return UCOM_RET_ERR_PARA;
    }

    UCOM_MemSet(&stDecodeInd, 0, sizeof(AUDIO_PLAYER_DECODE_IND_STRU));

    pstPlayer   = AUDIO_PlayerGetPlayPtr();

    /* ��ʼ����λ��  */
    pstPlayer->uwStartPos          = pstStartReqMsg->uwStartPos;

    /* ����UpdateBuff�Ƿ�����ļ�ͷ������������Ҫ�����ļ�ͷ */
    if (VOS_TRUE == pstStartReqMsg->uhwWithHeader)
    {
        pstPlayer->uwFileHeaderRdy = VOS_FALSE;
    }
    else
    {
        pstPlayer->uwFileHeaderRdy = VOS_TRUE;
    }

    /* ����ָ��ͨ������Ƶ��Ϣ�������ڶ�Ӧ��ȫ�ֱ���ͨ���� */
    pstPlayer->enFormat             = pstStartReqMsg->enFormat;
    pstPlayer->uwSampleRate         = pstStartReqMsg->uwSampleRate;

    /* ����DMA���˳��ȣ�ģ��codecʵ�ʽ�֧�ֲ�����48k,16bit���ݴ���, ���Ұ���ʱֻ����˫����������ÿ�ΰ���0xF00 */
    pstPlayer->uw20msDataSize       = ((pstStartReqMsg->uwSampleRate * AUDIO_PLAYER_PLAY_CHN_NUM)\
                                       / AUDIO_PCM_FRAME_PER_SEC) * AUDIO_PCM_FORMAT_16_SIZE;

    pstPlayer->uwTotalFileSize      = pstStartReqMsg->uwTotalFileSize;

    /* ִ�в�������ʼ�� */
    uwRet = AUDIO_PLAYER_Init();

    /* ��ʼ���ɹ����������ߣ�ʧ��ֱ�ӷ��� */
    if (UCOM_RET_SUCC == uwRet)
    {
        /* ��PCMͨ������ʼ���ݴ��� */
        AUDIO_PLAYER_PcmStart();

        /* ��ʼ���ɹ�����ʼ�������ݣ�������Ч�������䲥��buffer�� */
        stDecodeInd.uhwMsgId = ID_AUDIO_PLAYER_START_DECODE_IND;

        /* ����ͨ��VOS���ͽӿڣ�������Ϣ */
        UCOM_SendOsaMsg(DSP_PID_AUDIO_RT,
                        DSP_PID_AUDIO,
                       &stDecodeInd,
                        sizeof(AUDIO_PLAYER_DECODE_IND_STRU));
    }
    else
    {
        /* �����ʼ��ʧ�ܣ�ֹͣ����ִ�� */
        stDecodeInd.uhwMsgId = ID_AUDIO_PLAYER_STOP_DECODE_IND;

        /* ����ͨ��VOS���ͽӿڣ�������Ϣ */
        UCOM_SendOsaMsg(DSP_PID_AUDIO_RT,
                        DSP_PID_AUDIO,
                       &stDecodeInd,
                        sizeof(AUDIO_PLAYER_DECODE_IND_STRU));

        OM_LogError1(AUDIO_PlayerXADecode_InitFail, uwRet);
    }

    /* �ظ�AP start req��� */
    AUDIO_PLAYER_SendApAudioCnf(ID_AUDIO_AP_PLAY_START_CNF,
                                uwRet,
                                pvMsgTail,
                                sizeof(AUDIO_HIFIMISC_MSG_TAIL_STRU));

    return uwRet;
}
VOS_UINT32  AUDIO_PLAYER_MsgStartDecodeInd(VOS_VOID *pvOsaMsg)
{
    AUDIO_PLAYER_DoPlay();

    return UCOM_RET_SUCC;
}


VOS_UINT32  AUDIO_PLAYER_MsgStopDecodeInd(VOS_VOID *pvOsaMsg)
{
    AUDIO_PLAYER_DoStop();

    return UCOM_RET_SUCC;
}


VOS_UINT32 AUDIO_PLAYER_MsgQueryTimeReq(VOS_VOID *pvOsaMsg)
{
    AUDIO_PLAYER_OBJ_STRU              *pstPlayer       = VOS_NULL;
    AUDIO_PLAYER_QUERY_TIME_CNF_STRU    stQueryTimeCnfMsg;
    VOS_VOID                           *pvMsgTail      = VOS_NULL;

    UCOM_MemSet(&stQueryTimeCnfMsg, 0, sizeof(AUDIO_PLAYER_QUERY_TIME_CNF_STRU));

    /* ����Ϣ��Ҫ��ԭ��Ϣ��CmdId��Sn�Żظ���Hifi_misc���� */
    pvMsgTail       = (VOS_VOID *)((VOS_UINT32)(((MsgBlock *) pvOsaMsg)->aucValue) + sizeof(AUDIO_PLAYER_QUERY_TIME_REQ_STRU));

    pstPlayer               = AUDIO_PlayerGetPlayPtr();

    /* ���ݵ�ǰ�Ѳ��ŵĽ��Ƚ��лظ� */
    stQueryTimeCnfMsg.uhwMsgId        = ID_AUDIO_AP_PLAY_QUERY_TIME_CNF;
    stQueryTimeCnfMsg.uwPlayedBytes   = pstPlayer->uwPlayedBytes;

    /* ���ÿ����Ƶchannel�ӿ�,֪ͨAP���Ž��� */
    UCOM_COMM_SendDspMsg(DSP_PID_AUDIO,
                         ACPU_PID_OM,           /* �˴���дAP��DRV��Ӧ��PID,,���� */
                        &stQueryTimeCnfMsg,
                         sizeof(stQueryTimeCnfMsg),
                         pvMsgTail,
                         sizeof(AUDIO_HIFIMISC_MSG_TAIL_STRU));

    return UCOM_RET_SUCC;
}


VOS_UINT32 AUDIO_PLAYER_MsgQueryStatusReq(VOS_VOID *pvOsaMsg)
{
    AUDIO_PLAYER_OBJ_STRU                  *pstPlayer           = VOS_NULL;
    AUDIO_PLAYER_QUERY_STATUS_CNF_STRU      stQueryStatusCnfMsg;
    VOS_VOID                               *pvMsgTail      = VOS_NULL;

    pstPlayer           = AUDIO_PlayerGetPlayPtr();
    /* ����Ϣ��Ҫ��ԭ��Ϣ��CmdId��Sn�Żظ���Hifi_misc���� */
    pvMsgTail       = (VOS_VOID *)((VOS_UINT32)(((MsgBlock *) pvOsaMsg)->aucValue) + sizeof(AUDIO_PLAYER_QUERY_STATUS_REQ_STRU));

    stQueryStatusCnfMsg.uhwMsgId        = ID_AUDIO_AP_PLAY_QUERY_STATUS_CNF;

    /* ���ݵ�ǰ����״̬�ظ� */
    stQueryStatusCnfMsg.enPlayStatus    = pstPlayer->enPlayStatus;

    /* ���ÿ����Ƶchannel�ӿ�,֪ͨAP���Ž��� */
    UCOM_COMM_SendDspMsg(DSP_PID_AUDIO,
                         ACPU_PID_OM,           /* �˴���дAP��DRV��Ӧ��PID,,���� */
                        &stQueryStatusCnfMsg,
                         sizeof(stQueryStatusCnfMsg),
                         pvMsgTail,
                         sizeof(AUDIO_HIFIMISC_MSG_TAIL_STRU));

    return UCOM_RET_SUCC;
}


VOS_UINT32 AUDIO_PLAYER_MsgSeekReq(VOS_VOID *pvOsaMsg)
{
    AUDIO_PLAYER_SEEK_REQ_STRU         *pstSeekReqMsg   = VOS_NULL;
    AUDIO_PLAYER_OBJ_STRU              *pstPlayer       = VOS_NULL;
    AUDIO_PLAY_RESULT_CNF_ENUM_UINT16   enSeekResult;
    VOS_UINT32                          uwResult        = UCOM_RET_FAIL;
    VOS_UINT32                          uwSkipSize;
    CODEC_SEEK_DERECT_ENUM_UINT16       enSeekDirect;
    VOS_UINT32                          uwPosInDecBytes;
    VOS_UINT32                          uwDataLeft;
    VOS_UINT32                          uwDecBuff;
    AUDIO_PLAYER_DECODE_IND_STRU        stDecodeInd;
    VOS_VOID                           *pvMsgTail      = VOS_NULL;

    UCOM_MemSet(&stDecodeInd, 0, sizeof(AUDIO_PLAYER_DECODE_IND_STRU));

    /* ��ID_AP_AUDIO_PLAY_SEEK_REQ��ʽ������Ϣ */
    pstSeekReqMsg   = (AUDIO_PLAYER_SEEK_REQ_STRU *)((MsgBlock *) pvOsaMsg)->aucValue;

    /* ����Ϣ��Ҫ��ԭ��Ϣ��CmdId��Sn�Żظ���Hifi_misc���� */
    pvMsgTail       = (VOS_VOID *)((VOS_UINT32)(((MsgBlock *) pvOsaMsg)->aucValue) + sizeof(AUDIO_PLAYER_SEEK_REQ_STRU));

    pstPlayer       = AUDIO_PlayerGetPlayPtr();

    /* IDLE״̬������Seek���� */
    if(AUDIO_PLAYER_STATE_IDLE == AUDIO_PlayerGetState())
    {
        /* �ظ�AP seekִ�н�� */
        AUDIO_PLAYER_SendPlaySeekCnf(AUDIO_PLAY_RESULT_FAIL,
                                     pvMsgTail,
                                     sizeof(AUDIO_HIFIMISC_MSG_TAIL_STRU));

        return UCOM_RET_FAIL;
    }

    /* ������player״̬Ϊ��ͣ */
    AUDIO_PlayerSetState(AUDIO_PLAYER_STATE_PAUSE);

    /* ���õ�ǰΪ���� */
    pstPlayer->stVolume.uwOriginal  = 0;
    pstPlayer->stVolume.uwCurr      = 0;

    /* ��XA�õ��ѽ���λ�� */
    uwPosInDecBytes             = AUDIO_PLAYER_GetCurrDecPos(pstPlayer->enFormat);

    if ( pstSeekReqMsg->uwSeekPos > uwPosInDecBytes )
    {
        enSeekDirect = CODEC_SEEK_FORWARDS;
        uwSkipSize   = pstSeekReqMsg->uwSeekPos - uwPosInDecBytes;
    }
    else
    {
        enSeekDirect = CODEC_SEEK_BACKWARDS;
        uwSkipSize   = uwPosInDecBytes - pstSeekReqMsg->uwSeekPos;
    }

    /* ����TENSILICA seek APIִ��seek */
    uwResult = AUDIO_PLAYER_DoSeek(uwSkipSize, enSeekDirect);

    if (VOS_OK == uwResult)
    {
        /* ����start position */
        pstPlayer->uwStartPos       = pstSeekReqMsg->uwSeekPos;

        /* ���»�ȡ��ǰ�Ѵ������ݼ��� */
        pstPlayer->uwPlayedBytes    = pstSeekReqMsg->uwSeekPos;

    }
    else
    {
        OM_LogError3(AUDIO_PlayerDecoderSeekFailed, uwResult, uwSkipSize, uwPosInDecBytes);
    }

    if( VOS_TRUE != pstSeekReqMsg->uhwIsNeedNewMem )
    {
        /* ���¼��㵱ǰ�鲥��buffer�����ݶ�ȡ���� */
        uwDataLeft = pstPlayer->uwDecDataLeft - (pstSeekReqMsg->uwSeekPos - uwPosInDecBytes);

        /* ���¼���DecBufAddr */
        uwDecBuff  = pstPlayer->uwDecBufAddr + (pstSeekReqMsg->uwSeekPos - uwPosInDecBytes);

        if((uwDecBuff >= pstPlayer->uwApBufAddr) && (0 < uwDataLeft))
        {
            pstPlayer->uwDecDataLeft    = uwDataLeft;
            pstPlayer->uwDecBufAddr     = uwDecBuff;
        }
        else
        {
            OM_LogWarning3(AUDIO_PlayerSeekDecBufCalcErr, uwDataLeft, (VOS_INT32)uwDecBuff, (VOS_INT32)pstPlayer->uwApBufAddr);

            pstPlayer->uwDecBufAddr     = pstPlayer->uwApBufAddr;
            pstPlayer->uwDecDataLeft    = pstPlayer->uwApDataLeft;
        }
    }

    if( UCOM_RET_SUCC != uwResult ) /* seek failed */
    {
        enSeekResult = AUDIO_PLAY_RESULT_FAIL;

        OM_LogInfo(AUDIO_PlayerSeekFail);
    }
    else /* seek sucessfully */
    {
        enSeekResult = AUDIO_PLAY_RESULT_OK;

        /* ��־���ݶ�ȡ����buffer��ʼ */
        pstPlayer->uhwIsDataBak     = VOS_FALSE;

        /* ���¼��㵱ǰ����bytes */
        pstPlayer->uwPlayedBytes = AUDIO_PLAYER_GetCurrDecPos(pstPlayer->enFormat);

        OM_LogInfo(AUDIO_PlayerSeekSucc);
    }

    /* �ظ�AP seekִ�н�� */
    AUDIO_PLAYER_SendPlaySeekCnf(enSeekResult,
                                 pvMsgTail,
                                 sizeof(AUDIO_HIFIMISC_MSG_TAIL_STRU));

    /* ���seekʧ�ܣ�ֹͣ��Ƶ���� */
    if(AUDIO_PLAY_RESULT_FAIL == enSeekResult)
    {
        stDecodeInd.uhwMsgId = ID_AUDIO_PLAYER_STOP_DECODE_IND;

        /* ����ͨ��VOS���ͽӿڣ�������Ϣ */
        UCOM_SendOsaMsg(DSP_PID_AUDIO_RT,
                        DSP_PID_AUDIO,
                       &stDecodeInd,
                        sizeof(AUDIO_PLAYER_DECODE_IND_STRU));

        /* ����ֹͣPCM����ͨ�� */
        AUDIO_PLAYER_PcmStop();
    }
    else
    {
        if( VOS_FALSE == pstSeekReqMsg->uhwIsNeedNewMem )
        {
            /* ����player״̬runing */
            AUDIO_PlayerSetState(AUDIO_PLAYER_STATE_RUNNING);
        }
    }

    return uwResult;
}
VOS_UINT32 AUDIO_PLAYER_MsgStopReq(VOS_VOID *pvOsaMsg)
{
    AUDIO_PLAYER_DECODE_IND_STRU        stDecodeInd;
    VOS_VOID                           *pvMsgTail      = VOS_NULL;

    /* ����Ϣ��Ҫ��ԭ��Ϣ��CmdId��Sn�Żظ���Hifi_misc���� */
    pvMsgTail       = (VOS_VOID *)((VOS_UINT32)(((MsgBlock *) pvOsaMsg)->aucValue) + sizeof(AUDIO_PLAYER_PAUSE_REQ_STRU));

    if(AUDIO_PlayerGetState() != AUDIO_PLAYER_STATE_IDLE)
    {
        UCOM_MemSet(&stDecodeInd, 0, sizeof(AUDIO_PLAYER_DECODE_IND_STRU));

        /* ����AP��ֹͣ���͹��Ĳ���ָ�����Ϣ���û���ͣ��ֹͣ����ʱ���ᷢ�� */
        stDecodeInd.uhwMsgId = ID_AUDIO_PLAYER_STOP_DECODE_IND;

        /* ����ͨ��VOS���ͽӿڣ�������Ϣ */
        UCOM_SendOsaMsg(DSP_PID_AUDIO_RT,
                    DSP_PID_AUDIO,
                   &stDecodeInd,
                    sizeof(AUDIO_PLAYER_DECODE_IND_STRU));

        /* ����ֹͣPCM����ͨ�� */
        AUDIO_PLAYER_PcmStop();
    }

    /* �ظ�APֹͣ����ִ�н�� */
    AUDIO_PLAYER_SendPlayStopCnf(pvMsgTail, sizeof(AUDIO_HIFIMISC_MSG_TAIL_STRU));

    return UCOM_RET_SUCC;
}
VOS_UINT32 AUDIO_PLAYER_MsgPlayDoneSignalReq(VOS_VOID *pvOsaMsg)
{
    VOS_UINT32                          uwRet = VOS_OK;

    /* ���Ž��� */
    uwRet = AUDIO_PLAYER_SendPlayDoneInd(AUDIO_PLAY_DONE_RESET);

    return uwRet;
}
VOS_UINT32 AUDIO_PLAYER_MsgUpdateApBuffCmd(VOS_VOID *pvOsaMsg)
{
    AUDIO_PLAYER_UPDATE_BUF_CMD_STRU       *pstUpdateCmd  = VOS_NULL;
    AUDIO_PLAYER_OBJ_STRU                  *pstPlayer     = VOS_NULL;

    /* ��AUDIO_PLAYER_UPDATE_BUF_CMD_STRU��ʽ������Ϣ */
    pstUpdateCmd            = (AUDIO_PLAYER_UPDATE_BUF_CMD_STRU*)((MsgBlock *) pvOsaMsg)->aucValue;

    pstPlayer               = AUDIO_PlayerGetPlayPtr();

    /* ��AP��Buff��δ�������ʱ������Buff��Ϣ�������������� */
    if ((0 != pstPlayer->uwApDataLeft) &&
        (AUDIO_PLAYER_STATE_RUNNING == AUDIO_PlayerGetState()))
    {
        OM_LogError(AUDIO_PlayerUpdateApBuffCmd_StateError);
    }

    pstPlayer->uwApBufAddr     = pstUpdateCmd->uwBufAddr;
    pstPlayer->uwApBufSize     = pstUpdateCmd->uwBufSize;
    pstPlayer->uwApDataLeft    = pstUpdateCmd->uwDataSize;

    /* ���Ϊseekʱ��ͣ״̬��������Ϊruning */
    if (AUDIO_PLAYER_STATE_PAUSE == AUDIO_PlayerGetState())
    {
        pstPlayer->uwDecBufAddr = pstPlayer->uwApBufAddr;
        pstPlayer->uwDecDataLeft= pstPlayer->uwApDataLeft;
        AUDIO_PlayerSetState(AUDIO_PLAYER_STATE_RUNNING);
    }
    else if(AUDIO_PLAYER_STATE_RUNNING != AUDIO_PlayerGetState())
    {
        AUDIO_PlayerSetState(AUDIO_PLAYER_STATE_BUTT);
    }
    else
    {
        /* pclint */
    }

    return UCOM_RET_SUCC;
}


VOS_UINT32 AUDIO_PLAYER_MsgUpdatePcmBuffCmd(VOS_VOID *pvOsaMsg)
{
    VOS_UINT32                          uwBufPoolDepth, uwSrcDepth;
    VOS_UINT32                          uwProcessTime,uwSrcConsum,uwChunkSize;
    VOS_CHAR                           *pSrcDolbyBuff   = VOS_NULL;
    AUDIO_PLAYER_OBJ_STRU              *pstPlayer       = VOS_NULL;
    VOS_UINT32                          uwRet           = VOS_OK;

    /* ��1�� �˴�����ǰ20ms����������������src����� */
    uwRet += AUDIO_PLAYER_ShiftBuff(AUDIO_PLAYER_SRC_BUFF, AUDIO_PLAYER_PCM_20MS_BUFF_SIZE);

    /* ��2�����б�������� */
    uwSrcDepth = AUDIO_PlayerGetSrcBufPoolDepth();
    pstPlayer  = AUDIO_PlayerGetPlayPtr();

    /* ��ʣ��src��������ݲ���һ��20ms���ˣ���������� */
    if (uwSrcDepth < AUDIO_PLAYER_PCM_20MS_BUFF_SIZE)
    {
        /* ��ȡһ֡20ms����chunksize */
        uwChunkSize     = (pstPlayer->uw20msDataSize)/sizeof(VOS_INT16);

        /* ��ȡ��Ҫ����Ĵ��� */
        uwProcessTime   = (uwChunkSize/(XA_SRC_INPUT_CHUNKSIZE_128 * AUDIO_PLAYER_PLAY_CHN_NUM)) + 1;

        /* ����src��Ҫ���Ľ���buff��С */
        uwSrcConsum     = (uwProcessTime * (XA_SRC_INPUT_CHUNKSIZE_128 * AUDIO_PLAYER_PLAY_CHN_NUM)) * sizeof(VOS_INT16);

        /* ��ȡ���µĽ������� */
        uwBufPoolDepth  = AUDIO_PlayerGetPlayBufPoolDepth();

        /* ��3����������ʣ�����ݲ���uwSrcConsum���������֡ */
        if (uwBufPoolDepth < uwSrcConsum)
        {
            uwRet += AUDIO_PLAYER_DoPlay();
        }

        /* ��ȡSRC����ء��������� */
        uwSrcDepth      = AUDIO_PlayerGetSrcBufPoolDepth();
        pSrcDolbyBuff   = AUDIO_PlayerGetSrcBuffPtr();

        uwRet += AUDIO_PLAYER_DoSrc((VOS_INT16 *)AUDIO_PlayerGetPlayBufPoolPtr(),
                                    (VOS_INT16 *)(pSrcDolbyBuff + uwSrcDepth),
                                    uwProcessTime);

        /* ��4�����ڱ���������˽���֮�����ݣ��ʽ����ĵĽ��������Ƴ� */
        uwRet += AUDIO_PLAYER_ShiftBuff(AUDIO_PLAYER_DEC_BUFF, uwSrcConsum);

    }

    /* ��5�� �������� */
    CODEC_OpVcMultR((VOS_INT16*)AUDIO_PlayerGetSrcBuffPtr(),
                    ((AUDIO_PLAYER_PCM_20MS_BUFF_SIZE)/2),
                    (VOS_INT16)AUDIO_PLAYER_GetCurrVol(),
                    (VOS_INT16*)AUDIO_PlayerGetSrcBuffPtr());

    /* ��6�� �ڷ��͸�PCMģ��֮ǰ������DTS���� */
    uwRet += (VOS_UINT32)AUDIO_PLAYER_DoDts((VOS_INT16 *)AUDIO_PlayerGetSrcBuffPtr(),
                                            (VOS_INT16 *)AUDIO_PlayerGetDtsBuffPtr());

    /* ���ܻ�������Ƿ����㹻�ѽ������ݣ�����Ҫ��PCM�������ݸ�����Ϣ */
    uwRet += AUDIO_PLAYER_SendPcmSetBufCmd((VOS_UINT32)AUDIO_PlayerGetDtsBuffPtr(), AUDIO_PLAYER_PCM_20MS_BUFF_SIZE);

    /* ���ִ��ʧ�ܣ����ϱ�error */
    if (VOS_OK != uwRet)
    {
        OM_LogError1(AUDIO_PlayerProcError, uwRet);
    }

    return uwRet;
}


VOS_UINT32 AUDIO_PLAYER_MsgSetVolReq(VOS_VOID *pvOsaMsg)
{
    AUDIO_PLAYER_SET_VOL_REQ_STRU      *pstSetVolReqMsg     = VOS_NULL;
    AUDIO_PLAYER_OBJ_STRU              *pstPlayer           = VOS_NULL;

    /* ��AUDIO_PLAYER_SET_VOL_REQ_STRU��ʽ������Ϣ */
    pstSetVolReqMsg     = (AUDIO_PLAYER_SET_VOL_REQ_STRU*)((MsgBlock *) pvOsaMsg)->aucValue;
    pstPlayer           = AUDIO_PlayerGetPlayPtr();

    pstPlayer->stVolume.uwOriginal  = pstSetVolReqMsg->uwValue;
    pstPlayer->stVolume.uwCurr      = pstSetVolReqMsg->uwValue;
    pstPlayer->stVolume.uwTarget    = pstSetVolReqMsg->uwValue;

    return VOS_OK;
}
VOS_UINT32 AUDIO_PLAYER_HandleFile(CODEC_STATE_ENUM_UINT16 *penDecoderState)
{
    VOS_CHAR                           *pscPlayBufPool      = VOS_NULL;
    AUDIO_PLAYER_OBJ_STRU              *pstPlayer           = VOS_NULL ;
    VOS_UINT32                          uwPlayBufPoolDepth;
    VOS_UINT32                          uwDecoderState;
    VOS_INT32                           swOutputSize        = 0;
    VOS_INT32                          *pswDecConsumed      = VOS_NULL;
    VOS_UINT32                          enIsEndOfStream;
    XA_AUDIO_DEC_USER_CFG_STRU          stDecUserCfg;                           /* MP3��AAC����������Ϣ */
    VOS_UINT32                          uwErrorCode;

    uwDecoderState     = CODEC_STATE_INITIALIZED;

    /* ��ȡ������������buffer pool */
    pscPlayBufPool      = AUDIO_PlayerGetPlayBufPoolPtr();

    /* ��ȡ����ģ������� */
    pstPlayer           = AUDIO_PlayerGetPlayPtr();

    /* ��ȡ��ǰ�����ڼ��Input Buff����������� */
    pswDecConsumed      = (VOS_INT32*)&pstPlayer->uwDecConsumed;

    /* ��ȡ������������buffer pool size */
    uwPlayBufPoolDepth  = AUDIO_PlayerGetPlayBufPoolDepth();

    OM_CPUVIEW_EnterArea((VOS_UCHAR)OM_CPUVIEW_AREA_AUDIO_PLAYER_DECODE);

    /* ���buffer pool�е�������С����֡20ms���ݣ���ô��Ҫ����ֱ�������㹻 */
    while((uwPlayBufPoolDepth < (2 * pstPlayer->uw20msDataSize) )
       && (CODEC_STATE_INITIALIZED == uwDecoderState))
    {
        /* �����ǰ״̬Ϊpause�����buffer poolΪȫ0 */
        if (AUDIO_PLAYER_STATE_PAUSE== pstPlayer->enState)
        {
            UCOM_MemSet(pscPlayBufPool, 0, pstPlayer->uw20msDataSize);

            /* ����buffer pool depth */
            uwPlayBufPoolDepth = pstPlayer->uw20msDataSize;
            AUDIO_PlayerSetPlayBufPoolDepth(uwPlayBufPoolDepth);
            break;
        }

        /* ��ǰ���ŵ��ֽڴ�С */
        pstPlayer->uwPlayedBytes = AUDIO_PLAYER_GetCurrDecPos(pstPlayer->enFormat);

        /* ���ݵ�ǰbuffe״̬���Input�����л� */
        AUDIO_PLAYER_FillBuff(pstPlayer);

        /* MP3��AAC�����û�������Ϣ��� */
        UCOM_MemSet(&stDecUserCfg, 0, sizeof(XA_AUDIO_DEC_USER_CFG_STRU));

        if (pstPlayer->uwPlayedBytes >= pstPlayer->uwTotalFileSize)
        {
            enIsEndOfStream = AUDIO_PLAYER_POS_END_OF_STEAM;
        }
        else
        {
            enIsEndOfStream = AUDIO_PLAYER_POS_NORMAL;
        }

        /* ����AUDIO_PlayerGetOutputBufPtr() */
        AUDIO_PlayerGetOutputBufPtr() = pscPlayBufPool + uwPlayBufPoolDepth;

        /* �������MP3 */
        if (AUDIO_FILE_FORMAT_MP3 == pstPlayer->enFormat)
        {
            /* ����MP3��Ϣ */
            stDecUserCfg.enIsEndOfStream   = enIsEndOfStream;
            stDecUserCfg.uwPcmWidthSize    = AUDIO_PLAYER_PCM_WIDTH_SIZE;

            /* ��������BUFF���׵�ַ */
            stDecUserCfg.pvInBuff          = AUDIO_PlayerGetInputBufPtr();

            /* ��������BUFF��Ч��С */
            stDecUserCfg.swInSize          = (VOS_INT32)pstPlayer->uwDecInMemSize;

            /* ����MP3����BUFF */
            stDecUserCfg.pvOutBuff         = AUDIO_PlayerGetOutputBufPtr();

            /* ��������������ڽ�������У�Ӧ���������������:
            stMP3UserCfg.swConsumed;     // �˴ν������ĵ�BUFF��С
            stMP3UserCfg.swOutputSize;   // �����������ݵ��ܴ�С
            stMP3UserCfg.uwDecoderState; // ����״̬ */
            uwErrorCode = MP3_DECODER_Decode(&stDecUserCfg);

            XA_ERROR_HANDLE(uwErrorCode);

            /* ����������BUFF���� */
            *pswDecConsumed                = stDecUserCfg.swConsumed;

            /* ����������������ܴ�С */
            swOutputSize                   = stDecUserCfg.swOutputSize;

            /* �������״̬ */
            uwDecoderState                 = stDecUserCfg.uwDecoderState;

        }

        /* �������AAC */
        else
        {
            /* ����AAC��Ϣ */
            stDecUserCfg.enIsEndOfStream   = enIsEndOfStream;
            stDecUserCfg.uwPcmWidthSize    = AUDIO_PLAYER_PCM_WIDTH_SIZE;

            /* ��������BUFF���׵�ַ */
            stDecUserCfg.pvInBuff          = AUDIO_PlayerGetInputBufPtr();

            /* ��������BUFF��Ч��С */
            stDecUserCfg.swInSize          = (VOS_INT32)pstPlayer->uwDecInMemSize;

            /* ����AAC����BUFF */
            stDecUserCfg.pvOutBuff         = AUDIO_PlayerGetOutputBufPtr();

            /* ��������������ڽ�������У�Ӧ���������������:
            stAacUserCfg.swConsumed;     // �˴ν������ĵ�BUFF��С
            stAacUserCfg.swOutputSize;   // �����������ݵ��ܴ�С
            stAacUserCfg.uwDecoderState; // ����״̬ */
            uwErrorCode = AAC_DECODER_Decode(&stDecUserCfg);
            XA_ERROR_HANDLE(uwErrorCode);

            /* ����������BUFF���� */
            *pswDecConsumed                = stDecUserCfg.swConsumed;

            /* ����������������ܴ�С */
            swOutputSize                   = stDecUserCfg.swOutputSize;

            /* �������״̬ */
            uwDecoderState                 = stDecUserCfg.uwDecoderState;

        }

        /* ����buffer pool depth */
        uwPlayBufPoolDepth += (VOS_UINT32)swOutputSize;

        AUDIO_PlayerSetPlayBufPoolDepth(uwPlayBufPoolDepth);

    }
    *penDecoderState = (CODEC_STATE_ENUM_UINT16)uwDecoderState;

    OM_CPUVIEW_ExitArea((VOS_UCHAR)OM_CPUVIEW_AREA_AUDIO_PLAYER_DECODE);

    return VOS_OK;
}


VOS_UINT32 AUDIO_PLAYER_PcmStart(VOS_VOID)
{
    AUDIO_PCM_OPEN_REQ_STRU             stPcmOpenMsg;
    AUDIO_PCM_HW_PARAMS_REQ_STRU        stPcmHwParaMsg;
    AUDIO_PCM_TRIGGER_REQ_STRU          stPcmTriggerMsg;
    AUDIO_PLAYER_OBJ_STRU              *pstPlayer   = VOS_NULL;

    UCOM_MemSet(&stPcmOpenMsg, 0, sizeof(AUDIO_PCM_OPEN_REQ_STRU));
    UCOM_MemSet(&stPcmHwParaMsg, 0, sizeof(AUDIO_PCM_HW_PARAMS_REQ_STRU));
    UCOM_MemSet(&stPcmTriggerMsg, 0, sizeof(AUDIO_PCM_TRIGGER_REQ_STRU));

    /* �����ϢID_AP_AUDIO_PCM_OPEN_REQ */
    stPcmOpenMsg.uhwMsgId  = ID_AP_AUDIO_PCM_OPEN_REQ;
    stPcmOpenMsg.enPcmMode = AUDIO_PCM_MODE_PLAYBACK;

    /* ģ��AP������ϢID_AP_AUDIO_PCM_OPEN_REQ��PCM */
    UCOM_SendAudioMsg(DSP_PID_AUDIO,
                      DSP_PID_AUDIO_RT,
                      &stPcmOpenMsg,
                      sizeof(AUDIO_PCM_OPEN_REQ_STRU));

    /* ģ��AP������ϢID_AP_AUDIO_PCM_HW_PARA_REQ��PCM */
    stPcmHwParaMsg.uhwMsgId     = ID_AP_AUDIO_PCM_HW_PARA_REQ;
    stPcmHwParaMsg.enPcmMode    = AUDIO_PCM_MODE_PLAYBACK;
    stPcmHwParaMsg.uwChannelNum = AUDIO_PCM_MAX_CHANNEL_NUM;
    stPcmHwParaMsg.uwSampleRate = DRV_SIO_SAMPLING_48K;

    /* ģ��AP������ϢID_AP_AUDIO_PCM_HW_PARA_REQ��PCM */
    UCOM_SendAudioMsg(DSP_PID_AUDIO,
                      DSP_PID_AUDIO_RT,
                      &stPcmHwParaMsg,
                      sizeof(AUDIO_PCM_HW_PARAMS_REQ_STRU));

    pstPlayer   = AUDIO_PlayerGetPlayPtr();

    /* ģ��AP������ϢID_AP_AUDIO_PCM_TRIGGER_REQ��PCM */
    stPcmTriggerMsg.uhwMsgId     = ID_AP_AUDIO_PCM_TRIGGER_REQ;
    stPcmTriggerMsg.enPcmMode    = AUDIO_PCM_MODE_PLAYBACK;
    stPcmTriggerMsg.enCmd        = AUDIO_PCM_TRIGGER_START;
    stPcmTriggerMsg.enPcmObj     = AUDIO_PCM_OBJ_HIFI;
    stPcmTriggerMsg.uwBufAddr    = (VOS_UINT32)AUDIO_PlayerGetPlayBufPoolPtr();
    stPcmTriggerMsg.uwBufSize    = pstPlayer->uw20msDataSize;

    /* ģ��AP������ϢID_AP_AUDIO_PCM_TRIGGER_REQ��PCM */
    UCOM_SendAudioMsg(DSP_PID_AUDIO,
                      DSP_PID_AUDIO_RT,
                      &stPcmTriggerMsg,
                      sizeof(AUDIO_PCM_TRIGGER_REQ_STRU));

    return UCOM_RET_SUCC;
}


VOS_VOID AUDIO_PLAYER_PcmStop(VOS_VOID)
{
    AUDIO_PCM_TRIGGER_REQ_STRU          stPcmTriggerMsg;
    AUDIO_PCM_CLOSE_REQ_STRU            stPcmCloseMsg;

    UCOM_MemSet(&stPcmTriggerMsg, 0, sizeof(AUDIO_PCM_TRIGGER_REQ_STRU));
    UCOM_MemSet(&stPcmCloseMsg, 0, sizeof(AUDIO_PCM_CLOSE_REQ_STRU));

    /* ģ��AP������ϢID_AP_AUDIO_PCM_TRIGGER_REQ��PCM */
    stPcmTriggerMsg.uhwMsgId     = ID_AP_AUDIO_PCM_TRIGGER_REQ;
    stPcmTriggerMsg.enPcmMode    = AUDIO_PCM_MODE_PLAYBACK;
    stPcmTriggerMsg.enCmd        = AUDIO_PCM_TRIGGER_STOP;
    stPcmTriggerMsg.enPcmObj     = AUDIO_PCM_OBJ_HIFI;

    /* ������Ƶר��ͨ�����Ϳ����Ϣ */
    UCOM_SendAudioMsg(DSP_PID_AUDIO,
                    DSP_PID_AUDIO_RT,
                   &stPcmTriggerMsg,
                    sizeof(AUDIO_PCM_TRIGGER_REQ_STRU));

    /* ģ��AP������ϢID_AP_AUDIO_PCM_CLOSE_REQ��PCM */
    stPcmCloseMsg.uhwMsgId     = ID_AP_AUDIO_PCM_CLOSE_REQ;
    stPcmCloseMsg.enPcmMode    = AUDIO_PCM_MODE_PLAYBACK;

    /* ������Ƶר��ͨ�����Ϳ����Ϣ */
    UCOM_SendAudioMsg(DSP_PID_AUDIO,
                    DSP_PID_AUDIO_RT,
                    &stPcmCloseMsg,
                    sizeof(AUDIO_PCM_CLOSE_REQ_STRU));


    return;
}


VOS_VOID AUDIO_PLAYER_SendApAudioCnf(VOS_UINT16    uhwMsgId,
                                                  VOS_UINT32    uwRet,
                                                  VOS_VOID     *pvTail,
                                                  VOS_UINT32    uwTailLen)
{
    APAUDIO_OP_RSLT_STRU    stCnfMsg;

    /* �����Ϣ */
    UCOM_MemSet(&stCnfMsg, 0, sizeof(APAUDIO_OP_RSLT_STRU));

    /* ������Ϣ�ͷ���ֵ */
    stCnfMsg.uhwMsgId         = uhwMsgId;
    stCnfMsg.uhwResult        = (VOS_UINT16)uwRet;

    /*  �ظ���Ϣ��AP */
    UCOM_COMM_SendDspMsg(DSP_PID_AUDIO,
                         ACPU_PID_OM,
                        &stCnfMsg,
                         sizeof(APAUDIO_OP_RSLT_STRU),
                         pvTail,
                         uwTailLen);
}
VOS_UINT32 AUDIO_PLAYER_SendPcmSetBufCmd(VOS_UINT32 uwBufPoolAddr,
                                                        VOS_UINT32 uwBufSize)
{
    AUDIO_PCM_SET_BUF_CMD_STRU          stSetBufMsg;

    UCOM_MemSet(&stSetBufMsg, 0, sizeof(AUDIO_PCM_SET_BUF_CMD_STRU));

    /* ģ��AP������ϢID_AP_AUDIO_PCM_SET_BUF_CMD��PCM */
    stSetBufMsg.uhwMsgId    = ID_AP_AUDIO_PCM_SET_BUF_CMD;
    stSetBufMsg.enPcmMode   = AUDIO_PCM_MODE_PLAYBACK;
    stSetBufMsg.enPcmObj    = AUDIO_PCM_OBJ_HIFI;
    stSetBufMsg.uwBufAddr   = uwBufPoolAddr;
    stSetBufMsg.uwBufSize   = uwBufSize;

    /* ����ͨ��VOS���ͽӿڣ�������Ϣ */
    UCOM_SendAudioMsg(DSP_PID_AUDIO,
                      DSP_PID_AUDIO_RT,
                     &stSetBufMsg,
                      sizeof(AUDIO_PCM_SET_BUF_CMD_STRU));

    /* �˴�����һ֡20ms���������Ѵ�����ɣ�hifi�������ddr��hifi��MCUͶƱ��Ҫ��DDR������ˢ��ģʽ */
    UCOM_LOW_PowerDRF();

    return UCOM_RET_SUCC;
}
VOS_UINT32 AUDIO_PLAYER_SendPlayDoneInd(AUDIO_PLAY_DONE_IND_ENUM_UINT16   enPlayDone)
{
    AUDIO_PLAYER_DONE_IND_STRU         stPlayDoneIndMsg;
    AUDIO_HIFIMISC_MSG_TAIL_STRU       stMsgTail;

    /* ��AUDIO_PLAYER_DONE_IND_STRU��ʽ�����Ϣ */
    stPlayDoneIndMsg.uhwMsgId      = ID_AUDIO_AP_PLAY_DONE_IND;
    stPlayDoneIndMsg.enPlayDone    = enPlayDone;
    stMsgTail.uwCmdId              = AUDIO_MISC_CMD_NOTIFY;
    stMsgTail.uwSn                 = AUDIO_HIFIMISC_MSG_SN_DEFAULT;

    /* ���ÿ����Ƶchannel�ӿ�,֪ͨAP������� */
    UCOM_COMM_SendDspMsg(DSP_PID_AUDIO,
                         ACPU_PID_OM,           /* �˴���дAP��DRV��Ӧ��PID,,���� */
                        &stPlayDoneIndMsg,
                         sizeof(AUDIO_PLAYER_DONE_IND_STRU),
                         &stMsgTail,
                         sizeof(AUDIO_HIFIMISC_MSG_TAIL_STRU));

    return UCOM_RET_SUCC;
}
VOS_VOID AUDIO_PLAYER_SendPlayStopCnf(VOS_VOID *pvTail, VOS_UINT32 uwTailLen)
{
    AUDIO_PLAYER_PAUSE_CNF_STRU         stPlayPauseCnfMsg;
    AUDIO_PLAYER_OBJ_STRU              *pstPlayer    = VOS_NULL;

    pstPlayer                           = AUDIO_PlayerGetPlayPtr();

    /* ��stPlayPauseCnfMsg��ʽ�����Ϣ */
    stPlayPauseCnfMsg.uhwMsgId          = ID_AUDIO_AP_PLAY_PAUSE_CNF;
    stPlayPauseCnfMsg.uhwResult         = UCOM_RET_SUCC;
    stPlayPauseCnfMsg.uwPlayedBytes     = pstPlayer->uwPlayedBytes;

    /* ���ÿ����Ƶchannel�ӿ�,֪ͨAP���Ž��� */
    UCOM_COMM_SendDspMsg(DSP_PID_AUDIO,
                         ACPU_PID_OM,           /* �˴���дAP��DRV��Ӧ��PID,,���� */
                         &stPlayPauseCnfMsg,
                         sizeof(AUDIO_PLAYER_PAUSE_CNF_STRU),
                         pvTail,
                         uwTailLen);

    return;

}
VOS_VOID AUDIO_PLAYER_SendPlaySeekCnf(AUDIO_PLAY_RESULT_CNF_ENUM_UINT16 enSeekResult,
                                                  VOS_VOID               *pvTail,
                                                  VOS_UINT32              uwTailLen)
{
    AUDIO_PLAYER_SEEK_CNF_STRU          stSeekCnfMsg;

    /* ��AUDIO_PLAYER_SEEK_CNF_STRU��ʽ�����Ϣ */
    stSeekCnfMsg.uhwMsgId           = ID_AUDIO_AP_PLAY_SEEK_CNF;
    stSeekCnfMsg.enSeekResult       = enSeekResult;

    /* ���ÿ����Ƶchannel�ӿ�,֪ͨAP seek result */
    UCOM_COMM_SendDspMsg(DSP_PID_AUDIO,
                         ACPU_PID_OM,           /* �˴���дAP��DRV��Ӧ��PID */
                        &stSeekCnfMsg,
                         sizeof(AUDIO_PLAYER_SEEK_CNF_STRU),
                         pvTail,
                         uwTailLen);
    return;

}
VOS_UINT32 AUDIO_PLAYER_DoSrc(
                                VOS_INT16  *pshwBufIn,
                                VOS_INT16  *pshwBufOut,
                                VOS_UINT32  uwProcTime)
{
    VOS_UINT32                          uwCnt;
    VOS_UINT32                          uwRet;
    VOS_UINT32                          uwSrcDolbyPoolDepth = AUDIO_PlayerGetSrcBufPoolDepth();
    VOS_VOID                           *pInputBuff;
    VOS_VOID                           *pOutputBuff;
    VOS_UINT32                          uwOutputSize;

    OM_CPUVIEW_EnterArea((VOS_UCHAR)OM_CPUVIEW_AREA_AUDIO_PLAYER_SRC);

    /* �������buff */
    pOutputBuff = pshwBufOut;

    /* ����SRCһ�δ���128�������㣬�ʲ��÷ֶδ���ÿ�δ���AUDIO_PLAYER_CHUNK_SIZE�������� */
    for (uwCnt = 0; uwCnt < uwProcTime; uwCnt++)
    {
        /* ��������buff */
        pInputBuff = (VOS_VOID *)(pshwBufIn + (uwCnt * (XA_SRC_INPUT_CHUNKSIZE_128 * AUDIO_PLAYER_PLAY_CHN_NUM)));

        /* ִ�б���� */
        uwRet = XA_SRC_ExeProcess(XA_SRC_PlAYBACK,
                                  pInputBuff,
                                  pOutputBuff,
                                  &uwOutputSize);

        /* ִ���쳣������ */
        if(uwRet != VOS_OK)
        {
            OM_LogError1(XA_SRC_ExeProcess_Fail, uwRet);

            return VOS_ERR;
        }

        /* �����������ָ�� */
        pOutputBuff = (VOS_INT16*)pOutputBuff + \
                      (uwOutputSize/(VOS_INT32)sizeof(VOS_INT16));

        /* ����SRC��DOLBY�������� */
        uwSrcDolbyPoolDepth     = uwSrcDolbyPoolDepth + uwOutputSize;
        AUDIO_PlayerSetSrcBufPoolDepth(uwSrcDolbyPoolDepth);

    }

    OM_CPUVIEW_ExitArea((VOS_UCHAR)OM_CPUVIEW_AREA_AUDIO_PLAYER_SRC);

    return VOS_OK;

}
VOS_INT32  AUDIO_PLAYER_DoDts(
                VOS_INT16 *pshwInput,
                VOS_INT16 *pshwOutput)
{
    XA_DTS_USER_CFG_STRU            stDtsCfg;
    VOS_UINT32                      uwCnt;
    VOS_INT32                       swRet = VOS_OK;

    /* ����DTS���� */
    stDtsCfg.swChannelNum       = AUDIO_PLAYER_PLAY_CHN_NUM;
    stDtsCfg.uwBlockSize        = AUDIO_PLAYER_DTS_CHUNK_SIZE;
    stDtsCfg.uwFilterCfg        = 0;
    stDtsCfg.uwSampleRate       = AUDIO_PLAYER_OUTPUT_SAMPLE_RATE;

    OM_CPUVIEW_EnterArea((VOS_UCHAR)OM_CPUVIEW_AREA_AUDIO_PLAYER_DTS);

    /* ������DMA���˵�������DTS���� */
    for ( uwCnt = 0; uwCnt < AUDIO_PLAYER_DTS_PROC_NUM; uwCnt++ )
    {
        /* ������������ */
        stDtsCfg.pshwInputBuff  = pshwInput + ((stDtsCfg.uwBlockSize * (VOS_UINT32)stDtsCfg.swChannelNum) * uwCnt);

        /* ����������� */
        stDtsCfg.pshwOutputBuff = pshwOutput + ((stDtsCfg.uwBlockSize * (VOS_UINT32)stDtsCfg.swChannelNum) * uwCnt);

        /* ִ��DTS��Ч���� */
        swRet += XA_DTS_ExeProcess(&stDtsCfg);
    }

    OM_CPUVIEW_ExitArea((VOS_UCHAR)OM_CPUVIEW_AREA_AUDIO_PLAYER_DTS);

    return swRet;

}
VOS_UINT32  AUDIO_PLAYER_ShiftBuff(
                AUDIO_PLAYER_BUFF_ENUM_UINT16 enBuffType,
                VOS_UINT32                    uwShiftSize)
{
    VOS_UINT32                  uwDecBuffSize;
    VOS_UINT32                  uwSrcBuffSize;

    /* �ƶ�����֮��Ļ���buff */
    if (AUDIO_PLAYER_DEC_BUFF == enBuffType)
    {
        /* ��ȡ���µĽ������� */
        uwDecBuffSize  = AUDIO_PlayerGetPlayBufPoolDepth();

        if (uwDecBuffSize >= uwShiftSize)
        {
            /* ����dec����� */
            XA_COMM_ShiftBuff(AUDIO_PlayerGetPlayBufPoolPtr(),
                              (VOS_INT32)uwDecBuffSize,
                              (VOS_INT32)uwShiftSize);

            uwDecBuffSize = uwDecBuffSize - uwShiftSize;
        }
        else
        {
            uwDecBuffSize = 0;

            OM_LogWarning2(AUDIO_PlayerDecBuffSizeWarning, uwDecBuffSize, uwShiftSize);
        }

        /* ����dec�������� */
        AUDIO_PlayerSetPlayBufPoolDepth(uwDecBuffSize);

    }
    /* �ƶ������֮��Ļ���buff */
    else if(AUDIO_PLAYER_SRC_BUFF == enBuffType)
    {
        /* ��ȡ���µ�SRC����ص���� */
        uwSrcBuffSize = AUDIO_PlayerGetSrcBufPoolDepth();

        /* ����src����� */
        if (uwSrcBuffSize >= uwShiftSize)
        {
             XA_COMM_ShiftBuff(AUDIO_PlayerGetSrcBuffPtr(),             \
                      (VOS_INT32)(uwSrcBuffSize),       \
                      (VOS_INT32)uwShiftSize);

             uwSrcBuffSize = uwSrcBuffSize - uwShiftSize;
        }
        else
        {
            uwSrcBuffSize = 0;

            OM_LogWarning2(AUDIO_PlayerSrcBuffSizeWarning, uwSrcBuffSize, uwShiftSize);
        }

        /* ����src�������� */
        AUDIO_PlayerSetSrcBufPoolDepth(uwSrcBuffSize);

    }
    else
    {
        /* Ŀǰ����buff�ƶ�ֻ�漰dec��src */
        OM_LogError1(AUDIO_PlayerShiftBuffError, enBuffType);

        return VOS_ERR;
    }

    return VOS_OK;

}
VOS_VOID AUDIO_PLAYER_AudioCallBack(
                VOS_VOID  *pshwBufOut,
                VOS_VOID  *pshwBufIn,
                VOS_UINT32 uwSampleRate,
                VOS_UINT32 uwChannelNum,
                VOS_UINT32 uwBufSize)
{
    AUDIO_ENHANCE_ProcSpkOut(
                pshwBufOut,
                pshwBufIn,
                uwSampleRate,
                uwChannelNum,
                uwBufSize);
}


VOS_UINT32  AUDIO_PLAYER_MsgSetDtsEnableCmd(VOS_VOID *pvOsaMsg)
{
    AUDIO_PLAYER_OBJ_STRU                   *pstPlayer          = AUDIO_PlayerGetPlayPtr();
    AUDIO_EFFECT_DTS_SET_ENABLE_REQ_STRU    *pstSetParaCmd;
    AUDIO_PLAYER_DTS_SET_ENABLE_CMD_STRU     stSetDtsEnable;

    /* �쳣��μ�� */
    if ( VOS_NULL == pvOsaMsg )
    {
        OM_LogError(AUDIO_PlayerSetDtsEnableFail);
        return VOS_ERR;
    }

    /* ����DTS��Ч�����ṹ����� */
    pstSetParaCmd   = (AUDIO_EFFECT_DTS_SET_ENABLE_REQ_STRU *)((MsgBlock *) pvOsaMsg)->aucValue;

    /* ���ñ�־λ */
    pstPlayer->enDtsEnable               = pstSetParaCmd->uhwEnableFlag;

    /* ���������״̬��������������� */
    if (AUDIO_PLAYER_STATE_RUNNING == AUDIO_PlayerGetState())
    {
        pstPlayer->stVolume.enSetVolEnum        = AUDIO_PLAYER_DTS_ENABLE_VOL;

        pstPlayer->stVolume.uwDtsEnableFrame    = AUDIO_PLAYER_MORPHING_VOL_FRAME_0;

        /* Ϊ�����������ʹ��DTS�����������쳣��ÿ�ε���ǰ���������õ�ǰ���� */
        pstPlayer->stVolume.uwCurr              = pstPlayer->stVolume.uwTarget;
    }
    /* �����������״̬����ֱ�ӷ���Ϣ��DTS */
    else
    {
        UCOM_MemSet(&stSetDtsEnable, 0, sizeof(AUDIO_PLAYER_DTS_SET_ENABLE_CMD_STRU));

        stSetDtsEnable.uhwMsgId         = ID_AUDIO_PLAYER_SET_DTS_ENABLE_IND;
        stSetDtsEnable.uhwEnableFlag    = pstPlayer->enDtsEnable;

        /* ����ͨ��VOS���ͽӿڣ�������Ϣ */
        UCOM_SendOsaMsg(DSP_PID_AUDIO_RT,
                        DSP_PID_AUDIO_RT,
                       &stSetDtsEnable,
                        sizeof(AUDIO_PLAYER_DTS_SET_ENABLE_CMD_STRU));
    }

    return VOS_OK;

}
VOS_UINT32  AUDIO_PLAYER_DtsMorphingVol(VOS_VOID)
{
    AUDIO_PLAYER_OBJ_STRU                   *pstPlayer          = AUDIO_PlayerGetPlayPtr();
    AUDIO_PLAYER_DTS_SET_ENABLE_CMD_STRU     stSetDtsEnable;
    VOS_UINT32                               uwTmpVol;

    /* ���浱ǰ����ֵ������tmp����ֵ */
    uwTmpVol        = 0;

    /* ��1�������½����̡�0 - 10 ֡ */
    if ( pstPlayer->stVolume.uwDtsEnableFrame < AUDIO_PLAYER_MORPHING_VOL_FRAME_10 )
    {
        /* �����𼶱䶯, 5֡������λ */
        pstPlayer->stVolume.uwCurr -= (pstPlayer->stVolume.uwTarget - uwTmpVol)/AUDIO_PLAYER_MORPHING_VOL_FRAME_10;

        /* ��¼�Ѵ���֡�� */
        pstPlayer->stVolume.uwDtsEnableFrame++;

        /* �������ڳ����������� */
        if (10 == pstPlayer->stVolume.uwDtsEnableFrame)
        {
            pstPlayer->stVolume.uwCurr = 0;
        }
    }
    /* ��2���������̡���10֡ */
    else if ( pstPlayer->stVolume.uwDtsEnableFrame < AUDIO_PLAYER_MORPHING_VOL_FRAME_11 )
    {
        /* �ڵ�5֡���þ��� */
        pstPlayer->stVolume.uwCurr = 0;

        /* ��¼�Ѵ���֡�� */
        pstPlayer->stVolume.uwDtsEnableFrame++;

    }
    /* ��3�������������̡�11 - 20 ֡ */
    else if ( pstPlayer->stVolume.uwDtsEnableFrame < AUDIO_PLAYER_MORPHING_VOL_FRAME_21 )
    {
        /* �����𼶱䶯, 5֡������λ */
        pstPlayer->stVolume.uwCurr += (pstPlayer->stVolume.uwTarget - uwTmpVol)/AUDIO_PLAYER_MORPHING_VOL_FRAME_10;

        /* �ڵ�11֡��DTSģ��ת������Ϣ */
        if (11 == pstPlayer->stVolume.uwDtsEnableFrame)
        {
            UCOM_MemSet(&stSetDtsEnable, 0, sizeof(AUDIO_PLAYER_DTS_SET_ENABLE_CMD_STRU));

            stSetDtsEnable.uhwMsgId      = ID_AUDIO_PLAYER_SET_DTS_ENABLE_IND;
            stSetDtsEnable.uhwEnableFlag = pstPlayer->enDtsEnable;

            /* ����ͨ��VOS���ͽӿڣ�������Ϣ */
            UCOM_SendOsaMsg(DSP_PID_AUDIO_RT,
                            DSP_PID_AUDIO_RT,
                           &stSetDtsEnable,
                            sizeof(AUDIO_PLAYER_DTS_SET_ENABLE_CMD_STRU));
        }

        /* ��¼�Ѵ���֡�� */
        pstPlayer->stVolume.uwDtsEnableFrame++;

    }
    /* ��4������������� */
    else
    {
        /* �ָ��Ѵ���֡�� */
        pstPlayer->stVolume.uwDtsEnableFrame = AUDIO_PLAYER_MORPHING_VOL_FRAME_0;

        /* �ָ������������� */
        pstPlayer->stVolume.enSetVolEnum = AUDIO_PLAYER_NORMAL_VOL;

        /* �������ڳ�����������ָ�֮ǰ�������������� */
        pstPlayer->stVolume.uwOriginal  = pstPlayer->stVolume.uwTarget;
        pstPlayer->stVolume.uwCurr      = pstPlayer->stVolume.uwTarget;
    }

    return VOS_OK;

}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif




