#ifndef MidiInput_h
#define MidiInput_h


#include "SensorizerServer.h"
#include "Configurator.h"

class MidiInput {

	SensorizerServer* server;


	void updateField(byte sensorIdx, byte fieldIdx, byte value);

	void checkSerial(Stream* input);

public:
	MidiInput(SensorizerServer* server);


	/*** 
	run this every loop to check the serial RX.
	***/
	void check();
};

#endif