
/***************************************************************************** 
 * @file 	timer.h \n
 * @brief 	Timer Treiber Header \n
 * @version	V1.0 \n
 * @date		2014-06-02 \n
 * @author	112222\INS \n
******************************************************************************/

#ifndef __TIMER_H 
#define __TIMER_H

#include "lpc17xx.h"

#define MAXTIMER 	4 //!< Maximal moegliche Anzahl der Timer 
#define TIMER0  	0
#define TIMER1		1
#define TIMER2		2
#define TIMER3		3
#define PIN2 			2
#define TIME_INTERVAL	(9000000/100 - 1)

typedef struct{
	uint8_t USED;
	uint8_t TimerEnable;
	uint8_t TimerNumber;
}TIMER;

typedef TIMER* TIMERHANDLE;

extern TIMERHANDLE timer_open	(uint8_t);
extern uint8_t timer_init			(TIMERHANDLE , uint32_t);
extern uint8_t start_timer		(TIMERHANDLE);
extern uint8_t stop_timer			(TIMERHANDLE);
extern uint8_t reset_timer		(TIMERHANDLE);
extern void TIMER0_IRQHandler (void);
extern void TIMER1_IRQHandler (void);
extern void TIMER2_IRQHandler (void);
extern void TIMER3_IRQHandler (void);

#endif