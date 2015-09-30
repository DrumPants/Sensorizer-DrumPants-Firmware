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

#include "EventLooper.h"

EventLooper::~EventLooper() {
	for(int i = 0; i <= this->curLayerIdx ; i++) {
		LoopLayer* l = this->layers[i];

		if (l != NULL) {

			MidiEvent* ev;
			l->moveToStart();
			while ((ev = l->getEvent()) != NULL) {
				MidiEvent::destroyEvent(ev);

				l->moveToNext();
			}

			delete l;
		}
	}
}


void EventLooper::setOutputDevice(MidiDevice* d) {
	this->device = d;
}

void EventLooper::send(MidiEvent* ev) {
	this->device->talkMIDI(ev->cmd, ev->data1, ev->data2, true); // send as silent so we don't give ourselves an infinite loop
}

/***
	The listener function. Called by a sensor input whenever it sends an output.

***/
void EventLooper::onSendOutput(byte cmd, byte data1, byte data2) {
	MidiEvent* ev = MidiEvent::createEvent(this->curTick, cmd, data1, data2);

	LoopLayer* curLayer = this->layers[this->curLayerIdx];
	if (!curLayer->addEvent(ev)) { // the layer must be full, add a new one
		curLayer = addNewLayer();
		curLayer->addEvent(ev);
	};
}

void EventLooper::tick(int curTime) {

	if (++this->curTick > LOOP_LENGTH_IN_TICKS) {
		this->onLoop();
	}

	// play all notes in each layer that match up with this tick time
	for(int i = 0; i <= this->curLayerIdx; i++) {
		LoopLayer* l = this->layers[i];

		MidiEvent* ev = l->getEvent();

		while (ev != NULL && ev->tickTime <= curTick) {
			this->send(ev);

			l->moveToNext();
			ev = l->getEvent();
		}
	}

	//this->lastTickTime = curTime;
}



void EventLooper::onLoop() {
	this->curTick = 0;

	// reset all layers back to beginning
	for(int i = 0; i <= this->curLayerIdx ; i++) {
		this->layers[i]->moveToStart();
	}

	// move to next layer since we can't record over a layer
	addNewLayer();
}


LoopLayer* EventLooper::addNewLayer() {
	return this->layers[++this->curLayerIdx] = new LoopLayer();
}