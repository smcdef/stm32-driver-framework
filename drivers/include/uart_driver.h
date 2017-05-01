/*
*****************************************************************************************************************
*                                           MuChun Electron
*                                       ZhenJiang,Jiangsu,China
*                             (c) Copyright 2016,MuChun Electron,Zhenjiang,China
*                                         All Rights Reserved
*
*
*
*文 件 名 :uart_driver.h
*作    者 :SMC
*文件说明 :串口驱动代码，可跨平台移植
*****************************************************************************************************************
*/
/* $ PAGE*/
#ifndef __UART_DRIVER_H
#define __UART_DRIVER_H



#if !defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN)
	#if defined (STM32F10X_LD) || defined (STM32F10X_LD_VL) || defined (STM32F10X_MD) || defined (STM32F10X_MD_VL) || \
		defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_XL) || defined (STM32F10X_CL) 
		#define LITTLE_ENDIAN       /*芯片为小端模式，以支持匿名上位机*/
//		#warning "you are using the chip(eg:ARM) which is little endian"
	#else
		#define BIG_ENDIAN          /*芯片为大端模式，以支持匿名上位机*/
//		#warning "you are using the chip(eg:51,STM8) which is big endian"
	#endif
#endif

#if !defined (BIG_ENDIAN) && !defined (LITTLE_ENDIAN)
	#error "BIG_ENDIAN or LITTLE_ENDIAN should be defined in uart_driver.h"	
#endif

#if defined (BIG_ENDIAN) && defined (LITTLE_ENDIAN)
	#error "BIG_ENDIAN and LITTLE_ENDIAN should be only one in uart_driver.h"	
#endif

typedef struct uart_init_data {
	void (*gpio_init)(void);                    /* gpio初始化                             */
	void (*uart_init)(unsigned int baudrate);   /* 串口初始化                             */
	unsigned int baudrate;                      /* 初始化波特率                           */
} uart_init_data;

typedef struct uart_device {
	uart_init_data *uart_init_arr;
	void (*upper_support)(unsigned char tx);   /* 匿名上位机串口支持，选择串口发送一个字节 */
	void (*printf_support)(unsigned char tx);  /* printf串口支持，选择串口发送一个字节     */
	unsigned int num;                          /* 需要初始化的串口设备总数                 */
} uart_device;

/*
******************************************************************************************************************
*                                            FUNCTION PROTOTYPES
******************************************************************************************************************
*/

int uart_init(const uart_device *uart_dev);                                 /* 串口初始化             */
int upper_computer(unsigned char n, const float *src);                      /* 匿名上位机查看数据接口 */


#endif



