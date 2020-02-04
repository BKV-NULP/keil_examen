/**********************************************************************
* $Id$    pwm.c          2020-01-30
*
* @file    pwm.c
* @brief  This example describes use of PWM to set the brightness of the LEDs and read duty cycle 
* from the specified pin
* @version  5.0
* @date    31. January. 2019
* @author  BVK
*
***********************************************************************
* The PWM pins in this project use pins P2.2 - P2.5.
*  This file contains 2 functions that are called from the main file. The functions are used to work with the PWM.
*  The ReadPinPWM(uint16_t num, uint16_t perc) function takes in the parameters 2 values:
*  num - the number of the corresponding LED pin for which the PWM will be used;
*  perc - is the percentage of the duty cycle as a percentage 
* to set the brightness of the LED P2.2 - P2.5 at the user's choice.
*  The ReadDCPin(uint8_t pin_num) function takes in the parameters 1 values:
*  pin_num - the LED pin number from which the user wants to read the duty cycle value as a percentage;
**********************************************************************/
#include "pwm.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_pwm.h"
#include "lpc17xx_pinsel.h"


uint8_t  timer0_flag = FALSE, timer1_flag = FALSE;
BOOL_8 toggle=TRUE, first_capture, done;
uint8_t tem, temp, temp2, count=0,cs = 0;
uint32_t T1, T2, temcap, capture;


void readPinPWM(uint16_t lNum, uint16_t lPerc)
{
	
	GPIO_SetDir(1, 0xB0000000, 1);         
  GPIO_SetDir(2, 0x0000007C, 1);           

  GPIO_ClearValue(1, 0xB0000000);
  GPIO_ClearValue(2, 0x0000007C);
	
	PINSEL_CFG_Type PinCfg;
	
	uint8_t temp2;
	PWM_TIMERCFG_Type PWMCfgDat;
	PWM_MATCHCFG_Type PWMMatchCfgDat;


	/* PWM block section -------------------------------------------- */
	/* Initialize PWM peripheral, timer mode
	 * PWM prescale value = 1 (absolute value - tick value) */
	PWMCfgDat.PrescaleOption = PWM_TIMER_PRESCALE_TICKVAL;
	PWMCfgDat.PrescaleValue = 1;
	PWM_Init(LPC_PWM1, PWM_MODE_TIMER, (void *) &PWMCfgDat);

	/*
	 * Initialize PWM pin connect
	 */
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 2;
	
	PinCfg.Pinnum = lNum;
	PINSEL_ConfigPin(&PinCfg);

	PWM_MatchUpdate(LPC_PWM1, 0, cQuantityScalePWM, PWM_MATCH_UPDATE_NOW);
	/* PWM Timer/Counter will be reset when channel 0 matching
	 * no interrupt when match
	 * no stop when match */
	PWMMatchCfgDat.IntOnMatch = DISABLE;
	PWMMatchCfgDat.MatchChannel = 0;
	PWMMatchCfgDat.ResetOnMatch = ENABLE;
	PWMMatchCfgDat.StopOnMatch = DISABLE;
	PWM_ConfigMatch(LPC_PWM1, &PWMMatchCfgDat);

	/* Configure each PWM channel: --------------------------------------------- */
	/* - Single edge
	 * - PWM Duty on each PWM channel determined by
	 * the match on channel 0 to the match of that match channel.
	 * Example: PWM Duty on PWM channel 1 determined by
	 * the match on channel 0 to the match of match channel 1.
	 */

	/* Configure PWM channel edge option
	 * Note: PWM Channel 1 is in single mode as default state and
	 * can not be changed to double edge mode */

	PWM_ChannelConfig(LPC_PWM1, lNum+1, PWM_CHANNEL_SINGLE_EDGE);

	temp2 = 256 * lPerc / 100;
	
	// Set up match value 
	PWM_MatchUpdate(LPC_PWM1, lNum+1, temp2, PWM_MATCH_UPDATE_NOW);
	// Configure match option 
	PWMMatchCfgDat.IntOnMatch = DISABLE;
	PWMMatchCfgDat.MatchChannel = lNum+1;
	PWMMatchCfgDat.ResetOnMatch = DISABLE;
	PWMMatchCfgDat.StopOnMatch = DISABLE;
	PWM_ConfigMatch(LPC_PWM1, &PWMMatchCfgDat);
	// Enable PWM Channel Output 
	PWM_ChannelCmd(LPC_PWM1, lNum+1, ENABLE);

	// Reset and Start counter 
	PWM_ResetCounter(LPC_PWM1);
	PWM_CounterCmd(LPC_PWM1, ENABLE);

	// Start PWM now 
	PWM_Cmd(LPC_PWM1, ENABLE);

}

int readDCPinPWM(uint8_t lPinNum)
{		
		// 100 - range from 0 to 100
 		return PWM_MatchRead(LPC_PWM1, lPinNum + 1)*100 / cQuantityScalePWM;  
}

