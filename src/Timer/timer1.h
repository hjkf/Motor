/*
 * timer1_brak.h
 *
 *  Created on: 2020��4��29��
 *      Author: HSD
 */

#ifndef TIMER_TIMER1_H_
#define TIMER_TIMER1_H_
#include "../sys/sys.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//ͨ�ö�ʱ�� ��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/06
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.1 20140306
//����TIM1_CH1��PWM��������������
//////////////////////////////////////////////////////////////////////////////////

//ͨ���ı�TIM1->CCR1��ֵ���ı�ռ�ձȣ��Ӷ�����LED0������
//#define PWM1_CH1 TIM1->CCR1
//#define PWM1_CH2 TIM1->CCR2
//#define PWM1_CH3 TIM1->CCR3
//#define PWM1_CH4 TIM1->CCR4

void Timer1_Init(u16 arr,u16 psc);
void PWM1_Init(u16 arr,u16 psc);

#endif /* TIMER_TIMER1_H_ */
