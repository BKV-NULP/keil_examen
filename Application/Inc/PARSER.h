#ifndef PARSER_H
#define PARSER_H

#include "lpc_types.h"

#define SIZE_DATA_STR 8

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
	PWMX,
	ERRORS
}; 

enum State Parser(char* str, uint8_t count);

uint8_t getPort(void);

uint8_t getPin(void);

uint8_t getData(void);

uint8_t getDC(void);

uint8_t* getDataStr(void);


void setPort(uint8_t variable);

void setPin(uint8_t variable);

void setData(uint8_t variable);

void setDC(uint8_t variable);

void seDataStr(uint8_t* variable);

#endif
