/**
* @file			SPI.c 
* @brief    Treiber zum einlesen der Key Matrix und Darstellung an der 7-Seg. Anzeige \n
* @version	V1.0 
* @date			2014-05-31
* @author 	112222\INS \n
*		   			Westsächsische Hochschule Zwickau \n
*		   			Fakultät Elektrotechnik - Informationstechnik \n
*/ 

#include "lpc17xx.h"
#include "type.h"
#include "spi.h"

/// @Pointer Adresse auf Typedef Pointer wird Max. Anzahl der Master aufgespannte
SPI_MASTER SSP_MASTER[SPIMaster_Number];					

extern volatile  unsigned char ucRx_byte ;  //!< sichert den Tastaturwert im Interrupt-Handler SPI

/**
* @brief  Moeglichkeit verwendete Ports auzuwerten 
* Aufruf wird in den Funktionen init und close des SPI-Treiber verwendet
*/
LPC_GPIO_TypeDef *GPIO_GetPointer(uint8_t portNum) 
{
	LPC_GPIO_TypeDef *pGPIO = NULL;

	switch (portNum) {
	case 0:
		pGPIO = LPC_GPIO0;
		break;
	case 1:
		pGPIO = LPC_GPIO1;
		break;
	case 2:
		pGPIO = LPC_GPIO2;
		break;
	case 3:
		pGPIO = LPC_GPIO3;
		break;
	case 4:
		pGPIO = LPC_GPIO4;
		break;
	default:
		break;
	}

	return pGPIO;
}

/******************SPI_open*****************************************/
/**
 * @brief	Exlusive Reservierung des SPI Master für LPC1768
 * Wahl des SPI-Master und zugehoerige SPI-Slave
 * @return	Addresse auf Masterhandle
 * @param   SSP Masternummer
 * @param   SSPO Slave Handle
 * @param   SSP1 Slave Handle 
 *******************************************************************/
SPIMASTERHANDLE SPI_open(uint8_t SSPNumber, SPISLAVE0HANDLE SSP0Slave, SPISLAVE1HANDLE SSP1Slave)
{
	if(SSPNumber < SPIMaster_Number)
	{
		if(SSP_MASTER[SSPNumber].USED == FALSE)
		{
				switch(SSPNumber)
				{
					case SSPMASTER0: 	SSP_MASTER[SSPNumber].USED = 1;
														SSP_MASTER[SSPNumber].SPI_SLAVESHANDLES = SSP0Slave;
														break;
					case SSPMASTER1: 	SSP_MASTER[SSPNumber].USED = 1;
														SSP_MASTER[SSPNumber].SPI_SLAVESHANDLES = SSP1Slave;
														break;
				}return(&SSP_MASTER[SSPNumber]);
			}
	}
	return(NULL);
}

/******************SPI_close*****************************************/
/**
 * @brief	Exlusive Reservierung des SPI Master für LPC1768
 * Wahl des SPI-Master und zugehoerige SPI-Slave\n
 * @return	Addresse auf Masterhandle
 * @param   SSP Masternummer
 * @param   SSPO Slave Handle
 * @param   SSP1 Slave Handle 
 *******************************************************************/
uint8_t SPI_close(uint8_t SSPNumber, SPISLAVE0HANDLE SSP0Slave, SPISLAVE1HANDLE SSP1Slave)
{
	int8_t SlaveCounter;
	uint8_t i;
	LPC_GPIO_TypeDef *pGPIO;
	
	if(SSP_MASTER[SSPNumber].USED == TRUE)
	{
		switch(SSPNumber)
		{
			case SSPMASTER0: 	SSP_MASTER[SSPNumber].USED = 0;
												SlaveCounter = sizeof(*(SSP0Slave));
												for(i=0;i<SlaveCounter;i++)
												{
													pGPIO = GPIO_GetPointer(SSP_MASTER[SSPNumber].SPI_SLAVESHANDLES[SlaveCounter].GPIO_PORT_CS);
													pGPIO->FIOSET |= (1<<SSP_MASTER[SSPNumber].SPI_SLAVESHANDLES[SlaveCounter].GPIO_PIN_CS);
												}
												return(TRUE);
			case SSPMASTER1: 	SSP_MASTER[SSPNumber].USED = 0;
												SlaveCounter = sizeof(*(SSP1Slave));
												for(i=0;i<SlaveCounter;i++)
												{
													pGPIO = GPIO_GetPointer(SSP_MASTER[SSPNumber].SPI_SLAVESHANDLES[SlaveCounter].GPIO_PORT_CS);
													pGPIO->FIOSET |= (1<<SSP_MASTER[SSPNumber].SPI_SLAVESHANDLES[SlaveCounter].GPIO_PIN_CS);
												}
												return(TRUE);
		}return(1);
	}
	return(1);
}

/******************SPI_init*****************************************/
/**
 * @brief	Hardware-Konfiguration LPC1768
 * Setzen der Portrichtung, Portfunktionen und SPI-Konfiguration \n
 * @return	Wert ob erfolreiche oder nicht erfolgreiche Ausfuehrung \n
 * @param   Zeiger auf Masterhandle
 *******************************************************************/
uint8_t SPI_init(SPIMASTERHANDLE SSP)
{			
		uint8_t count;
		uint8_t arraycounter;
		LPC_GPIO_TypeDef *pGPIO;
	  // Anzahl der Slaves
		count = sizeof(*(SSP->SPI_SLAVESHANDLES)); 
		if(count != 0)
		{
			for(arraycounter=0; arraycounter<count; arraycounter++) // Konfigurieren aller Ports der Slaves
			{
				pGPIO = GPIO_GetPointer(SSP->SPI_SLAVESHANDLES[arraycounter].GPIO_PORT_CS);
				
				pGPIO->FIODIR |= (1<<SSP->SPI_SLAVESHANDLES[arraycounter].GPIO_PIN_CS);
				pGPIO->FIOSET |= (1<<SSP->SPI_SLAVESHANDLES[arraycounter].GPIO_PIN_CS);
			}
		}	
				
  	LPC_PINCON->PINSEL4 |=(1<<GPIOPIN26);	// P2.13 -> /EINT3	für KEY MATRIX
			
	  if(SSP != NULL)
		{			
			switch(SSP->SPI_Number)
			{// Hardware-Konfiguration SPI-Master SSP
				case SSPMASTER0:	//pin connect block
													LPC_PINCON->PINSEL3 |=SCK0EN;					//P1.20 -> SCK0
													LPC_PINCON->PINSEL3 |=MISO0;					//P1.23 -> MISO0
													LPC_PINCON->PINSEL3 |=MOSI0;					//P1.24 -> MOSI0
						
													LPC_SC->PCONP       |= PCSSP0;				// enable power SSP0
													LPC_SC->PCLKSEL1    |= 0x00000000; 		// PCLKSP0 = CCLK/4 = 72MHz/4 = 18MHz
													LPC_SSP0->CPSR       = 36;            // 18MHz / 36 => 500 KBaud
													LPC_SSP0->CR0				|= CPOL_HIGH | CPHA_BACK_TO | DSS_8BIT;		// CPHA=1, CPOL=1, Bit 3:0 data size: 8bit
													LPC_SSP0->CR1       |= SSEEnable;	  	// Enable SSP0				
													break;
				case SSPMASTER1: 	
													LPC_PINCON->PINSEL3 |=SCK1EN;					//P0.07 -> SCK1
													LPC_PINCON->PINSEL0 |=MISO1;					//P0.08 -> MISO1
													LPC_PINCON->PINSEL0 |=MOSI1;					//P0.09 -> MOSI1
				
													LPC_SC->PCONP       |= PCSSP1;
													LPC_SC->PCLKSEL0    |= 0x00000000; 		//PCLKSP0 = CCLK/4 = 72MHz/4 = 18MHz
													LPC_SSP1->CPSR       = 36;            // 18MHz / 36 => 500 KBaud
													LPC_SSP1->CR0				|= CPOL_HIGH | CPHA_BACK_TO | DSS_8BIT;		// CPHA=1, CPOL=1, Bit 3:0 data size: 8bit
													LPC_SSP1->CR1       |= SSEEnable;	  	// Enable SSP1
													break;
			}
			void Initial_Interrupt();
			return(TRUE);
	}
	return(FALSE);
			
	
}
/*******************************************************************/
/**
 * @brief		Interrupt-Handler Tastatur auslesen
 * @return	ASCII-Wert der gedrückten Taste wird in ucRx_byte geschrieben
 * @param 	void
 *******************************************************************/
//SPI_KEYMATRIX_read INTERRUPT
void  SPI_KEYMATRIX_read  (void)						//Int in startup_LPC17xx aendern
{		//EINT3 Vorberitung in Init-Fkt
    CS_KEYS_EN;															// Chipselect Tastatur Lowpegel
    LPC_SSP0->DR = 0x00; 										// Dummy byte ins SR schreiben -> Start SPI
  	while ((LPC_SSP0->SR & 0x10) != 0);     // Ende der Datenübertragung -> wait BYS for transfer to finish 
  	CS_KEYS_DIS;														// Chipselect Tastatur Highpegel
    ucRx_byte = LPC_SSP0->DR;               // Tastaturwert puffern 
    LPC_SC->EXTINT |= 0x08; 								// clear interrupt flag	 
}
/***********************SPI_7Seg_write*************************************/
/**
 * @brief	Setzen einer 7-Segment-Anzeige
 * @note  Der Zugriff auf die SPI muss atomar erfolgen, da mehrere Slaves parallel am SPI Master angeschlossen!
 * @param	uchPosition Nummer des Ausgabesegments [0..7]
 * @param	uchValue Daten (siehe Aufbau 7-Segment-Anzeige --> #SSEG_THO und folgende)
 * @return	none
 *******************************************************************/
void SPI_7Seg_write(unsigned char uchPosition, unsigned char uchValue)
{    
  unsigned char ucDummy = 0;                  // Hilfsvariable zum Leeren des Puffers
    
	fnEnterCritSec();                           // nachfolgenden Schritte werden nie unterbrochen 
  CS_SSEG_EN;
	LPC_SSP0->DR = uchPosition; 								// erstes Byte ins SR schreiben	-> Start SPI
	while ((LPC_SSP0->SR & 0x02) == 0);					// Buffer prüfen -> TNF = 1: transmit buffer not full 
	LPC_SSP0->DR = uchValue; 										// zweites Byte ins SR schreiben -> Start SPI
	while ((LPC_SSP0->SR & 0x10) != 0);      		// Ende der Datenübertragung -> wait BYS for transfer to finish
	while ((LPC_SSP0->SR & 0x04) != 0)      		// Empfangspuffer löschen --> RNE = 1: receive buffer not empty
	{
		ucDummy	= LPC_SSP0->DR; 									// Dummy-Transmission bis Empfangspuffer leer ist
  	}
	CS_SSEG_DIS;
  fnLeaveCritSec();                           // Ende atomarer Ablauf 
}
/*****************Initial_Interrupt**********************************/
/**
 * @brief	Interrupt-Konfiguration LPC1768
 * Konfiguration externer Interrupt und Freigabe \n
 * Port 2.13, flankengesteuert, Trigger	auf fallende Flanke
 * @return	void
 * @param   void
 *******************************************************************/
void Initial_Interrupt()
{
  LPC_SC->EXTMODE  |= 0x08;										// Flankengesteuert
  LPC_SC->EXTPOLAR |= 0x08;		   							// rising edge
  NVIC_SetPriority(EINT3_IRQn,50);			
  NVIC_EnableIRQ(EINT3_IRQn);
  LPC_SC->EXTINT |= 0x08;
}

