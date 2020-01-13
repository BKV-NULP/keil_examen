#ifndef TERMINAL_H
#define TERMINAL_H


#include "debug_frmwrk.h"

extern UART_CFG_Type UARTConfigStruct;
extern uint32_t MatchCount;


extern unsigned long time;

extern uint8_t port;
extern uint8_t pin;
extern uint8_t data;
extern uint8_t dc;
extern uint8_t data_str[8];

enum State {
	HELP, 
	PINX, 
	PINXexactlyY,
	ADC,
	ADCcount,
	TIME,
	DAC,
	STORE,
	READ,
	PWMXexactlyY,
	PWMX
}; 



int legenda(void);

enum State ReadUart(void);

void help(void);

#endif
