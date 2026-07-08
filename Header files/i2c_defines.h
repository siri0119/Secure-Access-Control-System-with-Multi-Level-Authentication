// Start header guard
#ifndef _I2C_DEFINES_H_

// Define header guard
#define _I2C_DEFINES_H_


// I2C pin numbers
#define SCL 2   // SCL pin
#define SDA 3   // SDA pin


// I2C pin function values
#define SCL_PIN_FUNC 0X00000010   // Select SCL function
#define SDA_PIN_FUNC 0X00000040   // Select SDA function


// Clock values
#define FOSC 12000000   // Oscillator frequency

#define CCLK (FOSC*5)   // CPU clock

#define PCLK (CCLK/4)   // Peripheral clock

#define I2C_SPEED 100000   // I2C speed

#define LOAD_VAL ((PCLK/I2C_SPEED)/2)   // I2C clock load value


// I2CONSET bit positions
#define AA   2    // Acknowledge bit
#define SI   3    // Interrupt flag bit
#define STO  4    // Stop bit
#define STA  5    // Start bit
#define I2EN 6    // I2C enable bit


// I2CONCLR bit positions
#define AAC   2    // Clear acknowledge bit
#define SIC   3    // Clear interrupt flag
#define STAC  5    // Clear start bit
#define I2ENC 6    // Clear I2C enable bit


// End header guard
#endif
