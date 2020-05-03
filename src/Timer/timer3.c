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


static int64_t i;
static int64_t j;
//static _iq16 Amplitude=0;//��ǰ�����0
//static u16 Angle;//����ϵ��
//_iq16 a_speed=0.2;
//_iq16 MotorAout;
//_iq16 MotorBout;
//static _iq16 a;

//CH1��CH2��PA6���80Khz��ռ�ձ�91%��PA7���80Khz��ռ�ձȱ仯�ķ���
void PWM3_Init(u16 psc)
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


	TIM3->ARR=50;//�趨�������Զ���װֵ
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
  	MY_NVIC_Init(1,1,TIM3_IRQn,2);//��ռ1�������ȼ�3����2

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
//	       Amplitude+=a_speed;//�ۻ�λ��
//	        if((Amplitude>>16)>=200*128)
//	        {
//	            Amplitude=0;//����λ������
//	        }
//	        //TI���̣����ݵ�ǰ������ϸ�������������������ֵ����Ӧ��IQsinTable[]���±�
//	        Angle =(Amplitude>>16) & 511;
//
//	        //���������ռ�ձ�
//	        MotorAout = (u16)IQsinTable[Angle] >> 16;
//	        MotorBout = (u16)IQcosTable[Angle] >> 16;
//
//            a = (MotorAout+ 65536) >> 1;
//            TIM3->CCR3=(1000*a)>>16;
//            TIM3->CCR4=(1000*a)>>16;


//		if(j>=2000)
//		{
//			j=2000;
//			TIM3->CR1|=~(0x01);    //ʹ�ܶ�ʱ��3
//			PB12OUT_HIGH;
//		}
	}
	TIM3->SR&=~(1<<0);//����жϱ�־λ
}
