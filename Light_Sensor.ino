#pragma once
#include"Light_Sensor.h"

void light_sensor_setup(void) {  //functions to set the pins for the light sensor
	pinMode(Sensor_SIG, INPUT);
}

int get_sensor_value(void) {  //return current reading value from the light sensor
return analogRead(Sensor_SIG);
}

bool line_detected(void) {  //function to return 1 if line is detected and 0 otherwise
	int sensor_value = analogRead(Sensor_SIG);

	if (sensor_value > 500) {  // if value is higher than 500, return 1
		return true; 
	}
	else {
		return false;
	}
}

void light_sensor_debug(void) {  //print the reading values from the light sensor to the serial monitor for debugging
	Serial.println(analogRead(Sensor_SIG));
}