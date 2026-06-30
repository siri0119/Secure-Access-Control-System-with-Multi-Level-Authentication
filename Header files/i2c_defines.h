/**
 * **************************************************************************
 * @file    i2c_defines.h
 * @brief   Clock distribution calculations and register mask constants for I2C.
 * **************************************************************************
 */

#ifndef _I2C_DEFINES_H_              
#define _I2C_DEFINES_H_              

/* Hardware Peripheral Pins allocations mapping */
#define SCL 2                         
#define SDA 3                         

/* Pin Alternate Function configuration values */
#define SCL_PIN_FUNC 0X00000010      
#define SDA_PIN_FUNC 0X00000040      

/* Frequency Engine Calculations */
#define FOSC       12000000          /* Quartz Crystal Oscillator input frequency */
#define CCLK      (FOSC*5)           /* Core CPU processing Clock frequency speed */
#define PCLK      (CCLK/4)           /* Peripheral Clock distribution speed output */
#define I2C_SPEED 100000             /* Desired transmission rate target (100kHz) */
#define LOAD_VAL  ((PCLK/I2C_SPEED)/2) /* Register countdown period calculation divisor */

/* Bit position masks for the I2C Control Set Register (I2CONSET) */
#define AA   2                       /* Assert Acknowledge Flag */
#define SI   3                       /* I2C State Interrupt Bit */
#define STO  4                       /* Assert STOP Bit */
#define STA  5                       /* Assert START Bit */
#define I2EN 6                       /* Hardware Engine Interface Enable Bit */

/* Bit position masks for the I2C Control Clear Register (I2CONCLR) */
#define AAC    2                     /* Clear Acknowledge flag */
#define SIC    3                     /* Clear Status Interrupt flag */
#define STAC   5                     /* Clear START state tracker */
#define I2ENC  6                     /* Disengage hardware engine block */

#endif
