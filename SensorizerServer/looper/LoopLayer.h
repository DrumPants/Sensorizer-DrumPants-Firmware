#ifndef LoopLayer_h
#define LoopLayer_h

#include "MidiEvent.h"

// maximum accessible by a byte
#define LOOPLAYER_MAX_EVENTS 128

class LoopLayer {
	MidiEvent* events[LOOPLAYER_MAX_EVENTS];

	// TOOD: this should be byte if possible, but will nedd overflow detection in getEvent()
	int curEventIdx, lastEventIdx;


public: 

	LoopLayer() {
		this->curEventIdx = 0;
		this->lastEventIdx = 0;
	}

	//int getEventsCount();

	/**
		Returns the current event, or null if there are no more events.
	**/
	MidiEvent* getEvent();

	/***
		Adds an event to the end of the list.

		Note you can never add an event that has happened before the last one in the list! 
		Make a new layer if you want to overdub!

		@returns true if event was successfully added, false if there's no more room
	**/
	bool addEvent(MidiEvent* event);

	/**
		Moves the current event pointer to the next event.

		@returns itself for chaining
	**/
	LoopLayer* moveToNext();

	/**
		Moves the current event pointer to the beginning of the list.

		@returns itself for chaining
	**/
	LoopLayer* moveToStart();
};

#endif