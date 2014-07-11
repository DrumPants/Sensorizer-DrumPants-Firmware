#include "SensorJackSwitches.h"

#include <Arduino.h>
#include <Constants.h>  
#include <Debug.h>


#if PRESET >= PRESET_PREPRODUCTION_RED
// J2
#	define JACK_PIN_IDX_A 42
// J3
#	define JACK_PIN_IDX_B 43
// J5 (yes it's out of order!)
#	define JACK_PIN_IDX_C 50
// J4 (yes it's out of order!)
#	define JACK_PIN_IDX_D 49
#endif


void initSensorJackSwitches() {

#ifdef JACK_PIN_IDX_A
	pinMode(JACK_PIN_IDX_A, INPUT);
	pinMode(JACK_PIN_IDX_B, INPUT);
	pinMode(JACK_PIN_IDX_C, INPUT);
	pinMode(JACK_PIN_IDX_D, INPUT);
#endif 

}

/**
 * 
	Checks the switch on a sensor jack to determine if a sensor is actually plugged in.
	Use for disabling inputs that aren't attached to something, to avoid floating input data.
 * @param  analogPinIdx The index of the analog pin the sensor is connected to. The correct switch will be checked for that jack.
 * @return              True if sensor is connected (or switch-checking not supported), false if nothing plugged into that jack.
 */
bool isSensorPluggedIn(int analogPinIdx) {

#ifdef JACK_PIN_IDX_A	
	// do this the super efficient, slightly icky way
	int jackToRead = -1;

	switch (analogPinIdx) {
		// Jack A
		case 0:  // fall-through!
		case 4:  // fall-through!
		case 8:  // fall-through!
			jackToRead = JACK_PIN_IDX_A;
			break;

		// Jack B
		case 1:  // fall-through!
		case 5:  // fall-through!
		case 9:  // fall-through!
			jackToRead = JACK_PIN_IDX_B;
			break;

		// Jack C
		case 2:  // fall-through!
		case 6:  // fall-through!
		case 10:  // fall-through!
			jackToRead = JACK_PIN_IDX_C;
			break;

		// Jack D
		case 3:  // fall-through!
		case 7:  // fall-through!
		case 11:  // fall-through!
			jackToRead = JACK_PIN_IDX_D;
			break;
	}

	// TODO: optimize by caching the read each tick.
	if (jackToRead >= 0 && digitalRead(jackToRead) == LOW) {
		return false;
	}

#endif

	return true;
}