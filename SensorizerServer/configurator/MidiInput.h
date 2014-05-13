#ifndef MidiInput_h
#define MidiInput_h


#include "SensorizerServer.h"
#include "Configurator.h"
#include "ConfigurationStore.h"

/***
	Time in milliseconds to wait after the last update before saving config to storage.

	This tries to save the EEPROM to write as infrequently as possible. 

	Ten seconds seems legit: long enough for them to decide to be done changing values, 
	but not too long that they might turn off the device before anything was saved.
***/
#define AUTOSAVE_DELAY (10 * 1000)

class MidiInput {

	SensorizerServer* server;

	ConfigurationStore* store;

	/**
	 * System Time in milliseconds of when the last update happened.
	 *
	 * If 0, no updates have been performed.
	 */
	unsigned long lastUpdatedTime;

	/**
	 * Changes configuration on the server as well as saves to EEPROM store.
	 * @param sensorIdx Index of sensor to update, or CHANNEL_COMMAND_SAVE to save to EEPROM (other params are ignored in this case).
	 * @param fieldIdx  [description]
	 * @param value     [description]
	 * @return	true on success, false if sensorIdx doesn't exist or command not recognized.
	 */
	bool updateField(byte sensorIdx, byte fieldIdx, byte value);

	void checkSerial(Stream* input);


public:
	MidiInput(SensorizerServer* server, ConfigurationStore* store);


	/*** 
	run this every loop to check the serial RX.
	***/
	void check();

	/**
	 * Saves the current configuration to EEPROM. 
	 *
	 * This only has effect if something has been written via updateField()
	 */
	void saveConfiguration();

	/**
	 * For testing. Only used by TestConfigurator.h
	 * @param midi the object to test.
	 */
	friend int configurator_testEEPROM(MidiInput* midi, SensorizerServer* server, ConfigurationStore* store);
};

#endif