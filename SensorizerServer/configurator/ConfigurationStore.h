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

/***
	ConfigurationStore: saves and loads presets from EEPROM memory.
***/

#ifndef ConfigurationStore_h
#define ConfigurationStore_h

#include "SensorizerServer.h"
#include "Configurator.h"

class ConfigurationStore {

protected:

	virtual void write(unsigned int address, byte* data, int dataLength);
	virtual void read(unsigned int address, byte* data, int dataLength);

public:
	
	ConfigurationStore(SensorizerServer* server);

	/**
	 * Sets the data for a given sensor to be saved. 
	 *
	 * WON'T BE SAVED UNTIL YOU CALL saveSensor()
	 * @param sensorIdx [description]
	 * @param fieldIdx  [description]
	 * @param value     [description]
	 */
	void setSensor(int sensorIdx, int fieldIdx, float value);

	/**
	 * Actually flushes changes to the memory.
	 * 
	 * @param sensorIdx [description]
	 */
	void saveSensor(int sensorIdx);

	void loadSensors();

	/**
	 * Saves all dirty sensors to EEPROM. 
	 *
	 * This just calls saveSensor() for all dirty sensors.
	 */
	void saveSensors();

	/**
	 * Wipes the configuration memory and resets to factory defaults.
	 */
	virtual void eraseAllAndReset();

private: 

	SensorizerServer* server;

	byte dirtySensors[CONFIGURATOR_SENSOR_INPUTS_LENGTH];

	/**
	 * Marks the given field for the given sensor as needing to be saved to EEPROM.
	 * @param sensorIdx from MIDI msg
	 * @param fieldIdx  from MIDI msg
	 */
	void setDirty(int sensorIdx, int fieldIdx);


	/**
	 * Resets dirty flags for all sensors.
	 */
	void clearDirty();

};


#endif