#pragma once
#include "pins.h"  //include pins header file

//light sensor functions prototypes
void light_sensor_setup(void);
bool line_detected(void);
void light_sensor_debug(void);
int get_sensor_value(void);