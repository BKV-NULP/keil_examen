#ifndef PORTS_H
#define PORTS_H


#include "lpc_types.h"

int ReadPin(uint8_t port, uint8_t pin);
void WritePin_OnOff(uint8_t port, uint8_t pin, uint8_t data);
void GPIO_init(void);
#endif

