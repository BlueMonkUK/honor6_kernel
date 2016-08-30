#ifndef _HISILICON_UDP_BOARD_ID_H_
#define _HISILICON_UDP_BOARD_ID_H_

#define 	UNKNOWN_LCD	0
#define	TOSHIBA_MDY90_LCD	1
#define	JDI_OTM1902B_LCD	2
#define JDI_NT35695_LCD		3
#define JDI_NT35695B_LCD		4

/******************************************************************************
Function:	    read_lcd_type
Description:	    ��ȡMMU�۰��Ӧ��LCD����
Input:		    none
Output:		    none
Return:		    01: TOSHIBA_MDY90_LCD  02: JDI_OTM1902B_LCD ����: ERROR
******************************************************************************/
extern int read_lcd_type(void);
extern int read_tp_color(void);

#endif
