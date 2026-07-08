// Start header guard
#ifndef _I2C_H_

// Define header guard
#define _I2C_H_

// Include user defined data types
#include "types.h"


// Initialize I2C
void init_i2c(void);

// Start I2C
void i2c_start(void);

// Stop I2C
void i2c_stop(void);

// Restart I2C
void i2c_restart(void);

// Write one byte
void i2c_write(u8 sdat);

// Read one byte with ACK
u8 i2c_mack(void);

// Read one byte without ACK
u8 i2c_nack(void);


// End header guard
#endif
