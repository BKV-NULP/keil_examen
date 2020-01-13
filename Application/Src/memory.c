/**********************************************************************
* $Id$    memory.c          2019-12-24
*
* @file    memory.c
* @brief  This example describes the functions of recording and reading data from memory 
* @version  3.0
* @date    24. December. 2019
* @author  BVK
*
*******************************************************************
The MEMORY pins in this project use pins P0.27 - P0.28.
* This file contains 3 functions that are called from the main file. The functions are used to work with the memory.
* The write_memory(LPC_I2C_TypeDef *I2Cx, uint8_t data_start,uint16_t lengtch , uint8_t* b)  
* function takes in the parameters 4 values:
* I2Cx - device address;
* data_start - the starting address of writing the data to a file;
* lengtch - the number of bytes of data that is written to a file; 
* b - the data entered.
* The read_memory(LPC_I2C_TypeDef *I2Cx, uint8_t data_start,uint16_t lengtch , uint8_t* b)  
* function takes in the parameters 4 values:
* I2Cx - device address;
* data_start - the starting address of writing the data to a file;
* lengtch - the number of bytes of data that is written to a file; 
* b - the data readable.
* The InitiCanal(PINSEL_CFG_Type PinCfg)  function takes in the parameters 1 values:
* PinCfg - initialization of pins;
**********************************************************************/

#include "memory.h"
#include "lpc17xx_i2c.h"
//#include "lpc17xx_libcfg_default.h"

char* k= 0;

uint8_t* read_memory(LPC_I2C_TypeDef *I2Cx, uint8_t data_start,uint16_t lengtch , uint8_t* b){
	debug_frmwrk_init();
	PINSEL_CFG_Type PinCfg;
	PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
	PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
	InitiCanal(PinCfg);
	//Buffer_Init(1);
	I2C_M_SETUP_Type transferMCfg;
	uint8_t data = data_start;
	/* Start I2C slave device first */
	transferMCfg.sl_addr7bit = I2CDEV_S_ADDR;
	transferMCfg.tx_data = &data;
	transferMCfg.tx_length = 1;
	transferMCfg.rx_data = b;
	transferMCfg.rx_length = lengtch;
	transferMCfg.retransmissions_max = 3;
	I2C_MasterTransferData(I2Cx, &transferMCfg, I2C_TRANSFER_POLLING);
	//_DBG("\n");
	//_DBG((char*)b);
	//_DBG("\n");
	k = (char*)b;
	return 0;
}


void write_memory(LPC_I2C_TypeDef *I2Cx, uint8_t data_start,uint16_t lengtch , uint8_t* b){
	debug_frmwrk_init();
	PINSEL_CFG_Type PinCfg;
	PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
	PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
	InitiCanal(PinCfg);
	I2C_M_SETUP_Type transferMCfg;
	I2C_Cmd(I2Cx, I2C_MASTER_MODE, ENABLE);
	transferMCfg.sl_addr7bit = I2CDEV_S_ADDR;
	transferMCfg.tx_data = b ;
	transferMCfg.tx_length = lengtch;
	transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0; 
	transferMCfg.retransmissions_max = 3;
	I2C_MasterTransferData(I2Cx, &transferMCfg, I2C_TRANSFER_POLLING);
}

void InitiCanal(PINSEL_CFG_Type PinCfg){
#if ((USEDI2CDEV_M == 0))
	PinCfg.Funcnum = 1;
	PinCfg.Pinnum = 27;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 28;
	PINSEL_ConfigPin(&PinCfg);
#endif
	I2C_Init(I2CDEV_M, 100000);
	I2C_Cmd(I2CDEV_M, I2C_MASTER_MODE, ENABLE);
}

#ifdef  DEBUG
void check_failed(uint8_t *file, uint32_t line)
{
	while(1);
}
#endif
