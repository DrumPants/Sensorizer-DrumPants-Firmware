#include "KnobsAndButtons.h"
#include "Metro.h"

#define DEBOUNCE_DELAY 110

#define VOLUME_UP_PIN 27
#define VOLUME_DOWN_PIN 26

// things start to distort at 127 volume.
#define MAX_VOLUME 120

#define DEFAULT_VOLUME 90

// TURN IT UP TO ELEVEN!
#define NUM_VOLUME_LEVELS 11
#define VOLUME_INC (MAX_VOLUME / NUM_VOLUME_LEVELS)

Metro met;

KnobsAndButtons::KnobsAndButtons() {
	Knobs();

	this->setVolume(DEFAULT_VOLUME);

	this->debounceTimer = millis();
	this->downButtonPressed = false;
	this->upButtonPressed = false;

	pinMode(VOLUME_UP_PIN, INPUT);
	pinMode(VOLUME_DOWN_PIN, INPUT);

	met.timePerTick = 80;
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
			//DEBUG_PRINT("down pressed");
			this->setVolume((int)(this->volume) - VOLUME_INC);
		}
		else if (upPressed) {
			//DEBUG_PRINT("up pressed");
			this->setVolume((int)(this->volume) + VOLUME_INC);
		}
	}

	this->downButtonPressed = downPressed;
	this->upButtonPressed = upPressed;
}


void KnobsAndButtons::setVolume(int vol) {

	if (met.hasTicked()) {
		this->volume = max(0, min(MAX_VOLUME, vol));

		this->server->midiDevice->setVolume(MIDI_CHANNEL, this->volume);

		DEBUG_PRINT_NUM("Set volume to ", this->volume);

		this->lcd.showTemporarily(min(NUM_VOLUME_LEVELS, this->volume / VOLUME_INC)); // use min as a makeshift floor function
	}
}
