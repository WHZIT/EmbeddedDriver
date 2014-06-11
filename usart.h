#include <stdint.h>

#define USART_NOT_IN_USE		0
#define USART_IN_USE			1
#define PCONP_REGISTER			0x400FC0C4
#define UART0_START_REGISTER	0x4000C000
#define UART1_START_REGISTER	0x40010000
#define UART2_START_REGISTER	0x40098000
#define UART3_START_REGISTER	0x4009C000
#define UART_LSR_OFFSET			14
#define UART_FIFOLVL_OFFSET		58


/*********************************************************************//**
 * Macro defines for Macro defines for UART interrupt enable register
 **********************************************************************/
#define UART_IER_RBRINT_EN		((uint32_t)(1<<0)) 	/*!< RBR Interrupt enable*/
#define UART_IER_THREINT_EN		((uint32_t)(1<<1)) 	/*!< THR Interrupt enable*/
#define UART_IER_RLSINT_EN		((uint32_t)(1<<2)) 	/*!< RX line status interrupt enable*/
#define UART1_IER_MSINT_EN		((uint32_t)(1<<3))	/*!< Modem status interrupt enable */
#define UART1_IER_CTSINT_EN		((uint32_t)(1<<7))	/*!< CTS1 signal transition interrupt enable */
#define UART_IER_ABEOINT_EN		((uint32_t)(1<<8)) 	/*!< Enables the end of auto-baud interrupt */
#define UART_IER_ABTOINT_EN		((uint32_t)(1<<9)) 	/*!< Enables the auto-baud time-out interrupt */
#define UART_IER_BITMASK		((uint32_t)(0x307)) /*!< UART interrupt enable register bit mask */
#define UART1_IER_BITMASK		((uint32_t)(0x38F)) /*!< UART1 interrupt enable register bit mask */

 typedef enum {
	UART_INTCFG_RBR = 0,	/*!< RBR Interrupt enable*/
	UART_INTCFG_THRE,		/*!< THR Interrupt enable*/
	UART_INTCFG_RLS,		/*!< RX line status interrupt enable*/
	UART1_INTCFG_MS,		/*!< Modem status interrupt enable (UART1 only) */
	UART1_INTCFG_CTS,		/*!< CTS1 signal transition interrupt enable (UART1 only) */
	UART_INTCFG_ABEO,		/*!< Enables the end of auto-baud interrupt */
	UART_INTCFG_ABTO		/*!< Enables the auto-baud time-out interrupt */
} UART_INT_Type;

typedef enum{
	DISABLE = 0,
	ENABLE
}UARTSTATE;


typedef struct {
	uint8_t isInUse; 
	uint8_t handle; 
	uintptr_t start_register;
	LPC_UART_TypeDef *UART_reg;
}USART_MGMT;

uint8_t usart_open(uint8_t number);

uint8_t usart_init(uint8_t handle);

uint8_t usart_configure(uint8_t handle, uint32_t bitrate, uint8_t parity, uint8_t stop_bit, uint8_t data_size, uintptr_t *callback_function, uint8_t callback_watermark); //Bei der verwendung des triggers sollte ein timeout vorgesehen werden (evtl. durch hardware)

uint8_t usart_read(uint8_t handle, uint8_t *destination, uint8_t read_length); //Länge der zu lesenden Daten kann angegeben werden

uint8_t usart_write(uint8_t handle, uint8_t *source, uint8_t length); //asynchron realisieren, wird gerade gesendet müsssen die neuen daten angehängt werden

uint8_t usart_close(uint8_t handle);

uint16_t usart_calculate_divider(uint32_t baudrate);

void usart_int_config(LPC_UART_TypeDef *UARTx, UART_INT_Type UARTIntConfig, UARTSTATE State)

//Available-Funktion