/*
 * Motor.c
 *
 *  Created on: 2020��11��15��
 *      Author: HJKF
 */
#include "stm32f10x.h"
#include "Motor00.h"
#include "math.h"
#include "../Customer/MotorType/MotorType_All.h"


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
	 TIM1->PSC = MOTOR_PSC;

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

//speedMaxAbs :��������ٶȣ���֧�����������븺���������Ԥ�ϡ���ʵ���������ϣ����������ߵĻ��ȱ仯�ٶȵ��������ֵ���仯�ٶ�ʱ�䵥λ����������ִ��Ƶ�ʵ�λ
//acc :���ٶȣ�֧��������ʾ���򣬿�������ת����תʱ�ض�ʱ���ݼ��ٶȼ��ٶȼ��ٺ��ټ��١���ʵ���������ϣ����������߻��ȵı仯�ʡ��仯�ٶȵ�ʱ�䵥λ����������ִ��Ƶ�ʵ�λ
//����ֵ����ǰ����
s32 Motor_IRQ(float speedMaxAbs,float acc)
{
	static float speed = 0;	//��ǰ���ٶȣ�����/ִ��Ƶ��ʱ�䵥λ��
	static float rad = 0;	//�ۼƵĽǶȣ����ȣ�
	static const float PI2 = 2 * 3.1415926;	//һ��Բ�ܵĻ���
	static const float PI = 3.1415926;	//��Բ�ܵĻ���
	static const float PI_Inver = 1.0 / 3.1415926 / 2.0;//һ��Բ�ܵĻ��ȣ����ڽ�������Ϊ�˷�
	static s32 steps = 0;

	float Iref = (float)fabs(speed) * (float)IREF_K_DEFAULT + (float)IREF_C_DEFAUL;	//�������
	if(Iref > (float)IREF_MAX)
	{
		Iref = (float)IREF_MAX;
	}

	//�ٶȵĻ���
	float speedAbs = fabs(speed);
	float accAbs =fabs(acc);

	s8 speedSign = speed ==0.0f ? 0 : (speed > 0.0f ? 1:-1);
	s8 accSign = acc ==0.0f ? 0 : (acc > 0.0f ? 1:-1);
	if(speedSign * accSign <= 0 || speedMaxAbs - speedAbs >=accAbs)	//��ǰ�ٶ�����ٶȷ����෴�������ٶȵľ���ֵС��ָ����������ֵ�������Լ�������ٶ�
	{
		speed += acc;
	}
	else if(speedMaxAbs < speedAbs)	//����ⲿ����������ٶȣ�����ǿ�Ƽ���
	{
		if(speedAbs > accAbs)
		{
			speedAbs -=accAbs;
		}
		else
		{
			speedAbs -=accAbs;
		}

		speed = speedAbs * speedSign;
	}

	rad +=speed;
	if(rad > PI2)
	{
		rad -= PI2;
		steps +=4;	//һ������4��
	}
	else if(rad < -PI)
	{
		rad +=PI2;
		steps -=4;
	}
	s32 stepsMicro = steps + 4.0f *(rad *PI_Inver);	//����δ��һ�����ڵĲ���


#if MOTORXY_TYPE == 0 //������

	float sin = sinf(rad);
	float cos = cosf(rad);

	if(sin<0.0f)
	{
		MOTOR_PWM_A = (1.0f+sin) *(float)MOTOR_ARR * Iref;
		MOTOR_PWM_B = MOTOR_ARR;
	}
	else
	{
		MOTOR_PWM_A = MOTOR_ARR;
		MOTOR_PWM_B = (1.0f-sin) *(float)MOTOR_ARR * Iref;
	}

	if(cos<0.0f)
	{
		MOTOR_PWM_C = (1.0f+cos) *(float)MOTOR_ARR * Iref;
		MOTOR_PWM_D = MOTOR_ARR;
	}
	else
	{
		MOTOR_PWM_C = MOTOR_ARR;
		MOTOR_PWM_D = (1.0f-cos) *(float)MOTOR_ARR * Iref;
	}


#elif MOTORXY_TYPE == 1 //������

		 float Aout,Bout,Cout;
		 float cosA;

		Aout = sinf(rad);
		cosA = cosf(rad);

        //��ʽ�Ƶ�������120��240��ʾ�Ƕȣ�a��ʾAout������ĽǶȣ�Aout����֪�ĵ�һ�����
        //sin(a) = Aout
        //Bout = sin(a + 120) = sin(a)cos(120) + cos(a)sin(120) = Aout * cos(120) + cosA * sin(120)
        //Cout = sin(a + 240) = sin(a)cos(240) + cos(a)sin(240) = Aout * cos(240) + cosA * sin(240)

        Bout = Aout * cosf((2*PI)/3) + cosA * sinf((2*PI)/3);
        Cout = Aout * cosf((4*PI)/3) + cosA * sinf((4*PI)/3);

        //��-1��1֮�䣬����ƫ�Ƶ�0��1֮��
        Aout *= 0.5;
        Bout *= 0.5;
        Cout *= 0.5;
        Aout += 0.5;
        Bout += 0.5;
        Cout += 0.5;

        MOTOR_PWM_A = Aout * MOTOR_ARR;
        MOTOR_PWM_B = Bout * MOTOR_ARR;
        MOTOR_PWM_C = Cout * MOTOR_ARR;
#endif

        return stepsMicro;
}


void TIM2_IRQHandler()
{
	 if(TIM2->SR & 0x1)
	 {
	  Motor_IRQ(150,0.05);
	 }

	 TIM2->SR &= ~(1<<0);//����жϱ�־λ
}
