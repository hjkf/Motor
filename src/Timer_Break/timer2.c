/*
 * timer2.c
 *
 *  Created on: 2020��4��30��
 *      Author: HSD
 */
#include "stm32f10x.h"
#include "timer2.h"
#include "../LED/LED.h"

static float Amplitude;
static u16 flag;


//CH1��CH2��PA6���80Khz��ռ�ձ�91%��PA7���80Khz��ռ�ձȱ仯�ķ���
void PWM3_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;       //TIM3ʱ��ʹ��
	RCC->APB2ENR|=1<<2;       //ʹ��PortAʱ��
	RCC->APB2ENR|=1<<3;				//ʹ��PortBʱ��

	GPIOA->CRL&=0X00FFFFFF;//PA6,7���
	GPIOA->CRL|=0XBB000000;//���ù������
	GPIOB->CRL&=0XFFFFFF00;//PB0,1���
	GPIOB->CRL|=0X000000BB;//���ù������

	GPIOA->ODR|=1<<6;//PA6����
	GPIOA->ODR|=1<<7;//PA7����
	GPIOB->ODR|=1<<0;//PB0����
	GPIOB->ODR|=1<<1;//PB1����


	TIM3->ARR=arr;//�趨�������Զ���װֵ
	TIM3->PSC=psc;//Ԥ��Ƶ������Ƶ

	TIM3->CCMR1|=7<<4; 	//CH1 PWM2ģʽ
	TIM3->CCMR1|=1<<3;	//CH1Ԥװ��ʹ��
	TIM3->CCMR1|=7<<12; //CH2 PWM2ģʽ
	TIM3->CCMR1|=1<<11; //CH2Ԥװ��ʹ��
	TIM3->CCMR2|=7<<4; 	//CH3 PWM2ģʽ
	TIM3->CCMR2|=1<<3;	//CH3Ԥװ��ʹ��
	TIM3->CCMR2|=7<<12; //CH4 PWM2ģʽ
	TIM3->CCMR2|=1<<11; //CH4Ԥװ��ʹ��

	TIM3->CCER|=1<<0;   //��CH1
	TIM3->CCER|=1<<4;   //��CH2 ͨ��
	TIM3->CCER|=1<<8;   //��CH3 ͨ��
	TIM3->CCER|=1<<12;  //��CH4 ͨ��

	TIM3->DIER|=1<<0;   //��������ж�
	TIM3->CR1|=1<<3;   //������ģʽ
	TIM3->DIER|=1<<6;   //�������ж�


	TIM3->CR1=0x0080;   //ARPEʹ��
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
	TIM3->SR=0;//������б�־λ
  	MY_NVIC_Init(2,3,TIM3_IRQn,2);//��ռ1�������ȼ�3����2

}

//void Timer3_Init(u16 arr,u16 psc)
//{
//	RCC->APB1ENR|=1<<1;//TIM3ʱ��ʹ��
//
// 	TIM3->ARR=arr; 		  //�趨�������Զ���װֵ
//	TIM3->PSC=psc;  		//Ԥ��Ƶ��
//	TIM3->DIER|=1<<0;   //��������ж�
//	TIM3->DIER|=1<<6;   //�������ж�
//
//	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
//  	MY_NVIC_Init(2,3,TIM3_IRQn,2);//��ռ1�������ȼ�3����2
//}
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)
{
	if(TIM3->SR&0X0001)//����ж�
	{
		if(flag==0)
		{
			Amplitude=Amplitude+1;
		}
		if(flag==1)
		{
			Amplitude=Amplitude-1;
		}
	}
	TIM3->SR&=~(1<<0);//����жϱ�־λ
}
void Motor_Run()
{
	TIM3->CCR3=30+Amplitude;		//����ռ�ձ�
	TIM3->CCR4=100+Amplitude;
	if((TIM3->CCR4)>=300)
	{
		flag=1;
	}
	if((TIM3->CCR4)<=90)
	{
		flag=0;
	}
}

