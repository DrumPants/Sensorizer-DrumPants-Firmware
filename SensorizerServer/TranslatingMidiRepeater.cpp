#include "TranslatingMidiRepeater.h"

#include "SensorizerServer.h"

TranslatingMidiRepeater::TranslatingMidiRepeater(SensorizerServer* server) {
	MidiRepeater();

	this->server = server;
}



void TranslatingMidiRepeater::onSendOutput(byte cmd, byte data1, byte data2) {
    int idx = server->getSensorIdxForNote(data1);

    if (idx != -1) {
    	// send a 0 note. sure, why not? it's creepy, but whatevs
	    MidiRepeater::onSendOutput(cmd, idx, data2);
	}
	else {
		DEBUG_PRINT("Failed to find sensor index");
	}
}