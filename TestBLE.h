#ifndef TEST_BLE_H
#define TEST_BLE_H

#include "BLEDevice.h"

#define BLE_PROGRAMMING_TIMEOUT 10000

#define PACKET_LEN 4

const uint8_t startPacket[] = {0x01, 0x2E, 0xFC, 0x00};
const uint8_t nextPacket[] = {0x01, 0x03, 0x0C, 0x00};

void ble_writeToBle(const uint8_t* packet) {
  
  Serial.write(startPacket, PACKET_LEN);
  
  SerialToComputer.print("\nWrite: ");
  SerialToComputer.write(startPacket, PACKET_LEN);
  SerialToComputer.println(" --");
}

int ble_testBLEProgramming() {
  	

  	int numAttempts = 0;
  	int startTime = millis();
  	while (millis() - startTime < BLE_PROGRAMMING_TIMEOUT) {
		
		// wait for good measure
		delay(10);
		
  		ble_resetAndPrepareForProgramming();

		// wait for good measure
		delay(1000);

		ble_writeToBle(startPacket);

		while(Serial.available()) {
			SerialToComputer.write(Serial.read()); 

			return 0;
			//writeToBle(nextPacket);
  		}

  		numAttempts++;
	}

	return numAttempts;
}


#endif
