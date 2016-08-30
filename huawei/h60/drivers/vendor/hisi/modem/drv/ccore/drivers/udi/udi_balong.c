#include <vxWorks.h>
#include <logLib.h>
#include <usrLib.h>
#include <string.h>
#include <stdio.h>
#include <bsp_om.h>
#include "udi_balong.h"


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define UDI_MAX_MAIN_DEV_NUM UDI_DEV_MAX
#define UDI_MAX_DEV_TYPE_NUM 32
#define UDI_MAX_OPEN_NODE_NUM 64
//#define UDI_MAX_INIT_FUNC_NUM UDI_TYPE_MAX
#define UDI_OPEN_NODE_HEADER  0x5A0000

/* Mutex ��Ϊ��װ */
#define UDI_MUTEX_T SEM_ID

#define UDI_MTX_CREATE(mtx) \
do {\
    mtx = semMCreate(SEM_Q_FIFO | SEM_DELETE_SAFE);\
}while((0))

#define UDI_MTX_DELETE(mtx) \
do {\
	semDelete(mtx); \
	mtx = NULL;\
}while((0))
#define UDI_MTX_ENTER(mtx) (void)semTake(mtx, WAIT_FOREVER)
#define UDI_MTX_LEAVE(mtx) (void)semGive(mtx)

#define UDI_DEBUG

/**************************************************************************
  ���Ͷ���
**************************************************************************/
/* �豸�򿪽ڵ� */
typedef struct tagUDI_OPEN_NODE
{
    BSP_BOOL bOpen;
    VOID* pPrivate;
    UDI_DRV_INTEFACE_TABLE *pstDrvTable;
}UDI_OPEN_NODE;

/* �豸ʵ������ */
typedef struct tagUDI_DEV_INSTANCE
{
    /*BSP_U32 u32MagicNum;*/                    /* ����� */
    /*BSP_U32 u32DevId;    */                   /* �豸ID */
    BSP_U32 u32Capability;                  /* �豸���� */
    /*BSP_U8* strDevName; */                    /* �豸��,��������ϵͳ��׼�豸 */
    UDI_DRV_INTEFACE_TABLE *pDrvInterface;  /* �ӿڻص��б� */
    BSP_VOID* pPrivate;                     /* ÿ������˽��ȫ�� */
}UDI_DEV_INSTANCE;


/**************************************************************************
  ȫ�ֱ���
**************************************************************************/
UDI_OPEN_NODE    g_openNodeTable[UDI_MAX_OPEN_NODE_NUM] = {{0}};
BSP_U32 g_openNodeCurPos = 0;
UDI_DEV_INSTANCE g_deviceTable[UDI_MAX_MAIN_DEV_NUM][UDI_MAX_DEV_TYPE_NUM] = {{{0}}};
extern UDI_ADP_INIT_CB_T  g_udiInitFuncTable[UDI_DEV_MAX+1];
UDI_MUTEX_T g_udiMtxOpen;

/**************************************************************************
  ��ʵ��
**************************************************************************/
#define UDI_OFFSET_OF(type, member) ((BSP_U32) (&((type *)0)->member))
#define UDI_IDX_TO_HANDLE(idx) ((UDI_HANDLE)(UDI_OPEN_NODE_HEADER | (idx)))
#define UDI_HANDLE_TO_IDX(hdl) ((BSP_U32)((hdl) & 0xFFFF))


#define UDI_PARSE_DEV_ID(devId, mainId, devType) \
do{\
    mainId = UDI_GET_MAIN_DEV_ID(devId);\
    devType = UDI_GET_DEV_TYPE(devId);\
    \
    BSP_ASSERT(mainId < UDI_MAX_MAIN_DEV_NUM);\
    BSP_ASSERT(devType < UDI_MAX_DEV_TYPE_NUM);\
}while(0)

#define UDI_CALL_OPT_FUNC_SAFELY(handle, param1, param2, functionCB, ret) \
do{\
    BSP_U32 u32Idx;\
    UDI_DRV_INTEFACE_TABLE *pstDrvTable;\
    VOID* pPrivate;\
    \
    UDI_CHECK_HANDLE(handle);\
    u32Idx = UDI_HANDLE_TO_IDX(handle);\
    \
    if (u32Idx >= UDI_MAX_OPEN_NODE_NUM)\
    {\
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "invalid_handle:0x%x, line:%d\n", \
                  (BSP_U32)handle, __LINE__, 0,0,0,0);\
        return ERROR;\
    }\
    /* ȡ������ָ�뼰���� */\
    pstDrvTable = g_openNodeTable[u32Idx].pstDrvTable;\
    BSP_ASSERT(pstDrvTable);\
    if (NULL == pstDrvTable->functionCB)\
    {\
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "%s callback is NULL, line:%d\n", \
                      #functionCB, __LINE__, 0,0,0,0);\
        return ERROR;\
    }\
    pPrivate = g_openNodeTable[u32Idx].pPrivate;\
    \
    /* �����û��Ļص����� */\
    ret = pstDrvTable->functionCB(pPrivate, param1, param2);\
}while(0)

#ifdef UDI_DEBUG
#define UDI_CHECK_HANDLE(handle) \
do{\
    if ((handle & 0xFF0000) != UDI_OPEN_NODE_HEADER || \
        UDI_HANDLE_TO_IDX(handle) >= UDI_MAX_OPEN_NODE_NUM)\
    {\
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "invalid_handle:0x%x, line:%d\n", \
                  (BSP_U32)handle, __LINE__, 0,0,0,0);\
        return ERROR;\
    }\
}while(0)
#else
#define UDI_CHECK_HANDLE(handle)
#endif

/**************************************************************************
  �ڲ�����
**************************************************************************/
LOCAL UDI_HANDLE udiGetOutOpenNode(VOID)
{
    BSP_U32 u32Cnt;
    UDI_HANDLE handle = UDI_INVALID_HANDLE;

    UDI_MTX_ENTER(g_udiMtxOpen);
    /* ���ȴӵ�ǰλ���� */
    for(u32Cnt = g_openNodeCurPos; u32Cnt < UDI_MAX_OPEN_NODE_NUM; u32Cnt++)
    {
        if (FALSE == g_openNodeTable[u32Cnt].bOpen)
        {
            handle = UDI_IDX_TO_HANDLE(u32Cnt);
            break;
        }
    }

    /* ����, �ٴ�ͷ�� */
    if(UDI_INVALID_HANDLE == handle)
    {
        for (u32Cnt = 0; u32Cnt < g_openNodeCurPos; u32Cnt++)
        {
            if (FALSE == g_openNodeTable[u32Cnt].bOpen)
            {
                handle = UDI_IDX_TO_HANDLE(u32Cnt);
                break;
            }
        }
    }

    /* �ҵ�һ�����õ�handle */
    if (UDI_INVALID_HANDLE != handle)
    {
        g_openNodeCurPos = (u32Cnt+1) % UDI_MAX_OPEN_NODE_NUM;
        g_openNodeTable[u32Cnt].bOpen = TRUE;
    }
    UDI_MTX_LEAVE(g_udiMtxOpen);

    return handle;
}

LOCAL BSP_S32 udiReturnOpenNode(BSP_U32 u32Idx)
{
    BSP_ASSERT(u32Idx < UDI_MAX_OPEN_NODE_NUM);

    UDI_MTX_ENTER(g_udiMtxOpen);
    g_openNodeTable[u32Idx].bOpen = FALSE;
    g_openNodeTable[u32Idx].pstDrvTable = (UDI_DRV_INTEFACE_TABLE*)(-1);
    UDI_MTX_LEAVE(g_udiMtxOpen);
    return OK;
}

/**************************************************************************
  �ӿ�ʵ��
**************************************************************************/

/*****************************************************************************
* �� �� ��  : BSP_UDI_SetPrivate
*
* ��������  : ���������ڲ�˽������
*
* �������  : devId: �豸ID
*             pPrivate: ˽������
* �������  : ��
* �� �� ֵ  : �ɹ�/ʧ��
*****************************************************************************/
BSP_S32 BSP_UDI_SetPrivate(UDI_DEVICE_ID devId, VOID* pPrivate)
{
    BSP_U32 u32MainId;
    BSP_U32 u32DevType;

    UDI_PARSE_DEV_ID(devId, u32MainId, u32DevType);
    g_deviceTable[u32MainId][u32DevType].pPrivate = pPrivate;
    return OK;
}

/*****************************************************************************
* �� �� ��  : BSP_UDI_SetCapability
*
* ��������  : �����豸����ֵ
*
* �������  : devId: �豸ID
*             u32Capability: �豸����ֵ
* �������  : ��
* �� �� ֵ  : �ɹ�/ʧ��
*****************************************************************************/
BSP_S32 BSP_UDI_SetCapability(UDI_DEVICE_ID devId, BSP_U32 u32Capability)
{
    BSP_U32 u32MainId;
    BSP_U32 u32DevType;

    UDI_PARSE_DEV_ID(devId, u32MainId, u32DevType);
    g_deviceTable[u32MainId][u32DevType].u32Capability = u32Capability;
    return OK;
}

/*****************************************************************************
* �� �� ��  : BSP_UDI_SetInterfaceTable
*
* ��������  : �����豸�ص������б�(����������)
*
* �������  : devId: �豸ID
*             pDrvInterface: ������Ļص������б�
* �������  : ��
* �� �� ֵ  : �ɹ�/ʧ��
*****************************************************************************/
BSP_S32 BSP_UDI_SetInterfaceTable(UDI_DEVICE_ID devId, UDI_DRV_INTEFACE_TABLE *pDrvInterface)
{
    BSP_U32 u32MainId;
    BSP_U32 u32DevType;

    UDI_PARSE_DEV_ID(devId, u32MainId, u32DevType);
    g_deviceTable[u32MainId][u32DevType].pDrvInterface = pDrvInterface;
    return OK;
}

/*****************************************************************************
* �� �� ��  : BSP_UDI_Init
*
* ��������  : UDI ģ���ʼ��
*
* �������  : ��
* �������  : ��
* �� �� ֵ  : �ɹ�/ʧ��
*****************************************************************************/
BSP_S32 BSP_UDI_Init(VOID)
{
    UDI_ADP_INIT_CB_T initCB;
    BSP_U32 u32Cnt;

    memset((VOID*)g_deviceTable, 0, sizeof(g_deviceTable));
    memset((VOID*)g_openNodeTable, 0, sizeof(g_openNodeTable));
    UDI_MTX_CREATE(g_udiMtxOpen);
    if (NULL == g_udiMtxOpen)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "create mutex fail, line:%d\n",
                  __LINE__, 0,0,0,0,0);
        return ERROR;
    }

    /* ���ó�ʼ������ */
    for (u32Cnt = 0; u32Cnt < (BSP_U32)UDI_DEV_MAX; u32Cnt++)
    {
        initCB = g_udiInitFuncTable[u32Cnt];
        if (initCB)
        {
            if (initCB() != OK)
            {
                bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "usr initCB fail, line:%d\n",
                          __LINE__, 0,0,0,0,0);
                UDI_MTX_DELETE(g_udiMtxOpen);
                return ERROR;
            }
        }
    }
    return OK;
}

/*****************************************************************************
* �� �� ��  : udi_get_capability
*
* ��������  : �����豸ID��ȡ��ǰ�豸֧�ֵ�����
*
* �������  : devId: �豸ID
* �������  : ��
* �� �� ֵ  : ֧�ֵ�����ֵ
*****************************************************************************/
BSP_S32 udi_get_capability(UDI_DEVICE_ID devId)
{
    BSP_U32 u32MainId;
    BSP_U32 u32DevType;

    UDI_PARSE_DEV_ID(devId, u32MainId, u32DevType);
    return (BSP_S32)g_deviceTable[u32MainId][u32DevType].u32Capability;
}

/*****************************************************************************
* �� �� ��  : udi_open
*
* ��������  : ���豸(����ͨ��)
*
* �������  : pParam: �豸�Ĵ����ò���
* �������  : ��
* �� �� ֵ  : -1:ʧ�� / ����:�ɹ�
*****************************************************************************/
UDI_HANDLE udi_open(UDI_OPEN_PARAM *pParam)
{
    BSP_U32 u32MainId;
    BSP_U32 u32DevType;
    BSP_U32 u32Idx;
    UDI_HANDLE handle;
    UDI_DRV_INTEFACE_TABLE *pstDrvTable;

    if (NULL == pParam)
    {
        goto UDI_OPEN_ERR;
    }

    UDI_PARSE_DEV_ID(pParam->devid, u32MainId, u32DevType);

    /* ����һ�����õĽڵ� */
    handle = udiGetOutOpenNode();
    if (UDI_INVALID_HANDLE == handle)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "can't find open node, line:%d\n",
                  __LINE__, 0,0,0,0,0);
        goto UDI_OPEN_ERR;
    }
    u32Idx = UDI_HANDLE_TO_IDX(handle);

    /* �����û��ص����� */
    pstDrvTable = g_deviceTable[u32MainId][u32DevType].pDrvInterface;
    if (NULL == pstDrvTable ||
        NULL == pstDrvTable->udi_open_cb)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "usr open cb is NULL, line:%d\n",
                  __LINE__, 0,0,0,0,0);
        goto UDI_OPEN_ERR_RET_NODE;
    }
    if (pstDrvTable->udi_open_cb(pParam, handle))
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "usr open fail, line:%d\n",
                  __LINE__, 0,0,0,0,0);
        goto UDI_OPEN_ERR_RET_NODE;
    }

    /* ��������˽������ */
    g_openNodeTable[u32Idx].pstDrvTable =
        g_deviceTable[u32MainId][u32DevType].pDrvInterface;
    g_openNodeTable[u32Idx].pPrivate =
        g_deviceTable[u32MainId][u32DevType].pPrivate;

    return handle;

UDI_OPEN_ERR_RET_NODE:
    (BSP_VOID)udiReturnOpenNode(u32Idx);
UDI_OPEN_ERR:
    return UDI_INVALID_HANDLE;
}

/*****************************************************************************
* �� �� ��  : udi_close
*
* ��������  : �ر��豸(����ͨ��)
*
* �������  : handle: �豸��handle
* �������  : ��
* �� �� ֵ  : ��
*****************************************************************************/
BSP_S32 udi_close(UDI_HANDLE handle)
{
    BSP_S32 s32Ret = -1;
    BSP_U32 u32Idx;
    UDI_DRV_INTEFACE_TABLE *pstDrvTable;

    UDI_CHECK_HANDLE(handle);

    u32Idx = UDI_HANDLE_TO_IDX(handle);

    if (u32Idx >= UDI_MAX_OPEN_NODE_NUM)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "invalid handle:0x%x, line:%d\n",
                  handle, __LINE__, 0,0,0,0);
        return ERROR;
    }
    /* �����û��Ļص����� */
    pstDrvTable = g_openNodeTable[u32Idx].pstDrvTable;
    BSP_ASSERT(pstDrvTable);
    if (NULL == pstDrvTable->udi_close_cb)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "udi_close_cb is NULL, line:%d\n",
                  __LINE__, 0,0,0,0,0);
        return ERROR;
    }
    s32Ret = pstDrvTable->udi_close_cb(g_openNodeTable[u32Idx].pPrivate);

    /* �ͷ� Open Node */
    (BSP_VOID)udiReturnOpenNode(u32Idx);
    return s32Ret;
}

/*****************************************************************************
* �� �� ��  : udi_write
*
* ��������  : ����д
*
* �������  : handle:  �豸��handle
*             pMemObj: buffer�ڴ� �� �ڴ��������
*             u32Size: ����д�ߴ� �� �ڴ��������ɲ�����
* �������  :
*
* �� �� ֵ  : ����ֽ��� �� �ɹ�/ʧ��
*****************************************************************************/
BSP_S32 udi_write(UDI_HANDLE handle, void* pMemObj, BSP_U32 u32Size)
{
    BSP_S32 s32Ret = -1;

    UDI_CALL_OPT_FUNC_SAFELY(handle, pMemObj, u32Size, udi_write_cb, s32Ret);
    return s32Ret;
}

/*****************************************************************************
* �� �� ��  : udi_read
*
* ��������  : ���ݶ�
*
* �������  : handle:  �豸��handle
*             pMemObj: buffer�ڴ� �� �ڴ��������
*             u32Size: ���ݶ��ߴ� �� �ڴ��������ɲ�����
* �������  :
*
* �� �� ֵ  : ����ֽ��� �� �ɹ�/ʧ��
*****************************************************************************/
BSP_S32 udi_read(UDI_HANDLE handle, void* pMemObj, BSP_U32 u32Size)
{
    BSP_S32 s32Ret = -1;

    UDI_CALL_OPT_FUNC_SAFELY(handle, pMemObj, u32Size, udi_read_cb, s32Ret);
    return s32Ret;
}

/*****************************************************************************
* �� �� ��  : udi_ioctl
*
* ��������  : ����ͨ����������
*
* �������  : handle: �豸��handle
*             u32Cmd: IOCTL������
*             pParam: ��������
* �������  :
*
* �� �� ֵ  : �ɹ�/ʧ��
*****************************************************************************/
BSP_S32 udi_ioctl(UDI_HANDLE handle, BSP_U32 u32Cmd, VOID* pParam)
{
    BSP_S32 s32Ret = -1;

    UDI_CALL_OPT_FUNC_SAFELY(handle, u32Cmd, pParam, udi_ioctl_cb, s32Ret);
    return s32Ret;
}


/**************************************************************************
  ������Ϣʵ��
**************************************************************************/
extern BSP_S32 BSP_UDI_DumpOpenNode(BSP_BOOL bDumpAll);
extern BSP_S32 BSP_UDI_DumpDev(BSP_BOOL bAll);

BSP_S32 BSP_UDI_DumpOpenNode(BSP_BOOL bDumpAll)
{
    BSP_U32 cnt;

    printf("+- BSP UDI OpenNode Info Dump:\n");
    printf("|-- g_openNodeCurPos:%d\n", g_openNodeCurPos);
    for(cnt = 0; cnt < UDI_MAX_OPEN_NODE_NUM; cnt++)
    {
        if (bDumpAll || g_openNodeTable[cnt].bOpen)
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "|-+-- OpenNode idx:           %d:\n", cnt, 0, 0, 0, 0, 0);
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "  |-- OpenNode private:       0x%x\n", (BSP_U32)g_openNodeTable[cnt].pPrivate, 0, 0, 0, 0, 0);
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "  |-- OpenNode close cb:      0x%x\n", (BSP_U32)g_openNodeTable[cnt].pstDrvTable->udi_close_cb, 0, 0, 0, 0, 0);
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "  |-- OpenNode open  cb:      0x%x\n", (BSP_U32)g_openNodeTable[cnt].pstDrvTable->udi_open_cb, 0, 0, 0, 0, 0);
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "  |-- OpenNode write cb:      0x%x\n", (BSP_U32)g_openNodeTable[cnt].pstDrvTable->udi_write_cb, 0, 0, 0, 0, 0);
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "  |-- OpenNode read  cb:      0x%x\n", (BSP_U32)g_openNodeTable[cnt].pstDrvTable->udi_read_cb, 0, 0, 0, 0, 0);
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "  |-- OpenNode ioctl cb:      0x%x\n", (BSP_U32)g_openNodeTable[cnt].pstDrvTable->udi_ioctl_cb, 0, 0, 0, 0, 0);
        }
    }
    return 0;
}

#define BSP_UDI_PRINT_DEVID_BEG(devid) {switch(devid){
#define BSP_UDI_PRINT_DEVID_ITEM(name) case(name):return ((BSP_U8*)(#name));
#define BSP_UDI_PRINT_DEVID_END() default:return ((BSP_U8*)("unknown")); }}

LOCAL BSP_U8* udiGetDevID(UDI_DEVICE_ID devid)
{
    BSP_UDI_PRINT_DEVID_BEG(devid)

    BSP_UDI_PRINT_DEVID_ITEM(UDI_ACM_CTRL_ID)
    BSP_UDI_PRINT_DEVID_ITEM(UDI_ACM_AT_ID)
    BSP_UDI_PRINT_DEVID_ITEM(UDI_ACM_SHELL_ID)
    BSP_UDI_PRINT_DEVID_ITEM(UDI_ACM_OM_ID)
    BSP_UDI_PRINT_DEVID_ITEM(UDI_ACM_MODEM_ID)
    BSP_UDI_PRINT_DEVID_ITEM(UDI_NCM_NDIS_ID)
    BSP_UDI_PRINT_DEVID_ITEM(UDI_NCM_CTRL_ID)

    BSP_UDI_PRINT_DEVID_END()
}

LOCAL BSP_U8* udiGetDevMainID(UDI_DEVICE_MAIN_ID mainId)
{
    BSP_UDI_PRINT_DEVID_BEG(mainId)

    BSP_UDI_PRINT_DEVID_ITEM(UDI_DEV_USB_ACM)
    BSP_UDI_PRINT_DEVID_ITEM(UDI_DEV_USB_NCM)
    BSP_UDI_PRINT_DEVID_ITEM(UDI_DEV_ICC)

    BSP_UDI_PRINT_DEVID_END()
}

LOCAL BSP_U8* udiGetDevTypeID(BSP_U32 mainId, BSP_U32 type)
{
    switch(mainId)
    {
    case UDI_DEV_USB_ACM:
    {
        BSP_UDI_PRINT_DEVID_BEG(type)
        BSP_UDI_PRINT_DEVID_ITEM(UDI_ACM_CTRL_ID)
        BSP_UDI_PRINT_DEVID_ITEM(UDI_USB_ACM_AT)
        BSP_UDI_PRINT_DEVID_ITEM(UDI_USB_ACM_SHELL)
        BSP_UDI_PRINT_DEVID_ITEM(UDI_USB_ACM_LTE_DIAG)
        BSP_UDI_PRINT_DEVID_ITEM(UDI_USB_ACM_OM)
        BSP_UDI_PRINT_DEVID_ITEM(UDI_USB_ACM_MODEM)
        BSP_UDI_PRINT_DEVID_END()
    }
    case UDI_DEV_USB_NCM:
    {
        BSP_UDI_PRINT_DEVID_BEG(type)
        BSP_UDI_PRINT_DEVID_ITEM(UDI_USB_NCM_NDIS)
        BSP_UDI_PRINT_DEVID_ITEM(UDI_USB_NCM_CTRL)
        BSP_UDI_PRINT_DEVID_END()
    }
    default:
        return ((BSP_U8*)("unknown"));
    }
}

BSP_S32 BSP_UDI_DumpDev(BSP_BOOL bAll)
{
    BSP_U32 mainId, typeId;

    printf("+- BSP UDI OpenNode Info Dump:\n");
    for (mainId = 0; mainId < UDI_MAX_MAIN_DEV_NUM; mainId++)
    {
        for (typeId = 0; typeId < UDI_MAX_DEV_TYPE_NUM; typeId++)
        {
            if (bAll || NULL != g_deviceTable[mainId][typeId].pDrvInterface)
            {
                bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "|-Dev Name:%s\n", udiGetDevID((UDI_DEVICE_ID)UDI_BUILD_DEV_ID(mainId, typeId)), 0, 0, 0, 0, 0);
                bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "|-+-- mainId:%s, typeId:%s\n", udiGetDevMainID((UDI_DEVICE_MAIN_ID)mainId), udiGetDevTypeID(mainId, typeId), 0, 0, 0, 0, 0);
                bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "  |-- dev private:       0x%x\n", (BSP_U32)g_deviceTable[mainId][typeId].pPrivate, 0, 0, 0, 0, 0);
                bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "  |-- dev close cb:      0x%x\n", (BSP_U32)g_deviceTable[mainId][typeId].pDrvInterface->udi_close_cb, 0, 0, 0, 0, 0);
                bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "  |-- dev open  cb:      0x%x\n", (BSP_U32)g_deviceTable[mainId][typeId].pDrvInterface->udi_open_cb, 0, 0, 0, 0, 0);
                bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "  |-- dev write cb:      0x%x\n", (BSP_U32)g_deviceTable[mainId][typeId].pDrvInterface->udi_write_cb, 0, 0, 0, 0, 0);
                bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "  |-- dev read  cb:      0x%x\n", (BSP_U32)g_deviceTable[mainId][typeId].pDrvInterface->udi_read_cb, 0, 0, 0, 0, 0);
                bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "  |-- dev ioctl cb:      0x%x\n", (BSP_U32)g_deviceTable[mainId][typeId].pDrvInterface->udi_ioctl_cb, 0, 0, 0, 0, 0);
                bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_UDI, "  |  \n");
            }
        }
    }
    return 0;
}


#ifdef __cplusplus
}
#endif /* __cplusplus */



