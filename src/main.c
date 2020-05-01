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
#include "../include/IQmath/v160/include/IQmathLib.h"
#include "_Global.h"
#include "stdbool.h"
#include "../include/math_clocks/v2.1/sin_cos_table.h"
#include "PWM/_pwm_All.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

//extern _iq IQsinTable[];
//extern _iq IQcosTable[];

int main(int argc, char* argv[])
{
	//�Ĵ������λ����һ�������������ٸ�ֵ���ȿ��Խ�Լ����ռ䣬���ܱ��ִ���Ŀ��Ķ���
	//RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��
	//RCC->APB2ENR|=1<<3;//ʹ������PORTBʱ��
	//RCC->APB2ENR|=1<<4;//ʹ������PORTCʱ��
	//RCC->APB2ENR|=1<<5;//ʹ������PORTDʱ��
	//RCC->APB2ENR |= 1<<0; //��������ʱ��
//	bool falg;
////    static bool dir=true;
//    static int x_turn=0;//��ת��Ȧ�ı���
////    static int y_turn=0;//��ת��Ȧ�ı���
//    static _iq16 Amplitude=0;//��ǰ�����0
//    static u16 Angle;//����ϵ��
//    static _iq16 a;
//    _iq16 MotorAout;
//    _iq16 MotorBout;
//     _iq16 a_speed=_IQ16(0.2);
//     u16 b=30;
//     u16 ClaIntPeriod;

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

	//Timer3_Init(500,7199);
//	PWM3_Init(5,7199);// 72M/7200=10khz, 1/10khz//0.1 * 500=50ms   Ƶ��=72000 000/�� ��ARR+1����PSC+1��
//	PWM_Init(PWM3_CH1,0);//��ʼ��PWM3ͨ��1	//Ƶ��=72000 000/�� ��7199+1��*��0+1�� ����ÿ��10000��
//	PWM_Init(PWM3_CH2,0);//��ʼ��PWM3ͨ��1
//	PWM_Init(PWM3_CH3,0);//��ʼ��PWM3ͨ��1
//	PWM_Init(PWM3_CH4,0);//��ʼ��PWM3ͨ��1
//	PWM_Init(PWM1_CH2,0);
  while (1)
    {
	  PA11OUT_HIGH;
	  delay_ms(50);
	  PA10OUT_LOW;
	  delay_ms(50);

	  PA11OUT_LOW;
	  delay_ms(50);
	  PA10OUT_HIGH;
	  delay_ms(50);

//PWM_VAL=(arr_init*PWM_DUTY/10000.0);
//	  PWM_duty(PWM3_CH1,1000);//��PWM��Ϊ1000�����10000����10%;
//	  PWM_duty(PWM3_CH2,2000);//��PWM��Ϊ1000�����10000����10%;
//	  PWM_duty(PWM3_CH3,3000);//��PWM��Ϊ1000�����10000����10%;
//	  PWM_duty(PWM3_CH4,4000);//��PWM��Ϊ1000�����10000����10%;
    }
}
//
//	        Amplitude+=a_speed;//�ۻ�λ��
//	        if((Amplitude>>16)>=200*128)
//	        {
//	           Amplitude=0;//����λ������
//	            //x_turn++;
//	        }
//
//	    //TI���̣����ݵ�ǰ������ϸ�������������������ֵ����Ӧ��IQsinTable[]���±�
//	    Angle =(Amplitude>>16) & 511;
//
//	    //���������ռ�ձ�
//	    MotorAout = IQsinTable[Angle] >> 14;
//	    MotorBout = IQcosTable[Angle] >> 14;
//	    a = (MotorAout + 65536) >> 1;
//	    a = (MotorBout + 65536) >> 1;
//	    b=((u16)b*a)>>16;
//		//ClaIntPeriod =60 * 1000000 * (0.001/60);  // Prescaler X1 (T1), ISR period = T x 1
//
//		//PA10out=a;
//		TIM3->CCR4=Angle;
//		TIM3->CCR3=Angle;

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
