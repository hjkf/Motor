/*
 * timer1_break.c
 *
 *  Created on: 2020��4��29��
 *      Author: HSD
 */
#include "timer1.h"
#include "stm32f10x.h"
#include "../_Global.h"

void Timer1_Init(u16 arr,u16 psc)
{
	RCC->APB2ENR|=1<<11;	//TIM1ʱ��ʹ��

 	TIM1->ARR=arr;  			//�趨�������Զ���װֵ
	TIM1->PSC=psc;  			//Ԥ��Ƶ��
	TIM1->DIER|=1<<0;   	//��������ж�
	TIM1->DIER|=1<<6;  	  //�������ж�

	TIM1->CR1|=0x01;    	//ʹ�ܶ�ʱ��1
	TIM1->BDTR|=0x8000;  	//ʹ�ܶ�ʱ��1���
  	MY_NVIC_Init(1,1,TIM1_UP_IRQn,2);//��ռ1�������ȼ�3����2
}

/*��ʱ��1-5����PWM��ʼ������*/
void PWM1_Init(u16 arr,u16 psc)		//PA8 PA11
{
	 RCC->APB2ENR|=1<<11;       //TIM1ʱ��ʹ��
	 RCC->APB2ENR|=1<<2;        //ʹ��PortAʱ��
	 RCC->APB2ENR|=1<<0;        //ʹ��AFIOʱ��	(?)

	 GPIOA->CRH&=0XFFFF0000;//PA8 11 ���
	 GPIOA->CRH|=0X0000BBBB;//���ù������

	 TIM1->BDTR |=0xC0;   //ARPEʹ�� 		1100 0000		Tdtg=8*Tdts=1000ns    ����ʱ��125ns
	 TIM1->ARR=arr;				//�趨�������Զ���װֵ
	 TIM1->PSC=psc;				//Ԥ��Ƶ������Ƶ

	 TIM1->CCR1=600;		//����ռ�ձ�
	 TIM1->CCR2=300;
	 TIM1->CCR3=500;
	 TIM1->CCR4=800;

	 TIM1->CCMR1|=7<<4;   //CH2 PWM1ģʽ CH1/2
	 TIM1->CCMR1|=1<<3; 	//CH2Ԥװ��ʹ��

	 TIM1->CCMR1|=7<<12; //CH2 PWM2ģʽ
	 TIM1->CCMR1|=1<<11; //CH2Ԥװ��ʹ��

	 TIM1->CCMR2|=7<<4; 	//CH3 PWM2ģʽ
	 TIM1->CCMR2|=1<<3;	//CH3Ԥװ��ʹ��

	 TIM1->CCMR2|=7<<12;  //CH2 PWM1ģʽ CH3/4
	 TIM1->CCMR2|=1<<11;  //CH2Ԥװ��ʹ��

	 TIM1->CCER|=1<<0;    //OC1 ���ʹ��
 	 TIM1->CCER|=1<<4;   //��CH2 ͨ��
	 TIM1->CCER|=1<<8;   //��CH3 ͨ��
	 TIM1->CCER|=1<<12;   //OC4 ���ʹ��

	 TIM1->CR1 |=0x80;    //ARPEʹ��
	 TIM1->CR1|=0x01;     //ʹ�ܶ�ʱ��1
	 TIM1->BDTR|=0x8000;  //ʹ�ܶ�ʱ��1��� 		1000  �����ʹ��
}

/*��ʱ���жϺ���1-5
�ж�����			*/

//��ʱ��1�жϷ������
void TIM1_UP_IRQHandler(void)
{
	if(TIM1->SR&0X0001)//����ж�
	{
		PC15OUT=!PC15OUT;
	}
	TIM1->SR&=~(1<<0);//����жϱ�־λ
}
