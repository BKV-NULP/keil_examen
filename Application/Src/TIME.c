/**********************************************************************
* $Id$    time.c          2020-01-24
*
* @file    time.c
* @brief	This file is intended for use with time.
* @version  5.0
* @date    31. January. 2019
* @author  BVK
*
***********************************************************************
*	This file is intended for use with Timer.
*
**********************************************************************/


#include "time.h"
#include "LPC17xx.h"
#include "lpc_types.h"

volatile unsigned long SysTickCnt = 0;
unsigned long gTime = 0;
int flagTime = TIME_OFF;


void SysTick_Handler (void) {
  SysTickCnt++;
	//we wait for 1 second;
	if(SysTickCnt >= ONE_SECOND){ 
		setFlag(TIME_ON);
		SysTickCnt = 0;
		gTime++;
	}
}

 unsigned long getTime(void){
		return gTime;
}
 
int getFlag(void){
	return flagTime;
}

void setFlag(int lVariable){
	flagTime = lVariable;
}

void initTimer(void){
	SysTick_Config(SystemCoreClock/ONE_SECOND - 1); 
}
