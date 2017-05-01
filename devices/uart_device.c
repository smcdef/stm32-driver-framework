/*
*****************************************************************************************************************
*                                           MuChun Electron
*                                       ZhenJiang,Jiangsu,China
*                             (c) Copyright 2016,MuChun Electron,Zhenjiang,China
*                                         All Rights Reserved
*
*
*
*文 件 名 :uart_driver.c
*作    者 :SMC
*文件说明 :串口驱动代码，可跨平台移植
*****************************************************************************************************************
*/
/* $ PAGE*/
#include "uart_device.h"
#include "common.h"
#include "stm32f10x.h"



static uart_init_data uart_init_arr[] = {
	[0] = {
		.gpio_init = uart1_gpio_init,
		.uart_init = uart1_init,
		.baudrate  = 9600,
	},
	[1] = {
		.gpio_init = uart1_gpio_init,
		.uart_init = uart1_init,
		.baudrate  = 115200,
	},
};

const uart_device uart_dev = {
	.uart_init_arr  = uart_init_arr,              
	.upper_support  = uart1_send,                /* 匿名上位机串口支持，选择串口发送一个字节 */
	.printf_support = uart1_send,                /* printf串口支持，选择串口发送一个字节     */
	.num            = ARRAY_SIZE(uart_init_arr), /* 需要初始化的串口设备总数                 */
};

static void uart1_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;           /* uart TX引脚 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     /* 50MHz       */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;      /* 引脚复用    */
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;           /* uart RX引脚 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; /* 浮空输入    */
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void uart1_init(unsigned int baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
	USART_DeInit(USART1);  

	NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);	

	USART_InitStructure.USART_BaudRate            = baudrate;                        /* 波特率     */
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;             /* 8bit数据位 */
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;                /* 1bit停止位 */
	USART_InitStructure.USART_Parity              = USART_Parity_No;                 /* 无奇偶校验 */
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  /* 无硬件流控 */
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;   /* 收发模式   */

	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

void uart1_send(unsigned char tx)
{
	USART1->DR = tx;
	/* 循环发送,直到发送完毕 */
	while ((USART1->SR & 0X40) == 0);      
}


void USART1_IRQHandler(void)               
{
	unsigned char  Res;
	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		Res =USART_ReceiveData(USART1);
				 
	}
}









