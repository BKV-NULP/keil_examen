#ifndef ADC_H
#define ADC_H


#include "lpc_types.h"

#include "lpc17xx_pinsel.h"

#define _ADC_INT			ADC_ADINTEN2
#define _ADC_CHANNEL		ADC_CHANNEL_2


void initCanalADC(PINSEL_CFG_Type lPinCfg);

float readVoltageADC(void);

#endif

