#include "TranslatingMidiRepeater.h"

#include "SensorizerServer.h"


TranslatingMidiRepeater::TranslatingMidiRepeater(SensorizerServer* server) {
	MidiRepeater();

	this->server = server;
}



void TranslatingMidiRepeater::onSendOutput(byte cmd, byte data1, byte data2) {
    int idx;
    if (ENABLE_SEND_METRONOME_TO_USB_AND_BLE &&
    	((cmd & 0x0F) == METRONOME_MIDI_CHANNEL)) { // allow metronome to send (for testing)

    	// prepare for USB to get it.
    	idx = 1; // send a phony index just so it will sound in the app.
    	cmd = (cmd & 0xF0) | MIDI_CHANNEL;
    } 
    else {
    	idx = server->getSensorIdxForNote(data1);
    }

    if (idx != -1) {
    	// send a 0 note. sure, why not? it's creepy, but whatevs
	    MidiRepeater::onSendOutput(cmd, idx, data2);
	}
	else {
		DEBUG_PRINT("Failed to find sensor index");
	}
}