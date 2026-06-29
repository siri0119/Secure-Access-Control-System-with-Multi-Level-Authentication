/**
 * **************************************************************************
 * @file    exint_menu.h
 * @brief   Function declarations and vector table channels for System UI.
 * **************************************************************************
 */

#include "types.h"
#ifndef _EXINT_MENU_H_                 
#define _EXINT_MEMU_H_                 

/* Pin Hardware Mapping Definitions */
#define EINT2_INPUT_PIN 0x0000C000    /* Map P0.7 as EINT2 Alternative Function */
#define EINT2_VIC_CHNO 16             /* Channel 16 assigned to External Interrupt 2 */

/* Public API Functions Declarations */
void init_eint2(void);                
void eint2_isr(void) __irq;           
unsigned short is_id_in_db(char id);  
void enroll_id(void);                 
void menu(void);                      
s8 check_admin(void);                 
void login(void);                     
void forget_password(void);           
void finger_edit(void);               
void password_edit(void);             
void init_ids(void);                  
void disable(void);                   
void enable(void);                    
void edit(void);                      
void delete_id(void);                 
void title(void);

#endif