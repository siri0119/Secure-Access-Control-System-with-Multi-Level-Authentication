/**
 * ============================================================================
 * @file        delay.h
 * @brief       Timing Engine Abstract Implementation API Prototypes
 * ============================================================================
 */

#ifndef _DELAY_H_                
#define _DELAY_H_                

#include "types.h"               

/* Public Clock Gating APIs declarations */
void delay_us(u32);
void delay_ms(u32);
void delay_s(u32);

#endif
