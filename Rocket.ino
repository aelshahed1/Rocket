//include header files used in this programme
#include <vector>
#include <math.h>
#include "pins.h"
#include"motor.h"
#include"Light_Sensor.h"
#include"Acc_and_gyro.h"
#include "BLE.h"
using namespace std;

int state = 1;  // chosing the starting state of the program
int lap_start_time;  // variable to store the value of time passed since start of software
float lapTime;  // value to store the lap time
BLEService Track_timer("0000");  //creating the BLE service to use
BLEUnsignedIntCharacteristic lap_time("1000", BLERead | BLENotify);  //assigining a characterisitc to the BLE service which is the lap time of time Read and notify when change

void setup() {  //Running all setup functions at the start of the program
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(9600);
	motor_setup();
	light_sensor_setup();
	gyro_setup();
	ble_setup();
}

void loop() {
	if (state == 1) {
		while (!ble_connected()) {  //wait for a device to connect via BLE
			digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
			delay(300);
		}
		for (int i = 0; i < 500; i++) {  //delay 5 seconds for BLE to connect 
			send_time(0.0);
		}
		move_constant_speed(Motor_Forwards, Motor_Backwards, 125);  //car starts moving 
		while (!line_detected()) {  // run the speed control algorithm while polling for the start line
			turning();
		}
		lap_start_time = millis();  // store time passed since start of the program
		delay(20);
		state = 2;  // go to state 2
	}
	if (state == 2) {	
		while (!line_detected()){  // run the speed control algorithm while polling for the start line
			turning();
		}
		lapTime = (float)(millis() - lap_start_time) / 1000;  //calculate lap time
		lap_start_time = millis();  // store start time for new lap
		send_time(lapTime); //send time via BLE
	}
}

