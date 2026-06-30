#ifndef _R305_H_                     
#define _R305_H_                     

#include "types.h"                  

/* ==========================================================================
 * TRANSCEIVER ENGINE & PIPELINE MANAGERS
 * ========================================================================== */
void send_packet(u8 *ptr,u8 n);     // Low level output byte driver interface
s8 Genimg(void);                    // Photo-matrix scan capture trigger routine
s8 img2tp1(void);                   // Structural vector capture parser mapping loop 1
s8 img2tp2(void);                   // Structural vector capture parser mapping loop 2
s8 regmodel(void);                  // Synthesis execution linking routine
s8 store(s8 id);                    // Local hardware sector data commit routine
s8 delete(s8 id);                   // Local hardware sector data erase routine
s8 match(void);                     // Differential vector arithmetic execution logic
s8 load_char(s8 id);                // Database flash read routine mirroring vector slots
s8 enroll(s8 id);                   // Full sequence entry validation workflow interface
s8 verify(s8 id);                   // Security checkpoint challenge handler routine

#endif
