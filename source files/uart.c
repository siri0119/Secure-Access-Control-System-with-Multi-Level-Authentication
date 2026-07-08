#include "uart.h"
#include "lcd.h"
#include "types.h"
#include <LPC21xx.H>

// Global variables
char ch,dummy;

// External variables
extern u8 i,rec_buf[25],frame_ready;


// Initialize UART0
void init_uart0(void)
{
        // Clear UART pin function bits
        PINSEL0 &= ~(15<<0);

        // Select TXD0 and RXD0 pin functions
        PINSEL0 |= (TXD0_PIN|RXD0_PIN);

        // Enable DLAB bit
        U0LCR = (1<<DLAB);

        // Set UART word length
        U0LCR |= (WORD_LENGTH_SELECT);

        // Set divisor high byte
        U0DLM = DIVISOR>>8;

        // Set divisor low byte
        U0DLL = DIVISOR;

        // Disable DLAB bit
        U0LCR &= ~(1<<DLAB);

        // Select IRQ interrupt
        VICIntSelect = 0x00000000;

        // Set UART0 interrupt function address
        VICVectAddr0 = (unsigned)UART0_isr;

        // Set UART0 interrupt channel
        VICVectCntl0 = 0x20 | 6;

        // Enable UART0 interrupt
        VICIntEnable = 1 << 6;

        // Enable UART0 interrupts
        U0IER = 0x03;
}


// UART0 interrupt function
void UART0_isr(void) __irq
{
        // Check receive interrupt
        if((U0IIR & 0x04))
        {
                // Read received byte
                ch = U0RBR;

                // Store byte in receive buffer
                rec_buf[i++] = ch;

                // Check received byte count
                if(i >= 12)
                {
                        // Set frame ready flag
                        frame_ready = 1;
                }
        }
        else
        {
                // Read interrupt register
                dummy = U0IIR;
        }

        // Clear VIC interrupt
        VICVectAddr = 0;
}


// Send one byte
void u0_Tx_byte(unsigned char byte)
{
        // Store byte in transmit register
        U0THR = byte;

        // Wait until transmission completes
        while(((U0LSR>>TEMT_BIT)&1)==0);
}


// Receive one byte
unsigned char u0_Rx_byte(void)
{
        // Wait until data is received
        while(((U0LSR>>DR_BIT)&1)==0);

        // Return received byte
        return U0RBR;
}


// Send string
void u0_Tx_str(char *str)
{
        // Send until null character
        while(*str)
        {
                // Send one character
                u0_Tx_byte(*str++);
        }
}


// Receive string
char* u0_Rx_str(void)
{
        // Buffer to store string
        static char str[50];

        // Index variable
        char i = 0;

        do
        {
                // Receive one character
                str[i] = u0_Rx_byte();

                // Send received character
                u0_Tx_byte(str[i]);

                // Check ENTER character
                if((str[i]=='\r') || (str[i]=='\n'))
                {
                        // Exit loop
                        break;
                }

                // Increase index
                i++;

        }while((i!=49));

        // Add null character
        str[i-1] = 0;

        // Return string
        return str;
}
