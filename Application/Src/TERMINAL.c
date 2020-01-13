/**********************************************************************
* $Id$    TERMINAL.c          2019-12-24
*
* @file    TERMINAL.c
* @brief	File to work with the terminal via the UART interface.
* @version  3.0
* @date    24. December. 2019
* @author  BVK
*
***********************************************************************
*	File to work with the terminal via the UART interface.
*	This file is intended to generate the project menu, to call all the functions implemented in the project.
*	The user enters the terminal console into the terminal function. 
*	Through the UART interface, the command is sent to the program,
*	parsed and the corresponding function is performed.
* A help menu has been implemented to assist the user while working.
**********************************************************************/

#include "TERMINAL.h"
#include "PORTS.h"
#include "ADC.h"

#include "DAC.h"
#include "memory.h"
#include "PWM.h"
#include "time.h"

#include "debug_frmwrk.h"
#include "lpc17xx_gpio.h"

#include "stdio.h"



UART_CFG_Type UARTConfigStruct;
uint32_t MatchCount;

int i, c = 0;
uint8_t b[9] = {0, 1, 2, 3 , 4, 5, 6 , 7,7 };

uint8_t port;
uint8_t pin;
uint8_t data;
uint8_t dc;
uint8_t data_str[8];
//-------PWMX------
extern uint32_t param;
//-----------------

//-------ADC and ADCcount------
extern uint32_t adc_value, tmp;
extern float volt;
extern char str1[10];
//-----------------------------

//Time
extern unsigned long time;
extern char str2[20];
//__________________________

extern char* k;

extern int flag;


void SysTick_Handler (void) {
  SysTickCnt++;
	if(SysTickCnt>=1000){
		ADCflag = 1;
		SysTickCnt = 0;
		time++;
	}
}



void help(){
	_DBG("\n*************** HELP ********************\n");
	_DBG("\nhelp; or h; - this function show list all  functions\n");
	_DBG("\n");
	_DBG("\npinX;       - this function show status pin\n");
	_DBG("\n              exemple: pin1.03;\n");
	_DBG("\n");
	_DBG("\npinX=Y;     - this function writes 0 or 1 to pinY. 0-off 1-on \n");
	_DBG("\n              exemple: pin1.03=0;\n");
	_DBG("\n");
	_DBG("\nADC;        - this function shows result from ADC\n");
	_DBG("\n              exemple: ADC;\n");
	_DBG("\n");
	_DBG("\nADCcount;   - this function shows result from ADC in forever loop\n");
	_DBG("\n              exemple: ADCcount;\n");
	_DBG("\n");
	_DBG("\ntime;       - this function show? time(in second) from start work app\n");
	_DBG("\n              exemple: time;\n");
	_DBG("\n");
	_DBG("\nDACX;       - this function create? voltage in pin DAC\n");
	_DBG("\n              exemple: DAC;\n");
	_DBG("\n");
	_DBG("\nstore'XXX'; - writes string typed in by the user (0 to 8)\n");
	_DBG("\n              exemple: store'xxx';\n");
	_DBG("\nread;       - read string typed in by the user (0 to 8)\n");
	_DBG("\n              exemple: read;\n");
	_DBG("\n");
	_DBG("\nPWMX=Y;     - writes duty cycle Y  in to PWMX\n");
	_DBG("\n               exemple:PWM1.02=90;\n");
	_DBG("\n");	
	_DBG("\nPWMX;       - reads out in PWMX\n");
	_DBG("\n              exemple:PWM1.02;\n");
	_DBG("\n");	
}

int legenda(){
	SysTick_Config(SystemCoreClock/1000 - 1); 
	
	debug_frmwrk_init();
	enum State st = DAC;
	
	GPIO_SetDir(1, 0xB0000000, 1);         
  GPIO_SetDir(2, 0x0000007C, 1);           

  GPIO_ClearValue(1, 0xB0000000);
  GPIO_ClearValue(2, 0x0000007C);
	
	
	
	
	for(;;){
		_DBG("start");
		_DBG("\n");
		st = ReadUart();
		switch (st){
			case HELP:
				_DBG("\n---------------HELP----------------\n");
				help();
				break;
			case PINX:
				_DBG("\n---------------PINX----------------\n");
				ReadPin(port, pin);
			if(flag == 1) _DBG("Pin ON");
			else _DBG("Pin OFF");
				break;
			case PINXexactlyY:
				WritePin_OnOff(port, pin, data);
				_DBG("\n---------------PINX=Y----------------\n");
				break;
			case ADC:
				_DBG("\n---------------ADC----------------\n");
				ReadVoltage();
		sprintf(str1,"%f V",volt);
		_DBG("ADC value on channel 2: ");
		_DBG(str1);
		_DBG("\n ");
				break;
			case ADCcount:
				_DBG("\n---------------ADCcount----------------\n");
				//int i = 0;
				while(i<10){
				if(ADCflag == 1)
					{
						ReadVoltage();
			sprintf(str1,"%f V",volt);
		_DBG("ADC value on channel 2: ");
		_DBG(str1);
		_DBG("\n ");
						ADCflag = 0;
						
						i++;
					}
				}
				break;
			case TIME:
				_DBG("\n---------------TIME----------------\n");
				getTime();
			sprintf(str2,"%lu second",time);
		_DBG("time from start work: ");
		_DBG(str2);
				break;
			case DAC:
				_DBG("\n---------------DAC----------------\n");
				//OutputVoltage();
				_DBG("Please, use osciloscope");
				while(c<20)
				{_DBG("/");
				OutputVoltage();
				c++;}
				break;
			case STORE:
				_DBG("\n---------------STORE----------------\n");

				
				for(int i = 1; i<9; i++){
					b[i] = data_str[i-1];
				}
				b[0] = 0;
				write_memory(I2CDEV_M, 0 ,9 , b);
			
				break;
			case READ:
				_DBG("\n---------------read----------------\n");
				read_memory(I2CDEV_M, 0 ,8 , data_str);
			_DBG("\n");
	_DBG(k);
	_DBG("\n");
				break;
			case PWMXexactlyY:
				_DBG("\n---------------PWMXexactlyY----------------\n");
				ReadPinPWM(pin, dc);
			_DBG("Done.Please, watch on the pin.");
				break;
			case PWMX:
				_DBG("\n---------------PWMX----------------\n");
			ReadDCPin(pin);
		_DBG("Duty cycle value = ");
		_DBD(param);
		_DBG("%");
		_DBG("\n");
				break;
			default:
				//_DBG("\n-------------------------------\n");
				break;
			
		}
	}
}
enum State ReadUart(){
	uint8_t t[20];
	uint8_t count = 0;
	//char str_str[20];
	for(int i = 0; i< 20; i++){
		t[i] = _DG;
		if(t[i] == 59){
			count = i;
			i = 21;
		}
	}	
	char* str;
	str = (char*)t;	
	if(str[0] == ('h'&& str[1] == ';')||
	(str[0]=='h' &&	 str[1]=='e' && str[2] == 'l' && str[3] == 'p' && str[5] == ';')){
		return HELP;
	}else if(str[0] == 'p' && str[1] =='i' && str[2] == 'n'
	&& count < 9 && count>=7){
				
		int pin0  = (int)str[6]-48;
		int pin2  = ((int)str[5]-48)*10;
		pin = (uint8_t)(pin2 + pin0);
		_DBG("\npin:");
		_DBD(pin);
		_DBG("\n");

		_DBG("port:");
		port =  (uint8_t)str[3]-48;
		_DBD(port);
		_DBG("\n");
			
		return PINX;
	}else if(str[0] == 'p' && str[1] == 'i' && str[2] == 'n' && count < 15 && count>=9){
		int pin0  = (int)str[6]-48;
		int pin2  = ((int)str[5]-48)*10;
		pin = (uint8_t)(pin2 + pin0);
		_DBG("\npin:");
		_DBD(pin);
		_DBG("\n");

		_DBG("port:");
		port =  (uint8_t)str[3]-48;
		_DBD(port);
		_DBG("\n");
		
		data  = (uint8_t)str[8] - 48;
		
		return PINXexactlyY;
	}else if(str[0] == 'A' && str[1] == 'D' && str[2] == 'C' && count>=2&&count<=4){
		return ADC;
	}else if(str[0] == 'A' && str[1] == 'D' && str[2] == 'C' && str[3] == 'c' && str[4] == 'o' && str[5] == 'u' && str[6] == 'n' && str[7] == 't'){
		return ADCcount;
	}else if(str[0] == 't' && str[1] == 'i' && str[2] == 'm' && str[3] == 'e' ){
		return TIME;
	}else if(str[0] == 'D' && str[1] == 'A' && str[2] == 'C'){
		return DAC;
	}else if(str[0] == 's' && str[1] == 't' && str[2] == 'o' && str[3] == 'r' && str[4] == 'e'){
		data_str[0] = (uint8_t)str[6];
		data_str[1] = (uint8_t)str[7];
		data_str[2] = (uint8_t)str[8];
		data_str[3] = (uint8_t)str[9];
		data_str[4] = (uint8_t)str[10];
		data_str[5] = (uint8_t)str[11];
		data_str[6] = (uint8_t)str[12];
		data_str[7] = (uint8_t)str[13];

		
	return STORE;
	
	
	}else if(str[0] == 'r' && str[1] == 'e' && str[2] == 'a' && str[3] == 'd' ){
	
		return READ;
	}
	else if(str[0] == 'P' && str[1] == 'W' && str[2] == 'M' && str[4] == '='){
		int dc1  = (int)str[6]-48;
		int dc2  = ((int)str[5]-48)*10;
		dc = (uint8_t)(dc2 + dc1);
		_DBG("\ndc:");
		_DBD(dc);
		_DBG("\n");

		_DBG("\npin:");
		pin =  (uint8_t)str[3]-48;
		_DBD(pin);
		_DBG("\n");
		
	//	data  = (uint8_t)str[8] - 48;
		return PWMXexactlyY;
	}else if(str[0] == 'P' && str[1] == 'W' && str[2] == 'M'  && count<6){
		
		_DBG("\npin:");
		pin =  (uint8_t)str[3]-48;
		_DBD(pin);
		_DBG("\n");
		
		return PWMX;
	}
	else{
		_DBG("\n Error:   please press 'help;' for more information\n");
		//return (enum State) 0;
	}
	return (enum State) 0;
}

