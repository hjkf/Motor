/*
 * Motor.h
 *
 *  Created on: 2020Äê11ÔÂ15ÈÕ
 *      Author: HJKF
 */

#ifndef MOTOR_MOTOR00_H_
#define MOTOR_MOTOR00_H_

#include "stm32f10x.h"

#define MOTOR_WAKEUP PCout(14) = 1
#define MOTOR_Sleep PCout(14) = 0
//#define cosf((2*PI)/3) (0.86602540378443864676372317075294)
//#define SIN240 (-0.86602540378443864676372317075294)
//#define COS120 (-0.5)
//#define COS240 (-0.5)


void Motor00_Init();

#endif /* MOTOR_MOTOR00_H_ */
