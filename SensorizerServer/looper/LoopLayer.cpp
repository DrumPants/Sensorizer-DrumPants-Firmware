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

#include "LoopLayer.h"

/**
	Returns the current event, or null if there are no more events.
**/
MidiEvent* LoopLayer::getEvent() {

	if (this->curEventIdx < this->lastEventIdx) {
		return this->events[this->curEventIdx];
	}

	return NULL;
}

/**
	Moves the current event pointer to the next event.

	@returns itself for chaining
**/
LoopLayer* LoopLayer::moveToNext() {

	this->curEventIdx++;

	return this;
}

/**
	Moves the current event pointer to the beginning of the list.

	@returns itself for chaining
**/
LoopLayer* LoopLayer::moveToStart() {
	
	this->curEventIdx = 0;

	return this;
}


/***
	Adds an event to the end of the list.

	Note you can never add an event that has happened before the last one in the list! 
	Make a new layer if you want to overdub!

	@returns true if event was successfully added, false if there's no more room
**/
bool LoopLayer::addEvent(MidiEvent* event) {

	if (this->lastEventIdx + 1 < LOOPLAYER_MAX_EVENTS) {
		
		this->events[++this->lastEventIdx] = event;

		return true;
	}
	else {
		return false;
	}
}