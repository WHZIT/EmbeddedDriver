/***************************************************************************** 
 * @file timer.c \n
 * @brief Timer Treiber C-Datei \n
 * @version	V1.0 \n
 * @date	2011-03-11 \n
 * @author	112222\INS \n
******************************************************************************/
#include "lpc17xx.h"
#include "type.h"
#include "timer.h"

volatile uint32_t timer0_counter = 0;
volatile uint32_t timer1_counter = 0;
volatile uint32_t timer2_counter = 0;
volatile uint32_t timer3_counter = 0;

TIMER timer_struct[MAXTIMER];

/***********************timer_open*************************************/
/**
 * @brief		Exlusive Reservierung der Timer für LPC1768
 * @param		Timer Nummmer [0..3]
 * @param		uchValue Daten (siehe Aufbau 7-Segment-Anzeige --> #SSEG_THO und folgende)
 * @return	Addresse auf Masterhandle
 **********************************************************************/
TIMERHANDLE timer_open(uint8_t TimerNumber)
{
	if(TimerNumber < MAXTIMER)
	{
		if(timer_struct[TimerNumber].USED == FALSE)
		{
			switch(TimerNumber)
			{
				case TIMER0:	timer_struct[TimerNumber].USED = TRUE;
								timer_struct[TimerNumber].TimerNumber = TimerNumber;
								break; 
				case TIMER1:	timer_struct[TimerNumber].USED = TRUE;
								timer_struct[TimerNumber].TimerNumber = TimerNumber;
								break;
				/*case TIMER2:	timer_struct[TimerNumber].USED = TRUE;
								timer_struct[TimerNumber].TimerNumber = TimerNumber;
								break;
				case TIMER3:	timer_struct[TimerNumber].USED = TRUE;
								timer_struct[TimerNumber].TimerNumber = TimerNumber;
								break;
				*/
			}
		return (&(timer_struct[TimerNumber]));	
		}
	}
	return(NULL);
}


/******************Timer_init***************************************/
/**
 * @brief	Hardware-Konfiguration LPC1768
 * Funktion zur Einstellung der Hardware für diese Anwendung 
 * (Systembus und Taktfrequenz sind bereits eingerichtet)
 * Setzen des Interrupt \n
 * Timerintervall wird gesetzt \n
 * Timercounter auf Anfangswert setzten (0) \n
 * @return	Wert ob erfolreiche oder nicht erfolgreiche Ausfuehrung \n
 * @param   Zeiger auf Masterhandle
 * @param   Zeitwert 
 *******************************************************************/
uint8_t timer_init(TIMERHANDLE TimerHandle, uint32_t TimerInterval)
{
	switch(TimerHandle->TimerNumber)
	{//MR0:3 Match Register 
		case TIMER0:	timer0_counter = 0;
						LPC_TIM0->MR0 = TimerInterval;
						LPC_TIM0->MCR = 3;										/* Interrupt and Reset on MR0 */
						NVIC_EnableIRQ(TIMER0_IRQn);
						return (TRUE);
		case TIMER1:	timer1_counter = 0;
						LPC_TIM1->MR1 = TimerInterval;
						LPC_TIM1->MCR = 3;										/* Interrupt and Reset on MR1 */
						NVIC_EnableIRQ(TIMER1_IRQn);
						return (TRUE);
		/*case TIMER2:	timer2_counter = 0;
						LPC_TIM2->MR2 = TimerInterval;
						LPC_TIM2->MCR = 3;										// Interrupt and Reset on MR2 
						NVIC_EnableIRQ(TIMER2_IRQn);
						return (TRUE);
		case TIMER3:	timer3_counter = 0;
						LPC_TIM3->MR3 = TimerInterval;
						LPC_TIM3->MCR = 3;										// Interrupt and Reset on MR3 
						NVIC_EnableIRQ(TIMER3_IRQn);
						return (TRUE);
		*/
	}
	return (FALSE);
}

/******************start_Timer***************************************/
/**
 * @brief	Es wird geprueft ob der zu startende Timer gesetzt ist. \n
 * Wenn Timer zugeordnet und pausierend wird er gestartet\n
 * @return	Wert ob erfolreiche oder nicht erfolgreiche Ausfuehrung \n
 * @param   Zeiger auf Masterhandle
 *******************************************************************/
uint8_t start_timer( TIMERHANDLE TimerHandle )
{
  switch ( TimerHandle->TimerNumber )
  { 
	if(timer_struct[TimerHandle->TimerNumber].TimerEnable == FALSE)
	{
		case TIMER0: 	LPC_TIM0->TCR = 1;    										/* TCR=1: Timer/Counter enable */
						return (TRUE);
		case TIMER1: 	LPC_TIM1->TCR = 1;
						return (TRUE);
		/*case TIMER2: 	LPC_TIM2->TCR = 1;
						return (TRUE);
		case TIMER3: 	LPC_TIM3->TCR = 1;
						return (TRUE);
		*/
	}
  }
  return (FALSE);
}
/******************stop_Timer***************************************/
/**
 * @brief	Es wird geprueft ob der zu stoppende Timer gesetzt ist. \n
 * Wenn Timer zugeordnet und gestartet wird er gestoppt\n
 * @return	Wert ob erfolreiche oder nicht erfolgreiche Ausfuehrung \n
 * @param   Zeiger auf Masterhandle
 *******************************************************************/
uint8_t stop_timer( TIMERHANDLE TimerHandle )
{
  switch ( TimerHandle->TimerNumber )
  {
	if(timer_struct[TimerHandle->TimerNumber].TimerEnable == TRUE)
	{
	case TIMER0: 	LPC_TIM0->TCR = 0;    										/* TCR=0: Timer/Counter disable */
					return (TRUE);
	case TIMER1: 	LPC_TIM1->TCR = 0;
					return (TRUE);
	/*case TIMER2: 	LPC_TIM2->TCR = 0;
					return (TRUE);
	case TIMER3: 	LPC_TIM3->TCR = 0;
					return (TRUE);
		*/
	}				
  }
  return (FALSE);
}

/******************reset_Timer***************************************/
/**
 * @brief	Es wird geprueft ob Timernummer des Handles zugeordnet ist. \n
 * Wenn Timer zugeordnet wird ein Timer Reset durchgefuehrt
 * @return	Wert ob erfolreiche oder nicht erfolgreiche Ausfuehrung \n
 * @param   Zeiger auf Masterhandle
 *******************************************************************/
uint8_t reset_timer( TIMERHANDLE TimerHandle)
{
  uint32_t regVal;

	switch ( TimerHandle->TimerNumber )
	{
		if(timer_struct[TimerHandle->TimerNumber].TimerEnable == TRUE)
		{
		case TIMER0: 	regVal = LPC_TIM0->TCR;
						regVal |= 0x02;
						LPC_TIM0->TCR = regVal;						
						//Pin: 2  
						//Symbol:TCR=Timer Counter Reset 
						//Description:When one, the Timer Counter and the Prescale Counter are synchronously reset on the
						//next positive edge of PCLK. The counters remain reset until TCR[1] is returned to zero.
						LPC_TIM0->TCR = LPC_TIM0->TCR |= (1<<PIN2);
						return (TRUE);
		case TIMER1: 	regVal = LPC_TIM1->TCR;
						regVal |= 0x02;
						LPC_TIM1->TCR = regVal;
						return (TRUE);
		/*case TIMER2: 	regVal = LPC_TIM2->TCR;
						regVal |= 0x02;
						LPC_TIM2->TCR = regVal;
						return (TRUE);
		case TIMER3: 	regVal = LPC_TIM3->TCR;
						regVal |= 0x02;
						LPC_TIM3->TCR = regVal;
						return (TRUE);
			*/
		}		
	}
	return (FALSE);
 }

 //!< TIMER IRQ noch als Funktionszeiger zu aendern
 void TIMER0_IRQHandler (void) 
{  
  LPC_TIM0->IR = 1;											/* clear interrupt flag */
  timer0_counter++;
  return;
}
 
 void TIMER1_IRQHandler (void)  
{  
  LPC_TIM1->IR = 1;											/* clear interrupt flag */
  timer1_counter++;
  return;
}

 /*void TIMER2_IRQHandler (void)  
{  
  LPC_TIM2->IR = 1;											// clear interrupt flag
  timer2_counter++;
  return;
}
 void TIMER3_IRQHandler (void)  
{  
  LPC_TIM3->IR = 1;											// clear interrupt flag
  timer3_counter++;
  return;
}

*/