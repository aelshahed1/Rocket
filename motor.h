#pragma once
#include "pins.h"   //include pins header file

#define PI 3.14159265359  //definition of pi
#define wheel_diameter 2.10   //wheel diameter in cm
#define ticks_per_revolution 14  // encoder ticks per revolution

volatile unsigned int encoder_ticks = 0;   // variable to store encoder ticks 


// Motor functions prototypes
void motor_setup(void);  
void move_constant_speed(int direction, int anti_direction, int PWM_value);  
void move_distance(int direction, int anti_direction, double distance, int speed); 
void motor_stop(void);  


// Encoder functions prototyopes
void encoder_serial_debug(void); 
void encoder_CHA_counting(void); 
void encoder_CHB_counting(void);
void measure_distance(void);
int get_encoder_ticks(void); 
