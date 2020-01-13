#ifndef PORTS_H
#define PORTS_H


#include "debug_frmwrk.h"

void ReadPin(uint8_t port, uint8_t pin);
void WritePin_OnOff(uint8_t port, uint8_t pin, uint8_t data);

#endif

