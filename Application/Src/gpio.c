/**********************************************************************
* $Id$    gpio.c          2019-01-24
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
#include "gpio.h"
#include "lpc17xx_gpio.h"	


int readPin(uint8_t lPort, uint8_t lPin){

	GPIO_SetDir(lPort, (0x00000000 & 0x00000001)<<lPin, 1);           /* LEDs on PORT1 defined as Output    */

  GPIO_ClearValue(lPort,(0x00000000 & 0x00000001)<<lPin);
  
	
	if((GPIO_ReadValue(lPort)>>lPin & 0x00000001) == 1){
		return 1;
	}else{
		return 0;
	}
}

void writePinOnOff(uint8_t lPort, uint8_t lPin, uint8_t lData){
	GPIO_SetDir(lPort, (0x00000000 & 0x00000001)<<lPin, 1);           /* LEDs on PORT1 defined as Output    */

  GPIO_ClearValue(lPort,(0x00000000 & 0x00000001)<<lPin);
  if(lData == 1){
		GPIO_SetValue(lPort, 1<<lPin);
	}else{
		 GPIO_ClearValue(lPort, 1<<lPin);
	}	
}

void initGPIO(void){
	GPIO_SetDir(1, 0xB0000000, 1);         
  GPIO_SetDir(2, 0x0000007C, 1);           

  GPIO_ClearValue(1, 0xB0000000);
  GPIO_ClearValue(2, 0x0000007C);
}
