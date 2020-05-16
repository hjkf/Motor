/*
 * timer2.c
 *
 *  Created on: 2020年4月30日
 *      Author: HSD
 */
#include "timer3.h"

#include "stm32f10x.h"
#include "../sys/sys.h"
#include"../_Global.h"
#include "math.h"
#include "Timer.h"



//static int64_t j;
//static _iq16 Amplitude=0;//当前的振幅0
//static u16 Angle;//电流系数
//_iq16 a_speed=0.2;
//_iq16 MotorAout;
//_iq16 MotorBout;
//static _iq16 a;
//extern _iq IQsinTable[];
//extern _iq IQcosTable[];
//CH1与CH2，PA6输出80Khz，占空比91%；PA7输出80Khz，占空比变化的方波
static int i;
void PWM3_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;       //TIM3时钟使能
	RCC->APB2ENR|=1<<2;       //使能PortA时钟
	RCC->APB2ENR|=1<<3;				//使能PortB时钟

	GPIOA->CRL&=0X00FFFFFF;//PA6,7输出
	GPIOA->CRL|=0XBB000000;//复用功能输出
	GPIOB->CRL&=0XFFFFFF00;//PB0,1输出
	GPIOB->CRL|=0X000000BB;//复用功能输出




	TIM3->ARR=arr;//设定计数器自动重装值
	TIM3->PSC=psc;//预分频器不分频

	TIM3->CCMR1|=7<<4; 	//CH1 PWM2模式
	TIM3->CCMR1|=1<<3;	//CH1预装载使能
	TIM3->CCMR1|=7<<12; //CH2 PWM2模式
	TIM3->CCMR1|=1<<11; //CH2预装载使能
	TIM3->CCMR2|=7<<4; 	//CH3 PWM2模式
	TIM3->CCMR2|=1<<3;	//CH3预装载使能
	TIM3->CCMR2|=7<<12; //CH4 PWM2模式
	TIM3->CCMR2|=1<<11; //CH4预装载使能

	TIM3->CCER|=1<<0;   //开CH1
	TIM3->CCER|=1<<4;   //开CH2 通道
	TIM3->CCER|=1<<8;   //开CH3 通道
	TIM3->CCER|=1<<12;  //开CH4 通道

	TIM3->DIER|=1<<0;   //允许更新中断
	TIM3->CR1|=1<<3;   //单脉冲模式
	TIM3->DIER|=1<<6;   //允许触发中断


	TIM3->CR1=0x0080;   //ARPE使能
	TIM3->CR1|=0x01;    //使能定时器3
	TIM3->SR=0;//清除所有标志位
  	MY_NVIC_Init(0,1,TIM3_IRQn,2);//抢占1，子优先级3，组2

}


//定时器3中断服务程序
void TIM3_IRQHandler(void)
{

	if(TIM3->SR&0X0001)//溢出中断
	{
			i++;
		  if(i>=512)
		  {
			  i=0;
		  }
		  for(i=0;i<512;i++)
		  {
			  TIM3->CCR3=sin(i+360/512)*10000;
			  TIM3->CCR4=cos(i+360/512)*10000;
		  }

	}

	TIM3->SR&=~(1<<0);//清除中断标志位
}
