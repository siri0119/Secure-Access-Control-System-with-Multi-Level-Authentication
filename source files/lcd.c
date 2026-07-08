#include <lpc21XX.h>
#include "macros.h"
#include "types.h"
#include "delay.h"
#include "lcd.h"


// Write data to LCD
void write_lcd(u8 data)
{
        // Select write operation
        G_SETBIT(IOCLR0,RW);

        // Send data to LCD
        WBYTE(IOPIN0,DATA,data);

        // Set enable pin
        G_SETBIT(IOSET0,EN);

        // Small delay
        delay_us(1);

        // Clear enable pin
        G_SETBIT(IOCLR0,EN);

        // Delay
        delay_ms(2);
}


// Send command to LCD
void cmd_lcd(u8 cmd)
{
        // Select command register
        G_SETBIT(IOCLR0,RS);

        // Send command
        write_lcd(cmd);
}


// Initialize LCD
void init_lcd()
{
        // Set data pins as output
        WBYTE(IODIR0,DATA,0XFF);

        // Set RS pin as output
        SETBIT(IODIR0,RS);

        // Set RW pin as output
        SETBIT(IODIR0,RW);

        // Set EN pin as output
        SETBIT(IODIR0,EN);

        // Delay
        delay_ms(15);

        // Set 8-bit mode
        cmd_lcd(MODE_8BIT_LINE1);

        // Delay
        delay_ms(5);

        // Send command
        cmd_lcd(0x30);

        // Small delay
        delay_us(100);

        // Set 8-bit mode
        cmd_lcd(MODE_8BIT_LINE1);

        // Set two line mode
        cmd_lcd(MODE_8BIT_LINE2);

        // Turn ON display
        cmd_lcd(DISP_ON);

        // Clear LCD
        cmd_lcd(LCD_CLR);

        // Set cursor increment mode
        cmd_lcd(DISP_SHIFT_OFF_INC);
}


// Display one character
void char_lcd(u8 character)
{
        // Select data register
        SETBIT(IOSET0,RS);

        // Send character
        write_lcd(character);
}


// Display string
void str_lcd(u8 *p)
{
        // Display until null character
        while(*p)
                char_lcd(*p++);
}


// Display unsigned number
void u32_lcd(u32 n)
{
        u8 a[10];   // Stores digits

        s32 i=0;    // Index variable

        // Check number is zero
        if(n==0)
        {
                // Display zero
                char_lcd('0');
        }
        else
        {
                // Get all digits
                while(n)
                {
                        // Convert digit to ASCII
                        a[i]=(n%10)+48;

                        // Increase index
                        i++;

                        // Remove last digit
                        n/=10;
                }

                // Display digits in correct order
                for(--i;i>=0;i--)
                {
                        // Display digit
                        char_lcd(a[i]);
                }
        }
}


// Display float number
void float_lcd(f32 f,s32 np)
{
        s32 i;   // Stores integer value

        // Check negative value
        if(f<0)
        {
                // Display minus sign
                char_lcd('-');

                // Make value positive
                f*=-1;
        }

        // Get integer part
        i=f;

        // Display integer part
        u32_lcd(i);

        // Display decimal point
        char_lcd('.');

        // Get decimal part
        f = f-i;

        // Move decimal digits
        while(np)
        {
                // Multiply by 10
                f*=10;

                // Decrease count
                np--;
        }

        // Convert decimal part to integer
        i = f;

        // Display decimal part
        u32_lcd(i);
}


// Display signed number
void s32_lcd(s32 n)
{
        // Check negative value
        if(n<0)
        {
                // Display minus sign
                char_lcd('-');

                // Make value positive
                n*=-1;
        }

        // Display number
        u32_lcd(n);
}


// Display title
void title(void)
{
        u8 i;   // Loop variable

        // First line text
        char line1[] = " SECURE ACCESS ";

        // Second line text
        char line2[] = " CONTROL SYSTEM ";

        // Clear LCD
        cmd_lcd(0x01);

        // Display first line
        for(i=0;line1[i]!=0;i++)
        {
                // Set first line position
                cmd_lcd(GOTO_LINE1_POS_0+i);

                // Delay
                delay_ms(100);

                // Display character
                char_lcd(line1[i]);

                // Delay
                delay_ms(100);
        }

        // Display second line
        for(i=0;line2[i]!=0;i++)
        {
                // Set second line position
                cmd_lcd(GOTO_LINE2_POS_0+i);

                // Delay
                delay_ms(100);

                // Display character
                char_lcd(line2[i]);

                // Delay
                delay_ms(100);
        }
}
