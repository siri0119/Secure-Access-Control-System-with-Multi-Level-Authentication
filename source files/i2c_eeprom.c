// i2c_eeprom.c

#include "delay.h"
#include "types.h"
#include "i2c.h"


// Write one byte into EEPROM
void i2c_eeprom_write_byte(u8 slaveAddr, u16 wBuffAddr, u8 dat)
{
	// Start I2C
	i2c_start();

	// Send slave address for write
	i2c_write(slaveAddr << 1);

	// Send buffer address
	i2c_write(wBuffAddr >> 8);
	i2c_write(wBuffAddr);

	// Send data
	i2c_write(dat);

	// Stop I2C
	i2c_stop();

	// Wait after write
	delay_ms(10);
}


// Read one byte from EEPROM
u8 i2c_eeprom_read_byte(u8 slaveAddr, u16 rBuffAddr)
{
	u8 dat;

	// Start I2C
	i2c_start();

	// Send slave address for write
	i2c_write(slaveAddr << 1);

	// Send buffer address
	i2c_write(rBuffAddr >> 8);
	i2c_write(rBuffAddr);

	// Restart I2C
	i2c_restart();

	// Send slave address for read
	i2c_write((slaveAddr << 1) | 1);

	// Read data without ACK
	dat = i2c_nack();

	// Stop I2C
	i2c_stop();

	// Return read data
	return dat;
}


// Write multiple bytes into EEPROM
void i2c_eeprom_write_page(u8 slaveAddr, u16 wBuffStartAddr,
                           s8 *p, u8 nBytes)
{
	int i;

	// Start I2C
	i2c_start();

	// Send slave address for write
	i2c_write(slaveAddr << 1);

	// Send start buffer address
	i2c_write(wBuffStartAddr >> 8);
	i2c_write(wBuffStartAddr);

	// Write all bytes
	for(i = 0; i < nBytes; i++)
	{
		i2c_write(p[i]);
	}

	// Stop I2C
	i2c_stop();

	// Wait after write
	delay_ms(10);
}


// Read multiple bytes from EEPROM
void i2c_eeprom_seq_read(u8 slaveAddr, u16 rBuffStartAddr,
                         s8 *p, u8 nBytes)
{
	u8 i;

	// Start I2C
	i2c_start();

	// Send slave address for write
	i2c_write(slaveAddr << 1);

	// Send start buffer address
	i2c_write(rBuffStartAddr >> 8);
	i2c_write(rBuffStartAddr);

	// Restart I2C
	i2c_restart();

	// Send slave address for read
	i2c_write((slaveAddr << 1) | 1);

	// Read bytes with ACK
	for(i = 0; i < (nBytes - 1); i++)
	{
		p[i] = i2c_mack();
	}

	// Read last byte without ACK
	p[i] = i2c_nack();

	// Add null character
	p[i + 1] = '\0';

	// Stop I2C
	i2c_stop();
}
