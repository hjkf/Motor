/*
 * Motor.c
 *
 *  Created on: 2020年11月15日
 *      Author: HJKF
 */
#include "stm32f10x.h"
#include "Motor00.h"
#include "math.h"
#include "../Customer/MotorType/MotorType_All.h"

//#define PI 3.14159265358979
#define MOTOR_ARR 4500	//设定计数器自动重装值，PWM频率 = 72000 / （TIM3->ARR + 1）kHz

//TIM2用于中断
void TIM2_Int_Init()
{
	 NVIC_InitTypeDef NVIC_InitStructure;

	 RCC->APB1ENR|=1<<0; //TIM2时钟使能
	  TIM2->ARR=300;   //设定计数器自动重装值
	 TIM2->PSC=0;   //预分频器设置
	 TIM2->DIER|=1<<0;   //允许更新中断
	 TIM2->CR1 |= 1<<2;//只有计数器溢出/下溢才产生更新中断或DMA请求。
	 TIM2->CR1|=0x01;    //使能定时器2


	 //Usart1 NVIC 配置
	 NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级1
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //子优先级1
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //IRQ通道使能
	 NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
}

void Motor00_Init()
{
	 RCC->APB2ENR |= 1<<11 ; //使能TIM1时钟

	 GPIOA->CRH &= 0xFFFF0000; //IO配置
	 GPIOA->CRH |= 0x0000BBBB; //复用输出

	 TIM1->ARR = MOTOR_ARR - 1;
	 TIM1->PSC = 0;

	 TIM1->CCMR1 |= 7<<4; //CH1 PWM2模式,CCR1的值越大，负向电平宽度越大
	 //TIM1->CCMR1|=6<<4;   //CH1 PWM1模式,CCR1的值越大，正向电平宽度越大
	 TIM1->CCMR1 |= 1<<3; //CH1预装载使能
	 TIM1->CCER |= 1<<0;  //OC1 输出使能

	 TIM1->CCMR1|=7<<12;   //CH2 PWM2模式,CCR1的值越大，负向电平宽度越大
	 //TIM1->CCMR1|=6<<12;   //CH2 PWM1模式,CCR1的值越大，正向电平宽度越大
	 TIM1->CCMR1|=1<<11;  //CH2预装载使能
	 TIM1->CCER|=1<<4;    //OC2 输出使能

	 TIM1->CCMR2 |= 7<<4; //CH1 PWM2模式,CCR1的值越大，负向电平宽度越大
	 //TIM1->CCMR2|=6<<4;   //CH1 PWM1模式,CCR1的值越大，正向电平宽度越大
	 TIM1->CCMR2 |= 1<<3; //CH1预装载使能
	 TIM1->CCER |= 1<<8;  //OC1 输出使能

	 TIM1->CCMR2|=7<<12;   //CH2 PWM2模式,CCR1的值越大，负向电平宽度越大
	 //TIM1->CCMR2|=6<<12;   //CH2 PWM1模式,CCR1的值越大，正向电平宽度越大
	 TIM1->CCMR2|=1<<11;  //CH2预装载使能
	 TIM1->CCER|=1<<12;    //OC2 输出使能

	 TIM1->BDTR |= 1<<15; //MOE 主输出使能
	 TIM1->CR1 = 0x0080;  //ARPE使能
	 TIM1->CR1 |= 0x01;  //使能定时器1


	 TIM2_Int_Init();
}

static void Motor_IRQ()
{
	static float speed = 0.0000001;
	static float rad = 0;
	static const float PI2 = 2 * 3.1415926;
	static const float PI = 3.1415926;
	static const float PI_Inver = 1.0 / 3.1415926 / 2.0;
	static s32 steps = 0;
	float acc = 0.000005;
	static int flag=0;

	if(flag==0)
	{
		speed += acc;
		if(speed>=0.2)
		{
			speed=0.2;
			acc=0;
		}
	}
	else
	{
		speed -= acc;
		if(speed<=0.00001)
			flag=0;
	}

	rad +=speed;
	if(rad > PI2)
	{
		rad -= PI2;
		steps +=4;	//一个周期4步
	}
	else if(rad < -PI)
	{
		rad +=PI2;
		steps -=4;
	}
	s32 stepsMicro = steps + 4.0f *(rad *PI_Inver);
	if(stepsMicro>=2000)
	{
		speed=0;
		acc=0;
	}
#if MOTORXY_TYPE == 0 //两相电机

	float sin = sinf(rad);
	float cos = cosf(rad);
	float Iref = 0.00000001;

		if(sin < 0.0f)
		{
			TIM1->CCR1 = (1.0f+sin)*MOTOR_ARR*Iref;
			TIM1->CCR2 = MOTOR_ARR;
		}
		else
		{
			TIM1->CCR1 =MOTOR_ARR;
			TIM1->CCR2 = (1.0f-sin)*MOTOR_ARR*Iref;

		}
		if(cos < 0.0f)
		{
			TIM1->CCR3 = (1.0f+cos)*MOTOR_ARR*Iref;
			TIM1->CCR4 = MOTOR_ARR;
		}
		else
		{

			TIM1->CCR3 = MOTOR_ARR;
			TIM1->CCR4 = (1.0f-cos)*MOTOR_ARR*Iref;
		}
#elif MOTORXY_TYPE == 1 //三相电机

		 float Aout,Bout,Cout;
		 float cosA;

		Aout = sinf(rad);
		cosA = cosf(rad);

        //公式推导：其中120和240表示角度，a表示Aout所代表的角度，Aout是已知的第一相电流
        //sin(a) = Aout
        //Bout = sin(a + 120) = sin(a)cos(120) + cos(a)sin(120) = Aout * cos(120) + cosA * sin(120)
        //Cout = sin(a + 240) = sin(a)cos(240) + cos(a)sin(240) = Aout * cos(240) + cosA * sin(240)

        Bout = Aout * cosf((2*PI)/3) + cosA * sinf((2*PI)/3);
        Cout = Aout * cosf((4*PI)/3) + cosA * sinf((4*PI)/3);

        //从-1到1之间，正向偏移到0到1之间
        Aout *= 0.5;
        Bout *= 0.5;
        Cout *= 0.5;
        Aout += 0.5;
        Bout += 0.5;
        Cout += 0.5;

        TIM1->CCR1 = Aout * MOTOR_ARR;
        TIM1->CCR2 = Bout * MOTOR_ARR;
        TIM1->CCR3 = Cout * MOTOR_ARR;
#endif

}


void TIM2_IRQHandler()
{
	 if(TIM2->SR & 0x1)
	 {
	  Motor_IRQ();
	 }

	 TIM2->SR &= ~(1<<0);//清除中断标志位
}
