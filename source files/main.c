#include "lcd.h"
#include "kpm.h"
#include "i2c.h"
#include "i2c_eeprom.h"
#include "uart.h"
#include "r305.h"
#include "delay.h"
#include "exint_menu.h"
#include "i2c_eeprom.h"
#include "types.h"
#include "l293d.h"

// External menu flag
extern u8 menu_flag;


int main()
{
        // Initialize LCD
        init_lcd();

        // Initialize keypad
        init_kpm();

        // Initialize UART0
        init_uart0();

        // Initialize I2C
        init_i2c();

        // Initialize external interrupt
        init_eint2();

        // Initialize stored IDs
        init_ids();

        // Initialize motor
        init_l293d();

        // Display title
        title();

        // Delay
        delay_ms(1000);


        while(1)
        {
                // Check menu flag
                if(menu_flag == 1)
                {
                        // Open menu
                        menu();
                }

                // Perform login
                login();
        }
}
