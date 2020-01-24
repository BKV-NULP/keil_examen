/**********************************************************************
* $Id$    PORTS.c          2019-01-24
*
* @file    PORTS.c
* @brief	This file is intended for use with GPIO.
* @version  4.0
* @date    24. January. 2019
* @author  BVK
*
***********************************************************************
*	This file is intended for use with GPIOs. Contains 2 functions:
*	ReadPin(uint8_t port, uint8_t pin) allows you to read the status of the corresponding pin
*	The WritePin_OnOff(uint8_t port, uint8_t pin, uint8_t data) designed to control pins (on / off).
**********************************************************************/
#include "PORTS.h"
#include "lpc17xx_gpio.h"	


int flag;

int ReadPin(uint8_t port, uint8_t pin){
	
	int flag;
	
	GPIO_SetDir(port, (0x00000000 & 0x00000001)<<pin, 1);           /* LEDs on PORT1 defined as Output    */

  GPIO_ClearValue(port,(0x00000000 & 0x00000001)<<pin);
  
	
	if((GPIO_ReadValue(port)>>pin & 0x00000001) == 1){
		flag = 1;
	}else{
		flag = 0;
	}
	return flag;
}

void WritePin_OnOff(uint8_t port, uint8_t pin, uint8_t data){
	GPIO_SetDir(port, (0x00000000 & 0x00000001)<<pin, 1);           /* LEDs on PORT1 defined as Output    */

  GPIO_ClearValue(port,(0x00000000 & 0x00000001)<<pin);
  if(data == 1){
		GPIO_SetValue(port, 1<<pin);
	}else{
		 GPIO_ClearValue(port, 1<<pin);
	}	
}

void GPIO_init(void){
	GPIO_SetDir(1, 0xB0000000, 1);         
  GPIO_SetDir(2, 0x0000007C, 1);           

  GPIO_ClearValue(1, 0xB0000000);
  GPIO_ClearValue(2, 0x0000007C);
}
