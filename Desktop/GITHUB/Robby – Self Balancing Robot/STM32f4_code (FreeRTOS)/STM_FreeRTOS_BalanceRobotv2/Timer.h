#ifndef TIMER_H
#define TIMER_H

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
 
void TimerGPIO_Init(void);
void TIMER_Init(void);
void PWM_Init(void);
void UpdatePWMDutyCycle(int DutyCycle, int channel);
void MotorController_GPIOInit(void);
void Timer5_Init_EncodersTick(void);
void Timer2_Init_AcquisitionTick(void);
#endif
