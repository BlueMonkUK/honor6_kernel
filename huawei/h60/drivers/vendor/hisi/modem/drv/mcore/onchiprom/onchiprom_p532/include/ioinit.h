#include "sys.h"
#include "config.h"
#include "ios_ao_drv_macro.h"
#include "ios_pd_drv_macro.h"


/* NFIO�������� */
#define NF_IOS_SYS_CONFIG() \
{\
}

#if PLATFORM==PLATFORM_PORTING
/* SPI0 �� Porting��SPI0��������,ƬѡΪCS1 */
#define SPI_IOS_SYS_CONFIG() \
{\
}
#else
/* SPI1 �� ASIC��SPI1�������ã�ƬѡΪCS1 */
#define SPI_IOS_SYS_CONFIG() \
{\
}
#endif

/* EMMC�ܹܽ����� */
#if PLATFORM==PLATFORM_PORTING
#define EMMC_IOS_SYS_CONFIG() \
{\
}
#else
/* ASICʹ��MMC0 */
#define EMMC_IOS_SYS_CONFIG() \
{\
}
#endif

/* HsUart�ܽ����� */
#define HSUART_IOS_SYS_CONFIG() \
{\
}

