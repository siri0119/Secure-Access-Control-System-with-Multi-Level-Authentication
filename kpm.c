/**
 * **************************************************************************
 * @file    kpm.c
 * @brief   Matrix Keypad Scanning, Number Parsers, and Password Masking Engine.
 * **************************************************************************
 */

#include "kpm.h"
#include "types.h"
#include "macros.h"
#include "delay.h"
#include "lcd.h"
#include <lpc21xx.h>

u8 menu_flag = 0;

#define KEYPAD

#ifdef KEYPAD
/* Active Hardware layout Matrix translation array map */
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

/**
 * @brief  Configures the GPIO directions for Keypad Matrix row scanning.
 */
void init_kpm(void)
{
        WNIBBLE(IODIR1,row0,15); /* Designate row pins as outputs, columns as inputs */
}

/**
 * @brief  Samples line states across matrix input column connections.
 * @return 0 if keypress registered, 1 if lines idle high.
 */
u32 colscan(void)
{
        return (RNIBBLE(IOPIN1,col0)<15)?0:1;
}

/**
 * @brief  Isolates and identifies the active pressed matrix row.
 */
u32 rowcheck(void)
{
        u32 rno;
        for(rno=0;rno<4;rno++)
        {
                WNIBBLE(IOPIN1,row0,~(1<<rno)); /* Drive individual test row down */
                if(colscan()==0)
                {
                        break; /* Active target row isolated */
                }
        }
        WNIBBLE(IOPIN1,row0,0X0); /* Restore default low condition line levels */
        return rno;
}

/**
 * @brief  Isolates and identifies the active pressed matrix column.
 */
u32 colcheck(void)
{
        u32 cno;
        for(cno=0;cno<4;cno++)
        {
                if(RBIT(IOPIN1,(col0+cno))==0)
                {
                        break; /* Active target column isolated */
                }
        }
        return cno;
}

/**
 * @brief  Monitors the keypad matrix and decodes active character entries.
 */
s32 keyscan(void)
{
        s32 keyv,rno,cno;
        while(colscan() && menu_flag==0); /* Wait loop until button down */
        if(menu_flag==1)
        {
                return -1; /* Exit routine instantly if interrupt breaks execution */
        }
        
        rno = rowcheck();
        cno = colcheck();
        keyv = kpmlut[rno][cno]; /* Extract key character mapping translation */
        while(!colscan());       /* Debounce loop waiting for button release */
        delay_ms(100);
        return keyv;
}

/**
 * @brief  Assembles individual numeric keypress inputs into integers.
 */
s32 readnum(void)
{
        s32 key;
        s32 cnt = 0;
        int num = 0;

        while(1)
        {
                        key = keyscan();
                        if(key == -1)
                        {
                                cnt = 0;
                                return -1;
                        }
                        if((key>='0') && (key<='9') && (cnt<4))
                        {
                                 cnt++;
                                 char_lcd(key); /* Render value string character on screen display */
                                 num = num*10 + (key-48); /* Left-shift integer base accumulator */
                        }
                        else if((key=='B') && (cnt!=0)) /* Delete functionality handling */
                        {
                                cnt--;
                                cmd_lcd(0x10);  /* Move screen cursor backward */
                                char_lcd(' ');  /* Clear terminal output character layout block */
                                cmd_lcd(0x10);  /* Rewind processing position marker alignment */
                                num /= 10;      /* Pop numerical base element */
                        }
                        else if((key == '=') && (cnt!=0)) /* Key submission indicator */
                        {
                                cnt = 0;
                                return num;
                        }
        }
}

/**
 * @brief  Accepts sensitive password characters and displays masking asterisks ('*').
 */
void password_kpm(char *ptr)
{
        u8 a;
        u8 cnt = 0;

        while(1)
        {
                a = keyscan();
                if((a>='0') && (a<='9') && (cnt!=4))
                {
                        cnt++;
                        char_lcd(a);     /* Briefly show plaintext input element */
                        delay_ms(300);
                        cmd_lcd(0x10);   /* Rewind line character cursor placement */
                        char_lcd('*');   /* Overwrite entry with masking asterisk symbol */
                        *ptr = a;        /* Store value char to destination array matrix element */
                        ptr++;
                }
                else if((a=='B') && (cnt!=0)) /* backspace erase handling */
                {
                        cmd_lcd(0x10); char_lcd(' '); cmd_lcd(0x10);
                        cnt--;
                        ptr--;
                }
                else if((a=='=') && (cnt==4))
                {
                        cnt = 0;
                        *ptr = '\0';     /* Inject terminal string termination token */
                        return;
                }
        }
}

/**
 * @brief  Captures custom array strings containing raw ID identifiers.
 */
void id_kpm(s8 *ptr)
{
        s8 a;
        s8 cnt = 0;

        while(1)
        {
                a = keyscan();
                if((a>='0') && (a<='9') && (cnt!=4))
                {
                        char_lcd(a);
                        ptr[cnt] = a;
                        cnt++;
                }
                else if((a=='B') && (cnt!=0))
                {
                        cnt--;
                        cmd_lcd(0x10); char_lcd(' '); cmd_lcd(0x10);
                }
                else if((a == '=') && (cnt==4))
                {
                        ptr[cnt] = '\0';
                        return;
                }
        }
}