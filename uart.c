#include "uart.h"                    
#include "lcd.h"                     
#include "types.h"                   
#include <LPC21xx.H>                 

/* ==========================================================================
 * DRIVER BUFFER STORAGE & REPLICATED SYSTEM PARMETERS
 * ========================================================================== */
char ch, dummy;                       
extern u8 i, rec_buf[25], frame_ready; 

/**
 * @brief Arms, configures, and schedules operational profiles for UART Line 0
 */
void init_uart0(void)
{
        PINSEL0 &= ~(15<<0);         // Force unlock operational configurations for Tx0/Rx0 pins
        PINSEL0 |= (TXD0_PIN|RXD0_PIN); // Restructure port architecture assignments explicitly to UART

        U0LCR = (1<<DLAB);           // Engage Divisor Latch Access Bit logic latch lock
        U0LCR |= (WORD_LENGTH_SELECT); // Formulate character payload formatting configuration (8-bit)
        
        U0DLM = DIVISOR>>8;          // Load computed Baud frequency coefficient high byte bits
        U0DLL = DIVISOR;             // Load computed Baud frequency coefficient low byte bits

        U0LCR &= ~(1<<DLAB);         // Drop Divisor Latch Access Bit to lock in configurations

        /* --- VECTOR INTERRUPT CONTROLLER ASSIGNMENT ROUTINES --- */
        VICIntSelect = 0x00000000;   // Set all vector slots explicitly to IRQ processing mode
        VICVectAddr0 = (unsigned)UART0_isr; // Bind driver address profile to Priority Vector Slot 0
        VICVectCntl0 = 0x00000020 | 6; // Establish Slot 0 as active, connecting source Line Channel 6 (UART0)
        
        U0IER = 0X01;                // Arm Receive Buffer Storage Interrupt updates
        VICIntEnable = 1<<6;         // Assert Master Core Enable bit mask over System Interrupt Channel 6
}

/**
 * @brief Hardware Vector Interrupt Service Routine for serial transceiver channels
 */
void UART0_isr(void) __irq
{
        if((U0IIR & 4) == 4)         // Interrogate Interrupt ID identification register for Data Ready flag
        {
                rec_buf[i] = U0RBR;  // Siphon pending payload byte straight from Receiver Buffer Register

                if(rec_buf[i] == 0x0A) // Trap line terminator checks (Line Feed tracking)
                {
                        frame_ready = 1; // Assert volatile signaling condition status flag
                }
                i++;
        }
        else
        {
                dummy = U0IIR;       // Purge unhandled or stale line status metrics
        }

        VICVectAddr = 0;             // Force clear prioritized execution tracking markers in VIC hardware
}

/**
 * @brief Basic data push channel primitive streaming outbound lines
 */
void u0_Tx_byte(unsigned char byte)
{
        U0THR = byte;                // Inject candidate token directly down into Transmit Holding Register
        while(((U0LSR>>TEMT_BIT)&1)==0); // Stall operational sequence thread while Transmit FIFO unloads
}

/**
 * @brief Blocking hardware read implementation for intercepting incoming characters
 */
unsigned char u0_Rx_byte(void)
{
        while(((U0LSR>>DR_BIT)&1)==0); // Wait on Line Status Data Ready monitoring flag indicators
        return U0RBR;                // Pull cleared character content straight out from hardware buffers
}

/**
 * @brief Streams array data string pointers onward to external listening lines
 */
void u0_Tx_str(char *str)
{
         while(*str)                 
         {
                  u0_Tx_byte(*str++); // Loop stream character sequences forward across interface lines
         }
}

/**
 * @brief Listens for incoming remote string vectors from terminal connections
 */
char* u0_Rx_str(void)
{
        static char str[50];         // Persistent workspace allocation preventing stack fragmentation
        char i = 0;                  

        do
        {
                str[i] = u0_Rx_byte(); // Capture indexed character token
                u0_Tx_byte(str[i]);  // Mirror response token straight back to verify terminal connectivity

                if((str[i]=='\r') || (str[i]=='\n')) // Interrogate character metrics for carriage returns
                {
                        break;       // Escape dynamic assembly frame sequence constraints
                }
                i++;
        }while(1);

        str[i]='\0';                 // Seal text block boundaries with accurate null terminator configurations
        return str;                  
}