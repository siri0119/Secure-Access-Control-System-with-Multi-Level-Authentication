#include "types.h"   // Includes user-defined data types


// Function to generate delay in microseconds
void delay_us(u32 us)
{
        for(us *= 12; us > 0; us--);
}


// Function to generate delay in milliseconds
void delay_ms(u32 ms)
{
        for(ms *= 12000; ms > 0; ms--);
}


// Function to generate delay in seconds
void delay_s(u32 s)
{
        for(s *= 12000000; s > 0; s--);
}
