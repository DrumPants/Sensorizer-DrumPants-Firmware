/**
 * Copyright 2011-2015 DrumPants, Inc.
 * 
 * http://developers.drumpants.com
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
#include "../MidiListener.h"

#define LOOP_LENGTH_IN_SECONDS 8
#define LOOP_LENGTH_IN_TICKS 512
#define MAX_LOOP_LAYERS 32

class EventLooper : public MidiListener {

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
