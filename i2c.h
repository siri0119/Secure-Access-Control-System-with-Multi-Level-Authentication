/**
 * **************************************************************************
 * @file    i2c.h
 * @brief   Abstract prototypes for the LPC21xx I2C engine driver layer.
 * **************************************************************************
 */

#ifndef _I2C_H_                      
#define _I2C_H_                      

#include "types.h"                  

void init_i2c(void);                
void i2c_start(void);               
void i2c_stop(void);                
void i2c_restart(void);             
void i2c_write(u8 sdat);            
u8 i2c_mack(void);                  
u8 i2c_nack(void);                  

#endif