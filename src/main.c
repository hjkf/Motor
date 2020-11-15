#include "stm32f10x.h"
#include "Timer.h"
#include "sys/sys.h"
#include "MOTOR/Motor00.h"



int main(void)
{
	 //寄存器多个位先用一个变量合起来再赋值，既可以节约代码空间，又能保持代码的可阅读性
	 //RCC->APB2ENR|=1<<2;   //使能PORTA口时钟
	 //RCC->APB2ENR|=1<<3;//使能外设PORTB时钟
	 //RCC->APB2ENR|=1<<4;//使能外设PORTC时钟
	 //RCC->APB2ENR |= 1<<0; //开启辅助时钟

	 const u32 APB2ENR = (1<<2) | (1<<3) | (1<<4) | 1;
	// const u32 APB1ENR = (1ul << 27) | (1ul << 28); //关于备份寄存器的使能
	 RCC->APB2ENR |= APB2ENR;
	// RCC->APB1ENR |= APB1ENR;

	 //电机芯片睡眠线
	 GPIOC->CRH &= 0xF0FFFFFF; //按键口配置
	 GPIOC->CRH |= 0x01000000; // PC14 推挽输出 1_nSLEEP

	 timer_start();
	 Motor00_Init();
	 MOTOR_WAKEUP;

	 while(1)
	 {

	 }

}
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
