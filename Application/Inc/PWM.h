#ifndef PWM_H
#define PWM_H

#include "lpc_types.h"

#define cQuantityScalePWM 256

void readPinPWM(uint16_t num, uint16_t perc);
int readDCPinPWM(uint8_t pin_num);
#endif

