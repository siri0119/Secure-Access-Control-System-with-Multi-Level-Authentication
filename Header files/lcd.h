#ifndef _LCD_H_                      
#define _LCD_H_                      

#include <string.h>                  
#include "types.h"                  

/* ==========================================================================
 * PIN CONFIGURATION: Peripheral Routing Targets (LPC214x Ports)
 * ========================================================================== */
#define DATA 8                       // Dynamic starting offset for 8-bit bus mapping
#define RS 16                        // Register Select Control Pin Line
#define RW 18                        // Read / Write System Toggle Line
#define EN 17                        // Controller Strobe Pulse (Latch Enable)

/* ==========================================================================
 * HD44780 CONTROLLER DRIVER COMMAND SET DEFINITIONS
 * ========================================================================== */
#define LCD_CLR 0X01                 // Wipe display buffer ram entirely
#define RET_CUR_HOME 0X02            // Rewind cursor coordinates to (0,0)

#define DISP_OFF 0X08                // Disable pixel display mask matrix
#define DISP_ON 0X0C                 // Wake up active visual rendering
#define DISP_ON_CUR_ON_BLK_ON 0X0F   // Visual array on, visible block cursor blinking
#define DISP_ON_CUR_ON 0X0E          // Visual array on, underlining cursor active

#define DISP_SHIFT_OFF_INC 0X06      // Text flow: Left-to-Right auto-increment mode
#define DISP_SHIFT_INC 0X07          // Accompany text flow with full layout shifting

#define MODE_8BIT_LINE1 0X30         // Initialize 8-bit single row rendering layout
#define MODE_8BIT_LINE2 0X38         // Initialize 8-bit dual row rendering layout
#define MODE_4BIT_LINE1 0X20         // Initialize 4-bit single row rendering layout
#define MODE_4BIT_LINE2 0X28         // Initialize 4-bit dual row rendering layout

#define GOTO_LINE1_POS_0 0X80        // Row 1 Memory Home DDRAM Base Address
#define GOTO_LINE2_POS_0 0XC0        // Row 2 Memory Home DDRAM Base Address
#define GOTO_LINE3_POS_0 0X94        // Row 3 Memory Home DDRAM Base Address
#define GOTO_LINE4_POS_0 0XD4        // Row 4 Memory Home DDRAM Base Address

#define SHIFT_CUR_LEFT  0X10         // Move the cursor backward 1 index
#define SHIFT_CUR_RIGHT 0X14         // Advance the cursor forward 1 index

#define GOTO_CGRAM 0X40              // Shift interface pointer to custom glyph bank

/* ==========================================================================
 * CORE LCD FUNCTION INTERFACES
 * ========================================================================== */
void write_lcd(u8);                  
void cmd_lcd(u8);                    
void init_lcd(void);                 
void char_lcd(u8);                   
void str_lcd(u8*);                   
void u32_lcd(u32);                   

#endif
