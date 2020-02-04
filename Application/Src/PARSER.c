/**********************************************************************
* $Id$    parser.c        2019-01-24
*
* @file    parser.c
* @brief  This example describes the functions of parser
* @version  5.0
* @date    31. January. 2019
* @author  BVK
*
*******************************************************************
The file is intended to parse the entered characters from the
terminal and to recognize the corresponding commands.
**********************************************************************/

#include "parser.h"

uint8_t gPort;
uint8_t gPin;
uint8_t gData;
uint8_t gDC;
uint8_t gDataStr[SIZE_DATA_STR];

enum State Parser(char* lStr, uint8_t lCount){

	if((lStr[0] == 'h' && lStr[1] == 0x0D)||(lStr[0]=='h' &&	 lStr[1]=='e' && lStr[2] == 'l' && lStr[3] == 'p')){
		return HELP;
	}else if(lStr[0] == 'p' && lStr[1] =='i' && lStr[2] == 'n'
	&& lCount < 9 && lCount>=5){
		int nPin0  = (int)lStr[6]-48;
		int nPin2  = ((int)lStr[5]-48)*10;
		setPin((uint8_t)(nPin2 + nPin0));
		setPort((uint8_t)lStr[3]-48);
		return PINX;
	}else if(lStr[0] == 'p' && lStr[1] == 'i' && lStr[2] == 'n' && lCount < 18 && lCount>=7){
		int nPin0  = (int)lStr[6]-48;
		int nPin2  = ((int)lStr[5]-48)*10;
		setPin((uint8_t)(nPin2 + nPin0));
		setPort((uint8_t)lStr[3]-48);
		setData((uint8_t)lStr[8] - 48);
		return PINXexactlyY;
	}else if(lStr[0] == 'A' && lStr[1] == 'D' && lStr[2] == 'C' && lCount>=2&&lCount<=6){
		return ADC;
	}else if(lStr[0] == 'A' && lStr[1] == 'D' && lStr[2] == 'C' && lStr[3] == 'c' && lStr[4] == 'o' && lStr[5] == 'u' && lStr[6] == 'n' && lStr[7] == 't'){
		return ADCcount;
	}else if(lStr[0] == 't' && lStr[1] == 'i' && lStr[2] == 'm' && lStr[3] == 'e' ){
		return TIME;
	}else if(lStr[0] == 'D' && lStr[1] == 'A' && lStr[2] == 'C'){
		return DAC;
	}else if(lStr[0] == 's' && lStr[1] == 't' && lStr[2] == 'o' && lStr[3] == 'r' && lStr[4] == 'e'){
		seDataStr((uint8_t*)lStr);		
	return STORE;
	}else if(lStr[0] == 'r' && lStr[1] == 'e' && lStr[2] == 'a' && lStr[3] == 'd' ){
		return READ;
	}
	else if(lStr[0] == 'P' && lStr[1] == 'W' && lStr[2] == 'M' && lStr[4] == '='){
		int iDC1  = (int)lStr[6]-48;
		int iDC2  = ((int)lStr[5]-48)*10;
		setDC((uint8_t)(iDC2 + iDC1));
		setPin((uint8_t)lStr[3]-48);		
		return PWMXexactlyY;
	}else if(lStr[0] == 'P' && lStr[1] == 'W' && lStr[2] == 'M'  && lCount<6){
		setPin((uint8_t)lStr[3]-48);
		return PWMX;
	}
	else{
		return ERRORS;
	}
}

uint8_t getPort(void){
	return gPort;
}

uint8_t getPin(void){
	return gPin;
}

uint8_t getData(void){
	return gData;
}

uint8_t getDC(void){
	return gDC;
}

uint8_t* getDataStr(void){
	return gDataStr;
}

void setPort(uint8_t variable){
	gPort = variable;
}

void setPin(uint8_t variable){
	gPin = variable;
}

void setData(uint8_t variable){
	gData = variable;
}

void setDC(uint8_t variable){
	gDC = variable;
}

void seDataStr(uint8_t* variable){
	for(int i = 0;i<SIZE_DATA_STR;i++){
		gDataStr[i] = variable[i+6];
	}
}
