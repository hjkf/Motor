/*
 * motor_int.c
 *
 *  Created on: 2020��4��29��
 *      Author: HSD
 */

#include "stm32f10x.h"

void motor_init()
{
	GPIOA->CRH&=0XFFFF00FF;
	GPIOA->CRH|=0X00003300;//PA10 11 �������

	GPIOB->CRH&=0XFFF0FFFF;
	GPIOB->CRH|=0X00030000;//PB12  �������
}


