#ifndef ADC_H
#define ADC_H


#include "debug_frmwrk.h"

#include "lpc17xx_pinsel.h"

#define _ADC_INT			ADC_ADINTEN2
#define _ADC_CHANNEL		ADC_CHANNEL_2

extern uint8_t ADCflag;


void InitCanal(PINSEL_CFG_Type PinCfg);

void ReadVoltage(void);

//void SysTick_Handler (void);

void ReadVoltageCount(void);



#endif

