// Start header guard
#ifndef _I2C_EEPROM_H_

// Define header guard
#define _I2C_EEPROM_H_

// Include user defined data types
#include "types.h"


// Write one byte to EEPROM
void i2c_eeprom_write_byte
(
        u8 SlaveAddr,
        u16 BuffAddr,
        u8 data
);


// Read one byte from EEPROM
u8 i2c_eeprom_read_byte
(
        u8 SlaveAddr,
        u16 BufferAddr
);


// Write multiple bytes to EEPROM
void i2c_eeprom_write_page
(
        u8 SlavAddr,
        u16 BuffAddr,
        s8 *P,
        u8 nbytes
);


// Read multiple bytes from EEPROM
void i2c_eeprom_seq_read
(
        u8 SlaveAddr,
        u16 BuffAdrr,
        s8 *p,
        u8 nbytes
);


// End header guard
#endif
