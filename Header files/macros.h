// Start header guard
#ifndef _MACROS_H_

// Define header guard
#define _MACROS_H_


// Set a bit
#define SETBIT(WORD,POS) (WORD |= 1<<POS)

// Clear a bit
#define CLRBIT(WORD,POS) (WORD &= ~(1<<POS))

// Set only one bit
#define G_SETBIT(WORD,POS) (WORD = 1<<POS)

// Read a bit
#define RBIT(WORD,BITPOS) ((WORD>>BITPOS)&1)


// Write one byte
#define WBYTE(WORD,POS,DATA) \
(WORD = (WORD&~(255<<POS))|(DATA<<POS))


// Write one nibble
#define WNIBBLE(WORD,POS,DATA) \
(WORD =(WORD&~(15<<POS))|(DATA<<POS))

// Read one nibble
#define RNIBBLE(WORD,POS) ((WORD>>POS)&15)


// End header guard
#endif
