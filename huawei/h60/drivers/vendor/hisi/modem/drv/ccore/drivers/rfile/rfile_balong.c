

#include "rfile_balong.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

extern int BSP_mass_read(const s8 *partition, u32 offset, u32 size, void* buf);
extern int BSP_mass_write(const s8 *partition, u32 offset, u32 size, void* buf);
extern void rfile_mntnshow(void);


struct bsp_rfile_main_stru g_stRfileMain = {BSP_FALSE, };

typedef void (*RFILE_CNF_FUN)(struct bsp_rfile_que_stru *pstRfileQue, void *pstCnf);

struct bsp_rfile_table_stru
{
    u32                             enRfileType;
    void                            *pFun;
};

/* ����A�˵�Ӧ�������� */
struct bsp_rfile_table_stru astRfileCnf[] = {
    {EN_RFILE_OP_OPEN,              rfile_CloseCnf},   /* ��close��Ӧ����һ�� */
    {EN_RFILE_OP_CLOSE,             rfile_CloseCnf},
    {EN_RFILE_OP_WRITE,             rfile_CloseCnf},   /* ��close��Ӧ����һ�� */
    {EN_RFILE_OP_READ,              rfile_ReadCnf},
    {EN_RFILE_OP_SEEK,              rfile_CloseCnf},   /* ��close��Ӧ����һ�� */
    {EN_RFILE_OP_TELL,              rfile_CloseCnf},   /* ��close��Ӧ����һ�� */
    {EN_RFILE_OP_REMOVE,            rfile_CloseCnf},   /* ��close��Ӧ����һ�� */
    {EN_RFILE_OP_MKDIR,             rfile_CloseCnf},   /* ��close��Ӧ����һ�� */
    {EN_RFILE_OP_RMDIR,             rfile_CloseCnf},   /* ��close��Ӧ����һ�� */
    {EN_RFILE_OP_OPENDIR,           rfile_OpendirCnf},
    {EN_RFILE_OP_READDIR,           rfile_ReadCnf},    /* ��read��Ӧ����һ�� */
    {EN_RFILE_OP_CLOSEDIR,          rfile_CloseCnf},   /* ��close��Ӧ����һ�� */
    {EN_RFILE_OP_STAT,              rfile_StatCnf},
    {EN_RFILE_OP_ACCESS,            rfile_CloseCnf},   /* ��close��Ӧ����һ�� */
    {EN_RFILE_OP_MASSRD,            rfile_ReadCnf},    /* ��read��Ӧ����һ�� */
    {EN_RFILE_OP_MASSWR,            rfile_CloseCnf},   /* ��close��Ӧ����һ�� */
    {EN_RFILE_OP_RENAME,            rfile_CloseCnf},   /* ��close��Ӧ����һ�� */
};


struct bsp_rfile_mntn_stru g_stRfileMntn[EN_RFILE_OP_BUTT+1];

struct rfile_mntn_stru g_stRfMntn;


/*lint -save -e516 -e713 -e732 -e737 -e958*/

s32 bsp_open(const s8 *path, s32 flags,  s32 mode)
{
    s32 ret;
    struct bsp_rfile_que_stru stRfileQue = {EN_RFILE_IDLE, };
    u32 ulNameLen, ulSize;
    struct bsp_rfile_open_req *pstReq;
    struct rfile_mntn_info_stru stmntn = {EN_RFILE_OP_OPEN, };

    rfile_MntnTypeIn(EN_RFILE_OP_OPEN);

    stmntn.enType = EN_RFILE_OP_OPEN;

    if(!path)
    {
        stmntn.enErr = EN_RFILE_ERR_PARA_INVALID;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> path null.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    stRfileQue.stData.optype = EN_RFILE_OP_OPEN;
    stRfileQue.ret = BSP_ERROR;

    stmntn.param3 = (strlen((char*)path) >= RFILE_MNTN_DATA_LEN) ? (RFILE_MNTN_DATA_LEN-1) : strlen((char*)path);
    memcpy((void*)stmntn.pData, path, stmntn.param3);

    ulNameLen = strlen((char*)path) + 1;

    ulSize = sizeof(struct bsp_rfile_open_req) + ulNameLen;
    
    stmntn.param1 = ulSize;

    if(ulSize > RFILE_LEN_MAX)
    {
        stmntn.enErr = EN_RFILE_ERR_LEN;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "![rfile]: <%s> ulSize %d > RFILE_LEN_MAX.\n", __FUNCTION__, ulSize);
        return BSP_ERROR;
    }

    pstReq = Rfile_Malloc(ulSize);
    if(!pstReq)
    {
        stmntn.enErr = EN_RFILE_ERR_MALLOC; /* [false alarm]:fortify */
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> Rfile_Malloc failed.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    pstReq->opType = EN_RFILE_OP_OPEN;
    pstReq->pstlist = &stRfileQue.stlist;
    pstReq->nameLen = ulNameLen;
    pstReq->mode = mode;
    pstReq->flags = flags;

    memcpy((void*)pstReq->aucData, path, (s32)ulNameLen);

    stRfileQue.pData = pstReq;
    stRfileQue.ulLen = ulSize;

    ret = rfile_HandleReq(&stRfileQue);
    if(ret)
    {
        stmntn.enErr = EN_RFILE_ERR_TIMEOUT;
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE,
            "[rfile]: <%s> rfile_HandleReq failed fd %d.\n", __FUNCTION__, stRfileQue.ret);
    }

    Rfile_Free(pstReq);

    stmntn.param2 = stRfileQue.ret;
    rfile_MntnRecord(&stmntn);

    return stRfileQue.ret;
}
s32 bsp_close(u32 fd)
{
    s32 ret;
    struct bsp_rfile_que_stru stRfileQue = {EN_RFILE_IDLE, };
    struct bsp_rfile_close_req stReq;
    struct rfile_mntn_info_stru stmntn = {EN_RFILE_OP_OPEN, };

    rfile_MntnTypeIn(EN_RFILE_OP_CLOSE);

    stmntn.enType = EN_RFILE_OP_CLOSE;
    stmntn.param1 = fd;

    stRfileQue.stData.optype = EN_RFILE_OP_CLOSE;
    stRfileQue.ret = BSP_ERROR;

    stReq.opType = EN_RFILE_OP_CLOSE;
    stReq.pstlist = &stRfileQue.stlist;
    stReq.fd = fd;

    stRfileQue.pData = &stReq;
    stRfileQue.ulLen = sizeof(struct bsp_rfile_close_req);

    ret = rfile_HandleReq(&stRfileQue);
    if(ret)
    {
        stmntn.enErr = EN_RFILE_ERR_TIMEOUT;
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, 
            "[rfile]: <%s> rfile_HandleReq failed , stRfileQue.ret %d.\n", __FUNCTION__, stRfileQue.ret);
    }

    rfile_MntnRecord(&stmntn);

    return stRfileQue.ret;
}


static s32 rfile_write(u32 fd ,void *ptr, u32 size)
{
    s32 ret;
    struct bsp_rfile_que_stru stRfileQue = {EN_RFILE_IDLE, };
    struct bsp_rfile_write_req *pstReq;
    u32 ulLen;

    stRfileQue.stData.optype = EN_RFILE_OP_WRITE;
    stRfileQue.ret = BSP_ERROR;

    ulLen = sizeof(struct bsp_rfile_write_req) + size;

    pstReq = Rfile_Malloc(ulLen);
    if(!pstReq)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> Rfile_Malloc failed.\n", __FUNCTION__); /* [false alarm]:fortify */
        return BSP_ERROR;
    }

    pstReq->opType = EN_RFILE_OP_WRITE;
    pstReq->pstlist = &stRfileQue.stlist;
    pstReq->fd = fd;
    pstReq->ulSize = size;
    memcpy((void*)pstReq->aucData, ptr, (s32)size);

    stRfileQue.pData = pstReq;
    stRfileQue.ulLen = ulLen;

    ret = rfile_HandleReq(&stRfileQue);
    if(ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, 
            "[rfile]: <%s> rfile_HandleReq failed, stRfileQue.ret %d.\n", __FUNCTION__, stRfileQue.ret);
    }

    Rfile_Free(pstReq);

    return stRfileQue.ret;
}
s32 bsp_write(u32 fd, const s8 *ptr, u32 size)
{
    s32 ret = -1;
    u32 ulCur, ulLen;
    struct rfile_mntn_info_stru stmntn = {EN_RFILE_OP_OPEN, };

    rfile_MntnTypeIn(EN_RFILE_OP_WRITE);

    stmntn.enType = EN_RFILE_OP_WRITE;
    stmntn.param1 = fd;
    stmntn.param2 = size;

    if(!ptr)
    {
        stmntn.enErr = EN_RFILE_ERR_PARA_INVALID;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> ptr null.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    /* ������ȳ���ICC��󳤶����ƣ���Ҫ��ִ��� */
    ulCur = 0;
    while(ulCur < size)
    {
        ulLen = ((size - ulCur) > RFILE_WR_LEN_MAX) ? RFILE_WR_LEN_MAX : (size - ulCur);

        ret = rfile_write(fd, ((u8*)ptr+ulCur), ulLen);

        ulCur += ret;

        if(ret != (s32)ulLen)
        {
            break;
        }
    }
    
    stmntn.param3 = ulCur;
    
    rfile_MntnRecord(&stmntn);

    return ulCur;
}
static s32 rfile_read(u32 fd, s8 *ptr, u32 size)
{
    s32 ret;
    struct bsp_rfile_que_stru stRfileQue = {EN_RFILE_IDLE, };
    struct bsp_rfile_read_req stReq;

    stRfileQue.stData.optype = EN_RFILE_OP_READ;
    stRfileQue.stData.ptr    = ptr;
    stRfileQue.stData.datalen = size;
    stRfileQue.stData.retlen = BSP_ERROR;

    stReq.opType = EN_RFILE_OP_READ;
    stReq.pstlist = &stRfileQue.stlist;
    stReq.fd = fd;
    stReq.ulSize = size;

    stRfileQue.pData = &stReq;
    stRfileQue.ulLen = sizeof(struct bsp_rfile_read_req);

    ret = rfile_HandleReq(&stRfileQue);
    if(ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "![rfile]: <%s> rfile_HandleReq failed.\n", __FUNCTION__);
    }

    return stRfileQue.stData.retlen;
}



s32 bsp_read(u32 fd ,s8 *ptr, u32 size)
{
    s32 ret = -1;
    u32 ulCur, ulLen;
    struct rfile_mntn_info_stru stmntn = {EN_RFILE_OP_OPEN, };

    rfile_MntnTypeIn(EN_RFILE_OP_READ);

    stmntn.enType = EN_RFILE_OP_READ;
    stmntn.param1 = fd;
    stmntn.param2 = size;

    if(!ptr)
    {
        stmntn.enErr = EN_RFILE_ERR_PARA_INVALID;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> ptr null.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    /* ������ȳ���ICC��󳤶����ƣ���Ҫ��ִ��� */
    ulCur = 0;
    while(ulCur < size)
    {
        ulLen = ((size - ulCur) > RFILE_RD_LEN_MAX) ? RFILE_RD_LEN_MAX : (size - ulCur);

        ret = rfile_read(fd, (s8*)(ptr+ulCur), ulLen);

        ulCur += ret;

        if(ret != (s32)ulLen)
        {
            break;
        }
    }

    stmntn.param3 = ulCur;
    
    rfile_MntnRecord(&stmntn);

    return ulCur;
}
s32 bsp_lseek(u32 fd, long offset, s32 whence)
{
    s32 ret;
    struct bsp_rfile_que_stru stRfileQue = {EN_RFILE_IDLE, };
    struct bsp_rfile_seek_req stReq;
    struct rfile_mntn_info_stru stmntn = {EN_RFILE_OP_OPEN, };

    rfile_MntnTypeIn(EN_RFILE_OP_SEEK);

    stmntn.enType = EN_RFILE_OP_SEEK;
    stmntn.param1 = fd;
    stmntn.param2 = offset;
    stmntn.param3 = whence;

    stRfileQue.stData.optype = EN_RFILE_OP_SEEK;
    stRfileQue.ret = BSP_ERROR;

    stReq.opType = EN_RFILE_OP_SEEK;
    stReq.pstlist = &stRfileQue.stlist;
    stReq.fd = fd;
    stReq.offset = offset;
    stReq.whence = whence;

    stRfileQue.pData = &stReq;
    stRfileQue.ulLen = sizeof(struct bsp_rfile_seek_req);

    ret = rfile_HandleReq(&stRfileQue);
    if(ret)
    {
        stmntn.enErr = EN_RFILE_ERR_TIMEOUT;

        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, 
            "![rfile]: <%s> rfile_HandleReq failed, stRfileQue.ret %d.\n", __FUNCTION__, stRfileQue.ret);
    }

    rfile_MntnRecord(&stmntn);

    return stRfileQue.ret;
}


long bsp_tell(u32 fd)
{
    s32 ret;
    struct bsp_rfile_que_stru stRfileQue = {EN_RFILE_IDLE, };
    struct bsp_rfile_tell_req stReq;
    struct rfile_mntn_info_stru stmntn = {EN_RFILE_OP_OPEN, };

    rfile_MntnTypeIn(EN_RFILE_OP_TELL);

    stmntn.enType = EN_RFILE_OP_TELL;
    stmntn.param1 = fd;

    stRfileQue.stData.optype = EN_RFILE_OP_TELL;
    stRfileQue.ret = BSP_ERROR;

    stReq.opType = EN_RFILE_OP_TELL;
    stReq.pstlist = &stRfileQue.stlist;
    stReq.fd = fd;

    stRfileQue.pData = &stReq;
    stRfileQue.ulLen = sizeof(struct bsp_rfile_tell_req);

    ret = rfile_HandleReq(&stRfileQue);
    if(ret)
    {
        stmntn.enErr = EN_RFILE_ERR_TIMEOUT;

        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, 
            "![rfile]: <%s> rfile_HandleReq failed, stRfileQue.ret %d.\n", __FUNCTION__, stRfileQue.ret);
    }

    stmntn.param2 = stRfileQue.ret;
    rfile_MntnRecord(&stmntn);

    return stRfileQue.ret;
}


s32 bsp_remove(const s8 *pathname)
{
    s32 ret;
    u32 ulLen;
    struct bsp_rfile_que_stru stRfileQue = {EN_RFILE_IDLE, };
    struct bsp_rfile_remove_req *pstReq;
    struct rfile_mntn_info_stru stmntn = {EN_RFILE_OP_OPEN, };

    rfile_MntnTypeIn(EN_RFILE_OP_REMOVE);

    stmntn.enType = EN_RFILE_OP_REMOVE;

    if(!pathname)
    {
        stmntn.enErr = EN_RFILE_ERR_PARA_INVALID;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> pathname null.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    stRfileQue.stData.optype = EN_RFILE_OP_REMOVE;
    stRfileQue.ret = BSP_ERROR;

    ulLen = sizeof(struct bsp_rfile_remove_req) + strlen((char*)pathname) + 1;
    
    stmntn.param1 = ulLen;

    stmntn.param3 = (strlen((char*)pathname) >= RFILE_MNTN_DATA_LEN) ? (RFILE_MNTN_DATA_LEN-1) : strlen((char*)pathname);
    memcpy((void*)stmntn.pData, pathname, stmntn.param3);

    if(ulLen > RFILE_LEN_MAX)
    {
        stmntn.enErr = EN_RFILE_ERR_LEN;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "![rfile]: <%s> ulLen %d > RFILE_LEN_MAX.\n", __FUNCTION__, ulLen);
        return BSP_ERROR;
    }

    pstReq = Rfile_Malloc(ulLen);
    if(!pstReq)
    {
        stmntn.enErr = EN_RFILE_ERR_MALLOC; /* [false alarm]:fortify */
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> Rfile_Malloc failed.\n", __FUNCTION__);
        return stRfileQue.ret;
    }

    pstReq->opType = EN_RFILE_OP_REMOVE;
    pstReq->pstlist = &stRfileQue.stlist;
    pstReq->pathLen = strlen((char*)pathname) + 1;

    memcpy((void*)pstReq->aucData, pathname, pstReq->pathLen);

    stRfileQue.pData = pstReq;
    stRfileQue.ulLen = ulLen;

    ret = rfile_HandleReq(&stRfileQue);
    if(ret)
    {
        stmntn.enErr = EN_RFILE_ERR_TIMEOUT;
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "![rfile]: <%s> rfile_HandleReq failed.\n", __FUNCTION__);
    }

    Rfile_Free(pstReq);
    
    stmntn.param2 = stRfileQue.ret;
    rfile_MntnRecord(&stmntn);

    return stRfileQue.ret;
}
s32 bsp_mkdir(s8 *dirName, s32 mode)
{
    s32 ret;
    u32 ulLen;
    struct bsp_rfile_que_stru stRfileQue = {EN_RFILE_IDLE, };
    struct bsp_rfile_mkdir_req *pstReq;
    struct rfile_mntn_info_stru stmntn = {EN_RFILE_OP_OPEN, };

    rfile_MntnTypeIn(EN_RFILE_OP_MKDIR);

    stmntn.enType = EN_RFILE_OP_MKDIR;

    if(!dirName)
    {
        stmntn.enErr = EN_RFILE_ERR_PARA_INVALID;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> dirName null.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    stRfileQue.stData.optype = EN_RFILE_OP_MKDIR;
    stRfileQue.ret = BSP_ERROR;

    ulLen = sizeof(struct bsp_rfile_mkdir_req) + strlen((char*)dirName) + 1;

    stmntn.param3 = (strlen((char*)dirName) >= RFILE_MNTN_DATA_LEN) ? (RFILE_MNTN_DATA_LEN-1) : strlen((char*)dirName);
    memcpy((void*)stmntn.pData, dirName, stmntn.param3);

    stmntn.param1 = ulLen;

    if(ulLen > RFILE_LEN_MAX)
    {
        stmntn.enErr = EN_RFILE_ERR_LEN;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "![rfile]: <%s> ulLen %d > RFILE_LEN_MAX.\n", __FUNCTION__, ulLen);
        return BSP_ERROR;
    }

    pstReq = Rfile_Malloc(ulLen);
    if(!pstReq)
    {
        stmntn.enErr = EN_RFILE_ERR_MALLOC; /* [false alarm]:fortify */
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> Rfile_Malloc failed.\n", __FUNCTION__);
        return stRfileQue.ret;
    }

    pstReq->opType = EN_RFILE_OP_MKDIR;
    pstReq->pstlist = &stRfileQue.stlist;
    pstReq->pathLen = strlen((char*)dirName) + 1;
    pstReq->mode = mode;

    memcpy((void*)pstReq->aucData, dirName, pstReq->pathLen);

    stRfileQue.pData = pstReq;
    stRfileQue.ulLen = ulLen;

    ret = rfile_HandleReq(&stRfileQue);
    if(ret)
    {
        stmntn.enErr = EN_RFILE_ERR_TIMEOUT;
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, 
            "![rfile]: <%s> rfile_HandleReq failed, stRfileQue.ret %d.\n", __FUNCTION__, stRfileQue.ret);
    }

    Rfile_Free(pstReq);

    stmntn.param2 = stRfileQue.ret;

    rfile_MntnRecord(&stmntn);

    return stRfileQue.ret;
}
s32 bsp_rmdir(s8 *path)
{
    s32 ret;
    u32 ulLen;
    struct bsp_rfile_que_stru stRfileQue = {EN_RFILE_IDLE, };
    struct bsp_rfile_rmdir_req *pstReq;
    struct rfile_mntn_info_stru stmntn = {EN_RFILE_OP_OPEN, };

    rfile_MntnTypeIn(EN_RFILE_OP_RMDIR);

    stmntn.enType = EN_RFILE_OP_RMDIR;

    if(!path)
    {
        stmntn.enErr = EN_RFILE_ERR_PARA_INVALID;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> path null.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    stRfileQue.stData.optype = EN_RFILE_OP_RMDIR;
    stRfileQue.ret = BSP_ERROR;

    ulLen = sizeof(struct bsp_rfile_rmdir_req) + strlen((char*)path) + 1;

    stmntn.param1 = ulLen;

    stmntn.param3 = (strlen((char*)path) >= RFILE_MNTN_DATA_LEN) ? (RFILE_MNTN_DATA_LEN-1) : strlen((char*)path);
    memcpy((void*)stmntn.pData, path, stmntn.param3);

    if(ulLen > RFILE_LEN_MAX)
    {
        stmntn.enErr = EN_RFILE_ERR_LEN;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "![rfile]: <%s> ulLen %d > RFILE_LEN_MAX.\n", __FUNCTION__, ulLen);
        return BSP_ERROR;
    }

    pstReq = Rfile_Malloc(ulLen);
    if(!pstReq)
    {
        stmntn.enErr = EN_RFILE_ERR_MALLOC; /* [false alarm]:fortify */
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> Rfile_Malloc failed.\n", __FUNCTION__);
        return stRfileQue.ret;
    }

    pstReq->opType = EN_RFILE_OP_RMDIR;
    pstReq->pstlist = &stRfileQue.stlist;
    pstReq->pathLen = strlen((char*)path) + 1;

    memcpy((void*)pstReq->aucData, path, pstReq->pathLen);

    stRfileQue.pData = pstReq;
    stRfileQue.ulLen = ulLen;

    ret = rfile_HandleReq(&stRfileQue);
    if(ret)
    {
        stmntn.enErr = EN_RFILE_ERR_TIMEOUT;
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, 
            "![rfile]: <%s> rfile_HandleReq failed, stRfileQue.ret %d.\n", __FUNCTION__, stRfileQue.ret);
    }

    Rfile_Free(pstReq);
    
    stmntn.param2 = stRfileQue.ret;
    rfile_MntnRecord(&stmntn);

    return stRfileQue.ret;
}
s32 bsp_opendir(s8 *dirName)
{
    s32 ret;
    u32 ulLen;
    struct bsp_rfile_que_stru stRfileQue = {EN_RFILE_IDLE, };
    struct bsp_rfile_opendir_req *pstReq;
    struct rfile_mntn_info_stru stmntn = {EN_RFILE_OP_OPEN, };

    rfile_MntnTypeIn(EN_RFILE_OP_OPENDIR);

    stmntn.enType = EN_RFILE_OP_OPENDIR;

    if(!dirName)
    {
        stmntn.enErr = EN_RFILE_ERR_PARA_INVALID;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> dirName null.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    stRfileQue.stData.optype = EN_RFILE_OP_OPENDIR;
    stRfileQue.stData.dirhandle = BSP_ERROR;

    ulLen = sizeof(struct bsp_rfile_opendir_req) + strlen((char*)dirName) + 1;

    stmntn.param1 = ulLen;

    stmntn.param3 = (strlen((char*)dirName) >= RFILE_MNTN_DATA_LEN) ? (RFILE_MNTN_DATA_LEN-1) : strlen((char*)dirName);
    memcpy((void*)stmntn.pData, dirName, stmntn.param3);

    if(ulLen > RFILE_LEN_MAX)
    {
        stmntn.enErr = EN_RFILE_ERR_LEN;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "![rfile]: <%s> ulLen %d > RFILE_LEN_MAX.\n", __FUNCTION__, ulLen);
        return BSP_ERROR;
    }

    pstReq = Rfile_Malloc(ulLen);
    if(!pstReq)
    {
        stmntn.enErr = EN_RFILE_ERR_MALLOC; /* [false alarm]:fortify */
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> Rfile_Malloc failed.\n", __FUNCTION__);
        return stRfileQue.stData.dirhandle;
    }

    pstReq->opType = EN_RFILE_OP_OPENDIR;
    pstReq->pstlist = &stRfileQue.stlist;
    pstReq->nameLen = strlen((char*)dirName) + 1;

    memcpy((void*)pstReq->aucData, dirName, pstReq->nameLen);

    stRfileQue.pData = pstReq;
    stRfileQue.ulLen = ulLen;

    ret = rfile_HandleReq(&stRfileQue);
    if(ret)
    {
        stmntn.enErr = EN_RFILE_ERR_TIMEOUT;
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, 
            "![rfile]: <%s> rfile_HandleReq failed, stRfileQue.stData.dirhandle %d.\n",
            __FUNCTION__, stRfileQue.stData.dirhandle);
    }

    Rfile_Free(pstReq);

    stmntn.param2 = stRfileQue.stData.dirhandle;
    rfile_MntnRecord(&stmntn);

    return stRfileQue.stData.dirhandle;
}


s32 rfile_readdir(u32 fd, void *dirent, u32 count)
{
    s32 ret;
    struct bsp_rfile_que_stru stRfileQue = {EN_RFILE_IDLE, };
    struct bsp_rfile_readdir_req stReq;

    stRfileQue.stData.optype = EN_RFILE_OP_READDIR;
    stRfileQue.stData.ptr = dirent;
    stRfileQue.stData.datalen = count;
    stRfileQue.stData.retlen = -1;

    stReq.opType = EN_RFILE_OP_READDIR;
    stReq.pstlist = &stRfileQue.stlist;
    stReq.dir = fd;
    stReq.count = count;

    stRfileQue.pData = &stReq;
    stRfileQue.ulLen = sizeof(struct bsp_rfile_readdir_req);

    ret = rfile_HandleReq(&stRfileQue);
    if(ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, 
            "![rfile]: <%s> rfile_HandleReq failed, stRfileQue.stData.retlen %d.\n",
            __FUNCTION__, stRfileQue.stData.retlen);
    }

    return stRfileQue.stData.retlen;
}
s32 bsp_readdir(u32 fd, void *dirent, u32 count)
{
    s32 ret = -1;
    u32 ulCur, ulLen, ulMaxLen;
    struct rfile_mntn_info_stru stmntn = {EN_RFILE_OP_OPEN, };

    rfile_MntnTypeIn(EN_RFILE_OP_READDIR);

    stmntn.enType = EN_RFILE_OP_READDIR;
    stmntn.param1 = fd;
    stmntn.param2 = count;

    if(!dirent)
    {
        stmntn.enErr = EN_RFILE_ERR_PARA_INVALID;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> dirent null.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    /* ÿ���ܶ�ȡ����󳤶� */
    ulMaxLen = RFILE_RD_LEN_MAX;

    /* ������ȳ���ICC��󳤶����ƣ���Ҫ��ִ��� */
    ulCur = 0;
    while(ulCur < count)
    {
        ulLen = ((count - ulCur) > ulMaxLen) ? ulMaxLen : (count - ulCur);

        ret = rfile_readdir(fd, ((u8*)dirent+ulCur), ulLen);

        ulCur += ret;

        if(ret != (s32)ulLen)
        {
            break;
        }

    }

    stmntn.param2 = ulCur;
    
    rfile_MntnRecord(&stmntn);

    return ulCur;
}



s32 bsp_closedir(s32 pDir)
{
    s32 ret;
    struct bsp_rfile_que_stru stRfileQue = {EN_RFILE_IDLE, };
    struct bsp_rfile_closedir_req stReq;
    struct rfile_mntn_info_stru stmntn = {EN_RFILE_OP_OPEN, };

    rfile_MntnTypeIn(EN_RFILE_OP_CLOSEDIR);

    stmntn.enType = EN_RFILE_OP_CLOSEDIR;
    stmntn.param1 = pDir;

    stRfileQue.stData.optype = EN_RFILE_OP_CLOSEDIR;
    stRfileQue.ret = BSP_ERROR;

    stReq.opType = EN_RFILE_OP_CLOSEDIR;
    stReq.pstlist = &stRfileQue.stlist;
    stReq.dir = pDir;

    stRfileQue.pData = &stReq;
    stRfileQue.ulLen = sizeof(struct bsp_rfile_closedir_req);

    ret = rfile_HandleReq(&stRfileQue);
    if(ret)
    {
        stmntn.enErr = EN_RFILE_ERR_TIMEOUT;
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, 
            "![rfile]: <%s> rfile_HandleReq failed, stRfileQue.ret %d.\n", __FUNCTION__, stRfileQue.ret);
    }

    stmntn.param2 = stRfileQue.ret;
    rfile_MntnRecord(&stmntn);

    return stRfileQue.ret;
}


s32 bsp_stat(s8 *name, void *pStat)
{
    s32 ret;
    u32 ulLen;
    struct bsp_rfile_que_stru stRfileQue = {EN_RFILE_IDLE, };
    struct bsp_rfile_stat_req *pstReq;
    struct rfile_mntn_info_stru stmntn = {EN_RFILE_OP_OPEN, };

    rfile_MntnTypeIn(EN_RFILE_OP_STAT);

    stmntn.enType = EN_RFILE_OP_STAT;

    if((!name) || (!pStat))
    {
        stmntn.enErr = EN_RFILE_ERR_PARA_INVALID;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> name pStat null.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    stRfileQue.stData.optype = EN_RFILE_OP_STAT;
    stRfileQue.ret = -1;

    ulLen = sizeof(struct bsp_rfile_stat_req) + strlen((char*)name) + 1;

    pstReq = Rfile_Malloc(ulLen);
    if(!pstReq)
    {
        stmntn.enErr = EN_RFILE_ERR_MALLOC; /* [false alarm]:fortify */
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> Rfile_Malloc failed.\n", __FUNCTION__);
        return stRfileQue.ret;
    }

    pstReq->opType = EN_RFILE_OP_STAT;
    pstReq->pstlist = &stRfileQue.stlist;
    pstReq->ulSize = strlen((char*)name) + 1;

    memcpy((void*)pstReq->aucData, name, (s32)pstReq->ulSize);

    stRfileQue.pData = pstReq;
    stRfileQue.ulLen = ulLen;

    ret = rfile_HandleReq(&stRfileQue);
    if(ret)
    {
        stmntn.enErr = EN_RFILE_ERR_TIMEOUT;
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> rfile_HandleReq failed.\n", __FUNCTION__);
    }

    Rfile_Free(pstReq);

    if(BSP_OK == stRfileQue.ret)
    {
        memcpy(pStat, (void*)&stRfileQue.stData.ststat, sizeof(struct rfile_stat_stru));
    }

    stmntn.param1 = stRfileQue.ret;
    rfile_MntnRecord(&stmntn);

    return stRfileQue.ret;
}
s32 bsp_access(s8 *path, s32 mode)
{
    s32 ret;
    struct bsp_rfile_que_stru stRfileQue = {EN_RFILE_IDLE, };
    u32 ulNameLen, ulSize;
    struct bsp_rfile_access_req *pstReq;
    struct rfile_mntn_info_stru stmntn = {EN_RFILE_OP_OPEN, };

    rfile_MntnTypeIn(EN_RFILE_OP_ACCESS);

    stmntn.enType = EN_RFILE_OP_ACCESS;

    if(!path)
    {
        stmntn.enErr = EN_RFILE_ERR_PARA_INVALID;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> path null.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    stRfileQue.stData.optype = EN_RFILE_OP_ACCESS;
    stRfileQue.ret = BSP_ERROR;

    stmntn.param3 = (strlen((char*)path) >= RFILE_MNTN_DATA_LEN) ? (RFILE_MNTN_DATA_LEN-1) : strlen((char*)path);
    memcpy((void*)stmntn.pData, (void*)path, stmntn.param3);

    ulNameLen = strlen((char*)path) + 1;

    ulSize = sizeof(struct bsp_rfile_access_req) + ulNameLen;
    
    stmntn.param1 = ulSize;

    if(ulSize > RFILE_LEN_MAX)
    {
        stmntn.enErr = EN_RFILE_ERR_LEN;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "![rfile]: <%s> ulSize %d > RFILE_LEN_MAX.\n", __FUNCTION__, ulSize);
        return BSP_ERROR;
    }

    pstReq = Rfile_Malloc(ulSize);
    if(!pstReq)
    {
        stmntn.enErr = EN_RFILE_ERR_MALLOC; /* [false alarm]:fortify */
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> Rfile_Malloc failed.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    pstReq->opType = EN_RFILE_OP_ACCESS;
    pstReq->pstlist = &stRfileQue.stlist;
    pstReq->pathlen = ulNameLen;
    pstReq->mode = mode;

    memcpy(pstReq->aucData, path, ulNameLen);

    stRfileQue.pData = pstReq;
    stRfileQue.ulLen = ulSize;

    ret = rfile_HandleReq(&stRfileQue);
    if(ret)
    {
        stmntn.enErr = EN_RFILE_ERR_TIMEOUT;
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE,
            "[rfile]: <%s> rfile_HandleReq failed fd %d.\n", __FUNCTION__, stRfileQue.ret);
    }

    Rfile_Free(pstReq);

    stmntn.param2 = stRfileQue.ret;
    rfile_MntnRecord(&stmntn);

    return stRfileQue.ret;
}
s32 bsp_rename( const char * oldname, const char * newname )
{
    s32 ret;
    u32 ulLen;
    struct bsp_rfile_que_stru stRfileQue = {EN_RFILE_IDLE, };
    struct bsp_rfile_rename_req *pstReq;
    struct rfile_mntn_info_stru stmntn = {EN_RFILE_OP_RENAME, };

    rfile_MntnTypeIn(EN_RFILE_OP_RENAME);

    stmntn.enType = EN_RFILE_OP_RENAME;

    if((!oldname) || (!newname))
    {
        stmntn.enErr = EN_RFILE_ERR_PARA_INVALID;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> name pStat null.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    stRfileQue.stData.optype = EN_RFILE_OP_RENAME;
    stRfileQue.ret = -1;

    ulLen = sizeof(struct bsp_rfile_rename_req) + strlen((char*)oldname) + strlen((char*)newname) + 2;

    pstReq = Rfile_Malloc(ulLen);
    if(!pstReq)
    {
        stmntn.enErr = EN_RFILE_ERR_MALLOC; /* [false alarm]:fortify */
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> Rfile_Malloc failed.\n", __FUNCTION__);
        return stRfileQue.ret;
    }

    stRfileQue.pData = pstReq;
    stRfileQue.ulLen = ulLen;

    pstReq->opType = EN_RFILE_OP_RENAME;
    pstReq->pstlist = &stRfileQue.stlist;
    pstReq->ulSize = strlen((char*)oldname) + strlen((char*)newname) + 2;

    memset((void*)pstReq->aucData, 0, pstReq->ulSize);
    
    ulLen = strlen((char*)oldname) + 1;
    memcpy((void*)pstReq->aucData, oldname, (s32)ulLen);
    
    memcpy((void*)(pstReq->aucData + ulLen), newname, (s32)strlen((char*)newname));

    ret = rfile_HandleReq(&stRfileQue);
    if(ret)
    {
        stmntn.enErr = EN_RFILE_ERR_TIMEOUT;
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> rfile_HandleReq failed.\n", __FUNCTION__);
    }

    Rfile_Free(pstReq);

    stmntn.param1 = stRfileQue.ret;
    rfile_MntnRecord(&stmntn);

    return stRfileQue.ret;
}
static s32 rfile_massread(const s8 *partition, u32 offset, u32 size, void* buf)
{
    u32 len;
    s32 ret;
    struct bsp_rfile_que_stru stRfileQue = {EN_RFILE_IDLE, };
    struct bsp_rfile_massread_req *pstReq;

    stRfileQue.stData.optype = EN_RFILE_OP_MASSRD;
    stRfileQue.stData.ptr    = buf;
    stRfileQue.stData.datalen = size;
    stRfileQue.stData.retlen = BSP_ERROR;

    len = strlen((char*)partition)+1;
    
    pstReq = Rfile_Malloc(sizeof(struct bsp_rfile_massread_req)+len);
    if(!pstReq)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> malloc failed.\n", __FUNCTION__); /* [false alarm]:fortify */
        return -1;
    }
    
    pstReq->opType = EN_RFILE_OP_MASSRD;
    pstReq->pstlist = &stRfileQue.stlist;
    pstReq->offset = offset;
    pstReq->size = size;
    pstReq->datalen = len;
    memcpy(pstReq->aucData, partition, len);

    stRfileQue.pData = pstReq;
    stRfileQue.ulLen = sizeof(struct bsp_rfile_massread_req) + len;

    ret = rfile_HandleReq(&stRfileQue);
    if(ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "![rfile]: <%s> rfile_HandleReq failed.\n", __FUNCTION__);
    }

    Rfile_Free(pstReq);
    
    return stRfileQue.stData.retlen;
}
int BSP_mass_read(const s8 *partition, u32 offset, u32 size, void* buf)
{
    s32 ret = -1;
    u32 ulCur, ulLen, ulMaxLen;
    struct rfile_mntn_info_stru stmntn = {EN_RFILE_OP_OPEN, };

    rfile_MntnTypeIn(EN_RFILE_OP_MASSRD);

    stmntn.enType = EN_RFILE_OP_MASSRD;
    stmntn.param1 = offset;
    stmntn.param2 = size;

    if((!partition) || (!buf))
    {
        stmntn.enErr = EN_RFILE_ERR_PARA_INVALID;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> ptr null.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    /* ÿ���ܶ�ȡ����󳤶� */
    ulMaxLen = RFILE_RD_LEN_MAX;

    /* ������ȳ���ICC��󳤶����ƣ���Ҫ��ִ��� */
    ulCur = 0;
    while(ulCur < size)
    {
        ulLen = ((size - ulCur) > ulMaxLen) ? ulMaxLen : (size - ulCur);

        ret = rfile_massread(partition, offset, ulLen, ((u8*)buf+ulCur));

        ulCur += ret;

        if((u32)ret != ulLen)
        {
            break;
        }
    }

    stmntn.param3 = ulCur;
    
    rfile_MntnRecord(&stmntn);

    if(ulCur != size)
    {
        return -1;
    }

    return 0;
}



static s32 rfile_masswrite(const s8 *partition, u32 offset, u32 size, void* buf)
{
    s32 ret;
    struct bsp_rfile_que_stru stRfileQue = {EN_RFILE_IDLE, };
    struct bsp_rfile_masswrite_req *pstReq;
    u32 ulLen;

    stRfileQue.stData.optype = EN_RFILE_OP_MASSWR;
    stRfileQue.ret = BSP_ERROR;

    ulLen = sizeof(struct bsp_rfile_masswrite_req) + size + (strlen((char*)partition) + 1);

    pstReq = Rfile_Malloc(ulLen);
    if(!pstReq)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> Rfile_Malloc failed.\n", __FUNCTION__); /* [false alarm]:fortify */
        return BSP_ERROR;
    }

    pstReq->opType = EN_RFILE_OP_MASSWR;
    pstReq->pstlist = &stRfileQue.stlist;
    pstReq->size = size;
    pstReq->offset = offset;
    pstReq->partitionlen = strlen((char*)partition) + 1;
    pstReq->datalen = pstReq->partitionlen + size;
    
    memcpy(pstReq->aucData, partition, pstReq->partitionlen);
    memcpy((pstReq->aucData + pstReq->partitionlen), buf, size);

    stRfileQue.pData = pstReq;
    stRfileQue.ulLen = ulLen;

    ret = rfile_HandleReq(&stRfileQue);
    if(ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, 
            "[rfile]: <%s> rfile_HandleReq failed, stRfileQue.ret %d.\n", __FUNCTION__, stRfileQue.ret);
    }

    Rfile_Free(pstReq);

    return stRfileQue.ret;
}
int BSP_mass_write(const s8 *partition, u32 offset, u32 size, void* buf)
{
    s32 ret = -1;
    u32 ulCur, ulLen, ulMaxLen;
    struct rfile_mntn_info_stru stmntn = {EN_RFILE_OP_OPEN, };

    rfile_MntnTypeIn(EN_RFILE_OP_MASSWR);

    stmntn.enType = EN_RFILE_OP_MASSWR;
    stmntn.param1 = offset;
    stmntn.param2 = size;

    if((!partition) || (!buf))
    {
        stmntn.enErr = EN_RFILE_ERR_PARA_INVALID;
        rfile_MntnRecord(&stmntn);
        
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> ptr null.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    ulMaxLen = RFILE_LEN_MAX - sizeof(struct bsp_rfile_masswrite_req) - (strlen((char*)partition)+1);

    /* ������ȳ���ICC��󳤶����ƣ���Ҫ��ִ��� */
    ulCur = 0;
    while(ulCur < size)
    {
        ulLen = ((size - ulCur) > ulMaxLen) ? ulMaxLen : (size - ulCur);

        ret = rfile_masswrite(partition, offset, ulLen, ((u8*)buf+ulCur));

        ulCur += ret;

        if((u32)ret != ulLen)
        {
            break;
        }
    }
    
    stmntn.param3 = ulCur;
    
    rfile_MntnRecord(&stmntn);

    if(ulCur != size)
    {
        return -1;
    }

    return 0;
}
s32 rfile_CnfProc(struct list_head *plistcnf, u32 enOptype, void *pData)
{
    s32 ret;
    RFILE_CNF_FUN   pCnfFun;
    struct bsp_rfile_que_stru *pstRfileQue;
    struct list_head *me = BSP_NULL;

    /* ��������ĵط��ȴ���ʱ��ִ��ɾ���ڵ�������˵����ź������� */
    /* coverity[example_assign] */
    ret = osl_sem_downtimeout(&g_stRfileMain.semList, RFILE_TIMEOUT_MAX);
    if(BSP_OK != ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> semTake failed.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    /* �������в����Ƿ�����Ӧ���Ӧ������ڵ� */
    list_for_each(me, &g_stRfileMain.stlist)
    {
        if(me == plistcnf)
        {
            break;
        }
    }

    /* ���ڳ�ʱ������ڵ���ɾ��ʱ���ݴ��� */
    if(me != plistcnf)
    {
        osl_sem_up(&g_stRfileMain.semList);

        bsp_trace(BSP_LOG_LEVEL_CRIT, BSP_MODU_RFILE, "[rfile]: <%s> can't find list node.\n", __FUNCTION__);

        return BSP_ERROR;
    }

    pstRfileQue = list_entry(me, struct bsp_rfile_que_stru, stlist);

    if((EN_RFILE_DOING != pstRfileQue->enState) || enOptype != pstRfileQue->stData.optype)
    {
        osl_sem_up(&g_stRfileMain.semList);

        bsp_trace(BSP_LOG_LEVEL_CRIT, BSP_MODU_RFILE, "[rfile]: <%s> State: %d, cnf type: %d, req type: %d.\n",
            __FUNCTION__, pstRfileQue->enState, enOptype, pstRfileQue->stData.optype);

        return BSP_ERROR;
    }

    pCnfFun = astRfileCnf[enOptype].pFun;

    pCnfFun(pstRfileQue, pData);

    osl_sem_up(&pstRfileQue->semReq);

    osl_sem_up(&g_stRfileMain.semList);

    return BSP_OK;
}


s32 bsp_RfileCallback(u32 channel_id, u32 len, void *context)
{
    bsp_trace(BSP_LOG_LEVEL_DEBUG, BSP_MODU_RFILE, "[rfile]: <%s> entry.\n", __FUNCTION__);

    if(RFILE_CCORE_ICC_RD_CHAN != channel_id)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "![rfile]: <%s> channel_id %d error.\n", __FUNCTION__, channel_id);
        
        return BSP_ERROR;
    }

    osl_sem_up(&g_stRfileMain.semTask);

    return BSP_OK;
}



struct bsp_rfile_que_stru *rfile_GetQueNode(void)
{
    s32 ret;
    struct bsp_rfile_que_stru * pstRfile;
    struct list_head *me = NULL;

    bsp_trace(BSP_LOG_LEVEL_DEBUG, BSP_MODU_RFILE, "[rfile]: <%s> entry.\n", __FUNCTION__);

    ret = osl_sem_downtimeout(&g_stRfileMain.semList, RFILE_TIMEOUT_MAX);
    if(BSP_OK != ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> semTake failed.\n", __FUNCTION__);
        return BSP_NULL;
    }

    list_for_each(me, &g_stRfileMain.stlist)
    {
        pstRfile = list_entry(me, struct bsp_rfile_que_stru , stlist);

        if(EN_RFILE_IDLE == pstRfile->enState)
        {
            pstRfile->enState = EN_RFILE_DOING;

            osl_sem_up(&g_stRfileMain.semList);
            return pstRfile;
        }
    }

    osl_sem_up(&g_stRfileMain.semList);

    return BSP_NULL;
}



s32 rfile_RcvIccData(void)
{
    u32 enOptype;
    s32 ret;
    struct list_head *plistcnf = NULL;
    
    ret = bsp_icc_read(RFILE_CCORE_ICC_RD_CHAN, g_stRfileMain.data, RFILE_LEN_MAX);

    if(0 == ret)
    {
        return BSP_ERROR;
    }
    
    if(((u32)ret > RFILE_LEN_MAX) || (ret < 0))
    {
        bsp_trace(BSP_LOG_LEVEL_DEBUG, BSP_MODU_RFILE,
            "[rfile]: <%s> icc_read failed %d.\n", __FUNCTION__, ret);

        return BSP_ERROR;
    }
    
#ifdef RFILE_TEST_NEWICC
    bsp_trace(BSP_LOG_LEVEL_DEBUG, BSP_MODU_RFILE, "[rfile]: <%s> bsp_icc_read %d.\n", __FUNCTION__, ret);
#endif

    /* Ӧ��ĵ�һ�����ֽڶ�Ӧ���� op type */
    enOptype = *(u32*)(g_stRfileMain.data);

    if(enOptype >= EN_RFILE_OP_BUTT)
    {
        bsp_trace(BSP_LOG_LEVEL_CRIT, BSP_MODU_RFILE,
            "[rfile]: <%s> enOptype %d.\n", __FUNCTION__, enOptype);

        return BSP_ERROR;
    }

    /* Ӧ��ĵڶ������ֽڶ�Ӧ���� ����ڵ�ָ�� */
    plistcnf= (struct list_head *)(*((u32*)(g_stRfileMain.data) + 1));

    ret = rfile_CnfProc(plistcnf, enOptype, (g_stRfileMain.data));
    if(ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE,
            "[rfile]: <%s> rfile_CnfProc failed %d.\n", __FUNCTION__, ret);
    }

    return BSP_OK;
}



void rfile_SendIccData(void)
{
    s32 ret, i;
    struct bsp_rfile_que_stru * pstRfile;
#ifdef RFILE_TEST_NEWICC
    u32 times = 0;

    bsp_trace(BSP_LOG_LEVEL_DEBUG, BSP_MODU_RFILE, "[rfile]: <%s> entry.\n", __FUNCTION__);

#endif

    /* ��pstRfile��ֵ�����ж�pstRfile��Ϊ0 */
    while(BSP_NULL != (pstRfile = rfile_GetQueNode()))
    {
        if(pstRfile->stData.optype >= EN_RFILE_OP_BUTT)
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE,
                "[rfile]: <%s> optype %d.\n", __FUNCTION__, pstRfile->stData.optype);

            break;
        }

        for(i = 0; i < RFILE_MAX_SEND_TIMES; i++) /* [false alarm]:fortify */
        {
            ret = bsp_icc_send(ICC_CPU_APP, RFILE_CCORE_ICC_WR_CHAN, pstRfile->pData, pstRfile->ulLen);
            if((u32)ICC_INVALID_NO_FIFO_SPACE == (u32)ret)
            {
                /* buffer������ʱ���ط� */
                RFILE_SLEEP(50); /* [false alarm]:fortify */
#ifdef RFILE_TEST_NEWICC
                times++;
#endif
                continue;
            }
            else if(pstRfile->ulLen != (u32)ret)
            {
                bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> icc_send failed.\n", __FUNCTION__);
#ifdef RFILE_TEST_NEWICC
                if(times)
                {
                    bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> send failed %d times.\n", __FUNCTION__, times);
                }
#endif
                return;
            }
            else
            {
#ifdef RFILE_TEST_NEWICC
                if(times)
                {
                    bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> send failed %d times.\n", __FUNCTION__, times);
                }
#endif
                return;
            }
        }

        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> failed RFILE_MAX_SEND_TIMES.\n", __FUNCTION__); /* [false alarm]:fortify */

        /* ��������ʹ����󣬴�������ʧ�� */
        osl_sem_up(&pstRfile->semReq);
    }
}


void rfile_TaskProc(void* obj)
{
    s32 ret;

    bsp_trace(BSP_LOG_LEVEL_DEBUG, BSP_MODU_RFILE, "[rfile]: <%s> entry.\n", __FUNCTION__);

    while(1) /*lint !e716*/
    {
        /* coverity[check_return] */
        osl_sem_downtimeout(&g_stRfileMain.semTask, RFILE_WAIT_FOREVER);

        ret = rfile_RcvIccData();
        if(BSP_OK == ret)
        {
            /* ����ICCͨ�����л������ݣ��ٴδ�����ȡ */
            osl_sem_up(&g_stRfileMain.semTask);
            continue;
        }

        rfile_SendIccData();
    }
}
s32 bsp_rfile_init(void)
{
    s32 ret;

    osl_sem_init(0, &(g_stRfileMain.semTask));

    osl_sem_init(1, &(g_stRfileMain.semList));

    if((BSP_NULL == g_stRfileMain.semTask) || (BSP_NULL == g_stRfileMain.semList))
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> semBCreate failed.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    INIT_LIST_HEAD(&g_stRfileMain.stlist);

    osl_task_init("rfile", 16, RFILE_TASK_STACK_SIZE, (void *)rfile_TaskProc, NULL, &g_stRfileMain.taskid);

    adp_rfile_init();

    /* mntn infomation initial */
    memset((void*)g_stRfileMntn, 0, sizeof(struct bsp_rfile_mntn_stru)*(EN_RFILE_OP_BUTT+1));

    memset((void*)&g_stRfMntn, 0, sizeof(g_stRfMntn));

    g_stRfileMain.bInitFlag = BSP_TRUE;

    ret = bsp_icc_event_register(RFILE_CCORE_ICC_RD_CHAN, bsp_RfileCallback, NULL, NULL, NULL); /* [false alarm]:fortify */
    if(ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> bsp_icc_event_register failed.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> success.\n", __FUNCTION__); /* [false alarm]:fortify */

    return BSP_OK;
}
s32 rfile_AddNode(struct bsp_rfile_que_stru *pstNode)
{
    s32 ret;

    ret = osl_sem_downtimeout(&g_stRfileMain.semList, RFILE_TIMEOUT_MAX);
    if(BSP_OK != ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> semTake failed.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    /* �������������ź��� */
    osl_sem_init(0, &(pstNode->semReq ));
    if(BSP_NULL == pstNode->semReq)
    {
        osl_sem_up(&g_stRfileMain.semList);
        return BSP_ERROR;
    }

    pstNode->ret = BSP_ERROR;
    pstNode->enState = EN_RFILE_IDLE;

    list_add(&pstNode->stlist, &g_stRfileMain.stlist);

    osl_sem_up(&g_stRfileMain.semList);

    return BSP_OK;
}
s32 rfile_DelNode(struct bsp_rfile_que_stru *pstNode)
{
    s32 ret;

    ret = osl_sem_downtimeout(&g_stRfileMain.semList, RFILE_TIMEOUT_MAX);
    if(BSP_OK != ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> semTake failed.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    /* ɾ�����������ź��� */
    ret = osl_sema_delete(&pstNode->semReq);
    if(ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> osl_sema_delete failed.\n", __FUNCTION__);
    }

    list_del(&pstNode->stlist);

    pstNode->stlist.next = 0;
    pstNode->stlist.prev = 0;

    osl_sem_up(&g_stRfileMain.semList);

    return BSP_OK;
}
s32 rfile_HandleReq(struct bsp_rfile_que_stru *pstRfileQue)
{
    s32 ret;

    ret = rfile_AddNode(pstRfileQue);
    if(BSP_OK != ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> rfile_AddNode failed.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    /* ֪ͨ������ýڵ� */
    osl_sem_up(&g_stRfileMain.semTask);

    /* �ȴ�A�˵�Ӧ�� */
    ret = osl_sem_downtimeout(&pstRfileQue->semReq, RFILE_TIMEOUT_MAX);
    if(BSP_OK != ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> semTake failed.\n", __FUNCTION__);
    }

    /* ��Ҫͬʱ�ж���һ�ε�ret�ķ���ֵ */
    ret |= rfile_DelNode(pstRfileQue);
    if(BSP_OK != ret)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_RFILE, "[rfile]: <%s> rfile_DelNode failed.\n", __FUNCTION__);
        return BSP_ERROR;
    }

    return BSP_OK;
}


/*****************************************************************************
 �� �� ��  : rfile_CloseCnf
 ��������  : fclose��Ӧ����
 �������  : struct bsp_rfile_que_stru *pstRfileQue
             struct bsp_rfile_open_cnf *pstCnf
*****************************************************************************/
void rfile_CloseCnf(struct bsp_rfile_que_stru *pstRfileQue, struct bsp_rfile_common_cnf *pstCnf)
{
    pstRfileQue->ret = pstCnf->ret;

    g_stRfileMain.errorno = pstCnf->errorno;

    return ;
}


/*****************************************************************************
 �� �� ��  : rfile_ReadCnf
 ��������  : fread��Ӧ����
 �������  : struct bsp_rfile_que_stru *pstRfileQue
             struct bsp_rfile_open_cnf *pstCnf
*****************************************************************************/
void rfile_ReadCnf(struct bsp_rfile_que_stru *pstRfileQue, struct bsp_rfile_read_cnf *pstCnf)
{
    pstRfileQue->stData.retlen = pstCnf->Size;

    /* Ӧ�����ݵĳ��ȴ�����С�ڵ�������ĳ��ȣ��һ�����ָ�벻Ϊ�� */
    if((pstCnf->Size > 0) && (pstRfileQue->stData.ptr) && (pstCnf->Size <= (s32)pstRfileQue->stData.datalen))
    {
        memcpy(pstRfileQue->stData.ptr, pstCnf->aucData, pstCnf->Size); /* [false alarm]:fortify */
    }

    g_stRfileMain.errorno = pstCnf->errorno;

    return ;
}
/*lint -restore*/


/*****************************************************************************
 �� �� ��  : rfile_OpendirCnf
 ��������  : fread��Ӧ����
 �������  : struct bsp_rfile_que_stru *pstRfileQue
             struct bsp_rfile_opendir_cnf *pstCnf
*****************************************************************************/
void rfile_OpendirCnf(struct bsp_rfile_que_stru *pstRfileQue, struct bsp_rfile_opendir_cnf *pstCnf)
{
    pstRfileQue->stData.dirhandle = pstCnf->dirhandle;

    g_stRfileMain.errorno = pstCnf->errorno;

    return ;
}


/*****************************************************************************
 �� �� ��  : rfile_StatCnf
 ��������  : fread��Ӧ����
 �������  : struct bsp_rfile_que_stru *pstRfileQue
             struct bsp_rfile_stat_cnf *pstCnf
*****************************************************************************/
void rfile_StatCnf(struct bsp_rfile_que_stru *pstRfileQue, struct bsp_rfile_stat_cnf *pstCnf)
{
    pstRfileQue->ret = pstCnf->ret;
    if(BSP_OK == pstRfileQue->ret)
    {
        memcpy(&pstRfileQue->stData.ststat, &pstCnf->ststat, sizeof(struct rfile_stat_stru));
    }

    g_stRfileMain.errorno = pstCnf->errorno;

    return ;
}


void rfile_MntnTypeIn(enum _BSP_RFILE_OP_EN entype)
{
    g_stRfMntn.sttype.enType[g_stRfMntn.sttype.ptr] = entype;
    g_stRfMntn.sttype.slice[g_stRfMntn.sttype.ptr]  = BSP_GetSliceValue();
    g_stRfMntn.sttype.ptr = (g_stRfMntn.sttype.ptr+1)%RFILE_MNTN_INFO_NUM;
}


void rfile_MntnRecord(struct rfile_mntn_info_stru *pstmntn)
{
    s32 pos;
    s32 type;

    type = (pstmntn->enType > EN_RFILE_OP_BUTT) ? EN_RFILE_OP_BUTT : pstmntn->enType;
    pos = g_stRfileMntn[type].position;
    
    memcpy(&(g_stRfileMntn[type].stMntnInfo[pos]), pstmntn, sizeof(struct rfile_mntn_info_stru));

    g_stRfileMntn[type].stMntnInfo[pos].slice = BSP_GetSliceValue();
    
    g_stRfileMntn[type].position = (g_stRfileMntn[type].position+1)%RFILE_MNTN_INFO_NUM;
}


void rfile_mntnshow(void)
{
    s32 i,j,k,err;
    char *errorinfo[EN_RFILE_ERR_BUTT+1] = {"success",
                                            "invalid parameter",
                                            "malloc failed",
                                            "timeout",
                                            "length error",
                                            "icc process failed",
                                            "other failure"
                                            };
    
    char *typeinfo[EN_RFILE_OP_BUTT] = {"open",
                                        "close",
                                        "write",
                                        "read",
                                        "seek",
                                        "tell",
                                        "remove",
                                        "mkdir",
                                        "rmdir",
                                        "opendir",
                                        "readdir",
                                        "closedir",
                                        "stat",
                                        "access",
                                        "massrd",
                                        "masswr",
                                        "rename"
                                        };
    
    for(i = 0; i < EN_RFILE_OP_BUTT; i++)
    {
        printf("============%s operation infomation==============:\n", typeinfo[i]);
        for(k = 0; k < RFILE_MNTN_INFO_NUM; k++)
        {
            j = (g_stRfileMntn[i].position+k)%RFILE_MNTN_INFO_NUM;
            if(0 == g_stRfileMntn[i].stMntnInfo[j].slice)
            {
                continue;
            }
            err = g_stRfileMntn[i].stMntnInfo[j].enErr;
            printf("slice : 0x%08x, error msg : %s.\n", g_stRfileMntn[i].stMntnInfo[j].slice, errorinfo[err]);
            printf("param1: %d, param2 : %d, param3 : %d.\n", g_stRfileMntn[i].stMntnInfo[j].param1, 
                g_stRfileMntn[i].stMntnInfo[j].param2, g_stRfileMntn[i].stMntnInfo[j].param3);
            printf("pdata : %s.\n\n", g_stRfileMntn[i].stMntnInfo[j].pData);
        }
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


