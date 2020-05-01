/*
 * _pwm_All.h
 *
 *  Created on: 2020年5月2日
 *      Author: HJKF
 */

#ifndef PWM__PWM_ALL_H_
#define PWM__PWM_ALL_H_

#include "stm32f10x.h"
#include "../sys/sys.h"

typedef enum
{
	PWM1_CH1,//TIM1_CH1,
	PWM1_CH2,//TIM1_CH2,
	PWM1_CH3,//TIM1_CH3,
	PWM1_CH4,//TIM1_CH4,

	PWM2_CH1,//TIM2_CH1,
	PWM2_CH2,//TIM2_CH2,
	PWM2_CH3,//TIM2_CH3,
	PWM2_CH4,//TIM2_CH4,

	PWM3_CH1,//TIM3_CH1,
	PWM3_CH2,//TIM3_CH2,
	PWM3_CH3,//TIM3_CH3,
	PWM3_CH4,//TIM3_CH4,

	PWM4_CH1,//TIM4_CH1,
	PWM4_CH2,//TIM4_CH2,
	PWM4_CH3,//TIM4_CH3,
	PWM4_CH4,//TIM4_CH4,

//	PWM5_CH1,//TIM5_CH1,
//	PWM5_CH2,//TIM5_CH2,
//	PWM5_CH3,//TIM5_CH3,
//	PWM5_CH4,//TIM5_CH4,
}PWM_Ch_init;
//————————————————
//版权声明：本文为CSDN博主「_Electric_」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/zhuhao19990902/java/article/details/100808869
void PWM_Init(PWM_Ch_init n,u16 PWM_VAL);
void PWM_duty(PWM_Ch_init n,u16 PWM_DUTY);


#endif /* PWM__PWM_ALL_H_ */
