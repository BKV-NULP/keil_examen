#ifndef MEMORY_H
#define MEMORY_H

#include "lpc17xx_pinsel.h"
#include "debug_frmwrk.h"

#define USEDI2CDEV_M		0
// Own Slave address in Slave I2C device 
#define I2CDEV_S_ADDR	(0x50)

#if (USEDI2CDEV_M == 0)
#define I2CDEV_M LPC_I2C0
#endif

Status readMemory(LPC_I2C_TypeDef *I2Cx, uint8_t dataStart,uint16_t lLength );

Status writeMemory(LPC_I2C_TypeDef *I2Cx, uint8_t dataStart,uint16_t lLength , uint8_t* lData);

void initCanalMemmory(PINSEL_CFG_Type lPinCfg);

uint8_t* getMemmoryData(void);
#endif
