//
// This file is part of the GNU ARM Eclipse Plug-ins project.
// Copyright (c) 2014 Liviu Ionescu.
//

#include "Timer.h"
#include "cortexm/ExceptionHandlers.h"

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
volatile u32 TIMER_COUNTER_32BIT = 0xFFFFFFFF;

// ----------------------------------------------------------------------------

const u8  delay_par = 		(1.6*((72000000.0/8000000.0)));


void timer_start (void)
{
  // Use SysTick as reference for the delay loops.
  SysTick_Config (SystemCoreClock / TIMER_FREQUENCY_HZ);
}

void delay_ms(u32 x)
{
	volatile u32 _dcnt = x * (u32)delay_par * 1000ul;

	while(--_dcnt>0);

}

void delay_us(u32 x)
{
	volatile u32 _dcnt = x * (u32)delay_par;

	while(--_dcnt>0);
}

// ----- SysTick_Handler() ----------------------------------------------------

void SysTick_Handler (void)
{
#if defined(USE_HAL_DRIVER)
  HAL_IncTick();
#endif

  TIMER_COUNTER_32BIT --;

}

// ----------------------------------------------------------------------------
