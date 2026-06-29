/**
 * **************************************************************************
 * @file    l293d.c
 * @brief   H-Bridge Motor Driver Control (Door Actuator Open/Close/Stop Actions).
 * **************************************************************************
 */

#include <lpc21xx.h>
#include "macros.h"
#include "l293d.h"

/**
 * @brief  Configures designated GPIO control channels as outputs.
 */
void init_l293d(void)
{
        IODIR0 |= ((1<<M1) | (1<<M2)); /* Assign H-Bridge control lines to Output mode */
}

/**
 * @brief  Drives the H-Bridge lines to actuate forward rotational movement (Open Door).
 */
void door_open(void)
{
         G_SETBIT(IOSET0,M1);  /* Drive line M1 High */
         G_SETBIT(IOCLR0,M2);  /* Pull line M2 Low */
}

/**
 * @brief  Drives the H-Bridge lines to actuate reverse rotational movement (Close Door).
 */
void door_close(void)
{
         G_SETBIT(IOCLR0,M1);  /* Pull line M1 Low */
         G_SETBIT(IOSET0,M2);  /* Drive line M2 High */
}

/**
 * @brief  Brakes the H-Bridge to stall rotational shaft movement (Stop Door).
 */
void motor_stop(void)
{
         G_SETBIT(IOCLR0,M1);  /* Force line M1 Low */
         G_SETBIT(IOCLR0,M2);  /* Force line M2 Low */
}