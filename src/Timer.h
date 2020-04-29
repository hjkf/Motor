//
// This file is part of the GNU ARM Eclipse Plug-ins project.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef TIMER_H_
#define TIMER_H_

#include "cmsis_device.h"

// ----------------------------------------------------------------------------

#define TIMER_FREQUENCY_HZ (1000u)



extern void delay_ms(u32 x);
extern void delay_us(u32 x);



extern volatile u32 TIMER_COUNTER_32BIT;

extern void timer_start (void);

// ----------------------------------------------------------------------------

#endif // TIMER_H_
