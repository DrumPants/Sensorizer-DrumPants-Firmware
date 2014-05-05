#include "ConfigurationStore.h"

ConfigurationStore::ConfigurationStore() {
	this->clearDirty();
}

void ConfigurationStore::saveSensor(int sensorIdx) {

} 

void ConfigurationStore::loadSensors(SensorizerServer* server){

} 


void ConfigurationStore::setSensor(int sensorIdx, int fieldIdx, float value) {


	this->setDirty(sensorIdx, fieldIdx);
}



void ConfigurationStore::clearDirty() {
	memset(&(this->dirtySensors[0]), 0, sizeof(this->dirtySensors));
}

void ConfigurationStore::setDirty(int sensorIdx, int fieldIdx) {
	this->dirtySensors[sensorIdx] = 1;
}

void ConfigurationStore::saveSensors() {

	for (int i = 0; i < SENSOR_INPUTS_LENGTH; i++) {
		if (this->dirtySensors[i] > 0) {
			// TODO: save to EEPROM
			this->saveSensor(i);
		}
	}
}


void ConfigurationStore::write(int address, byte* data, int dataLength) {
	DEBUG_PRINT_NUMS(F("Virtual writing to config store "), address, dataLength);
}

void ConfigurationStore::read(int address, byte* data, int dataLength) {
	DEBUG_PRINT_NUMS(F("Reading from EEPROM "), address, dataLength);
}