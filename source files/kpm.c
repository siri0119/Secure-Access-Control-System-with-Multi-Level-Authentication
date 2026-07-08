#include "kpm.h"
#include "types.h"
#include "macros.h"
#include "delay.h"
#include "lcd.h"
#include <lpc21xx.h>

// Menu flag
u8 menu_flag = 0;

// Enable keypad
#define KEYPAD


// Keypad values
#ifdef KEYPAD

s32 kpmlut[4][4] =
{
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','-'},
        {'*','0','=','B'}
};

#else

s32 kpmlut[4][4] =
{
        {'1','2','3','4'},
        {'5','6','7','8'},
        {'9','0','+','-'},
        {'*','/','=','B'}
};

#endif


// Initialize keypad
void init_kpm(void)
{
        // Set row pins as output
        WNIBBLE(IODIR1,row0,15);
}


// Check keypad columns
u32 colscan(void)
{
        // Return keypad column status
        return (RNIBBLE(IOPIN1,col0)<15)?0:1;
}


// Find pressed row
u32 rowcheck(void)
{
        u32 rno;   // Row number

        // Check all rows
        for(rno=0;rno<4;rno++)
        {
                // Activate one row
                WNIBBLE(IOPIN1,row0,~(1<<rno));

                // Check key press
                if(colscan()==0)
                {
                        break;
                }
        }

        // Clear rows
        WNIBBLE(IOPIN1,row0,0X0);

        // Return row number
        return rno;
}


// Find pressed column
u32 colcheck(void)
{
        u32 cno;   // Column number

        // Check all columns
        for(cno=0;cno<4;cno++)
        {
                // Check column
                if(RBIT(IOPIN1,(col0+cno))==0)
                {
                        break;
                }
        }

        // Return column number
        return cno;
}


// Read keypad key
s32 keyscan(void)
{
        s32 keyv,rno,cno;   // Key, row and column values

        // Wait for key press
        while(colscan() && menu_flag==0);

        // Check menu flag
        if(menu_flag==1)
        {
                return -1;
        }

        // Find row
        rno = rowcheck();

        // Find column
        cno = colcheck();

        // Get key value
        keyv = kpmlut[rno][cno];

        // Wait for key release
        while(!colscan());

        // Delay
        delay_ms(100);

        // Return key
        return keyv;
}


// Read number from keypad
s32 readnum(void)
{
        s32 key;      // Key value
        s32 cnt = 0;  // Digit count
        int num = 0;  // Stores number

        while(1)
        {
                // Read key
                key = keyscan();

                // Check exit condition
                if(key == -1)
                {
                        cnt = 0;

                        return -1;
                }

                // Check numeric key
                if((key>='0') && (key<='9') && (cnt<4))
                {
                        // Increase digit count
                        cnt++;

                        // Display digit
                        char_lcd(key);

                        // Add digit to number
                        num = num*10 + (key-48);
                }

                // Check backspace key
                else if((key=='B') && (cnt!=0))
                {
                        // Decrease digit count
                        cnt--;

                        // Move cursor left
                        cmd_lcd(0x10);

                        // Clear digit
                        char_lcd(' ');

                        // Move cursor left
                        cmd_lcd(0x10);

                        // Remove last digit
                        num /= 10;
                }

                // Check enter key
                else if((key == '=') && (cnt!=0))
                {
                        // Reset digit count
                        cnt = 0;

                        // Return number
                        return num;
                }
        }
}


// Read password from keypad
void password_kpm(char *ptr)
{
        u8 a;        // Key value
        u8 cnt = 0;  // Digit count

        while(1)
        {
                // Read key
                a = keyscan();

                // Check numeric key
                if((a>='0') && (a<='9') && (cnt!=4))
                {
                        // Increase digit count
                        cnt++;

                        // Display digit
                        char_lcd(a);

                        // Delay
                        delay_ms(300);

                        // Move cursor left
                        cmd_lcd(0x10);

                        // Display star
                        char_lcd('*');

                        // Store password digit
                        *ptr = a;

                        // Move pointer forward
                        ptr++;
                }

                // Check backspace key
                else if((a=='B') && (cnt!=0))
                {
                        // Move cursor left
                        cmd_lcd(0x10);

                        // Clear digit
                        char_lcd(' ');

                        // Move cursor left
                        cmd_lcd(0x10);

                        // Decrease digit count
                        cnt--;

                        // Move pointer backward
                        ptr--;
                }

                // Check enter key
                else if((a=='=') && (cnt==4))
                {
                        // Reset digit count
                        cnt = 0;

                        // Add null character
                        *ptr = '\0';

                        // Exit function
                        return;
                }
        }
}


// Read ID from keypad
void id_kpm(s8 *ptr)
{
        s8 a;        // Key value
        s8 cnt = 0;  // Digit count

        while(1)
        {
                // Read key
                a = keyscan();

                // Check numeric key
                if((a>='0') && (a<='9') && (cnt!=4))
                {
                        // Display digit
                        char_lcd(a);

                        // Store digit
                        ptr[cnt] = a;

                        // Increase digit count
                        cnt++;
                }

                // Check backspace key
                else if((a=='B') && (cnt!=0))
                {
                        // Decrease digit count
                        cnt--;

                        // Move cursor left
                        cmd_lcd(0x10);

                        // Clear digit
                        char_lcd(' ');

                        // Move cursor left
                        cmd_lcd(0x10);
                }

                // Check enter key
                else if((a == '=') && (cnt==4))
                {
                        // Add null character
                        ptr[cnt] = '\0';

                        // Exit function
                        return;
                }
        }
}
