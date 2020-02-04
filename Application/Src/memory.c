/**********************************************************************
* $Id$    memory.c          2019-01-24
*
* @file    memory.c
* @brief  This example describes the functions of recording and reading data from memory 
* @version  5.0
* @date    31. January. 2019
* @author  BVK
*
*******************************************************************
The MEMORY pins in this project use pins P0.27 - P0.28.
- SDA -> P0.27
- SCL -> P0.28
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

uint8_t* gDataMemmory;

Status readMemory(LPC_I2C_TypeDef *I2Cx, uint8_t dataStart,uint16_t lLength ){
	Status errFlag;
	I2C_M_SETUP_Type transferMCfg;
	gDataMemmory[0] = dataStart;
	/* Start I2C slave device first */
	transferMCfg.sl_addr7bit = I2CDEV_S_ADDR;
	transferMCfg.tx_data = &dataStart;
	transferMCfg.tx_length = 1;
	transferMCfg.rx_data = gDataMemmory;
	transferMCfg.rx_length = lLength+1;
	transferMCfg.retransmissions_max = 3;
	errFlag = I2C_MasterTransferData(I2Cx, &transferMCfg, I2C_TRANSFER_POLLING);
	return errFlag;
}


Status writeMemory(LPC_I2C_TypeDef *I2Cx, uint8_t dataStart,uint16_t lLength , uint8_t* lData){
	uint8_t* pArrayDataSend;
	Status errFlag;

	pArrayDataSend[0] = dataStart;
	for(int i = 0; i<=lLength; i++){
		pArrayDataSend[i+1]=lData[i];
	}
	I2C_M_SETUP_Type transferMCfg;
	I2C_Cmd(I2Cx, I2C_MASTER_MODE, ENABLE);
	transferMCfg.sl_addr7bit = I2CDEV_S_ADDR;
	transferMCfg.tx_data = pArrayDataSend ;
	transferMCfg.tx_length = lLength+1;
	transferMCfg.rx_data = NULL;
	transferMCfg.rx_length = 0; 
	transferMCfg.retransmissions_max = 3;
	errFlag = I2C_MasterTransferData(I2Cx, &transferMCfg, I2C_TRANSFER_POLLING);
	return errFlag;
	
}

void initCanalMemmory(PINSEL_CFG_Type lPinCfg){
#if ((USEDI2CDEV_M == 0))
	lPinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
	lPinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
	lPinCfg.Funcnum = 1;
	lPinCfg.Pinnum = 27;
	lPinCfg.Portnum = 0;
	PINSEL_ConfigPin(&lPinCfg);
	lPinCfg.Pinnum = 28;
	PINSEL_ConfigPin(&lPinCfg);
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


uint8_t* getMemmoryData(void){
	return gDataMemmory;
}
