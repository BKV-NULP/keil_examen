/**********************************************************************
* $Id$    TIME.c          2020-01-24
*
* @file    TIME.c
* @brief	This file is intended for use with time.
* @version  4.0
* @date    24. January. 2019
* @author  BVK
*
***********************************************************************
*	This file is intended for use with Timer.
*
**********************************************************************/


#include "time.h"
#include "debug_frmwrk.h"

volatile unsigned long SysTickCnt = 0;
unsigned long time = 0;
int flagTime = 0;


void SysTick_Handler (void) {
  SysTickCnt++;
	if(SysTickCnt>=1000){
		setFlag(1);
		SysTickCnt = 0;
		time++;
	}
}

 unsigned long getTime(void){
		return time;
}
 
int getFlag(void){
	return flagTime;
}

void setFlag(int variable){
	flagTime = variable;
}

void timer_init(void){
	SysTick_Config(SystemCoreClock/1000 - 1); 
}
