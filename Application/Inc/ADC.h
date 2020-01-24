#ifndef ADC_H
#define ADC_H


#include "lpc_types.h"

#include "lpc17xx_pinsel.h"

#define _ADC_INT			ADC_ADINTEN2
#define _ADC_CHANNEL		ADC_CHANNEL_2


void InitCanal(PINSEL_CFG_Type PinCfg);

float ReadVoltage(void);

#endif

