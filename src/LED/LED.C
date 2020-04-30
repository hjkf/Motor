/*
 * LED.C
 *
 *  Created on: 2020年4月30日
 *      Author: HSD
 */
#include "stm32f10x.h"
#include "LED.h"

void LED1()
{
	GPIOC->CRH&=0X0FFFFFFF;
	GPIOC->CRH|=0X30000000;//PA10 11 推挽输出
}


