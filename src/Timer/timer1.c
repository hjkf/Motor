/*
 * timer1_break.c
 *
 *  Created on: 2020锟斤拷4锟斤拷29锟斤拷
 *      Author: HSD
 */
#include "timer1.h"
#include "stm32f10x.h"
#include "../_Global.h"


/*定时器1-5发出PWM初始化程序*/
void Timer1_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<0;       //TIM3时钟使能
	RCC->APB2ENR|=1<<2;       //使能PortA时钟
	RCC->APB2ENR|=1<<3;				//使能PortB时钟

//	GPIOA->CRL&=0X00FFFFFF;//PA6,7输出
//	GPIOA->CRL|=0XBB000000;//复用功能输出
//	GPIOB->CRL&=0XFFFFFF00;//PB0,1输出
//	GPIOB->CRL|=0X000000BB;//复用功能输出

	TIM2->ARR=arr;//设定计数器自动重装值
	TIM2->PSC=psc;//预分频器不分频

	TIM2->CCMR1|=7<<12; //CH2 PWM2模式
	TIM2->CCMR1|=1<<11; //CH2预装载使能
	TIM2->CCMR2|=7<<4; 	//CH3 PWM2模式
	TIM2->CCMR2|=1<<3;	//CH3预装载使能
	TIM2->CCMR2|=7<<12; //CH4 PWM2模式
	TIM2->CCMR2|=1<<11; //CH4预装载使能

	TIM2->CCER|=1<<0;   //开CH1
	TIM2->CCER|=1<<4;   //开CH2 通道
	TIM2->CCER|=1<<8;   //开CH3 通道
	TIM2->CCER|=1<<12;  //开CH4 通道

	TIM2->DIER|=1<<0;   //允许更新中断
	TIM2->CR1|=1<<3;   //单脉冲模式
	TIM2->DIER|=1<<6;   //允许触发中断


	TIM2->CR1=0x0080;   //ARPE使能
	TIM2->CR1|=0x01;    //使能定时器3
	TIM2->SR=0;//清除所有标志位
  	MY_NVIC_Init(0,1,TIM2_IRQn,2);//抢占1，子优先级3，组2

}

/*定时器中断函数1-5
中断设置			*/

//定时器1中断服务程序
void TIM2_IRQHandler(void)
{
	if(TIM2->SR&0X0001)//溢出中断
	{
		PC15OUT=!PC15OUT;

	}
	TIM2->SR&=~(1<<0);//清除中断标志位
}
