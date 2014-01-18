#include "TranslatingMidiRepeater.h"

#include "SensorizerServer.h"

TranslatingMidiRepeater::TranslatingMidiRepeater(SensorizerServer* server) {
	MidiRepeater::MidiRepeater();

	this->server = server;
}



void TranslatingMidiRepeater::onSendOutput(byte cmd, byte data1, byte data2) {
    byte idx = server->getSensorIdxForNote(data1);

    if (idx != -1) {
    	// don't send a 0 note. that's creepy
	    MidiRepeater::onSendOutput(cmd, idx + 1, data2);
	}
}