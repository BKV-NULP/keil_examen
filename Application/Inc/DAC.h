#ifndef DAC_H
#define DAC_H

#include "lpc17xx_dac.h"
#include "debug_frmwrk.h"
#include "lpc17xx_gpdma.h"
#include "lpc17xx_pinsel.h"

#define DMA_SIZE		60
#define NUM_SINE_SAMPLE	60
#define SINE_FREQ_IN_HZ	60
#define PCLK_DAC_IN_MHZ	25

void InitCanale(PINSEL_CFG_Type PinCfg);
void OutputVoltage(void);
void OffDAC(void);

#endif

