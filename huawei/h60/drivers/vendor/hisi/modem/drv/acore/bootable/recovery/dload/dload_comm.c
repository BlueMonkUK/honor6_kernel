
#include <linux/types.h>
#include <linux/fs.h>
#include <fcntl.h>
#include <mtd/mtd-abi.h>
#include "dload_comm.h"
#include "mtdutils/mtdutils.h"
#include "cutils/android_reboot.h"

/*--------------------------------------------------------------*
 * �궨��                                                       *
 *--------------------------------------------------------------*/
#define OOB_SIZE (16)
#define PTABLE_SIZE AXI_PTABLE_SIZE_MAX
#define RECOVERY_ANAME "recovery-a"
#define RECOVERY_BNAME "recovery-b"
#define PTABLE_NAME	"ptable"
#define PTABLE_HEAD_SIZE                sizeof(struct ST_PART_TBL)
//#define PTABLE_END_STR          "T" /*"pTableTail"*/


/*--------------------------------------------------------------*
 * ȫ�ֱ�������                                                 *
 *--------------------------------------------------------------*/
//enum IMAGE_TYPE g_ucFinishImageType         = IMAGE_SFS_TOP;
//unsigned int g_ulDloadMidwayRestartFlag=0;
//unsigned int g_ulFlashDloadAlignSize;    /*��Ҫ�����⼸����С�Ķ���:1.USB���ݰ���С4k, 2.nandpage��С2k��3.yaffspage��С2k+16 */
unsigned int mtd_partitions_scanned =0;

/*--------------------------------------------------------------*
 * �ⲿ��������                                                 *
 *--------------------------------------------------------------*/
extern  struct dload_ctrl_block *dcb;

void dload_reboot(void)
{
	android_reboot(ANDROID_RB_RESTART, 0, 0);
}
/*****************************************************************************
* �� �� ��    : write_ptable_to_flash
*
* ��������  : ��flash��д��ptable   
*
* �������  : 
			data:��Ҫд�������buf
			size:��Ҫд������ݳ���
* �������  : 
*
* �� �� ֵ    : 
			0:  	д��ɹ�
			-1:	д��ʧ��
* ����˵��  : 
*
*****************************************************************************/

int write_ptable_to_flash(char *data,ssize_t size)
{
	MtdWriteContext *write;

	if (!mtd_partitions_scanned) {
                mtd_scan_partitions();
                mtd_partitions_scanned = 1;
          }

	/*���ҵ�flash�е�ptable����*/
	const MtdPartition* part=mtd_find_partition_by_name(PTABLE_NAME);
	if (part == NULL) {
                Dbug("mtd partition \"%s\" not found for writing\n", PTABLE_NAME);
	        return ERROR;
         }

	/*��д�ķ�ʽ��ptable����*/
	write=mtd_write_partition(part);
	if(write==NULL){
		Dbug("Error:can't open ptable partition!\n");
		return ERROR;
	}
	
	/*��flash��д���޸ĺ��ptable*/
	if(mtd_write_data(write,data,size)!=size){
		Dbug("Error:can't write ptable partition!\n");
		mtd_write_close(write);
		return ERROR;	
	}

	if(mtd_erase_blocks(write, -1) < 0) {
                Dbug("error finishing mtd write of ptable!\n");
                mtd_write_close(write);
		return ERROR;
	}
	/*�ر�ptable����*/
	if(mtd_write_close(write)){
		Dbug("Error:can't finish ptable partition!\n");
		return ERROR;
	}
	return OK;
}

/*****************************************************************************
* �� �� ��    : dload_get_proc_ptable
*
* ��������  : ͨ��proc_fs��ȡ�ڴ��з�ȡ�����Ϣ
*
* �������  : 
			data:��Ҫ����������buf
			size:��Ҫ���������ݳ���
* �������  : 
*
* �� �� ֵ    : 
			0:  	��ȡ�ɹ�
			-1:	��ȡʧ��
* ����˵��  : �˺�����ȡ�ķ���������ͷ����Ϣ����Ҫget_ptable_from_ram����������ſ���ʹ��
*
*****************************************************************************/

static int dload_get_proc_ptable(char *ptable_product,int len)
{
	int ret;
	int fd;
	char ptable_procfile_name[50]="/proc/ptable/ptable_bin";

	if(len<0)
	{
		Dbug("Error:buf len is invalid!\n");
		return -1;
	}
	/*�Զ��ķ�ʽ��ptable proc�ļ�*/
	fd=open(ptable_procfile_name,O_RDONLY);
	if(fd<0)
	{
		Dbug("Error:can not open ptable proc file!ERROR NO is %d\n",ret);
		return -1;
	}
	/*���������е����ݶ�����������buf��*/
	ret=read(fd,ptable_product,len-1);
	
	close(fd);
	if(ret<0)
	{
		Dbug("Error:can not open ptable proc file!ERROR NO is %d\n",ret);
		free(ptable_product);
		return -1;
	}
	ptable_product[ret]='\0';
	
	return ret;

}

/*****************************************************************************
* �� �� ��    : get_ptable_from_ram
*
* ��������  : ��ȡ�ڴ��еķ�����
*
* �������  : 
			ptable:��Ҫ����������buf
			
* �������  : 
*
* �� �� ֵ    : 
			  0:  	��ȡ�ɹ�
			-1:	��ȡʧ��
* ����˵��  : 
*
*****************************************************************************/
int get_ptable_from_ram(struct ST_PART_TBL* ptable)
{
	struct ST_PART_TBL* ptable_temp;
	char *ptable_proc=NULL;
	char *ptable_proc_temp=ptable_proc;
	int ret;					/*���ڼ�¼���ڴ��ж�����ptable��Ϣ�ĳ���*/

	/*����2k�ռ�,���ڴ��ڴ�洢���ڴ��ж�������ptable*/
	ptable_proc=(char *)malloc(PTABLE_SIZE);	
	if(NULL==ptable_proc)
	{
		Dbug("malloc for proc ptable fail!");
		goto error;
	}

	/*��ȡ�ڴ��еķ�������Ϣ*/
	ret=dload_get_proc_ptable(ptable_proc, PTABLE_SIZE);
	if(ret<0)
	{
		Dbug("Error:get ptable proc file fail!\n");
		goto error;
	}

	/*�������ڴ��ж�����pable��Ϣ*/
	int	i=0;
	ptable_temp=ptable;
	ptable_proc=ptable_proc+PTABLE_HEAD_SIZE;
	while(ret>0&&NULL!=ptable_temp)
	{
		int matches=0;
		ptable_temp->name[0]='\0';


		matches=sscanf(ptable_proc,"%x %x %x %x %x %x %x %x \"%15[^\"]",
								&ptable_temp->image,
								&ptable_temp->offset,
								&ptable_temp->loadsize,
								&ptable_temp->capacity,
								&ptable_temp->loadaddr,
								&ptable_temp->entry,
								&ptable_temp->property,
								&ptable_temp->count,
								ptable_temp->name);
		
		/*����ɹ���ȡһ�����������ݣ�ָ������ƶ�*/
		if(matches==9){
			ptable_temp++;
		}
		while(ret>0&&*ptable_proc!='\n'){
			ptable_proc++;
			ret--;
		}
		if(ret>0)
		{
			ptable_proc++;
			ret--;
		}
	}
	ptable_temp->name[0]='\0';
	strcat(ptable_temp->name,PTABLE_END_STR);
	free(ptable_proc_temp);
	return OK;
error:
	free(ptable_proc_temp);
	return ERROR;
}

/*****************************************************************************
* �� �� ��    : get_ptable_from_flash
*
* ��������  : ��ȡflash�еķ�����
*
* �������  : 
			ptable:��Ҫ����������buf
			
* �������  : 
*
* �� �� ֵ    : 
			  0:  	��ȡ�ɹ�
			-1:	��ȡʧ��
* ����˵��  : �˺�����ȡ�ķ��������ͷ����Ϣ��Ϊchar*��ʽ
*
*****************************************************************************/
int get_ptable_from_flash(char* ptable)
{
	MtdReadContext *read;
	ssize_t size=PTABLE_SIZE;
	char* data;

	data=(char *)malloc(size);
	if (!mtd_partitions_scanned) {
                mtd_scan_partitions();
                mtd_partitions_scanned = 1;
          }

	/*���ҵ�flash�е�ptable����*/
	const MtdPartition* part=mtd_find_partition_by_name(PTABLE_NAME);
	if (part == NULL) {
                Dbug("mtd partition \"%s\" not found for writing\n", PTABLE_NAME);
	        goto error;
         }
	/*�Զ��ķ�ʽ�򿪷���*/
	read=mtd_read_partition(part);
	if(read==NULL){
		Dbug("Error:can't open ptable\n");

		goto error;
	}
	/*��ȡ�����е�����*/
	ssize_t ret=mtd_read_data(read,data,size);
	if(ret!=size){
		Dbug("Error:mtd can't read ptable!\n");
		goto error;
	}
	/*��������ϣ��ر�*/
	mtd_read_close(read);
	if(ret!=size){
		Dbug("Error:mtd can't read ptable!\n");
		goto error;
	}
	/*�޸ķ������ж�Ӧ��recovery������countֵ*/
	memcpy(ptable,data,PTABLE_SIZE);

	free(data);
	return OK;
	
error:
	free(data);
	return ERROR;

}

/*****************************************************************************
* �� �� ��    : get_ptable_changed
*
* ��������  : �жϷ������Ƿ�仯
*
* �������  : 
			ptable:��Ҫ����������buf
			
* �������  : TODO
*
* �� �� ֵ    :  TODO
			  0:  	�ޱ仯
			-1:	�����仯
* ����˵��  :  TODO
*
*****************************************************************************/

int get_ptable_changed(void)
{
	struct ST_PART_TBL* old_ptable;		/*�ɷ�����(�ڴ��еķ�����)*/
	struct ST_PART_TBL* new_ptable;	/*�·�����(flash�еķ�����)*/
	char *new_ptable_temp=NULL;
	char *old_ptable_temp =NULL;
	char old_ptable_name[PTABLE_NAME_SIZE];
	char old_ptable_property[PTABLE_HEAD_PROPERTY_SIZE];
	char old_ptable_ver[PTABLE_VER_ID_SIZE*2];
	int matches=0;
	int changed=0;
	int ret;

	/*����2k�ռ�,���ڴ��ڴ�洢���ڴ��ж�������ptable*/
	old_ptable_temp=(char *)malloc(PTABLE_SIZE);
	if(NULL==old_ptable_temp)
	{
		Dbug("malloc for proc ptable fail!");
		return ERROR;
	}

	/*����2k�ռ�,���ڴӴ洢flash�е�ptable*/
	new_ptable_temp=(char *)malloc(PTABLE_SIZE);
	if(NULL==new_ptable_temp)
	{
		Dbug("malloc for flash ptable fail!");
		free(old_ptable_temp);
		return ERROR;
	}


	/*��ȡ�ڴ��еķ�������Ϣ*/
	ret=dload_get_proc_ptable(old_ptable_temp, PTABLE_SIZE);
	if(ret<0)
	{
		Dbug("Error:get ptable proc file fail!\n");
		changed=-1;
		goto exit;
	}
	
	/*��ȡflash�еķ�����*/
	ret=get_ptable_from_flash(new_ptable_temp);
		if(ret<0)
	{
		Dbug("Error:get ptable from flash fail!\n");
		changed=-1;
		goto exit;
	}

	/*�������ڴ��ж������ķ������ͷ����Ϣ*/
	/*proc fs�е�������ʽ
	seq_printf(sfile,"%s %s %s\n",
				(char*)ptable_product+PTABLE_HEAD_PROPERTY_OFFSET,
				(char*)ptable_product + PTABLE_BOOTROM_VER_OFFSET,
				(char*)ptable_product + PTABLE_NAME_OFFSET);

	*/
	old_ptable_name[0]='\0';//memset
	old_ptable_property[0]='\0';
	old_ptable_ver[0]='\0';
	
	matches=sscanf(old_ptable_temp,"%s %s %s ",
						old_ptable_name,
						old_ptable_property,
						old_ptable_ver);
	if(3!=matches)
	{
		Dbug("Error:can't get old ptable head infomation");
		changed=-1;
		goto exit;
	}
	printf("old_ptable_name=%s old_ptable_property=%s old_ptable_ver=%s ",
			old_ptable_name,
			old_ptable_property,
			old_ptable_ver);
	
	/*�Ƚ��¡�������������ı�ͷ��Ϣ�Ƿ��в���*/	
	    if(((!strncmp((const void *)(new_ptable_temp + PTABLE_NAME_OFFSET),old_ptable_name,PTABLE_NAME_SIZE)) \
            || (!strncmp((const void *)(new_ptable_temp + PTABLE_HEAD_PROPERTY_OFFSET),old_ptable_property,PTABLE_HEAD_PROPERTY_SIZE))) \
            || (!strncmp((const void *)(new_ptable_temp + PTABLE_BOOTROM_VER_OFFSET),old_ptable_ver,PTABLE_VER_ID_SIZE)))
	    {
		changed=1;
		Dbug("Ptable head has changed!\n");
		goto exit;
	    }
	
	/*�Ƚ��¡���������������count��size���У������Ƿ��в��*/
	new_ptable=(struct ST_PART_TBL*)(new_ptable_temp+PTABLE_HEAD_SIZE);
	old_ptable=(struct ST_PART_TBL*)(new_ptable_temp+PTABLE_HEAD_SIZE);

	while((0 != strcmp((const void *)PTABLE_END_STR, (const void *)old_ptable->name)) &&
          		(0 != strcmp((const void *)PTABLE_END_STR, (const void *)new_ptable->name)))
	{
		if(
	                (old_ptable->offset != new_ptable->offset)      ||
	                (old_ptable->capacity != new_ptable->capacity)  ||
	                (old_ptable->loadaddr != new_ptable->loadaddr)  ||
	                (old_ptable->entry != new_ptable->entry)  ||
	                (old_ptable->image != new_ptable->image)  ||
	                (0 != strcmp( (const void *)old_ptable->name, (const void *)new_ptable->name))
	           )
	        {
		            /*���ֲ�ͬ�����ñ�־λ*/
		            changed=1;
			    Dbug("ptable context changed\n");
		            goto exit;
	        }
        
	        old_ptable++;
	        new_ptable++;
    	}

		/*���ϱ��Ƿ�����в���*/
	    if((0 != strcmp(PTABLE_END_STR, new_ptable->name))||(0 != strcmp(PTABLE_END_STR, old_ptable->name)))
	    {
			/*���ָ������죬���ñ�־λ*/
			changed=1;
			Dbug("ptable number changed \n");
			goto exit;
	    }

exit:	
	free(old_ptable_temp);
	free(new_ptable_temp);
	return changed;
}

/*****************************************************************************
* �� �� ��    : dload_get_nand_page_size
*
* ��������  : ��ȡ��nand flash��ҳ��С
*
* �������  : 
			fd:����mtd�豸ʱ��Ӧ���ļ�������
			
* �������  : ��
*
* �� �� ֵ    : 
			  ret:  	nand��ҳ��С
			  -1:		��ȡʧ��
* ����˵��  : 
*
*****************************************************************************/

unsigned int get_nand_page_size(int fd)
{
	struct mtd_info_user mtd_info;
	int ret;
	memset(&mtd_info,0,sizeof(mtd_info));

	/*��ȡmtd_info*/
	ret=ioctl(fd,MEMGETINFO,&mtd_info);
	
	if ( ret < 0) {
		Dbug("system image:write data error no is%d \n",ret);
		return ERROR;
	}
	/*����nand page �Ĵ�С*/
	ret=mtd_info.writesize;
	Dbug("nand page size is 0x%x\n",ret);
	
	return ret;
}

/*****************************************************************************
* �� �� ��    : get_image_property
*
* ��������  : ��ȡimage������(��������֮ǰ����Ҫ�жϸþ������ļ�ϵͳ���� ��������ͨ����)
*
* �������  : 
			image_idx:	��Ҫ��д�����ID	
			
* �������  : ��
*
* �� �� ֵ    : 
			  property:  	���������ֵ
				   -1:	����ʧ��
* ����˵��  : 
*
*****************************************************************************/
int get_image_property(unsigned int image_idx)
{
	int ret;
	int property=-1;
	struct ST_PART_TBL *ptable_temp;
	struct ST_PART_TBL *ptable=(struct ST_PART_TBL *)malloc(PTABLE_SIZE);
	if(!ptable)
	{
		Dbug("Error:can't malloc for ptable_product!\n");
		return ERROR;
	}

	/*��ȡram�еķ�����������ͷ����Ϣ*/
	ptable_temp=ptable;
	ret=get_ptable_from_ram(ptable);
	if(ret<0)
	{
		Dbug("Error:get ptable from ram failed\n");
		goto error;
	}

	/*��ȡ��ptable��IDΪimage_idx�����countֵ*/
	while(NULL!=ptable&&strcmp(ptable->name,PTABLE_END_STR))
	{
		if(ptable->image==image_idx){
				property=ptable->property;
		}
		else{
			;
		}
		ptable++;
	}

	free(ptable_temp);
	return property;
error:

	free(ptable_temp);
	return ERROR;
}
/*****************************************************************************
* �� �� ��    : update_image_count
*
* ��������  : ��������ʱ���޸ķ������е�count ��ֵ 
*
* �������  : ��			
			
* �������  : ��
*
* �� �� ֵ    : 
			  0:  	�����ɹ�
			-1:	����ʧ��
* ����˵��  : 
*
*****************************************************************************/

int update_image_count(enum IMAGE_TYPE image_idx)
{
	char *name;
	char*ptable_flash;	/*������Ŵ�flash�ж�����ptable,������ͷ����Ϣ*/
	struct ST_PART_TBL *ptable_flash_temp;
	int ret;

	if(IMAGE_RECOVERY_A==image_idx)
		return update_recovery_count();

	/*�����ڴ�*/
	ptable_flash=(char *)malloc(PTABLE_SIZE);
	if(NULL==ptable_flash)
	{
		Dbug("Error:malloc failed!\n");
		return ERROR;
	}

	/*��ȡflash�еķ���������ͷ����Ϣ*/
	ret=get_ptable_from_flash(ptable_flash);
	if(ret<0)
	{
		Dbug("Error:get ptable from flash failed\n");
		goto error;
	}
	
	/*ȥ���������ͷ����Ϣ(�����ܲ���עͷ����Ϣ)*/
	ptable_flash_temp=(struct ST_PART_TBL *)(ptable_flash+PTABLE_HEAD_SIZE);

	/*����imageID�ҵ���Ӧ�ľ���ͬʱ���¾����countֵ*/
	while(NULL!=ptable_flash_temp&&strcmp(ptable_flash_temp->name,PTABLE_END_STR))
	{
		if(ptable_flash_temp->image==image_idx){
				ptable_flash_temp->count=ptable_flash_temp->count+1;
		}
		else{
			;
		}
		ptable_flash_temp++;
	}
	
	/*�����º��bufд��flash*/
	ret=write_ptable_to_flash(ptable_flash,PTABLE_SIZE);
	if(ret<0){
		Dbug("Error:update ptable failed!\n");
		goto error;
	}
	
	free(ptable_flash);
	return OK;

error:
	free(ptable_flash);
	return ERROR;
	
}
/*****************************************************************************
* �� �� ��    : update_ptable_count
*
* ��������  : ����ptable����ʱ�����·������е�count ��ֵ 
*
* �������  : ��			
			
* �������  : ��
*
* �� �� ֵ    : 
			  0:  	�����ɹ�
			-1:	����ʧ��
* ����˵��  : 
*
*****************************************************************************/
int update_ptable_count(void)
{
	struct ST_PART_TBL *ptable_flash;	/*������Ŵ�flash�ж�����ptable,������ͷ����Ϣ*/
	struct ST_PART_TBL *ptable_ram;	/*������Ŵ��ڴ��ж�����ptable*/
	struct ST_PART_TBL *ptable_ram_temp;	/*Ϊ���ͷ�ptable_ram����Ŀռ�*/
	char *ptable_flash_all;		/*������Ŵ�flash�ж�����ptable,����ͷ����Ϣ*/
	int ret=-1;

	/*�����ڴ�*/
	ptable_flash_all=(char *)malloc(PTABLE_SIZE);
	if(NULL==ptable_flash_all)
	{
		Dbug("Error:malloc failed!\n");
		return ERROR;
	}
	ptable_ram=(struct ST_PART_TBL *)malloc(PTABLE_SIZE);
	if(NULL==ptable_ram)
	{
		Dbug("Error:malloc failed!\n");
		free(ptable_flash_all);
		return ERROR;
	}

	/*��ȡflash�еķ���������ͷ����Ϣ*/
	ret=get_ptable_from_flash(ptable_flash_all);
	if(ret<0)
	{
		Dbug("Error:get ptable from flash failed\n");
		goto error;
	}
	
	/*ȥ���������ͷ����Ϣ(�����ܲ���עͷ����Ϣ)*/
	ptable_flash=(struct ST_PART_TBL *)(ptable_flash_all+PTABLE_HEAD_SIZE);

	/*��ȡram�еķ�����������ͷ����Ϣ*/
	ptable_ram_temp=ptable_ram;
	ret=get_ptable_from_ram(ptable_ram);
	if(ret<0)
	{
		Dbug("Error:get ptable from ram failed\n");
		goto error;
	}

	while(strcmp(ptable_ram->name,PTABLE_END_STR)&&strcmp(ptable_flash->name,PTABLE_END_STR))
	{
		if(ptable_ram->image==ptable_flash->image)
			ptable_flash->count=ptable_ram->count;
		
		else{
			Dbug("Error:ram ptable is different with flash ptable!\n");
			goto error;
		}
		if(ptable_flash->image==IMAGE_PTABLE)
			ptable_flash->count=ptable_flash->count+1;
		ptable_ram++;
		ptable_flash++;
	}

	/*�����º��bufд��flash*/
	ret=write_ptable_to_flash(ptable_flash_all,PTABLE_SIZE);
	if(ret<0){
		Dbug("Error:update ptable failed!\n");
		goto error;
	}
	
	/*�ͷ�����Ŀռ�*/
	free(ptable_ram_temp);
	free(ptable_flash_all);
	return OK;

error:
	free(ptable_ram_temp);
	free(ptable_flash_all);
	return ERROR;
}

/*****************************************************************************
* �� �� ��    : update_recovery_count
*
* ��������  : ����recovery����֮���޸ķ������е�recovery ������count ��ֵ 
*
* �������  : ��			
			
* �������  : ��
*
* �� �� ֵ    : 
			  0:  	�����ɹ�
			-1:	����ʧ��
* ����˵��  : 
*
*****************************************************************************/
int update_recovery_count(void)
{
	struct ST_PART_TBL *ptable,*ptable_temp;
	char* data;
	ssize_t size=PTABLE_SIZE;
	data=(char *)malloc(PTABLE_SIZE);
	int count_i=0;		/*����ptable_temp �ļ���*/
	int count_a=-1;		/*����recovery-A��countֵ*/
	int count_b=-1;		/*����recovery-B��countֵ*/
	int a=0;			/*����recovery-Aָ��λ�õı���*/
	int b=0;			/*����recovery-Bָ��λ�õı���*/
	int ret;
	MtdReadContext *read;
	MtdWriteContext *write;

	if (!mtd_partitions_scanned) {
                mtd_scan_partitions();
                mtd_partitions_scanned = 1;
          }

	/*��ȡflash�еķ���������ͷ����Ϣ*/
	ret=get_ptable_from_flash(data);
	if(ret<0)
	{
		Dbug("Error:get ptable from flash failed\n");
		goto error;
	}
	ptable_temp=(struct ST_PART_TBL *)(data+PTABLE_HEAD_SIZE);
	ptable=ptable_temp;

	/*�޸ķ������ж�Ӧ��recovery������countֵ*/
	while(0 != strcmp(PTABLE_END_STR, ptable_temp->name))
	{
		if(!strcmp(ptable_temp->name,RECOVERY_ANAME)){
			count_a=ptable_temp->count;
			a=count_i;
		}
		if(!strcmp(ptable_temp->name,RECOVERY_BNAME)){
			count_b=ptable_temp->count;
			b=count_i;
		}
		count_i++;
		ptable_temp++;
	}

	/* �ж��Ƿ��ȡ��count��ֵ */
	if(count_a<0||count_b<0)
	{
		Dbug("Error:modify recovery count fail!\n");
		goto error;
	}
	
	/*recovery ˫����:
	  *��ʼ��recovery������countֵ��Ϊ0����ʱ��A����������count+1;
	  *�ǵ�һ������������countֵС�������������ɹ�����count+2			*/
	if(count_a<count_b)
	{
		ptable[a].count=ptable[a].count+2;
	}
	else if(count_a>count_b)
	{
 		ptable[b].count=ptable[b].count+2;

	}
	else
	{
		ptable[a].count=ptable[a].count+1;

	}

	/*�����º��bufд��flash*/
	ret=write_ptable_to_flash(data,PTABLE_SIZE);
	if(ret<0)
	{
		Dbug("Error:write ptable to flash failed!\n");
		goto error;
	}

	free(data);
	return OK;
error:
	
	free(data);
	return ERROR;

}
/*****************************************************************************
* �� �� ��    : ptable_find_older_recovery
*
* ��������  : ����recovery����ʱ�������������е�recovery ������count ��ֵ�ж������ĸ�����
*
* �������  : ��			
			
* �������  : ��
*
* �� �� ֵ    : 
			  name:  	��������recovery����������
			  NULL:	��ȡʧ��
* ����˵��  : 
*
*****************************************************************************/
char* ptable_find_older_recovery(void)
{
	int recovery_acount=-1;
	int recovery_bcount=-1;
	int i,ret;
	char *name;
	struct ST_PART_TBL *ptable=(struct ST_PART_TBL *)malloc(PTABLE_SIZE);
	struct ST_PART_TBL *ptable_temp;
	
	if(!ptable)
	{
		Dbug("Error:can't malloc for ptable_product!\n");
		return NULL;
	}

	/*��ȡram�еķ�����������ͷ����Ϣ*/
	ptable_temp=ptable;
	ret=get_ptable_from_ram(ptable);
	if(ret<0)
	{
		Dbug("Error:get ptable from ram failed\n");
		goto error;
	}

	/*��ȡ��recovery�����е�countֵ*/
	while(NULL!=ptable&&strcmp(ptable->name,PTABLE_END_STR))
	{
		if(!strcmp(ptable->name,RECOVERY_ANAME)){
				recovery_acount=ptable->count;
		}
		else if(!strcmp(ptable->name,RECOVERY_BNAME)){
				recovery_bcount=ptable->count;

		}
		else{
			;
		}
		ptable++;
	}
	/*�ж�recovery count�ĺϷ���*/	
	if(recovery_acount<0||recovery_bcount<0)
	{
		Dbug("Error:find recovery count fail!recoverya_count: %x,recoveryb_count:%x\n",recovery_acount,recovery_bcount);
		goto error;
	}

	/*recovery��������ʱ����recovery������countֵ��С���Ǹ���������(������countС������Ϊ�ɷ���)*/
	name=recovery_acount>recovery_bcount?RECOVERY_BNAME:RECOVERY_ANAME;
	free(ptable_temp);
	return name;

error:
	free(ptable_temp);
	return NULL;
}

/*****************************************************************************
* �� �� ��    : ptable_find_name_by_type
*
* ��������  : ����ʱ�������������е�imagetype���ҵ����������֣��Բ���mtd����
*
* �������  : 
			typeid:��Ҫ�����ľ����imageID 			
			
* �������  : ��
*
* �� �� ֵ    : 
			  name:  	�����ķ���������
			  NULL:	��ȡʧ��
* ����˵��  : 
*
*****************************************************************************/

char* ptable_find_name_by_type(unsigned int type_id)
{
	int i,ret;
	int fd;
	char  *ptable_product=(char *)malloc(PTABLE_SIZE);
	char *ptable_product_temp=ptable_product;
	char  ptable_name[50];
	char *recovery_name;
	
	if(!ptable_product)
	{
		Dbug("Error:can't malloc for ptable_product!\n");
		free(ptable_product);
		return NULL;
	}
	
	if(type_id==IMAGE_RECOVERY_A)
	{
		recovery_name=NULL;
		ptable_name[0]='\0';
		recovery_name=ptable_find_older_recovery();
		if(NULL==recovery_name){
			Dbug("find older recovery failed!\n");

		}
		return recovery_name;
	}
	ret=	dload_get_proc_ptable(ptable_product, PTABLE_SIZE);
	if(ret<0)
	{
		Dbug("Error:can't get ptable proc file!\n");
		return NULL;
	}
	
	i=0;
	ptable_product=ptable_product+PTABLE_HEAD_SIZE;
	while(ret>0)
	{
		int imagetype;
		char *name;
		int matches;
		
		ptable_name[0]='\0';
		imagetype=-1;


		matches=sscanf(ptable_product,"%x %*x %*x %*x %*x %*x %*x %*x \"%49[^\"]",&imagetype,ptable_name);

		if(matches==2){
			name=strdup(ptable_name);
			
			if(type_id==imagetype){
				free(ptable_product_temp);
				return ptable_name;
			}
		}
		while(ret>0&&*ptable_product!='\n'){
			ptable_product++;
			ret--;
		}
		if(ret>0)
		{
			ptable_product++;
			ret--;
		}
	}
	
	free(ptable_product_temp);
	return NULL;
}


/*****************************************************************************
* �� �� ��    : ptable_dload_write_fsimg
*
* ��������  : �ļ�ϵͳ������д
*
* �������  : 
			data_buf  :	��Ҫ��д�ľ���ĵ�ַ
			image_idx:	��������
			this_offset:	������д��������buf��ַ�е�ƫ��
			len           :	Ҫ��д�����ݳ���
			
* �������  : ��
*
* �� �� ֵ    : 
			   0:  	д��ɹ�
			  -1:		д��ʧ��
* ����˵��  : 
*
*****************************************************************************/
unsigned int ptable_dload_write_fsimg(unsigned int data_buf, unsigned int image_idx, unsigned int this_offset , unsigned int len)
{
	char* partition_name=NULL;
	char mtd_dev_name[32];
	int fd;
	int i,ret;
	unsigned int page_size=0;
	struct mtd_write_req write_req;
	memset(&write_req,0,sizeof(write_req));
	write_req.mode=MTD_OPS_AUTO_OOB;
	write_req.ooblen=OOB_SIZE;
	
	off_t pos=this_offset;
	
	if (!mtd_partitions_scanned) {
                mtd_scan_partitions();
                mtd_partitions_scanned = 1;
          }

	/*��ȡ����������*/
	partition_name=ptable_find_name_by_type(image_idx);
	if(NULL==partition_name){
		Dbug("Error:can't find partition name!\n");
		return ERROR;
	}

	 /*���ݷ������ƻ�ȡ����*/
	const MtdPartition* mtd=mtd_find_partition_by_name(partition_name);
	if (mtd == NULL) {
                Dbug("mtd partition \"%s\" not found for writing\n", partition_name);
                return ERROR;
         }


	/*�򿪶�Ӧ��mtd�豸*/
	Dbug("mtd->device_index:%d\n",mtd->device_index);
	sprintf(mtd_dev_name,"/dev/mtd/mtd%d",mtd->device_index);
	fd=open(mtd_dev_name,O_RDWR);
	if(fd<0){
		return ERROR;
	}

	/*��ȡ��nand flash��ҳ��С*/
	page_size=get_nand_page_size(fd);
	if(page_size<0){
		Dbug("Error:nand page size is wrong!\n");
		printf("Error:nand page size is wrong!\n");
		return ERROR;
	}

	/*��ÿ��дһҳ�ķ�ʽ��flash��д���ļ�ϵͳ�ľ���*/
	i=0;
	while(0<(this_offset+len-pos-i*OOB_SIZE)){
		write_req.usr_oob=NULL;
		write_req.usr_data=(char *)data_buf+i*(page_size+OOB_SIZE);
		write_req.len=page_size;
		write_req.start=pos;
		
		/*д������*/
		ret=ioctl(fd, MEMWRITE, &write_req);
		if ( ret < 0) {
			Dbug("system image:write data error no is%d \n",ret);
			return ERROR;
		}

		/*д��OOB*/
		write_req.usr_oob=(char *)data_buf+i*(page_size+OOB_SIZE)+page_size;
		write_req.usr_data=NULL;
		write_req.len=OOB_SIZE;
		write_req.start=pos;
		ret=ioctl(fd, MEMWRITE, &write_req) ;
		if (ret< 0) {
			Dbug("system image:write oob error no is %d \n",ret);
			return ERROR;
		}
		pos+=page_size;
		i++;
	}

	return OK;
	
	
}


/*****************************************************************************
* �� �� ��    : ptable_dload_write
*
* ��������  : ���ļ�ϵͳ������д
*
* �������  : 
			data_buf  :	��Ҫ��д�ľ���ĵ�ַ
			image_idx:	��������
			this_offset:	������д��������buf��ַ�е�ƫ��
			len           :	Ҫ��д�����ݳ���
			
* �������  : ��
*
* �� �� ֵ    : 
			   0:  	д��ɹ�
			  -1:		д��ʧ��
* ����˵��  : 
*
*****************************************************************************/

unsigned int ptable_dload_write(unsigned int data_buf, unsigned int image_idx, unsigned int this_offset , unsigned int len)
{
	char* name=NULL;
	char older_recovery;
	
	if (!mtd_partitions_scanned) {
                mtd_scan_partitions();
                mtd_partitions_scanned = 1;
          }

	/*���ݾ����id��������*/
	name=ptable_find_name_by_type(image_idx);

	/*���Ҷ�Ӧ����*/
	const MtdPartition* mtd=mtd_find_partition_by_name(name);
	if (mtd == NULL) {
                Dbug("mtd partition \"%s\" not found for writing\n", name);
                goto error;
         }

	/*��write��ʽ�򿪷���*/
	 MtdWriteContext* ctx = mtd_write_partition(mtd);
	 if (ctx == NULL) {
             Dbug("failed to init mtd partition \"%s\" for writing\n", name);
                goto error;
          }
	if (lseek(ctx->fd, this_offset, 0) != this_offset ) {
                goto error;
     	}
	/*�������д������*/
	size_t written = mtd_write_data(ctx, (char*)data_buf, len);
	if (written != len) {
		Dbug("only wrote %d of %d bytes to MTD %s\n",written, len, name);
                mtd_write_close(ctx);
                goto error;
	}
	if (mtd_erase_blocks(ctx, -1) < 0) {
                Dbug("error finishing mtd write of %s\n", name);
                mtd_write_close(ctx);
		goto error;
	}

	if (mtd_write_close(ctx)) {
                Dbug("error closing mtd write of %s\n", name);
                goto error;
	}
	return OK;
error:
	
	return ERROR;
}
//end for ������׮


/*****************************************************************************
* �� �� ��  : BSP_DLOAD_SetCdromMarker
*
* ��������  : �����豸��̬��־
*
* �������  : BSP_BOOL bCdromMarker  :
*             BSP_TRUE  :���������豸��̬
*             BSP_FALSE :����bootrom�豸��̬
* �������  : ��
*
* �� �� ֵ  : ��
*
* ����˵��  : ��������̨�ļ�ʱ������Ϊ�ϱ������豸��̬�����ָ�NV��
*             ������ǰ̨�ļ�ʱ������Ϊ�ϱ�bootrom�豸��̬���ָ�NV��
*
*****************************************************************************/
void dload_SetCdromMarker(bool bCdromMarker)
{
	/*TODO:*/
	//(void)setCdromFlag(bCdromMarker);//��ʱ��׮
}

/*****************************************************************************
* �� �� ��    : dload_align_buffer_write
*
* ��������  :  �����յ��ľ����ļ�ϵͳ�ͷ��ļ�ϵͳ�������֣������Է�ʽд��flash
*
* �������  : ��
* �������  : 
*
* �� �� ֵ    : 
			  0:�ɹ�д��
			-1:д��ʧ��
*
* ����˵��  : 
*
*****************************************************************************/

int dload_align_buffer_write(struct dload_buf_ctrl *write_buf)
{

	int ret=(int)ERROR;
	int data_type;

	if(( dcb->image_type > IMAGE_PART_START ) && ( dcb->image_type < IMAGE_PART_TOP ))
	{
        	/*������ַ��Χ*/        
        	#if defined (DLOAD_DEBUG_FLASH)
        	Dbug("dloadAlignBufferWrite:addr=0x%x,len=0x%x\n",
        	dcb->flash_current_offset, write_buf->used);
        	#endif
			
		data_type=get_image_property(dcb->image_type);
		if(data_type<0)
		{
			Dbug("Error:get image property fail!\n");
			return ERROR;
		}
		/*���ļ�ϵͳ�ľ���*/
		if(DATA_YAFFS!=(data_type&PTABLE_IMAGE_TYPE_MASK))
		{
			ret = ptable_dload_write((unsigned int)write_buf->data,
	                            dcb->image_type,
	                            dcb->flash_current_offset,
	                            write_buf->used);
			Dbug("\ng_stDloadWriteBuffer.data:0x%x\nimage type:0x%x\ncurrent_loaded_length:%d;\ng_stDloadWriteBuffer.used:0x%x\n",write_buf->data,dcb->image_type,dcb->flash_current_offset,write_buf->used);
			printf("\ng_stDloadWriteBuffer.data:0x%x\nimage type:0x%x\ncurrent_loaded_length:%d;\ng_stDloadWriteBuffer.used:0x%x\n",write_buf->data,dcb->image_type,dcb->flash_current_offset,write_buf->used);

			dcb->flash_current_offset += write_buf->used;
		}
		else
		{/*�ļ�ϵͳ�ľ���*/
			ret = ptable_dload_write_fsimg((unsigned int)write_buf->data,
	                            dcb->image_type,
	                            dcb->flash_current_offset,
	                            write_buf->used);
			Dbug("\ng_stDloadWriteBuffer.data:0x%x\nimage type:0x%x\ncurrent_loaded_length:%d;\ng_stDloadWriteBuffer.used:0x%x\n",write_buf->data,dcb->image_type,dcb->flash_current_offset,write_buf->used);
			dcb->flash_current_offset+=write_buf->used-(DLOAD_NAND_SPARE_ALIGN_ADD-DLOAD_NAND_SPARE_ALIGN);
		}
        	return ret;   
    	}
    	else
    	{
    		Dbug("Error:image id is invalid!\n");
        	return ERROR; 
    	}

    return ret;
}

/*****************************************************************************
* �� �� ��    : image_dload_to_flash
*
* ��������  :  �����յ������ݷ�����flash��д�������buf�У�������д�뺯��
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ    : 
			    0:�����ɹ�
			  -1:����ʧ��
* 
* ����˵��  : 
*
*****************************************************************************/
int image_dload_to_flash(struct dload_buf_ctrl **write_buf,unsigned int align_size)
{
	unsigned int ulIndex = 0;    /*��ǰ���������g_pDloadWriteBuffer������ƫ����*/      
	unsigned int remainLen = 0;  /*��ǵ�ǰ���ݰ�ʣ���ֽ���*/
	int ret;
	/*��ڲ������*/
	if( NULL == (*write_buf)->data)
	{
		Dbug("ImageDloadToFlash: NULL == g_stDloadWriteBuffer data error.\n");
		return ERROR;
	}

	/*��ǰ���İ�������g_pDloadWriteBufferʣ��Ŀ��ÿռ�*/
    	if( dcb->current_loaded_length >= (align_size - (*write_buf)->used) )
    	{
        	/*��g_pDloadWriteBufferʣ��Ŀ��ÿռ�����*/
        	ulIndex = align_size - (*write_buf)->used;
        
        	memcpy((unsigned char*)((unsigned char*)(*write_buf)->data + (*write_buf)->used),  (unsigned char*)dcb->ram_addr, ulIndex);
        	(*write_buf)->used = align_size;
		ret=dload_align_buffer_write(*write_buf);
        	if( OK != ret)      /*��g_pDloadWriteBuffer������д��flash*/
        	{
            		Dbug("ImageDloadToFlash: dloadAlignBufferWrite error.1\n");
            		return ERROR;
        	}
        
        	(*write_buf)->used = 0;    /*��g_stDloadWriteBuffer���*/

        	/* �����������Ҫ����,����д�뻺���� */
        	remainLen = dcb->current_loaded_length - ulIndex;
        	if (0 != remainLen)
        	{
            		memcpy((unsigned char *)((*write_buf)->data + (*write_buf)->used), (unsigned char *)((unsigned char *)dcb->ram_addr + ulIndex),  remainLen);
           		 (*write_buf)->used += remainLen; 
       		 }
	}
	else
   	{
    	}

    	/*��ʣ�����������g_pDloadWriteBuffer*/
    	memcpy((unsigned char*)((unsigned char*)(*write_buf)->data + (*write_buf)->used), 
                  (unsigned char *)((unsigned char*)dcb->ram_addr + ulIndex), dcb->current_loaded_length - ulIndex);
    	(*write_buf)->used += dcb->current_loaded_length - ulIndex;

    	dcb->last_loaded_total_length += dcb->current_loaded_length;

    	#if defined (DLOAD_DEBUG_FLASH)
	Dbug("unsec:F b/e/c:0x%x,0x%x,0x%x,0x%x\n",dcb->flash_current_offset,
    	dcb->current_loaded_length,dcb->last_loaded_total_length,dcb->last_loaded_packet);
    	#endif

    	/*��ǰ��Ϊ�������һ��������*/
    	if( dcb->image_size == dcb->last_loaded_total_length )    
    	{
    		ret=dload_align_buffer_write(*write_buf);
        	if(OK != ret)
        	{
        		Dbug("the last dload packge fail,ret is %d",ret);
            		return ERROR;
        	}        

        	/* �ڲ������һ��������������ɺ󣬼�¼��ǰ��¼��������������,����DATA_ENDʱ
        	  * �ж��Ƿ�ΪCDROMISO/WEBUI/RECOVERY����
        	 */
        	dcb->finish_image_type= dcb->image_type;		

        	/*��ӳ��������д��ϣ���ȫ�ֱ�����λ*/
        	Dbug("sec: the image 0x%x dload success!\n", dcb->image_type);

		/*���g_stDloadWriteBuffer*/ 
        	(*write_buf)->used = 0;    
   	 }    

	return OK;

}

/*****************************************************************************
* �� �� ��    : image_dload_process
*
* ��������  :  ����������д
*
* �������  : ��
* �������  : 
*
* �� �� ֵ    : 
			  0:�����ɹ�
			-1:����ʧ��
*
* ����˵��  : 
*
*****************************************************************************/
int image_dload_process(struct dload_buf_ctrl *write_buf,unsigned int align_size)
{
	/*��ڲ������*/
	if((NULL == dcb) || (dcb->current_loaded_length > align_size) || (0 == dcb->current_loaded_length) )
	{
		Dbug("Error: g_pDCBStr->current_loaded_length=0x%x align_size=0x%x.\n", (int)dcb->current_loaded_length,align_size);
		return (int)ERROR;
    	}    

	/*��ǰ��Ϊ������һ������������ȡ��ǰ������д��Ŀ�ĵ�ַ*/
	if( 0 == dcb->last_loaded_total_length )
	{
		Dbug("imageDloadProcess: g_pDCBStr->image_type = 0x%x.\n",dcb->image_type);
        
        	/* ����д�������ַ��0 */
        	dcb->flash_current_offset = 0;

		if( 0 != write_buf->used )
        	{
			Dbug("imageDloadProcess: g_stDloadWriteBuffer.used=0x%x error.\n", (int)write_buf->used);
			return (int)ERROR;
        	}
    	}

	/*��flash��д��������*/
	if(OK != image_dload_to_flash(&write_buf,align_size))
        {
        	Dbug("dload image to flash failed!\n");
		return (int)ERROR;
        }

	return OK;

}

/*****************************************************************************
* �� �� ��  : dload_buf_init
*
* ��������  : �������ػ������ռ�
*
* �������  : 
			type: ����id
			len  : �����buf����
* �������  : 
*
* �� �� ֵ  : 
*			  0:�����ɹ�
			-1:����ʧ��

* ����˵��  : 
*
*****************************************************************************/
int dload_buf_init(enum IMAGE_TYPE type, unsigned int len,struct dload_buf_ctrl *write_buf)
{
	
	 /*�ͷ���Ҫ�����ָ��*/
	if(write_buf->data)
	{
		free(write_buf->data);    
		write_buf->data = NULL;
	}

	if( NULL == write_buf->data)
	{        
		if( NULL == (write_buf->data = (unsigned char *)malloc((unsigned int)len)) )
                {
                    Dbug("dloadInit: g_stDloadWriteBuffer data OSAL_Malloc error.\n");
                    return (unsigned int) ERROR;
                }
            }

            write_buf->size   = len;
            write_buf->used   = 0;
                
            memset((void*)write_buf->data, 0x0, len);
           
	return OK;
}

/*****************************************************************************
* �� �� ��    : dloadNeedMidwayRestart
*
* ��������  : TODO
*
* �������  : TODO
			
* �������  : TODO
*
* �� �� ֵ    : 

* ����˵��  : 
*
*****************************************************************************/

int dloadNeedMidwayRestart(void)
{

	//TODO:
	return 0;
}

/******************************************************************************
*  Function:  dloadGetNvImeiNumber
*  Description:
*      ��ȡIMEI
*
*  Calls:
*
*  Called By:
*
*  Data Accessed: 
*
*  Data Updated: 
*
*  Input:
*        aucIMEI: ���IMEI
*
*  Output:
*         NONE
*
*  Return:
*         0: �����ɹ�����
*         -1: ����ʧ��
*
********************************************************************************/
int dloadGetNvImeiNumber(unsigned char aucIMEI[])
{
    /*TODO: */
    return OK;
}

/*****************************************************************************
 Function   :dloadIsDefaultImeiValue
 Description: �ж��Ƿ���Ĭ��IMEI
 Input      : 
 Return     :
 History    :
*****************************************************************************/
int dloadIsDefaultImeiValue(unsigned char aucIMEI[])
{
    unsigned char  aucDefaultIMEI[IMEI_LEN]  = {0x0};
    unsigned char  ucIndex;


    for(ucIndex = 0; ucIndex < IMEI_LEN; ucIndex++)
    {
        if (aucIMEI[ucIndex] != aucDefaultIMEI[ucIndex])
        {
            return ERROR;
        }
    }

    return OK;

}


/******************************************************************************
*  Function:  getAuthorityId
*  Description:
*      ��ȡ���ؼ�ȨЭ��Id
*
*  Calls:
*
*  Called By:
*
*  Data Accessed: 
*
*  Data Updated: 
*
*  Input:
*        buf: ������ؼ�ȨЭ��Id�Ļ���
*        len: ���泤��
*
*  Output:
*         NONE
*
*  Return:
*         0: �����ɹ�����
*         1: ����ʧ��
*
********************************************************************************/
int get_authorityId(unsigned char buf[], int len)
{
	unsigned char  aucBuffer[IMEI_LEN] = {0x0};
	unsigned char  ucIndex;

	Dbug("getAuthorityId\n");

	if((NULL == buf) || (len <= 0) || (len < (IMEI_LEN + 3)))
	{
		Dbug("param error!\n");
	        return ERROR;
	}

	memset(buf, 0x0, (unsigned int)len);

	if (ERROR == dloadGetNvImeiNumber(aucBuffer))
	{
	        return ERROR;
	}

	 /* ����IMEI��ΪĬ��NV��,���ʾ��NV��δ����*/
	if (OK == dloadIsDefaultImeiValue(aucBuffer))
	{
	        return ERROR;
	}    

	for(ucIndex = 0; ucIndex < (IMEI_LEN -1); ucIndex++)//ʵ��IMEI��Ϊ15λ
	{
	        if(((unsigned char)aucBuffer[ucIndex] <= 0x09))
	        {
	            buf[ucIndex] = aucBuffer[ucIndex] + '0';
	        }
	        else
	        {
	            return ERROR;
	        }
	}

	buf[ucIndex] = ',';
	buf[ucIndex + 1] = '0';   

	return OK;
}
