// Start header guard
#ifndef _R305_H_

// Define header guard
#define _R305_H_


// Include user defined data types
#include "types.h"


// Send packet
void send_packet(u8 *ptr,u8 n);

// Capture fingerprint image
s8 Genimg(void);

// Convert image to template buffer 1
s8 img2tp1(void);

// Convert image to template buffer 2
s8 img2tp2(void);

// Generate fingerprint model
s8 regmodel(void);

// Store fingerprint
s8 store(s8 id);

// Delete fingerprint
s8 delete(s8 id);

// Match fingerprints
s8 match(void);

// Load stored fingerprint
s8 load_char(s8 id);

// Enroll fingerprint
s8 enroll(s8 id);

// Verify fingerprint
s8 verify(s8 id);


// End header guard
#endif
