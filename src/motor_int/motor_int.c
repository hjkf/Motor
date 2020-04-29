/*
 * motor_int.c
 *
 *  Created on: 2020年4月29日
 *      Author: HSD
 */

#include "stm32f10x.h"

void motor_init()
{
	GPIOA->CRH&=0XFFFF00FF;
	GPIOA->CRH|=0X00003300;//PA10 11 推挽输出

	GPIOB->CRH&=0XFFF0FFFF;
	GPIOB->CRH|=0X00030000;//PB12  推挽输出
}


