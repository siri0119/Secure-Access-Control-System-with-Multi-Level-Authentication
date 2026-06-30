#include <lpc21XX.h>                 // LPC214x Register Definitions
#include "macros.h"                  // Bit manipulation wrappers
#include "types.h"                   // Standard data type aliases
#include "delay.h"                   // System timing routines
#include "lcd.h"                     // Alphanumeric LCD controller definitions

/**
 * @brief Low-level primitive to stream raw bytes to the HD44780 controller
 */
void write_lcd(u8 data)
{
         G_SETBIT(IOCLR0,RW);        // Flag operational mode: Write (RW = 0)
         WBYTE(IOPIN0,DATA,data);    // Assert 8-bit payload onto configured data lines
         G_SETBIT(IOSET0,EN);        // Raise Enable to latch-high state
         delay_us(1);                // Fulfill data setup time constraint
         G_SETBIT(IOCLR0,EN);        // Fall Enable low to complete latch execution
         delay_ms(2);                // Wait out execution delay for busy controller
}

/**
 * @brief Routes execution parameter directives to the display logic
 */
void cmd_lcd(u8 cmd)
{
        G_SETBIT(IOCLR0,RS);         // Map instruction register (RS = 0)
        write_lcd(cmd);              // Send command payload
}

/**
 * @brief Executes the formal hardware power-up initialization sequence
 */
void init_lcd()
{
        WBYTE(IODIR0,DATA,0XFF);     // Allocate data lines as digital outputs
        SETBIT(IODIR0,RS);           // Allocate Register Select pin as output
        SETBIT(IODIR0,RW);           // Allocate Read/Write pin as output
        SETBIT(IODIR0,EN);           // Allocate Enable strobe pin as output

        delay_ms(15);                // Post-power stabilization window
        cmd_lcd(MODE_8BIT_LINE1);    // Step 1: Force controller wake-up
        delay_ms(5);                 // Command execution padding
        cmd_lcd(0x30);               // Step 2: Affirm interface width
        delay_us(100);               // Microsecond recovery stall
        cmd_lcd(MODE_8BIT_LINE1);    // Step 3: Re-verify operational status
        
        cmd_lcd(MODE_8BIT_LINE2);    // Multi-line rendering active (8-bit bus)
        cmd_lcd(DISP_ON);            // Power matrix grid elements
        cmd_lcd(LCD_CLR);            // Clear background buffers
        cmd_lcd(DISP_SHIFT_OFF_INC); // Auto-advance cursor rightwards
}

/**
 * @brief Prints an isolated ASCII symbol onto the screen grid
 */
void char_lcd(u8 character)
{
        SETBIT(IOSET0,RS);           // Map character data register (RS = 1)
        write_lcd(character);        // Stream character payload
}

/**
 * @brief Decodes and streams a standard C string terminal buffer
 */
void str_lcd(u8 *p)
{
        while(*p)                    // Interrogate for null-terminator string bound
                char_lcd(*p++);      // Push character pointer and step index forward
}

/**
 * @brief Unrolls an unsigned 32-bit integer array into human-readable text
 */
void u32_lcd(u32 n)
{
        u8 a[10];                    // Temporary extraction stack for maximum digits
        s32 i=0;                     // Element cursor index

        if(n==0)                     
        {
                char_lcd('0');       // Handle base exception
        }
        else
        {
                while(n)             
                {
                        a[i]=(n%10)+48; // Capture remainder; lift to ASCII mapping
                        i++;         
                        n/=10;       // Scale integer down down by 1 radix base
                }
                for(--i;i>=0;i--)    // Empty tracking stack in FIFO chronological order
                {
                        char_lcd(a[i]); 
                }
        }
}

/**
 * @brief Decodes floating-point arithmetic with configurable precision limits
 */
void float_lcd(f32 f,s32 np)
{
        s32 i;                       

        if(f<0)                      // Manage negative parameters
        {
                char_lcd('-');       
                f*=-1;               // Flip inversion sign
        }

        i=f;                         // Extract the whole integer segment
        u32_lcd(i);                  // Flush whole number out first
        char_lcd('.');               // Hardcode decimal notation indicator
        f = f-i;                     // Strip real values to fractional delta remnants

        while(np)                    // Scale decimal accuracy bounds
        {
                f*=10;               // Push precision numbers above the radix line
                np--;                
        }

        i = f;                       // Isolate normalized decimal values
        u32_lcd(i);                  // Render precise post-decimal value
}

/**
 * @brief Decodes a signed 32-bit scalar value onto the visual grid
 */
void s32_lcd(s32 n)
{
        if(n<0)                      // Validate polarity
        {
                char_lcd('-');       
                n*=-1;               // Transform value absolute
        }
        u32_lcd(n);                  
}

/**
 * @brief Handles UI splash animations for the main security gateway system
 */
void title(void)
{
        u8 i;
        char line1[] = " SECURE ACCESS "; 
        char line2[] = " CONTROL SYSTEM "; 

        cmd_lcd(0x01);               // Purge screen memory maps
        
        // Animated print sequence for System Banner Line 1
        for(i=0;line1[i]!=0;i++)
        {
                cmd_lcd(GOTO_LINE1_POS_0+i);  
                delay_ms(100);
                char_lcd(line1[i]);
                delay_ms(100);  
        }
        
        // Animated print sequence for System Banner Line 2
        for(i=0;line2[i]!=0;i++)
        {
                cmd_lcd(GOTO_LINE2_POS_0+i);   
                delay_ms(100);		
                char_lcd(line2[i]); 
                delay_ms(100);
        }
}
