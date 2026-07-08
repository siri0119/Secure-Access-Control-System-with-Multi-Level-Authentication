// Start header guard
#ifndef __l293d

// Define header guard
#define __l293d


// Motor pin M1
#define M1 5

// Motor pin M2
#define M2 6


// Initialize motor
void init_l293d(void);

// Open door
void door_open(void);

// Close door
void door_close(void);

// Stop motor
void motor_stop(void);


// End header guard
#endif
