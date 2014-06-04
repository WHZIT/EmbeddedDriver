/*******************************************************************/
/**
 * @file		spi.h
 * @brief		SPI Treiber  \n
 * 					Spricht 7-Seg-Anzeige und Key Matrix an \n
 * @version	V1.0
 * @date		2014-05-31
 * @author 	112222\INS \n
 *		   		Westsächsische Hochschule Zwickau \n
 *		   		Fakultät Elektrotechnik - Informationstechnik \n
 *******************************************************************/

#ifndef __SPI_H
#define __SPI_H

#include "lpc17xx.h"

/*FIOPIN: Fast Port Pin value register using FIOMASK. The current state
* of digital port pins can be read from this register, regardless of
* pin direction or alternate function selection (as long as pins are
* not configured as an input to ADC).
*
* FIOSET: Fast Port Output Set register using FIOMASK. This register
* controls the state of output pins. Writing 1s produces highs at
* the corresponding port pins. Writing 0s has no effect. Reading
* this register returns the current contents of the port output
* register. Only bits enabled by 0 in FIOMASK can be altered.
*
* FIOCLR: Fast Port Output Clear register using FIOMASK. This register
* controls the state of output pins. Writing 1s produces lows at
* the corresponding port pins. Writing 0s has no effect. Only bits
* enabled by 0 in FIOMASK can be altered.
*
* PINSEL: The PINSEL4 register controls the functions of the lower half of Port 2.
*/


#define  GPIOPIN26 26
#define  GPIOPIN21 21
#define  GPIOPIN20 20
#define  GPIOPIN17 17
#define  GPIOPIN16 16
#define  GPIOPIN15 15
#define  GPIOPIN14 14
#define  GPIOPIN13 13
#define	 GPIOPIN10 10
#define  GPIOPIN09 9
#define  GPIOPIN08 8
#define  GPIOPIN07 7
#define  GPIOPIN06 6
#define  GPIOPIN02 2
#define  GPIOPIN01 1
#define  GPIOPIN00 0


#define  EINT_KEYS_ACTIVE		((LPC_GPIO2->FIOPIN |= (1<<GPIOPIN13)) == 0) //!<1/0-Flanke an P2.13 -> neuer Tastenwert liegt bereit
#define  EINT_KEYS_INACTIVE	((LPC_GPIO2->FIOPIN |= (1<<GPIOPIN13)) != 0) //!<High-Pegel an P2.13 -> kein Interrupt
#define  CS_SSEG_DIS				(LPC_GPIO2->FIOSET |= (1<<GPIOPIN06))				 //!<High-Pegel an P2.06 -> Slave 7-SEG-Anzeige disable)	
#define  CS_SSEG_EN					(LPC_GPIO2->FIOCLR |= (1<<GPIOPIN06))				 //!<Low-Pegel an P2.06  -> Slave 7-SEG-Anzeige enable)	
#define  CS_KEYS_DIS				(LPC_GPIO2->FIOSET |= (1<<GPIOPIN07)) 			 //!<High-Pegel an P2.07 -> Slave Tastatur disable)		
#define  CS_KEYS_EN					(LPC_GPIO2->FIOCLR |= (1<<GPIOPIN07)) 			 //!<Low-Pegel an P2.07  -> Slave Tastatur enable)		


//	Adressierung der Balken bei 7-Segment-Anzeigen	
//		    0x01
//	      ----
//	0x20 |    | 0x02
//       |0x40|
//		    ----
//	0x10 |    | 0x04
//       |0x08|
//		    ----	(.)0x80	

#define SSEG_THO 0x01 //!<Balken oben horizontal
#define SSEG_TRI 0x02 //!<Balken oben rechts
#define SSEG_BRI 0x04 //!<Balken unten rechts
#define SSEG_BHO 0x08 //!<Balken unten horizontal 
#define SSEG_BLE 0x10 //!<Balken unten links
#define SSEG_TLE 0x20 //!<Balken oben rechts
#define SSEG_MHO 0x40 //!<Balken mitte horizontal
#define SSEG_DOT 0x80 //!<Punkt unten rechts

/////////////ZIFFERN Wochentag/Uhrzeit///////////////
#define SSEG_SIGN_1 (SSEG_TRI | SSEG_BRI) 
#define SSEG_SIGN_2 (SSEG_THO | SSEG_TRI | SSEG_MHO | SSEG_BLE | SSEG_BHO)
#define SSEG_SIGN_3 (SSEG_THO | SSEG_TRI | SSEG_MHO | SSEG_BRI | SSEG_BHO)
#define SSEG_SIGN_4 (SSEG_TLE | SSEG_TRI | SSEG_MHO | SSEG_TLE | SSEG_BRI)
#define SSEG_SIGN_5 (SSEG_THO | SSEG_TLE | SSEG_MHO | SSEG_BRI | SSEG_BHO)
#define SSEG_SIGN_6 (SSEG_THO | SSEG_TLE | SSEG_MHO | SSEG_BLE | SSEG_BHO | SSEG_BRI)
#define SSEG_SIGN_7 (SSEG_THO | SSEG_TLE | SSEG_TRI | SSEG_BRI)
#define SSEG_SIGN_8 (SSEG_THO | SSEG_TLE | SSEG_TRI | SSEG_MHO | SSEG_BLE | SSEG_BHO | SSEG_BRI)
#define SSEG_SIGN_9 (SSEG_THO | SSEG_TLE | SSEG_TRI | SSEG_MHO | SSEG_BRI | SSEG_BHO )
#define SSEG_SIGN_0 (SSEG_THO | SSEG_TLE | SSEG_TRI | SSEG_BLE | SSEG_BHO | SSEG_BRI)

#define SSEG_SIGN_A (SSEG_THO | SSEG_TLE | SSEG_TRI | SSEG_MHO | SSEG_BLE | SSEG_BRI)
#define SSEG_SIGN_D (SSEG_TRI | SSEG_MHO | SSEG_BLE | SSEG_BHO | SSEG_BRI)
#define SSEG_SIGN_E (SSEG_THO | SSEG_TLE | SSEG_MHO | SSEG_BLE | SSEG_BHO)
#define SSEG_SIGN_F (SSEG_THO | SSEG_TLE | SSEG_MHO | SSEG_BLE)
#define SSEG_SIGN_I (SSEG_TRI | SSEG_BRI) 
#define SSEG_SIGN_M (SSEG_BLE | SSEG_MHO | SSEG_BRI )

#define SSEG_SIGN_O (SSEG_THO | SSEG_TRI | SSEG_BRI | SSEG_BHO | SSEG_BLE | SSEG_TLE )
#define SSEG_SIGN_R (SSEG_THO | SSEG_TLE | SSEG_TRI | SSEG_MHO | SSEG_BLE | SSEG_BRI)
#define SSEG_SIGN_S (SSEG_THO | SSEG_TLE | SSEG_MHO | SSEG_BRI | SSEG_MHO | SSEG_BHO)
#define SSEG_SIGN_U (SSEG_TLE | SSEG_BLE | SSEG_BHO | SSEG_BRI | SSEG_TRI)

#define SSEG_SIGN_d (SSEG_MHO | SSEG_BLE | SSEG_BHO | SSEG_BRI | SSEG_TRI)
#define SSEG_SIGN_h (SSEG_TLE | SSEG_MHO | SSEG_BLE | SSEG_BRI)
#define SSEG_SIGN_short_m_left (SSEG_BLE | SSEG_MHO | SSEG_BRI )
#define SSEG_SIGN_short_m_right (SSEG_BLE | SSEG_MHO | SSEG_BRI )
#define SSEG_SIGN_r (SSEG_BLE | SSEG_MHO)

// Zurodnung der Taster der Key-Matrix zum Symbolischen Abbild
#define SSEG_SIGN_DOWN (SSEG_TLE | SSEG_BLE | SSEG_BHO) 
#define SSEG_SIGN_UP (SSEG_BRI | SSEG_BLE | SSEG_TLE | SSEG_THO) 
#define SSEG_SIGN_OK (SSEG_BLE | SSEG_TLE) 
#define SSEG_SIGN_Betrieb (SSEG_TRI | SSEG_MHO) 
#define SSEG_SIGN_Einstellung (SSEG_THO | SSEG_MHO | SSEG_BRI)  
#define SSEG_SIGN_Funktion (SSEG_TRI | SSEG_BRI | SSEG_MHO)  

// Defines zur Registermaipulation 
#define CPOL_HIGH			(1<<6) //!<CPOL clock out polarity -  clock high between frames
#define CPHA_BACK_TO 	(1<<7) //!<Clock out phase - transition back to the inter_frame state of clk-line
#define DSS_8BIT			(1<<2) | (1<<1) | (1<<0) //!<8-Bit Transfer
#define	SSPMASTER0		0
#define	SSPMASTER1		1
#define SSEEnable			(1<<1)
#define PCSSP0        (1<<21)
#define PCSSP1        (1<<10)
#define SCK0EN				(1<<9) | (1<<8)
#define MISO0 				(1<<15) | (1<<14)
#define MOSI0					(1<<17) | (1<<16)
#define MISO1 				(1<<17) | (0<<16)
#define MOSI1 				(1<<19) | (0<<18)
#define SCK1EN				(1<<15) | (0<<14)

#define GPIO_PORT0				0
#define GPIO_PORT1				1
#define GPIO_PORT2				2
#define GPIO_PORT3				3
#define GPIO_PORT4				4
#define SPIMaster_Number  2

/*******************************************************************/
/**
 * @brief	SPI_Slave typedef
 *******************************************************************/
typedef struct {
	uint8_t GPIO_PIN_CS;     	//!< Pinzuordnung
	uint8_t GPIO_PORT_CS;			//!< Portzuordnung
}SPI_SLAVE;

/*******************************************************************/
/**
 * @brief	SPI_Master typedef \n
 *******************************************************************/
typedef struct {
	uint8_t USED;						//!< Abfrage ob Master bereits verwendet
	uint8_t SPI_Number;			//!< gibt ausgewehlten SPI-Master an
	SPI_SLAVE* SPI_SLAVESHANDLES; //!< verweisst auf zugeordneten Slave 
}SPI_MASTER;

/*******************************************************************/
/**
 * @typedef Zeiger Handle auf Master und zwei Slaves
 * @brief	SPI_Master_Slave Zeiger  \n
 *******************************************************************/
typedef SPI_MASTER* SPIMASTERHANDLE;
typedef SPI_SLAVE*  SPISLAVE0HANDLE;
typedef SPI_SLAVE*  SPISLAVE1HANDLE;

extern SPIMASTERHANDLE SPI_open(uint8_t, SPISLAVE0HANDLE, SPISLAVE1HANDLE);
extern uint8_t SPI_init(SPIMASTERHANDLE);
extern void SPI_KEYMATRIX_read();
extern void SPI_7Seg_write(unsigned char, unsigned char);
extern void Initial_Interrupt();
extern uint8_t SPI_close(uint8_t, SPISLAVE0HANDLE, SPISLAVE1HANDLE);

/*******************************************************************/
/**
 * @define fnEnterCritSec
 * @brief	Eintritt in kritischen Programmbereich
 * Beim Aufruf der Funktion fn_SpiSsegWrite darf kein Wert kein 
 * externer Interrupt ausgeführt werden.  
 * @return	void 
*******************************************************************/
#define fnEnterCritSec(void) ( NVIC_DisableIRQ(EINT3_IRQn))

/*******************************************************************/
/**
 * @define fnLeaveCritSec
 * @brief	Verlassen des kritischen Programmbereich
 * Nach Beendigung der Funktion fn_SpiSsegWrite wird der externe\n
 * Interrupt wieder freigegeben.  
 * @return	void 
*******************************************************************/
#define fnLeaveCritSec(void) ( NVIC_EnableIRQ(EINT3_IRQn))

#endif
