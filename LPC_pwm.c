/**
* @file		LPC_pwm.c 
* @brief    Treiber für das Steuern von ein bzw. mehrerer PWM Signale \n
* @version	V1.0 
* @date			2014-05-31
* @author 	112222\INS \n
*		   			Westsächsische Hochschule Zwickau \n
*		   			Fakultät Elektrotechnik - Informationstechnik \n
*/ 
#include <stdint.h>
#include <stdlib.h>
#include "lpc17xx.h"
#include "LPC_pwm.h"

uint32_t match_counter0, match_counter1;

PWMCTRL pwm_ctrl[PWM_COUNT];//!< sichert den Tastaturwert im Interrupt-Handler SPI


/*
void PWM1_IRQHandler (void) 
{
  uint32_t regVal;

  regVal = LPC_PWM1->IR;  						// Read Interrupt Flag
  if ( regVal & MR0_INT )
  {
	match_counter1++;		  					// Match-0 Interrupt
  }
  LPC_PWM1->IR |= regVal;						// Clear Interrupt Flag on Match-0 
  return;
}
*/

PWMCTRL pwm_ctrl[PWM_COUNT];

/******************PWM_open*****************************************/
/**
 * @brief	Exlusive Reservierung 
 * @return	Handle des verwendeten PWMs
 * @param   Nummer der PWM
 *******************************************************************/
uint8_t PWM_open(uint8_t pwmNumber)
{
	if(pwmNumber < PWM_COUNT)
	{
		if(pwm_ctrl[pwmNumber].used != PWM_USED)
		{
			pwm_ctrl[pwmNumber].handle = pwmNumber;
			pwm_ctrl[pwmNumber].used = PWM_USED;
			return pwm_ctrl[pwmNumber].handle;
		}
		return ERROR;//PWM already used
	}
	return ERROR;
}


/******************PWM_init*****************************************/
/**
 * @brief	Jeweilige PWM wird initialisiert
 * @return	Handle des verwendeten PWMs
 * @param   PWM Handle
 * @param   Zyklusdauer für MR0(Haupt-PWM Timer)
 *******************************************************************/
void PWM_init(uint32_t cycle)
{

	//uintptr_t pwmConp = (uintptr_t *) PCONP_REGISTER;
	//uintptr_t pwmPerCLK = (uintptr_t *) PCLKSELR0;
		// *pwmConp |= (PWM1_CLK_ENABLE);			//pheriphal Clock enabled
		// *pwmPerCLK |= (PWM1_CLK_CCLK);			//set Perripheral Clock
 	
		match_counter1 = 0;

		LPC_PWM1->TCR = TCR_RESET;			// Counter Reset 
		//LPC_PWM1->PR = 0x00;					// Prescale = 1 -> count frequency:Fpclk(72MHz/4=18MHz)
		LPC_PWM1->PR = 17;							// Prescale = 18 = 18MHz/18	= 1MHz
		LPC_PWM1->MR0 = cycle;					//Main cyclce

}

/******************PWM_set*****************************************/
/**
 * @brief	Einstellung welche PWM erlaubt wird und Zykluszeit der jeweiligen PWM
 * @return	Handle der verwendeten PWMs
 * @param   PWM Handle
 * @param   Zyklusdauer für jeweilige PWM
 *******************************************************************/
void PWM_set(uint8_t pwm_handle, uint32_t cycle)
{

	if(pwm_handle < PWM_COUNT)
	{
		switch(pwm_handle)
		{
			case PWM1: 	LPC_PINCON->PINSEL4 &= ~(0x03<<0); 	// Reset P2.0 = GPIO   
									LPC_PINCON->PINSEL4 |=  (0x01<<0);	// Config P2.0 = PWM1
									LPC_PWM1->MR1 = cycle;
									LPC_PWM1->LER |= LER1_EN;
									break;
			case PWM2: 	LPC_PINCON->PINSEL4 &= ~(0x03<<2); 	// Reset P2.1 = GPIO   
							LPC_PINCON->PINSEL4 |=  (0x01<<2);	// Config P2.1 = PWM2
				LPC_PWM1->MR2 = cycle;	
						LPC_PWM1->LER |= LER2_EN;
						break;
			case PWM3:	LPC_PINCON->PINSEL4 &= ~(0x03<<4); 	// Reset P2.2 = GPIO   
							LPC_PINCON->PINSEL4 |=  (0x01<<4);	// Config P2.2 = PWM3
				LPC_PWM1->MR3 = cycle;
						LPC_PWM1->LER |= LER3_EN;
						break;
			case PWM4:	LPC_PINCON->PINSEL4 &= ~(0x03<<6); 	// Reset P2.3 = GPIO   
							LPC_PINCON->PINSEL4 |=  (0x01<<6);	// Config P2.3 = PWM4
				LPC_PWM1->MR4 = cycle;
						LPC_PWM1->LER |= LER4_EN;
						break;
			case PWM5:	LPC_PINCON->PINSEL4 &= ~(0x03<<8); 	// Reset P2.4 = GPIO   
							LPC_PINCON->PINSEL4 |=  (0x01<<8);	// Config P2.4 = PWM5
				LPC_PWM1->MR5 = cycle;
						LPC_PWM1->LER |= LER5_EN;
						break;
			case PWM6:	LPC_PINCON->PINSEL4 &= ~(0x03<<10);	// Reset P2.5 = GPIO   
							LPC_PINCON->PINSEL4 |=  (0x01<<10);	// Config P2.5 = PWM6
				LPC_PWM1->MR6 = cycle;
						LPC_PWM1->LER |= LER6_EN;
						break;
		}
	}

}

void PWM_start(uint8_t pwm_handle)
{

	if(pwm_handle < PWM_COUNT)
	{
		switch(pwm_handle) //all PWM single edge
		{
			case PWM1: 	LPC_PWM1->PCR |= PWMENA1;
						break;
			case PWM2: 	LPC_PWM1->PCR |= PWMENA2;
						break;
			case PWM3: 	LPC_PWM1->PCR |= PWMENA3;
						break;
			case PWM4: 	LPC_PWM1->PCR |= PWMENA4;
						break;
			case PWM5: 	LPC_PWM1->PCR |= PWMENA5;
						break;
			case PWM6: 	LPC_PWM1->PCR |= PWMENA6;
						break;
		}
		LPC_PWM1->TCR = TCR_CNT_EN | TCR_PWM_EN;		
	}
}


void PWM_stop(uint8_t pwm_handle)
{
	if(pwm_handle < PWM_COUNT)
	{
		switch(pwm_handle) //all PWM single edge
		{
			case PWM1: 	LPC_PWM1->PCR |= PWMDISA1;
						break;
			case PWM2: 	LPC_PWM1->PCR |= PWMDISA2;
						break;
			case PWM3: 	LPC_PWM1->PCR |= PWMDISA3;
						break;
			case PWM4: 	LPC_PWM1->PCR |= PWMDISA4;
						break;
			case PWM5: 	LPC_PWM1->PCR |= PWMDISA5;
						break;
			case PWM6: 	LPC_PWM1->PCR |= PWMDISA6;
						break;
		}
		LPC_PWM1->TCR = TCR_RESET;	
	}
}