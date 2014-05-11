#ifndef MidiInput_h
#define MidiInput_h


#include "SensorizerServer.h"
#include "Configurator.h"
#include "ConfigurationStore.h"

class MidiInput {

	SensorizerServer* server;

	ConfigurationStore* store;

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