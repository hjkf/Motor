/*
 * sys.c
 *
 *  Created on: 2015年9月1日
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

	AFIO->MAPR &= 0XF8FFFFFF; //清除MAPR的[26:24]
	AFIO->MAPR |= temp;       //设置jtag模式
}

void SystemReset(void)
{
	//利用窗口看门狗立即重启。独立看门狗的等待时间比较长，不能立即重启。
	//以前用上面的代码重启没有问题，但是后来PCB改版，将LCD的屏幕复位线连接到boot引脚，而不是用GPIO引脚控制，
	//这样，如果用上面的代码复位，会导致LCD无法复位，屏幕异常
	RCC->APB1ENR |= 1<<11; //使能窗口看门狗时钟
	WWDG->CR = 0x80;       //使能窗口看门狗
}

//拉低所有端口
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
//设置向量表偏移地址
//NVIC_VectTab:基址
//Offset:偏移量
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)
{
	SCB->VTOR = NVIC_VectTab|(Offset & (u32)0x1FFFFF80);//设置NVIC的向量表偏移寄存器
	//用于标识向量表是在CODE区还是在RAM区
}

//不能在这里执行所有外设复位!否则至少引起串口不工作.
//把所有时钟寄存器复位
void MYRCC_DeInit(void)
{
 	RCC->APB1RSTR = 0x00000000;//复位结束
	RCC->APB2RSTR = 0x00000000;

  	RCC->AHBENR = 0x00000014;  //睡眠模式闪存和SRAM时钟使能.其他关闭.
  	RCC->APB2ENR = 0x00000000; //外设时钟关闭.
  	RCC->APB1ENR = 0x00000000;
	RCC->CR |= 0x00000001;     //使能内部高速时钟HSION
	RCC->CFGR &= 0xF8FF0000;   //复位SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]
	RCC->CR &= 0xFEF6FFFF;     //复位HSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;     //复位HSEBYP
	RCC->CFGR &= 0xFF80FFFF;   //复位PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE
	RCC->CIR = 0x00000000;     //关闭所有中断
	//配置向量表
#ifdef  VECT_TAB_RAM
	MY_NVIC_SetVectorTable(0x20000000, 0x0);
#else
	MY_NVIC_SetVectorTable(0x08000000,0x0);
#endif
}
//系统时钟初始化函数
//pll:选择的倍频数，从2开始，最大值为16
void Stm32_Clock_Init(u8 PLL)
{
	unsigned char temp=0;
	MYRCC_DeInit();		  //复位并配置向量表
 	RCC->CR|=0x00010000;  //外部高速时钟使能HSEON
	while(!(RCC->CR>>17));//等待外部时钟就绪
	RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
	PLL-=2;				  //抵消2个单位（因为是从2开始的，设置0就是2）
	RCC->CFGR|=PLL<<18;   //设置PLL值 2~16
	RCC->CFGR|=1<<16;	  //PLLSRC ON
	FLASH->ACR|=0x32;	  //FLASH 2个延时周期
	RCC->CR|=0x01000000;  //PLLON
	while(!(RCC->CR>>25));//等待PLL锁定
	RCC->CFGR|=0x00000002;//PLL作为系统时钟
	while(temp!=0x02)     //等待PLL作为系统时钟设置成功
	{
		temp=RCC->CFGR>>2;
		temp&=0x03;
	}
}
//设置向量表偏移地址
//NVIC_VectTab:基址
//Offset:偏移量
//void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset)
//{
//	SCB->VTOR = NVIC_VectTab|(Offset & (u32)0x1FFFFF80);//设置NVIC的向量表偏移寄存器
//	//用于标识向量表是在CODE区还是在RAM区
//}
//设置NVIC分组
//NVIC_Group:NVIC分组 0~4 总共5组
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)
{
	u32 temp,temp1;
	temp1=(~NVIC_Group)&0x07;//取后三位
	temp1<<=8;
	temp=SCB->AIRCR;  //读取先前的设置
	temp&=0X0000F8FF; //清空先前分组
	temp|=0X05FA0000; //写入钥匙
	temp|=temp1;
	SCB->AIRCR=temp;  //设置分组
}
//设置NVIC
//NVIC_PreemptionPriority:抢占优先级
//NVIC_SubPriority       :响应优先级
//NVIC_Channel           :中断编号
//NVIC_Group             :中断分组 0~4
//注意优先级不能超过设定的组的范围!否则会有意想不到的错误
//组划分:
//组0:0位抢占优先级,4位响应优先级
//组1:1位抢占优先级,3位响应优先级
//组2:2位抢占优先级,2位响应优先级
//组3:3位抢占优先级,1位响应优先级
//组4:4位抢占优先级,0位响应优先级
//NVIC_SubPriority和NVIC_PreemptionPriority的原则是,数值越小,越优先
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)
{
	u32 temp;
	MY_NVIC_PriorityGroupConfig(NVIC_Group);//设置分组
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;//取低四位
	NVIC->ISER[NVIC_Channel/32]|=(1<<NVIC_Channel%32);//使能中断位(要清除的话,相反操作就OK)
	NVIC->IP[NVIC_Channel]|=temp<<4;//设置响应优先级和抢断优先级
}
