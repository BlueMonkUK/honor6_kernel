
#include <bsp_cpufreq.h>
#include <drv_comm.h>

#ifdef __cplusplus
extern "C"
{
#endif


void  cpufreq_init(void)
{
    return ;
}
/*�ṩpm���̵��ã��������Ƶ��CPU/DDR*/
void cpufreq_set_max_freq(void)
{
	
}
/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_GetCcpuLoadCB
 ��������  : arm �ṩ��TTF�Ļص�����
 �������  : pFunc:TTF����ָ��
 �������  : ��
 �� �� ֵ  : ����ռ�ĵ�ַ ��
*****************************************************************************/
 void BSP_PWRCTRL_GetCcpuLoadCB(PWRCTRLFUNCPTR pFunc )
 {
	(void)(*pFunc)(1);
 }
/*
 * ��ȡ��ǰprofile
 */
int pwrctrl_dfs_get_profile()
{
	return 0;
}
/*
 * ����profile
 * success: return BSP_OK
 * fail:    return BSP_ERROR
 */
int pwrctrl_dfs_set_profile(int profile)
{
	return BSP_OK;
}

/*
 * ����profile����
 * success: return BSP_OK	
 * fail:    return BSP_ERROR
 */
int pwrctrl_dfs_set_baseprofile(int baseprofile)
{
	return BSP_OK;
}

/*
 * ������Ƶ lock=0����;lock=1����
 */
void pwrctrl_dfs_lock(u32 lock)
{
	
}

/*
 * ���Խӿڣ�����Ƶ�� BSP_ERROR ����ʧ��;BSP_OK ���óɹ�
 *
 */
int pwrctrl_dfs_target(int a9freq, int ddrfreq, int slowfreq)
{
	return BSP_OK;
}

int pwrctrl_dfs_current(int *a9freq, int *ddrfreq, int *slowfreq)
{
	return BSP_OK;
}

void cpufreq_update_frequency(void)
{
	
}
void cpufreq_checkload_in_sysint(void)
{

}
/*for v9r1*/
BSP_S32 PWRCTRL_DfsQosRequest(BSP_S32 qos_id, BSP_U32 req_value, BSP_S32 *req_id)
{
	return 0;
}
BSP_S32 PWRCTRL_DfsQosRelease(BSP_S32 qos_id, BSP_S32 *req_id)
{
	return 0;
}
BSP_S32 PWRCTRL_DfsQosUpdate(BSP_S32 qos_id, BSP_S32 req_id, BSP_U32 req_value)
{
	return 0;
}

/********************************adp interface start**************************************/

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_GetCpuLoad
 ��������  : arm cpuռ���ʲ�ѯ����
 �������  : ��
 �������  : ulAcpuLoad:Acpuռ���ʵ�ַ.
             ulCcpuLoad:Ccpuռ���ʵ�ַ.
 �� �� ֵ  : 0/1 ��
*****************************************************************************/
 unsigned int DRV_PWRCTRL_GetCpuLoad(unsigned int *ulAcpuLoad,unsigned int *ulCcpuLoad)
 {
	return 0;
 }


/*****************************************************************************
Function:   DRV_PWRCTRL_DFS_SET_PRF_CCPU
Description:Set the System Min Profile
Input:      ulClientID: The Vote Module Client
            enProfile:  The Min Profile Value
Output:     None
Return:     The Vote Result
Others:
*****************************************************************************/
 BSP_U32  DRV_PWRCTRL_DFS_SET_PRF_CCPU(PWC_DFS_ID_E ulClientID, PWC_DFS_PROFILE_E enProfile)
 {
	return 0;
 }

/*****************************************************************************
Function:   DRV_PWRCTRL_DFS_RLS_PRF_CCPU
Description:Release the Vote Result
Input:      ulClientID: The Vote Module Client
Output:     None;
Return:     The Vote Result
Others:
*****************************************************************************/
BSP_U32 DRV_PWRCTRL_DFS_RLS_PRF_CCPU(PWC_DFS_ID_E ulClientID)
{
	return 0;
}
 /*****************************************************************************
 �� �� ��  : BSP_DFS_GetCurCpuLoad
 ��������  : ��ѯ��ǰCPU
 �������  : pu32AcpuLoad ACPUloadָ��
             pu32CcpuLoad CCPUloadָ��
 �������  : pu32AcpuLoad ACPUloadָ��
             pu32CcpuLoad CCPUloadָ��
 �� �� ֵ  : 0:  �����ɹ���
                      -1������ʧ�ܡ�
*****************************************************************************/
BSP_U32 BSP_DFS_GetCurCpuLoad(BSP_U32 *pu32AcpuLoad,BSP_U32 *pu32CcpuLoad)
{
    return ERROR;
}
/********************************adp interface end***************************************/

#ifdef __cplusplus
}
#endif

