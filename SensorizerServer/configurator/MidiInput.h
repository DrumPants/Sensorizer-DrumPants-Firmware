#ifndef MidiInput_h
#define MidiInput_h


#include "SensorizerServer.h"

class MidiInput {

	SensorizerServer* server;


public:
	MidiInput(SensorizerServer* server);


	/*** 
	run this every loop to check the serial RX.
	***/
	void check();
};

#endif