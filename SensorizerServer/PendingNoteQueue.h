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

#ifndef PendingNoteQueue_h
#define PendingNoteQueue_h

#include <Arduino.h>
#include "Constants.h" 

#include "MidiDevice.h" 

// the number of ticks a note has to live before its corresponding noteOff is sent.
#define NOTE_TIME_TILL_DEATH_DEFAULT MIDI_NOTE_DURATION

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

			sendNoteOff(note);

			// free this one up to be reused.
			// we assume that these are added in order
			startIdx = (noteIdx + 1) % MAX_PENDING_NOTES;
		}
	}


	void sendNoteOff(PendingNote* note) {
		midi->noteOff(note->channel, note->note, 0); //127, 127);//
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

	void add(byte channel, byte note, int duration = NOTE_TIME_TILL_DEATH_DEFAULT) {

		// first look for an existing note, and just extend that one's time.
		// this way, when the hit the same sensor a bunch, the first hit doesn't cut off the subsequent ones.
		// this fixes the choppy sound bug.
		for (int i = endIdx - 1; i >= startIdx; i--) {
			if (pendingNotes[i].note == note && 
				pendingNotes[i].channel == channel) {

				// finish off the currently playing note to make room for the next one.
				// this is what the MIDI spec recommends, although not has weird effects on some devices.
				sendNoteOff(&(pendingNotes[i]));

				pendingNotes[i].timeTillDeath = duration;		
				return;
			}
		}

		pendingNotes[endIdx].channel = channel;
		pendingNotes[endIdx].note = note;
		pendingNotes[endIdx].timeTillDeath = duration;

		// for now, we don't care if we run over other stuff.
		// TODO: auto-kill things when we run out of space!
		endIdx = (endIdx + 1) % MAX_PENDING_NOTES;
	}
};

#endif