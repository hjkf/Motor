################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Timer_Break/timer1_break.c \
../src/Timer_Break/timer2.c 

OBJS += \
./src/Timer_Break/timer1_break.o \
./src/Timer_Break/timer2.o 

C_DEPS += \
./src/Timer_Break/timer1_break.d \
./src/Timer_Break/timer2.d 


# Each subdirectory must supply rules for building sources it contributes
src/Timer_Break/%.o: ../src/Timer_Break/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


