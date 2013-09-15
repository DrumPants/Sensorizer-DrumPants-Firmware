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