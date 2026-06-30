#ifndef __l293d                     
#define __l293d                     

/* ==========================================================================
 * HARDWARE CONFIGURATION: L293D Motor Driver Pin Assignments
 * ========================================================================== */
#define M1 5                        // Control Pin 1 for Actuator Motor
#define M2 6                        // Control Pin 2 for Actuator Motor

/* ==========================================================================
 * API FUNCTION PROTOTYPES: Access Control Mechanism Drivers
 * ========================================================================== */
void init_l293d(void);              // Configures GPIOs for driver interfacing
void door_open(void);               // Drives motor clockwise to unlatch door
void door_close(void);              // Drives motor counter-clockwise to lock door
void motor_stop(void);              // Cuts power to motor (brakes mechanism)

#endif
