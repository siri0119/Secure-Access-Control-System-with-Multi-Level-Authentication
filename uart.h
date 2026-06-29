#ifndef _UART_H_                     
#define _UART_H_                     

/* ==========================================================================
 * CLOCK DISTRIBUTION MATRIX: Frequency Layout Arithmetic Configurations
 * ========================================================================== */
#define FOSC 12000000                // Core crystal base frequency value (12 MHz Oscillator input)
#define PLL  5                       // Phase Locked Loop system scaling structural multiplier ratio
#define CCLK (FOSC*PLL)              // Primary Core Processor running speed target (60 MHz Execution space)
#define PCLK (CCLK/4)                // Peripheral Device Bus operating velocity bounds (15 MHz Interface lines)
#define BAUD 9600                    // Target Serial Transfer Speed metric
#define DIVISOR (PCLK/(16*BAUD))     // Computed fractional clock scaling scale setting scalar (For standard 9600 BAUD)

/* ==========================================================================
 * HARDWARE CONFIGURATION: PINSEL Pin Function Allocation Mappings
 * ========================================================================== */
#define TXD0_PIN 0x00000001          // Route Port 0 Pin 0 to active alternative function TXD0
#define RXD0_PIN 0x00000004          // Route Port 0 Pin 1 to active alternative function RXD0

/* ==========================================================================
 * LINE CONTROL STRUCT BIT MASKS & OFFSETS
 * ========================================================================== */
#define DLAB 7                       // Divisor Latch Access Bit pointer switch position
#define _8BIT 3                      // 8-bit character configuration mask frame
#define WORD_LENGTH_SELECT _8BIT     // Assign character length parameters configuration layout

#define TEMT_BIT 6                   // Transmitter Empty Status flag check offset indexing point
#define DR_BIT  0                    // Data Ready Line Status monitor tracker position location

/* ==========================================================================
 * CORE DRIVER SUBROUTINE INTERFACES
 * ========================================================================== */
void init_uart0(void);               
void u0_Tx_byte(unsigned char byte); 
unsigned char u0_Rx_byte(void);      

#endif