// Start header guard
#ifndef __kpm_h__

// Define header guard
#define __kpm_h__

// Include user defined data types
#include "types.h"


// Keypad row pins
#define row0 16   // Row 0 pin
#define row1 17   // Row 1 pin
#define row2 18   // Row 2 pin
#define row3 19   // Row 3 pin


// Keypad column pins
#define col0 20   // Column 0 pin
#define col1 21   // Column 1 pin
#define col2 22   // Column 2 pin
#define col3 23   // Column 3 pin


// Initialize keypad
void init_kpm(void);

// Check keypad columns
u32 colscan(void);

// Read pressed key
s32 keyscan(void);

// Check keypad row
u32 rowcheck(void);

// Check keypad column
u32 colcheck(void);

// Read number
s32 readnum(void);

// Read ID
void id_kpm(s8 *ptr);

// Read password
void password_kpm(s8 *ptr);


// End header guard
#endif
