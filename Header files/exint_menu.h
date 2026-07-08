#include "types.h"

// Start header guard
#ifndef _EXINT_MENU_H_

// Define header guard
#define _EXINT_MEMU_H_

// EINT2 input pin value
#define EINT2_INPUT_PIN 0x0000C000

// EINT2 VIC channel number
#define EINT2_VIC_CHNO 16


// Initialize EINT2
void init_eint2(void);

// EINT2 interrupt function
void eint2_isr(void) __irq;

// Check ID in database
unsigned short is_id_in_db(char id);

// Enroll new ID
void enroll_id(void);

// Open menu
void menu(void);

// Check admin
s8 check_admin(void);

// Login function
void login(void);

// Forgot password function
void forget_password(void);

// Edit fingerprint
void finger_edit(void);

// Edit password
void password_edit(void);

// Initialize IDs
void init_ids(void);

// Disable EINT2
void disable(void);

// Enable EINT2
void enable(void);

// Open edit menu
void edit(void);

// Delete ID
void delete_id(void);

// Display title
void title(void);

// End header guard
#endif
