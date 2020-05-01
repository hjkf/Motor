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
	//寄存器多个位先用一个变量合起来再赋值，既可以节约代码空间，又能保持代码的可阅读性
	//RCC->APB2ENR|=1<<2;   //使能PORTA口时钟
	//RCC->APB2ENR|=1<<3;//使能外设PORTB时钟
	//RCC->APB2ENR|=1<<4;//使能外设PORTC时钟
	//RCC->APB2ENR|=1<<5;//使能外设PORTD时钟
	//RCC->APB2ENR |= 1<<0; //开启辅助时钟
//	bool falg;
////    static bool dir=true;
//    static int x_turn=0;//正转半圈的变量
////    static int y_turn=0;//反转半圈的变量
//    static _iq16 Amplitude=0;//当前的振幅0
//    static u16 Angle;//电流系数
//    static _iq16 a;
//    _iq16 MotorAout;
//    _iq16 MotorBout;
//     _iq16 a_speed=_IQ16(0.2);
//     u16 b=30;
//     u16 ClaIntPeriod;

	Stm32_Clock_Init(9); //系统时钟设置

	const u32 APB2ENR = (1<<2) | (1<<3) | (1<<4) | (1<<5) | 1;
	const u32 APB1ENR = (1ul << 27) | (1ul << 28); //关于备份寄存器的使能
	RCC->APB2ENR |= APB2ENR;
	RCC->APB1ENR |= APB1ENR;

	GPIOC->CRH&=0X0FFFFFFF;
	GPIOC->CRH|=0X30000000;//PA10 11 推挽输出

	motor_init();//初始化端口
	PB12OUT_LOW;//电机使能线拉高
	motor_init();

	//Timer3_Init(500,7199);
//	PWM3_Init(5,7199);// 72M/7200=10khz, 1/10khz//0.1 * 500=50ms   频率=72000 000/（ （ARR+1）（PSC+1）
//	PWM_Init(PWM3_CH1,0);//初始化PWM3通道1	//频率=72000 000/（ （7199+1）*（0+1） ），每秒10000次
//	PWM_Init(PWM3_CH2,0);//初始化PWM3通道1
//	PWM_Init(PWM3_CH3,0);//初始化PWM3通道1
//	PWM_Init(PWM3_CH4,0);//初始化PWM3通道1
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
//	  PWM_duty(PWM3_CH1,1000);//将PWM设为1000，最大10000，即10%;
//	  PWM_duty(PWM3_CH2,2000);//将PWM设为1000，最大10000，即10%;
//	  PWM_duty(PWM3_CH3,3000);//将PWM设为1000，最大10000，即10%;
//	  PWM_duty(PWM3_CH4,4000);//将PWM设为1000，最大10000，即10%;
    }
}
//
//	        Amplitude+=a_speed;//累积位移
//	        if((Amplitude>>16)>=200*128)
//	        {
//	           Amplitude=0;//弧度位移清零
//	            //x_turn++;
//	        }
//
//	    //TI例程，根据当前步数和细分数，换算两相电流的值，对应到IQsinTable[]的下标
//	    Angle =(Amplitude>>16) & 511;
//
//	    //两相电流的占空比
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
