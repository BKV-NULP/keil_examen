/**********************************************************************
* $Id$    PARSER.c        2019-01-24
*
* @file    PARSER.c
* @brief  This example describes the functions of parser
* @version  4.0
* @date    24. January. 2019
* @author  BVK
*
*******************************************************************
The file is intended to parse the entered characters from the
terminal and to recognize the corresponding commands.
**********************************************************************/

#include "PARSER.h"



uint8_t port;
uint8_t pin;
uint8_t data;
uint8_t dc;
uint8_t data_str[SIZE_DATA_STR];

enum State parser(char* str, uint8_t count){

	if(str[0] == ('h'&& str[1] == 0x0D)||
	(str[0]=='h' &&	 str[1]=='e' && str[2] == 'l' && str[3] == 'p' && str[4] == 0x0D)){
		return HELP;
	}else if(str[0] == 'p' && str[1] =='i' && str[2] == 'n'
	&& count < 9 && count>=5){
		int pin0  = (int)str[6]-48;
		int pin2  = ((int)str[5]-48)*10;
		setPin((uint8_t)(pin2 + pin0));
		setPort((uint8_t)str[3]-48);
		return PINX;
	}else if(str[0] == 'p' && str[1] == 'i' && str[2] == 'n' && count < 18 && count>=7){
		int pin0  = (int)str[6]-48;
		int pin2  = ((int)str[5]-48)*10;
		setPin((uint8_t)(pin2 + pin0));
		setPort((uint8_t)str[3]-48);
		setData((uint8_t)str[8] - 48);
		return PINXexactlyY;
	}else if(str[0] == 'A' && str[1] == 'D' && str[2] == 'C' && count>=2&&count<=6){
		return ADC;
	}else if(str[0] == 'A' && str[1] == 'D' && str[2] == 'C' && str[3] == 'c' && str[4] == 'o' && str[5] == 'u' && str[6] == 'n' && str[7] == 't'){
		return ADCcount;
	}else if(str[0] == 't' && str[1] == 'i' && str[2] == 'm' && str[3] == 'e' ){
		return TIME;
	}else if(str[0] == 'D' && str[1] == 'A' && str[2] == 'C'){
		return DAC;
	}else if(str[0] == 's' && str[1] == 't' && str[2] == 'o' && str[3] == 'r' && str[4] == 'e'){
		seDataStr((uint8_t*)str);		
	return STORE;
	}else if(str[0] == 'r' && str[1] == 'e' && str[2] == 'a' && str[3] == 'd' ){
		return READ;
	}
	else if(str[0] == 'P' && str[1] == 'W' && str[2] == 'M' && str[4] == '='){
		int dc1  = (int)str[6]-48;
		int dc2  = ((int)str[5]-48)*10;
		setDC((uint8_t)(dc2 + dc1));
		setPin((uint8_t)str[3]-48);		
		return PWMXexactlyY;
	}else if(str[0] == 'P' && str[1] == 'W' && str[2] == 'M'  && count<6){
		setPin((uint8_t)str[3]-48);
		return PWMX;
	}
	else{
		return ERRORS;
	}
}

uint8_t getPort(void){
	return port;
}

uint8_t getPin(void){
	return pin;
}

uint8_t getData(void){
	return data;
}

uint8_t getDC(void){
	return dc;
}

uint8_t* getDataStr(void){
	return data_str;
}

void setPort(uint8_t variable){
	port = variable;
}

void setPin(uint8_t variable){
	pin = variable;
}

void setData(uint8_t variable){
	data = variable;
}

void setDC(uint8_t variable){
	dc = variable;
}

void seDataStr(uint8_t* variable){
	for(int i = 0;i<SIZE_DATA_STR;i++){
		data_str[i] = variable[i+6];
	}
}
