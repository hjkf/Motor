/*
 * Motor.h
 *
 *  Created on: 2020��11��15��
 *      Author: HJKF
 */

#ifndef MOTOR_MOTOR00_H_
#define MOTOR_MOTOR00_H_

#include "stm32f10x.h"

#define MOTOR_WAKEUP PCout(14) = 1

void Motor00_Init();

#endif /* MOTOR_MOTOR00_H_ */
