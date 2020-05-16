/*
 * timer1_break.c
 *
 *  Created on: 2020��4��29��
 *      Author: HSD
 */
#include "timer1.h"
#include "stm32f10x.h"
#include "../_Global.h"


/*��ʱ��1-5����PWM��ʼ������*/
void Timer1_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<0;       //TIM3ʱ��ʹ��
	RCC->APB2ENR|=1<<2;       //ʹ��PortAʱ��
	RCC->APB2ENR|=1<<3;				//ʹ��PortBʱ��

//	GPIOA->CRL&=0X00FFFFFF;//PA6,7���
//	GPIOA->CRL|=0XBB000000;//���ù������
//	GPIOB->CRL&=0XFFFFFF00;//PB0,1���
//	GPIOB->CRL|=0X000000BB;//���ù������

	TIM2->ARR=arr;//�趨�������Զ���װֵ
	TIM2->PSC=psc;//Ԥ��Ƶ������Ƶ

	TIM2->CCMR1|=7<<12; //CH2 PWM2ģʽ
	TIM2->CCMR1|=1<<11; //CH2Ԥװ��ʹ��
	TIM2->CCMR2|=7<<4; 	//CH3 PWM2ģʽ
	TIM2->CCMR2|=1<<3;	//CH3Ԥװ��ʹ��
	TIM2->CCMR2|=7<<12; //CH4 PWM2ģʽ
	TIM2->CCMR2|=1<<11; //CH4Ԥװ��ʹ��

	TIM2->CCER|=1<<0;   //��CH1
	TIM2->CCER|=1<<4;   //��CH2 ͨ��
	TIM2->CCER|=1<<8;   //��CH3 ͨ��
	TIM2->CCER|=1<<12;  //��CH4 ͨ��

	TIM2->DIER|=1<<0;   //��������ж�
	TIM2->CR1|=1<<3;   //������ģʽ
	TIM2->DIER|=1<<6;   //�������ж�


	TIM2->CR1=0x0080;   //ARPEʹ��
	TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��3
	TIM2->SR=0;//������б�־λ
  	MY_NVIC_Init(0,1,TIM2_IRQn,2);//��ռ1�������ȼ�3����2

}

/*��ʱ���жϺ���1-5
�ж�����			*/

//��ʱ��1�жϷ������
void TIM2_IRQHandler(void)
{
	if(TIM2->SR&0X0001)//����ж�
	{
		PC15OUT=!PC15OUT;

	}
	TIM2->SR&=~(1<<0);//����жϱ�־λ
}
