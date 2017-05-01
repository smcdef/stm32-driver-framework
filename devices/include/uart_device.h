/*
*****************************************************************************************************************
*                                           MuChun Electron
*                                       ZhenJiang,Jiangsu,China
*                             (c) Copyright 2016,MuChun Electron,Zhenjiang,China
*                                         All Rights Reserved
*
*
*
*文 件 名 :uart_device.h
*作    者 :SMC
*文件说明 :串口设备数据，移植相关
*****************************************************************************************************************
*/
/* $ PAGE*/
#ifndef __UART_DEVICE_H
#define __UART_DEVICE_H

#include "uart_driver.h"



extern const uart_device uart_dev;

static void uart1_gpio_init(void);
static void uart1_init(unsigned int baudrate);
void uart1_send(unsigned char tx);



#endif


