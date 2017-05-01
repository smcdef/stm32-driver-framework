/*
*****************************************************************************************************************
*                                           MuChun Electron
*                                       ZhenJiang,Jiangsu,China
*                             (c) Copyright 2016,MuChun Electron,Zhenjiang,China
*                                         All Rights Reserved
*
*
*
*文 件 名 :_uart_drv.c
*作    者 :SMC
*文件说明 :串口驱动代码，可跨平台移植
*****************************************************************************************************************
*/
/* $ PAGE*/
#include "uart_driver.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include "malloc.h"




static uart_device *_uart_drv;

/*
******************************************************************************************************************
*                                                  uart_init()
*
*作    者 : SMC
*函数说明 : 串口初始化
*输入参数 : uart_dev
*输出参数 : none
*返 回 值 : 0:初始化完成; -1:初始化失败
*注意事项 : 设备驱动信息中可以不包含串口的gpio初始化，可以将gpio初始化相关代码直接写在串口初始化相关函数中，因此
*           gpio_init不是必须的，但是uart_init是必须要有的
******************************************************************************************************************
*/
int uart_init(const uart_device *uart_dev)
{
	int i;
	
	for (i = 0; i < uart_dev->num; i++) {
		if (uart_dev->uart_init_arr[i].gpio_init)
			uart_dev->uart_init_arr[i].gpio_init();                                     /* gpio初始化                 */
		if (uart_dev->uart_init_arr[i].uart_init)
			uart_dev->uart_init_arr[i].uart_init(uart_dev->uart_init_arr[i].baudrate);  /* 初始化串口波特率为baudrate */
		else 
			return -1;
	}
	
	if (uart_dev->upper_support == NULL || uart_dev->printf_support == NULL)
		return -1;
	_uart_drv = (uart_device *)uart_dev;
	
	return 0;
}

/*
******************************************************************************************************************
*                                                  upper_computer()
*
*作    者 : SMC
*函数说明 : 匿名上位机通信查看数据波形
*输入参数 : 数据个数n，数据首地址src
*输出参数 : none
*返 回 值 : 发送数据成功返回发送的数据字节数，否则返回-1
*注意事项 : 此函数分为两个版本，分别针对大小端不同的处理器
******************************************************************************************************************
*/
#ifdef LITTLE_ENDIAN       /* 小端处理器 */
int upper_computer(unsigned char n, const float *src)
{
	unsigned char i, j, sum;
	unsigned char *send_data;
	
	send_data = (unsigned char *)malloc_m(sizeof(*src) * n + 4);
	if (send_data == NULL)
		return -1;
	
	send_data[0] = 0xAA;                            /* 帧头     */
	send_data[1] = 0xAA;		                    /* 帧头     */
	send_data[2] = 0xF1;                            /* 功能字   */
	send_data[3] = n * sizeof(*src);                /* 数据长度 */

	for (i = 0; i < n; i++, src++) {
		for (j = 0; j < sizeof(*src); j++)
			send_data[4 + i * sizeof(*src) + j] = *((char *)src + sizeof(*src) - 1 - j);
	}
	
	for (i = 0, sum = 0; i < 4 + send_data[3]; i++) {
		sum += send_data[i];
		_uart_drv->upper_support(send_data[i]);
	}
	_uart_drv->upper_support(sum);
	free_m(send_data);
	
	return n * sizeof(*src);
}
#endif

#ifdef BIG_ENDIAN         /* 大端处理器 */
int upper_computer(unsigned char n, const float *src)
{
	unsigned char i, j, sum;
	unsigned char *send_data;
	
	send_data = (unsigned char *)malloc_m(sizeof(*src) * n + 4);
	if (send_data == NULL)
		return -1;
	
	send_data[0] = 0xAA;                            /* 帧头     */
	send_data[1] = 0xAA;		                    /* 帧头     */
	send_data[2] = 0xF1;                            /* 功能字   */
	send_data[3] = n * sizeof(*src);                /* 数据长度 */

	for (i = 0; i < n; i++, src++) {
		for (j = 0; j < sizeof(*src); j++)
			send_data[4 + i * sizeof(*src) + j] = *((char *)src + j);
	}
	
	for (i = 0, sum = 0; i < 4 + send_data[3]; i++) {
		sum += send_data[i];
		_uart_drv->upper_support(send_data[i]);
	}
	_uart_drv->upper_support(sum);
	free_m(send_data);
	
	return n * sizeof(*src);
}
#endif

/*
******************************************************************************************************************
*                                                  fputc()
*
*作    者 : SMC
*函数说明 : pruntf函数支持
*输入参数 : none
*输出参数 : none
*返 回 值 : none
*注意事项 : none
******************************************************************************************************************
*/
#pragma import(__use_no_semihosting)             
struct __FILE                       /* 标准库需要的支持函数 */              
{ 
	int handle;
};

FILE __stdout;

void _sys_exit(int x)               /* 定义_sys_exit()以避免使用半主机模式 */
{
	x = x;
}

int fputc(int ch, FILE *f)          /* 重定义fputc函数 */
{
	_uart_drv->printf_support((unsigned char)ch);

	return ch;
}

void _ttywrch(int ch)               /* 删除此函数会导致malloc无法使用 */
{
	_uart_drv->printf_support((unsigned char)ch);
}











