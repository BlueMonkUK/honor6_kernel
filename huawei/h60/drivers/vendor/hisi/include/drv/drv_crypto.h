/*************************************************************************
*   ��Ȩ����(C) 1987-2011, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  DrvInterface.h
*
*   ��    �� :  wangxiandong
*
*   ��    �� :  ���ļ�����Ϊ"DrvInterface.h", ����V7R1�����Э��ջ֮���API�ӿ�ͳ��
*
*   �޸ļ�¼ :  2013��3��12��  v1.00  wangxiandong�޸�
*************************************************************************/

#ifndef __DRV_CYPTO_H__
#define __DRV_CYPTO_H__

#include "drv_comm.h"

/*for create_crypto_key,hash algorithm enum*/
typedef enum 
{
    CREATE_CRYPTO_KEY_ALGORITHM_MD5 = 0x0,
    CREATE_CRYPTO_KEY_ALGORITHM_SHA1,
    CREATE_CRYPTO_KEY_ALGORITHM_SHA256,
    CREATE_CRYPTO_KEY_ALGORITHM_MAX
}CREATE_CRYPTO_KEY_ALGORITHM;

/*for crypto_hash,hash algorithm enum*/
typedef enum 
{
    CRYPTO_ALGORITHM_MD5 = 0x0,
    CRYPTO_ALGORITHM_SHA1,
    CRYPTO_ALGORITHM_SHA256,
    CRYPTO_ALGORITHM_MAX
}CRYPTO_HASH_ALGORITHM;

/*for crypto_encrypt,aes algorithm enum*/
typedef enum 
{
    CRYPTO_ENCRYPT_ALGORITHM_AES_ECB = 0x0,
    CRYPTO_ENCRYPT_ALGORITHM_MAX
}CRYPTO_ENCRYPT_ALGORITHM;



/*****************************************************************************
* �� �� ��  : create_crypto_key
*
* ��������  : ʹ����������ݺ�HUK��������Կ��
*��ǰ֧��MD5����SHA-1�㷨��������Կ�ķ�������HUK������
*����������������ΪMD5��SHA-1�㷨�����룬������HASHֵ
*
* �������  : data������������������������Կ�����ݡ�
*                           len���������������������ݳ���(�ֽ�)
*                           algorithm��������������ڲ�����Կ���㷨��
*                           key�����������������ɵ���Կ��
*            ע�⣺�䳤�ȱ��벻С��16�ֽڡ���Ϊ��ԿΪ16�ֽڡ�
*                           klen�����������������Ϊ������������key��
*            �������ĳ��ȡ���Ϊ���������������ɵ���Կ�ĳ��ȡ�(�ֽ�)
* �������  : 
*
* �� �� ֵ  :  BSP_OK--���ܳɹ�;BSP_ERROR--����ʧ��
*
* ����˵��  :�ڴ��ɵ���������
*
*****************************************************************************/
int create_crypto_key(char *data, int len, CREATE_CRYPTO_KEY_ALGORITHM algorithm, char *key, int *klen);
#define CREATE_CRYPTO_KEY(data,len,algorithm,key,klen)  create_crypto_key(data,len,algorithm,key,klen)

/*****************************************************************************
* �� �� ��  : crypto_hash
*
* ��������  : ������������ݵ�HASHֵ��
*                           ��ǰ֧��MD5����SHA-1�㷨�����HASHֵ������16�ֽ�
*
* �������  : data��������������������Ҫ����HASHֵ�����ݡ�
*                           len���������������������ݳ���(�ֽ�)
*                           algorithm�����������HASH�㷨��
*                           hash�����������������ɵ�HASHֵ��
*                           hlen�����������������Ϊ������������HASHֵ�Ļ������ĳ��ȡ�(�ֽ�)
*                           ��Ϊ���������������ɵ�HASHֵ�ĳ��ȡ�
* �������  : 
*
* �� �� ֵ  : BSP_OK--���ܳɹ�;BSP_ERROR--����ʧ��
*
* ����˵��  : �ڴ��ɵ���������
*
*****************************************************************************/
int crypto_hash(char *data, int len, CRYPTO_HASH_ALGORITHM algorithm, char *hash, int *hlen);
#define CRYPTO_HASH(data,len,algorithm,hash,hlen)  crypto_hash(data,len,algorithm,hash,hlen)

/*****************************************************************************
* �� �� ��  : crypto_encrypt
*
* ��������  : ʹ��ָ������Կ��ָ�����㷨����������ݼ��ܣ�������ܺ�����ݡ�
                             ��ǰ֧��AES-ECB�㷨��
*
* �������  : data����������������Ҫ���ܵ����ݡ�
*                           len���������������������ݳ���(�ֽ�)
*                           algorithm�����������HASH�㷨��
*                           key�������������ż�����Կ��
*                           klen�����������key�ĳ��ȡ�(�ֽ�)
*                           cipher_data�������������ż��ܺ�����ݡ�
*                           cipher_len�����������������Ϊ���������������ĵĻ������ĳ��ȡ�(�ֽ�)
*               ��Ϊ���������������ɵ����ĵĳ��ȡ�
* �������  : 
*
* �� �� ֵ  :  BSP_OK--���ܳɹ�;BSP_ERROR--����ʧ��
*
* ����˵��  : �ڴ��ɵ���������
*
*****************************************************************************/
int crypto_encrypt (char *data, int len, CRYPTO_ENCRYPT_ALGORITHM algorithm, char *key, int klen, char *cipher_data, int *cipher_len);
#define CRYPTO_ENCRYPT(data,len,algorithm,key,klen,cipher_data,cipher_len)  \
crypto_encrypt(data,len,algorithm,key,klen,cipher_data,cipher_len)

/*****************************************************************************
* �� �� ��  : crypto_decrypt
*
* ��������  : ʹ��ָ������Կ��ָ�����㷨����������ݽ��ܣ�������ܺ�����ݡ�
*             ��ǰ֧��AES-ECB�㷨��
*
* �������  : 
*             cipher_data: ���ܵ����ݵĴ��buffer��
*             cipher_len:  �����ܵ����ݵ�ʵ�ʳ��ȡ�(byte)
*             algorithm:   ���ý����㷨����ֻ�ṩAES-ECB��
*             key:         ��Կbuffer��
*             klen:        ��Կbuffer���ȡ�(byte)
*             len:  ���ܺ�����ݵĴ��buffer��buffer size��(byte)(û�м��)
*
* �������  : 
*             data:        ���ܺ�����ݡ�
*             len:         ���ܺ�����ݳ��ȡ�(byte)
*
* �� �� ֵ  : BSP_OK:      ���ܳɹ���
*             BSP_ERROR:   ����ʧ�ܡ�
*
* ����˵��  : lenΪ����/��������������len���������ڴ�����д�ء�
*             ���Ա���ֱ�Ӵ�������sizeof()�ĺ������ý����
*
*****************************************************************************/
extern int crypto_decrypt (char *cipher_data,int cipher_len,CRYPTO_ENCRYPT_ALGORITHM algorithm, char *key, int klen, char *data, int *len);
#define CRYPTO_DECRYPT(cipher_data,cipher_len,algorithm, key, klen, data, len)  \
crypto_decrypt(cipher_data,cipher_len,algorithm, key, klen, data, len)


/*****************************************************************************
* �� �� ��  : crypto_rsa_encrypt
*
* ��������  : ʹ�ñ�����NV�еĸ�����RSA��Կ����ȡʱ��Ҫͬ������ǩ����֤��
*           ����������ݼ��ܣ�������ܺ�����ݡ�
* �������  : data����������������Ҫ���ܵ����ݡ�
*                           len���������������������ݳ���(�ֽ�)
*                           rsa_key:RSA��Կ
*                           rsa_len:RSA��Կ����(�ֽ�)
*                           cipher_data�������������ż��ܺ�����ݡ�
*                           cipher_len�����������������Ϊ���������������ĵĻ������ĳ��ȡ�(�ֽ�)
*               ��Ϊ���������������ɵ����ĵĳ��ȡ�
* �������  : 
*
* �� �� ֵ  :  BSP_OK--���ܳɹ�;BSP_ERROR--����ʧ��
*
* ����˵��  : �ڴ��ɵ���������
*
*****************************************************************************/
int crypto_rsa_encrypt (char *data, int len, char *rsa_key, int rsa_klen, char *cipher_data, int *cipher_len);
#define CRYPTO_RSA_ENCRYT(data,len,rsa_key,rsa_klen,cipher_data,cihper_len) \
crypto_rsa_encrypt(data,len,rsa_key,rsa_klen,cipher_data,cihper_len)

/*****************************************************************************
* �� �� ��  : crypto_rsa_decrypt
*
* ��������  : ʹ�ñ�����NV�еĸ�����RSA��Կ������������ݽ���
*               ������ܺ�����ݡ�
* �������  : cipher_data�������������ż������ݡ�
*                           cipher_len�����������������ĵĻ������ĳ��ȡ�(�ֽ�)
*                           rsa_key:RSA��Կ
*                           rsa_len:RSA��Կ����(�ֽ�)
*                           data������������������ܺ�����ݡ�
*                           len�����������������Ϊ�����������Ž��ܺ�Ļ������ĳ���(�ֽ�)
*               ��Ϊ���������������ɵ����ĵĳ���
* �������  : 
*
* �� �� ֵ  :  BSP_OK--���ܳɹ�;BSP_ERROR--����ʧ��
*
* ����˵��  : �ڴ��ɵ���������
*
*****************************************************************************/
int crypto_rsa_decrypt (char *cipher_data, int cipher_len, char *rsa_key, int rsa_klen, char *data, int *len);
#define CRYPTO_RSA_DECRYPT(cipher_data,cihper_len,rsa_key,rsa_klen,data,len) \
crypto_rsa_decrypt(cipher_data,cihper_len,rsa_key,rsa_klen,data,len)

/*****************************************************************************
* �� �� ��  : crypto_rand
*
* ��������  : ��������ɽӿ�
* �������  : rand_data:��������buffer
*                           len:�����õ���������ֽ���
*
* �������  : 
*
* �� �� ֵ  :  BSP_OK--��ȡ������ɹ�;BSP_ERROR--��ȡʧ��
*
* ����˵��  : �ڴ��ɵ���������
*
*****************************************************************************/
int crypto_rand (char *rand_data, int len);
#define CRYPTO_RAND(rand_data,len)  crypto_rand(rand_data,len)


void _Clarinet_MD5Init(void *mdContext);
void _Clarinet_MD5Update(void *mdContext, unsigned char *inBuf, unsigned int inLen);
void _Clarinet_MD5Final(unsigned char *hash, void *mdContext);

#endif

