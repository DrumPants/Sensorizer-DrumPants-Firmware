#ifndef MidiInput_h
#define MidiInput_h


#include "SensorizerServer.h"
#include "Configurator.h"
#include "ConfigurationStore.h"

class MidiInput {

	SensorizerServer* server;

	ConfigurationStore* store;

	void updateField(byte sensorIdx, byte fieldIdx, byte value);

	void checkSerial(Stream* input);


public:
	MidiInput(SensorizerServer* server, ConfigurationStore* store);


	/*** 
	run this every loop to check the serial RX.
	***/
	void check();
};

#endif