/*
 * _Global.c
 *
 *  Created on: 2020年5月1日
 *      Author: HJKF
 */
#include "stm32f10x.h"

void led_Init()
{
	GPIOC->CRH&=0X0FFFFFFF;
	GPIOC->CRH|=0X30000000;//PA10 11 推挽输出
}



