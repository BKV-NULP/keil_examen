/**********************************************************************
* $Id$    adc.c          2019-01-24
*
* @file    adc.c
* @brief	This file is intended for use with ADC.
* @version  5.0
* @date    31. January. 2019
* @author  BVK
*
***********************************************************************
*	This file is intended for use with ADC.
*	When a function is called from the master file, the voltage in volts is output
*	to the terminal via the UART interface. 
*	With the potentiometer, the voltage value on the ADC can be changed. 
*	The corresponding changes will be reflected in the terminal.
**********************************************************************/
#include "lpc17xx_pinsel.h"
#include "adc.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_adc.h"



void initCanalADC(PINSEL_CFG_Type lPinCfg){
	lPinCfg.Funcnum = 1;
	lPinCfg.OpenDrain = 0;
	lPinCfg.Pinmode = 0;
	lPinCfg.Pinnum = 25;
	lPinCfg.Portnum = 0;
	PINSEL_ConfigPin(&lPinCfg);
	
	ADC_Init(LPC_ADC, 200000);
}

float readVoltageADC(){
	uint32_t gValueADC;
	
	ADC_IntConfig(LPC_ADC,_ADC_INT,DISABLE);
	ADC_ChannelCmd(LPC_ADC,_ADC_CHANNEL,ENABLE);
	
	ADC_StartCmd(LPC_ADC,ADC_START_NOW);
	
	//Wait conversion complete
	while (!(ADC_ChannelGetStatus(LPC_ADC,_ADC_CHANNEL,ADC_DATA_DONE)));
	gValueADC = ADC_ChannelGetData(LPC_ADC,_ADC_CHANNEL);
	
	//Display the result of conversion on the UART0
	return (3.3*gValueADC/4096);
}

