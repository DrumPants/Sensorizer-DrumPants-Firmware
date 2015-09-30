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

#ifndef MidiEvent_h
#define MidiEvent_h

#include <Arduino.h>

struct MidiEvent {
	int tickTime;
	byte cmd, data1, data2;

	/**
		Returns a new MidiEvent from the pool.

		DO NOT delete IT YOURSELF. You must call destroyEvent() when done with it.
	**/
	static MidiEvent* createEvent(int curTick, byte cmd, byte data1, byte data2) {
		MidiEvent* res = new MidiEvent();

		res->tickTime = curTick;
		res->cmd = cmd;
		res->data1 = data1;
		res->data2 = data2;

		return res;
	}

	/**
	 * Reclaims the memory for an event created with createEvent().
	 *
	 * Note it may not actually be reclaimed, but just shoved back in the pool.
	 * 
	 * @param event the event to free.
	 */
	static void destroyEvent(MidiEvent* event) {
		delete event;
	}
};

#endif