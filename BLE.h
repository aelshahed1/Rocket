#pragma once
#include "pins.h" //include pins header file

//Bluetooth low energy  functions prototypes
void ble_setup(void);  
void send_time(float current_time);
bool ble_connected(void);