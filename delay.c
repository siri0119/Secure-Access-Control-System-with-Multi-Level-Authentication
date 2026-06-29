/**
 * ============================================================================
 * @file        delay.c
 * @brief       Hardware-Locked Blocking Delay Generation Loops
 * @details     Provides cycle-calibrated iteration countdown blocks for precise
 * microsecond, millisecond, and second operational time pauses.
 * ============================================================================
 */

#include "types.h"

/**
 * @brief  Blocks code execution sequence for a specified period of microseconds.
 */
void delay_us(u32 us)
{
        /* Loop tracking scaling calibrations optimized matching core clock rates */
        for(us *= 12; us > 0; us--);
}

/**
 * @brief  Blocks code execution sequence for a specified period of milliseconds.
 */
void delay_ms(u32 ms)
{
        /* Loop tracking scaling calibrations optimized matching core clock rates */
        for(ms *= 12000; ms > 0; ms--);
}

/**
 * @brief  Blocks code execution sequence for a specified period of seconds.
 */
void delay_s(u32 s)
{
        /* Loop tracking scaling calibrations optimized matching core clock rates */
        for(s *= 12000000; s > 0; s--);
}