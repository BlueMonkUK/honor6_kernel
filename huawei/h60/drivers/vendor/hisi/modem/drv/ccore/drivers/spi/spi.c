
#include <bsp_spi.h>
#include <spi_balong.h>

/*��ģ��꿪������Ϊʵ��ʵ�֣�����Ϊ��׮�ӿ�*/
#ifdef CONFIG_SPI
void bsp_spi_init(void)
{
	balong_spi_init();
}
s32 spi_recv (u32 spiNo, u32 cs, u16* prevData, u32 recvSize,u16* psendData,u32 sendSize )
{
	return balong_spi_recv(spiNo, cs, prevData, recvSize, psendData, sendSize);
}
s32 spi_send (u32 spiNo, u32 cs, u16* pData, u32 ulLen)
{
	return balong_spi_send(spiNo, cs, pData, ulLen);
}
#else
void bsp_spi_init(void)
{
	return;
}
s32 spi_recv (u32 spiNo, u32 cs, u16* prevData, u32 recvSize,u16* psendData,u32 sendSize )
{
	return OK;
}
s32 spi_send (u32 spiNo, u32 cs, u16* pData, u32 ulLen)
{
	return OK;
}
#endif

