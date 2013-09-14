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