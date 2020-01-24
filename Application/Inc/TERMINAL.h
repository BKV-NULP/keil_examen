#ifndef TERMINAL_H
#define TERMINAL_H


#include "debug_frmwrk.h"

extern UART_CFG_Type UARTConfigStruct;
extern uint32_t MatchCount;



int terminal(void);

enum State ReadUart(void);

void help(void);

#endif
