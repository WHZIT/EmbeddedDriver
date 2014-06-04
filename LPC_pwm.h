#ifndef __PWM_H 
#define __PWM_H

#include <stdint.h>
#include <stdlib.h>

#define PWM_CYCLE		1200
#define PWM_OFFSET		200

#define MR0_INT			1 << 0
#define MR1_INT			1 << 1
#define MR2_INT			1 << 2
#define MR3_INT			1 << 3
#define MR4_INT			1 << 8
#define MR5_INT			1 << 9
#define MR6_INT			1 << 10

#define TCR_CNT_EN		0x00000001
#define TCR_RESET		0x00000002
#define TCR_PWM_EN		0x00000008

#define PWMMR0I			1 << 0
#define PWMMR0R			1 << 1
#define PWMMR0S			1 << 2
#define PWMMR1I			1 << 3
#define PWMMR1R			1 << 4
#define PWMMR1S			1 << 5
#define PWMMR2I			1 << 6
#define PWMMR2R			1 << 7
#define PWMMR2S			1 << 8
#define PWMMR3I			1 << 9
#define PWMMR3R			1 << 10
#define PWMMR3S			1 << 11
#define PWMMR4I			1 << 12
#define PWMMR4R			1 << 13
#define PWMMR4S			1 << 14
#define PWMMR5I			1 << 15
#define PWMMR5R			1 << 16
#define PWMMR5S			1 << 17
#define PWMMR6I			1 << 18
#define PWMMR6R			1 << 19
#define PWMMR6S			1 << 20

#define PWMSEL2			1 << 2
#define PWMSEL3			1 << 3
#define PWMSEL4			1 << 4
#define PWMSEL5			1 << 5
#define PWMSEL6			1 << 6

#define PWMENA1			1 << 9
#define PWMENA2			1 << 10
#define PWMENA3			1 << 11
#define PWMENA4			1 << 12
#define PWMENA5			1 << 13
#define PWMENA6			1 << 14

#define PWMDISA1			0 << 9
#define PWMDISA2			0 << 10
#define PWMDISA3			0 << 11
#define PWMDISA4			0 << 12
#define PWMDISA5			0 << 13
#define PWMDISA6			0 << 14

#define LER0_EN			1 << 0
#define LER1_EN			1 << 1
#define LER2_EN			1 << 2
#define LER3_EN			1 << 3
#define LER4_EN			1 << 4
#define LER5_EN			1 << 5
#define LER6_EN			1 << 6


#define PWM_INT 				0x40018000	//PWM Interrupt Register
#define PWM_TIMER_CONTROL 		0x40018004	//PWM Timer Control Register
#define PWM_COUNT_CONTROL 		0x40018070 	//PWM Count Control Register	
#define PWM_MATCH_CONTROL 		0x40018014	//PWM Match Register
#define PWM_CAPTURE_CONTROL 	0x40018028 //PWM Capture Control Register
#define PWM_Control Register 	0x4001 804C //PWM Control Register

#define PWM_USED	 		1
#define PWM_NOT_USED 		0
#define PCONP_REGISTER 		0x400FC0C4
#define PCLKSEL0_REGISTER 	0x400FC1A8
#define PWM_COUNT			6			//Anzahl der möglichen PWM
#define ERROR				0

#define PWM1_CLK_ENABLE		(1<<1)
#define PWM1_CLK_CCLK		(1<<11)	//PCLK_peripheral = CCLK
#define PCLKSELR0			0x400FC1A8 	//Peripheral Clock Selection register 0

enum pwmCount{PWM1, PWM2, PWM3, PWM4, PWM5, PWM6}; //Auswahl für PWMs

//Kontrollstruktur für eine PWM
typedef struct
{
	uint8_t used;
	uint8_t cycle;
	uint8_t handle;
	
}PWMCTRL;

extern uint8_t PWM_open(uint8_t pwmNumber);
extern void PWM_init(uint32_t cycle);
extern void PWM_set(uint8_t pwm_handle, uint32_t cycle);
extern void PWM_start(uint8_t pwm_handle);
extern void PWM_stop(uint8_t pwm_handle);


#endif /* end __PWM_H */
