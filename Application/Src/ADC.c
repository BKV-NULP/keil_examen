/**********************************************************************
* $Id$    ADC.c          2019-01-24
*
* @file    ADC.c
* @brief	This file is intended for use with ADC.
* @version  4.0
* @date    24. January. 2019
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
#include "ADC.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_adc.h"



	uint32_t adc_value, tmp=0;
	float volt = 0;
	

void InitCanal(PINSEL_CFG_Type PinCfg){
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 25;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
}

float ReadVoltage(){
	
	PINSEL_CFG_Type PinCfg;

	
	InitCanal(PinCfg);
	
	ADC_Init(LPC_ADC, 200000);
	ADC_IntConfig(LPC_ADC,_ADC_INT,DISABLE);
	ADC_ChannelCmd(LPC_ADC,_ADC_CHANNEL,ENABLE);

		ADC_StartCmd(LPC_ADC,ADC_START_NOW);
		//Wait conversion complete

		while (!(ADC_ChannelGetStatus(LPC_ADC,_ADC_CHANNEL,ADC_DATA_DONE)));
		adc_value = ADC_ChannelGetData(LPC_ADC,_ADC_CHANNEL);
		//Display the result of conversion on the UART0
		return (3.3*adc_value/4096);
}
