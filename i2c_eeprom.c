/**
 * **************************************************************************
 * @file    i2c_eeprom.c
 * @brief   Device Driver Implementation for standard serial AT24Cxx EEPROM ICs.
 * **************************************************************************
 */

#include"delay.h"
#include"types.h"
#include"i2c.h"

/**
 * @brief  Writes an unaligned data byte into a specified EEPROM target address location.
 */
void i2c_eeprom_write_byte(u8 slaveAddr,u16 wBuffAddr,u8 dat)
{
	i2c_start();
	i2c_write(slaveAddr<<1);   /* Pass standard slave target device location token (Write Option) */
	i2c_write(wBuffAddr>>8);   /* Ship upper byte segment of internal data address */
	i2c_write(wBuffAddr);      /* Ship lower byte segment of internal data address */
	i2c_write(dat);            /* Transmit payload data character */
	i2c_stop();
	delay_ms(10);              /* Enforce essential write cycle pause window requirement */
}

/**
 * @brief  Fetches an individual data byte from a specified target location.
 */
u8 i2c_eeprom_read_byte(u8 slaveAddr,u16 rBuffAddr)
{
	u8 dat;
	i2c_start();
	i2c_write(slaveAddr<<1);   /* Setup target parameters matching the component */
	i2c_write(rBuffAddr>>8);   /* Address MSB alignment write */
	i2c_write(rBuffAddr);      /* Address LSB alignment write */
	i2c_restart();             /* Execute standard I2C handover procedure changeover */
	i2c_write((slaveAddr<<1)|1);/* Switch operational directive bit high (Read Option) */
	dat=i2c_nack();            /* Pull string segment payload data */
	i2c_stop();
	return dat;
}

/**
 * @brief  Streams a sequential page block data write across consecutive address locations.
 */
void i2c_eeprom_write_page(u8 slaveAddr,u16 wBuffStartAddr,s8 *p,u8 nBytes)
{
	int i;
	i2c_start();
	i2c_write(slaveAddr<<1);
	i2c_write(wBuffStartAddr>>8);
	i2c_write(wBuffStartAddr);
	for(i=0;i<nBytes;i++)
	{
		i2c_write(p[i]);       /* Sequential flash write operations loop */
	}
	i2c_stop();
	delay_ms(10);              /* Wait window required for internal data routing */
}

/**
 * @brief  Performs a multi-byte sequential block read operation from EEPROM memory.
 */
void i2c_eeprom_seq_read(u8 slaveAddr,u16 rBuffStartAddr, s8 *p,u8 nBytes)
{
	u8 i;
	i2c_start();
	i2c_write(slaveAddr<<1);
	i2c_write(rBuffStartAddr>>8);
	i2c_write(rBuffStartAddr);
	i2c_restart();
	i2c_write(slaveAddr<<1|1);
    
	for(i=0;i<(nBytes-1);i++)
	{
		p[i]=i2c_mack();       /* Keep pulling bytes using consecutive positive acknowledgements */
	}
	p[i]=i2c_nack();           /* Conclude reading sequence stream operations using NACK */
	p[i+1]='\0';               /* Insert string termination marker character */
	i2c_stop();
}