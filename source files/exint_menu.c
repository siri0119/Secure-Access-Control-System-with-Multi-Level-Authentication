#include <lpc21xx.h>
#include "lcd.h"
#include "types.h"
#include "exint_menu.h"
#include "kpm.h"
#include "lcd.h"
#include "string.h"
#include "delay.h"
#include "i2c_eeprom.h"
#include "r305.h"
#include "l293d.h"

#define OLD_ROM   // Use old EEPROM data

extern u8 menu_flag;   // Menu flag

static u8 flag = 0;    // Status flag

u8 admin_id, ids = 0;  // Admin ID and ID count

s8 pswd1[5], pswd2[5] = "2026";   // Password arrays

u32 id;   // Stores user ID


// Initialize external interrupt 2
void init_eint2(void)
{
        // Clear P0.7 function bits
        PINSEL0 &= ~((u32)3 << 14);

        // Select EINT2 function
        PINSEL0 |= EINT2_INPUT_PIN;

        // Enable EINT2 interrupt
        VICIntEnable |= (1 << EINT2_VIC_CHNO);

        // Select VIC slot 1
        VICVectCntl1 = (1 << 5) | EINT2_VIC_CHNO;

        // Store ISR address
        VICVectAddr1 = (u32)eint2_isr;

        // Set EINT2 as edge triggered
        EXTMODE = (1 << 2);
}


// Initialize ID count
void init_ids(void)
{
#ifdef NEW_ROM

        // Set ID count to 0
        ids = 0;

        // Store ID count in EEPROM
        i2c_eeprom_write_byte(0x50, 0x0000, ids);

#endif

#ifdef OLD_ROM

        // Read ID count from EEPROM
        ids = i2c_eeprom_read_byte(0x50, 0x0000);

#endif
}


// EINT2 interrupt function
void eint2_isr(void) __irq
{
        // Set menu flag
        menu_flag = 1;

        // Clear EINT2 interrupt
        EXTINT = (1 << 2);

        // Clear VIC interrupt
        VICVectAddr = 0;
}


// Search ID in EEPROM
u16 is_id_in_db(char id)
{
        char i = 0;   // Loop variable

        char id2;     // Stores EEPROM ID

        // Check all stored IDs
        for(i = 0; i < ids; i++)
        {
                // Read ID from EEPROM
                id2 = i2c_eeprom_read_byte(
                        0x50,
                        (0x0001 + (i * 6))
                );

                // Check ID
                if(id2 == id)
                {
                        // Return ID address
                        return (0x0001 + (i * 6));
                }
        }

        // ID not found
        return 0;
}
