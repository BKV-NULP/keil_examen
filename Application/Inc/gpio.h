#ifndef GPIO_H
#define GPIO_H


#include "lpc_types.h"

int readPin(uint8_t lPort, uint8_t lPin);

void writePinOnOff(uint8_t lPort, uint8_t lPin, uint8_t lData);

void initGPIO(void);
#endif

