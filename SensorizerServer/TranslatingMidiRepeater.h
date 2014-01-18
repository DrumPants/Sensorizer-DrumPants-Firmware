#ifndef TranslatingMidiRepeater_h
#define TranslatingMidiRepeater_h

#include "MidiRepeater.h"

class SensorizerServer;

/***

	Repeats all Midi messages sent from a MidiDevice to the BLE chip,
	translating the notes into static sensor indexes using the current scale.

***/
class TranslatingMidiRepeater : public MidiRepeater {

	SensorizerServer* server;

public: 

	TranslatingMidiRepeater(SensorizerServer* server);

	/***
	The listener function. Called by a sensor input whenever it sends an output.

	***/
	void onSendOutput(byte cmd, byte data1, byte data2);

};


#endif