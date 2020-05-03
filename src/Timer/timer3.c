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


static int64_t i;
static int64_t j;
//static _iq16 Amplitude=0;//当前的振幅0
//static u16 Angle;//电流系数
//_iq16 a_speed=0.2;
//_iq16 MotorAout;
//_iq16 MotorBout;
//static _iq16 a;

//CH1与CH2，PA6输出80Khz，占空比91%；PA7输出80Khz，占空比变化的方波
void PWM3_Init(u16 psc)
{
	RCC->APB1ENR|=1<<1;       //TIM3时钟使能
	RCC->APB2ENR|=1<<2;       //使能PortA时钟
	RCC->APB2ENR|=1<<3;				//使能PortB时钟

	GPIOA->CRL&=0X00FFFFFF;//PA6,7输出
	GPIOA->CRL|=0XBB000000;//复用功能输出
	GPIOB->CRL&=0XFFFFFF00;//PB0,1输出
	GPIOB->CRL|=0X000000BB;//复用功能输出

	GPIOA->ODR|=1<<6;//PA6上拉
	GPIOA->ODR|=1<<7;//PA7上拉
	GPIOB->ODR|=1<<0;//PB0上拉
	GPIOB->ODR|=1<<1;//PB1上拉


	TIM3->ARR=50;//设定计数器自动重装值
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
  	MY_NVIC_Init(1,1,TIM3_IRQn,2);//抢占1，子优先级3，组2

}

//void Timer3_Init(u16 arr,u16 psc)
//{
//	RCC->APB1ENR|=1<<1;//TIM3时钟使能
//
// 	TIM3->ARR=arr; 		  //设定计数器自动重装值
//	TIM3->PSC=psc;  		//预分频器
//	TIM3->DIER|=1<<0;   //允许更新中断
//	TIM3->DIER|=1<<6;   //允许触发中断
//
//	TIM3->CR1|=0x01;    //使能定时器3
//  	MY_NVIC_Init(2,3,TIM3_IRQn,2);//抢占1，子优先级3，组2
//}
//定时器3中断服务程序
void TIM3_IRQHandler(void)
{

	if(TIM3->SR&0X0001)//溢出中断
	{
		i=i+0.2;
		j++;
		//TIM3->ARR=10;

//		if(j>=1000)
//		{
//			TIM3->ARR=8;
//			arr=8;
//		}
//		if(j%100==0)
//		{
//			(TIM3->ARR)--;
//		}
//		switch(i)
//		{
//			case 1:PA11OUT_HIGH;TIM3->CCR3=100;TIM3->CCR4=0;break;
//			case 2:PA10OUT_LOW;TIM3->CCR4=100;TIM3->CCR3=0;break;
//
//			case 3:PA11OUT_LOW;TIM3->CCR3=100;TIM3->CCR4=0;break;
//			default :i=0;PA10OUT_HIGH;TIM3->CCR4=0;TIM3->CCR3=100;break;
//
//		}
//		if(j%10==0)
//		{
//			TIM3->ARR=(TIM3->ARR)-0.01;
//		}
//	       Amplitude+=a_speed;//累积位移
//	        if((Amplitude>>16)>=200*128)
//	        {
//	            Amplitude=0;//弧度位移清零
//	        }
//	        //TI例程，根据当前步数和细分数，换算两相电流的值，对应到IQsinTable[]的下标
//	        Angle =(Amplitude>>16) & 511;
//
//	        //两相电流的占空比
//	        MotorAout = (u16)IQsinTable[Angle] >> 16;
//	        MotorBout = (u16)IQcosTable[Angle] >> 16;
//
//            a = (MotorAout+ 65536) >> 1;
//            TIM3->CCR3=(1000*a)>>16;
//            TIM3->CCR4=(1000*a)>>16;


//		if(j>=2000)
//		{
//			j=2000;
//			TIM3->CR1|=~(0x01);    //使能定时器3
//			PB12OUT_HIGH;
//		}
	}
	TIM3->SR&=~(1<<0);//清除中断标志位
}
