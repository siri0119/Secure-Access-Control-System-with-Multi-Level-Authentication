/**
 * **************************************************************************
 * @file    exint_menu.c
 * @brief   Core Security System Logic: Admin Validation, User Enrollment, 
 *          Authentication, Keypad Interactions, and Access Control Management.
 * **************************************************************************
 */

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

#define OLD_ROM        
extern u8 menu_flag;    /* Monitors external interrupt state for routine escapes */

static u8 flag = 0;     /* UI Toggle flag to prevent screen flickering */

u8 admin_id, ids = 0;  /* System metrics tracking stored IDs */

s8 pswd1[5], pswd2[5] = "2026"; /* Password working buffers */

u32 id;                /* Runtime ID assignment container */

/**
 * @brief  Configures External Interrupt 2 (EINT2) for Menu activation.
 */
void init_eint2(void)
{
        PINSEL0 &= ~((u32)3 << 14);            /* Clear Pin configuration for P0.7 */
        PINSEL0 |= EINT2_INPUT_PIN;            /* Assign P0.7 functionality to EINT2 */
        VICIntEnable |= (1 << EINT2_VIC_CHNO); /* Register EINT2 in the Interrupt Controller */

        /* Prioritize Handler Mapping */
        VICVectCntl1 = (1 << 5) | EINT2_VIC_CHNO;
        VICVectAddr1 = (u32)eint2_isr;

        EXTMODE = (1 << 2);                    /* Triggering configuration: Edge Sensitive */
}

/**
 * @brief  Resets and synchronizes data parameters inside the I2C EEPROM.
 */
void init_ids(void)
{
        ids = 0;
        i2c_eeprom_write_byte(0x50, 0x0000, ids);    /* Wipe record count */
        ids = i2c_eeprom_read_byte(0x50, 0x0000);     /* Verify memory write loop */
}

/**
 * @brief  Interrupt Service Routine for EINT2. Drops system into configuration menu.
 */
void eint2_isr(void) __irq
{
        menu_flag = 1;       /* Signal application layer to divert into menu routine */
        EXTINT = (1 << 2);   /* Reset Interrupt Flag */
        VICVectAddr = 0;     /* End-Of-Interrupt clear token for VIC */
}

/**
 * @brief  Checks if a Target ID resides inside the database lookup tables.
 * @return 16-bit EEPROM Memory address offsets if true, 0 if false.
 */
u16 is_id_in_db(char id)
{
        char i = 0;
        char id2;

        for(i = 0; i < ids; i++)
        {
                /* Calculate data structure padding boundary (6 bytes per user entry) */
                id2 = i2c_eeprom_read_byte(0x50,(0x0001 + (i * 6)));

                if(id2 == id)
                {
                        return (0x0001 + (i * 6)); /* Return target reference pointer */
                }
        }
        return 0;
}

/**
 * @brief  Enforces validation protocol before granting access to Admin panel.
 */
s8 check_admin(void)
{
        u8 wrong = 0;

ID:
        cmd_lcd(LCD_CLR);
        cmd_lcd(GOTO_LINE1_POS_0);
        str_lcd("enter admin ID");
        cmd_lcd(GOTO_LINE2_POS_0);
        id = readnum();

        if(id != 11) /* Root Level ID */
        {
                if(wrong == 3)
                {
                        cmd_lcd(LCD_CLR);
                        str_lcd("ACCESS DENIED");
                        delay_ms(1000);
                        return 1;
                }

                cmd_lcd(LCD_CLR);
                cmd_lcd(GOTO_LINE1_POS_0);
                str_lcd("wrong ID..!!");
                cmd_lcd(GOTO_LINE2_POS_0);
                char_lcd(2 - wrong + 48); /* Inform user of remaining attempts */
                str_lcd(" more chance");
                delay_ms(500);

                if(wrong != 3)
                {
                        wrong++;
                        goto ID;
                }
        }

        wrong = 0;

PSWD:
        cmd_lcd(LCD_CLR);
        cmd_lcd(GOTO_LINE1_POS_0);
        str_lcd("enter PASSWORD");
        cmd_lcd(GOTO_LINE2_POS_0);
        password_kpm(pswd1);

        if(strcmp((const char*)pswd1, "2026") != 0)
        {
                cmd_lcd(LCD_CLR);
                cmd_lcd(GOTO_LINE1_POS_0);
                str_lcd("wrong password..!!");
                cmd_lcd(GOTO_LINE2_POS_0);
                char_lcd(2 - wrong + 48);
                str_lcd(" more chance");

                if(wrong != 3)
                {
                        wrong++;
                        goto PSWD;
                }
                return 1;
        }

        cmd_lcd(LCD_CLR);
        cmd_lcd(GOTO_LINE1_POS_0);
        str_lcd("ACCESS GRANTED");
        delay_ms(500);
        return 0;
}

/**
 * @brief  Registers a new system user profile (ID + Secret Key + Biometrics).
 */
void enroll_id(void)
{
ID:
        cmd_lcd(0x01);
        str_lcd("ENTER ID(1-10):");
        cmd_lcd(0xC0);
        id = readnum();

        if(!((id >= 1) && (id <= 10)))
        {
                cmd_lcd(0x01);
                char_lcd(0);
                str_lcd("INVALID ENTRY");
                char_lcd(0);
                cmd_lcd(0xC0);
                str_lcd("try again...");
                delay_ms(500);
                goto ID;
        }

        if(is_id_in_db(id))
        {
                cmd_lcd(0x01);
                str_lcd("IT ALREADY EXIST");
                cmd_lcd(0xC0);
                str_lcd("TRY OTHER ID");
                delay_s(1);
                goto ID;
        }

PSWD:
        cmd_lcd(0x01);
        str_lcd("ENTER PASSWORD:");
        cmd_lcd(0xC0);
        password_kpm(pswd1);

        cmd_lcd(0x01);
        str_lcd("ENTER AGAIN:");
        cmd_lcd(0xC0);
        password_kpm(pswd2);

        if(strcmp((const char*)pswd1, (const char*)pswd2) != 0)
        {
                cmd_lcd(0x01);
                str_lcd("NOT MATCHED");
                cmd_lcd(0xC0);
                str_lcd("try again");
                delay_ms(500);
                goto PSWD;
        }
        else
        {
                cmd_lcd(0x01);
                str_lcd(" go to finger");
                delay_ms(500);
        }

FP1:
        if(enroll(id) != 0) /* Interface with optical sensor API */
        {
                cmd_lcd(0x01);
                str_lcd("FAIL FINGER");
                cmd_lcd(0xC0);
                str_lcd("PLEASE TRY AGAIN");
                delay_ms(500);
                goto FP1;
        }

        /* Flash parameters into Non-Volatile EEPROM Memory storage matrix */
        i2c_eeprom_write_byte(0x50,(0x0001 + (ids * 6)),id);
        i2c_eeprom_write_page(0x50,(0x0002 + (ids * 6)),(s8*)pswd1,5);

        ids++; /* Advance registration tracking metadata */
        i2c_eeprom_write_byte(0x50,0x0000,ids);

        cmd_lcd(0x01);
        str_lcd("ENROLL DONE ");
        char_lcd(1);
        delay_s(1);
}

/**
 * @brief  Main login pipeline execution module. Implements double-tier authentication.
 */
void login(void)
{
        int l_id;
        s8 buf1[5], buf2[5];
        s8 l;
        s8 retry = 0;
        u16 found = 0;

        if(ids == 0)
        {
                if(flag == 0)
                {
                        flag = 1;
                        cmd_lcd(0x01);
                        str_lcd("NO DATA FOUND");
                        cmd_lcd(0xC0);
                        str_lcd("PRESS SW TO ENROLL");
                }
                return;
        }

        flag = 0;
        cmd_lcd(0x01);
        cmd_lcd(0x80);
        str_lcd("ENTER THE ID:");
        l_id = readnum();

        if(l_id == -1) { return; }

        if((found = is_id_in_db(l_id)) == 0)
        {
                cmd_lcd(0x01);
                str_lcd("ID IS NOT FOUND");
                delay_ms(1000);
                return;
        }

        cmd_lcd(0x01);
        str_lcd("ID IS FOUND");
        delay_ms(600);

        /* Extract corresponding password hash footprint */
        i2c_eeprom_seq_read(0x50,found + 1,(s8 *)buf1,5);

PSWD:
        cmd_lcd(0x01);
        str_lcd("ENTER PASSWORD:");
        cmd_lcd(0xC0);
        password_kpm((s8 *)buf2);

        if(strcmp((const char*)buf1, (const char *)buf2) != 0)
        {
                cmd_lcd(0x01);
                str_lcd("WRONG PASSWORD ");
                char_lcd(0);
                retry++;

                if(retry == 3)
                {
                        cmd_lcd(0x01);
                        str_lcd("LOGIN FAILS");
                        delay_ms(1000);
                        return;
                }

                cmd_lcd(0xC0);
                str_lcd("you have ");
                char_lcd(3 - retry + 48);
                str_lcd(" chances");
                delay_ms(1000);
                goto PSWD;
        }

        cmd_lcd(0x01);
        str_lcd("PASSWORD VERIFED");
        delay_ms(500);
        retry = 0;

FP:
        cmd_lcd(0x01);
        if(verify(l_id) != 0) /* Tier 2: Biometric matching loop */
        {
                cmd_lcd(0x01);
                str_lcd("FINGER NOT FOUND");
                cmd_lcd(0xC0);
                str_lcd("try again");
                delay_ms(600);
                retry++;

                if(retry == 1)
                {
                        cmd_lcd(0x01);
                        str_lcd("two more chance");
                        delay_ms(800);
                        goto FP;
                }
                else if(retry == 2)
                {
                        cmd_lcd(0x01);
                        str_lcd("last chance");
                        delay_ms(800);
                        goto FP;
                }
                else
                {
                        cmd_lcd(0x01);
                        str_lcd("ACCESS DENIED");
                        delay_ms(1000);
                        return;
                }
        }

        /* BOTH CREDENTIALS CONFIRMED -> TRIGGER RELAY ACTUATOR */
        cmd_lcd(0x01);
        str_lcd("DOOR OPENING ");
        door_open();
        delay_ms(5000);
        motor_stop();

        cmd_lcd(0x01);
        str_lcd("DOOR OPENED ");
        delay_ms(500);

        /* Lockdown delay display sequence */
        for(l = 30; l > 0; l--)
        {
                cmd_lcd(0x80);
                str_lcd("DOOR CLOSE IN ");
                cmd_lcd(0xC0);
                str_lcd("  ");
                char_lcd((l / 10) + 48);
                char_lcd((l % 10) + 48);
                str_lcd(" sec ");
                delay_ms(500);
        }

        cmd_lcd(0x01);
        str_lcd("DOOR IS CLOSEING");
        cmd_lcd(0xC0);
        str_lcd("   ");
        door_close();
        delay_ms(5000);
        motor_stop();

        cmd_lcd(0x01);
        str_lcd("DOOR IS CLOSED ");
        delay_ms(1000);
}

void disable(void) { VICIntEnClr = 1 << EINT2_VIC_CHNO; } /* Mask Interrupts */
void enable(void)  { VICIntEnable |= 1 << EINT2_VIC_CHNO; } /* Unmask Interrupts */

/**
 * @brief  Allows valid users to overwrite and modify passwords in the EEPROM storage.
 */
void password_edit(void)
{
        s8 p_id;
        s8 pw1[5], pw2[5];
        s8 retry = 0;
        u16 found = 0;

        cmd_lcd(0x01);
        str_lcd("ENTER UR ID:");
        p_id = readnum();

        if((found = is_id_in_db(p_id)) == 0)
        {
                cmd_lcd(0x01);
                str_lcd("ID IS NOT FOUND");
                delay_ms(1000);
                return;
        }

        cmd_lcd(0x01);
        str_lcd("ID IS FOUND");
        delay_ms(700);
        i2c_eeprom_seq_read(0x50, found + 1, pw1, 5);

PSWD:
        cmd_lcd(0x01);
        str_lcd("ENTER OLD PASSWORD:");
        cmd_lcd(0xC0);
        password_kpm(pw2);

        if(strcmp((const char*)pw2, (const char *)pw1) != 0)
        {
                cmd_lcd(0x01);
                str_lcd("WRONG PASSWORD");
                retry++;

                if(retry == 3)
                {
                        cmd_lcd(0x01);
                        char_lcd(0);
                        str_lcd("   NOT MATCHED ");
                        char_lcd(0);
                        cmd_lcd(0xC0);
                        str_lcd(" CAN'T CHANGED ");
                        delay_ms(1000);
                        return;
                }

                cmd_lcd(0xC0);
                str_lcd("you have ");
                char_lcd(3 - retry + 48);
                str_lcd(" chances");
                delay_ms(1000);
                goto PSWD;
        }

        cmd_lcd(0x01);
        str_lcd("PASSWORD VERIFED");
        delay_ms(500);
        retry = 0;
   
FP:
        cmd_lcd(0x01);
        if(verify(p_id) != 0)
        {
                cmd_lcd(0x01);
                str_lcd("FINGER NOT FOUND");
                cmd_lcd(0xC0);
                str_lcd("try again");
                delay_ms(600);
                retry++;

                if(retry == 1)
                {
                        cmd_lcd(0x01);
                        str_lcd("two more chance");
                        delay_ms(500);
                        goto FP;
                }
                else if(retry == 2)
                {
                        cmd_lcd(0x01);
                        str_lcd("last chance");
                        delay_ms(500);
                        goto FP;
                }
                else
                {
                        cmd_lcd(0x01);
                        char_lcd(0);
                        str_lcd("   NOT MATCHED ");
                        char_lcd(0);
                        cmd_lcd(0xC0);
                        str_lcd("    CAN'T CHANGED ");
                        delay_ms(1000);
                        return;
                }
        }
        else
        {
PSWD1:
                cmd_lcd(0x01);
                str_lcd("ENTER NEW PASSWORD:");
                cmd_lcd(0xC0);
                password_kpm(pw1);

                cmd_lcd(0x01);
                str_lcd("ENTER AGAIN:");
                cmd_lcd(0xC0);
                password_kpm(pw2);

                if(strcmp((const char*)pw1, (const char*)pw2) != 0)
                {
                        cmd_lcd(0x01);
                        str_lcd("NOT MATCHED");
                        cmd_lcd(0xC0);
                        str_lcd("try again");
                        delay_ms(500);
                        goto PSWD1;
                }
                else
                {
                        i2c_eeprom_write_page(0x50, found + 1, pw1, 5);
                        cmd_lcd(0x01);
                        str_lcd(" NEW PASSWORD ");
                        cmd_lcd(0xC0);
                        str_lcd("UPDATED ");
                        char_lcd(' ');
                        char_lcd(1);
                        delay_ms(500);
                }
        }
}

/**
 * @brief  Overwrites and updates an existing fingerprint ID structure in memory.
 */
void finger_edit(void)
{
        s8 f_id;
        s8 pw1[5], pw2[5];
        s8 retry = 0;
        u16 found;

        cmd_lcd(0x01);
        str_lcd("ENTER THE ID:");
        f_id = readnum();

        if((found = is_id_in_db(f_id)) == 0)
        {
                cmd_lcd(0x01);
                str_lcd("ID IS NOT FOUND");
                delay_ms(1000);
                return;
        }

        cmd_lcd(0x01);
        str_lcd("ID IS FOUND");
        delay_ms(900);
        i2c_eeprom_seq_read(0x50, found + 1, pw2, 5);

PSWD:
        cmd_lcd(0x01);
        str_lcd("ENTER PASSWORD:");
        cmd_lcd(0xC0);
        password_kpm(pw1);

        if(strcmp((const char*)pw2, (const char *)pw1) != 0)
        {
                cmd_lcd(0x01);
                str_lcd("WRONG PASSWORD");
                delay_ms(1000);
                retry++;

                if(retry == 3)
                {
                        cmd_lcd(0x01);
                        char_lcd(0);
                        str_lcd("   NOT MATCHED ");
                        char_lcd(0);
                        cmd_lcd(0xC0);
                        str_lcd("CAN'T CHANGED ");
                        delay_ms(1000);
                        return;
                }

                cmd_lcd(0xC0);
                str_lcd("you have ");
                char_lcd(3 - retry + 48);
                str_lcd(" chances");
                delay_ms(1000);
                goto PSWD;
        }

FP1:
        delete(f_id); /* Erase old template from module sensor bank */

        if(enroll(f_id) != 0) /* Scan and capture replacement biometric pattern */
        {
                cmd_lcd(0x01);
                str_lcd("FAIL FINGER");
                cmd_lcd(0xC0);
                str_lcd("please try again");
                delay_ms(500);
                goto FP1;
        }

        cmd_lcd(0x01);
        str_lcd(" SUCCESSFULLY ");
        cmd_lcd(0xC0);
        str_lcd("  UPDATED ");
        char_lcd(1);
}

/**
 * @brief  Provides an override interface to rewrite keys via a fallback master token.
 */
void forget_password(void)
{
        s8 fo_id;
        s8 pw1[5], pw2[5];
        s8 retry = 0;
        u16 found;

        cmd_lcd(0x01);
        str_lcd("ENTER THE ID:");
        fo_id = readnum();

        if((found = is_id_in_db(fo_id)) == 0)
        {
                cmd_lcd(0x01);
                str_lcd("ID IS NOT FOUND");
                delay_ms(1000);
                return;
        }

        cmd_lcd(0x01);
        str_lcd("ID IS FOUND");
        delay_ms(1000);

PSWD:
        cmd_lcd(0x01);
        str_lcd("ENTER THE DEFAULT ");
        cmd_lcd(0xC0);
        str_lcd("PASSWORD:");
        password_kpm(pw1);

        if(strcmp((const char*)"1729", (const char *)pw1) != 0) /* Hardcoded default escape key */
        {
                cmd_lcd(0x01);
                str_lcd("WRONG PASSWORD");
                delay_ms(1000);
                retry++;

                if(retry == 2)
                {
                        cmd_lcd(0x01);
                        char_lcd(0);
                        str_lcd("   NOT MATCHED ");
                        char_lcd(0);
                        cmd_lcd(0xC0);
                        str_lcd(" CAN'T CHANGED ");
                        delay_ms(1000);
                        return;
                }

                cmd_lcd(0xC0);
                str_lcd("try one more");
                delay_ms(1000);
                goto PSWD;
        }

PSWD1:
        cmd_lcd(0x01);
        str_lcd("ENTER NEW PASSWORD:");
        cmd_lcd(0xC0);
        password_kpm(pw1);

        cmd_lcd(0x01);
        str_lcd("ENTER AGAIN:");
        cmd_lcd(0xC0);
        password_kpm(pw2);

        if(strcmp((const char*)pw1, (const char*)pw2) != 0)
        {
                cmd_lcd(0x01);
                str_lcd("NOT MATCHED");
                cmd_lcd(0xC0);
                str_lcd("try again");
                delay_ms(500);
                goto PSWD1;
        }
        else
        {
                i2c_eeprom_write_page(0x50, found + 1, pw1, 5);
                cmd_lcd(0x01);
                str_lcd(" NEW PASSWORD ");
                cmd_lcd(0xC0);
                str_lcd("UPDATED ");
                char_lcd(' ');
                delay_ms(500);
        }
}

/**
 * @brief  Erases a user's data and collapses the memory array to prevent memory gaps.
 */
void delete_id(void)
{
        s8 id;
        s8 total;
        s8 i;
        s8 buf[6];
        u16 found_addr;
        u16 next_addr;
        u16 last_addr;

ID:
        cmd_lcd(0x01);
        cmd_lcd(0x80);
        str_lcd("ENTER ID:");
        id = readnum();

        found_addr = is_id_in_db(id);
        if(found_addr == 0)
        {
                cmd_lcd(0x01);
                str_lcd("ID NOT FOUND");
                delay_ms(1000);
                goto ID;
        }

        if(delete(id) != 0)
        {
                cmd_lcd(0x01);
                str_lcd("DELETE FAIL");
                delay_ms(1000);
                return;
        }

        total = i2c_eeprom_read_byte(0x50,0x0000);
        last_addr = 0x01 + ((total-1) * 6);

        /* Array compaction loop shift elements leftwards into the erased space */
        while(found_addr < last_addr)
        {
                next_addr = found_addr + 6;
                i2c_eeprom_seq_read(0x50, next_addr, buf, 6);
                i2c_eeprom_write_page(0x50, found_addr, buf, 6);
                found_addr += 6;
        }

        /* Clear tail artifacts */
        for(i=0;i<6;i++) { buf[i] = 0xFF; }
        i2c_eeprom_write_page(0x50, last_addr, buf, 6);

        if(total > 0)
        {
                total--;
                i2c_eeprom_write_byte(0x50, 0x0000, total);
        }

        cmd_lcd(0x01);
        cmd_lcd(0x80);
        str_lcd("USER DELETED");
        cmd_lcd(0xC0);
        str_lcd("SUCCESS");
        delay_ms(1500);
}

/**
 * @brief  Secondary profile data selection state machine router.
 */
void edit(void)
{
        u8 choice;

        if(ids == 0)
        {
                if(flag == 0)
                {
                        flag = 1;
                        cmd_lcd(0x01);
                        str_lcd("NO DATA FOUND");
                        cmd_lcd(0xC0);
                        str_lcd("PRESS SW TO ENROLL");
                }
                return;
        }

        flag = 0;
        while(1)
        {
                cmd_lcd(0x01);
                str_lcd("1.PSSWRD 2.FINGER");
                cmd_lcd(0xC0);
                str_lcd("3.FORGOT 4.EXIT");

                choice = keyscan();
                switch(choice)
                {
                        case '1': password_edit();   break;
                        case '2': finger_edit();     break;
                        case '3': forget_password(); break;
                        case '4': break;
                }

                if(choice == '4') { break; }
        }
}

/**
 * @brief  Primary Execution Administrative context selection interface menu.
 */
void menu(void)
{
        u8 choice;
        menu_flag = 0;
        disable(); /* Safeguard execution context from menu entry loops */
        flag = 0;

        while(1)
        {
                cmd_lcd(LCD_CLR);
                cmd_lcd(GOTO_LINE1_POS_0);
                str_lcd("1.ENROLL 2.EDIT");
                cmd_lcd(GOTO_LINE2_POS_0);
                str_lcd("3.DELETE 4.EXIT");

                choice = keyscan();
                switch(choice)
                {
                        case '1': enroll_id(); break;
                        case '2': edit();      break;
                        case '3': delete_id(); break;
                        case '4': break;
                }

                if(choice == '4')
                {
                        cmd_lcd(0x01);
                        menu_flag = 0;
                        enable(); /* Restore interrupt flag monitoring */
                        char_lcd(ids + 48);
                        break;
                }
        }
}
