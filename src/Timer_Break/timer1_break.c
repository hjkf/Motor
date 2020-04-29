/*
 * timer1_break.c
 *
 *  Created on: 2020锟斤拷4锟斤拷29锟斤拷
 *      Author: HSD
 */
#include "timer1_brak.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
 	u16 led0pwmval=0;
	u8 dir=1;
	u16 pwm1_index;
	u16 pwm2_index;
	u16 pwm3_index;
	u16 pwm4_index;
	u16 indexWave[];

//TIM1_CH1 PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM1_PWM_Init(u16 arr,u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	//此部分需手动修改IO口设置
	RCC->APB2ENR|=1<<11; 	//TIM1时钟使能
	GPIOA->CRH&=0XFFFF0000;	//PA8清除之前的设置
	GPIOA->CRH|=0X0000BBBB;	//复用功能输出

	TIM1->ARR=arr;			//设定计数器自动重装值
	TIM1->PSC=psc;			//预分频器设置
	TIM1->DIER|=1<<0;   	//允许更新中断

	TIM1->CCER|=1<<0;      //TIM1CH1 输出使能,高电平有效      ③3
	TIM1->CCER|=1<<4;      //TIM1CH2 输出使能
	TIM1->CCER|=1<<8;      //TIM1CH3 输出使能
	TIM1->CCER|=1<<12;      //TIM1CH4 输出使能

	TIM1->CCER|=1<<2;      //TIM1CH1N 互补输出使能
	TIM1->CCER|=1<<6;      //TIM1CH2N 互补输出使能
	TIM1->CCER|=1<<10;      //TIM1CH3N 互补输出使能

	TIM1->CCMR1|=7<<4;     //CH1 PWM2模式           ④4
	TIM1->CCMR1|=1<<3;     //CH1预装载使能

	TIM1->CCMR1|=7<<12;    //CH2 PWM2模式
	TIM1->CCMR1|=1<<11;    //CH2预装载使能

	TIM1->CCMR2|=7<<4;     //CH3 PWM2模式
	TIM1->CCMR2|=1<<3;     //CH3预装载使能

	TIM1->CCMR2|=7<<12;    //CH4 PWM2模式
	TIM1->CCMR2|=1<<11;    //CH4预装载使能

	TIM1->BDTR|=0x14;       //死区时间设置          ⑤5
	TIM1->BDTR|=1<<15;     //MOE 主输出使能        ⑥6

	TIM1->CR1 |= 0x80;       //ARPE使能,开始所有输出通道,默认向上计数   ⑦7
	TIM1->CR1 |= 0x01;       //使能计数器

	TIM_ITConfig(TIM1, TIM_IT_Update ,ENABLE);  //TIM1   使能或者失能指定的TIM中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

void TIM1_UP_IRQHandler()
{

}
