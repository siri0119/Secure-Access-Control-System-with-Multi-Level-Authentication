// Start header guard
#ifndef _UART_H_

// Define header guard
#define _UART_H_


// Oscillator frequency
#define FOSC 12000000

// PLL value
#define PLL  5

// CPU clock
#define CCLK (FOSC*PLL)

// Peripheral clock
#define PCLK (CCLK/4)

// UART baud rate
#define BAUD 9600

// Baud rate divisor
#define DIVISOR (PCLK/(16*BAUD))


// TXD0 pin function
#define TXD0_PIN 0x00000001

// RXD0 pin function
#define RXD0_PIN 0x00000004


// DLAB bit position
#define DLAB 7

// 8-bit value
#define _8BIT 3

// UART word length
#define WORD_LENGTH_SELECT _8BIT


// Transmitter empty bit
#define TEMT_BIT 6

// Data ready bit
#define DR_BIT 0


// Initialize UART0
void init_uart0(void);

// Send one byte
void u0_Tx_byte(unsigned char byte);

// Receive one byte
unsigned char u0_Rx_byte(void);

// Send string
void u0_Tx_str(char *str);

// Receive string
char* u0_Rx_str(void);

// UART0 interrupt function
void UART0_isr(void) __irq;


// End header guard
#endif
