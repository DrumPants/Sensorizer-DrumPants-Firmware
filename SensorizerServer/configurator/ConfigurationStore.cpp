#include "ConfigurationStore.h"

ConfigurationStore::ConfigurationStore(SensorizerServer* server) {
	this->server = server;
	this->clearDirty();
}

void ConfigurationStore::saveSensor(int sensorIdx) {
	//SensorOutput* sensor = this->server->sensorInputs[sensorIdx];

	// include the first byte, which is the dirty bit (fields start at byte 1)
	byte dataToStore[CONFIGURATOR_FIELDS_END];

	// mark as dirty
	dataToStore[0] = 1;

	for (int fieldIdx = CONFIGURATOR_FIELDS_START; fieldIdx <= CONFIGURATOR_FIELDS_END; fieldIdx++) {

		byte value = Configurator::getFieldValue(server, sensorIdx, fieldIdx);
		dataToStore[fieldIdx] = value;
	}


	int address = sensorIdx * (CONFIGURATOR_FIELDS_END + 1);

	// do page writes
	this->write(address, dataToStore, CONFIGURATOR_FIELDS_END);

} 

void ConfigurationStore::loadSensors(SensorizerServer* server){
	
	for (int sensorIdx = 0; sensorIdx < SENSOR_INPUTS_LENGTH; sensorIdx++) {

		for (int fieldIdx = CONFIGURATOR_FIELDS_START; fieldIdx <= CONFIGURATOR_FIELDS_END; fieldIdx++) {

			// TODO: get value from EEPROM

			if (!Configurator::setField(server, sensorIdx, fieldIdx, value)) {
				DEBUG_PRINT_NUM(F("Failed to save sensor field: ", i));
			}
		}
	}
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


void ConfigurationStore::write(unsigned int address, byte* data, int dataLength) {
	DEBUG_PRINT_NUMS(F("Virtual writing to config store "), address, dataLength);
}

void ConfigurationStore::read(unsigned int address, byte* data, int dataLength) {
	DEBUG_PRINT_NUMS(F("Reading from EEPROM "), address, dataLength);
}