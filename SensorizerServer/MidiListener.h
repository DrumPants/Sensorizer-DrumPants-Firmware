

#ifndef MidiListener_h
#define MidiListener_h

#include <Arduino.h>

class MidiListener {
	
public:
	/***
	The listener function. Called by a MidiMapping whenever it sends an output.

	***/
	virtual void onSendOutput(byte cmd, byte data1, byte data2);
};

#endif