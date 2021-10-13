#pragma once
#include"Acc_and_gyro.h"

vector<float> gyroX_original, gyroY_original, gyroZ_original;  //vectors of type float to store readings from the the gyroscp[e
vector<int> enc_ticks , light_sensor; //vectors of type int to store readings from the light sensor and motor encoder 
int stop_flag = 0; // a flag for the car to reverse break after the detection of a turn

void gyro_setup(void) {  //setup the gyroscope for later used
	while (!IMU.begin());
	delay(10);
}

void gyro_serial_debug(void) {  //function to print the values from the gyroscope to the serial monitor for debugging
	 
	while (!Serial);   // wait for serial monitor to be opened
	delay(500);
	Serial.println("X\tY\tZ");
	
	float x, y, z;

	if (IMU.gyroscopeAvailable()) {   //get readings from gyroscope if available 
		IMU.readGyroscope(x, y, z);

		//print values obtained to serial monitor
		Serial.print(x);
		Serial.print("/t");
		Serial.print(y);
		Serial.print("/t");
		Serial.println(z);
		delay(10);
	}
}



void gyro_save_serial_data(void) {  //function to collect data from the gyroscope as the car moves and prints them to serial monitor later
	
	while (!Serial) {  //collect data until the microcontroller is connected to serial monitor

		move_constant_speed(Motor_Forwards, Motor_Backwards, 125);  //car moving along the track
		digitalWrite(LED_BUILTIN, HIGH);
		float x, y, z;

		if (IMU.gyroscopeAvailable()) {
			IMU.readGyroscope(x, y, z);
			
			//store data collected in vectors previously created
			gyroX_original.push_back(abs(x));
			gyroY_original.push_back(abs(y));
			gyroZ_original.push_back(abs(z));
			enc_ticks.push_back(get_encoder_ticks());
			light_sensor.push_back(get_sensor_value());
			delay(10);
		}
	}
			motor_stop(); //stop the car after serial connection
			digitalWrite(LED_BUILTIN, LOW);

			delay(1000);
			Serial.print("No. of  samples =    ");
			Serial.println(gyroX_original.size());
			Serial.println("Sample\tline\tticks\tX\tY\tZ");
			delay(500);

			for (int i = 0; i < gyroX_original.size(); i++) {   //print all the data collected to serial monitor
				Serial.print(i + 1);
				Serial.print('\t');
				Serial.print(light_sensor[i]);
				Serial.print('\t');
				Serial.print(enc_ticks[i]);
				Serial.print('\t');
				Serial.print(gyroX_original[i]);
				Serial.print('\t');
				Serial.print(gyroY_original[i]);
				Serial.print('\t');
				Serial.println(gyroZ_original[i]);
			}
			Serial.println("done");		
}

void turning(void) {  //function to control the speed of the car by detecting turns
	move_constant_speed(Motor_Forwards, Motor_Backwards, 120); //start moving
	float x, y, z;
	
	if (IMU.gyroscopeAvailable()) {
		IMU.readGyroscope(x, y, z);
		gyroZ_original.push_back(abs(z));   //get the absolute value of the gyroscope Z-axis reading
		if (gyroZ_original.size() > 9) {
			bool turning;  // variable to store the state of the car 
			for (int j = (gyroZ_original.size() - 1); j > (gyroZ_original.size() - 6); j--) {    // check 5 consecutive readings

				if ((gyroZ_original[j] > 50) && (gyroZ_original[j - 1] > 50)) {  //if 2 consective readings are higher than 50 dps, the car is turning
					turning = true;
					break; //break loop if car is turning
				}
				else {
					turning = false;
				}
			}
			if (turning) {
				digitalWrite(LED_BUILTIN, HIGH);  //turn LED while turning
				if (stop_flag++ == 0) { // reverse breaking after detecting a turn for 1 time
					move_constant_speed(Motor_Backwards, Motor_Forwards, 180);
					delay(350);
					motor_stop();
				}
				move_constant_speed(Motor_Forwards, Motor_Backwards, 125);  // move slowly until a straight line is detected
			}
			else {
				stop_flag = 0;  // reset stop flag so the car reverse break again when a new turn comes
				move_constant_speed(Motor_Forwards, Motor_Backwards, 180);  //move fast
				digitalWrite(LED_BUILTIN, LOW); // turn LED off while moving in a straight line
			}
		}
	}
		delay(10);
}

