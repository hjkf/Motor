/*
 * _Global.c
 *
 *  Created on: 2020��5��1��
 *      Author: HJKF
 */
#include "stm32f10x.h"

void led_Init()
{
	GPIOC->CRH&=0X0FFFFFFF;
	GPIOC->CRH|=0X30000000;//PA10 11 �������
}



