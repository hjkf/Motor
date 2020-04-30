/*
 * sys.c
 *
 *  Created on: 2015��9��1��
 *      Author: yhh
 */

#include "stm32f10x.h"
#include "sys.h"
#include "stdbool.h"

void JTAG_Set(u8 mode)
{
	u32 temp;
	temp = mode;
	temp <<= 25;

	AFIO->MAPR &= 0XF8FFFFFF; //���MAPR��[26:24]
	AFIO->MAPR |= temp;       //����jtagģʽ
}

void SystemReset(void)
{
	//���ô��ڿ��Ź������������������Ź��ĵȴ�ʱ��Ƚϳ�����������������
	//��ǰ������Ĵ�������û�����⣬���Ǻ���PCB�İ棬��LCD����Ļ��λ�����ӵ�boot���ţ���������GPIO���ſ��ƣ�
	//���������������Ĵ��븴λ���ᵼ��LCD�޷���λ����Ļ�쳣
	RCC->APB1ENR |= 1<<11; //ʹ�ܴ��ڿ��Ź�ʱ��
	WWDG->CR = 0x80;       //ʹ�ܴ��ڿ��Ź�
}

//�������ж˿�
void PullAllIO(bool value)
{
	for(u8 i = 0; i<16; i++)
	{
		PAout(i) = value;
		PBout(i) = value;
		PCout(i) = value;
		PDout(i) = value;
		PEout(i) = value;
		PFout(i) = value;
		PGout(i) = value;
	}
}
//����������ƫ�Ƶ�ַ
//NVIC_VectTab:��ַ
//Offset:ƫ����
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)
{
	SCB->VTOR = NVIC_VectTab|(Offset & (u32)0x1FFFFF80);//����NVIC��������ƫ�ƼĴ���
	//���ڱ�ʶ����������CODE��������RAM��
}

//����������ִ���������踴λ!�����������𴮿ڲ�����.
//������ʱ�ӼĴ�����λ
void MYRCC_DeInit(void)
{
 	RCC->APB1RSTR = 0x00000000;//��λ����
	RCC->APB2RSTR = 0x00000000;

  	RCC->AHBENR = 0x00000014;  //˯��ģʽ�����SRAMʱ��ʹ��.�����ر�.
  	RCC->APB2ENR = 0x00000000; //����ʱ�ӹر�.
  	RCC->APB1ENR = 0x00000000;
	RCC->CR |= 0x00000001;     //ʹ���ڲ�����ʱ��HSION
	RCC->CFGR &= 0xF8FF0000;   //��λSW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]
	RCC->CR &= 0xFEF6FFFF;     //��λHSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;     //��λHSEBYP
	RCC->CFGR &= 0xFF80FFFF;   //��λPLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE
	RCC->CIR = 0x00000000;     //�ر������ж�
	//����������
#ifdef  VECT_TAB_RAM
	MY_NVIC_SetVectorTable(0x20000000, 0x0);
#else
	MY_NVIC_SetVectorTable(0x08000000,0x0);
#endif
}
//ϵͳʱ�ӳ�ʼ������
//pll:ѡ��ı�Ƶ������2��ʼ�����ֵΪ16
void Stm32_Clock_Init(u8 PLL)
{
	unsigned char temp=0;
	MYRCC_DeInit();		  //��λ������������
 	RCC->CR|=0x00010000;  //�ⲿ����ʱ��ʹ��HSEON
	while(!(RCC->CR>>17));//�ȴ��ⲿʱ�Ӿ���
	RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
	PLL-=2;				  //����2����λ����Ϊ�Ǵ�2��ʼ�ģ�����0����2��
	RCC->CFGR|=PLL<<18;   //����PLLֵ 2~16
	RCC->CFGR|=1<<16;	  //PLLSRC ON
	FLASH->ACR|=0x32;	  //FLASH 2����ʱ����
	RCC->CR|=0x01000000;  //PLLON
	while(!(RCC->CR>>25));//�ȴ�PLL����
	RCC->CFGR|=0x00000002;//PLL��Ϊϵͳʱ��
	while(temp!=0x02)     //�ȴ�PLL��Ϊϵͳʱ�����óɹ�
	{
		temp=RCC->CFGR>>2;
		temp&=0x03;
	}
}
//����������ƫ�Ƶ�ַ
//NVIC_VectTab:��ַ
//Offset:ƫ����
//void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)
//{
//	SCB->VTOR = NVIC_VectTab|(Offset & (u32)0x1FFFFF80);//����NVIC��������ƫ�ƼĴ���
//	//���ڱ�ʶ����������CODE��������RAM��
//}
//����NVIC����
//NVIC_Group:NVIC���� 0~4 �ܹ�5��
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)
{
	u32 temp,temp1;
	temp1=(~NVIC_Group)&0x07;//ȡ����λ
	temp1<<=8;
	temp=SCB->AIRCR;  //��ȡ��ǰ������
	temp&=0X0000F8FF; //�����ǰ����
	temp|=0X05FA0000; //д��Կ��
	temp|=temp1;
	SCB->AIRCR=temp;  //���÷���
}
//����NVIC
//NVIC_PreemptionPriority:��ռ���ȼ�
//NVIC_SubPriority       :��Ӧ���ȼ�
//NVIC_Channel           :�жϱ��
//NVIC_Group             :�жϷ��� 0~4
//ע�����ȼ����ܳ����趨����ķ�Χ!����������벻���Ĵ���
//�黮��:
//��0:0λ��ռ���ȼ�,4λ��Ӧ���ȼ�
//��1:1λ��ռ���ȼ�,3λ��Ӧ���ȼ�
//��2:2λ��ռ���ȼ�,2λ��Ӧ���ȼ�
//��3:3λ��ռ���ȼ�,1λ��Ӧ���ȼ�
//��4:4λ��ռ���ȼ�,0λ��Ӧ���ȼ�
//NVIC_SubPriority��NVIC_PreemptionPriority��ԭ����,��ֵԽС,Խ����
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)
{
	u32 temp;
	MY_NVIC_PriorityGroupConfig(NVIC_Group);//���÷���
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;//ȡ����λ
	NVIC->ISER[NVIC_Channel/32]|=(1<<NVIC_Channel%32);//ʹ���ж�λ(Ҫ����Ļ�,�෴������OK)
	NVIC->IP[NVIC_Channel]|=temp<<4;//������Ӧ���ȼ����������ȼ�
}
