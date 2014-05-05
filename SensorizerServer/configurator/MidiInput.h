#ifndef MidiInput_h
#define MidiInput_h


#include "SensorizerServer.h"
#include "Configurator.h"

class MidiInput {

	SensorizerServer* server;


	void updateField(byte sensorIdx, byte fieldIdx, byte value);

	void checkSerial(Stream* input);


	byte dirtySensors[SENSOR_INPUTS_LENGTH];

	/**
	 * Marks the given field for the given sensor as needing to be saved to EEPROM.
	 * @param sensorIdx from MIDI msg
	 * @param fieldIdx  from MIDI msg
	 */
	void setDirty(int sensorIdx, int fieldIdx);

	/**
	 * Saves all dirty fields to the EEPROM.
	 */
	void saveDirty();

	/**
	 * Resets dirty flags for all sensors.
	 */
	void clearDirty();

public:
	MidiInput(SensorizerServer* server);


	/*** 
	run this every loop to check the serial RX.
	***/
	void check();
};

#endif