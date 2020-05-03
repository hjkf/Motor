#include "stm32f10x.h"
#include "sys/sys.h"
#include "_Global.h"
#include "Motor/Motor1.h"
#include "Timer/timer1.h"
#include "Timer/timer3.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

//extern _iq IQsinTable[];
//extern _iq IQcosTable[];
static _iq16 Amplitude=0;//��ǰ�����0
static u16 Angle;//����ϵ��
_iq16 a_speed=0.2;
_iq16 MotorAout;
_iq16 MotorBout;
static _iq16 a;


int main(int argc, char* argv[])
{

//    static _iq16 Amplitude=0;//��ǰ�����0
//    static u16 Angle;//����ϵ��
//    static _iq16 a;
//    static _iq16 b;
//    int kk;
//    _iq16 MotorAout;
//    _iq16 MotorBout;
//     _iq16 a_speed=_IQ16(0.2);
//     uint16_t ClaIntPeriod=100;

	Stm32_Clock_Init(9); //ϵͳʱ������

	const u32 APB2ENR = (1<<2) | (1<<3) | (1<<4) | (1<<5) | 1;
	const u32 APB1ENR = (1ul << 27) | (1ul << 28); //���ڱ��ݼĴ�����ʹ��
	RCC->APB2ENR |= APB2ENR;
	RCC->APB1ENR |= APB1ENR;

	led_Init();
	motor1_init();//��ʼ���˿�
	PB12OUT_LOW;//���ʹ��������

//	Timer3_Init(500,7199);
	PWM3_Init(7199);// 72M/7200=10khz, 1/10khz//0.1 * 500=50ms   Ƶ��=72000 000/�� ��ARR+1����PSC+1��

  while (1)
    {
      Amplitude+=a_speed;//�ۻ�λ��
       if((Amplitude>>16)>=200*128)
       {
           Amplitude=0;//����λ������
       }
       //TI���̣����ݵ�ǰ������ϸ�������������������ֵ����Ӧ��IQsinTable[]���±�
       Angle =(Amplitude>>16) & 511;

       //���������ռ�ձ�
       MotorAout = (u16)IQsinTable[Angle] >> 16;
       MotorBout = (u16)IQcosTable[Angle] >> 16;

       a = (MotorAout+ 65536) >> 1;
       TIM3->CCR3=(1000*a)>>16;
       TIM3->CCR4=(1000*a)>>16;

    }
}


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
