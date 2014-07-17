#ifndef MidiRepeater_h
#define MidiRepeater_h

#include <Arduino.h>
#include "Constants.h" 
#include "Debug.h"

#include "MidiListener.h"

/***

	Repeats all Midi messages sent from a MidiDevice to the BLE chip.

***/
class MidiRepeater : public MidiListener {

public: 

	MidiRepeater();

	/***
	The listener function. Called by a sensor input whenever it sends an output.

	***/
	virtual void onSendOutput(byte cmd, byte data1, byte data2);

protected:
	void writeToSerial(byte data);
};


#endif