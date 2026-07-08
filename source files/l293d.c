#include <lpc21xx.h>
#include "macros.h"
#include "l293d.h"


// Initialize motor pins
void init_l293d(void)
{
        // Set M1 and M2 pins as output
        IODIR0 |= ((1<<M1) | (1<<M2));
}


// Open the door
void door_open(void)
{
        // Set M1 pin high
        G_SETBIT(IOSET0,M1);

        // Set M2 pin low
        G_SETBIT(IOCLR0,M2);
}


// Close the door
void door_close(void)
{
        // Set M1 pin low
        G_SETBIT(IOCLR0,M1);

        // Set M2 pin high
        G_SETBIT(IOSET0,M2);
}


// Stop the motor
void motor_stop(void)
{
        // Set M1 pin low
        G_SETBIT(IOCLR0,M1);

        // Set M2 pin low
        G_SETBIT(IOCLR0,M2);
}
