/***
	EVENT LOOPER

	Listens for sensor inputs and records them to be looped.

***/


#ifndef EventLooper_h
#define EventLooper_h

#include <Arduino.h>
#include "MidiEvent.h"
#include "LoopLayer.h"

#include "../MidiDevice.h"

#define LOOP_LENGTH_IN_SECONDS 8
#define LOOP_LENGTH_IN_TICKS 512
#define MAX_LOOP_LAYERS 32

class EventLooper {

	LoopLayer* layers[MAX_LOOP_LAYERS];

	// keeps track of the last added layer
	byte curLayerIdx;

	LoopLayer* addNewLayer(); 

	MidiDevice* device;

	int curTick;

public: 
	EventLooper() {
		this->curLayerIdx = 0;
	}

	~EventLooper();

	void setOutputDevice(MidiDevice* d);
	void send(MidiEvent* ev);

	/***
	The listener function. Called by a sensor input whenever it sends an output.

	***/
	void onSendOutput(byte cmd, byte data1, byte data2);
	
	void tick(int curTime);

	/**
		Called when the tick reaches the end of playback and loops to the beginning.
	**/
	void onLoop();

};

#endif
