#include "motor.h"

 float wheel_circumference = PI * wheel_diameter;  //calculation of wheel's circumference
 volatile float distance_moved;  //variable to store distance moved by the car

 //motor functions
 void motor_setup(void) {    //Function to setup up the PWM pins to use the motor and attach ISR to encoder pins
	 pinMode(Motor_Forwards, OUTPUT);
	 pinMode(Motor_Backwards, OUTPUT);
	 digitalWrite(Motor_Forwards, LOW);
	 digitalWrite(Motor_Backwards, LOW);
	 pinMode(ENC_CHA, INPUT_PULLUP);
	 pinMode(ENC_CHB, INPUT_PULLUP);
	 attachInterrupt(digitalPinToInterrupt(ENC_CHA), encoder_CHA_counting, CHANGE);
	 attachInterrupt(digitalPinToInterrupt(ENC_CHA), measure_distance, CHANGE);
	 attachInterrupt(digitalPinToInterrupt(ENC_CHB), encoder_CHB_counting, CHANGE);
 }
 
 void move_constant_speed(int direction, int anti_direction, int PWM_value) {   // function to move the car in constant speed 
	 analogWrite(direction, PWM_value);
	 digitalWrite(anti_direction, LOW);
 }
 
 void move_distance(int direction, int anti_direction, double distance, int speed) { // move certain distance in cm

	 while (distance_moved < distance) {

		 move_constant_speed(direction, anti_direction, speed);
	 }
	 motor_stop();
 }

 void motor_stop(void) {  //stop the car
	 analogWrite(Motor_Forwards, 0);
	 analogWrite(Motor_Backwards, 0);
	 digitalWrite(Motor_Forwards, LOW);
	 digitalWrite(Motor_Backwards, LOW);
 }
 
 //encoder functions
 void encoder_serial_debug(void) {       //print encoder ticks and distance moved values to serial monitor for debugging
	 Serial.print("Encoder Ticks :        ");
	 Serial.print(encoder_ticks);
	 Serial.print("                                    ");
	 Serial.print("Distance Moved:          ");
	 Serial.println(distance_moved);
 }

 void encoder_CHA_counting(void) {   // Channel A ISR

	 if (digitalRead(ENC_CHA) == 1) {  // check the transition in channel A 

		 if (digitalRead(ENC_CHB) == 0) {  //check the direction of car movement
			 encoder_ticks++;
		 }       // The car is moving forwards, hence increment encoder ticks

		 else {
			 encoder_ticks--;
		 }       // The car is moving backwards, hence decrement encoder ticks
	 }

	 else {
		 if (digitalRead(ENC_CHB) == 1) {  //check the direction of car movement
			 encoder_ticks++;
		 }   // The car is moving forwards, hence increment encoder ticks
		 else {
			 encoder_ticks--;
		 }      // The car is moving backwards, hence decrement encoder ticks
	 }
 }

 void encoder_CHB_counting(void) { // Channel B ISR

	 if (digitalRead(ENC_CHB) == 1) {   // check the transition in channel B

		 if (digitalRead(ENC_CHA) == 1) {  //check the direction of car movement
			 encoder_ticks++;
		 }    // The car is moving forwards, hence increment encoder ticks
		 else {
			 encoder_ticks--;
		 }    // The car is moving backwards, hence decrement encoder ticks
	 }

	 else {

		 if (digitalRead(ENC_CHA) == 0) { //check the direction of car movement
			 encoder_ticks++;
		 }   // The car is moving forwards, hence increment encoder ticks

		 else {
			 encoder_ticks--;
		 }     // The car is moving backwards, hence decrement encoder ticks
	 }
 }

 void measure_distance(void) {  //calculate the distance moved by the car
	 distance_moved = (encoder_ticks / (float)ticks_per_revolution) * wheel_circumference;
 }

int get_encoder_ticks(void) {  //return the value of the current encoder ticks
	return encoder_ticks;
}


