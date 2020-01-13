/**********************************************************************
* $Id$    PWM.c          2019-12-24
*
* @file    PWM.c
* @brief  This example describes use of PWM to set the brightness of the LEDs and read duty cycle 
* from the specified pin
* @version  3.0
* @date    24. December. 2019
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
#include "PWM.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_pwm.h"
#include "lpc17xx_pinsel.h"

uint32_t param = 0;
uint8_t volatile timer0_flag = FALSE, timer1_flag = FALSE;
BOOL_8 toggle=TRUE, first_capture, done;
uint8_t tem, temp, temp2, count=0,cs = 0;
uint32_t T1, T2, temcap, capture;

/*inline void ClearInterruptFlagEINTX(uint32_t X)
{
    LPC_SC->EXTINT |= (1<<X);  // Clear Interrupt Flag
}

void InitPower()
{
    LPC_SC->PCONP |= (1 << 15);                     // Enable power
}

void InitLED()
{
    LPC_PINCON->PINSEL3 &= ~(0 << 25) | ~(0 << 24);   // LED connected to P1.28 is in GPIO mode (see Table 83)
    LPC_GPIO1->FIODIR   = (1<< 28);                   // Configure LED pins as OUTPUT - P1.28
    LPC_GPIO1->FIOCLR |= (1 << 28);                   // set P1.28 to LOW
}
void InitPushButton()
{

    LPC_PINCON->PINSEL4     =     (1<<20);      // Configure P2_10, as EINT0
    LPC_GPIO2->FIODIR       &= ~(0 << 10);      // P2.10 is an input pin
    LPC_GPIOINT->IO2IntEnF  |=  (1 << 10);      // P2.10 reads the falling edges to generate an interrupt

}*/

void ReadPinPWM(uint16_t num, uint16_t perc)
{
	
	
	GPIO_SetDir(1, 0xB0000000, 1);         
  GPIO_SetDir(2, 0x0000007C, 1);           

  GPIO_ClearValue(1, 0xB0000000);
  GPIO_ClearValue(2, 0x0000007C);
	
		PINSEL_CFG_Type PinCfg;
	debug_frmwrk_init();
	
	//for(;;){
	//_DBG("dsfgnm");
	//}
	uint8_t temp2;
	PWM_TIMERCFG_Type PWMCfgDat;
	PWM_MATCHCFG_Type PWMMatchCfgDat;
	//PINSEL_CFG_Type PinCfg;

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
	//for (temp = 0; temp <= 5; temp++){
		PinCfg.Pinnum = num;
		PINSEL_ConfigPin(&PinCfg);
	//}
	PWM_MatchUpdate(LPC_PWM1, 0, 256, PWM_MATCH_UPDATE_NOW);
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
	//for (temp = 2; temp < 7; temp++)
	//{
		PWM_ChannelConfig(LPC_PWM1, num+1, PWM_CHANNEL_SINGLE_EDGE);
	//}	
	//temp2=10;
	temp2 = 256 * perc / 100;
	
		/* Set up match value */
		PWM_MatchUpdate(LPC_PWM1, num+1, temp2, PWM_MATCH_UPDATE_NOW);
		/* Configure match option */
		PWMMatchCfgDat.IntOnMatch = DISABLE;
		PWMMatchCfgDat.MatchChannel = num+1;
		PWMMatchCfgDat.ResetOnMatch = DISABLE;
		PWMMatchCfgDat.StopOnMatch = DISABLE;
		PWM_ConfigMatch(LPC_PWM1, &PWMMatchCfgDat);
		/* Enable PWM Channel Output */
		PWM_ChannelCmd(LPC_PWM1, num+1, ENABLE);
		/* Increase match value by 10 */
		//temp2 += 40;
	

	/* Reset and Start counter */
	PWM_ResetCounter(LPC_PWM1);
	PWM_CounterCmd(LPC_PWM1, ENABLE);

	/* Start PWM now */
	PWM_Cmd(LPC_PWM1, ENABLE);
	//_DBG("\n");
}
/*void ToggleLED()
{
    #define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

    // check if bit is set, then toggle it depending on result.
    if(CHECK_BIT(LPC_GPIO1->FIOPIN, 28))
    {
        LPC_GPIO1->FIOCLR |= (1 << 28);
    }
    else
    {
        LPC_GPIO1->FIOSET |= (1 << 28);
    }
}*/

int ReadDCPin(uint8_t pin_num)
{
		uint16_t num = pin_num + 1;
	//uint32_t param = 0;
		param = PWM_MatchRead(LPC_PWM1, num)*100 / 256;
	return param;
		//_DBG("Duty cycle value = ");
		//_DBD(param);
		//_DBG("%");
		//_DBG("\n");
}

