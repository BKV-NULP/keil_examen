/**********************************************************************
* $Id$    dac.c          2019-01-24
*
* @file    dac.c
* @brief	This file is intended for use with DAC.
* @version  5.0
* @date    31. January. 2019
* @author  BVK
*
***********************************************************************
*	This file is intended for use with DAC.
*	The DAC generates a sinusoidal signal from the coefficient table.
* The signal is output at P0.26. Use an oscilloscope to demonstrate the result.
**********************************************************************/
#include "dac.h"

void initCanalDAC(PINSEL_CFG_Type lPinCfg){
	lPinCfg.Funcnum = 2;
	lPinCfg.OpenDrain = 0;
	lPinCfg.Pinmode = 0;
	lPinCfg.Pinnum = 26;
	lPinCfg.Portnum = 0;
	PINSEL_ConfigPin(&lPinCfg);
}

void outputVoltageDAC(){
	
	GPDMA_Channel_CFG_Type lGPDMACfg;
	DAC_CONVERTER_CFG_Type lDAC_ConverterConfigStruct;
	GPDMA_LLI_Type lDMA_LLI_Struct;
	uint32_t iTmp;
	
	uint32_t sin_0_to_90_16_samples[16]={\
			0,1045,2079,3090,4067,\
			5000,5877,6691,7431,8090,\
			8660,9135,9510,9781,9945,10000\
	};
	uint32_t dac_sine_lut[NUM_SINE_SAMPLE];

	for(uint32_t i=0;i<NUM_SINE_SAMPLE;i++)
	{
		if(i<=15)
		{
			dac_sine_lut[i] = 512 + 512*sin_0_to_90_16_samples[i]/10000;
			if(i==15) dac_sine_lut[i]= 1023;
		}
		else if(i<=30)
		{
			dac_sine_lut[i] = 512 + 512*sin_0_to_90_16_samples[30-i]/10000;
		}
		else if(i<=45)
		{
			dac_sine_lut[i] = 512 - 512*sin_0_to_90_16_samples[i-30]/10000;
		}
		else
		{
			dac_sine_lut[i] = 512 - 512*sin_0_to_90_16_samples[60-i]/10000;
		}
		dac_sine_lut[i] = (dac_sine_lut[i]<<6);
	}
	//Prepare DMA link list item structure
	lDMA_LLI_Struct.SrcAddr= (uint32_t)dac_sine_lut;
	lDMA_LLI_Struct.DstAddr= (uint32_t)&(LPC_DAC->DACR);
	lDMA_LLI_Struct.NextLLI= (uint32_t)&lDMA_LLI_Struct;
	lDMA_LLI_Struct.Control= DMA_SIZE
							| (2<<18) //source width 32 bit
							| (2<<21) //dest. width 32 bit
							| (1<<26) //source increment
							;

	/* GPDMA block section -------------------------------------------- */
	/* Initialize GPDMA controller */
	GPDMA_Init();

	// Setup GPDMA channel --------------------------------
	// channel 0
	lGPDMACfg.ChannelNum = 0;
	// Source memory
	lGPDMACfg.SrcMemAddr = (uint32_t)(dac_sine_lut);
	// Destination memory - unused
	lGPDMACfg.DstMemAddr = 0;
	// Transfer size
	lGPDMACfg.TransferSize = DMA_SIZE;
	// Transfer width - unused
	lGPDMACfg.TransferWidth = 0;
	// Transfer type
	lGPDMACfg.TransferType = GPDMA_TRANSFERTYPE_M2P;
	// Source connection - unused
	lGPDMACfg.SrcConn = 0;
	// Destination connection
	lGPDMACfg.DstConn = GPDMA_CONN_DAC;
	// Linker List Item - unused
	lGPDMACfg.DMALLI = (uint32_t)&lDMA_LLI_Struct;
	// Setup channel with given parameter
	GPDMA_Setup(&lGPDMACfg);

	lDAC_ConverterConfigStruct.CNT_ENA =SET;
	lDAC_ConverterConfigStruct.DMA_ENA = SET;
	DAC_Init(LPC_DAC);
	/* set time out for DAC*/
	iTmp = (PCLK_DAC_IN_MHZ*1000000)/(SINE_FREQ_IN_HZ*NUM_SINE_SAMPLE);
	DAC_SetDMATimeOut(LPC_DAC,iTmp);
	DAC_ConfigDAConverterControl(LPC_DAC, &lDAC_ConverterConfigStruct);
	
	// Enable GPDMA channel 0
	GPDMA_ChannelCmd(0, ENABLE);
	//while(1);
}

void offDAC(void){
	GPDMA_ChannelCmd(0, DISABLE);
}
