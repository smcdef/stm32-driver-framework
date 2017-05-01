/*
*****************************************************************************************************************
*                                           MuChun Electron
*                                       ZhenJiang,Jiangsu,China
*                             (c) Copyright 2016,MuChun Electron,Zhenjiang,China
*                                         All Rights Reserved
*
*
*
*文 件 名 :main.c
*作    者 :SMC
*文件说明 :none
*****************************************************************************************************************
*/
/* $ PAGE*/

#include <stm32f10x.h>	
#include "uart_driver.h"
#include "uart_device.h"
#include "common.h"



void delay(unsigned int n)
{
	int i, j;
	
	for (i = 0; i < n; i++)
		for (j = 0; j < 2000; j++);
}
/*
******************************************************************************************************************
*                                                  main()
*
*作    者 : SMC
*函数说明 : none
*输入参数 : none
*输出参数 : none
*返 回 值 : none
*注意事项 : none
******************************************************************************************************************
*/
int main(void)
{
	float num[3] = { 0 };
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);      /* 设置NVIC中断分组2:2位抢占优先级，2位响应优先级 */
	uart_init(&uart_dev);                                /* 初始化所有串口设备 */
	
	while (1) {
//		printf("I LOVE YOU\n");
		upper_computer(ARRAY_SIZE(num), num);

		if (num[0]++ > 255)
			num[0] = 0;
		if (num[1]++ > 255)
			num[1] = 0;
		if (num[2]++ > 255)
			num[2] = 0;

		delay(100);
	}
}














