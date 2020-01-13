#ifndef MEMORY_H
#define MEMORY_H

#include "lpc17xx_pinsel.h"
#include "debug_frmwrk.h"

#define USEDI2CDEV_M		0
/** Own Slave address in Slave I2C device */
#define I2CDEV_S_ADDR	(0x50)

#if (USEDI2CDEV_M == 0)
#define I2CDEV_M LPC_I2C0
#endif

uint8_t* read_memory(LPC_I2C_TypeDef *I2Cx, uint8_t data_start,uint16_t lengtch,uint8_t* b);

void write_memory(LPC_I2C_TypeDef *I2Cx, uint8_t data_start,uint16_t lengtch , uint8_t* b);

void InitiCanal(PINSEL_CFG_Type PinCfg);


#endif
