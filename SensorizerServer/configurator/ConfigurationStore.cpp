/**
 * Copyright 2011-2015 DrumPants, Inc.
 * 
 * http://developers.drumpants.com
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ConfigurationStore.h"
#include "Configurator.h"

#include "Constants.h"

#define CONFIGURATOR_FIELDS_LENGTH (CONFIGURATOR_FIELDS_END + 1)

#define GET_ADDRESS_FOR_SENSOR(sensorIdx) (sensorIdx * CONFIGURATOR_FIELDS_LENGTH)

#if EEPROM_SIZE_IN_KBITS < 4
	// for the first betas, we don't have enough room to save all the sensors. only save half of them (plus metadata)
	#define SENSOR_INPUTS_TO_SAVE_LENGTH (CONFIGURATOR_SENSOR_INPUTS_LENGTH / 2 + 1)
#else 
	#define SENSOR_INPUTS_TO_SAVE_LENGTH CONFIGURATOR_SENSOR_INPUTS_LENGTH
#endif

ConfigurationStore::ConfigurationStore(SensorizerServer* server) {
	this->server = server;
	this->clearDirty();
}

void ConfigurationStore::saveSensor(int sensorIdx) {
	//SensorOutput* sensor = this->server->sensorInputs[sensorIdx];

	// include the first byte, which is the dirty bit (fields start at byte 1)
	byte dataToStore[CONFIGURATOR_FIELDS_LENGTH];

	// mark as dirty with the current file version number so we don't try to load from an earlier version.
	dataToStore[CONFIGURATOR_VERSION_FIELD_IDX] = CONFIGURATOR_VERSION;

	for (int fieldIdx = CONFIGURATOR_FIELDS_START; fieldIdx <= CONFIGURATOR_FIELDS_END; fieldIdx++) {

		byte value = Configurator::getField(this->server, sensorIdx, fieldIdx);
		dataToStore[fieldIdx] = value;
	}


	int address = GET_ADDRESS_FOR_SENSOR(sensorIdx);

	// do page writes
	this->write(address, dataToStore, CONFIGURATOR_FIELDS_LENGTH);

} 

void ConfigurationStore::loadSensors(){
	
	for (int sensorIdx = 0; sensorIdx < SENSOR_INPUTS_TO_SAVE_LENGTH; sensorIdx++) {

		// include the first byte, which is the dirty bit (fields start at byte 1)
		byte dataFromStore[CONFIGURATOR_FIELDS_LENGTH];

		// get value from EEPROM
		this->read(GET_ADDRESS_FOR_SENSOR(sensorIdx), dataFromStore, CONFIGURATOR_FIELDS_LENGTH);

		// only load the values if the user has actually saved something there: check version also so we don't load bad data 
		if (dataFromStore[CONFIGURATOR_VERSION_FIELD_IDX] == CONFIGURATOR_VERSION) {
			for (int fieldIdx = CONFIGURATOR_FIELDS_START; fieldIdx <= CONFIGURATOR_FIELDS_END; fieldIdx++) {
				byte value = dataFromStore[fieldIdx];

				if (!Configurator::setField(this->server, sensorIdx, fieldIdx, value)) {
					DEBUG_PRINT_NUMS("Failed to save sensor field: ", sensorIdx, fieldIdx);
				}
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

	for (int i = 0; i < SENSOR_INPUTS_TO_SAVE_LENGTH; i++) {
		if (this->dirtySensors[i] > 0) {
			// TODO: save to EEPROM
			this->saveSensor(i);
		}
	}

	this->clearDirty();
}


void ConfigurationStore::write(unsigned int address, byte* data, int dataLength) {
	DEBUG_PRINT_NUMS("Virtual writing to config store ", address, dataLength);
}

void ConfigurationStore::read(unsigned int address, byte* data, int dataLength) {
	DEBUG_PRINT_NUMS("Virtual Reading from EEPROM ", address, dataLength);
}


void ConfigurationStore::eraseAllAndReset() {
	DEBUG_PRINT("Virtual Clearing all EEPROM memory!");
}