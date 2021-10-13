#include "BLE.h"

void ble_setup(void) {  // Function to initialise the communication of BLE 
	
	while (!BLE.begin());
	delay(10);
	BLE.setLocalName("Track Timer");  // set name to be advertised
	BLE.setAdvertisedService(Track_timer);  // Advertise name through BLE
	Track_timer.addCharacteristic(lap_time); // Add characterstic to service
	BLE.addService(Track_timer);  
	lap_time.writeValue(0); // initialise value of lap time characterisitc
	BLE.advertise(); 
}

void send_time(float current_time) {  //Send lap time through BLE communications 

	BLEDevice central = BLE.central();
		if (central.connected()) { //check if a device is connected to service
			int time_in_decimal = (int)(current_time * 1000);  //convert time to integer before sending
			delay(20);
			lap_time.writeValue(time_in_decimal); //send lap time 
		}
}

bool ble_connected(void) {    // function to check if a device is connected via bluetooth

	BLEDevice central = BLE.central();
	if (central) {
		return true;
	}
	else {
		return false;
	}
}
