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

//#define NEW_ROM
#define OLD_ROM

// External menu flag
extern u8 menu_flag;

// Status flag
static u8 flag = 0;

// Admin ID and ID count
u8 admin_id, ids = 0;

// Password arrays
s8 pswd1[5], pswd2[5] = "2026";

// Stores ID
u32 id;


// Initialize EINT2
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

        // Set ID count to zero
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
        // Loop variable
        char i = 0;

        // Stores EEPROM ID
        char id2;

        // Check all stored IDs
        for(i = 0; i < ids; i++)
        {
                // Read ID from EEPROM
                id2 = i2c_eeprom_read_byte(
                        0x50,
                        (0x0001 + (i * 6))
                );

                // Compare IDs
                if(id2 == id)
                {
                        // Return ID address
                        return (0x0001 + (i * 6));
                }
        }

        // ID not found
        return 0;
}


// Check admin details
s8 check_admin(void)
{
        // Wrong attempt count
        u8 wrong = 0;


ID:

        // Clear LCD
        cmd_lcd(LCD_CLR);

        // Move cursor to first line
        cmd_lcd(GOTO_LINE1_POS_0);

        // Display admin ID message
        str_lcd("enter admin ID");

        // Move cursor to second line
        cmd_lcd(GOTO_LINE2_POS_0);

        // Read admin ID
        id = readnum();

        // Check admin ID
        if(id != 11)
        {
                // Check wrong attempts
                if(wrong == 3)
                {
                        // Clear LCD
                        cmd_lcd(LCD_CLR);

                        // Display access denied
                        str_lcd("ACCESS DENIED");

                        // Delay
                        delay_ms(1000);

                        // Return failure
                        return 1;
                }

                // Clear LCD
                cmd_lcd(LCD_CLR);

                // Move cursor to first line
                cmd_lcd(GOTO_LINE1_POS_0);

                // Display wrong ID
                str_lcd("wrong ID..!!");

                // Move cursor to second line
                cmd_lcd(GOTO_LINE2_POS_0);

                // Display remaining chances
                char_lcd(2 - wrong + 48);

                // Display message
                str_lcd(" more chance");

                // Delay
                delay_ms(500);

                // Check retry
                if(wrong != 3)
                {
                        // Increase wrong count
                        wrong++;

                        // Check ID again
                        goto ID;
                }
        }

        // Reset wrong count
        wrong = 0;


PSWD:

        // Clear LCD
        cmd_lcd(LCD_CLR);

        // Move cursor to first line
        cmd_lcd(GOTO_LINE1_POS_0);

        // Display password message
        str_lcd("enter PASSWORD");

        // Move cursor to second line
        cmd_lcd(GOTO_LINE2_POS_0);

        // Read password
        password_kpm(pswd1);

        // Compare password
        if(strcmp((const char*)pswd1, "2026") != 0)
        {
                // Clear LCD
                cmd_lcd(LCD_CLR);

                // Move cursor to first line
                cmd_lcd(GOTO_LINE1_POS_0);

                // Display wrong password
                str_lcd("wrong password..!!");

                // Move cursor to second line
                cmd_lcd(GOTO_LINE2_POS_0);

                // Display remaining chances
                char_lcd(2 - wrong + 48);

                // Display message
                str_lcd(" more chance");

                // Delay
                delay_ms(500);

                // Check retry
                if(wrong != 3)
                {
                        // Increase wrong count
                        wrong++;

                        // Check password again
                        goto PSWD;
                }

                // Return failure
                return 1;
        }

        // Clear LCD
        cmd_lcd(LCD_CLR);

        // Move cursor to first line
        cmd_lcd(GOTO_LINE1_POS_0);

        // Display access granted
        str_lcd("ACCESS GRANTED");

        // Delay
        delay_ms(500);

        // Return success
        return 0;
}


// Enroll new ID
void enroll_id(void)
{

ID:

        // Clear LCD
        cmd_lcd(0x01);

        // Display ID message
        str_lcd("ENTER ID(1-10):");

        // Move cursor to second line
        cmd_lcd(0xC0);

        // Read ID
        id = readnum();

        // Check ID range
        if(!((id >= 1) && (id <= 10)))
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display symbol
                char_lcd(0);

                // Display invalid entry
                str_lcd("INVALID ENTRY");

                // Display symbol
                char_lcd(0);

                // Move cursor to second line
                cmd_lcd(0xC0);

                // Display retry message
                str_lcd("try again...");

                // Delay
                delay_ms(500);

                // Read ID again
                goto ID;
        }

        // Check ID in EEPROM
        if(is_id_in_db(id))
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display ID exists message
                str_lcd("IT ALREADY EXIST");

                // Move cursor to second line
                cmd_lcd(0xC0);

                // Display another ID message
                str_lcd("TRY OTHER ID");

                // Delay
                delay_s(1);

                // Read ID again
                goto ID;
        }


PSWD:

        // Clear LCD
        cmd_lcd(0x01);

        // Display password message
        str_lcd("ENTER PASSWORD:");

        // Move cursor to second line
        cmd_lcd(0xC0);

        // Read password
        password_kpm(pswd1);

        // Clear LCD
        cmd_lcd(0x01);

        // Display password message
        str_lcd("ENTER AGAIN:");

        // Move cursor to second line
        cmd_lcd(0xC0);

        // Read password again
        password_kpm(pswd2);

        // Compare passwords
        if(strcmp((const char*)pswd1,
                  (const char*)pswd2) != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display not matched
                str_lcd("NOT MATCHED");

                // Move cursor to second line
                cmd_lcd(0xC0);

                // Display retry message
                str_lcd("try again");

                // Delay
                delay_ms(500);

                // Read password again
                goto PSWD;
        }
        else
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display finger message
                str_lcd(" go to finger");

                // Delay
                delay_ms(500);
        }


FP1:

        // Enroll fingerprint
        if(enroll(id) != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display failure message
                str_lcd("FAIL FINGER");

                // Move cursor to second line
                cmd_lcd(0xC0);

                // Display retry message
                str_lcd("PLEASE TRY AGAIN");

                // Delay
                delay_ms(500);

                // Enroll fingerprint again
                goto FP1;
        }

        // Store ID in EEPROM
        i2c_eeprom_write_byte(
                0x50,
                (0x0001 + (ids * 6)),
                id
        );

        // Store password in EEPROM
        i2c_eeprom_write_page(
                0x50,
                (0x0002 + (ids * 6)),
                (s8*)pswd1,
                5
        );

        // Increase ID count
        ids++;

        // Store ID count in EEPROM
        i2c_eeprom_write_byte(0x50,0x0000,ids);

        // Clear LCD
        cmd_lcd(0x01);

        // Display enroll done
        str_lcd("ENROLL DONE ");

        // Display symbol
        char_lcd(1);

        // Delay
        delay_s(1);
}


// Login function
void login(void)
{
        // Stores login ID
        int l_id;

        // Password buffers
        s8 buf1[5], buf2[5];

        // Countdown variable
        s8 l;

        // Retry count
        s8 retry = 0;

        // Stores ID address
        u16 found = 0;


        // Check ID count
        if(ids == 0)
        {
                // Check flag
                if(flag == 0)
                {
                        // Set flag
                        flag = 1;

                        // Clear LCD
                        cmd_lcd(0x01);

                        // Display no data message
                        str_lcd("NO DATA FOUND");

                        // Move cursor to second line
                        cmd_lcd(0xC0);

                        // Display enroll message
                        str_lcd("PRESS SW TO ENROLL");
                }

                // Exit function
                return;
        }

        // Reset flag
        flag = 0;

        // Clear LCD
        cmd_lcd(0x01);

        // Move cursor to first line
        cmd_lcd(0x80);

        // Display ID message
        str_lcd("ENTER THE ID:");

        // Read ID
        l_id = readnum();

        // Check exit value
        if(l_id == -1)
        {
                return;
        }

        // Search ID
        if((found = is_id_in_db(l_id)) == 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display ID not found
                str_lcd("ID IS NOT FOUND");

                // Delay
                delay_ms(1000);

                // Exit function
                return;
        }

        // Clear LCD
        cmd_lcd(0x01);

        // Display ID found
        str_lcd("ID IS FOUND");

        // Delay
        delay_ms(600);

        // Read password from EEPROM
        i2c_eeprom_seq_read(
                0x50,
                found + 1,
                (s8 *)buf1,
                5
        );


PSWD:

        // Clear LCD
        cmd_lcd(0x01);

        // Display password message
        str_lcd("ENTER PASSWORD:");

        // Move cursor to second line
        cmd_lcd(0xC0);

        // Read password
        password_kpm((s8 *)buf2);

        // Compare passwords
        if(strcmp((const char*)buf1,
                  (const char *)buf2) != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display wrong password
                str_lcd("WRONG PASSWORD ");

                // Display symbol
                char_lcd(0);

                // Increase retry count
                retry++;

                // Check retry count
                if(retry == 3)
                {
                        // Clear LCD
                        cmd_lcd(0x01);

                        // Display login failure
                        str_lcd("LOGIN FAILS");

                        // Delay
                        delay_ms(1000);

                        // Exit function
                        return;
                }

                // Move cursor to second line
                cmd_lcd(0xC0);

                // Display chances
                str_lcd("you have ");

                // Display remaining count
                char_lcd(3 - retry + 48);

                // Display message
                str_lcd(" chances");

                // Delay
                delay_ms(1000);

                // Check password again
                goto PSWD;
        }

        // Clear LCD
        cmd_lcd(0x01);

        // Display password verified
        str_lcd("PASSWORD VERIFED");

        // Delay
        delay_ms(500);

        // Reset retry count
        retry = 0;


FP:

        // Clear LCD
        cmd_lcd(0x01);

        // Verify fingerprint
        if(verify(l_id) != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display finger not found
                str_lcd("FINGER NOT FOUND");

                // Move cursor to second line
                cmd_lcd(0xC0);

                // Display retry message
                str_lcd("try again");

                // Delay
                delay_ms(600);

                // Increase retry count
                retry++;

                // Check first retry
                if(retry == 1)
                {
                        // Clear LCD
                        cmd_lcd(0x01);

                        // Display chances
                        str_lcd("two more chance");

                        // Delay
                        delay_ms(800);

                        // Verify again
                        goto FP;
                }

                // Check second retry
                else if(retry == 2)
                {
                        // Clear LCD
                        cmd_lcd(0x01);

                        // Display last chance
                        str_lcd("last chance");

                        // Delay
                        delay_ms(800);

                        // Verify again
                        goto FP;
                }

                else
                {
                        // Clear LCD
                        cmd_lcd(0x01);

                        // Display access denied
                        str_lcd("ACCESS DENIED");

                        // Delay
                        delay_ms(1000);

                        // Exit function
                        return;
                }
        }

        // Clear LCD
        cmd_lcd(0x01);

        // Display door opening
        str_lcd("DOOR OPENING ");

        // Open door
        door_open();

        // Delay
        delay_ms(5000);

        // Stop motor
        motor_stop();

        // Clear LCD
        cmd_lcd(0x01);

        // Display door opened
        str_lcd("DOOR OPENED ");

        // Delay
        delay_ms(500);

        // Countdown from 30
        for(l = 30; l > 0; l--)
        {
                // Move cursor to first line
                cmd_lcd(0x80);

                // Display close message
                str_lcd("DOOR CLOSE IN ");

                // Move cursor to second line
                cmd_lcd(0xC0);

                // Display spaces
                str_lcd("  ");

                // Display tens digit
                char_lcd((l / 10) + 48);

                // Display units digit
                char_lcd((l % 10) + 48);

                // Display seconds
                str_lcd(" sec ");

                // Delay
                delay_ms(500);
        }

        // Clear LCD
        cmd_lcd(0x01);

        // Display door closing
        str_lcd("DOOR IS CLOSEING");

        // Move cursor to second line
        cmd_lcd(0xC0);

        // Display spaces
        str_lcd("   ");

        // Close door
        door_close();

        // Delay
        delay_ms(5000);

        // Stop motor
        motor_stop();

        // Clear LCD
        cmd_lcd(0x01);

        // Display door closed
        str_lcd("DOOR IS CLOSED ");

        // Delay
        delay_ms(1000);
}


// Disable EINT2
void disable(void)
{
        // Disable EINT2 interrupt
        VICIntEnClr = 1 << EINT2_VIC_CHNO;
}


// Enable EINT2
void enable(void)
{
        // Enable EINT2 interrupt
        VICIntEnable |= 1 << EINT2_VIC_CHNO;
}


// Edit password
void password_edit(void)
{
        // Stores entered ID
        s8 p_id;

        // Password buffers
        s8 pw1[5], pw2[5];

        // Retry count
        s8 retry = 0;

        // Stores ID address
        u16 found = 0;

        // Clear LCD
        cmd_lcd(0x01);

        // Display ID message
        str_lcd("ENTER UR ID:");

        // Read ID
        p_id = readnum();

        // Search ID
        if((found = is_id_in_db(p_id)) == 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display ID not found
                str_lcd("ID IS NOT FOUND");

                // Delay
                delay_ms(1000);

                // Exit function
                return;
        }

        // Clear LCD
        cmd_lcd(0x01);

        // Display ID found
        str_lcd("ID IS FOUND");

        // Delay
        delay_ms(700);

        // Read password from EEPROM
        i2c_eeprom_seq_read(
                0x50,
                found + 1,
                pw1,
                5
        );


PSWD:

        // Clear LCD
        cmd_lcd(0x01);

        // Display old password message
        str_lcd("ENTER OLD PASSWORD:");

        // Move cursor to second line
        cmd_lcd(0xC0);

        // Read old password
        password_kpm(pw2);

        // Compare passwords
        if(strcmp((const char*)pw2,
                  (const char *)pw1) != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display wrong password
                str_lcd("WRONG PASSWORD");

                // Increase retry count
                retry++;

                // Check retry count
                if(retry == 3)
                {
                        // Clear LCD
                        cmd_lcd(0x01);

                        // Display symbol
                        char_lcd(0);

                        // Display not matched
                        str_lcd("   NOT MATCHED ");

                        // Display symbol
                        char_lcd(0);

                        // Move cursor to second line
                        cmd_lcd(0xC0);

                        // Display message
                        str_lcd(" CAN'T CHANGED ");

                        // Delay
                        delay_ms(1000);

                        // Exit function
                        return;
                }

                // Move cursor to second line
                cmd_lcd(0xC0);

                // Display chances
                str_lcd("you have ");

                // Display remaining count
                char_lcd(3 - retry + 48);

                // Display message
                str_lcd(" chances");

                // Delay
                delay_ms(1000);

                // Check password again
                goto PSWD;
        }

        // Clear LCD
        cmd_lcd(0x01);

        // Display password verified
        str_lcd("PASSWORD VERIFED");

        // Delay
        delay_ms(500);

        // Reset retry count
        retry = 0;


FP:

        // Clear LCD
        cmd_lcd(0x01);

        // Verify fingerprint
        if(verify(p_id) != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display finger not found
                str_lcd("FINGER NOT FOUND");

                // Move cursor to second line
                cmd_lcd(0xC0);

                // Display retry message
                str_lcd("try again");

                // Delay
                delay_ms(600);

                // Increase retry count
                retry++;

                // Check first retry
                if(retry == 1)
                {
                        // Clear LCD
                        cmd_lcd(0x01);

                        // Display chances
                        str_lcd("two more chance");

                        // Delay
                        delay_ms(500);

                        // Verify again
                        goto FP;
                }

                // Check second retry
                else if(retry == 2)
                {
                        // Clear LCD
                        cmd_lcd(0x01);

                        // Display last chance
                        str_lcd("last chance");

                        // Delay
                        delay_ms(500);

                        // Verify again
                        goto FP;
                }

                else
                {
                        // Clear LCD
                        cmd_lcd(0x01);

                        // Display symbol
                        char_lcd(0);

                        // Display not matched
                        str_lcd("   NOT MATCHED ");

                        // Display symbol
                        char_lcd(0);

                        // Move cursor to second line
                        cmd_lcd(0xC0);

                        // Display message
                        str_lcd("    CAN'T CHANGED ");

                        // Delay
                        delay_ms(1000);

                        // Exit function
                        return;
                }
        }

        else
        {

PSWD1:

                // Clear LCD
                cmd_lcd(0x01);

                // Display new password message
                str_lcd("ENTER NEW PASSWORD:");

                // Move cursor to second line
                cmd_lcd(0xC0);

                // Read new password
                password_kpm(pw1);

                // Clear LCD
                cmd_lcd(0x01);

                // Display password message
                str_lcd("ENTER AGAIN:");

                // Move cursor to second line
                cmd_lcd(0xC0);

                // Read password again
                password_kpm(pw2);

                // Compare passwords
                if(strcmp((const char*)pw1,
                          (const char*)pw2) != 0)
                {
                        // Clear LCD
                        cmd_lcd(0x01);

                        // Display not matched
                        str_lcd("NOT MATCHED");

                        // Move cursor to second line
                        cmd_lcd(0xC0);

                        // Display retry message
                        str_lcd("try again");

                        // Delay
                        delay_ms(500);

                        // Read password again
                        goto PSWD1;
                }

                else
                {
                        // Store new password
                        i2c_eeprom_write_page(
                                0x50,
                                found + 1,
                                pw1,
                                5
                        );

                        // Clear LCD
                        cmd_lcd(0x01);

                        // Display new password message
                        str_lcd(" NEW PASSWORD ");

                        // Move cursor to second line
                        cmd_lcd(0xC0);

                        // Display updated
                        str_lcd("UPDATED ");

                        // Display space
                        char_lcd(' ');

                        // Display symbol
                        char_lcd(1);

                        // Delay
                        delay_ms(500);
                }
        }
}


// Edit fingerprint
void finger_edit(void)
{
        // Stores entered ID
        s8 f_id;

        // Password buffers
        s8 pw1[5], pw2[5];

        // Retry count
        s8 retry = 0;

        // Stores ID address
        u16 found;

        // Clear LCD
        cmd_lcd(0x01);

        // Display ID message
        str_lcd("ENTER THE ID:");

        // Read ID
        f_id = readnum();

        // Search ID
        if((found = is_id_in_db(f_id)) == 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display ID not found
                str_lcd("ID IS NOT FOUND");

                // Delay
                delay_ms(1000);

                // Exit function
                return;
        }

        // Clear LCD
        cmd_lcd(0x01);

        // Display ID found
        str_lcd("ID IS FOUND");

        // Delay
        delay_ms(900);

        // Read password from EEPROM
        i2c_eeprom_seq_read(
                0x50,
                found + 1,
                pw2,
                5
        );


PSWD:

        // Clear LCD
        cmd_lcd(0x01);

        // Display password message
        str_lcd("ENTER PASSWORD:");

        // Move cursor to second line
        cmd_lcd(0xC0);

        // Read password
        password_kpm(pw1);

        // Compare passwords
        if(strcmp((const char*)pw2,
                  (const char *)pw1) != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display wrong password
                str_lcd("WRONG PASSWORD");

                // Delay
                delay_ms(1000);

                // Increase retry count
                retry++;

                // Check retry count
                if(retry == 3)
                {
                        // Clear LCD
                        cmd_lcd(0x01);

                        // Display symbol
                        char_lcd(0);

                        // Display not matched
                        str_lcd("   NOT MATCHED ");

                        // Display symbol
                        char_lcd(0);

                        // Move cursor to second line
                        cmd_lcd(0xC0);

                        // Display message
                        str_lcd("CAN'T CHANGED ");

                        // Delay
                        delay_ms(1000);

                        // Exit function
                        return;
                }

                // Move cursor to second line
                cmd_lcd(0xC0);

                // Display chances
                str_lcd("you have ");

                // Display remaining count
                char_lcd(3 - retry + 48);

                // Display message
                str_lcd(" chances");

                // Delay
                delay_ms(1000);

                // Check password again
                goto PSWD;
        }


FP1:

        // Delete fingerprint
        delete(f_id);

        // Enroll fingerprint
        if(enroll(f_id) != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display failure message
                str_lcd("FAIL FINGER");

                // Move cursor to second line
                cmd_lcd(0xC0);

                // Display retry message
                str_lcd("please try again");

                // Delay
                delay_ms(500);

                // Enroll again
                goto FP1;
        }

        // Clear LCD
        cmd_lcd(0x01);

        // Display success message
        str_lcd(" SUCCESSFULLY ");

        // Move cursor to second line
        cmd_lcd(0xC0);

        // Display updated
        str_lcd("  UPDATED ");

        // Display symbol
        char_lcd(1);
}


// Forgot password
void forget_password(void)
{
        // Stores entered ID
        s8 fo_id;

        // Password buffers
        s8 pw1[5], pw2[5];

        // Retry count
        s8 retry = 0;

        // Stores ID address
        u16 found;

        // Clear LCD
        cmd_lcd(0x01);

        // Display ID message
        str_lcd("ENTER THE ID:");

        // Read ID
        fo_id = readnum();

        // Search ID
        if((found = is_id_in_db(fo_id)) == 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display ID not found
                str_lcd("ID IS NOT FOUND");

                // Delay
                delay_ms(1000);

                // Exit function
                return;
        }

        // Clear LCD
        cmd_lcd(0x01);

        // Display ID found
        str_lcd("ID IS FOUND");

        // Delay
        delay_ms(1000);


PSWD:

        // Clear LCD
        cmd_lcd(0x01);

        // Display default password message
        str_lcd("ENTER THE DEFAULT ");

        // Move cursor to second line
        cmd_lcd(0xC0);

        // Display password message
        str_lcd("PASSWORD:");

        // Read password
        password_kpm(pw1);

        // Compare default password
        if(strcmp((const char*)"0119",
                  (const char *)pw1) != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display wrong password
                str_lcd("WRONG PASSWORD");

                // Delay
                delay_ms(1000);

                // Increase retry count
                retry++;

                // Check retry count
                if(retry == 2)
                {
                        // Clear LCD
                        cmd_lcd(0x01);

                        // Display symbol
                        char_lcd(0);

                        // Display not matched
                        str_lcd("   NOT MATCHED ");

                        // Display symbol
                        char_lcd(0);

                        // Move cursor to second line
                        cmd_lcd(0xC0);

                        // Display message
                        str_lcd(" CAN'T CHANGED ");

                        // Delay
                        delay_ms(1000);

                        // Exit function
                        return;
                }

                // Move cursor to second line
                cmd_lcd(0xC0);

                // Display retry message
                str_lcd("try one more");

                // Delay
                delay_ms(1000);

                // Check password again
                goto PSWD;
        }


PSWD1:

        // Clear LCD
        cmd_lcd(0x01);

        // Display new password message
        str_lcd("ENTER NEW PASSWORD:");

        // Move cursor to second line
        cmd_lcd(0xC0);

        // Read new password
        password_kpm(pw1);

        // Clear LCD
        cmd_lcd(0x01);

        // Display password message
        str_lcd("ENTER AGAIN:");

        // Move cursor to second line
        cmd_lcd(0xC0);

        // Read password again
        password_kpm(pw2);

        // Compare passwords
        if(strcmp((const char*)pw1,
                  (const char*)pw2) != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display not matched
                str_lcd("NOT MATCHED");

                // Move cursor to second line
                cmd_lcd(0xC0);

                // Display retry message
                str_lcd("try again");

                // Delay
                delay_ms(500);

                // Read password again
                goto PSWD1;
        }

        else
        {
                // Store new password
                i2c_eeprom_write_page(
                        0x50,
                        found + 1,
                        pw1,
                        5
                );

                // Clear LCD
                cmd_lcd(0x01);

                // Display new password message
                str_lcd(" NEW PASSWORD ");

                // Move cursor to second line
                cmd_lcd(0xC0);

                // Display updated
                str_lcd("UPDATED ");

                // Display space
                char_lcd(' ');

                // Delay
                delay_ms(500);
        }
}


// Delete user ID
void delete_id(void)
{
        // Stores entered ID
        s8 id;

        // Stores total IDs
        s8 total;

        // Loop variable
        s8 i;

        // User data buffer
        s8 buf[6];

        // EEPROM addresses
        u16 found_addr;
        u16 next_addr;
        u16 last_addr;


ID:

        // Clear LCD
        cmd_lcd(0x01);

        // Move cursor to first line
        cmd_lcd(0x80);

        // Display ID message
        str_lcd("ENTER ID:");

        // Read ID
        id = readnum();

        // Search ID
        found_addr = is_id_in_db(id);

        // Check ID address
        if(found_addr == 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display ID not found
                str_lcd("ID NOT FOUND");

                // Delay
                delay_ms(1000);

                // Read ID again
                goto ID;
        }

        // Delete fingerprint
        if(delete(id) != 0)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display delete failure
                str_lcd("DELETE FAIL");

                // Delay
                delay_ms(1000);

                // Exit function
                return;
        }

        // Read total IDs
        total = i2c_eeprom_read_byte(0x50,0x0000);

        // Calculate last record address
        last_addr = 0x01 + ((total-1) * 6);

        // Shift remaining records
        while(found_addr < last_addr)
        {
                // Calculate next record address
                next_addr = found_addr + 6;

                // Read next record
                i2c_eeprom_seq_read(
                        0x50,
                        next_addr,
                        buf,
                        6
                );

                // Write record to current address
                i2c_eeprom_write_page(
                        0x50,
                        found_addr,
                        buf,
                        6
                );

                // Move to next address
                found_addr += 6;
        }

        // Clear buffer
        for(i=0;i<6;i++)
        {
                // Store empty value
                buf[i] = 0xFF;
        }

        // Clear last record
        i2c_eeprom_write_page(
                0x50,
                last_addr,
                buf,
                6
        );

        // Check total count
        if(total > 0)
        {
                // Decrease total count
                total--;

                // Store total count
                i2c_eeprom_write_byte(
                        0x50,
                        0x0000,
                        total
                );
        }

        // Clear LCD
        cmd_lcd(0x01);

        // Move cursor to first line
        cmd_lcd(0x80);

        // Display deleted message
        str_lcd("USER DELETED");

        // Move cursor to second line
        cmd_lcd(0xC0);

        // Display success
        str_lcd("SUCCESS");

        // Delay
        delay_ms(1500);
}


// Edit menu
void edit(void)
{
        // Stores menu choice
        u8 choice;

        // Check ID count
        if(ids == 0)
        {
                // Check flag
                if(flag == 0)
                {
                        // Set flag
                        flag = 1;

                        // Clear LCD
                        cmd_lcd(0x01);

                        // Display no data
                        str_lcd("NO DATA FOUND");

                        // Move cursor to second line
                        cmd_lcd(0xC0);

                        // Display enroll message
                        str_lcd("PRESS SW TO ENROLL");
                }

                // Exit function
                return;
        }

        // Reset flag
        flag = 0;

        while(1)
        {
                // Clear LCD
                cmd_lcd(0x01);

                // Display first menu line
                str_lcd("1.PSSWRD 2.FINGER");

                // Move cursor to second line
                cmd_lcd(0xC0);

                // Display second menu line
                str_lcd("3.FORGOT 4.EXIT");

                // Read menu choice
                choice = keyscan();

                // Check menu choice
                switch(choice)
                {
                        case '1':

                                // Edit password
                                password_edit();

                                break;


                        case '2':

                                // Edit fingerprint
                                finger_edit();

                                break;


                        case '3':

                                // Reset password
                                forget_password();

                                break;


                        case '4':

                                // Exit switch
                                break;
                }

                // Check exit choice
                if(choice == '4')
                {
                        // Exit loop
                        break;
                }
        }
}


// Main menu
void menu(void)
{
        // Stores menu choice
        u8 choice;

        // Clear menu flag
        menu_flag = 0;

        // Disable EINT2
        disable();

        // Reset flag
        flag = 0;

        while(1)
        {
                // Clear LCD
                cmd_lcd(LCD_CLR);

                // Move cursor to first line
                cmd_lcd(GOTO_LINE1_POS_0);

                // Display first menu line
                str_lcd("1.ENROLL 2.EDIT");

                // Move cursor to second line
                cmd_lcd(GOTO_LINE2_POS_0);

                // Display second menu line
                str_lcd("3.DELETE 4.EXIT");

                // Read menu choice
                choice = keyscan();

                // Check menu choice
                switch(choice)
                {
                        case '1':

                                // Enroll new ID
                                enroll_id();

                                break;


                        case '2':

                                // Open edit menu
                                edit();

                                break;


                        case '3':

                                // Delete ID
                                delete_id();

                                break;


                        case '4':

                                // Exit switch
                                break;
                }

                // Check exit choice
                if(choice == '4')
                {
                        // Clear LCD
                        cmd_lcd(0x01);

                        // Clear menu flag
                        menu_flag = 0;

                        // Enable EINT2
                        enable();

                        // Display ID count
                        char_lcd(ids + 48);

                        // Exit loop
                        break;
                }
        }
}
