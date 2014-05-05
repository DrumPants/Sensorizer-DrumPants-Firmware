/***
	ConfigurationStore: saves and loads presets from EEPROM memory.
***/

#ifndef ConfigurationStore_h
#define ConfigurationStore_h

#include "SensorizerServer.h"

class ConfigurationStore {

protected:

	virtual void write(int address, byte* data, int dataLength);
	virtual void read(int address, byte* data, int dataLength);

public:
	
	ConfigurationStore();

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

	void loadSensors(SensorizerServer* server);

	/**
	 * Saves all dirty sensors to EEPROM. 
	 *
	 * This just calls saveSensor() for all dirty sensors.
	 */
	void saveSensors();

private: 


	byte dirtySensors[SENSOR_INPUTS_LENGTH];

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