#include "lcd.h"                     
#include "kpm.h"                     
#include "i2c.h"                     
#include "i2c_eeprom.h"              
#include "uart.h"                    
#include "r305.h"                    
#include "delay.h"                   
#include "exint_menu.h"              
#include "types.h"                   
#include "l293d.h"                   

/* ==========================================================================
 * SYSTEM VOLATILE STATE CAPTURES
 * ========================================================================== */
extern u8 menu_flag;                 // Raised via external hardware interrupt vectors

/**
 * @brief Main execution firmware lifecycle entrypoint
 */
int main()
{
        /* --- HARDWARE PERIPHERAL INITIALIZATION MATRIX --- */
        init_lcd();                  // Map and wake up system visual metrics
        init_kpm();                  // Configure keypad input processing array
        init_uart0();                // Establish serial linkage to fingerprint module
        init_i2c();                  // Wake communication architecture bus for EEPROM
        init_eint2();                // Attach asynchronous user menu hardware interrupt triggers
        init_ids();                  // Pull operational configuration parameters from storage
        init_l293d();                // Set configuration for physical locking gear assembly
        
        /* --- RUNTIME BANNER ENTRY --- */
        title();                     // Render rolling splash interface banner
        delay_ms(1000);  	         // Hold title screen frame for 1 second

        /* --- PRIMARY SUPERVISOR KERNEL CONTINUOUS LOOP --- */
        while(1)
        {
                if(menu_flag==1)      // Priority assessment loop checkpoint
                {
                        menu();       // Branch execution path to terminal setup mode
                }
                login();              // Evaluate background authorization tasks
        }		  
}