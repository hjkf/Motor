/*
 * timer2.c
 *
 *  Created on: 2020��4��30��
 *      Author: HSD
 */
#include "timer3.h"

#include "stm32f10x.h"
#include "../sys/sys.h"
#include"../_Global.h"
#include "math.h"
#include "Timer.h"



//static int64_t j;
//static _iq16 Amplitude=0;//��ǰ�����0
//static u16 Angle;//����ϵ��
//_iq16 a_speed=0.2;
//_iq16 MotorAout;
//_iq16 MotorBout;
//static _iq16 a;
//extern _iq IQsinTable[];
//extern _iq IQcosTable[];
//CH1��CH2��PA6���80Khz��ռ�ձ�91%��PA7���80Khz��ռ�ձȱ仯�ķ���
static int i;
void PWM3_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;       //TIM3ʱ��ʹ��
	RCC->APB2ENR|=1<<2;       //ʹ��PortAʱ��
	RCC->APB2ENR|=1<<3;				//ʹ��PortBʱ��

	GPIOA->CRL&=0X00FFFFFF;//PA6,7���
	GPIOA->CRL|=0XBB000000;//���ù������
	GPIOB->CRL&=0XFFFFFF00;//PB0,1���
	GPIOB->CRL|=0X000000BB;//���ù������




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
  	MY_NVIC_Init(0,1,TIM3_IRQn,2);//��ռ1�������ȼ�3����2

}


//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)
{

	if(TIM3->SR&0X0001)//����ж�
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

	TIM3->SR&=~(1<<0);//����жϱ�־λ
}
