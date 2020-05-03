/*
 * timer1_break.c
 *
 *  Created on: 2020锟斤拷4锟斤拷29锟斤拷
 *      Author: HSD
 */
#include "timer1.h"
#include "stm32f10x.h"
#include "../_Global.h"

void Timer1_Init(u16 arr,u16 psc)
{
	RCC->APB2ENR|=1<<11;	//TIM1时钟使能

 	TIM1->ARR=arr;  			//设定计数器自动重装值
	TIM1->PSC=psc;  			//预分频器
	TIM1->DIER|=1<<0;   	//允许更新中断
	TIM1->DIER|=1<<6;  	  //允许触发中断

	TIM1->CR1|=0x01;    	//使能定时器1
	TIM1->BDTR|=0x8000;  	//使能定时器1输出
  	MY_NVIC_Init(1,1,TIM1_UP_IRQn,2);//抢占1，子优先级3，组2
}

/*定时器1-5发出PWM初始化程序*/
void PWM1_Init(u16 arr,u16 psc)		//PA8 PA11
{
	 RCC->APB2ENR|=1<<11;       //TIM1时钟使能
	 RCC->APB2ENR|=1<<2;        //使能PortA时钟
	 RCC->APB2ENR|=1<<0;        //使能AFIO时钟	(?)

	 GPIOA->CRH&=0XFFFF0000;//PA8 11 输出
	 GPIOA->CRH|=0X0000BBBB;//复用功能输出

	 TIM1->BDTR |=0xC0;   //ARPE使能 		1100 0000		Tdtg=8*Tdts=1000ns    步长时间125ns
	 TIM1->ARR=arr;				//设定计数器自动重装值
	 TIM1->PSC=psc;				//预分频器不分频

	 TIM1->CCR1=600;		//调节占空比
	 TIM1->CCR2=300;
	 TIM1->CCR3=500;
	 TIM1->CCR4=800;

	 TIM1->CCMR1|=7<<4;   //CH2 PWM1模式 CH1/2
	 TIM1->CCMR1|=1<<3; 	//CH2预装载使能

	 TIM1->CCMR1|=7<<12; //CH2 PWM2模式
	 TIM1->CCMR1|=1<<11; //CH2预装载使能

	 TIM1->CCMR2|=7<<4; 	//CH3 PWM2模式
	 TIM1->CCMR2|=1<<3;	//CH3预装载使能

	 TIM1->CCMR2|=7<<12;  //CH2 PWM1模式 CH3/4
	 TIM1->CCMR2|=1<<11;  //CH2预装载使能

	 TIM1->CCER|=1<<0;    //OC1 输出使能
 	 TIM1->CCER|=1<<4;   //开CH2 通道
	 TIM1->CCER|=1<<8;   //开CH3 通道
	 TIM1->CCER|=1<<12;   //OC4 输出使能

	 TIM1->CR1 |=0x80;    //ARPE使能
	 TIM1->CR1|=0x01;     //使能定时器1
	 TIM1->BDTR|=0x8000;  //使能定时器1输出 		1000  主输出使能
}

/*定时器中断函数1-5
中断设置			*/

//定时器1中断服务程序
void TIM1_UP_IRQHandler(void)
{
	if(TIM1->SR&0X0001)//溢出中断
	{
		PC15OUT=!PC15OUT;
	}
	TIM1->SR&=~(1<<0);//清除中断标志位
}
