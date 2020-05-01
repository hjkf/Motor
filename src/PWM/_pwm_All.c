/*
 * pwm_All.c
 *
 *  Created on: 2020年5月2日
 *      Author: HJKF
 */
#include "_pwm_All.h"
#include "stm32f10x.h"

/*
此库可以支持的PWM引脚通道对照表：
通道名       对应引脚		复用引脚
PWM1_CH1     PA8			PE9
PWM1_CH2	 PA9			PE11
PWM1_CH3	 PA10			PE13
PWM1_CH4	 PA11			PE14

PWM2_CH1     PA0            PA15
PWM2_CH2	 PA1			PB3
PWM2_CH3	 PA2			PB10
PWM2_CH4	 PA3			PB11

PWM3_CH1 	 PA6			PB4
PWM3_CH2	 PA7			PB5
PWM3_CH3	 PB0			PC8
PWM4_CH4	 PB1			PC9

PWM4_CH1	 PB6			PD12
PWM4_CH2	 PB7			PD13
PWM4_CH3	 PB8			PD14
PWM4_CH4	 PB9			PD15
*/

#define arr_init 7199     //频率=72000 000/（ （7199+1）*（0+1） ），每秒10000次
#define psc_init 500

/*
函数名：PWM初始化函数
参数：一：PWM通道
	 二：初始PWM值
时间：2019.8.14
例：PWM_Init(PWM1_CH1,0);
注：本pwm初始化库，可以控制TIM1~TIM4定时共输出16路定时器
TIM1->CCMR1|=7<<4;  	//CH1 PWM2模式适用于led
*/
void PWM_Init(PWM_Ch_init n,u16 PWM_VAL)
{
	switch(n)
	{
		case PWM1_CH1://PA8
		{
			RCC->APB2ENR|=1<<11; 	//TIM1时钟使能
			RCC->APB2ENR|=1<<2; //GPIOA时钟使能
			GPIOA->CRH&=0XFFFFFFF0;	//PA8清除之前的设置
			GPIOA->CRH|=0X0000000B;	//复用功能输出

			TIM1->ARR=arr_init;			//设定计数器自动重装值
			TIM1->PSC=psc_init;			//预分频器设置

			TIM1->CCMR1|=6<<4;  	//CH1 PWM1模式
			TIM1->CCMR1|=1<<3; 		//CH1预装载使能
			TIM1->CCER|=1<<0;   	//OC1 输出使能
				TIM1->BDTR|=1<<15;   	//MOE 主输出使能

			TIM1->CR1=0x0080;   	//ARPE使能
			TIM1->CR1|=0x01;    	//使能定时器1

			TIM1->CCR1 =PWM_VAL;

			break;
		}
		case PWM1_CH2://PA9
		{
			RCC->APB2ENR|=1<<11; 	//TIM1时钟使能
			RCC->APB2ENR|=1<<2; //GPIOA时钟使能
			GPIOA->CRH&=0XFFFFFF0F;	//PA9清除之前的设置
			GPIOA->CRH|=0X000000B0;	//复用功能输出

			TIM1->ARR=arr_init;			//设定计数器自动重装值
			TIM1->PSC=psc_init;			//预分频器设置

			TIM1->CCMR1|=6<<12;//CH2 PWM1模式
			TIM1->CCMR1|=1<<11; //CH2预装载使能
			TIM1->CCER|=1<<4; //CH2输出使能
				TIM1->BDTR |= 1<<15;//TIM1必须要这句话才能输出PWM

			TIM1->CR1=0x80; //ARPE使能
			TIM1->CR1|=0x01; //使能定时器2

			TIM1->CCR2 =PWM_VAL;

			break;
		}
		case PWM1_CH3://PA10
		{
			RCC->APB2ENR|=1<<11; 	//TIM1时钟使能
			RCC->APB2ENR|=1<<2; //GPIOA时钟使能
			GPIOA->CRH&=0XFFFFF0FF;	//PA10清除之前的设置
			GPIOA->CRH|=0X00000B00;	//复用功能输出

			TIM1->ARR=arr_init;			//设定计数器自动重装值
			TIM1->PSC=psc_init;			//预分频器设置

			TIM1->CCMR2|=6<<4;//CH2 PWM1模式
			TIM1->CCMR2|=1<<3; //CH2预装载使能
			TIM1->CCER|=1<<8; //CH2输出使能
				TIM1->BDTR |= 1<<15;//TIM1必须要这句话才能输出PWM

			TIM1->CR1=0x80; //ARPE使能
			TIM1->CR1|=0x01; //使能定时器2

			TIM1->CCR3 =PWM_VAL;

			break;
		}
		case PWM1_CH4://PA11
		{
			RCC->APB2ENR|=1<<11; 	//TIM1时钟使能
			RCC->APB2ENR|=1<<2; //GPIOA时钟使能
			GPIOA->CRH&=0XFFFF0FFF;	//PA10清除之前的设置
			GPIOA->CRH|=0X0000B000;	//复用功能输出

			TIM1->ARR=arr_init;			//设定计数器自动重装值
			TIM1->PSC=psc_init;			//预分频器设置

			TIM1->CCMR2|=6<<12;//CH2 PWM1模式
			TIM1->CCMR2|=1<<11; //CH2预装载使能
			TIM1->CCER|=1<<12; //CH2输出使能
				TIM1->BDTR |= 1<<15;//TIM1必须要这句话才能输出PWM

			TIM1->CR1=0x80; //ARPE使能
			TIM1->CR1|=0x01; //使能定时器2

			TIM1->CCR3 =PWM_VAL;

			break;
		}
		////////////////////////////////////////////////////////////////////////
		case PWM2_CH1://PA0
		{
			RCC->APB1ENR|=1; //使能TIM2时钟
			RCC->APB2ENR|=1<<2; //GPIOA时钟使能
			GPIOA->CRL&=0XFFFFFFF0; //PA 0 复位
			GPIOA->CRL|=0X0000000B; //PA 0 复用输出

			TIM2->ARR=arr_init;//设定计数器自动重装值
			TIM2->PSC=psc_init; //预分频器不分频

			TIM2->CCMR1|=6<<4; //CH1 PWM1模式
			TIM2->CCMR1|=1<<3; //CH1预装载使能
			TIM2->CCER|=1<<0; //CH1输出使能

			TIM2->CR1=0x80; //ARPE使能
			TIM2->CR1|=0x01; //使能定时器2

			TIM2->CCR1 =PWM_VAL;
			break;
		}
		case PWM2_CH2://PA1
		{
			RCC->APB1ENR|=1; //使能TIM2时钟
			RCC->APB2ENR|=1<<2; //GPIOA时钟使能
			GPIOA->CRL&=0XFFFFFF0F; //PA  1复位
			GPIOA->CRL|=0X000000B0; //PA  1复用输出

			TIM2->ARR=arr_init;//设定计数器自动重装值
			TIM2->PSC=psc_init; //预分频器不分频

			TIM2->CCMR1|=6<<12;//CH2 PWM1模式
			TIM2->CCMR1|=1<<11; //CH2预装载使能
			TIM2->CCER|=1<<4; //CH2输出使能

			TIM2->CR1=0x80; //ARPE使能
			TIM2->CR1|=0x01; //使能定时器2

			TIM2->CCR2 =PWM_VAL;
			break;
		}
		case PWM2_CH3://PA2
		{
			RCC->APB1ENR|=1; //使能TIM2时钟
			RCC->APB2ENR|=1<<2; //GPIOA时钟使能
			GPIOA->CRL&=0XFFFFF0FF;	//PA2清除之前的设置
			GPIOA->CRL|=0X00000B00;	//复用功能输出

			TIM2->ARR=arr_init;			//设定计数器自动重装值
			TIM2->PSC=psc_init;			//预分频器设置

			TIM2->CCMR2|=6<<4;//CH3 PWM1模式
			TIM2->CCMR2|=1<<3; //CH3预装载使能
			TIM2->CCER|=1<<8; //CH3输出使能

			TIM2->CR1=0x80; //ARPE使能
			TIM2->CR1|=0x01; //使能定时器2

			TIM2->CCR3 =PWM_VAL;

			break;
		}
		case PWM2_CH4://PA3
		{
			RCC->APB1ENR|=1; //使能TIM2时钟
			RCC->APB2ENR|=1<<2; //GPIOA时钟使能
			GPIOA->CRL&=0XFFFF0FFF;	//PA3清除之前的设置
			GPIOA->CRL|=0X0000B000;	//复用功能输出

			TIM2->ARR=arr_init;			//设定计数器自动重装值
			TIM2->PSC=psc_init;			//预分频器设置

			TIM2->CCMR2|=6<<12;//CH4 PWM1模式
			TIM2->CCMR2|=1<<11; //CH4预装载使能
			TIM2->CCER|=1<<12; //CH4输出使能

			TIM2->CR1=0x80; //ARPE使能
			TIM2->CR1|=0x01; //使能定时器2

			TIM2->CCR4 =PWM_VAL;

			break;
		}
		////////////////////////////////
		case PWM3_CH1://PA6
		{
			RCC->APB1ENR|=1<<1; //使能TIM3时钟
			RCC->APB2ENR|=1<<2; //GPIOA时钟使能
			GPIOA->CRL&=0XF0FFFFFF; //PA6复位
			GPIOA->CRL|=0X0B000000; //PA6 复用输出

			TIM3->ARR=arr_init;//设定计数器自动重装值
			TIM3->PSC=psc_init; //预分频器不分频

			TIM3->CCMR1|=6<<4; //CH1 PWM1模式
			TIM3->CCMR1|=1<<3; //CH1预装载使能
			TIM3->CCER|=1<<0; //CH1输出使能

			TIM3->CR1=0x80; //ARPE使能
			TIM3->CR1|=0x01; //使能定时器2

			TIM3->CCR1 =PWM_VAL;

			break;
		}
		case PWM3_CH2://PA7
		{
			RCC->APB1ENR|=1<<1; //使能TIM3时钟
			RCC->APB2ENR|=1<<2; //GPIOA时钟使能
			GPIOA->CRL&=0X0FFFFFFF; //PA7 复位
			GPIOA->CRL|=0XB0000000; //PA7 复用输出

			TIM3->ARR=arr_init;//设定计数器自动重装值
			TIM3->PSC=psc_init; //预分频器不分频

			TIM3->CCMR1|=6<<12;//CH2 PWM1模式
			TIM3->CCMR1|=1<<11; //CH2预装载使能
			TIM3->CCER|=1<<4; //CH2输出使能

			TIM3->CR1=0x80; //ARPE使能
			TIM3->CR1|=0x01; //使能定时器2

			TIM3->CCR2 =PWM_VAL;

			break;
		}
		case PWM3_CH3://PB0
		{
			RCC->APB1ENR|=1<<1; //使能TIM3时钟
			RCC->APB2ENR|=1<<3;
			GPIOB->CRL&=0XFFFFFFF0;	//PB0清除之前的设置
			GPIOB->CRL|=0X0000000B;	//复用功能输出

			TIM3->ARR=arr_init;			//设定计数器自动重装值
			TIM3->PSC=psc_init;			//预分频器设置

			TIM3->CCMR2|=6<<4;//CH3 PWM1模式
			TIM3->CCMR2|=1<<3; //CH3预装载使能
			TIM3->CCER|=1<<8; //CH3输出使能

			TIM3->CR1=0x80; //ARPE使能
			TIM3->CR1|=0x01; //使能定时器2

			TIM3->CCR3 =PWM_VAL;

			break;
		}
		case PWM3_CH4://PB1
		{
			RCC->APB1ENR|=1<<1; //使能TIM3时钟
			RCC->APB2ENR|=1<<3;
			GPIOB->CRL&=0XFFFFFF0F;	//PB1清除之前的设置
			GPIOB->CRL|=0X000000B0;	//复用功能输出

			TIM3->ARR=arr_init;			//设定计数器自动重装值
			TIM3->PSC=psc_init;			//预分频器设置

			TIM3->CCMR2|=6<<12;//CH4 PWM1模式
			TIM3->CCMR2|=1<<11; //CH4预装载使能
			TIM3->CCER|=1<<12; //CH4输出使能

			TIM3->CR1=0x80; //ARPE使能
			TIM3->CR1|=0x01; //使能定时器2

			TIM3->CCR4 =PWM_VAL;

			break;
		}

		///////////////////////////////////
		case PWM4_CH1://PB6
		{
			RCC->APB1ENR|=1<<2; //使能TIM4时钟
			RCC->APB2ENR|=1<<3; //GPIOB时钟使能
			GPIOB->CRL&=0XF0FFFFFF; //PB6复位
			GPIOB->CRL|=0X0B000000; //PB6 复用输出

			TIM4->ARR=arr_init;//设定计数器自动重装值
			TIM4->PSC=psc_init; //预分频器不分频

			TIM4->CCMR1|=6<<4; //CH1 PWM1模式
			TIM4->CCMR1|=1<<3; //CH1预装载使能
			TIM4->CCER|=1<<0; //CH1输出使能

			TIM4->CR1=0x80; //ARPE使能
			TIM4->CR1|=0x01; //使能定时器2

			TIM4->CCR1 =PWM_VAL;

			break;
		}
		case PWM4_CH2://PB7
		{
			RCC->APB1ENR|=1<<2; //使能TIM4时钟
			RCC->APB2ENR|=1<<3; //GPIOB时钟使能
			GPIOB->CRL&=0X0FFFFFFF; //PB7复位
			GPIOB->CRL|=0XB0000000; //PB7 复用输出

			TIM4->ARR=arr_init;//设定计数器自动重装值
			TIM4->PSC=psc_init; //预分频器不分频

			TIM4->CCMR1|=6<<12;//CH2 PWM1模式
			TIM4->CCMR1|=1<<11; //CH2预装载使能
			TIM4->CCER|=1<<4; //CH2输出使能

			TIM4->CR1=0x80; //ARPE使能
			TIM4->CR1|=0x01; //使能定时器2

			TIM4->CCR2 =PWM_VAL;

			break;
		}
		case PWM4_CH3://PB8
		{
			RCC->APB1ENR|=1<<2; //使能TIM4时钟
			RCC->APB2ENR|=1<<3; //GPIOB时钟使能
			GPIOB->CRH&=0XFFFFFFF0; //PB8复位
			GPIOB->CRH|=0X0000000B; //PB8 复用输出

			TIM4->ARR=arr_init;//设定计数器自动重装值
			TIM4->PSC=psc_init; //预分频器不分频

			TIM4->CCMR2|=6<<4;//CH3 PWM1模式
			TIM4->CCMR2|=1<<3; //CH3预装载使能
			TIM4->CCER|=1<<8; //CH3输出使能

			TIM4->CR1=0x80; //ARPE使能
			TIM4->CR1|=0x01; //使能定时器2

			TIM4->CCR3 =PWM_VAL;

			break;
		}
		case PWM4_CH4://PB9
		{
			RCC->APB1ENR|=1<<2; //使能TIM4时钟
			RCC->APB2ENR|=1<<3; //GPIOB时钟使能
			GPIOB->CRH&=0XFFFFFF0F; //PB9复位
			GPIOB->CRH|=0X000000B0; //PB9 复用输出

			TIM4->ARR=arr_init;//设定计数器自动重装值
			TIM4->PSC=psc_init; //预分频器不分频

			TIM4->CCMR2|=6<<12;//CH4 PWM1模式
			TIM4->CCMR2|=1<<11; //CH4预装载使能
			TIM4->CCER|=1<<12; //CH4输出使能

			TIM4->CR1=0x80; //ARPE使能
			TIM4->CR1|=0x01; //使能定时器2

			TIM4->CCR4 =PWM_VAL;

			break;
		}
	}
}
/*
函数名:占空比设置函数
参数：一：PWM通道
	 二：占空比大小（0~10000）
时间：2019.8.14
例：PWM_duty(PWM1_CH1,1000);
*/
void PWM_duty(PWM_Ch_init n,u16 PWM_DUTY)
{
	float PWM_VAL;

	PWM_VAL=(arr_init*PWM_DUTY/10000.0);
	switch(n)
	{
		case PWM1_CH1://PA8
		{
			TIM1->CCR1 =PWM_VAL;
			break;
		}
		case PWM1_CH2://PA9
		{
			TIM1->CCR2 =PWM_VAL;
			break;
		}
		case PWM1_CH3://PA10
		{
			TIM1->CCR3 =PWM_VAL;
			break;
		}
		case PWM1_CH4://PA11
		{
			TIM1->CCR4 =PWM_VAL;
			break;
		}

		case PWM2_CH1://PA0
		{
			TIM2->CCR1 =PWM_VAL;
			break;
		}
		case PWM2_CH2://PA1
		{
			TIM2->CCR2 =PWM_VAL;
			break;
		}
		case PWM2_CH3://PA2
		{
			TIM2->CCR3 =PWM_VAL;
			break;
		}
		case PWM2_CH4://PA3
		{
			TIM2->CCR4 =PWM_VAL;
			break;
		}

		case PWM3_CH1://PA6
		{
			TIM3->CCR1 =PWM_VAL;
			break;
		}
		case PWM3_CH2://PA7
		{
			TIM3->CCR2 =PWM_VAL;
			break;
		}
		case PWM3_CH3://PB0
		{
			TIM3->CCR3 =PWM_VAL;
			break;
		}
		case PWM3_CH4://PB1
		{
			TIM3->CCR4 =PWM_VAL;
			break;
		}

		case PWM4_CH1://PB6
		{
			TIM4->CCR1 =PWM_VAL;
			break;
		}
		case PWM4_CH2://PB7
		{
			TIM4->CCR2 =PWM_VAL;
			break;
		}
		case PWM4_CH3://PB8
		{
			TIM4->CCR3 =PWM_VAL;
			break;
		}
		case PWM4_CH4://PB9
		{
			TIM4->CCR4 =PWM_VAL;
			break;
		}
	}
}
//————————————————
//版权声明：本文为CSDN博主「_Electric_」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/zhuhao19990902/java/article/details/100808869/*
//函数名:占空比设置函数
//参数：一：PWM通道
//	 二：占空比大小（0~10000）
//时间：2019.8.14
//例：PWM_duty(PWM1_CH1,1000);
//*/
