/*
 * Motor.c
 *
 *  Created on: 2020��11��15��
 *      Author: HJKF
 */
#include "stm32f10x.h"
#include "Motor00.h"
#include "math.h"

#define PI 3.14159265358979
#define MOTOR_ARR 4500	//�趨�������Զ���װֵ��PWMƵ�� = 72000 / ��TIM3->ARR + 1��kHz

//TIM2�����ж�
void TIM2_Int_Init()
{
	 NVIC_InitTypeDef NVIC_InitStructure;

	 RCC->APB1ENR|=1<<0; //TIM2ʱ��ʹ��
	  TIM2->ARR=300;   //�趨�������Զ���װֵ
	 TIM2->PSC=0;   //Ԥ��Ƶ������
	 TIM2->DIER|=1<<0;   //��������ж�
	 TIM2->CR1 |= 1<<2;//ֻ�м��������/����Ų��������жϻ�DMA����
	 TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��2


	 //Usart1 NVIC ����
	 NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�1
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�1
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //IRQͨ��ʹ��
	 NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

void Motor00_Init()
{
	 RCC->APB2ENR |= 1<<11 ; //ʹ��TIM1ʱ��

	 GPIOA->CRH &= 0xFFFF0000; //IO����
	 GPIOA->CRH |= 0x0000BBBB; //�������

	 TIM1->ARR = MOTOR_ARR - 1;
	 TIM1->PSC = 0;

	 TIM1->CCMR1 |= 7<<4; //CH1 PWM2ģʽ,CCR1��ֵԽ�󣬸����ƽ���Խ��
	 //TIM1->CCMR1|=6<<4;   //CH1 PWM1ģʽ,CCR1��ֵԽ�������ƽ���Խ��
	 TIM1->CCMR1 |= 1<<3; //CH1Ԥװ��ʹ��
	 TIM1->CCER |= 1<<0;  //OC1 ���ʹ��

	 TIM1->CCMR1|=7<<12;   //CH2 PWM2ģʽ,CCR1��ֵԽ�󣬸����ƽ���Խ��
	 //TIM1->CCMR1|=6<<12;   //CH2 PWM1ģʽ,CCR1��ֵԽ�������ƽ���Խ��
	 TIM1->CCMR1|=1<<11;  //CH2Ԥװ��ʹ��
	 TIM1->CCER|=1<<4;    //OC2 ���ʹ��

	 TIM1->CCMR2 |= 7<<4; //CH1 PWM2ģʽ,CCR1��ֵԽ�󣬸����ƽ���Խ��
	 //TIM1->CCMR2|=6<<4;   //CH1 PWM1ģʽ,CCR1��ֵԽ�������ƽ���Խ��
	 TIM1->CCMR2 |= 1<<3; //CH1Ԥװ��ʹ��
	 TIM1->CCER |= 1<<8;  //OC1 ���ʹ��

	 TIM1->CCMR2|=7<<12;   //CH2 PWM2ģʽ,CCR1��ֵԽ�󣬸����ƽ���Խ��
	 //TIM1->CCMR2|=6<<12;   //CH2 PWM1ģʽ,CCR1��ֵԽ�������ƽ���Խ��
	 TIM1->CCMR2|=1<<11;  //CH2Ԥװ��ʹ��
	 TIM1->CCER|=1<<12;    //OC2 ���ʹ��

	 TIM1->BDTR |= 1<<15; //MOE �����ʹ��
	 TIM1->CR1 = 0x0080;  //ARPEʹ��
	 TIM1->CR1 |= 0x01;  //ʹ�ܶ�ʱ��1


	 TIM2_Int_Init();
}

static void Motor_IRQ()
{
	static float speed = 0.05;
	static float rad = 0;
	static const float PI2 = 2 * 3.1415926;
	static const float PI_Inver = 1.0 / 3.1415926 / 2.0;
	static s32 steps = 0;
	float acc = 0.0005;

	//speed += acc;

	rad +=speed;
	if(rad > PI2)
	{
		rad -= PI2;
		steps +=4;
	}

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

}
void TIM2_IRQHandler()
{
	 if(TIM2->SR & 0x1)
	 {
	  Motor_IRQ();
	 }

	 TIM2->SR &= ~(1<<0);//����жϱ�־λ
}
