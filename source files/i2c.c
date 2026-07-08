#include <lpc21xx.h>
#include "i2c_defines.h"
#include "types.h"


// Initialize I2C
void init_i2c(void)
{
        // Clear I2C pin function bits
        PINSEL0 &= ~(15<<2*2);

        // Select SCL and SDA pin functions
        PINSEL0 |= SCL_PIN_FUNC | SDA_PIN_FUNC;

        // Set I2C clock low value
        I2SCLL = LOAD_VAL;

        // Set I2C clock high value
        I2SCLH = LOAD_VAL;

        // Enable I2C
        I2CONSET = 1<<I2EN;
}


// Generate I2C start condition
void i2c_start(void)
{
        // Set START bit
        I2CONSET = 1<<STA;

        // Wait until SI bit is set
        while(((I2CONSET>>SI)&1)==0);

        // Clear START bit
        I2CONCLR = 1<<STAC;
}


// Generate I2C stop condition
void i2c_stop(void)
{
        // Set STOP bit
        I2CONSET = 1<<STO;

        // Clear SI bit
        I2CONCLR = 1<<SIC;
}


// Generate I2C restart condition
void i2c_restart(void)
{
        // Set START bit
        I2CONSET = 1<<STA;

        // Clear SI bit
        I2CONCLR = 1<<SIC;

        // Wait until SI bit is set
        while(((I2CONSET>>SI)&1)==0);

        // Clear START bit
        I2CONCLR = 1<<STA;
}


// Write one byte
void i2c_write(u8 sdat)
{
        // Store data in I2C data register
        I2DAT = sdat;

        // Clear SI bit
        I2CONCLR = 1<<SIC;

        // Wait until SI bit is set
        while(((I2CONSET>>SI)&1)==0);
}


// Read data without acknowledgment
u8 i2c_nack(void)
{
        // Clear acknowledge bit
        I2CONCLR = 1<<AAC;

        // Clear SI bit
        I2CONCLR = 1<<SIC;

        // Wait until SI bit is set
        while(((I2CONSET>>SI)&1)==0);

        // Return received data
        return I2DAT;
}


// Read data with acknowledgment
u8 i2c_mack(void)
{
        // Set acknowledge bit
        I2CONSET = 1<<AA;

        // Clear SI bit
        I2CONCLR = 1<<SIC;

        // Wait until SI bit is set
        while(((I2CONSET>>SI)&1)==0);

        // Clear acknowledge bit
        I2CONCLR = 1<<AAC;

        // Return received data
        return I2DAT;
}
