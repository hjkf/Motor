/*
 * timer1_brak.h
 *
 *  Created on: 2020年4月29日
 *      Author: HSD
 */

#ifndef TIMER_TIMER1_H_
#define TIMER_TIMER1_H_
#include "../sys/sys.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//通用定时器 驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/06
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.1 20140306
//增加TIM1_CH1，PWM输出设置相关内容
//////////////////////////////////////////////////////////////////////////////////

//通过改变TIM1->CCR1的值来改变占空比，从而控制LED0的亮度
//#define PWM1_CH1 TIM1->CCR1
//#define PWM1_CH2 TIM1->CCR2
//#define PWM1_CH3 TIM1->CCR3
//#define PWM1_CH4 TIM1->CCR4

void Timer1_Init(u16 arr,u16 psc);
void PWM1_Init(u16 arr,u16 psc);

#endif /* TIMER_TIMER1_H_ */
