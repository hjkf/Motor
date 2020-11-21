/*
 * Motor.h
 *
 *  Created on: 2020年11月15日
 *      Author: HJKF
 */

#ifndef MOTOR_MOTOR00_H_
#define MOTOR_MOTOR00_H_

#include "stm32f10x.h"

#define MOTOR_ARR 4500	//设定计数器自动重装值，PWM频率 = 72000 / (TIM3->ARR+1)	kHz
#define MOTOR_PSC 0

#define MOTOR_PWM_A TIM1->CCR1
#define MOTOR_PWM_B TIM1->CCR2
#define MOTOR_PWM_C TIM1->CCR3
#define MOTOR_PWM_D TIM1->CCR4

#define IREF_MAX 0.2//最大电流
#define IREF_K_DEFAULT 0.000005
#define IREF_C_DEFAUL 0.02


#define MOTOR_WAKEUP PCout(14) = 1
#define MOTOR_SLEEP PCout(14) = 0


void Motor00_Init();

#endif /* MOTOR_MOTOR00_H_ */
