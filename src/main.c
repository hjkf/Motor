#include "stm32f10x.h"
#include "Timer.h"
#include "sys/sys.h"
#include "MOTOR/Motor00.h"



int main(void)
{
	 //�Ĵ������λ����һ�������������ٸ�ֵ���ȿ��Խ�Լ����ռ䣬���ܱ��ִ���Ŀ��Ķ���
	 //RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��
	 //RCC->APB2ENR|=1<<3;//ʹ������PORTBʱ��
	 //RCC->APB2ENR|=1<<4;//ʹ������PORTCʱ��
	 //RCC->APB2ENR |= 1<<0; //��������ʱ��

	 const u32 APB2ENR = (1<<2) | (1<<3) | (1<<4) | 1;
	// const u32 APB1ENR = (1ul << 27) | (1ul << 28); //���ڱ��ݼĴ�����ʹ��
	 RCC->APB2ENR |= APB2ENR;
	// RCC->APB1ENR |= APB1ENR;

	 //���оƬ˯����
	 GPIOC->CRH &= 0xF0FFFFFF; //����������
	 GPIOC->CRH |= 0x01000000; // PC14 ������� 1_nSLEEP

	 timer_start();
	 Motor00_Init();
	 MOTOR_WAKEUP;

	 while(1)
	 {

	 }

}
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
