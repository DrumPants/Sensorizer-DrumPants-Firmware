#ifndef PendingNoteQueue_h
#define PendingNoteQueue_h

#include <Arduino.h>
#include "Constants.h" 

#include "MidiDevice.h" 

// the number of ticks a note has to live before its corresponding noteOff is sent.
#define NOTE_TIME_TILL_DEATH_DEFAULT 1024

// let's hope they can't play more than 32 notes at a time (per sensor) before we can turn them back off
#define MAX_PENDING_NOTES 32

// this holds note offs that are waiting to be sent.
class PendingNote {

public:
	byte channel;
	byte note;
	int timeTillDeath;

	PendingNote() {
		channel = MIDI_CHANNEL;
		note = 60;
		timeTillDeath = NOTE_TIME_TILL_DEATH_DEFAULT;
	}

	PendingNote* tick() {
		if (timeTillDeath > 0)
			--timeTillDeath;

		return this;
	};

	bool isDead() {
		return (timeTillDeath <= 0); 
	};
};



struct PendingNoteQueue {
	PendingNote pendingNotes[MAX_PENDING_NOTES];

	byte startIdx, endIdx;

	MidiDevice* midi;


	void checkNote(byte noteIdx) {
		PendingNote* note = &(pendingNotes[noteIdx]);

		if (!note->isDead() && note->tick()
				->isDead()) {

			midi->noteOff(note->channel, note->note, 0); //127, 127);//

			// free this one up to be reused.
			// we assume that these are added in order
			startIdx = (noteIdx + 1) % MAX_PENDING_NOTES;
		}
	}

public: 

	PendingNoteQueue(MidiDevice* device) {
		startIdx = 0;
		endIdx = 0;
		midi = device;
	};

	void tick() {
		if (endIdx < startIdx) { // it wrapped, check both sides
			for (byte i = startIdx; i < MAX_PENDING_NOTES; i++) {
				checkNote(i);
			}
			for (byte i = 0; i < endIdx; i++) {
				checkNote(i);
			}
		}
		else {
			for (byte i = startIdx; i < endIdx; i++) {
				checkNote(i);
			}
		}


	};	

	void add(byte channel, byte note) {

		// first look for an existing note, and just extend that one's time.
		// this way, when the hit the same sensor a bunch, the first hit doesn't cut off the subsequent ones.
		// this fixes the choppy sound bug.
		for (int i = endIdx - 1; i >= startIdx; i--) {
			if (pendingNotes[i].note == note && 
				pendingNotes[i].channel == channel) {

				pendingNotes[i].timeTillDeath = NOTE_TIME_TILL_DEATH_DEFAULT;		
				return;
			}
		}

		pendingNotes[endIdx].channel = channel;
		pendingNotes[endIdx].note = note;
		pendingNotes[endIdx].timeTillDeath = NOTE_TIME_TILL_DEATH_DEFAULT;

		// for now, we don't care if we run over other stuff.
		// TODO: auto-kill things when we run out of space!
		endIdx = (endIdx + 1) % MAX_PENDING_NOTES;
	}
};

#endif