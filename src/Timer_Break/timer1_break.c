/*
 * timer1_break.c
 *
 *  Created on: 2020��4��29��
 *      Author: HSD
 */
#include "timer1_brak.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
 	u16 led0pwmval=0;
	u8 dir=1;
	u16 pwm1_index;
	u16 pwm2_index;
	u16 pwm3_index;
	u16 pwm4_index;
	u16 indexWave[];

//TIM1_CH1 PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM1_PWM_Init(u16 arr,u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	//�˲������ֶ��޸�IO������
	RCC->APB2ENR|=1<<11; 	//TIM1ʱ��ʹ��
	GPIOA->CRH&=0XFFFF0000;	//PA8���֮ǰ������
	GPIOA->CRH|=0X0000BBBB;	//���ù������

	TIM1->ARR=arr;			//�趨�������Զ���װֵ
	TIM1->PSC=psc;			//Ԥ��Ƶ������
	TIM1->DIER|=1<<0;   	//��������ж�

	TIM1->CCER|=1<<0;      //TIM1CH1 ���ʹ��,�ߵ�ƽ��Ч      ��3
	TIM1->CCER|=1<<4;      //TIM1CH2 ���ʹ��
	TIM1->CCER|=1<<8;      //TIM1CH3 ���ʹ��
	TIM1->CCER|=1<<12;      //TIM1CH4 ���ʹ��

	TIM1->CCER|=1<<2;      //TIM1CH1N �������ʹ��
	TIM1->CCER|=1<<6;      //TIM1CH2N �������ʹ��
	TIM1->CCER|=1<<10;      //TIM1CH3N �������ʹ��

	TIM1->CCMR1|=7<<4;     //CH1 PWM2ģʽ           ��4
	TIM1->CCMR1|=1<<3;     //CH1Ԥװ��ʹ��

	TIM1->CCMR1|=7<<12;    //CH2 PWM2ģʽ
	TIM1->CCMR1|=1<<11;    //CH2Ԥװ��ʹ��

	TIM1->CCMR2|=7<<4;     //CH3 PWM2ģʽ
	TIM1->CCMR2|=1<<3;     //CH3Ԥװ��ʹ��

	TIM1->CCMR2|=7<<12;    //CH4 PWM2ģʽ
	TIM1->CCMR2|=1<<11;    //CH4Ԥװ��ʹ��

	TIM1->BDTR|=0x14;       //����ʱ������          ��5
	TIM1->BDTR|=1<<15;     //MOE �����ʹ��        ��6

	TIM1->CR1 |= 0x80;       //ARPEʹ��,��ʼ�������ͨ��,Ĭ�����ϼ���   ��7
	TIM1->CR1 |= 0x01;       //ʹ�ܼ�����

	TIM_ITConfig(TIM1, TIM_IT_Update ,ENABLE);  //TIM1   ʹ�ܻ���ʧ��ָ����TIM�ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

void TIM1_UP_IRQHandler()
{

}
