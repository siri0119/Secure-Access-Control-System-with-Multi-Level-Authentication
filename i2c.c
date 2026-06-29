/**
 * **************************************************************************
 * @file    i2c.c
 * @brief   Hardware Peripheral Driver for I2C Implementation on LPC21xx.
 * **************************************************************************
 */

#include <lpc21xx.h>
#include "i2c_defines.h"
#include "types.h"

/**
 * @brief  Initializes I2C0 Hardware registers with parameter constraints.
 */
void init_i2c(void)
{
        PINSEL0 &= ~(15<<2*2);                   /* Wipe line pin configs on P0.2 and P0.3 */
        PINSEL0 |=  SCL_PIN_FUNC | SDA_PIN_FUNC; /* Map alternative hardware SCL/SDA functions */
        I2SCLL = LOAD_VAL;                       /* Set Duty Cycle registers for Clock Line Low */
        I2SCLH = LOAD_VAL;                       /* Set Duty Cycle registers for Clock Line High */
        I2CONSET = 1<<I2EN;                      /* Turn on the core I2C Peripheral Engine */
}

/**
 * @brief  Generates Master Bus Transmission START condition sequence.
 */
void i2c_start(void)
{
        I2CONSET = 1<<STA;                     /* Assert Start Flag */
        while(((I2CONSET>>SI)&1)==0);          /* Wait for Hardware Interrupt Flag confirmation */
        I2CONCLR = 1<<STAC;                    /* Reset Start State Bit */
}

/**
 * @brief  Generates Master Bus Transmission STOP condition sequence.
 */
void i2c_stop(void)
{
        I2CONSET  = 1<<STO;                    /* Assert Stop Bit */
        I2CONCLR = 1<<SIC;                     /* Release block latch to finalize state */
}

/**
 * @brief  Issues a REPEATED START condition sequence for atomic switch operations.
 */
void i2c_restart(void)
{
        I2CONSET = 1<<STA;                     /* Assert Start Bit */
        I2CONCLR = 1<<SIC;                     /* Force clear Status Indicator */
        while(((I2CONSET>>SI)&1)==0);          /* Sync wait loop until locked */
        I2CONCLR = 1<<STA;                     /* Retract baseline bit */
}

/**
 * @brief  Pushes an 8-bit word into the I2C shift register.
 */
void i2c_write(u8 sdat)
{
        I2DAT = sdat;                          /* Charge data transmission register */
        I2CONCLR = 1<<SIC;                     /* Clear interrupt flag to ignite clock out */
        while(((I2CONSET>>SI)&1)==0);          /* Hold execution until hardware data flushes */
}

/**
 * @brief  Reads an inbound byte and responds with a Negative Acknowledge (NACK).
 */
u8 i2c_nack(void)
{
        I2CONCLR = 1<<AAC;                     /* Disengage Master Auto-Acknowledge pulse generation */
        I2CONCLR = 1<<SIC;                     /* Advance data pipeline shift operation */
        while(((I2CONSET>>SI)&1)==0);          /* Wait until data shifts completely into memory */
        return I2DAT;                          /* Harvest received data value */
}

/**
 * @brief  Reads an inbound byte and responds with a Positive Acknowledge (MACK).
 */
u8 i2c_mack(void)
{
        I2CONSET = 1<<AA;                      /* Enable Master Auto-Acknowledge pulse generation */
        I2CONCLR = 1<<SIC;                     /* Clear execution gate indicator */
        while(((I2CONSET>>SI)&1)==0);          /* Wait for receipt validation status flag */
        I2CONCLR = 1<<AAC;                     /* Revoke bit declaration */
        return I2DAT;                          /* Harvest received data value */
}