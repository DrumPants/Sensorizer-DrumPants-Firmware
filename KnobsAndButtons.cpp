#include "KnobsAndButtons.h"
#include "Metro.h"

#define DEBOUNCE_DELAY 80

#define VOLUME_UP_PIN 27
#define VOLUME_DOWN_PIN 26

// things start to distort at 127 volume.
#define MAX_VOLUME 120
#define NUM_VOLUME_LEVELS 10
#define VOLUME_INC (MAX_VOLUME / NUM_VOLUME_LEVELS)

Metro met;

KnobsAndButtons::KnobsAndButtons() {
	Knobs::Knobs();

	this->setVolume(MAX_VOLUME);

	this->debounceTimer = millis();
	this->downButtonPressed = false;
	this->upButtonPressed = false;

	pinMode(VOLUME_UP_PIN, INPUT);
	pinMode(VOLUME_DOWN_PIN, INPUT);

	met.timePerTick = 200;
}

void KnobsAndButtons::check() {
	Knobs::check();


	bool downPressed = (digitalRead(VOLUME_DOWN_PIN) == LOW);
	bool upPressed = (digitalRead(VOLUME_UP_PIN) == LOW);


	if (downPressed != this->downButtonPressed ||
		upPressed != this->upButtonPressed) {
		
		this->debounceTimer = millis(); // reset timer
	}


	if ((millis() - this->debounceTimer) > DEBOUNCE_DELAY) {

		if (downPressed) {
			this->setVolume(this->volume - VOLUME_INC);
		}
		else if (upPressed) {
			this->setVolume(this->volume + VOLUME_INC);
		}
	}

	this->downButtonPressed = downPressed;
	this->upButtonPressed = upPressed;
}


void KnobsAndButtons::setVolume(byte vol) {
	this->volume = max(0, min(MAX_VOLUME, vol));

	if (met.hasTicked()) {
		this->server->midiDevice->setVolume(MIDI_CHANNEL, this->volume);

		DEBUG_PRINT_NUM("Set volume to ", this->volume);
	}
}
