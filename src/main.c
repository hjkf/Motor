#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "Timer.h"
#include "BlinkLed.h"
#include "stm32f10x.h"
#include "sys/sys.h"
#include "motor_int/motor_int.h"
#include "motor_int/motor_define.h"
#include "Timer_Break/timer1_brak.h"
#include "Timer_Break/timer2.h"
#include "LED/LED.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(int argc, char* argv[])
{
	//�Ĵ������λ����һ�������������ٸ�ֵ���ȿ��Խ�Լ����ռ䣬���ܱ��ִ���Ŀ��Ķ���
	//RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��
	//RCC->APB2ENR|=1<<3;//ʹ������PORTBʱ��
	//RCC->APB2ENR|=1<<4;//ʹ������PORTCʱ��
	//RCC->APB2ENR|=1<<5;//ʹ������PORTDʱ��
	//RCC->APB2ENR |= 1<<0; //��������ʱ��

// 	u16 led0pwmval=0;
//	u8 dir=1;

	Stm32_Clock_Init(9); //ϵͳʱ������

	const u32 APB2ENR = (1<<2) | (1<<3) | (1<<4) | (1<<5) | 1;
	const u32 APB1ENR = (1ul << 27) | (1ul << 28); //���ڱ��ݼĴ�����ʹ��
	RCC->APB2ENR |= APB2ENR;
	RCC->APB1ENR |= APB1ENR;

	GPIOC->CRH&=0X0FFFFFFF;
	GPIOC->CRH|=0X30000000;//PA10 11 �������

	motor_init();//��ʼ���˿�
	PB12OUT_LOW;//���ʹ��������
	motor_init();

	//TIM1_PWM_Init(49,7200);    // 72M/7200=10khz, 1/10khz * 500=50ms   ��8
	//Timer1_Init(500,7199);
//	Timer3_Init(5000,7199);
	PWM3_Init(300,7199);// 72M/7200=10khz, 1/10khz//0.1 * 500=50ms   ��8

  while (1)
    {
	  Motor_Run();
    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
