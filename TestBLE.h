#ifndef TEST_BLE_H
#define TEST_BLE_H

#include "BLEDevice.h"

#define BLE_PROGRAMMING_TIMEOUT 10000
#define BLE_RESPONSE_TIMEOUT BLE_PROGRAMMING_TIMEOUT / 4

#define PACKET_LEN 4

const uint8_t startPacket[] = {0x01, 0x2E, 0xFC, 0x00};
const uint8_t nextPacket[] = {0x01, 0x03, 0x0C, 0x00};

void ble_writeToBle(const uint8_t* packet) {
  
  Serial.write(startPacket, PACKET_LEN);
  
  SerialToComputer.print("\nBLE Write: ");
  SerialToComputer.write(startPacket, PACKET_LEN);
  SerialToComputer.println(" --");
}

int ble_testBLEProgramming() {
  	

  	int numAttempts = 0;
  	int startTime = millis();
  	while (millis() - startTime < BLE_PROGRAMMING_TIMEOUT) {
		
		// wait for good measure
		delay(10);
		
  		ble_resetAndPrepare(true);

		// wait for good measure
		delay(1000);

		// flush the buffer of whatever's in there before reset
		while(Serial.available()) {		
			Serial.read(); 
	  	}

		ble_writeToBle(startPacket);

		bool isFound = false;
		int waitTime = millis();

  		SerialToComputer.print("\nBLE Read: |");
  		while (millis() - waitTime < BLE_RESPONSE_TIMEOUT) {
			while(Serial.available()) {
				isFound = true;


				SerialToComputer.write(Serial.read()); 

				//writeToBle(nextPacket);
	  		}
	  	}

	  	if (isFound) {
  			SerialToComputer.println("|\nSUCCESS! BLE Responded");
			return 0;
	  	}
	  	else {
  			SerialToComputer.println("\nFAILED! BLE did not respond!");
	  	}

  		numAttempts++;
	}

	return numAttempts;
}


#endif
