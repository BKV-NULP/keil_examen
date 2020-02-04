/**********************************************************************
* $Id$    terminal.c          2019-01-24
*
* @file    terminal.c
* @brief	File to work with the terminal via the UART interface.
* @version  5.0
* @date    31. January. 2019
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

#include "terminal.h"
#include "gpio.h"
#include "adc.h"

#include "dac.h"
#include "memory.h"
#include "pwm.h"
#include "time.h"

#include "debug_frmwrk.h"
#include "lpc17xx_gpio.h"

#include "stdio.h"

#include "PARSER.H"

int i, c = 0;
uint8_t b[9] = {0};

uint8_t counts = 0;

//-------ADC and ADCcount------
char str1[10];
//-----------------------------

//Time
char str2[20];
//__________________________

extern char* k;

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

int terminal(){
	
	//local variable
	
	PINSEL_CFG_Type lPinCfgADC;
	PINSEL_CFG_Type lPinCfgDAC;
	PINSEL_CFG_Type lPinCfgMemmory;
	
	//part init modul
	
	initTimer();
	debug_frmwrk_init();
	initGPIO();
	initCanalADC(lPinCfgADC);
	initCanalDAC(lPinCfgDAC);
	initCanalMemmory(lPinCfgMemmory);
	enum State st = DAC;
	for(;;){
		_DBG("start");
		_DBG("\n");

		st = ReadUart();

		_DBG("\n");

		switch (st){
			case HELP:
				_DBG("\n---------------HELP----------------\n");
				help();
				break;
			
			case PINX:
				_DBG("\n---------------PINX----------------\n");
				if(readPin(getPort(), getPin()) == 1) _DBG("Pin ON\n");
				else _DBG("Pin OFF\n");
				break;
			
			case PINXexactlyY:
				writePinOnOff(getPort(), getPin(), getData());
				_DBG("\n---------------PINX=Y----------------\n");
				break;
			
			case ADC:
				_DBG("\n---------------ADC----------------\n");
				sprintf(str1,"%f V",readVoltageADC());
				_DBG("ADC value on channel 2: ");
				_DBG(str1);
				_DBG("\n ");
				break;
			
			case ADCcount:
				_DBG("\n---------------ADCcount----------------\n");
				i = 0;
				while(i<10){//It`s time in second how long will be to work ADC
					if(getFlag() == 1) // chack flag
					{
						sprintf(str1,"%f V",readVoltageADC());
						_DBG("ADC value on channel 2: ");
						_DBG(str1);
						_DBG("\n ");
						setFlag(0);			 //reset flag
						i++;
					}
				}
				break;
			
			case TIME:
				_DBG("\n---------------TIME----------------\n");
				sprintf(str2,"%lu second",getTime());
				_DBG("time from start work: ");
				_DBG(str2);
				_DBG("\n");
				break;
			
			case DAC:
				_DBG("\n---------------DAC----------------\n");
				_DBG("Please, use osciloscope\n");
				
				outputVoltageDAC();
				
				while(c<5)// we wait for 5 second 
				{
					if(getFlag() == 1){ // check flag
						c++;
						setFlag(0);	// reset flag 
					}
				}
				
				offDAC();
				break;
			
			case STORE:
				_DBG("\n---------------STORE----------------\n");
				uint8_t* data_strrr;
				data_strrr = getDataStr();
				for(int i = 1; i<9; i++){
					b[i] = data_strrr[i-1];
				}
				b[0] = 0;
				if(writeMemory(I2CDEV_M, 0 ,8 , b) == SUCCESS){
					_DBG("SUCCESS! We write data to memmory!!!");
				}else{
					_DBG("Error! We can`t to write data to memmory!!!");
				}
				break;
			
			case READ:
				_DBG("\n---------------read----------------\n");
				_DBG("\n");
				
				if(readMemory(I2CDEV_M, 0 ,8) == SUCCESS){
					_DBG((char*)getMemmoryData());
				}
				else{
					_DBG("Error! We can`t to read data with memmory!!!");
				}
				_DBG("\n");
				break;
			
			case PWMXexactlyY:
				_DBG("\n---------------PWMXexactlyY--------------\n");
				readPinPWM(getPin(), getDC());
				_DBG("\nDone.Please, watch on the pin.\n");
				break;
			
			case PWMX:
				_DBG("\n---------------PWMX----------------\n");
				
				_DBG("\nDuty cycle value = ");
				_DBD(readDCPinPWM(getPin()));
				_DBG("%");
				_DBG("\n");
				break;
			
			default:
				_DBG("\nError comand\n");
				help();
				break;
			
		}
	}
}

enum State ReadUart(){
	uint8_t t[20];
	char* str;
	
	for(int i = 0; i< 20; i++){
		t[i] = _DG;
		if(t[i] == 0x0D){
			counts = i;
			i = 21;
		}
	}	

	str = (char*)t;	
	
	return Parser(str, counts);
}


