// Start header guard
#ifndef _LCD_H_

// Define header guard
#define _LCD_H_


// Include string functions
#include <string.h>

// Include user defined data types
#include "types.h"


// LCD pin positions
#define DATA 8   // Data pin starting position

#define RS 16    // RS pin

#define RW 18    // RW pin

#define EN 17    // Enable pin


// Clear LCD
#define LCD_CLR 0X01

// Return cursor to home
#define RET_CUR_HOME 0X02


// Turn display OFF
#define DISP_OFF 0X08

// Turn display ON
#define DISP_ON 0X0C

// Turn display, cursor and blink ON
#define DISP_ON_CUR_ON_BLK_ON 0X0F

// Turn display and cursor ON
#define DISP_ON_CUR_ON 0X0E


// Move cursor forward without display shift
#define DISP_SHIFT_OFF_INC 0X06

// Move cursor forward with display shift
#define DISP_SHIFT_INC 0X07


// Set 8-bit single line mode
#define MODE_8BIT_LINE1 0X30

// Set 8-bit two line mode
#define MODE_8BIT_LINE2 0X38

// Set 4-bit single line mode
#define MODE_4BIT_LINE1 0X20

// Set 4-bit two line mode
#define MODE_4BIT_LINE2 0X28


// First line starting position
#define GOTO_LINE1_POS_0 0X80

// Second line starting position
#define GOTO_LINE2_POS_0 0XC0

// Third line starting position
#define GOTO_LINE3_POS_0 0X94

// Fourth line starting position
#define GOTO_LINE4_POS_0 0XD4


// Move cursor left
#define SHIFT_CUR_LEFT 0X10

// Move cursor right
#define SHIFT_CUR_RIGHT 0X14


// Select CGRAM
#define GOTO_CGRAM 0X40


// Write data to LCD
void write_lcd(u8);

// Send command to LCD
void cmd_lcd(u8);

// Initialize LCD
void init_lcd(void);

// Display one character
void char_lcd(u8);

// Display string
void str_lcd(u8*);

// Display unsigned number
void u32_lcd(u32);

// Display float number
void float_lcd(f32,s32);

// Display signed number
void s32_lcd(s32);

// Display title
void title(void);


// End header guard
#endif
