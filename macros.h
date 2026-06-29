#ifndef _MACROS_H_                   
#define _MACROS_H_                   

/* ==========================================================================
 * BIT MANIPULATION WRAPPERS: Atomic Register Operations
 * ========================================================================== */
#define SETBIT(WORD,POS) (WORD |= 1<<POS)           // Atomically force bit at position high
#define CLRBIT(WORD,POS) (WORD &= ~(1<<POS))        // Atomically force bit at position low
#define G_SETBIT(WORD,POS) (WORD = 1<<POS)          // Direct write overriding register state
#define RBIT(WORD,BITPOS) ((WORD>>BITPOS)&1)        // Interrogate individual isolated bit status

/* ==========================================================================
 * BUS LEVEL WRAPPERS: Multi-bit Struct Operations
 * ========================================================================== */
// Completely wipe an 8-bit block slot and overwrite it with a safe data byte
#define WBYTE(WORD,POS,DATA) \
(WORD = (WORD&~(255<<POS))|(DATA<<POS))             

// Completely wipe a 4-bit block slot and overwrite it with a safe nibble payload
#define WNIBBLE(WORD,POS,DATA) \
(WORD =(WORD&~(15<<POS))|(DATA<<POS))               

#define RNIBBLE(WORD,POS) ((WORD>>POS)&15)          // Sample an isolated 4-bit field snippet

#endif