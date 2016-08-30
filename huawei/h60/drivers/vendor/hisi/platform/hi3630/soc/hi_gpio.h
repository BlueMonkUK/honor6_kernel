/******************************************************************************/
/*  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : hi_gpio.h */
/* Version       : 2.0 */
/* Created       : 2013-02-18*/
/* Last Modified : */
/* Description   :  The C union definition file for the module gpio*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Modification  : Create file */
/******************************************************************************/

#ifndef __HI_GPIO_H__
#define __HI_GPIO_H__



#define HI_K3_GPIO

/********************************************************************************/
/*    gpio �Ĵ���ƫ�ƶ��壨��Ŀ��_ģ����_�Ĵ�����_OFFSET)        */
/********************************************************************************/
#define    HI_GPIO_SWPORT_DR_OFFSET                          (0x0) /* GPIO������ݼĴ����� */
#define    HI_GPIO_SWPORT_DDR_OFFSET                         (0x4) /* GPIO���ݷ�����ƼĴ����� */
#define    HI_GPIO_INTEN_OFFSET                              (0x30) /* GPIO�ж�ʹ�ܼĴ����� */
#define    HI_GPIO_INTMASK_OFFSET                            (0x34) /* GPIO�ж����μĴ����� */
#define    HI_GPIO_INTTYPE_LEVEL_OFFSET                      (0x38) /* GPIO�жϴ������ͼĴ����� */
#define    HI_GPIO_INT_PLOARITY_OFFSET                       (0x3C) /* GPIO�жϼ��ԼĴ����� */
#define    HI_GPIO_INTSTATUS_OFFSET                          (0x40) /* GPIO�ж�״̬�Ĵ����� */
#define    HI_GPIO_RAWINTSTATUS_OFFSET                       (0x44) /* GPIOԭʼ�ж�״̬�Ĵ����� */
#define    HI_GPIO_PORT_EOI_OFFSET                           (0x4C) /* GPIO�ж�����Ĵ����� */
#define    HI_GPIO_EXT_PORT_OFFSET                           (0x50) /* GPIO�������ݼĴ����� */

#endif // __HI_GPIO_H__

